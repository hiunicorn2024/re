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

namespace re::inner::fns {

void test_rbtree_adaptor() {
  using tree_t = rbtree_adaptor<rbtree_traits
                                <rbtree_node<int>, 0, 1, 1,
                                 stateful_test_allocator<int>>>;

  // new_node
  // delete_node
  {
    tree_t t;
    tree_t::iterator it = t.new_node(1);
    assert(*it == 1);
    t.delete_node(it);
  }

  // range operations
  {
    tree_t t;
    static_assert(bidirectional_iterator<tree_t::iterator>);
    static_assert(bidirectional_iterator<tree_t::const_iterator>);
    static_assert(is_just_bitr<tree_t::iterator>);
    static_assert(is_just_bitr<tree_t::const_iterator>);
    assert(t.empty());
    assert(as_const(t).empty());
    assert(t.size() == 0);
    assert(as_const(t).size() == 0);
    test_bitr(t.begin(), t.end(), empty_rng<int>());
    test_bitr(t.cbegin(), t.end(), empty_rng<int>());
    test_bitr(t.begin(), t.cend(), empty_rng<int>());
    test_bitr(t.cbegin(), t.cend(), empty_rng<int>());
    const auto r = irng(0, 10);
    for (int x : r)
      t.insert(t.end(), x);
    test_bitr(t.begin(), t.end(), r);
    test_bitr(t.cbegin(), t.end(), r);
    test_bitr(t.begin(), t.cend(), r);
    test_bitr(t.cbegin(), t.cend(), r);
    assert(t.size() == 10);
    assert(as_const(t).size() == 10);
    assert(!t.empty());
    assert(!as_const(t).empty());
  }

  // allocator_aware container
  {
    stateful_test_allocator<int> a, a2;
    assert(a != a2);
    tree_t t(a);
    assert(t.get_allocator() == a);

    t.insert(t.end(), 1);
    t.insert(t.end(), 2);
    tree_t t2(t, a2);
    assert(equal(t, irng(1, 3)));
    assert(equal(t2, irng(1, 3)));
    assert(t2.get_allocator() == a2);

    tree_t t3(move(t2), a);
    assert(t2.empty() && equal(t3, irng(1, 3)));
    assert(t2.get_allocator() == a2);
    assert(t3.get_allocator() == a);
  }
  // special member functions
  {
    const auto init = [](auto *p, auto r, auto a) {
      using t = decay_t<decltype(*p)>;
      new(p) t(a);
      for (const auto &x : r)
        (*p).insert((*p).end(), x);
    };
    const auto good = [](auto *p, auto r) {
      return equal(*p, r) && (*p).empty() == empty(r) && (*p).size() == size(r)
        && inner::good(*p);
    };
    using P00 = type_pack<true_type, false_type>;
    using P0 = type_pack_mul<P00, P00>;
    using P = type_pack_mul<type_pack_mul<P0, P0>, P00>;
    for (int n1 : irng(0, 50))
      for (int n2 : irng(0, 50)) {
        type_pack_for_each<P>
          ([n1, n2, init, good](auto a, auto b, auto c, auto d, auto e) {
            constexpr bool y1 = decltype(a)::type::value;
            constexpr bool y2 = decltype(b)::type::value;
            constexpr bool y3 = decltype(c)::type::value;
            constexpr bool y4 = decltype(d)::type::value;
            constexpr bool y5 = decltype(e)::type::value;
            using alloc_t = stateful_test_allocator
              <int, ez_map, y1, y2, y3, y4>;
            using t = rbtree_adaptor<rbtree_traits
                                     <rbtree_node<int>, 0, 1, 1,
                                      alloc_t>>;
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

  // lower_bound
  // upper_bound
  {
    for (int i : irng(0, 40)) {
      tree_t t;
      for (int j : irng(0, i))
        t.insert(t.end(), j);
      assert(equal(t, irng(0, i)));
      for (int k : irng(0, i)) {
        const auto it = t.lower_bound(as_const, less<int>{}, k);
        assert(it != t.end() && *it == k);
        const auto it2 = t.upper_bound(as_const, less<int>{}, k);
        assert(it2 != t.begin() && prev(it2) == it);
      }
    }
  }

  // insert(cit, x)
  {
    ez_vector<int> v;
    for (int i : irng(0, 40))
      for (int j : irng(0, i + 1)) {
        v.erase(v.begin(), v.end());
        copy(irng(0, i), to_back(v));
        v.insert(nth(v, j), -1);

        tree_t t;
        copy(irng(0, i), to_back(t));
        const auto it = t.insert(nth(t, j), -1);
        assert(*it == -1);
        assert(equal(v, t));
        assert(t.leftmost() != t.end() && *t.leftmost() == front(v));
        assert(t.rightmost() != t.end() && *t.rightmost() == back(v));
        assert(t.root() != nullptr);
        assert(inner::good(t));
      }
  }
  // erase(cit)
  {
    ez_vector<int> v;
    for (int i : irng(0, 40))
      for (int j : irng(0, i)) {
        v.erase(v.begin(), v.end());
        copy(irng(0, i), to_back(v));
        v.erase(nth(v, j));

        tree_t t;
        copy(irng(0, i), to_back(t));
        const auto it = t.erase(nth(t, j));
        assert(it == t.end() || *it == j + 1);
        assert(equal(v, t));
        assert(t.size() == v.size());
        assert(t.empty() == v.empty());
        if (t.empty()) {
          assert(t.leftmost() == t.end());
          assert(t.rightmost() == t.end());
          assert(t.root() == nullptr);
        }
        else {
          assert(*t.leftmost() == front(v));
          assert(*t.rightmost() == back(v));
          assert(t.root() != nullptr);
        }
        assert(inner::good(t));
      }
  }

  // insert(get_key, key_less, key, get_new_node, do_when_equal)
  {
    // insert 10000 values by ascending and descending order
    {
      tree_t t;
      for (int x : irng(0, 10000)) {
        auto [it, success]
          = t.insert(as_const, less<int>{}, x,
                     [&t, x]() {return t.new_node(x);}, empty_function{});
        assert(success && *it == x);
        assert(equal(t, irng(0, x + 1)));
        assert(!t.empty());
        assert(t.leftmost() != t.end() && *t.leftmost() == 0);
        assert(t.rightmost() != t.end() && *t.rightmost() == x);
        assert(t.size() == to_unsigned(x + 1));
      }
      assert(inner::good(t));

      tree_t t2;
      for (int x : rrng(irng(0, 10000))) {
        const auto [it, success]
          = t2.insert(as_const, less<int>{}, x,
                      [&t2, x]() {return t2.new_node(x);}, empty_function{});
        assert(success && *it == x);
        assert(equal(t2, irng(x, 10000)));
        assert(!t2.empty());
        assert(t2.size() == to_unsigned(10000 - x));
        assert(t2.leftmost() != t.end() && *t2.leftmost() == x);
        assert(t2.rightmost() != t.end() && *t2.rightmost() == 9999);
      }
      assert(inner::good(t2));
    }
    // insert 1-7 values for all order
    {
      for (int i : irng(1, 8)) {
        ez_vector<int> v;
        copy(irng(0, i), to_back(v));
        do {
          tree_t t;
          for (int x : v)
            t.insert(as_const, less<int>{}, x,
                     [&t, x]() {return t.new_node(x);}, empty_function{});
          assert(equal(t, irng(0, i)));
          assert(t.size() == to_unsigned(i));
          assert(!t.empty());
          assert(inner::good(t));
          assert(t.leftmost() != t.end() && *t.leftmost() == 0);
          assert(t.rightmost() != t.end() && *t.rightmost() == i - 1);
        } while (next_permutation(v));
      }
    }
  }
  // insert(get_key, key_less, hint, key, get_new_node, do_when_equal)
  {
    // precise hint insert by ascending order
    {
      tree_t t;
      for (int i : irng(0, 5000))
        assert(*t.insert(as_const, less<int>{}, upper_bound(t, i), i,
                         [&t, i]() {return t.new_node(i);}, empty_function{})
               == i);
      assert(equal(t, irng(0, 5000)));
      assert(t.size() == 5000);
      assert(!t.empty());
      assert(inner::good(t));
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
    }
    // precise hint insert by descending order
    {
      tree_t t;
      for (int i : rrng(irng(0, 5000)))
        assert(*t.insert(as_const, less<int>{}, upper_bound(t, i), i,
                         [&t, i]() {return t.new_node(i);}, empty_function{})
               == i);
      assert(equal(t, irng(0, 5000)));
      assert(t.size() == 5000);
      assert(!t.empty());
      assert(inner::good(t));
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
    }
    // insert by ascending order then hint insert by all possible positions
    {
      for (int i : irng(0, 12))
        for (int j : irng(0, i + 1)) {
          ez_vector<int> v;
          for (int k : irng(0, i + 1)) {
            const auto r = bind_rng(irng(0, i), bind(multiplies<int>{}, 2, _1));
            tree_t t;
            copy(r, to_back(t));
            v.erase(v.begin(), v.end());
            copy(r, to_back(v));
            assert(equal(v, t));
            const int kk = k * 2 - 1;

            const auto it
              = t.insert(as_const, less<int>{}, nth(t, j), kk,
                         [&t, kk]() {return t.new_node(kk);}, empty_function{});
            assert(*it = kk);

            v.insert(upper_bound(v, kk), kk);
            assert(equal(t, v));
            assert(t.size() == v.size());
            assert(t.empty() == v.empty());
            assert(inner::good(t));
            assert(t.leftmost() != t.end() && *t.leftmost() == front(v));
            assert(t.rightmost() != t.end() && *t.rightmost() == back(v));
          }
        }
    }
  }
  // insert + erase
  {
    // ascending order insertion + all possible erasion order
    {
      ez_vector<int> v;
      ez_vector<iter_pair<iterator_iterator<int>>> v_erase_seq;
      const int c = 8;
      for (int i : irng(0, c)) {
        v_erase_seq.erase(v_erase_seq.begin(), v_erase_seq.end());
        for (int j : rrng(irng(0, i)))
          v_erase_seq.insert(v_erase_seq.end(), irng(0, j + 1));
        for (decltype(auto) r
             : inner_cartesian_product_rng(as_const(v_erase_seq))) {
          v.erase(v.begin(), v.end());
          copy(irng(0, i), to_back(v));

          tree_t t;
          for (int x : irng(0, i))
            t.insert(as_const, less<int>{}, x,
                     [&t, x]() {return t.new_node(x);}, empty_function{});
          assert(equal(t, irng(0, i)));
          assert(t.size() == to_unsigned(i));
          assert(!t.empty());
          assert(inner::good(t));
          assert(t.leftmost() != t.end() && *t.leftmost() == 0);
          assert(t.rightmost() != t.end() && *t.rightmost() == i - 1);
          for (int x : r) {
            assert(to_unsigned(x) < v.size());
            const auto v_it = v.erase(nth(v, x));
            const auto t_it = t.erase(nth(t, x));
            assert((t_it == t.end() && v_it == v.end())
                   || (*t_it == *v_it));
            assert(equal(t, v));
            assert(t.size() == v.size());
            assert(t.empty() == v.empty());
            assert(inner::good(t));
            assert((t.leftmost() == t.end()) == v.empty());
            assert((t.rightmost() == t.end()) == v.empty());
            if (!v.empty()) {
              assert(*t.leftmost() == front(v));
              assert(*t.rightmost() == back(v));
            }
          }
          assert(t.empty());
        }
      }
    }
    // randomized insertion order + randomized erasion order
    {
      rander<minstd_rand> g;

      ez_vector<int> v;
      for (int c = 20; c != 0; --c) {
        v.erase(v.begin(), v.end());
        copy(irng(0, static_cast<int>(g(100, 10000))), to_back(v));
        shuffle(v, g);

        tree_t t;
        for (int x : v) {
          const auto [i, success]
            = t.insert(as_const, less<int>{}, x,
                       [&t, x]() {return t.new_node(x);}, empty_function{});
          assert(success);
          assert(*i == x);
        }
        sort(v);
        assert(equal(t, v));
        assert(t.size() == v.size());
        assert(t.empty() == v.empty());
        assert(inner::good(t));
        assert(t.leftmost() != t.end() && *t.leftmost() == front(v));
        assert(t.rightmost() != t.end() && *t.rightmost() == back(v));

        while (!v.empty()) {
          const int i = g(0, v.size() - 1u);
          const auto v_it0 = nth(v, i);
          const auto t_it0 = nth(t, i);
          const auto v_it = v.erase(v_it0);
          const auto t_it = t.erase(t_it0);
          assert((t_it == t.end() && v_it == v.end()) || (*v_it == *t_it));
        }
        assert(t.begin() == t.end());
        assert(t.size() == 0);
        assert(t.empty());
        assert(inner::good(t));
        assert(t.leftmost() == t.end());
        assert(t.rightmost() == t.end());
        assert(t.root() == nullptr);
      }
    }
  }

  // insert_equal(get_key, key_less, key, get_new_node, do_when_equal)
  {
    // insert 5000 values by ascending order then repeat by 2 times
    {
      tree_t t;
      for (int x : irng(0, 5000)) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        assert(it != t.end() && *it == x);
        assert(equal(t, irng(0, x + 1)));
        assert(!t.empty());
        assert(t.leftmost() != t.end() && *t.leftmost() == 0);
        assert(t.rightmost() != t.end() && *t.rightmost() == x);
        assert(t.size() == to_unsigned(x + 1));
      }
      assert(inner::good(t));

      for (int x : irng(0, 5000)) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        const auto it_next = next(it);
        assert(it != t.end() && *it == x);
        assert(it != t.begin() && *prev(it) == x);
        assert(it_next == t.end() || *it_next != x);
      }
      assert(equal(t, join_rng(bind_rng(irng(0, 5000),
                                        [](int z) {return rng(2, z);}))));
      assert(!t.empty());
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
      assert(t.size() == 10000u);
      assert(inner::good(t));

      for (int x : irng(0, 5000)) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        const auto it_next = next(it);
        assert(it != t.end() && *it == x);
        assert(it != t.begin() && *prev(it) == x);
        assert(it_next == t.end() || *it_next != x);
      }
      assert(equal(t, join_rng(bind_rng(irng(0, 5000),
                                        [](int z) {return rng(3, z);}))));
      assert(!t.empty());
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
      assert(t.size() == 15000u);
      assert(inner::good(t));
    }

    // insert 5000 values by descending order then repeat by 2 times
    {
      tree_t t;
      for (int x : rrng(irng(0, 5000))) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        assert(it != t.end() && *it == x);
        assert(equal(t, irng(x, 5000)));
        assert(!t.empty());
        assert(t.leftmost() != t.end() && *t.leftmost() == x);
        assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
        assert(t.size() == to_unsigned(5000 - x));
      }
      assert(inner::good(t));

      for (int x : rrng(irng(0, 5000))) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        const auto it_next = next(it);
        assert(it != t.end() && *it == x);
        assert(it != t.begin() && *prev(it) == x);
        assert(it_next == t.end() || *it_next != x);
      }
      assert(equal(t, join_rng(bind_rng(irng(0, 5000),
                                        [](int z) {return rng(2, z);}))));
      assert(!t.empty());
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
      assert(t.size() == 10000u);
      assert(inner::good(t));

      for (int x : rrng(irng(0, 5000))) {
        const auto it = t.insert_equal(as_const, less<int>{}, x,
                                       [&t, x]() {return t.new_node(x);});
        const auto it_next = next(it);
        assert(it != t.end() && *it == x);
        assert(it != t.begin() && *prev(it) == x);
        assert(it_next == t.end() || *it_next != x);
      }
      assert(equal(t, join_rng(bind_rng(irng(0, 5000),
                                        [](int z) {return rng(3, z);}))));
      assert(!t.empty());
      assert(t.leftmost() != t.end() && *t.leftmost() == 0);
      assert(t.rightmost() != t.end() && *t.rightmost() == 4999);
      assert(t.size() == 15000u);
      assert(inner::good(t));
    }

    // randomized order
    {
      ez_vector<int> v;
      rander<minstd_rand> g;
      for (int i : irng(1, 50)) {
        v.erase(v.begin(), v.end());
        copy(irng(0, i), to_back(v));
        copy(irng(0, i), to_back(v));
        copy(irng(0, i), to_back(v));
        copy(irng(0, i), to_back(v));
        shuffle(v, g);

        tree_t t;
        for (int j : v) {
          const auto it
            = t.insert_equal(as_const, less<int>{}, j,
                             [&t, j]() {return t.new_node(j);});
          assert(it != t.end() && *it == j);
          const auto it2 = next(it);
          assert(it2 == t.end() || *it2 != j);
        }
        sort(v);
        assert(equal(t, v));
        assert(!t.empty());
        assert(t.size() == v.size());
        assert(inner::good(t));
        assert(t.leftmost() != t.end() && *t.leftmost() == front(v));
        assert(t.rightmost() != t.end() && *t.rightmost() == back(v));
        assert(t.root() != nullptr);
      }
    }
  }
  // insert_equal(get_key, key_less, hint, key, get_new_node, do_when_equal)
  {
    tree_t t;
    for (int i : irng(0, 1000)) {
      const auto it = t.insert_equal(as_const, less<int>{},
                                     upper_bound(t, i), i,
                                     [&t, i]() {return t.new_node(i);});
      assert(it != t.end() && *it == i);
    }
    for (int c = 2; c != 0; --c)
      for (int i : irng(0, 1000)) {
        const auto it = t.insert_equal(as_const, less<int>{},
                                       lower_bound(t, i), i,
                                       [&t, i]() {return t.new_node(i);});
        const auto it2 = next(it);
        assert(it != t.end() && *it == i);
        assert(it2 != t.end() && *it2 == i);
      }
    for (int c = 2; c != 0; --c)
      for (int i : irng(0, 1000)) {
        const auto it = t.insert_equal(as_const, less<int>{},
                                       upper_bound(t, i), i,
                                       [&t, i]() {return t.new_node(i);});
        const auto it2 = next(it);
        assert(it2 != t.begin());
        const auto it3 = prev(it2);
        assert(it != t.end() && *it == i);
        assert(it2 == t.end() || *it2 != i);
        assert(*it3 == i);
      }
    assert(equal(t, join_rng(bind_rng(irng(0, 1000),
                                      [](int x) {return rng(5, x);}))));
    assert(!t.empty());
    assert(t.size() == 5000);
    assert(inner::good(t));
    assert(t.leftmost() != t.end() && *t.leftmost() == 0);
    assert(t.rightmost() != t.end() && *t.rightmost() == 999);
    assert(t.root() != nullptr);
  }

