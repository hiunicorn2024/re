#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>

#include <cassert>

namespace re::inner::test {

void test_permutation_combination_count() {
  // permutation_count
  // combination_count
  {
    assert(permutation_count(0) == 1);
    assert(permutation_count(1) == 1);
    assert(permutation_count(2) == 2);
    assert(permutation_count(3) == 6);
    assert(permutation_count(5) == 5 * 4 * 3 * 2);
    assert(permutation_count(1, 0) == 1);
    assert(permutation_count(3, 0) == 1);
    assert(permutation_count(5, 0) == 1);
    assert(permutation_count(5, 1) == 5);
    assert(permutation_count(5, 3) == 5 * 4 * 3);
    assert(permutation_count(5, 5) == 5 * 4 * 3 * 2);

    assert(combination_count(0, 0) == 1);
    assert(combination_count(1, 0) == 1);
    assert(combination_count(1, 1) == 1);
    assert(combination_count(3, 0) == 1);
    assert(combination_count(3, 1) == 3);
    assert(combination_count(3, 2) == 3);
    assert(combination_count(5, 0) == 1);
    assert(combination_count(5, 5) == 1);
    assert(combination_count(5, 3) == 10);
    assert(combination_count(5, 4) == 5);
    assert(combination_count(9, 4)
           == permutation_count(9, 4) / permutation_count(4));
  }
}
void test_combination_iterator() {
  // compile-time info
  {
    // special member functions
    {
      using iter_t = combination_iterator<int *, ez_vector<int>>;
      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
      static_assert(is_trivially_copyable<iter_t>);
    }
    // member types
    {
      using iter_t = combination_iterator<int *, ez_vector<int>>;
      static_assert(same_as
                    <iter_t::value_type,
                     bind_range<ez_vector<int> &, decay<decltype(deref)>>>);
      static_assert(same_as<iter_t::reference, iter_t::value_type>);
      static_assert(same_as<iter_t::pointer, void>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::iterator_category, input_iterator_tag>);
      static_assert(semiregular<iter_t> && swappable<iter_t>);
    }
    // iterator concept
    {
      using iter_t = combination_iterator<int *, ez_vector<int>>;
      static_assert(input_iterator<iter_t>);
      static_assert(!forward_iterator<iter_t>);
      static_assert(cpp17_input_iterator<iter_t>);
      static_assert(!cpp17_forward_iterator<iter_t>);
    }
  }
  // special member functions
  // combination_iterator(bufp, r_ed)
  // iterator operations
  {
    using v_iter_t = ez_vector<int>::iterator;
    using iter_t = combination_iterator<v_iter_t, ez_vector<v_iter_t>>;
    using r_t = iter_pair<iter_t>;
    using init_t = ez_function<void (r_t *)>;
    using eq_t = ez_function<bool (r_t *)>;
    ez_vector<pair<init_t, eq_t>> fns;

    // default construct iter_pair
    {
      init_t x = [](r_t *p) {new(p) r_t;};
      eq_t y = [](const r_t *p) {
        return test_equal((*p).first, (*p).first)
          && test_equal((*p).second, (*p).second)
          && test_equal((*p).first, (*p).second);
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 1 from 1
    ez_vector<int> v11 = {1};
    ez_vector<int *> l11 = {begin(v11)};
    {
      init_t x = [&v11, &l11](r_t *p) {
        new(p) r_t(iter_t(addressof(l11), end(v11)),
                   iter_t(nullptr, end(v11)));
      };
      eq_t y = [&v11, &l11](r_t *p) {
        if (equal(*p, single_rng(irng(1, 2)), equal)) {
          if (l11 == ez_vector<int *>({end(v11)})) {
            l11 = {begin(v11)};
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 1 from 5
    ez_vector<int> v1 = {1, 2, 3, 4, 5};
    ez_vector<int *> l1 = {begin(v1)};
    {
      init_t x = [&v1, &l1](r_t *p) {
        new(p) r_t(iter_t(addressof(l1), end(v1)),
                   iter_t(nullptr, end(v1)));
      };
      eq_t y = [&v1, &l1](r_t *p) {
        if (equal(*p, bind_rng(irng(1, 6), bind(single_rng, _1)), equal)) {
          if (l1 == ez_vector<int *>({end(v1)})) {
            l1 = {begin(v1)};
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 2 from 5
    ez_vector<int> v2 = {1, 2, 3, 4, 5};
    const ez_vector<int *> cmp2_start = {nth(v2, 0), nth(v2, 1)};
    const ez_vector<int *> cmp2_final = {nth(v2, 4), nth(v2, 5)};
    ez_vector<int *> l2 = cmp2_start;
    const ez_vector<ez_vector<int>> cmp2 = {
      {1, 2}, {1, 3}, {1, 4}, {1, 5},
      {2, 3}, {2, 4}, {2, 5},
      {3, 4}, {3, 5},
      {4, 5}
    };
    {
      init_t x = [&v2, &l2](r_t *p) {
        new(p) r_t(iter_t(addressof(l2), end(v2)),
                   iter_t(nullptr, end(v2)));
      };
      eq_t y = [&v2, &l2, &cmp2_start, &cmp2_final, &cmp2](r_t *p) {
        if (equal(*p, cmp2, equal)) {
          if (l2 == cmp2_final) {
            l2 = cmp2_start;
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 3 from 5
    ez_vector<int> v3 = {1, 2, 3, 4, 5};
    const ez_vector<int *> cmp3_start = {nth(v3, 0), nth(v3, 1), nth(v3, 2)};
    const ez_vector<int *> cmp3_final = {nth(v3, 3), nth(v3, 4), nth(v3, 5)};
    ez_vector<int *> l3 = cmp3_start;
    const ez_vector<ez_vector<int>> cmp3 = {
      {1, 2, 3}, {1, 2, 4}, {1, 2, 5},
      {1, 3, 4}, {1, 3, 5}, {1, 4, 5},
      {2, 3, 4}, {2, 3, 5}, {2, 4, 5},
      {3, 4, 5}
    };
    {
      init_t x = [&v3, &l3](r_t *p) {
        new(p) r_t(iter_t(addressof(l3), end(v3)),
                   iter_t(nullptr, end(v3)));
      };
      eq_t y = [&v3, &l3, &cmp3_start, &cmp3_final, &cmp3](r_t *p) {
        if (equal(*p, cmp3, equal)) {
          if (l3 == cmp3_final) {
            l3 = cmp3_start;
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 4 from 5
    ez_vector<int> v4 = {1, 2, 3, 4, 5};
    const ez_vector<int *> cmp4_start = {
      nth(v4, 0), nth(v4, 1), nth(v4, 2), nth(v4, 3)
    };
    const ez_vector<int *> cmp4_final = {
      nth(v4, 2), nth(v4, 3), nth(v4, 4), nth(v4, 5)
    };
    ez_vector<int *> l4 = cmp4_start;
    const ez_vector<ez_vector<int>> cmp4 = {
      {1, 2, 3, 4},
      {1, 2, 3, 5},
      {1, 2, 4, 5},
      {1, 3, 4, 5},
      {2, 3, 4, 5}
    };
    {
      init_t x = [&v4, &l4](r_t *p) {
        new(p) r_t(iter_t(addressof(l4), end(v4)),
                   iter_t(nullptr, end(v4)));
      };
      eq_t y = [&v4, &l4, &cmp4_start, &cmp4_final, &cmp4](r_t *p) {
        if (equal(*p, cmp4, equal)) {
          if (l4 == cmp4_final) {
            l4 = cmp4_start;
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    // select 5 from 5
    ez_vector<int> v5 = {1, 2, 3, 4, 5};
    const ez_vector<int *> cmp5_start = {
      nth(v5, 0), nth(v5, 1), nth(v5, 2), nth(v5, 3), nth(v5, 4)
    };
    const ez_vector<int *> cmp5_final = {
      nth(v5, 1), nth(v5, 2), nth(v5, 3), nth(v5, 4), nth(v5, 5)
    };
    ez_vector<int *> l5 = cmp5_start;
    const ez_vector<ez_vector<int>> cmp5 = {{1, 2, 3, 4, 5}};
    {
      init_t x = [&v5, &l5](r_t *p) {
        new(p) r_t(iter_t(addressof(l5), end(v5)),
                   iter_t(nullptr, end(v5)));
      };
      eq_t y = [&v5, &l5, &cmp5_start, &cmp5_final, &cmp5](r_t *p) {
        if (equal(*p, cmp5, equal)) {
          if (l5 == cmp5_final) {
            l5 = cmp5_start;
            return true;
          }
        }
        return false;
      };
      fns.insert(fns.end(), pair(move(x), move(y)));
    }

    for (auto &x : fns)
      for (auto &y : fns) {
        test_ownership<r_t>(x.first, x.second, y.first, y.second,
                            [](const r_t *) {return true;});
      }
  }
}
void test_combination_range() {
  // compile-time info
  {
    using r_t = combination_range<ez_list<int>>;
    using r2_t = combination_range<ez_list<int> &>;
    using r3_t = combination_range<iter_pair<ez_list<int>::iterator>>;

    // special member functions
    {
      static_assert(semiregular<r_t>);
      static_assert(!is_trivially_copyable<r_t>);

      static_assert(!semiregular<r2_t>);
      static_assert(!is_trivially_copyable<r2_t>);
      static_assert(!is_default_constructible<r2_t>);
      static_assert(is_copy_constructible<r2_t>);
      static_assert(is_move_constructible<r2_t>);
      static_assert(!is_copy_assignable<r2_t>);
      static_assert(!is_move_assignable<r2_t>);

      static_assert(semiregular<r3_t>);
      static_assert(!is_trivially_copyable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r_t>);
      static_assert(!is_rng<const r_t>);

      static_assert(is_rng<r2_t>);
      static_assert(!is_rng<const r2_t>);

      static_assert(is_rng<r3_t>);
      static_assert(!is_rng<const r3_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r_t>);
      static_assert(!is_rng_ref<r_t &&>);
      static_assert(is_rng_ref<r_t &>);
      static_assert(!is_rng_ref<const r_t &>);
      static_assert(!is_rng_ref<r_t &&>);
      static_assert(!is_rng_ref<const r_t &&>);

      static_assert(!is_rng_ref<r2_t>);
      static_assert(!is_rng_ref<const r2_t>);

      static_assert(!is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<const r3_t>);
    }
  }

  // default construct
  // construct by (r, n)
  // special member functions
  // range operations
  {
    // for value
    {
      using v_t = ez_vector<int>;
      using r_t = combination_range<v_t>;

      using init_t = ez_function<void (r_t *)>;
      using eq_t = ez_function<bool (r_t *)>;
      ez_vector<pair<init_t, eq_t>> fns;

      // select 0 from 0
      {
        init_t x = [](r_t *p) {
          new(p) r_t;
        };
        eq_t y = [](r_t *p) {
          return (*p).empty() && (*p).begin() == (*p).end();
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{}, 0);
        };
        eq_t y = [](r_t *p) {
          return (*p).empty() && (*p).begin() == (*p).end();
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      // select 0 from 4
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{1, 2, 3, 4}, 0);
        };
        eq_t y = [](r_t *p) {
          return (*p).empty() && (*p).begin() == (*p).end();
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      // select 2 from 4
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{1, 2, 3, 4}, 2);
        };
        eq_t y = [](r_t *p) {
          const auto cmp_v = ez_vector{
            v_t{1, 2}, v_t{1, 3}, v_t{1, 4},
            v_t{2, 3}, v_t{2, 4}, v_t{3, 4}
          };
          if (!(*p).empty() && equal(*p, cmp_v, equal)) {
            if ((*p).empty()) {
              *p = *p;
              if (equal(*p, cmp_v, equal)) {
                *p = *p;
                return true;
              }
            }
          }
          return false;
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      // select 4 from 4
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{1, 2, 3, 4}, 4);
        };
        eq_t y = [](r_t *p) {
          const auto cmp_v = single_rng(v_t{1, 2, 3, 4});
          if (!(*p).empty() && equal(*p, cmp_v, equal)) {
            if ((*p).empty()) {
              *p = move(*p);
              if (equal(*p, cmp_v, equal)) {
                adl_swap(*p, *p);
                return true;
              }
            }
          }
          return false;
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      // select 5 from 4
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{1, 2, 3, 4}, 5);
        };
        eq_t y = [](r_t *p) {
          const auto cmp_v = single_rng(v_t{1, 2, 3, 4});
          return (*p).empty() && (*p).begin() == (*p).end();
          return false;
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }
      // select 4 from 9
      {
        init_t x = [](r_t *p) {
          new(p) r_t(v_t{1, 2, 3, 4, 5, 6, 7, 8, 9}, 4);
        };
        eq_t y = [](r_t *p) {
          int a[4] = {};
          int c = 0;
          for (decltype(auto) x : *p) {
            static_assert(!is_reference<decltype(x)>);
            static_assert(!is_reference<decltype(x)>);
            ++c;
            copy(const_rng(x), begin(a));
          }
          if (combination_count(9, 4) == c
              && (*p).empty() && (*p).begin() == (*p).end()) {
            *p = *p;
            return true;
          }
          return false;
        };
        fns.insert(fns.end(), pair(move(x), move(y)));
      }

      // test each pair combination selected from above situations
      for (auto &x : fns)
        for (auto &y : fns)
          test_ownership<r_t>(x.first, x.second, y.first, y.second,
                              [](r_t *p) {
                                return (*p).empty()
                                  && (*p).begin() == (*p).end();
                              });
    }

    // for reference (briefly)
    {
      using l_t = ez_list<int>;
      using r_t = combination_range<l_t &>;

      l_t l = {1, 2, 3, 4, 5};
      const auto eq = [&l](r_t &x) {
        return equal(bind_rng(x, bind(bind_rng, _1, addressof)),
                     bind_rng(irng(0, 5),
                              bind(single_rng,
                                   bind(to_address,
                                        bind(nth, ref(l), _1)))),
                     equal);
      };
      const auto empt = [](r_t &x) {
        return x.empty() && x.begin() == x.end();
      };
      r_t r(l, 1);
      assert(eq(r) && empt(r));
      r_t r2(r);
      assert(empt(r) && eq(r2) && empt(r2));
    }

    // for iter_pair (briefly)
    {
      using l_t = ez_list<int>;
      using r_t = combination_range<iter_pair<l_t::iterator>>;

      l_t l = {1, 2, 3, 4, 5};
      const auto mk1 = [&l](r_t *p) {
        new(p) r_t(rng(l), 1);
      };
      const auto eq1 = [&l](r_t *p) {
        if (equal(bind_rng(*p, bind(bind_rng, _1, addressof)),
                  bind_rng(irng(0, 5),
                           bind(single_rng,
                                bind(to_address,
                                     bind(nth, ref(l), _1)))),
                  equal)) {
          adl_swap(*p, *p);
          return true;
        }
        return false;
      };
      const auto mk2 = [&l](r_t *p) {
        new(p) r_t(rng(l), 5);
      };
      const auto eq2 = [&l](r_t *p) {
        if (equal(bind_rng(*p, bind(bind_rng, _1, addressof)),
                  single_rng(bind_rng(l, addressof)),
                  equal)) {
          adl_swap(*p, *p);
          return true;
        }
        return false;
      };
      const auto empt = [](r_t *) {return true;};
      test_ownership<r_t>(mk1, eq1, mk2, eq2, empt);
    }
  }

  // combination_rng
  {
    assert(equal(combination_rng(empty_rng<int>(), 0),
                 empty_rng<empty_range<int>>(),
                 equal));

    assert(equal(combination_rng(ez_vector{1, 2, 3}, 3),
                 single_rng(ez_vector{1, 2, 3}),
                 equal));
    ez_vector v = {1, 2, 3, 4};
    assert(equal(combination_rng(rng(v), 3),
                 ez_vector{
                   ez_vector{1, 2, 3},
                   ez_vector{1, 2, 4},
                   ez_vector{1, 3, 4},
                   ez_vector{2, 3, 4},
                 },
                 equal));
  }

  // combination
  {
    ez_vector<int> v;
    ez_vector<ez_vector<int>> vv, vv2;

    for (int i : iters(0, 7))
      for (int j : iters(0, i)) {
        v.erase(v.begin(), v.end());
        vv.erase(vv.begin(), vv.end());
        vv2.erase(vv2.begin(), vv2.end());
        copy(irng(0, i), to_back(v));

        combination(v, j, output_itr([&vv](auto &r) {
          ez_vector<int> tmp;
          copy(r, to_back(tmp));
          vv.insert(vv.end(), move(tmp));
        }));
        copy(combination_rng(v, j), output_itr([&vv2](auto &&r) {
          ez_vector<int> tmp;
          copy(r, to_back(tmp));
          vv2.insert(vv2.end(), move(tmp));
        }));
        assert(vv == vv2);
      }
  }
}
void test_filter() {
  ez_vector<int> v;
  copy(rng(2, 0), to_back(v));
  assert(filter_plus(irng(0, 5), begin(v),
                     [](int x) {return x == 2 || x == 3;})
         == pair(iitr(5), end(v)));
  assert(equal(v, irng(2, 4)));
  assert(filter_plus(irng(0, 5), begin(v), [](int x) {return false;})
         == pair(iitr(5), begin(v)));
  assert(equal(v, irng(2, 4)));
  assert(filter_plus(irng(0, 0), begin(v), [](int x) {return false;})
         == pair(iitr(0), begin(v)));
  assert(equal(v, irng(2, 4)));

  assert(filter(irng(0, 5), begin(v),
                [](int x) {return x == 1 || x == 3;})
         == end(v));
  assert(equal(v, ez_vector{1, 3}));
}
void test_filter_iterator() {
  // compile-time info
  {
    using iter_t = filter_iterator<int *, bool (*)(int)>;
    static_assert(same_as<iter_t::value_type, int>);
    static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
    static_assert(same_as<iter_t::reference, int &>);
    static_assert(same_as<iter_t::pointer, int *>);
    static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);

    using iter2_t = filter_iterator
      <combination_iterator<int *, copyable_array<int *>>,
       bool (*)(const copyable_array<int *> &)>;
    static_assert(same_as<iter2_t::iterator_category, input_iterator_tag>);
    static_assert(regular<iter2_t>);
    static_assert(input_iterator<iter2_t>);

    static_assert(regular<iter_t>);
    static_assert(forward_iterator<iter_t>);
    static_assert(cpp17_forward_iterator<iter_t>);

    static_assert(is_convertible
                  <filter_iterator<int *, bool (*)(int)>,
                   filter_iterator<const int *, bool (*)(int)>>);
    static_assert(!is_convertible
                  <filter_iterator<const int *, bool (*)(int)>,
                   filter_iterator<int *, bool (*)(int)>>);
    
  }

  // swap
  // construct by stored data
  // construct by this_tmpl<...>
  // base
  // fn
  {
    int a[] = {1, 0, 2, 0};
    filter_iterator<int *, bool (*)(int)> i(begin(a), end(a),
                                            [](int x) {return x != 0;});
    filter_iterator<int *, bool (*)(int)> j(end(a), nullptr, nullptr);
    assert(i.base() == begin(a));
    assert(j.base() == end(a));
    assert(!i.fn()(0) && i.fn()(1));
    assert(j.fn() == nullptr);

    assert(equal(rng(i, j), irng(1, 3)));
    adl_swap(i, j);
    assert(equal(rng(j, i), irng(1, 3)));
    adl_swap(i, j);
    assert(equal(rng(i, j), irng(1, 3)));

    filter_iterator<const int *, bool (*)(int)> i2 = i;
    assert(equal(rng(i2, j), irng(1, 3)));
  }

  // filter_itr
  // iterator operations
  {
    int a[] = {1, 0, 2, 0, 3, 0, 4};
    using iter_t = filter_iterator<int *, bool (*)(int)>;
    using citer_t = filter_iterator<const int *, bool (*)(int)>;
    test_fitr(rng(iter_t(begin(a), end(a), [](int x) {return x != 0;}),
                  iter_t(end(a), nullptr, nullptr)),
              irng(1, 5));
    test_fitr(rng(iter_t(begin(a), end(a), [](int x) {return x != 0;}),
                  citer_t(end(a), nullptr, nullptr)),
              irng(1, 5));
    test_fitr(rng(citer_t(begin(a), end(a), [](int x) {return x != 0;}),
                  iter_t(end(a), nullptr, nullptr)),
              irng(1, 5));
    test_fitr(rng(citer_t(begin(a), end(a), [](int x) {return x != 0;}),
                  citer_t(end(a), nullptr, nullptr)),
              irng(1, 5));
  }
  {
    using iter_t = filter_iterator<ez_list<int>::iterator, bool (*)(int)>;
    for (int n : iters(1, 4)) {
      for (decltype(auto) x : combination_rng(irng(0, 6), 2)) {
        ez_list<int> l = {1, 2, 3, 4, 5};
        const auto i1 = nth(l, ref(x, 0));
        const auto i2 = nth(l, ref(x, 1));
        copy(rng(n, 0), inserter(l, i1));
        copy(rng(n, 0), inserter(l, i2));
        test_fitr(filter_rng(l, bind(not_equal_to{}, _1, 0)), irng(1, 6));
      }
    }
  }
}
void test_filter_range() {
  // compile-time info
  {
    using r_t = filter_range<ez_list<int>, bool (*)(int)>;
    static_assert(semiregular<r_t>);
    static_assert(!is_trivially_copyable<r_t>);
    static_assert(!is_rng_ref<r_t>);

    using r2_t = filter_range<ez_list<int> &, bool (*)(int)>;
    static_assert(!semiregular<r2_t>);
    static_assert(!is_default_constructible<r2_t>);
    static_assert(is_copy_constructible<r2_t>);
    static_assert(is_move_constructible<r2_t>);
    static_assert(!is_copy_assignable<r2_t>);
    static_assert(!is_move_assignable<r2_t>);
    static_assert(!is_trivially_copyable<r2_t>);
    static_assert(is_rng_ref<r2_t>);

    using r3_t = filter_range<iter_pair<int *>, bool (*)(int)>;
    static_assert(semiregular<r3_t>);
    static_assert(!is_trivially_copyable<r3_t>);
    static_assert(is_rng_ref<r3_t>);

    int tmp{};
    using r4_t = filter_range
      <iter_pair<int *>,
       semiregular_function<decltype([tmp](int) {return 0;})>>;
    static_assert(semiregular<r4_t>);
    static_assert(!is_trivially_copyable<r4_t>);
    static_assert(is_rng_ref<r4_t>);
  }
  // special member functions
  {
    const ez_vector<int> v = {0, 0, 1, 0, 2, 0, 3, 0};
    const ez_vector<int> v2 = {0, 4, 5, 6, 7};
    const auto f1 = [](int x) {return x != 0;};
    const int tmp = 0;
    const auto f2 = [tmp](int x) {return x != tmp;};
    const auto f3 = [&tmp](int x) {return x != tmp;};
    // for iter_pair
    {
      using t0 = filter_range<iter_pair<const int *>, decltype(f1)>;
      static_assert(!semiregular<t0>);
      static_assert(!is_copy_assignable<t0>);
      static_assert(!is_move_assignable<t0>);
      using t1 = filter_range<iter_pair<const int *>, decay<decltype(f1)>>;
      static_assert(semiregular<t1>);
      using t2 = filter_range<iter_pair<const int *>, decay<decltype(f2)>>;
      static_assert(!semiregular<t2>);
      static_assert(!is_default_constructible<t2>);
      using t3 = filter_range<iter_pair<const int *>, decay<decltype(f3)>>;
      static_assert(!semiregular<t3>);
      static_assert(!is_default_constructible<t3>);

      test_copy_construct<t0>([&](t0 *p) {new(p) t0(rng(v), f1);},
                              [](const t0 *p) {return equal(*p, irng(1, 4));},
                              [](const t0 *) {return true;});
      test_ownership<t1>
        ([&](t1 *p) {new(p) t1(rng(v), f1);},
         [](const t1 *p) {return equal(*p, irng(1, 4));},
         [&](t1 *p) {new(p) t1(rng(v2), f1);},
         [](const t1 *p) {return equal(*p, irng(4, 8));},
         [](const t1 *) {return true;});
      test_ownership<t2>
        ([&](t2 *p) {new(p) t2(rng(v), f2);},
         [](const t2 *p) {return equal(*p, irng(1, 4));},
         [&](t2 *p) {new(p) t2(rng(v2), f2);},
         [](const t2 *p) {return equal(*p, irng(4, 8));},
         [](const t2 *) {return true;});
      test_ownership<t3>
        ([&](t3 *p) {new(p) t3(rng(v), f3);},
         [](const t3 *p) {return equal(*p, irng(1, 4));},
         [&](t3 *p) {new(p) t3(rng(v2), f3);},
         [](const t3 *p) {return equal(*p, irng(4, 8));},
         [](const t3 *) {return true;});
    }
    // for reference
    {
      using t0 = filter_range<const ez_vector<int> &, decltype(f1)>;
      static_assert(!semiregular<t0>);
      using t1 = filter_range<const ez_vector<int> &, decay<decltype(f1)>>;
      static_assert(!semiregular<t1>);
      static_assert(!is_default_constructible<t1>);
      static_assert(!is_copy_assignable<t1>);
      static_assert(!is_move_assignable<t1>);
      using t2 = filter_range<const ez_vector<int> &, decay<decltype(f2)>>;
      static_assert(!semiregular<t2>);
      static_assert(!is_default_constructible<t2>);
      static_assert(!is_copy_assignable<t2>);
      static_assert(!is_move_assignable<t2>);
      using t3 = filter_range<const ez_vector<int> &, decay<decltype(f3)>>;
      static_assert(!semiregular<t3>);
      static_assert(!is_default_constructible<t2>);
      static_assert(!is_copy_assignable<t2>);
      static_assert(!is_move_assignable<t2>);

      test_copy_construct<t0>([&](t0 *p) {new(p) t0(v, f1);},
                              [](const t0 *p) {return equal(*p, irng(1, 4));},
                              [](const t0 *) {return true;});
      test_copy_construct<t1>
        ([&](t1 *p) {new(p) t1(v, f1);},
         [](const t1 *p) {return equal(*p, irng(1, 4));},
         [](const t1 *) {return true;});
      test_copy_construct<t2>
        ([&](t2 *p) {new(p) t2(v, f2);},
         [](const t2 *p) {return equal(*p, irng(1, 4));},
         [](const t2 *) {return true;});
      test_copy_construct<t3>
        ([&](t3 *p) {new(p) t3(v, f3);},
         [](const t3 *p) {return equal(*p, irng(1, 4));},
         [](const t3 *) {return true;});
    }
    // for value
    {
      using t0 = filter_range<ez_vector<int>, decltype(f1)>;
      static_assert(!semiregular<t0>);
      static_assert(is_default_constructible<t0>);
      static_assert(is_copy_constructible<t0>);
      static_assert(is_move_constructible<t0>);
      static_assert(!is_copy_assignable<t0>);
      static_assert(!is_move_assignable<t0>);
      using t1 = filter_range<ez_vector<int>, decay<decltype(f1)>>;
      static_assert(semiregular<t1>);
      using t2 = filter_range<ez_vector<int>, decay<decltype(f2)>>;
      static_assert(!semiregular<t2>);
      static_assert(!is_default_constructible<t2>);
      using t3 = filter_range<ez_vector<int>, decay<decltype(f3)>>;
      static_assert(!semiregular<t3>);
      static_assert(!is_default_constructible<t3>);

      test_rng(t0{}, empty_rng<int>());

      test_copy_construct<t0>([&](t0 *p) {new(p) t0(v, f1);},
                              [](t0 *p) {return equal(*p, irng(1, 4));},
                              [](t0 *p) {return (*p).empty();});
      test_ownership<t1>
        ([&](t1 *p) {new(p) t1(v, f1);},
         [](t1 *p) {return equal(*p, irng(1, 4));},
         [&](t1 *p) {new(p) t1(v2, f1);},
         [](t1 *p) {return equal(*p, irng(4, 8));},
         [](t1 *p) {return (*p).empty();});
      test_ownership<t2>
        ([&](t2 *p) {new(p) t2(v, f2);},
         [](t2 *p) {return equal(*p, irng(1, 4));},
         [&](t2 *p) {new(p) t2(v2, f2);},
         [](t2 *p) {return equal(*p, irng(4, 8));},
         [](t2 *p) {return (*p).empty();});
      test_ownership<t3>
        ([&](t3 *p) {new(p) t3(v, f3);},
         [](t3 *p) {return equal(*p, irng(1, 4));},
         [&](t3 *p) {new(p) t3(v2, f3);},
         [](t3 *p) {return equal(*p, irng(4, 8));},
         [](t3 *p) {return (*p).empty();});
    }
  }
  // constructor(r, f)
  {
    ez_list<int> l = {0, 1, 0, 2, 0};
    auto r = filter_rng(move(l), bind(not_equal_to{}, _1, 0));
    assert(l.empty());
    assert(equal(r, irng(1, 3)));
  }
  // range operations
  {
    for (ez_vector<int> &v
           : ez_vector({
               ez_vector<int>{},
               ez_vector<int>{0},
               ez_vector<int>{0, 0, 0},
               ez_vector<int>{1},
               ez_vector<int>{0, 1},
               ez_vector<int>{0, 0, 1, 0, 0},
               ez_vector<int>{0, 0, 0, 1, 2, 0, 3},
               ez_vector<int>{0, 1, 2, 3},
               ez_vector<int>{1, 2, 3, 0},
             })) {
      const ez_vector<int> vv = filter(v, to_back(ez_vector<int>{}),
                                       bind(not_equal_to{}, _1, 0)).base();

      const auto f = [](int x) {return x != 0;};
      filter_range<ez_vector<int> &, bool (*)(int)> r(v, f);
      auto r2 = filter_rng(v, f);
      static_assert(same_as
                    <decltype(r2),
                     filter_range<ez_vector<int> &,
                                  remove_const<decltype(f)>>>);
      auto r3 = filter_rng(rng(v), f);
      static_assert(same_as
                    <decltype(r3),
                     filter_range<iter_pair<int *>, decay<decltype(f)>>>);
      assert(equal(r, vv));
      assert(equal(r2, vv));
      assert(equal(r3, vv));
      assert(begin(r).base() == find_if(v, f));
      assert(begin(r2).base() == find_if(v, f));
      assert(begin(r3).base() == find_if(v, f));
    }
  }
}
void test_take() {
  int a[] = {1, 2, 3};
  ez_vector<int> v = {0, 0, 0};
  assert(take_plus(a, 4, begin(v)) == pair(end(a), end(v)));
  assert(equal(v, irng(1, 4)));
  v = {0, 0, 0};
  assert(take(a, 4, begin(v)) == end(v));
  assert(equal(v, irng(1, 4)));
  v = {0, 0};
  assert(take(a, 2, begin(v)) == end(v));
  assert(equal(v, irng(1, 3)));
  v = {1};
  assert(take(a, 1, begin(v)) == end(v));
  assert(equal(v, irng(1, 2)));
  v = {};
  assert(take(a, 0, begin(v)) == begin(v));
}
void test_take_iterator() {
  // compile-time info
  {
    // as input iterator
    {
      using iter_t = take_iterator<degraded_input_iterator<int *>>;
      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);
      static_assert(is_nothrow_swappable<iter_t>);
      static_assert(input_iterator<iter_t>);
      static_assert(!forward_iterator<iter_t>);
      static_assert(cpp17_input_iterator<iter_t>);
      static_assert(!cpp17_forward_iterator<iter_t>);
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, degraded_input_iterator<int *>>);
      static_assert(same_as<iter_t::iterator_category, input_iterator_tag>);

      using c_iter_t = take_iterator<degraded_input_iterator<const int *>>;
      static_assert(regular<c_iter_t>);
      static_assert(swappable<c_iter_t>);
      static_assert(is_nothrow_swappable<c_iter_t>);
      static_assert(input_iterator<c_iter_t>);
      static_assert(!forward_iterator<c_iter_t>);
      static_assert(cpp17_input_iterator<c_iter_t>);
      static_assert(!cpp17_forward_iterator<c_iter_t>);
      static_assert(same_as<c_iter_t::value_type, int>);
      static_assert(same_as<c_iter_t::reference, const int &>);
      static_assert(same_as<c_iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as
                    <c_iter_t::pointer, degraded_input_iterator<const int *>>);
      static_assert(same_as<c_iter_t::iterator_category, input_iterator_tag>);

      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(convertible_to<const iter_t &, c_iter_t>);
      static_assert(!convertible_to<const c_iter_t &, iter_t>);
    }
    // as forward iterator
    {
      using iter_t = take_iterator<int *>;
      static_assert(regular<iter_t>);
      static_assert(swappable<iter_t>);
      static_assert(forward_iterator<iter_t>);
      static_assert(!bidirectional_iterator<iter_t>);
      static_assert(cpp17_forward_iterator<iter_t>);
      static_assert(!cpp17_bidirectional_iterator<iter_t>);
      static_assert(same_as<iter_t::value_type, int>);
      static_assert(same_as<iter_t::reference, int &>);
      static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<iter_t::pointer, int *>);
      static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);

      using c_iter_t = take_iterator<const int *>;
      static_assert(regular<c_iter_t>);
      static_assert(swappable<c_iter_t>);
      static_assert(forward_iterator<c_iter_t>);
      static_assert(!bidirectional_iterator<c_iter_t>);
      static_assert(cpp17_forward_iterator<c_iter_t>);
      static_assert(!cpp17_bidirectional_iterator<c_iter_t>);
      static_assert(same_as<c_iter_t::value_type, int>);
      static_assert(same_as<c_iter_t::reference, const int &>);
      static_assert(same_as<c_iter_t::difference_type, ptrdiff_t>);
      static_assert(same_as<c_iter_t::pointer, const int *>);
      static_assert(same_as<c_iter_t::iterator_category, forward_iterator_tag>);

      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(convertible_to<const iter_t &, c_iter_t>);
      static_assert(!convertible_to<const c_iter_t &, iter_t>);
    }
  }
  // special member functions
  {
    take_iterator<int *> a[500];
    for (auto &i : a) {
      assert(i.base() == nullptr && i.nth() == 0);
      assert(i == i);
    }
  }
  {
    using iter_t = take_iterator<const int *>;
    const int tmp = 1;
    auto init_f1 = [&tmp](iter_t *p) {
      new(p) iter_t;
    };
    auto init_f2 = [&tmp](iter_t *p) {
      new(p) iter_t(addressof(tmp), 1);
    };
    auto eq_f1 = [&tmp](const iter_t *p) {
      return (*p).base() == nullptr && (*p).nth() == 0;
    };
    auto eq_f2 = [&tmp](const iter_t *p) {
      return (*p).base() == addressof(tmp) && (*p).nth() == 1;
    };
    test_ownership<iter_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](const iter_t *) {return true;});
  }
  // take_iterator(it, n)
  // base
  // nth
  {
    int tmp = 1;
    take_iterator<int *> it(addressof(tmp), 1);
    assert(it.base() == addressof(tmp) && it.nth() == 1);
    assert(*it == 1 && tmp == 1);
    tmp = 2;
    assert(*it == 2 && tmp == 2);
  }
  // take_iterator(const take_iterator<...> &)
  {
    {
      take_iterator<int *> it;
      take_iterator<const int *> it2 = it;
      assert(it.base() == nullptr && it.nth() == 0);
      assert(it2.base() == nullptr && it2.nth() == 0);
    }
    {
      int tmp{};
      take_iterator<int *> it(addressof(tmp), 1);
      take_iterator<const int *> it2 = it;
      assert(it.base() == addressof(tmp) && it.nth() == 1);
      assert(it2.base() == addressof(tmp) && it2.nth() == 1);
      assert(tmp == 0);
    }
  }
  // operations for input iterator
  {
    for (auto &l : ez_vector{ez_list<int>{}, ez_list<int>{1, 2, 3}})
      for (int &n : iters(0, 6)) {
        {
          using iter_t = take_iterator<ez_list<int>::iterator>;
          test_fitr(rng(iter_t(l.begin(), 0), iter_t(l.end(), n)),
                    irng(1, 1 + min_value(n, ssize(l))));
        }
        {
          using iter_t0 = take_iterator<ez_list<int>::iterator>;
          using iter_t = degraded_input_iterator<iter_t0>;
          test_rng(rng(iter_t(iter_t0(l.begin(), 0)),
                       iter_t(iter_t0(l.end(), n))),
                   irng(1, 1 + min_value(n, ssize(l))));
        }
      }
  }
  // take_itr
  {
    const int a[] = {1, 2, 3};
    static_assert(same_as<decltype(take_itr(begin(a), 0)),
                          take_iterator<const int *>>);
    assert(equal(rng(take_itr(begin(a), 0), take_itr(end(a), 0)),
                 empty_rng<int>()));
    assert(equal(rng(take_itr(begin(a), 0), take_itr(end(a), 1)), irng(1, 2)));
    assert(equal(rng(take_itr(begin(a), 0), take_itr(end(a), 3)), irng(1, 4)));
    assert(equal(rng(take_itr(begin(a), 0), take_itr(end(a), 4)), irng(1, 4)));
  }
}
void test_take_range() {
  // compile-time info
  {
    using v_t = ez_vector<int>;
    static_assert(is_rng<take_range<v_t>>);
    static_assert(is_rng<take_range<v_t &>>);
    static_assert(is_rng<take_range<const v_t &>>);
    static_assert(semiregular<take_range<v_t>>);
    static_assert(!semiregular<take_range<v_t &>>);
    static_assert(!semiregular<take_range<const v_t &>>);
    static_assert(swappable<take_range<v_t>>);
    static_assert(!swappable<take_range<v_t &>>);

    static_assert(is_rng<take_range<ez_vector<int>>>);
    static_assert(is_rng<const take_range<ez_vector<int>>>);
    static_assert(is_rng<take_range<combination_range<ez_vector<int>>>>);
    static_assert(!is_rng<const take_range<combination_range<ez_vector<int>>>>);

    static_assert(rng_is_sized<take_range<ez_vector<int>>>);
    static_assert(!rng_is_sized<take_range<ez_forward_list<int>>>);
    static_assert(!rng_is_sized<take_range<combination_range<ez_vector<int>>>>);

    static_assert(!is_rng_ref<take_range<ez_vector<int>>>);
    static_assert(is_rng_ref<take_range<ez_vector<int> &>>);
  }
  // special member functions
  {
    // for value
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<v_t>;
      assert(equal(take_range<v_t>{}, empty_rng<int>()));
      const auto init_f1 = [](r_t *p) {new(p) r_t(v_t{1, 2, 3, 4}, 2);};
      const auto eq_f1 = [](const r_t *p) {return equal(*p, irng(1, 3));};
      const auto init_f2 = [](r_t *p) {new(p) r_t(v_t{5, 6}, 3);};
      const auto eq_f2 = [](const r_t *p) {return equal(*p, irng(5, 7));};
      const auto empty_f = [](const r_t *p) {return p->empty();};
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2, empty_f);
    }
    // for reference
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<v_t &>;
      v_t v1 = {1, 2, 3, 4};
      const auto init_f1 = [&v1](r_t *p) {
        new(p) r_t(v1, 2);
      };
      const auto eq_f1 = [&v1](const r_t *p) {
        return equal(*p, irng(1, 3))
          && to_address((*p).begin()) == to_address(v1.begin());
      };
      const auto empty_f = [](const r_t *) {
        return true;
      };
      test_copy_construct<r_t>(init_f1, eq_f1, empty_f);
      assert(equal(v1, irng(1, 5)));
    }
    // for iter_pair
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<iter_pair<rng_itr<v_t>>>;
      v_t v1 = {1, 2, 3, 4};
      v_t v2 = {5, 6};
      const auto init_f1 = [&v1](r_t *p) {
        new(p) r_t(rng(v1), 2);
      };
      const auto eq_f1 = [&v1](const r_t *p) {
        return equal(*p, irng(1, 3))
          && to_address((*p).begin()) == to_address(v1.begin());
      };
      const auto init_f2 = [&v2](r_t *p) {
        new(p) r_t(rng(v2), 3);
      };
      const auto eq_f2 = [&v2](const r_t *p) {
        return equal(*p, irng(5, 7))
          && to_address((*p).begin()) == to_address(v2.begin());
      };
      const auto empty_f = [](const r_t *) {
        return true;
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2, empty_f);
    }
  }
  // take_range(r, n)
  {
    // for value
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<v_t>;
      r_t r(v_t{1, 2, 3}, 4);
      assert(equal(r, irng(1, 4)));
    }
    // for reference
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<v_t &>;
      v_t v = {1, 2, 3, 4};
      r_t r(v, 4);
      assert(equal(r, irng(1, 5))
             && to_address(r.begin()) == to_address(v.begin()));
    }
    // for iter_pair
    {
      using v_t = ez_vector<int>;
      using r_t = take_range<iter_pair<v_t::iterator>>;
      v_t v = {1, 2, 3, 4};
      r_t r(rng(v), 4);
      assert(equal(r, irng(1, 5))
             && to_address(r.begin()) == to_address(v.begin()));
    }
  }
  // range operations
  {
    using l_t = ez_forward_list<int>;
    using r_t = take_range<l_t>;
    for (int &i : iters(0, 3)) {
      r_t r(l_t{}, i);
      const auto &c_r = r;
      assert(r.begin() == r.end() && r.empty());
      assert(c_r.begin() == c_r.end() && c_r.empty());
    }
    {
      for (int &i : iters(0, 10)) {
        r_t r(l_t{1, 2, 3, 4, 5, 6}, i);
        test_rng(r, irng(1, 1 + min(i, 6)));
        test_rng(as_const(r), irng(1, 1 + min(i, 6)));
      }
    }
  }
  // take_rng(r, n)
  {
    ez_list<int> l = {1, 2, 3, 4, 5};
    ez_vector<int> v = {1, 2, 3, 4, 5};
    for (int i : iters(0, 9)) {
      test_rng(take_rng(l, i), irng(1, 1 + min(i, 5)));
      test_rng(take_rng(copy(l), i), irng(1, 1 + min(i, 5)));
      test_rng(take_rng(v, i), irng(1, 1 + min(i, 5)));
      test_rng(take_rng(copy(v), i), irng(1, 1 + min(i, 5)));
      test_rng(take_rng(rng(v), i), irng(1, 1 + min(i, 5)));
    }
  }
}
void test_drop() {
  int a[] = {1, 2, 3};
  ez_vector<int> v = {0, 0, 0};
  assert(drop_plus(a, 0, begin(v)) == pair(end(a), end(v)));
  v = {0, 0, 0};
  assert(drop(a, 0, begin(v)) == end(v));
  assert(equal(v, irng(1, 4)));
  v = {0, 0};
  assert(drop(a, 1, begin(v)) == end(v));
  assert(equal(v, irng(2, 4)));
  v = {1};
  assert(drop(a, 2, begin(v)) == end(v));
  assert(equal(v, irng(3, 4)));
  v = {};
  assert(drop(a, 3, begin(v)) == begin(v));
  assert(drop(a, 4, begin(v)) == begin(v));
  assert(drop(a, 5, begin(v)) == begin(v));
}
void test_drop_range() {
  // compile-time info
  {
    // iterator
    {
      static_assert(same_as
                    <rng_itr<drop_range<iter_pair<int *>>>, int *>);
      static_assert(same_as
                    <rng_itr<drop_range<ez_vector<int>>>,
                     ez_vector<int>::iterator>);
      static_assert(same_as
                    <rng_itr<drop_range<ez_list<int> &>>,
                     ez_list<int>::iterator>);
    }
    // special member functions
    {
      static_assert(default_initializable<drop_range<ez_vector<int>>>);
      static_assert(!default_initializable<drop_range<ez_vector<int> &>>);
      static_assert(default_initializable<drop_range<iter_pair<int *>>>);

      static_assert(semiregular<drop_range<ez_vector<int>>>);
      static_assert(!semiregular<drop_range<ez_vector<int> &>>);
      static_assert(semiregular<drop_range<iter_pair<int *>>>);
      static_assert(semiregular<drop_range<ez_list<int>>>);
      static_assert(!semiregular<drop_range<ez_list<int> &>>);
      static_assert(!semiregular<drop_range<iter_pair<int *> &>>);
      static_assert(!semiregular<const drop_range<ez_vector<int>>>);

      static_assert(swappable<drop_range<ez_vector<int>>>);
      static_assert(!swappable<drop_range<ez_vector<int> &>>);
      static_assert(swappable<drop_range<iter_pair<int *>>>);
    }
    // is_rng
    {
      static_assert(is_rng<drop_range<ez_vector<int>>>);
      static_assert(!is_rng<const drop_range<ez_vector<int>>>);
      static_assert(is_rng<drop_range<ez_list<int>>>);
      static_assert(!is_rng<const drop_range<ez_list<int>>>);
      static_assert(is_rng<drop_range<iter_pair<ez_list<int>::iterator>>>);
      static_assert(is_rng
                    <const drop_range<iter_pair<ez_list<int>::iterator>>>);
      static_assert(is_rng<drop_range<ez_vector<int> &>>);
      static_assert(is_rng<const drop_range<ez_vector<int> &>>);
      static_assert(is_rng<drop_range<ez_list<int> &>>);
      static_assert(is_rng<const drop_range<ez_list<int> &>>);
      static_assert(is_rng<drop_range<iter_pair<ez_list<int>::iterator> &>>);
      static_assert(is_rng
                    <const drop_range<iter_pair<ez_list<int>::iterator> &>>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<drop_range<ez_list<int>>>);
      static_assert(is_rng_ref<drop_range<ez_list<int> &>>);
      static_assert(is_rng_ref<drop_range<iter_pair<int *>>>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<drop_range<ez_vector<int>>>);
      static_assert(!rng_is_sized<const drop_range<ez_vector<int>>>);
      static_assert(rng_is_sized<drop_range<ez_vector<int> &>>);
      static_assert(rng_is_sized<const drop_range<ez_vector<int> &>>);

      static_assert(rng_is_sized<drop_range<ez_list<int>>>);
      static_assert(!rng_is_sized<const drop_range<ez_list<int>>>);
      static_assert(rng_is_sized<drop_range<ez_list<int> &>>);
      static_assert(rng_is_sized<const drop_range<ez_list<int> &>>);

      static_assert(!rng_is_sized<drop_range<ez_forward_list<int>>>);
      static_assert(!rng_is_sized<const drop_range<ez_forward_list<int>>>);
      static_assert(!rng_is_sized<drop_range<ez_forward_list<int> &>>);
      static_assert(!rng_is_sized<const drop_range<ez_forward_list<int> &>>);

      static_assert(!rng_is_sized
                    <drop_range<iter_pair<ez_forward_list<int>::iterator>>>);
      static_assert(!rng_is_sized
                    <const drop_range
                     <iter_pair<ez_forward_list<int>::iterator>>>);
      static_assert(rng_is_sized
                    <drop_range<iter_pair<ez_vector<int>::iterator>>>);
      static_assert(!rng_is_sized
                    <drop_range<iter_pair<ez_list<int>::iterator>>>);
    }
  }
  // special member functions
  // drop_range(r, n)
  // range operations
  {
    test_rng(drop_range<ez_vector<int>>{}, empty_rng<int>());

    for (int i : irng(0, 5))
      for (int j : irng(0, 10))
        for (int ii : irng(0, 5))
          for (int jj : irng(0, 10)) {
            using v_t = ez_vector<instance_counter<int>>;
            using l_t = ez_forward_list<instance_counter<int>>;
            v_t v = copy(irng(0, i), to_back(v_t{})).base();
            v_t vv = copy(irng(0, ii), to_back(v_t{})).base();
            l_t l = copy(irng(0, i), to_front(l_t{})).base();
            l_t ll = copy(irng(0, ii), to_front(l_t{})).base();
            const auto res1 = irng(min_value(i, j), i);
            const auto res2 = irng(min_value(ii, jj), ii);

            // range<v_t>
            {
              using r_t = drop_range<v_t>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(v, j);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(vv, jj);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).size() == size(res2)
                     && (*p).empty() == empty(res2);
                 },
                 [](r_t *p) {
                   return (*p).begin() == (*p).end()
                     && (*p).empty() && (*p).size() == 0;
                 });
            }
            // range<l_t>
            {
              using r_t = drop_range<l_t>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(l, j);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(ll, jj);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).empty() == empty(res2);
                 },
                 [](r_t *p) {
                   return (*p).begin() == (*p).end() && (*p).empty();
                 });
            }
            // range<v_t &>
            {
              using r_t = drop_range<v_t &>;
              test_copy_construct<r_t>
                ([&](r_t *p) {new(p) r_t(v, j);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1);
                 },
                 [](r_t *) {return true;});
            }
            // range<iter_pair>
            {
              using r_t = drop_range<iter_pair<v_t::iterator>>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(rng(v), j);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1)
                     && equal(as_const(*p), res1)
                     && as_const(*p).size() == size(res1)
                     && as_const(*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(rng(vv), jj);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).size() == size(res2)
                     && (*p).empty() == empty(res2)
                     && equal(as_const(*p), res2)
                     && as_const(*p).size() == size(res2)
                     && as_const(*p).empty() == empty(res2);
                 },
                 [](r_t *) {return true;});
            }

            assert(equal(v, l) && equal(l, irng(0, i)));
            assert(equal(vv, ll) && equal(ll, irng(0, ii)));
          }
    assert(instance_counter<int>::count() == 0);
  }
  // drop_rng(r, n)
  {
    {
      static_assert(is_same
                    <decltype(drop_rng(ez_vector{1, 2, 3, 4}, 2)),
                     drop_range<ez_vector<int>>>);
      assert(equal(drop_rng(ez_vector{1, 2, 3, 4}, 2), irng(3, 5)));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      static_assert(is_same
                    <decltype(drop_rng(v, 2)),
                     drop_range<ez_vector<int> &>>);
      assert(equal(drop_rng(v, 2), irng(3, 5)));
    }
    {
      ez_vector<int> v = {1, 2, 3, 4};
      static_assert(is_same
                    <decltype(drop_rng(rng(v), 2)),
                     drop_range<iter_pair<int *>>>);
      assert(equal(drop_rng(rng(v), 2), irng(3, 5)));
    }
  }
}
void test_take_while() {
  int a[] = {1, 2, 3};
  ez_vector<int> v = {0, 0, 0};
  assert(take_while_plus(a, bind(less{}, _1, 4), begin(v))
         == pair(end(a), end(v)));
  assert(equal(v, irng(1, 4)));
  v = {0, 0, 0};
  assert(take_while(a, bind(less{}, _1, 4), begin(v)) == end(v));
  assert(equal(v, irng(1, 4)));
  v = {0, 0};
  assert(take_while(a, bind(less{}, _1, 3), begin(v)) == end(v));
  assert(equal(v, irng(1, 3)));
  v = {0};
  assert(take_while(a, bind(less{}, _1, 2), begin(v)) == end(v));
  assert(equal(v, irng(1, 2)));
  v = {};
  assert(take_while(a, bind(less{}, _1, 1), begin(v)) == end(v));
  assert(v.empty());
}
void test_take_while_iterator() {
  // compile-time info
  {
    // member types
    {
      // as input iterator
      {
        using iter_t = take_while_iterator<degraded_input_iterator<int *>,
                                           bool (*)(const int &)>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int &>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, degraded_input_iterator<int *>>);
        static_assert(same_as<iter_t::iterator_category, input_iterator_tag>);
      }
      // as forward iterator
      {
        using iter_t = take_while_iterator<const int *, bool (*)(const int &)>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, const int &>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, const int *>);
        static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      // as input iterator
      {
        using iter_t = take_while_iterator<degraded_input_iterator<int *>,
                                           bool (*)(const int &)>;
        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = take_while_iterator<int *, bool (*)(const int &)>;
        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
      // with non-semiregular function
      {
        int tmp{};
        auto f = [&](int) {return true;};
        using iter_t = take_while_iterator<int *, decltype(f)>;
        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
    }
    // iterator and const_iterator
    {
      // as input iterator
      {
        using iter_t = take_while_iterator<degraded_input_iterator<int *>,
                                           bool (*)(const int &)>;
        using c_iter_t = take_while_iterator<degraded_input_iterator
                                             <const int *>,
                                             bool (*)(const int &)>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
      }
      // as forward iterator
      {
        using iter_t = take_while_iterator<int *, bool (*)(const int &)>;
        using c_iter_t = take_while_iterator<const int *,
                                             bool (*)(const int &)>;
        static_assert(convertible_to<iter_t, c_iter_t>);
        static_assert(!convertible_to<c_iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, iter_t>);
        static_assert(equality_comparable_with<iter_t, c_iter_t>);
      }
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = take_while_iterator<degraded_input_iterator<int *>,
                                           bool (*)(const int &)>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = take_while_iterator<int *, bool (*)(const int &)>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
    }
  }
  // special member functions
  // construct by (it, ed, f)
  // base
  // fn
  {
    int tmp = 1;
    using iter_t = take_while_iterator<int *, bool (*)(int)>;
    using init_f = ez_function<void (iter_t *)>;
    using eq_f = ez_function<bool (const iter_t *)>;
    ez_vector<pair<init_f, eq_f>> v;
    {
      auto x = [](iter_t *p) {new(p) iter_t;};
      auto y = [](const iter_t *p) {
        return (*p).base() == nullptr && (*p).fn() == nullptr;
      };
      v.insert(v.end(), pair(x, y));
    }
    {
      auto x = [&tmp](iter_t *p) {
        new(p) iter_t(addressof(tmp), addressof(tmp) + 1,
                      [](int) {return true;});
      };
      auto y = [&tmp](const iter_t *p) {
        return (*p).base() == addressof(tmp) && (*p).fn()(0)
          && equal(rng(*p, next(*p)), single_rng(1));
      };
      v.insert(v.end(), pair(x, y));
    }
    for (auto &x : v)
      for (auto &y : v)
        test_ownership<iter_t>(x.first, x.second, y.first, y.second,
                               [](const iter_t *) {return true;});
  }
  // construct const_iterator by iterator
  // iterator operations
  {
    for (int i : iters(0, 8))
      for (int j : iters(0, 8)) {
        ez_vector<int> v;
        copy(rng(i, 0), to_back(v));
        copy(rng(j, 1), to_back(v));
        using iter_t = take_while_iterator<int *, bool (*)(int)>;
        using c_iter_t = take_while_iterator<int *, bool (*)(int)>;
        const auto f = [](int x) {return x == 0;};

        iter_t x(begin(v), end(v), f);
        c_iter_t cx(x);
        iter_t y(end(v), end(v), f);
        c_iter_t cy(y);

        test_fitr(x, y, rng(i, 0));
        test_fitr(cx, y, rng(i, 0));
        test_fitr(x, cy, rng(i, 0));
        test_fitr(cx, cy, rng(i, 0));
        assert(next(x, y).base() == nth(v, i));
        assert(next(cx, y).base() == nth(v, i));
        assert(next(x, cy).base() == nth(v, i));
        assert(next(cx, cy).base() == nth(v, i));
      }
  }
  // take_while_itr
  {
    int a[] = {1, 2, 3};
    using fp_t = bool (*)(int);
    fp_t f = [](int x) {return x < 3;};
    assert(equal(rng(take_while_itr(begin(a), end(a), f),
                     take_while_itr(end(a), end(a), f)),
                 irng(1, 3)));
  }
}
void test_take_while_range() {
  // compile-time info
  {
    int tmp{};
    using f1_t = bool (*)(int);
    using f2_t = decltype([&tmp](int) {return true;});
    using v_t = ez_vector<int>;
    using r1_t = take_while_range<v_t, f1_t>;
    using r2_t = take_while_range<v_t &, f1_t>;
    using r3_t = take_while_range<iter_pair<rng_itr<v_t>>, f1_t>;
    using r11_t = take_while_range<v_t, f2_t>;
    using r22_t = take_while_range<v_t &, f2_t>;
    using r33_t = take_while_range<iter_pair<rng_itr<v_t>>, f2_t>;

    // special member functions
    {
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);
      static_assert(!semiregular<r11_t>);
      static_assert(!semiregular<r22_t>);
      static_assert(!semiregular<r33_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!is_swappable<r2_t>);
      static_assert(is_swappable<r3_t>);
      static_assert(is_nothrow_swappable<r11_t>);
      static_assert(!is_swappable<r22_t>);
      static_assert(is_nothrow_swappable<r33_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r1_t>);
      static_assert(is_rng<const r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(is_rng<const r2_t>);
      static_assert(is_rng<r3_t>);
      static_assert(is_rng<const r3_t>);
      static_assert(is_rng<r11_t>);
      static_assert(is_rng<const r11_t>);
      static_assert(is_rng<r22_t>);
      static_assert(is_rng<const r22_t>);
      static_assert(is_rng<r33_t>);
      static_assert(is_rng<const r33_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<const r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<const r2_t>);
      static_assert(is_rng_ref<r3_t>);
      static_assert(is_rng_ref<const r3_t>);
      static_assert(!is_rng_ref<r11_t>);
      static_assert(!is_rng_ref<const r11_t>);
      static_assert(!is_rng_ref<r22_t>);
      static_assert(!is_rng_ref<const r22_t>);
      static_assert(!is_rng_ref<r33_t>);
      static_assert(!is_rng_ref<const r33_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<const r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<const r3_t>);
      static_assert(!rng_is_sized<r11_t>);
      static_assert(!rng_is_sized<const r11_t>);
      static_assert(!rng_is_sized<r22_t>);
      static_assert(!rng_is_sized<const r22_t>);
      static_assert(!rng_is_sized<r33_t>);
      static_assert(!rng_is_sized<const r33_t>);
    }
  }
  // special member functions
  // construct by (r, f)
  // range operations
  {
    using l_t = ez_forward_list<int>;
    using fp_t = bool (*)(int);
    using r_t = take_while_range<l_t, fp_t>;
    fp_t f = [](int x) {return x >= 0;};
    for (int i : iters(0, 5))
      for (int j : iters(0, 5))
        for (int ii : iters(0, 5))
          for (int jj : iters(0, 5)) {
            ez_vector<int> v, vv;
            auto f1 = [=](r_t *p) {
              new(p) r_t(copy(rng(j, -1), copy(irng(0, i), to_front(l_t{})))
                         .base(), f);
            };
            auto f2 = [=](r_t *p) {
              return equal(*p, irng(0, i)) && (*p).empty() == (i == 0);
            };
            auto f3 = [=](r_t *p) {
              new(p) r_t(copy(rng(jj, -1), copy(irng(0, ii), to_front(l_t{})))
                         .base(), f);
            };
            auto f4 = [=](r_t *p) {
              return equal(*p, irng(0, ii)) && (*p).empty() == (ii == 0);
            };
            test_ownership<r_t>(f1, f2, f3, f4, [](r_t *p) {
              return (*p).empty() && test_equal((*p).begin(), (*p).end());
            });
          }
  }
  // take_while_rng
  {
    const ez_forward_list<int> l = {1, 2, 3, 4, 5};
    assert(equal(take_while_rng(rng(l), [](int x) {return x < 4;}),
                 irng(1, 4)));
    assert(equal(take_while_rng(copy(l), [](int x) {return x < 4;}),
                 irng(1, 4)));
    assert(equal(take_while_rng(l, [](int x) {return x < 4;}),
                 irng(1, 4)));
  }
}
void test_drop_while() {
  int a[] = {1, 2, 3};
  ez_vector<int> v = {0, 0, 0};
  assert(drop_while_plus(a, bind(less{}, _1, 1), begin(v))
         == pair(end(a), end(v)));
  assert(equal(v, irng(1, 4)));
  v = {0, 0};
  assert(drop_while_plus(a, bind(less{}, _1, 2), begin(v))
         == pair(end(a), end(v)));
  assert(equal(v, irng(2, 4)));
  v = {0};
  assert(drop_while(a, bind(less{}, _1, 3), begin(v)) == end(v));
  assert(equal(v, irng(3, 4)));
  v = {};
  assert(drop_while(a, bind(less{}, _1, 4), begin(v)) == end(v));
}
void test_drop_while_range() {
  // compile-time info
  {
    using f_t = bool (*)(int);
    using v_t = ez_vector<int>;
    using r1_t = drop_while_range<v_t, f_t>;
    using r2_t = drop_while_range<v_t &, f_t>;
    using r3_t = drop_while_range<iter_pair<int *>, f_t>;

    // iterator
    {
      static_assert(same_as<rng_itr<r1_t>, int *>);
      static_assert(same_as<rng_itr<r2_t>, int *>);
      static_assert(same_as<rng_itr<r3_t>, int *>);
      static_assert(same_as
                    <rng_itr<drop_while_range<const v_t &, f_t>>, const int *>);
    }
    // special member functions
    {
      static_assert(default_initializable<r1_t>);
      static_assert(!default_initializable<r2_t>);
      static_assert(default_initializable<r3_t>);
      int tmp{};
      auto ff = [&tmp](int) {return true;};
      static_assert(!default_initializable
                    <drop_while_range<v_t, decltype(ff)>>);

      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);

      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!is_swappable<r2_t>);
      static_assert(is_nothrow_swappable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(is_rng<r3_t>);
      static_assert(!is_rng<const r1_t>);
      static_assert(is_rng<const r2_t>);
      static_assert(is_rng<const r3_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<drop_while_range<ez_forward_list<int>, f_t>>);
    }
  }
  // special member functions
  // drop_while_range(r, f)
  // range operations
  {
    using r0_t = drop_while_range<ez_vector<int>, bool (*)(int)>;
    r0_t r0;
    assert(r0.empty() && r0.size() == 0 && r0.begin() == r0.end());

    for (int i : irng(0, 5))
      for (int j : irng(0, 10))
        for (int ii : irng(0, 5))
          for (int jj : irng(0, 10)) {
            using v_t = ez_vector<int>;
            using l_t = ez_forward_list<int>;
            v_t v = copy(irng(0, i), to_back(v_t{})).base();
            v_t vv = copy(irng(0, ii), to_back(v_t{})).base();
            l_t l = copy(irng(0, i), to_front(l_t{})).base();
            l_t ll = copy(irng(0, ii), to_front(l_t{})).base();
            const auto res1 = irng(min_value(i, j), i);
            const auto res2 = irng(min_value(ii, jj), ii);

            // range<v_t>
            {
              auto f1 = bind(less{}, _1, j);
              auto f2 = bind(less{}, _1, jj);
              using r_t = drop_while_range<v_t, decltype(f1)>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(v, f1);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(vv, f2);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).size() == size(res2)
                     && (*p).empty() == empty(res2);
                 },
                 [](r_t *p) {
                   return (*p).begin() == (*p).end()
                     && (*p).empty() && (*p).size() == 0;
                 });
            }
            // range<l_t>
            {
              auto f1 = bind(less{}, _1, j);
              auto f2 = bind(less{}, _1, jj);
              using r_t = drop_while_range<l_t, decltype(f1)>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(l, f1);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(ll, f2);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).empty() == empty(res2);
                 },
                 [](r_t *p) {
                   return (*p).begin() == (*p).end() && (*p).empty();
                 });
            }
            // range<v_t &>
            {
              auto f1 = bind(less{}, _1, j);
              using r_t = drop_while_range<v_t &, decltype(f1)>;
              test_copy_construct<r_t>
                ([&](r_t *p) {new(p) r_t(v, f1);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1);
                 },
                 [](r_t *) {return true;});
            }
            // range<iter_pair>
            {
              auto f1 = bind(less{}, _1, j);
              auto f2 = bind(less{}, _1, jj);
              using r_t = drop_while_range<iter_pair<v_t::iterator>,
                                           decltype(f1)>;
              test_ownership<r_t>
                ([&](r_t *p) {new(p) r_t(rng(v), f1);},
                 [&](r_t *p) {
                   return equal(*p, res1) && (*p).size() == size(res1)
                     && (*p).empty() == empty(res1)
                     && equal(as_const(*p), res1)
                     && as_const(*p).size() == size(res1)
                     && as_const(*p).empty() == empty(res1);
                 },
                 [&](r_t *p) {new(p) r_t(rng(vv), f2);},
                 [&](r_t *p) {
                   return equal(*p, res2) && (*p).size() == size(res2)
                     && (*p).empty() == empty(res2)
                     && equal(as_const(*p), res2)
                     && as_const(*p).size() == size(res2)
                     && as_const(*p).empty() == empty(res2);
                 },
                 [](r_t *) {return true;});
            }

            assert(equal(v, l) && equal(l, irng(0, i)));
            assert(equal(vv, ll) && equal(ll, irng(0, ii)));
          }
  }
  // drop_while_rng
  {
    ez_vector v = {1, 2, 3};
    assert(equal(drop_while_rng(copy(v), bind(less{}, _1, 3)), single_rng(3)));
    assert(equal(drop_while_rng(rng(v), bind(less{}, _1, 3)), single_rng(3)));
  }
}
void test_join() {
  const auto r
    = join(ez_vector{ez_vector{1, 2}, ez_vector<int>{}, ez_vector{3, 4}},
           to_back(ez_vector<int>{})).base();
  assert(equal(r, irng(1, 5)));
}
void test_join_iterator() {
  // compile-time info
  {
    // member types
    {
      // for array of array
      {
        using iter_t = join_iterator<int (*)[3]>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int &>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, int *>);
        static_assert(same_as
                      <iter_t::iterator_category, bidirectional_iterator_tag>);

        using c_iter_t = join_iterator<const int (*)[3]>;
        static_assert(same_as<c_iter_t::value_type, int>);
        static_assert(same_as<c_iter_t::reference, const int &>);
        static_assert(same_as<c_iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<c_iter_t::pointer, const int *>);
        static_assert(same_as
                      <c_iter_t::iterator_category,
                       bidirectional_iterator_tag>);
      }
      // for container of container
      {
        using iter_t = join_iterator<ez_vector<ez_list<int>>::iterator>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int &>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, ez_list<int>::iterator>);
        static_assert(same_as
                      <iter_t::iterator_category, bidirectional_iterator_tag>);
      }
    }
    // special member functions
    {
      // for array of array
      {
        using iter_t = join_iterator<int (*)[3]>;
        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);
        static_assert(is_nothrow_copyable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);

        using c_iter_t = join_iterator<const int (*)[3]>;
        static_assert(regular<c_iter_t>);
        static_assert(swappable<c_iter_t>);
        static_assert(is_nothrow_copyable<c_iter_t>);
        static_assert(is_nothrow_swappable<c_iter_t>);
      }
      // for container of container
      {
        using iter_t = join_iterator<ez_vector<ez_list<int>>::iterator>;
        static_assert(regular<iter_t>);
        static_assert(swappable<iter_t>);
        static_assert(is_nothrow_copyable<iter_t>);
        static_assert(is_nothrow_swappable<iter_t>);
      }
    }
    // iterator concept
    {
      // as input iterator
      {
        using iter_t = join_iterator<degraded_input_iterator<int (*)[2]>>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = join_iterator<ez_forward_list<int> *>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = join_iterator<int (*)[3]>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
    }
    // iterator and const_iterator
    {
      using iter_t = join_iterator<int (*)[3]>;
      using c_iter_t = join_iterator<const int (*)[3]>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
    }
  }
  // special member functions
  // construct by (it, inner_it, ed)
  // base
  // outer_base
  {
    // default constructor
    {
      join_iterator<int (*)[3]> a[500];
      for (auto &x : a) {
        assert(x.base() == nullptr);
        assert(x.outer_iter() == nullptr);
      }
    }
    using iter_t = join_iterator<int (*)[3]>;
    using iter_pair_t = iter_pair<iter_t>;
    using init_f = ez_function<void (iter_pair_t *)>;
    using pred_f = ez_function<bool (iter_pair_t *)>;
    ez_vector<pair<init_f, pred_f>> v;

    {
      const auto x = [](iter_pair_t *p) {new(p) iter_pair_t;};
      const auto y = [](iter_pair_t *p) {
        return test_equal(p->begin(), p->end())
          && p->begin().base() == nullptr
          && p->begin().outer_iter() == nullptr
          && p->end().base() == nullptr
          && p->end().outer_iter() == nullptr;
      };
      v.insert(v.end(), pair(x, y));
    }

    int a[][3] = {{1, 2, 3}, {4, 5, 6}};
    {
      const auto x = [&](iter_pair_t *p) {
        new(p) iter_pair_t(iter_t(begin(a), begin(*begin(a)), end(a)),
                           iter_t(end(a), {}, end(a)));
      };
      const auto y = [&](iter_pair_t *p) {
        return equal(*p, irng(1, 7))
          && p->begin().base() == begin(*begin(a))
          && p->begin().outer_iter() == begin(a)
          && p->end().base() == nullptr
          && p->end().outer_iter() == end(a);
      };
      v.insert(v.end(), pair(x, y));
    }

    int aa[][3] = {{7, 8, 9}, {10, 11, 12}};
    {
      const auto x = [&](iter_pair_t *p) {
        new(p) iter_pair_t(iter_t(begin(aa), begin(*begin(aa)), end(aa)),
                           iter_t(end(aa), {}, end(aa)));
      };
      const auto y = [&](iter_pair_t *p) {
        return equal(*p, irng(7, 13))
          && p->begin().base() == begin(*begin(aa))
          && p->begin().outer_iter() == begin(aa)
          && p->end().base() == nullptr
          && p->end().outer_iter() == end(aa);
      };
      v.insert(v.end(), pair(x, y));
    }

    for (auto &x : v)
      for (auto &y : v)
        test_ownership<iter_pair_t>(x.first, x.second, y.first, y.second,
                                    [](const iter_pair_t *) {return true;});
  }
  // iterator operations
  // iterator operations between const_iterator and iterator
  {
    using v_t = ez_vector<int>;
    using vv_t = ez_vector<v_t>;
    using iter_t = join_iterator<rng_itr<vv_t>>;
    using c_iter_t = join_iterator<rng_itr<vv_t>>;
    vv_t v = {
      v_t{}, v_t{}, v_t{},
      v_t{1}, v_t{2, 3}, v_t{4, 5, 6}, v_t{7, 8, 9, 10}
    };
    while (next_permutation(v)) {
      const auto v_copy = join(v, to_back(v_t{})).base();

      const auto i = find_if_not(v, empty);
      iter_t it(i, begin(*i), end(v));
      iter_t ed(end(v), {}, end(v));
      c_iter_t it2(i, begin(*i), end(v));
      c_iter_t ed2(end(v), {}, end(v));
      test_bitr(it, ed, v_copy);
      test_bitr(it2, ed, v_copy);
      test_bitr(it, ed2, v_copy);
      test_bitr(it2, ed2, v_copy);

      test_bitr(c_iter_t(it), c_iter_t(ed), v_copy);
      test_bitr(c_iter_t(it), ed, v_copy);
      test_bitr(it, c_iter_t(ed), v_copy);
    }
  }
  // join_itr
  {
    int a[2][3] = {{1, 2, 3}, {4, 5, 6}};
    test_bitr(join_itr(begin(a), begin(front(a)), end(a)),
              join_itr(end(a), {}, end(a)),
              irng(1, 7));
  }
}
void test_join_range() {
  // compile-time info
  {
    using v_t = ez_vector<int>;
    using vv_t = ez_vector<v_t>;
    using r0_t = join_range<iter_pair<rng_itr<vv_t>>>;
    using r1_t = join_range<vv_t>;
    using r2_t = join_range<vv_t &>;
    using r3_t = join_range<bind_range<v_t, v_t (*)(int)>>;

    // special member functions
    {
      static_assert(semiregular<r0_t>);
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);

      static_assert(is_nothrow_swappable<r0_t>);
      static_assert(is_nothrow_swappable<r1_t>);
      static_assert(!is_swappable<r2_t>);
      static_assert(is_swappable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r0_t>);
      static_assert(is_rng<const r0_t>);
      static_assert(is_rng<r1_t>);
      static_assert(!is_rng<const r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(is_rng<const r2_t>);
      static_assert(is_rng<r3_t>);
      static_assert(!is_rng<const r3_t>);
    }
    // is_rng_ref
    {
      static_assert(is_rng_ref<r0_t>);
      static_assert(is_rng_ref<const r0_t>);
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<const r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<const r2_t>);
      static_assert(!is_rng_ref<r3_t>);
      static_assert(!is_rng_ref<const r3_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r0_t>);
      static_assert(!rng_is_sized<const r0_t>);
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<const r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<const r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<const r3_t>);
    }
  }
  // with no cached middle-range
  {
    // special member functions
    {
      // join_range<value>
      {
        using l_t = ez_forward_list<int>;
        using ll_t = ez_forward_list<l_t>;
        using r_t = join_range<ll_t>;

        using init_ft = ez_function<void (r_t *)>;
        using pred_ft = ez_function<bool (r_t *)>;
        ez_vector<pair<init_ft, pred_ft>> fns;

        {
          const auto x = [](r_t *p) {
            new(p) r_t;
          };
          const auto y = [](r_t *p) {
            return p->begin() == p->end() && p->empty();
          };
          fns.insert(fns.end(), pair(x, y));
        }
        {
          const auto x = [](r_t *p) {
            new(p) r_t(ll_t{l_t{1}, l_t{2}, l_t{3}});
          };
          const auto y = [](r_t *p) {
            return equal(*p, irng(1, 4));
          };
          fns.insert(fns.end(), pair(x, y));
        }
        {
          const auto x = [](r_t *p) {
            new(p) r_t(ll_t{l_t{1, 2}, l_t{3, 4}});
          };
          const auto y = [](r_t *p) {
            return equal(*p, irng(1, 5));
          };
          fns.insert(fns.end(), pair(x, y));
        }

        for (auto &x : fns)
          for (auto &y : fns)
            test_ownership<r_t>(x.first, x.second, y.first, y.second,
                                [](r_t *p) {
                                  return p->begin() == p->end() && p->empty();
                                });
      }
      // join_range<reference>
      {
        using l_t = ez_forward_list<int>;
        using ll_t = ez_forward_list<l_t>;
        using r_t = join_range<ll_t &>;

        using init_ft = ez_function<void (r_t *)>;
        using pred_ft = ez_function<bool (r_t *)>;
        ez_vector<pair<init_ft, pred_ft>> fns;

        ll_t l0 = {};
        {
          const auto x = [&l0](r_t *p) {
            new(p) r_t(l0);
          };
          const auto y = [](r_t *p) {
            return p->begin() == p->end() && p->empty();
          };
          fns.insert(fns.end(), pair(x, y));
        }
        ll_t l1 = {{1, 2}, {3, 4}};
        {
          const auto x = [&l1](r_t *p) {
            new(p) r_t(l1);
          };
          const auto y = [&l1](r_t *p) {
            return equal(*p, irng(1, 5))
              && p->begin().base() == begin(front(l1));
          };
          fns.insert(fns.end(), pair(x, y));
        }

        for (auto &x : fns)
          test_copy_construct<r_t>(x.first, x.second, [](r_t *) {return true;});
      }
      // join_range<iter_pair>
      {
        using l_t = ez_forward_list<int>;
        using ll_t = ez_forward_list<l_t>;
        using r_t = join_range<iter_pair<ll_t::iterator>>;

        using init_ft = ez_function<void (r_t *)>;
        using pred_ft = ez_function<bool (r_t *)>;
        ez_vector<pair<init_ft, pred_ft>> fns;

        ll_t l0 = {};
        {
          const auto x = [&l0](r_t *p) {
            new(p) r_t(rng(l0));
          };
          const auto y = [](r_t *p) {
            return p->begin() == p->end() && p->empty();
          };
          fns.insert(fns.end(), pair(x, y));
        }
        ll_t l1 = {{1}, {2, 3}};
        {
          const auto x = [&l1](r_t *p) {
            new(p) r_t(rng(l1));
          };
          const auto y = [&](r_t *p) {
            return equal(*p, irng(1, 4))
              && p->begin().base() == begin(front(l1));
          };
          fns.insert(fns.end(), pair(x, y));
        }

        for (auto &x : fns)
          for (auto &y : fns)
            test_ownership<r_t>(x.first, x.second, y.first, y.second,
                                [](r_t *) {return true;});
      }
    }
    // construct by (r)
    // range operations
    {
      using v_t = ez_vector<int>;
      using vv_t = ez_vector<v_t>;
      {
        vv_t v = {v_t{}};
        join_range<vv_t> r(v);
        join_range<vv_t &> r2(v);
        assert(r.empty() && r.begin() == r.end());
        assert(r2.empty() && r2.begin() == r2.end());
        assert(as_const(r2).empty()
               && as_const(r2).begin() == as_const(r2).end());
      }
      {
        vv_t v = {v_t{}, v_t{}, v_t{}};
        join_range<vv_t> r(v);
        join_range<vv_t &> r2(v);
        assert(r.empty() && r.begin() == r.end());
        assert(r2.empty() && r2.begin() == r2.end());
        assert(as_const(r2).empty()
               && as_const(r2).begin() == as_const(r2).end());
      }
      {
        vv_t v = {v_t{}, v_t{1, 2}};
        join_range<vv_t> r(v);
        join_range<vv_t &> r2(v);
        assert(equal(r, irng(1, 3)) && !r.empty());
        assert(equal(r2, irng(1, 3)) && !r2.empty());
        assert(equal(as_const(r2), irng(1, 3)) && !as_const(r2).empty());
      }
      {
        vv_t v = {v_t{}, v_t{}, v_t{}, v_t{1, 2}};
        join_range<vv_t> r(v);
        join_range<vv_t &> r2(v);
        assert(equal(r, irng(1, 3)) && !r.empty());
        assert(equal(r2, irng(1, 3)) && !r2.empty());
        assert(equal(as_const(r2), irng(1, 3)) && !as_const(r2).empty());
      }
    }
  }
  // with cached middle-range
  {
    // special member functions
    {
      // join_range<value>
      {
        using l_t = ez_forward_list<int>;
        using ll_t = bind_range<l_t, l_t (*)(int)>;
        using r_t = join_range<ll_t>;
        static_assert(is_irng<r_t>);

        using init_ft = ez_function<void (r_t *)>;
        using pred_ft = ez_function<bool (r_t *)>;
        ez_vector<pair<init_ft, pred_ft>> fns;

        {
          const auto x = [](r_t *p) {
            new(p) r_t;
          };
          const auto y = [](r_t *p) {
            return p->begin() == p->end() && p->empty();
          };
          fns.insert(fns.end(), pair(x, y));
        }
        {
          const auto x = [](r_t *p) {
            new(p) r_t(ll_t(l_t{1, 2, 3}, [](int x) {return l_t{0, x};}));
          };
          const auto y = [](r_t *p) {
            const bool ret = equal(*p, l_t{0, 1, 0, 2, 0, 3});
            assert(p->empty());
            *p = *p;
            assert(!p->empty());
            return ret;
          };
          fns.insert(fns.end(), pair(x, y));
        }
        {
          const auto x = [](r_t *p) {
            new(p) r_t(ll_t(l_t{1, 2, 3}, [](int x) {return l_t{0, 0, x};}));
          };
          const auto y = [](r_t *p) {
            const bool ret = equal(*p, l_t{0, 0, 1, 0, 0, 2, 0, 0, 3});
            assert(p->empty());
            *p = *p;
            assert(!p->empty());
            return ret;
          };
          fns.insert(fns.end(), pair(x, y));
        }

        for (auto &x : fns)
          for (auto &y : fns)
            test_ownership<r_t>(x.first, x.second, y.first, y.second,
                                [](r_t *p) {
                                  return p->begin() == p->end() && p->empty();
                                });
      }
      // join_range<reference>
      // join_range<iter_pair>
    }
    // construct by (r)
    // range operations
    {
      using l_t = ez_forward_list<int>;
      using ll1_t = bind_range<l_t, l_t (*)(int)>;
      using ll2_t = bind_range<l_t &, l_t (*)(int)>;
      using r1_t = join_range<ll1_t>;
      using r2_t = join_range<ll2_t>;
      using r3_t = join_range<ll1_t &>;
      using r4_t = join_range<ll2_t &>;

      {
        r1_t r(ll1_t(l_t{1, 2, 3}, [](int x) {return l_t{0, x};}));
        assert(equal(r, l_t{0, 1, 0, 2, 0, 3}));
        assert(equal(r, rng(0, 0)));
        r = r;
        assert(equal(r, l_t{0, 1, 0, 2, 0, 3}));
        assert(equal(r, rng(0, 0)));
      }
      {
        l_t l = {1, 2, 3};
        r2_t r(ll2_t(l, [](int x) {return l_t{0, x};}));
        l = {4, 5, 6};
        assert(equal(r, l_t{0, 4, 0, 5, 0, 6}));
        assert(equal(r, rng(0, 0)));
      }
      {
        l_t l = {1, 2, 3};
        ll1_t ll(l, [](int x) {return l_t{0, x};});
        r3_t r(ll);
        l = {4, 5, 6};
        assert(equal(r, l_t{0, 1, 0, 2, 0, 3}));
        assert(equal(r, rng(0, 0)));
      }
      {
        l_t l = {1, 2, 3};
        ll2_t ll(l, [](int x) {return l_t{0, x};});
        r4_t r(ll);
        l = {4, 5, 6};
        assert(equal(r, l_t{0, 4, 0, 5, 0, 6}));
        assert(equal(r, rng(0, 0)));
      }
    }

    auto r = join_rng(bind_rng(irng(0, 4), [](int x) {
      return copy(irng(0, x), to_back(ez_vector<int>{})).base();
    }));
    static_assert(is_irng<decltype(r)>);
    assert(equal(r, ez_vector{0, 0, 1, 0, 1, 2}));
  }
  // join_rng
  {
    using v_t = ez_vector<int>;
    using vv_t = ez_vector<v_t>;
    vv_t vv = {{1, 2}, {3, 4}};

    static_assert(same_as
                  <decltype(join_rng(rng(vv))),
                   join_range<iter_pair<rng_itr<vv_t>>>>);
    assert(equal(join_rng(rng(vv)), irng(1, 5)));
    assert(equal(join_rng(vv), irng(1, 5)));
    assert(equal(join_rng(move(vv)), irng(1, 5)));

    vv = {{1, 2}, {3, 4}};
    assert(equal(join_rng(begin(vv), end(vv)), irng(1, 5)));
    assert(equal(join_rng(begin(vv), 2), irng(1, 5)));
  }
  // join_range::for_each
  {
    ez_vector<ez_vector<int>> v = {{1, 2, 3}, {4}, {5, 6}};
    ez_vector<int> vv;
    for (int &x : join_rng(v))
      vv.insert(vv.end(), x);
    assert(equal(vv, irng(1, 7)));
    vv.erase(vv.begin(), vv.end());
    assert(vv.empty());
    join_rng(v).for_each([&vv](int &x) {vv.insert(vv.end(), x);});
    assert(equal(vv, irng(1, 7)));
  }
}
void test_join_with() {
  using v_t = ez_vector<int>;
  using vv_t = ez_vector<v_t>; 
  assert(equal(join_with(vv_t{v_t{1, 2}}, v_t{0, 0},
                         to_back(v_t{})).base(),
               v_t{1, 2}));
  assert(equal(join_with(vv_t{v_t{1, 2}, v_t{3, 4}}, v_t{0, 0},
                         to_back(v_t{})).base(),
               v_t{1, 2, 0, 0, 3, 4}));
  assert(equal(join_with(vv_t{v_t{1, 2}, v_t{3, 4}, v_t{5, 6}}, v_t{0, 0},
                         to_back(v_t{})).base(),
               v_t{1, 2, 0, 0, 3, 4, 0, 0, 5, 6}));
}
void test_join_with_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = join_with_iterator<ez_vector<ez_vector<int>>::iterator,
                                          ez_vector<int>::iterator>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int &>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, bidirectional_iterator_tag>);
      }
      {
        using iter_t = join_with_iterator<ez_vector<ez_vector<int>>::iterator,
                                          ez_vector<char>::iterator>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, bidirectional_iterator_tag>);
      }
      {
        using iter_t = join_with_iterator<ez_vector<ez_vector<int>>::iterator,
                                          ez_forward_list<char>::iterator>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, forward_iterator_tag>);
      }
      {
        using iter_t = join_with_iterator
          <ez_vector<ez_vector<int>>::iterator,
           degraded_input_iterator<ez_forward_list<char>::iterator>>;
        static_assert(same_as<iter_t::value_type, int>);
        static_assert(same_as<iter_t::reference, int>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, input_iterator_tag>);
      }
    }
    // special member functions
    {
      using iter_t = join_with_iterator<int (*)[3], int *>;

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
        using iter_t = join_with_iterator<degraded_input_iterator<int (*)[2]>,
                                          int *>;
        static_assert(input_iterator<iter_t>);
        static_assert(!forward_iterator<iter_t>);
        static_assert(cpp17_input_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_iitr<iter_t>);
      }
      // as forward iterator
      {
        using iter_t = join_with_iterator<ez_forward_list<int> *, int *>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(cpp17_forward_iterator<iter_t>);
        static_assert(!cpp17_bidirectional_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = join_with_iterator<ez_list<int> *, int *>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(cpp17_bidirectional_iterator<iter_t>);
        static_assert(!cpp17_random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = join_with_iterator<int (*)[3], int *>;
      using c_iter_t = join_with_iterator<const int (*)[3], const int *>;
      static_assert(convertible_to<iter_t, c_iter_t>);
      static_assert(!convertible_to<c_iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, iter_t>);
      static_assert(equality_comparable_with<iter_t, c_iter_t>);
      static_assert(equality_comparable_with<c_iter_t, c_iter_t>);
    }
  }

  // construct from (it, inner_it, ed, op2, it2, ed2)
  // iterator operations
  {
    {
      ez_vector<ez_vector<int>> v = {{1, 2}};
      ez_vector<int> v2 = {0};
      using iter_t = join_with_iterator<rng_itr<decltype(v)>,
                                        rng_itr<decltype(v2)>>;
      test_fitr(iter_t(begin(v), begin(ref(v, 0)), end(v),
                       begin(v2), end(v2), end(v2)),
                iter_t(end(v), {}, end(v),
                       begin(v2), end(v2), end(v2)),
                ez_vector{1, 2});
    }
    {
      ez_vector<ez_vector<int>> v0 = {
        {}, {}, {}, {1}, {2}, {3, 4}, {5, 6}, {7, 8, 9}, {10, 11, 12}
      };
      for (int i : irng(0, 7))
        for (int j : irng(0, 4)) {
          const auto v2 = irng(100, 100 + j);

          ez_vector<int> cmp_v;
          for (decltype(auto) v : combination_rng(v0, i)) {
            using iter_t = join_with_iterator<rng_itr<decltype(v)>,
                                              rng_itr<decltype(v2)>>;

            cmp_v.erase(cmp_v.begin(), cmp_v.end());
            join_with(v, v2, to_back(cmp_v));

            iter_pair<iter_t> p(iter_t{},
                                iter_t(end(v), {}, end(v),
                                       begin(v2), end(v2), end(v2)));
            if (j == 0) {
              auto it = find_if(v, not_fn(empty));
              if (it == end(v))
                p.first = p.second;
              else
                p.first = iter_t(it, begin(*it), end(v),
                                 begin(v2), end(v2), end(v2));
            }
            else {
              auto it = begin(v);
              if (empty(*it)) {
                ++it;
                if (it == end(v))
                  p.first = p.second;
                else
                  p.first = iter_t(it, begin(*it), end(v),
                                   begin(v2), begin(v2), end(v2));
              }
              else
                p.first = iter_t(it, begin(*it), end(v),
                                 begin(v2), end(v2), end(v2));
            }

            test_bitr(p, cmp_v);
          }
        }
    }
  }

  // special member functions
  {
    ez_vector<ez_vector<int>> v = {{1}, {2, 3}, {4, 5, 6}};
    ez_vector<int> v2 = {0};
    ez_vector<ez_vector<int>> vv = {{10}, {20, 30}, {40, 50, 60}};
    ez_vector<int> vv2 = {100};

    using iter_t = join_with_iterator<rng_itr<decltype(v)>,
                                      rng_itr<decltype(v2)>>;
    using pair_t = iter_pair<iter_t>;
    test_ownership<pair_t>
      (([&](pair_t *p) {
        new(p) pair_t(iter_t(begin(v), begin(*begin(v)), end(v),
                             begin(v2), end(v2), end(v2)),
                      iter_t(end(v), {}, end(v),
                             begin(v2), end(v2), end(v2)));
      }),
        [](const pair_t *p) {
          return equal(*p, ez_vector{1, 0, 2, 3, 0, 4, 5, 6});
        },
        [&](pair_t *p) {
          new(p) pair_t(iter_t(begin(vv), begin(*begin(vv)), end(vv),
                               begin(vv2), end(vv2), end(vv2)),
                        iter_t(end(vv), {}, end(vv),
                               begin(vv2), end(vv2), end(vv2)));
        },
        [](const pair_t *p) {
          return equal(*p, ez_vector{10, 100, 20, 30, 100, 40, 50, 60});
        },
        [](...) {return true;});
  }

  // operations with const iterator
  {
    ez_vector<ez_vector<int>> v = {{1, 2}, {4, 5}};
    ez_vector<int> v2 = {3};

    using iter_t = join_with_iterator<rng_itr<decltype(v)>,
                                      rng_itr<decltype(v2)>>;
    using c_iter_t = join_with_iterator<rng_itr<decltype(as_const(v))>,
                                        rng_itr<decltype(as_const(v2))>>;
    iter_t i(begin(v), begin(*begin(v)), end(v),
             begin(v2), end(v2), end(v2));
    iter_t j(end(v), {}, end(v),
             begin(v2), end(v2), end(v2));
    assert(equal(rng(i, j), irng(1, 6)));
    c_iter_t ci = i;
    c_iter_t cj = j;
    assert(equal(rng(ci, cj), irng(1, 6)));
    assert(i == ci && j == cj);
  }

  // join_with_itr
  {
    ez_vector<ez_vector<int>> v = {{1, 2}, {3, 4}};
    ez_vector<int> v2 = {0, 0};
    test_rng(rng(join_with_itr(begin(v), begin(front(v)), end(v),
                               begin(v2), end(v2), end(v2)),
                 join_with_itr(end(v), {}, end(v),
                               begin(v2), end(v2), end(v2))),
             ez_vector{1, 2, 0, 0, 3, 4});
  }
}
void test_join_with_range() {
  // compile-time info
  {
    using r0_t = join_with_range<ez_vector<ez_vector<int>>, ez_vector<int>>;
    using r1_t = join_with_range<ez_vector<ez_vector<int>> &, ez_vector<int>>;
    using r2_t = join_with_range<ez_vector<ez_vector<int>>, ez_vector<int> &>;
    using r3_t = join_with_range<ez_vector<ez_vector<int>> &, ez_vector<int> &>;
    using r4_t = join_with_range<iter_pair<ez_vector<int> *>, iter_pair<int *>>;

    // special member functions
    {
      static_assert(semiregular<r0_t>);
      static_assert(!semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(semiregular<r4_t>);

      static_assert(!is_nothrow_copy_assignable<r0_t>);
      static_assert(is_nothrow_copy_assignable<r4_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r0_t>);
      static_assert(!is_rng<const r0_t>);
      static_assert(is_rng<r1_t>);
      static_assert(!is_rng<const r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(!is_rng<const r2_t>);
      static_assert(is_rng<r3_t>);
      static_assert(is_rng<const r3_t>);
      static_assert(is_rng<r4_t>);
      static_assert(is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r0_t>);
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r0_t>);
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(!rng_is_sized<r4_t>);
    }
  }
  // with no cache
  {
    // special member functions
    {
      using r_t = join_with_range
        <iter_pair<ez_vector<ez_vector<int>>::iterator>,
         iter_pair<ez_vector<int>::iterator>>;
      ez_vector<ez_vector<int>> v = {{1, 2}, {4, 5}};
      ez_vector<int> v2 = {3};
      ez_vector<ez_vector<int>> vv = {{6}, {7, 8, 9}};
      ez_vector<int> vv2 = {};
      test_ownership<r_t>([&](r_t *p) {new(p) r_t(rng(v), rng(v2));},
                          [](const r_t *p) {return equal(*p, irng(1, 6));},
                          [&](r_t *p) {new(p) r_t(rng(vv), rng(vv2));},
                          [](const r_t *p) {return equal(*p, irng(6, 10));},
                          [](const r_t *) {return true;});

      using r2_t = join_with_range<ez_vector<ez_vector<int>>, ez_vector<int>>;
      r2_t x(v, v2), y(vv, vv2);
      assert(addressof(x = y) == addressof(x));
      test_rng(x, irng(6, 10));
      test_rng(y, irng(6, 10));
    }
    // construct from (r, r2)
    // range operations
    {
      using r_t = join_with_range<ez_vector<ez_vector<int>> &,
                                  ez_vector<int> &>;
      ez_vector<int> cmp_v;
      for (int &i : iters(0, 3)) {
        ez_vector<int> v2;
        copy(rng(i, 0), to_back(v2));
        ez_vector<ez_vector<int>> v = {{}, {}, {}, {1, 2}, {3, 4}, {5, 6, 7}};

        while (find_if(v, not_fn(empty)) != v.end()) {
          do {
            for (int &j : iters(0, 6)) {
              cmp_v.erase(cmp_v.begin(), cmp_v.end());
              join_with(v, v2, to_back(cmp_v));

              r_t r(v, v2);

              assert(r.empty() == (r.begin() == r.end()));
              test_bitr(r, cmp_v);
              assert(as_const(r).empty() == (r.begin() == r.end()));
              test_bitr(as_const(r), cmp_v);
            }
          } while (next_permutation(v));

          v.erase(before_end(v), end(v));
        }
      }
    }
  }
  // with cache
  {
    // special member functions
    {
      using r_t = join_with_range<ez_vector<ez_vector<int>>, ez_vector<int>>;
      ez_vector<ez_vector<int>> v = {{1, 2}, {4, 5}};
      ez_vector<int> v2 = {3};
      ez_vector<ez_vector<int>> vv = {{6}, {7, 8, 9}};
      ez_vector<int> vv2 = {};
      test_ownership<r_t>([&](r_t *p) {new(p) r_t(v, v2);},
                          [](r_t *p) {
                            const bool ret = equal(*p, irng(1, 6));
                            *p = *p;
                            return ret;
                          },
                          [&](r_t *p) {new(p) r_t(vv, vv2);},
                          [](r_t *p) {
                            const bool ret = equal(*p, irng(6, 10));
                            *p = *p;
                            return ret;
                          },
                          [](r_t *p) {
                            return p->empty() && p->begin() == p->end();
                          });
    }
    // construct from (r, r2)
    // range operations
    {
      using r_t = join_with_range<ez_vector<ez_vector<int>>, ez_vector<int>>;
      ez_vector<int> cmp_v;
      for (int &i : iters(0, 4)) {
        ez_vector<int> v2;
        copy(rng(i, 0), to_back(v2));
        ez_vector<ez_vector<int>> v = {{}, {}, {}, {1, 2}, {3, 4}, {5, 6, 7}};

        while (find_if(v, not_fn(empty)) != v.end()) {
          do {
            cmp_v.erase(cmp_v.begin(), cmp_v.end());
            join_with(v, v2, to_back(cmp_v));

            r_t r(v, v2);

            assert(r.empty() == (r.begin() == r.end()));
            test_rng(r, cmp_v);
          } while (next_permutation(v));

          v.erase(before_end(v), end(v));
        }
      }
    }
  }
  // join_with_rng
  {
    test_rng(join_with_rng(ez_vector{ez_vector{1, 2}, ez_vector{3, 4}},
                           ez_vector<int>{}), irng(1, 5));
    test_rng(join_with_rng(ez_vector{ez_vector{1}, ez_vector{3}},
                           ez_vector{2}), irng(1, 4));
    {
      ez_vector<ez_vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
      ez_vector<int> vv = {0};
      auto r = join_with_rng(v, vv);
      front(vv) = 10;
      front(front(v)) = 0;
      test_rng(r, ez_vector{0, 2, 10, 3, 4, 10, 5, 6});
    }
    {
      ez_vector<ez_vector<int>> v = {{1, 2}, {3, 4}, {5, 6}};
      ez_vector<int> vv = {0};
      auto r = join_with_rng(copy(v), vv);
      front(vv) = 10;
      front(front(v)) = 0;
      test_rng(r, ez_vector{1, 2, 10, 3, 4, 10, 5, 6});
    }
  }
}
void test_adjacent() {
  {
    int a[] = {1, 2, 3};
    ez_vector<tuple<int, int, int, int>> v;
    assert(adjacent_plus<4>(a, begin(v)) == pair(end(a), begin(v)));
  }
  {
    int a[] = {1, 2, 3};
    ez_vector<tuple<int, int>> v = {{}, {}};
    assert(adjacent_plus<2>(a, begin(v)) == pair(end(a), end(v)));
    assert(equal(v, seq(tuple(1, 2), tuple(2, 3))));
  }

  {
    using v_t = ez_vector<tuple<int>>;
    v_t v = adjacent<1>(irng(1, 4), to_back(v_t{})).base();
    assert(equal(v, v_t{1, 2, 3}));
    v = adjacent<1>(irng(1, 3), to_back(v_t{})).base();
    assert(equal(v, v_t{1, 2}));
    v = adjacent<1>(irng(1, 2), to_back(v_t{})).base();
    assert(equal(v, v_t{1}));
    v = adjacent<1>(irng(1, 1), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
  }
  {
    using v_t = ez_vector<tuple<int, int>>;
    v_t v = adjacent<2>(irng(1, 4), to_back(v_t{})).base();
    assert(equal(v, v_t{{1, 2}, {2, 3}}));
    v = adjacent<2>(irng(1, 3), to_back(v_t{})).base();
    assert(equal(v, v_t{{1, 2}}));
    v = adjacent<2>(irng(1, 2), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
    v = adjacent<2>(irng(1, 1), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
  }
  {
    using v_t = ez_vector<tuple<int, int, int, int>>;
    v_t v = adjacent<4>(irng(1, 5), to_back(v_t{})).base();
    assert(equal(v, v_t{{1, 2, 3, 4}}));
    v = adjacent<4>(irng(1, 4), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
    v = adjacent<4>(irng(1, 3), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
    v = adjacent<4>(irng(1, 1), to_back(v_t{})).base();
    assert(equal(v, v_t{}));
    v = adjacent<4>(irng(1, 6), to_back(v_t{})).base();
    assert(equal(v, v_t{{1, 2, 3, 4}, {2, 3, 4, 5}}));
    v = adjacent<4>(irng(1, 7), to_back(v_t{})).base();
    assert(equal(v, v_t{{1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}}));
  }
}
void test_adjacent_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = adjacent_iterator<int *, 1>;
        static_assert(same_as<iter_t::value_type, tuple<int>>);
        static_assert(same_as<iter_t::reference, tuple<int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
      {
        using iter_t = adjacent_iterator<ez_forward_list<int>::iterator, 3>;
        static_assert(same_as<iter_t::value_type, tuple<int, int, int>>);
        static_assert(same_as<iter_t::reference, tuple<int &, int &, int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);
      }
      {
        using iter_t = adjacent_iterator
          <ez_forward_list<int>::const_iterator, 5>;
        static_assert(same_as
                      <iter_t::value_type,
                       tuple<int, int, int, int, int>>);
        static_assert(same_as
                      <iter_t::reference,
                       tuple<const int &, const int &, const int &,
                             const int &, const int &>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      using iter_t = adjacent_iterator<ez_forward_list<int>::iterator, 3>;

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
        using iter_t = adjacent_iterator<ez_forward_list<int>::iterator, 3>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = adjacent_iterator<ez_list<int>::iterator, 3>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = adjacent_iterator<int *, 3>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = adjacent_iterator<int *, 3>;
      using c_iter_t = adjacent_iterator<const int *, 3>;
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

  // construct from tuple
  // iterator operations
  {
    {
      using iter_t = adjacent_iterator<int *, 1>;
      using c_iter_t = adjacent_iterator<const int *, 1>;
      int a[] = {1, 2, 3, 4};
      test_ritr(iter_t(tuple(begin(a))), iter_t(tuple(end(a))),
                bind_rng(irng(1, 5), make_tuple));
      test_ritr(c_iter_t(tuple(begin(a))), iter_t(tuple(end(a))),
                bind_rng(irng(1, 5), make_tuple));
      test_ritr(iter_t(tuple(begin(a))), c_iter_t(tuple(end(a))),
                bind_rng(irng(1, 5), make_tuple));
      test_ritr(c_iter_t(tuple(begin(a))), c_iter_t(tuple(end(a))),
                bind_rng(irng(1, 5), make_tuple));
    }
    {
      using iter_t = adjacent_iterator<int *, 2>;
      using c_iter_t = adjacent_iterator<const int *, 2>;
      int a[] = {1, 2, 3, 4};
      const auto cmp_r = bind_rng(irng(1, 4),
                                  [](int x) {return tuple(x, x + 1);});
      test_ritr(iter_t(tuple(begin(a), begin(a) + 1)),
                iter_t(tuple(before_end(a), end(a))),
                cmp_r);
      test_ritr(c_iter_t(tuple(begin(a), begin(a) + 1)),
                iter_t(tuple(before_end(a), end(a))),
                cmp_r);
      test_ritr(iter_t(tuple(begin(a), begin(a) + 1)),
                c_iter_t(tuple(before_end(a), end(a))),
                cmp_r);
      test_ritr(c_iter_t(tuple(begin(a), begin(a) + 1)),
                c_iter_t(tuple(before_end(a), end(a))),
                cmp_r);
    }
    {
      using l_t = ez_forward_list<int>;
      using iter_t = adjacent_iterator<l_t::iterator, 4>;
      using c_iter_t = adjacent_iterator<l_t::const_iterator, 4>;
      l_t l = {1, 2, 3, 4, 5, 6};
      const auto cmp_r = bind_rng(irng(1, 4), [](int x) {
        return tuple(x, x + 1, x + 2, x + 3);
      });
      test_fitr(iter_t(tuple(nth(l, 0), nth(l, 1), nth(l, 2), nth(l, 3))),
                iter_t(tuple(nth(l, 3), nth(l, 4), nth(l, 5), nth(l, 6))),
                cmp_r);
    }
  }

  // special member functions
  {
    using iter_t = adjacent_iterator<int *, 3>;
    int x = 1;
    const auto init_f1 = [&x](iter_t *p) {
      new(p) iter_t(tuple(addressof(x), addressof(x), addressof(x)));
    };
    const auto eq_f1 = [&x](iter_t *p) {
      return p->base() == tuple(addressof(x), addressof(x), addressof(x));
    };
    const auto init_f2 = [](iter_t *p) {
      new(p) iter_t();
    };
    const auto eq_f2 = [](iter_t *p) {
      return p->base() == tuple(nullptr, nullptr, nullptr);
    };
    test_ownership<iter_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](iter_t *) {return true;});
  }

  // adjacent_itr
  {
    int a[] = {1, 2, 3, 4, 5};
    assert(equal(rng(adjacent_itr(nth(a, 0), nth(a, 1), nth(a, 2)),
                     adjacent_itr(nth(a, 3), nth(a, 4), nth(a, 5))),
                 ez_vector{tuple(1, 2, 3), tuple(2, 3, 4), tuple(3, 4, 5)}));
  }
}
void test_adjacent_range() {
  // compile-time info
  {
    using r0_t = adjacent_range<int [2], 2>;
    using r1_t = adjacent_range<ez_forward_list<int>, 2>;
    using r2_t = adjacent_range<ez_vector<int> &, 3>;
    using r3_t = adjacent_range<iter_pair<int *>, 3>;

    // special member functions
    {
      // static_assert(!semiregular<r0_t>);
      static_assert(copyable<r0_t> && is_default_constructible<r0_t>);
      static_assert(semiregular<r1_t>);
      static_assert(!semiregular<r2_t>);
      static_assert(semiregular<r3_t>);

      static_assert(is_swappable<r0_t>);
      static_assert(is_swappable<r1_t>);
      static_assert(!is_swappable<r2_t>);
      static_assert(is_swappable<r3_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r0_t>);
      static_assert(is_rng<r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(is_rng<r3_t>);

      using r4_t = adjacent_range<filter_range<ez_vector<int>, bool (*)(int)>,
                                  2>;
      static_assert(is_rng<r4_t>);
      static_assert(!is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r0_t>);
      static_assert(!is_rng_ref<r1_t>);
      static_assert(is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
    }
    // rng_is_sized
    {
      static_assert(rng_is_sized<r0_t>);
      static_assert(!rng_is_sized<r1_t>);
      static_assert(rng_is_sized<r2_t>);
      static_assert(rng_is_sized<r3_t>);
    }
  }
  // construct from r
  // range operations
  {
    // N = 1
    {
      using r_t = adjacent_range<ez_vector<int> &, 1>;
      ez_vector<int> v = {1, 2, 3, 4};
      r_t r(v);
      const auto &cr = r;

      assert(!r.empty());
      assert(equal(r, bind_rng(irng(1, 5), make_tuple)));
      assert(!cr.empty());
      assert(equal(cr, bind_rng(irng(1, 5), make_tuple)));

      v = {};
      assert(r.empty() && r.begin() == r.end());
      assert(cr.empty() && cr.begin() == cr.end());
    }
    // N = 2
    {
      using r_t = adjacent_range<ez_vector<int> &, 2>;
      ez_vector<int> v = {1, 2, 3, 4};
      r_t r(v);
      const auto &cr = r;

      assert(!r.empty());
      assert(equal(r, bind_rng(irng(1, 4),
                               [](int x) {return tuple(x, x + 1);})));
      assert(!cr.empty());
      assert(equal(cr, bind_rng(irng(1, 4),
                                [](int x) {return tuple(x, x + 1);})));

      v = {1, 2};
      assert(!r.empty());
      assert(equal(r, ez_vector{pair(1, 2)}));
      assert(!cr.empty());
      assert(equal(cr, ez_vector{pair(1, 2)}));

      v = {1};
      assert(r.empty() && r.begin() == r.end() && r.size() == 0);
      assert(cr.empty() && cr.begin() == cr.end() && r.size() == 0);
      v = {};
      assert(r.empty() && r.begin() == r.end() && r.size() == 0);
      assert(cr.empty() && cr.begin() == cr.end() && r.size() == 0);
    }
    // N = 5
    {
      using r_t = adjacent_range<ez_vector<int>, 5>;
      r_t r(copy(irng(1, 99), to_back(ez_vector<int>{})).base());
      assert(equal
             (r, bind_rng(irng(1, 95),
                          [](int x) {
                            return tuple(x, x + 1, x + 2, x + 3, x + 4);
                          })));
      test_ritr(r, bind_rng(irng(1, 95),
                            [](int x) {
                              return tuple(x, x + 1, x + 2, x + 3, x + 4);
                            }));

      r = r_t(copy(irng(1, 5), to_back(ez_vector<int>{})).base());
      assert(r.empty() && r.size() == 0 && r.begin() == r.end());

      r = r_t(copy(irng(1, 6), to_back(ez_vector<int>{})).base());
      assert(r.size() == 1 && !r.empty()
             && next(r.begin()) == r.end() && prev(r.end()) == r.begin());
      assert(front(r) == tuple(1, 2, 3, 4, 5));
      test_ritr(r, ez_vector{tuple(1, 2, 3, 4, 5)});

      r = r_t{};
      assert(r.empty() && r.size() == 0 && r.begin() == r.end());
    }
  }
  
  // special member functions
  {
    using r_t = adjacent_range<ez_vector<int>, 2>;
    const auto init_f1 = [](r_t *p) {new(p) r_t{};};
    const auto eq_f1 = [](r_t *p) {return p->empty();};
    const auto init_f2 = [](r_t *p) {new(p) r_t({1, 2, 3});};
    const auto eq_f2 = [](r_t *p) {
      return equal(*p, ez_vector{pair(1, 2), pair(2, 3)});
    };
    test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                        [](r_t *) {return true;});
  }

  // adjacent_rng
  {
    assert(adjacent_rng<4>(irng(1, 4)).empty());
    assert(equal(adjacent_rng<4>(irng(1, 5)), ez_vector{tuple(1, 2, 3, 4)}));
  }
  // bind_adjacent_rng
  {
    int a[] = {1, 2, 3};
    auto f = [](int x, int y, int z) {return x * 100 + y * 10 + z;};
    auto r = bind_adjacent_rng<3>(a, f);
    assert(r.size() == 1 && front(r) == 123);
  }
}
void test_slide() {
  int a[] = {1, 2, 3, 4, 5};
  ez_vector<ez_vector<int>> v;
  const auto f = [&v](auto r) {
    ez_vector<int> tmp;
    copy(r, to_back(tmp));
    v.insert(v.end(), move(tmp));
  };

  slide(a, 3, output_itr(f));
  assert(equal(v, ez_vector<ez_vector<int>>{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}}));

  v.erase(v.begin(), v.end());
  slide(a, 1, output_itr(f));
  assert(equal(v, ez_vector<ez_vector<int>>{{1}, {2}, {3}, {4}, {5}}));

  v.erase(v.begin(), v.end());
  slide(a, 5, output_itr(f));
  assert(equal(v, ez_vector<ez_vector<int>>{{1, 2, 3, 4, 5}}));

  v.erase(v.begin(), v.end());
  slide(a, 6, output_itr(f));
  assert(v.empty());

  int aa[] = {1};
  v.erase(v.begin(), v.end());
  slide(aa, 1, output_itr(f));
  assert(equal(v, ez_vector<ez_vector<int>>{{1}}));
  v.erase(v.begin(), v.end());
  slide(aa, 2, output_itr(f));
  assert(equal(v, ez_vector<ez_vector<int>>{}));
}
void test_slide_iterator() {
  // compile-time info
  {
    // member types
    {
      {
        using iter_t = slide_iterator<int *>;
        static_assert(same_as<iter_t::value_type, iter_pair<int *>>);
        static_assert(same_as<iter_t::reference, iter_pair<int *>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as
                      <iter_t::iterator_category, random_access_iterator_tag>);
      }
      {
        using iter0_t = ez_forward_list<int>::iterator;
        using iter_t = slide_iterator<iter0_t>;
        static_assert(same_as<iter_t::value_type, iter_pair<iter0_t>>);
        static_assert(same_as<iter_t::reference, iter_pair<iter0_t>>);
        static_assert(same_as<iter_t::difference_type, ptrdiff_t>);
        static_assert(same_as<iter_t::pointer, void>);
        static_assert(same_as<iter_t::iterator_category, forward_iterator_tag>);
      }
    }
    // special member functions
    {
      using iter_t = slide_iterator<ez_forward_list<int>::iterator>;

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
        using iter_t = slide_iterator<ez_forward_list<int>::iterator>;
        static_assert(forward_iterator<iter_t>);
        static_assert(!bidirectional_iterator<iter_t>);
        static_assert(!cpp17_forward_iterator<iter_t>);
        static_assert(is_just_fitr<iter_t>);
      }
      // as bidirectional iterator
      {
        using iter_t = slide_iterator<ez_list<int>::iterator>;
        static_assert(bidirectional_iterator<iter_t>);
        static_assert(!random_access_iterator<iter_t>);
        static_assert(is_just_bitr<iter_t>);
      }
      // as random access iterator
      {
        using iter_t = slide_iterator<int *>;
        static_assert(random_access_iterator<iter_t>);
        static_assert(!contiguous_iterator<iter_t>);
        static_assert(is_just_ritr<iter_t>);
      }
    }
    // iterator and const iterator
    {
      using iter_t = slide_iterator<int *>;
      using c_iter_t = slide_iterator<const int *>;
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

  // construct from (i, ii)
  // iterator operations
  {
    {
      using iter_t = slide_iterator<int *>;
      using c_iter_t = slide_iterator<const int *>;
      int a[] = {1, 2, 3, 4};
      const ez_vector<ez_vector<int>> v_cmp{{1, 2}, {2, 3}, {3, 4}};
      test_ritr(iter_t(nth(a, 0), nth(a, 1)), iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_ritr(c_iter_t(nth(a, 0), nth(a, 1)), iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_ritr(iter_t(nth(a, 0), nth(a, 1)), c_iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_ritr(c_iter_t(nth(a, 0), nth(a, 1)), c_iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
    }
    {
      using iter_t = slide_iterator<ez_forward_list<int>::iterator>;
      using c_iter_t = slide_iterator<ez_forward_list<int>::const_iterator>;
      ez_forward_list a = {1, 2, 3, 4};
      const ez_vector<ez_vector<int>> v_cmp{{1, 2}, {2, 3}, {3, 4}};
      test_fitr(iter_t(nth(a, 0), nth(a, 1)), iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_fitr(c_iter_t(nth(a, 0), nth(a, 1)), iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_fitr(iter_t(nth(a, 0), nth(a, 1)), c_iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
      test_fitr(c_iter_t(nth(a, 0), nth(a, 1)), c_iter_t(nth(a, 3), nth(a, 4)),
                v_cmp, equal);
    }
  }

  // special member functions
  {
    using iter_t = slide_iterator<int *>;
    int x = 1;
    const auto init_f1 = [&x](iter_t *p) {
      new(p) iter_t(addressof(x), addressof(x) + 1);
    };
    const auto eq_f1 = [&x](iter_t *p) {
      return p->base() == pair(addressof(x), addressof(x) + 1);
    };
    const auto init_f2 = [](iter_t *p) {
      new(p) iter_t();
    };
    const auto eq_f2 = [](iter_t *p) {
      return p->base() == pair(nullptr, nullptr);
    };
    test_ownership<iter_t>(init_f1, eq_f1, init_f2, eq_f2,
                           [](iter_t *) {return true;});
  }

  // slide_itr
  {
    int a[] = {1, 2, 3};
    test_ritr(slide_itr(nth(a, 0), 1), slide_itr(nth(a, 3), 1),
              ez_vector<ez_vector<int>>{{1}, {2}, {3}}, equal);
    test_ritr(slide_itr(nth(a, 0), nth(a, 2)), slide_itr(nth(a, 1), nth(a, 3)),
              ez_vector<ez_vector<int>>{{1, 2, 3}}, equal);
  }
}
void test_slide_range() {
  // compile-time info
  {
    using r0_t = slide_range<ez_forward_list<int>>;
    using r1_t = slide_range<ez_bidirectional_list<int>>;
    using r2_t = slide_range<ez_list<int>>;
    using r3_t = slide_range<ez_forward_list<int> &>;
    using r4_t = slide_range<ez_vector<int> &>;

    // special member functions
    {
      static_assert(semiregular<r0_t>);
      static_assert(semiregular<r1_t>);
      static_assert(semiregular<r2_t>);
      static_assert(!semiregular<r3_t>);
      static_assert(!semiregular<r4_t>);

      static_assert(!is_default_constructible<r3_t>);
      static_assert(is_nothrow_copy_constructible<r3_t>);
      static_assert(is_nothrow_move_constructible<r3_t>);
      static_assert(!is_copy_assignable<r3_t>);
      static_assert(!is_move_assignable<r3_t>);

      static_assert(!is_default_constructible<r4_t>);
      static_assert(is_nothrow_copy_constructible<r4_t>);
      static_assert(is_nothrow_move_constructible<r4_t>);
      static_assert(!is_copy_assignable<r4_t>);
      static_assert(!is_move_assignable<r4_t>);
    }
    // is_rng
    {
      static_assert(is_rng<r0_t>);
      static_assert(is_rng<r1_t>);
      static_assert(is_rng<r2_t>);
      static_assert(is_rng<r3_t>);
      static_assert(is_rng<r4_t>);

      static_assert(!is_rng<const r0_t>);
      static_assert(!is_rng<const r1_t>);
      static_assert(!is_rng<const r2_t>);
      static_assert(is_rng<const r3_t>);
      static_assert(is_rng<const r4_t>);
    }
    // is_rng_ref
    {
      static_assert(!is_rng_ref<r0_t>);
      static_assert(!is_rng_ref<r1_t>);
      static_assert(!is_rng_ref<r2_t>);
      static_assert(is_rng_ref<r3_t>);
      static_assert(is_rng_ref<r4_t>);
    }
    // rng_is_sized
    {
      static_assert(!rng_is_sized<r0_t>);
      static_assert(!rng_is_sized<r1_t>);
      static_assert(!rng_is_sized<r2_t>);
      static_assert(!rng_is_sized<r3_t>);
      static_assert(rng_is_sized<r4_t>);

      static_assert(rng_is_sized<const r4_t>);
    }
  }
  // special member functions
  {
    // frng
    {
      using r_t = slide_range<ez_forward_list<int>>;
      ez_forward_list<int> l = {1, 2, 3, 4};
      ez_forward_list<int> ll = {1};

      const auto init_f1 = [&](r_t *p) {new(p) r_t(l, 2);};
      const auto eq_f1 = [&](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}, {3, 4}},
                     equal);
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t(ll, 2);};
      const auto eq_f2 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{}, equal);
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    // brng
    {
      using r_t = slide_range<ez_list<int>>;
      ez_list<int> l = {1, 2, 3, 4};
      ez_list<int> ll = {1};

      const auto init_f1 = [&](r_t *p) {new(p) r_t(l, 2);};
      const auto eq_f1 = [&](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}, {3, 4}},
                     equal);
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t(ll, 2);};
      const auto eq_f2 = [](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{}, equal);
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          bind(empty, bind(deref, _1)));
    }
    // frng ref
    {
      using r_t = slide_range<iter_pair<ez_forward_list<int>::iterator>>;
      ez_forward_list<int> l = {1, 2, 3, 4};
      ez_forward_list<int> ll = {1};

      const auto init_f1 = [&](r_t *p) {new(p) r_t(rng(l), 2);};
      const auto eq_f1 = [&](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}, {3, 4}},
                     equal);
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t(rng(ll), 2);};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && as_const(p)->empty() && p->begin() == p->end()
          && cbegin(*p) == cend(*p);
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
    // brng ref
    {
      using r_t = slide_range<iter_pair<ez_list<int>::iterator>>;
      ez_list<int> l = {1, 2, 3, 4};
      ez_list<int> ll = {1};

      const auto init_f1 = [&](r_t *p) {new(p) r_t(rng(l), 2);};
      const auto eq_f1 = [&](r_t *p) {
        return equal(*p, ez_vector<ez_vector<int>>{{1, 2}, {2, 3}, {3, 4}},
                     equal);
      };
      const auto init_f2 = [&](r_t *p) {new(p) r_t(rng(ll), 2);};
      const auto eq_f2 = [](r_t *p) {
        return p->empty() && as_const(p)->empty() && p->begin() == p->end()
          && cbegin(*p) == cend(*p);
      };
      test_ownership<r_t>(init_f1, eq_f1, init_f2, eq_f2,
                          [](auto) {return true;});
    }
  }
  // construct from (r, n)
  // range operations
  {
    ez_forward_list<int> l;
    ez_vector<int> v;
    ez_vector<iter_pair<int *>> vv;
    for (int i : irng(1, 5))
      for (int j : irng(1, 6)) {
        l.erase_after(l.before_begin(), l.end());
        v.erase(v.begin(), v.end());
        copy(irng(1, i), to_front(l));
        copy(irng(1, i), to_back(v));

        vv.erase(vv.begin(), vv.end());
        slide(v, j, to_back(vv));

        {
          slide_range<ez_vector<int> &> r(v, j);
          assert(equal(r, vv));
          assert(equal(as_const(r), vv));
        }
        {
          slide_range<ez_vector<int>> r(v, j);
          assert(equal(r, vv, equal));
        }
        {
          slide_range<ez_forward_list<int> &> r(l, j);
          assert(equal(r, vv, equal));
          assert(equal(as_const(r), vv, equal));
        }
        {
          slide_range<ez_forward_list<int>> r(l, j);
          assert(equal(r, vv, equal));
        }
      }
  }
  // slide_rng
  {
    assert(equal(slide_rng(ez_vector{1, 2, 3, 4}, 3),
                 ez_vector<ez_vector<int>>{{1, 2, 3}, {2, 3, 4}},
                 equal));
  }
}

void test_algorithm_by_range() {
  printf("algorithm_by_range: ");

  re::inner::test::test_permutation_combination_count();
  re::inner::test::test_combination_iterator();
  re::inner::test::test_combination_range();
  re::inner::test::test_filter();
  re::inner::test::test_filter_iterator();
  re::inner::test::test_filter_range();
  re::inner::test::test_take();
  re::inner::test::test_take_iterator();
  re::inner::test::test_take_range();
  re::inner::test::test_drop();
  re::inner::test::test_drop_range();
  re::inner::test::test_take_while();
  re::inner::test::test_take_while_iterator();
  re::inner::test::test_take_while_range();
  re::inner::test::test_drop_while();
  re::inner::test::test_drop_while_range();
  re::inner::test::test_join();
  re::inner::test::test_join_iterator();
  re::inner::test::test_join_range();
  re::inner::test::test_join_with();
  re::inner::test::test_join_with_iterator();
  re::inner::test::test_join_with_range();
  re::inner::test::test_adjacent();
  re::inner::test::test_adjacent_iterator();
  re::inner::test::test_adjacent_range();
  re::inner::test::test_slide();
  re::inner::test::test_slide_iterator();
  re::inner::test::test_slide_range();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_algorithm_by_range();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
