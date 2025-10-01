//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>

#include <cassert>

using namespace re;

namespace re::inner::test::help_allocator_traits {

template <class T = int>
struct tmp_alloc_0 {
  static int i;
  using value_type = int;
  int *allocate(size_t) {
    ++i;
    return nullptr;
  }
  void deallocate(int *, size_t) {
    --i;
  }
};
template <class T>
int tmp_alloc_0<T>::i = 0;
struct tmp_alloc_12 : tmp_alloc_0<> {
  template <class U>
  struct rebind {
    using other = void;
  };
};

struct tmp_al {};
struct tmp_al2 {
  using value_type = int;
  void construct(int *, int &&) {}
};

}
namespace re {

template <>
struct allocator_traits<inner::test::help_allocator_traits::tmp_al> {};

}

namespace re::inner::test {

void test_allocator_traits() {
  using namespace help_allocator_traits;

  using a0 = tmp_alloc_0<>;
  using at0 = allocator_traits<a0>;

  struct a1 : tmp_alloc_0<> {};
  struct a2 : tmp_alloc_0<> {
    using pointer = float *;
  };
  using at1 = allocator_traits<a1>;
  using at2 = allocator_traits<a2>;
  static_assert(is_same<at1::pointer, int *>);
  static_assert(is_same<at2::pointer, float *>);

  struct a3 : tmp_alloc_0<> {
    using const_pointer = float *;
  };
  using at3 = allocator_traits<a3>;
  static_assert(is_same<at3::const_pointer, float *>);
  static_assert(is_same<at1::const_pointer, const int *>);

  struct a4 : tmp_alloc_0<> {
    using void_pointer = float *;
  };
  using at4 = allocator_traits<a4>;
  static_assert(is_same<at4::void_pointer, float *>);
  static_assert(is_same<at1::void_pointer, void *>);

  struct a5 : tmp_alloc_0<> {
    using const_void_pointer = float *;
  };
  using at5 = allocator_traits<a5>;
  static_assert(is_same<at5::const_void_pointer, float *>);
  static_assert(is_same<at1::const_void_pointer, const void *>);

  struct a6 : tmp_alloc_0<> {
    using difference_type = float *;
  };
  using at6 = allocator_traits<a6>;
  static_assert(is_same<at6::difference_type, float *>);
  static_assert(is_same<at1::difference_type, ptrdiff_t>);

  struct a7 : tmp_alloc_0<> {
    using size_type = float *;
  };
  using at7 = allocator_traits<a7>;
  static_assert(is_same<at7::size_type, float *>);
  static_assert(is_same<at1::size_type, make_unsigned<ptrdiff_t>>);

  struct a8 : tmp_alloc_0<> {
    using propagate_on_container_copy_assignment = true_type;
  };
  using at8 = allocator_traits<a8>;
  static_assert(at8::propagate_on_container_copy_assignment::value);
  static_assert(!at1::propagate_on_container_copy_assignment::value);

  struct a9 : tmp_alloc_0<> {
    using propagate_on_container_move_assignment = true_type;
  };
  struct a90 : tmp_alloc_0<> {
    using propagate_on_container_move_assignment = false_type;
  };
  using at9 = allocator_traits<a9>;
  using at90 = allocator_traits<a90>;
  static_assert(at9::propagate_on_container_move_assignment::value);
  static_assert(!at90::propagate_on_container_move_assignment::value);
  static_assert(!at1::propagate_on_container_move_assignment::value);

  struct a10 : tmp_alloc_0<> {
    using propagate_on_container_swap = true_type;
  };
  struct a100 : tmp_alloc_0<> {
    using propagate_on_container_swap = false_type;
  };
  using at10 = allocator_traits<a10>;
  using at100 = allocator_traits<a100>;
  static_assert(at10::propagate_on_container_swap::value);
  static_assert(!at100::propagate_on_container_swap::value);
  static_assert(!at1::propagate_on_container_swap::value);

  struct a11 : tmp_alloc_0<> {
    using is_always_equal = false_type;
  };
  struct a110 : tmp_alloc_0<> {
    using is_always_equal = true_type;
  };
  using at11 = allocator_traits<a11>;
  using at110 = allocator_traits<a110>;
  static_assert(!at11::is_always_equal::value);
  static_assert(at110::is_always_equal::value);
  static_assert(at1::is_always_equal::value);

  using a12 = tmp_alloc_12;
  using at12 = allocator_traits<a12>;
  static_assert(is_same<at12::rebind_alloc<float>, void>);
  static_assert(is_same<at0::rebind_alloc<float>, tmp_alloc_0<float>>);
  static_assert(is_same<at12::rebind_traits<float>, allocator_traits<void>>);
  static_assert(is_same<at0::rebind_traits<float>,
                        allocator_traits<tmp_alloc_0<float>>>);

  a0 a;
  int *p = at0::allocate(a, 0);
  assert(p == nullptr && a.i == 1);
  at0::deallocate(a, nullptr, 0);
  assert(a.i == 0);
  {
    static_assert(!inner::allocator_can_implicitly_allocate_1
                  <test_allocator<int>>);
    static_assert(!inner::allocator_can_implicitly_deallocate_1
                  <test_allocator<int>>);
    struct tmp_alloc_t {
      void *allocate() {return nullptr;}
      void deallocate(void *) {}
    };
    static_assert(inner::allocator_can_implicitly_allocate_1<tmp_alloc_t>);
    static_assert(inner::allocator_can_implicitly_deallocate_1<tmp_alloc_t>);
  }

  struct a13 : tmp_alloc_0<> {
    using tmp_alloc_0<>::allocate;
    int *allocate(size_t, const void *p) {
      return reinterpret_cast<int *>(const_cast<void *>(p));
    }
    void construct(int *, int) {}
    void destroy(int *) {}
    size_t max_size() const {
      return 9;
    }
    a13 select_on_container_copy_construction() const {
      tmp_alloc_0<>::i = 4;
      return *this;
    }
  };
  {
    int x = 1;
    a13 tmp_a;
    using at13 = allocator_traits<a13>;
    static_assert(f_is_well_formed
                  <inner::check_alloc_is_capable_of_hint_allocation,
                   a13>);
    const auto u = at13::allocate(tmp_a, 3,
                                  static_cast<const void *>(addressof(x)));
    assert(u == addressof(x) && *u == 1);
    static_assert(same_as<size_t, decltype(at13::max_size(tmp_a))>);
    assert(at13::max_size(tmp_a) == 9);
    a13 v = at13::select_on_container_copy_construction(tmp_a);
    assert(a13::i == 4);
  }
  static_assert(!f_is_well_formed
                <inner::check_alloc_can_destroy_by_member_function,
                 a0, int>);
  static_assert(f_is_well_formed
                <inner::check_alloc_can_destroy_by_member_function,
                 a13, int>);
  static_assert(!f_is_well_formed
                <inner::check_alloc_can_construct_by_member_function,
                 a0, int, int>);
  static_assert(f_is_well_formed
                <inner::check_alloc_can_construct_by_member_function,
                 a13, int, int>);
  static_assert(!f_is_well_formed
                <inner::check_alloc_has_member_function_max_size, a0>);
  static_assert
    (!f_is_well_formed
     <inner::check_alloc_can_select_on_container_copy_construction,
      a0>);
  static_assert
    (f_is_well_formed
     <inner::check_alloc_can_select_on_container_copy_construction,
      a13>);
  static_assert
    (f_is_well_formed
     <inner::check_alloc_is_capable_of_hint_allocation, a13>);
  static_assert
    (!f_is_well_formed
     <inner::check_alloc_is_capable_of_hint_allocation, a0>);

  {
    using a = allocator<int>;
    static_assert(same_as<alloc_vt<a>, int>);
    static_assert(same_as<alloc_ptr<a>, int *>);
    static_assert(same_as<alloc_cptr<a>, const int *>);
    static_assert(same_as<alloc_void_ptr<a>, void *>);
    static_assert(same_as<alloc_const_void_ptr<a>, const void *>);
    static_assert(same_as<alloc_dft<a>, ptrdiff_t>);
    static_assert(same_as<alloc_szt<a>, size_t>);
    static_assert(same_as<alloc_rebind<a, char>, allocator<char>>);
    static_assert(same_as<alloc_rebind_traits<a, char>,
                          allocator_traits<allocator<char>>>);
    static_assert(same_as<alloc_rebind_ptr<a, char>, char *>);
    static_assert(same_as<alloc_rebind_cptr<a, char>, const char *>);
    static_assert(alloc_always_equal<a>);
    static_assert(!alloc_copy_prpg<a>);
    static_assert(alloc_move_prpg<a>);
    static_assert(!alloc_swap_prpg<a>);
  }

  // allocator_with_primary_traits
  // allocator_provides_construct_function
  // allocator_provides_destroy_function
  // nothrow_constructible_by_allocator
  // nothrow_move_constructible_by_allocator
  {
    static_assert(allocator_with_primary_traits<allocator<int>>);
    static_assert(!allocator_with_primary_traits<tmp_al>);

    static_assert(!allocator_provides_construct_function<allocator<int>, int>);
    static_assert(!allocator_provides_construct_function
                  <allocator<int>, int, int>);
    static_assert(!allocator_provides_destroy_function<allocator<int>, int>);

    static_assert(nothrow_constructible_by_allocator
                  <allocator<int>, int, int>);
    static_assert(!nothrow_constructible_by_allocator
                  <allocator<int>, int, void>);
    static_assert(!nothrow_constructible_by_allocator
                  <allocator<int>, void>);

    static_assert(nothrow_move_constructible_by_allocator<allocator<int>, int>);
    static_assert(allocator_provides_construct_function<tmp_al2, int, int &&>);
    static_assert(!nothrow_move_constructible_by_allocator<tmp_al2, int>);
    static_assert(!is_nothrow_move_constructible<test_object<int>>);
    static_assert(!nothrow_move_constructible_by_allocator
                  <tmp_al2, test_object<int>>);
    static_assert(!allocator_with_primary_traits<tmp_al>);
    static_assert(!nothrow_move_constructible_by_allocator<tmp_al, char>);
  }
};
void test_uninitialized_sequence() {
  // default_construct_non_array_at
  // construct_non_array_at
  // destroy_non_array_at
  // destroy_at
  // destroy
  {
    {
      using t = instance_counter<int>;
      test_allocator<t> a;

      assert(t::count() == 0);
      t *p = a.allocate(1);
      construct_non_array_at(p, 3);
      assert(**p == 3);
      assert(p->count() == 1);
      destroy_at(p);
      assert(p->count() == 0);
      a.deallocate(p, 1);
    }
    {
      test_allocator<int> a;

      int *p = a.allocate(1000);
      for (auto q : irng(p, 1000))
        assert(construct_non_array_at(q, 1) == q);
      for (const auto &q : rng(p, 1000))
        assert(q == 1);
      for (auto q : irng(p, 1000))
        destroy_non_array_at(q);
      a.deallocate(p, 1000);

      p = a.allocate(10000);
      for (auto q : irng(p, 10000))
        assert(construct_non_array_at(q) == q);
      for (auto q : rng(p, 10000))
        assert(q == 0);
      for (auto q : irng(p, 10000))
        destroy_non_array_at(q);
      a.deallocate(p, 10000);
    }

    // construct_non_array_at for const T *
    {
      void *p = ::operator new(sizeof(int));
      auto p2 = static_cast<const int *>(p);
      construct_non_array_at(p2);
      destroy_non_array_at(p2);
      ::operator delete(p);
    }

    // destroy_at for bounded array
    {
      using t = int [5];
      t *p = (t *)::operator new(sizeof(t));
      for (auto &it : iters(*p))
        construct_non_array_at(it, 1);
      for (int &x : *p)
        assert(x == 1);
      destroy_at(p);
      ::operator delete(p);
    }
  }

  // default_construct_array(p)
  {
    using t = int [3];
    t *p = (t *)::operator new(sizeof(t));
    default_construct_array(p);
    ::operator delete(p);
  }
  // construct_array(p)
  {
    using t = int [3][4][5];
    t *p = (t *)::operator new(sizeof(t));
    construct_array(p);
    for (int i : irng(0, 3))
      for (int j : irng(0, 4))
        for (int k : irng(0, 5))
          assert((*p)[i][j][k] == 0);
    ::operator delete(p);
  }
  // construct_array(p, fll)
  {
    {
      using t = int [3][4][5];
      t *p = (t *)::operator new(sizeof(t));
      construct_array(p, 9);
      for (int i : irng(0, 3))
        for (int j : irng(0, 4))
          for (int k : irng(0, 5))
            assert((*p)[i][j][k] == 9);
      ::operator delete(p);
    }
    {
      using t = int [2][2];
      t *p = reinterpret_cast<t *>(::operator new(sizeof(t)));
      int a[2] = {1, 2};
      construct_array(p, a);
      assert((*p)[0][0] == 1);
      assert((*p)[0][1] == 2);
      assert((*p)[1][0] == 1);
      assert((*p)[1][1] == 2);
      destroy(rng(p, 2));
      ::operator delete(const_cast<void *>
                        (reinterpret_cast<const volatile void *>(p)));
    }
    {
      using t = int [2][2];
      t *p = reinterpret_cast<t *>(::operator new(sizeof(t)));
      int a[2][2] = {{1, 2}, {3, 4}};
      construct_array(p, a);
      assert((*p)[0][0] == 1);
      assert((*p)[0][1] == 2);
      assert((*p)[1][0] == 3);
      assert((*p)[1][1] == 4);
      ::operator delete(const_cast<void *>
                        (reinterpret_cast<const volatile void *>(p)));
    }
  }
  // clean after throwing for:
  //   default_construct_array
  //   construct_array
  {
#ifndef RE_NOEXCEPT
    using t = test_object<exception_countdown<int>>;
    void *p = ::operator new(sizeof(t) * 5);
    const auto guard = exit_fn([p]() {::operator delete(p);});
    {
      exception_countdown<int>::constructor_countdown(3);
      bool y = false;
      try {
        default_construct_array(reinterpret_cast<t (*)[5]>(p));
      }
      catch (...) {
        y = true;
      }
      assert(y);
    }
    {
      exception_countdown<int>::constructor_countdown(3);
      bool y = false;
      try {
        construct_array(reinterpret_cast<t (*)[5]>(p));
      }
      catch (...) {
        y = true;
      }
      assert(y);
    }
    {
      exception_countdown<int>::constructor_countdown(4);
      bool y = false;
      try {
        construct_array(reinterpret_cast<t (*)[2][2]>(p), t{});
      }
      catch (...) {
        y = true;
      }
      assert(y);
    }
    {
      exception_countdown<int>::constructor_countdown(4);
      bool y = false;
      try {
        construct_array(reinterpret_cast<t (*)[5]>(p), t{});
      }
      catch (...) {
        y = true;
      }
      assert(y);
    }
    {
      bool y = false;
      t a[2]{};
      for (int i : iters(1, 5)) {
        exception_countdown<int>::constructor_countdown(i);
        try {
          construct_array(reinterpret_cast<t (*)[2][2]>(p), a);
        }
        catch (...) {
          y = true;
        }
        assert(y);
      }
    }
#endif
  }

  // default_construct_maybe_array
  {
    using t = test_object<int>;
    void *p = ::operator new(sizeof(t) * 2);
    const auto guard = exit_fn([p]() {::operator delete(p);});

    default_construct_maybe_array(static_cast<t *>(p));
    assert(*static_cast<t *>(p) == 0);
    destroy_at(static_cast<t *>(p));

    default_construct_maybe_array(static_cast<t (*)[2]>(p));
    assert(*static_cast<t *>(p) == 0);
    assert(*(static_cast<t *>(p) + 1) == 0);
    destroy_at(static_cast<t *>(p));
    destroy_at(static_cast<t *>(p) + 1);

    default_construct_maybe_array(static_cast<const t (*)[2]>(p));
    assert(*static_cast<const t *>(p) == 0);
    assert(*(static_cast<const t *>(p) + 1) == 0);
    destroy_at(static_cast<const t *>(p));
    destroy_at(static_cast<const t *>(p) + 1);
  }
  // construct_maybe_array
  {
    using t = test_object<int>;
    void *p = ::operator new(sizeof(t) * 2);
    const auto guard = exit_fn([p]() {::operator delete(p);});

    construct_maybe_array(static_cast<t *>(p), 1);
    assert(*static_cast<t *>(p) == 1);
    destroy_at(static_cast<t *>(p));

    construct_maybe_array(static_cast<t (*)[2]>(p), 1);
    assert(static_cast<t *>(p)[0] == 1);
    assert(static_cast<t *>(p)[1] == 1);
    destroy_at(static_cast<t (*)[2]>(p));

    construct_maybe_array(static_cast<const t (*)[2]>(p), 1);
    assert(static_cast<const t *>(p)[0] == 1);
    assert(static_cast<const t *>(p)[1] == 1);
    destroy_at(static_cast<const t (*)[2]>(p));
  }

  // initialize
  // securely_initialize
  {
    int a[3]{};
    initialize(a, [i = 0](int *p) mutable {*p = ++i;});
    test_rng(a, irng(1, 4));
    int b[3]{};
    int *c = initialize(a, begin(b),
                        [](const int *p, int *pp) {*pp = *p;});
    assert(c == end(b));
    test_rng(a, irng(1, 4));
    test_rng(b, irng(1, 4));
    auto d = initialize(a, move_itr(begin(b)),
                        [](const int *p, int *pp) {*pp = *p;});
    assert(c == end(b));
    assert(d.base() == end(b));
    test_rng(a, irng(1, 4));
    test_rng(b, irng(1, 4));

    for (auto &x : a)
      x = 0;
    for (auto &x : b)
      x = 0;
    c = {};
    d = {};
    securely_initialize(a, [i = 0](int *p) mutable {*p = ++i;},
                        empty_function{});
    test_rng(a, irng(1, 4));
    c = securely_initialize(a, begin(b),
                            [](const int *p, int *pp) {*pp = *p;},
                            empty_function{});
    assert(c == end(b));
    test_rng(a, irng(1, 4));
    test_rng(b, irng(1, 4));
    d = securely_initialize(a, move_itr(begin(b)),
                            [](const int *p, int *pp) {*pp = *p;},
                            empty_function{});
    assert(c == end(b));
    assert(d.base() == end(b));
    test_rng(a, irng(1, 4));
    test_rng(b, irng(1, 4));
  }
  {
#ifndef RE_NOEXCEPT
    const auto tmpf = [](size_t sz, size_t n) {
      using obj_t = exception_countdown<int>;
      test_allocator<obj_t> a;
      obj_t *const p = a.allocate(sz);
      obj_t *const pp = a.allocate(sz);
      const auto guard = exit_fn([&]() {a.deallocate(p, sz);});
      const auto guard2 = exit_fn([&]() {a.deallocate(pp, sz);});

      const auto r = rng(p, sz);
      const auto rr = rng(pp, sz);
      obj_t::constructor_countdown(n);
      assert(obj_t::constructor_countdown() == n);
      try {
        securely_initialize(r, bind(construct_at, _1, 1), destroy_at);
        assert(false);
      }
      catch (const logic_error &) {}
      assert(obj_t::constructor_countdown() == 0);
      assert(obj_t::count() == 0);

      initialize(r, bind(construct_at, _1, 1));
      const auto guard3 = exit_fn([&]() {initialize(r, destroy_at);});
      assert(obj_t::count() == sz);
      obj_t::constructor_countdown(n);
      assert(obj_t::constructor_countdown() == n);
      try {
        securely_initialize(r, begin(rr),
                            bind(construct_at, _2, bind(deref, _1)),
                            destroy_at);
        assert(false);
      }
      catch (const logic_error &) {}
      assert(obj_t::constructor_countdown() == 0);
      assert(obj_t::count() == sz);
    };
    for (auto i : irng(1, 11))
      tmpf(10, i);
#endif
  }
  // initialize_plus
  // securely_initialize_plus
  {
    const auto testf = [](auto f) {
      int a[3]{};
      auto i = f(rng(begin(a), 3),
                 [i = 0](int *p) mutable {construct_at(p, ++i);});
      test_rng(a, irng(1, 4));
      assert(i == end(a));

      int b[3]{};
      auto t = f(rng(begin(a), 3), begin(b),
                 [](const int *p, int *pp) {*pp = *p;});
      assert(t == tuple(end(a), end(b)));
      test_rng(a, irng(1, 4));
      test_rng(b, irng(1, 4));
    };
    testf(initialize_plus);
    testf(bind_back(securely_initialize_plus, empty_function{}));
  }
}
void test_default_allocator() {
  using al = default_allocator<int>;
  static_assert(same_as<al, allocator<int>>);

  using at = allocator_traits<al>;

  static_assert(same_as<at::value_type, int>);
  static_assert(same_as<at::pointer, int *>);
  static_assert(same_as<at::const_pointer, const int *>);
  static_assert(same_as<at::void_pointer, void *>);
  static_assert(same_as<at::const_void_pointer, const void *>);
  static_assert(same_as<at::size_type, size_t>);
  static_assert(same_as<at::difference_type, ptrdiff_t>);
  static_assert(same_as
                <typename at::template rebind_alloc<void>, allocator<void>>);
  static_assert(same_as
                <typename at::template rebind_traits<void>,
                 allocator_traits<allocator<void>>>);
  static_assert(at::propagate_on_container_move_assignment::value);
  static_assert(!at::propagate_on_container_copy_assignment::value);
  static_assert(!at::propagate_on_container_swap::value);
  static_assert(at::is_always_equal::value);

  static_assert(regular<al>);
  static_assert(is_trivially_constructible<al>);
  static_assert(is_trivially_copy_constructible<al>);
  static_assert(is_trivially_move_constructible<al>);
  static_assert(is_trivially_copy_assignable<al>);
  static_assert(is_trivially_move_assignable<al>);
  al a;
  at::rebind_alloc<int> aa(a);
  test_equality(a, aa);

  using al2 = at::rebind_alloc<void>;
  using at2 = at::rebind_traits<void>;
  static_assert(same_as<at2::value_type, void>);
  static_assert(same_as<at2::pointer, void *>);
  static_assert(same_as<at2::const_pointer, const void *>);
  static_assert(same_as<at2::void_pointer, void *>);
  static_assert(same_as<at2::const_void_pointer, const void *>);
  static_assert(same_as<at2::size_type, size_t>);
  static_assert(same_as<at2::difference_type, ptrdiff_t>);
  static_assert(same_as
                <typename at2::template rebind_alloc<int>, allocator<int>>);
  static_assert(same_as
                <typename at2::template rebind_traits<int>,
                 allocator_traits<allocator<int>>>);
  static_assert(at2::propagate_on_container_move_assignment::value);
  static_assert(!at2::propagate_on_container_copy_assignment::value);
  static_assert(!at2::propagate_on_container_swap::value);
  static_assert(at2::is_always_equal::value);
}
void test_allocator_wrapper() {
  {
    using alw_t = allocator_wrapper<test_allocator<int>>;
    static_assert(is_default_constructible<alw_t>);
    static_assert(!is_trivially_constructible<alw_t>);
    static_assert(is_trivially_destructible<alw_t>);
    static_assert(is_trivially_copyable<alw_t>);
    static_assert(is_nothrow_swappable<alw_t>);
    static_assert(regular<alw_t::allocator_type>);
    static_assert(same_as<alw_t::allocator_type, test_allocator<int>>);
    static_assert(same_as
                  <alw_t::traits, allocator_traits<test_allocator<int>>>);
  }

  // get
  // rebind
  // max_size
  {
    using alw_t = allocator_wrapper<stateful_test_allocator<int>>;
    using alw2_t = allocator_wrapper<stateful_test_allocator<pair<int, int>>>;
    alw_t a;
    alw2_t aa(a.get());
    static_assert(!is_implicitly_constructible<alw2_t, decltype(a.get())>);
    test_equal(a.get(), aa.get());
    test_equal(a.get(), as_const(aa).get());
    test_equal(as_const(a).get(), aa.get());
    test_equal(as_const(a).get(), as_const(aa).get());

    {
      alw2_t a2 = a.rebind<pair<int, int>>();
      pair<int, int> *p = a2.allocate();
      assert(a2.get().size() == 1);
      a2.deallocate(p);
      assert(a2.get().size() == 0);
    }

    assert(a.max_size() == a.get().max_size());
  }
  // min_alignment
  {
    using alw_t = allocator_wrapper<allocator<char>>;
    using alw2_t = allocator_wrapper<test_allocator<char>>;
    using alw3_t = allocator_wrapper<stateful_test_allocator<char>>;
    alw_t a{};
    alw2_t a2{};
    alw3_t a3{};
    assert(a.min_alignment() == RE_DEFAULT_NEW_ALIGNMENT);
    assert(a2.min_alignment() == RE_DEFAULT_NEW_ALIGNMENT);
    assert(a3.min_alignment() == RE_DEFAULT_NEW_ALIGNMENT);

    struct t {
      alignas(512) byte placeholder;
    };
    using alw4_t = allocator_wrapper<allocator<t>>;
    alw4_t a4{};
    assert(a4.min_alignment() == 512);
  }

  // allocate_alignas
  // deallocate_alignas
  {
    struct t1 {
      alignas(16) char x;
    };
    struct t2 {
      alignas(32) char x;
    };
    struct t3 {
      alignas(64) char x;
    };
    struct t4 {
      alignas(128) char x;
    };
    struct t5 {
      alignas(256) char x;
    };
    using tp = type_pack<t1, t2, t3, t4, t5>;
    type_pack_for_each<tp>([](auto tag) {
      using t = typename decltype(tag)::type;
      using alw_t = allocator_wrapper<test_allocator<byte>>;
      alw_t a{};
      //assert(a.min_alignment() >= 8u);
      for (size_t i : irng(1, 128)) {
        auto p = a.allocate_alignas(alignof(t), i * sizeof(t));
        assert(is_aligned(p.first, alignof(t)));
        a.deallocate_alignas(alignof(t), p, i * sizeof(t));
      }
    });
  }

  // headed_buffer_ptr
  // allocate_headed_buffer
  // deallocate_headed_buffer
  {
    allocator_wrapper<test_allocator<byte>> a;    
    auto p = a.allocate_headed_buffer<int, int>(3, 1);
    assert(p.head() == 1);
    static_assert(same_as<decltype(p.data()), int *>);
    a.rebind<int>().uninitialized_copy(irng(1, 4), p.data());
    test_rng(rng(p.data(), 3), irng(1, 4));
    a.rebind<int>().destroy(rng(p.data(), 3));
    assert(p.size() == 3);
    a.deallocate_headed_buffer(p);
    {
      int x = 2;
      p.refer_to_only_head(x);
      assert(p.head() == 2);
      assert(p.data() == nullptr);
      assert(p.size() == 0);
    }

    auto p0 = a.allocate_headed_buffer<int, int>(0, 1);
    assert(p0.head() == 1);
    static_assert(same_as<decltype(p.data()), int *>);
    assert(p0.size() == 0);
    a.deallocate_headed_buffer(p0);

    struct alignas(64) align64_t {
      int value = 1;
    };
    struct alignas(256) align256_t {
      int value = 1;
    };

    auto p2 = a.allocate_headed_buffer<align64_t, align256_t>(10);
    assert(p2.head().value == 1);
    for (int i = 0; i != 10; ++i)
      a.rebind<align256_t>().construct(p2.data() + i, i);
    for (int i = 0; i != 10; ++i)
      assert((p2.data() + i)->value == i);
    assert(p2.size() == 10);
    for (int i = 0; i != 10; ++i)
      a.rebind<align256_t>().destroy(p2.data() + i);
    a.deallocate_headed_buffer(p2);

    auto p3 = a.allocate_headed_buffer<align256_t, align64_t>(10);
    assert(p3.head().value == 1);
    for (int i = 0; i != 10; ++i)
      a.rebind<align256_t>().construct(p3.data() + i, i);
    for (int i = 0; i != 10; ++i)
      assert((p3.data() + i)->value == i);
    assert(p3.size() == 10);
    for (int i = 0; i != 10; ++i)
      a.rebind<align256_t>().destroy(p3.data() + i);
    a.deallocate_headed_buffer(p3);
  }
  // more test for allocate_headed_buffer 
  {
    using alloc_t = test_allocator<byte>;
    using alw_t = allocator_wrapper<alloc_t>;
    alw_t aw;

    // first_align == second_align
    {
      using ptr_t = typename alw_t::template headed_buffer_ptr<intmax_t,
                                                               intmax_t>;
      ez_vector<ptr_t> v;
      for (int i : irng(1, 3000)) {
        const auto p = aw.allocate_headed_buffer<intmax_t, intmax_t>(i, 1);
        assert(inner::good(p));
        assert(p.head() == 1);
        assert(p.size() == to_unsigned(i));
        v.insert(v.end(), p);
      }
      for (auto &p : v)
        aw.deallocate_headed_buffer(p);
    }
    // first_align > second_align
    {
      struct alignas(16u) t {
        int x;
        int y;
      };
      assert(alignof(t) == 16u);
      using ptr_t = typename alw_t::template headed_buffer_ptr<t, intmax_t>;
      ez_vector<ptr_t> v;
      for (int i : irng(1, 3000)) {
        const auto p = aw.allocate_headed_buffer<t, intmax_t>(i);
        assert(inner::good(p));
        assert(p.head().x == 0 && p.head().y == 0);
        assert(p.size() == to_unsigned(i));
        v.insert(v.end(), p);
      }
      for (auto &p : v)
        aw.deallocate_headed_buffer(p);
    }
    // first_align < second_align
    {
      struct alignas(64u) t {
        int x;
        int y;
      };
      assert(alignof(t) == 64u);
      using ptr_t = typename alw_t::template headed_buffer_ptr<intmax_t, t>;
      ez_vector<ptr_t> v;
      for (int i : irng(1, 3000)) {
        const auto p = aw.allocate_headed_buffer<intmax_t, t>(i, 1);
        assert(inner::good(p));
        assert(p.head() == 1);
        assert(p.size() == to_unsigned(i));
        v.insert(v.end(), p);
      }
      for (auto &p : v)
        aw.deallocate_headed_buffer(p);
    }
  }

  // headed_bytebuf_ptr
  // allocate_headed_bytebuf
  // deallocate_headed_bytebuf
  {
    allocator_wrapper<test_allocator<byte>> a;
    {
      auto p = a.allocate_headed_bytebuf_alignas<int>(128, 0, 1);
      assert(p.head() == 1);
      assert(p.size() == 0u);
      assert(good(p));
      a.deallocate_headed_bytebuf(p);
    }
    {
      auto p = a.allocate_headed_bytebuf_alignas<int>(128, 3, 1);
      assert(p.head() == 1);
      assert(p.size() == 3u);
      assert(good(p));
      a.deallocate_headed_bytebuf(p);
    }
    allocator_wrapper<test_allocator<byte>>::headed_bytebuf_ptr<int> p;
    int x = 2;
    p.refer_to_only_head(x);
    assert(p.head() == 2);
    assert(p.data() == nullptr);
    assert(p.size() == 0);
  }

  // allocate
  // construct_at
  // destroy_at
  // deallocate
  // destroy
  // destroy_function
  // destroy_fn
  {
    using alw_t = allocator_wrapper<stateful_test_allocator
                                    <instance_counter<int>>>;
    alw_t a;
    auto p = a.allocate();
    assert(a.get().size() == 1);
    a.construct(to_address(p), 3);
    assert(**p == 3);
    assert(instance_counter<int>::count() == 1);
    a.destroy(to_address(p));
    assert(instance_counter<int>::count() == 0);
    a.deallocate(p);

    p = a.allocate(3);
    for (int i : iters(0, 3))
      a.construct(p + i);
    assert(instance_counter<int>::count() == 3);
    const auto r = rng(p, 3);
    a.destroy(r);
    assert(instance_counter<int>::count() == 0);
    a.deallocate(p, 3);

    static_assert(!is_default_constructible<alw_t::destroy_function>);
    static_assert(is_trivially_copyable<alw_t::destroy_function>);
    static_assert(is_swappable<alw_t::destroy_function>);

    p = a.allocate();
    a.construct(p);
    a.destroy_fn()(p);
    assert(instance_counter<int>::count() == 0);
    a.deallocate(p);
  }

  // uninitialized_fill(_plus)
  {
    using t = instance_counter<int>;
    using alw_t = allocator_wrapper<stateful_test_allocator<t>>;
    alw_t a{}, aa{};
    assert(a.get() != aa.get());

    assert(t::count() == 0);
    {
      a.uninitialized_fill(rng(decltype(a.allocate(1)){}, 0));
      assert(t::count() == 0);
    }
    assert(t::count() == 0);
    {
      auto p = a.allocate(1000);
      auto r = rng(p, 1000);
      a.uninitialized_fill(r);
      assert(t::count() == 1000);
      for (auto &x : r)
        assert(x == 0);
      a.destroy(r);
      a.deallocate(p, size(r));
    }

    assert(t::count() == 0);
    {
      auto p = a.uninitialized_fill_plus(rng(decltype(a.allocate(1)){}, 0));
      assert(p == nullptr);
    }
    assert(t::count() == 0);
    {
      auto p = a.allocate(1000);
      auto r = rng(p, 1000);
      assert(a.uninitialized_fill_plus(r) == end(r));
      assert(t::count() == 1000);
      for (auto &x : r)
        assert(x == 0);
      a.destroy(r);
      a.deallocate(p, size(r));
    }

    assert(t::count() == 0);
    {
      a.uninitialized_fill(rng(decltype(a.allocate(1)){}, 0), 0);
      assert(t::count() == 0);
    }
    assert(t::count() == 0);
    {
      auto p = a.allocate(1000);
      auto r = rng(p, 1000);
      a.uninitialized_fill(r, 1);
      assert(t::count() == 1000);
      for (auto &x : r)
        assert(x == 1);
      a.destroy(r);
      a.deallocate(p, size(r));
    }
  }

  // uninitialized_copy(_plus)
  {
    using t = instance_counter<test_object<int>>;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

    assert(t::count() == 0);
    a.uninitialized_fill(r, 1);
    assert(t::count() == 10);
    for (auto x : r)
      assert(*x == 1);

    assert(a.uninitialized_copy(r, begin(rr)) == end(rr));
    assert(t::count() == 20);
    for (auto x : r)
      assert(*x == 1);
    for (auto x : rr)
      assert(*x == 1);

    a.destroy(rr);
    assert(t::count() == 10);

    for (auto &x : r)
      *x = 2;
    assert(a.uninitialized_copy_plus(rng(begin(r), 10), begin(rr))
           == pair(end(r), end(rr)));
    for (auto x : r)
      assert(*x == 2);
    for (auto x : rr)
      assert(*x == 2);
    assert(t::count() == 20);

    a.destroy(r);
    a.destroy(rr);
    assert(t::count() == 0);
  }

  // uninitialized_move(_plus)
  {
    using t = instance_counter<test_object<int>>;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

    assert(t::count() == 0);
    a.uninitialized_fill(r, 1);
    assert(t::count() == 10);
    for (auto x : r)
      assert(*x == 1);

    assert(a.uninitialized_move(r, begin(rr)) == end(rr));
    assert(t::count() == 20);
    for (auto x : r)
      assert(*x == 0);
    for (auto x : rr)
      assert(*x == 1);

    a.destroy(rr);
    assert(t::count() == 10);

    for (auto &x : r)
      *x = 2;
    assert(a.uninitialized_move_plus(rng(begin(r), 10), begin(rr))
           == pair(end(r), end(rr)));
    for (auto x : r)
      assert(*x == 0);
    for (auto x : rr)
      assert(*x == 2);
    assert(t::count() == 20);

    a.destroy(r);
    a.destroy(rr);
    assert(t::count() == 0);
  }

  // uninitialized_fully_move(_plus)
  {
    using t = exception_countdown<int>;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

    {
      assert(t::count() == 0);
      a.uninitialized_fill(r, 1);
      for (const auto &x : r)
        assert(*x == 1);
      assert(t::count() == 10);
      assert(a.uninitialized_fully_move_plus(r, begin(rr))
             == pair(end(r), end(rr)));
      assert(t::count() == 10);
      for (const auto &x : rr)
        assert(*x == 1);
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_fill(r, 1);
      for (const auto &x : r)
        assert(*x == 1);
      assert(t::count() == 10);
      assert(a.uninitialized_fully_move_plus(r, begin(rr), a.destroy_fn())
             == pair(end(r), end(rr)));
      assert(t::count() == 10);
      for (const auto &x : rr)
        assert(*x == 1);
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_fill(r, 1);
      for (const auto &x : r)
        assert(*x == 1);
      assert(t::count() == 10);
      assert(a.uninitialized_fully_move(r, begin(rr), a.destroy_fn())
             == end(rr));
      assert(t::count() == 10);
      for (const auto &x : rr)
        assert(*x == 1);
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_fill(r, 1);
      for (const auto &x : r)
        assert(*x == 1);
      assert(t::count() == 10);
      assert(a.uninitialized_fully_move(r, begin(rr)) == end(rr));
      assert(t::count() == 10);
      for (const auto &x : rr)
        assert(*x == 1);
      a.destroy(rr);
      assert(t::count() == 0);
    }

#ifndef RE_NOEXCEPT
    {
      t::move_constructor_countdown(5);
      assert(t::move_constructor_countdown() == 5);
      {
        assert(t::count() == 0);
        a.uninitialized_copy(irng(0, 10), begin(r));
        assert(t::count() == 10);
        test_rng(r, irng(0, 10));

        assert(a.uninitialized_fully_move(r, begin(rr)) == end(rr));
        assert(t::count() == 10);
        test_rng(rr, irng(0, 10));

        a.destroy(rr);
        assert(t::count() == 0);
      }
      assert(t::move_constructor_countdown() == 5);
      t::move_constructor_countdown(0);
    }
    {
      t::copy_constructor_countdown(5);
      assert(t::copy_constructor_countdown() == 5);
      {
        assert(t::count() == 0);
        a.uninitialized_copy(irng(0, 10), begin(r));
        assert(t::count() == 10);
        test_rng(r, irng(0, 10));

        try {
          a.uninitialized_fully_move(r, begin(rr));
        }
        catch (const logic_error &) {}

        assert(t::count() == 10);
        test_rng(r, irng(0, 10));
        a.destroy(r);
        assert(t::count() == 0);
      }
      assert(t::copy_constructor_countdown() == 0);
    }
#endif
  }

  // fully_move(_plus)
  {
    using t = exception_countdown<int>;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

    {
      assert(t::count() == 0);
      a.uninitialized_copy(irng(0, 10), begin(r));
      a.uninitialized_copy(irng(10, 20), begin(rr));
      test_rng(r, irng(0, 10));
      test_rng(rr, irng(10, 20));
      assert(t::count() == 20);

      assert(a.fully_move(r, begin(rr)) == end(rr));
      assert(t::count() == 10);
      test_rng(rr, irng(0, 10));
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_copy(irng(0, 10), begin(r));
      a.uninitialized_copy(irng(10, 20), begin(rr));
      test_rng(r, irng(0, 10));
      test_rng(rr, irng(10, 20));
      assert(t::count() == 20);

      assert(a.fully_move(r, begin(rr), a.destroy_fn()) == end(rr));
      assert(t::count() == 10);
      test_rng(rr, irng(0, 10));
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_copy(irng(0, 10), begin(r));
      a.uninitialized_copy(irng(10, 20), begin(rr));
      test_rng(r, irng(0, 10));
      test_rng(rr, irng(10, 20));
      assert(t::count() == 20);

      assert(a.fully_move_plus(r, begin(rr), a.destroy_fn())
             == pair(end(r), end(rr)));
      assert(t::count() == 10);
      test_rng(rr, irng(0, 10));
      a.destroy(rr);
      assert(t::count() == 0);
    }
    {
      assert(t::count() == 0);
      a.uninitialized_copy(irng(0, 10), begin(r));
      a.uninitialized_copy(irng(10, 20), begin(rr));
      test_rng(r, irng(0, 10));
      test_rng(rr, irng(10, 20));
      assert(t::count() == 20);

      assert(a.fully_move_plus(r, begin(rr)) == pair(end(r), end(rr)));
      assert(t::count() == 10);
      test_rng(rr, irng(0, 10));
      a.destroy(rr);
      assert(t::count() == 0);
    }

#ifndef RE_NOEXCEPT
    {
      for (int n : irng(1, 11)) {
        a.uninitialized_fill(r, n);
        a.uninitialized_fill(rr, n + 1);
        for (const auto &x : r)
          assert(*x == n);
        for (const auto &x : rr)
          assert(*x == n + 1);
        assert(t::count() == 20);

        static_assert(!is_nothrow_move_assignable<t>);
        assert(t::copy_assignment_countdown() == 0);
        t::copy_assignment_countdown(n);
        try {
          a.fully_move(r, begin(rr));
          assert(false);
        }
        catch (const logic_error &) {}
        assert(t::copy_assignment_countdown() == 0);
        for (const auto &x : r)
          assert(*x == n);
        assert(t::count() == 20);

        a.destroy(r);
        a.destroy(rr);
      }
    }
#endif
  }
  {
    using t = instance_counter<int>;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

#ifndef RE_NOEXCEPT
    {
      const int n = 1;
      assert(t::count() == 0);
      a.uninitialized_fill(r, n);
      a.uninitialized_fill(rr, n + 1);
      for (const auto &x : r)
        assert(*x == n);
      for (const auto &x : rr)
        assert(*x == n + 1);
      assert(t::count() == 20);

      a.fully_move(r, begin(rr));
      assert(t::count() == 10);
      for (const auto &x : rr)
        assert(*x == n);

      a.destroy(rr);
      assert(t::count() == 0);
    }
#endif
  }
  {
    using t = int;
    allocator_wrapper<test_allocator<t>> a;
    auto *const p = a.allocate(10);
    auto *const pp = a.allocate(10);
    const auto guard = exit_fn([&]() {a.deallocate(p, 10);});
    const auto guard2 = exit_fn([&]() {a.deallocate(pp, 10);});
    const auto r = rng(p, 10);
    const auto rr = rng(pp, 10);

#ifndef RE_NOEXCEPT
    {
      const int n = 1;
      a.uninitialized_fill(r, n);
      a.uninitialized_fill(rr, n + 1);
      for (const auto &x : r)
        assert(x == n);
      for (const auto &x : rr)
        assert(x == n + 1);

      a.fully_move(r, begin(rr));
      for (const auto &x : rr)
        assert(x == n);

      a.destroy(rr);
    }
#endif
  }

  // new_1
  // new_n
  // delete_1
  // delete_n
  {
    using alw_t = allocator_wrapper<test_allocator<instance_counter<int>>>;
    alw_t a;
    {
      auto p = a.new_1(1);
      assert(*p == 1);
      assert(p->count() == 1);
      a.delete_1(p);
    }
    {
      auto p = a.new_n(5);
      const auto r = rng(p, 5);
      test_rng(r, rng(5, 0));
      assert(instance_counter<int>::count() == 5);
      a.delete_n(begin(r), size(r));
      assert(instance_counter<int>::count() == 0);
    }
    {
      auto p = a.new_n(5, 1);
      const auto r = rng(p, 5);
      test_rng(r, rng(5, 1));
      assert(instance_counter<int>::count() == 5);
      a.delete_n(begin(r), size(r));
      assert(instance_counter<int>::count() == 0);
    }
  }

  // new_node
  // delete_node
  {
    using t = instance_counter<int>;
    struct node_t {
      alignas(alignof(t)) byte data[sizeof(t)];
    };
    using alw_t = allocator_wrapper<test_allocator<t>>;
    {
      alw_t a;
      node_t *const p = a.new_node<node_t>(5);
      assert(reinterpret_cast<t &>(p->data) == 5);
      assert(t::count() == 1);
      a.delete_node(p);
      assert(t::count() == 0);
    }
  }

  // temporary
  // make_temporary
  {
    {
      struct t {
        alignas(256) char holder;
      };
      static_assert(alignof(t) == 256);
      static_assert(alignof(default_alloc_wrapper<t>::temporary) == 256);
    }
    {
      using alw_t = allocator_wrapper<test_allocator<instance_counter<int>>>;
      alw_t a;
      {
        assert(instance_counter<int>::count() == 0);
        alw_t::temporary x = a.make_temporary(1);
        assert(*x == 1);
        assert(*as_const(x) == 1);
        static_assert(same_as
                      <decltype(*as_const(x)), const instance_counter<int> &>);
        assert(*x.operator ->() == 1);
        assert(*as_const(x).operator ->() == 1);
        assert(instance_counter<int>::count() == 1);
      }
      assert(instance_counter<int>::count() == 0);
    }
  }

  // uninitialized
  // make_uninitialized
  {
    using alw_t = allocator_wrapper
      <stateful_test_allocator<instance_counter<int>>>;
    using t = alw_t::uninitialized;
    static_assert(!default_initializable<t>);
    static_assert(movable<t>);
    static_assert(!copyable<t>);
    static_assert(swappable<t>);
    alw_t a{};
    {
      t x = a.make_uninitialized(10);
      assert(x.size() == 10);
      a.uninitialized_copy(irng(0, 10), begin(x));
      test_rng(x, irng(0, 10));
      assert(instance_counter<int>::count() == 10);
      a.destroy(x);
    }
    {
      t x = a.make_uninitialized(10);
      a.uninitialized_copy(irng(0, 10), begin(x));
      test_rng(x, irng(0, 10));
      assert(instance_counter<int>::count() == 10);

      t y = move(x);
      assert(empty(x) && begin(x) == nullptr);
      test_rng(y, irng(0, 10));
      a.destroy(y);
      assert(instance_counter<int>::count() == 0);

      a.uninitialized_copy(irng(0, 10), begin(y));
      y = move(y);
      test_rng(y, irng(0, 10));
      assert(instance_counter<int>::count() == 10);
      a.destroy(y);
    }
    {
      t x = a.make_uninitialized(10);
      t y = a.make_uninitialized(10);
      a.uninitialized_copy(irng(0, 10), begin(x));
      a.uninitialized_copy(irng(10, 20), begin(y));
      assert(instance_counter<int>::count() == 20);
      adl_swap(x, y);
      test_rng(x, irng(10, 20));
      test_rng(y, irng(0, 10));
      a.destroy(x);
      a.destroy(y);
      assert(instance_counter<int>::count() == 0);
    }
  }

  // pointer_pair
  // new_array
  // new_array_move_individually
  // delete_array
  {
    {
      using alw_t = allocator_wrapper<test_allocator<instance_counter<int>>>;
      alw_t a;
      static_assert
        (same_as<alw_t::pointer_pair, iter_pair<alw_t::traits::pointer>>);
      assert(instance_counter<int>::count() == 0);
      alw_t::pointer_pair p = a.new_array(10);
      assert(instance_counter<int>::count() == 10);
      a.delete_array(p);
      assert(instance_counter<int>::count() == 0);

      p = a.new_array(0);
      assert(empty(p) && begin(p) == nullptr);
      p = a.new_array(0, 1);
      assert(empty(p) && begin(p) == nullptr);
      p = a.new_array(rng(0, 1));
      assert(empty(p) && begin(p) == nullptr);

      p = a.new_array(3, 10);
      assert(instance_counter<int>::count() == 3);
      test_rng(p, rng(3, 10));
      a.delete_array(p);
      assert(instance_counter<int>::count() == 0);

      p = a.new_array(irng(1, 20));
      assert(instance_counter<int>::count() == 19);
      test_rng(p, irng(1, 20));
      a.delete_array(p);
      assert(instance_counter<int>::count() == 0);
    }
    {
      using t = instance_counter<test_object<int>>;
      using alw_t = allocator_wrapper<stateful_test_allocator<t>>;

      alw_t a, a2;
      assert(a != a2);
      alw_t::pointer_pair p{}, pp{};
      assert(empty(p) && begin(p) == nullptr);
      assert(empty(pp) && begin(pp) == nullptr);
      p = a.new_array_move_individually(pp);
      assert(empty(p) && begin(p) == nullptr);
      assert(empty(pp) && begin(pp) == nullptr);

      p = a.new_array(irng(0, 5));
      pp = a.new_array_move_individually(p, a);
      assert(empty(p) && begin(p) == nullptr);
      test_rng(deref_rng(pp), irng(0, 5));
      a.delete_array(pp);
      p = pp = {};
      assert(empty(p) && begin(p) == nullptr);
      assert(empty(pp) && begin(pp) == nullptr);

      p = a.new_array(irng(0, 5));
      pp = a2.new_array_move_individually(p, a);
      assert(empty(p) && begin(p) == nullptr);
      test_rng(deref_rng(pp), irng(0, 5));
      a2.delete_array(pp);
    }
    {
#ifndef RE_NOEXCEPT
      using t = exception_countdown<test_object<int>>;
      using alw_t = allocator_wrapper<stateful_test_allocator<t>>;
      alw_t a, a2;
      assert(a != a2);
      auto p = a.new_array(irng(0, 10));
      test_rng(deref_rng(p), irng(0, 10));
      t::copy_constructor_countdown(4);
      iter_pair<alw_t::traits::pointer> pp{};
      try {
        pp = a2.new_array_move_individually(p, a.get());
      }
      catch (const logic_error &) {}
      assert(empty(pp) && begin(pp) == nullptr);
      test_rng(deref_rng(p), irng(0, 10));

      pp = a.new_array_move_individually(p);
      test_rng(deref_rng(pp), irng(0, 10));
      assert(empty(p) && begin(p) == nullptr);

      a.delete_array(pp);
#endif
    }
  }

  // default_alloc_wrapper
  {
    using t = default_alloc_wrapper<int>;
    using tt = allocator_wrapper<default_allocator<int>>;
    static_assert(same_as<t, tt>);
  }
}
void test_unique_ptr() {
  // alloc_delete
  {
    using t = instance_counter<int>;
    using alloc_t = stateful_test_allocator<t>;
    using d_t = alloc_delete<alloc_t>;
    static_assert(semiregular<d_t>);
    static_assert(is_nothrow_swappable<d_t>);
    alloc_t a;
    d_t d(a);
    static_assert(same_as<d_t::allocator_type, alloc_t>);
    assert(d.get_allocator() == a);

    using tt = instance_counter<int *>;
    using alloc2_t = stateful_test_allocator<tt>;
    using d2_t = alloc_delete<alloc2_t>;
    d2_t d2(a);
    assert(d2.get_allocator() == d.get_allocator());
    static_assert(is_convertible<const d2_t &, d_t>);
    static_assert(is_convertible<const d_t &, d2_t>);
    assert(d2_t(d).get_allocator() == d.get_allocator());

    auto p = allocator_wrapper<alloc_t>(a).new_1(3);
    assert(*p == 3 && t::count() == 1);
    d(p);
    assert(t::count() == 0);

    iter_pair<decltype(p)> pp = allocator_wrapper<alloc_t>(a).new_array(5);
    assert(t::count() == 5);
    d(pp);
    assert(t::count() == 0);
  }
  // default_delete
  // inner::unique_ptr_pointer_type_t
  {
    struct d0_t {};
    struct d_t {
      using pointer = int **;
    };
    static_assert(is_same
                  <inner::unique_ptr_pointer_type_t<int, d0_t>, int *>);
    static_assert(is_same
                  <inner::unique_ptr_pointer_type_t<int, d_t>, int **>);
  }
  // empty base optimization
  {
    static_assert(sizeof(unique_ptr<int>)
                  == sizeof(unique_ptr<int, alloc_delete<allocator<int>>>));
    static_assert(sizeof(unique_ptr<int>)
                  < sizeof(unique_ptr
                           <int,
                            alloc_delete<stateful_test_allocator<int>>>));
  }
  // member types
  {
    using d_t = alloc_delete<allocator<int>>;
    using unique_t = unique_ptr<int, d_t>;
    static_assert(same_as<unique_t::pointer, int *>);
    static_assert(same_as<unique_t::element_type, int>);
    static_assert(same_as<unique_t::deleter_type, d_t>);
  }
  // special member functions and swap
  {
    using alloc_t = stateful_test_allocator<int>;
    using unique_t = unique_ptr<int, alloc_delete<alloc_t>>;
    static_assert(default_initializable<unique_t>);
    static_assert(movable<unique_t>);
    static_assert(!copyable<unique_t>);
    static_assert(is_nothrow_default_constructible<unique_t>);
    static_assert(is_destructible<unique_t>);
    static_assert(!is_copy_constructible<unique_t>);
    static_assert(!is_copy_assignable<unique_t>);
    static_assert(is_nothrow_move_constructible<unique_t>);
    static_assert(is_nothrow_move_assignable<unique_t>);
    static_assert(is_nothrow_swappable<unique_t>);

    alloc_t a{};
    unique_t u(allocator_wrapper<alloc_t>(a).new_1(2), a);
    assert(*u == 2 && u.get_deleter().get_allocator() == a);

    unique_t uu(move(u));
    assert(u == nullptr && uu != nullptr && *uu == 2);
    assert(u.get_deleter().get_allocator() == a);
    assert(u.get_deleter().get_allocator()
           == uu.get_deleter().get_allocator());

    u = move(uu);
    u = move(u);
    adl_swap(u, u);
    assert(uu == nullptr && u != nullptr && *u == 2);
    assert(u.get_deleter().get_allocator() == a);
    assert(u.get_deleter().get_allocator()
           == uu.get_deleter().get_allocator());

    alloc_t a2;
    assert(a != a2);
    u = unique_t(nullptr, a);
    uu = unique_t(nullptr, a2);
    assert(!u && !uu);
    assert(u.get_deleter().get_allocator() == a);
    assert(uu.get_deleter().get_allocator() == a2);
    u = move(uu);
    assert(!u && !uu);
    assert(u.get_deleter().get_allocator() == a2);
    assert(uu.get_deleter().get_allocator() == a2);
  }
  {
    using t = instance_counter<int>;
    using alloc_t = test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    using u_t = unique_ptr<t, alloc_delete<alloc_t>>;
    for (int i : ez_vector{0, 1, 2})
      for (int j : ez_vector{0, 1, 2}) {
        auto l_init = [=](u_t *p) {
          i == 0 ? ::new(p) u_t(nullptr) : ::new(p) u_t(alw_t{}.new_1(i));
        };
        auto l_eq = [=](u_t *p) {
          return i == 0 ? p->get() == nullptr : **p == i;
        };
        auto r_init = [=](u_t *p) {
          j == 0 ? ::new(p) u_t(nullptr) : ::new(p) u_t(alw_t{}.new_1(j));
        };
        auto r_eq = [=](u_t *p) {
          return j == 0 ? p->get() == nullptr : **p == j;
        };
        auto mty = [](u_t *p) {return p->get() == nullptr;};
        test_move<u_t>(l_init, l_eq, r_init, r_eq, mty);
        test_swap<u_t>(l_init, l_eq, r_init, r_eq);
      }
    assert(t::count() == 0);
  }
  // non-special constructors and assignment operators
  {
    using t = instance_counter<int>;
    using alloc_t = stateful_test_allocator<t>;
    using d_t = alloc_delete<alloc_t>;
    using u_t = unique_ptr<t, d_t>;
    using alloc0_t = test_allocator<t>;
    using d0_t = alloc_delete<alloc0_t>;
    using u0_t = unique_ptr<t, d0_t>;
    {
      u0_t u(allocator_wrapper(alloc0_t{}).new_1(1));
      assert(u && *u == 1);
    }
    assert(t::count() == 0);
    {
      alloc_t a{};
      d_t d(a);
      u_t u(allocator_wrapper{a}.new_1(1), d);
      assert(u && *u == 1);
    }
    assert(t::count() == 0);
    {
      alloc_t a{};
      d_t d(a);
      u_t u(allocator_wrapper{a}.new_1(1), move(d));
      assert(u && *u == 1);
    }
    assert(t::count() == 0);
  }
  {
    using t = instance_counter<int>;
    using d_t = void (*)(t *);
    using alloc_t = test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    using u_t = unique_ptr<t, d_t>;

    {
      d_t d = [](t *p) {alw_t{}.delete_1(p);};
      u_t u(alw_t{}.new_1(6), d);
      assert(u && *u == 6);
    }
    assert(t::count() == 0);
  }
  {
    struct t {
      instance_counter<int> x;

      t() = default;
      virtual ~t() {}
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;
    };
    struct tt : t {
      instance_counter<long> y;
    };

    using alloc_t = stateful_test_allocator<t>;
    using alloc2_t = stateful_test_allocator<tt>;
    using d_t = alloc_delete<alloc_t>;
    using d2_t = alloc_delete<alloc2_t>;
    using u_t = unique_ptr<t, d_t>;
    using u2_t = unique_ptr<tt, d2_t>;
    using alw_t = allocator_wrapper<alloc_t>;
    using alw2_t = allocator_wrapper<alloc2_t>;
    alw_t a{};
    alw2_t a2{};
    assert(a != a2);
    {
      u2_t u2(a2.new_1(), a2);
      u2->x = 5;
      u2->y = 6;
      u_t u = move(u2);
      assert(u && u->x == 5 && static_cast<tt *>(u.get())->y == 6);
      assert(u.get_deleter().get_allocator() == a2);
      assert(!u2);
      assert(u2.get_deleter().get_allocator() == a2);
    }
    {
      u2_t u2(a2.new_1(), a2);
      u2->x = 5;
      u2->y = 6;
      u_t u(a.new_1(), a);
      u->x = 3;
      assert(addressof(u = move(u2)) == addressof(u));
      assert(u && u->x == 5 && static_cast<tt *>(u.get())->y == 6);
      assert(u.get_deleter().get_allocator() == a2);
      assert(!u2);
      assert(u2.get_deleter().get_allocator() == a2);
    }
    assert(instance_counter<int>::count() == 0);
    assert(instance_counter<long>::count() == 0);
  }
  // access the stored pointer
  {
    unique_ptr<int> p(default_alloc_wrapper<int>{}.new_1(3));
    static_assert(same_as<decltype(p.operator ->()), int *>);
    assert(*(p.operator ->()) == 3);
    static_assert(same_as<decltype(*p), int &>);
    assert(*p == 3);
    static_assert(same_as<decltype(p.get()), int *>);
    assert(*p.get() == 3);
    {
      bool y = false;
      if (p)
        y = true;
      assert(y);
    }
    assert(p);

    unique_ptr<int> pp;
    assert(pp.operator ->() == nullptr);
    assert(pp.get() == nullptr);
    assert(!pp);
  }
  // access the stored deleter
  {
    unique_ptr<int> p;
    using d_t = default_delete<int>;
    static_assert(same_as<decltype(p.get_deleter()), d_t &>);
    static_assert(same_as<decltype(as_const(p).get_deleter()), const d_t &>);

    using alloc_t = stateful_test_allocator<int>;
    alloc_t a{};
    unique_ptr<int, alloc_delete<alloc_t>> pp(nullptr, a);
    assert(pp.get_deleter().get_allocator() == a);
    assert(as_const(pp).get_deleter().get_allocator() == a);
  }
  // release
  // reset
  {
    using alloc_t = stateful_test_allocator<int>;
    using d_t = alloc_delete<alloc_t>;
    using u_t = unique_ptr<int, d_t>;
    u_t p;
    assert(p.release() == nullptr);
    assert(p.get() == nullptr);

    p.reset(allocator_wrapper(p.get_deleter().get_allocator()).new_1(3));
    assert(p && *p == 3);
    p.reset(p.release());
    assert(p && *p == 3);
    int *pp = p.release();
    assert(!p);
    allocator_wrapper(p.get_deleter().get_allocator()).delete_1(pp);
  }
  // comparation
  {
    unique_ptr<int> p = make_unique<int>(1);
    unique_ptr<int> pp = make_unique<int>(2);
    assert((p <=> pp) == (p.get() <=> pp.get()));
    assert(p != pp);
    test_equality(p, p);
    test_equality(pp, pp);
    test_lessness(nullptr, p);
    p.reset();
    test_equality(p, p);
  }
  // make_unique
  {
    unique_ptr<int> p = make_unique<int>(3);
    assert(p && *p == 3);

    allocator_wrapper<test_allocator<unique_ptr<int>>> a;
    auto x = a.make_uninitialized(500);
    initialize(x, [](auto pp) {::new(pp) unique_ptr<int>(make_unique<int>());});
    for (auto &y : x)
      assert(y && *y == 0);
    initialize(x, [](auto pp) {pp->~unique_ptr<int>();});
  }
  {
    using t = instance_counter<int>;
    using alloc_t = stateful_test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    alw_t a{};
    alw_t::unique_ptr p = a.make_unique();
    assert(p && *p == 0);
    assert(t::count() == 1);
    alw_t::unique_ptr pp = a.make_unique(1);
    assert(pp && *pp == 1);
    assert(t::count() == 2);
  }

  // array version
  {
    unique_ptr<int []> p(new int[2]);
    p[0] = 1;
    p[1] = 2;
    assert(p[0] == 1 && p[1] == 2);
    p = make_unique<int []>(0);
    assert(p.get() != nullptr);
    p = make_unique<int []>(1);
    assert(p.get() != nullptr);
    p[0] = 1;
    assert(p[0] == 1);
  }
}
void test_unique_array() {
  // size_type
  {
    using d0_t = alloc_delete<allocator<int>>;
    static_assert(same_as
                  <inner::unique_array_size_type_t<int, d0_t>, size_t>);

    struct a_t {
      using value_type = int;
      using pointer = int *;
      using size_type = unsigned long;
    };
    using d_t = alloc_delete<a_t>;
    static_assert(same_as
                  <inner::unique_array_size_type_t<int, d_t>, unsigned long>);

    struct d2_t {};
    static_assert(same_as<inner::unique_array_size_type_t<int, d2_t>, size_t>);
  }
  // member types
  {
    using t = unique_array<int>;
    static_assert(same_as<t::pointer, int *>);
    static_assert(same_as<t::size_type, size_t>);
    static_assert(same_as<t::difference_type, ptrdiff_t>);
    static_assert(same_as<t::element_type, int>);
    static_assert(same_as<t::reference, int &>);
    static_assert(same_as<t::const_reference, const int &>);
  }
  // special member functions and swap
  {
    using alloc_t = stateful_test_allocator<int>;
    using unique_t = unique_array<int, alloc_delete<alloc_t>>;
    static_assert(default_initializable<unique_t>);
    static_assert(movable<unique_t>);
    static_assert(!copyable<unique_t>);
    static_assert(is_nothrow_default_constructible<unique_t>);
    static_assert(is_destructible<unique_t>);
    static_assert(!is_copy_constructible<unique_t>);
    static_assert(!is_copy_assignable<unique_t>);
    static_assert(is_nothrow_move_constructible<unique_t>);
    static_assert(is_nothrow_move_assignable<unique_t>);
    static_assert(is_nothrow_swappable<unique_t>);

    alloc_t a{};
    unique_t u(allocator_wrapper<alloc_t>(a).new_array(3, 4), a);
    test_rng(u, rng(3, 4));
    assert(u.get_deleter().get_allocator() == a);

    unique_t uu(move(u));
    assert(u.empty());
    test_rng(uu, rng(3, 4));
    assert(u.get_deleter().get_allocator() == a);
    assert(u.get_deleter().get_allocator()
           == uu.get_deleter().get_allocator());

    u = move(uu);
    u = move(u);
    adl_swap(u, u);
    assert(uu.empty());
    test_rng(u, rng(3, 4));
    assert(u.get_deleter().get_allocator() == a);
    assert(u.get_deleter().get_allocator()
           == uu.get_deleter().get_allocator());

    alloc_t a2;
    assert(a != a2);
    u = unique_t({}, a);
    uu = unique_t({}, a2);
    assert(u.empty() && uu.empty());
    assert(u.get_deleter().get_allocator() == a);
    assert(uu.get_deleter().get_allocator() == a2);
    u = move(uu);
    assert(u.empty() && uu.empty());
    assert(u.get_deleter().get_allocator() == a2);
    assert(uu.get_deleter().get_allocator() == a2);
  }
  {
    using t = instance_counter<int>;
    using alloc_t = test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    using u_t = unique_array<t, alloc_delete<alloc_t>>;
    for (int i : ez_vector{0, 1, 2})
      for (int j : ez_vector{0, 1, 2}) {
        auto l_init = [=](u_t *p) {
          i == 0 ? ::new(p) u_t{} : ::new(p) u_t(alw_t{}.new_array(i, 1));
        };
        auto l_eq = [=](u_t *p) {
          test_rng(*p, rng(i, 1));
          return p->size() == to_unsigned(i) && (p->empty() == (i == 0));
        };
        auto r_init = [=](u_t *p) {
          j == 0 ? ::new(p) u_t{} : ::new(p) u_t(alw_t{}.new_array(j, 2));
        };
        auto r_eq = [=](u_t *p) {
          test_rng(*p, rng(j, 2));
          return p->size() == to_unsigned(j) && (p->empty() == (j == 0));
        };
        auto mty = [](u_t *p) {return p->empty() && p->begin() == nullptr;};
        test_move<u_t>(l_init, l_eq, r_init, r_eq, mty);
        test_swap<u_t>(l_init, l_eq, r_init, r_eq);
      }
    assert(t::count() == 0);
  }
  // non-special constructors and assignment operators
  {
    using t = instance_counter<int>;
    using alloc_t = stateful_test_allocator<t>;
    using d_t = alloc_delete<alloc_t>;
    using u_t = unique_array<t, d_t>;
    using alloc0_t = test_allocator<t>;
    using d0_t = alloc_delete<alloc0_t>;
    using u0_t = unique_array<t, d0_t>;
    {
      u0_t u(allocator_wrapper(alloc0_t{}).new_array(1, 1));
      assert(!u.empty() && size(u) == 1 && ref(u, 0) == 1);
    }
    assert(t::count() == 0);
    {
      alloc_t a{};
      d_t d(a);
      u_t u(allocator_wrapper{a}.new_array(1, 1), d);
      assert(!u.empty() && size(u) == 1 && ref(u, 0) == 1);
    }
    assert(t::count() == 0);
    {
      alloc_t a{};
      d_t d(a);
      u_t u(allocator_wrapper{a}.new_array(3, 1), move(d));
      test_rng(u, rng(3, 1));
    }
    assert(t::count() == 0);
  }
  {
    using t = instance_counter<int>;
    using d_t = void (*)(iter_pair<t *>);
    using alloc_t = test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    using u_t = unique_array<t, d_t>;

    {
      d_t d = [](iter_pair<t *> p) {alw_t{}.delete_array(p);};
      u_t u(alw_t{}.new_array(1, 6), d);
      assert(!u.empty() && u.size() == 1 && ref(u, 0) == 6);
    }
    {
      d_t d = [](iter_pair<t *> p) {alw_t{}.delete_array(p);};
      u_t u(alw_t{}.new_array(4, 6), d);
      test_rng(u, rng(4, 6));
    }
    assert(t::count() == 0);
  }
  {
    using t = instance_counter<int>;
    using alloc_t = test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    struct d_t {
      void operator ()(iter_pair<t *> p) const {
        alw_t{}.delete_array(p);
      }
    };
    struct dd_t : d_t {};
    using u_t = unique_array<t, d_t>;
    using uu_t = unique_array<t, dd_t>;

    assert(t::count() == 0);
    {
      u_t uu(alw_t{}.new_array(3, 4));
      u_t u(move(uu));
      assert(uu.empty());
      test_rng(u, rng(3, 4));
    }
    assert(t::count() == 0);
    {
      u_t uu(alw_t{}.new_array(3, 4));
      u_t u;
      u = move(uu);
      assert(uu.empty());
      test_rng(u, rng(3, 4));
    }
    assert(t::count() == 0);
  }
  // range operations
  {
    unique_array<int> u;
    assert(u.empty());
    assert(u.size() == 0);
    assert(u.begin() == u.end());
    assert(u.begin() == nullptr);
    u = make_array<int>(3, 4);
    assert(!u.empty());
    assert(u.size() == 3);
    assert(u.begin() + 3 == u.end());
    test_rng(u, rng(3, 4));
    assert(u.front() == 4);
    assert(as_const(u).front() == 4);
    assert(u.back() == 4);
    assert(as_const(u).back() == 4);
    assert(u[1] == 4);
    assert(as_const(u)[1] == 4);
  }
  // access the stored deleter
  {
    unique_array<int> p;
    using d_t = default_delete<int>;
    static_assert(same_as<decltype(p.get_deleter()), d_t &>);
    static_assert(same_as<decltype(as_const(p).get_deleter()), const d_t &>);

    using alloc_t = stateful_test_allocator<int>;
    alloc_t a{};
    unique_array<int, alloc_delete<alloc_t>> pp({}, a);
    assert(pp.get_deleter().get_allocator() == a);
    assert(as_const(pp).get_deleter().get_allocator() == a);
  }
  // release
  // reset
  {
    using alloc_t = stateful_test_allocator<int>;
    using d_t = alloc_delete<alloc_t>;
    using u_t = unique_array<int, d_t>;
    u_t p;
    const auto x = p.release();
    assert(x.first == nullptr && x.second == 0);

    p.reset(allocator_wrapper(p.get_deleter().get_allocator()).new_array(3, 4));
    assert(!p.empty());
    test_rng(p, rng(3, 4));
    p.reset(p.release());
    assert(!p.empty());
    test_rng(p, rng(3, 4));
    iter_pair<int *> pp = p.release();
    assert(p.empty());
    p.get_deleter()(pp);
  }
  // comparation
  {
    static_assert(equality_comparable<unique_array<int>>);
    static_assert(three_way_comparable<unique_array<int>>);
    static_assert(!three_way_comparable
                  <unique_array<int>, unique_array<const int>>);

    unique_array<int> u;
    test_equality(u, u);
    u = make_array<int>(ez_vector{1, 2, 3, 4});
    test_equality(u, u);

    test_lessness(make_array<int>(ez_vector<int>{}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_lessness(make_array<int>(ez_vector{1}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_lessness(make_array<int>(ez_vector{2, 2}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_lessness(make_array<int>(ez_vector{2, 3, 3}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_lessness(make_array<int>(ez_vector{1, 2, 3}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_equality(make_array<int>(ez_vector{2, 3, 4}),
                  make_array<int>(ez_vector{2, 3, 4}));
    test_equality(make_array<int>(ez_vector<int>{}),
                  make_array<int>(ez_vector<int>{}));
    test_inequality(make_array<int>(ez_vector{2, 3, 4}),
                    make_array<int>(ez_vector{2, 4, 4}));
    test_inequality(make_array<int>(ez_vector{2, 4}),
                    make_array<int>(ez_vector{2, 4, 4}));
  }
  // make_array
  {
    unique_array<int> p = make_array<int>(0);
    assert(p.empty());

    p = make_array<int>(200);
    test_rng(p, rng(200, 0));

    p = make_array<int>(3, 4);
    test_rng(p, rng(3, 4));

    p = make_array<int>(rng(4, 5));
    test_rng(p, rng(4, 5));
  }
  {
    using t = instance_counter<int>;
    using alloc_t = stateful_test_allocator<t>;
    using alw_t = allocator_wrapper<alloc_t>;
    assert(t::count() == 0);
    {
      alw_t a;
      alw_t::unique_array p = a.make_array(0);
      assert(p.empty());
      assert(t::count() == 0);

      p = a.make_array(10);
      test_rng(p, rng(10, 0));
      assert(t::count() == 10);

      p = a.make_array(6, 7);
      test_rng(p, rng(6, 7));
      assert(t::count() == 6);

      p = a.make_array(ez_vector{1, 2, 3, 4});
      test_rng(p, ez_vector{1, 2, 3, 4});
      assert(t::count() == 4);
    }
    assert(t::count() == 0);
  }
}
void test_copyable_ptr() {
  using t = instance_counter<int>;
  static_assert(same_as<copyable_ptr<t>::value_type, t>);
  static_assert(semiregular<copyable_ptr<t>>);

  // special member functions and constructor
  {
    ez_vector<pair<ez_function<void (copyable_ptr<t> *)>,
                   ez_function<bool (copyable_ptr<t> *)>>> v;
    v.insert(v.end(), {
        [](copyable_ptr<t> *p) {new(p) copyable_ptr<t>{};},
        [](copyable_ptr<t> *p) {return *p == nullptr;}
      });
    v.insert(v.end(), {
        [](copyable_ptr<t> *p) {
          new(p) copyable_ptr<t>(in_place, 1);
        },
        [](copyable_ptr<t> *p) {
          return *p != nullptr && **p == 1;
        }
      });
    v.insert(v.end(), {
        [](copyable_ptr<t> *p) {
          new(p) copyable_ptr<t>(make_unique<t>(2).release());
        },
        [](copyable_ptr<t> *p) {
          return *p != nullptr && **p == 2;
        }
      });
    for (auto &x : v)
      for (auto &y : v)
        test_ownership<copyable_ptr<t>>(x.first, x.second,
                                        y.first, y.second,
                                        bind(empty, bind(deref, _1)));
  }
  assert(t::count() == 0);

  // operations with nullptr
  // operator bool
  // empty
  // release
  // reset
  // clear
  // emplace
  // ->
  // *
  // get
  {
    copyable_ptr<t> p;
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.emplace(1);
    assert(test_unequal(p, nullptr) && static_cast<bool>(p) && !p.empty());
    assert(*p == 1);
    p.emplace(2);
    assert(test_unequal(p, nullptr) && static_cast<bool>(p) && !p.empty());
    assert(*p == 2);
    p = nullptr;
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.reset(make_unique<t>(3).release());
    assert(test_unequal(p, nullptr) && static_cast<bool>(p) && !p.empty());
    assert(*p == 3);
    p.reset(make_unique<t>(4).release());
    assert(test_unequal(p, nullptr) && static_cast<bool>(p) && !p.empty());
    assert(*p == 4);
    p.reset(nullptr);
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.reset(nullptr);
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.clear();
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.emplace(1);
    p.clear();
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.emplace(1);
    p.reset();
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    assert(p.release() == nullptr);
    assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
    p.emplace(1);
    {
      t *pp = p.release();
      const auto guard = exit_fn([pp]() {unique_ptr<t> tmp(pp);});
      assert(*pp == 1);
      assert(test_equal(p, nullptr) && !static_cast<bool>(p) && p.empty());
      assert(p.release() == nullptr);
      assert(p.empty());
    }

    p.emplace(1);
    assert(*p.operator ->() == 1);
    assert(*p == 1);
    assert(*p.get() == 1);
  }
  assert(t::count() == 0);
}
namespace help_test_copyable_array {

const auto equal = [](const auto &r, const auto &rr) {
  if (size(r) == size(rr)) {
    auto i2 = begin(rr);
    for (auto &i : iters(r)) {
      if (!(*i == *i2))
        return false;
      ++i2;
    }
  }
  return true;
};

}
void test_copyable_array() {
  using t = instance_counter<int>;
  static_assert(same_as<copyable_array<t>::pointer, t *>);
  static_assert(same_as<copyable_array<t>::element_type, t>);
  static_assert(same_as<copyable_array<t>::value_type, t>);
  static_assert(same_as<copyable_array<t>::reference, t &>);
  static_assert(same_as<copyable_array<t>::const_reference, const t &>);
  static_assert(same_as<copyable_array<t>::difference_type, ptrdiff_t>);
  static_assert(same_as<copyable_array<t>::size_type, size_t>);
  static_assert(same_as<copyable_array<t>::pointer_pair, iter_pair<t *>>);
  static_assert(semiregular<copyable_array<t>>);

  using help_test_copyable_array::equal;

  // special member functions and constructor
  {
    ez_vector<pair<ez_function<void (copyable_array<t> *)>,
                   ez_function<bool (copyable_array<t> *)>>> v;
    v.insert(v.end(), {
        [](copyable_array<t> *p) {new(p) copyable_array<t>{};},
        [](copyable_array<t> *p) {return (*p).empty();}
      });
    v.insert(v.end(), {
        [](copyable_array<t> *p) {
          new(p) copyable_array<t>
            (make_array<t>(ez_vector<t>{1, 2, 3}).release());
        },
        [](copyable_array<t> *p) {
          return !(*p).empty() && equal(*p, irng(1, 4));
        }
      });
    v.insert(v.end(), {
        [](copyable_array<t> *p) {
          new(p) copyable_array<t>(3);
        },
        [](copyable_array<t> *p) {
          return !(*p).empty() && equal(*p, rng(3, 0));
        }
      });
    v.insert(v.end(), {
        [](copyable_array<t> *p) {
          new(p) copyable_array<t>(3, 2);
        },
        [](copyable_array<t> *p) {
          return !(*p).empty() && equal(*p, rng(3, 2));
        }
      });
    v.insert(v.end(), {
        [](copyable_array<t> *p) {
          new(p) copyable_array<t>(from_range, ez_vector{4, 5});
        },
        [](copyable_array<t> *p) {
          return !(*p).empty() && equal(*p, irng(4, 6));
        }
      });
    for (auto &x : v)
      for (auto &y : v)
        test_ownership<copyable_array<t>>(x.first, x.second,
                                          y.first, y.second,
                                          bind(empty, bind(deref, _1)));
  }
  assert(t::count() == 0);

  // release
  {
    copyable_array<t> a;
    assert(equal(a.release(), iter_pair<t *>{}));
    a = {from_range, irng(1, 5)};
    assert(equal(a, irng(1, 5)));
    iter_pair<t *> x = a.release();
    const auto guard = exit_fn([x]() {unique_array<t> tmp(x);});
    assert(equal(x, irng(1, 5)) && a.empty());
  }
  assert(t::count() == 0);

  // reset
  {
    copyable_array<t> a;
    a.reset(make_array<t>(irng(1, 4)).release());
    assert(equal(a, irng(1, 4)));
    a.reset(make_array<t>(irng(4, 6)).release());
    assert(equal(a, irng(4, 6)));
    a.reset(iter_pair<t *>{});
    assert(a.empty());
    a.reset(iter_pair<t *>{});
    assert(a.empty());
    a = {from_range, irng(1, 4)};
    a.reset();
    assert(a.empty());
    a.reset();
    assert(a.empty());
  }
  assert(t::count() == 0);

  // clear
  {
    copyable_array<t> a(from_range, irng(1, 4));
    a.clear();
    assert(a.empty());
    a.clear();
    assert(a.empty());
  }

  // ==
  // <=>
  {
    copyable_array<t> a(from_range, irng(1, 4));
    copyable_array<t> b(from_range, irng(2, 4));
    test_equality(a, a);
    test_lessness(a, b);
    assert(a <=> a == 0);
    static_assert(same_as<decltype(a <=> a), strong_ordering>);
  }

  // begin
  // end
  // cbegin
  // cend
  // empty
  // size
  {
    copyable_array<t> a;
    assert(a.begin() == a.end() && a.empty() && a.size() == 0);
    a = {from_range, irng(1, 3)};
    assert(equal(rng(a.begin(), a.end()), irng(1, 3))
           && equal(rng(a.cbegin(), a.cend()), irng(1, 3))
           && !a.empty() && a.size() == 2);
    assert(a[1] == 2 && as_const(a)[0] == 1);
    assert(a.front() == 1 && as_const(a).front() == 1);
    assert(a.back() == 2 && as_const(a).back() == 2);
  }
}
void test_maybe_owner_ptr() {
  using t = test_object<int>;

  maybe_owner_ptr<t> p0;
  assert(p0.operator ->() == nullptr && p0.get() == nullptr
         && p0.empty() && !p0.owns());
  p0.clear();
  assert(p0.operator ->() == nullptr && p0.get() == nullptr
         && p0.empty() && !p0.owns());
  p0.reset();
  assert(p0.operator ->() == nullptr && p0.get() == nullptr
         && p0.empty() && !p0.owns());

  maybe_owner_ptr<t> p1(in_place, 1);
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());
  p1 = move(p1);
  p1 = p1;
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());

  maybe_owner_ptr<t> p2 = p1;
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());
  assert(p2.get() != nullptr && *p2 == 1 && !p2.empty() && !p2.owns());
  p1 = p2;
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());
  assert(p2.get() != nullptr && *p2 == 1 && !p2.empty() && !p2.owns());
  p2 = p2;
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());
  assert(p2.get() != nullptr && *p2 == 1 && !p2.empty() && !p2.owns());

  maybe_owner_ptr<t> p3(nullptr);
  p3 = p1;
  p1 = move(p3);
  assert(p1.get() != nullptr && *p1 == 1 && !p1.empty() && p1.owns());
  assert(p2.get() != nullptr && *p2 == 1 && !p2.empty() && !p2.owns());
  assert(p3.get() != nullptr && *p3 == 1 && !p3.empty() && !p3.owns());
  p3 = move(p1);
  assert(p1 == nullptr && !p1.owns());
  assert(p2.get() != nullptr && *p2 == 1 && !p2.empty() && !p2.owns());
  assert(p3.get() != nullptr && *p3 == 1 && !p3.empty() && p3.owns());

  p3 = nullptr;
  p2 = nullptr;
  assert(p2 == nullptr && p3 == nullptr);
}
void test_buffer() {
  // compile-time traits
  {
    using buf_t = buffer<int, test_allocator<int>>;
    static_assert(is_default_constructible<buf_t>);
    static_assert(is_destructible<buf_t>);
    static_assert(!is_copy_constructible<buf_t>);
    static_assert(!is_copy_assignable<buf_t>);
    static_assert(is_move_constructible<buf_t>);
    static_assert(!is_move_assignable<buf_t>);
    static_assert(!swappable<buf_t>);

    static_assert(is_rng<buf_t>);
    static_assert(is_rng<const buf_t>);
    static_assert(rng_is_sized<buf_t>);
    static_assert(is_same<rng_szt<buf_t>, size_t>);

    static_assert(same_as<buf_t::value_type, int>);
    static_assert(same_as<buf_t::reference, int &>);
    static_assert(same_as<buf_t::const_reference, const int &>);
    static_assert(same_as<buf_t::iterator, int *>);
    static_assert(same_as<buf_t::const_iterator, const int *>);
    static_assert(same_as<buf_t::difference_type, ptrdiff_t>);
    static_assert(same_as<buf_t::size_type, size_t>);
    static_assert(same_as<buf_t::allocator_type, test_allocator<int>>);
  }
  // special member functions
  {
    using buf_t = buffer<int, stateful_test_allocator<int>>;
    buf_t buf{};
    assert(buf.empty() && buf.size() == 0);
    assert(buf.capacity() == 0);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    buf_t buf2(move(buf));
    assert(buf.empty() && buf.size() == 0);
    assert(buf.capacity() == 0);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    assert(buf2.empty() && buf2.size() == 0);
    assert(buf2.capacity() == 0);
    assert(buf2.front_raw_space() == 0);
    assert(buf2.back_raw_space() == 0);
    assert(buf2.get_allocator() == buf.get_allocator());
    test_rng(buf, ez_vector<int>{});
    test_rng(as_const(buf), ez_vector<int>{});
  }
  // buffer(alloc);
  {
    using buf_t = buffer<int, stateful_test_allocator<int>>;
    using alloc_t = stateful_test_allocator<int>;
    using buf_t = buffer<int, alloc_t>;
    alloc_t a, aa;
    assert(a != aa);
    buf_t buf(a);
    buf_t buf2(aa);
    assert(buf.get_allocator() == a);
    assert(buf2.get_allocator() == aa);

    assert(buf.empty() && buf2.empty());
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    assert(buf2.front_raw_space() == 0);
    assert(buf2.back_raw_space() == 0);
  }
  // buffer(n);
  {
    using buf_t = buffer<int, stateful_test_allocator<int>>;
    buf_t buf(3);
    for (int &i : iters(1, 4))
      buf.push_back(i);
    assert(!buf.empty() && buf.size() == 3);
    assert(buf.capacity() == 3);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    test_rng(buf, irng(1, 4));
    test_rng(as_const(buf), irng(1, 4));

    buf_t buf2(0);
    assert(buf2.empty() && buf2.size() == 0);
    assert(buf2.capacity() == 0);
    assert(buf2.front_raw_space() == 0);
    assert(buf2.back_raw_space() == 0);
    test_rng(buf2, ez_vector<int>{});
    test_rng(as_const(buf2), ez_vector<int>{});
  }
  // get_allocator
  {
    using alloc_t = stateful_test_allocator<int>;
    using buf_t = buffer<int, alloc_t>;
    buf_t buf{};
    buf_t buf2(buf.get_allocator());
    buf_t buf3{};
    static_assert(same_as<decltype(buf.get_allocator()), alloc_t>);
    assert(buf.get_allocator() == buf2.get_allocator());
    assert(buf.get_allocator() != buf3.get_allocator());
  }
  // range operations
  {
    {
      buffer<int, test_allocator<int>> buf{};
      test_rng(buf, ez_vector<int>{});
      test_ritr(buf, ez_vector<int>{});
      test_ritr(as_const(buf), ez_vector<int>{});
      test_ritr(rrng(buf), ez_vector<int>{});
      test_ritr(rrng(as_const(buf)), ez_vector<int>{});
    }
    {
      buffer<int, test_allocator<int>> buf(10);
      for (int &i : iters(0, 10))
        buf.push_back(i);
      test_rng(buf, irng(0, 10));
      test_ritr(buf, irng(0, 10));
      test_ritr(as_const(buf), irng(0, 10));
      test_ritr(rrng(buf), rrng(irng(0, 10)));
      test_ritr(rrng(as_const(buf)), rrng(irng(0, 10)));

      assert(buf[0] == 0 && as_const(buf)[9] == 9);
      assert(buf.front() == 0 && as_const(buf).front() == 0);
      assert(buf.back() == 9 && as_const(buf).back() == 9);
    }
  }
  // max_size()
  {
    using buf_t = buffer<int, allocator<int>>;
    buf_t buf;
    const auto n = to_unsigned(numeric_limits<ptrdiff_t>::max()) / sizeof(int);
    assert(as_const(buf).max_size() == n);
    assert(buf.max_size() == n);
  }
  // capacity()
  {
    using buf_t = buffer<int, test_allocator<int>>;
    buf_t buf{};
    assert(buf.capacity() == 0);
    buf_t buf2(123);
    assert(buf2.capacity() == 123);
    buf2.reallocate(0);
    assert(buf2.capacity() == 0);
    buf2.reallocate(4567);
    assert(buf2.capacity() == 4567);
  }
  // push_front
  // pop_front
  // push_back
  // pop_back
  // front_raw_space
  // back_raw_space
  {
    buffer<int, test_allocator<int>> buf(20);
    assert(empty(buf));
    assert(size(buf) == 0);
    assert(buf.capacity() == 20);
    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 17);
    buf.pop_back();
    buf.pop_front();
    assert(buf.front_raw_space() == 1);
    assert(buf.back_raw_space() == 18);
    test_rng(buf, ez_vector<int>{2});
    buf.pop_front();
    assert(buf.front_raw_space() == 2);
    assert(buf.back_raw_space() == 18);
    assert(buf.empty() && buf.size() == 0);
    buf.push_back(1);
    buf.pop_back();
    assert(buf.front_raw_space() == 2);
    assert(buf.back_raw_space() == 18);
    assert(buf.empty() && buf.size() == 0);
    buf.push_back(1);
    buf.pop_front();
    assert(buf.front_raw_space() == 3);
    assert(buf.back_raw_space() == 17);
    assert(buf.empty() && buf.size() == 0);
  }
  // reallocate
  // increase_capacity
  {
    buffer<int, test_allocator<int>> buf{};
    buf.reallocate(100);
    assert(buf.capacity() == 100);
    assert(buf.size() == 0 && buf.empty());
    for (int i = 0; i != 100; ++i)
      buf.push_back(i);
    assert(buf.size() == buf.capacity());
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    test_rng(buf, irng(0, 100));
  
    buf.pop_front();
    buf.pop_front();
    buf.increase_capacity(100);
    assert(buf.capacity() == 200);
    for (int i = 2; i != 100; ++i)
      assert(ref(buf, i - 2) == i);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 102);
    buf.reallocate(98);
    for (int i : irng(0, 98))
      assert(ref(buf, i) == i + 2);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);
    buf.increase_capacity(0);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 0);

    buf.reset();
    assert(buf.capacity() == 0 && buf.size() == 0);
    buf.reallocate(20);
    assert(buf.capacity() == 20 && buf.size() == 0);
    assert(buf.front_raw_space() == 0);
    assert(buf.back_raw_space() == 20);
  }
  // reset(n)
  {
    using buf_t
      = buffer<instance_counter<int>, test_allocator<instance_counter<int>>>;
    buf_t buf{};
    buf.reset(5);
    assert(empty(buf) && buf.capacity() == 5);
    buf.reset(1);
    assert(empty(buf) && buf.capacity() == 1);
    buf.reset(1234);
    assert(empty(buf) && buf.capacity() == 1234);
    for (int &x : iters(0, 1234))
      buf.push_back(x);
    assert(instance_counter<int>::count() == 1234);
    buf.reset(1);
    assert(empty(buf) && buf.capacity() == 1);
    buf.reset();
    assert(empty(buf) && buf.capacity() == 0);
  }
  // reserve(n)
  // reserve_more(n)
  {
    buffer<int> buf;
    buf.reserve(0);
    assert(buf.capacity() == 0 && empty(buf));
    buf.reserve(1);
    assert(buf.capacity() == 1 && empty(buf));
    buf.push_back(0);
    assert(buf.capacity() == 1 && ref(buf, 0) == 0 && buf.size() == 1u);
    buf.reserve(0);
    assert(buf.capacity() == 1 && ref(buf, 0) == 0 && buf.size() == 1u);
    buf.reserve(1);
    assert(buf.capacity() == 1 && ref(buf, 0) == 0 && buf.size() == 1u);
    buf.reserve(2);
    assert(buf.capacity() == 2 && ref(buf, 0) == 0 && buf.size() == 1u);
    buf.reserve(1);
    assert(buf.capacity() == 2 && ref(buf, 0) == 0 && buf.size() == 1u);
    buf.reserve(3);
    assert(buf.capacity() == 3 && ref(buf, 0) == 0 && buf.size() == 1u);

    buf.reserve_more(3u);
    assert(buf.capacity() == 4u && ref(buf, 0) == 0 && buf.size() == 1u);
  }
  // clear
  {
    using t = instance_counter<int>;
    using buf_t = buffer<instance_counter<int>, test_allocator<t>>;
    assert(t::count() == 0);
    buf_t buf{};
    buf.clear();
    assert(buf.empty() && buf.capacity() == 0);
    buf.reset(10);
    for (int x : irng(1, 4))
      buf.push_back(x);
    buf.clear();
    assert(buf.empty() && buf.capacity() == 10);
    buf.reset();
    assert(buf.empty() && buf.capacity() == 0);
    assert(t::count() == 0);
  }
  // release
  {
    using t = instance_counter<int>;
    using buf_t = buffer<instance_counter<int>, test_allocator<t>>;
    assert(t::count() == 0);
    buf_t buf(3);
    for (int x : irng(1, 4))
      buf.push_back(x);
    const auto p = buf.begin();
    const auto n = buf.size();
    const auto nn = buf.capacity();
    buf.release();
    assert(buf.empty() && buf.capacity() == 0);

    allocator_wrapper<test_allocator<t>> a;
    a.destroy(rng(p, n));
    a.deallocate(p, nn);
    assert(t::count() == 0);
  }
}
void test_scoped_allocator_adaptor() {
  // inner helper
  {
    struct t {
      void outer_allocator() {}
    };
    static_assert(inner::scoped_alloc_with_mem_fn_outer_allocator<t>);
    static_assert(!inner::scoped_alloc_with_mem_fn_outer_allocator<int>);

    using a1_t = stateful_test_allocator<int>;
    using a2_t = scoped_allocator_adaptor<stateful_test_allocator<int>>;
    using a3_t = scoped_allocator_adaptor<stateful_test_allocator<int>,
                                          stateful_test_allocator<int *>,
                                          stateful_test_allocator<int **>>;
    a1_t a1;
    a2_t a2(a1);
    a3_t a3(a1, {}, {});
    static_assert(same_as
                  <decltype(inner::fns::scoped_alloc_outermost(a1)), a1_t &>);
    static_assert(same_as
                  <decltype(inner::fns::scoped_alloc_outermost(a2)), a1_t &>);
    static_assert(same_as
                  <decltype(inner::fns::scoped_alloc_outermost(a3)), a1_t &>);
    assert(inner::fns::scoped_alloc_outermost(a1) == a1);
    assert(inner::fns::scoped_alloc_outermost(a2) == a1);    
    assert(inner::fns::scoped_alloc_outermost(a3) == a1);

    static_assert(same_as<inner::scoped_alloc_inner_t<>, inner::empty_type>);
    static_assert(same_as<inner::scoped_alloc_inner_t<a1_t>,
                          scoped_allocator_adaptor<a1_t>>);
    static_assert(same_as<inner::scoped_alloc_inner_t<a1_t, a2_t, a3_t>,
                          scoped_allocator_adaptor<a1_t, a2_t, a3_t>>);
  }
  // member types
  {
    using a_t = test_allocator<int>;
    using aa_t = stateful_test_allocator<int>;
    using a1_t = scoped_allocator_adaptor<test_allocator<int>>;
    using a2_t = scoped_allocator_adaptor<test_allocator<int>,
                                          test_allocator<int *>,
                                          test_allocator<int **>>;
    static_assert(same_as<a1_t::outer_allocator_type, test_allocator<int>>);
    static_assert(same_as<a2_t::outer_allocator_type, test_allocator<int>>);
    static_assert(same_as<a1_t::inner_allocator_type, a1_t>);
    static_assert(same_as<a2_t::inner_allocator_type,
                          scoped_allocator_adaptor<test_allocator<int *>,
                                                   test_allocator<int **>>>);
    static_assert(same_as<a1_t::value_type, int>);
    static_assert(same_as<a1_t::size_type, size_t>);
    static_assert(same_as<a1_t::difference_type, ptrdiff_t>);
    static_assert(same_as<a1_t::pointer, int *>);
    static_assert(same_as<a1_t::const_pointer, const int *>);
    static_assert(same_as<a1_t::void_pointer, void *>);
    static_assert(same_as<a1_t::const_void_pointer, const void *>);
    static_assert(same_as<a2_t::value_type, int>);
    static_assert(same_as<a2_t::size_type, size_t>);
    static_assert(same_as<a2_t::difference_type, ptrdiff_t>);
    static_assert(same_as<a2_t::pointer, int *>);
    static_assert(same_as<a2_t::const_pointer, const int *>);
    static_assert(same_as<a2_t::void_pointer, void *>);
    static_assert(same_as<a2_t::const_void_pointer, const void *>);

    static_assert(!a1_t::propagate_on_container_copy_assignment::value);
    static_assert(!a1_t::propagate_on_container_move_assignment::value);
    static_assert(!a1_t::propagate_on_container_swap::value);
    static_assert(a1_t::is_always_equal::value);
    static_assert(!a2_t::propagate_on_container_copy_assignment::value);
    static_assert(!a2_t::propagate_on_container_move_assignment::value);
    static_assert(!a2_t::propagate_on_container_swap::value);
    static_assert(a2_t::is_always_equal::value);

    static_assert(alloc_always_equal<scoped_allocator_adaptor<a_t>>);
    static_assert(!alloc_always_equal<scoped_allocator_adaptor<aa_t>>);
    static_assert(alloc_always_equal<scoped_allocator_adaptor<a_t, a_t, a_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<a_t, a_t, aa_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<a_t, aa_t, a_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<aa_t, a_t, a_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<aa_t, aa_t, a_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<aa_t, a_t, aa_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<a_t, aa_t, aa_t>>);
    static_assert(!alloc_always_equal
                  <scoped_allocator_adaptor<aa_t, aa_t, aa_t>>);

    static_assert(same_as<a1_t::rebind<long>::other,
                          scoped_allocator_adaptor<test_allocator<long>>>);
    static_assert(same_as<a2_t::rebind<long>::other,
                          scoped_allocator_adaptor<test_allocator<long>,
                                                   test_allocator<int *>,
                                                   test_allocator<int **>>>);
  }
  // special member functions and swap
  {
    using a1_t = scoped_allocator_adaptor<allocator<int>>;
    using a2_t = scoped_allocator_adaptor<allocator<int>,
                                          allocator<int *>,
                                          allocator<int **>>;

    static_assert(is_trivially_default_constructible<a1_t>);
    static_assert(is_trivially_destructible<a1_t>);
    static_assert(is_trivially_copy_constructible<a1_t>);
    static_assert(is_trivially_copy_assignable<a1_t>);
    static_assert(is_trivially_move_constructible<a1_t>);
    static_assert(is_trivially_move_assignable<a1_t>);
    static_assert(is_nothrow_swappable<a1_t>);

    static_assert(is_trivially_default_constructible<a2_t>);
    static_assert(is_trivially_destructible<a2_t>);
    static_assert(is_trivially_copy_constructible<a2_t>);
    static_assert(is_trivially_copy_assignable<a2_t>);
    static_assert(is_trivially_move_constructible<a2_t>);
    static_assert(is_trivially_move_assignable<a2_t>);
    static_assert(is_nothrow_swappable<a2_t>);
  }
  {
    using a1_t = scoped_allocator_adaptor<stateful_test_allocator<int>>;
    using a2_t = scoped_allocator_adaptor<stateful_test_allocator<int>,
                                          stateful_test_allocator<int *>,
                                          stateful_test_allocator<int **>>;
    {
      stateful_test_allocator<int> a, aa;
      a1_t x(a);
      a1_t y(aa);
      adl_swap(x, y);
      assert(x == aa && y == a);
      adl_swap(x, x);
      assert(x == aa && y == a);
      adl_swap(y, y);
      assert(x == aa && y == a);
    }
    {
      stateful_test_allocator<int> a, aa, aaa;
      stateful_test_allocator<int> a2, aa2, aaa2;
      a2_t x(a, aa, aaa);
      a2_t y(a2, aa2, aaa2);
      adl_swap(x, y);
      assert(x == a2
             && x.inner_allocator() == aa2
             && x.inner_allocator().inner_allocator() == aaa2);
      assert(y == a
             && y.inner_allocator() == aa
             && y.inner_allocator().inner_allocator() == aaa);
      adl_swap(x, x);
      adl_swap(y, y);
      assert(x == a2
             && x.inner_allocator() == aa2
             && x.inner_allocator().inner_allocator() == aaa2);
      assert(y == a
             && y.inner_allocator() == aa
             && y.inner_allocator().inner_allocator() == aaa);
    }
  }
  // other constructors
  {
    using a0_t = stateful_test_allocator<int>;
    using a00_t = stateful_test_allocator<int *>;
    using a000_t = stateful_test_allocator<int **>;
    {
      a0_t a, aa;
      scoped_allocator_adaptor<a0_t> x(a);
      assert(x == a);
      scoped_allocator_adaptor<a0_t, a00_t, a000_t> y(a, aa, a);
      assert(y == a && y.inner_allocator() == aa
             && y.inner_allocator().inner_allocator() == a);
    }
    {
      a0_t a, aa, aaa;
      scoped_allocator_adaptor<a0_t> b = scoped_allocator_adaptor<a00_t>(a);
      assert(b == a);

      scoped_allocator_adaptor<a0_t, a00_t, a000_t> x(a, aa, aaa);
      {
        scoped_allocator_adaptor<a000_t, a00_t, a000_t> y = x;
        assert(y == a && y.inner_allocator() == aa
               && y.inner_allocator().inner_allocator() == aaa);
      }
      {
        scoped_allocator_adaptor<a000_t, a00_t, a000_t> y = move(x);
        assert(y == a && y.inner_allocator() == aa
               && y.inner_allocator().inner_allocator() == aaa);
      }
    }
  }
  // outer_allocator
  // inner_allocator
  {
    using a0_t = stateful_test_allocator<int>;
    using a00_t = stateful_test_allocator<int>;
    {
      a0_t a;
      using a_t = scoped_allocator_adaptor<a0_t>;
      a_t x(a);
      assert(x.outer_allocator() == a);
      assert(as_const(x).outer_allocator() == a);
      static_assert(same_as<decltype(x.outer_allocator()), a0_t &>);
      static_assert(same_as<decltype(as_const(x).outer_allocator()),
                            const a0_t &>);
      assert(x.inner_allocator() == a);
      assert(as_const(x).inner_allocator() == a);
      static_assert(same_as<decltype(x.inner_allocator()), a_t &>);
      static_assert(same_as<decltype(as_const(x).inner_allocator()),
                            const a_t &>);
    }
    {
      a0_t a, aa;
      using a_t = scoped_allocator_adaptor<a0_t, a00_t, a00_t>;
      using aa_t = scoped_allocator_adaptor<a00_t, a00_t>;
      a_t x(a, aa, aa);
      assert(x.outer_allocator() == a);
      assert(as_const(x).outer_allocator() == a);
      static_assert(same_as<decltype(x.outer_allocator()), a0_t &>);
      static_assert(same_as<decltype(as_const(x).outer_allocator()),
                            const a0_t &>);
      assert(x.inner_allocator() == aa);
      assert(as_const(x).inner_allocator() == aa);
      static_assert(same_as<decltype(x.inner_allocator()), aa_t &>);
      static_assert(same_as<decltype(as_const(x).inner_allocator()),
                            const aa_t &>);
    }
  }
  // allocate
  // deallocate
  {
    stateful_test_allocator<int> a;
    scoped_allocator_adaptor<stateful_test_allocator<int>,
                             test_allocator<int>> x(a, {});
    {
      int *p = x.allocate(1);
      x.deallocate(p, 1);
    }
    {
      int *p = a.allocate(1);
      x.deallocate(p, 1);
    }
    {
      int *p = x.allocate(1);
      a.deallocate(p, 1);
    }
  }
  // construct
  // destroy
  {
    using a0_t = stateful_test_allocator<int>;
    using a00_t = stateful_test_allocator<long>;
    {
      using a_t = scoped_allocator_adaptor<a0_t>;
      using alw_t = allocator_wrapper<a_t>;
      alw_t a;
      a.delete_1(a.new_1());
      a.delete_array(a.new_array(3));
    }
    {
      using a_t = scoped_allocator_adaptor<a0_t, a0_t>;
      using alw_t = allocator_wrapper<a_t>;
      alw_t a;
      a.delete_1(a.new_1());
      a.delete_array(a.new_array(3));
    }
    {
      using a_t = scoped_allocator_adaptor<a0_t, a00_t, a0_t, a00_t>;
      using alw_t = allocator_wrapper<a_t>;
      alw_t a;
      a.delete_1(a.new_1());
      a.delete_array(a.new_array(3));
    }
  }
  // select_on_container_copy_construction
  {
    using a0_t = stateful_test_allocator<int>;
    {
      using a_t = scoped_allocator_adaptor<a0_t>;
      a0_t a;
      a_t x(a);
      a_t y = x.select_on_container_copy_construction();
      assert(y == a);
    }
    {
      using a_t = scoped_allocator_adaptor<a0_t, a0_t, a0_t>;
      a0_t a, aa, aaa;
      a_t x(a, aa, aaa);
      a_t y = x.select_on_container_copy_construction();
      assert(y == a && y.inner_allocator() == aa
             && y.inner_allocator().inner_allocator() == aaa);
    }
  }
  // comparation
  {
    using a0_t = stateful_test_allocator<int>;
    using a_t = scoped_allocator_adaptor<a0_t>;
    using aa_t = scoped_allocator_adaptor<a0_t, a0_t, a0_t>;
    a0_t a, aa, aaa;
    {
      a_t x(a);
      a_t y(aa);
      test_inequality(x, y);
    }
    {
      a_t x(a);
      a_t y(a);
      test_equality(x, y);
    }
    {
      aa_t x(a, aa, aaa);
      aa_t y(a, aaa, aa);
      test_inequality(x, y);
    }
    {
      aa_t x(a, aa, aaa);
      aa_t y(x);
      test_equality(x, y);
    }
  }
}
void test_object_pool() {
  {
    object_pool<int, test_allocator<byte>> pl;
    assert(pl.capacity() == 0u);
    assert(pl.used_size() == 0u);
    assert(pl.idle_size() == 0u);
    ez_vector<int *> v;
    v.insert(v.end(), pl.fetch_pointer(1));
    pl.free_pointer(back(v));
    assert(pl.capacity() != 0u);
    assert(pl.used_size() == 0u);
    assert(pl.idle_size() == pl.capacity());
  }
  {
    object_pool<int, test_allocator<byte>> pl;
    ez_vector<int *> v;
    for (int repeat_c : irng(0, 5)) {
      const auto r = irng(0, 10000);
      for (int i : r)
        v.insert(v.end(), pl.fetch_pointer(i));
      test_rng(deref_rng(v), r);
      for (int *p : v)
        pl.free_pointer(p);
      v.erase(v.begin(), v.end());
      assert(pl.capacity() != 0u);
      assert(pl.used_size() == 0u);
      assert(pl.idle_size() == pl.capacity());
    }
  }
  {
    object_pool<int, test_allocator<byte>> pl;
    ez_vector<int *> v;
    for (int i : irng(0, 5000))
      v.insert(v.end(), pl.fetch_pointer(i));
    test_rng(deref_rng(v), irng(0, 5000));
    for (int repeat_c : irng(0, 5)) {
      const auto r = irng(5000, 10000);
      for (int i : r)
        v.insert(v.end(), pl.fetch_pointer(i));
      test_rng(deref_rng(v), irng(0, 10000));
      for (int repeat_cc : irng(0, 5000)) {
        pl.free_pointer(back(v));
        v.erase(prev(v.end()));
      }
      test_rng(deref_rng(v), irng(0, 5000));
      assert(pl.capacity() >= 5000);
      assert(pl.used_size() == 5000);
      assert(pl.used_size() + pl.idle_size() == pl.capacity());
    }
  }

  {
    object_pool<int, test_allocator<byte>> pl;
    buffer<pool_object<int, test_allocator<byte>>> v(1000);
    for (int i : irng(0, 1000))
      v.push_back(pl.fetch(i));
    test_rng(deref_rng(v), irng(0, 1000));
  }
}
namespace help_memory_pool {

constexpr auto to_vec = [](int i) {
  ez_vector<int> v;
  while (i != 0) {
    const int r = i % 10;        
    const int d = i / 10;
    v.insert(v.end(), r);
    i = d;
  }
  for (auto it = v.begin(), it2 = v.end(); it < it2;) {
    adl_swap(*it, *--it2);
    ++it;
  }
  return v;
};

}
void test_raw_object_pool() {
  using help_memory_pool::to_vec;
  {
    raw_object_pool<test_allocator<byte>> pl(sizeof(ez_vector<int>), 128u);
    assert(pl.object_size() == 128u);
    assert(pl.object_align() == 128u);
    ez_vector<ez_vector<int> *> v;
    for (int i : irng(0, 100000)) {
      ez_vector<int> *p = reinterpret_cast<ez_vector<int> *>(pl.allocate());
      assert(p != nullptr);
      ::new(p) ez_vector<int>(to_vec(i));
      v.insert(v.end(), p);
    }
    test_rng(deref_rng(v), bind_rng(irng(0, ssize(v)), to_vec));
    assert(pl.used_size() == 100000u);
    assert(pl.capacity() >= pl.used_size());
    assert(pl.capacity() == pl.idle_size() + pl.used_size());
    for (auto p : v) {
      p->~ez_vector<int>();
      pl.deallocate(p);
    }
  }
  {
    raw_object_pool<test_allocator<byte>> pl(sizeof(ez_vector<int>), 128u);
    pl.reserve_more(10000);
    ez_vector<ez_vector<int> *> v;
    for (int i : irng(0, 100000)) {
      ez_vector<int> *p = reinterpret_cast<ez_vector<int> *>(pl.allocate());
      assert(p != nullptr);
      ::new(p) ez_vector<int>(to_vec(i));
      v.insert(v.end(), p);
    }
    test_rng(deref_rng(v), bind_rng(irng(0, ssize(v)), to_vec));
    assert(pl.used_size() == 100000u);
    assert(pl.capacity() >= pl.used_size());
    assert(pl.capacity() == pl.idle_size() + pl.used_size());

    const auto tmp_z = pl.idle_size() + 100u;
    pl.reserve_more(tmp_z);
    assert(pl.idle_size() >= tmp_z);
    const auto idle_sz = pl.idle_size();
    for (int i : irng(100000, 100000 + idle_sz)) {
      ez_vector<int> *p = reinterpret_cast<ez_vector<int> *>(pl.allocate());
      assert(p != nullptr);
      ::new(p) ez_vector<int>(to_vec(i));
      v.insert(v.end(), p);
    }
    test_rng(deref_rng(v), bind_rng(irng(0, ssize(v)), to_vec));
    assert(pl.used_size() == 100000u + idle_sz);
    assert(pl.capacity() == pl.used_size());
    assert(pl.idle_size() == 0u);

    for (auto p : v) {
      p->~ez_vector<int>();
      pl.deallocate(p);
    }
  }
}
void test_memory_pool() {
  using help_memory_pool::to_vec;
  {
    using pool_t = memory_pool<test_allocator<byte>>;
    using pool_alloc_t = pool_allocator<byte, pool_t>;
    pool_t pl;
    allocator_wrapper<pool_alloc_t> alw0(pool_alloc_t{pl});

    {
      auto alw = alw0.rebind<int>();
      int *const p = alw.new_1(1);
      assert(*p == 1);
      alw.delete_1(p);
    }

    {
      auto alw = alw0.rebind<int>();
      ez_vector<int *> v;
      for (int i : irng(0, 10000))
        v.insert(v.end(), alw.new_1(i));
      test_rng(deref_rng(v), irng(0, 10000));
      for (auto p : v)
        alw.delete_1(p);
    }
    {
      auto alw = alw0.rebind<test_object<int>>();
      ez_vector<test_object<int> *> v;
      for (int i : irng(0, 10000))
        v.insert(v.end(), alw.new_1(i));
      test_rng(deref_rng(deref_rng(v)), irng(0, 10000));
      for (auto p : v)
        alw.delete_1(p);
    }
    {
      auto alw = alw0.rebind<ez_vector<int>>();
      ez_vector<ez_vector<int> *> v;
      for (int i : irng(0, 10000))
        v.insert(v.end(), alw.new_1(to_vec(i)));
      test_rng(deref_rng(v), bind_rng(irng(0, 10000), to_vec));
      for (auto p : v)
        alw.delete_1(p);
    }
    {
      struct t {
        int a[1000000] = {};
        ez_vector<int> v;
      };
      auto alw = alw0.rebind<t>();
      auto p1 = alw.new_1();
      auto p2 = alw.new_1();
      p1->v = to_vec(123);
      assert(p1->v == to_vec(123));
      p2->v = to_vec(123);
      assert(p2->v == to_vec(123));
      alw.delete_1(p1);
      alw.delete_1(p2);
    }
  }
}
void test_allocator() {
  printf("allocator: ");

  inner::test::test_allocator_traits();
  inner::test::test_uninitialized_sequence();
  inner::test::test_default_allocator();
  inner::test::test_allocator_wrapper();
  inner::test::test_unique_ptr();
  inner::test::test_unique_array();
  inner::test::test_copyable_ptr();
  inner::test::test_copyable_array();
  inner::test::test_buffer();
  inner::test::test_scoped_allocator_adaptor();
  inner::test::test_object_pool();
  inner::test::test_raw_object_pool();
  inner::test::test_memory_pool();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_allocator();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
