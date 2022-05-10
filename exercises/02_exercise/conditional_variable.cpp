#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread() {
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return ready;});

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

int main()
{
    /*
     * Conditional variables are prone to spurious and lost wakeup. A spurious wakeup can happen, when the receiver finished
     * its task before the sender has sent its notification. The receiver is susceptible for spurious wakeup. Therefore, the
     * receiver wakes up, although no notification has been sent. A lost wakeup happens when the sender sends its notification
     * before the receiver gets to a wait state. In this case the notification gets lost.
     *
     * This could be solved using std::atomic flags, std::atomic flags with conditional variables or a combination between
     * promises/futures in order to create an order of execution and ensure that the condition is always fulfill.
     * */
    std::thread worker(worker_thread);

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}