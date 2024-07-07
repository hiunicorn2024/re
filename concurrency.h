#ifndef RE_DEFINED_CONCURRENCY_H
#define RE_DEFINED_CONCURRENCY_H

#include "base.h"

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

#ifndef RE_WIN32_NO_X64_INTRINSICS
template <>
struct atomic<long long> {
  using value_type = long long;

private:
  mutable volatile long long v;

public:
  inline atomic() noexcept : v() {}
  ~atomic() = default;
  atomic(const atomic &) = delete;
  atomic &operator =(const atomic &) = delete;
  atomic(atomic &&) = delete;
  atomic &operator =(atomic &&) = delete;

  atomic &operator =(const atomic &) volatile = delete;

  inline atomic(long long x) noexcept : v(x) {}
  inline long long operator =(long long x) noexcept {
    _InterlockedExchange64(&v, x);
    return x;
  }
  inline long long operator =(long long x) volatile noexcept {
    _InterlockedExchange64(&v, x);
    return x;
  }
  inline operator long long() const noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline operator long long() const volatile noexcept {
    return _InterlockedOr64(&v, 0l);
  }

  inline long long load() const noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline long long load() const volatile noexcept {
    return _InterlockedOr64(&v, 0l);
  }
  inline void store(long long x) noexcept {
    _InterlockedExchange64(&v, x);
  }
  inline void store(long long x) volatile noexcept {
    _InterlockedExchange64(&v, x);
  }
  inline long long exchange(long long x) noexcept {
    return _InterlockedExchange64(&v, x);
  }
  inline long long exchange(long long x) volatile noexcept {
    return _InterlockedExchange64(&v, x);
  }

  inline bool compare_exchange(long long &expected,
                               long long desired) noexcept {
    if (const long long x = _InterlockedCompareExchange64(&v,
                                                          desired,
                                                          expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline bool compare_exchange(long long &expected,
                               long long desired) volatile noexcept {
    if (const long long x = _InterlockedCompareExchange64(&v,
                                                          desired,
                                                          expected);
        x != expected) {
      expected = x;
      return false;
    }
    else
      return true;
  }
  inline long long fetch_add(long long x) noexcept {
    return _InterlockedExchangeAdd64(&v, x);
  }
  inline long long fetch_add(long long x) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, x);
  }
  inline long long fetch_sub(long long x) noexcept {
    return _InterlockedExchangeAdd64(&v, -x);
  }
  inline long long fetch_sub(long long x) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, -x);
  }

  inline long long operator ++() noexcept {
    return _InterlockedIncrement64(&v);
  }
  inline long long operator ++() volatile noexcept {
    return _InterlockedIncrement64(&v);
  }
  inline long long operator ++(int) noexcept {
    return _InterlockedExchangeAdd64(&v, 1l);
  }
  inline long long operator ++(int) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, 1l);
  }

  inline long long operator --() noexcept {
    return _InterlockedDecrement64(&v);
  }
  inline long long operator --() volatile noexcept {
    return _InterlockedDecrement64(&v);
  }
  inline long long operator --(int) noexcept {
    return _InterlockedExchangeAdd64(&v, -1l);
  }
  inline long long operator --(int) volatile noexcept {
    return _InterlockedExchangeAdd64(&v, -1l);
  }

  inline long long operator +=(long long x) noexcept {
    return InterlockedAdd64(&v, x);
  }
  inline long long operator +=(long long x) volatile noexcept {
    return InterlockedAdd64(&v, x);
  }
  inline long long operator -=(long long x) noexcept {
    return InterlockedAdd64(&v, -x);
  }
  inline long long operator -=(long long x) volatile noexcept {
    return InterlockedAdd64(&v, -x);
  }

  inline long long operator &=(long long x) noexcept {
    return _InterlockedAnd64(&v, x) & x;
  }
  inline long long operator &=(long long x) volatile noexcept {
    return _InterlockedAnd64(&v, x) & x;
  }
  inline long long operator |=(long long x) noexcept {
    return _InterlockedOr64(&v, x) | x;
  }
  inline long long operator |=(long long x) volatile noexcept {
    return _InterlockedOr64(&v, x) | x;
  }
  inline long long operator ^=(long long x) noexcept {
    return _InterlockedXor64(&v, x) ^ x;
  }
  inline long long operator ^=(long long x) volatile noexcept {
    return _InterlockedXor64(&v, x) ^ x;
  }
};
#endif

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

