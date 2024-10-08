#ifndef RE_DEFINED_BASE_H
#define RE_DEFINED_BASE_H

// optional macro:
//   RE_NOEXCEPT
//   RE_DEFAULT_ALLOCATOR // e.g. re::stateless_test_allocator

// macro for inner usage:
//   RE_DEFAULT_NEW_ALIGNMENT
//   RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS
//   RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS

#ifdef __MINGW32__
#ifdef WINVER
#undef WINVER
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <cstdlib>
namespace re::inner {
inline const int mingw_no_assertion_failure_popup
  = []() {
    _set_error_mode(_OUT_TO_STDERR);
    return 0;
  }();
}
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifdef _MSC_VER
#ifdef RE_WIN32_WINDOW_PROGRAM
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif
#include <windows.h>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <shlwapi.h>
#include <intrin.h>
#include <immintrin.h>

// dependence of windows api
//   base.h
//   test.h
//     re::ez_mutex
//     re::test_allocator // use re::ez_mutex
//   time.h
//     re::inner::fns::win32_get_system_time_precise_as_file_time
//     re::inner::fns::win32_query_performance_counter
//     re::inner::fns::win32_query_performance_frequency
//     re::inner::fns::win32_sleep_ms
//     re::system_clock
//     re::steady_clock
//   io.h
//     re::file
//   concurrency.h
//     ...
//   graphics.h
//     re::inner::fns::win32_enable_wstring_console_for_command_line_program
//     re::inner::fns::win32_enable_utf8_console_for_window_program

#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <cfloat>
#include <cwchar>
#include <cstdint>
#include <cctype>
#include <cstring>
#include <ctime>
#include <cmath>

#include <cwctype>
#include <cwchar>

#include <type_traits>
#include <typeinfo>
#include <initializer_list>
#include <exception>
#include <stdexcept>
#include <new>
#include <compare>
#include <source_location>
#include <tuple>
#include <bit>

#ifndef __MINGW32__
#define RE_DEFAULT_NEW_ALIGNMENT __STDCPP_DEFAULT_NEW_ALIGNMENT__
#else
#define RE_DEFAULT_NEW_ALIGNMENT alignof(max_align_t)
#endif

// std dependence
namespace re {

using size_t = std::size_t;
using ptrdiff_t = std::ptrdiff_t;
using nullptr_t = std::nullptr_t;
using max_align_t = std::max_align_t;

using byte = std::byte;

using int_least8_t = std::int_least8_t;
using uint_least8_t = std::uint_least8_t;
using int_least16_t = std::int_least16_t;
using uint_least16_t = std::uint_least16_t;
using int_least32_t = std::int_least32_t;
using uint_least32_t = std::uint_least32_t;
using int_least64_t = std::int_least64_t;
using uint_least64_t = std::uint_least64_t;

using int_fast8_t = std::int_fast8_t;
using uint_fast8_t = std::uint_fast8_t;
using int_fast16_t = std::int_fast16_t;
using uint_fast16_t = std::uint_fast16_t;
using int_fast32_t = std::int_fast32_t;
using uint_fast32_t = std::uint_fast32_t;
using int_fast64_t = std::int_fast64_t;
using uint_fast64_t = std::uint_fast64_t;

using intmax_t = std::intmax_t;
using uintmax_t = std::uintmax_t;

#ifdef INTPTR_MAX
using intptr_t = std::intptr_t;
using uintptr_t = std::uintptr_t;
#endif
#ifdef INT8_MAX
using int8_t = std::int8_t;
using uint8_t = std::uint8_t;
#endif
#ifdef INT16_MAX
using int16_t = std::int16_t;
using uint16_t = std::uint16_t;
#endif
#ifdef INT32_MAX
using int32_t = std::int32_t;
using uint32_t = std::uint32_t;
#endif
#ifdef INT64_MAX
using int64_t = std::int64_t;
using uint64_t = std::uint64_t;
#endif

using std::initializer_list;

using type_info = std::type_info;

using exception = std::exception;
using bad_exception = std::bad_exception;

using terminate_handler = std::terminate_handler;
using std::get_terminate;
using std::set_terminate;
using std::terminate;
using std::uncaught_exceptions;

using std::current_exception;
using std::rethrow_exception;
using exception_ptr = std::exception_ptr;
struct fo_make_exception_ptr {
  template <class E>
  exception_ptr operator ()(E e) const noexcept {
    return std::make_exception_ptr(static_cast<E &&>(e));
  }
};
inline constexpr fo_make_exception_ptr make_exception_ptr{};

using std::nested_exception;
struct fo_throw_with_nested {
  template <class T>
  [[noreturn]] void operator ()(T &&t) const {
    std::throw_with_nested(static_cast<T &&>(t));
  }
};
inline constexpr fo_throw_with_nested throw_with_nested{};
struct fo_rethrow_if_nested {
  template <class E>
  void operator ()(const E &e) const {
    std::rethrow_if_nested(e);
  }
};
inline constexpr fo_rethrow_if_nested rethrow_if_nested{};

using bad_alloc = std::bad_alloc;
using bad_array_new_length = std::bad_array_new_length;
using bad_typeid = std::bad_typeid;
using bad_cast = std::bad_cast;

using logic_error = std::logic_error;
using domain_error = std::domain_error;
using invalid_argument = std::invalid_argument;
using length_error = std::length_error;
using out_of_range = std::out_of_range;

using runtime_error = std::runtime_error;
using range_error = std::range_error;
using overflow_error = std::overflow_error;
using underflow_error = std::underflow_error;

using std::malloc;
using std::free;
using std::system;
using std::getenv;
using std::abort;
using std::exit;

using std::nothrow_t;
using std::nothrow;
using std::align_val_t;
using std::destroying_delete_t;
// using std::hardware_destructive_interference_size;
// using std::hardware_constructive_interference_size;

using new_handler = void (*)();
using std::get_new_handler;
using std::set_new_handler;

using FILE = std::FILE;
using fpos_t = std::fpos_t;
using std::fopen;
using std::freopen;
using std::fclose;
using std::rename;
using std::fflush;
using std::setbuf;
using std::setvbuf;
using std::fread;
using std::fwrite;
using std::fgetc;
using std::getc;
using std::fputc;
using std::putc;
using std::fputs;
using std::getchar;
using std::putchar;
using std::puts;
using std::ungetc;
using std::scanf;
using std::fscanf;
using std::sscanf;
using std::printf;
using std::fprintf;
using std::sprintf;
using std::snprintf;
using std::ftell;
using std::fgetpos;
using std::fseek;
using std::fsetpos;
using std::rewind;
using std::clearerr;
using std::feof;
using std::ferror;
using std::perror;
using std::rename;
using std::tmpfile;
using std::tmpnam;

using std::isalnum;
using std::isalpha;
using std::islower;
using std::isupper;
using std::isdigit;
using std::isxdigit;
using std::iscntrl;
using std::isgraph;
using std::isspace;
using std::isblank;
using std::isprint;
using std::ispunct;
using std::tolower;
using std::toupper;

using std::strcpy;
using std::strncpy;
using std::strcat;
using std::strncat;
using std::strxfrm;
using std::strlen;
using std::strcmp;
using std::strncmp;
using std::strcoll;
using std::strrchr;
using std::strspn;
using std::strcspn;
using std::strpbrk;
using std::strstr;
using std::strtok;

using std::memchr;
using std::memcmp;
using std::memset;
using std::memcpy;
using std::memmove;

using std::is_constant_evaluated;

using std::source_location;

using std::fgetwc;
using std::fgetws;
using std::fputwc;
using std::fputws;
using std::getwchar;
using std::putwchar;
using std::ungetwc;
using wctrans_t = std::wctrans_t;
using wctype_t = std::wctype_t;
using wint_t = std::wint_t;

}

// back door to access private data and functions
namespace re::inner {

template <class T>
struct fo_make {
  template <class...S>
  T operator ()(S &&...s) const {
    return T(static_cast<S &&>(s)...);
  }
};
template <class T>
inline constexpr fo_make<T> make{};

struct fo_to_mutable {
  template <class T>
  auto operator ()(const T &x) const->decltype(x.to_mutable()) {
    return x.to_mutable();
  }
};
inline constexpr fo_to_mutable to_mutable{};

struct fo_node {
  template <class T>
  auto operator ()(const T &x) const->decltype(x.node()) {
    return x.node();
  }
};
inline constexpr fo_node node{};

struct fo_base {
  template <class T>
  constexpr auto operator ()(T &&x) const
    ->decltype(static_cast<T &&>(x).base()) {
    return static_cast<T &&>(x).base();
  }
};
inline constexpr fo_base base{};

struct fo_good {
  template <class T>
  auto operator ()(const T &x) const->decltype(x.good()) {
    return x.good();
  }
};
inline constexpr fo_good good{};

}

// puti
// is_big_endian
// is_little_endian
// putb
namespace re {

template <class, class>
struct is_same;
template <class>
struct decay;
template <class>
struct is_integral;
struct fo_puti {
  template <class T>
  void operator ()(T x) const
    requires is_same<typename decay<T>::type, bool>::value {
    puts(x ? "1" : "0");
    fflush(stdout);
  }
  template <class T>
  void operator ()(T x) const
    requires (!is_same<typename decay<T>::type, bool>::value
              && is_integral<T>::value && !(T(0) < T(-1))) {
    char s[40]{};
    char *p = s;
    if (x < 0) {
      *p = '-';
      ++p;
      x = -x;
    }
    for (;;) {
      *p = '0' + x % 10;
      ++p;
      x /= 10;
      if (x == 0)
        break;
    }
    {
      auto it = ((s[0] == '-') ? s + 1 : s + 0);
      auto it2 = p;
      while (it < it2) {
        --it2;
        const auto tmp = *it;
        *it = *it2;
        *it2 = tmp;
        ++it;
      }
    }
    *p = '\0';
    puts(static_cast<const char *>(s));
    fflush(stdout);
  }
  template <class T>
  void operator ()(T x) const
    requires (!is_same<typename decay<T>::type, bool>::value
              && is_integral<T>::value && (T(0) < T(-1))) {
    char s[40]{};
    char *p = s;
    for (;;) {
      *p = '0' + static_cast<int>(x % 10u);
      ++p;
      x /= 10u;
      if (x == 0u)
        break;
    }
    for (auto it = s + 0, it2 = p; it < it2;) {
      --it2;
      const auto tmp = *it;
      *it = *it2;
      *it2 = tmp;
      ++it;
    }
    *p = '\0';
    puts(static_cast<const char *>(s));
    fflush(stdout);
  }
};
inline constexpr fo_puti puti{};

static inline bool is_little_endian_v
  = (std::endian::native == std::endian::little);
static inline bool is_big_endian_v = !is_little_endian_v;

struct fo_putb {
  template <class T>
  void operator ()(T x) const {
    char s[130]{};
    char *sp = s;
    const unsigned char *p
      = &reinterpret_cast<const unsigned char &>(x);
    const unsigned char *pp = p + sizeof(T);
    if (is_little_endian_v) {
      while (p != pp) {
        --pp;
        unsigned char a = 0b10000000;
        for (; a != 0u; a >>= 1u) {
          *sp = (((*pp & a) != 0u) ? '1' : '0');
          ++sp;
        }
      }
    }
    else {
      for (; p != pp; ++p) {
        unsigned char a = 0b10000000;
        for (; a != 0u; a >>= 1u) {
          *sp = (((*p & a) != 0u) ? '1' : '0');
          ++sp;
        }
      }
    }
    *sp = '\0';
    puts(s);
    fflush(stdout);
  }
};
inline constexpr fo_putb putb{};

}

// print_then_terminate
// throw_or_terminate
namespace re {

struct fo_print_then_terminate {
  void operator ()(const char *s) const {
    fputs(s, stderr);
    fflush(stderr);
    terminate();
  }
  template <class...S>
  void operator ()(const char *s, S &&...ss) const
    requires (sizeof...(S) != 0) {
    fputs(s, stderr);
    operator ()(static_cast<S &&>(ss)...);
  }
};
inline constexpr fo_print_then_terminate print_then_terminate{};

template <class E>
struct fo_throw_or_terminate {
  void operator ()() const {
#ifdef RE_NOEXCEPT
    terminate();
#else
    throw E{};
#endif
  }
  template <class T>
  void operator ()(T &&s) const {
#ifdef RE_NOEXCEPT
    fputs(s, stderr);
    fflush(stderr);
    terminate();
#else
    throw E(static_cast<T &&>(s));
#endif
  }
};
template <class E>
inline constexpr fo_throw_or_terminate<E> throw_or_terminate{};

}

// forward declarations
namespace re {

template <bool Y, class T = void>
struct enable_if;
template <bool Y, class T = void>
using enable_if_t = typename enable_if<Y, T>::type;

template <class>
struct add_lvalue_reference;
template <class>
struct add_rvalue_reference;

template <class>
struct type_tag;
template <class...>
struct type_pack;

template <class>
struct is_move_constructible;
template <class>
struct is_move_assignable;
template <class>
struct is_nothrow_move_constructible;
template <class>
struct is_nothrow_move_assignable;

template <bool, class, class>
struct conditional;
template <class>
struct remove_cv;
template <class>
struct decay;
template <class, class>
struct is_same;
template <class T>
struct is_referenceable;

template <class>
struct integral_traits;

template <class...S>
struct common_type;
template <class...S>
using common_type_t = typename common_type<S...>::type;

template <class T>
struct negation;

template <class, class...>
struct invoke_result;
template <class, class...>
struct is_nothrow_invocable;
struct fo_invoke;

template <class BASE, class DERIVED>
struct is_base_of;
template <class BASE, class DERIVED>
inline constexpr bool is_base_of_v = is_base_of<BASE, DERIVED>::value;

template <bool Y, class T, class U>
struct conditional;
template <bool Y, class T, class U>
using conditional_t = typename conditional<Y, T, U>::type;

template <class T>
struct add_const;
template <class T>
struct add_volatile;
template <class T>
struct add_cv;

template <class>
struct remove_all_extents;

template <class T>
struct remove_reference;

}

// basic template metaprogramming tools
namespace re {

template <class T>
struct fo_declval {
  typename add_rvalue_reference<T>::type operator ()() const noexcept;
};
template <class T>
inline constexpr fo_declval<T> declval{};

template <class T, T...>
struct integer_sequence {};
template <size_t...I>
using index_sequence = integer_sequence<size_t, I...>;
namespace inner {

template <class T, T FROM, T TO, T...N>
struct make_integer_sequence_impl {
  using type
    = typename make_integer_sequence_impl<T, FROM + 1, TO, N..., FROM>::type;
};
template <class T, T TO, T...N>
struct make_integer_sequence_impl<T, TO, TO, N...> {
  using type = integer_sequence<T, N...>;
};

template <class T, T S, T FROM, T TO, T...N>
struct make_integer_sequence_by_stride_impl {
  using type = typename make_integer_sequence_by_stride_impl
    <T, S, FROM + 1, TO, N..., FROM * S>::type;
};
template <class T, T S, T TO, T...N>
struct make_integer_sequence_by_stride_impl<T, S, TO, TO, N...> {
  using type = integer_sequence<T, N...>;
};

template <class T, class...S>
struct integer_sequence_cat_impl;
template <class T>
struct integer_sequence_cat_impl<T> {
  using type = integer_sequence<T>;
};
template <class T, T...IDS>
struct integer_sequence_cat_impl<T, integer_sequence<T, IDS...>> {
  using type = integer_sequence<T, IDS...>;
};
template <class T, T...P1, T...P2, class...S>
struct integer_sequence_cat_impl<T,
                                 integer_sequence<T, P1...>,
                                 integer_sequence<T, P2...>,
                                 S...> {
  using type = typename integer_sequence_cat_impl
    <T, integer_sequence<T, P1..., P2...>, S...>::type;
};

}
template <class T, T N>
using make_integer_sequence
  = typename inner::make_integer_sequence_impl<T, 0, N>::type;
template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

template <class T, T N, T S>
using make_integer_sequence_by_stride
  = typename inner::make_integer_sequence_by_stride_impl<T, S, 0, N>::type;
template <size_t N, size_t S>
using make_index_sequence_by_stride
  = make_integer_sequence_by_stride<size_t, N, S>;

template <class T, class...S>
using integer_sequence_cat
  = typename inner::integer_sequence_cat_impl<T, S...>::type;
template <class...S>
using index_sequence_cat
  = typename inner::integer_sequence_cat_impl<size_t, S...>::type;

namespace inner {

template <class T, class S, T N>
struct integer_sequence_offset_impl;
template <class T, T...IDS, T N>
struct integer_sequence_offset_impl<T, integer_sequence<T, IDS...>, N> {
  using type = integer_sequence<T, (IDS + N)...>;
};

}
template <class T, class S, T N>
using integer_sequence_offset
  = typename inner::integer_sequence_offset_impl<T, S, N>::type;
template <class S, size_t N>
using index_sequence_offset
  = typename inner::integer_sequence_offset_impl<size_t, S, N>::type;

template <class T, T V>
struct integral_constant {
  static constexpr T value{V};
  using value_type = T;
  using type = integral_constant<T, V>;
  constexpr operator value_type() const noexcept {
    return value;
  }
  constexpr value_type operator()() const noexcept {
    return value;
  }
};
template <bool B>
using bool_constant = integral_constant<bool, B>;
template <size_t B>
using size_constant = integral_constant<size_t, B>;
template <int B>
using int_constant = integral_constant<int, B>;
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

namespace inner {

struct enable {};
struct disable {};

template <class F, class...S>
struct f_is_well_formed_impl {
  template <class FF, class...SS>
  static decltype(FF::f(declval<type_pack<SS...>>())) f(type_pack<FF, SS...>);
  static disable f(...);

  static constexpr bool value = !is_same
    <decltype(f(declval<type_pack<F, S...>>())), disable>::value;
};

template <class F, class T, class...S>
struct return_type_of_f_or_impl {
  template <class FF, class...SS>
  static type_tag<typename decltype(FF::f(declval<type_pack<SS...>>()))::type>
  f(type_pack<FF, SS...>);
  static type_tag<T> f(...);

  using type = typename decltype(f(declval<type_pack<F, S...>>()))::type;
};

}
template <class F, class...S>
struct f_is_well_formed
  : bool_constant<inner::f_is_well_formed_impl<F, S...>::value> {};
template <class F, class...S>
inline constexpr bool f_is_well_formed_v = f_is_well_formed<F, S...>::value;

template <class F, class T, class...S>
struct return_type_of_f_or {
  using type = typename inner::return_type_of_f_or_impl<F, T, S...>::type;
};
template <class F, class T, class...S>
using return_type_of_f_or_t = typename return_type_of_f_or<F, T, S...>::type;


namespace inner {

struct check_has_member_type_type {
  template <class T>
  static type_tag<typename T::type> f(type_pack<T>);
};

}
template <class T>
struct has_member_type_type
  : f_is_well_formed<inner::check_has_member_type_type, T> {};
template <class T>
inline constexpr bool has_member_type_type_v = has_member_type_type<T>::value;


namespace inner {

struct check_has_member_static_value {
  template <class T>
  static type_tag<decltype(T::value)> f(type_pack<T>);
};

}
template <class T>
struct has_member_static_value
  : f_is_well_formed<inner::check_has_member_static_value, T> {};
template <class T>
inline constexpr bool has_member_static_value_v
  = has_member_static_value<T>::value;


template <size_t N, class...S>
struct nth_type {
  using type = void;
};
template <class T, class...S>
struct nth_type<0, T, S...> {
  using type = T;
};
template <size_t N, class T, class...S>
struct nth_type<N, T, S...> {
  using type = typename nth_type<N - 1, S...>::type;
};
template <size_t N, class...S>
using nth_type_t = typename nth_type<N, S...>::type;


template <class T, class...S>
struct is_one_of_types : false_type {};
template <class T, class U, class...S>
struct is_one_of_types<T, U, S...> : is_one_of_types<T, S...> {};
template <class T, class...S>
struct is_one_of_types<T, T, S...> : true_type {};
template <class T>
struct is_one_of_types<T> : false_type {};
template <class T, class...S>
inline constexpr bool is_one_of_types_v = is_one_of_types<T, S...>::value;


namespace inner {

template <size_t I, class T, class...S>
struct find_type_impl;
template <size_t I, class T, class U, class...S>
struct find_type_impl<I, T, U, S...> : find_type_impl<I + 1, T, S...> {};
template <size_t I, class T, class...S>
struct find_type_impl<I, T, T, S...> : size_constant<I> {};
template <size_t I, class T>
struct find_type_impl<I, T> : size_constant<I> {};

}
template <class T, class...S>
struct find_type : inner::find_type_impl<0, T, S...>::type {};
template <class T, class...S>
inline constexpr size_t find_type_v = find_type<T, S...>::value;


namespace inner {

template <class T, class...>
struct occurs_exactly_once_impl;
template <class T, class U, class...S>
struct occurs_exactly_once_impl<T, U, S...>
  : occurs_exactly_once_impl<T, S...> {};
template <class T, class...S>
struct occurs_exactly_once_impl<T, T, S...>
  : negation<is_one_of_types<T, S...>> {};
template <class T>
struct occurs_exactly_once_impl<T> : false_type {};

}
template <class T, class...S>
struct occurs_exactly_once : inner::occurs_exactly_once_impl<T, S...>::type {};
template <class T, class...S>
inline constexpr bool occurs_exactly_once_v
  = occurs_exactly_once<T, S...>::value;


template <class X, class Y>
struct compile_time_add
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>,
                      static_cast<common_type_t<typename X::value_type,
                                                typename Y::value_type>>
                      (X::value + Y::value)> {};
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_add_v = compile_time_add<X, Y>::value;
template <class X, class Y>
using compile_time_add_t = typename compile_time_add<X, Y>::type;


template <class X, class Y>
struct compile_time_sub
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>,
                      static_cast<common_type_t<typename X::value_type,
                                                typename Y::value_type>>
                      (X::value - Y::value)> {};
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_sub_v = compile_time_sub<X, Y>::value;
template <class X, class Y>
using compile_time_sub_t = typename compile_time_sub<X, Y>::type;


template <class X, class Y>
struct compile_time_mul
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>,
                      static_cast<common_type_t<typename X::value_type,
                                                typename Y::value_type>>
                      (X::value * Y::value)> {};
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_mul_v = compile_time_mul<X, Y>::value;
template <class X, class Y>
using compile_time_mul_t = typename compile_time_mul<X, Y>::type;


template <class X, class Y>
struct compile_time_div
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>,
                      static_cast<common_type_t<typename X::value_type,
                                                typename Y::value_type>>
                      (Y::value == 0 ? 0 : X::value / Y::value)> {};
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_div_v = compile_time_div<X, Y>::value;
template <class X, class Y>
using compile_time_div_t = typename compile_time_div<X, Y>::type;


template <class X, class Y, bool = Y::value != 0>
struct compile_time_mod
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>,
                      static_cast<common_type_t<typename X::value_type,
                                                typename Y::value_type>>
                      (X::value % Y::value)> {};
template <class X, class Y>
struct compile_time_mod<X, Y, false>
  : integral_constant<common_type_t<typename X::value_type,
                                    typename Y::value_type>, 0> {};
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_mod_v = compile_time_mod<X, Y>::value;
template <class X, class Y>
using compile_time_mod_t = typename compile_time_mod<X, Y>::type;


template <class X>
struct compile_time_neg
  : integral_constant<typename X::value_type,
                      static_cast<typename X::value_type>(-X::value)> {};
template <class X>
using compile_time_neg_t = typename compile_time_neg<X>::type;
template <class X>
inline constexpr typename X::value_type compile_time_neg_v
  = compile_time_neg<X>::value;


template <class X>
struct compile_time_abs
  : integral_constant<typename X::value_type,
                      conditional_t<(X::value < 0), compile_time_neg<X>, X>
                      ::value> {};
template <class X>
using compile_time_abs_t = typename compile_time_abs<X>::type;
template <class X>
inline constexpr typename X::value_type compile_time_abs_v
  = compile_time_abs<X>::value;


namespace inner {

template <class R, class X, class Y>
struct compile_time_gcd_impl
  : conditional_t<X::value == 0, integral_constant<R, Y::value>,
                  conditional_t<Y::value == 0, integral_constant<R, X::value>,
                                conditional_t<compile_time_mod_v<X, Y> == 0,
                                              integral_constant<R, Y::value>,
                                              compile_time_gcd_impl
                                              <R, Y, compile_time_mod_t<X, Y>>>
                                >
                  > {};

}
template <class X, class Y, bool = (X::value > Y::value)>
struct compile_time_gcd
  : inner::compile_time_gcd_impl<common_type_t<typename X::value_type,
                                               typename Y::value_type>,
                                 compile_time_abs_t<X>,
                                 compile_time_abs_t<Y>>::type {};
template <class X, class Y>
struct compile_time_gcd<X, Y, false> : compile_time_gcd<Y, X> {};
template <class X>
struct compile_time_gcd<X, X, true> : compile_time_abs_t<X> {};
template <class X>
struct compile_time_gcd<X, X, false> : compile_time_abs_t<X> {};
template <class X, class Y>
using compile_time_gcd_t = typename compile_time_gcd<X, Y>::type;
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_gcd_v = compile_time_gcd<X, Y>::value;


template <class X, class Y>
struct compile_time_lcm
  : conditional_t<X::value == 0 || Y::value == 0,
                  integral_constant<common_type_t<typename X::value_type,
                                                  typename Y::value_type>, 0>,
                  compile_time_mul_t<compile_time_div_t
                                     <compile_time_abs_t<X>,
                                      compile_time_gcd_t<X, Y>>,
                                     compile_time_abs_t<Y>>> {};
template <class X, class Y>
using compile_time_lcm_t = typename compile_time_lcm<X, Y>::type;
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_lcm_v = compile_time_lcm<X, Y>::value;


template <class X, class Y>
struct compile_time_max
  : conditional_t
  <(X::value < Y::value),
   integral_constant
   <common_type_t<typename X::value_type, typename Y::value_type>,
    static_cast<common_type_t<typename X::value_type, typename Y::value_type>>
    (Y::value)>,
   integral_constant
   <common_type_t<typename X::value_type, typename Y::value_type>,
    static_cast<common_type_t<typename X::value_type, typename Y::value_type>>
    (X::value)>
   > {};
template <class X, class Y>
using compile_time_max_t = typename compile_time_max<X, Y>::type;
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_max_v = compile_time_max<X, Y>::value;


template <class X, class Y>
struct compile_time_min
  : conditional_t
  <(Y::value < X::value),
   integral_constant
   <common_type_t<typename X::value_type, typename Y::value_type>,
    static_cast<common_type_t<typename X::value_type, typename Y::value_type>>
    (Y::value)>,
   integral_constant
   <common_type_t<typename X::value_type, typename Y::value_type>,
    static_cast<common_type_t<typename X::value_type, typename Y::value_type>>
    (X::value)>
   > {};
template <class X, class Y>
using compile_time_min_t = typename compile_time_min<X, Y>::type;
template <class X, class Y>
inline constexpr common_type_t<typename X::value_type, typename Y::value_type>
compile_time_min_v = compile_time_min<X, Y>::value;


template <template <class, class> class BOP, class...S>
struct compile_time_acc;
template <template <class, class> class BOP, class INIT>
struct compile_time_acc<BOP, INIT> {
  using type = INIT;
};
template <template <class, class> class BOP, class INIT, class T>
struct compile_time_acc<BOP, INIT, T> : BOP<INIT, T> {};
template <template <class, class> class BOP, class INIT, class T, class...S>
struct compile_time_acc<BOP, INIT, T, S...> {};
template <template <class, class> class BOP, class INIT, class T, class...S>
requires requires {typename BOP<INIT, T>::type;}
struct compile_time_acc<BOP, INIT, T, S...>
  : compile_time_acc<BOP, typename BOP<INIT, T>::type, S...> {};
template <template <class, class> class BOP, class...S>
using compile_time_acc_t = typename compile_time_acc<BOP, S...>::type;
template <template <class, class> class BOP, class...S>
inline constexpr common_type_t<typename S::value_type...>
compile_time_acc_v = compile_time_acc_t<BOP, S...>::value;


template <class T>
struct type_tag {
  using type = T;
};


struct type_pack_tag {};
template <class T>
struct type_pack_size : size_constant<1> {};
template <class T>
inline constexpr size_t type_pack_size_v = type_pack_size<T>::value;
template <size_t N, class T>
struct type_pack_element : nth_type<N, T> {};
template <size_t N, class T>
using type_pack_element_t = typename type_pack_element<N, T>::type;

template <class A, class B>
struct type_pack_eql {
private:
  template <size_t...I>
  static type_pack<enable_if_t<is_same<type_pack_element_t<I, A>,
                                       type_pack_element_t<I, B>>::value>...>
  helper_f(index_sequence<I...>);
  static inner::disable helper_f(...);

public:
  static constexpr bool value = (type_pack_size_v<A> == type_pack_size_v<B>)
    && !is_same<decltype(helper_f
                         (make_index_sequence<type_pack_size_v<A>>{})),
                inner::disable>::value;
};
template <class A, class B>
inline constexpr bool type_pack_eql_v = type_pack_eql<A, B>::value;

template <class...S>
struct type_pack : type_pack_tag {
  using type = type_pack;
};
template <class...S>
struct type_pack_size<type_pack<S...>> : size_constant<sizeof...(S)> {};
template <size_t N, class...S>
struct type_pack_element<N, type_pack<S...>> : nth_type<N, S...> {};

template <class INT, INT...I>
struct type_pack_size<integer_sequence<INT, I...>>
  : size_constant<sizeof...(I)> {};
template <size_t N, class INT, INT...I>
struct type_pack_element<N, integer_sequence<INT, I...>>
  : nth_type<N, integral_constant<INT, I>...> {};

namespace inner::fns {

template <class T, class F, size_t...I>
constexpr void type_pack_for_each_impl0(F, index_sequence<I...>);

template <class T, size_t I, size_t J, class F>
enable_if_t<I == J> type_pack_for_each_impl(F) {}
template <class T, size_t I, size_t J, class F>
enable_if_t<I != J> type_pack_for_each_impl(F f) {
  type_pack_for_each_impl0<type_pack_element_t<I, T>>
    (f, make_index_sequence
     <type_pack_size_v<type_pack_element_t<I, T>>>());
  inner::fns::type_pack_for_each_impl<T, I + 1, J>(f);
};

}
template <class T>
struct fo_type_pack_for_each {
  template <class F>
  void operator ()(F f) const {
    inner::fns::type_pack_for_each_impl<T, 0, type_pack_size_v<T>>(f);
  }
};
template <class T>
inline constexpr fo_type_pack_for_each<T> type_pack_for_each{};

namespace inner::fns {

template <class T, class F, size_t...I>
constexpr bool type_pack_for_each_until_false_impl0(F, index_sequence<I...>);

template <class T, size_t I, size_t J, class F>
enable_if_t<I == J> type_pack_for_each_until_false_impl(F) {}
template <class T, size_t I, size_t J, class F>
enable_if_t<I != J> type_pack_for_each_until_false_impl(F f) {
  if (type_pack_for_each_until_false_impl0<type_pack_element_t<I, T>>
      (f, make_index_sequence
       <type_pack_size_v<type_pack_element_t<I, T>>>()) == false)
    return;
  inner::fns::type_pack_for_each_until_false_impl<T, I + 1, J>(f);
};

}
template <class T>
struct fo_type_pack_for_each_until_false {
  template <class F>
  constexpr void operator ()(F f) const {
    inner::fns::type_pack_for_each_until_false_impl
      <T, 0, type_pack_size_v<T>>(f);
  }
};
template <class T>
inline constexpr fo_type_pack_for_each_until_false<T>
type_pack_for_each_until_false{};

template <template <class...> class APPLY, class P>
struct type_pack_apply {
private:
  template <size_t...I>
  static APPLY<type_pack_element_t<I, P>...> helper_f(index_sequence<I...>);
public:
  using type = decltype(helper_f(make_index_sequence<type_pack_size_v<P>>()));
};
template <template <class...> class APPLY, class P>
using type_pack_apply_t = typename type_pack_apply<APPLY, P>::type;

template <class P>
struct type_pack_decay {
private:
  template <size_t...I>
  static type_pack<type_pack_element_t<I, P>...> helper_f(index_sequence<I...>);
public:
  using type = decltype(helper_f(make_index_sequence<type_pack_size_v<P>>()));
};
template <class P>
using type_pack_decay_t = typename type_pack_decay<P>::type;

template <class P, bool = is_base_of_v<type_pack_tag, P>>
struct type_pack_recursive_decay {
private:
  template <size_t...I>
  static type_pack
  <typename type_pack_recursive_decay<type_pack_element_t<I, P>>::type...>
  helper_f(index_sequence<I...>);
public:
  using type = decltype(helper_f(make_index_sequence<type_pack_size_v<P>>()));
};
template <class P>
struct type_pack_recursive_decay<P, false> {
  using type = P;
};
template <class P>
using type_pack_recursive_decay_t = typename type_pack_recursive_decay<P>::type;

template <class A, class B,
          bool = is_base_of_v<type_pack_tag, A>
          || is_base_of_v<type_pack_tag, B>>
struct type_pack_recursive_eql {
private:
  template <size_t...I,
            bool Y = (... && type_pack_recursive_eql
                      <type_pack_element_t<I, A>, type_pack_element_t<I, B>>
                      ::value)>
  static bool_constant<Y> helper_f(index_sequence<I...>);
public:
  static constexpr bool value
    = (type_pack_size_v<A> == type_pack_size_v<B>)
    && decltype(helper_f(make_index_sequence<type_pack_size_v<A>>()))::value;
};
template <class A, class B>
struct type_pack_recursive_eql<A, B, false> : is_same<A, B> {};
template <class A, class B>
inline constexpr bool type_pack_recursive_eql_v
  = type_pack_recursive_eql<A, B>::value;

template <class A, class B>
struct type_pack_add : type_pack_tag {
  using type = type_pack_add;
};
template <class A, class B>
struct type_pack_size<type_pack_add<A, B>>
  : compile_time_add<type_pack_size<A>, type_pack_size<B>>::type {};
namespace inner {

template <class A, class B, size_t N, bool = (N < type_pack_size_v<A>)>
struct type_pack_add_element_impl : type_pack_element<N, A> {};
template <class A, class B, size_t N>
struct type_pack_add_element_impl<A, B, N, false>
  : type_pack_element<compile_time_sub_v
                      <size_constant<N>, type_pack_size<A>>, B> {};

}
template <size_t N, class A, class B>
struct type_pack_element<N, type_pack_add<A, B>>
  : inner::type_pack_add_element_impl<A, B, N> {};

template <class A, class B>
struct type_pack_mul : type_pack_tag {
  using type = type_pack_mul;
  template <class F>
  void operator ()(F &&f) const {
    type_pack_for_each<type_pack_mul<A, B>>(static_cast<F &&>(f));
  }
};
template <class A, class B>
struct type_pack_size<type_pack_mul<A, B>>
  : compile_time_mul<type_pack_size<A>, type_pack_size<B>>::type {};
template <size_t N, class A, class B>
struct type_pack_element<N, type_pack_mul<A, B>> {
  using type
    = type_pack_add
    <type_pack_element_t<compile_time_div_v
                         <size_constant<N>, typename type_pack_size<B>::type>,
                         A>,
     type_pack_element_t<compile_time_mod_v
                         <size_constant<N>, typename type_pack_size<B>::type>,
                         B>>;
};

template <size_t N, class P>
struct type_pack_first_part : type_pack_tag {};
template <size_t N, class P>
struct type_pack_size<type_pack_first_part<N, P>> : size_constant<N> {};
template <size_t I, size_t N, class P>
struct type_pack_element<I, type_pack_first_part<N, P>>
  : type_pack_element<I, P> {};

template <size_t N, class P>
struct type_pack_second_part : type_pack_tag {};
template <size_t N, class P>
struct type_pack_size<type_pack_second_part<N, P>>
  : compile_time_sub<type_pack_size<P>, size_constant<N>> {};
template <size_t I, size_t N, class P>
struct type_pack_element<I, type_pack_second_part<N, P>>
  : type_pack_element<I + N, P> {};

namespace inner {

template <size_t N, class T>
struct n_type_pack_impl {
  using type = type_pack_add<type_pack<T>,
                             typename inner::n_type_pack_impl<N - 1, T>::type>;
};
template <class T>
struct n_type_pack_impl<0, T> {
  using type = type_pack<>;
};

}
template <size_t N, class T>
struct n_type_pack
  : type_pack_decay<typename inner::n_type_pack_impl<N, T>::type> {};
template <size_t N, class T>
using n_type_pack_t = typename n_type_pack<N, T>::type;

template <class...>
struct conjunction : true_type {};
template <class T, class...S>
struct conjunction<T, S...>
  : conditional<!T::value, T, conjunction<S...>>::type {};
template <class...S>
inline constexpr bool conjunction_v = conjunction<S...>::value;

template <class...>
struct disjunction : false_type {};
template <class T, class...S>
struct disjunction<T, S...>
  : conditional<T::value, T, disjunction<S...>>::type {};
template <class...S>
inline constexpr bool disjunction_v = disjunction<S...>::value;

template <class T>
struct negation : conditional<T::value, false_type, true_type>::type {};
template <class T>
inline constexpr bool negation_v = negation<T>::value;


namespace inner {

template <size_t ID, class...S>
struct select_type_helper;
template <size_t ID>
struct select_type_helper<ID> {
  static size_constant<ID> f(...);
};
template <size_t ID, class X, class...S>
struct select_type_helper<ID, X, S...> : select_type_helper<ID + 1, S...> {
  using select_type_helper<ID + 1, S...>::f;
  static size_constant<ID> f(X);
};

template <class X, class...S>
struct select_type_impl {
private:
  using F = inner::select_type_helper<0, S...>;
  static size_constant<sizeof...(S)> f(...);
  template <class U = decltype(F::f(declval<X>()))>
  static U f(int);
public:
  static constexpr size_t value = decltype(f(0))::value;
};

}
template <class X, class...S>
struct select_type
  : size_constant<inner::select_type_impl<X, S...>::value> {};
template <class X, class...S>
inline constexpr size_t select_type_v = select_type<X, S...>::value;

struct fo_accumulate_args {
  template <class F, class X>
  constexpr X operator ()(F, X ret) const {
    return ret;
  }
  template <class F, class X, class S0, class...S>
  constexpr decltype(auto) operator ()(F add, X ret, S0 x, S...s) const {
    return operator ()(add,
                       add(static_cast<X &&>(ret), static_cast<S0 &&>(x)),
                       static_cast<S &&>(s)...);
  }
};
inline constexpr fo_accumulate_args accumulate_args{};

namespace inner::fns {

template <size_t I, class T, class...S>
constexpr nth_type_t<I, T &&, S &&...>
nth_arg_impl(T &&x, S &&...s) noexcept {
  if constexpr (I == 0) {
    return static_cast<T &&>(x);
  }
  else {
    return inner::fns::nth_arg_impl<I - 1>(static_cast<S &&>(s)...);
  }
}

}
template <size_t I>
struct fo_nth_arg {
  template <class T, class...S>
  constexpr nth_type_t<I, T &&, S &&...>
  operator ()(T &&x, S &&...s) const noexcept {
    return inner::fns::nth_arg_impl<I, T &&, S &&...>(static_cast<T &&>(x),
                                                      static_cast<S &&>(s)...);
  }
};
template <size_t I>
inline constexpr fo_nth_arg<I> nth_arg{};

}

