#ifndef RE_DEFINED_IO_H
#define RE_DEFINED_IO_H

#include "range.h"
#include "container.h"
#include "time.h"

// win32_enable_utf8_console
namespace re::inner::fns {

inline void win32_enable_utf8_console() {
  SetConsoleOutputCP(65001);
}

}

// fputc_iterator
// c_file
// console_c_file
namespace re {

struct fputc_iterator {
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

private:
  FILE *s{};

public:
  fputc_iterator() = default;
  ~fputc_iterator() = default;
  fputc_iterator(const fputc_iterator &) = default;
  fputc_iterator &operator =(const fputc_iterator &) = default;
  fputc_iterator(fputc_iterator &&) = default;
  fputc_iterator &operator =(fputc_iterator &&) = default;
  friend void swap(fputc_iterator &x, fputc_iterator &y) noexcept {
    default_swap(x, y);
  }

  explicit fputc_iterator(FILE *s = stdout) : s(s) {}
  FILE *base() const {
    return s;
  }

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
    if (addressof(x) != this) {
      clean_data();
      fp = x.fp;
      x.release_data();
    }
    return *this;
  }
  friend void swap(c_file &x, c_file &y) noexcept {
    default_swap(x, y);
  }

  bool operator ==(nullptr_t) const noexcept {
    return fp == nullptr;
  }

  explicit c_file(const char *filename, const char *mode = "a+b") {
    fp = fopen(filename, mode);
  }
  explicit c_file(FILE *f) : fp(f) {}

  void open(const char *filename, const char *mode = "a+b") {
    if (fp != nullptr)
      close();
    fp = fopen(filename, mode);
  }

  bool empty() const noexcept {
    return fp == nullptr;
  }
  void close() {
    if (!empty()) {
      fclose(fp);
      fp = nullptr;
    }
  }
  FILE *base() const noexcept {
    return fp;
  }
  FILE *release() noexcept {
    return exchange(fp, nullptr);
  }
  bool error() const {
    return empty() || ferror(fp);
  }

  int getc() const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::getc(): null file pointer\n");
    return fgetc(fp);
  }
  const this_t &putc(int c) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::putc(c): null file pointer\n");
    fputc(c, fp);
    return *this;
  }
  const this_t &putwc(wint_t c) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::putc(c): null file pointer\n");
    fputwc(c, fp);
    return *this;
  }
  fputc_iterator putc_iter() const noexcept {
    return fputc_iterator(base());
  }
  const this_t &flush() const {
    if (!empty())
      fflush(fp);
    return *this;
  }

  template <class STR = string>
  STR get() const noexcept {
    if (empty())
      throw_or_terminate<logic_error>("re::c_file::get(): null file pointer\n");
    STR s;
    int c;
    while ((c = fgetc(fp)) != EOF)
      s.push_back(c);
    return s;
  }
  const this_t &put(const char *s) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::put(const char *): null file pointer\n");
    fputs(s, fp);
    flush();
    return *this;
  }
  template <class R>
  enable_if<(!is_convertible<R &&, const char *>
             && is_rng<R> && is_citr<rng_itr<R>>
             && (is_same<rng_vt<R>, char>
                 || is_same<rng_vt<R>, signed char>
                 || is_same<rng_vt<R>, unsigned char>)),
            const this_t &>
  put(R &&r) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::put(csr): null file pointer\n");
    const int result = fwrite((void *)to_address(begin(r)), 1, size(r), fp);
    if (result == EOF)
      throw_or_terminate<runtime_error>("fwrite() failed");
    flush();
    return *this;
  }
  template <class R>
  enable_if<(!is_convertible<R &&, const char *>
             && is_rng<R> && !is_citr<rng_itr<R>>
             && is_iitr<rng_itr<R>>
             && (is_same<rng_vt<R>, char>
                 || is_same<rng_vt<R>, signed char>
                 || is_same<rng_vt<R>, unsigned char>)),
            const this_t &>
  put(R &&r) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::put(rsr): null file pointer\n");
    for (auto c : r)
      fputc(c, fp);
    flush();
    return *this;
  }

  const this_t &putws(const wchar_t *s) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::put(const wchar_t *): null file pointer\n");
    fputws(s, fp);
    flush();
    return *this;
  }
  template <class R>
  enable_if<(!is_convertible<R &&, const wchar_t *>
             && is_rng<R> && is_iitr<rng_itr<R>>
             && is_same<rng_vt<R>, wchar_t>),
            const this_t &>
  putws(R &&r) const {
    if (empty())
      throw_or_terminate<logic_error>
        ("re::c_file::put(wsr): null file pointer\n");
    for (auto c : r)
      fputwc(c, fp);
    flush();
    return *this;
  }
};

inline bool c_file_exists(const char *s) {
  c_file f;
  f.open(s, "rb");
  return f != nullptr;
}

class console_c_file : protected c_file {
public:
  console_c_file() = delete;
  ~console_c_file() {
    c_file::fp = nullptr;
  }
  console_c_file(const console_c_file &) = delete;
  console_c_file &operator =(const console_c_file &) = delete;
  console_c_file(console_c_file &&) = delete;
  console_c_file &operator =(console_c_file &&) = delete;

  using c_file::base;
  using c_file::error;
  using c_file::getc;
  using c_file::putc;
  using c_file::putc_iter;
  using c_file::flush;
  using c_file::get;
  using c_file::put;
  using c_file::putwc;
  using c_file::putws;

private:
  explicit console_c_file(FILE *fp) {
    c_file::fp = fp;
  }

  friend inline console_c_file stdin_f() noexcept;
  friend inline console_c_file stdout_f() noexcept;
  friend inline console_c_file stderr_f() noexcept;
};
inline console_c_file stdin_f() noexcept {
  return console_c_file(stdin);
}
inline console_c_file stdout_f() noexcept {
  return console_c_file(stdout);
}
inline console_c_file stderr_f() noexcept {
  return console_c_file(stderr);
}

}

// print_tag
namespace re {

struct print_tag_left {};
inline constexpr print_tag_left left{};
struct print_tag_right {};
inline constexpr print_tag_right right{};

struct print_tag_min_width {
  size_t value{};
};
struct fo_setw {
  print_tag_min_width operator ()(size_t w) const noexcept {
    return print_tag_min_width{w};
  }
};
inline constexpr fo_setw setw{};

struct print_tag_fill {
  char32_t value = U' ';
};
struct fo_setfill {
  print_tag_fill operator ()(char c) const noexcept {
    return print_tag_fill{to_unsigned(c)};
  }
  print_tag_fill operator ()(char32_t c) const noexcept {
    return print_tag_fill{c};
  }
};
inline constexpr fo_setfill setfill{};

struct print_tag_showpos {};
inline constexpr print_tag_showpos showpos{};
struct print_tag_noshowpos {};
inline constexpr print_tag_noshowpos noshowpos{};

struct print_tag_dec {};
inline constexpr print_tag_dec dec{};
struct print_tag_hex {};
inline constexpr print_tag_hex hex{};
struct print_tag_oct {};
inline constexpr print_tag_oct oct{};
struct print_tag_bin {};
inline constexpr print_tag_bin bin{};

struct print_tag_separator {
  size_t value = 0;
};
struct fo_setseparator {
  print_tag_separator operator ()(size_t n) const {
    return print_tag_separator{n};
  }
};
inline constexpr fo_setseparator setseparator{};

struct print_tag_uppercase {};
inline constexpr print_tag_uppercase uppercase{};
struct print_tag_nouppercase {};
inline constexpr print_tag_nouppercase nouppercase{};

struct print_tag_fixed {};
inline constexpr print_tag_fixed fixed{};
struct print_tag_scientific {};
inline constexpr print_tag_scientific scientific{};

struct print_tag_precision {
  size_t value{};
};
struct fo_setprecision {
  print_tag_precision operator ()(size_t w) const noexcept {
    return print_tag_precision(w);
  }
};
inline constexpr fo_setprecision setprecision{};

struct print_tag_precision_fold0 {
  size_t value{};
};
struct fo_setprecision_fold0 {
  print_tag_precision_fold0 operator ()(size_t w) const noexcept {
    return print_tag_precision_fold0(w);
  }
};
inline constexpr fo_setprecision_fold0 setprecision_fold0{};

}

// sscan
namespace re {

namespace inner::fns {

template <class C>
bool is_unicode_digit(C c) {
  return to_unsigned(c) >= U'0' && to_unsigned(c) <= U'9';
}

}
namespace inner {

template <class T>
struct sscan_int_traits_bin {
  static constexpr T base = 2;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (to_unsigned(*it) == U'1')
      x = 1;
    else if (to_unsigned(*it) == U'0')
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
    if (inner::fns::is_unicode_digit(*it)) {
      x = to_unsigned(*it) - U'0';
      return true;
    }
    else
      return false;
  }
};
template <class T>
struct sscan_int_traits_oct {
  static constexpr T base = 8;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (to_unsigned(*it) <= U'7' && to_unsigned(*it) >= U'0') {
      x = to_unsigned(*it) - U'0';
      return true;
    }
    else
      return false;
  }
};
template <class T>
struct sscan_int_traits_hex {
  static constexpr T base = 16;

  template <class ITER>
  static bool scan_single_char(ITER it, T &x) {
    if (inner::fns::is_unicode_digit(*it))
      x = to_unsigned(*it) - U'0';
    else if (U'a' <= to_unsigned(*it) && to_unsigned(*it) <= U'f')
      x = 10u + (to_unsigned(*it) - U'a');
    else if (U'A' <= to_unsigned(*it) && to_unsigned(*it) <= U'F')
      x = 10u + (to_unsigned(*it) - U'A');
    else
      return false;
    return true;
  }
};

template <>
struct sscan_int_traits_bin<bool> {};
template <>
struct sscan_int_traits_dec<bool> {};
template <>
struct sscan_int_traits_oct<bool> {};
template <>
struct sscan_int_traits_hex<bool> {};

}
namespace inner::fns {

template <class TRAITS, class SV, class T>
bool sscan_int_impl(SV &v, T &o) requires unsigned_integral<T> {
  constexpr auto upper_lim = integral_traits<T>::max();

  const auto ed = end(v);
  auto it = begin(v);
  if (it == ed)
    return false;
  bool neg_sign = false;
  if (to_unsigned(*it) == U'+') {
    ++it;
    if (it == ed)
      return false;
  }
  else if (to_unsigned(*it) == U'-') {
    ++it;
    if (it == ed)
      return false;
    neg_sign = true;
  }

  T x;
  if (!TRAITS::scan_single_char(it, x))
    return false;
  for (++it; it != ed; ++it) {
    if (to_unsigned(*it) == U'\'') {
      ++it;
      if (it == ed) {
        --it;
        break;
      }
    }
    T tmp;
    if (!TRAITS::scan_single_char(it, tmp)) {
      if (to_unsigned(*prev(it)) == U'\'')
        --it;
      break;
    }
    if (x > upper_lim / TRAITS::base)
      return false;
    x *= TRAITS::base;
    if (tmp > upper_lim - x)
      return false;
    x += tmp;
  }
  if (neg_sign && x != 0u)
    return false;

  v = {it, ed};
  o = x;
  return true;
}
template <class TRAITS, class SV, class T>
bool sscan_int_impl(SV &v, T &o) requires signed_integral<T> {
  const auto ed = end(v);
  auto it = begin(v);
  bool neg_sign = false;
  if (it == ed)
    return false;
  if (to_unsigned(*it) == U'+') {
    ++it;
    if (it == ed)
      return false;
  }
  else if (to_unsigned(*it) == U'-') {
    ++it;
    if (it == ed)
      return false;
    neg_sign = true;
  }

  T x{};
  if (!TRAITS::scan_single_char(it, x))
    return false;
  if (neg_sign)
    x = -x;

  const auto f_non_neg = [](T &acc, const T &new_number)->bool {
    constexpr T upper_lim = integral_traits<T>::max();
    constexpr T lower_lim = integral_traits<T>::min();
    if (acc > upper_lim / TRAITS::base)
      return false;
    acc *= TRAITS::base;
    if (new_number > upper_lim - acc)
      return false;
    acc += new_number;
    return true;
  };
  const auto f_neg = [](T &acc, const T &new_number)->bool {
    constexpr T upper_lim = integral_traits<T>::max();
    constexpr T lower_lim = integral_traits<T>::min();
    if (acc < lower_lim / TRAITS::base)
      return false;
    acc *= TRAITS::base;
    if (-new_number < lower_lim - acc)
      return false;
    acc -= new_number;
    return true;
  };
  bool (*const fp)(T &acc, const T &) = neg_sign ? f_neg : f_non_neg;

  for (++it; it != ed; ++it) {
    if (to_unsigned(*it) == U'\'') {
      ++it;
      if (it == ed) {
        --it;
        break;
      }
    }
    T tmp;
    if (!TRAITS::scan_single_char(it, tmp)) {
      if (to_unsigned(*prev(it)) == U'\'')
        --it;
      break;
    }
    if (!fp(x, tmp))
      return false;
  }

  v = {it, ed};
  o = x;
  return true;
}

template <class TRAITS, class SV>
bool sscan_int_impl(SV &v, bool &o) {
  const auto ed = end(v);
  auto it = begin(v);
  if (it == ed)
    return false;
  if (to_unsigned(*it) == U'+') {
    ++it;
    if (it == ed)
      return false;
  }
  else if (to_unsigned(*it) == U'-') {
    ++it;
    if (it == ed)
      return false;
  }

  int x = 0;
  bool y = false;
  if (!inner::fns::is_unicode_digit(*it))
    return false;
  if (to_unsigned(*it) != U'0')
    y = true;
  for (++it; it != ed; ++it) {
    if (to_unsigned(*it) == U'\'') {
      ++it;
      if (it == ed) {
        --it;
        break;
      }
    }
    if (!inner::fns::is_unicode_digit(*it)) {
      if (to_unsigned(*prev(it)) == U'\'')
        --it;
      break;
    }
    if (to_unsigned(*it) != U'0')
      y = true;
  }

  o = y;
  v = {it, ed};
  return true;
}

}
struct fo_sscan {
  bool operator ()(sview &v, sview v2) const {
    const auto v_sz = ssize(v);
    const auto v2_sz = ssize(v2);
    if (v_sz >= v2_sz && !v2.empty() && equal(v2, v.begin())) {
      v = {v.begin() + v2_sz, v.end()};
      return true;
    }
    else
      return false;
  }
  bool operator ()(u32sview &v, u32sview v2) const {
    const auto v_sz = ssize(v);
    const auto v2_sz = ssize(v2);
    if (v_sz >= v2_sz && !v2.empty() && equal(v2, v.begin())) {
      v = {v.begin() + v2_sz, v.end()};
      return true;
    }
    else
      return false;
  }
  template <class T>
  bool operator ()(string_reference<const T> &v,
                   string_reference<const T> v2) const
    requires (!is_same<remove_cv<T>, char>) {
    const auto v_sz = ssize(v);
    const auto v2_sz = ssize(v2);
    if (v_sz >= v2_sz && !v2.empty() && equal(v2, v.begin())) {
      v = {v.begin() + v2_sz, v.end()};
      return true;
    }
    else
      return false;
  }

  template <class U, class T>
  bool operator ()(string_reference<const U> &v, T &o) const
    requires is_integral<T> {
    return inner::fns::sscan_int_impl<inner::sscan_int_traits_dec<T>>(v, o);
  }
  template <class U, class T>
  bool operator ()(string_reference<const U> &v, T &o, print_tag_dec) const
    requires is_integral<T> {
    return operator ()(v, o);
  }
  template <class U, class T>
  bool operator ()(string_reference<const U> &v, T &o, print_tag_bin) const
    requires is_integral<T> {
    return inner::fns::sscan_int_impl<inner::sscan_int_traits_bin<T>>(v, o);
  }
  template <class U, class T>
  bool operator ()(string_reference<const U> &v, T &o, print_tag_oct) const
    requires is_integral<T> {
    return inner::fns::sscan_int_impl<inner::sscan_int_traits_oct<T>>(v, o);
  }
  template <class U, class T>
  bool operator ()(string_reference<const U> &v, T &o, print_tag_hex) const
    requires is_integral<T> {
    return inner::fns::sscan_int_impl<inner::sscan_int_traits_hex<T>>(v, o);
  }

private:
  template <class T, class F>
  static bool impl_for_float(string_reference<T> &v, F &x);
public:
  template <class T>
  bool operator ()(string_reference<const T> &v, float &x) const {
    return impl_for_float(v, x);
  }
  template <class T>
  bool operator ()(string_reference<const T> &v, double &x) const {
    return impl_for_float(v, x);
  }
};
inline constexpr fo_sscan sscan{};

struct fo_sscan_single {
  template <class T>
  bool operator ()(string_reference<const T> &v, T &c) const {
    if (v.empty())
      return false;
    else {
      c = *v.begin();
      v = {v.begin() + 1, v.end()};
      return true;
    }
  }
  template <class T>
  optional<T> operator ()(string_reference<const T> &v) const {
    if (v.empty())
      return nullopt;
    else {
      optional<int> ret = *v.begin();
      v = {v.begin() + 1, v.end()};
      return ret;
    }
  }
};
inline constexpr fo_sscan_single sscan_single{};

