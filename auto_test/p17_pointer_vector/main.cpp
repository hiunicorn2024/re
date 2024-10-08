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

namespace re::inner::test {

void test_allocator_type_helper() {
  // has_allocator_type
  // allocator_type_of
  {
    static_assert(!has_allocator_type<int>);
    static_assert(has_allocator_type<vector<int>>);
    static_assert(has_allocator_type<const vector<int>>);
    static_assert(!has_allocator_type<conditional_allocator_type<int>>);
    static_assert(has_allocator_type<conditional_allocator_type<vector<int>>>);
  }
}
void test_pointer_vector_iterator() {
  int a[3][1] = {{1}, {2}, {3}};
  using itr_t = inner::ptrvec_iterator<int, decltype(begin(a))>;
  using citr_t = inner::ptrvec_iterator<const int, decltype(begin(a))>;
  static_assert(is_same_v<itr_vt<itr_t>, int>);
  static_assert(is_same_v<itr_vt<citr_t>, int>);
  static_assert(is_same_v<itr_dft<itr_t>, ptrdiff_t>);
  static_assert(is_same_v<itr_dft<citr_t>, ptrdiff_t>);
  static_assert(is_same_v<itr_ptr<itr_t>, int *>);
  static_assert(is_same_v<itr_ptr<citr_t>, const int *>);
  static_assert(is_same_v<itr_ref<itr_t>, int &>);
  static_assert(is_same_v<itr_ref<citr_t>, const int &>);
  static_assert(is_ritr<itr_t> && !is_citr<itr_t>);
  static_assert(is_ritr<citr_t> && !is_citr<citr_t>);

  auto testf = [](auto i, auto j) {
    test_equality(i, i);
    test_equality(j, j);
    test_lessness(i, j);
    assert(i + 3 == j);
    assert(3 + i == j);
    assert(i - -3 == j);
    assert(j - 3 == i);
    assert(i - j == -3);
    assert(j - i == 3);
    assert(i[0] == 1 && *i == 1);
    assert(i[1] == 2 && *(i + 1) == 2);
    assert(i[2] == 3 && *(i + 2) == 3);
    assert(j[-1] == 3 && *(j - 1) == 3);
    assert((i += 3) == j);
    assert(i == j);
    assert(i-- == j);
    assert(i == j - 1);
    assert(++i == j);
    --i;
    assert(i++ == j - 1);
    assert(i == j);
    assert((i -= 3) == j - 3);
    assert(i == j - 3);
  };
  testf(inner::make<itr_t>(begin(a)),
        inner::make<itr_t>(end(a)));
  testf(inner::make<citr_t>(begin(a)),
        inner::make<itr_t>(end(a)));
  testf(inner::make<citr_t>(begin(a)),
        inner::make<itr_t>(end(a)));
  testf(inner::make<itr_t>(begin(a)),
        inner::make<citr_t>(end(a)));
  testf(inner::make<citr_t>(begin(a)),
        inner::make<citr_t>(end(a)));
}
void test_pointer_vector_briefly() {
  using V = pointer_vector
    <int, circular_vector
     <int *, stateful_test_allocator<int *, unordered_map>>>;

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
      V v({1, 2, 3}, al);
      V vv(move(v), al);
      assert(equal(vv, seq(1, 2, 3)) && vv.get_allocator() == al);
      assert(v.empty());
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
      assert(v.emplace_back(2) == 2);
      assert(v.back() == 2);
      v.pop_back();
      v.pop_back();
      assert(v.empty());

      v.push_front(2);
      v.push_front((const int &)1);
      assert(v.emplace_front(0) == 0);
      assert(equal(v, seq(0, 1, 2)));
      v.pop_front();
      assert(equal(v, seq(1, 2)));
      v.pop_front();
      v.pop_front();
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
      v.push_front(seq(3, 4));
      v.push_front(seq(1, 2));
      assert(equal(v, irng(1, 5)));
      v.pop_front(2);
      assert(equal(v, irng(3, 5)));
      v.pop_front();
      assert(equal(v, seq(4)));
      v.pop_front();
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

      v.clear();
      v.prepend(0);
      v.prepend(x, 4, seq(5, 6, 7), rng(3, 9));
      assert(equal(v, seq(3, 4, 5, 6, 7, 9, 9, 9, 0)));
    }
  }

  // node operations
  {
    V v;
    v.delete_node(v.new_node(3));
    v = {3};
    v.delete_node(v.exchange_node(v.begin(), v.new_node(4)));
    assert(v == V{4});
    v = {3, 4};
    //v.exchange_node(prev(v.end()), v.exchange_node(v.begin(), prev(v.end())));
    //assert(v == V({4, 3}));

    v = {};
    typename V::iterator it = v.link(v.end(), v.new_node(3));
    assert(*it == 3);
    assert(v == V{3});
    v.delete_node(inner::node(v.begin()));
    v.unlink(v.begin());
    assert(v.empty());
    assert(*v.link_front(v.new_node(1)) == 1);
    assert(*v.link_back(v.new_node(2)) == 2);
    assert(v == V({1, 2}));
    auto p1 = inner::node(v.begin());
    auto p2 = inner::node(prev(v.end()));
    v.unlink(v.begin(), v.end());
    assert(v.empty());
    v.delete_node(p1);
    v.delete_node(p2);
    v = {1, 2};
    p1 = inner::node(v.begin());
    p2 = inner::node(prev(v.end()));
    v.unlink_front();
    assert(v == V{2});
    v.unlink_back();
    assert(v.empty());
    v.delete_node(p1);
    v.delete_node(p2);

    v = {3};
    auto nd = v.extract(v.begin());
    assert(v.empty());
    assert(*nd == 3);
    v.insert(v.end(), move(nd));
    assert(nd == nullptr);
    assert(v == V{3});
  }

  // list operations
  {
    {
      V v = {1, 2, 3};
      V v2({4, 5}, v.get_allocator());
      v.splice(v.end(), v2);
      assert(equal(v, seq(1, 2, 3, 4, 5)));
      assert(v2.empty());
      v.splice(v.end(), move(v2));
      assert(equal(v, seq(1, 2, 3, 4, 5)));
      assert(v2.empty());

      v = {1, 2, 3, 4};
      v.splice(nth(v, 1), v, nth(v, 1));
      assert(equal(v, seq(1, 2, 3, 4)));
      v.splice(nth(v, 1), v, nth(v, 2));
      assert(equal(v, seq(1, 3, 2, 4)));
      v.splice(nth(v, 3), v, nth(v, 0));
      assert(equal(v, seq(3, 2, 1, 4)));
      v.splice(v.end(), v, v.begin());
      assert(equal(v, seq(2, 1, 4, 3)));
      v = {1, 2, 3};
      v2 = {4, 5};
      v.splice(v.begin(), v2, v2.begin());
      assert(equal(v, seq(4, 1, 2, 3)) && equal(v2, seq(5)));
      v.splice(v.end(), v2, v2.begin());
      assert(equal(v, seq(4, 1, 2, 3, 5)) && v2.empty());

      v = {1, 2, 3, 4, 5};
      v.splice(nth(v, 2), v, nth(v, 3), nth(v, 3));
      assert(equal(v, irng(1, 6)));
      v.splice(nth(v, 2), v, nth(v, 3), nth(v, 5));
      assert(equal(v, seq(1, 2, 4, 5, 3)));
      v.splice(nth(v, 4), v, nth(v, 1), nth(v, 4));
      assert(equal(v, seq(1, 2, 4, 5, 3)));
      v.splice(prev(v.end()), v, nth(v, 1), nth(v, 3));
      assert(equal(v, seq(1, 5, 2, 4, 3)));
      v = {1, 2};
      v2 = {3, 4, 5, 6};
      v.splice(nth(v, 1), v2, nth(v2, 1), nth(v2, 3));
      assert(equal(v, seq(1, 4, 5, 2)));
      assert(equal(v2, seq(3, 6)));
      v.splice(nth(v, 1), v2, nth(v2, 1), nth(v2, 1));
      assert(equal(v, seq(1, 4, 5, 2)));
      assert(equal(v2, seq(3, 6)));
    }

    {
      V v = {1, 99, 2, 3, 99, 4};
      assert(v.remove(99) == 2);
      assert(equal(v, seq(1, 2, 3, 4)));
      v.remove(99);
      assert(equal(v, seq(1, 2, 3, 4)));
      v.remove(4);
      assert(equal(v, seq(1, 2, 3)));
      v.remove(1);
      assert(equal(v, seq(2, 3)));
      v.remove_if([](int x) {return x == 2 || x == 3;});
      assert(v.empty());
    }

    {
      V v = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
      v.unique();
      assert(equal(v, irng(1, 6)));
      v = {1, 2, 3, 4, 5};
      v.unique([](auto x, auto y) {return y == x + 1;});
      assert(equal(v, seq(1, 3, 5)));
    }

    {
      V v = {2, 4, 6};
      V vv({1, 3, 5}, v.get_allocator());
      v.merge(vv);
      assert(equal(v, irng(1, 7)));
      assert(vv.empty());
      v = {6, 4, 2};
      vv = {5, 3};
      v.merge(vv, greater<>());
      assert(equal(v, rrng(irng(2, 7))));
      assert(vv.empty());
    }

    {
      V v = {1, 2, 3};
      v.reverse();
      assert(equal(v, seq(3, 2, 1)));
    }

    {
      V v(irng(0, 100));
      auto r = make_rander<minstd_rand0>();
      shuffle(v, r);
      assert(!equal(v, irng(0, 100)));
      v.sort();
      assert(equal(v, irng(0, 100)));
      shuffle(v, r);
      assert(!equal(v, irng(0, 100)));
      v.sort(greater<>());
      assert(equal(v, rrng(irng(0, 100))));
    }
  }

  // swap
  {
    using L = V;
    {
      L l(irng(0, 10));
      vector<int> v(irng(0, 10));
      for (int i : irng(0, 10))
        for (int j : irng(0, 10)) {
          const auto x = nth(l, i);
          const auto y = nth(l, j);
          l.swap(x, y);
          adl_swap(v[i], v[j]);
          assert(equal(l, v));
        }
    }
    {
      L l = {1, 2, 3}, ll({4, 5, 6}, l.get_allocator());
      l.swap(nth(l, 1), ll, nth(ll, 1));
      assert(equal(l, seq(1, 5, 3)));
      assert(equal(ll, seq(4, 2, 6)));
    }
  }

  // replace
  {
    using S = V;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}
