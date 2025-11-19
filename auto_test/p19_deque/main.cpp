//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>

#include <cassert>

// for test_deque_iterator()
namespace re::inner {

template <>
struct deque_default_lower_buffer_size<int>
  : integral_constant<size_t, 3> {};
template <>
struct deque_default_lower_buffer_size<test_object<int>>
  : integral_constant<size_t, 3> {};

}

namespace re::inner::test {

void test_deque_iterator() {
  using data_t = inner::deque_data<allocator<int>>;
  using itr_t = inner::deque_iterator<int, allocator<int>>;
  using citr_t = inner::deque_iterator<int, allocator<int>>;

  static_assert(random_access_iterator<itr_t>);
  static_assert(!contiguous_iterator<itr_t>);
  static_assert(regular<itr_t>);

  int **p;
  p = new int *[5];
  {
    int c = 0;
    for (auto &x : rng(p, 5))
      for (auto &y : rng(x = new int[3], 3))
        y = ++c;
  }
  const auto guard = exit_fn([&]() {
      for (auto x : rng(p, 5))
        delete [] x;
      delete [] p;
    });

  {
    data_t::upper_buffer_t shared = {p, p + 5, p};
    test_ritr(inner::make<itr_t>(p, *p, &shared),
              inner::make<itr_t>(p + 4, *(p + 4) + 2, &shared),
              irng(1, 15));
  }
  {
    data_t::upper_buffer_t shared = {p, p + 5, p + 4};
    test_ritr(inner::make<itr_t>(p + 4, *(p + 4) + 2, &shared),
              inner::make<itr_t>(p + 3, *(p + 3) + 1, &shared),
              seq(15,
                  1, 2, 3,
                  4, 5, 6,
                  7, 8, 9,
                  10));
    test_ritr(inner::make<itr_t>(p + 4, *(p + 4) + 2, &shared),
              inner::make<itr_t>(p + 3, *(p + 3) + 2, &shared),
              seq(15,
                  1, 2, 3,
                  4, 5, 6,
                  7, 8, 9,
                  10, 11));
    test_ritr(inner::make<itr_t>(p, *p, &shared),
              inner::make<itr_t>(p, *p + 2, &shared),
              seq(1, 2));
    test_ritr(inner::make<itr_t>(p, *p, &shared),
              inner::make<itr_t>(p + 1, *(p + 1), &shared),
              seq(1, 2, 3));
    test_ritr(inner::make<citr_t>(p, *p, &shared),
              inner::make<itr_t>(p + 1, *(p + 1), &shared),
              seq(1, 2, 3));
    test_ritr(inner::make<itr_t>(p, *p, &shared),
              inner::make<citr_t>(p + 1, *(p + 1), &shared),
              seq(1, 2, 3));
    test_ritr(inner::make<citr_t>(p, *p, &shared),
              inner::make<citr_t>(p + 1, *(p + 1), &shared),
              seq(1, 2, 3));
  }
}

void test_deque_briefly() {
  using v_t = deque<int, stateful_test_allocator<int>>;

  // container
  // optional container operations
  // reversible container
  {
    v_t v;
    assert(v.begin() == v.end());
    assert(v.begin() == v.cbegin());
    assert(v.cbegin() == v.begin());
    assert(v.cbegin() == v.cbegin());
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.size() < v.max_size());

    v = {1, 2, 3};
    assert(v.begin() + 3 == v.end());
    assert(!v.empty());
    assert(v.size() == 3);
    test_equality(v, v);
    assert(equal(rrng(v), seq(3, 2, 1)));
    assert(equal(rrng(as_const(v)), seq(3, 2, 1)));

    v_t v2 = {1, 3, 3};
    test_lessness(v, v2);
  }

  // allocator-aware container
  {
    typename v_t::allocator_type al;
    {
      v_t v(al);
      assert(v.empty() && v.get_allocator() == al);
    }
    {
      v_t v = {1, 2, 3};
      v_t vv(v, al);
      assert(equal(vv, seq(1, 2, 3)) && vv.get_allocator() == al);
    }
    {
      v_t v = {1, 2, 3};
      v_t vv(move(v), al);
      assert(equal(vv, seq(1, 2, 3)) && vv.get_allocator() == al);
      assert(v.empty());
    }
  }

