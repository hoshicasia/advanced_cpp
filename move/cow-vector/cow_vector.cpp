#include "cow_vector.h"

COWVector::COWVector() {
    inner_repr_ = new State();
}

COWVector::~COWVector() {
    if (inner_repr_->ref_counter_ == 1) {
        delete inner_repr_;
        inner_repr_ = nullptr;
    } else {
        --inner_repr_->ref_counter_;
    }
}

COWVector::COWVector(const COWVector& other) {
    inner_repr_ = other.inner_repr_;
    ++inner_repr_->ref_counter_;
}

COWVector& COWVector::operator=(const COWVector& other) {
    if (inner_repr_->ref_counter_ == 1) {
        --inner_repr_->ref_counter_;
        delete inner_repr_;
        inner_repr_ = nullptr;
    } else {
        --inner_repr_->ref_counter_;
    }
    inner_repr_ = other.inner_repr_;
    ++inner_repr_->ref_counter_;
    return *this;
}

COWVector& COWVector::operator=(COWVector&& other) {
    inner_repr_->inner = std::move(other.inner_repr_->inner);
    return *this;
}

size_t COWVector::Size() const {
    return inner_repr_->inner_size_;
}

const std::string& COWVector::Get(size_t at) {
    return inner_repr_->inner.at(at);
}
const std::string& COWVector::Back() {
    return inner_repr_->inner.back();
}

void COWVector::PushBack(const std::string& value) {
    if (inner_repr_->ref_counter_ == 1) {
        inner_repr_->inner.push_back(value);
        inner_repr_->inner_size_ += 1;
    } else {
        std::vector<std::string> new_inner = inner_repr_->inner;
        new_inner.push_back(value);
        --inner_repr_->ref_counter_;
        inner_repr_ = new State();
        inner_repr_->ref_counter_ = 1;
        inner_repr_->inner = new_inner;
        inner_repr_->inner_size_ = new_inner.size();
    }
}

void COWVector::Set(size_t at, const std::string& value) {
    if (inner_repr_->ref_counter_ == 1) {
        inner_repr_->inner[at] = value;
    } else {
        std::vector<std::string> new_inner = inner_repr_->inner;
        new_inner[at] = value;
        --inner_repr_->ref_counter_;
        inner_repr_ = new State();
        inner_repr_->ref_counter_ = 1;
        inner_repr_->inner = new_inner;
        inner_repr_->inner_size_ = new_inner.size();
    }
}

void COWVector::Resize(size_t size) {
    if (inner_repr_->ref_counter_ == 1) {
        inner_repr_->inner.resize(size);
        inner_repr_->inner_size_ = size;
    } else {
        std::vector<std::string> new_inner;
        new_inner = inner_repr_->inner;
        new_inner.resize(size);
        --inner_repr_->ref_counter_;
        inner_repr_ = new State();
        inner_repr_->inner = new_inner;
        inner_repr_->ref_counter_ = 1;
        inner_repr_->inner_size_ = new_inner.size();
    }
}