  // merge
  {
    ez_vector<int> v, v2, v3;
    for (int i : irng(0, 30))
      for (int j : irng(0, 30))
        for (int k : irng(0, 30)) {
          tree_t t1, t2(t1.get_allocator());
          const auto init = [&t1, &t2, i, j, k]() {
            for (int x : irng(0, i + j))
              t1.insert(as_const, less<int>{}, x,
                        [&t1, x]() {return t1.new_node(x);},
                        empty_function{});
            for (int x : irng(i, i + j + k))
              t2.insert(as_const, less<int>{}, x,
                        [&t2, x]() {return t2.new_node(x);},
                        empty_function{});
          };
          init();
          assert(equal(t1, irng(0, i + j)));
          assert(equal(t2, irng(i, i + j + k)));

          v.erase(v.begin(), v.end());
          v2.erase(v2.begin(), v2.end());
          v3.erase(v3.begin(), v3.end());
          copy(irng(0, i + j), to_back(v));
          copy(irng(i, i + j + k), to_back(v2));
          merge(v, v2, to_back(v3));
          t1.clear();
          t2.clear();
          init();
          assert(equal(t1, irng(0, i + j)));
          assert(equal(t2, irng(i, i + j + k)));
          t1.merge_equal(as_const, less<int>{}, t2);
          assert(equal(t1, v3));
          assert(t1.empty() == v3.empty());
          assert(t1.size() == v3.size());
          assert(inner::good(t1));
          assert(t1.leftmost() == t1.end() || *t1.leftmost() == front(v3));
          assert(t1.rightmost() == t1.end() || *t1.rightmost() == back(v3));
          assert((t1.root() == nullptr) == v3.empty());
          assert(t2.begin() == t2.end());
          assert(t2.cbegin() == t2.cend());
          assert(t2.empty());
          assert(t2.size() == 0);
          assert(inner::good(t2));
          assert(t2.leftmost() == t2.end());
          assert(t2.rightmost() == t2.end());
          assert(t2.root() == nullptr);
        }
  }
}

