#pragma once

#include <string>
#include <cstring>
#include <bitset>

class StringView {
public:
    StringView(const std::string& my_s, size_t subs_start = 0,
               size_t subs_len = std::string::npos) {
        str_ptr_ = my_s.data();
        subs_start_ = subs_start;
        if (subs_len > my_s.size()) {
            subs_end_ = my_s.size();
        } else {
            subs_end_ = subs_len + subs_start_;
        }
    }

    StringView(const char* str_p) {
        str_ptr_ = str_p;
        subs_end_ = std::strlen(str_p);
    }

    StringView(const char* str_p, size_t subs_len) {
        str_ptr_ = str_p;
        subs_end_ = subs_len;
    }

    char operator[](size_t idx) const {
        return str_ptr_[subs_start_ + idx];
    }

    size_t Size() const {
        return subs_end_ - subs_start_;
    }

private:
    const char* str_ptr_ = nullptr;
    size_t subs_start_ = 0;
    size_t subs_end_ = 0;
};
