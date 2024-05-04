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

using namespace re;

namespace local_vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r) const {
    ::new(p) V();
    p->append(r);
    assert(equal(*p, r));
  }  
};
constexpr fo_init init{};

struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r) const {
    return equal(*p, r);
  }
};
constexpr fo_good good{};

struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty();
  }
};
constexpr fo_is_empty is_empty{};

}
void test_local_vector_ownership() {
  for (int i : irng(0, 10))
    for (int j : irng(0, 10)) {
      using namespace local_vector_test0;
      using local_vector_test0::is_empty;
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        test_copy<vec_t>(bind(init, _1, irng(0, i)),
                         bind(good, _1, irng(0, i)),
                         bind(init, _1, irng(0, j)),
                         bind(good, _1, irng(0, j)));
        test_swap<vec_t>(bind(init, _1, irng(0, i)),
                         bind(good, _1, irng(0, i)),
                         bind(init, _1, irng(0, j)),
                         bind(good, _1, irng(0, j)));
        test_move<vec_t>(bind(init, _1, irng(0, i)),
                         bind(good, _1, irng(0, i)),
                         bind(init, _1, irng(0, j)),
                         bind(good, _1, irng(0, j)),
                         is_empty);
        test_ownership<vec_t>(bind(init, _1, irng(0, i)),
                              bind(good, _1, irng(0, i)),
                              bind(init, _1, irng(0, j)),
                              bind(good, _1, irng(0, j)),
                              is_empty);
      };
      testf(type_tag<local_vector<int, 20>>{});
      testf(type_tag<local_vector<test_object<int>, 20>>{});
    }
}

