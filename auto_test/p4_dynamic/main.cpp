//#define RE_NOEXCEPT
#define RE_DEFAULT_ALLOCATOR re::test_allocator
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>

#include <cassert>

namespace re::inner::fns {

namespace dynamic_help {

struct t0 : dynamic_object {
  virtual size_t f() const = 0;
};
template <size_t I>
struct t1 : t0 {
  instance_counter<size_t> a[I];

  t1() {
    int i = 1;
    for (auto &x : a) {
      *x = i;
      ++i;
    }
  }
  t1(const t1 &) = default;
  t1 &operator =(const t1 &) = delete;
  t1(t1 &&) = default;
  t1 &operator =(t1 &&) = delete;

  virtual size_t f() const override {
    size_t n = 0;
    for (const auto &x : a)
      n += *x;
    return n;
  }
};

using dyn_t0_t = dynamic<t0, sizeof(size_t) * 3>;

template <size_t I>
void init(dyn_t0_t *p) {
  construct_at(p, t1<I>{});
}
template <size_t I>
bool good(dyn_t0_t *p) {
  size_t n = 0;
  for (auto x : irng(static_cast<size_t>(1), I + 1))
    n += x;
  return (*p)->f() == n;
}

template <>
void init<0>(dyn_t0_t *p) {
  construct_at(p);
}
template <>
bool good<0>(dyn_t0_t *p) {
  return (*p).empty();
}

template <size_t I, size_t J>
void test_smf() {
  test_ownership<dyn_t0_t>
    (init<I>, good<I>, init<J>, good<J>, bind(empty, bind(deref, _1)));
}

}
void test_dynamic_construct_from_nullptr_or_derived() {
  {
    using dynamic_help::dyn_t0_t;
    using dynamic_help::t1;
    dyn_t0_t d0 = nullptr;
    assert(d0.local());
    test_equality(d0, nullptr);
#ifndef RE_NOEXCEPT
    assert(test_throwing<bad_dynamic_access>([&]() {d0.value();}));
    assert(test_throwing<bad_dynamic_access>([&]() {as_const(d0).value();}));
#endif

    dyn_t0_t d = t1<3>{};
    assert(!d.local() && d.value().f() == 6);
    assert(addressof(d) == addressof(d = t1<2>{}));
    assert(d.local() && d.value().f() == 3);

    dyn_t0_t dd = t1<2>{};
    assert(dd.local() && dd.value().f() == 3);
    assert(addressof(dd) == addressof(dd = t1<3>{}));
    assert(!dd.local() && dd.value().f() == 6);
    assert(addressof(dd) == addressof(dd = nullptr));
    assert(dd.local() && dd == nullptr);

    dyn_t0_t ddd = t1<1>{};
    assert(ddd.local() && ddd.value().f() == 1);
    assert(addressof(ddd) == addressof(ddd = t1<2>{}));
    assert(ddd.local() && ddd.value().f() == 3);
    assert(addressof(ddd) == addressof(ddd = nullptr));
    assert(ddd.local() && ddd == nullptr);
  }
  assert(instance_counter<size_t>::count() == 0);
}
void test_dynamic_smf() {
  using P = make_index_sequence<5>;
  type_pack_for_each<type_pack_mul<P, P>>([](auto x, auto y) {
      static constexpr size_t i = decltype(x)::type::value;
      static constexpr size_t j = decltype(y)::type::value;
      dynamic_help::test_smf<i, j>();
    });
}

void test_dynamic_t() {
  test_dynamic_construct_from_nullptr_or_derived();
  test_dynamic_smf();
}

template <class T1, class T2>
void test_dynamic_1_impl() {
  struct A : T1 {
    int i = 1;

    virtual int f() {
      return i;
    }
  };
  struct B : T2 {
    int i = 2;

    virtual int f() {
      return i;
    }
  };
  struct C : B, A {
    int i = 3;

    C() = default;
    ~C() = default;
    C(const C &) = default;
    C &operator =(const C &) = delete;
    C(C &&) = default;
    C &operator =(C &&) = delete;

    virtual int f() {
      return i;
    }
  };
  struct D : A {
    int i = 4;
    int j[100];

    D() = default;
    ~D() = default;
    D(const D &) = default;
    D &operator =(const D &) = delete;
    D(D &&) = default;
    D &operator =(D &&) = delete;

    virtual int f() {
      return i;
    }
  };

  {
    dynamic<A> a(in_place_type<C>);
    assert(a->f() == 3);
    {
      dynamic<A> b;
      assert(b.empty());
    }
    {
      dynamic<A> b = a;
      assert(b->f() == 3);
    }
    {
      dynamic<A> b = move(a);
      assert(b->f() == 3 && a.empty());
      a = move(b);
      assert(a->f() == 3 && b.empty());
    }
    {
      dynamic<A> b;
      b = a;
      assert(b->f() == 3);
      b.template emplace<D>();
      assert(b->f() == 4);
      b.template emplace<C>();
      assert(b->f() == 3);
      b = dynamic<A>(in_place_type<D>);
      assert(b->f() == 4);
    }
  }

  {
    dynamic<A> a(in_place_type<C>);
    assert(a->f() == 3);

    dynamic<A> b;
    assert(b.empty());
    assert(nullptr == b);
    assert(b.empty());
    assert(!b.has_value());

    b = move(a);
    assert(a.empty());
    assert(b->f() == 3);

    auto c = move(b);
    assert(b.empty());
    assert(c->f() == 3);

    auto d = c;
    assert(d->f() == 3);
    assert(d.template is<C>());
    assert(d.type() == typeid(C));
    d.template as<C>().i = 33;
    assert(d->f() == 33);
    d.clear();
    assert(d.empty());

    d = move(c);
    assert(c.empty());
    assert(d->f() == 3);

    swap(c, d);
    assert(d.empty());
    assert(c->f() == 3);
    swap(d, d);
    assert(d.empty());
    d = C();
    assert(d.template is<C>());
    d.template as<C>().i = 33;
    assert(d->f() == 33);
    swap(c, d);
    assert(c->f() == 33);
    assert(d->f() == 3);
    c.reset();
    swap(d, c);
    assert(c->f() == 3);
    assert(d.empty());
  }

  {
    dynamic<B> b0;
    assert(b0.type() == typeid(void));
    assert(b0.template is<void>());

    dynamic<B, sizeof(inner::dynamic_data<B>)> b1 = B();
    assert(b1.local());
    assert(b1->f() == 2);

    dynamic<B, 0> b2 = B();
    assert(!b2.local());
    assert(b2->f() == 2);

    dynamic<B> b3 = b2;
    assert(b2->f() == 2);
    assert(b3->f() == 2);

    b3.template as<B>().i = 22;
    assert(b3->f() == 22);
    b3 = move(b2);
    assert(b3->f() == 2);
    assert(b2.empty());

    dynamic<B> b4 = b1;
    assert(b4->f() == 2);
    b1.template as<B>().i = 22;
    assert(b1->f() == 22);
    b4 = move(b1);
    assert(b4->f() == 22);
    assert(b1.empty());
  }
}
void test_dynamic_1() {
  test_dynamic_1_impl<dynamic_object, inner::empty_type>();
  test_dynamic_1_impl<inner::empty_type, dynamic_object>();
}

void test_dynamic_2() {
  struct A : dynamic_object {
    virtual int f() {
      return 1;
    }
  };
  struct E : A {
    void *a[100];

    E() = default;
    ~E() = default;
    E(const E &) = default;
    E &operator =(const E &) = delete;
    E(E &&) noexcept = default;
    E &operator =(E &&) = delete;

    int f() override {
      return 5;
    }
  };
  dynamic<A> a(in_place_type<E>);
  assert(a->f() == 5);
  auto b = move(a);
  assert(b->f() == 5 && a.empty());
  a = dynamic<A>(E());

  struct F : A {
    F() = default;
    ~F() = default;
    F(const F &) = default;
    F &operator =(const F &) = delete;
    F(F &&) noexcept = default;
    F &operator =(F &&) = delete;

    int i;
    F(int i) : i(i) {}

    int f() override {
      return i;
    }
  };
  dynamic<A> c = F(9);
  dynamic<A> d = F(10);
  adl_swap(c, d);
  assert(c->f() == 10 && d->f() == 9);
  adl_swap(c, d);
  c = E();
  adl_swap(c, d);
  adl_swap(c, d);
  adl_swap(c, d);
  assert(c->f() == 10 && d->f() == 5);

  dynamic<A> e = F(4);
  dynamic<A, sizeof(void *) * 200> f = E();
  assert(e->f() == 4 && f->f() == 5);
  e = f;
  assert(e->f() == 5);
  e.reset();
  e = move(f);
  assert(e->f() == 5 && f.empty() && e.type() == typeid(E));
}
void test_dynamic_3() {
  class A : dynamic_object {
  public:
    virtual int f() {
      return -1;
    }
  };
  class B : public A {
    int i;
    int j;

  public:  
    B() : i(), j() {}
    ~B() = default;
    B(const B &) = default;
    B &operator =(const B &) = delete;
    B(B &&) noexcept = default;
    B &operator =(B &&) = delete;

    B(int i, int j) : i(i), j(j) {}

    int f() override {
      return i + j;
    }
  };
  class C : public A {
    int holder[30];
    int k;

  public:
    C() = default;
    ~C() = default;
    C(const C &) = default;
    C &operator =(const C &) = delete;
    C(C &&) noexcept = default;
    C &operator =(C &&) = delete;

    C(int k) : k(k) {}

    int f() override {
      return k;
    }
  };
  class D0 {
    int a[20];
  };
  class D1 {
    int i;
  };
  class D : public D0, public A, public D1 {
    int k;
    ez_vector<test_object<int>> v;

  public:
    D() : D0{}, A{}, D1{}, k{}, v({1, 2, 3, 4}) {}
    ~D() = default;
    D(const D &) = default;
    D &operator =(const D &) = delete;
    D(D &&) noexcept = default;
    D &operator =(D &&) = delete;

    D(int kk) : D() {
      k = kk;
    }

    int f() override {
      return k;
    }
  };
  static auto empty_check = [](dynamic<A> *p) {
    return (*p).empty();
  };
  struct test_0 {
    static void init(dynamic<A> *p) {
      ::new(p) dynamic<A>();
    }
    static bool good(dynamic<A> *p) {
      auto &x = *p;
      return x.local() && x.empty() && x.is<void>();
    }
  };
  struct test_1 {
    static void init(dynamic<A> *p) {
      ::new(p) dynamic<A>(B(0, 1));
    }
    static bool good(dynamic<A> *p) {
      auto &x = *p;
      return x.local() && x->f() == 1 && x.is<B>();
    }
  };
  struct test_2 {
    static void init(dynamic<A> *p) {
      ::new(p) dynamic<A>(C(2));
    }
    static bool good(dynamic<A> *p) {
      auto &x = *p;
      return !x.local() && x->f() == 2 && x.is<C>();
    }
  };
  struct test_3 {
    static void init(dynamic<A> *p) {
      ::new(p) dynamic<A>(D(3));
    }
    static bool good(dynamic<A> *p) {
      auto &x = *p;
      return !x.empty() && !x.local() && x->f() == 3 && x.is<D>();
    }
  };

  const auto testf = [](auto tag1, auto tag2) {
    using T1 = typename decltype(tag1)::type;
    using T2 = typename decltype(tag2)::type;
    test_ownership<dynamic<A>>(T1::init, T1::good,
                               T2::init, T2::good,
                               empty_check);
  };
  using L1 = type_pack<test_0, test_1, test_2, test_3>;
  using L2 = L1;
  type_pack_mul<L1, L2>{}(testf);

  {
    dynamic<A, 100> a(in_place_type<D>, 3);
    assert(a->f() == 3);
    dynamic<A> b = a;
    assert(b->f() == 3 && a->f() == 3);
    dynamic<A, 100> a2(in_place_type<D>, 4);
    b = a2;
    assert(b->f() == 4 && a2->f() == 4);
    dynamic<A, 100> a3(in_place_type<A>);
    assert(a3->f() == -1 && a3.local());
    b = a3;
    assert(b->f() == -1 && a3->f() == -1);
    b = move(a3);
    assert(b->f() == -1 && a3.empty());
    b = move(a2);
    assert(b->f() == 4 && a2.empty());
  }
}
template <class T>
void test_dynamic_4_impl() {
  using base_t = T;
  struct A : base_t {
    test_object<int> i = 'A';

    virtual int f() const {
      return i;
    }
  };
  struct B : A {
    test_object<int> i = 'B';

    int f() const override {
      return i;
    }
  };
  struct C : type_tag<void>, B {
    test_object<int> i = 'C';

    int f() const override {
      return i;
    }
  };

  auto testf1 = [](auto dynamic_tag) {
    using dynamic_t = typename decltype(dynamic_tag)::type;

    using pack1 = type_pack<A, B, C>;
    type_pack_mul<pack1, pack1>{}([](auto tag1, auto tag2) {
        static auto get_test_fns_tag
          = [](auto dynamic_tag, auto wrapped_tag) {
          using dynamic_t = typename decltype(dynamic_tag)::type;
          using wrapped_t = typename decltype(wrapped_tag)::type;

          struct fns {
            static void init(dynamic_t *p) {
              ::new(p) dynamic_t(wrapped_t());
            }
            static bool good(dynamic_t *p) {
              return (*p)->f() == wrapped_t().i;
            }
          };
          return type_tag<fns>();
        };
        using T1 = typename decltype
          (get_test_fns_tag(type_tag<dynamic_t>(), tag1))::type;
        using T2 = typename decltype
          (get_test_fns_tag(type_tag<dynamic_t>(), tag2))::type;
        test_ownership<dynamic_t>
          (T1::init, T1::good, T2::init, T2::good,
           [](auto p) {return (*p).empty();});
      });
  };

  testf1(type_tag<dynamic<A>>{});
  testf1(type_tag<dynamic<A, 0>>{});
  testf1(type_tag<dynamic<A, sizeof(A)>>{});
  testf1(type_tag<dynamic<A, sizeof(A) * 4>>{});
}
void test_dynamic_4() {
  test_dynamic_4_impl<dynamic_object>();
  test_dynamic_4_impl<inner::empty_type>();
}

void test_dynamic_void() {
  auto testf = [](auto tag) {
    using any_t = typename decltype(tag)::type;

    any_t x(in_place_type<ez_vector<test_object<int>>>,
            {test_object<int>(1), test_object<int>(2),
             test_object<int>(3)});
    assert((x.template is
            <ez_vector<test_object<int>>>()));
    assert((x.template as<ez_vector<test_object<int>>>()
            == ez_vector<test_object<int>>{1, 2, 3}));
    x.clear();
    assert(x.empty());
    assert(x.type() == typeid(void));

    x = test_object<int>(3);
    assert(x.template is<test_object<int>>());
    assert(x.template as<test_object<int>>() == 3);
    any xx = move(x);
    assert(x.empty());
    assert(xx.template is<test_object<int>>());
    assert(xx.template as<test_object<int>>() == 3);

    x.template emplace<long long>(100);
    assert(is<long long>(x));
    assert(as<long long>(x) == 100);
  };
  testf(type_tag<dynamic<void, 0>>{});
  testf(type_tag<dynamic<void, 4>>{});
  testf(type_tag<dynamic<void, 8>>{});
  testf(type_tag<dynamic<void, 16>>{});
  testf(type_tag<dynamic<void, 32>>{});

  {
#ifndef RE_NOEXCEPT
    any x;
    x.emplace<int>(3);

    auto f = [](auto ff) {
      bool y = false;
      try {
        ff();
      }
      catch (const bad_any_cast &) {
        y = true;
      }
      assert(y == true);
    };
    f([&]() {any_cast<short &>(x);});
    f([&]() {any_cast<short>(as_const(x));});
    f([&]() {any_cast<const short &>(move(x));});

    {
      auto p = any_cast<int>(addressof(x));
      assert(*p == 3);
      auto pp = any_cast<short>(addressof(x));
      assert(pp == nullptr);
    }
    {
      auto p = any_cast<int>(addressof(as_const(x)));
      assert(*p == 3);
      auto pp = any_cast<short>(addressof(as_const(x)));
      assert(pp == nullptr);
    }
#endif
  }

  assert(make_any<int>(3).is<int>()
         && make_any<int>(3).as<int>() == 3);
  assert((make_any<ez_vector<int>>().is<ez_vector<int>>()
          && make_any<ez_vector<int>>({1, 2, 3})
          .as<ez_vector<int>>() == ez_vector<int>{1, 2, 3}));
}

void test_function() {
  int (*fp)() = []() {return 1;};
  function<int ()> f = fp;
  assert(f() == 1);

  auto s = ez_vector<test_object<int>>({1, 2, 3});
  function<int ()> f2
    = [s]() {
      int x = 0;
      for (auto &i : iters(s))
        x += *i;
      return x;
    };
  assert(f2() == 6);
  adl_swap(f, f2);
  assert(f() == 6 && f2() == 1);
  adl_swap(f2, f);
  assert(f() == 1 && f2() == 6);

  const auto impl = [s = ez_vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}]() {
    return *(s.end() - 1);
  };
  f2 = nullptr;
  assert(f2 == nullptr);
  assert(nullptr == f2);
  assert(f2.empty());
  assert(f2.type() == typeid(void));

