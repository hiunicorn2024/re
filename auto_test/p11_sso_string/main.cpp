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

namespace re::inner::fns {

template <class T>
struct is_sso_string : false_type {};
template <class A, size_t B, class C>
struct is_sso_string<sso_string<A, B, C>> : true_type {};
template <class V>
void test_string_briefly() {
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
      auto it = v.insert(v.end(), 1);
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
      v = rng(3, typename V::value_type{});
      v.shrink_to_fit();
      v.reserve_more(30u);
      assert(v.capacity() == 33u);
    }

    {
      V v;
      v.reserve(20);
      assert(v.capacity() == 20);
      v.shrink_to_fit();
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
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
      if (!is_sso_string<decay_t<decltype(v)>>::value) {
        assert(v.capacity() == 3);
        assert(v.full());
      }
      else {
        assert(v.capacity() == 15);
        assert(!v.full());
      }
    }

    {
      V v;
      assert(v.empty());
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 0);

      v.reallocate(0);
      assert(v.empty());
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 0);

      v.reallocate(2);
      assert(v.empty());
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 2);

      v = {1, 2, 3};
      v.shrink_to_fit();
      assert(equal(v, seq(1, 2, 3)));
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 3);
      v.reallocate(5);
      assert(equal(v, seq(1, 2, 3)));
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 5);

      v.reallocate(3);
      assert(equal(v, seq(1, 2, 3)));
      if (is_sso_string<decay_t<decltype(v)>>::value)
        assert(v.capacity() == 15);
      else
        assert(v.capacity() == 3);
    }
  }

  // operations for range
  {
    {
      V v(from_range, seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      V v2(from_range, seq(1, 2, 3), v.get_allocator());
      assert(equal(v, seq(1, 2, 3)));
      assert(equal(v, v2));
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
}

void test_sso_string_1() {
  using S = sso_string<int, 5, test_allocator<int>>;

  S s;
  assert(s.empty());

  s.push_back('a');
  s.push_back('b');
  assert(s.size() == 2 && s[0] == 'a' && s[1] == 'b' && s[2] == '\0');
  s.pop_back();
  s.pop_back();
  assert(s.empty());

  for (int i = 1001; i; --i)
    s.push_back('c');
  assert(equal(s, rng(1001, 'c')));

  s.reallocate(2000);
  assert(equal(s, rng(1001, 'c')));
  for (int i = 1001; i; --i)
    s.push_back('c');
  assert(equal(s, rng(2002, 'c')));
  s.shrink_to_fit();
  assert(equal(s, rng(2002, 'c')));
  assert(s.capacity() == s.size());

  s.clear();
  assert(s.empty());
  copy(seq('a', 'b', 'c', 'd'), back_inserter(s));
  assert(equal(s, seq('a', 'b', 'c', 'd')));

  auto it = s.insert(s.begin() + 1, 'Z');
  assert(*it == 'Z');
  assert(equal(s, seq('a', 'Z', 'b', 'c', 'd')));
  it = s.insert(s.begin() + 3, 'X');
  assert(*it == 'X');
  assert(equal(s, seq('a', 'Z', 'b', 'X', 'c', 'd')));

  s.clear();
  assert(s.empty());
  s.shrink_to_fit();
  assert(s.empty());
  assert(*s.insert(s.begin(), 'a') == 'a');
  assert(equal(s, seq('a')) && s.capacity() != 0);
  assert(*s.insert(s.end(), 'b') == 'b');
  assert(equal(s, seq('a', 'b')));
  s.insert(s.end(), 'c');
  assert(equal(s, seq('a', 'b', 'c')));
  assert(*s.erase(s.begin() + 1) == 'c');
  assert(equal(s, seq('a', 'c')) && s[2] == '\0');
  s.erase(s.begin(), s.end());
  assert(s.empty() && s[0] == '\0');

  s.shrink_to_fit();
  s.append(seq('a')).append(seq('b', 'c')).append(seq('d', 'e', 'f'));
  assert(equal(seq('a', 'b', 'c', 'd', 'e', 'f', '\0'), s.data()));

  s.clear();
  s.shrink_to_fit();
  s.append(seq('a', 'b', 'c'));
  s.resize(4, 'd');
  assert(equal(seq('a', 'b', 'c', 'd', '\0'), s.data()));
  s.resize(2);
  assert(equal(seq('a', 'b', '\0'), s.data()));

  s = S{'a', 'b', 'c'};
  assert(equal(seq('a', 'b', 'c', '\0'), s.data()));
  s = forward_list<int>{'a', 'b', 'c', 'd'};
  assert(equal(seq('a', 'b', 'c', 'd', '\0'), s.data()));

  s = S(1001);
  assert(s.size() == 1001);
  s = S(503, 'k');
  assert(equal(s, rng(503, 'k')));

  s.clear();
  s.shrink_to_fit();
  s = {'a', 'b', 'c'};
  assert(equal(s, seq('a', 'b', 'c')));
  s = {};
  assert(s.empty());
  s.clear();
  s.shrink_to_fit();
  s = {'d', 'e'};
  assert(equal(s, seq('d', 'e')));
  s = {'g', 'h', 'i', 'j', 'k', 'l', 'm'};
  assert(equal(s, seq('g', 'h', 'i', 'j', 'k', 'l', 'm')));
}
void test_sso_string_2() {
  using V = sso_string<int, 5, stateful_test_allocator<int>>;
  V v, vv;

  // container

  assert(v.begin() == v.end());
  assert(v.begin() == v.cbegin());
  assert(v.cbegin() == v.begin());
  assert(v.cbegin() == v.cbegin());
  assert(v.empty());
  assert(v.size() == 0);
  assert(v.size() < v.max_size());

  // optional container operations

  assert(v == v);
  assert(!(v != v));
  assert(v <= v);
  assert(v >= v);
  assert(!(v < vv));
  assert(!(v > vv));

  // reversible container

  v = {1, 2, 3};
  assert(equal(rrng(v), rng(v.rbegin(), v.rend())));
  assert(equal(rrng(v), rng(v.crbegin(), v.crend())));

  // allocator-aware container

  typename V::allocator_type al;
  v = V(al);
  assert(v.empty() && v.get_allocator() == al);
  vv = {1, 2, 3};
  v = V(vv, al);
  assert(equal(v, seq(1, 2, 3)) && v.get_allocator() == al);
  v = V(move(vv), al);
  assert(equal(v, seq(1, 2, 3)) && v.get_allocator() == al);
  assert(vv.empty());

  // sequence container

  v = V(3);
  assert(v.size() == 3);
  v = V(3, v.get_allocator());
  assert(v.size() == 3);
  v = V(4, 4);
  assert(v.size() == 4 && all_of_equal(v, 4));
  v = V(4, 4, v.get_allocator());
  assert(v.size() == 4 && all_of_equal(v, 4));

  vv = {1, 2, 3};
  v = V(vv.begin(), vv.end());
  assert(v == vv);
  v = V(vv.rbegin(), vv.rend(), vv.get_allocator());
  assert(equal(v, rrng(vv)));
  vv = {2, 3, 4};
  v.assign(vv.begin() + 1, vv.end());
  assert(equal(v, seq(3, 4)));

  v = V({1, 2, 3, 4});
  assert(equal(v, seq(1, 2, 3, 4)));
  v = V({1, 2, 3}, v.get_allocator());
  assert(equal(v, seq(1, 2, 3)));
  v = {3, 3, 3};
  assert(equal(v, seq(3, 3, 3)));
  v.assign({3, 2, 1});
  assert(equal(v, seq(3, 2, 1)));

  v = V();
  auto it = v.insert(v.end(), 1);
  assert(it == v.begin());
  assert(equal(v, seq(1)));
  it = v.insert(v.begin(), 2);
  assert(it == v.begin());
  assert(equal(v, seq(2, 1)));
  int tmp = 3;
  it = v.insert(v.begin() + 1, tmp);
  assert(it == v.begin() + 1);
  assert(equal(v, seq(2, 3, 1)));

  it = v.insert(v.begin() + 1, 2, 9);
  assert(it == v.begin() + 1);
  assert(equal(v, seq(2, 9, 9, 3, 1)));
  const auto r = irng(10, 13);
  it = v.insert(v.end() - 1, r.begin(), r.end());
  assert(it == v.end() - 4);
  assert(equal(v, seq(2, 9, 9, 3, 10, 11, 12, 1)));
  it = v.insert(v.begin(), {0, 1});
  assert(it == v.begin());
  assert(equal(v, seq(0, 1, 2, 9, 9, 3, 10, 11, 12, 1)));

  it = v.erase(v.begin());
  assert(it == v.begin());
  assert(equal(v, seq(1, 2, 9, 9, 3, 10, 11, 12, 1)));
  it = v.erase(v.begin() + 3, v.begin() + 5);
  assert(it == v.begin() + 3);
  assert(equal(v, seq(1, 2, 9, 10, 11, 12, 1)));

  auto c = v.capacity();
  v.clear();
  assert(v.empty() && v.size() == 0 && v.capacity() == c);

  // optional sequence container operations

  v.push_back(1);
  assert(v.front() == v.back() && v.front() == 1);
  v.push_back(2);
  tmp = 3;
  v.push_back(tmp);
  assert(equal(v, seq(1, 2, 3)));
  c = v.capacity();
  v.pop_back();
  assert(equal(v, seq(1, 2)));
  while (!v.empty())
    v.pop_back();
  assert(v.capacity() == c);

  v = {1, 2, 3};
  assert(v[0] == 1 && v[2] == 3);
#ifndef RE_NOEXCEPT
  bool y = false;
  try {
    v.at(3);
  }
  catch (const out_of_range &) {
    y = true;
  }
  assert(y);
#endif

  // specialized operations of std::basic_string

  v = {1, 2, 3};
  int *p = v.data();
  assert(equal(v, p));
  v.resize(1);
  assert(equal(v, seq(1)));
  v.resize(10);
  assert(v.size() == 10);
  v = {1, 2, 3};
  v.resize(5, 4);
  assert(equal(v, seq(1, 2, 3, 4, 4)));

  v = {};
  v.reserve(100);
  for (int i : irng(0, 3))
    v.push_back(i);
  assert(equal(v, seq(0, 1, 2)));
  auto pp = v.data();
  v.shrink_to_fit();
  assert(equal(v, seq(0, 1, 2)));
  assert(v.data() != pp);

  // extensions

  v = V(seq(1, 2, 3));
  assert(equal(v, seq(1, 2, 3)));
  v = seq(4, 5, 6);
  assert(equal(v, seq(4, 5, 6)));
  v.assign(rng(3, 0));
  assert(equal(v, seq(0, 0, 0)));

  v = {};
  it = v.insert(v.end(), seq(1, 2, 2));
  assert(it == v.begin());
  assert(equal(v, seq(1, 2, 2)));
  it = v.insert_range(v.begin(), rng(2, 0));
  assert(it == v.begin());
  assert(equal(v, seq(0, 0, 1, 2, 2)));
  v.push_back(irng(3, 6));
  assert(equal(v, seq(0, 0, 1, 2, 2, 3, 4, 5)));
  v.append_range(list<int>{6, 7, 8});
  assert(equal(v, seq(0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 8)));

  {
    V z = {1, 0, 2, 3, 0, 0, 4, 0};
    assert(z.remove(0) == 4);
    assert(equal(z, irng(1, 5)));
    assert(z.remove_if([](const auto &x) {return 2 <= x && x <= 3;}) == 2);
    assert(equal(z, seq(1, 4)));
  }

  // replace
  {
    using S = string;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}

namespace sso_string_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, size_t cap) const {
    ::new(p) V();
    p->reserve(cap);
    p->append(r);
    assert(*p->end() == 0);
    assert(p->capacity() == max_value(cap, 15u));
    assert(equal(*p, r));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->reserve(cap);
    p->append(r);
    assert(*p->end() == 0);
    assert(p->capacity() == max_value(cap, 15u));
    assert(equal(*p, r));
  }
};
constexpr fo_init init{};
struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r) const {
    return equal(*p, r) && *p->end() == 0;
  }
  template <class V, class R, class AL>
  bool operator ()(V *p, R &&r, const AL &al) const {
    return equal(*p, r) && p->get_allocator() == al && *p->end() == 0;
  }
};
constexpr fo_good good{};
struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty() && p->capacity() == 15;
  }
};
constexpr fo_is_empty is_empty{};

}