// swap
namespace re_adl {

template <class T1, class T2>
struct swap_is_well_formed {
  template <class T>
  static T &&get_ref() noexcept;

  template <class TT1, class TT2>
  static char f(...);
  template <class TT1, class TT2,
            class = decltype(swap(get_ref<TT1>(), get_ref<TT2>()))>
  static long long f(int);

  static constexpr bool value = sizeof(decltype(f<T1, T2>(0))) != 1;
};

template <class T1, class T2, bool = swap_is_well_formed<T1, T2>::value>
struct swap_is_noexcept {
  static constexpr bool value = false;
};
template <class T1, class T2>
struct swap_is_noexcept<T1, T2, true> {
  template <class T>
  static T &&get_ref() noexcept;

  static constexpr bool value = noexcept(swap(get_ref<T1>(), get_ref<T2>()));
};

template <class T, class U>
constexpr void do_swap(T &&x, U &&y) {
  swap(static_cast<T &&>(x), static_cast<U &&>(y));
}

}
namespace re {

struct fo_default_swap {
  template <class T>
  constexpr void operator ()(T &x, T &y)
    const noexcept(is_nothrow_move_constructible<T>::value
                   && is_nothrow_move_assignable<T>::value)
    requires (is_move_constructible<T>::value
              && is_move_assignable<T>::value) {
    T z = static_cast<T &&>(x);
    x = static_cast<T &&>(y);
    y = static_cast<T &&>(z);
  }
};
inline constexpr fo_default_swap default_swap{};
template <class R>
concept default_swappable = is_move_constructible<R>::value
  && is_move_assignable<R>::value;
template <class R>
concept nothrow_default_swappable = is_nothrow_move_constructible<R>::value
  && is_nothrow_move_assignable<R>::value;

template <class, class>
struct is_swappable_with;
template <class, class>
struct is_nothrow_swappable_with;

template <class>
struct is_rvalue_reference;
struct fo_adl_swap {
private:
  template <class T>
  static constexpr void f(T &x, T &y)
    noexcept(is_nothrow_move_constructible<T>::value
             && is_nothrow_move_assignable<T>::value)
    requires is_move_constructible<T>::value
    && is_move_assignable<T>::value {
    T z = static_cast<T &&>(x);
    x = static_cast<T &&>(y);
    y = static_cast<T &&>(z);
  }
  template <class T, size_t N>
  static constexpr void f(T (&x)[N], T (&y)[N])
    noexcept(is_nothrow_swappable_with<T &, T &>::value)
    requires is_swappable_with<T &, T &>::value {
    const auto x_end = x + N;
    for (T *p = x, *pp = y; p != x_end;) {
      fo_adl_swap{}(*p, *pp);
      ++p;
      ++pp;
    }
  }

public:
  template <class T, class U>
  constexpr void operator ()(T &&x, U &&y) const
    noexcept(re_adl::swap_is_noexcept<T, U>::value)
    requires re_adl::swap_is_well_formed<T, U>::value {
    re_adl::do_swap(static_cast<T &&>(x), static_cast<U &&>(y));
  }
  template <class T, class U>
  constexpr auto operator ()(T &&x, U &&y) const
    noexcept(noexcept(f(declval<T>(), declval<U>())))
    ->decltype(f(declval<T>(), declval<U>())) {
    f(static_cast<T &&>(x), static_cast<U &&>(y));
  }
};
inline constexpr fo_adl_swap adl_swap;

namespace inner {

struct check_is_swappable_with {
  template <class T, class U>
  static type_pack<decltype(adl_swap(declval<T>(), declval<U>())),
                   decltype(adl_swap(declval<U>(), declval<T>()))>
  f(type_pack<T, U>);
};

}
template <class T, class U>
struct is_swappable_with
  : f_is_well_formed<inner::check_is_swappable_with, T, U> {};
template <class T, class U>
inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;
template <class T>
struct is_swappable : is_swappable_with<T &, T &> {};
template <class T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

namespace inner {

template <class T, class U, bool Y = is_swappable_with<T, U>::value>
struct is_nothrow_swappable_with_impl {
  static constexpr bool value = false;
};
template <class T, class U>
struct is_nothrow_swappable_with_impl<T, U, true> {
  static constexpr bool value = noexcept(adl_swap(declval<T>(), declval<U>()));
};

}
template <class T, class U>
struct is_nothrow_swappable_with
  : bool_constant<inner::is_nothrow_swappable_with_impl<T, U>::value> {};
template <class T, class U>
inline constexpr bool is_nothrow_swappable_with_v
  = is_nothrow_swappable_with<T, U>::value;
template <class T>
using is_nothrow_swappable = is_nothrow_swappable_with<T &, T &>;
template <class T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

}

// type traits
namespace re {

// primary type categories

template <class T>
struct is_void : is_same<typename remove_cv<T>::type, void> {};
template <class T>
inline constexpr bool is_void_v = is_void<T>::value;

template <class T>
struct is_null_pointer : is_same<typename remove_cv<T>::type, nullptr_t> {};
template <class T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

template <class T>
struct is_integral
  : bool_constant<integral_traits
                  <typename remove_cv<T>::type>::is_specialized> {};
template <class T>
inline constexpr bool is_integral_v = is_integral<T>::value;

template <class T>
struct is_floating_point
  : is_one_of_types<typename remove_cv<T>::type, float, double, long double> {};
template <class T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template <class T>
struct is_array : false_type {};
template <class T>
struct is_array<T []> : true_type {};
template <class T, size_t N>
struct is_array<T [N]> : true_type {};
template <class T>
inline constexpr bool is_array_v = is_array<T>::value;

template <class T>
struct is_pointer : false_type {};
template <class T>
struct is_pointer<T *> : true_type {};
template <class T>
struct is_pointer<T *const> : true_type {};
template <class T>
struct is_pointer<T *volatile> : true_type {};
template <class T>
struct is_pointer<T *const volatile> : true_type {};
template <class T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

template <class T>
struct is_lvalue_reference : false_type {};
template <class T>
struct is_lvalue_reference<T &> : true_type {};
template <class T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template <class T>
struct is_rvalue_reference : false_type {};
template <class T>
struct is_rvalue_reference<T &&> : true_type {};
template <class T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template <class T>
struct is_function;
template <class T>
struct is_member_object_pointer : false_type {};
template <class T, class U>
struct is_member_object_pointer<T U::*>
  : bool_constant<!is_function<T>::value> {};
template <class T, class U>
struct is_member_object_pointer<T U::*const>
  : bool_constant<!is_function<T>::value> {};
template <class T, class U>
struct is_member_object_pointer<T U::*volatile>
  : bool_constant<!is_function<T>::value> {};
template <class T, class U>
struct is_member_object_pointer<T U::*const volatile>
  : bool_constant<!is_function<T>::value> {};
template <class T>
inline constexpr bool is_member_object_pointer_v
  = is_member_object_pointer<T>::value;

template <class T>
struct is_member_function_pointer : false_type {};
template <class T, class U>
struct is_member_function_pointer<T U::*>
  : bool_constant<is_function<T>::value> {};
template <class T, class U>
struct is_member_function_pointer<T U::*const>
  : bool_constant<is_function<T>::value> {};
template <class T, class U>
struct is_member_function_pointer<T U::*volatile>
  : bool_constant<is_function<T>::value> {};
template <class T, class U>
struct is_member_function_pointer<T U::*const volatile>
  : bool_constant<is_function<T>::value> {};
template <class T>
inline constexpr bool is_member_function_pointer_v
  = is_member_function_pointer<T>::value;

template <class T>
struct is_enum : bool_constant<std::is_enum<T>::value> {};
template <class T>
inline constexpr bool is_enum_v = is_enum<T>::value;

template <class T>
struct is_union : bool_constant<std::is_union<T>::value> {};
template <class T>
inline constexpr bool is_union_v = is_union<T>::value;

template <class T>
struct is_class : bool_constant<std::is_class<T>::value> {};
template <class T>
inline constexpr bool is_class_v = is_class<T>::value;

template <class T>
struct is_function : false_type {};
template <class T, class...S>
struct is_function<T (S...)> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile> : true_type {};
template <class T, class...S>
struct is_function<T (S...) &> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const &> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile &> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile &> : true_type {};
template <class T, class...S>
struct is_function<T (S...) &&> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const &&> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile &&> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile &&> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...)> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) &> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const &> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile &> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile &> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) &&> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const &&> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile &&> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile &&> : true_type {};
template <class T, class...S>
struct is_function<T (S...) noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) volatile && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S...) const volatile && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile & noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) volatile && noexcept> : true_type {};
template <class T, class...S>
struct is_function<T (S..., ...) const volatile && noexcept> : true_type {};
template <class T>
inline constexpr bool is_function_v = is_function<T>::value;

// composite type categories

template <class T>
struct is_reference : false_type {};
template <class T>
struct is_reference<T &> : true_type {};
template <class T>
struct is_reference<T &&> : true_type {};
template <class T>
inline constexpr bool is_reference_v = is_reference<T>::value;

template <class T>
struct is_arithmetic : bool_constant<is_integral<T>::value
                                     || is_floating_point<T>::value> {};
template <class T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
struct is_fundamental : bool_constant<is_arithmetic<T>::value
                                      || is_void<T>::value
                                      || is_null_pointer<T>::value> {};
template <class T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

template <class T>
struct is_object : bool_constant<!is_reference<T>::value
                                 && !is_function<T>::value
                                 && !is_void<T>::value> {};
template <class T>
inline constexpr bool is_object_v = is_object<T>::value;

template <class T>
struct is_member_pointer;
template <class T>
struct is_scalar : bool_constant<is_arithmetic<T>::value
                                 || is_enum<T>::value
                                 || is_pointer<T>::value
                                 || is_member_pointer<T>::value
                                 || is_null_pointer<T>::value> {};
template <class T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

template <class T>
struct is_compound : bool_constant<is_array<T>::value
                                   || is_function<T>::value
                                   || is_pointer<T>::value
                                   || is_reference<T>::value
                                   || is_class<T>::value
                                   || is_union<T>::value
                                   || is_enum<T>::value
                                   || is_member_pointer<T>::value> {};
template <class T>
inline constexpr bool is_compound_v = is_compound<T>::value;

template <class T>
struct is_member_pointer : false_type {};
template <class T, class U>
struct is_member_pointer<T U::*> : true_type {};
template <class T, class U>
struct is_member_pointer<T U::*const> : true_type {};
template <class T, class U>
struct is_member_pointer<T U::*volatile> : true_type {};
template <class T, class U>
struct is_member_pointer<T U::*const volatile> : true_type {};
template <class T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template <class T>
struct is_referenceable
  : bool_constant<is_object<T>::value || is_reference<T>::value> {};
template <class T, class...S>
struct is_referenceable<T (S...)> : true_type {};
template <class T, class...S>
struct is_referenceable<T (S..., ...)> : true_type {};
template <class T, class...S>
struct is_referenceable<T (S...) noexcept> : true_type {};
template <class T, class...S>
struct is_referenceable<T (S..., ...) noexcept> : true_type {};
template <class T>
inline constexpr bool is_referenceable_v = is_referenceable<T>::value;

// type properties

template <class T>
struct is_const : false_type {};
template <class T>
struct is_const<const T> : true_type {};
template <class T>
inline constexpr bool is_const_v = is_const<T>::value;

template <class T>
struct is_volatile : false_type {};
template <class T>
struct is_volatile<volatile T> : true_type {};
template <class T>
inline constexpr bool is_volatile_v = is_volatile<T>::value;

template <class T>
struct is_trivial : bool_constant<std::is_trivial<T>::value> {};
template <class T>
inline constexpr bool is_trivial_v = is_trivial<T>::value;

template <class T>
struct is_trivially_copyable
  : bool_constant<std::is_trivially_copy_constructible<T>::value
                  && std::is_trivially_copy_assignable<T>::value
                  && std::is_trivially_move_constructible<T>::value
                  && std::is_trivially_move_assignable<T>::value
                  && std::is_trivially_copyable<T>::value> {};
template <class T>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

template <class T>
using is_standard_layout = bool_constant<std::is_standard_layout<T>::value>;
template <class T>
inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

template <class T>
struct is_empty : bool_constant<std::is_empty<T>::value> {};
template <class T>
inline constexpr bool is_empty_v = is_empty<T>::value;

template <class T>
struct is_polymorphic : bool_constant<std::is_polymorphic<T>::value> {};
template <class T>
inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

template <class T>
struct is_abstract : bool_constant<std::is_abstract<T>::value> {};
template <class T>
inline constexpr bool is_abstract_v = is_abstract<T>::value;

template <class T>
struct is_final : bool_constant<std::is_final<T>::value> {};
template <class T>
inline constexpr bool is_final_v = is_final<T>::value;

template <class T>
struct is_aggregate : bool_constant<std::is_aggregate<T>::value> {};
template <class T>
inline constexpr bool is_aggregate_v = is_aggregate<T>::value;

template <class T, bool Y = is_arithmetic<T>::value>
struct is_signed : false_type {};
template <class T>
struct is_signed<T, true> : bool_constant<(T(-1) < T(0))> {};
template <class T>
struct is_signed<const T, true> : is_signed<T> {};
template <class T>
struct is_signed<volatile T, true> : is_signed<T> {};
template <class T>
struct is_signed<const volatile T, true> : is_signed<T> {};
template <class T>
inline constexpr bool is_signed_v = is_signed<T>::value;

template <class T, bool Y = is_arithmetic<T>::value>
struct is_unsigned : false_type {};
template <class T>
struct is_unsigned<T, true> : bool_constant<(T(0) < T(-1))> {};
template <class T>
struct is_unsigned<const T, true> : is_unsigned<T> {};
template <class T>
struct is_unsigned<volatile T, true> : is_unsigned<T> {};
template <class T>
struct is_unsigned<const volatile T, true> : is_unsigned<T> {};
template <class T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

template <class T>
struct is_bounded_array : false_type {};
template <class T, size_t N>
struct is_bounded_array<T [N]> : true_type {};
template <class T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

template <class T>
struct is_unbounded_array : false_type {};
template <class T>
struct is_unbounded_array<T []> : true_type {};
template <class T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

// todo: is_scoped_enum

template <class T, class...S>
struct is_constructible
  : bool_constant<std::is_constructible<T, S...>::value> {};
template <class T, class...S>
inline constexpr bool is_constructible_v = is_constructible<T, S...>::value;

template <class T>
struct is_default_constructible : is_constructible<T> {};
template <class T>
inline constexpr bool is_default_constructible_v
  = is_default_constructible<T>::value;

template <class T>
struct is_copy_constructible
  : bool_constant<is_referenceable<T>::value
                  && is_constructible
                  <T, typename add_lvalue_reference
                   <typename add_const<T>::type>::type>::value> {};
template <class T>
inline constexpr bool is_copy_constructible_v
  = is_copy_constructible<T>::value;

template <class T>
struct is_move_constructible
  : bool_constant<is_referenceable<T>::value
                  && is_constructible
                  <T, typename add_rvalue_reference<T>::type>::value> {};
template <class T>
inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

namespace inner {

struct check_is_assignable {
  template <class T, class U>
  static type_tag<decltype(declval<T>() = declval<U>())>
  f(type_pack<T, U>);
};

}
template <class T, class U>
struct is_assignable : f_is_well_formed<inner::check_is_assignable, T, U> {};
template <class T, class U>
inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

template <class T>
struct is_copy_assignable
  : is_assignable<typename add_lvalue_reference<T>::type,
                  typename add_lvalue_reference
                  <typename add_const<T>::type>::type> {};
template <class T>
inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template <class T>
struct is_move_assignable
  : is_assignable<typename add_lvalue_reference<T>::type,
                  typename add_rvalue_reference<T>::type> {};
template <class T>
inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

namespace inner {

struct check_is_destructible {
  template <class T>
  static enable f(type_pack<T &>);
  template <class T>
  static enable f(type_pack<T &&>);

  template <class T>
  static disable f(type_pack<T []>);

  template <class T,
            class U = typename remove_all_extents<T>::type,
            class = typename enable_if<!is_reference<U>::value, void>::type>
  static decltype(declval<U &>().~U()) f(type_pack<T>);
};

}
template <class T>
struct is_destructible : f_is_well_formed<inner::check_is_destructible, T> {};
template <class T>
inline constexpr bool is_destructible_v = is_destructible<T>::value;

template <class T, class...S>
struct is_trivially_constructible
  : bool_constant<std::is_trivially_constructible<T, S...>::value> {};
template <class T, class...S>
inline constexpr bool is_trivially_constructible_v
  = is_trivially_constructible<T, S...>::value;

template <class T>
struct is_trivially_default_constructible
  : bool_constant<std::is_trivially_default_constructible<T>::value> {};
template <class T>
inline constexpr bool is_trivially_default_constructible_v
  = is_trivially_default_constructible<T>::value;

template <class T>
struct is_trivially_copy_constructible
  : bool_constant<std::is_trivially_copy_constructible<T>::value> {};
template <class T>
inline constexpr bool is_trivially_copy_constructible_v
  = is_trivially_copy_constructible<T>::value;

template <class T>
struct is_trivially_move_constructible
  : bool_constant<std::is_trivially_move_constructible<T>::value> {};
template <class T>
inline constexpr bool is_trivially_move_constructible_v
  = is_trivially_move_constructible<T>::value;

template <class T, class U>
struct is_trivially_assignable
  : bool_constant<std::is_trivially_assignable<T, U>::value> {};
template <class T, class U>
inline constexpr bool is_trivially_assignable_v
  = is_trivially_assignable<T, U>::value;

template <class T>
struct is_trivially_copy_assignable
  : bool_constant<std::is_trivially_copy_assignable<T>::value> {};
template <class T>
inline constexpr bool is_trivially_copy_assignable_v
  = is_trivially_copy_assignable<T>::value;

template <class T>
struct is_trivially_move_assignable
  : bool_constant<std::is_trivially_move_assignable<T>::value> {};
template <class T>
inline constexpr bool is_trivially_move_assignable_v
  = is_trivially_move_assignable<T>::value;

template <class T>
struct is_trivially_destructible
  : bool_constant<std::is_trivially_destructible<T>::value> {};
template <class T>
inline constexpr bool is_trivially_destructible_v
  = is_trivially_destructible<T>::value;

namespace inner {

struct check_is_nothrow_constructible {
  template <class T, class...S,
            class = enable_if_t<is_constructible<T, S...>::value>,
            class = enable_if_t<noexcept(typename remove_all_extents<T>::type
                                         (declval<S>()...))>>
  static void f(type_pack<T, S...>);
};

}
template <class T, class...S>
struct is_nothrow_constructible
  : f_is_well_formed<inner::check_is_nothrow_constructible, T, S...> {};
template <class T>
struct is_nothrow_constructible<T []> : false_type {};
template <class T, class...S>
inline constexpr bool is_nothrow_constructible_v
  = is_nothrow_constructible<T, S...>::value;

template <class T>
struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};
template <class T>
inline constexpr bool is_nothrow_default_constructible_v
  = is_nothrow_default_constructible<T>::value;

template <class T>
struct is_nothrow_copy_constructible
  : bool_constant<is_referenceable<T>::value
                  && is_nothrow_constructible
                  <T, typename add_lvalue_reference
                   <typename add_const<T>::type>::type>::value> {};
template <class T>
inline constexpr bool is_nothrow_copy_constructible_v
  = is_nothrow_copy_constructible<T>::value;

template <class T>
struct is_nothrow_move_constructible
  : bool_constant<is_referenceable<T>::value
                  && is_nothrow_constructible
                  <T, typename add_rvalue_reference<T>::type>::value> {};
template <class T>
inline constexpr bool is_nothrow_move_constructible_v
  = is_nothrow_move_constructible<T>::value;

namespace inner {

template <bool Y, class T, class U>
struct is_nothrow_assignable_impl {
  static constexpr bool value = noexcept(declval<T>() = declval<U>());
};
template <class T, class U>
struct is_nothrow_assignable_impl<false, T, U> {
  static constexpr bool value = false;
};

}
template <class T, class U>
struct is_nothrow_assignable
  : bool_constant<inner::is_nothrow_assignable_impl
                  <is_assignable<T, U>::value, T, U>::value> {};
template <class T, class U>
inline constexpr bool is_nothrow_assignable_v
  = is_nothrow_assignable<T, U>::value;

template <class T>
struct is_nothrow_copy_assignable
  : bool_constant<is_referenceable<T>::value
                  && is_nothrow_assignable
                  <typename add_lvalue_reference<T>::type,
                   typename add_lvalue_reference
                   <typename add_const<T>::type>::type>::value> {};
template <class T>
inline constexpr bool is_nothrow_copy_assignable_v
  = is_nothrow_copy_assignable<T>::value;

template <class T>
struct is_nothrow_move_assignable
  : bool_constant<is_referenceable<T>::value
                  && is_nothrow_assignable
                  <typename add_lvalue_reference<T>::type,
                   typename add_rvalue_reference<T>::type>::value> {};
template <class T>
inline constexpr bool is_nothrow_move_assignable_v
  = is_nothrow_move_assignable<T>::value;

namespace inner {

template <class T, bool = is_reference<T>::value>
struct is_nothrow_destructible_impl0 {
  static constexpr bool value = true;
};
template <class T>
struct is_nothrow_destructible_impl0<T, false> {
  using t = typename remove_all_extents<T>::type;
  static constexpr bool value = noexcept(declval<t &>().~t());
};

template <class T, bool = is_destructible<T>::value>
struct is_nothrow_destructible_impl : is_nothrow_destructible_impl0<T> {};
template <class T>
struct is_nothrow_destructible_impl<T, false> {
  static constexpr bool value = false;
};

}
template <class T>
struct is_nothrow_destructible
  : bool_constant<inner::is_nothrow_destructible_impl<T>::value> {};
template <class T>
inline constexpr bool is_nothrow_destructible_v
  = is_nothrow_destructible<T>::value;

template <class T>
struct has_virtual_destructor
  : bool_constant<std::has_virtual_destructor<T>::value> {};
template <class T>
inline constexpr bool has_virtual_destructor_v
  = has_virtual_destructor<T>::value;
template <class T>
struct has_unique_object_representations
  : bool_constant<std::has_unique_object_representations<T>::value> {};
template <class T>
inline constexpr bool has_unique_object_representations_v
  = has_unique_object_representations<T>::value;

// todo:
//   reference_constructs_from_temporary<T, U>
//   reference_converts_from_temporary<T, U>

// type property queries

template <class T>
struct alignment_of : integral_constant<size_t, alignof(T)> {};
template <class T>
inline constexpr size_t alignment_of_v = alignment_of<T>::value;

template <class T>
struct rank : integral_constant<size_t, 0> {};
template <class T, size_t N>
struct rank<T [N]> : integral_constant<size_t, rank<T>::value + 1> {};
template <class T>
struct rank<T []> : integral_constant<size_t, rank<T>::value + 1> {};
template <class T>
inline constexpr size_t rank_v = rank<T>::value;

template <class T, unsigned I = 0>
struct extent : integral_constant<size_t, 0> {};
template <class T>
struct extent<T, 0> : integral_constant<size_t, 0> {};
template <class T, size_t N>
struct extent<T [N], 0> : integral_constant<size_t, N> {};
template <class T, size_t N, unsigned I>
struct extent<T [N], I> : extent<T, I - 1> {};
template <class T, unsigned I = 0>
inline constexpr size_t extent_v = extent<T, I>::value;

// type relations

template <class T, class U>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template <class BASE, class DERIVED>
struct is_base_of : bool_constant<std::is_base_of<BASE, DERIVED>::value> {};

template <class FROM, class TO>
struct is_convertible : bool_constant<std::is_convertible<FROM, TO>::value> {};
template <class FROM, class TO>
inline constexpr bool is_convertible_v = is_convertible<FROM, TO>::value;

template <class FROM, class TO>
struct is_nothrow_convertible
  : bool_constant<std::is_nothrow_convertible<FROM, TO>::value> {};
template <class FROM, class TO>
inline constexpr bool is_nothrow_convertible_v
  = is_nothrow_convertible<FROM, TO>::value;

// todo:
//   is_layout_compatible
//   is_pointer_interconvertible_base_of

// const-volatile modifications

template <class T>
struct remove_const {
  using type = T;
};
template <class T>
struct remove_const<const T> {
  using type = T;
};
template <class T>
using remove_const_t = typename remove_const<T>::type;

template <class T>
struct remove_volatile {
  using type = T;
};
template <class T>
struct remove_volatile<volatile T> {
  using type = T;
};
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;

template <class T>
struct remove_cv {
  using type
    = typename remove_const<typename remove_volatile<T>::type>::type;
};
template <class T>
using remove_cv_t = typename remove_cv<T>::type;

namespace inner {

template <class T, bool = !is_function<T>::value>
struct add_const_impl {
  using type = T;
};
template <class T>
struct add_const_impl<T, true> {
  using type = const T;
};
template <class T>
struct add_const_impl<const T, true> {
  using type = const T;
};

}
template <class T>
struct add_const {
  using type = typename inner::add_const_impl<T>::type;
};
template <class T>
using add_const_t = typename add_const<T>::type;

namespace inner {

template <class T, bool = !is_function<T>::value>
struct add_volatile_impl {
  using type = T;
};
template <class T>
struct add_volatile_impl<T, true> {
  using type = volatile T;
};
template <class T>
struct add_volatile_impl<volatile T, true> {
  using type = volatile T;
};

}
template <class T>
struct add_volatile {
  using type = typename inner::add_volatile_impl<T>::type;
};
template <class T>
using add_volatile_t = typename add_volatile<T>::type;

template <class T>
struct add_cv {
  using type = typename add_const<typename add_volatile<T>::type>::type;
};
template <class T>
using add_cv_t = typename add_cv<T>::type;

// reference modifications

template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

namespace inner {

template <class T, bool = is_referenceable<T>::value>
struct add_lvalue_reference_impl {
  using type = T;
};
template <class T>
struct add_lvalue_reference_impl<T, true> {
  using type = T &;
};

}
template <class T>
struct add_lvalue_reference {
  using type = typename inner::add_lvalue_reference_impl<T>::type;
};
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

namespace inner {

template <class T, bool = is_referenceable<T>::value>
struct add_rvalue_reference_impl {
  using type = T;
};
template <class T>
struct add_rvalue_reference_impl<T, true> {
  using type = T &&;
};

}
template <class T>
struct add_rvalue_reference {
  using type = typename inner::add_rvalue_reference_impl<T>::type;
};
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// sign modifications

template <class T>
struct make_signed {
  using type = typename conditional
    <is_unsigned<T>::value,
     typename integral_traits<T>::change_signedness, T>::type;
};
template <>
struct make_signed<char> {
  using type = signed char;
};
template <class T>
struct make_signed<const T> {
  using type = const typename make_signed<T>::type;
};
template <class T>
struct make_signed<volatile T> {
  using type = volatile typename make_signed<T>::type;
};
template <class T>
struct make_signed<const volatile T> {
  using type = const volatile typename make_signed<T>::type;
};
template <class T>
using make_signed_t = typename make_signed<T>::type;

template <class T>
struct make_unsigned {
  using type = typename conditional
    <is_signed<T>::value,
     typename integral_traits<T>::change_signedness, T>::type;
};
template <>
struct make_unsigned<char> {
  using type = unsigned char;
};
template <>
struct make_unsigned<char8_t> {
  using type = unsigned char;
};
template <>
struct make_unsigned<char16_t> {
  using type = uint_least16_t;
};
template <>
struct make_unsigned<char32_t> {
  using type = uint_least32_t;
};
template <class T>
struct make_unsigned<const T> {
  using type = const typename make_unsigned<T>::type;
};
template <class T>
struct make_unsigned<volatile T> {
  using type = volatile typename make_unsigned<T>::type;
};
template <class T>
struct make_unsigned<const volatile T> {
  using type = const volatile typename make_unsigned<T>::type;
};
template <class T>
using make_unsigned_t = typename make_unsigned<T>::type;

// array modifications

template <class T>
struct remove_extent {
  using type = T;
};
template <class T>
struct remove_extent<T []> {
  using type = T;
};
template <class T, size_t N>
struct remove_extent<T [N]> {
  using type = T;
};
template <class T>
using remove_extent_t = typename remove_extent<T>::type;

template <class T>
struct remove_all_extents {
  using type = T;
};
template <class T>
struct remove_all_extents<T []> : remove_all_extents<T> {};
template <class T, size_t N>
struct remove_all_extents<T [N]> : remove_all_extents<T> {};
template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

// pointer modifications

template <class T>
struct remove_pointer {
  using type = T;
};
template <class T>
struct remove_pointer<T *> {
  using type = T;
};
template <class T>
struct remove_pointer<T *const> {
  using type = T;
};
template <class T>
struct remove_pointer<T *volatile> {
  using type = T;
};
template <class T>
struct remove_pointer<T *const volatile> {
  using type = T;
};
template <class T>
using remove_pointer_t = typename remove_pointer<T>::type;

namespace inner {

template <class T, bool = is_referenceable<T>::value || is_void<T>::value>
struct add_pointer_impl {
  using type = T;
};
template <class T>
struct add_pointer_impl<T, true> {
  using type = typename remove_reference<T>::type *;
};

}
template <class T>
struct add_pointer {
  using type = typename inner::add_pointer_impl<T>::type;
};
template <class T>
using add_pointer_t = typename add_pointer<T>::type;

