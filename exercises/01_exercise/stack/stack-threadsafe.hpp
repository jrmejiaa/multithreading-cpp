#ifndef INC_01_STACK_THREADSAFE_HPP
#define INC_01_STACK_THREADSAFE_HPP

#include <array>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <type_traits>

struct no_copy {
    no_copy() = default;

    no_copy(const no_copy& other) = delete;
    no_copy& operator=(const no_copy& other) = delete;

    no_copy(no_copy&& other) = default; // Move Default Constructor
    no_copy& operator=(no_copy&& other) = default; // Move Assigment Operator
};

struct no_move {
    no_move() = default;

    no_move(const no_move& other) = default;
    no_move& operator=(const no_move& other) = default;

    no_move(no_move&& other) = delete;
    no_move& operator=(no_move&& other) = delete;
};

struct no_copy_no_move {
    no_copy_no_move() = default;

    no_copy_no_move(const no_copy_no_move& other) = delete;
    no_copy_no_move& operator=(const no_copy_no_move& other) = delete;

    no_copy_no_move(no_copy_no_move&& other) = delete;
    no_copy_no_move& operator=(no_copy_no_move&& other) = delete;
};

struct copy_exception {
    copy_exception() = default;

    copy_exception(const copy_exception& other) noexcept(false) = default;
    copy_exception& operator=(const copy_exception& other) noexcept(false) = default;

    copy_exception(copy_exception&& other) = delete;
    copy_exception& operator=(copy_exception&& other) = delete;
};

struct move_exception {
    move_exception() = default;

    move_exception(const move_exception& other) = delete;
    move_exception& operator=(const move_exception& other) = delete;

    move_exception(move_exception&& other) noexcept(false) = default;
    move_exception& operator=(move_exception&& other) noexcept(false) = default;
};

struct multiple_copy {
    multiple_copy() = default;

    multiple_copy(const multiple_copy& other) = default;
    multiple_copy& operator=(const multiple_copy& other) = default;

    multiple_copy(multiple_copy& other) noexcept(false) {
        throw 4;
    }
    multiple_copy& operator=(multiple_copy& other) noexcept(false) {
        throw 4;
    }

    multiple_copy(multiple_copy&& other) = delete;
    multiple_copy& operator=(multiple_copy&& other) = delete;

};

struct empty_stack : std::exception {
    const char *what() const noexcept override {
        return "Error, Empty Stack";
    };
};

template<typename T>
class threadsafe_stack {
private:
    // In order to make a wrapped unique_ptr, we need to create a stack with the specific typename
    std::stack<std::unique_ptr<T>> data{};
    // Mutable is used when we want to change the value inside a const object
    mutable std::mutex m{};
public:
    threadsafe_stack() {}

    // The thread-safe stack is move-constructable, but not assignable or copy-constructable.
    threadsafe_stack(threadsafe_stack &&other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = std::move(other.data);
    }

    threadsafe_stack(const threadsafe_stack &other) = delete;

    threadsafe_stack &operator=(const threadsafe_stack &) = delete;

    threadsafe_stack &operator=(threadsafe_stack &&) = delete;

    // There is a method to push one element by copy semantics. This method only exists if the type is
    // copy-able and copy-constructable without exceptions.
    template<typename D = T, std::enable_if_t<std::is_same_v<D, T> && std::is_nothrow_copy_constructible_v<D> &&
                                              std::is_nothrow_copy_assignable_v<D>, int> = 0>
    void push(const T &value) {
        std::lock_guard<std::mutex> lock(m);

        auto unique_ptr = std::make_unique<T>(value);
        data.push(std::move(unique_ptr));
    }

    // There is a method to push one element by move semantics. This method only exists if the type is
    // move-able and move-constructable without exceptions.
    template<typename D = T, std::enable_if_t<std::is_same_v<D, T> && std::is_nothrow_move_constructible_v<D> &&
                                              std::is_nothrow_move_assignable_v<D>, int> = 0>
    void push(T &&value) {
        std::lock_guard<std::mutex> lock(m);

        auto unique_ptr = std::make_unique<T>(std::move(value));
        data.push(std::move(unique_ptr));
    }

    // There is a method to push one element that is already wrapped inside a std::unique ptr
    void push(std::unique_ptr<T> ptr_to_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(ptr_to_value));
    }

    // There is a method that takes an std::array of variable size (> 0) and pushes all elements. The
    // elements must each be wrapped inside std::unique ptr.
    template<size_t elements_count>
    void push(std::array<std::unique_ptr<T>, elements_count> arr) {
        static_assert(elements_count > 0);

        std::lock_guard<std::mutex> lock(m);

        for (auto &ptr_to_value: arr) {
            data.push(std::move(ptr_to_value));
        }
    }

    // There is a method that removes the uppermost element and returns it within a std::unique ptr.
    std::unique_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);

        if (data.empty()) {
            return nullptr;
        }

        auto res = std::move(data.top());
        data.pop();

        return res;
    }

    // There is a method that takes as argument a reference to an object and populates it with the top
    // element. This method only exists if the type is copy-assignable.
    template<typename D = T, std::enable_if_t<std::is_same_v<D, T> && std::is_nothrow_copy_constructible_v<D> &&
                                              std::is_nothrow_copy_assignable_v<D>, int> = 0>
    void pop(T &elem) {
        std::lock_guard<std::mutex> lock(m);

        if (data.empty()) {
            throw empty_stack{};
        }

        elem = *data.top();
        data.pop();
    }

    // There is a method that returns the number of currently stored elements any integral type.
    template<typename integral_type>
    std::enable_if_t<std::is_integral_v<integral_type> && !std::is_same_v<bool, integral_type>, integral_type> size() {
        std::lock_guard<std::mutex> lock(m);

        auto size = data.size();
        auto cast_size = static_cast<integral_type>(size);

        return cast_size;
    }

};

#endif //INC_01_STACK_THREADSAFE_HPP
