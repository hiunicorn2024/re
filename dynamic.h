#ifndef RE_DEFINED_DYNAMIC_H
#define RE_DEFINED_DYNAMIC_H

#include "base.h"
#include "allocator.h"

// base
namespace re {

template <class, size_t, size_t>
class dynamic;
template <class F, size_t, size_t>
class function;
template <class...S>
class variant;

template <class T>
struct fo_is {
  template <class U>
  constexpr auto operator ()(U &&x)
    const noexcept(noexcept(declval<U>().template is<T>()))
    ->decltype(declval<U>().template is<T>()) {
    return forward<U>(x).template is<T>();
  }
};
template <class T>
inline constexpr fo_is<T> is{};

template <size_t I>
struct in_place_index_t {
  explicit in_place_index_t() = default;
};
template <size_t I>
inline constexpr in_place_index_t<I> in_place_index{};
template <class T>
struct is_in_place_index : false_type {};
template <size_t I>
struct is_in_place_index<in_place_index_t<I>> : true_type {};
template <size_t I>
struct is_in_place_index<const in_place_index_t<I>> : true_type {};
template <size_t I>
struct is_in_place_index<volatile in_place_index_t<I>> : true_type {};
template <size_t I>
struct is_in_place_index<const volatile in_place_index_t<I>> : true_type {};
template <class T>
inline constexpr bool is_in_place_index_v = is_in_place_index<T>::value;

}

// dynamic
namespace re {

class dynamic_object {
public:
  virtual void re_dynamic_clear() noexcept {}
  virtual void *re_dynamic_auto_copy_to(void *) const {
    return nullptr;
  }
  virtual void *re_dynamic_auto_move_to(void *) noexcept {
    return nullptr;
  }
  virtual void *re_dynamic_copy_to_uniform(void *) const {
    return nullptr;
  }
  virtual void *re_dynamic_move_to_uniform(void *) {
    return nullptr;
  }
  virtual const type_info &re_dynamic_typeid() const noexcept {
    return typeid(void);
  }
  virtual size_t re_dynamic_size() const noexcept {
    return 0;
  }
  virtual size_t re_dynamic_align() const noexcept {
    return 0;
  }
  virtual size_t re_dynamic_wrapper_size() const noexcept {
    return 0;
  }
  virtual size_t re_dynamic_wrapper_align() const noexcept {
    return 0;
  }
};

template <class BASE>
struct dynamic_default_buffer_traits {
  static constexpr size_t align = __STDCPP_DEFAULT_NEW_ALIGNMENT__;
  static constexpr size_t size = (sizeof(BASE) <= (sizeof(void *) * 3))
    ? (sizeof(void *) * 7) : (sizeof(BASE) * 5 / 2);
};

struct bad_dynamic_access : public exception {
  virtual const char *what() const noexcept override {
    return "re::dynamic: try to access value of empty dynamic \n";
  }
};

namespace inner {

template <class, class, size_t, size_t>
class dynamic_impl;

template <class T>
class dynamic_composite_data : public T, public dynamic_object {
public:
  dynamic_composite_data() : T() {}
  ~dynamic_composite_data() = default;
  dynamic_composite_data(const dynamic_composite_data &) = delete;
  dynamic_composite_data &operator =(const dynamic_composite_data &) = delete;
  dynamic_composite_data(dynamic_composite_data &&) = delete;
  dynamic_composite_data &operator =(dynamic_composite_data &&) = delete;

  template <class...S>
  dynamic_composite_data(S &&...s) : T(forward<S>(s)...) {}
};
template <class T>
using dynamic_data = conditional_t<is_base_of_v<dynamic_object, T>,
                                   T, dynamic_composite_data<T>>;

template <class IMPL_T>
concept local_dynamic_impl = (sizeof(IMPL_T) <= IMPL_T::bufsz
                              && alignof(IMPL_T) <= IMPL_T::bufalign)
#ifndef RE_NOEXCEPT
  && is_nothrow_move_constructible_v<typename IMPL_T::value_type>
#endif
  ;

template <class BASE, class T, size_t BUFSZ, size_t BUFALIGN>
class dynamic_impl : public dynamic_data<T> {
public:
  using this_t = dynamic_impl;
  using data_t = dynamic_data<T>;
  using base_t = BASE;
  using value_type = T;
  static constexpr size_t bufsz = BUFSZ;
  static constexpr size_t bufalign = BUFALIGN;

  using alloc_t = default_allocator<this_t>;
  using alw_t = allocator_wrapper<alloc_t>;
  using uniform_t = dynamic_impl<BASE, T,
                                 dynamic_default_buffer_traits<BASE>::size,
                                 dynamic_default_buffer_traits<BASE>::align>;
  using uniform_alloc_t = default_allocator<uniform_t>;
  using uniform_alw_t = allocator_wrapper<uniform_alloc_t>;

  T &value() noexcept {
    return static_cast<T &>(*this);
  }
  const T &value() const noexcept {
    return static_cast<const T &>(*this);
  }

  dynamic_impl() : data_t() {}
  ~dynamic_impl() = default;
  dynamic_impl(const dynamic_impl &) = delete;
  dynamic_impl &operator =(const dynamic_impl &) = delete;
  dynamic_impl(dynamic_impl &&) = delete;
  dynamic_impl &operator =(dynamic_impl &&) = delete;

  template <class...S>
  dynamic_impl(S &&...s) : data_t(forward<S>(s)...) {}

  virtual void re_dynamic_clear() noexcept override {
    if constexpr (local_dynamic_impl<this_t>) {
      alw_t{}.destroy(this);
    }
    else {
      alw_t{}.delete_1(pointer_to<alloc_ptr<alloc_t>>(*this));
    }
  }
  virtual void *re_dynamic_auto_copy_to(void *bufp) const override {
    if constexpr (local_dynamic_impl<this_t>) {
      this_t *const p = reinterpret_cast<this_t *>(bufp);
      alw_t{}.construct(p, value());
      return static_cast<BASE *>(p);
    }
    else {
      this_t *const p = to_address(alw_t{}.new_1(value()));
      return static_cast<BASE *>(p);
    }
  }
  virtual void *re_dynamic_auto_move_to(void *bufp) noexcept override {
    if constexpr (local_dynamic_impl<this_t>) {
      this_t *const p = reinterpret_cast<this_t *>(bufp);
      alw_t{}.construct(p, move(value()));
      re_dynamic_clear();
      return static_cast<BASE *>(p);
    }
    else {
      return static_cast<BASE *>(this);
    }
  }
  virtual void *re_dynamic_copy_to_uniform(void *bufp) const override {
    if constexpr (local_dynamic_impl<uniform_t>) {
      uniform_t *const p = reinterpret_cast<uniform_t *>(bufp);
      uniform_alw_t{}.construct(p, value());
      return static_cast<BASE *>(p);
    }
    else {
      uniform_t *const p = to_address(uniform_alw_t{}.new_1(value()));
      return static_cast<BASE *>(p);
    }
  }
  virtual void *re_dynamic_move_to_uniform(void *bufp) override {
    if constexpr (local_dynamic_impl<uniform_t>) {
      uniform_t *const p = reinterpret_cast<uniform_t *>(bufp);
      uniform_alw_t{}.construct(p, move(value()));
      re_dynamic_clear();
      return static_cast<BASE *>(p);
    }
    else {
      uniform_t *p = to_address(uniform_alw_t{}.new_1(move(value())));
      re_dynamic_clear();
      return static_cast<BASE *>(p);
    }
  }
  virtual const type_info &re_dynamic_typeid() const noexcept override {
    return typeid(T);
  }
  virtual size_t re_dynamic_size() const noexcept override {
    return sizeof(T);
  }
  virtual size_t re_dynamic_align() const noexcept override {
    return alignof(T);
  }
  virtual size_t re_dynamic_wrapper_size() const noexcept override {
    return sizeof(this_t);
  }
  virtual size_t re_dynamic_wrapper_align() const noexcept override {
    return alignof(this_t);
  }
};

namespace fns {

template <class IMPL_T, class...S>
void *make_dynamic_impl(void *bufp, S &&...s) {
  using alw_t = typename IMPL_T::alw_t;
  IMPL_T *p = to_address(alw_t{}.new_1(forward<S>(s)...));
  return static_cast<typename IMPL_T::base_t *>(p);
}
template <class IMPL_T, class...S>
void *make_dynamic_impl(void *bufp, S &&...s)
  requires local_dynamic_impl<IMPL_T> {
  using alw_t = typename IMPL_T::alw_t;
  IMPL_T *const p = reinterpret_cast<IMPL_T *>(bufp);
  alw_t{}.construct(p, forward<S>(s)...);
  return static_cast<typename IMPL_T::base_t *>(p);
}

}

template <size_t BUFSZ, size_t ALIGN>
struct dynamic_optional_buf {
  alignas(ALIGN) byte buf[BUFSZ];

  void *bufp() noexcept {
    return reinterpret_cast<void *>(addressof(buf));
  }
};
template <size_t ALIGN>
struct dynamic_optional_buf<0, ALIGN> {
  void *bufp() noexcept {
    return nullptr;
  }
};

template <class U, class T>
concept compatible_type_of_dynamic
  = is_base_of_v<T, U> && is_convertible_v<U &, T &>
  && is_move_constructible_v<U> && is_copy_constructible_v<U>;
template <class U>
concept compatible_type_of_any
  = is_move_constructible_v<U> && is_copy_constructible_v<U>;

}
template <class T = void,
          size_t BUFSZ = dynamic_default_buffer_traits<T>::size,
          size_t BUFALIGN = dynamic_default_buffer_traits<T>::align>
class dynamic : inner::dynamic_optional_buf<BUFSZ, BUFALIGN> {
  static_assert(is_class_v<T>);
  static_assert(is_same_v<remove_cvref_t<T>, T>);

  template <class, size_t, size_t>
  friend class dynamic;
  template <class, size_t, size_t>
  friend class function;

  using this_t = dynamic;
  using inner::dynamic_optional_buf<BUFSZ, BUFALIGN>::bufp;

  template <class X>
  using impl_t = inner::dynamic_impl<T, X, BUFSZ, BUFALIGN>;

  template <class, size_t, size_t>
  friend class dynamic;

  T *p;

