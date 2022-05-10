#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

std::string data;
std::atomic_flag ready = false;
std::atomic_flag processed = false;

void worker_thread() {
    // Wait until main() sends data
    ready.wait(false);

    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed.test_and_set();
    processed.notify_one();
    std::cout << "Worker thread signals data processing completed\n";
}

int main() {
    std::thread worker(worker_thread);

    data = "Example data";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // send data to the worker thread
    {
        ready.test_and_set();
        ready.notify_one();
        std::cout << "main() signals data ready for processing\n";
    }

    // wait for the worker
    {
        std::cout << "Wait until finish to process the data\n";
        processed.wait(false);
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}