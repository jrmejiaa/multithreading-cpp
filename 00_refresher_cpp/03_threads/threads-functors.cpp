#include <iostream>
#include <thread>
#include <vector>
#include "lib/utility.h"

int main(int argc, char **argv) {
    const int number_of_threads = 10;
    uint64_t number_of_elements = 1000 * 1000* 1000;
    uint64_t step = number_of_elements / number_of_threads;
    std::vector<std::thread> threads;
    std::vector<AccumulateFunctor *> v_functors;

    for (uint64_t i = 0; i < number_of_threads; i++) {
        // This would overload the operator to be able to use that as a function, and we can retrieve the value from an
        // attribute of the class.
        auto *accumulateFunctor = new AccumulateFunctor();
        threads.emplace_back(std::ref(*accumulateFunctor), i*step, (i+1) * step);
        v_functors.emplace_back(accumulateFunctor);
    }

    for (std::thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    uint64_t total = 0;
    for (auto functor : v_functors){
        total += functor->_sum;
    }

    std::cout << "total: " << total << std::endl;

    return 0;
}
