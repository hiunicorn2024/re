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

void test_forward_list_ownership() {
  const auto r = irng(0, 10);
  for (int n1 : r)
    for (int n2 : r) {
      const auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        const auto init1 = [=](vec_t *p) {::new(p) vec_t(irng(0, n1));};
        const auto good1 = [=](vec_t *p) {return equal(*p, irng(0, n1));};
        const auto init2 = [=](vec_t *p) {::new(p) vec_t(irng(1, n2 + 1));};
        const auto good2 = [=](vec_t *p) {return equal(*p, irng(1, n2 + 1));};
        const auto check_empty = [](vec_t *p) {return p->empty();};
        test_allocator_aware_container_ownership<vec_t>
          (init1, good1, init2, good2, check_empty);
      };
      testf(type_tag<forward_list<int>>());
      testf(type_tag<forward_list<int, test_allocator<int>>>());
      testf(type_tag<forward_list<int, stateful_test_allocator<int>>>());
      testf(type_tag<forward_list<test_object<int>>>());
      testf(type_tag<slist<int>>());
      testf(type_tag<slist<int, test_allocator<int>>>());
      testf(type_tag<slist<int, stateful_test_allocator<int>>>());
      testf(type_tag<slist<test_object<int>>>());

      using P00 = type_pack<true_type, false_type>;
      using P0 = type_pack_mul<P00, P00>;
      using P = type_pack_mul<P0, P0>;
      type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
          constexpr bool y1 = decltype(a)::type::value;
          constexpr bool y2 = decltype(b)::type::value;
          constexpr bool y3 = decltype(c)::type::value;
          constexpr bool y4 = decltype(d)::type::value;

          {
            using vec_t = forward_list
              <int, stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
            using alloc_t
              = stateful_test_allocator<int, ez_map, y1, y2, y3, y4>;
            const auto init1 = [=](vec_t *p, const alloc_t &al) {
              ::new(p) vec_t(irng(0, n1), al);
              assert(p->get_allocator() == al);
            };
            const auto good1 = [=](vec_t *p) {
              return equal(*p, irng(0, n1));
            };
            const auto init2 = [=](vec_t *p, const alloc_t &al) {
              ::new(p) vec_t(irng(1, n2 + 1), al);
              assert(p->get_allocator() == al);
            };
            const auto good2 = [=](vec_t *p) {
              return equal(*p, irng(1, n2 + 1));
            };
            const auto check_empty = [](vec_t *p) {return p->empty();};

            const alloc_t a1, a2;

            test_allocator_aware_container_ownership<vec_t>
              (bind(init1, _1, a1), good1, bind(init2, _1, a1), good2,
               check_empty);
            test_allocator_aware_container_ownership<vec_t>
              (bind(init1, _1, a1), good1, bind(init2, _1, a2), good2,
               check_empty);
          }
          {
            using vec_t = slist
              <int, stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
            using alloc_t
              = stateful_test_allocator<int, ez_map, y1, y2, y3, y4>;
            const auto init1 = [=](vec_t *p, const alloc_t &al) {
              ::new(p) vec_t(irng(0, n1), al);
              assert(p->get_allocator() == al);
            };
            const auto good1 = [=](vec_t *p) {
              return equal(*p, irng(0, n1));
            };
            const auto init2 = [=](vec_t *p, const alloc_t &al) {
              ::new(p) vec_t(irng(1, n2 + 1), al);
              assert(p->get_allocator() == al);
            };
            const auto good2 = [=](vec_t *p) {
              return equal(*p, irng(1, n2 + 1));
            };
            const auto check_empty = [](vec_t *p) {return p->empty();};

            const alloc_t a1, a2;

            test_allocator_aware_container_ownership<vec_t>
              (bind(init1, _1, a1), good1, bind(init2, _1, a1), good2,
               check_empty);
            test_allocator_aware_container_ownership<vec_t>
              (bind(init1, _1, a1), good1, bind(init2, _1, a2), good2,
               check_empty);
          }
        });
    }
};
struct myt2 : join_forward_list<> {
  int i;

  myt2() = default;
  ~myt2() = default;
  myt2(const myt2 &) = default;
  myt2 &operator =(const myt2 &) = default;
  myt2(myt2 &&) = default;
  myt2 &operator =(myt2 &&) = default;
  void swap(myt2 &x) noexcept {
    adl_swap(i, x.i);
    adl_swap((join_list<> &)*this, (join_list<> &)x);
  }

  myt2(int i) : i(i) {}

  bool operator ==(const myt2 &x) const {
    return i == x.i;
  }
  bool operator !=(const myt2 &x) const {
    return i != x.i;
  }
  bool operator <(const myt2 &x) const {
    return i < x.i;
  }

