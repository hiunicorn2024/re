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

namespace re::inner::test {

template <class V>
void test_small_vector_briefly() {
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
      assert(v.local());
    }
    {
      V v(3, al);
      assert(v.size() == 3);
      assert(v.get_allocator() == al);
      assert(v.local());
    }

    {
      V v(4, 4);
      assert(v.size() == 4 && all_of_equal(v, 4));
      v.assign(4, 5);
      assert(v.size() == 4 && all_of_equal(v, 5));
    }
    {
      V v(20, 4, al);
      assert(v.size() == 20 && all_of_equal(v, 4));
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
      assert(v.capacity() == v.local_buffer_size());

      v.reserve(20);
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() == 20);
      assert(!v.full());
      v.resize(v.local_buffer_size());
      v.shrink_to_fit();
      assert(equal(rng(begin(v), 3), seq(1, 2, 3)));
      assert(v.capacity() == v.local_buffer_size());
      assert(v.full());
    }

    {
      V v;
      assert(v.empty());
      assert(v.capacity() == v.local_buffer_size());

      v.reallocate(0);
      assert(v.empty());
      assert(v.capacity() == v.local_buffer_size());

      v.reallocate(2);
      assert(v.empty());
      assert(v.capacity() >= 2);

      v = {1, 2, 3};
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() >= 3);
      v.reallocate(5);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() >= 5);

      v.reallocate(3);
      assert(equal(v, seq(1, 2, 3)));
      assert(v.capacity() >= 3);
    }
  }

  // operations for range
  {
    {
      V v(from_range, seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      V vv(from_range, seq(1, 2, 3), v.get_allocator());
      assert(equal(vv, seq(1, 2, 3)));
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
      assert(v.local());
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
namespace small_vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, size_t cap) const {
    ::new(p) V();
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap || p->capacity() == V::local_buffer_size());
    assert(equal(*p, r));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap || p->capacity() == V::local_buffer_size());
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
    return p->empty() && p->capacity() == V::local_buffer_size();
  }
};
constexpr fo_is_empty is_empty{};

}
void test_small_vector_ownership() {
  auto r = irng(0, 10);
  for (int i : r)
    for (int j : r)
      for (int ii : r)
        for (int jj : r) {
          using namespace small_vector_test0;
          using small_vector_test0::is_empty;
          const int n1 = i;
          const int cap1 = i + j;
          const int n2 = ii;
          const int cap2 = ii + jj;
          auto testf = [=](auto tag)->void {
            using vec_t = typename decltype(tag)::type;
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), cap1),
               bind(good, _1, irng(0, n1), cap1),
               bind(init, _1, irng(0, n2), cap2),
               bind(good, _1, irng(0, n2), cap2),
               is_empty);
          };
          testf(type_tag<small_vector<int, 10>>{});
          testf(type_tag<small_vector
                         <int, 10, test_allocator<int>>>{});
          testf(type_tag
                <small_vector<int, 10, stateful_test_allocator<int>>>{});
          testf(type_tag<small_vector<test_object<int>, 10>>{});

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
                  = small_vector
                  <int, 10,
                   stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
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
void test_small_vector_construct_from_range() {
  for (int i : irng(0, 20)) {
    auto testf = [i](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i)
             && v1.capacity() == (i <= 10 ? 10u : to_unsigned(i)));
      vec_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i)
             && v2.capacity() == (i <= 10 ? 10u : to_unsigned(i)));
      assert(equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i)
             && v3.capacity() >= to_unsigned(i)
             && v3.capacity() >= 10u);
      assert(equal(v3, irng(0, i)));
    };
    testf(type_tag<small_vector<int, 10>>{});
    testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
    testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  }
}
void test_small_vector_assign_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns)
      for (int i : irng(0, 30)) {
        vec_t v(f());
        v = irng(0, i);
        assert(equal(v, irng(0, i)));
      }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      const auto n = f().size();
      for (int i : irng(0, n)) {
        vec_t v(f()), v_bk(f());
        auto it = v.erase(v.begin() + i);
        assert(it == v.begin() + i);
        assert(equal(rng(it, end(v)), rng(begin(v_bk) + i + 1, end(v_bk))));
      }
      for (int i : irng(0, n))
        for (int j : irng(i, n)) {
          vec_t v(f()), v_bk(f());
          assert(v.erase(begin(v), begin(v)) == begin(v));
          assert(v == v_bk);
          auto it = v.erase(begin(v) + i, begin(v) + j);
          assert(it == begin(v) + i);
          assert(equal(rng(begin(v), it), rng(begin(v_bk), i)));
          assert(equal(rng(it, end(v)), rng(begin(v_bk) + j, end(v_bk))));
        }
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_insert_1() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      const auto n = f().size();
      {
        vec_t v(f()), v_bk(f());
        v.push_back(999);
        assert(v.back() == 999);
        assert(equal(rng(begin(v), end(v) - 1), v_bk));
      }
      for (int i : irng(0, n + 1)) {
        vec_t v(f()), v_bk(f());
        auto it = v.insert(begin(v) + i, 999);
        assert(it == begin(v) + i);
        assert(equal(rng(begin(v), it), rng(begin(v_bk), i)));
        assert(*it == 999);
        assert(equal(rng(it + 1, end(v)), rng(begin(v_bk) + i, end(v_bk))));
      }
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      const auto n = f().size();
      for (int i : irng(0, 40)) {
        {
          vec_t v(f()), v_bk(f());
          v.append_range(irng(0, i));
          assert(equal(v_bk, begin(v)));
          assert(equal(rng(end(v) - i, end(v)), irng(0, i)));
        }
        {
          vec_t v(f()), v_bk(f());
          v.push_back(degraded_irng(irng(0, i)));
          assert(equal(v_bk, begin(v)));
          assert(equal(rng(end(v) - i, end(v)), irng(0, i)));
        }
      }
      for (int i : irng(0, n + 1)) {
        for (int j : irng(0, 40)) {
          vec_t v(f()), v_bk(f());
          auto it = v.insert_range(begin(v) + i, irng(100, 100 + j));
          assert(it == begin(v) + i);
          assert(equal(rng(begin(v), it), rng(begin(v_bk), i)));
          assert(equal(rng(it, j), irng(100, 100 + j)));
          assert(equal(rng(it + j, end(v)), rng(begin(v_bk) + i, end(v_bk))));
        }
      }
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_clear() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      vec_t v(f());
      const auto cp = v.capacity();
      const bool local = v.local();
      v.clear();
      assert(v.capacity() == cp);
      assert(local == v.local());
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      const auto sz = f().size();
      for (int i : irng(0, sz)) {
        {
          vec_t v(f()), v_bk(f());
          v.resize(i);
          assert(v.size() == to_unsigned(i));
          assert(equal(v_bk, begin(v)));
        }
        {
          vec_t v(f()), v_bk(f());
          v.resize(i, 999);
          assert(v.size() == to_unsigned(i));
          assert(equal(v_bk, begin(v)));
        }
      }
      for (int i : irng(sz, sz * 2)) {
        vec_t v(f()), v_bk(f());
        v.resize(i, 999);
        assert(v.size() == to_unsigned(i));
        assert(equal(v_bk, begin(v)));
        assert(all_of_equal(rng(begin(v) + v_bk.size(), end(v)), 999));
      }
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}
void test_small_vector_reallocate() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    vector<function<vec_t ()>> fns;
    for (int i : irng(0, 20))
      for (int j : irng(i, i * 2))
        fns.push_back([=]()->vec_t {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.reserve(j);
            assert(equal(v, irng(0, i)));
            assert(v.capacity() >= to_unsigned(j));
            return v;
          });
    for (auto f : fns) {
      {
        vec_t v(f());
        v.reallocate(v.size());
        assert(v.capacity() == v.size()
               || v.capacity() == v.local_buffer_size());
      }
      for (int i : irng(f().size(), f().size() * 2)) {
        vec_t v(f());
        v.reallocate(i);
        assert(v.capacity() == to_unsigned(i)
               || v.capacity() == v.local_buffer_size());
        v.shrink_to_fit();
        assert(v.size() == v.capacity() || v.size() < v.local_buffer_size());
      }
    }
  };
  testf(type_tag<small_vector<int, 10>>{});
  testf(type_tag<small_vector<int, 10, test_allocator<int>>>{});
  testf(type_tag<small_vector<int, 10, stateful_test_allocator<int>>>{});
}

void test_small_vector() {
  printf("container - small_vector: ");

  inner::test::test_small_vector_briefly<small_vector<int, 8>>();
  inner::test::test_small_vector_briefly<small_vector
                                         <int, 8, test_allocator<int>>>();
  inner::test::test_small_vector_briefly
    <small_vector<int, 8, stateful_test_allocator<int>>>();
  inner::test::test_small_vector_briefly<small_vector<test_object<int>, 8>>();
  inner::test::test_small_vector_ownership();
  inner::test::test_small_vector_construct_from_range();
  inner::test::test_small_vector_assign_range();
  inner::test::test_small_vector_erase();    
  inner::test::test_small_vector_insert_1();
  inner::test::test_small_vector_insert_range();
  inner::test::test_small_vector_clear();
  inner::test::test_small_vector_resize();
  inner::test::test_small_vector_reallocate();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_small_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
