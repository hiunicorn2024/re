#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>

#include <cassert>

using namespace re;

void test_bitset_impl() {
  {
    bitset<0> s;
    assert(s.empty());
    assert(s.size() == 0);
    assert(s.begin() == s.end());
    assert(as_const(s).begin() == as_const(s).end());
    assert((s <=> s) == 0);
    assert(s == s);
  }
  {
    auto testf = [](auto tag) {
      using S = typename decltype(tag)::type;
      S s;
      array<bool, s.size()> a;
      generate(s, bind(rational_bernoulli_distribution(6, 10), minstd_rand{}));
      generate(a, bind(rational_bernoulli_distribution(6, 10), minstd_rand{}));
      assert(equal(a, s));
      assert(equal(a, as_const(s)));

      assert(s == s);
      auto ss = s;
      ss[ss.size() - 1] = !ss[ss.size() - 1];
      assert(s != ss);
      ss = s;
      ss[0] = false;
      assert(s != ss);
      ss = s;
      assert(s == ss);
      ss[ss.size() / 2].flip();
      assert(s != ss);
      ss[ss.size() / 2].flip();
      assert(s == ss);
    };
    type_pack_for_each<type_pack<bitset<1>, bitset<2>,
                                 bitset<4>, bitset<5>,
                                 bitset<8>, bitset<9>,
                                 bitset<16>, bitset<19>,
                                 bitset<32>, bitset<33>,
                                 bitset<64>, bitset<65>,
                                 bitset<128>, bitset<129>,
                                 bitset<192>, bitset<193>,
                                 bitset<256>, bitset<257>>
                       >(testf);
  }
  {
    string s;
    bitset<5> x;
    x[0] = true;
    x[3] = true;
    copy(bind_rng(x, [](bool y)->char {return '0' + y;}), back_inserter(s));
    assert(s == "10010");
    s.clear();
    x.print(back_inserter(s));
    assert(s == "10010");
    s = x.to<string>();
    assert(s == "01001");

    bitset<133> xx("1000000000100000000000000000000000000000000000000"
                   "100000000000001");
    assert(xx.to<string>()
           == "0000000000000000000000000000000000000000000000000000000000000000"
           "0000010000000001000000000000000000000000000000000000001000000000000"
           "01"_s);

    bitset<133> xxx(10000);
    assert(xxx.to_ulong() == 10000);
    assert(xxx.to_ullong() == 10000);
    {
      bitset<7> z("1001000");
      assert(z.to<string>() == string("1001000"));
    }
  }
  {
    bitset<133> x, y;
    x.set();
    assert(x.count() == 133);
    for (int i : irng(0, 133))
      if (i % 2 == 0)
        x.reset(i);
    assert(x.count() == 66);
    x.set();
    assert(x.count() == 133);
    for (auto xx : y)
      xx = true;
    assert(x == y);
    for (auto xx : y)
      xx = false;
    assert(x == ~y);
  }
  {
    bitset<5> x("10001");
    assert(!x.none());
    assert(x.any());
    assert(!x.all());
    bitset<255> y;
    y.set();
    assert(all_of_equal(y, true));
    assert(y.all());
    assert(!y.none());
    assert(y.any());
  }
  {
    bitset<7> s;
    s.set();
    s <<= 3;
    assert(s.to<string>() == "1111000");
    s <<= 5;
    assert(s.none());
    s = 0b11111000;
    assert(s.to<string>() == "1111000");
    s <<= 1;
    assert(s.to<string>() == "1110000");
    minstd_rand r;
    auto testf = [&](auto tag) {
      using S = typename decltype(tag)::type;
      S s;
      generate(s, bind(rational_bernoulli_distribution(5, 10), ref(r)));
      for (int i : irng(0, s.size())) {
        auto tmp = s;
        s <<= i;
        assert(equal(rng(s.begin() + i, s.end()), tmp.begin()));
      }
    };
    type_pack_for_each
      <type_pack
       <bitset<5>, bitset<11>, bitset<13>, bitset<33>, bitset<66>,
        bitset<69>, bitset<145>, bitset<167>, bitset<244>, bitset<255>
        >>(testf);
  }
  {
    bitset<7> s = 0b100011;
    s >>= 4;
    assert(s == 0b000010ull);
    s >>= 1;
    assert(s == 0b000001ull);
    minstd_rand r;
    auto testf = [&](auto tag) {
      using S = typename decltype(tag)::type;
      S s;
      generate(s, bind(rational_bernoulli_distribution(5, 10), ref(r)));
      for (int i : irng(1, s.size())) {
        auto tmp = s;
        s >>= i;
        assert(equal(rng(s.begin(), s.end() - i), tmp.begin() + i));
      }
    };
    type_pack_for_each
      <type_pack
       <bitset<5>, bitset<11>, bitset<13>, bitset<33>,
        bitset<66>, bitset<69>, bitset<145>, bitset<167>,
        bitset<244>, bitset<255>
        >>(testf);
  }
  {
    bitset<5> s;
    decltype(auto) r1 = s[0];
    decltype(auto) r2 = s[3];
    r1 = true;
    r2 = false;
    swap(r1, r2);
    assert(r1 == false && r2 == true);
  }
  {
    bitset<3> x;
    auto it = x.begin();
    *it = true;
    assert(x[0] == true);
    decltype(x)::const_iterator it2 = it;
    *it = false;
    assert(*it2 == false);
    static_assert(!is_convertible_v
                  <decltype(x)::const_iterator, decltype(x)::iterator>);
  }
}

void test_bitset() {
  printf("bitset: ");

  test_bitset_impl();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_bitset();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
