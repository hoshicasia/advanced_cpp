#include "lru_cache.h"

LruCache::LruCache(size_t max_size) {
    max_size_ = max_size;
}

void LruCache::Set(const std::string& key, const std::string& value) {
    if (iterators_map_.find(key) != iterators_map_.end()) {
        iterators_map_[key]->second = value;
        cache_.splice(cache_.begin(), cache_, iterators_map_[key]);
    } else {
        cache_.push_front(std::pair<std::string, std::string>(key, value));
        iterators_map_[key] = cache_.begin();
        if (cache_.size() > max_size_) {
            iterators_map_.erase(cache_.back().first);
            cache_.pop_back();
        }
    }
}

bool LruCache::Get(const std::string& key, std::string* value) {
    if (iterators_map_.find(key) != iterators_map_.end()) {
        *value = iterators_map_[key]->second;
        cache_.splice(cache_.begin(), cache_, iterators_map_[key]);
        iterators_map_[key] = cache_.begin();
        return true;
    }
    return false;
}