// other transformations

template <class T>
struct type_identity {
  using type = T;
};
template< class T >
using type_identity_t = typename type_identity<T>::type;

template <class T>
struct remove_cvref {
  using type = remove_cv_t<remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <class T>
struct decay {
  using type = conditional_t
    <is_array_v<remove_reference_t<T>>,
     add_pointer_t<remove_extent_t<remove_reference_t<T>>>,
     conditional_t
     <is_function_v<remove_reference_t<T>>,
      add_pointer_t<remove_reference_t<T>>,
      remove_cv_t<remove_reference_t<T>>>>;
};
template <class T>
using decay_t = typename decay<T>::type;

template <bool Y, class T>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};

template <bool Y, class T, class U>
struct conditional {
  using type = T;
};
template <class T, class U>
struct conditional<false, T, U> {
  using type = U;
};

namespace inner {

template <class X>
using const_reference = add_lvalue_reference_t<const remove_reference_t<X>>;
template <class X, class Y>
using conditional_operator_result_type
  = decltype(false ? declval<X (&)()>()() : declval<Y (&)()>()());

template <class D1, class D2>
struct common_type_impl2 {};
template <class D1, class D2>
requires requires {
  typename conditional_operator_result_type
    <const_reference<D1>, const_reference<D2>>;
}
struct common_type_impl2<D1, D2> {
  using type = decay_t<conditional_operator_result_type
                       <const_reference<D1>, const_reference<D2>>>;
};

template <class D1, class D2>
struct common_type_impl : common_type_impl2<D1, D2> {};
template <class D1, class D2>
requires requires {
  typename decay_t<decltype(false ? declval<D1>() : declval<D2>())>;
}
struct common_type_impl<D1, D2> {
  using type = decay_t<decltype(false ? declval<D1>() : declval<D2>())>;
};

}
template <class...S>
struct common_type {};
template <>
struct common_type<> {};
template <class T>
struct common_type<T> : common_type<T, T> {};
template <class T1, class T2>
requires is_same_v<T1, decay_t<T1>> && is_same_v<T2, decay_t<T2>>
struct common_type<T1, T2> : inner::common_type_impl2<T1, T2> {};
template <class T1, class T2>
struct common_type<T1, T2> : common_type<decay_t<T1>, decay_t<T2>> {};
template <class T1, class T2, class...S>
struct common_type<T1, T2, S...>
  : compile_time_acc<common_type, T1, T2, S...> {};
template <class...S>
using common_type_t = typename common_type<S...>::type;

template <class T, class U,
          template <class> class TQUAL,
          template <class> class UQUAL>
struct basic_common_reference {};
template <class...S>
struct common_reference {};
namespace inner {

template <class X>
using cref_t = add_lvalue_reference_t<add_const_t<remove_reference_t<X>>>;

template <class FROM, class TO>
struct copycv {
  using type = TO;
};
template <class FROM, class TO>
struct copycv<const FROM, TO> {
  using type = add_const_t<TO>;
};
template <class FROM, class TO>
struct copycv<volatile FROM, TO> {
  using type = add_volatile_t<TO>;
};
template <class FROM, class TO>
struct copycv<const volatile FROM, TO> {
  using type = add_cv_t<TO>;
};
template <class FROM, class TO>
using copycv_t = typename copycv<FROM, TO>::type;

template <class FROM, class TO>
struct copycvref {
  using type = TO;
};
template <class FROM, class TO>
struct copycvref<const FROM, TO> {
  using type = add_const_t<TO>;
};
template <class FROM, class TO>
struct copycvref<volatile FROM, TO> {
  using type = add_volatile_t<TO>;
};
template <class FROM, class TO>
struct copycvref<const volatile FROM, TO> {
  using type = add_cv_t<TO>;
};
template <class FROM, class TO>
struct copycvref<FROM &, TO> {
  using type = add_lvalue_reference_t<TO>;
};
template <class FROM, class TO>
struct copycvref<const FROM &, TO> {
  using type = add_lvalue_reference_t<add_const_t<TO>>;
};
template <class FROM, class TO>
struct copycvref<volatile FROM &, TO> {
  using type = add_lvalue_reference_t<add_volatile_t<TO>>;
};
template <class FROM, class TO>
struct copycvref<const volatile FROM &, TO> {
  using type = add_lvalue_reference_t<add_cv_t<TO>>;
};
template <class FROM, class TO>
struct copycvref<FROM &&, TO> {
  using type = add_rvalue_reference_t<TO>;
};
template <class FROM, class TO>
struct copycvref<const FROM &&, TO> {
  using type = add_rvalue_reference_t<add_const_t<TO>>;
};
template <class FROM, class TO>
struct copycvref<volatile FROM &&, TO> {
  using type = add_rvalue_reference_t<add_volatile_t<TO>>;
};
template <class FROM, class TO>
struct copycvref<const volatile FROM &&, TO> {
  using type = add_rvalue_reference_t<add_cv_t<TO>>;
};
template <class FROM, class TO>
using copycvref_t = typename copycvref<FROM, TO>::type;

template <class A>
struct xref {
  template <class U> // U is non-reference cv-unqualified type
  using tmpl = copycvref_t<A, U>;
};

template <class X, class Y>
struct cond_res {
  template <class XX, class YY>
  static type_tag
  <decltype(false ? declval<XX (&)()>()() : declval<YY (&)()>()())>
  f(type_pack<XX, YY>);
  static type_tag<disable> f(...);

  using type = typename decltype(f(declval<type_pack<X, Y>>()))::type;
};
template <class X, class Y>
using cond_res_t = typename cond_res<X, Y>::type;

template <class A, class B>
struct common_ref {
  using type = disable;
};
template <class X, class Y>
struct common_ref<X &, Y &> {
  using cond_res_type = cond_res_t<copycv_t<X, Y> &, copycv_t<Y, X> &>;
  using type = conditional_t<!is_same_v<cond_res_type, disable>
                             && is_reference_v<cond_res_type>,
                             cond_res_type, disable>;
};
template <class X, class Y>
struct common_ref<X &&, Y &&> {
  using A = X &&;
  using B = Y &&;
  using t = typename common_ref<X &, Y &>::type;
  using C = remove_reference_t<t> &&;
  using type = conditional_t<!is_same_v<t, disable>
                             && is_convertible_v<A, C>
                             && is_convertible_v<B, C>,
                             C, disable>;
};
template <class X, class Y>
struct common_ref<X &&, Y &> {
  using A = X &&;
  using B = Y &;
  using D = typename common_ref<const X &, Y &>::type;
  using type = conditional_t<!is_same_v<D, disable> && is_convertible_v<A, D>,
                             D, disable>;
};
template <class X, class Y>
struct common_ref<X &, Y &&> : common_ref<Y &&, X &> {};
template <class A, class B>
using common_ref_t = typename common_ref<A, B>::type;

template <class T1, class T2>
struct common_reference_impl4 {};
template <class T1, class T2>
requires requires {typename common_type_t<T1, T2>;}
struct common_reference_impl4<T1, T2> {
  using type = common_type_t<T1, T2>;
};

template <class T1, class T2>
struct common_reference_impl3 : common_reference_impl4<T1, T2> {};
template <class T1, class T2>
requires requires {
  requires (!is_same_v<cond_res_t<T1, T2>, disable>);
}
struct common_reference_impl3<T1, T2> {
  using type = cond_res_t<T1, T2>;
};

template <class T1, class T2>
struct common_reference_impl2 : common_reference_impl3<T1, T2> {};
template <class T1, class T2>
requires requires {
  typename basic_common_reference
    <remove_cvref_t<T1>, remove_cvref_t<T2>,
     xref<T1>::template tmpl, xref<T2>::template tmpl>::type;
}
struct common_reference_impl2<T1, T2> {
  using type = typename basic_common_reference
    <remove_cvref_t<T1>, remove_cvref_t<T2>,
     xref<T1>::template tmpl, xref<T2>::template tmpl>::type;
};

template <class T1, class T2, bool = is_reference_v<T1> && is_reference_v<T2>
          && !is_same_v<common_ref_t<T1, T2>, disable>>
struct common_reference_impl {
  using type = common_ref_t<T1, T2>;
};
template <class T1, class T2>
struct common_reference_impl<T1, T2, false> : common_reference_impl2<T1, T2> {};

}
template <>
struct common_reference<> {};
template <class T>
struct common_reference<T> {
  using type = T;
};
template <class T1, class T2>
struct common_reference<T1, T2> : inner::common_reference_impl<T1, T2> {};
template <class T1, class T2, class...S>
struct common_reference<T1, T2, S...>
  : compile_time_acc<common_reference, T1, T2, S...> {};
template <class...S>
using common_reference_t = typename common_reference<S...>::type;

template <class T, bool = has_member_type_type_v<std::underlying_type<T>>>
struct underlying_type {
  using type = typename std::underlying_type<T>::type;
};
template <class T>
struct underlying_type<T, false> {};
template <class T>
using underlying_type_t = typename underlying_type<T>::type;

template <class...>
using void_t = void;

// todo:
//   // member relationships
//   is_pointer_interconvertible_with_class(M S::*) noexcept
//   is_corresponding_member(M S::*, M2, S2::*) noexcept

namespace inner {

struct check_uses_allocator {
  template <class T, class ALLOC>
  static enable_if_t<is_convertible_v<ALLOC, typename T::allocator_type>>
  f(type_pack<T, ALLOC>);
};

}
template <class T, class ALLOC>
struct uses_allocator
  : f_is_well_formed<inner::check_uses_allocator, T, ALLOC> {};
template <class T, class ALLOC>
inline constexpr bool uses_allocator_v = uses_allocator<T, ALLOC>::value;

// non-std

template <class...S>
struct max_align_of_types {
  static constexpr size_t value = 1;
};
template <class X, class...S>
struct max_align_of_types<X, S...> {
  static constexpr size_t value
    = max_align_of_types<S...>::value > alignof(X)
    ? max_align_of_types<S...>::value : alignof(X);
};
template <class...S>
inline constexpr size_t max_align_of_types_v = max_align_of_types<S...>::value;

template <class...S>
struct max_size_of_types {
  static constexpr size_t value = 0;
};
template <class X, class...S>
struct max_size_of_types<X, S...> {
  static constexpr size_t value
    = max_size_of_types<S...>::value > sizeof(X)
    ? max_size_of_types<S...>::value : sizeof(X);
};
template <class...S>
inline constexpr size_t max_size_of_types_v = max_size_of_types<S...>::value;

template <class T, class...>
using type_t = T;

namespace inner {

struct check_is_implicitly_constructible {
  template <class T>
  static void helper_f(const T &);

  template <class T, class...S,
            class = enable_if_t<is_constructible_v<T, S...>>>
  static decltype(helper_f<T>({declval<S>()...})) f(type_pack<T, S...>);
};

}
template <class T, class...S>
struct is_implicitly_constructible
  : f_is_well_formed<inner::check_is_implicitly_constructible, T, S...> {};
template <class T, class...S>
inline constexpr bool is_implicitly_constructible_v
  = is_implicitly_constructible<T, S...>::value;
template <class T>
struct is_implicitly_default_constructible
  : is_implicitly_constructible<T> {};
template <class T, class...S>
inline constexpr bool is_implicitly_default_constructible_v
  = is_implicitly_default_constructible<T, S...>::value;

template <class T>
struct is_trivial_empty
  : bool_constant<is_trivial_v<T> && is_empty_v<T>> {};
template <class T>
inline constexpr bool is_trivial_empty_v = is_trivial_empty<T>::value;

template <class FROM, class TO>
struct copy_const {
  using type = TO;
};
template <class FROM, class TO>
struct copy_const<const FROM, TO> {
  using type = add_const_t<TO>;
};
template <class FROM, class TO>
using copy_const_t = typename copy_const<FROM, TO>::type;

template <class FROM, class TO>
struct copy_volatile {
  using type = TO;
};
template <class FROM, class TO>
struct copy_volatile<volatile FROM, TO> {
  using type = add_volatile_t<TO>;
};
template <class FROM, class TO>
using copy_volatile_t = typename copy_volatile<FROM, TO>::type;

template <class FROM, class TO>
struct copy_lvalue_reference {
  using type = TO;
};
template <class FROM, class TO>
struct copy_lvalue_reference<FROM &, TO> {
  using type = add_lvalue_reference_t<TO>;
};
template <class FROM, class TO>
using copy_lvalue_reference_t = typename copy_lvalue_reference<FROM, TO>::type;

template <class FROM, class TO>
struct copy_rvalue_reference {
  using type = TO;
};
template <class FROM, class TO>
struct copy_rvalue_reference<FROM &&, TO> {
  using type = add_rvalue_reference_t<TO>;
};
template <class FROM, class TO>
using copy_rvalue_reference_t = typename copy_rvalue_reference<FROM, TO>::type;

template <class FROM, class TO>
struct copy_reference {
  using type = TO;
};
template <class FROM, class TO>
struct copy_reference<FROM &, TO> {
  using type = add_lvalue_reference_t<TO>;
};
template <class FROM, class TO>
struct copy_reference<FROM &&, TO> {
  using type = add_rvalue_reference_t<TO>;
};
template <class FROM, class TO>
using copy_reference_t = typename copy_reference<FROM, TO>::type;

template <class FROM, class TO>
struct copy_cv {
  using type = copy_volatile_t<FROM, copy_const_t<FROM, TO>>;
};
template <class FROM, class TO>
using copy_cv_t = typename copy_cv<FROM, TO>::type;

template <class FROM, class TO>
struct copy_cvref {
  using type = copy_reference_t<FROM, copy_cv_t<remove_reference_t<FROM>, TO>>;
};
template <class FROM, class TO>
using copy_cvref_t = typename copy_cvref<FROM, TO>::type;

template <class T>
struct is_nothrow_movable
  : bool_constant<is_nothrow_move_constructible_v<T>
                  && is_nothrow_move_assignable_v<T>> {};
template <class T>
inline constexpr bool is_nothrow_movable_v = is_nothrow_movable<T>::value;

template <class T>
struct is_nothrow_copyable
  : bool_constant<is_nothrow_move_constructible_v<T>
                  && is_nothrow_move_assignable_v<T>
                  && is_nothrow_copy_constructible_v<T>
                  && is_nothrow_copy_assignable_v<T>> {};
template <class T>
inline constexpr bool is_nothrow_copyable_v = is_nothrow_copyable<T>::value;

template <class Y, class T>
struct is_compatible_pointer_with;
template <class Y, class T>
struct is_compatible_pointer_with<Y *, T *>
  : is_convertible<Y *, T *> {};
template <class Y, class T>
struct is_compatible_pointer_with<Y *, T []>
  : is_convertible<Y (*)[], T (*)[]> {};
template <class Y, class T, size_t N>
struct is_compatible_pointer_with<Y *, T [N]>
  : is_convertible<Y (*)[N], T (*)[N]> {};
template <class Y, class T>
inline constexpr bool is_compatible_pointer_with_v
  = is_compatible_pointer_with<Y, T>::value;

}

// basic utility functions
namespace re {

template <class T>
struct fo_forward {
  constexpr T &&operator ()(remove_reference_t<T> &x) const noexcept {
    return static_cast<T &&>(x);
  }
  constexpr T &&operator ()(remove_reference_t<T> &&x) const noexcept
    requires (!is_lvalue_reference_v<T>) {
    return static_cast<T &&>(x);
  }
};
template <class T>
inline constexpr fo_forward<T> forward{};

struct fo_move {
  template <class T>
  constexpr remove_reference_t<T> &&operator ()(T &&x) const noexcept {
    return static_cast<remove_reference_t<T> &&>(x);
  }
  template <class A, class B>
  auto operator ()(A &&, B) const;
};
inline constexpr fo_move move{};

struct fo_move_if_noexcept {
  template <class T>
  constexpr conditional_t<!is_nothrow_move_constructible_v<T>
                          && is_copy_constructible_v<T>,
                          const T &, T &&>
  operator ()(T &x) const noexcept {
    return move(x);
  }
};
inline constexpr fo_move_if_noexcept move_if_noexcept{};

struct fo_exchange {
  template <class T, class U>
  constexpr T operator ()(T &obj, U &&new_val) const
    noexcept(is_nothrow_move_constructible_v<T>
             && is_nothrow_assignable_v<T &, U>) {
    T old_val = move(obj);
    obj = forward<U>(new_val);
    return old_val;
  }
};
inline constexpr fo_exchange exchange{};

struct fo_as_const {
  template <class T>
  constexpr add_const_t<T> &operator ()(T &x) const noexcept {
    return x;
  }
};
inline constexpr fo_as_const as_const{};

struct fo_cmp_equal {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    if constexpr (is_signed_v<T> == is_signed_v<U>) {
      return t == u;
    }
    else if constexpr (is_signed_v<T>) {
      using UT = make_unsigned_t<T>;
      return t < 0 ? false : UT(t) == u;
    }
    else {
      using UU = make_unsigned_t<U>;
      return u < 0 ? false : t == UU(u);
    }
  }
};
inline constexpr fo_cmp_equal cmp_equal{};
struct fo_cmp_not_equal {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    return !cmp_equal(t, u);
  }
};
inline constexpr fo_cmp_not_equal cmp_not_equal{};
struct fo_cmp_less {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    if constexpr (is_signed_v<T> == is_signed_v<U>) {
      return t < u;
    }
    else if constexpr (is_signed_v<T>) {
      using UT = make_unsigned_t<T>;
      return t < 0 ? true : UT(t) < u;
    }
    else {
      using UU = make_unsigned_t<U>;
      return u < 0 ? false : t < UU(u);
    }
  }
};
inline constexpr fo_cmp_less cmp_less{};
struct fo_cmp_greater {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    return cmp_less(u, t);
  }
};
inline constexpr fo_cmp_greater cmp_greater{};
struct fo_cmp_less_equal {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    return !cmp_greater(t, u);
  }
};
inline constexpr fo_cmp_less_equal cmp_less_equal{};
struct fo_cmp_greater_equal {
  template <class T, class U>
  constexpr bool operator ()(T t, U u) const noexcept
    requires is_integral_v<T> && is_integral_v<U> {
    return !cmp_less(t, u);
  }
};
inline constexpr fo_cmp_greater_equal cmp_greater_equal{};

template <class>
struct numeric_limits;
template <class R>
struct fo_in_range {
  template <class T>
  constexpr bool operator ()(T t) const noexcept
    requires is_integral_v<R> && is_integral_v<T> {
    return cmp_greater_equal(t, numeric_limits<R>::min())
      && cmp_less_equal(t, numeric_limits<R>::max());
  }
};
template <class R>
inline constexpr fo_in_range<R> in_range{};

struct fo_to_underlying {
  template <class T>
  constexpr underlying_type_t<T> operator ()(T e) const noexcept {
    return static_cast<underlying_type_t<T>>(e);
  }
};
inline constexpr fo_to_underlying to_underlying{};

// todo: unreachable

namespace inner {

struct addressof_holderplace_t {};
void operator &(addressof_holderplace_t &); // make msvc happy

template <class T>
concept has_in_class_addressof_operator = requires(T &x) {
  x.operator &();
};
template <class T>
concept has_out_class_addressof_operator = requires(T &x) {
  operator &(x);
};
template <class T>
concept addressof_overloaded
  = inner::has_out_class_addressof_operator<T>
  || inner::has_in_class_addressof_operator<T>;
template <class T>
concept addressof_invocable = requires(T &x) {
  &x;
};

}
struct fo_addressof {
  template <class T>
  constexpr T *operator ()(T &x) const noexcept
    requires inner::addressof_invocable<T>
    && (!inner::addressof_overloaded<T>) {
    return &x;
  }
  template <class T>
  T *operator ()(T &x) const noexcept {
    return reinterpret_cast<T *>
      (&const_cast<char &>(reinterpret_cast<const volatile char &>(x)));
  }

  template <class T>
  const T *addressof(const T &&) const = delete;
};
inline constexpr fo_addressof addressof{};

struct fo_align {
  // update p and decrease n by space for alignment
  // return p
  // Note: it can be called repeatedly with possibly different alignment and
  //   size arguments for the same buffer
  void *operator ()
    (size_t a, size_t sz, void *&p, size_t &n) const noexcept {
    using bp_t = byte *;
    const size_t pos = static_cast<size_t>(static_cast<bp_t>(p)
                                           - bp_t(nullptr));
    void *ret = nullptr;
    if (n >= sz) {
      if (const size_t rem = pos % a; rem != 0) {
        const size_t offset = a - rem;
        if (n - sz >= offset) {
          p = static_cast<bp_t>(p) + offset;
          n -= offset;
          ret = p;
        }
      }
      else
        ret = p;
    }
    return ret;
  }
};
inline constexpr fo_align align{};

}

// pointer_traits
namespace re {

namespace inner {

struct get_pointer_element_type_0 {
  template <class T>
  static type_tag<typename T::element_type> f(type_pack<T>);
};
template <class T,
          bool = f_is_well_formed_v<get_pointer_element_type_0, T>>
struct get_pointer_element_type {
  // using type = void;
};
template <class T>
struct get_pointer_element_type<T, true> {
  using type = typename T::element_type;
};
template <template <class...> class T, class U, class...S>
struct get_pointer_element_type<T<U, S...>, false> {
  using type = U;
};

struct get_pointer_difference_type {
  template <class T>
  static type_tag<typename T::difference_type> f(type_pack<T>);
};

struct get_pointer_rebind_type_0 {
  template <class P, class T>
  static type_tag<typename P::template rebind<T>> f(type_pack<P, T>);
};
template <class P, class T,
          bool = f_is_well_formed_v<get_pointer_rebind_type_0, P, T>>
struct get_pointer_rebind_type {};
template <class P, class T>
struct get_pointer_rebind_type<P, T, true> {
  using type = typename P::template rebind<T>;
};
template <template <class, class...> class T, class U, class...S, class V>
struct get_pointer_rebind_type<T<U, S...>, V, false> {
  using type = T<V, S...>;
};

template <class P>
concept well_formed_pointer_traits = requires {
  typename get_pointer_element_type<P>::type;
};

}
template <class P>
struct pointer_traits;
template <class P>
requires inner::well_formed_pointer_traits<P>
struct pointer_traits<P> {
  using pointer = P;
  using element_type = typename inner::get_pointer_element_type<P>::type;
  using difference_type = typename return_type_of_f_or
    <inner::get_pointer_difference_type, ptrdiff_t, P>::type;
  template <class U>
  using rebind = typename inner::get_pointer_rebind_type<P, U>::type;

  template <class T>
  static pointer pointer_to(T &&x) requires is_void_v<element_type> {
    return P::pointer_to(forward<T>(x));
  }
  template <class E = element_type>
  static constexpr pointer pointer_to(E &x) requires (!is_void_v<E>) {
    return P::pointer_to(x);
  }
};
template <class T>
struct pointer_traits<T *> {
  using pointer = T *;
  using element_type = T;
  using difference_type = ptrdiff_t;
  template <class U>
  using rebind = U *;

  template <class U>
  static pointer pointer_to(U &&x) noexcept
    requires is_void_v<element_type> {
    return addressof(forward<U>(x));
  }
  template <class E = element_type>
  static constexpr pointer pointer_to(E &x)
    noexcept requires (!is_void_v<E>) {
    return addressof(x);
  }
};

template <class P>
using pointer_element_t = typename pointer_traits<P>::element_type;
template <class P>
using pointer_difference_t = typename pointer_traits<P>::difference_type;
template <class P, class X>
using pointer_rebind_t = typename pointer_traits<P>::template rebind<X>;

template <class PTR>
struct fo_pointer_to {
  template <class T>
  typename pointer_traits<PTR>::template rebind<remove_reference_t<T>>
  operator ()(T &&x) const {
    return pointer_traits<typename pointer_traits<PTR>
                          ::template rebind<remove_reference_t<T>>>
      ::pointer_to(x);
  }
};
template <class PTR>
inline constexpr fo_pointer_to<PTR> pointer_to{};

struct fo_to_address {
  template <class T>
  constexpr T *operator ()(T *p) const noexcept {
    return p;
  }
  template <class T>
  constexpr auto operator ()(const T &p) const noexcept
    ->decltype(declval<fo_to_address &>()(p.operator ->())) {
    return (*this)(p.operator ->());
  }
  template <class T>
  constexpr auto operator ()(const T &p) const noexcept
    requires requires {
      typename pointer_traits<T>;
      pointer_traits<T>::to_address(p);
    } {
    return pointer_traits<T>::to_address(p);
  }
};
inline constexpr fo_to_address to_address{};

template <class T>
concept can_apply_to_address = requires(T &&x) {
  requires is_referenceable_v<T>;
  to_address(forward<T>(x));
};

}

// reference_wrapper and invoke
namespace re {

template <class>
class reference_wrapper;
template <class>
struct is_reference_wrapper : false_type {};
template <class T>
struct is_reference_wrapper<reference_wrapper<T>> : true_type {};
template <class T>
struct is_reference_wrapper<const reference_wrapper<T>> : true_type {};
template <class T>
struct is_reference_wrapper<volatile reference_wrapper<T>> : true_type {};
template <class T>
struct is_reference_wrapper<const volatile reference_wrapper<T>> : true_type {};
template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

template <class T>
struct unwrap_reference {
  using type = T;
};
template <class T>
struct unwrap_reference<reference_wrapper<T>> {
  using type = T &;
};
template <class T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

template <class T>
struct unwrap_ref_decay : unwrap_reference<decay_t<T>> {};
template <class T>
using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;

namespace inner::fns {

template <class F, class T, class X, class...S>
constexpr
enable_if_t<is_function_v<F> && is_base_of_v<T, remove_reference_t<X>>,
            decltype((declval<X>().*declval<F T::*>())(declval<S>()...))>
invoke_impl(F T::*f, X &&x, S &&...s)
  noexcept(noexcept((forward<X>(x).*f)(forward<S>(s)...))) {
  return (forward<X>(x).*f)(forward<S>(s)...);
}
template <class F, class T, class X, class...S>
constexpr
enable_if_t<is_function_v<F> && !is_base_of_v<T, decay_t<X>>
            && is_reference_wrapper_v<decay_t<X>>,
            decltype(((declval<X>().get()).*declval<F T::*>())
                     (declval<S>()...))>
invoke_impl(F T::*f, X &&x, S &&...s)
  noexcept(noexcept(((x.get()).*f)(forward<S>(s)...))) {
  return ((x.get()).*f)(forward<S>(s)...);
}
template <class F, class T, class X, class...S>
constexpr
enable_if_t<is_function_v<F> && !is_base_of_v<T, decay_t<X>>
            && !is_reference_wrapper_v<decay_t<X>>,
            decltype(((*declval<X>()).*declval<F T::*>())(declval<S>()...))>
invoke_impl(F T::*f, X &&x, S &&...s)
  noexcept(noexcept(((*forward<X>(x)).*f)(forward<S>(s)...))) {
  return ((*forward<X>(x)).*f)(forward<S>(s)...);
}
template <class F, class T, class X, class...S>
constexpr
enable_if_t<!is_function_v<F> && is_base_of_v<T, decay_t<X>>,
            decltype(declval<X>().*declval<F T::*>())>
invoke_impl(F T::*f, X &&x) noexcept(noexcept((forward<X>(x)).*f)) {
  return forward<X>(x).*f;
}
template <class F, class T, class X, class...S>
constexpr
enable_if_t<!is_function_v<F> && !is_base_of_v<T, decay_t<X>>
            && is_reference_wrapper_v<decay_t<X>>,
            decltype(declval<X>().get().*declval<F T::*>())>
invoke_impl(F T::*f, X &&x) noexcept(noexcept(x.get().*f)) {
  return x.get().*f;
}
template <class F, class T, class X, class...S>
constexpr
enable_if_t<!is_function_v<F> && !is_base_of_v<T, decay_t<X>>
            && !is_reference_wrapper_v<decay_t<X>>,
            decltype((*declval<X>()).*declval<F T::*>())>
invoke_impl(F T::*f, X &&x) noexcept(noexcept((*forward<X>(x)).*f)) {
  return (*forward<X>(x)).*f;
}
template <class F, class...S>
constexpr
enable_if_t<!is_member_pointer_v<decay_t<F>>,
            decltype(declval<F>()(declval<S>()...))>
invoke_impl(F &&f, S &&...s)
  noexcept(noexcept(forward<F>(f)(forward<S>(s)...))) {
  return forward<F>(f)(forward<S>(s)...);
}

template <class R, class F, class...S, class = enable_if_t<is_void_v<R>>>
constexpr auto invoke_r_impl(F &&f, S &&...s)
  noexcept(noexcept(inner::fns::invoke_impl(forward<F>(f), forward<S>(s)...)))
  ->decltype(static_cast<void>
             (inner::fns::invoke_impl(forward<F>(f), forward<S>(s)...))) {
  return static_cast<void>
    (inner::fns::invoke_impl(forward<F>(f), forward<S>(s)...));
}
template <class R, class F, class...S>
constexpr
enable_if_t
<!is_void_v<R> && is_convertible_v
 <decltype(inner::fns::invoke_impl(declval<F>(), declval<S>()...)), R>, R>
invoke_r_impl(F &&f, S &&...s)
  noexcept(noexcept(static_cast<R>(inner::fns::invoke_impl
                                   (forward<F>(f), forward<S>(s)...)))) {
  return inner::fns::invoke_impl(forward<F>(f), forward<S>(s)...);
}

}

namespace inner {

struct check_is_invocable {
  template <class F, class...S>
  static decltype(inner::fns::invoke_impl(declval<F>(), declval<S>()...))
  f(type_pack<F, S...>);
};

}
template <class F, class...S>
struct is_invocable
  : f_is_well_formed<inner::check_is_invocable, F, S...> {};
template <class F, class...S>
constexpr bool is_invocable_v = is_invocable<F, S...>::value;

namespace inner {

struct check_is_invocable_r {
  template <class R, class F, class...S>
  static decltype(inner::fns::invoke_r_impl<R>(declval<F>(), declval<S>()...))
  f(type_pack<R, F, S...>);
};

}
template <class R, class F, class...S>
struct is_invocable_r
  : f_is_well_formed<inner::check_is_invocable_r, R, F, S...> {};
template <class F, class...S>
constexpr bool is_invocable_r_v = is_invocable_r<F, S...>::value;

namespace inner {

template <bool INVOCABLE, class F, class...S>
struct invoke_result_impl {};
template <class F, class...S>
struct invoke_result_impl<true, F, S...> {
  using type = decltype(inner::fns::invoke_impl(declval<F>(), declval<S>()...));
};

}
template <class F, class...S>
struct invoke_result
  : inner::invoke_result_impl<is_invocable_v<F, S...>, F, S...> {};
template <class F, class...S>
using invoke_result_t = typename invoke_result<F, S...>::type;

namespace inner {

template <bool INVOCABLE, class F, class...S>
struct is_nothrow_invocable_impl {
  static constexpr bool value = false;
};
template <class F, class...S>
struct is_nothrow_invocable_impl<true, F, S...> {
  static constexpr bool value
    = noexcept(inner::fns::invoke_impl(declval<F>(), declval<S>()...))
    && (is_nothrow_move_constructible_v<invoke_result_t<F, S...>>
        || is_void_v<invoke_result_t<F, S...>>);
};

}
template <class F, class...S>
struct is_nothrow_invocable
  : bool_constant<inner::is_nothrow_invocable_impl
                  <is_invocable<F, S...>::value, F, S...>::value> {};
template <class F, class...S>
constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<F, S...>::value;

namespace inner {

template <bool INVOCABLE_R, class R, class F, class...S>
struct is_nothrow_invocable_r_impl {
  static constexpr bool value = false;
};
template <class R, class F, class...S>
struct is_nothrow_invocable_r_impl<true, R, F, S...> {
  static constexpr bool value
    = noexcept(inner::fns::invoke_r_impl<R>(declval<F>(), declval<S>()...));
};

}
template <class R, class F, class...S>
struct is_nothrow_invocable_r
  : bool_constant<inner::is_nothrow_invocable_r_impl
                  <is_invocable_r<R, F, S...>::value, R, F, S...>::value> {};
template <class R, class F, class...S>
inline constexpr bool is_nothrow_invocable_r_v
  = is_nothrow_invocable_r<R, F, S...>::value;

struct fo_invoke {
  template <class F, class...S>
  constexpr typename invoke_result<F, S...>::type operator ()(F &&f, S &&...s)
    const noexcept(is_nothrow_invocable<F, S...>::value) {
    return inner::fns::invoke_impl(forward<F>(f), forward<S>(s)...);
  }
};
inline constexpr fo_invoke invoke{};
namespace inner::fns {

template <class T, class F, size_t...I>
constexpr void type_pack_for_each_impl0(F f, index_sequence<I...>)
{
  invoke(f, type_tag<type_pack_element_t<I, T>>()...);
}
template <class T, class F, size_t...I>
constexpr bool type_pack_for_each_until_false_impl0(F f, index_sequence<I...>)
{
  return invoke(f, type_tag<type_pack_element_t<I, T>>()...);
}

}

template <class R>
struct fo_invoke_r {
  template <class F, class...S>
  constexpr decltype(auto) operator ()(F &&f, S &&...s)
    const noexcept(is_nothrow_invocable_r_v<R, F, S...>) {
    return inner::fns::invoke_r_impl<R>(forward<F>(f), forward<S>(s)...);
  }
};
template <class R>
inline constexpr fo_invoke_r<R> invoke_r{};

namespace inner {

template <class T>
struct refw_check_u {
  static void ff(T &) noexcept;
  static void ff(T &&) = delete;

  template <class U>
  static decltype(ff(declval<U>())) f(type_pack<U>);
};

}
template <class T>
class reference_wrapper {
  T *p;

  void f(T &) noexcept;
  void f(T &&) = delete;

public:
  using type = T;

  reference_wrapper() = delete;
  ~reference_wrapper() = default;
  reference_wrapper(const reference_wrapper &) = default;
  reference_wrapper &operator =(const reference_wrapper &) = default;
  reference_wrapper(reference_wrapper &&) = default;
  reference_wrapper &operator =(reference_wrapper &&) = default;

  template <class U>
  constexpr reference_wrapper(U &&x) noexcept(noexcept(f(declval<U>())))
    requires (!is_same_v<remove_cvref_t<U>, reference_wrapper>)
    && f_is_well_formed_v<inner::refw_check_u<T>, U> {
    T &r = forward<U>(x);
    p = addressof(r);
  }