struct fo_sscan_while {
  template <class T, class F>
  string_reference<const T>
  operator ()(string_reference<const T> &v, F f) const {
    const auto op = v.begin();
    const auto ed = v.end();
    auto it = v.begin();
    for (; it != ed && f(*it); ++it)
      ;
    v = {it, ed};
    return string_reference<const T>(op, it);
  }
};
inline constexpr fo_sscan_while sscan_while{};

struct fo_sscan_ln {
  template <class T>
  bool operator ()(string_reference<const T> &v) const {
    if (v.empty())
      return false;
    else {
      const auto c = *v.begin();
      if (to_unsigned(c) == 10u) {
        v = {v.begin() + 1, v.end()};
        return true;
      }
      else if (to_unsigned(c) == 13u) {
        if (v.begin() + 1 != v.end()
            && to_unsigned(*(v.begin() + 1)) == 10u) {
          v = {v.begin() + 2, v.end()};
          return true;
        }
        v = {v.begin() + 1, v.end()};
        return true;
      }
      else
        return false;
    }
  }
};
inline constexpr fo_sscan_ln sscan_ln{};

struct fo_sscan_line {
  template <class T>
  optional<string_reference<const T>>
  operator ()(string_reference<const T> &v) const {
    using view_t = string_reference<const T>;
    const auto it = v.begin();
    for (;;) {
      auto it2 = v.begin();
      if (sscan_ln(v))
        return view_t(it, it2);
      else {
        if (v.empty()) {
          if (it != it2)
            return view_t(it, it2);
          else
            return nullopt;
        }
        else
          v = {v.begin() + 1, v.end()};
      }
    }
  }

  template <class T>
  optional<string_reference<const T>>
  operator ()(string_reference<const T> &v, bool &has_ln) const {
    using view_t = string_reference<const T>;
    const auto it = v.begin();
    for (;;) {
      auto it2 = v.begin();
      if (sscan_ln(v)) {
        has_ln = true;
        return view_t(it, it2);
      }
      else {
        if (v.empty()) {
          has_ln = false;
          if (it != it2)
            return view_t(it, it2);
          else
            return nullopt;
        }
        else
          v = {v.begin() + 1, v.end()};
      }
    }
  }
};
inline constexpr fo_sscan_line sscan_line{};

struct fo_sscan_spaces {
  void operator ()(sview &v) const {
    if (!v.empty()) {
      auto it = v.begin();
      while (it != v.end() && isspace(*it))
        ++it;
      v = {it, v.end()};
    }
  }
  void operator ()(u32sview &v) const {
    if (!v.empty()) {
      auto it = v.begin();
      while (it != v.end() && unicode_isspace(*it))
        ++it;
      v = {it, v.end()};
    }
  }
};
inline constexpr fo_sscan_spaces sscan_spaces{};

struct fo_sscan_common_spaces {
  template <class T>
  void operator ()(string_reference<const T> &v) const {
    if (!v.empty()) {
      auto it = v.begin();
      while (it != v.end() && unicode_is_common_space(to_unsigned(*it)))
        ++it;
      v = {it, v.end()};
    }
  }
};
inline constexpr fo_sscan_common_spaces sscan_common_spaces{};

}

// sprint
// put
namespace re {

namespace inner {

inline constexpr char from_0_to_f[]
  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     'a', 'b', 'c', 'd', 'e', 'f'};
inline constexpr char from_0_to_F[]
  = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
     'A', 'B', 'C', 'D', 'E', 'F'};

template <class T>
concept sprint_s_arg = requires (T &v) {
  v.push_back(declval<rng_vt<T>>());
};

}
namespace inner::fns {

template <class T>
T padding_char_to(char32_t c) {
  if constexpr (is_unsigned<T>)
    return static_cast<T>(c);
  else
    return static_cast<make_unsigned<T>>(c);
}

}

struct print_args {
  size_t min_width = 0;
  bool left_padding = true;
  char32_t padding_char = U' ';

  print_args() = default;
  ~print_args() = default;
  print_args(const print_args &) = default;
  print_args &operator =(const print_args &) = default;
  print_args(print_args &&) = default;
  print_args &operator =(print_args &&) = default;

  print_args &left() {
    left_padding = true;
    return *this;
  }
  print_args &right() {
    left_padding = false;
    return *this;
  }
  print_args &setw(size_t n) {
    min_width = n;
    return *this;
  }
  print_args &setfill(char c) {
    padding_char = c;
    return *this;
  }
  print_args &setfill(char32_t c) {
    padding_char = c;
    return *this;
  }
};
struct int_print_args : print_args {
  size_t radix = 10;
  const char *numbers = inner::from_0_to_f;
  bool upper_case = false;
  bool show_positive_symbol = false;
  size_t separator_n = 0;

  int_print_args() = default;
  ~int_print_args() = default;
  int_print_args(const int_print_args &) = default;
  int_print_args &operator =(const int_print_args &) = default;
  int_print_args(int_print_args &&) = default;
  int_print_args &operator =(int_print_args &&) = default;

  int_print_args &left() {
    print_args::left();
    return *this;
  }
  int_print_args &right() {
    print_args::right();
    return *this;
  }
  int_print_args &setw(size_t n) {
    print_args::setw(n);
    return *this;
  }
  int_print_args &setfill(char c) {
    print_args::setfill(c);
    return *this;
  }

  int_print_args &dec() {
    radix = 10;
    return *this;
  }
  int_print_args &bin() {
    radix = 2;
    return *this;
  }
  int_print_args &oct() {
    radix = 8;
    return *this;
  }
  int_print_args &hex() {
    radix = 16;
    return *this;
  }
  int_print_args &uppercase() {
    upper_case = true;
    return *this;
  }
  int_print_args &nouppercase() {
    upper_case = false;
    return *this;
  }
  int_print_args &showpos() {
    show_positive_symbol = true;
    return *this;
  }
  int_print_args &noshowpos() {
    show_positive_symbol = false;
    return *this;
  }
  int_print_args &setseparator(size_t n) {
    separator_n = n;
    return *this;
  }
};
struct float_print_args : print_args {
  const char *numbers = inner::from_0_to_f;
  bool upper_case = false;
  bool show_positive_symbol = false;
  size_t separator_n = 0u;
  bool scientific_notation = false;
  size_t precision = numeric_limits<size_t>::max();
  bool fold_zero = true;

  float_print_args() = default;
  ~float_print_args() = default;
  float_print_args(const float_print_args &) = default;
  float_print_args &operator =(const float_print_args &) = default;
  float_print_args(float_print_args &&) = default;
  float_print_args &operator =(float_print_args &&) = default;

  float_print_args &left() {
    print_args::left();
    return *this;
  }
  float_print_args &right() {
    print_args::right();
    return *this;
  }
  float_print_args &setw(size_t n) {
    print_args::setw(n);
    return *this;
  }
  float_print_args &setfill(char c) {
    print_args::setfill(c);
    return *this;
  }

  float_print_args &uppercase() {
    upper_case = true;
    return *this;
  }
  float_print_args &nouppercase() {
    upper_case = false;
    return *this;
  }

  float_print_args &showpos() {
    show_positive_symbol = true;
    return *this;
  }
  float_print_args &noshowpos() {
    show_positive_symbol = false;
    return *this;
  }

  float_print_args &setseparator(size_t n) {
    separator_n = n;
    return *this;
  }

  float_print_args &scientific() {
    scientific_notation = true;
    return *this;
  }
  float_print_args &fixed() {
    scientific_notation = false;
    return *this;
  }

  float_print_args &setprecision(size_t x) {
    precision = x;
    fold_zero = (x == numeric_limits<size_t>::max());
    return *this;
  }

  float_print_args &setprecision_fold0(size_t x) {
    precision = x;
    fold_zero = true;
    return *this;
  }
};

struct fo_sprint {
private:
  template <class S, class...SS>
  static S &impl_for_string(S &&s, string_reference<const rng_vt<S>> sv,
                            print_args &pa) {
    const auto sz = size(sv);
    if (sz >= pa.min_width)
      s.push_back(sv);
    else {
      if (pa.left_padding) {
        s.append_range(rng(pa.min_width - sz,
                           inner::fns::padding_char_to<rng_vt<S>>
                           (pa.padding_char)));
        s.push_back(sv);
      }
      else {
        s.push_back(sv);
        s.append_range(rng(pa.min_width - sz,
                           inner::fns::padding_char_to<rng_vt<S>>
                           (pa.padding_char)));
      }
    }
    return s;
  }
  template <class S, class...SS>
  static S &impl_for_string(S &&s, string_reference<const rng_vt<S>> sv,
                            print_args &pa,
                            print_tag_left, SS &&...ss) {
    pa.left();
    return impl_for_string(s, sv, pa, forward<SS>(ss)...);
  }
  template <class S, class...SS>
  static S &impl_for_string(S &&s, string_reference<const rng_vt<S>> sv,
                            print_args &pa,
                            print_tag_right, SS &&...ss) {
    pa.right();
    return impl_for_string(s, sv, pa, forward<SS>(ss)...);
  }
  template <class S, class...SS>
  static S &impl_for_string(S &&s, string_reference<const rng_vt<S>> sv,
                            print_args &pa,
                            print_tag_min_width x, SS &&...ss) {
    pa.setw(x.value);
    return impl_for_string(s, sv, pa, forward<SS>(ss)...);
  }
  template <class S, class...SS>
  static S &impl_for_string(S &&s, string_reference<const rng_vt<S>> sv,
                            print_args &pa,
                            print_tag_fill x, SS &&...ss) {
    pa.setfill(x.value);
    return impl_for_string(s, sv, pa, forward<SS>(ss)...);
  }
public:
  template <class S>
  S &operator ()(S &&s,
                 string_reference<const rng_vt<S>> sv,
                 print_args pa = {}) const
    requires inner::sprint_s_arg<S> {
    return impl_for_string(s, sv, pa);
  }
  template <class S, class...SS>
  S &operator ()(S &&s,
                 string_reference<const rng_vt<S>> sv,
                 SS &&...ss) const
    requires (inner::sprint_s_arg<S>
              && !(sizeof...(SS) == 1u
                   && is_convertible<nth_type<0, SS &&...>, print_args>)) {
    print_args pa{};
    return impl_for_string(s, sv, pa, forward<SS>(ss)...);
  }

private:
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa) {
    typename decay<S>::size_type n = s.size();
    auto guard = exit_fn([&]() {s.resize(n);}, true);

    bool sign_symbol_is_shown = false;
    make_unsigned<I> u{};
    if (i < 0) {
      s.push_back(U'-');
      sign_symbol_is_shown = true;
      u = -to_signed(i);
    }
    else {
      if (pa.show_positive_symbol) {
        s.push_back(U'+');
        sign_symbol_is_shown = true;
      }
      u = i;
    }
#ifndef RE_NOEXCEPT
    try {
#endif
      for (;;) {
        const char *num_a = (pa.upper_case  && pa.numbers == inner::from_0_to_f)
          ? inner::from_0_to_F : inner::from_0_to_f;
        s.push_back(to_unsigned(num_a[u % pa.radix]));
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
    const auto r = rng(s.begin() + n + (sign_symbol_is_shown ? 1u : 0u),
                       s.end());
    reverse(r);
    if (pa.separator_n != 0) {
      const rng_szt<S> digit_len = size(r);
      rng_szt<S> sep_count = digit_len / pa.separator_n;
      const rng_szt<S> rem = digit_len % pa.separator_n;
      if (sep_count != 0) {
        if (rem == 0)
          --sep_count;
        const auto r_len = ssize(r);
        s.reserve_more(sep_count);
        s.resize(s.size() + sep_count);
        const auto r_op = s.end() - to_signed(sep_count) - r_len;
        auto it = s.end() - to_signed(sep_count);
        auto it2 = s.end();
        for (rng_szt<S> k = 0u; r_op != it && it != it2;) {
          *--it2 = *--it;
          ++k;
          if (k == pa.separator_n) {
            k = 0u;
            *--it2 = U'\'';
          }
        }
      }
    }
    if (s.size() - n < pa.min_width)
      s.insert(pa.left_padding ? (s.begin() + n) : s.end(),
               rng(pa.min_width - (s.size() - n),
                   inner::fns::padding_char_to<rng_vt<S>>(pa.padding_char)));
    guard.unset();
    return s;
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_left, SS &&...ss) {
    pa.left();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_right, SS &&...ss) {
    pa.right();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_min_width x, SS &&...ss) {
    pa.setw(x.value);
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_fill x, SS &&...ss) {
    pa.setfill(x.value);
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_showpos x, SS &&...ss) {
    pa.showpos();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_noshowpos x, SS &&...ss) {
    pa.noshowpos();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_dec x, SS &&...ss) {
    pa.dec();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_hex x, SS &&...ss) {
    pa.hex();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_uppercase, SS &&...ss) {
    pa.uppercase();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_nouppercase, SS &&...ss) {
    pa.nouppercase();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_oct x, SS &&...ss) {
    pa.oct();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_bin x, SS &&...ss) {
    pa.bin();
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }
  template <class I, class S, class...SS>
  static S &impl_for_int(S &&s, I i, int_print_args &pa,
                         print_tag_separator x, SS &&...ss) {
    pa.setseparator(x.value);
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }

public:
  template <class I, class S>
  enable_if<inner::sprint_s_arg<S> && is_same<bool, decay<I>>, S &>
  operator ()(S &&s, I y, int_print_args pa = {}) const {
    return impl_for_int(s, (int)y, pa);
  }
  template <class I, class S, class...SS>
  enable_if<inner::sprint_s_arg<S> && is_same<bool, decay<I>>, S &>
  operator ()(S &&s, I y, SS &&...ss) const {
    int_print_args pa{};
    return impl_for_int(s, (int)y, pa, forward<SS>(ss)...);
  }

  template <class I, class S>
  enable_if<inner::sprint_s_arg<S>
            && !is_same<bool, decay<I>> && is_integral<I>, S &>
  operator ()(S &&s, I i, int_print_args pa = {}) const {
    return impl_for_int(s, i, pa);
  }
  template <class I, class S, class...SS>
  enable_if<(inner::sprint_s_arg<S>
             && !is_same<bool, decay<I>> && is_integral<I>
             && !(sizeof...(SS) == 1u
                  && is_convertible
                  <nth_type<0, SS &&...>, int_print_args>)),
            S &>
  operator ()(S &&s, I i, SS &&...ss) const {
    int_print_args pa{};
    return impl_for_int(s, i, pa, forward<SS>(ss)...);
  }

private:
  template <class F, class S>
  static S &impl_for_float(S &&, F, const float_print_args &)
    requires inner::sprint_s_arg<S>;
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_left, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.left();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_right, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.right();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_min_width u, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.setw(u.value);
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_fill f, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.setfill(f.value);
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_showpos, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.showpos();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_noshowpos, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.noshowpos();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_separator u, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.setseparator(u.value);
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_uppercase, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.uppercase();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_nouppercase, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.nouppercase();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_fixed, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.fixed();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_scientific, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.scientific();
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_precision u, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.setprecision(u.value);
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
  template <class F, class S, class...SS>
  static S &impl_for_float(S &&s, F x, float_print_args &pa,
                           print_tag_precision_fold0 u, SS &&...ss)
    requires inner::sprint_s_arg<S> {
    pa.setprecision_fold0(u.value);
    return impl_for_float<F>(s, x, pa, forward<SS>(ss)...);
  }
public:
  template <class S>
  inline S &operator ()(S &&s, float x, float_print_args pa = {}) const
    requires inner::sprint_s_arg<S> {
    return impl_for_float<float>(s, x, pa);
  }
  template <class S>
  inline S &operator ()(S &&s, double x, float_print_args pa = {}) const
    requires inner::sprint_s_arg<S> {
    return impl_for_float<double>(s, x, pa);
  }
  template <class S, class...SS>
  S &operator ()(S &&s, float x, SS &&...ss) const
    requires (inner::sprint_s_arg<S>
              && !(sizeof...(SS) == 1u
                   && is_convertible
                   <nth_type<0, SS &&...>, float_print_args>)) {
    float_print_args pa{};
    return impl_for_float<float>(s, x, pa, forward<SS>(ss)...);
  }
  template <class S, class...SS>
  S &operator ()(S &&s, double x, SS &&...ss) const
    requires (inner::sprint_s_arg<S>
              && !(sizeof...(SS) == 1u
                   && is_convertible
                   <nth_type<0, SS &&...>, float_print_args>)) {
    float_print_args pa{};
    return impl_for_float<double>(s, x, pa, forward<SS>(ss)...);
  }

  template <class X, class...S>
  string operator ()(X &&x, S &&...s) const
    requires (!inner::sprint_s_arg<X>) {
    string ret;
    operator ()(ret, forward<X>(x), forward<S>(s)...);
    return ret;
  }
};
inline constexpr fo_sprint sprint{};
struct fo_sprint_u32 {
  template <class X, class...S>
  u32string operator ()(X &&x, S &&...s) const
    requires (!inner::sprint_s_arg<X>) {
    u32string ret;
    sprint(ret, forward<X>(x), forward<S>(s)...);
    return ret;
  }
  template <class X, class...S>
  X &operator ()(X &&x, S &&...s) const
    requires inner::sprint_s_arg<X> && is_same<rng_vt<X>, char32_t> {
    return sprint(x, forward<S>(s)...);
  }
};
inline constexpr fo_sprint_u32 sprint_u32{};

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

struct fo_putws {
  template <class...S>
  void operator ()(S &&...s) const {
    stdout_f().putws(sprint(wstring{}, forward<S>(s)...));
    stdout_f().flush();
  }
};
inline constexpr fo_putws putws{};
struct fo_putwsln {
  void operator ()() const {
    putws(L"\n");
  }
  template <class...S>
  void operator ()(S &&...s) const {
    putws(forward<S>(s)...);
    putws(L"\n");
  }
};
inline constexpr fo_putwsln putwsln{};

template <class T, class C>
template <class F>
string matrix<T, C>::sprint(F f) const {
  string ret;
  size_t max_w = 0;
  for (decltype(auto) x : range()) {
    const string tmp_s = f(x);
    if (const auto z = tmp_s.size();
        z > max_w)
      max_w = z;
  }
  for (auto r : rows())
    for_each_excluding_last(r,
                            [max_w, &ret, &f](const value_type &x) {
                              const string tmp_s = f(x);
                              re::sprint(ret, tmp_s, setw(max_w));
                              ret.push_back(' ');
                            },
                            [max_w, &ret, &f](const value_type &x) {
                              const string tmp_s = f(x);
                              re::sprint(ret, tmp_s, setw(max_w));
                              ret.push_back('\n');
                            });
  return ret;
}
template <class T, class C>
string matrix<T, C>::sprint() const {
  return sprint([](const T &x) {
    string ret;
    re::sprint(ret, x, setprecision_fold0(5));
    return ret;
  });
}

template <class T, class C>
string matrix<T, C>::
sprint_solutions_of_linear_equations(size_t precision,
                                     double zero_threshold) const
  requires is_same<T, double> {

  matrix m = *this;
  vector<pair<int, bool>> buf;
  m.make_echelon_impl(zero_threshold, buf);

  if (width() <= 1 || buf[width() - 1].first >= 0)
    return "no solution"_s;
  string ret;
  for (int &i : iters(0, width() - 1)) {
    if (buf[i].first < 0) {
      re::sprint(ret, "x"_sv);
      re::sprint(ret, i);
      re::sprint(ret, " = any\n"_sv);
      continue;
    }
    const auto r = m.row(buf[i].first);

    re::sprint(ret, "x"_sv);
    re::sprint(ret, i);
    re::sprint(ret, " = "_sv);
    if (!buf[i].second) {
      for_each_excluding_first(filter_rng(irng(i + 1, width() - 1),
                                          [=](int j) {
                                            return ref(r, j) != 0.0;
                                          }),
                               [&ret, &r, precision](int j) {
                                 re::sprint(ret, " + "_sv);
                                 re::sprint(ret, -ref(r, j),
                                            setprecision(precision));
                                 re::sprint(ret, "*x"_sv);
                                 re::sprint(ret, j);
                               },
                               [&ret, &r, precision](int j) {
                                 re::sprint(ret, -ref(r, j),
                                            setprecision(precision));
                                 re::sprint(ret, "*x"_sv);
                                 re::sprint(ret, j);
                               });
      if (const double tmp = back(r);
          tmp != 0.0) {
        re::sprint(ret, " + "_sv);
        re::sprint(ret, tmp, setprecision(precision));
      }
      re::sprint(ret, "\n"_sv);
    }
    else {
      re::sprint(ret, back(r), setprecision(precision));
      re::sprint(ret, "\n"_sv);
    }
  }
  return ret;
}

}

