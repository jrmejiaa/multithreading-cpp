#include <iostream>
#include <thread>
#include <vector>
#include "lib/utility.h"

int main(int argc, char **argv) {
    const int number_of_threads = 10;
    uint64_t number_of_elements = 1000 * 1000* 1000;
    uint64_t step = number_of_elements / number_of_threads;
    std::vector<std::thread> threads;
    std::vector<uint64_t> partial_sums(number_of_threads);

    for (uint64_t i = 0; i < number_of_threads; i++) {
        // It is necessary to use the std::ref because when I want to pass parameters to the thread, it would
        // always make a copy of the value.
        threads.emplace_back(AccumulateRange, std::ref(partial_sums[i]),
                                      i * step, (i + 1) * step);
    }

    for (std::thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    uint64_t total =
            std::accumulate(partial_sums.begin(), partial_sums.end(), uint64_t(0));

    PrintVector(partial_sums);
    std::cout << "total: " << total << std::endl;

    return 0;
}