  // sequence container
  // optional sequence container operations
  {
    typename v_t::allocator_type al;

    {
      v_t v(3);
      assert(v.size() == 3);
    }
    {
      v_t v(3, al);
      assert(v.size() == 3);
      assert(v.get_allocator() == al);
    }

    {
      v_t v(4, 4);
      assert(v.size() == 4 && all_of_equal(v, 4));

      v.assign(4, 5);
      assert(v.size() == 4 && all_of_equal(v, 5));
    }
    {
      v_t v(4, 4, al);
      assert(v.size() == 4 && all_of_equal(v, 4));
      assert(v.get_allocator() == al);
    }

    {
      const auto a = seq(1, 2, 3);
      const auto a2 = seq(4, 5);
      v_t v(begin(a), end(a));
      assert(equal(v, seq(1, 2, 3)));

      v.assign(begin(a2), end(a2));
      assert(equal(v, seq(4, 5)));
    }
    {
      const auto a = seq(1, 2, 3);
      v_t v(begin(a), end(a), al);
      assert(equal(v, a));
      assert(v.get_allocator() == al);
    }

    {
      v_t v = {1, 2, 3};
      assert(equal(v, seq(1, 2, 3)));

      v = {4, 5, 6};
      assert(equal(v, seq(4, 5, 6)));

      v.assign({7, 8, 9, 10});
      assert(equal(v, seq(7, 8, 9, 10)));
    }
    {
      v_t v({1, 2, 3}, al);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      v_t v;
      auto it = v.emplace(v.end(), 1);
      assert(it == v.begin());
      assert(v.size() == 1 && v.front() == 1);

      it = v.insert(v.begin(), 2);
      assert(it == v.begin());
      assert(equal(v, seq(2, 1)));

      const int tmp = 3;
      it = v.insert(v.begin() + 1, tmp);
      assert(it == v.begin() + 1);
      assert(equal(v, seq(2, 3, 1)));

      it = v.insert(v.begin(), 2, 4);
      assert(it == v.begin());
      assert(equal(v, seq(4, 4, 2, 3, 1)));

      const auto a = seq(1, 2, 3);
      it = v.insert(v.end(), begin(a), end(a));
      assert(equal(v, seq(4, 4, 2, 3, 1, 1, 2, 3)));

      it = v.insert(v.begin(), {4, 4});
      assert(equal(v, seq(4, 4, 4, 4, 2, 3, 1, 1, 2, 3)));

      it = v.erase(v.begin() + 4);
      assert(equal(v, seq(4, 4, 4, 4, 3, 1, 1, 2, 3)));
      it = v.erase(it);
      assert(equal(v, seq(4, 4, 4, 4, 1, 1, 2, 3)));
      it = v.erase(it, v.end() - 1);
      assert(equal(v, seq(4, 4, 4, 4, 3)));
    }

    {
      v_t v = {1, 2, 3};
      v.clear();
      assert(v.empty());
    }

    {
      v_t v;
#ifndef RE_NOEXCEPT
      assert(test_throwing<out_of_range>([&]() {v.at(1);}));
#endif
      v = {1, 2, 3};
      assert(v[0] == 1);
      assert(as_const(v)[1] == 2);
      assert(v.at(2) == 3);
#ifndef RE_NOEXCEPT
      assert(test_throwing<out_of_range>([&]() {v.at(4);}));
#endif

      v = {};
      v.push_back(1);
      assert(v.back() == 1);
      v.emplace_back(2);
      assert(v.back() == 2);
      v.pop_back();
      v.pop_back();
      assert(v.empty());

      v = {};
      v.push_front(1);
      assert(v.front() == 1);
      v.emplace_front(2);
      assert(v.front() == 2);
      v.pop_front();
      v.pop_front();
      assert(v.empty());
    }
  }

  // specialized operations of deque
  {
    {
      v_t v;
      v.resize(3);
      assert(v.size() == 3);
      v.resize(0);
      assert(v.empty());
      v = {1, 2};
      v.resize(4, 4);
      assert(equal(v, seq(1, 2, 4, 4)));
      v.resize(1, 4);
      assert(equal(v, seq(1)));
    }

    {
      v_t v;
      v.reserve_more(30u);
      assert(v.lower_capacity() >= 30u);
      const auto u = rng(3, typename v_t::value_type{});
      v = u;
      v.shrink_to_fit();
      v.reserve_more(30u);
      assert(equal(v, u) && v.lower_capacity() >= 33u);
    }

    {
      v_t v;
      v.reserve(20);
      assert(v.lower_capacity() >= 20);
      v.shrink_to_fit();
      assert(v.lower_capacity() == 0);
      assert(v.upper_capacity() == 0);

      v.reserve(20);
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.lower_capacity() >= 20);
      assert(!v.full());
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
    }

