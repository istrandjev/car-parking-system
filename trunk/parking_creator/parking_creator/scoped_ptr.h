#ifndef SCOPED_PTR_H
#define SCOPED_PTR_H

#include <cstddef>

template<class T>
class scoped_ptr {
 public:
  scoped_ptr() : ptr_(NULL) {}
  scoped_ptr(T* ptr) : ptr_(ptr) {}
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

#endif  // SCOPED_PTR_H