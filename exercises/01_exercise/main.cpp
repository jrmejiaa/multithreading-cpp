#include "stack/stack-threadsafe.hpp"

int main() {
    threadsafe_stack<int> tss_int{};
    int value = 5;

    tss_int.push(value);
    tss_int.push(std::move(value));

    std::array<std::unique_ptr<int>, 5> arr{};
    tss_int.push(std::move(arr));

    auto size = tss_int.size<int>();

    std::cout << "The size of Stack is: " << size << "\n" << std::endl;


    multiple_copy mc{};

    threadsafe_stack<multiple_copy> tss_multiple_copy{};
    tss_multiple_copy.push(mc);
    tss_multiple_copy.push(std::move(mc));

    tss_multiple_copy.pop(mc);

    no_copy nc{};

/*    threadsafe_stack<no_copy> tss_no_copy{};
    tss_no_copy.push(nc);
    tss_no_copy.push(std::move(nc));


    no_move nm{};

    threadsafe_stack<no_move> tss_no_move{};
    tss_no_move.push(nm);
    tss_no_move.push(std::move(nm));


    no_copy_no_move ncnm{};

    threadsafe_stack<no_copy_no_move> tss_no_copy_no_move{};
    tss_no_copy_no_move.push(ncnm);
    tss_no_copy_no_move.push(std::move(ncnm));


    copy_exception nmce{};

    threadsafe_stack<copy_exception> tss_copy_exception{};
    tss_copy_exception.push(nmce);
    tss_copy_exception.push(std::move(nmce));


    move_exception ncme{};

    threadsafe_stack<move_exception> tss_move_exception{};
    tss_move_exception.push(ncme);
    tss_move_exception.push(std::move(ncme));*/

    return 0;
}