  constexpr operator T &() const noexcept {
    return *p;
  }

  constexpr T &get() const noexcept {
    return *p;
  }

  template <class...S>
  constexpr invoke_result_t<T &, S...> operator()(S &&...s) const {
    return invoke(*p, forward<S>(s)...);
  }
};
template <class T>
reference_wrapper(T &)->reference_wrapper<T>;

template <class>
struct range_traits;
struct fo_ref {
  template <class T>
  constexpr reference_wrapper<T> operator ()(T &x) const noexcept {
    return reference_wrapper<T>(x);
  }
  template <class T>
  constexpr reference_wrapper<T>
  operator ()(reference_wrapper<T> x) const noexcept {
    return operator ()(x.get());
  }
  template <class T>
  void operator ()(const T &&) const = delete;

  template <class R>
  constexpr decltype(auto)
  operator ()(R &, typename range_traits<R>::difference_type) const;
  template <class R>
  constexpr decltype(auto)
  operator ()(const R &, typename range_traits<R>::difference_type) const;
};
inline constexpr fo_ref ref{};

struct fo_cref {
  template <class T>
  constexpr
  reference_wrapper<const T> operator ()(const T &x) const noexcept {
    return reference_wrapper<const T>(x);
  }
  template <class T>
  constexpr reference_wrapper<const T>
  operator ()(reference_wrapper<T> x) const noexcept {
    return x;
  }

  template <class T>
  void operator ()(const T &&) const = delete;
};
inline constexpr fo_cref cref{};

}

// language-related concepts
namespace re {

template <class A, class B>
concept same_as = is_same_v<A, B> && is_same_v<B, A>;

template <class DERIVED, class BASE>
concept derived_from = is_base_of_v<BASE, DERIVED>
  && is_convertible_v<const volatile DERIVED *, const volatile BASE *>;

template <class FROM, class TO>
concept convertible_to
  = is_convertible_v<FROM, TO> && requires {static_cast<TO>(declval<FROM>());};

template <class T, class U>
concept common_reference_with
  = same_as<common_reference_t<T, U>, common_reference_t<U, T>>
  && convertible_to<T, common_reference_t<T, U>>
  && convertible_to<U, common_reference_t<T, U>>;

template <class T, class U>
concept common_with
  = same_as<common_type_t<T, U>, common_type_t<U, T>>
  && requires {
    static_cast<common_type_t<T, U>>(declval<T>());
    static_cast<common_type_t<T, U>>(declval<U>());
  }
  && common_reference_with<add_lvalue_reference_t<const T>,
                           add_lvalue_reference_t<const U>>
  && common_reference_with<add_lvalue_reference_t<common_type_t<T, U>>,
                           common_reference_t<add_lvalue_reference_t<const T>,
                                              add_lvalue_reference_t<const U>>>;

template<class T>
concept integral = is_integral_v<T>;
template<class T>
concept signed_integral = integral<T> && is_signed_v<T>;
template<class T>
concept unsigned_integral = integral<T> && !signed_integral<T>;
template<class T>
concept floating_point = is_floating_point_v<T>;

template <class LHS, class RHS>
concept assignable_from
  = is_lvalue_reference_v<LHS>
  && common_reference_with<const remove_reference_t<LHS> &,
                           const remove_reference_t<RHS> &>
  && requires(LHS lhs, RHS &&rhs) {
    {lhs = static_cast<RHS &&>(rhs)}->same_as<LHS>;
  };

template <class T>
concept destructible = is_nothrow_destructible_v<T>;

template <class T, class...S>
concept constructible_from = destructible<T> && is_constructible_v<T, S...>;

template<class T>
concept default_initializable
  = constructible_from<T>
  && requires {T{};}
  && requires {::new(static_cast<void *>(nullptr)) T;};

template <class T>
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

template<class T>
concept copy_constructible = move_constructible<T>
  && constructible_from<T, T &> && convertible_to<T &, T>
  && constructible_from<T, const T &> && convertible_to<const T &, T>
  && constructible_from<T, const T> && convertible_to<const T, T>;

}
namespace re_adl::inner {

template <class T>
void swap(T &, T &) = delete;
template <class X, class Y>
concept ranges_swap_adl_version_capable =
  (re::is_class_v<re::remove_reference_t<X>>
   || re::is_enum_v<re::remove_reference_t<X>>
   || re::is_class_v<re::remove_reference_t<Y>>
   || re::is_enum_v<re::remove_reference_t<Y>>)
  && requires {swap(re::declval<X>(), re::declval<Y>());};
template <class X, class Y>
void do_ranges_swap_adl_version(X &&x, Y &&y)
  noexcept(noexcept((void)swap(re::declval<X>(), re::declval<Y>()))) {
  (void)swap(static_cast<X &&>(x), static_cast<Y &&>(y));
}

}
namespace re::inner {

template <class T, class U>
struct are_lvalues_of_equal_extent_array : false_type {};
template <class T, class U>
requires is_array_v<T> && (extent_v<T> == extent_v<U>)
struct are_lvalues_of_equal_extent_array<T &, U &> : true_type {};

template <class T, class U>
struct are_lvalues_of_same_type : false_type {};
template <class T>
struct are_lvalues_of_same_type<T &, T &> : true_type {};

}
namespace re::ranges {

struct fo_swap {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y)
    const noexcept(noexcept(re_adl::inner::do_ranges_swap_adl_version
                            (forward<X>(x), forward<Y>(y))))
    requires re_adl::inner::ranges_swap_adl_version_capable<X &&, Y &&> {
    re_adl::inner::do_ranges_swap_adl_version(forward<X>(x), forward<Y>(y));
  }
  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y)
    const noexcept(noexcept(declval<fo_swap>()(*x, *y)))
    requires (!re_adl::inner::ranges_swap_adl_version_capable<X &&, Y &&>)
    && inner::are_lvalues_of_equal_extent_array<X &&, Y &&>::value
    && requires {declval<fo_swap &>()(*x, *y);} {
    const auto x_end = addressof(*x) + extent_v<remove_reference_t<X>>;
    for (auto p = addressof(*x), pp = addressof(*y);
         p != x_end;) {
      fo_swap{}(*p, *pp);
      ++p;
      ++pp;
    }
  }
  template <class X, class Y>
  constexpr enable_if_t<inner::are_lvalues_of_same_type<X &&, Y &&>::value
                        && move_constructible<remove_reference_t<X>>
                        && assignable_from<remove_reference_t<X> &,
                                           remove_reference_t<X>>>
  operator ()(X &&x, Y &&y) const
    noexcept(is_nothrow_move_constructible_v<remove_reference_t<X>>
             && is_nothrow_move_assignable_v<remove_reference_t<X>>) {
    remove_reference_t<X> tmp(move(x));
    x = move(y);
    y = move(tmp);
  }
};
inline constexpr fo_swap swap{};

}
namespace re {

template <class T>
concept swappable = requires(T &a, T &b) {ranges::swap(a, b);};
template <class T, class U>
concept swappable_with = common_reference_with<T, U>
  && requires(T &&t, U &&u) {
    ranges::swap(static_cast<T &&>(t), static_cast<T &&>(t));
    ranges::swap(static_cast<U &&>(u), static_cast<U &&>(u));
    ranges::swap(static_cast<T &&>(t), static_cast<U &&>(u));
    ranges::swap(static_cast<U &&>(u), static_cast<T &&>(t));
  };

namespace inner {

template <class T>
concept boolean_testable = convertible_to<T, bool>
  && requires(T &&t) {{!static_cast<T &&>(t)}->convertible_to<bool>;};

template <class T, class U>
concept weakly_equality_comparable_with
  = requires(const remove_reference_t<T> &t,
             const remove_reference_t<U> &u) {
    {t == u}->inner::boolean_testable;
    {t != u}->inner::boolean_testable;
    {u == t}->inner::boolean_testable;
    {u != t}->inner::boolean_testable;
  };

}
template <class T>
concept equality_comparable = inner::weakly_equality_comparable_with<T, T>;
template <class T, class U>
concept equality_comparable_with
  = equality_comparable<T> && equality_comparable<U>
  && common_reference_with<const remove_reference_t<T> &,
                           const remove_reference_t<U> &>
  && equality_comparable<common_reference_t<const remove_reference_t<T> &,
                                            const remove_reference_t<U> &>>
  && inner::weakly_equality_comparable_with<T, U>;

namespace inner {

template <class T, class U>
concept partially_ordered_with
  = requires(const remove_reference_t<T> &t,
             const remove_reference_t<U> &u) {
    {t < u}->inner::boolean_testable;
    {t > u}->inner::boolean_testable;
    {t <= u}->inner::boolean_testable;
    {t >= u}->inner::boolean_testable;
    {u < t}->inner::boolean_testable;
    {u > t}->inner::boolean_testable;
    {u <= t}->inner::boolean_testable;
    {u >= t}->inner::boolean_testable;
  };

template <class T>
concept partially_ordered = inner::partially_ordered_with<T, T>;

}
template<class T>
concept totally_ordered = equality_comparable<T>
  && inner::partially_ordered_with<T, T>;
template <class T, class U>
concept totally_ordered_with
  = totally_ordered<T> && totally_ordered<U>
  && equality_comparable_with<T, U>
  && totally_ordered<common_reference_t<const remove_reference_t<T> &,
                                        const remove_reference_t<U> &>>
  && inner::partially_ordered_with<T, U>;

template <class T>
concept movable = is_object_v<T> && move_constructible<T>
  && assignable_from<T &, T> && swappable<T>;
template <class T>
concept copyable = copy_constructible<T> && movable<T>
  && assignable_from<T &, T &>
  && assignable_from<T &, const T &>
  && assignable_from<T &, const T>;
template <class T>
concept semiregular = copyable<T> && default_initializable<T>;
template <class T>
concept regular = semiregular<T> && equality_comparable<T>;

template <class F, class...S>
concept invocable = requires(F &&f, S &&...s) {
  invoke(forward<F>(f), forward<S>(s)...);
};
template <class F, class...S>
concept regular_invocable = invocable<F, S...>;
template <class F, class...S>
concept predicate = regular_invocable<F, S...>
  && inner::boolean_testable<invoke_result_t<F, S...>>;
template <class R, class T, class U>
concept relation = predicate<R, T, T> && predicate<R, U, U>
  && predicate<R, T, U> && predicate<R, U, T>;
template <class R, class T, class U>
concept equivalence_relation = relation<R, T, U>;
template <class R, class T, class U>
concept strict_weak_order = relation<R, T, U>;

}

// concept-constrained comparisons
namespace re::inner {

struct transparent_tag {};

}
namespace re::ranges {

struct equal_to {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires equality_comparable_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return forward<X>(x) == forward<Y>(y);
  }
};
struct not_equal_to {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires equality_comparable_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return !(forward<X>(x) == forward<Y>(y));
  }
};
struct less {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires totally_ordered_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return forward<X>(x) < forward<Y>(y);
  }
};
struct greater {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires totally_ordered_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return forward<Y>(y) < forward<X>(x);
  }
};
struct less_equal {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires totally_ordered_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return !(forward<Y>(y) < forward<X>(x));
  }
};
struct greater_equal {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  requires totally_ordered_with<X, Y>
  constexpr bool operator ()(X &&x, Y &&y) const {
    return !(forward<X>(x) < forward<Y>(y));
  }
};

}

// integral_traits
namespace re {

namespace inner {

template <class T>
struct integral_traits_base {
  using change_signedness = T;

  static constexpr bool is_specialized = false;
  static constexpr bool is_signed = false;
  static constexpr typename conditional
  <is_array<T>::value || is_function<T>::value, int, T>::type min()
    noexcept {
    return typename conditional
      <is_array<T>::value || is_function<T>::value, int, T>::type();
  }
  static constexpr typename conditional
  <is_array<T>::value || is_function<T>::value, int, T>::type max()
    noexcept {
    return typename conditional
      <is_array<T>::value || is_function<T>::value, int, T>::type();
  }
};
template <>
struct integral_traits_base<bool> {
  using change_signedness = bool;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr bool min() noexcept {
    return false;
  }
  static constexpr bool max() noexcept {
    return true;
  }
};
template <>
struct integral_traits_base<char> {
  using change_signedness
    = typename conditional
    <(CHAR_MIN < 0), unsigned char, signed char>::type;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = CHAR_MIN < 0;
  static constexpr char min() noexcept {
    return CHAR_MIN;
  }
  static constexpr char max() noexcept {
    return CHAR_MAX;
  }
};
template <>
struct integral_traits_base<signed char> {
  using change_signedness = unsigned char;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr signed char min() noexcept {
    return SCHAR_MIN;
  }
  static constexpr signed char max() noexcept {
    return SCHAR_MAX;
  }
};
template <>
struct integral_traits_base<unsigned char> {
  using change_signedness = signed char;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr unsigned char min() noexcept {
    return 0;
  }
  static constexpr unsigned char max() noexcept {
    return UCHAR_MAX;
  }
};

#define RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS(X, Y, Z)    \
  template <>                                           \
  struct integral_traits_base<X> {                      \
    using change_signedness = Z;                        \
                                                        \
    static constexpr bool is_specialized = true;        \
    static constexpr bool is_signed = true;             \
    static constexpr X min() noexcept {                 \
      return Y##_MIN;                                   \
    }                                                   \
    static constexpr X max() noexcept {                 \
      return Y##_MAX;                                   \
    }                                                   \
  }
RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS(short, SHRT, unsigned short);
RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS(int, INT, unsigned);
RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS(long, LONG, unsigned long);
RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS(long long, LLONG, unsigned long long);

#ifdef RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS
static_assert(false);
#endif
#define RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS(X, Y, Z)  \
  template <>                                           \
  struct integral_traits_base<X> {                      \
    using change_signedness = Z;                        \
                                                        \
    static constexpr bool is_specialized = true;        \
    static constexpr bool is_signed = false;            \
    static constexpr X min() noexcept {                 \
      return 0;                                         \
    }                                                   \
    static constexpr X max() noexcept {                 \
      return Y##_MAX;                                   \
    }                                                   \
  }
RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS(unsigned short, USHRT, short);
RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS(unsigned int, UINT, int);
RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS(unsigned long, ULONG, long);
RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS(unsigned long long, ULLONG, long long);

using get_unsigned_underlying_type_of_wchar_t
  = conditional_t<sizeof(wchar_t) == sizeof(unsigned short),
                  unsigned short,
                  conditional_t<sizeof(wchar_t) == sizeof(unsigned int),
                                unsigned int,
                                conditional_t<sizeof(wchar_t)
                                              == sizeof(unsigned long),
                                              unsigned long,
                                              unsigned long long>>>;

template <>
struct integral_traits_base<wchar_t> {
  using change_signedness
    = typename conditional
    <(WCHAR_MIN < 0),
     get_unsigned_underlying_type_of_wchar_t,
     typename integral_traits_base<get_unsigned_underlying_type_of_wchar_t>
     ::change_signedness>::type;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = WCHAR_MIN < 0;
  static constexpr wchar_t min() noexcept {
    return WCHAR_MIN;
  }
  static constexpr wchar_t max() noexcept {
    return WCHAR_MAX;
  }
};

template <size_t>
struct intt_useless_tag {};

}
template <class T>
struct integral_traits : inner::integral_traits_base<T> {};
template <class T>
struct integral_traits<const T> : integral_traits<T> {};
template <class T>
struct integral_traits<volatile T> : integral_traits<T> {};
template <class T>
struct integral_traits<const volatile T> : integral_traits<T> {};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_least8_t,
                                 char, signed char, short, int, long,
                                 long long>,
               inner::intt_useless_tag<0>, int_least8_t>> {
  using change_signedness = uint_least8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_least8_t min() noexcept {
    return INT_LEAST8_MIN;
  }
  static constexpr int_least8_t max() noexcept {
    return INT_LEAST8_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_least16_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t>,
               inner::intt_useless_tag<1>, int_least16_t>> {
  using change_signedness = uint_least16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_least16_t min() noexcept {
    return INT_LEAST16_MIN;
  }
  static constexpr int_least16_t max() noexcept {
    return INT_LEAST16_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_least32_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t>,
               inner::intt_useless_tag<2>, int_least32_t>> {
  using change_signedness = uint_least32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_least32_t min() noexcept {
    return INT_LEAST32_MIN;
  }
  static constexpr int_least32_t max() noexcept {
    return INT_LEAST32_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_least64_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t, int_least32_t>,
               inner::intt_useless_tag<3>, int_least64_t>> {
  using change_signedness = uint_least64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_least64_t min() noexcept {
    return INT_LEAST64_MIN;
  }
  static constexpr int_least64_t max() noexcept {
    return INT_LEAST64_MAX;
  }
};

template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_least8_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long>,
               inner::intt_useless_tag<4>, uint_least8_t>> {
  using change_signedness = int_least8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_least8_t min() noexcept {
    return 0;
  }
  static constexpr uint_least8_t max() noexcept {
    return UINT_LEAST8_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_least16_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long, uint_least8_t>,
               inner::intt_useless_tag<5>, uint_least16_t>> {
  using change_signedness = int_least16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_least16_t min() noexcept {
    return 0;
  }
  static constexpr uint_least16_t max() noexcept {
    return UINT_LEAST16_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_least32_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t>,
               inner::intt_useless_tag<6>, uint_least32_t>> {
  using change_signedness = int_least32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_least32_t min() noexcept {
    return 0;
  }
  static constexpr uint_least32_t max() noexcept {
    return UINT_LEAST32_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_least64_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t>,
               inner::intt_useless_tag<7>, uint_least64_t>> {
  using change_signedness = int_least64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_least64_t min() noexcept {
    return 0;
  }
  static constexpr uint_least64_t max() noexcept {
    return UINT_LEAST64_MAX;
  }
};

template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_fast8_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t>,
               inner::intt_useless_tag<8>, int_fast8_t>> {
  using change_signedness = uint_fast8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_fast8_t min() noexcept {
    return INT_FAST8_MIN;
  }
  static constexpr int_fast8_t max() noexcept {
    return INT_FAST8_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_fast16_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t>,
               inner::intt_useless_tag<9>, int_fast16_t>> {
  using change_signedness = uint_fast16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_fast16_t min() noexcept {
    return INT_FAST16_MIN;
  }
  static constexpr int_fast16_t max() noexcept {
    return INT_FAST16_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_fast32_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t>,
               inner::intt_useless_tag<10>, int_fast32_t>> {
  using change_signedness = uint_fast32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_fast32_t min() noexcept {
    return INT_FAST32_MIN;
  }
  static constexpr int_fast32_t max() noexcept {
    return INT_FAST32_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int_fast64_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t, int_fast32_t>,
               inner::intt_useless_tag<11>, int_fast64_t>> {
  using change_signedness = uint_fast64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int_fast64_t min() noexcept {
    return INT_FAST64_MIN;
  }
  static constexpr int_fast64_t max() noexcept {
    return INT_FAST64_MAX;
  }
};

template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_fast8_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t>,
               inner::intt_useless_tag<12>, uint_fast8_t>> {
  using change_signedness = int_fast8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_fast8_t min() noexcept {
    return 0;
  }
  static constexpr uint_fast8_t max() noexcept {
    return UINT_FAST8_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_fast16_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t, uint_fast8_t>,
             inner::intt_useless_tag<13>, uint_fast16_t>> {
  using change_signedness = int_fast16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_fast16_t min() noexcept {
    return 0;
  }
  static constexpr uint_fast16_t max() noexcept {
    return UINT_FAST16_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_fast32_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t>,
               inner::intt_useless_tag<14>, uint_fast32_t>> {
  using change_signedness = int_fast32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_fast32_t min() noexcept {
    return 0;
  }
  static constexpr uint_fast32_t max() noexcept {
    return UINT_FAST32_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint_fast64_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t>,
               inner::intt_useless_tag<15>, uint_fast64_t>> {
  using change_signedness = int_fast64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint_fast64_t min() noexcept {
    return 0;
  }
  static constexpr uint_fast64_t max() noexcept {
    return UINT_FAST64_MAX;
  }
};

template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uintmax_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t>,
               inner::intt_useless_tag<16>, uintmax_t>> {
  using change_signedness = intmax_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uintmax_t min() noexcept {
    return 0;
  }
  static constexpr uintmax_t max() noexcept {
    return UINTMAX_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<intmax_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t>,
               inner::intt_useless_tag<17>, intmax_t>> {
  using change_signedness = uintmax_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr intmax_t min() noexcept {
    return INTMAX_MIN;
  }
  static constexpr intmax_t max() noexcept {
    return INTMAX_MAX;
  }
};

#ifdef UINTPTR_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uintptr_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t,
                                 uintmax_t>,
               inner::intt_useless_tag<18>, uintptr_t>> {
  using change_signedness = intptr_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uintptr_t min() noexcept {
    return 0;
  }
  static constexpr uintptr_t max() noexcept {
    return UINTPTR_MAX;
  }
};
#endif

#ifdef INTPTR_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<intptr_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t, intmax_t>,
               inner::intt_useless_tag<19>, intptr_t>> {
  using change_signedness = uintptr_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr intptr_t min() noexcept {
    return INTPTR_MIN;
  }
  static constexpr intptr_t max() noexcept {
    return INTPTR_MAX;
  }
};
#endif

#ifdef INT8_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int8_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t, intmax_t
#ifdef INTPTR_MAX
                                 , intptr_t
#endif
                                 >,
               inner::intt_useless_tag<20>, int8_t>> {
  using change_signedness = uint8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int8_t min() noexcept {
    return INT8_MIN;
  }
  static constexpr int8_t max() noexcept {
    return INT8_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint8_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t, uintmax_t
#ifdef UINTPTR_MAX
                                 , uintptr_t
#endif
                                 >,
               inner::intt_useless_tag<21>, uint8_t>> {
  using change_signedness = int8_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint8_t min() noexcept {
    return 0;
  }
  static constexpr uint8_t max() noexcept {
    return UINT8_MAX;
  }
};
#endif

#ifdef INT16_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int16_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t, intmax_t
#ifdef INTPTR_MAX
                                 , intptr_t
#endif
                                 >,
               inner::intt_useless_tag<22>, int16_t>> {
  using change_signedness = uint16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int16_t min() noexcept {
    return INT16_MIN;
  }
  static constexpr int16_t max() noexcept {
    return INT16_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint16_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t, uintmax_t
#ifdef UINTPTR_MAX
                                 , uintptr_t
#endif
                                 >,
               inner::intt_useless_tag<23>, uint16_t>> {
  using change_signedness = int16_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint16_t min() noexcept {
    return 0;
  }
  static constexpr uint16_t max() noexcept {
    return UINT16_MAX;
  }
};
#endif

#ifdef INT32_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int32_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t, intmax_t
#ifdef INTPTR_MAX
                                 , intptr_t
#endif
                                 >,
               inner::intt_useless_tag<24>, int32_t>> {
  using change_signedness = uint32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int32_t min() noexcept {
    return INT32_MIN;
  }
  static constexpr int32_t max() noexcept {
    return INT32_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint32_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t, uintmax_t
#ifdef UINTPTR_MAX
                                 , uintptr_t
#endif
                                 >,
               inner::intt_useless_tag<25>, uint32_t>> {
  using change_signedness = int32_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint32_t min() noexcept {
    return 0;
  }
  static constexpr uint32_t max() noexcept {
    return UINT32_MAX;
  }
};
#endif

#ifdef INT64_MAX
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<int64_t,
                                 char, signed char, short, int, long, long long,
                                 int_least8_t, int_least16_t,
                                 int_least32_t, int_least64_t,
                                 int_fast8_t, int_fast16_t,
                                 int_fast32_t, int_fast64_t, intmax_t
#ifdef INTPTR_MAX
                                 , intptr_t
#endif
                                 >,
               inner::intt_useless_tag<26>, int64_t>> {
  using change_signedness = uint64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr int64_t min() noexcept {
    return INT64_MIN;
  }
  static constexpr int64_t max() noexcept {
    return INT64_MAX;
  }
};
template <>
struct integral_traits
<conditional_t<is_one_of_types_v<uint64_t,
                                 char, unsigned char, unsigned short,
                                 unsigned int, unsigned long,
                                 unsigned long long,
                                 uint_least8_t, uint_least16_t,
                                 uint_least32_t, uint_least64_t,
                                 uint_fast8_t, uint_fast16_t,
                                 uint_fast32_t, uint_fast64_t, uintmax_t
#ifdef UINTPTR_MAX
                                 , uintptr_t
#endif
                                 >,
               inner::intt_useless_tag<27>, uint64_t>> {
  using change_signedness = int64_t;

  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = false;
  static constexpr uint64_t min() noexcept {
    return 0;
  }
  static constexpr uint64_t max() noexcept {
    return UINT64_MAX;
  }
};
#endif

template <>
struct integral_traits<char8_t> : integral_traits<unsigned char> {};
template <>
struct integral_traits<char16_t> : integral_traits<uint_least16_t> {};
template <>
struct integral_traits<char32_t> : integral_traits<uint_least32_t> {};

}

// numeric_limits for integer
namespace re {

template <class T>
struct floating_point_traits;

template <class T>
struct numeric_limits {};
template <integral T>
struct numeric_limits<T> : private integral_traits<T> {
  using integral_traits<T>::is_specialized;
  using integral_traits<T>::is_signed;
  using integral_traits<T>::min;
  using integral_traits<T>::max;
  static constexpr T lowest() {
    return min();
  }
  static constexpr bool is_integer = true;
};

}

// three-way comparison
namespace re {

using partial_ordering = std::partial_ordering;
using weak_ordering = std::weak_ordering;
using strong_ordering = std::strong_ordering;

struct fo_is_eq {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o == 0;
  }
};
struct fo_is_neq {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o != 0;
  }
};
struct fo_is_lt {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o < 0;
  }
};
struct fo_is_gt {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o > 0;
  }
};
struct fo_is_lteq {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o <= 0;
  }
};
struct fo_is_gteq {
  constexpr bool operator ()(partial_ordering o) const noexcept {
    return o >= 0;
  }
};
inline constexpr fo_is_eq is_eq{};
inline constexpr fo_is_neq is_neq{};
inline constexpr fo_is_lt is_lt{};
inline constexpr fo_is_gt is_gt{};
inline constexpr fo_is_lteq is_lteq{};
inline constexpr fo_is_gteq is_gteq{};

inline constexpr partial_ordering partial_lt = partial_ordering::less;
inline constexpr partial_ordering partial_gt = partial_ordering::greater;
inline constexpr partial_ordering partial_eq
  = std::partial_ordering::equivalent;
inline constexpr partial_ordering partial_uo = partial_ordering::unordered;

inline constexpr weak_ordering weak_lt = weak_ordering::less;
inline constexpr weak_ordering weak_gt = weak_ordering::greater;
inline constexpr weak_ordering weak_eq = weak_ordering::equivalent;

inline constexpr strong_ordering strong_lt = strong_ordering::less;
inline constexpr strong_ordering strong_gt = strong_ordering::greater;
inline constexpr strong_ordering strong_eq = strong_ordering::equivalent;

struct fo_reverse_ordering {
  constexpr strong_ordering operator ()(strong_ordering x) const noexcept {
    if (x == strong_lt)
      return strong_gt;
    else if (x == strong_gt)
      return strong_lt;
    else
      return x;
  }
  constexpr weak_ordering operator ()(weak_ordering x) const noexcept {
    if (x == weak_lt)
      return weak_gt;
    else if (x == weak_gt)
      return weak_lt;
    else
      return x;
  }
  constexpr partial_ordering
  operator ()(partial_ordering x) const noexcept {
    if (x == partial_lt)
      return partial_gt;
    else if (x == partial_gt)
      return partial_lt;
    else
      return x;
  }
};
inline constexpr fo_reverse_ordering reverse_ordering{};

namespace inner {

template <class T>
struct int_of_comparison_category {
  static constexpr int value = 0;
};
template <class T>
struct int_of_comparison_category<const T>
  : int_of_comparison_category<T> {};
template <class T>
struct int_of_comparison_category<volatile T>
  : int_of_comparison_category<T> {};
template <class T>
struct int_of_comparison_category<const volatile T>
  : int_of_comparison_category<T> {};
template <>
struct int_of_comparison_category<strong_ordering> {
  static constexpr int value = 3;
};
template <>
struct int_of_comparison_category<weak_ordering> {
  static constexpr int value = 2;
};
template <>
struct int_of_comparison_category<partial_ordering> {
  static constexpr int value = 1;
};

template <int I>
struct comparison_category_of_int {
  using type = void;
};
template <>
struct comparison_category_of_int<1> {
  using type = partial_ordering;
};
template <>
struct comparison_category_of_int<2> {
  using type = weak_ordering;
};
template <>
struct comparison_category_of_int<3> {
  using type = strong_ordering;
};

namespace fns {

consteval int min_int(int a) {
  return a;
}
consteval int min_int(int a, int b) {
  return b < a ? b : a;
}
template <class...S>
consteval int min_int(int a, int b, S...s) {
  return min_int(inner::fns::min_int(a, b), s...);
}

}
template <class...S>
struct common_comparison_category_impl
  : int_constant
  <inner::fns::min_int(int_of_comparison_category<S>::value...)> {};
template <>
struct common_comparison_category_impl<> : int_constant<3> {};

}
template <class...S>
struct common_comparison_category
  : inner::comparison_category_of_int
  <inner::common_comparison_category_impl<S...>::value> {};
template <class...S>
using common_comparison_category_t
  = typename common_comparison_category<S...>::type;

namespace inner {

template <class T, class CAT = partial_ordering>
concept compares_as = same_as<common_comparison_category_t<T, CAT>, CAT>;

}
template <class T, class CAT = partial_ordering>
concept three_way_comparable
  = inner::weakly_equality_comparable_with<T, T>
  && inner::partially_ordered_with<T, T>
  && requires(const remove_reference_t<T> &a,
              const remove_reference_t<T> &b) {
    {a <=> b}->inner::compares_as<CAT>;
  };
template <class T, class U, class CAT = partial_ordering>
concept three_way_comparable_with
  = three_way_comparable<T, CAT>
  && three_way_comparable<U, CAT>
  && common_reference_with
  <const remove_reference_t<T> &, const remove_reference_t<U> &>
  && three_way_comparable<common_reference_t<const remove_reference_t<T> &,
                                             const remove_reference_t<U> &>,
                          CAT>
  && inner::weakly_equality_comparable_with<T, U>
  && inner::partially_ordered_with<T, U>
  && requires(const remove_reference_t<T> &t,
              const remove_reference_t<U> &u) {
    {t <=> u}->inner::compares_as<CAT>;
    {u <=> t}->inner::compares_as<CAT>;
  };

namespace inner {

struct check_three_way_callable {
  template <class A, class B>
  static decltype(declval<const remove_reference_t<A> &>()
                  <=> declval<const remove_reference_t<B> &>())
  f(type_pack<A, B>);
};
template <class A, class B,
          bool = f_is_well_formed_v<check_three_way_callable, A, B>>
struct compare_three_way_result_impl {};
template <class A, class B>
struct compare_three_way_result_impl<A, B, true> {
  using type = decltype(declval<const remove_reference_t<A> &>()
                        <=> declval<const remove_reference_t<B> &>());
};

}
template <class A, class B>
struct compare_three_way_result : inner::compare_three_way_result_impl<A, B> {};
template <class A, class B>
using compare_three_way_result_t
  = typename compare_three_way_result<A, B>::type;
struct compare_three_way {
  using is_transparent = inner::transparent_tag;

  template <class A, class B>
  constexpr auto operator()(A &&a, B &&b) const
    ->decltype(declval<A>() <=> declval<B>()) {
    return forward<A>(a) <=> forward<B>(b);
  }
};
template <class A, class B>
concept can_apply_compare_three_way = requires(A &&x, B &&y) {
  requires is_referenceable_v<A>;
  requires is_referenceable_v<B>;
  forward<A>(x) <=> forward<B>(y);
};

constexpr auto synth_3way
  = []<class T, class U>(const T &t, const U &u)
  requires (requires {
      {t < u}->inner::boolean_testable;
      {u < t}->inner::boolean_testable;
    }) {
  if constexpr (three_way_comparable_with<T, U>) {
    return t <=> u;
  }
  else {
    if (t < u)
      return weak_ordering::less;
    if (u < t)
      return weak_ordering::greater;
    return weak_ordering::equivalent;
  }
};
template <class T, class U = T>
using synth_3way_result = decltype(synth_3way(declval<T &>(), declval<U &>()));

}

// type_index
namespace re {

class type_index {
  const type_info *p;

public:
  type_index() = delete;
  ~type_index() = default;
  type_index(const type_index &) = default;
  type_index &operator =(const type_index &) = default;
  type_index(type_index &&) = default;
  type_index &operator =(type_index &&) = default;

  type_index(const type_info &x) noexcept : p(addressof(x)) {}

  bool operator ==(const type_index &x) const noexcept {
    return *p == *x.p;
  }
  bool operator <(const type_index &x) const noexcept {
    return (*p).before(*x.p);
  }
  bool operator >(const type_index &x) const noexcept {
    return x.p->before(*p);
  }
  bool operator <=(const type_index &x) const noexcept {
    return !x.p->before(*p);
  }
  bool operator >=(const type_index &x) const noexcept {
    return !p->before(*x.p);
  }
  strong_ordering operator <=>(const type_index &x) const noexcept {
    if (*p == *x.p)
      return strong_eq;
    if (p->before(*x.p))
      return strong_lt;
    return strong_gt;
  }

  size_t hash_code() const noexcept {
    return p->hash_code();
  }
  const char *name() const noexcept {
    return p->name();
  }
};

}

// basic function objects
namespace re {

namespace inner {

struct check_is_transparent_function {
  template <class X, class = typename X::is_transparent>
  static void f(type_pack<X>);
};

}
template <class T>
struct is_transparent_function
  : f_is_well_formed<inner::check_is_transparent_function, T> {};
template <class T>
inline constexpr bool is_transparent_function_v
  = is_transparent_function<T>::value;

// arithmetic operations

template <class T = void>
struct plus {
  constexpr T operator()(const T &x, const T &y) const {
    return x + y;
  }
};
template <>
struct plus<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) + forward<Y>(y)) {
    return forward<X>(x) + forward<Y>(y);
  };
};
template <class X, class Y>
concept can_apply_plus = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) + forward<Y>(y);
};

template <class T = void>
struct minus {
  constexpr T operator()(const T &x, const T &y) const {
    return x - y;
  }
};
template <>
struct minus<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) - forward<Y>(y)) {
    return forward<X>(x) - forward<Y>(y);
  };
};
template <class X, class Y>
concept can_apply_minus = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) - forward<Y>(y);
};

template <class T = void>
struct multiplies {
  constexpr T operator()(const T &x, const T &y) const {
    return x * y;
  }
};
template <>
struct multiplies<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) * forward<Y>(y)) {
    return forward<X>(x) * forward<Y>(y);
  };
};
template <class X, class Y>
concept can_apply_multiplies = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) * forward<Y>(y);
};

template <class T = void>
struct divides {
  constexpr T operator()(const T &x, const T &y) const {
    return x / y;
  }
};
template <>
struct divides<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) / forward<Y>(y)) {
    return forward<X>(x) / forward<Y>(y);
  };
};
template <class X, class Y>
concept can_apply_divides = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) / forward<Y>(y);
};

template <class T = void>
struct modulus {
  constexpr T operator()(const T &x, const T &y) const {
    return x % y;
  }
};
template <>
struct modulus<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) % forward<Y>(y)) {
    return forward<X>(x) % forward<Y>(y);
  };
};
template <class X, class Y>
concept can_apply_modulus = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) % forward<Y>(y);
};

template <class T = void>
struct negate {
  constexpr T operator()(const T &x) const {
    return -x;
  }
};
template <>
struct negate<void> {
  using is_transparent = inner::transparent_tag;

  template <class X>
  constexpr auto operator()(X &&x) const->decltype(-forward<X>(x)) {
    return -forward<X>(x);
  };
};
template <class X>
concept can_apply_negate = requires(X &&x) {
  requires is_referenceable_v<X>;
  -forward<X>(x);
};

// comparisons

template <class T = void>
struct equal_to {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x == y;
  }
};
template <>
struct equal_to<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) == forward<Y>(y)) {
    return forward<X>(x) == forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_equal_to = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) == forward<Y>(y);
};

