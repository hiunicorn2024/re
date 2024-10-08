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

template <class S, class F, class F2>
void test_mixed_set_common_operations_impl(F mk, F2 key) {
  // tree_iter
  // unordered_iter
  // tree()
  // unordered()
  {
    S s = {mk(1)};

    assert(equal(s.tree(), single_rng(mk(1))));
    assert(equal(as_const(s).tree(), single_rng(mk(1))));
    assert(equal(s.unordered(), single_rng(mk(1))));
    assert(equal(as_const(s).unordered(), single_rng(mk(1))));

    assert(s.tree_iter(s.tree().end()) == s.end());
    assert(s.tree_iter(s.tree().end()) == s.tree().end());
    assert(s.tree_iter(s.tree().cend()) == s.end());
    assert(s.tree_iter(s.tree().cend()) == s.tree().end());
    assert(s.tree_iter(s.unordered().end()) == s.end());
    assert(s.tree_iter(s.unordered().end()) == s.tree().end());
    assert(s.tree_iter(s.unordered().cend()) == s.end());
    assert(s.tree_iter(s.unordered().cend()) == s.tree().end());

    assert(s.unordered_iter(s.tree().end()) == s.unordered().end());
    assert(s.unordered_iter(s.tree().cend()) == s.unordered().end());
    assert(s.unordered_iter(s.unordered().end()) == s.unordered().end());
    assert(s.unordered_iter(s.unordered().cend()) == s.unordered().end());

    assert(S::no_ed_unordered_iter(s.begin()) == s.unordered().begin());
    assert(S::no_ed_unordered_iter(s.unordered().begin())
           == s.unordered().begin());
    assert(S::no_ed_tree_iter(s.unordered().begin()) == s.tree().begin());
    assert(S::no_ed_tree_iter(s.tree().begin()) == s.tree().begin());
  }

  // range operations
  {
    {
      S s;
      assert(s.begin() == s.end());
      assert(s.begin() == s.cend());
      assert(s.cbegin() == s.end());
      assert(s.cbegin() == s.cend());
      assert(s.max_size() > 0u);
      assert(s.size() == 0u);
      assert(s.empty());
    }
    {
      S s = {mk(1)};
      assert(next(s.begin()) == s.end());
      assert(next(s.begin()) == s.cend());
      assert(next(s.cbegin()) == s.end());
      assert(next(s.cbegin()) == s.cend());
      assert(s.max_size() > 0u);
      assert(s.size() == 1u);
      assert(!s.empty());
    }
  }

  // special member functions
  {
    static_assert(regular<S>);
    static_assert(is_nothrow_movable_v<S>);
    static_assert(!is_nothrow_copyable_v<S>);
    static_assert(is_nothrow_swappable_v<S>);

    S s1 = {mk(1)};
    S s2 = {mk(2), mk(3)};
    // copy constructor
    {
      S s(s1);
      assert(s == s1);
    }
    // copy assignment
    {
      S s(s1);
      assert(addressof(s = s2) == addressof(s));
      assert(s == s2);
    }
    // move constructor
    {
      S s(move(s1));
      assert(s1.tree().empty());
      assert(s1.unordered().empty());
      assert(equal(s.tree(), single_rng(mk(1))));
      assert(equal(s.unordered(), single_rng(mk(1))));
    }
    // move assignment
    {
      assert(addressof(s1 = move(s2)) == addressof(s1));
      assert(s2.tree().empty());
      assert(s2.unordered().empty());
      assert(equal(s1.tree(), seq(mk(2), mk(3))));
      assert(equal(s1.unordered(), seq(mk(2), mk(3)))
             || equal(s1.unordered(), seq(mk(3), mk(2))));
    }
    // swap
    {
      s1 = {mk(1)};
      s2 = {mk(2), mk(3)};
      adl_swap(s1, s2);
      adl_swap(s1, s1);
      adl_swap(s2, s2);
      assert(equal(s1.tree(), seq(mk(2), mk(3))));
      assert(equal(s1.unordered(), seq(mk(2), mk(3)))
             || equal(s1.unordered(), seq(mk(3), mk(2))));
      assert(equal(s2.tree(), single_rng(mk(1))));
      assert(equal(s2.unordered(), single_rng(mk(1))));
    }
  }

  // == <=>
  {
    static_assert(equality_comparable<S>);
    static_assert(three_way_comparable<S>);
    S s = {mk(1), mk(2)};
    S s2;
    assert(s == s);
    assert((s <=> s) == 0);
    assert(s != s2);
    assert((s <=> s2) > 0);
  }

  // rbegin
  // rend
  // crbegin
  // crend
  {
    S s = {mk(1), mk(2)};
    assert(equal(rng(s.rbegin(), s.rend()),
                 seq(mk(2), mk(1))));
    assert(equal(rng(as_const(s).rbegin(), as_const(s).rend()),
                 seq(mk(2), mk(1))));
    assert(equal(rng(s.crbegin(), s.crend()),
                 seq(mk(2), mk(1))));
  }

  // constructor and assignment
  {
    hash<int> hf;
    equal_to<int> eq;
    less<int> cmp;
    const typename S::allocator_type a;

    // get_allocator()
    // this_t(alloc);
    // this_t(const this_t &, alloc);
    // this_t(this_t &&, alloc);
    {
      S s(a);
      assert(s.get_allocator() == a);

      S s2 = {mk(1)};
      S s3(s2, a);
      assert(s2.get_allocator() != a);
      assert(equal(s2, single_rng(mk(1))));
      assert(s3.get_allocator() == a);
      assert(equal(s3, single_rng(mk(1))));

      S s4(move(s2), a);
      S s5(move(s3), a);
      assert(s2.empty() && s3.empty());
      assert(equal(s4, single_rng(mk(1))));
      assert(equal(s5, single_rng(mk(1))));
      assert(s4.get_allocator() == a);
      assert(s5.get_allocator() == a);
    }

    // this_t(hf, eq, cmp)
    {
      S s(hf, eq, cmp);
      assert(s.empty() && s.get_allocator() != a);
    }
    // this_t(hf, eq, cmp, a)
    {
      S s(hf, eq, cmp, a);
      assert(s.empty() && s.get_allocator() == a);
    }

    // this_t(first, last, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(first, last, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(first, last)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r));
      assert(equal(s, r));
    }
    // this_t(first, last, alloc)
    // assign(first, last)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), a);
      assert(equal(s, r) && s.get_allocator() == a);

      const auto r2 = single_rng(mk(3));
      s.assign(begin(r2), end(r2));
      assert(equal(s, r2));
    }

    // this_t(il)
    {
      S s = {mk(1), mk(2)};
      assert(equal(s, seq(mk(1), mk(2))));
    }
    // this_t(il, alloc)
    {
      S s({mk(1), mk(2)}, a);
      assert(equal(s, seq(mk(1), mk(2))));
      assert(s.get_allocator() == a);
    }
    // this_t(il, hf, eq, cmp)
    {
      S s({mk(1), mk(2)}, hf, eq, cmp);
      assert(equal(s, seq(mk(1), mk(2))));
    }
    // this_t(il, hf, eq, cmp, alloc)
    // =(il)
    // assign(il)
    {
      S s({mk(1), mk(2)}, hf, eq, cmp, a);
      assert(equal(s, seq(mk(1), mk(2))));
      assert(s.get_allocator() == a);

      s = {mk(1)};
      assert(equal(s, single_rng(mk(1))));
      s.assign({mk(2)});
      assert(equal(s, single_rng(mk(2))));
    }

    // this_t(from_range, r, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(from_range, r, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(from_range, r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r);
      assert(equal(s, r));
    }
    // this_t(from_range, r, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }

    // this_t(r, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r, hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(r, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r, hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r);
      assert(equal(s, r));
    }
    // this_t(r, alloc)
    // =(r)
    // assign(r)
    // assign_range(r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r, a);
      assert(equal(s, r) && s.get_allocator() == a);

      s.assign(seq(mk(3)));
      assert(equal(s, seq(mk(3))));

      s.assign_range(seq(mk(4), mk(5)));
      assert(equal(s, seq(mk(4), mk(5))));
    }
  }

  // key_comp
  // value_comp
  {
    S s;
    assert(s.key_comp()(key(mk(1)), key(mk(2))));
    assert(s.value_comp()(mk(1), mk(2)));
  }

  // emplace(s...)
  // emplace_hint(s...)
  {
    S s;
    auto [it, success] = s.emplace(mk(1));
    assert(success && *it == mk(1));
    tie(it, success) = s.emplace(mk(1));
    assert(!success && *it == mk(1));
    assert(equal(s.tree(), seq(mk(1))));
    assert(equal(s.unordered(), seq(mk(1))));

    it = s.emplace_hint(s.end(), mk(1));
    assert(it == s.begin());
    assert(equal(s.tree(), seq(mk(1))));
    assert(equal(s.unordered(), seq(mk(1))));

    it = s.emplace_hint(s.end(), mk(2));
    assert(equal(s.tree(), seq(mk(1), mk(2))));
    assert(equal(s.unordered(), seq(mk(1), mk(2)))
           || equal(s.unordered(), seq(mk(2), mk(1))));
  }

  // insert(x)
  // insert(hint, x)
  {
    S s;
    auto [it, success] = s.insert(mk(1));
    assert(success && *it == mk(1));
    tie(it, success) = s.insert(mk(1));
    assert(!success && *it == mk(1));
    assert(equal(s.tree(), seq(mk(1))));
    assert(equal(s.unordered(), seq(mk(1))));

    it = s.insert(s.end(), mk(1));
    assert(it == s.begin());
    assert(equal(s.tree(), seq(mk(1))));
    assert(equal(s.unordered(), seq(mk(1))));

    it = s.insert(s.end(), mk(2));
    assert(equal(s.tree(), seq(mk(1), mk(2))));
    assert(equal(s.unordered(), seq(mk(1), mk(2)))
           || equal(s.unordered(), seq(mk(2), mk(1))));
  }

  // insert(from, to)
  // insert(il)
  // insert(r)
  // insert_range(r)
  {
    S s;
    const auto r = seq(mk(1), mk(2));
    s.insert(begin(r), end(r));
    s.insert({mk(0), mk(2), mk(3)});
    s.insert(seq(mk(2), mk(4)));
    s.insert_range(seq(mk(2), mk(5), mk(4)));
    assert(equal(s.tree(), bind_rng(irng(0, 6), mk)));
  }

  // extract(cit)
  // extract(key)
  // insert(nh)
  // insert(hing, nh)
  {
    // no hint
    {
      S s = {mk(1)};
      typename S::node_type n = s.extract(s.begin());
      assert(!n.empty());
      s = {mk(1)};
      typename S::node_type n2 = s.extract(key(mk(1)));
      assert(!n2.empty());
      typename S::node_type n3 = s.extract(key(mk(1)));
      assert(n3.empty());

      typename S::insert_return_type x = s.insert(move(n));
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
      assert(x.position == s.begin());
      assert(x.inserted);
      assert(x.node.empty() && n.empty());

      x = s.insert(move(n));
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
      assert(x.position == s.end());
      assert(!x.inserted);
      assert(x.node.empty() && n.empty());

      x = s.insert(move(n2));
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
      assert(x.position == s.begin());
      assert(!x.inserted);
      assert(!x.node.empty() && n2.empty());
    }
    // hint
    {
      S s = {mk(1)};
      typename S::node_type n = s.extract(s.begin());
      s = {mk(1)};
      typename S::node_type n2 = s.extract(key(mk(1)));

      typename S::iterator it = s.insert(s.end(), move(n));
      assert(it == s.begin() && n.empty());
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));

      it = s.insert(s.end(), move(n));
      assert(it == s.end() && n.empty());
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));

      it = s.insert(s.end(), move(n2));
      assert(it == s.begin() && !n2.empty());
      assert(equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
    }
  }

  // erase
  // clear()
  {
    // erase(cit)
    {
      S s = {mk(1)};
      assert(s.erase(s.begin()) == s.end());
      assert(s.empty());
      assert(s.tree().empty());
      assert(s.unordered().empty());
    }
    // erase(cit, cit)
    {
      S s = {mk(1), mk(2), mk(3), mk(4)};
      const auto it = nth(s, 3);
      assert(s.erase(nth(s, 1), it) == it);
      assert(equal(s, seq(mk(1), mk(4))));
      assert(equal(s.unordered(), seq(mk(1), mk(4)))
             || equal(s.unordered(), seq(mk(4), mk(1))));
    }
  }

  // erase(key)
  // remove
  // remove_if
  {
    // erase(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.erase(1) == 1u);
      assert(s.empty() && s.unordered().empty());
    }
    // remove(key)
    {
      S s;
      s.insert(mk(1));
      assert(s.remove(1) == 1u);
      assert(s.empty() && s.unordered().empty());
    }
    // remove_if(eq)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      assert(s.remove_if([=](const auto &x) {return key(x) < 3;}) == 2u);
      assert(s.empty() && s.unordered().empty());
    }
  }

  // merge
  {
    S s1 = {mk(1), mk(2), mk(3)};
    S s2({mk(2), mk(3), mk(4)}, s1.get_allocator());
    s1.merge(s2);
    assert(equal(s1.tree(), seq(mk(1), mk(2), mk(3), mk(4))));
    assert(is_permutation(s1.unordered(), s1.tree()));
    assert(equal(s2.tree(), seq(mk(2), mk(3))));
    assert(is_permutation(s2.unordered(), s2.tree()));
    s2.merge(move(s1));
    assert(equal(s2.tree(), seq(mk(1), mk(2), mk(3), mk(4))));
    assert(is_permutation(s2.unordered(), s2.tree()));
    assert(equal(s1.tree(), seq(mk(2), mk(3))));
    assert(is_permutation(s1.unordered(), s1.tree()));
  }

  // find(key)
  // contains(key)
  // count(key)
  {
    S s = {mk(1), mk(2), mk(3)};
    {
      assert(s.find(4) == s.end());
      const auto it = s.find(2);
      assert(it == nth(s, 1) && *it == mk(2));
      assert(!s.contains(4));
      assert(s.contains(2));
      assert(s.count(4) == 0u);
      assert(s.count(2) == 1u);
    }
    {
      assert(as_const(s).find(4) == as_const(s).end());
      const auto it = as_const(s).find(2);
      assert(it == nth(s, 1) && *it == mk(2));
      assert(!as_const(s).contains(4));
      assert(as_const(s).contains(2));
      assert(as_const(s).count(4) == 0u);
      assert(as_const(s).count(2) == 1u);
    }
  }

  // lower_bound(key)
  // upper_bound(key)
  // equal_range(key)
  // find_range(min, max)
  {
    S s = {mk(1), mk(2), mk(3)};

    assert(s.lower_bound(4) == s.tree().lower_bound(4));
    assert(s.upper_bound(4) == s.tree().upper_bound(4));
    assert(s.equal_range(4) == s.tree().equal_range(4));
    assert(s.lower_bound(2) == s.tree().lower_bound(2));
    assert(s.upper_bound(2) == s.tree().upper_bound(2));
    assert(s.equal_range(2) == s.tree().equal_range(2));
    assert(s.find_range(1, 2) == iter_pair(nth(s, 0), nth(s, 2)));

    assert(as_const(s).lower_bound(4) == as_const(s).tree().lower_bound(4));
    assert(as_const(s).upper_bound(4) == as_const(s).tree().upper_bound(4));
    assert(as_const(s).equal_range(4) == as_const(s).tree().equal_range(4));
    assert(as_const(s).lower_bound(2) == as_const(s).tree().lower_bound(2));
    assert(as_const(s).upper_bound(2) == as_const(s).tree().upper_bound(2));
    assert(as_const(s).equal_range(2) == as_const(s).tree().equal_range(2));
    assert(as_const(s).find_range(1, 2) == iter_pair(nth(s, 0), nth(s, 2)));
  }

  // partition_point(eql)
  {
    S s = {mk(2), mk(4), mk(6), mk(7), mk(9)};
    const auto it = s.partition_point([=](auto x) {return key(x) % 2 == 0;});
    assert(*it == mk(7));
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator it = s.new_node(mk(1));
    assert(*it == mk(1));
    s.delete_node(it);
  }

  // unlink
  // unlink_key
  // link
  {
    // unlink(cit)
    {
      S s = {mk(1)};
      auto it = s.begin();
      assert(s.unlink(s.begin()) == s.end());
      s.delete_node(it);
    }
    // unlink(cit, cit)
    {
      S s = {mk(1), mk(2)};
      const auto it = nth(s, 0);
      const auto it2 = nth(s, 1);
      assert(s.unlink(s.begin(), s.end()) == s.end());
      s.delete_node(it);
      s.delete_node(it2);
    }
    // unlink()
    {
      S s = {mk(1), mk(2)};
      const auto it = nth(s, 0);
      const auto it2 = nth(s, 1);
      s.unlink();
      assert(s.empty() && s.unordered().empty());
      s.delete_node(it);
      s.delete_node(it2);
    }
    // unlink_key(key)
    {
      S s = {mk(1), mk(2), mk(3)};
      const auto it = nth(s, 1);
      assert(s.unlink_key(0) == 0u);
      assert(s.unlink_key(4) == 0u);
      assert(s.unlink_key(2) == 1u);
      s.delete_node(it);
    }
  }

  // try_link(key, get_node)
  // try_link(key, get_node, do_when_eq)
  // try_link_hint(hint, key, get_node)
  // try_link_hint(hint, key, get_node, do_when_eq)
  {
    S s;
    auto [it, y] = s.try_link(1, [&]() {return s.new_node(mk(1));});
    assert(y && *it == mk(1));
    tie(it, y) = s.try_link(1, [&]() {return s.new_node(mk(1));},
                            empty_function{});
    assert(!y && *it == mk(1));
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));

    it = s.try_link_hint(s.begin(), 1, [&]() {return s.new_node(mk(1));});
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));

    it = s.try_link_hint(s.begin(), 1, [&]() {return s.new_node(mk(1));},
                         empty_function{});
    assert(it == s.begin());
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
  }

  // front
  // back
  {
    S s = {mk(1)};
    assert(s.front() == mk(1));
    assert(as_const(s).front() == mk(1));
    assert(s.back() == mk(1));
    assert(as_const(s).back() == mk(1));
  }

  // root
  // leftmost
  // rightmost
  {
    S s;
    assert(s.root() == nullptr);
    assert(s.leftmost() == s.end());
    assert(s.rightmost() == s.end());
    s = {mk(1)};
    assert(s.root() == s.begin());
    assert(s.leftmost() == s.begin());
    assert(s.rightmost() == s.begin());
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
  }

  // erase_or_unlink
  {
    S s = {mk(1)};
    s.erase_or_unlink(s.begin());
    assert(s.empty() && s.unordered().empty());
  }

  // unique
  {
    S s;
    s.unique();
    assert(s.empty() && s.unordered().empty());
    s = {mk(1), mk(2)};
    s.unique([=](auto x, auto y) {return key(x) + 1 == key(y);});
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
  }

  // hash_function
  // key_eq
  {
    S s;
    typename S::hasher hf = s.hash_function();
    assert(hf(0) == 0);
    typename S::key_equal eq = s.key_eq();
    assert(eq(0, 0) && !eq(0, 1));
  }

  // bucket_count
  // max_bucket_count
  // active_bucket_count
  // bucket_size
  // bucket_range
  // bucket
  // load_factor
  // max_load_factor
  // rehash
  // reserve
  // begin(n)
  // end(n)
  // cbegin(n)
  // cend(n)
  {
    S s;
    assert(s.bucket_count() == 0u);
    assert(s.max_bucket_count() > 0u);
    assert(s.active_bucket_count() == 0u);
    s = {mk(1)};
    assert(s.bucket_size(0u) == 0u);
    assert(empty(s.bucket_range(0u)));
    assert(s.bucket(1) == 1u);
    assert(s.load_factor() < 1.0f);
    assert(s.max_load_factor() == 1.0f);
    s.max_load_factor(2.0f);
    assert(s.max_load_factor() == 2.0f);
    s.rehash(100u);
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
    assert(s.begin(1u) != s.end(1u));
    assert(as_const(s).begin(1u) != as_const(s).end(1u));
    assert(s.cbegin(1u) != s.cend(1u));
  }

  // append
  {
    S s;
    assert(equal(s.append(mk(1), mk(2)).append(seq(mk(1), mk(3))),
                 bind_rng(irng(1, 4), mk)));
    assert(is_permutation(s.unordered(), bind_rng(irng(1, 4), mk)));
  }
}
void test_mixed_set_common_operations() {
  test_mixed_set_common_operations_impl
    <mixed_set<int, hash<int>, equal_to<int>, less<int>,
               stateful_test_allocator<int>>>(copy, identity{});
  test_mixed_set_common_operations_impl
    <mixed_map<int, int, hash<int>, equal_to<int>, less<int>,
               stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S, class F, class F2>
void test_mixed_multiset_common_operations_impl(F mk, F2 key) {
  // tree_iter
  // unordered_iter
  // tree()
  // unordered()
  {
    S s = {mk(1)};

    assert(equal(s.tree(), single_rng(mk(1))));
    assert(equal(as_const(s).tree(), single_rng(mk(1))));
    assert(equal(s.unordered(), single_rng(mk(1))));
    assert(equal(as_const(s).unordered(), single_rng(mk(1))));

    assert(s.tree_iter(s.tree().end()) == s.end());
    assert(s.tree_iter(s.tree().end()) == s.tree().end());
    assert(s.tree_iter(s.tree().cend()) == s.end());
    assert(s.tree_iter(s.tree().cend()) == s.tree().end());
    assert(s.tree_iter(s.unordered().end()) == s.end());
    assert(s.tree_iter(s.unordered().end()) == s.tree().end());
    assert(s.tree_iter(s.unordered().cend()) == s.end());
    assert(s.tree_iter(s.unordered().cend()) == s.tree().end());

    assert(s.unordered_iter(s.tree().end()) == s.unordered().end());
    assert(s.unordered_iter(s.tree().cend()) == s.unordered().end());
    assert(s.unordered_iter(s.unordered().end()) == s.unordered().end());
    assert(s.unordered_iter(s.unordered().cend()) == s.unordered().end());

    assert(S::no_ed_unordered_iter(s.begin()) == s.unordered().begin());
    assert(S::no_ed_unordered_iter(s.unordered().begin())
           == s.unordered().begin());
    assert(S::no_ed_tree_iter(s.unordered().begin()) == s.tree().begin());
    assert(S::no_ed_tree_iter(s.tree().begin()) == s.tree().begin());
  }

  // range operations
  {
    {
      S s;
      assert(s.begin() == s.end());
      assert(s.begin() == s.cend());
      assert(s.cbegin() == s.end());
      assert(s.cbegin() == s.cend());
      assert(s.max_size() > 0u);
      assert(s.size() == 0u);
      assert(s.empty());
    }
    {
      S s = {mk(1)};
      assert(next(s.begin()) == s.end());
      assert(next(s.begin()) == s.cend());
      assert(next(s.cbegin()) == s.end());
      assert(next(s.cbegin()) == s.cend());
      assert(s.max_size() > 0u);
      assert(s.size() == 1u);
      assert(!s.empty());
    }
  }

  // special member functions
  {
    static_assert(regular<S>);
    static_assert(is_nothrow_movable_v<S>);
    static_assert(!is_nothrow_copyable_v<S>);
    static_assert(is_nothrow_swappable_v<S>);

    S s1 = {mk(1)};
    S s2 = {mk(2), mk(3)};
    // copy constructor
    {
      S s(s1);
      assert(s == s1);
    }
    // copy assignment
    {
      S s(s1);
      assert(addressof(s = s2) == addressof(s));
      assert(s == s2);
    }
    // move constructor
    {
      S s(move(s1));
      assert(s1.tree().empty());
      assert(s1.unordered().empty());
      assert(equal(s.tree(), single_rng(mk(1))));
      assert(equal(s.unordered(), single_rng(mk(1))));
    }
    // move assignment
    {
      assert(addressof(s1 = move(s2)) == addressof(s1));
      assert(s2.tree().empty());
      assert(s2.unordered().empty());
      assert(equal(s1.tree(), seq(mk(2), mk(3))));
      assert(equal(s1.unordered(), seq(mk(2), mk(3)))
             || equal(s1.unordered(), seq(mk(3), mk(2))));
    }
    // swap
    {
      s1 = {mk(1)};
      s2 = {mk(2), mk(3)};
      adl_swap(s1, s2);
      adl_swap(s1, s1);
      adl_swap(s2, s2);
      assert(equal(s1.tree(), seq(mk(2), mk(3))));
      assert(equal(s1.unordered(), seq(mk(2), mk(3)))
             || equal(s1.unordered(), seq(mk(3), mk(2))));
      assert(equal(s2.tree(), single_rng(mk(1))));
      assert(equal(s2.unordered(), single_rng(mk(1))));
    }
  }

  // == <=>
  {
    static_assert(equality_comparable<S>);
    static_assert(three_way_comparable<S>);
    S s = {mk(1), mk(1), mk(2), mk(100)};
    S s2;
    S s3 = s;
    s3.rehash(100);
    assert(s == s);
    assert((s <=> s) == 0);
    assert(s != s2);
    assert((s <=> s2) > 0);
    assert(s == s3);
    assert((s <=> s3) == 0);
  }

  // rbegin
  // rend
  // crbegin
  // crend
  {
    S s = {mk(1), mk(2)};
    assert(equal(rng(s.rbegin(), s.rend()),
                 seq(mk(2), mk(1))));
    assert(equal(rng(as_const(s).rbegin(), as_const(s).rend()),
                 seq(mk(2), mk(1))));
    assert(equal(rng(s.crbegin(), s.crend()),
                 seq(mk(2), mk(1))));
  }

  // constructor and assignment
  {
    hash<int> hf;
    equal_to<int> eq;
    less<int> cmp;
    const typename S::allocator_type a;

    // get_allocator()
    // this_t(alloc);
    // this_t(const this_t &, alloc);
    // this_t(this_t &&, alloc);
    {
      S s(a);
      assert(s.get_allocator() == a);

      S s2 = {mk(1)};
      S s3(s2, a);
      assert(s2.get_allocator() != a);
      assert(equal(s2, single_rng(mk(1))));
      assert(s3.get_allocator() == a);
      assert(equal(s3, single_rng(mk(1))));

      S s4(move(s2), a);
      S s5(move(s3), a);
      assert(s2.empty() && s3.empty());
      assert(equal(s4, single_rng(mk(1))));
      assert(equal(s5, single_rng(mk(1))));
      assert(s4.get_allocator() == a);
      assert(s5.get_allocator() == a);
    }

    // this_t(hf, eq, cmp)
    {
      S s(hf, eq, cmp);
      assert(s.empty() && s.get_allocator() != a);
    }
    // this_t(hf, eq, cmp, a)
    {
      S s(hf, eq, cmp, a);
      assert(s.empty() && s.get_allocator() == a);
    }

    // this_t(first, last, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(first, last, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(first, last)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r));
      assert(equal(s, r));
    }
    // this_t(first, last, alloc)
    // assign(first, last)
    {
      const auto r = seq(mk(1), mk(2));
      S s(begin(r), end(r), a);
      assert(equal(s, r) && s.get_allocator() == a);

      const auto r2 = single_rng(mk(3));
      s.assign(begin(r2), end(r2));
      assert(equal(s, r2));
    }

    // this_t(il)
    {
      S s = {mk(1), mk(2)};
      assert(equal(s, seq(mk(1), mk(2))));
    }
    // this_t(il, alloc)
    {
      S s({mk(1), mk(2)}, a);
      assert(equal(s, seq(mk(1), mk(2))));
      assert(s.get_allocator() == a);
    }
    // this_t(il, hf, eq, cmp)
    {
      S s({mk(1), mk(2)}, hf, eq, cmp);
      assert(equal(s, seq(mk(1), mk(2))));
    }
    // this_t(il, hf, eq, cmp, alloc)
    // =(il)
    // assign(il)
    {
      S s({mk(1), mk(2)}, hf, eq, cmp, a);
      assert(equal(s, seq(mk(1), mk(2))));
      assert(s.get_allocator() == a);

      s = {mk(1)};
      assert(equal(s, single_rng(mk(1))));
      s.assign({mk(2)});
      assert(equal(s, single_rng(mk(2))));
    }

    // this_t(from_range, r, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(from_range, r, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(from_range, r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r);
      assert(equal(s, r));
    }
    // this_t(from_range, r, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(from_range, r, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }

    // this_t(r, hf, eq, cmp)
    {
      const auto r = seq(mk(1), mk(1), mk(2));
      S s(r, hf, eq, cmp);
      assert(equal(s, r));
    }
    // this_t(r, hf, eq, cmp, alloc)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r, hf, eq, cmp, a);
      assert(equal(s, r) && s.get_allocator() == a);
    }
    // this_t(r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r);
      assert(equal(s, r));
    }
    // this_t(r, alloc)
    // =(r)
    // assign(r)
    // assign_range(r)
    {
      const auto r = seq(mk(1), mk(2));
      S s(r, a);
      assert(equal(s, r) && s.get_allocator() == a);

      s.assign(seq(mk(3)));
      assert(equal(s, seq(mk(3))));

      s.assign_range(seq(mk(4), mk(5)));
      assert(equal(s, seq(mk(4), mk(5))));
    }
  }

  // key_comp
  // value_comp
  {
    S s;
    assert(s.key_comp()(key(mk(1)), key(mk(2))));
    assert(s.value_comp()(mk(1), mk(2)));
  }

  // emplace(s...)
  {
    S s;
    auto it = s.emplace(mk(1));
    assert(it == s.begin() && *it == mk(1));
    it = s.emplace(mk(1));
    assert(it == next(s.begin()) && *it == mk(1));
    assert(equal(s.tree(), rng(2, mk(1))));
    assert(equal(s.unordered(), rng(2, mk(1))));
  }
  // emplace_hint(s...)
  {
    {
      S s = {mk(1)};
      auto it = s.emplace_hint(s.begin(), mk(1));
      assert(it == s.begin());
      assert(equal(s.tree(), rng(2, mk(1))));
      assert(equal(s.unordered(), rng(2, mk(1))));
      assert(equal(irng(s.tree().equal_range(1)),
                   irng(s.unordered().equal_range(1)),
                   [](auto x, auto y) {return x == S::no_ed_tree_iter(y);}));
    }
    {
      S s = {mk(1), mk(1)};
      auto it = s.emplace_hint(nth(s, 1), mk(1));
      assert(it == nth(s, 1));
      assert(equal(s.tree(), rng(3, mk(1))));
      assert(equal(s.unordered(), rng(3, mk(1))));
      assert(equal(irng(s.tree().equal_range(1)),
                   irng(s.unordered().equal_range(1)),
                   [](auto x, auto y) {return x == S::no_ed_tree_iter(y);}));
    }
    {
      S s = {mk(1), mk(1)};
      auto it = s.emplace_hint(s.end(), mk(1));
      assert(it == nth(s, 2));
      assert(equal(s.tree(), rng(3, mk(1))));
      assert(equal(s.tree(), s.unordered()));
      assert(equal(irng(s.tree().equal_range(1)),
                   irng(s.unordered().equal_range(1)),
                   [](auto x, auto y) {return x == S::no_ed_tree_iter(y);}));
    }
    {
      S s = {mk(1), mk(1), mk(3)};
      auto it = s.emplace_hint(nth(s, 2), mk(1));
      assert(it == nth(s, 2));
      assert(equal(s.tree(), seq(mk(1), mk(1), mk(1), mk(3))));
      assert(equal(s.unordered(), seq(mk(1), mk(1), mk(1), mk(3)))
             || equal(s.unordered(), seq(mk(3), mk(1), mk(1), mk(1))));
      assert(equal(irng(s.tree().equal_range(1)),
                   irng(s.unordered().equal_range(1)),
                   [](auto x, auto y) {return x == S::no_ed_tree_iter(y);}));
    }
    {
      S s = {mk(1), mk(1), mk(3)};
      auto it = s.emplace_hint(s.end(), mk(1));
      assert(it == nth(s, 2));
      assert(equal(s.tree(), seq(mk(1), mk(1), mk(1), mk(3))));
      assert(equal(s.unordered(), seq(mk(1), mk(1), mk(1), mk(3)))
             || equal(s.unordered(), seq(mk(3), mk(1), mk(1), mk(1))));
      assert(equal(irng(s.tree().equal_range(1)),
                   irng(s.unordered().equal_range(1)),
                   [](auto x, auto y) {return x == S::no_ed_tree_iter(y);}));
    }
  }

  // insert(x)
  // insert(hint, x)
  {
    S s;
    auto it = s.insert(mk(1));
    assert(*it == mk(1)
           && equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));

    it = s.insert(s.begin(), mk(1));
    assert(it == s.begin());
    assert(equal(s.tree(), rng(2, mk(1))));
    assert(equal(s.tree(), s.unordered()));
  }

  // insert(from, to)
  // insert(il)
  // insert(r)
  // insert_range(r)
  {
    S s;
    const auto r = seq(mk(1), mk(2));
    s.insert(begin(r), end(r));
    s.insert({mk(0), mk(2), mk(3)});
    s.insert(seq(mk(2), mk(4)));
    s.insert_range(seq(mk(2), mk(5), mk(4)));
    assert(equal(s.tree(), bind_rng(seq(0, 1, 2, 2, 2, 2, 3, 4, 4, 5), mk)));
  }

  // extract(cit)
  // extract(key)
  // insert(nh)
  // insert(hint, nh)
  {
    S s = {mk(1)};
    typename S::node_type n = s.extract(s.begin());
    assert(!n.empty());
    s = {mk(1), mk(1)};
    typename S::node_type n2 = s.extract(key(mk(1)));
    assert(!n2.empty());
    {
      typename S::node_type n3 = s.extract(key(mk(1)));
      assert(!n3.empty());
      n3 = s.extract(key(mk(1)));
      assert(n3.empty());
    }

    typename S::iterator it = s.insert(move(n));
    assert(it == s.begin()
           && equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
    assert(n.empty());
    it = s.insert(move(n));
    assert(it == s.end()
           && equal(s, single_rng(mk(1))) && equal(s.tree(), s.unordered()));
    assert(n.empty());
    it = s.insert(s.begin(), move(n2));
    assert(it == s.begin()
           && equal(s, rng(2, mk(1))) && equal(s.tree(), s.unordered()));
    assert(n2.empty());
    it = s.insert(s.begin(), move(n2));
    assert(it == s.end()
           && equal(s, rng(2, mk(1))) && equal(s.tree(), s.unordered()));
  }

  // erase
  // clear()
  {
    // erase(cit)
    {
      S s = {mk(1)};
      assert(s.erase(s.begin()) == s.end());
      assert(s.empty());
      assert(s.tree().empty());
      assert(s.unordered().empty());
    }
    // erase(cit, cit)
    {
      S s = {mk(1), mk(2), mk(3), mk(4)};
      const auto it = nth(s, 3);
      assert(s.erase(nth(s, 1), it) == it);
      assert(equal(s, seq(mk(1), mk(4))));
      assert(equal(s.unordered(), seq(mk(1), mk(4)))
             || equal(s.unordered(), seq(mk(4), mk(1))));
    }
  }

  // erase(key)
  // remove
  // remove_if
  {
    // erase(key)
    {
      S s = {mk(1)};
      assert(s.erase(1) == 1u);
      assert(s.empty() && s.unordered().empty());
      s = {mk(1), mk(1), mk(1)};
      assert(s.erase(1) == 3u);
      assert(s.empty() && s.unordered().empty());
    }
    // remove(key)
    {
      S s = {mk(1)};
      assert(s.remove(1) == 1u);
      assert(s.empty() && s.unordered().empty());
      s = {mk(1), mk(1), mk(1)};
      assert(s.remove(1) == 3u);
      assert(s.empty() && s.unordered().empty());
    }
    // remove_if(eq)
    {
      S s;
      s.insert(mk(1));
      s.insert(mk(2));
      assert(s.remove_if([=](const auto &x) {return key(x) < 3;}) == 2u);
      assert(s.empty() && s.unordered().empty());
    }
  }

  // merge
  {
    {
      S s1 = {mk(1), mk(2), mk(3)};
      S s2({mk(2), mk(3), mk(4)}, s1.get_allocator());
      const auto it = s2.begin();
      const auto it2 = next(it);
      s1.merge(s2);
      assert(equal(s1.tree(), seq(mk(1), mk(2), mk(2), mk(3), mk(3), mk(4))));
      assert(it == nth(s1, 2) && it2 == nth(s1, 4));
      assert(is_permutation(s1.tree(), s1.unordered()));
      assert(s2.empty());
    }
    {
      S s1 = {mk(1), mk(2), mk(3)};
      S s2({mk(2), mk(3), mk(4)}, s1.get_allocator());
      const auto it = s2.begin();
      const auto it2 = next(it);
      s1.merge(move(s2));
      assert(equal(s1.tree(), seq(mk(1), mk(2), mk(2), mk(3), mk(3), mk(4))));
      assert(it == nth(s1, 2) && it2 == nth(s1, 4));
      assert(is_permutation(s1.tree(), s1.unordered()));
      assert(s2.empty());
    }
  }

  // find(key)
  // contains(key)
  // count(key)
  {
    S s = {mk(1), mk(2), mk(2), mk(3)};
    assert(s.find(4) == s.end());
    assert(as_const(s).find(4) == s.end());
    const typename S::iterator it = s.find(2);
    const typename S::const_iterator it2 = as_const(s).find(2);
    assert(it == nth(s, 2) && *it == mk(2));
    assert(it2 == nth(s, 2) && *it2 == mk(2));

    assert(!s.contains(4));
    assert(s.contains(2));
    assert(!as_const(s).contains(4));
    assert(as_const(s).contains(2));

    assert(s.count(4) == 0u);
    assert(s.count(2) == 2u);
    assert(s.count(1) == 1u);
    assert(as_const(s).count(4) == 0u);
    assert(as_const(s).count(2) == 2u);
    assert(as_const(s).count(1) == 1u);
  }

  // lower_bound(key)
  // upper_bound(key)
  // equal_range(key)
  // find_range(min, max)
  {
    S s = {mk(1), mk(2), mk(2), mk(3), mk(3), mk(3), mk(4)};

    assert(s.lower_bound(3) == s.tree().lower_bound(3));
    assert(s.upper_bound(3) == s.tree().upper_bound(3));
    assert(s.equal_range(3) == s.tree().equal_range(3));
    assert(s.lower_bound(2) == s.tree().lower_bound(2));
    assert(s.upper_bound(2) == s.tree().upper_bound(2));
    assert(s.equal_range(2) == s.tree().equal_range(2));
    assert(s.find_range(2, 3) == iter_pair(nth(s, 1), nth(s, 6)));

    assert(as_const(s).lower_bound(3) == as_const(s).tree().lower_bound(3));
    assert(as_const(s).upper_bound(3) == as_const(s).tree().upper_bound(3));
    assert(as_const(s).equal_range(3) == as_const(s).tree().equal_range(3));
    assert(as_const(s).lower_bound(2) == as_const(s).tree().lower_bound(2));
    assert(as_const(s).upper_bound(2) == as_const(s).tree().upper_bound(2));
    assert(as_const(s).equal_range(2) == as_const(s).tree().equal_range(2));
    assert(as_const(s).find_range(2, 3) == iter_pair(nth(s, 1), nth(s, 6)));
  }

  // partition_point(eql)
  {
    S s = {mk(2), mk(4), mk(6), mk(7), mk(9)};
    const auto it = s.partition_point([=](auto x) {return key(x) % 2 == 0;});
    assert(*it == mk(7));
  }

  // new_node
  // delete_node
  {
    S s;
    typename S::iterator it = s.new_node(mk(1));
    assert(*it == mk(1));
    s.delete_node(it);
  }

  // unlink
  // unlink_key
  // link
  {
    // unlink(cit)
    {
      S s = {mk(1)};
      auto it = s.begin();
      assert(s.unlink(s.begin()) == s.end());
      s.delete_node(it);
    }
    // unlink(cit, cit)
    {
      S s = {mk(1), mk(2)};
      const auto it = nth(s, 0);
      const auto it2 = nth(s, 1);
      assert(s.unlink(s.begin(), s.end()) == s.end());
      s.delete_node(it);
      s.delete_node(it2);
    }
    // unlink()
    {
      S s = {mk(1), mk(2)};
      const auto it = nth(s, 0);
      const auto it2 = nth(s, 1);
      s.unlink();
      assert(s.empty() && s.unordered().empty());
      s.delete_node(it);
      s.delete_node(it2);
    }
    // unlink_key(key)
    {
      S s = {mk(1), mk(2), mk(2), mk(3)};
      const auto it1 = nth(s, 0);
      const auto it2 = nth(s, 1);
      const auto it3 = nth(s, 2);
      assert(s.unlink_key(0) == 0u);
      assert(s.unlink_key(4) == 0u);
      assert(s.unlink_key(1) == 1u);
      assert(s.unlink_key(2) == 2u);
      s.delete_node(it1);
      s.delete_node(it2);
      s.delete_node(it3);
    }
  }

  // try_link(key, get_node)
  // try_link_hint(hint, key, get_node)
  {
    S s;
    auto it = s.try_link(1, [&]() {return s.new_node(mk(1));});
    assert(*it == mk(1) && it == s.begin());
    it = s.try_link(1, [&]() {return s.new_node(mk(1));});
    assert(*it == mk(1) && it == prev(s.end()));
    assert(equal(s, rng(2, mk(1))) && equal(s.tree(), s.unordered()));

    it = s.try_link_hint(s.begin(), 1, [&]() {return s.new_node(mk(1));});
    assert(it == s.begin() && *it == mk(1));
    assert(equal(s, rng(3, mk(1))) && equal(s.tree(), s.unordered()));
  }

  // front
  // back
  {
    S s = {mk(1)};
    assert(s.front() == mk(1));
    assert(as_const(s).front() == mk(1));
    assert(s.back() == mk(1));
    assert(as_const(s).back() == mk(1));
  }

  // root
  // leftmost
  // rightmost
  {
    S s;
    assert(s.root() == nullptr);
    assert(s.leftmost() == s.end());
    assert(s.rightmost() == s.end());
    s = {mk(1)};
    assert(s.root() == s.begin());
    assert(s.leftmost() == s.begin());
    assert(s.rightmost() == s.begin());
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
  }

  // erase_or_unlink
  {
    S s = {mk(1)};
    s.erase_or_unlink(s.begin());
    assert(s.empty() && s.unordered().empty());
  }

  // unique
  {
    S s = {mk(1), mk(1), mk(2), mk(2), mk(2), mk(3), mk(4)};
    s.unique();
    assert(equal(s, seq(mk(1), mk(2), mk(3), mk(4)))
           && is_permutation(s.tree(), s.unordered()));
    s = {mk(1), mk(2)};
    s.unique([=](auto x, auto y) {return key(x) + 1 == key(y);});
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
  }

  // hash_function
  // key_eq
  {
    S s;
    typename S::hasher hf = s.hash_function();
    assert(hf(0) == 0);
    typename S::key_equal eq = s.key_eq();
    assert(eq(0, 0) && !eq(0, 1));
  }

  // bucket_count
  // max_bucket_count
  // active_bucket_count
  // bucket_size
  // bucket_range
  // bucket
  // load_factor
  // max_load_factor
  // rehash
  // reserve
  // begin(n)
  // end(n)
  // cbegin(n)
  // cend(n)
  {
    S s;
    assert(s.bucket_count() == 0u);
    assert(s.max_bucket_count() > 0u);
    assert(s.active_bucket_count() == 0u);
    s = {mk(1)};
    assert(s.bucket_size(0u) == 0u);
    assert(empty(s.bucket_range(0u)));
    assert(s.bucket(1) == 1u);
    assert(s.load_factor() < 1.0f);
    assert(s.max_load_factor() == 1.0f);
    s.max_load_factor(2.0f);
    assert(s.max_load_factor() == 2.0f);
    s.rehash(100u);
    assert(equal(s, single_rng(mk(1)))
           && equal(s.unordered(), single_rng(mk(1))));
    assert(s.begin(1u) != s.end(1u));
    assert(as_const(s).begin(1u) != as_const(s).end(1u));
    assert(s.cbegin(1u) != s.cend(1u));
  }

  // append
  {
    S s;
    assert(equal(s.append(mk(1), mk(2)).append(seq(mk(1), mk(3))),
                 seq(mk(1), mk(1), mk(2), mk(3))));
    assert(is_permutation(s.tree(), s.unordered()));
  }
}
void test_mixed_multiset_common_operations() {
  test_mixed_multiset_common_operations_impl
    <mixed_multiset<int, hash<int>, equal_to<int>, less<int>,
                    stateful_test_allocator<int>>>(copy, identity{});
  test_mixed_multiset_common_operations_impl
    <mixed_multimap<int, int, hash<int>, equal_to<int>, less<int>,
                    stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1), at<0>);
}

