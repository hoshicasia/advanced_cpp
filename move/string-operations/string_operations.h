#pragma once
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <cstring>

bool StartsWith(std::string_view string, std::string_view text);

bool EndsWith(std::string_view string, std::string_view text);

std::string_view StripPrefix(std::string_view string, std::string_view prefix);

std::string_view StripSuffix(std::string_view string, std::string_view suffix);

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n);

std::string_view StripAsciiWhitespace(std::string_view s);

std::string AddSlash(std::string_view path);

std::string_view RemoveSlash(std::string_view path);

std::string ReadN(const std::string& filename, size_t n);

std::string_view Dirname(std::string_view path);

std::string_view Basename(std::string_view path);

std::string CollapseSlashes(std::string_view path);

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter);

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim);

inline size_t Size(const std::string& s) {
    return s.size();
}

inline size_t Size(std::string_view s) {
    return s.size();
}

inline size_t Size(const char* s) {
    return strlen(s);
}

inline size_t ToString(std::string& x, const std::string& s) {
    size_t idx = x.find('*');
    for (size_t i = 0; i < s.size(); ++i) {
        x[idx + i] = s[i];
    }
    return 0;
}

inline size_t ToString(std::string& x, const std::string_view& s) {
    size_t idx = x.find('*');
    for (size_t i = 0; i < s.size(); ++i) {
        x[idx + i] = s[i];
    }
    return 0;
}

inline size_t ToString(std::string& x, const char* s) {
    size_t idx = x.find('*');
    for (size_t i = 0; i < Size(s); ++i) {
        x[idx + i] = s[i];
    }
    return 0;
}

inline size_t ToString(std::string& x, const int& n) {
    size_t cnt = n == 0 ? 1 : 0;
    int m = n;
    while (m != 0) {
        m /= 10;
        cnt += 1;
    }
    size_t idx = x.find('*');
    if (n < 0) {
        x[idx] = '-';
        idx += 1;
    }
    int num = std::abs(n);
    for (size_t i = idx + cnt - 1; i >= idx; --i) {
        x[i] = static_cast<char>(std::abs(num % 10) + 48);
        num /= 10;
        if (i == 0) {
            break;
        }
    }
    return 0;
}

inline size_t ToString(std::string& x, const long& n) {
    size_t cnt = n == 0 ? 1 : 0;
    long m = n;
    while (m != 0) {
        m /= 10;
        cnt += 1;
    }
    size_t idx = x.find('*');
    if (n < 0) {
        x[idx] = '-';
        idx += 1;
    }
    long num = std::labs(n);
    for (size_t i = idx + cnt - 1; i >= idx; --i) {
        x[i] = static_cast<char>(std::abs(num % 10) + 48);
        num /= 10;
        if (i == 0) {
            break;
        }
    }
    return 0;
}

inline size_t ToString(std::string& x, const long long& n) {
    size_t cnt = n == 0 ? 1 : 0;
    long long m = n;
    while (m != 0) {
        m /= 10;
        cnt += 1;
    }
    size_t idx = x.find('*');
    if (n < 0) {
        x[idx] = '-';
        idx += 1;
    }
    long long num = n;
    for (size_t i = idx + cnt - 1; i >= idx; --i) {
        x[i] = static_cast<char>(std::abs(num % 10) + 48);
        num /= 10;
        if (i == 0) {
            break;
        }
    }
    return 0;
}

template <typename T>
size_t ToString(std::string& x, const T& n) {
    size_t cnt = n == 0 ? 1 : 0;
    T m = n;
    while (m != 0) {
        m /= 10;
        cnt += 1;
    }
    size_t idx = x.find('*');
    T k = n - n - 1;
    k += 1 + n;
    if (k == n) {
        if (n < 0) {
            x[idx] = '-';
            idx += 1;
        }
    }
    T num = n;
    for (size_t i = idx + cnt - 1; i >= idx; --i) {
        x[i] = static_cast<char>(num % 10 + 48);
        num /= 10;
        if (i == 0) {
            break;
        }
    }
    return 0;
}

inline size_t Size(const int& n) {
    size_t res = 0;
    int num = n;
    do {
        num /= 10;
        res += 1;
    } while (num);
    if (n < 0) {
        res += 1;
    }
    return res;
}

inline size_t Size(const long& n) {
    size_t res = 0;
    long num = n;
    do {
        num /= 10;
        res += 1;
    } while (num);
    if (n < 0) {
        res += 1;
    }
    return res;
}

inline size_t Size(const long long& n) {
    size_t res = 0;
    long long num = n;
    do {
        num /= 10;
        res += 1;
    } while (num);
    if (n < 0) {
        res += 1;
    }
    return res;
}

template <typename T>
size_t Size(const T& n) {
    size_t res = 0;
    T num = n;
    do {
        num /= 10;
        res += 1;
    } while (num);
    return res;
}

template <typename... Args>
std::string StrCat(const Args&... args) {
    std::string s((Size(args) + ... + Size("")), '*');
    (ToString(s, args) + ... + ToString(s, ""));
    return s;
}