#pragma once

#include <algorithm>

class ListHook {
public:
    ListHook() = default;

    bool IsLinked() const {
        if (prev_ != nullptr && next_ != nullptr) {
            return true;
        }
        return false;
    }

    void Unlink() {
        prev_->next_ = next_;
        next_->prev_ = prev_;
        this->prev_ = nullptr;
        this->next_ = nullptr;
    }

    ~ListHook() {
        if (IsLinked()) {
            Unlink();
        }
        delete prev_;
        delete next_;
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;
    ListHook* prev_ = nullptr;
    ListHook* next_ = nullptr;

    void LinkBefore(ListHook* dm) {
        if (dm->prev_ == nullptr) {
            dm->prev_ = this;
            this->prev_ = dm;
            this->next_ = dm;
            dm->next_ = this;
        } else if (dm->prev_->prev_ == dm) {
            this->prev_ = dm->prev_;
            this->next_ = dm;
            dm->prev_->next_ = this;
            dm->prev_ = this;
        } else {
            this->next_ = dm;
            this->prev_ = dm->prev_;
            dm->prev_->next_ = this;
            dm->prev_ = this;
        }
    }
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(T* ptr) {
            current_ = ptr;
        }
        Iterator& operator++() {
            ListHook* elem_cp = static_cast<ListHook*>(current_);
            current_ = static_cast<T*>(elem_cp->next_);
            return *this;
        }
        Iterator operator++(int) {
            auto cpy = *this;
            ListHook* elem_cp = static_cast<ListHook*>(current_);
            current_ = static_cast<T*>(elem_cp->next_);
            return cpy;
        }

        T& operator*() const {
            return *current_;
        }
        T* operator->() const {
            return current_;
        }

        bool operator==(const Iterator& rhs) const {
            return current_ == rhs.current_;
        }
        bool operator!=(const Iterator& rhs) const {
            return current_ != rhs.current_;
        }

    private:
        T* current_;
    };

    List() = default;
    List(const List&) = delete;
    List(List&& other) {
        dummy_ = std::move(other.dummy_);
        other.dummy_.next_ = nullptr;
        other.dummy_.prev_ = nullptr;
    }

    // must unlink all elements from list
    ~List() {
        if (!this->IsEmpty()) {
            if (dummy_.prev_ == dummy_.next_) {
                dummy_.next_->Unlink();
            } else {
                while (dummy_.next_ != nullptr) {
                    auto tmp = dummy_.next_;
                    dummy_.next_ = dummy_.next_->next_;
                    tmp->Unlink();
                }
            }
        }
    }

    List& operator=(const List&) = delete;
    List& operator=(List&& other) {
        dummy_ = std::move(other.dummy_);
        other.dummy_.next_ = nullptr;
        other.dummy_.prev_ = nullptr;
        return *this;
    }

    bool IsEmpty() const {
        if (dummy_.prev_ == nullptr && dummy_.next_ == nullptr) {
            return true;
        } else if (dummy_.prev_ == &dummy_ && dummy_.next_ == &dummy_) {
            return true;
        }
        return false;
    }
    // that method is allowed to be O(n)
    size_t Size() const {
        if (!this->IsEmpty()) {
            size_t sz = 1;
            auto cr = dummy_.next_;
            while (cr->next_ != &dummy_) {
                cr = cr->next_;
                sz += 1;
            }
            return sz;
        }
        return 0;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        ListHook* elem_cp = static_cast<ListHook*>(elem);
        elem_cp->LinkBefore(&dummy_);
    }
    void PushFront(T* elem) {
        ListHook* elem_cp = static_cast<ListHook*>(elem);
        if (IsEmpty()) {
            elem_cp->LinkBefore(&dummy_);
        } else {
            elem_cp->LinkBefore(dummy_.next_);
        }
    }

    T& Front() {
        T& cpy = *static_cast<T*>(dummy_.next_);
        return cpy;
    }
    const T& Front() const {
        const T& cpy = *static_cast<T*>(dummy_.next_);
        return cpy;
    }

    T& Back() {
        T& cpy = *static_cast<T*>(dummy_.prev_);
        return cpy;
    }
    const T& Back() const {
        const T& cpy = *static_cast<T*>(dummy_.prev_);
        return cpy;
    }

    void PopBack() {
        dummy_.prev_->Unlink();
        if (dummy_.prev_ == &dummy_ && dummy_.next_ == &dummy_) {
            dummy_.prev_ = nullptr;
            dummy_.next_ = nullptr;
        }
    }
    void PopFront() {
        dummy_.next_->Unlink();
        if (dummy_.prev_ == &dummy_ && dummy_.next_ == &dummy_) {
            dummy_.prev_ = nullptr;
            dummy_.next_ = nullptr;
        }
    }

    Iterator Begin() {
        return Iterator(static_cast<T*>(&dummy_));
    }
    Iterator End() {
        return Iterator(static_cast<T*>(&dummy_));
    }

    Iterator IteratorTo(T* element) {
        return Iterator(element);
    };

private:
    ListHook dummy_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
