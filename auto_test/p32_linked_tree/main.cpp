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

void test_tree_base() {
  using t = linked_tree<int, test_allocator<int>>;

  static_assert(is_same_v<t::key_type, int>);
  static_assert(is_same_v<t::reference, t::value_type &>);
  static_assert(is_same_v<t::const_reference, const t::value_type &>);
  static_assert(is_same_v<t::difference_type, ptrdiff_t>);
  static_assert(is_same_v<t::size_type, size_t>);

  // zero or one node
  {
    t x;
    assert(inner::good(x));
    assert(x.empty() && x == nullptr && x == x);
    assert(x.root() == nullptr);
    assert(as_const(x).root() == nullptr);
    assert(x.croot() == nullptr);

    x.emplace(1);
    assert(inner::good(x));
    assert(!x.empty() && x != nullptr && x == x);
    t::iterator it = x.root();
    t::const_iterator cit = x.croot();
    assert(**it == 1);
    assert(**cit == 1);

    x.clear();
    assert(inner::good(x));
    assert(x.empty() && x == nullptr && x == x);
    assert(x.root() == nullptr);
    assert(as_const(x).root() == nullptr);
    assert(x.croot() == nullptr);
  }
  // one node with one line
  {
    t x;
    x.emplace(1);
    x.append(x.root(), 2, 3, 4);
    assert(inner::good(x));
    assert(!x.empty() && x != nullptr && x == x);

    t y(1, t(2), t(3), t(4));
    assert(inner::good(y));
    assert(x == y);
  }
  // one node with too lines
  {
    t x;
    x.emplace(1);
    x.append(x.root(), 2, 3, 4);
    x.append(nth(*x.root(), 0), 5, 6, 7);
    x.append(nth(*x.root(), 1), 8, 9, 10);
    x.append(nth(*x.root(), 2), 11, 12, 13);
    assert(inner::good(x));
    assert(!x.empty() && x != nullptr && x == x);

    t y(1,
        t(2,
          t(5), t(6), t(7)),
        t(3,
          t(8), t(9), t(10)),
        t(4,
          t(11), t(12), t(13)));
    assert(inner::good(y));
    assert(x == y);
    y.append(nth(*y.root(), 2), 14);
    assert(equal(deref_rng(ref(*y.root(), 2)), seq(11, 12, 13, 14)));
    assert(x != y);
  }
}
void test_tree_iterator() {
  using t = linked_tree<int>;
  using iter_t = t::iterator;
  static_assert(is_bitr<t::iterator>);
  static_assert(bidirectional_iterator<t::iterator>);
  static_assert(bidirectional_iterator<t::const_iterator>);
  static_assert(is_convertible_v<t::iterator, t::const_iterator>);
  static_assert(!is_convertible_v<t::const_iterator, t::iterator>);
  static_assert(equality_comparable_with<t::iterator, t::const_iterator>);

  t x = t(1, t(2), t(3), t(4), t(5, t(6), t(7)));
  test_bitr(deref_rng(*x.root()), seq(2, 3, 4, 5));
  test_bitr(deref_rng(*x.croot()), seq(2, 3, 4, 5));
  assert(x.root()->parent() == iter_t{});
}
void test_tree_special_member_functions() {
  // with stateless allocator
  {
    using t = linked_tree<int, test_allocator<int>>;
    using init_ft = ez_function<void (t *)>;
    using eq_ft = ez_function<bool (const t *)>;
    const auto empt_f = [](const t *p)->bool {
      return p->empty() && *p == nullptr && p->root() == nullptr;
    };
    pair<init_ft, eq_ft> a[] = {
      {
        init_ft([](t *p) {
          new(p) t{};
        }),
        eq_ft([](const t *p)->bool {
          return p->empty() && *p == nullptr && inner::good(*p);
        })
      },
      {
        init_ft([](t *p) {
          new(p) t{};
          p->emplace(1);
        }),
        eq_ft([](const t *p)->bool {
          return !p->empty() && *p != nullptr && inner::good(*p)
            && **p->root() == 1
            && empty(*p->root()) && size(*p->root()) == 0;
        })
      },
      {
        init_ft([](t *p) {
          new(p) t(1,
                   t(2,
                     t(5), t(6), t(7)),
                   t(3,
                     t(8), t(9), t(10)),
                   t(4,
                     t(11), t(12), t(13)));
        }),
        eq_ft([](const t *p)->bool {
          return !p->empty() && *p != nullptr && inner::good(*p)
            && (p->root() != nullptr && **p->root() == 1)
            && (p->root()->size() == 3
                && equal(deref_rng(*p->root()), seq(2, 3, 4)))
            && (nth(*p->root(), 0)->size() == 3
                && equal(deref_rng(*nth(*p->root(), 0)), seq(5, 6, 7))
                && all_of(*nth(*p->root(), 0), empty))
            && (nth(*p->root(), 1)->size() == 3
                && equal(deref_rng(*nth(*p->root(), 1)), seq(8, 9, 10))
                && all_of(*nth(*p->root(), 1), empty))
            && (nth(*p->root(), 2)->size() == 3
                && equal(deref_rng(*nth(*p->root(), 2)), seq(11, 12, 13))
                && all_of(*nth(*p->root(), 2), empty));
        })
      },
      {
        init_ft([](t *p) {
          new(p) t(1,
                   t(2),
                   t(3,
                     t(5),
                     t(6,
                       t(9), t(10), t(11)),
                     t(7)),
                   t(4),
                   t(5,
                     t(8)));
        }),
        eq_ft([](const t *p)->bool {
          return !p->empty() && *p != nullptr && inner::good(*p)
            && *p == t(1,
                       t(2),
                       t(3,
                         t(5),
                         t(6,
                           t(9), t(10), t(11)),
                         t(7)),
                       t(4),
                       t(5,
                         t(8)));
        })
      }
    };
    for (auto &x : a)
      for (auto &y : a)
        test_allocator_aware_container_ownership<t>
          (x.first, x.second, y.first, y.second, empt_f);
  }
  // with stateful allocator
  {
    using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
    using t = linked_tree<int, alloc_t>;
    using init_ft = ez_function<void (t *)>;
    using eq_ft = ez_function<bool (const t *)>;
    const auto empt_f = [](const t *p)->bool {
      return p->empty() && *p == nullptr && p->root() == nullptr;
    };
    vector<pair<init_ft, eq_ft>> v;
    alloc_t a1, a2;
    assert(a1 != a2);
    for (auto a : seq(a1, a2)) {
      v.push_back({
          init_ft([=](t *p) {
            new(p) t{a};
          }),
          eq_ft([](const t *p)->bool {
            return p->empty() && *p == nullptr && inner::good(*p);
          })
        });
      v.push_back({
          init_ft([=](t *p) {
            new(p) t{a};
            p->emplace(1);
          }),
          eq_ft([](const t *p)->bool {
            return !p->empty() && *p != nullptr && inner::good(*p)
              && **p->root() == 1
              && empty(*p->root()) && size(*p->root()) == 0;
          })
        });
      v.push_back({
          init_ft([=](t *p) {
            auto mk = t::maker_with_allocator(a);
            new(p) t(mk(1,
                        mk(2,
                           mk(5), mk(6), mk(7)),
                        mk(3,
                           mk(8), mk(9), mk(10)),
                        mk(4,
                           mk(11), mk(12), mk(13))));
          }),
          eq_ft([](const t *p)->bool {
            return !p->empty() && *p != nullptr && inner::good(*p)
              && (p->root() != nullptr && **p->root() == 1)
              && (p->root()->size() == 3
                  && equal(deref_rng(*p->root()), seq(2, 3, 4)))
              && (nth(*p->root(), 0)->size() == 3
                  && equal(deref_rng(*nth(*p->root(), 0)), seq(5, 6, 7))
                  && all_of(*nth(*p->root(), 0), empty))
              && (nth(*p->root(), 1)->size() == 3
                  && equal(deref_rng(*nth(*p->root(), 1)), seq(8, 9, 10))
                  && all_of(*nth(*p->root(), 1), empty))
              && (nth(*p->root(), 2)->size() == 3
                  && equal(deref_rng(*nth(*p->root(), 2)), seq(11, 12, 13))
                  && all_of(*nth(*p->root(), 2), empty));
          })
        });
      v.push_back({
          init_ft([=](t *p) {
            auto mk = t::maker_with_allocator(a);
            new(p) t(mk(1,
                        mk(2),
                        mk(3,
                           mk(5),
                           mk(6,
                              mk(9), mk(10), mk(11)),
                           mk(7)),
                        mk(4),
                        mk(5,
                           mk(8))));
          }),
          eq_ft([=](const t *p)->bool {
            auto mk = t::maker_with_allocator(a);
            return !p->empty() && *p != nullptr && inner::good(*p)
              && *p == mk(1,
                         mk(2),
                         mk(3,
                           mk(5),
                           mk(6,
                             mk(9), mk(10), mk(11)),
                           mk(7)),
                         mk(4),
                         mk(5,
                           mk(8)));
          })
        });
    }
    for (auto &x : v)
      for (auto &y : v)
        test_allocator_aware_container_ownership<t>
          (x.first, x.second, y.first, y.second, empt_f);
  }
}
void test_tree_construct_assign() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;

  // t(a)
  // t(const t &, a)
  // t(t &&, a)
  {
    alloc_t a1, a2;
    const auto mk = t::maker_with_allocator(a1);
    t x(a1);
    assert(x.empty() && x.get_allocator() == a1);
    t y(x, a2);
    assert(y.empty() && y.get_allocator() == a2);
    t z(mk(1, mk(2, mk(3, mk(4), mk(5)), mk(4))), a2);
    assert(z == mk(1, mk(2, mk(3, mk(4), mk(5)), mk(4))));
    assert(z.get_allocator() == a2);
    t zz(z, a1);
    assert(z == mk(1, mk(2, mk(3, mk(4), mk(5)), mk(4))));
    assert(z.get_allocator() == a2);
    assert(zz == mk(1, mk(2, mk(3, mk(4), mk(5)), mk(4))));
    assert(zz.get_allocator() == a1);
  }

  // t(t_ref, a)
  // =(t_ref)
  {
    alloc_t a1, a2;
    const auto mk1 = t::maker_with_allocator(a1);
    const auto mk2 = t::maker_with_allocator(a2);
    t x = mk1(1, mk1(2), mk1(3));
    tt y(x, a2);
    assert(inner::good(x) && inner::good(y));
    assert(x == mk1(1, mk1(2), mk1(3)) && x == y);
    assert(x.get_allocator() == a1);
    assert(y.get_allocator() == a2);
    t z(a2);
    assert(inner::good(z));
    z.emplace(1);
    assert(!z.empty() && **z.root() == 1 && z.root()->size() == 0
           && z.get_allocator() == a2);
    z = y;
    assert(inner::good(x) && inner::good(y));
    assert(y == mk1(1, mk1(2), mk1(3)) && y.get_allocator() == a2);
    assert(z == mk1(1, mk1(2), mk1(3)) && z.get_allocator() == a2);
    z = move(y);
    assert(inner::good(y) && inner::good(z));
    assert(y.empty() && y.get_allocator() == a2);
    assert(z == mk1(1, mk1(2), mk1(3)) && z.get_allocator() == a2);
    t zz(move(z), a1);
    assert(inner::good(z) && inner::good(zz));
    assert(z.empty() && z.get_allocator() == a2);
    assert(zz == mk1(1, mk1(2), mk1(3)) && zz.get_allocator() == a1);
  }

  // explicit t(in_place, s...)
  // explicit t(key, s...)
  {
    t x(in_place, static_cast<short>(1));
    assert(inner::good(x) && **x.root() == 1 && empty(*x.root()));
    t y(1);
    assert(inner::good(y) && **y.root() == 1 && empty(*y.root()));
    t z(1, t(2, t(4)), t(3, t(5), t(6)));
    assert(inner::good(z));
    assert(**z.root() == 1 && size(*z.root()) == 2);
    assert(size(*nth(*z.root(), 0)) == 1);
    assert(**(nth(*z.root(), 0)->begin()) == 4);
    assert((nth(*z.root(), 0)->begin())->size() == 0);
    assert(empty(*(nth(*z.root(), 1)->begin())));
    assert(empty(*next(nth(*z.root(), 1)->begin())));
    assert(**(nth(*z.root(), 1)->begin()) == 5);
    assert(**next(nth(*z.root(), 1)->begin()) == 6);
  }
  // explicit t(allocator_arg, a, in_place, s...)
  // explicit t(allocator_arg, a, key, s...)
  {
    alloc_t a1, a2;
    t x(allocator_arg, a1, in_place, 1);
    assert(inner::good(x));
    assert(x.get_allocator() == a1);
    assert(empty(*x.root()) && **x.root() == 1);
    t y(allocator_arg, a2, 1, t(allocator_arg, a2, 2));
    assert(inner::good(y));
    assert(y.get_allocator() == a2);
    assert(size(*y.root()) == 1u && **y.root() == 1);
    assert(empty(*y.root()->begin()) && **y.root()->begin() == 2);
  }

  // maker()
  // maker(a)
  {
    const auto mk1 = t::maker();
    alloc_t a;
    const auto mk2 = t::maker_with_allocator(a);
    t x = mk1(1, mk1(2, mk1(3, mk1(4), mk1(5))));
    t y = mk2(1, mk2(2, mk2(3, mk2(4), mk2(5))));
    assert(inner::good(x));
    assert(inner::good(y));
    assert(y.get_allocator() == a);
    assert(x == y);
  }
}
void test_tree_node_operations() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;

  // copy(i, get_key)
  {
    const auto mk = t::maker();
    t x = mk(1,
             mk(2,
                mk(5),
                mk(6,
                   mk(8), mk(9)),
                mk(7)),
             mk(3),
             mk(4));
    t xx = x;
    t x2 = x.copy(x.root());
    t x3 = x.copy(x.croot());
    assert(x == x2 && x == x3 && x == xx);

    t x4 = x.copy(next(x.root()->begin()->begin()),
                  [](auto &u) {return exchange(*u, 0);});
    assert(inner::good(x4) && x4 == mk(6, mk(8), mk(9)));                  
    assert(x == mk(1,
                   mk(2,
                      mk(5),
                      mk(0,
                         mk(0), mk(0)),
                      mk(7)),
                   mk(3),
                   mk(4)));
    assert(x.copy(next(x.root()->cbegin()->cbegin()), deref)
           == mk(0, mk(0), mk(0)));

    tt x5;
    const auto x5_a = x5.get_allocator();
    x5 = x5.copy(x4, x4.root(), deref);
    assert(x5 == x4);
    assert(inner::good(x5));
    assert(x5.get_allocator() == x5_a);
  }

  // swap(i, j)
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);
    t x = mk(1, mk(2, mk(3), mk(4)), mk(5));
    x.swap(nth(*x.root(), 0), nth(*x.root(), 1));
    assert(inner::good(x) && x == mk(1, mk(5), mk(2, mk(3), mk(4))));
    x.swap(x.root(), x.root());
    assert(inner::good(x) && x == mk(1, mk(5), mk(2, mk(3), mk(4))));
    x.swap(nth(*x.root(), 1), nth(*x.root(), 1));
    assert(inner::good(x) && x == mk(1, mk(5), mk(2, mk(3), mk(4))));
  }
  // swap(i, l, j)
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5));
      t y = mk(6, mk(7), mk(8));
      x.swap(x.root(), y, y.root());
      assert(inner::good(x) && inner::good(y));
      assert(x == mk(6, mk(7), mk(8)));
      assert(y == mk(1, mk(2, mk(3), mk(4)), mk(5)));
    }
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5));
      t y = mk(6, mk(7), mk(8));
      x.swap(x.root(), y, y.root()->begin());
      assert(inner::good(x) && inner::good(y));
      assert(x == mk(7));
      assert(y == mk(6, mk(1, mk(2, mk(3), mk(4)), mk(5)), mk(8)));
    }
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5));
      t y = mk(6, mk(7), mk(8));
      y.swap(y.root()->begin(), x, x.root());
      assert(inner::good(x) && inner::good(y));
      assert(x == mk(7));
      assert(y == mk(6, mk(1, mk(2, mk(3), mk(4)), mk(5)), mk(8)));
    }
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5));
      t y = mk(6, mk(7), mk(8));
      x.swap(x.root()->begin(), y, next(y.root()->begin()));
      assert(inner::good(x) && inner::good(y));
      assert(x == mk(1, mk(8), mk(5)));
      assert(y == mk(6, mk(7), mk(2, mk(3), mk(4))));
    }
  }

  // extract(i)->tree_type
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5, mk(6), mk(7)));
      t y = x.extract(x.root());
      assert(inner::good(x) && x.empty());
      assert(inner::good(y)
             && y == mk(1, mk(2, mk(3), mk(4)), mk(5, mk(6), mk(7))));
    }
    {
      t x = mk(1, mk(2, mk(3), mk(4)), mk(5, mk(6), mk(7)));
      t y = x.extract(x.root()->begin());
      assert(inner::good(x) && x == mk(1, mk(5, mk(6), mk(7))));
      assert(inner::good(y) && y == mk(2, mk(3), mk(4)));
    }
  }
  // insert(i, tree_type &&)->iterator
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);
    t x = mk(1, mk(2), mk(3, mk(5), mk(6)), mk(4));
    t y = mk(0, mk(1), mk(2));
    t z;

    assert(x.insert(x.root()->begin(), move(z)) == x.root()->begin());
    assert(inner::good(x));
    assert(x == mk(1, mk(2), mk(3, mk(5), mk(6)), mk(4)));

    const auto it = x.insert(next(x.root()->begin())->end(), move(y));
    assert(inner::good(x));
    assert(x == mk(1, mk(2), mk(3, mk(5), mk(6), mk(0, mk(1), mk(2))), mk(4)));
    assert(**it == 0);
    assert(inner::good(y) && y.empty());

    y = mk(0, mk(1), mk(2));
    assert(**y.insert(next(y.root()->begin()), mk(3)) == 3);
    assert(inner::good(y));
    assert(y == mk(0, mk(1), mk(3), mk(2)));
  }
  // exchange(i, tree_type &&)->tree_type
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);

    t y = mk(0, mk(1), mk(2), mk(3));
    t z = y.exchange(next(y.root()->begin()), t{});
    assert(inner::good(y) && inner::good(z));
    assert(y == mk(0, mk(1), mk(3)));
    assert(z == mk(2));

    y = mk(0, mk(1), mk(2, mk(3), mk(4)));
    z = mk(5, mk(6));
    t zz = y.exchange(next(y.root()->begin()), move(z));
    assert(inner::good(y));
    assert(inner::good(z) && z.empty());
    assert(inner::good(zz));
    assert(y == mk(0, mk(1), mk(5, mk(6))));
    assert(zz == mk(2, mk(3), mk(4)));

    assert(zz.exchange(zz.root(), t{}) == mk(2, mk(3), mk(4)));
    assert(inner::good(zz));
    assert(zz.empty());
    assert(zz.exchange(zz.root(), t{}).empty());
    assert(inner::good(zz));
    assert(zz.empty());
  }
  // replace(i, tree_type &&)->iterator
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);

    t x = mk(1, mk(2), mk(3));
    assert(x.replace(x.root(), t{}) == t::iterator{});
    assert(inner::good(x));
    assert(x == t{});

    x = mk(1, mk(2), mk(3));
    auto it = x.replace(x.root(), mk(4, mk(5)));
    assert(inner::good(x));
    assert(it == x.root());
    assert(x == mk(4, mk(5)));

    t y = mk(5, mk(6));
    it = x.replace(x.root()->begin(), move(y));
    assert(inner::good(x) && inner::good(y));
    assert(it == x.root()->begin() && **it == 5);
    assert(x == mk(4, mk(5, mk(6))));
    assert(y.empty());

    it = x.replace(x.root()->begin(), move(y));
    assert(inner::good(x) && inner::good(y));
    assert(it == x.root()->end());
    assert(x == mk(4));
    assert(y.empty());
  }

  // splice(next, tree_ref, i)
  // splice(next, tree_ref, i, i2)
  {
    alloc_t a;
    const auto mk = t::maker_with_allocator(a);

    t x = mk(1, mk(2), mk(3), mk(4));
    x.splice(nth(*x.root(), 1), x, nth(*x.root(), 2));
    assert(inner::good(x));
    assert(x == mk(1, mk(2), mk(4), mk(3)));
    x.splice(nth(*x.root(), 1), move(x), nth(*x.root(), 2));
    assert(inner::good(x));
    assert(x == mk(1, mk(2), mk(3), mk(4)));

    x = mk(1, mk(2, mk(3), mk(4), mk(5)));
    t y = mk(6, mk(7), mk(8), mk(9));
    x.splice(x.root()->end(), y, y.root()->begin(), y.root()->end());
    assert(inner::good(x) && inner::good(y));
    assert(x == mk(1, mk(2, mk(3), mk(4), mk(5)), mk(7), mk(8), mk(9)));
    assert(y == mk(6));
    x.splice(x.root()->begin(), y, y.root(), y.root());
    assert(inner::good(x) && inner::good(y));
    assert(x == mk(1, mk(2, mk(3), mk(4), mk(5)), mk(7), mk(8), mk(9)));
    assert(y == mk(6));
  }
}
void test_tree_insert_erase() {
  using t = linked_tree<int, test_allocator<int>>;

  // emplace(i, s...)
  {
    t x = t(1, t(2), t(3));
    assert(**x.emplace(x.root()->end(), 4) == 4);
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3), t(4)));
    const int n = 5;
    assert(**x.emplace(x.root()->begin()->end(), n) == 5);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(5)), t(3), t(4)));
  }
  // insert(i, x)
  {
    t x = t(1, t(2), t(3));
    assert(**x.insert(x.root()->end(), 4) == 4);
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3), t(4)));
    const int n = 5;
    assert(**x.insert(x.root()->begin()->end(), n) == 5);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(5)), t(3), t(4)));
  }
  // insert(i, n, x)
  // insert(i, from, to)
  // insert(i, l)
  // insert(i, r)
  // insert_range(i, r)
  {
    t x = t(1);
    auto it = x.insert(x.root()->end(), 2, 3);
    assert(inner::good(x));
    assert(it == x.root()->begin() && **it == 3);
    assert(x == t(1, t(3), t(3)));

    auto a = seq(4, 5);
    it = x.insert(next(x.root()->begin())->end(), begin(a), end(a));
    assert(inner::good(x));
    assert(**it == 4);
    assert(x == t(1, t(3), t(3, t(4), t(5))));
  }

  // erase(i)
  // erase(i, i2)
  // clear(i)
  {
    t x(1);
    assert(x.erase(x.root()) == t::iterator{});
    assert(inner::good(x));
    assert(x.empty());

    x = t(1, t(2, t(3), t(4)), t(5));
    auto it = x.erase(x.root()->begin());
    assert(it == x.root()->begin());
    assert(inner::good(x));
    assert(x == t(1, t(5)));

    assert(x.erase(x.root(), x.root()) == x.root());
    assert(inner::good(x));
    assert(x == t(1, t(5)));
    assert(x.erase(x.root()->begin(), x.root()->begin()) == x.root()->begin());
    assert(inner::good(x));
    assert(x == t(1, t(5)));

    x = t(1, t(2), t(3), t(4, t(5)), t(6), t(7));
    it = x.erase(next(x.root()->begin(), 2), prev(x.root()->end()));
    assert(inner::good(x));
    assert(it == prev(x.root()->end()));
    assert(x == t(1, t(2), t(3), t(7)));

    x.clear();
    assert(inner::good(x));
    assert(x.empty());
  }
}
void test_tree_back_insert_erase() {
  using t = linked_tree<int, test_allocator<int>>;

  // emplace_back(i, s...)
  {
    t x(1, t(2), t(3));
    assert(x.emplace_back(x.root(), 4).key() == 4);
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3), t(4)));
  }

  // push_back(i, k)
  {
    t x(1, t(2, t(3), t(4)));
    x.push_back(x.root()->begin()->begin(), 0);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(3, t(0)), t(4))));
    const int n = 0;
    x.push_back(x.root()->begin()->begin(), n);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(3, t(0), t(0)), t(4))));
  }

  // push_back(i, tree_rref)
  {
    t x(1, t(2));
    x.push_back(x.root(), t(3));
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3)));

    x.push_back(x.root(), t{});
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3)));
  }

  // push_back(i, kr)
  // append_range(i, r)
  // append(i, s...)
  // pop_back(i)
  // pop_back(i, n)
  {
    t x(1, t(2));
    x.push_back(x.root(), seq(3, 4, 5));
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3), t(4), t(5)));
    x.append_range(x.root()->begin(), seq(0));
    assert(inner::good(x));
    assert(x == t(1, t(2, t(0)), t(3), t(4), t(5)));

    x = t(1, t(2));
    x.append(x.root(), 3, t(4, t(5)));
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3), t(4, t(5))));
    x.pop_back(x.root());
    assert(inner::good(x));
    assert(x == t(1, t(2), t(3)));
    x.pop_back(x.root());
    assert(inner::good(x));
    assert(x == t(1, t(2)));
    x.pop_back(x.root());
    assert(inner::good(x));
    assert(x == t(1));
  }
}
void test_tree_front_insert_erase() {
  using t = linked_tree<int, test_allocator<int>>;

  // emplace_front(i, s...)
  {
    t x(1, t(2), t(3));
    assert(x.emplace_front(x.root(), 1).key() == 1);
    assert(inner::good(x));
    assert(x == t(1, t(1), t(2), t(3)));
  }

  // push_front(i, k)
  {
    t x(1, t(2, t(3), t(4)));
    x.push_front(x.root()->begin()->begin(), 1);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(3, t(1)), t(4))));
    const int n = 0;
    x.push_front(x.root()->begin()->begin(), n);
    assert(inner::good(x));
    assert(x == t(1, t(2, t(3, t(0), t(1)), t(4))));
  }

  // push_front(i, tree_rref)
  {
    t x(1, t(2));
    x.push_front(x.root(), t(3));
    assert(inner::good(x));
    assert(x == t(1, t(3), t(2)));

    x.push_front(x.root(), t{});
    assert(inner::good(x));
    assert(x == t(1, t(3), t(2)));
  }

  // push_front(i, kr)
  // prepend_range(i, r)
  // prepend(i, s...)
  // pop_front(i)
  // pop_front(i, n)
  {
    t x(1, t(6));
    x.push_front(x.root(), seq(3, 4, 5));
    assert(inner::good(x));
    assert(x == t(1, t(3), t(4), t(5), t(6)));
    x.prepend_range(x.root()->begin(), seq(0));
    assert(inner::good(x));
    assert(x == t(1, t(3, t(0)), t(4), t(5), t(6)));

    x = t(1, t(2));
    x.prepend(x.root(), 3, t(4, t(5)));
    assert(inner::good(x));
    assert(x == t(1, t(3), t(4, t(5)), t(2)));
    x.pop_front(x.root());
    assert(inner::good(x));
    assert(x == t(1, t(4, t(5)), t(2)));
    x.pop_front(x.root());
    assert(inner::good(x));
    assert(x == t(1, t(2)));
    x.pop_front(x.root());
    assert(inner::good(x));
    assert(x == t(1));
  }
}
void test_tree_first_last_nth_order() {
  using t = linked_tree<int, test_allocator<int>>;
  // first order
  {
    t x;
    assert(inner::good(x));
    assert(empty(deref_rng(x.root().first_order())));
    {
      empty(x.root().nth_order(0));
    }
    x = t(1);
    assert(inner::good(x));
    auto r = deref_rng(x.root()->end().first_order());
    static_assert(bidirectional_iterator<rng_itr<decltype(r)>>);
    test_bitr(r, empty_rng<int>());
    {
      auto r2 = deref_rng(x.root()->end().nth_order(0));
      static_assert(bidirectional_iterator<rng_itr<decltype(r2)>>);
      test_bitr(r2, empty_rng<int>());
    }
    r = deref_rng(x.root().first_order());
    test_bitr(r, seq(1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    x = t(0, t(0), t(1), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).first_order());
    test_bitr(r, seq(1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(0));
      test_bitr(r2, seq(1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1, t(2), t(3));
    assert(inner::good(x));
    r = deref_rng(x.root().first_order());
    test_bitr(r, seq(1, 2, 3));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(x.root().nth_order(0));
      test_bitr(r2, seq(1, 2, 3));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }
    x = t(0, t(0), t(1, t(2), t(3)), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).first_order());
    test_bitr(r, seq(1, 2, 3));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(0));
      test_bitr(r2, seq(1, 2, 3));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1,
          t(2,
            t(3, t(4), t(5)),
            t(6),
            t(7, t(8), t(9, t(10), t(11)))));
    assert(inner::good(x));
    r = deref_rng(x.root().first_order());
    test_bitr(r, irng(1, 12));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(x.root().nth_order(0));
      test_bitr(r2, irng(1, 12));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }
    x = t(0, t(0), t(1,
                     t(2,
                       t(3, t(4), t(5)),
                       t(6),
                       t(7, t(8), t(9, t(10), t(11))))), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).first_order());
    test_bitr(r, irng(1, 12));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(0));
      test_bitr(r2, irng(1, 12));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1,
          t(2, t(3), t(4), t(5)),
          t(6, t(7, t(0), t(0)), t(8), t(9)),
          t(10, t(11), t(12, t(0, t(0, t(0)))), t(13)));
    auto rr = x.root().first_order();
    assert(prev(rr.begin()) == rr.end() && next(rr.end()) == rr.begin());
    for (auto it = rr.begin(); it != rr.end();) {
      if (!it->empty() && all_of_equal(deref_rng(*it), 0)) {
        const auto tmp = it.removed_next();
        x.erase(it.base());
        it = tmp;
      }
      else
        ++it;
    }
    assert(inner::good(x));
    assert(x == t(1,
                  t(2, t(3), t(4), t(5)),
                  t(6, t(8), t(9)),
                  t(10, t(11), t(13))));

    x = t(1, t(2, t(3), t(4)), t(5));
    {
      ez_vector<pair<int, int>> v;
      const auto rrr = x.root().first_order();
      ptrdiff_t depth = 0;
      auto it = rrr.begin();
      while (it != rrr.end()) {
        v.insert(v.end(), pair(**it, depth));
        depth += it.advance();
      }
      assert(equal(v, seq(pair(1, 0), pair(2, 1), pair(3, 2), pair(4, 2),
                          pair(5, 1))));
      assert(depth == -1);
      assert(it.advance() == 1);
    }
  }
  // last order
  {
    t x;
    assert(inner::good(x));
    assert(empty(deref_rng(x.root().last_order())));
    x = t(1);
    assert(inner::good(x));
    auto r = deref_rng(x.root()->end().last_order());
    static_assert(bidirectional_iterator<rng_itr<decltype(r)>>);
    test_bitr(r, empty_rng<int>());
    {
      auto r2 = deref_rng(x.root()->end().nth_order(100));
      static_assert(bidirectional_iterator<rng_itr<decltype(r2)>>);
      test_bitr(r2, empty_rng<int>());
    }
    r = deref_rng(x.root().last_order());
    test_bitr(r, seq(1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(x.root().nth_order(100));
      test_bitr(r2, seq(1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(0, t(0), t(1), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).last_order());
    test_bitr(r, seq(1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(100));
      test_bitr(r2, seq(1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1, t(2), t(3));
    assert(inner::good(x));
    r = deref_rng(x.root().last_order());
    test_bitr(r, seq(2, 3, 1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(x.root().nth_order(100));
      test_bitr(r2, seq(2, 3, 1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }
    x = t(0, t(0), t(1, t(2), t(3)), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).last_order());
    test_bitr(r, seq(2, 3, 1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(100));
      test_bitr(r2, seq(2, 3, 1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1,
          t(2,
            t(3, t(4), t(5)),
            t(6),
            t(7, t(8), t(9, t(10), t(11)))));
    assert(inner::good(x));
    r = deref_rng(x.root().last_order());
    test_bitr(r, seq(4, 5, 3, 6, 8, 10, 11, 9, 7, 2, 1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(x.root().nth_order(100));
      test_bitr(r2, seq(4, 5, 3, 6, 8, 10, 11, 9, 7, 2, 1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }
    x = t(0, t(0), t(1,
                     t(2,
                       t(3, t(4), t(5)),
                       t(6),
                       t(7, t(8), t(9, t(10), t(11))))), t(0));
    assert(inner::good(x));
    r = deref_rng(nth(*x.root(), 1).last_order());
    test_bitr(r, seq(4, 5, 3, 6, 8, 10, 11, 9, 7, 2, 1));
    assert(prev(r.begin()) == r.end() && next(r.end()) == r.begin());
    {
      auto r2 = deref_rng(nth(*x.root(), 1).nth_order(100));
      test_bitr(r2, seq(4, 5, 3, 6, 8, 10, 11, 9, 7, 2, 1));
      assert(prev(r2.begin()) == r2.end() && next(r2.end()) == r2.begin());
    }

    x = t(1,
          t(2, t(3), t(4), t(5)),
          t(6, t(7, t(0), t(0)), t(8), t(9)),
          t(10, t(11), t(12, t(0, t(0, t(0)))), t(13)));
    assert(inner::good(x));
    const auto rr = x.root().last_order();
    assert(prev(rr.begin()) == rr.end() && next(rr.end()) == rr.begin());
    test_bitr(deref_rng(rr),
              seq(3, 4, 5, 2, 0, 0, 7, 8, 9, 6, 11, 0, 0, 0, 12, 13, 10, 1));
    {
      const auto rr2 = x.root().nth_order(100);
      assert(prev(rr2.begin()) == rr2.end() && next(rr2.end()) == rr2.begin());
      test_bitr(deref_rng(rr2),
                seq(3, 4, 5, 2, 0, 0, 7, 8, 9, 6, 11, 0, 0, 0, 12, 13, 10, 1));
    }
  }
  // nth order
  {
    t x = t(1,
            t(2,
              t(3,
                t(5), t(6), t(7)),
              t(8,
                t(9), t(10), t(11))),
            t(12,
              t(13,
                t(14), t(15), t(16)),
              t(17,
                t(18), t(19))));
    const auto r = x.root().nth_order(1);
    static_assert(bidirectional_iterator<rng_itr<decltype(r)>>);
    test_bitr(deref_rng(r), seq(5, 3, 6, 7, 2, 9, 8, 10, 11,
                                1, 14, 13, 15, 16, 12, 18, 17, 19));
    assert(next(r.end()) == r.begin() && prev(r.begin()) == r.end());
    const auto r2 = x.root()->begin().nth_order(1);
    test_bitr(deref_rng(r2), seq(5, 3, 6, 7, 2, 9, 8, 10, 11));
    assert(next(r2.end()) == r2.begin() && prev(r2.begin()) == r2.end());
    const auto r3 = x.root()->end().nth_order(1);
    test_bitr(deref_rng(r3), empty_rng<int>());
  }
}
void test_tree_mscl_for_sequence_container() {
  using t = linked_tree<int, test_allocator<int>>;

  // resize(i, n, x = ...)
  // first_order_resize(i, n, x = ...)
  {
    t x = t(1, t(2), t(3));
    x.resize(x.root(), 1);
    assert(inner::good(x) && x == t(1, t(2)));
    x.resize(x.root(), 5);
    assert(inner::good(x) && x == t(1, t(2), t(0), t(0), t(0), t(0)));

    x = t(1, t(2, t(3), t(4)), t(5));
    x.first_order_resize(x.root(), 1);
    assert(inner::good(x) && x == t(1, t(2, t(3))));
    x.first_order_resize(x.root(), 2, 0);
    assert(inner::good(x) && x == t(1, t(2, t(3), t(0)), t(0)));
  }

  // remove_if(i, eq)
  // first_order_remove_if(i, eq)
  {
    t x = t(1, t(2, t(0), t(3), t(4)), t(0, t(6)), t(5));
    x.first_order_remove_if(x.root(), [](auto &x) {return *x == 0;});
    assert(x == t(1, t(2, t(3), t(4)), t(5)));
    x.first_order_remove_if(x.root(), [](auto &x) {return *x == 3;});
    assert(x == t(1, t(2, t(4)), t(5)));
  }

  // unique(i, eq)
  // first_order_unique(i, eq)
  {
    t x = t(1, t(2), t(2), t(3));
    const auto eqf = [](auto &x, auto &y) {return *x == *y;};
    x.unique(x.root(), eqf);
    assert(inner::good(x) && x == t(1, t(2), t(3)));
    x = t(1, t(2), t(2, t(4), t(4)), t(3, t(5), t(5)));
    x.first_order_unique(x.root(), eqf);
    assert(inner::good(x) && x == t(1, t(2), t(3, t(5))));
  }

  // merge(i, tree_ref, i2, less)
  {
    t x = t(0, t(2), t(4));
    t y = t(0, t(1), t(3), t(5));
    x.merge(x.root(), y, y.root(), [](auto &x, auto &y) {return *x < *y;});
    assert(inner::good(x) && inner::good(y));
    assert(x == t(0, t(1), t(2), t(3), t(4), t(5)));
    assert(y == t(0));
  }

  // sort(i, less)
  // first_order_sort(i, less)
  {
    t x = t(0, t(0), t(2), t(3), t(1), t(4));
    const auto cmp_f = [](auto &x, auto &y) {return *x < *y;};
    x.sort(x.root(), cmp_f);
    assert(inner::good(x) && x == t(0, t(0), t(1), t(2), t(3), t(4)));
    x = t(1, t(3), t(1, t(5), t(4)), t(2));
    x.first_order_sort(x.root(), cmp_f);
    assert(inner::good(x) && x == t(1, t(1, t(4), t(5)), t(2), t(3)));
  }
}

void test_tree_vector_base() {
  using vec_t = linked_tree_vector<int, test_allocator<int>>;
  using t = linked_tree<int, test_allocator<int>>;

  static_assert(is_same_v<vec_t::key_type, int>);
  static_assert(is_same_v<vec_t::reference, vec_t::value_type &>);
  static_assert(is_same_v<vec_t::const_reference, const vec_t::value_type &>);
  static_assert(is_same_v<vec_t::difference_type, ptrdiff_t>);
  static_assert(is_same_v<vec_t::size_type, size_t>);
  static_assert(is_same_v<vec_t::iterator, t::iterator>);
  static_assert(is_same_v<vec_t::const_iterator, t::const_iterator>);
  static_assert(is_same_v<vec_t::tree_type, t::tree_type>);
  static_assert(is_same_v<vec_t::vector_type, t::vector_type>);

  // one level
  {
    vec_t x;
    assert(inner::good(x));
    assert(x.empty() && x.size() == 0);

    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    assert(inner::good(x));
    assert(!x.empty() && x.size() == 3);
    assert(x == x);
  }
  // two levels
  {
    vec_t x(t(1), t(2, t(3), t(4)), t(5));
    assert(equal(deref_rng(x), seq(1, 2, 5)));
    assert(nth(x, 0)->empty());
    assert(nth(x, 2)->empty());
    assert(equal(deref_rng(*nth(x, 1)), seq(3, 4)));
    assert(!nth(x, 1)->empty());
    assert(nth(x, 1)->size() == 2);
    assert(all_of(*nth(x, 1), [](const auto &x) {return x.empty();}));
    assert(x == x);
    vec_t y(t(1), t(2, t(3), t(666)), t(5));
    assert(x != y);
  }
}
void test_tree_vector_special_member_functions() {
  // with stateless allocator
  {
    using t = linked_tree<int, test_allocator<int>>;
    using vec_t = linked_tree_vector<int, test_allocator<int>>;

    using init_ft = ez_function<void (vec_t *)>;
    using eq_ft = ez_function<bool (const vec_t *)>;
    const auto empt_f = [](const vec_t *p)->bool {
      return p->empty() && p->size() == 0 && inner::good(*p);
    };
    pair<init_ft, eq_ft> a[] = {
      {
        init_ft([](vec_t *p) {
          new(p) vec_t{};
        }),
        eq_ft([](const vec_t *p)->bool {
          return p->empty() && p->size() == 0 && inner::good(*p);
        })
      },
      {
        init_ft([](vec_t *p) {
          new(p) vec_t{};
          p->push_back(1);
        }),
        eq_ft([](const vec_t *p)->bool {
          return !p->empty() && p->size() == 1u && inner::good(*p)
            && **p->begin() == 1;
        })
      },
      {
        init_ft([](vec_t *p) {
          new(p) vec_t(t(2,
                         t(5), t(6), t(7)),
                       t(3,
                         t(8), t(9), t(10)),
                       t(4,
                         t(11), t(12), t(13)));
        }),
        eq_ft([](const vec_t *p)->bool {
          return equal(deref_rng((*p)), seq(2, 3, 4))
            && equal(deref_rng(*nth(*p, 0)), seq(5, 6, 7))
            && all_of(*nth(*p, 0), [](const auto &x) {return x.empty();})
            && equal(deref_rng(*nth(*p, 1)), seq(8, 9, 10))
            && all_of(*nth(*p, 1), [](const auto &x) {return x.empty();})
            && equal(deref_rng(*nth(*p, 2)), seq(11, 12, 13))
            && all_of(*nth(*p, 2), [](const auto &x) {return x.empty();});
        })
      },
      {
        init_ft([](vec_t *p) {
          new(p) vec_t(t(2),
                       t(3,
                         t(5),
                         t(6,
                           t(9), t(10), t(11)),
                         t(7)),
                       t(4),
                       t(5,
                         t(8)));
        }),
        eq_ft([](const vec_t *p)->bool {
          return !p->empty() && p->size() == 4 && inner::good(*p)
            && *p == vec_t(t(2),
                       t(3,
                         t(5),
                         t(6,
                           t(9), t(10), t(11)),
                         t(7)),
                       t(4),
                       t(5,
                         t(8)));
        })
      }
    };
    for (auto &x : a)
      for (auto &y : a)
        test_allocator_aware_container_ownership<vec_t>
          (x.first, x.second, y.first, y.second, empt_f);
  }
  // with stateful allocator
  {
    using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
    using t = linked_tree<int, alloc_t>;
    using vec_t = linked_tree_vector<int, alloc_t>;
    using init_ft = ez_function<void (vec_t *)>;
    using eq_ft = ez_function<bool (const vec_t *)>;
    const auto empt_f = [](const vec_t *p)->bool {
      return p->empty() && p->size() == 0 && inner::good(*p);
    };
    vector<pair<init_ft, eq_ft>> v;
    alloc_t a1, a2;
    assert(a1 != a2);
    for (auto a : seq(a1, a2)) {
      v.push_back({
          init_ft([=](vec_t *p) {
            new(p) vec_t{a};
          }),
          eq_ft([](const vec_t *p)->bool {
            return p->empty() && p->size() == 0u && inner::good(*p);
          })
        });
      v.push_back({
          init_ft([=](vec_t *p) {
            new(p) vec_t{a};
            p->push_back(1);
          }),
          eq_ft([](const vec_t *p)->bool {
            return !p->empty() && p->size() == 1u && inner::good(*p)
              && *front(*p) == 1;
          })
        });
      v.push_back({
          init_ft([=](vec_t *p) {
            auto mk = t::maker_with_allocator(a);
            new(p) vec_t(mk(2,
                            mk(5), mk(6), mk(7)),
                         mk(3,
                            mk(8), mk(9), mk(10)),
                         mk(4,
                            mk(11), mk(12), mk(13)));
          }),
          eq_ft([](const vec_t *p)->bool {
            return !p->empty() && p->size() == 3u && inner::good(*p)
              && equal(deref_rng((*p)), seq(2, 3, 4))
              && equal(deref_rng(*nth(*p, 0)), seq(5, 6, 7))
              && all_of(*nth(*p, 0), [](const auto &x) {return x.empty();})
              && equal(deref_rng(*nth(*p, 1)), seq(8, 9, 10))
              && all_of(*nth(*p, 1), [](const auto &x) {return x.empty();})
              && equal(deref_rng(*nth(*p, 2)), seq(11, 12, 13))
              && all_of(*nth(*p, 2), [](const auto &x) {return x.empty();});
          })
        });
      v.push_back({
          init_ft([=](vec_t *p) {
            auto mk = t::maker_with_allocator(a);
            new(p) vec_t(mk(1,
                            mk(2),
                            mk(3,
                               mk(5),
                               mk(6,
                                  mk(9), mk(10), mk(11)),
                               mk(7)),
                            mk(4),
                            mk(5,
                               mk(8))));
          }),
          eq_ft([=](const vec_t *p)->bool {
            auto mk = t::maker_with_allocator(a);
            return !p->empty() && p->size() == 1u && inner::good(*p)
              && front(*p) == *mk(1,
                                  mk(2),
                                  mk(3,
                                     mk(5),
                                     mk(6,
                                        mk(9), mk(10), mk(11)),
                                     mk(7)),
                                  mk(4),
                                  mk(5,
                                     mk(8))).root();
          })
        });
    }
    for (auto &x : v)
      for (auto &y : v)
        test_allocator_aware_container_ownership<vec_t>
          (x.first, x.second, y.first, y.second, empt_f);
  }
}
void test_tree_vector_construct_assign() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using vec_t = linked_tree_vector<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;
  using vec_tt = linked_tree_vector<long, alloc_tt>;

  // vec_t(a)
  // vec_t(const vec_t &, a)
  // vec_t(vec_t &&, a)
  {
    alloc_t a1, a2;
    vec_t x(a1);
    assert(x.get_allocator() == a1
           && x.empty() && x.size() == 0u && inner::good(x));
    vec_t y(x, a2);
    assert(x.get_allocator() == a1
           && x.empty() && x.size() == 0u && inner::good(x));
    assert(y.get_allocator() == a2
           && y.empty() && y.size() == 0u && inner::good(y));

    const auto mk = t::maker_with_allocator(a1);
    vec_t z(vec_t(mk(2), mk(3, mk(4), mk(5)), mk(4)), a2);
    assert(z == vec_t(mk(2), mk(3, mk(4), mk(5)), mk(4))
           && inner::good(z)
           && z.get_allocator() == a2);

    vec_t zz(z, a1);
    assert(z == vec_t(mk(2), mk(3, mk(4), mk(5)), mk(4)) && inner::good(z)
           && z.get_allocator() == a2);
    assert(z == zz && inner::good(zz) && zz.get_allocator() == a1);

    vec_t u(move(z), a2);
    assert(z.empty() && inner::good(z) && z.get_allocator() == a2);
    assert(u == vec_t(mk(2), mk(3, mk(4), mk(5)), mk(4)) && inner::good(u)
           && u.get_allocator() == a2);

    vec_t uu(move(u), a1);
    assert(u.empty() && inner::good(u) && u.get_allocator() == a2);
    assert(uu == vec_t(mk(2), mk(3, mk(4), mk(5)), mk(4)) && inner::good(uu)
           && uu.get_allocator() == a1);
  }

  // vec_t(vec_ref, a)
  // =(vec_ref)
  {
    alloc_t a1, a2;
    const auto mk1 = t::maker_with_allocator(a1);
    const auto mk2 = t::maker_with_allocator(a2);
    vec_t x(allocator_arg, a1, mk1(1), mk1(2, mk1(3)));
    vec_tt y(x, a2);
    assert(inner::good(x) && inner::good(y));
    assert(x.get_allocator() == a1 && y.get_allocator() == a2);
    assert(x == vec_t(mk1(1), mk1(2, mk1(3))));
    assert(y == vec_t(mk1(1), mk1(2, mk1(3))));

    vec_t z(allocator_arg, a1, mk1(4));
    z = y;
    assert(y == vec_t(mk1(1), mk1(2, mk1(3))) && inner::good(y)
           && y.get_allocator() == a2);
    assert(z == y && inner::good(z) && z.get_allocator() == a1);

    vec_t yy(move(y), a2);
    assert(y.empty() && inner::good(y) && y.get_allocator() == a2);
    assert(yy == vec_t(mk1(1), mk1(2, mk1(3))) && inner::good(yy)
           && yy.get_allocator() == a2);

    vec_t zz(a1);
    zz = move(yy);
    assert(yy.empty() && inner::good(yy) && yy.get_allocator() == a2);
    assert(zz == vec_t(mk1(1), mk1(2, mk1(3))) && inner::good(zz)
           && zz.get_allocator() == a1);
  }

  // vec_t(n)
  // vec_t(n, a)
  // assign(n, k)
  {
    {
      vec_t v(3);
      assert(inner::good(v) && equal(deref_rng(v), rng(3, 0)));
    }
    {
      alloc_t a;
      vec_t v(3, a);
      assert(inner::good(v) && equal(deref_rng(v), rng(3, 0))
             && v.get_allocator() == a);

      v.assign(4, 1);
      assert(inner::good(v) && equal(deref_rng(v), rng(4, 1))
             && v.get_allocator() == a);
      v.assign(3, 2);
      assert(inner::good(v) && equal(deref_rng(v), rng(3, 2))
             && v.get_allocator() == a);
      v.assign(5, 3);
      assert(inner::good(v) && equal(deref_rng(v), rng(5, 3))
             && v.get_allocator() == a);
    }
  }

  // vec_t(from, to, a = {})
  // assign(from, to)
  {
    alloc_t a;
    auto q = seq(1, 2, 3);
    vec_t v(q.begin(), q.end(), a);
    vec_t vv(q.begin(), q.end() - 1);
    assert(inner::good(v) && inner::good(vv));
    assert(equal(deref_rng(v), q));
    assert(equal(deref_rng(vv), seq(1, 2)));
    assert(v.get_allocator() == a);
    vv.assign(deref_itr(v.begin()), deref_itr(v.end()));
    assert(inner::good(v) && inner::good(vv));
    assert(v == vv && equal(deref_rng(v), irng(1, 4)));
  }

  // vec_t(il, a = {})
  // =(il)
  // assign(il)
  {
    alloc_t a;
    vec_t v = {1, 2, 3};
    vec_t vv({4, 5}, a);
    assert(inner::good(v) && inner::good(vv));
    assert(all_of(v, [](auto &x) {return x.empty();}));
    assert(equal(deref_rng(v), irng(1, 4)));
    assert(all_of(vv, [](auto &x) {return x.empty();}));
    assert(equal(deref_rng(vv), seq(4, 5)));
    v.assign({6, 7, 8, 9});
    assert(inner::good(v) && all_of(v, [](auto &x) {return x.empty();})
           && equal(deref_rng(v), seq(6, 7, 8, 9)));
    v = {1, 2};
    assert(inner::good(v) && all_of(v, [](auto &x) {return x.empty();})
           && equal(deref_rng(v), seq(1, 2)));
  }

  // vec_t(r, a = {})
  // vec_t(from_range, r, a = {})
  // assign(r)
  // assign_range(r)
  {
    {
      vec_t v(seq(1, 2, 3));
      vec_t vv(from_range, seq(1, 2, 3));
      assert(inner::good(v) && equal(deref_rng(v), seq(1, 2, 3)));
      assert(inner::good(vv) && equal(deref_rng(vv), seq(1, 2, 3)));
      assert(v == vv);
    }
    {
      vec_t v(t(1, t(2)), t(3), t(4));
      v.assign(seq(1, 2));
      assert(inner::good(v) && v == vec_t(t(1), t(2)));
      v.assign_range(rng(3, 6));
      assert(inner::good(v) && v == vec_t(t(6), t(6), t(6)));
    }
  }

  // vec_t(trees...)
  // vec_t(allocator_arg, a, trees...)
  {
    vec_t v(t(1), t(2, t(3)), t(4, t(5), t(6)));
    assert(inner::good(v) && v == vec_t(t(1), t(2, t(3)), t(4, t(5), t(6))));
    alloc_t a;
    vec_t vv(allocator_arg, a,
             t(1), t(2, t(3)), t(4, t(5), t(6)));
    assert(inner::good(vv) && vv == vec_t(t(1), t(2, t(3)), t(4, t(5), t(6)))
           && vv.get_allocator() == a);
  }
}
void test_tree_vector_node_operations() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;
  using vec_t = linked_tree_vector<int, alloc_t>;
  using vec_tt = linked_tree_vector<long, alloc_tt>;

  {
    vec_t v(t(1, t(2), t(3)));
    vec_t vv = v.extract_children(v.begin());
    assert(v.get_allocator() == vv.get_allocator());
    assert(inner::good(v) && v == vec_t(t(1)));
    assert(vv == vec_t(t(2), t(3)));
    assert(inner::good(vv));

    t x = t(1, t(2), t(3));
    vec_t y = x.extract_children(x.root());
    assert(x.get_allocator() == y.get_allocator());
    assert(inner::good(x) && x == t(1));
    assert(inner::good(y) && y == vec_t({2, 3}));
  }

  // copy(it, get_key)->tree_type
  // copy(tree/vec_ref, it, get_key)->tree_type
  {
    vec_t v(t(1), t(2, t(3, t(4)), t(5)));
    assert(inner::good(v));
    t x = v.copy(next(v.begin()));
    assert(inner::good(x) && x == t(2, t(3, t(4)), t(5)));

    t y = v.copy(next(v.begin()), [](auto &x) {return exchange(*x, 0);});
    assert(inner::good(y) && y == t(2, t(3, t(4)), t(5)));
    assert(inner::good(v) && v == vec_t(t(1), t(0, t(0, t(0)), t(0))));
  }

  // clear()
  {
    vec_t v(t(1), t(2, t(3)));
    v.clear();
    assert(inner::good(v) && v.empty());
  }

  // swap(i, j)
  // swap(i, tree/vec_ref, j)
  {
    vec_t v = {1, 2, 3, 4};
    v.swap(v.begin(), before_end(v));
    assert(inner::good(v) && v == vec_t(t(4), t(2), t(3), t(1)));

    vec_t vv(t(1, t(2)), t(3, t(4)));
    vv.swap(vv.begin()->begin(), before_end(vv)->begin());
    assert(inner::good(vv) && vv == vec_t(t(1, t(4)), t(3, t(2))));

    t x = t(1, t(2), t(3));
    vec_t y(allocator_arg, x.get_allocator(), t(4));
    y.swap(y.begin(), x, x.root());
    assert(inner::good(x) && inner::good(y));
    assert(x == t(4) && y == vec_t(t(1, t(2), t(3))));
  }

  // extract(i)->tree_type
  {
    {
      vec_t v(t(1, t(2)), t(3));
      t x = v.extract(v.begin());
      assert(inner::good(v) && v == vec_t(t(3)));
      assert(inner::good(x) && x == t(1, t(2)));
    }
    {
      vec_t v(t(1, t(2)), t(3));
      t x = v.extract(v.begin()->begin());
      assert(inner::good(v) && v == vec_t(t(1), t(3)));
      assert(inner::good(x) && x == t(2));
    }
  }
  // insert(next, tree_type &&)->iterator
  {
    vec_t v(t(1, t(2), t(3)), t(4, t(5)));
    auto it = v.insert(next(v.begin()->begin())->end(),
                       t(t(0), v.get_allocator()));
    assert(**it == 0);
    assert(inner::good(v));
    assert(v == vec_t(t(1, t(2), t(3, t(0))), t(4, t(5))));
  }
  // exchange(i, tree_type &&)->tree_type
  {
    vec_t v(t(1, t(2)), t(3, t(4)));
    t x = v.exchange(v.begin(), t(t(0), v.get_allocator()));
    assert(inner::good(v) && v == vec_t(t(0), t(3, t(4))));
    assert(inner::good(x) && x == t(1, t(2)));
  }
  // replace(i, tree_type &&)->iterator
  {
    vec_t v(t(1), t(2), t(3, t(4), t(5)));
    const auto it
      = v.replace(before_end(v)->begin(), t(t(0), v.get_allocator()));
    assert(inner::good(v) && v == vec_t(t(1), t(2), t(3, t(0), t(5))));
    assert(**it == 0);
  }

  // extract(i1, i2)->vector_type
  {
    vec_t v = {1, 2, 3, 4};
    vec_t vv = v.extract(nth(v, 1), nth(v, 3));
    assert(inner::good(v) && v == vec_t(t(1), t(4)));
    assert(inner::good(vv) && vv == vec_t(t(2), t(3)));

    t x = t(1, t(2), t(3));
    vec_t y = x.extract(x.root()->begin(), x.root()->end());
    assert(inner::good(x) && x == t(1));
    assert(inner::good(y) && y == vec_t(t(2), t(3)));
    assert(x.get_allocator() == y.get_allocator());
  }
  // insert(i, vector_type &&)->iterator
  {
    vec_t v = {1, 2, 3, 4};
    vec_t vv({5, 6}, v.get_allocator());
    assert(**v.insert(prev(v.end()), move(vv)) == 5);
    assert(inner::good(v) && v == vec_t({1, 2, 3, 5, 6, 4}));
    assert(inner::good(vv) && vv.empty());

    t x = t(1, t(2));
    vec_t y(allocator_arg, x.get_allocator(), t(3), t(4));
    auto it = x.insert(x.root()->begin()->end(), move(y));
    assert(**it == 3);
    assert(inner::good(x) && x == t(1, t(2, t(3), t(4))));
    assert(inner::good(y) && y.empty());
  }
  // exchange(i1, i2, vector_type &&)->vector_type
  {
    vec_t v = {1, 2, 3, 4};
    vec_t vv(allocator_arg, v.get_allocator(), t(5, t(6), t(7)));
    vec_t vvv = v.exchange(nth(v, 1), nth(v, 3), move(vv));
    assert(inner::good(v) && v == vec_t(t(1), t(5, t(6), t(7)), t(4)));
    assert(inner::good(vv) && vv.empty());
    assert(inner::good(vvv) && vvv == vec_t({2, 3}));
    assert(v.get_allocator() == vv.get_allocator());
    assert(v.get_allocator() == vvv.get_allocator());

    t x = t(1, t(2), t(3), t(4));
    vec_t y({0, 0}, x.get_allocator());
    vec_t z = x.exchange(next(x.root()->begin()), next(x.root()->begin(), 2),
                         move(y));
    assert(inner::good(z) && z == vec_t({3}));
    assert(inner::good(y) && y.empty());
    assert(inner::good(x) && x == t(1, t(2), t(0), t(0), t(4)));
  }
  // replace(i1, i2, vector_type &&)
  {
    vec_t v = {1, 2, 3, 4};
    vec_t vv(allocator_arg, v.get_allocator(), t(5, t(6), t(7)));
    v.replace(nth(v, 1), nth(v, 3), move(vv));
    assert(inner::good(v) && v == vec_t(t(1), t(5, t(6), t(7)), t(4)));
    assert(inner::good(vv) && vv.empty());
    assert(v.get_allocator() == vv.get_allocator());

    t x = t(1, t(2), t(3), t(4));
    vec_t y({0, 0}, x.get_allocator());
    const auto it = x.replace(next(x.root()->begin()),
                              next(x.root()->begin(), 2),
                              move(y));
    assert(inner::good(y) && y.empty());
    assert(inner::good(x) && x == t(1, t(2), t(0), t(0), t(4)));
    assert(**it == 0);
  }

  // extract_children(i)->vector_type
  {
    vec_t v(t(1, t(2), t(3)));
    vec_t vv = v.extract_children(v.begin());
    assert(v.get_allocator() == vv.get_allocator());
    assert(inner::good(v) && v == vec_t(t(1)));
    assert(inner::good(vv) && vv == vec_t(t(2), t(3)));

    t x = t(1, t(2), t(3));
    vec_t y = x.extract_children(x.root());
    assert(x.get_allocator() == y.get_allocator());
    assert(inner::good(x) && x == t(1));
    assert(inner::good(y) && y == vec_t({2, 3}));
  }
  // exchange_children(i, vector_type &&)->vector_type
  {
    vec_t v(t(1, t(2), t(3)));
    vec_t vv(allocator_arg, v.get_allocator(), t(4));
    vec_t vvv = v.exchange_children(v.begin(), move(vv));
    assert(inner::good(v) && v == vec_t(t(1, t(4))));
    assert(inner::good(vv) && vv.empty());
    assert(inner::good(vvv) && vvv == vec_t({2, 3}));
    assert(vv.get_allocator() == v.get_allocator());
    assert(vvv.get_allocator() == v.get_allocator());

    t x = t(1, t(2));
    vec_t y(allocator_arg, x.get_allocator());
    vec_t z = x.exchange_children(x.root(), move(y));
    assert(inner::good(y) && y.empty());
    assert(inner::good(x) && x == t(1));
    assert(inner::good(z) && z == vec_t({2}));
    assert(y.get_allocator() == x.get_allocator());
    assert(z.get_allocator() == x.get_allocator());
  }
  // replace_children(i, vector_type &&)->iterator
  {
    vec_t v(t(1, t(2), t(3)));
    vec_t vv(allocator_arg, v.get_allocator(), t(4), t(5), t(6));
    const auto it = v.replace_children(v.begin(), move(vv));
    assert(inner::good(v) && v == vec_t(t(1, t(4), t(5), t(6))));
    assert(inner::good(vv) && vv.empty());
    assert(vv.get_allocator() == v.get_allocator());
  }

  // splice(next, tree/vector_ref, i)
  {
    vec_t v(t(1), t(2), t(3, t(4)));
    v.splice(v.end(), v, before_end(v)->begin());
    assert(inner::good(v) && equal(v, vec_t({1, 2, 3, 4})));
    t x(t(5, t(5)), v.get_allocator());
    v.splice(v.end(), x, x.root()->begin());
    assert(inner::good(v) && equal(v, vec_t({1, 2, 3, 4, 5})));
    assert(inner::good(x) && x == t(5));

    t y = t(1, t(2), t(3));
    vec_t z({4, 5}, y.get_allocator());
    y.splice(before_end(*y.root()), z, z.begin());
    assert(inner::good(y) && y == t(1, t(2), t(4), t(3)));
    assert(inner::good(z) && z == vec_t({5}));
    y.splice(y.root()->begin(), y, prev(y.root()->end()));
    assert(inner::good(y) && y == t(1, t(3), t(2), t(4)));
  }
  // splice(next, tree/vector_ref, i, i2)
  {
    vec_t v = {1, 2, 3, 4, 5};
    v.splice(v.begin(), v, prev(v.end()), v.end());
    assert(v == vec_t({5, 1, 2, 3, 4}));
    t u(t(1, t(2)), v.get_allocator());
    v.splice(v.begin(), u, u.root()->begin(), u.root()->end());
    assert(inner::good(u) && u == t(1));
    assert(inner::good(v)
           && v == vec_t(t(2), t(5), t(1), t(2), t(3), t(4)));
    assert(v.get_allocator() == u.get_allocator());

    t x = t(1, t(2), t(3));
    vec_t y({4, 5}, x.get_allocator());
    x.splice(x.root()->begin(), y, y.begin(), y.end());
    assert(inner::good(x) && x == t(1, t(4), t(5), t(2), t(3)));
    assert(inner::good(y) && y.empty());
    assert(x.get_allocator() == y.get_allocator());
  }
  // splice(next, vector_ref)
  {
    vec_t v({1, 2, 3});
    vec_t vv({4, 5}, v.get_allocator());
    v.splice(v.begin()->end(), vv);
    assert(inner::good(v) && v == vec_t(t(1, t(4), t(5)), t(2), t(3)));
    assert(inner::good(vv) && vv.empty());
    assert(v.get_allocator() == vv.get_allocator());
  }

  // emplace(i, s...)->iterator
  // insert(i, k)->iterator
  // insert(i, n, k)->iterator
  // insert(i, from, to)->iterator
  // insert(i, l)->iterator
  // insert(i, r)->iterator
  // insert_range(i, r)->iterator
  {
    vec_t v = {1, 2, 3};
    auto it = v.emplace(v.begin(), 0);
    assert(it == v.begin());
    assert(inner::good(v) && equal(deref_rng(v), irng(0, 4)));

    it = v.insert(v.begin()->end(), 4);
    assert(inner::good(v) && equal(v, vec_t(t(0, t(4)), t(1), t(2), t(3))));
    assert(**it == 4);
    it = v.insert(v.begin()->end(), 2, 5);
    assert(inner::good(v)
           && equal(v, vec_t(t(0, t(4), t(5), t(5)), t(1), t(2), t(3))));
    assert(**it == 5);

    v = {1, 2, 3};
    it = v.insert(v.end(), {4, 5});
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5}));
    assert(**it == 4);
    it = v.insert(v.end(), seq(6));
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5, 6}));
    assert(**it == 6);
    it = v.insert_range(v.end(), seq(7, 8));
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5, 6, 7, 8}));
    assert(**it == 7);
  }

  // erase(i)->iterator
  {
    vec_t v = {1, 2, 3};
    auto it = v.erase(v.begin());
    assert(inner::good(v) && it == v.begin() && v == vec_t({2, 3}));
  }
  // erase(i1, i2)->iterator
  {
    vec_t v(t(1), t(2, t(3), t(4)), t(5));
    auto it = v.erase(next(v.begin())->begin(), next(v.begin())->end());
    assert(it == next(v.begin())->end());
    assert(inner::good(v) && v == vec_t(t(1), t(2), t(5)));
  }
  // clear(i)
  {
    vec_t v = {1, 2, 3};
    v.clear(vec_t::iterator{});
    assert(inner::good(v) && v.empty());
    v = {1, 2, 3};
    v.clear(v.begin());
    assert(inner::good(v) && v == vec_t({1, 2, 3}));
    v = vec_t(t(1), t(2, t(3)));
    v.clear(next(v.begin()));
    assert(inner::good(v) && v == vec_t({1, 2}));
  }
}
void test_tree_vector_insert_erase() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;
  using vec_t = linked_tree_vector<int, alloc_t>;
  using vec_tt = linked_tree_vector<long, alloc_tt>;

  // emplace_back(s...)->iterator
  // push_back(k)
  // push_back(tree_ref)
  // push_back(vector_ref)
  // push_back(kr)
  // append_range(r)
  // append(s...)
  // pop_back()
  // pop_back(n)
  {
    vec_t v = {1, 2, 3};
    assert(v.emplace_back(4).key() == 4);
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4}));
    v.push_back(5);
    v.push_back(t(t(6, t(7)), v.get_allocator()));
    v.push_back(vec_t({0}, v.get_allocator()));
    v.push_back(seq(9, 10));
    assert(inner::good(v)
           && v == vec_t(t(1), t(2), t(3), t(4), t(5), t(6, t(7)),
                         t(0), t(9), t(10)));
    v.clear();
    v.append_range(seq(1, 2));
    v.append(3, t(t(4), v.get_allocator()), vec_t({5}, v.get_allocator()))
      .append(seq(6));
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5, 6}));

    v.pop_back(5);
    assert(inner::good(v) && v == vec_t({1}));
    v.pop_back();
    assert(inner::good(v) && v.empty());
  }
  // emplace_back(i, s...)
  // push_back(i, k)
  // push_back(i, tree_ref)
  // push_back(i, vector_ref)
  // push_back(i, kr)
  // append_range(i, r)
  // append(i, s...)
  // pop_back(i)
  // pop_back(i, n)
  {
    vec_t v(t(0, t(1), t(2)));
    assert(*v.emplace_back(v.begin(), 3) == 3);
    assert(inner::good(v) && v == vec_t(t(0, t(1), t(2), t(3))));
    v.push_back(v.begin(), 4);
    v.push_back(v.begin(), t(t(5, t(6)), v.get_allocator()));
    v.push_back(v.begin(), vec_t({7}, v.get_allocator()));
    v.push_back(v.begin(), seq(8, 9));
    assert(inner::good(v)
           && v == vec_t(t(0, t(1), t(2), t(3), t(4), t(5, t(6)),
                           t(7), t(8), t(9))));
    v.clear();
    v.append_range(vec_t::iterator{}, seq(1, 2));
    v.append(vec_t::iterator{},
             3, t(t(4), v.get_allocator()), vec_t({5}, v.get_allocator()),
             seq(6));
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5, 6}));
    v.pop_back(vec_t::iterator{}, 5);
    v.pop_back(vec_t::iterator{});
    assert(inner::good(v) && v.empty());
  }

  // emplace_front(s...)
  // push_front(k)
  // push_front(tree_ref)
  // push_front(vector_ref)
  // push_front(kr)
  // prepend_range(r)
  // prepend(s...)
  // pop_front()
  // pop_front(n)
  {
    vec_t v = {1, 2, 3};
    assert(*v.emplace_front(0) == 0);
    assert(inner::good(v) && v == vec_t({0, 1, 2, 3}));
    v.push_front(5);
    v.push_front(t(t(6, t(7)), v.get_allocator()));
    v.push_front(vec_t({0}, v.get_allocator()));
    v.push_front(seq(9, 10));
    assert(inner::good(v)
           && v == vec_t(t(9), t(10), t(0), t(6, t(7)), t(5),
                         t(0), t(1), t(2), t(3)));
    v.clear();
    v.prepend_range(seq(1, 2));
    v.prepend(3, t(t(4), v.get_allocator()),
              vec_t({5}, v.get_allocator()))
      .prepend(seq(6));
    assert(inner::good(v) && v == vec_t({6, 3, 4, 5, 1, 2}));

    v.pop_front(5);
    assert(inner::good(v) && v == vec_t({2}));
    v.pop_front();
    assert(inner::good(v) && v.empty());
  }
  // emplace_front(i, s...)
  // push_front(i, k)
  // push_front(i, tree_ref)
  // push_front(i, vector_ref)
  // push_front(i, kr)
  // prepend_range(i, r)
  // prepend(i, s...)
  // pop_front(i)
  // pop_front(i, n)
  {
    vec_t v(t(0, t(1), t(2)));
    assert(*v.emplace_front(v.begin(), 3) == 3);
    assert(inner::good(v) && v == vec_t(t(0, t(3), t(1), t(2))));
    v.push_front(v.begin(), 4);
    v.push_front(v.begin(), t(t(5, t(6)), v.get_allocator()));
    v.push_front(v.begin(), vec_t({7}, v.get_allocator()));
    v.push_front(v.begin(), seq(8, 9));
    assert(inner::good(v)
           && v == vec_t(t(0, t(8), t(9), t(7), t(5, t(6)),
                           t(4), t(3), t(1), t(2))));
    v.clear();
    v.prepend_range(vec_t::iterator{}, seq(1, 2));
    v.prepend(vec_t::iterator{},
              3, t(t(4), v.get_allocator()), vec_t({5}, v.get_allocator()),
              seq(6));
    assert(inner::good(v) && v == vec_t({3, 4, 5, 6, 1, 2}));
    v.pop_front(vec_t::iterator{}, 5);
    v.pop_front(vec_t::iterator{});
    assert(inner::good(v) && v.empty());
  }
}
void test_tree_vector_mscl_for_sequence_container() {
  using alloc_t = stateful_test_allocator<int, ez_map, false, false, false>;
  using alloc_tt = stateful_test_allocator<long, ez_map, false, false, false>;
  using t = linked_tree<int, alloc_t>;
  using tt = linked_tree<long, alloc_tt>;
  using vec_t = linked_tree_vector<int, alloc_t>;
  using vec_tt = linked_tree_vector<long, alloc_tt>;

  // resize(n, x = ...)
  // resize(i, n, x = ...)
  // first_order_resize(i, n, x = ...)
  {
    vec_t v;
    v.resize(2);
    assert(v == vec_t({0, 0}));
    v.resize(4, 1);
    assert(v == vec_t({0, 0, 1, 1}));
    v.resize(1, 1);
    assert(v == vec_t({0}));

    vec_t vv(t(1, t(2), t(3)));
    vv.first_order_resize(4);
    assert(vv == vec_t(t(1, t(2), t(3), t(0), t(0)), t(0), t(0), t(0)));
  }

  // remove_if(eq)
  // remove_if(i, eq)
  // first_order_remove_if(eq)
  // first_order_remove_if(i, eq)
  {
    const auto eq = [](auto &x) {return *x == 0;};
    {
      vec_t v(t(1), t(0), t(2, t(0, t(3)), t(4), t(0)), t(3, t(4)));
      assert(v.remove_if(eq) == 1u);
      assert(inner::good(v)
             && v == vec_t(t(1), t(2, t(0, t(3)), t(4), t(0)), t(3, t(4))));
      assert(v.remove_if(next(v.begin()), eq) == 2u);
      assert(inner::good(v)
             && v == vec_t(t(1), t(2, t(4)), t(3, t(4))));
    }
    {
      vec_t v(t(1), t(0), t(2, t(0, t(3)), t(4), t(0)), t(3, t(4)));
      v.first_order_remove_if(eq);
      assert(inner::good(v) && v == vec_t(t(1), t(2, t(4)), t(3, t(4))));
    }
  }

  // unique(eq)
  // unique(i, eq)
  // first_order_unique(eq)
  // first_order_unique(i, eq)
  {
    vec_t v = {1, 2, 2, 3, 3, 3, 4};
    const auto eqf = [](auto &x, auto &y) {return *x == *y;};
    v.unique(eqf);
    assert(inner::good(v) && v == vec_t({1, 2, 3, 4}));
    v = vec_t(t(0, t(1), t(2), t(2), t(3)));
    v.unique(v.begin(), eqf);
    assert(inner::good(v) && v == vec_t(t(0, t(1), t(2), t(3))));

    v = vec_t(t(1), t(2, t(4), t(4), t(5)), t(2, t(6)), t(3));
    v.first_order_unique(eqf);
    assert(inner::good(v) && v == vec_t(t(1), t(2, t(4), t(5)), t(3)));
  }

  // merge(tree/vec_ref, i2, less)
  // merge(i, tree/vec_ref, i2, less)
  // merge(vec_ref, less)
  // merge(i, vec_ref, less)
  {
    const auto compf = [](auto &x, auto &y) {return *x < *y;};
    {
      vec_t v = {1, 3, 5};
      t x(t(0, t(2), t(4)), v.get_allocator());
      v.merge(x, x.root(), compf);
      assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5}));
      assert(inner::good(x) && x == t(0));

      x.merge(x.root(), v, vec_t::const_iterator{}, compf);
      assert(inner::good(v) && v == vec_t{});
      assert(inner::good(x) && x == t(0, t(1), t(2), t(3), t(4), t(5)));
    }
    {
      vec_t v = {1, 3, 5};
      vec_t vv(allocator_arg, v.get_allocator(), t(0, t(2), t(4)));
      v.merge(vv, vv.begin(), compf);
      assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5}));
      assert(inner::good(vv) && vv == vec_t(t(0)));
    }
    {
      vec_t v = {1, 3, 5};
      vec_t vv({2, 4, 6}, v.get_allocator());
      v.merge(vv, compf);
      assert(inner::good(v) && v == vec_t({1, 2, 3, 4, 5, 6}));
      assert(inner::good(vv) && vv.empty());
    }
  }

  // sort(less)
  // sort(i, less)
  // first_order_sort(less)
  // first_order_sort(i, less)
  {
    const auto compf = [](auto &x, auto &y) {return *x < *y;};
    {
      vec_t v = {2, 1, 3};
      v.sort(compf);
      assert(inner::good(v) && v == vec_t({1, 2, 3}));
    }
    {
      vec_t v(t(2), t(1, t(0, t(2), t(1), t(3))), t(3));
      v.first_order_sort(compf);
      assert(inner::good(v)
             && v == vec_t(t(1, t(0, t(1), t(2), t(3))), t(2), t(3)));
    }
  }
}

void test_linked_tree() {
  printf("linked_tree: ");

  inner::fns::test_tree_base();
  inner::fns::test_tree_iterator();
  inner::fns::test_tree_special_member_functions();
  inner::fns::test_tree_construct_assign();
  inner::fns::test_tree_node_operations();
  inner::fns::test_tree_insert_erase();
  inner::fns::test_tree_back_insert_erase();
  inner::fns::test_tree_front_insert_erase();
  inner::fns::test_tree_first_last_nth_order();
  inner::fns::test_tree_mscl_for_sequence_container();

  inner::fns::test_tree_vector_base();
  inner::fns::test_tree_vector_special_member_functions();
  inner::fns::test_tree_vector_construct_assign();
  inner::fns::test_tree_vector_node_operations();
  inner::fns::test_tree_vector_insert_erase();
  inner::fns::test_tree_vector_mscl_for_sequence_container();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_linked_tree();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