  function<int (), 100> f3 = impl;
  auto f4 = f3;
  assert(f4() == 10);
  f2 = move(f3);
  assert(f2() == 10 && f3 == nullptr);

  function<int (), 0> f5 = impl;
  assert(f5() == 10);
  f5 = nullptr;
  assert(f5.empty());
  f5 = move(impl);
  assert(f5() == 10);

  f5 = fp;
  assert(is<int (*)()>(f5));
  assert(as<int (*)()>(f5)() == 1);
  assert(f5 != nullptr && nullptr != f5);
  assert(!f5.empty());
  assert(f5.type() == typeid(int (*)()));
  f5.reset();
  assert(f5.empty());
}
void test_unique_function() {
  {
    int (*fp)() = []() {return 1;};
    using f_t = unique_function<int ()>;
    f_t f = fp;
    static_assert(movable<f_t>);
    static_assert(!is_copy_constructible_v<f_t>);
    static_assert(!is_copy_assignable_v<f_t>);
    static_assert(swappable<f_t>);
    assert(f() == 1);
    assert(f.local());

    struct tmp_t {
      void *a[6] = {};
    };
    f = [x = tmp_t{}]() {return 1;};
    assert(f.local());
    static_assert(decltype(f)::local<int (*)()>());

    assert(!f.empty() && f != nullptr && f.operator bool());
    f = f_t(nullptr);
    assert(f.empty() && nullptr == f && f.operator bool() == false);
    f.reset();
    assert(f.empty() && nullptr == f && f.operator bool() == false);
    f.clear();
    assert(f.empty() && nullptr == f && f.operator bool() == false);
    f = nullptr;
    assert(f.empty() && nullptr == f && f.operator bool() == false);
    assert(f.local());

    struct tmp_t2 {
      void *a[100] = {};
    };
    f = [x = tmp_t2{}]() {return 2;};
    assert(!f.local());
    assert(f() == 2);
  }
  {
    using f_t = void (int &);
    using fw_t = unique_function<f_t>;
    using fw2_t = unique_function<f_t, 0u>;

    static_assert(inner::is_class_unique_function_of<fw_t, void (int &)>
                  ::value);
    static_assert(!inner::is_class_unique_function_of<fw_t, void (int &&)>
                  ::value);

    int x = 1;
    fw2_t f0 = [](int &x) {++x;};
    fw_t f(move(f0));
    assert(f0.empty() && !f.empty());
    assert(f0.local());
    assert(f.local());
    f(x);
    assert(x == 2);
    f = move(f0);
    assert(f.empty());

    f = fw2_t([](int &x) {++x;});
    assert(!f.empty());
    f(x);
    assert(x == 3);
  }
}

