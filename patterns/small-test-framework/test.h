#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() = default;
};

class TestRegistry {
public:
    TestRegistry(const TestRegistry&) = delete;
    TestRegistry& operator=(const TestRegistry&) = delete;

    template <typename TestClass>
    void RegisterClass(const std::string& class_name) {
        registry_.push_back(class_name);
        registry_2_[class_name] = []() -> std::unique_ptr<TestClass> {
            return std::make_unique<TestClass>();
        };
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        if (std::find(registry_.begin(), registry_.end(), class_name) == registry_.end()) {
            throw std::out_of_range("");
        }
        return registry_2_[class_name]();
    }

    void RunTest(const std::string& test_name) {
        std::unique_ptr<AbstractTest> test = CreateTest(test_name);
        test->SetUp();
        try {
            test->Run();
        } catch (...) {
            test->TearDown();
            throw;
        }
        test->TearDown();
    }

    template <typename Predicate>
    std::vector<std::string> ShowTests(Predicate callback) {
        std::sort(registry_.begin(), registry_.end());
        std::vector<std::string> res;
        for (const auto& s : registry_) {
            if (callback(s)) {
                res.push_back(s);
            }
        }
        return res;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> res;
        for (auto& pair : registry_2_) {
            res.push_back(pair.first);
        }
        return res;
    }

    template <typename Predicate>
    void RunTests(Predicate callback) {
        std::sort(registry_.begin(), registry_.end());
        for (const auto& s : registry_) {
            if (callback(s)) {
                RunTest(s);
            }
        }
    }

    void Clear() {
        registry_.clear();
        registry_2_.clear();
    }

    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

private:
    TestRegistry() = default;
    std::map<std::string, std::function<std::unique_ptr<AbstractTest>()>> registry_2_;
    std::vector<std::string> registry_;
};

struct FullMatch {
    FullMatch(const std::string& s) {
        str_ = s;
    }
    bool operator()(const std::string& test) {
        return test == str_;
    }

private:
    std::string str_;
};

struct Substr {
    Substr(const std::string& substr) {
        substr_ = substr;
    }
    bool operator()(const std::string& test) {
        return test.find(substr_) != std::string::npos;
    }

private:
    std::string substr_;
};
