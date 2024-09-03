#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <math.h>
#include <deque>

template <class RandomAccessIterator, class T, class Func>
void ThreadReduce(RandomAccessIterator first, RandomAccessIterator last, size_t num, Func func,
                  std::deque<T>& result) {
    result[num] = *(first++);
    while (first != last) {
        result[num] = func(result[num], *first++);
    }
}

template <class RandomAccessIterator, class T, class Func>
T Reduce(RandomAccessIterator first, RandomAccessIterator last, const T& initial_value, Func func) {
    T cur_value(initial_value);
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    int bound = 0;
    RandomAccessIterator first_copy = first;
    while (first_copy != last) {
        first_copy++;
        bound++;
    }
    int batch = bound / num_threads > 0 ? bound / num_threads : 1;
    int sz = (batch - 1 + bound) / batch;
    std::deque<T> results(sz);
    for (int i = 0; i < sz; i++) {
        if (first + i * batch >= last) {
            break;
        }
        threads.emplace_back(ThreadReduce<RandomAccessIterator, T, Func>, first + i * batch,
                             std::min(first + (i + 1) * batch, last), i, func, std::ref(results));
    }
    for (auto& x : threads) {
        x.join();
    }
    for (const T& x : results) {
        cur_value = func(cur_value, x);
    }
    return cur_value;
}
