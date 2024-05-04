//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>
#include <re/io.h>

#include <cassert>

using namespace re;

void test_sscan_string() {
  {
    string s = "12345_&#++234_+1_2_567";
    sview sv = s;
    assert(sscan(sv, "12345"));
    assert(sv == "_&#++234_+1_2_567");
    unsigned x;
    assert(sscan(sv, "_&#"));
    assert(sv == "++234_+1_2_567");
    assert(!sscan(sv, x));
    assert(sv == "++234_+1_2_567");
    assert(sscan(sv, "+"));
    assert(sv == "+234_+1_2_567");
    assert(sscan(sv, x) && x == 234);
    assert(sv == "_+1_2_567");
    assert(sscan(sv, "_"));
    assert(sscan(sv, x));
    assert(x == 1);
    assert(sv == "_2_567");
    assert(!sscan(sv, x));
    assert(sscan(sv, "_"));
    assert(sscan(sv, x) && x == 2);
    assert(sscan(sv, "_"));
    assert(sscan(sv, x) && x == 567);
    assert(empty(sv));
  }
  {
    string s = "-12345_&#-+-234_-1_2_567";
    sref sv = s;
    int x;
    assert(sscan(sv, x) && x == -12345);
    assert(sscan(sv, "_&#"));
    assert(!sscan(sv, x));
    assert(sscan(sv, "-"));
    assert(!sscan(sv, x));
    assert(sscan(sv, "+"));
    assert(sscan(sv, x) && x == -234);
    assert(sscan(sv,  "_"));
    assert(sscan(sv, x) && x == -1);
    assert(sscan(sv,  "_"));
    assert(sscan(sv, x) && x == 2);
    assert(sscan(sv,  "_"));
    assert(sscan(sv, x) && x == 567);
    assert(empty(sv));
  }
}
void test_sscan_integral() {
  {
    sview v = "255";
    unsigned char c;
    assert(sscan(v, c) && c == 255 && empty(v));
    v = "256";
    assert(!sscan(v, c) && c == 255 && v == "256");
    v = "0";
    assert(sscan(v, c) && c == 0 && empty(v));
    v = "000";
    assert(sscan(v, c) && c == 0 && empty(v));
    v = "+123";
    assert(sscan(v, c) && c == 123 && empty(v));
    v = "++123";
    assert(!sscan(v, c) && v == "++123");
    v = "-123";
    assert(!sscan(v, c) && v == "-123");
    v = "--123";
    assert(!sscan(v, c) && v == "--123");
  }
  {
    sview v = "127";
    signed char c;
    assert(sscan(v, c) && c == 127 && empty(v));
    v = "128";
    assert(!sscan(v, c) && c == 127 && v == "128");
    v = "0";
    assert(sscan(v, c) && c == 0 && empty(v));
    v = "000";
    assert(sscan(v, c) && c == 0 && empty(v));
    v = "+123";
    assert(sscan(v, c) && c == 123 && empty(v));
    v = "++123";
    assert(!sscan(v, c) && v == "++123");
    {
      sview z = "-0000000000128";
      signed char cc;
      assert(sscan(z, cc) && cc == -128 && empty(z));
      z = "-129";
      assert(!sscan(z, cc) && cc == -128 && z == "-129");
      z = "-0";
      assert(sscan(z, cc) && cc == 0 && empty(z));
      z = "-000";
      assert(sscan(z, cc) && cc == 0 && empty(z));
      z = "-00123";
      assert(sscan(z, cc) && cc == -123 && empty(z));
      z = "-123456";
      assert(!sscan(z, cc) && cc == -123 && z == "-123456");
    }
  }

  {
    uint16_t x;
    sview v = "1111000010100000";
    assert(sscan_bin(v, x));
    assert(empty(v));
    assert(x == 0b1111000010100000);

    v = "11110000101000000";
    assert(!sscan_bin(v, x));
    assert(v == "11110000101000000");
    assert(x == 0b1111000010100000);
  }
  {
    int32_t x;
    sview v = "7BcDeFfF";
    assert(sscan_hex(v, x));
    assert(empty(v));
    assert(x == 0x7bcdefff);
  }
}
void test_sprint_string() {
  string s;
  sprint(s, "");
  assert(empty(s));
  sprint(s, "abc");
  assert(s == "abc");
  sprint(s, "def", print_args{}.min_wid(5));
  assert(s == "abc  def");
  s.clear();
  sprint(s, "abc"_sv, print_args{}.min_wid(5).pad('$').left_align());
  assert(s == "abc$$");
  s.clear();
  sprint(s, "abc"_sv, print_args{}.min_wid(4).right_align());
  assert(s == " abc");
}
void test_sprint_integral() {
  /*
  {
    int64_t x = 0xabcd;
    auto m = mref::from_object(x);
    string s, ss;
    sprint(s, mref::from_object(x));
    for (unsigned char c : m) {
      sprint(ss, c, int_print_args{}.bin().min_wid(8).pad('0'));
      ss.push_back(' ');
    }
    ss.pop_back();
    assert(s == ss);
  }
  */
  {
    int i = 123;
    string s;
    sprint(s, i);
    assert(s == "123");
    sprint(s, i, int_print_args{}.min_wid(8));
    assert(s == "123     123");
    sprint(s, i, int_print_args{}.min_wid(8).pad('0'));
    assert(s == "123     12300000123");
    s.clear();
    sprint(s, i, int_print_args{}.min_wid(3));
    assert(s == "123");
    s.clear();
    sprint(s, i, int_print_args{}.min_wid(0));
    assert(s == "123");
    s.clear();
    sprint(s, i, int_print_args{}.min_wid(5).left_align());
    assert(s == "123  ");
    s.clear();
    sprint(s, i, int_print_args{}.min_wid(5).right_align());
    assert(s == "  123");
    s.clear();
  }
  {
    string s;
    uint64_t i = 0xfffffffffffffffe;
    sprint(s, i, int_print_args{}.hex());
    assert(s == "fffffffffffffffe");
    s.clear();
    sprint(s, i, int_print_args{}.HEX());
    assert(s == "FFFFFFFFFFFFFFFE");
    s.clear();
    sprint(s, i, int_print_args{}.bin());
    assert(s == "11111111111111111111111111111111"
           "11111111111111111111111111111110");
    s.clear();
    sprint(s, i, int_print_args{}.hex().show_plus());
    assert(s == "+fffffffffffffffe");
    s.clear();
    sprint(s, 01234567, int_print_args{}.oct().hide_plus());
    assert(s == "1234567");
    s.clear();
    sprint(s, 0);
    assert(s == "0");
  }
}
void test_ssplitter() {
  {
    ssplitter<> fmt;
    assert(fmt.empty());
  }
  {
    ssplitter<> fmt("%", '%');
    assert(!fmt.empty());
    fmt.clear();
    assert(fmt.empty());
    fmt = {"%", '%'};
  }
  const auto test_f = [](auto tag) {
    using view_t = typename decltype(tag)::type;

    ssplitter<view_t> fmt("abc % def % ghi", '%');
    assert(!fmt.empty());

    string s;
    fmt(seq("x"_sv, "y"), s);
    assert(equal(s, "abc x def y ghi"_sv));
    s.clear();
    fmt(begin(seq("x"_sv, "y")), s);
    assert(equal(s, "abc x def y ghi"_sv));

    s.clear();
    fmt("%%%%", s);
    assert(equal(s, "abc %%%% def %%%% ghi"_sv));
  };
  test_f(type_tag<sview>());
  test_f(type_tag<string>());
  {
    string s;
    ssplitter<>("abcdcdcdc", "cdc")("%%%%%%", to_back(s));
    assert(s == "ab%%%%%%d%%%%%%");
    s.clear();
    ssplitter<>("abcdcdcdc", "cdc")("", to_back(s));
    assert(s == "abd");
  }
}
void test_ssplit() {
  const auto test_f = [](auto tag) {
    using str_t = typename decltype(tag)::type;
    {
      str_t s = "";
      str_t ss = ssplit(s, "");
      assert(empty(ss) && empty(s));
    }
    {
      str_t s = "abc";
      str_t ss = ssplit(s, "");
      assert(ss == "abc" && empty(s));
    }
    {
      str_t s = "abc";
      str_t ss = ssplit(s, "abc");
      assert(empty(ss) && empty(s));
    }
    {
      str_t s = "abcdefg";
      str_t ss = ssplit(s, "cd");
      assert(ss == "ab" && s == "efg");
      ss = ssplit(s, "cd");
      assert(ss == "efg" && s == "");
      ss = ssplit(s, "cd");
      assert(empty(ss) && empty(s));
    }
    {
      str_t s = "ab__cd__efg";
      vector<str_t> v;
      while (!empty(s))
        v.append(ssplit(s, "__"));
      assert(equal(v, seq("ab"_sv, "cd", "efg")));
    }
    {
      str_t s = "ab__cd__efg__";
      vector<str_t> v;
      while (!empty(s))
        v.append(ssplit(s, "__"));
      assert(equal(v, seq("ab"_sv, "cd", "efg")));
    }
    {
      str_t s = "ab__cd__efg";
      vector<str_t> v;
      while (!empty(s))
        v.append(ssplit(s, "_"));
      assert(equal(v, seq("ab"_sv, "", "cd", "", "efg")));
    }
  };
  test_f(type_tag<string>{});
  test_f(type_tag<sview>{});
}

void test_io() {
  printf("io: ");

  test_sscan_string();
  test_sscan_integral();
  test_sprint_string();
  test_sprint_integral();
  test_ssplitter();
  test_ssplit();

  printf("ok\n");
}

int main() {
#ifndef RE_NOEXCEPT
  try {
#endif
    test_io();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
