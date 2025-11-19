//#define RE_NOEXCEPT
#define RE_DEFAULT_ALLOCATOR re::test_allocator
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>
#include <re/io.h>
#include <re/time.h>
#include <re/concurrency.h>

#include <cassert>

namespace re::inner::test {

template <class LL, class SZT>
struct test_llong_t {
  static void f() {}
};
template <class LL, class SZT>
requires (sizeof(SZT) == 8u)
struct test_llong_t<LL, SZT> {
  static void f() {
    using llong = LL;

    atomic<llong> a[1000];
    assert(all_of_equal(a, 0));
    atomic<llong> x = 1;
    volatile atomic<llong> vx = 10;

    assert((x = 2) == 2);
    assert((vx = 20) == 20);
    assert(x.operator llong() == 2);
    assert(vx.operator llong() == 20);

    assert(x.load() == 2);
    assert(vx.load() == 20);
    assert(x.exchange(3) == 2);
    assert(vx.exchange(30) == 20);
    assert(x.load() == 3);
    assert(vx.load() == 30);
    x.store(4);
    vx.store(40);
    assert(x.load() == 4);
    assert(vx.load() == 40);

    llong tmp = 3;
    assert(!x.compare_exchange(tmp, 5));
    assert(x == 4 && tmp == 4);
    tmp = 30;
    assert(!vx.compare_exchange(tmp, 50));
    assert(vx == 40 && tmp == 40);
    tmp = 4;
    assert(x.compare_exchange(tmp, 5));
    assert(x == 5 && tmp == 4);
    tmp = 40;
    assert(vx.compare_exchange(tmp, 50));
    assert(vx == 50 && tmp == 40);

    x = 4;
    vx = 40;
    assert(x.fetch_add(1) == 4);
    assert(vx.fetch_add(10) == 40);
    assert(x == 5 && vx == 50);

    assert(x.fetch_sub(1) == 5);
    assert(vx.fetch_sub(10) == 50);
    assert(x == 4 && vx == 40);

    assert(++x == 5);
    assert(++vx == 41);
    assert(x == 5 && vx == 41);
    assert(x++ == 5);
    assert(vx++ == 41);
    assert(x == 6 && vx == 42);

    assert(--x == 5);
    assert(--vx == 41);
    assert(x == 5 && vx == 41);
    assert(x-- == 5);
    assert(vx-- == 41);
    assert(x == 4 && vx == 40);

    assert((x += 1) == 5);
    assert(x == 5);
    assert((vx += 10) == 50);
    assert(vx == 50);

    assert((x -= 1) == 4);
    assert(x == 4);
    assert((vx -= 10) == 40);
    assert(vx == 40);

    x = 0b1111;
    vx = 0b11111;
    assert((x &= 0b1001) == 0b1001);
    assert(x == 0b1001);
    assert((vx &= 0b10001) == 0b10001);
    assert(vx == 0b10001);

    assert((x |= 0b0110) == 0b1111);
    assert(x == 0b1111);
    assert((vx |= 0b01110) == 0b11111);
    assert(vx == 0b11111);

    assert((x ^= (llong)(((make_unsigned<llong>)-1) << 4u)) == (llong)-1);
    assert(x == (int)-1);
    assert((vx ^= (llong)(((make_unsigned<llong>)-1) << 5u)) == (llong)-1);
    assert(vx == (int)-1);
  }
};

void test_atomic_llong() {
  test_llong_t<long long, size_t>::f();
}
void test_atomic() {
  // atomic<int>
  {
    atomic<int> a[1000];
    assert(all_of_equal(a, 0));
    atomic<int> x = 1;
    volatile atomic<int> vx = 10;

    assert((x = 2) == 2);
    assert((vx = 20) == 20);
    assert(x.operator int() == 2);
    assert(vx.operator int() == 20);

    assert(x.load() == 2);
    assert(vx.load() == 20);
    assert(x.exchange(3) == 2);
    assert(vx.exchange(30) == 20);
    assert(x.load() == 3);
    assert(vx.load() == 30);
    x.store(4);
    vx.store(40);
    assert(x.load() == 4);
    assert(vx.load() == 40);

    int tmp = 3;
    assert(!x.compare_exchange(tmp, 5));
    assert(x == 4 && tmp == 4);
    tmp = 30;
    assert(!vx.compare_exchange(tmp, 50));
    assert(vx == 40 && tmp == 40);
    tmp = 4;
    assert(x.compare_exchange(tmp, 5));
    assert(x == 5 && tmp == 4);
    tmp = 40;
    assert(vx.compare_exchange(tmp, 50));
    assert(vx == 50 && tmp == 40);

    x = 4;
    vx = 40;
    assert(x.fetch_add(1) == 4);
    assert(vx.fetch_add(10) == 40);
    assert(x == 5 && vx == 50);

    assert(x.fetch_sub(1) == 5);
    assert(vx.fetch_sub(10) == 50);
    assert(x == 4 && vx == 40);

    assert(++x == 5);
    assert(++vx == 41);
    assert(x == 5 && vx == 41);
    assert(x++ == 5);
    assert(vx++ == 41);
    assert(x == 6 && vx == 42);

    assert(--x == 5);
    assert(--vx == 41);
    assert(x == 5 && vx == 41);
    assert(x-- == 5);
    assert(vx-- == 41);
    assert(x == 4 && vx == 40);

    assert((x += 1) == 5);
    assert(x == 5);
    assert((vx += 10) == 50);
    assert(vx == 50);

    assert((x -= 1) == 4);
    assert(x == 4);
    assert((vx -= 10) == 40);
    assert(vx == 40);

    x = 0b1111;
    vx = 0b11111;
    assert((x &= 0b1001) == 0b1001);
    assert(x == 0b1001);
    assert((vx &= 0b10001) == 0b10001);
    assert(vx == 0b10001);

    assert((x |= 0b0110) == 0b1111);
    assert(x == 0b1111);
    assert((vx |= 0b01110) == 0b11111);
    assert(vx == 0b11111);

    assert((x ^= 0b11111111111111111111111111110000) == (int)-1);
    assert(x == (int)-1);
    assert((vx ^= 0b11111111111111111111111111100000) == (int)-1);
    assert(vx == (int)-1);
  }

  // atomic<long>
  {
    atomic<long> a[1000];
    assert(all_of_equal(a, 0));
    atomic<long> x = 1;
    volatile atomic<long> vx = 10;

    assert((x = 2) == 2);
    assert((vx = 20) == 20);
    assert(x.operator long() == 2);
    assert(vx.operator long() == 20);

    assert(x.load() == 2);
    assert(vx.load() == 20);
    assert(x.exchange(3) == 2);
    assert(vx.exchange(30) == 20);
    assert(x.load() == 3);
    assert(vx.load() == 30);
    x.store(4);
    vx.store(40);
    assert(x.load() == 4);
    assert(vx.load() == 40);

    long tmp = 3;
    assert(!x.compare_exchange(tmp, 5));
    assert(x == 4 && tmp == 4);
    tmp = 30;
    assert(!vx.compare_exchange(tmp, 50));
    assert(vx == 40 && tmp == 40);
    tmp = 4;
    assert(x.compare_exchange(tmp, 5));
    assert(x == 5 && tmp == 4);
    tmp = 40;
    assert(vx.compare_exchange(tmp, 50));
    assert(vx == 50 && tmp == 40);

    x = 4;
    vx = 40;
    assert(x.fetch_add(1) == 4);
    assert(vx.fetch_add(10) == 40);
    assert(x == 5 && vx == 50);

    assert(x.fetch_sub(1) == 5);
    assert(vx.fetch_sub(10) == 50);
    assert(x == 4 && vx == 40);

    assert(++x == 5);
    assert(++vx == 41);
    assert(x == 5 && vx == 41);
    assert(x++ == 5);
    assert(vx++ == 41);
    assert(x == 6 && vx == 42);

    assert(--x == 5);
    assert(--vx == 41);
    assert(x == 5 && vx == 41);
    assert(x-- == 5);
    assert(vx-- == 41);
    assert(x == 4 && vx == 40);

    assert((x += 1) == 5);
    assert(x == 5);
    assert((vx += 10) == 50);
    assert(vx == 50);

    assert((x -= 1) == 4);
    assert(x == 4);
    assert((vx -= 10) == 40);
    assert(vx == 40);

    x = 0b1111;
    vx = 0b11111;
    assert((x &= 0b1001) == 0b1001);
    assert(x == 0b1001);
    assert((vx &= 0b10001) == 0b10001);
    assert(vx == 0b10001);

    assert((x |= 0b0110) == 0b1111);
    assert(x == 0b1111);
    assert((vx |= 0b01110) == 0b11111);
    assert(vx == 0b11111);

    assert((x ^= 0b11111111111111111111111111110000) == (int)-1);
    assert(x == (int)-1);
    assert((vx ^= 0b11111111111111111111111111100000) == (int)-1);
    assert(vx == (int)-1);
  }

  // atomic<long long>
  test_atomic_llong();

  // atomic<int *>
  {
    int i = 3;
    int j = 4;

    atomic<int *> x;
    volatile atomic<int *> vx;
    assert(x.load() == nullptr);
    assert(vx.load() == nullptr);
    x.store(&i);
    vx.store(&i);
    assert(x.load() != nullptr && *x.load() == 3);
    assert(vx.load() != nullptr && *vx.load() == 3);
    assert(x.operator int *() != nullptr && *x.operator int *() == 3);
    assert(vx.operator int *() != nullptr && *vx.operator int *() == 3);
    x = &j;
    vx = &j;
    assert(x.operator int *() != nullptr && *x.operator int *() == 4);
    assert(vx.operator int *() != nullptr && *vx.operator int *() == 4);
    assert(*x.exchange(nullptr) == 4);
    assert(*vx.exchange(nullptr) == 4);
    assert(x.load() == nullptr);
    assert(vx.load() == nullptr);

    int *p = nullptr;
    assert(x.compare_exchange(p, p));
    assert(vx.compare_exchange(p, p));
    assert(x.load() == nullptr);
    assert(vx.load() == nullptr);
    x = &j;
    int *pp = &j;
    assert(x.compare_exchange(pp, &i));
    assert(pp != nullptr && *pp == 4);
    assert(x.load() != nullptr && *x.load() == 3);
    assert(!x.compare_exchange(pp, &j));
    assert(pp != nullptr && *pp == 3);
    assert(x.load() != nullptr && *x.load() == 3);

    vx = &j;
    pp = &j;
    assert(vx.compare_exchange(pp, &i));
    assert(pp != nullptr && *pp == 4);
    assert(vx.load() != nullptr && *vx.load() == 3);
    assert(!vx.compare_exchange(pp, &j));
    assert(pp != nullptr && *pp == 3);
    assert(vx.load() != nullptr && *vx.load() == 3);
  }

  // atomic_flag
  {
    atomic_flag a[1000];
    assert(all_of(a, [](auto &x) {return !x.test();}));
    atomic_flag x;
    volatile atomic_flag vx;
    assert(x.test() == false);
    assert(vx.test() == false);

    x.test_and_set();
    vx.test_and_set();
    assert(x.test() == true);
    assert(vx.test() == true);
    assert(x.test_and_set() == true);
    assert(vx.test_and_set() == true);
    assert(x.test() == true);
    assert(vx.test() == true);

    x.clear();
    vx.clear();
    assert(x.test() == false);
    assert(vx.test() == false);
  }
}
void test_shared_ptr() {
  // smf
  {
    using t = test_object<int>;
    // nullptr with no counter
    {
      shared_ptr<t> p;
      shared_ptr<t> pp = p;
      assert(!p && !pp);
      assert(p.use_count() == 0 && pp.use_count() == 0);
      shared_ptr<t> ppp = move(p);
      assert(!p && !ppp);
      assert(p.use_count() == 0 && ppp.use_count() == 0);
    }
    // nullptr with counter
    {
      shared_ptr<t> p(nullptr, default_delete<t>{});
      assert(!p && p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(!p && p.use_count() == 2);
      assert(!pp && p.use_count() == 2);
      shared_ptr<t> ppp = move(p);
      assert(!p && p.use_count() == 0);
      assert(!ppp && ppp.use_count() == 2);
    }
    // valid with counter
    {
      shared_ptr<t> p(new t(1));
      assert(p && *p == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && *p == 1);
      assert(p.use_count() == 2);
      assert(pp && *pp == 1);
      assert(pp.use_count() == 2);
      assert(p.get() == pp.get());
      shared_ptr<t> ppp = move(p);
      assert(ppp && *ppp == 1);
      assert(ppp.use_count() == 2);
      assert(!p && p.use_count() == 0);
      ppp = ppp;
      assert(ppp && *ppp == 1);
      assert(ppp.use_count() == 2);
      ppp = move(ppp);
      assert(ppp && *ppp == 1);
      assert(ppp.use_count() == 2);
      adl_swap(ppp, ppp);
      assert(ppp && *ppp == 1);
      assert(ppp.use_count() == 2);
    }
  }

  // shared_ptr(nullptr)
  // reset()
  {
    using t = test_object<int>;
    shared_ptr<t> p(new t(2));
    p.reset();
    assert(p.get() == nullptr);
    assert(p.use_count() == 0);
    shared_ptr<t> pp = nullptr;
    assert(pp.get() == nullptr);
    assert(pp.use_count() == 0);
  }

  // shared_ptr(Y *)
  {
    using t = test_object<int>;
    // non-array enable_shared_shared_from_this
    {
      struct tt : enable_shared_from_this<tt> {
        int value;
        tt(int x) : value(x) {}
      };
      shared_ptr<tt> p(::new tt(1));
      shared_ptr<tt> p2 = p.get()->shared_from_this();
      assert(p && p.get() == p2.get() && p->value == 1);
      assert(p.use_count() == 2);
      assert(p2.use_count() == 2);

      shared_ptr<const tt> ppp(::new const tt(1));
      shared_ptr<const tt> pppp(::new tt(1));
    }
    // non-array
    {
      shared_ptr<t> p(::new t(1));
      assert(p.get() != nullptr && *p.get() == 1);
      assert(p.operator ->() != nullptr);
      assert(addressof(p.operator *()) == p.get());
      assert(p.use_count() == 1);

      shared_ptr<t> pp = p;
      assert(p != nullptr && *p == 1 && p.use_count() == 2);
      assert(pp != nullptr && *pp == 1 && pp.use_count() == 2);
      assert(p.get() == pp.get());
    }
    // bounded array
    {
      shared_ptr<t [3]> p(::new t [3]{1, 2, 3});
      assert(p.get() != nullptr);
      assert(equal(rng(p.get(), 3), irng(1, 4)));
      assert(p[0] == 1 && p[1] == 2 && p[2] == 3);
      assert(p.use_count() == 1);

      shared_ptr<t [3]> pp = p;
      assert(p != nullptr && p[0] == 1 && p[1] == 2 && p[2] == 3);
      assert(p.use_count() == 2);
      assert(pp != nullptr && pp[0] == 1 && pp[1] == 2 && pp[2] == 3);
      assert(pp.use_count() == 2);
      assert(p.get() == pp.get());
    }
    // unbounded array
    {
      shared_ptr<t []> p(::new t [3]{1, 2, 3});
      assert(p.get() != nullptr);
      assert(equal(rng(p.get(), 3), irng(1, 4)));

      shared_ptr<t []> pp = p;
      assert(p != nullptr && p[0] == 1 && p[1] == 2 && p[2] == 3);
      assert(p.use_count() == 2);
      assert(pp != nullptr && pp[0] == 1 && pp[1] == 2 && pp[2] == 3);
      assert(pp.use_count() == 2);
      assert(p.get() == pp.get());
    }
  }
  // reset(Y *)
  {
    using t = test_object<int>;
    shared_ptr<t> p;
    p.reset(new t(2));
    assert(p != nullptr);
    assert(*p == 2);
    assert(p.use_count() == 1);
  }

  // shared_ptr(Y *, D)
  {
    struct t {
      virtual int f() {return 0;}
    };
    struct tt : t {
      int value;
      tt() = default;
      tt(int x) : value(x) {}
      virtual int f() override {return value;}
    };
    // non-array enable_shared_shared_from_this
    {
      struct t2 : enable_shared_from_this<t2> {
        int value;
        t2(int x) : value(x) {}
      };
      shared_ptr<t2> p(::new t2(1), default_delete<t2>{});
      shared_ptr<t2> p2 = p.get()->shared_from_this();
      assert(p && p.get() == p2.get() && p->value == 1);
      assert(p.use_count() == 2);
      assert(p2.use_count() == 2);
    }
    // non-array
    {
      stateful_test_allocator<tt> a;
      shared_ptr<t> p(alloc_wrapper(a).new_1(1),
                      [a](tt *p) {alloc_wrapper(a).delete_1(p);});
      assert(p != nullptr && p->f() == 1);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p->f() == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // bounded array
    {
      stateful_test_allocator<int> a;
      auto p0 = alloc_wrapper(a).rebind<tt>().new_n(2);
      p0[0] = 1;
      p0[1] = 2;
      shared_ptr<tt [2]> p
        (p0,
         [a](tt *ptr) {alloc_wrapper(a).rebind<tt>().delete_n(ptr, 2);});
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // unbounded array
    {
      stateful_test_allocator<int> a;
      auto p0 = alloc_wrapper(a).rebind<tt>().new_n(2);
      p0[0] = 1;
      p0[1] = 2;
      shared_ptr<tt []> p
        (p0,
         [a](tt *ptr) {alloc_wrapper(a).rebind<tt>().delete_n(ptr, 2);});
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // shared_ptr(nullptr, D)
  {
    using t = test_object<int>;
    shared_ptr<t> p(nullptr, default_delete<t>{});
    assert(p == nullptr && p.use_count() == 1);
    auto pp = p;
    assert(p == nullptr && p.use_count() == 2);
    assert(pp == nullptr && pp.use_count() == 2);
  }
  // reset(Y *, D)
  {
    using t = test_object<int>;
    shared_ptr<t> p;
    p.reset(static_cast<t *>(nullptr), default_delete<t>{});
    assert(p == nullptr && p.use_count() == 1);
    auto pp = p;
    assert(p == nullptr && p.use_count() == 2);
    assert(pp == nullptr && pp.use_count() == 2);
  }

  // shared_ptr(Y *, D, A)
  {
    struct t {
      virtual int f() {return 0;}
    };
    struct tt : t {
      int value;
      tt() = default;
      tt(int x) : value(x) {}
      virtual int f() override {return value;}
    };
    // non-array enable_shared_shared_from_this
    {
      struct t2 : enable_shared_from_this<t2> {
        virtual ~t2() {}
      };
      struct t3 : t2 {
        int value;
        t3(int x) : value(x) {}
        ~t3() = default;
      };
      shared_ptr<t2> p(::new t3(1), default_delete<t3>{},
                       stateful_test_allocator<t2>{});
      shared_ptr<t2> p2 = p.get()->shared_from_this();
      assert(p && p.get() == p2.get()
             && (dynamic_cast<t3 *>(p.get()) != nullptr
                 && dynamic_cast<t3 *>(p.get())->value == 1));
      assert(p.use_count() == 2);
      assert(p2.use_count() == 2);
    }
    // non-array
    {
      stateful_test_allocator<tt> a;
      shared_ptr<t> p(alloc_wrapper(a).new_1(1),
                      [a](tt *p) {alloc_wrapper(a).delete_1(p);},
                      stateful_test_allocator<int>{});
      assert(p != nullptr && p->f() == 1);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p->f() == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // bounded array
    {
      stateful_test_allocator<int> a;
      auto p0 = alloc_wrapper(a).rebind<tt>().new_n(2);
      p0[0] = 1;
      p0[1] = 2;
      shared_ptr<tt [2]> p
        (p0,
         [a](tt *ptr) {alloc_wrapper(a).rebind<tt>().delete_n(ptr, 2);},
         a);
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // unbounded array
    {
      stateful_test_allocator<int> a;
      auto p0 = alloc_wrapper(a).rebind<tt>().new_n(2);
      p0[0] = 1;
      p0[1] = 2;
      shared_ptr<tt []> p
        (p0,
         [a](tt *ptr) {alloc_wrapper(a).rebind<tt>().delete_n(ptr, 2);},
         a);
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 1);

      auto pp = p;
      assert(p != nullptr && p[0].f() == 1 && p[1].f() == 2);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // shared_ptr(nullptr, D, A)
  {
    using t = test_object<int>;
    shared_ptr<t> p(nullptr, default_delete<t>{},
                    stateful_test_allocator<int>{});
    assert(p.get() == nullptr);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p == nullptr && p.use_count() == 2);
    assert(pp == nullptr && pp.use_count() == 2);
  }
  // reset(Y *, D, A)
  {
    using t = test_object<int>;
    shared_ptr<t> p;
    p.reset(static_cast<t *>(nullptr), default_delete<t>{},
            stateful_test_allocator<int>{});
    assert(p == nullptr && p.use_count() == 1);
    auto pp = p;
    assert(p == nullptr && p.use_count() == 2);
    assert(pp == nullptr && pp.use_count() == 2);
  }

  // shared_ptr(const shared_ptr<Y> &)
  // =(const shared_ptr<Y> &)
  {
    struct t {
      virtual ~t() {}
      virtual int f() {return 1;}
    };
    struct tt : t {
      virtual int f() override {return 2;}
    };
    {
      shared_ptr<tt> p2(new tt);
      shared_ptr<t> p(p2);
      assert(p.use_count() == 2 && p2.use_count() == 2);
      assert(p.get() != nullptr && p.get() == static_cast<t *>(p2.get()));
    }
    {
      shared_ptr<tt> p2;
      shared_ptr<t> p(p2);
      assert(p.use_count() == 0 && p2.use_count() == 0);
      assert(p.get() == nullptr && p2.get() == nullptr);
    }
    {
      shared_ptr<t> p(new t);
      shared_ptr<tt> p2(new tt);
      p = p2;
      assert(p.use_count() == 2 && p2.use_count() == 2);
      assert(p.get() != nullptr && p.get() == static_cast<t *>(p2.get()));
    }
  }

  // shared_ptr(const shared_ptr<Y> &, element_type *)
  {
    shared_ptr<int> p(new int(1));
    shared_ptr<void *> pp(p, nullptr);
    assert(pp.get() == nullptr && pp.use_count() == 2);
    assert(p.use_count() == 2);
    p.reset();
    assert(pp.use_count() == 1);
  }
  // shared_ptr(shared_ptr<Y> &&, element_type *)
  {
    shared_ptr<int> p(new int(1));
    shared_ptr<void *> pp(move(p), nullptr);
    assert(pp.get() == nullptr && pp.use_count() == 1);
    assert(!p && p.use_count() == 0);
  }

  // shared_ptr(unique_ptr<Y, D> &&)
  // shared_ptr &operator =(unique_ptr<Y, D> &&)
  {
    // no enable shared_from_this
    {
      using t = test_object<int>;
      unique_ptr<t> p = make_unique<t>(1);
      shared_ptr<t> p2 = move(p);
      assert(p == nullptr);
      assert(p2 && *p2 == 1);
      assert(p2.use_count() == 1);
    }
    // enable shared_from_this
    {
      struct t : enable_shared_from_this<t> {
        test_object<int> value = 1;
      };
      unique_ptr<t> p = make_unique<t>();
      shared_ptr<t> p2 = move(p);
      assert(p == nullptr);
      assert(p2 && p2->value == 1);
      assert(p2.use_count() == 1);
      shared_ptr<t> p3 = p2->shared_from_this();
      assert(p == nullptr);
      assert(p2 && p2->value == 1);
      assert(p2.use_count() == 2);
      assert(p3.get() == p2.get());
    }
  }

  // explicit shared_ptr(const weak_ptr<Y> &)
  {
    using t = test_object<int>;
    weak_ptr<t> p;
#ifndef RE_NOEXCEPT
    bool y = false;
    try {
      shared_ptr<t> p2(p);
    }
    catch (const bad_weak_ptr &) {
      y = true;
    }
    assert(y);
#endif

    shared_ptr<t> p2 = make_shared<t>(1);
    p = p2;
    shared_ptr<t> p3(p);
    assert(p3.get() == p2.get());
    assert(p3.use_count() == 2);
    assert(p2.use_count() == 2);
    assert(*p3 == 1);
  }

  // get()
  // * -> []
  // use_count()
  // explicit bool()

  // owner_before(const shared_ptr<U> &)
  // owner_before(const weak_ptr<U> &)
  {
    weak_ptr<int> wp;
    shared_ptr<int> p;
    shared_ptr<int> p2(new int(1));
    assert(!p.owner_before(wp) && !wp.owner_before(p));
    assert(p.owner_before(p2) && !p2.owner_before(p));
  }

  // ==(const shared_ptr<T> &, const shared_ptr<U> &)
  {
    shared_ptr<int> p, p2;
    assert(p == p2);
    p2.reset(new int(1));
    assert(!(p == p2));
  }
  // ==(const shared_ptr<T> &, nullptr_t)
  {
    shared_ptr<int> p;
    assert(p == nullptr);
    p.reset(new int(1));
    assert(!(p == nullptr));
  }
  // <=>(const shared_ptr<T> &, const shared_ptr<U> &)
  {
    shared_ptr<int> p;
    shared_ptr<int> p2(new int(1));
    assert((p <=> p2) == strong_lt);
  }
  // <=>(const shared_ptr<T> &, nullptr_t)
  {
    shared_ptr<int> p;
    assert((p <=> nullptr) == strong_eq);
    p.reset(new int(1));
    assert((p <=> nullptr) == strong_gt);
  }

  // get_deleter<D>(const shared_ptr<T> &)
  {
    shared_ptr<int> p(nullptr, default_delete<int>{});
    default_delete<int> *dp = get_deleter<default_delete<int>>(p);
    assert(dp != nullptr);
    default_delete<long> *dp2 = get_deleter<default_delete<long>>(p);
    assert(dp2 == nullptr);
  }

  // hash<shared_ptr<T>>
  {
    shared_ptr<int> p;
    assert(hash<shared_ptr<int>>{}(p) == hash<int *>{}(nullptr));
  }

  // make_shared<T>
  {
    // non-const
    {
      using t = test_object<int>;
      shared_ptr<t> p = make_shared<t>(1);
      assert(p && *p == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && *p == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // const
    {
      using t = const test_object<int>;
      shared_ptr<t> p = make_shared<t>(1);
      assert(p && *p == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && *p == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // make_shared<T [N]>(const T &)
  {
    // single demension, non-const
    {
      using t = test_object<int> [2];
      shared_ptr<t> p = make_shared<t>(test_object<int>(1));
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // single demension, const
    {
      using t = const test_object<int> [2];
      shared_ptr<t> p = make_shared<t>(test_object<int>(1));
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 2-demension, non-const
    {
      using t = test_object<int> [2][2];
      test_object<int> a[2] = {test_object<int>(1), test_object<int>(2)};
      shared_ptr<t> p = make_shared<t>(a);
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 2-demension, const
    {
      using t = const test_object<int> [2][2];
      shared_ptr<t> p
        = make_shared<t>({test_object<int>(1), test_object<int>(2)});
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [2][2][2];
      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      shared_ptr<t> p = make_shared<t>(a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 3-demension, const
    {
      using t = const test_object<int> [2][2][2];
      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      shared_ptr<t> p = make_shared<t>(a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // make_shared<T [N]>()
  {
    // single demension, non-const
    {
      using t = test_object<int> [2];
      shared_ptr<t> p = make_shared<t>();
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // single demension, const
    {
      using t = const test_object<int> [2];
      shared_ptr<t> p = make_shared<t>();
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [2][2][2];
      shared_ptr<t> p = make_shared<t>();
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 3-demension, const
    {
      using t = const test_object<int> [2][2][2];
      shared_ptr<t> p = make_shared<t>();
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // make_shared<T []>
  {
    // single demension, non-const
    {
      using t = test_object<int>;
      shared_ptr<t []> p = make_shared<t []>(0);
      assert(p);
      assert(p.use_count() == 1);

      p = make_shared<t []>(3);
      assert(p);
      assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      p = make_shared<t []>(3, 1);
      assert(p);
      assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }
    // single demension, const
    {
      using t = const test_object<int>;
      shared_ptr<t []> p = make_shared<t []>(0);
      assert(p);
      assert(p.use_count() == 1);

      p = make_shared<t []>(3);
      assert(p);
      assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      p = make_shared<t []>(3, 1);
      assert(p);
      assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [][2][2];

      shared_ptr<t> p = make_shared<t>(0);
      assert(p && p.use_count() == 1);
      p = make_shared<t>(2);
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
               && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      p = make_shared<t>(2, a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
               && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }
    {
      using t = const test_object<int> [][2][2];

      shared_ptr<t> p = make_shared<t>(0);
      assert(p && p.use_count() == 1);
      p = make_shared<t>(2);
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
               && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      p = make_shared<t>(2, a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
               && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }
  }

  // make_shared_for_overwrite<T>
  {
    using t = test_object<int>;
    shared_ptr<t> p = make_shared_for_overwrite<t>();
    assert(p && *p == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && *p == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }
  // make_shared_for_overwrite<T [N]>
  {
    using t = test_object<int>;
    shared_ptr<t [2]> p = make_shared_for_overwrite<t [2]>();
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }
  // make_shared_for_overwrite<T []>
  {
    using t = test_object<int>;
    shared_ptr<t []> p = make_shared_for_overwrite<t []>(0);
    assert(p);
    assert(p.use_count() == 1);
    p = make_shared_for_overwrite<t []>(2);
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }

  // allocate_shared<T>
  {
    // non-const
    {
      using t = test_object<int>;
      using a_t = stateful_test_allocator<int>;
      a_t a;
      shared_ptr<t> p = allocate_shared<t>(a, 1);
      assert(p && *p == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && *p == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // const
    {
      using t = const test_object<int>;
      using a_t = stateful_test_allocator<int>;
      a_t a;
      shared_ptr<t> p = allocate_shared<t>(a, 1);
      assert(p && *p == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && *p == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // for scoped_allocator_adaptor
    {
      using t = vector<test_object<int>,
                       stateful_test_allocator<test_object<int>>>;
      using a_t = scoped_allocator_adaptor<stateful_test_allocator<int>>;
      a_t a;
      shared_ptr<t> p = allocate_shared<t>(a);
      assert(p && p.use_count() == 1);
      assert(p->get_allocator() == a.outer_allocator());
    }
  }
  // allocate_shared<T [N]>(a, u)
  {
    // single demension, non-const
    {
      using t = test_object<int> [2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{}, test_object<int>(1));
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // single demension, const
    {
      using t = const test_object<int> [2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{}, test_object<int>(1));
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 1 && p[1] == 1);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 2-demension, non-const
    {
      using t = test_object<int> [2][2];
      using a_t = stateful_test_allocator<int>;
      test_object<int> a[2] = {test_object<int>(1), test_object<int>(2)};
      shared_ptr<t> p = allocate_shared<t>(a_t{}, a);
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 2-demension, const
    {
      using t = const test_object<int> [2][2];
      using a_t = stateful_test_allocator<int>;
      test_object<int> a[2] = {test_object<int>(1), test_object<int>(2)};
      shared_ptr<t> p = allocate_shared<t>(a_t{}, a);
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && equal(p[0], seq(1, 2)) && equal(p[1], seq(1, 2)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [2][2][2];
      using a_t = stateful_test_allocator<int>;
      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      shared_ptr<t> p = allocate_shared<t>(a_t{}, a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 3-demension, const
    {
      using t = const test_object<int> [2][2][2];
      using a_t = stateful_test_allocator<int>;
      test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      shared_ptr<t> p = allocate_shared<t>(a_t{}, a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // allocate_shared<T [N]>(a)
  {
    // single demension, non-const
    {
      using t = test_object<int> [2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{});
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // single demension, const
    {
      using t = const test_object<int> [2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{});
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p && p[0] == 0 && p[1] == 0);
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [2][2][2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{});
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
    // 3-demension, const
    {
      using t = const test_object<int> [2][2][2];
      using a_t = stateful_test_allocator<int>;
      shared_ptr<t> p = allocate_shared<t>(a_t{});
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      shared_ptr<t> pp = p;
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 2);
      assert(p.get() == pp.get());
      assert(pp.use_count() == 2);
    }
  }
  // allocate_shared<T []>
  {
    // single demension, non-const
    {
      using t = test_object<int>;
      using a_t = stateful_test_allocator<int>;
      a_t a;
      shared_ptr<t []> p = allocate_shared<t []>(a, 0);
      assert(p);
      assert(p.use_count() == 1);

      p = allocate_shared<t []>(a, 3);
      assert(p);
      assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 0 && p[1] == 0 && p[2] == 0);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      p = allocate_shared<t []>(a, 3, 1);
      assert(p);
      assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p[0] == 1 && p[1] == 1 && p[2] == 1);
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }

    // 3-demension, non-const
    {
      using t = test_object<int> [][2][2];
      using alloc_t = stateful_test_allocator<int>;
      alloc_t alloc;
      shared_ptr<t> p = allocate_shared<t>(alloc, 0);
      assert(p && p.use_count() == 1);
      p = allocate_shared<t>(alloc, 2);
      assert(p
             && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
             && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(0, 0)) && equal(p[0][1], seq(0, 0))
               && equal(p[1][0], seq(0, 0)) && equal(p[1][1], seq(0, 0)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }

      const test_object<int> a[2][2] = {
        {test_object<int>(1), test_object<int>(2)},
        {test_object<int>(3), test_object<int>(4)}
      };
      p = allocate_shared<t>(alloc, 2, a);
      assert(p
             && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
             && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
      assert(p.use_count() == 1);
      {
        auto pp = p;
        assert(p
               && equal(p[0][0], seq(1, 2)) && equal(p[0][1], seq(3, 4))
               && equal(p[1][0], seq(1, 2)) && equal(p[1][1], seq(3, 4)));
        assert(p.use_count() == 2);
        assert(p.get() == pp.get());
        assert(pp.use_count() == 2);
      }
    }
  }

  // allocate_shared_for_overwrite<T>
  {
    using t = test_object<int>;
    using a_t = stateful_test_allocator<int>;
    shared_ptr<t> p = allocate_shared_for_overwrite<t>(a_t{});
    assert(p && *p == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && *p == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }
  // allocate_shared_for_overwrite<T [N]>
  {
    using t = test_object<int>;
    using a_t = stateful_test_allocator<int>;
    shared_ptr<t [2]> p = allocate_shared_for_overwrite<t [2]>(a_t{});
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }
  // allocate_shared_for_overwrite<T []>
  {
    using t = test_object<int>;
    using a_t = stateful_test_allocator<int>;
    shared_ptr<t []> p = allocate_shared_for_overwrite<t []>(a_t{}, 0);
    assert(p);
    assert(p.use_count() == 1);
    p = allocate_shared_for_overwrite<t []>(a_t{}, 2);
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 1);
    auto pp = p;
    assert(p && p[0] == 0 && p[1] == 0);
    assert(p.use_count() == 2);
    assert(p.get() == pp.get());
    assert(pp.use_count() == 2);
  }

  // static_pointer_cast
  {
    struct t {
      t() {}
      virtual ~t() {}
      virtual int f() {return 0;}
    };
    struct tt : t {
      test_object<int> value = test_object<int>(1);

      tt() {}
      virtual ~tt() {}
      virtual int f() {return *value;}
    };
    shared_ptr<tt> p = make_shared<tt>();
    shared_ptr<t> p2 = static_pointer_cast<t>(p);
    assert(p2 && p2->f() == 1);
    assert(p2.use_count() == 2);
    assert(p.get() == p2.get());
    shared_ptr<t> p3 = static_pointer_cast<t>(move(p));
    assert(!p && p.use_count() == 0);
    assert(p3 && p3->f() == 1);
    assert(p3.use_count() == 2);
  }
  // reinterpret_pointer_cast
  {
    shared_ptr<byte [4]> p = make_shared<byte [4]>();
    shared_ptr<uint32_t> p2 = reinterpret_pointer_cast<uint32_t>(p);
    shared_ptr<uint32_t> p3 = reinterpret_pointer_cast<uint32_t>(move(p));
    assert(!p && p.use_count() == 0);
    assert(p2 && p2.get() == p3.get());
    assert(p2.use_count() == 2);
    assert(p3.use_count() == 2);
  }
  // const_pointer_cast
  {
    using t = test_object<int>;
    shared_ptr<t> p = make_shared<t>(1);
    shared_ptr<const t> p2 = const_pointer_cast<const t>(p);
    shared_ptr<t> p3 = const_pointer_cast<t>(move(p2));
    assert(!p2 && p2.use_count() == 0);
    assert(p.get() == p3.get());
    assert(p.use_count() == p3.use_count());
    assert(p3 && *p3 == 1);
    assert(p3.use_count() == 2);
  }
  // dynamic_pointer_cast
  {
    struct t {
      t() {}
      virtual ~t() {}
    };
    struct t2 {
      t2() {}
      virtual ~t2() {}
    };
    struct tt : t, t2 {};
    shared_ptr<tt> p = make_shared<tt>();
    shared_ptr<t> p2(p);
    shared_ptr<t> p3 = make_shared<t>();
    {
      shared_ptr<t2> p4 = dynamic_pointer_cast<t2>(p3);
      assert(!p4 && p4.use_count() == 0);
      p4 = dynamic_pointer_cast<t2>(p2);
      assert(p4 && p4.use_count() == 3);
    }
    {
      shared_ptr<t2> p4 = dynamic_pointer_cast<t2>(move(p3));
      assert(!p4 && p4.use_count() == 0);
      assert(p3 && p3.use_count() == 1);
      p4 = dynamic_pointer_cast<t2>(move(p2));
      assert(p4 && p4.use_count() == 2);
      assert(!p2 && p2.use_count() == 0);
    }
  }
}
void test_weak_ptr() {
  using t = test_object<int>;
  weak_ptr<t> p;
  assert(!p.lock());
  assert(p.lock().use_count() == 0);
  {
    {
      weak_ptr<t> pp = p;
      assert(p.expired() && pp.expired());
    }
    {
      weak_ptr<t> pp = move(p);
      assert(p.expired() && pp.expired());
    }
  }

  shared_ptr<t> p2 = make_shared<t>();
  p = p2;
  assert(p.use_count() == 1 && p2.use_count() == 1);
  assert(p.lock().use_count() == 2);
  assert(p.lock().get() == p2.get());
  {
    {
      weak_ptr<t> pp = p;
      assert(p.use_count() == 1);
      assert(pp.use_count() == 1);
      weak_ptr<t> ppp = move(pp);
      assert(pp.expired());
      assert(ppp.use_count() == 1);
      assert(ppp.lock().get() == p2.get());
    }
  }
}
void test_thread() {
  // sort smultiple arrays
  {
    rander r;

    int a[10][10];
    for (int i : irng(0, 10))
      copy(irng(0, 10), begin(a[i]));
    for (auto &x : a)
      while (is_sorted(x))
        shuffle(x, r);
    assert(none_of(a, [](auto &x) {return equal(x, irng(0, 10));}));

    ez_list<thread> l;
    for (auto &x : a)
      l.insert(l.end(), thread(sort, ref(x)));
    for (auto &x : l)
      x.join();
    assert(all_of(l, [](auto &x) {return !x.joinable();}));

    assert(all_of(a, [](auto &x) {return equal(x, irng(0, 10));}));
  }
}
void test_mutex() {
  // special member functions
  {
    static_assert(is_default_constructible<mutex>);
    static_assert(!is_copy_constructible<mutex>);
    static_assert(!is_copy_assignable<mutex>);
    static_assert(!is_move_constructible<mutex>);
    static_assert(!is_move_assignable<mutex>);
  }

  // try_lock()
  // lock()
  // unlock()
  {
    {
      mutex m;
      m.lock();
      thread t([&m]() {
        assert(m.try_lock() == false);
      });
      t.join();
      m.unlock();

      t = thread([&m]() {
        assert(m.try_lock() == true);
        m.unlock();
      });
      t.join();
    }
    {
      mutex m;
      volatile int x{};
      atomic<int> c = 0;

      const auto f = [&m, &x, &c]() {
        m.lock();
        ++c;
        assert(c == 1);
        sleep_for(0.001_s);
        x = x + 1;
        --c;
        m.unlock();
      };
      vector<thread> v(rng(100, f));
      for (auto &z : v)
        z.join();
      assert(x == 100);
      assert(c == 0);
    }
  }
}
void test_recursive_mutex() {
  recursive_mutex mtx;
  mtx.lock();
  mtx.lock();
  mtx.unlock();
  {
    optional<bool> y;
    {
      thread t([&]() {y = mtx.try_lock();});
    }
    assert(!y.empty() && y.value() == false);
  }
  {
    mtx.unlock();
    optional<bool> y;
    {
      thread t([&]() {y = mtx.try_lock();});
    }
    assert(!y.empty() && y.value() == true);
  }
}
void test_timed_mutex() {
  {
    timed_mutex m;
    bool y = false;
    m.lock();
    thread t([&]() {y = m.try_lock_for(100_ms);});
    sleep_for(1200_ms);
    m.unlock();
    t.join();
    assert(y == false);
  }
  {
    timed_mutex m;
    bool y = false;
    m.lock();
    thread t([&]() {y = m.try_lock_for(1200_ms);});
    sleep_for(100_ms);
    m.unlock();
    t.join();
    assert(y == true);
  }
  {
    timed_mutex m;
    bool y = false;
    m.lock();
    thread t([&]() {y = m.try_lock_until(steady_clock::now() + 100_ms);});
    sleep_for(1200_ms);
    m.unlock();
    t.join();
    assert(y == false);
  }
}
void test_lock() {
  // lock_guard
  {
    mutex m;
    {
      m.lock();
      lock_guard g(m, adopt_lock);
    }
    {
      lock_guard g(m);
    }
  }
  // unique_lock
  {
    // unique_lock(m)
    // special member functions
    {
      mutex m;
      using lock_t = unique_lock<mutex>;
      static_assert(is_default_constructible<lock_t>);
      static_assert(!is_copy_constructible<lock_t>);
      static_assert(!is_copy_assignable<lock_t>);
      static_assert(is_move_constructible<lock_t>);
      static_assert(is_move_assignable<lock_t>);
      static_assert(is_swappable<lock_t>);

      lock_t l;
      assert(!l.owns_lock()
             && l.mutex() == nullptr
             && l.operator bool() == false);
      lock_t ll(move(l));
      assert(!l.owns_lock()
             && l.mutex() == nullptr
             && l.operator bool() == false);
      assert(!ll.owns_lock()
             && ll.mutex() == nullptr
             && ll.operator bool() == false);

      l = lock_t(m);
      assert(l.owns_lock()
             && l.mutex() == addressof(m)
             && l.operator bool() == true);

      ll = move(ll);
      assert(!ll.owns_lock()
             && ll.mutex() == nullptr
             && ll.operator bool() == false);
      ll = move(l);
      assert(!l.owns_lock()
             && l.mutex() == nullptr
             && l.operator bool() == false);
      assert(ll.owns_lock()
             && ll.mutex() == addressof(m)
             && ll.operator bool() == true);
      adl_swap(l, ll);
      assert(!ll.owns_lock()
             && ll.mutex() == nullptr
             && ll.operator bool() == false);
      assert(l.owns_lock()
             && l.mutex() == addressof(m)
             && l.operator bool() == true);
    }

    // unique_lock(m, defer_lock)
    // unique_lock(m, try_to_lock)
    // unique_lock(m, adopt_lock)
    {
      mutex m;
      {
        unique_lock l(m, defer_lock);
        assert(!l.owns_lock());
        assert(l.mutex() == addressof(m));
      }
      {
        unique_lock l(m, try_to_lock);
        assert(l.owns_lock());
        assert(l.mutex() == addressof(m));
      }
      {
        m.lock();
        unique_lock l(m, adopt_lock);
        assert(l.owns_lock());
        assert(l.mutex() == addressof(m));
      }
    }

    // lock()
    // try_lock()
    // unlock()
    // release()
    // owns_lock()
    // operator bool()
    // mutex()
    {
      mutex m;
      unique_lock l(m, defer_lock);
      assert(!l.owns_lock());
      assert(!l.operator bool());
      assert(l.mutex() == addressof(m));
      l.lock();
      assert(l.owns_lock());
      assert(l.mutex() == addressof(m));
      l.unlock();
      assert(!l.owns_lock());
      assert(l.mutex() == addressof(m));
      assert(l.try_lock() == true);
      assert(l.owns_lock());
      assert(l.operator bool());
      assert(l.mutex() == addressof(m));

      thread t([&]() {
        unique_lock ll(m, defer_lock);
        assert(ll.try_lock() == false);
        assert(!ll.owns_lock() && ll.mutex() == addressof(m));
      });
      t.join();

      assert(l.owns_lock());
      assert(l.mutex() == addressof(m));
      assert(l.release() == addressof(m));
      assert(!l.owns_lock());
      assert(l.mutex() == nullptr);
      m.unlock();

      assert(l.release() == nullptr);
      assert(!l.owns_lock());
      assert(l.mutex() == nullptr);
    }
  }
}
void test_shared_mutex() {
  shared_mutex m;
  m.lock();
  assert(m.try_lock() == false);
  m.unlock();

  m.lock_shared();
  m.lock_shared();
  m.lock_shared();
  assert(m.try_lock() == false);
  m.unlock_shared();
  assert(m.try_lock() == false);
  m.unlock_shared();
  assert(m.try_lock() == false);
  m.unlock_shared();
  assert(m.try_lock() == true);
  m.unlock();
}
void test_shared_lock() {
  static_assert(movable<shared_lock<shared_mutex>>);
  static_assert(!copyable<shared_lock<shared_mutex>>);
  shared_mutex m;
  {
    {
      shared_lock l(m);
      assert(l.owns_lock());
    }
    shared_lock l(m);
    assert(l.owns_lock());
    unique_lock ll(m, try_to_lock);
    assert(!ll.owns_lock());
  }
  unique_lock ll(m, try_to_lock);
  assert(ll.owns_lock());
  unique_lock ll2(move(ll));
  assert(ll.owns_lock() == false && ll2.owns_lock());
  ll2 = move(ll);
  assert(ll.owns_lock() == false && ll2.owns_lock() == false);
  unique_lock ll3(m, try_to_lock);
  assert(ll3.owns_lock());
  ll3.unlock();

  shared_lock ll4(m, defer_lock);
  assert(ll4.owns_lock() == false);
  assert(!ll4);
  assert(ll4.mutex() == addressof(m));
  assert(ll4.release() == addressof(m));
  assert(ll4.mutex() == nullptr);
  assert(ll4.owns_lock() == false);

  shared_lock ll5(m, defer_lock);
  assert(ll5.owns_lock() == false && ll5.mutex() == addressof(m));

  shared_timed_mutex m2;
  shared_lock ll6(m2,
                  steady_clock::now()
                  + duration_round<steady_clock::duration>(10_ms));
  assert(ll6.owns_lock() && ll6.mutex() == addressof(m2));
  ll6.unlock();
  shared_lock ll7(m2, 10_ms);
  assert(ll7.owns_lock() && ll7.mutex() == addressof(m2));
  ll7.unlock();

  {
    const thread t([&]() {
      unique_lock lk(m2);
      sleep_for(100_ms);
    });
    const thread t2([&]() {
      shared_lock lk(m2, defer_lock);
      assert(lk.try_lock_for(1200_ms));
    });
  }
  {
    const thread t([&]() {
      unique_lock lk(m2);
      sleep_for(100_ms);
    });
    const thread t2([&]() {
      shared_lock lk(m2, defer_lock);
      assert(lk.try_lock_until
             (steady_clock::now()
              + duration_ceil<steady_clock::duration>(1200_ms)));
    });
  }
}
void test_condition_variable() {
  // special member functions
  {
    static_assert(is_default_constructible<condition_variable>);
    static_assert(!is_copy_constructible<condition_variable>);
    static_assert(!is_copy_assignable<condition_variable>);
    static_assert(!is_move_constructible<condition_variable>);
    static_assert(!is_move_assignable<condition_variable>);
  }
  // 1-N producer-consumer model
  //   produce N and notify all consumers to eat (every consumer eat 1)
  {
    struct shared_t {
      mutex m; // protect shared_t except m
      mutex mm;
      // only if m and mm are both unlocked,
      // below data can be accessed
      int x;
      int count_initialized_consumers;
    };
    shared_t shared{};
    condition_variable v;
    assert(shared.x == 0
           && shared.count_initialized_consumers == 0);

    thread producer;
    thread consumers[200];

    // 1-N producer-consumer model
    const auto producer_f = [&]() {
      const auto consumers_ready = [&]() {
        unique_lock l(shared.m);
        unique_lock ll(shared.mm);
        return shared.count_initialized_consumers == ssize(consumers);
      };
      while (!consumers_ready())
        sleep_for(1_ms);

      unique_lock l(shared.m);
      unique_lock ll(shared.mm);

      sleep_for(1_ms);

      shared.x += (int)size(consumers);
      l.unlock();
      v.notify_all();
      ll.unlock();
    };
    const auto consumer_f = [&]() {
      unique_lock l(shared.m);
      unique_lock ll(shared.mm);

      sleep_for(1_ms);

      ll.unlock();
      v.wait(l, [&]() {
        const lock_guard<mutex> guard(shared.mm);
        static thread_local const int guard2 = [&]() {
          ++shared.count_initialized_consumers;
          return 0;
        }();
        return shared.x != 0;
      });
      ll.lock();

      --shared.x;
    };

    producer = thread(producer_f);
    for (auto &t : consumers)
      t = thread(consumer_f);
    producer.join();
    for (auto &t : consumers)
      t.join();
    assert(shared.x == 0);
  }
  // wait_until(lock, t, eq)
  // wait_for(lock, t, eq)
  {
    condition_variable v;
    mutex m;
    unique_lock l(m);
    assert(v.wait_for(l, 1_ms) == cv_timeout);
  }
}
void test_mutex_area() {
  {
    struct producer_t {
      vector<int> v;

      bool start_flag = false;
      bool producer_ready = false;
      bool consumers_ready = false;

      condition_variable waited_consumers;
      condition_variable waited_producer;

      producer_t() = default;
      ~producer_t() = default;
      producer_t(const producer_t &) = delete;
      producer_t &operator =(const producer_t &) = delete;
      producer_t(producer_t &&) = delete;
      producer_t &operator =(producer_t &&) = delete;
    };
    mutex_area<producer_t> producer;

    struct consumer_t {
      thread thrd;
      int i = 0;
      big_int v = big_int(0); // v will be assigned by i!
      function<void ()> f;
    };
    consumer_t consumers[100];
    atomic<int> count_of_initialized_consumers = 0;

    for (auto &z : consumers)
      z.f = [&]() {
        producer.enter([&]() {
          auto &x = producer;
          sleep_for(1_ms);

          x.leave_until([&x]() {return x->start_flag;},
                        x->waited_consumers);

          x.busy_wait([&x]() {return x->producer_ready;});
          if (++count_of_initialized_consumers == ssize(consumers)) {
            x->consumers_ready = true;
            x.leave_notify_one(x->waited_producer);
          }

          x.leave_until([&x]() {return x->v.empty() == false;},
                        x->waited_consumers);

          if (x->v.size() != 0) {
            z.i = x->v.back();
            x->v.pop_back();

            z.v = 1;
            for (int i : irng(2, z.i))
              z.v.mul(i);
          };
        });
      };
    const auto producer_f = [&]() {
      producer.enter([&x = producer]() {
        sleep_for(1_ms);

        x.leave_until([&x]()->bool {return x->start_flag;},
                      x->waited_producer);
        x->producer_ready = true;
        x.leave_until([&x]()->bool {return x->consumers_ready;},
                      x->waited_producer);

        x->v.append_range(irng(400, 500));
        x.leave_notify_all(x->waited_consumers);
      });
    };

    for (auto &z : consumers)
      z.thrd = thread(z.f);
    thread producer_thrd(producer_f);
    producer.enter([&x = producer]() {x->start_flag = true;});
    producer->waited_producer.notify_one();
    producer->waited_consumers.notify_all();

    for (auto &z : consumers)
      z.thrd.join();
    producer_thrd.join();

    assert(is_permutation(irng(400, 500),
                          bind_rng(consumers,
                                   [](const auto &x) {return x.i;})));
    for (auto &x : consumers) {
      big_int v(1);
      for (int i : irng(2, x.i))
        v.mul(i);
      assert(v == x.v);
    }
  }
}
void test_pool_thread() {
  {
    pool_thread t;
    assert(t.joinable() == false);
    assert(t.pool() == nullptr);
  }
  {
    atomic<int> x = 0;
    auto f = [&]() {++x;};
    {
      stable_vector<pool_thread> v;
      for (int c : irng(0, 32))
        v.push_back(pool_thread(f));
      for (auto &u : v) {
        if (u.joinable())
          assert(u.pool() == addressof(default_thread_pool()));
      }
    }
    assert(x.load() == 32);
    {
      stable_vector<re::pool_thread> v;
      for (auto c : irng(0u, thread::hardware_concurrency() * 8u))
        v.push_back(pool_thread(f));
    }
    assert(x.load() == 32 + to_signed(thread::hardware_concurrency()) * 8);
    {
      stable_vector<re::pool_thread> v;
      for (auto c : irng(0u, thread::hardware_concurrency() * 32u))
        v.push_back(pool_thread(f));
    }
    assert(x.load() == 32 + to_signed(thread::hardware_concurrency()) * 40);
  }
  {
    {
      pool_thread t([]() {});
    }
    using thrd_t = pool_thread;
    struct t {
      thrd_t thrd;
      big_int i;
    };
    t a[32]{};
    for (auto &x : a)
      x.thrd = thrd_t([&x]() {
        x.i = 1;
        for (int j : iters(2, 400))
          x.i.mul(j);
      });
    for (auto &x : a)
      if (x.thrd.joinable())
        x.thrd.join();
    big_int i(1);
    for (int j : iters(2, 400))
      i.mul(j);
    assert(equal(bind_rng(a, [](auto &x) {return x.i.data().size();}),
                 rng(32, i.data().size())));
  }
}
void test_thread_pool() {
  {
    int i = 0;
    thread_pool pool(2);
    const auto f = [&i]() {++i;};
    auto t = pool.fetch(f);
    auto t2 = pool.fetch_real(f);
    auto t3 = pool.fetch(f);
    assert(t.joinable());
    assert(t2.joinable());
    assert(!t3.joinable());
    assert(pool.count() == 2u);
    assert(pool.count_running() == 2u);
    assert(pool.count_sleeping() == 0u);
    t.join();
    t2.join();
    assert(i == 3);
    assert(pool.count() == 2u);
    assert(pool.count_running() == 0u);
    assert(pool.count_sleeping() == 2u);
    pool.shrink_to_fit();
    assert(pool.count() == 0u);
    assert(pool.count_running() == 0u);
    assert(pool.count_sleeping() == 0u);

    pool.append(3u);
    atomic<int> j;
    auto f2 = [v = vector<int, test_allocator<int>>(3), &j]() {j += ssize(v);};
    for (int cc : irng(0, 3)) {
      j.store(0);
      {
        vector<pool_thread> v;
        v.push_back(pool.fetch_real(f2));
        v.push_back(pool.fetch_real(f2));
        v.push_back(pool.fetch_real(f2));
      }
      assert(j.load() == 9);
      assert(pool.count() == 3u);
      assert(pool.count_running() == 0u);
      assert(pool.count_sleeping() == 3u);
    }
  }
  {
    thread_pool pool(0u);
    atomic<int> i;
    const auto f = [&i]() {++i;};
    {
      auto t1 = pool.fetch_real(f);
      auto t2 = pool.fetch_real(f);
      auto t3 = pool.fetch_real(f);
    }
    assert(i == 3);
    assert(pool.count() == 3u);
    assert(pool.count_running() == 0u);
    assert(pool.count_sleeping() == 3u);
  }
}
void test_exclusive_thread_pool() {
  exclusive_thread_pool pl(80);
  const auto t0 = steady_clock::now();
  for (int c0 : irng(0, 30)) {
    mutex_area<vector<int>> a;
    for (int c : irng(0, 100)) {
      pl.fetch([&a]() {
        a.enter([&a]() {
          int j = (*a).empty() ? 0 : (*a).back();
          for (int cc : irng(0, 300))
            (*a).push_back(++j);
        });
      });
    }
    pl.join();
    assert(equal(*a, irng(1, 100 * 300 + 1)));
  }
}

void test_concurrency() {
  put("concurrency: ");

  test_atomic();
  test_shared_ptr();
  test_weak_ptr();
  test_thread();
  test_mutex();
  test_recursive_mutex();
  test_timed_mutex();
  test_lock();
  test_shared_mutex();
  test_shared_lock();
  test_condition_variable();
  test_mutex_area();
  test_pool_thread();
  test_thread_pool();
  test_exclusive_thread_pool();

  putln("ok");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_concurrency();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