template <class V>
void test_local_vector_briefly() {
  // container
  // optional container operations
  // reversible container
  {
    V v;
    assert(v.begin() == v.end());
    assert(v.begin() == v.cbegin());
    assert(v.cbegin() == v.begin());
    assert(v.cbegin() == v.cbegin());
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.size() < v.max_size());

    v = {1, 2, 3};
    assert(v.begin() + 3 == v.end());
    assert(!v.empty());
    assert(v.size() == 3);
    test_equality(v, v);
    assert(equal(rrng(v), seq(3, 2, 1)));
    assert(equal(rrng(as_const(v)), seq(3, 2, 1)));

    V v2 = {1, 3, 3};
    test_lessness(v, v2);
  }

  // sequence container
  // optional sequence container operations
  {
    {
      V v(3);
      assert(v.size() == 3);
    }
    {
      V v(4, 4);
      assert(v.size() == 4 && all_of_equal(v, 4));

      v.assign(4, 5);
      assert(v.size() == 4 && all_of_equal(v, 5));
    }
    {
      const auto a = seq(1, 2, 3);
      const auto a2 = seq(4, 5);
      V v(begin(a), end(a));
      assert(equal(v, seq(1, 2, 3)));

      v.assign(begin(a2), end(a2));
      assert(equal(v, seq(4, 5)));
    }
    {
      V v = {1, 2, 3};
      assert(equal(v, seq(1, 2, 3)));

      v = {4, 5, 6};
      assert(equal(v, seq(4, 5, 6)));

      v.assign({7, 8, 9, 10});
      assert(equal(v, seq(7, 8, 9, 10)));
    }
    {
      V v;
      auto it = v.emplace(v.end(), 1);
      assert(it == v.begin());
      assert(v.size() == 1 && v.front() == 1);

      it = v.insert(v.begin(), 2);
      assert(it == v.begin());
      assert(equal(v, seq(2, 1)));

      const int tmp = 3;
      it = v.insert(v.begin() + 1, tmp);
      assert(it == v.begin() + 1);
      assert(equal(v, seq(2, 3, 1)));

      it = v.insert(v.begin(), 2, 4);
      assert(it == v.begin());
      assert(equal(v, seq(4, 4, 2, 3, 1)));

      const auto a = seq(1, 2, 3);
      it = v.insert(v.end(), begin(a), end(a));
      assert(equal(v, seq(4, 4, 2, 3, 1, 1, 2, 3)));

      it = v.insert(v.begin(), {4, 4});
      assert(equal(v, seq(4, 4, 4, 4, 2, 3, 1, 1, 2, 3)));

      it = v.erase(v.begin() + 4);
      assert(equal(v, seq(4, 4, 4, 4, 3, 1, 1, 2, 3)));
      it = v.erase(it);
      assert(equal(v, seq(4, 4, 4, 4, 1, 1, 2, 3)));
      it = v.erase(it, v.end() - 1);
      assert(equal(v, seq(4, 4, 4, 4, 3)));
    }
    {
      V v = {1, 2, 3};
      v.clear();
      assert(v.empty());
    }
    {
      V v;
#ifndef RE_NOEXCEPT
      assert(test_throwing<out_of_range>([&]() {v.at(1);}));
#endif
      v = {1, 2, 3};
      assert(v[0] == 1);
      assert(as_const(v)[1] == 2);
      assert(v.at(2) == 3);
#ifndef RE_NOEXCEPT
      assert(test_throwing<out_of_range>([&]() {v.at(4);}));
#endif

      v = {};
      v.push_back(1);
      assert(v.back() == 1);
      v.emplace_back(2);
      assert(v.back() == 2);
      v.pop_back();
      v.pop_back();
      assert(v.empty());
    }
  }

  // specialized operations
  {
    {
      V v;
      v.resize(3);
      assert(v.size() == 3);
      v.resize(0);
      assert(v.empty());
      v = {1, 2};
      v.resize(4, 4);
      assert(equal(v, seq(1, 2, 4, 4)));
      v.resize(1, 4);
      assert(equal(v, seq(1)));
    }
  }

  // operations for range
  {
    {
      V v = {from_range, seq(1, 2, 3)};
      assert(equal(v, seq(1, 2, 3)));
    }
    {
      V v(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v = seq(3, 2, 1);
      assert(equal(v, seq(3, 2, 1)));
      v.assign(seq(4, 5, 6, 7));
      assert(equal(v, irng(4, 8)));
      v.assign_range(rng(0, 0));
      assert(v.empty());
    }
    {
      V v = {1, 5};
      assert(equal(v, seq(1, 5)));
      auto it = v.insert(v.begin() + 1, seq(2, 3, 4));
      assert(it == v.begin() + 1);
      assert(equal(v, irng(1, 6)));
      it = v.insert_range(v.end(), irng(6, 8));
      assert(it == v.end() - 2);
      assert(equal(v, irng(1, 8)));
    }
    {
      V v;
      v.push_back(seq(1, 2, 3));
      assert(equal(v, seq(1, 2, 3)));
      v.append_range(seq(4));
      assert(equal(v, seq(1, 2, 3, 4)));
      v.pop_back(2);
      assert(equal(v, seq(1, 2)));
      v.pop_back(2);
      assert(v.empty());
    }
    {
      V v;
      const typename V::value_type x = 3;
      v.append(x);
      assert(equal(v, seq(3)));
      v.append(4);
      assert(equal(v, seq(3, 4)));
      v.append(rng(3, 9));
      assert(equal(v, seq(3, 4, 9, 9, 9)));

      v.clear();
      v.append(x, 4, seq(5, 6, 7), rng(3, 9));
      assert(equal(v, seq(3, 4, 5, 6, 7, 9, 9, 9)));
    }
  }

  // remove
  {
    V v = {1, 0, 2, 3, 0, 0, 4, 0};
    assert(v.remove(0) == 4);
    assert(equal(v, irng(1, 5)));
    assert(v.remove_if([](const auto &x) {return 2 <= x && x <= 3;}) == 2);
    assert(equal(v, seq(1, 4)));
  }

  // replace
  {
    using S = V;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}

void test_local_vector_construct_from_range() {
  for (int i : irng(0, 20)) {
    auto testf = [i](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i) && v1.capacity() == 20u);
      vec_t v2(irng(0, i));
      assert(v2.size() == to_unsigned(i) && v2.capacity() == 20u);
      assert(equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(v3.size() == to_unsigned(i) && v3.capacity() == 20u);
      assert(equal(v3, irng(0, i)));
    };
    testf(type_tag<local_vector<int, 20>>{});
    testf(type_tag<local_vector<test_object<int>, 20>>{});
  }
}
void test_local_vector_assign_range() {
  for (int i : irng(0, 20)) {
    auto testf = [=](auto tag) {
      using vec_t = typename decltype(tag)::type;
      vec_t v(rrng(irng(0, i)));
      assert(v.capacity() == 20);
      assert(equal(v, rrng(irng(0, i))));

      for (int j : irng(0, 20)) {
        vec_t v2(irng(0, j));
        assert(equal(v2, irng(0, j)));
        assert(v2.capacity() == 20);

        v = v2;
        assert(v == v2);

        vec_t z(rrng(irng(0, i)));
        assert(z.capacity() == 20);
        assert(equal(z, rrng(irng(0, i))));
        z = degraded_irng(v2);
        assert(z == v2);
      }
    };
    testf(type_tag<local_vector<int, 20>>{});
    testf(type_tag<local_vector<test_object<int>, 20>>{});
  }
}
void test_local_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            vec_t v(irng(0, i));
            assert(v.capacity() == 20 && equal(v, irng(0, i)));
            auto p0 = v.begin();
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(v.begin() == p0);
            assert(equal(rng(v.begin(), p), irng(0, j)));
          }
        for (int j : irng(0, i)) {
          vec_t v(irng(0, i));
          auto p0 = v.begin();
          auto p = v.erase(nth(v, j));
          assert(v.begin() == p0);
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
        }
      }
    }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_insert_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 19)) {
      vec_t v(irng(0, i));
      assert(v.emplace_back(i) == i);
      assert(equal(v, irng(0, i + 1)));
    }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_insert_1() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(0, i)) {
        vec_t v(irng(0, i));
        assert(v.capacity() == 20 && equal(v, irng(0, i)));
        const auto p = v.insert(nth(v, j), -1);
        assert(*p == -1);
        assert(equal(rng(v.begin(), p), irng(0, j)));
        assert(equal(rng(p + 1, v.end()), irng(j, i)));
      }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_insert_range_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 10))
      for (int j : irng(i, 10)) {
        {
          vec_t v(irng(0, i));
          vec_t v2(irng(0, j));
          assert(equal(v, irng(0, i)) && v.capacity() == 20);
          assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
          v.append_range(v2);
          assert(equal(irng(0, i), v.begin()));
          assert(equal(rng(v.begin() + i, v.end()), irng(0, j)));
          assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
        }
        {
          vec_t v(irng(0, i));
          vec_t v2(irng(0, j));
          assert(equal(v, irng(0, i)) && v.capacity() == 20);
          assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
          v.append_range(degraded_irng(v2));
          assert(equal(irng(0, i), v.begin()));
          assert(equal(rng(v.begin() + i, v.end()), irng(0, j)));
          assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
        }
      }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 10))
      for (int j : irng(i, 10))
        for (int k : irng(0, i + 1)) {
          {
            vec_t v(irng(0, i));
            vec_t v2(irng(0, j));
            assert(equal(v, irng(0, i)) && v.capacity() == 20);
            assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
            auto it = v.insert(nth(v, k), v2);
            assert(equal(rng(begin(v), it), irng(0, k)));
            assert(equal(rng(it, size(v2)), v2));
            assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
            assert(equal(rng(it + size(v2), end(v)), irng(k, i)));
          }
          {
            vec_t v(irng(0, i));
            vec_t v2(irng(0, j));
            assert(equal(v, irng(0, i)) && v.capacity() == 20);
            assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
            auto it = v.insert(nth(v, k), degraded_irng(v2));
            assert(equal(rng(begin(v), it), irng(0, k)));
            assert(equal(rng(it, size(v2)), v2));
            assert(equal(v2, irng(0, j)) && v2.capacity() == 20);
            assert(equal(rng(it + size(v2), end(v)), irng(k, i)));
          }
        }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_clear() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20)) {
      vec_t v(irng(0, i));
      assert(v.capacity() == 20 && equal(v, irng(0, i)));
      v.clear();
      assert(v.empty());
      assert(v.capacity() == 20);
    }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}
