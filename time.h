#ifndef RE_TIME_H
#define RE_TIME_H

#include "base.h"

namespace re::inner::fns {

inline unsigned long long win32_get_system_time_precise_as_file_time() {
  FILETIME ft;
  GetSystemTimePreciseAsFileTime(addressof(ft));
  return bit_cast<unsigned long long>(ft);
}
inline unsigned long long win32_query_performance_counter() {
  LARGE_INTEGER x{};
  if (QueryPerformanceCounter(addressof(x)) == 0)
    throw_or_terminate<runtime_error>
      ("re::inner::fns::win32_query_performance_counter(): failed\n");
  return static_cast<unsigned long long>(x.QuadPart);
}
inline unsigned long long win32_query_performance_frequency() {
  LARGE_INTEGER x{};
  if (QueryPerformanceFrequency(addressof(x)) == 0)
    throw_or_terminate<runtime_error>
      ("re::inner::fns::win32_query_performance_frequency(): failed\n");
  return static_cast<unsigned long long>(x.QuadPart);
}
inline void win32_sleep_ms(uint32_t x) {
  Sleep(x);
}

}

// duration
namespace re {

template <class R>
struct treat_as_floating_point : false_type {};
template <>
struct treat_as_floating_point<float> : true_type {};
template <>
struct treat_as_floating_point<double> : true_type {};
template <class R>
inline constexpr bool treat_as_floating_point_v
  = treat_as_floating_point<R>::value;

template <class R, ratio P = ratio(1, 1)>
class duration;
namespace inner {

template <class>
struct is_duration : false_type {};
template <class R, ratio P>
struct is_duration<duration<R, P>> : true_type {};
template <class R, ratio P>
struct is_duration<const duration<R, P>> : true_type {};

}
template <class R1, ratio P1, class R2, ratio P2>
struct template_common_type<duration<R1, P1>, duration<R2, P2>> {
  using type = duration<common_type<R1, R2>,
                        ratio(inner::fns::constexpr_gcd(P1.num, P2.num),
                              inner::fns::constexpr_lcm(P1.den, P2.den))>;
};
using nanoseconds = duration<long long, nano>;
using microseconds = duration<long long, micro>;
using milliseconds = duration<long long, milli>;
using seconds = duration<long long>;
using minutes = duration<long long, ratio(60)>;
using hours = duration<long long, ratio(3600)>;
using days = duration<long long, ratio(86400)>;
using weeks = duration<long long, ratio(604800)>;
using months = duration<long long, ratio(2629746)>;
using years = duration<long long, ratio(31556952)>;

template <class R>
struct duration_values {
  static constexpr R zero() noexcept {
    return R(0);
  }
  static constexpr R min() noexcept {
    return numeric_limits<R>::lowest();
  }
  static constexpr R max() noexcept {
    return numeric_limits<R>::max();
  }
};

template <class R, ratio P>
class duration {
  R r;

public:
  using rep = R;
  static constexpr ratio period = P;

  duration() = default;
  ~duration() = default;
  duration(const duration &) = default;
  duration &operator =(const duration &) = default;
  duration(duration &&) = default;
  duration &operator =(duration &&) = default;

  template <class R2>
  constexpr explicit duration(const R2 &x)
    requires ((treat_as_floating_point_v<rep>
               && is_convertible<const R2 &, rep>)
              || !treat_as_floating_point_v<R2>)
    : r(x) {}
  constexpr rep count() const {
    return r;
  }

  template <class R2, ratio P2>
  constexpr duration(const duration<R2, P2> &x)
    requires (treat_as_floating_point_v<R> || (P2 / P).den == 1);

  constexpr common_type<duration> operator +() const {
    return common_type<duration>(*this);
  }
  constexpr common_type<duration> operator -() const {
    return common_type<duration>(-r);
  }
  constexpr duration &operator ++() {
    ++r;
    return *this;
  }
  constexpr duration operator ++(int) {
    return duration(r++);
  }
  constexpr duration &operator --() {
    --r;
    return *this;
  }
  constexpr duration operator --(int) {
    return duration(r--);
  }
  constexpr duration &operator +=(const duration &d) {
    r += d.count();
    return *this;
  }
  constexpr duration &operator -=(const duration &d) {
    r -= d.count();
    return *this;
  }

