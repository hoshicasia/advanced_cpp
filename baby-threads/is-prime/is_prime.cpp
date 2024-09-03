#include "is_prime.h"
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<bool> res = true;

void Prime(uint64_t x, uint64_t st, uint64_t end) {
    if (x == 1 || x == 0) {
        res = false;
        return;
    }
    if (x % 2 == 0) {
        res = false;
        return;
    }
    for (uint64_t i = st; i < end; ++i) {
        if (x % i == 0) {
            res = false;
            return;
        }
    }
}

bool IsPrime(uint64_t x) {
    if (x == 1 || x == 0) {
        res = false;
        return res;
    }
    res = true;
    int num_threads = std::thread::hardware_concurrency();
    uint64_t root = sqrt(x);
    auto bound = std::min(root + 6, x);
    size_t batch = bound / num_threads;
    std::vector<std::thread> threads;
    std::vector<uint64_t> tasks;
    for (size_t i = 2; i < bound; i += batch) {
        threads.emplace_back(Prime, std::cref(x), i, i + batch);
    }
    for (auto& t : threads) {
        t.join();
    }
    return res;
}
