//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/random.h>
#include <re/container.h>

#include <cassert>

using namespace re;

template <class S, class F, class F2>
void test_flat_set_common_operations_impl(F mk, F2 key) {
  // special member functions
  {
    static_assert(regular<S>);
    static_assert(is_nothrow_movable_v<S>);
    static_assert(is_nothrow_swappable_v<S>);
    S s1, s2;
    s1.insert(mk(1));
    s2.insert(mk(2));
    adl_swap(s1, s2);
    assert(equal(s1, seq(mk(2))));
    assert(equal(s2, seq(mk(1))));
  }

  // extract
  // replace
  {
    S s;
    s.insert(mk(1));
    s.insert(mk(2));
    assert(equal(move(s).extract(), bind_rng(irng(1, 3), mk)));
    assert(empty(s));
    s.replace(typename S::container_type{mk(1), mk(2)});
    assert(equal(s, seq(mk(1), mk(2))));
  }

  // container
  {
    S s;
    assert(s.begin() == s.end());
    assert(s.cbegin() == s.end());
    assert(s.begin() == s.cend());
    assert(s.cbegin() == s.cend());
    assert(s.empty());
    assert(s.size() == 0);
    assert(s.size() < s.max_size());
    s.insert(mk(3));
    s.insert(mk(1));
    s.insert(mk(2));
    assert(equal(s, seq(mk(1), mk(2), mk(3))));
    assert(equal(as_const(s), s));
    assert(s == s);
    assert((s <=> s) == 0);
  }

  // constructor and assignment
  {
    using alloc_t = typename S::allocator_type;
    using less_t = typename S::key_compare;
    alloc_t al1, al2;
    less_t cmp;

    // special member functions
    {
      static_assert(regular<S>);
      static_assert(swappable<S>);
      static_assert(equality_comparable<S>);
      static_assert(three_way_comparable<S>);
      static_assert(is_nothrow_movable_v<S>);
      static_assert(is_nothrow_swappable_v<S>);

      S s(al1);
      assert(s.empty() && s.get_allocator() == al1);
      s.append(mk(2), mk(1));
      assert(equal(s, seq(mk(1), mk(2))));
      S s2(s, al2);
      assert(s2 == s && s2.get_allocator() == al2);
      S s3(move(s2), al1);
      assert(equal(s3, seq(mk(1), mk(2))) && s3.get_allocator() == al1);
      assert(s2.empty());
    }
    // cmp
    {
      S s(cmp, al1);
      assert(s.empty());
      assert(s.get_allocator() == al1);
      S s2(cmp);
      assert(s2.empty());
    }
    // [from, to)
    {
      decltype(mk(1)) a[] = {mk(1), mk(2), mk(3)};
      S s(begin(a), end(a), cmp, al1);
      assert(equal(s, a) && s.get_allocator() == al1);
      S s2(begin(a), end(a), al2);
      assert(s2 == s && s2.get_allocator() == al2);
      S s3(begin(a), end(a));
      assert(s3 == s2);
      s3.assign(begin(a), next(begin(a)));
      assert(equal(s3, bind_rng(irng(1, 2), mk)));
    }
    // il
    {
      S s({mk(1), mk(2), mk(3)}, cmp, al1);
      assert(equal(s, bind_rng(irng(1, 4), mk)) && s.get_allocator() == al1);
      S s2({mk(1), mk(2), mk(3)}, al2);
      assert(equal(s2, bind_rng(irng(1, 4), mk)) && s2.get_allocator() == al2);
      S s3 = {mk(3), mk(2), mk(1)};
      assert(equal(s3, bind_rng(irng(1, 4), mk)));
      s3 = {mk(5), mk(4), mk(6)};
      assert(equal(s3, bind_rng(irng(4, 7), mk)));
      s3.assign({mk(1)});
      assert(equal(s3, bind_rng(irng(1, 2), mk)));
    }
    // range
    {
      S s(seq(mk(1), mk(2), mk(3)), cmp, al1);
      assert(equal(s, bind_rng(irng(1, 4), mk)) && s.get_allocator() == al1);
      S s2(seq(mk(1), mk(2), mk(3)), al2);
      assert(equal(s2, bind_rng(irng(1, 4), mk)) && s2.get_allocator() == al2);
      S s3(seq(mk(3), mk(2), mk(1)));
      assert(equal(s3, bind_rng(irng(1, 4), mk)));
      s3 = bind_rng(seq(5, 4, 6), mk);
      assert(equal(s3, bind_rng(irng(4, 7), mk)));
      s3.assign_range(bind_rng(irng(1, 4), mk));
      assert(equal(s3, bind_rng(irng(1, 4), mk)));
      s3.assign(bind_rng(irng(5, 9), mk));
      assert(equal(s3, bind_rng(irng(5, 9), mk)));
    }
    // from_range
    {
      S s(from_range, bind_rng(seq(1, 2, 3), mk), cmp, al1);
      assert(equal(s, bind_rng(irng(1, 4), mk)) && s.get_allocator() == al1);
      S s2(from_range, bind_rng(seq(1, 2, 3), mk), al2);
      assert(equal(s2, bind_rng(irng(1, 4), mk)) && s2.get_allocator() == al2);
      S s3(from_range, bind_rng(seq(3, 2, 1), mk));
      assert(equal(s3, bind_rng(irng(1, 4), mk)));
    }
  }

  // insert single value
  {
    {
      S s;
      for (int &x : iters(0, 20000))
        s.insert(mk(x));
      assert(equal(s, bind_rng(irng(0, 20000), mk)));
    }
    {
      S s;
      for (int &x : r_iters(0, 20000))
        s.insert(mk(x));
      assert(equal(s, bind_rng(irng(0, 20000), mk)));
    }
    {
      for (int i : irng(1, 8)) {
        ez_vector<int> v;
        copy(irng(0, i), to_back(v));
        while (next_permutation(v)) {
          S s;
          for (auto x : bind_rng(v, mk))
            s.insert(x);
          assert(equal(s, bind_rng(irng(0, i), mk)));
        }
      }
    }
    {
      minstd_rand g{};
      constexpr int size_lim = 1024;
      constexpr int repeat_lim = 1024;
      for (int c = repeat_lim; c != 0; --c) {
        ez_vector<decltype(mk(1))> v;
        copy(bind_rng(irng(0, size_lim), mk), to_back(v));
        if (c <= repeat_lim / 2)
          shuffle(v, g);
        S s;
        for (auto &x : v) {
          if (c % (repeat_lim / 2) < (repeat_lim / 4))
            ((c % 2 == 0) ? s.insert(x) : s.emplace(x));
          else {
            const typename S::iterator it
              = (c % (repeat_lim / 4) < (repeat_lim / 8))
              ? lower_bound(s, x, s.value_comp())
              : ((c % (repeat_lim / 8) < (repeat_lim / 16))
                 ? s.end()
                 : s.begin());
            assert(it == s.end() || *it != x);
            const typename S::iterator it2
              = ((c % 2 == 0) ? s.insert(it, x) : s.emplace_hint(it, x));
            assert(*it2 == x);
          }
          assert(is_sorted(s));
        }
        assert(equal(s, bind_rng(irng(0, size_lim), mk)));
      }
    }
  }
  // insert range
  {
    // insert(from, to)
    // insert(il)
    // insert_range(r)
    // append
    S s;
    {
      const auto tmp = bind_rng(irng(0, 5), mk);
      s.insert(begin(tmp), end(tmp));
      assert(equal(s, tmp));
    }
    s.insert({mk(-1), mk(5)});
    assert(equal(s, bind_rng(irng(-1, 6), mk)));

    s.clear();
    s.append(seq(mk(1), mk(5)), seq(mk(3)), mk(2), seq(mk(6), mk(4), mk(0)));
    assert(equal(s, bind_rng(irng(0, 7), mk)));

    s.clear();
    s.insert_range(seq(mk(1), mk(5)));
    s.insert_range(seq(mk(3)));
    s.insert_range(seq(mk(2)));
    s.insert_range(seq(mk(6), mk(4), mk(0)));
    assert(equal(s, bind_rng(irng(0, 7), mk)));
  }

  // erase
  // clear
  // remove
  // remove_if
  {
    const auto init_s = [mk](S &s) {
      for (int &x : iters(0, 100))
        s.insert(s.end(), mk(x));
      assert(equal(s, bind_rng(irng(0, 100), mk)));
    };
    // erase(from, to)
    // clear()
    {
      S s;
      init_s(s);
      s.erase(s.begin(), s.end());
      assert(s.empty());
      init_s(s);
      s.clear();
      assert(s.empty());
    }
    // erase(it)
    {
      rander<minstd_rand> g;
      S s;
      ez_vector<decltype(mk(1))> v;

      for (int c = 200; c != 0; --c) {
        assert(s.empty());
        init_s(s);
        while (!s.empty()) {
          v.erase(v.begin(), v.end());
          copy(s, to_back(v));
          const auto k = g(0, to_signed(s.size()) - 1);
          v.erase(nth(v, k));
          s.erase(nth(s, k));
          assert(equal(s, v));
        }
        assert(v.empty());
      }
    }
    // erase(key)
    // remove(key)
    // remove_if(key)
    {
      rander<minstd_rand> g;
      S s;
      ez_vector<decltype(mk(1))> v;

      for (int c = 200; c != 0; --c) {
        assert(s.empty());
        init_s(s);
        while (!s.empty()) {
          v.erase(v.begin(), v.end());
          copy(s, to_back(v));
          const auto j = g(0, to_signed(s.size()) - 1);
          const auto k = key(*nth(s, j));
          v.erase(nth(v, j));
          if (c < 100) {
            if (c % 2 == 0) {
              assert(s.erase(k) == 1);
              assert(s.erase(k) == 0);
            }
            else {
              assert(s.remove(k) == 1);
              assert(s.remove(k) == 0);
            }
          }
          else {
            const auto eq = [key, k](const auto &x) {return key(x) == k;};
            assert(s.remove_if(eq) == 1);
            assert(s.remove_if(eq) == 0);
          }
          assert(equal(s, v));
        }
        assert(v.empty());
      }
    }
  }

  // key_compare
  // key_comp
  // value_compare
  // value_comp
  {
    S s;
    const typename S::key_compare cmp1 = s.key_comp();
    const typename S::value_compare cmp2 = s.value_comp();
    using key_t = typename S::key_type;
    assert(cmp1(key_t(1), key_t(2)));
    assert(!cmp1(key_t(1), key_t(1)));
    assert(!cmp1(key_t(2), key_t(1)));
    assert(cmp2(mk(1), mk(2)));
    assert(!cmp2(mk(1), mk(1)));
    assert(!cmp2(mk(2), mk(1)));
  }

  // find
  // contains
  // count
  // lower/upper_bound
  // equal_range
  // find_range
  {
    using key_t = typename S::key_type;
    S s(from_range, bind_rng(irng(0, 100), mk));
    for (int &i : iters(0, 100)) {
      const typename S::iterator it = s.find(key_t(i));
      assert(it != s.end() && *it == mk(i));
      const typename S::const_iterator cit = as_const(s).find(key_t(i));
      assert(cit != s.cend() && *cit == mk(i));

      assert(s.contains(key_t(i)));
      assert(as_const(s).contains(key_t(i)));

      assert(s.count(key_t(i)) == 1);
      assert(as_const(s).count(key_t(i)) == 1);

      assert(s.lower_bound(key_t(i)) == it);
      assert(as_const(s).lower_bound(key_t(i)) == it);
      assert(s.upper_bound(key_t(i)) == next(it));
      assert(as_const(s).upper_bound(key_t(i)) == next(it));
      assert(s.equal_range(key_t(i)) == iter_pair(it, next(it)));
      assert(as_const(s).equal_range(key_t(i)) == iter_pair(it, next(it)));
      assert(s.find_range(key_t(i), key_t(i)) == iter_pair(it, next(it)));
      assert(as_const(s).find_range(key_t(i), key_t(i))
             == iter_pair(it, next(it)));
    }

    assert(s.find(key_t(-1)) == s.end());
    assert(s.find(key_t(-10)) == s.end());
    assert(s.find(key_t(100)) == s.end());
    assert(s.find(key_t(1000)) == s.end());
    assert(as_const(s).find(key_t(-1)) == s.end());
    assert(as_const(s).find(key_t(-10)) == s.end());
    assert(as_const(s).find(key_t(100)) == s.end());
    assert(as_const(s).find(key_t(1000)) == s.end());

    assert(!s.contains(key_t(-1)));
    assert(!s.contains(key_t(-10)));
    assert(!s.contains(key_t(100)));
    assert(!s.contains(key_t(1000)));
    assert(!as_const(s).contains(key_t(-1)));
    assert(!as_const(s).contains(key_t(-10)));
    assert(!as_const(s).contains(key_t(100)));
    assert(!as_const(s).contains(key_t(1000)));

    assert(s.count(key_t(-1)) == 0);
    assert(s.count(key_t(-10)) == 0);
    assert(s.count(key_t(100)) == 0);
    assert(s.count(key_t(1000)) == 0);
    assert(as_const(s).count(key_t(-1)) == 0);
    assert(as_const(s).count(key_t(-10)) == 0);
    assert(as_const(s).count(key_t(100)) == 0);
    assert(as_const(s).count(key_t(1000)) == 0);

    assert(s.lower_bound(key_t(-1)) == s.begin());
    assert(s.lower_bound(key_t(-10)) == s.begin());
    assert(s.lower_bound(key_t(100)) == s.end());
    assert(s.lower_bound(key_t(1000)) == s.end());
    assert(as_const(s).lower_bound(key_t(-1)) == s.begin());
    assert(as_const(s).lower_bound(key_t(-10)) == s.begin());
    assert(as_const(s).lower_bound(key_t(100)) == s.end());
    assert(as_const(s).lower_bound(key_t(1000)) == s.end());

    assert(s.upper_bound(key_t(-1)) == s.begin());
    assert(s.upper_bound(key_t(-10)) == s.begin());
    assert(s.upper_bound(key_t(100)) == s.end());
    assert(s.upper_bound(key_t(1000)) == s.end());
    assert(as_const(s).upper_bound(key_t(-1)) == s.begin());
    assert(as_const(s).upper_bound(key_t(-10)) == s.begin());
    assert(as_const(s).upper_bound(key_t(100)) == s.end());
    assert(as_const(s).upper_bound(key_t(1000)) == s.end());

    s.clear();
    s.insert_range(bind_rng(seq(1, 2, 5, 7), mk));
    assert(*s.lower_bound(key_t(3)) == mk(5));
    assert(*s.upper_bound(key_t(3)) == mk(5));
    assert(*s.lower_bound(key_t(4)) == mk(5));
    assert(*s.upper_bound(key_t(4)) == mk(5));
    assert(*s.lower_bound(key_t(5)) == mk(5));
    assert(*s.upper_bound(key_t(5)) == mk(7));
  }

  // front
  // back
  {
    S s;
    s.insert(mk(1));
    assert(s.front() == mk(1));
    assert(s.back() == mk(1));
    assert(as_const(s).front() == mk(1));
    assert(as_const(s).back() == mk(1));

    s.clear();
    s.insert_range(bind_rng(irng(0, 100), mk));
    assert(s.front() == mk(0));
    assert(s.back() == mk(99));
    assert(as_const(s).front() == mk(0));
    assert(as_const(s).back() == mk(99));
  }

  // unique
  {
    {
      S s;
      s.unique();
      assert(s.empty());
    }
    {
      S s;
      s.insert_range(bind_rng(irng(0, 50), mk));
      s.insert_range(bind_rng(irng(0, 50), mk));
      s.insert_range(bind_rng(irng(0, 50), mk));
      s.unique();
      assert(equal(s, bind_rng(irng(0, 50), mk)));
    }
  }
}
void test_flat_set_common_operations() {
  test_flat_set_common_operations_impl<flat_set
                                       <int, less<int>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_common_operations_impl<flat_multiset
                                       <int, less<int>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_common_operations_impl<flat_map
                                       <int, int, less<int>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);
  test_flat_set_common_operations_impl<flat_multimap
                                       <int, int, less<int>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);

  test_flat_set_common_operations_impl<flat_set
                                       <int, less<>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_common_operations_impl<flat_multiset
                                       <int, less<>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_common_operations_impl<flat_map
                                       <int, int, less<>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);
  test_flat_set_common_operations_impl<flat_multimap
                                       <int, int, less<>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S, class F, class F2>
