#ifndef RE_DEFINED_IO_H
#define RE_DEFINED_IO_H

#include "range.h"
#include "container.h"

namespace re {

class c_file {
  using this_t = c_file;

protected:
  FILE *fp;

  void clean_data() noexcept {
    if (fp != nullptr)
      fclose(fp);
  }
  void release_data() noexcept {
    fp = nullptr;
  }

public:
  c_file() noexcept : fp(nullptr) {}
  ~c_file() {
    clean_data();
  }
  c_file(const c_file &) = delete;
  c_file &operator =(const c_file &) = delete;
  c_file(c_file &&x) noexcept {
    fp = x.fp;
    x.fp = nullptr;
  }
  c_file &operator =(c_file &&x) noexcept {
    if (addressof(*this) != addressof(x)) {
      clean_data();
      fp = x.fp;
      x.release_data();
    }
    return *this;
  }

  c_file(const char *filename, const char *mode = "a+b") {
    fp = fopen(filename, mode);
  }

  c_file(nullptr_t) noexcept : fp(nullptr) {}
  c_file &operator =(nullptr_t) noexcept {
    fp = nullptr;
    return *this;
  }
  bool empty() const noexcept {
    return fp == nullptr;
  }

  void open(const char *filename, const char *mode = "a+b") {
    if (fp != nullptr)
      close();
    fp = fopen(filename, mode);
  }
  void close() {
    fclose(fp);
    fp = nullptr;
  }

  operator FILE *() const noexcept {
    return fp;
  }
  FILE *base() const noexcept {
    return fp;
  }

  bool error() const noexcept {
    return ferror(fp);
  }

  int getc() const noexcept {
    return fgetc(fp);
  }
  template <class STR = string>
  STR get() const noexcept {
    STR s;
    int c;
    while ((c = fgetc(fp)) != EOF)
      s.push_back(c);
    return s;
  }

  this_t &putc(int c) {
    fputc(c, fp);
    return *this;
  }
  this_t &put(char c) {
    fputc(c, fp);
    return *this;
  }
  this_t &put(const char *s) {
    fputs(s, fp);
    return *this;
  }
  template <class R>
  enable_if_t<is_rng<R> && is_citr<rng_itr<R>>
              && (is_same_v<rng_vt<R>, char>
                  || is_same_v<rng_vt<R>, signed char>
                  || is_same_v<rng_vt<R>, unsigned char>),
              this_t &>
  put(R &&r) {
    const int result = fwrite((void *)to_address(begin(r)), 1, size(r), fp);
    if (result == EOF)
      throw_or_abort<runtime_error>("fwrite() failed");
    return *this;
  }
  template <class R>
  enable_if_t<is_rng<R> && !is_citr<rng_itr<R>> && is_iitr<rng_itr<R>>
              && (is_same_v<rng_vt<R>, char>
                  || is_same_v<rng_vt<R>, signed char>
                  || is_same_v<rng_vt<R>, unsigned char>),
              this_t &>
  put(R &&r) {
    for (auto c : r)
      fputc(c, fp);
    return *this;
  }

  this_t &flush() {
    fflush(fp);
    return *this;
  }

  friend inline bool operator ==(const c_file &x, const c_file &y) noexcept;
};
inline bool operator ==(const c_file &x, const c_file &y) noexcept {
  return x.fp == y.fp;
}
inline bool operator !=(const c_file &x, const c_file &y) noexcept {
  return !(x == y);
}
inline bool operator ==(nullptr_t, const c_file &f) noexcept {
  return f.base() == nullptr;
}
inline bool operator ==(const c_file &f, nullptr_t) noexcept {
  return f.base() == nullptr;
}
inline bool operator !=(nullptr_t, const c_file &f) noexcept {
  return f.base() != nullptr;
}
inline bool operator !=(const c_file &f, nullptr_t) noexcept {
  return f.base() != nullptr;
}
inline bool file_exists(const char *s) {
  c_file f;
  f.open(s, "rb");
  return f != nullptr;
}

struct fputc_iterator {
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = void;
  using iterator_category = output_iterator_tag;

  FILE *s;