DWORD WINAPI win32_thread_entry_fn(void *p) {
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
      print_then_terminate
        ("re::thread::~thread(): can not owns a thread at this time point\n");
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
        ("re::thread::=(thread &&): only null thread can be assigned to\n");
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
        ("thread(f, s...): CreateThread() failed");
    if (id_num == id{})
      throw_or_terminate<runtime_error>
        ("thread(f, s...): id is zero after CreateThread()");
  }

  bool joinable() const noexcept {
    return get_id() != id{};
  }
  void join() {
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
  mutex &operator=(const mutex &) = delete;
  mutex(mutex &&) = delete;
  mutex &operator=(mutex &&) = delete;

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
    p = exchange(u.p, nullptr);
    owns = exchange(u.owns, false);
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
      throw runtime_error
        ("re::unique_lock<M>::lock(): already locked\n");
    if (p == nullptr)
      throw runtime_error
        ("re::unique_lock<M>::lock(): no mutex\n");
    p->lock();
    owns = true;
  }
  bool try_lock() {
    if (owns)
      throw runtime_error
        ("re::unique_lock<M>::try_lock(): already locked\n");
    if (p == nullptr)
      throw runtime_error
        ("re::unique_lock<M>::try_lock(): no mutex\n");
    owns = p->try_lock();
    return owns;
  }
  template <class R, ratio P>
  bool try_lock_for(const duration<R, P> &t) {
    if (owns)
      throw runtime_error
        ("re::unique_lock<M>::try_lock_for(t): already locked\n");
    if (p == nullptr)
      throw runtime_error
        ("re::unique_lock<M>::try_lock_for(t): no mutex\n");
    owns = p->try_lock_for(t);
    return owns;
  }
  template <class C, class D>
  bool try_lock_until(const time_point<C, D> &t) {
    if (owns)
      throw runtime_error
        ("re::unique_lock<M>::try_lock_until(t): already locked\n");
    if (p == nullptr)
      throw runtime_error
        ("re::unique_lock<M>::try_lock_until(t): no mutex\n");
    owns = p->try_lock_until(t);
    return owns;
  }
  void unlock() {
    if (!owns)
      throw runtime_error
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
  mutex_area() = default;
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

// thread_pool
namespace re {

class basic_thread_pool {
  using this_t = basic_thread_pool;

  struct pool_t;
  struct thrd_t {
    function<void (), 1024u> any_fn;
    thread thrd;
    thread::id thrd_id;
    mutex_area<pool_t> *pool_p = nullptr;
    bool to_start_or_join = false; // true: start  false: join
    bool to_quit = false; // true: quit
    condition_variable waited_owner;
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

public:
  basic_thread_pool(size_t n
                    = max_value(thread::hardware_concurrency() * 16u,
                                32u)) {
    a->sleeping_threads.resize(n);
    for (auto it : iters(a->sleeping_threads)) {
      auto &x = *it;
      x->pool_p = addressof(a);
      x->thrd = thread([&]() {
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
              x.leave_notify_one(x->waited_owner);
            }
          });
          if (quit_flag)
            return;
        }
      });
    }
  }
  ~basic_thread_pool() {
    for (auto &x : a->sleeping_threads) {
      x.enter([&]() {x->to_quit = true;});
      x->waited_worker.notify_one();
      x->thrd.join();
    }
  }
  basic_thread_pool(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  basic_thread_pool(this_t &&) = delete;
  this_t &operator =(this_t &&) = delete;

  using handle_t = bidirectional_list<mutex_area<thrd_t>>::iterator;
  template <class F>
  handle_t awake(F f) {
    handle_t ret{};
    a.enter([&]() {
      if (a->sleeping_threads.empty())
        return;
      auto it = a->sleeping_threads.begin();
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
                    x->waited_owner);
      x->any_fn.clear();
    });
    a.enter([&]() {
      auto &l = a->running_threads;
      auto &ll = a->sleeping_threads;
      ll.splice(ll.end(), l, it);
    });
  }

  void clear() {
    for (auto &x : a->sleeping_threads) {
      x.enter([&]() {x->to_quit = true;});
      x->waited_worker.notify_one();
      x->thrd.join();
    }
    a->sleeping_threads.clear();
  }
};
namespace inner::fns {

basic_thread_pool &get_thread_pool_ref() {
  static basic_thread_pool p;
  return p;
}

}

class pool_thread {
public:
  using id = thread::id;
  using native_handle_type = HANDLE;

private:
  using handle_t = basic_thread_pool::handle_t;
  handle_t it = handle_t{};

public:
  pool_thread() noexcept = default;
  ~pool_thread() {
    if (it != handle_t{})
      inner::fns::get_thread_pool_ref().join(it);
  }
  pool_thread(const pool_thread &) = delete;
  pool_thread &operator =(const pool_thread &) = delete;
  pool_thread(pool_thread &&x) noexcept : it(x.it) {
    x.it = handle_t{};
  }
  pool_thread &operator =(pool_thread &&x) noexcept {
    it = exchange(x.it, handle_t{});
    return *this;
  }
  friend void swap(pool_thread &x, pool_thread &y) noexcept {
    adl_swap(x.it, y.it);
  }

  template <class F>
  explicit pool_thread(F &&f)
    requires (is_constructible_v<decay_t<F>, F>
              && is_invocable_v<decay_t<F>>) {
    it = inner::fns::get_thread_pool_ref().awake(forward<F>(f));
    if (it == handle_t{})
      throw_or_terminate<runtime_error>
        ("re::pool_thread(f, real_thread): full pool\n");
  }

  bool joinable() const noexcept {
    return true;
  }
  void join() {
    if (it != handle_t{}) {
      inner::fns::get_thread_pool_ref().join(it);
      it = handle_t{};
    }
  }

  id get_id() const noexcept {
    return (it == handle_t{}) ? id{} : (*it)->thrd_id;
  }
  native_handle_type native_handle() {
    return (it == handle_t{})
      ? INVALID_HANDLE_VALUE : (*it)->thrd.native_handle();
  }

  static unsigned int hardware_concurrency() noexcept {
    return thread::hardware_concurrency();
  }
};

}

#endif
