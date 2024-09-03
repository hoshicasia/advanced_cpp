#pragma once

#include <initializer_list>
#include <algorithm>
#include <memory>
#include <math.h>
#include <iostream>

const size_t kBlockSize = 128;

struct Block {
    int first = 0;
    int last = 0;
    int block[128];
};

class Deque {
public:
    Deque() = default;
    Deque(const Deque& rhs) {
        buff_ = rhs.buff_;
        head_ = rhs.head_;
        tail_ = rhs.tail_;
        size_ = rhs.size_;
        block_count_ = rhs.block_count_;
    }

    Deque(Deque&& rhs) {
        buff_ = std::move(rhs.buff_);
        head_ = rhs.head_;
        tail_ = rhs.tail_;
        size_ = rhs.size_;
        block_count_ = rhs.block_count_;
        rhs.buff_ = static_cast<Block**>(std::malloc(1 * sizeof(Block*)));
        rhs.head_ = 0;
        rhs.tail_ = 0;
        rhs.size_ = 0;
        rhs.block_count_ = 0;
    }
    explicit Deque(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) {
        for (auto el : list) {
            PushBack(el);
        }
    }
    Deque& operator=(Deque rhs) {
        buff_ = rhs.buff_;
        head_ = rhs.head_;
        tail_ = rhs.tail_;
        size_ = rhs.size_;
        block_count_ = rhs.block_count_;
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(rhs.buff_, buff_);
        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        std::swap(size_, rhs.size_);
        std::swap(tail_, rhs.tail_);
    }

    void PushBack(int value) {
       if (IsEmpty()) {
           Block* new_block = new Block;
           new_block->last = 1;
           new_block->block[0] = value;
           block_count_ += 1;
           buff_[0] = new_block;
       } else if (buff_[tail_]->last == 128) {
           if (tail_ + 1 == block_count_) {
               GrowBuff();
           }
           Block* new_block = new Block;
           new_block->last = 1;
           new_block->block[0] = value;
           buff_[tail_] = new_block;
       } else {
           buff_[tail_]->block[buff_[tail_]->last] = value;
           buff_[tail_]->last += 1;
       }
       size_ += 1;
    }

    void PopBack() {
       buff_[tail_]->block[buff_[tail_]->last - 1] = 0;
       buff_[tail_]->last -= 1;
       size_ -= 1;
       if (buff_[tail_]->last == 0) {
           delete buff_[tail_];
           block_count_ -=1;
           tail_ -= 1;
       }
    }

    void PushFront(int value) {
        if (IsEmpty()) {
           std::shared_ptr<Block> new_block = std::shared_ptr<Block>(new Block);
           new_block->last = 127;
           new_block->block[127] = value;
           block_count_ += 1;
        }
        else if (buff_[head_]->first == 0) {
           if (head_ == 0) {
               GrowBuff();
           }
           Block* new_block = new Block;
           new_block->last = 127;
           new_block->first = 126;
           new_block->block[127] = value;
           head_ -= 1;
           buff_[head_] = new_block;
        } else {
           buff_[head_]->block[buff_[head_]->first - 1] = value;
           buff_[tail_]->first -= 1;
        }
        size_ += 1;
    }

    void PopFront() {
        buff_[head_]->block[buff_[head_]->first] = 0;
        buff_[head_]->first += 1;
        size_ -= 1;
        if (buff_[head_]->first - buff_[head_]->last == 0) {
           delete buff_[head_];
           block_count_ -=1;
           head_ += 1;
        }
    }

    int& operator[](size_t ind) {
        int bl_num = ind / (128 * block_count_);
        int ix = ind % 128;
        if (bl_num > head_ + 1) {
            int x = tail_ + bl_num - head_ - 1;
            return buff_[x]->block[buff_[x]->first + ix];
        } else {
            int x = head_ - bl_num;
            return buff_[x]->block[buff_[x]->first + ix];
        }
    }

    int operator[](size_t ind) const {
        size_t bl_num = ind / (128 * block_count_);
        size_t ix = ind % 128;
        if (bl_num > head_ + 1) {
            size_t x = tail_ + bl_num - head_ - 1;
            return buff_[x]->block[buff_[x]->first + ix];
        } else {
            size_t x = head_ - bl_num;
            return buff_[x]->block[buff_[x]->first + ix];
        }
    }

    size_t Size() const {
       return size_;
    }

    void Clear() {
       while (!IsEmpty()) {
           PopBack();
       }
    }

    bool IsEmpty() {
        if (size_ == 0) {
            return true;
        }
        return false;
    }


    void GrowBuff() {
        buff_ = static_cast<Block**>(realloc(buff_, std::max(4, block_count_ * 2) * sizeof(Block*)));;
        for (size_t i = tail_; i < block_count_; ++i) {
            buff_[i + block_count_] = buff_[i];
        }
        size_t cnt = sizeof(buff_);
        size_t mesure = std::max(4, block_count_ * 2) * sizeof(std::shared_ptr<Block>);
        head_ += std::max(1, block_count_ / 2);
        tail_ += std::max(1, block_count_ / 2) + 1;
        if (block_count_ == 1) {
            block_count_ = 4;
        } else {
            block_count_ *= 2;
        }
    }

private:
    Block **buff_ = static_cast<Block**>(malloc(1 * sizeof(Block*)));
    int head_ = 0;
    int tail_ = 0;
    size_t size_ = 0;
    int block_count_ = 0;
};
