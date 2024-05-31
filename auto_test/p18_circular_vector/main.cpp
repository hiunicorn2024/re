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

using namespace re;

void test_circular_vector_iterator() {
  using alloc_t = allocator<int>;
  using shared_t = inner::circular_vector_head<alloc_t>;
  using itr_t = inner::circular_vector_iterator<int, alloc_t>;
  using citr_t = inner::circular_vector_iterator<const int, alloc_t>;
  static_assert(random_access_iterator<itr_t>);
  static_assert(!contiguous_iterator<itr_t>);
  static_assert(regular<itr_t>);

  {
    itr_t i{};
    citr_t ii = i;

    test_equality(i, ii);
    test_equality(i, i);
    test_equality(ii, i);
    test_equality(ii, ii);

    assert(i - i == 0);
    assert(i - ii == 0);
    assert(ii - ii == 0);
    assert(ii - i == 0);
  }
  {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    shared_t shared{begin(a), end(a), begin(a), begin(a)};

    for (int &i : iters(0, 10)) {
      const auto wall = nth(a, i);
      auto v = rotate_copy(a, wall, to_back(ez_vector<int>{})).base();
      v.erase(prev(v.end()));
      shared.data_op = wall;
      const itr_t it1 = inner::make<itr_t>(wall, &shared);
      const itr_t it2 = inner::make<itr_t>(wall == begin(a)
                                           ? prev(end(a)) : prev(wall),
                                           &shared);
      const citr_t cit1 = inner::make<citr_t>(wall, &shared);
      const citr_t cit2 = inner::make<citr_t>(wall == begin(a)
                                              ? prev(end(a)) : prev(wall),
                                              &shared);
      test_ritr(rng(it1, it2), v);
      test_ritr(rng(cit1, it2), v);
      test_ritr(rng(it1, cit2), v);
      test_ritr(rng(cit1, cit2), v);
    }
  }
}

