#include <re/test.h>

#include <cassert>

namespace re::inner::fns {

void test_owner_ptr() {
  {
    using t = owner_ptr<int>;
    static_assert(semiregular<t>);
    static_assert(swappable<t>);
    static_assert(!is_nothrow_copy_constructible_v<t>);
    static_assert(!is_nothrow_copy_assignable_v<t>);
    static_assert(is_nothrow_move_constructible_v<t>);
    static_assert(is_nothrow_move_assignable_v<t>);
    static_assert(is_nothrow_swappable_v<t>);
    static_assert(is_convertible_v<nullptr_t, t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(!is_nothrow_constructible_v<t, int>);
    static_assert(!is_convertible_v<int, t>);
    static_assert(noexcept(*declval<t &>()));
    static_assert(noexcept(declval<t &>().operator ->()));
    static_assert(noexcept(declval<t &>().empty()));
    static_assert(noexcept(declval<t &>().clear()));
    static_assert(noexcept(declval<t &>() == nullptr));
    static_assert(noexcept(nullptr == declval<t &>()));
  }
  {
    owner_ptr<int> p;
    assert(p.empty());
    owner_ptr pp(1), ppp(2);
    assert(pp != nullptr && *pp == 1);
    assert(nullptr != ppp && *(ppp.operator ->()) == 2);
    p = move(pp);
    assert(p != nullptr && *p == 1);
    assert(pp == nullptr);
    p = p;
    p = move(p);
    assert(p != nullptr && *p == 1);
    adl_swap(p, ppp);
    assert(!p.empty() && *p == 2);
    assert(!ppp.empty() && *ppp == 1);
  }
  {
    owner_ptr<int> a{}, b{2}, c{3};
    assert(a == nullptr);
    assert(!b.empty() && b != nullptr && *b == 2);
    assert(c != nullptr && *c == 3);

    owner_ptr<int> d{move(a)}, e{move(b)}, f{a}, g{c};
    assert(d == nullptr && a == nullptr);
    assert(e != nullptr && *e == 2 && b == nullptr);
    assert(f == nullptr && a == nullptr);
    assert(g != nullptr && *g == 3 && c != nullptr && *c == 3);

    owner_ptr<int> h = nullptr;
    assert(h == nullptr);
    assert(nullptr == h);
    assert(h.empty());
  }
  {
    owner_ptr<int> a{}, b{}, c{3}, d{4};
    assert(a == nullptr && b == nullptr);
    assert(c != nullptr && *c == 3);
    assert(d != nullptr && *d == 4);
    a = a;
    assert(a == nullptr);
    c = c;
    assert(c != nullptr && *c == 3);
    a = b;
    assert(a == nullptr && b == nullptr);
    a = c;
    assert(a != nullptr && *a == 3);
    assert(c != nullptr && *c == 3);
    a = d;
    assert(a != nullptr && *a == 4);
    assert(d != nullptr && *d == 4);
    a = b;
    assert(a == nullptr && b == nullptr);
  }
  {
    owner_ptr<int> a{1}, b{2};
    adl_swap(a, b);
    assert(a != nullptr && *a == 2);
    assert(b != nullptr && *b == 1);
    a.clear();
    assert(a.empty());
    a.clear();
    assert(a.empty());
  }
  {
    using counter_t = instance_counter<int>;
    using pt = owner_ptr<counter_t>;
    const auto is_empty = [](pt *p) {
      return p->empty() && *p == nullptr && nullptr == *p;
    };
    pair<ez_function<void (pt *)>,
         ez_function<bool (pt *)>> a[]
      = {
      {[](pt *p) {::new(p) pt;},
       [](pt *p) {return p->empty() && *p == nullptr && nullptr == *p;}},
      {[](pt *p) {::new(p) pt(1);},
       [](pt *p) {
         return !p->empty() && *p != nullptr && nullptr != *p
           && **p == 1;
       }},
      {[](pt *p) {::new(p) pt(2);},
       [](pt *p) {
         return !p->empty() && *p != nullptr && nullptr != *p
           && **p == 2;
       }}
    };
    for (int i = 0; i != 3; ++i)
      for (int j = 0; j != 3; ++j)
        test_ownership<pt>(a[i].first, a[i].second,
                           a[j].first, a[j].second,
                           is_empty);
    assert(counter_t::count() == 0);
  }
}

void test_test_throwing() {
  static_assert(is_trivial_empty_v<decltype(test_throwing_any)>);
  static_assert(is_trivial_empty_v<decltype(test_throwing<int>)>);
#ifndef RE_NOEXCEPT
  assert(test_throwing_any([]() {throw 0;}));
  assert(!test_throwing_any([]() {}));
  assert(test_throwing<int>([]() {throw 0;}));
  assert(!test_throwing<int>([]() {throw 0.0;}));
  assert(!test_throwing<int>([]() {throw "abc";}));
  assert(!test_throwing<int>([]() {throw 0ll;}));
  struct a {};
  struct b : a {};
  assert(test_throwing<a>([]() {throw b{};}));
#endif
}

void test_test_ownership() {
  {
    using t = instance_counter<int>;
    {
      test_ownership<t>
        ([](t *p) {::new(p) t(1);}, [](t *p) {return *p == 1;},
         [](t *p) {::new(p) t(2);}, [](t *p) {return *p == 2;},
         [](t *) {return true;});
      test_copy<t>
        ([](t *p) {::new(p) t(1);}, [](t *p) {return *p == 1;},
         [](t *p) {::new(p) t(2);}, [](t *p) {return *p == 2;});
      test_move<t>
        ([](t *p) {::new(p) t(1);}, [](t *p) {return *p == 1;},
         [](t *p) {::new(p) t(2);}, [](t *p) {return *p == 2;},
         [](t *) {return true;});
      test_swap<t>
        ([](t *p) {::new(p) t(1);}, [](t *p) {return *p == 1;},
         [](t *p) {::new(p) t(2);}, [](t *p) {return *p == 2;});
      test_copy_construct<t>
        ([](t *p) {::new(p) t(1);}, [](t *p) {return *p == 1;},
         [](t *) {return true;});
    }
    assert(t::count() == 0);
  }

  {
    struct a {};
    struct b {
      using propagate_on_container_copy_assignment = true_type;
    };
    struct c {
      using propagate_on_container_move_assignment = true_type;
    };
    struct d {
      using propagate_on_container_swap = true_type;
    };
    static_assert(!inner::fns::copy_prpg<a>());
    static_assert(!inner::fns::move_prpg<a>());
    static_assert(!inner::fns::swap_prpg<a>());
    static_assert(inner::fns::copy_prpg<b>());
    static_assert(inner::fns::move_prpg<c>());
    static_assert(inner::fns::swap_prpg<d>());

    struct t {
      int value;

      t() = default;
      ~t() = default;
      t(const t &x) : value(x.value) {}
      t &operator =(const t &x) {value = x.value; return *this;}
      t(t &&x) : value(x.value) {x.value = 0;}
      t &operator =(t &&x) {
        if (addressof(x) != this) {
          value = x.value;
          x.value = 0;
        }
        return *this;
      }

      t(int x) : value(x) {}

      int get_allocator() const noexcept {return 0;}
      bool operator ==(const t &y) {return value == y.value;}
    };
    test_allocator_aware_container_ownership<t>
      ([](t *p) {::new(p) t{1};}, [](const t *p) {return p->value == 1;},
       [](t *p) {::new(p) t{2};}, [](const t *p) {return p->value == 2;},
       [](const t *p) {return p->value == 0;});
    test_move<t>
      ([](t *p) {::new(p) t{1};}, [](const t *p) {return p->value == 1;},
       [](t *p) {::new(p) t{2};}, [](const t *p) {return p->value == 2;},
       [](const t *p) {return p->value == 0;});
  }
}

void test_rational_operator_test() {
  struct t {
    int value;
    bool operator ==(const t &x) const & {return value == x.value;}
  };
  struct tt {
    int value;
    strong_ordering operator <=>(const tt &x) const & = default;
  };
  assert(test_equal(t(1), t(1)));
  assert(test_unequal(t(1), t(2)));
  assert(test_unequal(t(2), t(1)));
  assert(test_equal(tt(1), tt(1)));
  assert(test_unequal(tt(1), tt(2)));
  assert(test_unequal(tt(2), tt(1)));
  assert(test_less(tt(1), tt(2)));
  test_equality(t(1), t(1));
  test_inequality(t(1), t(2));
  test_inequality(t(2), t(1));
  test_equality(tt(1), tt(1));
  test_inequality(tt(1), tt(2));
  test_inequality(tt(2), tt(1));
  test_lessness(tt(1), tt(2));
  static_assert(!noexcept(test_equal(declval<t &>(), declval<t &>())));
  static_assert(!noexcept(test_unequal(declval<t &>(), declval<t &>())));
  static_assert(!noexcept(test_less(declval<t &>(), declval<t &>())));
  static_assert(!noexcept(test_equal(declval<tt &>(), declval<tt &>())));
  static_assert(!noexcept(test_unequal(declval<tt &>(), declval<tt &>())));
  static_assert(!noexcept(test_less(declval<tt &>(), declval<tt &>())));

  static_assert(test_equal(nullptr, nullptr));
  static_assert(test_equal(1, 1));
  static_assert(test_equal((short)1, (long)1));
  static_assert(test_unequal(0, 1));
  static_assert(test_unequal(0, 1.0));
  static_assert(test_unequal((short)0, (long)1));
  static_assert(test_less(0, 1));
  static_assert(test_less(0, 1.0));
  test_equality(nullptr, nullptr);
  test_equality(1, 1);
  test_equality((short)1, (long)1);
  test_inequality(0, 1);
  test_inequality(0, 1.0);
  test_inequality((short)0, (long)1);
  test_lessness(0, 1);
  test_lessness(0, 1.0);
}

void test_instance_counter() {
  {
    using t = instance_counter<int>;
    static_assert(regular<t>);
    static_assert(!is_nothrow_constructible_v<t>);
    static_assert(!is_nothrow_copy_constructible_v<t>);
    static_assert(!is_nothrow_copy_assignable_v<t>);
    static_assert(is_nothrow_move_constructible_v<t>);
    static_assert(is_nothrow_move_assignable_v<t>);
    static_assert(is_nothrow_swappable_v<t>);
    static_assert(!noexcept(t(0)));
    static_assert(noexcept(declval<t &>().operator *()));
    static_assert(noexcept(declval<const t &>().operator *()));
    static_assert(noexcept(declval<t &>().operator ->()));
    static_assert(noexcept(declval<const t &>().operator ->()));
    static_assert(noexcept(declval<t &>().operator int &()));
    static_assert(noexcept(declval<t &>().operator const int &()));
    static_assert(noexcept(declval<t &>().count()));
    
    struct tt {
      tt() = default;
      tt(const tt &) = default;
      tt &operator = (const tt &) = default;
      tt(tt &&) {}
      tt &operator = (tt &&) {return *this;}
    };
    using ttt = instance_counter<tt>;
    static_assert(semiregular<ttt>);
    static_assert(!is_nothrow_constructible_v<ttt>);
    static_assert(!is_nothrow_copy_constructible_v<ttt>);
    static_assert(!is_nothrow_copy_assignable_v<ttt>);
    static_assert(!is_nothrow_move_constructible_v<ttt>);
    static_assert(!is_nothrow_move_assignable_v<ttt>);
    static_assert(!is_nothrow_swappable_v<ttt>);

    using tttt = exception_countdown<int>;
    static_assert(regular<tttt>);
    static_assert(!is_nothrow_constructible_v<ttt>);
    static_assert(!is_nothrow_copy_constructible_v<ttt>);
    static_assert(!is_nothrow_copy_assignable_v<ttt>);
    static_assert(!is_nothrow_move_constructible_v<ttt>);
    static_assert(!is_nothrow_move_assignable_v<ttt>);
    static_assert(!is_nothrow_swappable_v<ttt>);
    static_assert(!noexcept(tttt(0)));
    static_assert(noexcept(declval<tttt &>().operator *()));
    static_assert(noexcept(declval<const t &>().operator *()));
    static_assert(noexcept(declval<tttt &>().operator ->()));
    static_assert(noexcept(declval<const t &>().operator ->()));
    static_assert(noexcept(declval<tttt &>().operator int &()));
    static_assert(noexcept(declval<tttt &>().operator const int &()));
    static_assert(noexcept(declval<tttt &>().count()));
  }

  const auto f = []<class T>(type_tag<T>) {
    // counter
    {
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = 2;
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = x;
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = move(x);
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = 2;
        x = xx;
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = 2;
        x = move(xx);
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T x = 1;
        T xx = 2;
        adl_swap(x, xx);
        assert(T::count() == 2);
      }
      {
        assert(T::count() == 0);
        T s;
        T ss = s;
        T sss = move(s);
        assert(T::count() == 3);
        sss = ss;
        ss = sss;
        s = ss;
        ss = s;
        s = move(ss);
        ss = move(s);
        assert(T::count() == 3);
      }
    }
    // access to wrapped object
    {
      T x = 1;
      const T cx = 1;

      assert(*x == 1);
      assert(x == 1);
      [](int x) {assert(x == 1);}(x);
      assert(to_address(x) == addressof(*x));
      assert(*to_address(x) == 1);
      static_assert(is_same_v<decltype(*x), int &>);
      static_assert(is_same_v<decltype(to_address(x)), const int *>);

      assert(*cx == 1);
      assert(cx == 1);
      [](int x) {assert(x == 1);}(cx);
      assert(to_address(cx) == addressof(*cx));
      assert(*to_address(cx) == 1);
      static_assert(is_same_v<decltype(*cx), const int &>);
      static_assert(is_same_v<decltype(to_address(cx)), const int *>);

      assert(x == cx);
      assert(x <=> cx == 0);
      assert(x == 1);
      assert(1 == x);
      assert(x <=> 1 == 0);
      assert(1 <=> x == 0);
    }
  };
  f(type_tag<instance_counter<int>>{});
  f(type_tag<exception_countdown<int>>{});

#ifndef RE_NOEXCEPT
  using T = exception_countdown<int>;
  {
    assert(T::constructor_countdown() == 0);
    assert(T::default_constructor_countdown() == 0);
    assert(T::copy_constructor_countdown() == 0);
    assert(T::move_constructor_countdown() == 0);
    assert(T::copy_assignment_countdown() == 0);
    assert(T::move_assignment_countdown() == 0);
    T::constructor_countdown(1);
    T::default_constructor_countdown(2);
    T::copy_constructor_countdown(3);
    T::move_constructor_countdown(4);
    T::copy_assignment_countdown(5);
    T::move_assignment_countdown(6);
    assert(T::constructor_countdown() == 1);
    assert(T::default_constructor_countdown() == 2);
    assert(T::copy_constructor_countdown() == 3);
    assert(T::move_constructor_countdown() == 4);
    assert(T::copy_assignment_countdown() == 5);
    assert(T::move_assignment_countdown() == 6);
    T::constructor_countdown(0);
    T::default_constructor_countdown(0);
    T::copy_constructor_countdown(0);
    T::move_constructor_countdown(0);
    T::copy_assignment_countdown(0);
    T::move_assignment_countdown(0);
    assert(T::constructor_countdown() == 0);
    assert(T::default_constructor_countdown() == 0);
    assert(T::copy_constructor_countdown() == 0);
    assert(T::move_constructor_countdown() == 0);
    assert(T::copy_assignment_countdown() == 0);
    assert(T::move_assignment_countdown() == 0);
  }
  {
    T::constructor_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T t = 1;}));

    T::constructor_countdown(1);
    assert(test_throwing<logic_error>([&]() {T t = 1;}));

    T::constructor_countdown(3);
    assert(!test_throwing<logic_error>([&]() {T t = 1;}));
    assert(!test_throwing<logic_error>([&]() {T t = 1;}));
    assert(test_throwing<logic_error>([&]() {T t = 1;}));
  }
  {
    T::default_constructor_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T t;}));

    T::default_constructor_countdown(1);
    assert(test_throwing<logic_error>([&]() {T t;}));

    T::default_constructor_countdown(3);
    assert(!test_throwing<logic_error>([&]() {T t;}));
    assert(!test_throwing<logic_error>([&]() {T t;}));
    assert(test_throwing<logic_error>([&]() {T t;}));
  }
  {
    T t;

    T::copy_constructor_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T tt = t;}));

    T::copy_constructor_countdown(1);
    assert(test_throwing<logic_error>([&]() {T tt = t;}));

    T::copy_constructor_countdown(3);
    assert(!test_throwing<logic_error>([&]() {T tt = t;}));
    assert(!test_throwing<logic_error>([&]() {T tt = t;}));
    assert(test_throwing<logic_error>([&]() {T tt = t;}));
  }
  {
    T t;

    T::move_constructor_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T tt = move(t);}));

    T::move_constructor_countdown(1);
    assert(test_throwing<logic_error>([&]() {T tt = move(t);}));

    T::move_constructor_countdown(3);
    assert(!test_throwing<logic_error>([&]() {T tt = move(t);}));
    assert(!test_throwing<logic_error>([&]() {T tt = move(t);}));
    assert(test_throwing<logic_error>([&]() {T tt = move(t);}));
  }
  {
    T t;

    T::copy_assignment_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T tt; tt = t;}));

    T::copy_assignment_countdown(1);
    assert(test_throwing<logic_error>([&]() {T tt; tt = t;}));

    T::copy_assignment_countdown(4);
    assert(!test_throwing<logic_error>([&]() {T tt; tt = t;}));
    assert(!test_throwing<logic_error>([&]() {T tt; tt = t;}));
    assert(!test_throwing<logic_error>([&]() {T tt; tt = t;}));
    assert(test_throwing<logic_error>([&]() {T tt; tt = t;}));
  }
  {
    T t;

    T::move_assignment_countdown(0);
    assert(!test_throwing<logic_error>([&]() {T tt; tt = move(t);}));

    T::move_assignment_countdown(1);
    assert(test_throwing<logic_error>([&]() {T tt; tt = move(t);}));

    T::move_assignment_countdown(4);
    assert(!test_throwing<logic_error>([&]() {T tt; tt = move(t);}));
    assert(!test_throwing<logic_error>([&]() {T tt; tt = move(t);}));
    assert(!test_throwing<logic_error>([&]() {T tt; tt = move(t);}));
    assert(test_throwing<logic_error>([&]() {T tt; tt = move(t);}));
  }
