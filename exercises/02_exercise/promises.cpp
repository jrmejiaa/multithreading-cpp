#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <chrono>

std::string data;

void worker_thread(std::promise<bool> processed) {
    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    processed.set_value(true);
    std::cout << "Worker thread signals data processing completed\n";
}

int main() {
    std::promise<bool> processed;
    std::future<bool> processed_future = processed.get_future();
    std::thread worker(worker_thread, std::move(processed));

    data = "Example data";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "main() signals data ready for processing\n";

    // wait for the worker
    {
        std::cout << "Wait until finish to process the data\n";
        processed_future.get();
    }
    std::cout << "Back in main(), data = " << data << '\n';

    worker.join();
}