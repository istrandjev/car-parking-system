#ifndef INCLUDE_UTILS_SCOPED_PTR_H_
#define INCLUDE_UTILS_SCOPED_PTR_H_

#include <cstddef>

template<class T>
class scoped_ptr {
 public:
  scoped_ptr() : ptr_(NULL) {}
  explicit scoped_ptr(T* ptr) : ptr_(ptr) {}
  ~scoped_ptr() {
    delete ptr_;
  }

  T& operator*() {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T* operator->() const {
    return ptr_;
  }

  T* get() {
    return ptr_;
  }

  void reset(T *ptr) {
    ptr_ = ptr;
  }

 private:
  T *ptr_;
};

#endif  // INCLUDE_UTILS_SCOPED_PTR_H_
