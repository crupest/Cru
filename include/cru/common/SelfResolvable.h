#pragma once

#include <cassert>
#include <memory>

namespace cru {
template <typename T>
class SelfResolvable;

template <typename T>
class ObjectResolver {
  friend SelfResolvable<T>;

 private:
  explicit ObjectResolver(T* o) : shared_object_ptr_(new T*(o)) {}

 public:
  ObjectResolver(const ObjectResolver&) = default;
  ObjectResolver& operator=(const ObjectResolver&) = default;
  ObjectResolver(ObjectResolver&&) = default;
  ObjectResolver& operator=(ObjectResolver&&) = default;
  ~ObjectResolver() = default;

  bool IsValid() const { return this->shared_object_ptr_ != nullptr; }

  T* Resolve() const {
    assert(IsValid());
    return *this->shared_object_ptr_;
  }

  explicit operator bool() const { return this->IsValid(); }

 private:
  void SetResolvedObject(T* o) {
    assert(IsValid());
    *this->shared_object_ptr_ = o;
  }

 private:
  std::shared_ptr<T*> shared_object_ptr_;
};

template <typename T>
class SelfResolvable {
 public:
  SelfResolvable() : resolver_(static_cast<T*>(this)) {}
  SelfResolvable(const SelfResolvable&) = delete;
  SelfResolvable& operator=(const SelfResolvable&) = delete;

  // Resolvers to old object will resolve to new object.
  SelfResolvable(SelfResolvable&& other)
      : resolver_(std::move(other.resolver_)) {
    this->resolver_.SetResolvedObject(this);
  }

  // Old resolvers for this object will resolve to nullptr.
  // Other's resolvers will now resolve to this.
  SelfResolvable& operator=(SelfResolvable&& other) {
    if (this != &other) {
      this->resolver_ = std::move(other.resolver_);
      this->resolver_.SetResolvedObject(this);
    }
    return *this;
  }

  virtual ~SelfResolvable() {
    if (this->resolver_.IsValid()) {
      this->resolver_.SetResolvedObject(nullptr);
    }
  }

  ObjectResolver<T> CreateResolver() { return resolver_; }

 private:
  ObjectResolver<T> resolver_;
};
}  // namespace cru
