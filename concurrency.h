#ifndef RE_DEFINED_CONCURRENCY_H
#define RE_DEFINED_CONCURRENCY_H

#include "base.h"
#include "allocator.h"

// atomic
namespace re {

template <class T>
struct atomic;
using atomic_bool = atomic<bool>;
using atomic_char = atomic<char>;
using atomic_schar = atomic<signed char>;
using atomic_uchar = atomic<unsigned char>;
using atomic_short = atomic<short>;
using atomic_ushort = atomic<unsigned short>;
using atomic_int = atomic<int>;
using atomic_uint = atomic<unsigned int>;
using atomic_long = atomic<long>;
using atomic_ulong = atomic<unsigned long>;
using atomic_llong = atomic<long long>;
using atomic_ullong = atomic<unsigned long long>;
using atomic_char8_t = atomic<char8_t>;
using atomic_char16_t = atomic<char16_t>;
using atomic_char32_t = atomic<char32_t>;
using atomic_wchar_t = atomic<wchar_t>;
using atomic_int8_t = atomic<int8_t>;
using atomic_uint8_t = atomic<uint8_t>;
using atomic_int16_t = atomic<int16_t>;
using atomic_uint16_t = atomic<uint16_t>;
using atomic_int32_t = atomic<int32_t>;
using atomic_uint32_t = atomic<uint32_t>;
using atomic_int64_t = atomic<int64_t>;
using atomic_uint64_t = atomic<uint64_t>;
using atomic_int_least8_t = atomic<int_least8_t>;
using atomic_uint_least8_t = atomic<uint_least8_t>;
using atomic_int_least16_t = atomic<int_least16_t>;
using atomic_uint_least16_t = atomic<uint_least16_t>;
using atomic_int_least32_t = atomic<int_least32_t>;
using atomic_uint_least32_t = atomic<uint_least32_t>;
using atomic_int_least64_t = atomic<int_least64_t>;
using atomic_uint_least64_t = atomic<uint_least64_t>;
using atomic_int_fast8_t = atomic<int_fast8_t>;
using atomic_uint_fast8_t = atomic<uint_fast8_t>;
using atomic_int_fast16_t = atomic<int_fast16_t>;
using atomic_uint_fast16_t = atomic<uint_fast16_t>;
using atomic_int_fast32_t = atomic<int_fast32_t>;
using atomic_uint_fast32_t = atomic<uint_fast32_t>;
using atomic_int_fast64_t = atomic<int_fast64_t>;
using atomic_uint_fast64_t = atomic<uint_fast64_t>;
using atomic_intptr_t = atomic<intptr_t>;
using atomic_uintptr_t = atomic<uintptr_t>;
using atomic_size_t = atomic<size_t>;
using atomic_ptrdiff_t = atomic<ptrdiff_t>;
using atomic_intmax_t = atomic<intmax_t>;
using atomic_uintmax_t = atomic<uintmax_t>;

template <>
struct atomic<int> {
  using value_type = long;

private:
  mutable volatile long v;

public:
  inline atomic() noexcept : v() {}
  ~atomic() = default;
  atomic(const atomic &) = delete;
  atomic &operator =(const atomic &) = delete;
  atomic(atomic &&) = delete;
  atomic &operator =(atomic &&) = delete;

  atomic &operator =(const atomic &) volatile = delete;

  inline atomic(int x) noexcept : v(x) {}
  inline int operator =(int x) noexcept {
    _InterlockedExchange(&v, (long)x);
    return x;
  }
  inline int operator =(int x) volatile noexcept {
    _InterlockedExchange(&v, (long)x);
    return x;
  }
  inline operator int() const noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline operator int() const volatile noexcept {
    return _InterlockedOr(&v, 0l);
  }

  inline int load() const noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline int load() const volatile noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline void store(int x) noexcept {
    _InterlockedExchange(&v, (long)x);
  }
  inline void store(int x) volatile noexcept {
    _InterlockedExchange(&v, (long)x);
  }
  inline int exchange(int x) noexcept {
    return _InterlockedExchange(&v, (long)x);
  }
  inline int exchange(int x) volatile noexcept {
    return _InterlockedExchange(&v, (long)x);
  }