  constexpr duration &operator *=(const rep &x) {
    r *= x;
    return *this;
  }
  constexpr duration &operator /=(const rep &x) {
    r /= x;
    return *this;
  }
  constexpr duration &operator %=(const rep &x) {
    r %= x;
    return *this;
  }
  constexpr duration &operator %=(const duration &x) {
    r %= x.count();
    return *this;
  }

  static constexpr duration zero() noexcept {
    return duration(duration_values<rep>::zero());
  }
  static constexpr duration min() noexcept {
    return duration(duration_values<rep>::min());
  }
  static constexpr duration max() noexcept {
    return duration(duration_values<rep>::max());
  }

  template <class T>
  constexpr T floor() const
    requires (inner::is_duration<T>::value
              && !treat_as_floating_point_v<typename T::rep>);
  template <class T>
  constexpr T ceil() const
    requires (inner::is_duration<T>::value
              && !treat_as_floating_point_v<typename T::rep>);
  template <class T>
  constexpr T round() const
    requires (inner::is_duration<T>::value
              && !treat_as_floating_point_v<typename T::rep>);

  constexpr duration abs() const requires numeric_limits<R>::is_signed {
    return *this > zero() ? *this : -*this;
  }
};

template <class T>
struct fo_duration_cast {};
template <class R, ratio P>
struct fo_duration_cast<duration<R, P>> {
  template <class R2, ratio P2>
  duration<R, P> operator ()(const duration<R2, P2> &x) const {
    constexpr ratio P3 = P2 / P;
    using t = duration<R, P>;
    using rep_t = typename t::rep;
    using common_rep_t = common_type<rep_t, R2, intmax_t>;

    if constexpr (P3.num == 1 && P3.den == 1) {
      return t(static_cast<rep_t>(x.count()));
    }
    else if (P3.num != 1 && P3.den == 1) {
      return t(static_cast<rep_t>
               (static_cast<common_rep_t>(x.count())
                * static_cast<common_rep_t>(P3.num)));
    }
    else if (P3.num == 1 && P3.den != 1) {
      return t(static_cast<rep_t>
               (static_cast<common_rep_t>(x.count())
                / static_cast<common_rep_t>(P3.den)));
    }
    else {
      return t(static_cast<rep_t>
               (static_cast<common_rep_t>(x.count())
                * static_cast<common_rep_t>(P3.num)
                / static_cast<common_rep_t>(P3.den)));
    }
  }
};
template <class T>
inline constexpr fo_duration_cast<T> duration_cast{};

template <class R, ratio P>
template <class R2, ratio P2>
constexpr duration<R, P>::duration(const duration<R2, P2> &x)
  requires (treat_as_floating_point_v<R> || (P2 / P).den == 1) {
  r = duration_cast<duration>(x).count();
}

template <class R, ratio P>
template <class T>
constexpr T duration<R, P>::floor() const
  requires (inner::is_duration<T>::value
            && !treat_as_floating_point_v<typename T::rep>) {
  const T tmp = duration_cast<T>(*this);
  return (tmp > *this) ? (tmp - T(1)) : tmp;
}
template <class R, ratio P>
template <class T>
constexpr T duration<R, P>::ceil() const
  requires (inner::is_duration<T>::value
            && !treat_as_floating_point_v<typename T::rep>) {
  const T tmp = duration_cast<T>(*this);
  return (tmp < *this) ? (tmp + T(1)) : tmp;
}
template <class R, ratio P>
template <class T>
constexpr T duration<R, P>::round() const
  requires (inner::is_duration<T>::value
            && !treat_as_floating_point_v<typename T::rep>) {
  const T t0 = this->template floor<T>();
  const T t1 = t0 + T(1);
  const auto diff0 = *this - t0;
  const auto diff1 = t1 - *this;
  if (diff0 == diff1)
    return ((to_unsigned(t0.count()) & 1u) == 0u) ? t0 : t1;
  else if (diff0 < diff1)
    return t0;
  else
    return t1;
}
template <class T>
struct fo_duration_floor {
  template <class R, ratio P>
  auto operator ()(const duration<R, P> &x) const
    ->decltype(x.template floor<T>()) {
    return x.template floor<T>();
  }
};
template <class T>
inline constexpr fo_duration_floor<T> duration_floor{};
template <class T>
struct fo_duration_ceil {
  template <class R, ratio P>
  auto operator ()(const duration<R, P> &x) const
    ->decltype(x.template ceil<T>()) {
    return x.template ceil<T>();
  }
};
template <class T>
inline constexpr fo_duration_ceil<T> duration_ceil{};
template <class T>
struct fo_duration_round {
  template <class R, ratio P>
  auto operator ()(const duration<R, P> &x) const
    ->decltype(x.template round<T>()) {
    return x.template round<T>();
  }
};
template <class T>
inline constexpr fo_duration_round<T> duration_round{};

template <class R, ratio P, class R2, ratio P2>
constexpr bool operator ==(const duration<R, P> &x,
                           const duration<R2, P2> &y) {
  using ct = common_type<duration<R, P>, duration<R2, P2>>;
  return ct(x).count() == ct(y).count();
}
template <class R, ratio P, class R2, ratio P2>
constexpr synth_3way_result<typename
                            common_type<duration<R, P>, duration<R2, P2>>
                            ::rep>
operator <=>(const duration<R, P> &x, const duration<R2, P2> &y) {
  using ct = common_type<duration<R, P>, duration<R2, P2>>;
  return synth_3way(ct(x).count(), ct(y).count());
}

template <class R1, ratio P1, class R2, ratio P2>
constexpr common_type<duration<R1, P1>, duration<R2, P2>>
operator +(const duration<R1, P1> &x, const duration<R2, P2> &y) {
  using ret_t = common_type<duration<R1, P1>, duration<R2, P2>>;
  return ret_t(ret_t(x).count() + ret_t(y).count());
}

template <class R1, ratio P1, class R2, ratio P2>
constexpr common_type<duration<R1, P1>, duration<R2, P2>>
operator -(const duration<R1, P1> &x, const duration<R2, P2> &y) {
  using ret_t = common_type<duration<R1, P1>, duration<R2, P2>>;
  return ret_t(ret_t(x).count() - ret_t(y).count());
}

template <class R1, ratio P1, class R2>
constexpr duration<common_type<R1, R2>, P1>
operator *(const duration<R1, P1> &x, const R2 &k)
  requires is_convertible<const R2 &, common_type<R1, R2>> {
  using ret_t = duration<common_type<R1, R2>, P1>;
  return ret_t(ret_t(x).count() * k);
}

template <class R2, class R1, ratio P1>
constexpr duration<common_type<R2, R1>, P1>
operator *(const R2 &k, const duration<R1, P1> &x)
  requires is_convertible<const R2 &, common_type<R2, R1>> {
  return x * k;
}

template <class R1, ratio P1, class R2>
constexpr duration<common_type<R1, R2>, P1>
operator /(const duration<R1, P1> &d, const R2 &k)
  requires is_convertible<const R2 &, common_type<R1, R2>> {
  using ret_t = duration<common_type<R1, R2>, P1>;
  return ret_t(ret_t(d).count() / k);
}

template <class R1, ratio P1, class R2, ratio P2>
constexpr common_type<R1, R2>
operator /(const duration<R1, P1> &x, const duration<R2, P2> &y) {
  using t = common_type<duration<R1, P1>, duration<R2, P2>>;
  return t(x).count() / t(y).count();
}

template <class R1, ratio P1, class R2>
constexpr duration<common_type<R1, R2>, P1>
operator %(const duration<R1, P1> &d, const R2 &k)
  requires (is_convertible<const R2 &, common_type<R1, R2>>
            && !inner::is_duration<R2>::value) {
  using ret_t = duration<common_type<R1, R2>, P1>;
  return ret_t(ret_t(d).count() % k);
}

template <class R1, ratio P1, class R2, ratio P2>
constexpr common_type<duration<R1, P1>, duration<R2, P2>>
operator %(const duration<R1, P1> &x, const duration<R2, P2> &y) {
  using ret_t = common_type<duration<R1, P1>, duration<R2, P2>>;
  return ret_t(ret_t(x).count() % ret_t(y).count());
}

constexpr hours operator""_h(unsigned long long x) {
  return hours(static_cast<long long>(x));
}
constexpr minutes operator""_min(unsigned long long x) {
  return minutes(static_cast<long long>(x));
}
constexpr seconds operator""_s(unsigned long long x) {
  return seconds(static_cast<long long>(x));
}
constexpr milliseconds operator""_ms(unsigned long long x) {
  return milliseconds(static_cast<long long>(x));
}
constexpr microseconds operator""_us(unsigned long long x) {
  return microseconds(static_cast<long long>(x));
}
constexpr nanoseconds operator""_ns(unsigned long long x) {
  return nanoseconds(static_cast<long long>(x));
}

constexpr duration<double, ratio(3600)> operator""_h(long double x) {
  return duration<double, ratio(3600)>(static_cast<double>(x));
}
constexpr duration<double, ratio(60)> operator""_min(long double x) {
  return duration<double, ratio(60)>(static_cast<double>(x));
}
constexpr duration<double> operator""_s(long double x) {
  return duration<double>(static_cast<double>(x));
}
constexpr duration<double, milli> operator""_ms(long double x) {
  return duration<double, milli>(static_cast<double>(x));
}
constexpr duration<double, micro> operator""_us(long double x) {
  return duration<double, micro>(static_cast<double>(x));
}
constexpr duration<double, nano> operator""_ns(long double x) {
  return duration<double, nano>(static_cast<double>(x));
}

}