#endif
}

void test_ez_function() {
  static_assert(is_nothrow_constructible_v<ez_dynamic<int>>);
  static_assert(!is_nothrow_copy_constructible_v<ez_dynamic<int>>);
  static_assert(!is_nothrow_copy_assignable_v<ez_dynamic<int>>);
  static_assert(is_nothrow_move_constructible_v<ez_dynamic<int>>);
  static_assert(is_nothrow_move_assignable_v<ez_dynamic<int>>);
  static_assert(!noexcept(ez_dynamic<int>::make(0)));
  static_assert(noexcept(declval<ez_dynamic<int> &>().empty()));
  static_assert(noexcept(declval<ez_dynamic<int> &>().clear()));
  static_assert(noexcept(declval<ez_dynamic<int> &>().operator ->()));
  static_assert(noexcept(declval<ez_dynamic<int> &>().operator *()));

  static_assert(is_nothrow_constructible_v<ez_function<int (int)>>);
  static_assert(!is_nothrow_copy_constructible_v<ez_function<int (int)>>);
  static_assert(!is_nothrow_copy_assignable_v<ez_function<int (int)>>);
  static_assert(is_nothrow_move_constructible_v<ez_function<int (int)>>);
  static_assert(is_nothrow_move_assignable_v<ez_function<int (int)>>);
  static_assert(!noexcept(ez_function<int (int)>([](int)->int {return 0;})));
  static_assert(!noexcept(declval<ez_function<int (int)> &>()(0)));
  static_assert(noexcept(declval<ez_function<int (int)> &>().empty()));
  static_assert(noexcept(declval<ez_function<int (int)> &>().clear()));

  // ez_dynamic
  {
    using t = instance_counter<int>;
    using d = ez_dynamic<t>;

    assert(t::count() == 0);
    {
      d x;
      assert(x.empty());
      assert(t::count() == 0);
    }
    {
      d x = d::make(1);
      assert(!x.empty());
      assert(*x == 1);
      assert(t::count() == 1);
    }
    assert(t::count() == 0);

    {
      d x = d::make(1);
      assert(*x == 1);
      assert(t::count() == 1);
      d y = x;
      assert(*x == 1 && *y == 1);
      assert(t::count() == 2);

      d z;
      assert(z.empty());
      assert(t::count() == 2);
      x = z;
      assert(x.empty() && z.empty());
      assert(t::count() == 1);

      x = d::make(2);
      assert(*x == 2);
      assert(t::count() == 2);
      z = x;
      assert(*z == 2 && *x == 2);
      assert(t::count() == 3);
    }
    assert(t::count() == 0);
    {
      d x;
      d y = x;
      assert(x.empty() && y.empty());
      assert(t::count() == 0);
      x = y;
      assert(x.empty() && y.empty());
      assert(t::count() == 0);
    }
    assert(t::count() == 0);
    {
      d x = d::make(1);
      d y = d::make(2);
      assert(t::count() == 2);
      x = y;
      assert(*x == 2 && *y == 2);
      assert(t::count() == 2);
    }
    assert(t::count() == 0);
    {
      d x;
      x = x;
      assert(x.empty());
      assert(t::count() == 0);
    }
    assert(t::count() == 0);
    {
      d x = d::make(1);
      x = x;
      assert(*x == 1);
      assert(!x.empty());
      assert(t::count() == 1);
    }
    assert(t::count() == 0);
  }
  {
    using t = instance_counter<int>;

    struct t0 {
      virtual ~t0() {}
      virtual t0 *clone() const {return nullptr;}
    };
    struct t1 : t0 {
      t value;

      t1() = default;
      virtual ~t1() override = default;
      t1(const t1 &) = delete;
      t1 &operator =(const t1 &) = delete;
      t1(t1 &&) = delete;
      t1 &operator =(t1 &&) = delete;

      explicit t1(int x) : value(x) {}

      virtual t0 *clone() const override {return new t1{*value};}
    };
    struct t2 : t0 {
      t value;

      t2() = default;
      virtual ~t2() override = default;
      t2(const t2 &) = delete;
      t2 &operator =(const t2 &) = delete;
      t2(t2 &&) = delete;
      t2 &operator =(t2 &&) = delete;

      explicit t2(int x) : value(x) {}

      virtual t0 *clone() const override {return new t2{*value};}
    };

    using d = ez_dynamic<t0>;
    {
      const auto is_1 = [](d &x) {
        return !x.empty()
          && typeid(*x) == typeid(t1)
          && to_address(x) == addressof(*x)
          && ((t1 &)*x).value == 1;
      };
      const auto is_2 = [](d &x) {
        return !x.empty()
          && typeid(*x) == typeid(t2)
          && to_address(x) == addressof(*x)
          && ((t2 &)*x).value == 2;
      };

      d x = d::make<t1>(1);
      assert(is_1(x));
      assert(t::count() == 1);
      d y = d::make<t2>(2);
      assert(is_2(y));
      assert(t::count() == 2);
      d z;
      assert(z.empty());
      assert(t::count() == 2);

      {
        d tmp = x;
        assert(is_1(tmp));
        assert(t::count() == 3);
      }
      assert(t::count() == 2);
      {
        d tmp = z;
        assert(tmp.empty());
        assert(t::count() == 2);
      }
      assert(t::count() == 2);
      {
        d tmp;
        assert(tmp.empty());
        assert(t::count() == 2);
        d tmp2 = move(tmp);
        assert(tmp2.empty());
        assert(t::count() == 2);
      }
      assert(t::count() == 2);

      x = move(x);
      assert(is_1(x));
      assert(t::count() == 2);
      x = x;
      assert(is_1(x));
      assert(t::count() == 2);
      z = move(z);
      assert(z.empty());
      assert(t::count() == 2);
      z = z;
      assert(z.empty());
      assert(t::count() == 2);

      {
        d tmp;
        assert(tmp.empty());
        assert(t::count() == 2);
        tmp = z;
        assert(tmp.empty());
        assert(z.empty());
        assert(t::count() == 2);
        tmp = move(z);
        assert(tmp.empty());
        assert(z.empty());
        assert(t::count() == 2);
      }
      assert(t::count() == 2);

      {
        z = x;
        assert(is_1(z));
        assert(t::count() == 3);
        z.clear();
        assert(z.empty());
        assert(t::count() == 2);
      }
      {
        z = move(y);
        assert(y.empty());
        assert(is_2(z));
        assert(t::count() == 2);
        y = move(z);
        assert(z.empty());
        assert(is_2(y));
        assert(t::count() == 2);
      }
      assert(t::count() == 2);
      {
        d tmp;
        assert(tmp.empty());
        assert(t::count() == 2);
        tmp = x;
        assert(is_1(tmp));
        assert(t::count() == 3);
        tmp = y;
        assert(is_2(tmp));
        assert(is_2(y));
        assert(t::count() == 3);
        tmp = move(y);
        assert(is_2(tmp));
        assert(y.empty());
        assert(t::count() == 2);
      }
      assert(t::count() == 1);
      assert(is_1(x));
      assert(y.empty());
      assert(z.empty());
      x = move(y);
      assert(x.empty());
      assert(y.empty());
      assert(z.empty());
      assert(t::count() == 0);
    }
    assert(t::count() == 0);
  }
  // ez_function
  {
    {
      ez_function<int ()> f;
      assert(f.empty());
    }
    {
      ez_function<int ()> f = []() {return 1;};
      assert(!f.empty() && f() == 1);
      ez_function<int ()> ff = f;
      assert(!f.empty() && f() == 1);
      assert(!ff.empty() && ff() == 1);
      ff = []() {return 2;};
      assert(!ff.empty() && ff() == 2);
      f = ff;
      assert(!ff.empty() && ff() == 2);
      assert(!f.empty() && f() == 2);
      f = move(ff);
      assert(ff.empty());
      assert(!f.empty() && f() == 2);
      ez_function<int ()> fff(move(f));
      assert(f.empty());
      assert(!fff.empty() && fff() == 2);
      fff.clear();
      assert(fff.empty());
    }

    {
#ifndef RE_NOEXCEPT
      ez_function<void ()> f;
      assert(test_throwing<logic_error>(f));
      f = []() {};
      assert(!test_throwing<logic_error>(f));
#endif
    }
  }
}

