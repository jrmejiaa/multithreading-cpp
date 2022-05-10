#pragma once

#include <array>
#include <atomic>
#include <stdexcept>
#include <type_traits>

template<typename T>
class atomic_memory_pointer;

template<typename T>
class atomic_memory_location {
	static_assert(std::is_trivially_copyable_v<T>);

	friend class atomic_memory_pointer<T>;

	std::atomic<size_t> writers = ATOMIC_VAR_INIT(0);
	std::atomic<size_t> history = ATOMIC_VAR_INIT(0);

	std::atomic<size_t> pointers = ATOMIC_VAR_INIT(0);

	T* data = nullptr;
	size_t size = 0;

public:
	atomic_memory_location(size_t number_elements) {
		data = new T[number_elements];
		size = number_elements;
	}

	~atomic_memory_location() {
		for (auto current_number_pointers = pointers.load(std::memory_order::relaxed);
			current_number_pointers != 0;) {
			pointers.wait(current_number_pointers, std::memory_order::relaxed);
			current_number_pointers = pointers.fetch_add(0, std::memory_order::relaxed);
		}

		delete[] data;
	}

	void write(const T& value, size_t position) {
		if (position >= size) {
			throw std::runtime_error("position too large");
		}

		while (true) {
            const auto prevWriters = writers.fetch_add(1, std::memory_order::relaxed);
			if (prevWriters == 0) {
				break;
			}
			writers.fetch_sub(1, std::memory_order::relaxed);
		}

		history.fetch_add(1, std::memory_order::relaxed);
		data[position] = value;
		history.fetch_add(1, std::memory_order::release);

		writers.fetch_sub(1, std::memory_order::relaxed);
	}

	T read(size_t position) {
		if (position >= size) {
			throw std::runtime_error("position too large");
		}

		T value{};

		while (true) {
			const auto previous_version = history.load(std::memory_order::acquire);
			if (previous_version % 2 == 1) {
				continue;
			}

			value = data[position];
			const auto current_version = history.fetch_add(0, std::memory_order::relaxed);

			if (previous_version != current_version) {
				continue;
			}

			return value;
		}
	}

	template<size_t array_size>
	void write_array(const std::array<T, array_size>& values, size_t position) {
		if (position + array_size >= size) {
			throw std::runtime_error("position + array_size too large");
		}

		while (true) {
			const auto value = writers.fetch_add(1, std::memory_order::relaxed);
			if (value == 0) {
				break;
			}
			writers.fetch_sub(1, std::memory_order::relaxed);
		}

		history.fetch_add(1, std::memory_order::relaxed);
		for (auto i = 0; i < array_size; i++) {
			data[position + i] = values[i];
		}
		history.fetch_add(1, std::memory_order::release);

		writers.fetch_sub(1, std::memory_order::relaxed);
	}

	template<size_t array_size>
	std::array<T, array_size> read_array(size_t position) {
		if (position + array_size >= size) {
			throw std::runtime_error("position too large");
		}

		std::array<T, array_size> values{};

		while (true) {
			const auto previous_version = history.load(std::memory_order::acquire);
			if (previous_version % 2 == 1) {
				continue;
			}

			for (auto i = 0; i < array_size; i++) {
				values[i] = data[position + i];
			}

			const auto current_version = history.fetch_add(0, std::memory_order::relaxed);
			if (previous_version != current_version) {
				continue;
			}

			return values;
		}
	}
};


template<typename T>
class atomic_memory_pointer {
	static_assert(std::is_trivially_copyable_v<T>);

	atomic_memory_location<T>* ptr;

public:
	atomic_memory_pointer(atomic_memory_location<T>& memory_location) :
		ptr(&memory_location) {
		ptr->pointers.fetch_add(1, std::memory_order::relaxed);
	}

	~atomic_memory_pointer() {
		if (ptr == nullptr) {
			return;
		}

		ptr->pointers.fetch_sub(1, std::memory_order::relaxed);
		ptr->pointers.notify_one();
	}

	atomic_memory_pointer(const atomic_memory_pointer& other) :
		ptr(other.ptr) {
		ptr->pointers.fetch_add(1, std::memory_order::relaxed);
	}

	atomic_memory_pointer& operator=(const atomic_memory_pointer& other) {
		if (ptr != nullptr) {
			ptr->pointers.fetch_sub(1, std::memory_order::relaxed);
		}

		ptr = other.ptr;
		ptr->pointers.fetch_add(1, std::memory_order::relaxed);
	}

	atomic_memory_pointer(atomic_memory_pointer&& other) :
		ptr(other.ptr) {
		other.ptr = nullptr;
	}

	atomic_memory_pointer& operator=(atomic_memory_pointer&& other) {
		if (ptr != nullptr) {
			ptr->pointers.fetch_sub(1, std::memory_order::relaxed);
		}

		ptr = other.ptr;
		other.ptr = nullptr;
	}

	void write(const T& value, size_t position) {
		ptr->write(value, position);
	}

	T read(size_t position) {
		return ptr->read(position);
	}

	template<size_t array_size>
	void write_array(const std::array<T, array_size>& values, size_t position) {
		ptr->write_array(values, position);
	}

	template<size_t array_size>
	std::array<T, array_size> read_array(size_t position) {
		return ptr->template read_array<array_size>(position);
	}
};