// time_point
// system_clock
// steady_clock
// scoped_timer
namespace re {

template <class C, class D = typename C::duration>
class time_point {
public:
  using clock = C;
  using duration = D;
  using rep = typename duration::rep;
  static constexpr ratio period = duration::period;

private:
  duration d;

public:
  time_point() : d(duration::zero()) {}
  ~time_point() = default;
  time_point(const time_point &) = default;
  time_point &operator =(const time_point &) = default;
  time_point(time_point &&) = default;
  time_point &operator =(time_point &&) = default;

  constexpr explicit time_point(const duration &dd) : d(dd) {}
  template <class D2>
  constexpr time_point(const time_point<C, D2> &t)
    requires is_convertible<D2, duration> : d(t.time_since_epoch()) {}

  constexpr duration time_since_epoch() const {
    return d;
  }
  constexpr duration time() const {
    return d;
  }
  constexpr auto count() const {
    return time().count();
  }

  constexpr time_point &operator ++() {
    ++d;
    return *this;
  }
  constexpr time_point operator ++(int) {
    return time_point(d++);
  }
  constexpr time_point &operator --() {
    --d;
    return *this;
  }
  constexpr time_point operator --(int) {
    return time_point(d--);
  }
  constexpr time_point &operator +=(const duration &dd) {
    d += dd;
    return *this;
  }
  constexpr time_point &operator -=(const duration &dd) {
    d -= dd;
    return *this;
  }