template <class S>
void test_mixed_map_specialized_operations_impl() {
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
void test_mixed_map_specialized_operations() {
  test_mixed_map_specialized_operations_impl
    <mixed_map<int, int, hash<int>, equal_to<int>, less<int>,
               stateful_test_allocator<pair<int, int>>>>();
  test_mixed_map_specialized_operations_impl
    <mixed_map<int, int, hash<>, equal_to<>, less<>,
               stateful_test_allocator<pair<int, int>>>>();
}

template <class S, class F>
void test_mixed_container_nth_impl(F mk) {
  S s(bind_rng(irng(0, 100), mk));
  for (int i : irng(0, 100)) {
    assert(*s.nth(i) == mk(i));
    assert(*as_const(s).nth(i) == mk(i));
    assert(s.nth(s.nth(i)) == i);
    assert(s.nth(i).nth() == i);
  }
}
void test_mixed_container_nth() {
  test_mixed_container_nth_impl
    <mixed_ranked_set<int, hash<int>, equal_to<int>, less<int>,
                      stateful_test_allocator<int>>>
    (copy);
  test_mixed_container_nth_impl
    <mixed_ranked_multiset<int, hash<int>, equal_to<int>, less<int>,
                           stateful_test_allocator<int>>>
    (copy);
  test_mixed_container_nth_impl
    <mixed_ranked_map<int, int, hash<int>, equal_to<int>, less<int>,
                      stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1));
  test_mixed_container_nth_impl
    <mixed_ranked_multimap<int, int, hash<int>, equal_to<int>, less<int>,
                           stateful_test_allocator<pair<int, int>>>>
    (bind(make_pair, _1, _1));
}

void test_mixed_map() {
  printf("mixed_map: ");

  inner::test::test_mixed_set_common_operations();
  inner::test::test_mixed_multiset_common_operations();
  inner::test::test_mixed_map_specialized_operations();
  inner::test::test_mixed_container_nth();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_mixed_map();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