template <class S, class F, class F2>
void test_rb_set_common_operations_impl(F mk, F2 key) {
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

  // root
  // leftmost
  // rightmost
  {
    S s;
    assert(s.leftmost() == s.end());
    assert(s.rightmost() == s.end());
    assert(s.root() == nullptr);
    assert(as_const(s).leftmost() == s.end());
    assert(as_const(s).rightmost() == s.end());
    assert(as_const(s).root() == nullptr);
    s.insert(mk(1));
    assert(s.leftmost() != s.end() && *s.leftmost() == mk(1));
    assert(s.rightmost() != s.end() && *s.rightmost() == mk(1));
    assert(s.root() != nullptr && *s.root() == mk(1));
    assert(as_const(s).leftmost() != s.end()
           && *as_const(s).leftmost() == mk(1));
    assert(as_const(s).rightmost() != s.end()
           && *as_const(s).rightmost() == mk(1));
    assert(as_const(s).root() != nullptr
           && *as_const(s).root() == mk(1));
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
    assert(s.size() == 3);
    assert(!s.empty());
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

  // emplace
  // emplace_hint
  {
    S s;
    const auto [it, success] = s.emplace(mk(1));
    assert(success && *it == mk(1));
    assert(equal(s, bind_rng(irng(1, 2), mk)));
    const auto it2 = s.emplace_hint(s.begin(), mk(0));
    assert(*it2 == mk(0));
    assert(equal(s, bind_rng(irng(0, 2), mk)));
  }

  // insert
  {
    S s;
    const auto [it, success] = s.insert(mk(1));
    assert(success && *it == mk(1));
    assert(equal(s, bind_rng(irng(1, 2), mk)));
    const auto it2 = s.insert(s.begin(), mk(0));
    assert(*it2 == mk(0));
    assert(equal(s, bind_rng(irng(0, 2), mk)));

    s.clear();
    assert(s.empty());
    s.insert(bind_rng(irng(1, 4), mk));
    assert(equal(s, bind_rng(irng(1, 4), mk)));

    s.clear();
    assert(s.empty());
    const auto tmp_r = bind_rng(irng(1, 4), mk);
    s.insert(begin(tmp_r), end(tmp_r));
    assert(equal(s, bind_rng(irng(1, 4), mk)));

    s.clear();
    assert(s.empty());
    s.insert({mk(1), mk(2), mk(3)});
    assert(equal(s, bind_rng(irng(1, 4), mk)));
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
      assert(s.erase(s.begin(), it) == it);
      assert(equal(s, single_rng(mk(3))));
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
    assert(equal(s2, single_rng(mk(3))));
    assert(equal(s, bind_rng(irng(1, 7), mk)));
  }

  // find
  // contains
  // count
  // lower/upper_bound
  // equal_range
  // partition_point
  // find_range
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

    // lower_bound(key)
    // upper_bound(key)
    // equal_range(key)
    {
      S s;
      s.insert_range(bind_rng(irng(1, 4), mk));
      assert(s.lower_bound(key(mk(2))) == nth(s, 1));
      assert(as_const(s).lower_bound(key(mk(2))) == nth(s, 1));
      assert(s.upper_bound(key(mk(2))) == nth(s, 2));
      assert(as_const(s).upper_bound(key(mk(2))) == nth(s, 2));
      assert(s.equal_range(key(mk(2))) == iter_pair(nth(s, 1), nth(s, 2)));
      assert(as_const(s).equal_range(key(mk(2)))
             == iter_pair(nth(s, 1), nth(s, 2)));
      assert(equal(s, bind_rng(irng(1, 4), mk)));
    }
    // partition_point(eq)
    {
      S s;
      s.insert_range(bind_rng(irng(1, 4), mk));
      assert(s.partition_point
             ([=](const auto &x) {return key(x) < 3;}) == nth(s, 2));
      assert(as_const(s).partition_point
             ([=](const auto &x) {return key(x) < 3;}) == nth(s, 2));
      assert(equal(s, bind_rng(irng(1, 4), mk)));
    }
    // find_range(min, max)
    {
      S s;
      s.insert_range(bind_rng(irng(1, 4), mk));
      assert(equal(s, s.find_range(key(mk(1)), key(mk(3)))));
      assert(equal(s, as_const(s).find_range(key(mk(1)), key(mk(3)))));
      assert(equal(s, bind_rng(irng(1, 4), mk)));
    }
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator it = s.new_node(mk(1));
    assert(*it == mk(1));
    s.delete_node(it);
  }

  // unlink(cit)
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it = nth(s, 1);
    const auto it2 = nth(s, 2);
    assert(s.unlink(it) == it2);
    s.delete_node(it);
    assert(equal(s, bind_rng(seq(1, 3), mk)));
  }
  // unlink(cit, cit)
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it0 = nth(s, 0);
    const auto it1 = nth(s, 1);
    const auto it2 = nth(s, 2);
    assert(s.unlink(it0, it2) == it2);
    s.delete_node(it0);
    s.delete_node(it1);
    assert(equal(s, single_rng(mk(3))));
  }
  // unlink()
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it0 = nth(s, 0);
    const auto it1 = nth(s, 1);
    const auto it2 = nth(s, 2);
    s.unlink();
    s.delete_node(it0);
    s.delete_node(it1);
    s.delete_node(it2);
    assert(s.empty());
  }

  // link(cit)
  {
    S s;
    const auto it = s.new_node(mk(1));
    assert(s.link(it) == pair(it, true));
    const auto it2 = s.new_node(mk(1));
    assert(s.link(it2) == pair(it, false));
    s.delete_node(it2);
    assert(equal(s, single_rng(mk(1))));
  }
  // link(hint, cit)
  {
    S s;
    const auto it = s.new_node(mk(1));
    assert(s.link(s.end(), it) == it);
    assert(equal(s, single_rng(mk(1))));
  }

  // try_link(key, get_node, do_when_eq)
  {
    S s;
    const auto [it, success]
      = s.try_link(key(mk(1)), [&s, mk]() {return s.new_node(mk(1));},
                   empty_function{});
    assert(success);
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1))));
  }
  // try_link_hint(key, get_node, do_when_eq)
  {
    S s;
    const auto it = s.try_link_hint(s.end(), key(mk(1)),
                                    [&s, mk]() {return s.new_node(mk(1));},
                                    empty_function{});
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1))));
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
  }

  // erase_or_unlink(cit)
  {
    S s;
    s.insert(mk(1));
    s.erase_or_unlink(s.begin());
    assert(s.empty());
  }

  // unique
  {
    S s;
    s.unique();
    s.unique(equal_to{});
  }

  // assign(r)
  // assign_range(r)
  // insert(r)
  // insert_range(r)
  {
    S s;
    s.assign(single_rng(mk(1)));
    assert(equal(s, single_rng(mk(1))));
    s.assign_range(single_rng(mk(2)));
    assert(equal(s, single_rng(mk(2))));

    s.insert(seq(mk(1), mk(3)));
    assert(equal(s, seq(mk(1), mk(2), mk(3))));
    s.insert(seq(mk(2), mk(4)));
    assert(equal(s, seq(mk(1), mk(2), mk(3), mk(4))));
  }

  // append(s...)
  {
    S s;
    assert(equal(s.append(mk(1)).append(seq(mk(1), mk(2))),
                 seq(mk(1), mk(2))));
  }
}
void test_rb_set_common_operations() {
  test_rb_set_common_operations_impl<rb_set
                                     <int, less<int>,
                                      stateful_test_allocator<int>>>
    (copy, identity{});
  test_rb_set_common_operations_impl<rb_map
                                     <int, int, less<int>,
                                      stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S, class F, class F2>
void test_rb_multiset_common_operations_impl(F mk, F2 key) {
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

  // root
  // leftmost
  // rightmost
  {
    S s;
    assert(s.leftmost() == s.end());
    assert(s.rightmost() == s.end());
    assert(s.root() == nullptr);
    assert(as_const(s).leftmost() == s.end());
    assert(as_const(s).rightmost() == s.end());
    assert(as_const(s).root() == nullptr);
    s.insert(mk(1));
    assert(s.leftmost() != s.end() && *s.leftmost() == mk(1));
    assert(s.rightmost() != s.end() && *s.rightmost() == mk(1));
    assert(s.root() != nullptr && *s.root() == mk(1));
    assert(as_const(s).leftmost() != s.end()
           && *as_const(s).leftmost() == mk(1));
    assert(as_const(s).rightmost() != s.end()
           && *as_const(s).rightmost() == mk(1));
    assert(as_const(s).root() != nullptr
           && *as_const(s).root() == mk(1));
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
    s.insert(mk(1));
    s.insert(mk(3));
    assert(equal(s, seq(mk(1), mk(1), mk(2), mk(3), mk(3))));
    assert(equal(as_const(s), s));
    assert(s.size() == 5);
    assert(!s.empty());
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
      decltype(mk(1)) a[] = {mk(1), mk(1), mk(2), mk(3)};
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
      S s({mk(1), mk(1), mk(2), mk(3)}, cmp, al1);
      assert(equal(s, seq(mk(1), mk(1), mk(2), mk(3)))
             && s.get_allocator() == al1);
      S s2({mk(1), mk(2), mk(2), mk(3)}, al2);
      assert(equal(s2, seq(mk(1), mk(2), mk(2), mk(3)))
             && s2.get_allocator() == al2);
      S s3 = {mk(3), mk(2), mk(1), mk(3)};
      assert(equal(s3, seq(mk(1), mk(2), mk(3), mk(3))));
      s3 = {mk(5), mk(5), mk(4), mk(6)};
      assert(equal(s3, seq(mk(4), mk(5), mk(5), mk(6))));
      s3.assign({mk(1), mk(1)});
      assert(equal(s3, seq(mk(1), mk(1))));
    }
    // range
    {
      S s(seq(mk(1), mk(1), mk(2), mk(3)), cmp, al1);
      assert(equal(s, seq(mk(1), mk(1), mk(2), mk(3)))
             && s.get_allocator() == al1);
      S s2(seq(mk(1), mk(1), mk(2), mk(3)), al2);
      assert(equal(s2, seq(mk(1), mk(1), mk(2), mk(3)))
             && s2.get_allocator() == al2);
      S s3(seq(mk(3), mk(2), mk(1), mk(3)));
      assert(equal(s3, seq(mk(1), mk(2), mk(3), mk(3))));
      s3 = seq(mk(3), mk(2), mk(1), mk(2));
      assert(equal(s3, seq(mk(1), mk(2), mk(2), mk(3))));
      s3.assign_range(seq(mk(1), mk(1)));
      assert(equal(s3, seq(mk(1), mk(1))));
      s3.assign(seq(mk(2), mk(2), mk(2)));
      assert(equal(s3, seq(mk(2), mk(2), mk(2))));
    }
    // from_range
    {
      S s(from_range, bind_rng(seq(1, 2, 1, 2), mk), cmp, al1);
      assert(equal(s, bind_rng(seq(1, 1, 2, 2), mk))
             && s.get_allocator() == al1);
      S s2(from_range, bind_rng(seq(1, 2, 1), mk), al2);
      assert(equal(s2, bind_rng(seq(1, 1, 2), mk))
             && s2.get_allocator() == al2);
      S s3(from_range, bind_rng(seq(3, 2, 1, 1), mk));
      assert(equal(s3, bind_rng(seq(1, 1, 2, 3), mk)));
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

  // emplace
  // emplace_hint
  {
    S s;
    const auto it = s.emplace(mk(1));
    assert(*it == mk(1));
    assert(equal(s, seq(mk(1))));
    const auto it2 = s.emplace_hint(s.begin(), mk(0));
    assert(*it2 == mk(0));
    assert(*s.emplace(mk(0)) == mk(0));
    assert(equal(s, seq(mk(0), mk(0), mk(1))));
  }

  // insert
  {
    S s;
    const auto it = s.insert(mk(1));
    const auto it2 = s.insert(s.end(), mk(1));
    assert(it == s.begin());
    assert(next(it2) == s.end());
    assert(equal(s, seq(mk(1), mk(1))));

    s.clear();
    assert(s.empty());
    s.insert(seq(mk(1), mk(2), mk(1)));
    assert(equal(s, seq(mk(1), mk(1), mk(2))));

    s.clear();
    assert(s.empty());
    const auto tmp_r = seq(mk(2), mk(3), mk(4), mk(2), mk(3));
    s.insert(begin(tmp_r), end(tmp_r));
    assert(equal(s, seq(mk(2), mk(2), mk(3), mk(3), mk(4))));

    s.clear();
    assert(s.empty());
    s.insert({mk(1), mk(2), mk(1)});
    assert(equal(s, seq(mk(1), mk(1), mk(2))));
  }

  // insert/extract node_handle
  {
    S s;
    s.insert(mk(1));
    typename S::node_type x = s.extract(s.begin());
    s.insert(mk(1));
    typename S::node_type y = s.extract(s.begin());

    const auto it = s.insert(move(x));
    assert(*it == mk(1));
    assert(x.empty());

    const auto it2 = s.insert(move(y));
    assert(*it2 == mk(1));
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
      s.insert(mk(2));
      const auto it = prev(s.end());
      assert(s.erase(s.begin(), it) == it);
      assert(equal(s, single_rng(mk(2))));
    }
    // erase(key)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(1));
      s.insert(mk(1));
      assert(s.erase(key(mk(1))) == 3u);
      assert(s.empty());
    }
    // remove(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.remove(key(mk(1))) == 1u);
      assert(s.empty());
    }
    // remove_if(eq)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      s.insert(mk(3));
      assert(s.remove_if([=](const auto &x) {return key(x) < 3;}) == 2u);
      assert(equal(s, single_rng(mk(3))));
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
    assert(equal(s, seq(mk(1), mk(2), mk(3), mk(3), mk(4), mk(5), mk(6))));
  }

  // find
  // contains
  // count
  // lower/upper_bound
  // equal_range
  // partition_point
  // find_range
  {
    // find(key)
    // contains(key)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(1));
      assert(equal(s, seq(mk(1), mk(1))));

      assert(s.find(key(mk(1))) == s.begin());
      assert(as_const(s).find(key(mk(1))) == s.begin());
      assert(s.contains(key(mk(1))));
      assert(equal(s, seq(mk(1), mk(1))));

      assert(s.find(key(mk(2))) == s.end());
      assert(as_const(s).find(key(mk(2))) == s.end());
      assert(!s.contains(key(mk(2))));
      assert(equal(s, seq(mk(1), mk(1))));
    }

    // count(key)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(1));
      s.insert(mk(2));
      assert(s.count(key(mk(1))) == 2u);
      assert(s.count(key(mk(2))) == 1u);
    }

    // lower_bound(key)
    // upper_bound(key)
    // equal_bound(key)
    {
      S s;
      s.insert_range(seq(mk(1), mk(1), mk(2), mk(2), mk(2), mk(3)));
      assert(s.lower_bound(key(mk(2))) == nth(s, 2));
      assert(as_const(s).lower_bound(key(mk(2))) == nth(s, 2));
      assert(s.upper_bound(key(mk(2))) == nth(s, 5));
      assert(as_const(s).upper_bound(key(mk(2))) == nth(s, 5));
      assert(s.equal_range(key(mk(2))) == iter_pair(nth(s, 2), nth(s, 5)));
      assert(as_const(s).equal_range(key(mk(2)))
             == iter_pair(nth(s, 2), nth(s, 5)));
      assert(equal(s, seq(mk(1), mk(1), mk(2), mk(2), mk(2), mk(3))));
    }
    // partition_point(eq)
    {
      S s;
      s.insert_range(bind_rng(irng(1, 4), mk));
      assert(s.partition_point
             ([=](const auto &x) {return key(x) < 3;}) == nth(s, 2));
      assert(as_const(s).partition_point
             ([=](const auto &x) {return key(x) < 3;}) == nth(s, 2));
      assert(equal(s, bind_rng(irng(1, 4), mk)));
    }
    // find_range(min, max)
    {
      S s;
      s.insert_range(seq(mk(1), mk(2), mk(2), mk(3), mk(4)));
      assert(equal(s.find_range(key(mk(2)), key(mk(3))),
                   seq(mk(2), mk(2), mk(3))));
      assert(equal(as_const(s).find_range(key(mk(2)), key(mk(3))),
                   seq(mk(2), mk(2), mk(3))));
      assert(equal(s, seq(mk(1), mk(2), mk(2), mk(3), mk(4))));
    }
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator it = s.new_node(mk(1));
    assert(*it == mk(1));
    s.delete_node(it);
  }

  // unlink(cit)
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it = nth(s, 1);
    const auto it2 = nth(s, 2);
    assert(s.unlink(it) == it2);
    s.delete_node(it);
    assert(equal(s, bind_rng(seq(1, 3), mk)));
  }
  // unlink(cit, cit)
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it0 = nth(s, 0);
    const auto it1 = nth(s, 1);
    const auto it2 = nth(s, 2);
    assert(s.unlink(it0, it2) == it2);
    s.delete_node(it0);
    s.delete_node(it1);
    assert(equal(s, single_rng(mk(3))));
  }
  // unlink()
  {
    S s(from_range, bind_rng(irng(1, 4), mk));
    const auto it0 = nth(s, 0);
    const auto it1 = nth(s, 1);
    const auto it2 = nth(s, 2);
    s.unlink();
    s.delete_node(it0);
    s.delete_node(it1);
    s.delete_node(it2);
    assert(s.empty());
  }

  // link(cit)
  {
    S s;
    const auto it = s.new_node(mk(1));
    assert(s.link(it) == it);
    const auto it2 = s.new_node(mk(1));
    assert(s.link(it2) == it2);
    assert(equal(s, seq(mk(1), mk(1))));
  }
  // link(hint, cit)
  {
    S s;
    const auto it = s.new_node(mk(1));
    assert(s.link(s.end(), it) == it);
    assert(equal(s, single_rng(mk(1))));
  }

  // try_link(key, get_node, do_when_eq)
  {
    S s;
    const auto it = s.try_link(key(mk(1)),
                               [&s, mk]() {return s.new_node(mk(1));});
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1))));
  }
  // try_link_hint(key, get_node, do_when_eq)
  {
    S s;
    const auto it = s.try_link_hint(s.end(), key(mk(1)),
                                    [&s, mk]() {return s.new_node(mk(1));});
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1))));
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
  }

  // erase_or_unlink(cit)
  {
    S s;
    s.insert(mk(1));
    s.erase_or_unlink(s.begin());
    assert(s.empty());
  }

  // unique
  {
    S s(from_range, seq(mk(1), mk(1), mk(2), mk(2), mk(2), mk(3), mk(4)));
    s.unique();
    assert(equal(s, seq(mk(1), mk(2), mk(3), mk(4))));
    s.unique([=](const auto &x, const auto &y) {return key(x) + 1 == key(y);});
    assert(equal(s, seq(mk(1), mk(3))));
  }

  // assign(r)
  // assign_range(r)
  // insert(r)
  // insert_range(r)
  {
    S s;
    s.assign(seq(mk(1), mk(1)));
    assert(equal(s, seq(mk(1), mk(1))));
    s.assign_range(seq(mk(2), mk(2)));
    assert(equal(s, seq(mk(2), mk(2))));

    s.insert(seq(mk(1), mk(3)));
    assert(equal(s, seq(mk(1), mk(2), mk(2), mk(3))));
    s.insert(seq(mk(2), mk(4)));
    assert(equal(s, seq(mk(1), mk(2), mk(2), mk(2), mk(3), mk(4))));
  }

  // append(s...)
  {
    S s;
    assert(equal(s.append(mk(1)).append(seq(mk(1), mk(2))),
                 seq(mk(1), mk(1), mk(2))));
  }
}
void test_rb_multiset_common_operations() {
  test_rb_multiset_common_operations_impl<rb_multiset
                                          <int, less<int>,
                                           stateful_test_allocator<int>>>
    (copy, identity{});
  test_rb_multiset_common_operations_impl<rb_multimap
                                          <int, int, less<int>,
                                           stateful_test_allocator
                                           <pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S>
void test_rb_map_specialized_operations_impl() {
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
void test_rb_map_specialized_operations() {
  test_rb_map_specialized_operations_impl
    <rb_map<int, int, less<int>, test_allocator<pair<int, int>>>>();
  test_rb_map_specialized_operations_impl
    <rb_map<int, int, less<>, test_allocator<pair<int, int>>>>();
}

void test_rb_map() {
  printf("rb_map: ");

  inner::fns::test_rbtree_adaptor();
  inner::fns::test_rb_set_common_operations();
  inner::fns::test_rb_multiset_common_operations();
  inner::fns::test_rb_map_specialized_operations();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_rb_map();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
