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

void test_hashtable_adaptor() {
  using table_t = hashtable_adaptor<hashtable_traits
                                    <hashtable_node<int>, 0, 1,
                                     stateful_test_allocator<int>>>;
  const auto eq = [](const auto &s, auto &&r) {
    if (!(s.empty() == empty(r)))
      return false;

    if (!(s.size() == size(r)))
      return false;

    if (!is_permutation(s, r))
      return false;

    if (!is_permutation(rrng(s), r))
      return false;

    {
      ez_vector<int> v;
      for (auto &i : iters(0u, s.bucket_count()))
        for (auto &x : s.bucket_range(i)) {
          v.insert(v.end(), x);
          if (!(hash{}(x) % s.bucket_count() == i))
            return false;
        }
      if (!is_permutation(v, r))
        return false;
    }

    if (!(s.active_bucket_count() <= s.bucket_count()))
      return false;

    {
      size_t c = 0;
      for (auto &i : iters(0u, s.bucket_count()))
        c += s.bucket_size(i);
      if (!(c == s.size()))
        return false;
    }

    return true;
  };
  // new_node
  // delete_node
  {
    table_t s;
    table_t::iterator it = s.new_node(1);
    assert(*it == 1);
    s.delete_node(it);
  }
  // range operations
  {
    static_assert(bidirectional_iterator<table_t::iterator>);
    static_assert(bidirectional_iterator<table_t::const_iterator>);
    static_assert(is_just_bitr<table_t::iterator>);
    static_assert(is_just_bitr<table_t::const_iterator>);    
    static_assert(regular<table_t::iterator>);
    static_assert(regular<table_t::const_iterator>);
    static_assert(is_nothrow_copyable_v<table_t::iterator>);
    static_assert(is_nothrow_copyable_v<table_t::const_iterator>);
    static_assert(is_nothrow_swappable_v<table_t::iterator>);
    static_assert(is_nothrow_swappable_v<table_t::const_iterator>);

    table_t s;
    assert(s.empty());
    assert(as_const(s).empty());
    assert(s.size() == 0);
    assert(as_const(s).size() == 0);
    test_bitr(s.begin(), s.end(), empty_rng<int>());
    test_bitr(s.cbegin(), s.end(), empty_rng<int>());
    test_bitr(s.begin(), s.cend(), empty_rng<int>());
    test_bitr(s.cbegin(), s.cend(), empty_rng<int>());

    s.max_load_factor(10.0);
    for (int x : irng(0, 13))
      s.insert(as_const, hash{}, equal_to{}, x,
               [&s, x]() {return s.new_node(x);}, empty_function{});
    assert(eq(s, irng(0, 13)));
    assert(eq(as_const(s), irng(0, 13)));
  }
  // find
  // count
  // equal_range
  {
    // empty
    {
      table_t s;
      assert(s.find(as_const, hash{}, equal_to{}, 0) == s.end());
      assert(s.find(as_const, hash{}, equal_to{}, 1) == s.end());
      assert(s.count(as_const, hash{}, equal_to{}, 0) == 0);
      assert(s.count(as_const, hash{}, equal_to{}, 1) == 0);
      assert(empty(s.equal_range(as_const, hash{}, equal_to{}, 0)));
      assert(empty(s.equal_range(as_const, hash{}, equal_to{}, 1)));
    }
    // every bucket has 1 value at most
    {
      table_t s;
      for (int i : irng(0, 7))
        s.insert(as_const, hash{}, equal_to{}, i);
      assert(s.bucket_count() == 13);
      assert(s.active_bucket_count() == 7);
      assert(eq(s, irng(0, 7)));
      for (int i : irng(0, 7)) {
        const auto it = s.find(as_const, hash{}, equal_to{}, i);
        assert(it != s.end() && *it == i);
        assert(s.count(as_const, hash{}, equal_to{}, i) == 1);
        assert(equal(s.equal_range(as_const, hash{}, equal_to{}, i),
                     rng(1, i)));
      }
      for (int i : seq(-100, -3, -2, -1, 7, 8, 9, 100)) {
        assert(s.find(as_const, hash{}, equal_to{}, i) == s.end());
        assert(s.count(as_const, hash{}, equal_to{}, i) == 0);
        assert(empty(s.equal_range(as_const, hash{}, equal_to{}, i)));
      }
    }

    // no duplicate-value inserted
    // some bucket has multiple values
    {
      table_t s;
      assert(s.bucket_count() == 0);
      s.max_load_factor(10.0f);
      for (int i : irng(0, 13))
        s.insert(as_const, hash{}, equal_to{}, i);
      assert(s.bucket_count() == 5);
      for (int i : irng(0, 13)) {
        const auto it = s.find(as_const, hash{}, equal_to{}, i);
        assert(it != s.end() && *it == i);
        assert(s.count(as_const, hash{}, equal_to{}, i) == 1);
        assert(equal(s.equal_range(as_const, hash{}, equal_to{}, i),
                     rng(1, i)));
      }
      for (int i : seq(-100, -3, -2, -1, 13, 14, 15, 100)) {
        assert(s.find(as_const, hash{}, equal_to{}, i) == s.end());
        assert(s.count(as_const, hash{}, equal_to{}, i) == 0);
        assert(empty(s.equal_range(as_const, hash{}, equal_to{}, i)));
      }
    }

    // duplicate-value inserted
    {
      table_t s;
      assert(s.bucket_count() == 0);
      s.max_load_factor(1.0f);
      s.reserve(as_const, hash{}, equal_to{}, 20);
      s.max_load_factor(10.0f);
      for (int c = 2; c != 0; --c)
        for (int i : irng(0, 9))
          s.insert_equal(as_const, hash{}, equal_to{}, i);
      assert(s.bucket_count() == 23);
      assert(eq(s, join_rng(rng(2, irng(0, 9)))));
      for (int i : irng(0, 9)) {
        const auto it = s.find(as_const, hash{}, equal_to{}, i);
        assert(it != s.end() && *it == i);
        assert(prev(it) != s.end() && *prev(it) == i);
        assert(s.count(as_const, hash{}, equal_to{}, i) == 2);
        assert(equal(s.equal_range(as_const, hash{}, equal_to{}, i),
                     rng(2, i)));
      }
      for (int i : seq(-100, -3, -2, -1, 9, 10, 11, 100)) {
        assert(s.find(as_const, hash{}, equal_to{}, i) == s.end());
        assert(s.count(as_const, hash{}, equal_to{}, i) == 0);
        assert(empty(s.equal_range(as_const, hash{}, equal_to{}, i)));
      }

      for (int i : irng(0, 9))
        s.insert_equal(as_const, hash{}, equal_to{}, i);
      assert(s.bucket_count() == 23);
      assert(eq(s, join_rng(rng(3, irng(0, 9)))));
      for (int i : irng(0, 9)) {
        const auto it = s.find(as_const, hash{}, equal_to{}, i);
        assert(it != s.end() && *it == i);
        assert(prev(it) != s.end() && *prev(it) == i);
        assert(prev(it, 2) != s.end() && *prev(it, 2) == i);
        assert(s.count(as_const, hash{}, equal_to{}, i) == 3);
        assert(equal(s.equal_range(as_const, hash{}, equal_to{}, i),
                     rng(3, i)));
      }
      for (int i : seq(-100, -3, -2, -1, 9, 10, 11, 100)) {
        assert(s.find(as_const, hash{}, equal_to{}, i) == s.end());
        assert(s.count(as_const, hash{}, equal_to{}, i) == 0);
        assert(empty(s.equal_range(as_const, hash{}, equal_to{}, i)));
      }
    }
  }
  // allocator_aware container
  {
    stateful_test_allocator<int> a, a2;
    assert(a != a2);
    table_t t(a);
    assert(t.get_allocator() == a);

    t.insert(as_const, hash{}, equal_to{}, 1);
    t.insert(as_const, hash{}, equal_to{}, 2);
    table_t t2(t, a2);
    assert(eq(t, irng(1, 3)));
    assert(eq(t2, irng(1, 3)));
    assert(t2.get_allocator() == a2);

    table_t t3(move(t2), a);
    assert(t2.empty() && eq(t3, irng(1, 3)));
    assert(t2.get_allocator() == a2);
    assert(t3.get_allocator() == a);
  }
  // special member functions
  {
    const auto init = [](auto *p, auto r, auto a) {
      using t = decay_t<decltype(*p)>;
      new(p) t(a);
      for (const auto &x : r)
        (*p).insert(as_const, hash{}, equal_to{}, x);
    };
    const auto good = [=](auto *p, auto r) {
      return is_permutation(*p, r) && (*p).size() == r.size()
        && all_of(r, [p](auto i) {
          const auto it = (*p).find(as_const, hash{}, equal_to{}, i);
          return it != (*p).end() && *it == i;
        });
    };
    using P00 = type_pack<true_type, false_type>;
    using P0 = type_pack_mul<P00, P00>;
    using P = type_pack_mul<type_pack_mul<P0, P0>, P00>;
    for (int n1 : irng(0, 16))
      for (int n2 : irng(0, 16)) {
        type_pack_for_each<P>
          ([n1, n2, init, good](auto a, auto b, auto c, auto d, auto e) {
            constexpr bool y1 = decltype(a)::type::value;
            constexpr bool y2 = decltype(b)::type::value;
            constexpr bool y3 = decltype(c)::type::value;
            constexpr bool y4 = decltype(d)::type::value;
            constexpr bool y5 = decltype(e)::type::value;
            using alloc_t = stateful_test_allocator
              <int, ez_map, y1, y2, y3, y4>;
            using t = hashtable_adaptor<hashtable_traits
                                        <hashtable_node<int>, 0, 1, alloc_t>>;
            alloc_t a1;
            alloc_t a2 = y5 ? a1 : alloc_t{};
            test_allocator_aware_container_ownership<t>
              (bind(init, _1, irng(0, n1), a1),
               bind(good, _1, irng(0, n1)),
               bind(init, _1, irng(0, n2), a2),
               bind(good, _1, irng(0, n2)),
               bind(empty, bind(deref, _1)));
          });
      }
  }
  // insert
  {
    for (float f : seq(1.0f, 3.0f)) {
      table_t t;
      t.max_load_factor(f);
      for (int x : irng(0, 10000)) {
        auto [it, success]
          = t.insert(as_const, hash{}, equal_to{}, x,
                     [&t, x]() {return t.new_node(x);}, empty_function{});
        assert(success && *it == x);
        assert(!t.empty());
        assert(t.size() == to_unsigned(x + 1));
      }
      assert(eq(t, irng(0, 10000)));
      assert(t.load_factor() <= t.max_load_factor());
      assert(t.load_factor() > (0.4f * f));
    }
  }
  // insert_equal
  {
    for (float f : seq(1.0f, 3.0f)) {
      table_t t;
      t.max_load_factor(f);
      for (int x : irng(0, 1000)) {
        auto it = t.insert_equal(as_const, hash{}, equal_to{}, x,
                                 [&t, x]() {return t.new_node(x);});
        assert(it != t.end() && *it == x);
      }
      for (int x : irng(0, 1000)) {
        auto it = t.insert_equal(as_const, hash{}, equal_to{}, x,
                                 [&t, x]() {return t.new_node(x);});
        assert(it != t.end() && *it == x);
      }

      assert(eq(t, join_rng(rng(2, irng(0, 1000)))));
      for (int x : irng(0, 1000))
        assert(equal(t.equal_range(as_const, hash{}, equal_to{}, x),
                     rng(2, x)));
      assert(t.load_factor() <= t.max_load_factor());
      assert(t.load_factor() > (0.4f * f));
    }
  }
  // insert_equal with hint
  {
    for (float f : seq(1.0f, 3.0f)) {
      {
        table_t t;
        t.max_load_factor(f);
        for (int i : irng(0, 30))
          t.insert_equal(as_const, hash{}, equal_to{}, t.end(), i);
        assert(eq(t, irng(0, 30)));
      }
      {
        table_t t;
        t.max_load_factor(f);
        for (int i : irng(0, 30))
          t.insert_equal(as_const, hash{}, equal_to{}, t.begin(), i);
        assert(eq(t, irng(0, 30)));
      }
      {
        table_t t;
        t.max_load_factor(f);
        for (int i : irng(0, 30))
          t.insert_equal(as_const, hash{}, equal_to{},
                         t.find(as_const, hash{}, equal_to{}, i),
                         i);
        assert(eq(t, irng(0, 30)));
      }
      {
        table_t t;
        t.max_load_factor(f);
        for (int i : irng(0, 30))
          t.insert_equal(as_const, hash{}, equal_to{},
                         next(t.find(as_const, hash{}, equal_to{}, i),
                              1, t.end()),
                         i);
        assert(eq(t, irng(0, 30)));
      }
    }
  }
  // erase(cit, cit)
  {
    for (float f : seq(1.0f, 3.0f)) {
      {
        table_t t;
        t.max_load_factor(f);
        for (int x : irng(0, 1000))
          t.insert(as_const, hash{}, equal_to{}, x);
        assert(t.erase(t.begin(), t.end()) == t.end());
        assert(t.empty());
      }
      {
        table_t t;
        t.max_load_factor(f);
        for (int x : irng(0, 1000))
          t.insert(as_const, hash{}, equal_to{}, x);
        const auto mid = nth(t, 500);
        assert(t.erase(t.begin(), mid) == mid);
        assert(t.erase(mid, t.end()) == t.end());
        assert(t.erase(t.end(), t.end()) == t.end());
        assert(t.empty());
      }
    }
  }
  // erase(cit)
  {
    for (float f : seq(1.0f, 3.0f))
      for (decltype(auto) r
           : inner_cartesian_product_rng
             (bind_rng(rrng(irng(1, 8)), [](int x) {return irng(0, x);}))) {
        flat_set<int> v;
        table_t t;
        t.max_load_factor(f);
        for (int x : irng(0, 7)) {
          t.insert(as_const, hash{}, equal_to{}, x);
          v.insert(x);
        }
        assert(is_permutation(t, v));

        for (int x : r) {
          const auto it = nth(t, x);
          assert(it != t.end());
          t.erase(it);
          v.erase(*it);
          assert(is_permutation(v, t));
        }
        assert(t.empty() && v.empty());
      }
  }
  // erase(get_key, hash, eq, key)
  {
    for (float f : seq(1.0f, 3.0f)) {
      for (int i : irng(0, 30))
        for (int j : irng(0, i)) {
          flat_set<int> v;
          table_t t;
          t.max_load_factor(f);
          for (int x : irng(0, i)) {
            v.insert(x);
            t.insert(as_const, hash{}, equal_to{}, x);
          }
          v.erase(j);
          assert(t.erase(as_const, hash{}, equal_to{}, j) == 1u);
          assert(eq(t, v));
        }
      for (int i : irng(0, 20))
        for (int j : irng(0, i)) {
          flat_multiset<int> v;
          table_t t;
          t.max_load_factor(f);
          for (int c = 2; c != 0; --c)
            for (int x : irng(0, i)) {
              v.insert(x);
              t.insert_equal(as_const, hash{}, equal_to{}, x);
            }
          v.erase(j);
          assert(t.erase(as_const, hash{}, equal_to{}, j) == 2u);
          assert(eq(t, v));
        }
      for (int i : irng(0, 20))
        for (int j : irng(0, i)) {
          flat_multiset<int> v;
          table_t t;
          t.max_load_factor(f);
          for (int c = 3; c != 0; --c)
            for (int x : irng(0, i)) {
              v.insert(x);
              t.insert_equal(as_const, hash{}, equal_to{}, x);
            }
          v.erase(j);
          assert(t.erase(as_const, hash{}, equal_to{}, j) == 3u);
          assert(eq(t, v));
        }
    }
  }
  // rehash
  {
    for (float f : seq(1.0f, 3.0f)) {
      table_t t;
      t.max_load_factor(f);
      for (int x : irng(0, 100))
        t.insert(as_const, hash{}, equal_to{}, x);
      assert(eq(t, irng(0, 100)));

      t.rehash(as_const, hash{}, equal_to{}, 0);
      assert(t.load_factor() <= t.max_load_factor());
      assert(eq(t, irng(0, 100)));

      t.rehash(as_const, hash{}, equal_to{}, 1000);
      assert(t.load_factor() <= t.max_load_factor());
      assert(t.bucket_count() >= 1000);
      assert(eq(t, irng(0, 100)));

      t.rehash(as_const, hash{}, equal_to{}, 10000);
      assert(t.load_factor() <= t.max_load_factor());
      assert(t.bucket_count() >= 10000);
      assert(eq(t, irng(0, 100)));

      t.rehash(as_const, hash{}, equal_to{}, 0);
      assert(t.load_factor() <= t.max_load_factor());
      assert(t.bucket_count() < 300);
      assert(eq(t, irng(0, 100)));
    }
  }
  // merge
  // merge_equal
  {
    ez_vector<int> v, v2, v3;
    for (int i : irng(0, 30))
      for (int j : irng(0, 30))
        for (int k : irng(0, 30)) {
          table_t t1, t2(t1.get_allocator());
          const auto init = [&t1, &t2, i, j, k]() {
            for (int x : irng(0, i + j))
              t1.insert(as_const, hash{}, equal_to{}, x,
                        [&t1, x]() {return t1.new_node(x);},
                        empty_function{});
            for (int x : irng(i, i + j + k))
              t2.insert(as_const, hash{}, equal_to{}, x,
                        [&t2, x]() {return t2.new_node(x);},
                        empty_function{});
          };
          init();
          assert(eq(t1, irng(0, i + j)));
          assert(eq(t2, irng(i, i + j + k)));

          v.erase(v.begin(), v.end());
          v2.erase(v2.begin(), v2.end());
          v3.erase(v3.begin(), v3.end());
          copy(irng(0, i + j), to_back(v));
          copy(irng(i, i + j + k), to_back(v2));
          merge(v, v2, to_back(v3));
          t1.clear();
          t2.clear();
          init();
          assert(eq(t1, irng(0, i + j)));
          assert(eq(t2, irng(i, i + j + k)));
          t1.merge_equal(as_const, hash{}, equal_to{}, t2);
          assert(eq(t1, v3));
          assert(t2.empty());
        }
  }
  // equal
  // equal_equal
  {
    {
      table_t t1, t2(t1.get_allocator());
      assert(t1.equal(as_const, hash{}, equal_to{}, hash{}, equal_to{}, t2));
      assert(t1.equal_equal(as_const, hash{}, equal_to{},
                            hash{}, equal_to{}, t2));
    }
    {
      for (int i : irng(0, 30))
        for (int j : irng(0, 30)) {
          {
            table_t t1, t2(t1.get_allocator());
            for (int k : irng(0, 10)) {
              t1.insert(as_const, hash{}, equal_to{}, k);
              t2.insert(as_const, hash{}, equal_to{}, k);
            }
            t1.rehash(as_const, hash{}, equal_to{}, i);
            t2.rehash(as_const, hash{}, equal_to{}, j);
            assert(t1.equal(as_const, hash{}, equal_to{},
                            hash{}, equal_to{}, t2));
            assert(t1.equal_equal(as_const, hash{}, equal_to{},
                                  hash{}, equal_to{}, t2));
          }
          {
            table_t t1, t2(t1.get_allocator());
            for (int k : irng(0, 10)) {
              t1.insert_equal(as_const, hash{}, equal_to{}, k);
              t1.insert_equal(as_const, hash{}, equal_to{}, k);
              t1.insert_equal(as_const, hash{}, equal_to{}, k);
              t2.insert_equal(as_const, hash{}, equal_to{}, k);
              t2.insert_equal(as_const, hash{}, equal_to{}, k);
              t2.insert_equal(as_const, hash{}, equal_to{}, k);
            }
            t1.rehash(as_const, hash{}, equal_to{}, i * 3);
            t2.rehash(as_const, hash{}, equal_to{}, j * 3);
            assert(t1.equal_equal(as_const, hash{}, equal_to{},
                                  hash{}, equal_to{}, t2));
          }
        }
    }
  }
}

