#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>
#include <re/random.h>

#include <cassert>

namespace re::inner::fns {

void test_random_engine_distribution() {
  const auto f = [](auto tag) {
    {
      using r_t = typename decltype(tag)::type;
      using uint_t = typename r_t::result_type;
      using d_t = uniform_int_distribution<uint_t>;
      r_t r1, r2;
      d_t d(r_t::min(), r_t::max());
      ez_vector<uint_t> v1, v2;
      for (int c = 1000; c != 0; --c) {
        v1.insert(v1.end(), d(r1));
        v2.insert(v2.end(), r2());
      }
      assert(v1 == v2);
    }

    using r_t = typename decltype(tag)::type;
    r_t r;
    using d_t = uniform_int_distribution<int>;
    {
      int a[10] = {};
      for (int c = 10000; c != 0; --c) {
        const int x = d_t(0, 9)(r);
        ++a[x];
      }
      for (auto &x : a)
        assert(x >= 870 && x <= 1130);
    }
    {
      int a[234] = {};
      for (int c = 234 * 1000; c != 0; --c) {
        const int x = d_t(0, 233)(r);
        ++a[x];
      }
      for (auto &x : a)
        assert(x >= 870 && x <= 1130);
    }
    {
      int a = 0, b = 0;
      for (int c = 10000; c != 0; --c)
        ++(rational_bernoulli_distribution<int>(1, 2)(r) ? a : b);
      assert(abs(a - b) <= 500);
    }
    {
      int a = 0, b = 0;
      for (int c = 10000; c != 0; --c)
        ++(rational_bernoulli_distribution<int>(66, 100)(r) ? a : b);
      assert(6600 - 500 <= a && a <= 6600 + 500);
      assert(3400 - 500 <= b && b <= 3400 + 500);
    }
  };
  f(type_tag<minstd_rand0>{});
  f(type_tag<minstd_rand>{});
  f(type_tag<mt19937>{});
  f(type_tag<mt19937_64>{});
}

void test_random() {
  printf("random: ");

  inner::fns::test_random_engine_distribution();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_random();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
