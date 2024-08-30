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
#include <iostream>

namespace re::inner::fns {

namespace vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, size_t cap) const {
    ::new(p) V();
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
  }
};
constexpr fo_init init{};
struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r, size_t cap) const {
    return equal(*p, r);
  }
  template <class V, class R, class AL>
  bool operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    return equal(*p, r) && p->get_allocator() == al;
  }
};
constexpr fo_good good{};
struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty() && p->capacity() == 0;
  }
};
constexpr fo_is_empty is_empty{};

}
void test_vector_ownership() {
  auto r = irng(0, 10);
  for (int i : r)
    for (int j : r)
      for (int ii : r)
        for (int jj : r) {
          using namespace vector_test0;
          using vector_test0::is_empty;
          const int n1 = i;
          const int cap1 = i + j;
          const int n2 = ii;
          const int cap2 = ii + jj;
          auto testf = [=](auto tag) {
            using vec_t = typename decltype(tag)::type;
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), cap1),
               bind(good, _1, irng(0, n1), cap1),
               bind(init, _1, irng(0, n2), cap2),
               bind(good, _1, irng(0, n2), cap2),
               is_empty);
          };
          testf(type_tag<vector<int>>{});
          testf(type_tag<vector<int, test_allocator<int>>>{});
          testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
          testf(type_tag<vector<test_object<int>>>{});

          using P00 = type_pack<true_type, false_type>;
          using P0 = type_pack_mul<P00, P00>;
          using P = type_pack_mul<P0, P0>;
          type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
              constexpr bool y1 = decltype(a)::type::value;
              constexpr bool y2 = decltype(b)::type::value;
              constexpr bool y3 = decltype(c)::type::value;
              constexpr bool y4 = decltype(d)::type::value;
              {
                using vec_t
                  = vector<int, stateful_test_allocator
                           <int, ez_map, y1, y2, y3, y4>>;
                const stateful_test_allocator
                  <int, ez_map, y1, y2, y3, y4> a1, a2;
                test_allocator_aware_container_ownership<vec_t>
                  (bind(init, _1, irng(0, n1), cap1, a1),
                   bind(good, _1, irng(0, n1), cap1),
                   bind(init, _1, irng(0, n2), cap2, a1),
                   bind(good, _1, irng(0, n2), cap2),
                   is_empty);
                test_allocator_aware_container_ownership<vec_t>
                  (bind(init, _1, irng(0, n1), cap1, a1),
                   bind(good, _1, irng(0, n1), cap1),
                   bind(init, _1, irng(0, n2), cap2, a2),
                   bind(good, _1, irng(0, n2), cap2),
                   is_empty);
              }
            });
        }
}

