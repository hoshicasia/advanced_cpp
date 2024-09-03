#pragma once

#include <memory>
#include <vector>
#include <string>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> out;
    for (size_t i = 0; i < items.size(); ++i) {
        out.push_back(std::make_unique<string>(*(items[i])));
    }
    return out;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> out;
    for (size_t i = 0; i < items.size(); ++i) {
        bool flag = false;
        for (size_t j = 0; j < out.size(); ++j) {
            if (*(items[i]) == *(out[j])) {
                out.push_back(out[j]);
                flag = true;
                break;
            }
        }
        if (!flag) {
            out.push_back(std::make_shared<string>(*(items[i])));
        }
    }
    return out;
}
