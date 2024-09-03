#pragma once
#include <vector>
#include <mutex>
#include <deque>
#include <atomic>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentHashMap {
public:
    ConcurrentHashMap(const Hash& hasher = Hash()) : ConcurrentHashMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentHashMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentHashMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentHashMap(int expected_size, int expected_threads_count, const Hash& hasher = Hash())
        : hash_(hasher) {
        table_.resize(15000);
        mutex_block_.resize(expected_threads_count);
        size_ = 15000;
    }

    bool Insert(const K& key, const V& value) {
        size_t bucket_id = hash_(key) % size_;
        size_t m_id = bucket_id % mutex_block_.size();
        bool rehash_worth = false;
        {
            std::scoped_lock<std::mutex> lock(mutex_block_[m_id]);
            size_t bid = hash_(key) % size_;
            for (const std::pair<K, V>& pair : table_[bid]) {
                if (key == pair.first) {
                    return false;
                }
            }
            table_[bid].emplace_back(key, value);
            if (table_[bid].size() > k_default_threshold) {
                rehash_worth = true;
            }
        }

        if (rehash_worth) {
                //std::scoped_lock<std::mutex> lock(mutex_);
                for (size_t i = 0; i < mutex_block_.size(); ++i) {
                    mutex_block_[i].lock();
                }
                size_t nz = size_ * 2;
                size_ = size_ * 2;
                std::vector<std::vector<std::pair<K, V>>> new_table(nz);

                for (size_t i = 0; i < table_.size(); ++i) {
                    for (const std::pair<K, V>& pr : table_[i]) {
                        size_t idx = hash_(pr.first) % nz;
                        new_table[idx].push_back(pr);
                    }
                }
                table_ = std::move(new_table);
                for (int i = mutex_block_.size() - 1; i >= 0; --i) {
                    mutex_block_[i].unlock();
                }
        }
        return true;
    }

    bool Erase(const K& key) {
        size_t bucket_id = hash_(key) % size_;
        size_t m_id = bucket_id % mutex_block_.size();
        std::scoped_lock<std::mutex> lock(mutex_block_[m_id]);
        size_t bid = hash_(key) % size_;
        for (auto pr = table_[bid].begin(); pr != table_[bid].end(); pr++) {
            if (pr->first == key) {
                table_[bid].erase(pr);
                return true;
            }
        }
        return false;
    }

    void Clear() {
        for (size_t i = 0; i < mutex_block_.size(); ++i) {
            mutex_block_[i].lock();
        }
//        for (size_t i = 0; i < table_.size(); ++i) {
//            table_[i].clear();
//        }
        table_.clear();
        table_.resize(15000);
        size_ = 15000;
        for (int i = mutex_block_.size() - 1; i >= 0; --i) {
            mutex_block_[i].unlock();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        size_t bucket_id = hash_(key) % size_;
        size_t m_id = bucket_id % mutex_block_.size();
        std::scoped_lock<std::mutex> lock(mutex_block_[m_id]);
        size_t bid = hash_(key) % size_;
        for (const std::pair<K, V>& pr : table_[bid]) {
            if (pr.first == key) {
                return {true, pr.second};
            }
        }
        return {false, V()};
    }

    const V At(const K& key) const {
        size_t bucket_id = hash_(key) % size_;
        size_t m_id = bucket_id % mutex_block_.size();
        std::scoped_lock<std::mutex> lock(mutex_block_[m_id]);
        size_t bid = hash_(key) % size_;
        for (const std::pair<K, V>& pr : table_[bid]) {
            if (pr.first == key) {
                return pr.second;
            }
        }
        throw std::out_of_range("");
    }

    size_t Size() const {
        for (size_t i = 0; i < mutex_block_.size(); ++i) {
            mutex_block_[i].lock();
        }
        size_t ans = 0;
        for (const std::vector<std::pair<K, V>>& pr : table_) {
            ans += pr.size();
        }
        for (int i = mutex_block_.size() - 1; i >= 0; --i) {
            mutex_block_[i].unlock();
        }
        return ans;
    }

    static const int kUndefinedSize;
    static const int kDefaultConcurrencyLevel;
    static int k_default_threshold;

private:
    //mutable std::mutex mutex_;
    std::vector<std::vector<std::pair<K, V>>> table_;
    Hash hash_;
    mutable std::deque<std::mutex> mutex_block_;
    std::atomic<size_t> size_ = 0;

//    void Lock() const {
//        for (size_t i = 0; i < mutex_block_.size(); ++i) {
//            mutex_block_[i].lock();
//        }
//    }
//
//    void Unlock() const {
//        for (int i = mutex_block_.size() - 1; i >= 0; --i) {
//            mutex_block_[i].unlock();
//        }
//    }

};

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentHashMap<K, V, Hash>::kUndefinedSize = 2;

template <class K, class V, class Hash>
int ConcurrentHashMap<K, V, Hash>::k_default_threshold = 15;