void test_sso_string_ownership() {
  auto r = irng(0, 10);
  for (int i : r)
    for (int j : r)
      for (int ii : r)
        for (int jj : r) {
          using namespace sso_string_test0;
          using sso_string_test0::is_empty;
          const int n1 = i;
          const int cap1 = i + j;
          const int n2 = ii;
          const int cap2 = ii + jj;
          auto testf = [=](auto tag) {
            using str_t = typename decltype(tag)::type;
            test_allocator_aware_container_ownership<str_t>
              (bind(init, _1, irng(0, n1), cap1),
               bind(good, _1, irng(0, n1)),
               bind(init, _1, irng(0, n2), cap2),
               bind(good, _1, irng(0, n2)),
               is_empty);
          };
          testf(type_tag<sso_string<int, 15>>());
          testf(type_tag<sso_string<int, 15, test_allocator<int>>>());
          testf(type_tag
                <sso_string<int, 15, stateful_test_allocator<int>>>());

          using P00 = type_pack<true_type, false_type>;
          using P0 = type_pack_mul<P00, P00>;
          using P = type_pack_mul<P0, P0>;
          type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
              constexpr bool y1 = decltype(a)::type::value;
              constexpr bool y2 = decltype(b)::type::value;
              constexpr bool y3 = decltype(c)::type::value;
              constexpr bool y4 = decltype(d)::type::value;

              using str_t = sso_string
                <int, 15,
                 stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
              const stateful_test_allocator
                <int, ez_map, y1, y2, y3, y4> a1, a2;
              test_allocator_aware_container_ownership<str_t>
                (bind(init, _1, irng(0, n1), cap1, a1),
                 bind(good, _1, irng(0, n1)),
                 bind(init, _1, irng(0, n2), cap2, a1),
                 bind(good, _1, irng(0, n2)),
                 is_empty);
              test_allocator_aware_container_ownership<str_t>
                (bind(init, _1, irng(0, n1), cap1, a1),
                 bind(good, _1, irng(0, n1)),
                 bind(init, _1, irng(0, n2), cap2, a2),
                 bind(good, _1, irng(0, n2)),
                 is_empty);
            });
        }
}
void test_sso_string_construct_from_range() {
  for (int i : irng(0, 100)) {
    auto testf = [i](auto tag, size_t k) {
      using str_t = typename decltype(tag)::type;
      str_t v1(i);
      assert(v1.size() == to_unsigned(i)
             && v1.capacity() == max_value(to_unsigned(i), k));
      assert(*v1.end() == 0);
      str_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i)
             && v2.capacity() == max_value(to_unsigned(i), k));
      assert(equal(v2, irng(0, i)));
      assert(*v2.end() == 0);
      str_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i));
      assert(equal(v3, irng(0, i)));
      assert(*v3.end() == 0);
    };
    testf(type_tag<sso_string<int, 15>>(), 15);
    testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>(), 15);
  }
}
void test_sso_string_assign_from_range() {
  for (int i : irng(0, 20)) {
    for (int j : irng(i, 20)) {
      auto testf = [=](auto tag) {
        using str_t = typename decltype(tag)::type;
        str_t v(rrng(irng(0, i)));
        assert(v.capacity() == to_unsigned(max_value(15, i)));
        assert(*v.end() == 0);
        v.reserve(j);
        assert(v.capacity() == to_unsigned(max_value(15, j)));
        assert(equal(v, rrng(irng(0, i))));
        assert(*v.end() == 0);

        for (int k : irng(0, 20))
          for (int l : irng(k, 20)) {
            str_t v2(irng(0, k));
            assert(v2.capacity() == to_unsigned(max_value(15, k)));
            assert(*v2.end() == 0);
            v2.reserve(l);
            assert(v2.capacity() == to_unsigned(max_value(15, l)));
            assert(equal(v2, irng(0, k)));
            assert(*v2.end() == 0);
            v = v2;
            assert(v == v2);

            str_t z(rrng(irng(0, i)));
            z.reserve(j);
            z = degraded_irng(v2);
            assert(z == v2);
            assert(*z.end() == 0);
          }
      };
      testf(type_tag<sso_string<int, 15>>());
      testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
    }
  }
}
void test_sso_string_erase() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            str_t v(irng(0, i));
            assert(v.capacity() == to_unsigned(max_value(15, i))
                   && equal(v, irng(0, i)));
            assert(*v.end() == 0);
            auto p0 = v.begin();
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(v.begin() == p0);
            assert(equal(rng(v.begin(), p), irng(0, j)));
            assert(*v.end() == 0);
          }
        for (int j : irng(0, i)) {
          str_t v(irng(0, i));
          auto p0 = v.begin();
          auto p = v.erase(nth(v, j));
          assert(v.begin() == p0);
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
          assert(*v.end() == 0);
        }
      }
    }
  };
  testf(type_tag<sso_string<int, 15>>());
  testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
}
void test_sso_string_insert_1_back() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        str_t v;
        v.reserve(j);
        v = irng(0, i);
        const auto p0 = v.begin();
        const bool y = v.size() == v.capacity();
        v.push_back(i);
        assert(v.back() == i);
        assert(equal(v, irng(0, i + 1)));
        if (i != 0) {
          if (y)
            assert(p0 != v.begin());
          else
            assert(p0 == v.begin());
        }
      }
  };
  testf(type_tag<sso_string<int, 15>>());
  testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
}
void test_sso_string_insert_1() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        for (int k : irng(0, i + 1)) {
          str_t v;
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
  testf(type_tag<sso_string<int, 15>>());
  testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
}
void test_sso_string_insert_range_back() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 20)) {
          const auto r = irng(0, k);
          str_t v;
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
  testf(type_tag<sso_string<int, 15>>());
  testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
}
void test_sso_string_insert_range() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int pos : irng(0, i + 1))
          for (int k : irng(0, 20)) {
            auto fn = [=](auto get_range, bool one_by_one) {
              const auto r = irng(0, k);
              str_t v;
              v.reserve(j);
              v.push_back(irng(0, i));
              volatile const auto old_begin = v.begin();
              bool full = (v.capacity() - v.size()) < size(r);
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
  testf(type_tag<sso_string<int, 5>>());
  testf(type_tag<sso_string<int, 5, stateful_test_allocator<int>>>());
}
void test_sso_string_clear() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        str_t v;
        v.reserve(j);
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(max_value(15, j)));
        v.clear();
        assert(v.empty());
        assert(v.capacity() == to_unsigned(max_value(15, j)));
      }
  };
  testf(type_tag<sso_string<int, 15>>());
  testf(type_tag<sso_string<int, 15, stateful_test_allocator<int>>>());
}
void test_sso_string_resize() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 30)) {
          {
            str_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(max_value(5, j)));
            const auto old_begin = v.begin();
            v.resize(k);
            if (k <= i) {
              assert(v.capacity() == to_unsigned(max_value(5, j)));
              assert(equal(v, irng(0, k)));
              assert(v.begin() == old_begin);
            }
            else {
              if (i != 0) {
                if (max_value(5, j) >= k)
                  assert(v.begin() == old_begin);
                else
                  assert(v.begin() != old_begin);
              }
              assert(equal(rng(v.begin(), i), irng(0, i)));
            }
          }
          {
            str_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(max_value(5, j)));
            const auto old_begin = v.begin();
            v.resize(k, -1);
            if (k <= i) {
              assert(v.capacity() == to_unsigned(max_value(5, j)));
              assert(equal(v, irng(0, k)));
              assert(v.begin() == old_begin);
            }
            else {
              if (i != 0) {
                if (max_value(5, j) >= k)
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
  testf(type_tag<sso_string<int, 5>>());
  testf(type_tag<sso_string<int, 5, stateful_test_allocator<int>>>());
}
void test_sso_string_reallocate() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 30)) {
          str_t v;
          v.reserve(j);
          v.push_back(irng(0, i));
          assert(equal(v, irng(0, i)));
          assert(v.capacity() == to_unsigned(max_value(5, j)));
          v.reallocate(k);
          assert(v.capacity() >= to_unsigned(k));
          assert(equal(v, irng(0, i)));
        }
  };
  testf(type_tag<sso_string<int, 5>>());
  testf(type_tag<sso_string<int, 5, stateful_test_allocator<int>>>());
}
void test_sso_string_shrink_to_fit() {
  auto testf = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(i, 30)) {
          str_t v;
          v.reserve(j);
          v.push_back(irng(0, i));
          assert(equal(v, irng(0, i)));
          assert(v.capacity() == to_unsigned(max_value(5, j)));
          const bool full = (v.size() == v.capacity());
          const auto old_begin = v.begin();
          const bool old_local = v.local();
          v.shrink_to_fit();
          assert(v.capacity() == to_unsigned(max_value(5, i)));
          if (old_local) {
            assert(v.local());
            assert(v.capacity() == 5);
          }
          else {
            if (full)
              assert(v.begin() == old_begin);
            else {
              assert(v.begin() != old_begin);
              if (i <= 5)
                assert(v.local());
              else
                assert(!v.local());
            }
          }
        }
  };
  testf(type_tag<sso_string<int, 5>>());
  testf(type_tag<sso_string<int, 5, stateful_test_allocator<int>>>());
}