template <class V>
void test_vector_briefly() {
  // container
  // optional container operations
  // reversible container
  {
    V v;
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

    V v2 = {1, 3, 3};
    test_lessness(v, v2);
  }

  // allocator-aware container
  {
    typename V::allocator_type al;
    {
      V v(al);
      assert(v.empty() && v.get_allocator() == al);
    }
    {
      V v = {1, 2, 3};
      V vv(v, al);
      assert(equal(vv, seq(1, 2, 3)) && vv.get_allocator() == al);
    }
    {
      V v = {1, 2, 3};
      V vv(move(v), al);
      assert(equal(vv, seq(1, 2, 3)) && vv.get_allocator() == al);
      assert(v.empty());
    }
  }

  // sequence container
  // optional sequence container operations
  {
    typename V::allocator_type al;

    {
      V v(3);
      assert(v.size() == 3);
    }
    {
      V v(3, al);
      assert(v.size() == 3);
      assert(v.get_allocator() == al);
    }

    {
      V v(4, 4);
      assert(v.size() == 4 && all_of_equal(v, 4));

      v.assign(4, 5);
      assert(v.size() == 4 && all_of_equal(v, 5));
    }
    {
      V v(4, 4, al);
      assert(v.size() == 4 && all_of_equal(v, 4));
      assert(v.get_allocator() == al);
    }

    {
      const auto a = seq(1, 2, 3);
      const auto a2 = seq(4, 5);
      V v(begin(a), end(a));
      assert(equal(v, seq(1, 2, 3)));

      v.assign(begin(a2), end(a2));
      assert(equal(v, seq(4, 5)));
    }
    {
      const auto a = seq(1, 2, 3);
      V v(begin(a), end(a), al);
      assert(equal(v, a));
      assert(v.get_allocator() == al);
    }

    {
      V v = {1, 2, 3};
      assert(equal(v, seq(1, 2, 3)));

      v = {4, 5, 6};
      assert(equal(v, seq(4, 5, 6)));

      v.assign({7, 8, 9, 10});
      assert(equal(v, seq(7, 8, 9, 10)));
    }
    {
      V v({1, 2, 3}, al);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      V v;
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
      V v = {1, 2, 3};
      v.clear();
      assert(v.empty());
    }

    {
      V v;
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
    }
  }

  // specialized operations
  {
    {
      V v;
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
      V v;
      v.reserve_more(30u);
      assert(v.capacity() == 30u);
      const auto u = rng(3, typename V::value_type{});
      v = u;
      v.shrink_to_fit();
      v.reserve_more(30u);
      assert(equal(v, u) && v.capacity() == 33u);
    }

    {
      V v;
      v.reserve(20);
      assert(v.capacity() == 20);
      v.shrink_to_fit();
      assert(v.capacity() == 0);

      v.reserve(20);
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 20);
      assert(!v.full());
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 3);
      assert(v.full());
    }

    {
      V v;
      assert(v.empty());
      assert(v.capacity() == 0);

      v.reallocate(0);
      assert(v.empty());
      assert(v.capacity() == 0);

      v.reallocate(2);
      assert(v.empty());
      assert(v.capacity() == 2);

      v = {1, 2, 3};
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 3);
      v.reallocate(5);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 5);

      v.reallocate(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 3);
    }
  }

  // operations for range
  {
    {
      V v0 = {1, 2, 3};
      V v(from_range, v0, v0.get_allocator());
      V vv(from_range, v0);
      assert(equal(v, irng(1, 4)));
      assert(equal(vv, irng(1, 4)));
    }
    {
      V v(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v = seq(3, 2, 1);
      assert(equal(v, seq(3, 2, 1)));
      v.assign(seq(4, 5, 6, 7));
      assert(equal(v, irng(4, 8)));
      v.assign_range(rng(0, 0));
      assert(v.empty());
    }
    {
      typename V::allocator_type al;
      V v(irng(1, 4), al);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      V v = {1, 5};
      auto it = v.insert(v.begin() + 1, seq(2, 3, 4));
      assert(it == v.begin() + 1);
      assert(equal(v, irng(1, 6)));
      it = v.insert_range(v.end(), irng(6, 8));
      assert(it == v.end() - 2);
      assert(equal(v, irng(1, 8)));
    }

    {
      V v;
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
      V v;
      const typename V::value_type x = 3;
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
  }

  // remove
  {
    V v = {1, 0, 2, 3, 0, 0, 4, 0};
    assert(v.remove(0) == 4);
    assert(equal(v, irng(1, 5)));
    assert(v.remove_if([](const auto &x) {return 2 <= x && x <= 3;}) == 2);
    assert(equal(v, seq(1, 4)));
  }

  // replace
  {
    using S = V;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}
template <class INT>
void test_vector_carefully() {
  using V = vector<INT, stateful_test_allocator<INT>>;
  {
    V v;
    const auto is_empty = [](auto &v) {
      return (v.begin() == v.end() && v.cbegin() == v.cend()
              && v.cbegin() == v.cend() && v.begin() == v.cend()
              && v.rbegin() == v.rend() && v.crbegin() == v.crend()
              && v.empty() && v.size() == 0
              && v.size() < v.max_size());
    };
    const auto is_empty_const = [](const auto &v) {
      return (v.begin() == v.end() && v.cbegin() == v.cend()
              && v.cbegin() == v.cend() && v.begin() == v.cend()
              && v.rbegin() == v.rend() && v.crbegin() == v.crend()
              && v.crbegin() == v.rend() && v.rbegin() == v.crend()
              && v.empty() && v.size() == 0
              && v.size() < v.max_size());
    };
    assert(is_empty(v));
    assert(is_empty_const(v));
    assert(v.capacity() == 0);
    assert(v.data() == to_address(v.end()));
    assert(v.data() == to_address(v.cend()));
    assert(((const V &)v).data() == to_address(v.end()));
    assert(((const V &)v).data() == to_address(v.cend()));
  }
  {
    V v;
    v.emplace_back(1);
    assert(v.front() == 1 && v.back() == 1);
    assert(((const V &)v).front() == 1 && ((const V &)v).back() == 1);
    assert(!v.empty() && v.size() == 1
           && equal(v, seq(1))
           && equal((const V &)v, seq(1)));
    v.pop_back();
    assert(v.empty() && v.size() == 0);
    assert(v.capacity() != 0);
    v.shrink_to_fit();
    assert(v.capacity() == 0);
  }
  {
    V v, vv;
    for (int i : irng(1, 4))
      v.emplace_back(i);
    assert(equal(v, irng(1, 4)));
    for (auto i : v)
      vv.emplace_back(i);
    assert(v == vv && !v.empty());
    v.shrink_to_fit();
    assert(!v.empty() && v == vv);
    typename V::size_type c = v.capacity();
    while (!v.empty())
      v.pop_back();
    assert(v.capacity() == c);
  }
  {
    for (int i : irng(0, 10)) {
      V v;
      for (int j : irng(0, i * 100))
        v.emplace_back(j);
      assert(equal(v, irng(0, i * 100)));
    }
    for (int i : irng(0, 10)) {
      V v;
      for (int j : irng(0, i * 100))
        v.emplace(v.end(), j);
      assert(equal(v, irng(0, i * 100)));
    }
    V v;
    v.reserve(40);
    assert(v.capacity() == 40);
    for (int i : irng(0, 40))
      v.emplace_back(i);
    assert(v.size() == 40 && v.size() == v.capacity());
    v.emplace_back(40);
    assert(equal(v, irng(0, 41)));
  }
  {
    const auto init_v = [&](auto &v) {
      for (int i : irng(0, 50))
        v.emplace_back(i);
      assert(equal(v, irng(0, 50)));
    };
    for (int i : irng(0, 50)) {
      V v;
      init_v(v);
      const typename V::iterator it = v.emplace(v.begin() + i, 9);
      assert(equal(rng(v.begin(), it), irng(0, i)));
      assert(*it == 9);
      assert(equal(rng(it + 1, v.end()), irng(i, 50)));
    }
    for (int i : irng(0, 50)) {
      V v;
      v.reserve(50);
      assert(v.capacity() == 50);
      for (int j : irng(0, 50))
        v.emplace_back(j);
      assert(v.size() == v.capacity());
      const typename V::iterator it = v.emplace(v.begin() + i, 99);
      assert(equal(rng(v.begin(), it), irng(0, i)));
      assert(*it == 99);
      assert(equal(rng(it + 1, v.end()), irng(i, 50)));
    }
  }
  {
    V v;
    v.emplace_back(3);
    assert(equal(v, seq(3)));
    assert(v.begin() == v.erase(v.begin(), v.begin()));
    assert(v.begin() == v.erase(v.begin()));
    assert(v.empty() && v.size() == 0 && v.capacity() != 0);
    v.emplace_back(3);
    assert(equal(v, seq(3)));
    assert(v.begin() == v.erase(v.begin(), v.end()));
    assert(v.empty() && v.size() == 0 && v.capacity() != 0);
    v.reserve(50);
    assert(v.capacity() == 50);
    v.clear();
    assert(v.capacity() == 50);
    for (int i : irng(0, 30))
      for (int j : irng(i, 30)) {
        v.clear();
        v.append_range(irng(0, 30));
        const typename V::iterator it = v.erase(v.begin() + i, v.begin() + j);
        assert(equal(rng(v.begin(), it), irng(0, i)));
        assert(equal(rng(it, v.end()), irng(j, 30)));
      }
  }
  {
    V v;
    minstd_rand rand;
    uniform_int_distribution<int> dis(0, 100);
    int i;
    for (i = 0; i < 10000;) {
      const int j = dis(rand);
      v.append_range(irng(i, i + j));
      i += j;
    }
    assert(i >= 10000 && v.size() == to_unsigned(i) && equal(v, irng(0, i)));
  }
  {
    V v;
    v.reserve(100);
    v.append_range(irng(0, 100));
    v.push_back(100);
    v.clear();
    v.shrink_to_fit();
    assert(v.capacity() == 0);
    v.reserve(100);
    v.append_range(irng(0, 80));
    v.push_back(0);
    v.append_range(rng(20, 0));
    v.clear();
    v.shrink_to_fit();
    assert(v.capacity() == 0);
    v.reserve(100);
    v.append_range(irng(0, 80));
    v.append_range(rng(21, 0));
    v.erase(v.end() - 21, v.end());
    assert(equal(v, irng(0, 80)));
  }
  {
    V v;
    const auto r = irng(0, 10000);
    const forward_list<int> l(begin(r), end(r));
    const vector<int> ll(begin(r), end(r));
    v.append_range(l);
    assert(equal(v, r));
    v.clear();
    v.shrink_to_fit();
    v.append_range(ll);
    assert(equal(v, r));
    while (!v.empty())
      v.pop_back();
    assert(v.size() == 0 && v.capacity() != 0);
  }
  {
    V v;
    const auto test_insert_range = [&](auto &&l, int i) {
      typename V::iterator it = v.insert_range(v.begin() + i, l);
      assert(equal(rng(v.begin(), it), irng(0, i)));
      assert(equal(rng(it, size(l)), l));
      assert(equal(rng(it + size(l), v.end()), irng(i, v.back() + 1)));
    };
    for (int i : irng(0, 25))
      for (int j : irng(0, 25)) {
        const auto r = irng(0, j);
        const vector<int> l(begin(r), end(r));
        for (int k : irng(0, i)) {
          v.clear();
          v.shrink_to_fit();
          assert(v.capacity() == 0);
          v.reserve(i);
          v.append_range(irng(0, i));
          assert(v.capacity() == to_unsigned(i));
          test_insert_range(l, k);
        }
      }
    for (int i : irng(0, 25))
      for (int j : irng(0, 25)) {
        const auto r = irng(0, j);
        const vector<int> l(begin(r), end(r));
        for (int k : irng(0, i)) {
          v.clear();
          v.shrink_to_fit();
          v.reserve(33);
          v.append_range(irng(0, i));
          test_insert_range(l, k);
        }
      }
    for (int i : irng(0, 25))
      for (int j : irng(0, 25)) {
        const auto r = irng(0, j);
        const forward_list<int> l(begin(r), end(r));
        const auto ll = degraded_irng(l);
        for (int k : irng(0, i)) {
          adl_swap(as_lvalue(V()), v);
          v.reserve(i);
          assert(v.capacity() == to_unsigned(i));
          v.append_range(irng(0, i));
          assert(v.capacity() == to_unsigned(i));
          test_insert_range(ll, k);
        }
      }
    for (int i : irng(0, 25))
      for (int j : irng(0, 25)) {
        const auto r = irng(0, j);
        const forward_list<int> l(begin(r), end(r));
        const auto ll = degraded_irng(l);
        for (int k : irng(0, i)) {
          v.clear();
          v.shrink_to_fit();
          v.reserve(33);
          v.append_range(irng(0, i));
          test_insert_range(ll, k);
        }
      }
  }
  {
    {
      V v(irng(0, 0));
      assert(v.empty());
    }
    {
      V v(irng(0, 5));
      assert(equal(v, irng(0, 5)));
    }
    {
      V v(irng(0, 100));
      assert(equal(v, irng(0, 100)));
      assert(v.capacity() == 100);
    }
    {
      V v(forward_list<int>(0, 0));
      assert(v.empty());
    }
    {
      V v(forward_list<int>(5, 0));
      assert(equal(v, rng(5, 0)));
    }
    {
      V v(forward_list<int>(100, 0));
      assert(v.size() == 100 && all_of_equal(v, 0));
      assert(v.capacity() != 100);
    }
  }
  {
    V v;
    v.reserve(30);
    v.append_range(irng(0, 30));
    assert(v.capacity() == 30);
    v.assign(rrng(irng(0, 30)));
    assert(equal(v, rrng(irng(0, 30))));
    assert(v.capacity() == 30);
    v.assign(irng(0, 40));
    assert(equal(v, irng(0, 40)));
    auto c = v.capacity();
    assert(c > 30);
    v.assign(irng(10, 20));
    assert(equal(v, irng(10, 20)));
    assert(v.capacity() == c);
    v.assign(irng(20, 30));
    assert(equal(v, irng(20, 30)));
    assert(v.capacity() == c);
    v.assign(irng(10, 30));
    assert(equal(v, irng(10, 30)));
    assert(v.capacity() == c);
    v.assign(irng(0, 5));
    assert(equal(v, irng(0, 5)));
    assert(v.capacity() == c);
    auto r = irng(0, 100);
    v.assign(forward_list<int>(begin(r), end(r)));
    assert(equal(v, r));
    assert(v.capacity() > 100);
    c = v.capacity();
    r = irng(0, 30);
    v.assign(forward_list<int>(begin(r), end(r)));
    assert(equal(v, r));
    assert(v.capacity() == c);
  }
  {
    {
      V v(100);
      assert(v.size() == 100);
    }
    {
      V v(30);
      assert(v.size() == 30);
    }
    {
      V v(0);
      assert(v.capacity() == 0);
    }
    {
      V v(irng(0, 30));
      auto it = v.insert(v.begin() + 1, rng(3, 3));
      assert(it == v.begin() + 1);
      it = v.begin();
      assert(equal(seq(0), it));
      ++it;
      assert(equal(seq(3, 3, 3), it));
      it += 3;
      assert(equal(irng(1, 30), it));
    }
  }
  {
    V v(irng(0, 10));
    auto p = v.data();
    v.resize(7, 0);
    assert(equal(v, irng(0, 7)));
    assert(v.data() == p);
    v.resize(7, 0);
    assert(equal(v, irng(0, 7)));
    assert(v.data() == p);
    v.resize(8, 7);
    assert(equal(v, irng(0, 8)));
    v.resize(100, 100);
    auto it = v.begin();
    assert(equal(irng(0, 8), it));
    it += 8;
    assert(equal(rng(100 - 8, 100), it));
    assert(it + (100 - 8) == v.end());
  }
}

void test_vector_construct_from_range() {
  for (int i : irng(0, 100)) {
    auto testf = [i](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i) && v1.capacity() == to_unsigned(i));
      vec_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i) && v2.capacity() == to_unsigned(i));
      assert(equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i));
      assert(equal(v3, irng(0, i)));
    };
    testf(type_tag<vector<int>>{});
    testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
    testf(type_tag<vector<test_object<int>>>{});
  }
}
void test_vector_assign_range() {
  for (int i : irng(0, 20)) {
    for (int j : irng(i, 20)) {
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        vec_t v(rrng(irng(0, i)));
        assert(v.capacity() == to_unsigned(i));
        v.reserve(j);
        assert(v.capacity() == to_unsigned(j));
        assert(equal(v, rrng(irng(0, i))));

        for (int k : irng(0, 20))
          for (int l : irng(k, 20)) {
            vec_t v2(irng(0, k));
            assert(v2.capacity() == to_unsigned(k));
            v2.reserve(l);
            assert(v2.capacity() == to_unsigned(l));
            assert(equal(v2, irng(0, k)));
            v = v2;
            assert(v == v2);

            vec_t z(rrng(irng(0, i)));
            z.reserve(j);
            z = degraded_irng(v2);
            assert(z == v2);
          }
      };
      testf(type_tag<vector<int>>{});
      testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
      testf(type_tag<vector<test_object<int>>>{});
    }
  }
}
void test_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            vec_t v(irng(0, i));
            assert(v.capacity() == to_unsigned(i) && equal(v, irng(0, i)));
            auto p0 = v.begin();
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(v.begin() == p0);
            assert(equal(rng(v.begin(), p), irng(0, j)));
          }
        for (int j : irng(0, i)) {
          vec_t v(irng(0, i));
          auto p0 = v.begin();
          auto p = v.erase(nth(v, j));
          assert(v.begin() == p0);
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
        }
      }
    }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_insert_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v = irng(0, i);
        const auto p0 = v.begin();
        const bool y = v.size() == v.capacity();
        assert(v.emplace_back(i) == i);
        assert(equal(v, irng(0, i + 1)));
        if (i != 0) {
          if (y)
            assert(p0 != v.begin());
          else
            assert(p0 == v.begin());
        }
      }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_insert_1() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        for (int k : irng(0, i + 1)) {
          vec_t v;
          v.reserve(j);
          v = irng(0, i);
          const bool full = v.size() == v.capacity();
          const auto old_begin = v.begin();
          const auto p = v.insert(nth(v, k), -1);
          assert(*p == -1);
          if (i != 0) {
            if (full)
              assert(old_begin != v.begin());
            else
              assert(old_begin == v.begin());
          }
          assert(equal(rng(v.begin(), p), irng(0, k)));
          assert(equal(rng(p + 1, v.end()), irng(k, i)));
        }
      }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_insert_range_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 20)) {
          const auto r = irng(0, k);
          vec_t v;
          v.reserve(j);
          v = irng(0, i);
          const auto old_begin = v.begin();
          const auto full = size(r) > (v.capacity() - v.size());
          v.push_back(r);
          if (i != 0) {
            if (full)
              assert(old_begin != v.begin());
            else
              assert(old_begin == v.begin());
          }
          assert(v.size() == size(r) + i);
          assert(equal(rng(v.begin(), nth(v, i)), irng(0, i)));
          assert(equal(rng(nth(v, i), v.end()), irng(0, k)));
        }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int pos : irng(0, i + 1))
          for (int k : irng(0, 20)) {
            auto fn = [=](auto get_range, bool one_by_one) {
              const auto r = irng(0, k);
              vec_t v;
              v.reserve(j);
              v.push_back(irng(0, i));
              const auto old_begin = v.begin();
              const bool full = (v.capacity() - v.size()) < size(r);
              const auto p = v.insert(nth(v, pos), get_range(r));
              assert(equal(rng(p, size(r)), r));
              assert(equal(rng(v.begin(), p), irng(0, pos)));
              assert(equal(rng(p + size(r), v.end()), irng(pos, i)));
              if (!one_by_one && i != 0) {
                if (full)
                  assert(v.begin() != old_begin);
                else
                  assert(v.begin() == old_begin);
              }
            };
            fn(identity{}, false);
            fn(degraded_irng, true);
          }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_clear() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(j));
        v.clear();
        assert(v.empty());
        assert(v.capacity() == to_unsigned(j));
      }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 30)) {
          {
            vec_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(j));
            const auto old_begin = v.begin();
            v.resize(k);
            if (k <= i) {
              assert(v.capacity() == to_unsigned(j));
              assert(equal(v, irng(0, k)));
              assert(v.begin() == old_begin);
            }
            else {
              if (i != 0) {
                if (j >= k)
                  assert(v.begin() == old_begin);
                else
                  assert(v.begin() != old_begin);
              }
              assert(equal(rng(v.begin(), i), irng(0, i)));
            }
          }
          {
            vec_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(j));
            const auto old_begin = v.begin();
            v.resize(k, -1);
            if (k <= i) {
              assert(v.capacity() == to_unsigned(j));
              assert(equal(v, irng(0, k)));
              assert(v.begin() == old_begin);
            }
            else {
              if (i != 0) {
                if (j >= k)
                  assert(v.begin() == old_begin);
                else
                  assert(v.begin() != old_begin);
              }
              assert(equal(rng(v.begin(), i), irng(0, i)));
              assert(all_of_equal(rng(nth(v, i), nth(v, k)), -1));
            }
          }
        }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_reallocate() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(i, 30)) {
          vec_t v;
          v.reserve(j);
          v.push_back(irng(0, i));
          assert(equal(v, irng(0, i)));
          assert(v.capacity() == to_unsigned(j));
          v.reallocate(k);
          assert(v.capacity() == to_unsigned(k));
          assert(equal(v, irng(0, i)));
        }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}