void test_ez_vector() {
  using t = instance_counter<int>;
  using vt = ez_vector<t>;

  {
    static_assert(noexcept(declval<vt &>().begin()));
    static_assert(noexcept(declval<vt &>().end()));
    static_assert(noexcept(declval<const vt &>().begin()));
    static_assert(noexcept(declval<const vt &>().end()));
    static_assert(noexcept(declval<vt &>().max_size()));
    static_assert(noexcept(declval<vt &>().size()));
    static_assert(noexcept(declval<vt &>().empty()));
    static_assert(noexcept(declval<vt &>().capacity()));
    static_assert(is_nothrow_constructible_v<vt>);
    static_assert(is_nothrow_destructible_v<vt>);
    static_assert(!is_nothrow_copy_constructible_v<vt>);
    static_assert(!is_nothrow_copy_assignable_v<vt>);
    static_assert(is_nothrow_move_constructible_v<vt>);
    static_assert(is_nothrow_move_assignable_v<vt>);
    static_assert(is_nothrow_swappable_v<vt>);
    static_assert(is_constructible_v<vt, initializer_list<vt::value_type>>);
    static_assert(!is_nothrow_constructible_v<vt, initializer_list<int>>);
    static_assert(three_way_comparable<vt, strong_ordering>);
    static_assert(equality_comparable<vt>);
    static_assert(!noexcept(declval<vt &>() == declval<vt &>()));
    static_assert(!noexcept(declval<vt &>() <=> declval<vt &>()));
    static_assert(!noexcept(declval<vt &>().insert
                            (declval<vt::const_iterator>(),
                             declval<const vt::value_type &>())));
    static_assert(noexcept(declval<vt &>().erase
                           (declval<vt::const_iterator>(),
                            declval<vt::const_iterator>())));
    static_assert(noexcept(declval<vt &>().erase
                           (declval<vt::const_iterator>())));

    struct t {};
    static_assert(!equality_comparable<ez_vector<t>>);
    static_assert(!three_way_comparable<ez_vector<t>>);
  }

  {
    vt v;
    assert(v.empty());
    assert(t::count() == 0);

    assert(v.begin() == v.end());
    assert(v.begin() == as_const(v).end());
    assert(as_const(v).begin() == v.end());
    assert(as_const(v).begin() == as_const(v).end());

    assert(v.max_size() > 0);
    assert(v.size() == 0);
    assert(v.empty());
    assert(v.capacity() == 0);

    assert(v == v);
    assert(!(v != v));
    assert(!(v < v));
    assert(v <= v);
    assert(!(v > v));
    assert(v >= v);
    assert((v <=> v) == 0);

    static_assert(is_same_v<vt::value_type, t>);
    static_assert(is_same_v<vt::reference, t &>);
    static_assert(is_same_v<vt::const_reference, const t &>);
    static_assert(is_same_v<vt::iterator, t *>);
    static_assert(is_same_v<vt::const_iterator, const t *>);
    static_assert(is_same_v<vt::difference_type, ptrdiff_t>);
    static_assert(is_same_v<vt::size_type, size_t>);
  }
  assert(t::count() == 0);
  {
    vt v;
    for (int i = 0; i < 10; ++i) {
      t *ii = v.insert(v.end(), i);
      assert(*ii == i);
    }
    for (int i = 0; i < 10; ++i)
      assert(*(v.begin() + i) == i);
    assert(v.max_size() > v.size());
    assert(v.size() == 10);
    assert(!v.empty());
    assert(v.capacity() == 16);

    vt v2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; ++i)
      assert(*(v2.begin() + i) == i);
    assert(v2.max_size() > v2.size());
    assert(v2.size() == 10);
    assert(!v2.empty());
    assert(v2.capacity() == 16);

    assert(v == v2);
    assert((v <=> v2) == 0);
  }
  assert(t::count() == 0);
  {
    {
      vt v;
      vt::iterator i = v.erase(v.begin(), v.end());
      assert(i == v.begin());
      assert(v.empty());
    }
    {
      vt v = {1};
      assert(!v.empty());
      assert(v.size() == 1);
      assert(v.capacity() == 1);
      assert(*v.begin() == 1);

      vt::iterator i = v.erase(v.begin());
      assert(v.empty());
      assert(v.size() == 0);
      assert(v.capacity() == 1);
      assert(i == v.end());
    }
    {
      vt v = {1, 2, 3, 4, 5};
      assert(!v.empty());
      assert(v.size() == 5);
      assert(v.capacity() == 8);

      t *i = v.erase(v.begin());
      assert(*i == 2 && i == v.begin());
      assert(!v.empty());
      assert(v.size() == 4);
      assert(v.capacity() == 8);
      assert(v == vt({2, 3, 4, 5}));
    }
    {
      vt v = {1, 2, 3, 4, 5};
      assert(!v.empty());
      assert(v.size() == 5);
      assert(v.capacity() == 8);

      t *i = v.erase(v.end() - 1);
      assert(i == v.end());
      assert(!v.empty());
      assert(v.size() == 4);
      assert(v.capacity() == 8);
      assert(v == vt({1, 2, 3, 4}));
    }
    {
      vt v = {1, 2, 3, 4, 5};
      assert(!v.empty());
      assert(v.size() == 5);
      assert(v.capacity() == 8);

      t *i = v.erase(v.begin() + 2);
      assert(i == v.begin() + 2 && *i == 4);
      assert(!v.empty());
      assert(v.size() == 4);
      assert(v.capacity() == 8);
      assert(v == vt({1, 2, 4, 5}));
    }
    {
      vt v = {1, 2, 3, 4, 5};
      v.erase(v.begin(), v.end());
      assert(v.empty());
      assert(v.size() == 0);
      assert(v.capacity() == 8);
    }
    {
      vt v = {1, 2, 3, 4, 5};
      auto i = v.erase(v.begin(), v.begin() + 2);
      assert(i == v.begin());
      assert(v == vt({3, 4, 5}));
      i = v.erase(v.begin() + 1, v.end());
      assert(i == v.end());
      assert(v == vt({3}));
    }
    {
      vt v = {1, 2, 3, 4, 5};
      auto i = v.erase(v.begin() + 1, v.end() - 1);
      assert(*i == 5);
      assert(v == vt({1, 5}));
    }
  }
  assert(t::count() == 0);
  {
    vt v = {1};
    t *i = v.insert(v.begin(), 2);
    assert(*i = 2);
    assert(v.size() == 2);
    assert(v.capacity() == 2);
    assert(v == vt({2, 1}));
    i = v.insert(v.begin() + 1, 3);
    assert(*i == 3);
    assert(v == vt({2, 3, 1}));
    i = v.insert(v.begin(), 4);
    assert(*i == 4);
    assert(v == vt({4, 2, 3, 1}));
    i = v.insert(v.begin() + 2, 5);
    assert(*i == 5);
    assert(v == vt({4, 2, 5, 3, 1}));
    assert(v.capacity() == 8);
  }
  assert(t::count() == 0);
  {
    vt v = {1, 2, 3};
    vt vv = v;
    assert(v == vv);
  }
  assert(t::count() == 0);
  {
    test_ownership<vt>([](auto p) {::new(p) vt({1, 2, 3, 4});},
                       [](auto p) {return *p == vt({1, 2, 3, 4});},
                       [](auto p) {::new(p) vt({5, 6, 7});},
                       [](auto p) {return *p == vt({5, 6, 7});},
                       [](auto p) {return p->empty() && p->capacity() == 0;});
    test_ownership<vt>([](auto p) {::new(p) vt({});},
                       [](auto p) {return p->empty();},
                       [](auto p) {::new(p) vt({5, 6, 7});},
                       [](auto p) {return *p == vt({5, 6, 7});},
                       [](auto p) {return p->empty() && p->capacity() == 0;});
  }
  assert(t::count() == 0);
}

