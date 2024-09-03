#pragma once

#include <utility>
#include <optional>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <memory>

template <class T>
class UnbufferedChannel {
public:
    explicit UnbufferedChannel() = default;

    void Send(const T& value) {
        std::unique_lock lock(global_);
        std::lock_guard lock_guard(send_);
        if (close_) {
            throw std::runtime_error("");
        }
        val_ = value;
        add_.notify_one();
        while (size_ == queue_.size()) {
            if (close_) {
                throw std::runtime_error("");
            }
            cv_.wait(lock);
        }
        if (close_) {
            throw std::runtime_error("");
        }
        queue_.push(value);
        add_.notify_all();
    }

    std::optional<T> Recv() {
        std::unique_lock lock(global_);
        while (queue_.empty()) {
            if (close_) {
                return std::nullopt;
            }
            add_.wait(lock);
        }
        if (close_ && queue_.empty()) {
            return std::nullopt;
        }
        T res = queue_.front();
        queue_.pop();
        cv_.notify_all();
        return res;
    }

    void Close() {
        std::unique_lock lock(global_);
        close_ = true;
        cv_.notify_all();
        add_.notify_all();
    }

private:
    std::atomic<bool> close_ = false;
    std::mutex global_;
    std::mutex send_;
    std::condition_variable cv_;
    std::condition_variable add_;
    std::atomic<size_t> size_;
    std::shared_ptr<T> val_;
};