void test_sso_string() {
  test_string_briefly<sso_string<int, 15>>();
  test_string_briefly<sso_string<int, 15, stateful_test_allocator<int>>>();

  test_sso_string_1();
  test_sso_string_2();

  test_sso_string_ownership();
  test_sso_string_construct_from_range();
  test_sso_string_assign_from_range();
  test_sso_string_erase();
  test_sso_string_insert_1_back();
  test_sso_string_insert_1();
  test_sso_string_insert_range_back();
  test_sso_string_insert_range();
  test_sso_string_clear();
  test_sso_string_resize();
  test_sso_string_reallocate();
  test_sso_string_shrink_to_fit();
}

void test_string_reference() {
  // compile-time check for sref constructors
  {
    static_assert(is_convertible_v<char &, sref>);
    static_assert(!is_convertible_v<const char &, sref>);
    static_assert(!is_convertible_v<signed char &, sref>);
    static_assert(!is_convertible_v<const signed char &, sref>);
    static_assert(!is_convertible_v<unsigned char &, sref>);
    static_assert(!is_convertible_v<const unsigned char &, sref>);

    static_assert(is_convertible_v<iter_pair<char *>, sref>);
    static_assert(!is_convertible_v<iter_pair<const char *>, sref>);
    static_assert(!is_convertible_v<iter_pair<unsigned char *>, sref>);
    static_assert(!is_convertible_v<iter_pair<const unsigned char *>, sref>);
    static_assert(!is_convertible_v<iter_pair<signed char *>, sref>);
    static_assert(!is_convertible_v<iter_pair<const signed char *>, sref>);

    static_assert(is_convertible_v<ez_vector<char>, sref>);
    static_assert(!is_convertible_v<ez_vector<const char>, sref>);
    static_assert(!is_convertible_v<ez_vector<unsigned char>, sref>);
    static_assert(!is_convertible_v<ez_vector<const unsigned char>, sref>);
    static_assert(!is_convertible_v<ez_vector<signed char>, sref>);
    static_assert(!is_convertible_v<ez_vector<const signed char>, sref>);

    static_assert(is_convertible_v<char (&)[2], sref>);
    static_assert(!is_convertible_v<const char (&)[2], sref>);
    static_assert(!is_convertible_v<unsigned char (&)[2], sref>);

    static_assert(is_convertible_v<char *, sref>);
    static_assert(!is_convertible_v<unsigned char *, sref>);
  }
  // compile-time check for sview constructors
  {
    static_assert(is_convertible_v<char &, sview>);
    static_assert(is_convertible_v<const char &, sview>);
    static_assert(!is_convertible_v<signed char &, sview>);
    static_assert(!is_convertible_v<unsigned char &, char &>);
    static_assert(!is_convertible_v<const signed char &, sview>);
    static_assert(!is_convertible_v<unsigned char &, sview>);
    static_assert(!is_convertible_v<const unsigned char &, sview>);

    static_assert(is_convertible_v<iter_pair<char *>, sview>);
    static_assert(is_convertible_v<iter_pair<const char *>, sview>);
    static_assert(!is_convertible_v<iter_pair<unsigned char *>, sview>);
    static_assert(!is_convertible_v<iter_pair<const unsigned char *>, sview>);
    static_assert(!is_convertible_v<iter_pair<signed char *>, sview>);
    static_assert(!is_convertible_v<iter_pair<const signed char *>, sview>);

    static_assert(is_convertible_v<ez_vector<char>, sview>);
    static_assert(is_convertible_v<ez_vector<const char>, sview>);
    static_assert(!is_convertible_v<ez_vector<unsigned char>, sview>);
    static_assert(!is_convertible_v<ez_vector<const unsigned char>, sview>);
    static_assert(!is_convertible_v<ez_vector<signed char>, sview>);
    static_assert(!is_convertible_v<ez_vector<const signed char>, sview>);

    static_assert(is_convertible_v<char (&)[2], sview>);
    static_assert(is_convertible_v<const char (&)[2], sview>);
    static_assert(!is_convertible_v<unsigned char (&)[2], sview>);

    static_assert(is_convertible_v<char *, sview>);
    static_assert(!is_convertible_v<unsigned char *, sview>);
  }

  {
    char a[] = {'a', 'b', 'c', 'd', '\0'};
    const char b[] = "abcd";

    auto r = sref(a);
    reverse(r);
    assert(equal(r, seq('d', 'c', 'b', 'a')));
    assert(equal(a, seq('d', 'c', 'b', 'a', '\0')));
    reverse(r);
    assert(!equal(r, a));
    assert(equal(r, begin(a)));

    auto v = sview(b);
    assert(equal(v, seq('a', 'b', 'c', 'd')));
    assert(equal(rrng(v), seq('d', 'c', 'b', 'a')));
    assert(!empty(v));
    assert(size(v) == 4);
    sview vv = nullptr;
    assert(empty(vv));
    assert(size(vv) == 0);
  }
  {
    vector<char> v = {'a', 'b', 'c'};
    vector<char> v2 = {'a', 'b', 'c', '\0'};
    char a[] = {'a', 'b'};
    char a2[] = {'a', 'b', 'c', '\0'};

    sref s = v;
    assert(equal(s, seq('a', 'b', 'c')));
    assert(equal(rrng(s), rrng(seq('a', 'b', 'c'))));
    assert(!s.empty());
    assert(s.size() == 3);
    s = rng(nth(v, 1), 1);
    assert(!s.empty());
    assert(s.size() == 1);
    assert(s[0] == 'b');

    s = v2;
    assert(equal(s, seq('a', 'b', 'c', '\0')));

    s = a;
    assert(equal(s, seq('a', 'b')));

    s = a2;
    assert(equal(s, seq('a', 'b', 'c')));

    s = {v.begin(), v.end()};
    assert(equal(s, v));

    s = {v, 1, 2};
    assert(equal(s, seq('b', 'c')));

    sview sv = s;
    assert(equal(sv, seq('b', 'c')));
  }

  {
    char s[] = {'a', 'b', 'c', 'd',
                'a', 'b', 'c', 'd'};
    assert(sview(begin(s), 3) == sview(s, 4, 3));
    assert(sview(begin(s), 3) == sref(s, 4, 3));
    assert(sref(begin(s), 3) == sview(s, 4, 3));
    assert(sref(begin(s), 3) == sref(s, 4, 3));
    assert(sview(begin(s), 3) == string("abc"));
    assert(sview(begin(s), 3) < sview(s, 4, 4));
    assert(sview(begin(s), 3) < sref(s, 4, 4));
    assert(sref(begin(s), 3) < sview(s, 4, 4));
    assert(sref(begin(s), 3) < sref(s, 4, 4));
    assert(!(sview(s, 0, 3) < sview(s, 4, 3)));
    assert(!(sview(s, 0, 3) < sview(s, 4, 2)));
    test_lessness(sview(s, 0, 3), sview(s, 3, 3));
    test_lessness(sview("abcd"), sview("abcdd"));
    test_lessness(sview("abcd"), sview("abce"));
    test_lessness(sview("abcd"), sview("accd"));

    test_lessness(sview("abc"), sso_string<char, 8>("abd"));
    test_lessness(sso_string<char, 8>("abc"), sview("abd"));
    test_lessness(sview("abc"), basic_string<char>("abd"));
    test_lessness(basic_string<char>("abc"), sview("abd"));
    test_lessness(sview("abc"), "abd");
    test_lessness("abc", sview("abd"));
  }
}

