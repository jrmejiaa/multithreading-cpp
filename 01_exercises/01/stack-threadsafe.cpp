#include <array>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <type_traits>

struct no_copy {
	no_copy(const no_copy& other) = delete;
	no_copy& operator=(const no_copy& other) = delete;

	no_copy(no_copy&& other) = default;
	no_copy& operator=(no_copy&& other) = default;
};

struct no_move {
	no_move(const no_move& other) = default;
	no_move& operator=(const no_move& other) = default;

	no_move(no_move&& other) = delete;
	no_move& operator=(no_move&& other) = delete;
};

struct no_copy_no_move {
	no_copy_no_move(const no_copy_no_move& other) = delete;
	no_copy_no_move& operator=(const no_copy_no_move& other) = delete;

	no_copy_no_move(no_copy_no_move&& other) = delete;
	no_copy_no_move& operator=(no_copy_no_move&& other) = delete;
};

struct copy_exception {
	copy_exception(const copy_exception& other) noexcept(false) = default;
	copy_exception& operator=(const copy_exception& other) noexcept(false) = default;

	copy_exception(copy_exception&& other) = delete;
	copy_exception& operator=(copy_exception&& other) = delete;
};

struct move_exception {
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
	const char* what() const throw();
};

template<typename T>
class threadsafe_stack {
private:
	std::stack<std::unique_ptr<T>> data{};
	mutable std::mutex m{};
public:
	threadsafe_stack() {}

	threadsafe_stack(const threadsafe_stack& other) = delete;

	threadsafe_stack(threadsafe_stack&& other) {
		std::lock_guard<std::mutex> lock(other.m);
		data = std::move(other.data);
	}

	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	threadsafe_stack& operator=(threadsafe_stack&&) = delete;

	template <typename D = T, std::enable_if_t<std::is_same_v<D, T>&& std::is_nothrow_copy_constructible_v<D>&& std::is_nothrow_copy_assignable_v<D>, int> = 0>
	void push(const T& value) {
		std::lock_guard<std::mutex> lock(m);

		auto unique_ptr = std::make_unique<T>(value);
		data.push(std::move(unique_ptr));
	}

	template <typename D = T, std::enable_if_t<std::is_same_v<D, T>&& std::is_nothrow_move_constructible_v<D>&& std::is_nothrow_move_assignable_v<D>, int> = 0>
	void push(T&& value) {
		std::lock_guard<std::mutex> lock(m);

		auto unique_ptr = std::make_unique<T>(std::move(value));
		data.push(std::move(unique_ptr));
	}

	void push(std::unique_ptr<T> ptr_to_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(ptr_to_value));
	}

	template<size_t elements_count>
	void push(std::array<std::unique_ptr<T>, elements_count> arr) {
		static_assert(elements_count > 0);

		std::lock_guard<std::mutex> lock(m);

		for (auto& ptr_to_value : arr) {
			data.push(std::move(ptr_to_value));
		}
	}

	std::unique_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);

		if (data.empty()) {
			return nullptr;
		}

		auto res = std::move(data.top());
		data.pop();

		return res;
	}

	template <typename D = T, std::enable_if_t<std::is_same_v<D, T>&& std::is_nothrow_copy_constructible_v<D>&& std::is_nothrow_copy_assignable_v<D>, int> = 0>
	void pop(T& elem) {
		std::lock_guard<std::mutex> lock(m);

		if (data.empty()) {
			throw empty_stack{};
		}

		elem = *data.top();
		data.pop();
	}

	template<typename integral_type>
	std::enable_if_t<std::is_integral_v<integral_type>, integral_type> size() {
		std::lock_guard<std::mutex> lock(m);

		auto size = data.size();
		auto cast_size = static_cast<integral_type>(size);

		return cast_size;
	}
};


int main() {
	threadsafe_stack<int> tss_int{};
	int value = 5;

	tss_int.push(value);
	tss_int.push(std::move(value));

	std::array<std::unique_ptr<int>, 2> arr{};
	tss_int.push(std::move(arr));

	auto size = tss_int.size<bool>();

	multiple_copy mc{};

	threadsafe_stack<multiple_copy> tss_multiple_copy{};
	tss_multiple_copy.push(mc);
	tss_multiple_copy.push(std::move(mc));

	tss_multiple_copy.pop(mc);

	no_copy nc{};

	threadsafe_stack<no_copy> tss_no_copy{};
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
	tss_move_exception.push(std::move(ncme));

	return 0;
}