  static constexpr time_point min() noexcept {
    return time_point(duration::min());
  }
  static constexpr time_point max() noexcept {
    return time_point(duration::max());
  }

  template <class T>
  constexpr time_point<C, T> floor() const {
    return time_point<C, T>(d.template floor<T>());
  }
  template <class T>
  constexpr time_point<C, T> ceil() const {
    return time_point<C, T>(d.template ceil<T>());
  }
  template <class T>
  constexpr time_point<C, T> round() const {
    return time_point<C, T>(d.template round<T>());
  }
};
template <class T>
struct fo_time_point_cast {
  template <class C, class D>
  constexpr time_point<C, T> operator ()(const time_point<C, D> &t) {
    return time_point<C, T>(duration_cast<T>(t.time_since_epoch()));
  }
};
template <class T>
inline fo_time_point_cast<T> time_point_cast{};
template <class T>
struct fo_time_point_ceil {
  template <class C, class D>
  constexpr auto operator ()(const time_point<C, D> &t)
    ->decltype(t.template ceil<T>()) {
    return t.template ceil<T>();
  }
};
template <class T>
inline fo_time_point_ceil<T> time_point_ceil{};
template <class T>
struct fo_time_point_floor {
  template <class C, class D>
  constexpr auto operator ()(const time_point<C, D> &t)
    ->decltype(t.template floor<T>()) {
    return t.template floor<T>();
  }
};
template <class T>
inline fo_time_point_floor<T> time_point_floor{};
template <class T>
struct fo_time_point_round {
  template <class C, class D>
  constexpr auto operator ()(const time_point<C, D> &t)
    ->decltype(t.template round<T>()) {
    return t.template round<T>();
  }
};
template <class T>
inline fo_time_point_round<T> time_point_round{};
template <class C, class D, class D2>
constexpr bool operator ==(const time_point<C, D> &x,
                           const time_point<C, D2> &y) {
  return x.time_since_epoch() == y.time_since_epoch();
}
template <class C, class D, class D2>
constexpr auto operator <=>(const time_point<C, D> &x,
                            const time_point<C, D2> &y) {
  return x.time_since_epoch() <=> y.time_since_epoch();
}
template <class C, class D, class R, ratio P>
constexpr time_point<C, common_type<D, duration<R, P>>>
operator +(const time_point<C, D> &x, const duration<R, P> &y) {
  return time_point<C, common_type<D, duration<R, P>>>
    (x.time_since_epoch() + y);
}
template <class R, ratio P, class C, class D>
constexpr time_point<C, common_type<D, duration<R, P>>>
operator +(const duration<R, P> &y, const time_point<C, D> &x) {
  return x + y;
}
template <class C, class D, class R, ratio P>
constexpr time_point<C, common_type<D, duration<R, P>>>
operator -(const time_point<C, D> &x, const duration<R, P> &y) {
  return time_point<C, common_type<D, duration<R, P>>>
    (x.time_since_epoch() - y);
}
template <class C, class D, class D2>
constexpr common_type<D, D2>
operator -(const time_point<C, D> &x, const time_point<C, D2> &y) {
  return x.time_since_epoch() - y.time_since_epoch();
}

template <class>
class scoped_timer;
class system_clock {
public:
  using rep = long long;
  static constexpr ratio period = ratio(1, 10000000);
  using duration = duration<rep, period>;
  using time_point = time_point<system_clock>;
  static constexpr bool is_steady = false;