  static dynamic_object *base(T *pp) noexcept {
    return dynamic_cast<dynamic_object *>(pp);
  }
  static dynamic_object *base(T *pp) noexcept
    requires is_base_of_v<dynamic_object, T> {
    return (dynamic_object *)pp; // accept private base
  }

  void auto_copy_from(const dynamic &x) {
    if (x.p != nullptr)
      p = reinterpret_cast<T *>(base(x.p)->re_dynamic_auto_copy_to(bufp()));
  }
  void auto_move_from(dynamic &x) {
    if (x.p != nullptr) {
      p = reinterpret_cast<T *>(base(x.p)->re_dynamic_auto_move_to(bufp()));
      x.p = nullptr;
    }
  }

  template <size_t SZ, size_t ALGN>
  void uniform_copy_from(const dynamic<T, SZ, ALGN> &x) {
    if (x.p != nullptr)
      p = reinterpret_cast<T *>(base(x.p)->re_dynamic_copy_to_uniform(bufp()));
  }
  template <size_t SZ, size_t ALGN>
  void uniform_move_from(dynamic<T, SZ, ALGN> &x) {
    if (x.p != nullptr) {
      p = reinterpret_cast<T *>(base(x.p)->re_dynamic_move_to_uniform(bufp()));
      x.p = nullptr;
    }
  }

public:
  dynamic() noexcept : p(nullptr) {}
  ~dynamic() {
    if (p != nullptr)
      base(p)->re_dynamic_clear();
  }
  dynamic(const dynamic &x) : p(nullptr) {
    auto_copy_from(x);
  }
  dynamic &operator =(const dynamic &x) {
    copy_and_move(x, *this);
    return *this;
  }
  dynamic(dynamic &&x) noexcept : p(nullptr) {
    auto_move_from(x);
  }
  dynamic &operator =(dynamic &&x) noexcept {
    if (addressof(x) != this) {
      reset();
      auto_move_from(x);
    }
    return *this;
  }
  friend void swap(dynamic &x, dynamic &y) noexcept {
    if (x.p != nullptr) {
      if (y.p != nullptr) {
        if (x.p != y.p) {
          inner::dynamic_optional_buf<BUFSZ, BUFALIGN> tmp;
          void *const tmp_bufp = tmp.bufp();
          T *tmp_p = reinterpret_cast<T *>(base(x.p)
                                           ->re_dynamic_auto_move_to(tmp_bufp));
          x.p = reinterpret_cast<T *>(base(y.p)
                                      ->re_dynamic_auto_move_to(x.bufp()));
          y.p = reinterpret_cast<T *>(base(tmp_p)
                                      ->re_dynamic_auto_move_to(y.bufp()));
        }
      }
      else {
        y.p = reinterpret_cast<T *>(base(x.p)
                                    ->re_dynamic_auto_move_to(y.bufp()));
        x.p = nullptr;
      }
    }
    else {
      if (y.p != nullptr) {
        x.p = reinterpret_cast<T *>(base(y.p)
                                    ->re_dynamic_auto_move_to(x.bufp()));
        y.p = nullptr;
      }
    }
  }

  dynamic(nullptr_t) noexcept {
    p = nullptr;
  }
  dynamic &operator =(nullptr_t) noexcept {
    reset();
    return *this;
  }

  template <class U>
  dynamic(U &&x)
    requires (!is_same_v<decay_t<U>, this_t>
              && !is_in_place_type_v<decay_t<U>>
              && inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_constructible_v<decay_t<U>, U &&>) {
    p = reinterpret_cast<T *>
      (inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
       (bufp(), forward<U>(x)));
  }
  template <class U>
  dynamic &operator =(U &&x)
    requires (!is_same_v<decay_t<U>, this_t>
              && inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_assignable_v<decay_t<U> &, U &&>) {
    reset();
    p = reinterpret_cast<T *>
      (inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
       (bufp(), forward<U>(x)));
    return *this;
  }

  template <class U, class...S>
  explicit dynamic(in_place_type_t<U>, S &&...s)
    requires (inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_constructible_v<decay_t<U>, S &&...>) {
    p = reinterpret_cast<T *>(inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
                              (bufp(), forward<S>(s)...));
  }
  template <class U, class...S>
  void emplace(S &&...s)
    requires (inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_constructible_v<decay_t<U>, S &&...>) {
    reset();
    p = reinterpret_cast<T *>(inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
                              (bufp(), forward<S>(s)...));
  }

  template <class U, class V, class...S>
  explicit dynamic(in_place_type_t<U>, initializer_list<V> l, S &&...s)
    requires (inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_constructible_v
              <decay_t<U>, initializer_list<V> &, S &&...>) {
    p = reinterpret_cast<T *>
      (inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
       (bufp(), l, forward<S>(s)...));
  }
  template <class U, class V, class...S>
  void emplace(initializer_list<V> l, S &&...s)
    requires (inner::compatible_type_of_dynamic<decay_t<U>, T>
              && is_constructible_v
              <decay_t<U>, initializer_list<V> &, S &&...>) {
    reset();
    p = reinterpret_cast<T *>
      (inner::fns::make_dynamic_impl<impl_t<decay_t<U>>>
       (bufp(), l, forward<S>(s)...));
  }

  template <size_t SZ, size_t ALGN>
  dynamic(const dynamic<T, SZ, ALGN> &x)
    requires (!(SZ == BUFSZ && ALGN == BUFALIGN)
              && BUFSZ == dynamic_default_buffer_traits<T>::size
              && BUFALIGN == dynamic_default_buffer_traits<T>::align)
    : p(nullptr) {
    uniform_copy_from(x);
  }
  template <size_t SZ, size_t ALGN>
  dynamic &operator =(const dynamic<T, SZ, ALGN> &x)
    requires (!(SZ == BUFSZ && ALGN == BUFALIGN)
              && BUFSZ == dynamic_default_buffer_traits<T>::size
              && BUFALIGN == dynamic_default_buffer_traits<T>::align) {
    reset();
    uniform_copy_from(x);
    return *this;
  }

  template <size_t SZ, size_t ALGN>
  dynamic(dynamic<T, SZ, ALGN> &&x)
    requires (!(SZ == BUFSZ && ALGN == BUFALIGN)
              && BUFSZ == dynamic_default_buffer_traits<T>::size
              && BUFALIGN == dynamic_default_buffer_traits<T>::align)
    : p(nullptr) {
    uniform_move_from(x);
  }
  template <size_t SZ, size_t ALGN>
  dynamic &operator =(dynamic<T, SZ, ALGN> &&x)
    requires (!(SZ == BUFSZ && ALGN == BUFALIGN)
              && BUFSZ == dynamic_default_buffer_traits<T>::size
              && BUFALIGN == dynamic_default_buffer_traits<T>::align) {
    reset();
    uniform_move_from(x);
    return *this;
  }

  T &operator *() noexcept {
    return *p;
  }
  T *operator ->() noexcept {
    return p;
  }
  const T &operator *() const noexcept {
    return *p;
  }
  const T *operator ->() const noexcept {
    return p;
  }
  T &value() {
    if (p == nullptr)
      throw_or_terminate<bad_dynamic_access>();
    return *p;
  }
  const T &value() const {
    if (p == nullptr)
      throw_or_terminate<bad_dynamic_access>();
    return *p;
  }

  const type_info &type() const noexcept {
    return (p != nullptr) ? base(p)->re_dynamic_typeid() : typeid(void);
  }
  template <class U>
  bool is() const noexcept {
    return type() == typeid(U);
  }
  template <class U>
  U &as() & noexcept {
    if (type() != typeid(U))
      throw_or_terminate<bad_dynamic_access>();
    return *static_cast<U *>(p);
  }
  template <class U>
  const U &as() const & noexcept {
    if (type() != typeid(U))
      throw_or_terminate<bad_dynamic_access>();
    return *static_cast<add_const_t<U> *>(p);
  }
  template <class U>
  U &&as() && noexcept {
    if (type() != typeid(U))
      throw_or_terminate<bad_dynamic_access>();
    return move(*static_cast<U *>(p));
  }
  template <class U>
  const U &&as() const && noexcept {
    if (type() != typeid(U))
      throw_or_terminate<bad_dynamic_access>();
    return move(*static_cast<add_const_t<U> *>(p));
  }

  bool empty() const noexcept {
    return p == nullptr;
  }
  void clear() noexcept {
    if (p != nullptr) {
      base(p)->re_dynamic_clear();
      p = nullptr;
    }
  }
  explicit operator bool() const noexcept {
    return !empty();
  }
  bool has_value() const noexcept {
    return !empty();
  }
  void reset() noexcept {
    clear();
  }

  bool local() const noexcept {
    return p == nullptr
      || (reinterpret_cast<const byte *>(this)
          <= reinterpret_cast<const byte *>(p)
          && reinterpret_cast<const byte *>(p)
          < (reinterpret_cast<const byte *>(this) + sizeof(this_t)));
  }
  template <class U>
  static constexpr bool local() noexcept {
    return inner::local_dynamic_impl<impl_t<decay_t<U>>>;
  }
  size_t size() const noexcept {
    return p == nullptr ? 0 : base(p)->re_dynamic_size();
  }
  size_t align() const noexcept {
    return p == nullptr ? 0 : base(p)->re_dynamic_align();
  }
  size_t wrapper_size() const noexcept {
    return p == nullptr ? 0 : base(p)->re_dynamic_wrapper_size();
  }
  size_t wrapper_align() const noexcept {
    return p == nullptr ? 0 : base(p)->re_dynamic_wrapper_align();
  }
};
template <class T, size_t SZ, size_t ALIGN>
bool operator ==(const dynamic<T, SZ, ALIGN> &x, nullptr_t) noexcept {
  return x.empty();
}

template <class T>
using dynamic_ptr = dynamic<T, 0>;

}

// dynamic<void>
namespace re {

namespace inner {

class any_wrapper_base : dynamic_object {
public:
  virtual const type_info &type() const noexcept = 0;
  virtual size_t size() const noexcept = 0;
  virtual size_t align() const noexcept = 0;
};
template <class T>
class any_wrapper : public any_wrapper_base {
public:
  T data;

  any_wrapper() : data() {}
  ~any_wrapper() = default;
  any_wrapper(const any_wrapper &) = default;
  any_wrapper &operator =(const any_wrapper &) = delete;
  any_wrapper(any_wrapper &&) = default;
  any_wrapper &operator =(any_wrapper &&) = delete;

