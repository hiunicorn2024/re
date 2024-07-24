#include <cassert>
#include <utility>
#include <type_traits>
#include <optional>
#include <iterator>
#include <compare>

template <class F>
struct fo_assign_non_assignable {
  template <class X = F>
  std::enable_if_t<std::is_assignable_v<X &, const X &>>
  operator ()(F &to, const F &from) const
    noexcept(std::is_nothrow_assignable_v<F &, const F &>) {
    to = from;
  }
  template <class X = F>
  std::enable_if_t<!std::is_assignable_v<X &, const X &>>
  operator ()(F &to, const F &from) const
    noexcept(std::is_nothrow_constructible_v<F, const F &>) {
    if (&to != &from) {
      to.~F();
      ::new(&to) F(from);
    }
  }

  template <class X = F>
  std::enable_if_t<std::is_assignable_v<X &, X &&>>
  operator ()(F &to, F &&from) const
    noexcept(std::is_nothrow_assignable_v<F &, F &&>) {
    to = std::move(from);
  }
  template <class X = F>
  std::enable_if_t<!std::is_assignable_v<X &, X &&>>
  operator ()(F &to, F &&from) const
    noexcept(std::is_nothrow_constructible_v<F, F &&>) {
    if (&to != &from) {
      to.~F();
      ::new(&to) F(std::move(from));
    }
  }
};
template <class F>
inline constexpr fo_assign_non_assignable<F> assign_non_assignable{};

template <class F>
class copyable_wrapper {
  template <class T>
  friend class copyable_wrapper;

  F f = F{};

public:
  copyable_wrapper() = delete;
  copyable_wrapper() requires std::is_default_constructible_v<F> = default;
  ~copyable_wrapper() = default;
  copyable_wrapper(const copyable_wrapper &) = default;
  copyable_wrapper(copyable_wrapper &&) = default;
  copyable_wrapper &operator =(const copyable_wrapper &) = delete;
  copyable_wrapper &operator =(copyable_wrapper &&) = delete;
  constexpr copyable_wrapper &operator =(const copyable_wrapper &x)
    noexcept(noexcept(assign_non_assignable<F>(f, x.f)))
    requires (std::is_copy_constructible_v<F> && !std::is_const_v<F>) {
    assign_non_assignable<F>(f, x.f);
    return *this;
  }
  constexpr copyable_wrapper &operator =(copyable_wrapper &&x)
    noexcept(noexcept(assign_non_assignable<F>(f, move(x).f)))
    requires (std::is_move_constructible_v<F> && !std::is_const_v<F>) {
    assign_non_assignable<F>(f, move(x).f);
    return *this;
  }

  constexpr explicit copyable_wrapper(const F &ff) : f(ff) {}
  constexpr explicit copyable_wrapper(F &&ff)
    requires (!std::is_same_v<F &&, const F &>) : f(std::forward<F>(ff)) {}

  template <class F2>
  copyable_wrapper(const copyable_wrapper<F2> &x)
    requires (!std::is_same_v<F, F2> && std::is_convertible_v<const F2 &, F>)
    : f(x.f) {}

  constexpr F *operator ->() noexcept {
    return &f;
  }
  constexpr const F *operator ->() const noexcept {
    return &f;
  }
  constexpr F &operator *() noexcept {
    return f;
  }
  constexpr const F &operator *() const noexcept {
    return f;
  }
};

struct fo_call_or_deref_call {
  template <class F, class...S>
  constexpr auto operator ()(F &&f, S &&...s) const
    ->decltype((*f)(std::forward<S>(s)...)) {
    return (*f)(std::forward<S>(s)...);
  }
  template <class F, class...S>
  constexpr decltype(auto) operator ()(F &&f, S &&...s) const
    requires (requires {f(static_cast<S &&>(s)...);}) {
    return f(std::forward<S>(s)...);
  }
};
inline constexpr fo_call_or_deref_call call_or_deref_call{};

template <class F>
class semiregular_function {
  template <class>
  friend class semiregular_function;