  using timer = scoped_timer<system_clock>;

  static time_point now() noexcept {
    return time_point
      (duration(to_signed
                (inner::fns::win32_get_system_time_precise_as_file_time())));
  }
};
class steady_clock {
  struct freq_cache_t {
    long long value;

    freq_cache_t()
      : value(static_cast<long long>
              (inner::fns::win32_query_performance_frequency())) {}
    ~freq_cache_t() = default;
    freq_cache_t(const freq_cache_t &) = delete;
    freq_cache_t &operator =(const freq_cache_t &) = delete;
    freq_cache_t(freq_cache_t &&) = delete;
    freq_cache_t &operator =(const freq_cache_t &&) = delete;
  };

public:
  using rep = long long;
  static constexpr ratio period = ratio(1, 10000000);
  using duration = duration<rep, period>;
  using time_point = time_point<steady_clock>;
  static constexpr bool is_steady = true;

  using timer = scoped_timer<steady_clock>;

  inline static time_point now() noexcept {
    static const freq_cache_t f{};
    const auto c = static_cast<long long>
      (inner::fns::win32_query_performance_counter());
    return time_point(duration
                      ((f.value == 10000000)
                       ? c
                       : static_cast<long long>(c * (10000000.0 / f.value))));
  }
};
template <class CLOCK>
class scoped_timer {
  typename CLOCK::time_point t;
  double &d;

public:
  scoped_timer() = delete;
  ~scoped_timer() {
    const auto dur = CLOCK::now() - t;
    constexpr ratio r = decltype(dur)::period;
    d += (double)dur.count() / r.den * r.num;
  }
  scoped_timer(const scoped_timer &) = delete;
  scoped_timer &operator =(const scoped_timer &) = delete;
  scoped_timer(scoped_timer &&) = delete;
  scoped_timer &operator =(scoped_timer &&) = delete;

  explicit scoped_timer(double &d2) : t(CLOCK::now()), d(d2) {}
};

}

// wait_for
namespace re {

struct fo_wait_for {
  template <class R, ratio P>
  void operator ()(const duration<R, P> &d) const {
    auto t0 = steady_clock::now();
    auto t1 = t0 + d;
    while (steady_clock::now() < t1)
      ;
    return;
  }
};
inline constexpr fo_wait_for wait_for{};

}

#endif
