#pragma once
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <map>

template <class T>
class TimerQueue {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

public:
    void Add(const T& item, TimePoint at) {
        std::unique_lock<std::mutex> lock(global_);
        timepoints_.emplace(at, item);
        cv_.notify_one();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(global_);
        if (timepoints_.empty()) {
            cv_.wait(lock);
        }
        std::pair<TimePoint, T> pr = *(timepoints_.begin());
        timepoints_.erase(timepoints_.begin());
        cv_.wait_until(lock, pr.first);
        return pr.second;
    }

private:
    std::multimap<TimePoint, T> timepoints_;
    std::condition_variable cv_;
    std::mutex global_;
};
