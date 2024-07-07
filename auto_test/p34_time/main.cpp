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

namespace re::inner::fns {

void test_duration() {
  // treat_as_floating
  {
    static_assert(!treat_as_floating_point<int>::value);
    static_assert(treat_as_floating_point<float>::value);
    static_assert(treat_as_floating_point<double>::value);
  }

  // inner::is_duration
  {
    static_assert(inner::is_duration<nanoseconds>::value);
    static_assert(inner::is_duration<const nanoseconds>::value);
    static_assert(!inner::is_duration<int>::value);
  }

  // common_type
  {
    static_assert(same_as<common_type_t<microseconds, seconds>, microseconds>);
    static_assert(same_as<common_type_t<microseconds, years>, microseconds>);
    static_assert(same_as<common_type_t<nanoseconds, years>, nanoseconds>);
    static_assert(same_as<common_type_t<days, weeks>, days>);
    static_assert(same_as<common_type_t<days, days>, days>);
  }

  // duration_values
  {
    static_assert(duration_values<int>::zero() == 0);
    static_assert(duration_values<int>::max() == numeric_limits<int>::max());
    static_assert(duration_values<int>::min() == numeric_limits<int>::min());
  }

  // rep
  // period
  {
    static_assert(is_same_v<typename seconds::rep, long long>);
    static_assert(seconds::period == ratio(1));
  }

  // smf
  {
    static_assert(regular<seconds>);
  }

  // explicit this_t(r2)
  // explicit this_t(const duration<R2, P2> &)
  {
    seconds s(1);
    milliseconds ms(s);
    assert(ms.count() == 1000);
  }

  // unary +
  // unary -
  {
    seconds s(1);
    seconds s2 = +s;
    seconds s3 = -s;
    assert(s.count() == 1 && s2.count() == 1 && s3.count() == -1);
  }

  // ++
  // --
  {
    seconds s(1);
    seconds s2 = ++s;
    assert(s.count() == 2 && s2.count() == 2);
    seconds s3 = s2++;
    assert(s3.count() == 2 && s2.count() == 3);
  }

  // +=
  // -=
  {
    seconds s(1);
    seconds s2(2);
    assert((s += s2) == seconds(3));
    assert((s -= s2) == seconds(1));
  }

  // *=
  // /=
  // %=
  {
    seconds s(2);
    s *= 3;
    assert(s.count() == 6);
    s /= 7;
    assert(s.count() == 0);
    s = 5_s;
    assert((s %= 2) == 1_s);
    s = 5_s;
    assert((s %= 2_s) == 1_s);
  }

  // zero()
  // min()
  // max()
  {
    assert(seconds::zero() == 0_s);
    assert(seconds::max() == 9223372036854775807_s);
    assert(seconds::min() == -seconds::max() - 1_s);
  }

  // duration_cast
  {
    // num != 1 && den != 1
    {
      using d1_t = duration<intmax_t, ratio(3, 7)>;
      using d2_t = duration<intmax_t, ratio(4, 5)>;
      const d1_t d(5);
      const d2_t d2 = duration_cast<d2_t>(d);
      assert(d2.count() == 2);
    }
    // num == 1 || den == 1
    {
      const seconds s(1);
      const milliseconds s2 = duration_cast<milliseconds>(s);
      assert(s2.count() == 1000);
      const seconds s3 = duration_cast<seconds>(s2);
      assert(s3.count() == 1);
    }
    // num == 1 && den == 1
    {
      const seconds s(3);
      const seconds s2 = duration_cast<seconds>(s);
      assert(s2.count() == 3);
    }
  }

  // floor
  // ceil
  // round
  {
    auto s = 60_min;
    assert(s.floor<hours>().count() == 1);
    assert(s.ceil<hours>().count() == 1);
    assert(s.round<hours>().count() == 1);
    s = 30_min;
    assert(s.floor<hours>().count() == 0);
    assert(s.ceil<hours>().count() == 1);
    assert(s.round<hours>().count() == 0);
    s = 150_min;
    assert(s.floor<hours>().count() == 2);
    assert(s.ceil<hours>().count() == 3);
    assert(s.round<hours>().count() == 2);
    s = 149_min;
    assert(s.floor<hours>().count() == 2);
    assert(s.ceil<hours>().count() == 3);
    assert(s.round<hours>().count() == 2);
    s = 151_min;
    assert(s.floor<hours>().count() == 2);
    assert(s.ceil<hours>().count() == 3);
    assert(s.round<hours>().count() == 3);

    assert(duration_floor<hours>(30_min).count() == 0);
    assert(duration_ceil<hours>(30_min).count() == 1);
    assert(duration_round<hours>(30_min).count() == 0);
    assert(duration_round<hours>(31_min).count() == 1);
  }

  // == <=>
  {
    assert(1_s == 1000_ms);
    assert(1_s == 1000000_us);
    assert((1_s <=> 1000000_us) == strong_eq);
    assert((1_s <=> 2_s) == strong_lt);
  }

  // +
  // -
  {
    assert(1_s + 100_ms == 1100_ms);
    assert(1_s - 100_ms == 900_ms);
  }

  // *
  // /
  {
    seconds s = 100_s;
    s = s * 2;
    assert(s == 200_s);
    s = 2 * s;
    assert(s == 400_s);
    s = s / 2;
    assert(s == 200_s);
    assert(s / 200_s == 1);
  }

  // %
  {
    assert(7_s % 4_s == 3_s);
    assert(7_s % 4 == 3_s);
  }

  // _h
  // _min
  // _s
  // _ms
  // _us
  // _ns
  {
    assert(1_h == 3600_s);
    assert(1_min == 60_s);
    assert(1_s == 1000_ms);
    assert(1_ms == 1000_us);
    assert(1_us == 1000_ns);

    assert(1.0_h == 3600_s);
    assert(1.0_min == 60_s);
    assert(1.0_s == 1000_ms);
    assert(1.0_ms == 1000_us);
    assert(1.0_us == 1000_ns);
  }

  // abs
  {
    seconds s = 1_s;
    assert(-s != s);
    assert((-s).abs() == s);
    assert(abs(-s) == s);
  }
}
void test_time_point() {
  // clock
  // duration
  // rep
  // period
  {
    using t = time_point<system_clock, seconds>;
    static_assert(is_same_v<t::clock, system_clock>);
    static_assert(is_same_v<t::duration, seconds>);
    static_assert(is_same_v<t::rep, long long>);
    static_assert(t::period == ratio(1));
  }
  // smf
  {
    using t = time_point<system_clock, seconds>;
    static_assert(regular<t>);
    t x;
    assert(x.time_since_epoch().count() == 0);
  }
  // explicit time_point(d)
  {
    using t = time_point<system_clock, seconds>;
    t x(2_s);
    static_assert(!is_convertible_v<seconds, t>);
    assert(x.time_since_epoch() == 2_s);
  }
  // time_point(const time_point<C, D2> &)
  {
    using t2 = time_point<system_clock, duration<float, ratio(1)>>;
    using t3 = time_point<system_clock, duration<float, ratio(1, 1000)>>;
    t2 x;
    t3 y = x;
    assert(x.time().count() == 0);
    assert(y.time().count() == 0);

    static_assert(!is_convertible_v
                  <time_point<system_clock, milliseconds>,
                   time_point<system_clock, seconds>>);
  }
  // ++
  // --
  {
    using t = time_point<system_clock, seconds>;
    t x;
    ++x;
    assert(x.time().count() == 1);
    assert((x++).time().count() == 1);
    assert(x.time().count() == 2);
    assert((--x).time().count() == 1);
    assert((x--).time().count() == 1);
    assert(x.time().count() == 0);
  }
  // +=
  // -=
  {
    using t = time_point<system_clock, seconds>;
    t x;
    x += 1_s;
    assert(x.time().count() == 1);
    x -= 1_s;
    assert(x.time().count() == 0);
  }
  // min()
  // max()
  {
    using t = time_point<system_clock, seconds>;
    static_assert(t::min().time().count() == numeric_limits<long long>::min());
    static_assert(t::max().time().count() == numeric_limits<long long>::max());
  }
  // floor
  // ceil
  // round
  {
    using t = time_point<system_clock, seconds>;
    t x(30_s);
    assert(x.floor<hours>().time().count() == 0);
    assert(x.ceil<hours>().time().count() == 1);
    assert(x.round<hours>().time().count() == 0);

    assert(time_point_ceil<minutes>(x).time().count() == 1);
    assert(time_point_floor<minutes>(x).time().count() == 0);
    assert(time_point_round<minutes>(x).time().count() == 0);
  }
  // time_point_cast
  {
    using t = time_point<system_clock, seconds>;
    t x(60_s);
    assert(time_point_cast<minutes>(x).time().count() == 1);
  }
  // ==
  // <=>
  {
    using t = time_point<system_clock, seconds>;
    using t2 = time_point<system_clock, minutes>;
    t x(60_s);
    t2 y(1_min);
    t2 z(2_min);
    assert(x == x && x == y && y != z);
    assert((x <=> z) == strong_lt);
    assert((z <=> z) == strong_eq);
  }
  // +
  {
    using t = time_point<system_clock, seconds>;
    t x(60_s);
    assert((x + 10_s) == t(70_s));
    assert((10_s + x) == t(70_s));
    assert((x + 1_min) == t(2_min));
    assert((1_min + x) == t(2_min));
  }
  // -
  {
    using t = time_point<system_clock, seconds>;
    t x(60_s);
    assert(x - 30_s == t(30_s));
    assert(x - x == 0_s);
  }
}

void test_time() {
  put("time: ");

  inner::fns::test_duration();
  inner::fns::test_time_point();

  putln("ok");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_time();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