template <class S, class F, class F2>
void test_unordered_set_common_operations_impl(F mk, F2 key) {
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
    assert(is_permutation(s, seq(mk(1), mk(2), mk(3))));
    assert(is_permutation(as_const(s), s));
    assert(s.size() == 3);
    assert(!s.empty());
    assert(s == s);
  }

  // constructor and assignment
  {
    using alloc_t = typename S::allocator_type;
    using hash_t = typename S::hasher;
    using eq_t = typename S::key_equal;
    alloc_t al1, al2;

    // special member functions
    {
      static_assert(regular<S>);
      static_assert(swappable<S>);
      static_assert(equality_comparable<S>);
      static_assert(!three_way_comparable<S>);
      static_assert(is_nothrow_movable_v<S>);
      static_assert(is_nothrow_swappable_v<S>);

      S s(al1);
      assert(s.max_load_factor() == 1.0f);
      assert(s.empty() && s.get_allocator() == al1);
      s.append(mk(2), mk(2));
      assert(equal(s, seq(mk(2))));
      S s2(s, al2);
      assert(s2.max_load_factor() == 1.0f);
      assert(s2 == s && s2.get_allocator() == al2);
      S s3(move(s2), al1);
      assert(s3.max_load_factor() == 1.0f);
      assert(equal(s3, seq(mk(2))) && s3.get_allocator() == al1);
      assert(s2.empty());
    }
    // this_t(hash, eq, alloc = {})
    // this_t(n, hash = {}, eq = {}, alloc = {})
    {
      {
        S s(hash_t{}, eq_t{}, al1);
        assert(s.get_allocator() == al1);
        assert(s.empty());
      }
      {
        S s(hash_t{}, eq_t{});
        assert(s.empty());
      }
      {
        S s(17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(s.empty());
      }
      {
        S s(17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
      {
        S s(17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
      {
        S s(17);
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
    }
    // this_t(from, to, n, hash, eq, alloc)
    // this_t(from, to, n, hash, eq)
    // this_t(from, to, n, hash)
    // this_t(from, to, n)
    // this_t(from, to, n, hash, alloc)
    // this_t(from, to, n, alloc)
    // assign(from, to)
    {
      const auto a = seq(mk(1), mk(2), mk(3));
      {
        S s(begin(a), end(a), 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a));
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign(begin(a), end(a));
        assert(is_permutation(s, a));
      }
    }
    // this_t(il, n, hash, eq, alloc)
    // this_t(il, n, hash, eq)
    // this_t(il, n, hash)
    // this_t(il, n)
    // this_t(il)
    // this_t(il, n, alloc)
    // this_t(il, n, hash, alloc)
    // assign(il)
    {
      const auto a = seq(mk(1), mk(2), mk(3));
      {
        S s({mk(1), mk(2), mk(3)}, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)}, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)}, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)}, 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)}, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(3)}, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign({mk(1), mk(2), mk(3)});
        assert(is_permutation(s, a));
      }
    }
    // this_t(from_range, r, n, hash, eq, alloc)
    // this_t(from_range, r, n, hash, eq)
    // this_t(from_range, r, n, hash)
    // this_t(from_range, r, n)
    // this_t(from_range, r, n, hash, alloc)
    // this_t(from_range, r, n, alloc)
    // assign_range(from_range, r)
    {
      const auto a = seq(mk(1), mk(2), mk(3));
      {
        S s(from_range, a, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a, 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign_range(a);
        assert(is_permutation(s, a));
      }
    }
    // this_t(r, n, hash, eq, alloc)
    // this_t(r, n, hash, eq)
    // this_t(r, n, hash)
    // this_t(r, n)
    // this_t(r, n, hash, alloc)
    // this_t(r, n, alloc)
    // assign(r)
    {
      const auto a = seq(mk(1), mk(2), mk(3));
      {
        S s(a, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign(a);
        assert(is_permutation(s, a));
      }
    }
  }

  // allocator_type
  // get_allocator()
  {
    typename S::allocator_type a;
    S s(a);
    assert(s.get_allocator() == a);
  }

  // hash_function()
  // key_eq()
  {
    S s;
    assert(s.hash_function()(0) == hash<int>{}(0));
    assert(s.key_eq()(1, 1));
    assert(!s.key_eq()(1, 2));
  }

  // emplace(s...)
  // emplace_hint(hint, s...)
  {
    S s;
    const auto [it, success] = s.emplace(mk(1));
    assert(success && *it == mk(1));
    assert(is_permutation(s, bind_rng(irng(1, 2), mk)));
    const auto it2 = s.emplace_hint(s.end(), mk(0));
    assert(*it2 == mk(0));
    assert(is_permutation(s, bind_rng(irng(0, 2), mk)));
  }

  // insert(x)
  // insert(hint, x)
  // insert(from, to)
  // insert(il)
  {
    S s;
    const auto [it, success] = s.insert(mk(1));
    assert(success && *it == mk(1));
    assert(is_permutation(s, bind_rng(irng(1, 2), mk)));
    const auto it2 = s.insert(s.end(), mk(0));
    assert(*it2 == mk(0));
    assert(is_permutation(s, bind_rng(irng(0, 2), mk)));

    s.clear();
    assert(s.empty());
    s.insert(bind_rng(irng(1, 4), mk));
    assert(is_permutation(s, bind_rng(irng(1, 4), mk)));

    s.clear();
    assert(s.empty());
    const auto tmp_r = bind_rng(irng(1, 4), mk);
    s.insert(begin(tmp_r), end(tmp_r));
    assert(is_permutation(s, bind_rng(irng(1, 4), mk)));

    s.clear();
    assert(s.empty());
    s.insert({mk(1), mk(2), mk(3)});
    assert(is_permutation(s, bind_rng(irng(1, 4), mk)));
  }

  // insert/extract node_handle
  {
    S s;
    s.insert(mk(1));
    typename S::node_type x = s.extract(s.begin());
    s.insert(mk(1));
    typename S::node_type y = s.extract(s.begin());

    auto z = s.insert(move(x));
    assert(z.position == s.begin());
    assert(z.inserted);
    assert(z.node.empty());
    assert(x.empty());

    z = s.insert(move(y));
    assert(z.position == s.begin());
    assert(!z.inserted);
    assert(!z.node.empty());
    assert(y.empty());
  }

  // clear
  // erase
  // remove
  // remove_if
  {
    // clear()
    {
      S s;
      s.insert(mk(1));
      assert(!s.empty());
      s.clear();
      assert(s.empty());
    }
    // erase(cit)
    {
      S s;
      s.insert(mk(1));
      assert(s.erase(s.begin()) == s.end());
      assert(s.empty());
    }
    // erase(cit, cit)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      s.insert(mk(3));
      const auto it = prev(s.end());
      auto x = *it;
      assert(s.erase(s.begin(), it) == it);
      assert(*it == x);
      assert(s.begin() == it && it != s.end() && next(it) == s.end());
    }
    // erase(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.erase(key(mk(1))) == 1);
      assert(s.empty());
    }
    // remove(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.remove(key(mk(1))) == 1);
      assert(s.empty());
    }
    // remove_if(eq)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      assert(s.remove_if([=](const auto &x) {return key(x) < 3;}) == 2u);
      assert(s.empty());
    }
  }

  // merge
  {
    S s, s2(s.get_allocator());
    s.insert(mk(1));
    s.insert(mk(3));
    s.insert(mk(5));
    s2.insert(mk(2));
    s2.insert(mk(3));
    s2.insert(mk(4));
    s2.insert(mk(6));
    s.merge(s2);
    assert(is_permutation(s2, single_rng(mk(3))));
    assert(is_permutation(s, bind_rng(irng(1, 7), mk)));
  }

  // find
  // contains
  // count
  // equal_range
  {
    // find(key)
    // contains(key)
    {
      S s;
      s.insert(mk(1));
      assert(equal(s, single_rng(mk(1))));

      assert(s.find(key(mk(1))) == s.begin());
      assert(as_const(s).find(key(mk(1))) == s.begin());
      assert(s.contains(key(mk(1))));
      assert(equal(s, single_rng(mk(1))));

      assert(s.find(key(mk(2))) == s.end());
      assert(as_const(s).find(key(mk(2))) == s.end());
      assert(!s.contains(key(mk(2))));
      assert(equal(s, single_rng(mk(1))));
    }

    // count(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.count(key(mk(1))) == 1u);
      assert(s.count(key(mk(2))) == 0u);
    }

    // equal_range(key)
    {
      S s;
      s.insert_range(bind_rng(irng(1, 4), mk));
      assert(equal(s.equal_range(2), single_rng(mk(2))));
    }
  }

  // bucket_count
  // active_bucket_count
  // max_bucket_count
  // bucket_size
  // bucket
  {
    S s;
    assert(s.bucket_count() == 0u);
    assert(s.active_bucket_count() == 0u);
    assert(s.max_bucket_count() > 0u);
    s.insert(mk(0));
    assert(s.bucket_size(0) == 1);
    for (int i : irng(1, 5))
      assert(s.bucket_size(i) == 0);
    assert(s.bucket(0) == 0);
    assert(s.bucket(1) == 1);
  }

  // local_iterator
  // const_local_iterator
  // begin(n)
  // end(n)
  // cbegin(n)
  // cend(n)
  // bucket_range(n)
  {
    S s;
    s.max_load_factor(3.0f);
    for (int x : irng(0, 10))
      s.insert(mk(x));
    {
      ez_vector<decltype(mk(1))> v;
      for (auto &i : iters(0u, s.bucket_count()))
        for (auto &x : s.bucket_range(i))
          v.insert(v.end(), x);
      assert(is_permutation(v, s));
    }
    {
      ez_vector<decltype(mk(1))> v;
      for (auto &i : iters(0u, s.bucket_count()))
        for (auto &x : as_const(s).bucket_range(i))
          v.insert(v.end(), x);
      assert(is_permutation(v, s));
    }
  }

  // load_factor()
  // max_load_factor()
  // max_load_factor(f)
  // rehash(n)
  // reserve(n)
  {
    S s;
    assert(s.max_load_factor() == 1.0f);
    s.max_load_factor(2.0f);
    assert(s.max_load_factor() == 2.0f);
    s.max_load_factor(1.0f);
    assert(s.max_load_factor() == 1.0f);
    s.rehash(2);
    assert(s.empty());
    s.insert(mk(1));
    s.rehash(20);
    assert(equal(s, single_rng(mk(1))));
    s.reserve(100);
    assert(equal(s, single_rng(mk(1))));
    assert(s.bucket_count() >= 100);
    const auto c = s.bucket_count();
    s.reserve(0);
    assert(s.bucket_count() == 5u);
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator x = s.new_node(mk(1));
    assert(*x == mk(1));
    s.delete_node(x);
  }

  // unlink
  // unlink_key
  // link
  {
    S s;
    s.insert(mk(1));
    const auto it0 = s.begin();
    assert(s.unlink(s.begin()) == s.end());
    assert(s.empty());
    s.delete_node(it0);

    s.insert(mk(1));
    s.insert(mk(2));
    const auto it = s.begin();
    const auto it2 = next(s.begin());
    assert(s.unlink(s.begin(), s.end()) == s.end());
    const auto [p, success] = s.link(it);
    assert(p == s.begin() && success);
    assert(*s.link(s.begin(), it2) == (*p == mk(1) ? mk(2) : mk(1)));

    s.clear();
    const auto it3 = s.new_node(mk(1));
    s.link(it3);
    assert(s.unlink_key(1) == 1u);
    assert(s.empty());
    s.delete_node(it3);
  }

  // try_link
  // try_link_hint
  {
    S s;
    const auto [it, success]
      = s.try_link(1, [&s, mk]() {return s.new_node(mk(1));}, empty_function{});
    assert(it == s.begin() && *it == mk(1) && success);
    auto it2 = s.try_link_hint(s.end(), 1,
                               [&s, mk]() {return s.new_node(mk(1));},
                               empty_function{});
    assert(it2 == s.begin() && equal(s, single_rng(mk(1))));
  }

  // erase_or_unlink
  {
    S s;
    s.insert(mk(1));
    s.erase_or_unlink(s.begin());
    assert(s.empty());
  }

  // unique
  {
    S s = {mk(1), mk(2)};
    s.unique();
    assert(is_permutation(s, seq(mk(1), mk(2))));
    s.unique([key](auto x, auto y) {
      const auto z = key(x) - key(y);
      return z == 1 || z == -1;
    });
    assert(equal(s, seq(mk(1))) || equal(s, seq(mk(2))));
  }

  // append
  {
    S s;
    s.append(mk(1), mk(2), seq(mk(3), mk(4)));
    assert(is_permutation(s, bind_rng(irng(1, 5), mk)));
  }
}
void test_unordered_set_common_operations() {
  test_unordered_set_common_operations_impl
    <unordered_set<int, hash<int>, equal_to<int>, stateful_test_allocator<int>>>
    (copy, identity{});
  test_unordered_set_common_operations_impl
    <unordered_map<int, int, hash<int>, equal_to<int>,
                   stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S, class F, class F2>
void test_unordered_multiset_common_operations_impl(F mk, F2 key) {
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
    s.insert(mk(3));
    assert(is_permutation(s, seq(mk(1), mk(3), mk(3))));
    assert(is_permutation(as_const(s), s));
    assert(s.size() == 3);
    assert(!s.empty());
    assert(s == s);
  }

  // constructor and assignment
  {
    using alloc_t = typename S::allocator_type;
    using hash_t = typename S::hasher;
    using eq_t = typename S::key_equal;
    alloc_t al1, al2;

    // special member functions
    {
      static_assert(regular<S>);
      static_assert(swappable<S>);
      static_assert(equality_comparable<S>);
      static_assert(!three_way_comparable<S>);
      static_assert(is_nothrow_movable_v<S>);
      static_assert(is_nothrow_swappable_v<S>);

      S s(al1);
      assert(s.max_load_factor() == 1.0f);
      assert(s.empty() && s.get_allocator() == al1);
      s.append(mk(1), mk(1));
      assert(equal(s, seq(mk(1), mk(1))));
      S s2(s, al2);
      assert(s2.max_load_factor() == 1.0f);
      assert(s2 == s && s2.get_allocator() == al2);
      S s3(move(s2), al1);
      assert(s3.max_load_factor() == 1.0f);
      assert(equal(s3, seq(mk(1), mk(1))) && s3.get_allocator() == al1);
      assert(s2.empty());
    }
    // this_t(hash, eq, alloc = {})
    // this_t(n, hash = {}, eq = {}, alloc = {})
    {
      {
        S s(hash_t{}, eq_t{}, al1);
        assert(s.get_allocator() == al1);
        assert(s.empty());
      }
      {
        S s(hash_t{}, eq_t{});
        assert(s.empty());
      }
      {
        S s(17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(s.empty());
      }
      {
        S s(17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
      {
        S s(17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
      {
        S s(17);
        assert(s.bucket_count() == 23u);
        assert(s.empty());
      }
    }
    // this_t(from, to, n, hash, eq, alloc)
    // this_t(from, to, n, hash, eq)
    // this_t(from, to, n, hash)
    // this_t(from, to, n)
    // this_t(from, to, n, hash, alloc)
    // this_t(from, to, n, alloc)
    // assign(from, to)
    {
      const auto a = seq(mk(1), mk(2), mk(2));
      {
        S s(begin(a), end(a), 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a));
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(begin(a), end(a), 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign(begin(a), end(a));
        assert(is_permutation(s, a));
      }
    }
    // this_t(il, n, hash, eq, alloc)
    // this_t(il, n, hash, eq)
    // this_t(il, n, hash)
    // this_t(il, n)
    // this_t(il)
    // this_t(il, n, alloc)
    // this_t(il, n, hash, alloc)
    // assign(il)
    {
      const auto a = seq(mk(1), mk(2), mk(1));
      {
        S s({mk(1), mk(2), mk(1)}, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)}, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)}, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)}, 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)}, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s({mk(1), mk(2), mk(1)}, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign({mk(1), mk(2), mk(1)});
        assert(is_permutation(s, a));
      }
    }
    // this_t(from_range, r, n, hash, eq, alloc)
    // this_t(from_range, r, n, hash, eq)
    // this_t(from_range, r, n, hash)
    // this_t(from_range, r, n)
    // this_t(from_range, r, n, hash, alloc)
    // this_t(from_range, r, n, alloc)
    // assign_range(from_range, r)
    {
      const auto a = seq(mk(1), mk(1), mk(1));
      {
        S s(from_range, a, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(equal(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(equal(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(equal(s, a));
      }
      {
        S s(from_range, a, 17);
        assert(s.bucket_count() == 23u);
        assert(equal(s, a));
      }
      {
        S s(from_range, a);
        assert(s.bucket_count() == 23u);
        assert(equal(s, a));
      }
      {
        S s(from_range, a, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(equal(s, a));
      }
      {
        S s(from_range, a, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(equal(s, a));
        s.clear();
        assert(s.empty());
        s.assign_range(a);
        assert(equal(s, a));
      }
    }
    // this_t(r, n, hash, eq, alloc)
    // this_t(r, n, hash, eq)
    // this_t(r, n, hash)
    // this_t(r, n)
    // this_t(r, n, hash, alloc)
    // this_t(r, n, alloc)
    // assign(r)
    {
      const auto a = seq(mk(1), mk(2), mk(3), mk(2));
      {
        S s(a, 17, hash_t{}, eq_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{}, eq_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{});
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a);
        assert(s.bucket_count() == 23u);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
      }
      {
        S s(a, 17, hash_t{}, al1);
        assert(s.bucket_count() == 23u);
        assert(s.get_allocator() == al1);
        assert(is_permutation(s, a));
        s.clear();
        assert(s.empty());
        s.assign(a);
        assert(is_permutation(s, a));
      }
    }
  }

  // allocator_type
  // get_allocator()
  {
    typename S::allocator_type a;
    S s(a);
    assert(s.get_allocator() == a);
  }

  // hash_function()
  // key_eq()
  {
    S s;
    assert(s.hash_function()(0) == hash<int>{}(0));
    assert(s.key_eq()(1, 1));
    assert(!s.key_eq()(1, 2));
  }

  // emplace(s...)
  // emplace_hint(hint, s...)
  {
    S s;
    const auto it = s.emplace(mk(1));
    assert(*it == mk(1));
    assert(equal(s, single_rng(mk(1))));
    const auto it2 = s.emplace_hint(s.end(), mk(1));
    assert(*it2 == mk(1));
    assert(equal(s, rng(2, mk(1))));
  }

  // insert(x)
  // insert(hint, x)
  // insert(from, to)
  // insert(il)
  {
    S s;
    const auto it = s.insert(mk(1));
    assert(*it == mk(1));
    assert(equal(s, single_rng(mk(1))));
    const auto it2 = s.insert(s.end(), mk(1));
    assert(*it2 == mk(1));
    assert(equal(s, rng(2, mk(1))));

    s.clear();
    assert(s.empty());
    s.insert(bind_rng(seq(1, 2, 1, 2), mk));
    assert(is_permutation(s, bind_rng(seq(1, 2, 1, 2), mk)));

    s.clear();
    assert(s.empty());
    const auto tmp_r = bind_rng(seq(1, 2, 1, 2), mk);
    s.insert(begin(tmp_r), end(tmp_r));
    assert(is_permutation(s, bind_rng(seq(1, 2, 1, 2), mk)));

    s.clear();
    assert(s.empty());
    s.insert({mk(1), mk(2), mk(1)});
    assert(is_permutation(s, bind_rng(seq(1, 1, 2), mk)));
  }

  // insert/extract node_handle
  {
    S s;
    s.insert(mk(1));
    typename S::node_type x = s.extract(s.begin());
    s.insert(mk(1));
    typename S::node_type y = s.extract(s.begin());

    typename S::iterator it = s.insert(move(x));
    assert(*it == mk(1));
    assert(equal(s, single_rng(mk(1))));

    it = s.insert(move(y));
    assert(*it == mk(1));
    assert(equal(s, rng(2, mk(1))));
  }

  // clear
  // erase
  // remove
  // remove_if
  {
    // clear()
    {
      S s;
      s.insert(mk(1));
      assert(!s.empty());
      s.clear();
      assert(s.empty());
    }
    // erase(cit)
    {
      S s;
      s.insert(mk(1));
      assert(s.erase(s.begin()) == s.end());
      assert(s.empty());
    }
    // erase(cit, cit)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      s.insert(mk(3));
      const auto it = prev(s.end());
      auto x = *it;
      assert(s.erase(s.begin(), it) == it);
      assert(*it == x);
      assert(s.begin() == it && it != s.end() && next(it) == s.end());
    }
    // erase(key)
    {
      S s;
      s.insert_range(rng(3, mk(1)));
      assert(s.erase(key(mk(1))) == 3);
      assert(s.empty());
    }
    // remove(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.remove(key(mk(1))) == 1);
      assert(s.empty());
    }
    // remove_if(eq)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      assert(s.remove_if([=](const auto &x) {return key(x) < 3;}) == 2u);
      assert(s.empty());
    }
  }

  // merge
  {
    S s, s2(s.get_allocator());
    s.insert(mk(1));
    s.insert(mk(3));
    s.insert(mk(5));
    s2.insert(mk(2));
    s2.insert(mk(3));
    s2.insert(mk(4));
    s2.insert(mk(6));
    s.merge(s2);
    assert(s2.empty());
    assert(is_permutation(s, bind_rng(seq(1, 2, 3, 3, 4, 5, 6), mk)));
  }

  // find
  // contains
  // count
  // equal_range
  {
    // find(key)
    // contains(key)
    {
      S s;
      s.insert(mk(1));
      assert(equal(s, single_rng(mk(1))));

      assert(s.find(key(mk(1))) == s.begin());
      assert(as_const(s).find(key(mk(1))) == s.begin());
      assert(s.contains(key(mk(1))));
      assert(equal(s, single_rng(mk(1))));

      assert(s.find(key(mk(2))) == s.end());
      assert(as_const(s).find(key(mk(2))) == s.end());
      assert(!s.contains(key(mk(2))));
      assert(equal(s, single_rng(mk(1))));
    }

    // count(key)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(1));
      assert(s.count(key(mk(1))) == 2u);
      assert(s.count(key(mk(2))) == 0u);
    }

    // equal_range(key)
    {
      S s;
      s.insert_range(bind_rng(rng(3, 1), mk));
      assert(equal(s.equal_range(1), bind_rng(rng(3, 1), mk)));
    }
  }

  // bucket_count
  // active_bucket_count
  // max_bucket_count
  // bucket_size
  // bucket
  {
    S s;
    assert(s.bucket_count() == 0u);
    assert(s.active_bucket_count() == 0u);
    assert(s.max_bucket_count() > 0u);
    s.insert(mk(0));
    s.insert(mk(0));
    assert(s.bucket_size(0) == 2);
    for (int i : irng(1, 5))
      assert(s.bucket_size(i) == 0);
    assert(s.bucket(0) == 0);
    assert(s.bucket(1) == 1);
  }

  // local_iterator
  // const_local_iterator
  // begin(n)
  // end(n)
  // cbegin(n)
  // cend(n)
  // bucket_range(n)
  {
    S s;
    s.max_load_factor(6.0f);
    for (int x : irng(0, 10)) {
      s.insert(mk(x));
      s.insert(mk(x));
    }
    {
      ez_vector<decltype(mk(1))> v;
      for (auto &i : iters(0u, s.bucket_count()))
        for (auto &x : s.bucket_range(i))
          v.insert(v.end(), x);
      assert(is_permutation(v, s));
    }
    {
      ez_vector<decltype(mk(1))> v;
      for (auto &i : iters(0u, s.bucket_count()))
        for (auto &x : as_const(s).bucket_range(i))
          v.insert(v.end(), x);
      assert(is_permutation(v, s));
    }
  }

  // load_factor()
  // max_load_factor()
  // max_load_factor(f)
  // rehash(n)
  // reserve(n)
  {
    S s;
    assert(s.max_load_factor() == 1.0f);
    s.max_load_factor(2.0f);
    assert(s.max_load_factor() == 2.0f);
    s.max_load_factor(1.0f);
    assert(s.max_load_factor() == 1.0f);
    s.rehash(2);
    assert(s.empty());
    s.insert(mk(1));
    s.rehash(20);
    assert(equal(s, single_rng(mk(1))));
    s.reserve(100);
    assert(equal(s, single_rng(mk(1))));
    assert(s.bucket_count() >= 100);
    const auto c = s.bucket_count();
    s.reserve(0);
    assert(s.bucket_count() == 5u);
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator x = s.new_node(mk(1));
    assert(*x == mk(1));
    s.delete_node(x);
  }

  // unlink
  // unlink_key
  // link
  {
    S s;
    s.insert(mk(1));
    const auto it0 = s.begin();
    assert(s.unlink(s.begin()) == s.end());
    assert(s.empty());
    s.delete_node(it0);

    s.insert(mk(1));
    s.insert(mk(1));
    const auto it = s.begin();
    const auto it2 = next(s.begin());
    assert(s.unlink(s.begin(), s.end()) == s.end());
    const auto p = s.link(it);
    assert(p == s.begin());
    assert(*s.link(s.begin(), it2) == mk(1));

    s.clear();
    const auto it3 = s.new_node(mk(1));
    const auto it4 = s.new_node(mk(1));
    s.link(it3);
    s.link(it4);
    assert(s.unlink_key(1) == 2u);
    assert(s.empty());
    s.delete_node(it3);
    s.delete_node(it4);
  }

  // try_link
  // try_link_hint
  {
    S s;
    const auto it = s.try_link(1, [&s, mk]() {return s.new_node(mk(1));});
    assert(it == s.begin() && *it == mk(1));
    auto it2 = s.try_link_hint(s.end(), 1,
                               [&s, mk]() {return s.new_node(mk(1));});
    assert(it2 == prev(s.end()) && equal(s, rng(2, mk(1))));
  }

  // erase_or_unlink
  {
    S s;
    s.insert(mk(1));
    s.erase_or_unlink(s.begin());
    assert(s.empty());
  }

  // unique
  {
    S s = {mk(1), mk(2), mk(1)};
    s.unique();
    assert(is_permutation(s, seq(mk(1), mk(2))));
    s.unique([key](auto x, auto y) {
      const auto z = key(x) - key(y);
      return z == 1 || z == -1;
    });
    assert(equal(s, seq(mk(1))) || equal(s, seq(mk(2))));
  }

  // append
  {
    S s;
    s.append(mk(1), mk(2), seq(mk(2), mk(4)));
    assert(is_permutation(s, bind_rng(seq(1, 2, 2, 4), mk)));
  }
}
void test_unordered_multiset_common_operations() {
  test_unordered_multiset_common_operations_impl
    <unordered_multiset
     <int, hash<int>, equal_to<int>, stateful_test_allocator<int>>>
    (copy, identity{});
  test_unordered_multiset_common_operations_impl
    <unordered_multimap<int, int, hash<int>, equal_to<int>,
                        stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S>
void test_unordered_map_specialized_operations_impl() {
  const auto mk = bind(make_pair, _1, _1);
  // try_emplace
  {
    S s;
    auto [it, y] = s.try_emplace(1, 2);
    assert(*it == pair(1, 2) && y);
    tie(it, y) = s.try_emplace(1, 2);
    assert(*it == pair(1, 2) && !y);
  }
  // try_emplace_hint
  {
    S s;
    for (int i : irng(0, 1000)) {
      const typename S::iterator it = s.try_emplace(s.end(), i, i);
      assert(*it == pair(i, i));
    }
    assert(is_permutation(s, bind_rng(irng(0, 1000), bind(make_pair, _1, _1))));
    for (int i : irng(0, 1000)) {
      const typename S::iterator it = s.try_emplace(s.end(), i, i);
      assert(*it == pair(i, i));
    }
    assert(is_permutation(s, bind_rng(irng(0, 1000), bind(make_pair, _1, _1))));
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
    assert(is_permutation
           (s, bind_rng(irng(0, 10),
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
      auto it = s.insert_or_assign(s.end(), i, i + 1);
      assert(*it == pair(i, i + 1));
    }
    assert(is_permutation
           (s, bind_rng(irng(0, 10),
                        bind(make_pair, _1, bind(plus{}, _1, 1)))));
  }
  // []
  {
    S s;
    for (int i : irng(0, 10))
      s[i] = i;
    for (int i : irng(0, 10))
      s[i] = i + 1;
    assert(is_permutation
           (s, bind_rng(irng(0, 10),
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
void test_unordered_map_specialized_operations() {
  test_unordered_map_specialized_operations_impl
    <unordered_map<int, int, hash<int>, equal_to<int>,
                   test_allocator<pair<int, int>>>>();
  test_unordered_map_specialized_operations_impl
    <unordered_map<int, int, hash<>, equal_to<>,
                   test_allocator<pair<int, int>>>>();
}

void test_unordered_map() {
  printf("unordered_map: ");

  inner::test::test_hashtable_adaptor();
  inner::test::test_unordered_set_common_operations();
  inner::test::test_unordered_multiset_common_operations();
  inner::test::test_unordered_map_specialized_operations();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_unordered_map();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