void test_local_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(0, 20)) {
        {
          vec_t v(irng(0, i));
          assert(v.capacity() == 20 && equal(v, irng(0, i)));
          v.resize(j);
          assert(v.size() == to_unsigned(j));
          if (j <= i)
            assert(equal(v, irng(0, j)));
          else
            assert(equal(irng(0, i), begin(v)));
        }
        {
          vec_t v(irng(0, i));
          assert(v.capacity() == 20 && equal(v, irng(0, i)));
          v.resize(j, -1);
          assert(v.size() == to_unsigned(j));
          if (j <= i)
            assert(equal(v, irng(0, j)));
          else {
            assert(equal(irng(0, i), begin(v)));
            assert(all_of_equal(rng(begin(v) + i, end(v)), -1));
          }
        }
      }
  };
  testf(type_tag<local_vector<int, 20>>{});
  testf(type_tag<local_vector<test_object<int>, 20>>{});
}

void test_local_vector() {
  printf("container - local_vector: ");

  test_local_vector_ownership();
  test_local_vector_briefly<local_vector<int, 20>>();
  test_local_vector_briefly<local_vector<test_object<int>, 20>>();
  test_local_vector_construct_from_range();
  test_local_vector_assign_range();
  test_local_vector_erase();
  test_local_vector_insert_1_back();
  test_local_vector_insert_1();
  test_local_vector_insert_range_back();
  test_local_vector_insert_range();
  test_local_vector_clear();
  test_local_vector_resize();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_local_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
