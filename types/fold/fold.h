#pragma once
#include <vector>
#include <string>

struct Sum {
public:
    Sum() = default;
    int operator()(int a, int b) {
        return a + b;
    }
};

struct Prod {
    Prod() = default;
    int operator()(int a, int b) {
        return a * b;
    }
};

struct Concat {
public:
    Concat() = default;
    std::vector<int> operator()(std::vector<int> a, std::vector<int> b) {
        std::vector<int> cr;
        for (auto el : a) {
            cr.push_back(el);
        }
        for (auto el : b) {
            cr.push_back(el);
        }
        return cr;
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    while (first != last) {
        init = func(init, *first);
        first++;
    }
    return init;
}

class Length {
public:
    int* res_ = nullptr;
    Length(int* future_res) {
        res_ = future_res;
    }

    int operator()(std::string a, std::string b) {
        *res_ += 1;
        return a.size() + b.size();
    }

    int operator()(int a, int b) {
        *res_ += 1;
        return a + b;
    }
};