  fputc_iterator() = default;
  ~fputc_iterator() = default;
  fputc_iterator(const fputc_iterator &) = default;
  fputc_iterator &operator =(const fputc_iterator &) = default;
  fputc_iterator(fputc_iterator &&) = default;
  fputc_iterator &operator =(fputc_iterator &&) = default;

  fputc_iterator(FILE *s = stdout) : s(s) {}

  fputc_iterator &operator *() {
    return *this;
  }
  fputc_iterator &operator ++() {
    return *this;
  }
  fputc_iterator &operator ++(int) {
    return *this;
  }
  fputc_iterator &operator =(int c) {
    fputc(c, s);
    return *this;
  }
};
class console_c_file : protected c_file {
public:
  console_c_file() = default;
  ~console_c_file() {
    c_file::fp = nullptr;
  }
  console_c_file(const console_c_file &) = delete;
  console_c_file &operator =(const console_c_file &) = delete;
  console_c_file(console_c_file &&) = default;
  console_c_file &operator =(console_c_file &&) = delete;

  using c_file::operator FILE *;
  using c_file::error;
  using c_file::get;
  using c_file::getc;
  using c_file::put;
  using c_file::putc;
  using c_file::flush;

  explicit console_c_file(FILE *fp) {
    c_file::fp = fp;
  }
};
inline console_c_file stdin_f() noexcept {
  return console_c_file(stdin);
}
inline console_c_file stdout_f() noexcept {
  return console_c_file(stdout);
}

namespace inner {

template <class T>
struct sscan_int_traits_bin {
  static constexpr T base = 2;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (*it == '1')
      x = 1;
    else if (*it == '0')
      x = 0;
    else
      return false;
    return true;
  }
};
template <class T>
struct sscan_int_traits_dec {
  static constexpr T base = 10;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (isdigit(*it))
      x = *it - '0';
    else
      return false;
    return true;
  }
};
template <class T>
struct sscan_int_traits_oct {
  static constexpr T base = 8;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (*it <= '7' && *it >= '0')
      x = *it - '0';
    else
      return false;
    return true;
  }
};
template <class T>
struct sscan_int_traits_hex {
  static constexpr T base = 16;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (isdigit(*it))
      x = *it - '0';
    else if ('a' <= *it && *it <= 'f')
      x = 10 + (*it - 'a');
    else if ('A' <= *it && *it <= 'F')
      x = 10 + (*it - 'A');
    else
      return false;
    return true;
  }
};

