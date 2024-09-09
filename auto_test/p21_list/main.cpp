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

void test_list_ownership() {
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
      testf(type_tag<list<int, stateful_test_allocator<int>>>());
      testf(type_tag<list<test_object<int>>>());
      testf(type_tag
            <bidirectional_list<int, stateful_test_allocator<int>>>());
      testf(type_tag<bidirectional_list<test_object<int>>>());

      using P00 = type_pack<true_type, false_type>;
      using P0 = type_pack_mul<P00, P00>;
      using P = type_pack_mul<P0, P0>;
      type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
          constexpr bool y1 = decltype(a)::type::value;
          constexpr bool y2 = decltype(b)::type::value;
          constexpr bool y3 = decltype(c)::type::value;
          constexpr bool y4 = decltype(d)::type::value;

          {
            using vec_t = list<int, stateful_test_allocator
                               <int, ez_map, y1, y2, y3, y4>>;
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
            using vec_t = bidirectional_list
              <int, stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
            using alloc_t = stateful_test_allocator
              <int, ez_map, y1, y2, y3, y4>;
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
struct myt : join_list<> {
  int i;

  myt() = default;
  ~myt() = default;
  myt(const myt &) = default;
  myt &operator =(const myt &) = default;
  myt(myt &&) = default;
  myt &operator =(myt &&) = default;
  void swap(myt &x) noexcept {
    adl_swap(i, x.i);
    adl_swap((join_list<> &)*this, (join_list<> &)x);
  }

  myt(int i) : i(i) {}

  bool operator ==(const myt &x) const {
    return i == x.i;
  }
  bool operator !=(const myt &x) const {
    return i != x.i;
  }
  bool operator <(const myt &x) const {
    return i < x.i;
  }

  bool operator ==(int j) const {
    return i == j;
  }
};
void swap(myt &x, myt &y) noexcept {
  x.swap(y);
}
template <bool NOSZ>
void test_list_intrusive_mode() {
  vector<myt> v;
  using L = list_adaptor<list_traits<myt, 0, !NOSZ>>;
  static_assert(is_default_constructible_v<L>);
  static_assert(!is_copy_constructible_v<L>);
  static_assert(!is_copy_assignable_v<L>);
  static_assert(is_nothrow_move_constructible_v<L>);
  static_assert(is_nothrow_move_assignable_v<L>);
  static_assert(is_nothrow_swappable_v<L>);

  {
    v = {1, 2, 3, 4, 5};
    L l;
    for (auto &x : v)
      l.link(l.end(), rng_itr<L>(addressof(x)));
    assert(equal(l, seq(1, 2, 3, 4, 5)));
    assert(l.unlink(next(l.begin(), 2))->i == 4);
    assert(equal(l, seq(1, 2, 4, 5)));
    l.unlink(l.begin());
    l.unlink(--l.end());
    l.unlink(l.begin());
    l.unlink(l.begin(), l.begin());
    assert(size(l) == 1 && l.front() == 4);
    l.unlink(l.begin());
    assert(l.empty());
    l.link(l.end(), rng_itr<L>(addressof(v[2])));
    assert(size(l) == 1 && l.front() == 3);
    l.link(l.begin(), rng_itr<L>(addressof(v[3])));
    assert(equal(l, seq(4, 3)));
    l.unlink();
    assert(l.empty());
  }
  {
    v = {1, 2, 3};
    L l, ll;
    l.link(l.end(), rng_itr<L>(addressof(v[0])));
    ll = move(l);
    assert(l.empty() && size(ll) == 1 && ll.front() == 1);
    L lll = move(ll);
    assert(ll.empty() && size(lll) == 1 && lll.front() == 1);
    swap(l, lll);
    assert(lll.empty() && size(l) == 1 && l.front() == 1);
  }
  {
    v = irng(0, 1001);
    L l;
    shuffle(v, minstd_rand{});
    for (auto &x : v)
      l.link(l.end(), rng_itr<L>(addressof(x)));
    l.sort();
    assert(is_sorted(l));
    l.reverse();
    assert(!is_sorted(l) && is_sorted(rrng(l)));
    l.unlink();
    l.sort();
    l.reverse();
    l.sort();
    l.reverse();
    assert(l.empty());
    l.link(l.end(), rng_itr<L>(addressof(v[0])));
    l.reverse();
    assert(size(l) == 1 && l.front() == v[0]);
  }
  {
    v = {1, 2, 3, 4, 5};
    L l;
    for (auto &x : v)
      l.link(l.end(), rng_itr<L>(addressof(x)));
    assert(equal(l, v));
    assert(l.unlink(l.begin(), l.end()) == l.end() && l.empty());
    l.link_front(rng_itr<L>(addressof(v[1])));
    l.link_back(rng_itr<L>(addressof(v[2])));
    l.link_front(rng_itr<L>(addressof(v[0])));
    assert(equal(l, seq(1, 2, 3)));
    l.unlink();
    assert(l.empty());
  }
  {
    v = {1, 2, 0, 0, 3, 4, 0, 5};
    L l;
    for (auto &x : v)
      l.link_back(rng_itr<L>(addressof(x)));
    l.unique();
    assert(equal(l, seq(1, 2, 0, 3, 4, 0, 5)));
    l.remove(0);
    l.unique();
    assert(equal(l, seq(1, 2, 3, 4, 5)));
    assert(equal(v, seq(1, 2, 0, 0, 3, 4, 0, 5)));
    l.remove(3);
    assert(equal(l, seq(1, 2, 4, 5)));
    l.remove_if([](auto) {return true;});
    assert(l.empty() && size(l) == 0);
  }
  {
    v = {1, 3, 5, 7, 9};
    decltype(v) vv = {2, 4, 6, 8, 10};
    L l, ll;
    l.merge(l);
    assert(l.empty());
    for (auto &x : v)
      l.link_back(rng_itr<L>(addressof(x)));
    l.merge(l);
    assert(equal(l, seq(1, 3, 5, 7, 9)));
    for (auto &x : vv)
      ll.link_back(rng_itr<L>(addressof(x)));
    l.merge(ll);
    assert(equal(l, irng(1, 11)) && ll.empty());
    ll.merge(l);
    assert(equal(ll, irng(1, 11)) && l.empty());
  }
}
template <class L>
void test_list_briefly() {
  {
    L l;
    auto n1 = l.new_node(1);
    auto n2 = l.new_node(2);
    auto n3 = l.new_node(3);
    assert(*l.link_back(n3) == 3);
    assert(*l.link_front(n1) == 1);
    assert(*l.link(++l.begin(), n2) == 2);
    assert(equal(l, seq(1, 2, 3)));
    assert(*l.unlink(next(l.begin())) == 3);
    l.delete_node(n2);
    l.unlink();
    assert(l.empty());
    l.delete_node(n1);
    l.delete_node(n3);
  }

  L l, ll;

  // container

  assert(l.begin() == l.end());
  assert(l.cbegin() == l.end());
  assert(l.begin() == l.cend());
  assert(l.cbegin() == l.cend());
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

  // reversible container

  l = {1, 2, 3};
  assert(equal(rrng(l), rng(l.rbegin(), l.rend())));
  assert(equal(rrng(l), rng(l.crbegin(), l.crend())));

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
  l = L(ll.rbegin(), ll.rend(), ll.get_allocator());
  assert(equal(l, rrng(ll)));
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
  auto it = l.emplace(l.end(), 1);
  assert(it == l.begin() && equal(l, seq(1)));
  it = l.insert(l.begin(), 2);
  assert(it == l.begin() && equal(l, seq(2, 1)));
  int tmp = 3;
  it = l.insert(++l.begin(), tmp);
  assert(it == ++l.begin() && equal(l, seq(2, 3, 1)));

  it = l.insert(++l.begin(), 2, 9);
  assert(it == ++l.begin() && equal(l, seq(2, 9, 9, 3, 1)));
  const auto r = irng(10, 13);
  it = l.insert(--l.end(), r.begin(), r.end());
  assert(it == prev(l.end(), 4));
  assert(equal(l, seq(2, 9, 9, 3, 10, 11, 12, 1)));
  it = l.insert(l.begin(), {0, 1});
  assert(it == l.begin());
  assert(equal(l, seq(0, 1, 2, 9, 9, 3, 10, 11, 12, 1)));

  it = l.erase(l.begin());
  assert(it == l.begin());
  l.erase(l.begin(), l.begin());
  l.erase(l.end(), l.end());
  assert(equal(l, seq(1, 2, 9, 9, 3, 10, 11, 12, 1)));
  it = l.erase(next(l.begin(), 3), next(l.begin(), 5));
  assert(it == next(l.begin(), 3));
  assert(equal(l, seq(1, 2, 9, 10, 11, 12, 1)));

  l.clear();
  assert(l.empty() && size(l) == 0);

  // optional sequence container operations

  l.push_back(1);
  assert(l.front() == l.back() && l.front() == 1);
  auto &x = l.emplace_back(2);
  assert(addressof(x) == addressof(l.back()));
  tmp = 3;
  l.push_back(tmp);
  assert(equal(l, seq(1, 2, 3)));
  assert(l.emplace_front(1) == 1);
  l.push_front(2);
  l.push_front(tmp);
  assert(equal(l, seq(3, 2, 1, 1, 2, 3)));
  l.pop_front();
  l.pop_back();
  assert(equal(l, seq(2, 1, 1, 2)));
  while (!l.empty())
    l.pop_back();

  // specialized operations of std::list

  l = {1, 2, 3};
  l.resize(2);
  assert(equal(l, seq(1, 2)));
  l.resize(4, 9);
  assert(equal(l, seq(1, 2, 9, 9)));

  l = {};
  ll = {1, 2, 3};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice(l.end(), ll);
  assert(equal(l, seq(1, 2, 3)) && ll.empty());
  ll.splice(ll.end(), move(l));
  assert(equal(ll, seq(1, 2, 3)) && l.empty());

  l = {1};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice(l.end(), ll, ++ll.begin());
  assert(equal(l, seq(1, 2)) && equal(ll, seq(1, 3)));
  ll.splice(++ll.begin(), move(l), ++l.begin());
  assert(equal(l, seq(1)) && equal(ll, seq(1, 2, 3)));

  l = {1, 2};
  ll = L({1, 2, 3}, l.get_allocator());
  l.splice(l.end(), ll, ll.begin(), ll.end());
  assert(equal(l, seq(1, 2, 1, 2, 3)) && ll.empty());
  ll.splice(ll.end(), l, l.begin(), ++++l.begin());
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
  assert(equal(l, seq(0, 0, 0)));

  l = {};
  it = l.insert(l.end(), seq(1, 2, 2));
  assert(it == l.begin() && equal(l, seq(1, 2, 2)));
  it = l.insert_range(l.begin(), rng(2, 0));
  assert(it == l.begin());
  assert(equal(l, seq(0, 0, 1, 2, 2)));
  l.push_back(irng(3, 6));
  assert(equal(l, seq(0, 0, 1, 2, 2, 3, 4, 5)));
  l.append_range(list<int>{6, 7, 8});
  assert(equal(l, seq(0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 8)));
  l.push_front(rng(1, 9));
  l.prepend_range(rng(2, 9));
  assert(equal(l, seq(9, 9, 9, 0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 8)));

  []() {
    L l = {1, 2, 3, 4};
    auto it = nth(l, 2);
    auto it2 = nth(l, 3);
    assert(l.end() == l.unlink(it, next(it2)));
    l.link(l.begin(), it, it2);
    assert(equal(l, seq(3, 4, 1, 2)));
    assert(equal(rrng(l), rrng(seq(3, 4, 1, 2))));
  }();

  []() {
    L l = {1, 2, 3};
    decltype(l) ll({4, 5, 6}, l.get_allocator());
    {
      assert(equal(l, irng(1, 4)));
      assert(equal(ll, irng(4, 7)));
      l.swap(l.begin(), l.begin());
      assert(equal(l, irng(1, 4)));
      assert(equal(rrng(l), rrng(irng(1, 4))));
      l.swap(l.begin(), next(l.begin()));
      assert(equal(l, seq(2, 1, 3)));
      assert(equal(rrng(l), rrng(seq(2, 1, 3))));
      l.swap(next(l.begin()), l.begin());
      assert(equal(l, seq(1, 2, 3)));
      assert(equal(rrng(l), rrng(seq(1, 2, 3))));
      l.swap(l.begin(), prev(l.end()));
      assert(equal(l, seq(3, 2, 1)));
      assert(equal(rrng(l), rrng(seq(3, 2, 1))));
      l.swap(l.begin(), prev(l.end()));
      assert(equal(l, seq(1, 2, 3)));
      assert(equal(rrng(l), rrng(seq(1, 2, 3))));
    }
    {
      assert(equal(l, irng(1, 4)));
      assert(equal(ll, irng(4, 7)));
      l.swap(l.begin(), ll, ll.begin());
      assert(equal(l, seq(4, 2, 3)));
      assert(equal(ll, seq(1, 5, 6)));
      assert(equal(rrng(l), rrng(seq(4, 2, 3))));
      assert(equal(rrng(ll), rrng(seq(1, 5, 6))));
      l.swap(l.begin(), ll, ll.begin());
      assert(equal(l, seq(1, 2, 3)));
      assert(equal(ll, seq(4, 5, 6)));
      assert(equal(rrng(l), rrng(seq(1, 2, 3))));
      assert(equal(rrng(ll), rrng(seq(4, 5, 6))));
      l.swap(next(l.begin()), ll, next(ll.begin()));
      assert(equal(l, seq(1, 5, 3)));
      assert(equal(ll, seq(4, 2, 6)));
      assert(equal(rrng(l), rrng(seq(1, 5, 3))));
      assert(equal(rrng(ll), rrng(seq(4, 2, 6))));
      l.swap(next(l.begin()), ll, next(ll.begin()));
      assert(equal(l, seq(1, 2, 3)));
      assert(equal(ll, seq(4, 5, 6)));
      assert(equal(rrng(l), rrng(seq(1, 2, 3))));
      assert(equal(rrng(ll), rrng(seq(4, 5, 6))));
    }
  }();

  []() {
    L l = {1, 0, 0, 2, 0, 3, 0};
    assert(l.remove(0) == 4 && equal(l, irng(1, 4)));
  }();

  // replace
  {
    using S = list<int>;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}
template <class L>
void test_list_carefully() {
  {
    L l;
    const auto ed = l.end();
    l.insert(ed, 1);
    assert(equal(l, seq(1)));
    l.clear();
    assert(l.insert(ed, rng(0, 2)) == ed);
    assert(l.empty());
    l.erase(l.begin(), l.end());
    assert(l.empty());
    l.insert(ed, seq(1, 2));
    assert(equal(l, seq(1, 2)));
    l.push_front(0);
    l.push_back(3);
    l.push_back(seq(4, 5));
    assert(equal(l, irng(0, 6)));
    l.pop_front();
    l.pop_back();
    l.erase(prev(ed));
    l.erase(prev(prev(ed)));
    assert(equal(l, seq(1, 3)));
    assert(l.erase(ed, ed) == ed);
    l.insert(next(l.begin()), rng(3, 9));
    assert(equal(l, seq(1, 9, 9, 9, 3)));
    l.insert(next(l.begin()), degraded_irng(rng(2, 9)));
    assert(equal(l, seq(1, 9, 9, 9, 9, 9, 3)));
    assert(*l.erase(++++l.begin(), ----copy(ed)) == 9);
    assert(equal(l, seq(1, 9, 9, 3)));
    l.clear();
    l.insert(ed, {1, 2, 3, 4});
    l.insert(ed, {5});
    l.insert(ed, {});
    assert(size(l) == 5);
    assert(equal(l, irng(1, 6)));
    l.erase(l.begin(), prev(ed));
    l.erase(prev(ed));
    assert(l.empty());
    l = seq(1, 2, 3);
    l.resize(5, 4);
    assert(equal(l, seq(1, 2, 3, 4, 4)));
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
      assert(size(l) == 1 && l.back() == 3);
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
    assert(equal(l, seq(1, 3)));
    l.splice(l.end(), L());
    assert(equal(l, seq(1, 3)));
    l.splice(++l.begin(), L(seq(2), l.get_allocator()));
    assert(equal(l, seq(1, 2, 3)));
    l = {1, 3};
    l.splice(++l.begin(), L{});
    assert(equal(l, seq(1, 3)));
    L ll(l.get_allocator());
    ll = seq(2, 2, 2, 2);
    l.clear();
    l.splice(l.end(), ll);
    assert(ll.empty());
    assert(equal(l, rng(4, 2)));
  }
  {
    L l;
    L ll(seq(1, 2, 3), l.get_allocator());
    l.splice(l.end(), ll, ++ll.begin());
    assert(equal(l, seq(2)) && equal(ll, seq(1, 3)));
    l.splice(l.begin(), ll, ll.begin());
    assert(equal(l, seq(1, 2)) && equal(ll, seq(3)));
    l.splice(l.end(), ll, ll.begin());
    assert(equal(l, seq(1, 2, 3)) && ll.empty());
    l.splice(l.begin(), l, ++l.begin());
    assert(equal(l, seq(2, 1, 3)));
    l.splice(l.begin(), l, l.begin());
    assert(equal(l, seq(2, 1, 3)));
    l.splice(++l.begin(), l, l.begin());
    assert(equal(l, seq(2, 1, 3)));
  }
  {
    L l;
    L ll(l.get_allocator());
    l = seq(1, 2, 3);
    l.splice(l.end(), l, l.end(), l.end());
    assert(equal(l, seq(1, 2, 3)));
    l.splice(l.end(), l, l.begin(), l.end());
    assert(equal(l, seq(1, 2, 3)));
    l = seq(0, 1, 2, 3);
    l.splice(++l.begin(), l, ++++l.begin(), l.end());
    assert(equal(l, seq(0, 2, 3, 1)));
    l.splice(l.end(), l, l.begin(), --l.end());
    assert(equal(l, seq(1, 0, 2, 3)));
    l = seq(1, 2, 3);
    ll = seq(4, 5, 6);
    l.splice(++l.begin(), ll, ++ll.begin(), ++++ll.begin());
    assert(equal(l, seq(1, 5, 2, 3)) && equal(ll, seq(4, 6)));
    l.splice(l.begin(), ll, ll.begin(), ll.end());
    assert(equal(l, seq(4, 6, 1, 5, 2, 3)) && ll.empty());
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

  // swap
  {
    L l = {1};
    l.swap(l.begin(), l.begin());
    assert(equal(l, single_rng(1)));
    l.swap(l.begin(), l, l.begin());
    assert(equal(l, single_rng(1)));

    l = {1, 2, 3};
    l.swap(nth(l, 1), nth(l, 1));
    assert(equal(l, irng(1, 4)));
    l.swap(nth(l, 1), nth(l, 1));
    assert(equal(l, irng(1, 4)));

    l.swap(nth(l, 0), nth(l, 0));
    assert(equal(l, irng(1, 4)));
    l.swap(nth(l, 0), nth(l, 0));
    assert(equal(l, irng(1, 4)));

    l.swap(nth(l, 2), nth(l, 2));
    assert(equal(l, irng(1, 4)));
    l.swap(nth(l, 2), nth(l, 2));
    assert(equal(l, irng(1, 4)));
  }
}
void test_list_exception_safety_of_insert() {
#ifndef RE_NOEXCEPT
  list<exception_countdown<int>,
       stateful_test_allocator<exception_countdown<int>>> l;
  exception_countdown<int>::copy_constructor_countdown(3);
  bool y = false;
  try {
    l.insert(l.end(), rng(5, exception_countdown<int>()));
  }
  catch (const logic_error &) {
    y = true;
  }
  assert(y && l.empty());
#endif
}

void test_list_node_and_slice() {
  using L = list<int, stateful_test_allocator<int>>;
  {
    L::node_type n0;
    assert(n0.empty());
    L l;
    L::node_type n = l.make_node(1);
    assert(!n.empty() && *n == 1);
  }
  {
    L l = {1, 2, 3};
    L::node_type n = l.extract(nth(l, 1));
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    L::iterator it = l.insert(l.begin(), move(n));
    assert(n.empty());
    assert(*it == 2 && equal(l, seq(2, 1, 3)));

    n = l.exchange(l.begin(), L::node_type{});
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    n = l.exchange(l.begin(), n);
    assert(!n.empty() && *n == 1);
    assert(equal(l, seq(2, 3)));

    assert(*l.replace(l.begin(), move(n)) == 1);
    assert(equal(l, seq(1, 3)));
    assert(l.replace(l.begin(), L::node_type{}) == next(l.begin()));
    assert(equal(l, seq(3)));
  }
  {
    L l = {1, 2, 3, 4};
    L m(l.extract(l.begin(), l.end()), l.get_allocator());
    assert(l.empty() && equal(m, irng(1, 5)));
    assert(l.insert(l.end(), L{}) == l.end());
    assert(l.empty());
    const auto tmp = l.insert(l.end(), L({1}, l.get_allocator()));
    assert(tmp == l.begin() && equal(l, seq(1)));

    l = {1, 0, 0, 4};
    m = l.exchange(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator()));
    assert(equal(m, seq(0, 0)) && equal(l, irng(1, 5)));
    m = l.exchange(l.begin(), l.end(), m);
    assert(equal(l, seq(0, 0)) && equal(m, irng(1, 5)));

    l = {1, 0, 0, 4};
    assert(*l.replace(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator())) == 2);
    assert(equal(l, irng(1, 5)));
  }
  {
    L l = {1, 2, 3};
    L::node_type n = l.make_node(0);
    l.push_front(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3)));
    l.push_front(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3)));
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3)));
    n = l.make_node(4);
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(0, 1, 2, 3, 4)));
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
  {
    L l = {1, 2, 3};
    L m({0, 0}, l.get_allocator());
    const auto p1 = addressof(ref(m, 0));
    const auto p2 = addressof(ref(m, 1));
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
    assert(addressof(ref(l, 3)) == p1);
    assert(addressof(ref(l, 4)) == p2);
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
  }
}

void test_list() {
  printf("container - list: ");

  inner::fns::test_list_intrusive_mode<false>();
  inner::fns::test_list_intrusive_mode<true>();
  inner::fns::test_list_briefly<list<int, stateful_test_allocator<int>>>();
  inner::fns::test_list_briefly<list<test_object<int>,
                                     stateful_test_allocator
                                     <test_object<int>>>>();
  inner::fns::test_list_briefly<list_adaptor
                                <list_traits
                                 <list_node<int, stateful_test_allocator<int>>,
                                  0, 0, 1, stateful_test_allocator<int>>>>();
  inner::fns::test_list_carefully<list<int, stateful_test_allocator<int>>>();
  inner::fns::test_list_carefully<list<test_object<int>,
                                       stateful_test_allocator
                                       <test_object<int>>>>();
  inner::fns::test_list_exception_safety_of_insert();
  inner::fns::test_list_node_and_slice();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_list();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
