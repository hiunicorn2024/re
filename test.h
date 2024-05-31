#ifndef RE_DEFINED_TEST_H
#define RE_DEFINED_TEST_H

#include "base.h"

// owner_ptr
namespace re {

template <class T>
class owner_ptr {
  T *p;

public:
  owner_ptr() noexcept : p{} {}
  ~owner_ptr() {
    if (p != nullptr)
      delete p;
  }
  owner_ptr(const owner_ptr &x) {
    if (x.p != nullptr)
      p = new T(*x.p);
    else
      p = nullptr;
  }
  owner_ptr &operator =(const owner_ptr &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  owner_ptr(owner_ptr &&x) noexcept {
    p = x.p;
    x.p = nullptr;
  }
  owner_ptr &operator =(owner_ptr &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(owner_ptr &x, owner_ptr &y) noexcept {
    adl_swap(x.p, y.p);
  }

  owner_ptr(nullptr_t) noexcept : p{} {}
  bool operator ==(nullptr_t) const noexcept {
    return empty();
  }

  explicit owner_ptr(T xx) : p(new T(forward<T>(xx))) {}

  T &operator *() const noexcept {
    return *p;
  }
  T *operator ->() const noexcept {
    return p;
  }

  bool empty() const noexcept {
    return p == nullptr;
  }
  void clear() noexcept {
    if (p != nullptr)
      delete p;
    p = nullptr;
  }
};

}

// test_throwing
namespace re {

template <class E>
struct fo_test_throwing {
  template <class F>
  bool operator ()(F &&f) const {
    bool y = false;
#ifndef RE_NOEXCEPT
    try {
      f();
    }
    catch (const E &) {
      y = true;
    }
    catch (...) {}
#endif
    return y;
  }
};
template <class E>
inline constexpr fo_test_throwing<E> test_throwing{};

struct fo_test_throwing_any {
  template <class F>
  bool operator ()(F &&f) const {
    bool y = false;
#ifndef RE_NOEXCEPT
    try {
      f();
    }
    catch (...) {
      y = true;
    }
#endif
    return y;
  }
};
inline constexpr fo_test_throwing_any test_throwing_any{};

}

// test_ownership
// test_copy
// test_move
// test_swap
// test_allocator_aware_container_ownership
namespace re {

template <class, class...>
struct fo_test_ownership;
template <class T, class...S>
inline constexpr fo_test_ownership<T, S...> test_ownership{};
template <class T>
struct fo_test_ownership<T> {
  template <class F1, class F2, class F3, class F4, class F5>
  void operator ()(F1 init_left, F2 equal_left,
                   F3 init_right, F4 equal_right,
                   F5 is_empty) const
    requires copyable<T> && movable<T> && swappable<T> {
    alignas(T) byte buf1[sizeof(T)];
    alignas(T) byte buf2[sizeof(T)];

    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    T *p2 = reinterpret_cast<T *>(addressof(buf2));
    init_left(p1);
    const auto guard1 = exit_fn([=]() {p1->~T();});
    init_right(p2);
    const auto guard2 = exit_fn([=]() {p2->~T();});
    if (!(equal_left(p1) && equal_right(p2)))
      print_then_abort("re::test_ownership(...) failed at step 0\n");

    T &l = *p1;
    T &r = *p2;

    T cpy = l;
    if (!equal_left(addressof(cpy)))
      print_then_abort("re::test_ownership(...) failed at step 1\n");
    if (!equal_left(addressof(l)))
      print_then_abort("re::test_ownership(...) failed at step 2\n");
    T cpy2 = r;
    if (!equal_right(addressof(cpy2)))
      print_then_abort("re::test_ownership(...) failed at step 3\n");
    if (!equal_right(addressof(r)))
      print_then_abort("re::test_ownership(...) failed at step 4\n");
    if (!(addressof(cpy = cpy2) == addressof(cpy)
          && equal_right(addressof(cpy))))
      print_then_abort("re::test_ownership(...) failed at step 5\n");
    if (!equal_right(addressof(cpy2)))
      print_then_abort("re::test_ownership(...) failed at step 6\n");
    if (!(addressof(cpy = cpy) == addressof(cpy)
          && equal_right(addressof(cpy))))
      print_then_abort("re::test_ownership(...) failed at step 7\n");

    T mov = move(l);
    if (!equal_left(addressof(mov)))
      print_then_abort("re::test_ownership(...) failed at step 8\n");
    if (!is_empty(addressof(l)))
      print_then_abort("re::test_ownership(...) failed at step 9\n");
    T mov2 = move(r);
    if (!equal_right(addressof(mov2)))
      print_then_abort("re::test_ownership(...) failed at step 10\n");
    if (!is_empty(addressof(r)))
      print_then_abort("re::test_ownership(...) failed at step 11\n");
    adl_swap(mov, mov2);
    if (!equal_right(addressof(mov)))
      print_then_abort
        ("re::test_ownership(...) failed at step 12\n");
    if (!equal_left(addressof(mov2)))
      print_then_abort
        ("re::test_ownership(...) failed at step 13\n");

    adl_swap(mov, mov2);
    if (!equal_left(addressof(mov)))
      print_then_abort
        ("re::test_ownership(...) failed at step 14\n");
    if (!equal_right(addressof(mov2)))
      print_then_abort
        ("re::test_ownership(...) failed at step 15\n");
    if (!(addressof(mov = move(mov2)) == addressof(mov)
          && equal_right(addressof(mov))))
      print_then_abort
        ("re::test_ownership(...) failed at step 16\n");
    if (!is_empty(addressof(mov2)))
      print_then_abort
        ("re::test_ownership(...) failed at step 17\n");
    if (!(addressof(mov = move(mov)) == addressof(mov)
          && equal_right(addressof(mov))))
      print_then_abort
        ("re::test_ownership(...) failed at step 18\n");
    adl_swap(mov, mov);
    if (!equal_right(addressof(mov)))
      print_then_abort
        ("re::test_ownership(...) failed at step 19\n");
  }
};
template <class T, class EMPTY_CHECK, class...INIT_TYPES>
struct fo_test_ownership<T, EMPTY_CHECK, INIT_TYPES...> {
  void operator ()() const {
    const auto f = [](auto tag, auto tag2) {
      using T1 = typename decltype(tag)::type;
      using T2 = typename decltype(tag2)::type;
      test_ownership<T>(T1::init, T1::good, T2::init, T2::good,
                        EMPTY_CHECK::apply);
    };
    using L = type_pack<INIT_TYPES...>;
    (L() * L())(f);
  }
};

template <class T>
struct fo_test_copy {
  template <class F1, class F2, class F3, class F4>
  void operator ()(F1 init_left, F2 equal_left,
                   F3 init_right, F4 equal_right) const
    requires copyable<T> {
    alignas(T) byte buf1[sizeof(T)];
    alignas(T) byte buf2[sizeof(T)];

    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    T *p2 = reinterpret_cast<T *>(addressof(buf2));
    init_left(p1);
    const auto guard1 = exit_fn([&]() {p1->~T();});
    init_right(p2);
    const auto guard2 = exit_fn([&]() {p2->~T();});

    if (!(equal_left(p1) && equal_right(p2)))
      print_then_abort("re::test_copy() failed at step 0\n");

    T &l = *p1;
    T &r = *p2;

    T cpy = l;
    if (!(equal_left(addressof(cpy)) && equal_left(addressof(l))))
      print_then_abort("re::test_copy() failed at step 1\n");
    T cpy2 = r;
    if (!(equal_right(addressof(cpy2)) && equal_right(addressof(r))))
      print_then_abort("re::test_copy() failed at step 2\n");
    if (!(addressof(cpy = cpy2) == addressof(cpy)
          && equal_right(addressof(cpy)) && equal_right(addressof(cpy2))))
      print_then_abort("re::test_copy() failed at step 3\n");
    if (!(addressof(cpy = cpy) == addressof(cpy)
          && equal_right(addressof(cpy))))
      print_then_abort("re::test_copy() failed at step 4\n");
  }
};
template <class T>
inline constexpr fo_test_copy<T> test_copy{};

template <class T>
struct fo_test_move {
  template <class F1, class F2, class F3, class F4, class F5>
  void operator ()(F1 init_left, F2 equal_left, F3 init_right, F4 equal_right,
                   F5 is_empty) const
    requires movable<T> {
    alignas(T) byte buf1[sizeof(T)];
    alignas(T) byte buf2[sizeof(T)];
    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    T *p2 = reinterpret_cast<T *>(addressof(buf2));
    init_left(p1);
    const auto guard1 = exit_fn([&]() {p1->~T();});
    init_right(p2);
    const auto guard2 = exit_fn([&]() {p2->~T();});
    if (!(equal_left(p1) && equal_right(p2)))
      print_then_abort("re::test_move() failed at step 0\n");

    T &l = *p1;
    T &r = *p2;

    T mov = move(l);
    if (!(equal_left(addressof(mov)) && is_empty(addressof(l))))
      print_then_abort("re::test_move() failed at step 1\n");
    T mov2 = move(r);
    if (!(equal_right(addressof(mov2)) && is_empty(addressof(r))))
      print_then_abort("re::test_move() failed at step 2\n");
    if (!(addressof(mov = move(mov2)) == addressof(mov)
          && equal_right(addressof(mov)) && is_empty(addressof(mov2))))
      print_then_abort("re::test_move() failed at step 3\n");
    if (!(addressof(mov = move(mov)) == addressof(mov)
          && equal_right(addressof(mov))))
      print_then_abort("re::test_move() failed at step 4\n");
  }
};
template <class T>
inline constexpr fo_test_move<T> test_move{};

template <class T>
struct fo_test_swap {
  template <class F1, class F2, class F3, class F4>
  void operator ()(F1 init_left, F2 equal_left,
                   F3 init_right, F4 equal_right) const
    requires swappable<T> {
    alignas(T) byte buf1[sizeof(T)];
    alignas(T) byte buf2[sizeof(T)];
    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    T *p2 = reinterpret_cast<T *>(addressof(buf2));
    init_left(p1);
    const auto guard1 = exit_fn([&]() {p1->~T();});
    init_right(p2);
    const auto guard2 = exit_fn([&]() {p2->~T();});
    if (!(equal_left(p1) && equal_right(p2)))
      print_then_abort("re::test_swap() failed at step 0\n");

    T &l = *p1;
    T &r = *p2;

    adl_swap(l, r);
    if (!(equal_right(addressof(l)) && equal_left(addressof(r))))
      print_then_abort("re::test_swap() failed at step 1\n");
    adl_swap(l, r);
    if (!(equal_left(addressof(l)) && equal_right(addressof(r))))
      print_then_abort("re::test_swap() failed at step 2\n");
    adl_swap(l, l);
    if (!equal_left(addressof(l)))
      print_then_abort("re::test_swap() failed at step 3\n");
  }
};
template <class T>
inline constexpr fo_test_swap<T> test_swap{};

template <class T>
struct fo_test_copy_construct {
  template <class F1, class F2, class F3>
  void operator ()(F1 init_left, F2 equal_left, F3 empt) const
    requires is_copy_constructible_v<T> && is_move_constructible_v<T> {
    alignas(T) byte buf1[sizeof(T)];

    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    init_left(p1);
    const auto guard1 = exit_fn([&]() {p1->~T();});

    if (!(equal_left(p1)))
      print_then_abort("re::test_copy_construct() failed at step 0\n");

    T &l = *p1;

    T cpy = l;
    if (!(equal_left(addressof(cpy)) && equal_left(addressof(l))))
      print_then_abort("re::test_copy_construct() failed at step 1\n");
    T cpy2 = move(l);
    if (!(equal_left(addressof(cpy2)) && empt(p1)))
      print_then_abort("re::test_copy_construct() failed at step 2\n");
  }
};
template <class T>
inline constexpr fo_test_copy_construct<T> test_copy_construct{};

namespace inner::fns {

template <class T>
constexpr bool copy_prpg() {
  return false;
}
template <class T>
constexpr bool copy_prpg()
  requires requires {typename T::propagate_on_container_copy_assignment;} {
  return T::propagate_on_container_copy_assignment::value;
}

template <class T>
constexpr bool move_prpg() {
  return false;
}
template <class T>
constexpr bool move_prpg()
  requires requires {typename T::propagate_on_container_move_assignment;} {
  return T::propagate_on_container_move_assignment::value;
}

template <class T>
constexpr bool swap_prpg() {
  return false;
}
template <class T>
constexpr bool swap_prpg()
  requires requires {typename T::propagate_on_container_swap;} {
  return T::propagate_on_container_swap::value;
}

}
template <class T>
struct fo_test_allocator_aware_container_ownership {
  template <class F1, class F2, class F3, class F4, class F5>
  void operator ()(F1 init_left, F2 equal_left,
                   F3 init_right, F4 equal_right,
                   F5 is_empty) const {
    alignas(T) byte buf1[sizeof(T)];
    alignas(T) byte buf2[sizeof(T)];
    T *p1 = reinterpret_cast<T *>(addressof(buf1));
    T *p2 = reinterpret_cast<T *>(addressof(buf2));
    init_left(p1);
    const auto guard1 = exit_fn([=]() {p1->~T();});
    init_right(p2);
    const auto guard2 = exit_fn([=]() {p2->~T();});
    if (!(equal_left(p1) && equal_right(p2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 0\n");

    T &l = *p1;
    T &r = *p2;
    const auto l_al = l.get_allocator();
    const auto r_al = r.get_allocator();

    T cpy = l;
    if (!equal_left(addressof(cpy)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 1\n");
    if (!equal_left(addressof(l)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 2\n");
    T cpy2 = r;
    if (!equal_right(addressof(cpy2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 3\n");
    if (!equal_right(addressof(r)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 4\n");
    cpy = cpy2;
    if (!(l_al == r_al ? cpy.get_allocator() == cpy2.get_allocator()
          : (inner::fns::copy_prpg<decltype(l_al)>()
             ? cpy.get_allocator() == r_al
             : cpy.get_allocator() == l_al)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 5\n");
    if (!equal_right(addressof(cpy)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 6\n");
    if (!equal_right(addressof(cpy2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 7\n");

    T mov = move(l);
    if (!equal_left(addressof(mov)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 8\n");
    if (!is_empty(addressof(l)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 9\n");
    T mov2 = move(r);
    if (!equal_right(addressof(mov2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 10\n");
    if (!is_empty(addressof(r)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 11\n");
    adl_swap(mov, mov2);
    if (!(l_al == r_al ? mov.get_allocator() == mov2.get_allocator()
          : (inner::fns::swap_prpg<decltype(l_al)>()
             ? (mov.get_allocator() == r_al
                && mov2.get_allocator() == l_al)
             : (mov.get_allocator() == l_al
                && mov2.get_allocator() == r_al))))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 12\n");
    if (!equal_right(addressof(mov)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 13\n");
    if (!equal_left(addressof(mov2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 14\n");

    adl_swap(mov, mov2);
    if (!(mov.get_allocator() == l_al && mov2.get_allocator() == r_al))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 15\n");
    if (!equal_left(addressof(mov)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 16\n");
    if (!equal_right(addressof(mov2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 17\n");
    mov = move(mov2);
    if (!(l_al == r_al ? mov.get_allocator() == mov2.get_allocator()
          : (inner::fns::move_prpg<decltype(l_al)>()
             ? mov.get_allocator() == r_al
             : mov.get_allocator() == l_al)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 18\n");
    if (!equal_right(addressof(mov)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 19\n");
    if (!is_empty(addressof(mov2)))
      print_then_abort
        ("re::test_allocator_aware_container_ownership(...) "
         "failed at step 20\n");
  }
};
template <class T>
inline constexpr fo_test_allocator_aware_container_ownership<T>
test_allocator_aware_container_ownership{};

}

// test_equal
// test_unequal
// test_less
// test_equality
// test_inequality
// test_lessness
namespace re {

namespace inner::fns {

template <class X, class Y>
constexpr enable_if_t<!can_apply_less<X &, Y &>, bool>
test_equal_impl1(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr enable_if_t<can_apply_less<X &, Y &>, bool>
test_equal_impl1(X &&x, Y &&y) {
  if (!(!(x < y) && !(y < x)))
    return false;
  if (!(!(y > x) && !(x > y)))
    return false;
  if (!(x <= y && y <= x))
    return false;
  if (!(y >= x && x >= y))
    return false;
  return true;
}

template <class X, class Y>
constexpr enable_if_t<!three_way_comparable_with<X &, Y &>, bool>
test_equal_impl2(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr enable_if_t<three_way_comparable_with<X &, Y &>, bool>
test_equal_impl2(X &&x, Y &&y) {
  if (!(x <=> y == 0 && y <=> x == 0))
    return false;
  return true;
}

template <class X, class Y>
constexpr bool test_unequal_impl1(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr bool test_unequal_impl1(X &&x, Y &&y)
  requires can_apply_less<X &, Y &> {
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

template <class X, class Y>
constexpr bool test_less_impl1(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr bool test_less_impl1(X &&x, Y &&y)
  requires equality_comparable_with<X &, Y &> {
  if (!(!(x == y) && !(y == x) && x != y && y != x))
    return false;
  return true;
}

template <class X, class Y>
constexpr bool test_less_impl2(X &&x, Y &&y) {
  return true;
}
template <class X, class Y>
constexpr bool test_less_impl2(X &&x, Y &&y)
  requires three_way_comparable_with<X &, Y &> {
  if (!(x <=> y < 0 && y <=> x > 0))
    return false;
  return true;
}

}
struct fo_test_equal {
  template <class X, class Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    if (!(x == y && y == x && !(x != y) && !(y != x)))
      return false;
    if (!inner::fns::test_equal_impl1(x, y))
      return false;
    if (!inner::fns::test_equal_impl2(x, y))
      return false;
    return true;
  }
};
inline constexpr fo_test_equal test_equal{};
struct fo_test_unequal {
  template <class X, class Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    if (!(x != y && y != x && !(x == y) && !(y == x)))
      return false;
    if (!inner::fns::test_unequal_impl1(x, y))
      return false;
    if (!inner::fns::test_unequal_impl2(x, y))
      return false;
    return true;
  }
};
inline constexpr fo_test_unequal test_unequal{};
struct fo_test_less {
  template <class X, class Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    if (!inner::fns::test_less_impl1(x, y))
      return false;

    if (!(x < y && !(y < x)))
      return false;
    if (!(y > x && !(x > y)))
      return false;
    if (!(x <= y && !(y <= x)))
      return false;
    if (!(y >= x && !(x >= y)))
      return false;

    if (!inner::fns::test_less_impl2(x, y))
      return false;

    return true;
  }
};
inline constexpr fo_test_less test_less{};

struct fo_test_equality {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y) const {
    if (!test_equal(forward<X>(x), forward<Y>(y)))
      print_then_abort("re::test_equality(x, y) failed\n");
  }
};
inline constexpr fo_test_equality test_equality{};
struct fo_test_inequality {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y) const {
    if (!test_unequal(forward<X>(x), forward<Y>(y)))
      print_then_abort("re::test_inequality(x, y) failed\n");
  }
};
inline constexpr fo_test_inequality test_inequality{};
struct fo_test_lessness {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y) const {
    if (!test_less(forward<X>(x), forward<Y>(y)))
      print_then_abort("re::test_lessness(x, y) failed\n");
  }
};
inline constexpr fo_test_lessness test_lessness{};

}

// instance_counter
// exception_countdown
namespace re {

template <class T>
class instance_counter {
  static size_t c;
  T data;

  void plus() {
    ++c;
    if (c == integral_traits<int>::max())
      print_then_abort("re::instance_counter: count beyond limit\n");
  }

public:
  instance_counter() : data{} {
    plus();
  }
  ~instance_counter() {
    --c;
  }
  instance_counter(const instance_counter &x) : data(x.data) {
    plus();
  }
  instance_counter &operator =(const instance_counter &x) {
    data = x.data;
    return *this;
  }
  instance_counter(instance_counter &&x)
    noexcept(is_nothrow_move_constructible_v<T>)
    : data(move(x.data)) {
    plus();
  }
  instance_counter &operator =(instance_counter &&x)
    noexcept(is_nothrow_move_assignable_v<T>) {
    data = move(x.data);
    return *this;
  }
  friend void swap(instance_counter &x, instance_counter &y)
    noexcept(is_nothrow_swappable_v<T>)
    requires (is_swappable_v<T>) {
    adl_swap(x.data, y.data);
  }

  template <class F, class...S,
            class = enable_if_t
            <!(is_same_v<instance_counter, remove_cvref_t<F>>
               && sizeof...(S) == 0)
             && is_constructible_v<T, F &&, S &&...>>>
  explicit(sizeof...(S) == 0 && !is_convertible_v<F &&, T>)
  instance_counter(F &&t, S &&...s)
    : data(forward<F>(t), forward<S>(s)...) {
    plus();
  }

  const T *operator ->() const noexcept {
    return addressof(data);
  }
  T *operator ->() noexcept {
    return addressof(data);
  }

  const T &operator *() const noexcept {
    return data;
  }
  T &operator *() noexcept {
    return data;
  }

  operator T &() noexcept {
    return data;
  }
  operator const T &() const noexcept {
    return data;
  }

  static size_t count() noexcept {
    return c;
  }

  template <class S>
  friend bool operator ==(const instance_counter<S> &,
                          const instance_counter<S> &)
    requires equality_comparable<S>;
  template <class S>
  friend synth_3way_result<S> operator <=>(const instance_counter<S> &,
                                           const instance_counter<S> &);
};
template <class S>
bool operator ==(const instance_counter<S> &x, const instance_counter<S> &y)
  requires equality_comparable<S> {
  return x.data == y.data;
}
template <class S>
synth_3way_result<S> operator <=>(const instance_counter<S> &x,
                                  const instance_counter<S> &y) {
  return synth_3way(x.data, y.data);
}
template <class T>
inline size_t instance_counter<T>::c = 0;

template <class T>
class exception_countdown : public instance_counter<T> {
  using this_t = exception_countdown;
  using base_t = instance_counter<T>;

  static unsigned default_constructor_left;
  static unsigned constructor_left;
  static unsigned copy_constructor_left;
  static unsigned move_constructor_left;
  static unsigned copy_assignment_left;
  static unsigned move_assignment_left;

  void constructor_check() {
    if (constructor_left)
      if (--constructor_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
  }

public:
  exception_countdown() : base_t() {
    if (default_constructor_left)
      if (--default_constructor_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
    constructor_check();
  }
  ~exception_countdown() = default;
  exception_countdown(const exception_countdown &t)
    : base_t(static_cast<const base_t &>(t)) {
    if (copy_constructor_left)
      if (--copy_constructor_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
    constructor_check();
  }
  exception_countdown &operator =(const exception_countdown &t) {
    if (copy_assignment_left)
      if (--copy_assignment_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
    base_t::operator =(static_cast<const base_t &>(t));
    return *this;
  }
  exception_countdown(exception_countdown &&t)
    : base_t(move(static_cast<base_t &>(t))) {
    if (move_constructor_left)
      if (--move_constructor_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
    constructor_check();
  }
  exception_countdown &operator =(exception_countdown &&t) {
    if (move_assignment_left)
      if (--move_assignment_left == 0)
        throw_or_abort<logic_error>("exception for test\n");
    base_t::operator =(move(static_cast<base_t &>(t)));
    return *this;
  }

  template <class F, class...S, class = enable_if_t
            <!(is_same_v<this_t, remove_cvref_t<F>> && sizeof...(S) == 0)
             && is_constructible_v<T, F &&, S &&...>>>
  explicit(sizeof...(S) == 0 && !is_convertible_v<F &&, T>)
  exception_countdown(F &&t, S &&...s)
    : base_t(forward<F>(t), forward<S>(s)...) {
    constructor_check();
  }

  base_t &base() {
    return *this;
  }
  const base_t &base() const {
    return *this;
  }

  static void constructor_countdown(unsigned i) {
    constructor_left = i;
  }
  static auto constructor_countdown() {
    return constructor_left;
  }
  static void default_constructor_countdown(unsigned i) {
    default_constructor_left = i;
  }
  static auto default_constructor_countdown() {
    return default_constructor_left;
  }
  static void copy_constructor_countdown(unsigned i) {
    copy_constructor_left = i;
  }
  static auto copy_constructor_countdown() {
    return copy_constructor_left;
  }
  static void move_constructor_countdown(unsigned i) {
    move_constructor_left = i;
  }
  static auto move_constructor_countdown() {
    return move_constructor_left;
  }
  static void copy_assignment_countdown(unsigned i) {
    copy_assignment_left = i;
  }
  static auto copy_assignment_countdown() {
    return copy_assignment_left;
  }
  static void move_assignment_countdown(unsigned i) {
    move_assignment_left = i;
  }
  static auto move_assignment_countdown() {
    return move_assignment_left;
  }
};
template <class S>
bool operator ==(const exception_countdown<S> &x,
                 const exception_countdown<S> &y)
  requires equality_comparable<S> {
  return x.base() == y.base();
}
template <class S>
synth_3way_result<S> operator <=>(const exception_countdown<S> &x,
                                  const exception_countdown<S> &y) {
  return x.base() <=> y.base();
}
template <class T>
inline unsigned exception_countdown<T>::default_constructor_left = 0;
template <class T>
inline unsigned exception_countdown<T>::constructor_left = 0;
template <class T>
inline unsigned exception_countdown<T>::copy_constructor_left = 0;
template <class T>
inline unsigned exception_countdown<T>::move_constructor_left = 0;
template <class T>
inline unsigned exception_countdown<T>::copy_assignment_left = 0;
template <class T>
inline unsigned exception_countdown<T>::move_assignment_left = 0;

}

// ez_dynamic
// ez_function
namespace re {

namespace inner {

template <class U>
concept ezdy_has_clone_fn = requires(const U &x) {{x.clone()}->same_as<U *>;};

}

template <class T>
class ez_dynamic {
  T *p;

  explicit ez_dynamic(T *pp) : p(pp) {}

  ez_dynamic clone() const requires inner::ezdy_has_clone_fn<T> {
    return p != nullptr ? ez_dynamic(p->clone()) : ez_dynamic{};
  }
  ez_dynamic clone() const {
    return p != nullptr ? make(*p) : ez_dynamic{};
  }

public:
  ez_dynamic() noexcept : p(nullptr) {}
  ~ez_dynamic() {
    if (p != nullptr) delete p;
  }
  ez_dynamic(const ez_dynamic &x)
    requires is_copy_constructible_v<T> || inner::ezdy_has_clone_fn<T>
    : ez_dynamic(x.clone()) {}
  ez_dynamic &operator =(const ez_dynamic &x)
    requires is_copy_constructible_v<T> || inner::ezdy_has_clone_fn<T> {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_dynamic(ez_dynamic &&x) noexcept : p(x.p) {
    x.p = nullptr;
  }
  ez_dynamic &operator =(ez_dynamic &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_dynamic &x, ez_dynamic &y) noexcept {
    adl_swap(x.p, y.p);
  }

  template <class U, class...S>
  static ez_dynamic make(S &&...s)
    requires same_as<T, U>
    || (is_base_of_v<T, U> && has_virtual_destructor_v<T>
        && inner::ezdy_has_clone_fn<T>) {
    return ez_dynamic(new U(forward<S>(s)...));
  }
  template <class...S>
  static ez_dynamic make(S &&...s) {
    return ez_dynamic(new T(forward<S>(s)...));
  }

  bool empty() const noexcept {
    return p == nullptr;
  }
  void clear() noexcept {
    *this = ez_dynamic{};
  }

  T *operator ->() const noexcept {
    return p;
  }
  T &operator *() const noexcept {
    return *p;
  }
};

namespace inner {

struct ezfn_tag {};
template <class R, class...S>
class ezfn_base {
public:
  virtual ~ezfn_base() {}
  virtual ezfn_base *clone() const = 0;
  virtual R call(S...s) const = 0;
};
template <class IMPL, class R, class...S>
class ezfn_impl : public ezfn_base<R, S...> {
  using base_t = ezfn_base<R, S...>;
  using this_t = ezfn_impl;

  IMPL f;

public:
  ezfn_impl() {}
  virtual ~ezfn_impl() = default;
  ezfn_impl(const ezfn_impl &) = default;
  ezfn_impl &operator =(const ezfn_impl &) = default;
  ezfn_impl(ezfn_impl &&) = default;
  ezfn_impl &operator =(ezfn_impl &&) = default;

  ezfn_impl(const IMPL &x) : f(x) {}
  ezfn_impl(IMPL &&x) : f(move(x)) {}

  base_t *clone() const {
    return new this_t(f);
  }
  R call(S...s) const {
    return f(forward<S>(s)...);
  }
};

}
template <class F>
class ez_function;
template <class R, class...S>
class ez_function<R (S...)> {
  using fn_base_t = inner::ezfn_base<R, S...>;
  template <class IMPL>
  using fn_impl_t = inner::ezfn_impl<IMPL, R, S...>;
  using dynamic_t = ez_dynamic<fn_base_t>;

  dynamic_t d;

  template <class F>
  ez_function(inner::ezfn_tag, F &&f)
    : d(dynamic_t::template make<fn_impl_t<F>>(forward<F>(f))) {}

public:
  ez_function() = default;
  ~ez_function() = default;
  ez_function(const ez_function &) = default;
  ez_function &operator =(const ez_function &) = default;
  ez_function(ez_function &&) = default;
  ez_function &operator =(ez_function &&) = default;

  template <class F>
  ez_function(F f) requires (!is_same_v<remove_cvref_t<F>, ez_function>)
    : ez_function(inner::ezfn_tag{}, move(f)) {}

  R operator ()(S...s) const {
    if (d.empty())
      throw_or_abort<logic_error>("re::ez_function: bad function call\n");
    return d->call(forward<S>(s)...);
  }

  bool empty() const noexcept {
    return d.empty();
  }
  void clear() noexcept {
    d.clear();
  }
};

}

// ez_vector
namespace re {

template <class T>
class ez_vector {
  T *op;
  T *now;
  T *ed;

  ez_vector(T *a, T *b, T *c) : op(a), now(b), ed(c) {}

  template <class IT, class IT2>
  static IT2 uninitialized_copy(IT i, IT j, IT2 o) {
    using vt = decay_t<decltype(*o)>;
    IT2 o_bk = o;
#ifndef RE_NO_EXCPTIONS
    try {
#endif
      for (; i != j; ++i) {
        ::new(to_address(o)) vt(*i);
        ++o;
      }
#ifndef RE_NO_EXCPTIONS
    }
    catch (...) {
      for (IT k = o_bk; k != o; ++k)
        k->~vt();
      throw;
    }
#endif
    return o;
  }

  void destroy() noexcept {
    if (op != nullptr) {
      for (auto i = op; i != now; ++i)
        i->~value_type();
      operator delete(op);
    }
  }

  void increase_capacity() {
    if (to_unsigned(ed - op) == max_size())
      throw_or_abort<logic_error>("re::ez_vector: size overflow\n");
    size_type new_capacity = (op != ed)
      ? ((to_unsigned(ed - op) < (max_size() / 2))
         ? to_unsigned((ed - op) * 2) : max_size())
      : 1;
    value_type *const new_op
      = reinterpret_cast<value_type *>
      (operator new(sizeof(value_type) * new_capacity));
    ez_vector tmp(new_op, new_op, new_op + new_capacity);
    tmp.now = uninitialized_copy(op, now, new_op);

    adl_swap(tmp, *this);
  }

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = T *;
  using const_iterator = const T *;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  iterator begin() noexcept {
    return op;
  }
  const_iterator begin() const noexcept {
    return op;
  }
  iterator end() noexcept {
    return now;
  }
  const_iterator end() const noexcept {
    return now;
  }
  bool empty() const noexcept {
    return now == op;
  }
  size_t size() const noexcept {
    return now - op;
  }
  size_t max_size() const noexcept {
    return integral_traits<make_signed_t<size_t>>::max() / sizeof(value_type);
  }
  size_t capacity() const noexcept {
    return ed - op;
  }

  ez_vector() noexcept : op(nullptr), now(nullptr), ed(nullptr) {}
  ~ez_vector() {
    destroy();
  }
  ez_vector(const ez_vector &x) {
    op = reinterpret_cast<value_type *>
      (operator new((x.ed - x.op) * sizeof(value_type)));
    ed = op + (x.ed - x.op);
    now = op;
    now = uninitialized_copy(x.begin(), x.end(), op);
  }
  ez_vector &operator =(const ez_vector &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_vector(ez_vector &&x) noexcept : op(x.op), now(x.now), ed(x.ed) {
    x.op = x.now = x.ed = nullptr;
  }
  ez_vector &operator =(ez_vector &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_vector &x, ez_vector &y) noexcept {
    adl_swap(x.op, y.op);
    adl_swap(x.now, y.now);
    adl_swap(x.ed, y.ed);
  }

  ez_vector(initializer_list<value_type> l) : ez_vector{} {
    for (auto i = l.begin(); i != l.end(); ++i)
      insert(end(), *i);
  }

  iterator insert(const_iterator pos, const value_type &x) {
    size_type offset = pos - op;

    if (now == ed)
      increase_capacity();

    ::new(to_address(now)) value_type(x);
    ++now;
    for (auto i = op + offset, j = now - 1; i != j;) {
      --j;
      adl_swap(*j, *(j + 1));
    }
    return op + offset;
  }
  iterator erase(const_iterator from, const_iterator to) noexcept {
    value_type *o = const_cast<value_type *>(from);
    value_type *i = const_cast<value_type *>(to);
    while (i != now)
      *o++ = *i++;
    value_type *const new_now = o;
    for (; o != now; ++o)
      o->~value_type();
    now = new_now;
    return const_cast<value_type *>(from);
  }
  iterator erase(const_iterator from) noexcept {
    return erase(from, from + 1);
  }
};
template <class U>
bool operator ==(const ez_vector<U> &x, const ez_vector<U> &y)
  requires equality_comparable<U> {
  if (x.size() != y.size())
    return false;
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  while (i != j) {
    if (!(*i == *ii))
      return false;
    ++i;
    ++ii;
  }
  return true;
}
template <class U>
synth_3way_result<U> operator <=>(const ez_vector<U> &x,
                                  const ez_vector<U> &y) {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j)
      return ii == jj ? strong_eq : strong_lt;
    else if (ii == jj)
      return strong_gt;

    const auto cmp = synth_3way(*i, *ii);
    if (cmp != 0)
      return cmp;
    ++i;
    ++ii;
  }
}

}

// ez_forward_list
namespace re {

template <class T>
class ez_forward_list;

namespace inner {

template <class T>
struct ez_forward_list_node;
template <class T>
struct ez_forward_list_node_base {
  const ez_forward_list_node_base *next;
};
template <class T>
struct ez_forward_list_node : ez_forward_list_node_base<T> {
  using base_t = ez_forward_list_node_base<T>;
  T value;

  ez_forward_list_node() = delete;
  ~ez_forward_list_node() = default;
  ez_forward_list_node(const ez_forward_list_node &) = delete;
  ez_forward_list_node &operator =(const ez_forward_list_node &) = delete;
  ez_forward_list_node(ez_forward_list_node &&) = delete;
  ez_forward_list_node &operator =(ez_forward_list_node &&) = delete;

  ez_forward_list_node(const base_t *n, const T &x)
    : base_t(n), value(x) {}
  ez_forward_list_node(const base_t *n, T &&x)
    : base_t(n), value(static_cast<T &&>(x)) {}
};

template <class T>
class ez_forward_list_iterator {
  template <class>
  friend class ez_forward_list_iterator;
  template <class>
  friend class re::ez_forward_list;

  using this_t = ez_forward_list_iterator;

  using node_base_t = ez_forward_list_node_base<remove_const_t<T>>;
  using node_t = ez_forward_list_node<remove_const_t<T>>;
  const node_base_t *p;

public:
  using value_type = remove_const_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = forward_iterator_tag;

  ez_forward_list_iterator() noexcept : p{} {}
  ~ez_forward_list_iterator() = default;
  ez_forward_list_iterator(const this_t &) = default;
  ez_forward_list_iterator &operator =(const this_t &) = default;
  ez_forward_list_iterator(this_t &&) = default;
  ez_forward_list_iterator &operator =(this_t &&) = default;

private:
  explicit ez_forward_list_iterator(const node_base_t *pp) : p(pp) {}
  const node_base_t *node() const {
    return p;
  }
public:
  operator ez_forward_list_iterator<add_const_t<T>>() const
    requires (!is_const_v<T>) {
    return ez_forward_list_iterator<add_const_t<T>>(p);
  }

  T *operator ->() const {
    return addressof(operator *());
  }
  T &operator *() const {
    return static_cast<node_t *>(const_cast<node_base_t *>(p))->value;
  }
  template <class X, class Y>
  friend bool operator ==(const ez_forward_list_iterator<X> &,
                          const ez_forward_list_iterator<Y> &)
    requires is_same_v<remove_const_t<X>, remove_const_t<Y>>;

  ez_forward_list_iterator &operator ++() {
    p = p->next;
    return *this;
  }
  ez_forward_list_iterator operator ++(int) {
    auto ret = *this;
    operator ++();
    return ret;
  }
};
template <class X, class Y>
bool operator ==(const ez_forward_list_iterator<X> &x,
                 const ez_forward_list_iterator<Y> &y)
  requires is_same_v<remove_const_t<X>, remove_const_t<Y>> {
  return x.p == y.p;
}

}

template <class T>
class ez_forward_list {
  using node_t = inner::ez_forward_list_node<T>;
  using node_base_t = inner::ez_forward_list_node_base<T>;

  node_base_t end_node;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = inner::ez_forward_list_iterator<T>;
  using const_iterator = inner::ez_forward_list_iterator<const T>;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  ez_forward_list() noexcept {
    end_node.next = nullptr;
  }
  ~ez_forward_list() {
    while (!empty())
      erase_after(before_begin());
  }
  ez_forward_list(const ez_forward_list &x) : ez_forward_list() {
    auto i = before_begin();
    for (const T &z : x)
      i = insert_after(i, z);
  }
  ez_forward_list &operator =(const ez_forward_list &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_forward_list(ez_forward_list &&x) noexcept : ez_forward_list() {
    adl_swap(*this, x);
  }
  ez_forward_list &operator =(ez_forward_list &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_forward_list &x, ez_forward_list &y) noexcept {
    adl_swap(x.end_node, y.end_node);
  }

  ez_forward_list(initializer_list<T> l) : ez_forward_list() {
    auto i = before_begin();
    for (const T &x : l)
      i = insert_after(i, x);
  }

  iterator before_begin() noexcept {
    return iterator(addressof(end_node));
  }
  iterator begin() noexcept {
    return iterator(end_node.next);
  }
  iterator end() noexcept {
    return iterator(nullptr);
  }
  const_iterator before_begin() const noexcept {
    return const_iterator(addressof(end_node));
  }
  const_iterator begin() const noexcept {
    return const_iterator(end_node.next);
  }
  const_iterator end() const noexcept {
    return const_iterator(nullptr);
  }
  bool empty() const noexcept {
    return begin() == end();
  }
  size_t max_size() const noexcept {
    return integral_traits<make_signed_t<size_t>>::max() / sizeof(value_type);
  }

  iterator new_node(const T &x) {
    node_t *p = new node_t(nullptr, x);
    return iterator(static_cast<const node_base_t *>(p));
  }
  iterator new_node(T &&x) {
    node_t *p = new node_t(nullptr, move(x));
    return iterator(static_cast<const node_base_t *>(p));
  }
  void delete_node(const_iterator i) noexcept {
    node_t *p = static_cast<node_t *>(const_cast<node_base_t *>(i.node()));
    delete p;
  }
  iterator link_after(const_iterator i, iterator n) noexcept {
    node_base_t *it = const_cast<node_base_t *>(i.node());
    node_base_t *next_it = const_cast<node_base_t *>(it->next);
    node_base_t *p = const_cast<node_base_t *>(n.node());
    it->next = p;
    p->next = next_it;
    return iterator(p);
  }
  iterator unlink_after(const_iterator i) noexcept {
    node_base_t *it = const_cast<node_base_t *>(i.node());
    node_base_t *next_it = const_cast<node_base_t *>(it->next);
    node_base_t *next_next_it = const_cast<node_base_t *>(next_it->next);
    it->next = next_next_it;
    return iterator(next_next_it);
  }
  iterator insert_after(const_iterator i, const T &x) {
    return link_after(i, new_node(x));
  }
  iterator insert_after(const_iterator i, T &&x) {
    return link_after(i, new_node(move(x)));
  }
  iterator erase_after(const_iterator i) noexcept {
    auto x = ++copy(i);
    auto ret = unlink_after(i);
    delete_node(x);
    return ret;
  }
  iterator erase_after(const_iterator i, const_iterator j) noexcept {
    while (++copy(i) != j)
      erase_after(i);
    return iterator(j.node());
  }
};
template <class U>
bool operator ==(const ez_forward_list<U> &x, const ez_forward_list<U> &y)
  requires equality_comparable<U> {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j) {
      if (ii == jj)
        return true;
      else
        return false;
    }
    else if (ii == jj)
      return false;

    if (!(*i == *ii))
      return false;
    ++i;
    ++ii;
  }
}
template <class U>
synth_3way_result<U> operator <=>(const ez_forward_list<U> &x,
                                  const ez_forward_list<U> &y) {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j)
      return ii == jj ? strong_eq : strong_lt;
    else if (ii == jj)
      return strong_gt;

    const auto cmp = synth_3way(*i, *ii);
    if (cmp != 0)
      return cmp;
    ++i;
    ++ii;
  }
}

}

// ez_slist
namespace re {

template <class T>
class ez_slist;

namespace inner {

template <class T>
struct ez_slist_node;
template <class T>
struct ez_slist_node_base {
  const ez_slist_node_base *next;
};
template <class T>
struct ez_slist_node : ez_slist_node_base<T> {
  using base_t = ez_slist_node_base<T>;
  T value;

  ez_slist_node() = delete;
  ~ez_slist_node() = default;
  ez_slist_node(const ez_slist_node &) = delete;
  ez_slist_node &operator =(const ez_slist_node &) = delete;
  ez_slist_node(ez_slist_node &&) = delete;
  ez_slist_node &operator =(ez_slist_node &&) = delete;

  ez_slist_node(const base_t *n, const T &x)
    : base_t(n), value(x) {}
  ez_slist_node(const base_t *n, T &&x)
    : base_t(n), value(static_cast<T &&>(x)) {}
};

template <class T>
class ez_slist_iterator {
  template <class>
  friend class ez_slist_iterator;
  template <class>
  friend class re::ez_slist;

  using this_t = ez_slist_iterator;

  using node_base_t = ez_slist_node_base<remove_const_t<T>>;
  using node_t = ez_slist_node<remove_const_t<T>>;
  const node_base_t *p;

public:
  using value_type = remove_const_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = forward_iterator_tag;

  ez_slist_iterator() noexcept : p{} {}
  ~ez_slist_iterator() = default;
  ez_slist_iterator(const this_t &) = default;
  ez_slist_iterator &operator =(const this_t &) = default;
  ez_slist_iterator(this_t &&) = default;
  ez_slist_iterator &operator =(this_t &&) = default;

private:
  explicit ez_slist_iterator(const node_base_t *pp) : p(pp) {}
  const node_base_t *node() const {
    return p;
  }
public:
  operator ez_slist_iterator<add_const_t<T>>() const
    requires (!is_const_v<T>) {
    return ez_slist_iterator<add_const_t<T>>(p);
  }

  T *operator ->() const {
    return addressof(operator *());
  }
  T &operator *() const {
    return static_cast<node_t *>(const_cast<node_base_t *>(p))->value;
  }
  template <class X, class Y>
  friend bool operator ==(const ez_slist_iterator<X> &,
                          const ez_slist_iterator<Y> &)
    requires is_same_v<remove_const_t<X>, remove_const_t<Y>>;

  ez_slist_iterator &operator ++() {
    p = p->next;
    return *this;
  }
  ez_slist_iterator operator ++(int) {
    auto ret = *this;
    operator ++();
    return ret;
  }
};
template <class X, class Y>
bool operator ==(const ez_slist_iterator<X> &x,
                 const ez_slist_iterator<Y> &y)
  requires is_same_v<remove_const_t<X>, remove_const_t<Y>> {
  return x.p == y.p;
}

}

template <class T>
class ez_slist {
  using node_t = inner::ez_slist_node<T>;
  using node_base_t = inner::ez_slist_node_base<T>;

  node_base_t end_node;
  size_t sz;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = inner::ez_slist_iterator<T>;
  using const_iterator = inner::ez_slist_iterator<const T>;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  ez_slist() noexcept {
    end_node.next = nullptr;
    sz = 0;
  }
  ~ez_slist() {
    while (!empty())
      erase_after(before_begin());
  }
  ez_slist(const ez_slist &x) : ez_slist() {
    auto i = before_begin();
    for (const T &z : x)
      i = insert_after(i, z);
    sz = x.sz;
  }
  ez_slist &operator =(const ez_slist &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_slist(ez_slist &&x) noexcept : ez_slist() {
    adl_swap(*this, x);
  }
  ez_slist &operator =(ez_slist &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_slist &x, ez_slist &y) noexcept {
    adl_swap(x.end_node, y.end_node);
    adl_swap(x.sz, y.sz);
  }

  ez_slist(initializer_list<T> l) : ez_slist() {
    auto i = before_begin();
    for (const T &x : l)
      i = insert_after(i, x);
  }

  iterator before_begin() noexcept {
    return iterator(addressof(end_node));
  }
  iterator begin() noexcept {
    return iterator(end_node.next);
  }
  iterator end() noexcept {
    return iterator(nullptr);
  }
  const_iterator before_begin() const noexcept {
    return const_iterator(addressof(end_node));
  }
  const_iterator begin() const noexcept {
    return const_iterator(end_node.next);
  }
  const_iterator end() const noexcept {
    return const_iterator(nullptr);
  }
  bool empty() const noexcept {
    return begin() == end();
  }
  size_t size() const noexcept {
    return sz;
  }
  size_t max_size() const noexcept {
    return integral_traits<make_signed_t<size_t>>::max() / sizeof(value_type);
  }

  iterator new_node(const T &x) {
    node_t *p = new node_t(nullptr, x);
    return iterator(static_cast<const node_base_t *>(p));
  }
  iterator new_node(T &&x) {
    node_t *p = new node_t(nullptr, move(x));
    return iterator(static_cast<const node_base_t *>(p));
  }
  void delete_node(const_iterator i) noexcept {
    node_t *p = static_cast<node_t *>(const_cast<node_base_t *>(i.node()));
    delete p;
  }
  iterator link_after(const_iterator i, iterator n) noexcept {
    node_base_t *it = const_cast<node_base_t *>(i.node());
    node_base_t *next_it = const_cast<node_base_t *>(it->next);
    node_base_t *p = const_cast<node_base_t *>(n.node());
    it->next = p;
    p->next = next_it;
    ++sz;
    return iterator(p);
  }
  iterator unlink_after(const_iterator i) noexcept {
    node_base_t *it = const_cast<node_base_t *>(i.node());
    node_base_t *next_it = const_cast<node_base_t *>(it->next);
    node_base_t *next_next_it = const_cast<node_base_t *>(next_it->next);
    it->next = next_next_it;
    --sz;
    return iterator(next_next_it);
  }
  iterator insert_after(const_iterator i, const T &x) {
    return link_after(i, new_node(x));
  }
  iterator insert_after(const_iterator i, T &&x) {
    return link_after(i, new_node(move(x)));
  }
  iterator erase_after(const_iterator i) noexcept {
    auto x = ++copy(i);
    auto ret = unlink_after(i);
    delete_node(x);
    return ret;
  }
  iterator erase_after(const_iterator i, const_iterator j) noexcept {
    while (++copy(i) != j)
      erase_after(i);
    return iterator(j.node());
  }
};
template <class U>
bool operator ==(const ez_slist<U> &x, const ez_slist<U> &y)
  requires equality_comparable<U> {
  if (x.size() != y.size())
    return false;

  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j) {
      if (ii == jj)
        return true;
      else
        return false;
    }
    else if (ii == jj)
      return false;

    if (!(*i == *ii))
      return false;
    ++i;
    ++ii;
  }
}
template <class U>
synth_3way_result<U> operator <=>(const ez_slist<U> &x,
                                  const ez_slist<U> &y) {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j)
      return ii == jj ? strong_eq : strong_lt;
    else if (ii == jj)
      return strong_gt;

    const auto cmp = synth_3way(*i, *ii);
    if (cmp != 0)
      return cmp;
    ++i;
    ++ii;
  }
}

}

// ez_bidirectional_list
namespace re {

template <class T>
class ez_bidirectional_list;

namespace inner {

template <class T>
struct ez_bd_list_node;
template <class T>
struct ez_bd_list_node_base {
  const ez_bd_list_node_base *next;
  const ez_bd_list_node_base *prev;
};
template <class T>
struct ez_bd_list_node : ez_bd_list_node_base<T> {
  using base_t = ez_bd_list_node_base<T>;
  T value;

  ez_bd_list_node() = delete;
  ~ez_bd_list_node() = default;
  ez_bd_list_node(const ez_bd_list_node &) = delete;
  ez_bd_list_node &operator =(const ez_bd_list_node &) = delete;
  ez_bd_list_node(ez_bd_list_node &&) = delete;
  ez_bd_list_node &operator =(ez_bd_list_node &&) = delete;

  ez_bd_list_node(const base_t *n, const base_t *p, const T &x)
    : base_t(n, p), value(x) {}
  ez_bd_list_node(const base_t *n, const base_t *p, T &&x)
    : base_t(n, p), value(static_cast<T &&>(x)) {}
};

template <class T>
class ez_bd_list_iterator {
  template <class>
  friend class ez_bd_list_iterator;
  template <class>
  friend class re::ez_bidirectional_list;

  using node_base_t = ez_bd_list_node_base<remove_const_t<T>>;
  using node_t = ez_bd_list_node<remove_const_t<T>>;
  const node_base_t *p;

public:
  using value_type = remove_const_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = bidirectional_iterator_tag;

  ez_bd_list_iterator() noexcept : p{} {}
  ~ez_bd_list_iterator() = default;
  ez_bd_list_iterator(const ez_bd_list_iterator &) = default;
  ez_bd_list_iterator &operator =(const ez_bd_list_iterator &) = default;
  ez_bd_list_iterator(ez_bd_list_iterator &&) = default;
  ez_bd_list_iterator &operator =(ez_bd_list_iterator &&) = default;

private:
  explicit ez_bd_list_iterator(const node_base_t *pp) : p(pp) {}
  const node_base_t *node() const {
    return p;
  }
public:
  operator ez_bd_list_iterator<add_const_t<T>>() const
    requires (!is_const_v<T>) {
    return ez_bd_list_iterator<add_const_t<T>>(p);
  }

  T *operator ->() const {
    return addressof(operator *());
  }
  T &operator *() const {
    return static_cast<node_t *>(const_cast<node_base_t *>(p))->value;
  }
  template <class X, class Y>
  friend bool operator ==(const ez_bd_list_iterator<X> &,
                          const ez_bd_list_iterator<Y> &)
    requires is_same_v<remove_const_t<X>, remove_const_t<Y>>;

  ez_bd_list_iterator &operator ++() {
    p = p->next;
    return *this;
  }
  ez_bd_list_iterator operator ++(int) {
    auto ret = *this;
    operator ++();
    return ret;
  }

  ez_bd_list_iterator &operator --() {
    p = p->prev;
    return *this;
  }
  ez_bd_list_iterator operator --(int) {
    auto ret = *this;
    operator --();
    return ret;
  }
};
template <class X, class Y>
bool operator ==(const ez_bd_list_iterator<X> &x,
                 const ez_bd_list_iterator<Y> &y)
  requires is_same_v<remove_const_t<X>, remove_const_t<Y>> {
  return x.p == y.p;
}

}

template <class T>
class ez_bidirectional_list {
  using node_t = inner::ez_bd_list_node<T>;
  using node_base_t = inner::ez_bd_list_node_base<T>;

  node_base_t end_node;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = inner::ez_bd_list_iterator<T>;
  using const_iterator = inner::ez_bd_list_iterator<const T>;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  ez_bidirectional_list() noexcept {
    end_node.next = addressof(end_node);
    end_node.prev = addressof(end_node);
  }
  ~ez_bidirectional_list() {
    for (auto i = begin(); i != end();)
      i = erase(i);
  }
  ez_bidirectional_list(const ez_bidirectional_list &x)
    : ez_bidirectional_list() {
    for (const T &z : x)
      insert(end(), z);
  }
  ez_bidirectional_list &operator =(const ez_bidirectional_list &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_bidirectional_list(ez_bidirectional_list &&x) noexcept
    : ez_bidirectional_list() {
    adl_swap(*this, x);
  }
  ez_bidirectional_list &operator =(ez_bidirectional_list &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_bidirectional_list &x,
                   ez_bidirectional_list &y) noexcept {
    if (!x.empty()) {
      if (!y.empty()) {
        adl_swap(x.end_node, y.end_node);

        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));
        node_base_t *x_next = const_cast<node_base_t *>(x_ed->next);
        node_base_t *x_prev = const_cast<node_base_t *>(x_ed->prev);
        x_prev->next = x_ed;
        x_next->prev = x_ed;

        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));
        node_base_t *y_next = const_cast<node_base_t *>(y_ed->next);
        node_base_t *y_prev = const_cast<node_base_t *>(y_ed->prev);
        y_prev->next = y_ed;
        y_next->prev = y_ed;
      }
      else {
        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));
        node_base_t *x_next = const_cast<node_base_t *>(x_ed->next);
        node_base_t *x_prev = const_cast<node_base_t *>(x_ed->prev);

        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));

        y_ed->next = x_next;
        x_next->prev = y_ed;
        y_ed->prev = x_prev;
        x_prev->next = y_ed;

        x_ed->next = x_ed;
        x_ed->prev = x_ed;
      }
    }
    else {
      if (!y.empty()) {
        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));
        node_base_t *y_next = const_cast<node_base_t *>(y_ed->next);
        node_base_t *y_prev = const_cast<node_base_t *>(y_ed->prev);

        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));

        x_ed->next = y_next;
        y_next->prev = x_ed;
        x_ed->prev = y_prev;
        y_prev->next = x_ed;

        y_ed->next = y_ed;
        y_ed->prev = y_ed;
      }
    }
  }

  ez_bidirectional_list(initializer_list<T> l) : ez_bidirectional_list() {
    for (const T &x : l)
      insert(end(), x);
  }

  iterator begin() noexcept {
    return iterator(end_node.next);
  }
  iterator end() noexcept {
    return iterator(addressof(end_node));
  }
  const_iterator begin() const noexcept {
    return const_iterator(end_node.next);
  }
  const_iterator end() const noexcept {
    return const_iterator(addressof(end_node));
  }
  bool empty() const noexcept {
    return begin() == end();
  }
  size_t max_size() const noexcept {
    return integral_traits<make_signed_t<size_t>>::max() / sizeof(value_type);
  }

  iterator new_node(const T &x) {
    node_t *p = new node_t(nullptr, nullptr, x);
    return iterator(static_cast<const node_base_t *>(p));
  }
  iterator new_node(T &&x) {
    node_t *p = new node_t(nullptr, nullptr, move(x));
    return iterator(static_cast<const node_base_t *>(p));
  }
  void delete_node(const_iterator i) noexcept {
    node_t *p = static_cast<node_t *>(const_cast<node_base_t *>(i.node()));
    delete p;
  }
  iterator link(const_iterator i, iterator n) noexcept {
    node_base_t *y = const_cast<node_base_t *>(i.node());
    node_base_t *x = const_cast<node_base_t *>(y->prev);
    node_base_t *p = const_cast<node_base_t *>(n.node());
    p->next = y;
    p->prev = x;
    x->next = p;
    y->prev = p;
    return iterator(p);
  }
  iterator unlink(const_iterator i) noexcept {
    node_base_t *p = const_cast<node_base_t *>(i.node());
    node_base_t *x = const_cast<node_base_t *>(p->prev);
    node_base_t *y = const_cast<node_base_t *>(p->next);
    x->next = y;
    y->prev = x;
    return iterator(y);    
  }
  iterator insert(const_iterator i, const T &x) {
    return link(i, new_node(x));
  }
  iterator insert(const_iterator i, T &&x) {
    return link(i, new_node(move(x)));
  }
  iterator erase(const_iterator i) noexcept {
    auto ret = unlink(i);
    delete_node(i);
    return ret;
  }
  iterator erase(const_iterator i, const_iterator j) noexcept {
    for (const_iterator k = i; k != j;)
      k = erase(k);
    return iterator(j.node());
  }
};
template <class U>
bool operator ==(const ez_bidirectional_list<U> &x,
                 const ez_bidirectional_list<U> &y)
  requires equality_comparable<U> {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j) {
      if (ii == jj)
        return true;
      else
        return false;
    }
    else if (ii == jj)
      return false;

    if (!(*i == *ii))
      return false;
    ++i;
    ++ii;
  }
}
template <class U>
synth_3way_result<U> operator <=>(const ez_bidirectional_list<U> &x,
                                  const ez_bidirectional_list<U> &y) {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j)
      return ii == jj ? strong_eq : strong_lt;
    else if (ii == jj)
      return strong_gt;

    const auto cmp = synth_3way(*i, *ii);
    if (cmp != 0)
      return cmp;
    ++i;
    ++ii;
  }
}

}

// ez_list
namespace re {

template <class T>
class ez_list;

namespace inner {

template <class T>
struct ez_list_node;
template <class T>
struct ez_list_node_base {
  const ez_list_node_base *next;
  const ez_list_node_base *prev;
};
template <class T>
struct ez_list_node : ez_list_node_base<T> {
  using base_t = ez_list_node_base<T>;
  T value;

  ez_list_node() = delete;
  ~ez_list_node() = default;
  ez_list_node(const ez_list_node &) = delete;
  ez_list_node &operator =(const ez_list_node &) = delete;
  ez_list_node(ez_list_node &&) = delete;
  ez_list_node &operator =(ez_list_node &&) = delete;

  ez_list_node(const base_t *n, const base_t *p, const T &x)
    : base_t(n, p), value(x) {}
  ez_list_node(const base_t *n, const base_t *p, T &&x)
    : base_t(n, p), value(static_cast<T &&>(x)) {}
};

template <class T>
class ez_list_iterator {
  template <class>
  friend class ez_list_iterator;
  template <class>
  friend class re::ez_list;

  using node_base_t = ez_list_node_base<remove_const_t<T>>;
  using node_t = ez_list_node<remove_const_t<T>>;
  const node_base_t *p;

public:
  using value_type = remove_const_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = bidirectional_iterator_tag;

  ez_list_iterator() noexcept : p{} {}
  ~ez_list_iterator() = default;
  ez_list_iterator(const ez_list_iterator &) = default;
  ez_list_iterator &operator =(const ez_list_iterator &) = default;
  ez_list_iterator(ez_list_iterator &&) = default;
  ez_list_iterator &operator =(ez_list_iterator &&) = default;

private:
  explicit ez_list_iterator(const node_base_t *pp) : p(pp) {}
  const node_base_t *node() const {
    return p;
  }
public:
  operator ez_list_iterator<add_const_t<T>>() const
    requires (!is_const_v<T>) {
    return ez_list_iterator<add_const_t<T>>(p);
  }

  T *operator ->() const {
    return addressof(operator *());
  }
  T &operator *() const {
    return static_cast<node_t *>(const_cast<node_base_t *>(p))->value;
  }
  template <class X, class Y>
  friend bool operator ==(const ez_list_iterator<X> &,
                          const ez_list_iterator<Y> &)
    requires is_same_v<remove_const_t<X>, remove_const_t<Y>>;

  ez_list_iterator &operator ++() {
    p = p->next;
    return *this;
  }
  ez_list_iterator operator ++(int) {
    auto ret = *this;
    operator ++();
    return ret;
  }

  ez_list_iterator &operator --() {
    p = p->prev;
    return *this;
  }
  ez_list_iterator operator --(int) {
    auto ret = *this;
    operator --();
    return ret;
  }
};
template <class X, class Y>
bool operator ==(const ez_list_iterator<X> &x,
                 const ez_list_iterator<Y> &y)
  requires is_same_v<remove_const_t<X>, remove_const_t<Y>> {
  return x.p == y.p;
}

}

template <class T>
class ez_list {
  using node_t = inner::ez_list_node<T>;
  using node_base_t = inner::ez_list_node_base<T>;

  node_base_t end_node;
  size_t sz;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

  using iterator = inner::ez_list_iterator<T>;
  using const_iterator = inner::ez_list_iterator<const T>;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  ez_list() noexcept : sz(0) {
    end_node.next = addressof(end_node);
    end_node.prev = addressof(end_node);
  }
  ~ez_list() {
    for (auto i = begin(); i != end();)
      i = erase(i);
  }
  ez_list(const ez_list &x) : ez_list() {
    for (const T &z : x)
      insert(end(), z);
  }
  ez_list &operator =(const ez_list &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  ez_list(ez_list &&x) noexcept : ez_list() {
    adl_swap(*this, x);
  }
  ez_list &operator =(ez_list &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_list &x, ez_list &y) noexcept {
    if (!x.empty()) {
      if (!y.empty()) {
        adl_swap(x.end_node, y.end_node);

        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));
        node_base_t *x_next = const_cast<node_base_t *>(x_ed->next);
        node_base_t *x_prev = const_cast<node_base_t *>(x_ed->prev);
        x_prev->next = x_ed;
        x_next->prev = x_ed;

        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));
        node_base_t *y_next = const_cast<node_base_t *>(y_ed->next);
        node_base_t *y_prev = const_cast<node_base_t *>(y_ed->prev);
        y_prev->next = y_ed;
        y_next->prev = y_ed;
      }
      else {
        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));
        node_base_t *x_next = const_cast<node_base_t *>(x_ed->next);
        node_base_t *x_prev = const_cast<node_base_t *>(x_ed->prev);

        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));

        y_ed->next = x_next;
        x_next->prev = y_ed;
        y_ed->prev = x_prev;
        x_prev->next = y_ed;

        x_ed->next = x_ed;
        x_ed->prev = x_ed;
      }
    }
    else {
      if (!y.empty()) {
        node_base_t *y_ed = addressof(const_cast<node_base_t &>(y.end_node));
        node_base_t *y_next = const_cast<node_base_t *>(y_ed->next);
        node_base_t *y_prev = const_cast<node_base_t *>(y_ed->prev);

        node_base_t *x_ed = addressof(const_cast<node_base_t &>(x.end_node));

        x_ed->next = y_next;
        y_next->prev = x_ed;
        x_ed->prev = y_prev;
        y_prev->next = x_ed;

        y_ed->next = y_ed;
        y_ed->prev = y_ed;
      }
    }
    adl_swap(x.sz, y.sz);
  }

  ez_list(initializer_list<T> l) : ez_list() {
    for (const T &x : l)
      insert(end(), x);
  }

  iterator begin() noexcept {
    return iterator(end_node.next);
  }
  iterator end() noexcept {
    return iterator(addressof(end_node));
  }
  const_iterator begin() const noexcept {
    return const_iterator(end_node.next);
  }
  const_iterator end() const noexcept {
    return const_iterator(addressof(end_node));
  }
  bool empty() const noexcept {
    return begin() == end();
  }
  size_t size() const noexcept {
    return sz;
  }
  size_t max_size() const noexcept {
    return integral_traits<make_signed_t<size_t>>::max() / sizeof(value_type);
  }

  iterator new_node(const T &x) {
    node_t *p = new node_t(nullptr, nullptr, x);
    return iterator(static_cast<const node_base_t *>(p));
  }
  iterator new_node(T &&x) {
    node_t *p = new node_t(nullptr, nullptr, move(x));
    return iterator(static_cast<const node_base_t *>(p));
  }
  void delete_node(const_iterator i) noexcept {
    node_t *p = static_cast<node_t *>(const_cast<node_base_t *>(i.node()));
    delete p;
  }
  iterator link(const_iterator i, iterator n) noexcept {
    node_base_t *y = const_cast<node_base_t *>(i.node());
    node_base_t *x = const_cast<node_base_t *>(y->prev);
    node_base_t *p = const_cast<node_base_t *>(n.node());
    p->next = y;
    p->prev = x;
    x->next = p;
    y->prev = p;
    ++sz;
    return iterator(p);
  }
  iterator unlink(const_iterator i) noexcept {
    node_base_t *p = const_cast<node_base_t *>(i.node());
    node_base_t *x = const_cast<node_base_t *>(p->prev);
    node_base_t *y = const_cast<node_base_t *>(p->next);
    x->next = y;
    y->prev = x;
    --sz;
    return iterator(y);    
  }
  iterator insert(const_iterator i, const T &x) {
    return link(i, new_node(x));
  }
  iterator insert(const_iterator i, T &&x) {
    return link(i, new_node(move(x)));
  }
  iterator erase(const_iterator i) noexcept {
    auto ret = unlink(i);
    delete_node(i);
    return ret;
  }
  iterator erase(const_iterator i, const_iterator j) noexcept {
    for (const_iterator k = i; k != j;)
      k = erase(k);
    return iterator(j.node());
  }
};
template <class U>
bool operator ==(const ez_list<U> &x, const ez_list<U> &y)
  requires equality_comparable<U> {
  if (x.size() != y.size())
    return false;
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  while (i != j) {
    if (!(*i == *ii))
      return false;
    ++i;
    ++ii;
  }
  return true;
}
template <class U>
synth_3way_result<U> operator <=>(const ez_list<U> &x,
                                  const ez_list<U> &y) {
  auto i = x.begin();
  const auto j = x.end();
  auto ii = y.begin();
  const auto jj = y.end();
  for (;;) {
    if (i == j)
      return ii == jj ? strong_eq : strong_lt;
    else if (ii == jj)
      return strong_gt;

    const auto cmp = synth_3way(*i, *ii);
    if (cmp != 0)
      return cmp;
    ++i;
    ++ii;
  }
}

}

// ez_map
namespace re {

namespace inner {

template <class K, class M>
struct ez_map_node : pair<K, M> {
  ez_map_node *p;
  ez_map_node *l;
  ez_map_node *r;

  ez_map_node() = default;
  ~ez_map_node() = default;
  ez_map_node(const ez_map_node &) = delete;
  ez_map_node &operator =(const ez_map_node &) = delete;
  ez_map_node(ez_map_node &&) = delete;
  ez_map_node &operator =(ez_map_node &&) = delete;

  template <class...S>
  ez_map_node(S &&...s) : pair<K, M>(forward<S>(s)...) {}
};

}
template <class K, class M>
class ez_map {
  using node_t = inner::ez_map_node<K, M>;
  node_t *root;
  size_t sz;

  static node_t *make_node() {
    return new node_t{};
  }
  static node_t *make_node(const K &k) {
    return new node_t(piecewise_construct,
                      forward_as_tuple(k),
                      forward_as_tuple());
  }
  static void destroy_node(node_t *n) {
    delete n;
  }

  void insert_to_null(node_t *n) {
    n->p = nullptr;
    n->l = nullptr;
    n->r = nullptr;
    root = n;
    ++sz;
  }
  void insert_to_left(node_t *n, node_t *p) {
    p->l = n;
    n->p = p;
    n->l = nullptr;
    n->r = nullptr;
    ++sz;
  }
  void insert_to_right(node_t *n, node_t *p) {
    p->r = n;
    n->p = p;
    n->l = nullptr;
    n->r = nullptr;
    ++sz;
  }
  template <class UPRED>
  node_t *partition_point(UPRED eq) const {
    node_t *ret = nullptr;
    node_t *i = root;
    for (;;) {
      if (i == nullptr)
        return ret;
      if (eq(*i))
        i = i->r;
      else {
        ret = i;
        i = i->l;
      }
    }
  }
  node_t *lower_bound(const K &k) {
    return partition_point([k](const node_t &n) {return n.first < k;});
  }

public:
  ez_map() : root(nullptr), sz(0) {}
  ~ez_map() {
    clear();
  }
  ez_map(const ez_map &m) {
    root = nullptr;
    sz = 0;
    if (m.root != nullptr) {
      node_t *i = m.root;
      while (i->l != nullptr)
        i = i->l;
      while (i != nullptr) {
        operator [](i->first) = i->second;

        if (i->r != nullptr) {
          i = i->r;
          while (i->l != nullptr)
            i = i->l;
        }
        else {
          while (i->p != nullptr && i == i->p->r)
            i = i->p;
          i = i->p;
        }
      }
    }
  }
  ez_map &operator =(const ez_map &m) {
    copy_and_swap(m, *this);
    return *this;
  }
  ez_map(ez_map &&x) noexcept : root(x.root), sz(x.sz) {
    x.root = nullptr;
    x.sz = 0;
  }
  ez_map &operator =(ez_map &&x) noexcept {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(ez_map &x, ez_map &y) noexcept {
    adl_swap(x.root, y.root);
    adl_swap(x.sz, y.sz);
  }

  M &operator [](const K &key) {
    if (root == nullptr) {
      node_t *new_node = make_node(key);
      insert_to_null(new_node);
      return new_node->second;
    }

    node_t *i = lower_bound(key);
    if (i == nullptr) {
      node_t *new_node = make_node(key);
      node_t *rightmost = root;
      while (rightmost->r != nullptr)
        rightmost = rightmost->r;
      insert_to_right(new_node, rightmost);
      return new_node->second;
    }
    else if (key < i->first) {
      node_t *new_node = make_node(key);
      if (i->l == nullptr)
        insert_to_left(new_node, i);
      else {
        i = i->l;
        while (i->r != nullptr)
          i = i->r;
        insert_to_right(new_node, i);
      }
      return new_node->second;
    }
    else
      return i->second;
  }
  pair<K, M> *find(const K &key) const {
    node_t *i = root;
    while (i != nullptr) {
      if (key < i->first)
        i = i->l;
      else if (i->first < key)
        i = i->r;
      else
        return i;
    }
    return nullptr;
  }
  pair<K, M> *end() const noexcept {
    return nullptr;
  }
  void erase(pair<K, M> *i) noexcept {
    --sz;
    const node_t *const n = static_cast<node_t *>(i);
    if (n->l == nullptr) {
      if (n->r == nullptr) {
        if (n->p == nullptr)
          root = nullptr;
        else {
          if (n == n->p->l)
            n->p->l = nullptr;
          else
            n->p->r = nullptr;
        }
      }
      else {
        if (n->p == nullptr)
          root = n->r;
        else {
          if (n == n->p->l)
            n->p->l = n->r;
          else
            n->p->r = n->r;
        }
        n->r->p = n->p;
      }
    }
    else {
      if (n->r == nullptr) {
        if (n->p == nullptr)
          root = n->l;
        else {
          if (n == n->p->l)
            n->p->l = n->l;
          else
            n->p->r = n->l;
        }
        n->l->p = n->p;
      }
      else {
        auto next_n = n->r;
        while (next_n->l != nullptr)
          next_n = next_n->l;
        if (next_n == n->r) {
          if (n->p == nullptr)
            root = next_n;
          else {
            if (n == n->p->l)
              n->p->l = next_n;
            else
              n->p->r = next_n;
          }
          next_n->p = n->p;

          next_n->l = n->l;
          n->l->p = next_n;
        }
        else {
          const auto next_n_p = next_n->p;

          next_n_p->l = next_n->r;
          if (next_n->r != nullptr)
            next_n->r->p = next_n_p;

          if (n->p == nullptr)
            root = next_n;
          else {
            if (n == n->p->r)
              n->p->r = next_n;
            else
              n->p->l = next_n;
          }
          next_n->p = n->p;

          next_n->l = n->l;
          n->l->p = next_n;

          next_n->r = n->r;
          n->r->p = next_n;
        }
      }
    }
    destroy_node(const_cast<node_t *>(n));
  }
  void erase(const K &key) {
    remove(key);
  }
  bool remove(const K &key) {
    const auto i = find(key);
    if (i != end()) {
      erase(i);
      return true;
    }
    else
      return false;
  }
  bool empty() const noexcept {
    return root == nullptr;
  }
  size_t size() const noexcept {
    return sz;
  }
  void clear() noexcept {
    while (root != nullptr)
      erase(root);
    sz = 0;
  }
};

}

// test_allocator
// stateful_test_allocator
// test_object
namespace re {

// inner::alloc_log
namespace inner {

template <template <class, class> class MAP>
struct alloc_log {
  struct alloc_record {
    type_index type = typeid(void);
    size_t size = 0;
  };
  MAP<void *, alloc_record> m;
  MAP<void *, size_t> m_zero;

  alloc_log() = default;
  ~alloc_log() {
    if (!empty())
      print_then_abort("error: re::alloc_log: memory leak\n");
  }
  alloc_log(const alloc_log &) = default;
  alloc_log &operator =(const alloc_log &) = default;
  alloc_log(alloc_log &&) = default;
  alloc_log &operator =(alloc_log &&) = default;

  // allocate(p, n)
  //   requires: p != nullptr
  template <class T>
  void allocate(T *p, size_t n) {
    if (n == 0) {
      if (++m_zero[p] == numeric_limits<size_t>::max())
        print_then_abort
          ("error: re::alloc_log: zero allocation counter overflow\n");
      return;
    }

    if (m.find(p) != m.end())
      print_then_abort
        ("error: re::alloc_log: "
         "two allocation operations share the one address\n");

    m[p] = {typeid(T), n};
  }
  template <class T>
  void deallocate(T *p, size_t n) {
    if (n == 0) {
      auto &c = m_zero[p];
      if (c == 0)
        print_then_abort
          ("error: re::alloc_log: "
           "deallocate unexisted zero allocated pointer\n");
      --c;
      if (c == 0)
        m_zero.remove(p);
      return;
    }

    const auto i = m.find(p);
    if (i == m.end())
      print_then_abort
        ("error: re::inner::alloc_log: unexisted address to deallocate\n");

    if (i->second.size != n)
      print_then_abort
        ("error: re::inner::alloc_log: dismatched size to deallocate\n");

    if (i->second.type != typeid(T) && n != 1)
      print_then_abort
        ("error: re::inner::alloc_log: "
         "dismatched pointer type to deallocate\n");

    m.erase(i);
  }
  bool empty() const noexcept {
    return m.empty() && m.size() == 0 && m_zero.empty();
  }
  auto size() const noexcept {
    return m.size();
  }
};

}

namespace inner {

template <template <class, class> class MAP>
struct test_allocator_base {
  static inner::alloc_log<MAP> *p;

  test_allocator_base() noexcept {
    static inner::alloc_log<MAP> l;
    p = addressof(l);
  }
  ~test_allocator_base() = default;
  test_allocator_base(const test_allocator_base &) = default;
  test_allocator_base &operator =(const test_allocator_base &) = default;
  test_allocator_base(test_allocator_base &&) = default;
  test_allocator_base &operator =(test_allocator_base &&) = default;
};
template <template <class, class> class MAP>
inline inner::alloc_log<MAP> *test_allocator_base<MAP>::p = nullptr;

}
template <class T, template <class, class> class MAP = ez_map>
class test_allocator : inner::test_allocator_base<MAP> {
  using base_t = inner::test_allocator_base<MAP>;

public:
  using value_type = T;
  using size_type = size_t;
  size_type max_size() const {
    return to_unsigned(integral_traits<ptrdiff_t>::max()) / sizeof(value_type);
  }
  size_type min_alignment() const {
    return __STDCPP_DEFAULT_NEW_ALIGNMENT__;
  }
  template <class U>
  struct rebind {
    using other = test_allocator<U, MAP>;
  };

  template <class U>
  operator test_allocator<U, MAP>() const noexcept
    requires (!same_as<U, T>) {
    return {};
  }

  template <class U>
  constexpr bool operator ==(const test_allocator<U, MAP> &)
    const noexcept {
    return true;
  }

  T *allocate(size_t n) {
    static_assert(alignof(T) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    if (n > integral_traits<size_t>::max() / sizeof(T))
      print_then_abort
        ("re::test_allocator: too big size to allocate\n");

    T *const p = reinterpret_cast<T *>(operator new(n * sizeof(T)));
    if (p == nullptr)
      throw_or_abort<bad_alloc>();
    base_t::p->allocate(p, n);
    return p;
  }
  void deallocate(T *p, size_t n) noexcept {
    base_t::p->deallocate(p, n);
    operator delete(p);
  }

  static bool empty() noexcept {
    return base_t::p->empty();
  }
  size_type size() const noexcept {
    return base_t::p->size();
  }
};

namespace inner {

struct scalloc_leak_guard {
  size_t c;

  scalloc_leak_guard() : c(0) {}
  ~scalloc_leak_guard() {
    if (c != 0)
      print_then_abort("re::stateful_test_allocator: leak\n");
  }
  scalloc_leak_guard(const scalloc_leak_guard &) = delete;
  scalloc_leak_guard &operator =(const scalloc_leak_guard &) = delete;
  scalloc_leak_guard(scalloc_leak_guard &&) = delete;
  scalloc_leak_guard &operator =(scalloc_leak_guard &&) = delete;

  void plus() {
    if (c == integral_traits<size_t>::max())
      print_then_abort
        ("re::stateful_test_allocator: too many overall instances\n");
    ++c;
  }
  void minus() {
    if (c == 0)
      print_then_abort
        ("re::stateful_test_allocator: negative overall reference count\n");
    --c;
  }
};
namespace fns {

inline scalloc_leak_guard &get_scalloc_leak_guard() {
  static scalloc_leak_guard g;
  return g;
}

}

}
template <class T, template <class, class> class MAP = ez_map,
          bool COPY_PRPG = true, bool MOVE_PRPG = true,
          bool SWAP_PRPG = true, bool ALWAYS_EQUAL = false>
class stateful_test_allocator {
  template <class U, template <class, class> class, bool, bool, bool, bool>
  friend class stateful_test_allocator;

  using this_t = stateful_test_allocator;

  pair<inner::alloc_log<MAP>, size_t> *p;

  void plus() {
    if (p->second == integral_traits<size_t>::max())
      print_then_abort("re::stateful_test_allocator: too many instances\n");
    ++p->second;
    inner::fns::get_scalloc_leak_guard().plus();
  }
  void minus() {
    if (p->second == 0)
      print_then_abort
        ("re::stateful_test_allocator: negative reference count\n");
    if (--p->second == 0) {
      if (!p->first.empty())
        print_then_abort("re::stateful_test_allocator: memory leak\n");
      delete p;
    }
    inner::fns::get_scalloc_leak_guard().minus();
  }

  void refer_to(pair<inner::alloc_log<MAP>, size_t> *pp) {
    p = pp;
    plus();
  }

public:
  using value_type = T;
  using size_type = size_t;
  size_type max_size() const {
    return to_unsigned(integral_traits<ptrdiff_t>::max()) / sizeof(value_type);
  }
  size_type min_alignment() const {
    return __STDCPP_DEFAULT_NEW_ALIGNMENT__;
  }
  template <class U>
  struct rebind {
    using other = stateful_test_allocator
      <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, ALWAYS_EQUAL>;
  };
  using propagate_on_container_copy_assignment = bool_constant<COPY_PRPG>;
  using propagate_on_container_move_assignment = bool_constant<MOVE_PRPG>;
  using propagate_on_container_swap = bool_constant<SWAP_PRPG>;
  using is_always_equal = bool_constant<ALWAYS_EQUAL>;

  stateful_test_allocator() {
    refer_to(new pair<inner::alloc_log<MAP>, size_t>);
  }
  ~stateful_test_allocator() {
    minus();
  }
  stateful_test_allocator(const this_t &x) noexcept {
    refer_to(x.p);
  }
  stateful_test_allocator &operator =(const this_t &x) noexcept {
    if (addressof(x) != this) {
      minus();
      if (x.p == nullptr)
        print_then_abort
          ("re::stateful_test_allocator: nullptr assignment\n");
      refer_to(x.p);
    }
    return *this;
  }
  stateful_test_allocator(this_t &&x) noexcept
    : stateful_test_allocator(x) {}
  stateful_test_allocator &operator =(this_t &&x) noexcept {
    return operator =(x);
  }

  template <class U>
  bool operator ==
  (const stateful_test_allocator
   <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, ALWAYS_EQUAL> &x)
    const noexcept {
    return p == x.p;
  }

  template <class U>
  stateful_test_allocator
  (const stateful_test_allocator
   <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, ALWAYS_EQUAL> &x) noexcept
    requires (!same_as<U, T>) {
    refer_to(x.p);
  }

  T *allocate(size_t n) {
    static_assert(alignof(T) <= __STDCPP_DEFAULT_NEW_ALIGNMENT__);
    if (n > integral_traits<size_t>::max() / sizeof(T))
      print_then_abort
        ("re::test_allocator: too big size to allocate\n");

    T *const pp = reinterpret_cast<T *>(operator new(n * sizeof(T)));
    if (pp == nullptr)
      throw_or_abort<bad_alloc>();
    p->first.allocate(pp, n);
    return pp;
  }
  void deallocate(T *pp, size_t n) noexcept {
    p->first.deallocate(pp, n);
    operator delete(pp);
  }

  bool empty() const noexcept {
    return p->first.empty();
  }
  size_type size() const noexcept {
    return p->first.size();
  }
};
template <class T, template <class, class> class MAP,
          bool COPY_PRPG, bool MOVE_PRPG, bool SWAP_PRPG>
class stateful_test_allocator<T, MAP,
                              COPY_PRPG, MOVE_PRPG, SWAP_PRPG, true>
  : test_allocator<T, MAP> {
  using base_t = test_allocator<T, MAP>;

public:
  using value_type = T;  
  template <class U>
  struct rebind {
    using other = stateful_test_allocator
      <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, true>;
  };
  using propagate_on_container_copy_assignment = bool_constant<COPY_PRPG>;
  using propagate_on_container_move_assignment = bool_constant<MOVE_PRPG>;
  using propagate_on_container_swap = bool_constant<SWAP_PRPG>;

  template <class U>
  operator stateful_test_allocator
    <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, true>() const noexcept
    requires (!same_as<U, T>) {
    return {};
  }

  template <class U>
  constexpr bool operator ==
    (const stateful_test_allocator
     <U, MAP, COPY_PRPG, MOVE_PRPG, SWAP_PRPG, true> &) const noexcept {
    return true;
  }

  using base_t::allocate;
  using base_t::deallocate;
  using base_t::empty;
};

template <class T, template <class, class> class MAP = ez_map>
class test_object {
  using alloc_t = test_allocator<T, MAP>;

  T *p;

  template <class...S>
  static T *make(S &&...s) {
    alloc_t a;
    T *p = a.allocate(1);
#ifndef RE_NOEXCEPT
    try {
      ::new(p) T(forward<S>(s)...);
    }
    catch (...) {
      a.deallocate(p, 1);
    }
#else
    ::new(p) T(forward<S>(s)...);
#endif
    return p;
  }
  static void destroy(T *p) noexcept {
    alloc_t a;
    p->~T();
    a.deallocate(p, 1);
  }

public:
  test_object() : p(make()) {}
  ~test_object() {
    destroy(p);
  }
  test_object(const test_object &x) : p(make(*x.p)) {}
  test_object &operator =(const test_object &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  test_object(test_object &&x) {
    auto pp = make();
    p = x.p;
    x.p = pp;
  }
  test_object &operator =(test_object &&x) {
    copy_and_swap(move(x), *this);
    return *this;
  }
  friend void swap(test_object &x, test_object &y) noexcept {
    adl_swap(x.p, y.p);
  }

  template <class X>
  explicit(!is_same_v<decay_t<X>, test_object>
           && is_constructible_v<T, X &&> && !is_convertible_v<X &&, T>)
  test_object(X &&x)
    requires (!is_same_v<decay_t<X>, test_object>
              && is_constructible_v<T, X &&>)
    : p(make(forward<X>(x))) {}
  template <class X>
  test_object &operator =(X &&x)
    requires ((!is_same_v<decay_t<X>, test_object>)
              && is_assignable_v<T &, X &&>) {
    *p = forward<X>(x);
    return *this;
  }

  T *operator ->() noexcept {
    return p;
  }
  const T *operator ->() const noexcept {
    return p;
  }

  T &operator *() noexcept {
    return *p;
  }
  const T &operator *() const noexcept {
    return *p;
  }

  operator T &() noexcept {
    return *p;
  }
  operator const T &() const noexcept {
    return *p;
  }
};

}

// test_fitr
// test_bitr
// test_ritr
// test_rng
// test_itr_minus
namespace re {

namespace inner::fns {

template <class I, class J>
constexpr void test_iitr_impl_impl(I i, J j, const char *errs) {}
template <class I, class J>
constexpr void test_iitr_impl_impl(I i, J j, const char *errs)
  requires is_same_v<I, J> {
  I x = i;
  I y = j;
  adl_swap(x, y);
  if (!(x == j && y == i))
    print_then_abort(errs, "failed at step 0-2\n");
}
template <class I, class J, class IT, class EQ>
constexpr void test_iitr_impl(I i, J j, IT it1, IT it2, EQ eq,
                              const char *errs) {
  {
    I x = i;
    adl_swap(x, x);
    if (!(x == i))
      print_then_abort(errs, " failed at step 0-0\n");

    J y = j;
    adl_swap(y, y);
    if (!(y == j))
      print_then_abort(errs, " failed at step 0-1\n");

    inner::fns::test_iitr_impl_impl(i, j, errs);
  }

  auto ii = i;
  const auto jj = j;
  auto a_i = it1;
  const auto a_j = it2;
  for (; a_i != a_j; ++ii, (void)++a_i) {
    if (!test_equal(ii, ii))
      print_then_abort(errs, " failed at step 1-0\n");
    if (test_equal(ii, jj))
      print_then_abort(errs, " failed at step 1-1\n");
    if (!eq(*ii, *a_i))
      print_then_abort(errs, " failed at step 1-2\n");
  }
  if (!test_equal(ii, jj))
    print_then_abort(errs, " failed at step 1-3\n");
}

template <class I, class J, class IT, class EQ>
constexpr void test_fitr_impl0(I i, J j, IT it1, IT it2, EQ eq,
                               const char *errs) {
  {
    I ii = i;
    J jj = j;
    for (; ii != jj; ++ii) {
      for (auto x = i; x != ii; ++x)
        if (!test_unequal(x, ii))
          print_then_abort(errs, " failed at step 1-4\n");
      for (auto x = ii; x != jj;) {
        ++x;
        if (!test_unequal(x, ii))
          print_then_abort(errs, " failed at step 1-5\n");
      }
    }
  }
  {
    I kk = i;
    if (i != j) {
      I ii = i;
      if (!(ii++ == i))
        print_then_abort(errs, " failed at step 2-0\n");
      if (!test_equal(++kk, ii))
        print_then_abort(errs, " failed at step 2-1\n");
    }
  }
}
template <class I, class J, class IT, class EQ>
constexpr void test_fitr_impl(I i, J j, IT it1, IT it2, EQ eq,
                              const char *errs) {
  inner::fns::test_iitr_impl(i, j, it1, it2, eq, errs);
  inner::fns::test_fitr_impl0(i, j, it1, it2, eq, errs);  
}

template <class I, class J, class IT, class EQ>
constexpr void test_bitr_impl0(I i, J j, IT it1, IT it2, EQ eq,
                               const char *errs) {
  I x = i;
  I y = i;
  adl_swap(x, x);
  if (!(x == i))
    print_then_abort("re::test_bitr(...) failed at step 00-0\n");

  if (i != j) {
    ++x;
    adl_swap(x, y);
    if (!(x == i))
      print_then_abort("re::test_bitr(...) failed at step 00-1\n");
    ++x;
    if (!(x == y))
      print_then_abort("re::test_bitr(...) failed at step 00-2\n");

    J z = j;
    adl_swap(z, z);
    if (!(z == j))
      print_then_abort("re::test_bitr(...) failed at step 00-3\n");

    J zz = j;
    --zz;
    adl_swap(z, zz);
    if (!(zz == j))
      print_then_abort("re::test_bitr(...) failed at step 00-4\n");
    ++z;
    if (!(z == zz))
      print_then_abort("re::test_bitr(...) failed at step 00-5\n");
  }
}
template <class I, class J, class IT, class EQ>
constexpr void test_bitr_impl00(I i, J j, IT it1, IT it2, EQ eq,
                                const char *errs) {
  {
    I ii = i;
    J jj = j;
    IT a_i = it1;
    IT a_j = it2;
    while (a_i != a_j) {
      --a_j;
      if (test_equal(ii, jj))
        print_then_abort(errs, " failed at step 3-0\n");
      --jj;
      if (!(eq(*jj, *a_j)))
        print_then_abort(errs, " failed at step 3-1\n");
    }
    if (!test_equal(ii, jj))
      print_then_abort(errs, " failed at step 3-2\n");
  }

  if (i != j) {
    auto iii = i;
    auto kkk = i;
    ++iii;
    ++kkk;
    if (!(iii-- == kkk))
      print_then_abort(errs, " failed at step 4-0\n");
    if (!test_equal(--kkk, iii))
      print_then_abort(errs, " failed at step 4-1\n");

    auto jjj = j;
    J kkkk = j;
    if (!test_equal(jjj--, j))
      print_then_abort(errs, " failed at step 4-2\n");
    if (!test_equal(--kkkk, jjj))
      print_then_abort(errs, " failed at step 4-3\n");
  }
}
template <class I, class J, class IT, class EQ>
constexpr void test_bitr_impl(I i, J j, IT it1, IT it2, EQ eq,
                              const char *errs) {
  inner::fns::test_bitr_impl0(i, j, it1, it2, eq, errs);
  inner::fns::test_fitr_impl(i, j, it1, it2, eq, errs);
  inner::fns::test_bitr_impl00(i, j, it1, it2, eq, errs);
}

template <class I, class J, class IT, class EQ>
constexpr void test_ritr_impl0(I i, J j, IT it1, IT it2, EQ eq,
                               const char *errs) {
  const auto sz = it2 - it1;

  const auto test_equal = [](auto a, auto b) {
    return synth_3way(a, b) == 0 && a == b;
  };
  I ii = i;
  using dft = decay_t<decltype(sz)>;
  for (dft k = 0; k <= sz; ++k) {
    ii = i + k;
    if (!(test_equal(i + k, ii)
          && test_equal(i - (-k), ii)
          && i - ii == -k
          && ii - i == k
          && test_equal(i, ii - k)
          && test_equal(i, ii + (-k))))
      print_then_abort(errs, " failed at step 5-0\n");
    for (dft ll = 0; ll <= sz; ++ll) {
      if (ll != sz)
        if (!(eq(ii[ll - k], it1[ll])
              && eq(*(ii + (ll - k)), it1[ll])
              && eq(*((ll - k) + ii), it1[ll])
              && eq(*(ii - (-(ll - k))), it1[ll])))
          print_then_abort(errs, " failed at step 5-1\n");
      if (!(ii - (i + ll) == k - ll
            && (i + ll) - ii == -(k - ll)
            && test_equal(ii - (k - ll), i + ll)
            && test_equal(ii + (-(k - ll)), i + ll)
            && test_equal(ii, (i + ll) + (k - ll))
            && test_equal(ii, (i + ll) - (-(k - ll)))))
        print_then_abort(errs, " failed at step 5-2\n");
      if (k < ll) {
        if (!test_less(i + k, i + ll))
          print_then_abort(errs, " failed at step 5-3\n");
      }
      else if (k == ll) {
        if (!test_equal(i + k, i + ll))
          print_then_abort(errs, " failed at step 5-4\n");
      }
    }
  }
}
template <class I, class J, class IT, class EQ>
constexpr void test_ritr_impl(I i, J j, IT it1, IT it2, EQ eq,
                              const char *errs) {
  inner::fns::test_bitr_impl(i, j, it1, it2, eq, errs);
  inner::fns::test_ritr_impl0(i, j, it1, it2, eq, errs);
  {
    const auto bk_i = i;
    while (i != j)
      ++i;
    while (j != bk_i)
      --j;
  }
  inner::fns::test_ritr_impl0(j, i, it1, it2, eq, errs);
}

template <class R>
constexpr auto range_begin(R &r)->decltype(r.begin()) {
  return r.begin();
}
template <class R>
constexpr auto range_begin(const R &r)->decltype(r.begin()) {
  return r.begin();
}
template <class T, size_t N>
constexpr T *range_begin(T (&a)[N]) noexcept {
  return a;
}

template <class R>
constexpr auto range_end(R &r)->decltype(r.end()) {
  return r.end();
}
template <class R>
constexpr auto range_end(const R &r)->decltype(r.end()) {
  return r.end();
}
template <class T, size_t N>
constexpr T *range_end(T (&a)[N]) noexcept {
  return a + N;
}

template <class R>
constexpr bool range_empty(R &r) {
  return inner::fns::range_begin(r) == inner::fns::range_end(r);
}
template <class R>
constexpr bool range_empty(R &r) requires (requires {r.empty();}) {
  return r.empty();
}
template <class R>
constexpr bool range_empty(const R &r) {
  return inner::fns::range_begin(r) == inner::fns::range_end(r);
}
template <class R>
constexpr bool range_empty(const R &r) requires (requires {r.empty();}) {
  return r.empty();
}

template <class R>
constexpr uintmax_t range_size(R &r) {
  uintmax_t sz = 0;
  auto i = inner::fns::range_begin(r);
  const auto ed = inner::fns::range_end(r);
  while (i != ed) {
    ++sz;
    ++i;
  }
  return sz;
}
template <class R>
constexpr auto range_size(R &r) requires (requires {r.size();}) {
  return r.size();
}
template <class R>
constexpr uintmax_t range_size(const R &r) {
  uintmax_t sz = 0;
  auto i = inner::fns::range_begin(r);
  const auto ed = inner::fns::range_end(r);
  while (i != ed) {
    ++sz;
    ++i;
  }
  return sz;
}
template <class R>
constexpr auto range_size(const R &r) requires (requires {r.size();}) {
  return r.size();
}

template <class T>
concept has_szmf = requires(const T &x) {x.size();};

template <class R>
constexpr void test_rng_impl0(R &&r) {
  if constexpr (has_szmf<R>) {
    if (inner::fns::range_empty(r)) {
      if (!(inner::fns::range_begin(r) == inner::fns::range_end(r)
            && inner::fns::range_size(r) == 0))
        print_then_abort("re::test_rng() failed at step 0\n");
    }
    else {
      if (!(inner::fns::range_begin(r) != inner::fns::range_end(r)
            && inner::fns::range_size(r) != 0))
        print_then_abort("re::test_rng() failed at step 1\n");
    }
  }
}
template <class I, class J, class EQ>
constexpr void test_rng_impl(I i, I ii, J j, J jj, EQ eq) {
  for (;;) {
    if (i == ii) {
      if (!(test_equal(i, i) && test_equal(j, j)))
        print_then_abort("re::test_rng() failed at step 2\n");
      if (!(j == jj))
        print_then_abort("re::test_rng() failed at step 3\n");
      break;
    }
    else {
      if (!(test_equal(i, i) && test_equal(j, j)))
        print_then_abort("re::test_rng() failed at step 4\n");
      if (!(j != jj))
        print_then_abort("re::test_rng() failed at step 5\n");
      if (!eq(*i, *j))
        print_then_abort("re::test_rng() failed at step 6\n");
    }
    ++i;
    ++j;
  }
}

}
struct fo_test_iitr {
  template <class I, class J, class RR, class EQ>
  constexpr void operator ()(I i, J j, const RR &rr, EQ eq) const {
    inner::fns::test_iitr_impl(i, j,
                               inner::fns::range_begin(rr),
                               inner::fns::range_end(rr),
                               eq, "test_iitr()");
  }
  template <class I, class J, class RR>
  constexpr void operator ()(I i, J j, const RR &rr) const {
    operator ()(i, j, rr, equal_to<>{});
  }
  template <class R, class RR>
  constexpr void operator ()(R &&r, const RR &rr) const {
    operator ()(inner::fns::range_begin(r), inner::fns::range_end(r), rr);
  }
};
inline constexpr fo_test_iitr test_iitr{};
struct fo_test_fitr {
  template <class I, class J, class RR, class EQ>
  constexpr void operator ()(I i, J j, const RR &rr, EQ eq) const {
    inner::fns::test_fitr_impl(i, j,
                               inner::fns::range_begin(rr),
                               inner::fns::range_end(rr),
                               eq, "test_fitr()");
  }
  template <class I, class J, class RR>
  constexpr void operator ()(I i, J j, const RR &rr) const {
    operator ()(i, j, rr, equal_to<>{});
  }
  template <class R, class RR>
  constexpr void operator ()(R &&r, const RR &rr) const {
    operator ()(inner::fns::range_begin(r), inner::fns::range_end(r), rr);
  }
};
inline constexpr fo_test_fitr test_fitr{};
struct fo_test_bitr {
  template <class I, class J, class RR, class EQ>
  constexpr void operator ()(I i, J j, const RR &rr, EQ eq) const {
    inner::fns::test_bitr_impl(i, j,
                               inner::fns::range_begin(rr),
                               inner::fns::range_end(rr),
                               eq, "test_bitr()");
  }
  template <class I, class J, class RR>
  constexpr void operator ()(I i, J j, const RR &rr) const {
    operator ()(i, j, rr, equal_to<>{});
  }
  template <class R, class RR>
  constexpr void operator ()(R &&r, const RR &rr) const {
    operator ()(inner::fns::range_begin(r), inner::fns::range_end(r), rr);
  }
};
inline constexpr fo_test_bitr test_bitr{};
struct fo_test_ritr {
  template <class I, class J, class RR, class EQ>
  constexpr void operator ()(I i, J j, const RR &rr, EQ eq) const {
    inner::fns::test_ritr_impl(i, j,
                               inner::fns::range_begin(rr),
                               inner::fns::range_end(rr),
                               eq, "test_ritr()");
  }
  template <class I, class J, class RR>
  constexpr void operator ()(I i, J j, const RR &rr) const {
    operator ()(i, j, rr, equal_to<>{});
  }
  template <class R, class RR>
  constexpr void operator ()(R &&r, const RR &rr) const {
    operator ()(inner::fns::range_begin(r), inner::fns::range_end(r), rr);
  }
};
inline constexpr fo_test_ritr test_ritr{};

struct fo_test_rng {
  template <class R, class RR, class EQ>
  constexpr void operator ()(R &&r, const RR &rr, EQ eq) const {
    inner::fns::test_rng_impl0(r);
    inner::fns::test_rng_impl(inner::fns::range_begin(r),
                              inner::fns::range_end(r),
                              inner::fns::range_begin(rr),
                              inner::fns::range_end(rr), eq);
  }
  template <class R, class RR>
  constexpr void operator ()(R &&r, const RR &rr) const {
    operator ()(r, rr, equal_to<>{});
  }
};
inline constexpr fo_test_rng test_rng{};

namespace inner {

template <class I>
concept itr_can_minus_minus = requires(I &i) {
  {--i}->same_as<I &>;
};

}
namespace inner::fns {

template <class I, class J>
constexpr void test_itr_minus_impl1(I, J, size_t) {}
template <class I, class J>
constexpr void test_itr_minus_impl1(I i, J j, size_t n)
  requires inner::itr_can_minus_minus<J> {
  using dif_t = decltype(j - i);
  const dif_t dif = static_cast<dif_t>(n);
  const I op = i;
  J it = j;
  dif_t it_nth = dif;
  for (;;) {
    J it2 = j;
    dif_t it2_nth = dif;
    for (;;) {
      if (!(it - it2 == it_nth - it2_nth
            && it2 - it == it2_nth - it_nth))
        print_then_abort("re::test_itr_minus(...) failed at step 2\n");

      if (op == it2)
        break;
      --it2;
      --it2_nth;
    }

    if (op == it)
      break;
    --it;
    --it_nth;
  }
}

template <class I, class J>
constexpr void test_itr_minus_impl2(I, J, size_t) {}
template <class I, class J>
constexpr void test_itr_minus_impl2(I i, J j, size_t n)
  requires inner::itr_can_minus_minus<J> {
  using dif_t = decltype(j - i);
  const dif_t dif = static_cast<dif_t>(n);
  const I op = i;
  const J ed = j;
  I it = i;
  dif_t it_nth = 0;
  for (;;) {
    J it2 = j;
    dif_t it2_nth = dif;

    for (;;) {
      if (!(it - it2 == it_nth - it2_nth
            && it2 - it == it2_nth - it_nth))
        print_then_abort("re::test_itr_minus(...) failed at step 3\n");

      if (op == it2)
        break;
      --it2;
      --it2_nth;
    }

    if (it == ed)
      break;
    ++it;
    ++it_nth;
  }
}

}
struct fo_test_itr_minus {
  template <class I, class J>
  constexpr void operator ()(I i, J j, size_t n) const {
    using dif_t = decltype(j - i);
    const dif_t dif = static_cast<dif_t>(n);
    if (!(j - i == dif && i - j == -dif))
      print_then_abort("re::test_itr_minus(...) failed at step 0\n");

    using common_t = common_type_t<I, J>;
    static_assert(is_same_v<I, common_t> || is_same_v<J, common_t>);

    // I and I
    {
      const J ed = j;
      I it = i;
      dif_t it_nth = 0;
      for (;;) {
        if (!(it - ed == it_nth - dif
              && ed - it == dif - it_nth))
          print_then_abort("re::test_itr_minus(...) failed at step 1-0\n");
        I it2 = i;
        dif_t it2_nth = 0;
        for (;;) {
          if (!(it - it2 == it_nth - it2_nth
                && it2 - it == it2_nth - it_nth))
            print_then_abort("re::test_itr_minus(...) failed at step 1-1\n");
          if (it2 == ed)
            break;
          ++it2;
          ++it2_nth;
        }

        if (it == ed)
          break;
        ++it;
        ++it_nth;
      }
    }

    // J and J
    inner::fns::test_itr_minus_impl1(i, j, n);

    // I and J
    inner::fns::test_itr_minus_impl2(i, j, n);
  }
  template <class I, class J>
  constexpr void operator ()(I i, J j) const requires (!is_integral_v<J>) {
    return operator ()(i, j, j - i);
  }
  template <class R, class T>
  constexpr void operator ()(R &&r, T n) const requires is_integral_v<T> {
    operator ()(inner::fns::range_begin(r),
                inner::fns::range_end(r), static_cast<size_t>(n));
  }
  template <class R>
  constexpr void operator ()(R &&r) const {
    operator ()(inner::fns::range_begin(r),
                inner::fns::range_end(r),
                static_cast<size_t>(inner::fns::range_size(r)));
  }
};
inline constexpr fo_test_itr_minus test_itr_minus{};

}

#endif