template <class T = void>
struct not_equal_to {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x != y;
  }
};
template <>
struct not_equal_to<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) != forward<Y>(y)) {
    return forward<X>(x) != forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_not_equal_to = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) != forward<Y>(y);
};

template <class T = void>
struct greater {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x > y;
  }
};
template <>
struct greater<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) > forward<Y>(y)) {
    return forward<X>(x) > forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_greater = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) > forward<Y>(y);
};

template <class T = void>
struct less {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x < y;
  }
};
template <>
struct less<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) < forward<Y>(y)) {
    return forward<X>(x) < forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_less = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) < forward<Y>(y);
};

template <class T = void>
struct greater_equal {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x >= y;
  }
};
template <>
struct greater_equal<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) >= forward<Y>(y)) {
    return forward<X>(x) >= forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_greater_equal = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) >= forward<Y>(y);
};

template <class T = void>
struct less_equal {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x <= y;
  }
};
template <>
struct less_equal<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) <= forward<Y>(y)) {
    return forward<X>(x) <= forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_less_equal = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) <= forward<Y>(y);
};

// logical operations

template <class T = void>
struct logical_and {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x && y;
  }
};
template <>
struct logical_and<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) && forward<Y>(y)) {
    return forward<X>(x) && forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_logical_and = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) && forward<Y>(y);
};

template <class T = void>
struct logical_or {
  constexpr bool operator ()(const T &x, const T &y) const {
    return x || y;
  }
};
template <>
struct logical_or<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) || forward<Y>(y)) {
    return forward<X>(x) || forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_logical_or = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) || forward<Y>(y);
};

template <class T = void>
struct logical_not {
  constexpr bool operator ()(const T &x) const {
    return !x;
  }
};
template <>
struct logical_not<void> {
  using is_transparent = inner::transparent_tag;

  template <class X>
  constexpr auto operator ()(X &&x) const->decltype(!forward<X>(x)) {
    return !forward<X>(x);
  }
};
template <class X>
concept can_apply_logical_not = requires(X &&x) {
  requires is_referenceable_v<X>;
  !forward<X>(x);
};

// bitwise operations

template <class T = void>
struct bit_and {
  constexpr T operator ()(const T &x, const T &y) const {
    return x & y;
  }
};
template <>
struct bit_and<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) & forward<Y>(y)) {
    return forward<X>(x) & forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_bit_and = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) & forward<Y>(y);
};

template <class T = void>
struct bit_or {
  constexpr T operator ()(const T &x, const T &y) const {
    return x | y;
  }
};
template <>
struct bit_or<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) | forward<Y>(y)) {
    return forward<X>(x) | forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_bit_or = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) | forward<Y>(y);
};

template <class T = void>
struct bit_xor {
  constexpr T operator ()(const T &x, const T &y) const {
    return x ^ y;
  }
};
template <>
struct bit_xor<void> {
  using is_transparent = inner::transparent_tag;

  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    ->decltype(forward<X>(x) ^ forward<Y>(y)) {
    return forward<X>(x) ^ forward<Y>(y);
  }
};
template <class X, class Y>
concept can_apply_bit_xor = requires(X &&x, Y &&y) {
  requires is_referenceable_v<X>;
  requires is_referenceable_v<Y>;
  forward<X>(x) ^ forward<Y>(y);
};

template <class T = void>
struct bit_not {
  constexpr T operator ()(const T &x) const {
    return ~x;
  }
};
template <>
struct bit_not<void> {
  using is_transparent = inner::transparent_tag;

  template <class X>
  constexpr auto operator ()(X &&x) const->decltype(~forward<X>(x)) {
    return ~forward<X>(x);
  }
};
template <class X>
concept can_apply_bit_not = requires(X &&x) {
  requires is_referenceable_v<X>;
  ~forward<X>(x);
};

struct identity {
  using is_transparent = inner::transparent_tag;

  template <class X>
  constexpr X &&operator ()(X &&x) const noexcept {
    return forward<X>(x);
  }
};


namespace inner {

template <class F>
class not_fn_t {
  F f;

public:
  not_fn_t() = default;
  ~not_fn_t() = default;
  not_fn_t(const not_fn_t &) = default;
  not_fn_t &operator =(const not_fn_t &) = default;
  not_fn_t(not_fn_t &&) = default;
  not_fn_t &operator =(not_fn_t &&) = default;
  constexpr void swap(not_fn_t &x, not_fn_t &y)
    noexcept(is_nothrow_swappable_v<F>)
    requires is_swappable_v<F> {
    adl_swap(x.f, y.f);
  }

  constexpr not_fn_t(const F &ff)
    noexcept(is_nothrow_copy_constructible_v<F>) : f(ff) {}
  constexpr not_fn_t(F &&ff)
    noexcept(is_nothrow_move_constructible_v<F>)
    requires (!is_same_v<F &&, const F &>) : f(forward<F>(ff)) {}

  template <class...S>
  constexpr auto operator()(S &&...s) &
    ->decltype(!invoke(f, forward<S>(s)...)) {
    return !invoke(f, forward<S>(s)...);
  }
  template <class...S>
  constexpr auto operator()(S &&...s) const &
    ->decltype(!invoke(f, forward<S>(s)...)) {
    return !invoke(f, forward<S>(s)...);
  }
  template <class...S>
  constexpr auto operator()(S &&...s) &&
    ->decltype(!invoke(static_cast<F &&>(f), forward<S>(s)...)) {
    return !invoke(static_cast<F &&>(f), forward<S>(s)...);
  }
  template <class...S>
  constexpr auto operator()(S &&...s) const &&
    ->decltype(!invoke(static_cast<const F &&>(f), forward<S>(s)...)) {
    return !invoke(static_cast<const F &&>(f), forward<S>(s)...);
  }
};

}
struct fo_not_fn {
  template <class F>
  constexpr auto operator ()(F &&f)
    const noexcept(noexcept(inner::not_fn_t<decay_t<F>>(forward<F>(f))))
    ->decltype(inner::not_fn_t<decay_t<F>>(forward<F>(f))) {
    return inner::not_fn_t<decay_t<F>>(forward<F>(f));
  }
};
inline constexpr fo_not_fn not_fn{};


namespace inner {

template <class R, class T>
class mem_fn_t {
  R T::*fp;

public:
  mem_fn_t();
  ~mem_fn_t() = default;
  mem_fn_t(const mem_fn_t &) = default;
  mem_fn_t &operator =(const mem_fn_t &) = default;
  mem_fn_t(mem_fn_t &&) = default;
  mem_fn_t &operator =(mem_fn_t &&) = default;

  constexpr mem_fn_t(R T::*f) noexcept : fp(f) {}

  template <class...S>
  constexpr auto operator ()(S &&...s) const
    ->decltype(invoke(fp, forward<S>(s)...)) {
    return invoke(fp, forward<S>(s)...);
  }
};

}
struct fo_mem_fn {
  template <class R, class T>
  constexpr inner::mem_fn_t<R, T> operator ()(R T::*p) const noexcept {
    return inner::mem_fn_t<R, T>(p);
  }
};
inline constexpr fo_mem_fn mem_fn{};

}

// tuple
namespace re {

template <class...>
class tuple;
template <class, size_t>
class array;

}
namespace std {

template <class...S>
struct tuple_size<re::tuple<S...>> {
  static constexpr size_t value = sizeof...(S);
};
template <size_t I, class...S>
struct tuple_element<I, re::tuple<S...>> {
  using type = re::nth_type_t<I, S...>;
};

template <class T, size_t N>
struct tuple_size<re::array<T, N>> {
  static constexpr size_t value = N;
};
template <size_t I, class T, size_t N>
struct tuple_element<I, re::array<T, N>> {
  using type = T;
};

}
namespace re {

struct piecewise_construct_t {
  explicit piecewise_construct_t() = default;
};
namespace inner {

struct ignore_t {
  ignore_t() = default;
  ~ignore_t() = default;
  ignore_t(const ignore_t &) = default;
  ignore_t &operator =(const ignore_t &) = default;
  ignore_t(ignore_t &&) = default;
  ignore_t &operator =(ignore_t &&) = default;

  template <class T>
  constexpr ignore_t &operator =(T &&) noexcept {
    return *this;
  }
  template <class T>
  constexpr const ignore_t &operator =(T &&) const noexcept {
    return *this;
  }
};

}
struct allocator_arg_t {
  explicit allocator_arg_t() = default;
};
inline constexpr piecewise_construct_t piecewise_construct{};
inline constexpr inner::ignore_t ignore{};
inline constexpr allocator_arg_t allocator_arg{};

template <class...>
class tuple;

template <class...S>
tuple(S...)->tuple<S...>;
template <class A, class...S>
tuple(allocator_arg_t, A, S...)->tuple<S...>;
template <class A, class...S>
tuple(allocator_arg_t, A, tuple<S...>)->tuple<S...>;
template <class...S>
requires (sizeof...(S) == 2)
using pair = tuple<S...>;

template <class...S, class...SS,
          template <class> class F, template <class> class FF>
requires requires {
  typename type_pack<common_reference_t<F<S>, FF<SS>>...>;
  requires sizeof...(S) == sizeof...(SS);
}
struct basic_common_reference<tuple<S...>, tuple<SS...>, F, FF> {
  using type = tuple<common_reference_t<F<S>, FF<SS>>...>;
};

template <class...S, class...SS>
requires requires {
  typename tuple<common_type_t<S, SS>...>;
  requires sizeof...(S) == sizeof...(SS);
}
struct common_type<tuple<S...>, tuple<SS...>> {
  using type = tuple<common_type_t<S, SS>...>;
};

template <class T>
struct tuple_size {};
template <class T>
struct tuple_size<const T> : tuple_size<T> {};
template <class T>
struct tuple_size<volatile T> : tuple_size<T> {};
template <class T>
struct tuple_size<const volatile T> : tuple_size<T> {};
template <class...S>
struct tuple_size<tuple<S...>> : integral_constant<size_t, sizeof...(S)> {};
template <class T>
inline constexpr size_t tuple_size_v = tuple_size<T>::value;

template <size_t I, class T>
struct tuple_element {};
template <size_t I, class T>
struct tuple_element<I, const T> {
  using type = add_const_t<typename tuple_element<I, T>::type>;
};
template <size_t I, class T>
struct tuple_element<I, volatile T> {
  using type = add_volatile_t<typename tuple_element<I, T>::type>;
};
template <size_t I, class T>
struct tuple_element<I, const volatile T> {
  using type = add_cv_t<typename tuple_element<I, T>::type>;
};
template <size_t I, class...S>
struct tuple_element<I, tuple<S...>> {
  using type = nth_type_t<I, S...>;
};
template <size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;

template <class T, size_t N>
struct tuple_size<array<T, N>> : integral_constant<size_t, N> {};
template <size_t I, class T, size_t N>
struct tuple_element<I, array<T, N>> {
  using type = T;
};

template <class T, class...S>
constexpr T &get(tuple<S...> &t) noexcept {
  return t.template as<T>();
}
template <class T, class...S>
constexpr const T &get(const tuple<S...> &t) noexcept {
  return t.template as<T>();
}
template <class T, class...S>
constexpr T &&get(tuple<S...> &&t) noexcept {
  return forward<tuple<S...>>(t).template as<T>();
}
template <class T, class...S>
constexpr const T &&get(const tuple<S...> &&t) noexcept {
  return forward<const tuple<S...>>(t).template as<T>();
}

template <size_t I, class...S>
constexpr tuple_element_t<I, tuple<S...>> &
get(tuple<S...> &t) noexcept {
  return t.template at<I>();
}
template <size_t I, class...S>
constexpr const tuple_element_t<I, tuple<S...>> &
get(const tuple<S...> &t) noexcept {
  return t.template at<I>();
}
template <size_t I, class...S>
constexpr tuple_element_t<I, tuple<S...>> &&
get(tuple<S...> &&t) noexcept {
  return forward<tuple<S...>>(t).template at<I>();
}
template <size_t I, class...S>
constexpr const tuple_element_t<I, tuple<S...>> &&
get(const tuple<S...> &&t) noexcept {
  return forward<const tuple<S...>>(t).template at<I>();
}

template <class T>
struct fo_as {
  template <class U>
  constexpr auto operator ()(U &&x)
    const noexcept(noexcept(declval<U>().template as<T>()))
    ->decltype(declval<U>().template as<T>()) {
    return forward<U>(x).template as<T>();
  }
};
template <class T>
inline constexpr fo_as<T> as{};
template <size_t I>
struct fo_at {
  template <class T>
  constexpr auto operator ()(T &&x)
    const noexcept(noexcept(declval<T>().template at<I>()))
    ->decltype(declval<T>().template at<I>()) {
    return forward<T>(x).template at<I>();
  }
};
template <size_t I>
inline constexpr fo_at<I> at{};

struct fo_forward_as_tuple {
  template <class...S>
  constexpr tuple<S &&...> operator ()(S &&...s) const noexcept {
    return tuple<S &&...>(forward<S>(s)...);
  }
};
inline constexpr fo_forward_as_tuple forward_as_tuple{};

template <class...S, class AL>
struct uses_allocator<tuple<S...>, AL> : true_type {};
template <class T>
struct fo_uses_allocator_construction_args {
  template <class AL, class...S>
  constexpr enable_if_t<!uses_allocator_v<T, AL>, tuple<S &&...>>
  operator ()(const AL &, S &&...s) const noexcept {
    return tuple<S &&...>(forward<S>(s)...);
  }
  template <class AL, class...S>
  constexpr
  enable_if_t<uses_allocator_v<T, AL>
              && is_constructible_v<T, allocator_arg_t, const AL &, S &&...>,
              tuple<allocator_arg_t, const AL &, S &&...>>
  operator ()(const AL &al, S &&...s) const noexcept {
    return tuple<allocator_arg_t, const AL &, S &&...>
      (allocator_arg, al, forward<S>(s)...);
  }
  template <class AL, class...S>
  constexpr
  enable_if_t<uses_allocator_v<T, AL>
              && !is_constructible_v<T, allocator_arg_t, const AL &, S &&...>,
              tuple<S &&..., const AL &>>
  operator ()(const AL &al, S &&...s) const noexcept {
    return tuple<S &&..., const AL &>(forward<S>(s)..., al);
  }
};
template <class T>
inline constexpr
fo_uses_allocator_construction_args<T> uses_allocator_construction_args{};

namespace inner {

struct alloc_arg_t {};
struct piecewise_t {};

template <class...>
struct tuple_data;
template <class T, class...S>
struct tuple_data<T, S...> {
  template <class...>
  friend struct tuple_data;

  T value;
  tuple_data<S...> following;

  constexpr tuple_data() : value{}, following{} {}
  ~tuple_data() = default;
  tuple_data(const tuple_data &) = default;
  tuple_data &operator =(const tuple_data &x) = default;
  tuple_data(tuple_data &&) = default;
  tuple_data &operator =(tuple_data &&x) = default;
  constexpr void swap(tuple_data &x) {
    adl_swap(value, x.value);
    following.swap(x.following);
  }
  constexpr void swap(const tuple_data &x) const {
    adl_swap(value, x.value);
    following.swap(x.following);
  }

  template <class TT, class...SS>
  constexpr tuple_data(TT &&t, SS &&...s)
    : value(forward<TT>(t)), following(forward<SS>(s)...) {}
  template <class TT, class...SS>
  constexpr tuple_data(tuple_data<TT, SS...> &x)
    : value(x.value), following(x.following) {}
  template <class TT, class...SS>
  constexpr tuple_data(const tuple_data<TT, SS...> &x)
    : value(x.value), following(x.following) {}
  template <class TT, class...SS>
  constexpr tuple_data(tuple_data<TT, SS...> &&x)
    : value(forward<TT>(x.value)), following(move(x.following)) {}
  template <class TT, class...SS>
  constexpr tuple_data(const tuple_data<TT, SS...> &&x)
    : value(forward<const TT>(x.value)), following(move(x.following)) {}

  struct constructor_tag0_t {};
  struct constructor_tag_t {};
  template <class TUPLE, size_t...IDS, class...SS>
  constexpr tuple_data(constructor_tag0_t, index_sequence<IDS...>,
                       TUPLE &&x, SS &&...s)
    : value(re::at<IDS>(forward<TUPLE>(x))...)
    , following(forward<SS>(s)...) {}
  template <class TUPLE, class...SS>
  constexpr tuple_data(constructor_tag_t, TUPLE &&x, SS &&...s)
    : tuple_data(constructor_tag0_t{},
                 make_index_sequence<tuple_size_v<decay_t<TUPLE>>>(),
                 forward<TUPLE>(x), forward<SS>(s)...) {}

  template <class AL>
  constexpr tuple_data(alloc_arg_t, const AL &al)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>(al),
                 alloc_arg_t{}, al) {}
  template <class AL, class TT, class...SS>
  constexpr tuple_data(alloc_arg_t, const AL &al, TT &&t, SS &&...s)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>(al, forward<TT>(t)),
                 alloc_arg_t{}, al, forward<SS>(s)...) {}
  template <class AL, class TT, class...SS>
  constexpr tuple_data(alloc_arg_t, const AL &al, tuple_data<TT, SS...> &x)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>(al, x.value),
                 alloc_arg_t{}, al, x.following) {}
  template <class AL, class TT, class...SS>
  constexpr tuple_data(alloc_arg_t, const AL &al,
                       const tuple_data<TT, SS...> &x)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>(al, x.value),
                 alloc_arg_t{}, al, x.following) {}
  template <class AL, class TT, class...SS>
  constexpr tuple_data(alloc_arg_t, const AL &al,
                       tuple_data<TT, SS...> &&x)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>(al, forward<TT>(x.value)),
                 alloc_arg_t{}, al, move(x.following)) {}
  template <class AL, class TT, class...SS>
  constexpr tuple_data(alloc_arg_t, const AL &al,
                       const tuple_data<TT, SS...> &&x)
    : tuple_data(constructor_tag_t{},
                 uses_allocator_construction_args<T>
                 (al, forward<const TT>(x.value)),
                 alloc_arg_t{}, al, move(x.following)) {}

  template <class X, class...SS>
  constexpr tuple_data(piecewise_t, X &&x, SS &&...s)
    : tuple_data(constructor_tag_t(), forward<X>(x),
                 piecewise_t{}, forward<SS>(s)...) {}

  template <size_t I>
  constexpr enable_if_t<I == 0, T &> at() noexcept {
    return value;
  }
  template <size_t I>
  constexpr enable_if_t<I != 0, nth_type_t<I - 1, S...> &> at() noexcept {
    return following.template at<I - 1>();
  }

  template <size_t I>
  constexpr enable_if_t<I == 0, const T &> at() const noexcept {
    return value;
  }
  template <size_t I>
  constexpr enable_if_t<I != 0, const nth_type_t<I - 1, S...> &>
  at() const noexcept {
    return following.template at<I - 1>();
  }

  template <class TT>
  constexpr enable_if_t<is_same_v<TT, T>, TT &> as() noexcept {
    return value;
  }
  template <class TT>
  constexpr enable_if_t<!is_same_v<TT, T>, TT &> as() noexcept {
    return following.template as<TT>();
  }

  template <class TT>
  constexpr enable_if_t<is_same_v<TT, T>, const TT &> as() const noexcept {
    return value;
  }
  template <class TT>
  constexpr enable_if_t<!is_same_v<TT, T>, const TT &>
  as() const noexcept {
    return following.template as<TT>();
  }

  template <class TT, class...SS>
  constexpr void assign(const tuple_data<TT, SS...> &x) {
    value = x.value;
    following.assign(x.following);
  }
  template <class TT, class...SS>
  constexpr void assign(tuple_data<TT, SS...> &&x) {
    value = forward<TT>(x.value);
    following.assign(move(x.following));
  }
  template <class TT, class...SS>
  constexpr bool equal_to(const tuple_data<TT, SS...> &x) const {
    if (!(value == x.value))
      return false;
    return following.equal_to(x.following);
  }
  template <class TT, class...SS>
  constexpr common_comparison_category_t
  <synth_3way_result<T, TT>,
   synth_3way_result<S, SS>...>
  three_way(const tuple_data<TT, SS...> &x) const {
    const auto o = synth_3way(value, x.value);
    if (o != 0)
      return o;
    return following.three_way(x.following);
  }
};
template <class T>
struct tuple_data<T> {
  template <class...>
  friend struct tuple_data;

  T value;

  constexpr tuple_data() : value{} {}
  ~tuple_data() = default;
  tuple_data(const tuple_data &) = default;
  tuple_data &operator =(const tuple_data &) = default;
  tuple_data(tuple_data &&) = default;
  tuple_data &operator =(tuple_data &&) = default;
  constexpr void swap(tuple_data &x) {
    adl_swap(value, x.value);
  }
  constexpr void swap(const tuple_data &x) const {
    adl_swap(value, x.value);
  }

  template <class TT>
  constexpr tuple_data(TT &&t) : value(forward<TT>(t)) {}
  template <class TT>
  constexpr tuple_data(tuple_data<TT> &x) : value(x.value) {}
  template <class TT>
  constexpr tuple_data(const tuple_data<TT> &x) : value(x.value) {}
  template <class TT>
  constexpr tuple_data(tuple_data<TT> &&x) : value(forward<TT>(x.value)) {}
  template <class TT>
  constexpr tuple_data(const tuple_data<TT> &&x)
    : value(forward<const TT>(x.value)) {}

  struct constructor_tag0_t {};
  struct constructor_tag_t {};
  template <class TUPLE, size_t...IDS>
  constexpr tuple_data(constructor_tag0_t, index_sequence<IDS...>, TUPLE &&x)
    : value(re::at<IDS>(forward<TUPLE>(x))...) {}
  template <class TUPLE>
  constexpr tuple_data(constructor_tag_t, TUPLE &&x)
    : tuple_data(constructor_tag0_t(),
                 make_index_sequence<tuple_size_v<decay_t<TUPLE>>>(),
                 forward<TUPLE>(x)) {}

  template <class AL>
  constexpr tuple_data(alloc_arg_t, const AL &al)
    : tuple_data(constructor_tag_t(),
                 uses_allocator_construction_args<T>(al)) {}
  template <class AL, class TT>
  constexpr tuple_data(alloc_arg_t, const AL &al, TT &&t)
    : tuple_data(constructor_tag_t(),
                 uses_allocator_construction_args<T>(al, forward<TT>(t))) {}
  template <class AL, class TT>
  constexpr tuple_data(alloc_arg_t, const AL &al, tuple_data<TT> &x)
    : tuple_data(alloc_arg_t{}, al, x.value) {}
  template <class AL, class TT>
  constexpr tuple_data(alloc_arg_t, const AL &al, const tuple_data<TT> &x)
    : tuple_data(alloc_arg_t{}, al, x.value) {}
  template <class AL, class TT>
  constexpr tuple_data(alloc_arg_t, const AL &al, tuple_data<TT> &&x)
    : tuple_data(alloc_arg_t{}, al, forward<TT>(x.value)) {}
  template <class AL, class TT>
  constexpr tuple_data(alloc_arg_t, const AL &al, const tuple_data<TT> &&x)
    : tuple_data(alloc_arg_t{}, al, forward<const TT>(x.value)) {}

  template <class X>
  constexpr tuple_data(piecewise_t, X &&x)
    : tuple_data(constructor_tag_t(), forward<X>(x)) {}

  template <size_t I>
  constexpr enable_if_t<I == 0, T &> at() noexcept {
    return value;
  }
  template <size_t I>
  constexpr enable_if_t<I == 0, const T &> at() const noexcept {
    return value;
  }

  template <class TT>
  constexpr enable_if_t<is_same_v<TT, T>, TT &> as() noexcept {
    return value;
  }
  template <class TT>
  constexpr enable_if_t<is_same_v<TT, T>, const TT &> as() const noexcept {
    return value;
  }

  template <class TT>
  constexpr void assign(const tuple_data<TT> &x) {
    value = x.value;
  }
  template <class TT>
  constexpr void assign(tuple_data<TT> &&x) {
    value = forward<TT>(x.value);
  }
  template <class TT>
  constexpr bool equal_to(const tuple_data<TT> &x) const {
    return value == x.value;
  }
  template <class TT>
  constexpr common_comparison_category_t<synth_3way_result<T, TT>>
  three_way(const tuple_data<TT> &x) const {
    return synth_3way(value, x.value);
  }
};

template <class X, class Y>
concept eqable = requires (const X &x, const Y &y) {
  {x == y}->inner::boolean_testable;
};

}
template <class...S>
requires (sizeof...(S) != 0 && sizeof...(S) != 2)
class tuple<S...> {
  using data_t = inner::tuple_data<S...>;
  data_t data;

  template <class...>
  friend class tuple;

public:
  constexpr
  explicit(disjunction_v
           <typename negation
            <typename is_implicitly_constructible<S>::type>::type...>)
  tuple() noexcept((is_nothrow_constructible_v<S> && ...))
    requires (is_default_constructible_v<S> && ...)
    = default;
  ~tuple() = default;
  constexpr tuple(const tuple &)
    requires (is_copy_constructible_v<S> && ...)
    = default;
  constexpr tuple &operator =(const tuple &)
    requires ((is_copy_assignable_v<S> && ...)
              && is_copy_assignable_v<data_t>)
    = default;
  constexpr tuple &operator =(const tuple &x)
    noexcept((is_nothrow_copy_assignable_v<S> && ...))
    requires ((is_copy_assignable_v<S> && ...)
              && !is_copy_assignable_v<data_t>) {
    data.assign(x.data);
    return *this;
  }
  constexpr tuple(tuple &&) requires (is_move_constructible_v<S> && ...)
    = default;
  constexpr tuple &operator =(tuple &&)
    requires ((is_move_assignable_v<S> && ...)
              && is_move_assignable_v<data_t>)
    = default;
  constexpr tuple &operator =(tuple &&x)
    noexcept((is_nothrow_move_assignable_v<S> && ...))
    requires ((is_move_assignable_v<S> && ...)
              && !is_move_assignable_v<data_t>) {
    data.assign(move(x.data));
    return *this;
  }
  constexpr void swap(tuple &x) noexcept((is_nothrow_swappable_v<S> && ...))
    requires (is_swappable_v<S> && ...) {
    data.swap(x.data);
  }
  constexpr void swap(const tuple &x) const
    noexcept((is_nothrow_swappable_v<const S> && ...))
    requires (is_swappable_v<const S> && ...) {
    data.swap(x.data);
  }

  template <class AL>
  constexpr
  explicit(disjunction_v
           <typename negation
            <typename is_implicitly_constructible<S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al)
    requires (is_default_constructible_v<S> && ...)
    : data(inner::alloc_arg_t{}, al) {}

  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &al, const tuple &x)
    requires (is_copy_constructible_v<S> && ...)
    : data(inner::alloc_arg_t{}, al, x.data) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &al, tuple &&x)
    requires (is_move_constructible_v<S> && ...)
    : data(inner::alloc_arg_t{}, al, move(x.data)) {}

  constexpr explicit(negation_v
                     <conjunction
                      <typename is_convertible<const S &, S>::type...>>)
  tuple(const S &...s) requires (is_copy_constructible_v<S> && ...)
    : data(s...) {}
  template <class AL>
  constexpr
  explicit(disjunction_v
           <typename negation
            <typename is_convertible<const S &, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, const S &...s)
    requires (is_copy_constructible_v<S> && ...)
    : data(inner::alloc_arg_t{}, al, s...) {}

  template <class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                      <typename is_convertible<SS &&, S>::type>::type...>)
  tuple(SS &&...s)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &&> && ...))
    : data(forward<SS>(s)...) {}
  template <class AL, class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<SS &&, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, SS &&...s)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &&> && ...))
    : data(inner::alloc_arg_t{}, al, forward<SS>(s)...) {}

  template <class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                      <typename is_convertible<SS &, S>::type>::type...>)
  tuple(tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<tuple<SS> &, S> && ...)
                      && !(is_constructible_v<S, tuple<SS> &> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(static_cast<typename tuple<SS...>::data_t &>(x.data)) {}
  template <class AL, class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<SS &, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<tuple<SS> &, S> && ...)
                      && !(is_constructible_v<S, tuple<SS> &> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(inner::alloc_arg_t{}, al,
           static_cast<typename tuple<SS...>::data_t &>(x.data)) {}
  template <class...SS>
  constexpr tuple &operator =(tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_assignable_v<S &, SS &> && ...)) {
    data.assign(x.data);
    return *this;
  }

  template <class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                      <typename is_convertible<const SS &, S>::type>::type...>)
  tuple(const tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, const SS &> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<const tuple<SS> &, S> && ...)
                      && !(is_constructible_v<S, const tuple<SS> &> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(static_cast<const typename tuple<SS...>::data_t &>(x.data)) {}
  template <class AL, class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<const SS &, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, const tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, const SS &> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<const tuple<SS> &, S> && ...)
                      && !(is_constructible_v<S, const tuple<SS> &> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(inner::alloc_arg_t{}, al,
           static_cast<const typename tuple<SS...>::data_t &>(x.data)) {}
  template <class...SS>
  constexpr tuple &operator =(const tuple<SS...> &x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_assignable_v<S &, const SS &> && ...)) {
    data.assign(x.data);
    return *this;
  }

  template <class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                      <typename is_convertible<SS &&, S>::type>::type...>)
  tuple(tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &&> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<tuple<SS> &&, S> && ...)
                      && !(is_constructible_v<S, tuple<SS> &&> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(move(x.data)) {}
  template <class AL, class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<SS &&, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, SS &&> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<tuple<SS> &&, S> && ...)
                      && !(is_constructible_v<S, tuple<SS> &&> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(inner::alloc_arg_t{}, al, move(x.data)) {}
  template <class...SS>
  constexpr tuple &operator =(tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_assignable_v<S &, SS &&> && ...)) {
    data.assign(move(x.data));
    return *this;
  }

  template <class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<const SS &&, S>::type>::type...>)
  tuple(const tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, const SS &&> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<const tuple<SS> &&, S> && ...)
                      && !(is_constructible_v<S, const tuple<SS> &&> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(move(x.data)) {}
  template <class AL, class...SS>
  constexpr explicit(disjunction_v
                     <typename negation
                     <typename is_convertible<const SS &&, S>::type>::type...>)
  tuple(allocator_arg_t, const AL &al, const tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_constructible_v<S, const SS &&> && ...)
              && (sizeof...(S) != 1
                  || (!(is_convertible_v<const tuple<SS> &&, S> && ...)
                      && !(is_constructible_v<S, const tuple<SS> &&> && ...)
                      && !(is_same_v<S, SS> && ...))))
    : data(inner::alloc_arg_t{}, al, move(x.data)) {}
  template <class...SS>
  constexpr tuple &operator =(const tuple<SS...> &&x)
    requires (sizeof...(S) == sizeof...(SS)
              && (is_assignable_v<S &, const SS &&> && ...)) {
    data.assign(move(x.data));
    return *this;
  }

  template <class...SS>
  constexpr tuple(piecewise_construct_t, SS...s)
    : data(inner::piecewise_t{}, move(s)...) {}

  template <size_t I>
  constexpr tuple_element_t<I, tuple> &at() & noexcept
    requires (I < sizeof...(S)) {
    return data.template at<I>();
  }
  template <size_t I>
  constexpr const tuple_element_t<I, tuple> &at() const & noexcept
    requires (I < sizeof...(S)) {
    return data.template at<I>();
  }
  template <size_t I>
  constexpr tuple_element_t<I, tuple> &&at() && noexcept
    requires (I < sizeof...(S)) {
    return (tuple_element_t<I, tuple> &&)data.template at<I>();
  }
  template <size_t I>
  constexpr const tuple_element_t<I, tuple> &&at() const && noexcept
    requires (I < sizeof...(S)) {
    return (const tuple_element_t<I, tuple> &&)data.template at<I>();
  }

  template <class T>
  constexpr T &as() & noexcept
    requires occurs_exactly_once_v<T, S...> {
    return data.template as<T>();
  }
  template <class T>
  constexpr const T &as() const & noexcept
    requires occurs_exactly_once_v<T, S...> {
    return data.template as<T>();
  }
  template <class T>
  constexpr T &&as() && noexcept
    requires occurs_exactly_once_v<T, S...> {
    return static_cast<T &&>(data.template as<T>());
  }
  template <class T>
  constexpr const T &&as() const && noexcept
    requires occurs_exactly_once_v<T, S...> {
    return static_cast<const T &&>(data.template as<T>());
  }

  template <class...SS, class...SSS>
  friend constexpr bool operator ==(const tuple<SS...> &,
                                    const tuple<SSS...> &)
    requires (sizeof...(SS) == sizeof...(SSS)
              && (inner::eqable<SS, SSS> && ...));
  template <class...SS, class...SSS>
  friend constexpr
  common_comparison_category_t<synth_3way_result<SS, SSS>...>
  operator <=>(const tuple<SS...> &, const tuple<SSS...> &);
};
template <class...S>
constexpr auto swap(tuple<S...> &x, tuple<S...> &y)
  noexcept(noexcept(x.swap(y)))->decltype(x.swap(y)) {
  x.swap(y);
}
template <class...S>
constexpr auto swap(const tuple<S...> &x, const tuple<S...> &y)
  noexcept(noexcept(x.swap(y)))->decltype(x.swap(y)) {
  x.swap(y);
}
template <class...S, class...SS>
constexpr bool operator ==(const tuple<S...> &x, const tuple<SS...> &y)
  requires (sizeof...(S) == sizeof...(SS)
            && (inner::eqable<S, SS> && ...)) {
  return x.data.equal_to(y.data);
}
template <class...SS, class...SSS>
constexpr common_comparison_category_t
<synth_3way_result<SS, SSS>...>
operator <=>(const tuple<SS...> &x, const tuple<SSS...> &y) {
  return x.data.three_way(y.data);
}