void test_ez_forward_list() {
  using t = instance_counter<int>;
  using l_t = ez_forward_list<t>;
  // member types
  {
    static_assert(same_as<l_t::value_type, t>);
    static_assert(same_as<l_t::reference, t &>);
    static_assert(same_as<l_t::const_reference, const t &>);
    static_assert(is_same_v<l_t::iterator::iterator_category,
                            forward_iterator_tag>);
    static_assert(is_same_v<l_t::const_iterator::iterator_category,
                            forward_iterator_tag>);
    static_assert(is_convertible_v<l_t::iterator, l_t::const_iterator>);
    static_assert(!is_convertible_v<l_t::const_iterator, l_t::iterator>);
    static_assert(same_as<l_t::difference_type, ptrdiff_t>);
    static_assert(same_as<l_t::size_type, size_t>);
  }
  // special member functions
  {
    const auto mty = [](l_t *p) {
      return p->empty() && p->begin() == p->end();
    };
    const auto init_f = [](l_t *l, int i) {
      new(l) l_t{};
      auto z = l->before_begin();
      for (int x = 0; x != i; ++x)
        z = l->insert_after(z, t(x));
    };
    const auto eq_f = [=](l_t *l, int i)->bool {
      int ii = 0;
      for (auto p = l->begin(); p != l->end(); ++p) {
        if (*p != ii)
          return false;
        ++ii;
      }
      return true;
    };

    for (int i : ez_vector{0, 1, 2, 3, 4})
      for (int j : ez_vector{0, 1, 2, 3, 4})
        test_ownership<l_t>(bind_back(init_f, i), bind_back(eq_f, i),
                            bind_back(init_f, j), bind_back(eq_f, j),
                            mty);
    assert(t::count() == 0);
  }
  // construct from initializer_list
  // begin
  // end
  // empty
  // max_size
  {
    const auto comp = [](l_t &l, const ez_vector<int> &v) {
      test_fitr(l.begin(), l.end(), v);
      test_fitr(l.begin(), as_const(l).end(), v);
      test_fitr(as_const(l).begin(), l.end(), v);
      test_fitr(as_const(l).begin(), as_const(l).end(), v);
      test_rng(l, v);
      assert(l.max_size() > 1000);
    };
    {
      l_t l = {1};
      const ez_vector v = {1};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3};
      const ez_vector v = {1, 2, 3};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3, 4, 5};
      const ez_vector v = {1, 2, 3, 4, 5};
      comp(l, v);
    }
  }
  // new_node
  // delete_node
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    l.delete_node(i);
  }
  // link_after
  // unlink_after
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    i = l.link_after(l.before_begin(), i);
    assert(*i == 1);
    test_fitr(l, ez_vector{1});
    test_rng(l, ez_vector{1});
  }
  // insert_after
  // erase
  {
    for (int i = 0; i != 6; ++i) {
      l_t l = {1, 2, 3, 4, 5};
      ez_vector v = {1, 2, 3, 4, 5};
      auto p = l.before_begin();
      for (auto c = i; c; --c)
        ++p;
      assert(*l.insert_after(p, 0) == 0);
      v.insert(v.begin() + i, 0);
      test_rng(l, v);
    }
    for (int i = 0; i != 5; ++i) {
      l_t l = {1, 2, 3, 4, 5};
      ez_vector v = {1, 2, 3, 4, 5};
      auto p = l.before_begin();
      for (auto c = i; c; --c)
        ++p;
      const auto pp = ++(++copy(p));
      assert(l.erase_after(p) == pp);
      v.erase(v.begin() + i);
      test_rng(l, v);
    }
    for (int i = 0; i != 6; ++i)
      for (int ii = i + 1; ii != 6; ++ii) {
        l_t l = {1, 2, 3, 4, 5};
        ez_vector v = {1, 2, 3, 4, 5};

        auto p = l.before_begin();
        for (auto c = i; c; --c)
          ++p;

        auto pp = l.begin();
        for (auto c = ii; c; --c)
          ++pp;
        
        assert(l.erase_after(p, pp) == pp);
        v.erase(v.begin() + i, v.begin() + ii);
        test_rng(l, v);
      }
  }
  // == <=>
  {
    l_t l0 = {};
    l_t l = {1, 2, 3};
    l_t ll = {1, 3, 2};
    l_t lll = {3, 2, 1, 0};
    assert(l0 < l && l < ll && l < lll);
    assert(l0 != l && l != ll && ll != lll && l != lll);
    assert(l0 == l0 && l == l && ll == ll && lll == lll);
  }
  assert(t::count() == 0);
}