void test_flat_set_unique_operations_impl(F mk, F2 key) {
  // duplicate insert
  {
    S s;
    for (int &i : iters(0, 50)) {
      const auto x = ((i % 2 == 0) ? s.insert(mk(i)) : s.emplace(mk(i)));
      assert(x.second);
      assert(*x.first == mk(i));
      assert(equal(s, bind_rng(irng(0, i + 1), mk)));
      for (int &j : iters(0, i + 1)) {
        const auto y = ((i % 2 == 0) ? s.insert(mk(j)) : s.emplace(mk(j)));
        assert(!y.second);
        assert(*y.first == mk(j));

        const auto z = ((i % 2 == 0)
                        ? s.insert(s.upper_bound(key(mk(j))), mk(j))
                        : s.emplace_hint(s.upper_bound(key(mk(j))), mk(j)));
        assert(*z == mk(j));

        const auto q = ((i % 2 == 0)
                        ? s.insert(s.begin(), mk(j))
                        : s.emplace_hint(s.begin(), mk(j)));
        assert(*q == mk(j));

        const auto w = ((i % 2 == 0)
                        ? s.insert(s.end(), mk(j))
                        : s.emplace_hint(s.end(), mk(j)));
        assert(*w == mk(j));
      }
      assert(equal(s, bind_rng(irng(0, i + 1), mk)));
    }
  }
  // merge
  {
    for (decltype(auto) x
           : join_rng(bind_rng
                      (irng(0, 5),
                       bind(combination_rng, irng(0, 10), _1))))
      for (decltype(auto) y
           : join_rng(bind_rng
                      (irng(0, 5),
                       bind(combination_rng, irng(0, 10), _1)))) {
        S s(from_range, bind_rng(x, mk));
        S s2(from_range, bind_rng(y, mk));
        S s3 = s;
        s3.merge(s2);
        assert(is_sorted(s3, bind(less{}, bind(key, _1), bind(key, _2))));
        assert(includes(s3, s) && includes(s3, s2)
               && adjacent_find(s3) == s3.end());
      }
  }
}
void test_flat_set_unique_operations() {
  test_flat_set_unique_operations_impl<flat_set
                                       <int, less<int>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_unique_operations_impl<flat_map
                                       <int, int, less<int>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);

  test_flat_set_unique_operations_impl<flat_set
                                       <int, less<>,
                                        vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_unique_operations_impl<flat_map
                                       <int, int, less<>,
                                        vector<pair<int, int>,
                                               test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S, class F, class F2>