void test_string_compare() {
  auto r0 = seq('a', 'b', 'c', 'd');
  auto r1 = join_rng(seq(inner_cartesian_product_rng(ez_vector{r0}),
                         inner_cartesian_product_rng(ez_vector{r0, r0}),
                         inner_cartesian_product_rng(ez_vector{r0, r0, r0}),
                         inner_cartesian_product_rng
                         (ez_vector{r0, r0, r0, r0})));
  auto r2 = r1;
  for (decltype(auto) x : r1)
    for (decltype(auto) y : r2) {
      char a[5] = {};
      char aa[5] = {};
      fill_zero(rng(copy(x, begin(a)), end(a)));
      fill_zero(rng(copy(y, begin(aa)), end(aa)));
      const auto p = find(a, 0);
      const auto pp = find(aa, 0);

      sview b(rng(begin(a), p));
      sview bb(rng(begin(aa), pp));

      string c(b);
      string cc(bb);

      assert(equal(rng(begin(a), prev(end(a))), bb)
             == (a == bb));
      assert(equal(b, rng(begin(aa), prev(end(aa))))
             == (b == aa));
      assert(lexicographical_synth_3way(rng(begin(a), prev(end(a))), bb)
             == (a <=> bb));
      assert(lexicographical_synth_3way(b, rng(begin(aa), prev(end(aa))))
             == (b <=> aa));

      assert(equal(rng(begin(a), prev(end(a))), cc)
             == (a == cc));
      assert(equal(c, rng(begin(aa), prev(end(aa))))
             == (c == aa));
      assert(lexicographical_synth_3way(rng(begin(a), prev(end(a))), cc)
             == (a <=> cc));
      assert(lexicographical_synth_3way(c, rng(begin(aa), prev(end(aa))))
             == (c <=> aa));

      const auto z = (b == bb);
      assert((b == cc) == z);
      assert((c == bb) == z);
      assert((c == cc) == z);

      const auto o = (b <=> bb);
      assert((b <=> cc) == o);
      assert((c <=> bb) == o);
      assert((c <=> cc) == o);
    }

  {
    using str_t = basic_string<char>;
    less<str_t> cmp{};
    str_t s = "abc";
    const char a[] = "abd";
    sview sv("abe");
    assert(cmp(s, a));
    assert(cmp(s, sv));
    assert(cmp(a, sv));
  }
  {
    using str_t = sso_string<char, 15>;
    less<str_t> cmp{};
    str_t s = "abc";
    const char a[] = "abd";
    sview sv("abe");
    assert(cmp(s, a));
    assert(cmp(s, sv));
    assert(cmp(a, sv));
  }
}