    {
      v_t v;
      assert(v.empty());
      assert(v.lower_capacity() == 0);

      v.reallocate(0);
      assert(v.empty());
      assert(v.lower_capacity() == 0);

      v.reallocate(2);
      assert(v.empty());
      assert(v.upper_capacity() >= 2);

      v = {1, 2, 3};
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
      v.reallocate(5);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.upper_capacity() >= 5);

      v.reallocate(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.upper_capacity() >= 3);
    }
  }

  // operations for range
  {
    {
      v_t v0 = {1, 2, 3};
      v_t v(from_range, v0, v0.get_allocator());
      v_t vv(from_range, v0);
      assert(equal(v, irng(1, 4)));
      assert(equal(vv, irng(1, 4)));
    }
    {
      v_t v(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v = seq(3, 2, 1);
      assert(equal(v, seq(3, 2, 1)));
      v.assign(seq(4, 5, 6, 7));
      assert(equal(v, irng(4, 8)));
      v.assign_range(rng(0, 0));
      assert(v.empty());
    }
    {
      typename v_t::allocator_type al;
      v_t v(irng(1, 4), al);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      v_t v = {1, 5};
      auto it = v.insert(v.begin() + 1, seq(2, 3, 4));
      assert(it == v.begin() + 1);
      assert(equal(v, irng(1, 6)));
      it = v.insert_range(v.end(), irng(6, 8));
      assert(it == v.end() - 2);
      assert(equal(v, irng(1, 8)));
    }

    {
      v_t v;
      v.push_back(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v.append_range(seq(4));
      assert(equal(v, seq(1, 2, 3, 4)));
      v.pop_back(2);
      assert(equal(v, seq(1, 2)));
      v.pop_back(2);
      assert(v.empty());
    }

    {
      v_t v;
      v.push_front(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v.prepend_range(seq(0));
      assert(equal(v, seq(0, 1, 2, 3)));
      v.pop_front(2);
      assert(equal(v, seq(2, 3)));
      v.pop_front(2);
      assert(v.empty());
    }

    {
      v_t v;
      const typename v_t::value_type x = 3;
      v.append(x);
      assert(equal(v, seq(3)));
      v.append(4);
      assert(equal(v, seq(3, 4)));
      v.append(rng(3, 9));
      assert(equal(v, seq(3, 4, 9, 9, 9)));

      v.clear();
      v.append(x, 4, seq(5, 6, 7), rng(3, 9));
      assert(equal(v, seq(3, 4, 5, 6, 7, 9, 9, 9)));
    }

    {
      v_t v;
      const typename v_t::value_type x = 3;
      v.prepend(x);
      assert(equal(v, seq(x)));
      v.prepend(4);
      assert(equal(v, seq(4, 3)));
      v.prepend(rng(3, 9));
      assert(equal(v, seq(9, 9, 9, 4, 3)));

      v.clear();
      v.prepend(seq(7, 6, 5), 4, 3);
      assert(equal(v, seq(7, 6, 5, 4, 3)));
    }
  }

  // remove
  {
    v_t v = {1, 0, 2, 3, 0, 0, 4, 0};
    assert(v.remove(0) == 4);
    assert(equal(v, irng(1, 5)));
    assert(v.remove_if([](const auto &x) {return 2 <= x && x <= 3;}) == 2);
    assert(equal(v, seq(1, 4)));
  }

  // replace
  {
    using S = deque<int>;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }

  // extra: use big alignment to test headed_buffer
  {
    struct alignas(16) t00 {
      int64_t a[1] = {};
      int64_t value = 0;
      t00() = default;
      explicit t00(int x) : value(x) {}
      const int64_t &operator *() const {return value;}
      const int64_t *operator ->() const {return addressof(value);}
    };
    struct alignas(32) t0 {
      int64_t a[3] = {};
      int64_t value = 0;
      t0() = default;
      explicit t0(int x) : value(x) {}
      const int64_t &operator *() const {return value;}
      const int64_t *operator ->() const {return addressof(value);}
    };
    struct alignas(64) t1 {
      int64_t a[7] = {};
      int64_t value = 0;
      t1() = default;
      explicit t1(int x) : value(x) {}
      const int64_t &operator *() const {return value;}
      const int64_t *operator ->() const {return addressof(value);}
    };
    struct alignas(128) t2 {
      int64_t a[15] = {};
      int64_t value = 0;
      t2() = default;
      explicit t2(int x) : value(x) {}
      const int64_t &operator *() const {return value;}
      const int64_t *operator ->() const {return addressof(value);}
    };
    type_pack_for_each<type_pack<t00, t0, t1, t2>>
      ([]<class T>(type_tag<T>) {
        for (int i : irng(1, 10000)) {
          deque<T, test_allocator<T>> v(irng(0, i));
          assert(equal(bind_rng(v, deref), irng(0, i)));
        }
      });
  }
}
void test_deque_carefully() {
  using v_t = deque<int, stateful_test_allocator<int>>;

  // null iterator + 0
  {
    v_t v;
    assert(v.begin() + 0 == v.end());
    assert(v.cbegin() + 0 == v.cend());
  }
  // pop_front + pop_back
  {
    v_t v;
    for (auto &x : iters(0, 1000))
      v.push_back(x);
    auto r = irng(0, 1000);
    for (;;) {
      if (v.empty())
        break;
      v.pop_front();
      r.first += 1;
      assert(equal(v, r));
      if (v.empty())
        break;
      v.pop_back();
      r.second -= 1;
      assert(equal(v, r));
    }
  }
  // push_back 3 + pop_front 2
  {
    v_t v;

    for (const auto x : irng(0, 1024))
      v.emplace_back(x);
    assert(equal(v, irng(0, 1024)));
    for (int c = 500; c; --c)
      v.pop_front();
    for (int c = 500; c; --c)
      v.pop_back();
    for (int c = 10000; c; --c) {
      v.push_back(9);
      v.push_back(9);
      v.push_back(9);
      v.pop_front();
      v.pop_front();
    }
    assert(all_of_equal(v, 9) && v.size() == 10024);
    while (!v.empty())
      v.pop_front();
    assert(v.empty());

    for (const auto x : rrng(irng(0, 1024)))
      v.emplace_front(x);
    assert(equal(v, irng(0, 1024)));
    for (int c = 500; c; --c)
      v.pop_back();
    for (int c = 500; c; --c)
      v.pop_front();
    for (int c = 10000; c; --c) {
      v.push_front(9);
      v.push_front(9);
      v.push_front(9);
      v.pop_back();
      v.pop_back();
    }
    assert(all_of_equal(v, 9) && v.size() == 10024);

    while (!v.empty())
      v.pop_front();
    assert(v.begin() == v.end() && v.empty());
  }
  // push_back 2 + pop_front 2
  {
    v_t v0;
    for (int c = 10000; c != 0; --c) {
      v0.push_back(9);
      v0.push_back(9);
      v0.pop_front();
      v0.pop_front();
    }
    assert(v0.begin() == v0.end() && v0.empty());

    v_t v;
    for (int &x : iters(0, 1000))
      v.push_back(x);
    for (int c = 10000; c != 0; --c) {
      v.push_back(9);
      v.push_back(9);
      v.pop_front();
      v.pop_front();
    }
    assert(equal(v, rng(1000, 9)));
  }
  // pop_back 1 + push_front 1
  {
    v_t v;
    v.reserve(1);
    assert(v.lower_capacity() == 3);
    v.append(1, 2, 3);
    assert(equal(v, irng(1, 4)));
    const auto addr_bk = seq(addressof(ref(v, 0)),
                             addressof(ref(v, 1)),
                             addressof(ref(v, 2)));
    for (int c = 600; c != 0; --c) {
      const int tmp = v.back();
      v.pop_back();
      v.push_front(tmp);
    }
    assert(equal(v, irng(1, 4)));
    assert(equal(bind_rng(v, addressof), addr_bk));
    assert(v.lower_capacity() == 3);
  }
  // append_range by randomized size
  {
    rander<minstd_rand0> rd{};
    v_t v;
    for (int i = 0; i < 1001;) {
      const int j = i + to_signed(rd(0, 7));
      v.append_range(irng(i, min_value(j, 1001)));
      i = j;
    }
    assert(equal(v, irng(0, 1001)));
  }
  // preappend_range by randomized size
  {
    rander<minstd_rand0> rd{};
    v_t v;
    for (int i = 0; i < 1001;) {
      const int j = i + to_signed(rd(0, 7));
      v.prepend_range(rrng(irng(i, min_value(j, 1001))));
      i = j;
    }
    assert(equal(v, rrng(irng(0, 1001))));
  }
}

namespace deque_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, ptrdiff_t factor) const {
    ::new(p) V();
    if (factor > 0) {
      for (auto x : r)
        p->push_back(x);
      assert(equal(*p, r));
      p->reserve(size(r));
      assert(equal(*p, r));
      if (size(r) != 0) {
        const auto cap = p->lower_capacity();
        for (; factor; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
        assert(p->lower_capacity() == cap);
        assert(equal(*p, r));
      }
    }
    else {
      for (auto x : rrng(r))
        p->push_front(x);
      assert(equal(*p, r));
      p->reserve(size(r));
      assert(equal(*p, r));
      if (size(r) != 0) {
        const auto cap = p->lower_capacity();
        while (factor++) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
        assert(p->lower_capacity() == cap);
        assert(equal(*p, r));
      }
    }
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, ptrdiff_t factor, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    if (factor > 0) {
      for (auto x : r)
        p->push_back(x);
      assert(equal(*p, r));
      p->reserve(size(r));
      assert(equal(*p, r));
      if (size(r) != 0) {
        const auto cap = p->lower_capacity();
        for (; factor; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
        assert(p->lower_capacity() == cap);
        assert(equal(*p, r));
      }
    }
    else {
      for (auto x : rrng(r))
        p->push_front(x);
      assert(equal(*p, r));
      p->reserve(size(r));
      assert(equal(*p, r));
      if (size(r) != 0) {
        const auto cap = p->lower_capacity();
        while (factor++) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
        assert(p->lower_capacity() == cap);
        assert(equal(*p, r));
      }
    }
  }
};
constexpr fo_init init{};

struct fo_init2 {
  template <class V, class R>
  void operator ()(V *p, R &&r, ptrdiff_t factor) const {
    ::new(p) V(r);
    assert(equal(*p, r));
    if (size(r) != 0) {
      if (factor > 0) {
        for (; factor != 0; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
        assert(equal(*p, r));
      }
      else {
        for (; factor != 0; ++factor) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
        assert(equal(*p, r));
      }
    }
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, ptrdiff_t factor, const AL &al) const {
    ::new(p) V(r, al);
    assert(p->get_allocator() == al);
    assert(equal(*p, r));
    if (size(r) != 0) {
      if (factor > 0) {
        for (; factor != 0; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
        assert(equal(*p, r));
      }
      else {
        for (; factor != 0; ++factor) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
        assert(equal(*p, r));
      }
    }
  }
};
constexpr fo_init2 init2{};

struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r) const {
    return equal(*p, r);
  }
  template <class V, class R, class AL>
  bool operator ()(V *p, R &&r, const AL &al) const {
    return equal(*p, r) && p->get_allocator() == al;
  }
};
constexpr fo_good good{};

struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty() && p->lower_capacity() == 0 && p->upper_capacity() == 0;
  }
};
constexpr fo_is_empty is_empty{};

}
void test_deque_ownership() {
  for (auto n1 : irng(0, 13))
    for (auto n2 : irng(0, 13))
      for (auto i1 : irng(0, n1 * 3 / 2))
        for (auto i2 : irng(0, n2 * 3 / 2)) {
          using namespace deque_test0;
          using deque_test0::is_empty;
          auto testf = [=](auto tag) {
            using vec_t = typename decltype(tag)::type;
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), i1),
               bind(good, _1, irng(0, n1)),
               bind(init, _1, irng(0, n2), i2),
               bind(good, _1, irng(0, n2)),
               is_empty);
          };
          testf(type_tag<deque<int, stateful_test_allocator<int>>>());
          testf(type_tag<deque<test_object<int>>>());

          using P00 = type_pack<true_type, false_type>;
          using P0 = type_pack_mul<P00, P00>;
          using P = type_pack_mul<P0, P0>;
          type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
              constexpr bool y1 = decltype(a)::type::value;
              constexpr bool y2 = decltype(b)::type::value;
              constexpr bool y3 = decltype(c)::type::value;
              constexpr bool y4 = decltype(d)::type::value;

              using vec_t
                = deque<int, stateful_test_allocator
                        <int, ez_map, y1, y2, y3, y4>>;
              const stateful_test_allocator
                <int, ez_map, y1, y2, y3, y4> a1, a2;
              test_allocator_aware_container_ownership<vec_t>
                (bind(init, _1, irng(0, n1), i1, a1),
                 bind(good, _1, irng(0, n1)),
                 bind(init, _1, irng(0, n2), i2, a1),
                 bind(good, _1, irng(0, n2)),
                 is_empty);
              test_allocator_aware_container_ownership<vec_t>
                (bind(init, _1, irng(0, n1), i1, a1),
                 bind(good, _1, irng(0, n1)),
                 bind(init, _1, irng(0, n2), i2, a2),
                 bind(good, _1, irng(0, n2)),
                 is_empty);
            });
        }
}
void test_deque_construct_from_range() {
  for (int i : irng(0, 20)) {
    auto testf = [i](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i));
      vec_t v2(irng(0, i));
      assert(equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(equal(v3, irng(0, i)));
    };
    testf(type_tag<deque<int, stateful_test_allocator<int>>>());
    testf(type_tag<deque<test_object<int>>>());
  }
}
void test_deque_assign_from_range() {
  for (int n : irng(0, 20))
      for (int i : irng(0, 24))
        for (int j : irng(0, 30)) {
          auto testf = [=](auto tag) {
            using namespace deque_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), i);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              v.assign_range(irng(1, j + 1));
              assert(equal(v, irng(1, j + 1)));
              v.~vec_t();
            }
            {
              init(p, irng(0, n), i);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              v.assign_range(degraded_irng(irng(1, j + 1)));
              assert(equal(v, irng(1, j + 1)));
              v.~vec_t();
            }
            {
              init2(p, irng(0, n), i);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              v.assign_range(irng(1, j + 1));
              assert(equal(v, irng(1, j + 1)));
              v.~vec_t();
            }
            {
              init2(p, irng(0, n), i);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              v.assign_range(degraded_irng(irng(1, j + 1)));
              assert(equal(v, irng(1, j + 1)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<deque<int, stateful_test_allocator<int>>>());
          testf(type_tag<deque<test_object<int>>>());
        }
}
void test_deque_erase() {
  for (int n : irng(0, 20))
    for (int x : irng(0, 24)) {
      for (int i : irng(0, n)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            const auto it = v.erase(begin(v) + i);
            assert(equal(rng(begin(v), it), irng(0, i)));
            assert(equal(rng(it, end(v)), irng(i + 1, n)));
            v.~vec_t();
          }
          {
            init2(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            const auto it = v.erase(begin(v) + i);
            assert(equal(rng(begin(v), it), irng(0, i)));
            assert(equal(rng(it, end(v)), irng(i + 1, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
      for (int i : irng(0, n))
        for (int j : irng(i, n)) {
          auto testf = [=](auto tag) {
            using namespace deque_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              const auto it = v.erase(begin(v) + i, begin(v) + j);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(equal(rng(it, end(v)), irng(j, n)));
              v.~vec_t();
            }
            {
              init2(p, irng(0, n), x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              const auto it = v.erase(begin(v) + i, begin(v) + j);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(equal(rng(it, end(v)), irng(j, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<deque<int, stateful_test_allocator<int>>>());
          testf(type_tag<deque<test_object<int>>>());
        }
    }
}
void test_deque_insert_1_front() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2)) {
      auto testf = [=](auto tag) {
        using namespace deque_test0;
        using vec_t = typename decltype(tag)::type;
        vec_t *p = (vec_t *)malloc(sizeof(vec_t));
        {
          init(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          assert(v.emplace_front(-1) == -1);
          assert(*v.begin() == -1);
          assert(equal(rng(begin(v) + 1, end(v)), irng(0, n)));
          v.~vec_t();
        }
        {
          init2(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          assert(v.emplace_front(-1) == -1);
          assert(*v.begin() == -1);
          assert(equal(rng(begin(v) + 1, end(v)), irng(0, n)));
          v.~vec_t();
        }
        free(p);
      };
      testf(type_tag<deque<int, stateful_test_allocator<int>>>());
      testf(type_tag<deque<test_object<int>>>());
    }
}
void test_deque_insert_1_back() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2)) {
      auto testf = [=](auto tag) {
        using namespace deque_test0;
        using vec_t = typename decltype(tag)::type;
        vec_t *p = (vec_t *)malloc(sizeof(vec_t));
        {
          init(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          assert(v.emplace_back(-1) == -1);
          assert(*(v.end() - 1) == -1);
          assert(equal(rng(begin(v), end(v) - 1), irng(0, n)));
          v.~vec_t();
        }
        {
          init2(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          assert(v.emplace_back(-1) == -1);
          assert(*(v.end() - 1) == -1);
          assert(equal(rng(begin(v), end(v) - 1), irng(0, n)));
          v.~vec_t();
        }
        free(p);
      };
      testf(type_tag<deque<int, stateful_test_allocator<int>>>());
      testf(type_tag<deque<test_object<int>>>());
    }
}
void test_deque_insert_1() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(0, n)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            const auto it = v.emplace(begin(v) + i, -1);
            assert(equal(rng(begin(v), it), irng(0, i)));
            assert(*it == -1);
            assert(equal(rng(it + 1, end(v)), irng(i, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
}
void test_deque_insert_range_front() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(0, 30)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            auto r = irng(1, i + 1);
            v.prepend_range(r);
            assert(size(v) == size(r) + n);
            assert(equal(rng(begin(v), size(r)), r));
            assert(equal(rng(begin(v) + size(r), end(v)), irng(0, n)));
            v.~vec_t();
          }
          {
            init2(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            auto r = irng(1, i + 1);
            v.prepend_range(r);
            assert(size(v) == size(r) + n);
            assert(equal(rng(begin(v), size(r)), r));
            assert(equal(rng(begin(v) + size(r), end(v)), irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
}
void test_deque_insert_range_back() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(0, 30)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            auto r = irng(1, i + 1);
            v.append_range(r);
            assert(size(v) == size(r) + n);
            assert(equal(rng(begin(v), end(v) - size(r)), irng(0, n)));
            assert(equal(rng(end(v) - size(r), end(v)), r));
            v.~vec_t();
          }
          {
            init2(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            auto r = irng(1, i + 1);
            v.append_range(r);
            assert(size(v) == size(r) + n);
            assert(equal(rng(begin(v), end(v) - size(r)), irng(0, n)));
            assert(equal(rng(end(v) - size(r), end(v)), r));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
}
void test_deque_insert_range() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(0, n + 1))
        for (int j : irng(0, 20)) {
          auto testf = [=](auto tag) {
            using namespace deque_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              auto r = irng(1, j + 1);
              auto it = v.insert(begin(v) + i, r);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(equal(rng(it, it + size(r)), r));
              assert(equal(rng(it + size(r), end(v)), irng(i, n)));
              v.~vec_t();
            }
            {
              init2(p, irng(0, n), x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              auto r = irng(1, j + 1);
              auto it = v.insert(begin(v) + i, r);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(equal(rng(it, it + size(r)), r));
              assert(equal(rng(it + size(r), end(v)), irng(i, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<deque<int, stateful_test_allocator<int>>>());
          testf(type_tag<deque<test_object<int>>>());
        }
}
void test_deque_clear() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2)) {
      auto testf = [=](auto tag) {
        using namespace deque_test0;
        using vec_t = typename decltype(tag)::type;
        vec_t *p = (vec_t *)malloc(sizeof(vec_t));
        {
          init(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          v.clear();
          assert(v.empty());
          assert(v.size() == 0);
          v.~vec_t();
        }
        {
          init2(p, irng(0, n), x);
          auto &v = *p;
          assert(equal(v, irng(0, n)));
          v.clear();
          assert(v.empty());
          assert(v.size() == 0);
          v.~vec_t();
        }
        free(p);
      };
      testf(type_tag<deque<int, stateful_test_allocator<int>>>());
      testf(type_tag<deque<test_object<int>>>());
    }
}
void test_deque_resize() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(0, 35)) {
        using namespace deque_test0;
        auto testf = [=](auto tag, auto init) {
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.resize(i);
            assert(v.size() == to_unsigned(i));
            if (i <= n)
              assert(equal(v, begin(irng(0, n))));
            else
              assert(equal(irng(0, n), begin(v)));
            v.~vec_t();
          }
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.resize(i, -1);
            assert(v.size() == to_unsigned(i));
            if (i <= n)
              assert(equal(v, begin(irng(0, n))));
            else {
              assert(equal(irng(0, n), begin(v)));
              assert(all_of_equal(rng(begin(v) + n, end(v)), -1));
            }
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int>>(), init);
        testf(type_tag<deque<int>>(), init2);
        testf(type_tag<deque<int, stateful_test_allocator<int>>>(), init);
        testf(type_tag<deque<int, stateful_test_allocator<int>>>(), init2);
        testf(type_tag<deque<test_object<int>>>(), init);
        testf(type_tag<deque<test_object<int>>>(), init2);
      }
}
void test_deque_reallocate() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2))
      for (int i : irng(n, 30)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.reallocate(i);
            assert(to_unsigned(abs((int)v.lower_capacity() - i))
                   <= inner::deque_default_lower_buffer_size<int>::value);
            assert(equal(v, irng(0, n)));
            v.~vec_t();
          }
          {
            init2(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.reallocate(i);
            assert(to_unsigned(abs((int)v.lower_capacity() - i))
                   <= inner::deque_default_lower_buffer_size<int>::value);
            assert(equal(v, irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
}
void test_deque_shrink_to_fit() {
  for (int n : irng(0, 15))
    for (int x : irng(0, n * 2)) {
        auto testf = [=](auto tag) {
          using namespace deque_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.shrink_to_fit();
            assert(to_unsigned(abs((int)v.lower_capacity() - n))
                   <= inner::deque_default_lower_buffer_size<int>::value);
            assert(equal(v, irng(0, n)));
            v.~vec_t();
          }
          {
            init2(p, irng(0, n), x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            v.shrink_to_fit();
            assert(to_unsigned(abs((int)v.lower_capacity() - n))
                   <= inner::deque_default_lower_buffer_size<int>::value);
            assert(equal(v, irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<deque<int, stateful_test_allocator<int>>>());
        testf(type_tag<deque<test_object<int>>>());
      }
}
void test_deque_lower_capacity_is_full() {
  {
    deque<int, stateful_test_allocator<int>> v;
    assert(v.full());
  }
  {
    for (int i : irng(4, 6)) {
      deque<int, stateful_test_allocator<int>> v(i);
      assert(v.full());
    }
    for (int i : irng(1, 3)) {
      deque<int, stateful_test_allocator<int>> v(i);
      assert(!v.full());
      assert(v.lower_capacity() == 3);
      for (int ii = 10; ii; --ii) {
        v.pop_back();
        v.emplace_front();
        assert(!v.full());
      }
      assert(v.lower_capacity() == 3);
      assert(v.size() == to_unsigned(i));
    }
    {
      deque<int, stateful_test_allocator<int>> v(3);
      assert(v.lower_capacity() == 3);
      assert(v.upper_capacity() == 5);
      assert(v.full());
      for (int i = 10; i; --i) {
        v.pop_back();
        v.emplace_front();
        assert(v.full());
      }
      assert(v.lower_capacity() == 3);
      assert(v.size() == 3);
    }
  }
}

void test_deque() {
  printf("container - deque: ");

  inner::test::test_deque_iterator();
  inner::test::test_deque_briefly();
  inner::test::test_deque_carefully();
  inner::test::test_deque_ownership();
  inner::test::test_deque_construct_from_range();
  inner::test::test_deque_assign_from_range();
  inner::test::test_deque_erase();
  inner::test::test_deque_insert_1_front();
  inner::test::test_deque_insert_1_back();
  inner::test::test_deque_insert_1();
  inner::test::test_deque_insert_range_front();
  inner::test::test_deque_insert_range_back();
  inner::test::test_deque_insert_range();
  inner::test::test_deque_clear();
  inner::test::test_deque_resize();
  inner::test::test_deque_reallocate();
  inner::test::test_deque_shrink_to_fit();
  inner::test::test_deque_lower_capacity_is_full();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_deque();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
