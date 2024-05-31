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

void test_c_file() {
  const string filename_f = string("./").append(__FILE__, "_f");
  const string filename_f1 = string("./").append(__FILE__, "_f1");
  const string filename_f2 = string("./").append(__FILE__, "_f2");
  const char *nf = filename_f.data();
  const char *nf1 = filename_f1.data();
  const char *nf2 = filename_f2.data();

  re::remove(nf);
  re::remove(nf1);
  re::remove(nf2);

  // special member functions
  // ==
  // c_file(name, mode = ...)
  // open(name, mode = ...)
  // empty()
  // error()
  // close()
  {
    static_assert(is_nothrow_default_constructible_v<c_file>);
    static_assert(movable<c_file>);
    static_assert(swappable<c_file>);

    {
      c_file f(nf1);
      c_file f2(nf2);
      test_inequality(f, nullptr);
      assert(!f.empty() && !f2.empty());
      assert(!f.error() && !f2.error());
      adl_swap(f, f2);
      f.put("abc");
      f2.put("def");
      f.close();
      f2.close();
      assert(f.empty() && f2.empty());

      f.open(nf1);
      f2.open(nf2);
      const string s1 = f.get();
      const string s2 = f2.get();
      assert(s1 == "def");
      assert(s2 == "abc");
      f.close();
      f2.close();
      re::remove(nf1);
      re::remove(nf2);
    }
    {
      c_file f;
      c_file f2(nf2);
      adl_swap(f, f2);
      adl_swap(f, f2);
      f2.put("xyz");
      f2.close();
      f2.open(nf2);
      assert(f2.get() == "xyz");
      f2.close();
      re::remove(nf2);
    }
  }

  // base()
  // release()
  // c_file(FILE *)
  {
    c_file f(nf);
    assert(!f.empty());
    FILE *p1 = f.base();
    assert(!f.empty());
    FILE *p2 = f.release();
    assert(f.release() == nullptr);
    assert(f.empty());
    assert(p1 != nullptr && p1 == p2);
    f = c_file(p1);
    f.put("ab");
    f.close();
    f.open(nf);
    const string s = f.get();
    assert(s == "ab");
    f.close();
    re::remove(nf);
  }

  // file_exists
  // remove(const char *fname)
  // rename(const char *fname, const char *new_name)
  {
    assert(!file_exists(nf));
    c_file f(nf);
    f.close();
    assert(file_exists(nf));
    rename(nf, string(__FILE__).append("_f2").data());
    assert(!file_exists(nf));
    assert(file_exists(string("./").append(__FILE__, "_f2").data()));
    re::remove(string(__FILE__).append("_f2").data());

    f.open(nf);
    assert(file_exists(nf));
    f.close();
    re::remove(nf);
  }

  // putc(c)
  // flush()
  // getc()
  {
    c_file f(nf);
    f.putc('a');
    f.putc('b');
    f.putc('c');
    f.flush();
    f.close();
    f.open(nf);
    string s;
    for (int c; (c = f.getc()) != EOF;)
      s.push_back(c);
    assert(s == "abc");
    f.close();
    re::remove(nf);
  }

  // put(const char *)
  // put(s_range)
  // get()->string
  {
    c_file f(nf);
    f.put("abc");
    f.put("def"_sv);
    f.put(list<char>{'g', 'h', 'i'});
    f.close();
    f.open(nf);
    string s = f.get();
    assert(s == "abcdefghi");
    f.close();
    re::remove(nf);
  }
}
void test_console_c_file() {
  // special member functions
  // stdin_f()
  // stdout_f()
  // base()
  {
    static_assert(!is_default_constructible_v<console_c_file>);
    static_assert(is_destructible_v<console_c_file>);
    static_assert(!is_move_constructible_v<console_c_file>);
    static_assert(!is_move_assignable_v<console_c_file>);
    static_assert(!is_copy_constructible_v<console_c_file>);
    static_assert(!is_copy_assignable_v<console_c_file>);

    static_assert(is_same_v<decltype(stdin_f()), console_c_file>);
    static_assert(is_same_v<decltype(stdout_f()), console_c_file>);
    static_assert(is_same_v<decltype(stderr_f()), console_c_file>);
    assert(stdin_f().base() == stdin);
    assert(stdout_f().base() == stdout);
    assert(stderr_f().base() == stderr);
    assert(!stdin_f().error());
    assert(!stdout_f().error());
    assert(!stderr_f().error());
  }
}
void test_fputc_iterator() {
  static_assert(is_oitr<fputc_iterator>);
  static_assert(output_iterator<fputc_iterator, int>);

  const string filename_f = string("./").append(__FILE__, "_f");
  const char *nf = filename_f.data();
  re::remove(nf);
  c_file f(nf);
  assert(f.putc_iter().base() == f.base());
  copy("abcd"_sv, f.putc_iter());
  f.close();
  f.open(nf);
  string s = f.get();
  assert(s == "abcd");
  f.close();
  re::remove(nf);
}
void test_sscan() {
  // scan string
  {
    sview v0;
    sview v = "abcdefg";

    assert(!sscan(v, "Abcd") && v == "abcdefg");
    assert(!sscan(v, "aBcd") && v == "abcdefg");
    assert(!sscan(v, "abCd") && v == "abcdefg");
    assert(!sscan(v, "abcD") && v == "abcdefg");

    assert(v0.empty() && !sscan(v0, ""));

    assert(!sscan(v, ""));
    assert(v == "abcdefg");

    assert(sscan(v, "abc"));
    assert(v == "defg");

    assert(!sscan(v, "defgh"));
    assert(v == "defg");

    assert(sscan(v, "defg"));
    assert(v == "");
  }
  // scan integral
  {
    const auto test_sscan_success = [](auto type_tag, sview v, auto cmp,
                                       int n = 0) {
      using t = typename decltype(type_tag)::type;
      {
        t x = 0;
        const auto v_op = v.begin();
        const auto v_ed = v.end();
        const bool y = sscan(v, x);
        if (!y)
          return false;
        const bool y2 = (v.begin() == (n == 0 ? v_ed : v_op + n)
                         && x == cmp);
        if (!y2)
          return false;
        v = {v_op, v_ed};
      }
      {
        t x = 0;
        const auto v_op = v.begin();
        const auto v_ed = v.end();
        const bool y = sscan(v, x, dec);
        if (!y)
          return false;
        const bool y2 = (v.begin() == (n == 0 ? v_ed : v_op + n)
                         && x == cmp);
        if (!y2)
          return false;
      }
      return true;
    };
    const auto test_sscan_failure = [](auto type_tag, sview v) {
      using t = typename decltype(type_tag)::type;
      t x = 0;
      const auto vv = v;
      const bool y = !sscan(v, x);
      if (!y)
        return false;
      const bool y2 = (v == vv && x == 0);
      return y2;
    };

    // scan signed integral
    {
      assert(test_sscan_success(type_tag<int>{}, "0", 0));
      assert(test_sscan_success(type_tag<int>{}, "00", 0));
      assert(test_sscan_success(type_tag<int>{}, "000", 0));
      assert(test_sscan_success(type_tag<int>{}, "0000", 0));
      assert(test_sscan_success(type_tag<int>{}, "00000", 0));
      assert(test_sscan_success(type_tag<int>{}, "+00000", 0));
      assert(test_sscan_success(type_tag<int>{}, "-00000", 0));
      assert(test_sscan_failure(type_tag<int>{}, "++00000"));
      assert(test_sscan_failure(type_tag<int>{}, "--00000"));
      assert(test_sscan_success(type_tag<int>{}, "012", 12));
      assert(test_sscan_success(type_tag<int>{}, "12", 12));
      assert(test_sscan_failure(type_tag<int>{}, "++012"));
      assert(test_sscan_failure(type_tag<int>{}, "++12"));
      assert(test_sscan_success(type_tag<int>{}, "+012", 12));
      assert(test_sscan_success(type_tag<int>{}, "+0012", 12));
      assert(test_sscan_success(type_tag<int>{}, "+12", 12));
      assert(test_sscan_success(type_tag<int>{}, "+1234", 1234));
      assert(test_sscan_success(type_tag<int>{}, "1234", 1234));
      assert(test_sscan_failure(type_tag<int>{}, "---12"));
      assert(test_sscan_failure(type_tag<int>{}, "--12"));
      assert(test_sscan_success(type_tag<int>{}, "-012", -12));
      assert(test_sscan_success(type_tag<int>{}, "-0012", -12));
      assert(test_sscan_success(type_tag<int>{}, "-12", -12));

      assert(test_sscan_success(type_tag<int>{}, "1'2'3'4", 1234));
      assert(test_sscan_success(type_tag<int>{}, "+1'2'3'4", 1234));
      assert(test_sscan_success(type_tag<int>{}, "1'234", 1234));
      assert(test_sscan_success(type_tag<int>{}, "+1'234", 1234));
      assert(test_sscan_success(type_tag<int>{}, "12'34", 1234));
      assert(test_sscan_success(type_tag<int>{}, "+12'34", 1234));
      assert(test_sscan_success(type_tag<int>{}, "123'4", 1234));
      assert(test_sscan_success(type_tag<int>{}, "+123'4", 1234));
      assert(test_sscan_success(type_tag<int>{}, "1234'", 1234, 4));
      assert(test_sscan_success(type_tag<int>{}, "+1234'", 1234, 5));
      assert(test_sscan_failure(type_tag<int>{}, "'1234"));
      assert(test_sscan_failure(type_tag<int>{}, "+'1234"));
      assert(test_sscan_success(type_tag<int>{}, "-1234", -1234));
      assert(test_sscan_success(type_tag<int>{}, "-1'2'3'4", -1234));
      assert(test_sscan_success(type_tag<int>{}, "-1''2''3'4", -1, 2));
      assert(test_sscan_success(type_tag<int>{}, "-1'234", -1234));
      assert(test_sscan_success(type_tag<int>{}, "-12'34", -1234));
      assert(test_sscan_success(type_tag<int>{}, "-123'4", -1234));
      assert(test_sscan_success(type_tag<int>{}, "-1234'", -1234, 5));
      assert(test_sscan_failure(type_tag<int>{}, "-'1234"));

      assert(test_sscan_success(type_tag<int64_t>{}, "1234567890", 1234567890));
      assert(test_sscan_failure(type_tag<int64_t>{}, "-9223372036854775809"));
      assert(test_sscan_failure(type_tag<int64_t>{}, "9223372036854775808"));
      assert(test_sscan_failure(type_tag<int64_t>{}, "+9223372036854775808"));
      assert(test_sscan_success(type_tag<int64_t>{}, "-9223372036854775808",
                                -9223372036854775807 - 1));
      assert(test_sscan_success(type_tag<int64_t>{}, "9223372036854775807",
                                9223372036854775807));

      assert(test_sscan_failure(type_tag<int8_t>{}, "-1000"));
      assert(test_sscan_failure(type_tag<int8_t>{}, "-129"));
      assert(test_sscan_failure(type_tag<int8_t>{}, "128"));
      assert(test_sscan_failure(type_tag<int8_t>{}, "+128"));
      assert(test_sscan_failure(type_tag<int8_t>{}, "1000"));
      assert(test_sscan_failure(type_tag<int8_t>{}, "+1000"));
      assert(test_sscan_success(type_tag<int8_t>{}, "127", 127));
      assert(test_sscan_success(type_tag<int8_t>{}, "-128", -128));

      int8_t x = 0;
      assert(!sscan(as_lvalue("10000000"_sv), x, bin));
      assert(!sscan(as_lvalue("-10000001"_sv), x, bin));
      sview v = "11011";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == 0b11011);
      v = "1111111";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == 0b1111111);
      v = "-10000000";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == -0b1111111 - 1);

      int16_t y = 0;
      assert(!sscan(as_lvalue("100000"_sv), y, oct));
      assert(!sscan(as_lvalue("-100001"_sv), y, oct));
      v = "123";
      assert(sscan(v, y, oct) && v.empty() && y == 0123);
      v = "-7654";
      assert(sscan(v, y, oct) && v.empty() && y == -07654);
      v = "100";
      assert(sscan(v, y, oct) && v.empty() && y == 0100);
      v = "77777";
      assert(sscan(v, y, oct));
      assert(v.empty() && y == 077777);
      v = "-100000";
      assert(sscan(v, y, oct));
      assert(v.empty() && y == -0100000);

      int32_t z = 0;
      assert(!sscan(as_lvalue("80000000"_sv), z, hex));
      assert(!sscan(as_lvalue("-80000001"_sv), z, hex));
      v = "7fffffff";
      assert(sscan(v, z, hex));
      assert(v.empty() && z == 0x7fffffff);
      v = "-80000000";
      assert(sscan(v, z, hex));
      assert(v.empty() && z == -0x7fffffff - 1);

      int64_t zz = 0;
      v = "7FFFFFFFFFFFFFFF";
      assert(sscan(v, zz, hex));
      assert(v == "" && zz == 0x7FFFFFFFFFFFFFFF);
      v = "7FFFFFFFFFFFFFFF";
      assert(sscan(v, zz, hex));
      assert(v.empty() && zz == 0x7fffffffffffffff);
      v = "-8000000000000000";
      assert(sscan(v, zz, hex));
      assert(v.empty() && zz == -0x7fffffffffffffff - 1);
      v = "1234CdeF";
      assert(sscan(v, zz, hex));
      assert(v.empty());
      assert(zz == 0x1234cdef);
    }
    // scan unsigned integral
    {
      assert(test_sscan_success(type_tag<unsigned>{}, "012", 12u));
      assert(test_sscan_success(type_tag<unsigned>{}, "12", 12u));
      assert(test_sscan_failure(type_tag<unsigned>{}, "++012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "++12"));
      assert(test_sscan_success(type_tag<unsigned>{}, "+012", 12u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+0012", 12u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+12", 12u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+1234", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "1234", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "-000", 0u));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-001"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "---12"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "--12"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-12"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "---012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "--012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "---0012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "--0012"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-0012"));

      assert(test_sscan_success(type_tag<unsigned>{}, "1'2'3'4", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+1'2'3'4", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "1'234", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+1'234", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "12'34", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+12'34", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "123'4", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "+123'4", 1234u));
      assert(test_sscan_success(type_tag<unsigned>{}, "1234'", 1234u, 4));
      assert(test_sscan_success(type_tag<unsigned>{}, "+1234'", 1234u, 5));
      assert(test_sscan_failure(type_tag<unsigned>{}, "'1234"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "+'1234"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-1234"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-1'2'3'4"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-1'234"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-12'34"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-123'4"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-1234'"));
      assert(test_sscan_failure(type_tag<unsigned>{}, "-'1234"));

      assert(test_sscan_failure(type_tag<uint64_t>{}, "99999999999999999999"));
      assert(test_sscan_failure(type_tag<uint64_t>{}, "18446744073709551616"));
      assert(test_sscan_failure(type_tag<uint64_t>{}, "-1"));
      assert(test_sscan_success(type_tag<uint64_t>{}, "18446744073709551615",
                                18446744073709551615u));

      assert(test_sscan_failure(type_tag<uint8_t>{}, "999"));
      assert(test_sscan_failure(type_tag<uint8_t>{}, "256"));
      assert(test_sscan_failure(type_tag<uint8_t>{}, "-1"));
      assert(test_sscan_success(type_tag<uint8_t>{}, "+0", 0u));
      assert(test_sscan_success(type_tag<uint8_t>{}, "-0", 0u));
      assert(test_sscan_success(type_tag<uint8_t>{}, "255", 255u));
      assert(test_sscan_success(type_tag<uint8_t>{}, "23", 23u));

      uint8_t x = 0;
      assert(!sscan(as_lvalue("100000000"_sv), x, bin));
      assert(!sscan(as_lvalue("-1"_sv), x, bin));
      sview v = "11011";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == 0b11011u);
      v = "11111111";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == 0b11111111u);
      v = "0";
      assert(sscan(v, x, bin));
      assert(v.empty() && x == 0u);

      uint16_t y = 0;
      assert(!sscan(as_lvalue("200000"_sv), y, oct));
      assert(!sscan(as_lvalue("-1"_sv), y, oct));
      v = "177777";
      assert(sscan(v, y, oct));
      assert(v.empty() && y == 0177777u);
      v = "0";
      assert(sscan(v, y, oct));
      assert(v.empty() && y == 0u);

      uint32_t z = 0;
      assert(!sscan(as_lvalue("1'0000'0000"_sv), z, hex));
      assert(!sscan(as_lvalue("-1"_sv), z, hex));
      v = "ffff'ffff";
      assert(sscan(v, z, hex));
      assert(v.empty() && z == 0xffff'ffffu);
      v = "0";
      assert(sscan(v, z, hex));
      assert(v.empty() && z == 0u);

      uint64_t zz = 0;
      v = "FFFF'FFFF'FFFF'FFFF";
      assert(sscan(v, zz, hex));
      assert(v.empty() && zz == 0xFFFF'FFFF'FFFF'FFFFu);
      v = "0";
      assert(sscan(v, zz, hex));
      assert(v.empty() && zz == 0u);
    }
  }
}
void test_sprint() {
  // for string
  {
    string s;
    sprint(s, "");
    assert(s.empty());
    sprint(s, "abc");
    assert(s == "abc");
    sprint(s, "abc", setw(3));
    assert(s == "abcabc");
    sprint(s, "abc", setw(6));
    assert(s == "abcabc   abc");
    s.clear();
    sprint(s, "abc", setw(3), left);
    assert(s == "abc");
    s.clear();
    sprint(s, "abc", setw(3), right);
    assert(s == "abc");
    s.clear();
    sprint(s, "abc", setw(6), left);
    assert(s == "   abc");
    s.clear();
    sprint(s, "abc", setw(6), right);
    assert(s == "abc   ");
    s.clear();
    sprint(s, "abc", setw(6), right, setfill('0'));
    assert(s == "abc000");
    s.clear();
    sprint(s, "", setw(6));
    assert(s == "      ");
    s.clear();
    sprint(s, "abc", setw(1));
    assert(s == "abc");
  }
  // for int
  {
    // the simplest usage
    {
      string s;
      sprint(s, 123);
      assert(s == "123");
      s.clear();
      sprint(s, 123, dec);
      assert(s == "123");
      s.clear();
      sprint(s, 0b11111111, bin);
      assert(s == "11111111");
      s.clear();
      sprint(s, 0765432101ull, oct);
      assert(s == "765432101");
      s.clear();
      sprint(s, 0xfedcba9, hex);
      assert(s == "fedcba9");
      s.clear();
      sprint(s, 0xfedcba9, hex, uppercase);
      assert(s == "FEDCBA9");
      s.clear();
      sprint(s, 0xffff'ffff'ffff'ffffu, hex);
      assert(s == "ffffffffffffffff");
      s.clear();
    }
    // print_args
    {
      string s;
      sprint(s, 123);
      assert(s == "123");
      s.clear();
      sprint(s, 123, setw(3));
      assert(s == "123");
      s.clear();
      sprint(s, 123, setw(6));
      assert(s == "   123");
      s.clear();
      sprint(s, 123, setw(3), left);
      assert(s == "123");
      s.clear();
      sprint(s, 123, setw(3), right);
      assert(s == "123");
      s.clear();
      sprint(s, 123, setw(6), left);
      assert(s == "   123");
      s.clear();
      sprint(s, 123, setw(6), right);
      assert(s == "123   ");
      s.clear();
      sprint(s, 123, setw(6), right, setfill('0'));
      assert(s == "123000");
      s.clear();
      sprint(s, 123, setw(1));
      assert(s == "123");
      s.clear();
    }
    // int_print_args
    {
      string s;
      sprint(s, 123, setseparator(1));
      assert(s == "1'2'3");
      s.clear();
      sprint(s, 123, setseparator(3));
      assert(s == "123");
      s.clear();
      sprint(s, 123, setseparator(9));
      assert(s == "123");
      s.clear();
      sprint(s, -123, setseparator(3));
      assert(s == "-123");
      s.clear();
      sprint(s, 123, setseparator(3), showpos);
      assert(s == "+123");
      s.clear();
      sprint(s, 1234567890, setseparator(4));
      assert(s == "12'3456'7890");
      s.clear();

      sprint(s, 123, noshowpos);
      assert(s == "123");
      s.clear();

      sprint(s, 0xfffe, hex);
      assert(s == "fffe");
      s.clear();
      sprint(s, 0xfffe, hex, uppercase);
      assert(s == "FFFE");
      s.clear();
      sprint(s, 0765, oct);
      assert(s == "765");
      s.clear();
      sprint(s, 0b10110, bin);
      assert(s == "10110");
      s.clear();

      sprint(s, 1234567, setseparator(0u));
      assert(s == "1234567");
      s.clear();
      sprint(s, 1234567, setseparator(1u));
      assert(s == "1'2'3'4'5'6'7");
      s.clear();
      sprint(s, 1234567, setseparator(2u));
      assert(s == "1'23'45'67");
      s.clear();
      sprint(s, 1234567, setseparator(3u));
      assert(s == "1'234'567");
      s.clear();
      sprint(s, 1234567, setseparator(4u));
      assert(s == "123'4567");
      s.clear();
      sprint(s, 1234567, setseparator(5u));
      assert(s == "12'34567");
      s.clear();
      sprint(s, 1234567, setseparator(6u));
      assert(s == "1'234567");
      s.clear();
      sprint(s, 1234567, setseparator(7u));
      assert(s == "1234567");
      s.clear();

      sprint(s, -1234567, setseparator(0u));
      assert(s == "-1234567");
      s.clear();
      sprint(s, -1234567, setseparator(1u));
      assert(s == "-1'2'3'4'5'6'7");
      s.clear();
      sprint(s, -1234567, setseparator(2u));
      assert(s == "-1'23'45'67");
      s.clear();
      sprint(s, -1234567, setseparator(3u));
      assert(s == "-1'234'567");
      s.clear();
      sprint(s, -1234567, setseparator(4u));
      assert(s == "-123'4567");
      s.clear();
      sprint(s, -1234567, setseparator(5u));
      assert(s == "-12'34567");
      s.clear();
      sprint(s, -1234567, setseparator(6u));
      assert(s == "-1'234567");
      s.clear();
      sprint(s, -1234567, setseparator(7u));
      assert(s == "-1234567");
      s.clear();
    }
  }
}
void test_big_int() {
  // explicit big_int(int64_t)
  // =(int64_t)
  // to_int64()->optional<int64_t>
  // to_int32()->optional<int32_t>
  // is_non_negative()
  // is_zero()
  // neg()
  // is_zero()
  // data()
  // data(v_ref)
  {
    {
      big_int i(0);
      assert(i.is_zero());
      assert(i.is_non_negative() && i.data().empty() && inner::good(i));
      assert(!i.to_int64().empty() && i.to_int64().value() == 0);
      i.neg();
      assert(i.is_non_negative() && i.data().empty() && inner::good(i));
    }
    {
      big_int i(0x7000'0001'2000'000f);
      assert(i.is_non_negative() && i.data().size() == 2u && inner::good(i));
      assert(!i.to_int64().empty()
             && i.to_int64().value() == 0x7000'0001'2000'000f);
      auto v = move(i).data();
      assert(i.is_non_negative() && i.data().empty() && inner::good(i));
      assert(equal(v, vector<uint32_t>({0x2000'000fu, 0x7000'0001u})));
      i.data(move(v));
      assert(equal(i.data(), vector<uint32_t>({0x2000'000fu, 0x7000'0001u})));
      assert(i.is_non_negative() && i.data().size() == 2u && inner::good(i));
      assert(!i.to_int64().empty()
             && i.to_int64().value() == 0x7000'0001'2000'000f);
    }
    {
      big_int i(-0x7000'0001);
      assert(!i.is_non_negative() && i.data().size() == 1u && inner::good(i));
      assert(!i.to_int64().empty() && i.to_int64().value() == -0x7000'0001);
      auto v = move(i).data();
      assert(i.is_non_negative() && i.data().empty() && inner::good(i));
      assert(equal(v, seq(0x7000'0001u)));
    }
    {
      big_int i(numeric_limits<int64_t>::max());
      assert(!i.to_int64().empty()
             && i.to_int64().value() == numeric_limits<int64_t>::max());
      i = numeric_limits<int64_t>::min();
      assert(!i.to_int64().empty()
             && i.to_int64().value() == numeric_limits<int64_t>::min());
    }
    {
      big_int ii((int64_t)numeric_limits<int32_t>::min() - 1);
      big_int i(numeric_limits<int32_t>::min());
      big_int j(numeric_limits<int32_t>::max());
      big_int jj((int64_t)numeric_limits<int32_t>::max() + 1);
      auto x = i.to_int32();
      assert(!x.empty() && x == numeric_limits<int32_t>::min());
      x = j.to_int32();
      assert(!x.empty() && x == numeric_limits<int32_t>::max());
      x = ii.to_int32();
      assert(x.empty());
      x = jj.to_int32();
      assert(x.empty());
    }
  }

  // mul_pow_of_2(int32_t)
  {
    big_int i(0x7fed'cba9'8765'4321);

    i.mul_pow_of_2(0);
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0x8765'4321, 0x7fed'cba9)));

    i.mul_pow_of_2(4);
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0x7654'3210u, 0xfedc'ba98u, 0x7u)));
    i.mul_pow_of_2(-4);
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0x8765'4321, 0x7fed'cba9)));

    i.mul_pow_of_2(4 + 32 + 32);
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0u, 0u, 0x7654'3210u, 0xfedc'ba98u, 0x7u)));
    i.mul_pow_of_2(-(4 + 32 + 32));
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0x8765'4321, 0x7fed'cba9)));

    i.mul_pow_of_2(15);
    i.mul_pow_of_2(-15);
    assert(i.is_non_negative() && inner::good(i));
    assert(equal(i.data(), seq(0x8765'4321, 0x7fed'cba9)));

    i.mul_pow_of_2(-100);
    assert(i.is_non_negative() && inner::good(i));
    assert(i.data().empty());
  }

  // mul_pow_of_10(int32_t)
  {
    big_int i(0x7fed'cba9'8765'4321);
    i.mul_pow_of_10(7);
    i.mul_pow_of_10(-7);
    assert(i == big_int(0x7fed'cba9'8765'4321));
    i.mul_pow_of_10(100);
    i.mul_pow_of_10(-100);
    assert(i == big_int(0x7fed'cba9'8765'4321));
    i.mul_pow_of_10(-100);
    assert(i == big_int(0));
    i = 1;
    i.mul_pow_of_10(18);
    assert(!i.to_int64().empty() && *i.to_int64() == 1000000000000000000);
    i.mul_pow_of_10(-18);
    assert(!i.to_int64().empty() && *i.to_int64() == 1);
    i.mul_pow_of_10(-1);
    assert(!i.to_int64().empty() && *i.to_int64() == 0);
  }

  // mul(uint32_t)
  // div(uint32_t)
  {
    big_int i(0x7fed'cba9'8765'4321);
    i.mul(1000000000u);
    i.mul(100000000u);
    i.mul_pow_of_10(-17);
    assert(i == big_int(0x7fed'cba9'8765'4321));
    i.mul(1000000000u);
    assert(i.div(1000000000u) == 0u);
    assert(i == big_int(0x7fed'cba9'8765'4321));
    
    i = 0;
    i.mul(100);
    assert(i == big_int(0));

    i = 13;
    assert(i.div(3u) == 1u);
    assert(i == big_int(4));
  }

  // +=(const this_t &)
  // -=(const this_t &)
  // +(const this_t &)->this_t
  // -(const this_t &)->this_t
  {
    big_int i, j;
    i.data({0x1234'5678u, 0x8765'4321u, 0x1111'1111u, 0x1u});
    j.data({0x8765'4321u, 0x1234'5678u, 0x8888'8888u, 0x8u});
    big_int k = i + j;
    assert(k.is_non_negative());
    assert(equal(k.data(), seq(0x9999'9999u, 0x9999'9999u,
                               0x9999'9999u, 0x9u)));
    assert(inner::good(k));
    i.data({0x1234'5678u, 0x8765'4321u, 0x1111'1111u, 0x1u});
    j.data({0x8765'4321u, 0x789a'bcdfu, 0xeeee'eeeeu, 0x8u});
    k = i + j;
    assert(k.is_non_negative());
    assert(equal(k.data(), seq(0x9999'9999u, 0x0u, 0x0u, 0xau)));
    assert(inner::good(k));
    k = i - i;
    assert(k == big_int(0));
  }

  // inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
  {
    string s;
    // "0"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "0";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() == to_unsigned(i + 1) && s.front() == 'a');
      };

      init_s();
      assert_s();
      for (unsigned j : irng(0u, 100u))
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg(s, i, j);
      assert_s();
      assert(equal(rng(s.begin() + i, s.end()), "0"_sv));
    }
    // "12345"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "12345";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "12345"_sv);
      f(1u, "1'2'3'4'5"_sv);
      f(2u, "1'23'45"_sv);
      f(3u, "12'345"_sv);
      f(4u, "1'2345"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(sep_n, "12345"_sv);
    }
    // "123456"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "123456";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "123456"_sv);
      f(1u, "1'2'3'4'5'6"_sv);
      f(2u, "12'34'56"_sv);
      f(3u, "123'456"_sv);
      f(4u, "12'3456"_sv);
      f(5u, "1'23456"_sv);
      for (auto sep_n : irng(6u, 10u))
        f(sep_n, "123456"_sv);
    }
    // "1.23456"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "1.23456";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "1.23456"_sv);
      f(1u, "1.2'3'4'5'6"_sv);
      f(2u, "1.23'45'6"_sv);
      f(3u, "1.234'56"_sv);
      f(4u, "1.2345'6"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(0u, "1.23456"_sv);
    }
    // "1.2345"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "1.2345";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "1.2345"_sv);
      f(1u, "1.2'3'4'5"_sv);
      f(2u, "1.23'45"_sv);
      f(3u, "1.234'5"_sv);
      for (auto sep_n : irng(4u, 10u))
        f(0u, "1.2345"_sv);
    }
    // "12345.0"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "12345.0";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "12345.0"_sv);
      f(1u, "1'2'3'4'5.0"_sv);
      f(2u, "1'23'45.0"_sv);
      f(3u, "12'345.0"_sv);
      f(4u, "1'2345.0"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(0u, "12345.0"_sv);
    }
    // "1234.0"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "1234.0";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "1234.0"_sv);
      f(1u, "1'2'3'4.0"_sv);
      f(2u, "12'34.0"_sv);
      f(3u, "1'234.0"_sv);
      for (auto sep_n : irng(4u, 10u))
        f(0u, "1234.0"_sv);
    }
    // "1234.5678"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "1234.5678";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "1234.5678"_sv);
      f(1u, "1'2'3'4.5'6'7'8"_sv);
      f(2u, "12'34.56'78"_sv);
      f(3u, "1'234.567'8"_sv);
      for (auto sep_n : irng(4u, 10u))
        f(0u, "1234.5678"_sv);
    }
    // "1234.56789"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "1234.56789";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "1234.56789"_sv);
      f(1u, "1'2'3'4.5'6'7'8'9"_sv);
      f(2u, "12'34.56'78'9"_sv);
      f(3u, "1'234.567'89"_sv);
      f(4u, "1234.5678'9"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(0u, "1234.56789"_sv);
    }
    // "12345.6789"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "12345.6789";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "12345.6789"_sv);
      f(1u, "1'2'3'4'5.6'7'8'9"_sv);
      f(2u, "1'23'45.67'89"_sv);
      f(3u, "12'345.678'9"_sv);
      f(4u, "1'2345.6789"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(0u, "12345.6789"_sv);
    }
    // "12345.67890"
    for (int i : seq(0, 1)) {
      const auto init_s = [&]() {
        s = "12345.67890";
        if (i == 1)
          s.insert(s.begin(), 'a');
      };
      const auto assert_s = [&]() {
        if (i == 1)
          assert(s.size() > 1u && s.front() == 'a');
      };
      const auto f = [&](size_t sep_n, sview v) {
        init_s();
        assert_s();
        inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
          (s, i, sep_n);
        assert_s();
        assert(equal(rng(s.begin() + i, s.end()), v));
      };
      f(0u, "12345.67890"_sv);
      f(1u, "1'2'3'4'5.6'7'8'9'0"_sv);
      f(2u, "1'23'45.67'89'0"_sv);
      f(3u, "12'345.678'90"_sv);
      f(4u, "1'2345.6789'0"_sv);
      for (auto sep_n : irng(5u, 10u))
        f(0u, "12345.67890"_sv);
    }
  }
  // inner::fns::fixed_fraction_string_to_scientific
  {
    string s;
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("+0.00000", (size_t)(-1), 0u, false, true);
    assert(s == "+0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("+0.00000", (size_t)(-1), 0u, false, false);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("-0.00000", (size_t)(-1), 0u, false, false);
    assert(s == "-0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00000", (size_t)(-1), 0u, false, false);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00000", (size_t)(-1), 0u, false, true);
    assert(s == "+0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("+0.00000", (size_t)(-1), 0u, false, true);
    assert(s == "+0e+0");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", (size_t)-1, 0u, false, false);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 0u, 0u, false, false);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 1u, 0u, false, false);
    assert(s == "0.0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 3u, 0u, false, false);
    assert(s == "0.000e+0");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1", (size_t)-1, 0u, false, false);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1", (size_t)-1, 0u, false, false);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.10", (size_t)-1, 0u, false, false);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1234", (size_t)-1, 0u, false, false);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.12340", (size_t)-1, 0u, false, false);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.12340000", (size_t)-1, 0u, false, false);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.0001", 0u, 0u, false, false);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00015", 0u, 0u, false, false);
    assert(s == "2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00014", 0u, 0u, false, false);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00095", 0u, 0u, false, false);
    assert(s == "1e-3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 1u, 0u, false, false);
    assert(s == "1.2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 3u, 0u, false, false);
    assert(s == "1.200e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 1u, 0u, false, false);
    assert(s == "2.3e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 2u, 0u, false, false);
    assert(s == "2.35e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 3u, 0u, false, false);
    assert(s == "2.346e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.01995", 2u, 0u, false, false);
    assert(s == "2.00e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.09995", 2u, 0u, false, false);
    assert(s == "1.00e-1");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100.00", (size_t)-1, 0u, false, false);
    assert(s == "1e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1", 0u, 0u, false, false);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.0", 0u, 0u, false, false);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.5", 0u, 0u, false, false);
    assert(s == "2e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("9.5", 0u, 0u, false, false);
    assert(s == "1e+1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("99.5", 0u, 0u, false, false);
    assert(s == "1e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.51234000", 0u, 0u, false, false);
    assert(s == "1e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("159999.51234000", 0u, 0u, false, false);
    assert(s == "2e+5");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 2u, 0u, false, false);
    assert(s == "1.23e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 10u, 0u, false, false);
    assert(s == "1.2345000000e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 5u, 0u, false, false);
    assert(s == "1.23450e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 4u, 0u, false, false);
    assert(s == "1.2345e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("125.45", 1u, 0u, false, false);
    assert(s == "1.3e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999990.45", 3u, 0u, false, false);
    assert(s == "1.000e+6");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false);
    assert(s == "1.300e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false);
    assert(s == "1.300e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 2u, 0u, false, false);
    assert(s == "1.24e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 3u, 0u, false, false);
    assert(s == "1.236e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9912", 3u, 0u, false, false);
    assert(s == "1.000e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 0u, false, false);
    assert(s == "1.0000e+3");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 2u, false, false);
    assert(s == "1.00'00e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 1u, false, false);
    assert(s == "1.0'0'0'0e+1'1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 3u, false, false);
    assert(s == "1.000'0e+11");
  }

  // sscan
  // sscan_hex
  // sprint
  // sprint_hex
  {
    // sscan + sprint
    {
      auto r = make_rander<mt19937>();
      for (int c = 1000; c != 0; --c) {
        string s;
        s.push_back((char)(to_unsigned('0') + r(1u, 9u)));
        for (int cc = r(0u, 100u); cc != 0; --cc)
          s.push_back((char)(to_unsigned('0') + r(0u, 9u)));
        sview sv = s;
        big_int x;
        assert(x.sscan(sv) && inner::good(x) && sv.empty());
        const string s2 = x.sprint();
        assert(s == s2);
      }
    }
    // sscan
    {
      big_int i;
      auto v = "+"_sv;
      auto v2 = v;
      assert(!i.sscan(v2) && v2 == v);

      v = "-";
      v2 = v;
      assert(!i.sscan(v2) && v2 == v);

      v = "1";
      v2 = v;
      assert(i.sscan(v2) && v2.empty()
             && inner::good(i) && i == big_int(1));

      v = "-1";
      v2 = v;
      assert(i.sscan(v2) && v2.empty()
             && inner::good(i) && i == big_int(-1));

      v = "+9223372036854775807";
      v2 = v;
      assert(i.sscan(v2) && v2.empty()
             && inner::good(i) && i == big_int(9223372036854775807));
      v = "-9223372036854775807";
      v2 = v;
      assert(i.sscan(v2) && v2.empty()
             && inner::good(i) && i == big_int(-9223372036854775807));

      v = "1234.";
      v2 = v;
      assert(i.sscan(v2) && v2 == "."
             && inner::good(i) && i == big_int(1234));
      v = "-1234.";
      v2 = v;
      assert(i.sscan(v2) && v2 == "."
             && inner::good(i) && i == big_int(-1234));
    }
    // sprint
    {
      big_int i(1234567);
      string s = i.sprint(3u);
      assert(s == "1'234'567");
      s = i.sprint(1u);
      assert(s == "1'2'3'4'5'6'7");
      s = i.sprint(6u);
      assert(s == "1'234567");
      s = i.sprint(7u);
      assert(s == "1234567");
    }
    // sscan_hex
    // sprint_hex
    {
      sview v = "-eeef'feee'abcd'1234'eeee'eeee";
      big_int i;
      assert(i.sscan_hex(v) && v.empty() && inner::good(i));
      assert(i.sprint_hex(4u) == "-eeef'feee'abcd'1234'eeee'eeee");
    }
  }

  // sscan_scientific
  // sprint_scientific
  {
    big_int i;
    assert(i.sprint_scientific() == "0.000000e+0");
    assert(i.sprint_scientific(0u) == "0e+0");
    assert(i.sprint_scientific(1u) == "0.0e+0");
    assert(i.sprint_scientific(2u) == "0.00e+0");
    i = 0;

    auto v = "1234.456"_sv;
    assert(!i.sscan_scientific(v) && i.is_zero() && inner::good(i));
    i = 0;

    v = "1234.567e3"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.234567e+6");

    v = "1234.567e-3"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.000000e+0");

    v = "1234.567e+0"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.235000e+3");

    v = "1534.567e-3"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "2.000000e+0");

    v = "1234.567e-100"_sv;
    assert(i.sscan_scientific(v) && i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "0.000000e+0");

    v = "1234.567e8"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.234567e+11");

    v = "1'2'3'4.5'6'7e8"_sv;
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.234567e+11");

    v = "12345678901234567890";
    assert(!i.sscan_scientific(v));
    assert(v == "12345678901234567890");
    v = "12345678901234567890e0";
    assert(i.sscan_scientific(v) && !i.is_zero() && inner::good(i));
    assert(v.empty());
    assert(i.sprint_scientific() == "1.234568e+19");
    assert(i.sprint_scientific(6u, 1u, true, true) == "+1.2'3'4'5'6'8E+1'9");
    assert(i.sprint_scientific(6u, 2u, false, false) == "1.23'45'68e+19");
    assert(i.sprint_scientific(6u, 3u, false, false) == "1.234'568e+19");
    assert(i.sprint_scientific(6u, 5u, false, false) == "1.23456'8e+19");
    assert(i.sprint_scientific(6u, 6u, false, false) == "1.234568e+19");
  }
}
template <class F>
void test_sprint_floating_point_impl() {
  using traits = floating_point_traits<F>;
  // nan
  {
    const F f = numeric_limits<F>::nan();
    const F f2 = -numeric_limits<F>::nan();

    assert(traits::is_nan(f));
    assert(traits::is_nan(f2));
    assert(traits::is_positive(f));
    assert(traits::is_negative(f2));

    assert(traits::e(f) == traits::e(f2));

    for (float x : seq(f, f2)) {
      assert(sprint(string{}, x) == "nan");
      assert(sprint(string{}, x, uppercase) == "NAN");
      assert(sprint(string{}, x, setw(5u)) == "  nan");
      assert(sprint(string{}, x, setw(5u), left) == "  nan");
      assert(sprint(string{}, x, setw(5u), right) == "nan  ");
      assert(sprint(string{}, x, setw(5u), setfill('_')) == "__nan");
    }
  }
  // infinity
  {
    const F pos_inf = numeric_limits<F>::infinity();
    const F neg_inf = -numeric_limits<F>::infinity();
    assert(floating_point_traits<F>::is_infinity(pos_inf));
    assert(floating_point_traits<F>::is_infinity(neg_inf));
    assert(traits::is_positive(pos_inf));
    assert(traits::is_negative(neg_inf));
    assert(traits::e(pos_inf) == traits::e_max);
    assert(traits::e(neg_inf) == traits::e_max);
    assert(traits::f(pos_inf) == 0u);
    assert(traits::f(neg_inf) == 0u);
    assert(sprint(""_s, pos_inf) == "inf");
    assert(sprint(""_s, pos_inf, showpos) == "+inf");
    assert(sprint(""_s, neg_inf) == "-inf");
    assert(sprint(""_s, pos_inf, uppercase) == "INF");
    assert(sprint(""_s, pos_inf, showpos, uppercase) == "+INF");
    assert(sprint(""_s, neg_inf, uppercase) == "-INF");
  }
  // denormalized
  // normalized
  {
    // fixed
    {
      assert(sprint(""_s, (F)0.0) == "0");
      assert(sprint(""_s, (F)0.0, setseparator(1u)) == "0");
      assert(sprint(""_s, (F)0.0, setseparator(10u)) == "0");
      assert(sprint(""_s, (F)0.0, showpos) == "+0");
      assert(sprint(""_s, (F)0.0, setw(5u)) == "    0");
      assert(sprint(""_s, (F)0.0, setw(5u), right) == "0    ");
      assert(sprint(""_s, (F)0.0, setw(5u), setfill('_')) == "____0");
      assert(sprint(""_s, (F)0.0, setw(5u), right, setfill('_')) == "0____");
      assert(sprint(""_s, (F)0.0, setprecision(5u)) == "0.00000");
      assert(sprint(""_s, (F)0.0, setprecision(1u)) == "0.0");
      assert(sprint(""_s, (F)0.0, setprecision(0u)) == "0");
      assert(sprint(""_s, (F)123.5, setprecision(0u)) == "124");
      assert(sprint(""_s, (F)123.5678, setprecision(3u)) == "123.568");
      assert(sprint(""_s, (F)123.56745, setprecision(3u)) == "123.567");
      assert(sprint(""_s, (F)123.56745, setprecision(1u)) == "123.6");
      assert(sprint(""_s, (F)123.56745, setprecision(1u),
                    setseparator(2u))
             == "1'23.6");
      assert(sprint(""_s, (F)1.003125, setprecision(7u)) == "1.0031250");
      assert(sprint(""_s, (F)1.003125, setprecision(7u), setseparator(1u))
             == "1.0'0'3'1'2'5'0");
      assert(sprint(""_s, (F)1.003125, setprecision(7u), setseparator(4u))
             == "1.0031'250");
    }
    // scientific
    {
      assert(sprint(""_s, (F)15345.6, scientific, setprecision(0u)) == "2e+4");
      assert(sprint(""_s, (F)14345.6, scientific, setprecision(0u)) == "1e+4");
      assert(sprint(""_s, (F)14345.6, scientific, setprecision(4u))
             == "1.4346e+4");
      assert(sprint(""_s, (F)14345.6, scientific, setprecision(4u), showpos)
             == "+1.4346e+4");
      assert(sprint(""_s, (F)-14345.6, scientific, setprecision(4u))
             == "-1.4346e+4");

      assert(sprint(""_s, (F)0.00000123456, scientific, setprecision(4u))
             == "1.2346e-6");

      assert(sprint(""_s, (F)0.0, scientific, setprecision(4u))
             == "0.0000e+0");
      assert(sprint(""_s, (F)0.0, scientific,
                    setprecision(4u), setseparator(2u))
             == "0.00'00e+0");
      assert(sprint(""_s, (F)0.0, scientific) == "0e+0");

      assert(sprint(""_s, (F)99999, scientific, setprecision(4u))
             == "9.9999e+4");
      assert(sprint(""_s, (F)99999, scientific, setprecision(3u))
             == "1.000e+5");
      assert(sprint(""_s, (F)99999, scientific,
                    setprecision(3u), setseparator(1u))
             == "1.0'0'0e+5");
      assert(sprint(""_s, (F)99999, scientific,
                    setprecision(3u), setseparator(1u), uppercase)
             == "1.0'0'0E+5");
    }
  }
}
void test_sprint_floating_point() {
  // inner::fns::fixed_fraction_string_apply_print_args
  {
    string s;
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("+0.01234", (size_t)-1, 0u, false);
    assert(s == "0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("+0.01234", (size_t)-1, 0u, true);
    assert(s == "+0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.01234", (size_t)-1, 0u, false);
    assert(s == "0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.01234", (size_t)-1, 0u, true);
    assert(s == "+0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("-0.01234", (size_t)-1, 0u, true);
    assert(s == "-0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("-0.01234", (size_t)-1, 0u, false);
    assert(s == "-0.01234");

    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", (size_t)-1, 0u, false);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 0u, 0u, false);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 1u, 0u, false);
    assert(s == "1.0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 5u, 0u, false);
    assert(s == "1.00000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.00000", (size_t)-1, 0u, false);
    assert(s == "0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.12345", 0u, 0u, false);
    assert(s == "0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("9.52345", 0u, 0u, false);
    assert(s == "10");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1239.52345", 0u, 0u, false);
    assert(s == "1240");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("123999999.52345", 0u, 0u, false);
    assert(s == "124000000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 3u, 0u, false);
    assert(s == "1.234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 4u, 0u, false);
    assert(s == "1.2340");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 6u, 0u, false);
    assert(s == "1.234000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 2u, 0u, false);
    assert(s == "1.23");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 1u, 0u, false);
    assert(s == "1.2");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.99990", 3u, 0u, false);
    assert(s == "13000.000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.19990", 3u, 0u, false);
    assert(s == "12999.200");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("99999.99990", 3u, 0u, false);
    assert(s == "100000.000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1234.56789", (size_t)-1, 2u, false);
    assert(s == "12'34.56'78'9");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1234.56789", (size_t)-1, 2u, true);
    assert(s == "+12'34.56'78'9");
  }

  test_sprint_floating_point_impl<float>();
  test_sprint_floating_point_impl<double>();
}
template <class F>
void test_sscan_floating_point_impl() {
  using traits = floating_point_traits<F>;
  using float_t = typename traits::float_t;
  using uint_t = typename traits::uint_t;
  using int_t = typename traits::int_t;

  // nan
  {
    for (sview v : seq("nan"_sv, "NAN", "+nan", "+NAN")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_nan(f) && traits::is_positive(f));
    }
    for (sview v : seq("-nan"_sv, "-NAN")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_nan(f) && traits::is_negative(f));
    }
  }
  // inf
  {
    for (sview v : seq("inf"_sv, "INF", "+inf", "+INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::is_positive(f));
    }
    for (sview v : seq("-inf"_sv, "-INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::is_negative(f));
    }
  }
  // finite number
  {
    rander<mt19937_64> r;
    for (uint_t e : irng((uint_t)0u, (uint_t)traits::e_max)) {
      const auto init = [e](uint_t &z) {
        z = 0u;
        z += (uint_t)(e << traits::k);
      };
      uint_t i;
      float_t x;
      string s;
      sview v;

      const auto f = [&]() {
        const float_t x0 = bit_cast<float_t>(i);
        s = sprint(""_s, x0);
        assert(sscan(v = s, x) && v.empty());
        assert(x == x0);
      };

      // min_f
      init(i);
      f();
      // min_f + 1
      init(i);
      i += 1u;
      f();
      // max_f - 1;
      init(i);
      i += traits::f_max;
      --i;
      f();
      // max_f
      init(i);
      i += traits::f_max;
      f();

      for (int c = 22; c != 0; --c) {
        init(i);
        i += (uint_t)r(2u, traits::f_max - 2u);
        f();
      }
    }

    float_t x{};
    sview v = "+0.0";
    assert(sscan(v, x) && v.empty());
    assert(x == (float_t)0.0);
    assert(sprint(""_s, x) == "0");
    v = "-0.0";
    assert(sscan(v, x) && v.empty());
    assert(x == (float_t)0.0);
    assert(sprint(""_s, x) == "-0");
    v = "1.234e6";
    assert(sscan(v, x) && v.empty());
    assert(x == 1234000.0);
    v = "1.23e310";
    assert(sscan(v, x) && v.empty());
    assert(x == traits::positive_inf());
    v = "-1.23e310";
    assert(sscan(v, x) && v.empty());
    assert(x == traits::negative_inf());
    v = "1.23e10000000000000000000";
    assert(sscan(v, x) && v.empty());
    assert(x == traits::positive_inf());
    v = "-1.23e1000000000000000000";
    assert(sscan(v, x) && v.empty());
    assert(x == traits::negative_inf());
    v = "1.23e-325";
    assert(sscan(v, x) && v.empty());
    assert(bit_cast<uint_t>(x) == bit_cast<uint_t>((float_t)+0.0f));
    v = "-1.23e-325";
    assert(sscan(v, x) && v.empty());
    assert(bit_cast<uint_t>(x) == bit_cast<uint_t>((float_t)-0.0f));
    v = "1.23e-10000000000000000000000000";
    assert(sscan(v, x) && v.empty());
    assert(bit_cast<uint_t>(x) == bit_cast<uint_t>((float_t)+0.0f));
    v = "-1.23e-10000000000000000000000000";
    assert(sscan(v, x) && v.empty());
    assert(bit_cast<uint_t>(x) == bit_cast<uint_t>((float_t)-0.0f));
  }
}
void test_sscan_floating_point() {
  test_sscan_floating_point_impl<float>();
  test_sscan_floating_point_impl<double>();
}
void test_ssplitter() {
  {
    ssplitter s("ab_cd_efg_h", "_");
    ssplitter s2("ab_cd_efg_h", '_');
    ssplitter s3("_cd_efg_h", "_");
    ssplitter s4("_", "_");
    ssplitter s5("", "_");
    ssplitter s6("", "");
    ssplitter s7("abc", "");
    ssplitter s8("", "abc");
    assert(equal(s, seq("ab"_sv, "cd", "efg", "h")));
    assert(equal(s2, s));
    assert(equal(s3, seq(""_sv, "cd", "efg", "h")));
    assert(equal(s4, seq(""_sv, "")));
    assert(s5.empty());
    assert(s6.empty());
    assert(equal(s7, seq("abc"_sv)));
    assert(s8.empty());
  }
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
    fmt(seq("x"_sv, "y"), to_back(s));
    assert(equal(s, "abc x def y ghi"_sv));
    s.clear();
    fmt(begin(seq("x"_sv, "y")), to_back(s));
    assert(equal(s, "abc x def y ghi"_sv));

    s.clear();
    fmt("%%%%", to_back(s));
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

  {
    ssplitter s("ab_cd_efg_h", "_");
    ssplitter s2("ab_cd_efg_h", '_');
    ssplitter s3("_cd_efg_h", "_");
    ssplitter s4("_", "_");
    ssplitter s5("", "_");
    ssplitter s6("", "");
    ssplitter s7("abc", "");
    ssplitter s8("", "abc");

    assert(equal(s, seq("ab"_sv, "cd", "efg", "h")));
    assert(equal(s2, s));
    assert(equal(s3, seq(""_sv, "cd", "efg", "h")));
    assert(equal(s4, seq(""_sv, "")));
    assert(s5.empty());
    assert(s6.empty());
    assert(equal(s7, seq("abc"_sv)));
    assert(s8.empty());

    assert(s("||", to_back(string{})).base() == "ab||cd||efg||h");
    vector<string> v = {"$", "%%", "^^^", "&&&&"};
    string z;
    const auto p = s(v, to_back(z));
    assert(p.first == nth(v, 3));
    assert(z == "ab$cd%%efg^^^h");
    z.clear();
    v.resize(2u);
    const auto pp = s(v, to_back(z));
    assert(pp.first == end(v));
    assert(z == "ab$cd%%efg%%h");
  }
}
void test_ssplit() {
  string s = "ab__cd__efg";
  vector<string> v;
  ssplit(s, "__", output_itr([&](auto sv) {v.emplace_back(sv);}));
  assert(equal(v, seq("ab"_sv, "cd", "efg")));

  s = "__";
  v.clear();
  ssplit(s, "__", output_itr([&](auto sv) {v.emplace_back(sv);}));
  assert(equal(v, seq(""_sv, "")));

  s = "";
  v.clear();
  ssplit(s, "__", output_itr([&](auto sv) {v.emplace_back(sv);}));
  assert(empty(v));

  s = "__";
  v.clear();
  ssplit(s, "", output_itr([&](auto sv) {v.emplace_back(sv);}));
  assert(equal(v, seq("__"_sv)));
}

void test_io() {
  printf("io: ");

  test_c_file();
  test_console_c_file();
  test_fputc_iterator();
  test_sscan();
  test_sprint();
  test_big_int();
  test_sprint_floating_point();
  test_sscan_floating_point();
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