void test_utf8_utf16_utf32_conversion() {
  // utf16->utf32 conversion rule
  {
    // >= 0x10000
    // 1'0000'0000'0000'0000'0000
    // 1'1111'1111'1111'1111'1111
    const uint32_t j = 0x20bb7u - 0x10000u;
    const uint32_t h = (uint32_t)(j >> (uint32_t)10u);
    const uint32_t l = (uint32_t)((uint32_t)((uint32_t)(j << (uint32_t)22u))
                                  >> (uint32_t)22u);
    const char16_t word_1 = h + 0xd800u;
    const char16_t word_2 = l + 0xdc00u;

    u16sview v = u"𠮷";
    assert(v.size() == 2u);
    assert(v[0] == word_1 && v[1] == word_2);

    u32sview v2 = U"𠮷";
    assert(v2.size() == 1u);

    assert(v2[0] == 0x20bb7u);

    u16string s(u"一二abc三𠮷");
    u16string s2;
    u32string ss(U"一二abc三𠮷");
    u32string ss2;

    assert(s2.assign_unicode(ss));
    assert(s == s2);
    assert(ss2.assign_unicode(s));
    assert(ss == ss2);

    string s3;
    assert(s3.assign_unicode(ss2));
    assert(s3 == "一二abc三𠮷");
    assert(ss2.assign_unicode(s3));
    assert(ss == ss2);
  }

  // inner::fns::unicode_sprint_impl_get_iter_pair
  {
    const sview v = "view";
    const char *p = "pointer";
    assert(equal(inner::fns::unicode_sprint_impl_get_iter_pair(v),
                 "view"_sv));
    assert(equal(inner::fns::unicode_sprint_impl_get_iter_pair(p),
                 "pointer"_sv));
  }

  // utf8-utf16-utf32 conversion
  {
    rander<mt19937> r;

    //r(1u, 0xd800u)
    for (int i : irng(1, 1000)) {
      string s, sb, sc, sd;
      u16string s2, s2b, s2c, s2d;
      u32string s3, s3b, s3c, s3d;

      for (int j : irng(0, i)) {
        uint32_t k = 0;
        for (;;) {
          k = r(1u, 0x10ffffu);
          if (!(k >= 0xd800u && k <= 0xdfffu))
            break;
        }
        s3.push_back(k);
      }
      assert(s.assign_unicode(s3));
      assert(s3b.assign_unicode(s));
      assert(s3 == s3b);
      assert(s2.assign_unicode(s3));
      assert(s3c.assign_unicode(s2));
      assert(s3 == s3c);

      assert(sb.assign_unicode(s2));
      assert(s2b.assign_unicode(sb));
      assert(s2 == s2b);

      assert(s2c.assign_unicode(s));
      assert(sc.assign_unicode(s2c));
      assert(s == sc);

      assert(sd.assign_unicode(s) && s == sd);
      assert(s2d.assign_unicode(s2) && s2 == s2d);
      assert(s3d.assign_unicode(s3) && s3 == s3d);
    }

    const uint32_t a[] = {
      1u, 2u, 3u, 0xe000u, 0xe001u, 0xe002u, 0x10fffd, 0x10fffe, 0x10ffff
    };
    for (int i : irng(1, 100)) {
      string s, sb, sc, sd;
      u16string s2, s2b, s2c, s2d;
      u32string s3, s3b, s3c, s3d;

      for (int c : irng(0, i))
        s3.push_back(a[r(1u, size(a)) - 1u]);

      assert(s.assign_unicode(s3));
      assert(s3b.assign_unicode(s));
      assert(s3 == s3b);
      assert(s2.assign_unicode(s3));
      assert(s3c.assign_unicode(s2));
      assert(s3 == s3c);

      assert(sb.assign_unicode(s2));
      assert(s2b.assign_unicode(sb));
      assert(s2 == s2b);

      assert(s2c.assign_unicode(s));
      assert(sc.assign_unicode(s2c));
      assert(s == sc);

      assert(sd.assign_unicode(s) && s == sd);
      assert(s2d.assign_unicode(s2) && s2 == s2d);
      assert(s3d.assign_unicode(s3) && s3 == s3d);
    }
  }
}

void test_string2() {
  printf("container - sso_string: ");

  inner::fns::test_sso_string();
  inner::fns::test_string_reference();
  inner::fns::test_string_compare();
  inner::fns::test_utf8_utf16_utf32_conversion();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_string2();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