void test_flat_set_equal_operations_impl(F mk, F2 key) {
  // duplicate insert
  {
    // insert sorted
    {
      S s;
      for (int i : irng(0, 1000)) {
        typename S::iterator it = s.insert(mk(i));
        assert(*it == mk(i));
      }
      for (int i : irng(0, 1000)) {
        typename S::iterator it = s.insert(mk(i));
        assert(*it == mk(i));
        assert(it != s.begin() && *prev(it) == mk(i));
      }
      for (int i : irng(0, 1000)) {
        typename S::iterator it = s.insert(mk(i));
        assert(*it == mk(i));
        assert(it != s.begin() && *prev(it) == mk(i));
        assert(prev(it) != s.begin() && *prev(it, 2) == mk(i));
      }
    }
    // insert randomized
    {
      {
        minstd_rand g;
        auto v1 = copy(irng(0, 1000), to_back(ez_vector<int>{})).base();
        auto v2 = copy(irng(0, 1000), to_back(ez_vector<int>{})).base();
        auto v3 = copy(irng(0, 1000), to_back(ez_vector<int>{})).base();
        for (int c = 50; c != 0; --c) {
          shuffle(v1, g);
          shuffle(v2, g);
          shuffle(v3, g);
          S s;
          for (int i : v1) {
            typename S::iterator it = s.insert(mk(i));
            assert(*it == mk(i));
          }
          for (int i : v2) {
            typename S::iterator it = s.insert(mk(i));
            assert(*it == mk(i));
            assert(it != s.begin() && *prev(it) == mk(i));
          }
          for (int i : v3) {
            typename S::iterator it = s.insert(mk(i));
            assert(*it == mk(i));
            assert(it != s.begin() && *prev(it) == mk(i));
            assert(prev(it) != s.begin() && *prev(it, 2) == mk(i));
          }
        }
      }
      {
        minstd_rand g;
        ez_vector<int> v;
        for (int c = 3; c != 0; --c)
          copy(irng(0, 1000), to_back(v));
        for (int c = 50; c != 0; --c) {
          shuffle(v, g);
          S s;
          for (int i : v) {
            typename S::iterator it = s.insert(mk(i));
            assert(*it == mk(i));
          }
          assert(equal
                 (s, join_rng(bind_rng(irng(0, 1000),
                                       [mk](int x) {return rng(3, mk(x));}))));
        }
      }
    }
  }
  // duplicate erase
  {
    S s;
    for (int c = 3; c != 0; --c)
      for (auto x : bind_rng(irng(0, 1000), mk))
        assert(*s.insert(x) == x);
    for (int i : irng(0, 1000))
      assert(s.erase(i) == 3);
    assert(s.empty());
  }

  // find series
  {
    S s;
    for (int c = 3; c != 0; --c)
      s.insert_range(bind_rng(irng(0, 1000), mk));
    for (int i : irng(0, 1000)) {
      const auto it = s.find(key(mk(i)));
      assert(it != s.end());
      assert(next(it, 2) != s.end());
      assert(equal(rng(it, 3), rng(3, mk(i))));

      assert(s.contains(key(mk(i))));
      assert(s.count(key(mk(i))) == 3);

      const auto l = s.lower_bound(key(mk(i)));
      const auto u = s.upper_bound(key(mk(i)));
      assert(s.equal_range(i) == rng(l, u));
      assert(s.find_range(i, i) == rng(l, u));
    }
  }

  // merge
  {
    S s;
    S s2;
    copy(bind_rng(irng(0, 3), mk), to_back(s));
    copy(bind_rng(irng(0, 3), mk), to_back(s2));
    s.merge(s2);
    assert(equal(s, bind_rng(seq(0, 0, 1, 1, 2, 2), mk)));
    assert(equal(s2, bind_rng(seq(0, 1, 2), mk)));
    s.merge(move(s2));
    assert(equal(s, bind_rng(seq(0, 0, 0, 1, 1, 1, 2, 2, 2), mk)));
    assert(s2.empty());
  }
}
void test_flat_set_equal_operations() {
  test_flat_set_equal_operations_impl<flat_multiset
                                      <int, less<int>,
                                       vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_equal_operations_impl<flat_multimap
                                      <int, int, less<int>,
                                       vector<pair<int, int>,
                                              test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);

  test_flat_set_equal_operations_impl<flat_multiset
                                      <int, less<>,
                                       vector<int, test_allocator<int>>>>
    (copy, identity{});
  test_flat_set_equal_operations_impl<flat_multimap
                                      <int, int, less<>,
                                       vector<pair<int, int>,
                                              test_allocator<pair<int, int>>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S>