template <class V>
void test_circular_vector_briefly() {
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
    assert(equal(rrng(v), seq(3u, 2, 1)));
    assert(equal(rrng(as_const(v)), seq(3u, 2, 1)));

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
      assert(equal(vv, seq(1u, 2, 3)) && vv.get_allocator() == al);
    }
    {
      V v = {1, 2, 3};
      V vv(move(v), al);
      assert(equal(vv, seq(1u, 2, 3)) && vv.get_allocator() == al);
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
      assert(v.size() == 4 && all_of_equal(v, 4u));

      v.assign(4, 5);
      assert(v.size() == 4 && all_of_equal(v, 5u));
    }
    {
      V v(4, 4, al);
      assert(v.size() == 4 && all_of_equal(v, 4u));
      assert(v.get_allocator() == al);
    }

    {
      const auto a = seq(1u, 2, 3);
      const auto a2 = seq(4u, 5);
      V v(begin(a), end(a));
      assert(equal(v, seq(1u, 2, 3)));

      v.assign(begin(a2), end(a2));
      assert(equal(v, seq(4u, 5)));
    }
    {
      const auto a = seq(1u, 2, 3);
      V v(begin(a), end(a), al);
      assert(equal(v, a));
      assert(v.get_allocator() == al);
    }

    {
      V v = {1, 2, 3};
      assert(equal(v, seq(1u, 2, 3)));

      v = {4, 5, 6};
      assert(equal(v, seq(4u, 5, 6)));

      v.assign({7, 8, 9, 10});
      assert(equal(v, seq(7u, 8, 9, 10)));
    }
    {
      V v({1, 2, 3}, al);
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      V v;
      auto it = v.emplace(v.end(), 1);
      assert(it == v.begin());
      assert(v.size() == 1 && v.front() == 1);

      it = v.insert(v.begin(), 2);
      assert(it == v.begin());
      assert(equal(v, seq(2u, 1)));

      const int tmp = 3;
      it = v.insert(v.begin() + 1, tmp);
      assert(it == v.begin() + 1);
      assert(equal(v, seq(2u, 3, 1)));

      it = v.insert(v.begin(), 2, 4);
      assert(it == v.begin());
      assert(equal(v, seq(4u, 4, 2, 3, 1)));

      const auto a = seq(1u, 2, 3);
      it = v.insert(v.end(), begin(a), end(a));
      assert(equal(v, seq(4u, 4, 2, 3, 1, 1, 2, 3)));

      it = v.insert(v.begin(), {4, 4});
      assert(equal(v, seq(4u, 4, 4, 4, 2, 3, 1, 1, 2, 3)));

      it = v.erase(v.begin() + 4);
      assert(equal(v, seq(4u, 4, 4, 4, 3, 1, 1, 2, 3)));
      it = v.erase(it);
      assert(equal(v, seq(4u, 4, 4, 4, 1, 1, 2, 3)));
      it = v.erase(it, v.end() - 1);
      assert(equal(v, seq(4u, 4, 4, 4, 3)));
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

  // specialized operations of circular_vector
  {
    {
      V v;
      v.resize(3);
      assert(v.size() == 3);
      v.resize(0);
      assert(v.empty());
      v = {1, 2};
      v.resize(4, 4);
      assert(equal(v, seq(1u, 2, 4, 4)));
      v.resize(1, 4);
      assert(equal(v, seq(1u)));
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
      assert(v.capacity() == 20u);
      v.shrink_to_fit();
      assert(v.capacity() == 0u);

      v.reserve(20);
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.capacity() == 20u);
      assert(!v.full());
      v.shrink_to_fit();
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.capacity() == 3u);
      assert(v.full());
    }

    {
      V v;
      assert(v.empty());
      assert(v.capacity() == 0u);

      v.reallocate(0);
      assert(v.empty());
      assert(v.capacity() == 0u);

      v.reallocate(2);
      assert(v.empty());
      assert(v.capacity() == 2u);

      v = {1, 2, 3};
      v.shrink_to_fit();
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.capacity() == 3u);
      v.reallocate(5);
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.capacity() == 5u);

      v.reallocate(3);
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.capacity() == 3u);
    }
  }

  // operations for range
  {
    {
      V v(seq(1u, 2, 3));
      assert(equal(v, seq(1u, 2, 3)));
      v = seq(3u, 2, 1);
      assert(equal(v, seq(3u, 2, 1)));
      v.assign(seq(4u, 5, 6, 7));
      assert(equal(v, irng(4u, 8u)));
      v.assign_range(rng(0u, 0u));
      assert(v.empty());
    }
    {
      typename V::allocator_type al;
      V v(irng(1u, 4u), al);
      assert(equal(v, seq(1u, 2, 3)));
      assert(v.get_allocator() == al);
    }

    {
      V v = {1, 5};
      auto it = v.insert(v.begin() + 1, seq(2u, 3, 4));
      assert(it == v.begin() + 1);
      assert(equal(v, irng(1u, 6u)));
      it = v.insert_range(v.end(), irng(6u, 8u));
      assert(it == v.end() - 2);
      assert(equal(v, irng(1u, 8u)));
    }

    {
      V v;
      v.push_back(seq(1u, 2, 3));
      assert(equal(v, seq(1u, 2, 3)));
      v.append_range(seq(4u));
      assert(equal(v, seq(1u, 2, 3, 4)));
      v.pop_back(2);
      assert(equal(v, seq(1u, 2)));
      v.pop_back(2);
      assert(v.empty());
    }

    {
      V v;
      v.push_front(seq(1u, 2, 3));
      assert(equal(v, seq(1u, 2, 3)));
      v.prepend_range(seq(0u));
      assert(equal(v, seq(0u, 1, 2, 3)));
      v.pop_front(2);
      assert(equal(v, seq(2u, 3)));
      v.pop_front(2);
      assert(v.empty());
    }

    {
      V v;
      const typename V::value_type x = 3;
      v.append(x);
      assert(equal(v, seq(3u)));
      v.append(4);
      assert(equal(v, seq(3u, 4)));
      v.append(rng(3, 9));
      assert(equal(v, seq(3u, 4, 9, 9, 9)));

      v.clear();
      v.append(x, 4, seq(5u, 6, 7), rng(3, 9u));
      assert(equal(v, seq(3u, 4, 5, 6, 7, 9, 9, 9)));
    }

    {
      V v;
      const typename V::value_type x = 3;
      v.prepend(x);
      assert(equal(v, seq(x)));
      v.prepend(4);
      assert(equal(v, seq(4u, 3)));
      v.prepend(rng(3, 9));
      assert(equal(v, seq(9u, 9, 9, 4, 3)));

      v.clear();
      v.prepend(seq(7u, 6, 5), 4, 3);
      assert(equal(v, seq(7u, 6, 5, 4, 3)));
    }
  }

  // remove
  {
    V v = {1, 0, 2, 3, 0, 0, 4, 0};
    assert(v.remove(0u) == 4u);
    assert(equal(v, irng(1u, 5u)));
    assert(v.remove_if([](const auto &x) {return 2u <= x && x <= 3u;}) == 2u);
    assert(equal(v, seq(1u, 4)));
  }

  // replace
  {
    using S = V;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0u, 0, 0));
    assert(equal(s, seq(1u, 0, 0, 0, 4)));
  }
}