// big_int
namespace re {

namespace inner::fns {

template <class S>
void no_sign_fixed_fraction_string_apply_separator_arg(S &s,
                                                       ptrdiff_t i,
                                                       size_t sep_n) {
  // sv has valid format and is not empty
  // sv has no leading zero except "0."
  if (sep_n == 0u)
    return;

  const ptrdiff_t j = ssize(s);
  const auto eq_dot = [](auto c)->bool {return to_unsigned(c) == U'.';};
  const ptrdiff_t k = find_if(rng(nth(s, i), nth(s, j)), eq_dot) - s.begin();
  if (k == j) {
    // sep_n > 0
    // [nth(s, i), nth(s, j))

    const ptrdiff_t len = k - i;
    if (sep_n >= to_unsigned(len))
      return;
    ptrdiff_t q = len / to_signed(sep_n);
    const ptrdiff_t rem = len % to_signed(sep_n);
    if (rem == 0)
      --q;

    ptrdiff_t c = to_signed(sep_n);
    s.reserve_more(to_unsigned(q));
    s.resize(s.size() + to_unsigned(q));

    auto it2 = s.end();
    auto it = nth(s, j);
    for (;;) {
      *--it2 = *--it;
      --c;
      if (c == 0) {
        c = to_signed(sep_n);
        *--it2 = U'\'';
        if (it2 == it)
          break;
      }
    }
  }
  else {
    // sep_n > 0
    // [nth(s, i), nth(s, k)) // non-empty
    // [nth(s, k + 1), nth(s, j)) // non-empty

    const ptrdiff_t r1_len = k - i;
    const ptrdiff_t r2_len = j - (k + 1);

    ptrdiff_t q1 = r1_len / to_signed(sep_n);
    const ptrdiff_t rem1 = r1_len % to_signed(sep_n);
    ptrdiff_t q2 = r2_len / to_signed(sep_n);
    const ptrdiff_t rem2 = r2_len % to_signed(sep_n);
    if (q1 > 0 && rem1 == 0)
      --q1;
    if (q2 > 0 && rem2 == 0)
      --q2;

    s.reserve_more(to_unsigned(q1 + q2));
    s.resize(s.size() + to_unsigned(q1 + q2));

    auto it2 = s.end();
    // right part of dot
    {
      auto it = nth(s, j);
      const auto stop_point = nth(s, k + 1);
      ptrdiff_t c = (rem2 == 0 ? to_signed(sep_n) : rem2);
      for (;;) {
        *--it2 = *--it;
        if (it == stop_point)
          break;
        --c;
        if (c == 0) {
          c = to_signed(sep_n);
          *--it2 = U'\'';
        }
      }
      *--it2 = U'.';
    }
    // left part of dot
    {
      auto it = nth(s, k);
      ptrdiff_t c = to_signed(sep_n);
      for (;;) {
        if (it == it2)
          break;
        *--it2 = *--it;
        --c;
        if (c == 0) {
          c = to_signed(sep_n);
          *--it2 = U'\'';
        }
      }
    }
  }
}
template <class S = string>
S fixed_fraction_string_to_scientific(string_reference<const rng_vt<S>> sv,
                                      size_t precision, size_t sep_n,
                                      bool upper_case,
                                      bool show_pos,
                                      bool fold_zero) {
  // sv has valid format and is not empty
  // sv has no leading zero except "0."
  using view_t = string_reference<const rng_vt<S>>;
  S s2;
  if (to_unsigned(front(sv)) == U'+') {
    sv = {next(sv.begin()), sv.end()};
    if (show_pos)
      s2.append(U'+');
  }
  else if (to_unsigned(front(sv)) == U'-') {
    sv = {next(sv.begin()), sv.end()};
    s2.append(U'-');
  }
  else {
    if (show_pos)
      s2.append(U'+');
  }

  const auto s2_old_ssz = ssize(s2);
  const auto eq_dot = [](auto c)->bool {return to_unsigned(c) == U'.';};
  const auto dot_pos = find_if(sv, eq_dot);
  view_t sv_l = view_t(sv.begin(), dot_pos);
  view_t sv_r = view_t(dot_pos != sv.end() ? next(dot_pos) : sv.end(),
                       sv.end());
  rng_dft<view_t> e = 0;
  const auto eqf = [](auto c, auto c2) {return to_unsigned(c) == c2;};
  const auto not0 = [](auto c) {return to_unsigned(c) != U'0';};
  if (equal(sv_l, single_rng(U'0'), eqf)
      && all_of_equal(sv_r, U'0', eqf)) {
    s2.append(U'0');
    if (precision != 0u && !fold_zero) {
      s2.append(U'.');
      s2.append(rng(precision, U'0'));
    }
  }
  else {
    if (equal(sv_l, single_rng(U'0'), eqf)) {
      const auto i = find_if(sv_r, [](auto c) {return to_unsigned(c) != U'0';});
      e = -1 + (sv_r.begin() - i);
      view_t v(i, sv_r.end());
      const view_t v2(next(i), sv_r.end()); // v2 may be empty
      if (precision == 0u) {
        if (empty(v2))
          s2.append(front(v));
        else {
          if (to_unsigned(front(v2)) < U'5')
            s2.append(front(v));
          else {
            if (to_unsigned(front(v)) != U'9')
              s2.append(to_unsigned(front(v)) + 1u);
            else {
              s2.append(U'1');
              ++e;
            }
          }
        }
      }
      else if (size(v2) <= precision) {
        s2.append(front(v));
        if (!fold_zero) {
          s2.append(U'.');
          s2.append(v2);
          s2.append(rng(precision - size(v2), U'0'));
        }
        else {
          if (!all_of_equal(v2, U'0', eqf)) {
            // v2 is not empty
            s2.append(U'.');
            s2.append(view_t(v2.begin(), next(find_last_if(v2, not0))));
          }
        }
      }
      else {
        const auto j = nth(v2, to_signed(precision)); // j is dereferenceable
        if (to_unsigned(*j) < U'5') {
          s2.append(front(v));
          s2.append(U'.');
          s2.append(view_t(v2.begin(), j));

          if (fold_zero) {
            while (to_unsigned(s2.back()) == U'0')
              s2.pop_back();
            if (to_unsigned(s2.back()) == U'.')
              s2.pop_back();
          }
        }
        else {
          if (all_of_equal(view_t(v.begin(), j), U'9', eqf)) {
            s2.append(U'1');
            ++e;

            if (!fold_zero) {
              s2.append(U'.');
              s2.append(rng(precision, U'0'));
            }
          }
          else {
            s2.append(front(v));
            s2.append(U'.');
            s2.append(view_t(v2.begin(), j));
            for (auto &c : rrng(s2.begin() + s2_old_ssz, s2.end())) {
              if (to_unsigned(c) != U'.') {
                if (to_unsigned(c) != U'9') {
                  ++c;
                  break;
                }
                else
                  c = U'0';
              }
            }

            if (fold_zero) {
              while (to_unsigned(s2.back()) == U'0')
                s2.pop_back();
              if (to_unsigned(s2.back()) == U'.')
                s2.pop_back();
            }
          }
        }
      }
    }
    else {
      // sv_l is not empty

      e = ssize(sv_l);
      --e;
      const view_t sv_l_excluding_first(next(sv_l.begin()), sv_l.end());

      if (precision == 0u) {
        if (e == 0) {
          if (sv_r.empty())
            s2.append(front(sv_l));
          else {
            if (to_unsigned(front(sv_r)) < U'5')
              s2.append(front(sv_l));
            else {
              if (to_unsigned(front(sv_l)) < U'9')
                s2.append(to_unsigned(front(sv_l)) + 1u);
              else {
                s2.append(U'1');
                ++e;
              }
            }
          }
        }
        else {
          // sv_l has two or more elements
          if (to_unsigned(front(sv_l_excluding_first)) < U'5')
            s2.append(front(sv_l));
          else {
            if (to_unsigned(front(sv_l)) < U'9')
              s2.append(to_unsigned(front(sv_l)) + 1u);
            else {
              s2.append(U'1');
              ++e;
            }
          }
        }
      }
      else {
        const size_t l_numbers = ssize(sv_l) - 1u;
        const size_t numbers = l_numbers + size(sv_r);
        if (numbers <= precision) {
          s2.append(front(sv_l));
          if (!fold_zero) {
            s2.append(U'.');
            s2.append(sv_l_excluding_first);
            s2.append(sv_r);
            s2.append(rng(precision - numbers, U'0'));
          }
          else {
            if (!all_of_equal(sv_r, U'0', eqf)) {
              // sv_r is not empty
              s2.append(U'.');
              s2.append(sv_l_excluding_first);
              s2.append(view_t(sv_r.begin(), next(find_last_if(sv_r, not0))));
            }
            else {
              if (!all_of_equal(sv_l_excluding_first, U'0', eqf)) {
                // sv_l_excluding_first is not empty
                s2.append(U'.');
                s2.append
                  (view_t(sv_l_excluding_first.begin(),
                          next(find_last_if(sv_l_excluding_first, not0))));
              }
            }
          }
        }
        else if (precision < l_numbers) {
          const auto j = sv_l_excluding_first.begin() + precision;
          // j is dereferenceable
          if (to_unsigned(*j) < U'5') {
            s2.append(front(sv_l));
            s2.append(U'.');
            s2.append(view_t(sv_l_excluding_first.begin(), j));

            if (fold_zero) {
              while (to_unsigned(s2.back()) == U'0')
                s2.pop_back();
              if (to_unsigned(s2.back()) == U'.')
                s2.pop_back();
            }
          }
          else {
            if (all_of_equal(view_t(sv_l.begin(), j), U'9', eqf)) {
              s2.append(U'1');
              ++e;

              if (!fold_zero) {
                s2.append(U'.');
                s2.append(rng(precision, U'0'));
              }
            }
            else {
              s2.append(front(sv_l));
              s2.append(U'.');
              s2.append(view_t(next(sv_l.begin()), j));
              for (auto &c : rrng(nth(s2, s2_old_ssz), s2.end())) {
                if (to_unsigned(c) != U'.') {
                  if (to_unsigned(c) != U'9') {
                    ++c;
                    break;
                  }
                  else
                    c = U'0';
                }
              }

              if (fold_zero) {
                while (to_unsigned(s2.back()) == U'0')
                  s2.pop_back();
                if (to_unsigned(s2.back()) == U'.')
                  s2.pop_back();
              }
            }
          }
        }
        else {
          const auto j = next(sv_r.begin(), (precision - l_numbers));
          // j is dereferenceable
          if (to_unsigned(*j) < U'5') {
            s2.append(front(sv_l));
            s2.append(U'.');
            s2.append(view_t(next(sv_l.begin()), sv_l.end()));
            s2.append(view_t(sv_r.begin(), j));

            if (fold_zero) {
              while (to_unsigned(s2.back()) == U'0')
                s2.pop_back();
              if (to_unsigned(s2.back()) == U'.')
                s2.pop_back();
            }
          }
          else {
            if (all_of_equal(sv_l, U'9', eqf)
                && all_of_equal(view_t(sv_r.begin(), j), U'9', eqf)) {
              s2.append(U'1');
              ++e;

              if (!fold_zero) {
                s2.append(U'.');
                s2.append(rng(precision, U'0'));
              }
            }
            else {
              s2.append(front(sv_l));
              s2.append(U'.');
              s2.append(sv_l_excluding_first);
              s2.append(view_t(sv_r.begin(), j));
              for (auto &c : rrng(nth(s2, s2_old_ssz), s2.end())) {
                if (to_unsigned(c) != U'.') {
                  if (to_unsigned(c) != U'9') {
                    ++c;
                    break;
                  }
                  else
                    c = U'0';
                }
              }

              if (fold_zero) {
                while (to_unsigned(s2.back()) == U'0')
                  s2.pop_back();
                if (to_unsigned(s2.back()) == U'.')
                  s2.pop_back();
              }
            }
          }
        }
      }
    }
  }

  no_sign_fixed_fraction_string_apply_separator_arg(s2, s2_old_ssz, sep_n);
  s2.append(upper_case ? U'E' : U'e');
  sprint(s2, e, showpos, setseparator(sep_n));
  return s2;
}

}
class big_int {
  using this_t = big_int;
  using vec_t = small_vector<uint32_t, 8>;
  using vec_dif_t = vec_t::difference_type;
  using vec_iter_t = vec_t::iterator;

  bool sign = true;
  vec_t v;

  friend struct inner::fo_good;
  bool good() const noexcept {
    return v.empty() ? sign == true : back(v) != 0u;
  }