  using t0 = std::conditional_t<std::is_default_constructible_v<F>,
                                F, std::optional<F>>;
  using t = copyable_wrapper<t0>;

  t f = t{};

public:
  semiregular_function() = default;
  ~semiregular_function() = default;
  semiregular_function(const semiregular_function &) = default;
  semiregular_function &operator =(const semiregular_function &) = default;
  semiregular_function(semiregular_function &&) = default;
  semiregular_function &operator =(semiregular_function &&) = default;

  constexpr explicit semiregular_function(F ff) : f(ff) {}

  template <class F2>
  semiregular_function(const semiregular_function<F2> &x)
    requires (!std::is_same_v<F, F2> && std::is_convertible_v<const F2 &, F>)
    : f(x.f) {}

  template <class...S>
  constexpr auto operator ()(S &&...s)
    ->decltype(call_or_deref_call(std::declval<F &>(),
                                  std::forward<S>(s)...)) {
    return call_or_deref_call(*f, std::forward<S>(s)...);
  }
  template <class...S>
  constexpr auto operator ()(S &&...s) const
    ->decltype(call_or_deref_call(std::declval<const F &>(),
                                  std::forward<S>(s)...)) {
    return call_or_deref_call(*f, std::forward<S>(s)...);
  }

  constexpr F base() const {
    if constexpr (std::is_default_constructible_v<F>) {
      return *f;
    }
    else {
      return **f;
    }
  }
};
struct fo_semiregular_fn {
  template <class F>
  semiregular_function<F> operator ()(F f) const {
    return semiregular_function<F>(f);
  }
};
inline constexpr fo_semiregular_fn semiregular_fn{};

template <class T, class U>
concept three_way_comparable_with
  = requires(const std::remove_reference_t<T> &a,
             const std::remove_reference_t<U> &b) {a <=> b;};
template <class FROM, class TO>
concept convertible_to
  = std::is_convertible_v<FROM, TO>
  && requires {static_cast<TO>(std::declval<FROM>());};
template <class T>
concept boolean_testable = convertible_to<T, bool>
  && requires(T &&t) {{!static_cast<T &&>(t)}->convertible_to<bool>;};
constexpr auto synth_3way
  = []<class T, class U>(const T &t, const U &u)
  requires (requires {
      {t < u}->boolean_testable;
      {u < t}->boolean_testable;
    }) {
  if constexpr (three_way_comparable_with<T, U>) {
    return t <=> u;
  }
  else {
    if (t < u)
      return std::weak_ordering::less;
    if (u < t)
      return std::weak_ordering::greater;
    return std::weak_ordering::equivalent;
  }
};
template <class T, class U = T>
using synth_3way_result
  = decltype(synth_3way(std::declval<T &>(), std::declval<U &>()));

template <class II, class F>
class bind_iterator {
  using this_t = bind_iterator;

  template <class, class>
  friend class bind_iterator;

  II i = II{};
  semiregular_function<F> f = semiregular_function<F>{};

public:
  using difference_type = typename std::iterator_traits<II>::difference_type;
  using reference = typename std::iterator_traits<II>::reference;
  using pointer = void;
  using value_type = std::remove_cvref_t<reference>;
  using iterator_category
    = typename std::iterator_traits<II>::iterator_category;

  bind_iterator() = default;
  ~bind_iterator() = default;
  bind_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  bind_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(std::is_nothrow_swappable_v<II>
             && std::is_nothrow_swappable_v<semiregular_function<F>>) {
    using std::swap;
    swap(x.i, y.i);
    swap(x.f, y.f);
  }

  constexpr bind_iterator(II ii, F ff) : i(ii), f(ff) {}

  template <class II2, class FF>
  constexpr bind_iterator(const bind_iterator<II2, FF> &x)
    requires (!(std::is_same_v<II, II2> && std::is_same_v<F, FF>)
              && std::is_convertible_v<const II2 &, II>
              && std::is_convertible_v<const FF &, F>)
    : i(x.i), f(x.f) {}

