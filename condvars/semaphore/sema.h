#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

class DefaultCallback {
public:
    void operator()(int& value) {
        --value;
    }
};

class Semaphore {
public:
    Semaphore(int count) : count_(count) {
    }

    void Leave() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!cv_.empty()) {
            cv_.front()->notify_one();
            cv_.pop();
        }
        ++count_;
    }

    template <class Func>
    void Enter(Func callback) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!count_) {
            cv_.push(std::make_shared<std::condition_variable>());
            cv_.back()->wait(lock);
        }
        count_ -= 1;
        callback(count_);
    }

    void Enter() {
        DefaultCallback callback;
        Enter(callback);
    }

private:
    std::mutex mutex_;
    std::queue<std::shared_ptr<std::condition_variable>> cv_;
    int count_ = 0;
};
