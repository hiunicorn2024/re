#include <re/test.h>
#include <re/range.h>
#include <re/algorithm.h>
#include <re/random.h>

#include <re/io.h>

#include <cassert>

using namespace re;

void test_random_engine_distribution() {
  const auto f = [](auto tag) {
    typename decltype(tag)::type r;
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

  test_random_engine_distribution();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_random();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