  bool operator ==(int j) const {
    return i == j;
  }
  bool operator !=(int j) const {
    return i != j;
  }
};
void swap(myt2 &x, myt2 &y) noexcept {
  x.swap(y);
}
void test_forward_list_intrusive_mode() {
  using L = forward_list_adaptor<forward_list_traits<myt2>>;
  static_assert(is_default_constructible_v<L>);
  static_assert(!is_copy_constructible_v<L>);
  static_assert(!is_copy_assignable_v<L>);
  static_assert(is_nothrow_move_constructible_v<L>);
  static_assert(is_nothrow_move_assignable_v<L>);
  static_assert(is_nothrow_swappable_v<L>);

  {
    vector<myt2> v = {1, 2, 3, 4, 5};
    L l;
    for (auto &x : v)
      l.link_after(l.before_begin(), rng_itr<L>(&x));
    assert(l.front() == 5 && equal(l, rrng(irng(1, 6))));
    l.unlink();
    assert(l.empty());
    assert(*l.link_front(rng_itr<L>(&v[0])) == 1);
    assert(*l.link_after(l.begin(), rng_itr<L>(&v[1])) == 2);
    l.link_after(++l.begin(), rng_itr<L>(&v[2]));
    assert(equal(l, seq(1, 2, 3)));
    auto it = l.unlink_after(++l.begin());
    assert(it == l.end());
    assert(equal(l, seq(1, 2)));
    it = l.unlink_after(l.before_begin(), l.end());
    assert(it == l.end() && l.empty());
    for (auto &x : v)
      l.link_after(l.before_begin(), rng_itr<L>(&x));
    L ll = move(l);
    assert(l.empty() && equal(ll, rrng(irng(1, 6))));
    swap(l, ll);
    assert(ll.empty() && equal(l, rrng(irng(1, 6))));
    ll = move(l);
    assert(l.empty() && equal(ll, rrng(irng(1, 6))));
  }
  {
    vector<myt2> v = {1, 2, 3, 4};
    vector<myt2> vv = {5, 6, 7, 8};
    L l, ll;
    for (auto x : bind_rng(rrng(v), [](auto &x) {return &x;}))
      l.link_front(rng_itr<L>(x));
    for (auto x : bind_rng(rrng(vv), [](auto &x) {return &x;}))
      ll.link_front(rng_itr<L>(x));
    assert(equal(l, seq(1, 2, 3, 4)));
    assert(equal(ll, seq(5, 6, 7, 8)));
    l.splice_after(++++++l.begin(), ll);
    assert(ll.empty() && equal(l, irng(1, 9)));
    ll.splice_after(ll.before_begin(), l,
                    next(l.before_begin(), 4),
                    next(l.before_begin(), 9));
    assert(equal(l, seq(1, 2, 3, 4)));
    assert(equal(ll, seq(5, 6, 7, 8)));
    l.splice_after(l.before_begin(), l, l.before_begin());
    assert(equal(l, seq(1, 2, 3, 4)));
    l.splice_after(l.begin(), l, l.begin());
    assert(equal(l, seq(1, 2, 3, 4)));
    l.splice_after(l.begin(), l, l.before_begin());
    assert(equal(l, seq(1, 2, 3, 4)));
    l.splice_after(++l.begin(), l, l.begin());
    assert(equal(l, seq(1, 2, 3, 4)));
  }
  {
    vector<myt2> v = {1, 2, 3, 3, 4, 5};
    L l;
    copy(bind_rng(v, [](myt2 &x) {return rng_itr<L>(addressof(x));}),
         output_itr([&l, i = l.before_begin()](auto x) mutable {
             i = l.link_after(i, x);
           }));
    assert(equal(l, seq(1, 2, 3, 3, 4, 5)));
    l.remove_if([](auto &x) {return x == 3;});
    assert(equal(l, seq(1, 2, 4, 5)));
    l.unlink();
    copy(bind_rng(v, [](myt2 &x) {return rng_itr<L>(addressof(x));}),
         output_itr([&l, i = l.before_begin()](auto x) mutable {
             i = l.link_after(i, x);
           }));
    l.remove(1);
    l.remove(3);
    assert(equal(l, seq(2, 4, 5)));
    l.remove(5);
    assert(equal(l, seq(2, 4)));
  }
  {
    auto a1 = seq((myt2)1, 1, 2, 2, 3, 3);
    L l;
    copy(a1, output_itr([&l, i = l.before_begin()](auto &x) mutable {
        i = l.link_after(i, rng_itr<L>(addressof(x)));
      }));
    assert(equal(l, seq(1, 1, 2, 2, 3, 3)));
    l.unique();
    assert(equal(l, seq(1, 2, 3)));
    auto a2 = seq((myt2)1, 2, 3, 3, 3);
    l.unlink();
    copy(a2, output_itr([&l, i = l.before_begin()](auto &x) mutable {
          i = l.link_after(i, rng_itr<L>(addressof(x)));
        }));
    assert(equal(l, seq(1, 2, 3, 3, 3)));
    l.unique(equal_to<>());
    assert(equal(l, seq(1, 2, 3)));
  }
  {
    vector<myt2> v1 = {1, 3, 5};
    vector<myt2> v2 = {2, 4, 6, 8};
    L l1, l2;
    copy(v1, output_itr([&l1, i = l1.before_begin()](auto &x) mutable {
          i = l1.link_after(i, rng_itr<L>(addressof(x)));
        }));
    copy(v2, output_itr([&l2, i = l2.before_begin()](auto &x) mutable {
          i = l2.link_after(i, rng_itr<L>(addressof(x)));
        }));
    assert(equal(l1, seq(1, 3, 5)));
    assert(equal(l2, seq(2, 4, 6, 8)));
    l1.merge(l1);
    assert(equal(l1, seq(1, 3, 5)));
    assert(equal(l2, seq(2, 4, 6, 8)));
    l1.merge(l2);
    assert(l2.empty() && equal(l1, seq(1, 2, 3, 4, 5, 6, 8)));
    l1.unlink();
    copy(v1, output_itr([&l1, i = l1.before_begin()](auto &x) mutable {
          i = l1.link_after(i, rng_itr<L>(addressof(x)));
        }));
    copy(v2, output_itr([&l2, i = l2.before_begin()](auto &x) mutable {
          i = l2.link_after(i, rng_itr<L>(addressof(x)));
        }));
    assert(equal(l1, seq(1, 3, 5)));
    assert(equal(l2, seq(2, 4, 6, 8)));
    l2.merge(l1);
    assert(l1.empty() && equal(l2, seq(1, 2, 3, 4, 5, 6, 8)));
  }
  {
    auto a = seq((myt2)1, 2, 3);
    L l;
    assert(l.empty());
    l.reverse();
    assert(l.empty());
    for (auto &x : rrng(a))
      l.link_front(rng_itr<L>(addressof(x)));
    assert(equal(l, seq(1, 2, 3)));
    l.reverse();
    assert(equal(l, seq(3, 2, 1)));
    l.unlink();
    l.link_front(rng_itr<L>(addressof(a[0])));
    assert(equal(l, seq(1)));
    l.reverse();
    assert(equal(l, seq(1)));
  }
  {
    vector<myt2> v(irng(0, 1001));
    L l;
    shuffle(v, minstd_rand{});
    for (auto &x : v)
      l.link_front(rng_itr<L>(addressof(x)));
    l.sort();
    assert(is_sorted(l));
  }
}
template <class L>
void test_forward_list_briefly() {
  {
    L l;
    auto n1 = l.new_node(1);
    auto n2 = l.new_node(2);
    auto n3 = l.new_node(3);
    assert(*l.link_front(n1) == 1);
    assert(*l.link_after(l.begin(), n2) == 2);
    assert(*l.link_after(++l.begin(), n3) == 3);
    assert(equal(l, seq(1, 2, 3)));
    assert(*l.unlink_after(l.before_begin()) == 2);
    assert(equal(l, seq(2, 3)));
    assert(l.unlink_after(l.begin()) == l.end());
    assert(equal(l, seq(2)));
    l.unlink();
    assert(l.empty());
    l.delete_node(n1);
    l.delete_node(n2);
    l.delete_node(n3);
  }

  L l, ll;

  // container

  static_assert(forward_iterator<typename L::iterator>);
  static_assert(forward_iterator<typename L::const_iterator>);
  static_assert(is_convertible_v
                <const typename L::iterator &, typename L::const_iterator>);
  static_assert(!is_convertible_v
                <const typename L::const_iterator &, typename L::iterator>);
  assert(l.begin() == l.end());
  assert(l.cbegin() == l.end());
  assert(l.begin() == l.cend());
  assert(l.cbegin() == l.cend());
  assert(++l.before_begin() == l.begin());
  assert(++l.cbefore_begin() == l.begin());
  assert(++l.before_begin() == l.cbegin());
  assert(++l.cbefore_begin() == l.cbegin());
  assert(l.empty());
  assert(size(l) == 0);
  assert(size(l) < l.max_size());

  // optional container operations

  assert(l == l);
  assert(!(l != l));
  assert(l <= l);
  assert(l >= l);
  assert(!(l < ll));
  assert(!(l > ll));

  // allocator-aware container

  typename L::allocator_type al;
  l = L(al);
  assert(l.empty() && l.get_allocator() == al);
  ll = {1, 2, 3};
  l = L(ll, al);
  assert(equal(l, seq(1, 2, 3)) && l.get_allocator() == al);
  l = L(move(ll), al);
  assert(equal(l, seq(1, 2, 3)) && l.get_allocator() == al);
  assert(ll.empty());

  // sequence container

  l = L(3);
  assert(size(l) == 3);
  l = L(3, l.get_allocator());
  assert(size(l) == 3);
  l = L(4, 4);
  assert(size(l) == 4 && all_of_equal(l, 4));
  l = L(4, 5, l.get_allocator());
  assert(size(l) == 4 && all_of_equal(l, 5));

  ll = {1, 2, 3};
  l = L(ll.begin(), ll.end());
  assert(l == ll);
  l = L();
  l = L(ll.begin(), ll.end(), ll.get_allocator());
  assert(l == ll);
  ll = {2, 3, 4};
  l.assign(++ll.begin(), ll.end());
  assert(equal(l, seq(3, 4)));

  l = L({1, 2, 3, 4});
  assert(equal(l, seq(1, 2, 3, 4)));
  l = L({1, 2, 3}, l.get_allocator());
  assert(equal(l, seq(1, 2, 3)));
  l = {3, 3, 3};
  assert(equal(l, seq(3, 3, 3)));
  l.assign({3, 2, 1});
  assert(equal(l, seq(3, 2, 1)));

  l = L();
  auto it = l.emplace_after(l.before_begin(), 1);
  assert(it == l.begin() && equal(l, seq(1)));
  it = l.insert_after(l.before_begin(), 2);
  assert(it == l.begin() && equal(l, seq(2, 1)));
  int tmp = 3;
  it = l.insert_after(l.begin(), tmp);
  assert(it == ++l.begin() && equal(l, seq(2, 3, 1)));
  it = l.insert_after(l.begin(), 2, 9);
  assert(it == next(l.begin(), 2) && equal(l, seq(2, 9, 9, 3, 1)));
  const auto r = irng(10, 13);
  it = l.insert_after(next(l.begin(), 3), r.begin(), r.end());
  assert(it == next(l.begin(), 6));
  assert(equal(l, seq(2, 9, 9, 3, 10, 11, 12, 1)));
  it = l.insert_after(l.before_begin(), {0, 1});
  assert(it == ++l.begin());
  assert(equal(l, seq(0, 1, 2, 9, 9, 3, 10, 11, 12, 1)));

  it = l.erase_after(l.begin());
  assert(it == ++l.begin());
  assert(equal(l, seq(0, 2, 9, 9, 3, 10, 11, 12, 1)));
  it = l.erase_after(next(l.begin(), 2), next(l.begin(), 5));
  assert(it == next(l.begin(), 3));
  assert(equal(l, seq(0, 2, 9, 10, 11, 12, 1)));

  l.clear();
  assert(l.empty() && size(l) == 0);

  // optional sequence container operations

  l.push_front(1);
  assert(l.front() == 1);
  auto &x = l.emplace_front(2);
  assert(&x == &l.front());
  tmp = 3;
  l.push_front(tmp);
  assert(equal(l, seq(3, 2, 1)));
  assert(l.emplace_front(4) == 4);
  l.push_front(5);
  assert(equal(l, seq(5, 4, 3, 2, 1)));
  l.pop_front();
  assert(equal(l, seq(4, 3, 2, 1)));
  l.pop_front();
  l.pop_front();
  assert(equal(l, seq(2, 1)));
  while (!l.empty())
    l.pop_front();

  // specialized operations of std::forward_list

  l = {1, 2, 3};
  l.resize(2);
  assert(equal(l, seq(1, 2)));
  l.resize(4, 9);
  assert(equal(l, seq(1, 2, 9, 9)));
  l.resize(3, 10);
  assert(equal(l, seq(1, 2, 9)));

  l = {};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice_after(l.before_begin(), ll);
  assert(equal(l, seq(1, 2, 3)) && ll.empty());
  ll.splice_after(ll.before_begin(), l);
  assert(l.empty() && equal(ll, seq(1, 2, 3)));

  l = {1};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice_after(l.begin(), ll, ll.begin());
  assert(equal(l, seq(1, 2)) && equal(ll, seq(1, 3)));
  ll.splice_after(ll.begin(), move(l), l.begin());
  assert(equal(l, seq(1)) && equal(ll, seq(1, 2, 3)));

  l = {1, 2};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice_after(++l.begin(), ll, ll.before_begin(), ll.end());
  assert(equal(l, seq(1, 2, 1, 2, 3)) && ll.empty());
  ll.splice_after(ll.before_begin(), l, l.before_begin(), ++++l.begin());
  assert(equal(ll, seq(1, 2)) && equal(l, seq(1, 2, 3)));

  l = {1, 2, 1};
  l.remove_if([](auto x) {return x == 2;});
  assert(equal(l, seq(1, 1)));
  l.remove(1);
  assert(l.empty());

  l = {1, 2, 2, 2, 2, 3, 4, 4, 5};
  l.unique();
  assert(equal(l, irng(1, 6)));
  l.unique([](auto x, auto y) {return y == x + 1;});
  assert(equal(l, seq(1, 3, 5)));

  l = {1, 3, 5};
  ll = {2, 4, 6};
  l.merge(ll);
  assert(ll.empty() && equal(l, irng(1, 7)));
  ll.merge(move(l));
  assert(l.empty() && equal(ll, irng(1, 7)));
  l.merge(ll, less<>());
  assert(ll.empty() && equal(l, irng(1, 7)));
  ll.merge(move(l), less<>());
  assert(l.empty() && equal(ll, irng(1, 7)));

  l = {1, 2, 3};
  l.reverse();
  assert(equal(l, seq(3, 2, 1)));

  l = {2, 6, 3, 4, 1, 5};
  l.sort();
  assert(equal(l, irng(1, 7)));
  l.sort(greater<>());
  assert(equal(l, rrng(irng(1, 7))));

  // extensions

  l = L(from_range, seq(1, 2, 3));
  assert(equal(l, seq(1, 2, 3)));
  l = L(seq(1, 2, 3));
  assert(equal(l, seq(1, 2, 3)));
  l = seq(4, 5, 6);
  assert(equal(l, seq(4, 5, 6)));
  l.assign(rng(3, 0));
  assert(equal(l, rng(3, 0)));
  l.assign_range(rng(3, 0));
  assert(equal(l, rng(3, 0)));

  l = {};
  it = l.insert_after(l.before_begin(), seq(1, 2, 2));
  assert(it == next(l.begin(), 2) && equal(l, seq(1, 2, 2)));
  it = l.insert_range_after(l.before_begin(), rng(2, 0));
  assert(it == ++l.begin() && equal(l, seq(0, 0, 1, 2, 2)));
  l.push_front(irng(3, 6));
  assert(equal(l, seq(3, 4, 5, 0, 0, 1, 2, 2)));
  l.prepend_range(list<int>{6, 7, 8});
  assert(equal(l, seq(6, 7, 8, 3, 4, 5, 0, 0, 1, 2, 2)));
  l.push_front(rng(1, 9));
  assert(equal(l, seq(9, 6, 7, 8, 3, 4, 5, 0, 0, 1, 2, 2)));
  l.prepend_range(rng(2, 9));
  assert(equal(l, seq(9, 9, 9, 6, 7, 8, 3, 4, 5, 0, 0, 1, 2, 2)));

  []() {
    L l = {1, 2, 3, 4, 5};
    auto prev_it = next(l.begin(), 2);
    auto it = next(prev_it);
    auto it2 = next(l.begin(), 4);
    assert(*prev_it == 3 && *it == 4 && *it2 == 5);
    l.unlink_after(prev_it, l.end());
    l.link_after(l.before_begin(), it, it2);
    assert(equal(l, seq(4, 5, 1, 2, 3)));
  }();

  []() {
    L l = {1, 2, 3};
    decltype(l) ll({4, 5, 6}, l.get_allocator());
    {
      assert(equal(l, irng(1, 4)));
      assert(equal(ll, irng(4, 7)));
      l.swap_after(l.before_begin(), l.before_begin());
      assert(equal(l, irng(1, 4)));
      l.swap_after(l.begin(), l.begin());
      assert(equal(l, irng(1, 4)));
      l.swap_after(next(l.begin()), next(l.begin()));
      assert(equal(l, irng(1, 4)));
      l.swap_after(l.before_begin(), l.begin());
      assert(equal(l, seq(2, 1, 3)));
      l.swap_after(l.begin(), l.before_begin());
      assert(equal(l, seq(1, 2, 3)));
      l.swap_after(l.begin(), next(l.begin()));
      assert(equal(l, seq(1, 3, 2)));
      l.swap_after(next(l.begin()), l.begin());
      assert(equal(l, seq(1, 2, 3)));
      l.swap_after(l.before_begin(), next(l.begin()));
      assert(equal(l, seq(3, 2, 1)));
      l.swap_after(next(l.begin()), l.before_begin());
      assert(equal(l, seq(1, 2, 3)));
    }
    {
      assert(equal(l, irng(1, 4)));
      assert(equal(ll, irng(4, 7)));
      l.swap_after(l.before_begin(), ll, ll.before_begin());
      assert(equal(l, seq(4, 2, 3)));
      assert(equal(ll, seq(1, 5, 6)));
      l.swap_after(l.before_begin(), ll, ll.before_begin());
      assert(equal(l, irng(1, 4)));
      assert(equal(ll, irng(4, 7)));
      l.swap_after(l.begin(), ll, ll.begin());
      assert(equal(l, seq(1, 5, 3)));
      assert(equal(ll, seq(4, 2, 6)));
      l.swap_after(l.begin(), ll, ll.begin());
      assert(equal(l, seq(1, 2, 3)));
      assert(equal(ll, seq(4, 5, 6)));
    }
  }();

  []() {
    L l = {1, 0, 0, 2, 0, 3, 0};
    assert(l.remove(0) == 4 && equal(l, irng(1, 4)));
  }();
}
template <class L>
void test_forward_list_carefully() {
  {
    L l;
    auto it = l.insert_after(l.before_begin(), 1);
    assert(it == l.begin() && equal(l, seq(1)));
    it = l.erase_after(l.before_begin());
    assert(it == l.end() && l.empty());
    l.clear();

    it = l.before_begin();
    assert((l.insert_after(it, rng(0, 0)) == it));
    assert(l.empty());
    l.erase_after(l.before_begin(), l.end());
    assert(l.empty());
    it = l.insert_after(it, rng(3, 0));
    assert(++it == l.end());
    assert(l == L({0, 0, 0}));

    l.push_front(0);
    l.emplace_front(0);
    assert(equal(l, rng(5, 0)));
    l.pop_front();
    assert(equal(l, rng(4, 0)));
    l.pop_front();
    assert(equal(l, rng(3, 0)));
    it = l.begin();
    while (next(it) != l.end())
      ++it;
    *it = 1;
    assert(equal(l, seq(0, 0, 1)));
    assert(l.erase_after(++l.begin()) == l.end());
    assert(equal(l, seq(0, 0)));
    l.pop_front();
    l.pop_front();
    assert(l.empty());

    it = l.insert_after(l.before_begin(), seq(1, 2, 3, 4, 5));
    assert(*it == 5);
    l.erase_after(next(l.before_begin(), 1));
    l.erase_after(next(l.before_begin(), 2));
    assert(equal(l, seq(1, 3, 5)));
    l.push_front(0);
    assert(equal(l, seq(0, 1, 3, 5)));
    it = l.insert_after(l.begin(), 3, 9);
    assert(*++it == 1);
    assert(equal(l, seq(0, 9, 9, 9, 1, 3, 5)));
    it = l.erase_after(next(l.before_begin(), 2),
                       next(l.before_begin(), 6));
    assert(*it == 3);
    assert(equal(l, seq(0, 9, 3, 5)));
    it = l.insert_after(++++++l.before_begin(), seq(0, 1));
    assert(*it == 1);
    assert(equal(l, seq(0, 9, 3, 0, 1, 5)));
    l.clear();
    assert(l.empty() && size(l) == 0);
  }
  {
    const auto test_assignment = [](auto &l) {
      const auto ll = l;
      assert(l == ll);
      l.assign(0, 1);
      assert(l.empty());
      l = ll;
      l.assign(rng(3, 3));
      assert(equal(l, rng(3, 3)));
      l = ll;
      l.assign({1, 2, 3});
      assert(equal(l, seq(1, 2, 3)));
      const auto v = seq(1, 2, 3);
      l = ll;
      l.assign(v.begin(), v.end());
      assert(equal(l, seq(1, 2, 3)));
      l = ll;
      l.assign(seq(1, 2, 3));
      assert(equal(l, seq(1, 2, 3)));
    };
    {
      L l;
      assert(l.empty());
      test_assignment(l);
    }
    {
      L l(1);
      assert(size(l) == 1);
      test_assignment(l);
    }
    {
      L l(3);
      assert(size(l) == 3);
      test_assignment(l);
    }
    {
      L l(1, 3);
      assert(size(l) == 1 && *l.begin() == 3);
      test_assignment(l);
    }
    {
      L l((int *)nullptr, (int *)nullptr);
      assert(l.empty());
      test_assignment(l);
    }
    {
      const auto v = seq(1, 2, 3);
      L l(v.begin(), v.end());
      assert(equal(l, seq(1, 2, 3)));
      test_assignment(l);
    }
    {
      L l(seq(1, 2, 3));
      assert(equal(l, seq(1, 2, 3)));
      test_assignment(l);
    }
    {
      L l(rng(3, 3));
      assert(equal(l, rng(3, 3)));
      test_assignment(l);      
    }
    {
      L l = {1, 2, 3};
      assert(equal(l, seq(1, 2, 3)));
    }
  }
  {
    L l = {1, 3};
    l.splice_after(l.before_begin(), L());
    assert(equal(l, seq(1, 3)));
    l.splice_after(l.begin(), L(seq(2), l.get_allocator()));
    assert(equal(l, seq(1, 2, 3)));
    l.splice_after(next(l.begin(), 2), L({4, 5}, l.get_allocator()));
    assert(equal(l, irng(1, 6)));
    l = {};
    l.splice_after(l.before_begin(), L(seq(1), l.get_allocator()));
    assert(equal(l, seq(1)));
  }
  {
    L l;
    L ll(seq(1, 2, 3), l.get_allocator());
    l.splice_after(l.before_begin(), ll, ll.begin());
    assert(equal(l, seq(2)));
    assert(equal(ll, seq(1, 3)));
    ll.splice_after(ll.begin(), ll, ll.before_begin());
    assert(equal(ll, seq(1, 3)));
    ll.splice_after(ll.begin(), ll, ll.begin());
    assert(equal(ll, seq(1, 3)));
    ll.splice_after(ll.before_begin(), ll, ll.begin());
    assert(equal(ll, seq(3, 1)));
    ll.splice_after(ll.begin(), l, l.before_begin());
    assert(l.empty() && equal(ll, seq(3, 2, 1)));
  }
  {
    L l;
    L ll(l.get_allocator());

    l = seq(1, 2, 3, 4);
    l.splice_after(l.begin(), l, l.begin(), l.end());
    assert(equal(l, seq(1, 2, 3, 4)));
    l.splice_after(next(l.begin(), 3),
                   l, l.before_begin(), next(l.begin(), 3));
    assert(equal(l, seq(4, 1, 2, 3)));

    l = seq(1, 2, 3, 4);
    ll = seq(5, 6, 7);
    const auto it = next(l.begin(), 3);
    l.splice_after(it, ll, ll.before_begin(), ll.end());
    assert(ll.empty() && equal(l, irng(1, 8)));
    l.splice_after(++l.begin(), l, l.before_begin(), ++l.begin());
  }
  {
    L l;
    assert(l.empty());
    l.remove(0);
    assert(l.empty());
    l = seq(0, 1, 1, 2, 3, 1, 4, 1);
    l.remove(1);
    assert(equal(l, seq(0, 2, 3, 4)));
    l.remove(2);
    assert(equal(l, seq(0, 3, 4)));
    l.remove(2);
    assert(equal(l, seq(0, 3, 4)));
    l.remove(3);
    assert(equal(l, seq(0, 4)));
  }
  {
    L l;
    assert(l.empty());
    l.unique();
    assert(l.empty());
    l = seq(1, 1, 2, 3);
    l.unique();
    assert(equal(l, seq(1, 2, 3)));
    l = seq(1, 1, 1, 1, 2, 3);
    l.unique();
    assert(equal(l, seq(1, 2, 3)));
    l = seq(1, 1, 1, 2, 3, 3, 3, 4, 4, 5, 6, 7, 7, 7, 8, 9);
    l.unique();
    assert(equal(l, irng(1, 10)));
  }
  {
    L l;
    L ll(l.get_allocator());
    assert(l.empty());
    l.merge(l);
    assert(l.empty());
    l = seq(1, 3, 5);
    ll = seq(2, 4, 6);
    l.merge(ll);
    assert(equal(l, irng(1, 7)) && ll.empty());
    l = seq(11, 12, 13, 14, 20, 30, 40);
    ll = seq(0, 10, 15, 25);
    l.merge(ll, [](const auto &x, const auto &y) {
        return ((int)x / 10) < ((int)y / 10);
      });
    assert(ll.empty() && equal(l, seq(0,
                                     11, 12, 13, 14, 10, 15,
                                     20, 25,
                                     30,
                                     40)));
  }
  {
    L l;
    assert(l.empty());
    l.reverse();
    assert(l.empty());
    l = seq(1, 2, 3);
    l.reverse();
    assert(equal(l, seq(3, 2, 1)));
    l = seq(1);
    l.reverse();
    assert(equal(l, seq(1)));
    l = seq(1, 2, 3, 4);
    l.reverse();
    assert(equal(l, seq(4, 3, 2, 1)));
    l = irng(0, 100);
    l.reverse();
    assert(equal(l, rrng(irng(0, 100))));
  }
  {
    vector<int> v(irng(0, 1001));
    shuffle(v, minstd_rand{});
    assert(!equal(v, irng(0, 1001)));
    L l;
    l = v;
    l.sort();
    assert(equal(l, irng(0, 1001)));
    l = seq(0,
            20,
            13, 14, 15, 16, 17, 18, 19,
            50,
            40,
            30);
    shuffle(v, minstd_rand{});
    l.sort([](const auto &x, const auto &y) {
        return ((int)x / 10) < ((int)y / 10);
      });
    assert(equal(l, seq(0,
                        13, 14, 15, 16, 17, 18, 19,
                        20,
                        30,
                        40,
                        50)));
  }
}
template <class L>
void test_slist() {
  {
    L l;
    l.link_after(l.before_begin(), l.new_node(1));
    assert(equal(l, seq(1)));
    assert(l.size() == 1);

    l.link_front(l.new_node(2));
    assert(equal(l, seq(2, 1)));
    assert(l.size() == 2);

    auto it = l.begin(), it2 = l.end();
    l.unlink();
    assert(l.size() == 0);
    for_each_node(rng(it, it2), next, [&](auto it) {l.delete_node(it);});

    l.link_after(l.before_begin(), l.new_node(3));
    l.link_after(l.before_begin(), l.new_node(2));
    l.link_after(l.before_begin(), l.new_node(1));
    assert(equal(l, seq(1, 2, 3)));
    assert(l.size() == 3);
    it = next(l.begin());
    it2 = next(l.before_begin(), l.size());
    l.unlink_after(l.begin(), l.end());
    assert(equal(l, seq(1)));
    assert(l.size() == 1);
    l.link_after(l.begin(), it, it2);
    assert(equal(l, seq(1, 2, 3)));
    assert(l.size() == 3);
    []() {
      L l = {1, 2, 3, 4};
      assert(l.size() == 4);
      l.assign(seq(1, 2));
      assert(l.size() == 2);
    }();
    []() {
      L l = {};
      assert(l.size() == 0);
      l.assign(L{});
      assert(l.size() == 0);
      l.assign(seq(1));
      assert(l.size() == 1);
    }();
    []() {
      L l = {1, 2, 3, 4};
      L ll = {2};
      ll.splice_after(ll.before_begin(), l);
      assert(ll.size() == 5);
      assert(l.size() == 0);

      l = {1, 2};
      ll = {3, 4};
      l.splice_after(l.before_begin(), ll, ll.begin());
      assert(l.size() == 3 && ll.size() == 1);
      assert(equal(l, seq(4, 1, 2)) && equal(ll, seq(3)));

      l = {1, 2};
      ll = {2, 3, 4, 5, 6, 7};
      l.splice_after(nth(l, 1), ll, ll.begin(), ll.end());
      assert(equal(l, seq(1, 2, 3, 4, 5, 6, 7)));
      assert(equal(ll, seq(2)));
      assert(l.size() == 7);
      assert(ll.size() == 1);

      l = {1, 1, 2, 2};
      l.unique();
      assert(equal(l, seq(1, 2)));
      assert(l.size() == 2);

      l = {1, 2, 3, 4};
      l.remove(3);
      assert(equal(l, seq(1, 2, 4)));
      assert(l.size() == 3);

      l = {4, 3, 2, 1};
      l.sort();
      assert(equal(l, irng(1, 5)));
      assert(l.size() == 4);
    }();
  }
}

