#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::atomic<int> sharedValue(0);
std::atomic<bool> ready(false);

void IncrementSharedValue10000000Times ( )
{
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return ready.load();});
    ready.store(false);

    int count = 0;
    while ( count < 10000000)
    {
        sharedValue.store(sharedValue.load() + 1);
        count++;
    }

    // Unlock and heads up the thread that it is open
    lk.unlock();
    ready.store(true);
    cv.notify_one();
}
/*
 * Questions about the code
 *
 * a) What would be the expected output of this program?
 *      R/ The expected output of the program would be 20000000 in the
 *      sharedValue variable after both threads has finished.
 * a.1) Is it consistent between executions? If not, give a reason why.
 *      R/ No, every time that the code is executed, there are problems of consistency with the variable, because
 *      there are two threads trying to access the same global variable at the same time and there are no memory
 *      consistency
*/

int main (int argc , char * argv [] )
{
    sharedValue = 0;
    std::thread thread1 (IncrementSharedValue10000000Times);
    std::thread thread2 (IncrementSharedValue10000000Times);
    ready.store(true);
    cv.notify_one();
    thread1.join();
    thread2.join() ;
    printf("sharedValue=%d \n", sharedValue.load()) ;
    return 0;
}