  inline bool compare_exchange(int &expected,
                               int desired) noexcept {
    if (const int x = _InterlockedCompareExchange(&v,
                                                  (long)desired,
                                                  (long)expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline bool compare_exchange(int &expected,
                               int desired) volatile noexcept {
    if (const int x = _InterlockedCompareExchange(&v,
                                                  (long)desired,
                                                  (long)expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline int fetch_add(int x) noexcept {
    return _InterlockedExchangeAdd(&v, (long)x);
  }
  inline int fetch_add(int x) volatile noexcept {
    return _InterlockedExchangeAdd(&v, (long)x);
  }
  inline int fetch_sub(int x) noexcept {
    return _InterlockedExchangeAdd(&v, (long)-x);
  }
  inline int fetch_sub(int x) volatile noexcept {
    return _InterlockedExchangeAdd(&v, (long)-x);
  }

  inline int operator ++() noexcept {
    return _InterlockedIncrement(&v);
  }
  inline int operator ++() volatile noexcept {
    return _InterlockedIncrement(&v);
  }
  inline int operator ++(int) noexcept {
    return _InterlockedExchangeAdd(&v, 1l);
  }
  inline int operator ++(int) volatile noexcept {
    return _InterlockedExchangeAdd(&v, 1l);
  }

  inline int operator --() noexcept {
    return _InterlockedDecrement(&v);
  }
  inline int operator --() volatile noexcept {
    return _InterlockedDecrement(&v);
  }
  inline int operator --(int) noexcept {
    return _InterlockedExchangeAdd(&v, -1l);
  }
  inline int operator --(int) volatile noexcept {
    return _InterlockedExchangeAdd(&v, -1l);
  }

  inline int operator +=(int x) noexcept {
    return InterlockedAdd(&v, (long)x);
  }
  inline int operator +=(int x) volatile noexcept {
    return InterlockedAdd(&v, (long)x);
  }
  inline int operator -=(int x) noexcept {
    return InterlockedAdd(&v, (long)-x);
  }
  inline int operator -=(int x) volatile noexcept {
    return InterlockedAdd(&v, (long)-x);
  }

  inline int operator &=(int x) noexcept {
    return _InterlockedAnd(&v, (long)x) & x;
  }
  inline int operator &=(int x) volatile noexcept {
    return _InterlockedAnd(&v, (long)x) & x;
  }
  inline int operator |=(int x) noexcept {
    return _InterlockedOr(&v, (long)x) | x;
  }
  inline int operator |=(int x) volatile noexcept {
    return _InterlockedOr(&v, (long)x) | x;
  }
  inline int operator ^=(int x) noexcept {
    return _InterlockedXor(&v, (long)x) ^ x;
  }
  inline int operator ^=(int x) volatile noexcept {
    return _InterlockedXor(&v, (long)x) ^ x;
  }
};

template <>
struct atomic<long> {
  using value_type = long;

private:
  mutable volatile long v;

public:
  inline atomic() noexcept : v() {}
  ~atomic() = default;
  atomic(const atomic &) = delete;
  atomic &operator =(const atomic &) = delete;
  atomic(atomic &&) = delete;
  atomic &operator =(atomic &&) = delete;

  atomic &operator =(const atomic &) volatile = delete;

  inline atomic(long x) noexcept : v(x) {}
  inline long operator =(long x) noexcept {
    _InterlockedExchange(&v, x);
    return x;
  }
  inline long operator =(long x) volatile noexcept {
    _InterlockedExchange(&v, x);
    return x;
  }
  inline operator long() const noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline operator long() const volatile noexcept {
    return _InterlockedOr(&v, 0l);
  }

  inline long load() const noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline long load() const volatile noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline void store(long x) noexcept {
    _InterlockedExchange(&v, x);
  }
  inline void store(long x) volatile noexcept {
    _InterlockedExchange(&v, x);
  }
  inline long exchange(long x) noexcept {
    return _InterlockedExchange(&v, x);
  }
  inline long exchange(long x) volatile noexcept {
    return _InterlockedExchange(&v, x);
  }

  inline bool compare_exchange(long &expected,
                               long desired) noexcept {
    if (const long x = _InterlockedCompareExchange(&v,
                                                   desired,
                                                   expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline bool compare_exchange(long &expected,
                               long desired) volatile noexcept {
    if (const long x = _InterlockedCompareExchange(&v,
                                                   desired,
                                                   expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline long fetch_add(long x) noexcept {
    return _InterlockedExchangeAdd(&v, x);
  }
  inline long fetch_add(long x) volatile noexcept {
    return _InterlockedExchangeAdd(&v, x);
  }
  inline long fetch_sub(long x) noexcept {
    return _InterlockedExchangeAdd(&v, -x);
  }
  inline long fetch_sub(long x) volatile noexcept {
    return _InterlockedExchangeAdd(&v, -x);
  }

  inline long operator ++() noexcept {
    return _InterlockedIncrement(&v);
  }
  inline long operator ++() volatile noexcept {
    return _InterlockedIncrement(&v);
  }
  inline long operator ++(int) noexcept {
    return _InterlockedExchangeAdd(&v, 1l);
  }
  inline long operator ++(int) volatile noexcept {
    return _InterlockedExchangeAdd(&v, 1l);
  }

  inline long operator --() noexcept {
    return _InterlockedDecrement(&v);
  }
  inline long operator --() volatile noexcept {
    return _InterlockedDecrement(&v);
  }
  inline long operator --(int) noexcept {
    return _InterlockedExchangeAdd(&v, -1l);
  }
  inline long operator --(int) volatile noexcept {
    return _InterlockedExchangeAdd(&v, -1l);
  }

  inline long operator +=(long x) noexcept {
    return InterlockedAdd(&v, x);
  }
  inline long operator +=(long x) volatile noexcept {
    return InterlockedAdd(&v, x);
  }
  inline long operator -=(long x) noexcept {
    return InterlockedAdd(&v, -x);
  }
  inline long operator -=(long x) volatile noexcept {
    return InterlockedAdd(&v, -x);
  }

  inline long operator &=(long x) noexcept {
    return _InterlockedAnd(&v, x) & x;
  }
  inline long operator &=(long x) volatile noexcept {
    return _InterlockedAnd(&v, x) & x;
  }
  inline long operator |=(long x) noexcept {
    return _InterlockedOr(&v, x) | x;
  }
  inline long operator |=(long x) volatile noexcept {
    return _InterlockedOr(&v, x) | x;
  }
  inline long operator ^=(long x) noexcept {
    return _InterlockedXor(&v, x) ^ x;
  }
  inline long operator ^=(long x) volatile noexcept {
    return _InterlockedXor(&v, x) ^ x;
  }
};

template <class LL>
requires (is_same_v<LL, long long> && sizeof(size_t) == 8u)
struct atomic<LL> {
  using value_type = LL;

private:
  mutable volatile LL v;

public:
  inline atomic() noexcept : v() {}
  ~atomic() = default;
  atomic(const atomic &) = delete;
  atomic &operator =(const atomic &) = delete;
  atomic(atomic &&) = delete;
  atomic &operator =(atomic &&) = delete;

  atomic &operator =(const atomic &) volatile = delete;

  inline atomic(LL x) noexcept : v(x) {}
  inline LL operator =(LL x) noexcept {
    _InterlockedExchange64(&v, x);
    return x;
  }
  inline LL operator =(LL x) volatile noexcept {
    _InterlockedExchange64(&v, x);
    return x;
  }
  inline operator LL() const noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline operator LL() const volatile noexcept {
    return _InterlockedOr64(&v, 0l);
  }

  inline LL load() const noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline LL load() const volatile noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline void store(LL x) noexcept {
    _InterlockedExchange64(&v, x);
  }
  inline void store(LL x) volatile noexcept {
    _InterlockedExchange64(&v, x);
  }
  inline LL exchange(LL x) noexcept {
    return _InterlockedExchange64(&v, x);
  }
  inline LL exchange(LL x) volatile noexcept {
    return _InterlockedExchange64(&v, x);
  }

  inline bool compare_exchange(LL &expected,
                               LL desired) noexcept {
    if (const LL x = _InterlockedCompareExchange64(&v,
                                                   desired,
                                                   expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline bool compare_exchange(LL &expected, LL desired) volatile noexcept {
    if (const LL x = _InterlockedCompareExchange64(&v,
                                                   desired,
                                                   expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline LL fetch_add(LL x) noexcept {
    return _InterlockedExchangeAdd64(&v, x);
  }
  inline LL fetch_add(LL x) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, x);
  }
  inline LL fetch_sub(LL x) noexcept {
    return _InterlockedExchangeAdd64(&v, -x);
  }
  inline LL fetch_sub(LL x) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, -x);
  }

  inline LL operator ++() noexcept {
    return _InterlockedIncrement64(&v);
  }
  inline LL operator ++() volatile noexcept {
    return _InterlockedIncrement64(&v);
  }
  inline LL operator ++(int) noexcept {
    return _InterlockedExchangeAdd64(&v, 1l);
  }
  inline LL operator ++(int) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, 1l);
  }

  inline LL operator --() noexcept {
    return _InterlockedDecrement64(&v);
  }
  inline LL operator --() volatile noexcept {
    return _InterlockedDecrement64(&v);
  }
  inline LL operator --(int) noexcept {
    return _InterlockedExchangeAdd64(&v, -1l);
  }
  inline LL operator --(int) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, -1l);
  }

  inline LL operator +=(LL x) noexcept {
    return InterlockedAdd64(&v, x);
  }
  inline LL operator +=(LL x) volatile noexcept {
    return InterlockedAdd64(&v, x);
  }
  inline LL operator -=(LL x) noexcept {
    return InterlockedAdd64(&v, -x);
  }
  inline LL operator -=(LL x) volatile noexcept {
    return InterlockedAdd64(&v, -x);
  }

  inline LL operator &=(LL x) noexcept {
    return _InterlockedAnd64(&v, x) & x;
  }
  inline LL operator &=(LL x) volatile noexcept {
    return _InterlockedAnd64(&v, x) & x;
  }
  inline LL operator |=(LL x) noexcept {
    return _InterlockedOr64(&v, x) | x;
  }
  inline LL operator |=(LL x) volatile noexcept {
    return _InterlockedOr64(&v, x) | x;
  }
  inline LL operator ^=(LL x) noexcept {
    return _InterlockedXor64(&v, x) ^ x;
  }
  inline LL operator ^=(LL x) volatile noexcept {
    return _InterlockedXor64(&v, x) ^ x;
  }
};

template <class T>
struct atomic<T *> {
private:
  mutable void *volatile v;

public:
  using value_type = T *;
  using difference_type = ptrdiff_t;

  inline atomic() noexcept : v() {}
  ~atomic() = default;
  atomic(const atomic &) = delete;
  atomic &operator =(const atomic &) = delete;
  atomic(atomic &&) = delete;
  atomic &operator =(atomic &&) = delete;

  atomic &operator =(const atomic &) volatile = delete;

  inline void store(T *p) noexcept {
    _InterlockedExchangePointer(&v, (void *)p);
  }
  inline void store(T *p) volatile noexcept {
    _InterlockedExchangePointer(&v, (void *)p);
  }
  inline T *load() noexcept {
    return (T *)_InterlockedCompareExchangePointer(&v, nullptr, nullptr);
  }
  inline T *load() volatile noexcept {
    return (T *)_InterlockedCompareExchangePointer(&v, nullptr, nullptr);
  }
  inline operator T *() const noexcept {
    return (T *)_InterlockedCompareExchangePointer(&v, nullptr, nullptr);
  }
  inline operator T *() const volatile noexcept {
    return (T *)_InterlockedCompareExchangePointer(&v, nullptr, nullptr);
  }
  inline T *operator =(T *p) noexcept {
    _InterlockedExchangePointer(&v, (void *)p);
    return p;
  }
  inline T *operator =(T *p) volatile noexcept {
    _InterlockedExchangePointer(&v, (void *)p);
    return p;
  }

  inline T *exchange(T *p) noexcept {
    return (T *)(void *)_InterlockedExchangePointer(&v, (void *)p);
  }
  inline T *exchange(T *p) volatile noexcept {
    return (T *)(void *)_InterlockedExchangePointer(&v, (void *)p);
  }
  inline bool compare_exchange(T *&expected, T *desired) noexcept {
    if (const auto x = _InterlockedCompareExchangePointer(&v,
                                                          (void *)desired,
                                                          (void *)expected);
        x != (void *)expected) {
      expected = (T *)x;
      return false;
    }
    else
      return true;
  }
  inline bool compare_exchange(T *&expected, T *desired) volatile noexcept {
    if (const auto x = _InterlockedCompareExchangePointer(&v,
                                                          (void *)desired,
                                                          (void *)expected);
        x != (void *)expected) {
      expected = (T *)x;
      return false;
    }
    else
      return true;
  }
};

struct atomic_flag {
private:
  mutable volatile long v;

public:
  inline atomic_flag() noexcept : v() {}
  ~atomic_flag() = default;
  atomic_flag(const atomic_flag &) = delete;
  atomic_flag &operator =(const atomic_flag &) = delete;
  atomic_flag(atomic_flag &&) = delete;
  atomic_flag &operator =(atomic_flag &&) = delete;

  atomic_flag &operator =(const atomic_flag &) volatile = delete;

  inline bool test() const noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline bool test() const volatile noexcept {
    return _InterlockedOr(&v, 0l);
  }
  inline bool test_and_set() noexcept {
    return _InterlockedExchange(&v, 1l);
  }
  inline bool test_and_set() volatile noexcept {
    return _InterlockedExchange(&v, 1l);
  }
  inline void clear() noexcept {
    _InterlockedExchange(&v, 0l);
  }
  inline void clear() volatile noexcept {
    _InterlockedExchange(&v, 0l);
  }
};

}

// shared_ptr
namespace re {

class bad_weak_ptr : public exception {
public:
  const char *what() const noexcept override {
    return "re::bad_weak_ptr";
  }
};

template <class>
class shared_ptr;
template <class>
class weak_ptr;
template <class>
class enable_shared_from_this;
template <class>
struct fo_make_shared;
template <class>
struct fo_allocate_shared;
template <class>
struct fo_make_shared_for_overwrite;
template <class>
struct fo_allocate_shared_for_overwrite;

namespace inner {

class shared_ptr_control_block {
  using this_t = shared_ptr_control_block;

protected:
  atomic<long> counter;

public:
  shared_ptr_control_block() = default;
  virtual ~shared_ptr_control_block() = default;
  shared_ptr_control_block(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  shared_ptr_control_block(this_t &&) = delete;
  this_t &operator =(this_t &&) = delete;

  explicit shared_ptr_control_block(long c) noexcept : counter(c) {}

  virtual void clear() noexcept = 0;
  long count() const noexcept {
    return counter.load();
  }
  bool plus_1() noexcept {
    if (counter++ == numeric_limits<long>::max()) {
      --counter;
      return true;
    }
    else
      return false;
  }
  bool minus_1() noexcept {
    return --counter == 0;
  }

private:
  virtual void *deleter_stored_ptr() noexcept = 0;
  virtual const type_info &deleter_typeid() noexcept = 0;
public:
  template <class D>
  D *get_deleter() noexcept requires is_same_v<remove_cv_t<D>, D> {
    void *const p = deleter_stored_ptr();
    if (p == nullptr)
      return nullptr;
    if (typeid(D) == deleter_typeid())
      return reinterpret_cast<D *>(p);
    else
      return nullptr;
  }
};
template <class T, class D = default_delete<T>, class A = default_allocator<T>>
class shared_ptr_control_block_impl
  : public shared_ptr_control_block
  , public derivable_wrapper<D>
  , public allocator_wrapper<A> {
  using this_t = shared_ptr_control_block_impl;
  using alw_t = allocator_wrapper<A>;
  using base_t = shared_ptr_control_block;
  using base_t::counter;

  using element_t = remove_extent_t<T>;
  element_t *p;

public:
  shared_ptr_control_block_impl() = default;
  virtual ~shared_ptr_control_block_impl() override = default;
  shared_ptr_control_block_impl(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  shared_ptr_control_block_impl(this_t &&) = delete;
  this_t &operator =(this_t &&) = delete;

  explicit shared_ptr_control_block_impl(element_t *pp)
    : base_t(1)
    , derivable_wrapper<D>{}
    , alw_t{}
    , p(pp) {}
  shared_ptr_control_block_impl(element_t *pp, D d)
    : base_t(1)
    , derivable_wrapper<D>(forward<D>(d))
    , alw_t{}
    , p(pp) {}
  shared_ptr_control_block_impl(element_t *pp, D d, A a)
    : base_t(1)
    , derivable_wrapper<D>(forward<D>(d))
    , alw_t(a)
    , p(pp) {}

  virtual void clear() noexcept override {
    (*static_cast<derivable_wrapper<D> &>(*this))(p);
    alw_t::template rebind<this_t>().delete_1(this);
  }

  element_t *get() const noexcept {
    return p;
  }
  using base_t::count;
  using base_t::plus_1;
  using base_t::minus_1;

private:
  void *deleter_stored_ptr_impl() noexcept {
    return addressof(derivable_wrapper<D>::operator *());
  }
  void *deleter_stored_ptr_impl() noexcept
    requires (!is_same_v<remove_cv_t<D>, D>) {
    return nullptr;
  }
  virtual void *deleter_stored_ptr() noexcept override {
    return deleter_stored_ptr_impl();
  }
  virtual const type_info &deleter_typeid() noexcept override {
    return typeid(D);
  }
};

}

template <class T>
class weak_ptr {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;
  template <class>
  friend class enable_shared_from_this;

  using this_t = weak_ptr;

  remove_extent_t<T> *p;
  inner::shared_ptr_control_block *cb;

public:
  using element_type = remove_extent_t<T>;

  weak_ptr() noexcept : p{}, cb{} {}
  ~weak_ptr() = default;
  weak_ptr(const weak_ptr &) noexcept = default;
  weak_ptr &operator =(const weak_ptr &) noexcept = default;
  weak_ptr(weak_ptr &&x) noexcept : p(x.p), cb(x.cb) {
    x.p = nullptr;
    x.cb = nullptr;
  }
  weak_ptr &operator =(weak_ptr &&x) noexcept {
    p = exchange(x.p, nullptr);
    cb = exchange(x.cb, nullptr);
    return *this;
  }
  friend void swap(weak_ptr &x, weak_ptr &y) noexcept {
    adl_swap(x.p, y.p);
    adl_swap(x.cb, y.cb);
  }

  template<class Y>
  weak_ptr(const weak_ptr<Y> &x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>)
    : p(x.p), cb(x.cb) {}
  template <class Y>
  weak_ptr &operator =(const weak_ptr<Y> &x) noexcept {
    copy_and_swap(this_t(x), *this);
    return *this;
  }

  template <class Y>
  weak_ptr(weak_ptr<Y> &&x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>)
    : p(x.p), cb(x.cb) {
    x.p = nullptr;
    x.cb = nullptr;
  }
  template <class Y>
  weak_ptr &operator =(weak_ptr<Y> &&x) noexcept {
    copy_and_swap(this_t(move(x)), *this);
    return *this;
  }

  template <class Y>
  weak_ptr(const shared_ptr<Y> &x) noexcept
    requires is_compatible_pointer_with_v<Y *, T *>
    : p(x.p), cb(x.cb) {}
  template <class Y>
  weak_ptr &operator =(const shared_ptr<Y> &x) noexcept
    requires is_compatible_pointer_with_v<Y *, T *> {
    copy_and_swap(this_t(x), *this);
    return *this;
  }

  void reset() noexcept {
    p = nullptr;
    cb = nullptr;
  }
  long use_count() const noexcept {
    return cb == nullptr ? 0 : cb->count();
  }
  bool expired() const noexcept {
    return use_count() == 0;
  }
  shared_ptr<T> lock() const noexcept {
    return expired() ? shared_ptr<T>{} : shared_ptr<T>(*this);
  }

  template<class U>
  bool owner_before(const shared_ptr<U> &b) const noexcept {
    return cb < b.cb;
  }
  template<class U>
  bool owner_before(const weak_ptr<U> &b) const noexcept {
    return cb < b.cb;
  }
};
template <class T>
weak_ptr(shared_ptr<T>)->weak_ptr<T>;

template <class T = void>
struct owner_less;
template <class T>
struct owner_less<shared_ptr<T>> {
  bool operator()(const shared_ptr<T> &x,
                  const shared_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
  bool operator()(const shared_ptr<T> &x,
                  const weak_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
  bool operator()(const weak_ptr<T> &x,
                  const shared_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
};
template <class T>
struct owner_less<weak_ptr<T>> {
  bool operator()(const weak_ptr<T> &x,
                  const weak_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
  bool operator()(const shared_ptr<T> &x,
                  const weak_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
  bool operator()(const weak_ptr<T> &x,
                  const shared_ptr<T> &y) const noexcept {
    return x.owner_before(y);
  }
};
template <>
struct owner_less<void> {
  using is_transparent = inner::transparent_tag;

  template<class T, class U>
  bool operator()(const shared_ptr<T> &x,
                  const shared_ptr<U> &y) const noexcept {
    return x.owner_before(y);
  }
  template<class T, class U>
  bool operator()(const shared_ptr<T> &x,
                  const weak_ptr<U> &y) const noexcept {
    return x.owner_before(y);
  }
  template<class T, class U>
  bool operator()(const weak_ptr<T> &x,
                  const shared_ptr<U> &y) const noexcept {
    return x.owner_before(y);
  }
  template<class T, class U>
  bool operator()(const weak_ptr<T> &x,
                  const weak_ptr<U> &y) const noexcept {
    return x.owner_before(y);
  }
};

template <class T>
class enable_shared_from_this {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;
  template <class>
  friend class enable_shared_from_this;

  using this_t = enable_shared_from_this;

  using t = T;

  mutable weak_ptr<T> weak_this;

protected:
  constexpr enable_shared_from_this() noexcept : weak_this{} {}
  ~enable_shared_from_this() = default;
  enable_shared_from_this(const this_t &) noexcept : weak_this{} {}
  this_t &operator =(const this_t &) noexcept {
    return *this;
  }
  enable_shared_from_this(this_t &&) noexcept : weak_this{} {}
  this_t &operator =(this_t &&) noexcept {
    return *this;
  }

public:
  shared_ptr<T> shared_from_this() {
    return shared_ptr<T>(weak_this);
  }
  shared_ptr<const T> shared_from_this() const {
    return shared_ptr<const T>(weak_this);
  }
  weak_ptr<T> weak_from_this() noexcept {
    return weak_ptr<T>(weak_this);
  }
  weak_ptr<const T> weak_from_this() const noexcept {
    return weak_ptr<const T>(weak_this);
  }
};

namespace inner::fns {

template <class T>
enable_shared_from_this<T> *
get_enable_shared_from_this(enable_shared_from_this<T> *p) noexcept {
  return p;
}

}
template <class T>
class shared_ptr {
  template <class>
  friend struct fo_make_shared;
  template <class>
  friend struct fo_allocate_shared;
  template <class>
  friend struct fo_make_shared_for_overwrite;
  template <class>
  friend struct fo_allocate_shared_for_overwrite;

  template <class>
  friend struct fo_get_deleter;

  template <class>
  friend class weak_ptr;
  template <class>
  friend class shared_ptr;
  template <class>
  friend class enable_shared_from_this;

  using this_t = shared_ptr;

  using cb_t = inner::shared_ptr_control_block;

  remove_extent_t<T> *p;
  cb_t *cb;

public:
  using element_type = remove_extent_t<T>;
  using weak_type = weak_ptr<T>;

  constexpr shared_ptr() noexcept : p{}, cb{} {}
  ~shared_ptr() {
    if (cb != nullptr) {
      if (cb->minus_1())
        cb->clear();
    }
  }
  shared_ptr(const shared_ptr &x) noexcept : p(x.p), cb(x.cb) {
    if (cb != nullptr) {
      if (cb->plus_1())
        throw_or_terminate<length_error>
          ("re::shared_ptr<T>::copy_constructor: ref-count overflow\n");
    }
  }
  shared_ptr &operator =(const shared_ptr &x) noexcept {
    copy_and_swap(x, *this);
    return *this;
  }
  shared_ptr(shared_ptr &&x) noexcept : p(x.p), cb(x.cb) {
    x.p = nullptr;
    x.cb = nullptr;
  }
  shared_ptr &operator =(shared_ptr &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(this_t &x, this_t &y) noexcept {
    adl_swap(x.p, y.p);
    adl_swap(x.cb, y.cb);
  }

  constexpr shared_ptr(nullptr_t) noexcept : shared_ptr{} {}
  void reset() noexcept {
    copy_and_swap(this_t{}, *this);
  }

private:
  template <class Y>
  void enable_shrd_from_this(Y *) {}
  template <class Y>
  void enable_shrd_from_this(Y *pp)
    requires requires {
      inner::fns::get_enable_shared_from_this(pp);
      requires is_convertible_v<remove_cv_t<Y> *, T *>;
    } {
    const auto enable_shard_p = inner::fns::get_enable_shared_from_this(pp);
    enable_shard_p->weak_this
      = shared_ptr<remove_cv_t<Y>>(*this, const_cast<remove_cv_t<Y> *>(pp));
  }
public:
  template <class Y>
  explicit shared_ptr(Y *pp)
    requires (!is_array_v<T> && is_convertible_v<Y *, T *>)
    : p(pp)
    , cb(default_alloc_wrapper<inner::shared_ptr_control_block_impl<Y>>()
         .new_1(pp)) {
    enable_shrd_from_this(pp);
  }
  template <class Y>
  explicit shared_ptr(Y *pp)
    requires (is_unbounded_array_v<T>
              && is_convertible_v<Y (*)[], T *>)
    : p(pp)
    , cb(default_alloc_wrapper<inner::shared_ptr_control_block_impl<Y []>>()
         .new_1(pp)) {}
  template <class Y>
  explicit shared_ptr(Y *pp)
    requires (is_bounded_array_v<T>
              && is_convertible_v<Y (*)[extent_v<T>], T *>)
    : p(pp)
    , cb(default_alloc_wrapper
         <inner::shared_ptr_control_block_impl<Y [extent_v<T>]>>()
         .new_1(pp)) {}
  template <class Y>
  void reset(Y *pp) {
    copy_and_swap(this_t(pp), *this);
  }

  template <class Y, class D>
  shared_ptr(Y *pp, D d)
    requires requires {
      requires !is_array_v<T>;
      requires is_convertible_v<Y *, T *>;
      requires is_move_constructible_v<D>;
      d(pp);
    }
    : p(pp)
    , cb(default_alloc_wrapper
         <inner::shared_ptr_control_block_impl<Y, D>>()
         .new_1(pp, move(d))) {
    enable_shrd_from_this(pp);
  }
  template <class Y, class D>
  shared_ptr(Y *pp, D d)
    requires requires {
      requires is_unbounded_array_v<T>;
      requires is_convertible_v<Y (*)[], T *>;
      requires is_move_constructible_v<D>;
      d(pp);
    }
    : p(pp)
    , cb(default_alloc_wrapper
         <inner::shared_ptr_control_block_impl<Y [], D>>()
         .new_1(pp, move(d))) {}
  template <class Y, class D>
  shared_ptr(Y *pp, D d)
    requires requires {
      requires is_bounded_array_v<T>;
      requires is_convertible_v<Y (*)[extent_v<T>], T *>;
      requires is_move_constructible_v<D>;
      d(pp);
    }
    : p(pp)
    , cb(default_alloc_wrapper
         <inner::shared_ptr_control_block_impl<Y [extent_v<T>], D>>()
         .new_1(pp, move(d))) {}
  template <class D>
  shared_ptr(nullptr_t, D d)
    requires requires {d(p);}
    : p(nullptr)
    , cb(default_alloc_wrapper
         <inner::shared_ptr_control_block_impl<T, D>>()
         .new_1(nullptr, move(d))) {}
  template <class Y, class D>
  void reset(Y *pp, D d) {
    copy_and_swap(this_t(pp, move(d)), *this);
  }

  template <class Y, class D, class A>
  shared_ptr(Y *pp, D d, A a)
    requires requires {
      requires !is_array_v<T>;
      requires is_convertible_v<Y *, T *>;
      requires is_move_constructible_v<D>;
      d(pp);
      requires is_pointer_v<alloc_ptr<A>>;
    }
    : p(pp)
    , cb(allocator_wrapper<A>(a)
         .template rebind
         <inner::shared_ptr_control_block_impl<Y, D, alloc_rebind<A, Y>>>()
         .new_1(pp, move(d), a)) {
    enable_shrd_from_this(pp);
  }
  template <class Y, class D, class A>
  shared_ptr(Y *pp, D d, A a)
    requires requires {
      requires is_unbounded_array_v<T>;
      requires is_convertible_v<Y (*)[], T *>;
      requires is_move_constructible_v<D>;
      d(pp);
      requires is_pointer_v<alloc_ptr<A>>;
    }
    : p(pp)
    , cb(allocator_wrapper<A>(a)
         .template rebind
         <inner::shared_ptr_control_block_impl<Y [], D, alloc_rebind<A, Y>>>()
         .new_1(pp, move(d), a)) {}
  template <class Y, class D, class A>
  shared_ptr(Y *pp, D d, A a)
    requires requires {
      requires is_bounded_array_v<T>;
      requires is_convertible_v<Y (*)[extent_v<T>], T *>;
      requires is_move_constructible_v<D>;
      d(pp);
      requires is_pointer_v<alloc_ptr<A>>;
    }
    : p(pp)
    , cb(allocator_wrapper<A>(a)
         .template rebind
         <inner::shared_ptr_control_block_impl
          <Y [extent_v<T>], D, alloc_rebind<A, Y>>>()
         .new_1(pp, move(d), a)) {}
  template <class D, class A>
  shared_ptr(nullptr_t, D d, A a)
    requires requires {
      requires is_move_constructible_v<D>;
      d(p);
      requires is_pointer_v<alloc_ptr<A>>;
    }
    : p{}, cb(allocator_wrapper<A>(a)
              .template rebind
              <inner::shared_ptr_control_block_impl<T, D, alloc_rebind<A, T>>>()
              .new_1(nullptr, move(d), a)) {}
  template <class Y, class D, class A>
  void reset(Y *pp, D d, A a) {
    copy_and_swap(this_t(pp, move(d), a), *this);
  }

  template <class Y>
  shared_ptr(const shared_ptr<Y> &x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>)
    : p(x.p), cb(x.cb) {
    if (cb != nullptr) {
      if (cb->plus_1())
        throw_or_terminate<length_error>
          ("re::shared_ptr<T>::shared_ptr(const shared_ptr<Y> &):\n"
           "  ref-count overflow\n");
    }
  }
  template <class Y>
  shared_ptr &operator =(const shared_ptr<Y> &x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>) {
    copy_and_swap(this_t(x), *this);
    return *this;
  }

  template <class Y>
  shared_ptr(shared_ptr<Y> &&x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>)
    : p(x.p), cb(x.cb) {
    x.p = nullptr;
    x.cb = nullptr;
  }
  template <class Y>
  shared_ptr &operator =(shared_ptr<Y> &&x) noexcept
    requires (!is_same_v<Y, T>
              && is_compatible_pointer_with_v<Y *, T *>) {
    copy_and_swap(this_t(move(x)), *this);
    return *this;
  }

  template <class Y>
  shared_ptr(const shared_ptr<Y> &x, element_type *pp) noexcept
    : p(pp), cb(x.cb) {
    if (cb != nullptr) {
      if (cb->plus_1())
        throw_or_terminate<length_error>
          ("re::shared_ptr<T>\n"
           "::shared_ptr(const shared_ptr<Y> &, element_type *):\n"
           "  ref-count overflow\n");
    }
  }
  template <class Y>
  shared_ptr(shared_ptr<Y> &&x, element_type *pp) noexcept
    : p(pp), cb(x.cb) {
    x.p = nullptr;
    x.cb = nullptr;
  }

  template <class Y, class D>
  shared_ptr(unique_ptr<Y, D> &&x)
    requires (is_pointer_v<typename unique_ptr<Y, D>::pointer>
              && !is_array_v<T>
              && is_convertible_v
              <typename unique_ptr<Y, D>::pointer, T *>) {
    if (const auto tmp_p = x.get();
        tmp_p == nullptr) {
      p = nullptr;
      cb = nullptr;
    }
    else {
      cb = default_alloc_wrapper<inner::shared_ptr_control_block_impl<Y, D>>{}
        .new_1(tmp_p, x.get_deleter());
      x.release();
      p = tmp_p;
      enable_shrd_from_this(tmp_p);
    }
  }
  template <class Y, class D>
  shared_ptr &operator =(unique_ptr<Y, D> &&x)
    requires (is_pointer_v<typename unique_ptr<Y, D>::pointer>
              && is_convertible_v
              <typename unique_ptr<Y, D>::pointer, T *>) {
    copy_and_swap(this_t(move(x)), *this);
    return *this;
  }

  template <class Y>
  explicit shared_ptr(const weak_ptr<Y> &x)
    requires is_compatible_pointer_with_v<Y *, T *> {
    if (x.expired())
      throw_or_terminate<bad_weak_ptr>();
    p = x.p;
    cb = x.cb;
    if (cb->plus_1())
      throw_or_terminate<length_error>
        ("re::shared_ptr<T>::shared_ptr(const weak_ptr<Y> &):\n"
         "  ref-count overflow\n");
  }

  element_type *get() const noexcept {
    return p;
  }
  element_type &operator *() const noexcept requires (!is_array_v<T>) {
    return *p;
  }
  element_type *operator->() const noexcept requires (!is_array_v<T>) {
    return p;
  }
  element_type &operator [](ptrdiff_t i) const noexcept
    requires is_array_v<T> {
    return p[i];
  }

  long use_count() const noexcept {
    return cb == nullptr ? 0 : cb->count();
  }
  explicit operator bool() const noexcept {
    return get() != nullptr;
  }

  template <class U>
  bool owner_before(const shared_ptr<U> &b) const noexcept {
    return cb < b.cb;
  }
  template <class U>
  bool owner_before(const weak_ptr<U> &b) const noexcept {
    return cb < b.cb;
  }
};
template <class T>
shared_ptr(weak_ptr<T>)->shared_ptr<T>;
template <class T, class D>
shared_ptr(unique_ptr<T, D>)->shared_ptr<T>;
template <class T, class U>
bool operator ==(const shared_ptr<T> &a, const shared_ptr<U> &b) noexcept {
  return a.get() == b.get();
}
template <class T>
bool operator ==(const shared_ptr<T> &a, nullptr_t) noexcept {
  return !a;
}
template <class T, class U>
strong_ordering operator <=>(const shared_ptr<T> &a,
                             const shared_ptr<U> &b) noexcept {
  return compare_three_way{}(a.get(), b.get());
}
template <class T>
strong_ordering operator <=>(const shared_ptr<T> &a,
                             nullptr_t) noexcept {
  return compare_three_way{}(a.get(),
                             static_cast
                             <typename shared_ptr<T>::element_type *>
                             (nullptr));
}

namespace inner {

struct shrdp_cb_mkshrd_deleter {
  template <class T>
  void operator ()(T *p) const noexcept {
    allocator_wrapper<default_allocator<T>>{}.delete_1(p);
  }
};
struct shrdp_cb_mkshrd_uba_deleter {
  using alloc_t = default_allocator<byte>;
  template <class T>
  void operator ()(T p) const noexcept {
    allocator_wrapper<alloc_t>{}.deallocate_headed_buffer(p);
  }
};
struct shrdp_cb_mkshrd_default_tag {};
template <class T,
          class D = conditional_t<is_unbounded_array_v<T>,
                                  shrdp_cb_mkshrd_uba_deleter,
                                  shrdp_cb_mkshrd_deleter>>
class shrdp_cb_mkshrd : public shared_ptr_control_block, D {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;

  using this_t = shrdp_cb_mkshrd;

  using base_t = shared_ptr_control_block;
  using base_t::count;
  using base_t::plus_1;
  using base_t::minus_1;

  alignas(alignof(T)) byte data[sizeof(T)];

  virtual void clear() noexcept override {
    destroy_at(ptr());
    static_cast<D &>(*this)(this);
  }
  virtual void *deleter_stored_ptr() noexcept override {
    return nullptr;
  }
  virtual const type_info &deleter_typeid() noexcept override {
    return typeid(void);
  }

public:
  shrdp_cb_mkshrd() = delete;
  ~shrdp_cb_mkshrd() = default;
  shrdp_cb_mkshrd(const this_t &) noexcept = delete;
  this_t &operator =(const this_t &) noexcept = delete;
  shrdp_cb_mkshrd(this_t &&) noexcept = delete;
  this_t &operator =(this_t &&) noexcept = delete;

  template <class...S>
  shrdp_cb_mkshrd(in_place_t, S &&...s) requires (!is_array_v<T>)
    : base_t(1) {
    ::new(void_ptr()) T(forward<S>(s)...);
  }
  shrdp_cb_mkshrd(in_place_t) requires is_bounded_array_v<T> : base_t(1) {
    construct_array(ptr());
  }
  template <class U>
  shrdp_cb_mkshrd(in_place_t, const U &u)
    requires is_bounded_array_v<T> : base_t(1) {
    construct_array(ptr(), u);
  }

  shrdp_cb_mkshrd(shrdp_cb_mkshrd_default_tag)
    requires (!is_array_v<T>) : base_t(1) {
    ::new(void_ptr()) T;
  }
  shrdp_cb_mkshrd(shrdp_cb_mkshrd_default_tag)
    requires is_bounded_array_v<T> : base_t(1) {
    default_construct_array(ptr());
  }

  T *ptr() noexcept {
    return reinterpret_cast<T *>(addressof(data));
  }
  remove_extent_t<T> *element_ptr() noexcept {
    return reinterpret_cast<remove_extent_t<T> *>(addressof(data));
  }
  void *void_ptr() noexcept {
    return const_cast<void *>(reinterpret_cast<const volatile void *>(ptr()));
  }
};
template <class T, class D>
class shrdp_cb_mkshrd<T [], D> : public shared_ptr_control_block, D {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;

  using this_t = shrdp_cb_mkshrd;

  using base_t = shared_ptr_control_block;
  using base_t::count;
  using base_t::plus_1;
  using base_t::minus_1;

  using buf_ptr = typename allocator_wrapper<typename D::alloc_t>
    ::template headed_buffer_ptr<this_t, remove_cv_t<T>>;
  buf_ptr buf_p = buf_ptr(nullptr);

  virtual void clear() noexcept override {
    if (buf_p != nullptr) {
      for (auto &p : iters(buf_p.data(), buf_p.data() + buf_p.size()))
        destroy_at(static_cast<T *>(p));
      static_cast<D &>(*this)(buf_p);
    }
  }
  virtual void *deleter_stored_ptr() noexcept override {
    return nullptr;
  }
  virtual const type_info &deleter_typeid() noexcept override {
    return typeid(void);
  }

public:
  shrdp_cb_mkshrd() : base_t(1) {}
  ~shrdp_cb_mkshrd() = default;
  shrdp_cb_mkshrd(const this_t &) noexcept = delete;
  this_t &operator =(const this_t &) noexcept = delete;
  shrdp_cb_mkshrd(this_t &&) noexcept = delete;
  this_t &operator =(this_t &&) noexcept = delete;

  void set_buffer_pointer(buf_ptr p) noexcept {
    buf_p = p;
  }
};

struct shrdp_cb_allcshrd_default_tag {};
template <class T, class A>
class shrdp_cb_allcshrd
  : public shared_ptr_control_block
  , allocator_wrapper<A> {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;

  using this_t = shrdp_cb_allcshrd;

  using base_t = shared_ptr_control_block;
  using base_t::count;
  using base_t::plus_1;
  using base_t::minus_1;

  using alw_t = allocator_wrapper<A>;

  alignas(alignof(T)) byte data[sizeof(T)];

  virtual void clear() noexcept override {
    destroy_at(no_cv_ptr(), alw_t::destroy_non_array_fn());
    alw_t tmp_alw(static_cast<const alw_t &>(*this));
    tmp_alw.template rebind<this_t>().delete_1(this);
  }
  virtual void *deleter_stored_ptr() noexcept override {
    return nullptr;
  }
  virtual const type_info &deleter_typeid() noexcept override {
    return typeid(void);
  }

public:
  shrdp_cb_allcshrd() = delete;
  ~shrdp_cb_allcshrd() = default;
  shrdp_cb_allcshrd(const this_t &) noexcept = delete;
  this_t &operator =(const this_t &) noexcept = delete;
  shrdp_cb_allcshrd(this_t &&) noexcept = delete;
  this_t &operator =(this_t &&) noexcept = delete;

  template <class...S>
  shrdp_cb_allcshrd(in_place_t, const A &a, S &&...s) requires (!is_array_v<T>)
    : base_t(1), alw_t(a) {
    alw_t::construct_non_array_fn()(no_cv_ptr(), forward<S>(s)...);
  }
  shrdp_cb_allcshrd(in_place_t, const A &a) requires is_bounded_array_v<T>
    : base_t(1), alw_t(a) {
    construct_array(no_cv_ptr(),
                    alw_t::construct_non_array_fn(),
                    alw_t::destroy_non_array_fn());
  }
  template <class U>
  shrdp_cb_allcshrd(in_place_t, const A &a, const U &u)
    requires is_bounded_array_v<T>
    : base_t(1), alw_t(a) {
    construct_array(no_cv_ptr(), u,
                    alw_t::construct_non_array_fn(),
                    alw_t::destroy_non_array_fn());
  }

  shrdp_cb_allcshrd(shrdp_cb_allcshrd_default_tag, const A &a)
    requires (!is_array_v<T>)
    : base_t(1), alw_t(a) {
    alw_t::default_construct_non_array_fn()(no_cv_ptr());
  }
  shrdp_cb_allcshrd(shrdp_cb_allcshrd_default_tag, const A &a)
    requires is_bounded_array_v<T>
    : base_t(1), alw_t(a) {
    default_construct_array(no_cv_ptr(),
                            alw_t::default_construct_non_array_fn(),
                            alw_t::destroy_non_array_fn());
  }

  remove_cv_t<T> *no_cv_ptr() noexcept {
    return reinterpret_cast<remove_cv_t<T> *>(addressof(data));
  }
  remove_extent_t<T> *element_ptr() noexcept {
    return reinterpret_cast<remove_extent_t<T> *>(addressof(data));
  }
};
template <class T, class A>
class shrdp_cb_allcshrd<T [], A>
  : public shared_ptr_control_block
  , allocator_wrapper<A> {
  template <class>
  friend class shared_ptr;
  template <class>
  friend class weak_ptr;

  using this_t = shrdp_cb_allcshrd;

  using base_t = shared_ptr_control_block;
  using base_t::count;
  using base_t::plus_1;
  using base_t::minus_1;

  using alw_t = allocator_wrapper<A>;

  using buf_ptr = alw_t::template headed_buffer_ptr<this_t, remove_cv_t<T>>;
  buf_ptr buf_p = buf_ptr(nullptr);

  virtual void clear() noexcept override {
    if (buf_p != nullptr) {
      for (auto &p : iters(buf_p.data(), buf_p.data() + buf_p.size()))
        destroy_at(to_address(p), alw_t::destroy_non_array_fn());
      alw_t tmp_alw(static_cast<const alw_t &>(*this));
      tmp_alw.deallocate_headed_buffer(buf_p);
    }
  }
  virtual void *deleter_stored_ptr() noexcept override {
    return nullptr;
  }
  virtual const type_info &deleter_typeid() noexcept override {
    return typeid(void);
  }

public:
  shrdp_cb_allcshrd() = delete;
  ~shrdp_cb_allcshrd() = default;
  shrdp_cb_allcshrd(const this_t &) noexcept = delete;
  this_t &operator =(const this_t &) noexcept = delete;
  shrdp_cb_allcshrd(this_t &&) noexcept = delete;
  this_t &operator =(this_t &&) noexcept = delete;

  explicit shrdp_cb_allcshrd(const A &a) : base_t(1), alw_t(a) {}

  void set_buffer_pointer(buf_ptr p) noexcept {
    buf_p = p;
  }
};

}
template <class T>
struct fo_make_shared {
  template <class...S>
  shared_ptr<T> operator ()(S &&...s) const {
    const auto p
      = default_alloc_wrapper<inner::shrdp_cb_mkshrd<T>>()
      .new_1(in_place, forward<S>(s)...);
    shared_ptr<T> ret;
    ret.p = p->element_ptr();
    ret.cb = p;
    return ret;
  }
};
template <class T, size_t N>
struct fo_make_shared<T [N]> {
  template <class...S>
  shared_ptr<T [N]> operator ()() const requires (N != 0u) {
    const auto p
      = default_alloc_wrapper<inner::shrdp_cb_mkshrd<T [N]>>()
      .new_1(in_place);
    shared_ptr<T [N]> ret;
    ret.p = p->element_ptr();
    ret.cb = p;
    return ret;
  }
  template <class...S>
  shared_ptr<T [N]> operator ()(const T &x) const requires (N != 0u) {
    const auto p
      = default_alloc_wrapper<inner::shrdp_cb_mkshrd<T [N]>>()
      .new_1(in_place, x);
    shared_ptr<T [N]> ret;
    ret.p = p->element_ptr();
    ret.cb = p;
    return ret;
  }
};
template <class T>
struct fo_make_shared<T []> {
private:
  template <class...S>
  static shared_ptr<T []> impl(size_t n, S &&...s) {
    default_alloc_wrapper<byte> alw;
    const auto bufp
      = alw.allocate_headed_buffer<inner::shrdp_cb_mkshrd<T []>,
                                   remove_cv_t<T>>(n);
    auto guard
      = exit_fn([&alw, bufp]() {alw.deallocate_headed_buffer(bufp);},
                true);
    const auto d = static_cast<T *>(to_address(bufp.data()));
    securely_initialize(rng(d, n),
                        [&](auto p) {
                          construct_maybe_array(p, forward<S>(s)...);
                        },
                        [](auto p) {destroy_at(p);});
    bufp.head().set_buffer_pointer(bufp);
    guard.unset();
    shared_ptr<T []> ret;
    ret.p = static_cast<T *>(to_address(bufp.data()));
    ret.cb = addressof(bufp.head());
    return ret;
  }

public:
  shared_ptr<T []> operator ()(size_t n) const {
    return impl(n);
  }
  shared_ptr<T []> operator ()(size_t n, const T &x) const {
    return impl(n, x);
  }
};
template <class T>
inline constexpr fo_make_shared<T> make_shared{};

template <class T>
struct fo_make_shared_for_overwrite {
  shared_ptr<T> operator ()() const {
    const auto p
      = default_alloc_wrapper<inner::shrdp_cb_mkshrd<T>>()
      .new_1(inner::shrdp_cb_mkshrd_default_tag{});
    shared_ptr<T> ret;
    ret.p = p->element_ptr();
    ret.cb = p;
    return ret;
  }
};
template <class T>
struct fo_make_shared_for_overwrite<T []> {
  shared_ptr<T []> operator ()(size_t n) const {
    default_alloc_wrapper<byte> alw;
    const auto bufp
      = alw.allocate_headed_buffer<inner::shrdp_cb_mkshrd<T []>,
                                   remove_cv_t<T>>(n);
    auto guard
      = exit_fn([&alw, bufp]() {alw.deallocate_headed_buffer(bufp);},
                true);
    const auto d = static_cast<T *>(bufp.data());
    securely_initialize(rng(d, n),
                        [&](auto p) {default_construct_maybe_array(p);},
                        [](auto p) {destroy_at(p);});
    bufp.head().set_buffer_pointer(bufp);
    guard.unset();
    shared_ptr<T []> ret;
    ret.p = static_cast<T *>(bufp.data());
    ret.cb = addressof(bufp.head());
    return ret;
  }
};
template <class T>
inline constexpr fo_make_shared_for_overwrite<T> make_shared_for_overwrite{};

template <class T>
struct fo_allocate_shared {
  template <class A, class...S>
  shared_ptr<T> operator ()(const A &a, S &&...s) const {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto p = alw.template rebind
      <inner::shrdp_cb_allcshrd<T, alloc_rebind<A, byte>>>()
      .new_1(in_place, alw.get(), forward<S>(s)...);
    shared_ptr<T> ret;
    ret.p = p->element_ptr();
    ret.cb = to_address(p);
    return ret;
  }
};
template <class T, size_t N>
struct fo_allocate_shared<T [N]> {
  template <class A>
  shared_ptr<T [N]> operator ()(const A &a) const {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto p = alw.template rebind
      <inner::shrdp_cb_allcshrd<T [N], alloc_rebind<A, byte>>>()
      .new_1(in_place, alw.get());
    shared_ptr<T [N]> ret;
    ret.p = p->element_ptr();
    ret.cb = to_address(p);
    return ret;
  }
  template <class A>
  shared_ptr<T [N]> operator ()(const A &a, const T &x) const {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto p = alw.template rebind
      <inner::shrdp_cb_allcshrd<T [N], alloc_rebind<A, byte>>>()
      .new_1(in_place, alw.get(), x);
    shared_ptr<T [N]> ret;
    ret.p = p->element_ptr();
    ret.cb = to_address(p);
    return ret;
  }
};
template <class T>
struct fo_allocate_shared<T []> {
private:
  template <class A, class...S>
  static shared_ptr<T []> impl(const A &a, size_t n, S &&...s) {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto bufp
      = alw.template allocate_headed_buffer<inner::shrdp_cb_allcshrd
                                            <T [], alloc_rebind<A, byte>>,
                                            remove_cv_t<T>>(n, alw.get());
    auto guard
      = exit_fn([&alw, bufp]() {alw.deallocate_headed_buffer(bufp);},
                true);
    securely_initialize(rng(to_address(bufp.data()), n),
                        [&, alw](auto p) {
                          construct_maybe_array(p, forward<S>(s)...,
                                                alw.construct_non_array_fn(),
                                                alw.destroy_non_array_fn());
                        },
                        [alw](auto p) {
                          destroy_at(p, alw.destroy_non_array_fn());
                        });
    bufp.head().set_buffer_pointer(bufp);
    guard.unset();
    shared_ptr<T []> ret;
    ret.p = static_cast<T *>(to_address(bufp.data()));
    ret.cb = addressof(bufp.head());
    return ret;
  }

public:
  template <class A>
  shared_ptr<T []> operator ()(const A &a, size_t n) const {
    return impl(a, n);
  }
  template <class A>
  shared_ptr<T []> operator ()(const A &a, size_t n, const T &x) const {
    return impl(a, n, x);
  }
};
template <class T>
inline constexpr fo_allocate_shared<T> allocate_shared{};

template <class T>
struct fo_allocate_shared_for_overwrite {
  template <class A>
  shared_ptr<T> operator ()(const A &a) const {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto p = alw.template rebind
      <inner::shrdp_cb_allcshrd<T, alloc_rebind<A, byte>>>()
      .new_1(inner::shrdp_cb_allcshrd_default_tag{}, alw.get());
    shared_ptr<T> ret;
    ret.p = p->element_ptr();
    ret.cb = to_address(p);
    return ret;
  }
};
template <class T>
struct fo_allocate_shared_for_overwrite<T []> {
  template <class A>
  shared_ptr<T []> operator ()(const A &a, size_t n) const {
    auto alw = alloc_wrapper(a).template rebind<byte>();
    const auto bufp
      = alw.template allocate_headed_buffer<inner::shrdp_cb_allcshrd
                                            <T [], alloc_rebind<A, byte>>,
                                            remove_cv_t<T>>(n, alw.get());
    auto guard
      = exit_fn([&alw, bufp]() {alw.deallocate_headed_buffer(bufp);},
                true);
    securely_initialize(rng(to_address(bufp.data()), n),
                        [&, alw](auto p) {
                          construct_maybe_array
                            (p,
                             alw.default_construct_non_array_fn(),
                             alw.destroy_non_array_fn());
                        },
                        [alw](auto p) {
                          destroy_at(p, alw.destroy_non_array_fn());
                        });
    bufp.head().set_buffer_pointer(bufp);
    guard.unset();
    shared_ptr<T []> ret;
    ret.p = static_cast<T *>(to_address(bufp.data()));
    ret.cb = addressof(bufp.head());
    return ret;
  }
};
template <class T>
inline constexpr
fo_allocate_shared_for_overwrite<T> allocate_shared_for_overwrite{};

template <class T>
struct fo_static_pointer_cast {
  template <class U>
  shared_ptr<T> operator ()(const shared_ptr<U> &x) const noexcept
    requires requires {static_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (x, static_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
  template <class U>
  shared_ptr<T> operator ()(shared_ptr<U> &&x) const noexcept
    requires requires {static_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (move(x), static_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
};
template <class T>
inline constexpr fo_static_pointer_cast<T> static_pointer_cast{};
template <class T>
struct fo_reinterpret_pointer_cast {
  template <class U>
  shared_ptr<T> operator ()(const shared_ptr<U> &x) const noexcept
    requires requires {reinterpret_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (x, reinterpret_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
  template <class U>
  shared_ptr<T> operator ()(shared_ptr<U> &&x) const noexcept
    requires requires {reinterpret_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (move(x),
       reinterpret_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
};
template <class T>
inline constexpr fo_reinterpret_pointer_cast<T> reinterpret_pointer_cast{};
template <class T>
struct fo_const_pointer_cast {
  template <class U>
  shared_ptr<T> operator ()(const shared_ptr<U> &x) const noexcept
    requires requires {const_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (x, const_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
  template <class U>
  shared_ptr<T> operator ()(shared_ptr<U> &&x) const noexcept
    requires requires {const_cast<T *>((U *)nullptr);} {
    return shared_ptr<T>
      (move(x),
       const_cast<typename shared_ptr<T>::element_type *>(x.get()));
  }
};
template <class T>
inline constexpr fo_const_pointer_cast<T> const_pointer_cast{};
template <class T>
struct fo_dynamic_pointer_cast {
  template <class U>
  shared_ptr<T> operator ()(const shared_ptr<U> &x) const noexcept
    requires requires {
      dynamic_cast<T *>((U *)nullptr);
      dynamic_cast<typename shared_ptr<T>::element_type *>(x.get());
    } {
    const auto p
      = dynamic_cast<typename shared_ptr<T>::element_type *>(x.get());
    return p != nullptr ? shared_ptr<T>(x, p) : shared_ptr<T>{};
  }
  template <class U>
  shared_ptr<T> operator ()(shared_ptr<U> &&x) const noexcept
    requires requires {
      dynamic_cast<T *>((U *)nullptr);
      dynamic_cast<typename shared_ptr<T>::element_type *>(x.get());
    } {
    const auto p
      = dynamic_cast<typename shared_ptr<T>::element_type *>(x.get());
    return p != nullptr ? shared_ptr<T>(move(x), p) : shared_ptr<T>{};
  }
};
template <class T>
inline constexpr fo_dynamic_pointer_cast<T> dynamic_pointer_cast{};

template <class D>
struct fo_get_deleter {
  template <class T>
  D *operator ()(const shared_ptr<T> &p) const noexcept
    requires is_same_v<remove_cv_t<D>, D> {
    return p.cb->template get_deleter<D>();
  }
};
template <class D>
inline constexpr fo_get_deleter<D> get_deleter{};

template <class T>
struct hash<shared_ptr<T>> {
  size_t operator ()(const shared_ptr<T> &p) const {
    return hash<typename shared_ptr<T>::element_type *>{}(p.get());
  }
};

}

// thread
namespace re {

class pool_thread; // forward declaration

namespace inner {

struct thread_stored_fn_base {
  virtual unsigned long call() = 0;
  virtual void destroy() noexcept = 0;
};
template <class F, class...S>
struct thread_stored_fn_impl : inner::thread_stored_fn_base {
  using this_t = thread_stored_fn_impl;

  F f;
  tuple<S...> t;

  thread_stored_fn_impl() = delete;
  ~thread_stored_fn_impl() = default;
  thread_stored_fn_impl(const thread_stored_fn_impl &) = delete;
  thread_stored_fn_impl &operator =(const thread_stored_fn_impl &) = delete;
  thread_stored_fn_impl(thread_stored_fn_impl &&) = delete;
  thread_stored_fn_impl &operator =(thread_stored_fn_impl &&) = delete;

  template <class FF, class...SS>
  thread_stored_fn_impl(in_place_t, FF &&ff, SS &&...ss)
    : f(forward<FF>(ff)), t(forward<SS>(ss)...) {}

  virtual unsigned long call() override {
    apply(move(f), move(t));
    return 0u;
  }
  virtual void destroy() noexcept override {
    default_alloc_wrapper<this_t>{}.delete_1(this);
  }
};

}
namespace inner::fns {

inline DWORD WINAPI win32_thread_entry_fn(void *p) {
  const auto guard = exit_fn([p]() {
    reinterpret_cast<inner::thread_stored_fn_base *>(p)->destroy();
  });
  return reinterpret_cast<inner::thread_stored_fn_base *>(p)->call();
}

}
namespace this_thread {

struct fo_get_id;

};
class thread {
public:
  class id {
    unsigned long v = {};

    friend class thread;
    friend class pool_thread;
    template <class>
    friend struct hash;

    friend struct this_thread::fo_get_id;
    friend struct fo_get_thread_id;

  public:
    id() noexcept = default;
    ~id() = default;
    id(const id &) = default;
    id &operator =(const id &) = default;
    id(id &&) = default;
    id &operator =(id &&) = default;

    friend inline bool operator ==(id x, id y) noexcept {
      return x.v == y.v;
    }
    friend inline strong_ordering operator <=>(id x, id y) noexcept {
      return x.v <=> y.v;
    }
  };
  using native_handle_type = HANDLE;

private:
  HANDLE h;
  id id_num;

public:
  thread() noexcept : h(INVALID_HANDLE_VALUE), id_num() {}
  ~thread() {
    if (joinable())
      join();
  }
  thread(const thread &) = delete;
  thread &operator =(const thread &) = delete;
  thread(thread &&x) noexcept : h(x.h), id_num(x.id_num) {
    x.h = INVALID_HANDLE_VALUE;
    x.id_num = id{};
  }
  thread &operator =(thread &&x) noexcept {
    if (joinable())
      print_then_terminate
        ("re::thread::=(re::thread &&): only null thread can be assigned to\n");
    h = exchange(x.h, INVALID_HANDLE_VALUE);
    id_num = exchange(x.id_num, id{});
    return *this;
  }
  friend void swap(thread &x, thread &y) noexcept {
    adl_swap(x.h, y.h);
    adl_swap(x.id_num, y.id_num);
  }

  template <class F, class...S>
  explicit thread(F &&f, S &&...s)
    requires (is_constructible_v<decay_t<F>, F>
              && (is_constructible_v<unwrap_ref_decay_t<S>, S> && ...)
              && is_invocable_v<decay_t<F>, unwrap_ref_decay_t<S>...>) {
    using t = inner::thread_stored_fn_impl<decay_t<F>,
                                           unwrap_ref_decay_t<S>...>;
    const auto fp = default_alloc_wrapper<t>{}
      .new_1(in_place, forward<F>(f), forward<S>(s)...);
    h = CreateThread(NULL, 0,
                     addressof(inner::fns::win32_thread_entry_fn),
                     reinterpret_cast<void *>(fp),
                     0, addressof(id_num.v));
    if (h == NULL)
      throw_or_terminate<runtime_error>
        ("thread(f, s...): CreateThread() failed\n");
    if (id_num == id{})
      throw_or_terminate<runtime_error>
        ("thread(f, s...): id is zero after CreateThread()\n");
  }

  bool joinable() const noexcept {
    return get_id() != id{};
  }
  void join() {
    if (h == INVALID_HANDLE_VALUE)
      throw_or_terminate<runtime_error>
        ("re::thread::join(): null thread\n");
    const auto x = WaitForSingleObject(h, INFINITE);
    if (x != WAIT_OBJECT_0)
      throw_or_terminate<runtime_error>
        ("re::thread::join(): WaitForSingleObject() failed\n");
    if (CloseHandle(h) == 0)
      throw_or_terminate<runtime_error>
        ("re::thread::join(): CloseHandle() failed\n");
    h = INVALID_HANDLE_VALUE;
    id_num = id{};
  }
  void detach() {
    if (!joinable())
      throw_or_terminate<logic_error>
        ("re::thread::detach(): can not detach non-joinable thread\n");
    h = INVALID_HANDLE_VALUE;
    id_num = id{};
  }
  id get_id() const noexcept {
    return id_num;
  }
  native_handle_type native_handle() {
    return h;
  }

  static unsigned int hardware_concurrency() noexcept {
    SYSTEM_INFO info;
    GetSystemInfo(addressof(info));
    return static_cast<unsigned int>(info.dwNumberOfProcessors);
  }
};

template <>
struct hash<thread::id> {
  using argument_type = thread::id;
  using result_type = size_t;
  size_t operator ()(thread::id x) const noexcept {
    return hash<unsigned long>{}(x.v);
  }
};

namespace this_thread {

struct fo_get_id {
  thread::id operator ()() const noexcept {
    thread::id ret;
    ret.v = GetCurrentThreadId();
    return ret;
  }
};
inline constexpr fo_get_id get_id{};

struct fo_sleep_for {
  template <class R, ratio P>
  void operator ()(const duration<R, P> &d) const {
    inner::fns::win32_sleep_ms(duration_ceil<milliseconds>(d).count());
  }
};
inline fo_sleep_for sleep_for{};

struct fo_sleep_until {
  template <class C, class D>
  void operator ()(const time_point<C, D> &t) const {
    const auto t0 = C::now();
    const auto d = t - t0;
    if (d.count() > 0)
      inner::fns::win32_sleep_ms(duration_ceil<milliseconds>(d).count());
  }
};
inline fo_sleep_until sleep_until{};

}
using this_thread::sleep_for;
using this_thread::sleep_until;
struct fo_get_thread_id {
  thread::id operator ()() const noexcept {
    thread::id ret;
    ret.v = GetCurrentThreadId();
    return ret;
  }
};
inline constexpr fo_get_thread_id get_thread_id{};

}

// mutex
// recursive_mutex
// timed_mutex
// lock_guard
// unique_lock
namespace re {

class mutex {
  CRITICAL_SECTION v;

public:
  mutex() noexcept {
    InitializeCriticalSection(addressof(v));
  }
  ~mutex() {
    DeleteCriticalSection(addressof(v));
  }
  mutex(const mutex &) = delete;
  mutex &operator =(const mutex &) = delete;
  mutex(mutex &&) = delete;
  mutex &operator =(mutex &&) = delete;

  void lock() {
    EnterCriticalSection(addressof(v));
  }
  bool try_lock() {
    return TryEnterCriticalSection(addressof(v));
  }
  void unlock() {
    LeaveCriticalSection(addressof(v));
  }

  using native_handle_type = CRITICAL_SECTION *;
  native_handle_type native_handle() {
    return addressof(v);
  }
};

class recursive_mutex {
  CRITICAL_SECTION v;

public:
  recursive_mutex() noexcept {
    InitializeCriticalSection(addressof(v));
  }
  ~recursive_mutex() {
    DeleteCriticalSection(addressof(v));
  }
  recursive_mutex(const recursive_mutex &) = delete;
  recursive_mutex &operator =(const recursive_mutex &) = delete;
  recursive_mutex(recursive_mutex &&) = delete;
  recursive_mutex &operator =(recursive_mutex &&) = delete;

  void lock() {
    EnterCriticalSection(addressof(v));
  }
  bool try_lock() {
    return TryEnterCriticalSection(addressof(v));
  }
  void unlock() {
    LeaveCriticalSection(addressof(v));
  }

  using native_handle_type = CRITICAL_SECTION *;
  native_handle_type native_handle() {
    return addressof(v);
  }
};

class timed_mutex : mutex {
public:
  timed_mutex() noexcept = default;
  ~timed_mutex() = default;
  timed_mutex(const timed_mutex &) = delete;
  timed_mutex &operator =(const timed_mutex &) = delete;
  timed_mutex(timed_mutex &&) = delete;
  timed_mutex &operator =(timed_mutex &&) = delete;

  using mutex::lock;
  using mutex::try_lock;
  using mutex::unlock;
  using native_handle_type = mutex::native_handle_type;
  using mutex::native_handle;

  template <class R, ratio P>
  bool try_lock_for(const duration<R, P> &d) {
    const steady_clock::time_point t2 = steady_clock::now()
      + duration_ceil<steady_clock::duration>(d);
    for (;;) {
      if (steady_clock::now().count() > t2.count())
        return false;
      if (try_lock())
        return true;
    }
  }
  template <class C, class D>
  bool try_lock_until(const time_point<C, D> &t) {
    const typename C::time_point t2 = time_point_ceil<typename C::duration>(t);
    for (;;) {
      if (C::now().count() > t2.count())
        return false;
      if (try_lock())
        return true;
    }
  }
};

struct defer_lock_t {
  explicit defer_lock_t() = default;
};
inline constexpr defer_lock_t defer_lock{};
struct try_to_lock_t {
  explicit try_to_lock_t() = default;
};
inline constexpr try_to_lock_t try_to_lock{};
struct adopt_lock_t {
  explicit adopt_lock_t() = default;
};
inline constexpr adopt_lock_t adopt_lock{};

template <class M>
class lock_guard {
  M &m;

public:
  using mutex_type = M;

  lock_guard() = delete;
  ~lock_guard() {
    m.unlock();
  }
  lock_guard(const lock_guard &) = delete;
  lock_guard &operator =(const lock_guard &) = delete;
  lock_guard(lock_guard &&) = delete;
  lock_guard &operator =(lock_guard &&) = delete;

  explicit lock_guard(mutex_type &mm) : m(mm) {
    m.lock();
  }
  lock_guard(mutex_type &mm, adopt_lock_t) : m(mm) {}
};

template <class M>
class unique_lock {
  M *p;
  bool owns;

public:
  using mutex_type = M;

  unique_lock() noexcept : p(nullptr), owns(false) {}
  ~unique_lock() {
    if (owns)
      p->unlock();
  }
  unique_lock(const unique_lock &) = delete;
  unique_lock &operator =(const unique_lock &) = delete;
  unique_lock(unique_lock &&u) noexcept : p(u.p), owns(u.owns) {
    u.p = nullptr;
    u.owns = false;
  }
  unique_lock &operator =(unique_lock &&u) {
    if (this != addressof(u)) {
      if (owns)
        p->unlock();
      p = u.p;
      owns = u.owns;
      u.p = nullptr;
      u.owns = false;
    }
    return *this;
  }
  friend void swap(unique_lock &x, unique_lock &y) noexcept {
    adl_swap(x.p, y.p);
    adl_swap(x.owns, y.owns);
  }

  explicit unique_lock(mutex_type &m) : p(addressof(m)), owns(true) {
    m.lock();
  }
  unique_lock(mutex_type &m, defer_lock_t) noexcept
    : p(addressof(m)), owns(false) {}
  unique_lock(mutex_type &m, try_to_lock_t)
    : p(addressof(m)), owns(m.try_lock()) {}
  unique_lock(mutex_type &m, adopt_lock_t) : p(addressof(m)), owns(true) {}

  template <class C, class D>
  unique_lock(mutex_type &m, const time_point<C, D> &t)
    : p(addressof(m)), owns(m.try_lock_until(t)) {}
  template<class R, ratio P>
  unique_lock(mutex_type &m, const duration<R, P> &t)
    : p(addressof(m)), owns(m.try_lock_for(t)) {}

  void lock() {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::lock(): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::lock(): no mutex\n");
    p->lock();
    owns = true;
  }
  bool try_lock() {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock(): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock(): no mutex\n");
    owns = p->try_lock();
    return owns;
  }
  template <class R, ratio P>
  bool try_lock_for(const duration<R, P> &t) {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock_for(t): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock_for(t): no mutex\n");
    owns = p->try_lock_for(t);
    return owns;
  }
  template <class C, class D>
  bool try_lock_until(const time_point<C, D> &t) {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock_until(t): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::try_lock_until(t): no mutex\n");
    owns = p->try_lock_until(t);
    return owns;
  }
  void unlock() {
    if (!owns)
      throw_or_terminate<runtime_error>
        ("re::unique_lock<M>::unlock(t): no locked mutex\n");
    p->unlock();
    owns = false;
  }

  mutex_type *release() noexcept {
    owns = false;
    return exchange(p, nullptr);
  }
  bool owns_lock() const noexcept {
    return owns;
  }
  explicit operator bool() const noexcept {
    return owns;
  }
  mutex_type *mutex() const noexcept {
    return p;
  }
};

}

// condition_variable
// mutex_area
namespace re {

enum class cv_status {
  no_timeout,
  timeout
};
class condition_variable {
  CONDITION_VARIABLE v;

public:
  condition_variable() {
    InitializeConditionVariable(addressof(v));
  }
  ~condition_variable() = default;
  condition_variable(const condition_variable &) = delete;
  condition_variable &operator =(const condition_variable &) = delete;
  condition_variable(condition_variable &&) = delete;
  condition_variable &operator =(condition_variable &&) = delete;

  void notify_one() noexcept {
    WakeConditionVariable(addressof(v));
  }
  void notify_all() noexcept {
    WakeAllConditionVariable(addressof(v));
  }

  void wait(unique_lock<mutex> &lock) {
    if (SleepConditionVariableCS(addressof(v),
                                 lock.mutex()->native_handle(),
                                 INFINITE)
        == 0)
      print_then_terminate("re::condition_variable::wait(lock): failed");
  }
  template <class F>
  void wait(unique_lock<mutex> &lock, F pred) {
    while (!pred())
      wait(lock);
  }

  template <class C, class D>
  cv_status wait_until(unique_lock<mutex> &lock,
                       const time_point<C, D> &t) {
    const auto d = t - C::now();
    const auto d_ms = duration_ceil<milliseconds>(d).count();
    const auto x = SleepConditionVariableCS
      (addressof(v),
       lock.mutex()->native_handle(),
       static_cast<DWORD>(duration_ceil<milliseconds>(d).count()));
    if (x != 0)
      return cv_status::no_timeout;
    else {
      if (GetLastError() == ERROR_TIMEOUT)
        return cv_status::timeout;
      print_then_terminate
        ("re::condition_variable::wait_until(lock, t): failed()");
      return cv_status::timeout;
    }
  }
  template <class C, class D, class F>
  bool wait_until(unique_lock<mutex> &lock,
                  const time_point<C, D> &t,
                  F pred) {
    while (!pred())
      if (wait_until(lock, t) == cv_status::timeout)
        return pred();
    return true;
  }

  template <class R, ratio P>
  cv_status wait_for(unique_lock<mutex> &lock,
                     const duration<R, P> &t) {
    return wait_until(lock, steady_clock::now() + t);
  }
  template <class R, ratio P, class F>
  bool wait_for(unique_lock<mutex> &lock,
                const duration<R, P> &t,
                F pred) {
    while (!pred())
      if (wait_for(lock, t) == cv_status::timeout)
        return pred();
    return true;
  }

  using native_handle_type = CONDITION_VARIABLE *;
  native_handle_type native_handle() {
    return addressof(v);
  }
};
inline constexpr cv_status cv_no_timeout = cv_status::no_timeout;
inline constexpr cv_status cv_timeout = cv_status::timeout;

template <class T>
class mutex_area {
  using this_t = mutex_area;

  T v;
  mutex m;

public:
  mutex_area() noexcept(is_nothrow_default_constructible_v<T>) : v(), m() {}
  ~mutex_area() = default;
  mutex_area(const mutex_area &) = delete;
  mutex_area &operator =(const mutex_area &) = delete;
  mutex_area(mutex_area &&) = delete;
  mutex_area &operator =(mutex_area &&) = delete;

  template <class...S>
  explicit mutex_area(in_place_t, S &&...s) : v(forward<S>(s)...) {}

  using value_type = T;
  T &data() noexcept {
    return v;
  }
  T &operator *() noexcept {
    return v;
  }
  T *operator ->() noexcept {
    return addressof(v);
  }
  const T &data() const noexcept {
    return v;
  }
  const T &operator *() const noexcept {
    return v;
  }
  const T *operator ->() const noexcept {
    return addressof(v);
  }

  template <class F>
  bool try_enter(F f) {
    if (m.try_lock()) {
      const auto guard = exit_fn([&]() {m.unlock();});
      f();
      return true;
    }
    else
      return false;
  }
  template <class F>
  void enter(F f) {
    const lock_guard l(m);
    f();
  }
  template <class F, class F2>
  void enter_until(F f, F2 eq) {
    for (;;) {
      const lock_guard l(m);
      if (eq()) {
        f();
        return;
      }
      else
        continue;
    }
  }
  template <class F>
  void enter_notify_one(F f, condition_variable &v) {
    {
      const lock_guard l(m);
      f();
    }
    v.notify_one();
  }
  template <class F>
  void enter_notify_all(F f, condition_variable &v) {
    {
      const lock_guard l(m);
      f();
    }
    v.notify_all();
  }
  template <class F>
  void leave_until(F eq, condition_variable &cv) {
    while (!eq()) {
      if (SleepConditionVariableCS(cv.native_handle(),
                                   m.native_handle(),
                                   INFINITE)
          == 0)
        print_then_terminate
          ("re::mutex_area::leave_until(eq, condvar_ref): "
           "SleepConditionVariableCS() failed");
    }
  }
  void leave_notify_one(condition_variable &cv) {
    m.unlock();
    const auto guard = exit_fn([&]() {m.lock();});
    cv.notify_one();
  }
  void leave_notify_all(condition_variable &cv) {
    m.unlock();
    const auto guard = exit_fn([&]() {m.lock();});
    cv.notify_all();
  }
  template <class F>
  void busy_wait(F eq) {
    while (!eq()) {
      m.unlock();
      m.lock();
    }
  }
};

}

// shared_mutex
// shared_timed_mutex
// shared_lock
namespace re {

class shared_mutex {
  struct state_t {
    CRITICAL_SECTION cs;
    bool locked = false;
    unsigned long long reading_count = 0u;
  };
  mutex_area<state_t> s;
  condition_variable l;

public:
  shared_mutex() noexcept {
    InitializeCriticalSection(addressof(s->cs));
  }
  ~shared_mutex() {
    DeleteCriticalSection(addressof(s->cs));
  }
  shared_mutex(const shared_mutex &) = delete;
  shared_mutex &operator =(const shared_mutex &) = delete;
  shared_mutex(shared_mutex &&) = delete;
  shared_mutex &operator =(shared_mutex &&) = delete;

  void lock() {
    s.enter([&]() {
      const auto eq = [&]() {
        return s->reading_count == 0u && s->locked == false;
      };
      if (!eq())
        s.leave_until(eq, l);
      EnterCriticalSection(addressof(s->cs));
      s->locked = true;
    });
  }
  bool try_lock() {
    bool y = false;
    s.enter([&]() {
      if (s->reading_count == 0u && s->locked == false) {
        EnterCriticalSection(addressof(s->cs));
        s->locked = true;
        y = true;
      }
    });
    return y;
  }
  void unlock() {
    s.enter([&]() {
      LeaveCriticalSection(addressof(s->cs));
      s->locked = false;
    });
    l.notify_all();
  }

  void lock_shared() {
    s.enter([&]() {
      const auto eq = [&]() {
        return s->locked == false || s->reading_count != 0u;
      };
      if (!eq())
        s.leave_until(eq, l);
      if (s->locked == false) {
        EnterCriticalSection(addressof(s->cs));
        s->locked = true;
        s->reading_count = 1u;
      }
      else {
        if (s->reading_count == numeric_limits<unsigned long long>::max())
          throw_or_terminate<runtime_error>
            ("re::shared_mutex::lock_shared(): too many shared readers\n");
        s->reading_count += 1u;
      }
    });
  }
  bool try_lock_shared() {
    bool y = false;
    s.enter([&]() {
      const auto eq = [&]() {
        return s->locked == false || s->reading_count != 0u;
      };
      if (!eq())
        return;
      if (s->locked == false) {
        EnterCriticalSection(addressof(s->cs));
        s->locked = true;
        s->reading_count = 1u;
      }
      else {
        if (s->reading_count == numeric_limits<unsigned long long>::max())
          throw_or_terminate<runtime_error>
            ("re::shared_mutex::try_lock_shared(): too many shared readers\n");
        s->reading_count += 1u;
      }
      y = true;
    });
    return y;
  }
  void unlock_shared() {
    bool y = false;
    s.enter([&]() {
      if (s->reading_count == 0u) {
        LeaveCriticalSection(addressof(s->cs));
        s->locked = false;
        y = true;
      }
      else {
        s->reading_count -= 1u;
        if (s->reading_count == 0u) {
          LeaveCriticalSection(addressof(s->cs));
          s->locked = false;
          y = true;
        }
      }
    });
    if (y)
      l.notify_all();
  }

  using native_handle_type = CRITICAL_SECTION *;
  native_handle_type native_handle() {
    return addressof(s->cs);
  }
};

class shared_timed_mutex : shared_mutex {
public:
  shared_timed_mutex() noexcept = default;
  ~shared_timed_mutex() = default;
  shared_timed_mutex(const shared_timed_mutex &) = delete;
  shared_timed_mutex &operator =(const shared_timed_mutex &) = delete;
  shared_timed_mutex(shared_timed_mutex &&) = delete;
  shared_timed_mutex &operator =(shared_timed_mutex &&) = delete;

  using shared_mutex::lock;
  using shared_mutex::try_lock;
  using shared_mutex::unlock;
  using shared_mutex::lock_shared;
  using shared_mutex::try_lock_shared;
  using shared_mutex::unlock_shared;
  using native_handle_type = shared_mutex::native_handle_type;
  using shared_mutex::native_handle;

  template <class R, ratio P>
  bool try_lock_for(const duration<R, P> &d) {
    const steady_clock::time_point t2 = steady_clock::now()
      + duration_ceil<steady_clock::duration>(d);
    for (;;) {
      if (steady_clock::now().count() > t2.count())
        return false;
      if (try_lock())
        return true;
    }
  }
  template <class C, class D>
  bool try_lock_until(const time_point<C, D> &t) {
    const typename C::time_point t2 = time_point_ceil<typename C::duration>(t);
    for (;;) {
      if (C::now().count() > t2.count())
        return false;
      if (try_lock())
        return true;
    }
  }

  template <class R, ratio P>
  bool try_lock_shared_for(const duration<R, P> &d) {
    const steady_clock::time_point t2 = steady_clock::now()
      + duration_ceil<steady_clock::duration>(d);
    for (;;) {
      if (steady_clock::now().count() > t2.count())
        return false;
      if (try_lock_shared())
        return true;
    }
  }
  template <class C, class D>
  bool try_lock_shared_until(const time_point<C, D> &t) {
    const typename C::time_point t2 = time_point_ceil<typename C::duration>(t);
    for (;;) {
      if (C::now().count() > t2.count())
        return false;
      if (try_lock_shared())
        return true;
    }
  }
};

template <class M>
class shared_lock {
  M *p;
  bool owns;

public:
  using mutex_type = M;

  shared_lock() noexcept : p(nullptr), owns(false) {}
  ~shared_lock() {
    if (owns)
      p->unlock_shared();
  }
  shared_lock(const shared_lock &) = delete;
  shared_lock &operator =(const shared_lock &) = delete;
  shared_lock(shared_lock &&u) noexcept : p(u.p), owns(u.owns) {
    u.p = nullptr;
    u.owns = false;
  }
  shared_lock &operator =(shared_lock &&u) {
    if (this != addressof(u)) {
      if (owns)
        p->unlock_shared();
      p = u.p;
      owns = u.owns;
      u.p = nullptr;
      u.owns = false;
    }
    return *this;
  }
  friend void swap(shared_lock &x, shared_lock &y) noexcept {
    adl_swap(x.p, y.p);
    adl_swap(x.owns, y.owns);
  }

  explicit shared_lock(mutex_type &m) : p(addressof(m)), owns(true) {
    m.lock_shared();
  }
  shared_lock(mutex_type &m, defer_lock_t) noexcept
    : p(addressof(m)), owns(false) {}
  shared_lock(mutex_type &m, try_to_lock_t)
    : p(addressof(m)), owns(m.try_lock_shared()) {}
  shared_lock(mutex_type &m, adopt_lock_t) : p(addressof(m)), owns(true) {}

  template <class C, class D>
  shared_lock(mutex_type &m, const time_point<C, D> &t)
    : p(addressof(m)), owns(m.try_lock_shared_until(t)) {}
  template<class R, ratio P>
  shared_lock(mutex_type &m, const duration<R, P> &t)
    : p(addressof(m)), owns(m.try_lock_shared_for(t)) {}

  void lock() {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::lock(): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::lock(): no mutex\n");
    p->lock_shared();
    owns = true;
  }
  bool try_lock() {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock(): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock(): no mutex\n");
    owns = p->try_lock_shared();
    return owns;
  }
  template <class R, ratio P>
  bool try_lock_for(const duration<R, P> &t) {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock_for(t): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock_for(t): no mutex\n");
    owns = p->try_lock_shared_for(t);
    return owns;
  }
  template <class C, class D>
  bool try_lock_until(const time_point<C, D> &t) {
    if (owns)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock_until(t): already locked\n");
    if (p == nullptr)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::try_lock_until(t): no mutex\n");
    owns = p->try_lock_shared_until(t);
    return owns;
  }
  void unlock() {
    if (!owns)
      throw_or_terminate<runtime_error>
        ("re::shared_lock<M>::unlock(t): no locked mutex\n");
    p->unlock_shared();
    owns = false;
  }

  mutex_type *release() noexcept {
    owns = false;
    return exchange(p, nullptr);
  }
  bool owns_lock() const noexcept {
    return owns;
  }
  explicit operator bool() const noexcept {
    return owns;
  }
  mutex_type *mutex() const noexcept {
    return p;
  }
};

}

// thread_pool
// pool_thread
namespace re {

class pool_thread;
class thread_pool {
  using this_t = thread_pool;

  friend class pool_thread;

  struct pool_t;
  struct thrd_t {
    function<void (), 256> any_fn;
    thread thrd;
    thread::id thrd_id;
    bool to_start_or_join = false; // true: start  false: join
    bool to_quit = false; // true: quit
    condition_variable waited_receiver;
    condition_variable waited_worker;

    thrd_t() = default;
    ~thrd_t() = default;
    thrd_t(const thrd_t &) = delete;
    thrd_t &operator =(const thrd_t &) = delete;
    thrd_t(thrd_t &&) = delete;
    thrd_t &operator =(thrd_t &&) = delete;
  };
  struct pool_t {
    bidirectional_list<mutex_area<thrd_t>> running_threads;
    bidirectional_list<mutex_area<thrd_t>> sleeping_threads;
  };
  mutex_area<pool_t> a;

  auto thread_function(mutex_area<thrd_t> &x) {
    return [&x]() {
      for (;;) {
        bool quit_flag = false;
        x.enter([&]() {
          x->thrd_id = get_thread_id();
          for (;;) {
            x.leave_until([&]() {return x->to_start_or_join || x->to_quit;},
                          x->waited_worker);
            if (x->to_quit) {
              quit_flag = true;
              return;
            }
            // to start
            x->any_fn();
            x->to_start_or_join = false;
            // to join
            x.leave_notify_one(x->waited_receiver);
          }
        });
        if (quit_flag)
          return;
      }
    };
  }
public:
  thread_pool()
    : thread_pool(max_value(thread::hardware_concurrency() * 2u, 16u)) {}
  ~thread_pool() {
    for (auto &x : a->sleeping_threads) {
      x.enter([&]() {x->to_quit = true;});
      x->waited_worker.notify_one();
    }
    for (auto &x : a->sleeping_threads)
      x->thrd.join();
  }
  thread_pool(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  thread_pool(this_t &&) = delete;
  this_t &operator =(this_t &&) = delete;

  explicit thread_pool(size_t n) {
    a->sleeping_threads.resize(n);
    for (auto it : iters(a->sleeping_threads)) {
      auto &x = *it;
      x->thrd = thread(thread_function(x));
    }
  }

private:
  using handle_t = bidirectional_list<mutex_area<thrd_t>>::iterator;
  template <class F>
  handle_t awake(F f) {
    handle_t ret{};
    a.enter([&]() {
      if (a->sleeping_threads.empty())
        return;
      const auto it = a->sleeping_threads.begin();
      auto &x = *it;
      x.enter([&]() {
        auto &l = a->running_threads;
        auto &ll = a->sleeping_threads;
        l.splice(l.end(), ll, it);
        x->any_fn = move(f);
        x->to_start_or_join = true; // to start
      });
      ret = it;
    });
    if (ret != handle_t{})
      (*ret)->waited_worker.notify_one();
    return ret;
  }
  void join(handle_t it) {
    auto &x = *it;
    x.enter([&]() {
      x.leave_until([&]() {return x->to_start_or_join == false;},
                    x->waited_receiver);
      x->any_fn.clear();
    });
    a.enter([&]() {
      auto &l = a->running_threads;
      auto &ll = a->sleeping_threads;
      ll.splice(ll.end(), l, it);
    });
  }

public:
  template <class F>
  pool_thread fetch(F &&);
  template <class F>
  pool_thread fetch_real(F &&);

  using size_type = size_t;
  size_type count() noexcept {
    size_type ret = 0u;
    a.enter([&]() {
      ret = re::size(a->running_threads) + re::size(a->sleeping_threads);
    });
    return ret;
  }
  size_type count_running() noexcept {
    size_type ret = 0u;
    a.enter([&]() {ret = re::size(a->running_threads);});
    return ret;
  }
  size_type count_sleeping() noexcept {
    size_type ret = 0u;
    a.enter([&]() {ret = re::size(a->sleeping_threads);});
    return ret;
  }
  void shrink_to_fit() {
    a.enter([&]() {
      for (auto &x : a->sleeping_threads) {
        x.enter([&]() {x->to_quit = true;});
        x->waited_worker.notify_one();
        x->thrd.join();
      }
      a->sleeping_threads.clear();
    });
  }
  void append(size_type n) {
    a.enter([&]() {
      for (; n != 0; --n) {
        auto &x = a->sleeping_threads.emplace_back();
        x->thrd = thread(thread_function(x));
      }
    });
  }
};

struct fo_default_thread_pool {
  thread_pool &operator ()() const {
    static thread_pool p(max_value(thread::hardware_concurrency() * 8u, 32u));
    return p;
  }
};
inline constexpr fo_default_thread_pool default_thread_pool{};

class pool_thread {
  friend class thread_pool;
public:
  using id = thread::id;
  using native_handle_type = HANDLE;

private:
  using handle_t = thread_pool::handle_t;
  handle_t it = handle_t{};
  thread_pool *p = nullptr;

public:
  pool_thread() noexcept = default;
  ~pool_thread() {
    if (it != handle_t{})
      p->join(it);
  }
  pool_thread(const pool_thread &) = delete;
  pool_thread &operator =(const pool_thread &) = delete;
  pool_thread(pool_thread &&x) noexcept : it(x.it), p(x.p) {
    x.it = handle_t{};
    x.p = nullptr;
  }
  pool_thread &operator =(pool_thread &&x) noexcept {
    if (this != addressof(x)) {
      if (it != handle_t{})
        print_then_terminate
          ("re::pool_thread::=(re::pool_thread &&): "
           "only null thread can be assigned to\n");
      it = exchange(x.it, handle_t{});
      p = exchange(x.p, nullptr);
    }
    return *this;
  }
  friend void swap(pool_thread &x, pool_thread &y) noexcept {
    adl_swap(x.it, y.it);
    adl_swap(x.p, y.p);
  }

  template <class F>
  explicit pool_thread(F &&f)
    requires (is_constructible_v<decay_t<F>, F>
              && is_invocable_v<decay_t<F>>) {
    p = addressof(default_thread_pool());
    it = p->awake(forward<F>(f));
    if (it == handle_t{})
      f();
  }

public:
  bool joinable() const noexcept {
    return it != handle_t{};
  }
  void join() {
    if (it == handle_t{})
      throw_or_terminate<runtime_error>
        ("re::pool_thread::join(): non-joinable\n");
    p->join(it);
    it = handle_t{};
    p = nullptr;
  }
  bool try_join() {
    if (it == handle_t{})
      return false;
    p->join(it);
    it = handle_t{};
    p = nullptr;
    return true;
  }

  thread_pool *pool() const noexcept {
    return p;
  }

  id get_id() const noexcept {
    return (it == handle_t{}) ? id{} : (*it)->thrd_id;
  }
  native_handle_type native_handle() {
    return (it == handle_t{})
      ? INVALID_HANDLE_VALUE : (*it)->thrd.native_handle();
  }
};
template <class F>
pool_thread thread_pool::fetch(F &&f) {
  pool_thread ret;
  ret.p = this;
  ret.it = awake(forward<F>(f));
  if (ret.it == handle_t{})
    f();
  return ret;
}
template <class F>
pool_thread thread_pool::fetch_real(F &&f) {
  pool_thread ret;
  ret.p = this;
  for (;;) {
    ret.it = awake(forward<F>(f));
    if (ret.it != handle_t{})
      break;
    append(1u);
  }
  return ret;
}

}

// exclusive_thread_pool
namespace re {

class exclusive_thread_pool {
  using this_t = exclusive_thread_pool;

  struct pool_t;
  struct thrd_t {
    function<void (), 256> any_fn;
    thread thrd;
    bool to_start = false;
    bool to_quit = false;
    condition_variable waited_worker;

    thrd_t() = default;
    ~thrd_t() = default;
    thrd_t(const thrd_t &) = delete;
    thrd_t &operator =(const thrd_t &) = delete;
    thrd_t(thrd_t &&) = delete;
    thrd_t &operator =(thrd_t &&) = delete;
  };
  struct pool_t {
    bidirectional_list<mutex_area<thrd_t>> running_threads;
    bidirectional_list<mutex_area<thrd_t>> sleeping_threads;
    condition_variable waited_controller;
  };
  mutex_area<pool_t> a;

  auto thread_function(bidirectional_list<mutex_area<thrd_t>>
                       ::iterator it) {
    return [it, this]() {
      mutex_area<thrd_t> &x = *it;
      for (;;) {
        bool quit_flag = false;
        x.enter([&]() {
          for (;;) {
            x.leave_until([&]() {return x->to_start || x->to_quit;},
                          x->waited_worker);
            if (x->to_quit) {
              quit_flag = true;
              return;
            }
            // to start
            x->any_fn();
            x->to_start = false;
            // to join
            a.enter([it, this]() {
              auto &l = a->running_threads;
              auto &ll = a->sleeping_threads;
              ll.splice(ll.end(), l, it);
            });
            if (a->running_threads.empty())
              a->waited_controller.notify_one();
          }
        });
        if (quit_flag)
          return;
      }
    };
  }
public:
  exclusive_thread_pool()
    : exclusive_thread_pool(max_value(thread::hardware_concurrency() * 2u,
                                      16u)) {}
  ~exclusive_thread_pool() {
    for (auto &x : a->sleeping_threads) {
      x.enter([&]() {x->to_quit = true;});
      x->waited_worker.notify_one();
    }
    for (auto &x : a->sleeping_threads)
      x->thrd.join();
  }
  exclusive_thread_pool(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  exclusive_thread_pool(this_t &&) = delete;
  this_t &operator =(this_t &&) = delete;

  explicit exclusive_thread_pool(size_t n) {
    a->sleeping_threads.resize(n);
    for (auto &it : iters(a->sleeping_threads))
      (*it)->thrd = thread(thread_function(it));
  }

  template <class F>
  void fetch(F f) {
    bool done = false;
    a.enter([&]() {
      if (a->sleeping_threads.empty())
        return;
      const auto it = a->sleeping_threads.begin();
      auto &x = *it;
      x.enter([&]() {
        auto &l = a->running_threads;
        auto &ll = a->sleeping_threads;
        l.splice(l.end(), ll, it);
        x->any_fn = move(f);
        x->to_start = true;
      });
      x->waited_worker.notify_one();
      done = true;
    });
    if (!done)
      f();
  }
  void join() {
    a.enter([&]() {
      a.leave_until([&]() {return a->running_threads.empty();},
                    a->waited_controller);
    });
  }

  using size_type = size_t;
  size_type count() noexcept {
    size_type ret = 0u;
    a.enter([&]() {
      ret = re::size(a->running_threads) + re::size(a->sleeping_threads);
    });
    return ret;
  }
  size_type count_running() noexcept {
    size_type ret = 0u;
    a.enter([&]() {ret = re::size(a->running_threads);});
    return ret;
  }
  size_type count_sleeping() noexcept {
    size_type ret = 0u;
    a.enter([&]() {ret = re::size(a->sleeping_threads);});
    return ret;
  }
  void shrink_to_fit() {
    a.enter([&]() {
      for (auto &x : a->sleeping_threads) {
        x.enter([&]() {x->to_quit = true;});
        x->waited_worker.notify_one();
      }
      for (auto &x : a->sleeping_threads)
        x->thrd.join();
      a->sleeping_threads.clear();
    });
  }
  void append(size_type n) {
    a.enter([&]() {
      for (; n != 0u; --n) {
        auto &x = a->sleeping_threads.emplace_back();
        x->thrd = thread(thread_function(before_end(a->sleeping_threads)));
      }
    });
  }
};

}

// sync_object_pool
namespace re {

template <class T, class AL = default_allocator<byte>>
class sync_pool_object;
template <class T, class AL = default_allocator<byte>>
class sync_object_pool {
  object_pool<T, AL> pl;
  mutex mtx;

public:
  using size_type = typename object_pool<T, AL>::size_type;

  sync_object_pool() : sync_object_pool(AL{}) {}
  ~sync_object_pool() = default;
  sync_object_pool(const sync_object_pool &) = delete;
  sync_object_pool &operator =(const sync_object_pool &) = delete;
  sync_object_pool(sync_object_pool &&) = delete;
  sync_object_pool &operator =(sync_object_pool &&) = delete;

  explicit sync_object_pool(const AL &a) : pl(a) {}

  size_type capacity() noexcept {
    lock_guard g(mtx);
    return pl.capacity();
  }
  size_type used_size() noexcept {
    lock_guard g(mtx);
    return pl.used_size();
  }
  size_type idle_size() noexcept {
    lock_guard g(mtx);
    return pl.idle_size();
  }
  void reserve_more(size_type n) {
    lock_guard g(mtx);
    pl.reserve_more(n);
  }
  template <class...S>
  T *fetch_pointer(S &&...s) {
    lock_guard g(mtx);
    return pl.fetch_pointer(forward<S>(s)...);
  }
  void free_pointer(T *p) noexcept {
    lock_guard g(mtx);
    pl.free_pointer(p);
  }

  template <class...S>
  sync_pool_object<T, AL> fetch(S &&...s) {
    lock_guard g(mtx);
    sync_pool_object<T, AL> ret;
    T *p = fetch_pointer(forward<S>(s)...);
    ret.p = p;
    ret.pl = this;
    return ret;
  }
};
template <class T, class AL>
class sync_pool_object {
  template <class, class>
  friend class sync_object_pool;

  T *p;
  sync_object_pool<T, AL> *pl;

public:
  sync_pool_object() : p{}, pl{} {}
  ~sync_pool_object() {
    if (p != nullptr)
      pl->free_pointer(p);
  }
  sync_pool_object(const sync_pool_object &) = delete;
  sync_pool_object &operator =(const sync_pool_object &) = delete;
  sync_pool_object(sync_pool_object &&x) noexcept : p(x.p), pl(x.pl) {
    x.p = nullptr;
    x.pl = nullptr;
  }
  sync_pool_object &operator =(sync_pool_object &&x) noexcept {
    sync_pool_object tmp(move(x));
    adl_swap(*this, tmp);
    return *this;
  }
  friend void swap(sync_pool_object &a, sync_pool_object &b) noexcept {
    adl_swap(a.p, b.p);
    adl_swap(a.pl, b.pl);
  }

  bool empty() const noexcept {
    return p == nullptr;
  }
  T &value() {
    if (p == nullptr)
      throw_or_terminate<logic_error>
        ("re::sync_pool_object::value(): no value\n");
    return *p;
  }
  const T &value() const {
    if (p == nullptr)
      throw_or_terminate<logic_error>
        ("re::sync_pool_object::value(): no value\n");
    return *p;
  }

  T *operator ->() noexcept {
    return p;
  }
  T &operator *() noexcept {
    return *p;
  }
  const T *operator ->() const noexcept {
    return p;
  }
  const T &operator *() const noexcept {
    return *p;
  }
};

}

#endif