void test_forward_list_replace() {
  {
    for (int i : irng(0, 10)) {
      for (int j : irng(0, 10)) {
        for (int k : irng(0, i + 1))
          for (int m : irng(0, i - k)) {
            forward_list<int> l(irng(0, i));
            const auto prev_it1 = next(l.before_begin(), k);
            const auto it1 = next(prev_it1);
            assert(m <= distance(it1, end(l)));
            const auto it2 = next(it1, m);
            const auto z = l.replace_after(prev_it1, it2, rng(j, 0));
            assert(next(z) == it2);
            vector<int> v(irng(0, i));
            v.replace(nth(v, k), nth(v, k) + m, rng(j, 0));
            assert(equal(l, v));
          }
      }
    }
    forward_list<int> l = {1, 2, 3, 4, 5};
    auto it = l.replace_after(l.before_begin(), l.end(),
                              degraded_irng(rng(3, 2)));
    assert(*it == 2 && equal(l, rng(3, 2)));
  }
}
void test_forward_list_node_and_slice() {
  using L = forward_list<int, stateful_test_allocator<int>>;
  // for node
  {
    L l = {1, 3};
    assert(*l.insert_after(l.begin(), l.make_node(2)) == 2);
    assert(equal(l, irng(1, 4)));
    assert(l.insert_after(l.before_begin(), L::node_type{})
           == l.before_begin());

    L::node_type n = l.extract_after(l.begin());
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));

    n = l.exchange_after(l.before_begin(), n);
    assert(*n == 1);
    assert(equal(l, seq(2, 3)));
    n = l.exchange_after(l.begin(), move(n));
    assert(*n == 3);
    assert(equal(l, seq(2, 1)));
    n = l.exchange_after(l.before_begin(), L::node_type{});
    assert(*n == 2);
    assert(equal(l, seq(1)));
    l = {2, 1};

    assert(*l.replace_after(l.begin(), l.make_node(3)) == 3);
    assert(equal(l, seq(2, 3)));
    assert(l.replace_after(l.before_begin(), L::node_type{})
           == l.before_begin());
  }
  {
    L l = {1, 2, 3};
    L::node_type n = l.make_node(0);
    l.push_front(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3)));
    l.push_front(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3)));
  }
  // for slice
  {
    L l = {1, 2, 3, 4};
    L m(l.extract_after(l.before_begin(), l.begin()),
        l.get_allocator());
    assert(m.empty());
    m = l.extract_after(l.begin(), nth(l, 3));
    assert(l == L({1, 4}) && m == L({2, 3}));

    assert(*l.insert_after(l.begin(), move(m)) == 3);
    assert(m.empty() && l == L({1, 2, 3, 4}));

    m = l.exchange_after(l.begin(), nth(l, 3), L({0, 0, 0}, l.get_allocator()));
    assert(l == L({1, 0, 0, 0, 4}));
    assert(m == L({2, 3}));
    m = l.exchange_after(l.before_begin(), l.begin(), L{});
    assert(m.empty() && l == L({1, 0, 0, 0, 4}));

    l = {1, 2, 3, 4};
    const auto it = l.replace_after(l.begin(), nth(l, 3),
                                    L({0, 0, 0}, l.get_allocator()));
    assert(*next(it) == 4);
    assert(l == L({1, 0, 0, 0, 4}));
    const auto it2 = l.replace_after(nth(l, 2), nth(l, 3), L{});
    assert(it2 == nth(l, 2));
    assert(l == L({1, 0, 0, 0, 4}));
  }
  {
    L l = {1, 2, 3};
    L m({0, 0}, l.get_allocator());
    const auto p1 = addressof(ref(m, 0));
    const auto p2 = addressof(ref(m, 1));
    l.push_front(move(m));
    assert(m.empty() && equal(l, seq(0, 0, 1, 2, 3)));
    assert(addressof(ref(l, 0)) == p1);
    assert(addressof(ref(l, 1)) == p2);
    l.push_front(move(m));
    assert(m.empty() && equal(l, seq(0, 0, 1, 2, 3)));
  }
}

void test_forward_list() {
  printf("container - forward_list: ");

  inner::fns::test_forward_list_ownership();
  inner::fns::test_forward_list_intrusive_mode();
  inner::fns::test_forward_list_briefly<forward_list
                                        <int, stateful_test_allocator<int>>>();
  inner::fns::test_forward_list_briefly<forward_list
                                        <int, test_allocator<int>>>();
  inner::fns::test_forward_list_briefly<forward_list
                                        <test_object<int>,
                                         stateful_test_allocator
                                         <test_object<int>>>>();
  inner::fns::test_forward_list_carefully
    <forward_list<int, stateful_test_allocator<int>>>();
  inner::fns::test_forward_list_carefully<forward_list
                                          <int, test_allocator<int>>>();
  inner::fns::test_forward_list_carefully
    <forward_list
     <test_object<int>,
      stateful_test_allocator<test_object<int>>>>();
  inner::fns::test_slist<slist<int, test_allocator<int>>>();

  inner::fns::test_forward_list_replace();
  inner::fns::test_forward_list_node_and_slice();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_forward_list();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