namespace circular_vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, size_t cap, ptrdiff_t factor) const {
    ::new(p) V();
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
    if (!empty(*p) && factor != 0) {
      if (factor > 0)
        for (; factor; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
      else
        while (factor++) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
      assert(p->capacity() == cap);
      assert(equal(*p, r));
    }
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, size_t cap, ptrdiff_t factor,
                   const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
    if (!empty(*p) && factor != 0) {
      if (factor > 0)
        for (; factor; --factor) {
          auto tmp = p->front();
          p->pop_front();
          p->emplace_back();
          move_backward(rng(begin(*p), end(*p) - 1), end(*p));
          p->front() = move(tmp);
        }
      else
        while (factor++) {
          auto tmp = p->back();
          p->pop_back();
          p->emplace_front();
          move(rng(begin(*p) + 1, end(*p)), begin(*p));
          p->back() = move(tmp);
        }
      assert(p->capacity() == cap);
      assert(equal(*p, r));
    }
  }
};
constexpr fo_init init{};
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
    return p->empty() && p->capacity() == 0;
  }
};
constexpr fo_is_empty is_empty{};

}
void test_circular_vector_ownership() {
  auto r = irng(0, 5);
  for (unsigned n1 : r)
    for (unsigned n2 : r)
      for (unsigned cap1 : irng(n1, 10))
        for (unsigned cap2 : irng(n2, 10))
          for (unsigned factor1 : irng(0, cap1))
            for (unsigned factor2 : irng(0, cap2)) {
              using namespace circular_vector_test0;
              using circular_vector_test0::is_empty;
              auto testf = [=](auto tag) {
                using vec_t = typename decltype(tag)::type;
                test_allocator_aware_container_ownership<vec_t>
                  (bind(init, _1, irng(0, n1), cap1, factor1),
                   bind(good, _1, irng(0, n1)),
                   bind(init, _1, irng(0, n2), cap2, factor2),
                   bind(good, _1, irng(0, n2)),
                   is_empty);
              };
              testf(type_tag<circular_vector<unsigned>>());
              testf(type_tag
                    <circular_vector<unsigned, test_allocator<unsigned>>>());
              testf(type_tag
                    <circular_vector
                     <unsigned, stateful_test_allocator<unsigned>>>());
              testf(type_tag<circular_vector<test_object<unsigned>>>());

              using P00 = type_pack<true_type, false_type>;
              using P0 = type_pack_mul<P00, P00>;
              using P = type_pack_mul<P0, P0>;
              type_pack_for_each<P>
                ([=](auto a, auto b, auto c, auto d) {
                  constexpr bool y1 = decltype(a)::type::value;
                  constexpr bool y2 = decltype(b)::type::value;
                  constexpr bool y3 = decltype(c)::type::value;
                  constexpr bool y4 = decltype(d)::type::value;

                  using vec_t = circular_vector
                    <unsigned, stateful_test_allocator
                     <unsigned, ez_map, y1, y2, y3, y4>>;
                  const stateful_test_allocator
                    <unsigned, ez_map, y1, y2, y3, y4> a1, a2;
                  test_allocator_aware_container_ownership<vec_t>
                    (bind(init, _1, irng(0, n1), cap1, factor1, a1),
                     bind(good, _1, irng(0, n1)),
                     bind(init, _1, irng(0, n2), cap2, factor2, a1),
                     bind(good, _1, irng(0, n2)),
                     is_empty);
                  test_allocator_aware_container_ownership<vec_t>
                    (bind(init, _1, irng(0, n1), cap1, factor1, a1),
                     bind(good, _1, irng(0, n1)),
                     bind(init, _1, irng(0, n2), cap2, factor2, a2),
                     bind(good, _1, irng(0, n2)),
                     is_empty);
                });
            }
}