void test_ez_slist() {
  using t = instance_counter<int>;
  using l_t = ez_slist<t>;
  // member types
  {
    static_assert(same_as<l_t::value_type, t>);
    static_assert(same_as<l_t::reference, t &>);
    static_assert(same_as<l_t::const_reference, const t &>);
    static_assert(is_same_v<l_t::iterator::iterator_category,
                            forward_iterator_tag>);
    static_assert(is_same_v<l_t::const_iterator::iterator_category,
                            forward_iterator_tag>);
    static_assert(is_convertible_v<l_t::iterator, l_t::const_iterator>);
    static_assert(!is_convertible_v<l_t::const_iterator, l_t::iterator>);
    static_assert(same_as<l_t::difference_type, ptrdiff_t>);
    static_assert(same_as<l_t::size_type, size_t>);
  }
  // special member functions
  {
    const auto mty = [](l_t *p) {
      return p->empty() && p->size() == 0 && p->begin() == p->end();
    };
    const auto init_f = [](l_t *l, int i) {
      new(l) l_t{};
      auto z = l->before_begin();
      for (int x = 0; x != i; ++x)
        z = l->insert_after(z, t(x));
    };
    const auto eq_f = [=](l_t *l, int i)->bool {
      int ii = 0;
      for (auto p = l->begin(); p != l->end(); ++p) {
        if (*p != ii)
          return false;
        ++ii;
      }
      if (l->size() != to_unsigned(ii))
        return false;
      return true;
    };

    for (int i : ez_vector{0, 1, 2, 3, 4})
      for (int j : ez_vector{0, 1, 2, 3, 4})
        test_ownership<l_t>(bind_back(init_f, i), bind_back(eq_f, i),
                            bind_back(init_f, j), bind_back(eq_f, j),
                            mty);
    assert(t::count() == 0);
  }
  // construct from initializer_list
  // begin
  // end
  // empty
  // size
  // max_size
  {
    const auto comp = [](l_t &l, const ez_vector<int> &v) {
      test_fitr(l.begin(), l.end(), v);
      test_fitr(l.begin(), as_const(l).end(), v);
      test_fitr(as_const(l).begin(), l.end(), v);
      test_fitr(as_const(l).begin(), as_const(l).end(), v);
      test_rng(l, v);
      assert(l.size() == v.size());
      assert(l.max_size() > 1000);
    };
    {
      l_t l = {1};
      const ez_vector v = {1};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3};
      const ez_vector v = {1, 2, 3};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3, 4, 5};
      const ez_vector v = {1, 2, 3, 4, 5};
      comp(l, v);
    }
  }
  // new_node
  // delete_node
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    l.delete_node(i);
  }
  // link_after
  // unlink_after
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    i = l.link_after(l.before_begin(), i);
    assert(*i == 1);
    test_fitr(l, ez_vector{1});
    test_rng(l, ez_vector{1});
  }
  // insert_after
  // erase
  {
    for (int i = 0; i != 6; ++i) {
      l_t l = {1, 2, 3, 4, 5};
      ez_vector v = {1, 2, 3, 4, 5};
      auto p = l.before_begin();
      for (auto c = i; c; --c)
        ++p;
      assert(*l.insert_after(p, 0) == 0);
      v.insert(v.begin() + i, 0);
      test_rng(l, v);
    }
    for (int i = 0; i != 5; ++i) {
      l_t l = {1, 2, 3, 4, 5};
      ez_vector v = {1, 2, 3, 4, 5};
      auto p = l.before_begin();
      for (auto c = i; c; --c)
        ++p;
      const auto pp = ++(++copy(p));
      assert(l.erase_after(p) == pp);
      v.erase(v.begin() + i);
      test_rng(l, v);
    }
    for (int i = 0; i != 6; ++i)
      for (int ii = i + 1; ii != 6; ++ii) {
        l_t l = {1, 2, 3, 4, 5};
        ez_vector v = {1, 2, 3, 4, 5};

        auto p = l.before_begin();
        for (auto c = i; c; --c)
          ++p;

        auto pp = l.begin();
        for (auto c = ii; c; --c)
          ++pp;
        
        assert(l.erase_after(p, pp) == pp);
        v.erase(v.begin() + i, v.begin() + ii);
        test_rng(l, v);
      }
  }
  // == <=>
  {
    l_t l0 = {};
    l_t l = {1, 2, 3};
    l_t ll = {1, 3, 2};
    l_t lll = {3, 2, 1, 0};
    assert(l0 < l && l < ll && l < lll);
    assert(l0 != l && l != ll && ll != lll && l != lll);
    assert(l0 == l0 && l == l && ll == ll && lll == lll);
  }
  assert(t::count() == 0);
}

void test_ez_bidirectional_list() {
  using t = instance_counter<int>;
  using l_t = ez_bidirectional_list<t>;
  // member types
  {
    static_assert(same_as<l_t::value_type, t>);
    static_assert(same_as<l_t::reference, t &>);
    static_assert(same_as<l_t::const_reference, const t &>);
    static_assert(is_same_v<l_t::iterator::iterator_category,
                            bidirectional_iterator_tag>);
    static_assert(is_same_v<l_t::const_iterator::iterator_category,
                            bidirectional_iterator_tag>);
    static_assert(is_convertible_v<l_t::iterator, l_t::const_iterator>);
    static_assert(!is_convertible_v<l_t::const_iterator, l_t::iterator>);
    static_assert(same_as<l_t::difference_type, ptrdiff_t>);
    static_assert(same_as<l_t::size_type, size_t>);
  }
  // special member functions
  {
    const auto mty = [](l_t *p) {
      return p->empty() && p->begin() == p->end();
    };
    const auto init_f = [](l_t *l, int i) {
      new(l) l_t{};
      for (int x = 0; x != i; ++x)
        l->insert(l->end(), t(x));
    };
    const auto eq_f = [=](l_t *l, int i)->bool {
      int ii = 0;
      for (auto p = l->begin(); p != l->end(); ++p) {
        if (*p != ii)
          return false;
        ++ii;
      }
      for (auto p = l->end(); p != l->begin();) {
        --p;
        --ii;
        if (*p != ii)
          return false;
      }
      return true;
    };

    for (int i : ez_vector{0, 1, 2, 3, 4})
      for (int j : ez_vector{0, 1, 2, 3, 4})
        test_ownership<l_t>(bind_back(init_f, i), bind_back(eq_f, i),
                            bind_back(init_f, j), bind_back(eq_f, j),
                            mty);
    assert(t::count() == 0);
  }
  // construct from initializer_list
  // begin
  // end
  // empty
  // max_size
  {
    const auto comp = [](l_t &l, const ez_vector<int> &v) {
      test_bitr(l.begin(), l.end(), v);
      test_bitr(l.begin(), as_const(l).end(), v);
      test_bitr(as_const(l).begin(), l.end(), v);
      test_bitr(as_const(l).begin(), as_const(l).end(), v);
      test_rng(l, v);
      assert(l.max_size() > 0);
    };
    {
      l_t l = {1};
      const ez_vector v = {1};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3};
      const ez_vector v = {1, 2, 3};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3, 4, 5};
      const ez_vector v = {1, 2, 3, 4, 5};
      comp(l, v);
    }
  }
  // new_node
  // delete_node
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    l.delete_node(i);
  }
  // link
  // unlink
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    i = l.link(l.end(), i);
    assert(*i == 1);
    test_bitr(l, ez_vector{1});
    test_rng(l, ez_vector{1});
  }
  // insert
  {
    for (int i : {0, 1, 2, 3, 4, 5})
      for (int j = 0; j != i + 1; ++j) {
        l_t l;
        for (int n = 0; n != i; ++n)
          l.insert(l.end(), n);
        ez_vector<int> v;
        for (int n = 0; n != i; ++n)
          v.insert(v.end(), n);
        test_rng(l, v);
        test_bitr(l, v);

        l_t::iterator it = l.begin();
        for (int jj = j; jj != 0; --jj)
          ++it;
        l_t::iterator it2 = l.insert(it, 666);
        assert(*it2 == 666);
        v.insert(v.begin() + j, 666);
        test_rng(l, v);
        test_bitr(l, v);
      }
  }
  // erase
  {
    for (int i : {1, 2, 3, 4, 5})
      for (int j = 0; j != i; ++j) {
        l_t l;
        for (int n = 0; n != i; ++n)
          l.insert(l.end(), n);
        ez_vector<int> v;
        for (int n = 0; n != i; ++n)
          v.insert(v.end(), n);
        test_rng(l, v);
        test_bitr(l, v);

        l_t::iterator it = l.begin();
        for (int jj = j; jj != 0; --jj)
          ++it;
        l_t::iterator it_next = it;
        ++it_next;
        it = l.erase(it);
        assert(it == it_next);

        v.erase(v.begin() + j);
        test_rng(l, v);
        test_bitr(l, v);
      }
  }
  {
    for (int i : {1, 2, 3, 4, 5})
      for (int j = 0; j != i; ++j)
        for (int k = j; k != i + 1; ++k) {
          l_t l;
          for (int n = 0; n != i; ++n)
            l.insert(l.end(), n);
          ez_vector<int> v;
          for (int n = 0; n != i; ++n)
            v.insert(v.end(), n);
          test_rng(l, v);
          test_bitr(l, v);

          l_t::iterator it = l.begin();
          for (int jj = j; jj != 0; --jj)
            ++it;
          l_t::iterator it2 = l.begin();
          for (int kk = k; kk != 0; --kk)
            ++it2;
          it = l.erase(it, it2);
          assert(it == it2);

          v.erase(v.begin() + j, v.begin() + k);
          test_rng(l, v);
          test_bitr(l, v);
        }
  }
  // == <=>
  {
    l_t l0 = {};
    l_t l = {1, 2, 3};
    l_t ll = {1, 3, 2};
    l_t lll = {3, 2, 1, 0};
    assert(l0 < l && l < ll && l < lll);
    assert(l0 != l && l != ll && ll != lll && l != lll);
    assert(l0 == l0 && l == l && ll == ll && lll == lll);
  }
  assert(t::count() == 0);
}