  void clear_tail_zero() {
    vec_iter_t i = v.end();
    for (vec_iter_t x : take_while_rng(rrng(irng(v)),
                                       [](auto i) {return *i == 0u;}))
      i = x;
    v.erase(i, v.end());
    if (v.empty()) {
      sign = true;
      return;
    }
  }

public:
  big_int() = default;
  ~big_int() = default;
  big_int(const big_int &) = default;
  big_int &operator =(const big_int &) = default;
  big_int(big_int &&x) noexcept : sign(x.sign), v(move(x.v)) {
    x.sign = true;
  }
  big_int &operator =(big_int &&x) noexcept {
    if (this != addressof(x)) {
      sign = x.sign;
      v = move(x.v);
      x.sign = true;
    }
    return *this;
  }
  friend void swap(this_t &x, this_t &y) noexcept {
    adl_swap(x.sign, y.sign);
    adl_swap(x.v, y.v);
  }

private:
  static strong_ordering abs_cmp(const vec_t &v1, const vec_t &v2) {
    const auto v1_sz = v1.size();
    const auto v2_sz = v2.size();
    if (v1_sz > v2_sz)
      return strong_gt;
    else if (v1_sz < v2_sz)
      return strong_lt;
    else
      return lexicographical_synth_3way(rrng(v1), rrng(v2));
  }
public:
  friend bool operator ==(const this_t &x, const this_t &y) noexcept = default;
  friend strong_ordering operator <=>(const this_t &x,
                                      const this_t &y) noexcept {
    if (x.sign) {
      if (y.sign)
        return abs_cmp(x.v, y.v);
      else
        return strong_gt;
    }
    else {
      if (y.sign)
        return strong_lt;
      else
        return reverse_ordering(abs_cmp(x.v, y.v));
    }
  }

private:
  void set_int64_impl(int64_t x) {
    uint64_t tmp;
    if (x < 0) {
      sign = false;
      tmp = static_cast<uint64_t>(-x);
    }
    else {
      sign = true;
      tmp = static_cast<uint64_t>(x);
    }
    uint32_t tmp2(tmp);
    uint32_t tmp3 = static_cast<uint32_t>((uint64_t)(tmp >> (uint64_t)32u));
    v.push_back(tmp2);
    v.push_back(tmp3);
    clear_tail_zero();
  }
  void set_int32_impl(int32_t x) {
    uint32_t xx{};
    if (x == 0) {
      sign = true;
      return;
    }
    else if (x < 0) {
      xx = to_unsigned(-x);
      sign = false;
    }
    else {
      xx = to_unsigned(x);
      sign = true;
    }
    v.push_back(xx);
  }
public:
  explicit big_int(int64_t x) {
    set_int64_impl(x);
  }
  explicit big_int(int32_t x) {
    set_int32_impl(x);
  }
  this_t &operator =(int64_t x) {
    v.clear();
    set_int64_impl(x);
    return *this;
  }
  this_t &operator =(int32_t x) {
    v.clear();
    set_int32_impl(x);
    return *this;
  }
  optional<int64_t> to_int64() const {
    if (v.empty())
      return optional<int64_t>(0);
    if (v.size() > 2u)
      return {};
    if (v.size() == 1u)
      return sign ? (int64_t)v[0] : -(int64_t)v[0];
    uint64_t x = (uint64_t)v[0] | (uint64_t)((uint64_t)v[1] << (uint64_t)32u);
    if (sign) {
      if (x > (uint64_t)numeric_limits<int64_t>::max())
        return {};
      return (int64_t)x;
    }
    else {
      if (x > (uint64_t)numeric_limits<int64_t>::max() + 1u)
        return {};
      return -(int64_t)x;
    }
  }
  optional<int32_t> to_int32() const {
    if (v.size() >= 2u)
      return {};
    else if (v.size() == 1u) {
      if (sign) {
        if (v.front() > to_unsigned(numeric_limits<int32_t>::max()))
          return {};
        return to_signed(v.front());
      }
      else {
        if (v.front() > to_unsigned(numeric_limits<int32_t>::max()) + 1u)
          return {};
        return -to_signed(v.front());
      }
    }
    else
      return optional<int32_t>(0);
  }

  bool is_non_negative() const noexcept {
    return sign;
  }
  void neg() noexcept {
    if (sign == true) {
      if (!v.empty())
        sign = false;
    }
    else
      sign = true;
  }

  bool is_zero() const noexcept {
    return v.empty();
  }

  using container_type = vec_t;
  const vec_t &data() const & noexcept {
    return v;
  }
  vec_t data() && noexcept {
    auto guard = exit_fn([&]() {sign = true;});
    return move(v);
  }
  void data(vec_t &&x) noexcept {
    v = move(x);
    if (v.empty())
      sign = true;
  }

  this_t &mul_pow_of_2(int32_t i) {
    if (v.empty())
      return *this;
    auto guard = exit_fn([&]() {clear_tail_zero();}, true);
    if (i > 0) {
      uint32_t n = i;
      const uint32_t q = n / 32u;
      const uint32_t r = n % 32u;
      uint32_t tmp = 0u;
      if (r != 0u)
        for (uint32_t &x : v) {
          const uint32_t tmp2 = (x >> (uint32_t)(32u - r));
          x = (uint32_t)(x << (uint32_t)r) | tmp;
          tmp = tmp2;
        }
      if (tmp != 0u)
        v.push_back(tmp);
      if (!v.empty())
        v.insert(v.begin(), rng(q, (uint32_t)0u));
    }
    else if (i < 0) {
      uint32_t n;
      n = -i;
      const uint32_t q = n / 32u;
      const uint32_t r = n % 32u;
      v.erase(v.begin(), next(v.begin(), q, v.end()));
      if (r != 0u) {
        uint32_t tmp = 0;
        for (uint32_t &x : rrng(v)) {
          const uint32_t tmp2 = (uint32_t)(x << (uint32_t)(32u - r));
          x = (uint32_t)(x >> (uint32_t)r) | tmp;
          tmp = tmp2;
        }
      }
      clear_tail_zero();
    }
    guard.unset();
    return *this;
  }
  this_t &mul_pow_of_10(int32_t i) {
    auto guard = exit_fn([&]() {clear_tail_zero();}, true);
    static const uint32_t a[] = {
      10u, 100u, 1000u, 10000u, 100000u, 1000000u, 10000000u, 100000000u,
      1000000000u
    };
    if (i > 0) {
      const uint32_t ui = i;
      const uint32_t k = size(a);
      const uint32_t q = ui / k;
      const uint32_t r = ui % k;
      for (auto c = q; c != 0; --c)
        mul(back(a));
      if (r != 0u)
        mul(a[(size_t)r - 1u]);
    }
    else if (i < 0) {
      const uint32_t ui = -i;
      const uint32_t k = size(a);
      const uint32_t q = ui / k;
      const uint32_t r = ui % k;
      for (auto c = q; c != 0; --c)
        div(back(a));
      if (r != 0u)
        div(a[(size_t)r - 1u]);
    }
    guard.unset();
    return *this;
  }
  this_t &mul(uint32_t x) {
    if (x == 0u) {
      v.clear();
      sign = true;
      return *this;
    }
    v.reserve_more(1u);
    const uint64_t xx = static_cast<uint64_t>(x);
    uint32_t pad = 0u;
    for (auto &it : iters(v)) {
      const uint64_t u = ((uint64_t)*it) * xx + pad;
      const uint32_t u_l = (u >> (uint64_t)32u);
      const uint32_t u_r = u;
      pad = u_l;
      *it = u_r;
    }
    if (pad != 0u)
      v.push_back(pad);
    return *this;
  }
  uint32_t div(uint32_t x) {
    if (x == 0u)
      throw_or_terminate<logic_error>("re::big_int: div by 0\n");
    uint64_t left = 0u;
    for (uint32_t &u : rrng(v)) {
      const uint64_t z = left | (uint64_t)u;
      u = z / x;
      left = z % x;
      left <<= (uint64_t)32u;
    }
    clear_tail_zero();
    left >>= 32u;
    return left;
  }

private:
  void abs_add_impl(const uint32_t &x, vec_dif_t dif) {
    uint64_t xx = x;
    for (auto &it : iters(nth(v, dif), v.end())) {
      const uint64_t tmp = static_cast<uint64_t>(*it) + xx;
      *it = tmp;
      xx = (tmp >> (uint64_t)32u);
      if (xx == 0u)
        break;
    }
  }
  void abs_add(const vec_t &vv) {
    v.resize((v.size() > vv.size()) ? (v.size() + 1u) : (vv.size() + 1u), 0u);
    vec_dif_t dif = 0;
    for (const uint32_t &x : vv) {
      abs_add_impl(x, dif);
      ++dif;
    }
    clear_tail_zero();
  }
  void abs_sub_smaller_impl(const uint32_t &x, vec_dif_t dif) {
    uint32_t xx = x;
    for (auto &it : iters(nth(v, dif), v.end())) {
      if (*it >= xx) {
        *it -= xx;
        break;
      }
      else {
        const uint64_t tmp = (uint64_t)*it
          + (uint64_t)((uint64_t)1u << (uint64_t)32u);
        *it = (uint32_t)(tmp - xx);
        xx = 1u;
      }
    }
  }
  void abs_sub_smaller(const vec_t &vv) {
    vec_dif_t dif = 0;
    for (const uint32_t &x : vv) {
      abs_sub_smaller_impl(x, dif);
      ++dif;
    }
    clear_tail_zero();
  }
public:
  this_t &operator +=(const this_t &x) {
    if (sign) {
      if (x.sign)
        abs_add(x.v);
      else {
        if (abs_cmp(v, x.v) >= 0)
          abs_sub_smaller(x.v);
        else {
          this_t tmp = x;
          tmp.abs_sub_smaller(v);
          *this = move(tmp);
        }
      }
    }
    else {
      if (x.sign) {
        if (abs_cmp(x.v, v) >= 0) {
          this_t tmp = x;
          tmp.abs_sub_smaller(v);
          *this = move(tmp);
        }
        else
          abs_sub_smaller(x.v);
      }
      else
        abs_add(x.v);
    }
    return *this;
  }
  this_t &operator -=(const this_t &x) {
    if (sign) {
      if (x.sign) {
        if (abs_cmp(v, x.v) >= 0)
          abs_sub_smaller(x.v);
        else {
          this_t tmp = x;
          tmp.abs_sub_smaller(v);
          tmp.sign = false;
          *this = move(tmp);
        }
      }
      else
        abs_add(x.v);
    }
    else {
      if (x.sign)
        abs_add(x.v);
      else {
        if (abs_cmp(x.v, v) >= 0) {
          this_t tmp = x;
          tmp.abs_sub_smaller(v);
          tmp.sign = true;
          *this = move(tmp);
        }
        else
          abs_sub_smaller(x.v);
      }
    }
    return *this;
  }
  this_t operator -(const this_t &x) const {
    this_t ret = *this;
    ret -= x;
    return ret;
  }
  this_t operator +(const this_t &x) const {
    this_t ret = *this;
    ret += x;
    return ret;
  }

private:
  // (optional sign)
  //
  // one((single number)
  //     + zero_or_more((optional '\'') + (single number))
  //
  // zero_or_one(('.' + (single number))
  //             + zero_or_more((optional '\') + (single number)))
  //
  // zero_or_one(('e' or 'E') + integer string)

  template <class C>
  bool sscan_sign(const C *&it, const C *ed, bool &s) {
    if (it == ed)
      return false;
    if (to_unsigned(*it) == U'+') {
      ++it;
      s = true;
    }
    else if (to_unsigned(*it) == U'-') {
      ++it;
      s = false;
    }
    else
      s = true;
    return true;
  }
  template <class C>
  bool sscan_single_number(const C *&it, const C *ed, int32_t &x) {
    if (it == ed)
      return false;
    if (inner::fns::is_unicode_digit(*it)) {
      x = to_unsigned(*it) - U'0';
      ++it;
      return true;
    }
    else
      return false;
  }
  template <class C>
  bool sscan_single_number_with_separator_prefix(const C *&it,
                                                 const C *ed,
                                                 int32_t &x) {
    if (it == ed)
      return false;
    if (to_unsigned(*it) == U'\'') {
      ++it;
      if (!sscan_single_number(it, ed, x)) {
        --it;
        return false;
      }
      else
        return true;
    }
    else
      return sscan_single_number(it, ed, x);
  }
  template <class C>
  bool sscan_dot_and_single_number(const C *&it, const C *ed,
                                   int32_t &x) {
    if (it == ed)
      return false;
    if (to_unsigned(*it) == U'.') {
      ++it;
      if (!sscan_single_number(it, ed, x)) {
        --it;
        return false;
      }
      else
        return true;
    }
    else
      return false;
  }

  template <class C>
  bool sscan_single_hex_number(const C *&it, const C *ed, int32_t &x) {
    if (it == ed)
      return false;
    if (inner::fns::is_unicode_digit(*it)) {
      x = to_unsigned(*it) - U'0';
      ++it;
      return true;
    }
    else if (U'a' <= to_unsigned(*it) && to_unsigned(*it) <= U'f') {
      x = 10u + (to_unsigned(*it) - U'a');
      ++it;
      return true;
    }
    else if (U'A' <= to_unsigned(*it) && to_unsigned(*it) <= U'F') {
      x = 10u + (to_unsigned(*it) - U'A');
      ++it;
      return true;
    }
    else
      return false;
  }
  template <class C>
  bool sscan_single_hex_number_with_separator_prefix(const C *&it,
                                                     const C *ed,
                                                     int32_t &x) {
    if (it == ed)
      return false;
    if (to_unsigned(*it) == U'\'') {
      ++it;
      if (!sscan_single_hex_number(it, ed, x)) {
        --it;
        return false;
      }
      else
        return true;
    }
    else
      return sscan_single_hex_number(it, ed, x);
  }
  template <class C>
  bool sscan_dot_and_single_hex_number(const C *&it, const C *ed,
                                       int32_t &x) {
    if (it == ed)
      return false;
    if (to_unsigned(*it) == U'.') {
      ++it;
      if (!sscan_single_hex_number(it, ed, x)) {
        --it;
        return false;
      }
      else
        return true;
    }
    else
      return false;
  }

  template <class C>
  bool sscan_e_and_int32(const C *&it, const C *ed, int32_t &x) {
    if (it == ed)
      return false;
    if (*it == 'e' || *it == 'E') {
      ++it;
      string_reference<const C> sv(it, ed);
      if (re::sscan(sv, x)) {
        it = sv.begin();
        return true;
      }
      else {
        --it;
        return false;
      }
    }
    else
      return false;
  }

  template <class S>
  S sprint_impl(size_t sep_n, const char *num, uint32_t base) const {
    S s;
    if (!sign)
      s.push_back(to_unsigned('-'));
    if (v.empty()) {
      s.push_back(to_unsigned('0'));
      return s;
    }
    const ptrdiff_t num_pos_dif = ssize(s);
    auto cp = *this;
    for (;;) {
      uint32_t rem = cp.div(base);
      s.push_back(to_unsigned(num[rem]));
      if (cp.v.empty())
        break;
    }
    const auto r = rng(s.begin() + num_pos_dif, s.end());
    reverse(r);
    if (sep_n != 0u) {
      const size_t digit_len = size(r);
      size_t sep_count = digit_len / sep_n;
      const size_t rem = digit_len % sep_n;
      if (sep_count != 0) {
        if (rem == 0)
          --sep_count;
        const auto r_len = ssize(r);
        s.reserve_more(sep_count);
        s.resize(s.size() + sep_count);
        const auto r_op = s.end() - to_signed(sep_count) - r_len;
        auto it = s.end() - to_signed(sep_count);
        auto it2 = s.end();
        for (size_t k = 0u; r_op != it && it != it2;) {
          *--it2 = *--it;
          ++k;
          if (k == sep_n) {
            k = 0u;
            *--it2 = '\'';
          }
        }
      }
    }
    return s;
  }

public:
  template <class T>
  bool sscan(string_reference<const T> &sv) {
    auto guard = exit_fn([&]() {operator =(0);}, true);
    auto it = sv.begin();
    bool sign_is_positive = false;
    {
      if (!sscan_sign(it, sv.end(), sign_is_positive))
        return false;
      int32_t x{};
      if (!sscan_single_number(it, sv.end(), x))
        return false;
      *this = x;
      while (sscan_single_number_with_separator_prefix(it, sv.end(), x))
        mul(10u) += this_t(x);
    }
    sign = sign_is_positive;
    if (v.empty())
      sign = true;
    sv = {it, sv.end()};
    guard.unset();
    return true;
  }
  bool sscan(sview &sv) {
    return this->sscan<char>(sv);
  }
  template <class S = string>
  S sprint(size_t sep_n = 0u) const {
    return sprint_impl<S>(sep_n, inner::from_0_to_f, 10u);
  }

  template <class T>
  bool sscan_hex(string_reference<const T> &sv) {
    auto guard = exit_fn([&]() {operator =(0);}, true);
    auto it = sv.begin();
    bool sign_is_positive = false;
    {
      if (!sscan_sign(it, sv.end(), sign_is_positive))
        return false;
      int32_t x{};
      if (!sscan_single_hex_number(it, sv.end(), x))
        return false;
      *this = x;
      while (sscan_single_hex_number_with_separator_prefix(it, sv.end(), x))
        mul(16u) += this_t(x);
    }
    sign = sign_is_positive;
    if (v.empty())
      sign = true;
    sv = {it, sv.end()};
    guard.unset();
    return true;
  }
  bool sscan_hex(sview &sv) {
    return this->sscan_hex<char>(sv);
  }
  template <class S = string>
  S sprint_hex(size_t sep_n = 0u, bool upper_case = false) const {
    return sprint_impl<S>(sep_n,
                          upper_case ? inner::from_0_to_F : inner::from_0_to_f,
                          16u);
  }