template <class TRAITS, class SV, class T>
bool sscan_uint_impl(SV &v, T &o) {
  constexpr auto upper_lim = integral_traits<T>::max();

  const auto ed = end(v);
  auto it = begin(v);
  if (it == ed)
    return false;
  if (*it == '+') {
    ++it;
    if (it == ed)
      return false;
  }
  T x;
  if (!TRAITS::scan_single_char(it, x))
    return false;
  for (++it; it != ed; ++it) {
    T tmp;
    if (!TRAITS::scan_single_char(it, tmp))
      break;
    if (x > upper_lim / TRAITS::base)
      return false;
    x *= TRAITS::base;
    if (tmp > upper_lim - x)
      return false;
    x += tmp;
  }
  v = {it, ed};
  o = x;
  return true;
}
template <class TRAITS, class SV, class T>
bool sscan_int_impl(SV &v, T &o) {
  constexpr auto upper_lim = integral_traits<T>::max();
  constexpr auto lower_lim = integral_traits<T>::min();

  const auto ed = end(v);
  auto it = begin(v);
  bool plus = true;
  if (it == ed)
    return false;
  if (*it == '+') {
    ++it;
    if (it == ed)
      return false;
  }
  else if (*it == '-') {
    ++it;
    if (it == ed)
      return false;
    plus = false;
  }

  T x;
  if (!TRAITS::scan_single_char(it, x))
    return false;
  if (plus) {
    for (++it; it != ed; ++it) {
      T tmp;
      if (!TRAITS::scan_single_char(it, tmp))
        break;
      if (x > upper_lim / TRAITS::base)
        return false;
      x *= TRAITS::base;
      if (tmp > upper_lim - x)
        return false;
      x += tmp;
    }
  }
  else {
    x = -x;
    for (++it; it != ed; ++it) {
      T tmp;
      if (!TRAITS::scan_single_char(it, tmp))
        break;
      if (x < lower_lim / TRAITS::base)
        return false;
      x *= TRAITS::base;
      if (tmp > x - lower_lim)
        return false;
      x -= tmp;
    }
  }
  v = {it, ed};
  o = x;
  return true;
}

}
struct fo_sscan {
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_uint_impl<inner::sscan_int_traits_dec<T>>(v, o);
  }
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && !is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_int_impl<inner::sscan_int_traits_dec<T>>(v, o);
  }

  template <class SV>
  bool operator ()(SV &v, sview v2) const {
    const auto ed = end(v);
    const auto op = begin(v);
    const auto ed2 = end(v2);

    auto it = op;
    auto it2 = begin(v2);
    if (ed2 - it2 > ed - op)
      return false;
    for (;;) {
      if (it2 == ed2) {
        v = {it, ed};
        return true;
      }
      if (*it != *it2)
        return false;
      ++it;
      ++it2;
    }
  }
};
inline constexpr fo_sscan sscan{};
struct fo_sscan_bin {
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_uint_impl<inner::sscan_int_traits_bin<T>>(v, o);
  }
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && !is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_int_impl<inner::sscan_int_traits_bin<T>>(v, o);
  }
};
inline constexpr fo_sscan_bin sscan_bin{};
struct fo_sscan_oct {
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_uint_impl<inner::sscan_int_traits_oct<T>>(v, o);
  }
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && !is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_int_impl<inner::sscan_int_traits_oct<T>>(v, o);
  }
};
inline constexpr fo_sscan_oct sscan_oct{};
struct fo_sscan_hex {
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_uint_impl<inner::sscan_int_traits_hex<T>>(v, o);
  }
  template <class SV, class T>
  enable_if_t<is_integral_v<T> && !is_unsigned_v<T>, bool>
  operator ()(SV &v, T &o) const {
    return inner::sscan_int_impl<inner::sscan_int_traits_hex<T>>(v, o);
  }
};
inline constexpr fo_sscan_hex sscan_hex{};

// left
// right
// showpos
// noshowpos
// setfill(c) 
// setw(6)
// dec
// hex
// oct
// bin

namespace inner {

inline const char from_0_to_1f[]
  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     'a', 'b', 'c', 'd', 'e', 'f'};
inline const char from_0_to_1F[]
  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     'A', 'B', 'C', 'D', 'E', 'F'};

namespace noadl {

template <size_t N>
struct fixed_point_for_fprint {
  // | reserved_bits | significant_bits | (left high right low)
  using uint = uint64_t;
  static const uint base = 10; // 1/base = some binary number with limited bits
  static const uint uint_bits = 64;
  static const uint reserved_bits = 4;
  static const uint significant_bits = 60;
  static const uint significant_max = ~(uint)0 >> reserved_bits;

  // short endian
  // dot is at left side of a[N]
  uint a[N * 2];

  void l_shift(uint64_t n) {
    const uint d = n / significant_bits;
    const uint r = n % significant_bits;
    const auto mid = end(a) - d;
    if (d != 0) {
      for (auto &p : iters(begin(a), mid))
        *p = *(p + d);
      for (auto &x : rng(mid, end(a)))
        x = 0;
    }
    if (r != 0) {
      uint tmp = 0;
      for (auto &x : rrng(begin(a), mid)) {
        const uint tmp2 = (uint)(x >> (uint)(significant_bits - r));
        x = (uint)((uint)(x << r) & significant_max) | tmp;
        tmp = tmp2;
      }
    }
  };
  void r_shift(uint64_t n) {
    const uint d = n / significant_bits;
    const uint r = n % significant_bits;
    const auto mid = begin(a) + d;
    if (d != 0) {
      for (auto &p : iters(mid, end(a)))
        *p = *(p - d);
      for (auto &x : rng(begin(a), mid))
        x = 0;
    }
    if (r != 0) {
      uint tmp = 0;
      for (auto &x : rng(mid, end(a))) {
        const uint tmp2
          = (uint)(x << (uint)(significant_bits - r)) & significant_max;
        x = (uint)(x >> r) | tmp;
        tmp = tmp2;
      }
    }
  };
  void assign(uint64_t x, int64_t k) {
    // actual value: x * (2 ^ k)
    a[N - 1] = x;
    if (k > 0)
      l_shift(k);
    else
      r_shift(-k);
  }

