#include <string>
#include <thread>
#include <atomic>

std::atomic<int> sharedValue(0);
std::atomic<bool> ready(false);

void IncrementSharedValue10000000Times() {
    int count = 0;
    while (count < 10000000) {
        sharedValue.fetch_add(1, std::memory_order_relaxed);
        count++;
    }
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

int main(int argc, char *argv[]) {
    sharedValue = 0;
    std::thread thread1(IncrementSharedValue10000000Times);
    std::thread thread2(IncrementSharedValue10000000Times);
    thread1.join();
    thread2.join();
    printf("sharedValue=%d \n", sharedValue.load());
    return 0;
}