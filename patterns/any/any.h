#pragma once
#include <memory>
#include <utility>

class Any {
public:
    Any() = default;

    template <class T>
    Any(const T& value) {
        holder_ = std::make_unique<Inner<T>>(value);
    }
    template <class T>
    Any& operator=(const T& value) {
        holder_ = std::make_unique<Inner<T>>(value);
        return *this;
    }

    Any(const Any& rhs) : holder_(rhs.holder_->Clone()) {
    }
    Any& operator=(const Any& rhs) {
        Any tmp = Any(rhs);
        std::swap(holder_, tmp.holder_);
        return *this;
    }
    ~Any() {
        holder_ = nullptr;
    }

    bool Empty() const {
        return holder_ == nullptr;
    }

    void Clear() {
        holder_ = nullptr;
    }
    void Swap(Any& rhs) {
        std::swap(holder_, rhs.holder_);
    }

    template <class T>
    const T& GetValue() const {
        if (holder_->GetType() != typeid(T)) {
            throw std::bad_cast();
        } else {
            return dynamic_cast<Inner<T>&>(*holder_).t_;
        }
    }

private:
    struct InnerBase {
        virtual ~InnerBase() {
        }
        virtual const std::type_info& GetType() const = 0;
        virtual InnerBase* Clone() const = 0;
    };

    template <typename T>
    struct Inner : InnerBase {
        Inner(const T& t) {
            t_ = t;
        }
        const std::type_info& GetType() const override {
            return typeid(T);
        }
        InnerBase* Clone() const override {
            return new Inner(t_);
        }
        T t_;
    };
    std::unique_ptr<InnerBase> holder_ = nullptr;
};
