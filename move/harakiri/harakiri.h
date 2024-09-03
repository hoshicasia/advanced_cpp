#pragma once

#include <string>

// Should not allow reuse and yell under sanitizers.
// Fix the interface and implementation.
// AwesomeCallback should add "awesomeness".

class OneTimeCallback {
public:
    virtual ~OneTimeCallback() = default;
    virtual std::string operator()() const&& = 0;

protected:
    std::string ret_;
};

// Implement ctor, operator(), maybe something else...
class AwesomeCallback : public OneTimeCallback {
public:
    AwesomeCallback(const std::string& s) {
        std::string tmp = s + "awesomeness";
        ret_ = tmp;
    }
    std::string operator()() const&& {
        std::string tmp = ret_;
        delete this;
        return tmp;
    }
};