void test_flat_map_specialized_operations_impl() {
  const auto mk = bind(make_pair, _1, _1);
  // try_emplace
  {
    {
      S s;
      auto [it, y] = s.try_emplace(1, 2);
      assert(*it == pair(1, 2) && y);
      tie(it, y) = s.try_emplace(1, 2);
      assert(*it == pair(1, 2) && !y);
    }
    {
      S s;
      for (int i : irng(0, 1000)) {
        auto [it, y] = s.try_emplace(i, i);
        assert(*it == pair(i, i) && y);
      }
      for (int i : irng(0, 1000)) {
        auto [it, y] = s.try_emplace(i, i + 1);
        assert(*it == pair(i, i) && !y);
      }
    }
    {
      S s;
      for (int i : r_iters(0, 1000)) {
        auto [it, y] = s.try_emplace(i, i);
        assert(*it == pair(i, i) && y);
      }
      for (int i : irng(0, 1000)) {
        auto [it, y] = s.try_emplace(i, i + 1);
        assert(*it == pair(i, i) && !y);
      }
    }
    {
      S s;
      ez_vector<int> v;
      copy(irng(0, 1000), to_back(v));
      minstd_rand g;
      for (int c = 50; c != 0; --c) {
        shuffle(v, g);
        s.clear();
        for (int i : v) {
          auto [it, y] = s.try_emplace(i, i);
          assert(*it == pair(i, i) && y);
        }
        for (int i : irng(0, 1000)) {
          auto [it, y] = s.try_emplace(i, i + 1);
          assert(*it == pair(i, i) && !y);
        }
      }
    }
  }
  // try_emplace_hint
  {
    {
      S s;
      for (int i : irng(0, 1000)) {
        const typename S::iterator it = s.try_emplace(s.upper_bound(i), i, i);
        assert(*it == pair(i, i));
      }
      assert(equal(s, bind_rng(irng(0, 1000), bind(make_pair, _1, _1))));
      for (int i : irng(0, 1000)) {
        const typename S::iterator it = s.try_emplace(s.upper_bound(i), i, i);
        assert(*it == pair(i, i));
      }
      assert(equal(s, bind_rng(irng(0, 1000), bind(make_pair, _1, _1))));
    }
    {
      minstd_rand g;
      for (int i : irng(0, 101))
        for (int j : irng(-1, 201)) {
          S s;
          for (int x : irng(0, 200)) {
            if (x % 2 == 0)
              s.insert(pair(x, x));
          }
          const typename S::iterator it = s.try_emplace(nth(s, i), j, j);
          assert(*it == pair(j, j));
          if (j >= 0 && j <= 199 && (j % 2 == 0))
            assert(equal(s, bind_rng(aligned_stride_rng(irng(0, 200), 2),
                                     bind(make_pair, _1, _1))));
          else {
            assert(s.size() == 101);
            const auto it2 = (it == s.end() ? s.end() : next(it));
            assert(equal(join_rng(seq(rng(s.begin(), it), rng(it2, s.end()))),
                         bind_rng(aligned_stride_rng(irng(0, 200), 2),
                                  bind(make_pair, _1, _1))));
          }
        }
    }
  }
  // insert_or_assign
  {
    S s;
    for (int i : irng(0, 10)) {
      auto [it, y] = s.insert_or_assign(i, i);
      assert(*it == pair(i, i) && y);
    }
    for (int i : irng(0, 10)) {
      auto [it, y] = s.insert_or_assign(i, i + 1);
      assert(*it == pair(i, i + 1) && !y);
    }
    assert(equal(s, bind_rng(irng(0, 10),
                             bind(make_pair, _1, bind(plus{}, _1, 1)))));
  }
  // insert_or_assign_hint
  {
    S s;
    for (int i : irng(0, 10)) {
      auto it = s.insert_or_assign(s.end(), i, i);
      assert(*it == pair(i, i));
    }
    for (int i : irng(0, 10)) {
      auto it = s.insert_or_assign(s.upper_bound(i), i, i + 1);
      assert(*it == pair(i, i + 1));
    }
    assert(equal(s, bind_rng(irng(0, 10),
                             bind(make_pair, _1, bind(plus{}, _1, 1)))));
  }
  // []
  {
    S s;
    for (int i : irng(0, 10))
      s[i] = i;
    for (int i : irng(0, 10))
      s[i] = i + 1;
    assert(equal(s, bind_rng(irng(0, 10),
                             bind(make_pair, _1, bind(plus{}, _1, 1)))));
    for (int i : irng(0, 10))
      assert(as_const(s)[i] == i + 1);
  }
  // at
  {
    S s;
    for (int i : irng(0, 10))
      s[i] = i;
    for (int i : irng(0, 10)) {
      assert(s.at(i) == i);
      assert(as_const(s).at(i) == i);
    }
#ifndef RE_NOEXCEPT
    assert(test_throwing<out_of_range>([&]() {s.at(10);}));
    assert(test_throwing<out_of_range>([&]() {as_const(s).at(10);}));
#endif
  }
}
void test_flat_map_specialized_operations() {
  test_flat_map_specialized_operations_impl
    <flat_map<int, int, less<int>,
              vector<pair<int, int>, test_allocator<pair<int, int>>>>>();
  test_flat_map_specialized_operations_impl
    <flat_map<int, int, less<>,
              vector<pair<int, int>, test_allocator<pair<int, int>>>>>();
}

void test_flat_map() {
  printf("flat_map: ");

  test_flat_set_common_operations();
  test_flat_set_unique_operations();
  test_flat_set_equal_operations();
  test_flat_map_specialized_operations();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_flat_map();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
