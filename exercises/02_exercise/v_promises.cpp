#include <iostream>
#include <thread>
#include <future>
#include <vector>

#define NUMBER_OF_THREADS 5

void worker_thread(const std::shared_future<bool> &share_future) {
    share_future.wait();
    std::cout << "Hello World from concurrent world\n";
}

int main() {
    std::promise<bool> processed;
    std::shared_future<bool> share_future = processed.get_future().share();
    std::vector<std::thread> threads(NUMBER_OF_THREADS);

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        threads.emplace_back(worker_thread, std::ref(share_future));
    }

    std::cout << "main() set signal for processing\n";
    processed.set_value(true);

    for (auto &t: threads) {
        t.join();
    }

    return 0;
}