void test_ez_list() {
  using t = instance_counter<int>;
  using l_t = ez_list<t>;
  // member types
  {
    static_assert(same_as<l_t::value_type, t>);
    static_assert(same_as<l_t::reference, t &>);
    static_assert(same_as<l_t::const_reference, const t &>);
    static_assert(is_same_v<l_t::iterator::iterator_category,
                            bidirectional_iterator_tag>);
    static_assert(is_same_v<l_t::const_iterator::iterator_category,
                            bidirectional_iterator_tag>);
    static_assert(is_convertible_v<l_t::iterator, l_t::const_iterator>);
    static_assert(!is_convertible_v<l_t::const_iterator, l_t::iterator>);
    static_assert(same_as<l_t::difference_type, ptrdiff_t>);
    static_assert(same_as<l_t::size_type, size_t>);
  }
  // special member functions
  {
    const auto mty = [](l_t *p) {
      return p->empty() && p->begin() == p->end();
    };
    const auto init_f = [](l_t *l, int i) {
      new(l) l_t{};
      for (int x = 0; x != i; ++x)
        l->insert(l->end(), t(x));
    };
    const auto eq_f = [=](l_t *l, int i)->bool {
      int ii = 0;
      for (auto p = l->begin(); p != l->end(); ++p) {
        if (*p != ii)
          return false;
        ++ii;
      }
      for (auto p = l->end(); p != l->begin();) {
        --p;
        --ii;
        if (*p != ii)
          return false;
      }
      return true;
    };

    for (int i : ez_vector{0, 1, 2, 3, 4})
      for (int j : ez_vector{0, 1, 2, 3, 4})
        test_ownership<l_t>(bind_back(init_f, i), bind_back(eq_f, i),
                            bind_back(init_f, j), bind_back(eq_f, j),
                            mty);
    assert(t::count() == 0);
  }
  // construct from initializer_list
  // begin
  // end
  // empty
  // size
  // max_size
  {
    const auto comp = [](l_t &l, const ez_vector<int> &v) {
      test_bitr(l.begin(), l.end(), v);
      test_bitr(l.begin(), as_const(l).end(), v);
      test_bitr(as_const(l).begin(), l.end(), v);
      test_bitr(as_const(l).begin(), as_const(l).end(), v);
      test_rng(l, v);
      assert(l.size() == v.size());
      if (l.size() != 0)
        assert(!l.empty());
      assert(l.max_size() > l.size());
    };
    {
      l_t l = {1};
      const ez_vector v = {1};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3};
      const ez_vector v = {1, 2, 3};
      comp(l, v);
    }
    {
      l_t l = {1, 2, 3, 4, 5};
      const ez_vector v = {1, 2, 3, 4, 5};
      comp(l, v);
    }
  }
  // new_node
  // delete_node
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    l.delete_node(i);
  }
  // link
  // unlink
  {
    l_t l;
    l_t::iterator i = l.new_node(1);
    i = l.link(l.end(), i);
    assert(*i == 1);
    test_bitr(l, ez_vector{1});
    test_rng(l, ez_vector{1});
  }
  // insert
  {
    for (int i : {0, 1, 2, 3, 4, 5})
      for (int j = 0; j != i + 1; ++j) {
        l_t l;
        for (int n = 0; n != i; ++n)
          l.insert(l.end(), n);
        ez_vector<int> v;
        for (int n = 0; n != i; ++n)
          v.insert(v.end(), n);
        test_rng(l, v);
        test_bitr(l, v);

        l_t::iterator it = l.begin();
        for (int jj = j; jj != 0; --jj)
          ++it;
        l_t::iterator it2 = l.insert(it, 666);
        assert(*it2 == 666);
        v.insert(v.begin() + j, 666);
        test_rng(l, v);
        test_bitr(l, v);
      }
  }
  // erase
  {
    for (int i : {1, 2, 3, 4, 5})
      for (int j = 0; j != i; ++j) {
        l_t l;
        for (int n = 0; n != i; ++n)
          l.insert(l.end(), n);
        ez_vector<int> v;
        for (int n = 0; n != i; ++n)
          v.insert(v.end(), n);
        test_rng(l, v);
        test_bitr(l, v);

        l_t::iterator it = l.begin();
        for (int jj = j; jj != 0; --jj)
          ++it;
        l_t::iterator it_next = it;
        ++it_next;
        it = l.erase(it);
        assert(it == it_next);

        v.erase(v.begin() + j);
        test_rng(l, v);
        test_bitr(l, v);
      }
  }
  {
    for (int i : {1, 2, 3, 4, 5})
      for (int j = 0; j != i; ++j)
        for (int k = j; k != i + 1; ++k) {
          l_t l;
          for (int n = 0; n != i; ++n)
            l.insert(l.end(), n);
          ez_vector<int> v;
          for (int n = 0; n != i; ++n)
            v.insert(v.end(), n);
          test_rng(l, v);
          test_bitr(l, v);

          l_t::iterator it = l.begin();
          for (int jj = j; jj != 0; --jj)
            ++it;
          l_t::iterator it2 = l.begin();
          for (int kk = k; kk != 0; --kk)
            ++it2;
          it = l.erase(it, it2);
          assert(it == it2);

          v.erase(v.begin() + j, v.begin() + k);
          test_rng(l, v);
          test_bitr(l, v);
        }
  }
  // == <=>
  {
    l_t l0 = {};
    l_t l = {1, 2, 3};
    l_t ll = {1, 3, 2};
    l_t lll = {3, 2, 1, 0};
    assert(l0 < l && l < ll && l < lll);
    assert(l0 != l && l != ll && ll != lll && l != lll);
    assert(l0 == l0 && l == l && ll == ll && lll == lll);
  }
  assert(t::count() == 0);
}

