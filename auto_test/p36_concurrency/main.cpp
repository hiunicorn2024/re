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

namespace re::inner::fns {

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
  {
    atomic<long long> a[1000];
    assert(all_of_equal(a, 0));
    atomic<long long> x = 1;
    volatile atomic<long long> vx = 10;

    assert((x = 2) == 2);
    assert((vx = 20) == 20);
    assert(x.operator long long() == 2);
    assert(vx.operator long long() == 20);

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

    long long tmp = 3;
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

    assert((x ^= (long long)(((unsigned long long)-1) << 4u))
           == (long long)-1);
    assert(x == (int)-1);
    assert((vx ^= (long long)(((unsigned long long)-1) << 5u))
           == (long long)-1);
    assert(vx == (int)-1);
  }

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
    static_assert(is_default_constructible_v<mutex>);
    static_assert(!is_copy_constructible_v<mutex>);
    static_assert(!is_copy_assignable_v<mutex>);
    static_assert(!is_move_constructible_v<mutex>);
    static_assert(!is_move_assignable_v<mutex>);
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
      static_assert(is_default_constructible_v<lock_t>);
      static_assert(!is_copy_constructible_v<lock_t>);
      static_assert(!is_copy_assignable_v<lock_t>);
      static_assert(is_move_constructible_v<lock_t>);
      static_assert(is_move_assignable_v<lock_t>);
      static_assert(is_swappable_v<lock_t>);

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
void test_condition_variable() {
  // special member functions
  {
    static_assert(is_default_constructible_v<condition_variable>);
    static_assert(!is_copy_constructible_v<condition_variable>);
    static_assert(!is_copy_assignable_v<condition_variable>);
    static_assert(!is_move_constructible_v<condition_variable>);
    static_assert(!is_move_assignable_v<condition_variable>);
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
    atomic<int> x = 0;
    auto f = [&]() {++x;};
    {
      stable_vector<re::pool_thread> v;
      for (int c : irng(0, 1000))
        v.push_back(pool_thread(f));
    }
    assert(x.load() == 1000);
    {
      stable_vector<re::pool_thread> v;
      for (auto c : irng(0u, thread::hardware_concurrency() * 8u))
        v.push_back(pool_thread(f));
    }
    assert(x.load() == 1000 + to_signed(thread::hardware_concurrency()) * 8);
    {
      stable_vector<re::pool_thread> v;
      for (auto c : irng(0u, thread::hardware_concurrency() * 8u))
        v.push_back(pool_thread(f));
    }
    assert(x.load() == 1000 + to_signed(thread::hardware_concurrency()) * 16);
  }
  {
    {
      pool_thread t([]() {});
    }
    //auto t1 = steady_clock::now();
    using thrd_t = pool_thread;
    struct t {
      thrd_t thrd;
      big_int i;
    };
    t a[256]{};
    for (auto &x : a)
      x.thrd = thrd_t([&x]() {
        x.i = 1;
        for (int j : iters(2, 400))
          x.i.mul(j);
      });
    for (auto &x : a)
      x.thrd.join();
    big_int i(1);
    for (int j : iters(2, 400))
      i.mul(j);
    assert(equal(bind_rng(a, [](auto &x) {return x.i.data().size();}),
                 rng(256, i.data().size())));
  }
}

void test_concurrency() {
  put("concurrency: ");

  inner::fns::test_atomic();
  inner::fns::test_thread();
  inner::fns::test_mutex();
  inner::fns::test_lock();
  inner::fns::test_condition_variable();
  inner::fns::test_mutex_area();
  inner::fns::test_pool_thread();

  putln("ok");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_concurrency();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