  bool interger_is0() const {
    return none_of(rrng(a, N));
  }
  bool fraction_is0() const {
    return none_of(rng(a + N, end(a)));
  }
  char interger_div() {
    uint tmp = 0;
    for (auto &x : iters(a, a + N)) {
      x |= (uint)(tmp << significant_bits);
      const uint q = x / base;
      const uint r = x % base;
      x = q;
      tmp = r;
    }
    return tmp - '0';
  }
  char fraction_mul() {
    uint tmp = 0;
    for (auto &x : r_iters(a + N, end(a))) {
      x += tmp;
      x *= 10;
      tmp = x >> (uint)(uint_bits - reserved_bits);
      x &= significant_max;
    }
    return tmp - '0';
  }
  template <class S>
  void sprint_integer_part(S &&s) {
    auto n = size(s);
    do {
      s.push_back(interger_div());
    } while (!interger_is0());
    reverse(rng(begin(s) + n, end(s)));
  }
  template <class S>
  void sprint_fraction_part(S &&s) {
    s.push_back('.');
    do {
      s.push_back(fraction_mul());
    } while (!fraction_is0());
  }
};

}

template <class S>
S &sprint_floating_point_impl(S &&s, float x) {
  //todo
  using uint = uint32_t;
  const uint uint_bits = 32;
  const uint n = 8;
  const uint k = 23;
  const make_signed_t<uint> bias = 127;

  uint bits = bit_cast<uint>(x);

  const bool minus = (uint)(bits >> (uint)(uint_bits - (uint)1));
  const uint e = (uint)((uint)(bits << (uint)1)
                        >> (uint)(k + (uint)1));

  if (e == (uint)~(uint)((uint)~(uint)0 << n)) {
    // inf or nan
    const uint f = bits & (uint)~(uint)((uint)~(uint)0 << k);
    if (f == 0)
      s.append_range("inf"_sv);
    else
      s.append_range("nan"_sv);
  }
  else if (e == 0) {
    // denormalized
    const uint f = bits & (uint)~(uint)((uint)~(uint)0 << k);
    inner::noadl::fixed_point_for_fprint<3> fx{};
    fx.assign(f, (make_signed_t<uint>)1 - bias - (k - 1));
  }
  else {
    // normalized
    const uint f = (uint)(bits & (uint)~(uint)((uint)~(uint)0 << k))
      | (uint)((uint)1 << k);
    const uint ff = (uint)((uint)1 << k) + f;
    inner::noadl::fixed_point_for_fprint<3> fx{};
    fx.assign(ff,
              (make_signed_t<uint>)e - bias - k);
  }
  return s;
}
template <class S>
S &sprint_floating_point_impl(S &&s, double x) {
  return s;
}
template <class S>
S &sprint_floating_point_impl(S &&s, long double x);

}
struct print_args {
  size_t min_width = 0;
  bool left_padding = true;
  char padding_char = ' ';

  print_args() = default;
  ~print_args() = default;
  print_args(const print_args &) = default;
  print_args &operator =(const print_args &) = default;
  print_args(print_args &&) = default;
  print_args &operator =(print_args &&) = default;

  inline print_args &right_align() {
    left_padding = true;
    return *this;
  }
  inline print_args &left_align() {
    left_padding = false;
    return *this;
  }
  inline print_args &min_wid(size_t n) {
    min_width = n;
    return *this;
  }
  inline print_args &pad(char c) {
    padding_char = c;
    return *this;
  }
};
struct int_print_args : print_args {
  size_t radix = 10;
  const char *numbers = inner::from_0_to_1f;
  bool show_positive_symbol = false;