template <>
class tuple<> {
public:
  constexpr tuple() = default;
  ~tuple() = default;
  constexpr tuple(const tuple &) = default;
  constexpr tuple &operator =(const tuple &) = default;
  constexpr tuple(tuple &&) = default;
  constexpr tuple &operator =(tuple &&) = default;
  constexpr void swap(tuple &) noexcept {}
  constexpr void swap(const tuple &) noexcept {}

  constexpr tuple(piecewise_construct_t) {}

  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &, tuple &) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &, const tuple &) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &, tuple &&) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &, const tuple &&) {}

  template <class>
  constexpr void as() const = delete;
  template <size_t>
  constexpr void at() const = delete;
};

namespace inner {

template <class T>
struct make_tuple_type {
  using type = T;
};
template <class T>
struct make_tuple_type<reference_wrapper<T>> {
  using type = T &;
};

}
struct fo_make_tuple {
  template <class...S>
  constexpr auto operator ()(S &&...s) const {
    return tuple<typename inner::make_tuple_type<decay_t<S>>::type...>
      (forward<S>(s)...);
  }
};
inline constexpr fo_make_tuple make_tuple{};
struct fo_make_pair {
  template <class T1, class T2>
  constexpr auto operator ()(T1 &&x, T2 &&y) const {
    return tuple<typename inner::make_tuple_type<decay_t<T1>>::type,
                 typename inner::make_tuple_type<decay_t<T2>>::type
                 >(forward<T1>(x), forward<T2>(y));
  }
};
inline constexpr fo_make_pair make_pair{};

struct fo_tie {
  template<class...S>
  constexpr tuple<S &...> operator ()(S &...s) const noexcept {
    return tuple<S &...>(s...);
  }
};
inline constexpr fo_tie tie{};

namespace inner {

template <class...S>
struct tuple_cat_result_impl;
template <>
struct tuple_cat_result_impl<> {
  using type = tuple<>;
};
template <class...S>
struct tuple_cat_result_impl<tuple<S...>> {
  using type = tuple<S...>;
};
template <class...S, class...SS, class...SSS>
struct tuple_cat_result_impl<tuple<S...>, tuple<SS...>, SSS...>
  : tuple_cat_result_impl<tuple<S..., SS...>, SSS...> {};

template <class...S>
struct tuple_cat_result {
  using type = typename tuple_cat_result_impl<remove_cvref_t<S>...>::type;
};
template <class...S>
using tuple_cat_result_t = typename tuple_cat_result<S...>::type;

}
namespace inner::fns {

template <class R, class...S, size_t...SS>
constexpr R tuple_cat_impl00(index_sequence<SS...>, tuple<S...> acc) {
  return R(at<SS>(move(acc))...);
}

template <size_t...S, size_t...SS, class...SSS, class X>
constexpr auto tuple_cat_impl0(index_sequence<S...>, index_sequence<SS...>,
                               tuple<SSS...> acc, X &&x) {
  return tuple<SSS..., decltype(at<SS>(forward<X>(x)))...>
    (at<S>(move(acc))..., at<SS>(forward<X>(x))...);
}

template <class R, class...S>
constexpr R tuple_cat_impl(tuple<S...> acc) {
  return inner::fns::tuple_cat_impl00<R>
    (make_index_sequence<sizeof...(S)>(), move(acc));
}
template <class R, class X, class...S, class...SS>
constexpr R tuple_cat_impl(tuple<S...> acc, X &&x, SS &&...s) {
  return inner::fns::tuple_cat_impl<R>
    (inner::fns::tuple_cat_impl0(make_index_sequence<sizeof...(S)>(),
                                 make_index_sequence
                                 <tuple_size_v<remove_reference_t<X>>>(),
                                 move(acc), forward<X>(x)),
     forward<SS>(s)...);
}

}
struct fo_tuple_cat {
  template <class...S>
  constexpr inner::tuple_cat_result_t<S...> operator ()(S &&...s) const {
    return inner::fns::tuple_cat_impl<inner::tuple_cat_result_t<S...>
                                      >(tuple<>{}, forward<S>(s)...);
  }
};
inline constexpr fo_tuple_cat tuple_cat{};

namespace inner::fns {

template <class F, class TUPLE, size_t...I>
constexpr decltype(auto)
apply_impl(F &&f, TUPLE &&t, index_sequence<I...>) {
  return inner::fns::invoke_impl(forward<F>(f), at<I>(forward<TUPLE>(t))...);
}

}
struct fo_apply {
  template <class F, class TUPLE>
  constexpr decltype(auto) operator ()(F &&f, TUPLE &&t) const {
    return inner::fns::apply_impl(forward<F>(f), forward<TUPLE>(t),
                                  make_index_sequence
                                  <tuple_size_v<decay_t<TUPLE>>>());
  }
};
inline constexpr fo_apply apply{};

template <class T>
struct fo_make_from_tuple {
  template <class TUPLE>
  constexpr T operator ()(TUPLE &&t) const {
    return apply([]<class...S>(S &&...s) {return T(forward<S>(s)...);},
                 forward<TUPLE>(t));
  }
};
template <class T>
inline constexpr fo_make_from_tuple<T> make_from_tuple{};

template <class T1, class T2>
struct min_pair {
  T1 first;
  T2 second;
};
template <class...S>
struct min_tuple {};
template <class T>
struct min_tuple<T> {
  T value;
};
template <class T, class...S>
struct min_tuple<T, S...> {
  T value;
  min_tuple<S...> following;
};

template <class T1, class T2>
class tuple<T1, T2> {
public:
  using first_type = T1;
  using second_type = T2;

  T1 first;
  T2 second;

  constexpr explicit(!(is_implicitly_constructible_v<T1>
                       && is_implicitly_constructible_v<T2>))
  tuple() noexcept(is_nothrow_default_constructible_v<T1>
                   && is_nothrow_default_constructible_v<T2>)
    requires (is_default_constructible_v<T1> && is_default_constructible_v<T2>)
    : first(), second() {}
  ~tuple() = default;
  constexpr tuple(const tuple &) = default;
  constexpr tuple &operator =(const tuple &)
    requires (is_copy_assignable_v<T1> && is_copy_assignable_v<T2>
              && is_copy_assignable_v<min_pair<T1, T2>>)
    = default;
  constexpr tuple &operator =(const tuple &x)
    noexcept (is_nothrow_copy_assignable_v<T1>
              && is_nothrow_copy_assignable_v<T2>)
    requires (is_copy_assignable_v<T1> && is_copy_assignable_v<T2>
              && !is_copy_assignable_v<min_pair<T1, T2>>) {
    first = x.first;
    second = x.second;
    return *this;
  }
  constexpr tuple(tuple &&) = default;
  constexpr tuple &operator =(tuple &&)
    requires (is_move_assignable_v<T1> && is_move_assignable_v<T2>
              && is_move_assignable_v<min_pair<T1, T2>>)
    = default;
  constexpr tuple &operator =(tuple &&x)
    noexcept (is_nothrow_move_assignable_v<T1>
              && is_nothrow_move_assignable_v<T2>)
    requires (is_move_assignable_v<T1> && is_move_assignable_v<T2>
              && !is_move_assignable_v<min_pair<T1, T2>>) {
    first = forward<T1>(x.first);
    second = forward<T2>(x.second);
    return *this;
  }
  constexpr void swap(tuple &x)
    noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>)
    requires (is_swappable_v<T1> && is_swappable_v<T2>) {
    adl_swap(first, x.first);
    adl_swap(second, x.second);
  }
  constexpr void swap(const tuple &x) const
    noexcept(is_nothrow_swappable_v<const T1>
             && is_nothrow_swappable_v<const T2>)
    requires (is_swappable_v<const T1> && is_swappable_v<const T2>) {
    adl_swap(first, x.first);
    adl_swap(second, x.second);
  }

private:
  template <class...S1, class...S2, size_t...I, size_t...J>
  constexpr tuple(piecewise_construct_t,
                  tuple<S1...> &&t1, tuple<S2...> &&t2,
                  index_sequence<I...>, index_sequence<J...>)
    : first(move(t1).template at<I>()...)
    , second(move(t2).template at<J>()...) {}
public:
  template <class...S1, class...S2>
  constexpr tuple(piecewise_construct_t, tuple<S1...> t1, tuple<S2...> t2)
    : tuple(piecewise_construct, move(t1), move(t2),
            make_index_sequence<sizeof...(S1)>(),
            make_index_sequence<sizeof...(S2)>()) {}

  template <class AL>
  constexpr
  explicit(!is_implicitly_constructible_v<T1>
           || !is_implicitly_constructible_v<T2>)
  tuple(allocator_arg_t, const AL &al)
    requires (is_default_constructible_v<T1>
              && is_default_constructible_v<T2>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al),
            uses_allocator_construction_args<T2>(al))
  {}

  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &al, const tuple &x)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, x.first),
            uses_allocator_construction_args<T2>(al, x.second)) {}
  template <class AL>
  constexpr tuple(allocator_arg_t, const AL &al, tuple &&x)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, forward<T1>(x.first)),
            uses_allocator_construction_args<T2>(al, forward<T2>(x.second))) {}

  constexpr explicit(!is_convertible_v<const T1 &, T1>
                     || !is_convertible_v<const T2 &, T2>)
  tuple(const T1 &x, const T2 &y)
    requires (is_copy_constructible_v<T1> && is_copy_constructible_v<T2>)
    : first(x), second(y) {}
  template <class AL>
  constexpr explicit(!is_convertible_v<const T1 &, T1>
                     || !is_convertible_v<const T2 &, T2>)
  tuple(allocator_arg_t, const AL &al, const T1 &x, const T2 &y)
    requires (is_copy_constructible_v<T1> && is_copy_constructible_v<T2>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, x),
            uses_allocator_construction_args<T2>(al, y)) {}

  template <class U1, class U2>
  constexpr explicit((!is_convertible_v<U1 &&, T1>
                      || !is_convertible_v<U2 &&, T2>))
  tuple(U1 &&x, U2 &&y)
    requires (is_constructible_v<T1, U1 &&> && is_constructible_v<T2, U2 &&>)
    : first(forward<U1>(x)), second(forward<U2>(y)) {}
  template <class AL, class U1, class U2>
  constexpr explicit((!is_convertible_v<U1 &&, T1>
                      || !is_convertible_v<U2 &&, T2>))
  tuple(allocator_arg_t, const AL &al, U1 &&x, U2 &&y)
    requires (is_constructible_v<T1, U1 &&> && is_constructible_v<T2, U2 &&>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, forward<U1>(x)),
            uses_allocator_construction_args<T2>(al, forward<U2>(y))) {}

  template <class U1, class U2>
  constexpr explicit(!is_convertible_v<U1 &, T1>
                     || !is_convertible_v<U2 &, T2>)
  tuple(tuple<U1, U2> &x)
    requires (is_constructible_v<T1, U1 &>
              && is_constructible_v<T2, U2 &>)
    : first(x.first), second(x.second) {}
  template <class AL, class U1, class U2>
  constexpr explicit(!is_convertible_v<U1 &, T1>
                     || !is_convertible_v<U2 &, T2>)
  tuple(allocator_arg_t, const AL &al, tuple<U1, U2> &x)
    requires (is_constructible_v<T1, U1 &>
              && is_constructible_v<T2, U2 &>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, x.first),
            uses_allocator_construction_args<T2>(al, x.second)) {}
  template <class U1, class U2>
  constexpr tuple &operator =(tuple<U1, U2> &x)
    requires (is_assignable_v<T1 &, U1 &>
              && is_assignable_v<T2 &, U2 &>) {
    first = x.first;
    second = x.second;
    return *this;
  }

  template <class U1, class U2>
  constexpr explicit(!is_convertible_v<const U1 &, T1>
                     || !is_convertible_v<const U2 &, T2>)
  tuple(const tuple<U1, U2> &x)
    requires (is_constructible_v<T1, const U1 &>
              && is_constructible_v<T2, const U2 &>)
    : first(x.first), second(x.second) {}
  template <class AL, class U1, class U2>
  constexpr explicit(!is_convertible_v<const U1 &, T1>
                     || !is_convertible_v<const U2 &, T2>)
  tuple(allocator_arg_t, const AL &al, const tuple<U1, U2> &x)
    requires (is_constructible_v<T1, const U1 &>
              && is_constructible_v<T2, const U2 &>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, x.first),
            uses_allocator_construction_args<T2>(al, x.second)) {}
  template <class U1, class U2>
  constexpr tuple &operator =(const tuple<U1, U2> &x)
    requires (is_assignable_v<T1 &, const U1 &>
              && is_assignable_v<T2 &, const U2 &>) {
    first = x.first;
    second = x.second;
    return *this;
  }

  template <class U1, class U2>
  constexpr explicit(!is_convertible_v<U1 &&, T1>
                     || !is_convertible_v<U2 &&, T2>)
  tuple(tuple<U1, U2> &&x)
    requires (is_constructible_v<T1, U1 &&> && is_constructible_v<T2, U2 &&>)
    : first(forward<U1>(x.first)), second(forward<U2>(x.second)) {}
  template <class AL, class U1, class U2>
  constexpr explicit(!is_convertible_v<U1 &&, T1>
                     || !is_convertible_v<U2 &&, T2>)
  tuple(allocator_arg_t, const AL &al, tuple<U1, U2> &&x)
    requires (is_constructible_v<T1, U1 &&> && is_constructible_v<T2, U2 &&>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>(al, forward<U1>(x.first)),
            uses_allocator_construction_args<T2>(al, forward<U2>(x.second))) {}
  template <class U1, class U2>
  constexpr tuple &operator =(tuple<U1, U2> &&x)
    requires (is_assignable_v<T1 &, U1 &&>
              && is_assignable_v<T2 &, U2 &&>) {
    first = forward<U1>(x.first);
    second = forward<U2>(x.second);
    return *this;
  }

  template <class U1, class U2>
  constexpr explicit(!is_convertible_v<U1 &&, T1>
                     || !is_convertible_v<U2 &&, T2>)
  tuple(const tuple<U1, U2> &&x)
    requires (is_constructible_v<T1, const U1 &&>
              && is_constructible_v<T2, const U2 &&>)
    : first(forward<const U1>(x.first)), second(forward<const U2>(x.second)) {}
  template <class AL, class U1, class U2>
  constexpr explicit(!is_convertible_v<const U1 &&, T1>
                     || !is_convertible_v<const U2 &&, T2>)
  tuple(allocator_arg_t, const AL &al, const tuple<U1, U2> &&x)
    requires (is_constructible_v<T1, const U1 &&>
              && is_constructible_v<T2, const U2 &&>)
    : tuple(piecewise_construct,
            uses_allocator_construction_args<T1>
            (al, forward<const U1>(x.first)),
            uses_allocator_construction_args<T2>
            (al, forward<const U2>(x.second))) {}
  template <class U1, class U2>
  constexpr tuple &operator =(const tuple<U1, U2> &&x)
    requires (is_assignable_v<T1 &, const U1 &&>
              && is_assignable_v<T2 &, const U2 &&>) {
    first = forward<const U1>(x.first);
    second = forward<const U2>(x.second);
    return *this;
  }

  template <size_t I>
  constexpr T1 &at() & noexcept
    requires (I == 0) {
    return first;
  }
  template <size_t I>
  constexpr const T1 &at() const & noexcept
    requires (I == 0) {
    return first;
  }
  template <size_t I>
  constexpr T1 &&at() && noexcept
    requires (I == 0) {
    return forward<T1>(first);
  }
  template <size_t I>
  constexpr const T1 &&at() const && noexcept
    requires (I == 0) {
    return forward<const T1>(first);
  }

  template <size_t I>
  constexpr T2 &at() & noexcept
    requires (I == 1) {
    return second;
  }
  template <size_t I>
  constexpr const T2 &at() const & noexcept
    requires (I == 1) {
    return second;
  }
  template <size_t I>
  constexpr T2 &&at() && noexcept
    requires (I == 1) {
    return forward<T2>(second);
  }
  template <size_t I>
  constexpr const T2 &&at() const && noexcept
    requires (I == 1) {
    return forward<const T2>(second);
  }

  template <class T>
  constexpr T &as() & noexcept
    requires occurs_exactly_once_v<T, T1, T2> {
    if constexpr (is_same_v<T, T1>) {
      return first;
    }
    else {
      return second;
    }
  }
  template <class T>
  constexpr const T &as() const & noexcept
    requires occurs_exactly_once_v<T, T1, T2> {
    if constexpr (is_same_v<T, T1>) {
      return first;
    }
    else {
      return second;
    }
  }
  template <class T>
  constexpr T &&as() && noexcept
    requires occurs_exactly_once_v<T, T1, T2> {
    if constexpr (is_same_v<T, T1>) {
      return forward<T>(first);
    }
    else {
      return forward<T>(second);
    }
  }
  template <class T>
  constexpr const T &&as() const && noexcept
    requires occurs_exactly_once_v<T, T1, T2> {
    if constexpr (is_same_v<T, T1>) {
      return forward<const T>(first);
    }
    else {
      return forward<const T>(second);
    }
  }
};
template <class T1, class T2>
constexpr auto swap(tuple<T1, T2> &x, tuple<T1, T2> &y)
  noexcept(noexcept(x.swap(y)))->decltype(x.swap(y)) {
  x.swap(y);
}
template <class T1, class T2>
constexpr auto swap(const tuple<T1, T2> &x, const tuple<T1, T2> &y)
  noexcept(noexcept(x.swap(y)))->decltype(x.swap(y)) {
  x.swap(y);
}
template <class T1, class T2, class T11, class T22>
constexpr bool operator ==(const tuple<T1, T2> &x,
                           const tuple<T11, T22> &y)
  requires inner::eqable<T1, T11> && inner::eqable<T2, T22> {
  return x.first == y.first && x.second == y.second;
}
template <class T1, class T2, class TT1, class TT2>
constexpr common_comparison_category_t<synth_3way_result<T1, TT1>,
                                       synth_3way_result<T2, TT2>>
operator <=>(const tuple<T1, T2> &x, const tuple<TT1, TT2> &y) {
  const auto o = synth_3way(x.first, y.first);
  if (o != 0)
    return o;
  return synth_3way(x.second, y.second);
}

inline constexpr void swap(tuple<> &, tuple<> &) noexcept {}
inline constexpr void swap(const tuple<> &, const tuple<> &) noexcept {}
inline constexpr bool operator ==(const tuple<> &, const tuple<> &) {
  return true;
}
inline constexpr strong_ordering operator <=>(const tuple<> &,
                                              const tuple<> &) {
  return strong_eq;
}

namespace inner::fns {

template <class T, class F, size_t...IDS>
constexpr auto bind_tuple_impl(T &&t, F f, index_sequence<IDS...>) {
  return tuple<decltype(f(at<IDS>(t)))...>(f(at<IDS>(t))...);
}

}
struct fo_bind_tuple {
  template <class T, class F>
  constexpr auto operator ()(T &&t, F f) const {
    return inner::fns::bind_tuple_impl(forward<T>(t), f,
                                       make_index_sequence
                                       <tuple_size_v<remove_cvref_t<T>>>{});
  }
};
inline constexpr fo_bind_tuple bind_tuple{};

}

// bind
namespace re {

namespace inner {

template <class, class, class>
class be;

template <size_t I>
struct bpos {
  static constexpr size_t nth = I;
};

}
template <class T>
struct is_bind_expression : false_type {};
template <class T>
struct is_bind_expression<const T> : is_bind_expression<T> {};
template <class T>
struct is_bind_expression<volatile T> : is_bind_expression<T> {};
template <class T>
struct is_bind_expression<const volatile T> : is_bind_expression<T> {};
template <class X, class Y, class Z>
struct is_bind_expression<inner::be<X, Y, Z>> : true_type {};
template <class T>
inline constexpr bool is_bind_expression_v = is_bind_expression<T>::value;

inline namespace placeholders {

inline constexpr inner::bpos<0> _1{};
inline constexpr inner::bpos<1> _2{};
inline constexpr inner::bpos<2> _3{};
inline constexpr inner::bpos<3> _4{};
inline constexpr inner::bpos<4> _5{};
inline constexpr inner::bpos<5> _6{};
inline constexpr inner::bpos<6> _7{};
inline constexpr inner::bpos<7> _8{};
inline constexpr inner::bpos<8> _9{};
inline constexpr inner::bpos<9> _10{};
inline constexpr inner::bpos<10> _11{};
inline constexpr inner::bpos<11> _12{};
inline constexpr inner::bpos<12> _13{};
inline constexpr inner::bpos<13> _14{};
inline constexpr inner::bpos<14> _15{};
inline constexpr inner::bpos<15> _16{};
inline constexpr inner::bpos<16> _17{};
inline constexpr inner::bpos<17> _18{};
inline constexpr inner::bpos<18> _19{};
inline constexpr inner::bpos<19> _20{};
inline constexpr inner::bpos<20> _21{};
inline constexpr inner::bpos<21> _22{};
inline constexpr inner::bpos<22> _23{};
inline constexpr inner::bpos<23> _24{};
inline constexpr inner::bpos<24> _25{};
inline constexpr inner::bpos<25> _26{};
inline constexpr inner::bpos<26> _27{};
inline constexpr inner::bpos<27> _28{};
inline constexpr inner::bpos<28> _29{};
inline constexpr inner::bpos<29> _30{};
inline constexpr inner::bpos<30> _31{};
inline constexpr inner::bpos<31> _32{};
inline constexpr inner::bpos<32> _33{};
inline constexpr inner::bpos<33> _34{};
inline constexpr inner::bpos<34> _35{};
inline constexpr inner::bpos<35> _36{};
inline constexpr inner::bpos<36> _37{};
inline constexpr inner::bpos<37> _38{};
inline constexpr inner::bpos<38> _39{};
inline constexpr inner::bpos<39> _40{};
inline constexpr inner::bpos<40> _41{};
inline constexpr inner::bpos<41> _42{};
inline constexpr inner::bpos<42> _43{};
inline constexpr inner::bpos<43> _44{};
inline constexpr inner::bpos<44> _45{};
inline constexpr inner::bpos<45> _46{};
inline constexpr inner::bpos<46> _47{};
inline constexpr inner::bpos<47> _48{};
inline constexpr inner::bpos<48> _49{};
inline constexpr inner::bpos<49> _50{};
inline constexpr inner::bpos<50> _51{};
inline constexpr inner::bpos<51> _52{};
inline constexpr inner::bpos<52> _53{};
inline constexpr inner::bpos<53> _54{};
inline constexpr inner::bpos<54> _55{};
inline constexpr inner::bpos<55> _56{};
inline constexpr inner::bpos<56> _57{};
inline constexpr inner::bpos<57> _58{};
inline constexpr inner::bpos<58> _59{};
inline constexpr inner::bpos<59> _60{};
inline constexpr inner::bpos<60> _61{};
inline constexpr inner::bpos<61> _62{};
inline constexpr inner::bpos<62> _63{};
inline constexpr inner::bpos<63> _64{};

}

template <class T>
struct is_placeholder : false_type {};
template <class T>
struct is_placeholder<const T> : is_placeholder<T> {};
template <class T>
struct is_placeholder<volatile T> : is_placeholder<T> {};
template <class T>
struct is_placeholder<const volatile T> : is_placeholder<T> {};
template <size_t I>
struct is_placeholder<inner::bpos<I>> : true_type {};
template <class T>
inline constexpr bool is_placeholder_v = is_placeholder<T>::value;

namespace inner {

struct be_auto_tag {};

struct bindimpl {
  template <size_t I, class TUPLE, class...S>
  static constexpr auto &&get(TUPLE &&x, tuple<S...> &&) {
    return at<I>(forward<TUPLE>(x));
  }
  template <size_t I, class TUPLE, class...S>
  static constexpr auto &&get(TUPLE &&, tuple<S...> &&args)
    requires is_placeholder_v<tuple_element_t<I, decay_t<TUPLE>>> {
    return at<tuple_element_t<I, decay_t<TUPLE>>::nth>(move(args));
  }
  template <size_t I, class TUPLE, class...S>
  static constexpr auto &&get(TUPLE &&x, tuple<S...> &&)
    requires is_reference_wrapper_v<tuple_element_t<I, decay_t<TUPLE>>> {
    return at<I>(forward<TUPLE>(x)).get();
  }
  template <size_t I, class TUPLE, class...S>
  static constexpr decltype(auto) get(TUPLE &&x, tuple<S...> &&args)
    requires is_bind_expression_v<tuple_element_t<I, decay_t<TUPLE>>> {
    return call(at<I>(forward<TUPLE>(x)), move(args));
  }

  template <class BINDEXPR, class...S, size_t...I>
  static constexpr decltype(auto) call_impl(BINDEXPR &&x, tuple<S...> &&args,
                                            index_sequence<I...>) {
    return invoke(x.f, get<I>(x.base(), move(args))...);
  };
  template <class BINDEXPR, class...S, size_t...I>
  static constexpr decltype(auto) call_impl(BINDEXPR &&x, tuple<S...> &&args,
                                            index_sequence<I...>)
    requires (!same_as
              <typename decay_t<BINDEXPR>::return_type,
               inner::be_auto_tag>) {
    return invoke_r<typename decay_t<BINDEXPR>::return_type>
      (x.f, get<I>(x.base(), move(args))...);
  }

  template <class BINDEXPR, class...S>
  static constexpr decltype(auto) call(BINDEXPR &&x, tuple<S...> &&args) {
    return call_impl(forward<BINDEXPR>(x), move(args),
                     make_index_sequence<x.size()>());
  }
};

template <class F, class TUPLE, class RETURN_TYPE = inner::be_auto_tag>
class be : TUPLE {
  F f;

  friend struct inner::bindimpl;

  constexpr TUPLE &base() noexcept {
    return *this;
  }
  constexpr const TUPLE &base() const noexcept {
    return *this;
  }

public:
  static constexpr size_t size() noexcept {
    return tuple_size_v<TUPLE>;
  }
  using return_type = RETURN_TYPE;

  be() = delete;
  constexpr be() noexcept(is_nothrow_default_constructible_v<F>
                          && is_nothrow_default_constructible_v<TUPLE>)
    requires (is_default_constructible_v<TUPLE>
              && is_default_constructible_v<F>)
    : TUPLE(), f() {}
  ~be() = default;
  be(const be &) = default;
  be &operator =(const be &) = default;
  be(be &&) = default;
  be &operator =(be &&) = default;
  friend constexpr void swap(be &x, be &y)
    noexcept(is_nothrow_swappable_v<F> && is_nothrow_swappable_v<TUPLE>)
    requires is_swappable_v<TUPLE> && is_swappable_v<F> {
    adl_swap(x.f, y.f);
    adl_swap(x.base(), y.base());
  }

  template <class X, class...S>
  constexpr explicit be(X &&x, S &&...s)
    requires (sizeof...(S) == size()) && is_constructible_v<F, X &&>
    : TUPLE(forward<S>(s)...), f(forward<X>(x)) {}

  template <class...S>
  constexpr decltype(auto) operator ()(S &&...s) {
    return inner::bindimpl::call(*this, forward_as_tuple(forward<S>(s)...));
  }
  template <class...S>
  constexpr decltype(auto) operator ()(S &&...s) const {
    return inner::bindimpl::call(*this, forward_as_tuple(forward<S>(s)...));
  }
};

}
struct fo_bind {
  template <class F, class...S>
  constexpr inner::be<decay_t<F>, tuple<decay_t<S>...>>
  operator ()(F &&f, S &&...s) const {
    return inner::be<decay_t<F>, tuple<decay_t<S>...>>
      (forward<F>(f), forward<S>(s)...);
  }
};
inline constexpr fo_bind bind{};
template <class R>
struct fo_bind_r {
  template <class F, class...S>
  constexpr inner::be<decay_t<F>, tuple<decay_t<S>...>, R>
  operator ()(F &&f, S &&...s) const {
    return inner::be<decay_t<F>, tuple<decay_t<S>...>, R>
      (forward<F>(f), forward<S>(s)...);
  }
};
template <class R>
inline constexpr fo_bind_r<R> bind_r{};

namespace inner {

template <class F, class...S>
class bind_front_expr {
  F f;
  tuple<S...> bound_args;

public:
  bind_front_expr() = default;
  ~bind_front_expr() = default;
  bind_front_expr(const bind_front_expr &) = default;
  bind_front_expr &operator =(const bind_front_expr &) = default;
  bind_front_expr(bind_front_expr &&) = default;
  bind_front_expr &operator =(bind_front_expr &&) = default;
  friend constexpr void swap(bind_front_expr &x, bind_front_expr &y)
    noexcept(is_nothrow_swappable_v<F>
             && is_nothrow_swappable_v<tuple<S...>>)
      requires is_swappable_v<F> && (is_swappable_v<S> && ...) {
    adl_swap(x.f, y.f);
    adl_swap(x.bound_args, y.bound_args);
  }

  template <class FF, class...SS>
  constexpr explicit bind_front_expr(FF &&ff, SS &&...s)
    : f(forward<FF>(ff)), bound_args(forward<SS>(s)...) {}

  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) & {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<F &>(f),
                      forward<SSS>(sss)..., forward<SS>(ss)...);
      }, bound_args);
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) const & {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<const F &>(f),
                      forward<SSS>(sss)..., forward<SS>(ss)...);
      }, bound_args);
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) && {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<F &&>(f),
                      forward<SSS>(sss)..., forward<SS>(ss)...);
      }, move(bound_args));
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) const && {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<const F &&>(f),
                      forward<SSS>(sss)..., forward<SS>(ss)...);
      }, move(bound_args));
  }
};

}
struct fo_bind_front {
  template <class F, class...S>
  constexpr inner::bind_front_expr<decay_t<F>, decay_t<S>...>
  operator ()(F &&f, S &&...s) const {
    return inner::bind_front_expr<decay_t<F>, decay_t<S>...>
      (forward<F>(f), forward<S>(s)...);
  }
};
inline constexpr fo_bind_front bind_front{};

namespace inner {

template <class F, class...S>
class bind_back_expr {
  F f;
  tuple<S...> bound_args;

public:
  bind_back_expr() = default;
  ~bind_back_expr() = default;
  bind_back_expr(const bind_back_expr &) = default;
  bind_back_expr &operator =(const bind_back_expr &) = default;
  bind_back_expr(bind_back_expr &&) = default;
  bind_back_expr &operator =(bind_back_expr &&) = default;
  friend constexpr void swap(bind_back_expr &x, bind_back_expr &y)
    noexcept(is_nothrow_swappable_v<F>
             && is_nothrow_swappable_v<tuple<S...>>)
      requires is_swappable_v<F> && (is_swappable_v<S> && ...) {
    adl_swap(x.f, y.f);
    adl_swap(x.bound_args, y.bound_args);
  }

  template <class FF, class...SS>
  constexpr explicit bind_back_expr(FF &&ff, SS &&...s)
    : f(forward<FF>(ff)), bound_args(forward<SS>(s)...) {}

  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) & {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<F &>(f),
                      forward<SS>(ss)..., forward<SSS>(sss)...);
      }, bound_args);
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) const & {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<const F &>(f),
                      forward<SS>(ss)..., forward<SSS>(sss)...);
      }, bound_args);
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) && {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<F &&>(f),
                      forward<SS>(ss)..., forward<SSS>(sss)...);
      }, move(bound_args));
  }
  template <class...SS>
  constexpr decltype(auto) operator ()(SS &&...ss) const && {
    return apply([&, this]<class...SSS>(SSS &&...sss)->decltype(auto) {
        return invoke(static_cast<const F &&>(f),
                      forward<SS>(ss)..., forward<SSS>(sss)...);
      }, move(bound_args));
  }
};

}
struct fo_bind_back {
  template <class F, class...S>
  constexpr inner::bind_back_expr<decay_t<F>, decay_t<S>...>
  operator ()(F &&f, S &&...s) const {
    return inner::bind_back_expr<decay_t<F>, decay_t<S>...>
      (forward<F>(f), forward<S>(s)...);
  }
};
inline constexpr fo_bind_back bind_back{};

}

// ratio
namespace re {

namespace inner::fns {

inline constexpr intmax_t constexpr_gcd(intmax_t, intmax_t);
inline constexpr intmax_t constexpr_lcm(intmax_t, intmax_t);

}
struct ratio {
  const intmax_t num;
  const intmax_t den;

private:
  static constexpr intmax_t min() {
    return integral_traits<intmax_t>::min();
  }
  static constexpr intmax_t max() {
    return integral_traits<intmax_t>::max();
  }

  static void fail();

