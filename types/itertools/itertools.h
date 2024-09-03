#pragma once

template <class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }


private:
    Iterator begin_, end_;
};

template <class Iter>
class Iterator {
public:
    Iterator(Iter iter) {
        cur_ = iter;
    }

    Iterator& operator++() {
        ++cur_;
        return *this;
    }

    bool operator!=(const Iterator& other) const {
        return cur_ != other.cur_;
    }

    auto operator*() const {
        return cur_;
    }

private:
    Iter cur_;
};