  template <class T>
  bool sscan_scientific(string_reference<const T> &sv) {
    auto guard = exit_fn([&]() {operator =(0);}, true);
    {
      string_reference<const T> sv2 = sv;
      if (!sscan(sv2))
        return false;

      int32_t e = 0;
      auto it = sv2.begin();
      int32_t x{};
      const bool sign_copy = sign;
      sign = true;
      if (sscan_dot_and_single_number(it, sv2.end(), x)) {
        mul(10u) += this_t(x);
        --e;
        while (sscan_single_number_with_separator_prefix(it, sv2.end(), x)) {
          mul(10u) += this_t(x);
          --e;
          if (e == numeric_limits<int32_t>::min())
            return false;
        }
      }
      sign = sign_copy;

      if (sscan_e_and_int32(it, sv2.end(), x)) {
        const int64_t tmp = (int64_t)e + (int64_t)x;
        if (tmp > numeric_limits<int32_t>::max()
            || tmp < -numeric_limits<int32_t>::max())
          return false;
        e = (int32_t)tmp;
      }
      else
        return false;

      if (e >= 0)
        mul_pow_of_10(e);
      else {
        mul_pow_of_10(e + 1);
        if (div(10u) >= 5) {
          if (is_non_negative())
            *this += this_t(1);
          else
            *this -= this_t(1);
        }
      }

      sv = {it, sv.end()};
    }
    guard.unset();
    return true;
  }
  bool sscan_scientific(sview &sv) {
    return this->sscan_scientific<char>(sv);
  }
  template <class S = string>
  S sprint_scientific(size_t precision,
                      bool fold_zero,
                      size_t sep_n = 0u,
                      bool upper_case = false,
                      bool show_pos = false) const {
    const S s = sprint<S>();
    string_reference<const rng_vt<S>> sv = s;
    return inner::fns::fixed_fraction_string_to_scientific<S>
      (sv, precision, sep_n, upper_case, show_pos, fold_zero);
  }
  template <class S = string>
  S sprint_scientific(size_t precision) const {
    return sprint_scientific<S>(precision,
                                (precision == numeric_limits<size_t>::max()),
                                0u, false, false);
  }
  template <class S = string>
  S sprint_scientific() const {
    return sprint_scientific<S>(6u, false, 0u, false, false);
  }
};

}

// sprint float
// sscan float
namespace re {

namespace inner::fns {

template <class S>
S fixed_fraction_string_apply_print_args(string_reference<const rng_vt<S>> sv,
                                         size_t precision, size_t sep_n,
                                         bool show_pos,
                                         bool fold_zero) {
  // sv has valid format and is not empty
  // sv has no leading zero except "0."
  using view_t = string_reference<const rng_vt<S>>;
  using ref_t = string_reference<rng_vt<S>>;
  S s2;
  if (to_unsigned(front(sv)) == U'+') {
    sv = {next(sv.begin()), sv.end()};
    if (show_pos)
      s2.append(U'+');
  }
  else if (to_unsigned(front(sv)) == U'-') {
    sv = {next(sv.begin()), sv.end()};
    s2.append(U'-');
  }
  else {
    if (show_pos)
      s2.append(U'+');
  }

  const auto s2_old_ssz = ssize(s2);
  const auto dot_pos = find_if(sv, [](auto c) {return to_unsigned(c) == U'.';});
  view_t r1(sv.begin(), dot_pos); // r1 is not empty
  view_t r2((dot_pos != sv.end() ? next(dot_pos) : sv.end()), sv.end());
  const auto eqf = [](auto c1, auto c2) {return to_unsigned(c1) == c2;};
  const auto not0 = [](auto c) {return to_unsigned(c) != U'0';};
  if (r2.empty()) {
    s2.append(r1);
    if (precision != 0u && !fold_zero) {
      s2.append(U'.');
      s2.append(rng(precision, U'0'));
    }
  }
  else {
    if (precision == 0u) {
      if (to_unsigned(r2.front()) < U'5')
        s2.append(r1);
      else {
        if (all_of_equal(r1, U'9', eqf)) {
          s2.append(U'1');
          s2.append(rng(size(r1), U'0'));
        }
        else {
          const auto old_ssz = ssize(s2);
          s2.append(r1);
          for (auto &j : r_iters(nth(s2, old_ssz), s2.end())) {
            auto &c = *j;
            if (to_unsigned(c) != U'9') {
              ++c;
              break;
            }
            else
              c = U'0';
          }
        }
      }
    }
    else if (precision >= r2.size()) {
      s2.append(r1);
      if (!fold_zero) {
        s2.append(U'.');
        s2.append(r2);
        s2.append(rng(precision - size(r2), U'0'));
      }
      else {
        if (!all_of_equal(r2, U'0', eqf)) {
          // r2 is not empty
          s2.append(U'.');
          s2.append(view_t(r2.begin(), next(find_last_if(r2, not0))));
        }
      }
    }
    else {
      const view_t v(r2.begin(), nth(r2, to_signed(precision)));
      if (to_unsigned(ref(r2, to_signed(precision))) < U'5') {
        s2.append(r1);
        s2.append(U'.');
        s2.append(v);

        if (fold_zero) {
          while (to_unsigned(s2.back()) == U'0')
            s2.pop_back();
          if (to_unsigned(s2.back()) == U'.')
            s2.pop_back();
        }
      }
      else {
        if (all_of_equal(r1, U'9', eqf) && all_of_equal(v, U'9', eqf)) {
          s2.append(U'1');
          s2.append(rng(size(r1), U'0'));

          if (!fold_zero) {
            s2.append(U'.');
            s2.append(rng(size(v), U'0'));
          }
        }
        else {
          s2.append(r1);
          s2.append(U'.');
          s2.append(v);
          auto it = s2.end();
          for (;;) {
            --it;
            if (to_unsigned(*it) != U'.') {
              if (to_unsigned(*it) != U'9') {
                ++*it;
                break;
              }
              else
                *it = U'0';
            }
          }

          if (fold_zero) {
            while (to_unsigned(s2.back()) == U'0')
              s2.pop_back();
            if (to_unsigned(s2.back()) == U'.')
              s2.pop_back();
          }
        }
      }
    }
  }

  inner::fns::no_sign_fixed_fraction_string_apply_separator_arg
    (s2,
     ((to_unsigned(s2.front()) == U'+' || to_unsigned(s2.front()) == U'-')
      ? 1 : 0),
     sep_n);
  return s2;
}

template <class F, class FN>
F sscan_float_final_step(int32_t e2, FN take_n_bits, bool non_neg) {
  // first 1 is reached
  // may approzimately get zero, but the original string is not zero

  using traits = floating_point_traits<F>;
  using float_t = F;
  using uint_t = typename traits::uint_t;
  using int_t = typename traits::int_t;

  const int32_t e2_max = traits::e_max - 1 - to_signed(traits::bias);
  const int32_t e2_min = 1 - to_signed(traits::bias);

  if (e2 > e2_max)
    return non_neg ? traits::positive_inf() : traits::negative_inf();
  else if (e2 >= e2_min) {
    uint64_t f = 0u;
    take_n_bits(traits::k + 1u, f);
    if (f == (uint64_t)(numeric_limits<uint64_t>::max()
                        >> (uint64_t)(64u - (traits::k + 1u)))) {
      f = 0u;
      ++e2;
      if (e2 > e2_max)
        return non_neg ? traits::positive_inf() : traits::negative_inf();
    }
    else {
      if ((uint64_t)(f << (uint64_t)63u) != 0u) {
        f >>= (uint64_t)1u;
        ++f;
      }
      else
        f >>= (uint64_t)1u;
    }

    uint_t z = 0u;
    if (!non_neg)
      z = (uint_t)((uint_t)1u << (uint_t)(traits::n + traits::k));
    z += (uint_t)((uint_t)(e2 + to_signed(traits::bias)) << traits::k);
    z += (uint_t)f;
    return bit_cast<float_t>(z);
  }
  else {
    // 0.XXXXXXXXX      e2_min
    //   |<- k ->|
    // 0 XXXX1.XXX      e2
    //       XXXXX.X    e2_lim = e2_min - k

    const int32_t e2_lim = e2_min - to_signed(traits::k);
    if (e2 >= e2_lim) {
      const size_t bits = to_unsigned(e2 - e2_lim) + 1u;

      uint64_t f = 1u;
      take_n_bits(bits, f);

      if (f == (uint64_t)(numeric_limits<uint64_t>::max()
                          >> (uint64_t)(64u - (bits + 1u))))
        return non_neg ? traits::min() : -traits::min();

      if ((uint64_t)(f << (uint64_t)63u) != 0u) {
        f >>= (uint64_t)1u;
        ++f;
      }
      else
        f >>= (uint64_t)1u;
      uint_t z = 0u;
      if (!non_neg)
        z = (uint_t)((uint_t)1u << (uint_t)(traits::n + traits::k));
      z += (uint_t)f;
      return bit_cast<float_t>(z);
    }
    else if (e2 == e2_lim - 1)
      return non_neg
        ? numeric_limits<float_t>::denorm_min()
        : -numeric_limits<float_t>::denorm_min();
    else
      return non_neg ? +0.0f : -0.0f;
  }
}

}
template <class F, class S>
S &fo_sprint::impl_for_float(S &&s, F x, const float_print_args &pa)
  requires inner::sprint_s_arg<S> {
  typename decay<S>::size_type old_sz = s.size();
  auto guard = exit_fn([&]() {s.resize(old_sz);}, true);

  using traits = floating_point_traits<F>;

  const auto sprint_sv = [](auto &ss, sview sv, const print_args &pa) {
    const auto sz = size(sv);
    const auto v = bind_rng(sv, to_unsigned);
    if (sz >= pa.min_width)
      ss.append_range(v);
    else {
      const auto padding_r = rng(pa.min_width - sz,
                                 inner::fns::padding_char_to<rng_vt<S>>
                                 (pa.padding_char));
      if (pa.left_padding) {
        ss.append_range(padding_r);
        ss.append_range(v);
      }
      else {
        ss.append_range(v);
        ss.append_range(padding_r);
      }
    }
  };
  if (traits::is_nan(x))
    sprint_sv(s, (pa.upper_case ? "NAN"_sv : "nan"_sv),
              static_cast<const print_args &>(pa));
  else if (traits::is_infinity(x)) {
    if (traits::sign(x))
      sprint_sv(s,
                (pa.upper_case
                 ? (pa.show_positive_symbol ? "+INF"_sv : "INF"_sv)
                 : (pa.show_positive_symbol ? "+inf"_sv : "inf"_sv)),
                pa);
    else
      sprint_sv(s, (pa.upper_case ? "-INF"_sv : "-inf"_sv), pa);
  }
  else {
    if (x == (F)0.0f) {
      if (floating_point_traits<F>::sign(x)) {
        if (pa.show_positive_symbol)
          s.push_back(U'+');
      }
      else
        s.push_back(U'-');
      s.push_back(U'0');
    }
    else {
      const auto f = traits::is_normalized(x)
        ? traits::normalized_final_f(x) : traits::denormalized_final_f(x);
      const auto e = traits::is_normalized(x)
        ? traits::normalized_final_e(x) : traits::denormalized_final_e(x);

      big_int dot_left(to_signed(f));
      dot_left.mul_pow_of_2((int32_t)e);

      big_int dot_right(to_signed(f));
      big_int tmp = dot_left;
      tmp.mul_pow_of_2((int32_t)-e);
      dot_right -= tmp;

      if (x >= 0.0) {
        if (pa.show_positive_symbol)
          s.push_back(U'+');
      }
      else
        s.push_back(U'-');
      s.append_range(dot_left.sprint<decay<S>>());

      size_t k = 0;
      if (dot_right.is_zero() == false) {
        s.push_back(U'.');

        big_int &num = dot_right;
        big_int den(1);
        den.mul_pow_of_2((int32_t)-e);

        for (;;) {
          num.mul(10u);

          big_int n = num;
          n.mul_pow_of_2(e);
          ++k;
          const int64_t number_0_9 = *n.to_int64();
          s.push_back(to_unsigned(inner::from_0_to_f[number_0_9]));

          if (number_0_9 != 0) {
            big_int z = den;
            z.mul((uint32_t)number_0_9);
            num -= z;
            if (num.is_zero())
              break;
          }
        }
      }
    }

    const string_reference<const rng_vt<S>> r(nth(as_const(s), old_sz),
                                              s.cend());
    decay<S> s2
      = !pa.scientific_notation
      ? (inner::fns::fixed_fraction_string_apply_print_args<decay<S>>
         (r, pa.precision, pa.separator_n,
          pa.show_positive_symbol, pa.fold_zero))
      : (inner::fns::fixed_fraction_string_to_scientific<decay<S>>
         (r, pa.precision, pa.separator_n, pa.upper_case,
          pa.show_positive_symbol, pa.fold_zero));
    s.replace(r.begin(), r.end(), s2);

    const auto new_sz = s.size() - old_sz;
    if (new_sz < pa.min_width) {
      const auto tmp_r = rng(pa.min_width - new_sz,
                             inner::fns::padding_char_to<rng_vt<S>>
                             (pa.padding_char));
      if (pa.left_padding)
        s.insert_range(nth(s, to_signed(old_sz)), tmp_r);
      else
        s.append_range(tmp_r);
    }
  }

  guard.unset();
  return s;
}

namespace inner::fns {

template <class T>
inline bool f_sscan_sign(string_reference<T> &v, bool &s) {
  if (v.empty())
    return false;
  if (to_unsigned(v.front()) == U'+') {
    v = {next(v.begin()), v.end()};
    s = true;
  }
  else if (to_unsigned(v.front()) == U'-') {
    v = {next(v.begin()), v.end()};
    s = false;
  }
  else
    s = true;
  return true;
}

template <class T>
inline bool f_sscan_single_number(string_reference<T> &v, int32_t &x) {
  if (v.empty())
    return false;
  if (inner::fns::is_unicode_digit(v.front())) {
    x = to_unsigned(v.front()) - U'0';
    v = {next(v.begin()), v.end()};
    return true;
  }
  else
    return false;
}

template <class T>
inline bool f_sscan_single_number_with_separator_prefix
(string_reference<T> &v, int32_t &x) {
  if (v.empty())
    return false;
  if (to_unsigned(v.front()) == U'\'') {
    v = {next(v.begin()), v.end()};
    if (!inner::fns::f_sscan_single_number(v, x)) {
      v = {prev(v.begin()), v.end()};
      return false;
    }
    else
      return true;
  }
  else
    return inner::fns::f_sscan_single_number(v, x);
}

template <class T>
inline bool f_sscan_dot_and_single_number(string_reference<T> &v,
                                          int32_t &x) {
  if (v.empty())
    return false;
  if (to_unsigned(v.front()) == U'.') {
    v = {next(v.begin()), v.end()};
    if (!inner::fns::f_sscan_single_number(v, x)) {
      v = {prev(v.begin()), v.end()};
      return false;
    }
    else
      return true;
  }
  else
    return false;
}

template <class T>
inline bool f_sscan_e_and_big_int(string_reference<T> &v, big_int &x) {
  if (v.empty())
    return false;
  if (to_unsigned(v.front()) == U'e' || to_unsigned(v.front()) == U'E') {
    string_reference<T> v2(next(v.begin()), v.end());
    if (x.sscan(v2)) {
      v = v2;
      return true;
    }
    else
      return false;
  }
  else
    return false;
}

}
template <class T, class F>
bool fo_sscan::impl_for_float(string_reference<T> &v, F &o) {
  using traits = floating_point_traits<F>;
  using float_t = F;
  using uint_t = typename traits::uint_t;
  using int_t = typename traits::int_t;

  const auto char_eq = [](char a, char b) {return tolower(a) == b;};
  if (starts_with(bind_rng(v, to_unsigned), U"nan"_sv, char_eq)) {
    o = traits::positive_nan();
    v = {v.begin() + 3, v.end()};
    return true;
  }
  else if (starts_with(bind_rng(v, to_unsigned), U"+nan"_sv, char_eq)) {
    o = traits::positive_nan();
    v = {v.begin() + 4, v.end()};
    return true;
  }
  else if (starts_with(bind_rng(v, to_unsigned), U"-nan"_sv, char_eq)) {
    o = traits::negative_nan();
    v = {v.begin() + 4, v.end()};
    return true;
  }

  if (starts_with(bind_rng(v, to_unsigned), U"inf"_sv, char_eq)) {
    o = traits::positive_inf();
    v = {v.begin() + 3, v.end()};
    return true;
  }
  else if (starts_with(bind_rng(v, to_unsigned), U"+inf"_sv, char_eq)) {
    o = traits::positive_inf();
    v = {v.begin() + 4, v.end()};
    return true;
  }
  else if (starts_with(bind_rng(v, to_unsigned), U"-inf"_sv, char_eq)) {
    o = traits::negative_inf();
    v = {v.begin() + 4, v.end()};
    return true;
  }

  bool non_neg = false;
  big_int x{};
  int32_t e = 0;
  string_reference<T> v2 = v;
  int32_t tmp{};

  make_signed<size_t> order = 0;
  bool touched_1st_non0 = false;
  const auto update_order = [&]() {
    if (touched_1st_non0)
      ++order;
    else if (tmp != 0) {
      touched_1st_non0 = true;
      ++order;
    }
  };

  if (!inner::fns::f_sscan_sign(v2, non_neg))
    return false;

  if (!inner::fns::f_sscan_single_number(v2, tmp))
    return false;
  update_order();
  x = tmp;

  while (inner::fns::f_sscan_single_number_with_separator_prefix(v2, tmp)) {
    update_order();
    x.mul(10u) += big_int(tmp);
  }

  if (inner::fns::f_sscan_dot_and_single_number(v2, tmp)) {
    --e;
    update_order();
    x.mul(10u) += big_int(tmp);
    while (inner::fns
           ::f_sscan_single_number_with_separator_prefix(v2, tmp)) {
      update_order();
      x.mul(10u) += big_int(tmp);
      --e;
      if (e == numeric_limits<int32_t>::min())
        return false;
    }
  }

  if (big_int tmp_e; inner::fns::f_sscan_e_and_big_int(v2, tmp_e)) {
    tmp_e += big_int(e);

    big_int order_final = tmp_e;
    order_final -= big_int(1);
    order_final += big_int(order);
    if (order_final > big_int(308)) {
      o = non_neg ? traits::positive_inf() : traits::negative_inf();
      v = v2;
      return true;
    }
    else if (order_final < big_int(-324)) {
      o = non_neg ? (float_t)+0.0f : (float_t)-0.0f;
      v = v2;
      return true;
    }

    const auto z = tmp_e.to_int32();
    if (z.empty() || *z == numeric_limits<int32_t>::min())
      return false;
    e = *z;
  }
  else {
    order += (e - 1);
    if (!x.is_zero()) {
      if (order > 308) {
        o = non_neg ? traits::positive_inf() : traits::negative_inf();
        v = v2;      
        return true;
      }
      else if (order < -324) {
        o = non_neg ? (float_t)+0.0f : (float_t)-0.0f;
        v = v2;
        return true;
      }
    }
  }

  big_int w = x;
  w.mul_pow_of_10(e);
  x -= big_int(w).mul_pow_of_10(-e);
  // left part of dot: w
  // right part of dot: x * 10^e

  if (w.is_zero()) {
    if (x.is_zero())
      o = non_neg ? (float_t)+0.0f : (float_t)-0.0f;
    else {
      big_int den(1);
      den.mul_pow_of_10(-e);
      // x / den

      int32_t e2 = 0;
      for (;;) {
        --e2;
        if (x.mul_pow_of_2(1) >= den) {
          x -= den;
          break;
        }
      }
      // (1 + x / den) * 2^e2   (x < den)

      const auto take_n_bits = [&](size_t bits, uint64_t &f) {
        // f == 0u for normalized
        // f == 1u for denormalized
        for (size_t c = 0; c != bits; ++c) {
          if (x.is_zero()) {
            f <<= (uint64_t)(bits - c);
            break;
          }
          else {
            x.mul_pow_of_2(1);
            f <<= (uint64_t)1u;
            if (x >= den) {
              ++f;
              x -= den;
            }
          }
        }
      };
      o = inner::fns::sscan_float_final_step<float_t>
        (e2, take_n_bits, non_neg);
    }
  }
  else {
    // left part of dot: w
    // right part of dot: x * 10^e
    //
    // w != 0

    int32_t e2 = 0;
    if (w.data().size() > 3u) {
      const int32_t k = to_signed((w.data().size() - 3u) * 32u);
      w.mul_pow_of_2(-k);
      e2 += k;
      // note: at this point, the right part of dot is no more possible to
      //   reach because the left part is big enough
    }

    vector<bool> s;
    for (;;) {
      s.push_back((uint32_t)(w.data().front() << (uint32_t)31u) != 0u);
      w.mul_pow_of_2(-1);
      ++e2;
      if (w.is_zero())
        break;
    }
    s.pop_back();
    --e2;

    const auto take_n_bits = [&](size_t bits, uint64_t &f) {
      // f == 0u for normalized
      // f == 1u for denormalized
      for (size_t c = 0; c != bits; ++c) {
        if (s.empty()) {
          for (size_t cc = bits - c; cc != 0; --cc) {
            // right part of dot: x * 10^e
            big_int den(1);
            den.mul_pow_of_10(-e);

            if (x.is_zero()) {
              f <<= (uint64_t)cc;
              return;
            }
            else {
              f <<= (uint64_t)1u;
              if (x.mul_pow_of_2(1) >= den) {
                ++f;
                x -= den;
              }
            }
          }
          return;
        }
        else {
          f <<= (uint64_t)1u;
          if (s.back())
            ++f;
          s.pop_back();
        }
      }
    };
    o = inner::fns::sscan_float_final_step<float_t>(e2, take_n_bits, non_neg);
  }

  v = v2;
  return true;
}

}