  static constexpr intmax_t neg(intmax_t a) {
    if (a == min())
      fail();
    return -a;
  }
  static constexpr intmax_t mul(intmax_t a, intmax_t b) {
    if (a == 0 || b == 0)
      return 0;
    if (a > 0) {
      if (b > 0) {
        if (max() / b < a)
          fail();
      }
      else {
        if (b != -1 && a > min() / b)
          fail();
      }
    }
    else {
      if (b > 0) {
        if (a != -1 && b > min() / a)
          fail();
      }
      else {
        if (a == min() || b == min())
          fail();
        if (max() / (-b) < (-a))
          fail();
      }
    }
    return a * b;
  }
  static constexpr intmax_t add(intmax_t a, intmax_t b) {
    // a + b <= max() (1)
    // a + b >= min() (2)
    //
    // if (a >= 0)
    //   for (1)
    //     because
    //       min() < 0 <= (max() - a) <= max()
    //         ==> min() <= max() - a <= max()
    //           ==>
    //             (max() - a) is representable
    //       (1) <==> b <= (max() - a)
    //     (1) can be calculated by (b <= (max() - a))
    //   for (2)
    //     if (b <= 0)
    //       because
    //         min() <= b <= 0
    //           ==> min() <= min() - b <= 0 < max()
    //             ==> min() <= min() - b <= max()
    //               ==>
    //                 (min() - b) is representable
    //         (2)<==> a >= min() - b
    //       (2) can be calculated by (a >= min() - b)
    //     if (b > 0)
    //       because
    //         a >= 0, b > 0
    //           ==> a >= 0, b >= 0
    //             ==> a + b >= 0 
    //               ==> a + b >= min()
    //         (2)<==> a + b >= min()
    //       (2) is always true
    // if (a < 0)
    //   for (1)
    //     if (b >= 0)
    //       (1) can be calculated by (a <= max() - b)
    //     if (b < 0)
    //       (1) is always true
    //   for (2)
    //     because
    //       min() <= a < 0
    //         ==> min() - a >= min(), min() - a < 0
    //           ==>
    //             (min() - a) is representable
    //       (2)<==> b >= min() - a
    //     (2) can be calculated by (b >= min() - a)

    if (a >= 0) {
      if (!(b <= (max() - a)))
        fail();
      if (b <= 0) {
        if (!(a >= min() - b))
          fail();
      }
    }
    else {
      if (b >= 0) {
        if (!(a <= max() - b))
          fail();
      }
      if (!(b >= min() - a))
        fail();
    }
    return a + b;
  }
  static constexpr intmax_t sub(intmax_t a, intmax_t b) {
    if (b != min())
      return add(a, -b);
    else {
      if (a >= 0)
        fail();
      return a - b;
    }
  }

  friend constexpr intmax_t inner::fns::constexpr_gcd(intmax_t, intmax_t);
  friend constexpr intmax_t inner::fns::constexpr_lcm(intmax_t, intmax_t);
  static constexpr intmax_t gcd(intmax_t t, intmax_t s) {
    if (t < 0)
      t = neg(t);
    if (s < 0)
      s = neg(s);

    if (t == 0)
      return s;
    if (s == 0)
      return t;

    intmax_t a = 0, b = 0, c = 0;
    if (s < t) {
      a = t;
      b = s;
    }
    else {
      a = s;
      b = t;
    }
    for (;;) {
      c = a % b;
      if (c == 0)
        return b;
      a = b;
      b = c;
    }
  }
  static constexpr intmax_t lcm(intmax_t t, intmax_t s) {
    if (t < 0)
      t = neg(t);
    if (s < 0)
      s = neg(s);

    if (t == 0 || s == 0)
      return 0;

    return mul(t / gcd(t, s), s);
  }

  struct tag_t {};
  constexpr ratio(intmax_t a, intmax_t b, tag_t) : num(a), den(b) {}
  static constexpr ratio make(intmax_t a, intmax_t b) {
    return ratio(a, b, tag_t{});
  }
  static constexpr ratio make_yield(intmax_t a, intmax_t b) {
    return ratio(a, b, tag_t{}).yield();
  }

  constexpr ratio yield() {
    intmax_t n = this->num;
    intmax_t d = this->den;

    if (d == 0)
      fail();
    if ((n < 0 && d < 0) || (n >= 0 && d < 0)) {
      n = neg(n);
      d = neg(d);
    }

    if (n == 0)
      d = 1;
    else {
      const intmax_t g = gcd(n, d);
      n /= g;
      d /= g;
    }

    return make(n, d);
  }

public:
  constexpr ratio() : num(0), den(1) {}
  constexpr ratio(const ratio &) = default;
  ratio &operator =(const ratio &) = delete;
  constexpr ratio(ratio &&) = default;
  ratio &operator =(ratio &&) = delete;

  constexpr explicit ratio(intmax_t a, intmax_t b = 1)
    : ratio(make_yield(a, b)) {}

  constexpr ratio operator +() const {
    return *this;
  }
  constexpr ratio operator -() const {
    return make(neg(num), den);
  }
  constexpr ratio operator +(ratio x) const {
    const intmax_t den_lcm = lcm(den, x.den);

    const intmax_t m1 = den_lcm / den;
    const intmax_t m2 = den_lcm / x.den;

    return make_yield(add(mul(num, m1), mul(x.num, m2)), den_lcm);
  }
  constexpr ratio operator -(ratio x) const {
    const intmax_t den_lcm = lcm(den, x.den);

    const intmax_t m1 = den_lcm / den;
    const intmax_t m2 = den_lcm / x.den;

    return make_yield(sub(mul(num, m1), mul(x.num, m2)), den_lcm);
  }
  constexpr ratio operator *(ratio x) const {
    if (num == 0 || x.num == 0)
      return ratio{};

    ratio a = make_yield(num, x.den);
    ratio b = make_yield(x.num, den);

    return make(mul(a.num, b.num), mul(a.den, b.den));
  }
  constexpr ratio operator /(ratio x) const {
    if (x.num == 0)
      fail();
    intmax_t n = x.den;
    intmax_t d = x.num;
    if (d < 0) {
      d = neg(d);
      n = neg(n);
    }
    return *this * make(n, d);
  }

