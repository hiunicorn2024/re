#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>

#include <cassert>

namespace re::inner::test {

void test_aligned_stride() {
  auto v = aligned_stride(irng(1, 7), 2, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 3, 5}));
  v = aligned_stride(irng(1, 7), 6, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1}));
  v = aligned_stride(irng(1, 1), 6, to_back(ez_vector<int>{})).base();
  assert(equal(v, empty_rng<int>()));
}
void test_aligned_stride_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = aligned_stride_iterator<int *>;
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = aligned_stride_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = aligned_stride_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = aligned_stride_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = aligned_stride_iterator<ez_vector<int>::iterator>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = aligned_stride_iterator<int *>;
      using c_iter_t = aligned_stride_iterator<const int *>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<iter_t, iter_t>);
      static_assert(three_way_comparable_with<iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // construct from (i, d)
  {
    using iter_t = aligned_stride_iterator<int *>;
    iter_t i;
    assert(i.base() == nullptr);
    assert(i.stride() == 1);
    assert(i - i == 0);
    int x{};
    iter_t ii(addressof(x), 3);
    assert(ii.base() == addressof(x));
    assert(ii.stride() == 3);
  }
  // iterator operations
  {
    {
      int a[] = {1, 2, 3, 4, 5, 6};
      {
        using iter_t = aligned_stride_iterator<int *>;
        using c_iter_t = aligned_stride_iterator<const int *>;
        test_ritr(iter_t(begin(a), 1), iter_t(end(a), 1), irng(1, 7));
        test_ritr(c_iter_t(begin(a), 1), iter_t(end(a), 1), irng(1, 7));
        test_ritr(iter_t(begin(a), 1), c_iter_t(end(a), 1), irng(1, 7));
        test_ritr(c_iter_t(begin(a), 1), c_iter_t(end(a), 1), irng(1, 7));
      }
      {
        using iter_t = aligned_stride_iterator<int *>;
        test_ritr(iter_t(begin(a), 2), iter_t(end(a), 2), ez_vector{1, 3, 5});
      }
      {
        using iter_t = aligned_stride_iterator<int *>;
        test_ritr(iter_t(begin(a), 6), iter_t(end(a), 6), ez_vector{1});
      }
    }
    {
      {
        ez_forward_list<int> a = {};
        using iter_t = aligned_stride_iterator<rng_itr<decltype(a)>>;
        test_fitr(iter_t(begin(a), 3), iter_t(end(a), 3), rng(0, 0));
      }
      {
        ez_forward_list<int> a = {1, 2, 3, 4};
        {
          using iter_t = aligned_stride_iterator<rng_itr<decltype(a)>>;
          test_fitr(iter_t(begin(a), 1), iter_t(end(a), 1), irng(1, 5));
        }
        {
          using iter_t = aligned_stride_iterator<rng_itr<decltype(a)>>;
          test_fitr(iter_t(begin(a), 2), iter_t(end(a), 2), ez_vector{1, 3});
        }
      }
    }
  }
  // special member functions
  {
    using iter_t = aligned_stride_iterator<int *>;
    int x{};
    const auto init_f1 = [&](iter_t *p) {
      new(p) iter_t(addressof(x), 2);
    };
    const auto eq_f1 = [&x](const iter_t *p) {
      return p->base() == addressof(x) && **p == 0 && p->stride() == 2;
    };
    const auto init_f2 = [](iter_t *p) {
      new(p) iter_t();
    };
    const auto eq_f2 = [](const iter_t *p) {
      return p->base() == nullptr && p->stride() == 1;
    };
    test_ownership<iter_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](const iter_t *) {return true;});
  }
  // aligned_stride_itr
  {
    int a[] = {1, 2, 3, 4};
    assert(equal(rng(aligned_stride_itr(begin(a), 2),
                     aligned_stride_itr(end(a), 2)),
                 ez_vector{1, 3}));
  }
}
void test_aligned_stride_range() {
  // compile-time info
  {
    using r1_t = aligned_stride_range<ez_vector<int>>;
    using r2_t = aligned_stride_range<ez_vector<int> &>;
    using r3_t = aligned_stride_range
      <iter_pair<ez_forward_list<int>::iterator>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);

      static_assert(is_default_constructible<r1_t>
                    && is_nothrow_default_constructible<r1_t>);
      static_assert(!is_default_constructible<r2_t>);
      static_assert(is_default_constructible<r3_t>
                    && is_nothrow_default_constructible<r3_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(!copyable<r2_t>);
      static_assert(copyable<r3_t> && is_nothrow_copyable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r1_t> && is_rng<const r1_t>);
      static_assert(is_rng<r2_t> && is_rng<const r2_t>);
      static_assert(is_rng<r3_t> && is_rng<const r3_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t> && !is_rng_ref<const r1_t>);
      static_assert(is_rng_ref<r2_t> && is_rng_ref<const r2_t>);
      static_assert(is_rng_ref<r3_t> && is_rng_ref<const r3_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t> && rng_is_sized<const r1_t>);
      static_assert(rng_is_sized<r2_t> && rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t> && !rng_is_sized<const r3_t>);
    }
  }
  // special member functions
  {
    using r_t = aligned_stride_range<ez_vector<int>>;

    const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
    const auto eq_f1 = [](r_t *p) {return equal(*p, ez_vector{1, 3});};
    const auto init_f2 = [&](r_t *p) {new(p) r_t;};
    const auto eq_f2 = [&](r_t *p) {
      return p->empty() && p->size() == 0 && p->begin() == p->end()
        && cbegin(*p) == cend(*p);
    };
    test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                        bind(empty, bind(deref, _1)));
  }
  // construct from (r)
  // range operations
  {
    {
      using r_t = aligned_stride_range<ez_vector<int>>;
      r_t r({1, 2, 3, 4}, 1);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 4 && as_const(r).size() == 4);
      assert(equal(r, irng(1, 5)));
      assert(equal(as_const(r), irng(1, 5)));
    }
    {
      using r_t = aligned_stride_range<ez_vector<int>>;
      r_t r({1, 2, 3}, 3);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 1 && as_const(r).size() == 1);
      assert(equal(r, irng(1, 2)));
      assert(equal(as_const(r), irng(1, 2)));
    }
    {
      using r_t = aligned_stride_range<ez_vector<int>>;
      r_t r({1, 2, 3, 4, 5, 6}, 3);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 2 && as_const(r).size() == 2);
      assert(equal(r, ez_vector{1, 4}));
      assert(equal(as_const(r), ez_vector{1, 4}));
    }
  }
  // aligned_stride_rng
  {
    ez_vector v = {1, 2, 3, 4};
    auto r = aligned_stride_rng(v, 2);
    v = {1, 2};
    assert(equal(r, irng(1, 2)));
  }
}
void test_stride() {
  auto v = stride(irng(1, 10), 3, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 4, 7}));
  v = stride(irng(1, 10), 4, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 5, 9}));
  v = stride(irng(1, 10), 5, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 6}));
  v = stride(irng(1, 10), 2, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 3, 5, 7, 9}));
  v = stride(irng(1, 10), 1, to_back(ez_vector<int>{})).base();
  assert(equal(v, irng(1, 10)));
  v = stride(irng(1, 20), 6, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 7, 13, 19}));
  v = stride(irng(1, 20), 5, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 6, 11, 16}));
  v = stride(irng(1, 20), 3, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 4, 7, 10, 13, 16, 19}));
  v = stride(irng(1, 20), 9, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 10, 19}));
  v = stride(irng(1, 20), 12, to_back(ez_vector<int>{})).base();
  assert(equal(v, ez_vector{1, 13}));
}
void test_stride_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = stride_iterator<int *>;
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = stride_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = stride_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = stride_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = stride_iterator<ez_vector<int>::iterator>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = stride_iterator<int *>;
      using c_iter_t = stride_iterator<const int *>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<iter_t, iter_t>);
      static_assert(three_way_comparable_with<iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // construct from (i, ed, s, missing)
  // iterator operations
  {
    ez_vector<int> v, vv;
    ez_list<int> l;
    ez_forward_list<int> l2;
    for (int &i : iters(0, 15))
      for (int &j : iters(1, 30)) {
        v.erase(v.begin(), v.end());
        vv.erase(vv.begin(), vv.end());
        copy(irng(0, i), to_back(v));
        stride(v, j, to_back(vv));
        using iter_t = stride_iterator<int *>;
        auto r = rng(iter_t(v.begin(), v.end(), j),
                     iter_t(v.end(), v.end(), j,
                            (j - ssize(v) % j) % j));
        test_ritr(r, vv);
        test_ritr(begin(r), cend(r), vv);
        test_ritr(cbegin(r), end(r), vv);
        test_ritr(as_const(r), vv);

        l.erase(l.begin(), l.end());
        copy(v, to_back(l));
        using iter2_t = stride_iterator<ez_list<int>::iterator>;
        auto rr = rng(iter2_t(l.begin(), l.end(), j),
                      iter2_t(l.end(), l.end(), j,
                              (j - ssize(l) % j) % j));
        test_bitr(rr, vv);
        test_bitr(begin(rr), cend(rr), vv);
        test_bitr(cbegin(rr), end(rr), vv);
        test_bitr(as_const(rr), vv);

        l2.erase_after(l2.before_begin(), l2.end());
        copy(v, to_front(l2));
        using iter3_t = stride_iterator<ez_forward_list<int>::iterator>;
        auto rrr = rng(iter3_t(l2.begin(), l2.end(), j),
                       iter3_t(l2.end(), l2.end(), j));
        test_fitr(rrr, vv);
        test_fitr(begin(rrr), cend(rrr), vv);
        test_fitr(cbegin(rrr), end(rrr), vv);
        test_fitr(as_const(rrr), vv);
      }
  }
  // special member functions
  {
    using iter_t = stride_iterator<int *>;
    using t = iter_pair<iter_t>;
    int a[] = {1, 2, 3, 4, 5};
    const auto init_f1 = [&](t *p) {
      new(p) t(iter_t(begin(a), end(a), 2),
               iter_t(end(a), end(a), 2, 1));
    };
    const auto eq_f1 = [&](const t *p) {
      return equal(*p, ez_vector{1, 3, 5})
        && equal(rrng(*p), ez_vector{5, 3, 1});
    };
    const auto init_f2 = [](t *p) {
      new(p) t(iter_t{}, iter_t{});
    };
    const auto eq_f2 = [](const t *p) {
      return p->empty();
    };
    test_ownership<t>(init_f1, eq_f1, init_f2, eq_f2, [](auto) {return true;});
  }
  // stride_itr
  {
    int a[] = {1, 2, 3, 4, 5};
    assert(equal(rng(stride_itr(begin(a), end(a), 2, 0),
                     stride_itr(end(a), end(a), 2, 1)), ez_vector{1, 3, 5}));
    assert(equal(rng(stride_itr(begin(a), end(a), 2),
                     stride_itr(end(a), end(a), 2, 1)), ez_vector{1, 3, 5}));
  }
}
void test_stride_range() {
  // compile-time info
  {
    using r1_t = stride_range<ez_forward_list<int>>;
    using r2_t = stride_range<iter_pair<ez_forward_list<int>::iterator>>;
    using r3_t = stride_range<ez_forward_list<int> &>;
    using r4_t = stride_range<ez_bidirectional_list<int>>;
    using r5_t = stride_range<iter_pair<ez_bidirectional_list<int>::iterator>>;
    using r6_t = stride_range<ez_list<int>>;
    using r7_t = stride_range<iter_pair<ez_list<int>::iterator>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
      static_assert(semiregular<r6_t>);
      static_assert(semiregular<r7_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!is_nothrow_copyable<r1_t>);

      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(is_nothrow_copyable<r2_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && is_frng<const r1_t>);
      static_assert(is_frng<r2_t> && is_frng<const r2_t>);
      static_assert(is_frng<r3_t> && is_frng<const r3_t>);
      static_assert(is_brng<r4_t> && !is_rng<const r4_t>);
      static_assert(is_brng<r5_t> && is_brng<const r5_t>);
      static_assert(is_brng<r6_t> && !is_rng<const r6_t>);
      static_assert(is_brng<r7_t> && is_brng<const r7_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t> && !is_rng_ref<const r1_t>);
      static_assert(is_rng_ref<r2_t> && is_rng_ref<const r2_t>);
      static_assert(is_rng_ref<r3_t> && is_rng_ref<const r3_t>);
      static_assert(!is_rng_ref<r4_t> && !is_rng_ref<const r4_t>);
      static_assert(is_rng_ref<r5_t> && is_rng_ref<const r5_t>);
      static_assert(!is_rng_ref<r6_t> && !is_rng_ref<const r6_t>);
      static_assert(is_rng_ref<r7_t> && is_rng_ref<const r7_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t> && !rng_is_sized<const r1_t>);
      static_assert(!rng_is_sized<r2_t> && !rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t> && !rng_is_sized<const r3_t>);
      static_assert(!rng_is_sized<r4_t> && !rng_is_sized<const r4_t>);
      static_assert(!rng_is_sized<r5_t> && !rng_is_sized<const r5_t>);
      static_assert(rng_is_sized<r6_t>);
      static_assert(!rng_is_sized<r7_t> && !rng_is_sized<const r7_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = stride_range<ez_vector<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, ez_vector{1, 3});};
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = stride_range<ez_forward_list<int>>;
      static_assert(!rng_is_sized<r_t>);
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, ez_vector{1, 3});};
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
  }
  // construct from (r, s)
  // range operations
  {
    ez_vector<int> v_cmp;
    ez_vector<int> l1;
    ez_list<int> l2;
    ez_bidirectional_list<int> l3;
    ez_forward_list<int> l4;
    for (auto &i : iters(0, 20))
      for (auto &j : iters(1, 40)) {
        v_cmp.erase(v_cmp.begin(), v_cmp.end());
        l1.erase(l1.begin(), l1.end());
        copy(irng(0, i), to_back(l1));
        assert(equal(l1, irng(0, i)));
        stride(l1, j, to_back(v_cmp));
        {
          auto r1 = stride_rng(l1, j);
          test_ritr(r1, v_cmp);
          test_ritr(begin(r1), cend(r1), v_cmp);
          test_ritr(cbegin(r1), end(r1), v_cmp);
          test_ritr(as_const(r1), v_cmp);
          assert(r1.size() == v_cmp.size());
          assert(as_const(r1).size() == v_cmp.size());
          assert(r1.empty() == v_cmp.empty());
          assert(as_const(r1).empty() == v_cmp.empty());

          auto r2 = stride_rng(copy(l1), j);
          test_ritr(r2, v_cmp);
          static_assert(!is_rng<const decltype(r2)>);
          assert(r2.size() == v_cmp.size());
          assert(r2.empty() == v_cmp.empty());
        }
        {
          l2.erase(l2.begin(), l2.end());
          copy(l1, to_back(l2));
          assert(equal(l2, irng(0, i)));
          auto r = stride_rng(l2, j);
          test_bitr(r, v_cmp);
        }
        {
          l3.erase(l3.begin(), l3.end());
          copy(l1, to_back(l3));
          assert(equal(l3, irng(0, i)));
          auto r = stride_rng(l3, j);
          test_fitr(r, v_cmp);
        }
        {
          l4.erase_after(l4.before_begin(), l4.end());
          copy(l1, to_front(l4));
          assert(equal(l4, irng(0, i)));
          auto r = stride_rng(l4, j);
          test_fitr(r, v_cmp);
        }
      }
  }
  // stride_rng
  {
    assert(equal(stride_rng(ez_vector{1, 2, 3, 4}, 2), ez_vector{1, 3}));
  }
}
void test_aligned_chunk() {
  {
    ez_vector<int> v0 = {1, 2, 3, 4};
    auto v = aligned_chunk(v0, 2,
                           to_back(ez_vector<iter_pair<int *>>{})).base();
    assert(equal(v, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}}, equal));
  }
  {
    ez_vector<int> v0 = {1, 2, 3, 4};
    auto v = aligned_chunk(v0, 4,
                           to_back(ez_vector<iter_pair<int *>>{})).base();
    assert(equal(v, ez_vector<ez_vector<int>>{{1, 2, 3, 4}}, equal));
  }
  {
    ez_vector<int> v0 = {1, 2, 3, 4};
    auto v = aligned_chunk(v0, 1,
                           to_back(ez_vector<iter_pair<int *>>{})).base();
    assert(equal(v, ez_vector<ez_vector<int>>{{1}, {2}, {3}, {4}}, equal));
  }
  {
    ez_vector<int> v0 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto v = aligned_chunk(v0, 3,
                           to_back(ez_vector<iter_pair<int *>>{})).base();
    assert(equal(v, ez_vector<ez_vector<int>>
                 ({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}}),
                 equal));
  }
  {
    ez_vector<int> v0;
    auto v = aligned_chunk(v0, 3,
                           to_back(ez_vector<iter_pair<int *>>{})).base();
    assert(equal(v, ez_vector<ez_vector<int>>{}, equal));
  }
}
void test_aligned_chunk_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = aligned_chunk_iterator<int *>;
        static_assert(same_as<iter_t::value_type, iter_pair<int *>>);
        static_assert(same_as<iter_t::reference, iter_pair<int *>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
      {
        using iter_t = aligned_chunk_iterator<ez_forward_list<int>::iterator>;
        static_assert(same_as
                      <iter_t::value_type,
                       iter_pair
                       <iter_n_iterator<ez_forward_list<int>::iterator>>>);
        static_assert(same_as<iter_t::reference, iter_t::value_type>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      using iter_t = aligned_chunk_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = aligned_chunk_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = aligned_chunk_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = aligned_chunk_iterator<ez_vector<int>::iterator>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = aligned_chunk_iterator<int *>;
      using c_iter_t = aligned_chunk_iterator<const int *>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<iter_t, iter_t>);
      static_assert(three_way_comparable_with<iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // construct from (i, i2, s)
  // construct const iterator from iterator
  {
    using iter_t = aligned_chunk_iterator<int *>;
    iter_t i;
    assert(i.base() == nullptr);
    assert(i.stride() == 1);
    assert(i - i == 0);
    int x{};
    iter_t ii(addressof(x), 3);
    assert(ii.base() == addressof(x));
    assert(ii.stride() == 3);

    using c_iter_t = aligned_chunk_iterator<const int *>;
    c_iter_t j = ii;
    assert(j.base() == addressof(x));
    assert(j.stride() == 3);
  }
  // iterator operations
  {
    {
      int a[] = {1, 2, 3, 4, 5, 6};
      {
        using iter_t = aligned_chunk_iterator<int *>;
        using c_iter_t = aligned_chunk_iterator<const int *>;
        const auto r = bind_rng(irng(1, 7), [](int x) {return irng(x, x + 1);});
        test_ritr(iter_t(begin(a), 1), iter_t(end(a), 1), r, equal);
        test_ritr(iter_t(begin(a), 1), c_iter_t(end(a), 1), r, equal);
        test_ritr(c_iter_t(begin(a), 1), iter_t(end(a), 1), r, equal);
        test_ritr(c_iter_t(begin(a), 1), c_iter_t(end(a), 1), r, equal);
      }
      {
        using iter_t = aligned_chunk_iterator<int *>;
        test_ritr(iter_t(begin(a), 2), iter_t(end(a), 2),
                  ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5, 6}},
                  equal);
      }
      {
        using iter_t = aligned_chunk_iterator<int *>;
        test_ritr(iter_t(begin(a), 6), iter_t(end(a), 6),
                  ez_vector<ez_vector<int>>{{1, 2, 3, 4, 5, 6}}, equal);
      }
    }
    {
      {
        ez_forward_list<int> a = {};
        using iter_t = aligned_chunk_iterator<rng_itr<decltype(a)>>;
        assert(iter_t(begin(a), 3) == iter_t(end(a), 3));
      }
      {
        ez_list<int> a = {1, 2, 3, 4};
        {
          using iter_t = aligned_chunk_iterator<rng_itr<decltype(a)>>;
          test_bitr(iter_t(begin(a), 1), iter_t(end(a), 1),
                    bind_rng(irng(1, 5), [](int x) {return irng(x, x + 1);}),
                    equal);
        }
        {
          using iter_t = aligned_chunk_iterator<rng_itr<decltype(a)>>;
          test_bitr(iter_t(begin(a), 2), iter_t(end(a), 2),
                    ez_vector<ez_vector<int>>{{1, 2}, {3, 4}},
                    equal);
        }
      }
      {
        ez_forward_list<int> a = {1, 2, 3, 4};
        {
          using iter_t = aligned_chunk_iterator<rng_itr<decltype(a)>>;
          test_fitr(iter_t(begin(a), 1), iter_t(end(a), 1),
                    bind_rng(irng(1, 5), [](int x) {return irng(x, x + 1);}),
                    equal);
        }
        {
          using iter_t = aligned_chunk_iterator<rng_itr<decltype(a)>>;
          test_fitr(iter_t(begin(a), 2), iter_t(end(a), 2),
                    ez_vector<ez_vector<int>>{{1, 2}, {3, 4}},
                    equal);
        }
      }
    }
  }
  // special member functions
  {
    using iter_t = aligned_chunk_iterator<int *>;
    int x{};
    const auto init_f1 = [&](iter_t *p) {
      new(p) iter_t(addressof(x), 2);
    };
    const auto eq_f1 = [&x](const iter_t *p) {
      return p->base() == addressof(x) && p->stride() == 2;
    };
    const auto init_f2 = [](iter_t *p) {
      new(p) iter_t();
    };
    const auto eq_f2 = [](const iter_t *p) {
      return p->base() == nullptr && p->stride() == 1;
    };
    test_ownership<iter_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](const iter_t *) {return true;});
  }
  // aligned_chunk_itr
  {
    int a[] = {1, 2, 3, 4};
    assert(equal(rng(aligned_chunk_itr(begin(a), 2),
                     aligned_chunk_itr(end(a), 2)),
                 ez_vector<ez_vector<int>>{{1, 2}, {3, 4}}, equal));
  }
}
void test_aligned_chunk_range() {
  // compile-time info
  {
    using r1_t = aligned_chunk_range<ez_vector<int>>;
    using r2_t = aligned_chunk_range<ez_vector<int> &>;
    using r3_t = aligned_chunk_range
      <iter_pair<ez_forward_list<int>::iterator>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);

      static_assert(is_default_constructible<r1_t>
                    && is_nothrow_default_constructible<r1_t>);
      static_assert(!is_default_constructible<r2_t>);
      static_assert(is_default_constructible<r3_t>
                    && is_nothrow_default_constructible<r3_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(!copyable<r2_t>);
      static_assert(copyable<r3_t> && is_nothrow_copyable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r1_t> && is_rng<const r1_t>);
      static_assert(is_rng<r2_t> && is_rng<const r2_t>);
      static_assert(is_rng<r3_t> && is_rng<const r3_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t> && !is_rng_ref<const r1_t>);
      static_assert(is_rng_ref<r2_t> && is_rng_ref<const r2_t>);
      static_assert(is_rng_ref<r3_t> && is_rng_ref<const r3_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t> && rng_is_sized<const r1_t>);
      static_assert(rng_is_sized<r2_t> && rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t> && !rng_is_sized<const r3_t>);
      using r4_t = aligned_chunk_range<ez_list<int>>;
      static_assert(rng_is_sized<r4_t> && rng_is_sized<r4_t>);
    }
  }
  // special member functions
  {
    using r_t = aligned_chunk_range<ez_vector<int>>;

    const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
    const auto eq_f1 = [](r_t *p) {
      return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}}, equal);
    };
    const auto init_f2 = [&](r_t *p) {new(p) r_t;};
    const auto eq_f2 = [&](r_t *p) {
      return p->empty() && p->size() == 0 && p->begin() == p->end()
        && cbegin(*p) == cend(*p);
    };
    test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                        bind(empty, bind(deref, _1)));
  }
  // construct from (r)
  // range operations
  {
    {
      using r_t = aligned_chunk_range<ez_vector<int>>;
      r_t r({1, 2, 3, 4}, 1);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 4 && as_const(r).size() == 4);
      assert(equal(r,
                   bind_rng(irng(1, 5),
                            [](int x) {return irng(x, x + 1);}),
                   equal));
      assert(equal(as_const(r),
                   bind_rng(irng(1, 5),
                            [](int x) {return irng(x, x + 1);}),
                   equal));
    }
    {
      using r_t = aligned_chunk_range<ez_vector<int>>;
      r_t r({1, 2, 3}, 3);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 1 && as_const(r).size() == 1);
      assert(equal(r, ez_vector<ez_vector<int>>{{1, 2, 3}}, equal));
      assert(equal(as_const(r), ez_vector<ez_vector<int>>{{1, 2, 3}}, equal));
    }
    {
      using r_t = aligned_chunk_range<ez_vector<int>>;
      r_t r({1, 2, 3, 4, 5, 6}, 3);
      assert(!r.empty() && !as_const(r).empty());
      assert(r.size() == 2 && as_const(r).size() == 2);
      assert(equal(r, ez_vector<ez_vector<int>>{{1, 2, 3}, {4, 5, 6}}, equal));
      assert(equal(as_const(r),
                   ez_vector<ez_vector<int>>{{1, 2, 3}, {4, 5, 6}}, equal));
    }
  }
  // aligned_chunk_rng
  {
    ez_vector<int> v;
    auto r = aligned_chunk_rng(v, 2);
    v = {1, 2, 3, 4};
    assert(equal(r, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}}, equal));
  }
}
void test_chunk() {
  ez_vector<ez_vector<int>> v1, v2;
  for (int &i : iters(0, 20))
    for (int &j : iters(1, 30)) {
      v1.erase(v1.begin(), v1.end());
      v2.erase(v2.begin(), v2.end());
      stride(irng(0, i), j, output_itr([&](int x) {
        v1.insert(v1.end(), {});
        copy(irng(x, min(x + j, i)), to_back(back(v1)));
      }));
      chunk(irng(0, i), j, output_itr([&](auto x) {
        v2.insert(v2.end(), {});
        copy(x, to_back(back(v2)));
      }));
      assert(v1 == v2);

      v1.erase(v1.begin(), v1.end());
      v2.erase(v2.begin(), v2.end());
      stride(irng(0, i), j, output_itr([&](int x) {
        v1.insert(v1.end(), {});
        copy(irng(x, min(x + j, i)), to_back(back(v1)));
      }));
      assert(chunk_plus(irng(0, i), j, output_itr([&](auto x) {
        v2.insert(v2.end(), {});
        copy(x, to_back(back(v2)));
      })).first == iitr(i));
      assert(v1 == v2);
    }
}
void test_chunk_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = chunk_iterator<int *>;
      static_assert(same_as<iter_t::value_type, iter_pair<int *>>);
      static_assert(same_as<iter_t::reference, iter_pair<int *>>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, void>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = chunk_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = chunk_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = chunk_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = chunk_iterator<ez_vector<int>::iterator>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = chunk_iterator<int *>;
      using c_iter_t = chunk_iterator<const int *>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<iter_t, iter_t>);
      static_assert(three_way_comparable_with<iter_t, c_iter_t>);
      static_assert(three_way_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // construct from (i, i2, ed, s, missing)
  // iterator operations
  // const iterator
  {
    ez_vector<int> v;
    ez_vector<iter_pair<int *>> vv;
    for (int i : iters(0, 20))
      for (int j : iters(1, 40)) {
        v.erase(v.begin(), v.end());
        vv.erase(vv.begin(), vv.end());
        copy(irng(0, i), to_back(v));
        chunk(v, j, to_back(vv));

        int m1{}, m2{};
        auto it = begin(v);
        if (i != 0) {
          it = next(begin(v), j, end(v));
          m2 = (j - ssize(v) % j) % j;
          if (i < j)
            m1 = m2;
        }
        const auto r
          = rng(chunk_iterator<int *>(begin(v), it, end(v), j, m1),
                chunk_iterator<int *>(end(v), end(v), end(v), j, m2));
        const auto r2 = rng(chunk_iterator<const int *>(r.begin()),
                            chunk_iterator<const int *>(r.end()));
        test_ritr(r, vv);
        test_ritr(r2, vv);
        test_ritr(begin(r), end(r2), vv);
        test_ritr(begin(r2), end(r), vv);
      }
  }
  // special member functions
  {
    using iter_t = chunk_iterator<int *>;
    using pair_t = iter_pair<iter_t>;
    int a[] = {1, 2, 3, 4, 5};
    const auto init_f1 = [&](pair_t *p) {
      new(p) pair_t(iter_t(nth(a, 0), nth(a, 2), end(a), 2, 0),
                    iter_t(end(a), end(a), end(a), 2, 1));
    };
    const auto eq_f1 = [](pair_t *p) {
      return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5}}, equal);
    };
    const auto init_f2 = [&](pair_t *p) {
      new(p) pair_t;
    };
    const auto eq_f2 = [](pair_t *p) {
      return p->begin() == p->end() && p->empty() && p->size() == 0;
    };
    test_ownership<pair_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](auto) {return true;});
  }
  // chunk_itr
  {
    int a[] = {1, 2, 3, 4, 5};
    test_ritr(chunk_itr(nth(a, 0), nth(a, 3), end(a), 3, 0),
              chunk_itr(end(a), end(a), end(a), 3, 1),
              ez_vector<ez_vector<int>>{{1, 2, 3}, {4, 5}},
              equal);
  }
}
void test_chunk_range() {
  // compile-time info
  {
    using r1_t = chunk_range<ez_forward_list<int>>;
    using r2_t = chunk_range<iter_pair<ez_forward_list<int>::iterator>>;
    using r3_t = chunk_range<ez_forward_list<int> &>;
    using r4_t = chunk_range<ez_vector<int>>;
    using r5_t = chunk_range<iter_pair<int *>>;
    using r6_t = chunk_range<ez_vector<int> &>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
      static_assert(!semiregular<r6_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(!is_swappable<r3_t>);
      static_assert(is_nothrow_swappable<r4_t>);
      static_assert(is_nothrow_swappable<r5_t>);
      static_assert(!is_swappable<r6_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(copyable<r2_t> && is_nothrow_copyable<r2_t>);
      static_assert(!copyable<r3_t> && !is_nothrow_copyable<r3_t>);
      static_assert(copyable<r4_t> && !is_nothrow_copyable<r4_t>);
      static_assert(copyable<r5_t> && is_nothrow_copyable<r5_t>);
      static_assert(!copyable<r6_t> && !is_nothrow_copyable<r6_t>);
    }
    // is_rng
    {
      static_assert(is_just_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_just_frng<r2_t> && is_rng<const r2_t>);
      static_assert(is_just_frng<r3_t> && is_rng<const r3_t>);
      static_assert(is_just_rrng<r4_t> && !is_rng<const r4_t>);
      static_assert(is_just_rrng<r5_t> && is_rng<const r5_t>);
      static_assert(is_just_rrng<r6_t> && is_rng<const r6_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
      static_assert(is_rng_ref<r6_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t> && !rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t> && !rng_is_sized<const r3_t>);
      static_assert(rng_is_sized<r4_t>);
      static_assert(rng_is_sized<r5_t> && rng_is_sized<const r5_t>);
      static_assert(rng_is_sized<r6_t> && rng_is_sized<const r6_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = chunk_range<ez_vector<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4, 5}, 2);};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5}}, equal)
          && !p->empty() && p->size() == 3;
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = chunk_range<ez_slist<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4, 5}, 2);};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5}}, equal)
          && !p->empty() && p->size() == 3;
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = chunk_range<ez_forward_list<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4, 5}, 2);};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = chunk_range<iter_pair<ez_forward_list<int>::iterator>>;
      ez_forward_list<int> l = {1, 2, 3, 4, 5};
      const auto init_f1 = [&l](r_t *p) {new(p) r_t(rng(l), 2);};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {3, 4}, {5}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // construct from (r, s)
  // range operations
  {
    ez_vector<ez_vector<int>> v_cmp;
    ez_vector<int> l1;
    ez_list<int> l2;
    ez_bidirectional_list<int> l3;
    ez_forward_list<int> l4;
    for (auto &i : iters(0, 20))
      for (auto &j : iters(1, 40)) {
        v_cmp.erase(v_cmp.begin(), v_cmp.end());
        l1.erase(l1.begin(), l1.end());
        copy(irng(0, i), to_back(l1));
        assert(equal(l1, irng(0, i)));
        chunk(l1, j, output_itr([&v_cmp](const auto &r) {
          v_cmp.insert(v_cmp.end(), {});
          copy(r, to_back(back(v_cmp)));
        }));
        {
          auto r1 = chunk_rng(l1, j);
          test_ritr(begin(r1), end(r1), v_cmp, equal);
          test_ritr(begin(r1), cend(r1), v_cmp, equal);
          test_ritr(cbegin(r1), end(r1), v_cmp, equal);
          test_ritr(cbegin(r1), cend(r1), v_cmp, equal);
          assert(r1.size() == v_cmp.size());
          assert(as_const(r1).size() == v_cmp.size());
          assert(r1.empty() == v_cmp.empty());
          assert(as_const(r1).empty() == v_cmp.empty());

          auto r2 = chunk_rng(copy(l1), j);
          test_ritr(begin(r2), end(r2), v_cmp, equal);
          static_assert(!is_rng<const decltype(r2)>);
          assert(r2.size() == v_cmp.size());
          assert(r2.empty() == v_cmp.empty());
        }
        {
          l2.erase(l2.begin(), l2.end());
          copy(l1, to_back(l2));
          assert(equal(l2, irng(0, i)));
          auto r = chunk_rng(l2, j);
          test_bitr(begin(r), end(r), v_cmp, equal);
        }
        {
          l3.erase(l3.begin(), l3.end());
          copy(l1, to_back(l3));
          assert(equal(l3, irng(0, i)));
          auto r = chunk_rng(l3, j);
          test_fitr(begin(r), end(r), v_cmp, equal);
        }
        {
          l4.erase_after(l4.before_begin(), l4.end());
          copy(l1, to_front(l4));
          assert(equal(l4, irng(0, i)));
          auto r = chunk_rng(l4, j);
          test_fitr(begin(r), end(r), v_cmp, equal);
        }
      }
  }
  // chunk_rng
  {
    assert(equal(chunk_rng(ez_vector<int>{1, 2, 3, 4}, 2),
                 ez_vector<ez_vector<int>>{{1, 2}, {3, 4}}, equal));
    assert(equal(chunk_rng(ez_vector<int>{1, 2, 3, 4}, 3),
                 ez_vector<ez_vector<int>>{{1, 2, 3}, {4}}, equal));
  }
}
void test_chunk_by() {
  // quick
  {
    ez_vector<ez_vector<int>> v;
    const auto to_v = output_itr([&v](const auto &r) {
      v.insert(v.end(), {});
      copy(r, to_back(back(v)));
    });
    chunk_by(ez_vector{1, 2, 3, 1, 1, 2, 3, 3, 4, 5, 4}, less{}, to_v);
    assert(v == ez_vector<ez_vector<int>>
           ({{1, 2, 3}, {1}, {1, 2, 3}, {3, 4, 5}, {4}}));
    v.erase(v.begin(), v.end());
    chunk_by(ez_vector{1}, less{}, to_v);
    assert(v == ez_vector<ez_vector<int>>{{1}});
    v.erase(v.begin(), v.end());
    chunk_by(empty_rng<int>(), less{}, to_v);
    assert(v.empty());
  }
  // exhaustive
  {
    ez_vector<int> v;
    copy(irng(1, 9), to_back(v));

    ez_vector<ez_vector<int>> vv;
    const auto to_vv = output_itr([&vv](const auto &r) {
      vv.insert(vv.end(), {});
      copy(r, to_back(back(vv)));
    });

    ez_vector<int> v2;
    do {
      vv.erase(vv.begin(), vv.end());
      chunk_by(v, less{}, to_vv);

      assert(all_of(vv, [](const auto &x) {
        return !x.empty() && is_sorted(x);
      }));
      adjacent<2>(vv, output_itr([](const auto &t) {
        assert(!(back(t.first) < front(t.second)));
      }));

      v2.erase(v2.begin(), v2.end());
      join(vv, to_back(v2));

      assert(is_permutation(v, v2));
    } while (next_permutation(v));
  }
  // chunk_by_plus
  {
    ez_vector<int> v = {1, 2, 2, 3};
    ez_vector<ez_vector<int>> vv;
    auto [it, o]
      = chunk_by_plus(v, less{}, output_itr([&vv](const auto &r) {
      vv.insert(vv.end(), {});
      copy(r, to_back(back(vv)));
    }));
    assert(it == end(v));
    assert(equal(vv, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}}));
  }
}
void test_chunk_by_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = chunk_by_iterator<int *, less<>>;
      static_assert(same_as<iter_t::value_type, iter_pair<int *>>);
      static_assert(same_as<iter_t::reference, iter_pair<int *>>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, void>);
      static_assert(same_as
                    <iter_t::iterator_category, bidirectional_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = chunk_by_iterator<int *, less<>>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = chunk_by_iterator<ez_slist<int>::iterator, less<>>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = chunk_by_iterator<ez_list<int>::iterator, less<>>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = chunk_by_iterator<int *, less<>>;
      using c_iter_t = chunk_by_iterator<const int *, less<>>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      static_assert(!three_way_comparable_with<iter_t, iter_t>);
      static_assert(!three_way_comparable_with<iter_t, c_iter_t>);
      static_assert(!three_way_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // construct from (i, i2, op, ed, f)
  // iterator operations
  // const iterator
  {
    ez_function<bool (int, int)> f = less{};

    using iter_t = chunk_by_iterator<int *, decltype(f) *>;
    using c_iter_t = chunk_by_iterator<const int *, decltype(f) *>;

    ez_vector<int> v = {1, 1, 2, 2, 3, 3, 4, 5, 6, 7};
    ez_vector<ez_vector<int>> vv;
    do {
      vv.erase(vv.begin(), vv.end());
      chunk_by(v, less{}, output_itr([&vv](const auto &r) {
        vv.insert(vv.end(), {});
        copy(r, to_back(back(vv)));
      }));

      auto it = adjacent_find(v, not_fn(f));
      if (it != end(v))
        ++it;

      const auto r = rng
        (iter_t(begin(v), it, begin(v), end(v), addressof(f)),
         iter_t(end(v), end(v), begin(v), end(v), addressof(f)));
      const auto cr = rng
        (c_iter_t(cbegin(v), static_cast<const int *>(it),
                cbegin(v), cend(v), addressof(f)),
         c_iter_t(cend(v), cend(v), cbegin(v), cend(v), addressof(f)));
      test_bitr(begin(r), end(r), vv, equal);
      test_bitr(begin(cr), end(cr), vv, equal);
      test_bitr(begin(cr), end(r), vv, equal);
      test_bitr(begin(r), end(cr), vv, equal);
    } while (next_permutation(v));
  }
  // special member functions
  {
    using iter_t = chunk_by_iterator<int *, less<>>;
    using c_iter_t = chunk_by_iterator<const int *, less<>>;
    using pair_t = iter_pair<iter_t>;
    int a[] = {1, 2, 2, 3, 1};

    const auto init_f1 = [&](pair_t *p) {
      new(p) pair_t(iter_t(nth(a, 0), nth(a, 2), begin(a), end(a), less{}),
                    iter_t(end(a), end(a), begin(a), end(a), less{}));
    };
    const auto eq_f1 = [](pair_t *p) {
      return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}, {1}}, equal);
    };
    const auto init_f2 = [&](pair_t *p) {
      new(p) pair_t;
    };
    const auto eq_f2 = [](pair_t *p) {
      return p->begin() == p->end() && p->empty();
    };
    test_ownership<pair_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](auto) {return true;});
  }
  // chunk_by_itr
  {
    int a[] = {1, 2, 2, 3};
    assert(equal(rng(chunk_by_itr
                     (begin(a), nth(a, 2), begin(a), end(a), less{}),
                     chunk_by_itr
                     (end(a), end(a), begin(a), end(a), less{})),
                 ez_vector<ez_vector<int>>{{1, 2}, {2, 3}},
                 equal));
  }
}
void test_chunk_by_range() {
  // compile-time info
  {
    using r1_t = chunk_by_range<ez_forward_list<int>, less<>>;
    using r2_t = chunk_by_range<iter_pair<ez_forward_list<int>::iterator>,
                                less<>>;
    using r3_t = chunk_by_range<ez_forward_list<int> &, less<>>;
    using r4_t = chunk_by_range<ez_vector<int>, less<>>;
    using r5_t = chunk_by_range<iter_pair<int *>, less<>>;
    using r6_t = chunk_by_range<ez_vector<int> &, less<>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
      static_assert(!semiregular<r6_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(!is_swappable<r3_t>);
      static_assert(is_nothrow_swappable<r4_t>);
      static_assert(is_nothrow_swappable<r5_t>);
      static_assert(!is_swappable<r6_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(copyable<r2_t> && is_nothrow_copyable<r2_t>);
      static_assert(!copyable<r3_t> && !is_nothrow_copyable<r3_t>);
      static_assert(copyable<r4_t> && !is_nothrow_copyable<r4_t>);
      static_assert(copyable<r5_t> && is_nothrow_copyable<r5_t>);
      static_assert(!copyable<r6_t> && !is_nothrow_copyable<r6_t>);
    }
    // is_rng
    {
      static_assert(is_just_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_just_frng<r2_t> && is_rng<const r2_t>);
      static_assert(is_just_frng<r3_t> && is_rng<const r3_t>);
      static_assert(is_just_brng<r4_t> && !is_rng<const r4_t>);
      static_assert(is_just_brng<r5_t> && is_rng<const r5_t>);
      static_assert(is_just_brng<r6_t> && is_rng<const r6_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
      static_assert(is_rng_ref<r6_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t> && !rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t> && !rng_is_sized<const r3_t>);
      static_assert(!rng_is_sized<r4_t>);
      static_assert(!rng_is_sized<r5_t> && !rng_is_sized<const r5_t>);
      static_assert(!rng_is_sized<r6_t> && !rng_is_sized<const r6_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = chunk_by_range<ez_vector<int>, less<>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 2, 3}, less{});};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = chunk_by_range<ez_slist<int>, less<>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 2, 3}, less{});};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = chunk_by_range<iter_pair<ez_slist<int>::iterator>, less<>>;
      ez_slist<int> l = {1, 2, 2, 3};
      const auto init_f1 = [&l](r_t *p) {new(p) r_t(rng(l), less{});};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // construct from (r, f)
  // range operations
  {
    ez_vector<ez_vector<int>> vv;
    ez_vector<int> v = {1, 1, 2, 2, 3, 4, 5};
    ez_list<int> l1;
    ez_slist<int> l2;
    do {
      vv.erase(vv.begin(), vv.end());
      chunk_by(v, less{}, output_itr([&vv](const auto &r) {
        vv.insert(vv.end(), {});
        copy(r, to_back(back(vv)));
      }));

      l1.erase(l1.begin(), l1.end());
      copy(v, to_back(l1));
      {
        chunk_by_range<decltype(l1), less<>> r(l1, less{});
        assert(equal(r, vv, equal));
        assert(r.empty() == vv.empty());
        test_bitr(begin(r), end(r), vv, equal);
      }
      {
        chunk_by_range<iter_pair<decltype(l1)::iterator>, less<>>
          r(rng(l1), less{});
        assert(equal(r, vv, equal));
        assert(r.empty() == vv.empty());
        assert(as_const(r).empty() == vv.empty());
        test_bitr(begin(r), end(r), vv, equal);
        test_bitr(begin(r), cend(r), vv, equal);
        test_bitr(cbegin(r), end(r), vv, equal);
        test_bitr(cbegin(r), cend(r), vv, equal);
      }

      l2.erase_after(l2.before_begin(), l2.end());
      copy(v, to_front(l2));
      {
        chunk_by_range<decltype(l2), less<>> r(l2, less{});
        assert(equal(r, vv, equal));
        assert(r.empty() == vv.empty());
        test_fitr(begin(r), end(r), vv, equal);
      }
      {
        chunk_by_range<iter_pair<decltype(l2)::iterator>, less<>>
          r(rng(l2), less{});
        assert(equal(r, vv, equal));
        assert(r.empty() == vv.empty());
        assert(as_const(r).empty() == vv.empty());
        test_fitr(begin(r), end(r), vv, equal);
        test_fitr(begin(r), cend(r), vv, equal);
        test_fitr(cbegin(r), end(r), vv, equal);
        test_fitr(cbegin(r), cend(r), vv, equal);
      }
    } while (next_permutation(v));
  }
  // chunk_by_rng
  {
    assert(equal(chunk_by_rng(ez_vector{1, 2, 2, 3}, less{}),
                 ez_vector<ez_vector<int>>{{1, 2}, {2, 3}},
                 equal));
    ez_vector<int> v = {1, 2, 2, 3};
    auto r = chunk_by_rng(v, less{});
    assert(equal(r, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}}, equal));
    assert(equal(bind_rng(join_rng(r), addressof), bind_rng(v, addressof)));
  }
}
void test_inner_cartesian_product() {
  ez_vector<ez_vector<int>> v;
  const auto to_v = output_itr([&](const auto &r) {
    v.insert(v.end(), {});
    copy(r, to_back(back(v)));
  });

  // for * 0
  {
    v.erase(v.begin(), v.end());
    auto r = empty_rng<empty_range<int>>();
    assert(inner_cartesian_product_plus(r, to_v).first == end(r));
    assert(v.empty());

    v.erase(v.begin(), v.end());
    inner_cartesian_product(r, to_v);
    assert(v.empty());
  }
  // for * 1
  {
    for (int &i : iters(0, 10)) {
      v.erase(v.begin(), v.end());
      auto r = single_rng(irng(0, i));
      assert(inner_cartesian_product_plus(r, to_v).first == end(r));
      assert(equal(v,
                   bind_rng(irng(0, i), bind(irng, _1, bind(plus{}, _1, 1))),
                   equal));

      v.erase(v.begin(), v.end());
      inner_cartesian_product(r, to_v);
      assert(equal(v,
                   bind_rng(irng(0, i), bind(irng, _1, bind(plus{}, _1, 1))),
                   equal));
    }
  }
  // for * 2
  {
    ez_vector<ez_vector<int>> vv;
    for (int &i : iters(0, 10))
      for (int &j : iters(0, 10)) {
        vv.erase(vv.begin(), vv.end());
        for (int &kk : iters(0, j))
          for (int &k : iters(0, i)) {
            vv.insert(vv.end(), {});
            copy(seq(k, kk), to_back(back(vv)));
          }

        v.erase(v.begin(), v.end());
        auto r = seq(irng(0, i), irng(0, j));
        assert(inner_cartesian_product_plus(r, to_v).first == end(r));
        assert(v == vv);
        v.erase(v.begin(), v.end());
        inner_cartesian_product(r, to_v);
        assert(v == vv);
      }
  }
  // for * 5
  {
    ez_vector<ez_vector<int>> vv;
    for (int &i1 : iters(0, 5))
      for (int &i2 : iters(0, 5))
        for (int &i3 : iters(0, 5))
          for (int &i4 : iters(0, 5))
            for (int &i5 : iters(0, 5)) {
              vv.erase(vv.begin(), vv.end());
              for (int &k5 : iters(0, i5))
                for (int &k4 : iters(0, i4))
                  for (int &k3 : iters(0, i3))
                    for (int &k2 : iters(0, i2))
                      for (int &k1 : iters(0, i1)) {
                        vv.insert(vv.end(), {});
                        copy(seq(k1, k2, k3, k4, k5), to_back(back(vv)));
                      }

              v.erase(v.begin(), v.end());
              auto r = seq(irng(0, i1), irng(0, i2), irng(0, i3),
                           irng(0, i4), irng(0, i5));
              assert(inner_cartesian_product_plus(r, to_v).first == end(r));
              assert(v == vv);
              v.erase(v.begin(), v.end());
              inner_cartesian_product(r, to_v);
              assert(v == vv);
            }
  }
}
void test_inner_cartesian_product_range() {
  // compile-time info
  {
    using r1_t = inner_cartesian_product_range<ez_vector<ez_vector<int>>>;
    using r2_t = inner_cartesian_product_range<ez_vector<ez_vector<int>> &>;
    using r3_t = inner_cartesian_product_range
      <const ez_vector<ez_vector<int>> &>;
    using r4_t = inner_cartesian_product_range<iter_pair<ez_vector<int> *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(semiregular<r4_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!is_swappable<r2_t>);
      static_assert(!is_swappable<r3_t>);
      static_assert(is_nothrow_swappable<r4_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(!copyable<r2_t> && !is_nothrow_copyable<r2_t>);
      static_assert(!copyable<r3_t> && !is_nothrow_copyable<r3_t>);
      static_assert(copyable<r4_t> && !is_nothrow_copyable<r4_t>);
    }
    // is_rng
    {
      static_assert(is_just_irng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_just_irng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_just_irng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_just_irng<r4_t> && !is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<r4_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<r4_t>);
    }
  }
  // construct from r
  // range operations
  {
    // quick
    {
      auto a = seq(seq(1, 2), seq(3, 4));
      using r_t = inner_cartesian_product_range<decltype(a) &>;
      r_t r(a);
      assert(equal(r, seq(seq(1, 3), seq(2, 3), seq(1, 4), seq(2, 4)), equal));
      assert(r.empty());
    }
    // exhaustive
    {
      ez_vector<ez_vector<int>> v, vv;
      for (int &j : iters(1, 5))
        for (int &i : iters(0, 12)) {
          const auto r0 = chunk_rng(irng(0, i), j);
          v.erase(v.begin(), v.end());
          for (auto x : r0) {
            v.insert(v.end(), {});
            copy(x, to_back(back(v)));
          }

          vv.erase(vv.begin(), vv.end());
          inner_cartesian_product(v, output_itr([&](const auto &x) {
            vv.insert(vv.end(), {});
            copy(x, to_back(back(vv)));
          }));
          inner_cartesian_product_range<decltype(v) &> r(v);
          assert(equal(r, vv, equal));
          assert(r.empty());
        }
    }
  }
  // special member functions
  {
    {
      using l_t = ez_slist<int>;
      using ll_t = ez_slist<l_t>;
      using r_t = inner_cartesian_product_range<ll_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(ll_t{{1, 2}, {3, 4}});
      };
      const auto eq_f1 = [](r_t *p) {
        bool y = equal(*p, ll_t{{1, 3}, {2, 3}, {1, 4}, {2, 4}}, equal);
        assert(p->empty());
        *p = *p;
        return y;
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
  }
  // inner_cartesian_product_rng
  {
    auto r = inner_cartesian_product_rng
      (ez_vector<ez_vector<int>>{{1, 2}, {3, 4}});
    assert(equal(r, seq(seq(1, 3), seq(2, 3), seq(1, 4), seq(2, 4)), equal));
    auto r2 = inner_cartesian_product_rng(ez_vector<ez_vector<int>>{});
    assert(r2.empty());
  }
}
void test_cartesian_product() {
  ez_vector<tuple<int>> v0;
  cartesian_product(ez_forward_list{1, 2, 3}, to_back(v0));
  assert(equal(v0, ez_vector{tuple{1}, tuple{2}, tuple{3}}));
  v0.erase(v0.begin(), v0.end());
  cartesian_product(ez_forward_list<int>{}, to_back(v0));
  assert(equal(v0, ez_vector<tuple<int>>{}));

  ez_vector<tuple<int, int, int>> v;
  cartesian_product(ez_forward_list{1, 2, 3}, ez_list{4, 5, 6},
                    ez_vector{7, 8, 9}, to_back(v));
  assert(!v.empty());
  assert(equal(v, ez_vector{
        tuple{1, 4, 7}, tuple{1, 4, 8}, tuple{1, 4, 9},
        tuple{1, 5, 7}, tuple{1, 5, 8}, tuple{1, 5, 9},
        tuple{1, 6, 7}, tuple{1, 6, 8}, tuple{1, 6, 9},
        tuple{2, 4, 7}, tuple{2, 4, 8}, tuple{2, 4, 9},
        tuple{2, 5, 7}, tuple{2, 5, 8}, tuple{2, 5, 9},
        tuple{2, 6, 7}, tuple{2, 6, 8}, tuple{2, 6, 9},
        tuple{3, 4, 7}, tuple{3, 4, 8}, tuple{3, 4, 9},
        tuple{3, 5, 7}, tuple{3, 5, 8}, tuple{3, 5, 9},
        tuple{3, 6, 7}, tuple{3, 6, 8}, tuple{3, 6, 9}
      }));
  v.erase(v.begin(), v.end());
  {
    cartesian_product(ez_forward_list{1, 2, 3}, ez_list<int>{},
                      ez_vector{7, 8, 9}, to_back(v));
    assert(v.empty());
    cartesian_product(ez_forward_list<int>{}, ez_list<int>{1, 2, 3},
                      ez_vector<int>{1, 2, 3}, to_back(v));
    assert(v.empty());
    cartesian_product(ez_forward_list<int>{1, 2, 3}, ez_list<int>{1, 2, 3},
                      ez_vector<int>{}, to_back(v));
    assert(v.empty());
  }

  ez_vector<tuple<int, int>> v2;
  cartesian_product(ez_forward_list{1, 2}, ez_list{3, 4}, to_back(v2));
  assert(equal(v2, ez_vector{
        tuple{1, 3}, tuple{1, 4}, tuple{2, 3}, tuple{2, 4}
      }));
}
void test_cartesian_product_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = cartesian_product_iterator<int *>;
        static_assert(same_as<iter_t::value_type, tuple<int>>);
        static_assert(same_as<iter_t::reference, tuple<int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
      {
        using iter_t = cartesian_product_iterator<int *, float *, void **>;
        static_assert(same_as<iter_t::value_type, tuple<int, float, void *>>);
        static_assert(same_as
                      <iter_t::reference, tuple<int &, float &, void *&>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
    }
    // special member functions
    {
      {
        using iter_t = cartesian_product_iterator<int *>;

        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);

        static_assert(is_nothrow_default_constructible<iter_t>);
        static_assert(is_nothrow_copyable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
      {
        using iter_t = cartesian_product_iterator
          <int *, ez_list<int>::iterator>;

        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);

        static_assert(is_nothrow_default_constructible<iter_t>);
        static_assert(is_nothrow_copyable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
    }
    // iterator concept
    {
      // as forward iterator
      {
        using iter_t = cartesian_product_iterator
          <ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = cartesian_product_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = cartesian_product_iterator<ez_vector<int>::iterator>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      {
        using iter_t = cartesian_product_iterator
          <int *, ez_list<int>::iterator>;
        using c_iter_t = cartesian_product_iterator
          <const int *, ez_list<int>::const_iterator>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
        static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
      }
      {      
        using iter_t = cartesian_product_iterator<int *, float *>;
        using c_iter_t = cartesian_product_iterator<const int *, const float *>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
        static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
        static_assert(three_way_comparable_with<iter_t, iter_t>);
        static_assert(three_way_comparable_with<iter_t, c_iter_t>);
        static_assert(three_way_comparable_with<c_iter_t, c_iter_t>);
      }
    }
  }
  // cartesian_product_itr
  {
    auto i = cartesian_product_itr
      (tuple(array<int *, 3>{nullptr, nullptr, nullptr},
             array<int *, 3>{nullptr, nullptr, nullptr}));
    static_assert(same_as
                  <decltype(i), cartesian_product_iterator<int *, int *>>);
    assert(i == i);
  }
}
void test_cartesian_product_range() {
  // compile-time info
  {
    using r1_t = cartesian_product_range<ez_vector<int>, ez_vector<int>>;
    using r2_t = cartesian_product_range<array<int, 5>, array<int, 5>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);

      static_assert(!is_nothrow_copy_assignable<r1_t>);
      static_assert(is_nothrow_copy_assignable<r2_t>);
    }
    // is_rng
    {
      static_assert(is_rrng<r1_t> && is_rrng<const r1_t>);
      static_assert(is_rrng<r2_t> && is_rrng<const r2_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(is_rng_ref<cartesian_product_range<iter_pair<int *>>>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t>);
      static_assert(rng_is_sized<const r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<const r2_t>);
      static_assert(rng_is_sized<cartesian_product_range<ez_list<int>>>);
      static_assert(rng_is_sized<const cartesian_product_range<ez_list<int>>>);
      static_assert(!rng_is_sized
                    <cartesian_product_range<ez_forward_list<int>>>);
      static_assert(!rng_is_sized
                    <const cartesian_product_range<ez_forward_list<int>>>);
    }
  }
  // special member functions
  {
    {
      using l_t = ez_slist<int>;
      using r_t = cartesian_product_range<l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 3});
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(tuple(1), tuple(2), tuple(3)))
          && !p->empty() && !as_const(*p).empty()
          && p->size() == 3 && as_const(*p).size() == 3;
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && as_const(*p).empty()
          && p->size() == 0 && as_const(*p).size() == 0
          && p->begin() == p->end()
          && as_const(*p).begin() == as_const(*p).end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using l_t = ez_vector<int>;
      using r_t = cartesian_product_range<l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 3});
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(tuple(1), tuple(2), tuple(3)))
          && !p->empty() && !as_const(*p).empty()
          && p->size() == 3 && as_const(*p).size() == 3;
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && as_const(*p).empty()
          && p->size() == 0 && as_const(*p).size() == 0
          && p->begin() == p->end()
          && as_const(*p).begin() == as_const(*p).end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
  }
  // one-dimension
  {
    auto r = cartesian_product_rng(ez_vector{1, 2, 3, 4, 5});
    test_ritr(r, seq(tuple(1), 2, 3, 4, 5));
    test_ritr(as_const(r), seq(tuple(1), 2, 3, 4, 5));
    assert(!r.empty());
    assert(!r.empty());
    assert(r.size() == 5);
    assert(as_const(r).size() == 5);

    auto r2 = cartesian_product_rng(empty_rng<int>());
    test_ritr(r2, empty_rng<tuple<int>>());
    test_ritr(as_const(r2), empty_rng<tuple<int>>());
    assert(r2.empty());
    assert(as_const(r2).empty());
    assert(r2.size() == 0);
    assert(as_const(r2).size() == 0);
  }
  // two-dimension
  {
    ez_vector<int> v;
    ez_vector<int> vv;
    ez_vector<pair<int &, int &>> vvv;
    for (int i : irng(0, 6))
      for (int j : irng(0, 6)) {
        v.erase(v.begin(), v.end());
        vv.erase(vv.begin(), vv.end());
        vvv.erase(vvv.begin(), vvv.end());
        copy(irng(0, i), to_back(v));
        copy(irng(0, j), to_back(vv));
        cartesian_product(v, vv, to_back(vvv));
        auto r = cartesian_product_rng(v, vv);
        test_ritr(r, vvv);
        test_ritr(as_const(r), vvv);
        assert(r.empty() == vvv.empty());
        assert(as_const(r).empty() == vvv.empty());
        assert(r.size() == vvv.size());
        assert(as_const(r).size() == vvv.size());
      }
  }
  // three-dimension
  {
    ez_vector<int> v1;
    ez_vector<int> v2;
    ez_vector<int> v3;
    ez_vector<tuple<int &, int &, int &>> vv;
    for (int i : irng(2, 3))
      for (int j : irng(2, 3))
        for (int k : irng(2, 3)) {
          v1.erase(v1.begin(), v1.end());
          v2.erase(v2.begin(), v2.end());
          v3.erase(v3.begin(), v3.end());
          vv.erase(vv.begin(), vv.end());
          copy(irng(0, i), to_back(v1));
          copy(irng(0, j), to_back(v2));
          copy(irng(0, k), to_back(v3));
          cartesian_product(v1, v2, v3, to_back(vv));          
          auto r = cartesian_product_rng(v1, v2, v3);
          test_ritr(r, vv);
          test_ritr(as_const(r), vv);
          assert(r.size() == vv.size());
          assert(as_const(r).size() == vv.size());
          assert(r.empty() == vv.empty());
          assert(as_const(r).empty() == vv.empty());
      }
  }
  // four-dimension
  {
    ez_vector<int> v1;
    ez_vector<int> v2;
    ez_vector<int> v3;
    ez_vector<int> v4;
    ez_vector<tuple<int &, int &, int &, int &>> vv;
    for (int i : irng(0, 5))
      for (int j : irng(0, 5))
        for (int k : irng(0, 5))
          for (int l : irng(0, 5)) {
            v1.erase(v1.begin(), v1.end());
            v2.erase(v2.begin(), v2.end());
            v3.erase(v3.begin(), v3.end());
            v4.erase(v4.begin(), v4.end());
            vv.erase(vv.begin(), vv.end());
            copy(irng(0, i), to_back(v1));
            copy(irng(0, j), to_back(v2));
            copy(irng(0, k), to_back(v3));
            copy(irng(0, l), to_back(v4));
            cartesian_product(v1, v2, v3, v4, to_back(vv));
            auto r = cartesian_product_rng(v1, v2, v3, v4);
            test_ritr(r, vv);
            test_ritr(as_const(r), vv);
            assert(r.size() == vv.size());
            assert(as_const(r).size() == vv.size());
            assert(r.empty() == vv.empty());
            assert(as_const(r).empty() == vv.empty());
          }
  }
  // four-dimension for fitr
  {
    ez_forward_list<int> v1;
    ez_forward_list<int> v2;
    ez_forward_list<int> v3;
    ez_forward_list<int> v4;
    ez_vector<tuple<int &, int &, int &, int &>> vv;
    for (int i : irng(0, 5))
      for (int j : irng(0, 5))
        for (int k : irng(0, 5))
          for (int l : irng(0, 5)) {
            v1.erase_after(v1.before_begin(), v1.end());
            v2.erase_after(v2.before_begin(), v2.end());
            v3.erase_after(v3.before_begin(), v3.end());
            v4.erase_after(v4.before_begin(), v4.end());
            vv.erase(vv.begin(), vv.end());
            copy(irng(0, i), to_front(v1));
            copy(irng(0, j), to_front(v2));
            copy(irng(0, k), to_front(v3));
            copy(irng(0, l), to_front(v4));
            cartesian_product(v1, v2, v3, v4, to_back(vv));
            auto r = cartesian_product_rng(v1, v2, v3, v4);
            test_fitr(r, vv);
            test_fitr(as_const(r), vv);
            static_assert(!rng_is_sized<decltype(r)>);
            assert(r.empty() == vv.empty());
            assert(as_const(r).empty() == vv.empty());
          }
  }
}

void test_algorithm_by_range_2() {
  printf("algorithm_by_range_2: ");

  inner::test::test_aligned_stride();
  inner::test::test_aligned_stride_iterator();
  inner::test::test_aligned_stride_range();
  inner::test::test_stride();
  inner::test::test_stride_iterator();
  inner::test::test_stride_range();
  inner::test::test_aligned_chunk();
  inner::test::test_aligned_chunk_iterator();
  inner::test::test_aligned_chunk_range();
  inner::test::test_chunk();
  inner::test::test_chunk_iterator();
  inner::test::test_chunk_range();
  inner::test::test_chunk_by();
  inner::test::test_chunk_by_iterator();
  inner::test::test_chunk_by_range();
  inner::test::test_inner_cartesian_product();
  inner::test::test_inner_cartesian_product_range();
  inner::test::test_cartesian_product();
  inner::test::test_cartesian_product_iterator();
  inner::test::test_cartesian_product_range();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_algorithm_by_range_2();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
