#pragma once
#include <mutex>
#include <condition_variable>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        blocked_readers_ += 1;
        lock.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        while (blocked_readers_) {
            cv_.wait(lock);
        }
        func();
        cv_.notify_one();
    }

private:
    std::condition_variable cv_;
    std::mutex global_;
    int blocked_readers_ = 0;

    void EndRead() {
        std::unique_lock<std::mutex> lock(global_);
        blocked_readers_ -= 1;
        if (!blocked_readers_) {
            cv_.notify_one();
        }
    }
};