void test_vector_shrink_to_fit() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(j));
        const auto old_begin = v.begin();
        v.shrink_to_fit();
        assert(v.capacity() == to_unsigned(i));
        if (i != 0) {
          if (j != i)
            assert(v.begin() != old_begin);
          else
            assert(v.begin() == old_begin);
        }
        else {
          if (j == 0)
            assert(v.begin() == old_begin);
          else
            assert(v.begin() != old_begin);
        }
      }
  };
  testf(type_tag<vector<int>>{});
  testf(type_tag<vector<int, stateful_test_allocator<int>>>{});
  testf(type_tag<vector<test_object<int>>>{});
}

void test_vector() {
  printf("container - vector: ");

  inner::fns::test_vector_ownership();
  inner::fns::test_vector_briefly<vector<int>>();
  inner::fns::test_vector_briefly<vector<int, stateful_test_allocator<int>>>();
  inner::fns::test_vector_briefly<vector<test_object<int>>>();
  inner::fns::test_vector_carefully<int>();
  inner::fns::test_vector_carefully<test_object<int>>();
  inner::fns::test_vector_construct_from_range();
  inner::fns::test_vector_assign_range();
  inner::fns::test_vector_erase();
  inner::fns::test_vector_insert_1_back();
  inner::fns::test_vector_insert_1();
  inner::fns::test_vector_insert_range_back();
  inner::fns::test_vector_insert_range();
  inner::fns::test_vector_clear();
  inner::fns::test_vector_resize();
  inner::fns::test_vector_reallocate();
  inner::fns::test_vector_shrink_to_fit();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