void test_pointer_vector_construct_from_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20)) {
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i));
      vec_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i) && equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i) && equal(v3, irng(0, i)));
    }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5,
                           test_allocator<int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
namespace pointer_vector_ownership_test {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r) const {
    ::new(p) V(r);
    assert(equal(*p, r));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, const AL &al) const {
    ::new(p) V(r, al);
    assert(p->get_allocator() == al);
    assert(equal(*p, r));
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
    return p->empty();
  }
};
constexpr fo_is_empty is_empty{};

}
void test_pointer_vector_ownership() {
  for (int i : irng(0, 20))
    for (int j : irng(0, 20)) {
      using namespace pointer_vector_ownership_test;
      using pointer_vector_ownership_test::is_empty;
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        test_allocator_aware_container_ownership<vec_t>
          (bind(init, _1, irng(0, i)),
           bind(good, _1, irng(0, i)),
           bind(init, _1, irng(100, 100 + j)),
           bind(good, _1, irng(100, 100 + j)),
           is_empty);
      };
      testf(type_tag<pointer_vector<int>>());
      testf(type_tag
            <pointer_vector
             <int, vector<int *,
              stateful_test_allocator<int *, unordered_map>>>>());
      testf(type_tag
            <pointer_vector
             <int, circular_vector<int *, stateful_test_allocator
                                   <int *, unordered_map>>>>());
      testf(type_tag<pointer_vector
            <int, small_vector<int *, 5,
                               test_allocator<int *, unordered_map>>>>());
      testf(type_tag<pointer_vector
            <int, ranked_rbtree
             <int *, test_allocator<int *, unordered_map>>>>());

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
            = pointer_vector<int, vector<int *, stateful_test_allocator
                                         <int *, unordered_map,
                                          y1, y2, y3, y4>>>;
          const stateful_test_allocator
            <int *, unordered_map, y1, y2, y3, y4> a1, a2;
          test_allocator_aware_container_ownership<vec_t>
            (bind(init, _1, irng(0, i), a1),
             bind(good, _1, irng(0, i)),
             bind(init, _1, irng(100, 100 + j), a1),
             bind(good, _1, irng(100, 100 + j)),
             is_empty);
          test_allocator_aware_container_ownership<vec_t>
            (bind(init, _1, irng(0, i), a1),
             bind(good, _1, irng(0, i)),
             bind(init, _1, irng(100, 100 + j), a2),
             bind(good, _1, irng(100, 100 + j)),
             is_empty);
        }
      });
    }
}
void test_pointer_vector_assign_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(0, 20)) {
        {
          vec_t v1(irng(0, i));
          v1.assign_range(irng(100, 100 + j));
          assert(equal(v1, irng(100, 100 + j)));
        }
        {
          vec_t v1(irng(0, i));
          v1.assign_range(degraded_irng(irng(100, 100 + j)));
          assert(equal(v1, irng(100, 100 + j)));
        }
      }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5, test_allocator
                           <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_erase_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      vec_t v(irng(0, i));
      if (!v.empty()) {
        v.pop_back();
        assert(equal(v, irng(0, i - 1)));
      }
    }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5,
                           test_allocator<int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_erase_1_front() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      vec_t v(irng(0, i));
      if (!v.empty()) {
        v.pop_front();
        assert(equal(v, irng(1, i)));
      }
    }
  };
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(equal(rng(v.begin(), p), irng(0, j)));
          }
        for (int j : irng(0, i)) {
          vec_t v(irng(0, i));
          auto p = v.erase(nth(v, j));
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
        }
      }
    }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5,
         test_allocator<int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_insert_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      vec_t v(irng(0, i));
      v.push_back(i);
      assert(equal(v, irng(0, i + 1)));
    }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5, test_allocator
                           <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_insert_1_front() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      vec_t v(irng(0, i));
      v.push_front(-1);
      assert(equal(v, irng(-1, i)));
    }
  };
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_insert_1() {
  const auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30))
      for (int j : irng(0, i + 1)) {
        vec_t v(irng(0, i));
        const auto it = v.insert(nth(v, j), 9);
        assert(*it == 9);
        assert(equal(rng(v.cbegin(), it), irng(0, j)));
        assert(equal(rng(next(it), v.cend()), irng(j, i)));
      }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5, test_allocator
                           <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30))
      for (int j : irng(0, i + 1))
        for (int k : irng(0, 40)) {
          vec_t v(irng(0, i));
          const auto it = v.insert(nth(v, j), irng(0, k));
          if (k == 0) {
            assert(it == nth(v, j));
            assert(equal(v, irng(0, i)));
          }
          else {
            assert(*it == 0);
            assert(equal(rng(v.cbegin(), it), irng(0, j)));
            assert(equal(rng(it, next(it, k)), irng(0, k)));
            assert(equal(rng(next(it, k), v.cend()), irng(j, i)));
          }
        }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5, test_allocator
                           <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30))
      for (int k : irng(0, 50)) {
        {
          vec_t v(irng(0, i));
          if (to_unsigned(k) < v.size()) {
            v.resize(k);
            assert(v.size() == to_unsigned(k));
            assert(equal(irng(0, k), v));
          }
          else {
            v.resize(k);
            assert(v.size() == to_unsigned(k));
            assert(equal(irng(0, i), v.begin()));
          }
        }
        {
          vec_t v(irng(0, i));
          if (to_unsigned(k) < v.size()) {
            v.resize(k, 999);
            assert(v.size() == to_unsigned(k));
            assert(equal(irng(0, k), v));
          }
          else {
            v.resize(k, 999);
            assert(v.size() == to_unsigned(k));
            assert(equal(irng(0, i), v.begin()));
            assert(all_of_equal(rng(nth(v, i), end(v)), 999));
          }
        }
      }
  };
  testf(type_tag<pointer_vector<int>>());
  testf(type_tag
        <pointer_vector
         <int, vector<int *, stateful_test_allocator
                      <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, circular_vector<int *, stateful_test_allocator
                               <int *, unordered_map>>>>());
  testf(type_tag
        <pointer_vector
         <int, deque<int *, stateful_test_allocator
                     <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
        <int, small_vector<int *, 5, test_allocator
                           <int *, unordered_map>>>>());
  testf(type_tag<pointer_vector
                 <int, ranked_rbtree
                  <int *, test_allocator<int *, unordered_map>>>>());
}
void test_pointer_vector_node_and_slice() {
  using L = pointer_vector<int, vector<int *, stateful_test_allocator<int *>>>;
  {
    L::node_type n0;
    assert(n0.empty());
    L l;
    L::node_type n = l.make_node(1);
    assert(!n.empty() && *n == 1);
  }
  {
    L l = {1, 2, 3};
    L::node_type n = l.extract(nth(l, 1));
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    L::iterator it = l.insert(l.begin(), move(n));
    assert(n.empty());
    assert(*it == 2 && equal(l, seq(2, 1, 3)));

    n = l.exchange(l.begin(), L::node_type{});
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    n = l.exchange(l.begin(), n);
    assert(!n.empty() && *n == 1);
    assert(equal(l, seq(2, 3)));

    assert(*l.replace(l.begin(), move(n)) == 1);
    assert(equal(l, seq(1, 3)));
    assert(l.replace(l.begin(), L::node_type{}) == l.begin());
    assert(equal(l, seq(3)));
  }
  {
    L l = {1, 2, 3, 4};
    L m(l.extract(l.begin(), l.end()), l.get_allocator());
    assert(l.empty() && equal(m, irng(1, 5)));
    assert(l.insert(l.end(), L{}) == l.end());
    assert(l.empty());
    const auto tmp = l.insert(l.end(), L({1}, l.get_allocator()));
    assert(tmp == l.begin() && equal(l, seq(1)));

    l = {1, 0, 0, 4};
    m = l.exchange(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator()));
    assert(equal(m, seq(0, 0)) && equal(l, irng(1, 5)));
    m = l.exchange(l.begin(), l.end(), m);
    assert(equal(l, seq(0, 0)) && equal(m, irng(1, 5)));

    l = {1, 0, 0, 4};
    assert(*l.replace(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator()))
           == 2);
    assert(equal(l, irng(1, 5)));
  }
  {
    L l = {1, 2, 3};
    L::node_type n;
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(1, 2, 3)));
    n = l.make_node(4);
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(1, 2, 3, 4)));
  }
  {
    L l = {1, 2, 3};
    L m({0, 0}, l.get_allocator());
    const auto p1 = addressof(ref(m, 0));
    const auto p2 = addressof(ref(m, 1));
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
    assert(addressof(ref(l, 3)) == p1);
    assert(addressof(ref(l, 4)) == p2);
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
  }
}
void test_pointer_vector_local_vector() {
  using vec_t = pointer_vector<int, local_vector<int *, 10>>;
  vec_t v;
  v.append(seq(1, 2, 3));
  assert(equal(v, irng(1, 4)));
}

void test_pointer_vector() {
  printf("container - pointer_vector: ");

  inner::test::test_allocator_type_helper();
  inner::test::test_pointer_vector_iterator();
  inner::test::test_pointer_vector_briefly();
  inner::test::test_pointer_vector_construct_from_range();
  inner::test::test_pointer_vector_ownership();
  inner::test::test_pointer_vector_assign_range();
  inner::test::test_pointer_vector_erase_1_back();
  inner::test::test_pointer_vector_erase_1_front();
  inner::test::test_pointer_vector_erase();
  inner::test::test_pointer_vector_insert_1_back();
  inner::test::test_pointer_vector_insert_1_front();
  inner::test::test_pointer_vector_insert_1();
  inner::test::test_pointer_vector_insert_range();
  inner::test::test_pointer_vector_resize();
  inner::test::test_pointer_vector_node_and_slice();
  inner::test::test_pointer_vector_local_vector();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_pointer_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
