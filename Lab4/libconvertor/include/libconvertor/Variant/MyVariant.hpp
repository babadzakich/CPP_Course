#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

template <typename... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};

template <typename... AllowedTypes>
class Variant {
 private:
  struct HolderBase {
    virtual const std::type_info& get_type() const = 0;
    virtual std::unique_ptr<HolderBase> clone() const = 0;
    virtual ~HolderBase() = default;
  };

  template <typename T>
  struct Holder : HolderBase {
    T value;
    Holder(T v) : value(std::move(v)) {}
    const std::type_info& get_type() const override { return typeid(T); }
    std::unique_ptr<HolderBase> clone() const override {
      return std::make_unique<Holder<T>>(value);
    }
  };

  std::unique_ptr<HolderBase> holder_;

  template <typename T>
  static constexpr bool is_allowed = (std::is_same_v<std::decay_t<T>, AllowedTypes> || ...);

 public:
  Variant() = default;

  template <typename T, typename = std::enable_if_t<is_allowed<std::decay_t<T>>>>
  Variant(T&& value) {
    holder_ = std::make_unique<Holder<std::decay_t<T>>>(std::forward<T>(value));
  }

  Variant(const Variant& other) {
    if (other.holder_)
      holder_ = other.holder_->clone();
  }

  Variant(Variant&& other) noexcept : holder_(std::move(other.holder_)) {}

  Variant& operator=(const Variant& other) {
    if (this != &other) {
      if (other.holder_)
        holder_ = other.holder_->clone();
      else
        holder_.reset();
    }
    return *this;
  }

  Variant& operator=(Variant&& other) noexcept {
    if (this != &other)
      holder_ = std::move(other.holder_);
    return *this;
  }
  
  template <typename T>
  bool holds_alternative() const {
    static_assert(is_allowed<T>, "T is not an allowed type");
    return holder_ && holder_->get_type() == typeid(T);
  }

  template <typename T>
  T& get() {
    static_assert(is_allowed<T>, "T is not an allowed type");
    if (!holds_alternative<T>())
      throw std::bad_variant_access();
    return static_cast<Holder<T>*>(holder_.get())->value;
  }

  template <typename T>
  const T& get() const {
    static_assert(is_allowed<T>, "T is not an allowed type");
    if (!holds_alternative<T>())
      throw std::bad_variant_access();
    return static_cast<Holder<T>*>(holder_.get())->value;
  }
};