  template <class...S>
  any_wrapper(S &&...s) : data(forward<S>(s)...) {}

  virtual const type_info &type() const noexcept override {
    return typeid(T);
  }
  virtual size_t size() const noexcept override {
    return sizeof(T);
  }
  virtual size_t align() const noexcept override {
    return alignof(T);
  }
};

template <class T>
struct is_dynamic_void : false_type {};
template <size_t A, size_t B>
struct is_dynamic_void<dynamic<void, A, B>> : true_type {};

}
template <>
struct dynamic_default_buffer_traits<void>
  : dynamic_default_buffer_traits<void *> {};
template <>
struct dynamic_default_buffer_traits<inner::any_wrapper_base>
  : dynamic_default_buffer_traits<void> {};
template <size_t BUFSZ, size_t ALIGN>
class dynamic<void, BUFSZ, ALIGN> {
  using this_t = dynamic;

  template <class, size_t, size_t>
  friend class dynamic;
  template <class, size_t, size_t>
  friend class function;

  using impl_t = dynamic<inner::any_wrapper_base, BUFSZ, ALIGN>;
  impl_t impl;

public:
  dynamic() = default;
  ~dynamic() = default;
  dynamic(const dynamic &) = default;
  dynamic &operator =(const dynamic &) = default;
  dynamic(dynamic &&) noexcept = default;
  dynamic &operator =(dynamic &&) noexcept = default;
  friend void swap(dynamic &x, dynamic &y) noexcept {
    adl_swap(x.impl, y.impl);
  }

  dynamic(nullptr_t) noexcept : impl(nullptr) {}
  dynamic &operator =(nullptr_t) noexcept {
    impl = nullptr;
    return *this;
  }

  template <class T>
  dynamic(T &&x)
    requires (!is_same_v<decay_t<T>, dynamic>
              && !(BUFSZ == dynamic_default_buffer_traits<void>::size
                   && ALIGN == dynamic_default_buffer_traits<void>::align
                   && inner::is_dynamic_void<decay_t<T>>::value)
              && !is_in_place_type_v<decay_t<T>>
              && is_constructible_v<decay_t<T>, T &&>
              && inner::compatible_type_of_any<decay_t<T>>)
    : impl(in_place_type<inner::any_wrapper<decay_t<T>>>,
           forward<T>(x)) {}
  template <class T>
  dynamic &operator =(T &&x)
    requires (!is_same_v<decay_t<T>, dynamic>
              && !(BUFSZ == dynamic_default_buffer_traits<void>::size
                   && ALIGN == dynamic_default_buffer_traits<void>::align
                   && inner::is_dynamic_void<decay_t<T>>::value)
              && is_constructible_v<decay_t<T>, T &&>
              && inner::compatible_type_of_any<decay_t<T>>) {
    impl.template emplace<inner::any_wrapper<decay_t<T>>>(forward<T>(x));
    return *this;
  }

  template <class T, class...S>
  dynamic(in_place_type_t<T>, S &&...s)
    requires (is_constructible_v<decay_t<T>, S &&...>
              && inner::compatible_type_of_any<decay_t<T>>)
    : impl(in_place_type<inner::any_wrapper<T>>, forward<S>(s)...) {}
  template <class T, class...S>
  void emplace(S &&...s)
    requires (is_constructible_v<decay_t<T>, S &&...>
              && inner::compatible_type_of_any<decay_t<T>>) {
    impl.template emplace<inner::any_wrapper<T>>(forward<S>(s)...);
  }

  template <class T, class U, class...S>
  dynamic(in_place_type_t<T>, initializer_list<U> l, S &&...s)
    requires (is_constructible_v<decay_t<T>, initializer_list<U> &, S &&...>
              && inner::compatible_type_of_any<decay_t<T>>)
    : impl(in_place_type<inner::any_wrapper<T>>, l, forward<S>(s)...) {}
  template <class T, class U, class...S>
  void emplace(initializer_list<U> l, S &&...s)
    requires (is_constructible_v<decay_t<T>, initializer_list<U> &, S &&...>
              && inner::compatible_type_of_any<decay_t<T>>) {
    impl.template emplace<inner::any_wrapper<T>>(l, forward<S>(s)...);
  }

  template <size_t A, size_t B>
  dynamic(const dynamic<void, A, B> &x)
    requires (BUFSZ == dynamic_default_buffer_traits<void>::size
              && ALIGN == dynamic_default_buffer_traits<void>::align
              && !(A == BUFSZ && B == ALIGN)) : impl(x.impl) {}
  template <size_t A, size_t B>
  dynamic &operator =(const dynamic<void, A, B> &x)
    requires (BUFSZ == dynamic_default_buffer_traits<void>::size
              && ALIGN == dynamic_default_buffer_traits<void>::align
              && !(A == BUFSZ && B == ALIGN)) {
    impl = x.impl;
    return *this;
  }

  template <size_t A, size_t B>
  dynamic(dynamic<void, A, B> &&x)
    requires (BUFSZ == dynamic_default_buffer_traits<void>::size
              && ALIGN == dynamic_default_buffer_traits<void>::align
              && !(A == BUFSZ && B == ALIGN))
    : impl(move(x.impl)) {}
  template <size_t A, size_t B>
  dynamic &operator =(dynamic<void, A, B> &&x)
    requires (BUFSZ == dynamic_default_buffer_traits<void>::size
              && ALIGN == dynamic_default_buffer_traits<void>::align
              && !(A == BUFSZ && B == ALIGN)) {
    impl = move(x.impl);
    return *this;
  }

  const type_info &type() const noexcept {
    return impl != nullptr ? impl->type() : typeid(void);
  }
  template <class TT>
  bool is() const noexcept {
    return type() == typeid(TT);
  }

  template <class TT>
  TT &as() & noexcept {
    if (type() != typeid(TT))
      throw_or_terminate<bad_dynamic_access>();
    return static_cast<TT &>
      (impl.template as<inner::any_wrapper<remove_cvref_t<TT>>>().data);
  }
  template <class TT>
  const TT &as() const & noexcept {
    if (type() != typeid(TT))
      throw_or_terminate<bad_dynamic_access>();
    return static_cast<const TT &>
      (impl.template as<inner::any_wrapper<remove_cvref_t<TT>>>().data);
  }
  template <class TT>
  TT &&as() && noexcept {
    if (type() != typeid(TT))
      throw_or_terminate<bad_dynamic_access>();
    return static_cast<TT &&>
      (impl.template as<inner::any_wrapper<remove_cvref_t<TT>>>().data);
  }
  template <class TT>
  const TT &&as() const && noexcept {
    if (type() != typeid(TT))
      throw_or_terminate<bad_dynamic_access>();
    return static_cast<const TT &&>
      (impl.template as<inner::any_wrapper<remove_cvref_t<TT>>>().data);
  }

  bool empty() const noexcept {
    return impl.empty();
  }
  void clear() noexcept {
    impl.clear();
  }
  explicit operator bool() const noexcept {
    return !empty();
  }
  void reset() noexcept {
    clear();
  }

  bool local() const noexcept {
    return impl.local();
  }
  template <class TT>
  static constexpr bool local() noexcept {
    return impl_t::template local<inner::any_wrapper<TT>>();
  }

  size_t size() const noexcept {
    return *this ? impl->size() : 0;
  }
  size_t align() const noexcept {
    return *this ? impl->align() : 0;
  }
  size_t wrapper_size() const noexcept {
    return impl.wrapper_size();
  }
  size_t wrapper_align() const noexcept {
    return impl.wrapper_align();
  }
};

using any = dynamic<>;
struct bad_any_cast : public bad_cast {
  virtual const char *what() const noexcept override {
    return "re::any: cast to a type different from the actual\n";
  }
};
template <class T>
struct fo_any_cast {
  T operator ()(const any &x) const {
    if (!is<T>(x))
      throw_or_terminate<bad_any_cast>();
    return as<T>(x);
  }
  T operator ()(any &x) const {
    if (!is<T>(x))
      throw_or_terminate<bad_any_cast>();
    return as<T>(x);
  }
  T operator ()(any &&x) const {
    if (!is<T>(x))
      throw_or_terminate<bad_any_cast>();
    return as<T>(x);
  }

  add_pointer_t<add_const_t<T>> operator ()(const any *x) const {
    return (x != nullptr && is<T>(*x)) ? addressof(as<T>(*x)) : nullptr;
  }
  add_pointer_t<T> operator ()(any *x) const {
    return (x != nullptr && is<T>(*x)) ? addressof(as<T>(*x)) : nullptr;
  }
};
template <class T>
inline constexpr fo_any_cast<T> any_cast{};

template <class T>
struct fo_make_any {
  template <class...S>
  any operator ()(S &&...s) const {
    return any(in_place_type<T>, forward<S>(s)...);
  }
  template <class U, class...S>
  any operator ()(initializer_list<U> l, S &&...s) const {
    return any(in_place_type<T>, l, forward<S>(s)...);
  }
};
template <class T>
inline constexpr fo_make_any<T> make_any{};

}

// function
namespace re {

class bad_function_call : public exception {
public:
  virtual const char *what() const noexcept override {
    return "try to access empty re::function\n";
  }
};

template <class, size_t, size_t>
class function;
namespace inner {

template <class F>
class fn_caller_base;
template <class R, class...S>
class fn_caller_base<R (S...)> : dynamic_object {
public:
  virtual R call(S...s) const = 0;
  virtual const type_info &type() const noexcept = 0;
  virtual size_t size() const noexcept = 0;
  virtual size_t align() const noexcept = 0;
};

template <class IMPL, class F>
class fn_caller;
template <class IMPL, class R, class...S>
class fn_caller<IMPL, R (S...)> : public fn_caller_base<R (S...)> {
  template <class, size_t, size_t>
  friend class re::function;

  mutable IMPL f;

public:
  fn_caller() = delete;
  fn_caller(const fn_caller &) = default;
  fn_caller &operator =(const fn_caller &) = delete;
  fn_caller(fn_caller &&)
    noexcept(noexcept(is_nothrow_move_constructible_v<IMPL>)) = default;
  fn_caller &operator =(fn_caller &&) = delete;

  template <class T>
  fn_caller(T &&x) : f(forward<T>(x)) {}

