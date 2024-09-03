#pragma once

#include <atomic>
#include <optional>
#include <stdexcept>
#include <utility>
#include <memory>

template <class T>
class MPSCStack {
public:
    // Push adds one element to stack top.
    //
    // Safe to call from multiple threads.
    void Push(const T& value) {
        Node* tmp = new Node(value, head_);
        while (!head_.compare_exchange_weak(tmp->next, tmp)) {
            // nothing
        }
    }

    // Pop removes top element from the stack.
    //
    // Not safe to call concurrently.
    std::optional<T> Pop() {
        Node* tmp = head_;
        if (!tmp) {
            return std::nullopt;
        }
        while (!head_.compare_exchange_weak(tmp, tmp->next)) {
            // nothing
        }
        T value = tmp->value;
        delete tmp;
        return {value};
    }

    // DequeuedAll Pop's all elements from the stack and calls cb() for each.
    //
    // Not safe to call concurrently with Pop()
    template <class TFn>
    void DequeueAll(const TFn& cb) {
        while (head_) {
            cb(Pop().value());
        }
    }

    ~MPSCStack() {
        while (head_) {
            Node* tmp = head_;
            head_ = tmp->next;
            delete tmp;
        }
    }

private:
    struct Node {
        T value;
        Node* next;
        Node(const T& value, Node* next) : value(value), next(next) {
        }
    };

    std::atomic<Node*> head_;
};
