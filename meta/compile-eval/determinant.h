#pragma once

#include <array>

template<int A, int B>
struct Sum {
    static constexpr int value = Sum<A/2, B/2>::value + (A - A/2) + (B - B/2);
};

template<>
struct Sum<0, 0> {
    static constexpr  int value = 0;
};


template<int N>
constexpr int determinant(const std::array<std::array<int, N>, N>& a) {
    return 0;
}