  virtual R call(S...s) const override {
    return invoke_r<R>(f, forward<S>(s)...);
  }
  virtual const type_info &type() const noexcept override {
    return typeid(IMPL);
  }
  virtual size_t size() const noexcept override {
    return sizeof(IMPL);
  }
  virtual size_t align() const noexcept override {
    return alignof(IMPL);
  }
};

template <class, class>
struct is_class_function_of : false_type {};
template <class A, size_t B, size_t C>
struct is_class_function_of<function<A, B, C>, A> : true_type {};

}
template <class F,
          size_t BUFSZ = dynamic_default_buffer_traits
          <inner::fn_caller_base<F>>::size,
          size_t ALIGN = dynamic_default_buffer_traits
          <inner::fn_caller_base<F>>::align>
class function;
template <class R, class...S, size_t BUFSZ, size_t ALIGN>
class function<R (S...), BUFSZ, ALIGN> {
  using this_t = function;

  template <class, size_t, size_t>
  friend class dynamic;
  template <class, size_t, size_t>
  friend class function;

  dynamic<inner::fn_caller_base<R (S...)>, BUFSZ, ALIGN> impl;

  using is_default
    = bool_constant<BUFSZ == dynamic_default_buffer_traits
                    <inner::fn_caller_base<R (S...)>>::size
                    && ALIGN == dynamic_default_buffer_traits
                    <inner::fn_caller_base<R (S...)>>::align>;

public:
  using result_type = R;

  function() = default;
  function(const function &) = default;
  function &operator =(const function &) = default;
  function(function &&) noexcept = default;
  function &operator =(function &&) noexcept = default;
  friend void swap(function &x, function &y) noexcept {
    adl_swap(x.impl, y.impl);
  }

  function(nullptr_t) : impl() {}
  function &operator =(nullptr_t) noexcept {
    impl.reset();
    return *this;
  }

  template <class T>
  function(T &&x)
    requires (!is_same_v<decay_t<T>, this_t>
              && is_invocable_r_v<R, add_lvalue_reference_t<decay_t<T>>, S...>
              && !(inner::is_class_function_of<decay_t<T>, R (S...)>::value
                   && is_default::value))
    : impl(in_place_type<inner::fn_caller<decay_t<T>, R (S...)>>,
           forward<T>(x)) {}
  template <class T>
  function &operator =(T &&x)
    requires (!is_same_v<decay_t<T>, this_t>
              && is_invocable_r_v<R, add_lvalue_reference_t<decay_t<T>>, S...>
              && !(inner::is_class_function_of<decay_t<T>, R (S...)>::value
                   && is_default::value)) {
    impl.template emplace<inner::fn_caller<decay_t<T>, R (S...)>
                          >(forward<T>(x));
    return *this;
  }
  template <class T>
  function &operator =(reference_wrapper<T> x) noexcept
    requires (is_invocable_r_v<R, reference_wrapper<T> &, S...>
              && is_default::value) {
    impl.template emplace<inner::fn_caller
                          <reference_wrapper<T>, R (S...)>>(x);
    return *this;
  }

  template <size_t BUFSZ2, size_t ALIGN2>
  function(const function<R (S...), BUFSZ2, ALIGN2> &x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value)
    : impl(x.impl) {}
  template <size_t BUFSZ2, size_t ALIGN2>
  function &operator =(const function<R (S...), BUFSZ2, ALIGN2> &x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value) {
    impl = x.impl;
    return *this;
  }

  template <size_t BUFSZ2, size_t ALIGN2>
  function(function<R (S...), BUFSZ2, ALIGN2> &&x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value)
    : impl(move(x.impl)) {}
  template <size_t BUFSZ2, size_t ALIGN2>
  function &operator =(function<R (S...), BUFSZ2, ALIGN2> &&x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value) {
    impl = move(x.impl);
    return *this;
  }

  R operator ()(S...s) const {
    if (impl.empty())
      throw_or_terminate<bad_function_call>();
    return impl->call(forward<S>(s)...);
  }

  template <class F2, size_t BUFSZ2, size_t ALIGN2>
  friend bool operator ==(const function<F2, BUFSZ2, ALIGN2> &,
                          nullptr_t) noexcept;

  const type_info &type() const noexcept {
    return impl != nullptr ? impl->type() : typeid(void);
  }
  template <class TT>
  bool is() const noexcept {
    return type() == typeid(TT);
  }

  template <class TT>
  TT &as() & noexcept {
    static_assert(is_invocable_r_v
                  <R, add_lvalue_reference_t<decay_t<TT>>, S...>);
    return impl.template as<inner::fn_caller<decay_t<TT>, R (S...)>>().f;
  }
  template <class TT>
  const TT &as() const & noexcept {
    static_assert(is_invocable_r_v
                  <R, add_lvalue_reference_t<decay_t<TT>>, S...>);
    return impl.template as<inner::fn_caller<decay_t<TT>, R (S...)>>().f;
  }
  template <class TT>
  TT &&as() && noexcept {
    static_assert(is_invocable_r_v
                  <R, add_lvalue_reference_t<decay_t<TT>>, S...>);
    return (static_cast<this_t &&>(*this))
      .impl.template as<inner::fn_caller<decay_t<TT>, R (S...)>>().f;
  }
  template <class TT>
  const TT &&as() const && noexcept {
    static_assert(is_invocable_r_v
                  <R, add_lvalue_reference_t<decay_t<TT>>, S...>);
    return (static_cast<const this_t &&>(*this))
      .impl.template as<inner::fn_caller<decay_t<TT>, R (S...)>>().f;
  }

  bool local() const noexcept {
    return impl.local();
  }
  template <class TT>
  static constexpr bool local() noexcept {
    return decltype(impl)::template local<inner::fn_caller<TT, R (S...)>>();
  }

  size_t size() const noexcept {
    return impl != nullptr ? impl->size() : 0;
  }
  size_t align() const noexcept {
    return impl != nullptr ? impl->align() : 0;
  }
  size_t wrapper_size() const noexcept {
    return impl.wrapper_size();
  }
  size_t wrapper_align() const noexcept {
    return impl.wrapper_align();
  }

  bool empty() const noexcept {
    return impl.empty();
  }
  void clear() noexcept {
    impl.clear();
  }
  void reset() noexcept {
    clear();
  }
  explicit operator bool() const noexcept {
    return !empty();
  }

  const type_info &target_type() const noexcept {
    return type();
  }
  template <class T>
  T *target() noexcept {
    if (is<T>())
      return addressof(as<T>());
    else
      return nullptr;
  }
  template <class T>
  const T *target() const noexcept {
    if (is<T>())
      return addressof(as<T>());
    else
      return nullptr;
  }
};
template <class F, size_t BUFSZ, size_t ALIGN>
bool operator ==(const function<F, BUFSZ, ALIGN> &x, nullptr_t) noexcept {
  return x.impl == nullptr;
}

}

