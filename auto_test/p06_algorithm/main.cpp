#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>

#include <cassert>

namespace re::inner::test {

void test_equal() {
  const iter_pair<const int *> z{};
  const int a[] = {1, 2, 3};
  const int b[] = {1, 2, 3, 4};
  const int c[] = {3, 2, 1};

  assert(equal(z, z));
  assert(equal(z, z, equal_to<>{}));
  assert(!equal(a, z, equal_to<>{}));
  assert(!equal(z, a, equal_to<>{}));
  assert(equal(z, begin(z)));
  assert(equal(z, begin(z), equal_to<>{}));
  assert(equal(z, begin(a), equal_to<>{}));

  assert(equal(a, a));
  assert(equal(a, a, equal_to<>{}));
  assert(equal(a, begin(a)));
  assert(equal(a, begin(a), equal_to<>{}));

  assert(equal(a, begin(b)));
  assert(equal(a, begin(b), equal_to<>{}));
  assert(!equal(a, b));
  assert(!equal(a, b, equal_to<>{}));
  assert(!equal(b, a));
  assert(!equal(b, a, equal_to<>{}));

  assert(!equal(a, begin(c)));
  assert(!equal(a, begin(c), equal_to<>{}));
  assert(!equal(a, c));
  assert(!equal(a, c, equal_to<>{}));
  assert(!equal(b, c));
  assert(!equal(b, c, equal_to<>{}));
  assert(!equal(c, b));
  assert(!equal(c, b, equal_to<>{}));

  {
    ez_list<int> l = {1, 2, 3, 4};
    for (int i : irng(0, 4)) {
      ez_list<int> ll = l;
      assert(equal(rng(l), rng(ll)));
      ++*nth(ll, i);
      assert(!equal(rng(l), rng(ll)));
    }
    for (int i : irng(0, 4)) {
      ez_list<int> ll = l;
      assert(!equal(rng(l), rng(begin(ll), nth(ll, i))));
    }
  }

  {
    constexpr int v[] = {1, 2, 3};
    static_assert(equal(v, v));
    static_assert(equal(v, begin(v)));
  }
}
void test_allanynone_of() {
  const iter_pair<const int *> z{};
  assert(all_of(z, bind(equal_to<>{}, _1, 1)));
  assert(none_of(z, bind(equal_to<>{}, _1, 1)));
  assert(!any_of(z, bind(equal_to<>{}, _1, 1)));

  const int a[] = {1, 1, 1};
  assert(all_of(a, bind(equal_to<>{}, _1, 1)));
  assert(all_of_equal(a, 1));
  assert(all_of_equal(a, 1, equal_to<>{}));
  assert(!all_of_equal(a, 2));
  assert(none_of_equal(a, 2));
  assert(!any_of_equal(a, 2));
  for (int &i : iters(0, 3)) {
    int aa[] = {1, 1, 1};
    ++aa[i];
    assert(!all_of_equal(aa, 1));
    assert(!none_of_equal(aa, 2));
    assert(any_of_equal(aa, 2));
  }

  {
    constexpr int v[] = {1, 1, 1};
    static_assert(all_of_equal(v, 1));
    static_assert(none_of_equal(v, 2));
    static_assert(any_of_equal(v, 1));
  }
}
void test_for_each() {
  const iter_pair<int *> z{};
  {
    int *const i = for_each(z, [](int &x) {++x;});
    assert(i == nullptr);
  }
  int a[] = {1, 2, 3};
  {
    assert(for_each(a, [](int &x) {x *= 2;}) == end(a));
    assert(equal(a, ez_vector{2, 4, 6}));
  }

  {
    int v[] = {1, 2, 3};
    int vv[] = {2, 3, 4};
    ez_vector<int> vvv;
    const auto f = [&vvv](int &x, int &y) {vvv.insert(vvv.end(), x * 10 + y);};

    assert(for_each(v, vv, f) == pair(end(v), end(vv)));
    assert(equal(vvv, ez_vector{12, 23, 34}));

    vvv.erase(begin(vvv), end(vvv));
    assert(for_each_plus(v, begin(vv), f) == pair(end(v), end(vv)));
    assert(equal(vvv, ez_vector{12, 23, 34}));

    vvv.erase(begin(vvv), end(vvv));
    assert(for_each(v, begin(vv), f) == end(vv));
    assert(equal(vvv, ez_vector{12, 23, 34}));
  }

  // for_each_excluding_first
  {
    ez_vector<int> buf;
    ez_forward_list<int> v;
    const auto f1 = [&](int x) {buf.insert(buf.end(), x);};
    const auto f2 = [&](int x) {buf.insert(buf.end(), x * 100);};
    assert(for_each_excluding_first(v, f1) == end(v));
    assert(for_each_excluding_first(v, f1, f2) == end(v));
    assert(empty(buf) && empty(v));
    for (int &i : iters(0, 10)) {
      v.erase_after(v.before_begin(), v.end());
      copy(irng(1, i + 1), to_front(v));
      assert(equal(v, irng(1, i + 1)));

      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_first(v, f1) == end(v));
      if (2 <= i + 1)
        assert(equal(buf, irng(2, i + 1)));
      else
        assert(empty(buf));

      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_first(v, f1, f2) == end(v));
      if (!empty(v)) {
        assert(front(buf) == 100);
        if (2 <= i + 1)
          assert(equal(rng(next(begin(buf)), end(buf)), irng(2, i + 1)));
        else
          assert(next(begin(buf)) == end(buf));
      }
      else
        assert(empty(buf));
    }
  }
  // for_each_excluding_last
  {
    using l_t = ez_forward_list<int>;
    using ll_t = ez_list<int>;
    l_t l;
    ll_t ll;
    ez_vector<int> buf;
    const auto f1 = [&](int x) {buf.insert(buf.end(), x);};
    const auto f2 = [&](int x) {buf.insert(buf.end(), x + 100);};
    for (int &i : iters(0, 10)) {
      l.erase_after(l.before_begin(), l.end());
      ll.erase(ll.begin(), ll.end());
      copy(irng(0, i), to_front(l));
      copy(irng(0, i), to_front(ll));
      assert(equal(l, ll) && equal(l, irng(0, i)));

      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_last(l, f1) == end(l));
      assert((empty(l) && empty(buf))
             || (size(buf) + 1 == size(l) && equal(buf, begin(l))));
      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_last(ll, f1) == end(ll));
      assert((empty(ll) && empty(buf))
             || (size(buf) + 1 == size(ll) && equal(buf, begin(ll))));

      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_last(l, f1, f2) == end(l));
      assert((empty(l) && empty(buf))
             || (size(buf) == size(l)
                 && equal(rng(begin(buf), prev(end(buf))), begin(l))
                 && back(buf) == (i - 1) + 100));
      buf.erase(buf.begin(), buf.end());
      assert(for_each_excluding_last(ll, f1, f2) == end(ll));
      assert((empty(ll) && empty(buf))
             || (size(buf) == size(ll)
                 && equal(rng(begin(buf), prev(end(buf))), begin(ll))
                 && back(buf) == (i - 1) + 100));
    }
  }

  // for_each_excluding_first_n
  // for_each_excluding_last_n
  {
    const auto test_f = []<class L>(type_tag<L>) {
      // empty
      {
        L r = {};
        int n1 = 0;
        int n2 = 0;
        assert(for_each_excluding_first_n(r, 0,
                                          [&](int) {++n2;},
                                          [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);
        assert(for_each_excluding_first_n(r, 1,
                                          [&](int) {++n2;},
                                          [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);
        assert(for_each_excluding_first_n(r, 3,
                                          [&](int) {++n2;},
                                          [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);

        n1 = n2 = 0;
        assert(for_each_excluding_last_n(r, 0,
                                         [&](int) {++n2;},
                                         [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);
        assert(for_each_excluding_last_n(r, 1,
                                         [&](int) {++n2;},
                                         [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);
        assert(for_each_excluding_last_n(r, 3,
                                         [&](int) {++n2;},
                                         [&](int) {++n1;})
               == end(r));
        assert(n1 == 0 && n2 == 0);
      }
      // single element
      {
        L r = {1};
        int n1 = 0;
        int n2 = 0;
        assert(for_each_excluding_first_n(r, 0,
                                          [&](int i) {n2 += i;},
                                          [&](int i) {n1 += i;})
               == end(r));
        assert(n1 == 0 && n2 == 1);
        n1 = n2 = 0;
        assert(for_each_excluding_first_n(r, 1,
                                          [&](int i) {n2 += i;},
                                          [&](int i) {n1 += i;})
               == end(r));
        assert(n1 == 1 && n2 == 0);
        n1 = n2 = 0;
        assert(for_each_excluding_first_n(r, 3,
                                          [&](int i) {n2 += i;},
                                          [&](int i) {n1 += i;})
               == end(r));
        assert(n1 == 1 && n2 == 0);
      }
      // four elements
      {
        L r = {1, 2, 3, 4};
        using vt = ez_vector<int>;
        vt v1;
        vt v2;

        const auto f_first = [&](int n) {
          v1.erase(v1.begin(), v1.end());
          v2.erase(v2.begin(), v2.end());
          assert(for_each_excluding_first_n
                 (r, n,
                  [&](int &i) {v2.insert(v2.end(), i);},
                  [&](int &i) {v1.insert(v1.end(), i);})
                 == end(r));
        };
        const auto f_last = [&](int n) {
          v1.erase(v1.begin(), v1.end());
          v2.erase(v2.begin(), v2.end());
          assert(for_each_excluding_last_n
                 (r, n,
                  [&](int &i) {v1.insert(v1.end(), i);},
                  [&](int &i) {v2.insert(v2.end(), i);})
                 == end(r));
        };

        f_first(0);
        assert(v1 == vt({}));
        assert(v2 == vt({1, 2, 3, 4}));
        f_last(0);
        assert(v1 == vt({1, 2, 3, 4}));
        assert(v2 == vt({}));

        f_first(1);
        assert(v1 == vt({1}));
        assert(v2 == vt({2, 3, 4}));
        f_last(1);
        assert(v1 == vt({1, 2, 3}));
        assert(v2 == vt({4}));

        f_first(2);
        assert(v1 == vt({1, 2}));
        assert(v2 == vt({3, 4}));
        f_last(2);
        assert(v1 == vt({1, 2}));
        assert(v2 == vt({3, 4}));

        f_first(4);
        assert(v1 == vt({1, 2, 3, 4}));
        assert(v2 == vt({}));
        f_last(4);
        assert(v1 == vt({}));
        assert(v2 == vt({1, 2, 3, 4}));

        f_first(5);
        assert(v1 == vt({1, 2, 3, 4}));
        assert(v2 == vt({}));
        f_last(5);
        assert(v1 == vt({}));
        assert(v2 == vt({1, 2, 3, 4}));

        f_first(100);
        assert(v1 == vt({1, 2, 3, 4}));
        assert(v2 == vt({}));
        f_last(100);
        assert(v1 == vt({}));
        assert(v2 == vt({1, 2, 3, 4}));
      }
    };
    test_f(type_tag<ez_forward_list<int>>{});
    test_f(type_tag<ez_vector<int>>{});

    // for_each_excluding_first_n for iitr-range
    {
      ez_vector<int> v = {};
      auto r = degraded_irng(v);
      assert(is_just_irng<decltype(r)>);
      ez_vector<int> v1;
      ez_vector<int> v2;

      for (int i : iters(0, 10))
        for (int j : iters(0, i + 5)) {
          v.erase(v.begin(), v.end());
          v1.erase(v1.begin(), v1.end());
          v2.erase(v2.begin(), v2.end());          

          for (int k : iters(0, i))
            v.insert(v.end(), k);

          assert(for_each_excluding_first_n
                 (r, j,
                  [&v2](auto &x) {v2.insert(v2.end(), x);},
                  [&v1](auto &x) {v1.insert(v1.end(), x);})
                 == end(r));
          const auto mid_it = next(v.begin(), j, v.end());
          test_rng(rng(v.begin(), mid_it), v1);
          test_rng(rng(mid_it, v.end()), v2);
        }
    }
  }
}
void test_find() {
  const iter_pair<const int *> z{};
  assert(find(z, 0) == end(z));
  assert(find(z, 0, equal_to<>{}) == end(z));
  assert(find_not(z, 0) == end(z));
  assert(find_not(z, 0, equal_to<>{}) == end(z));

  const int a[] = {1, 2, 3, 4};
  assert(find(a, 3) == nth(a, 2));
  assert(find(a, 3, equal_to<>{}) == nth(a, 2));
  assert(find(a, 5) == end(a));
  assert(find(a, 5, equal_to<>{}) == end(a));
  assert(find_not(a, 1) == nth(a, 1));
  assert(find_not(a, 1, equal_to<>{}) == nth(a, 1));

  const int b[] = {1, 1, 1};
  assert(find_not(b, 1) == end(b));
  assert(find_not(b, 1, equal_to<>{}) == end(b));

  {
    constexpr int v[] = {1, 2, 3};
    static_assert(find_if(v, bind(equal_to<>{}, _1, 4)) == end(v));
    static_assert(find_if_not(v, bind(equal_to<>{}, _1, 0)) == begin(v));
    static_assert(find(v, 2) == nth(v, 1));
    static_assert(find(v, 2, equal_to<>{}) == nth(v, 1));
    static_assert(find_not(v, 1) == nth(v, 1));
    static_assert(find_not(v, 1, equal_to<>{}) == nth(v, 1));
  }
}
void test_find_last() {
  const iter_pair<const int *> z{};
  assert(find_last(z, 0) == end(z));
  assert(find_last(z, 0, equal_to<>{}) == end(z));
  assert(find_last_not(z, 0) == end(z));
  assert(find_last_not(z, 0, equal_to<>{}) == end(z));

  const int a[] = {1, 2, 3, 1};
  assert(find_last(a, 0) == end(a));
  assert(find_last(a, 1) == prev(end(a)));
  assert(find_last(a, 1, equal_to<>{}) == prev(end(a)));
  assert(find_last(a, 2) == next(begin(a)));
  assert(find_last(a, 2, equal_to<>{}) == nth(a, 1));
  assert(find_last_not(a, 1) == nth(a, 2));
  assert(find_last_not(a, 1, not_equal_to<>{}) == prev(end(a)));

  ez_list b = {1, 2, 3, 1, 2};
  const auto bb = degraded_frng(begin(b), end(b));
  static_assert(is_just_frng<decltype(bb)>);
  assert(find_last(bb, 1) == nth(bb, 3));
  assert(find_last(bb, 2) == nth(bb, 4));
  assert(find_last(bb, 3) == nth(bb, 2));
  assert(find_last(bb, 0) == end(bb));

  {
    constexpr int v[] = {1, 2, 3};
    static_assert(find_last_if(v, bind(equal_to<>{}, _1, 4)) == end(v));
    static_assert(find_last_if_not(v, bind(equal_to<>{}, _1, 0))
                  == nth(v, 2));
    static_assert(find_last(v, 2) == nth(v, 1));
    static_assert(find_last(v, 2, equal_to<>{}) == nth(v, 1));
    static_assert(find_last_not(v, 3) == nth(v, 1));
    static_assert(find_last_not(v, 3, equal_to<>{}) == nth(v, 1));
  }
}
void test_find_first_of() {
  const int a[] = {1, 2, 3, 4};
  const int b[] = {0, 2, 3, 5};
  const int c[] = {0};
  const int d[] = {3};
  assert(find_first_of(a, b) == nth(a, 1));
  assert(find_first_of(a, iter_pair<const int *>{}) == end(a));
  assert(find_first_of(iter_pair<const int *>{}, iter_pair<const int *>{})
         == nullptr);
  assert(find_first_of(a, b, equal_to<>{}) == nth(a, 1));
  assert(find_first_of(a, iter_pair<const int *>{}, equal_to<>{}) == end(a));
  assert(find_first_of(iter_pair<const int *>{}, iter_pair<const int *>{},
                       equal_to<>{})
         == nullptr);
  assert(find_first_of(a, c) == end(a));
  assert(find_first_of(a, d) == nth(a, 2));
}
void test_find_last_of() {
  const int a[] = {1, 2, 3, 4};
  const int b[] = {0, 2, 3, 5};
  const int c[] = {0};
  const int d[] = {3};
  assert(find_last_of(a, b) == nth(a, 2));
  assert(find_last_of(a, iter_pair<const int *>{}) == end(a));
  assert(find_last_of(iter_pair<const int *>{}, iter_pair<const int *>{})
         == nullptr);
  assert(find_last_of(a, b, equal_to<>{}) == nth(a, 2));
  assert(find_last_of(a, iter_pair<const int *>{}, equal_to<>{}) == end(a));
  assert(find_last_of(iter_pair<const int *>{}, iter_pair<const int *>{},
                       equal_to<>{})
         == nullptr);
  assert(find_last_of(a, c) == end(a));
  assert(find_last_of(a, d) == nth(a, 2));

  const ez_list l = {1, 1, 1, 2, 1, 1, 3};
  const auto ll = degraded_frng(begin(l), end(l));
  assert(find_last_of(ll, ez_vector{0}) == end(ll));
  auto i = end(ll);
  assert(find_last_of(ll, ez_vector{0, 1}) == nth(ll, 5));
  assert(find_last_of(ll, ez_vector{0, 1, 1, 0}) == nth(ll, 5));
  assert(find_last_of(ll, ez_vector<int>{}) == end(ll));
}
void test_adjacent_find() {
  const int a[] = {1, 2, 2, 4};
  const int b[] = {1, 2, 2, 2, 3, 2, 2, 4};
  assert(adjacent_find(a) == nth(a, 1));
  assert(adjacent_find(a, equal_to<>{}) == nth(a, 1));
  assert(adjacent_find(b) == nth(b, 1));
  assert(adjacent_find(b, equal_to<>{}) == nth(b, 1));

  // adjacent_find_both
  // adjacent_find_both_while
  {
    ez_vector<char> v
      = {1, 2, 3, 4, 'a', 'b', 5, 6};
    assert(adjacent_find_both(v, isalpha) == nth(v, 4));
    assert(adjacent_find_both(rrng(v), isalpha).base() == nth(v, 6));
    assert(adjacent_find_both_while
           (v, isalpha, [&v](char &c) {return addressof(c) - v.begin() <= 3;})
           == v.end());
    assert(adjacent_find_both_while
           (v, isalpha, [&v](char &c) {return addressof(c) - v.begin() <= 4;})
           == nth(v, 4));

    v = {1, 2, 3, 'a', 'b'};
    assert(adjacent_find_both(v, isalpha) == nth(v, 3));
    assert(adjacent_find_both(rrng(v), isalpha).base() == v.end());

    v = {'a', 'b'};
    assert(adjacent_find_both(v, isalpha) == nth(v, 0));
    assert(adjacent_find_both(rrng(v), isalpha).base() == v.end());
  }
}
void test_count() {
  const int a[] = {1, 2, 2, 3};
  const int b[] = {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0};
  assert(count(a, 2) == 2);
  assert(count_if(a, bind(equal_to<>{}, _1, 2)) == 2);
  assert(count(b, 1) == 7);
  assert(count(b, 0) == 8);
}
void test_mismatch() {
  const int a[] = {1, 2, 3, 4};
  const int b[] = {1, 2, 3, 5};
  const int c[] = {2, 3, 4, 6};
  assert(mismatch(a, b) == pair(nth(a, 3), nth(b, 3)));
  assert(mismatch(a, b, equal_to<>{}) == pair(nth(a, 3), nth(b, 3)));
  assert(mismatch(a, begin(b)) == pair(nth(a, 3), nth(b, 3)));
  assert(mismatch(a, begin(b), equal_to<>{}) == pair(nth(a, 3), nth(b, 3)));
  assert(mismatch(b, c, [](auto x, auto y) {return x + 1 == y;})
         == pair(end(b), end(c)));
}
void test_is_permutation() {
  const auto z = iter_pair<const int *>{};
  const int a[] = {1, 2, 3};
  const int b[] = {2, 3, 1};
  assert(is_permutation(z, z));
  assert(is_permutation(z, z, equal_to<>{}));
  assert(!is_permutation(z, a));
  assert(!is_permutation(z, a, equal_to<>{}));
  assert(!is_permutation(a, z));
  assert(!is_permutation(a, z, equal_to<>{}));
  assert(is_permutation(z, begin(z)));
  assert(is_permutation(z, begin(z), equal_to<>{}));
  assert(is_permutation(a, b));
  assert(is_permutation(a, b, equal_to<>{}));
  assert(is_permutation(a, begin(b)));
  assert(is_permutation(a, begin(b), equal_to<>{}));

  ez_vector<ez_vector<int>> l;
  for (int i1 : iters(0, 5))
    for (int i2 : iters(0, 5))
      for (int i3 : iters(0, 5))
        for (int i4 : iters(0, 5))
          for (int i5 : iters(0, 5)) {
            ez_vector<int> v = {0, 0, 0, 0, 0};
            ref(v, i1) = 1;
            if (ref(v, i2) != 0)
              continue;
            ref(v, i2) = 2;
            if (ref(v, i3) != 0)
              continue;
            ref(v, i3) = 3;
            if (ref(v, i4) != 0)
              continue;
            ref(v, i4) = 4;
            if (ref(v, i5) != 0)
              continue;
            ref(v, i5) = 5;
            l.insert(l.end(), v);
          }
  assert(l.size() == 5 * 4 * 3 * 2);
  for (int i : iters(0, size(l))) {
    assert(is_permutation(ref(l, i), irng(1, 6)));
    for (int j : iters(0, size(l)))
      assert(is_permutation(ref(l, i), ref(l, j)));
  }
}
void test_find_subrange() {
  ez_vector<int> v;
  ez_list<int> l;
  ez_forward_list<int> l2;
  for (int k1 : iters(0, 5))
    for (int k2 : iters(0, 5))
      for (int k3 : iters(0, 5))
        for (int k : iters(0, 5)) {
          const auto pattern = irng(1, 1 + k);
          const auto pattern2 = copy(pattern,
                                     to_front(ez_forward_list<int>{})).base();
          v.erase(v.begin(), v.end());
          l.erase(l.begin(), l.end());
          l2.erase_after(l2.before_begin(), l2.end());

          copy(rng(k1, 0), to_back(v));
          copy(pattern, to_back(v));
          copy(rng(k2, 0), to_back(v));
          copy(pattern, to_back(v));
          copy(rng(k3, 0), to_back(v));
          copy(v, to_back(l));
          copy(v, to_front(l2));

          {
            const auto v_cmp = ((k == 0)
                                ? rng(nth(v, 0), nth(v, 0))
                                : rng(nth(v, k1), nth(v, k1 + k)));
            const auto l_cmp = ((k == 0)
                                ? rng(nth(l, 0), nth(l, 0))
                                : rng(nth(l, k1), nth(l, k1 + k)));
            const auto l2_cmp = ((k == 0)
                                 ? rng(nth(l2, 0), nth(l2, 0))
                                 : rng(nth(l2, k1), nth(l2, k1 + k)));
            assert(find_subrange(v, pattern) == v_cmp);
            assert(find_subrange(v, pattern2) == v_cmp);
            assert(find_subrange(l, pattern) == l_cmp);
            assert(find_subrange(l, pattern2) == l_cmp);
            assert(find_subrange(l2, pattern) == l2_cmp);
            assert(find_subrange(l2, pattern2) == l2_cmp);
          }
          {
            const auto v_cmp = ((k == 0)
                                ? rng(end(v), end(v))
                                : rng(nth(v, k1 + k + k2),
                                      nth(v, k1 + k + k2 + k)));
            const auto l_cmp = ((k == 0)
                                ? rng(end(l), end(l))
                                : rng(nth(l, k1 + k + k2),
                                      nth(l, k1 + k + k2 + k)));
            const auto l2_cmp = ((k == 0)
                                ? rng(end(l2), end(l2))
                                : rng(nth(l2, k1 + k + k2),
                                      nth(l2, k1 + k + k2 + k)));
            assert(find_last_subrange(v, pattern) == v_cmp);
            assert(find_last_subrange(v, pattern2) == v_cmp);
            assert(find_last_subrange(l, pattern) == l_cmp);
            assert(find_last_subrange(l, pattern2) == l_cmp);
            assert(find_last_subrange(l2, pattern) == l2_cmp);
            assert(find_last_subrange(l2, pattern2) == l2_cmp);
          }
        }
}
void test_search() {
  {
    const iter_pair<const int *> z{};
    const int a[] = {1, 2, 3, 2, 3, 3, 2};
    const int b[] = {3, 2};
    assert(search(z, z) == nullptr);
    assert(search(z, z, equal_to<>{}) == nullptr);
    assert(search(z, a) == nullptr);
    assert(search(z, a, equal_to<>{}) == nullptr);
    assert(search(a, z) == begin(a));
    assert(search(a, z, equal_to<>{}) == begin(a));
    assert(search(a, b) == begin(a) + 2);
    assert(search(a, b, equal_to<>{}) == begin(a) + 2);
    assert(search(b, a) == end(b));
  }

  const auto f = []<class T>(type_tag<T>) {
    using l_t = T;
    const l_t l = {1, 2, 3, 0, 0, 0, 0};
    const l_t l2 = {0, 0, 0, 0, 1, 2, 3};
    const l_t l3 = {0, 0, 0, 1, 2, 3, 0, 0};
    const l_t x = {1, 2, 3};
    const l_t y = {2};
    const l_t z;
    assert(search(l, x) == begin(l));
    assert(search(l2, x) == nth(l2, 4));
    assert(search(l3, x) == nth(l3, 3));
    assert(search(l, y) == nth(l, 1));
    assert(search(l2, y) == nth(l2, 5));
    assert(search(l3, y) == nth(l3, 4));
    assert(search(z, z) == begin(z));
    assert(search(l, z) == begin(l));
    assert(search(y, x) == end(y));
    assert(search(x, x) == begin(x));
    assert(search(y, y) == begin(y));
  };
  f(type_tag<ez_forward_list<int>>{});
  f(type_tag<ez_bidirectional_list<int>>{});
  f(type_tag<ez_list<int>>{});

  {
    using l_t = ez_forward_list<int>;
    const l_t l = {1, 1, 1, 0, 0, 0, 0};
    const l_t l2 = {0, 0, 0, 0, 1, 1, 1};
    const l_t l3 = {0, 0, 0, 1, 1, 1, 0, 0};
    const auto x = rng(3, 1);
    const auto y = rng(1, 1);
    const auto z = rng(0, 1);
    assert(search(l, x) == begin(l));
    assert(search(l2, x) == nth(l2, 4));
    assert(search(l3, x) == nth(l3, 3));
    assert(search(l, y) == nth(l, 0));
    assert(search(l2, y) == nth(l2, 4));
    assert(search(l3, y) == nth(l3, 3));
    assert(search(z, z) == begin(z));
    assert(search(l, z) == begin(l));
    assert(search(y, x) == begin(y));
    assert(search(x, x) == begin(x));
    assert(search(y, y) == begin(y));
  }
}
void test_find_end() {
  const iter_pair<const int *> z{};
  assert(find_end(z, z) == end(z));
  assert(find_end(z, z, equal_to<>{}) == end(z));

  const int a[] = {1, 2, 1, 2, 1, 2};
  assert(find_end(a, z) == end(a));
  assert(find_end(a, ez_vector{1, 2}) == end(a) - 2);
  assert(find_end(a, ez_vector{1, 2, 1}) == end(a) - 4);
  assert(find_end(a, ez_vector{1, 2, 1, 2}) == end(a) - 4);
  assert(find_end(a, ez_vector{1, 2, 1, 2, 1, 2}) == begin(a));
  assert(find_end(a, ez_vector{1, 2, 1, 2, 1, 2, 1}) == end(a));

  const ez_list<int> l = {1, 2, 1, 2, 1, 2};
  const auto b = rng(l);
  static_assert(!rng_is_sized<decltype(b)>);
  assert(find_end(b, z) == end(b));
  assert(find_end(b, ez_vector{1, 2}) == prev(end(b), 2));
  assert(find_end(b, ez_vector{1, 2, 1}) == prev(end(b), 4));
  assert(find_end(b, ez_vector{1, 2, 1, 2}) == prev(end(b), 4));
  assert(find_end(b, ez_vector{1, 2, 1, 2, 1, 2}) == begin(b));
  assert(find_end(b, ez_vector{1, 2, 1, 2, 1, 2, 1}) == end(b));

  const auto c = degraded_frng(begin(l), end(l));
  static_assert(!rng_is_sized<decltype(c)>);
  assert(find_end(c, z) == end(c));
  assert(find_end(c, ez_vector{1, 2}) == nth(c, 4));
  assert(find_end(c, ez_vector{1, 2, 1}) == nth(c, 2));
  assert(find_end(c, ez_vector{1, 2, 1, 2}) == nth(c, 2));
  assert(find_end(c, ez_vector{1, 2, 1, 2, 1, 2}) == begin(c));
  assert(find_end(c, ez_vector{1, 2, 1, 2, 1, 2, 1}) == end(c));
}
void test_contains() {
  using l_t = ez_forward_list<int>;
  const l_t z = {};
  const l_t a = {0, 0, 0, 1, 0, 1};
  const l_t b = {0, 0, 0, 1};
  const l_t c = {1, 0, 0, 0};
  const l_t d = {0, 1, 0};
  assert(!contains(z, 0));
  assert(!contains_subrange(z, irng(0, 1)));
  assert(contains_subrange(z, irng(0, 0)));
  assert(contains_subrange(z, z));
  assert(contains(a, 1, equal_to{}));
  assert(contains_subrange(a, irng(1, 2)));
  assert(contains(b, 1));
  assert(contains_subrange(b, irng(1, 2)));
  assert(contains(c, 1));
  assert(contains_subrange(c, irng(1, 2)));
  assert(contains(d, 1));
  assert(contains_subrange(d, irng(1, 2)));

  const l_t g = {0, 0, 1, 2, 3, 0, 0};
  const l_t h = {1, 2, 3, 0, 0, 0, 0};
  const l_t i = {0, 0, 0, 0, 1, 2, 3};
  assert(contains_subrange(g, g, equal_to{}));
  assert(contains_subrange(g, g));
  assert(contains_subrange(g, z));
  assert(!contains_subrange(z, g));
  assert(contains_subrange(g, irng(1, 4)));
  assert(contains_subrange(g, irng(1, 3)));
  assert(!contains_subrange(g, irng(1, 5)));
  assert(contains_subrange(h, irng(1, 4)));
  assert(contains_subrange(i, irng(1, 4)));
}
void test_starts_ends_with() {
  {
    using l_t = ez_forward_list<int>;
    const l_t z = {};
    const l_t a = {1, 2, 3, 4};
    const l_t b = {1};
    const l_t c = {1, 2};
    const l_t d = {1, 2, 3};
    assert(starts_with(z, z, equal_to{}));
    assert(starts_with(z, z));
    assert(starts_with(a, z));
    assert(starts_with(a, a));
    assert(starts_with(a, b));
    assert(starts_with(a, c));
    assert(starts_with(a, d));
    assert(ends_with(d, d));
    assert(!ends_with(d, irng(2, 5)));
    assert(ends_with(d, irng(2, 4)));
    assert(ends_with(d, irng(3, 4)));
    assert(ends_with(d, z));
    assert(!ends_with(d, c));
    assert(!ends_with(d, b));
  }
  {
    using l_t = ez_list<int>;
    l_t z = {};
    l_t a = {1, 2, 3, 4, 5};
    assert(ends_with(z, z, equal_to{}));
    assert(ends_with(z, z));
    assert(ends_with(a, z, equal_to{}));
    assert(ends_with(a, z));
    assert(!ends_with(a, irng(4, 10)));
    assert(!ends_with(a, irng(4, 7)));
    assert(ends_with(a, irng(4, 6)));
    assert(ends_with(a, irng(3, 6)));
    assert(ends_with(a, irng(2, 6)));
    assert(ends_with(a, irng(1, 6)));
    assert(ends_with(a, a));
    assert(!ends_with(a, irng(1, 7)));
  }
}
void test_fold_left_right() {
  {
    using l_t = ez_forward_list<int>;
    const l_t z = {};
    const l_t a = {1, 2, 3};
    const auto f = [](int x, int y)->int {return x * 10 + y;};
    static_assert(same_as
                  <decltype(fold_left_plus(z, 1, f)),
                   pair<l_t::const_iterator, int>>);
    assert(fold_left_plus(z, 1, f) == pair(end(z), 1));
    assert(fold_left_plus(a, 1, f) == pair(end(a), 1123));
    assert(fold_left_plus(a, 0, f) == pair(end(a), 123));
    assert(fold_left(z, 1, f) == 1);
    assert(fold_left(a, 1, f) == 1123);
    assert(fold_left(a, 0, f) == 123);
    static_assert(same_as
                  <decltype(fold_left_first_plus(z, f)),
                   pair<l_t::const_iterator, optional<int>>>);
    static_assert(same_as<decltype(fold_left_first(z, f)), optional<int>>);
    assert(fold_left_first_plus(z, f) == pair(end(z), optional<int>{}));
    assert(!fold_left_first(z, f));
    assert(fold_left_first(a, f) == 123);
  }
  {
    using l_t = ez_list<int>;
    const l_t z = {};
    const l_t a = {1, 2, 3};
    const auto f = [](int x, int y)->int {return x + y * 10;};
    static_assert(same_as<decltype(fold_right(z, 1, f)), int>);
    assert(fold_right(z, 1, f) == 1);
    assert(fold_right(a, 1, f) == 1321);
    assert(fold_right(a, 2, f) == 2321);
    static_assert(same_as
                  <decltype(fold_right_last(z, f)), optional<int>>);
    assert(!fold_right_last(z, f));
    assert(fold_right_last(a, f) == 321);
  }
}
void test_copy_move() {
  // copy_plus
  // copy
  // move_plus
  // move
  {
    using l_t = ez_forward_list<int>;
    const l_t a = {1, 2, 3};
    l_t b = {0, 0, 0};
    assert(copy_plus(a, begin(b)) == pair(end(a), end(b)));
    assert(b == l_t({1, 2, 3}));
    b = {0, 0, 0};
    assert(equal(b, rng(3, 0)));
    assert(copy(a, begin(b)) == end(b));
    assert(b == l_t({1, 2, 3}));

    l_t z = {};
    assert(copy(z, begin(z)) == end(z));
    assert(z.empty());
    assert(copy_plus(z, begin(z)) == pair(end(z), end(z)));
    assert(z.empty());
    assert(move(z, begin(z)) == end(z));
    assert(z.empty());
    assert(move_plus(z, begin(z)) == pair(end(z), end(z)));
    assert(z.empty());
  }
  {
    using v_t = ez_vector<int>;
    using l_t = ez_list<v_t>;
    l_t v = {{1}, {2}, {3}};
    l_t vv = {{}, {}, {}};
    assert(move_plus(v, begin(vv)) == pair(end(v), end(vv)));
    assert((v == l_t{{}, {}, {}}) && (vv == l_t{{1}, {2}, {3}}));
    assert(move(vv, begin(v)) == end(v));
    assert((vv == l_t{{}, {}, {}}) && (v == l_t{{1}, {2}, {3}}));
  }

  // copy_if_plus
  // copy_if
  {
    using l_t = ez_forward_list<int>;
    const l_t a = {0, 1, 0, 2, 3};
    const l_t b = {0, 3, 0, 2, 1, 0, 0};
    l_t c = {0, 0, 0};
    l_t z = {};
    assert(copy_if_plus(a, begin(c), bind(not_equal_to<>{}, _1, 0))
           == pair(end(a), end(c)));
    assert(c == l_t({1, 2, 3}));
    assert(copy_if(b, begin(c), bind(not_equal_to<>{}, _1, 0)) == end(c));
    assert(c == l_t({3, 2, 1}));
    assert(copy_if_plus(z, begin(c), bind(not_equal_to<>{}, _1, 0))
           == pair(end(z), begin(c)));
    assert(z.empty());
    assert(copy_if(z, begin(z), bind(not_equal_to<>{}, _1, 0)) == end(z));
    assert(z.empty());
  }

  // copy_backward
  // move_backward
  {
    using l_t = ez_list<int>;
    l_t a = {1, 2, 3, 4};
    l_t z = {};
    assert(copy_backward(rng(begin(a), nth(a, 3)), end(a)) == nth(a, 1));
    assert(equal(a, l_t{1, 1, 2, 3}));
    assert(copy_backward(rng(begin(a), begin(a)), end(a)) == end(a));
    assert(equal(a, l_t{1, 1, 2, 3}));
    assert(move_backward(rng(begin(a), begin(a)), end(a)) == end(a));
    assert(equal(a, l_t{1, 1, 2, 3}));
    assert(copy_backward(z, end(z)) == end(z));
    assert(z.empty());
    assert(move_backward(z, end(z)) == end(z));
    assert(z.empty());
  }
  {
    using v_t = ez_vector<int>;
    using l_t = ez_list<v_t>;
    l_t a = {{1}, {2}, {3}, {4}};
    assert(move_backward(rng(begin(a), nth(a, 3)), end(a)) == nth(a, 1));
    assert((a == l_t{{}, {1}, {2}, {3}}));
  }

  // copy_from_plus
  // copy_from
  // move_from_plus
  // move_from
  {
    using l_t = ez_list<int>;
    l_t z = {};
    l_t a = {0, 0, 0};
    const l_t b = {1, 2, 3};
    assert(copy_from_plus(a, begin(b)) == pair(end(a), end(b)));
    assert(equal(a, irng(1, 4)));
    a = {0, 0, 0};
    assert(copy_from(a, begin(b)) == end(b));
    assert(equal(a, irng(1, 4)));
    assert(copy_from_plus(z, begin(z)) == pair(end(z), end(z)));
    assert(z.empty());
    assert(copy_from(z, begin(a)) == begin(a));
    assert(z.empty());
    assert(equal(a, irng(1, 4)));
  }
  {
    using l_t = ez_list<ez_list<int>>;
    l_t a = {{}, {}, {}};
    l_t b = {{1}, {2}, {3}};
    assert(move_from_plus(a, begin(b)) == pair(end(a), end(b)));
    assert(a == l_t({{1}, {2}, {3}}));
    assert(b == l_t({{}, {}, {}}));
    a = {{}, {}, {}};
    b = {{1}, {2}, {3}};
    assert(move_from(a, begin(b)) == end(b));
    assert(a == l_t({{1}, {2}, {3}}));
    assert(b == l_t({{}, {}, {}}));
  }
}
void test_swap_ranges() {
  using l_t = ez_forward_list<int>;
  l_t z = {};
  l_t a = {1, 2, 3};
  l_t b = {4, 5, 6};
  assert(swap_ranges(z, begin(z)) == end(z));
  assert(swap_ranges(z, z) == pair(end(z), end(z)));
  assert(swap_ranges_plus(z, begin(z)) == pair(end(z), end(z)));
}
void test_transform() {
  using l_t = ez_forward_list<int>;
  l_t z = {};
  l_t a = {1, 2, 3};
  l_t b = {4, 5, 6, 7};
  l_t c = {0, 0, 0};

  assert(transform(z, begin(z), negate{}) == end(z));
  assert(transform(z, z, begin(z), plus{}) == end(z));
  assert(transform(z, begin(z), begin(z), plus{}) == end(z));
  assert(transform_plus(z, begin(z), negate{}) == pair(end(z), end(z)));
  assert(transform_plus(z, z, begin(z), plus{})
         == tuple(end(z), end(z), end(z)));
  assert(transform_plus(z, begin(z), begin(z), plus{})
         == tuple(end(z), end(z), end(z)));

  assert(transform(a, b, begin(c), plus{}) == end(c));
  assert(c == l_t({5, 7, 9}));
  c = {0, 0, 0};
  assert(transform(a, begin(b), begin(c), plus{}) == end(c));
  assert(c == l_t({5, 7, 9}));
  assert(transform(a, begin(c), negate{}) == end(c));
  assert(c == l_t({-1, -2, -3}));

  assert(transform_plus(a, b, begin(c), plus{})
         == tuple(end(a), nth(b, 3), end(c)));
  assert(c == l_t({5, 7, 9}));
  c = {0, 0, 0};
  assert(transform_plus(a, begin(b), begin(c), plus{})
         == tuple(end(c), nth(b, 3), end(c)));
  assert(c == l_t({5, 7, 9}));
  assert(transform_plus(a, begin(c), negate{})
         == pair(end(c), end(c)));
  assert(c == l_t({-1, -2, -3}));

  assert(transform(a, begin(a), begin(a), plus{}) == end(a));
  assert(equal(a, aligned_stride_rng(irng(2, 8), 2)));
}
void test_replace() {
  using l_t = ez_forward_list<int>;
  l_t z;
  assert(replace_if_plus(z, bind(equal_to{}, _1, 1), 2) == end(z));
  assert(z.empty());
  replace_if(z, bind(equal_to{}, _1, 1), 2);
  assert(z.empty());
  assert(replace_plus(z, 1, 2) == end(z));
  replace_plus(z, 1, 2);
  assert(z.empty());
  assert(replace_copy_if_plus(z, begin(z), bind(equal_to{}, _1, 1), 2)
         == pair(end(z), end(z)));
  assert(z.empty());
  assert(replace_copy_if(z, begin(z), bind(equal_to{}, _1, 1), 2) == end(z));
  assert(z.empty());
  assert(replace_copy_plus(z, begin(z), 1, 2) == pair(end(z), end(z)));
  assert(z.empty());
  assert(replace_copy(z, begin(z), 1, 2) == end(z));
  assert(z.empty());

  l_t a = {1, 2, 3, 2, 4};
  replace(a, 2, 0);
  assert(a == l_t({1, 0, 3, 0, 4}));
  a = {1, 2, 3, 2, 4};
  assert(replace_plus(a, 2, 0) == end(a));
  assert(a == l_t({1, 0, 3, 0, 4}));

  a = {1, 2, 3, 2, 4};
  l_t b = {0, 0, 0, 0, 0};
  assert(replace_copy(a, begin(b), 2, 0) == end(b));
  assert(a == l_t({1, 2, 3, 2, 4}));
  assert(b == l_t({1, 0, 3, 0, 4}));
  a = {1, 2, 3, 2, 4};
  b = {0, 0, 0, 0, 0};
  assert(replace_copy_plus(a, begin(b), 2, 0) == pair(end(a), end(b)));
  assert(a == l_t({1, 2, 3, 2, 4}));
  assert(b == l_t({1, 0, 3, 0, 4}));
}
void test_fill() {
  using l_t = ez_forward_list<int>;
  using v_t = ez_vector<int>;
  l_t z;
  l_t a = {0, 0, 0};
  assert(fill_plus(z, 1) == end(z));
  assert(z.empty());
  assert(fill_plus(a, 1) == end(a));
  assert(equal(a, rng(3, 1)));
  fill(a, 2);
  assert(equal(a, rng(3, 2)));

  fill_zero(a);
  assert(equal(a, rng(3, 0)));
  assert(fill_zero_plus(z) == end(z));
  assert(empty(z));
  ref(a, 1) = 1;
  assert(fill_zero_plus(a) == end(a));
  assert(equal(a, rng(3, 0)));

  v_t v = {1, 2, 3};
  bytewise_fill(v, 0);
  assert(equal(v, rng(3, 0)));
}
void test_generate() {
  using l_t = ez_forward_list<int>;
  l_t z;
  l_t a = {1, 2, 3};
  auto f = []() {return 0;};
  generate(z, f);
  assert(z.empty());
  assert(generate_plus(z, f) == end(z));
  assert(z.empty());
  assert(generate_plus(a, f) == end(a));
  assert(equal(a, rng(3, 0)));
  a = {1, 2, 3};
  generate(a, f);
  assert(equal(a, rng(3, 0)));  
}
void test_remove() {
  using l_t = ez_forward_list<int>;
  l_t z;
  l_t a = {1, 2, 3, 2, 5};
  l_t b = {0, 0, 0};
  assert(remove(z, 0) == end(z));
  assert(remove_if(z, bind(equal_to{}, _1, 0)) == end(z));
  assert(remove_copy_if_plus(z, begin(z), bind(equal_to{}, _1, 0))
         == pair(end(z), end(z)));
  assert(remove_copy_plus(z, begin(z), 0) == pair(end(z), end(z)));
  assert(remove(a, 2) == nth(a, 3));
  assert(size(a) == 5 && equal(l_t({1, 3, 5}), begin(a)));
  a = {1, 2, 3, 2, 5};
  assert(remove_copy_plus(a, begin(b), 2) == pair(end(a), end(b)));
  assert(b == l_t({1, 3, 5}));
  assert(size(a) == 5 && equal(a, l_t({1, 2, 3, 2, 5})));
  assert(remove_copy(a, begin(a), 2) == nth(a, 3));
  assert(size(a) == 5 && equal(l_t({1, 3, 5}), begin(a)));
}
void test_unique() {
  using l_t = ez_forward_list<int>;
  l_t z;
  assert(unique(z) == end(z) && empty(z));
  assert(unique_copy_plus(z, begin(z)) == pair(end(z), end(z)) && empty(z));
  assert(unique_copy(z, begin(z)) == end(z) && empty(z));
  l_t a = {1, 2, 2, 3, 3, 3, 4, 4};
  l_t b = {1, 1, 1, 2, 2, 3};
  assert(size(a) == 8 && equal(rng(begin(a), unique(a)), irng(1, 5)));
  assert(size(b) == 6 && equal(rng(begin(b), unique(b)), irng(1, 4)));
  a = {1, 2, 2, 3, 3, 3, 4, 4};
  b = {1, 1, 1, 2, 2, 3};
  assert(size(a) == 8
         && equal(rng(begin(a), unique_copy(a, begin(a))), irng(1, 5)));
  assert(size(b) == 6
         && equal(rng(begin(b), unique_copy(b, begin(b))), irng(1, 4)));
  a = {1, 2, 2, 3, 3, 3, 4, 4};
  b = {1, 1, 1, 2, 2, 3};
  assert(unique_copy_plus(a, begin(a)) == pair(end(a), nth(a, 4)));
  assert(unique_copy_plus(b, begin(b)) == pair(end(b), nth(b, 3)));
}
void test_reverse() {
  using l_t = ez_list<int>;
  l_t z;
  l_t a = {1, 2};
  l_t b = {1, 2, 3};
  l_t c = {1, 2, 3, 4};
  l_t d = {0, 0, 0, 0};
  reverse(z);
  assert(empty(z));
  reverse_copy(z, begin(z));
  assert(empty(z));
  reverse(a);
  assert(a == l_t({2, 1}));
  reverse(b);
  assert(b == l_t({3, 2, 1}));
  reverse(c);
  assert(c == l_t({4, 3, 2, 1}));
  assert(reverse_copy(c, begin(d)) == end(d));
  assert(equal(c, rrng(irng(1, 5))) && equal(d, irng(1, 5)));
}
void test_rotate() {
  using l_t = ez_forward_list<int>;
  using v_t = ez_vector<int>;
  l_t z;
  assert(rotate(z, begin(z)) == end(z));
  l_t a = {1, 2};
  assert(rotate(a, begin(a)) == end(a));
  assert(equal(a, irng(1, 3)));
  assert(rotate(a, end(a)) == begin(a));
  assert(equal(a, irng(1, 3)));
  assert(rotate(a, next(begin(a))) == next(begin(a)));
  assert(equal(a, rrng(irng(1, 3))));
  assert(equal(rotate_copy(a, nth(a, 1), to_back(v_t{})).base(), irng(1, 3)));

  const auto f = [](const l_t &l) {
    const auto sz = size(l);
    v_t v;
    copy(rng(sz, 0), to_back(v));

    for (rng_szt<l_t> n : irng(0, sz)) {
      l_t b = l;
      const auto i = nth(b, n);
      const auto ii = nth(b, sz - n);
      assert(rotate_copy(as_const(b), i, begin(v)) == end(v));
      assert(rotate(b, i) == ii);
      assert(equal(b, v));
    }
  };
  f(l_t{1, 2, 3});
  f(l_t{1, 2, 3, 4});
  f(l_t{1, 2, 3, 4, 5});
}
void test_shift() {
  // shift_left
  {
    using l_t = ez_forward_list<int>;
    l_t a = {1, 2, 3, 4, 5};
    auto i = shift_left(a, 1);
    assert(equal(rng(begin(a), i), irng(2, 6)));
    a = {1, 2, 3, 4, 5};
    i = shift_left(a, 0);
    assert(i == end(a) && equal(a, irng(1, 6)));
    a = {1, 2, 3, 4, 5};
    i = shift_left(a, 5);
    assert(i == begin(a) && equal(a, irng(1, 6)));
    a = {1, 2, 3, 4, 5};
    i = shift_left(a, 3);
    assert(i == nth(a, 2) && equal(rng(begin(a), i), irng(4, 6)));
    a = {1, 2, 3, 4, 5};
    i = shift_left(a, 4);
    assert(i == nth(a, 1) && equal(rng(begin(a), i), irng(5, 6)));
    i = shift_left(a, nth(a, 1));
    assert(i == nth(a, 4) && equal(rng(begin(a), i), irng(2, 6)));
  }
  // shift_right
  {
    using v_t = ez_vector<int>;
    const v_t v0 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i : irng(0, 11)) {
      v_t v = v0;
      v_t::iterator it = shift_right(v, i);
      assert(it == nth(v, i));
      assert(equal(rng(it, end(v)), irng(1, 1 + (10 - i))));
    }
    for (int i : irng(11, 15)) {
      v_t v = v0;
      v_t::iterator it = shift_right(v, i);
      assert(empty(rng(it, end(v))));
      assert(v == v0);
    }
    {
      v_t v = v0;
      auto it = shift_right(v, nth(v, 8));
      assert(equal(rng(it, end(v)), irng(1, 9)));
    }

    using l_t = ez_forward_list<int>;
    const l_t l0 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i : irng(0, 11)) {
      l_t l = l0;
      l_t::iterator it = shift_right(l, i);
      assert(it == nth(l, i));
      assert(equal(rng(it, end(l)), irng(1, 1 + (10 - i))));
    }
    for (int i : irng(11, 15)) {
      l_t l = l0;
      l_t::iterator it = shift_right(l, i);
      assert(empty(rng(it, end(l))));
      assert(l == l0);
    }
  }
}
void test_shuffle() {
  minstd_rand g;
  {
    ez_vector<int> v;
    shuffle(v, g);
    assert(v.empty());
  }
  {
    ez_vector<ez_vector<int>> vv;
    copy(rng(1000, ez_vector<int>{0, 1, 2, 3, 4}), to_back(vv));
    for (auto &v : vv) {
      shuffle(v, g);
      assert(is_permutation(v, irng(0, 5)));
    }
    array<array<int, 5>, 5> a{};
    for (int i : irng(0, 5)) {
      for (auto &v : vv) {
        const int k = find(v, i) - begin(v);
        assert(k >= 0 && k <= 4);
        ++a[i][k];
      }
    }
    for (auto &x : a)
      for (int y : x)
        assert(y >= 150 && y <= 250);
  }
}
void test_sample() {
  minstd_rand g;
  {
    ez_vector<int> v;
    ez_vector<int> v2;
    assert(sample(v, v2, g) == begin(v2));
    assert(sample(degraded_irng(v), v2, g) == begin(v2));
  }
  {
    ez_vector<int> v;
    ez_vector<int> v2 = {0};
    assert(sample(v, v2, g) == begin(v2));
    assert(sample(degraded_irng(v), v2, g) == begin(v2));
  }
  {
    ez_vector<int> v = {0};
    ez_vector<int> v2 = {0};
    assert(sample(v, v2, g) == end(v2));
    assert(equal(v2, irng(0, 1)));
    v2 = {0};
    assert(sample(degraded_irng(v), v2, g) == end(v2));
    assert(equal(v2, irng(0, 1)));
  }
  {
    ez_vector<int> v = {0, 1, 2};
    ez_vector<int> v2 = {0, 0, 0};
    assert(sample(v, v2, g) == end(v2));
    assert(equal(v2, irng(0, 3)));
    v2 = {0, 0, 0};
    assert(sample(degraded_irng(v), v2, g) == end(v2));
    assert(equal(v2, irng(0, 3)));
  }
  {
    ez_vector<int> v = {1, 2, 3};
    ez_vector<int> v2 = {0, 0, 0, 0};
    assert(sample(v, v2, g) == nth(v2, 3));
    assert(v2 == ez_vector<int>({1, 2, 3, 0}));
    v2 = {0, 0, 0, 0};    
    assert(sample(degraded_irng(v), v2, g) == nth(v2, 3));
    assert(v2 == ez_vector<int>({1, 2, 3, 0}));
  }
  {
    ez_vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ez_vector<int> v2 = {0, 0, 0, 0, 0};
    ez_vector<int> v_count = copy(rng(9, 0), to_back(ez_vector<int>{})).base();
    for (int c = 1000; c != 0; --c) {
      assert(sample(v, v2, g) == end(v2));
      assert(is_sorted(v2));
      for (int &x : v2)
        ++ref(v_count, x - 1);
    }
    const double upper_lim = (1000 * 5 / 9) * 1.15;
    const double lower_lim = (1000 * 5 / 9) * 0.85;
    for (int x : v_count)
      assert(x >= lower_lim && x <= upper_lim);

    v_count.erase(v_count.begin(), v_count.end());
    copy(rng(9, 0), to_back(v_count));
    for (int c = 1000; c != 0; --c) {
      assert(sample(degraded_irng(v), v2, g) == end(v2));
      for (int &x : v2)
        ++ref(v_count, x - 1);
    }
    for (int x : v_count)
      assert(x >= lower_lim && x <= upper_lim);
  }
}
void test_binary_search() {
  {
    for (int i : irng(0, 10))
      for (int j : irng(0, 10)) {
        ez_list<int> v;
        copy(rng(i, 0), to_back(v));
        copy(rng(j, 1), to_back(v));
        assert(equal(rng(i, 0), begin(v)));
        assert(equal(rng(nth(v, i), end(v)), rng(j, 1)));
        assert(partition_point(v, bind(equal_to{}, _1, 0)) == nth(v, i));
      }
  }
  {
    const ez_forward_list<int> v = {1, 2, 2, 3};
    assert(lower_bound(v, 2, less{}) == nth(v, 1));
    assert(lower_bound(v, 2) == nth(v, 1));
    assert(upper_bound(v, 2, less{}) == nth(v, 3));
    assert(upper_bound(v, 2) == nth(v, 3));
    assert(equal_range(v, 2, less{}) == pair(nth(v, 1), nth(v, 3)));
    assert(equal_range(v, 2) == pair(nth(v, 1), nth(v, 3)));
    assert(equal_range(v, 4) == pair(end(v), end(v)));

    for (int i : irng(0, 7))
      for (int j : irng(0, 7))
        for (int k : irng(0, 7)) {
          ez_vector<int> vv;
          copy(rng(i, 1), to_back(vv));
          copy(rng(j, 2), to_back(vv));
          copy(rng(k, 3), to_back(vv));
          assert(equal_range(vv, 2)
                 == pair(lower_bound(vv, 2), upper_bound(vv, 2)));
          assert(equal(equal_range(vv, 2), rng(j, 2)));
          assert(binary_search(vv, 2, less{}) == (j != 0));
          if (j == 0)
            assert(bs_find(vv, 2, less{}) == end(vv));
          else {
            const auto it = bs_find(vv, 2, less{});
            assert(*it == 2);
            assert(it == begin(vv) || *prev(it) == 1);
          }
          assert(equal(bs_find_range(vv, 2, 3),
                       rng(lower_bound(vv, 2), upper_bound(vv, 3))));
          assert(bs_find_range(vv, 1, 3) == rng(vv));
          assert(bs_find_range(vv, 1, 2)
                 == pair(begin(vv), lower_bound(vv, 3)));
          assert(bs_find_range(vv, 2, 2) == equal_range(vv, 2));
        }
  }
  {
    const ez_forward_list<int> a = {1, 1, 3, 3, 4, 5};
    auto it = lower_bound(a, 3);
    auto it2 = upper_bound(a, 3);
    assert(next(it, 2) == it2);
    assert(*it == 3 && *next(it) == 3);
    assert(equal(equal_range(a, 3), rng(2, 3)));
    assert(binary_search(a, 3) && !binary_search(a, 2));
    assert(equal(bs_find_range(a, 3, 4), ez_vector{3, 3, 4}));
  }
}
void test_partition() {
  // is_partitioned
  {
    using l_t = ez_forward_list<int>;
    const auto eq = bind(equal_to{}, _1, 0);
    for (int i : irng(0, 7))
      for (int j : irng(0, 7)) {
        l_t l;
        copy(rng(j, 1), to_front(l));
        copy(rng(i, 0), to_front(l));
        assert(is_partitioned(l, eq));
        assert(partition_point(l, eq) == nth(l, i));
      }
  }
  // partition
  // partition_copy
  {
    using v_t = ez_vector<int>;
    using l_t = ez_forward_list<int>;
    using ll_t = ez_list<int>;
    const auto eq = bind(equal_to{}, _1, 0);
    for (int i : irng(0, 5))
      for (int j : irng(0, 5)) {
        v_t v;
        copy(rng(i, 0), to_back(v));
        copy(rng(j, 1), to_back(v));
        while (next_permutation(v)) {
          {
            l_t l = copy(rrng(v), to_front(l_t{})).base();

            v_t v0, v1;
            copy(rng(i, 100), to_back(v0));
            copy(rng(j, 100), to_back(v1));
            const auto x = partition_copy(as_const(l),
                                          begin(v0), begin(v1), eq);
            assert(x.first == end(v0) && x.second == end(v1));
            assert(equal(v0, rng(i, 0)) && equal(v1, rng(j, 1)));

            fill(v0, 0);
            fill(v1, 0);
            const auto xx = partition_copy_plus(as_const(l),
                                                begin(v0), begin(v1), eq);
            assert(at<0>(xx) == end(l));
            assert(at<1>(xx) == end(v0));
            assert(at<2>(xx) == end(v1));
            assert(equal(v0, rng(i, 0)) && equal(v1, rng(j, 1)));

            const auto it = partition(l, eq);
            assert(partition_point(l, eq) == it);
            assert(equal(rng(begin(l), it), rng(i, 0)));
            assert(equal(rng(it, end(l)), rng(j, 1)));
          }
          {
            ll_t l = copy(v, to_back(ll_t{})).base();
            const auto it = partition(l, eq);
            assert(partition_point(l, eq) == it);
            assert(equal(rng(begin(l), it), rng(i, 0)));
            assert(equal(rng(it, end(l)), rng(j, 1)));
          }
        }
      }
  }
  // stable_partition_with_buffer
  // stable_partition_with_no_buffer
  // stable_partition
  {
    using t = pair<int, int>;
    using l_t = ez_list<t>;
    using v_t = ez_vector<t>;
    const auto eq = bind(equal_to{}, bind(at<0>, _1), 0);
    v_t v, vv;
    buffer<t> buf;
    for (int i : irng(0, 7))
      for (int j : irng(0, 7)) {
        l_t l;
        copy(bind_rng(irng(0, i), [](int x) {return pair(0, x);}),
             to_back(l));
        copy(bind_rng(irng(0, j), [](int x) {return pair(1, x);}),
             to_back(l));
        assert(equal(rng(i, 0), bind_itr(begin(l), at<0>)));
        assert(equal(rng(j, 1), bind_itr(nth(l, i), at<0>)));
        while (next_permutation
               (l, bind(less{}, bind(at<0>, _1), bind(at<0>, _2)))) {
          {
            l_t ll = l;
            v.erase(v.begin(), v.end());
            vv.erase(vv.begin(), vv.end());
            partition_copy(ll, to_back(v), to_back(vv), eq);
            const auto it = stable_partition_with_buffer(ll, buf, eq);
            assert(equal(rng(begin(ll), it), v));
            assert(equal(rng(it, end(ll)), vv));
          }
          {
            l_t ll = l;
            v.erase(v.begin(), v.end());
            vv.erase(vv.begin(), vv.end());
            partition_copy(ll, to_back(v), to_back(vv), eq);
            const auto it = stable_partition_with_no_buffer(ll, eq);
            assert(equal(rng(begin(ll), it), v));
            assert(equal(rng(it, end(ll)), vv));
          }
          {
            l_t ll = l;
            v.erase(v.begin(), v.end());
            vv.erase(vv.begin(), vv.end());
            partition_copy(ll, to_back(v), to_back(vv), eq);
            const auto it = stable_partition(ll, eq);
            assert(equal(rng(begin(ll), it), v));
            assert(equal(rng(it, end(ll)), vv));
          }
        }
      }
  }
}
void test_merge() {
  // merge(_plus)
  {
    using l_t = ez_forward_list<int>;
    using v_t = ez_vector<int>;
    const l_t z = {};
    const l_t a = {1, 3, 5};
    const l_t b = {2, 4, 6};
    v_t v = {0, 0, 0, 0, 0, 0};
    assert(merge_plus(z, z, begin(v)) == tuple(end(z), end(z), begin(v)));
    assert(merge(z, z, begin(v)) == begin(v));
    assert(merge_plus(a, b, begin(v)) == tuple(end(a), end(b), end(v)));
    assert(equal(v, irng(1, 7)));
    v.erase(v.begin(), v.end());
    copy(rng(6, 0), to_back(v));
    assert(merge(a, b, begin(v)) == end(v));
    assert(equal(v, irng(1, 7)));

    v = {};
    copy(rng(3, 0), to_back(v));
    assert(merge_plus(z, a, begin(v)) == tuple(end(z), end(a), end(v)));
    assert(equal(v, a));
    v = {};
    copy(rng(3, 0), to_back(v));
    assert(merge_plus(a, z, begin(v)) == tuple(end(a), end(z), end(v)));
    assert(equal(v, a));
    v = {};
    copy(rng(6, 0), to_back(v));
    l_t l = {1, 3, 5, 6};
    l_t ll = {2, 4};
    assert(merge_plus(l, ll, begin(v)) == tuple(end(l), end(ll), end(v)));
    assert(equal(v, irng(1, 7)));
    v = {};
    copy(rng(6, 0), to_back(v));
    l = {1, 3, 5, 6};
    ll = {2, 4};
    assert(merge_plus(ll, l, begin(v)) == tuple(end(ll), end(l), end(v)));
    assert(equal(v, irng(1, 7)));
    v = {};
    copy(rng(2, 0), to_back(v));
    l = {20};
    ll = {10};
    const auto tmp_less = [](int x, int y) {return x % 10 < y % 10;};
    assert(merge(l, ll, begin(v), tmp_less) == end(v));
    assert(equal(v, l_t{20, 10}));
  }
  {
    const ez_vector<int> v = {1, 2, 3, 4, 5, 6};
    ez_vector<int> vv = v;
    ez_vector<int> vvv = v;
    for (int x : irng(1, 6))
      for (const auto &r : combination_rng(irng(v), x)) {
        vv.erase(vv.begin(), vv.end());
        set_difference(irng(v), r,
                       output_itr([&vv](auto i) {vv.insert(vv.end(), *i);}));
        vvv.erase(vvv.begin(), vvv.end());
        merge(deref_rng(r), vv, to_back(vvv));
        assert(v == vvv);
      }
  }
  {
    const ez_vector<pair<int, int>> v = {{1, 1}, {1, 2}, {1, 3},
                                         {2, 1}, {2, 2}, {2, 3},
                                         {3, 1}, {3, 2}, {3, 3}};
    auto vv = v;
    auto v_concat_sorted = v;
    auto v_merged = v;
    for (int x : irng(1, 6))
      for (const auto &r : combination_rng(irng(v), x)) {
        vv.erase(vv.begin(), vv.end());
        set_difference(irng(v), r,
                       output_itr([&vv](auto i) {vv.insert(vv.end(), *i);}));

        const auto cmp = bind(less<>{}, bind(at<0>, _1), bind(at<0>, _2));

        v_concat_sorted.erase(v_concat_sorted.begin(), v_concat_sorted.end());
        copy(deref_rng(r), to_back(v_concat_sorted));
        copy(vv, to_back(v_concat_sorted));
        stable_sort(v_concat_sorted, cmp);

        v_merged.erase(v_merged.begin(), v_merged. end());
        merge(deref_rng(r), vv, to_back(v_merged), cmp);

        assert(v_concat_sorted == v_merged);
        sort(v_concat_sorted);
        assert(v == v_concat_sorted);
      }
  }

  // inplace_merge_with_no_buffer
  // inplace_merge_with_buffer
  // inplace_merge
  {
    ez_vector<int> v = {1, 3, 5, 2, 4, 6};
    inplace_merge(v, nth(v, 3));
    assert(equal(v, irng(1, 7)));

    v = {5, 3, 1, 6, 4, 2};
    inplace_merge(v, nth(v, 3), greater<>{});
    assert(equal(v, rrng(irng(1, 7))));

    v = {1, 3, 5, 2, 4, 6};
    inplace_merge_with_no_buffer(v, nth(v, 3));
    assert(equal(v, irng(1, 7)));

    v = {5, 3, 1, 6, 4, 2};
    inplace_merge_with_no_buffer(v, nth(v, 3), greater<>{});
    assert(equal(v, rrng(irng(1, 7))));
  }
  {
    for (int i : iters(2, 7)) {
      ez_vector<int> v;
      for (int x : iters(0, i))
        v.insert(v.end(), x);

      ez_vector<int> v_a = v, v_b = v, v_c = v;

      for (int j : iters(1, i)) {
        for (const auto &r : combination_rng(irng(v), j)) {
          v_a.erase(v_a.begin(), v_a.end());
          v_b.erase(v_b.begin(), v_b.end());
          v_c.erase(v_c.begin(), v_c.end());

          copy(deref_rng(r), to_back(v_a));
          set_difference(irng(v), r,
                         output_itr([&](auto x) {v_b.insert(v_b.end(), *x);}));
          copy(v_a, to_back(v_c));
          auto n = v_c.end() - v_c.begin();
          copy(v_b, to_back(v_c));
          assert(is_permutation(v_c, v));
          inplace_merge_with_no_buffer(v_c, nth(v_c, n));
          assert(v == v_c);

          v_c.erase(v_c.begin(), v_c.end());
          copy(v_a, to_back(v_c));
          copy(v_b, to_back(v_c));
          assert(is_permutation(v_c, v));
          buffer<rng_vt<decltype(v_c)>> buf;
          inplace_merge_with_buffer(v_c, nth(v_c, n), buf);
          assert(v == v_c);
          assert(equal(v, irng(0, i)));

          v_c.erase(v_c.begin(), v_c.end());
          copy(v_a, to_back(v_c));
          copy(v_b, to_back(v_c));
          assert(is_permutation(v_c, v));
          inplace_merge(v_c, nth(v_c, n));
          assert(v == v_c);
          assert(equal(v, irng(0, i)));
        }
      }
    }
  }
}
void test_set_operations() {
  {
    ez_forward_list<int> a = {1, 3, 5, 7};
    ez_list<int> b = {5, 7, 9, 10};
    ez_vector<int> c, d, e, f;
    set_union(a, b, to_back(c));
    set_intersection(a, b, to_back(d));
    set_difference(a, b, to_back(e));
    set_symmetric_difference(a, b, to_back(f));

    assert(includes(c, a) && includes(c, b));
    assert(equal(c, ez_vector{1, 3, 5, 7, 9, 10}));
    assert(equal(d, ez_vector{5, 7}));
    assert(equal(e, ez_vector{1, 3}));
    assert(equal(f, ez_vector{1, 3, 9, 10}));
  }
  {
    ez_forward_list<int> a = {1, 3, 5, 7};
    ez_list<int> b = {5, 7, 9, 10};
    ez_vector<int> c, d, e, f;
    const auto x1 = set_union_plus(a, b, to_back(c));
    const auto x2 = set_intersection_plus(a, b, to_back(d));
    const auto x3 = set_difference_plus(a, b, to_back(e));
    const auto x4 = set_symmetric_difference_plus(a, b, to_back(f));

    assert(at<0>(x1) == end(a) && at<1>(x1) == end(b));
    assert(at<0>(x2) == end(a) && at<1>(x2) == end(b));
    assert(at<0>(x3) == end(a) && at<1>(x3) == end(b));
    assert(at<0>(x4) == end(a) && at<1>(x4) == end(b));

    assert(equal(c, ez_vector{1, 3, 5, 7, 9, 10}));
    assert(equal(d, ez_vector{5, 7}));
    assert(equal(e, ez_vector{1, 3}));
    assert(equal(f, ez_vector{1, 3, 9, 10}));
  }
  {
    assert(set_union(ez_vector{1, 2, 2, 3}, ez_vector{1, 2, 2, 2, 3},
                     to_back(ez_vector<int>{})).base()
           == ez_vector({1, 2, 2, 2, 3}));
    assert(set_intersection(ez_vector{1, 2, 2, 2, 3}, ez_vector{1, 2, 2, 3},
                            to_back(ez_vector<int>{})).base()
           == ez_vector({1, 2, 2, 3}));
    assert(set_difference(ez_vector{1, 2, 2, 2, 3}, ez_vector{1, 2, 2, 3},
                          to_back(ez_vector<int>{})).base()
           == ez_vector({2}));
    assert(set_symmetric_difference
           (ez_vector{1, 2, 2, 2, 3}, ez_vector{1, 2, 2, 3},
            to_back(ez_vector<int>{})).base()
           == ez_vector<int>{2});
  }
  // exhaustion for includes
  {
    const ez_vector<int> v = {1, 2, 3, 4, 5, 6};
    const ez_vector<int> vv = v;
    const auto cmp = bind(less<>{}, bind(deref, _1), bind(deref, _2));
    for (int i : iters(1, 7))
      for (int j : iters(1, 7))
        for (const auto &r : combination_rng(irng(v), i))
          for (const auto &rr : combination_rng(irng(v), j)) {
            const bool y = includes(r, rr, cmp);
            const bool yy = [&]() {
              for (auto it : rr)
                if (!contains(deref_rng(r), *it))
                  return false;
              return true;
            }();
            assert(y == yy);
          }
  }
  // exhaustion for set_...
  {
    const ez_vector<int> v = {1, 2, 3, 4, 5, 6};
    const ez_vector<int> vv = v;
    ez_vector<int> buf = vv;
    ez_vector<int> buf2 = buf;
    for (int i : iters(1, 7))
      for (int j : iters(1, 7))
        for (const auto &r : combination_rng(irng(v), i))
          for (const auto &rr : combination_rng(irng(v), j)) {
            buf.erase(buf.begin(), buf.end());
            buf2.erase(buf2.begin(), buf2.end());
            set_union(deref_rng(r), deref_rng(rr), to_back(buf));
            for (const auto &x : deref_rng(r))
              buf2.insert(buf2.end(), x);
            for (const auto &y : deref_rng(rr))
              if (!contains(buf2, y))
                buf2.insert(buf2.end(), y);
            assert(is_sorted(buf));
            assert(is_permutation(buf, buf2));

            buf.erase(buf.begin(), buf.end());
            buf2.erase(buf2.begin(), buf2.end());
            set_intersection(deref_rng(r), deref_rng(rr), to_back(buf));
            for (const auto &x : deref_rng(r))
              if (contains(deref_rng(rr), x))
                buf2.insert(buf2.end(), x);
            assert(is_sorted(buf));
            assert(buf == buf2);

            buf.erase(buf.begin(), buf.end());
            buf2.erase(buf2.begin(), buf2.end());
            set_difference(deref_rng(r), deref_rng(rr), to_back(buf));
            for (const auto &x : deref_rng(r))
              if (!contains(deref_rng(rr), x))
                buf2.insert(buf2.end(), x);
            assert(is_sorted(buf));
            assert(buf == buf2);

            buf.erase(buf.begin(), buf.end());
            buf2.erase(buf2.begin(), buf2.end());
            set_symmetric_difference(deref_rng(r), deref_rng(rr), to_back(buf));
            for (const auto &x : deref_rng(r))
              if (!contains(deref_rng(rr), x))
                buf2.insert(buf2.end(), x);
            for (const auto &x : deref_rng(rr))
              if (!contains(deref_rng(r), x))
                buf2.insert(buf2.end(), x);
            assert(is_sorted(buf));
            assert(is_permutation(buf, buf2));
          }
  }
}
void test_heap_operations() {
  // push_heap
  for (int &n : iters(1, 10)) {
    ez_vector<int> v;
    copy(irng(0, n), to_back(v));

    ez_vector<int> buf = v;
    while (next_permutation(v)) {
      buf.erase(buf.begin(), buf.end());
      copy(v, to_back(buf));
      for (auto r = rng(buf.begin(), buf.begin());
           r.second != buf.end();
           ++r.second) {
        push_heap(r);
        assert(is_heap(r));
      }
      assert(is_permutation(buf, v));

      buf.erase(buf.begin(), buf.end());
      copy(v, to_back(buf));
      for (auto r = rng(buf.begin(), buf.begin());
           r.second != buf.end();
           ++r.second) {
        push_heap(r, greater<>{});
        assert(is_heap(r, greater<>{}));
      }
      assert(is_permutation(buf, v));
    }
    assert(equal(v, irng(0, n)));
  }
  // make_heap
  // pop_heap
  // sort_heap
  // is_heap(_until)
  for (int &n : iters(1, 10)) {
    ez_vector<int> v;
    copy(irng(0, n), to_back(v));

    auto buf = v;
    while (next_permutation(v)) {
      buf.erase(buf.begin(), buf.end());
      copy(v, to_back(buf));
      make_heap(buf);
      assert(is_heap(buf));
      assert(is_permutation(buf, v));
      assert(is_heap(rng(begin(buf), 1)));
      assert(is_heap_until(rng(begin(buf), 1)) == nth(buf, 1));
      for (auto i : iters(begin(buf) + 1, end(buf))) {
        int tmp = 100;
        adl_swap(*i, tmp);
        assert(is_heap(rng(begin(buf), i)));
        assert(is_heap_until(buf) == i);
        adl_swap(*i, tmp);
        assert(is_heap_until(buf) == end(buf));
      }
      sort_heap(buf);
      assert(is_sorted(buf));

      buf.erase(buf.begin(), buf.end());
      copy(v, to_back(buf));
      make_heap(buf, greater<>{});
      assert(is_heap(buf, greater<>{}));
      assert(is_permutation(buf, v));
      assert(is_heap(rng(begin(buf), 1), greater<>{}));
      assert(is_heap_until(rng(begin(buf), 1), greater<>{}) == nth(buf, 1));
      for (auto i : iters(begin(buf) + 1, end(buf))) {
        int tmp = -100;
        adl_swap(*i, tmp);
        assert(is_heap(rng(begin(buf), i), greater<>{}));
        assert(is_heap_until(buf, greater<>{}) == i);
        adl_swap(*i, tmp);
        assert(is_heap_until(buf, greater<>{}) == end(buf));
      }
      sort_heap(buf, greater<>{});
      assert(is_sorted(buf, greater<>{}));
    }
    assert(equal(v, irng(0, n)));
  }
  // heap_sort
  {
    ez_vector<int> v;
    copy(rrng(irng(0, 100)), to_back(v));
    heap_sort(v);
    assert(equal(v, irng(0, 100)));
    heap_sort(v, greater<>{});
    assert(equal(v, rrng(irng(0, 100))));
  }
}
void test_minmax() {
  {
    const int i = 3, j = 1;
    assert(min(i, j, greater<>{}) == 3);
    assert(min(i, j) == 1);
    assert(min({4, 5, 1, 3}, greater<>{}) == 5);
    assert(min({4, 5, 1, 3}) == 1);
    assert(max(i, j, greater<>{}) == 1);
    assert(max(i, j) == 3);
    assert(max({4, 5, 1, 3}, greater<>{}) == 1);
    assert(max({4, 5, 1, 3}) == 5);
    int ii, jj;
    tie(ii, jj) = minmax(i, j, greater<>{});
    assert(ii == 3 && jj == 1);
    tie(ii, jj) = minmax(i, j);
    assert(ii == 1 && jj == 3);
    tie(ii, jj) = minmax({4, 5, 1, 3});
    assert(ii == 1 && jj == 5);
    tie(ii, jj) = minmax({4, 5, 1, 3}, greater<>{});
    assert(ii == 5 && jj == 1);

    const ez_vector<int> v = {4, 5, 1, 3};
    assert(min_element(v) == begin(v) + 2);
    assert(min_element(v, greater<>{}) == begin(v) + 1);
    assert(max_element(v, greater<>{}) == begin(v) + 2);
    assert(max_element(v) == begin(v) + 1);
    assert(minmax_element(v)
	   == make_pair(begin(v) + 2, begin(v) + 1));
    assert(minmax_element(v, greater<>{})
	   == make_pair(begin(v) + 1, begin(v) + 2));
  }
  {
    ez_vector<int> vv;
    assert(min_element(vv) == end(vv));
    assert(max_element(vv) == end(vv));
    assert(minmax_element(vv) == pair(end(vv), end(vv)));
    vv.insert(vv.end(), 1);
    assert(min_element(vv) == begin(vv));
    assert(max_element(vv) == begin(vv));
    assert(minmax_element(vv) == pair(begin(vv), begin(vv)));
  }

  assert(min(1, 2) == 1);
  assert(min(2, 1) == 1);
  assert(min(3.25, 3.5, [](double x, double y) {return (int)x < (int)y;})
         == 3.25);
  assert(max(1, 2) == 2);
  assert(max(2, 1) == 2);
  assert(max(3.5, 3.25, [](double x, double y) {return (int)x < (int)y;})
	 == 3.5);
  assert(minmax({2}) == make_pair(2, 2));
  assert(minmax({2, 1}) == make_pair(1, 2));
  assert(minmax({1.25, 1.5},
                [](double x, double y) {return (int)x < (int)y;})
	 == make_pair(1.25, 1.5));
  assert(minmax({1.25, 1.5, 1.75},
                [](double x, double y) {return (int)x < (int)y;})
	 == make_pair(1.25, 1.75));
  assert(minmax({4.0, 1.0, 9.0, 1.2, 1.3, 5.0, 10.1, 10.2, 8.0, 10.25, 3.0},
                [](double x, double y) {return (int)x < (int)y;})
	 == make_pair(1.0, 10.25));
  assert(min({4.0, 1.0, 9.0, 1.2, 1.3, 5.0, 10.1, 10.2, 8.0, 10.3, 3.0},
             [](double x, double y) {return (int)x < (int)y;})
         == 1.0);
  assert(max({4.0, 1.1, 9.0, 1.2, 1.3, 5.0, 10.25, 10.2, 8.0, 10.3, 3.0},
             [](double x, double y) {return (int)x < (int)y;})
         == 10.25);
}
void test_bounded_value() {
  assert(clamp(-3, 0, 3) == 0);
  assert(clamp(-2, 0, 3) == 0);
  assert(clamp(-1, 0, 3) == 0);
  assert(clamp(-0, 0, 3) == 0);
  assert(clamp(1, 0, 3) == 1);
  assert(clamp(2, 0, 3) == 2);
  assert(clamp(3, 0, 3) == 3);
  assert(clamp(4, 0, 3) == 3);
  assert(clamp(5, 0, 3) == 3);
  assert(clamp(6, 0, 3) == 3);
}
void test_lexicographical_compare() {
  const auto assert_less = [](const auto &v, const auto &vv) {
    assert(lexicographical_compare(v, vv));
    assert(lexicographical_compare(v, vv, less<>{}));
    assert(!lexicographical_compare(vv, v));
    assert(!lexicographical_compare(vv, v, less<>{}));
    assert(lexicographical_compare_three_way(v, vv) < 0);
    assert(lexicographical_compare_three_way(vv, v) > 0);
    assert(lexicographical_synth_3way(v, vv) < 0);
    assert(lexicographical_synth_3way(vv, v) > 0);
  };
  const auto assert_equal = [](const auto &v, const auto &vv) {
    assert(!lexicographical_compare(v, vv));
    assert(!lexicographical_compare(v, vv, less<>{}));
    assert(!lexicographical_compare(vv, v));
    assert(!lexicographical_compare(vv, v, less<>{}));
    assert(lexicographical_compare_three_way(v, vv) == 0);
    assert(lexicographical_compare_three_way(vv, v) == 0);
    assert(lexicographical_synth_3way(v, vv) == 0);
    assert(lexicographical_synth_3way(vv, v) == 0);
  };
  using l_t = ez_forward_list<int>;
  assert_equal(l_t{}, l_t{});
  assert_less(l_t{}, l_t{1});
  assert_less(l_t{1}, l_t{2});
  assert_less(l_t{1, 2, 3, 4}, l_t{2, 2, 3, 4});
  assert_less(l_t{1, 2, 3, 4}, l_t{1, 3, 3, 4});
  assert_less(l_t{1, 2, 3, 4}, l_t{1, 2, 4, 4});
  assert_less(l_t{1, 2, 3, 4}, l_t{1, 2, 3, 5});
  assert_equal(l_t{1, 2, 3, 4}, l_t{1, 2, 3, 4});
  assert_less(l_t{1, 2, 3, 4}, l_t{1, 2, 3, 4, 0});
  assert_less(l_t{1, 2, 3, 4}, l_t{1, 2, 3, 4, 0, 0});
}
void test_permutation() {
  // next_permutation
  // prev_permutation
  {
    using v_t = ez_vector<int>;
    using vv_t = ez_vector<v_t>;
    v_t v = {1, 2, 3};
    vv_t vv;
    do {
      vv.insert(vv.end(), v);
    } while (next_permutation(v));
    assert(equal(v, irng(1, 4)));
    const vv_t v0 = {
      v_t{1, 2, 3}, v_t{1, 3, 2}, v_t{2, 1, 3},
      v_t{2, 3, 1}, v_t{3, 1, 2}, v_t{3, 2, 1}
    };
    assert(vv == v0);

    assert(!prev_permutation(v));
    assert(equal(v, rrng(irng(1, 4))));
    vv.erase(vv.begin(), vv.end());
    do {
      vv.insert(vv.end(), v);
    } while (prev_permutation(v));
    assert(equal(v, rrng(irng(1, 4))));
    assert(equal(vv, rrng(v0)));
  }
  {
    using v_t = ez_vector<int>;
    using vv_t = ez_vector<v_t>;
    for (int &i : iters(1, 8)) {
      v_t v;
      copy(irng(0, i), to_back(v));
      v_t buf = v;
      int c = 0;
      for (;;) {
        ++c;
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        const bool y = next_permutation(v);
        if (y)
          assert(buf < v);
        else
          assert(v < buf || i == 1);
        assert(is_permutation(buf, irng(0, i)));
        assert(is_permutation(v, irng(0, i)));
        prev_permutation(v);
        assert(v == buf);
        next_permutation(v);
        if (y == false)
          break;
      }
      assert(equal(v, irng(0, i)));
      assert(c == permutation_count(i));
    }
    for (int &i : iters(1, 8)) {
      v_t v;
      copy(rrng(irng(0, i)), to_back(v));
      v_t buf = v;
      int c = 0;
      for (;;) {
        ++c;
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        const bool y = next_permutation(v, greater<>{});
        if (y)
          assert(buf > v);
        else
          assert(v > buf || i == 1);
        assert(is_permutation(buf, irng(0, i)));
        assert(is_permutation(v, irng(0, i)));
        prev_permutation(v, greater<>{});
        assert(v == buf);
        next_permutation(v, greater<>{});
        if (y == false)
          break;
      }
      assert(equal(v, rrng(irng(0, i))));
      assert(c == permutation_count(i));
    }

    {
      const v_t v0 = {0, 1, 2, 3, 3, 3, 4, 5, 6};
      v_t v = v0;
      v_t buf = v;
      int c = 0;
      for (;;) {
        ++c;
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        const bool y = next_permutation(v);
        if (y)
          assert(buf < v);
        else
          assert(v < buf);
        assert(is_permutation(buf, v0));
        assert(is_permutation(v, v0));
        prev_permutation(v);
        assert(v == buf);
        next_permutation(v);
        if (y == false)
          break;
      }
      assert(equal(v, v_t{0, 1, 2, 3, 3, 3, 4, 5, 6}));
      assert(to_unsigned(c) == permutation_count(ssize(v))
             / to_unsigned(permutation_count(3)));
    }
  }
}
void test_sorting() {
  // is_sorted
  // is_sorted_until
  {
    ez_forward_list<int> v;
    for (int k : irng(0, 5)) {
      v.erase_after(v.before_begin(), v.end());
      assert(v.empty());
      copy(irng(0, k), to_front(v));
      assert(equal(v, irng(0, k)));
      assert(is_sorted(v, less<>{}));
      assert(is_sorted(v));
      assert(is_sorted_until(v, less<>{}) == end(v));
      assert(is_sorted_until(v) == end(v));
      if (k > 1)
        for (int i : irng(1, k)) {
          ref(v, i) -= 1000;
          assert(is_sorted_until(v, less<>{}) == nth(v, i));
          assert(is_sorted_until(v) == nth(v, i));
          ref(v, i) += 1000;
        }
    }
  }
  // sort
  {
    {
      ez_vector<int> v;
      sort(v);
      assert(empty(v) && is_sorted(v));
    }
    for (int k : irng(1, 5)) {
      ez_vector<int> v;
      copy(irng(0, k), to_back(v));
      ez_vector<int> buf = v;
      do {
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        sort(buf);
        assert(is_sorted(buf));
        assert(is_permutation(buf, irng(0, k)));
      } while (next_permutation(v));
    }
    for (int k : irng(10, 15)) {
      ez_vector<int> v;
      copy(irng(0, k), to_back(v));
      ez_vector<int> buf = v;
      unsigned long long c = 250000;
      do {
        if (c == 0)
          break;
        --c;
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        sort(buf);
        assert(is_sorted(buf));
        assert(is_permutation(buf, irng(0, k)));
      } while (next_permutation(v));

      v.erase(v.begin(), v.end());
      copy(rrng(irng(0, k)), to_back(v));
      c = 250000;
      do {
        if (c == 0)
          break;
        --c;
        buf.erase(buf.begin(), buf.end());
        copy(v, to_back(buf));
        sort(buf);
        assert(is_sorted(buf));
        assert(is_permutation(buf, irng(0, k)));
      } while (prev_permutation(v));
    }
    {
      ez_vector<int> v;
      copy(rrng(irng(0, 10000000)), to_back(v));
      sort(v);
      assert(equal(v, irng(0, 10000000)));
    }
  }
  // stable_sort
  {
    {
      ez_vector<int> v{};
      stable_sort(v);
      assert(empty(v));
      v = {1};
      stable_sort(v);
      assert(equal(v, rng(1, 1)));
      v = {2, 1};
      stable_sort(v);
      assert(equal(v, irng(1, 3)));
    }

    const ez_vector<pair<int, int>> v = {
      {1, 1}, {1, 2},
      {2, 1}, {2, 2}, {2, 3},
      {3, 1}, {3, 2},
      {4, 1}, {4, 2}, {4, 3},
      {5, 1}, {5, 2}
    };

    ez_vector<const pair<int, int> *> buf;
    ez_vector<const pair<int, int> *> buf2;
    const auto is_stable = [&buf, &buf2](const auto &x, const auto &y) {
      buf.erase(buf.begin(), buf.end());
      buf2.erase(buf2.begin(), buf2.end());
      for (int i : iters(1, 6)) {
        auto r = rng(bind_rng(rng(x), at<0>));
        auto rr = rng(bind_rng(rng(y), at<0>));
        for (;;) {
          auto it = find(r, i);
          if (it == end(r))
            break;
          buf.insert(buf.end(), it.base());
          r.first = next(it);
        }
        for (;;) {
          auto it = find(rr, i);
          if (it == end(rr))
            break;
          buf2.insert(buf2.end(), it.base());
          rr.first = next(it);
        }
        if (!equal(bind_rng(deref_rng(buf), at<1>),
                   bind_rng(deref_rng(buf2), at<1>)))
          return false;
      }
      return true;
    };

    const auto first_less = bind(less<>{}, bind(at<0>, _1), bind(at<0>, _2));
    ez_vector<pair<int, int>> vv_buf;
    for (int k : iters(0, ssize(v))) {
      ez_vector<pair<int, int>> vv;
      copy(rng(begin(v), k), to_back(vv));
      assert(equal(vv, v.begin()));
      do {
        vv_buf.erase(vv_buf.begin(), vv_buf.end());
        copy(vv, to_back(vv_buf));
        stable_sort(vv_buf, first_less);
        assert(is_sorted(vv_buf, first_less));
        assert(is_permutation(vv_buf, vv));
        assert(is_stable(vv_buf, vv));

        vv_buf.erase(vv_buf.begin(), vv_buf.end());
        copy(vv, to_back(vv_buf));
        stable_sort_with_no_buffer(vv_buf, first_less);
        assert(is_sorted(vv_buf, first_less));
        assert(is_permutation(vv_buf, vv));
        assert(is_stable(vv_buf, vv));

        vv_buf.erase(vv_buf.begin(), vv_buf.end());
        copy(vv, to_back(vv_buf));
        buffer<rng_vt<decltype(vv_buf)>> tmp_buf;
        stable_sort_with_buffer(vv_buf, tmp_buf, first_less);
        assert(is_sorted(vv_buf, first_less));
        assert(is_permutation(vv_buf, vv));
        assert(is_stable(vv_buf, vv));
      } while (next_permutation(vv, first_less));
    }
  }
  // partial_sort
  {
    {
      ez_vector<int> v;
      partial_sort(v, begin(v));
      assert(empty(v));
    }
    {
      ez_vector<int> v = {5, 4, 3, 2, 1};
      partial_sort(v, begin(v));
      assert(equal(v, rrng(irng(1, 6))));
      partial_sort(v, nth(v, 2));
      assert(equal(rng(begin(v), 2), irng(1, 3)));
      assert(is_permutation(v, irng(1, 6)));
      partial_sort(v, nth(v, 2), greater<>{});
      assert(equal(rng(begin(v), 2), rrng(irng(4, 6))));
      assert(is_permutation(v, irng(1, 6)));
    }
    {
      for (int k : irng(1, 9)) {
        for (int j : irng(1, k)) {
          ez_vector<int> v;
          ez_vector<int> vv;
          copy(irng(0, k), to_back(v));
          do {
            vv.erase(vv.begin(), vv.end());
            copy(v, to_back(vv));
            partial_sort(vv, nth(vv, j));
            assert(equal(rng(begin(vv), j), iitr(0)));
            assert(is_permutation(vv, irng(0, k)));
          } while (next_permutation(v));
        }
      }
    }
  }
  // partial_sort_copy
  {
    {
      ez_forward_list<int> l;
      ez_vector<int> v;
      assert(partial_sort_copy(l, v) == end(v));
      assert(partial_sort_copy(l, v, less<>{}) == end(v));
    }
    {
      for (int i : iters(1, 10)) {
        ez_list<int> l;
        copy(irng(0, i), to_back(l));
        assert(equal(l, irng(0, i)));

        ez_vector<int> v;
        do {
          for (int j : iters(0, 15)) {
            v.erase(v.begin(), v.end());
            copy(rng(j, 0), to_back(v));

            const auto it = partial_sort_copy(l, v);
            assert(it - begin(v) == min(i, j));
            assert(equal(rng(begin(v), it), iitr(0)));
          }
        } while (next_permutation(l));
      }
    }
  }
  // nth_element
  {
    {
      for (int i : iters(0, 10)) {
        ez_vector<int> v;
        copy(irng(0, i), to_back(v));
        ez_vector<int> vv;
        do {
          for (int j : iters(0, i)) {
            vv.erase(begin(vv), end(vv));
            copy(v, to_back(vv));
            auto it = nth(vv, j);
            nth_element(vv, it);
            assert(*it == j);
            assert(is_permutation(v, vv));

            vv.erase(begin(vv), end(vv));
            copy(v, to_back(vv));
            it = nth(vv, j);
            nth_element(vv, it, less<>{});
            assert(*it == j);
            assert(is_permutation(v, vv));
          }
          {
            vv.erase(begin(vv), end(vv));
            copy(v, to_back(vv));
            nth_element(vv, end(vv));
            assert(v == vv);
            nth_element(vv, end(vv), less<>{});
            assert(v == vv);
          }
        } while (next_permutation(v));
      }
    }
  }
}
void test_accumulate() {
  assert(accumulate(empty_rng<int>()) == 0);
  assert(accumulate(irng(1, 4)) == 6);
  assert(accumulate(irng(1, 4), 1) == 7);
  assert(accumulate(irng(1, 4), 1, minus<>{}) == -5);

  assert(accumulate_plus(empty_rng<int>()) == pair(nullptr, 0));
  assert(accumulate_plus(irng(1, 4)) == pair(iitr(4), 6));
  assert(accumulate_plus(irng(1, 4), 1) == pair(iitr(4), 7));
  assert(accumulate_plus(irng(1, 4), 1, minus<>{}) == pair(iitr(4), -5));

  assert(reduce(empty_rng<int>()) == 0);
  assert(reduce(irng(1, 4)) == 6);
  assert(reduce(irng(1, 4), 1) == 7);
  assert(reduce(irng(1, 4), 1, minus<>{}) == -5);

  assert(reduce_plus(empty_rng<int>()) == pair(nullptr, 0));
  assert(reduce_plus(irng(1, 4)) == pair(iitr(4), 6));
  assert(reduce_plus(irng(1, 4), 1) == pair(iitr(4), 7));
  assert(reduce_plus(irng(1, 4), 1, minus<>{}) == pair(iitr(4), -5));
}
void test_inner_product() {
  // inner_product(_plus)
  {
    assert(inner_product(empty_rng<int>(), begin(empty_rng<int>()), 0) == 0);
    assert(inner_product(irng(1, 4), iitr(2), 0) == 20);
    assert(inner_product(irng(1, 4), iitr(2), 2) == 22);
    assert(inner_product(ez_forward_list({2, 4, 6}),
                         begin(ez_forward_list{1, 3, 5}),
                         0, plus<>{}, multiplies<>{}) == 44);
    assert(inner_product(ez_forward_list({2, 4, 6}),
                         begin(ez_forward_list{1, 3, 5}),
                         -1, plus<>{}, multiplies<>{}) == 43);

    assert(inner_product_plus(empty_rng<int>(), begin(empty_rng<int>()), 0)
           == tuple(nullptr, nullptr, 0));
    assert(inner_product_plus(irng(1, 4), iitr(2), 0)
           == tuple(iitr(4), iitr(5), 20));
    assert(inner_product_plus(irng(1, 4), iitr(2), 2)
           == tuple(iitr(4), iitr(5), 22));
    assert(at<2>(inner_product_plus(ez_forward_list({2, 4, 6}),
                                    begin(ez_forward_list{1, 3, 5}),
                                    0, plus<>{}, multiplies<>{})) == 44);
    assert(at<2>(inner_product_plus(ez_forward_list({2, 4, 6}),
                                    begin(ez_forward_list{1, 3, 5}),
                                    -1, plus<>{}, multiplies<>{})) == 43);
  }
  // transform_reduce(_plus)
  {
    assert(transform_reduce_plus(empty_rng<int>(), begin(empty_rng<int>()), 0)
           == tuple(nullptr, nullptr, 0));
    assert(transform_reduce_plus(empty_rng<int>(), begin(empty_rng<int>()), 1,
                                 plus{}, multiplies{})
           == tuple(nullptr, nullptr, 1));
    assert(transform_reduce_plus(irng(1, 4), iitr(1), 0)
           == tuple(iitr(4), iitr(4), 14));
    assert(transform_reduce_plus(irng(1, 4), iitr(1), 0, plus{}, plus{})
           == tuple(iitr(4), iitr(4), 12));

    assert(transform_reduce_plus(empty_rng<int>(), 0, plus{}, identity{})
           == pair(nullptr, 0));
    assert(transform_reduce_plus(irng(1, 4), 0, plus{}, identity{})
           == pair(iitr(4), 6));
    assert(transform_reduce_plus(irng(1, 4), 1, plus{}, identity{})
           == pair(iitr(4), 7));
    assert(transform_reduce_plus(irng(1, 4), 1, plus{}, negate{})
           == pair(iitr(4), -5));

    assert(transform_reduce(empty_rng<int>(), begin(empty_rng<int>()), 0) == 0);
    assert(transform_reduce(empty_rng<int>(), begin(empty_rng<int>()), 1,
                            plus{}, multiplies{}) == 1);
    assert(transform_reduce(irng(1, 4), iitr(1), 0) == 14);
    assert(transform_reduce(irng(1, 4), iitr(1), 0, plus{}, plus{}) == 12);

    assert(transform_reduce(empty_rng<int>(), 0, plus{}, identity{}) == 0);
    assert(transform_reduce(irng(1, 4), 0, plus{}, identity{}) == 6);
    assert(transform_reduce(irng(1, 4), 1, plus{}, identity{}) == 7);
    assert(transform_reduce(irng(1, 4), 1, plus{}, negate{}) == -5);
  }
}
void test_partial_sum() {
  // partial_sum(_plus)
  {
    using v_t = ez_vector<int>;
    v_t v;
    assert(partial_sum_plus(irng(0, 5), to_back(v), minus{}).first == iitr(5));
    assert(v == v_t({0, -1, -3, -6, -10}));
    v.erase(v.begin(), v.end());
    assert(partial_sum_plus(irng(0, 5), to_back(v)).first == iitr(5));
    assert(v == v_t({0, 1, 3, 6, 10}));
    v.erase(v.begin(), v.end());

    partial_sum(irng(0, 1), to_back(v));
    assert(v == v_t({0}));
    v.erase(v.begin(), v.end());
    partial_sum(irng(0, 2), to_back(v));
    assert(v == v_t({0, 1}));
    v.erase(v.begin(), v.end());

    assert(partial_sum_plus(empty_rng<int>(), begin(v))
           == pair(nullptr, end(v)));
    assert(partial_sum_plus(empty_rng<int>(), begin(v), plus{})
           == pair(nullptr, end(v)));
    assert(partial_sum(empty_rng<int>(), begin(v)) == end(v));
    assert(partial_sum(empty_rng<int>(), begin(v), plus{}) == end(v));
  }
  // transform_exclusive_scan(_plus)
  // exclusive_scan(_plus)
  {
    using v_t = ez_vector<int>;
    v_t v;
    assert(transform_exclusive_scan_plus(irng(1, 5), to_back(v), -1,
                                         plus{}, negate{}).first
           == iitr(5));
    assert(v == v_t({-1, -2, -4, -7}));
    v.erase(v.begin(), v.end());

    assert(transform_exclusive_scan_plus(irng(0, 0), begin(v), -1,
                                         plus{}, negate{})
           == pair(iitr(0), end(v)));

    v.insert(v.end(), 0);
    assert(transform_exclusive_scan_plus(irng(0, 1), begin(v), -1,
                                         plus{}, negate{})
           == pair(iitr(1), end(v)));
    assert(v == v_t({-1}));
    v.erase(v.begin(), v.end());

    copy(rng(2, 0), to_back(v));
    assert(transform_exclusive_scan_plus(irng(0, 2), begin(v), -1,
                                         plus{}, negate{})
           == pair(iitr(2), end(v)));
    assert(v == v_t({-1, -1}));
    v.erase(v.begin(), v.end());

    assert(transform_exclusive_scan(irng(0, 0), begin(v), 0, plus{}, identity{})
           == end(v));
    assert(empty(v));

    copy(rng(3, 0), to_back(v));
    assert(transform_exclusive_scan(irng(1, 4), begin(v), 0, plus{}, identity{})
           == end(v));
    assert(v == v_t({0, 1, 3}));
    v.erase(v.begin(), v.end());

    copy(rng(3, 0), to_back(v));
    assert(exclusive_scan(irng(1, 4), begin(v), 0, plus{}) == end(v));
    assert(v == v_t({0, 1, 3}));
    v.erase(v.begin(), v.end());

    copy(rng(3, 0), to_back(v));
    assert(exclusive_scan_plus(irng(1, 4), begin(v), 0, minus{})
           == pair(iitr(4), end(v)));
    assert(v == v_t({0, -1, -3}));
    v.erase(v.begin(), v.end());

    copy(rng(3, 0), to_back(v));
    assert(exclusive_scan_plus(irng(1, 4), begin(v), 0)
           == pair(iitr(4), end(v)));
    assert(v == v_t({0, 1, 3}));
    v.erase(v.begin(), v.end());
  }
  // transform_inclusive_scan(_plus)
  // inclusive_scan(_plus)
  {
    using v_t = ez_vector<int>;
    v_t v;
    copy(rng(3, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 4), begin(v),
                                         minus{}, negate{})
           == pair(iitr(4), end(v)));
    assert(v == v_t({-1, 1, 4}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 4), begin(v),
                                         minus{}, negate{}, 10)
           == pair(iitr(4), end(v)));
    assert(v == v_t({11, 13, 16}));
    v.erase(v.begin(), v.end());

    copy(rng(2, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 3), begin(v),
                                         minus{}, negate{})
           == pair(iitr(3), end(v)));
    assert(v == v_t({-1, 1}));
    v.erase(v.begin(), v.end());
    copy(rng(2, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 3), begin(v),
                                         minus{}, negate{}, 10)
           == pair(iitr(3), end(v)));
    assert(v == v_t({11, 13}));
    v.erase(v.begin(), v.end());

    copy(rng(1, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 2), begin(v),
                                         minus{}, negate{})
           == pair(iitr(2), end(v)));
    assert(v == v_t({-1}));
    v.erase(v.begin(), v.end());
    copy(rng(1, 0), to_back(v));
    assert(transform_inclusive_scan_plus(irng(1, 2), begin(v),
                                         minus{}, negate{}, 10)
           == pair(iitr(2), end(v)));
    assert(v == v_t({11}));
    v.erase(v.begin(), v.end());

    assert(transform_inclusive_scan_plus(irng(1, 1), begin(v),
                                         minus{}, negate{})
           == pair(iitr(1), end(v)));
    assert(empty(v));
    assert(transform_inclusive_scan_plus(irng(1, 1), begin(v),
                                         minus{}, negate{}, 10)
           == pair(iitr(1), end(v)));
    assert(empty(v));

    copy(rng(3, 0), to_back(v));
    assert(transform_inclusive_scan(irng(1, 4), begin(v), minus{}, negate{})
           == end(v));
    assert(v == v_t({-1, 1, 4}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(transform_inclusive_scan(irng(1, 4), begin(v), minus{}, negate{}, 10)
           == end(v));
    assert(v == v_t({11, 13, 16}));
    v.erase(v.begin(), v.end());

    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan_plus(irng(1, 4), begin(v))
           == pair(iitr(4), end(v)));
    assert(v == v_t({1, 3, 6}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan_plus(irng(1, 4), begin(v), minus{})
           == pair(iitr(4), end(v)));
    assert(v == v_t({1, -1, -4}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan_plus(irng(1, 4), begin(v), minus{}, 10)
           == pair(iitr(4), end(v)));
    assert(v == v_t({9, 7, 4}));
    v.erase(v.begin(), v.end());

    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan(irng(1, 4), begin(v)) == end(v));
    assert(v == v_t({1, 3, 6}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan(irng(1, 4), begin(v), minus{}) == end(v));
    assert(v == v_t({1, -1, -4}));
    v.erase(v.begin(), v.end());
    copy(rng(3, 0), to_back(v));
    assert(inclusive_scan(irng(1, 4), begin(v), minus{}, 10) == end(v));
    assert(v == v_t({9, 7, 4}));
    v.erase(v.begin(), v.end());

    assert(inclusive_scan(irng(0, 0), begin(v)) == end(v));
    assert(empty(v));
  }
}
void test_adjacent_difference() {
  using v_t = ez_vector<int>;
  v_t v;
  assert(adjacent_difference_plus(irng(0, 0), begin(v), plus{})
         == pair(iitr(0), end(v)));
  assert(empty(v));
  assert(adjacent_difference_plus(irng(0, 0), begin(v))
         == pair(iitr(0), end(v)));
  assert(empty(v));
  assert(adjacent_difference(irng(0, 0), begin(v), plus{}) == end(v));
  assert(empty(v));
  assert(adjacent_difference(irng(0, 0), begin(v)) == end(v));
  assert(empty(v));

  copy(rng(1, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 2), begin(v), plus{})
         == pair(iitr(2), end(v)));
  assert(v == v_t({1}));
  v.erase(v.begin(), v.end());
  copy(rng(1, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 2), begin(v))
         == pair(iitr(2), end(v)));
  assert(v == v_t({1}));
  v.erase(v.begin(), v.end());
  copy(rng(1, 0), to_back(v));
  assert(adjacent_difference(irng(1, 2), begin(v), plus{}) == end(v));
  assert(v == v_t({1}));
  v.erase(v.begin(), v.end());
  copy(rng(1, 0), to_back(v));
  assert(adjacent_difference(irng(1, 2), begin(v)) == end(v));
  assert(v == v_t({1}));
  v.erase(v.begin(), v.end());

  copy(rng(2, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 3), begin(v), plus{})
         == pair(iitr(3), end(v)));
  assert(v == v_t({1, 3}));
  v.erase(v.begin(), v.end());
  copy(rng(2, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 3), begin(v))
         == pair(iitr(3), end(v)));
  assert(v == v_t({1, 1}));
  v.erase(v.begin(), v.end());
  copy(rng(2, 0), to_back(v));
  assert(adjacent_difference(irng(1, 3), begin(v), plus{}) == end(v));
  assert(v == v_t({1, 3}));
  v.erase(v.begin(), v.end());
  copy(rng(2, 0), to_back(v));
  assert(adjacent_difference(irng(1, 3), begin(v)) == end(v));
  assert(v == v_t({1, 1}));
  v.erase(v.begin(), v.end());

  copy(rng(4, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 5), begin(v), plus{})
         == pair(iitr(5), end(v)));
  assert(v == v_t({1, 3, 5, 7}));
  v.erase(v.begin(), v.end());
  copy(rng(4, 0), to_back(v));
  assert(adjacent_difference_plus(irng(1, 5), begin(v))
         == pair(iitr(5), end(v)));
  assert(v == v_t({1, 1, 1, 1}));
  v.erase(v.begin(), v.end());
  copy(rng(4, 0), to_back(v));
  assert(adjacent_difference(irng(1, 5), begin(v), plus{}) == end(v));
  assert(v == v_t({1, 3, 5, 7}));
  v.erase(v.begin(), v.end());
  copy(rng(4, 0), to_back(v));
  assert(adjacent_difference(irng(1, 5), begin(v)) == end(v));
  assert(v == v_t({1, 1, 1, 1}));
  v.erase(v.begin(), v.end());
}
void test_iota() {
  using l_t = ez_forward_list<int>;
  l_t l;

  iota(l, 1);
  assert(empty(l));

  copy(rng(1, 0), to_front(l));
  iota(l, 1);
  assert(l == l_t({1}));
  l.erase_after(l.before_begin(), l.end());

  copy(rng(2, 0), to_front(l));
  iota(l, 1);
  assert(l == l_t({1, 2}));
  l.erase_after(l.before_begin(), l.end());

  copy(rng(3, 0), to_front(l));
  iota(l, 1);
  assert(l == l_t({1, 2, 3}));
  l.erase_after(l.before_begin(), l.end());

  copy(rng(8, 0), to_front(l));
  iota(l, 1);
  assert(equal(l, irng(1, 9)));
  l.erase_after(l.before_begin(), l.end());
}
void test_gcd_and_lcm() {
  // gcd
  {
    assert(gcd(0, 0) == 0);
    assert(gcd(0, 5) == 5);
    assert(gcd(5, 0) == 5);
    assert(gcd(20, 30) == 10);
    assert(gcd(30, 20) == 10);
    assert(gcd(15, 17) == 1);
    assert(gcd(17, 15) == 1);
    assert(gcd(18, 15) == 3);
    assert(gcd(15, 18) == 3);

    assert(gcd(18u, 15u) == 3);
    assert(gcd(15u, 18u) == 3);
    assert(gcd(18, 15u) == 3);
    assert(gcd(15, 18u) == 3);
    assert(gcd(18u, 15) == 3);
    assert(gcd(15u, 18) == 3);
    assert(gcd(-5, 5) == 5);
    assert(gcd(-5, -5) == 5);
    assert(gcd(5, -5) == 5);
  }

  // lcm
  {
    assert(lcm(0, 0) == 0);
    assert(lcm(0, 6) == 0);
    assert(lcm(6, 0) == 0);
    assert(lcm(20, 25) == 100);
    assert(lcm(25, 20) == 100);
    assert(lcm(5, 6) == 30);
    assert(lcm(6, 5) == 30);
    assert(lcm(14, 13) == 14 * 13);
    assert(lcm(13, 14) == 14 * 13);

    assert(lcm(18u, 15u) == 90);
    assert(lcm(15u, 18u) == 90);
    assert(lcm(18, 15u) == 90);
    assert(lcm(15, 18u) == 90);
    assert(lcm(18u, 15) == 90);
    assert(lcm(15u, 18) == 90);
    assert(lcm(-5, 5) == 5);
    assert(lcm(-5, -5) == 5);
    assert(lcm(5, -5) == 5);
  }
}
void test_midpoint() {
  // for integral
  {
    assert(midpoint(3, 1) == 2);
    assert(midpoint(1, 3) == 2);
    assert(midpoint(4, 1) == 3);
    assert(midpoint(1, 4) == 2);
    assert(midpoint(3u, 1u) == 2);
    assert(midpoint(1u, 3u) == 2);
    assert(midpoint(4u, 1u) == 3);
    assert(midpoint(1u, 4u) == 2);

    assert(midpoint(integral_traits<int8_t>::min(),
                    integral_traits<int8_t>::max()) == -1);
    assert(midpoint(integral_traits<int8_t>::max(),
                    integral_traits<int8_t>::min()) == 0);

    assert(midpoint(integral_traits<uint8_t>::min(),
                    integral_traits<uint8_t>::max()) == 127);
    assert(midpoint(integral_traits<uint8_t>::max(),
                    integral_traits<uint8_t>::min()) == 128);

    assert(midpoint(integral_traits<intmax_t>::min(),
                    integral_traits<intmax_t>::max()) == -1);
    assert(midpoint(integral_traits<intmax_t>::max(),
                    integral_traits<intmax_t>::min()) == 0);
  }
  // for pointer
  {
    int a[5] = {};
    assert(midpoint(begin(a), end(a)) == nth(a, 2));
    assert(midpoint(end(a), begin(a)) == nth(a, 3));
    assert(midpoint(begin(a), begin(a)) == begin(a));

    int b[4] = {};
    assert(midpoint(begin(b), end(b)) == nth(b, 2));
    assert(midpoint(end(b), begin(b)) == nth(b, 2));
    assert(midpoint(begin(b), begin(b)) == begin(b));
  }

  // for floating point

  const auto put_info = [](auto x) {
    // for bug tracing
    puti(x.sign);
    putb(x.e);
    putb(x.f);
  };

  // NAN
  {
    // float
    {
      using ts = floating_point_traits<float>;
      assert(ts::is_nan(midpoint(NAN, NAN)));
      assert(ts::is_nan(midpoint(NAN, 1.0f)));
      assert(ts::is_nan(midpoint(1.0f, NAN)));
    }
    // double
    {
      using ts = floating_point_traits<double>;
      assert(ts::is_nan(midpoint(ts::nan(), ts::nan())));
      assert(ts::is_nan(midpoint(ts::nan(), 1.0)));
      assert(ts::is_nan(midpoint(1.0, ts::nan())));
    }
  }
  // overflow
  {
    // float
    {
      using ts = floating_point_traits<float>;

      assert(ts::is_infinity(midpoint(ts::infinity(), ts::max())));
      assert(ts::is_infinity(midpoint(ts::max(), ts::infinity())));

      assert(ts::is_infinity(midpoint(ts::lowest(), ts::infinity())));
      assert(ts::is_infinity(midpoint(ts::infinity(), ts::lowest())));

      assert(ts::is_infinity(midpoint(1.0f, ts::infinity())));
      assert(ts::is_infinity(midpoint(ts::infinity(), 1.0f)));
    }
    // double
    {
      using ts = floating_point_traits<double>;

      assert(ts::is_infinity(midpoint(ts::infinity(), ts::max())));
      assert(ts::is_infinity(midpoint(ts::max(), ts::infinity())));

      assert(ts::is_infinity(midpoint(ts::lowest(), ts::infinity())));
      assert(ts::is_infinity(midpoint(ts::infinity(), ts::lowest())));

      assert(ts::is_infinity(midpoint(1.0, ts::infinity())));
      assert(ts::is_infinity(midpoint(ts::infinity(), 1.0)));
    }
  }

  // usual
  {
    assert(midpoint(1.0, 2.0) == 1.5);
    assert(midpoint(2.0, 1.0) == 1.5);
    assert(midpoint(1.0f, 2.0f) == 1.5f);
    assert(midpoint(2.0f, 1.0f) == 1.5f);
  }

  // for divide 2 with precision losing
  {
    // float
    {
      using ts = floating_point_traits<float>;
      using float_t = ts::float_t;
      const float_t f = ts::min();
      const float_t f2 = midpoint(0.0f, f);
      const float_t f3 = midpoint(0.0f, -f);
      assert(f2 == ts::make(true, 0, 0b100'00000'00000'00000'00000u));
      assert(f3 == ts::make(false, 0, 0b100'00000'00000'00000'00000u));

      assert(midpoint(ts::make(true, 0, 0b111u),
                      ts::make(false, 0, 0b111u)) == 0.0f);
      assert(midpoint(ts::make(true, 0, 0b110u),
                      ts::make(false, 0, 0b111u)) == ts::make(true, 0, 0b0u));
      assert(midpoint(ts::make(false, 0, 0b111u),
                      ts::make(true, 0, 0b110u)) == ts::make(false, 0, 0b1u));

      assert(ts::make(true, 2u, 0u) == (ts::min() * 2));
      assert(midpoint
             (ts::make(0, 1u, 0b100'0000000000'0000000000u),
              ts::make(1, 2u, 0b100'0000000000'0000000000u))
             == ts::make(1, 0u, 0b110'0000000000'0000000000u));
      assert(midpoint
             (ts::make(0, 1u, 0b100'0000000000'0000000001u),
              ts::make(1, 2u, 0b100'0000000000'0000000000u))
             == ts::make(1, 0u, 0b101'1111111111'1111111111u));
      assert(midpoint
             (ts::make(1, 2u, 0b100'0000000000'0000000000u),
              ts::make(0, 1u, 0b100'0000000000'0000000001u))
             == ts::make(1, 0u, 0b110'0000000000'0000000000u));
    }
    // double
    {
      using ts = floating_point_traits<double>;
      using float_t = ts::float_t;
      const float_t f = ts::min();
      const float_t f2 = midpoint(0.0, f);
      const float_t f3 = midpoint(0.0, -f);
      assert(f2
             == ts::make
             (true, 0,
              0b10'0000000000'0000000000'0000000000'0000000000'0000000000u));
      assert(f3
             == ts::make
             (false, 0,
              0b10'0000000000'0000000000'0000000000'0000000000'0000000000u));

      assert(midpoint(ts::make(true, 0, 0b111u),
                      ts::make(false, 0, 0b111u)) == 0.0f);
      assert(midpoint(ts::make(true, 0, 0b110u),
                      ts::make(false, 0, 0b111u)) == ts::make(true, 0, 0b0u));
      assert(midpoint(ts::make(false, 0, 0b111u),
                      ts::make(true, 0, 0b110u)) == ts::make(false, 0, 0b1u));

      assert(ts::make(true, 2u, 0u) == (ts::min() * 2));
      assert(midpoint
             (ts::make
              (0, 1u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000000u),
              ts::make
              (1, 2u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000000u))
             == ts::make
             (1, 0u,
              0b11'0000000000'0000000000'0000000000'0000000000'0000000000u));
      assert(midpoint
             (ts::make
              (0, 1u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000001u),
              ts::make
              (1, 2u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000000u))
             == ts::make
             (1, 0u,
              0b10'1111111111'1111111111'1111111111'1111111111'1111111111u));
      assert(midpoint
             (ts::make
              (1, 2u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000000u),
              ts::make
              (0, 1u,
               0b10'0000000000'0000000000'0000000000'0000000000'0000000001u))
             == ts::make
             (1, 0u,
              0b11'0000000000'0000000000'0000000000'0000000000'0000000000u));
    }
  }
}
void test_for_each_node() {
  ez_list<test_object<int>> z;
  for_each_node(z, next, [&z](auto it) {z.erase(it);});
  assert(z.empty());    
  ez_list<test_object<int>> l = {1, 2, 3};
  for_each_node(l, next, [&l](auto it) {l.erase(it);});
  assert(l.empty());
}
void test_list_fns() {
  // list_functions
  {
    struct node_base_t {
      node_base_t *next;
      node_base_t *prev;
    };
    struct node_t : node_base_t {
      int data;
    };
    const auto get_next = [](auto it) {return it->next;};
    const auto set_next = [](auto it, auto x) {it->next = x;};
    const auto get_prev = [](auto it) {return it->prev;};
    const auto set_prev = [](auto it, auto x) {it->prev = x;};
    const auto fns = list_fns(get_next, set_next, get_prev, set_prev);

    const auto make_rng = bind(composite_brng,
                               _1, _2,
                               [](node_base_t *i) {
                                 return static_cast<node_t *>(i)->data;
                               },
                               [=](auto &i) {i = get_next(i);},
                               equal_to{},
                               [=](auto &i) {i = get_prev(i);});
    {
      node_t a[4];
      for_each(irng(0, 4), iitr(begin(a)),
               [](int x, node_t *i) {i->data = x;});
      assert(equal(a, irng(0, 4), [](auto &n, int x) {return n.data == x;}));
      node_base_t end_node = {addressof(end_node), addressof(end_node)};
      node_base_t *ed = addressof(end_node);
      assert(empty(make_rng(get_next(ed), ed)));

      node_base_t *it = fns.link(ed,
                                 static_cast<node_base_t *>(addressof(a[0])));
      assert(it == static_cast<node_base_t *>(addressof(a[0])));
      assert(static_cast<node_t *>(it)->data == 0);
      assert(get_next(ed) == it);
      assert(get_prev(ed) == it);
      assert(get_next(it) == ed);
      assert(get_prev(it) == ed);
      assert(equal(make_rng(get_next(ed), ed), irng(0, 1)));
      fns.link(ed, static_cast<node_base_t *>(addressof(a[1])));
      fns.link(ed, static_cast<node_base_t *>(addressof(a[2])));
      fns.link(ed, static_cast<node_base_t *>(addressof(a[3])));
      assert(equal(make_rng(get_next(ed), ed), irng(0, 4)));

      node_t z;
      z.data = -1;
      it = fns.link(get_next(ed), static_cast<node_base_t *>(addressof(z)));
      assert(it == static_cast<node_base_t *>(addressof(z)));
      assert(static_cast<node_t *>(it)->data == -1);
      assert(equal(make_rng(get_next(ed), ed), irng(-1, 4)));

      node_t zz;
      zz.data = 22;
      assert(fns.link(get_next(get_next(get_next(ed))),
                      static_cast<node_base_t *>(addressof(zz)))
             == static_cast<node_base_t *>(addressof(zz)));
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 22, 1, 2, 3})));

      it = get_next(get_next(get_next(ed)));
      assert(static_cast<node_t *>(fns.unlink(it))->data == 1);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 1, 2, 3})));
      fns.link(ed, it, it);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 1, 2, 3, 22})));

      it = get_next(get_next(ed));
      auto it2 = get_next(get_next(get_next(get_next(ed))));
      assert(fns.unlink(it, it2) == it2);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 2, 3, 22})));

      node_t n2;
      n2.prev = addressof(n2);
      n2.next = addressof(n2);
      node_base_t *ed2 = addressof(n2);
      assert(empty(make_rng(get_next(ed2), ed2)));
      it2 = get_next(it);
      assert(fns.link(ed2, it, it2) == it);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 2, 3, 22})));
      assert(equal(make_rng(get_next(ed2), ed2), irng(0, 2)));

      fns.unlink_all(ed);
      fns.unlink_all(ed2);
      assert(empty(make_rng(get_next(ed), ed)));
      assert(empty(make_rng(get_next(ed2), ed2)));
    }
  }
  // forward_list_functions
  {
    struct node_base_t {
      node_base_t *next;
    };
    struct node_t : node_base_t {
      int data;
    };
    const auto get_next = [](auto it) {return it->next;};
    const auto set_next = [](auto it, auto x) {it->next = x;};
    const auto fns = forward_list_fns(get_next, set_next);

    const auto make_rng = bind(composite_frng,
                               _1, _2,
                               [](node_base_t *i) {
                                 return static_cast<node_t *>(i)->data;
                               },
                               [=](auto &i) {i = get_next(i);},
                               equal_to{});
    {
      node_t a[4];
      for_each(irng(0, 4), iitr(begin(a)),
               [](int x, node_t *i) {i->data = x;});
      assert(equal(a, irng(0, 4), [](auto &n, int x) {return n.data == x;}));
      node_base_t end_node(addressof(end_node));
      node_base_t *ed = addressof(end_node);
      assert(empty(make_rng(get_next(ed), ed)));

      node_base_t *it
        = fns.link_after(ed,
                         static_cast<node_base_t *>(addressof(a[0])));
      assert(it == static_cast<node_base_t *>(addressof(a[0])));
      assert(static_cast<node_t *>(it)->data == 0);
      assert(get_next(ed) == it);
      assert(get_next(it) == ed);
      assert(equal(make_rng(get_next(ed), ed), irng(0, 1)));
      it = fns.link_after(it, static_cast<node_base_t *>(addressof(a[1])));
      it = fns.link_after(it, static_cast<node_base_t *>(addressof(a[2])));
      it = fns.link_after(it, static_cast<node_base_t *>(addressof(a[3])));
      assert(equal(make_rng(get_next(ed), ed), irng(0, 4)));

      node_t z;
      z.data = -1;
      it = fns.link_after(ed, static_cast<node_base_t *>(addressof(z)));
      assert(it == static_cast<node_base_t *>(addressof(z)));
      assert(static_cast<node_t *>(it)->data == -1);
      assert(equal(make_rng(get_next(ed), ed), irng(-1, 4)));

      node_t zz;
      zz.data = 22;
      assert(fns.link_after(get_next(get_next(ed)),
                            static_cast<node_base_t *>(addressof(zz)))
             == static_cast<node_base_t *>(addressof(zz)));
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 22, 1, 2, 3})));

      it = call_by_n_times(get_next, ed, 2);
      auto it2 = get_next(it);
      assert(static_cast<node_t *>(fns.unlink_after(it))->data == 1);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 1, 2, 3})));
      assert(fns.link_after(call_by_n_times(get_next, ed, 5), it2) == it2);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 0, 1, 2, 3, 22})));

      auto it3 = call_by_n_times(get_next, ed, 1);
      it = get_next(it3);
      it2 = call_by_n_times(get_next, ed, 4);
      assert(fns.unlink_after(it3, it2) == it2);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 2, 3, 22})));

      node_t n2;
      n2.next = addressof(n2);
      node_base_t *ed2 = addressof(n2);
      assert(empty(make_rng(get_next(ed2), ed2)));
      it2 = get_next(it);
      assert(fns.link_after(ed2, it, it2) == it);
      assert(equal(make_rng(get_next(ed), ed),
                   ez_vector({-1, 2, 3, 22})));
      assert(equal(make_rng(get_next(ed2), ed2), irng(0, 2)));

      fns.unlink_all(ed);
      fns.unlink_all(ed2);
      assert(empty(make_rng(get_next(ed), ed)));
      assert(empty(make_rng(get_next(ed2), ed2)));
    }
  }
}
void test_list_unique() {
  ez_list<int> l;
  list_unique(l);
  assert(l.empty() && l.size() == 0);

  l = {1, 2, 2, 3, 4, 4, 4, 5};
  list_unique(l);
  test_bitr(l, irng(1, 6));

  l = {1, 2, 3, 4, 4, 6};
  list_unique(l, [](int x, int y) {return x + 1 == y;});
  test_bitr(l, ez_vector{1, 3, 6});
  l = {};
  list_unique(l, [](int x, int y) {return x + 1 == y;});
  assert(l.empty() && l.size() == 0);

  ez_list<int> ll;
  l = {0, 0, 0, 1, 1, 2, 3, 4, 4, 5};
  list_unique(l, equal_to{}, [&](auto i) {
    auto ret = l.unlink(i);
    ll.link(ll.end(), i);
    return ret;
  });
  assert(equal(l, irng(0, 6)));
  assert(equal(ll, ez_vector{0, 0, 1, 4}));
}
void test_two_way_search() {
  // inner::fns::maximal_suffix_and_its_period
  {
    // empty
    {
      auto r = empty_rng<char>();
      auto [it, p] = inner::fns::maximal_suffix_and_its_period(r);
      assert(it == r.end());
      assert(p == 0);
      tie(it, p) = inner::fns::maximal_suffix_and_its_period
        (r, bind(synth_3way, _2, _1));
      assert(it == r.end());
      assert(p == 0);
    }
    // 1 element
    {
      auto r = single_rng('a');
      auto [it, p] = inner::fns::maximal_suffix_and_its_period(r);
      assert(it == r.begin());
      assert(p == 1);
      tie(it, p) = inner::fns::maximal_suffix_and_its_period
        (r, bind(synth_3way, _2, _1));
      assert(it == r.begin());
      assert(p == 1);
    }
    // 2 elements
    {
      ez_vector<char> v;
      v = {'a', 'a'};
      auto [it, p] = inner::fns::maximal_suffix_and_its_period(v);
      assert(it == v.begin());
      assert(p == 1);
      tie(it, p) = inner::fns::maximal_suffix_and_its_period
        (v, bind(synth_3way, _2, _1));
      assert(it == v.begin());
      assert(p == 1);
      v = {'a', 'b'};
      tie(it, p) = inner::fns::maximal_suffix_and_its_period(v);
      assert(it == before_end(v));
      assert(p == 1);
      tie(it, p) = inner::fns::maximal_suffix_and_its_period
        (v, bind(synth_3way, _2, _1));
      assert(it == v.begin());
      assert(p == 2);
    }
    // 7 elements
    {
      const auto get_period = []<class R>(R &&r)->rng_dft<R> {
        auto r_sz = ssize(r);
        if (r_sz == 0)
          return 0;
        else if (r_sz == 1)
          return 1;
        else {
          int p = r_sz;
          for (auto &possible_p : iters(1, r_sz)) {
            bool y = true;
            for (auto &j : iters(0, r_sz)) {
              for (auto k = j + possible_p; k < r_sz; k += possible_p) {
                if (ref(r, j) != ref(r, k)) {
                  y = false;
                  goto quit_lbl;
                }
              }
            }
          quit_lbl:
            if (y) {
              p = possible_p;
              break;
            }
          }
          return p;
        }
      };
      ez_vector<int> v;
      const auto r = irng(0, 4);
      for (int i1 : r)
        for (int i2 : r)
          for (int i3 : r)
            for (int i4 : r)
              for (int i5 : r)
                for (int i6 : r)
                  for (int i7 : r) {
                    v = {i1, i2, i3, i4, i5, i6, i7};
                    ez_vector<ez_vector<int>> vv;
                    for (int i : irng(0, 7)) {
                      vv.insert(vv.end(), {});
                      auto &back_v = back(vv);
                      for (int j : rng(nth(v, i), v.end()))
                        back_v.insert(back_v.end(), j);
                    }
                    sort(vv);
                    auto [it, p] = inner::fns::maximal_suffix_and_its_period(v);
                    test_rng(back(vv), rng(it, v.end()));
                    assert(p == get_period(back(vv)));
                  }
    }
  }
  // two_way_search
  {
    FILE *fp;
    fp = fopen("./main.cpp", "r+b");
    assert(fp != nullptr);
    ez_vector<char> s;
    int c;
    while ((c = fgetc(fp)) != EOF)
      s.insert(s.end(), c);
    fclose(fp);
    // for_each(s, putchar);

    ez_vector<ez_vector<char>> vv;
    auto it = s.begin();
    const auto check_char = [](char c) {
      return isdigit(c) || isalpha(c) || c == '_';
    };
    while (it != s.end()) {
      if (check_char(*it)) {
        auto it2 = next(it);
        while (it2 != s.end() && check_char(*it2))
          ++it2;
        {
          ez_vector<char> tmp_v;
          for (char x : rng(it, it2))
            tmp_v.insert(tmp_v.end(), x);
          if (!contains(vv, tmp_v))
            vv.insert(vv.end(), move(tmp_v));
        }
        it = it2;
      }
      else
        ++it;
    }

    sort(vv);
    // for (auto &x : vv) {for_each(x, putchar); putchar('\n');}

    ez_vector<int> all_pos_v0;
    ez_vector<int> all_pos_v;
    for (const auto &word : vv) {
      all_pos_v0.erase(all_pos_v0.begin(), all_pos_v0.end());
      all_pos_v.erase(all_pos_v.begin(), all_pos_v.end());

      auto i = s.begin();
      while ((i = search(rng(i, s.end()), word)) != s.end()) {
        assert(i + ssize(word) <= s.end());
        all_pos_v0.insert(all_pos_v0.end(), i - s.begin());
        i += ssize(word);
      }
      i = s.begin();
      while ((i = two_way_search(rng(i, s.end()), word)) != s.end()) {
        assert(i + ssize(word) <= s.end());
        all_pos_v.insert(all_pos_v.end(), i - s.begin());
        i += ssize(word);
      }
      assert(all_pos_v0 == all_pos_v);
    }
  }
}

void test_algorithm() {
  printf("algorithm: ");

  test_equal();
  test_allanynone_of();
  test_for_each();
  test_find();
  test_find_last();
  test_find_first_of();
  test_find_last_of();
  test_adjacent_find();
  test_count();
  test_mismatch();
  test_is_permutation();
  test_find_subrange();
  test_search();
  test_find_end();
  test_contains();
  test_starts_ends_with();
  test_fold_left_right();
  test_copy_move();
  test_swap_ranges();
  test_transform();
  test_replace();
  test_fill();
  test_generate();
  test_remove();
  test_unique();
  test_reverse();
  test_rotate();
  test_shift();
  test_shuffle();
  test_sample();
  test_binary_search();
  test_partition();
  test_merge();
  test_set_operations();
  test_heap_operations();
  test_minmax();
  test_bounded_value();
  test_lexicographical_compare();
  test_permutation();
  test_sorting();
  test_accumulate();
  test_inner_product();
  test_partial_sum();
  test_adjacent_difference();
  test_iota();
  test_gcd_and_lcm();
  test_midpoint();
  test_for_each_node();
  test_list_fns();
  test_list_unique();
  test_two_way_search();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_algorithm();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
