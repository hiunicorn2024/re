#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>

#include <cassert>

namespace re::inner::test {

void test_split() {
  // quick
  {
    {
      ez_vector<iter_pair<int *>> vv;
      ez_vector<int> v;
      split(v, irng(1, 1), to_back(vv));
      assert(empty(vv));
      split(v, irng(1, 3), to_back(vv));
      assert(empty(vv));
    }
    {
      ez_vector<iter_pair<int *>> vv;
      ez_vector<int> v = {1, 2};
      split(v, irng(1, 3), to_back(vv));
      assert(equal(vv, ez_vector<ez_vector<int>>{{}, {}}, equal));
    }
    {
      ez_vector<iter_pair<int *>> vv;
      ez_vector<int> v = {1, 2, 0, 1, 2, 0, 0, 1, 2, 1, 2};
      split(v, irng(1, 3), to_back(vv));
      ez_vector<ez_vector<int>> cmp_v = {{}, {0}, {0, 0}, {}, {}};
      assert(equal(vv, cmp_v, equal));
    }
    {
      ez_vector<iter_pair<int *>> vv;
      ez_vector<int> v = {1, 2, 0, 1, 2, 0, 0};
      split(v, irng(1, 3), to_back(vv));
      ez_vector<ez_vector<int>> cmp_v = {{}, {0}, {0, 0}};
      assert(equal(vv, cmp_v, equal));
    }
    {
      ez_vector<iter_pair<int *>> vv;
      ez_vector<int> v = {1, 2, 0, 1, 2, 0, 0};
      split(v, empty_rng<int>(), to_back(vv));
      assert(vv.size() == 1 && equal(front(vv), v));
    }
  }
  // exhaustive
  {
    ez_vector<ez_vector<int>> vv;
    ez_vector<int> v;
    ez_vector<iter_pair<int *>> v_cmp;
    for (int n = 11; n != 0; --n) {
      ez_vector<int> tmp;
      copy(irng(0, 3), to_back(tmp));
      vv.insert(vv.end(), move(tmp));
    }
    for (decltype(auto) x : inner_cartesian_product_rng(vv)) {
      v.erase(v.begin(), v.end());
      for (decltype(auto) r : chunk_rng(x, 2)) {
        if (front(r) != 0)
          copy(irng(0, front(r)), to_back(v));
        if (back(r) != 0)
          copy(rng(3, 100), to_back(v));
      }

      v_cmp.erase(v_cmp.begin(), v_cmp.end());
      split(v, rng(3, 100), to_back(v_cmp));

      auto r = split_rng(v, rng(3, 100));
      assert(equal(r, v_cmp));
    }
  }
}
void test_split_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = split_iterator<int *>;
        static_assert(same_as<iter_t::value_type, iter_pair<int *>>);
        static_assert(same_as<iter_t::reference, iter_pair<int *>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      {
        using iter_t = split_iterator<int *>;

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
        using iter_t = split_iterator<int *>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      {
        using iter_t = split_iterator<int *>;
        using c_iter_t = split_iterator<const int *>;
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
  }
  // iterator operations
  // split_itr
  {
    {
      int a[] = {1, 2, 0, 0, 3, 4};
      auto i = split_itr(nth(a, 0), nth(a, 2), end(a), 2);
      auto j = split_itr(end(a), end(a), end(a), 0);
      test_fitr(i, j, seq(rng(nth(a, 0), 2), rng(nth(a, 4), 2)));
    }
    {
      int a[] = {0, 0};
      auto i = split_itr(nth(a, 0), nth(a, 0), end(a), 2);
      auto j = split_itr(end(a), end(a), end(a), 0);
      test_fitr(i, j, seq(iter_pair<int *>{}, iter_pair<int *>{}), equal);
    }
    {
      int a[] = {1, 2, 0, 3, 4, 0, 5, 0, 6, 0};
      auto i = split_itr(nth(a, 0), nth(a, 2), end(a), 1);
      auto j = split_itr(end(a), end(a), end(a), 0);
      using v_t = ez_vector<int>;
      using vv_t = ez_vector<v_t>;
      test_fitr(i, j, vv_t{v_t{1, 2}, v_t{3, 4}, v_t{5}, v_t{6}, v_t{}}, equal);
    }
  }
}
void test_split_range() {
  // compile-time info
  {
    using r1_t = split_range<ez_forward_list<int>, ez_forward_list<int>>;
    using r2_t = split_range<ez_list<int>, ez_list<int>>;
    using r3_t = split_range<ez_vector<int>, ez_vector<int>>;
    using r4_t = split_range<ez_vector<int>, ez_vector<int> &>;
    using r5_t = split_range<iter_pair<int *>, iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_frng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_frng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_frng<r4_t> && !is_rng<const r4_t>);
      static_assert(is_frng<r5_t> && is_frng<const r5_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<r4_t>);
      static_assert(!rng_is_sized<r5_t>);
    }
  }
  // special member functions
  {
    {
      using l_t = ez_vector<int>;
      using r_t = split_range<l_t, l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 0, 3, 4, 0}, l_t{0});
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector{l_t{1, 2}, l_t{3, 4}, l_t{}}, equal)
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
      using l_t = iter_pair<int *>;
      using r_t = split_range<l_t, l_t>;
      using v_t = ez_vector<int>;
      v_t v = {0, 1, 2, 0};
      const auto init_f1 = [&](r_t *p) {
        new(p) r_t(rng(v), rng(nth(v, 0), 1));
      };
      const auto eq_f1 = [&](r_t *p) {
        return equal(*p, ez_vector{v_t{}, v_t{1, 2}, v_t{}}, equal)
          && !p->empty();
      };
      const auto init_f2 = [](r_t *p) {
        new(p) r_t;
      };
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // range operations
  // split_rng
  {
    {
      ez_forward_list<int> a = {};
      auto r = split_rng(a, seq(1, 2, 3));
      assert(r.empty());
    }
    {
      ez_forward_list<int> a = {1, 2};
      auto r = split_rng(a, empty_rng<int>());
      assert(equal(r, seq(seq(1, 2)), equal));
    }
    {
      ez_forward_list<int> a = {1, 0, 2, 0, 3, 0, 4};
      auto r = split_rng(a, seq(0));
      assert(equal(r, seq(seq(1), seq(2), seq(3), seq(4)), equal));
    }
    {
      ez_forward_list<int> a = {0, 1, 0, 2, 0, 3, 0, 4, 0};
      auto r = split_rng(a, seq(0));
      using v_t = ez_vector<int>;
      assert(equal(r, ez_vector{v_t{}, v_t{1}, v_t{2}, v_t{3}, v_t{4}, v_t{}},
                   equal));
    }
  }
}
void test_zip() {
  // one elements
  {
    //std::cout << (at<0>(zip_top(irng(1, 4))) - iitr(1)) << std::endl;
    assert(zip_top(irng(1, 4)) == tuple(iitr(4)));

    ez_vector<tuple<int>> v;
    zip(irng(1, 4), to_back(v));
    assert(equal(bind_rng(v, at<0>), irng(1, 4)));
    assert(zip_top(irng(1, 4)) == tuple(iitr(4)));
  }
  // two elements
  {
    assert(zip_top(irng(1, 4), irng(4, 10)) == tuple(iitr(4), iitr(7)));
    assert(zip_top(irng(4, 10), irng(1, 4)) == tuple(iitr(7), iitr(4)));

    ez_vector<tuple<int, int>> v;
    zip(irng(1, 4), iitr(4), to_back(v));
    assert(equal(v, seq(tuple(1, 4), tuple(2, 5), tuple(3, 6))));
    v.erase(v.begin(), v.end());
    zip(iitr(4), irng(1, 4), to_back(v));
    assert(equal(v, seq(tuple(4, 1), tuple(5, 2), tuple(6, 3))));

    assert(zip(irng(1, 4), irng(1, 7), v.begin()) == v.end());
    assert(equal(v, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3))));
  }
  // three elements
  {
    ez_vector<pair<int, int>> v;
    zip(iitr(1), irng(1, 4), to_back(v));
    assert(equal(v, seq(pair{1, 1}, pair{2, 2}, pair{3, 3})));

    v.erase(v.begin(), v.end());
    zip(irng(1, 4), iitr(5), to_back(v));
    assert(equal(v, seq(pair{1, 5}, pair{2, 6}, pair{3, 7})));
    assert(zip_top(irng(1, 4), iitr(5)) == tuple(iitr(4), iitr(8)));

    v.erase(v.begin(), v.end());
    ez_slist<int> l = {1, 2, 3};
    ez_list<int> l2 = {1, 2};
    zip(l, l2, to_back(v));
    assert(equal(v, seq(pair{1, 1}, pair{2, 2})));
    assert(zip_top(l, l2) == tuple(nth(l, 2), end(l2)));
  }
}
void test_zip_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = zip_iterator<tuple<int *>>;
        static_assert(same_as<iter_t::value_type, tuple<int>>);
        static_assert(same_as<iter_t::reference, tuple<int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, forward_iterator_tag>);
      }
      {
        using iter_t = zip_iterator<tuple<int *>, int *, int *>;
        static_assert(same_as<iter_t::value_type, tuple<int, int, int>>);
        static_assert(same_as<iter_t::reference, tuple<int &, int &, int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      {
        using iter_t = zip_iterator<tuple<int *>>;

        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);

        static_assert(is_nothrow_default_constructible<iter_t>);
        static_assert(is_nothrow_copyable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = zip_iterator
          <tuple<degraded_input_iterator<int *>>>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = zip_iterator<int *, tuple<int *>, int *>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      {
        using iter_t = zip_iterator
          <tuple<int *>, ez_list<int>::iterator>;
        using c_iter_t = zip_iterator
          <tuple<const int *>, ez_list<int>::const_iterator>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
        static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
        static_assert(!three_way_comparable_with<iter_t, iter_t>);
        static_assert(!three_way_comparable_with<iter_t, c_iter_t>);
        static_assert(!three_way_comparable_with<c_iter_t, c_iter_t>);
      }
      {
        using iter_t = zip_iterator<int *, float *, tuple<long *>>;
        using c_iter_t = zip_iterator
          <const int *, const float *, tuple<const long *>>;
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
  }
  // iterator operations
  // zip_itr
  {
    // one dimension
    {
      int a[] = {1, 2, 3};
      // empty
      {
        auto i = zip_itr(tuple(begin(a)));
        auto j = zip_itr(tuple(begin(a)));
        test_fitr(i, j, empty_rng<tuple<int>>());
      }
      // non-empty
      {
        auto i = zip_itr(tuple(begin(a)));
        auto j = zip_itr(tuple(end(a)));
        test_fitr(i, j, seq(tuple(1), tuple(2), tuple(3)));
      }
    }
    // two dimension
    {
      int a[] = {1, 2, 3};
      int a2[] = {4, 5, 6};
      int a3[] = {7, 8};
      // rng - rng
      {
        // empty
        {
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)));
            auto j = zip_itr(tuple(begin(a)), tuple(begin(a2)));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)));
            auto j = zip_itr(tuple(begin(a)), tuple(end(a2)));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)));
            auto j = zip_itr(tuple(end(a)), tuple(begin(a2)));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
        }
        // non-empty
        {
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)));
            auto j = zip_itr(tuple(end(a)), tuple(end(a2)));
            test_fitr(i, j, seq(tuple(1, 4), tuple(2, 5), tuple(3, 6)));
          }
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a3)));
            auto j = zip_itr(tuple(end(a)), tuple(end(a3)));
            test_fitr(i, j, seq(tuple(1, 7), tuple(2, 8)));
          }
          {
            auto i = zip_itr(tuple(begin(a3)), tuple(begin(a)));
            auto j = zip_itr(tuple(end(a3)), tuple(end(a)));
            test_fitr(i, j, seq(tuple(7, 1), tuple(8, 2)));
          }
        }
      }
      // itr - rng
      {
        // empty
        {
          {
            auto i = zip_itr(tuple(begin(a)), begin(a2));
            auto j = zip_itr(tuple(begin(a)), begin(a2));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), begin(a2));
            auto j = zip_itr(tuple(begin(a)), end(a2));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
          {
            auto i = zip_itr(begin(a2), tuple(begin(a)));
            auto j = zip_itr(begin(a2), tuple(begin(a)));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
          {
            auto i = zip_itr(begin(a2), tuple(begin(a)));
            auto j = zip_itr(end(a2), tuple(begin(a)));
            test_fitr(i, j, empty_rng<tuple<int &, int &>>());
          }
        }
        // non-empty
        {
          {
            auto i = zip_itr(tuple(begin(a3)), begin(a));
            auto j = zip_itr(tuple(end(a3)), begin(a));
            test_fitr(i, j, seq(tuple(7, 1), tuple(8, 2)));
          }
          {
            auto i = zip_itr(tuple(begin(a3)), begin(a));
            auto j = zip_itr(tuple(end(a3)), end(a));
            test_fitr(i, j, seq(tuple(7, 1), tuple(8, 2)));
          }
          {
            auto i = zip_itr(begin(a), tuple(begin(a3)));
            auto j = zip_itr(begin(a), tuple(end(a3)));
            test_fitr(i, j, seq(tuple(1, 7), tuple(2, 8)));
          }
          {
            auto i = zip_itr(begin(a), tuple(begin(a3)));
            auto j = zip_itr(end(a), tuple(end(a3)));
            test_fitr(i, j, seq(tuple(1, 7), tuple(2, 8)));
          }
        }
      }
    }
    // three dimension
    {
      int a[] = {1, 2, 3};
      int a2[] = {4, 5, 6, 7};
      int a3[] = {8, 9};
      // rng - rng
      {
        // empty
        {
          auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)), tuple(begin(a3)));
          auto j = zip_itr(tuple(begin(a)), tuple(begin(a2)), tuple(begin(a3)));
          test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
        }
        // non-empty
        {
          auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)), tuple(begin(a3)));
          auto j = zip_itr(tuple(end(a)), tuple(end(a2)), tuple(end(a3)));
          test_fitr(rng(i, j), seq(tuple(1, 4, 8), tuple(2, 5, 9)));
        }
      }
      // itr - rng
      {
        // empty
        {
          {
            auto i = zip_itr(begin(a), tuple(begin(a2)), tuple(begin(a3)));
            auto j = zip_itr(begin(a), tuple(begin(a2)), tuple(begin(a3)));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), begin(a2), tuple(begin(a3)));
            auto j = zip_itr(tuple(begin(a)), begin(a2), tuple(begin(a3)));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), tuple(begin(a2)), begin(a3));
            auto j = zip_itr(tuple(begin(a)), tuple(begin(a2)), begin(a3));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
          {
            auto i = zip_itr(tuple(begin(a)), begin(a2), begin(a3));
            auto j = zip_itr(tuple(begin(a)), begin(a2), begin(a3));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
          {
            auto i = zip_itr(begin(a), tuple(begin(a2)), begin(a3));
            auto j = zip_itr(begin(a), tuple(begin(a2)), begin(a3));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
          {
            auto i = zip_itr(begin(a), begin(a2), tuple(begin(a3)));
            auto j = zip_itr(begin(a), begin(a2), tuple(begin(a3)));
            test_fitr(rng(i, j), empty_rng<tuple<int, int, int>>());
          }
        }
        // non-empty
        {
          {
            auto i = zip_itr(begin(a), tuple(begin(a2)), tuple(begin(a3)));
            auto j = zip_itr(begin(a), tuple(end(a2)), tuple(end(a3)));
            test_fitr(rng(i, j), seq(tuple(1, 4, 8), tuple(2, 5, 9)));
          }
          {
            auto i = zip_itr(tuple(begin(a)), begin(a2), tuple(begin(a3)));
            auto j = zip_itr(tuple(end(a)), end(a2), tuple(end(a3)));
            test_fitr(rng(i, j), seq(tuple(1, 4, 8), tuple(2, 5, 9)));
          }
          {
            auto i = zip_itr(begin(a), begin(a2), tuple(begin(a3)));
            auto j = zip_itr(begin(a), begin(a2), tuple(end(a3)));
            auto ii = zip_itr(cbegin(a), cbegin(a2), tuple(cbegin(a3)));
            auto jj = zip_itr(cbegin(a), cbegin(a2), tuple(cend(a3)));
            test_fitr(i, j, seq(tuple(1, 4, 8), tuple(2, 5, 9)));
            test_fitr(i, jj, seq(tuple(1, 4, 8), tuple(2, 5, 9)));
            test_fitr(ii, j, seq(tuple(1, 4, 8), tuple(2, 5, 9)));
            test_fitr(ii, jj, seq(tuple(1, 4, 8), tuple(2, 5, 9)));
          }
        }
      }
    }
  }
}
void test_zip_range() {
  // compile-time info
  {
    using r1_t = zip_range<ez_forward_list<int>>;
    using r2_t = zip_range<ez_list<int>>;
    using r3_t = zip_range<ez_vector<int>, int *>;
    using r4_t = zip_range<int *, ez_vector<int> &>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(!is_copy_assignable<r4_t>);
      static_assert(!is_move_assignable<r4_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);
      static_assert(!is_swappable<r4_t>);

      static_assert(!is_nothrow_copy_assignable<r1_t>);
      static_assert(!is_nothrow_copy_assignable<r2_t>);
      static_assert(!is_nothrow_copy_assignable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && is_rng<const r1_t>);
      static_assert(is_frng<r2_t> && is_rng<const r2_t>);
      static_assert(is_frng<r3_t> && is_rng<const r3_t>);
      static_assert(is_frng<r4_t> && is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<r4_t>);
    }
  }
  // special member functions
  {
    {
      using l_t = ez_slist<int>;
      using r_t = zip_range<l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 3});
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(tuple(1), tuple(2), tuple(3)))
          && !p->empty() && !as_const(*p).empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && as_const(*p).empty()
          && p->begin() == p->end()
          && as_const(*p).begin() == as_const(*p).end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
  }
  // one-dimension
  {
    int a[] = {1, 2, 3, 4};
    auto r = zip_rng(a);
    assert(equal(r, bind_rng(irng(1, 5), make_tuple)));
  }
  // two-dimension
  {
    int a[] = {1, 2, 3};
    auto r = zip_rng(a, begin(a));
    auto r2 = zip_rng(begin(a), a);
    assert(equal(r, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3))));

    auto r3 = zip_rng(a, a);
    assert(equal(r3, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3))));
  }
  // three-dimension
  {
    int a[] = {1, 2, 3};
    int a2[] = {4, 5};
    int a3[] = {6, 7, 8, 9};
    auto r = zip_rng(a, a2, a3);
    assert(equal(r, seq(tuple(1, 4, 6), tuple(2, 5, 7))));
  }
}
void test_aligned_zip_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = aligned_zip_iterator<int *>;
        static_assert(same_as<iter_t::value_type, tuple<int>>);
        static_assert(same_as<iter_t::reference, tuple<int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
      {
        using iter_t = aligned_zip_iterator<int *, int *, int *>;
        static_assert(same_as<iter_t::value_type, tuple<int, int, int>>);
        static_assert(same_as<iter_t::reference, tuple<int &, int &, int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
    }
    // special member functions
    {
      {
        using iter_t = aligned_zip_iterator<int *>;

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
        using iter_t = aligned_zip_iterator
          <ez_forward_list<int>::iterator,
           ez_forward_list<int>::iterator,
           ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = aligned_zip_iterator
          <ez_list<int>::iterator,
           ez_bidirectional_list<int>::iterator,
           int *>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = aligned_zip_iterator<int *, const int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      {
        using iter_t = aligned_zip_iterator<int *, ez_list<int>::iterator>;
        using c_iter_t = aligned_zip_iterator
          <const int *, ez_list<int>::const_iterator>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
        static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
        static_assert(!three_way_comparable_with<iter_t, iter_t>);
        static_assert(!three_way_comparable_with<iter_t, c_iter_t>);
        static_assert(!three_way_comparable_with<c_iter_t, c_iter_t>);
      }
      {
        using iter_t = aligned_zip_iterator<int *, float *, long *>;
        using c_iter_t = aligned_zip_iterator
          <const int *, const float *, const long *>;
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
  // iterator operations
  // aligned_zip_itr
  {
    // one element
    {
      int a[] = {1, 2, 3};
      auto i = aligned_zip_itr(begin(a));
      auto j = aligned_zip_itr(end(a));
      auto ci = aligned_zip_itr(cbegin(a));
      auto cj = aligned_zip_itr(cend(a));
      test_ritr(i, j, seq(tuple(1), tuple(2), tuple(3)));
      test_ritr(i, cj, seq(tuple(1), tuple(2), tuple(3)));
      test_ritr(ci, j, seq(tuple(1), tuple(2), tuple(3)));
      test_ritr(ci, cj, seq(tuple(1), tuple(2), tuple(3)));
      test_ritr(i, i, empty_rng<tuple<int>>());
      test_ritr(ci, i, empty_rng<tuple<int>>());
      test_ritr(i, ci, empty_rng<tuple<int>>());
      test_ritr(ci, ci, empty_rng<tuple<int>>());
    }
    // two elements
    {
      int a[] = {1, 2, 3};
      int a2[] = {1, 2, 3};
      auto i = aligned_zip_itr(begin(a), begin(a2));
      auto j = aligned_zip_itr(end(a), end(a2));
      auto ci = aligned_zip_itr(cbegin(a), cbegin(a2));
      auto cj = aligned_zip_itr(cend(a), cend(a2));
      test_ritr(i, j, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3)));
      test_ritr(i, cj, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3)));
      test_ritr(ci, j, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3)));
      test_ritr(ci, cj, seq(tuple(1, 1), tuple(2, 2), tuple(3, 3)));
      test_ritr(i, i, empty_rng<tuple<int, int>>());
      test_ritr(ci, i, empty_rng<tuple<int, int>>());
      test_ritr(i, ci, empty_rng<tuple<int, int>>());
      test_ritr(ci, ci, empty_rng<tuple<int, int>>());
    }
    // three elements
    {
      int a[] = {1, 2, 3};
      int a2[] = {1, 2, 3};
      int a3[] = {1, 2, 3};
      auto i = aligned_zip_itr(begin(a), begin(a2), begin(a3));
      auto j = aligned_zip_itr(end(a), end(a2), end(a3));
      auto ci = aligned_zip_itr(cbegin(a), cbegin(a2), cbegin(a3));
      auto cj = aligned_zip_itr(cend(a), cend(a2), cend(a3));
      test_ritr(i, j, seq(tuple(1, 1, 1), tuple(2, 2, 2), tuple(3, 3, 3)));
      test_ritr(i, cj, seq(tuple(1, 1, 1), tuple(2, 2, 2), tuple(3, 3, 3)));
      test_ritr(ci, j, seq(tuple(1, 1, 1), tuple(2, 2, 2), tuple(3, 3, 3)));
      test_ritr(ci, cj, seq(tuple(1, 1, 1), tuple(2, 2, 2), tuple(3, 3, 3)));
      test_ritr(i, i, empty_rng<tuple<int, int, int>>());
      test_ritr(ci, i, empty_rng<tuple<int, int, int>>());
      test_ritr(i, ci, empty_rng<tuple<int, int, int>>());
      test_ritr(ci, ci, empty_rng<tuple<int, int, int>>());
    }
    // ten elements
    {
      int a[] = {1, 2, 3};
      int a2[] = {1, 2, 3};
      int a3[] = {1, 2, 3};
      int a4[] = {1, 2, 3};
      int a5[] = {1, 2, 3};
      int a6[] = {1, 2, 3};
      int a7[] = {1, 2, 3};
      int a8[] = {1, 2, 3};
      int a9[] = {1, 2, 3};
      int a10[] = {1, 2, 3};
      auto i = aligned_zip_itr(begin(a), begin(a2), begin(a3),
                               begin(a4), begin(a5), begin(a6),
                               begin(a7), begin(a8), begin(a9), begin(a10));
      auto j = aligned_zip_itr(end(a), end(a2), end(a3),
                               end(a4), end(a5), end(a6),
                               end(a7), end(a8), end(a9), end(a10));
      test_ritr(i, j,
                seq(tuple(1, 1, 1, 1, 1, 1, 1, 1, 1, 1),
                    tuple(2, 2, 2, 2, 2, 2, 2, 2, 2, 2),
                    tuple(3, 3, 3, 3, 3, 3, 3, 3, 3, 3)));
    }
  }
}
void test_aligned_zip_range() {
  // compile-time info
  {
    using r1_t = aligned_zip_range<ez_forward_list<int>>;
    using r2_t = aligned_zip_range<ez_list<int>>;
    using r3_t = aligned_zip_range<ez_vector<int>, int *>;
    using r4_t = aligned_zip_range<int *, ez_vector<int> &>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(!is_copy_assignable<r4_t>);
      static_assert(!is_move_assignable<r4_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);
      static_assert(!is_swappable<r4_t>);

      static_assert(!is_nothrow_copy_assignable<r1_t>);
      static_assert(!is_nothrow_copy_assignable<r2_t>);
      static_assert(!is_nothrow_copy_assignable<r3_t>);
      static_assert(!is_nothrow_copy_assignable<r4_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_brng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_rrng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_rrng<r4_t> && is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t>);
    }
  }
  // special member functions
  {
    {
      using l_t = ez_slist<int>;
      using r_t = aligned_zip_range<l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 3});
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(tuple(1), tuple(2), tuple(3))) && !p->empty();
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
  }
  // one element
  {
    int a[] = {1, 2, 3};
    {
      auto r = aligned_zip_rng(a);
      test_fitr(r, seq(tuple(1), tuple(2), tuple(3)));
      assert(!r.empty());
      assert(!as_const(r).empty());
      assert(r.size() == 3);
      assert(as_const(r).size() == 3);
    }
    {
      auto r = aligned_zip_rng(to_array(a));
      test_fitr(r, seq(tuple(1), tuple(2), tuple(3)));
      assert(!r.empty());
      assert(r.size() == 3);
    }
    {
      auto r = aligned_zip_rng(rng(begin(a), begin(a)));
      test_fitr(r, empty_rng<tuple<int>>());
      assert(r.empty());
      assert(as_const(r).empty());
      assert(r.size() == 0);
      assert(as_const(r).size() == 0);
    }
  }
  // two elements
  {
    int a[] = {1, 2, 3};
    int b[] = {4, 5};
    {
      auto r = aligned_zip_rng(a, b);
      assert(equal(r, seq(tuple(1, 4), tuple(2, 5))));
      assert(r.size() == 2);
      assert(as_const(r).size() == 2);
      assert(!r.empty());
      assert(!as_const(r).empty());
    }
    {
      auto r = aligned_zip_rng(to_array(a), to_array(b));
      assert(equal(r, seq(tuple(1, 4), tuple(2, 5))));
      assert(r.size() == 2);
      assert(!r.empty());
    }
    {
      auto r = aligned_zip_rng(begin(a), b);
      assert(equal(r, seq(tuple(1, 4), tuple(2, 5))));
      assert(r.size() == 2);
      assert(as_const(r).size() == 2);
      assert(!r.empty());
      assert(!as_const(r).empty());
    }
  }
  // four elements
  {
    ez_vector<int> a = {1, 2, 3};
    ez_vector<int> a2 = {1, 2, 3, 4};
    ez_vector<int> a3 = {1, 2};
    ez_vector<int> a4 = {};
    ez_vector<int> a5 = {1, 2, 3, 4, 5};

    {
      auto r = aligned_zip_rng(a, a2, a3, a5);
      assert(equal(r, seq(tuple(1, 1, 1, 1), tuple(2, 2, 2, 2))));
      assert(r.size() == 2 && !r.empty());
      assert(as_const(r).size() == 2 && !as_const(r).empty());
    }
    {
      auto r = aligned_zip_rng(a, a2, a4, a5);
      assert(r.size() == 0 && r.empty());
      assert(as_const(r).size() == 0 && as_const(r).empty());
    }
    {
      auto r = aligned_zip_rng(begin(a), a3, begin(a2), begin(a5));
      assert(equal(r, seq(tuple(1, 1, 1, 1), tuple(2, 2, 2, 2))));
      assert(r.size() == 2 && !r.empty());
    }
  }
}
void test_enumerate() {
  {
    int a[] = {1, 2, 3, 4};
    pair<ptrdiff_t, int> b[4] = {};
    assert(enumerate_plus(a, begin(b)) == pair(end(a), end(b)));
    assert(equal(b, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4))));
  }
  {
    int a[] = {1, 2, 3, 4};
    pair<ptrdiff_t, int> b[4] = {};
    assert(enumerate(a, begin(b)) == end(b));
    assert(equal(b, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4))));
  }
}
void test_enumerate_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = enumerate_iterator<int *>;
      static_assert(same_as<iter_t::value_type, pair<ptrdiff_t, int>>);
      static_assert(same_as<iter_t::reference, pair<ptrdiff_t, int &>>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, void>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = enumerate_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = enumerate_iterator<degraded_input_iterator<int *>>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = enumerate_iterator<degraded_forward_iterator<int *>>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = enumerate_iterator
          <degraded_bidirectional_iterator<int *>>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = enumerate_iterator<int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);        
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = enumerate_iterator<int *>;
      using c_iter_t = enumerate_iterator<const int *>;
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
  // iterator operations
  // enumerate_itr
  {
    int a[] = {1, 2, 3, 4};
    auto i = enumerate_itr(begin(a), 0);
    auto j = enumerate_itr(end(a), 4);
    auto ci = enumerate_itr(cbegin(a), 0);
    auto cj = enumerate_itr(cend(a), 4);
    test_ritr(i, j, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)));
    test_ritr(ci, j, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)));
    test_ritr(i, cj, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)));
    test_ritr(ci, cj, seq(pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)));
    test_ritr(i, i, empty_rng<pair<ptrdiff_t, int>>());
  }
}
void test_enumerate_range() {
  // compile-time info
  {
    using r1_t = enumerate_range<ez_forward_list<int>>;
    using r2_t = enumerate_range<ez_list<int>>;
    using r3_t = enumerate_range<ez_vector<int>>;
    using r4_t = enumerate_range<ez_vector<int> &>;
    using r5_t = enumerate_range<iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(semiregular<r5_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);
      static_assert(!is_swappable<r4_t>);
      static_assert(is_nothrow_swappable<r5_t>);

      static_assert(copyable<r1_t> && !is_nothrow_copyable<r1_t>);
      static_assert(copyable<r2_t> && !is_nothrow_copyable<r2_t>);
      static_assert(copyable<r3_t> && !is_nothrow_copyable<r3_t>);
      static_assert(!copyable<r4_t>);
      static_assert(copyable<r5_t> && is_nothrow_copyable<r5_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_brng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_rrng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_rrng<r4_t> && is_rrng<const r4_t>);
      static_assert(is_rrng<r5_t> && is_rrng<const r5_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t>);
      static_assert(rng_is_sized<r5_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = enumerate_range<ez_vector<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4});};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector{
            pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)
          });
      };
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = enumerate_range<iter_pair<int *>>;
      ez_vector<int> v = {1, 2, 3, 4};
      const auto init_f1 = [&](r_t *p) {new(p) r_t(rng(v));};
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, ez_vector{
            pair(0, 1), pair(1, 2), pair(2, 3), pair(3, 4)
          });
      };
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // range operations
  {
    {
      ez_forward_list<int> a = {0, 1, 2};
      auto r = enumerate_rng(a);
      test_fitr(r, seq(pair(0, 0), pair(1, 1), pair(2, 2)));
    }
    {
      ez_list<int> a = {0, 1, 2};
      auto r = enumerate_rng(a);
      test_bitr(r, seq(pair(0, 0), pair(1, 1), pair(2, 2)));
    }
    {
      int a[] = {0, 1, 2};
      auto r = enumerate_rng(a);
      test_ritr(r, seq(pair(0, 0), pair(1, 1), pair(2, 2)));
    }
    {
      ez_vector<int> a;
      auto r = enumerate_rng(a);
      test_ritr(r, empty_rng<pair<ptrdiff_t, int>>());
    }
  }
}
void test_exclusive_rotate() {
  {
    int a[] = {1};
    int b[] = {0};
    assert(exclusive_rotate(a, nth(a, 0), begin(b)) == begin(b));
  }
  {
    int a[] = {1, 2, 3};
    int b[] = {0, 0};
    assert(exclusive_rotate(a, nth(a, 0), begin(b)) == end(b));
    assert(equal(b, seq(2, 3)));
    assert(exclusive_rotate(a, nth(a, 1), begin(b)) == end(b));
    assert(equal(b, seq(3, 1)));
    assert(exclusive_rotate(a, nth(a, 2), begin(b)) == end(b));
    assert(equal(b, seq(1, 2)));
    assert(exclusive_rotate(a, end(a), begin(b)) == end(b));
    assert(equal(b, seq(2, 3)));
  }
}
void test_exclusive_rotate_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = exclusive_rotate_iterator<int *>;
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = exclusive_rotate_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = exclusive_rotate_iterator
          <degraded_input_iterator<int *>>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = exclusive_rotate_iterator
          <degraded_forward_iterator<int *>>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = exclusive_rotate_iterator
          <degraded_bidirectional_iterator<int *>>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = exclusive_rotate_iterator<int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = exclusive_rotate_iterator<int *>;
      using c_iter_t = exclusive_rotate_iterator<const int *>;
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
  // iterator operations
  // exclusive_rotate_itr
  {
    {
      ez_vector<int> v;
      auto i = exclusive_rotate_itr(begin(v), begin(v), begin(v), begin(v));
      auto j = exclusive_rotate_itr(begin(v), begin(v), begin(v), begin(v));
      test_ritr(i, j, empty_rng<int>());
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = exclusive_rotate_itr(begin(v), nth(v, 0), end(v), nth(v, 0));
      auto j = exclusive_rotate_itr(begin(v), nth(v, 0), end(v), nth(v, 3));
      test_ritr(i, j, seq(1, 2, 3));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = exclusive_rotate_itr(begin(v), nth(v, 1), end(v), nth(v, 1));
      auto j = exclusive_rotate_itr(begin(v), nth(v, 1), end(v), nth(v, 0));
      test_ritr(i, j, seq(2, 3, 4));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = exclusive_rotate_itr(begin(v), nth(v, 2), end(v), nth(v, 2));
      auto j = exclusive_rotate_itr(begin(v), nth(v, 2), end(v), nth(v, 1));
      test_ritr(i, j, seq(3, 4, 1));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = exclusive_rotate_itr(begin(v), nth(v, 3), end(v), nth(v, 3));
      auto j = exclusive_rotate_itr(begin(v), nth(v, 3), end(v), nth(v, 2));
      test_ritr(i, j, seq(4, 1, 2));
    }
  }
}
void test_exclusive_rotate_range() {
  // compile-time info
  {
    using r1_t = exclusive_rotate_range<ez_forward_list<int>>;
    using r2_t = exclusive_rotate_range<ez_list<int>>;
    using r3_t = exclusive_rotate_range<ez_vector<int>>;
    using r4_t = exclusive_rotate_range<ez_vector<int> &>;
    using r5_t = exclusive_rotate_range<iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_brng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_rrng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_rrng<r4_t> && is_rrng<const r4_t>);
      static_assert(is_rrng<r5_t> && is_rrng<const r5_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t> && rng_is_sized<const r4_t>);
      static_assert(rng_is_sized<r5_t> && rng_is_sized<const r5_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = exclusive_rotate_range<ez_vector<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, ez_vector{4, 1, 2});};
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = exclusive_rotate_range<iter_pair<int *>>;
      ez_vector<int> v = {1, 2, 3, 4};
      const auto init_f1 = [&](r_t *p) {new(p) r_t(rng(v), 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, seq(4, 1, 2));};
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // range operations
  // exclusive_rotate_rng
  {
    {
      ez_vector<int> a;
      auto r = exclusive_rotate_rng(a, end(a));
      test_ritr(r, empty_rng<int>());
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = exclusive_rotate_rng(a, end(a));
      test_ritr(r, seq(2, 3, 4));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = exclusive_rotate_rng(a, nth(a, 1));
      test_ritr(r, seq(3, 4, 1));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = exclusive_rotate_rng(a, nth(a, 0));
      test_ritr(r, seq(2, 3, 4));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = exclusive_rotate_rng(a, nth(a, 3));
      test_ritr(r, seq(1, 2, 3));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = exclusive_rotate_rng(move(a), 3);
      test_ritr(r, seq(1, 2, 3));
    }
  }
}
void test_rotate_iterator() {
  // compile-time info
  {
    // member types
    {
      using iter_t = rotate_iterator<int *>;
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = rotate_iterator<int *>;

      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);

      static_assert(is_nothrow_default_constructible<iter_t>);
      static_assert(is_nothrow_copyable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = rotate_iterator
          <degraded_input_iterator<int *>>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = rotate_iterator
          <degraded_forward_iterator<int *>>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = rotate_iterator
          <degraded_bidirectional_iterator<int *>>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = rotate_iterator<int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = rotate_iterator<int *>;
      using c_iter_t = rotate_iterator<const int *>;
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
  // iterator operations
  // rotate_itr
  {
    {
      ez_vector<int> v;
      auto i = rotate_itr(begin(v), begin(v), begin(v), begin(v));
      auto j = rotate_itr(begin(v), begin(v), begin(v), begin(v));
      test_ritr(i, j, empty_rng<int>());
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = rotate_itr(begin(v), nth(v, 0), end(v), nth(v, 0));
      auto j = rotate_itr(begin(v), nth(v, 0), end(v), end(v));
      test_ritr(i, j, seq(1, 2, 3, 4));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = rotate_itr(begin(v), nth(v, 1), end(v), nth(v, 1));
      auto j = rotate_itr(begin(v), nth(v, 1), end(v), end(v));
      test_ritr(i, j, seq(2, 3, 4, 1));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = rotate_itr(begin(v), nth(v, 2), end(v), nth(v, 2));
      auto j = rotate_itr(begin(v), nth(v, 2), end(v), end(v));
      test_ritr(i, j, seq(3, 4, 1, 2));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      auto i = rotate_itr(begin(v), nth(v, 3), end(v), nth(v, 3));
      auto j = rotate_itr(begin(v), nth(v, 3), end(v), end(v));
      test_ritr(i, j, seq(4, 1, 2, 3));
    }
  }
}
void test_rotate_range() {
  // compile-time info
  {
    using r1_t = rotate_range<ez_forward_list<int>>;
    using r2_t = rotate_range<ez_list<int>>;
    using r3_t = rotate_range<ez_vector<int>>;
    using r4_t = rotate_range<ez_vector<int> &>;
    using r5_t = rotate_range<iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(semiregular<r5_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_brng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_rrng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_rrng<r4_t> && is_rrng<const r4_t>);
      static_assert(is_rrng<r5_t> && is_rrng<const r5_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t> && rng_is_sized<const r4_t>);
      static_assert(rng_is_sized<r5_t> && rng_is_sized<const r5_t>);
    }
  }
  // special member functions
  {
    {
      using r_t = rotate_range<ez_vector<int>>;
      const auto init_f1 = [](r_t *p) {new(p) r_t({1, 2, 3, 4}, 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, ez_vector{3, 4, 1, 2});};
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    {
      using r_t = rotate_range<iter_pair<int *>>;
      ez_vector<int> v = {1, 2, 3, 4};
      const auto init_f1 = [&](r_t *p) {new(p) r_t(rng(v), 2);};
      const auto eq_f1 = [](r_t *p) {return equal(*p, seq(3, 4, 1, 2));};
      const auto init_f2 = [](r_t *p) {new(p) r_t;};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // range operations
  // rotate_rng
  {
    {
      ez_vector<int> a;
      auto r = rotate_rng(a, end(a));
      test_ritr(r, empty_rng<int>());
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = rotate_rng(a, end(a));
      test_ritr(r, seq(1, 2, 3, 4));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = rotate_rng(a, nth(a, 1));
      test_ritr(r, seq(2, 3, 4, 1));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = rotate_rng(a, nth(a, 0));
      test_ritr(r, seq(1, 2, 3, 4));
    }
    {
      ez_vector<int> a = {1, 2, 3, 4};
      auto r = rotate_rng(a, nth(a, 3));
      test_ritr(r, seq(4, 1, 2, 3));
    }
    {
      auto r = rotate_rng(ez_vector{1, 2, 3, 4}, 2);
      test_ritr(r, seq(3, 4, 1, 2));
    }
  }
}
void test_loop() {
  ez_vector<int> v, vv;
  {
    v = {};
    vv.erase(vv.begin(), vv.end());
    assert(loop(v, nth(v, 0), 0, begin(vv)) == begin(vv));
    assert(loop(v, nth(v, 0), 100, begin(vv)) == begin(vv));
  }
  {
    v = {1};
    vv = {0, 0, 0};
    assert(loop(v, begin(v), 0, begin(vv)) == begin(vv));
    assert(loop(v, begin(v), 3, begin(vv)) == end(vv));
    assert(equal(vv, rng(3, 1)));
  }
  {
    v = {1, 2, 3};
    vv = {0, 0, 0, 0, 0, 0, 0};
    assert(loop(v, nth(v, 1), 7, begin(vv)) == end(vv));
    assert(equal(vv, seq(2, 3, 1, 2, 3, 1, 2)));
    assert(loop(v, end(v), 7, begin(vv)) == end(vv));
    assert(equal(vv, seq(1, 2, 3, 1, 2, 3, 1)));
  }
}
void test_loop_iterator() {
  // compile-time info
  {
    // member type
    {
      using iter_t = loop_iterator<int *>;
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as
                    <iter_t::iterator_category, random_access_iterator_tag>);
    }
    // special member functions
    {
      using iter_t = loop_iterator<int *>;

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
        using iter_t = loop_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = loop_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = loop_iterator<int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = loop_iterator<int *>;
      using c_iter_t = loop_iterator<const int *>;
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
  // iterator operations
  {
    {
      int a[] = {1, 2, 3, 4};
      auto i = loop_itr(begin(a), begin(a), begin(a), 0);
      auto j = loop_itr(begin(a), begin(a), begin(a), 0);
      test_ritr(i, j, empty_rng<int>());
    }
    {
      int a[] = {1, 2, 3, 4};
      auto i = loop_itr(begin(a), end(a), nth(a, 0), 0);
      auto j = loop_itr(begin(a), end(a), nth(a, 0), 4);
      test_ritr(i, j, irng(1, 5));
    }
    {
      int a[] = {1, 2, 3, 4};
      auto i = loop_itr(begin(a), end(a), nth(a, 2), 0);
      auto j = loop_itr(begin(a), end(a), nth(a, 0), 10);
      auto ci = loop_itr(cbegin(a), cend(a), nth(as_const(a), 2), 0);
      auto cj = loop_itr(cbegin(a), cend(a), nth(as_const(a), 0), 10);
      test_ritr(i, j, seq(3, 4, 1, 2, 3, 4, 1, 2, 3, 4));
      test_ritr(ci, j, seq(3, 4, 1, 2, 3, 4, 1, 2, 3, 4));
      test_ritr(i, cj, seq(3, 4, 1, 2, 3, 4, 1, 2, 3, 4));
      test_ritr(ci, cj, seq(3, 4, 1, 2, 3, 4, 1, 2, 3, 4));
    }
    {
      ez_forward_list<int> a = {1, 2, 3, 4};
      auto i = loop_itr(begin(a), end(a), begin(a), 0);
      auto j = loop_itr(begin(a), end(a), begin(a), 5);
      test_fitr(i, j, seq(1, 2, 3, 4, 1));
    }
  }
}
void test_loop_range() {
  // compile-time info
  {
    using r1_t = loop_range<ez_forward_list<int>>;
    using r2_t = loop_range<ez_list<int>>;
    using r3_t = loop_range<ez_vector<int>>;
    using r4_t = loop_range<ez_vector<int> &>;
    using r5_t = loop_range<iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);
      static_assert(semiregular<r5_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(is_nothrow_swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);
      static_assert(!is_swappable<r4_t>);
      static_assert(is_nothrow_swappable<r5_t>);

      static_assert(!is_nothrow_copy_assignable<r1_t>);
      static_assert(!is_nothrow_copy_assignable<r2_t>);
      static_assert(!is_nothrow_copy_assignable<r3_t>);
      static_assert(!is_copy_assignable<r4_t>);
      static_assert(is_nothrow_copy_assignable<r5_t>);
    }
    // is_rng
    {
      static_assert(is_frng<r1_t> && !is_rng<const r1_t>);
      static_assert(is_frng<r2_t> && !is_rng<const r2_t>);
      static_assert(is_frng<r3_t> && !is_rng<const r3_t>);
      static_assert(is_frng<r4_t> && is_rng<const r4_t>);
      static_assert(is_frng<r5_t> && is_rng<const r5_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
      static_assert(is_rng_ref<r5_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t> && rng_is_sized<const r4_t>);
      static_assert(rng_is_sized<r5_t> && rng_is_sized<const r5_t>);
    }
  }
  // special member functions
  {
    // rvalue
    {
      using l_t = ez_slist<int>;
      using r_t = loop_range<l_t>;
      const auto init_f1 = [](r_t *p) {
        new(p) r_t(l_t{1, 2, 3}, 1, 4);
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(2, 3, 1, 2)) && !p->empty() && p->size() == 4;
      };
      const auto init_f2 = [&](r_t *p) {
        new(p) r_t;
      };
      const auto eq_f2 = [&](r_t *p) {
        return p->empty() && p->size() == 0 && p->begin() == p->end();
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    // lvalue
    {
      using l_t = ez_slist<int>;
      using r_t = loop_range<l_t>;
      l_t l{1, 2, 3};
      const auto init_f1 = [&](r_t *p) {
        new(p) r_t(l, 1, 4);
      };
      const auto eq_f1 = [](r_t *p) {
        return equal(*p, seq(2, 3, 1, 2)) && !p->empty() && p->size() == 4;
      };
      const auto init_f2 = [&](r_t *p) {
        new(p) r_t(l, 2, 2);
      };
      const auto eq_f2 = [&](r_t *p) {
        return equal(*p, seq(3, 1)) && !p->empty() && p->size() == 2;
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // range operations
  // loop_rng
  {
    // as frng
    {
      {
        ez_forward_list<int> a = {1, 2, 3};
        auto r = loop_rng(a, 1, 3);
        test_fitr(r, seq(2, 3, 1));
        test_fitr(as_const(r), seq(2, 3, 1));
        test_fitr(begin(r), cend(r), seq(2, 3, 1));
        test_fitr(cbegin(r), end(r), seq(2, 3, 1));
        assert(r.size() == 3 && as_const(r).size() == 3);
        assert(!r.empty() && !as_const(r).empty());
      }
      {
        ez_vector<int> vv;
        ez_forward_list<int> l;
        for (int &i : iters(0, 5))
          for (int &j : iters(0, i + 1))
            for (int &k : iters(0, 16)) {
              l.erase_after(l.before_begin(), l.end());
              copy(irng(0, i), to_front(l));
              vv.erase(vv.begin(), vv.end());
              loop(l, nth(l, j), k, to_back(vv));
              auto r = loop_rng(l, nth(l, j), k);
              auto r2 = loop_rng(l, j, k);
              auto r3 = loop_rng(copy(l), j, k);
              test_fitr(r, vv);
              test_fitr(as_const(r), vv);
              test_fitr(r2, vv);
              test_fitr(as_const(r2), vv);
              test_fitr(r3, vv);
            }
      }
    }
    // as brng
    {
      {
        ez_list<int> a = {1, 2, 3};
        auto r = loop_rng(a, 1, 3);
        test_bitr(r, seq(2, 3, 1));
        test_bitr(as_const(r), seq(2, 3, 1));
        test_bitr(begin(r), cend(r), seq(2, 3, 1));
        test_bitr(cbegin(r), end(r), seq(2, 3, 1));
        assert(r.size() == 3 && as_const(r).size() == 3);
        assert(!r.empty() && !as_const(r).empty());
      }
      {
        ez_vector<int> vv;
        ez_list<int> l;
        for (int &i : iters(0, 5))
          for (int &j : iters(0, i + 1))
            for (int &k : iters(0, 16)) {
              l.erase(l.begin(), l.end());
              copy(irng(0, i), to_back(l));
              vv.erase(vv.begin(), vv.end());
              loop(l, nth(l, j), k, to_back(vv));
              auto r = loop_rng(l, nth(l, j), k);
              auto r2 = loop_rng(l, j, k);
              auto r3 = loop_rng(copy(l), j, k);
              test_bitr(r, vv);
              test_bitr(as_const(r), vv);
              test_bitr(r2, vv);
              test_bitr(as_const(r2), vv);
              test_bitr(r3, vv);
            }
      }
    }
    // as rrng
    {
      {
        int a[] = {1, 2, 3};
        auto r = loop_rng(a, 1, 3);
        test_ritr(r, seq(2, 3, 1));
        test_ritr(as_const(r), seq(2, 3, 1));
        test_ritr(begin(r), cend(r), seq(2, 3, 1));
        test_ritr(cbegin(r), end(r), seq(2, 3, 1));
        assert(r.size() == 3 && as_const(r).size() == 3);
        assert(!r.empty() && !as_const(r).empty());
      }
      {
        ez_vector<int> vv;
        ez_vector<int> l;
        for (int &i : iters(0, 5))
          for (int &j : iters(0, i + 1))
            for (int &k : iters(0, 16)) {
              l.erase(l.begin(), l.end());
              copy(irng(0, i), to_back(l));
              vv.erase(vv.begin(), vv.end());
              loop(l, nth(l, j), k, to_back(vv));
              auto r = loop_rng(l, nth(l, j), k);
              auto r2 = loop_rng(l, j, k);
              auto r3 = loop_rng(copy(l), j, k);
              test_ritr(r, vv);
              test_ritr(as_const(r), vv);
              test_ritr(r2, vv);
              test_ritr(as_const(r2), vv);
              test_ritr(r3, vv);
            }
      }
    }
  }
}
void test_concat_range() {
  // 1
  {
    using a_t = int [3];
    int a[] = {1, 2, 3, 4, 5};
    auto r = concat_rng(a);
    static_assert(is_crng<decltype(r)>);
    test_rng(r, irng(1, 6));
    test_ritr(r.begin(), r.end(), a);
    const auto &c_r = r;
    test_rng(c_r, irng(1, 6));
    test_ritr(c_r.begin(), c_r.end(), a);

    ez_vector<int> v;
    auto r2 = concat_rng(v);
    static_assert(is_crng<decltype(r2)>);
    test_rng(r2, empty_rng<int>());
    test_ritr(r2.begin(), r2.end(), empty_rng<int>());
    const auto &c_r2 = r2;
    test_rng(c_r2, empty_rng<int>());
    test_ritr(c_r2.begin(), c_r2.end(), empty_rng<int>());
  }
  // 2
  {
    ez_vector<int> v_cmp;
    ez_vector<int> v1;
    ez_vector<int> v2;
    for (int i : iters(0, 4))
      for (int j : iters(0, 4)) {
        v1.erase(v1.begin(), v1.end());
        v2.erase(v2.begin(), v2.end());
        v_cmp.erase(v_cmp.begin(), v_cmp.end());
        for (int k : irng(0, i)) {
          v1.insert(v1.end(), k);
          v_cmp.insert(v_cmp.end(), k);
        }
        for (int k : irng(0, j)) {
          v2.insert(v2.end(), k);
          v_cmp.insert(v_cmp.end(), k);
        }
        auto r = concat_rng(v1, v2);
        static_assert(is_just_rrng<decltype(r)>);
        test_rng(r, v_cmp);
        test_ritr(r.begin(), r.end(), v_cmp);
        const auto &c_r = r;
        test_rng(c_r, v_cmp);
        test_ritr(c_r.begin(), r.end(), v_cmp);
      }
  }
  // 3
  {
    ez_vector<double> v_cmp;
    ez_vector<int> v1;
    ez_list<double> v2;
    ez_forward_list<float> v3;
    for (int i1 : iters(0, 4))
      for (int i2 : iters(0, 4))
        for (int i3 : iters(0, 4)) {
          v1.erase(v1.begin(), v1.end());
          v2.erase(v2.begin(), v2.end());
          v3.erase_after(v3.before_begin(), v3.end());
          v_cmp.erase(v_cmp.begin(), v_cmp.end());
          for (int k : irng(0, i1)) {
            v1.insert(v1.end(), k);
            v_cmp.insert(v_cmp.end(), k);
          }
          for (int k : irng(0, i2)) {
            v2.insert(v2.end(), k);
            v_cmp.insert(v_cmp.end(), k);
          }
          auto v3_it = v3.before_begin();
          for (int k : irng(0, i3)) {
            v3_it = v3.insert_after(v3_it, k);
            v_cmp.insert(v_cmp.end(), k);
          }
          auto r = concat_rng(v1, v2, v3);
          static_assert(is_just_frng<decltype(r)>);
          static_assert(is_same<rng_ref<decltype(r)>, double>);
          test_rng(r, v_cmp);
          test_fitr(r.begin(), r.end(), v_cmp);
          const auto &c_r = r;
          test_rng(c_r, v_cmp);
          test_fitr(c_r.begin(), c_r.end(), v_cmp);
        }
  }
  // 5
  {
    ez_vector<int> v_cmp;
    ez_vector<int> i_v;
    ez_vector<ez_vector<int>> vv = {{}, {}, {}, {}, {}};
    assert(vv.size() == 5u);
    
    for (int i1 : iters(0, 4))
      for (int i2 : iters(0, 4))
        for (int i3 : iters(0, 4))
          for (int i4 : iters(0, 4))
            for (int i5 : iters(0, 4)) {
              v_cmp.erase(v_cmp.begin(), v_cmp.end());
              i_v.erase(i_v.begin(), i_v.end());
              for (auto &v : vv)
                v.erase(v.begin(), v.end());

              i_v.insert(i_v.end(), i1);
              i_v.insert(i_v.end(), i2);
              i_v.insert(i_v.end(), i3);
              i_v.insert(i_v.end(), i4);
              i_v.insert(i_v.end(), i5);
              for (auto it : irng(i_v)) {
                auto &v = ref(vv, it - i_v.begin());
                for (int i : iters(0, *it)) {
                  v.insert(v.end(), i);
                  v_cmp.insert(v_cmp.end(), i);
                }
              }

              auto r = concat_rng(ref(vv, 0), ref(vv, 1), ref(vv, 2),
                                  ref(vv, 3), ref(vv, 4));
              using r_t = decltype(r);
              static_assert(is_just_rrng<r_t>);
              static_assert(is_same<rng_ref<r_t>, int &>);
              test_rng(r, v_cmp);
              test_ritr(r.begin(), r.end(), v_cmp);
              const auto &c_r = r;
              test_rng(c_r, v_cmp);
              test_ritr(c_r.begin(), c_r.end(), v_cmp);
              test_ritr(r.begin(), c_r.end(), v_cmp);
              test_ritr(c_r.begin(), r.end(), v_cmp);
            }
  }
}

void test_algorithm_by_range_3() {
  printf("algorithm_by_range_3: ");

  test_split();
  test_split_iterator();
  test_split_range();
  test_zip();
  test_zip_iterator();
  test_zip_range();
  test_aligned_zip_iterator();
  test_aligned_zip_range();
  test_enumerate();
  test_enumerate_iterator();
  test_enumerate_range();
  test_exclusive_rotate();
  test_exclusive_rotate_iterator();
  test_exclusive_rotate_range();
  test_rotate_iterator();
  test_rotate_range();
  test_loop();
  test_loop_iterator();
  test_loop_range();
  test_concat_range();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_algorithm_by_range_3();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