// move_only_function
// unique_function
namespace re {

template <class...>
class move_only_function;
namespace inner {

template <class CVREF_T, class F>
class any_fn_caller_base;
template <class CVREF_T, class R, class...S>
class any_fn_caller_base<CVREF_T, R (S...)> : dynamic_object {
public:
  virtual R call(S...s) const = 0;
};

template <class IMPL, class CVREF_T, class F>
class any_fn_caller;
template <class IMPL, class CVREF_T, class R, class...S>
class any_fn_caller<IMPL, CVREF_T, R (S...)>
  : public any_fn_caller_base<CVREF_T, R (S...)> {
  template <class, size_t, size_t>
  friend class re::function;

  mutable IMPL f;

public:
  any_fn_caller() = delete;
  any_fn_caller(const any_fn_caller &x)
    : any_fn_caller(move(const_cast<any_fn_caller &>
                         (((void)throw_or_terminate<logic_error>
                           ("re::inner::any_fn_caller: try to call the copy "
                            "constructor that is designed to be unreachable\n"),
                           x)))) {}
  any_fn_caller &operator =(const any_fn_caller &) = delete;
  any_fn_caller(any_fn_caller &&)
    noexcept(noexcept(is_nothrow_move_constructible_v<IMPL>)) = default;
  any_fn_caller &operator =(any_fn_caller &&) = delete;

  template <class...SS>
  explicit any_fn_caller(in_place_t, SS &&...s) : f(forward<SS>(s)...) {}

  virtual R call(S...s) const override {
    return invoke_r<R>(static_cast<copy_cvref_t<CVREF_T, IMPL>>(f),
                       forward<S>(s)...);
  }
};

template <class F>
class any_fn_helper;
template <class R, class...S>
class any_fn_helper<R (S...)> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = int;
  using cvref_t = int &;
  R operator ()(S...s) {
    using derived_t = move_only_function<R (S...)>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = const int;
  using cvref_t = const int &;
  R operator ()(S...s) const {
    using derived_t = move_only_function<R (S...) const>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) &> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = int &;
  using cvref_t = int &;
  R operator ()(S...s) & {
    using derived_t = move_only_function<R (S...) &>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const &> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = const int &;
  using cvref_t = const int &;
  R operator ()(S...s) const & {
    using derived_t = move_only_function<R (S...) const &>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) &&> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = int &&;
  using cvref_t = int &&;
  R operator ()(S...s) && {
    using derived_t = move_only_function<R (S...) &&>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const &&> {
protected:
  using f_t = R (S...);
  using is_noexcept = false_type;
  using result_type = R;
  using original_cvref_t = const int &&;
  using cvref_t = const int &&;
  R operator ()(S...s) const && {
    using derived_t = move_only_function<R (S...) const &&>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = int;
  using cvref_t = int &;
  R operator ()(S...s) noexcept {
    using derived_t = move_only_function<R (S...) noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = const int;
  using cvref_t = const int &;
  R operator ()(S...s) const noexcept {
    using derived_t = move_only_function<R (S...) const noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) & noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = int &;
  using cvref_t = int &;
  R operator ()(S...s) & noexcept {
    using derived_t = move_only_function<R (S...) & noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const & noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = const int &;
  using cvref_t = const int &;
  R operator ()(S...s) const & noexcept {
    using derived_t = move_only_function<R (S...) const & noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) && noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = int &&;
  using cvref_t = int &&;
  R operator ()(S...s) && noexcept {
    using derived_t = move_only_function<R (S...) && noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};
template <class R, class...S>
class any_fn_helper<R (S...) const && noexcept> {
protected:
  using f_t = R (S...);
  using is_noexcept = true_type;
  using result_type = R;
  using original_cvref_t = const int &&;
  using cvref_t = const int &&;
  R operator ()(S...s) const && noexcept {
    using derived_t = move_only_function<R (S...) const && noexcept>;
    return static_cast<const derived_t *>(this)->impl->call(forward<S>(s)...);
  }
};

template <class, class>
struct f_t_is_nothrow_invocable;
template <class R, class...S, class T>
struct f_t_is_nothrow_invocable<R (S...), T>
  : is_nothrow_invocable_r<R, T, S...> {};

template <class, class>
struct f_t_is_invocable;
template <class R, class...S, class T>
struct f_t_is_invocable<R (S...), T> : is_invocable_r<R, T, S...> {};

}
template <class F>
class move_only_function<F> : public inner::any_fn_helper<F> {
  template <class>
  friend class inner::any_fn_helper;

  using this_t = move_only_function;
  using base_t = inner::any_fn_helper<F>;
  using original_cvref_t = typename base_t::original_cvref_t;
  using cvref_t = typename base_t::cvref_t;
  using f_t = typename base_t::f_t;

  template <class VT>
  static constexpr bool is_callable_from
    = base_t::is_noexcept::value
    ? (inner::f_t_is_nothrow_invocable
       <f_t, copy_cvref_t<original_cvref_t, VT>>::value
       && inner::f_t_is_nothrow_invocable
       <f_t, copy_cvref_t<cvref_t, VT>>::value)
    : (inner::f_t_is_invocable<f_t, copy_cvref_t<original_cvref_t, VT>>::value
       && inner::f_t_is_invocable<f_t, copy_cvref_t<cvref_t, VT>>::value);

  dynamic<inner::any_fn_caller_base<cvref_t, f_t>> impl;

public:
  using base_t::result_type;
  using base_t::operator ();

  move_only_function() noexcept = default;
  ~move_only_function() = default;
  move_only_function(const this_t &) = delete;
  move_only_function &operator =(const this_t &) = delete;
  move_only_function(this_t &&) noexcept = default;
  move_only_function &operator =(this_t &&) = default;
  friend void swap(move_only_function &x, move_only_function &y) noexcept {
    adl_swap(x.impl, y.impl);
  }

  move_only_function(nullptr_t) noexcept {}
  move_only_function &operator =(nullptr_t) noexcept {
    impl.reset();
    return *this;
  }
  friend bool operator ==(const move_only_function &x, nullptr_t) noexcept {
    return x.impl.empty();
  }

  template <class T, class...S>
  explicit move_only_function(in_place_type_t<T>, S &&...s)
    requires (is_constructible_v<decay_t<T>, S...>
              && is_callable_from<decay_t<T>>)
    : impl(in_place_type<inner::any_fn_caller<decay_t<T>, cvref_t, f_t>>,
           in_place, forward<S>(s)...) {}
  template <class T, class U, class...S>
  explicit move_only_function(in_place_type_t<T>,
                              initializer_list<U> l, S &&...s)
    requires (is_constructible_v<decay_t<T>, initializer_list<U> &, S...>
              && is_callable_from<decay_t<T>>)
    : impl(in_place_type<inner::any_fn_caller<decay_t<T>, cvref_t, f_t>>,
           in_place, l, forward<S>(s)...) {}

  template <class T>
  move_only_function(T &&x)
    requires (!is_same_v<remove_cvref_t<T>, this_t>
              && !is_in_place_type_v<remove_cvref_t<T>>
              && is_callable_from<decay_t<T>>)
    : impl(in_place_type<inner::any_fn_caller<decay_t<T>, cvref_t, f_t>>,
           in_place, forward<T>(x)) {}
  template <class T>
  move_only_function &operator =(T &&x)
    requires (!is_same_v<remove_cvref_t<T>, this_t>
              && !is_in_place_type_v<remove_cvref_t<T>>
              && is_callable_from<decay_t<T>>) {
    impl.template emplace<inner::any_fn_caller<decay_t<T>, cvref_t, f_t>
                          >(in_place, forward<T>(x));
    return *this;
  }

  explicit operator bool() const noexcept {
    return impl.operator bool();
  }
};

template <class, size_t, size_t>
class unique_function;
namespace inner {

template <class F>
class unique_fn_caller_base;
template <class R, class...S>
class unique_fn_caller_base<R (S...)> : dynamic_object {
public:
  virtual R call(S...s) const = 0;
};

template <class IMPL, class F>
class unique_fn_caller;
template <class IMPL, class R, class...S>
class unique_fn_caller<IMPL, R (S...)>
  : public unique_fn_caller_base<R (S...)> {
  template <class, size_t, size_t>
  friend class re::unique_function;

  mutable IMPL f;

public:
  unique_fn_caller() = delete;
  unique_fn_caller(const unique_fn_caller &x)
    : unique_fn_caller(move(const_cast<unique_fn_caller &>
                         (((void)throw_or_terminate<logic_error>
                           ("re::inner::unique_fn_caller: try to call the copy "
                            "constructor that is designed to be unreachable\n"),
                           x)))) {}
  unique_fn_caller &operator =(const unique_fn_caller &) = delete;
  unique_fn_caller(unique_fn_caller &&)
    noexcept(noexcept(is_nothrow_move_constructible_v<IMPL>)) = default;
  unique_fn_caller &operator =(unique_fn_caller &&) = delete;

  template <class...SS>
  explicit unique_fn_caller(in_place_t, SS &&...s) : f(forward<SS>(s)...) {}

  virtual R call(S...s) const override {
    return invoke_r<R>(f, forward<S>(s)...);
  }
};

template <class, class>
struct is_class_unique_function_of : false_type {};
template <class A, size_t B, size_t C>
struct is_class_unique_function_of<unique_function<A, B, C>, A> : true_type {};

}
template <class F,
          size_t BUFSZ = dynamic_default_buffer_traits
          <inner::unique_fn_caller_base<F>>::size,
          size_t ALIGN = dynamic_default_buffer_traits
          <inner::unique_fn_caller_base<F>>::align>
class unique_function;
template <class R, class...S, size_t BUFSZ, size_t ALIGN>
class unique_function<R (S...), BUFSZ, ALIGN> {
  using this_t = unique_function;

  template <class, size_t, size_t>
  friend class unique_function;

  dynamic<inner::unique_fn_caller_base<R (S...)>, BUFSZ, ALIGN> impl;

  using is_default
    = bool_constant<BUFSZ == dynamic_default_buffer_traits
                    <inner::unique_fn_caller_base<R (S...)>>::size
                    && ALIGN == dynamic_default_buffer_traits
                    <inner::unique_fn_caller_base<R (S...)>>::align>;

public:
  unique_function() noexcept = default;
  ~unique_function() = default;
  unique_function(const this_t &) = delete;
  unique_function &operator =(const this_t &) = delete;
  unique_function(this_t &&) noexcept = default;
  unique_function &operator =(this_t &&) = default;
  friend void swap(unique_function &x, unique_function &y) noexcept {
    adl_swap(x.impl, y.impl);
  }

  unique_function(nullptr_t) noexcept {}
  unique_function &operator =(nullptr_t) noexcept {
    impl.reset();
    return *this;
  }
  bool operator ==(nullptr_t) noexcept {
    return impl.empty();
  }
  void reset() noexcept {
    impl.reset();
  }
  void clear() noexcept {
    impl.clear();
  }
  bool empty() const noexcept {
    return impl.empty();
  }
  explicit operator bool() const noexcept {
    return impl.operator bool();
  }

  template <class T>
  unique_function(T &&x)
    requires (!is_same_v<remove_cvref_t<T>, this_t>
              && !is_in_place_type_v<remove_cvref_t<T>>
              && is_invocable_r_v<R, decay_t<T> &, S...>
              && !(inner::is_class_unique_function_of
                   <decay_t<T>, R (S...)>::value
                   && is_default::value))
    : impl(in_place_type<inner::unique_fn_caller<decay_t<T>, R (S...)>>,
           in_place, forward<T>(x)) {}
  template <class T>
  unique_function &operator =(T &&x)
    requires (!is_same_v<remove_cvref_t<T>, this_t>
              && !is_in_place_type_v<remove_cvref_t<T>>
              && is_invocable_r_v<R, decay_t<T> &, S...>
              && !(inner::is_class_unique_function_of
                   <decay_t<T>, R (S...)>::value
                   && is_default::value)) {
    impl.template emplace<inner::unique_fn_caller<decay_t<T>, R (S...)>
                          >(in_place, forward<T>(x));
    return *this;
  }

  template <size_t BUFSZ2, size_t ALIGN2>
  unique_function(unique_function<R (S...), BUFSZ2, ALIGN2> &&x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value)
    : impl(move(x.impl)) {}
  template <size_t BUFSZ2, size_t ALIGN2>
  unique_function &operator =(unique_function<R (S...), BUFSZ2, ALIGN2> &&x)
    requires (!(BUFSZ2 == BUFSZ && ALIGN2 == ALIGN) && is_default::value) {
    impl = move(x.impl);
    return *this;
  }

  R operator ()(S...s) const {
    return impl->call(forward<S>(s)...);
  }

  bool local() const noexcept {
    return impl.local();
  }
  template <class TT>
  static constexpr bool local() noexcept {
    return decltype(impl)::template local<inner::unique_fn_caller
                                          <decay_t<TT>, R (S...)>>();
  }
};

}

// type_erased_invocation
namespace re {

template <class BASE_PTR>
struct fo_do_type_erased_invocation {
  template <class FP, class X, class...S>
  decltype(auto) operator ()(FP fp, X &&f, S &&...s) const {
    return fp(static_cast<const void *>(addressof(f)),
              static_cast<BASE_PTR>(addressof(s))...);
  }
};
template <class BASE_PTR>
inline constexpr
fo_do_type_erased_invocation<BASE_PTR> do_type_erased_invocation{};

namespace inner {

template <class F>
struct all_type_packs_is_invocable {
  template <class...P>
  struct impl
    : conjunction<type_pack_apply_t<is_invocable, type_pack_add<F, P>>...> {};
};

}
template <class F, class...TYPE_PACKS>
struct is_invocable_for_all_combinations
  : type_pack_apply_t
  <inner::all_type_packs_is_invocable<F>::template impl,
   compile_time_acc_t<type_pack_mul, TYPE_PACKS...>>::type {};
template <class F>
struct is_invocable_for_all_combinations<F> : is_invocable<F> {};
template <class F, class...TYPE_PACKS>
inline constexpr bool is_invocable_for_all_combinations_v
  = is_invocable_for_all_combinations<F, TYPE_PACKS...>::value;

namespace inner {

template <class F, bool>
struct get_common_type_from_all_invocations {
  template <class...P>
  struct impl : common_type<typename type_pack_apply_t
                            <invoke_result, type_pack_add<F, P>>::type...> {};
};
template <class F>
struct get_common_type_from_all_invocations<F, false> {
  template <class...>
  struct impl {};
};

}
template <class F, class...TYPE_PACKS>
struct invoke_result_for_all_combinations
  : type_pack_apply_t
  <inner::get_common_type_from_all_invocations
   <F, is_invocable_for_all_combinations_v<F, TYPE_PACKS...>>::template impl,
   compile_time_acc_t<type_pack_mul, TYPE_PACKS...>> {};
template <class F, class...TYPE_PACKS>
using invoke_result_for_all_combinations_t
  = typename invoke_result_for_all_combinations<F, TYPE_PACKS...>::type;

namespace inner {

template <class BASE_PTR, class R, class F, class...S>
struct gen_type_erased_invocation {
  using fp = R (*)(const void *, type_t<BASE_PTR, S>...);
  static constexpr fp get() {
    return [](const void *x, type_t<BASE_PTR, S>...s)->R {
      return invoke(static_cast<F &&>
                    (*const_cast<remove_reference_t<F> *>
                     (static_cast<add_const_t<remove_reference_t<F>> *>(x))),
                    static_cast<S &&>
                    (*const_cast<remove_reference_t<S> *>
                     (static_cast<add_const_t<remove_reference_t<S>> *>(s)))
                    ...);
    };
  }
};
namespace fns {

template <class BASE_PTR, class PACKS, class FP, class R, class F, size_t...I>
const FP *gen_type_erased_invocation_array(index_sequence<I...>) {
  static constexpr FP fns[type_pack_size_v<PACKS>]
    = {type_pack_apply_t
       <gen_type_erased_invocation,
        type_pack_add<type_pack<BASE_PTR, R, F>,
                      type_pack_element_t<I, PACKS>>>::get()...};
  return fns;
}

}

}
template <class BASE_PTR, class R, class FO, class...TYPE_PACKS>
struct fo_type_erased_invocation_array {
  constexpr R (*const *operator ()() const)
  (const void *, type_t<BASE_PTR, TYPE_PACKS>...) {
    using PACKS = compile_time_acc_t<type_pack_mul, TYPE_PACKS...>;
    return inner::fns::gen_type_erased_invocation_array
      <BASE_PTR, PACKS,
       R (*)(const void *, type_t<BASE_PTR, TYPE_PACKS>...), R, FO>
      (make_index_sequence<type_pack_size_v<PACKS>>());
  }
};
template <class BASE_PTR, class R, class F0, class...TYPE_PACKS>
inline constexpr
fo_type_erased_invocation_array<BASE_PTR, R, F0, TYPE_PACKS...>
type_erased_invocation_array{};

template <size_t I, class...S>
struct base_of_type_packs : size_constant<0> {};
template <size_t I, class X, class...S>
struct base_of_type_packs<I, X, S...>
  : base_of_type_packs<I - 1, S...> {};
template <class X, class...S>
struct base_of_type_packs<0, X, S...>
  : compile_time_acc<compile_time_mul, size_constant<1>,
                     typename type_pack_size<S>::type...>::type::type {};
template <size_t I, class...S>
inline constexpr size_t base_of_type_packs_v
  = base_of_type_packs<I, S...>::value;

namespace inner::fns {

template <class...TYPE_PACKS, size_t...I>
size_t get_type_erased_invocation_array_index
(type_t<size_t, TYPE_PACKS>...id, index_sequence<I...>) {
  return accumulate_args(plus<size_t>(),
                         (base_of_type_packs_v<I, TYPE_PACKS...> * id)...);
}

}
template <class...TYPE_PACKS>
struct fo_type_erased_invocation_index {
  size_t operator ()(type_t<size_t, TYPE_PACKS>...id) const noexcept {
    return inner::fns::get_type_erased_invocation_array_index<TYPE_PACKS...>
      (id..., make_index_sequence<sizeof...(TYPE_PACKS)>());
  }
};
template <class...TYPE_PACKS>
inline constexpr
fo_type_erased_invocation_index<TYPE_PACKS...>
type_erased_invocation_index{};

}

// variant
namespace re {

class bad_variant_access : public exception {
public:
  virtual const char *what() const noexcept override {
    return "re::variant: no specified type stored\n";
  }
};
template <class...>
class variant;

template <class T>
struct variant_size;
template <class T>
struct variant_size<const T> : variant_size<T> {};
template <class T>
struct variant_size<volatile T> : variant_size<T> {};
template <class T>
struct variant_size<const volatile T> : variant_size<T> {};
template <class...S>
struct variant_size<variant<S...>> : size_constant<sizeof...(S)> {};
template <class T>
inline constexpr size_t variant_size_v = variant_size<T>::value;

template <size_t I, class T>
struct variant_alternative;
template <size_t I, class T>
using variant_alternative_t = typename variant_alternative<I, T>::type;
template <size_t I, class T>
struct variant_alternative<I, const T> : variant_alternative<I, T> {};
template <size_t I, class T>
struct variant_alternative<I, volatile T> : variant_alternative<I, T> {};
template <size_t I, class T>
struct variant_alternative<I, const volatile T> : variant_alternative<I, T> {};
template <size_t I, class...S>
struct variant_alternative<I, variant<S...>> : nth_type<I, S...> {};

template <class T>
struct fo_holds_alternative {
  template <class...S>
  constexpr bool operator ()(const variant<S...> &x) const noexcept {
    static_assert(occurs_exactly_once_v<T, S...>);
    return find_type_v<T, S...> == x.index();
  }
};
template <class T>
inline constexpr fo_holds_alternative<T> holds_alternative{};

template <size_t I, class...S>
constexpr variant_alternative_t<I, variant<S...>> &
get(variant<S...> &x) requires (I < sizeof...(S)) {
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return x.template at<I>();
}
template <size_t I, class...S>
constexpr const variant_alternative_t<I, variant<S...>> &
get(const variant<S...> &x) requires (I < sizeof...(S)) {
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return x.template at<I>();
}
template <size_t I, class...S>
constexpr variant_alternative_t<I, variant<S...>> &&
get(variant<S...> &&x) requires (I < sizeof...(S)) {
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return move(x).template at<I>();
}
template <size_t I, class...S>
constexpr const variant_alternative_t<I, variant<S...>> &&
get(const variant<S...> &&x) requires (I < sizeof...(S)) {
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return move(x).template at<I>();
}

template <class T, class...S>
constexpr T &get(variant<S...> &x)
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return x.template at<I>();
}
template <class T, class...S>
constexpr const T &get(const variant<S...> &x)
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return x.template at<I>();
}
template <class T, class...S>
constexpr T &&get(variant<S...> &&x)
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return move(x).template at<I>();
}
template <class T, class...S>
constexpr const T &&get(const variant<S...> &&x)
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (x.index() != I)
    throw_or_terminate<bad_variant_access>();
  return move(x).template at<I>();
}

template <size_t I, class...S>
constexpr add_pointer_t<variant_alternative_t<I, variant<S...>>>
get_if(variant<S...> *p) noexcept requires (I < sizeof...(S)) {
  static_assert(I < sizeof...(S));
  if (p != nullptr && p->index() == I)
    return addressof(p->template at<I>());
  else
    return nullptr;
}
template <size_t I, class...S>
constexpr add_pointer_t<const variant_alternative_t<I, variant<S...>>>
get_if(const variant<S...> *p) noexcept requires (I < sizeof...(S)) {
  static_assert(I < sizeof...(S));
  if (p != nullptr && p->index() == I)
    return addressof(p->template at<I>());
  else
    return nullptr;
}

template <class T, class...S>
constexpr add_pointer_t<T> get_if(variant<S...> *p) noexcept
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (p != nullptr && p->index() == I)
    return addressof(p->template at<I>());
  else
    return nullptr;
}
template <class T, class...S>
constexpr add_pointer_t<const T> get_if(const variant<S...> *p) noexcept
  requires occurs_exactly_once_v<T, S...> {
  constexpr size_t I = find_type_v<T, S...>;
  if (p != nullptr && p->index() == I)
    return addressof(p->template at<I>());
  else
    return nullptr;
}

struct monostate {};
inline constexpr bool operator ==(monostate, monostate) noexcept {
  return true;
}
inline constexpr strong_ordering operator <=>(monostate,
                                              monostate) noexcept {
  return strong_eq;
}

namespace inner {

template <class T>
struct variant_to_type_pack {
  using type = T;
};
template <class...S>
struct variant_to_type_pack<variant<S...> &> {
  using type = type_pack<S &...>;
};
template <class...S>
struct variant_to_type_pack<const variant<S...> &> {
  using type = type_pack<const S &...>;
};
template <class...S>
struct variant_to_type_pack<variant<S...> &&> {
  using type = type_pack<S &&...>;
};
template <class...S>
struct variant_to_type_pack<const variant<S...> &&> {
  using type = type_pack<const S &&...>;
};
template <class T>
using variant_to_type_pack_t = typename variant_to_type_pack<T>::type;

template <class F>
struct variant_applying_wrapper {
  template <class...S>
  auto operator ()(S &&...s) const->decltype(F::apply(forward<S>(s)...));
};

template <class VISITOR, class...VARIANTS>
struct variant_visit_result {
  using type = invoke_result_t
    <VISITOR, decltype(at<0>(declval<VARIANTS>()))...>;
};

}
struct fo_visit {
  template <class VISITOR, class...VARIANTS>
  typename inner::variant_visit_result<VISITOR, VARIANTS...>::type
  operator ()(VISITOR &&f, VARIANTS &&...s) const {
    const auto get_id = [](auto &x) {
      if (x.index()
          == type_pack_size_v<inner::variant_to_type_pack_t<decltype(x)>>)
        throw_or_terminate<bad_variant_access>();
      return x.index();
    };

    const auto *fps = type_erased_invocation_array
      <const void *,
       typename inner::variant_visit_result<VISITOR, VARIANTS...>::type,
       VISITOR &&,
       inner::variant_to_type_pack_t<VARIANTS &&>...>();
    const size_t n = type_erased_invocation_index
      <inner::variant_to_type_pack_t<VARIANTS &&>...>(get_id(s)...);

    return do_type_erased_invocation<const void *>(fps[n], f, s.buf...);
  }
};
inline constexpr fo_visit visit{};
template <class R>
struct fo_visit_r {
  template <class VISITOR, class...VARIANTS>
  R operator ()(VISITOR &&f, VARIANTS &&...s) const
    requires is_invocable_for_all_combinations_v
    <VISITOR, inner::variant_to_type_pack_t<VARIANTS>...> {
    const auto get_id = [](auto &x) {
      if (x.index()
          == type_pack_size_v<inner::variant_to_type_pack_t<decltype(x)>>)
        throw_or_terminate<bad_variant_access>();
      return x.index();
    };

    const auto *fps = type_erased_invocation_array
      <const void *, R, VISITOR &&,
       inner::variant_to_type_pack_t<VARIANTS &&>...>();
    const size_t n = type_erased_invocation_index
      <inner::variant_to_type_pack_t<VARIANTS &&>...>(get_id(s)...);

    return do_type_erased_invocation<const void *>(fps[n], f, s.buf...);
  }
};
template <class R>
inline constexpr fo_visit_r<R> visit_r{};

namespace inner {

template <class T, class...S>
union variant_storage {
  T data;
  variant_storage<S...> following;

  constexpr variant_storage() {}
  constexpr ~variant_storage() {}
  ~variant_storage()
    requires (is_trivially_destructible_v<T>
              && (is_trivially_destructible_v<S> && ...)) = default;
  variant_storage(const variant_storage &) = default;
  variant_storage &operator =(const variant_storage &) = default;
  variant_storage(variant_storage &&) = default;
  variant_storage &operator =(variant_storage &&) = default;

  template <class...SS>
  constexpr variant_storage(in_place_index_t<0>, SS &&...s)
    : data(forward<SS>(s)...) {}
  template <size_t I, class...SS>
  constexpr variant_storage(in_place_index_t<I>, SS &&...s)
    : following(in_place_index<I - 1>, forward<SS>(s)...) {}
};
template <class T>
union variant_storage<T> {
  T data;

  constexpr variant_storage() noexcept {}
  constexpr ~variant_storage() {}
  ~variant_storage() requires is_trivially_destructible_v<T> = default;
  variant_storage(const variant_storage &) = default;
  variant_storage &operator =(const variant_storage &) = default;
  variant_storage(variant_storage &&) = default;
  variant_storage &operator =(variant_storage &&) = default;

  template <class...SS>
  constexpr variant_storage(in_place_index_t<0>, SS &&...s)
    : data(forward<SS>(s)...) {}
};

template <class...>
class variant_imaginary_function;
template <class X>
class variant_imaginary_function<X> {
public:
  static X f(X);
};
template <class X, class...S>
class variant_imaginary_function<X, S...>
  : variant_imaginary_function<S...> {
public:
  using variant_imaginary_function<S...>::f;
  static X f(X);
};

}
namespace inner::fns {

template <class T>
constexpr void (*make_variant_destroy_fp() noexcept)(void *) {
  return [](void *p) {
    static_cast<T *>(p)->~T();
  };
}
template <class T>
constexpr void
(*make_variant_copy_ctor_fp() noexcept)(void *, const void *) {
  return [](void *l, const void *r) {
    ::new(l) T(*static_cast<add_const_t<T> *>(r));
  };
}
template <class T>
constexpr void
(*make_variant_move_ctor_fp() noexcept)(void *, void *) {
  return [](void *l, void *r) {
    ::new(l) T(move(*static_cast<T *>(r)));
  };
}
template <class T>
constexpr void
(*make_variant_copy_assignment_fp() noexcept)(void *l, const void *r) {
  return [](void *l, const void *r) {
    *static_cast<T *>(l) = *static_cast<add_const_t<T> *>(r);
  };
}
template <class T>
constexpr void
(*make_variant_move_assignment_fp() noexcept)(void *l, void *r) {
  return [](void *l, void *r) {
    *static_cast<T *>(l) = move(*static_cast<T *>(r));
  };
}
template <class T>
constexpr void (*make_variant_swap_fp() noexcept)(void *, void *) {
  return [](void *l, void *r) {
    adl_swap(*static_cast<T *>(l), *static_cast<T *>(r));
  };
}

template <class T>
constexpr bool
(*make_variant_equal_fp() noexcept)(const void *, const void *) {
  return [](const void *l, const void *r) {
    return *static_cast<const T *>(l) == *static_cast<const T *>(r);
  };
};
template <class T, class O>
constexpr O (*make_variant_3way_fp() noexcept)
(const void *, const void *) {
  return [](const void *l, const void *r)->O {
    return synth_3way(*static_cast<const T *>(l), *static_cast<const T *>(r));
  };
};

}
template <class...S>
class variant {
  friend struct fo_visit;
  template <class>
  friend struct fo_visit_r;

  template <class...>
  friend class variant;

  using this_t = variant;

  inner::variant_storage<S...> buf;
  size_t i;

  void *void_ptr() noexcept {
    return static_cast<void *>(addressof(buf));
  }
  const void *void_ptr() const noexcept {
    return static_cast<const void *>(addressof(buf));
  }

  static void destroy(size_t id, void *l) noexcept {
    static constexpr void (*const fps[sizeof...(S) + 1])(void *)
      = {inner::fns::make_variant_destroy_fp<S>()..., [](void *) {}};
    fps[id](l);
  }
  static void copy_construct(size_t id, void *l, const void *r) {
    static constexpr void (*fps[sizeof...(S) + 1])(void *, const void *)
      = {inner::fns::make_variant_copy_ctor_fp<S>()...,
         [](void *, const void *) {}};
    fps[id](l, r);
  }
  static void move_construct(size_t id, void *l, void *r) {
    static constexpr void (*fps[sizeof...(S) + 1])(void *, void *)
      = {inner::fns::make_variant_move_ctor_fp<S>()...,
         [](void *, void *) {}};
    fps[id](l, r);
  }
  static void copy_assign(size_t id, void *l, const void *r) {
    static constexpr void (*fps[sizeof...(S) + 1])(void *, const void *)
      = {inner::fns::make_variant_copy_assignment_fp<S>()...,
         [](void *, const void *) {}};
    fps[id](l, r);
  }
  static void move_assign(size_t id, void *l, void *r) {
    static constexpr void (*fps[sizeof...(S) + 1])(void *, void *)
      = {inner::fns::make_variant_move_assignment_fp<S>()...,
         [](void *, void *) {}};
    fps[id](l, r);
  }
  static void swap(size_t id, void *l, void *r) {
    static constexpr void (*fps[sizeof...(S) + 1])(void *, void *)
      = {inner::fns::make_variant_swap_fp<S>()..., [](void *, void *) {}};
    fps[id](l, r);
  }

public:
  variant() = delete;
  ~variant() = delete;
  variant(const variant &) = delete;
  variant &operator =(const variant &) = delete;
  variant(variant &&) = delete;
  variant &operator =(variant &&) = delete;
  constexpr variant() noexcept(is_nothrow_constructible_v<nth_type_t<0, S...>>)
    requires is_constructible_v<nth_type_t<0, S...>>
    : buf(in_place_index<0>), i(0) {}
  ~variant() requires ((is_destructible_v<S> && ...)
                       && (is_trivially_destructible_v<S> && ...)) = default;
  ~variant() requires ((is_destructible_v<S> && ...)
                       && !(is_trivially_destructible_v<S> && ...)) {
    destroy(i, void_ptr());
  }
  variant(const variant &)
    requires ((is_copy_constructible_v<S> && ...)
              && (is_trivially_copy_constructible_v<S> && ...)) = default;
  variant(const variant &x)
    noexcept((is_nothrow_copy_constructible_v<S> && ...))
    requires ((is_copy_constructible_v<S> && ...)
              && !(is_trivially_copy_constructible_v<S> && ...)) : i(x.i) {
    copy_construct(i, void_ptr(), x.void_ptr());
  }
  variant &operator =(const variant &)
    requires (((is_copy_constructible_v<S> && is_copy_assignable_v<S>) && ...)
              && (is_trivially_copy_constructible_v<S> && ...)) = default;
  variant &operator =(const variant &x)
    noexcept(((is_nothrow_copy_constructible_v<S>
               && is_nothrow_copy_assignable_v<S>) && ...))
    requires (((is_copy_constructible_v<S> && is_copy_assignable_v<S>) && ...)
              && !(is_trivially_copy_constructible_v<S> && ...)) {
    if (i != x.i) {
      variant tmp(x);
      destroy(i, void_ptr());
      i = sizeof...(S);
      move_construct(x.i, void_ptr(), tmp.void_ptr());
      i = x.i;
    }
    else
      copy_assign(x.i, void_ptr(), x.void_ptr());
    return *this;
  }
  variant(variant &&)
    requires ((is_move_constructible_v<S> && ...)
              && (is_trivially_move_constructible_v<S> && ...)) = default;
  variant(variant &&x)
    noexcept((is_nothrow_move_constructible_v<S> && ...))
    requires ((is_move_constructible_v<S> && ...)
              && !(is_trivially_move_constructible_v<S> && ...)) : i(x.i) {
    move_construct(i, void_ptr(), x.void_ptr());
  }
  variant &operator =(variant &&)
    requires (((is_move_constructible_v<S> && is_move_assignable_v<S>) && ...)
              && (is_trivially_move_constructible_v<S> && ...)) = default;
  variant &operator =(variant &&x)
    noexcept(((is_nothrow_move_constructible_v<S>
               && is_nothrow_move_assignable_v<S>) && ...))
    requires (((is_move_constructible_v<S> && is_move_assignable_v<S>) && ...)
              && !(is_trivially_move_constructible_v<S> && ...)) {
    if (i != x.i) {
      destroy(i, void_ptr());
      i = sizeof...(S);
      move_construct(x.i, void_ptr(), x.void_ptr());
      i = x.i;
    }
    else
      move_assign(x.i, void_ptr(), x.void_ptr());
    return *this;
  }
  friend void swap(this_t &x, this_t &y)
    noexcept (((is_nothrow_move_constructible_v<S>
                && is_nothrow_move_assignable_v<S>) && ...))
    requires ((is_move_constructible_v<S> && ...)
              && (is_swappable_v<S> && ...)) {
    if constexpr (is_trivially_move_constructible_v<this_t>
                  && is_trivially_move_assignable_v<this_t>) {
      default_swap(x, y);
    }
    else {
      if (x.i != y.i)
        default_swap(x, y);
      else
        swap(x.i, x.void_ptr(), y.void_ptr());
    }
  }

  template <class T>
  constexpr explicit variant(in_place_type_t<T>) noexcept
    requires is_void_v<T>
    : buf()
    , i(sizeof...(S)) {}
  template <class T, class...SS>
  constexpr explicit variant(in_place_type_t<T>, SS &&...s)
    requires (!is_void_v<T>
              && occurs_exactly_once_v<T, S...>
              && is_constructible_v<T, SS &&...>)
    : buf(in_place_index<find_type_v<T, S...>>, forward<SS>(s)...)
    , i(find_type_v<T, S...>) {}
  template <class T, class U, class...SS>
  constexpr explicit variant(in_place_type_t<T>,
                             initializer_list<U> l, SS &&...s)
    requires (!is_void_v<T>
              && occurs_exactly_once_v<T, S...>
              && is_constructible_v<T, initializer_list<U> &, SS &&...>)
    : buf(in_place_index<find_type_v<T, S...>>, l, forward<SS>(s)...)
    , i(find_type_v<T, S...>) {}

  template <size_t I, class...SS>
  constexpr explicit variant(in_place_index_t<I>, SS &&...s)
    requires ((I < sizeof...(S))
              && is_constructible_v<nth_type_t<I, S...>, SS &&...>)
    : buf(in_place_index<I>, forward<SS>(s)...), i(I) {}
  template <size_t I, class U, class...SS>
  constexpr explicit variant(in_place_index_t<I>,
                             initializer_list<U> l, SS &&...s)
    requires ((I < sizeof...(S))
              && is_constructible_v
              <nth_type_t<I, S...>, initializer_list<U> &, SS &&...>)
    : buf(in_place_index<I>, l, forward<SS>(s)...), i(I) {}

  template <class T,
            class U = decltype(inner::variant_imaginary_function<S...>::f
                               (declval<T>()))>
  constexpr variant(T &&x) noexcept(is_nothrow_constructible_v<U, T>)
    requires (!is_same_v<decay_t<T>, variant>
              && !is_in_place_type_v<decay_t<U>>
              && !is_in_place_index_v<decay_t<U>>
              && is_constructible_v<U, T>)
    : buf(in_place_index<find_type_v<U, S...>>, forward<T>(x))
    , i(find_type_v<U, S...>) {}
  template <class T,
            class U = decltype(inner::variant_imaginary_function<S...>::f
                               (declval<T>()))>
  variant &operator =(T &&x)
    noexcept(is_nothrow_assignable_v<U &, T>
             && is_nothrow_constructible_v<U, T>)
    requires (!is_same_v<decay_t<T>, variant>
              && is_assignable_v<U &, T> && is_constructible_v<U &, T>) {
    constexpr size_t x_i = find_type_v<U, S...>;
    if (i == x_i)
      *static_cast<U *>(void_ptr()) = forward<T>(x);
    else {
      U tmp(forward<T>(x));
      destroy(i, void_ptr());
      i = sizeof...(S);
      ::new(void_ptr()) U(move(tmp));
      i = x_i;
    }
    return *this;
  }

private:
  template <size_t I, class...SS, class T = nth_type_t<I, S...>>
  T &emplace_impl(SS &&...s) {
    destroy(i, void_ptr());
    i = sizeof...(S);
    ::new(void_ptr()) T(forward<SS>(s)...);
    i = I;
    return *static_cast<T *>(void_ptr());
  }
public:
  template <class T>
  void emplace() noexcept requires is_void_v<T> {
    destroy(i, void_ptr());
    i = sizeof...(S);
  }
  template <class T, class...SS>
  T &emplace(SS &&...s)
    requires (!is_void_v<T>
              && occurs_exactly_once_v<T, S...>
              && is_constructible_v<T, SS &&...>) {
    return emplace_impl<find_type_v<T, S...>>(forward<SS>(s)...);
  }
  template <class T, class U, class...SS>
  T &emplace(initializer_list<U> l, SS &&...s)
    requires (!is_void_v<T>
              && occurs_exactly_once_v<T, S...>
              && is_constructible_v<T, initializer_list<U> &, SS &&...>) {
    return emplace_impl<find_type_v<T, S...>>(l, forward<SS>(s)...);
  }

  template <size_t I, class...SS>
  variant_alternative_t<I, variant<S...>> &emplace(SS &&...s)
    requires ((I < sizeof...(S))
              && is_constructible_v<nth_type_t<I, S...>, SS &&...>) {
    return emplace_impl<I>(forward<SS>(s)...);
  }
  template <size_t I, class U, class...SS>
  variant_alternative_t<I, variant<S...>> &emplace(initializer_list<U> l,
                                                   SS &&...s)
    requires ((I < sizeof...(S))
              && is_constructible_v
              <nth_type_t<I, S...>, initializer_list<U> &, SS &&...>) {
    return emplace_impl<I>(l, forward<SS>(s)...);
  }

  constexpr bool valueless_by_exception() const noexcept {
    return i == sizeof...(S);
  }
  static constexpr size_t npos() noexcept {
    return sizeof...(S);
  }
  constexpr size_t index() const noexcept {
    return i;
  }

  template <class T>
  constexpr enable_if_t<!is_void_v<T>, bool> is() const noexcept
    requires (occurs_exactly_once_v<T, S...>) {
    return i == find_type_v<T, S...>;
  }
  template <class T>
  constexpr enable_if_t<is_void_v<T>, bool> is() const noexcept {
    return i == sizeof...(S);
  }

  template <class T>
  constexpr T &as() & noexcept requires occurs_exactly_once_v<T, S...> {
    if (i != find_type_v<T, S...>)
      throw_or_terminate<bad_variant_access>();
    return *static_cast<T *>(void_ptr());
  }
  template <class T>
  constexpr const T &as() const & noexcept
    requires occurs_exactly_once_v<T, S...> {
    if (i != find_type_v<T, S...>)
      throw_or_terminate<bad_variant_access>();
    return *static_cast<const T *>(void_ptr());
  }
  template <class T>
  constexpr T &&as() && noexcept
    requires occurs_exactly_once_v<T, S...> {
    if (i != find_type_v<T, S...>)
      throw_or_terminate<bad_variant_access>();
    return static_cast<T &&>(*static_cast<T *>(void_ptr()));
  }
  template <class T>
  constexpr const T &&as() const && noexcept
    requires occurs_exactly_once_v<T, S...> {
    if (i != find_type_v<T, S...>)
      throw_or_terminate<bad_variant_access>();
    return static_cast<const T &&>(*static_cast<const T *>(void_ptr()));
  }

  template <size_t I, class T = nth_type_t<I, S...>>
  constexpr T &at() & noexcept requires (I < sizeof...(S)) {
    if (I != i)
      throw_or_terminate<bad_variant_access>();
    return *static_cast<T *>(void_ptr());
  }
  template <size_t I, class T = nth_type_t<I, S...>>
  constexpr const T &at() const & noexcept requires (I < sizeof...(S)) {
    if (I != i)
      throw_or_terminate<bad_variant_access>();
    return *static_cast<const T *>(void_ptr());
  }
  template <size_t I, class T = nth_type_t<I, S...>>
  constexpr T &&at() && noexcept requires (I < sizeof...(S)) {
    if (I != i)
      throw_or_terminate<bad_variant_access>();
    return static_cast<T &&>(*static_cast<T *>(void_ptr()));
  }
  template <size_t I, class T = nth_type_t<I, S...>>
  constexpr const T &&at() const && noexcept requires (I < sizeof...(S)) {
    if (I != i)
      throw_or_terminate<bad_variant_access>();
    return static_cast<const T &&>(*static_cast<const T *>(void_ptr()));
  }

  template <class...SS>
  friend constexpr bool operator ==(const variant<SS...> &,
                                    const variant<SS...> &);
  template <class...SS>
  friend constexpr common_comparison_category_t<synth_3way_result<SS>...>
  operator <=>(const variant<SS...> &, const variant<SS...> &);
};
template <class...S>
constexpr bool operator ==(const variant<S...> &x, const variant<S...> &y) {
  using fp_t = bool (*)(const void *, const void *);
  constexpr fp_t fps[sizeof...(S) + 1]
    = {inner::fns::make_variant_equal_fp<S>()...,
       [](const void *, const void *) {return true;}};
  return x.index() == y.index()
    && fps[x.index()](x.void_ptr(), y.void_ptr());
}
template <class...S>
constexpr common_comparison_category_t<synth_3way_result<S>...>
operator <=>(const variant<S...> &x, const variant<S...> &y) {
  using ordering_t = common_comparison_category_t<synth_3way_result<S>...>;
  using fp_t = ordering_t (*)(const void *, const void *);
  constexpr fp_t fps[sizeof...(S) + 1]
    = {inner::fns::make_variant_3way_fp<S, ordering_t>()...,
       [](const void *, const void *)->ordering_t {return strong_eq;}};
  if (x.valueless_by_exception() && y.valueless_by_exception())
    return strong_eq;
  if (x.valueless_by_exception())
    return strong_lt;
  if (y.valueless_by_exception())
    return strong_gt;
  if (x.index() != y.index())
    return x.index() <=> y.index();
  return fps[x.index()](x.void_ptr(), y.void_ptr());
}

template <>
struct hash<monostate> {
  using argument_type = monostate;
  using result_type = size_t;

  size_t operator ()(const monostate &x) const noexcept {
    return 0u;
  }
};
template <class...S>
struct hash<variant<S...>> {
  using argument_type = variant<S...>;
  using result_type = size_t;

  size_t operator ()(const variant<S...> &v) const
    noexcept((is_nothrow_hashable_v<S> && ...)) {
    const size_t a = v.index();
    const size_t b = visit_r<size_t>
      ([]<class T>(const T &x) {return hash<T>{}(x);}, v);
    const size_t ret = a ^ b;
    return ret;
  }
};

}

#endif
