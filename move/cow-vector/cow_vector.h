#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

class COWVector {
public:
    COWVector();

    ~COWVector();

    COWVector(const COWVector& other);

    COWVector& operator=(const COWVector& other);

    COWVector& operator=(COWVector&& other);

    size_t Size() const;

    void Resize(size_t size);

    const std::string& Get(size_t at);
    const std::string& Back();

    void PushBack(const std::string& value);

    void Set(size_t at, const std::string& value);

private:
    struct State {
        std::vector<std::string> inner;
        size_t ref_counter_ = 1;
        size_t inner_size_ = 0;
        State(const State&) = delete;
        State& operator=(const State&) = delete;
        State() = default;
        ~State() {
            ref_counter_ = 0;
            inner_size_ = 0;
        }
    };
    State* inner_repr_ = nullptr;
};
