#include "string_operations.h"
#include <math.h>

bool StartsWith(std::string_view string, std::string_view text) {
    if (text.size() > string.size()) {
        return false;
    }
    if (string.compare(0, text.size(), text, 0, text.size()) == 0) {
        return true;
    }
    return false;
}

bool EndsWith(std::string_view string, std::string_view text) {
    if (text.size() > string.size()) {
        return false;
    }
    if (string.compare(string.size() - text.size(), string.size(), text, 0, text.size()) == 0) {
        return true;
    }
    return false;
}

std::string_view StripPrefix(std::string_view string, std::string_view prefix) {
    if (StartsWith(string, prefix)) {
        return string.substr(prefix.size());
    }
    return string;
}

std::string_view StripSuffix(std::string_view string, std::string_view suffix) {
    if (EndsWith(string, suffix)) {
        return string.substr(0, string.size() - suffix.size());
    }
    return string;
}

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n = std::string_view::npos) {
    if (n > s.size()) {
        return s;
    }
    return s.substr(pos, n);
}

std::string_view StripAsciiWhitespace(std::string_view s) {
    size_t st = 0;
    size_t end = s.size() - 1;
    while (st < s.size() && isspace(s[st])) {
        st += 1;
    }
    while (end < s.size() && isspace(s[end])) {
        end -= 1;
    }
    return s.substr(st, end - st + 1);
}

std::string_view RemoveSlash(std::string_view path) {
    if (path.size() > 1) {
        if (path[path.size() - 1] == '/') {
            return path.substr(0, path.size() - 1);
        }
        return path;
    }
    return path;
}

std::string AddSlash(std::string_view path) {
    if (path[path.size() - 1] != '/') {
        std::string s = "/";
        return (path.data() + s);
    }
    return path.data();
}

std::string_view Dirname(std::string_view path) {
    size_t end = path.rfind('/');
    size_t st = path.find('/');
    if (st == end) {
        return path.substr(st, end - st + 1);
    }
    return path.substr(st, end - st);
}

std::string_view Basename(std::string_view path) {
    size_t end = path.rfind('/');
    return path.substr(end + 1, path.size());
}

std::string CollapseSlashes(std::string_view path) {
    if (path.empty()) {
        return "";
    }
    size_t trash = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        if (path[i] == path[i + 1] && path[i] == '/') {
            trash += 1;
        }
    }
    if (trash == path.size()) {
        return "/";
    }
    std::string s(path.size() - trash, '*');
    size_t j = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        if (!(path[i] == path[i + 1] && path[i] == '/')) {
            s[j] = path[i];
            j += 1;
        }
    }
    return s;
}

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter) {
    size_t buf_size = 0;
    if (strings.empty()) {
        return "";
    }
    for (const auto& s : strings) {
        buf_size += s.size();
    }
    buf_size += delimiter.size() * (strings.size() - 1);
    std::string str;
    str.reserve(buf_size + 75);
    str.resize(buf_size);
    size_t i = 0;
    for (auto s : strings) {
        for (auto c : s) {
            str[i] = c;
            i += 1;
        }
        if (i < buf_size - delimiter.size()) {
            for (auto c : delimiter) {
                str[i] = c;
                i += 1;
            }
        }
    }
    return str;
}

std::string ReadN(const std::string& filename, size_t n) {
    char buff[n];
    std::string s;
    int file_descriptor = open(filename.data(), O_RDONLY);
    if (file_descriptor == -1) {
        return "";
    }
    ssize_t bytes_read = read(file_descriptor, buff, n);

    close(file_descriptor);

    s = std::string(buff, bytes_read);

    return s;
}

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim) {
    if (text.empty()) {
        return {""};
    } else {
        size_t pos = 0;
        size_t count = 0;
        size_t dl_pos = 0;
        while (dl_pos != std::string::npos) {
            dl_pos = text.find(delim, pos);
            pos += delim.size() + text.substr(pos, dl_pos - pos).size();
            count += 1;
        }
        std::vector<std::string_view> res(count);
        pos = 0;
        dl_pos = 0;
        size_t i = 0;
        while (dl_pos != std::string::npos) {
            dl_pos = text.find(delim, pos);
            res[i] = text.substr(pos, dl_pos - pos);
            pos += delim.size() + text.substr(pos, dl_pos - pos).size();
            i += 1;
        }
        return res;
    }
}