void test_ez_map() {
  {
    using map_t = ez_map<int, int>;
    static_assert(semiregular<map_t>);
    static_assert(!is_nothrow_default_constructible_v<map_t>);
    static_assert(!is_nothrow_copy_constructible_v<map_t>);
    static_assert(!is_nothrow_copy_assignable_v<map_t>);
    static_assert(is_nothrow_move_constructible_v<map_t>);
    static_assert(is_nothrow_move_assignable_v<map_t>);
    static_assert(!noexcept(declval<map_t &>()[1]));
    static_assert(!noexcept(declval<map_t &>().find(1)));
    static_assert(noexcept(declval<map_t &>().end()));
    static_assert(noexcept(declval<map_t &>()
                           .erase(declval<pair<int, int> *>())));
    static_assert(!noexcept(declval<map_t &>().remove(1)));
    static_assert(noexcept(declval<map_t &>().empty()));
    static_assert(noexcept(declval<map_t &>().size()));
    static_assert(noexcept(declval<map_t &>().clear()));
  }

  using t = instance_counter<int>;
  assert(t::count() == 0);
  // brief
  {
    ez_map<t, t> m;

    m[1] = 2;
    assert(!m.empty());
    assert(m[1] == 2);
    assert(m.size() == 1);

    m[2] = 3;
    assert(!m.empty());
    assert(m[1] == 2);
    assert(m[2] == 3);
    assert(m.find(2) != m.end());
    assert(m.find(2)->second == 3);
    assert(m.size() == 2);

    m[2] = 3;
    m[2] = 4;
    assert(!m.empty());
    assert(m[2] == 4);
    assert(m.size() == 2);
  }
  assert(t::count() == 0);
  // brief
  {
    ez_map<t, t> m;
    for (int i = 0; i < 100; ++i)
      m[i] = i;
    for (int i = 0; i < 100; ++i) {
      assert(m[i] == i);
      auto ii = m.find(i);
      assert(ii != m.end());
      assert(ii->first == i);
      assert(ii->second == i);
    }
    assert(!m.empty());
    assert(m.size() == 100);
    m.clear();
    assert(m.empty());
    assert(m.size() == 0);
    for (int i = 100; i != 0;) {
      --i;
      m[i] = i;
    }
    assert(!m.empty());
    assert(m.size() == 100);
    for (int i = 0; i < 100; ++i) {
      assert(m[i] == i);
      auto ii = m.find(i);
      assert(ii != m.end());
      assert(ii->first == i);
      assert(ii->second == i);
    }
    for (int i = 0; i < 100; ++i)
      assert(m.remove(i));
    assert(m.empty());
    assert(m.size() == 0);
  }
  assert(t::count() == 0);
  // insert / erase / copy constructor
  {
    constexpr size_t n = 7;
    size_t a[n];
    for (int i = 0; i != n; ++i)
      a[i] = 0;

    while (a[0] != 1) {
      ez_map<t, t> m;

      ez_vector<int> v;
      for (int i = 0; i != n; ++i)
        v.insert(v.end(), i);

      for (int i = n; i != 0;) {
        --i;
        const auto p = v.begin() + a[i];
        m[*p] = *p + 1;
        v.erase(p);
      }

      assert(t::count() == n * 2);
      assert(!m.empty());
      assert(m.size() == n);
      for (int i = 0; i != n; ++i) {
        auto ii = m.find(i);
        assert(ii != m.end());
        assert(ii->first == i);
        assert(ii->second == i + 1);
        assert(m[i] == i + 1);
      }
      assert(t::count() == n * 2);

      ez_map<t, t> mm = m;
      assert(t::count() == n * 4);
      assert(!m.empty());
      assert(m.size() == n);
      assert(!mm.empty());
      assert(mm.size() == n);
      for (int i = 0; i != n; ++i) {
        auto ii = m.find(i);
        assert(ii != m.end());
        assert(ii->first == i);
        assert(ii->second == i + 1);
        assert(m[i] == i + 1);
        assert(mm[i] == i + 1);
      }
      assert(t::count() == n * 4);

      {
        size_t aa[n];
        for (int i = 0; i != n; ++i)
          aa[i] = 0;

        while (aa[0] != 1) {
          ez_map<t, t> mx = m;

          ez_vector<int> vv;
          for (int i = 0; i != n; ++i)
            vv.insert(vv.end(), i);

          for (int i = n; i != 0;) {
            --i;
            const auto p = vv.begin() + aa[i];
            const auto it = mx.find(*p);
            assert(it != mx.end());
            assert(it->first == *p);
            assert(it->second == *p + 1);
            assert(mx.remove(*p));
            assert(mx.find(*p) == mx.end());
            vv.erase(p);
          }
          assert(mx.empty());
          assert(mx.size() == 0);

          for (int i = n; i != 0;) {
            --i;
            ++aa[i];
            if (aa[i] != to_unsigned(i + 1))
              break;
            else {
              if (i == 0)
                break;
              else
                aa[i] = 0;
            }
          }
        }
      }

      for (int i = n; i != 0;) {
        --i;
        ++a[i];
        if (a[i] != to_unsigned(i + 1))
          break;
        else {
          if (i == 0)
            break;
          else
            a[i] = 0;
        }
      }
    }

    {
      ez_map<t, t> m;
      assert(m.empty());
      assert(m.size() == 0);

      ez_map<t, t> mm(m);
      assert(m.empty());
      assert(m.size() == 0);
      assert(mm.empty());
      assert(mm.size() == 0);
    }
  }
  assert(t::count() == 0);
  // move constructor / move assignment operator
  {
    ez_map<t, t> m;
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    {
      assert(!m.empty());
      assert(m.size() == 3);
      assert(m[1] == 1);
      assert(m[2] == 2);
      assert(m[3] == 3);
      assert(t::count() == 6);
    }
    ez_map<t, t> mm = move(m);
    {
      assert(m.empty());
      assert(m.size() == 0);
      assert(t::count() == 6);

      assert(mm[1] == 1);
      assert(mm[2] == 2);
      assert(mm[3] == 3);
      assert(!mm.empty());
      assert(mm.size() == 3);
      assert(t::count() == 6);
    }
    
    mm = move(mm);
    {
      assert(!mm.empty());
      assert(mm.size() == 3);
      assert(mm[1] == 1);
      assert(mm[2] == 2);
      assert(mm[3] == 3);
      assert(!mm.empty());
      assert(mm.size() == 3);
      assert(t::count() == 6);
    }
    m = move(mm);
    {
      assert(!m.empty());
      assert(m.size() == 3);
      assert(m[1] == 1);
      assert(m[2] == 2);
      assert(m[3] == 3);
      assert(!m.empty());
      assert(m.size() == 3);
      assert(t::count() == 6);

      assert(mm.empty());
      assert(mm.size() == 0);
      assert(t::count() == 6);
    }
  }
  assert(t::count() == 0);
  // copy assignment operator / swap
  {
    ez_function<ez_map<t, t> ()> mk1 = []() {return ez_map<t, t>{};};
    ez_function<bool (ez_map<t, t> &)> eq1 = [](auto &m) {
      return m.empty() && m.size() == 0;
    };
    ez_function<ez_map<t, t> ()> mk2 = []() {
      ez_map<t, t> m;
      m[1] = 1;
      m[2] = 2;
      return m;
    };
    ez_function<bool (ez_map<t, t> &)> eq2 = [](auto &m) {
      return !m.empty() && m.size() == 2
        && m.find(1) != m.end()
        && m.find(1)->second == 1
        && m.find(2) != m.end()
        && m.find(2)->second == 2;
    };
    ez_function<ez_map<t, t> ()> mk3 = []() {
      ez_map<t, t> m;
      m[3] = 4;
      m[4] = 5;
      m[5] = 6;
      return m;
    };
    ez_function<bool (ez_map<t, t> &)> eq3 = [](auto &m) {
      return !m.empty() && m.size() == 3
        && m.find(3) != m.end()
        && m.find(3)->second == 4
        && m.find(4) != m.end()
        && m.find(4)->second == 5
        && m.find(5) != m.end()
        && m.find(5)->second == 6;
    };
        
    const auto f = [=](auto f1, auto eq1, auto f2, auto eq2) {
      ez_map<t, t> m1 = f1();
      ez_map<t, t> m2 = f2();
      assert(eq1(m1));
      assert(eq2(m2));
      adl_swap(m1, m2);
      assert(eq2(m1));
      assert(eq1(m2));
      m1 = m2;
      assert(eq1(m1));
      assert(eq1(m2));
    };
    f(mk1, eq1, mk1, eq1);
    f(mk1, eq1, mk2, eq2);
    f(mk2, eq2, mk1, eq1);
    f(mk2, eq2, mk3, eq3);
  }
  assert(t::count() == 0);
}