  friend inline constexpr bool operator ==(ratio, ratio) = default;
  friend inline constexpr strong_ordering operator <=>(ratio x, ratio y) {
    const intmax_t den_lcm = lcm(x.den, y.den);
    const intmax_t m1 = den_lcm / x.den;
    const intmax_t m2 = den_lcm / y.den;
    return mul(x.num, m1) <=> mul(y.num, m2);
  }
};
namespace inner::fns {

inline constexpr intmax_t constexpr_gcd(intmax_t a, intmax_t b) {
  return ratio::gcd(a, b);
}
inline constexpr intmax_t constexpr_lcm(intmax_t a, intmax_t b) {
  return ratio::lcm(a, b);
}

}

//inline constexpr ratio yocto(1, 1'000'000'000'000'000'000'000'000);
//inline constexpr ratio zepto(1, 1'000'000'000'000'000'000'000);
inline constexpr ratio atto(1, 1'000'000'000'000'000'000);
inline constexpr ratio femto(1, 1'000'000'000'000'000);
inline constexpr ratio pico(1, 1'000'000'000'000);
inline constexpr ratio nano(1, 1'000'000'000);
inline constexpr ratio micro(1, 1'000'000);
inline constexpr ratio milli(1, 1'000);
inline constexpr ratio centi(1, 100);
inline constexpr ratio deci(1, 10);
inline constexpr ratio deca(10, 1);
inline constexpr ratio hecto(100, 1);
inline constexpr ratio kilo(1'000, 1);
inline constexpr ratio mega(1'000'000, 1);
inline constexpr ratio giga(1'000'000'000, 1);
inline constexpr ratio tera(1'000'000'000'000, 1);
inline constexpr ratio peta(1'000'000'000'000'000, 1);
inline constexpr ratio exa(1'000'000'000'000'000'000, 1);
//inline constexpr ratio zetta(1'000'000'000'000'000'000'000, 1);
//inline constexpr ratio yotta(1'000'000'000'000'000'000'000'000, 1);

//refresh();

}

// iterator tags
namespace re {

struct output_iterator_tag {};
struct input_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};
struct contiguous_iterator_tag : random_access_iterator_tag {};

}

// optional
namespace re {

struct in_place_t {
  explicit in_place_t() = default;
};
inline constexpr in_place_t in_place{};

template <class T>
struct in_place_type_t {
  explicit in_place_type_t() = default;
};
template <class T>
inline constexpr in_place_type_t<T> in_place_type{};
template <class T>
struct is_in_place_type : false_type {};
template <class T>
struct is_in_place_type<in_place_type_t<T>> : true_type {};
template <class T>
struct is_in_place_type<const in_place_type_t<T>> : true_type {};
template <class T>
struct is_in_place_type<volatile in_place_type_t<T>> : true_type {};
template <class T>
struct is_in_place_type<const volatile in_place_type_t<T>> : true_type {};
template <class T>
inline constexpr bool is_in_place_type_v = is_in_place_type<T>::value;

struct nullopt_t {};
inline constexpr nullopt_t nullopt{};

class bad_optional_access : public exception {
public:
  virtual const char *what() const noexcept override {
    return "re::optional: try to access value of empty optional\n";
  }
};

template <class T>
class optional;

namespace inner {

struct optional_in_place_tag {};
template <class T>
union optional_storage {
  T value;
  byte placeholder;

  constexpr optional_storage() noexcept : placeholder() {}
  constexpr ~optional_storage() {}
  ~optional_storage() requires is_trivially_destructible_v<T> = default;
  optional_storage(const optional_storage &) = default;
  optional_storage &operator =(const optional_storage &) = default;
  optional_storage(optional_storage &&) = default;
  optional_storage &operator =(optional_storage &&) = default;

  template <class...S>
  constexpr optional_storage(optional_in_place_tag, S &&...s)
    : value(forward<S>(s)...) {}

  template <class...S>
  void enable(S &&...s) {
    ::new(addressof(value)) T(forward<S>(s)...);
  }
  void disable() noexcept {
    value.~T();
  }
};

template <class T>
struct is_class_optional : false_type {};
template <class T>
struct is_class_optional<optional<T>> : true_type {};

}

template <class T>
class optional {
  template <class>
  friend class optional;

  using this_t = optional;

  inner::optional_storage<T> buf;
  bool y;

  template <class U>
  void construct_from_optional(U &&x) {
    if (x.y)
      buf.enable(forward<U>(x).buf.value);
    y = x.y;
  }
  template <class U>
  this_t &assign_optional(U &&x) {
    if (y) {
      if (x.y)
        buf.value = forward<U>(x).buf.value;
      else {
        buf.disable();
        y = false;
      }
    }
    else {
      if (x.y) {
        buf.enable(forward<U>(x).buf.value);
        y = true;
      }
    }
    return *this;
  }

public:
  using value_type = T;

  constexpr optional() noexcept : buf(), y(false) {};
  ~optional() = default;
  ~optional() requires (!is_trivially_destructible_v<T>) {
    if (y)
      buf.disable();
  }
  optional(const optional &x)
    requires is_trivially_copy_constructible_v<T> = default;
  optional(const optional &x)
    requires (!is_trivially_copy_constructible_v<T>
              && is_copy_constructible_v<T>) {
    construct_from_optional(x);
  }
  optional &operator =(const optional &)
    requires (is_trivially_copy_constructible_v<T>
              && is_trivially_copy_assignable_v<T>
              && is_trivially_destructible_v<T>) = default;
  optional &operator =(const optional &x)
    requires (!(is_trivially_copy_constructible_v<T>
                && is_trivially_copy_assignable_v<T>
                && is_trivially_destructible_v<T>)
              && (is_copy_constructible_v<T> && is_copy_assignable_v<T>)) {
    return assign_optional(x);
  }
  optional(optional &&x)
    requires is_trivially_move_constructible_v<T> = default;
  optional(optional &&x) noexcept(is_nothrow_move_constructible_v<T>)
    requires (!is_trivially_move_constructible_v<T>
              && is_move_constructible_v<T>) {
    construct_from_optional(move(x));
  }
  optional &operator =(optional &&)
    requires (is_trivially_move_constructible_v<T>
              && is_trivially_move_assignable_v<T>
              && is_trivially_destructible_v<T>) = default;
  optional &operator =(optional &&x)
    noexcept(is_nothrow_move_constructible_v<T>
             && is_nothrow_move_assignable_v<T>)
    requires (!((is_trivially_move_constructible_v<T>
                 && is_trivially_move_assignable_v<T>
                 && is_trivially_destructible_v<T>))
              && (is_move_constructible_v<T> && is_move_assignable_v<T>)) {
    return assign_optional(move(x));
  }
  friend constexpr void swap(optional &a, optional &b)
    noexcept(is_nothrow_move_constructible_v<T>
             && is_nothrow_swappable_v<T>)
    requires (is_move_constructible_v<T> && is_swappable_v<T>) {
    if constexpr (is_trivially_move_constructible_v<T>
                  && is_trivially_move_assignable_v<T>) {
      default_swap(a, b);
    }
    else {
      if (a.y) {
        if (b.y)
          adl_swap(a.buf.value, b.buf.value);
        else {
          b.buf.enable(move(a.buf.value));
          a.buf.disable();
          b.y = true;
          a.y = false;
        }
      }
      else {
        if (b.y) {
          a.buf.enable(move(b.buf.value));
          b.buf.disable();
          a.y = true;
          b.y = false;
        }
      }
    }
  }

  constexpr optional(nullopt_t) noexcept : y(false) {}
  optional &operator =(nullopt_t) noexcept {
    if (y) {
      buf.disable();
      y = false;
    }
    return *this;
  }

  template <class...S>
  constexpr explicit optional(in_place_t, S &&...s)
    requires is_constructible_v<T, S &&...>
    : buf(inner::optional_in_place_tag{}, forward<S>(s)...), y(true) {}
  template <class U, class...S>
  constexpr explicit optional(in_place_t, initializer_list<U> l, S &&...s)
    requires is_constructible_v<T, initializer_list<U> &, S &&...>
    : buf(inner::optional_in_place_tag{}, l, forward<S>(s)...), y(true) {}

  template <class U = T>
  explicit(negation<is_convertible<U &&, T>>::value)
  constexpr optional(U &&x)
    requires (!is_same_v<remove_cvref_t<U>, optional>
              && !is_same_v<remove_cvref_t<U>, in_place_t>
              && is_constructible_v<T, U>)
    : buf(inner::optional_in_place_tag{}, forward<U>(x)), y(true) {}
  template <class U = T>
  optional &operator =(U &&x)
    requires (!is_same_v<remove_cvref_t<U>, optional>
              && !(is_scalar_v<T> && is_same_v<T, remove_cvref_t<U>>)
              && is_constructible_v<T, U>
              && is_assignable_v<T &, U>) {
    if (y)
      buf.value = forward<U>(x);
    else {
      buf.enable(forward<U>(x));
      y = true;
    }
    return *this;
  }

  template <class U>
  explicit(negation<is_convertible<const U &, T>>::value)
  optional(const optional<U> &x)
    requires (!is_same_v<U, T>
              && is_constructible_v<T, const U &>
              && !is_constructible_v<T, optional<U> &>
              && !is_constructible_v<T, optional<U> &&>
              && !is_constructible_v<T, const optional<U> &>
              && !is_constructible_v<T, const optional<U> &&>
              && !is_convertible_v<optional<U> &, T>
              && !is_convertible_v<optional<U> &&, T>
              && !is_convertible_v<const optional<U> &, T>
              && !is_convertible_v<const optional<U> &&, T>) {
    construct_from_optional(x);
  }
  template <class U>
  optional<T> &operator =(const optional<U> &x)
    requires (!is_same_v<U, T>
              && is_constructible_v<T, const U &>
              && is_assignable_v<T &, const U &>
              && !is_constructible_v<T, optional<U> &>
              && !is_constructible_v<T, optional<U> &&>
              && !is_constructible_v<T, const optional<U> &>
              && !is_constructible_v<T, const optional<U> &&>
              && !is_convertible_v<optional<U> &, T>
              && !is_convertible_v<optional<U> &&, T>
              && !is_convertible_v<const optional<U> &, T>
              && !is_convertible_v<const optional<U> &&, T>
              && !is_assignable_v<T &, optional<U> &>
              && !is_assignable_v<T &, optional<U> &&>
              && !is_assignable_v<T &, const optional<U> &>
              && !is_assignable_v<T &, const optional<U> &&>) {
    return assign_optional(x);
  }

  template <class U>
  explicit(negation<is_convertible<U &&, T>>::value)
  optional(optional<U> &&x)
    requires (!is_same_v<U, T>
              && is_constructible_v<T, U>
              && !is_constructible_v<T, optional<U> &>
              && !is_constructible_v<T, optional<U> &&>
              && !is_constructible_v<T, const optional<U> &>
              && !is_constructible_v<T, const optional<U> &&>
              && !is_convertible_v<optional<U> &, T>
              && !is_convertible_v<optional<U> &&, T>
              && !is_convertible_v<const optional<U> &, T>
              && !is_convertible_v<const optional<U> &&, T>) {
    construct_from_optional(move(x));
  }
  template <class U>
  optional<T> &operator =(optional<U> &&x)
    requires (!is_same_v<U, T>
              && is_constructible_v<T, U>
              && is_assignable_v<T &, U>
              && !is_constructible_v<T, optional<U> &>
              && !is_constructible_v<T, optional<U> &&>
              && !is_constructible_v<T, const optional<U> &>
              && !is_constructible_v<T, const optional<U> &&>
              && !is_convertible_v<optional<U> &, T>
              && !is_convertible_v<optional<U> &&, T>
              && !is_convertible_v<const optional<U> &, T>
              && !is_convertible_v<const optional<U> &&, T>
              && !is_assignable_v<T &, optional<U> &>
              && !is_assignable_v<T &, optional<U> &&>
              && !is_assignable_v<T &, const optional<U> &>
              && !is_assignable_v<T &, const optional<U> &&>) {
    return assign_optional(move(x));
  }

  template <class...S>
  T &emplace(S &&...s) requires (is_constructible_v<T, S &&...>) {
    operator =(nullopt);
    buf.enable(forward<S>(s)...);
    y = true;
    return buf.value;
  }
  template <class U, class...S>
  T &emplace(initializer_list<U> l, S &&...s)
    requires is_constructible_v<T, initializer_list<U> &, S &&...> {
    operator =(nullopt);
    buf.enable(l, forward<S>(s)...);
    y = true;
    return buf.value;
  }

  constexpr const T *operator ->() const noexcept {
    return addressof(buf.value);
  }
  constexpr T *operator ->() noexcept {
    return addressof(buf.value);
  }

  constexpr const T &operator *() const & noexcept {
    return buf.value;
  }
  constexpr T &operator *() & noexcept {
    return buf.value;
  }
  constexpr T &&operator *() && noexcept {
    return static_cast<T &&>(buf.value);
  }
  constexpr const T &&operator *() const && noexcept {
    return static_cast<const T &&>(buf.value);
  }

  constexpr bool empty() const noexcept {
    return !y;
  }
  constexpr void clear() noexcept {
    operator =(nullopt);
  }
  constexpr explicit operator bool() const noexcept {
    return !empty();
  }
  constexpr bool has_value() const noexcept {
    return !empty();
  }

  constexpr const T &value() const & {
    if (!y)
      throw_or_terminate<bad_optional_access>();
    return buf.value;
  }
  constexpr T &value() & {
    if (!y)
      throw_or_terminate<bad_optional_access>();
    return buf.value;
  }
  constexpr T &&value() && {
    if (!y)
      throw_or_terminate<bad_optional_access>();
    return static_cast<T &&>(buf.value);
  }
  constexpr const T &&value() const && {
    if (!y)
      throw_or_terminate<bad_optional_access>();
    return static_cast<const T &&>(buf.value);
  }

  template <class U>
  constexpr T value_or(U &&x) const &
    requires (is_copy_constructible_v<T> && is_convertible_v<U &&, T>) {
    return y ? **this : static_cast<T>(forward<U>(x));
  }
  template <class U>
  constexpr T value_or(U &&x) &&
    requires (is_move_constructible_v<T> && is_convertible_v<U &&, T>) {
    return y ? move(**this) : static_cast<T>(forward<U>(x));
  }

  constexpr void reset() noexcept {
    clear();
  }

private:
  template <class X, class F>
  static constexpr auto and_then_impl(X &&x, F &&f) {
    using t = decltype(forward<X>(x).value());
    using U = invoke_result_t<F, t>;
    if (x.has_value()) {
      return invoke(forward<F>(f), static_cast<t>(*x));
    } else {
      return remove_cvref_t<U>{};
    }
  }
  template <class X, class F>
  static constexpr auto transform_impl(X &&x, F &&f) {
    using t = decltype(forward<X>(x).value());
    using U = remove_cv_t<invoke_result_t<F, t>>;
    U u(invoke(forward<F>(f), static_cast<t>(*x)));
    if (x.has_value()) {
      return optional<U>(invoke(forward<F>(f), static_cast<t>(*x)));
    } else {
      return optional<U>{};
    }
  }
  template <class X, class F>
  static constexpr optional or_else_impl(X &&x, F &&f) {
    static_assert(is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>);
    if (x.has_value())
      return forward<X>(x);
    else
      return forward<F>(f)();
  }
public:
  template <class F>
  constexpr auto and_then(F &&f) & {
    return and_then_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto and_then(F &&f) const & {
    return and_then_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto and_then(F &&f) && {
    return and_then_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto and_then(F &&f) const && {
    return and_then_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto transform(F &&f) & {
    return transform_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto transform(F &&f) const & {
    return transform_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto transform(F &&f) && {
    return transform_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr auto transform(F &&f) const && {
    return transform_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr optional or_else(F &&f) & {
    return or_else_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr optional or_else(F &&f) const & {
    return or_else_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr optional or_else(F &&f) && {
    return or_else_impl(*this, forward<F>(f));
  }
  template <class F>
  constexpr optional or_else(F &&f) const && {
    return or_else_impl(*this, forward<F>(f));
  }

  template <class...S>
  invoke_result_t<const T &, S &&...> operator ()(S &&...s) const {
    return invoke(value(), forward<S>(s)...);
  }
};
template <class T, class U>
constexpr bool operator ==(const optional<T> &x, const optional<U> &y)
  requires can_apply_equal_to<const T &, const U &> {
  if (x.has_value() != y.has_value())
    return false;
  if (!x.has_value())
    return true;
  return *x == *y;
}
template <class T, class U>
constexpr synth_3way_result<T, U>
operator <=>(const optional<T> &x, const optional<U> &y) {
  return (x && y) ? (*x <=> *y) : (x.has_value() <=> y.has_value());
}
template <class T>
constexpr bool operator ==(const optional<T> &x, nullopt_t) noexcept {
  return !x.has_value();
}
template <class T>
constexpr strong_ordering
operator <=>(const optional<T> &x, nullopt_t) noexcept {
  return x.has_value() ? strong_gt : strong_eq;
}
template <class T, class U>
constexpr bool operator ==(const optional<T> &x, const U &y)
  requires can_apply_equal_to<const T &, const U &> {
  return x.has_value() ? *x == y : false;
}
namespace inner {

namespace fns {

void compare_optional_with_t_help_f(...);
template <class T, class U>
synth_3way_result<T, U>
compare_optional_with_t_help_f(type_tag<T>, type_tag<U>);

}

template <class T, class U>
struct compare_optional_with_t_result_type {
  using type = decltype(inner::fns::compare_optional_with_t_help_f
                        (type_tag<T>{}, type_tag<U>{}));
  static constexpr bool enabled = !is_same_v<type, void>;
};
template <class T, class U>
requires inner::is_class_optional<U>::value
struct compare_optional_with_t_result_type<T, U> {
  using type = void;
  static constexpr bool enabled = false;
};

}
template <class T, class U>
constexpr enable_if_t
<inner::compare_optional_with_t_result_type<T, U>::enabled,
 typename inner::compare_optional_with_t_result_type<T, U>::type>
operator <=>(const optional<T> &x, const U &y)
  requires (!inner::is_class_optional<U>::value) {
  return x.has_value() ? synth_3way(*x, y) : strong_lt;
}

struct fo_make_optional {
  template <class T>
  constexpr optional<decay_t<T>> operator ()(T &&x) const {
    return optional<decay_t<T>>(forward<T>(x));
  }
  template <class T, class...S>
  constexpr optional<T> operator ()(in_place_type_t<T>, S &&...s) const {
    return optional<T>(in_place, forward<S>(s)...);
  }
  template <class T, class U, class...S>
  constexpr optional<T> operator ()(in_place_type_t<T>,
                                    initializer_list<U> l, S &&...s) const {
    return optional<T>(in_place, l, forward<S>(s)...);
  }
};
inline constexpr fo_make_optional make_optional{};

}

// semiregular_function
namespace re {

template <class F>
requires is_object_v<F>
struct fo_assign_non_assignable {
  template <class X = F>
  enable_if_t<is_assignable_v<X &, const X &>>
  operator ()(F &to, const F &from) const
    noexcept(is_nothrow_assignable_v<F &, const F &>) {
    to = from;
  }
  template <class X = F>
  enable_if_t<!is_assignable_v<X &, const X &>>
  operator ()(F &to, const F &from) const
    noexcept(is_nothrow_constructible_v<F, const F &>) {
    if (addressof(to) != addressof(from)) {
      to.~F();
      ::new(addressof(to)) F(from);
    }
  }

  template <class X = F>
  enable_if_t<is_assignable_v<X &, X &&>>
  operator ()(F &to, F &&from) const
    noexcept(is_nothrow_assignable_v<F &, F &&>) {
    to = move(from);
  }
  template <class X = F>
  enable_if_t<!is_assignable_v<X &, X &&>>
  operator ()(F &to, F &&from) const
    noexcept(is_nothrow_constructible_v<F, F &&>) {
    if (addressof(to) != addressof(from)) {
      to.~F();
      ::new(addressof(to)) F(move(from));
    }
  }
};
template <class F>
inline constexpr fo_assign_non_assignable<F> assign_non_assignable{};

template <class F>
requires is_object_v<F>
class copyable_wrapper {
  template <class T>
  requires is_object_v<T>
  friend class copyable_wrapper;

  F f = F{};

public:
  copyable_wrapper() = delete;
  copyable_wrapper() requires is_default_constructible_v<F> = default;
  ~copyable_wrapper() = default;
  copyable_wrapper(const copyable_wrapper &) = default;
  copyable_wrapper(copyable_wrapper &&) = default;
  copyable_wrapper &operator =(const copyable_wrapper &) = delete;
  copyable_wrapper &operator =(copyable_wrapper &&) = delete;
  constexpr copyable_wrapper &operator =(const copyable_wrapper &x)
    noexcept(noexcept(assign_non_assignable<F>(f, x.f)))
    requires (is_copy_constructible_v<F> && !is_const_v<F>) {
    assign_non_assignable<F>(f, x.f);
    return *this;
  }
  constexpr copyable_wrapper &operator =(copyable_wrapper &&x)
    noexcept(noexcept(assign_non_assignable<F>(f, move(x).f)))
    requires (is_move_constructible_v<F> && !is_const_v<F>) {
    assign_non_assignable<F>(f, move(x).f);
    return *this;
  }
  template <class FF = F>
  friend constexpr void swap(copyable_wrapper &x,
                             copyable_wrapper &y)
    noexcept(is_nothrow_swappable_v<FF>
             || nothrow_default_swappable<copyable_wrapper<FF>>)
    requires default_swappable<copyable_wrapper<FF>> {
    if constexpr (is_nothrow_swappable_v<FF>) {
      adl_swap(x.f, y.f);
    }
    else {
      default_swap(x, y);
    }
  }

  constexpr explicit copyable_wrapper(const F &ff) : f(ff) {}
  constexpr explicit copyable_wrapper(F &&ff)
    requires (!is_same_v<F &&, const F &>) : f(forward<F>(ff)) {}

  template <class F2>
  copyable_wrapper(const copyable_wrapper<F2> &x)
    requires (!is_same_v<F, F2> && is_convertible_v<const F2 &, F>)
    : f(x.f) {}

  constexpr F *operator ->() noexcept {
    return addressof(f);
  }
  constexpr const F *operator ->() const noexcept {
    return addressof(f);
  }
  constexpr F &operator *() noexcept {
    return f;
  }
  constexpr const F &operator *() const noexcept {
    return f;
  }
};

struct fo_call_or_deref_call {
  template <class F, class...S>
  constexpr auto operator ()(F &&f, S &&...s) const
    ->decltype((*f)(forward<S>(s)...)) {
    return (*f)(forward<S>(s)...);
  }
  template <class F, class...S>
  constexpr decltype(auto) operator ()(F &&f, S &&...s) const
    requires (requires {f(static_cast<S &&>(s)...);}) {
    return f(forward<S>(s)...);
  }
};
inline constexpr fo_call_or_deref_call call_or_deref_call{};

template <class F>
class semiregular_function {
  template <class>
  friend class semiregular_function;

  using t0 = conditional_t<is_default_constructible_v<F>, F, optional<F>>;
  using t = copyable_wrapper<t0>;

  t f = t{};

public:
  semiregular_function() = default;
  ~semiregular_function() = default;
  semiregular_function(const semiregular_function &) = default;
  semiregular_function &operator =(const semiregular_function &) = default;
  semiregular_function(semiregular_function &&) = default;
  semiregular_function &operator =(semiregular_function &&) = default;
  friend constexpr void swap(semiregular_function &x, semiregular_function &y)
    noexcept(is_nothrow_swappable_v<t>) {
    adl_swap(x.f, y.f);
  }

  constexpr explicit semiregular_function(F ff) : f(ff) {}

  template <class F2>
  semiregular_function(const semiregular_function<F2> &x)
    requires (!is_same_v<F, F2> && is_convertible_v<const F2 &, F>)
    : f(x.f) {}

  template <class...S>
  constexpr auto operator ()(S &&...s)
    ->decltype(call_or_deref_call(declval<F &>(), forward<S>(s)...)) {
    return call_or_deref_call(*f, forward<S>(s)...);
  }
  template <class...S>
  constexpr auto operator ()(S &&...s) const
    ->decltype(call_or_deref_call(declval<const F &>(), forward<S>(s)...)) {
    return call_or_deref_call(*f, forward<S>(s)...);
  }

  constexpr F base() const {
    if constexpr (is_default_constructible_v<F>) {
      return *f;
    }
    else {
      return **f;
    }
  }
};

struct fo_semiregular_fn {
  template <class F>
  semiregular_function<F> operator ()(F f) const {
    return semiregular_function<F>(f);
  }
};
inline constexpr fo_semiregular_fn semiregular_fn{};

}

// hash
namespace re {

namespace inner::fns {

template <class T>
size_t integer_hash(T x)
  requires (is_integral_v<T> && sizeof(T) <= sizeof(size_t)) {
  return static_cast<size_t>(static_cast<make_unsigned_t<T>>(x));
}
template <class T>
size_t integer_hash(T x)
  requires (is_integral_v<T> && sizeof(T) == (2u * sizeof(size_t))) {
  size_t buf[2];
  memcpy(static_cast<void *>(addressof(buf)),
         static_cast<const void *>(addressof(x)),
         sizeof(T));
  return buf[0] ^ buf[1];
}

inline size_t byte_array_hash(const void *pp, size_t nn) {
  const auto f = [](const unsigned char *pos, size_t amount)->size_t {
    size_t tmp = 0;
    memcpy(static_cast<void *>(addressof(tmp)),
           static_cast<const void *>(pos),
           amount);
    return tmp;
  };

  const size_t n = nn / sizeof(size_t);
  const size_t m = nn % sizeof(size_t);
  size_t x = 0;
  const unsigned char *p = static_cast<const unsigned char *>(pp);
  const unsigned char *const p2 = p + n * sizeof(size_t);
  for (; p != p2; p += sizeof(size_t))
    x = ((size_t)(x << (size_t)5u) - x) + f(p, sizeof(size_t));
  if (m != 0u)
    x = ((size_t)(x << (size_t)5u) - x) + f(p2, m);
  return x;
}

}
template <class T = void>
struct hash {
  using argument_type = T;
  using result_type = size_t;

  hash() = delete;
  ~hash() = delete;
  hash(const hash &) = delete;
  hash &operator =(const hash &) = delete;
  hash(hash &&) = delete;
  hash &operator =(hash &&) = delete;

  size_t operator ()(const T &) const = delete;
};

namespace inner {

struct check_is_nothrow_hashable {
  template <class T, bool Y = noexcept(declval<const hash<decay_t<T>> &>()
                                       (declval<const T &>()))>
  static enable_if_t<Y> f(type_pack<T>);
};

}
template <class T>
struct is_nothrow_hashable
  : f_is_well_formed<inner::check_is_nothrow_hashable, T> {};
template <class T>
inline constexpr bool is_nothrow_hashable_v = is_nothrow_hashable<T>::value;

template <>
struct hash<void> {
  using is_transparent = inner::transparent_tag;
  using result_type = size_t;
  template <class T>
  size_t operator ()(const T &x)
    const noexcept(noexcept(hash<decay_t<T>>()(x))) {
    return hash<decay_t<T>>{}(x);
  }
};

template <>
struct hash<short> {
  using argument_type = short;
  using result_type = size_t;
  size_t operator ()(short x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<unsigned short> {
  using argument_type = unsigned short;
  using result_type = size_t;
  size_t operator ()(unsigned short x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<int> {
  using argument_type = int;
  using result_type = size_t;
  size_t operator ()(int x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<unsigned int> {
  using argument_type = unsigned int;
  using result_type = size_t;
  size_t operator ()(unsigned int x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<long> {
  using argument_type = long;
  using result_type = size_t;
  size_t operator ()(long x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<unsigned long> {
  using argument_type = unsigned long;
  using result_type = size_t;
  size_t operator ()(unsigned long x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<long long> {
  using argument_type = long long;
  using result_type = size_t;
  size_t operator ()(long long x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<unsigned long long> {
  using argument_type = unsigned long long;
  using result_type = size_t;
  size_t operator ()(unsigned long long x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<char> {
  using argument_type = char;
  using result_type = size_t;
  size_t operator ()(char x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<unsigned char> {
  using argument_type = unsigned char;
  using result_type = size_t;
  size_t operator ()(unsigned char x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<signed char> {
  using argument_type = signed char;
  using result_type = size_t;
  size_t operator ()(signed char x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<char16_t> {
  using argument_type = char16_t;
  using result_type = size_t;
  size_t operator ()(char16_t x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<char32_t> {
  using argument_type = char32_t;
  using result_type = size_t;
  size_t operator ()(char32_t x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<wchar_t> {
  using argument_type = wchar_t;
  using result_type = size_t;
  size_t operator ()(wchar_t x) const noexcept {
    return inner::fns::integer_hash(x);
  }
};
template <>
struct hash<bool> {
  using argument_type = bool;
  using result_type = size_t;
  size_t operator ()(bool x) const noexcept {
    return x ? 1u : 0u;
  }
};

template <class T>
struct hash<T *> {
  using argument_type = T *;
  using result_type = size_t;
  size_t operator ()(T *x) const noexcept {
    return hash<uintptr_t>{}(reinterpret_cast<uintptr_t>(x));
  }
};

template <>
struct hash<nullptr_t> {
  using argument_type = nullptr_t;
  using result_type = size_t;
  size_t operator ()(nullptr_t) const noexcept {
    return 0u;
  }
};

template <>
struct hash<type_index> {
  using argument_type = type_index;
  using result_type = size_t;
  size_t operator ()(type_index x) const noexcept {
    return x.hash_code();
  }
};

template <class T>
struct hash<optional<T>> : private hash<T> {
  using argument_type = optional<T>;
  using result_type = size_t;

  size_t operator ()(const optional<T> &x)
    const noexcept(is_nothrow_hashable_v<T>) {
    return x != nullopt ? hash<T>::operator ()(*x) : 0u;
  }
};

}

// miscellaneous
namespace re {

struct fo_base {
  template <class T>
  constexpr auto operator ()(T &&x) const
    ->decltype(static_cast<T &&>(x).base()) {
    return static_cast<T &&>(x).base();
  }
};
inline constexpr fo_base base{};

struct from_range_t {
  explicit from_range_t() = default;
};
inline constexpr from_range_t from_range{};

struct fo_min_value {
  template <class T, class S>
  constexpr common_type_t<T, S> operator ()(T a, S b) const {
    return b < a ? b : a;
  }
};
inline constexpr fo_min_value min_value{};

struct fo_max_value {
  template <class T, class S>
  constexpr common_type_t<T, S> operator ()(T a, S b) const {
    return a < b ? b : a;
  }
};
inline constexpr fo_max_value max_value{};

template <bool Y1, bool Y2, bool Y3, bool Y4>
struct conditionally_enable_copy_and_move;
template <>
struct conditionally_enable_copy_and_move<0, 0, 0, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<0, 0, 0, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<0, 0, 1, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<0, 0, 1, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<0, 1, 0, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<0, 1, 0, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<0, 1, 1, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<0, 1, 1, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = delete;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<1, 0, 0, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<1, 0, 0, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<1, 0, 1, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<1, 0, 1, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = delete;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<1, 1, 0, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<1, 1, 0, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = delete;
  this_t &operator =(this_t &&)                      = default;
};
template <>
struct conditionally_enable_copy_and_move<1, 1, 1, 0> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = delete;
};
template <>
struct conditionally_enable_copy_and_move<1, 1, 1, 1> {
  using this_t = conditionally_enable_copy_and_move;
  conditionally_enable_copy_and_move() = default;
  ~conditionally_enable_copy_and_move() = default;
  conditionally_enable_copy_and_move(const this_t &) = default;
  this_t &operator =(const this_t &)                 = default;
  conditionally_enable_copy_and_move(this_t &&)      = default;
  this_t &operator =(this_t &&)                      = default;
};

// inner::empty_type
namespace inner {

struct empty_type {
  explicit empty_type() = default;
  ~empty_type() = default;
  empty_type(const empty_type &) = default;
  empty_type &operator =(const empty_type &) = default;
  empty_type(empty_type &&) = default;
  empty_type &operator =(empty_type &&) = default;

  template <class...S>
  constexpr explicit empty_type(S &&...) {}
};

}

struct empty_function {
  using is_transparent = inner::transparent_tag;

  template <class...S>
  constexpr void operator ()(S &&...) const noexcept {}
};

namespace inner {

template <class F>
class exit_fn_t {
  F f;

public:
  exit_fn_t() = delete;
  constexpr ~exit_fn_t() {
    f();
  }
  exit_fn_t(const exit_fn_t &) = delete;
  exit_fn_t &operator =(const exit_fn_t &) = delete;
  exit_fn_t(exit_fn_t &&) = default;
  exit_fn_t &operator =(exit_fn_t &&) = delete;

  template <class FF>
  constexpr explicit exit_fn_t(FF &&ff) noexcept
    requires is_constructible_v<F, FF &&> : f(forward<FF>(ff)) {}
};
template <class F>
class switchable_exit_fn_t {
  using this_t = switchable_exit_fn_t;

  bool y;
  F f;

public:
  switchable_exit_fn_t() = delete;
  constexpr ~switchable_exit_fn_t() {
    if (y)
      f();
  }
  switchable_exit_fn_t(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  switchable_exit_fn_t(this_t &&) = default;
  this_t &operator =(this_t &&) = delete;

  template <class FF>
  constexpr switchable_exit_fn_t(FF &&ff, bool y = true) noexcept
    requires is_constructible_v<F, FF &&> : y(y), f(forward<FF>(ff)) {}

  constexpr explicit operator bool() const {
    return y;
  }
  constexpr void set(bool x = true) {
    y = x;
  }
  constexpr void unset() {
    y = false;
  }
};

}
struct fo_exit_fn {
  template <class F>
  constexpr inner::exit_fn_t<decay_t<F>> operator ()(F &&f) const {
    return inner::exit_fn_t<decay_t<F>>(forward<F>(f));
  }
  template <class F>
  constexpr inner::switchable_exit_fn_t<decay_t<F>> operator ()
  (F &&f, bool y) const {
    return inner::switchable_exit_fn_t<decay_t<F>>(forward<F>(f), y);
  }
};
inline constexpr fo_exit_fn exit_fn{};

struct fo_copy {
  template <class T>
  constexpr remove_cvref_t<T> operator ()(T &&x) const
    noexcept(is_nothrow_constructible_v<remove_cvref_t<T>, T &&>) {
    return remove_cvref_t<T>(forward<T>(x));
  }
  template <class A, class B>
  auto operator ()(A &&, B) const;
};
inline constexpr fo_copy copy{};
struct fo_copy_if_rvalue {
  template <class T>
  constexpr T &&operator ()(T &&x) const noexcept {
    return forward<T>(x);
  }
  template <class T>
  constexpr remove_cvref_t<T> operator ()(T &&x) const
    noexcept(is_nothrow_constructible_v<remove_cvref_t<T>, T &&>)
    requires is_rvalue_reference_v<T &&> {
    return remove_cvref_t<T>(forward<T>(x));
  }
};
inline constexpr fo_copy_if_rvalue copy_if_rvalue{};

struct fo_as_lvalue {
  template <class T>
  constexpr T &operator ()(T &&x) const noexcept {
    return x;
  }
};
inline constexpr fo_as_lvalue as_lvalue{};

struct fo_is_lvalue {
  template <class T>
  constexpr bool operator ()(T &) const {
    return true;
  }
  template <class T>
  constexpr bool operator ()(T &&) const requires (!is_reference_v<T>) {
    return false;
  }
};
inline constexpr fo_is_lvalue is_lvalue{};

struct fo_deref {
  using is_transparent = inner::transparent_tag;

  template <class X>
  constexpr auto operator ()(X &&x) const->decltype(*forward<X>(x)) {
    return *forward<X>(x);
  }
};
inline constexpr fo_deref deref{};
template <class T>
concept can_apply_deref = requires(T &&x) {
  requires is_referenceable_v<T>;
  deref(forward<T>(x));
};

struct fo_assign_to {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y) const {
    forward<Y>(y) = forward<X>(x);
  }
};
inline constexpr fo_assign_to assign_to{};

namespace inner {

template <class T>
struct check_is_allocator_aware {
  template <class TT>
  static type_tag<typename TT::allocator_type> f(type_pack<TT>);
};

}
template <class T>
struct is_allocator_aware
  : f_is_well_formed<inner::check_is_allocator_aware<T>, T> {};
template <class T>
inline constexpr bool is_allocator_aware_v = is_allocator_aware<T>::value;

template <class T>
struct fo_ccast {
  template <class X>
  constexpr T operator ()(X &&x) const {
    return (T)forward<X>(x);
  }
};
template <class T>
inline constexpr fo_ccast<T> ccast{};

template <class X>
struct fo_bit_cast {
  template <class T>
  X operator ()(const T &t) const noexcept
    requires (sizeof(T) == sizeof(X)
              && is_trivially_copyable_v<X>
              && is_trivially_copyable_v<T>) {
    X x;
    memcpy(static_cast<void *>(addressof(x)),
           static_cast<const void *>(addressof(t)),
           sizeof(X));
    return x;
  }
};
template <class X>
inline constexpr fo_bit_cast<X> bit_cast{};

struct fo_to_signed {
  template <class I>
  constexpr make_signed_t<I> operator ()(I x) const
    noexcept requires integral<I> {
    return static_cast<make_signed_t<I>>(x);
  }
};
inline constexpr fo_to_signed to_signed{};
struct fo_to_unsigned {
  template <class I>
  constexpr make_unsigned_t<I> operator ()(I x) const
    noexcept requires integral<I> {
    return static_cast<make_unsigned_t<I>>(x);
  }
};
inline constexpr fo_to_unsigned to_unsigned{};

namespace inner {

template <template <class...> class T>
struct check_is_template {
  template <class...S>
  static void f(type_tag<T<S...>>);
};

}
template <class R, template <class...> class T>
concept is_template = requires(R &&r) {
  inner::check_is_template<T>::f(type_tag<remove_cvref_t<R>>{});
};

struct fo_is_aligned {
  template <class P>
  constexpr bool operator ()(P p, size_t a) const {
    return static_cast<size_t>(reinterpret_cast<const byte *>(to_address(p))
                               - static_cast<const byte *>(nullptr))
      % a == 0u;
  }
};
inline constexpr fo_is_aligned is_aligned{};

struct fo_div_round_up {
  template<class I>
  constexpr I operator ()(I x, I y) const requires is_integral_v<I> {
    if (y == 0)
      throw_or_terminate<logic_error>("re::div_round_up(x, y): divided by 0\n");
    I r = x / y;
    if ((x % y) != 0) {
      if (r > 0)
        ++r;
      else if (r < 0)
        --r;
      else {
        if (x * y > 0)
          ++r;
        else if (x * y < 0)
          --r;
      }
    }
    return r;
  }
};
inline constexpr fo_div_round_up div_round_up{};

struct fo_div_round {
  template <class I>
  I operator ()(I a, I b) const
    requires (is_integral_v<I> && is_unsigned_v<I>) {
    if (b == 0u)
      throw_or_terminate<logic_error>("re::div_round(x, y): divided by 0\n");
    const I q = a / b;
    const I r = a % b;
    if (r >= b - r)
      return q + 1u;
    else
      return q;
  }
  template <class I>
  I operator ()(I a, I b) const
    requires (is_integral_v<I> && is_signed_v<I>) {
    if (b == 0)
      throw_or_terminate<logic_error>("re::div_round(x, y): divided by 0\n");
    const I q = a / b;
    const I r = a % b;
    if (r == 0)
      return q;
    if (b > 0) {
      if (a > 0) {
        if (r >= b - r)
          return q + 1;
        else
          return q;
      }
      else {
        if (-r >= b + r)
          return q - 1;
        else
          return q;
      }
    }
    else {
      if (a > 0) {
        if (-r <= b + r)
          return q - 1;
        else
          return q;
      }
      else {
        if (r <= b - r)
          return q + 1;
        else
          return q;
      }
    }
  }
};
inline constexpr fo_div_round div_round{};

template <class T>
struct fo_integral_cast {
  template <class U>
  constexpr T operator ()(U u) const requires integral<U> && integral<T> {
    if constexpr (is_unsigned_v<U>) {
      if (u > numeric_limits<T>::max())
        throw_or_terminate<length_error>
          ("re::integral_cast<T>(x): upper overflow\n");
    }
    else {
      if (u >= 0) {
        if (to_unsigned(u) > to_unsigned(numeric_limits<T>::max()))
          throw_or_terminate<length_error>
            ("re::integral_cast<T>(x): upper overflow\n");
      }
      else {
        if constexpr (is_unsigned_v<T>) {
          throw_or_terminate<length_error>
            ("re::integral_cast<T>(x): lower overflow\n");
        }
        else {
          if (u < numeric_limits<T>::min())
            throw_or_terminate<length_error>
              ("re::integral_cast<T>(x): lower overflow\n");
        }
      }
    }
    return static_cast<T>(u);
  }
};
template <class T>
inline constexpr fo_integral_cast<T> integral_cast{};

struct fo_call_by_n_times {
  template <class F, class X>
  constexpr X operator ()(F f, X x, int n) const {
    for (;;) {
      x = f(x);
      --n;
      if (n <= 0)
        return x;
    }
  }
};
inline constexpr fo_call_by_n_times call_by_n_times{};

template <class T>
requires is_object_v<T>
class non_propagating_cache : optional<T> {
  using base_t = optional<T>;
  using this_t = non_propagating_cache;

public:
  using value_type = T;

  non_propagating_cache() = default;
  ~non_propagating_cache() = default;
  constexpr non_propagating_cache(const this_t &) noexcept {}
  constexpr this_t &operator =(const this_t &x) noexcept {
    // if (addressof(x) != this)
    clear();
    return *this;
  }
  constexpr non_propagating_cache(this_t &&x) noexcept {
    x.clear();
  }
  constexpr this_t &operator =(this_t &&x) {
    clear();
    x.clear();
    return *this;
  }
  friend constexpr void swap(this_t &x, this_t &y) noexcept {
    x.clear();
    y.clear();
  }

  template <class...S>
  explicit non_propagating_cache(in_place_t, S &&...s)
    requires is_constructible_v<T, S &&...>
    : base_t(in_place, forward<S>(s)...) {}

  template <class...S>
  constexpr void emplace(S &&...s) {
    base_t::emplace(forward<S>(s)...);
  }
  template <class X>
  constexpr void emplace_or_assign(X &&x) {
    base_t::operator =(forward<X>(x));
  }
  constexpr void clear() noexcept {
    base_t::clear();
  }

  constexpr bool empty() const noexcept {
    return base_t::empty();
  }
  constexpr T *operator ->() noexcept {
    return base_t::operator ->();
  }
  constexpr const T *operator ->() const noexcept {
    return base_t::operator ->();
  }
  constexpr T &operator *() noexcept {
    return base_t::operator *();
  }
  constexpr const T &operator *() const noexcept {
    return base_t::operator *();
  }
};

template <class T>
class simple_wrapper {
  T v;

public:
  simple_wrapper() = delete;
  simple_wrapper() requires is_default_constructible_v<T> : v() {}
  simple_wrapper(const simple_wrapper &) = default;
  simple_wrapper &operator =(const simple_wrapper &) = default;
  simple_wrapper(simple_wrapper &&) = default;
  simple_wrapper &operator =(simple_wrapper &&) = default;
  template <class TT = T>
  friend constexpr void swap(simple_wrapper &x, simple_wrapper &y)
    noexcept(is_nothrow_swappable_v<TT>)
    requires is_swappable_v<TT> && default_swappable<simple_wrapper<TT>> {
    adl_swap(x.v, y.v);
  }

  template <class...S>
  explicit simple_wrapper(in_place_t, S &&...s)
    requires is_constructible_v<T, S &&...>
    : v(forward<S>(s)...) {}

  T *operator ->() noexcept {
    return addressof(v);
  }
  const T *operator ->() const noexcept {
    return addressof(v);
  }

  T &operator *() noexcept {
    return v;
  }
  const T &operator *() const noexcept {
    return v;
  }
};

struct fo_copy_and_swap {
  template <class T>
  constexpr void operator ()(T from, T &to) const {
    adl_swap(to, from);
  }
};
inline constexpr fo_copy_and_swap copy_and_swap{};
struct fo_copy_and_move {
  template <class T>
  constexpr void operator ()(T from, T &to) const {
    to = move(from);
  }
};
inline constexpr fo_copy_and_move copy_and_move{};

template <class T>
class derivable_wrapper : private T {
public:
  constexpr derivable_wrapper() noexcept(is_nothrow_default_constructible_v<T>)
    : T() {}
  ~derivable_wrapper() = default;
  derivable_wrapper(const derivable_wrapper &) = default;
  derivable_wrapper &operator =(const derivable_wrapper &) = default;
  derivable_wrapper(derivable_wrapper &&) = default;
  derivable_wrapper &operator =(derivable_wrapper &&) = default;
  friend constexpr void swap(derivable_wrapper &x, derivable_wrapper &y)
    noexcept(is_nothrow_swappable_v<T>)
    requires is_swappable_v<T> {
    adl_swap(*x, *y);
  }

  constexpr explicit derivable_wrapper(const T &x) : T(x) {}
  constexpr explicit derivable_wrapper(T &&x) : T(forward<T>(x)) {}
  template <class...S>
  constexpr explicit derivable_wrapper(in_place_t, S &&...s)
    : T(forward<S>(s)...) {}

  constexpr T *operator ->() noexcept {
    return static_cast<T *>(this);
  }
  constexpr const T *operator ->() const noexcept {
    return static_cast<const T *>(this);
  }
  constexpr T &operator *() noexcept {
    return static_cast<T &>(*this);
  }
  constexpr const T &operator *() const noexcept {
    return static_cast<const T &>(*this);
  }
};
template <class T>
requires (is_scalar_v<T> || is_final_v<T>)
class derivable_wrapper<T> {
  T x{};

public:
  constexpr derivable_wrapper() = default;
  ~derivable_wrapper() = default;
  derivable_wrapper(const derivable_wrapper &) = default;
  derivable_wrapper &operator =(const derivable_wrapper &) = default;
  derivable_wrapper(derivable_wrapper &&) = default;
  derivable_wrapper &operator =(derivable_wrapper &&) = default;
  friend constexpr void swap(derivable_wrapper &x, derivable_wrapper &y)
    noexcept(is_nothrow_swappable_v<T>)
    requires is_swappable_v<T> {
    adl_swap(*x, *y);
  }

  constexpr explicit derivable_wrapper(const T &xx) : x(xx) {}
  constexpr explicit derivable_wrapper(T &&xx) : x(forward<T>(xx)) {}
  template <class...S>
  constexpr explicit derivable_wrapper(in_place_t, S &&...s)
    : x(forward<S>(s)...) {}

  constexpr T *operator ->() noexcept {
    return addressof(x);
  }
  constexpr const T *operator ->() const noexcept {
    return addressof(x);
  }
  constexpr T &operator *() noexcept {
    return x;
  }
  constexpr const T &operator *() const noexcept {
    return x;
  }
};

template <class C>
struct fo_container_regular_max_size {
  constexpr typename C::size_type operator ()() const {
    using vt = typename C::value_type;
    using szt = typename C::size_type;
    using dft = typename C::difference_type;
    constexpr make_unsigned_t<ptrdiff_t> n
      = min_value(integral_traits<dft>::max(),
                  to_signed(integral_traits<ptrdiff_t>::max() / sizeof(vt)));
    constexpr szt n2 = min_value(n, integral_traits<szt>::max());
    return n2;
  }
};
template <class C>
inline constexpr fo_container_regular_max_size<C> container_regular_max_size{};

struct fo_function_return_false {
  template <class F, class...S>
  constexpr bool operator ()(F &&f, S &&...s) const {
    forward<F>(f)(forward<S>(s)...);
    return false;
  }
  template <class F, class...S>
  constexpr bool operator ()(F &&f, S &&...s) const
    requires is_same_v<decltype(f(forward<S>(s)...)), bool> {
    return !forward<F>(f)(forward<S>(s)...);
  }
};
inline constexpr fo_function_return_false function_return_false{};

template <class T>
struct fo_l_shift {
  constexpr T operator ()(T x, T y) const {
    return x << y;
  }
};
template <class T>
inline constexpr fo_l_shift<T> l_shift{};

template <class T>
struct fo_r_shift {
  constexpr T operator ()(T x, T y) const {
    return x >> y;
  }
};
template <class T>
inline constexpr fo_r_shift<T> r_shift{};

template <class T>
struct fo_b_or {
  constexpr T operator ()(T x, T y) const {
    return x | y;
  }
};
template <class T>
inline constexpr fo_b_or<T> b_or{};

template <class T>
struct fo_b_and {
  constexpr T operator ()(T x, T y) const {
    return x & y;
  }
};
template <class T>
inline constexpr fo_b_and<T> b_and{};

template <class T>
struct fo_b_xor {
  constexpr T operator ()(T x, T y) const {
    return x ^ y;
  }
};
template <class T>
inline constexpr fo_b_xor<T> b_xor{};

namespace inner::fns {

template <class T>
constexpr bool no_overflow_after_adding(T x, T y)
  requires signed_integral<T> {
  T zero{};
  if (x >= zero) {
    if (y >= zero)
      return x <= numeric_limits<T>::max() - y;
    else
      return true;
  }
  else {
    if (y >= zero)
      return true;
    else
      return x >= numeric_limits<T>::min() - y;
  }
}

template <class T>
constexpr bool no_overflow_after_adding(T x, T y)
  requires unsigned_integral<T> {
  return x <= numeric_limits<T>::max() - y;
}

}
struct fo_add_with_check {
  template <class T, class...S>
  constexpr optional<T> operator ()(T x, T y, S...s) const
    requires (integral<T> && sizeof...(S) == 0u) {
    if (!inner::fns::no_overflow_after_adding(x, y))
      return nullopt;
    else
      return optional<T>(x + y);
  }
  template <class T, class...S>
  constexpr optional<T> operator ()(T x, T y, S...s) const
    requires (integral<T> && sizeof...(S) != 0u) {
    if (!inner::fns::no_overflow_after_adding(x, y))
      return nullopt;
    else
      return operator ()(static_cast<T>(x + y), s...);
  }
};
inline constexpr fo_add_with_check add_with_check{};

struct fo_sub_to_zero_at_most {
  template <class T>
  constexpr T operator ()(T x, T y) const requires unsigned_integral<T> {
    return (x > y) ? (x - y) : 0u;
  }
  template <class T, class...S>
  constexpr T operator ()(T x, T y, S...s) const
    requires unsigned_integral<T> {
    return operator ()(operator ()(x, y), s...);
  }
};
inline constexpr fo_sub_to_zero_at_most sub_to_zero_at_most{};

template <size_t N>
using max_uint_of_max_size
  = conditional_t<sizeof(uint64_t) <= N,
                  uint64_t,
                  conditional_t<sizeof(uint32_t) <= N,
                                uint32_t,
                                conditional_t<sizeof(uint16_t) <= N,
                                              uint16_t,
                                              conditional_t<(sizeof(uint8_t)
                                                             <= N),
                                                            uint8_t,
                                                            void>>>>;

}

// numeric_limits for floating point
// floating_point_traits
// hash for floating_point
namespace re {

namespace inner {

struct float_base {
  using float_t = float;
  using uint_t = uint32_t;
  using int_t = int32_t;
  static constexpr uint_t n = 8u;
  static constexpr uint_t k = 23u;
  static constexpr uint_t bias = 0b11'11111u;
  static constexpr uint_t e_max = 0b111'11111u;
  static constexpr uint_t f_max = 0b111'11111'11111'11111'11111u;

  static float positive_inf() {
    return bit_cast<float>((uint32_t)((uint32_t)0b111'11111u << k));
  }
  static float negative_inf() {
    return bit_cast<float>((uint32_t)((uint32_t)0b1111'11111u << k));
  }
  static float positive_nan() {
    return bit_cast<float>((uint32_t)((uint32_t)0b0'111'11111'1u
                                      << (uint32_t)(k - 1u))
                           | (uint32_t)1u);
  }
  static float negative_nan() {
    return bit_cast<float>((uint32_t)((uint32_t)0b1'111'11111'1u
                                      << (uint32_t)(k - 1u))
                           | (uint32_t)1u);
  }
};
struct double_base {
  using float_t = double;
  using uint_t = uint64_t;
  using int_t = int64_t;
  static constexpr uint_t n = 11u;
  static constexpr uint_t k = 52u;
  static constexpr uint_t bias = 0b11111'11111u;
  static constexpr uint_t e_max = 0b1'11111'11111u;
  static constexpr uint_t f_max
    = 0b11'1111111111'1111111111'1111111111'1111111111'1111111111u;

  static double positive_inf() {
    return bit_cast<double>((uint64_t)((uint64_t)0b01'11111'11111u << k));
  }
  static double negative_inf() {
    return bit_cast<double>((uint64_t)((uint64_t)0b11'11111'11111u << k));
  }
  static double positive_nan() {
    return bit_cast<double>((uint64_t)((uint64_t)0b0'1'11111'11111'1u
                                       << (uint64_t)(k - 1u))
                            | (uint64_t)1u);
  }
  static double negative_nan() {
    return bit_cast<double>((uint64_t)((uint64_t)0b1'1'11111'11111'1u
                                       << (uint64_t)(k - 1u))
                            | (uint64_t)1u);
  }
};

template <class BASE>
struct floating_point_min_traits : BASE {
  using float_t = typename BASE::float_t;
  using uint_t = typename BASE::uint_t;
  using int_t = typename BASE::int_t;
  using BASE::n;
  using BASE::k;
  using BASE::bias;
  using BASE::e_max;
  using BASE::f_max;
  using BASE::positive_inf;
  using BASE::negative_inf;
  using BASE::positive_nan;
  using BASE::negative_nan;

  static bool is_positive(float_t x) {
    return (uint_t)(bit_cast<uint_t>(x) >> (uint_t)(n + k)) == 0u;
  }
  static bool is_negative(float_t x) {
    return !is_positive(x);
  }
  static uint_t e(float_t x) {
    return (uint_t)((uint_t)(bit_cast<uint_t>(x) << (uint_t)1u)
                    >> (uint_t)(k + 1u));
  }
  static uint_t f(float_t x) {
    const uint_t z = n + 1u;
    return (uint_t)((uint_t)(bit_cast<uint_t>(x) << z) >> z);
  }

  static bool is_normalized(float_t x) {
    return e(x) != 0u && e(x) != e_max;
  }
  static bool is_denormalized(float_t x) {
    return e(x) == 0u;
  }
  static bool is_infinity(float_t x) {
    return e(x) == e_max && f(x) == 0u;
  }
  static bool is_nan(float_t x) {
    return e(x) == e_max && f(x) != 0u;
  }

  static uint_t normalized_final_f(float_t x) {
    return (uint_t)((uint_t)1u << k) | f(x);
  }
  static int_t normalized_final_e(float_t x) {
    return to_signed(e(x)) - to_signed(bias) - to_signed(k);
  }

  static uint_t denormalized_final_f(float_t x) {
    return f(x);
  }
  static int_t denormalized_final_e(float_t x) {
    return 1 - to_signed(bias) - to_signed(k);
  }
};
using float_min_traits = floating_point_min_traits<float_base>;
using double_min_traits = floating_point_min_traits<double_base>;

}

template <>
struct numeric_limits<float> {
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = false;
  static constexpr float lowest() {
    return -FLT_MAX;
  }
  static constexpr float denorm_min() {
    return FLT_TRUE_MIN;
  }
  static constexpr float min() {
    return FLT_MIN;
  }
  static constexpr float max() {
    return FLT_MAX;
  }
  static float infinity() {
    return inner::float_min_traits::positive_inf();
  }
  static float nan() {
    return inner::float_min_traits::positive_nan();
  }
  static bool has_infinity(float f) {
    return inner::float_min_traits::is_infinity(f);
  }
  static bool has_denorm(float f) {
    return inner::float_min_traits::is_denormalized(f);
  }
  static bool has_nan(float f) {
    return inner::float_min_traits::is_nan(f);
  }
};
template <>
struct numeric_limits<double> {
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = false;
  static constexpr double lowest() {
    return -DBL_MAX;
  }
  static constexpr double denorm_min() {
    return DBL_TRUE_MIN;
  }
  static constexpr double min() {
    return DBL_MIN;
  }
  static constexpr double max() {
    return DBL_MAX;
  }
  static double infinity() {
    return inner::double_min_traits::positive_inf();
  }
  static double nan() {
    return inner::double_min_traits::positive_nan();
  }
  static bool has_infinity(double f) {
    return inner::double_min_traits::is_infinity(f);
  }
  static bool has_denorm(double f) {
    return inner::double_min_traits::is_denormalized(f);
  }
  static bool has_nan(double f) {
    return inner::double_min_traits::is_nan(f);
  }
};

template <class T>
struct floating_point_traits;
template <>
struct floating_point_traits<float>
  : inner::float_min_traits, numeric_limits<float> {};
template <>
struct floating_point_traits<double>
  : inner::double_min_traits, numeric_limits<double> {};

template <class T>
struct floating_point_traits<const T> : floating_point_traits<T> {};
template <class T>
struct floating_point_traits<volatile T> : floating_point_traits<T> {};
template <class T>
struct floating_point_traits<const volatile T> : floating_point_traits<T> {};

using float_traits = floating_point_traits<float>;
using double_traits = floating_point_traits<double>;

template <>
struct hash<float> {
  using argument_type = float;
  using result_type = size_t;
  size_t operator ()(float x) const noexcept {
    return hash<uint32_t>{}(bit_cast<uint32_t>(x == 0.0f ? 0.0f : x));
  }
};
template <>
struct hash<double> {
  using argument_type = double;
  using result_type = size_t;
  size_t operator ()(double x) const noexcept {
    return hash<uint64_t>{}(bit_cast<uint64_t>(x == 0.0 ? 0.0 : x));
  }
};

}

// std dependence - floating-point related functions
namespace re {

struct fo_floor {
  float operator ()(float x) const noexcept {
    return std::floor(x);
  }
  double operator ()(double x) const noexcept {
    return std::floor(x);
  }
};
inline constexpr fo_floor floor{};

struct fo_ceil {
  float operator ()(float x) const noexcept {
    return std::ceil(x);
  }
  double operator ()(double x) const noexcept {
    return std::ceil(x);
  }
};
inline constexpr fo_ceil ceil{};

struct fo_round {
  float operator ()(float x) const noexcept {
    return std::round(x);
  }
  double operator ()(double x) const noexcept {
    return std::round(x);
  }
};
inline constexpr fo_round round{};

struct fo_trunc {
  float operator ()(float x) const noexcept {
    return std::trunc(x);
  }
  double operator ()(double x) const noexcept {
    return std::trunc(x);
  }
};
inline constexpr fo_trunc trunc{};

// NAN
// INFINITY // positive infinity

// FP_INFINITE
// FP_NAN
// FP_NORMAL
// FP_SUBNORMAL
// FP_ZERO
struct fo_fpclassify {
  int operator ()(float x) const noexcept {
    return std::fpclassify(x);
  }
  int operator ()(double x) const noexcept {
    return std::fpclassify(x);
  }
};
inline constexpr fo_fpclassify fpclassify{};

struct fo_isfinite {
  bool operator ()(float x) const noexcept {
    return std::isfinite(x);
  }
  bool operator ()(double x) const noexcept {
    return std::isfinite(x);
  }
};
inline constexpr fo_isfinite isfinite{};

struct fo_isinf {
  bool operator ()(float x) const noexcept {
    return std::isinf(x);
  }
  bool operator ()(double x) const noexcept {
    return std::isinf(x);
  }
};
inline constexpr fo_isinf isinf{};

struct fo_isnan {
  bool operator ()(float x) const noexcept {
    return std::isnan(x);
  }
  bool operator ()(double x) const noexcept {
    return std::isnan(x);
  }
};
inline constexpr fo_isnan isnan{};

struct fo_isnormal {
  bool operator ()(float x) const noexcept {
    return std::isnormal(x);
  }
  bool operator ()(double x) const noexcept {
    return std::isnormal(x);
  }
};
inline constexpr fo_isnormal isnormal{};

struct fo_signbit {
  bool operator ()(float x) const noexcept {
    return std::signbit(x);
  }
  bool operator ()(double x) const noexcept {
    return std::signbit(x);
  }
};
inline constexpr fo_signbit signbit{};

struct fo_isunordered {
  bool operator ()(float x, float y) const noexcept {
    return std::isunordered(x, y);
  }
  bool operator ()(double x, double y) const noexcept {
    return std::isunordered(x, y);
  }
};
inline constexpr fo_isunordered isunordered{};

}

// common mathematical functions
namespace re {

template <class, ratio>
class duration;

struct fo_abs {
  int operator ()(int x) const noexcept {
    return std::abs(x);
  }
  long operator ()(long x) const noexcept {
    return std::abs(x);
  }
  long long operator ()(long long x) const noexcept {
    return std::abs(x);
  }
  template <class T = intmax_t>
  intmax_t operator ()(intmax_t x) const noexcept
    requires (!is_same<T, int>::value
              && !is_same<T, long>::value
              && !is_same<T, long long>::value) {
    return std::abs(x);
  }
  float operator ()(float x) const noexcept {
    return std::abs(x);
  }
  double operator ()(double x) const noexcept {
    return std::abs(x);
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.abs();} {
    return forward<T>(x).abs();
  }
};
inline constexpr fo_abs abs{};

struct fo_sqrt {
  float operator ()(float x) const noexcept {
    return std::sqrt(x);
  }
  double operator ()(double x) const noexcept {
    return std::sqrt(x);
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.sqrt();} {
    return forward<T>(x).sqrt();
  }
};
inline constexpr fo_sqrt sqrt{};

struct fo_square {
  float operator ()(float x) const noexcept {
    return x * x;
  }
  double operator ()(double x) const noexcept {
    return x * x;
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.square();} {
    return forward<T>(x).square();
  }
};
inline constexpr fo_square square{};

struct fo_sin {
  float operator ()(float x) const noexcept {
    return std::sin(x);
  }
  double operator ()(double x) const noexcept {
    return std::sin(x);
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.sin();} {
    return forward<T>(x).sin();
  }
};
inline constexpr fo_sin sin{};

struct fo_cos {
  float operator ()(float x) const noexcept {
    return std::cos(x);
  }
  double operator ()(double x) const noexcept {
    return std::cos(x);
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.cos();} {
    return forward<T>(x).cos();
  }
};
inline constexpr fo_cos cos{};

struct fo_tan {
  float operator ()(float x) const noexcept {
    return std::tan(x);
  }
  double operator ()(double x) const noexcept {
    return std::tan(x);
  }

  template <class T>
  decltype(auto) operator ()(T &&x) const requires requires {x.tan();} {
    return forward<T>(x).tan();
  }
};
inline constexpr fo_tan tan{};

struct fo_approx_equal {
  bool operator ()(float x, float y, float dif) const {
    return abs(x - y) <= dif;
  }
  bool operator ()(double x, double y, double dif) const {
    return abs(x - y) <= dif;
  }
};
inline constexpr fo_approx_equal approx_equal{};

}

#endif