namespace help_move_only_function {

template <class FROM, class TO>
inline constexpr bool can_convert_assign_to
  = is_convertible_v<FROM, TO> && is_assignable_v<TO &, FROM>;

}
void test_move_only_function() {
  using help_move_only_function::can_convert_assign_to;

  // smf // todo
  {
    using f_t = move_only_function<int ()>;
    static_assert(movable<f_t>);
    static_assert(!copyable<f_t>);

    f_t f;
    assert(!f);
    test_equality(f, nullptr);
    f = []() {return 1;};
    assert(f);
    test_inequality(f, nullptr);
    assert(f() == 1);

    f_t ff(move(f));
    assert(ff);
    test_inequality(ff, nullptr);
    assert(!f);
    test_equality(f, nullptr);
    assert(ff() == 1);
  }
  // construct/assign/compare with nullptr
  {
    using f_t = move_only_function<int ()>;
    f_t f = nullptr;
    assert(!f);
    test_equality(f, nullptr);

    assert(addressof(f = nullptr) == addressof(f));
    assert(!f);
    test_equality(f, nullptr);

    f = []() {return 1;};
    assert(f && f() == 1);
    assert(addressof(f = nullptr) == addressof(f));
    assert(!f);
    test_equality(f, nullptr);
  }
  // in-place construct
  {
    struct t {
      int x;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      explicit t(int y) : x(y) {}
      explicit t(initializer_list<int> l, int y) : x(y + size(l)) {}

      int operator ()() const {return x;}
    };
    using f_t = move_only_function<int ()>;
    f_t f(in_place_type<t>, {1, 2, 3}, 10);
    assert(f && f() == 13);
    f_t ff(in_place_type<t>, 1);
    assert(ff && ff() == 1);
  }
  // construct/assign with f
  {
    {
      using f_t = move_only_function<int ()>;
      f_t f = []() {return 1;};
      assert(f && f() == 1);
      f = []() {return 2;};
      assert(f && f() == 2);
      f = nullptr;
      f = []() {return 3;};
      assert(f && f() == 3);
    }
    {
      struct ft {
        void operator ()(int) {}
      };
      struct ft_ref {
        void operator ()(int) & {}
      };
      struct ft_rref {
        void operator ()(int) && {}
      };
      struct ft_c {
        void operator ()(int) const {}
      };
      struct ft_c_ref {
        void operator ()(int) const & {}
      };
      struct ft_c_rref {
        void operator ()(int) const && {}
      };
      struct ft_nothrow {
        void operator ()(int) noexcept {}
      };
      struct ft_ref_nothrow {
        void operator ()(int) & noexcept {}
      };
      struct ft_rref_nothrow {
        void operator ()(int) && noexcept {}
      };
      struct ft_c_nothrow {
        void operator ()(int) const noexcept {}
      };
      struct ft_c_ref_nothrow {
        void operator ()(int) const & noexcept {}
      };
      struct ft_c_rref_nothrow {
        void operator ()(int) const && noexcept {}
      };
      {
        using fw_t = move_only_function<void (int)>;
        static_assert(can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(!is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        f(1);
        move(f)(2);
        assert(i == 3);
      }
      {
        using fw_t = move_only_function<void (int) &>;
        static_assert(can_convert_assign_to<ft, fw_t>);
        static_assert(can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(!is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(!is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        f(1);
        assert(i == 1);
      }
      {
        using fw_t = move_only_function<void (int) &&>;
        static_assert(can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(!is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(!is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        move(f)(1);
        assert(i == 1);
      }
      {
        using fw_t = move_only_function<void (int) const>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        f(1);
        move(f)(1);
        as_const(f)(1);
        move(as_const(f))(1);
        assert(i == 4);
      }
      {
        using fw_t = move_only_function<void (int) const &>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        f(1);
        move(f)(1);
        as_const(f)(1);
        move(as_const(f))(1);
        assert(i == 4);
      }
      {
        using fw_t = move_only_function<void (int) const &&>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_c, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(!is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) {i += x;};
        move(f)(1);
        move(as_const(f))(1);
        assert(i == 2);
      }
      {
        using fw_t = move_only_function<void (int) noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        f(1);
        move(f)(2);
        assert(i == 3);
      }
      {
        using fw_t = move_only_function<void (int) & noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(!is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(!is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        f(1);
        assert(i == 1);
      }
      {
        using fw_t = move_only_function<void (int) && noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(!is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(!is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        move(f)(1);
        assert(i == 1);
      }
      {
        using fw_t = move_only_function<void (int) const noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        f(1);
        move(f)(1);
        as_const(f)(1);
        move(as_const(f))(1);
        assert(i == 4);
      }
      {
        using fw_t = move_only_function<void (int) const & noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        f(1);
        move(f)(1);
        as_const(f)(1);
        move(as_const(f))(1);
        assert(i == 4);
      }
      {
        using fw_t = move_only_function<void (int) const && noexcept>;
        static_assert(!can_convert_assign_to<ft, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_ref, fw_t>);
        static_assert(!can_convert_assign_to<ft_c_rref, fw_t>);
        static_assert(!can_convert_assign_to<ft_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_ref_nothrow, fw_t>);
        static_assert(!can_convert_assign_to<ft_rref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_ref_nothrow, fw_t>);
        static_assert(can_convert_assign_to<ft_c_rref_nothrow, fw_t>);
        static_assert(!is_invocable_v<fw_t &, int>);
        static_assert(is_invocable_v<fw_t &&, int>);
        static_assert(!is_invocable_v<const fw_t &, int>);
        static_assert(is_invocable_v<const fw_t &&, int>);
        int i = 0;
        fw_t f = [&i](int x) noexcept {i += x;};
        move(f)(1);
        move(as_const(f))(1);
        assert(i == 2);
      }
    }
  }
}

namespace type_erased_invocation_test0 {

struct base_type {
  virtual int f() const = 0;
};
struct X : base_type {
  int i = 1;
  virtual int f() const override {
    return i;
  }
  int non_virtual_f() const {
    return i;
  }
};
struct Y : base_type {
  int x = 999;
  int i = 2;
  virtual int f() const override {
    return i;
  }
  int non_virtual_f() const {
    return i;
  }
};
struct Z : base_type {
  int x = 9999;
  int xx = 99999;
  int i = 3;
  virtual int f() const override {
    return i;
  }
  int non_virtual_f() const {
    return i;
  }
};

int fp_for_base_type(const base_type &a,
                     const base_type &b,
                     const base_type &c) {
  return a.f() * 100 + b.f() * 10 + c.f();
}

struct static_type_erased_visitor {
  template <class X, class Y, class Z>
  static int apply(X &&x, Y &&y, Z &&z) {
    return x.non_virtual_f() * 100 + y.non_virtual_f() * 10 + z.non_virtual_f();
  }
};
struct type_erased_visitor {
  int i = 1000;
  template <class X, class Y, class Z>
  int operator ()(X &&x, Y &&y, Z &&z) const {
    return 1000
      + x.non_virtual_f() * 100 + y.non_virtual_f() * 10 + z.non_virtual_f();
  }
};

template <class BASE_TYPE = base_type>
void test() {
  const X x;
  const Y y;
  const Z z;
  assert(fp_for_base_type(x, y, z) == 123);

  {
    using P = type_pack<X, Y, Z>;
    type_erased_visitor visitor;
    auto a = type_erased_invocation_array
      <const BASE_TYPE *, int, const type_erased_visitor &, P, P, P>();

    const int r1 = do_type_erased_invocation<const BASE_TYPE *>
      (a[type_erased_invocation_index<P, P, P>(0, 1, 2)], visitor, x, y, z);
    const int r2 = do_type_erased_invocation<const BASE_TYPE *>
      (a[type_erased_invocation_index<P, P, P>(2, 1, 0)], visitor, z, y, x);
    assert(r1 == 1123);
    assert(r2 == 1321);
  }
}

}
void test_type_erased_invocation() {
  {
    assert(accumulate_args(plus<size_t>(), 1) == 1);
    assert(accumulate_args(plus<size_t>(), 1, 2) == 3);
    assert(accumulate_args(plus<size_t>(), 1, 2, 3) == 6);
  }

  {
    using fp = void (*)(int);
    static_assert(is_invocable_for_all_combinations_v
                  <fp, type_pack<short, int, long, long long>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <fp, type_pack<void *>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <fp, type_pack<short, void *>>);
    static_assert(is_invocable_for_all_combinations_v
                  <fp, type_pack<int>>);
  }
  {
    using fp = void (*)(int, int);
    static_assert(is_invocable_v<fp, int, int>);
    static_assert(is_invocable_for_all_combinations_v
                  <fp, type_pack<int>, type_pack<int>>);
    static_assert(is_invocable_for_all_combinations_v
                  <fp, type_pack<int, long>, type_pack<short, int>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <fp, type_pack<int, long>,
                   type_pack<short, int, void *>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <fp, type_pack<int, long>,
                   type_pack<int, long>, type_pack<int, long>>);

    static_assert(!has_member_type_type_v
                  <invoke_result_for_all_combinations
                   <fp, type_pack<int>>>);
    static_assert(has_member_type_type_v
                  <invoke_result_for_all_combinations
                   <fp, type_pack<int>, type_pack<int>>>);
    static_assert(is_same_v
                  <invoke_result_for_all_combinations_t
                   <fp, type_pack<int>, type_pack<int>>, void>);
  }
  {
    auto f = [](auto x, auto y) {return x + y;};
    static_assert(is_invocable_for_all_combinations_v
                  <decltype(f) &,
                   type_pack<bool, short, int>, type_pack<long, long long>>);
    using r = invoke_result_for_all_combinations_t
      <decltype(f) &, type_pack<bool, short, int>, type_pack<long, long long>>;
    static_assert(is_same_v<r, long long>);

    auto a = type_erased_invocation_array
      <const void *, r, decltype(f) &,
       type_pack<short &, int &>, type_pack<long &, long long &>>();
    short i = 3;
    long long j = 4;
    for (int k = 0; k != 4; ++k)
      assert(a[k] != nullptr);
    long long k
      = do_type_erased_invocation<const void *>(a[0 * 2 + 1], f, i, j);
    assert(k == 7);
  }

  {
    static_assert(base_of_type_packs_v
                  <0, type_pack<int, int>, type_pack<int, int, int>,
                   type_pack<int, int, int, int>> == 12);
    static_assert(base_of_type_packs_v
                  <1, type_pack<int, int>, type_pack<int, int, int>,
                   type_pack<int, int, int, int>> == 4);
    static_assert(base_of_type_packs_v
                  <2, type_pack<int, int>, type_pack<int, int, int>,
                   type_pack<int, int, int, int>> == 1);

    assert((type_erased_invocation_index
            <type_pack<int, int>>(0) == 0));
    assert((type_erased_invocation_index
            <type_pack<int, int>>(1) == 1));
    assert((type_erased_invocation_index
            <type_pack<int, int, int, int>,
             type_pack<int, int, int, int, int>,
             type_pack<int, int, int>>(2, 3, 1) == 2 * 5 * 3 + 3 * 3 + 1));
    assert((type_erased_invocation_index
            <type_pack<int, int, int, int>,
             type_pack<int, int, int>>(2, 2) == 2 * 3 + 2));
  }

  {
    type_erased_invocation_test0::test();
    // warning: the following line violates the standard but may be right
    // by a very great possibility
    type_erased_invocation_test0::test<void>();
  }
}

void test_variant() {
  {
    static_assert(is_trivially_destructible_v<variant<int>>);
    static_assert(is_trivially_destructible_v<variant<int, void *>>);
  }

  {
    variant<test_object<int>,
            test_object<float>,
            const test_object<long long>> x;
    assert(x.is<test_object<int>>());
    assert(x.index() == 0);
    assert(!x.valueless_by_exception());
    x.emplace<void>();
    assert(x.valueless_by_exception());
    assert(x.index() == x.npos());

    x.emplace<1>(1.5);
    assert(as<test_object<float>>(x) == 1.5);

    x.emplace<2>(2011);
    assert(x.as<const test_object<long long>>() == 2011);
    assert(x.is<const test_object<long long>>());
    assert(is<const test_object<long long>>(x));

    x.emplace<void>();
    assert(x.valueless_by_exception());
  }
  {
    using var_t = variant<test_object<int>, test_object<float>>;
    var_t x;
    assert(x.index() == 0 && x.at<0>() == 0);
    x.emplace<0>(3);
    assert(x.index() == 0 && x.at<0>() == 3);
    var_t y = x;
    assert(x.index() == 0 && x.at<0>() == 3);
    assert(y.index() == 0 && y.at<0>() == 3);
    y.emplace<1>(4);
    assert(y.index() == 1 && y.at<1>() == 4);
    x = y;
    assert(x.index() == 1 && x.at<1>() == 4);
    assert(y.index() == 1 && y.at<1>() == 4);
    x.emplace<0>(3);
    adl_swap(x, y);
    assert(x.index() == 1 && x.at<1>() == 4);
    assert(y.index() == 0 && y.at<0>() == 3);
  }
  {
    using var_t = variant<test_object<int>,
                          ez_vector<test_object<int>>,
                          ez_vector<test_object<float>>>;
    struct X0 {
      static void init(var_t *p) {
        ::new(p) var_t();
        p->emplace<void>();
      }
      static bool good(var_t *p) {
        return p->index() == p->npos() && p->is<void>()
          && p->valueless_by_exception();
      }
    };
    struct X1 {
      static void init(var_t *p) {
        ::new(p) var_t();
      }
      static bool good(var_t *p) {
        return p->index() == 0 && p->is<test_object<int>>()
          && p->as<test_object<int>>() == 0;
      }
    };
    struct X2 {
      static void init(var_t *p) {
        ::new(p) var_t();
        p->emplace<1>({test_object<int>(1), test_object<int>(2)});
      }
      static bool good(var_t *p) {
        return p->index() == 1 && p->is<ez_vector<test_object<int>>>()
          && p->as<ez_vector<test_object<int>>>().size() == 2
          && *p->at<1>().begin() == 1
          && *(p->at<1>().begin() + 1) == 2;
      }
    };

    using P = type_pack<X0, X1, X2>;
    auto check_empty = [](var_t *p)->bool {
      var_t &x = *p;
      if (x.index() == 0)
        return x.template at<0>() == 0;
      if (x.index() == 1)
        return x.template at<1>().empty();
      return true;
    };
    type_pack_for_each<type_pack_mul<P, P>>
      ([=](auto tag1, auto tag2) {
        using T1 = typename decltype(tag1)::type;
        using T2 = typename decltype(tag2)::type;
        test_ownership<var_t>(T1::init, T1::good, T2::init, T2::good,
                                  check_empty);
      });
  }
  {
    using var_t = variant<const test_object<int>, const int>;
    static_assert(!is_copy_assignable_v<var_t>);

    var_t x(in_place_type<const int>, 3);
    var_t y(in_place_index<0>, 1);
    assert(y.index() == 0);
    assert(get<0>(y) == 1);
  }
  {
    using var_t = variant<ez_vector<int>, int>;
    var_t x;
    x.emplace<ez_vector<int>>({1, 2, 3});
    assert(x.index() == 0);
    assert((x.at<0>() == ez_vector<int>{1, 2, 3}));
    x.emplace<int>(3);
    assert(x.is<int>());
    assert(x.as<int>() == 3);
    x.emplace<0>({1, 2, 3});
    assert(x.index() == 0);
    assert((x.at<0>() == ez_vector<int>{1, 2, 3}));
    x.emplace<1>(3);
    assert(x.index() == 1);
    assert(x.at<1>() == 3);
  }
  {
    using var_t = variant<int, long, float>;
    var_t x00(in_place_type<void>);
    var_t x10 = 1;
    var_t x11 = 2;
    var_t x20 = 1L;
    var_t x21 = 2L;
    var_t x30 = 1.0f;
    var_t x31 = 2.0f;
    test_equality(x00, x00);
    test_inequality(x00, x10);
    test_inequality(x21, x30);
    test_lessness(x00, x10);
    test_lessness(x10, x11);
    test_lessness(x30, x31);
    test_lessness(x21, x30);
  }
  {
    using var_t = variant<test_object<int>>;
    var_t x = 3;
    assert(x.at<0>() == 3);
    var_t *p = nullptr;
    const var_t *cp = nullptr;

    assert(get_if<test_object<int>>(p) == nullptr);
    assert(*get_if<test_object<int>>(addressof(x)) == 3);
    assert(get_if<test_object<int>>(cp) == nullptr);
    assert(*get_if<test_object<int>>(addressof(as_const(x))) == 3);

    assert(get_if<0>(p) == nullptr);
    assert(*get_if<0>(addressof(x)) == 3);
    assert(get_if<0>(cp) == nullptr);
    assert(*get_if<0>(addressof(as_const(x))) == 3);
  }
}
struct F {
  template <class T1, class T2>
  static int apply(T1 x, T2 y) {
    return x + y;
  }
};
struct F2 {
  template <class T>
  static T apply(T x) {
    return x * 2;
  }
};
void test_variant_visit() {
  {
    int z{};
    variant<int, float> x;
    using t = decltype(visit([&z](const auto &)->auto & {return z;}, x));
    static_assert(same_as<t, int &>);
  }

  variant<int, long, float, double> x, y;
  x = 3l;
  y = 4.0;
  assert(x.index() == 1 && y.index() == 3);
  assert(visit([](auto x, auto y) {return x * y;}, x, y) == 12);
  assert(visit([](auto x, auto y, auto z) {return x * y * z;},
               x, y, y) == 48);
  visit([](auto x, auto y, auto &z) {z = x * y;}, x, y, y);
  assert(y.is<double>());
  assert(y.as<double>() == 12);
}

void test_dynamic() {
  printf("dynamic: ");

  inner::fns::test_dynamic_t();
  inner::fns::test_dynamic_2();
  inner::fns::test_dynamic_3();
  inner::fns::test_dynamic_4();
  inner::fns::test_dynamic_void();
  inner::fns::test_function();
  inner::fns::test_unique_function();
  inner::fns::test_move_only_function();
  inner::fns::test_type_erased_invocation();
  inner::fns::test_variant();
  inner::fns::test_variant_visit();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_dynamic();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