  constexpr reference operator *() const {
    return f(*i);
  }
  constexpr this_t &operator ++() {
    ++i;
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int)
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::forward_iterator_tag> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --()
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::bidirectional_iterator_tag> {
    --i;
    return *this;
  }
  constexpr this_t operator --(int)
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::bidirectional_iterator_tag> {
    auto ret = *this;
    operator --();
    return ret;
  }

  constexpr reference operator [](difference_type j) const
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::random_access_iterator_tag> {
    return *(*this + j);
  }
  constexpr this_t &operator +=(difference_type j)
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::random_access_iterator_tag> {
    i += j;
    return *this;
  }
  constexpr this_t &operator -=(difference_type j)
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::random_access_iterator_tag> {
    i -= j;
    return *this;
  }
  constexpr this_t operator +(difference_type j) const
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::random_access_iterator_tag> {
    auto tmp = *this;
    return tmp += j;
  }
  constexpr this_t operator -(difference_type j) const
    requires std::is_same_v<typename std::iterator_traits<II>
                            ::iterator_category,
                            std::random_access_iterator_tag> {
    auto tmp = *this;
    return tmp -= j;
  }

  constexpr II base() const {
    return i;
  }
  constexpr auto fn() const {
    return f.base();
  }
};
template <class II1, class II2, class FF>
constexpr auto operator ==(const bind_iterator<II1, FF> &x,
                           const bind_iterator<II2, FF> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class II1, class FF>
constexpr bind_iterator<II1, FF>
operator +(typename bind_iterator<II1, FF>::difference_type n,
           const bind_iterator<II1, FF> &x)
  requires std::is_same_v<typename std::iterator_traits<II1>::iterator_category,
                          std::random_access_iterator_tag> {
  return x + n;
}
template <class II1, class II2, class FF>
constexpr auto operator -(const bind_iterator<II1, FF> &x,
                          const bind_iterator<II2, FF> &y)
  ->decltype(x.base() - y.base()) {
  return x.base() - y.base();
}
template <class II1, class II2, class FF>
constexpr synth_3way_result<II1, II2>
operator <=>(const bind_iterator<II1, FF> &x,
             const bind_iterator<II2, FF> &y) {
  return synth_3way(x.base(), y.base());
}

template <class X, class Y>
constexpr bool test_unequal_impl1(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr bool test_unequal_impl1(X &&x, Y &&y)
  requires requires {std::declval<X &>() < std::declval<Y &>();} {
  if (!((x < y) || (y < x)))
    return false;
  if (!((y > x) || (x > y)))
    return false;
  if (!((x <= y && !(y <= x)) || (y <= x && !(x <= y))))
    return false;
  if (!((x >= y && !(y >= x)) || (y >= x && !(x >= y))))
    return false;
  return true;
}
template <class X, class Y>
constexpr bool test_unequal_impl2(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr bool test_unequal_impl2(X &&x, Y &&y)
  requires three_way_comparable_with<X &, Y &> {
  if (!((x <=> y < 0 && y <=> x > 0)
        || (x <=> y > 0 && y <=> x < 0)))
    return false;
  return true;
}
struct fo_test_unequal {
  template <class X, class Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    if (!(x != y && y != x && !(x == y) && !(y == x)))
      return false;
    if (!test_unequal_impl1(x, y))
      return false;
    if (!test_unequal_impl2(x, y))
      return false;
    return true;
  }
};
inline constexpr fo_test_unequal test_unequal{};
template <class I, class J>
void test_itr(I i, J j) {
  I ii = i;
  J jj = j;
  for (; ii != jj; ++ii) {
    for (auto x = i; x != ii; ++x)
      if (!test_unequal(x, ii))
        assert(false);
    for (auto x = ii; x != jj;) {
      ++x;
      if (!test_unequal(x, ii))
        assert(false);
    }
  }
}

int main() {
  const int a[] = {1, 2, 3, 4};
  auto f = [](const int &x) {return x * 10;};
  test_itr(bind_iterator(std::begin(a), f),
           bind_iterator(std::cend(a), f));
}
