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
#include <iostream>

namespace re::inner::test {

void test_bool_vector() {
  auto rng_bind_to_int = [](auto &&r) {
    return bind_rng(r, [](auto &&x)->int {return x;});
  };

  {
    vector<bool> v;
    assert(v.begin() == v.end());
    static_assert(is_convertible
                  <decltype(v)::iterator, decltype(v)::const_iterator>);
    static_assert(!is_convertible
                  <decltype(v)::const_iterator, decltype(v)::iterator>);

    v.push_back(true);
    assert(v.size() == 1);
    assert(!v.empty());
    assert(v.begin() + 1 == v.end());
    assert(v[0] == true);
  }
  {
    vector<int, stateful_test_allocator<int>> v1;
    vector<bool, stateful_test_allocator<bool>> v2;
    auto r = bind(rational_bernoulli_distribution(3, 4), minstd_rand());
    for (int c = 640; c; --c) {
      bool y = r();
      v1.push_back(y);
      v2.push_back(y);
    }
    assert(equal(v1, rng_bind_to_int(v2)));
    assert(equal(v1, rng_bind_to_int(as_const(v2))));
    assert(v1.back() == (int)v2.back());
    assert(equal(rrng(v1), rng_bind_to_int(rrng(v2))));
    assert(equal(rrng(v1), rng_bind_to_int(rrng(as_const(v2)))));
    assert(equal(v1,
                 rng_bind_to_int
                 (bind_rng(irng(-640, 0),
                           [&](int i) {return *(v2.end() + i);}))));
    assert(equal(v1,
                 rng_bind_to_int
                 (bind_rng(irng(0, 640), [&](int i) {return v2[i];}))));

    for (int i : irng(0, 40))
      test_lessness(v2.begin() + i, v2.begin() + 41);
    for (int i : irng(42, 640))
      test_lessness(v2.begin() + 41, v2.begin() + i);
    for (int i : irng(0, 128))
      test_lessness(v2.begin() + i, v2.begin() + 128);
    for (int i : irng(129, 640))
      test_lessness(v2.begin() + 128, v2.begin() + i);

    for (int i : irng(0, 640)) {
      assert(v2.begin() + i - i == v2.begin());
      for (int j : irng(0, 640))
        assert((v1.begin() + i) - (v1.begin() + j)
               == (v2.begin() + i) - (v2.begin() + j));
    }

    {
      sort(v1);
      assert(is_partitioned(v1, [](auto x) {return x == 0;}));

      auto v3 = v2;
      auto v4 = v2;
      auto v5 = v2;
      sort(v2);
      assert(equal(v1, rng_bind_to_int(v2)));
      heap_sort(v3);
      assert(equal(v1, rng_bind_to_int(v3)));
      stable_sort(v4);
      assert(equal(v1, rng_bind_to_int(v4)));
      stable_sort_with_no_buffer(v5);
      assert(equal(v1, rng_bind_to_int(v5)));
    }
  }
  {
    vector<int, stateful_test_allocator<int>> v1;
    vector<bool, stateful_test_allocator<bool>> v2;
    auto r = bind(rational_bernoulli_distribution(1, 2), minstd_rand());
    auto rand = minstd_rand();
    using dis_param_t = typename uniform_int_distribution<int>::param_type;
    uniform_int_distribution<int> dis;

    for (int c = 20000; c; --c) {
      bool y = r();
      v1.push_back(y);
      v2.push_back(y);
    }
    assert(equal(v1, rng_bind_to_int(v2)));

    {
      auto vv1 = v1;
      auto vv2 = v2;
      assert(equal(vv1, rng_bind_to_int(vv2)));
      for (int c = v1.size(); c; --c) {
        vv1.pop_back();
        vv2.pop_back();
        assert(equal(vv1, rng_bind_to_int(vv2)));
      }
    }

    {
      auto vv1 = v1;
      auto vv2 = v2;
      assert(equal(vv1, rng_bind_to_int(vv2)));
      while (!vv1.empty()) {
        int i = dis(rand,
                    dis_param_t(1, min_value(196, to_signed(vv1.size()))));
        auto it = vv1.erase(vv1.end() - i);
        auto it2 = vv2.erase(vv2.end() - i);
        assert(it - vv1.begin() == it2 - vv2.begin());
        assert(equal(vv1, rng_bind_to_int(vv2)));
      }
    }

    {
      auto vv1 = v1;
      auto vv2 = v2;
      assert(equal(vv1, rng_bind_to_int(vv2)));
      while (!vv1.empty()) {
        int i = dis(rand,
                    dis_param_t(1, min_value(196, to_signed(vv1.size()))));
        auto it = vv1.erase(vv1.end() - i, vv1.end());
        auto it2 = vv2.erase(vv2.end() - i, vv2.end());
        assert(it - vv1.begin() == it2 - vv2.begin());
        assert(equal(vv1, rng_bind_to_int(vv2)));
      }
    }

    assert(equal(v1, rng_bind_to_int(v2)));
    while (!v1.empty()) {
      int i = dis(rand, dis_param_t(0, v1.size() - 1));
      auto it = v1.erase(v1.begin() + i);
      auto it2 = v2.erase(v2.begin() + i);
      assert(it - v1.begin() == it2 - v2.begin());
      assert(equal(v1, rng_bind_to_int(v2)));
    }
    assert(v2.empty());

    for (int c = 80000; c; --c) {
      bool y = r();
      v1.push_back(y);
      v2.push_back(y);
    }
    assert(equal(v1, rng_bind_to_int(v2)));
    while (!v1.empty()) {
      int i = min_value(v1.size(), to_unsigned(dis(rand, dis_param_t(1, 192))));
      int j = dis(rand, dis_param_t(0, v1.size() - i));
      auto it1 = v1.erase(v1.begin() + j, v1.begin() + j + i);
      auto it2 = v2.erase(v2.begin() + j, v2.begin() + j + i);
      assert(it1 - v1.begin() == it2 - v2.begin());
      assert(equal(v1, rng_bind_to_int(v2)));
    }
  }
  {
    vector<bool, stateful_test_allocator<bool>> v;
    v = {true, false, true};
    assert(equal(v, seq(true, false, true)));
    v.assign(1000, true);
    assert(v.size() == 1000);
    assert(all_of_equal(v, true));
    assert(v.emplace_back(false) == false);
    v.clear();
    assert(v.emplace_back(false) == false);
    v = rng(200, true);
    assert(equal(v, rng(200, true)));
    v = seq(true, false, true);
    assert(equal(v, seq(true, false, true)));
    v = rng(135, true);
    v.flip();
    for (auto ref : v)
      ref.flip();
    vector<bool, stateful_test_allocator<bool>> v2(rng(135, true));
    assert(v.size() == 135);
    assert(v2.size() == 135);
    assert(all_of_equal(v, true));
    assert(all_of_equal(v2, true));
    assert(v == v2);
  }
  {
    auto r = bind(rational_bernoulli_distribution(1, 2), minstd_rand{});
    auto rand = minstd_rand{};
    using dis_param_t = typename uniform_int_distribution<int>::param_type;
    uniform_int_distribution<int> dis;

    using V0 = vector<int, stateful_test_allocator<int>>;
    using V = vector<bool, stateful_test_allocator<bool>>;
    V0 v0;
    V v;
    assert(v.empty());
    assert(*v.emplace(v.end(), true) == true);
    assert(v.size() == 1 && v[0] == true);
    v0.push_back(true);
    assert(equal(v0, rng_bind_to_int(v)));
    v.emplace(v.begin(), false);
    assert(v.size() == 2);
    assert(v[0] == false && v[1] == true);
    v.erase(v.begin());
    assert(v.size() == 1);
    assert(v[0] == true);
    assert(equal(v0, rng_bind_to_int(v)));

    for (int c = 999; c; --c) {
      bool y = r();
      auto it = v0.emplace(v0.begin(), y);
      auto it2 = v.emplace(v.begin(), y);
      assert(it - v0.begin() == it2 - v.begin());
      assert(equal(v0, rng_bind_to_int(v)));
    }
    v0.clear();
    v.clear();
    assert(equal(v0, rng_bind_to_int(v)));
    for (int c = 10086; c; --c) {
      bool y = r();
      const int i = dis(rand, dis_param_t(0, v0.size()));
      auto it = v0.emplace(v0.end() - i, y);
      auto it2 = v.emplace(v.end() - i, y);
      assert(*it == (int)*it2 && *it == (int)y);
      assert(equal(v0, rng_bind_to_int(v)));
    }
    for (int c = 1000; c; --c) {
      bool y = r();
      const int i = dis(rand, dis_param_t(1, 196));
      const int j = dis(rand, dis_param_t(0, v0.size() - 1));
      auto it = v0.insert(v0.begin() + j, rng(i, y));
      auto it2 = v.insert(v.begin() + j, rng(i, y));
      assert(*it == (int)*it2 && *it == (int)y);
      assert(equal(v0, rng_bind_to_int(v)));
    }
    v0.clear();
    v.clear();
    assert(equal(v0, rng_bind_to_int(v)));
    for (int c = 2000; c; --c) {
      const bool y = r();
      const int i = dis(rand, dis_param_t(1, 133));
      const int j = dis(rand, dis_param_t(0, v0.size()));
      auto it = v0.insert(v0.end() - j, degraded_irng(rng(i, y)));
      auto it2 = v.insert(v.end() - j, degraded_irng(rng(i, y)));
      assert(*it == (int)*it2 && *it == (int)y);
      assert(equal(v0, rng_bind_to_int(v)));
    }
    v0.clear();
    v.clear();
    assert(equal(v0, rng_bind_to_int(v)));
    for (int c = 2000; c; --c) {
      const bool y = r();
      const int i = dis(rand, dis_param_t(1, 134));
      v0.append_range(rng(i, y));
      v.append_range(rng(i, y));
      assert(equal(v0, rng_bind_to_int(v)));
    }
  }

  // replace
  {
    using S = vector<bool>;
    S s = {1, 0, 0, 1, 0};
    s.replace(nth(s, 1), nth(s, 4), seq(true, false));
    assert(equal(s, S{1, 1, 0, 0}));
  }
}
void test_bool_vector_2() {
  stateful_test_allocator<bool> a1, a2;
  assert(a1 != a2);
  using V = vector<bool, stateful_test_allocator<bool>>;

  {
    V v(a1);
    assert(v.get_allocator() == a1);
  }
  {
    V v(3);
    assert(v.size() == 3);
  }
  {
    V v(3, a2);
    assert(v.size() == 3);
    assert(v.get_allocator() == a2);
  }
  {
    V v(3, true);
    assert(equal(v, rng(3, true)));
  }
  {
    V v(3, true, a2);
    assert(equal(v, rng(3, true)));
    assert(v.get_allocator() == a2);
    v.assign(4, false);
    assert(equal(v, rng(4, false)));
    assert(v.get_allocator() == a2);
  }
  {
    int a[] = {0, 1, 1};
    V v(begin(a), end(a));
    assert(equal(v, seq(false, true, true)));
    assert(v.get_allocator() != a2);
    V vv(begin(a), end(a), a2);
    assert(vv.get_allocator() == a2);
    v.assign(begin(a), begin(a));
    assert(v.empty());
  }
  {
    V v = {1, 1, 0, 1};
    assert(equal(v, seq(true, true, false, true)));
    V vv({true, false}, a2);
    assert(equal(vv, seq(true, false)));
    assert(vv.get_allocator() == a2);
    v.assign({true, true, true});
    assert(equal(v, rng(3, true)));
    V vvv(v, a1);
    assert(equal(v, vvv));
    assert(vvv.get_allocator() == a1);
    V vvvv(move(v), a1);
    assert(v.empty());
    assert(vvv == vvvv);
    assert(vvvv.get_allocator() == a1);
  }
  {
    V v;
    assert(*v.emplace(v.end(), 100) == true);
    assert(*v.insert(v.end(), true) == true);
    const bool y = false;
    assert(*v.insert(v.begin() + 1, y) == false);
    assert(equal(v, seq(true, false, true)));
    auto it = v.erase(v.end() - 1);
    assert(it == v.end());
    it = v.erase(v.begin(), v.end());
    assert(it == v.end());
    assert(v.empty());
    assert(v.capacity() == sizeof(long long) * 8);
    v.resize(1, false);
    v.resize(3, true);
    assert(equal(v, seq(false, true, true)));
    v.resize(8);
    assert(equal(seq(false, true, true), v.begin()));
    assert(v.size() == 8);
    v.resize(0);
    assert(v.empty());
    v.reserve(300);
    assert(v.capacity() >= 300);
    v.shrink_to_fit();
    assert(v.capacity() == 0);
    v.assign(300, true);
    assert(equal(v, rng(300, true)));
    v.flip();
    assert(equal(v, rng(300, false)));
    v.flip();
    assert(equal(v, rng(300, true)));
    v.reallocate(300);
    assert(equal(v, rng(300, true)));
    assert(v.capacity() < 500);
    v = rng(3, false);
    v.shrink_to_fit();
    assert(v.capacity() == sizeof(long long) * 8);
    assert(equal(v, rng(3, false)));
    v = V(v, a2);
    assert(equal(v, rng(3, false)));
    assert(v.get_allocator() == a2);
    v.assign_range(rng(4, true));
    assert(equal(v, rng(4, true)));
    v.assign(rng(0, false));
    assert(v.empty());
    it = v.insert(v.end(), rng(3, false));
    assert(it == v.begin());
    assert(equal(v, rng(3, false)));
    it = v.insert_range(v.end(), forward_list<bool>{true, true});
    assert(it == v.begin() + 3);
    assert(equal(v, seq(false, false, false, true, true)));
    v.push_back(rng(1, true));
    v.append_range(rng(0, true));
    assert(equal(v, seq(false, false, false, true, true, true)));
    v.clear();
    v.append(true, rng(1, false));
    assert(equal(v, seq(true, false)));
  }
  {
    vector<bool, stateful_test_allocator<bool>> v = {1, 0, 1, 0, 1};
    assert(v.remove(0) == 2);
    assert(equal(v, rng(3, true)));
    v = {1, 0, 1, 0, 1};
    assert(v.remove_if(bind(equal_to<bool>(), _1, 1)) == 3);
    assert(equal(v, rng(2, false)));
  }
  {
    V v;
    v.reserve_more(256u);
    assert(v.capacity() == 256u);
    const auto u = rng(3, typename V::value_type{});
    v = u;
    v.shrink_to_fit();
    v.reserve_more(256u);
    assert(equal(v, u) && v.capacity() == (256u + 64u));
  }
}

void test_vector_bool() {
  printf("container - vector_bool: ");

  inner::test::test_bool_vector();
  inner::test::test_bool_vector_2();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_vector_bool();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