  int_print_args() = default;
  ~int_print_args() = default;
  int_print_args(const int_print_args &) = default;
  int_print_args &operator =(const int_print_args &) = default;
  int_print_args(int_print_args &&) = default;
  int_print_args &operator =(int_print_args &&) = default;

  inline int_print_args &right_align() {
    print_args::right_align();
    return *this;
  }
  inline int_print_args &left_align() {
    print_args::left_align();
    return *this;
  }
  inline int_print_args &min_wid(size_t n) {
    print_args::min_wid(n);
    return *this;
  }
  inline int_print_args &pad(char c) {
    print_args::pad(c);
    return *this;
  }

  inline int_print_args &bin() {
    radix = 2;
    return *this;
  }
  inline int_print_args &oct() {
    radix = 8;
    return *this;
  }
  inline int_print_args &hex() {
    radix = 16;
    return *this;
  }
  inline int_print_args &HEX() {
    radix = 16;
    numbers = inner::from_0_to_1F;
    return *this;
  }
  inline int_print_args &show_plus() {
    show_positive_symbol = true;
    return *this;
  }
  inline int_print_args &hide_plus() {
    show_positive_symbol = false;
    return *this;
  }
};
struct float_print_args : print_args {}; // todo
struct fo_sprint {
  template <class I, class S = string>
  enable_if_t<is_same_v<bool, decay_t<I>>, S &>
  operator ()(S &&s, I y, int_print_args pa = {}) const {
    operator ()(s, (int)y, pa);
    return s;
  }
  template <class I, class S = string>
  enable_if_t<!is_same_v<bool, decay_t<I>> && is_integral_v<I>, S &>
  operator ()(S &&s, I i, int_print_args pa = {}) const {
    bool sign_symbol_is_shown = false;
    typename decay_t<S>::size_type n = s.size();
    make_unsigned_t<I> u{};
    if (i < 0) {
      s.push_back('-');
      sign_symbol_is_shown = true;
      u = -to_signed(i);
    }
    else {
      if (pa.show_positive_symbol) {
        s.push_back('+');
        sign_symbol_is_shown = true;
      }
      u = i;
    }
#ifndef RE_NOEXCEPT
    try {
#endif
      for (;;) {
        s.push_back(pa.numbers[u % pa.radix]);
        u /= pa.radix;
        if (u == 0)
          break;
      }
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      s.erase(s.begin() + n, s.end());
      throw;
    }
#endif
    reverse(rng(s.begin() + n + sign_symbol_is_shown, s.end()));

    if (s.size() - n < pa.min_width)
      s.insert(pa.left_padding ? (s.begin() + n) : s.end(),
               rng(pa.min_width - (s.size() - n), pa.padding_char));
    return s;
  }

  template <class S = string>
  S &operator ()(S &&s, sview sv, print_args pa = {}) const {
    const auto sz = size(sv);
    if (sz >= pa.min_width)
      s.push_back(sv);
    else {
      if (pa.left_padding) {
        s.append_range(rng(pa.min_width - sz, pa.padding_char));
        s.push_back(sv);
      }
      else {
        s.push_back(sv);
        s.append_range(rng(pa.min_width - sz, pa.padding_char));
      }
    }
    return s;
  }

  template <class X, class S = string>
  enable_if_t<is_floating_point_v<X>, S &>
  operator ()(S &&s, X x, float_print_args pa = {}) const {
    return inner::sprint_floating_point_impl(s, x);
  }
};
inline constexpr fo_sprint sprint{};

struct fo_put {
  template <class...S>
  void operator ()(S &&...s) const {
    stdout_f().put(sprint(string{}, forward<S>(s)...));
    stdout_f().flush();
  }
};
inline constexpr fo_put put{};
struct fo_putln {
  void operator ()() const {
    put("\n");
  }
  template <class...S>
  void operator ()(S &&...s) const {
    put(forward<S>(s)...);
    put("\n");
  }
};
inline constexpr fo_putln putln{};

}

namespace re {

template <class STR = sview>
struct ssplitter {
  using vec_t = small_vector<STR, 10>;
  vec_t ss;