// ssplitter
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

  template <class SEARCH_F = decay<decltype(search)>>
  ssplitter(view_t s, view_t x, SEARCH_F search_f = SEARCH_F{}) {
    const auto ed = re::end(s);
    const auto x_sz = re::size(x);
    if (x_sz == 1u) {
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
      if (auto it = re::begin(s); it != ed) {
        if (x.empty())
          ss.append(STR(s));
        else {
          for (;;) {
            const auto it2 = search_f(rng(it, ed), x);
            ss.append(STR(it, it2));
            if (it2 == ed)
              break;
            it = it2 + x_sz;
          }
        }
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
  bool empty() const {
    return re::empty(ss);
  }
  vec_t::size_type size() const {
    return ss.size();
  }
  void clear() {
    ss.clear();
    ss.shrink_to_fit();
  }

  template <class OI>
  enable_if<!is_rng<OI>, OI> operator ()(view_t v, OI o) const {
    if (!empty()) {
      const auto ed = re::end(ss);
      const auto ed_pr = prev(ed);
      auto it = re::begin(ss);
      for (; it != ed_pr; ++it) {
        *o++ = *it;
        *o++ = v;
      }
      *o++ = *it;
    }
    return o;
  }
  template <class IT, class OI>
  enable_if<!is_rng<IT> && !is_convertible<IT, view_t>, pair<IT, OI>>
  operator ()(IT iter, OI o) const {
    if (!empty()) {
      const auto ed = re::end(ss);
      const auto ed_pr = prev(ed);
      auto it = re::begin(ss);
      for (; it != ed_pr; ++it) {
        *o++ = *it;
        *o++ = *iter;
        ++iter;
      }
      *o++ = *it;
    }
    return {iter, o};
  }
  template <class R, class OI>
  enable_if<is_rng<R> && !is_convertible<R, view_t>, pair<rng_itr<R>, OI>>
  operator ()(R &&r, OI o) const {
    if (!re::empty(r)) {
      bool used_out = false;
      if (!empty()) {
        auto p = rng(r);
        const auto r_ed = re::end(r);
        const auto ed = re::end(ss);
        const auto ed_pr = prev(ed);
        auto it = re::begin(ss);
        for (; it != ed_pr; ++it) {
          *o = *it;
          ++o;
          *o = *p.first;
          ++o;
          if (next(p.first) != r_ed)
            ++p.first;
          else
            used_out = true;
        }
        *o = *it;
        ++o;
        return {(used_out ? r_ed : p.first), o};
      }
    }
    return {re::begin(r), o};
  }
};

struct fo_ssplit {
  template <class S, class OI>
  OI operator ()(S &&s, string_reference<add_const<rng_vt<S>>> delimiter,
                 OI o) const {
    using str_t = remove_reference<S>;
    using sv_t = string_reference<add_const<rng_vt<S>>>;

    if (s.empty())
      return o;
    if (delimiter.empty()) {
      *o = sv_t(begin(s), end(s));
      ++o;
      return o;
    }

    const auto d_sz = ssize(delimiter);
    const auto s_ed = end(s);
    auto s_it = begin(s);
    for (;;) {
      const auto it = search(rng(s_it, s_ed), delimiter);
      *o = sv_t(s_it, it);
      ++o;
      if (it == s_ed)
        break;
      s_it = it;
      advance(s_it, d_sz);
    }
    return o;
  }
};
inline constexpr fo_ssplit ssplit{};

}

// arithmetic_parser
namespace re {

struct default_arithmetic_grammar {
  enum class opr {
    paren,
    pos, neg,
    pow,
    mul, div,
    add, sub,
  };
  using operator_type = opr;
  using operand_type = double;
  using stack_type = vector<operand_type>;
  using function_type = function<operand_type (const stack_type &)>;
  using char_type = char;
  using string_type = string;
  using view_type = sview;

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
             )
            );
    return g;
  }

private:
  flat_map<string, flat_map<int, function_type>> m;
  void init_m() {
    using fn_t = function_type;
    m["abs"] = {
      {1, fn_t([](const stack_type &s) {return abs(back(s));})}
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

private:
  using this_t = default_arithmetic_grammar;
public:
  default_arithmetic_grammar() {
    init_m();
  }
  ~default_arithmetic_grammar() = default;
  default_arithmetic_grammar(const this_t &) = delete;
  this_t &operator =(this_t &) = delete;
  default_arithmetic_grammar(this_t &&) noexcept = default;
  this_t &operator =(this_t &&) noexcept = default;
  friend void swap(this_t &x1, this_t &x2) noexcept {
    adl_swap(x1.m, x2.m);
  }
};
struct default_arithmetic_grammar_u32 {
  enum class opr {
    paren,
    pos, neg,
    pow,
    mul, div,
    add, sub,
  };
  using operator_type = opr;
  using operand_type = double;
  using stack_type = vector<operand_type>;
  using function_type = function<operand_type (const stack_type &)>;
  using char_type = char32_t;
  using string_type = u32string;
  using view_type = u32sview;

  const auto &operator_list() const {
    using fn_t = function_type;
    static const auto g
      = vec(hvec(true)
            (hvec(opr::paren, 1,
                  fn_t([](const stack_type &s) {return back(s);}))
             (U"("_sv, U"", U")")
             ),

            hvec(true)
            (hvec(opr::pos, 1,
                  fn_t([](const stack_type &s) {return back(s);}))
             (U"+"_sv, U""),
             hvec(opr::neg, 1,
                  fn_t([](const stack_type &s) {return -back(s);}))
             (U"-"_sv, U"")
             ),

            hvec(true)
            (hvec(opr::pow, 2,
                  fn_t([](const stack_type &s) {
                    return pow(*prev(s.end(), 2), back(s));
                  }))
             (U""_sv, U"^", U"")
             ),

            hvec(true)
            (hvec(opr::mul, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) * back(s);
                  }))
             (U""_sv, U"*", U""),
             hvec(opr::div, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) / back(s);
                  }))
             (U""_sv, U"/", U"")
             ),

            hvec(true)
            (hvec(opr::add, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) + back(s);
                  }))
             (U""_sv, U"+", U""),
             hvec(opr::sub, 2,
                  fn_t([](const stack_type &s) {
                    return *prev(s.end(), 2) - back(s);
                  }))
             (U""_sv, U"-", U"")
             )
            );
    return g;
  }

private:
  flat_map<u32string, flat_map<int, function_type>> m;
  void init_m() {
    using fn_t = function_type;
    m[U"abs"] = {
      {1, fn_t([](const stack_type &s) {return abs(back(s));})}
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

private:
  using this_t = default_arithmetic_grammar_u32;
public:
  default_arithmetic_grammar_u32() {
    init_m();
  }
  ~default_arithmetic_grammar_u32() = default;
  default_arithmetic_grammar_u32(const this_t &) = delete;
  this_t &operator =(this_t &) = delete;
  default_arithmetic_grammar_u32(this_t &&) noexcept = default;
  this_t &operator =(this_t &&) noexcept = default;
  friend void swap(this_t &x1, this_t &x2) noexcept {
    adl_swap(x1.m, x2.m);
  }
};

namespace inner {

template <class G>
struct arithmetic_grammar_opr_block {
  using operator_sequence_type
    = decltype(front(front(declval<const G &>().operator_list()).vec).vec);

  bool left_to_right;
  int precedence;
  int operand_count;
  const typename G::function_type *fn_ptr;
  const operator_sequence_type *seq_ptr;
  rng_itr<const operator_sequence_type &> seq_pos;
    // only stop at operand position or the end
};
template <class G>
struct arithmetic_grammar_fn_block {
  const remove_cvref<decltype(declval<const G &>().function_list()
                              [declval<const typename G::string_type &>()])
                     > *overload_list_ptr;
  int arg_pos;
};

template <class T>
concept can_shrink_to_fit = requires (T &x) {
  x.shrink_to_fit();
};

}
template <class G>
class arithmetic_parser {
  using opr_node_t = inner::arithmetic_grammar_opr_block<G>;
  using fn_node_t = inner::arithmetic_grammar_fn_block<G>;
  using s_node_t = variant<opr_node_t, fn_node_t>;

  G g;
  vector<s_node_t> s;
  typename G::stack_type opd_s;

  template <bool USE_FAILED_POS>
  static void assign_failed_pos(typename G::view_type::iterator *,
                                typename G::view_type::iterator) {}
  template <bool USE_FAILED_POS>
  static void assign_failed_pos(typename G::view_type::iterator *failed_pos,
                                typename G::view_type::iterator it)
    requires USE_FAILED_POS {
    *failed_pos = it;
  }

  template <bool USE_FAILED_POS>
  optional<typename G::operand_type>
  impl(typename G::view_type &v,
       typename G::view_type::iterator *failed_pos) {
    // note: two or more contiguous operators are acceptable
    //   but two or more contiguous operands are not acceptable
    //   e.g. seq("", "+", "+", "") is acceptable,
    //     but seq("", "", "+") is not acceptable

    const auto take_a_step = [&]()->bool {
      const auto start_pos = v.begin();
      assign_failed_pos<USE_FAILED_POS>(failed_pos, start_pos);
      using view_t = typename G::view_type;

      sscan_spaces(v);

      decltype(auto) l = g.operator_list();
      for (auto &l_it : iters(l)) {
        const auto &precedence_block = *l_it;
        for (auto &opr_info : precedence_block.vec) {
          const auto &seq = opr_info.vec;
          if (view_t(front(seq)).empty())
            continue;
          auto it = seq.begin();
          if (sscan(v, *it)) {
            for (;;) {
              ++it;
              if (it == seq.end())
                print_and_terminate
                  ("re::arithmetic_parser::impl(): "
                   "operator with no operand\n");
              if (view_t(*it).empty())
                break;
              sscan_spaces(v);
              if (!sscan(v, *it))
                return false;
            }
            s.emplace_back(in_place_index<0>,
                           precedence_block.h,
                           l_it - l.begin(),
                           at<1>(opr_info.h),
                           addressof(at<2>(opr_info.h)),
                           addressof(seq),
                           it);
            return true;
          }
        }
      }

      decltype(auto) fn_m = g.function_list();
      for (const auto &x : fn_m) {
        const auto &fn_name = x.first;
        const auto &overload_list = x.second;
        if (sscan(v, fn_name)) {
          sscan_spaces(v);
          typename G::char_type tmp_c{};
          if (sscan_single(v, tmp_c) && to_unsigned(tmp_c) == U'(') {
            s.emplace_back(in_place_index<1>,
                           addressof(overload_list),
                           0);
            return true;
          }
          else {
            v = {start_pos, v.end()};
            continue;
          }
        }
      }

      if (auto tmp = g.scan_operand(v);
          tmp.has_value())
        opd_s.push_back(move(*tmp));
      else
        return false;
    redo_label:
      assign_failed_pos<USE_FAILED_POS>(failed_pos, v.begin());
      const auto old_v_begin = v.begin();
      for (auto &l_it : iters(l)) {
        const auto &precedence_block = *l_it;
        for (auto &opr_info : precedence_block.vec) {
          const auto &seq = opr_info.vec;
          if (!view_t(front(seq)).empty())
            continue;
          auto it = next(seq.begin());
          if (it == seq.end())
            print_and_terminate
              ("re::arithmetic_parser::impl(): "
               "operator must contain at least one string label\n");

          sscan_spaces(v);
          if (sscan(v, *it)) {
            for (;;) {
              sscan_spaces(v);
              ++it;
              if (it == seq.end() || view_t(*it).empty())
                break;
              if (!sscan(v, *it))
                return false;
            }

            const int current_precedence = l_it - l.begin();
            for (;;) {
              if (s.empty())
                break;
              auto &top_var = s.back();
              if (top_var.index() == 0) {
                auto &top = at<0>(top_var);
                if (top.seq_pos != before_end(*top.seq_ptr))
                  break;
                if (top.precedence < current_precedence
                    || (top.precedence == current_precedence
                        && top.left_to_right)) {
                  auto tmp = (*top.fn_ptr)(opd_s);
                  opd_s.pop_back(top.operand_count);
                  opd_s.push_back(move(tmp));
                  s.pop_back();
                }
                else
                  break;
              }
              else
                break;
            }

            if (it != seq.end()) {
              // view_t(*it).empty()
              s.emplace_back(in_place_index<0>,
                             precedence_block.h,
                             l_it - l.begin(),
                             at<1>(opr_info.h),
                             addressof(at<2>(opr_info.h)),
                             addressof(seq),
                             it);
              return true;
            }
            else {
              opd_s.back() = at<2>(opr_info.h)(opd_s);
              goto redo_label;
            }
          }
        }
      }
      v = {old_v_begin, v.end()};

      // the last operand is just eaten,
      //   so there is no more connector operator afterward
      for (;;) {
        if (s.empty())
          return true;

        auto &top_var = s.back();
        if (top_var.index() == 0) {
          auto &top = at<0>(top_var);
          if (top.seq_pos == before_end(*top.seq_ptr)) {
            // only this way can continue the previous for (;;)
            auto tmp = (*top.fn_ptr)(opd_s);
            opd_s.pop_back(top.operand_count);
            opd_s.push_back(move(tmp));
            s.pop_back();
          }
          else {
            ++top.seq_pos;
            while (top.seq_pos != top.seq_ptr->end()
                   && !view_t(*top.seq_pos).empty()) {
              sscan_spaces(v);
              if (!sscan(v, *top.seq_pos))
                return false;
              ++top.seq_pos;
            }
            if (top.seq_pos != top.seq_ptr->end()) {
              // view_t(*top.seq_pos).empty()
              return true;
            }
            else {
              auto tmp = (*top.fn_ptr)(opd_s);
              opd_s.pop_back(top.operand_count);
              opd_s.push_back(move(tmp));
              s.pop_back();
              goto redo_label;
            }
          }
        }
        else {
          auto &top = at<1>(top_var);
          const auto &overload_l = *top.overload_list_ptr;
          ++top.arg_pos;

          if (top.arg_pos > overload_l.back().first)
            return false;

          sscan_spaces(v);
          typename G::char_type tmp_c{};
          sscan_single(v, tmp_c);
          if (to_unsigned(tmp_c) == U')') {
            const auto i = overload_l.find(top.arg_pos);
            if (i != overload_l.end()) {
              auto tmp = (i->second)(opd_s);
              opd_s.pop_back(top.arg_pos);
              opd_s.push_back(move(tmp));
              s.pop_back();
              goto redo_label;
            }
            else
              return false;
          }
          else if (to_unsigned(tmp_c) == U',')
            return true;
          else
            return false;
        }
      }
    };

    const auto v_backup = v;
    do {
      if (!take_a_step()) {
        v = v_backup;
        s.clear();
        opd_s.clear();
        return nullopt;
      }
    } while (!s.empty());

    if (opd_s.size() != 1u)
      print_and_terminate
        ("re::arithmetic_parser::impl(): the size of stack is not 1\n");
    auto ret = move(opd_s.back());
    opd_s.clear();
    return ret;
  }

public:
  arithmetic_parser() = default;
  ~arithmetic_parser() = default;
  arithmetic_parser(const arithmetic_parser &) = delete;
  arithmetic_parser &operator =(const arithmetic_parser &) = delete;
  arithmetic_parser(arithmetic_parser &&) = default;
  arithmetic_parser &operator =(arithmetic_parser &&) = default;
  friend void swap(arithmetic_parser &x1, arithmetic_parser &x2)
    noexcept(is_nothrow_swappable<G>) {
    adl_swap(x1.g, x2.g);
  }

  explicit arithmetic_parser(const G &gg)
    requires is_copy_constructible<G>
    : g(gg) {}
  explicit arithmetic_parser(G &&gg)
    requires is_move_constructible<G>
    : g(move(gg)) {}

  optional<typename G::operand_type>
  operator ()(typename G::view_type &v,
              typename G::view_type::iterator &failed_pos) {
    auto tmp = failed_pos;
    auto ret = impl<true>(v, addressof(tmp));
    if (ret.empty()) {
      failed_pos = tmp;
      return nullopt;
    }
    else
      return ret;
  }
  optional<typename G::operand_type>
  operator ()(typename G::view_type &v) {
    return impl<false>(v, nullptr);
  }

  void clear_buffer() {
    s.shrink_to_fit();
    if constexpr (inner::can_shrink_to_fit<typename G::stack_type>) {
      opd_s.shrink_to_fit();
    }
  }
};

using default_arithmetic_parser = arithmetic_parser<default_arithmetic_grammar>;
using default_arithmetic_parser_u32
  = arithmetic_parser<default_arithmetic_grammar_u32>;

struct fo_sscan_arithmetic {
  optional<double> operator ()(sview &v) const {
    default_arithmetic_parser parser;
    return parser(v);
  }
  optional<double> operator ()(u32sview &v) const {
    default_arithmetic_parser_u32 parser;
    return parser(v);
  }
};
inline constexpr fo_sscan_arithmetic sscan_arithmetic{};

}

