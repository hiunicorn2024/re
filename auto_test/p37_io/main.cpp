//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>
#include <re/time.h>
#include <re/io.h>

#include <cassert>

namespace re::inner::test {

void test_c_file() {
  const string filename_f = string("./").append(__FILE__, "_f");
  const string filename_f1 = string("./").append(__FILE__, "_f1");
  const string filename_f2 = string("./").append(__FILE__, "_f2");
  const char *nf = filename_f.data();
  const char *nf1 = filename_f1.data();
  const char *nf2 = filename_f2.data();

  remove(nf);
  remove(nf1);
  remove(nf2);

  // special member functions
  // ==
  // c_file(name, mode = ...)
  // open(name, mode = ...)
  // empty()
  // error()
  // close()
  {
    static_assert(is_nothrow_default_constructible<c_file>);
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
      remove(nf1);
      remove(nf2);
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
      remove(nf2);
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
    remove(nf);
  }

  // c_file_exists
  // remove(const char *fname)
  // rename(const char *fname, const char *new_name)
  {
    assert(!c_file_exists(nf));
    c_file f(nf);
    f.close();
    assert(c_file_exists(nf));
    rename(nf, string(__FILE__).append("_f2").data());
    assert(!c_file_exists(nf));
    assert(c_file_exists(string("./").append(__FILE__, "_f2").data()));
    remove(string(__FILE__).append("_f2").data());

    f.open(nf);
    assert(c_file_exists(nf));
    f.close();
    remove(nf);
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
    remove(nf);
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
    remove(nf);
  }
}
void test_console_c_file() {
  // special member functions
  // stdin_f()
  // stdout_f()
  // base()
  {
    static_assert(!is_default_constructible<console_c_file>);
    static_assert(is_destructible<console_c_file>);
    static_assert(!is_move_constructible<console_c_file>);
    static_assert(!is_move_assignable<console_c_file>);
    static_assert(!is_copy_constructible<console_c_file>);
    static_assert(!is_copy_assignable<console_c_file>);

    static_assert(is_same<decltype(stdin_f()), console_c_file>);
    static_assert(is_same<decltype(stdout_f()), console_c_file>);
    static_assert(is_same<decltype(stderr_f()), console_c_file>);
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
  remove(nf);
  c_file f(nf);
  assert(f.putc_iter().base() == f.base());
  copy("abcd"_sv, f.putc_iter());
  f.close();
  f.open(nf);
  string s = f.get();
  assert(s == "abcd");
  f.close();
  remove(nf);
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
  // scan wide/u16/u32 string
  {
    // wstring
    {
      wsview v = L"aghijz1234567890";
      assert(!sscan(v, L"aghijz124567890"_sv));
      assert(sscan(v, L"aghijz1234567890"_sv));
      assert(v == L"");
    }
    // u16string
    {
      u16sview v = u"aghijz1234567890";
      assert(!sscan(v, u"aghijz124567890"_sv));
      assert(sscan(v, u"aghijz1234567890"_sv));
      assert(v == u"");
    }
    // u32string
    {
      u32sview v = U"aghijz1234567890";
      assert(!sscan(v, U"aghijz124567890"_sv));
      assert(sscan(v, U"aghijz1234567890"_sv));
      assert(v == U"");
    }

    // wstring
    {
      wstring s = L"+12345";
      wsview v = s;
      int x = 0;
      assert(sscan(v, x));
      assert(v.empty() && x == 12345);

      s = L"+10110";
      v = s;
      x = 0;
      assert(sscan(v, x, bin) && v.empty() && x == 0b10110);

      s = L"-10110";
      v = s;
      x = 0;
      assert(sscan(v, x, oct) && v.empty() && x == -010110);

      s = L"fFfFf";
      v = s;
      x = 0;
      assert(sscan(v, x, hex) && v.empty() && x == 0xfffff);
    }
    // u16string
    {
      u16string s = u"+12345";
      u16sview v = s;
      int x = 0;
      assert(sscan(v, x));
      assert(v.empty() && x == 12345);

      s = u"+10110";
      v = s;
      x = 0;
      assert(sscan(v, x, bin) && v.empty() && x == 0b10110);

      s = u"-10110";
      v = s;
      x = 0;
      assert(sscan(v, x, oct) && v.empty() && x == -010110);

      s = u"fFfFf";
      v = s;
      x = 0;
      assert(sscan(v, x, hex) && v.empty() && x == 0xfffff);
    }
    // u32string
    {
      u32string s = U"+12345";
      u32sview v = s;
      int x = 0;
      assert(sscan(v, x));
      assert(v.empty() && x == 12345);

      s = U"+10110";
      v = s;
      x = 0;
      assert(sscan(v, x, bin) && v.empty() && x == 0b10110);

      s = U"-10110";
      v = s;
      x = 0;
      assert(sscan(v, x, oct) && v.empty() && x == -010110);

      s = U"fFfFf";
      v = s;
      x = 0;
      assert(sscan(v, x, hex) && v.empty() && x == 0xfffff);
    }
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
      assert(test_sscan_success(type_tag<bool>{}, "0", false));
      assert(test_sscan_success(type_tag<bool>{}, "-0", false));
      assert(test_sscan_failure(type_tag<bool>{}, "--0"));
      assert(test_sscan_success(type_tag<bool>{}, "00", false));
      assert(test_sscan_success(type_tag<bool>{}, "00000", false));
      assert(test_sscan_success(type_tag<bool>{}, "0000000", false));
      assert(test_sscan_success(type_tag<bool>{}, "-00", false));
      assert(test_sscan_success(type_tag<bool>{}, "-00000", false));
      assert(test_sscan_success(type_tag<bool>{}, "-0000000", false));
      assert(test_sscan_success(type_tag<bool>{}, "1", true));
      assert(test_sscan_success(type_tag<bool>{}, "+1", true));
      assert(test_sscan_success(type_tag<bool>{}, "-1", true));
      assert(test_sscan_failure(type_tag<bool>{}, "++1"));
      assert(test_sscan_failure(type_tag<bool>{}, "--1"));
      assert(test_sscan_success(type_tag<bool>{}, "1000000", true));
      assert(test_sscan_success(type_tag<bool>{}, "0100000", true));
      assert(test_sscan_success(type_tag<bool>{}, "0020000", true));
      assert(test_sscan_success(type_tag<bool>{}, "0003000", true));
      assert(test_sscan_success(type_tag<bool>{}, "0000400", true));
      assert(test_sscan_success(type_tag<bool>{}, "0000050", true));
      assert(test_sscan_success(type_tag<bool>{}, "0000006", true));
      assert(test_sscan_success(type_tag<bool>{}, "0123456", true));
      assert(test_sscan_success(type_tag<bool>{}, "123456", true));
      assert(test_sscan_success(type_tag<bool>{}, "-1000000", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0100000", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0020000", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0003000", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0000400", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0000050", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0000006", true));
      assert(test_sscan_success(type_tag<bool>{}, "-0123456", true));
      assert(test_sscan_success(type_tag<bool>{}, "-123456", true));

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

    // scan signed integral (utf16/utf32)
    {
      u16string s = u"+12345";
      u16sview v = s;
      int x = 0;
      assert(sscan(v, x));
      assert(v.empty() && x == 12345);

      s = u"+10110";
      v = s;
      x = 0;
      assert(sscan(v, x, bin) && v.empty() && x == 0b10110);

      s = u"-10110";
      v = s;
      x = 0;
      assert(sscan(v, x, oct) && v.empty() && x == -010110);

      s = u"fFfFf";
      v = s;
      x = 0;
      assert(sscan(v, x, hex) && v.empty() && x == 0xfffff);
    }
    // scan unsigned integral (utf16/utf32)
    {
      u16string s = u"+12345";
      u16sview v = s;
      unsigned x = 0;
      assert(sscan(v, x));
      assert(v.empty() && x == 12345);

      s = u"+10110";
      v = s;
      x = 0;
      assert(sscan(v, x, bin) && v.empty() && x == 0b10110u);

      s = u"-10110";
      v = s;
      x = 0;
      assert(!sscan(v, x, oct) && v.begin() == s.begin());

      s = u"fFfFf";
      v = s;
      x = 0;
      assert(sscan(v, x, hex) && v.empty() && x == 0xfffffu);
    }
  }

  // sscan_single
  {
    char c{};
    sview sv0 = "abc";
    assert(sscan_single(sv0, c) && c == 'a');
    assert(sscan_single(sv0, c) && c == 'b');
    assert(sscan_single(sv0, c) && c == 'c');
    assert(!sscan_single(sv0, c) && c == 'c');
    sv0 = "ab";
    assert(sscan_single(sv0) == 'a');
    assert(sscan_single(sv0) == 'b');
    assert(sscan_single(sv0) == nullopt);

    u32string s = U"abcd";
    u32sview sv(s);
    char32_t c2{};
    assert(sscan_single(sv, c2) && c2 == U'a');
    assert(sscan_single(sv, c2) && c2 == U'b');
    assert(sscan_single(sv, c2) && c2 == U'c');
    assert(sscan_single(sv, c2) && c2 == U'd');
    assert(!sscan_single(sv, c2) && c2 == U'd');
  }

  // sscan_while
  {
    sview sv = "123456789";
    assert(equal(sscan_while(sv, [](char c) {return c < '6';}),
                 "12345"_sv));
    assert(equal(sv, "6789"_sv));
  }

  // sscan_ln
  {
    string s = {10u};
    string s2 = {10u, 'a'};
    string s3 = {13u};
    string s4 = {13u, 'a'};
    string s5 = {13u, 10u};
    string s6 = {13u, 10u, 'a'};

    sview sv;
    sv = s;
    assert(sscan_ln(sv) && sv.empty());
    sv = s2;
    assert(sscan_ln(sv) && equal(sv, seq('a')));
    assert(!sscan_ln(sv) && equal(sv, seq('a')));
    sv = s3;
    assert(sscan_ln(sv) && sv.empty());
    sv = s4;
    assert(sscan_ln(sv) && equal(sv, seq('a')));
    assert(!sscan_ln(sv) && equal(sv, seq('a')));
    sv = s5;
    assert(sscan_ln(sv) && sv.empty());
    sv = s6;
    assert(sscan_ln(sv) && equal(sv, seq('a')));
    assert(!sscan_ln(sv) && equal(sv, seq('a')));
  }

  // sscan_line
  {
    sview sv = "abcd\nef\rg\r\nhij";
    assert(sscan_line(sv).value() == "abcd"_sv);
    assert(sscan_line(sv).value() == "ef"_sv);
    assert(sscan_line(sv).value() == "g"_sv);
    assert(sscan_line(sv).value() == "hij"_sv);
    assert(sscan_line(sv).empty());
    sv = "abcd\nef\rg\r\n";
    assert(sscan_line(sv).value() == "abcd"_sv);
    assert(sscan_line(sv).value() == "ef"_sv);
    assert(sscan_line(sv).value() == "g"_sv);
    sv = "";
    assert(sscan_line(sv).empty());
    sv = "xyz";
    assert(sscan_line(sv).value() == "xyz"_sv);

    sv = "abcd\nef\rg\r\nhij";
    bool has_ln{};
    assert(sscan_line(sv, has_ln).value() == "abcd"_sv);
    assert(has_ln);
    assert(sscan_line(sv, has_ln).value() == "ef"_sv);
    assert(has_ln);
    assert(sscan_line(sv, has_ln).value() == "g"_sv);
    assert(has_ln);
    assert(sscan_line(sv, has_ln).value() == "hij"_sv);
    assert(!has_ln);
    assert(sscan_line(sv, has_ln).empty());
    assert(!has_ln);
    sv = "abcd\nef\rg\r\n";
    assert(sscan_line(sv, has_ln).value() == "abcd"_sv);
    assert(has_ln);
    assert(sscan_line(sv, has_ln).value() == "ef"_sv);
    assert(has_ln);
    assert(sscan_line(sv, has_ln).value() == "g"_sv);
    assert(has_ln);
    sv = "";
    assert(sscan_line(sv, has_ln).empty());
    assert(!has_ln);
    sv = "xyz";
    assert(sscan_line(sv, has_ln).value() == "xyz"_sv);
    assert(!has_ln);
    sv = "xyz\n";
    assert(sscan_line(sv, has_ln).value() == "xyz"_sv);
    assert(has_ln);
  }

  // sscan_spaces
  // sscan_common_spaces
  {
    const string s = " \t \n \r \r\n ab";
    sview sv = s;
    sscan_spaces(sv);
    assert(sv == "ab");
    sv = s;
    sscan_common_spaces(sv);
    assert(sv == "\n \r \r\n ab");

    u32string s2 = U" \t \n \r \r\n cd";
    u32sview sv2 = s2;
    sscan_spaces(sv2);
    assert(sv2 == U"cd");
    sv2 = s2;
    sscan_common_spaces(sv2);
    assert(sv2 == U"\n \r \r\n cd");
  }
}
void test_sprint() {
  // inner::sprint_s_arg<T>
  {
    static_assert(inner::sprint_s_arg<string>);
    static_assert(inner::sprint_s_arg<string &>);
    static_assert(inner::sprint_s_arg<string &&>);
    static_assert(!inner::sprint_s_arg<int>);
    static_assert(!inner::sprint_s_arg<int &>);
    static_assert(!inner::sprint_s_arg<int &&>);
  }
  
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

    // no string argument
    assert(sprint("abc") == "abc");
    assert(sprint("abc", setw(6)) == "   abc");
    assert(sprint("abc", setw(6), right) == "abc   ");
    assert(sprint("abc", print_args{}.setw(6)) == "   abc");

    // sprint for u32string arg
    u32string s2;
    sprint(s2, U"abc");
    assert(s2 == U"abc");
    sprint(s2, U"abc", setw(6));
    assert(s2 == U"abc   abc");
    sprint(s2, U"abc", print_args{}.setw(6).right());
    assert(s2 == U"abc   abcabc   ");
    // sprint_u32 for u32string arg
    s2.clear();
    sprint_u32(s2, U"abc");
    assert(s2 == U"abc");
    sprint_u32(s2, U"abc", setw(6));
    assert(s2 == U"abc   abc");
    sprint_u32(s2, U"abc", print_args{}.setw(6).right());
    assert(s2 == U"abc   abcabc   ");
    // return u32string by sprint_u32
    assert(sprint_u32(U"abc") == U"abc");
    assert(sprint_u32(U"abc", setw(6)) == U"   abc");
    assert(sprint_u32(U"abc", setw(6), right) == U"abc   ");
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
    // for u32string
    {
      u32string s;
      sprint(s, 12345);
      assert(s == U"12345");
      s.clear();
      sprint(s, 12345, int_print_args{}.showpos());
      assert(s == U"+12345");
      s.clear();
      sprint(s, 6789, showpos);
      assert(s == U"+6789");
    }
    // return string or u32string
    {
      assert(sprint(12345) == "12345");
      assert(sprint_u32(12345) == U"12345");
      assert(sprint(6789, showpos) == "+6789");
      assert(sprint_u32(6789, showpos) == U"+6789");
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
      ("+0.00000", (size_t)(-1), 0u, false, true, true);
    assert(s == "+0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("+0.00000", (size_t)(-1), 0u, false, false, true);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("-0.00000", (size_t)(-1), 0u, false, false, true);
    assert(s == "-0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00000", (size_t)(-1), 0u, false, false, true);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00000", (size_t)(-1), 0u, false, true, true);
    assert(s == "+0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("+0.00000", (size_t)(-1), 0u, false, true, true);
    assert(s == "+0e+0");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", (size_t)-1, 0u, false, false, true);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 0u, 0u, false, false, true);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 0u, 0u, false, false, false);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 1u, 0u, false, false, false);
    assert(s == "0.0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 1u, 0u, false, false, true);
    assert(s == "0e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 3u, 0u, false, false, false);
    assert(s == "0.000e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0", 3u, 0u, false, false, true);
    assert(s == "0e+0");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1", (size_t)-1, 0u, false, false, true);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1", (size_t)-1, 0u, false, false, true);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.10", (size_t)-1, 0u, false, false, true);
    assert(s == "1e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.1234", (size_t)-1, 0u, false, false, true);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.12340", (size_t)-1, 0u, false, false, true);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.12340000", (size_t)-1, 0u, false, false, true);
    assert(s == "1.234e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.0001", 0u, 0u, false, false, false);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.0001", 0u, 0u, false, false, true);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00015", 0u, 0u, false, false, false);
    assert(s == "2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00015", 0u, 0u, false, false, true);
    assert(s == "2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00014", 0u, 0u, false, false, false);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00014", 0u, 0u, false, false, true);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00095", 0u, 0u, false, false, false);
    assert(s == "1e-3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00095", 0u, 0u, false, false, true);
    assert(s == "1e-3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 1u, 0u, false, false, false);
    assert(s == "1.2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 1u, 0u, false, false, true);
    assert(s == "1.2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 3u, 0u, false, false, false);
    assert(s == "1.200e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.00012", 3u, 0u, false, false, true);
    assert(s == "1.2e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.0001", 3u, 0u, false, false, false);
    assert(s == "1.000e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.0001", 3u, 0u, false, false, true);
    assert(s == "1e-4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 1u, 0u, false, false, false);
    assert(s == "2.3e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 1u, 0u, false, false, true);
    assert(s == "2.3e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 2u, 0u, false, false, false);
    assert(s == "2.35e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 2u, 0u, false, false, true);
    assert(s == "2.35e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 3u, 0u, false, false, false);
    assert(s == "2.346e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.02345678", 3u, 0u, false, false, true);
    assert(s == "2.346e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.01995", 2u, 0u, false, false, false);
    assert(s == "2.00e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.01995", 2u, 0u, false, false, true);
    assert(s == "2e-2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.09995", 2u, 0u, false, false, false);
    assert(s == "1.00e-1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("0.09995", 2u, 0u, false, false, true);
    assert(s == "1e-1");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100.00", (size_t)-1, 0u, false, false, true);
    assert(s == "1e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1", 0u, 0u, false, false, false);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1", 0u, 0u, false, false, true);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.0", 0u, 0u, false, false, false);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.0", 0u, 0u, false, false, true);
    assert(s == "1e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.5", 0u, 0u, false, false, false);
    assert(s == "2e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("1.5", 0u, 0u, false, false, true);
    assert(s == "2e+0");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("9.5", 0u, 0u, false, false, false);
    assert(s == "1e+1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("9.5", 0u, 0u, false, false, true);
    assert(s == "1e+1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("99.5", 0u, 0u, false, false, false);
    assert(s == "1e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("99.5", 0u, 0u, false, false, true);
    assert(s == "1e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.51234000", 0u, 0u, false, false, false);
    assert(s == "1e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.51234000", 0u, 0u, false, false, true);
    assert(s == "1e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("159999.51234000", 0u, 0u, false, false, false);
    assert(s == "2e+5");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("159999.51234000", 0u, 0u, false, false, true);
    assert(s == "2e+5");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, true);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 2u, 0u, false, false, false);
    assert(s == "1.23e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 2u, 0u, false, false, true);
    assert(s == "1.23e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 10u, 0u, false, false, false);
    assert(s == "1.2345000000e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 10u, 0u, false, false, true);
    assert(s == "1.2345e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 5u, 0u, false, false, false);
    assert(s == "1.23450e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 5u, 0u, false, false, true);
    assert(s == "1.2345e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 4u, 0u, false, false, false);
    assert(s == "1.2345e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 4u, 0u, false, false, true);
    assert(s == "1.2345e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, true);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, false);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.45", 1u, 0u, false, false, true);
    assert(s == "1.2e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("125.45", 1u, 0u, false, false, false);
    assert(s == "1.3e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("125.45", 1u, 0u, false, false, true);
    assert(s == "1.3e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999990.45", 3u, 0u, false, false, false);
    assert(s == "1.000e+6");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999990.45", 3u, 0u, false, false, true);
    assert(s == "1e+6");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false, false);
    assert(s == "1.300e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false, true);
    assert(s == "1.3e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false, false);
    assert(s == "1.300e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("12999.45", 3u, 0u, false, false, true);
    assert(s == "1.3e+4");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 2u, 0u, false, false, false);
    assert(s == "1.24e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 2u, 0u, false, false, true);
    assert(s == "1.24e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 3u, 0u, false, false, false);
    assert(s == "1.236e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("123.56", 3u, 0u, false, false, true);
    assert(s == "1.236e+2");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9912", 3u, 0u, false, false, false);
    assert(s == "1.000e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9912", 3u, 0u, false, false, true);
    assert(s == "1e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 0u, false, false, false);
    assert(s == "1.0000e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 0u, false, false, true);
    assert(s == "1e+3");

    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 2u, false, false, false);
    assert(s == "1.00'00e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("999.9952", 4u, 2u, false, false, true);
    assert(s == "1e+3");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 1u, false, false, false);
    assert(s == "1.0'0'0'0e+1'1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 1u, false, false, true);
    assert(s == "1e+1'1");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 3u, false, false, false);
    assert(s == "1.000'0e+11");
    s = inner::fns::fixed_fraction_string_to_scientific<string>
      ("100000000000.1234", 4u, 3u, false, false, true);
    assert(s == "1e+11");
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
    assert(i.sprint_scientific(6u, false, 1u, true, true)
           == "+1.2'3'4'5'6'8E+1'9");
    assert(i.sprint_scientific(6u, false, 2u, false, false)
           == "1.23'45'68e+19");
    assert(i.sprint_scientific(6u, false, 3u, false, false)
           == "1.234'568e+19");
    assert(i.sprint_scientific(6u, false, 5u, false, false)
           == "1.23456'8e+19");
    assert(i.sprint_scientific(6u, false, 6u, false, false)
           == "1.234568e+19");
  }

  // sscan (utf16/utf32)
  // sprint (utf16/utf32)
  {
    wsview v1 =
      L"-123456789012345678901234567890123456789012345678901234567890"_sv;
    u16sview v2 =
      u"-123456789012345678901234567890123456789012345678901234567890"_sv;
    u32sview v3 =
      U"-123456789012345678901234567890123456789012345678901234567890"_sv;
    big_int i, i2, i3;
    assert(i.sscan(v1) && v1.empty());
    assert(i2.sscan(v2) && v2.empty());
    assert(i3.sscan(v3) && v3.empty());
    static_assert(same_as<decltype(i.sprint<wstring>(10u)), wstring>);
    static_assert(same_as<decltype(i.sprint<u16string>(10u)), u16string>);
    static_assert(same_as<decltype(i.sprint<u32string>(10u)), u32string>);
    wstring s1 = i.sprint<wstring>(10u);
    u16string s2 = i.sprint<u16string>(10u);
    u32string s3 = i.sprint<u32string>(10u);
    assert(s1
           == L"-1234567890'1234567890'1234567890'1234567890'"
           "1234567890'1234567890");
    assert(s2
           == u"-1234567890'1234567890'1234567890'1234567890'"
           "1234567890'1234567890");
    assert(s3
           == U"-1234567890'1234567890'1234567890'1234567890'"
           "1234567890'1234567890");
  }
  // sscan_hex (utf16/utf32)
  // sprint_hex (utf16/utf32)
  {
    wsview v1 =
      L"-123def'123def'123def'123def'123def'123def'123def'123def"_sv;
    u16sview v2 =
      u"-123def'123def'123def'123def'123def'123def'123def'123def"_sv;
    u32sview v3 =
      U"-123def'123def'123def'123def'123def'123def'123def'123def"_sv;
    big_int i, i2, i3;
    assert(i.sscan_hex(v1) && v1.empty());
    assert(i2.sscan_hex(v2) && v2.empty());
    assert(i3.sscan_hex(v3) && v3.empty());
    static_assert(same_as<decltype(i.sprint_hex<wstring>(6u)), wstring>);
    static_assert(same_as<decltype(i.sprint_hex<u16string>(6u)), u16string>);
    static_assert(same_as<decltype(i.sprint_hex<u32string>(6u)), u32string>);
    wstring s1 = i.sprint_hex<wstring>(6u);
    u16string s2 = i.sprint_hex<u16string>(6);
    u32string s3 = i.sprint_hex<u32string>(6u);
    assert(s1
           == L"-123def'123def'123def'123def'123def'123def'123def'123def");
    assert(s2
           == u"-123def'123def'123def'123def'123def'123def'123def'123def");
    assert(s3
           == U"-123def'123def'123def'123def'123def'123def'123def'123def");
  }
  // sscan_scientific (utf16/utf32)
  // sprint_scientific (utf16/utf32)
  {
    wsview v1 = L"-123456.789e+20";
    u16sview v2 = u"-123456.789e+20";
    u32sview v3 = U"-123456.789e+20";
    big_int i, i2, i3;
    assert(i.sscan_scientific(v1) && v1.empty());
    assert(i2.sscan_scientific(v2) && v2.empty());
    assert(i3.sscan_scientific(v3) && v3.empty());
    static_assert(same_as<decltype(i.sprint_scientific<wstring>(2u)), wstring>);
    static_assert(same_as
                  <decltype(i.sprint_scientific<u16string>(2u)), u16string>);
    static_assert(same_as
                  <decltype(i.sprint_scientific<u32string>(2u)), u32string>);
    wstring s1 = i.sprint_scientific<wstring>((size_t)-1);
    u16string s2 = i.sprint_scientific<u16string>((size_t)-1);
    u32string s3 = i.sprint_scientific<u32string>((size_t)-1);
    assert(s1 == L"-1.23456789e+25");
    assert(s2 == u"-1.23456789e+25");
    assert(s3 == U"-1.23456789e+25");
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
    assert(traits::sign(f));
    assert(!traits::sign(f2));

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
    assert(traits::sign(pos_inf));
    assert(!traits::sign(neg_inf));
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
      ("+0.01234", (size_t)-1, 0u, false, true);
    assert(s == "0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("+0.01234", (size_t)-1, 0u, true, true);
    assert(s == "+0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.01234", (size_t)-1, 0u, false, true);
    assert(s == "0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.01234", (size_t)-1, 0u, true, true);
    assert(s == "+0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("-0.01234", (size_t)-1, 0u, true, true);
    assert(s == "-0.01234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("-0.01234", (size_t)-1, 0u, false, true);
    assert(s == "-0.01234");

    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", (size_t)-1, 0u, false, true);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 0u, 0u, false, false);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 0u, 0u, false, true);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 1u, 0u, false, false);
    assert(s == "1.0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 1u, 0u, false, true);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 5u, 0u, false, false);
    assert(s == "1.00000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1", 5u, 0u, false, true);
    assert(s == "1");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.00000", (size_t)-1, 0u, false, true);
    assert(s == "0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.12345", 0u, 0u, false, false);
    assert(s == "0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("0.12345", 0u, 0u, false, true);
    assert(s == "0");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("9.52345", 0u, 0u, false, false);
    assert(s == "10");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("9.52345", 0u, 0u, false, true);
    assert(s == "10");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1239.52345", 0u, 0u, false, false);
    assert(s == "1240");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1239.52345", 0u, 0u, false, true);
    assert(s == "1240");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("123999999.52345", 0u, 0u, false, false);
    assert(s == "124000000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("123999999.52345", 0u, 0u, false, true);
    assert(s == "124000000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 3u, 0u, false, false);
    assert(s == "1.234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 3u, 0u, false, true);
    assert(s == "1.234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 4u, 0u, false, false);
    assert(s == "1.2340");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 4u, 0u, false, true);
    assert(s == "1.234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 6u, 0u, false, false);
    assert(s == "1.234000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 6u, 0u, false, true);
    assert(s == "1.234");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 2u, 0u, false, false);
    assert(s == "1.23");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 2u, 0u, false, true);
    assert(s == "1.23");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 1u, 0u, false, false);
    assert(s == "1.2");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1.234", 1u, 0u, false, true);
    assert(s == "1.2");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.99990", 3u, 0u, false, false);
    assert(s == "13000.000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.99990", 3u, 0u, false, true);
    assert(s == "13000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.19990", 3u, 0u, false, false);
    assert(s == "12999.200");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("12999.19990", 3u, 0u, false, true);
    assert(s == "12999.2");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("99999.99990", 3u, 0u, false, false);
    assert(s == "100000.000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("99999.99990", 3u, 0u, false, true);
    assert(s == "100000");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1234.56789", (size_t)-1, 2u, false, true);
    assert(s == "12'34.56'78'9");
    s = inner::fns::fixed_fraction_string_apply_print_args<string>
      ("1234.56789", (size_t)-1, 2u, true, true);
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
      assert(v.empty() && traits::is_nan(f) && traits::sign(f));
    }
    for (sview v : seq("-nan"_sv, "-NAN")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_nan(f) && !traits::sign(f));
    }

    for (wsview v : seq(L"nan"_sv, L"NAN", L"+nan", L"+NAN")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_nan(f) && traits::sign(f));
    }
    for (wsview v : seq(L"-nan"_sv, L"-NAN")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_nan(f) && !traits::sign(f));
    }
  }
  // inf
  {
    for (sview v : seq("inf"_sv, "INF", "+inf", "+INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::sign(f));
    }
    for (sview v : seq("-inf"_sv, "-INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && !traits::sign(f));
    }

    for (wsview v : seq(L"inf"_sv, L"INF", L"+inf", L"+INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::sign(f));
    }
    for (wsview v : seq(L"-inf"_sv, L"-INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && !traits::sign(f));
    }

    for (u16sview v : seq(u"inf"_sv, u"INF", u"+inf", u"+INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::sign(f));
    }
    for (u16sview v : seq(u"-inf"_sv, u"-INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && !traits::sign(f));
    }

    for (u32sview v : seq(U"inf"_sv, U"INF", U"+inf", U"+INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && traits::sign(f));
    }
    for (u32sview v : seq(U"-inf"_sv, U"-INF")) {
      F f = (F)0;
      assert(sscan(v, f));
      assert(v.empty() && traits::is_infinity(f) && !traits::sign(f));
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
      string ws;
      string u16s;
      string u32s;
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
void test_sscan_sprint_floating_point_by_wide_string() {
  wsview v1 = L"-126.25e-1";
  u16sview v2 = u"-126.25e-1";
  u32sview v3 = U"-126.25e-1";
  float f1{}, f2{}, f3{};
  assert(sscan(v1, f1) && v1.empty());
  assert(sscan(v2, f2) && v2.empty());
  assert(sscan(v3, f3) && v3.empty());
  assert(f1 == f2 && f2 == f3);
  assert(f1 == -12.625f);
  wstring s1;
  u16string s2;
  u32string s3;
  sprint(s1, f1);
  sprint(s2, f2);
  sprint(s3, f3);
  assert(s1 == L"-12.625");
  assert(s2 == u"-12.625");
  assert(s3 == U"-12.625");
  s1.clear();
  s2.clear();
  s3.clear();
  sprint(s1, f1, scientific);
  sprint(s2, f2, scientific);
  sprint(s3, f3, scientific);
  assert(s1 == L"-1.2625e+1");
  assert(s2 == u"-1.2625e+1");
  assert(s3 == U"-1.2625e+1");
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
  {
    u32string s = U"ab__cd__efg";
    vector<u32string> v;
    ssplit(s, U"__", output_itr([&](auto sv) {v.emplace_back(sv);}));
    assert(equal(v, seq(U"ab"_sv, U"cd", U"efg")));
  }

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

struct g_t {
  enum class opr {
    paren,
    pos, neg,
    pow,
    mul, div,
    add, sub,
    cond,
    fn
  };
  using operator_type = opr;
  static bool operator_is_function(operator_type o) noexcept {
    return o == opr::fn;
  }
  using operand_type = double;
  using stack_type = vector<operand_type>;
  using function_type = function<operand_type (const stack_type &)>;
  using char_type = char;
  using string_type = string;
  using view_type = sview;
  struct tree_node_type {
    operator_type id;
    string_type function_name;
    optional<operand_type> result;
  };

  const auto &operator_list() const {
    using fn_t = function_type;
    static const auto g
      = vec(hvec(true)
            (hvec(opr::paren, 1,
                  fn_t([](const stack_type &s) {return back(s);}))
             ("(", "", ")")
             ),

            hvec(true)
            (hvec(opr::pos, 1,
                  fn_t([](const stack_type &s) {return back(s);}))
             ("+", ""),
             hvec(opr::neg, 1,
                  fn_t([](const stack_type &s) {return -back(s);}))
             ("-", "")
             ),

            hvec(true)
            (hvec(opr::pow, 2,
                  fn_t([](const stack_type &s) {
                    return pow(*prev(s.end(), 2), back(s));
                  }))
             ("", "^", "")
             ),

            hvec(true)
            (hvec(opr::mul, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) * back(s);
                  }))
             ("", "*", ""),
             hvec(opr::div, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) / back(s);
                  }))
             ("", "/", "")
             ),

            hvec(true)
            (hvec(opr::add, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) + back(s);
                  }))
             ("", "+", ""),
             hvec(opr::sub, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) - back(s);
                  }))
             ("", "-", "")
             ),

            hvec(false)
            (hvec(opr::cond, 3,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 3) ? *prev(s.end(), 2) : back(s);
                  }))
             ("", "?", "", ":", ""))
            );
    return g;
  }

private:
  flat_map<string, flat_map<int, function_type>> m;
  void init_m() {
    using fn_t = function_type;
    m["plus"] = {
      {1, fn_t([](const stack_type &s) {return back(s);})},
      {2, fn_t([](const stack_type &s) {return *prev(s.end(), 2) + back(s);})}
    };
    m["minus"] = {
      {1, fn_t([](const stack_type &s) {return -back(s);})},
      {2, fn_t([](const stack_type &s) {return *prev(s.end(), 2) - back(s);})}
    };
    m["abs"] = {
      {1, fn_t([](const stack_type &s) {return abs(back(s));})},
    };
  }
public:
  const auto &function_list() const {
    return m;
  }

  static optional<operand_type> scan_operand(view_type &v) {
    operand_type ret;
    if (sscan(v, ret))
      return ret;
    else
      return nullopt;
  }

  g_t() {
    init_m();
  }
  ~g_t() = default;
  g_t(const g_t &) = delete;
  g_t &operator =(const g_t &) = delete;
  g_t(g_t &&) noexcept = default;
  g_t &operator =(g_t &&) noexcept = default;
};
void test_arithmetic_parser() {
  // arithmetic_parser
  {
    auto scan_f = arithmetic_parser<g_t>{};

    sview v = "((-(-(3*2)+1+(-2-(2+0.5+3*4)))+3)+-2-22.5 ? 1 : 5 ? 3 : 2)";
    auto x = scan_f(v);
    assert(x.has_value());
    assert(v.empty());
    assert(*x == ((-(-(3*2)+1+(-2-(2+0.5+3*4)))+3)+-2-22.5 ? 1 : 5 ? 3 : 2));

    v = "1 + ( 100 ) + 3";
    x = scan_f(v);
    assert(x.has_value());
    assert(v.empty());
    assert(*x == 104);

    v = "1+(100)+4 ";
    x = scan_f(v);
    assert(x.has_value());
    assert(v == " ");
    assert(*x == 105);

    v = "minus(plus(1, 2*3+2^2), minus(minus(plus(3, 4)))) ";
    const char *p{};
    x = scan_f(v, p);
    assert(x.has_value());
    assert(p == nullptr);
    assert(v == " ");
    assert(*x == 4);
  }
  // default_arithmetic_parser
  // default_arithmetic_parser_u32
  // sscan_arithmetic
  {
    // sview
    {
      sview v = "abs(1-2*3^2) xyz";
      assert(sscan_arithmetic(v).value() == 17.0);
      assert(v == " xyz");
    }
    // u32sview
    {
      u32sview v = U"abs(1-2*3^2) xyz";
      assert(sscan_arithmetic(v).value() == 17.0);
      assert(v == U" xyz");
    }
  }
}

// XXX_test_dir is initialized to inlcude three files
string get_path() {
  string dir(".\\");
  dir.append(sview(__FILE__));
  dir.erase(find_last(dir, '.'), dir.end());
  dir.append("_test_dir");
  return dir;
}
void test_file() {
  string path = get_path();
  wstring wpath;
  u16string u16path;
  u32string u32path;
  assert(wpath.try_assign_unicode(path));
  assert(u16path.try_assign_unicode(path));
  assert(u32path.try_assign_unicode(path));
  try_remove_file(copy(path).append("\\tmp.txt"));

  static_assert(movable<file>);
  static_assert(!is_copy_constructible<file>);
  static_assert(!is_copy_assignable<file>);

  const auto test_empty_f = [](auto v) {
    file f(v);
    assert(!f.empty());
    assert(f.read().empty());
    assert(f.read().empty());
  };
  const auto test_non_empty_f = [](auto v) {
    file f(v);
    assert(!f.empty());
    file f2 = move(f);
    assert(f.empty() && !f2.empty());
    assert(f2.read() == "abcde\n");
    assert(f2.read() == "abcde\n");
    f = move(f2);
    assert(f2.empty() && !f.empty());
    assert(f.read() == "abcde\n");
    assert(f.read() == "abcde\n");
    f = move(f);
    assert(f.read() == "abcde\n");
    assert(f.read() == "abcde\n");
    f.close();
    assert(f.empty() && f2.empty());
  };
  const auto test_read_only_f = [](auto v) {
    file f(v);
    assert(!f.empty());
    assert(f.read() == "abc\n");
    assert(f.read() == "abc\n");
  };
  const auto test_write_f = [](auto v) {
    {
      assert(!is_file(v));
      file f(v);
      assert(!f.empty());
      f.write("abcd");
      assert(f.read() == "abcd");
      f.write("ab");
      assert(f.read() == "ab");
      f.write("abcde");
      f.write("abcde");
      f.close();
      f.open(v);
      assert(f.read() == "abcde");
    }
    remove_file(v);
  };
  test_empty_f(sview(copy(path).append("\\empty_f")));
  test_empty_f(wsview(copy(wpath).append(L"\\empty_f")));
  test_empty_f(u16sview(copy(u16path).append(u"\\empty_f")));
  test_empty_f(u32sview(copy(u32path).append(U"\\empty_f")));
  test_non_empty_f(sview(copy(path).append("\\6_byte.txt")));
  test_non_empty_f(wsview(copy(wpath).append(L"\\6_byte.txt")));
  test_non_empty_f(u16sview(copy(u16path).append(u"\\6_byte.txt")));
  test_non_empty_f(u32sview(copy(u32path).append(U"\\6_byte.txt")));
  test_read_only_f(sview(copy(path).append("\\read_only_abc.txt")));
  test_read_only_f(wsview(copy(wpath).append(L"\\read_only_abc.txt")));
  test_read_only_f(u16sview(copy(u16path).append(u"\\read_only_abc.txt")));
  test_read_only_f(u32sview(copy(u32path).append(U"\\read_only_abc.txt")));
  test_write_f(sview(copy(path).append("\\tmp.txt")));
  test_write_f(wsview(copy(wpath).append(L"\\tmp.txt")));
  test_write_f(u16sview(copy(u16path).append(u"\\tmp.txt")));
  test_write_f(u32sview(copy(u32path).append(U"\\tmp.txt")));
}
void test_file_ralated_fns() {
  string path = get_path();
  wstring wpath;
  u16string u16path;
  u32string u32path;
  assert(wpath.try_assign_unicode(path));
  assert(u16path.try_assign_unicode(path));
  assert(u32path.try_assign_unicode(path));
  try_remove_file(copy(path).append("\\tmp_dir"));
  try_remove_file(copy(path).append("\\a"));
  try_remove_file(copy(path).append("\\time0.txt"));
  try_remove_file(copy(path).append("\\time1.txt"));
  try_remove_file(copy(path).append("\\rename_f.txt"));
  try_remove_file(copy(path).append("\\rename_ff.txt"));

  // is_file
  // is_directory
  {
    const auto test_dir0 = [](auto v) {
      assert(is_file(v) && is_directory(v));
    };
    const auto test_dir = [=](auto v1, auto v2, auto v3, auto v4) {
      test_dir0(v1);
      test_dir0(v2);
      test_dir0(v3);
      test_dir0(v4);
    };
    const auto test_file0 = [](auto v) {
      assert(is_file(v) && !is_directory(v));
    };
    const auto test_file = [=](auto v1, auto v2, auto v3, auto v4) {
      test_file0(v1);
      test_file0(v2);
      test_file0(v3);
      test_file0(v4);
    };
    const auto test_unexisted0 = [](auto v) {
      assert(!is_file(v) && !is_directory(v));
    };
    const auto test_unexisted = [=](auto v1, auto v2, auto v3, auto v4) {
      test_unexisted0(v1);
      test_unexisted0(v2);
      test_unexisted0(v3);
      test_unexisted0(v4);
    };

    apply(test_dir, tuple(sview(path),
                          wsview(wpath),
                          u16sview(u16path),
                          u32sview(u32path)));

    auto f = copy(path).append("\\6_byte.txt");
    auto wf = copy(wpath).append(L"\\6_byte.txt");
    auto u16f = copy(u16path).append(u"\\6_byte.txt");
    auto u32f = copy(u32path).append(U"\\6_byte.txt");
    apply(test_file,
          tuple(sview(f), wsview(wf), u16sview(u16f), u32sview(u32f)));

    auto i = copy(path).append("\\zzzzzzzzzzzz");
    auto wi = copy(wpath).append(L"\\zzzzzzzzzzzz");
    auto u16i = copy(u16path).append(u"\\zzzzzzzzzzzz");
    auto u32i = copy(u32path).append(U"\\zzzzzzzzzzzz");
    apply(test_unexisted,
          tuple(sview(i), wsview(wi), u16sview(u16i), u32sview(u32i)));
  }

  // try_create_directory
  // create_directory
  {
    const auto test_f = [](auto v) {
      assert(!is_file(v));
      create_directory(v);
      assert(is_directory(v));
      assert(try_create_directory(v) == false);
      assert(is_directory(v));
      assert(try_remove_file(v) == true);
      assert(try_remove_file(v) == false);
      assert(!is_file(v));
      create_directory(v);
      assert(is_directory(v));
      remove_file(v);
      assert(!is_file(v));
    };
    test_f(sview(copy(path).append("\\tmp_dir")));
    test_f(wsview(copy(wpath).append(L"\\tmp_dir")));
    test_f(u16sview(copy(u16path).append(u"\\tmp_dir")));
    test_f(u32sview(copy(u32path).append(U"\\tmp_dir")));
  }

  // file_size (non-directory size)
  // file_time
  {
    {
      file f(copy(path).append("\\time0.txt"));
    }
    inner::fns::win32_sleep_ms(100);
    {
      file f2(copy(path).append("\\time1.txt"));
      f2.write("abc\n");
    }
    assert(file_size(copy(path).append("\\time0.txt")) == 0u);
    assert(file_size(copy(wpath).append(L"\\time0.txt")) == 0u);
    assert(file_size(copy(u16path).append(u"\\time0.txt")) == 0u);
    assert(file_size(copy(u32path).append(U"\\time0.txt")) == 0u);
    assert(file_size(copy(path).append("\\time1.txt")) == 4u);
    assert(file_size(copy(wpath).append(L"\\time1.txt")) == 4u);
    assert(file_size(copy(u16path).append(u"\\time1.txt")) == 4u);
    assert(file_size(copy(u32path).append(U"\\time1.txt")) == 4u);
    assert(duration_ceil<hours>
           (file_time(copy(path).append("\\time1.txt"))
            - file_time(copy(path).append("\\time0.txt")))
           .count() == 1);
    assert(duration_ceil<hours>
           (file_time(copy(wpath).append(L"\\time1.txt"))
            - file_time(copy(wpath).append(L"\\time0.txt")))
           .count() == 1);
    assert(duration_ceil<hours>
           (file_time(copy(u16path).append(u"\\time1.txt"))
            - file_time(copy(u16path).append(u"\\time0.txt")))
           .count() == 1);
    assert(duration_ceil<hours>
           (file_time(copy(u32path).append(U"\\time1.txt"))
            - file_time(copy(u32path).append(U"\\time0.txt")))
           .count() == 1);
    remove_file(copy(path).append("\\time0.txt"));
    remove_file(copy(path).append("\\time1.txt"));
  }

  // try_remove_file (remove non-directory)
  // remove_file (remove non-directory)
  {
    assert(try_remove_file(copy(path).append("\\a")) == false);
    assert(try_remove_file(copy(wpath).append(L"\\a")) == false);
    assert(try_remove_file(copy(u16path).append(u"\\a")) == false);
    assert(try_remove_file(copy(u32path).append(U"\\a")) == false);

    {
      {
        file f(copy(path).append("\\a"));
      }
      const auto s = copy(path).append("\\a");
      assert(is_file(s));
      assert(try_remove_file(s) == true);
      assert(!is_file(s));
      {
        {
          file f(copy(path).append("\\a"));
        }
        assert(is_file(s));
        remove_file(s);
        assert(!is_file(s));
      }
    }
    {
      {
        file f(copy(path).append("\\a"));
      }
      const auto s = copy(wpath).append(L"\\a");
      assert(is_file(s));
      assert(try_remove_file(s) == true);
      assert(!is_file(s));
      {
        {
          file f(copy(path).append("\\a"));
        }
        assert(is_file(s));
        remove_file(s);
        assert(!is_file(s));
      }
    }
    {
      {
        file f(copy(path).append("\\a"));
      }
      const auto s = copy(u16path).append(u"\\a");
      assert(is_file(s));
      assert(try_remove_file(s) == true);
      assert(!is_file(s));
      {
        {
          file f(copy(path).append("\\a"));
        }
        assert(is_file(s));
        remove_file(s);
        assert(!is_file(s));
      }
    }
    {
      {
        file f(copy(path).append("\\a"));
      }
      const auto s = copy(u32path).append(U"\\a");
      assert(is_file(s));
      assert(try_remove_file(s) == true);
      assert(!is_file(s));
      {
        {
          file f(copy(path).append("\\a"));
        }
        assert(is_file(s));
        remove_file(s);
        assert(!is_file(s));
      }
    }
  }

  // try_rename_file
  // rename_file
  {
    assert(!try_rename_file(copy(path).append("\\zzzzz"), "zzzzzz"));
    assert(!try_rename_file(copy(wpath).append(L"\\zzzzz"), L"zzzzzz"));
    assert(!try_rename_file(copy(u16path).append(u"\\zzzzz"), u"zzzzzz"));
    assert(!try_rename_file(copy(u32path).append(U"\\zzzzz"), U"zzzzzz"));

    file f(copy(path).append("\\rename_f.txt"));
    f.close();
    assert(is_file(copy(path).append("\\rename_f.txt")));
    assert(try_rename_file(copy(path).append("\\rename_f.txt"),
                           "rename_f.txt"));
    assert(is_file(copy(path).append("\\rename_f.txt")));
    assert(try_rename_file(copy(path).append("\\rename_f.txt"),
                           "rename_ff.txt"));
    assert(!is_file(copy(path).append("\\rename_f.txt")));
    assert(is_file(copy(path).append("\\rename_ff.txt")));

    remove_file(copy(path).append("\\rename_ff.txt"));
  }

  // to_full_path
  {
    {
      const auto s = copy(path).append("\\6_byte.txt");
      const auto s2 = to_full_path(s);
      assert(file_time(s) == file_time(s2));
      const auto a = file(s).read();
      const auto b = file(s2).read();
      assert(a == b);
    }
    {
      const auto s = copy(wpath).append(L"\\6_byte.txt");
      const auto s2 = to_full_path(s);
      assert(file_time(s) == file_time(s2));
      const auto a = file(s).read();
      const auto b = file(s2).read();
      assert(a == b);
    }
    {
      const auto s = copy(u16path).append(u"\\6_byte.txt");
      const auto s2 = to_full_path(s);
      assert(file_time(s) == file_time(s2));
      const auto a = file(s).read();
      const auto b = file(s2).read();
      assert(a == b);
    }
    {
      const auto s = copy(u32path).append(U"\\6_byte.txt");
      const auto s2 = to_full_path(s);
      assert(file_time(s) == file_time(s2));
      const auto a = file(s).read();
      const auto b = file(s2).read();
      assert(a == b);
    }
  }
  // simplify_path
  {
    assert(simplify_path("\\\\////") == "\\");
    assert(simplify_path("./abc////\\abc////\\\\abc\\\\////")
           == ".\\abc\\abc\\abc");
  }
  // remove_path_last_name
  {
    assert(remove_path_last_name("\\\\////") == "\\\\////"); // no last name
    assert(remove_path_last_name("./abc///bcd///\\\\") == "./abc///");
    assert(remove_path_last_name("./abc/") == "./");
  }
  // replace_path_last_name
  {
    assert(replace_path_last_name("./abc", "d") == "./d");
    assert(replace_path_last_name(".///abc", "d") == ".///d");
    assert(replace_path_last_name("./abc////abc\\///", "d") == "./abc////d");
  }

  // view_file
  // view_directory
  // try_remove_file (for directory)
  // remove_file (for directory)
  // file_size (for directory)
  {
    // view_file (for non-directory)
    {
      {
        auto x = view_file(copy(path).append("\\6_byte.txt"));
        assert(path_last_name(x.path) == x.name);
        assert(x.name == "6_byte.txt");
        assert(x.size == 6u);
        assert(x.time.count() != 0);
        assert(x.is_dir == false);
      }
      {
        auto x = view_file(copy(wpath).append(L"\\6_byte.txt"));
        assert(path_last_name(x.path) == x.name);
        assert(x.name == L"6_byte.txt");
        assert(x.size == 6u);
        assert(x.time.count() != 0);
        assert(x.is_dir == false);
      }
      {
        auto x = view_file(copy(u16path).append(u"\\6_byte.txt"));
        assert(path_last_name(x.path) == x.name);
        assert(x.name == u"6_byte.txt");
        assert(x.size == 6u);
        assert(x.time.count() != 0);
        assert(x.is_dir == false);
      }
      {
        auto x = view_file(copy(u32path).append(U"\\6_byte.txt"));
        assert(path_last_name(x.path) == x.name);
        assert(x.name == U"6_byte.txt");
        assert(x.size == 6u);
        assert(x.time.count() != 0);
        assert(x.is_dir == false);
      }
    }
    // view_file (for directory)
    {
      auto x = view_file(path);
      assert(path_last_name(x.path) == x.name);
      assert(x.size == 10u);
      assert(x.time.count() != 0);
      assert(x.is_dir == true);
    }

    // view_directory (height 1)
    {
      const auto test_f = [](auto t) {
        assert(!t.empty());
        assert(path_last_name((*t.root())->path) == (*t.root())->name);
        assert((*t.root())->size == 10u);
        assert((*t.root())->time.count() != 0);
        assert((*t.root())->is_dir);

        auto &l = *t.root();
        assert(l.size() == 3u);
        ez_vector<typename decay<decltype(t)>::key_type::string_type> v, v2;
        for (int i : irng(0, 3))
          v.insert(v.end(), l[i]->name);
        v2.insert(v2.end(), {});
        v2.insert(v2.end(), {});
        v2.insert(v2.end(), {});
        assert(ref(v2, 0).try_assign_unicode("6_byte.txt"));
        assert(ref(v2, 1).try_assign_unicode("empty_f"));
        assert(ref(v2, 2).try_assign_unicode("read_only_abc.txt"));
        assert(is_permutation(v, v2));
        assert(all_of(l, [](auto &x) {return x->is_dir == false;}));
        assert(all_of(l, [](auto &x) {return x->time.count() != 0;}));
        assert(any_of(l, [](auto &x) {return x->size != 0u;}));
        assert(any_of(l, [](auto &x) {return x->size == 0u;}));
      };
      test_f(view_directory(path));
      test_f(view_directory(wpath));
      test_f(view_directory(u16path));
      test_f(view_directory(u32path));
    }
    // view_directory (height more than 1)
    {
      assert(try_create_directory(copy(path).append("\\a")));
      assert(try_create_directory(copy(path).append("\\a\\b1")));
      assert(try_create_directory(copy(path).append("\\a\\b2")));
      assert(try_create_directory(copy(path).append("\\a\\b3")));
      file f;
      f.open(copy(path).append("\\a\\b4.txt"));
      f.write("x");
      f.close();
      f.open(copy(path).append("\\a\\b5.txt"));
      f.write("x");
      f.close();

      assert(try_create_directory(copy(path).append("\\a\\b1\\c1")));
      assert(try_create_directory(copy(path).append("\\a\\b1\\c2")));
      assert(try_create_directory(copy(path).append("\\a\\b2\\c3")));
      f.open(copy(path).append("\\a\\b3\\c4.txt"));
      f.write("x");
      f.close();

      f.open(copy(path).append("\\a\\b1\\c2\\d1.txt"));
      f.write("x");
      f.close();
      f.open(copy(path).append("\\a\\b1\\c2\\d2.txt"));
      f.write("x");
      f.close();
      assert(try_create_directory(copy(path).append("\\a\\b2\\c3\\d3")));

      f.open(copy(path).append("\\a\\b2\\c3\\d3\\e1.txt"));
      f.write("x");
      f.close();

      auto t = view_directory(copy(path).append("\\a"));
      assert(!t.empty());
      assert(path_last_name((*t.root())->path) == (*t.root())->name);
      assert((*t.root())->name == "a");
      assert((*t.root())->size == 6u);
      assert((*t.root())->time.count() != 0);
      assert((*t.root())->is_dir);

      t.pre_order_sort(t.root(), [](const auto &x, const auto &y) {
        return x->name < y->name;
      });
      const auto get_name = [](auto &x)->const auto & {return x->name;};
      const auto get_size = [](auto &x) {return x->size;};
      assert(equal(bind_rng(*t.root(), get_name),
                   seq("b1"_sv, "b2", "b3", "b4.txt", "b5.txt")));
      assert(equal(bind_rng(*t.root(), get_size),
                   seq(2u, 1u, 1u, 1u, 1u)));
      assert(equal(bind_rng(ref(*t.root(), 0), get_name),
                   seq("c1"_sv, "c2")));
      assert(equal(bind_rng(ref(*t.root(), 0), get_size),
                   seq(0u, 2u)));
      assert(equal(bind_rng(ref(*t.root(), 1), get_name),
                   seq("c3"_sv)));
      assert(equal(bind_rng(ref(*t.root(), 1), get_size),
                   seq(1u)));
      assert(equal(bind_rng(ref(*t.root(), 2), get_name),
                   seq("c4.txt"_sv)));
      assert(equal(bind_rng(ref(*t.root(), 2), get_size),
                   seq(1u)));
      assert(ref(*t.root(), 3).empty());
      assert(ref(*t.root(), 4).empty());

      assert(equal(bind_rng(ref(ref(*t.root(), 0), 1), get_name),
                   seq("d1.txt"_sv, "d2.txt")));
      assert(equal(bind_rng(ref(ref(*t.root(), 0), 1), get_size),
                   seq(1u, 1u)));

      assert(equal(bind_rng(ref(ref(*t.root(), 1), 0), get_name),
                   seq("d3"_sv)));
      assert(equal(bind_rng(ref(ref(*t.root(), 1), 0), get_size),
                   seq(1u)));

      assert(equal(bind_rng(ref(ref(ref(*t.root(), 1), 0), 0), get_name),
                   seq("e1.txt"_sv)));
      assert(equal(bind_rng(ref(ref(ref(*t.root(), 1), 0), 0), get_size),
                   seq(1u)));

      int c = 0;
      for (auto &x : t.root().pre_order()) {
        ++c;
        assert(path_last_name(x->path) == x->name);
        assert(x->time.count() != 0);
        if (x->is_dir)
          contains(seq("a"_sv, "b1", "b2", "b3", "c1", "c2", "c3", "d3"),
                   x->name);
        else
          contains(seq("b4.txt"_sv, "b5.txt", "c4.txt",
                       "d1.txt", "d2.txt", "e1.txt"),
                   x->name);
      }
      assert(c == 14);

      remove_file(copy(path).append("\\a"));
      assert(!is_file(copy(path).append("\\a")));
    }
  }
}

void test_io() {
  put("io: ");

  inner::test::test_c_file();
  inner::test::test_console_c_file();
  inner::test::test_fputc_iterator();
  inner::test::test_sscan();
  inner::test::test_sprint();
  inner::test::test_arithmetic_parser();
  inner::test::test_big_int();
  inner::test::test_sprint_floating_point();
  inner::test::test_sscan_floating_point();
  inner::test::test_sscan_sprint_floating_point_by_wide_string();
  inner::test::test_ssplitter();
  inner::test::test_ssplit();
  inner::test::test_file();
  inner::test::test_file_ralated_fns();

  putln("ok");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_io();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_and_terminate(e.what());
  }
#endif
}