  using char_t = rng_vt<STR>;
  using view_t = string_view<char_t>;

public:
  ssplitter() = default;
  ~ssplitter() = default;
  ssplitter(const ssplitter &) = default;
  ssplitter &operator =(const ssplitter &) = default;
  ssplitter(ssplitter &&) = default;
  ssplitter &operator =(ssplitter &&) = default;
  friend void swap(ssplitter &x, ssplitter &y) noexcept {
    adl_swap(x.ss, y.ss);
  }

  ssplitter(view_t s, view_t x) {
    const auto ed = re::end(s);
    const auto x_sz = re::size(x);
    if (x_sz == 1) {
      const auto c = front(x);
      if (auto it = re::begin(s); it != ed)
        for (;;) {
          const auto it2 = find(rng(it, ed), c);
          ss.append(STR(it, it2));
          if (it2 == ed)
            break;
          it = it2 + 1;
        }
    }
    else {
      if (auto it = re::begin(s); it != ed)
        for (;;) {
          const auto it2 = search(rng(it, ed), x);
          ss.append(STR(it, it2));
          if (it2 == ed)
            break;
          it = it2 + x_sz;
        }
    }
  }
  ssplitter(view_t s, char_t c) {
    const auto ed = re::end(s);
    if (auto it = re::begin(s); it != ed)
      for (;;) {
        const auto it2 = find(rng(it, ed), c);
        ss.append(STR(it, it2));
        if (it2 == ed)
          break;
        it = it2 + 1;
      }
  }
  // template <class SEARCHER>
  // ssplitter(view_t s, view_t x, SEARCHER f); // todo

  bool empty() const {
    return re::empty(ss);
  }
  void clear() {
    ss.clear();
    ss.shrink_to_fit();
  }

  template <class OI>
  enable_if_t<!is_rng<OI> && !is_rng<OI>, OI>
  operator ()(view_t v, OI o) const {
    const auto ed = re::end(ss);
    const auto ed_pr = prev(ed);
    auto it = re::begin(ss);
    for (; it != ed_pr; ++it) {
      *o++ = *it;
      *o++ = v;
    }
    *o++ = *it;
    return o;
  }
  template <class S>
  enable_if_t<is_rng<S> && is_rng<S>> operator ()(view_t v, S &o) const {
    operator ()(v, to_back(o));
  }

  template <class IT, class OIT>
  enable_if_t<!is_rng<IT> && !is_convertible_v<IT, view_t>
              && !is_rng<OIT>, pair<IT, OIT>>
  operator ()(IT iter, OIT o) const {
    const auto ed = re::end(ss);
    const auto ed_pr = prev(ed);
    auto it = re::begin(ss);
    for (; it != ed_pr; ++it) {
      *o++ = *it;
      *o++ = *iter;
      ++iter;
    }
    *o++ = *it;
    return {iter, o};
  }

  template <class IT, class S>
  enable_if_t<!is_rng<IT> && !is_convertible_v<IT, view_t>
              && is_rng<S>, IT>
  operator ()(IT iter, S &o) const {
    return operator ()(iter, to_back(o)).first;
  }
  template <class R, class S>
  enable_if_t<is_rng<R> && !is_convertible_v<R &&, view_t>
              && is_rng<S>>
  operator ()(R &&r, S &o) const {
    operator ()(re::begin(r), o);
  }

  auto begin() {
    return ss.begin();
  }
  auto end() {
    return ss.end();
  }
  auto begin() const {
    return ss.begin();
  }
  auto end() const {
    return ss.end();
  }

  template <class R = vec_t>
  auto size() const->decltype(declval<R &>().size()) {
    return ss.size();
  }
};

struct fo_ssplit {
  template <class S>
  remove_reference_t<S>
  operator ()(S &&s, string_reference
              <add_const_t<rng_vt<remove_reference_t<S>>>>
              first_shared_portion) const {
    using str_t = remove_reference_t<S>;
    const auto n = size(first_shared_portion);
    const auto it = search(s, first_shared_portion);

    str_t ret(begin(s), n == 0 ? end(s) : it);
    str_t new_s((n == 0 || it == end(s)) ? end(s) : next(it, n), end(s));
    s = move(new_s);
    return ret;
  }
};
inline constexpr fo_ssplit ssplit{};

}

#endif