void test_circular_vector_construct_from_range() {
  for (unsigned i : irng(0, 20)) {
    auto testf = [i](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i) && v1.capacity() == to_unsigned(i));
      vec_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i));
      assert(v2.capacity() == to_unsigned(i));
      assert(equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i));
      assert(equal(v3, irng(0, i)));
    };
    testf(type_tag<circular_vector<unsigned>>());
    testf(type_tag
          <circular_vector<unsigned, stateful_test_allocator<unsigned>>>());
    testf(type_tag<circular_vector<test_object<unsigned>>>());
  }
}
void test_circular_vector_assign_from_range() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, 10)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              v.assign_range(irng(0, i));
              assert(equal(v, irng(0, i)));
              v.~vec_t();
            }
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              v.assign_range(degraded_irng(irng(0, i)));
              assert(equal(v, irng(0, i)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_erase() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1)) {
        for (unsigned i : irng(0, n)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              const auto it = v.erase(begin(v) + i);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(equal(rng(it, end(v)), irng(i + 1, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
        for (unsigned i : irng(0, n))
          for (unsigned j : irng(i, n)) {
            auto testf = [=](auto tag) {
              using namespace circular_vector_test0;
              using vec_t = typename decltype(tag)::type;
              vec_t *p = (vec_t *)malloc(sizeof(vec_t));
              {
                init(p, irng(0, n), cap, x);
                auto &v = *p;
                assert(equal(v, irng(0, n)));
                assert(v.capacity() == cap);
                const auto it = v.erase(begin(v) + i, begin(v) + j);
                assert(equal(rng(begin(v), it), irng(0, i)));
                assert(equal(rng(it, end(v)), irng(j, n)));
                v.~vec_t();
              }
              free(p);
            };
            testf(type_tag<circular_vector<unsigned>>());
            testf(type_tag
                  <circular_vector
                   <unsigned, stateful_test_allocator<unsigned>>>());
            testf(type_tag<circular_vector<test_object<unsigned>>>());
          }
      }
}
void test_circular_vector_insert_1_front() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1)) {
        auto testf = [=](auto tag) {
          using namespace circular_vector_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), cap, x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            assert(v.capacity() == cap);
            assert(v.emplace_front(3u) == 3u);
            assert(*v.begin() == 3u);
            assert(equal(rng(begin(v) + 1, end(v)), irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<circular_vector<unsigned>>());
        testf(type_tag
              <circular_vector
               <unsigned, stateful_test_allocator<unsigned>>>());
        testf(type_tag<circular_vector<test_object<unsigned>>>());
      }
}
void test_circular_vector_insert_1_back() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1)) {
        auto testf = [=](auto tag) {
          using namespace circular_vector_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), cap, x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            assert(v.capacity() == cap);
            assert(v.emplace_back(-1) == (unsigned)-1);
            assert(*(v.end() - 1) == (unsigned)-1);
            assert(equal(rng(begin(v), end(v) - 1), irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<circular_vector<unsigned>>());
        testf(type_tag
              <circular_vector
               <unsigned, stateful_test_allocator<unsigned>>>());
        testf(type_tag<circular_vector<test_object<unsigned>>>());
      }
}
void test_circular_vector_insert_1() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, n)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              const auto it = v.emplace(begin(v) + i, -1);
              assert(equal(rng(begin(v), it), irng(0, i)));
              assert(*it == (unsigned)-1);
              assert(equal(rng(it + 1, end(v)), irng(i, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_insert_range_front() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, 20)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              auto r = irng(1, i + 1);
              v.prepend_range(r);
              assert(size(v) == size(r) + n);
              assert(equal(rng(begin(v), size(r)), r));
              assert(equal(rng(begin(v) + size(r), end(v)), irng(0, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_insert_range_back() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, 20)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              auto r = irng(1, i + 1);
              v.append_range(r);
              assert(size(v) == size(r) + n);
              assert(equal(rng(begin(v), end(v) - size(r)), irng(0, n)));
              assert(equal(rng(end(v) - size(r), end(v)), r));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_insert_range() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, n + 1))
          for (unsigned j : irng(0, 20)) {
            auto testf = [=](auto tag) {
              using namespace circular_vector_test0;
              using vec_t = typename decltype(tag)::type;
              vec_t *p = (vec_t *)malloc(sizeof(vec_t));
              {
                init(p, irng(0, n), cap, x);
                auto &v = *p;
                assert(equal(v, irng(0, n)));
                assert(v.capacity() == cap);

                auto r = irng(1, j + 1);
                auto it = v.insert(begin(v) + i, r);
                assert(equal(rng(begin(v), it), irng(0, i)));
                assert(equal(rng(it, it + size(r)), r));
                assert(equal(rng(it + size(r), end(v)), irng(i, n)));

                v.~vec_t();
              }
              free(p);
            };
            testf(type_tag<circular_vector<unsigned>>());
            testf(type_tag
                  <circular_vector
                   <unsigned, stateful_test_allocator<unsigned>>>());
            testf(type_tag<circular_vector<test_object<unsigned>>>());
          }
}
void test_circular_vector_clear() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1)) {
        auto testf = [=](auto tag) {
          using namespace circular_vector_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), cap, x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            assert(v.capacity() == cap);
            v.clear();
            assert(v.empty());
            assert(v.size() == 0);
            assert(v.capacity() == cap);
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<circular_vector<unsigned>>());
        testf(type_tag
              <circular_vector
               <unsigned, stateful_test_allocator<unsigned>>>());
        testf(type_tag<circular_vector<test_object<unsigned>>>());
      }
}
void test_circular_vector_resize() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(0, 30)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              v.resize(i);
              assert(v.size() == i);
              if (i <= n)
                assert(equal(v, begin(irng(0, n))));
              else
                assert(equal(irng(0, n), begin(v)));
              v.~vec_t();
            }
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              v.resize(i, (unsigned)-1);
              assert(v.size() == i);
              if (i <= n)
                assert(equal(v, begin(irng(0, n))));
              else {
                assert(equal(irng(0, n), begin(v)));
                assert(all_of_equal(rng(begin(v) + n, end(v)), (unsigned)-1));
              }
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_reallocate() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1))
        for (unsigned i : irng(n, 30)) {
          auto testf = [=](auto tag) {
            using namespace circular_vector_test0;
            using vec_t = typename decltype(tag)::type;
            vec_t *p = (vec_t *)malloc(sizeof(vec_t));
            {
              init(p, irng(0, n), cap, x);
              auto &v = *p;
              assert(equal(v, irng(0, n)));
              assert(v.capacity() == cap);
              v.reallocate(i);
              assert(v.capacity() == i);
              assert(equal(v, irng(0, n)));
              v.~vec_t();
            }
            free(p);
          };
          testf(type_tag<circular_vector<unsigned>>());
          testf(type_tag
                <circular_vector
                 <unsigned, stateful_test_allocator<unsigned>>>());
          testf(type_tag<circular_vector<test_object<unsigned>>>());
        }
}
void test_circular_vector_shrink_to_fit() {
  for (unsigned n : irng(0, 5))
    for (unsigned cap : irng(n, 10))
      for (unsigned x : irng(0, cap + 1)) {
        auto testf = [=](auto tag) {
          using namespace circular_vector_test0;
          using vec_t = typename decltype(tag)::type;
          vec_t *p = (vec_t *)malloc(sizeof(vec_t));
          {
            init(p, irng(0, n), cap, x);
            auto &v = *p;
            assert(equal(v, irng(0, n)));
            assert(v.capacity() == cap);
            v.shrink_to_fit();
            assert(v.capacity() == size(v));
            assert(equal(v, irng(0, n)));
            v.~vec_t();
          }
          free(p);
        };
        testf(type_tag<circular_vector<unsigned>>());
        testf(type_tag
              <circular_vector
               <unsigned, stateful_test_allocator<unsigned>>>());
        testf(type_tag<circular_vector<test_object<unsigned>>>());
      }
}

void test_circular_vector() {
  printf("container - circular_vector: ");

  test_circular_vector_iterator();
  test_circular_vector_briefly<circular_vector<unsigned>>();
  test_circular_vector_briefly
    <circular_vector<unsigned, stateful_test_allocator<unsigned>>>();
  test_circular_vector_briefly<circular_vector<test_object<unsigned>>>();
  test_circular_vector_ownership();
  test_circular_vector_construct_from_range();
  test_circular_vector_assign_from_range();
  test_circular_vector_erase();
  test_circular_vector_insert_1_front();
  test_circular_vector_insert_1_back();
  test_circular_vector_insert_1();
  test_circular_vector_insert_range_front();
  test_circular_vector_insert_range_back();
  test_circular_vector_insert_range();
  test_circular_vector_clear();
  test_circular_vector_resize();
  test_circular_vector_reallocate();
  test_circular_vector_shrink_to_fit();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_circular_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