// file
namespace re {

struct read_file_t {
  explicit read_file_t() = default;
};
inline constexpr read_file_t read_file{};
struct open_file_t {
  explicit open_file_t() = default;
};
inline constexpr open_file_t open_file{};
struct create_file_t {
  explicit create_file_t() = default;
};
inline constexpr create_file_t create_file{};
class file {
  HANDLE p;

public:
  file() noexcept : p(INVALID_HANDLE_VALUE) {}
  ~file() {
    close();
  }
  file(const file &) = delete;
  file &operator =(const file &) = delete;
  file(file &&f) noexcept : p(f.p) {
    f.p = INVALID_HANDLE_VALUE;
  }
  file &operator =(file &&f) noexcept {
    if (this != addressof(f)) {
      close();
      p = f.p;
      f.p = INVALID_HANDLE_VALUE;
    }
    return *this;
  }

  explicit file(sview v) : p(INVALID_HANDLE_VALUE) {
    file_open_impl(v, p);
  }
  explicit file(wsview v) : p(INVALID_HANDLE_VALUE) {
    file_open_impl(v, p);
  }
  explicit file(u16sview v) : p(INVALID_HANDLE_VALUE) {
    file_open_impl(v, p);
  }
  explicit file(u32sview v) : p(INVALID_HANDLE_VALUE) {
    file_open_impl(v, p);
  }

  file(sview v, read_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_read(v, p);
  }
  file(wsview v, read_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_read(v, p);
  }
  file(u16sview v, read_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_read(v, p);
  }
  file(u32sview v, read_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_read(v, p);
  }

  file(sview v, open_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_open(v, p);
  }
  file(wsview v, open_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_open(v, p);
  }
  file(u16sview v, open_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_open(v, p);
  }
  file(u32sview v, open_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_open(v, p);
  }

  file(sview v, create_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_create(v, p);
  }
  file(wsview v, create_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_create(v, p);
  }
  file(u16sview v, create_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_create(v, p);
  }
  file(u32sview v, create_file_t) : p(INVALID_HANDLE_VALUE) {
    file_open_impl_create(v, p);
  }

  bool empty() const noexcept {
    return p == INVALID_HANDLE_VALUE;
  }

private:
  template <class V>
  static void file_open_impl(V v, HANDLE &p) {
    wstring s;
    if (!s.try_assign_unicode(v))
      throw_or_terminate<logic_error>
        ("re::file::open(sv): invalid unicode string\n");
    p = CreateFile(s.data(),
                   (GENERIC_READ | GENERIC_WRITE), 0, NULL,
                   OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      p = CreateFile(s.data(),
                     GENERIC_READ, 0, NULL,
                     OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>
        ("re::file::file_open_impl(): failed to open\n");
  }
  template <class V>
  static void file_open_impl_read(V v, HANDLE &p) {
    wstring s;
    if (!s.try_assign_unicode(v))
      throw_or_terminate<logic_error>
        ("re::file::open(sv, read_file): invalid unicode string\n");
    p = CreateFile(s.data(),
                   GENERIC_READ, 0, NULL,
                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>
        ("re::file::file_open_impl_read(): failed to open\n");
  }
  template <class V>
  static void file_open_impl_open(V v, HANDLE &p) {
    wstring s;
    if (!s.try_assign_unicode(v))
      throw_or_terminate<logic_error>
        ("re::file::open(sv, open_file): invalid unicode string\n");
    p = CreateFile(s.data(),
                   (GENERIC_READ | GENERIC_WRITE), 0, NULL,
                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      p = CreateFile(s.data(),
                     GENERIC_READ, 0, NULL,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>
        ("re::file::file_open_impl_open(): failed to open\n");
  }
  template <class V>
  static void file_open_impl_create(V v, HANDLE &p) {
    wstring s;
    if (!s.try_assign_unicode(v))
      throw_or_terminate<logic_error>
        ("re::file::open(sv, create_file): invalid unicode string\n");
    p = CreateFile(s.data(),
                   (GENERIC_READ | GENERIC_WRITE), 0, NULL,
                   CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>
        ("re::file::file_open_impl_create(): failed to open\n");
  }
public:
  void open(sview v) {
    close();
    file_open_impl(v, p);
  }
  void open(wsview v) {
    close();
    file_open_impl(v, p);
  }
  void open(u16sview v) {
    close();
    file_open_impl(v, p);
  }
  void open(u32sview v) {
    close();
    file_open_impl(v, p);
  }
  void open(sview v, read_file_t) {
    close();
    file_open_impl_read(v, p);
  }
  void open(wsview v, read_file_t) {
    close();
    file_open_impl_read(v, p);
  }
  void open(u16sview v, read_file_t) {
    close();
    file_open_impl_read(v, p);
  }
  void open(u32sview v, read_file_t) {
    close();
    file_open_impl_read(v, p);
  }
  void open(sview v, open_file_t) {
    close();
    file_open_impl_open(v, p);
  }
  void open(wsview v, open_file_t) {
    close();
    file_open_impl_open(v, p);
  }
  void open(u16sview v, open_file_t) {
    close();
    file_open_impl_open(v, p);
  }
  void open(u32sview v, open_file_t) {
    close();
    file_open_impl_open(v, p);
  }
  void open(sview v, create_file_t) {
    close();
    file_open_impl_create(v, p);
  }
  void open(wsview v, create_file_t) {
    close();
    file_open_impl_create(v, p);
  }
  void open(u16sview v, create_file_t) {
    close();
    file_open_impl_create(v, p);
  }
  void open(u32sview v, create_file_t) {
    close();
    file_open_impl_create(v, p);
  }

  void close() noexcept {
    if (p != INVALID_HANDLE_VALUE) {
      CloseHandle(p);
      p = INVALID_HANDLE_VALUE;
    }
  }

  template <class S = string>
  S read() const
    requires (is_crng<S> && (is_same<rng_vt<S>, char>
                             || is_same<rng_vt<S>, signed char>
                             || is_same<rng_vt<S>, unsigned char>)) {
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>("re::file::read(): null file\n");
    S s;
    LARGE_INTEGER sz;
    if (GetFileSizeEx(p, addressof(sz)) == 0)
      throw_or_terminate<logic_error>
        ("re::file::read(): failed to get file size\n");
    if (to_unsigned(sz.QuadPart) > s.max_size()
        || to_unsigned(sz.QuadPart) > numeric_limits<DWORD>::max())
      throw_or_terminate<logic_error>("re::file::read(): too big file size\n");
    s.resize(static_cast<rng_szt<S>>(sz.QuadPart));
    DWORD holder{};
    if (ReadFile(p,
                 reinterpret_cast<void *>(to_address(s.begin())),
                 s.size(),
                 addressof(holder),
                 NULL)
        == 0)
      throw_or_terminate<logic_error>("re::file::read(): failed to read\n");
    if (SetFilePointer(p, 0, NULL, FILE_BEGIN)
        == INVALID_SET_FILE_POINTER)
      throw_or_terminate<logic_error>("re::file::read(): failed to reset\n");
    return s;
  }
private:
  template <class R>
  static void file_write_impl(R &&r, HANDLE p) {
    DWORD holder{};
    auto n = size(r);
    if (n > numeric_limits<DWORD>::max())
      throw_or_terminate<length_error>
        ("re::file::write(r): too big size to write\n");
    if (WriteFile(p,
                  reinterpret_cast<const void *>(to_address(begin(r))),
                  static_cast<DWORD>(n),
                  addressof(holder),
                  NULL)
        == 0)
      throw_or_terminate<logic_error>("re::file::write(r): failed to write\n");
    if (SetEndOfFile(p) == 0)
      throw_or_terminate<logic_error>("re::file::write(r): failed to resize\n");
    if (SetFilePointer(p, 0, NULL, FILE_BEGIN)
        == INVALID_SET_FILE_POINTER)
      throw_or_terminate<logic_error>("re::file::write(r): failed to reset\n");
  }
public:
  void write(sview sv) const {
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>("re::file::write(r): null file\n");
    file_write_impl(sv, p);
  }
  template <class R>
  void write(R &&r) const
    requires (is_crng<R> && (is_same<rng_vt<R>, char>
                             || is_same<rng_vt<R>, signed char>
                             || is_same<rng_vt<R>, unsigned char>)
              && !is_convertible<R, sview>) {
    if (p == INVALID_HANDLE_VALUE)
      throw_or_terminate<logic_error>("re::file::write(r): null file\n");
    file_write_impl(r, p);
  }
};

namespace inner::fns {

template <class V, class S>
void to_full_path_impl(V v, S &o2) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::to_full_path(sv): invalid unicode input\n");

  wstring o(128u);
  for (;;) {
    if (o.size() > numeric_limits<DWORD>::max())
      throw_or_terminate<length_error>
        ("re::to_full_path(sv): too long input string\n");
    const DWORD x = GetFullPathName(s.data(),
                                    static_cast<DWORD>(o.size()), o.data(),
                                    NULL);
    if (x == 0)
      throw_or_terminate<logic_error>
        ("re::to_full_path(sv): conversion failed\n");
    if (x != o.size()) {
      o.resize(x);
      break;
    }
    o.append_range(rng(o.size(), L'\0'));
  }

  if (!o2.try_assign_unicode(o))
    throw_or_terminate<logic_error>
      ("re::to_full_path(sv): invalid unicode result\n");
}

}
struct fo_to_full_path {
  string operator ()(sview v) const {
    string s;
    inner::fns::to_full_path_impl(v, s);
    return s;
  }
  wstring operator ()(wsview v) const {
    wstring s;
    inner::fns::to_full_path_impl(v, s);
    return s;
  }
  u16string operator ()(u16sview v) const {
    u16string s;
    inner::fns::to_full_path_impl(v, s);
    return s;
  }
  u32string operator ()(u32sview v) const {
    u32string s;
    inner::fns::to_full_path_impl(v, s);
    return s;
  }
};
inline constexpr fo_to_full_path to_full_path{};

namespace inner::fns {

template <class S, class V>
S simplify_path_impl(V v) {
  S s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::simplify_path(sv): invalid unicode input\n");

  const auto is_slash = [](auto c) {
    return to_unsigned(c) == U'/' || to_unsigned(c) == U'\\';
  };
  const auto is_slash_pair = [is_slash](auto c, auto c2) {
    return is_slash(c) && is_slash(c2);
  };
  s.erase(unique(s, is_slash_pair), s.end());
  while (is_slash(back(s)) && size(s) != 1u)
    s.pop_back();
  replace_if(s, is_slash, static_cast<rng_vt<S>>(U'\\'));

  return s;
}

}
struct fo_simplify_path {
  string operator ()(sview v) const {
    return inner::fns::simplify_path_impl<string>(v);
  }
  wstring operator ()(wsview v) const {
    return inner::fns::simplify_path_impl<wstring>(v);
  }
  u16string operator ()(u16sview v) const {
    return inner::fns::simplify_path_impl<u16string>(v);
  }
  u32string operator ()(u32sview v) const {
    return inner::fns::simplify_path_impl<u32string>(v);
  }
};
inline constexpr fo_simplify_path simplify_path{};

namespace inner::fns {

template <class S, class V>
S path_last_name_impl(V v) {
  S s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::path_last_name(sv): invalid unicode input\n");

  const auto is_slash = [](auto c) {
    return to_unsigned(c) == U'/' || to_unsigned(c) == U'\\';
  };
  const auto deref_to_slash = [](const auto p) {
    return to_unsigned(*p) == U'\\' || to_unsigned(*p) == U'/';
  };
  auto r = rng(s);
  if (is_slash(back(r))) {
    const auto r2 = drop_while_rng(rrng(irng(r)), deref_to_slash);
    if (empty(r2))
      throw_or_terminate<logic_error>("re::path_last_name(sv): no last name\n");
    r.second = next(front(r2));
  }
  if (const auto it = find_last_if(r, is_slash);
      it != r.end())
    s.erase(s.begin(), next(it));
  while(is_slash(back(s)))
    s.pop_back();
  return s;
}

}
struct fo_path_last_name {
  string operator ()(sview v) const {
    return inner::fns::path_last_name_impl<string>(v);
  }
  wstring operator ()(wsview v) const {
    return inner::fns::path_last_name_impl<wstring>(v);
  }
  u16string operator ()(u16sview v) const {
    return inner::fns::path_last_name_impl<u16string>(v);
  }
  u32string operator ()(u32sview v) const {
    return inner::fns::path_last_name_impl<u32string>(v);
  }
};
inline constexpr fo_path_last_name path_last_name{};

namespace inner::fns {

template <class S, class V>
S remove_path_last_name_impl(V v) {
  S s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::remove_path_last_name(sv): invalid unicode input\n");
  if (s.empty())
    return s;

  const auto is_slash = [](auto c) {
    return to_unsigned(c) == U'\\' || to_unsigned(c) == U'/';
  };
  const auto deref_to_slash = [](const auto p) {
    return to_unsigned(*p) == U'\\' || to_unsigned(*p) == U'/';
  };
  auto r = rng(s);
  if (is_slash(back(r))) {
    const auto r2 = drop_while_rng(rrng(irng(r)), deref_to_slash);
    if (empty(r2))
      return s;
    r.second = next(front(r2));
  }
  if (const auto it = find_last_if(r, is_slash);
      it != r.end())
    s.erase(next(it), s.end());
  return s;
}

}
struct fo_remove_path_last_name {
  string operator ()(sview v) const {
    return inner::fns::remove_path_last_name_impl<string>(v);
  }
  wstring operator ()(wsview v) const {
    return inner::fns::remove_path_last_name_impl<wstring>(v);
  }
  u16string operator ()(u16sview v) const {
    return inner::fns::remove_path_last_name_impl<u16string>(v);
  }
  u32string operator ()(u32sview v) const {
    return inner::fns::remove_path_last_name_impl<u32string>(v);
  }
};
inline constexpr fo_remove_path_last_name remove_path_last_name{};

namespace inner::fns {

template <class S, class V>
S replace_path_last_name_impl(V v, V v2) {
  S s, s2;
  if (!s.try_assign_unicode(v) || !s2.try_assign_unicode(v2))
    throw_or_terminate<logic_error>
      ("re::replace_path_last_name(v, v2): invalid unicode string\n");
  S ss = remove_path_last_name(s);
  if (ss == s)
    throw_or_terminate<logic_error>
      ("re::replace_path_last_name(v, v2): v has no last name\n");
  ss.append(s2);
  return ss;
}

}
struct fo_replace_path_last_name {
  string operator ()(sview v, sview v2) const {
    return inner::fns::replace_path_last_name_impl<string>(v, v2);
  }
  wstring operator ()(wsview v, wsview v2) const {
    return inner::fns::replace_path_last_name_impl<wstring>(v, v2);
  }
  u16string operator ()(u16sview v, u16sview v2) const {
    return inner::fns::replace_path_last_name_impl<u16string>(v, v2);
  }
  u32string operator ()(u32sview v, u32sview v2) const {
    return inner::fns::replace_path_last_name_impl<u32string>(v, v2);
  }
};
inline constexpr fo_replace_path_last_name replace_path_last_name{};


namespace inner::fns {

template <class V>
bool is_file_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::is_file(sv): invalid unicode string\n");
  const DWORD a = GetFileAttributes(s.data());
  return a != INVALID_FILE_ATTRIBUTES;
}

}
struct fo_is_file {
  bool operator ()(sview v) const {
    return inner::fns::is_file_impl(v);
  }
  bool operator ()(wsview v) const {
    return inner::fns::is_file_impl(v);
  }
  bool operator ()(u16sview v) const {
    return inner::fns::is_file_impl(v);
  }
  bool operator ()(u32sview v) const {
    return inner::fns::is_file_impl(v);
  }
};
inline constexpr fo_is_file is_file{};

namespace inner::fns {

template <class V>
bool is_directory_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::is_directory(sv): invalid unicode string\n");
  DWORD a = GetFileAttributes(s.data());
  if (a == INVALID_FILE_ATTRIBUTES)
    return false;
  return a & FILE_ATTRIBUTE_DIRECTORY;
}

}
struct fo_is_directory {
  bool operator ()(sview v) const {
    return inner::fns::is_directory_impl(v);
  }
  bool operator ()(wsview v) const {
    return inner::fns::is_directory_impl(v);
  }
  bool operator ()(u16sview v) const {
    return inner::fns::is_directory_impl(v);
  }
  bool operator ()(u32sview v) const {
    return inner::fns::is_directory_impl(v);
  }
};
inline constexpr fo_is_directory is_directory{};

namespace inner::fns {

template <class V>
bool try_create_directory_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::is_directory(sv): invalid unicode string\n");
  return CreateDirectory(s.data(), NULL) != 0;
}
template <class V>
void create_directory_impl(V v) {
  if (!inner::fns::try_create_directory_impl(v))
    throw_or_terminate<logic_error>("re::creaate_directory(sv): failed\n");
}

}
struct fo_try_create_directory {
  bool operator ()(sview v) const {
    return inner::fns::try_create_directory_impl(v);
  }
  bool operator ()(wsview v) const {
    return inner::fns::try_create_directory_impl(v);
  }
  bool operator ()(u16sview v) const {
    return inner::fns::try_create_directory_impl(v);
  }
  bool operator ()(u32sview v) const {
    return inner::fns::try_create_directory_impl(v);
  }
};
inline constexpr fo_try_create_directory try_create_directory{};
struct fo_create_directory {
  void operator ()(sview v) const {
    inner::fns::create_directory_impl(v);
  }
  void operator ()(wsview v) const {
    inner::fns::create_directory_impl(v);
  }
  void operator ()(u16sview v) const {
    inner::fns::create_directory_impl(v);
  }
  void operator ()(u32sview v) const {
    inner::fns::create_directory_impl(v);
  }
};
inline constexpr fo_create_directory create_directory{};

namespace inner::fns {

template <class V>
unsigned long long file_size_impl(V v);

}
struct fo_file_size {
  unsigned long long operator ()(sview v) const {
    return inner::fns::file_size_impl(v);
  }
  unsigned long long operator ()(wsview v) const {
    return inner::fns::file_size_impl(v);
  }
  unsigned long long operator ()(u16sview v) const {
    return inner::fns::file_size_impl(v);
  }
  unsigned long long operator ()(u32sview v) const {
    return inner::fns::file_size_impl(v);
  }
};
inline constexpr fo_file_size file_size{};

namespace inner::fns {

template <class V>
time_point<system_clock> file_time_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::file_time(sv): invalid unicode string\n");
  WIN32_FILE_ATTRIBUTE_DATA a;
  if (!GetFileAttributesEx(s.data(), GetFileExInfoStandard, addressof(a)))
    throw_or_terminate<logic_error>("re::file_time(sv): failed\n");
  return time_point<system_clock>(system_clock::duration
                                  (bit_cast<long long>(a.ftLastWriteTime)));
}

}
struct fo_file_time {
  time_point<system_clock> operator ()(sview v) const {
    return inner::fns::file_time_impl(v);
  }
  time_point<system_clock> operator ()(wsview v) const {
    return inner::fns::file_time_impl(v);
  }
  time_point<system_clock> operator ()(u16sview v) const {
    return inner::fns::file_time_impl(v);
  }
  time_point<system_clock> operator ()(u32sview v) const {
    return inner::fns::file_time_impl(v);
  }
};
inline constexpr fo_file_time file_time{};

namespace inner::fns {

template <class V>
bool try_remove_file_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::try_remove_file(sv): invalid unicode string\n");

