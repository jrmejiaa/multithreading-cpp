#pragma once

#include <algorithm>
#include <atomic>

template <typename T>
T atomic_fetch_max_read_modify_write(std::atomic<T>* pv,
	typename std::atomic<T>::value_type v,
	std::memory_order m) noexcept {
	auto t = pv->load(m);
	while (!pv->compare_exchange_weak(t, std::max(v, t), m));
	return t;
}

template <typename T>
T atomic_fetch_max_read_and_conditional_store(std::atomic<T>* pv,
	typename std::atomic<T>::value_type v,
	std::memory_order m) noexcept {
	auto t = pv->load(m);
	while (std::max(v, t) != t) {
		if (pv->compare_exchange_weak(t, v, m, m))
			break;
	}
	return t;
}