void test_test_allocator() {
  // inner::alloc_log
  {
    {
      int *p = nullptr;
      int x = {};
      int *pp = addressof(x);

      inner::alloc_log<ez_map> l;
      assert(l.size() == 0);
      l.allocate(p, 1);
      assert(l.size() == 1);
      l.deallocate(p, 1);
      assert(l.size() == 0);
      l.allocate(pp, 2);
      assert(l.size() == 1);
      l.deallocate(pp, 2);
      assert(l.size() == 0);
      assert(l.empty());
    }
  }
  // test_allocator
  {
    test_allocator<int> a;
    auto p1 = a.allocate(0);
    auto p2 = a.allocate(0);
    auto p3 = a.allocate(0);
    a.deallocate(p1, 0);
    a.deallocate(p2, 0);
    a.deallocate(p3, 0);
  }
  {
    using alloc_t = test_allocator<int>;
    using alloc2_t = test_allocator<long>;
    static_assert(same_as<alloc_t::value_type, int>);
    static_assert(same_as<alloc_t::size_type, size_t>);
    static_assert(regular<alloc_t>);
    static_assert(is_nothrow_default_constructible_v<alloc_t>);
    static_assert(is_trivially_copyable_v<alloc_t>);
    static_assert(is_nothrow_swappable_v<alloc_t>);
    static_assert(noexcept(declval<alloc_t &>() == declval<alloc_t &>()));
    static_assert(is_convertible_v<alloc_t, alloc2_t>);
    static_assert(is_nothrow_convertible_v<alloc_t, alloc2_t>);
    static_assert(!noexcept(declval<alloc_t &>().allocate(1)));
    static_assert(noexcept(declval<alloc_t &>().deallocate(nullptr, 1)));
    static_assert(noexcept(declval<alloc_t &>().empty()));
  }
  {
    test_allocator<int> a;
    int *const p = a.allocate(1);
    ::new(p) int(1);
    assert(*p == 1);
    a.deallocate(p, 1);
    assert(a.empty());
    assert(a.size() == 0);

    test_allocator<int> aa = a;
    assert(a == a);
    assert(!(a != a));
    assert(a == aa);
    assert(!(a != aa));
    assert(aa == aa);
    assert(!(aa != aa));
  }
  // stateful_test_allocator
  {
    using alloc_t = stateful_test_allocator<int>;
    using alloc2_t = stateful_test_allocator<long>;
    static_assert(same_as<alloc_t::value_type, int>);
    static_assert(same_as<alloc_t::size_type, size_t>);
    static_assert(regular<alloc_t>);
    static_assert(!is_nothrow_default_constructible_v<alloc_t>);
    static_assert(is_nothrow_copy_constructible_v<alloc_t>);
    static_assert(is_nothrow_copy_assignable_v<alloc_t>);
    static_assert(is_nothrow_move_constructible_v<alloc_t>);
    static_assert(is_nothrow_move_assignable_v<alloc_t>);
    static_assert(is_nothrow_swappable_v<alloc_t>);
    static_assert(noexcept(declval<alloc_t &>() == declval<alloc_t &>()));
    static_assert(is_convertible_v<alloc_t, alloc2_t>);
    static_assert(is_nothrow_convertible_v<alloc_t, alloc2_t>);
    static_assert(!noexcept(declval<alloc_t &>().allocate(1)));
    static_assert(noexcept(declval<alloc_t &>().deallocate(nullptr, 1)));
    static_assert(noexcept(declval<alloc_t &>().empty()));
  }
  {
    {
      stateful_test_allocator<int> a;
      int *const p = a.allocate(1);
      ::new(p) int(1);
      assert(*p == 1);
      a.deallocate(p, 1);
      assert(a.empty());
      assert(a.size() == 0);
    }
    {
      stateful_test_allocator<int> a;
      assert(a.size() == 0);
      auto p = a.allocate(1);
      assert(a.size() == 1);
      auto aa = a;
      auto aaa = aa;
      auto aaaa = aaa;
      aaaa.deallocate(p, 1);
    }
    {
      stateful_test_allocator<int> a, aa;
      assert(a != aa);
      auto a2 = a;
      auto a3 = a2;
      auto a4 = a3;
      auto a5 = a4;
      assert(a == a5);
      a = aa;
      assert(a == aa);
      assert(a != a2);
    }
    {
      stateful_test_allocator<int> a;
      auto p = a.allocate(1);

      stateful_test_allocator<int> aa;
      adl_swap(a, aa);
      aa.deallocate(p, 1);
    }
  }
}

void test_test_object() {
  {
    using t = test_object<int>;
    static_assert(regular<t>);
    static_assert(!is_nothrow_default_constructible_v<t>);
    static_assert(!is_nothrow_copy_constructible_v<t>);
    static_assert(!is_nothrow_copy_assignable_v<t>);
    static_assert(!is_nothrow_move_constructible_v<t>);
    static_assert(!is_nothrow_move_assignable_v<t>);
    static_assert(is_convertible_v<int, t>);
    static_assert(!is_nothrow_convertible_v<int, t>);
    static_assert(is_assignable_v<t &, const int &>);
    static_assert(!is_nothrow_assignable_v<t &, const int &>);
    static_assert(noexcept(*declval<t &>()));
    static_assert(noexcept(*declval<const t &>()));
    static_assert(noexcept(declval<t &>().operator ->()));
    static_assert(noexcept(declval<const t &>().operator ->()));
    static_assert(is_nothrow_convertible_v<t &, int &>);
    static_assert(is_nothrow_convertible_v<const t &, const int &>);
  }
  {
    using t = test_object<int>;
    test_ownership<t>([](auto p) {::new(p) t(1);},
                      [](auto p) {return *p == 1;},
                      [](auto p) {::new(p) t(2);},
                      [](auto p) {return *p == 2;},
                      [](auto p) {return *p == 0;});
  }
  {
    struct t {
      int value;

      t() = default;
      ~t() = default;
      t(const t &x) : value(x.value) {}
      t &operator =(const t &x) {value = x.value; return *this;}
      t(t &&x) : value(x.value) {x.value = 0;}
      t &operator =(t &&x) {
        if (addressof(x) != this) {
          value = x.value;
          x.value = 0;
        }
        return *this;
      }

      t(int x) : value(x) {}

      bool operator ==(const t &y) {return value == y.value;}
    };
    using tt = test_object<t>;
    test_ownership<tt>([](auto p) {::new(p) tt(1);},
                       [](auto p) {return *p == t(1);},
                       [](auto p) {::new(p) tt(2);},
                       [](auto p) {return *p == t(2);},
                       [](auto p) {return *p == t(0);});
  }
  {
    struct t {
      explicit t(int) {}
    };
    static_assert(!is_convertible_v<int, t>);
    static_assert(is_constructible_v<test_object<t>, int>);
    static_assert(!is_convertible_v<int, test_object<t>>);
    static_assert(!is_assignable_v<test_object<t> &, int>);

    test_object<int> x = 1;
    assert(x == 1);
    (x = 2) = 3;
    assert(x == 3);
    assert(*x.operator ->() == 3);
    assert(*as_const(x).operator ->() == 3);
    assert(*x == 3);
    assert(*as_const(x) == 3);
    int &y = x;
    y = 4;
    assert(x == 4);
    assert(y == 4);
    const int &z = as_const(x);
    assert(z == 4);
    assert(addressof(z) == to_address(x));
  }
}

void test_test_range() {
  // inner::fns::range_empty
  {
    ez_vector v0 = {1, 2};
    assert(!inner::fns::range_empty(v0));
    struct t {
      ez_vector<int> v = {};
      auto begin() const {
        return v.begin();
      }
      auto end() const {
        return v.end();
      }
    };
    t x{};
    assert(inner::fns::range_empty(x));
  }
  // inner::fns::range_size
  {
    ez_vector<int> v0 = {}, v1 = {1, 2, 3};
    assert(inner::fns::range_size(v0) == 0);
    assert(inner::fns::range_size(v1) == 3);
    struct t {
      ez_vector<int> v = {1, 2, 3};
      auto begin() const {
        return v.begin();
      }
      auto end() const {
        return v.end();
      }
    };
    t x{};
    assert(inner::fns::range_size(x) == 3);
  }

  // test_{i,f,b,r}itr
  // test_rng
  {
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    test_fitr(a, a);
    test_fitr(a, b);
    test_fitr(a, a + 3, a);
    test_fitr(a, a + 3, b);
    test_fitr(a, a + 3, b, equal_to<>{});
    test_bitr(a, a);
    test_bitr(a, b);
    test_bitr(a, a + 3, a);
    test_bitr(a, a + 3, b);
    test_bitr(a, a + 3, b, equal_to<>{});
    test_ritr(a, a);
    test_ritr(a, b);
    test_ritr(a, a + 3, a);
    test_ritr(a, a + 3, b);
    test_ritr(a, a + 3, b, equal_to<>{});
    constexpr auto f = []() constexpr {
      int a[] = {1, 2, 3, 4, 5};
      test_fitr(a, a);
      test_bitr(a, a);
      test_ritr(a, a);
      test_fitr(a, a + 5, a);
      test_bitr(a, a + 5, a);
      test_ritr(a, a + 5, a);
      return true;
    };
    static_assert(f());

    ez_vector<int> aa{};
    const ez_vector<int> bb{};
    test_fitr(aa.begin(), aa.end(), bb);
    test_bitr(aa.begin(), aa.end(), bb);
    test_ritr(aa.begin(), aa.end(), bb);
    test_rng(aa, bb);
    test_rng(aa, bb, equal_to<>{});
  }

  // test_itr_minus
  {
    int a[] = {1, 2, 3, 4};
    int *const i = a;
    int *const j = a + 4;
    const int *const ci = a;
    const int *const cj = a + 4;
    test_itr_minus(i, j, 4);
    test_itr_minus(ci, j, 4);
    test_itr_minus(i, cj, 4);
    test_itr_minus(ci, cj, 4);
    test_itr_minus(i, j);
    test_itr_minus(ci, j);
    test_itr_minus(i, cj);
    test_itr_minus(ci, cj);
    test_itr_minus(i, i, 0);
    test_rng(a, ez_vector{1, 2, 3, 4});

    test_itr_minus(ez_vector<int>{}, 0);
    test_itr_minus(ez_vector<int>{});
    test_itr_minus(ez_vector{1, 2, 3}, 3);
    test_itr_minus(ez_vector{1, 2, 3});
  }
}

void test_test() {
  printf("test: ");

  inner::fns::test_owner_ptr();
  inner::fns::test_test_throwing();
  inner::fns::test_test_ownership();
  inner::fns::test_rational_operator_test();
  inner::fns::test_instance_counter();
  inner::fns::test_ez_function();
  inner::fns::test_ez_vector();
  inner::fns::test_ez_forward_list();
  inner::fns::test_ez_bidirectional_list();
  inner::fns::test_ez_list();
  inner::fns::test_ez_map();
  inner::fns::test_test_allocator();
  inner::fns::test_test_object();
  inner::fns::test_test_range();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_test();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