  const DWORD a = GetFileAttributes(s.data());
  if (a != INVALID_FILE_ATTRIBUTES) {
    if (a & FILE_ATTRIBUTE_ARCHIVE) {
      if (DeleteFile(s.data()))
        return true;
    }
    else if (a & FILE_ATTRIBUTE_DIRECTORY) {
      s = to_full_path(s);
      s = simplify_path(s);
      s.push_back(L'\0');
      SHFILEOPSTRUCT file_op = {
        NULL,
        FO_DELETE,
        s.data(),
        NULL,
        FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT,
        false,
        0,
        L""
      };
      if (SHFileOperation(addressof(file_op)) == 0)
        return true;
    }
  }
  return false;
}
template <class V>
void remove_file_impl(V v) {
  if (!inner::fns::try_remove_file_impl(v))
   throw_or_terminate<logic_error>("re::remove_file(sv): failed\n");
}

}
struct fo_try_remove_file {
  bool operator ()(sview v) const {
    return inner::fns::try_remove_file_impl(v);
  }
  bool operator ()(wsview v) const {
    return inner::fns::try_remove_file_impl(v);
  }
  bool operator ()(u16sview v) const {
    return inner::fns::try_remove_file_impl(v);
  }
  bool operator ()(u32sview v) const {
    return inner::fns::try_remove_file_impl(v);
  }
};
inline constexpr fo_try_remove_file try_remove_file{};
struct fo_remove_file {
  void operator ()(sview v) const {
    inner::fns::remove_file_impl(v);
  }
  void operator ()(wsview v) const {
    inner::fns::remove_file_impl(v);
  }
  void operator ()(u16sview v) const {
    inner::fns::remove_file_impl(v);
  }
  void operator ()(u32sview v) const {
    inner::fns::remove_file_impl(v);
  }
};
inline constexpr fo_remove_file remove_file{};

namespace inner::fns {

template <class V>
bool try_rename_file_impl(V, V);
template <class V>
void rename_file_impl(V v, V v2) {
  if (!inner::fns::try_rename_file_impl(v, v2))
    throw_or_terminate<logic_error>("re::rename_file(v, v2): failed\n");
}

}
struct fo_try_rename_file {
  bool operator ()(sview v, sview v2) const {
    return inner::fns::try_rename_file_impl(v, v2);
  }
  bool operator ()(wsview v, wsview v2) const {
    return inner::fns::try_rename_file_impl(v, v2);
  }
  bool operator ()(u16sview v, u16sview v2) const {
    return inner::fns::try_rename_file_impl(v, v2);
  }
  bool operator ()(u32sview v, u32sview v2) const {
    return inner::fns::try_rename_file_impl(v, v2);
  }
};
inline constexpr fo_try_rename_file try_rename_file{};
struct fo_rename_file {
  void operator ()(sview v, sview v2) const {
    inner::fns::rename_file_impl(v, v2);
  }
  void operator ()(wsview v, wsview v2) const {
    inner::fns::rename_file_impl(v, v2);
  }
  void operator ()(u16sview v, u16sview v2) const {
    inner::fns::rename_file_impl(v, v2);
  }
  void operator ()(u32sview v, u32sview v2) const {
    inner::fns::rename_file_impl(v, v2);
  }
};
inline constexpr fo_rename_file rename_file{};

// implement inner::fns::try_rename_file_impl
namespace inner::fns {

template <class V>
bool try_rename_file_impl(V v, V v2) {
  wstring s, s2;
  if (!s.try_assign_unicode(v) || !s2.try_assign_unicode(v2))
    throw_or_terminate<logic_error>
      ("re::try_rename_file(v, v2): invalid unicode string\n");
  if (contains(s2, L'\\') || contains(s2, L'/'))
    throw_or_terminate<logic_error>
      ("re::try_rename_file(v, v2): invalid new name\n");

  wstring s3 = remove_path_last_name(s);
  if (s3 == s)
    throw_or_terminate<logic_error>
      ("re::try_rename_file(v, v2): v has no last name to rename\n");
  s3.append(s2);
  
  return MoveFile(s.data(), s3.data()) != 0;
}

}


template <class S = string>
struct file_info {
  using string_type = S;

  S path;
  S name;
  unsigned long long size;
  time_point<system_clock> time;
  bool is_dir;
};

namespace inner::fns {

template <class S, class V>
file_info<S> view_file_impl0(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::view_file(sv): invalid unicode string as input\n");
  s = simplify_path(to_full_path(s));

  S s2;
  if (!s2.try_assign_unicode(s))
    throw_or_terminate<logic_error>
      ("re::view_file(sv): invalid unicode string as input\n");

  file_info<S> ret;

  ret.path = s2;

  ret.name = path_last_name(s2);

  WIN32_FILE_ATTRIBUTE_DATA a;
  if (!GetFileAttributesEx(s.data(), GetFileExInfoStandard, addressof(a)))
    throw_or_terminate<logic_error>("re::view_file(sv): failed\n");

  ret.time = time_point<system_clock>
    (system_clock::duration(bit_cast<long long>(a.ftLastWriteTime)));

  ret.is_dir = (a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

  if (ret.is_dir)
    ret.size = 0u;
  else {
    LARGE_INTEGER tmp_sz;
    tmp_sz.HighPart = a.nFileSizeHigh;
    tmp_sz.LowPart = a.nFileSizeLow;
    ret.size = static_cast<unsigned long long>(tmp_sz.QuadPart);
  }

  return ret;
}
template <class S, class V>
file_info<S> view_file_impl(V v) {
  auto ret = inner::fns::view_file_impl0<S>(v);
  if (ret.is_dir)
    ret.size = file_size(v);
  return ret;
}

}
struct fo_view_file {
  file_info<string> operator ()(sview v) const {
    return inner::fns::view_file_impl<string>(v);
  }
  file_info<wstring> operator ()(wsview v) const {
    return inner::fns::view_file_impl<wstring>(v);
  }
  file_info<u16string> operator ()(u16sview v) const {
    return inner::fns::view_file_impl<u16string>(v);
  }
  file_info<u32string> operator ()(u32sview v) const {
    return inner::fns::view_file_impl<u32string>(v);
  }
};
inline constexpr fo_view_file view_file{};

namespace inner::fns {

template <class S, class V>
tree<file_info<S>> view_directory_impl(V);
template <class V>
unsigned long long file_size_impl(V);

}
struct fo_view_directory {
  tree<file_info<string>> operator ()(sview v) const {
    return inner::fns::view_directory_impl<string>(v);
  }
  tree<file_info<wstring>> operator ()(wsview v) const {
    return inner::fns::view_directory_impl<wstring>(v);
  }
  tree<file_info<u16string>> operator ()(u16sview v) const {
    return inner::fns::view_directory_impl<u16string>(v);
  }
  tree<file_info<u32string>> operator ()(u32sview v) const {
    return inner::fns::view_directory_impl<u32string>(v);
  }
};
inline constexpr fo_view_directory view_directory{};
namespace inner::fns {

template <class S, class V>
tree<file_info<S>> view_directory_impl(V v) {
  using key_t = file_info<S>;
  using tree_t = tree<file_info<S>>;
  using tree_vt = typename tree_t::value_type;

  tree_t t;
  t.emplace(inner::fns::view_file_impl0<S>(v));
  if ((**t.root()).is_dir == false)
    throw_or_terminate<logic_error>
      ("re::view_directory(sv): non-directory is unacceptable\n");

  const auto load_direct_children = [](const wchar_t *p, auto f) {
    WIN32_FIND_DATA find_data;
    HANDLE h = FindFirstFile(p, addressof(find_data));
    if (h != INVALID_HANDLE_VALUE) {
      do {
        f(find_data);
      } while (FindNextFile(h, addressof(find_data)));
      FindClose(h);
    }
  };
  const auto read_find_data = [](const WIN32_FIND_DATA &x, key_t &o) {
    if (o.name.try_assign_unicode(wsview(begin(x.cFileName))) == false)
      throw_or_terminate<logic_error>
        ("re::view_directory(sv): invalid unicode string from find data\n");

    o.time = time_point<system_clock>
      (system_clock::duration(bit_cast<long long>(x.ftLastWriteTime)));

    o.is_dir = (x.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

    if (o.is_dir)
      o.size = 0u;
    else {
      LARGE_INTEGER tmp_sz;
      tmp_sz.HighPart = x.nFileSizeHigh;
      tmp_sz.LowPart = x.nFileSizeLow;
      o.size = static_cast<unsigned long long>(tmp_sz.QuadPart);
    }
  };
  wstring tmp_ws;
  for (tree_vt &x : t.root().pre_order()) {
    if ((*x).is_dir) {
      const auto it = x.iter();
      if (tmp_ws.try_assign_unicode((*x).path) == false)
        throw_or_terminate<logic_error>
          ("re::view_directory(sv): invalid unicode string\n");
      tmp_ws.append(L'\\');
      tmp_ws.append(L'*');
      const auto load_find_data
        = [&t, it, read_find_data, &x](const WIN32_FIND_DATA &d) {
          key_t info;
          read_find_data(d, info);
          if (!((info.name.size() == 1u
                 && to_unsigned(info.name.front()) == U'.')
                || (info.name.size() == 2u
                    && to_unsigned(info.name.front()) == U'.'
                    && to_unsigned(info.name.back()) == U'.'))) {
            info.path = (*x).path;
            info.path.push_back(static_cast<rng_vt<S>>(U'\\'));
            info.path.append_range(info.name);
            t.push_back(it, move(info));
          }
        };
      load_direct_children(tmp_ws.data(), load_find_data);
    }
  }

  for (tree_vt &x : t.root().post_order()) {
    if ((*x).is_dir)
      for (tree_vt &u : x)
        (*x).size += (*u).size;
  }

  return t;
}

template <class V>
unsigned long long file_size_impl(V v) {
  wstring s;
  if (!s.try_assign_unicode(v))
    throw_or_terminate<logic_error>
      ("re::file_size(sv): invalid unicode string\n");
  WIN32_FILE_ATTRIBUTE_DATA a;
  if (!GetFileAttributesEx(s.data(), GetFileExInfoStandard, addressof(a)))
    throw_or_terminate<logic_error>("re::file_size(sv): failed\n");

  if (a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    const auto t = view_directory(v);
    return (**t.root()).size;
  }
  else {
    LARGE_INTEGER size;
    size.HighPart = a.nFileSizeHigh;
    size.LowPart = a.nFileSizeLow;
    return static_cast<unsigned long long>(size.QuadPart);
  }
}

}

}

#endif
