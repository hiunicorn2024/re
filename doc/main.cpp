#include <cassert>

#include <re/core>

namespace re::inner::test {

// base
void doc_base() {
  // optional macros:
  //   RE_NOEXCEPT
  //   RE_DEFAULT_ALLOCATOR re::test_allocator
  //
  // macros for inner usage:
  //   RE_DEFAULT_NEW_ALIGNMENT
  //   RE_TO_DEFINE_SIGNED_INTEGRAL_TRAITS
  //   RE_TO_DEFINE_UNSIGNED_INTEGRAL_TRAITS
  //   DEFINED_RE_..._H
  //
  // used namespaces:
  //   re
  //   re::inner
  //   re::inner::fns
  //   re::inner::test
  //   re_adl
  //   re_adl::inner
  //   re::some_namespace_name_of_std
  //
  // used names:
  //   fo_? // for defining function object
}
void doc_std_dependence() {
  // size_t = std::size_t // <cstddef>
  // ptrdiff_t = std::ptrdiff_t // <cstddef>
  // nullptr_t = std::nullptr_t // <cstddef>
  // max_align_t = std::max_align_t // <cstddef>
  // byte = std::byte // <cstddef>

  // // <cstdint>
  //
  // int_least8_t = std::int_least8_t
  // int_fast8_t = std::int_fast8_t
  // uint_least8_t = std::uint_least8_t
  // uint_fast8_t = std::uint_fast8_t
  //
  // int_least16_t = std::int_least16_t
  // int_fast16_t = std::int_fast16_t
  // uint_least16_t = std::uint_least16_t
  // uint_fast16_t = std::uint_fast16_t
  //
  // int_least32_t = std::int_least32_t
  // int_fast32_t = std::int_fast32_t
  // uint_least32_t = std::uint_least32_t
  // uint_fast32_t = std::uint_fast32_t
  //
  // int_least64_t = std::int_least64_t
  // int_fast64_t = std::int_fast64_t
  // uint_least64_t = std::uint_least64_t
  // uint_fast64_t = std::uint_fast64_t
  //
  // intmax_t = std::intmax_t
  // uintmax_t = std::uintmax_t
  //
  // intptr_t = std::intptr_t  // optional
  // uintptr_t = std::uintptr_t // optional
  //
  // int8_t = std::int8_t    // optional
  // uint8_t = std::uint8_t   // optional
  // int16_t = std::int16_t   // optional
  // uint16_t = std::uint16_t  // optional
  // int32_t = std::int32_t   // optional
  // uint32_t = std::uint32_t  // optional
  // int64_t = std::int64_t   // optional
  // uint64_t = std::uint64_t  // optional

  // initializer_list<E> = std::initializer_list<E> // <initializer_list>

  // type_info = std::type_info // <typeinfo>
  {
    static_assert(is_same<decltype(typeid(int)), const std::type_info &>);
  }

  // exception = std::exception // <exception>
  // bad_exception = std::bad_exception // <exception>
  // terminate_handler = std::terminate_handler // <exception>
  //
  // get_terminate()->terminate_handler
  //   // = std::get_terminate // <exception>
  // set_terminate(terminate_handler)->terminate_handler
  //   // = std::set_terminate // <exception>
  //
  // uncaught_exceptions()->int
  //   // = std::uncaught_exceptions // <exception>
  //
  // exception_ptr = std::exception_ptr // <exception>
  //
  // current_exception()->exception_ptr
  //   // = std::current_exception // <exception>
  // rethrow_exception(exception_ptr)
  //   // = std::rethrow_exception // <exception>
  // make_exception_ptr(E)->exception_ptr
  //   // = std::make_exception_ptr // <exception>
  //
  // nested_exception = std::nested_exception // <exception>
  // throw_with_nested(T &&) [[noreturn]]
  //   // = std::throw_with_nested // <exception>
  // rethrow_if_nested(const E &)
  //   // = std::rethrow_if_nested // <exception>

  // bad_alloc = std::bad_alloc // <new>
  // bad_array_new_length = std::bad_array_new_length // <new>
  // bad_typeid = std::bad_typeid // <typeinfo>
  // bad_cast = std::bad_cast // <typeinfo

  // logic_error = std::logic_error // <stdexcept>
  // domain_error = std::domain_error // <stdexcept>
  // invalid_argument = std::invalid_argument // <stdexcept>
  // length_error = std::length_error // <stdexcept>
  // out_of_range = std::out_of_range // <stdexcept>

  // runtime_error = std::runtime_error // <stdexcept>
  // range_error = std::range_error // <stdexcept>
  // overflow_error = std::overflow_error // <stdexcept>
  // underflow_error = std::underflow_error // <stdexcept>

  // malloc(size_t)->void * // = std::malloc // <cstdlib>
  // free(void *) // = std::free // <cstdlib>
  //
  // system(const char *)->int // = std::system // <cstdlib>
  // getenv(const char *)->char * // = std::getenv // <cstdlib>
  // abort() [[noreturn]] // = std::abort // <cstdlib>
  // exit(int) [[noreturn]] // = std::exit // <cstdlib>

  // nothrow_t = std::nothrow_t // <new>
  // std::nothrow // const nothrow_t // <new>
  // align_val_t = std::align_val_t // <new>
  // destroying_delete_t = std::destroying_delete_t // <new>

  // new_handler = void (*)() // <new>
  // get_new_handler()->new_handler // = std::get_new_handler // <new>
  // set_new_handler(new_handler)->new_handler
  //   // = std::set_new_handler // <new>

  // FILE = std::FILE // <cstdio>
  // fpos_t = std::fpos_t // <cstdio>
  // fopen(const char *fname, const char *mode)->FILE *
  //   // = std::fopen // <cstdio>
  // freopen(const char *fname, const char *mode, FILE *s)->FILE *
  //   // = std::freopen // <cstdio>
  // fclose(FILE *)->int
  //   // = std::fclose // <cstdio>
  // rename(const char *old_p, const char *new_p)->int
  //   // = std::rename // <cstdio>
  // remove(const char *fname)->int // defined in "re/algorithm.h"
  //   // = std::remove // <cstdio>
  // tmpfile()->FILE *
  //   // = std::tmpfile // <cstdio>
  // tmpnam(char *)->char *
  //   // = std::tmpnam // <cstdio>
  // setbuf(FILE *, char *buf)
  //   // = std::setbuf // <cstdio>
  // setvbuf(FILE *, char *buf, int mode, size_t sz)
  //   // = std::setvbuf // <cstdio>
  // fflush(FILE *)->int
  //   // = std::fflush // <cstdio>
  // fread(void *p, size_t sz, size_t nmemb, FILE *)->size_t
  //   // = std::fread // <cstdio>
  // fwrite(const void *p, size_t sz, size_t nmemb, FILE *)->size_t
  //   // = std::fwrite // <cstdio>

  // fgetc(FILE *)->int
  //   // = std::fgetc // <cstdio>
  // getc(FILE *)->int
  //   // = std::getc // <cstdio>
  // fputc(int c, FILE *)->int
  //   // = std::fputc // <cstdio>
  // putc(int c, FILE *)->int
  //   // = std::putc // <cstdio>
  // fputs(const char *s, FILE *)->int
  //   // = std::fputs // <cstdio>
  // getchar()->int
  //   // = std::getchar // <cstdio>
  // putchar(int c)->int
  //   // = std::putchar // <cstdio>
  // puts(const char *s)->int
  //   // = std::puts // <cstdio>
  // ungetc(int c, FILE *)->int
  //   // = std::ungetc // <cstdio>

  // fgetpos(FILE *, fpos_t *pos)->int
  //   // = std::fgetpos // <cstdio>
  // fseek(FILE *, long offset, int whence)->int
  //   // = std::fseek // <cstdio>
  // fsetpos(FILE *, fpos_t *pos)->int
  //   // = std::fsetpos // <cstdio>
  // ftell(FILE *)->long
  //   // = std::ftell // <cstdio>
  // rewind(FILE *)
  //   // = std::rewind // <cstdio>
  // clearerr(FILE *)
  //   // = std::clearerr // <cstdio>
  // feof(FILE *)->int
  //   // = std::feof // <cstdio>
  // ferror(FILE *)->int
  //   // = std::ferror // <cstdio>
  // perror(const char *)
  //   // = std::perror // <cstdio>

  // isalnum(int c)->bool // = std::isalnum // <cctype>
  // isalpha(int c)->bool // = std::isalpha // <cctype>
  // islower(int c)->bool // = std::islower // <cctype>
  // isupper(int c)->bool // = std::isupper // <cctype>
  // isdigit(int c)->bool // = std::isdigit // <cctype>
  // isxdigit(int c)->bool // = std::isxdigit // <cctype>
  // iscntrl(int c)->bool // = std::iscntrl // <cctype>
  // isgraph(int c)->bool // = std::isgraph // <cctype>
  // isspace(int c)->bool // = std::isspace // <cctype>
  // isblank(int c)->bool // = std::isblank // <cctype>
  // isprint(int c)->bool // = std::isprint // <cctype>
  // ispunct(int c)->bool // = std::ispunct // <cctype>
  // toupper(int c)->int // = std::toupper // <cctype>
  // tolower(int c)->int // = std::tolower // <cctype>

  // strcpy(char *target, const char *s)->char *
  //   // = std::strcpy // <cstring>
  // strncpy(char *target, const char *s, size_t n)->char *
  //   // = std::strncpy // <cstring>
  //
  // strcat(char *s, const char *s2)->char *
  //   // = std::strcat // <cstring>
  // strncat(char *s, const char *s2, size_t n)->char *
  //   // = std::strncat // <cstring>
  //
  // strlen(const char *s)->size_t
  //   // = std::strlen // <cstring>
  //
  // strcmp(const char *s, const char *s2)->int
  //   // = std::strcmp // <cstring>
  // strncmp(const char *s, const char *s2, size_t n)->int
  //   // = std::strncmp // <cstring>
  //
  // strspn(const char *s, const char *s2)->size_t
  //   // = std::strspn // <cstring>
  //   // return the length of the maximum span consists of only chars in s2
  // strcspn(const char *s, const char *s2)->size_t
  //   // = std::strcspn // <cstring>
  //   // return the length of the maximum span consists of chars out of s2
  //
  // strpbrk(char *s, const char *s2)->char *
  //   // = std::strpbrk // <cstring>
  //   // string pointer break: return the first position whose char is in s2
  // strpbrk(const char *s, const char *s2)->const char *
  //   // = std::strpbrk // <cstring>
  //
  // strchr(const char *, int)->const char *
  //   // = std::strchr // <cstring>
  //   // find char
  // strchr(char *, int)->char *
  //   // = std::strchr // <cstring>
  // strrchr(const char *, int)->const char *
  //   // = std::strrchr // <cstring>
  //   // find char from rightmost to leftmost
  // strrchr(char *, int)->char *
  //   // = std::strrchr // <cstring>
  //
  // strstr(const char *s, const char *s2)->const char *
  //   // = std::strstr // <cstring>
  // strstr(char *s, const char *s2)->char *
  //   // = std::strstr // <cstring>

  // memchr(const void *s, int c, size_t n)->const void *
  //   // = std::memchr // <cstring>
  // memchr(void *s, int c, size_t n)->void *
  //   // = std::memchr // <cstring>
  //
  // memcmp(const void *s, const void *s2, size_t n)->int
  //   // = std::memcmp // <cstring>
  //
  // memset(void *s, int c, size_t n)->void *
  //   // = std::memset // <cstring>
  //
  // memcpy(void *target, const void *s, size_t n)->void *
  //   // = std::memcpy // <cstring>
  // memmove(void *target, const void *s, size_t n)->void *
  //   // = std::memmove // <cstring>

  // is_constant_evaluated()
  //   // same as std::is_constant_evaluated() // <type_traits>

  // source_location // = std::source_location // <source_location>
  //   current()->source_location consteval
  //   line()->uint_least32_t
  //   column()->uint_least32_t
  //   file_name()->const char *
  //   function_name()->const char *
  {
    const source_location l = source_location::current();
    // assert(l.line() == 250);
    // assert(sview(l.file_name()) == "main.cpp");
  }

  // fgetwc(FILE *f)->wint_t
  //   // = std::fgetwc // <cwchar>
  // fgetws(wchar_t *s, int n, FILE *f)->wchar_t *
  //   // = std::fgetws // <cwchar>
  // fputwc(wchar_t c, FILE *f)->int
  //   // = std::fputwc // <cwchar>
  // fputws(const wchar_t *s, FILE *f)->int
  //   // = std::fputws // <cwchar>
  // getwchar()->wint_t
  //   // = std::getwchar // <cwchar>
  // putwchar(wchar_t c)->wint_t
  //   // = std::putwchar // <cwchar>
  // ungetwc(wint_t c, FILE *f)->wint_t
  //   // = std::ungetwc // <cwchar>
}
void doc_inner_back_door_functions() {
  // inner::make<T>(s...)
  // inner::to_mutable(x) // cast iterator to const_iterator
  // inner::node(x) // get the stored pointer from iterator
  // inner::base(x)
  // inner::good(x)
}
void doc_simple_put_functions() {
  // puti(i)
  //   // put integer to stdout, and then append an ln
  //   // for bool: print "1" for true or "0" for false
  //
  // is_little_endian
  // is_big_endian
  // putb(x)
  //   // put binary representation of integer or floating point to stdout,
  //   //   and then append an ln
  //   // e.g. putb((int8_t)-128): "10000000\n"
}
void doc_simple_mutex() {
  // simple_mutex
  //   special member functions: default-constructible
  //   lock()
  //   unlock()
}
void doc_print_error() {
  // set_error_file(FILE *) // the default error file is stderr
  //
  // print_and_terminate(const char *s)
  //   // call fputs(s), and then call terminate()
  // print_and_terminate(const char *s, ss...)
  //   // call fputs from left to right, and then call terminate()
  //
  // throw_or_terminate<E>()
  //   // if RE_NOEXCEPT is not defined, throw E{}, otherwise call terminate()
  // throw_or_terminate<E>(const char *s)
  //   // if RE_NOEXCEPT is not defined, throw E{},
  //   // otherwise call print_and_terminate(s)
}
void doc_basic_template_metaprogramming_tools() {
  // declval<T>()
  {
    static_assert(is_same<decltype(declval<int>()), int &&>);
    static_assert(is_same<decltype(declval<int &&>()), int &&>);
    static_assert(is_same<decltype(declval<int &>()), int &>);
  }

  // integer_sequence<INT, 0, 1, 2, ..., N>
  // make_integer_sequence<INT, N>
  // make_integer_sequence_by_stride<INT, N, S>
  // integer_sequence_cat<T, S...>
  // integer_sequence_offset<T, S, N>
  //
  // index_sequence<0, 1, 2, ..., N> // = integer_sequence<size_t, ...>
  // make_index_sequence<N>
  // make_index_sequence_by_stride<N, S>
  // index_sequence_cat<S...>
  // index_sequence_offset<S, N>
  {
    static_assert(is_same<integer_sequence<int, 0, 1, 2>,
                          make_integer_sequence<int, 3>>);
    static_assert(is_same<index_sequence<>, make_index_sequence<0>>);

    static_assert(is_same
                  <make_index_sequence_by_stride<3, 4>,
                   index_sequence<0, 4, 8>>);

    static_assert(is_same
                  <index_sequence_cat
                   <index_sequence<1>, index_sequence<2>, index_sequence<3, 4>>,
                   index_sequence<1, 2, 3, 4>>);

    static_assert(is_same
                  <index_sequence_offset<index_sequence<0, 1, 2>, 1>,
                   index_sequence<1, 2, 3>>);
  }

  // integral_constant<SZ_T, N>
  //   value_type
  //   value
  //   type // = type of *this
  //   operator value_type()
  //   value_type operator ()()
  // bool_constant<Y> = integral_constant<bool, Y>
  // true_type = bool_constant<true>
  // false_type = bool_constant<false>
  // size_constant<N> = integral_constant<size_t, N>
  // int_constant<N> = integral_constant<int, N>
  {
    static_assert(integral_constant<int, 3>::value == 3);
  }

  // template_f_is_well_formed<F, S...>
  // f_is_well_formed<F, S...>
  // template_return_type_of_f_or<F, T, S...>
  // return_type_of_f_or<F, T, S...>
  //   // The result is F::f(type_pack<S...>)::type if it is valid, otherwise T.
  //   // the match is precise, so there is not auto cast
  {
    struct check_t {
      static type_tag<int> f(type_pack<int>);
      static type_tag<int> f(type_pack<int, float>);
      static inner::disable f(type_pack<float>);
    };
    static_assert(f_is_well_formed<check_t, int>);
    static_assert(f_is_well_formed<check_t, int, float>);
    static_assert(!f_is_well_formed<check_t, double>);
    static_assert(is_same<return_type_of_f_or<check_t, void, int, float>, int>);
    static_assert(is_same<return_type_of_f_or<check_t, void, float>, void>);
    static_assert(is_same<return_type_of_f_or<check_t, void, double>, void>);
  }

  // template_has_member_type_type<T>
  // has_member_type_type<T>
  //
  // template_has_member_static_value<T>
  // has_member_static_value<T>
  //
  // template_member_static_value_type<T>
  //   // void if !has_member_static_value<T>
  // member_static_value_type<T>
  {
    static_assert(!has_member_type_type<int>);
    static_assert(has_member_type_type<int_constant<3>>);

    static_assert(!has_member_static_value<int>);
    static_assert(has_member_static_value<int_constant<3>>);

    static_assert(is_void<member_static_value_type<int>>);
    static_assert(is_same
                  <member_static_value_type<int_constant<3>>, const int>);
  }

  // template_nth_type<N, S...> // type is void if N >= sizeof...(S)
  // nth_type<N, S...>
  {
    static_assert(is_same<nth_type<0>, void>);
    static_assert(is_same<nth_type<100>, void>);
    static_assert(is_same<nth_type<0, char, int>, char>);
    static_assert(is_same<nth_type<1, char, int>, int>);
    static_assert(is_same<nth_type<2, char, int>, void>);
  }

  // template_is_one_of_types<T, S...>
  // is_one_of_types<T, S...>
  {
    static_assert(is_one_of_types<float, float, double>);
    static_assert(is_one_of_types<double, float, double>);
    static_assert(!is_one_of_types<int, float, double>);

    static_assert(!is_one_of_types<int>);
  }

  // template_find_type<T, S...>
  // find_type<T, S...>
  {
    static_assert(find_type<char> == 0);
    static_assert(find_type<char, int> == 1);

    static_assert(find_type<char, char, int, float> == 0);
    static_assert(find_type<int, char, int, float> == 1);
    static_assert(find_type<float, char, int, float> == 2);
    static_assert(find_type<double, char, int, float> == 3);

    static_assert(find_type<int, char, int, int, float> == 1);
  }

  // template_occurs_exactly_once<T, S...>
  // occurs_exactly_once<T, S...>
  {
    static_assert(!occurs_exactly_once<int>);
    static_assert(occurs_exactly_once<int, int>);
    static_assert(occurs_exactly_once<int, float, int, double>);
    static_assert(!occurs_exactly_once<int, int, int>);
    static_assert(!occurs_exactly_once<int, int, float, int>);
  }

  // // the following templates are designed for type-to-type calculation
  // // between types of integral_constant<...> or similar classes
  // //
  // // result value beyond minimal or maximum limit cause unknown result
  // //
  // // for XY version,
  // // the result type is common_type_t<X::value_type, Y::value_type>
  // //   // no integer promotion is performed if X is same as Y
  //
  // compile_time_add<X, Y>
  // compile_time_add_v<X, Y>
  // compile_time_add_t<X, Y>
  //
  // compile_time_sub<X, Y>
  // compile_time_sub_v<X, Y>
  // compile_time_sub_t<X, Y>
  //
  // compile_time_mul<X, Y>
  // compile_time_mul_v<X, Y>
  // compile_time_mul_t<X, Y>
  //
  // compile_time_div<X, Y> // the result is 0 if Y == 0
  // compile_time_div_v<X, Y>
  // compile_time_div_t<X, Y>
  //
  // compile_time_mod<X, Y> // Y != 0
  // compile_time_mod_v<X, Y>
  // compile_time_mod_t<X, Y>
  //
  // compile_time_neg<X>
  // compile_time_neg_v<X>
  // compile_time_neg_t<X>
  //
  // compile_time_abs<X>
  // compile_time_abs_v<X>
  // compile_time_abs_t<X>
  //
  // compile_time_gcd<X, Y>
  //   // greatest common divisor
  //   // if X::value == 0 or Y::value == 0, get the bigger one
  // compile_time_gcd_v<X, Y>
  // compile_time_gcd_t<X, Y>
  //
  // compile_time_lcm<X, Y>
  //   // least common multiple; 0 if X::value == 0 or Y::value == 0
  // compile_time_lcm_v<X, Y>
  // compile_time_lcm_t<X, Y>
  //
  // compile_time_max<X, Y> // pick X if equal
  // compile_time_max_v<X, Y>
  // compile_time_max_t<X, Y>
  //
  // compile_time_min<X, Y> // pick X if equal
  // compile_time_min_v<X, Y>
  // compile_time_min_t<X, Y>
  {
    static_assert(compile_time_add_v<int_constant<1>, int_constant<1>> == 2);
    static_assert(compile_time_sub_v<int_constant<1>, int_constant<1>> == 0);
    static_assert(compile_time_mul_v<int_constant<2>, int_constant<3>> == 6);
    static_assert(compile_time_div_v<int_constant<5>, int_constant<2>> == 2);
    static_assert(compile_time_mod_v<int_constant<5>, int_constant<2>> == 1);

    static_assert(compile_time_neg_v<int_constant<-3>> == 3);
    static_assert(compile_time_abs_v<int_constant<-3>> == 3);

    static_assert(compile_time_gcd_v<int_constant<6>, int_constant<-9>> == 3);
    static_assert(compile_time_gcd_v<size_constant<6>, size_constant<9>> == 3);
    static_assert(compile_time_gcd_v<size_constant<3>, size_constant<0>> == 3);

    static_assert(compile_time_lcm_v
                  <int_constant<6>, int_constant<-9>> == 18);
    static_assert(compile_time_lcm_v
                  <int_constant<6>, int_constant<9>> == 18);
    static_assert(compile_time_lcm_v<size_constant<6>, size_constant<0>> == 0);

    static_assert(compile_time_min_v<int_constant<1>, int_constant<2>> == 1);
    static_assert(compile_time_max_v<int_constant<1>, int_constant<2>> == 2);
  }

  // compile_time_acc<FN, FIRST, S...>
  //   // if sizeof...(S) == 0
  //   //   the member type is FIRST
  //   // else
  //   //   the member type is
  //   //   FN<...FN<FN<t1, t2>::type, t3>::type..., tn>::type
  //   //
  //   // if one of result of FN<>::type is legal but has no member type, there
  //   // is no member type
  //   //
  //   // _v version is only applied for template arguments similar to
  //   // <compile_time_add, int_constant<1>, int_constant<2>, int_constant<3>>
  //   type
  // compile_time_acc_v<FN, FIRST, S...>
  // compile_time_acc_t<FN, FIRST, S...>
  {
    static_assert(is_same
                  <compile_time_acc_t<type_pack, int, float, void *>,
                   type_pack<type_pack<int, float>::type, void *>::type>);
      // type_pack::type == type_pack
    static_assert(compile_time_acc_v<compile_time_mul,
                                     size_constant<2>,
                                     size_constant<3>,
                                     size_constant<4>> == 24);
  }

  // type_tag<T>
  //   type = T
  {
    static_assert(is_same<typename type_tag<int>::type, int>);
  }


  // // classes worked with type_pack_size and type_pack_element
  // //   type_pack<...>
  // //   T = type_pack<T>
  // //   integer_sequence<INT, ...> = type_pack<integral_constant<>...>
  // //   type_pack_add<...>
  // //   type_pack_mul<...>
  // //   type_pack_first_part<...>
  // //   type_pack_second_part<...>
  // //
  // // type_pack<A, B, C> is type pack of A, B, and C
  // // int is type pack of int
  // // index_sequence<1, 2, 3>
  // //   is type pack of size_constant<1>, ...<2>, and ...<3>
  // //
  // // type_pack<> is type pack of nothing
  // // index_sequence<> is type pack of nothing
  //
  // type_pack<X...>
  // type_pack_tag
  //   // being base class of:
  //   //   type_pack
  //   //   type_pack_add
  //   //   type_pack_mul
  //   //   type_pack_first_part
  //   //   type_pack_second_part
  {
    static_assert(is_base_of<type_pack_tag, type_pack<>>);
  }
  // template_type_pack_size<T>
  // type_pack_size<T>
  {
    static_assert(type_pack_size<int> == 1);
    static_assert(type_pack_size<type_pack<>> == 0);
    static_assert(type_pack_size<type_pack<int, float>> == 2);
    static_assert(type_pack_size<make_index_sequence<5>> == 5);
  }
  // template_type_pack_element<N, T> // get void if N >= type_pack_size<T>
  // type_pack_element<N, T>
  {
    static_assert(is_same<type_pack_element<2, index_sequence<1, 3, 5, 7>>,
                            size_constant<5>>);
  }
  // template_type_pack_eql<T1, T2>
  // type_pack_eql<T1, T2>
  {
    static_assert(type_pack_eql
                  <type_pack<int, float>,
                   type_pack_add<type_pack<int>, type_pack<float>>>);
  }
  // type_pack_for_each<T>(f) // may throw
  {
    type_pack_for_each<type_pack<int,
                                 type_pack<int, float>,
                                 make_index_sequence<3>>>
      ([]<class...S>(S...x) {
        if (sizeof...(x) == 1)
          assert((is_same<type_pack<S...>, type_pack<type_tag<int>>>));
        else if (sizeof...(x) == 2)
          assert((is_same<type_pack<S...>,
                          type_pack<type_tag<int>, type_tag<float>>>));
        else if (sizeof...(x) == 3) {
          assert((is_same<nth_type<0, S...>, type_tag<size_constant<0>>>));
          assert((is_same<nth_type<1, S...>, type_tag<size_constant<1>>>));
          assert((is_same<nth_type<2, S...>, type_tag<size_constant<2>>>));
        }
        else
          assert(false);
      });
  }
  // type_pack_for_each_until_false<T>(f) // may throw
  {
    int c = 0;
    auto f = [&](auto x) {
      const bool y = !is_pointer<typename decltype(x)::type>;
      if (y)
        ++c;
      return y;
    };
    type_pack_for_each_until_false<type_pack<int, int, int *, int>>(f);
    assert(c == 2);
  }
  // template_type_pack_apply<template <class...> APPLY, P>
  // type_pack_apply<template <class...> APPLY, P>
  {
    static_assert(is_same
                  <type_pack_apply
                   <type_pack, type_pack_add<int, type_pack<float, void *>>
                   >,
                  type_pack<int, float, void *>>);
  }
  // type_pack_add<X, Y> : type_pack_tag
  //   type = this_t // for compile_time_acc
  // type_pack_mul<X, Y> : type_pack_tag
  //   type = this_t // for compile_time_acc
  //   ()(f) // call type_pack_for_each for this_t
  {
    using P0 = type_pack<void *, void **>;
    using P1 = type_pack<int, long>;
    using P01 = type_pack_mul<P0, P1>;
    using PP = type_pack<type_pack_add<void *, int>,
                         type_pack_add<void *, long>,
                         type_pack_add<void **, int>,
                         type_pack_add<void **, long>>;
    static_assert(type_pack_eql<P01, PP>);
  }
  // template_type_pack_decay<TYPE_PACK>
  // type_pack_decay<TYPE_PACK>
  {
    static_assert(is_same
                  <type_pack_decay
                   <type_pack_add<type_pack<int>, type_pack<float>>>,
                   type_pack<int, float>>);
  }
  // template_type_pack_recursive_decay<TYPE_PACK>
  // type_pack_recursive_decay<TYPE_PACK>
  {
    using P = type_pack_mul<type_pack<void *, void **>,
                            type_pack<void ***, void ****>>;
    using PP = type_pack_recursive_decay<P>;
    using PPP = type_pack<type_pack<void *, void ***>,
                          type_pack<void *, void ****>,
                          type_pack<void **, void ***>,
                          type_pack<void **, void ****>>;
    static_assert(is_same<PP, PPP>);
  }
  // template_type_pack_recursive_eql<T1, T2>
  // type_pack_recursive_eql<T1, T2>
  {
    static_assert(type_pack_recursive_eql
                  <type_pack<type_pack<int>, float>, type_pack<int, float>>);
    static_assert(type_pack_recursive_eql
                  <type_pack<type_pack<int, long, long long>, float>,
                   type_pack<type_pack_add<type_pack_add<int, long>,
                                           type_pack<long long>>,
                             type_pack<float>>>);
  }
  // type_pack_first_part<N, P> 
  //   // P is type_pack, N <= type_pack_size<P>
  // type_pack_second_part<N, P>
  //   // P is type_pack, N <= type_pack_size<P>
  {
    using P = type_pack<int, float, void *, void **, void ***>;
    using P0 = type_pack_first_part<3, P>;
    using P1 = type_pack_second_part<3, P>;
    static_assert(type_pack_eql<P0, type_pack<int, float, void *>>);
    static_assert(type_pack_eql<P1, type_pack<void **, void ***>>);
    static_assert(is_same
                  <type_pack_decay<P1>, type_pack<void **, void ***>>);
  }
  // template_n_type_pack<N, T>
  //   // get type_pack<T, T, ..., T> // T occurs by N times
  // n_type_pack<N, T>
  {
    static_assert(same_as<n_type_pack<0, int>, type_pack<>>);
    static_assert(same_as<n_type_pack<1, int>, type_pack<int>>);
    static_assert(same_as<n_type_pack<2, int>, type_pack<int, int>>);
    static_assert(same_as<n_type_pack<3, int>, type_pack<int, int, int>>);
    static_assert(same_as
                  <n_type_pack<6, int>,
                   type_pack<int, int, int, int, int, int>>);
  }

  // conjunction<S...>
  //   // inherit from the first type whose value is false
  //   // if all is true, inherit from true_type
  // conjunction_v<S...> // get bool
  // conjunction_t<S...>
  //   // = conjunction<S...>::type
  //   // can be used to get the pure bool_constant to decrease name length
  //
  // disjunction<S...>
  //   // inherit from the first type whose value is true
  //   // if all is false, inherit from false_type
  // disjunction_v<S...> // get bool
  // disjunction_t<S...>
  //   // = disjunction<S...>::type
  //   // can be used to get the pure bool_constant to decrease name length
  //
  // negation<T> // inherit from bool_constant<!T::value>
  // negation_v<T> // get bool
  // negation_t<T>
  //   // = negation<T>::type
  //   // can be used to get the pure bool_constant to decrease name length
  {
    using t = true_type;
    using f = false_type;
    static_assert(conjunction_v<>);
    static_assert(conjunction_v<t, t, t, t, t>);
    static_assert(!disjunction_v<>);
    static_assert(!disjunction_v<f, f, f, f, f>);
    static_assert(!negation_v<t>);
    static_assert(negation_v<f>);
  }

  // template_select_type<X, S...>
  // select_type<X, S...>
  //   // use declval<X>() to join function overload resolution
  //   //   of f(X) and f(S)...;
  //   //   get index of the best one if there is a winner,
  //   //   otherwise get sizeof...(S)
  {
    static_assert(select_type<int> == 0);
    static_assert(select_type<int, const int &, int &&> == 1);
    static_assert(select_type<int &, const int &, int &&> == 0);
    static_assert(select_type<int &&, const int &, int &&> == 1);
    static_assert(select_type<float *, const int &, int &&> == 2);
  }

  // accumulate_args(add_f, ...) // may throw // can be compile time
  {
    static_assert(accumulate_args(plus<>{}, 1) == 1);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3) == 6);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3, 4, 5) == 15);
  }

  // nth_arg<I>(...) // I and args are all valid
  //   // only for immediate function call to avoid reference invalidating
  {
    static_assert(nth_arg<2>(1, 2, 3) == 3);
    static_assert(same_as<decltype(nth_arg<2>(1, 2, 3)), int &&>);
  }
}
void doc_swap() {
  // template_is_swappable_with<REF1, REF2>
  // is_swappable_with<REF1, REF2>
  // template_is_swappable<T>
  // is_swappable<T>
  // template_is_nothrow_swappable_with<REF1, REF2>
  // is_nothrow_swappable_with<REF1, REF2>
  // template_is_nothrow_swappable<T>
  // is_nothrow_swappable<T>
  {
    static_assert(is_swappable_with<int &, int &>);
    static_assert(is_swappable<int>);
    static_assert(is_nothrow_swappable_with<int &, int &>);
    static_assert(is_nothrow_swappable<int>);
  }

  // default_swap(x, y) // may throw
  //   // only use move-constructor and move-assignment operator

  // default_swappable<X>
  // adl_swap(x, y) // may throw
  {
    int x = 1, y = 2;
    int a[2] = {1, 2}, aa[2] = {4, 5};
    adl_swap(x, y);
    adl_swap(a, aa);
    assert(x == 2 && y == 1);
    assert(a[0] == 4 && a[1] == 5 && aa[0] == 1 && aa[1] == 2);
  }
}
void doc_type_traits() {
  // // primary type categories
  // template_is_void<T>
  // is_void<T>
  {
    static_assert(is_void<void>);
    static_assert(is_void<const volatile void>);
    static_assert(!is_void<void *>);
    static_assert(!is_void<int>);
  }
  // template_is_null_pointer<T>
  // is_null_pointer<T>
  {
    static_assert(is_null_pointer<nullptr_t>);
    static_assert(is_null_pointer<const volatile nullptr_t>);
    static_assert(!is_null_pointer<int>);
  }
  // template_is_integral<T>
  // is_integral<T>
  {
    static_assert(is_integral<bool>);
    static_assert(is_integral<int>);
    static_assert(is_integral<const volatile int>);
    static_assert(!is_integral<float>);
  }
  // template_is_floating_point<T>
  // is_floating_point<T>
  {
    static_assert(is_floating_point<float>);
    static_assert(is_floating_point<double>);
    static_assert(is_floating_point<const volatile double>);
    static_assert(!is_floating_point<int>);
  }
  // template_is_array<T>
  // is_array<T>
  {
    static_assert(is_array<int []>);
    static_assert(is_array<int [3]>);
    static_assert(!is_array<int (&)[3]>);
    static_assert(!is_array<int>);
  }
  // template_is_pointer<T>
  // is_pointer<T>
  {
    static_assert(is_pointer<int *>);
    static_assert(is_pointer<int *const volatile>);
    static_assert(!is_pointer<int>);
  }
  // template_is_lvalue_reference<T>
  // is_lvalue_reference<T>
  {
    static_assert(is_lvalue_reference<int &>);
    static_assert(!is_lvalue_reference<int &&>);
    static_assert(!is_lvalue_reference<int>);    
  }
  // template_is_rvalue_reference<T>
  // is_rvalue_reference<T>
  {
    static_assert(is_rvalue_reference<int &&>);
    static_assert(!is_rvalue_reference<int &>);
    static_assert(!is_rvalue_reference<int>);
  }
  // template_is_member_object_pointer<T>
  // is_member_object_pointer<T>
  {
    struct t {
      int &j;
      int k;
    };
    // auto pp = &t::j; // pointer to reference member is illegal
    auto pp = &t::k;
    static_assert(is_member_object_pointer<int t::*>);
    static_assert(!is_member_object_pointer<void (t::*)()>);
    static_assert(!is_member_object_pointer<int>);
  }
  // template_is_member_function_pointer<T>
  // is_member_function_pointer<T>
  {
    struct A {
      int f();
      int ff(int) &&;
    };
    static_assert(is_member_function_pointer<decltype(&A::f)>
                  && is_member_function_pointer<const decltype(&A::f)>
                  && is_member_function_pointer<volatile decltype(&A::f)>
                  && is_member_function_pointer<const volatile decltype(&A::f)>
                  && is_member_function_pointer<decltype(&A::ff)>
                  && is_member_function_pointer<int (A::*)()>
                  && is_member_function_pointer<int (A::*)() &&>
                  && !is_member_function_pointer<int A::*const>
                  && !is_member_function_pointer<int>);
  }
  // template_is_enum<T>
  // is_enum<T>
  {
    enum t {a, b, c};
    enum struct tt {aa, bb, cc};
    static_assert(is_enum<t>);
    static_assert(is_enum<tt>);
    static_assert(is_enum<decltype(a)>);
    static_assert(is_enum<decltype(tt::aa)>);
  }
  // template_is_union<T>
  // is_union<T>
  {
    union t {};
    static_assert(is_union<t>);
  }
  // template_is_class<T>
  // is_class<T>
  {
    struct t {};
    static_assert(is_class<t>);
  }
  // template_is_function<T>
  // is_function<T>
  {
    static_assert(is_function<void ()>);
    static_assert(is_function<void () const &>);
  }

  // // composite type categories
  // template_is_reference<T>
  // is_reference<T>
  {
    static_assert(is_reference<int &>);
    static_assert(is_reference<int &&>);
  }
  // template_is_arithmetic<T>
  // is_arithmetic<T>
  {
    static_assert(is_arithmetic<bool>);
    static_assert(is_arithmetic<char>);
    static_assert(is_arithmetic<const volatile int>);
    static_assert(is_arithmetic<float>);
  }
  // template_is_fundamental<T>
  // is_fundamental<T>
  // template_is_compound<T>
  // is_compound<T>
  {
    enum enum_t {a, b, c};
    union union_t {};
    class class_t {};
    static_assert(is_fundamental<int>);
    static_assert(is_fundamental<float>);
    static_assert(is_fundamental<void>);
    static_assert(is_fundamental<nullptr_t>);
    static_assert(is_compound<int[3]>);
    static_assert(is_compound<int *>);
    static_assert(is_compound<int &>);
    static_assert(is_compound<int ()>);
    static_assert(is_compound<enum_t>);
    static_assert(is_compound<union_t>);
    static_assert(is_compound<class_t>);
    static_assert(is_compound<int class_t::*>);
    static_assert(is_compound<int (class_t::*)()>);
  }
  // template_is_object<T>
  // is_object<T>
  {
    static_assert(!is_object<void>);
    static_assert(!is_object<int &>);
    static_assert(!is_object<int ()>);
  }
  // template_is_scalar<T>
  // is_scalar<T>
  {
    enum enum_t {};
    class t {};
    static_assert(is_scalar<int>);
    static_assert(is_scalar<float>);
    static_assert(is_scalar<nullptr_t>);
    static_assert(is_scalar<int *>);
    static_assert(is_scalar<int t::*>);
    static_assert(is_scalar<int (t::*)()>);
  }
  // template_is_member_pointer<T>
  // is_member_pointer<T>
  {
    class t {};
    static_assert(is_member_pointer<int t::*>);
    static_assert(is_member_pointer<int (t::*)()>);
  }
  // template_is_referenceable<T>
  // is_referenceable<T>
  {
    static_assert(!is_referenceable<void>);
    static_assert(!is_referenceable<void () &>);
    static_assert(!is_referenceable<void () const>);
  }

  // // type properties
  // template_is_const<T>
  // is_const<T>
  {
    static_assert(is_const<const int>);
  }
  // template_is_volatile<T>
  // is_volatile<T>
  {
    static_assert(is_volatile<volatile int>);
  }
  // template_is_trivially_copyable<T>
  // is_trivially_copyable<T>
  {
    static_assert(is_trivially_copyable<int>);
    struct t {
      t() = delete;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;
    };
    static_assert(is_trivially_copyable<t>);
  }
  // template_is_standard_layout<T>
  // is_standard_layout<T>
  {
    struct t {
      int x;
    };
    struct tt {
    private:
      int x;
    };
    struct ttt {
    private:
      int x;
    public:
      int y;
    };
    static_assert(is_standard_layout<t>);
    static_assert(is_standard_layout<tt>);
    static_assert(!is_standard_layout<ttt>);
  }
  // template_is_empty<T>
  // is_empty<T>
  {
    struct t {};
    struct tt {};
    struct ttt : t, tt {};
    static_assert(is_empty<t>);
    static_assert(is_empty<tt>);
    static_assert(is_empty<ttt>);
  }
  // template_is_polymorphic<T>
  // is_polymorphic<T>
  // template_is_abstract<T>
  // is_abstract<T>
  {
    struct t {virtual void f() = 0;};
    struct tt {virtual void f() {}};
    static_assert(is_polymorphic<t>);
    static_assert(is_polymorphic<tt>);
    static_assert(is_abstract<t>);
    static_assert(!is_abstract<tt>);
  }
  // template_is_final<T>
  // is_final<T>
  {
    struct t final {};
    static_assert(is_final<t>);
  }
  // template_is_aggregate<T>
  // is_aggregate<T>
  {
    struct t {
      int x;
      float y;
    };
    static_assert(is_aggregate<t>);
  }
  // template_is_signed<T>
  // is_signed<T>
  // template_is_unsigned<T>
  // is_unsigned<T>
  {
    static_assert(is_signed<int>);
    static_assert(is_unsigned<bool>);
    static_assert(!is_unsigned<void> && !is_signed<void>);
  }
  // template_is_bounded_array<T>
  // is_bounded_array<T>
  // template_is_unbounded_array<T>
  // is_unbounded_array<T>
  {
    static_assert(is_bounded_array<int [1]>);
    static_assert(is_unbounded_array<int []>);
  }
  // template_is_constructible<T, S...>
  // is_constructible<T, S...>
  // template_is_default_constructible<T>
  // is_default_constructible<T>
  // template_is_copy_constructible<T>
  // is_copy_constructible<T>
  // template_is_move_constructible<T>
  // is_move_constructible<T>
  // template_is_assignable<L_REF, R_REF>
  // is_assignable<L_REF, R_REF>
  // template_is_copy_assignable<T>
  // is_copy_assignable<T>
  // template_is_move_assignable<T>
  // is_move_assignable<T>
  // template_is_destructible<T>
  // is_destructible<T>

  // template_is_trivially_constructible<T, S...>
  // is_trivially_constructible<T, S...>
  // template_is_trivially_default_constructible<T>
  // is_trivially_default_constructible<T>
  // template_is_trivially_copy_constructible<T>
  // is_trivially_copy_constructible<T>
  // template_is_trivially_move_constructible<T>
  // is_trivially_move_constructible<T>
  // template_is_trivially_assignable<L_REF, R_REF>
  // is_trivially_assignable<L_REF, R_REF>
  // template_is_trivially_copy_assignable<T>
  // is_trivially_copy_assignable<T>
  // template_is_trivially_move_assignable<T>
  // is_trivially_move_assignable<T>
  // template_is_trivially_destructible<T>
  // is_trivially_destructible<T>

  // template_is_nothrow_constructible<T, S...>
  // is_nothrow_constructible<T, S...>
  // template_is_nothrow_default_constructible<T>
  // is_nothrow_default_constructible<T>
  // template_is_nothrow_copy_constructible<T>
  // is_nothrow_copy_constructible<T>
  // template_is_nothrow_move_constructible<T>
  // is_nothrow_move_constructible<T>
  // template_is_nothrow_assignable<L_REF, R_REF>
  // is_nothrow_assignable<L_REF, R_REF>
  // template_is_nothrow_copy_assignable<T>
  // is_nothrow_copy_assignable<T>
  // template_is_nothrow_move_assignable<T>
  // is_nothrow_move_assignable<T>
  // template_is_nothrow_destructible<T>
  // is_nothrow_destructible<T>

  // template_has_virtual_destructor<T>
  // has_virtual_destructor<T>
  {
    struct t {virtual ~t() {}};
    static_assert(has_virtual_destructor<t>);
  }

  // template_has_unique_object_representations<T>
  // has_unique_object_representations<T>

  // // type property queires
  // template_alignment_of<T>
  // alignment_of<T>
  {
    static_assert(alignment_of<char> == alignof(char));
  }

  // template_rank<T>
  // rank<T>
  {
    static_assert(rank<int> == 0u);
    static_assert(rank<int []> == 1u);
    static_assert(rank<int [][2]> == 2u);
    static_assert(rank<int [][2][3]> == 3u);
  }
  // template_extent<T, I = 0>
  // extent<T, I = 0>
  {
    static_assert(extent<int> == 0);
    static_assert(extent<int []> == 0);
    static_assert(extent<int [3]> == 3);
    static_assert(extent<int [3][4]> == 3);
    static_assert(extent<int [3][4], 0> == 3);
    static_assert(extent<int [3][4], 1> == 4);
  }

  // // type relations
  // template_is_same<T1, T2>
  // is_same<T1, T2>
  {
    static_assert(is_same<void, void>);
  }
  // template_is_base_of<T1, T2>
  // is_base_of<T1, T2>
  {
    struct t {};
    struct tt : t {};
    struct ttt : private t {};
    static_assert(!is_base_of<int, int>);
    static_assert(is_base_of<t, t>);
    static_assert(is_base_of<t, tt>);
    static_assert(is_base_of<t, ttt>);
  }
  // template_is_convertible<T1, T2>
  // is_convertible<T1, T2>
  {
    static_assert(is_convertible<int &, const int &>);
    static_assert(!is_convertible<const int &, int &>);
  }
  // template_is_nothrow_convertible<T1, T2>
  // is_nothrow_convertible<T1, T2>
  {
    struct t {};
    struct tt {tt(t) {}};
    static_assert(is_nothrow_convertible<int &, const int &>);
    static_assert(!is_nothrow_convertible<t, tt>);
    static_assert(!is_nothrow_convertible<const int &, int &>);
  }

  // // const-volatile modifications
  // template_remove_const<T>
  // remove_const<T>
  // template_remove_volatile<T>
  // remove_volatile<T>
  // template_remove_cv<T>
  // remove_cv<T>
  // template_add_const<T>
  // add_const<T>
  // template_add_volatile<T>
  // add_volatile<T>
  // template_add_cv<T>
  // add_cv<T>
  {
    static_assert(is_same<remove_const<const int>, int>);
    static_assert(is_same<remove_cv<const int>, int>);
    static_assert(is_same<remove_const<int &>, int &>);

    static_assert(is_same<remove_volatile<volatile int>, int>);
    static_assert(is_same<remove_cv<volatile int>, int>);
    static_assert(is_same<remove_volatile<volatile int &>, volatile int &>);

    static_assert(is_same<remove_cv<const volatile int>, int>);
    static_assert(is_same
                  <remove_cv<const volatile int &>, const volatile int &>);

    static_assert(is_same<add_const<int>, const int>);
    static_assert(is_same<add_const<int &>, int &>);
    static_assert(is_same<add_volatile<int>, volatile int>);
    static_assert(is_same<add_volatile<int &>, int &>);
    static_assert(is_same<add_cv<int>, const volatile int>);
    static_assert(is_same<add_cv<int &>, int &>);
  }

  // // reference modifications
  // template_remove_reference<T>
  // remove_reference<T>
  // template_add_lvalue_reference<T>
  // add_lvalue_reference<T>
  // template_add_rvalue_reference<T>
  // add_rvalue_reference<T>
  {
    static_assert(is_same<remove_reference<int &&>, int>);
    static_assert(is_same<add_lvalue_reference<int &&>, int &>);
    static_assert(is_same<add_rvalue_reference<int &&>, int &&>);
  }

  // // sign modifications
  // template_make_signed<T>
  // make_signed<T>
  // template_make_unsigned<T>
  // make_unsigned<T>
  {
    static_assert(is_same<make_signed<unsigned>, int>);
    static_assert(is_same<make_unsigned<int>, unsigned>);
    static_assert(is_same<make_unsigned<unsigned>, unsigned>);
  }

  // // array modifications
  // template_remove_extent<T>
  // remove_extent<T>
  // template_remove_all_extents<T>
  // remove_all_extents<T>
  {
    static_assert(is_same<remove_extent<int>, int>);
    static_assert(is_same<remove_extent<int []>, int>);
    static_assert(is_same<remove_extent<int [][3]>, int[3]>);

    static_assert(is_same<remove_all_extents<int [2][3]>, int>);
  }

  // // pointer modifications
  // template_remove_pointer<T>
  // remove_pointer<T>
  // template_add_pointer<T>
  // add_pointer<T>
  {
    static_assert(is_same<remove_pointer<int>, int>);
    static_assert(is_same<remove_pointer<int *>, int>);

    static_assert(is_same<add_pointer<int>, int *>);
    static_assert(is_same<add_pointer<int &>, int *>);

    static_assert(is_same<add_pointer<void>, void *>);
    static_assert(is_same<add_pointer<void ()>, void (*)()>);
    static_assert(is_same<add_pointer<void () &&>, void () &&>);
  }

  // // other transformations
  // template_type_identity<T>
  // type_identity<T>
  {
    static_assert(is_same<type_identity<int>, int>);
  }
  // template_remove_cvref<T>
  // remove_cvref<T>
  {
    static_assert(is_same<remove_cvref<const volatile int &>, int>);
  }
  // template_decay<T>
  // decay<T>
  {
    static_assert(is_same<decay<int [2][3]>, int (*)[3]>);
    static_assert(is_same<decay<int []>, int *>);
    static_assert(is_same<decay<int ()>, int (*)()>);
    static_assert(is_same<decay<int (&&)()>, int (*)()>);
    static_assert(is_same<decay<const int &>, int>);
  }
  // template_enable_if<T>
  // enable_if<T>
  {
    static_assert(is_same<enable_if<true, int>, int>);
    static_assert(is_same<enable_if<true>, void>);

    static_assert(!has_member_type_type<template_enable_if<false>>);
  }
  // template_conditional<Y, T1, T2>
  // conditional<Y, T1, T2>
  {
    static_assert(is_same<conditional<true, char, int>, char>);
    static_assert(is_same<conditional<false, char, int>, int>);
  }
  // template_common_type<...>
  // common_type<...>
  {
    static_assert(!has_member_type_type<template_common_type<>>);
    static_assert(is_same<common_type<char, char>, char>);
    static_assert(is_same<common_type<short, char>, int>);
    static_assert(is_same<common_type<char, unsigned>, unsigned>);
    static_assert(is_same<common_type<float, double>, double>);
    static_assert(is_same<common_type<int, float>, float>);
    static_assert(is_same<common_type<volatile int &&, const float &>, float>);
    struct t {};
    struct tt : t {};
    struct ttt : tt {};
    static_assert(is_same<common_type<t, tt, ttt>, t>);
  }
  // template_basic_common_reference<T, U, <class> TQUAL, <class> UQUAL>
  //   // no contents
  //   // make user defined specialization to provide type member
  // template_commmon_reference<S...>
  // commmon_reference<S...>
  {
    static_assert(is_same
                  <common_reference<volatile char &, const char &>,
                   const volatile char &>);
    struct t {};
    struct tt : t {};
    static_assert(is_same
                  <common_reference<volatile t &, const tt &>,
                   const volatile t &>);

    struct s {};
    struct ss {operator s();};
    struct sss : s {};
    static_assert(is_same<common_reference<s &, ss &>, s>);
    static_assert(is_same<common_reference<s &, sss &>, s &>);
  }
  // template_underlying_type<X> // if X is not enum, there is no member type
  // underlying_type<X>
  {
    enum t {a, b, c};
    static_assert(is_integral<underlying_type<t>>);
  }
  // void_t<S...>
  {
    static_assert(is_void<void_t<int, float, void *>>);
  }

  // template_uses_allocator<T, ALLOC>
  // uses_allocator<T, ALLOC>
  {
    struct X {
      using allocator_type = float;
    };
    static_assert(uses_allocator<X, float>);
    static_assert(uses_allocator<X, int>);
    static_assert(uses_allocator<X, char>);
    static_assert(!uses_allocator<X, void *>);
    static_assert(!uses_allocator<X, void>);
  }

  // template_max_align_of_types<S...>
  // max_align_of_types<S...>
  // template_max_size_of_types<S...>
  // max_size_of_types<S...>
  {
    static_assert(max_align_of_types<> == 1u);
    static_assert(max_align_of_types<char, long double, int>
                  == alignof(long double));

    static_assert(max_size_of_types<> == 0u);
    static_assert(max_size_of_types<int, long double, char>
                  == sizeof(long double));
  }
  // type_t<T, S...>
  {
    static_assert(is_void<type_t<void>>);
    static_assert(is_void<type_t<void, int, float, void *>>);
    static_assert(is_same<type_t<int>, int>);
    static_assert(is_same<type_t<int, int, float, void *>, int>);
  }
  // template_is_implicitly_constructible<T, S...>
  // is_implicitly_constructible<T, S...>
  // template_is_implicitly_default_constructible<T>
  // is_implicitly_default_constructible<T>
  {
    struct t {t();};
    struct t2 {explicit t2();};
    static_assert(is_implicitly_constructible<t>);
    static_assert(!is_implicitly_constructible<t2>);
    static_assert(is_implicitly_default_constructible<t>);
    static_assert(!is_implicitly_default_constructible<t2>);
  }
  // template_copy_const<FROM, TO>
  // copy_const<FROM, TO>
  // template_copy_volatile<FROM, TO>
  // copy_volatile<FROM, TO>
  // template_copy_lvalue_reference<FROM, TO>
  // copy_lvalue_reference<FROM, TO>
  // template_copy_rvalue_reference<FROM, TO>
  // copy_rvalue_reference<FROM, TO>
  // template_copy_reference<FROM, TO>
  // copy_reference<FROM, TO>
  // template_copy_cv<FROM, TO>
  // copy_cv<FROM, TO>
  // template_copy_cvref<FROM, TO>
  // copy_cvref<FROM, TO>
  {
    static_assert(is_same<copy_const<const int, char>, const char>);
    static_assert(is_same<copy_const<int, const char>, const char>);

    static_assert(is_same
                  <copy_cvref<const volatile int &, char>,
                   const volatile char &>);
    static_assert(is_same
                  <copy_cvref<const volatile int &, void>,
                   const volatile void>);
  }

  // template_is_nothrow_movable<T>
  // is_nothrow_movable<T>
  // template_is_nothrow_copyable<T>
  // is_nothrow_copyable<T>
  {
    static_assert(is_nothrow_movable<unique_ptr<int>>);
    static_assert(!is_nothrow_copyable<unique_ptr<int>>);
    static_assert(is_nothrow_copyable<int>);
  }

  // template_is_compatible_pointer_with<Y, T>
  // is_compatible_pointer_with<Y, T>
  {
    struct t {};
    struct tt : t {};

    static_assert(is_compatible_pointer_with<t *, const volatile t *>);

    static_assert(is_compatible_pointer_with<tt *, t *>);
    static_assert(is_compatible_pointer_with<tt *, const volatile t *>);

    static_assert(!is_compatible_pointer_with<tt (*)[], t (*)[]>);
    static_assert(is_compatible_pointer_with<t (*)[3], t (*)[]>);
    static_assert(is_compatible_pointer_with<t (*)[3], const volatile t (*)[]>);

    static_assert(!is_compatible_pointer_with<tt (*)[3], t (*)[3]>);
    static_assert(is_compatible_pointer_with<t (*)[3], t (*)[3]>);
  }
}
void doc_basic_utility_functions() {
  // forward<T>(x)->T &&
  // move(T &&)->remove_reference<T> &&
  // move_if_noexcept(T &)->{T && or const T &}

  // exchange(old_val, new_val)->old_val // may throw

  // as_const(x)->add_const<...> &

  // // cmp_***: only apply for integral argument
  // cmp_equal(x, y)->bool
  // cmp_not_equal(x, y)->bool
  // cmp_less(x, y)->bool
  // cmp_less_equal(x, y)->bool
  // cmp_greater(x, y)->bool
  // cmp_greater_equal(x, y)->bool
  // in_range<INT>(x)->bool // return true if INT can represent int of x
  {
    static_assert(cmp_less(-1, 1ull));
    static_assert(in_range<signed char>(127));
    static_assert(!in_range<signed char>(128));
    static_assert(in_range<signed char>(-128));
    static_assert(!in_range<signed char>(-129));
  }

  // to_underlying(e)->some_int // e is enum
  {
    enum class t : char {x, y, z};
    static_assert(is_same<decltype(to_underlying(t::x)), char>);
    assert(to_underlying(t::x) == 0);
  }

  // addressof(l_ref)->ptr
  //   // no constexpr if user-defined operator-> is selected
  {
    int x{};
    const volatile int y{};
    assert(addressof(x) == &x);
    assert(addressof(y) == &y);
  }

  // align(size_t a, size_t sz, void *&p, size_t &n)->void *
  //   // a is 2 ^ n, a > 0
  //   // if failed
  //   //   return nullptr, do nothing for p and n
  //   // else
  //   //   update p and return p, n -= space_for_align
  {
    unsigned char *const p0{};
    struct input_t {
      void *p;
      size_t n;

      bool operator ==(const input_t &x) const {return p == x.p && n == x.n;}
    };
    {
      {
        // 0 1 2 3 4 {5 6 7 [8 9]}
        // update i for 0-byte space aligned by 4
        input_t i(p0 + 5, 5);
        assert(align(4, 0, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 {5 6 7 [8 9]}
        // update i for 2-byte space aligned by 4
        input_t i(p0 + 5, 5);
        assert(align(4, 2, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 {5 6 7 8 9}
        // do nothing because no enough space for 3-byte space aligned by 4
        input_t i(p0 + 5, 5);
        assert(align(4, 3, i.p, i.n) == nullptr);
        assert(i == input_t(p0 + 5, 5));
      }
    }
  }

  // is_sufficiently_aligned<N>(T *)->bool
  {
    struct alignas(128) t {};
    t x;
    assert(is_sufficiently_aligned<128>(&x));
  }
}
void doc_pointer_traits() {
  // pointer_traits<P>
  //   pointer
  //   element_type
  //   difference_type
  //   rebind<X>
  //   pointer_to(x)->ptr_rebind<P, remove_reference<x_t>>
  // ptr_element<P> = pointer_traits<P>::element_type
  // ptr_dft<P> = pointer_traits<P>::difference_type
  // ptr_rebind<P, X> = pointer_traits<P>::rebind<X>
  // pointer_to<P>(x)->pointer_traits<P>::rebind<remove_reference<x_t>>
  {
    using pt = pointer_traits<const int *>;
    static_assert(is_same<pt::pointer, const int *>);
    static_assert(is_same<pt::element_type, const int>);
    static_assert(is_same<pt::difference_type, ptrdiff_t>);
    static_assert(is_same<pt::rebind<char>, char *>);
  }

  // to_address(p)->real_p
  // can_apply_to_address<T>
  {
    struct t {
      int *operator ->() const {return nullptr;}
    };
    assert(to_address(t{}) == nullptr);

    static_assert(can_apply_to_address<int *>);
    static_assert(!can_apply_to_address<int>);
  }
}
void doc_reference_wrapper_and_invoke() {
  // reference_wrapper<T>
  //   special member functions: no default constructor
  //   reference_wrapper(U &&)
  //   get()->T &
  //   ()(s...)->invoke_result<...> // may throw
  // reference_wrapper(T &)->reference_wrapper<T>
  // template_is_reference_wrapper<T>
  // is_reference_wrapper<T>
  // template_unwrap_reference<T>
  // unwrap_reference<T>
  //   // reference_wrapper<int> -> int &
  //   // cv reference_wrapper<int> -> unchanged type
  // template_unwrap_ref_decay<T>
  // unwrap_ref_decay<T> = unwrap_reference<decay<T>>
  // ref(x)->reference_wrapper<...>
  // cref(x)->reference_wrapper<const ...>
  {
    static_assert(is_reference_wrapper<reference_wrapper<int>>);

    static_assert(same_as
                  <unwrap_reference<const reference_wrapper<int>>,
                   const reference_wrapper<int>>);
    static_assert(same_as
                  <unwrap_reference<reference_wrapper<int>>,
                   int &>);

    static_assert(same_as
                  <unwrap_ref_decay<const reference_wrapper<int>>,
                   int &>);
  }

  // template_is_invocable<F, S...>
  // is_invocable<F, S...>
  // template_is_nothrow_invocable<F, S...>
  // is_nothrow_invocable<F, S...>
  // template_invoke_result<F, S...>
  // invoke_result<F, S...>
  // invoke(f, s...) // may throw
  //
  // template_is_invocable_r<R, F, S...>
  // is_invocable_r<R, F, S...>
  // template_is_nothrow_invocable_r<R, F, S...>
  // is_nothrow_invocable_r<R, F, S...>
  // invoke_r<R>(f, s...) // may throw
  {
    struct t {
      int value = 1;
      int f(int x, int y) {return x * 100 + y * 10 + value;}
    };
    t x{};
    assert(invoke(&t::value, x) == 1);
    assert(invoke(&t::value, ref(x)) == 1);
    assert(invoke(&t::value, addressof(x)) == 1);
    assert(invoke(&t::f, x, 3, 2) == 321);
    assert(invoke(&t::f, ref(x), 3, 2) == 321);
    assert(invoke(&t::f, addressof(x), 3, 2) == 321);
    assert(invoke([]() {return 0;}) == 0);
    assert(invoke([](int x, int y) {return x * 10 + y;}, 1, 2) == 12);
  }
}
void doc_language_related_concepts() {
  // same_as<A, B>
  {
    static_assert(same_as<void, void>);
  }
  // derived_from<DERIVED, BASE>
  {
    struct t {};
    struct tt : t {};
    struct ttt : private t {};
    static_assert(derived_from<tt, t>);
    static_assert(!derived_from<ttt, t>);
  }
  // convertible_to<FROM, TO>
  {
    static_assert(convertible_to<int *, void *>);
  }
  // common_reference_with<A, B>
  {
    struct t {};
    struct tt : t {};
    struct ttt : private t {};
    static_assert(common_reference_with<t &, t &>);
    static_assert(common_reference_with<t &, tt &>);
    static_assert(!common_reference_with<t &, ttt &>);
  }
  // common_with<A, B>
  {
    static_assert(common_with<int, int>);
  }
  // integral<T>
  // signed_integral<T>
  // unsigned_integral<T>
  // floating_point<T>
  {
    static_assert(integral<bool>);
    static_assert(unsigned_integral<bool>);
  }
  // assignable_from<L_REF, R_REF>
  // destructible<T>
  // constructible_from<T, S...>
  // default_initializable<T>
  // move_constructible<T>
  // copy_constructible<T>
  //
  // ranges::swap(x, y)
  // swappable<T>
  // swappable_with<A, B>
  // // is_swappable and is_swappable_with are both improved
  // //   from std versions,
  // //   and they are similar as swappable and swappable_with but a bit relaxed
  // // note: I prefer is_swappable, is_swappable_with, and adl_swap(x, y)
  {
    int x = 1, y = 2;
    ranges::swap(x, y);
    assert(x == 2 && y == 1);
    adl_swap(x, y);
    assert(x == 1 && y == 2);

    static_assert(swappable<int>);
    static_assert(swappable<int &>);
    static_assert(swappable_with<int &, int &>);
  }
  // equality_comparable<T>
  // equality_comparable_with<A, B>
  {
    static_assert(equality_comparable<int>);
    static_assert(equality_comparable<int ()>);
    static_assert(equality_comparable_with<int, float>);
  }
  // totally_ordered<T>
  // totally_ordered_with<A, B>
  {
    static_assert(totally_ordered<int>);
    static_assert(totally_ordered_with<int &, const int &>);
    struct t {};
    static_assert(!totally_ordered<t>);
    static_assert(!totally_ordered_with<const t &, t &>);
  }
  // movable<T> = is_object<T> && move_constructible<T>
  //   && assignable_from<T &, T> && swappable<T>
  // copyable<T> = copy_constructible<T> && movable<T>
  //   && assignable_from<T &, T &>
  //   && assignable_from<T &, const T &>
  //   && assignable_from<T &, const T>
  // semiregular<T> // have all special member functions
  // regular<T> // semiregular + equality_comparable
  {
    static_assert(movable<int>);
    static_assert(copyable<int>);
    static_assert(semiregular<int>);
    static_assert(regular<int>);
  }
  // invocable<F, S...> // same as is_invocable
  // // the following concepts are all not useful,
  // //   because in std they are just used to make ranges-functions look cute
  // //   but the only force constrait is just invocable
  // regular_invocable<F, S...>
  // predicate<F, S...>
  // relation<R, A, B>
  // equivalence_relation<R, A, B>
  // strict_weak_order<R, A, B>
  {
    static_assert(invocable<int (*)()>);
    static_assert(invocable<int (*)(int), int>);
  }
}
void doc_concept_constrained_comparisons() {
  // // concept-constrained comparisons are constrained by concept
  // //   like equality_comparable_with and so on
  // ranges::equal_to
  //   ()(i, ii)
  // ranges::not_equal_to
  //   ()(i, ii)
  // ranges::less
  //   ()(i, ii)
  // ranges::less_equal
  //   ()(i, ii)
  // ranges::greater
  //   ()(i, ii)
  // ranges::greater_equal
  //   ()(i, ii)
}
void doc_integral_traits() {
  // integral_traits<T>
  //   typename change_signedness
  //   static constexpr is_specialized
  //   static constexpr is_signed
  //   static constexpr min()
  //   static constexpr max()
  {
    using ts = integral_traits<int>;
    static_assert(is_same<ts::change_signedness, make_unsigned<int>>);
    static_assert(ts::is_specialized);
    static_assert(ts::is_signed);
    static_assert(ts::min() < ts::max());

    static_assert(!integral_traits<void>::is_specialized);
  }
}
void doc_numeric_limits() {
  // numeric_limits<INT_OR_FLOAT>
  //   static constexpr bool is_specialized = true
  //   static constexpr bool is_signed
  //   static constexpr lowest()
  //     // only for floating point, negative finite value
  //   static constexpr denorm_min()
  //     // only for floating point, minimal positive denomalized number
  //   static constexpr min()
  //     // for floating point: minimal positive normalized number
  //   static constexpr max()
  //     // for floating point: max positive normalized number
  //   static constexpr bool is_integer
  //
  //   static constexpr infinity() // only for floating point
  //   static constexpr nan() // only for floating point
  //   static has_infinity(f)->bool
  //     // only for floating point, check infinity or not
  //   static has_denorm(f)->bool
  //     // only for floating point, check denormalized or not
  //   static has_nan(f)->bool
  //     // only for floating point, check Not-an-Number or not
  {
    using ts = numeric_limits<float>;
    assert(ts::lowest() == -ts::max());
    assert(ts::min() > ts::denorm_min());
    assert(ts::denorm_min() > 0.0f);
  }
}
void doc_three_way_comparison() {
  // partial_ordering = std::partial_ordering
  // weak_ordering = std::weak_ordering
  // strong_ordering = std::strong_ordering

  // is_eq(ordering)->bool // equal to
  // is_neq(ordering)->bool // not equal to
  // is_lt(ordering)->bool // less than
  // is_lteq(ordering)->bool // less than or equal to
  // is_gt(ordering)->bool // greater than
  // is_gteq(ordering)->bool // greater than or equal to

  // partial_lt
  // partial_gt
  // partial_eq
  // partial_uo
  // weak_lt
  // week_gt
  // week_eq
  // strong_lt
  // strong_gt
  // strong_eq
  {
    assert((1 <=> 2) == strong_lt);
    assert((NAN <=> 1.0f) == partial_uo);
  }

  // reverse_ordering
  {
    assert(reverse_ordering(as_lvalue(strong_lt)) == strong_gt);
    assert(reverse_ordering(strong_gt) == strong_lt);
    assert(reverse_ordering(strong_eq) == strong_eq);
    assert(reverse_ordering(as_lvalue(weak_lt)) == weak_gt);
    assert(reverse_ordering(weak_gt) == weak_lt);
    assert(reverse_ordering(weak_eq) == weak_eq);
    assert(reverse_ordering(as_lvalue(partial_lt)) == partial_gt);
    assert(reverse_ordering(partial_gt) == partial_lt);
    assert(reverse_ordering(partial_eq) == partial_eq);
  }

  // template_common_comparison_category<S...>
  // common_comparison_category<S...>
  {
    static_assert(same_as<common_comparison_category<>, strong_ordering>);
    static_assert(same_as
                  <common_comparison_category
                   <partial_ordering, strong_ordering, weak_ordering>,
                   partial_ordering>);
    static_assert(is_void
                  <common_comparison_category
                   <partial_ordering, strong_ordering, weak_ordering, int>>);
  }

  // three_way_comparable<T, CAT = partial_ordering>
  // three_way_comparable_with<T, U, CAT = partial_ordering>
  {
    static_assert(three_way_comparable<int>);
    static_assert(!three_way_comparable<nullptr_t>);
    static_assert(three_way_comparable_with<int, int>);
    static_assert(!three_way_comparable_with<int, void *>);
  }

  // template_compare_three_way_result<A, B>
  // compare_three_way_result<A, B>
  // compare_three_way
  //   typename is_transparent
  //   operator ()(a, b) // may throw
  // can_apply_compare_three_way<A, B>
  {
    static_assert(!has_member_type_type
                  <template_compare_three_way_result<void *, int>>);
    static_assert(is_same
                  <compare_three_way_result<int, int>, strong_ordering>);
    static_assert(is_same
                  <compare_three_way_result<float, float>, partial_ordering>);

    assert(compare_three_way{}(0, 0.0) == 0);

    static_assert(can_apply_compare_three_way<const int &, const int &>);
  }

  // synth_3way(a, b) // may throw
  // synth_3way_result<T, U = T> // may throw
  {
    int a = 0, b = 1;
    assert(synth_3way(a, b) < 0);
    static_assert(is_same<synth_3way_result<int, int>, strong_ordering>);
  }
}
void doc_type_index() {
  // type_index
  //   special member functions: no default constructor
  //
  //   type_index(const type_info &)
  //   == < > <= >= <=>
  //   hash_code()
  //   name()
  {
    type_index x = typeid(int);
    type_index y = typeid(int);
    assert(x == y);
    assert((x <=> y) == 0);
    assert(x.hash_code() == y.hash_code());
    assert(strcmp(x.name(), y.name()) == 0);
  }
}
void doc_basic_function_objects() {
  // template_is_transparent_function<T>
  // is_transparent_function<T>
  //
  // pre_increment
  // pre_decrement
  // post_increment
  // post_decrement
  //   typename is_transparent
  //   ()(x) // may throw
  //
  // plus<T = void>
  //   typename is_transparent // enable if T == void
  //   ()(cx, cy) // may throw
  // minus<T = void>
  // multiplies<T = void>
  // divides<T = void>
  // modulus<T = void>
  // negate<T = void>
  // equal_to<T = void>
  // not_equal_to<T = void>
  // greater<T = void>
  // less<T = void>
  // greater_equal<T = void>
  // less_equal<T = void>
  // logical_and<T = void>
  // logical_or<T = void>
  // logical_not<T = void>
  // bit_and<T = void>
  // bit_or<T = void>
  // bit_xor<T = void>
  // bit_not<T = void>
  {
    static_assert(is_transparent_function<plus<>>);
    static_assert(!is_transparent_function<plus<int>>);
  }

  // can_apply_plus<X, Y> // check if we can apply + for X && and Y &&
  // can_apply_minus<X, Y>
  // can_apply_multiplies<X, Y>
  // can_apply_divides<X, Y>
  // can_apply_modulus<X, Y>
  // can_apply_negate<X>
  // can_apply_equal_to<X, Y>
  // can_apply_not_equal_to<X, Y>
  // can_apply_greater<X, Y>
  // can_apply_less<X, Y>
  // can_apply_greater_equal<X, Y>
  // can_apply_less_equal<X, Y>
  // can_apply_logical_and<X, Y>
  // can_apply_logical_or<X, Y>
  // can_apply_logical_not<X>
  // can_apply_bit_and<X, Y>
  // can_apply_bit_or<X, Y>
  // can_apply_bit_xor<X, Y>
  // can_apply_bit_not<X, Y>
  {
    static_assert(can_apply_plus<const int &, const int &>);;
  }

  // identity
  //   typename is_transparent
  //   ()(T &&)->T &&
  {
    assert(identity()(1) == 1);
  }

  // not_fn(f) // may throw
  // decltype(not_fn(f))
  //   ()(s...) & // may throw
  //   ()(s...) const & // may throw
  //   ()(s...) && // may throw
  //   ()(s...) const && // may throw
  {
    const auto f = not_fn([]() {return true;});
    assert(!f());
    const auto f2 = not_fn([](int, int) {return true;});
    assert(!f2(1, 1));
  }

  // mem_fn(fp)
  // decltype(mem_fn(fp))
  //   ()(s...) // may throw
  {
    struct A {
      int value = 1;
      int f() {return 1;}
    };
    A a;

    const auto f1 = mem_fn(&A::value);
    assert(f1(a) == 1);

    int (A::*const fp)() = &A::f;
    const auto f2 = mem_fn(fp);
    assert(f2(a) == 1);
  }
}
void doc_tuple() {
  // piecewise_construct_t
  // piecewise_construct
  // ignore
  // allocator_arg_t
  // allocator_arg

  // template_tuple_size<T>
  // tuple_size<T>
  // template_tuple_element<I, T>
  // tuple_element<I, T>
  {
    using A = array<int, 2>;
    using T = tuple<char, int>;

    static_assert(tuple_size<A> == 2);
    static_assert(tuple_size<T> == 2);

    static_assert(is_same<tuple_element<0, A>, int>);
    static_assert(is_same<tuple_element<1, A>, int>);
    static_assert(is_same<tuple_element<0, T>, char>);
    static_assert(is_same<tuple_element<1, T>, int>);
    static_assert(is_same<tuple_element<1, const T>, const int>);
  }

  // tuple(S...)->tuple<S...>
  // tuple(allocator_arg, a, S...)->tuple<S...>
  // tuple(allocator_arg, a, tuple<S...>)->tuple<S...>
  {
    {
      tuple t{};
      tuple tt(allocator_arg, 0);
      tuple ttt(allocator_arg, 0, tuple{});
      assert(t == tuple{} && t == tt && t == ttt);
    }
    {
      tuple t(1);
      tuple tt(allocator_arg, 0, 1);
      tuple ttt(allocator_arg, 0, tuple(1));
      assert(t == tuple(1) && t == tt && t == ttt);
    }
    {
      tuple t(1, 2);
      tuple tt(allocator_arg, 0, 1, 2);
      tuple ttt(allocator_arg, 0, tuple(1, 2));
      assert(t == tuple(1, 2) && t == tt && t == ttt);
    }
    {
      tuple t(1, 2, 3);
      tuple tt(allocator_arg, 0, 1, 2, 3);
      tuple ttt(allocator_arg, 0, tuple(1, 2, 3));
      assert(t == tuple(1, 2, 3) && t == tt && t == ttt);
    }
  }

  // simple_pair<A, B>
  //   first
  //   second
  // simple_tuple<S...>
  //   value
  //   following // enable if sizeof...(S) > 1
  //     value
  //     following // enable if sizeof...(S) > 2
  //       value
  //       ... // ...
  {
    static_assert(semiregular<simple_pair<int, float>>);
    static_assert(!is_copy_assignable<simple_pair<int, float &>>);
    static_assert(semiregular<simple_tuple<int, float, double>>);
    static_assert(!is_copy_assignable<simple_tuple<int, float &, double>>);

    simple_pair<int, float> t0{1, 2.0f};
    assert(pair(t0.first, t0.second) == pair(1, 2.0f));

    simple_tuple<int, float, double> t{1, 2.0f, 3.0};
    assert(t.value == 1);
    assert(t.following.value == 2.0f);
    assert(t.following.following.value == 3.0);
  }

  // pair<A, B> = tuple<A, B>
  {
    pair p(1, 2);
    static_assert(same_as<decltype(p), tuple<int, int>>);
  }

  // template_common_reference<CVREF tuple<...S1>, CVREF tuple<...S2>>
  {
    struct t {};
    struct tt : t {};
    static_assert(is_same
                  <common_reference<tuple<int &, t &>, tuple<long long, tt &>>,
                   tuple<long long, t &>>);
    static_assert(is_same
                  <common_reference
                   <tuple<int &, t &>, const tuple<long long, tt> &>,
                   tuple<long long, const t &>>);
    static_assert(is_same<common_reference<int &, long long &>, long long>);
    static_assert(is_same<common_reference<t &, const tt &>, const t &>);
  }

  // template_common_type<CVREF tuple<...S1>, CVREF tuple<...S2>>
  {
    struct t {};
    struct tt : t {};
    static_assert(is_same
                  <common_type<tuple<int, t *>, tuple<long long, tt *>>,
                   tuple<long long, t *>>);
  }

  // get<I>(tuple_ref)->tuple_element_ref // legal I
  // get<T>(tuple_ref)->tuple_element_ref // T is unique
  // at<I>(tuple_ref)->tuple_element_ref  // legal I
  // as<T>(tuple_ref)->tuple_element_ref  // T is unique
  {
    tuple<int, double> t(1, 2);
    assert(as<int>(t) == 1);
    assert(as<double>(t) == 2);
    assert(at<0>(t) == 1);
    assert(at<1>(t) == 2);
    assert(get<int>(t) == 1);
    assert(get<double>(t) == 2);
    assert(get<0>(t) == 1);
    assert(get<1>(t) == 2);
  }

  // forward_as_tuple(s...)
  {
    string s1 = "ab", s2 = "cd";
    const tuple<string, string> t = forward_as_tuple(s1, s2);
    assert(at<0>(t) == "ab" && at<1>(t) == "cd");
    assert(s1 == "ab" && s2 == "cd");
    const tuple<string, string> t2 = forward_as_tuple(move(s1), move(s2));
    assert(s1 == "" && s2 == "");
    assert(t == t2);
  }

  // template_uses_allocator<tuple<...>, AL> : true_type
  //   // tuple uses all allocator,
  //   //   but only really pass the allocator arg to
  //   //   constructor of every element uses it
  // uses_allocator_construction_args<T>(al, s...)->tuple<refs...> // may throw
  {
    static_assert(uses_allocator<tuple<>, int>);
    static_assert(uses_allocator<tuple<int, int>, int>);
    static_assert(uses_allocator<tuple<int, int, int>, char>);

    using t = decltype(uses_allocator_construction_args<tuple<int>>(1, 2));
    static_assert(tuple_size<t> == 3);
    static_assert(is_same<tuple_element<0, t>, allocator_arg_t>);
    static_assert(is_same<tuple_element<1, t>, const int &>);
    static_assert(is_same<tuple_element<2, t>, int &&>);
  }

  // make_tuple(s...) // may throw
  // make_pair(x, y) // may throw
  {
    const auto t = make_tuple(1, 2, 3);
    const int i = 0;
    const auto t2 = make_pair(1, ref(i));
    assert(t == make_tuple(1, 2, 3));
    assert(t2 == make_tuple(1, 0));
    static_assert(is_same<decltype(t), const tuple<int, int, int>>);
    static_assert(is_same<decltype(t2), const tuple<int, const int &>>);
  }

  // tie(s...)
  {
    int i, j;
    tie(i, j) = make_tuple(1, 2);
    assert(i == 1 && j == 2);
    tie(i, ignore, j) = make_tuple(1, 2, 3);
    assert(i == 1 && j == 3);
  }

  // tuple_cat(s...) // may throw
  {
    assert(make_tuple(1, 2) == tuple_cat(make_tuple(1, 2)));
    assert(make_tuple(1, 2, 3, 4, 5, 6, 7, 8)
           == tuple_cat(make_tuple(1, 2),
                        make_tuple(3),
                        make_tuple(),
                        make_tuple(4, 5, 6),
                        make_tuple(7, 8)));
    static_assert(same_as
                  <decltype(tuple_cat(declval<tuple<int> &>(),
                                      declval<const tuple<int> &>(),
                                      declval<tuple<int> &&>())),
                   tuple<int, int, int>>);
  }

  // apply(f, t) // may throw
  {
    string s1 = "ab", s2 = "cd", s3 = "ef";
    tuple<string, string, string> t
      = apply([](auto &&...s) {return make_tuple((decltype(s))s...);},
              forward_as_tuple(move(s1), move(s2), move(s3)));
    assert(t == make_tuple("ab"_s, "cd"_s, "ef"_s));
    assert(s1 == s2 && s2 == s3 && s3 == "");
  }

  // make_from_tuple<T>(t) // may throw
  {
    struct X {
      int i;
      int j;
    };
    X x = make_from_tuple<X>(make_tuple(1, 2));
    assert(x.i == 1 && x.j == 2);
  }

  // tuple<S...>
  //   explicit(...) tuple() // may throw
  //     // value initialize S...
  //   ~tuple()
  //   tuple(const tuple &) // may throw
  //   tuple &operator =(const tuple &) // may throw
  //   tuple(tuple &&) // may throw
  //   tuple &operator =(tuple &&) // may throw
  //   swap(tuple &) // may throw
  //   // tuple include reference member is assignable but struct can not
  //
  //   tuple(piecewise_construct, ...) // may throw
  //   explicit(...) tuple(const S &...) // may throw
  //   explicit(...) tuple(SS &&...) // may throw
  //   explicit(...) tuple(tuple<...> &) // may throw
  //   tuple &operator =(tuple<...> &) // may throw
  //   explicit(...) tuple(const tuple<...> &) // may throw
  //   tuple &operator =(const tuple<...> &) // may throw
  //   explicit(...) tuple(tuple<...> &&) // may throw
  //   tuple &operator =(tuple<...> &&) // may throw
  //   explicit(...) tuple(const tuple<...> &&) // may throw
  //   tuple &operator =(const tuple<...> &&) // may throw
  //   tuple(allocator_arg, alloc, ...) // may throw
  //     // no piecewise version
  //
  //   at<I>() // legal I
  //   as<T>() // T is unique among S...
  // == <=> // may throw

  // bind_tuple(t, f) // may throw
  {
    assert(bind_tuple(tuple(1, 2, 3), [](int x) {return x * 2;})
           == tuple(2, 4, 6));
  }
}
void doc_bind() {
  // template_is_bind_expression<T>
  // is_bind_expression<T>
  {
    static_assert(is_bind_expression<inner::be<plus<>, tuple<>>>);
    static_assert(!is_bind_expression<int>);
  }

  // inline namespace placeholders {
  // _1
  // _2
  // ...
  // }
  // template_is_placeholder<T>
  // is_placeholder<T>
  {
    static_assert(is_placeholder<decltype(_1)>);
    static_assert(!is_placeholder<int>);
  }

  // bind(f, s...) // may throw
  // bind_r<T>(f, s...) // may throw
  //
  // decltype(bind(_r<T>)(...))
  //   special member functions: dependent // may throw
  //   ()(...) // may throw
  {
    assert(bind(minus{}, _1, _2)(3, 4) == -1);
    assert(bind(minus{}, _2, _1)(3, 4) == 1);
    assert(bind(plus{},
                bind(multiplies{}, _1, _2),
                bind(multiplies{}, _3, _4))(1, 2, 3, 4) == 14);

    int x = 1, y = 2, z = 3;
    const auto assign = [](auto &&x, auto &&y)->auto & {
      x = y;
      return x;
    };
    bind(assign, x, bind(plus{}, y, z)())();
    assert(x == 1 && y == 2 && z == 3);
    bind(assign, ref(x), bind(plus{}, y, z)())();
    assert(x == 5 && y == 2 && z == 3);
  }

  // bind_front(f, s...) // may throw
  // bind_back(f, s...) // may throw
  //
  // decltype(bind_front/back(s...)) // store decay_t for function and args
  //   ()(s...) & // may throw
  //   ()(s...) const & // may throw
  //   ()(s...) && // may throw
  //   ()(s...) const && // may throw
  {
    const auto f = [](int a, int b, int c, int d) {
      return a * 1000 + b * 100 + c * 10 + d;
    };
    assert(bind_front(f, 1, 2)(3, 4) == 1234);
    assert(bind_back(f, 4)(1, 2, 3) == 1234);
  }
}
void doc_ratio() {
  // ratio
  //   num
  //   den
  //
  //   special member functions: no assignment operator
  //
  //   ratio(n, d = 1)
  //
  //   +
  //   -
  //   +(ratio)
  //   -(ratio)
  //   *(ratio)
  //   /(ratio)
  // ==
  // <=>
  {
    constexpr ratio r1;
    static_assert(r1.num == 0 && r1.den == 1);
    constexpr ratio r2(1, -1);
    static_assert(r2.num == -1 && r2.den == 1);
    constexpr ratio r3(0, -100);
    static_assert(r3.num == 0 && r3.den == 1);

    static_assert(-ratio(-1, 2) == ratio(1, 2));
    static_assert(+ratio(1, 2) == ratio(1, 2));
    static_assert(ratio(1, 2) - ratio(1, 2) == ratio(0, 1));
    static_assert(ratio(1, 2) + ratio(1, 2) == ratio(1, 1));
    static_assert(ratio(2, 3) * ratio(3, 2) == ratio(1, 1));
    static_assert(ratio(2, 3) / ratio(2, 3) == ratio(1, 1));

    static_assert(ratio(2, 3) <=> ratio(2, 3) == 0);
    static_assert(ratio(2, 3) <=> ratio(3, 3) < 0);
  }

  // ratio atto(1, 1'000'000'000'000'000'000);
  // ratio femto(1, 1'000'000'000'000'000);
  // ratio pico(1, 1'000'000'000'000);
  // ratio nano(1, 1'000'000'000);
  // ratio micro(1, 1'000'000);
  // ratio milli(1, 1'000);
  // ratio centi(1, 100);
  // ratio deci(1, 10);
  // ratio deca(10, 1);
  // ratio hecto(100, 1);
  // ratio kilo(1'000, 1);
  // ratio mega(1'000'000, 1);
  // ratio giga(1'000'000'000, 1);
  // ratio tera(1'000'000'000'000, 1);
  // ratio peta(1'000'000'000'000'000, 1);
  // ratio exa(1'000'000'000'000'000'000, 1);
  {
    static_assert(atto.num == 1);
    static_assert(atto.den == 1'000'000'000'000'000'000);
    static_assert(femto.num == 1);
    static_assert(femto.den == 1'000'000'000'000'000);
    static_assert(pico.num == 1);
    static_assert(pico.den == 1'000'000'000'000);
    static_assert(nano.num == 1);
    static_assert(nano.den == 1'000'000'000);
    static_assert(micro.num == 1);
    static_assert(micro.den == 1'000'000);
    static_assert(milli.num == 1);
    static_assert(milli.den == 1'000);
    static_assert(centi.num == 1);
    static_assert(centi.den == 100);
    static_assert(deci.num == 1);
    static_assert(deci.den == 10);
    static_assert(deca.num == 10);
    static_assert(deca.den == 1);
    static_assert(hecto.num == 100);
    static_assert(hecto.den == 1);
    static_assert(kilo.num == 1'000);
    static_assert(kilo.den == 1);
    static_assert(mega.num == 1'000'000);
    static_assert(mega.den == 1);
    static_assert(giga.num == 1'000'000'000);
    static_assert(giga.den == 1);
    static_assert(tera.num == 1'000'000'000'000);
    static_assert(tera.den == 1);
    static_assert(peta.num == 1'000'000'000'000'000);
    static_assert(peta.den == 1);
    static_assert(exa.num == 1'000'000'000'000'000'000);
    static_assert(exa.den == 1);
  }
}
void doc_optional() {
  // in_place_t
  // in_place
  //
  // in_place_type_t<T>
  // in_place_type<T>
  // template_is_in_place_type<T>
  // is_in_place_type<T>
  //
  // nullopt_t
  // nullopt
  //
  // bad_optional_access : public exception
  //
  // optional<T>
  //   value_type = T
  //
  //   special member functions: dependent // copy may throw
  //
  //   == <=> // may throw
  //   == <=> with optional<U> // may throw
  //   == <=> with nullopt
  //
  //   optional(nullopt)
  //   =(nullopt)
  //
  //   constexpr explicit optional(in_place, s...) // may throw
  //   constexpr explicit optional(in_place, {...}, s...) // may throw
  //
  //   constexpr explicit(...) optional(const U &) // may throw
  //   =(const U &) // may throw
  //   constexpr explicit(...) optional(U &&) // may throw
  //   =(U &&) // may throw
  //
  //   explicit(...) optional(const optional<U> &) // may throw
  //   =(const optional<U> &) // may throw
  //   explicit(...) optional(optional<U> &&) // may throw
  //   =(optional<U> &&) // may throw
  //
  //   T &emplace(s...) // may throw // no self-emplace
  //   T &emplace({...}, s...) // may throw // no self-emplace
  //
  //   ->()->T_ptr // has_value()
  //   *()->T_ref // has_value()
  //
  //   empty()->bool
  //   clear()
  //   explicit operator bool()
  //   has_value()->bool
  //   value()->T REF
  //     // may throw (throw bad_optional_access if there is no value)
  //   value_or(U &&)->T // may throw
  //   reset()
  //
  //   and_then(f) // may throw
  //     // return invoke(f, value()) or invoke_result<...>{}
  //   transform(f) // may throw
  //     // return optional<U>(invoke(f, value())) or optional<U>{}
  //   or_else(f) // may throw
  //     // is_same<remove_cvref<invoke_result<F>>, this_t>
  //     // return *this or f()
  //
  //   ()(...)->invoke_result<...>
  //
  // make_optional(in_place_type<T>, s...)->optional<T> // may throw
  // make_optional(in_place_type<T>, {...}, s...)->optional<T> // may throw
  // make_optional(x)->optional<decay<...>> // may throw
}
void doc_semiregular_function() {
  // assign_non_assignable<F>(to, from) // may throw
  //   // F is copy-constructible
  //   // F can not throw from copy/move-constructor
  // copyable_wrapper<F>
  //   // F is copy-constructible
  //   // F can not throw from copy/move-constructor
  //   special member functions: copyable, may be default constructible
  //   explicit copyable_wrapper(f)
  //   ->
  //   *
  //   base()->F
  //
  // call_or_deref_call(f, s...) // may throw
  //   // return f(forward<S>(s)...) or (*f)(forward<S>(s)...)
  //
  // semiregular_function<F>
  //   // note: semiregular = (copyable && default_initializable)
  //   // F is copy-constructible
  //   // F can not throw from default-constructible
  //   // F can not throw from copy/move-constructor
  //   special member functions: full
  //   explicit semiregular_function(F)
  //   ()(s...) // may throw
  //   base()->F
  // semiregular_fn(f)
}
void doc_hash() {
  // template_is_nothrow_hashable<T>
  // is_nothrow_hashable<T>
  //
  // hash<T = void>
  //   is_transparent = ...
  //   result_type = size_t
  //   ()(const T &)->size_t = delete
  // hash<short>
  //   argument_type = short
  //   result_type = size_t
  //   ()(argument_type)->size_t
  // hash<unsigned short> // similar to hash<short>
  // hash<int> // similar to hash<short>
  // hash<unsigned int> // similar to hash<short>
  // hash<long> // similar to hash<short>
  // hash<unsigned long> // similar to hash<short>
  // hash<long long> // similar to hash<short>
  // hash<unsigned long long> // similar to hash<short>
  // hash<char> // similar to hash<short>
  // hash<unsigned char> // similar to hash<short>
  // hash<char16_t> // similar to hash<short>
  // hash<char32_t> // similar to hash<short>
  // hash<wchar_t> // similar to hash<short>
  // hash<bool> // similar to hash<short>
  // hash<T *> // similar to hash<short>
  // hash<float> // similar to hash<short>
  // hash<double> // similar to hash<short>
  // hash<nullptr_t> // similar to hash<short>
  // hash<type_index> // similar to hash<short>
  // hash<optional<T>> // similar to hash<short>
  // hash<tuple<T1, T2>> // similar to hash<short>
  //
  // hash<unique_ptr<T, D>>
  // hash<monostate>
  // hash<variant<S...>>
  // hash<basic_string>
  // hash<sso_string>
  // hash<string_reference<T>>
  // hash<bitset<N>>
  // hash<vector<bool, AL>>
}
void doc_miscl() {
  // base(x) // may throw
  //   // = x.base()
  {
    struct T {
      int base() {return 1;}
    } t;
    assert(base(t) == 1);
  }

  // from_range_t
  // from_range

  // min_value(T x, U y)->common_type<T, U> // may throw
  // max_value(T x, U y)->common_type<T, U> // may throw
  {
    static_assert(min_value(1, 2) == 1);
    static_assert(max_value(1, 2) == 2);
  }

  // conditionally_enable_copy_and_move<Y1, Y2, Y3, Y4>
  //   // used as base type to disable the corresponding special member function
  {
    constexpr bool y1 = true, y2 = false, y3 = false, y4 = true;
    using t = conditionally_enable_copy_and_move<y1, y2, y3, y4>;
    static_assert(is_copy_constructible<t> == y1);
    static_assert(is_copy_assignable<t> == y2);
    static_assert(is_move_constructible<t> == y3);
    static_assert(is_move_assignable<t> == y4);
  }

  // empty_function
  //   is_transparent = ...
  //   ()(...)->void // do nothing
  {
    empty_function{}(1, 2, 3, 4, 5);
  }

  // inner::exit_fn_t<F>
  //   special member functions: only move-constructible
  //   explicit exit_fn_t(f) // may throw
  // inner::switchable_exit_fn_t<F>
  //   special member functions: only move-constructible
  //   explicit switchable_exit_fn_t(f, y = true) // may throw
  //   explicit operator bool()
  //   set(y = true)
  //   unset()
  // exit_fn(f) // may throw
  // exit_fn(f, y) // may throw
  {
    int x = 1;
    {
      const auto g = exit_fn(bind(assign_to,
                                  bind(plus<int>{}, ref(x), 1), ref(x)));
      auto g2 = exit_fn(bind(assign_to,
                             bind(plus<int>{}, ref(x), 1), ref(x)),
                        false);
      g2.set();
    }
    assert(x == 3);
  }

  // copy(x)->remove_cvref<x_t> // may throw
  // copy_if_rvalue(x)->remove_cvref_or_original_ref // may throw
  {
    assert(copy(1) == 1);
    int x = 1;
    copy_if_rvalue(x) = 2;
    assert(x == 2);
    decltype(auto) y = copy_if_rvalue(move(x));
    y = 1;
    assert(y == 1 && x == 2);
  }

  // as_lvalue(x)
  // is_lvalue(x)
  // deref(p) // may throw
  // can_apply_deref<P>
  // assign_to(from, to) // may throw
  {
    const auto f = [](int &) {};
    f(as_lvalue(1));
    static_assert(!is_lvalue(1));

    int i = 0;
    static_assert(is_lvalue(i));
    static_assert(!is_lvalue(move(i)));

    assert(deref(&i) == i);
    static_assert(can_apply_deref<int *>);
    static_assert(can_apply_deref<int *&>);
    static_assert(can_apply_deref<int *const &>);
    static_assert(can_apply_deref<int *&&>);
    static_assert(!can_apply_deref<int>);

    int x = 1, y = 2;
    assign_to(x, y);
    assert(x == 1 && y == 1);
  }

  // template_is_allocator_aware<T>
  // is_allocator_aware<T>
  //   // true if there is a public T::allocator_type
  //   // used to check if T is allocator-aware container
  {
    struct t1 {
      using allocator_type = void;
    };
    struct t2 {};
    static_assert(is_allocator_aware<t1>);
    static_assert(!is_allocator_aware<t2>);
    static_assert(!is_allocator_aware<int>);
  }

  // ccast<T>(x)->T // may throw
  //   // return (T)x
  {
    assert(ccast<float>(4) == 4.0f);
  }

  // bit_cast<T>(x)->T
  {
    float x = 0;
    int32_t y = bit_cast<int32_t>(x);
    x = bit_cast<float>(y);
    assert(x == 0);
  }

  // to_unsigned(x) // x is integral
  // to_signed(x) // x is integral
  {
    int x = 1;
    static_assert(is_same<decltype(to_signed(x)), int>);
    static_assert(is_same<decltype(to_unsigned(x)), unsigned>);
  }

  // is_template<T, TMPL>
  {
    static_assert(!is_template<int, type_pack>);
    static_assert(is_template<type_pack<int, int>, type_pack>);
    static_assert(is_template<type_pack<int, int> &, type_pack>);
    static_assert(is_template<const type_pack<int, int> &, type_pack>);
    static_assert(is_template<type_pack<int, int> &&, type_pack>);
    static_assert(is_template<const type_pack<int, int> &&, type_pack>);
  }

  // is_aligned(p, algn) // algn is 2 % n, algn > 0
  {
    void *p{};
    assert(is_aligned(p, alignof(void *)));
  }

  // div_round_up(x, y) // may throw logic_error if y == 0
  //   // x and y are the same integral type
  {
    assert(div_round_up(2, 3) == 1);
    assert(div_round_up(-3, 2) == -2);
  }

  // div_round(x, y) // may throw logic_error if y == 0
  //   // x and y are the same integral type
  {
    assert(div_round(5, 4) == 1);
    assert(div_round(6, 4) == 2);
    assert(div_round(-5, 4) == -1);
    assert(div_round(-6, 4) == -2);
  }

  // integral_cast<T>(x) // may throw length_error if unrepresentable
  //   // cast between integral types

  // call_by_n_times(f, x, n) // may throw
  //   // n > 0
  //   // repeat doing x = f(x) by n times
  assert(call_by_n_times(bind(plus{}, _1, 1), 1, 1) == 2);
  assert(call_by_n_times(bind(plus{}, _1, 1), 1, 3) == 4);

  // non_propagating_cache<T>
  //   value_type = T
  //   special member functions: full
  //   non_propagating_cache(in_place, s...) // may throw
  //   emplace(s...) // may throw
  //   emplace_or_assign(x) // may throw
  //   clear()
  //   empty()->bool
  //   ->()->T_ptr
  //   *()->T_ref // !empty()
  {
    using cache_t = non_propagating_cache<int>;

    cache_t x;
    assert(x.empty());
    x.emplace(1);
    assert(!x.empty() && *x == 1);

    cache_t x2 = x;
    assert(x2.empty() && !x.empty() && *x == 1);
    x2 = x;
    assert(x2.empty() && !x.empty() && *x == 1);

    cache_t x3 = move(x);
    assert(x3.empty() && x.empty());
    x.emplace(1);
    x3 = move(x);
    assert(x3.empty() && x.empty());
  }

  // simple_wrapper<T>
  //   // be used to replace place of non_propagating_cache,
  //   //   so there is no cache but straight storing
  //   special member functions: full
  //   simple_wrapper(in_place, s...) // may throw
  //   ->()->T_ptr
  //   *()->T_ref
  {
    simple_wrapper<int> x;
    assert(*x == 0);
  }

  // copy_and_swap(from, to) // may throw
  //   // implement copy-assignment operator by adl-swap
  //   // for move-assignment: call copy_and_swap(move(from), to)
  // copy_and_move(from, to) // may throw
  //   // implement copy-assignment operator by move-assignment operator
  {
    string s = "abc";
    string s2 = "";
    copy_and_swap(s, s2);
    assert(s2 == "abc" && s == s2);
    copy_and_swap(move(s), s2);
    assert(s2 == "abc" && s.empty());

    s = "abc";
    s2 = "";
    copy_and_move(s, s2);
    assert(s2 == "abc" && s == s2);
  }

  // derivable_wrapper<T>
  //   // make underivable type like function pointer derivable
  //   special member functions: dependent
  //   explicit derivable_wrapper(const T &)
  //   explicit derivable_wrapper(T &&)
  //   derivable_wrapper(in_place, s...)
  //   ->()->T_ptr
  //   *()->T_ref
  {
    derivable_wrapper<int> x;
    assert(*x == 0);
  }

  // container_regular_max_size<C>()->C::size_type
  //   // auto-calculate by
  //   //   ptrdiff_t, C::value_type, C::difference_type, and C::size_type

  // function_return_false(f, s...)
  {
    const auto f0 = [](int) {};
    const auto f1 = [](int)->int {return 0;};
    const auto f2 = [](int)->bool {return true;};
    const auto f3 = [](int)->bool {return false;};
    assert(!function_return_false(f0, 0));
    assert(!function_return_false(f1, 0));
    assert(!function_return_false(f2, 0));
    assert(function_return_false(f3, 0));
  }

  // l_shift<T>(T x, T y)->T // avoid auto integral promotion 
  // r_shift<T>(T x, T y)->T // avoid auto integral promotion 
  // b_and<T>(T x, T y)->T // avoid auto integral promotion 
  // b_or<T>(T x, T y)->T // avoid auto integral promotion 
  // b_xor<T>(T x, T y)->T // avoid auto integral promotion
  {
    assert(l_shift<uint8_t>(0b1111, 4) == 0b11110000u);
  }

  // add_with_check(integrals...)->optional<integral>
  // sub_with_check(integrals...)->optional<integral>
  // sub_to_zero_at_most(INT, INT, ...)->INT // all args greater than zero

  // max_uint_of_max_size<N> // max uint type less or equal to N-byte space
  {
    static_assert(same_as<max_uint_of_max_size<9>, uint64_t>);
    static_assert(same_as<max_uint_of_max_size<8>, uint64_t>);
    static_assert(same_as<max_uint_of_max_size<7>, uint32_t>);
    static_assert(same_as<max_uint_of_max_size<1>, uint8_t>);
    static_assert(same_as<max_uint_of_max_size<0>, void>);
  }
}
void doc_floating_point_traits() {
  // floating_point_traits<float or double>
  //   float_t
  //   uint_t
  //   int_t
  //   static constexpr uint_t n
  //   static constexpr uint_t k
  //   static constexpr uint_t bias
  //   static constexpr uint_t e_max
  //     // e.g. 0b11111111 for float
  //   static constexpr uint_t f_max
  //     // e.g. 0b111'11111'11111'11111'11111 for float
  //
  //   info_t
  //     bool sign // true for positive
  //     uint_t e
  //     uint_t f
  //     ==(const info_t &) = default
  //   static info(x)->info_t
  //   static make(info_t)->float_t
  //   static make(sign, e, f)->float_t
  //
  //   static sign(x)->bool // positive or negative (note: +0.0 and -0.0)
  //   static e(x)->uint_t
  //   static f(x)->uint_t
  //
  //   static is_normalized(float_t)->bool
  //   static is_denormalized(float_t)->bool
  //   static is_infinity(float_t)->bool
  //   static is_nan(float_t)->bool
  //
  //   static normalized_final_e(float_t)->int_t
  //   static normalized_final_f(float_t)->pair<uint_t, int_t>
  //
  //   static denormalized_final_e(float_t)->int_t
  //   static denormalized_final_f(float_t)->pair<uint_t, int_t>
  //
  //   static positive_inf()->float_t
  //   static negative_inf()->float_t
  //   static positive_nan()->float_t
  //   static negative_nan()->float_t
  //   static nan()->float_t
  //   static infinity()->float_t
  //   static has_infinity(f)->bool
  //   static has_denorm(f)->bool
  //   static has_nan(f)->bool
  //
  //   static constexpr bool is_specialized = true
  //   static constexpr bool is_signed = true
  //   static constexpr bool is_integer = false
  //   static lowest()->float_t // min negative value
  //   static denorm_min()->float_t // positive denorm_min
  //   static min()->float_t // positive norm_min
  //   static max()->float_t
  //   static constexpr bool is_integer
  //
  // float_traits = floating_point_traits<float>
  // double_traits = floating_point_traits<double>
}
void doc_std_dependence_fpfns() {
  // ceil(fp)->fp_t // from -inf to +inf
  // floor(fp)->fp_t // from -inf to +inf
  // round(fp)->fp_t // from zero to inf
  // trunc(fp)->fp_t // from inf toward zero

  // NAN // float
  // INFINITY // positive infinity float

  // FP_INFINITE
  // FP_NAN
  // FP_NORMAL
  // FP_SUBNORMAL
  // FP_ZERO
  // fpclassify(fp)->int
  //   // return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, or FP_ZERO
  {
    assert(fpclassify(0.0) == FP_ZERO);
    assert(fpclassify(NAN) == FP_NAN);
  }

  // isfinite(fp)->bool
  // isinf(fp)->bool
  // isnan(fp)->bool
  // isnormal(fp)->bool
  // signbit(fp)->bool
  // isunordered(fp, fp2)->bool
  {
    assert(signbit(-1.0f));
    assert(!signbit(1.0f));
    assert(isunordered(NAN, 1.0f));
    assert(isunordered(NAN, NAN));
  }
}
void doc_common_math() {
  // abs(i_or_f)->i_or_f_t // same as std::abs
  // sqrt(fp)->fp_t // same as std::sqrt
  // square(fp)->fp_t // same as std::square
  // sin(fp)->fp_t // same as std::sin
  // cos(fp)->fp_t // same as std::cos
  // tan(fp)->fp_t // same as std::tan
  //
  // approx_equal(f, ff, max_dif)->bool // return abs(f - ff) <= max_dif
  //
  // lerp(fp, fp2, fp3)->fp_t
  //   // same as std::lerp
  //   // return fp + fp3 * (fp2 - fp)
  {
    assert(lerp(1.0, 2.0, 0.0) == 1.0);
    assert(lerp(1.0, 2.0, 0.5) == 1.5);
    assert(lerp(1.0, 2.0, 1.0) == 2.0);
  }
}
void doc_basic_functions_for_char32_t() {
  // unicode_is_common_space(char32_t)->bool
  //   // only common space, so do not include '\n', '\r', ...
  // unicode_isspace(char32_t)->bool
  //   // spaces defined in unicode_is_common_space(char32_t)
  //   // and chars defined in isspace(char)
  //
  // unicode_is_lower(char32_t)->bool
  // unicode_is_upper(char32_t)->bool
  // unicode_is_alpha(char32_t)->bool
  // unicode_is_digit(char32_t)->bool
  // unicode_is_alnum(char32_t)->bool
  //
  // unicode_is_xdigit(char32_t)->bool // 0-9, a-f, or A-F
  //
  // unicode_to_upper(char32_t)->char32_t // unchange if non-lower
  // unicode_to_lower(char32_t)->char32_t // unchange if non-upper
}

// test
void doc_owner_ptr() {
  // owner_ptr<T>
  //   special member functions: full // copy may throw
  //   owner_ptr(nullptr_t)
  //   == with nullptr
  //   explicit owner_ptr(T x) // may throw
  //   *()->T & // !empty()
  //   ->()->T *
  //   empty()->bool
  //   clear()
  {
    owner_ptr<int> p;
    assert(p.empty());
    owner_ptr p2(1);
    assert(p2 != nullptr && *p2 == 1);
  }
}
void doc_test_throwing() {
  // test_throwing_any(f)->bool
  // test_throwing<E>(f)->bool

#ifndef RE_NOEXCEPT
  assert(test_throwing_any([]() {throw 'a';}));
  assert(!test_throwing_any([]() {}));
  assert(test_throwing<char>([]() {throw 'a';}));
  assert(!test_throwing<int>([]() {throw 'a';}));
#endif
}
void doc_test_ownership() {
  // // if failed, call print_and_terminate()
  // test_ownership<T>(INIT1, EQ1, INIT2, EQ2, EMPTY) // may throw
  // test_copy<T>(INIT1, EQ1, INIT2, EQ2) // may throw
  // test_move<T>(INIT1, EQ1, INIT2, EQ2, EMPTY) // may throw
  // test_swap<T>(INIT1, EQ1, INIT2, EQ2) // may throw
  //   // test_ownership = test_copy + test_move + test_swap
  // test_copy_construct<T>(INIT, EQ, EMPTY) // may throw
  // test_allocator_aware_container_ownership<T>(INIT1, EQ1, INIT2, EQ2, EMPTY)
  //   // may throw

  using t = test_object<int>;
  test_ownership<t>([](auto p) {new(p) t(1);},
                    [](auto p) {return *p == 1;},
                    [](auto p) {new(p) t(2);},
                    [](auto p) {return *p == 2;},
                    [](auto p) {return *p == 0;});

  using vt = vector<t>;
  test_ownership<vt>([](auto p) {new(p) vt{};},
                     [](auto p) {return p->empty();},
                     [](auto p) {new(p) vt{1, 2, 3};},
                     [](auto p) {return equal(*p, irng(1, 4));},
                     [](auto p) {return p->empty();});
}
void doc_test_rational_operator() {
  // test_equal(a, b)->bool // may throw
  // test_unequal(a, b)->bool // may throw
  // test_less(a, b)->bool // may throw
  //
  // test_equality(a, b) // may throw // if failed, call print_and_terminate()
  // test_inequality(a, b) // may throw // if failed, call print_and_terminate()
  // test_lessness(a, b) // may throw // if failed, call print_and_terminate()

  static_assert(test_equal(1, 1.0));
  static_assert(test_unequal(1, 2));
  static_assert(test_less(0, 1));
  test_equality(1, 1.0);
  test_inequality(1, 2);
  test_lessness(0, 1);
}
void doc_instance_counter() {
  // instance_counter<T>
  //   // throw logic_error if instance count overflow
  //   special member functions // may throw
  //     // move is noexcept if move of T is noexcept
  //   explicit(...) instance_counter(s...) // may throw
  //   ->()->T_ref
  //   *()->T_ptr
  //   operator T_ref()
  //   static count()
  // == <=> // may throw
  {
    assert(instance_counter<int>::count() == 0);
    instance_counter<int> x(1), y(2);
    assert(*x == 1 && *y == 2);
    assert(instance_counter<int>::count() == 2);
  }

  // exception_countdown<T> : public instance_sounter<T>
  //   special member functions: full, may throw
  //   explicit(...) exception_countdown(s...) // may throw
  //   base()->instance_sounter<T>_ref
  //   static constructor_countdown(unsigned)
  //   static constructor_countdown()->unsigned
  //   static default_constructor_countdown(unsigned)
  //   static default_constructor_countdown()->unsigned
  //   static copy_constructor_countdown(unsigned)
  //   static copy_constructor_countdown()->unsigned
  //   static move_constructor_countdown(unsigned)
  //   static move_constructor_countdown()->unsigned
  //   static copy_assignment_countdown(unsigned)
  //   static copy_assignment_countdown()->unsigned
  //   static move_assignment_countdown(unsigned)
  //   static move_assignment_countdown()->unsigned
  {
#ifndef RE_NOEXCEPT
    using T = exception_countdown<int>;
    T::constructor_countdown(3);
    assert(!test_throwing_any([&]() {T t;}));
    assert(!test_throwing_any([&]() {T t;}));
    assert(test_throwing_any([&]() {T t;}));
#endif
  }
}
void doc_ez_dynamic() {
  // ez_dynamic<T>
  //   special member functions: full
  //     // copy may throw
  //     // copy requires is_copy_constructible<T> or T::clone()->T *
  //   static make(s...) // make T // may throw
  //   static make<U>(s...) // make U which is derived from T // may throw
  //   empty()
  //   clear()
  //   ->
  //   * // !empty()
  {
    struct a {
      a() {}
      virtual ~a() {}
      virtual int f() const {return 1;}
      virtual a *clone() const {return new a(*this);}
    };
    struct b : a {
      virtual int f() const override {return 2;}
      virtual a *clone() const override {return new b(*this);}
    };
    auto d = ez_dynamic<a>::make<b>();
    assert(d->f() == 2);
    d = ez_dynamic<a>::make<a>();
    assert(d->f() == 1);
    auto dd = d;
    assert(to_address(dd) != to_address(d) && d->f() == 1 && dd->f() == 1);
  }
}
void doc_ez_function() {
  // ez_function<R (S...)>
  //   special member functions: full // copy may throw
  //
  //   ez_function(f) // may throw
  //   ()(S...)->R // may throw (throw logic_error if empty())
  //   empty()->bool
  //   clear()
  {
    ez_function<int ()> f = []() {return 1;};
    assert(f() == 1);
  }
}
void doc_ez_vector() {
  // ez_vector<T>
  //   // ez_vector accept only copyable T
  //   //   (note: move-only class is not acceptable)
  //
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = T *
  //   const_iterator = const T *
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_vector(initializer_list<T>) // may throw
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   empty()->bool
  //   size()->size_type
  //   max_size()->size_type
  //   capacity()->size_type
  //
  //   insert(const_iterator, const value_type &)->iterator // may throw
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(const_iterator)->iterator
  {
    ez_vector<int> v = {1, 2};
    auto i = v.insert(v.end(), 3);
    assert(*i == 3 && equal(v, irng(1, 4)));
    i = v.erase(v.erase(v.begin(), v.begin() + 2));
    assert(v.empty() && i == v.end());
  }
}
void doc_ez_forward_list() {
  // ez_forward_list<T>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = ...
  //   const_iterator = ...
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   before_begin()->(const_)iterator
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   empty()->bool
  //   max_size()->size_type
  //
  //   new_node(T_ref)->iterator // may throw
  //   delete_node(const_iterator)
  //   link_after(const_iterator, iterator)->iterator
  //   unlink_after(const_iterator)
  //   insert_after(const_iterator, T_ref) // may throw
  //   erase_after(const_iterator, const_iterator)->iterator
  //   erase_after(const_iterator)->iterator
}
void doc_ez_slist() {
  // ez_slist<T>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = ...
  //   const_iterator = ...
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   before_begin()->(const_)iterator
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   empty()->bool
  //   size()->size_type
  //   max_size()->size_type
  //
  //   new_node(T_ref)->iterator // may throw
  //   delete_node(const_iterator)
  //   link_after(const_iterator, iterator)
  //   unlink_after(const_iterator)->iterator
  //   insert_after(const_iterator, T_ref)->iterator // may throw
  //   erase_after(const_iterator, const_iterator)->iterator
  //   erase_after(const_iterator)->iterator
}
void doc_ez_bidirectional_list() {
  // ez_bidirectional_list<T>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = ...
  //   const_iterator = ...
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   empty()->bool
  //   max_size()->size_type
  //
  //   new_node(T_ref)->iterator // may throw
  //   delete_node(const_iterator)
  //   link(const_iterator, iterator)
  //   unlink(const_iterator)->iterator
  //   insert(const_iterator, T_ref)->iterator // may throw
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(const_iterator)->iterator
}
void doc_ez_list() {
  // ez_list<T>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = ...
  //   const_iterator = ...
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   empty()->bool
  //   size()->size_type
  //   max_size()->size_type
  //
  //   new_node(T_ref)->iterator // may throw
  //   delete_node(const_iterator)
  //   link(const_iterator, iterator)->iterator
  //   unlink(const_iterator)->iterator
  //   insert(const_iterator, T_ref)->iterator // may throw
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(const_iterator)->iterator
}
void doc_ez_map() {
  // ez_map<K, M>
  //   special member functions: full // copy may throw
  //   [](const K &)->M & // may throw from <
  //   find(k)->pair<K, M> * // may throw from <
  //   end()->pair<K, M> *
  //   erase(pair<K, M> *)
  //   erase(const K &)->bool // may throw from <
  //   remove(const K &)->bool // may throw from <
  //   empty()->bool
  //   size()->size_t
  //   clear()
  {
    ez_map<int, int> m;
    m[1] = 2;
    assert(m[1] == 2);
    assert(m.size() == 1);
    const auto i = m.find(1);
    assert(i != m.end());
    m.erase(i);
    assert(m.empty());
    assert(!m.remove(1));
  }
}
void doc_test_allocator() {
  // test_allocator<T, MAP = ez_map>
  //   // call print_and_terminate() if any illegal operation take place
  //   value_type = T
  //   size_type = size_t
  //   max_size()->size_type
  //   min_alignment()->size_type
  //   rebind<U>
  //     other = test_allocator<U, MAP>
  //   operator test_allocator<U, MAP>()
  //   special member functions: full
  //   ==
  //   allocate(size_t)->T * // may throw
  //   deallocate(T *, size_t)
  //   static empty()->bool // no instance is allocated
  //   size()->size_t
  //     // count instance allocated by allocator whose type is this_t
  // stateful_test_allocator<T, MAP = ez_map,
  //                         COPY_PRPG = true, MOVE_PRPG = true,
  //                         SWAP_PRPG = true, ALWAYS_EQUAL = false>
  //   // call print_and_terminate() if any illegal operation take place
  //   value_type = T
  //   size_type = size_t
  //   max_size()->size_type
  //   min_alignment()->size_type
  //   rebind<U>
  //     other = test_allocator<U, MAP>
  //   propagate_on_container_copy_assignment = bool_constant<COPY_PRPG>
  //   propagate_on_container_move_assignment = bool_constant<MOVE_PRPG>
  //   propagate_on_container_swap = bool_constant<SWAP_PRPG>
  //   is_always_equal = bool_constant<ALWAYS_EQUAL>
  //   special member functions: full
  //   ==
  //   stateful_test_allocator(const stateful_test_allocator<U, ...> &)
  //     // U != T
  //   allocate(size_t)->T * // may throw
  //   deallocate(T *, size_t)
  //   empty()->bool // no instance is allocated by allocator equal to *this
  //   size()->size_type // count instance allocated by allocator equal to *this
  {
    test_allocator<int> a;
    assert(a.size() == 0u && a.empty());
    int *p = a.allocate(1);
    new(p) int(1);
    assert(a.size() == 1u && !a.empty());
    using t = int;
    p->~t();
    a.deallocate(p, 1);
    assert(a.size() == 0u && a.empty());
  }

  // test_object<T, MAP = ez_map>
  //   // call print_and_terminate() if memory leak in the final check
  //   special member functions: full // may throw (only swap is noexcept)
  //   explicit(...) test_object(x) // may throw
  //   =(x) // may throw
  //   ->()->T_ptr
  //   *()->T_ref
  //   operator T_ref()
  {
    test_object<int> a = 1, b = 2;
    assert(a != b); // called *a != *b
  }
}
void doc_test_range() {
  // // below all: if any error is traced, call print_and_terminate()
  //
  // test_iitr(i, i2, ritr_cr, eq = ...) // may throw
  // test_fitr(i, i2, ritr_cr, eq = ...) // may throw
  // test_bitr(i, i2, ritr_cr, eq = ...) // may throw
  // test_ritr(i, i2, ritr_cr, eq = ...) // may throw
  //
  // test_iitr(r, ritr_cr) // may throw
  // test_fitr(r, ritr_cr) // may throw
  // test_bitr(r, ritr_cr) // may throw
  // test_ritr(r, ritr_cr) // may throw
  //
  // test_rng(r, rr, eq = ...) // may throw
  //   // used to do the usage of equal before equal is defined
  //
  // test_itr_minus(i, i2, n = i2 - i) // may throw
  //   // n >= 0 && n == i2 - i
  //   // used for non-random-access iterator but can call minus
  // test_itr_minus(r, n = size(r)) // may throw
  //   // size(r) == n
  //   // n is cast to size_t
}

// range
void doc_iterator_requirements() {
  // iterator exception requirements
  //   1) special member functions and swap can not throw
  //   2) confirm strong exception guarantee for every related function except
  //      for which takes at least one non-const input iterator argument

  // output_iterator_tag // pre-defined in base.h
  // input_iterator_tag // pre-defined in base.h
  // forward_iterator_tag // pre-defined in base.h
  // bidirectional_iterator_tag // pre-defined in base.h
  // random_access_iterator_tag // pre-defined in base.h
  // contiguous_iterator_tag // pre-defined in base.h

  // incrementable_traits<T> // not used
  //   difference_type
  // iter_difference_t<T> // not used
  {
    static_assert(same_as
                  <incrementable_traits<int *>::difference_type,
                   ptrdiff_t>);
    static_assert(same_as
                  <incrementable_traits<int *const>::difference_type,
                   ptrdiff_t>);
    static_assert(same_as<iter_difference_t<int *>, ptrdiff_t>);
  }

  // indirectly_readable_traits<T> // not used
  //   value_type
  // iter_value_t<T> // not used
  {
    static_assert(same_as
                  <indirectly_readable_traits<int *>::value_type,
                   int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int *>::value_type,
                   int>);
    static_assert(same_as<iter_value_t<int *>, int>);
  }

  // iter_reference_t<T> // not used
  {
    static_assert(same_as<iter_reference_t<int *>, int &>);
    static_assert(same_as<iter_reference_t<const int *>, const int &>);
  }

  // iterator_traits<T>
  {
    static_assert(same_as<iterator_traits<int *>::value_type, int>);
    static_assert(same_as<iterator_traits<const int *>::value_type, int>);
    static_assert(same_as<iterator_traits<int *>::reference, int &>);
    static_assert(same_as<iterator_traits<const int *>::reference,
                          const int &>);
    static_assert(same_as<iterator_traits<int *>::pointer, int *>);
    static_assert(same_as<iterator_traits<int *>::difference_type,
                          ptrdiff_t>);
    static_assert(same_as<iterator_traits<int *>::iterator_category,
                          contiguous_iterator_tag>);
  }

  // ranges::iter_move(i) // not used
  // ranges::iter_swap(i, ii) // not used
  {
    owner_ptr<int> p(1), pp(2);
    auto i = &p, ii = &pp;
    ranges::iter_swap(i, ii);
    assert(p != nullptr && *p == 2
           && pp != nullptr && *pp == 1);
    *i = ranges::iter_move(ii);
    assert(*p == 1 && pp == nullptr);
  }

  // indirectly_readable<I> // not used
  {
    static_assert(indirectly_readable<int *>);
  }
  // iter_common_reference<I> // not used
  {
    static_assert(same_as<iter_common_reference<int *>, int &>);
  }
  // indirectly_writable<I, T> // not used
  {
    static_assert(indirectly_writable<int *, int>);
    static_assert(indirectly_writable<int *, int &>);
    static_assert(indirectly_writable<int *, const int &>);
    static_assert(indirectly_writable<int *, int &&>);
  }
  // indirectly_movable<IN, OUT> // not used
  // indirectly_movable_storable<IN, OUT> // not used
  // indirectly_copyable<IN, OUT> // not used
  // indirectly_copyable_storable<IN, OUT> // not used
  {
    static_assert(indirectly_movable<int *, int *>);
    static_assert(indirectly_movable<int **, void **>);
    static_assert(indirectly_movable_storable<int *, int *>);
    static_assert(indirectly_movable_storable<int **, void **>);

    static_assert(indirectly_copyable<int *, int *>);
    static_assert(indirectly_copyable<int **, void **>);
    static_assert(indirectly_copyable_storable<int *, int *>);
    static_assert(indirectly_copyable_storable<int **, void **>);
  }
  // indirectly_swappable<I, II = I> // not used
  // indirectly_comparable<I, II, F, P = identity, P2 = identity> // not used
  {
    static_assert(indirectly_swappable<int *, int *>);
    static_assert(indirectly_comparable<int *, int *, equal_to<>>);
  }
  // weakly_incrementable<I> // not used
  // incrementable<I> // not used
  {
    static_assert(weakly_incrementable<int *>);
    static_assert(incrementable<int *>);
  }
  // input_or_output_iterator<I> // not used
  // sentinel_for<S, I> // not used
  // disable_sized_sentinel_for<S, I> // not used
  // sized_sentinel_for<S, I> // not used
  {
    static_assert(input_or_output_iterator<int *>);
    static_assert(sentinel_for<const int *, int *>);
    static_assert(sentinel_for<int *, const int *>);
    static_assert(!disable_sized_sentinel_for<int *, int *>);
    static_assert(sized_sentinel_for<int *, int *>);
  }
  // input_iterator<I> // not used
  // output_iterator<I, T> // not used
  // forward_iterator<I> // not used
  // bidirectional_iterator<I> // not used
  // random_access_iterator<I> // not used
  // contiguous_iterator<I> // not used
  {
    static_assert(input_iterator<int *>);
    static_assert(forward_iterator<int *>);
    static_assert(bidirectional_iterator<int *>);
    static_assert(random_access_iterator<int *>);
    static_assert(contiguous_iterator<int *>);
  }
  // cpp17_iterator<I> // not used
  // cpp17_input_iterator<I> // not used
  // cpp17_forward_iterator<I> // not used
  // cpp17_bidirectional_iterator<I> // not used
  // cpp17_random_access_iterator<I> // not used
  {
    static_assert(cpp17_iterator<int *>);
    static_assert(cpp17_input_iterator<int *>);
    static_assert(cpp17_forward_iterator<int *>);
    static_assert(cpp17_bidirectional_iterator<int *>);
    static_assert(cpp17_random_access_iterator<int *>);
  }
  // indirectly_unary_invocable<F, I> // not used
  // indirectly_regular_unary_invocable<F, I> // not used
  {
    const auto f = [](const int &) {};
    static_assert(indirectly_unary_invocable<decltype(f), int *>);
    static_assert(indirectly_regular_unary_invocable<decltype(f), int *>);
  }
  // indirect_unary_predicate<F, I> // not used
  // indirect_binary_predicate<F, I, II> // not used
  {
    const auto f = [](const int &) {return true;};
    static_assert(indirect_unary_predicate<decltype(f), int *>);
    const auto ff = [](const int &, void *&) {return true;};
    static_assert(indirect_binary_predicate<decltype(ff), int *, void **>);
  }
  // indirect_equivalence_relation<F, I, II = I> // not used
  // indirect_strict_weak_order<F, I, II = I> // not used
  {
    static_assert(indirect_equivalence_relation<equal_to<int>, int *, int *>);
    static_assert(indirect_strict_weak_order<less<int>, int *, int *>);
  }
  // indirect_result_t<F, ITERS...> // not used
  {
    const auto f = []() {return true;};
    const auto ff = [](int, int, int, int) {return true;};
    static_assert(same_as<indirect_result_t<decltype(f)>, bool>);
    static_assert(same_as<indirect_result_t
                          <decltype(ff), int *, int *, int *, int *>, bool>);
  }
  // projected<I, F> // not used
  {
    const auto f = [](const int &)->void * {return nullptr;};
    using pseudo_iter = projected<int *, decltype(f)>;
    static_assert(same_as<iter_value_t<pseudo_iter>, void *>);
    static_assert(same_as<iter_difference_t<pseudo_iter>, ptrdiff_t>);
    static_assert(same_as<iter_reference_t<pseudo_iter>, void *>);
  }
  // permutable<I> // not used
  // mergeable<I, II, OI, F = ranges::less, P = identity, PP = identity>
  //   // not used
  // sortable<I, F = ranges::less, P = identity> // not used
  {
    static_assert(permutable<int *>);
    static_assert(mergeable<int **, int **, void **>);
    static_assert(mergeable<int **, int **, void **,
                            ranges::less, identity, identity>);
    static_assert(sortable<int *>);
    static_assert(sortable<int *, less<int>, identity>);
  }
}
void doc_iterator_main_components() {
  // // itr series: the argument should has no const, volatile, and reference
  // itr_vt<I>
  //   // if I is class with optional const qualifier, I::value_type
  //   // if I is pointer to object T, remove_cv<T>
  //   // otherwise, void
  // itr_ptr<I>
  //   // if I is class with optional const qualifier, I::pointer
  //   // if I is pointer to object T, T *
  //   // otherwise, void
  // itr_ptr_from_ref<I>
  //   // if itr_ref<I> is not reference, void
  //   // otherwise, add_pointer<remove_reference<itr_ref<I>>>
  // itr_ref<I>
  //   // if I is class, I::reference
  //   // if I is pointer to object T, T &
  // itr_cref<I>
  //   // const reference if itr_ref<I> is real reference, otherwise itr_ref<I>
  // itr_rref<I>
  //   // rvalue reference if itr_ref<I> is real reference, otherwise itr_ref<I>
  // itr_common_ref<I>
  //   // used to define a variable to store one element by value
  //   // common_reference<add_lvalue_reference<itr_vt<I>>, itr_ref<I>>
  //   // or void if the common_reference is not existed
  // itr_dft<I>
  //   // if I is class with optional const qualifier, I::difference_type
  //   // if I is pointer to object T, ptrdiff_t
  //   // otherwise, void
  // itr_ctg<I>
  //   // if I is class with optional const qualifier, I::iterator_category
  //   // if I is pointer to object T, ptrdiff_t
  //   // otherwise, void
  // is_itr<I>
  //   // input_iterator_tag or output_iterator_tag is base of itr_ctg<I>
  // is_oitr<I> // output_iterator_tag is base of itr_ctg<I>
  // is_iitr<I> // input_iterator_tag is base of itr_ctg<I>
  // is_fitr<I> // forward_iterator_tag is base of itr_ctg<I>
  // is_bitr<I> // bidirectional_iterator_tag is base of itr_ctg<I>
  // is_ritr<I> // random_access_iterator_tag is base of itr_ctg<I>
  // is_citr<I> // contiguous_iterator_tag is base of itr_ctg<I>
  // is_just_iitr<I>
  // is_just_fitr<I>
  // is_just_bitr<I>
  // is_just_ritr<I>
  // is_itr_ctg<CTG>
  // is_oitr_ctg<CTG>
  // is_iitr_ctg<CTG>
  // is_fitr_ctg<CTG>
  // is_bitr_ctg<CTG>
  // is_ritr_ctg<CTG>
  // is_citr_ctg<CTG>
  // is_just_iitr_ctg<CTG>
  // is_just_fitr_ctg<CTG>
  // is_just_bitr_ctg<CTG>
  // is_just_ritr_ctg<CTG>

  // template_iterator_is_counted<I>
  // iterator_is_counted<I>
  // itr_is_counted<I> // short name of iterator_is_counted
  //   // can call minus operator for two I,
  //   //   and the return type is itr_dft<I>

  // advance(I &i, INTEGRAL n)
  // advance(I &i, S i2)
  // advance(I &i, itr_dft<I> n, S i2) // return n - distance(i, i2)
  //   // note:
  //   //   if n > 0, [i, i2) denotes a range,
  //   //   if n == 0, [i, i2) or [i2, i) denotes a range
  //   //   if n < 0, [i2, i) denotes a range
  // distance(I i, I i2)->itr_dft<I>
  // next(I i, itr_dft<I> n = 1)->i_t
  // next(I i, S i2)->i_t
  // next(I i, itr_dft<I> n, S i2)->i_t
  // prev(I i, itr_dft<I> n = 1)->i_t
  // prev(I i, itr_dft<I> n, S i2)->i_t

  // common_iter_t<A, B>
  //   // only used for rng(i1, i2)
  //   //   to get the iterator type can hold both argments
  //   //   (e.g. i1 is const int * and i2 is int *)
  {
    static_assert(is_same<common_iter_t<int *, const int *>, const int *>);
  }

  // template_common_iterator_category<CTG1, S...>
  //   // can not contain output_iterator_catagory
  //   // get the lowest catagory
  // common_iterator_category<CTG1, S...>
  {
    static_assert(is_same
                  <common_iterator_category
                   <rng_ctg<ez_vector<int>>,
                    rng_ctg<ez_list<int>>,
                    rng_ctg<ez_forward_list<int>>>,
                   forward_iterator_tag>);
   }

  // iter_post_increment_return_type<vt>
  //   // store the previous deref-value of input iterator
  //   special member functions: only move-constructible
  //   explicit this_t(T &&) // fill the private vt value by any argument
  //   *() &&->vt
  //
  // iter_post_increment(I i)->{I or iter_post_increment_return_type<itr_vt<I>>}
  //   // do i++ by ++i
  // iter_post_decrement(I i)->I
  //   // do i-- by --i
  {
    int a[] = {1, 2};
    int *i = begin(a);
    iter_post_increment_return_type<int> tmp(*i);
    ++i;
    assert(*move(tmp) == 1);
  }
}
void doc_range_main_components() {
  // begin(r)->rng_itr<r_t>
  // end(r)->rng_itr<r_t>
  // size(r)->rng_szt<r_t>
  // ssize(r)->make_signed<rng_szt<r_t>>
  // empty(r)->bool
  // is_rng<R> // just check lvalue reference no matter R is value or reference

  // n_value_iterator<T>
  //   iterator category: ritr
  //   n_value_iterator(T &, ptrdiff_t)
  //   base()->T *
  // n_value_itr(x, n)->n_value_iterator<...>
  // n_value<T>
  //   special member functions: dependent on T
  //   explicit(...) n_value(n, s...)
  // template_range_is_n_value<R>
  // range_is_n_value<R>
  {
    int x = 1;
    assert(n_value_itr(x, 0).base() == addressof(x));
    assert(equal(rng(n_value_itr(x, 0), n_value_itr(x, 3)), seq(1, 1, 1)));
    x = 2;
    assert(equal(rng(n_value_itr(x, 0), n_value_itr(x, 3)), seq(2, 2, 2)));

    assert(equal(n_value<int>(2, 3), seq(3, 3)));
    assert(equal(rng(2, 3), seq(3, 3)));
  }

  // // rng series:
  // //   get void if not well-formed
  // rng_itr<R>
  // rng_szt<R>
  // rng_vt<R> // itr_vt<rng_itr<R>>
  // rng_ptr<R> // itr_ptr<rng_itr<R>>
  // rng_ptr_from_ref<R> // itr_ptr_from_ref<rng_itr<R>>
  // rng_ref<R> // itr_ref<rng_itr<R>>
  // rng_cref<R> // itr_cref<rng_itr<R>>
  // rng_rref<R> // itr_rref<rng_itr<R>>
  // rng_common_ref<R> // itr_common_ref<rng_itr<R>>
  // rng_dft<R> // itr_dft<rng_itr<R>>
  // rng_ctg<R> // itr_ctg<rng_itr<R>>
  // rng_is_sized<R> // has member function size or is bounded-array
  // rng_is_counted<R> // itr_is_counted<rng_itr<R>>
  // rng_is_n_value<R> // is_rng<R> && range_is_n_value<R>

  // range_traits<R>
  //   iterator // = rng_itr<R>
  //   value_type // = rng_vt<R>
  //   reference  // = rng_ref<R>
  //   difference_type // = rng_dft<R>
  //   pointer // = rng_ptr<R>
  //   iterator_category // = rng_ctg<R>
  //   const_reference // = rng_cref<R>
  //   rvalue_reference // = rng_rref<R>
  //   common_reference // = rng_common_ref<R>
  //   size_type // = rng_szt<R>
  //   is_sized // = bool_constant<rng_is_sized<R>>
  //   is_counted // = bool_constant<rng_is_counted<R>>
  //   is_n_value // = bool_constant<rng_is_n_value<R>>

  // is_orng<R> // is_rng<R> && is_oitr<rng_itr<R>>
  // is_irng<R> // is_rng<R> && is_iitr<rng_itr<R>>
  // is_frng<R> // is_rng<R> && is_fitr<rng_itr<R>>
  // is_brng<R> // is_rng<R> && is_bitr<rng_itr<R>>
  // is_rrng<R> // is_rng<R> && is_ritr<rng_itr<R>>
  // is_crng<R> // is_rng<R> && is_citr<rng_itr<R>>
  // is_just_irng<R> // is_rng<R> && is_just_iitr<rng_itr<R>>
  // is_just_frng<R> // is_rng<R> && is_just_fitr<rng_itr<R>>
  // is_just_brng<R> // is_rng<R> && is_just_bitr<rng_itr<R>>
  // is_just_rrng<R> // is_rng<R> && is_just_ritr<rng_itr<R>>

  // is_iter_pair<R>
  {
    static_assert(is_iter_pair<iter_pair<int *>>);
    static_assert(is_iter_pair<const iter_pair<int *>>);
    static_assert(is_iter_pair<iter_pair<int *> &>);
    static_assert(is_iter_pair<const iter_pair<int *> &>);
    static_assert(is_iter_pair<iter_pair<int *> &&>);
    static_assert(is_iter_pair<const iter_pair<int *> &&>);

    struct t : iter_pair<int *> {};
    static_assert(is_iter_pair<t>);
  }

  // template_is_range_reference<R>
  // is_range_reference<R>
  // is_rng_ref<R> // = is_range_reference<R>::value
  //   // range reference means:
  //   //   (1) the underlying sequence is not stored in R
  //   //   (2) const R and R share the same iterator type
  {
    static_assert(is_rng_ref<iter_pair<int *>>);
    static_assert(is_rng_ref<ez_vector<int> &>);
    static_assert(!is_rng_ref<ez_vector<int>>);
  }

  // rng_forward_t<R>
  //   // used store R automatically by value or by reference
  //   // array value can not occur in function argument list,
  //   //   so there is not worry to consider it
  {
    static_assert(same_as<rng_forward_t<int (&)[20]>, int (&)[20]>);

    static_assert(same_as<rng_forward_t<iter_pair<int *> &>, iter_pair<int *>>);
    static_assert(same_as<rng_forward_t<const ez_vector<int> &>,
                          const ez_vector<int> &>);
    static_assert(same_as<rng_forward_t<ez_vector<int> &&>,
                          ez_vector<int>>);
  }

  // before_begin(r) // call r.before_begin()
  // before_end(r) // !empty(r), call r.before_end() or prev(end(r))
  // cbegin(r)
  // cend(r)
  // rbegin(r)
  // rend(r)
  // crbegin(r)
  // crend(r)
  // data(r) // r.data() or begin iterator of array or initializer_list
  // ref(R &r, rng_dft<R>)->rng_ref<R> // valid arguments
  // cref(const R &r, rng_dft<const R>)->rng_ref<const R> // valid arguments
  // nth(R &r, rng_dft<R>)->rng_itr<R> // valid arguments
  // cnth(const R &r, rng_dft<R>)->rng_itr<const R> // valid arguments  
  // at_most_nth(R &r, rng_dft<R>)->rng_itr<R>
  // front(R &r)->rng_ref<R>
  //   // !empty(r) and well formed for
  //   //   r.front(), *r.begin()
  // back(R &r)->rng_ref<R>
  //   // !empty(r) and well formed for
  //   //   r.back(), *r.before_end() or *prev(end(r))
  // range_fns
  //   static begin(r)
  //   static end(r)
  //   static rbegin(r)
  //   static rend(r)
  //   static size(r)
  //   static empty(r)
  //   static nth(r, n)
  //   static ref(r, n)
  //   static front(r)
  //   static back(r)
  //   static cbegin(r)
  //   static cend(r)
  //   static crbegin(r)
  //   static crend(r)
  //   static ssize(r)
  //   static data(r)
  //   static before_begin(r)
  //   static before_end(r)
}
void doc_degraded_iterator() {
  // degraded_iterator<I, CTG> // is_base_of<CTG, itr_ctg<I>>
  //   iterator_type = I
  //   iterator category: CTG = iitr - citr
  //   reference: itr_ref<I>
  //   degraded_iterator(const degraded_iterator<I2> &)
  //   explicit degraded_iterator(I)
  //   base()->I
  // degraded_input_iterator = degraded_iterator<...>
  // degraded_forward_iterator = degraded_iterator<...>
  // degraded_bidirectional_iterator = degraded_iterator<...>
  // degraded_random_access_iterator = degraded_iterator<...>
  // degraded_itr<TAG>(I)->degraded_iterator<I, TAG>
  //   // TAG is no more than itr_ctg<I>
  // degraded_iitr(I)->degraded_iterator<I, input_iterator_tag>
  // degraded_fitr(FI)->degraded_iterator<FI, forward_iterator_tag>
  // degraded_bitr(BI)->degraded_iterator<BI, bidirectional_iterator_tag>
  // degraded_ritr(RI)->degraded_iterator<RI, random_access_iterator_tag>
  {
    int *const p{};
    static_assert(is_citr<decltype(p)>);
    auto i = degraded_iitr(p);
    static_assert(is_just_iitr<decltype(i)>);
    assert(i.base() == p);
  }
}
void doc_reverse_iterator() {
  // reverse_iterator<BI> // is_bitr<BI>
  //   iterator_type = BI
  //   iterator category: itr_ctg<BI> = bitr - citr
  //   reference: itr_ref<BI>
  //   reverse_iterator(const reverse_iterator<BI2> &)
  //   =(const reverse_iterator<BI2> &)
  //   explicit reverse_iterator(BI)
  //   base()->BI
  // make_reverse_iterator(BI)->reverse_iterator<BI> // is_bitr<BI>
  // ritr(BI)->auto // return make_reverse_iterator(bi) or bi.base()
  {
    vector<int> v = {1, 2, 3};
    reverse_iterator<vector<int>::iterator> i(end(v)), ii(begin(v));
    assert(equal(rng(ii.base(), i.base()), seq(1, 2, 3)));
    assert(equal(rng(i, ii), seq(3, 2, 1)));
    assert(equal(rng(ritr(ii), ritr(i)), seq(1, 2, 3)));
  }
}
void doc_insert_iterator() {
  // back_insert_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   reference: void
  //   special member functions: no default constructor
  //   back_insert_iterator(C &)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // back_inserter(C &)->back_insert_iterator<C>
  //
  // front_insert_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   reference: void
  //   special member functions: no default constructor
  //   front_insert_iterator(C &)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // front_insert_inserter(C &)->front_insert_inserter<C>
  //
  // insert_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   reference: void
  //   special member functions: no default constructor
  //   insert_iterator(C &, C::iterator)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // inserter(C &, C::iterator)->insert_iterator<C>
  //
  // to_back_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   reference: void
  //   special member functions: no default constructor
  //   to_back_iterator(C &)
  //   to_back_iterator(C &&)
  //   base() &&->C
  // to_back(C &&)->to_back_iterator<remove_reference<C>>
  //
  // to_front_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   reference: void
  //   special member functions: no default constructor
  //   to_front_iterator(C &)
  //   to_front_iterator(C &&)
  //   base() &&->C
  // to_front(C &&)->to_front_iterator<remove_reference<C>>
  //
  // any_output_iterator<UF>
  //   iterator category: oitr
  //   reference: void
  //   special member functions: dependent
  //   explicit any_output_iterator(UF)
  //   =(x)
  // output_itr(F)->any_output_iterator<F>
  {
    list<int> v;

    auto o = output_itr([&v](const int &i) {v.push_back(i);});
    *o++ = 1;
    assert(equal(v, seq(1)));

    auto o2 = back_inserter(v);
    *o2++ = 3;
    assert(equal(v, seq(1, 3)));

    auto o3 = inserter(v, next(begin(v)));
    *o3++ = 2;
    *o3++ = 4;
    assert(equal(v, seq(1, 2, 4, 3)));

    auto o4 = front_inserter(v);
    *o4++ = 0;
    assert(equal(v, seq(0, 1, 2, 4, 3)));

    v.clear();
    copy(seq(1, 2, 3), to_back(v));
    assert(equal(v, seq(1, 2, 3)));

    assert(copy(seq(2, 4, 6), to_front(vector<int>{})).base()
           == vector<int>({2, 4, 6}));
  }
}
void doc_move_iterator() {
  // move_iterator<II>
  //   iterator_type = II
  //   iterator category: itr_ctg<II> = iitr - citr
  //   reference: itr_rref<II>
  //   move_iterator(const move_iterator<II2> &)
  //   =(const move_iterator<II2> &)
  //   explicit move_iterator(II)
  //   base()->II
  // make_move_iterator(II)->move_iterator<II>
  // move_itr(II)->move_iterator<II> // = make_move_iterator(i)
  {
    string a[2] = {"ab"_s, "cd"_s};
    string b[2]{};
    copy(rng(move_itr(begin(a)), move_itr(end(a))), begin(b));
    assert(equal(a, seq(""_sv, "")));
    assert(equal(b, seq("ab"_sv, "cd")));
  }
}
void doc_counted_iterator() {
  // counted_iterator<I>
  //   // I with a count() to record the length to the maybe-vitual end-position
  //   iterator_type = I
  //   iterator category: itr_ctg<I>
  //   reference: itr_ref<I>
  //   special member functions: dependent
  //   counted_iterator(const counted_iterator<I2> &)
  //   =(const counted_iterator<I2> &)
  //   explicit counted_iterator(I i, itr_dft<I> n) // n >= 0
  //   base()->I
  //   count()->itr_dft<I>
  // make_counted_iterator(I i, itr_dft<I> n)->counted_iterator<I> // n >= 0
  // counted_itr(I i, itr_dft<I> n)->counted_iterator<I>
  //   // = make_counted_iterator(i, n)
  {
    const ez_forward_list<int> l = {1, 2, 3};
    const auto i1 = counted_itr(begin(l), 3);
    const auto i2 = counted_itr(end(l), 0);
    assert(equal(rng(i1, i2), l));
    assert(i2 - i1 == 3);
    static_assert(rng_is_counted<decltype(rng(i1, i2))>);
    static_assert(!rng_is_counted<decltype(l)>);
  }
}
void doc_iterator_wrapper() {
  // iterator_wrapper<I>
  //   iterator_type = I
  //   iterator category: itr_ctg<I> = iitr - citr, or oitr
  //   reference: itr_ref<I>
  //   special member functions: dependent
  //   iterator_wrapper(const iterator_wrapper<I> &)
  //   =(const iterator_wrapper<I> &)
  //   explicit iterator_wrapper(I)
  //   base()->I
  // wrap_itr(I)->iterator_wrapper<I>
  {
    int *const p{};
    assert(wrap_itr(p).base() == p);
  }
}
void doc_array() {
  // array<T, size_t N> // N can be 0
  //   pointer = T *
  //   const_pointer = const T *
  //
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //
  //   iterator = T *
  //   const_iterator = const T *
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   <=> for (const array<T, N> &, const array<T, N> &)->synth_3way_result<T>
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //
  //   empty()->bool
  //   size()->size_type
  //   max_size()->size_type
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()->(const_)reverse_iterator
  //   rend()->(const_)reverse_iterator
  //   crbegin()->const_reverse_iterator
  //   crend()->const_reverse_iterator
  //
  //   front()->(const_)reference // !empty()
  //   back()->(const_)reference // !empty()
  //   operator [](size_type i)->(const_)reference
  //   at(size_type i)->(const_)reference // may throw out_of_range
  //
  //   data()->(const_)pointer
  //   fill(const T &)
  //
  // array(T, S...)->array<T, 1 + sizeof...(S)>
  // to_array(T (&a)[N])->array<remove_cv<T>, N>
  // seq(X &&, S &&...)->array<decay<X>, 1u + sizeof...(S)>
  //
  // get<size_t I>(array<T, N>_ref)->T_ref
  {
    const array<int, 3> a = seq(1, 2, 3);
    assert(equal(a, irng(1, 4)));

    const int b[] = {1, 2, 3};
    assert(equal(to_array(b), irng(1, 4)));

    ez_vector<int> c[] = {{1}};
    assert(equal(front(c), single_rng(1)));
    array<ez_vector<int>, 1> d = to_array(move(c));
    assert(front(c).empty());
    assert(equal(front(d), single_rng(1)));
  }
}
void doc_iter_pair() {
  // iter_pair<I> : public pair<I, I>
  //   cache: no
  //   iterator category: itr_ctg<I>
  //   reference: itr_ref<I>
  //   special member functions: dependent
  //   == with iter_pair<I2>
  //   explicit(...) iter_pair(const iter_pair<X> &)
  //   operator =(const iter_pair<X> &)
  //   iter_pair(i, ii)
  // iter_pair()->iter_pair<nullptr_t>
  // iter_pair(I, I)->iter_pair<I>
  // rng(I1, I2)->iter_pair<common_iter_t<I1, I2>>
  //   // !is_integral<I1> && !is_integral<I2>
  // rng(R &&)->iter_pair<rng_itr<R>>
  //   // is_rng<R> && is_rng_ref<R>
  {
    const int a[] = {1, 2, 3};
    iter_pair<const int *> v1 = rng(begin(a), end(a));
    assert(equal(v1, seq(1, 2, 3)));
    iter_pair<const int *> v2 = rng(a);
    assert(equal(v2, seq(1, 2, 3)));
  }
}
void doc_composite_range() {
  // // composite_..._iterator: implement operations by lambda arguments
  // composite_input_iterator<T, DEREF, PP, EQ>
  //   iterator category: iitr
  //   reference: decltype(declval<const DEREF &>()(declval<const T &>()))
  //   composite_input_iterator(const T &, deref_f, pp_f, eq_f)
  //   base()->T
  // composite_forward_iterator<T, DEREF, PP, EQ>
  //   iterator category: fitr
  //   reference: decltype(declval<const DEREF &>()(declval<const T &>()))
  //   composite_forward_iterator(const T &, deref_f, pp_f, eq_f)
  //   base()->T
  // composite_bidirectional_iterator<T, DEREF, PP, EQ, MM>
  //   iterator category: bitr
  //   reference: decltype(declval<const DEREF &>()(declval<const T &>()))
  //   composite_forward_iterator(const T &, deref_f, pp_f, eq_f, mm_f)
  //   base()->T
  //
  // composite_irng(x, y, df, pp, eq)->iter_pair<composite_iterator>
  // composite_frng(x, y, df, pp, eq)->iter_pair<composite_iterator>
  // composite_brng(x, y, df, pp, eq, mm)->iter_pair<composite_iterator>
  {
    auto r = composite_brng(1, 4,
                            identity{},
                            [](int &x) {++x;},
                            equal_to{},
                            [](int &x) {--x;});
    assert(equal(r, seq(1, 2, 3)));
  }
}
void doc_range_wrapper() {
  // range_wrapper<R>
  //   // to make built-in array copyable
  //   cache: no
  //   iterator category: rng_ctg<R>
  //   reference: rng_ref<R>, rng_ref<const R>
  //   special member functions: dependent
  //   explicit range_wrapper(R)
  // wrap_rng(R &&)->range_wrapper<rng_forward_t<R>>
  // wrap_rng(x, y)->decltype(wrap_rng(rng(x, y)))

  int a[] = {1, 2, 3};
  auto r = wrap_rng(a);
  assert(equal(r, seq(1, 2, 3)));
}
void doc_base_range() {
  // base_range<R>
  //   // get range of [r.base().begin(), r.base().end())
  //   cache: no
  //   iterator category: itr_ctg<decltype(begin(declval<R &>()).base())>
  //   reference:
  //     itr_ref<decltype(begin(declval<R &>()).base())>,
  //     itr_ref<...<const R &>...>
  //   special member functions: dependent
  //   explicit base_range(R)
  // base_rng(R &&)->base_range<rng_forward_t<R>>
  // base_rng(i, ii)->decltype(base_rng(rng(i, ii)))
  // base_rng(i, n)->decltype(base_rng(rng(i, n)))
  //
  // inplace_base_range<R>
  //   // inplace means:
  //   //   distance(begin(r).base(), end(r).base()) == ssize(r),
  //   //     so just call r.empty() and r.size(),
  //   //     rather than operations of r.base().begin() and r.base().end()
  //   cache: no
  //   iterator category: itr_ctg<decltype(begin(declval<R &>()).base())>
  //   reference:
  //     itr_ref<decltype(begin(declval<R &>()).base())>,
  //     itr_ref<...<const R &>...>
  //   special member functions: dependent
  //   explicit inplace_base_range(r)
  // inplace_base_rng(R &&)->inplace_base_range<rng_forward_t<R>>
  // inplace_base_rng(i, ii)->decltype(inplace_base_rng(rng(i, ii)))
  // inplace_base_rng(i, n)->decltype(inplace_base_rng(rng(i, n)))
}
void doc_empty_range() {
  // empty_range<T>
  //   cache: no
  //   iterator category: citr
  //   reference: T &
  //   special member functions: full
  // empty_rng<T>()->empty_range<T>
  {
    auto r = empty_rng<int>();
    assert(empty(r));
  }
}
void doc_single_range() {
  // single_range<T>
  //   cache: no
  //   iterator category: citr
  //   reference: T &, const T &
  //   special member functions: dependent
  //   explicit single_range(T)
  //   explicit single_range(in_place, s...)
  // single_rng(x)->single_range // take value
  // single_rng(ref(x))->single_range // take reference
  {
    int i = 1;
    auto r = single_rng(ref(i));
    front(r) = 2;
    assert(i == 2);
  }
}
void doc_counted_range() {
  // counted_range<R>
  //   // R with explicitly recorded size
  //   // copy/move-constructed new R has unchanged size than source
  //   cache: no
  //   iterator category: rng_ctg<R>
  //   reference: rng_ref<R>, rng_ref<const R>
  //   special member functions: dependent
  //   explicit counted_range(R r)
  //   counted_range(R r, rng_dft<R> n) // n == size(r)
  // counted_rng(fr)->counted_range
  // counted_rng(r, n)->counted_range // n == size(r)
  // sized_rng(fr)->decltype(inplace_base_rng(counted_rng(fr)))
  // sized_rng(r, n)->decltype(inplace_base_rng(counted_rng(r))) // n == size(r)
}
void doc_degraded_range() {
  // degraded_range<R, CTG> // is_base_of<CTG, rng_ctg<R>>
  //   cache: no
  //   iterator category: CTG = no more than rng_ctg<R>
  //   reference: rng_ref<R>, rng_ref<const R>
  //   special member functions: dependent
  //   explicit degraded_range(R r)
  // degraded_input_range = degraded_range<...>
  // degraded_forward_range = degraded_range<...>
  // degraded_bidirectional_range = degraded_range<...>
  // degraded_random_access_range = degraded_range<...>
  // degraded_rng<TAG>(r)->degraded_range
  // degraded_rng<TAG>(i, i2)->degraded_range // = degrade_rng<TAG>(rng(i, i2))
  // degraded_rng<TAG>(i, n)->degraded_range // = degrade_rng<TAG>(rng(i, n))
  // degraded_irng(r)->degraded_range
  // degraded_irng(i, i2)->degraded_range // = degraded_irng(rng(i, i2))
  // degraded_irng(i, n)->degraded_range // = degraded_irng(rng(i, n))
  // degraded_frng(r)->degraded_range
  // degraded_frng(i, i2)->degraded_range // = degraded_frng(rng(i, i2))
  // degraded_frng(i, n)->degraded_range // = degraded_frng(rng(i, n))
  // degraded_brng(r)->degraded_range
  // degraded_brng(i, i2)->degraded_range // = degraded_brng(rng(i, i2))
  // degraded_brng(i, n)->degraded_range // = degraded_brng(rng(i, n))
  // degraded_rrng(r)->degraded_range
  // degraded_rrng(i, i2)->degraded_range // = degraded_brng(rng(i, i2))
  // degraded_rrng(i, n)->degraded_range // = degraded_brng(rng(i, n))
}
void doc_move_range() {
  // move_range<R>
  //   cache: no
  //   iterator category: rng_ctg<R>
  //   reference: rng_rref<R>, rng_rref<const R>
  //   special member functions: dependent
  //   explicit move_range(R r)
  // move_rng(R &&r)->move_range<rng_forward_t<R>>
  // move_rng(i1, i2)->decltype(move_rng(rng(i1, i2)))
  // move_rng(i1, n)->decltype(move_rng(rng(i1, n)))
}
void doc_reverse_range() {
  // reverse_range<R>
  //   cache: no
  //   iterator category: rng_ctg<R> = bitr - ritr
  //   reference: rng_ref<R>, rng_ref<const R>
  //   special member functions: dependent
  //   explicit move_range(R r)
  // rrng(BR &&r)->reverse_range<rng_forward_t<BR>>
  // rrng(i1, i2)->decltype(rrng(rng(i1, i2)))
  // rrng(i1, n)->decltype(rrng(rng(i1, n)))
}
void doc_rng_for_iterator_n() {
  // iter_n_iterator<I>
  //   // similar to counted_iterator,
  //   //   but make it at most forward-iterator
  //   //   to avoid reverse scan from the end iterator
  //   iterator_type = I
  //   iterator category: iitr - fitr
  //   reference: itr_ref<I>
  //   iter_n_iterator(const iter_n_iterator<I2> &)
  //   iter_n_iterator(I i, itr_dft<I> n) // n > 0
  //   base()->I
  //   nth()->itr_dft<I>
  // iter_n_itr(I i, itr_dft<I> n)->iter_n_iterator<I> // n > 0
  // rng(i, n)->iter_pair // n > 0, if not ritr, use iter_n_iterator
  {
    const ez_list<int> l = {1, 2, 3};
    const auto r = rng(l.begin(), 3);
    assert(is_just_frng<decltype(r)>);
      // use iter_n_iterator, so category is degraded
    const auto r2 = rng(l.begin(), nth(l, 3));
    assert(is_just_brng<decltype(r2)>);
  }
}
void doc_rng_for_n_value() {
  // rng(n, x)->n_value // take value // n > 0
  // rng(n, ref(x))->n_value<x_cref_t> // take reference // n > 0
  {
    int x = 1;
    auto r = rng(3, ref(x));
    assert(equal(r, seq(1, 1, 1)));
    assert(addressof(front(r)) == addressof(x));
  }
}
void doc_iterator_range() {
  // iterator_iterator<I>
  //   iterator category:
  //     conditional<is_citr<I>, random_access_iterator_tag, itr_ctg<I>>,
  //     or random_access_iterator_tag for non-iterator (e.g. integral type)
  //   reference: I
  //   explicit iterator_iterator(i)
  //   base()->I
  // iitr(I)->iterator_iterator<I>
  //
  // iterator_range<R>
  //   cache: no
  //   iterator category: itr_ctg<iterator_iterator<rng_itr<R>>>
  //   reference: rng_itr<R>, rng_itr<const R>
  //   special member functions: dependent
  //   explicit iterator_range(R)
  // irng(R &&)->iterator_range<rng_forward_t<R>>
  // irng(n1, n2)->iter_pair<iterator_iterator<common_type<...>>>
  // irng(i1, i2)->iter_pair<iterator_iterator<common_iter_t<...>>>
  // irng(i1, n)->decltype(irng(rng(i1, n)))
  {
    assert(equal(irng(0, 3), seq(0, 1, 2)));
  }
}
void doc_bind_range() {
  // bind_iterator<II, UF>
  //   // UF has no cvref, f(*i) is well formed
  //   iterator category:
  //     conditional<is_ritr<II>, random_access_iterator_tag, itr_ctg<II>>
  //   reference:
  //     decltype(call_or_deref_call
  //              (declval<const F &>(), declval<itr_ref<II>>()))
  //   special member functions: full
  //   bind_iterator(const bind_iterator<II2, UF2> &) // enable if convertible
  //   bind_iterator(II, UF)
  //   base()->II
  //   fn()->UF
  // bind_itr(II, UF)->bind_iterator<II, UF>
  //
  // bind_range<R, UF>
  //   cache: no
  //   iterator category: itr_ctg<bind_iterator<rng_itr<R, UF>>>
  //   reference: itr_ref<bind_iterator<rng_itr<R, UF>>>, itr_ref<...const R...>
  //   special member functions: dependent
  //   bind_range(R, UF)
  // bind_rng(R &&, UF)->bind_range<rng_forward_t<R>, UF>
  //
  // deref_rng(r)->auto
  // deref_rng(i1, i2)->decltype(deref_rng(rng(i1, i2)))
  // deref_rng(i1, n)->decltype(deref_rng(rng(i1, n)))
  // deref_rng(n, x)->decltype(deref_rng(rng(n, x)))
  // deref_itr(i)->auto
  //
  // const_rng(r)->auto
  // const_rng(i1, i2)->decltype(const_rng(rng(i1, i2)))
  // const_rng(i1, n)->decltype(const_rng(rng(i1, n)))
  // const_rng(n, x)->decltype(const_rng(rng(n, x)))
  // const_itr(i)->auto
  {
    assert(equal(bind_rng(seq(1, 2, 3), [](int x) {return x + 1;}),
                 seq(2, 3, 4)));

    auto r = const_rng(irng(0, 3));
    static_assert(is_same<rng_ref<decltype(r)>, int>);
    int a[] = {1, 2, 3};
    auto r2 = const_rng(a);
    static_assert(is_same<rng_ref<decltype(r2)>, const int &>);
  }
}
void doc_cache_latest_range() {
  // // make an input range to make repeated deref operations consume
  // //   only one real deref operation
  // //   (try cache address firstly, and cache value if reference is not real)
  //
  // iterator_reference_cache<I>
  //   // store pointer for real reference, and store value for value
  //   special member functions: dependent
  //   =(itr_ref<I>)->this_t &
  //   reset()
  //   explicit operator bool()
  //   empty()->bool
  //   *()->itr_(c)ref<I>
  //
  // cache_latest_iterator<I, X = iterator_reference_cache<I>>
  //   iterator category: iitr
  //   reference: decltype(*declval<X &>())
  //   special member functions: full
  //   base()->I
  // cache_latest_itr(I, iterator_reference_cache<I> &)
  //   ->cache_latest_iterator<I>
  //
  // cache_latest_range<R>
  //   cache: yes, the cached value can be copied or moved to a new instance
  //   iterator category: iitr
  //   reference: itr_ref<cache_latest_iterator<rng_itr<R>>>
  //   special member functions: dependent
  //   explicit this_t(R)
  // cache_latest_rng(R &&)->cache_latest_range<rng_forward_t<R>>

  auto r = bind_rng(irng(0, 3), [](int x) {return x * x * x;});
  auto r2 = cache_latest_rng(r);
  static_assert(is_just_irng<decltype(r2)>);
  assert(equal(r2, seq(0, 1, 8)));
}
void doc_iters() {
  // for_iter_iterator<IT> // only used for iters()
  // for_iter_range<R> // only used for iters()
  // iters(i1, i2)->for_iter_range<iter_pair<common_iter_t<i1_t, i2_t>>>
  // iters(r)->for_iter_range<rng_forward_t<R>>
  //
  // r_for_iter_iterator<IT> // should only be used for r_iters
  // r_for_iter_range<R> // should only be used for r_iters
  // r_iters(i1, i2)->r_for_iter_range<iter_pair<common_iter_t<i1_t, i2_t>>>
  // r_iters(r)->r_for_iter_range<rng_forward_t<R>>
  {
    vector<int> v;
    for (int &x : iters(0, 3))
      v.push_back(x);
    assert(equal(v, irng(0, 3)));
    v.clear();
    for (int &x : r_iters(0, 3))
      v.push_back(x);
    assert(equal(v, rrng(irng(0, 3))));
  }
}
void doc_range_miscl() {
  // can_memmove_from_range_to_iterator<R, I>
  {
    static_assert(can_memmove_from_range_to_iterator<int[2], int *>);
    static_assert(!can_memmove_from_range_to_iterator<ez_list<int>, int *>);
  }
}

// allocator
void doc_allocator_traits() {
  // allocator_traits<AL>
  //   allocator_type
  //   value_type
  //   pointer
  //   const_pointer
  //   void_pointer
  //   const_void_pointer
  //   difference_type
  //   size_type
  //   propagate_on_container_copy_assignment
  //     value
  //   propagate_on_container_move_assignment
  //     value
  //   propagate_on_container_swap
  //     value
  //   is_always_equal
  //     value
  //   rebind_alloc<T>
  //   rebind_traits<T>
  //   static allocate(al)->pointer // may throw
  //   static allocate(al, n)->pointer // may throw
  //   static allocate(al, n, const_void_pointer hint)->pointer // may throw
  //   static construct(al, p, s...) // may throw
  //   static destroy(al, p)
  //   static deallocate(al, p)
  //   static deallocate(al, p, n)
  //   static max_size(al)->size_type
  //   static select_on_container_copy_construction(al)->allocator_type
  //
  // alloc_vt<A>
  // alloc_ptr<A>
  // alloc_cptr<A>
  // alloc_void_ptr<A>
  // alloc_const_void_ptr<A>
  // alloc_dft<A>
  // alloc_szt<A>
  // alloc_rebind<A, T>
  // alloc_rebind_traits<A, T>
  // alloc_rebind_ptr<A, T>
  // alloc_rebind_cptr<A, T>
  // alloc_copy_prpg<A>
  // alloc_move_prpg<A>
  // alloc_swap_prpg<A>
  // alloc_always_equal<A>
  //
  // allocator_with_primary_traits<AL> // allocator is not specialized
  // allocator_provides_construct_function<AL, T, S...>
  // allocator_provides_destroy_function<AL, T>
  // nothrow_constructible_by_allocator<AL, T, S...>
  //   // AL provide nothrow construct function
  //   // or T is nothrow-constructible by S... and AL provide no construct()
  // nothrow_move_constructible_by_allocator<AL, T>
  //   // AL provide nothrow construct function
  //   // or T is nothrow-move-constructible and AL provide no construct()
}
void doc_uninitialized_argo() {
  // default_construct_non_array_at(T *)->T * // may throw
  // construct_non_array_at(T *, S &&...)->T * // may throw
  // construct_at(T *, S &&...)->T * // same as construct_non_array_at
  // destroy_non_array_at(T *)
  //
  // destroy_at(T *p) // *p can be array
  // destroy_at(T *p, D d) // call d(p) for every element
  // destroy(r)
  // destroy(r, d) // call d(addressof(...)) for every element
  //
  // default_construct_array(p) // p is pointer to bounded array
  // default_construct_array(p, f, d)
  // construct_array(p)
  // construct_array(p, f, d) // call f(element_ptr) to initialize each element
  // construct_array(p, fll)
  // construct_array(p, fll, f, d)
  //   // call f(element_ptr, fll) to initialize each element
  // default_construct_maybe_array(p)
  // default_construct_maybe_array(p, f, d)
  // construct_maybe_array(p)
  // construct_maybe_array(p, f, d)
  // construct_maybe_array(p, fll)
  // construct_maybe_array(p, fll, f, d)
  //
  // initialize(fr, fn) // may throw
  //   // call fn(element_ptr) to initialize each element
  // initialize_plus(fr, fn)->fr_i // may throw
  // initialize(ir, fi, fn)->fi_itr // may throw
  //   // call fn(ir_itr, fi_to_address) to initialize [fi, next(fi, size(ir)))
  // initialize_plus(ir, fi, fn)->pair<ir_itr, fi_itr> // may throw
  //
  // securely_initialize(fr, fn, destroy_fn) // may throw
  // securely_initialize_plus(fr, fn, destroy_fn)->fr_itr // may throw
  //   // iterator-range operations of fr can not throw
  // securely_initialize(fr, fi, fn, destroy_fn)->fi_itr // may throw
  // securely_initialize_plus(fr, fi, fn, destroy_fn)->pair<fr_itr, fi_itr>
  //   // may throw
  //   // iterator-range operations of fi can not throw
}
void doc_default_allocator() {
  // allocator<T>
  //   value_type = T
  //   size_type = size_t
  //   difference_type = ptrdiff_t
  //   propagate_on_container_move_assignment = true_type
  //
  //   special member functions: full
  //   == for (const allocator<T1> &, const allocator<T2> &)
  //   allocator(const allocator<U> &)
  //   allocate(size_type)->T *
  //   deallocate(T *, size_type)
  //   max_size()->size_type
  //   min_alignment()->size_type // return __STDCPP_DEFAULT_NEW_ALIGNMENT__
  //
  // default_allocator<T> = RE_DEFAULT_ALLOCATOR<T> // allocator<T> ifndef
}
void doc_allocator_wrapper() {
  // allocator_wrapper<AL>
  //   allocator_type = AL
  //   traits = allocator_traits<AL>
  //
  //   special member functions: full
  //
  //   explicit allocator_wrapper(a)
  //   explicit(sizeof...(s) == 1) allocator_wrapper(s...)
  //
  //   get()->AL_ref
  //   rebind<T>()->allocator_wrapper<alloc_rebind<AL, T>>
  //   max_size()->alloc_szt<AL>
  //   min_alignment()->alloc_szt<AL>
  //
  //   allocate_alignas(algn, n = 1)->pair<offset_ptr, origin_ptr> // may throw
  //     // is_same<alloc_vt<AL>, byte>
  //   deallocate_alignas(algn, pair, n = 1)
  //     // is_same<alloc_vt<AL>, byte>
  //
  //   allocate(n = 1)->alloc_ptr<AL> // may throw
  //   deallocate(alloc_ptr<AL>, n = 1)
  //   construct(T *, S &&...) // may throw
  //   destroy(T *)
  //   destroy(fr) // range operations can not throw
  //
  //   destroy_function
  //     ()(alloc_vt<AL> *)
  //   destroy_fn()->destroy_function // returned value need *this to be alive
  //
  //   default_construct_non_array_fn()->auto // alloc_vt<AL> is byte
  //     // f(T *)
  //   construct_non_array_fn()->auto // alloc_vt<AL> is byte
  //     // f(T *, S &&...)
  //   destroy_non_array_fn()->auto // alloc_vt<AL> is byte
  //     // f(T *)
  //
  //   // uninitialized_...(): no overlap
  //   uninitialized_fill(fr) // may throw
  //   uninitialized_fill_plus(fr)->fr_itr // may throw
  //   uninitialized_fill(fr, x) // may throw
  //   uninitialized_fill_plus(fr, x)->fr_itr // may throw
  //   uninitialized_copy(ir, fi)->fi_itr // may throw
  //   uninitialized_copy_plus(ir, fi)->pair<ir_itr, fi_itr> // may throw
  //   uninitialized_move(ir, fi)->fi_itr // may throw
  //   uninitialized_move_plus(ir, fi)->pair<ir_itr, fi_itr> // may throw
  //   uninitialized_fully_move(fr, fi, destroy_f = destroy_fn())->fi_itr
  //   uninitialized_fully_move_plus(fr, fi, destroy_f = destroy_fn())
  //     ->pair<fr_itr, fi_itr>
  //     // may throw
  //     // destroy_f is for fr
  //     // range operations can not throw
  //   fully_move(fr, fi, destroy_f = destroy_fn())->fi_itr
  //     // may throw
  //     // destroy_f is for fr
  //     // range operations can not throw
  //   fully_move_plus(fr, fi, destroy_f = destroy_fn())->pair<fr_itr, fi_itr>
  //
  //   new_1(s...)->alloc_ptr<AL> // may throw
  //   new_n(n)->alloc_ptr<AL> // may throw
  //   new_n(n, x)->alloc_ptr<AL> // may throw
  //   delete_1(alloc_ptr<AL>)
  //   delete_n(alloc_ptr<AL>, alloc_szt<AL>)
  //
  //   new_node<NODE_T>(s...)->decltype((*this).rebind<NODE_T>().new_1(...))
  //     // may throw
  //   delete_node(node_p)
  //
  //   temporary
  //     special member functions: only move-constructible
  //       // may throw
  //     explicit(sizeof...(s) == 0) temporary(p, s...)
  //     ->()->(const) value_type *
  //     *()->(const) value_type &
  //   make_temporary(s...)->temporary // may throw
  //
  //   uninitialized
  //     special member functions: only movable
  //     begin()
  //     end()
  //     empty()
  //     size()
  //     release()->alloc_ptr<AL>
  //   make_uninitialized(alloc_szt<AL>)->uninitialized // may throw
  //
  //   headed_buffer_ptr<H, U>
  //     head_type = H
  //     value_type = U
  //     special member functions: full
  //     headed_buffer_ptr(nullptr)
  //     =(nullptr)
  //     ==(nullptr)
  //     head()->H &
  //     data()->alloc_rebind_ptr<AL, U>
  //     size()->alloc_szt<AL>
  //     refer_to_only_head(H &)
  //   allocate_headed_buffer<H, U>(n, s...)->headed_buffer_ptr<H, U>
  //     // alloc_vt<AL> is byte
  //     // construct head by s...
  //     // throw if n is not valid
  //   deallocate_headed_buffer(headed_buffer_ptr<H, U>)
  //     // alloc_vt<AL> is byte
  //     // auto destroy head type
  //
  //   headed_bytebuf_ptr<H>
  //     head_type = H
  //     value_type = U
  //     special member functions: full
  //     headed_bytebuf_ptr(nullptr)
  //     =(nullptr)
  //     ==(nullptr)
  //     head()->H &
  //     data()->alloc_ptr<AL>
  //     size()->alloc_szt<AL>
  //     align()->alloc_szt<AL>
  //     refer_to_only_head(H &)
  //   allocate_headed_bytebuf<H>(n, s...)->headed_bytebuf_ptr<H>
  //     // alloc_vt<AL> is byte
  //     // construct head by s...
  //     // algn = min_alignment()
  //   allocate_headed_bytebuf_alignas<H>(algn, n, s...)->headed_bytebuf_ptr<H>
  //     // alloc_vt<AL> is byte
  //     // construct head by s...
  //   deallocate_headed_bytebuf(headed_bytebuf_ptr<H>)
  //     // alloc_vt<AL> is byte
  //     // auto destroy head type
  //
  //   pointer_pair = iter_pair<alloc_ptr<AL>>
  //   new_array(n)->pointer_pair // may throw
  //   new_array(n, x)->pointer_pair // may throw
  //   new_array(fr)->pointer_pair // may throw
  //   new_array_move_individually(pointer_pair &x, x_al = get())
  //     ->pointer_pair // may throw
  //     // x == {} after return
  //   delete_array(pointer_pair)
  //
  //   unique_ptr = re::unique_ptr<...> // deallocate by *this
  //   unique_array = re::unique_array<...> // deallocate by *this
  //   make_unique(s...) // may throw
  //   make_array(n) // may throw
  //   make_array(n, vt) // may throw
  //   make_array(r) // may throw
  // allocator_wrapper(A)->allocator_wrapper(A)
  //
  // default_alloc_wrapper<T> = allocator_wrapper<default_allocator<T>>
  // alloc_wrapper(A)->allocator_wrapper<A>
}
void doc_unique_ptr() {
  // alloc_delete<AL>
  //   pointer
  //   pointer_pair
  //   size_type
  //   special member functions: full
  //   alloc_delete(const AL &)
  //   alloc_delete(const alloc_delete<U> &)
  //   ()(pointer)
  //   ()(pointer_pair)
  //
  // default_delete<T>
  //   pointer
  //   pointer_pair
  //   size_type
  //   special member functions: full
  //   alloc_delete(const AL &)
  //   alloc_delete(const alloc_delete<U> &)
  //   ()(pointer) // non-array
  //   ()(pointer_pair) // non-array
  //   ()(U *) // T is U[]
  //   ()(T *) // T is U[N]
  //
  // unique_ptr<T, D = default_delete<T>>
  //   pointer = D::pointer if D::pointer exists, otherwise T *
  //   element_type = T;
  //   deleter_type = D;
  //
  //   special member functions: default constructible, movable
  //   == <=> for (const unique_ptr<T1, D1> &, const unique_ptr<T2, D2> &)
  //   == <=> between const unique_ptr & and nullptr
  //
  //   unique_ptr(nullptr)
  //   =(nullptr)
  //   unique_ptr(unique_ptr<U, E> &&u)
  //   =(unique_ptr<U, E> &&u)
  //   explicit unique_ptr(pointer)
  //   unique_ptr(pointer, const deleter_type &)
  //
  //   ->()->pointer
  //   *()->T & // !empty()
  //   get()->pointer
  //
  //   empty()->bool
  //   clear()
  //   explicit operator bool()
  //
  //   get_deleter()->(const) deleter_type &
  //
  //   release()->pointer
  //   reset(pointer p = pointer{}) // p != get()
  //
  // unique_ptr<T [], D>
  //   pointer = D::pointer if D::pointer exists, otherwise T *
  //   element_type = T;
  //   deleter_type = D;
  //   [](size_t)->T &
  //   pointer get()
  //   ... // same as unique_ptr<T, D>
  //
  // hash<unique_ptr<T, D>>
  //
  // make_unique<T>(s...) // value initialize if sizeof...(s) == 0u // may throw
  // make_unique<T []>(n) // value initialize // may throw
  // make_unique_for_overwrite<T>() // default initialize // may throw
  // make_unique_for_overwrite<T []>(n) // default initialize // may throw
}
void doc_unique_array() {
  // unique_array<T, D = default_delete<T>>
  //   pointer = pointer type denoted by D::pointer
  //   const_pointer = ptr_rebind<pointer, add_const<T>>
  //   element_type
  //   value_type
  //   reference
  //   const_reference
  //   iterator = pointer
  //   const_iterator = const_pointer
  //   difference_type
  //   size_type
  //   deleter_type
  //   pointer_pair
  //
  //   special member functions:
  //     default constructible
  //     movable
  //   == <=> between unique_array objects with the same pointer element type
  //     // may throw
  //
  //   unique_array(unique_array<T2, D2> &&u)
  //   =(unique_array<T2, D2> &&u)
  //   explicit unique_array(pointer_pair)
  //   unique_array(pointer_pair, const D &)
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //   empty()->bool
  //   size()->size_type
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //   [](size_type n)->(const_)reference // n is valid
  //
  //   get_deleter()->(const) deleter_type &
  //
  //   release()->pointer_pair
  //   reset(pointer_pair = {}) // no self-reset
  //   clear()
  //
  // make_array<T>(n)->unique_array<T> // may throw
  // make_array<T>(n, x)->unique_array<T> // may throw
  // make_array<T>(r)->unique_array<T> // may throw
}
void doc_copyable_ptr() {
  // copyable_ptr<T> // T is copy-constructible
  //   element_type = T;
  //   value_type = remove_cvref<T>
  //
  //   special member functions: full // copy may throw
  //
  //   copyable_ptr(nullptr)
  //   =(nullptr)
  //   ==(nullptr)
  //   explicit operator bool()
  //   empty()->bool
  //
  //   explicit copyable_ptr(T *)
  //   release()->T *
  //   reset(T * = nullptr) // no self-reset
  //   clear()
  //
  //   explicit copyable_ptr(in_place, s...) // may throw
  //   emplace(s...) // may throw
  //
  //   ->()->T *
  //   *()->T &  // !empty()
  //   get()->T *
}
void doc_copyable_array() {
  // copyable_array<T>
  //   pointer = T *
  //   const_pointer = const T *
  //   element_type = T;
  //   value_type
  //   reference
  //   const_reference
  //   iterator = pointer
  //   const_iterator = const_pointer
  //   difference_type
  //   size_type
  //   pointer_pair = iter_pair<T *>
  //
  //   special member functions: full // copy may throw
  //
  //   explicit copyable_array(pointer_pair)
  //   release()->pointer_pair
  //   reset(pointer_pair = {})
  //   clear()
  //
  //   explicit copyable_array(n) // may throw
  //   copyable_array(n, x) // may throw
  //   copyable_array(from_range, r) // may throw
  //
  //   == <=> // may throw
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //   empty()->bool
  //   size()->size_type
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //   [](size_type n)->(const_)reference // n is valid
}
void doc_maybe_owner_ptr() {
  // maybe_owner_ptr<T>
  //   pointer = T *
  //   element_type = T
  //
  //   special member functions: full // copy just make non-owner
  //
  //   maybe_owner_ptr(nullptr)
  //   =(nullptr)
  //   ==(nullptr)
  //
  //   explicit(sizeof...(s) == 0) maybe_owner_ptr(in_place, s...) // owner
  //   explicit(T *) // not owner
  //
  //   ->()->T *
  //   *()->T & // !empty()
  //   get()->T *
  //
  //   empty()->bool
  //   owns()->bool
  //   clear()
  //   reset()
}
void doc_buffer() {
  // buffer<T, AL = default_allocator<T>>
  //   pointer = alloc_ptr<AL>
  //   const_pointer = alloc_cptr<AL>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = pointer
  //   const_iterator = const_pointer
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //
  //   special member functions: only move-constructible
  //
  //   explicit buffer(const AL &)
  //   explicit buffer(INT n) // may throw // n can be any integral
  //
  //   allocator_type = AL
  //   get_allocator()->allocator_type
  //
  //   begin()->(const_)pointer
  //   end()->(const_)pointer
  //
  //   max_size()->size_type
  //   size()->size_type
  //   empty()->bool
  //   capacity()->size_type
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //   [](n)->(const_)reference // n is valid
  //   emplace_front(s...) // may throw
  //     // front_raw_space() != 0
  //   push_front(x) // may throw
  //     // front_raw_space() != 0
  //   emplace_back(s...) // may throw
  //     // back_raw_space() != 0
  //   push_back(x) // may throw
  //     // back_raw_space() != 0
  //   pop_front() // !empty()
  //   pop_back() // !empty()
  //   front_raw_space()->size_type
  //   back_raw_space()->size_type
  //
  //   reallocate(size_type) // may throw
  //   increase_capacity(INT n) // may throw // n can be any integral
  //
  //   reset(INT n) // may throw // n can be any integral
  //   reserve(INT n) // may throw // n can be any integral
  //   reserve_more(size_type) // may throw
  //   clear()
  //
  //   release()
  {
    buffer<int> v(3);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.pop_front();
    assert(equal(v, seq(2, 3)));
    assert(v.front_raw_space() == 1);
    assert(v.back_raw_space() == 0);
    v.reallocate(3);
    assert(equal(v, seq(2, 3)));
    assert(v.front_raw_space() == 0);
    assert(v.back_raw_space() == 1);
  }
}
void doc_scoped_allocator_adaptor() {
  // scoped_allocator_adaptor<OUTER_ALLOC, INNER_ALLOCS...>
  //   outer_allocator_type // = OUTER_ALLOC
  //   inner_allocator_type
  //     // if (sizeof...(INNER_ALLOCS) == 0)
  //     //   = this_t
  //     // else
  //     //   = scoped_allocator_adaptor<INNER_ALLOCS...>
  //   value_type
  //   size_type
  //   difference_type
  //   pointer
  //   const_pointer
  //   void_pointer
  //   const_void_pointer
  //   propagate_on_container_copy_assignment
  //     value
  //   propagate_on_container_move_assignment
  //     value
  //   propagate_on_container_swap
  //     value
  //   is_always_equal
  //     value
  //   rebind<T>
  //     other
  //
  //   special member functions
  //     full
  //   ==
  //
  //   scoped_allocator_adaptor(a, s...)
  //   scoped_allocator_adaptor(const scoped_allocator_adaptor<A, S...> &)
  //   scoped_allocator_adaptor(scoped_allocator_adaptor<A, S...> &&)
  //
  //   outer_allocator()->ref
  //   inner_allocator()->ref
  //
  //   allocate(n) // may throw
  //   allocate(n, hint) // may throw
  //   deallocate(p, n)
  //
  //   max_size()
  //
  //   construct(p, s...) // may throw
  //   destroy(p)
}
void doc_allocator_aware_container_ownership() {
  // allocator_aware_container_ownership<T>
  //   //  requires:
  //   //    1) this type is a friend of T
  //   //    2) the following member functions of T are well defined:
  //   //      typename T::allocator_type &alloc_ref() noexcept
  //   //      const typename T::allocator_type &alloc_ref() const noexcept
  //   //      void initialize() noexcept
  //   //      void clean_data() noexcept
  //   //      void make_data(const T &)
  //   //      void assign_data(const T &)         (optional)
  //   //      void make_data(T &&)
  //   //      void make_data_individually(T &&)
  //   //      void assign_data_individually(T &&) (optional)
  //   //      void swap_data(T &)
  //   //      void swap_data_individually(T &)    (optional)
  //   //      [Note1]: self-assignment guard is not needed (done here) but
  //   //               swap_data(T &) must be compatibible to self-swap
  //   //      [Note2]: void make_data(T &&) must be noexcept or capable of
  //   //               keeping *this a legal condition after throwing
  //   static move_construct(T &target, T &from)
  //   static void copy_assign(const T &target, T &from) // may throw
  //   static void move_assign(T &target, T &from) // may throw
  //   static void swap(T &x, T &y) // may throw
}
void doc_object_pool() {
  // pool_object<T, AL = default_allocator<byte>>
  // sync_pool_object<T, AL = default_allocator<byte>> // in concurrency.h
  //   special member functions: default-constructible, movable
  //   empty()->bool
  //   value()->T_ref // throw logic_error if empty
  //   *()->T_ref
  //   ->()->T_ptr
  //
  // object_pool<T, AL = default_allocator<byte>>
  // sync_object_pool<T, AL = default_allocator<byte>>
  //   size_type = alloc_szt<AL>
  //   special member functions: default-constructible
  //   explicit object_pool(const AL &)
  //   capacity()->size_type
  //   used_size()->size_type
  //   idle_size()->size_type
  //   reserve_more(size_type)
  //   fetch_pointer(s...)->T *
  //   free_pointer(T *)
  //   fetch()->(sync_)pool_object<T, AL>
}
void doc_raw_object_pool() {
  // raw_object_pool<AL = default_allocator<byte>>
  //   size_type
  //
  //   special member functions: destructible
  //
  //   raw_object_pool(sz, algn, a = AL{})
  //     // sz != 0u
  //     // algn is valid alignment value
  //
  //   object_size()->size_type
  //   object_align()->size_type
  //   capacity()->size_type
  //   used_size()->size_type
  //   idle_size()->size_type
  //   reserve_more(size_type)
  //   allocate()->void *
  //   deallocate(void *)
}
void doc_memory_pool() {
  // memory_pool<AL = default_allocator<byte>>
  //   size_type = alloc_szt<AL>
  //   difference_type = alloc_dft<AL>
  //   special member functions: default-constructible
  //   explicit memory_pool(size_type algn, const AL & = AL{})
  //   memory_pool(fr, size_type algn, const AL & = AL{})
  //     // fr is range of object sizes like seq(8, 16, 32, 45, 99)
  //     // for sz < sizeof(void *), the real object size is sizeof(void *)
  //   allocate<T>(size_type)->T *
  //   deallocate(T *, size_type)
  //   min_alignment()->size_type
  //
  // pool_allocator<T, POOL = memory_pool<>>
  //   value_type = T
  //   size_type = POOL::size_type
  //   difference_type = POOL::difference_type
  //   max_size()->size_type
  //   max_alignment()->size_type
  //   rebind<U>
  //     other = pool_allocator<U, POOL>
  //
  //   special member functions: full
  //   explicit pool_allocator(POOL &)
  //   operator pool_allocator<U, POOL>()
  //   ==(const pool_allocator<U, POOL> &)
  //
  //   allocate(size_type)->T *
  //   deallocate(T *, size_type)
}

// dynamic
void doc_dynamic_base() {
  // is<T>(x)->decltype(x.is<T>())
  //
  // in_place_index_t<I>
  // in_place_index<I>
  // template_is_in_place_index<T>
  // is_in_place_index<T>
}
void doc_dynamic() {
  // as_dynamic
  //   ... // virtual functions are public but inner-used
  //
  // template_dynamic_buffer_size<BASE_TYPE>
  // dynamic_buffer_size<BASE_TYPE>
  // template_dynamic_buffer_alignment<BASE_TYPE>
  // dynamic_buffer_alignment<BASE_TYPE>
  //
  // bad_dynamic_access : public exception
  //
  // dynamic<T = void,
  //         BUFSZ = dynamic_buffer_size<T>::value,
  //         ALIGN = dynamic_buffer_alignment<T>::value>
  //   special member functions
  //     full // copy may throw // throw when copy uncopyable
  //
  //   dynamic(nullptr)
  //   =(nullptr)
  //   == with nullptr
  //
  //   dynamic(U &&) // may throw
  //   =(U &&) // may throw
  //
  //   explicit dynamic(in_place_type<U>, s...) // may throw
  //   explicit dynamic(in_place_type<U>, {...}, s...) // may throw
  //   emplace<U>(s...) // may throw
  //   emplace<U>({...}, s...) // may throw
  //
  //   dynamic(const dynamic<T, SZ2, ALGN2> &) // may throw
  //   operator =(const dynamic<T, SZ2, ALGN2> &) // may throw
  //   dynamic(dynamic<T, SZ2, ALGN2> &&) // may throw
  //   operator =(dynamic<T, SZ2, ALGN2> &&) // may throw
  //
  //   *()->T_ref // !empty()
  //   ->()->T_ptr
  //   value()->T_ref // throw bad_dynamic_access if empty()
  //
  //   type()->const type_info & // typeid(void) if empty
  //   is<X>()->bool
  //   as<X>()->X_ref
  //     // throw bad_dynamic_access if type() != typeid(X)
  //     // X should have no cvref
  //
  //   empty()->bool
  //   clear()
  //   explicit operator bool()
  //   has_value()->bool
  //   reset()
  //
  //   local()->bool
  //   size()->size_t
  //   align()->size_t
  //   wrapper_size()->size_t
  //   wrapper_align()->size_t
  {
    struct A : as_dynamic {virtual int f() {return 0;}};
    struct B : A {virtual int f() override {return 1;}};
    struct C : A {virtual int f() override {return 2;}};
    dynamic<A> a = A(), b = B(), c = C();
    assert(a->f() == 0 && b->f() == 1 && c->f() == 2);
  }
}
void doc_dynamic_void() {
  // template_dynamic_buffer_size<void>
  //   // equal to dynamic_buffer_size<void *>
  // template_dynamic_buffer_alignment<void>
  //   // equal to dynamic_buffer_alignment<void *>
  // dynamic<void, BUFSZ, ALIGN>
  //   special member functions
  //     full // copy may throw
  //
  //   dynamic(nullptr)
  //   =(nullptr)
  //   == with nullptr
  //
  //   dynamic(x) // may throw
  //   =(x) // may throw
  //
  //   explicit dynamic(in_place_type<U>, s...) // may throw
  //   explicit dynamic(in_place_type<U>, {...}, s...) // may throw
  //   emplace<U>(s...) // may throw
  //   emplace<U>({...}, s...) // may throw
  //
  //   dynamic(const dynamic<void, SZ2, ALGN2> &) // may throw
  //   operator =(const dynamic<void, SZ2, ALGN2> &) // may throw
  //   dynamic(dynamic<voidT, SZ2, ALGN2> &&) // may throw
  //   operator =(dynamic<void, SZ2, ALGN2> &&) // may throw
  //
  //   type()->const type_info & // typeid(void) if empty
  //   is<X>()->bool
  //   as<X>()->X_ref
  //     // throw bad_dynamic_access if type() != typeid(X)
  //     // X should have no cvref
  //
  //   empty()->bool
  //   clear()
  //   reset()
  //
  //   local()->bool
  //   static constexpr local<X>()->bool
  //
  //   size()->size_t
  //   align()->size_t
  //   wrapper_size()->size_t
  //   wrapper_align()->size_t
  //
  // any = dynamic<>;
  // make_any<T>(s...) // may throw
  // make_any<T>({...}, s...) // may throw
  //
  // bad_any_cast : bad_cast
  //   what()
  // any_cast<T>(any_ref) // may throw bad_any_cast // T should have no cvref
  // any_cast<T>(any_ptr) // may throw bad_any_cast // T should have no cvref
  {
    any x = make_any<int>(3);
    any y = make_any<float>(1.5);
    assert(as<int>(x) == 3);
    assert(as<float>(y) == 1.5);
    adl_swap(x, y);
    assert(as<int>(y) == 3);
    assert(as<float>(x) == 1.5);
  }
}
void doc_function() {
  // bad_function_call : public exception
  //
  // function<F, BUFSZ = ..., ALIGN = ...>
  //   result_type
  //
  //   special member functions
  //     full // copy may throw
  //
  //   function(nullptr)
  //   =(nullptr)
  //   == with nullptr
  //
  //   function(T &&) // may throw // decay_t<x> is copy-constructible
  //   =(T &&) // may throw // decay_t<x> is copy-constructible
  //   =(reference_wrapper<T>) // may throw
  //
  //   function(const function<R (S...), SZ2, ALGN2> &) // may throw
  //   =(const function<R (S...), SZ2, ALGN2> &) // may throw
  //   function(function<R (S...), SZ2, ALGN2> &&) // may throw
  //   =(function<R (S...), SZ2, ALGN2> &&) // may throw
  //
  //   ()(S...)->R // may throw
  //
  //   type()->const type_info & // typeid(void) if empty()
  //   is<X>()->bool
  //   as<X>()->X_ref
  //     // throw bad_dynamic_access if type() != typeid(X)
  //     // X should have no cvref
  //
  //   local()->bool
  //   static constexpr local<X>()->bool
  //
  //   size()->size_t
  //   align()->size_t
  //   wrapper_size()->size_t
  //   wrapper_align()->size_t
  //
  //   empty()->bool
  //   clear()
  //   reset()
  //   explicit operator bool()
  //
  //   target_type()->const type_info & // same as type()
  //   target<T>()->T * // addressof(as<T>()) or nullptr
  {
    function<int ()> f = []() {return 1;};
    assert(f() == 1);
  }
}
void doc_move_only_function() {
  // move_only_function<F>
  //   return_type
  //
  //   special member functions
  //     move only
  //
  //   move_only_function(nullptr_t)
  //   =(nullptr_t)
  //   == with nullptr
  //
  //   move_only_function(in_place_type<T>, ...) // may throw
  //   move_only_function(in_place_type<T>, {...}, ...) // may throw
  //
  //   move_only_function(T &&) // may throw
  //   =(T &&) // may throw
  //
  //   ()(...) // may throw // operator bool() == true // with F's qualifiers
  //   explicit operator bool()

  move_only_function<int ()> f = []() {return 1;};
  static_assert(!invocable<decltype(as_const(f))>);
  static_assert(invocable<decltype(f) &>);
}
void doc_unique_function() {
  // unique_function<F, BUFSZ, ALIGN>
  //   // same as function<F, BUFSZ, ALIGN> but not copyable
  //   // do not apply F's qualifier and only keep R ()(S...) const
  //
  //   result_type
  //
  //   special member functions: movable and default-constructible
  //
  //   unique_function(nullptr)
  //   =(nullptr)
  //   == with nullptr
  //   reset()
  //   clear()
  //   empty()->bool
  //   explicit operator bool()
  //
  //   unique_function(T &&) // may throw // no guard to nullptr
  //   =(T &&) // may throw
  //
  //   unique_function(const unique_function<F, SZ2, ALGN2> &) // may throw
  //   =(const unique_function<F, SZ2, ALGN2> &) // may throw
  //   unique_function(unique_function<F, SZ2, ALGN2> &&) // may throw
  //   =(unique_function<F, SZ2, ALGN2> &&) // may throw
  //
  //   ()(...)->auto // may throw // !empty()
  //
  //   local()->bool
  //   static constexpr local<T>()->bool
}
void doc_type_erased_invocation() {
  // do_type_erased_invocation(f, s...) // may throw
  {
    string (*f)(const void *, const void *)
      = [](const void *x, const void *y)->string {
      return copy(*(const string *)x).append(*(const string *)y);
    };
    string s = do_type_erased_invocation<const void *>
      (f, string("ab"), string("cd"));
    assert(s == "abcd");
  }

  // template_is_invocable_for_all_combinations<F, S...>
  // is_invocable_for_all_combinations<F, S...>
  {
    using f = void (*)(int);
    static_assert(is_invocable_for_all_combinations
                  <f, type_pack<int>>);
    static_assert(is_invocable_for_all_combinations
                  <f, type_pack<short, int, long, long long>>);
    static_assert(!is_invocable_for_all_combinations
                  <f, type_pack<short, void *>>);

    using f2 = void (*)(int, int);
    static_assert(is_invocable<f2, int, int>);
    static_assert(is_invocable_for_all_combinations
                  <f2, type_pack<int, long>, type_pack<short, int>>);
    static_assert(!is_invocable_for_all_combinations
                  <f2, type_pack<int, long>, type_pack<short, int, void *>>);

    using f3 = void (*)(int, int, int);
    static_assert(is_invocable_for_all_combinations
                  <f3,
                   type_pack<int, long>,
                   type_pack<int, long>,
                   type_pack<int, long>>);
    static_assert(!is_invocable_for_all_combinations
                  <f3,
                   type_pack<int, long>,
                   type_pack<int, long>,
                   type_pack<int, void *>>);
  }

  // template_invoke_result_for_all_combinations<F, S...>
  // invoke_result_for_all_combinations<F, S...>
  // type_erased_invocation_array<VOID_PTR, RET, FO, TYPE_PACKS...>()
  {
    const auto f = [](auto x, auto y) {return x + y;};
    static_assert(is_invocable_for_all_combinations
                  <decltype(f) &,
                   type_pack<bool, short, int>, type_pack<long, long long>>);
    using r = invoke_result_for_all_combinations
      <decltype(f) &, type_pack<bool, short, int>, type_pack<long, long long>>;
    static_assert(is_same<r, long long>);

    const auto a = type_erased_invocation_array
      <const void *, r, decltype(f) &,
       type_pack<short &, int &>, type_pack<long &, long long &>>();
    short i = 3;
    long long j = 4;
    assert(do_type_erased_invocation<const void *>(a[0 * 2 + 1], f, i, j) == 7);
  }

  // template_base_of_type_packs<I, S...>
  // base_of_type_packs<I, S...>
  {
    static_assert(base_of_type_packs
                  <0, type_pack<int, int>, type_pack<int, int, int>,
                   type_pack<int, int, int, int>> == 12);
    static_assert(base_of_type_packs
                  <1, type_pack<int, int>, type_pack<int, int, int>,
                  type_pack<int, int, int, int>> == 4);
    static_assert(base_of_type_packs
                  <2, type_pack<int, int>, type_pack<int, int, int>,
                  type_pack<int, int, int, int>> == 1);
    // 3 * 4 == 12
    // 4 == 4
    // 1 == 1
  }
  // type_erased_invocation_index<TYPE_PACKS...>(i1, i2, ...in)
  {
    assert((type_erased_invocation_index
            <type_pack<char, int, float>,
             type_pack<char, int, float>,
             type_pack<char, int, float>>(1, 2, 1))
           == (1 * 3 * 3 + 2 * 3 + 1));
  }
}
void doc_variant() {
  // bad_variant_access : public exception
  //
  // template_variant_size<T>
  // variant_size<T>
  // template_variant_alternative<I, T>
  // variant_alternative<I, T>
  //
  // holds_alternative<T>(v)->bool
  //
  // get<I>(v)->variant_alternative_ref // may throw bad_variant_access
  // get<T>(v)->variant_alternative_ref // may throw bad_variant_access
  // at<I>(v)->variant_alternative_ref // may throw bad_variant_access
  // as<T>(v)->variant_alternative_ref // may throw bad_variant_access
  //
  // // get_if can not be function object, just for consistence with std
  // get_if<I>(vp)->variant_alternative_pointer // nullptr if I is not right
  // get_if<T>(vp)->variant_alternative_pointer // nullptr if T is not right
  //
  // monostate
  //   == <=> for (monostate, monostate) // always equal
  //
  // uses_allocator<variant<S...>, AL> : true_type
  //
  // visit(f, vs...)->auto // may throw
  //   // the return type is decided by index == 0 for all args
  // visit_r<R>(f, vs...)->R // may throw
  // 
  // variant<S...>
  //   special member functions: dependent // may throw from smf of S...
  //
  //   == <=> // may throw
  //
  //   explicit variant(in_place_type<void>)
  //   explicit variant(in_place_type<T>, s...) // may throw
  //   explicit variant(in_place_type<T>, {...}, s...) // may throw
  //
  //   explicit variant(in_place_index<I>, s...) // may throw
  //   explicit variant(in_place_index<I>, {...}, s...) // may throw
  // 
  //   variant(T &&) // may throw
  //   =(T &&) // may throw
  //
  //   emplace<void>() // make instance valueless_by_exception() return true
  //
  //   emplace<X>(s...)->X & // may throw
  //   emplace<X>({...}, s...)->X & // may throw
  //
  //   emplace<I>(s...)->variant_alternative<I, this_t> & // may throw
  //   emplace<I>({...}, s...)->variant_alternative<I, this_t> & // may throw
  // 
  //   valueless_by_exception()->bool
  //   index()->size_t // index() == npos() if valueless_by_exception()
  //   npos()->size_t // return sizeof...(S)
  // 
  //   is<T>()->bool
  //   is<void>()->bool // same as valueless_by_exception()
  //   as<T>()->T_ref
  //     // may throw bad_variant_access
  //     // T should have no cvref
  //   at<I>()->variant_alternative<I, this_t>_ref
  //     // may throw bad_variant_access
  //
  //   visit(v)->auto // same as re::visit(v, this_ref)
  //   visit<R>(v)->R // same as re::visit(v, this_ref)
  //   visit_with_index(v)->auto
  //     // call v(size_constant<ID>{}, at<ID>(this_ref)) for current index()
  //
  // hash<monostate>
  // hash<variant<S...>>
  {
    variant<int, float, void *> v(in_place_type<void>);
    assert(v.valueless_by_exception());
    v = 3;
    assert(!v.valueless_by_exception());
    assert(v.is<int>() && is<int>(v));
    assert(v.as<int>() == 3 && as<int>(v) == 3);
    v = 3.0f;
    assert(v.index() == 1 && v.at<1>() == 3.0f);
  }
}

// random
void doc_random() {
  // linear_congruential_engine<UINT, A, C, M>
  //   result_type = UINT
  //   static constexpr result_type multiplier = A
  //   static constexpr result_type increment = C
  //   static constexpr result_type modulus = M
  //   static constexpr min()->result_type
  //   static constexpr max()->result_type
  //   static constexpr result_type default_seed = 1u
  //   special member functions: full
  //   ==
  //   explicit this_t(result_type)
  //   seed(result_type = default_seed)
  //   ()()->result_type
  //   discard(unsigned long long n)
  //     // call (*this)() by n times to update the stored seed value
  // minstd_rand0 = linear_congruential_engine<...>
  // minstd_rand = linear_congruential_engine<...>
  //
  // mersenne_twister_engine<UINT, ...>
  //   ...
  //   special member functions: full
  //   ==
  //   explicit this_t(result_type)
  //   seed(result_type = default_seed)
  //   ()()->result_type
  //   discard(unsigned long long n)
  //     // call (*this)() by n times to update the stored seed value
  // mt19937 = mersenne_twister_engine<...>
  // mt19937_64 = mersenne_twister_engine<...>
  //
  // uniform_int_distribution<INT = int>
  //   result_type = INT
  //   param_type = pair<INT, INT>
  //   special member functions: full
  //   ==
  //   explicit this_t(INT a, INT b = numeric_limits<INT>::max())
  //   explicit this_t(const param_type &)
  //   reset() // reset internal states if there are some internal states
  //   ()(G &)->result_type
  //   ()(G &, const param_type &)->result_type
  //   a()->result_type
  //   b()->result_type
  //   param()->param_type
  //   param(const param_type &)
  //   min()->result_type
  //   max()->result_type
  //
  // rational_bernoulli_distribution<INT = int>
  //   result_type = bool
  //   param_type = pair<INT, INT>
  //   special member functions: full
  //   ==
  //   explicit this_t(INT numerator,
  //                   INT denominator = numeric_limits<INT>::max())
  //   explicit this_t(const param_type &)
  //   reset() // reset internal states if there are some internal states
  //   ()(G &)->result_type
  //   ()(G &, const param_type &)->result_type
  //   a()->result_type // return numerator value
  //   b()->result_type // return denominator value
  //   param()->param_type
  //   param(const param_type &)
  //   min()->result_type // return false
  //   max()->result_type // return true
  //
  // rander<E = minstd_rand>
  //   result_type
  //   engine_type
  //
  //   special member functions: full
  //   ==
  //
  //   explicit rander(seed_i)
  //   seed(result_type)
  //   engine()->E // rander can just be seen as engine
  //   min()
  //   max()
  //
  //   ()()->result_type
  //   discard(z = 1)
  //   ()(i, j)->result_type // make a randomized value in [i, j]
  //   gen_bool(num, den)->bool
  //     // den != 0
  //     // if num >= den, num means den
  //     // if num == 0, always return false
  //   ()(prob)->bool
  // make_rander<E = minstd_rand>()
}

// algorithm
void doc_equal() {
  // equal(ir, ir2, eq = equal_to{})->bool // may throw
  // equal(ir, ii, eq = equal_to{})->bool // may throw

  int a[] = {1, 2, 3};
  int a2[] = {1, 2, 3};
  assert(equal(a, a2));
  assert(equal(a, begin(a2)));
}
void doc_allanynone_of() {
  // all_of(ir, eq)->bool // may throw // call eq(*ir_iter) to check
  // any_of(ir, eq)->bool // may throw // call eq(*ir_iter) to check
  // none_of(ir, eq)->bool // may throw // call eq(*ir_iter) to check
  //
  // all_of_equal(ir, const T &, eq = equal_to{})->bool // may throw
  // any_of_equal(ir, const T &, eq = equal_to{})->bool // may throw
  // none_of_equal(ir, const T &, eq = equal_to{})->bool // may throw

  assert(all_of_equal(empty_rng<int>(), 0));
  assert(!any_of_equal(empty_rng<int>(), 0));
  assert(none_of_equal(empty_rng<int>(), 0));
}
void doc_for_each() {
  // for_each(ir, f)->ir_itr // may throw
  //
  // for_each(ir, ir2, f)->pair<itr_ir, itr_ir2> // may throw
  // for_each(ir, ii, f)->ii_itr // may throw
  // for_each_plus(ir, ii, f)->pair<ir_itr, ii_itr> // may throw
  {
    int a[] = {1, 2, 3};
    assert(for_each(a, [](int &x) {++x;}) == end(a));
    assert(equal(a, seq(2, 3, 4)));
  }

  // for_each_excluding_first(ir, f, f_for_first = empty_function{})->ir_itr
  // for_each_excluding_last(fr, f, f_for_last = empty_function{})->fr_itr
  //
  // for_each_excluding_first_n(ir, ir_dft n, f,
  //                            f_for_first = empty_function{})->ir_itr
  // for_each_excluding_last_n(fr, fr_dft n, f,
  //                           f_for_last = empty_function{})->fr_itr
  {
    int a[] = {1, 2, 3};
    for_each_excluding_first(a, [](int &x) {++x;}, [](int &x) {--x;});
    assert(equal(a, seq(0, 3, 4)));
    for_each_excluding_last(a, [](int &x) {++x;}, [](int &x) {--x;});
    assert(equal(a, seq(1, 4, 3)));
  }
}
void doc_find() {
  // find(ir, const T &x, eq = equal_to{})->ir_itr // may throw
  // find_not(ir, const T &x, eq = equal_to{})->ir_itr // may throw
  // find_if(ir, eq)->ir_itr // may throw // call eq(*ir_i) to check
  // find_if_not(ir, eq)->ir_itr // may throw // call !eq(*ir_i) to check

  int a[] = {0, 0, 1, 1};
  assert(find(a, 1) == nth(a, 2));
  assert(find_not(a, 0) == nth(a, 2));
}
void doc_find_last() {
  // find_last(fr, x, eq = equal_to{})->fr_itr // may throw
  // find_last_not(fr, x, eq = equal_to{})->fr_itr // may throw
  // find_last_if(fr, eq)->fr_itr // may throw // call eq(*fr_i) to check
  // find_last_if_not(fr, eq)->fr_itr // may throw // call !eq(*fr_i) to check

  const int a[] = {1, 1, 2, 3};
  assert(find_last(a, 1) == nth(a, 1));
}
void doc_find_first_last_of() {
  // find_first_of(ir, fr, eq = equal_to{})->ir_itr // may throw
  //   // find the first element equal to one of fr
  // find_last_of(fr, fr, eq = equal_to{})->fr_itr // may throw
  //   // find the last element equal to one of fr

  const int a[] = {1, 2, 3, 4};
  assert(find_first_of(a, seq(0, 2, 3)) == nth(a, 1));
  assert(find_last_of(a, seq(0, 2, 3)) == nth(a, 2));
}
void doc_adjacent_find() {
  // adjacent_find(fr, eq = equal_to{})->fr_itr // may throw
  //   // find the first pos that eq(*pos, *next(pos)),
  //   //   return end(fr) if find nothing
  // adjacent_find_both(fr, eq)->fr_itr // may throw
  //   // find the first pos that eq(*pos) && eq(*next(pos)),
  //   //   return end(fr) if find nothing
  // adjacent_find_both_while(fr, eq, eq_while)->fr_itr // may throw
  //   // find the first pos that eq(*pos) && eq(*next(pos))
  //   //   from [begin(fr), j) that j is end(fr) or !eq_while(*j),
  //   //   return end(fr) if find nothing

  const int a[] = {1, 2, 2, 3};
  assert(adjacent_find(a) == begin(a) + 1);

  char v[] = {1, 2, 3, 4, 'a', 'b', 'c', 5, 6};
  assert(adjacent_find_both(v, isalpha) == nth(v, 4));
  assert(adjacent_find_both_while
         (v, isalpha, [&v](char &c) {return addressof(c) - begin(v) <= 4;})
         == nth(v, 4));
  assert(adjacent_find_both_while
         (v, isalpha, [&v](char &c) {return addressof(c) - begin(v) <= 3;})
         == end(v));
}
void doc_count() {
  // count_if(ir, eq)->ir_dft // may throw
  // count(ir, x)->ir_dft // may throw

  const int a[] = {1, 2, 2, 3};
  assert(count_if(a, bind(equal_to{}, _1, 2)) == 2);
  assert(count(a, 2) == 2);
  assert(count(a, 4) == 0);
}
void doc_mismatch() {
  // mismatch(ir, ii, eq = ...)->pair<ir_itr, ii_itr> // may throw
  // mismatch(ir, ir2, eq = ...)->pair<ir_itr, ir2_itr> // may throw

  const int a[] = {1, 2, 3};
  const int b[] = {1, 3, 3};
  auto [it1, it2] = mismatch(a, b);
  assert(it1 == nth(a, 1) && it2 == nth(b, 1));
}
void doc_is_permutation() {
  // is_permutation(fr, fr2, eq = ...)->bool // may throw
  // is_permutation(fr, fi, eq = ...)->bool // may throw

  const int a[] = {1, 2, 3};
  const int b[] = {2, 3, 1};
  assert(is_permutation(a, a));
  assert(is_permutation(a, b));
  assert(is_permutation(empty_rng<int>(), empty_rng<int>()));
}
void doc_search() {
  // search(fr, fr_2, eq = equal_to{})->fr_itr // may throw

  const int a[] = {1, 2, 2, 2, 3};
  assert(search(a, rng(3, 2)) == nth(a, 1));
  assert(search(a, seq(2, 2)) == nth(a, 1));
  assert(search(a, empty_rng<int>()) == begin(a));
}
void doc_find_end() {
  // find_end(fr, fr_2, eq = ...)->fr_itr // may throw

  const int a[] = {1, 2, 3, 4, 2, 3, 4};
  assert(find_end(a, seq(2, 3, 4)) == nth(a, 4));
}
void doc_find_subrange() {
  // find_subrange(fr, fr_2, eq = equal_to{})->iter_pair<fr_itr>
  //   // same as search but return a pair of iterator
  // find_last_subrange(fr, fr_2, eq = equal_to{})->iter_pair<fr_itr>
  //   // same as find_end but return a pair of iterator

  const auto a = seq(1, 2, 3, 4);
  const auto p = find_subrange(a, seq(2, 3));
  assert(p == rng(nth(a, 1), 2));
}
void doc_contains() {
  // contains(ir, x, eq = equal_to{})->bool // may throw
  // contains_subrange(fr, fr2, eq = equal_to{})->bool // may throw

  assert(contains(seq(1, 2, 3), 2));
  assert(contains_subrange(seq(1, 2, 3), seq(2, 3)));
}
void doc_starts_ends_with() {
  // starts_with(ir, ir2, eq = equal_to{})->bool // may throw
  // ends_with(fr, fr2, eq = equal_to{})->bool // may throw

  assert(starts_with(seq(1, 2, 3), seq(1, 2)));
  assert(ends_with(seq(1, 2, 3), seq(2, 3)));
}
void doc_fold_left_right() {
  // fold_left(ir, init, add_f)->add_f_return_t_decay // may throw
  //   // accumulate from left to right
  //   // call acc_value = add_f(acc_value, rng_ref) to accumulate
  // fold_left_plus(ir, init, add_f)->pair<ir_itr, add_f_return_t_decay>
  //   // may throw
  //
  // fold_left_first(ir, add_f)->optional<add_f_return_t_decay> // may throw
  //   // if ir is empty, return nullopt
  //   // otherwise
  //   //   return fold_left(rng(next(begin(ir)), end(ir)), front(ir), add_f)
  // fold_left_first_plus(ir, add_f)
  //   ->pair<ir_itr, optional<add_f_return_t_decay>> // may throw
  //
  // fold_right(br, init, add_f)->add_f_return_t_decay // may throw
  //   // accumulate from right to left
  //   // call acc_value = add_f(rng_ref, acc_value) to accumulate
  //
  // fold_right_last(br, add_f)->optional<add_f_return_t_decay> // may throw
  //   // if ir is empty, return nullopt
  //   // otherwise
  //   //   return fold_right(rng(begin(br), prev(end(br))), back(ir), add_f)

  const auto f = [](int x, int new_x) {return x * 10 + new_x;};
  assert(fold_left(seq(2, 3), 1, f) == 123);
  assert(fold_left_first(rng(0, 0), f).empty());
  assert(fold_left_first(seq(2, 3), f).value() == 23);

  const auto f2 = bind(f, _2, _1);
  assert(fold_right(seq(3, 2), 1, f2) == 123);
  assert(fold_right_last(rng(0, 0), f2).empty());
  assert(fold_right_last(seq(3, 2, 1), f2).value() == 123);
}
void doc_copy_move_if_backward_from() {
  // // if range-to-be-copied overlaps with target-range:
  // //   copy to the left position: copy(), move()
  // //   copy to the right position: copy_backward(), move_backward()
  //
  // copy(ir, oi)->oi_itr // may throw
  // copy_plus(ir, oi)->pair<ir_itr, oi_itr> // may throw
  // move(ir, oi)->oi_itr // may throw
  // move_plus(ir, oi)->pair<ir_itr, oi_itr> // may throw
  //
  // copy_if(ir, oi, eq)->oi_itr // may throw
  // copy_if_plus(ir, oi, eq)->pair<ir_itr, oi_itr> // may throw
  //
  // copy_backward(br, bi)->bi_itr // may throw
  // move_backward(br, bi)->bi_itr // may throw
  //
  // copy_from(fr, ii)->ii_itr // may throw
  // copy_from_plus(fr, ii)->pair<fr_itr, ii_itr> // may throw
  // move_from(fr, ii)->ii_itr // may throw
  // move_from_plus(fr, ii)->pair<fr_itr, ii_itr> // may throw

  int a[] = {1, 2, 3, 0};
  assert(copy_backward(rng(begin(a), 3), end(a)) == nth(a, 1));
  assert(equal(a, seq(1, 1, 2, 3)));
}
void doc_swap_ranges() {
  // swap_ranges(fr, fi)->fi_itr // may throw
  // swap_ranges(fr, fr2)->pair<fr_itr, fr2_itr> // may throw
  // swap_ranges_plus(fr, fi)->pair<fr_itr, fi_itr> // may throw

  int a[] = {1, 2, 3};
  int b[] = {4, 5, 6, 7};
  assert(swap_ranges(a, b) == pair(nth(a, 3), nth(b, 3)));
  assert(equal(a, seq(4, 5, 6)));
  assert(equal(b, seq(1, 2, 3, 7)));
}
void doc_transform() {
  // transform(ir, oi, ufn)->oi_itr // may throw
  // transform_plus(ir, oi, ufn)->pair<ir_itr, oi_itr> // may throw
  //
  // transform(ir, ii, oi, bfn)->oi_itr // may throw
  // transform(ir, ir2, oi, bfn)->oi_itr // may throw
  // transform_plus(ir, ii, oi, bfn)->tuple<ir_itr, ii_itr, oi_itr> // may throw
  // transform_plus(ir, ir2, oi, bfn)->tuple<ir_itr, ii_itr, oi_itr>
  //   // may throw

  const int a[] = {1, 2, 3};
  const int b[] = {2, 4, 6};
  int c[3];
  transform(a, begin(c), negate{});
  assert(equal(c, vector<int>{-1, -2, -3}));
  transform(a, begin(b), begin(c), plus{});
  assert(equal(c, vector<int>{3, 6, 9}));
}
void doc_replace_if_copy_if() {
  // replace(fr, const T &x, const U &y) // may throw
  // replace_plus(fr, x, y)->fr_itr // may throw
  //
  // replace_if(fr, eq, y) // may throw
  //   // call eq(*fr_i) to check
  // replace_if_plus(fr, eq, y)->fr_itr // may throw
  //
  // replace_copy(ir, oi, x, y)->oi_itr // may throw
  // replace_copy_plus(ir, oi, x, y)->pair<ir_itr, oi_itr> // may throw
  //
  // replace_copy_if(ir, oi, eq, y)->oi_itr // may throw
  //   // call eq(*fr_i) to check
  // replace_copy_if_plus(ir, oi, eq, y)->pair<ir_itr, oi_itr> // may throw

  int a[] = {1, 2, 3};
  replace(a, 2, 3);
  assert(equal(a, seq(1, 3, 3)));
}
void doc_fill() {
  // fill(ir, const T &x) // may throw
  // fill_plus(ir, x)->ir_itr // may throw
  //
  // fill_zero(ir) // may throw
  // fill_zero_plus(ir)->ir_itr // may throw
  //
  // bytewise_fill(cr, x) // may throw
  //   // fill with static_cast<unsigned char>(x) for all bytes

  int a[] = {1, 2, 3};
  fill(a, 0);
  assert(equal(a, seq(0, 0, 0)));

  copy(seq(1, 2, 3), begin(a));
  fill_zero(a);
  assert(equal(a, seq(0, 0, 0)));

  copy(seq(1, 2, 3), begin(a));
  bytewise_fill(a, 'a');
  assert(all_of_equal
         (rng((const char *)(void *)begin(a), sizeof(int) * 3), 'a'));
}
void doc_generate() {
  // generate(ir, ufn) // may throw
  // generate_plus(ir, ufn)->ir_itr // may throw

  int a[3] = {};
  generate(a, [i = 0]() mutable {return ++i;});
  assert(equal(a, seq(1, 2, 3)));
}
void doc_remove_if_copy_if() {
  // remove(const char *fname) // std::remove
  //
  // remove_if(fr, eq)->fr_itr // may throw // call eq(*fr_itr) to check
  // remove(fr, const T &x)->fr_itr // may throw
  //
  // remove_copy_if(ir, oi, eq)->oi_itr // may throw
  // remove_copy_if_plus(ir, oi, eq)->pair<ir_itr, oi_itr> // may throw
  //
  // remove_copy(ir, oi, x)->oi_itr // may throw
  // remove_copy_plus(ir, oi, x)->pair<ir_itr, oi_itr> // may throw

  int aa[] = {1, 0, 0, 2, 0, 3, 0, 0, 0, 4};
  int *const p = re::remove(aa, 0);
  assert(equal(rng(begin(aa), p), seq(1, 2, 3, 4)));

  int a2[] = {1, 2, 0, 3, 4};
  int a3[] = {0, 0, 0, 0, 0};
  assert(remove_copy(a2, begin(a3), 0) == nth(a3, 4));
  assert(equal(a3, seq(1, 2, 3, 4, 0)));
}
void doc_unique_copy() {
  // unique(fr, eq = equal_to{})->fr_itr // may throw
  //
  // unique_copy(ir, oi, eq = equal_to{})->oi_itr // may throw
  // unique_copy_plus(ir, oi, eq = equal_to{})->pair<ir_itr, oi_itr>
  //   // may throw

  int a[] = {1, 2, 2, 3, 3, 3};
  const int *p = unique(a);
  assert(equal(rng(cbegin(a), p), seq(1, 2, 3)));
}
void doc_reverse_copy() {
  // reverse(br, swap_fn = adl_swap) // may throw
  // reverse_copy(br, oi)->oi_itr // may throw

  int a[] = {1, 2, 3};
  reverse(a);
  assert(equal(a, rrng(seq(1, 2, 3))));
}
void doc_rotate_copy() {
  // rotate(fr, fr_i, do_swap = adl_swap)->fr_itr // may throw
  // rotate_copy(fr, fr_i, oi)->oi_itr // may throw

  int a[] = {1, 2, 3, 4, 5};
  int *it = rotate(a, begin(a) + 2);
  assert(equal(a, seq(3, 4, 5, 1, 2)));
  assert(equal(rng(begin(a), it), seq(3, 4, 5)));
  assert(equal(rng(it, end(a)), seq(1, 2)));
}
void doc_shift() {
  // shift_left(fr, fr_i)->fr_itr // may throw
  //   // shift [fr_i, end(fr)) to leftmost of fr
  //   // return end of the range after move
  // shift_left(fr, n)->fr_itr // may throw
  //   // shift the last max(0, ssize(fr) - n) elements to leftmost of fr
  //   // the shift distance is n
  //   // return end of the range after move
  //
  // shift_right(fr, fr_i)->fr_itr // may throw
  //   // shift [begin(fr), fr_i) to rightmost of fr
  //   // return begin of the range after move
  // shift_right(fr, n)->fr_itr // may throw
  //   // shift the first max(0, ssize(fr) - n) elements to rightmost of fr
  //   // the shift distance is n
  //   // return begin of the range after move

  ez_vector<int> v = {1, 2, 3, 4};
  assert(equal(rng(begin(v), shift_left(v, nth(v, 2))), seq(3, 4)));

  v = {1, 2, 3, 4};
  assert(equal(rng(shift_right(v, 1), end(v)), seq(1, 2, 3)));
  assert(equal(rng(shift_right(v, 3), end(v)), seq(1)));
  assert(equal(rng(shift_right(v, 4), end(v)), empty_rng<int>()));
}
void doc_sample() {
  // sample(ir, or, rand_engine_ref)->or_itr // may throw
  //   // take random min(size(ir), size(or)) elements to copy to or.begin()
  //   // return a iterator to indicate the used space in or
  //   // the selected elements copied to or keep the original order

  int a[3] = {1, 2, 3};
  int a2[3] = {};
  sample(a, a2, minstd_rand{});
  assert(equal(a2, seq(1, 2, 3)) && equal(a, a2));

  int a3[4] = {};
  const auto it = sample(a, a3, minstd_rand{});
  assert(it == nth(a3, 3));
  assert(equal(rng(begin(a3), it), seq(1, 2, 3)));
}
void doc_shuffle() {
  // shuffle(rr, rand_engine_ref) // may throw

  int a[] = {1, 2, 3};
  shuffle(a, minstd_rand{});
  assert(is_permutation(a, seq(1, 2, 3)));
}
void doc_binary_search_series() {
  // partition_point(fr, eq)->fr_itr // may throw
  //
  // lower_bound(fr, x, less_f = less{})->fr_itr // may throw
  // upper_bound(fr, x, less_f = less{})->fr_itr // may throw
  // equal_range(fr, x, less_f = less{})->fr_itr // may throw
  //   // equal_range = [lower_bound, upper_bound)
  //
  // binary_search(fr, x, less_f = less{})->bool // may throw
  // bs_find_range(fr, min, max, less_f = less{})->iter_pair<fr_itr>
  //   // may throw
  // bs_find(fr, x, less_f = less{})->fr_itr // may throw

  int a[] = {1, 2, 3, 8, 9, 10};

  assert(equal(equal_range(a, 4), empty_rng<int>()));
  assert(equal(equal_range(a, 3), seq(3)));

  assert(binary_search(a, 3) == true);
  assert(binary_search(a, 4) == false);
  assert(equal(bs_find_range(a, 3, 9), seq(3, 8, 9)));
  assert(equal(bs_find_range(a, 4, 7), empty_rng<int>()));
  assert(bs_find(a, 3) == nth(a, 2));
  assert(bs_find(a, 4) == end(a));
}
void doc_partition_series() {
  // is_partitioned(ir, eq)->bool // may throw
  //
  // partition(fr, eq)->fr_itr // may throw
  // partition_copy(ir, oi_true, oi_false, eq)
  //   ->pair<oi_true_itr, oi_false_itr>
  //   // may throw // call eq(*ir_i) to check
  // partition_copy_plus(ir, oi_true, oi_false, eq)
  //   ->pair<ir_itr, oi_true_itr, oi_false_itr>
  //   // may throw
  //
  // stable_partition(br, eq)->br_itr // may throw // call eq(*br_i) to check
  // stable_partition_with_buffer(fr, buf_ref, eq)->fr_itr // may throw
  //   // buf can be buffer<fr_vt> or vector<fr_vt>,
  //   //   and other similar container-like object is also acceptable
  // stable_partition_with_no_buffer(br, eq)->br_itr // may throw

  int a[] = {0, 0, 0, 1, 1, 1};
  assert(is_partitioned(a, bind(equal_to{}, _1, 0)));
  assert(!is_partitioned(a, bind(equal_to{}, _1, 1)));
  const auto it = partition(a, bind(equal_to{}, _1, 1));
  assert(it == nth(a, 3));
  assert(equal(a, seq(1, 1, 1, 0, 0, 0)));
}
void doc_merge_series() {
  // merge(ir, ir2, oi, less_f = less{})->oi_itr // may throw
  // merge_plus(ir, ir2, oi, less_f = less{})->tuple<ir_itr, ir2_itr, oi_itr>
  //   // may throw
  //
  // inplace_merge_with_no_buffer(br, bi, less_f = less{}) // may throw
  // inplace_merge_with_buffer(br, bi, buf_ref, less_f = less{}) // may throw
  //   // buf can be buffer<fr_vt> or vector<fr_vt>,
  //   //   and other similar container-like object is also acceptable
  // inplace_merge(br, bi, less_f = less{}) // may throw

  int a[] = {1, 3, 5};
  int a2[] = {2, 4, 6};
  vector<int> v;
  merge(a, a2, to_back(v));
  assert(equal(v, seq(1, 2, 3, 4, 5, 6)));

  int d[] = {1, 3, 5, 2, 4};
  inplace_merge(d, begin(d) + 3);
  assert(equal(d, seq(1, 2, 3, 4, 5)));
}
void doc_set_series() {
  // includes(ir, ir2, less_f = less{})->bool // may throw
  //
  // set_union(ir, ir2, oi, less_f = less{})->oi_itr // may throw
  // set_union_plus(ir, ir2, oi, less_f = less{}) // may throw
  //
  // set_intersection(ir, ir2, oi, less_f = less{})->oi_itr // may throw
  // set_intersection_plus(ir, ir2, oi, less_f = less{}) // may throw
  //
  // set_difference(ir, ir2, oi, less_f = less{})->oi_itr // may throw
  // set_difference_plus(ir, ir2, oi, less_f = less{}) // may throw
  //
  // set_symmetric_difference(ir, ir2, oi, less_f = less{})->oi_itr // may throw
  // set_symmetric_difference_plus(ir, ir2, oi, less_f = less{}) // may throw

  int a[] = {1, 3, 5, 7};
  int b[] = {5, 7, 9, 10};
  ez_vector<int> c, d, e, f;

  set_union(a, b, to_back(c));
  assert(includes(c, a) && includes(c, b));
  assert(equal(c, seq(1, 3, 5, 7, 9, 10)));

  set_intersection(a, b, to_back(d));
  assert(equal(d, seq(5, 7)));

  set_difference(a, b, to_back(e));
  assert(equal(e, seq(1, 3)));

  set_symmetric_difference(a, b, to_back(f));
  assert(equal(f, seq(1, 3, 9, 10)));
}
void doc_heap_series() {
  // push_heap(rr, less_f = less{}) // may throw
  // pop_heap(rr, less_f = less{}) // may throw
  // make_heap(rr, less_f = less{}) // may throw
  // sort_heap(rr, less_f = less{}) // may throw
  // is_heap(rr, less_f = less{})->bool // may throw
  // is_heap_until(rr, less_f = less{})->rr_itr // may throw
  //
  // heap_sort(rr, less_f = less{}) // may throw
  //   // call make_heap(), and then sort_heap()

  int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  shuffle(v, minstd_rand{});
  for (auto &i : iters(1, ssize(v) + 1))
    push_heap(rng(begin(v), i));
  assert(is_heap(v));
  assert(front(v) == 9);
  pop_heap(v);
  assert(is_heap(rng(begin(v), 8)));
  assert(back(v) == 9);
  assert(is_permutation(v, irng(1, 10)));
}
void doc_min_max() {
  // min_element(fr, less_f = less{})->fr_itr // may throw
  //   // get the first element than which no one is smaller
  // max_element(fr, less_f = less{})->fr_itr // may throw
  //   // get the first element than which no one is greater
  // minmax_element(fr, less_f = less{})->pair<fr_itr, fr_itr> // may throw
  //   // the min element is min_element(fr, less)
  //   // the max element is the rightmost one among equal values
  //
  // min(const T &x, const T &y, less_f = less{})->const T & // may throw
  // min({...}, less_f = less{})->T // may throw
  // max(const T &x, const T &y, less_f = less{})->const T & // may throw
  // max({...}, less_f = less{})->T // may throw
  // minmax(const T &x, const T &y, less_f = less{})->pair<const T &, const T &>
  //   // may throw
  // minmax({...}, less_f = less{})->pair<T, T> // may throw

  int x = 2;
  int y = 1;
  const auto [a, b] = minmax(x, y);
  assert((&a == &y) && (&b == &x));
}
void doc_clamp() {
  // clamp(const T &x, const T &lower, const T &upper,
  //       less_f = less{})->const T & // may throw

  assert(clamp(0, 1, 3) == 1);
  assert(clamp(1, 1, 3) == 1);
  assert(clamp(2, 1, 3) == 2);
  assert(clamp(3, 1, 3) == 3);
  assert(clamp(4, 1, 3) == 3);
}
void doc_lexicographical_compare() {
  // lexicographical_compare(ir1, ir2, less_f = less{})->bool // may throw
  //
  // lexicographical_compare_three_way(ir1, ir2, cmp = compare_three_way)
  //   ->delctype(cmp(ir1, ir2))
  //   // may throw
  // lexicographical_synth_3way(ir1, ir2)->decltype(synth_3way(ir1, ir2))
  //   // may throw
  //   // lexicographical_compare_three_way(ir1, ir2)

  assert(lexicographical_synth_3way("abc"_s, "ad"_s) == strong_lt);
  assert(lexicographical_synth_3way("abc"_s, "ab"_s) == strong_gt);
  assert(lexicographical_synth_3way("abc"_s, "abc"_s) == strong_eq);
}
void doc_permutation() {
  // next_permutation(r, less_f = less{})->bool // may throw
  // prev_permutation(r, less_f = less{})->bool // may throw

  int a[] = {1, 2, 3};
  assert(next_permutation(a));
  assert(equal(a, seq(1, 3, 2)));
  assert(next_permutation(a));
  assert(equal(a, seq(2, 1, 3)));
  assert(next_permutation(a));
  assert(equal(a, seq(2, 3, 1)));
  assert(next_permutation(a));
  assert(equal(a, seq(3, 1, 2)));
  assert(next_permutation(a));
  assert(equal(a, seq(3, 2, 1)));
  assert(!next_permutation(a));
  assert(equal(a, seq(1, 2, 3)));
}
void doc_sort_series() {
  // is_sorted_until(fr, less_f = less{})->fr_itr // may throw
  // is_sorted(fr, less_f = less{})->bool // may throw
  //
  // sort(rr, less_f = less{}) // may throw
  //
  // stable_sort(rr, less_f = less{}) // may throw
  // stable_sort_with_no_buffer(rr, less_f = less{}) // may throw
  // stable_sort_with_buffer(rr, buf_ref, less_f = less{}) // may throw
  //   // buf can be buffer<fr_vt> or vector<fr_vt>,
  //   //   and other similar container-like object is also acceptable
  //
  // partial_sort(rr, rr_i, less_f = less{}) // may throw
  //   // rearrange rr to make [rr.begin(), rr_i) be sorted smalleast sub-range
  //   //   of sorted rr
  // partial_sort_copy(fr, fr_i, less_f = less{}) // may throw
  //
  // nth_element(rr, rr_i, less_f = less{}) // may throw
  //   // rearrange rr
  //   //   to make *rr_i to be the corresponding element of sorted rr,
  //   //   and make any of the left-sub-range smaller
  //   //   than any of the right-sub-range

  int a[] = {5, 3, 4, 1, 6, 2, 7};
  int *i = nth(a, 3);
  partial_sort(a, i);
  assert(equal(rng(begin(a), i), seq(1, 2, 3)));

  int a2[] = {5, 3, 4, 1, 6, 2, 7};
  int *i2 = nth(a2, 3);
  nth_element(a2, i2);
  sort(rng(begin(a2), i2));
  sort(rng(next(i2), end(a2)));
  assert(equal(a2, seq(1, 2, 3, 4, 5, 6, 7)));
}
void doc_accumulate_reduce() {
  // accumulate(ir, T init = ir_vt{}, plus_f = plus{})->T // may throw
  //   // note: std::accumulate has no default init argument,
  //   //   add it for convenience
  // accumulate_plus(ir, init = ir_vt{}, plus_f = plus{})->pair<ir_itr, T>
  //   // may throw
  //
  // reduce(ir, T init = ir_vt{}, plus_f = plus{})->T // may throw
  // reduce_plus(ir, init = ir_vt{}, plus_f = plus{})->pair<ir_itr, T>
  //   // may throw

  assert(accumulate(seq(1, 2, 3)) == 6);
  assert(reduce(seq(1, 2, 3)) == 6);
}
void doc_inner_product() {
  // inner_product(ir, ii, T init, plus_f, mul_f)->T // may throw
  // inner_product(ir, ii, T init)->T // may throw
  // inner_product_plus(ir, ii, T init, plus_f, mul_f)
  //   ->tuple<ir_itr, ii_itr, T> // may throw
  // inner_product_plus(ir, ii, T init)->tuple<ir_itr, ii_itr, T> // may throw
  //
  // transform_reduce(ir, ii, T init, plus_f, mul_f)->T // may throw
  // transform_reduce(ir, ii, T init)->T // may throw
  // transform_reduce(ir, T init, plus_f, proj_f)->T // may throw
  // transform_reduce_plus(ir, ii, T init, plus_f, mul_f)
  //   ->tuple<ir_itr, ii_itr, T> // may throw
  // transform_reduce_plus(ir, ii, T init)
  //   ->tuple<ir_itr, ii_itr, T> // may throw
  // transform_reduce_plus(ir, T init, plus_f, proj_f)
  //   ->pair<ir_itr, T> // may throw

  assert(transform_reduce(seq(2, 3), begin(seq(4, 5)), 0) == (2 * 4 + 3 * 5));
  assert(transform_reduce(seq(2, 3), begin(seq(4, 5)), 0)
         == inner_product(seq(2, 3), begin(seq(4, 5)), 0));

  assert(transform_reduce(seq(2, 3), 0, plus{}, negate{}) == (-2 + -3));
}
void doc_partial_sum_series() {
  // partial_sum(ir, oi, plus_f = plus{})->oi_itr // may throw
  // partial_sum_plus(ir, oi, plus_f = plus{}) // may throw
  {
    int a[] = {1, 3, 5};
    int b[3];
    partial_sum(a, begin(b));
    assert(equal(b, seq(1, 4, 9)));
  }  
  // inclusive_scan(ir, oi, plus_f = plus{})->oi_itr // may throw
  // inclusive_scan(ir, oi, plus_f, T init)->oi_itr // may throw
  // transform_inclusive_scan(ir, oi, plus_f, proj_f)->oi_itr // may throw
  // transform_inclusive_scan(ir, oi, plus_f, proj_f, T init)->oi_itr
  //   // may throw
  // inclusive_scan_plus(ir, oi, plus_f = plus{})->pair<ir_itr, oi_itr>
  //   // may throw
  // inclusive_scan_plus(ir, oi, plus_f, T init)->pair<ir_itr, oi_itr>
  //   // may throw
  // transform_inclusive_scan_plus(ir, oi, plus_f, proj_f)
  //   ->pair<ir_itr, oi_itr> // may throw
  // transform_inclusive_scan_plus(ir, oi, plus_f, proj_f, T init)
  //   ->pair<ir_itr, oi_itr> // may throw
  //   // inclusive_scan(ir, oi) == partial_sum(ir, oi)
  //   // note: std:: version of inclusive_scan may use concurrency optimization
  //   //   as the evaluation sequence, but I do not care it to make easy
  {
    const int a[] = {1, 3, 5};
    int b[3];

    assert(inclusive_scan(a, begin(b)) == end(b));
    assert(equal(b, seq(1, 4, 9)));
    assert(inclusive_scan(a, begin(b), plus{}, 10) == end(b));
    assert(equal(b, seq(11, 14, 19)));

    assert(transform_inclusive_scan(a, begin(b), plus{}, negate{}) == end(b));
    assert(equal(b, seq(-1, -4, -9)));
    assert(transform_inclusive_scan(a, begin(b), plus{}, negate{}, 100)
           == end(b));
    assert(equal(b, seq(99, 96, 91)));
  }
  // exclusive_scan(ir, oi, T init, plus_f = plus{})->oi_itr // may throw
  // transform_exclusive_scan(ir, oi, T init, plus_f, proj_f)->oi_itr
  //   // may throw
  // exclusive_scan_plus(ir, oi, init, plus_f = plus{})->pair<ir_itr, oi_itr>
  //   // may throw
  // transform_exclusive_scan_plus(ir, oi, init, plus_f, proj_f)
  //   ->pair<ir_itr, oi_itr> // may throw
  //   // ...exclusive_scan must have init arg
  //   //   because the nth output do not use the nth element in ir
  //   // note: std:: version of inclusive_scan may use concurrency optimization
  //   //   as the evaluation sequence, but I do not care it to make easy
  {
    const int a[] = {1, 3, 5};
    int b[3];

    exclusive_scan(a, begin(b), 100);
    assert(equal(b, seq(100, 101, 104)));
    transform_exclusive_scan(a, begin(b), 100, plus{}, negate{});
    assert(equal(b, seq(100, 99, 96)));
  }
}
void doc_adjacent_difference() {
  // adjacent_difference(ir, oi, minus_f = minus{})->oi_itr // may throw
  // adjacent_difference_plus(ir, oi, minus_f = minus{})->pair<ir_itr, oi_itr>
  //   // may throw

  int a[] = {1, 3, 9, 11};
  int b[4];
  adjacent_difference(a, begin(b));
  assert(equal(b, seq(1, 2, 6, 2)));
}
void doc_iota() {
  // iota(fr, x) // may throw

  int a[3];
  iota(a, 1);
  assert(equal(a, irng(1, 4)));
}
void doc_gcd_and_lcm() {
  // gcd(x, y) // may throw
  //   // x >= 0 && y >= 0, the result is not right if overflow
  // lcm(x, y) // may throw
  //   // x >= 0 && y >= 0, the result is not right if overflow

  assert(gcd(6, 9) == 3);
  assert(lcm(6, 9) == 18);

  assert(gcd(0, 0) == 0);
  assert(lcm(0, 0) == 0);
  assert(gcd(0, 9) == 9);
  assert(lcm(0, 9) == 0);
}
void doc_midpoint() {
  // midpoint(T x, T y) // T is integral, floating-point, or pointer

  assert(midpoint(1, 3) == 2);
  assert(midpoint(3, 1) == 2);
  assert(midpoint(1, 4) == 2);
  assert(midpoint(4, 1) == 3);
  assert(midpoint(-1, 1) == 0);
  assert(midpoint(1, -1) == 0);
  assert(midpoint(-1, 2) == 0);
  assert(midpoint(2, -1) == 1);

  assert(midpoint(1.0, 2.0) == 1.5);

  int a[5] = {};
  auto p1 = begin(a) + 0;
  auto p2 = begin(a) + 5;
  assert(midpoint(p1, p2) == begin(a) + 2);
  assert(midpoint(p2, p1) == begin(a) + 3);
}
void doc_for_each_node() {
  // for_each_node(fr, get_next, fn, destroyf = empty_function{}) // may throw
  //   // iterator operations can not throw
  //   // fn's usage is fn(iterator),
  //   //   can invalidate the iterator and there is no influence to the rest
  //   // when throwing from fn(iterator) at nth element,
  //   //   apply destroyf for all iterators in [nth, end)

  list<int> l = {1, 2, 3};
  for_each_node(l, next, [&l](auto it) {l.erase(it);});
  assert(l.empty());
}
void doc_list_fns() {
  // list_functions<GET_NEXT, SET_NEXT, GET_PREV>
  //   special member functions:
  //     default-constructible (dependent)
  //     move-constructible (dependent)
  //   list_functions(GET_NEXT, SET_NEXT, GET_PREV, SET_PREV)
  //   link(next, node)->iter
  //   link(next, first_it, last_it)->iter
  //   unlink(node)->iter
  //   unlink(from, to)->iter
  //   unlink_all(end_node)
  // list_fns(get_next, set_next, get_prev, set_prev)->list_functions<...>
  // list_fns()->list_functions<...>
  //   // get_next(it): return it.next()
  //   // set_next(it, x): call it.next(x)
  //   // get_prev(it): return it.prev()
  //   // set_prev(it, x): call it.prev(x)
  //
  // forward_list_functions<GET_NEXT, SET_NEXT>
  //   special member functions:
  //     default-constructible (dependent)
  //     move-constructible (dependent)
  //   forward_list_functions(GET_NEXT, SET_NEXT)
  //   link_after(prev, node)->iter
  //   link_after(prev, it, last_it)->iter
  //   unlink_after(prev)->iter
  //   unlink_after(prev, next)->iter
  //   unlink_all(end_node)
  // forward_list_fns(get_next, set_next)->forward_list_functions<...>
  // forward_list_fns()->forward_list_functions<...>
  //   // get_next(it): return it.next()
  //   // set_next(it, x): call it.next(x)
}
void doc_list_unique() {
  // list_unique(l, eq, erase_f) // may throw

  list<int> l = {1, 2, 2, 3, 3, 3};
  list_unique(l, equal_to{}, [&l](auto i) {return l.erase(i);});
  assert(equal(l, irng(1, 4)));
}
void doc_two_way_search() {
  // two_way_search(rr, pattern_rr, eq = ...)->rr_itr

  const char a[] = "aaaaaddddcc";
  assert(two_way_search(a, "dd"_s) == nth(a, 5));
}
void doc_combination_range() {
  // permutation_count(const T &n, const T &k = n)->T // may throw
  //   // A(k, n)
  //   // k <= n, {n, k} >= 0, the result can not overflow
  //   // if k == 0 or k == n, return 1
  // combination_count(const T &n, const T &k)->T // may throw
  //   // C(k, n)
  //   // k <= n, {n, k} >= 0, the result can not overflow 
  //   // if k == 0 or k == n, return 1
  {
    assert(permutation_count(9, 0) == 1);
    assert(permutation_count(9) == 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2);

    assert(combination_count(9, 0) == 1);
    assert(combination_count(9, 4)
           == permutation_count(9, 4) / permutation_count(4));
    assert(combination_count(9, 4) == (9 * 8 * 7 * 6) / (4 * 3 * 2));
  }

  // combination_iterator<BR_ITER, BUF_T>
  //   // value_type of container BUF_T is BR_ITER
  //   iterator category: iitr
  //   reference: bind_range<BUF_T &, decay<decltype(deref)>>
  //   combination_iterator(BUF_T *, BR_ITER r_ed)
  // combination_itr(bufp, r_ed)->combination_iterator<...>
  //   // size of *bufp is among (0, size(r)]
  {
    auto a = seq(1, 2, 3);
    ez_vector<int *> v{addressof(a[0]), addressof(a[1])};
    auto i = combination_itr(addressof(v), end(a));
    decltype(i) j(nullptr, end(a));
    assert(equal(rng(i, j), seq(seq(1, 2), seq(1, 3), seq(2, 3)), equal));
  }

  // combination_range<BR>
  //   cache: yes
  //   iterator category: iitr
  //   special member functions: dependent
  //   reference: itr_ref<combination_iterator<rng_itr<BR>, ...>>
  //   combination_range(BR br, rng_dft<BR> n)
  //     // 0 <= n <= ssize(br)
  // combination_rng(R &&br, rng_dft<R> n)->combination_range<...>
  //   // 0 <= n <= ssize(br)
  {
    assert(equal(combination_rng(seq(1, 2, 3), 0),
                 empty_rng<empty_range<int>>(),
                 equal));
    assert(equal(combination_rng(seq(1, 2, 3), 1),
                 seq(seq(1), seq(2), seq(3)),
                 equal));
    assert(equal(combination_rng(seq(1, 2, 3), 2),
                 seq(seq(1, 2), seq(1, 3), seq(2, 3)),
                 equal));
    assert(equal(combination_rng(seq(1, 2, 3), 3),
                 seq(seq(1, 2, 3)),
                 equal));
  }

  // combination(br, br_szt n, oi)->oi_itr // 0 <= n <= size(br)
  {
    const ez_vector<int> v = {1, 2, 3};
    ez_vector<ez_vector<int>> vv;
    const auto oi = output_itr([&vv](auto &&r) {
      ez_vector<int> tmp;
      for (int x : r)
        tmp.insert(tmp.end(), x);
      vv.insert(vv.end(), move(tmp));
    });

    combination(v, 0, oi);
    assert(vv.empty());

    combination(v, 2, oi);
    assert(equal(vv, seq(seq(1, 2), seq(1, 3), seq(2, 3)), equal));
    vv.erase(vv.begin(), vv.end());

    combination(v, 3, oi);
    assert(equal(vv, seq(seq(1, 2, 3)), equal));
  }
}
void doc_filter_range() {
  // filter(ir, oi, eq)->oi_itr
  // filter_plus(ir, oi, eq)->pair<ir_itr, oi_itr>
  //
  // filter_iterator<II, F>
  //   iterator category:
  //     conditional<is_fitr<II>, forward_iterator_tag, itr_ctg<II>>
  //       = iitr - fitr
  //   reference: itr_ref<II>
  //   filter_iterator(II, II, F)
  //   filter_iterator(const filter_iterator<II2, F2> &)
  //   base()->II
  //   fn()->F
  // filter_itr(it, ed, f)->filter_iterator<...>
  //
  // filter_range<IR, F>
  //   cache: yes, delay if !is_rng_ref<IR>
  //   iterator category:
  //     conditional<is_fitr<rng_itr<IR>>,
  //                 forward_iterator_tag, itr_ctg<rng_itr<IR>>>
  //       = iitr - fitr
  //   reference:
  //     rng_ref<IR>
  //     rng_ref<const IR> if is_rng_ref<const R>
  //   special member functions: dependent
  //   filter_range(IR, F)
  // filter_rng(r, f)->filter_range<...>

  int a[] = {0, 0, 1, 0, 1, 1, 0};
  int b[3];
  assert(filter(a, begin(b), bind(equal_to{}, _1, 1)) == end(b));
  assert(equal(b, rng(3, 1)));

  assert(equal(filter_rng(seq(0, 1, 0, 1), bind(equal_to{}, _1, 1)),
               rng(2, 1)));
}
void doc_take_range() {
  // take(r, n, oi)->oi_itr
  // take_plus(r, n, oi)->pair<r_itr, oi_itr>
  //
  // take_iterator<I>
  //   iterator category:
  //     conditional<is_fitr<I>, forward_iterator_tag, itr_ctg<I>>
  //       = iitr - fitr
  //   reference: itr_ref<I>
  //   take_iterator(I, itr_dft<I>)
  //   base()->I
  //   nth()->itr_dft<I>
  // take_itr(i, n)->take_iterator<...>
  //
  // take_range<R>
  //   cache: no
  //   iterator category:
  //     conditional<is_frng<R>, forward_iterator_tag, rng_ctg<R>>
  //       = iitr - fitr
  //   reference: itr_ref<I>
  //   special member functions: dependent
  //   take_range(R, rng_dft<R>)
  // take_rng(r, n)->take_range<...>

  const auto r = seq(1, 2, 3);
  assert(equal(take(r, 4, to_back(ez_vector<int>{})).base(), seq(1, 2, 3)));
  assert(equal(take(r, 2, to_back(ez_vector<int>{})).base(), seq(1, 2)));
  assert(equal(take(r, 1, to_back(ez_vector<int>{})).base(), seq(1)));
  assert(equal(take_rng(r, 0), empty_rng<int>()));
  assert(equal(take_rng(r, 2), seq(1, 2)));
  assert(equal(take_rng(r, 4), irng(1, 4)));
}
void doc_drop_range() {
  // drop(r, n, oi)->oi_itr
  // drop_plus(r, n, oi)->pair<r_itr, oi_itr>
  //
  // drop_range<R>
  //   cache: yes, delay if !is_rng_ref<R>
  //   iterator category: rng_itr<R>
  //   reference: rng_ref<R>
  //   special member functions: dependent
  //   drop_range(R, rng_dft<R>)
  // drop_rng(r, n)->drop_range<...>

  const auto r = seq(1, 2, 3);
  assert(equal(drop(r, 4, to_back(ez_vector<int>{})).base(), empty_rng<int>()));
  assert(equal(drop(r, 2, to_back(ez_vector<int>{})).base(), seq(3)));
  assert(equal(drop(r, 1, to_back(ez_vector<int>{})).base(), seq(2, 3)));
  assert(equal(drop_rng(r, 0), irng(1, 4)));
  assert(equal(drop_rng(r, 2), seq(3)));
  assert(equal(drop_rng(r, 4), empty_rng<int>()));
}
void doc_take_while_range() {
  // take_while(r, f, oi)->oi_itr
  // take_while_plus(r, f, oi)->pair<r_itr, oi_itr>
  //
  // take_while_iterator<I, F>
  //   iterator category:
  //     conditional<is_fitr<I>, forward_iterator_tag, itr_ctg<I>> = iitr - fitr
  //   reference: itr_ref<I>
  //   take_while_iterator(I, I, F)
  //   base()->I
  //   fn()->F // the stored function is non-empty
  // take_while_itr(i, ed, f)->take_while_iterator<...>
  //
  // take_while_range<R, F>
  //   cache: no
  //   iterator category:
  //     conditional<is_frng<R>, forward_iterator_tag, rng_ctg<R>> = iitr - fitr
  //   reference:
  //     rng_ref<R>
  //     rng_ref<const R> if is_rng<const R>
  //   special member functions: dependent
  //   take_while_range(r, f)
  // take_while_rng(r, f)->take_while_range<...>

  const auto r = seq(1, 2, 3);
  assert(equal(take_while(r,
                          [](int x) {return x < 3;},
                          to_back(ez_vector<int>{})).base(),
               seq(1, 2)));
  assert(equal(take_while_rng(r,
                              [](int x) {return x < 3;}),
               seq(1, 2)));
}
void doc_drop_while_range() {
  // drop_while(r, f, oi)->oi_itr
  // drop_while_plus(r, f, oi)->pair<r_itr, oi_itr>
  //
  // drop_while_range<R, F>
  //   cache: yes, delay if !is_rng_ref<R>
  //   iterator category: rng_itr<R> = iitr - citr
  //   reference:
  //     rng_ref<R>
  //     rng_ref<const R> if is_rng_ref<const R>
  //   special member functions: dependent
  //   drop_while_range(R, F)
  // drop_while_rng(r, f)->drop_while_range<...>

  const auto r = seq(1, 2, 3);
  assert(equal(drop_while(r,
                          [](int x) {return x < 3;},
                          to_back(ez_vector<int>{})).base(),
               seq(3)));
  assert(equal(drop_while_rng(r,
                              [](int x) {return x < 3;}),
               seq(3)));
}
void doc_join_range() {
  // join(r, oi)->oi_itr
  //
  // join_iterator<II> // is_rng_ref<itr_ref<II>>
  //   iterator category: iitr - bitr
  //   reference: rng_ref<itr_ref<II>>
  //   join_iterator(II it, rng_itr<itr_ref<II>> inner_it, II it2)
  //   base()->rng_itr<itr_ref<II>>
  //   outer_iter()->II
  // join_itr(it, inner_it, it2)->join_iterator<...>
  //   // is_rng_ref<itr_ref<decltype(it)>>
  //
  // join_range<IR>
  //   cache: yes, delay if !is_rng_ref<IR>
  //   iterator category: iitr - bitr
  //   reference:
  //     rng_ref<rng_ref<IR>>
  //     rng_ref<rng_ref<const IR>> if is_rng_ref<add_const<IR>>
  //   special member functions: dependent
  //   explicit this_t(IR)
  //   for_each(f) // quick because working around iterator structure
  // join_rng(r)->join_range<...>
  // join_rng(it, it2)->decltype(join_rng(rng(it, it2)))
  // join_rng(it, n)->decltype(join_rng(rng(it, n)))

  assert(equal(join(seq("ab"_sv, "cd", "e"), to_back(ez_vector<int>{})).base(),
               "abcde"_sv));

  ez_vector<ez_vector<int>> a = {{1, 2}, {3, 4}, {5}};
  assert(equal(rng(join_itr(begin(a), begin(*begin(a)), end(a)),
                   join_itr(end(a), decltype(begin(*begin(a))){}, end(a))),
               seq(1, 2, 3, 4, 5)));

  assert(equal(join_rng(seq(""_sv, "ab", "cd")), "abcd"_sv));
}
void doc_join_with_range() {
  // join_with(r, r2, oi)->oi_itr
  //
  // join_with_iterator<II, FI>
  //   iterator category: iitr - bitr
  //   reference: common_reference<rng_ref<itr_ref<II>>, itr_ref<FI>>
  //   join_with_iterator(it, inner_it, ed, op2, it2, ed2)
  // join_with_itr(it, inner_it, ed, op2, it2, ed2)->join_with_iterator<...>
  //
  // join_with_range<R, R2>
  //   cache: yes, delay if !(is_rng_ref<R>
  //                          && is_rng_ref<rng_ref<R>> && is_rng_ref<R2>)
  //   iterator category: iitr - bitr
  //   reference:
  //     common_reference<rng_ref<rng_ref<R>>, rng_ref<R2>>>
  //     common_reference<rng_ref<rng_ref<const R>>, rng_ref<const R2>>>
  //       if is_rng<const R> && is_rng<const R2>
  //   special member functions: dependent
  //   explicit this_t(R, R2)
  // join_with_rng(r, r2)->join_with_range<...>

  assert(equal(join_with(seq("ab"_sv, "cd"_sv, "e"_sv), "|"_sv,
                         to_back(ez_vector<int>{})).base(),
               "ab|cd|e"_sv));

  ez_vector<ez_vector<int>> v = {{1}, {2}, {3, 4}};
  ez_vector<int> v2 = {0};
  assert(equal(rng(join_with_itr(begin(v), begin(*begin(v)), end(v),
                                 begin(v2), end(v2), end(v2)),
                   join_with_itr(end(v), decltype(begin(*begin(v))){}, end(v),
                                 begin(v2), end(v2), end(v2))),
               seq(1, 0, 2, 0, 3, 4)));

  assert(equal(join_with_rng(seq(seq(1, 2), seq(4, 5)),
                             seq(0, 0, 0)),
               seq(1, 2, 0, 0, 0, 4, 5)));
}
void doc_adjacent_range() {
  // adjacent<N>(fr, oi)->oi_itr // N > 0
  // adjacent_plus<N>(fr, oi)->oi_itr // N > 0
  //
  // adjacent_iterator<FI, N> // N > 0
  //   iterator category:
  //     conditional<is_citr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = iitr - ritr
  //   reference: tuple<itr_ref<FI>, ...(repeat by n times)>
  //   explicit adjacent_iterator(tuple)
  //   base()->tuple<ITERS...>
  // adjacent_itr(its...)->adjacent_iterator<...>
  //   // all iterator argument have the same type
  //
  // adjacent_range<FR, N> // N > 0
  //   cache: no
  //   iterator category: iitr - ritr
  //   reference:
  //     tuple<rng_ref<FR>, ...(repeat by n times)>
  //     tuple<rng_ref<const FR>, ...(repeat by n times)> if is_rng<const FR>
  //   special member functions: dependent
  //   explicit this_t(FR)
  // adjacent_rng<N>(fr)->adjacent_range<...> // N > 0
  // bind_adjacent_rng<N>(fr, f)->auto // N > 0
  //   // same as bind_rng(adjacent_rng<N>(fr), bind(apply, f, _1))

  assert(equal(adjacent<3>(seq(1, 2, 3, 4, 5),
                           to_back(ez_vector<tuple<int, int, int>>{})).base(),
               seq(tuple(1, 2, 3), tuple(2, 3, 4), tuple(3, 4, 5))));

  const int a[] = {1, 2, 3, 4};
  assert(equal(rng(adjacent_itr(nth(a, 0), nth(a, 1)),
                   adjacent_itr(nth(a, 3), nth(a, 4))),
               seq(pair(1, 2), pair(2, 3), pair(3, 4))));
  assert(equal(rng(adjacent_itr(nth(a, 0), nth(a, 1), nth(a, 2)),
                   adjacent_itr(nth(a, 2), nth(a, 3), nth(a, 4))),
               seq(tuple(1, 2, 3), tuple(2, 3, 4))));

  assert(equal(adjacent_rng<2>(seq(1, 2, 3, 4)),
               seq(pair(1, 2), pair(2, 3), pair(3, 4))));
  assert(equal(adjacent_rng<3>(seq(1, 2, 3)),
               seq(tuple(1, 2, 3))));
  assert(equal(adjacent_rng<3>(seq(1, 2)),
               empty_rng<tuple<int, int, int>>()));
}
void doc_slide_range() {
  // slide(fr, n, oi)->oi_itr // n > 0
  //
  // slide_iterator<FI>
  //   iterator category: iitr - ritr
  //   reference: iter_pair<FI>
  //   slide_iterator(FI, FI)
  //   slide_iterator(FI, itr_dft<FI>)
  //   base()->iter_pair<FI>
  // slide_itr(i1, i2)->slide_iterator<...>
  // slide_itr(i1, n)->slide_iterator<...> // n > 0
  //
  // slide_range<FR>
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category: iitr - ritr
  //   reference:
  //     iter_pair<rng_itr<FR>>
  //     iter_pair<rng_itr<const FR>> if is_rng_ref<const FR>
  //   special member functions: dependent
  //   this_t(FR, rng_dft<FR> n) // n > 0
  // slide_rng(fr, n)->slide_range<...> // n > 0

  ez_vector<ez_vector<int>> v;
  slide(seq(1, 2, 3, 4), 3, output_itr([&v](auto view) {
    v.insert(v.end(), {});
    auto &vv = back(v);
    for (int x : view)
      vv.insert(back(v).end(), x);
  }));
  assert(equal(v, seq(seq(1, 2, 3), seq(2, 3, 4)), equal));

  int a[] = {1, 2, 3, 4};
  assert(equal(rng(slide_itr(nth(a, 0), nth(a, 0)),
                   slide_itr(nth(a, 3), nth(a, 3))),
               seq(seq(1), seq(2), seq(3)), equal));
  ez_forward_list<int> l = {1, 2, 3, 4};
  assert(equal(rng(slide_itr(nth(l, 0), nth(l, 1)), slide_itr(end(l), end(l))),
               seq(seq(1, 2), seq(2, 3), seq(3, 4)), equal));

  assert(equal(slide_rng(seq(1, 2, 3, 4), 2),
               seq(seq(1, 2), seq(2, 3), seq(3, 4)),
               equal));
  assert(equal(slide_rng(seq(1, 2), 2), seq(seq(1, 2)), equal));
  assert(equal(slide_rng(seq(1), 2), empty_rng<decltype(seq(1, 2))>(), equal));
}
void doc_aligned_stride_range() {
  // aligned_stride(r, n, oi)->oi // n > 0 && (size(r) % n) == 0
  //
  // aligned_stride_iterator<I>
  //   iterator category:
  //     conditional<is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>
  //       = iitr - ritr
  //   reference: itr_ref<I>
  //   aligned_stride_iterator(I, itr_dft<I>) // n > 0
  //   base()->I
  //   stride()->itr_dft<I>
  // aligned_stride_itr(i, n)->aligned_stride_iterator<...> // n > 0
  //
  // aligned_stride_range<R>
  //   cache: no
  //   iterator category:
  //     conditional<is_rrng<R>, random_access_iterator_tag, rng_ctg<R>>
  //       = iitr - ritr
  //   reference:
  //     rng_ref<R>
  //     rng_ref<const R> if is_rng<const R>
  //   aligned_stride_range(R r, rng_dft<R> n) // n > 0 && size(r) % n == 0
  // aligned_stride_rng(r, n)->aligned_stride_range<...>
  //   // n > 0 && size(r) % n == 0

  assert(equal(aligned_stride(seq(1, 2, 3, 4), 2,
                              to_back(ez_vector<int>{})).base(),
               seq(1, 3)));

  int a[] = {1, 2, 3, 4, 5, 6};
  assert(equal(rng(aligned_stride_itr(begin(a), 3),
                   aligned_stride_itr(end(a), 3)),
               seq(1, 4)));

  assert(equal(aligned_stride_rng(seq(1, 2, 3, 4, 5, 6), 3), seq(1, 4)));
}
void doc_stride_range() {
  // stride(r, s, oi)->oi_itr // s > 0
  //
  // stride_iterator<I>
  //   iterator category:
  //     conditional<is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>
  //       = iitr - ritr
  //   reference: itr_ref<I>
  //   stride_iterator(I i, I ed, itr_dft<I> s, itr_dft<I> missing = 0) // s > 0
  //   base()->I
  //   stride()->itr_dft<I>
  // stride_itr(i, ed, s, missing = 0)->stride_iterator<...>
  //
  // stride_range<R>
  //   cache: yes, delay if !(!is_brng<R> || is_rng_ref<R>)
  //   iterator category:
  //     conditional<is_rrng<R>, random_access_iterator_tag, rng_ctg<R>>
  //       = iitr - ritr
  //   reference:
  //     rng_ref<R>
  //     rng_ref<const R> if is_rng<const R> && !delay_cache_init
  //   special member functions: dependent
  //   this_t(R, rng_dft<R> s) // s > 0
  // stride_rng(r, s)->stride_range<...> // s > 0

  assert(equal(stride(seq(1, 2, 3, 4), 3, to_back(ez_vector<int>{})).base(),
               seq(1, 4)));

  int a[] = {1, 2, 3, 4, 5};
  test_ritr(rng(stride_itr(begin(a), end(a), 2, 0),
                stride_itr(end(a), end(a), 2, 1)),
            seq(1, 3, 5));

  assert(equal(stride_rng(seq(1, 2, 3, 4, 5), 2), seq(1, 3, 5)));
}
void doc_aligned_chunk_range() {
  // aligned_chunk(r, s, oi)->oi_itr // s > 0 && (size(r) % s) == 0
  //
  // aligned_chunk_iterator<FI>
  //   iterator category:
  //     conditional<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = fitr - ritr
  //   reference: decltype(rng(fi, fi_dif))
  //   aligned_chunk_iterator(FI, itr_dft<FI>) // s > 0
  //   base()->FI
  //   stride()->itr_dft<FI>
  // aligned_chunk_itr(i, s)->aligned_chunk_iterator<...> // s > 0
  //
  // aligned_chunk_range<FR>
  //   cache: no
  //   iterator category:
  //     conditional<is_rrng<FR>, random_access_iterator_tag, rng_ctg<FR>>
  //       = fitr - ritr
  //   reference:
  //     decltype(rng(fr_i, fr_dif))
  //     decltype(rng(cfr_i, cfr_dif)) if is_rng<const FR>
  //   special member functions: dependent
  //   this_t(FR, rng_dft<FR>) // s > 0 && size(r) % s == 0
  // aligned_chunk_rng(r, s)->aligned_chunk_range<...>
  //   // s > 0 && size(r) % s == 0

  ez_forward_list<int> l = {1, 2, 3, 4};
  assert(equal(rng(aligned_chunk_itr(begin(l), 2),
                   aligned_chunk_itr(end(l), 2)),
               seq(seq(1, 2), seq(3, 4)), equal));

  assert(equal(aligned_chunk_rng(seq(1, 2, 3, 4), 2),
               seq(ez_vector{1, 2}, ez_vector{3, 4}),
               equal));
  assert(equal(aligned_chunk_rng(seq(1, 2, 3), 3), seq(seq(1, 2, 3)), equal));
  assert(equal(aligned_chunk_rng(empty_rng<int>(), 4),
               empty_rng<empty_range<int>>(),
               equal));
}
void doc_chunk_range() {
  // chunk(r, s, oi)->oi_itr // s > 0
  // chunk_plus(r, s, oi)->pair<r_itr, oi_itr> // s > 0
  //
  // chunk_iterator<FI>
  //   iterator category:
  //     conditional<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = fitr - ritr
  //   reference: iter_pair<FI>
  //   chunk_iterator(i1, i2, ed, s, m) // s > 0, m is valid
  //   base()->iter_pair<FI>
  //   stride()->itr_dft<FI>
  // chunk_itr(i1, i2, ed, s, m)->chunk_iterator<...> // s > 0, m is valid
  //
  // chunk_range<FR>
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category:
  //     conditional<is_rrng<FR>, random_access_iterator_tag, rng_ctg<FR>>
  //       = fitr - ritr
  //   reference:
  //     iter_pair<rng_itr<FR>>
  //     iter_pair<rng_itr<const FR>> if is_rng_ref<const FR>
  //   special member functions: dependent
  //   chunk_range(FR, rng_dft<FR> s) // s > 0
  // chunk_rng(fr, s)->chunk_range<...> // s > 0

  assert(equal(chunk_rng(seq(1, 2, 3, 4, 5), 2),
               ez_vector{ez_vector{1, 2}, ez_vector{3, 4}, ez_vector{5}},
               equal));

  int a[] = {1, 2, 3, 4, 5};
  test_ritr(chunk_itr(nth(a, 0), nth(a, 2), end(a), 2, 0),
            chunk_itr(end(a), end(a), end(a), 2, 1),
            ez_vector{ez_vector{1, 2}, ez_vector{3, 4}, ez_vector{5}},
            equal);

  ez_forward_list<int> l = {1, 2, 3, 4, 5};
  test_fitr(chunk_itr(nth(l, 0), nth(l, 2), end(l), 2, 0),
            chunk_itr(end(l), end(l), end(l), 2, 0),
            ez_vector{ez_vector{1, 2}, ez_vector{3, 4}, ez_vector{5}},
            equal);
}
void doc_chunk_by_range() {
  // chunk_by(fr, f, oi)->oi_itr
  // chunk_by_plus(fr, f, oi)->pair<fr_itr, oi_itr>
  //
  // chunk_by_iterator<FI, F>
  //   iterator category:
  //     conditional<is_bitr<FI>,
  //                 bidirectional_iterator_tag, forward_iterator_tag>
  //       = fitr - bitr
  //   reference: iter_pair<FI>
  //   chunk_by_iterator(FI i, FI i2, FI op, FI ed, F f)
  //   base()->iter_pair<FI>
  // chunk_by_itr(i, i2, op, ed, f)->chunk_by_iterator<...>
  //
  // chunk_by_range<FR, EQ>
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category:
  //     conditional<is_brng<FR>,
  //                 bidirectional_iterator_tag, forward_iterator_tag>
  //       = fitr - bitr
  //   reference:
  //     iter_pair<rng_itr<FR>>
  //     iter_pair<rng_itr<const FR>> if is_rng_ref<const FR>
  //   special member functions: dependent
  //   this_t(FR, EQ)
  // chunk_by_rng(fr, f)->chunk_by_range<...>

  ez_vector<ez_vector<int>> v;
  chunk_by(seq(1, 2, 3, 4, 4, 5), less{},
           output_itr([&v](auto x) {
             v.insert(v.end(), {});
             auto &vv = back(v);
             for (int u : x)
               vv.insert(vv.end(), u);
           }));
  assert(v == ez_vector<ez_vector<int>>({{1, 2, 3, 4}, {4, 5}}));

  int a[] = {1, 2, 2, 3};
  test_bitr(chunk_by_itr(nth(a, 0), nth(a, 2), begin(a), end(a), less{}),
            chunk_by_itr(end(a), end(a), begin(a), end(a), less{}),
            seq(seq(1, 2), seq(2, 3)), equal);

  assert(equal(chunk_by_rng(seq(1, 2, 2, 3), less{}),
               seq(seq(1, 2), seq(2, 3)), equal));
}
void doc_inner_cartesian_product_range() {
  // inner_cartesian_product(fr, oi)->oi_itr // *oi = tmp_range_of_refs
  // inner_cartesian_product_plus(fr, oi)->pair<fr_itr, oi_itr>
  //
  // inner_cartesian_product_range<FR>
  //   cache: yes, delay
  //   iterator category: iitr
  //   reference:
  //     decltype(deref_rng(declval<unique_array<rng_itr<rng_ref<FR>> &>()))
  //   special member functions: dependent
  //   explicit this_t(FR)
  // inner_cartesian_product_rng(r)->inner_cartesian_product_range<...>

  ez_vector v = {ez_vector{1, 2, 3}, ez_vector{4, 5}, ez_vector{6, 7}};
  decltype(v) v2;
  inner_cartesian_product(v,
                          output_itr([&](auto &&r) {
                            v2.insert(v2.end(), {});
                            auto &x = back(v2);
                            for (int &u : r)
                              x.insert(x.end(), u);
                          }));
  assert(equal(v2,
               seq(seq(1, 4, 6), seq(2, 4, 6), seq(3, 4, 6),
                   seq(1, 5, 6), seq(2, 5, 6), seq(3, 5, 6),
                   seq(1, 4, 7), seq(2, 4, 7), seq(3, 4, 7),
                   seq(1, 5, 7), seq(2, 5, 7), seq(3, 5, 7)),
               equal));

  assert(equal(inner_cartesian_product_rng(seq(seq(1, 2),
                                               seq(3, 4),
                                               seq(5, 6))),
               seq(seq(1, 3, 5), seq(2, 3, 5),
                   seq(1, 4, 5), seq(2, 4, 5),
                   seq(1, 3, 6), seq(2, 3, 6),
                   seq(1, 4, 6), seq(2, 4, 6)),
               equal));
}
void doc_cartesian_product_range() {
  // cartesian_product(r, s..., oi)->oi_itr // *oi = tmp_range_of_refs
  //
  // cartesian_product_iterator<IT, S...>
  //   iterator category: {min category of all iterators} = fitr - ritr
  //   reference: tuple<itr_ref<IT>, itr_ref<S>...>
  //   explicit cartesian_product_iterator(tuple<array<IT, 3>, array<S, 3>...>)
  //   base()->tuple<array<IT, 3>, array<S, 3>...>
  // cartesian_product_itr(tuple<array<IT, 3>, array<S, 3>...>)
  //   ->cartesian_product_iterator<IT, S...>
  //
  // cartesian_product_range<FR, S...>
  //   cache: no
  //   iterator category: {min iterator category of all range} = fitr - ritr
  //   reference:
  //     tuple<rng_ref<FR>, rng_ref<S>...>
  //     tuple<rng_ref<const FR>, rng_ref<const S>...>
  //       if ((is_rng<add_const<FR>> && (... && is_rng<add_const<S>>)))
  //   special member functions: dependent
  //   explicit(sizeof...(S) == 0) this_t(FR, S...)
  // cartesian_product_rng(fr, s...)->cartesian_product_range<...>

  ez_vector<tuple<int, int, int>> v;
  cartesian_product(seq(1, 2, 3), seq(3, 4), seq(5, 6),
                    output_itr([&](tuple<int &, int &, int &> x) {
                      v.insert(v.end(), {});
                      auto &t_ref = back(v);
                      t_ref = x;
                    }));
  assert(equal(v, seq(tuple(1, 3, 5), tuple(1, 3, 6),
                      tuple(1, 4, 5), tuple(1, 4, 6),
                      tuple(2, 3, 5), tuple(2, 3, 6),
                      tuple(2, 4, 5), tuple(2, 4, 6),
                      tuple(3, 3, 5), tuple(3, 3, 6),
                      tuple(3, 4, 5), tuple(3, 4, 6))));

  ez_forward_list<int> l1 = {1, 2};
  ez_forward_list<int> l2 = {3, 4};
  assert(equal(rng(cartesian_product_itr
                   (tuple(seq(begin(l1), begin(l1), end(l1)),
                          seq(begin(l2), begin(l2), end(l2)))),
                   cartesian_product_itr
                   (tuple(seq(end(l1), begin(l1), end(l1)),
                          seq(end(l2), begin(l2), end(l2))))),
               seq(tuple(1, 3), tuple(1, 4), tuple(2, 3), tuple(2, 4))));
  int a[] = {1, 2};
  int b[] = {3, 4};
  assert(equal(rng(cartesian_product_itr
                   (tuple(seq(begin(a), begin(a), end(a)),
                          seq(begin(b), begin(b), end(b)))),
                   cartesian_product_itr
                   (tuple(seq(nth(a, 1), begin(a), end(a)),
                          seq(end(b), begin(b), end(b))))),
               seq(tuple(1, 3), tuple(1, 4), tuple(2, 3), tuple(2, 4))));

  assert(equal(cartesian_product_rng(seq(1, 2), seq(3, 4), seq(5, 6)),
               seq(tuple(1, 3, 5), tuple(1, 3, 6),
                   tuple(1, 4, 5), tuple(1, 4, 6),
                   tuple(2, 3, 5), tuple(2, 3, 6),
                   tuple(2, 4, 5), tuple(2, 4, 6))));
}
void doc_split_range() {
  // split(fr, f, oi)->oi_itr // *o = iter_pair
  // split_plus(fr, f, oi)->pair<fr_itr, oi_itr>
  //
  // split_iterator<FI>
  //   iterator category: fitr
  //   reference: iter_pair<FI>
  //   split_iterator(FI i, FI i2, FI ed, itr_dft<FI> k)
  //   base()->iter_pair<FI>
  // split_itr(i, i2, ed, k)->split_iterator<...>
  //
  // split_range<FR, FR2>
  //   cache: yes, delay if !(is_rng_ref<FR> && is_rng_ref<FR2>)
  //   iterator category: fitr
  //   reference:
  //     iter_pair<rng_itr<FR>>
  //     iter_pair<rng_itr<const FR>> if is_rng_ref<FR> && is_rng_ref<FR2>
  //   special member functions: dependent
  //   this_t(FR, FR2)
  // split_rng(fr, fr2)->split_range<...>

  ez_vector<ez_vector<int>> v;
  split(seq(1, 2, 0, 3, 4, 0),
        seq(0),
        output_itr([&](auto p) {
          v.insert(v.end(), {});
          auto &vv = back(v);
          for (int &x : p)
            vv.insert(vv.end(), x);
        }));
  assert(equal(v,
               seq(ez_vector{1, 2}, ez_vector{3, 4}, ez_vector<int>{}),
               equal));

  int a[] = {1, 2, 0, 3, 4};
  assert(equal(rng(split_itr(nth(a, 0), nth(a, 2), end(a), 1),
                   split_itr(end(a), end(a), end(a), 0)),
               seq(seq(1, 2), seq(3, 4)), equal));
  int a2[] = {1, 2, 0};
  assert(equal(rng(split_itr(nth(a2, 0), nth(a2, 2), end(a2), 1),
                   split_itr(end(a2), end(a2), end(a2), 0)),
               ez_vector{ez_vector<int>{1, 2}, ez_vector<int>{}}, equal));
  int a3[] = {1, 2};
  assert(equal(rng(split_itr(nth(a3, 0), nth(a3, 2), end(a3), 0),
                   split_itr(end(a3), end(a3), end(a3), 0)),
               ez_vector<ez_vector<int>>{{1, 2}}, equal));

  assert(equal(split_rng(seq(1, 2, 0, 3, 4), seq(0)),
               seq(seq(1, 2), seq(3, 4)),
               equal));
  assert(equal(split_rng(seq(1, 2, 0, 3, 4, 0), seq(0)),
               seq(ez_vector{1, 2}, ez_vector{3, 4}, ez_vector<int>{}),
               equal));
}
void doc_zip_range() {
  // zip_top(x, s...)->tuple<iters...>
  // zip(x, s..., oi)->oi_itr
  //
  // zip_iterator<I, S...>
  //   iterator category: fitr if all be fitr, = iitr - fitr
  //   reference: tuple<itr_ref<...>...>
  //   explicit zip_iterator(tuple<I, S...>)
  //   explicit(sizeof...(S) == 0u) zip_iterator(I, S...)
  //   base()->tuple<I, S...>
  // zip_itr(i, s...)->zip_iterator<...>
  // zip_itr_from_tuple(t)->zip_iterator<...>
  //
  // zip_range<X, S...>
  //   cache: no
  //   iterator category: fitr if all be fitr, = iitr - fitr
  //   reference:
  //     tuple<refs...>
  //     tuple<crefs...> // valid const for all args
  //   special member functions: dependent
  //   explicit(sizeof...(S) == 0u) this_t(X, S...)
  // zip_rng(x, s...)->zip_range<...>

  int a[] = {1, 2};
  int b[] = {3, 4, 5};
  int c[] = {6};
  assert(zip_top(a, b, c) == tuple(nth(a, 1), nth(b, 1), nth(c, 1)));
  assert(zip_top(begin(a), begin(b), c)
         == tuple(nth(a, 1), nth(b, 1), nth(c, 1)));
  assert(equal(zip(a, b, c,
                   to_back(ez_vector<tuple<int, int, int>>{})).base(),
               seq(tuple(1, 3, 6))));

  int a1[] = {1, 2, 3};
  int a2[] = {4, 5};
  assert(equal(zip_rng(a1, a2), seq(tuple(1, 4), tuple(2, 5))));
  assert(equal(zip_rng(a2, begin(a1)), seq(tuple(4, 1), tuple(5, 2))));
  assert(equal(zip_rng(begin(a1), a2), seq(tuple(1, 4), tuple(2, 5))));
}
void doc_aligned_zip_range() {
  // // aligned_zip means storing zip_top before range operations
  //
  // aligned_zip_iterator<I, S...>
  //   iterator category: iitr - ritr // note: iitr only for single dimension
  //   reference: tuple<itr_ref<...>...>
  //   explicit aligned_zip_iterator(tuple<I, S...>)
  //   explicit(...) aligned_zip_iterator(I, S...)
  //   base()->tuple<I, S...>
  // aligned_zip_itr(i, s...)->aligned_zip_iterator<...>
  // aligned_zip_itr_from_tuple(t)->aligned_zip_iterator<...>
  //
  // aligned_zip_range<X, S...>
  //   cache: yes, delay if !((is_rng_ref<X> || !is_rng<X>)
  //                          && (... && (is_rng_ref<S> || !is_rng<S>)))
  //   iterator category: iitr - ritr // note: iitr only for single dimension
  //   reference:
  //     tuple<refs...>
  //     tuple<crefs...> // valid const for all args
  //   special member functions: dependent
  //   explicit(sizeof...(S) == 0) this_t(X, S...)
  // aligned_zip_rng(x, s...)->aligned_zip_range<...>

  int a[] = {1, 2};
  int b[] = {1, 2};
  int c[] = {1, 2};
  assert(equal(rng(aligned_zip_itr(begin(a), begin(b), begin(c)),
                   aligned_zip_itr(end(a), end(b), end(c))),
               seq(tuple(1, 1, 1), tuple(2, 2, 2))));

  assert(equal(aligned_zip_rng(iitr(1), seq(1, 2)),
               seq(tuple(1, 1), tuple(2, 2))));
  assert(equal(aligned_zip_rng(seq(1, 2), seq(1, 2, 3, 4), seq(1, 2, 3)),
               seq(tuple(1, 1, 1), tuple(2, 2, 2))));
}
void doc_enumerate_range() {
  // enumerate(r, oi)->oi_itr // *oi = pair<int_t, ref>
  // enumerate_plus(r, oi)->pair<r_itr, oi_itr>
  //
  // enumerate_iterator<I>
  //   iterator category:
  //     conditional<is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>
  //       = iitr - ritr
  //   reference: pair<itr_ref<I>, itr_ref<I>>
  //   enumerate_iterator(I, itr_dft<I>)
  //   base()->I
  //   index()->itr_dft<I>
  // enumerate_itr(i, n)->enumerate_iterator<...>
  //
  // enumerate_range<R>
  //   cache: yes, delay if !is_rng_ref<R> // store the ssize
  //   iterator category:
  //     conditional<is_rrng<R>, random_access_iterator_tag, rng_ctg<R>>
  //       = iitr - ritr
  //   reference:
  //     pair<rng_ref<R>, rng_ref<R>>
  //     pair<rng_ref<const R>, rng_ref<const R>>
  //       if is_rng<const R> && is_rng_ref<R>
  //   special member functions: dependent
  //   explicit this_t(R)
  // enumerate_rng(r)->enumerate_range<...>

  ez_vector<pair<int, int>> v;
  enumerate(seq(1, 3, 10), to_back(v));
  assert(equal(v, seq(pair(0, 1), pair(1, 3), pair(2, 10))));

  assert(equal(rng(enumerate_itr(iitr(1), 0),
                   enumerate_itr(iitr(4), 3)),
               seq(pair(0, 1), pair(1, 2), pair(2, 3))));

  assert(equal(enumerate_rng(seq(3, 7, 9)),
               seq(pair(0, 3), pair(1, 7), pair(2, 9))));
}
void doc_exclusive_rotate_range() {
  // exclusive_rotate(r, i, oi)->oi_itr
  //   // similar to rotate_copy()
  //   //   but exclude the first valid element if non-empty
  {
    const auto r0 = empty_rng<int>();
    assert(equal(exclusive_rotate(r0, end(r0),
                                  to_back(ez_vector<int>{})).base(),
                 empty_rng<int>()));
    const auto r1 = seq(1);
    assert(equal(exclusive_rotate(r1, r1.begin(),
                                  to_back(ez_vector<int>{})).base(),
                 empty_rng<int>()));
    const auto r = seq(1, 2, 3, 4);
    assert(equal(exclusive_rotate(r, r.end(), to_back(ez_vector<int>{})).base(),
                 seq(2, 3, 4)));
    assert(equal(exclusive_rotate(r, nth(r, 3),
                                  to_back(ez_vector<int>{})).base(),
                 seq(1, 2, 3)));
    assert(equal(exclusive_rotate(r, nth(r, 2),
                                  to_back(ez_vector<int>{})).base(),
                 seq(4, 1, 2)));
    assert(equal(exclusive_rotate(r, nth(r, 0),
                                  to_back(ez_vector<int>{})).base(),
                 seq(2, 3, 4)));
  }

  // exclusive_rotate_iterator<FI>
  //   iterator category:
  //     conditional<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = fitr - ritr
  //   reference: itr_ref<FI>
  //   exclusive_rotate_iterator(FI op, FI wall, FI ed, FI it)
  //     // wall != l || (wall == l && f == l)
  //     // for the last iterator of a non-empty range,
  //     //   it is prev(wall) if wall != op, otherwise prev(ed)
  //   base()->FI
  // exclusive_rotate_itr(FI op, FI wall, FI ed, FI it)
  //   ->exclusive_rotate_iterator<FI>
  {
    int a[] = {1, 2, 3, 4};
    const auto i1 = exclusive_rotate_itr(begin(a), nth(a, 3), end(a),
                                         nth(a, 3));
    const auto i2 = exclusive_rotate_itr(begin(a), nth(a, 3), end(a),
                                         nth(a, 2));
    assert(equal(rng(i1, i2), seq(4, 1, 2)));
  }

  // exclusive_rotate_range<FR>
  //   // exclusive: exclusive of the first element
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category:
  //     conditional<is_rrng<FR>, random_access_iterator_tag, rng_ctg<FR>>
  //       = fitr - ritr
  //   reference:
  //     rng_ref<FR>
  //     rng_ref<const FR> if is_rng<const FR> && is_rng_ref<FR>
  //   special member functions: dependent
  //   this_t(FR, rng_itr<FR>) requires is_rng_ref<FR>
  //   this_t(FR, rng_dft<FR>)
  // exclusive_rotate_rng(r, it)->exclusive_rotate_range<...>
  // exclusive_rotate_rng(r, d)->exclusive_rotate_range<...>
  {
    assert(equal(exclusive_rotate_rng(empty_rng<int>(), 0),
                 empty_rng<int>()));
    assert(equal(exclusive_rotate_rng(seq(1), 0), empty_rng<int>()));
    assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 4), seq(2, 3)));
    assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 2), seq(1, 2)));
    assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 1), seq(3, 1)));
    assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 0), seq(2, 3)));
  }
}
void doc_rotate_range() {
  // rotate_iterator<FI>
  //   iterator category:
  //     conditional<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = fitr - ritr
  //   reference: itr_ref<FI>
  //   rotate_iterator(FI op, FI wall, FI ed, FI it)
  //     // wall != l || (wall == l && f == l)
  //     // for the last iterator of a non-empty range,
  //     //   it == ed
  //   base()->FI
  // rotate_itr(op, wall, ed, it)->rotate_iterator<FI>
  {
    int a[] = {1, 2, 3, 4, 5};
    assert(equal(rng(rotate_itr(begin(a), nth(a, 2), end(a), nth(a, 2)),
                     rotate_itr(begin(a), nth(a, 2), end(a), end(a))),
                 seq(3, 4, 5,
                     1, 2)));
  }

  // rotate_range<FR>
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category:
  //     conditional<is_rrng<FR>, random_access_iterator_tag, rng_ctg<FR>>
  //       = fitr - ritr
  //   reference:
  //     rng_ref<FR>
  //     rng_ref<const FR> if is_rng<const FR> && is_rng_ref<FR>
  //   special member functions: dependent
  //   this_t(FR, rng_itr<FR>) requires is_rng_ref<FR>
  //   this_t(FR, rng_dft<FR>)
  // rotate_rng(r, it)->rotate_range<...>
  // rotate_rng(r, d)->rotate_range<...>

  assert(equal(rotate_rng(empty_rng<int>(), 0), empty_rng<int>()));
  assert(equal(rotate_rng(seq(1, 2, 3), 4), seq(1, 2, 3)));
  assert(equal(rotate_rng(seq(1, 2, 3), 1), seq(2, 3, 1)));
}
void doc_loop_range() {
  // loop(fr, fr_i, fr_dif, oi)->oi_itr
  {
    const int a[] = {1, 2, 3, 4};
    assert(equal(loop(a, nth(a, 2), 9, to_back(vector<int>{})).base(),
                 seq(3, 4,
                     1, 2, 3, 4,
                     1, 2, 3)));
  }
  
  // loop_iterator<FI>
  //   iterator category:
  //     conditional<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>
  //       = fitr - ritr
  //   reference: itr_ref<FI>
  //   loop_iterator(FI r_begin, FI r_end, FI it, itr_dft<FI> n)
  //   base()->FI
  //   index()->itr_dft<FI>
  // loop_itr(r_begin, r_end, it, n)->loop_iterator<...>
  {
    const int a[] = {1, 2, 3, 4};
    assert(equal(rng(loop_itr(begin(a), end(a), nth(a, 2), 0),
                     loop_itr(begin(a), end(a), nth(a, 2), 9)),
                 seq(3, 4,
                     1, 2, 3, 4,
                     1, 2, 3)));
  }

  // loop_range<FR>
  //   cache: yes, delay if !is_rng_ref<FR>
  //   iterator category:
  //     conditional<is_rrng<FR>, random_access_iterator_tag, rng_ctg<FR>>
  //       = fitr - ritr
  //     // different elements may point to the same underlying element
  //     //   because of loop
  //   reference:
  //     rng_ref<FR>
  //     rng_ref<const FR> if is_rng<const FR> && is_rng_ref<FR>
  //   special member functions: dependent
  //   this_t(FR, rng_itr<FR>, rng_dft<FR>)
  //   this_t(FR, rng_dft<FR>, rng_dft<FR>)
  // loop_rng(fr, i, n)->loop_range<...>
  // loop_rng(fr, n0, n)->loop_range<...>
  // loop_rng(fr, n)->loop_range<...> // same as loop_rng(fr, 0, n)
  {
    assert(loop_rng(empty_rng<int>(), 3).empty());
    assert(equal(loop_rng(seq(1, 2), 3), seq(1, 2, 1)));
    assert(equal(loop_rng(seq(1, 2), 1, 3), seq(2, 1, 2)));
  }
}
void doc_concat_range() {
  // concat_range<R, S...>
  //   cache: yes, delay if !(is_rng_ref<R> && is_rng_ref<S>...)
  //   iterator category:
  //     common_iterator_category<rng_ctg<R>, rng_ctg<S>...> = iitr - ritr
  //   reference:
  //     common_reference<rng_ref<R>, rng_ref<S>...>
  //     common_reference<rng_ref<const R>, rng_ref<const S>...>
  //       if is_rng<const R> && (... && is_rng<const S>)
  //   special member functions: dependent
  //   explicit(sizeof...(S) == 0u) concat_range(R, S...)
  // concat_rng(r, s...)->concat_range<...>

  assert(equal(concat_rng(ez_list<int>{},
                          seq(1, 2),
                          ez_forward_list<int>{3, 4, 5},
                          ez_vector<int>{}),
               seq(1, 2, 3, 4, 5)));
}

// container
void doc_string() {
  // from_unicode_t
  // from_unicode
  //
  // basic_string<T, AL = default_allocator<T>>
  // sso_string<T, size_t N, AL = default_allocator<T>>
  //   pointer = alloc_ptr<AL>
  //   const_pointer = alloc_cptr<AL>
  //   value_type = T
  //   reference = T &
  //   const_reference = const T &
  //   iterator = pointer
  //   const_iterator = const_pointer
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //   max_size()->size_type
  //   size()->size_type
  //   empty()->bool
  //
  //   special member functions: full // may throw
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()->(const_)reverse_iterator
  //   rend()->(const_)reverse_iterator
  //   crbegin()->const_reverse_iterator
  //   crend()->const_reverse_iterator
  //
  //   allocator_type = AL
  //   get_allocator()->allocator_type
  //
  //   this_t(const allocator_type &)
  //   this_t(const this_t &, const allocator_type &) // may throw
  //   this_t(this_t &&, const allocator_type &) // may throw
  //
  //   explicit this_t(size_type,
  //                   const allocator_type & = allocator_type{}) // may throw
  //   this_t(size_type, value_type,
  //          const allocator_type & = allocator_type{}) // may throw
  //   assign(size_type, value_type) // may throw
  //
  //   this_t(begin_iitr, end_iitr,
  //          const allocator_type & = allocator_type{}) // may throw
  //   assign(begin_iitr, end_iitr) // may throw
  //
  //   this_t(initializer_list<value_type>,
  //          const allocator_type & = allocator_type{}) // may throw
  //   =(initializer_list<value_type>)->this_t & // may throw
  //   assign(initializer_list<value_type>) // may throw
  //
  //   insert(const_iterator, value_type)->iterator // may throw
  //   insert(const_iterator, size_type, value_type)->iterator // may throw
  //   insert(const_iterator, begin_iitr, end_iitr)->iterator // may throw
  //   insert(const_iterator, initializer_list<value_type>)->iterator
  //     // may throw
  //   erase(const_iteartor)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   clear()
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //   push_back(value_type) // may throw
  //   pop_back()
  //   [](size_type)->reference
  //   at(size_type)->(const_)reference // may throw out_of_range
  //
  //   local()->bool // only for sso_string
  //   data()->(const) value_type *
  //   capacity()->size_type
  //   full()->bool
  //   resize(size_type, value_type) // may throw
  //   resize(size_type) // may throw
  //   reserve(size_type) // may throw
  //   reserve_more(size_type) // may throw
  //   reallocate(size_type = size()) // may throw
  //   shrink_to_fit() // may throw
  //   remove_if(eq)->size_type // may throw // eq(element_ref)
  //   remove(const X &)->size_type // may throw
  //   replace(const_iterator, const_iterator, r)->iterator // may throw
  //
  //   this_t(from_unicode, r) // may throw
  //   try_assign_unicode(r)->bool
  //   assign_unicode(r) // throw if !try_assign_unicode(s)
  //
  //   this_t(from_range, r,
  //          const allocator_type & = allocator_type{}) // may throw
  //   this_t(array_of_the_same_value_type) // may throw
  //   explicit this_t(r) // may throw
  //   this_t(r, const allocator_type & = allocator_type{}) // may throw
  //   =(r) // may throw
  //   assign(r) // may throw
  //   assign_range(r) // may throw
  //   insert(const_iterator, r)->iterator // may throw
  //   insert_range(const_iterator, r)->iterator // may throw
  //   push_back(r) // may throw
  //   append_range(r) // may throw
  //   pop_back(size_type n) // n <= size()
  //   append(s...)->this_t & // may throw
  //
  // using string = sso_string<char, 15u>;
  // template <class C, class AL = default_allocator<C>>
  // using default_sso_string = sso_string<C, 15u, AL>;
  // using wstring = basic_string<wchar_t>;
  // using u16string = basic_string<char16_t>;
  // using u32string = basic_string<char32_t>;
  // template <class T>
  // using string_t = ... // select sso_string or basic_string
  //
  // string operator ""_s(const char *s, size_t n);
  // wstring operator ""_s(const wchar_t *s, size_t n);
  // u16string operator ""_s(const char16_t *s, size_t n);
  // u32string operator ""_s(const char32_t *s, size_t n);
  //
  // less<basic_string<T, AL>>
  //   is_transparent = ...
  //   ()(const basic_string<T, AL> &, basic_string<T, AL> &)->bool
  //   ()(string_reference<const T>, string_reference<const T>)->bool
  //   ()(r1, r2)->bool // at least one of r1-and-r2 is array
  // less<sso_string<T, N, AL>>
  //   is_transparent = ...
  //   ()(const sso_string<T, N, AL> &, sso_string<T, N, AL> &)->bool
  //   ()(string_reference<const T>, string_reference<const T>)->bool
  //   ()(r1, r2)->bool // at least one of r1-and-r2 is array
  //
  // equal_to<basic_string<T, AL>>
  //   is_transparent = ...
  //   ()(const basic_string<T, AL> &, basic_string<T, AL> &)->bool
  //   ()(string_reference<const T>, string_reference<const T>)->bool
  //   ()(r1, r2)->bool // at least one of r1-and-r2 is array
  // equal_to<sso_string<T, N, AL>>
  //   is_transparent = ...
  //   ()(const sso_string<T, N, AL> &, sso_string<T, N, AL> &)->bool
  //   ()(string_reference<const T>, string_reference<const T>)->bool
  //   ()(r1, r2)->bool // at least one of r1-and-r2 is array
  //
  // hash<basic_string<T, AL>>
  //   is_transparent = ...
  //   argument_type = basic_string<T, AL>
  //   result_type = size_t
  //   ()(string_reference<const T>)->size_t
  // hash<sso_string<T, N, AL>>
  //   is_transparent = ...
  //   argument_type = sso_string<T, N, AL>
  //   result_type = size_t
  //   ()(string_reference<const T>)->size_t
}
void doc_string_reference() {
  // string_reference<T>
  //   pointer = T *
  //   const_pointer = const T *
  //   value_type = remove_cv<T> // T have no reference qualifier
  //   reference = T &
  //   const_reference = const T &
  //   iterator = T *
  //   const_iterator = const T *
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   string_reference(nullptr)
  //   =(nullptr)->this_t &
  //   clear() // call *this = nullptr
  //
  //   string_reference(char_ref)
  //   string_reference(r)
  //   string_reference(begin_iitr, end_iitr)
  //   string_reference(r, n, len)
  //   string_reference(const string_reference<const T> &) // cast ref to view
  //
  //   string_reference(pointer) // from c string
  //   string_reference(remove_const<T> *) requires is_const<T>
  //     // from c string
  //
  //   special member functions: full
  //   == <=> between two string_reference instances with the same value_type
  //   == <=> between built-in array and string_reference
  //   == <=> between basic_string and string_reference
  //   == <=> between sso_string and string_reference
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //   size()->size_type
  //   empty()->bool
  //   data()->pointer
  //   [](size_type)->(const_)reference
  //   at(size_type)->(const_)reference // may throw out_of_range
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()->(const_)reverse_iterator
  //   rend()->(const_)reverse_iterator
  //   crbegin()->const_reverse_iterator
  //   crend()->const_reverse_iterator
  //
  // template <class T>
  // using string_view = string_reference<const T>;
  // using sref = string_reference<char>;
  // using sview = string_reference<const char>;
  // using wsref = string_reference<wchar_t>;
  // using wsview = string_reference<const wchar_t>;
  // using u16sref = string_reference<char16_t>;
  // using u16sview = string_reference<const char16_t>;
  // using u32sref = string_reference<char32_t>;
  // using u32sview = string_reference<const char32_t>;
  //
  // ""_sv(const char *s, size_t n)->sview
  // ""_sv(const wchar_t *s, size_t n)->wsview
  // ""_sv(const char16_t *s, size_t n)->u16sview
  // ""_sv(const char32_t *s, size_t n)->u32sview
  //
  // less<string_reference<T>>
  //   is_transparent = ...
  //   ()(const string_reference<T> &, const string_reference<T> &)->bool
  // equal_to<string_reference<T>>
  //   is_transparent = ...
  //   ()(const string_reference<T> &, const string_reference<T> &)->bool
  // hash<string_reference<T>>
  //   is_transparent = ...
  //   argument_type = string_reference<T>
  //   result_type = size_t
  //   ()(string_reference<T>)->size_t
}
void doc_bitset() {
  // bitset<N>
  //   special member functions: full
  //   == <=>
  //
  //   bitset(unsigned long long)
  //     // make high-offset element by high bit, the part out of N is ignored
  //   operator =(unsigned long long)
  //     // make high-offset element by high bit, the part out of N is ignored
  //
  //   explicit bitset(const STR &,
  //                   STR::size_type pos = 0,
  //                   STR::size_type n = -1,
  //                   STR::value_type zero = STR::value_type('0'),
  //                   STR::value_type one = STR::value_type('1'))
  //     // may throw invalid_argument if not zero nor one
  //   explicit bitset(const C *, size_t n = -1,
  //                   C zero = C('0'), C one = C('1'))
  //     // may throw invalid_argument if not zero nor one
  //
  //   to_ullong()->unsigned long long // may throw overflow_error
  //   to_ulong()->unsigned long // may throw overflow_error
  //   to<STR>(STR::value_type zero = '0', STR::value_type one = '1')->STR
  //   to_string(char zero = '0', char one = '1')->string
  //   sprint(char zero = '0', char one = '1')->string
  //     // to_string(): high-offset element in left
  //     // sprint(): high-offset element in right
  //
  //   reference
  //     special member functions:
  //       no default-constructor
  //       move is just copy, work like pointer (copy the stored address)
  //     operator bool()
  //     ~()->bool
  //     flip()->this_t &
  //   const_reference = bool
  //   pointer = reference *
  //   const_pointer = const reference *
  //   value_type = bool
  //   iterator
  //   const_iterator
  //   difference_type = ptrdiff_t
  //   size_type = size_t
  //
  //   empty()->bool
  //   size()->size_t
  //   max_size()->size_t
  //
  //   [](size_t)->reference // for non-const
  //   [](size_t)->bool // for const
  //   at(size_t)->reference // for non-const // may throw out_of_range
  //   at(size_t)->bool // for const // may throw out_of_range
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()->(const_)reverse_iterator
  //   rend()->(const_)reverse_iterator
  //   crbegin()->const_reverse_iterator
  //   crend()->const_reverse_iterator
  //
  //   set()->this_t &
  //   set(size_t, bool)->this_t & // may throw out_of_range
  //   reset()->this_t &
  //   reset(size_t)->this_t & // may throw out_of_range
  //
  //   count()->size_t
  //   ~()->this_t
  //   flip()->this_t &
  //   flip(size_t)->this_t // may throw out_of_range
  //   test(size_t)->bool // may throw out_of_range
  //   all()->bool
  //   any()->bool
  //   none()->bool
  //
  //   &=(const this_t &)->this_t &
  //   |=(const this_t &)->this_t &
  //   ^=(const this_t &)->this_t &
  //   <<=(size_t)->this_t &
  //   >>=(size_t)->this_t &
  //   <<(size_t)->this_t
  //   >>(size_t)->this_t
  //
  // hash<bitset<N>>

  bitset<4> s(0b1011u);
  assert(equal(s, seq(true, true, false, true)));
  assert(s.to_string() == "1011");
  assert(s.sprint() == "1101");
  bitset<4> s2(0b1001011u);
  assert(s2.to_string() == "1011");

  bitset<4> s3("1011"_sv);
  assert(s3.to_string() == "1011");
  bitset<4> s4("10"_sv);
  assert(s4.to_string() == "0010");
  bitset<4> s5("10110001"_sv);
  assert(s5.to_string() == "1011");

  bitset<4> s6;
  copy(seq(1, 1, 0, 1), s6.begin());
  assert(equal(s6, seq(1, 1, 0, 1)));
  assert(s6.sprint() == "1101");
  assert(s6.to_string() == "1011");
}
void doc_vector() {
  // vector<T, AL = default_allocator<T>>
  //   pointer
  //   const_pointer
  //
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type = AL
  //   get_allocator()
  //   vector(alloc)
  //   vector(const vector &, alloc)
  //   vector(vector &&, alloc)
  //   explicit vector(n, alloc = AL{})
  //   vector(n, x, alloc = AL{})
  //   assign(n, x)
  //   vector(from, to, alloc = AL{})
  //   assign(from, to)
  //   vector(il, alloc = AL{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   data()->(const) value_type *
  //   capacity()->size_type
  //   full()->bool
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   vector(from_range, r, alloc = AL{})
  //   explicit vector(r)
  //   vector(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   append_range(r)
  //   pop_back(size_type n)
  //   append(s...)->this_t &
}
void doc_bool_vector() {
  // vector<bool, AL = default_allocator<bool>>
  //   pointer = void
  //   const_pointer = void
  //   value_type = bool
  //   reference = some class
  //   const_reference = bool
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   vector(alloc)
  //   vector(const vector &, alloc)
  //   vector(vector &&, alloc)
  //   explicit vector(n, alloc = AL{})
  //   vector(n, x, alloc = AL{})
  //   assign(n, x)
  //   vector(from, to, alloc = AL{})
  //   assign(from, to)
  //   vector(il, alloc = AL{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   capacity()->size_type
  //   full()->bool
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   flip()
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   vector(from_range, r, alloc = AL{})
  //   explicit vector(r)
  //   vector(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   append_range(r)
  //   pop_back(size_type n)
  //   append(s...)->this_t &
  //
  // hash<vector<bool, AL>>
}
void doc_local_vector() {
  // local_vector<T, size_t N>
  //   // no check if more than N elements are inserted
  //   pointer = T *
  //   const_pointer = const T *
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   local_vector(n)
  //   local_vector(n, x)
  //   assign(n, x)
  //   local_vector(from, to)
  //   assign(from, to)
  //   local_vector(il)
  //   operator =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   data()->(const) value_type *
  //   capacity()->size_type
  //   resize(n)
  //   resize(n, x)
  //   full()->bool
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   local_vector(from_range, r)
  //   explicit local_vector(r)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   append_range(r)
  //   pop_back(size_type n)
  //   append(s...)->this_t &
}
void doc_small_vector() {
  // small_small_vector<T, size_t N, AL = default_allocator<T>>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   small_vector(alloc)
  //   small_vector(const small_vector &, alloc)
  //   small_vector(small_vector &&, alloc)
  //   explicit small_vector(n, alloc = {})
  //   small_vector(n, x, alloc = {})
  //   assign(n, x)
  //   small_vector(from, to, alloc = {})
  //   assign(from, to)
  //   small_vector(il, alloc = {})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   local_buffer_size()->size_t
  //   local()->bool
  //   data()->(const) value_type *
  //   capacity()->size_type
  //   full()->bool
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reserve_more(n)
  //   reallocate(n = size())
  //   shrink_to_fit()
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   small_vector(from_range, r, alloc = AL{})
  //   explicit small_vector(r)
  //   small_vector(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   append_range(r)
  //   pop_back(size_type n)
  //   append(s...)->this_t &

  small_vector<int, 3, test_allocator<int>> v;
  assert(v.capacity() == 3);
}
void doc_pointer_vector() {
  // has_allocator_type<T>
  // conditional_allocator_type<T>
  //   allocator_type = T::allocator_type
  //     // if !has_allocator_type<T>, there is no allocator_type
  {
    struct t {
      using allocator_type = int;
    };
    static_assert(has_allocator_type<t>);
    static_assert(!has_allocator_type<int>);
  }
  // pointer_vector<T, V = vector<T *>>
  //   pointer = V::value_type
  //   const_pointer = ptr_rebind<pointer, const T>
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //
  //   new_node(s...)->pointer
  //   delete_node(pointer)
  //   exchange_node(cit, pointer)->pointer
  //   link(cit, pointer)->pointer
  //   link_front(pointer)->pointer
  //   link_back(pointer)->pointer
  //   unlink(cit)->pointer
  //   unlink(from, to)
  //   unlink()
  //   unlink_front()->iterator
  //   unlink_back()->iterator
  //
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type = V::allocator_type // exists if V has allocator_type
  //   get_allocator() // return V::get_allocator()
  //   explicit pointer_vector(a)
  //   pointer_vector(const pointer_vector &, a)
  //   pointer_vector(pointer_vector &&, a)
  //
  //   explicit pointer_vector(n, a = a_t{})
  //   pointer_vector(n, x, a = a_t{})
  //   assign(n, x)
  //   pointer_vector(from, to, a = a_t{})
  //   assign(from, to)
  //
  //   pointer_vector(il, a = a_t{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   [](n)
  //   at(n)
  //
  //   capacity()->size_type
  //   resize(n)
  //   resize(n, x)
  //   reserve(n)
  //   shrink_to_fit()
  //
  //   splice(next, this_ref) // no overlap
  //   splice(next, this_ref, i) // no overlap
  //   splice(next, this_ref, from, to) // no overlap
  //   swap(cit, cit)
  //   swap(cit, this_ref, cit)
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   unique(eqf = equal_to{})
  //   merge(this_ref, less_f = less{})
  //   reverse()
  //   sort(less_f = less{})
  //
  //   full()->bool
  //   reserve_more(n)
  //   reallocate(n = size())
  //   replace(i1, i2, r)->iterator // may throw
  //
  //   pointer_vector(from_range, r, a = a_t{})
  //   explicit pointer_vector(r)
  //   pointer_vector(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(size_type n)
  //   pop_front(size_type n)
  //   append(s...)->this_t &
  //   prepend(s...)->this_t &
  //
  //   node_type = allocator_wrapper<allocator_type>::unique_ptr
  //   make_node(s...)->node_type
  //   extract(const_iterator)->node_type
  //   insert(const_iterator, node_type &&)->iterator
  //   push_front(const_iterator, node_type &&)
  //   push_back(const_iterator, node_type &&)
  //   exchange(const_iterator, node_type &)->node_type
  //   exchange(const_iterator, node_type &&)->node_type
  //   replace(const_iterator, node_type &&)->iterator
  //
  //   extract(const_iterator, const_iterator)->this_t
  //   insert(const_iterator, this_t &&)->iterator // no overlap
  //   push_front(const_iterator, this_t &&) // no overlap
  //   push_back(const_iterator, this_t &&) // no overlap
  //   exchange(const_iterator, const_iterator, this_t &)->this_t // no overlap
  //   exchange(const_iterator, const_iterator, this_t &&)->this_t // no overlap
  //   replace(const_iterator, const_iterator, this_t &&)->iterator
  //     // no overlap
}
void doc_headed_vector() {
  // non_constructible_t
  //   // no contents but a disabled default-constructor
  //   special member functions: no default-constructor
  //
  // headed_vector<H, T>
  //   H h;
  //   vector<T> vec;
  //   operator headed_vector<H2, T2>()
  //     // if T is non_constructible_t,
  //     //   then any headed_vector<H2, T2> can be casted
  //
  // headed_vector_maker<H>
  //   H h
  //   ()(s...)->headed_vector<H, decay<first_s_t>>
  //   ()()->headed_vector<H, non_constructible_t>
  //   
  // hvec(h)->headed_vector_maker
  // hvec(x1, x2, x3)->headed_vector_maker // = hvec(make_tuple(x1, x2, x3))
  //
  // vec(s...)->vector<decay<first_s_t>>
  // vec()->vector<non_constructible_t>

  auto x = hvec(0)(1, 2, 3);
  assert(x.h == 0);
  assert(equal(x.vec, seq(1, 2, 3)));

  x = hvec(1)();
  assert(x.h == 1);
  assert(x.vec.empty());
}
void doc_circular_vector() {
  // circular_vector<T, AL = default_allocator<T>>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = AL{})
  //   this_t(n, x, alloc = AL{})
  //   assign(n, x)
  //   this_t(from, to, alloc = AL{})
  //   assign(from, to)
  //   this_t(il, alloc = AL{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   capacity()->size_type
  //   full()->bool
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   circular_vector(from_range, r, alloc = AL{})
  //   explicit circular_vector(r)
  //   circular_vector(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(size_type n)
  //   pop_front(size_type n)
  //   append(s...)->this_t &
  //   prepend(s...)->this_t &
}
void doc_deque() {
  // deque<T, AL = default_allocator<T>>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = AL{})
  //   this_t(n, x, alloc = AL{})
  //   assign(n, x)
  //   this_t(from, to, alloc = AL{})
  //   assign(from, to)
  //   this_t(il, alloc = AL{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   upper_capacity()->size_type
  //   lower_capacity()->size_type
  //   full()->bool // if reached lower_capacity
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   replace(i1, i2, r)->iterator
  //
  //   deque(from_range, r, alloc = AL{})
  //   explicit deque(r)
  //   deque(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)->iterator
  //   insert_range(cit, r)->iterator
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(size_type n)
  //   pop_front(size_type n)
  //   append(s...)->this_t &
  //   prepend(s...)->this_t &
}
void doc_forward_list() {
  // join_forward_list<size_t ID = 0, VOID_PTR = void *>
  // forward_list_node<T, AL = default_allocator<T>>
  //   : join_forward_list<0, typename allocator_traits<AL>::void_pointer>
  //   alignas(T) byte data[sizeof(T)]
  //   T &operator *() const
  //   T *operator ->() const
  //
  // forward_list_traits<NODE_T, ID = 0,
  //                     STORE_SIZE = false, STORE_NODE_ALLOCATOR = true,
  //                     AL = default_allocator<NODE_T>>
  //   value_type = alloc_vt<AL>
  //   allocator_type = AL
  //   node_type = NODE_T
  //   node_pointer = alloc_rebind_ptr<AL, node_type>
  //   node_base_type = join_forward_list<ID, alloc_void_ptr<AL>>
  //   node_base_pointer = alloc_rebind_ptr<AL, node_base_type>
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //   id = size_constant<ID>
  //   store_node_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_size = bool_constant<STORE_SIZE>
  //   static next(node_base_pointer)->node_base_pointer
  //   static next(node_base_pointer, node_base_pointer)
  //   static data(node_base_pointer)->value_type *
  //   static new_node(AL &, s...)->node_base_pointer
  //   static delete_node(AL &, node_base_pointer)
  //   header_type
  //   static before_begin_node(const header_type &)->node_base_pointer
  //   static size(const header_type &)->size_type
  //   static size(header_type &, size_type)
  //
  // forward_list_adaptor<TRAITS>
  // flstt<T, AL>
  //   = forward_list_traits<forward_list_node<T, AL>, 0, true, false, AL>
  // slstt<T, AL>
  //   = forward_list_traits<forward_list_node<T, AL>, 0, false, false, AL>
  // forward_list<T, AL> = forward_list_adaptor<flstt<T, AL>>
  // slist<T, AL> = forward_list_adaptor<slstt<T, AL>>
  //
  // forward_list_adaptor<TRAITS>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //     explicit this_t(TRAITS::node_base_pointer)
  //     node()->TRAITS::node_base_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
  //   const_iterator
  //   difference_type
  //   size_type
  //   before_begin()
  //   begin()
  //   end()
  //   cbefore_begin()
  //   cbegin()
  //   cend()
  //   max_size()
  //   empty()
  //   size()
  //   special member functions: full or movable
  //   == <=>
  //
  //   traits = TRAITS
  //   node_pointer = TRAITS::node_base_pointer
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //   exchange_node_after(const_iterator, const_iterator)->iterator
  //   link_after(const_iterator, const_iterator)->iterator
  //   link_after(const_iterator,
  //              const_iterator i, const_iterator i_last)->iterator
  //     // [i, i_last] are linked together
  //   link_front(const_iterator)->iterator
  //   link_front(const_iterator i, const_iterator i_last)->iterator
  //     // [i, i_last] are linked together
  //   unlink_after(const_iterator)->iterator
  //   unlink_after(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_front()->iterator
  //   erase_or_unlink_after(const_iteartor)->iterator
  //
  //   node_type
  //     special member functions: default-constructible, movable
  //     get_allocator()->allocator_type
  //     explicit operator bool()
  //     empty()->bool
  //     get()->node_pointer
  //     release()->node_pointer
  //     *()->value_type &
  //     clear()
  //   make_node(s...)->node_type
  //   extract_after(const_iterator)->node_type
  //   insert_after(const_iterator, node_type &&)->iterator
  //   push_front(node_type &&)
  //   exchange_after(const_iterator, node_type &)->node_type
  //   exchange_after(const_iterator, node_type &&)->node_type
  //   replace_after(const_iterator, node_type &&)->iterator
  //
  //   extract_after(const_iterator, const_iterator)->this_t
  //   insert_after(const_iterator, this_t &&)->iterator
  //   push_front(this_t &&)
  //   exchange_after(const_iterator, const_iterator, this_t &&)->this_t
  //   replace_after(const_iterator, const_iterator, this_t &&)->iterator
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = alloc_t{})
  //   this_t(n, x, alloc = alloc_t{})
  //   assign(n, x)
  //   this_t(from, to, alloc)
  //   assign(from, to)
  //   this_t(il, alloc = alloc_t{})
  //   =(il)
  //   assign(il)
  //
  //   emplace_after(prev, s...)
  //   insert_after(prev, x)
  //   insert_after(prev, n, x)
  //   insert_after(prev, from, to)
  //   insert_after(prev, il)
  //   erase_after(prev)
  //   erase_after(prev, next)
  //   clear()
  //
  //   front()
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //
  //   resize(n)
  //   resize(n, x)
  //   splice_after(prev, l)
  //   splice_after(prev, l, prev_i)
  //   splice_after(prev, l, prev_i1, i2)
  //   swap_after(prev, prev_i)
  //   swap_after(prev, l, prev_i)
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   unique(eq)
  //   unique()
  //   merge(l, less_f = less{})
  //   reverse()
  //   sort(less_f = less{})
  //   replace_after(prev_i1, i2, r)->iterator
  //     // return iterator to the last inserted pos or prev_i1
  //
  //   this_t(from_range, r, alloc = alloc_t{})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert_after(prev, r)->iterator
  //   insert_range_after(prev, r)->iterator
  //   push_front(r)
  //   prepend_range(r)
  //   pop_front(n)
  //   prepend(s...)->this_t &

  // example for intrusive mode
  {
    struct t : join_forward_list<0>, join_forward_list<1> {
      int value = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x) : value(x) {}
    };
    using traits_t
      = forward_list_traits<t, 0,
                            true, // STORE_SIZE = true
                            false, // STORE_NODE_ALLOCATOR = false
                            allocator<t> // allocator to provide types
                            >;
    using l_t = forward_list_adaptor<traits_t>;
    using traits2_t = forward_list_traits<t, 1, false, false, allocator<t>>;
    using l2_t = forward_list_adaptor<traits2_t>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1);
    t x2(in_place, 2);
    l_t l1;
    l2_t l2;

    l1.link_after(l1.before_begin(), l_t::iterator(&x1));
    l2.link_after(l2.before_begin(), l2_t::iterator(&x2));
    l2.link_after(l2.before_begin(), l2_t::iterator(&x1));
    const auto get_value = [](const auto &x) {return x.value;};
    assert(equal(bind_rng(l1, get_value), seq(1)));
    assert(equal(bind_rng(l2, get_value), seq(1, 2)));
    assert(&l1.front() == &x1);
    assert(&l2.front() == &x1);
    assert(to_address(next(l2.begin())) == &x2);
  }
}
void doc_list() {
  // join_list<size_t ID = 0, VOID_PTR = void *>
  // list_node<T, AL = default_allocator<T>>
  //   : join_list<0, typename allocator_traits<AL>::void_pointer>
  //   alignas(T) byte data[sizeof(T)]
  //   T &operator *() const
  //   T *operator ->() const
  //
  // list_traits<NODE_T, ID = 0,
  //             STORE_SIZE = true, STORE_ALLOCATOR = true,
  //             AL = default_allocator<NODE_T>>
  //   value_type = alloc_vt<AL>
  //   allocator_type = AL
  //   node_type = NODE_T
  //   node_pointer = alloc_rebind_ptr<AL, node_type>
  //   node_base_type = join_list<ID, alloc_void_ptr<AL>>
  //   node_base_pointer = alloc_rebind_ptr<AL, node_base_type>
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //   id = size_constant<ID>
  //   store_node_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_size = bool_constant<STORE_SIZE>
  //   static prev(node_base_pointer)->node_base_pointer
  //   static prev(node_base_pointer, node_base_pointer)
  //   static next(node_base_pointer)->node_base_pointer
  //   static next(node_base_pointer, node_base_pointer)
  //   static data(node_base_pointer)->value_type *
  //   static new_node(AL &, s...)->node_base_pointer
  //   static delete_node(AL &, node_base_pointer)
  //   header_type
  //   static end_node(const header_type &)->node_base_pointer
  //   static size(const header_type &)->size_type
  //   static size(header_type &, size_type)
  //
  // list_adaptor<TRAITS>;
  // lstt<T, AL> = list_traits<list_node<T, AL>, 0, false, false, AL>
  // bdlstt<T, AL> = list_traits<list_node<T, AL>, 0, true, false, AL>
  // template <class T, class AL = default_allocator<T>>
  // list<T, AL> = list_adaptor<lstt<T, AL>>;
  // bidirectional_list<T, AL> = list_adaptor<bdlstt<T, AL>>;
  //
  // list_adaptor<TRAITS>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //     explicit this_t(TRAITS::node_base_pointer)
  //     node()->TRAITS::node_base_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   empty()
  //   size()
  //   special member functions: full or movable
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   traits
  //   node_pointer = TRAITS::node_base_pointer
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //   exchange_node(const_iterator, const_iterator)->iterator
  //   link(const_iterator, const_iterator)->iterator
  //   link(const_iterator, const_iterator i, const_iterator i_last)->iterator
  //     // [i, i_last] are linked together
  //   link_front(const_iterator)->iterator
  //   link_front(const_iterator i, const_iterator i_last)->iterator
  //     // [i, i_last] are linked together
  //   link_back(const_iteartor)->iterator
  //   link_back(const_iteartor i, const_iteartor i_last)->iterator
  //     // [i, i_last] are linked
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_front()->iterator
  //   unlink_back()->iterator
  //   erase_or_unlink(const_iterator)->iterator
  //
  //   node_type
  //     special member functions: default-constructible, movable
  //     get_allocator()->allocator_type
  //     explicit operator bool()
  //     empty()->bool
  //     get()->node_pointer
  //     release()->node_pointer
  //     *()->value_type &
  //     clear()
  //   make_node(s...)->node_type
  //   extract(const_iterator)->node_type
  //   insert(const_iterator, node_type &&)->iterator
  //   push_back(node_type &&)
  //   push_front(node_type &&)
  //   exchange(const_iterator, node_type &)->node_type
  //   exchange(const_iterator, node_type &&)->node_type
  //   replace(const_iterator, node_type &&)->iterator
  //
  //   extract(const_iterator, const_iterator)->this_t
  //   insert(const_iterator, this_t &&)->iterator
  //   push_back(this_t &&)
  //   push_front(this_t &&)
  //   exchange(const_iterator, const_iterator, this_t &)->this_t
  //   exchange(const_iterator, const_iterator, this_t &&)->this_t
  //   replace(const_iterator, const_iterator, this_t &&)->iterator
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = alloc_t{})
  //   this_t(n, x, alloc = alloc_t{})
  //   assign(n, x)
  //   this_t(from, to, alloc)
  //   assign(from, to)
  //   this_t(il, alloc = {})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //
  //   resize(n)
  //   resize(n, x)
  //   splice(next, l)           // l is not *this
  //   splice(next, l, i)
  //   splice(next, l, from, to) // no overlap
  //   swap(i, j)
  //   swap(i, l, j)
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   unique(eqf = equal_to{})
  //   merge(l, less_f = less{})
  //   reverse()
  //   sort(less_f = less{})
  //   replace(i1, i2, r)->iterator
  //
  //   this_t(from_range, r, alloc = {})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(next, r)->iterator
  //   insert_range(next, r)->iterator
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(size_type n)
  //   pop_front(size_type n)
  //   append(s...)
  //   prepend(s...)->this_t &

  // example for intrusive mode
  {
    struct t : join_list<0>, join_list<1> {
      int value = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x) : value(x) {}
    };
    using traits_t
      = list_traits<t, 0,
                    true, // STORE_SIZE = true
                    false, // STORE_NODE_ALLOCATOR = false
                    allocator<t> // allocator to provide types
                    >;
    using l_t = list_adaptor<traits_t>;
    using traits2_t = list_traits<t, 1, false, false, allocator<t>>;
    using l2_t = list_adaptor<traits2_t>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1);
    t x2(in_place, 2);
    l_t l1;
    l2_t l2;

    l1.link(l1.end(), l_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x2));
    const auto get_value = [](const auto &x) {return x.value;};
    assert(equal(bind_rng(l1, get_value), seq(1)));
    assert(equal(bind_rng(l2, get_value), seq(1, 2)));
    assert(&l1.front() == &x1);
    assert(&l2.front() == &x1);
    assert(to_address(next(l2.begin())) == &x2);
  }
}
void doc_queue() {
  // queue<T, C = deque<T>>
  //   value_type
  //   reference
  //   const_reference
  //   size_type
  //   container_type
  //
  //   special member functions: dependent
  //   ==, <=>
  //
  //   explicit this_t(alloc)
  //   explicit this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   explicit this_t(c)
  //   this_t(c, alloc)
  //
  //   this_t(from, to)
  //   this_t(from, to, alloc)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, alloc)
  //
  //   empty()
  //   size()
  //   front()
  //   back()
  //   push(x)
  //   push_range(r)
  //   emplace(s...)
  //   pop()
  //
  // uses_allocator<queue<T, C>, AL> : uses_allocator<C, AL> {}
}
void doc_stack() {
  // stack<T, C = deque<T>>
  //   value_type
  //   reference
  //   const_reference
  //   size_type
  //   container_type
  //
  //   special member functions: dependent
  //   ==, <=>
  //
  //   explicit this_t(alloc)
  //   explicit this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   explicit this_t(c)
  //   this_t(c, alloc)
  //
  //   this_t(from, to)
  //   this_t(from, to, alloc)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, alloc)
  //
  //   empty()
  //   size()
  //   top()
  //   push(x)
  //   push_range(r)
  //   emplace(s...)
  //   pop()
  //
  // uses_allocator<stack<T, C>, AL> : uses_allocator<C, AL> {}
}
void doc_priority_queue() {
  // priority_queue<T, C = vector<T>, CMP = less<typename C::value_type>>
  //   value_type
  //   reference
  //   const_reference
  //   size_type
  //   container_type
  //   value_compare
  //
  //   special member functions: dependent
  //
  //   explicit this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   this_t(cmp)
  //   this_t(cmp, alloc)
  //   this_t(cmp, c)
  //   this_t(cmp, c, alloc)
  //
  //   this_t(from, to)
  //   this_t(from, to, alloc)
  //   this_t(from, to, cmp)
  //   this_t(from, to, cmp, alloc)
  //   this_t(from, to, cmp, c)
  //   this_t(from, to, cmp, c, alloc)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, cmp)
  //   this_t(from_range, r, alloc)
  //   this_t(from_range, r, cmp, alloc)
  //
  //   empty()
  //   size()
  //   top()->const_reference
  //   push(x)
  //   push_range(r)
  //   emplace(s...)
  //   pop()
  //
  // uses_allocator<priority_queue<T, C, CMP>, AL>
  //   : uses_allocator<C, AL> {}
}
void doc_flat_map() {
  // flat_set/multiset<T, LESS = less<T>, C = vector<T>>
  // flat_map/multimap<K, M, LESS = less<T>, C = vector<pair<K, M>>
  //   using container_type = C
  //   extract() &&->container_type
  //   replace(container_type &&)
  //
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type // no definition if no C::allocator_type
  //   get_allocator()
  //   explicit this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   key_type
  //   mapped_type // only for map and multimap
  //   key_compare
  //   value_compare
  //   key_comp()->key_compare
  //   value_comp()->value_compare
  //
  //   explicit this_t(cmp)
  //   this_t(cmp, alloc)
  //   this_t(from, to, cmp)
  //   this_t(from, to, alloc)
  //   this_t(from, to, cmp, alloc)
  //   assign(from, to)
  //   this_t(il, cmp)
  //   this_t(il, alloc)
  //   this_t(il, cmp, alloc)
  //   =(il)
  //   assign(il)
  //
  //   emplace(s...)->pair<iterator, bool> // unique only
  //   insert(x)->pair<iterator, bool> // unique only
  //   emplace(s...)->iterator // multi only
  //   insert(x)->iterator // multi only
  //   emplace_hint(hint, s...)->iterator
  //   insert(hint, x)->iterator
  //   insert(from, to)
  //   insert(il)
  //   erase(i)->iterator
  //   erase(from, to)->iterator
  //   erase(key)->size_type
  //   remove(key)->size_type
  //   remove_if(eq)->size_type
  //   clear()
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)->(const) mapped_type & // const version has no check
  //   at(key)->(const) mapped_type & // throw if not found
  //   ///
  //
  //   find(key)->iterator
  //   contains(key)->bool
  //   count(key)->size_type
  //   lower_bound(key)->(const_)iterator
  //   upper_bound(key)->(const_)iterator
  //   equal_range(key)->iter_pair<(const_)iterator>
  //   find_range(key_min, key_max)->iter_pair<(const_)iterator>
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //
  //   merge(this_t &&)
  //   merge(r)
  //   unique() // do nothing for unique map
  //   unique(eqf)
  //
  //   reserve(n)
  //   capacity()->size_type
  //   shrink_to_fit()
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, cmp)
  //   this_t(from_range, r, alloc)
  //   this_t(from_range, r, cmp, alloc)
  //   explicit this_t(r)
  //   this_t(r, cmp)
  //   this_t(r, alloc)
  //   this_t(r, cmp, alloc)
  //   =(r)
  //   assign_range(r)
  //   assign(r)
  //   insert(r)
  //   insert_range(r)
  //   append(s...)->this_t &
}
void doc_binary_search_tree_adaptor() {
  // rbt_node_base<ID = 0, VOID_PTR = void *>
  //   left_child
  //   right_child
  //   parent
  // join_rbtree<ID = 0, VOID_PTR = void *>
  //   : rbt_node_base<ID, VOID_PTR>
  //   red
  // rbtree_node<T, VOID_PTR = void *>
  //   : join_rbtree<0, alloc_void_ptr<AL>>
  //   data
  //   * ->
  //
  // avlt_node_base
  // join_avltree
  // avltree_node
  //
  // rrbt_node_base
  // join_ranked_rbtree
  // ranked_rbtree_node
  //
  // rbtree_traits<NODE_T, ID = 0,
  //               STORE_SIZE = true, STORE_ALLOCATOR = false,
  //               AL = default_allocator<NODE_T>>
  // avltree_traits<...>
  // ranked_rbtree_traits<...>
  //   value_type = alloc_vt<AL>
  //   allocator_type = AL
  //   node_type = NODE_T
  //   node_pointer = alloc_rebind_ptr<AL, node_type>
  //   node_base_type = avlt_node_base<ID, alloc_void_ptr<AL>>
  //   node_base_pointer = alloc_rebind_ptr<AL, node_base_type>
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //   id = size_constant<ID>
  //   store_size = bool_constant<STORE_SIZE>
  //   store_node_allocator = bool_constant<STORE_ALLOCATOR>
  //   store_allocator = bool_constant<STORE_ALLOCATOR>
  //
  //   static left_child(node_base_pointer)->node_base_pointer
  //   static right_child(node_base_pointer)->node_base_pointer
  //   static parent(node_base_pointer)->node_base_pointer
  //   static left_child(node_base_pointer, node_base_pointer)
  //   static right_child(node_base_pointer, node_base_pointer)
  //   static parent(node_base_pointer, node_base_pointer)
  //
  //   static red(node_base_pointer)->bool // only for rbtree
  //   static red(node_base_pointer, bool) // only for rbtree
  //
  //   static factor(node_base_pointer)->int // only for avltree
  //   static factor(node_base_pointer, int) // only for avltree
  //
  //   static state(node_base_pointer)->ptrdiff_t // only for ranked_rbtree
  //   static state(node_base_pointer, ptrdiff_t) // only for ranked_rbtree
  //
  //   static data(node_base_pointer)->value_type *
  //
  //   // the node type is regular raw space wrapper
  //   // or is same as value_type or 
  //   static new_node(AL &, s...)->node_base_pointer
  //   static delete_node(AL &, node_base_pointer)
  //
  //   header_type
  //   static end_node(const header_type &)->node_base_pointer
  //   // size(...) is not for ranked_rbtree,
  //   // and is only used if store_size::value
  //   static size_type size(const header_type &)
  //   static size(header_type &, size_type)
  //
  // rbtree_adaptor<TRAITS>
  // avltree_adaptor<TRAITS>
  // ranked_rbtree_adaptor<TRAITS>
  //   traits = TRAITS
  //   node_pointer = TRAITS::node_base_pointer
  //
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //     explicit this_t(TRAITS::node_base_pointer)
  //     node()->node_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
  //     promote()->random_access_iter // only for ranked_rbtree
  //     nth()->difference_type // return rank if *this
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //   delete_tree(const_iterator) // delete all node linked
  //   exchange_node(const_iterator, const_iterator)->iterator
  //     // ranked_rbtree only
  //
  //   root()->(const_)iterator // nullptr if empty
  //   leftmost()->(const_)iterator // end() if empty
  //   rightmost()->(const_)iterator // end() if empty
  //
  //   special member functions: full or movable
  //   ==
  //   <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   partition_point(eq)->iterator
  //   find(get_key, key_less, k)->iterator
  //   lower_bound(get_key, key_less, k)->iterator
  //   upper_bound(get_key, key_less, k)->iterator
  //   equal_range(get_key, key_less, k)->iter_pair<iterator>
  //   count(get_key, key_less, k)->size_type
  //   find_range(get_key, key_less, k1, k2)->iter_pair<iterator>
  //
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)
  //   unlink()
  //   unlink(get_key, key_less, k)->size_type
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(get_key, key_less, k)->size_type
  //   extract<NODE_T>(cit)->NODE_T
  //   extract<NODE_T>(getkey, keyless, k)->NODE_T
  //   clear()
  //
  //   link(const_iterator, const_iterator)->iterator
  //   emplace(const_iterator, s...)->iterator
  //   insert(const_iterator, x)->iterator
  //   insert(const_iterator, r)->iterator
  //   insert_range(const_iterator, r)->iterator
  //   assign_range(r)
  //
  //   insert(get_key, key_less,
  //          k, get_new_node, do_when_equal = ...)->pair<iterator, bool>
  //   insert(get_key, key_less,
  //          hint, k, get_new_node, do_when_equal = ...)->iterator
  //   insert_equal(get_key, key_less, k, get_new_node)->iterator
  //   insert_equal(get_key, key_less, hint, k, get_new_node)->iterator
  //
  //   link(get_key, key_less, it)->pair<iterator, bool>
  //   emplace(get_key, key_less, s...)->pair<iterator, bool>
  //   insert(get_key, key_less, const value_type &)->pair<iterator, bool>
  //   insert(get_key, key_less, value_type &&)->pair<iterator, bool>
  //
  //   link(get_key, key_less, hint, it)->iterator
  //   emplace_hint(get_key, key_less, hint, s...)->iterator
  //   insert(get_key, key_less, hint, const value_type &)->iterator
  //   insert(get_key, key_less, hint, value_type &&)->iterator
  //
  //   try_emplace(get_key, key_less, k, s...)->pair<iterator, bool>
  //   try_emplace_hint(get_key, key_less, hint, k, s...)->iterator
  //   insert_or_assign(get_key, get_mapped, key_less, k, x)
  //     ->pair<iterator, bool>
  //   insert_or_assign(get_key, get_mapped, key_less, hint, k, x)->iterator
  //   insert_node_handle<ITER>(get_key, key_less, nh_rref)
  //     ->inner::node_handle_insert_return_type<...>
  //   insert_node_handle(get_key, key_less, hint, nh_rref)->iterator
  //
  //   link_equal(get_key, key_less, it)->iterator
  //   link_equal(get_key, key_less, hint, it)->iterator
  //   emplace_equal(get_key, key_less, s...)->iterator
  //   emplace_equal_hint(get_key, key_less, hint, s...)->iterator
  //   insert_node_handle_equal(get_key, key_less, nh_rref)->iterator
  //   insert_node_handle_equal(get_key, key_less, hint, nh_rref)->iterator
  //
  //   merge(get_key, key_less, analogous_(set/map/multiset/multimap))
  //   merge_equal(get_key, key_less, analogous_(set/map/multiset/multimap))
  //
  //   insert_range(get_key, key_less, r)
  //   assign_range(get_key, key_less, r,
  //                insert_range_fn = insert_range(get_key, key_less, _1))
  //   insert_range_equal(get_key, key_less, r)
  //   assign_range_equal(get_key, key_less, r)
  //
  //   front() // ranked_rbtree only
  //   unlink_front() // ranked_rbtree only
  //   pop_front() // ranked_rbtree only
  //   pop_front(n) // ranked_rbtree only
  //   link_front(cit) // ranked_rbtree only
  //   emplace_front(s...) // ranked_rbtree only
  //   push_front(x) // ranked_rbtree only
  //
  //   back() // ranked_rbtree only
  //   unlink_back() // ranked_rbtree only
  //   pop_back() // ranked_rbtree only
  //   pop_back(size_type n) // ranked_rbtree only
  //   link_back(cit) // ranked_rbtree only
  //   emplace_back(s...) // ranked_rbtree only
  //   push_back(x) // ranked_rbtree only
  //
  //   [](n) // ranked_rbtree only
  //   at(n) // ranked_rbtree only
  //
  //   explicit this_t(n, alloc = alloc_t{}) // ranked_rbtree only
  //   this_t(n, x, alloc = alloc_t{}) // ranked_rbtree only
  //   assign(n, x) // ranked_rbtree only
  //   insert(cit, n, x) // ranked_rbtree only
  //
  //   this_t(from, to, alloc = alloc_t{}) // ranked_rbtree only
  //   assign(from, to) // ranked_rbtree only
  //   insert(cit, from, to) // ranked_rbtree only
  //
  //   this_t(il, alloc = alloc_t{}) // ranked_rbtree only
  //   =(il) // ranked_rbtree only
  //   assign(il) // ranked_rbtree only
  //   insert(cit, il) // ranked_rbtree only
  //
  //   this_t(from_range, r, alloc = alloc_t{}) // ranked_rbtree only
  //   explicit this_t(r) // ranked_rbtree only
  //   this_t(r, alloc) // ranked_rbtree only
  //   =(r) // ranked_rbtree only
  //   insert(cit, r)->iterator // ranked_rbtree only
  //   assign(r) // ranked_rbtree only
  //
  //   append_range(r) // ranked_rbtree only
  //   push_back(r) // ranked_rbtree only
  //   append(...)->this_t & // ranked_rbtree only
  //
  //   prepend_range(r) // ranked_rbtree only
  //   push_front(r) // ranked_rbtree only
  //   prepend(...)->this_t & // ranked_rbtree only
  //
  //   resize(n) // ranked_rbtree only
  //   resize(n, x) // ranked_rbtree only
  //
  //   splice(next, this_ref) // ranked_rbtree only
  //   splice(next, this_ref, cit) // ranked_rbtree only
  //   splice(next, this_ref, cit, cit) // ranked_rbtree only
  //     // next is not among [first, last)
  //
  //   swap(cit, cit) // ranked_rbtree only
  //   swap(cit, this_ref, cit) // ranked_rbtree only
  //
  //   erase_or_unlink(cit)->iterator // ranked_rbtree only
  //   remove_if(eq)->size_type // ranked_rbtree only
  //   remove(x)->size_type // ranked_rbtree only
  //
  //   unique(eq) // ranked_rbtree only
  //   unique() // ranked_rbtree only
  //
  //   merge(this_ref, less_f = less{}) // ranked_rbtree only
  //
  //   reverse() // ranked_rbtree only
  //
  //   sort(less_f = less{}) // ranked_rbtree only
  //
  //   replace(i1, i2, r)->iterator // ranked_rbtree only
  //
  //   node_type // ranked_rbtree only
  //   make_node(s...)->node_type // ranked_rbtree only
  //   extract(const_iterator)->node_type // ranked_rbtree only
  //   insert(const_iterator, node_type &&)->iterator // ranked_rbtree only
  //   exchange(const_iterator, node_type &)->node_type // ranked_rbtree only
  //   exchange(const_iterator, node_type &&)->node_type // ranked_rbtree only
  //   replace(const_iterator, node_type &&)->iterator // ranked_rbtree only
  //
  //   extract(const_iterator, const_iterator)->this_t // ranked_rbtree only
  //   insert(const_iterator, this_t &&)->iterator // ranked_rbtree only
  //   push_front(this_t &&) // ranked_rbtree only
  //   push_back(this_t &&) // ranked_rbtree only
  //   exchange(const_iterator, const_iterator, this_t &)->this_t
  //     // ranked_rbtree only
  //   exchange(const_iterator, const_iterator, this_t &&)->this_t
  //     // ranked_rbtree only
  //   replace(const_iterator, const_iterator, this_t &&)->iterator
  //     // ranked_rbtree only

  // example for intrusive mode
  {
    struct t : join_rbtree<0>, join_rbtree<1> {
      int value = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x) : value(x) {}
    };
    using traits_t
      = rbtree_traits<t, 0,
                      true, // STORE_SIZE = true
                      false, // STORE_NODE_ALLOCATOR = false
                      allocator<t> // allocator to provide types
                      >;
    using l_t = rbtree_adaptor<traits_t>;
    using traits2_t = rbtree_traits<t, 1, false, false, allocator<t>>;
    using l2_t = rbtree_adaptor<traits2_t>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1);
    t x2(in_place, 2);
    l_t l1;
    l2_t l2;

    l1.link(l1.end(), l_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x2));
    const auto get_value = [](const auto &x) {return x.value;};
    assert(equal(bind_rng(l1, get_value), seq(1)));
    assert(equal(bind_rng(l2, get_value), seq(1, 2)));
    assert(&front(l1) == &x1);
    assert(&front(l2) == &x1);
    assert(to_address(next(l2.begin())) == &x2);
  }
}
void doc_map_adaptor() {
  // set_adaptor<TREE, LESS>
  // multiset_adaptor<TREE, LESS>
  // map_adaptor<TREE, LESS>
  // multimap_adaptor<TREE, LESS>
  //   traits = TREE::traits
  //   node_pointer = TREE::node_base_pointer
  //
  //   root()->(const_)iterator // may be nullptr
  //   leftmost()->(const_)iterator // may be nullptr
  //   rightmost()->(const_)iterator // may be nullptr
  //
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full or movable
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type // no definition if no C::allocator_type
  //   get_allocator()
  //   explicit this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   key_type
  //   mapped_type // only for map and multimap
  //   key_compare
  //   value_compare
  //   node_type
  //     special member functions: default-constructible, movable
  //     get_allocator() // not empty
  //     explicit operator bool()
  //     empty()
  //     *()->non_const_ref
  //     value_type // only for set/multiset
  //     value()->value_type &    // only for set/multiset
  //     key_type   // only for map/multimap
  //     mapped_type   // only for map/multimap
  //     key()->key_type &         // only for map/multimap
  //     mapped()->mapped_type &      // only for map/multimap
  //     get()->node_pointer
  //     release()->node_pointer
  //   insert_return_type // unique only
  //     iterator position
  //     bool inserted
  //     node_type node
  //   key_comp()->LESS
  //   value_comp()->LESS
  //
  //   explicit this_t(cmp)
  //   this_t(cmp, alloc)
  //   this_t(from, to, cmp)
  //   this_t(from, to, alloc)
  //   this_t(from, to, cmp, alloc)
  //   assign(from, to)
  //   this_t(il, cmp)
  //   this_t(il, alloc)
  //   this_t(il, cmp, alloc)
  //   =(il)
  //   assign(il)
  //
  //   emplace(s...)->pair<iterator, bool> // unique only
  //   insert(x)->pair<iterator, bool> // unique only
  //   emplace(s...)->iterator // multi only
  //   insert(x)->iterator // multi only
  //   emplace_hint(hint, s...)->iterator
  //   insert(hint, x)->iterator
  //   insert(from, to)
  //   insert(il)
  //
  //   insert(node_type &&)->insert_return_type // unique only
  //   insert(node_type &&)->iterator // multi only
  //   insert(hint, node_type &&)->iterator
  //   extract(cit)->node_type
  //   extract(key)->node_type
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)->(const) mapped_type &
  //     // const version has no guard for unexisted key
  //   at(key)->(const) mapped_type & // throw if not found
  //   ///
  //
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(key)->size_type
  //   remove(key)->size_type
  //   remove_if(eq)->size_type
  //   clear()
  //
  //   merge(multi_or_non_multi_ref)
  //
  //   find(key)->(const_)iterator // get the first among equal elements
  //   contains(key)->bool
  //   count(key)->size_type
  //   lower_bound(key)->(const_)iterator
  //   upper_bound(key)->(const_)iterator
  //   equal_range(key)->iter_pair<(const_)iterator>
  //   partition_point(eqf)->iterator
  //   find_range(min_key, max_key)->iter_pair<(const_)iterator>
  //
  //   new_node(s...)->iterator
  //   delete_node(const_iteartor)
  //
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(const_iterator)->pair<iterator, bool> // unique only
  //   link(const_iterator)->iterator // multi only
  //   link(const_iterator hint, const_iterator)->iterator
  //
  //   try_link(key, get_node, do_when_eq = ...)->pair<iterator, bool>
  //     // unique only
  //   try_link_hint(hint, key, get_node, do_when_eq = ...)->iterator
  //     // unique only
  //   try_link(key, get_node)->iterator
  //     // multi only
  //   try_link_hint(hint, key, get_node)->iterator
  //     // multi only
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //
  //   nth(difference_type)->(const_)iterator // ranked_rbtree only
  //   nth(const_iterator)->iterator // ranked_rbtree only
  //
  //   erase_or_unlink(const_iterator)->iterator
  //
  //   unique(eq = ...)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, cmp)
  //   this_t(from_range, r, cmp, alloc)
  //   this_t(from_range, r, alloc)
  //   explicit this_t(r)
  //   this_t(r, cmp)
  //   this_t(r, alloc)
  //   this_t(r, cmp, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(r)
  //   insert_range(r)
  //   append(s...)->this_t &

  // example for intrusive mode
  {
    struct t : join_rbtree<0>, join_rbtree<1> {
      int key = 0;
      int mapped = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x, int y) : key(x), mapped(y) {}
    };
    struct traits_t
      : public rbtree_traits<t, 0,
                             true, // STORE_SIZE = true
                             false, // STORE_NODE_ALLOCATOR = false
                             allocator<t> // allocator to provide types
                             > {
      using base_t = rbtree_traits<t, 0, true, false, allocator<t>>;
      using key_type = int;
      using mapped_type = int; // for set and multiset, no mapped_type
      static const key_type &key(const typename base_t::value_type &x) {
        return x.key;
      }
      // for set and multiset, no mapped()
      static mapped_type &mapped(typename base_t::value_type &x) {
        return x.mapped;
      }
      static
      const mapped_type &mapped(const typename base_t::value_type &x) {
        return x.mapped;
      }
    };
    using l_t = map_adaptor<rbtree_adaptor<traits_t>, less<int>>;
    struct traits2_t
      : public rbtree_traits<t, 1, false, false, allocator<t>> {
      using base_t = rbtree_traits<t, 1, false, false, allocator<t>>;
      using key_type = int;
      using mapped_type = int;
        // for set and multiset, no mapped_type
      static const key_type &
      key(const typename base_t::value_type &x) {return x.key;}
      static mapped_type &
      mapped(typename base_t::value_type &x) {return x.mapped;}
      static const mapped_type &
      mapped(const typename base_t::value_type &x) {return x.mapped;}
    };
    using l2_t = map_adaptor<rbtree_adaptor<traits2_t>, greater<int>>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1, 1);
    t x2(in_place, 2, 2);
    t x3(in_place, 3, 3);
    l_t l1;
    l2_t l2;

    assert(l1.link(l_t::iterator(&x1)).second);
    assert(l1.link(l_t::iterator(&x2)).second);
    assert(l2.link(l2_t::iterator(&x2)).second);
    assert(l2.link(l2_t::iterator(&x3)).second);
    const auto get_value = [](const auto &x) {return pair(x.key, x.mapped);};
    assert(equal(bind_rng(l1, get_value), seq(pair(1, 1), pair(2, 2))));
    assert(equal(bind_rng(l2, get_value), seq(pair(3, 3), pair(2, 2))));
  }
}
void doc_map_typedef() {
  // rbtt<T, AL> : public rbtree_traits<rbtree_node<T, alloc_void_ptr<AL>>,
  //                                    0, 1, 1, AL>
  //
  // rbsett<T, AL> : public rbtt<T, AL>
  //   key_type = rbtt<T, AL>::value_type
  //   static key(const rbtt<T, AL>::value_type &)->const key_type &
  // rbmapt<T, AL> : public rbtt<T, AL>
  //   key_type = rbtt<T, AL>::first_type
  //   mapped_type = rbtt<T, AL>::second_type
  //   static key(const rbtt<T, AL>::value_type &)->const key_type &
  //   static mapped(rbtt<T, AL>::value_type &)->mapped_type &
  //   static mapped(const rbtt<T, AL>::value_type &)->const mapped_type &
  // rbtree<T, AL = default_allocator<T>>
  // rb_set<T, LESS = less<T>, AL = default_allocator<T>>
  // rb_multiset<T, LESS = less<T>, AL = default_allocator<T>>
  // rb_map<KEY, MAPPED, LESS = less<pair<KEY, MAPPED>>,
  //        AL = default_allocator<pair<KEY, MAPPED>>>
  // rb_multimap<KEY, MAPPED, LESS = less<pair<KEY, MAPPED>>,
  //             AL = default_allocator<pair<KEY, MAPPED>>>
  //
  // nosz_rbtt<T, AL>
  // nosz_rbsett<T, AL>
  // nosz_rbmapt<T, AL>
  // nosz_rbtree<T, AL = ...>
  // nosz_rb_set<T, LESS = ..., AL = ...>
  // nosz_rb_multiset<T, LESS = ..., AL = ...>
  // nosz_rb_map<KEY, MAPPED, LESS = ..., AL = ...>
  // nosz_rb_multimap<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // avltt<T, AL> : public avltree_traits<avltree_node<T, AL>, 0, 1, 1, AL>
  // avlsett<T, AL>
  // avlmapt<T, AL>
  // avltree<T, AL = ...>
  // avl_set<T, LESS = ..., AL = ...>
  // avl_multiset<T, LESS = ..., AL = ...>
  // avl_map<KEY, MAPPED, LESS = ..., AL = ...>
  // avl_multimap<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // nosz_avltt<T, AL>
  // nosz_avlsett<T, AL>
  // nosz_avlmapt<T, AL>
  // nosz_avltree<T, AL = ...>
  // nosz_avl_set<T, LESS = ..., AL = ...>
  // nosz_avl_multiset<T, LESS = ..., AL = ...>
  // nosz_avl_map<KEY, MAPPED, LESS = ..., AL = ...>
  // nosz_avl_multimap<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // set<T, LESS = less<T>, AL = default_allocator<T>>
  // multiset<T, LESS = less<T>, AL = default_allocator<T>>
  // map<KEY, MAPPED, LESS = less<KEY>,
  //     AL = default_allocator<pair<KEY, MAPPED>>>
  // multimap<KEY, MAPPED, LESS = less<KEY>,
  //          AL = default_allocator<pair<KEY, MAPPED>>>
  //
  // nosz_set<T, LESS = ..., AL = ...>
  // nosz_multiset<T, LESS = ..., AL = ...>
  // nosz_map<KEY, MAPPED, LESS = ..., AL = ...>
  // nosz_multimap<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // rrbtt<T, AL>
  // rsett<T, AL>
  // rmapt<T, AL>
  // ranked_rbtree<T, AL = default_allocator<T>>
  // ranked_set<T, LESS = less<T> AL = default_allocator<T>>
  // ranked_multiset<T, LESS = less<T>, AL = default_allocator<T>>
  // ranked_map<KEY, MAPPED, LESS = less<KEY>, AL = default_allocator<T>>
  // ranked_multimap<KEY, MAPPED, LESS = less<KEY>, AL = default_allocator<T>>
}
void doc_hashtable_adaptor() {
  // htb_node_base<ID = 0, VOID_PTR = void *>
  //   ptr_rebind<VOID_PTR, htb_node_base> next
  // join_hashtable<ID = 0, VOID_PTR = void *> : htb_node_base<ID, VOID_PTR>
  //   ptr_rebind<VOID_PTR, htb_node_base<ID, VOID_PTR>> prev
  // hashtable_node<T, VOID_PTR = void *> : join_hashtable<T, VOID_PTR>
  //   alignas(T) byte data[sizeof(T)]
  //   *()->T &
  //   ->()->T *
  //
  // hashtable_traits<NODE_T, ID = 0, STORE_NODE_ALLOCATOR = true,
  //                  AL = default_allocator<NODE_T>>
  //   value_type = alloc_vt<AL>
  //   allocator_type = AL
  //   node_type = NODE_T
  //   node_pointer = alloc_rebind_ptr<AL, node_type>
  //   node_base_type = htb_node_base<ID, alloc_void_ptr<AL>>
  //   node_base_pointer = alloc_rebind_ptr<AL, node_base_type>
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //
  //   id = size_constant<ID>
  //   store_node_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_allocator = true_type
  //
  //   static next(node_base_pointer)->node_base_pointer
  //   static next(node_base_pointer, node_base_pointer)
  //
  //   static prev(node_base_pointer)->node_base_pointer
  //   static prev(node_base_pointer, node_base_pointer)
  //
  //   static data(node_base_pointer)->value_type *
  //
  //   header_type
  //
  //   static end_node(const header_type &)->node_base_pointer
  //
  //   static placeholder_node(const header_type &)->node_base_pointer
  //
  //   static size(const header_type &)->size_type
  //   static size(const header_type &, size_type)
  //
  //   static factor(const header_type &)->float
  //   static factor(const header_type &, float)
  //
  //   static new_node(alloc, s...)->node_base_pointer
  //   static delete_node(alloc, node_base_pointer)
  //
  // hashtable_adaptor<TRAITS>
  //   traits = TRAITS
  //   node_pointer = TRAITS::node_base_pointer
  //
  //   value_type
  //   reference
  //   const_reference
  //
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //
  //   special member functions: full or movable
  //   ==
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   local_iterator
  //   const_local_iterator
  //   begin(size_type)->(const_)local_iterator
  //   end(size_type)->(const_)local_iterator
  //   cbegin(size_type)->const_local_iterator
  //   cend(size_type)->const_local_iterator
  //
  //   bucket_count()->size_type
  //   active_bucket_count()->size_type
  //   max_bucket_count()->size_type
  //   bucket_size(size_type)->size_type
  //   bucket(hash, key)->size_type
  //   bucket_range(size_type)->iter_pair<(const_)local_iterator>
  //
  //   load_factor()->float
  //   max_load_factor()->float
  //   max_load_factor(float)
  //
  //   find(get_key, hash, eq, key)->iterator
  //   equal_range(get_key, hash, eq, key)->iter_pair<iterator>
  //   count(get_key, hash, eq, key)->size_type
  //
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink(get_key, hash, eq, key)->size_type
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(get_key, hash, eq, key)->size_type
  //   extract<NODE_HANDLE>(const_iterator)->NODE_HANDLE
  //   extract<NODE_HANDLE>(get_key, hash, eq, key)->NODE_HANDLE
  //   clear()
  //
  //   auto_rehash(get_key, hash, eq, current_size)
  //   auto_rehash(get_key, hash, eq)
  //   rehash(get_key, hash, eq, n)
  //   reserve(get_key, hash, eq, n)
  //
  //   insert(get_key, hash, eq, key, get_new_node, do_when_equal)
  //     ->pair<iterator, bool>
  //   insert_equal(get_key, hash, eq, key, get_new_node)->iterator
  //
  //   link(get_key, hash, eq, it)->pair<iterator, bool>
  //   emplace(get_key, hash, eq, s...)->pair<iterator, bool>
  //   insert(get_key, hash, eq, const value_type &)->pair<iterator, bool>
  //   insert(get_key, hash, eq, value_type &&)->pair<iterator, bool>
  //
  //   try_emplace(get_key, hash, eq, key, s...)->pair<iterator, bool>
  //   insert_or_assign(get_key, get_mapped, hash, eq, key, x)
  //     ->pair<iterator, bool>
  //   insert_node_handle<ITER>(get_key, hash, eq, nh)
  //     ->node_handle_insert_return_type_for_ITER
  //
  //   link_equal(get_key, hash, eq, it)->iterator
  //   emplace_equal(get_key, hash, eq, s...)->iterator
  //   insert_equal(get_key, hash, eq, x)->iterator
  //   insert_node_handle_equal(get_key, hash, eq, nh)->iterator
  //
  //   link_equal(get_key, hash, eq, hint, it)->iterator
  //   emplace_equal_hint(get_key, hash, eq, hint, s...)->iterator
  //   insert_equal(get_key, hash, eq, hint, x)->iterator
  //   insert_node_handle_equal(get_key, hash, eq, hint, nh)->iterator
  //
  //   merge(get_key, hash, eq, analogous)
  //   merge_equal(get_key, hash, eq, analogous)
  //
  //   insert_range_transparent(get_key, hash, eq, r)
  //     // for unique set/map, only make node if the element doesn't exists
  //   insert_range(get_key, hash, eq, r)
  //   insert_range(get_key, hash, eq, r, no_rehash_ins)
  //     // no_rehash_ins(element_ref)
  //     // used to implement insert_range(get_key, hash, eq, r)
  //
  //   assign_range_transparent(get_key, hash, eq, r)
  //     // for unique set/map, only make node if the element doesn't exists
  //   assign_range(get_key, hash, eq, r)
  //   assign_range(get_key, hash, eq, r, no_rehash_ins)
  //     // no_rehash_ins(element_ref)
  //     // used to implement assign_range(get_key, hash, eq, r)
  //
  //   insert_range_equal(get_key, hash, eq, r)
  //   assign_range_equal(get_key, hash, eq, r)
  //
  //   equal(get_key, hash, eq, hash2, eq2, const this_t &)->bool
  //   equal_equal(get_key, hash, eq, hash2, eq2, const this_t &)->bool
  //
  //   static constexpr size_type default_bucket_count = 20;

  // example for intrusive mode
  {
    struct t : join_hashtable<0>, join_hashtable<1> {
      int value = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x) : value(x) {}
    };
    using traits_t = hashtable_traits
      <t, 0,
       false, // STORE_NODE_ALLOCATOR = false
       allocator<t> // allocator to provide types
       >;
    using l_t = hashtable_adaptor<traits_t>;
    using traits2_t = hashtable_traits<t, 0, false, allocator<t>>;
    using l2_t = hashtable_adaptor<traits2_t>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1);
    t x2(in_place, 2);
    t x3(in_place, 3);
    l_t l1;
    l2_t l2;

    const auto get_key = [](const t &x)->int {return x.value;};
    const auto hash_f = [](int x)->size_t {return x;};
    const auto eq_f = [](int x, int x2) {return x == x2;};
    assert(l1.link(get_key, hash_f, eq_f, l_t::iterator(&x1)).second);
    assert(l1.link(get_key, hash_f, eq_f, l_t::iterator(&x2)).second);
    assert(equal(bind_rng(l1, get_key), seq(1, 2))
           || equal(bind_rng(l1, get_key), seq(2, 1)));

    assert(l2.link(get_key, hash_f, eq_f, l2_t::iterator(&x2)).second);
    assert(l2.link(get_key, hash_f, eq_f, l2_t::iterator(&x3)).second);
    assert(equal(bind_rng(l2, get_key), seq(2, 3))
           || equal(bind_rng(l2, get_key), seq(3, 2)));
  }
}
void doc_unordered_map_adaptor() {
  // unordered_set_adaptor<TABLE, HASH, EQ>
  // unordered_multiset_adaptor<TABLE, HASH, EQ>
  // unordered_map_adaptor<TABLE, HASH, EQ>
  // unordered_multimap_adaptor<TABLE, HASH, EQ>
  //   traits = TABLE::triats
  //   node_pointer = traits::node_base_pointer
  //
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full or movable
  //   ==
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type // no definition if no C::allocator_type
  //   get_allocator()
  //   explicit this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   key_type = traits::key_type
  //   mapped_type = traits::mapped_type // only for map/multimap
  //   hasher = HASH
  //   key_equal = EQ
  //   node_type
  //     special member functions: default-constructible, movable
  //     get_allocator()->allocator_type
  //     explicit operator bool()
  //     empty()->bool
  //     *()->non_const_ref
  //     value_type // only for set/multiset
  //     value()    // only for set/multiset
  //     key_type   // only for map/multimap
  //     mapped_type   // only for map/multimap
  //     key()         // only for map/multimap
  //     mapped()      // only for map/multimap
  //     get()->node_pointer
  //     release()->node_pointer
  //   insert_return_type // unique only
  //     iterator position
  //     bool inserted
  //     node_type node
  //   hash_function()->hasher
  //   key_eq()->key_equal
  //
  //   this_t(hf, eq, alloc)
  //   this_t(hf, eq)
  //   explicit this_t(n, hf = HASH{}, eq = EQ{}, alloc = alloc_t{})
  //   this_t(n, alloc)
  //   this_t(n, hf, alloc)
  //
  //   this_t(from, to, n = ..., hf = HASH{}, eql = EQ{}, alloc = alloc_t{})
  //   this_t(from, to, alloc)
  //   this_t(from, to, n, alloc)
  //   this_t(from, to, n, hf, alloc)
  //   assign(from, to)
  //
  //   this_t(il, n = ..., hf = HASH{}, eql = EQ{}, alloc = alloc_t{})
  //   this_t(il, alloc)
  //   this_t(il, n, alloc)
  //   this_t(il, n, hf, alloc)
  //   =(il)
  //   assign(il)
  //
  //   emplace(s...)->pair<iterator, bool> // unique only
  //   insert(x)->pair<iterator, bool> // unique only
  //   emplace(s...)->iterator // multi only
  //   insert(x)->iterator // multi only
  //   emplace_hint(hint, s...)->iterator
  //   insert(hint, x)->iterator
  //   insert(from, to)
  //   insert(il)
  //
  //   insert(node_type &&)->insert_return_type // unique only
  //   insert(node_type &&)->iterator
  //   insert(hint, node_type &&)
  //   extract(cit)->node_type
  //   extract(key)->node_type
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)->(const) mapped_type & // const version has no check
  //   at(key)->(const) mapped_type & // throw if key not existed
  //   ///
  //
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   erase(key)->size_type
  //   remove(key)->size_type
  //   remove_if(eq)->size_type
  //   clear()
  //
  //   merge(multi_or_non_multi_version)
  //
  //   find(key)->iterator
  //     // for multi-version, get the last among equal elements
  //   contains(key)->bool
  //   count(key)->size_type
  //   equal_range(key)->iter_pair<iterator>
  //
  //   bucket_count()->size_type
  //   active_bucket_count()->size_type
  //   max_bucket_count()->size_type
  //   bucket_size(size_type)->size_type
  //   bucket(const key_type &)->size_type
  //
  //   local_iterator
  //   const_local_iterator
  //   begin(size_type)->(const_)local_iterator
  //   end(size_type)->(const_)local_iterator
  //   cbegin(size_type)->const_local_iterator
  //   cend(size_type)->const_local_iterator
  //   bucket_range(size_type)->iter_pair<(const_)local_iterator>
  //
  //   load_factor()->float
  //   max_load_factor()->float
  //   max_load_factor(float)
  //   rehash(size_type min_bucket_c)
  //   reserve(size_type n)
  //     // guarantee no rehash occur if size not bigger than n
  //
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(const_iterator)->pair<iterator, bool> // unique only
  //   link(const_iterator)->iterator // multi only
  //   link(hint, const_iterator)->iterator
  //
  //   try_link(key, get_node, do_when_eq = ...)->pair<iterator, bool>
  //     // unique only
  //   try_link_hint(hint, key, get_node, do_when_eq = ...)->iterator
  //     // unique only
  //   try_link(key, get_node)->iterator
  //     // multi only
  //   try_link_hint(hint, key, get_node)->iterator
  //     // multi only
  //
  //   erase_or_unlink(const_iterator)->iterator
  //
  //   unique(eq = ...)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, n, hf = HASH{}, eq = EQ{}, alloc = alloc_t{})
  //   this_t(from_range, r, alloc)
  //   this_t(from_range, r, n, alloc)
  //   this_t(from_range, r, n, hf, alloc)
  //   explicit this_t(r)
  //   this_t(r, n, hf = HASH{}, eq = EQ{}, alloc = alloc_t{})
  //   this_t(r, alloc)
  //   this_t(r, n, alloc)
  //   this_t(r, n, hf, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(r)
  //   insert_range(r)
  //   append(s...)->this_t &

  // example for intrusive mode
  {
    struct t : join_hashtable<0>, join_hashtable<1> {
      int key = 0;
      int mapped = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x, int y) : key(x), mapped(y) {}
    };
    struct traits_t
      : public hashtable_traits<t, 0,
                                false, // STORE_NODE_ALLOCATOR = false
                                allocator<t> // allocator to provide types
                                > {
      using base_t = hashtable_traits<t, 0, false, allocator<t>>;
      using key_type = int;
      using mapped_type = int; // for set and multiset, no mapped_type
      static const key_type &key(const typename base_t::value_type &x) {
        return x.key;
      }
      // for set and multiset, no mapped()
      static mapped_type &mapped(typename base_t::value_type &x) {
        return x.mapped;
      }
      static
      const mapped_type &mapped(const typename base_t::value_type &x) {
        return x.mapped;
      }
    };
    using l_t = unordered_map_adaptor<hashtable_adaptor<traits_t>,
                                      hash<int>, equal_to<int>>;
    struct traits2_t
      : public hashtable_traits<t, 1, false, allocator<t>> {
      using base_t = hashtable_traits<t, 1, false, allocator<t>>;
      using key_type = int;
      using mapped_type = int;
        // for set and multiset, no mapped_type
      static const key_type &
      key(const typename base_t::value_type &x) {return x.key;}
      static mapped_type &
      mapped(typename base_t::value_type &x) {return x.mapped;}
      static const mapped_type &
      mapped(const typename base_t::value_type &x) {return x.mapped;}
    };
    using l2_t = unordered_map_adaptor<hashtable_adaptor<traits2_t>,
                                       hash<int>, equal_to<int>>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1, 1);
    t x2(in_place, 2, 2);
    t x3(in_place, 3, 3);

    l_t l1;
    l2_t l2;

    assert(l1.link(l_t::iterator(&x1)).second);
    assert(l1.link(l_t::iterator(&x2)).second);
    assert(l2.link(l2_t::iterator(&x2)).second);
    assert(l2.link(l2_t::iterator(&x3)).second);
    const auto get_value = [](const auto &x) {return pair(x.key, x.mapped);};
    assert(equal(bind_rng(l1, get_value), seq(pair(1, 1), pair(2, 2)))
           || equal(bind_rng(l1, get_value), seq(pair(2, 2), pair(1, 1))));
    assert(equal(bind_rng(l2, get_value), seq(pair(2, 2), pair(3, 3)))
           || equal(bind_rng(l2, get_value), seq(pair(3, 3), pair(2, 2))));
  }
}
void doc_unordered_map_typedef() {
  // htbt<T, AL>
  //   : public hashtable_traits<hashtable_node<T, alloc_void_ptr<AL>>,
  //                             0, 1, AL>
  // hsett<T, AL> : public htbt<T, AL>
  //   key_type = htbt<T, AL>::value_type
  //   static key(const htbt<T, AL>::value_type &)->const key_type &
  // hmapt<T, AL> : public htbt<T, AL>
  //   key_type = htbt<T, AL>::value_type::first_type
  //   key_type = htbt<T, AL>::value_type::second_type
  //   static key(const htbt<T, AL>::value_type &)->const key_type &
  //   static mapped(const htbt<T, AL>::value_type &)->mapped_type &
  //   static mapped(const htbt<T, AL>::value_type &)->const mapped_type &
  // hashtable<T, AL> = hashtable_adaptor<htbt<T, AL>>
  // unordered_set<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //               AL = default_allocator<KEY>>
  //   = unordered_set_adaptor<hashtable_adaptor<hsett<KEY, AL>>, HASH, EQ>
  // unordered_multiset<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                    AL = default_allocator<KEY>>
  //   = unordered_multiset_adaptor<hashtable_adaptor<hsett<KEY, AL>>, HASH, EQ>
  // unordered_map<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //               AL = default_allocator<pair<KEY, MAPPED>>>
  //   = unordered_map_adaptor<hashtable_adaptor
  //                           <hmapt<pair<KEY, MAPPED>, AL>>, HASH, EQ>
  // unordered_multimap<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                    AL = default_allocator<pair<KEY, MAPPED>>>
  //   = unordered_multimap_adaptor<hashtable_adaptor
  //                                <hmapt<pair<KEY, MAPPED>, AL>>, HASH, EQ>
}
void doc_mixed_map_adaptor() {
  // mixed_container_node<T, AL>
  // mixed_ranked_container_node<T, AL>
  // mxdrkdsett<T, AL>
  //   tree_traits 
  //   hashtable_traits
  //   tree
  //   hashtable
  // mxdrkdmapt<T, AL>
  //   tree_traits
  //   hashtable_traits
  //   tree
  //   hashtable
  //
  // mixed_set_adaptor<TRAITS, HASH, EQ, LESS>
  // mixed_map_adaptor<TRAITS, HASH, EQ, LESS>
  // mixed_multiset_adaptor<TRAITS, HASH, EQ, LESS>
  // mixed_multimap_adaptor<TRAITS, HASH, EQ, LESS>
  //   tree_iterator
  //   tree_const_iterator
  //   unordered_iterator
  //   unordered_const_iterator
  //
  //   static no_ed_unordered_iter(two_cit)->unordered_iterator
  //   static no_ed_tree_iter(two_cit)->tree_iterator
  //   static no_ed_unordered_iter(two_cit)->unordered_iterator
  //   static no_ed_tree_iter(two_cit)->tree_iterator
  //
  //   unordered_iter(two_cit)->unordered_iterator
  //   tree_iter(two_cit)->tree_iterator
  //   unordered_iter(two_cit)->unordered_iterator
  //   tree_iter(two_cit)->tree_iterator
  //
  //   traits = TRAITS
  //   node_pointer = TRAITS::tree::node_pointer
  //   tree_type = ..._adaptor<TRAITS::tree, LESS>
  //   unordered_type = unordered_..._adaptor<TRAITS::hashtable, HASH, EQ>
  //   tree()->(const) tree_type &
  //   unordered()->(const) unordered_type &
  //
  //   pointer = tree_type::pointer
  //   const_pointer = tree_type::const_pointer
  //   value_type = tree_type::value_type
  //   reference = tree_type::reference
  //   const_reference = tree_type::const_reference
  //   iterator = tree_type::iterator
  //   const_iterator = tree_type::const_iterator
  //   difference_type = tree_type::difference_type
  //   size_type = tree_type::size_type
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   explicit this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   key_type = tree_type::key_type
  //   mapped_type = tree_type::mapped_type // only for map and multimap
  //   key_compare = tree_type::key_compare
  //   value_compare = tree_type::key_compare
  //   key_comp()->key_compare
  //   value_comp()->value_compare
  //
  //   this_t(hf, eq, cmp, alloc = alloc_t{})
  //
  //   this_t(first, last, hf, eq, cmp, alloc = alloc_t{})
  //   this_t(first, last, alloc = alloc_t{})
  //   assign(first, last)
  //
  //   this_t(il, hf, eq, cmp, alloc = alloc_t{})
  //   this_t(il, alloc = alloc_t{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(s...)->pair<iterator, bool> // unique only
  //   emplace(s...)->iterator // multi only
  //   emplace_hint(hint, s...)->iterator
  //   insert(x)->pair<iterator, bool> // unique only
  //   insert(hint, x)->iterator // unique only
  //   insert(from, to)
  //   insert(il)
  //
  //   node_type = tree_t::node_type;
  //   insert_return_type // unique only
  //     iterator position
  //     bool inserted
  //     node_type node
  //   extract(const_iterator)->node_type
  //   extract(key)->node_type
  //   insert(node_type &&)->insert_return_type
  //   insert(const_iterator hint, node_type &&)->iterator
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)->(const) mapped_type & // const version has no check
  //   at(key)->(const) mapped_type & // throw if not found
  //   ///
  //
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   clear()
  //
  //   erase(key)->size_type
  //   remove(key)->size_type
  //   remove_if(eql)->size_type
  //
  //   merge(multi_or_non_multi_version)
  //
  //   find(key)->iterator
  //   contains(key)->bool
  //   count(key)->size_type
  //   lower_bound(key)->(const_)iterator
  //   upper_bound(key)->(const_)iterator
  //   equal_range(key)->iter_pair<(const_)iterator>
  //   partition_point(eql)->(const_)iterator
  //   find_range(min, max)->iter_pair<(const_)iterator>
  //
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(const_iterator)->pair<iterator, bool> // unique only
  //   link(const_iterator)->iterator // multi only
  //   link(const_iterator hint, const_iterator)->iterator
  //
  //   try_link(key, get_node, do_when_eq = ...)->pair<iterator, bool>
  //     // unique only // get_node()->iterator, do_when_eq(iterator)
  //   try_link_hint(hint, key, get_node,
  //                 do_when_eq = ...)->iterator
  //     // unique only // get_node()->iterator, do_when_eq(iterator)
  //   try_link(key, get_node)->iterator
  //     // multi only // get_node()->iterator
  //   try_link_hint(hint, key, get_node)->iterator
  //     // multi only // get_node()->iterator
  //
  //   front()->(const_)reference
  //   back()->(const_)reference
  //
  //   root()->iterator
  //   leftmost()->iterator
  //   rightmost()->iterator
  //
  //   nth(difference_type)->(const_)iterator // ranked_rb_tree only
  //   nth(const_iterator)->iterator // ranked_rb_tree only
  //
  //   erase_or_unlink(const_iterator)->iterator
  //
  //   unique()
  //   unique(eql)
  //
  //   hasher = HASH
  //   key_equal = EQ
  //   hash_function()->hasher
  //   key_eq()->key_equal
  //   bucket_count()->size_type
  //   max_bucket_count()->size_type
  //   active_bucket_count()->size_type
  //   bucket_size(size_type)->size_type
  //   bucket_range(size_type)->iter_pair<(const_)local_iterator>
  //   bucket(key)
  //   load_factor()->float
  //   max_load_factor()->float
  //   max_load_factor(f)->float
  //   rehash(size_type min_bucket_c)
  //   reserve(size_type n)
  //     // guarantee no rehash occur if size not bigger than n
  //   local_iterator
  //   const_local_iterator
  //   begin(size_type)->(const_)local_iterator
  //   end(size_type)->(const_)local_iterator
  //   cbegin(size_type)->const_local_iterator
  //   cend(size_type)->const_local_iterator
  //
  //   this_t(from_range, r, alloc = alloc_t{})
  //   this_t(from_range, r, hf, eq, cmp, alloc = alloc_t{})
  //   explicit this_t(r)
  //   this_t(r, hf, eq, cmp, alloc = alloc_t{})
  //   this_t(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(r)
  //   insert_range(r)
  //   append(s...)->this_t &

  // // intrusive mode for mixed container is not encouraged
}
void doc_mixed_map_typedef() {
  // mixed_set<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>, LESS = less<KEY>,
  //           AL = default_allocator<KEY>>
  //   = mixed_set_adaptor<mxdsett<KEY, AL>, HASH, EQ, LESS>
  // mixed_multiset<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>, LESS = less<KEY>,
  //                AL = default_allocator<KEY>>
  //   = mixed_multiset_adaptor<mxdsett<KEY, AL>, HASH, EQ, LESS>
  // mixed_map<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //           LESS = less<KEY>, AL = default_allocator<pair<KEY, MAPPED>>>
  //   = mixed_map_adaptor<mxdmapt<pair<KEY, MAPPED>, AL>, HASH, EQ, LESS>
  // mixed_multimap<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                LESS = less<KEY>, AL = default_allocator<pair<KEY, MAPPED>>>
  //   = mixed_multimap_adaptor<mxdmapt<pair<KEY, MAPPED>, AL>, HASH, EQ, LESS>
  //
  // mixed_ranked_set<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                  LESS = less<KEY>, AL = default_allocator<KEY>>
  //   = mixed_set_adaptor<mxdrkdsett<KEY, AL>, HASH, EQ, LESS>
  // mixed_ranked_multiset<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                       LESS = less<KEY>, AL = default_allocator<KEY>>
  //   = mixed_multiset_adaptor<mxdrkdsett<KEY, AL>, HASH, EQ, LESS>
  // mixed_ranked_map<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                  LESS = less<KEY>,
  //                  AL = default_allocator<pair<KEY, MAPPED>>>
  //   = mixed_map_adaptor<mxdrkdmapt<pair<KEY, MAPPED>, AL>, HASH, EQ, LESS>
  // mixed_ranked_multimap<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                       LESS = less<KEY>,
  //                       AL = default_allocator<pair<KEY, MAPPED>>>
  //   = mixed_multimap_adaptor<mxdrkdmapt<pair<KEY, MAPPED>, AL>,
  //                            HASH, EQ, LESS>
}
void doc_stable_vector_adaptor() {
  // join_stable_vector<ID, VOID_PTR = void *>
  //   ptr_rebind<VOID_PTR, ptr_rebind<VOID_PTR, join_stable_vector>> iter
  // stable_vector_node<T, AL>
  //   alignas(T) byte data[sizeof(T)]
  //   *()->T &
  //   ->()->T *
  // stable_vector_traits<NODE_T, ID = 0,
  //                      STORE_NODE_ALLOCATOR = false,
  //                      AL = default_allocator<NODE_T>>
  //   value_type = alloc_vt<AL>
  //   allocator_type = AL
  //   node_type = NODE_T
  //   node_pointer = alloc_rebind_ptr<AL, node_type>
  //   node_base_type = join_stable_vector<ID, alloc_void_ptr<AL>>
  //   node_base_pointer = alloc_rebind_ptr<AL, node_base_type>
  //   difference_type = alloc_dft<AL>
  //   size_type = alloc_szt<AL>
  //
  //   id = size_constant<ID>
  //   store_node_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_allocator = true_type
  //
  //   node_base_pointer_pointer = alloc_rebind_ptr<AL, node_base_pointer>
  //   static index_iter(node_base_pointer)->node_base_pointer_pointer
  //   static index_iter(node_base_pointer, node_base_pointer_pointer)
  //
  //   static data(node_base_pointer)->value_type *
  //   new_node(AL &, s...)->node_base_pointer
  //   delete_node(AL &, node_base_pointer)
  //
  // stable_vector_adaptor<TRAITS, size_t LIM = 0>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //     explicit iterator(TRAITS::node_base_pointer)
  //     node()->TRAITS::node_base_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
  //   difference_type
  //   size_type
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   special member functions: full or movable
  //   == <=>
  //
  //   traits = TRAITS
  //   node_pointer = TRAITS::node_base_pointer
  //   new_node(s...)->iterator
  //   delete_node(const_iterator)
  //   exchange_node(const_iterator, const_iterator)->iterator
  //   link(const_iterator, const_iterator)->iterator
  //   link_back(const_iterator)->iterator
  //   unlink(const_iterator)->iterator
  //   unlink(const_iterator, const_iterator)->iterator
  //   unlink()
  //   unlink_back()->iterator
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   explicit this_t(n, alloc = alloc_t{})
  //   this_t(n, x, alloc = alloc_t{})
  //   assign(n, x)
  //   this_t(from, to, alloc = alloc_t{})
  //   assign(from, to)
  //   this_t(il, alloc = alloc_t{})
  //   =(il)
  //   assign(il)
  //
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, n, x)
  //   insert(cit, from, to)
  //   insert(cit, il)
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   capacity()->size_type
  //   resize(size_type)
  //   resize(size_type, const value_type &)
  //   reserve(size_type) // not for limited_stable_vector
  //   reserve_more(size_type) // not for limited_stable_vector
  //   shrink_to_fit() // not for limited_stable_vector
  //   splice(const_iterator, analoguous_ref)
  //   splice(const_iterator, analoguous_ref, const_iterator)
  //   splice(const_iterator, analoguous_ref, const_iterator, const_iterator)
  //   swap(const_iterator, const_iterator)
  //   swap(const_iterator, analoguous_ref, const_iterator)
  //   erase_or_unlink(const_iterator)
  //
  //   remove_if(eq)->size_type
  //   remove(x)->size_type
  //   unique(eq)
  //   unique()
  //   merge(analoguous_ref, less_f = less{})
  //   reverse()
  //   sort(less_f = less{})
  //
  //   full()->bool
  //   reallocate(size_type n = size()) // not for limited_stable_vector
  //   reserve_more(size_type) // not for limited_stable_vector
  //   replace(const_iterator, const_iterator, r)->iterator
  //
  //   node_type
  //     special member functions: default-constructible, movable
  //     get_allocator()->allocator_type
  //     explicit operator bool()
  //     empty()->bool
  //     get()->node_pointer
  //     release()->node_pointer
  //     *()->value_type &
  //     clear()
  //   make_node(s...)->node_type
  //   extract(const_iterator)->node_type
  //   insert(const_iterator, node_type &&)->iterator
  //   push_back(node_type &&)
  //   exchange(const_iterator, node_type &)->node_type
  //   exchange(const_iterator, node_type &&)->node_type
  //   replace(const_iterator, node_type &&)->iterator
  //
  //   extract(const_iterator, const_iterator)->this_t
  //   insert(const_iterator, this_t &&)->iterator
  //   push_back(this_t &&)
  //   exchange(const_iterator, const_iterator, this_t &)->this_t
  //   exchange(const_iterator, const_iterator, this_t &&)->this_t
  //   replace(const_iterator, const_iterator, this_t &&)->iterator
  //
  //   this_t(from_range, r, alloc = alloc_t{})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(const_iterator, r)
  //   insert_range(const_iterator, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(size_type n)
  //   append(s...)->this_t &

  // example for intrusive mode
  {
    struct t : join_stable_vector<0>, join_stable_vector<1> {
      int value = 0;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;

      t(in_place_t, int x) : value(x) {}
    };
    using traits_t
      = stable_vector_traits<t, 0,
                             false, // STORE_NODE_ALLOCATOR = false
                             allocator<t> // allocator to provide types
                             >;
    using l_t = stable_vector_adaptor<traits_t>;
    using traits2_t = stable_vector_traits<t, 1, false, allocator<t>>;
    using l2_t = stable_vector_adaptor<traits2_t>;
    static_assert(is_same<l_t::value_type, t>);
    static_assert(is_same<l2_t::value_type, t>);

    t x1(in_place, 1);
    t x2(in_place, 2);
    l_t l1;
    l2_t l2;

    l1.link(l1.end(), l_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x1));
    l2.link(l2.end(), l2_t::iterator(&x2));
    const auto get_value = [](const auto &x) {return x.value;};
    assert(equal(bind_rng(l1, get_value), seq(1)));
    assert(equal(bind_rng(l2, get_value), seq(1, 2)));
  }
}
void doc_stable_vector_typedef() {
  // stbvt<T, AL>
  //   : public stable_vector_traits<stable_vector_node<T, AL>, 0, 1, AL>
  // stable_vector<T, AL = default_allocator<T>>
  //   = stable_vector_adaptor<stbvt<T, AL>>
  // limited_stable_vector<T, N, AL = default_allocator<T>>
  //   = stable_vector_adaptor<stbvt<T, AL>, N>
}
void doc_tree_adaptor() {
  // tree_adaptor<TRAITS>
  //   // provide no intrusive mode, always own all nodes in *this
  //
  //   tree_type
  //   vector_type
  //
  //   key_type
  //   children_type
  //   tree_node_type
  //   value_type
  //     special member functions: none
  //     value_type reference const_reference different_type size_type
  //     iterator const_iterator
  //     begin() end() cbegin() cend()
  //     max_size() size() empty()
  //     reverse_iterator const_reverse_iterator
  //     rbegin() rend() crbegin() crend()
  //     front() back() [](size_type)
  //     nth(difference_type)->(const_)iterator
  //     key_type
  //     key()->(const) key_type &
  //     *()->(const) key_type &
  //     ->()->(const) key_type *
  //     parent()->(const_)iterator
  //     iter()->(const_)iterator
  //     children_type
  //     children()->(const) children_type &
  //   reference
  //   const_reference
  //
  //   iterator
  //     iterator category: dependent
  //     ==(nullptr)
  //     explicit this_t(underlying_iter_t)
  //     base()->underlying_iter_t
  //     node()->decltype(base().node())
  //     to_mutable()->decltype(base().to_mutable())
  //     parent()->this_t // *this != nullptr, note: end node can call parent
  //     children_type
  //     children()->(const) children_type &
  //     tree_node_type
  //     tree_node()->(const) tree_node_type &
  //       // same as operator * for normal case,
  //       //   but dereference action is controlled by the traits type
  //
  //     pre_order_iterator
  //     post_order_iterator
  //     in_order_iterator
  //       // common description for
  //       //   pre_order_iterator, post_order_iterator, and in_order_iterator
  //       iterator category: bidirectional
  //       // note: reverse sequence of last order is first order of tree with
  //       //   reverse children for all nodes
  //       // bidirectional
  //       // prev(begin) == end
  //       // begin == next(end)
  //       root()->iterator // const_iteartor for const_iterator
  //       base()->iterator // const_iteartor for const_iterator
  //       nth()->itr_dft<iterator> // only for in_order_iterator
  //         // const_iteartor for const_iterator
  //       removed_next()->this_t
  //         // *this is dereferenceable, only for pre_order_iterator
  //       advance()->difference_type
  //         // return relative depth to *this, only for pre_order_iterator
  //     pre_order_begin()->pre_order_iterator
  //     pre_order_end()->pre_order_iterator
  //     pre_order()->iter_pair<pre_order_iterator>
  //     post_order_begin()->post_order_iterator
  //     post_order_end()->post_order_iterator
  //     post_order()->iter_pair<post_order_iterator>
  //     in_order_begin(difference_type)->in_order_iterator
  //     in_order_end(difference_type)->in_order_iterator
  //     in_order(difference_type)->iter_pair<in_order_iterator>
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   root()->(const_)iterator // get null if empty
  //   croot()->const_iterator // get null if empty
  //
  //   empty()->bool
  //
  //   special member functions: full
  //   ==
  //
  //   allocator_type
  //   get_allocator()
  //
  //   tree_adaptor(a)
  //   tree_adaptor(const this_t &, a)
  //   tree_adaptor(this_t &&, a)
  //
  //   explicit tree_adaptor(const tree_adaptor<T2> &, a = a_t{})
  //   =(const tree_adaptor<T2> &)
  //
  //   explicit tree_adaptor(tree_adaptor<T2> &&, a = a_t{})
  //   =(tree_adaptor<T2> &&)
  //
  //   explicit tree_adaptor(in_place, s...)
  //   explicit tree_adaptor(in_place_type<U>, s...)
  //   explicit tree_adaptor(const key_type &, this_rrefs...)
  //     // all trees have equal allocators
  //   explicit tree_adaptor(key_type &&, this_rrefs...)
  //     // all trees have equal allocators
  //   explicit tree_adaptor(allocator_arg, a, in_place, s...)
  //   explicit tree_adaptor(allocator_arg, a, in_place_type<U>, s...)
  //   explicit tree_adaptor(allocator_arg, a, const key_type &, s...)
  //   explicit tree_adaptor(allocator_arg, a, key_type &&, s...)
  //   static maker()->[...](T &&, S &&...)->this_t
  //   static maker_with_allocator(a)->[...](T &&, S &&...)->this_t
  //
  //   copy(it, get_key = deref)->this_t // it belongs to *this
  //   copy(tree/vec_ref, it, get_key)->this_t
  //   emplace(s...)->reference
  //   clear()
  //
  //   swap(i, j) // sub-tree of i and j share no node
  //   swap(i, tree/vec_ref, j) // sub-tree of i and j share no node
  //
  //   extract(i)->tree_type
  //   insert(next, tree_type &&)->iterator
  //   exchange(i, tree_type &&)->tree_type
  //   replace(i, tree_type &&)->iterator
  //
  //   extract(i1, i2)->vector_type
  //   insert(i, vector_type &&)->iterator
  //   exchange(i1, i2, vector_type &&)->vector_type
  //   replace(i1, i2, vector_type &&)
  //
  //   extract_children(i)->vector_type
  //   exchange_children(i, vector_type &&)->vector_type
  //   replace_children(i, vector_type &&)->iterator
  //
  //   splice(next, tree/vector_ref, i)
  //   splice(next, tree/vector_ref, i, i2)
  //   splice(next, vector_ref)
  //
  //   emplace(i, s...)->iterator
  //   insert(i, k)->iterator
  //   insert(i, n, k)->iterator
  //   insert(i, from, to)->iterator
  //   insert(i, l)->iterator
  //   insert(i, r)->iterator
  //   insert_range(i, r)->iterator
  //
  //   erase(i)->iterator
  //   erase(i1, i2)->iterator
  //   clear(i)
  //
  //   emplace_back(i, s...)
  //   push_back(i, k)
  //   push_back(i, tree_ref)
  //   push_back(i, vector_ref)
  //   push_back(i, kr)
  //   append_range(i, r)
  //   append(i, s...)
  //   pop_back(i)
  //   pop_back(i, n)
  //
  //   emplace_front(i, s...)->reference
  //   push_front(i, k)
  //   push_front(i, tree_ref)
  //   push_front(i, vector_ref)
  //   push_front(i, kr)
  //   prepend_range(i, r)
  //   prepend(i, s...)
  //   pop_front(i)
  //   pop_front(i, n)
  //
  //   capacity(i)->size_type
  //   full(i)->bool
  //   reserve(i, n)
  //   pre_order_reserve(i, n);
  //   reserve_more(i, n)
  //   pre_order_reserve_more(i, n);
  //
  //   resize(i, n, x = ...)
  //   pre_order_resize(i, n, x = ...)
  //
  //   shrink_to_fit(i)
  //   pre_order_shrink_to_fit(i)
  //
  //   remove_if(i, eq)->size_type
  //   pre_order_remove_if(i, eq)
  //
  //   unique(i, eq)
  //   pre_order_unique(i, eq)
  //
  //   merge(i, tree/vec_ref, i2, less)
  //   merge(i, vec_ref, less)
  //
  //   sort(i, iter_less)
  //   pre_order_sort(i, less)
  //
  // private:
  //   good()->bool

  // make a new tree
  {
    // empty tree
    {
      tree<int> t;
      assert(t.root() == nullptr);
    }
    // one node tree
    {
      tree<int> t(in_place, 1);
      assert(t.root()->key() == 1);
      assert(t.root().children().empty());
    }
    // manually list all nodes
    {
      tree<int> t(1,
                  tree<int>(2,
                            tree<int>(3),
                            tree<int>(4)),
                  tree<int>(5));

      // for stateful allocator, specify the allocator arg to all nodes
      stateful_test_allocator<int> a;
      using tree_t = tree<int, stateful_test_allocator<int>>;
      auto mk = tree_t::maker();
      tree_t t2 = mk(1,
                     mk(2,
                        mk(3),
                        mk(4)),
                     mk(5));
      auto mk2 = tree_t::maker_with_allocator(a);
      tree_t t3 = mk2(1,
                     mk2(2,
                        mk2(3),
                        mk2(4)),
                      mk2(5));
        // initialization of t3 is faster than t2
        //   by reducing times to allocator node
      assert(t2 == t);
      assert(t3 == t);
      assert(t2.get_allocator() != t3.get_allocator());
    }
    // from another tree
    {
      const tree<int> t(in_place, 1);
      const tree<int> t2 = t;
      assert(t2 == t);
      const tree<float> t3(t); // need explicit
      assert(t3.root() != nullptr && t3.root()->key() == 1.0f);
    }
    // copy by customized node-copy function
    {
      tree<int> t(1,
                  tree<int>(2,
                            tree<int>(3),
                            tree<int>(4)));
      tree<int> t2 = t.copy(t.root(), [](const auto &x) {return x.key() * 2;});
      assert(t2 == tree<int>(2,
                             tree<int>(4,
                                       tree<int>(6),
                                       tree<int>(8))));
    }
  }
  // traverse
  {
    tree<int> t;
    assert(t.root() == nullptr);
    assert(empty(t.root().pre_order()));
    t.emplace(1);
    t.insert(t.root()->end(), 2);
    t.insert(t.root()->end(), 3);
    assert(empty(t.root()->end().pre_order()));
    assert(equal(bind_rng(t.root().pre_order(), deref),
                 seq(1, 2, 3)));
  }
}
void doc_tree_vector_adaptor() {
  // tree_vector_adaptor<TRAITS>
  //   // provide no intrusive mode, always own all nodes in *this
  //
  //   tree_type
  //   vector_type
  //
  //   key_type
  //   children_type
  //   tree_node_type
  //   value_type
  //     special member functions: none
  //     value_type reference const_reference different_type size_type
  //     iterator const_iterator
  //     begin() end() cbegin() cend()
  //     max_size() size() empty()
  //     reverse_iterator const_reverse_iterator
  //     rbegin() rend() crbegin() crend()
  //     front() back() [](size_type)
  //     nth(difference_type)->(const_)iterator
  //     key_type
  //     key()->(const) key_type &
  //     *()->(const) key_type &
  //     ->()->(const) key_type *
  //     parent()->(const_)iterator
  //     iter()->(const_)iterator
  //     children_type
  //     children()->(const) children_type &
  //   reference
  //   const_reference
  //
  //   iterator
  //     iterator category: dependent
  //     ==(nullptr)
  //     explicit this_t(underlying_iter_t)
  //     base()->underlying_iter_t
  //     node()->decltype(base().node())
  //     to_mutable()->decltype(base().to_mutable())
  //     parent()->this_t // *this != nullptr, note: end node can call parent
  //     children_type
  //     children()->(const) children_type &
  //     tree_node_type
  //     tree_node()->(const) tree_node_type &
  //       // same as operator * for normal case,
  //       //   but dereference action is controlled by the traits type
  //
  //     pre_order_iterator
  //     post_order_iterator
  //     in_order_iterator
  //       // common description for
  //       //   pre_order_iterator, post_order_iterator, and in_order_iterator
  //       iterator category: bidirectional
  //       // note: reverse sequence of last order is first order of tree with
  //       //   reverse children for all nodes
  //       // bidirectional
  //       // prev(begin) == end
  //       // begin == next(end)
  //       root()->iterator // const_iteartor for const_iterator
  //       base()->iterator // const_iteartor for const_iterator
  //       nth()->itr_dft<iterator> // only for in_order_iterator
  //         // const_iteartor for const_iterator
  //       removed_next()->this_t
  //         // *this is dereferenceable, only for pre_order_iterator
  //       advance()->difference_type
  //         // return relative depth to *this, only for pre_order_iterator
  //     pre_order_begin()->pre_order_iterator
  //     pre_order_end()->pre_order_iterator
  //     pre_order()->iter_pair<pre_order_iterator>
  //     post_order_begin()->post_order_iterator
  //     post_order_end()->post_order_iterator
  //     post_order()->iter_pair<post_order_iterator>
  //     in_order_begin(difference_type)->in_order_iterator
  //     in_order_end(difference_type)->in_order_iterator
  //     in_order(difference_type)->iter_pair<in_order_iterator>
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   max_size()
  //   size()
  //   empty()
  //   []
  //   nth(n)
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   special member functions: full
  //   ==
  //   == with nullptr
  //
  //   allocator_type
  //   get_allocator()
  //
  //   tree_vector_adaptor(a)
  //   tree_vector_adaptor(const this_t &, a)
  //   tree_vector_adaptor(this_t &&, a)
  //
  //   explicit tree_vector_adaptor(const tree_vector_adaptor<T2> &, a = a_t{})
  //   =(const tree_vector_adaptor<T2> &)
  //
  //   explicit tree_vector_adaptor(tree_vector_adaptor<T2> &&, a = a_t{})
  //   =(tree_vector_adaptor<T2> &&)
  //
  //   tree_vector_adaptor(n, a = a_t{})
  //   tree_vector_adaptor(n, k, a = a_t{})
  //   assign(n, k)
  //
  //   tree_vector_adaptor(from, to, a = a_t{})
  //   assign(from, to)
  //
  //   tree_vector_adaptor(il, a = a_t{})
  //   =(il)
  //   assign(il)
  //
  //   tree_vector_adaptor(r, a = a_t{})
  //   assign(r)
  //   tree_vector_adaptor(from_range, r, a = a_t{})
  //   assign_range(r)
  //
  //   explicit tree_vector_adaptor(tree_type_rref...)
  //   explicit tree_vector_adaptor(allocator_arg, a, tree_type_rref...)
  //
  //   copy(it, get_key = deref)->tree_type // it belongs to *this
  //   copy(tree/vec_ref, it, get_key)->tree_type
  //
  //   clear()
  //
  //   swap(i, j)
  //   swap(i, tree/vec_ref, j)
  //
  //   extract(i)->tree_type
  //   insert(next, tree_type &&)->iterator
  //   exchange(i, tree_type &&)->tree_type
  //   replace(i, tree_type &&)->iterator
  //
  //   extract(i1, i2)->vector_type
  //   insert(i, vector_type &&)->iterator
  //   exchange(i1, i2, vector_type &&)->vector_type
  //   replace(i1, i2, vector_type &&)
  //
  //   extract_children(i)->vector_type
  //   exchange_children(i, vector_type &&)->vector_type
  //   replace_children(i, vector_type &&)->iterator
  //
  //   splice(next, tree/vector_ref, i)
  //   splice(next, tree/vector_ref, i, i2)
  //   splice(next, vector_ref)
  //
  //   emplace(i, s...)->iterator
  //   insert(i, k)->iterator
  //   insert(i, n, k)->iterator
  //   insert(i, from, to)->iterator
  //   insert(i, l)->iterator
  //   insert(i, r)->iterator
  //   insert_range(i, r)->iterator
  //
  //   erase(i)->iterator
  //   erase(i1, i2)->iterator
  //   clear(i)
  //
  //   emplace_back(s...)
  //   push_back(k)
  //   push_back(tree_ref)
  //   push_back(vector_ref)
  //   push_back(kr)
  //   append_range(r)
  //   append(s...)
  //   pop_back()
  //   pop_back(n)
  //   emplace_back(i, s...)->iterator
  //   push_back(i, k)
  //   push_back(i, tree_ref)
  //   push_back(i, vector_ref)
  //   push_back(i, kr)
  //   append_range(i, r)
  //   append(i, s...)
  //   pop_back(i)
  //   pop_back(i, n)
  //
  //   emplace_front(s...)
  //   push_front(k)
  //   push_front(tree_ref)
  //   push_front(vector_ref)
  //   push_front(kr)
  //   append_range(r)
  //   append(s...)
  //   pop_front()
  //   pop_front(n)
  //   emplace_front(i, s...)->iterator
  //   push_front(i, k)
  //   push_front(i, tree_ref)
  //   push_front(i, vector_ref)
  //   push_front(i, kr)
  //   append_range(i, r)
  //   append(i, s...)
  //   pop_front(i)
  //   pop_front(i, n)
  //
  //   capacity()
  //   capacity(i)
  //   full()
  //   full(i)
  //
  //   reserve(n)
  //   reserve(i, n)
  //   pre_order_reserve(n)
  //   pre_order_reserve(i, n)
  //   reserve_more(n)
  //   reserve_more(i, n)
  //   pre_order_reserve_more(n)
  //   pre_order_reserve_more(i, n)
  //
  //   resize(n, x = ...)
  //   resize(i, n, x = ...)
  //   pre_order_resize(n, x = ...)
  //   pre_order_resize(i, n, x = ...)
  //
  //   shrink_to_fit()
  //   shrink_to_fit(i)
  //   pre_order_shrink_to_fit()
  //   pre_order_shrink_to_fit(i)
  //
  //   remove_if(eq)->size_type
  //   remove_if(i, eq)->size_type
  //   pre_order_remove_if(eq)
  //   pre_order_remove_if(i, eq)
  //
  //   unique(eq)
  //   unique(i, eq)
  //   pre_order_unique(eq)
  //   pre_order_unique(i, eq)
  //
  //   merge(tree/vec_ref, i2, less)
  //   merge(i, tree/vec_ref, i2, less)
  //   merge(vec_ref, less)
  //   merge(i, vec_ref, less)
  //
  //   sort(less)
  //   sort(i, less)
  //   pre_order_sort(less)
  //   pre_order_sort(i, less)
  //
  // private:
  //   good()->bool
}
void doc_tree_typedef() {
  // tree<T, AL = default_allocator<T>>
  // tree_vector<T, AL = default_allocator<T>>
  //
  // limited_tree<T, N, AL = default_allocator<T>>
  // limited_tree_vector<T, N, AL = default_allocator<T>>
  //
  // linked_tree<T, AL = default_allocator<T>>
  // linked_tree_vector<T, AL = default_allocator<T>>
  //
  // logn_tree<T, AL = default_allocator<T>>
  // logn_tree_vector<T, AL = default_allocator<T>>

  // dynamic_tree<T, TRAITS = inner::dtt>
  //   // same as tree<dynamic<T>> but iterator straightly dereference to T
  //   // there is no guard for null iterator, so do not dereference them
  // dynamic_tree_vector<T, TRAITS = inner::dtt>
  // linked_dynamic_tree<T, TRAITS = inner::dtt>
  // linked_dynamic_tree_vector<T, TRAITS = inner::dtt>
  // logn_dynamic_tree<T, TRAITS = inner::dtt>
  // logn_dynamic_tree_vector<T, TRAITS = inner::dtt>
}
void doc_matrix() {
  // matrix<T, C = vector<T>>
  //
  // is_matrix<T> // true if T is matrix<T, C> with optional cv-qualifier
  //
  // matrix<T, C>
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //
  //   range_type = iter_pair<C::iterator>
  //   const_range_type = iter_pair<C::const_iterator>
  //   range()->iter_pair<C::(const_)iterator>
  //
  //   empty()->bool // width() == 0 || height() == 0
  //
  //   width()->int
  //   height()->int
  //
  //   special member functions: full
  //
  //   allocator_type // dependent
  //   get_allocator() // dependent
  //
  //   matrix(a) // dependent
  //   matrix(m, a) // dependent
  //   matrix(m_rv, a) // dependent
  //
  //   matrix(w, h)
  //   matrix(w, h, a) // dependent
  //
  //   matrix(w, h, v)
  //   matrix(w, h, v, a) // dependent
  //
  //   matrix(w, h, r)
  //   matrix(w, h, r, a) // dependent
  //
  //   matrix(m2_lv_or_rv)
  //   matrix(m2, a) // dependent
  //   =(m2)
  //
  //   matrix(w, h, m2)
  //   matrix(w, h, m2, a)
  //   matrix(w, h, m2, fll)
  //   matrix(w, h, m2, fll, a)
  //   assign(w, h, m2)
  //   assign(w, h, m2, fll)
  //
  //   clear()
  //   resize(w, h, fll = value_type{})
  //   reset(w, h, fll = value_type{})
  //
  //   row(n)->auto
  //   rows()->auto
  //   column(n)->auto
  //   columns()->auto
  //   sub_range(x, y, w, h) // overflowed part are removed
  //   fill(x, y, w, h, value)
  //     // rectangle of (x, y)-(w, h) belong to *this or empty rectangle
  //   fill(value)
  //
  //   left_top()->(const_)reference
  //   left_bottom()->(const_)reference
  //   right_top()->(const_)reference
  //   right_bottom()->(const_)reference
  //   ()(x, y)->(const_)reference
  //   iter(x, y)->(const_)iterator
  //   includes_point(x, y)
  //
  //   capacity()->size_t
  //   reserve(size_t)
  //   reserve_more(size_t)
  //   shrink_to_fit()
  //
  //   swap(it, it2)
  //   swap(it, m, it2)
  //   replace(it, it2, m_rv)
  //   exchange(it, it2, m_rv)->matrix // returned matrix has 1 height
  //
  //   +()->this_t
  //   -()->this_t
  //   +(m)->this_t
  //   +=(m)->this_t &
  //   -(m)->this_t
  //   -=(m)->this_t &
  //   *(k)->this_t
  //   *=(k)->this_t &
  //   friend *(k, const this_t &)
  //   /(k)->this_t
  //   /=(k)->this_t &
  //
  //   *(m)->this_t
  //
  //   sprint(f = []<class U>(U &&x) {...})->string
  //     // a repeating call of f(element_ref) can not make a different effect
  //     // need re/io.h
  //
  //   make_echelon(double zero_threshold = 0.0001)->matrix // T == double
  //   sprint_solutions_of_linear_equations(size_t precision = 5,
  //                                        double zero_threshold = 0.000001)
  //     ->string
  //     // need re/io.h

  matrix<double> m(3, 3, seq(1.0, 2, 3,
                             1, 3, 5,
                             1, 2, 4));
  assert(m.make_echelon()
         == matrix<double>(3, 3, seq(1.0, 0, -1,
                                     0, 1, 2,
                                     0, 0, 1)));
  // for every row, make_echelon() try to make all elements
  //   except the corresponding 1.0 number and the rightmost number zero
  //   to get solutions of linear equations. it is not just echelon

  assert(m.sprint_solutions_of_linear_equations() == "no solution");
    // because the third row showed x0 * 0 + x2 * 0 = 1

  m = matrix<double>(3, 3, seq(1.0, 2, 3,
                               1, 3, 5,
                               1, 2, 3));
  // x0 + 2 * x1 = 3
  // x0 + 3 * x1 = 5
  // x0 + 2 * x1 = 3
  assert(m.make_echelon()
         == matrix<double>(3, 3, seq(1.0, 0, -1,
                                     0, 1, 2,
                                     0, 0, 0)));
  // x0 = -1
  // x1 = 2
  assert(m.sprint_solutions_of_linear_equations()
         == "x0 = -1.00000\nx1 = 2.00000\n");
}
void doc_dup_compressed_array() {
  // dup_compressed_array<T, AL = default_allocator<T>>
  //   stored_value_type = pair<T, uint>;
  //   stored_range()->rng_of_stored_value_type
  //
  //   value_type = T
  //   reference = value_type
  //   const_reference = value_type
  //
  //   iterator
  //     value_type = T
  //     pointer = void
  //     reference = value_type
  //     difference_type
  //     iterator_category = bidirectional_iterator_tag
  //     special member functions: full
  //     == <=>
  //     *
  //     ++
  //     --
  //   const_iterator = iterator
  //   difference_type
  //   size_type
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //
  //   max_size()
  //   empty()
  //
  //   special member functions: full
  //   == <=>
  //
  //   reverse_iterator
  //   const_reverse_iterator = reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   allocator_type = AL
  //   get_allocator()
  //   explicit dup_compressed_array(alloc)
  //   dup_compressed_array(const this_t &, alloc)
  //   dup_compressed_array(this_t &&, alloc)
  //
  //   explicit dup_compressed_array(n, alloc = alloc_t{})
  //   dup_compressed_array(n, value_type, alloc = alloc_t{})
  //   assign(n, value_type)
  //
  //   dup_compressed_array(from, to, alloc = alloc_t{})
  //   assign(from, to)
  //
  //   dup_compressed_array(il, alloc = alloc_t{})
  //   =(il)
  //   assign(il)
  //
  //   insert(iterator, value_type)->iterator
  //   insert(iterator, n, value_type)->iterator
  //   insert(iterator, from, to)->iterator
  //   insert(iterator, il)->iterator
  //   erase(iterator)->iterator
  //   erase(iterator, iterator)->iterator
  //   clear()
  //
  //   front()->value_type
  //   back()->value_type
  //   push_back(value_type)
  //   pop_back()
  //
  //   compressed_size()->size_type // return inner size of the inner vector
  //   capacity()->size_type
  //   full()->bool
  //   reserve(n)
  //   reserve_more(n)
  //   shrink_to_fit()
  //   reallocate(n = 0)
  //   replace(iterator, iterator, r)->iterator
  //
  //   explicit dup_compressed_array(const dup_compressed_array<T2, AL2> &)
  //     requires (!is_same<T, T2> && is_constructible<T, const T2 &>)
  //   dup_compressed_array(const dup_compressed_array<T2, AL2> &,
  //                        f_cast_T2_to_T)
  //     // f_cast_T2_to_T(const T2 &)->T
  //
  //   dup_compressed_array(from_range, r, alloc = alloc_t{})
  //   explicit dup_compressed_array(r)
  //   dup_compressed_array(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //
  //   insert(iterator, r)->iterator
  //   insert_range(iterator, r)->iterator
  //
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //
  //   append(s...)->this_t &

  dup_compressed_array<int> v = {1, 1, 1, 2, 3};
  assert(equal(v.stored_range(), seq(pair(1, 3u), pair(2, 1u), pair(3, 1u))));
  assert(equal(v, seq(1, 1, 1, 2, 3)));
}

void doc_duration() {
  // treat_as_floating_point<rep> : false_type
  // treat_as_floating_point<float> : true_type
  // treat_as_floating_point<float> : true_type
  //
  // duration_values<R>
  //   static zero()->R
  //   static min()->R
  //   static max()->R
  //
  // duration<R, ratio P>
  //   rep = R
  //   period = P
  //
  //   special member functions : full
  //   == <=> // with duration<R2, P2>
  //
  //   explicit duration(x)
  //   count()->rep
  //
  //   duration(const duration<R2, P2> &) // semi-safe
  //
  //   + // unary or binary, always return duration<...>
  //   - // ...
  //   ++
  //   --
  //   +=
  //   -=
  //
  //   *=(rep)->this_t &
  //   /=(rep)->this_t &
  //   %=(rep)->this_t &
  //   %=(const this_t &)->this_t &
  //
  //   *(with const R2 &)->duration<common_type<R, R2>, P>
  //   /(const R2 &)->duration<common_type<R, R2>, P>
  //   /(const duration<R2, P2> &)->common_type<R, R2>
  //   %(const R2 &)->duration<common_type<R, R2>, P>
  //   %(const duration<R2, P2> &)
  //     ->common_type<duration<R, P>, duration<R2, P2>>
  //
  //   static zero()->this_t
  //   static min()->this_t
  //   static max()->this_t
  //
  //   flour<d2_t>()->d2_t
  //   ceil<d2_t>()->d2_t
  //   round<d2_t>()->d2_t
  //   abs()->this_t
  // duration_cast<d2_t>(const duration<R, P> &)->d2_t
  // duration_floor<d2_t>(const duration<R, P> &)->d2_t
  // duration_ceil<d2_t>(const duration<R, P> &)->d2_t
  // duration_round<d2_t>(const duration<R, P> &)->d2_t
  //
  // nanoseconds = duration<long long, nano>
  // microseconds
  // milliseconds
  // seconds
  // minutes
  // hours
  // days
  // weeks
  // months
  // years
  //
  // ""_h(x)->hours
  // ""_min(x)->minutes
  // ""_s(x)->seconds
  // ""_ms(x)->milliseconds
  // ""_us(x)->microseconds
  // ""_ns(x)->nanoseconds
  //
  // ""_h(f)->duration<double, ratio(3600, 1)>
  // ""_min(f)
  // ""_s(f)
  // ""_ms(f)
  // ""_us(f)
  // ""_ns(f)
}
void doc_time_point() {
  // time_point<C, D = C::duration>
  //   clock = C
  //   duration = D
  //   rep = duration::rep
  //   period = duration::period
  //
  //   special member functions: full
  //
  //   explicit time_point(const duration &)
  //   time_point(const time_point<C, D2>)
  //     requires is_convertible<D2, duration>
  //
  //   time_since_epoch()->duration
  //   time()->duration // short name for time_since_epoch()
  //   count()->decltype(time().count())
  //     // short name for time_since_epoch().count()
  //
  //   ++ -- += -=
  //   +(with const duration<R, P> &>)
  //     ->time_point<C, common_type<D, duration<R, P>>
  //   -(const duration<R, P>)->time_point<C, common_type<D, duration<R, P>>
  //   -(const time_point<C, D2> &)->common_type<D, D2>
  //
  //   static min()->this_t
  //   static max()->this_t
  //
  //   flour<D2>()->time_point<C, D2>
  //   ceil<D2>()->time_point<C, D2>
  //   round<D2>()->time_point<C, D2>
  //
  // time_point_cast<D2>(const time_point<C, D> &)->time_point<C, D2>
  // time_point_floor<D2>(const time_point<C, D> &)->time_point<C, D2>
  // time_point_ceil<D2>(const time_point<C, D> &)->time_point<C, D2>
  // time_point_round<D2>(const time_point<C, D> &)->time_point<C, D2>
}
void doc_clock() {
  // scoped_timer<CLOCK>
  //   special member functions: destructible
  //   explicit scoped_timer(double &d)
  //     // do d += recorded_duration_count at the lifetime-end-point of *this
  //
  // system_clock
  // steady_clock
  //   rep = long long
  //   static constexpr ratio period = ...
  //   duration = duration<rep, period>
  //   time_point = time_point<this_t>
  //   static constexpr bool is_steady
  //
  //   timer = scoped_timer<this_t>
  //
  //   static now()->time_point
  //
  // wait_for(const duration<R, P> &)
  //   // use steady_clock to do busy wait
}

void doc_win32_enable_utf8_console() {
  // inner::fns::win32_enable_utf8_console()
}
void doc_c_file() {
  // fputc_iterator
  //   iterator category: oitr
  //   explicit fputc_iterator(FILE *= stdout)
  //   base()->FILE *
  //
  // c_file
  //   special member functions: default constructible, movable
  //   == with nullptr
  //   explicit c_file(const char *name, mode = "a+b")
  //   explicit c_file(FILE *)
  //   open(const char *name, mode = "a+b")
  //   empty()->bool
  //   close()
  //   base()->FILE *
  //   release()->FILE *
  //   error()->bool
  //
  //   getc()->int
  //   putc(int)->const this_t & // no flush()
  //   putc_iter()->fputc_iterator // no flush()
  //   flush()->const this_t &
  //
  //   get<STR = string>()->STR
  //   put(const char *)->const this_t & // auto call flush()
  //   put(char_range)->const this_t & // auto call flush()
  //
  //   putws(const wchar_t *)->const this_t &
  //   putws(wchar_range)->const this_t &
  //
  // c_file_exists(const char *)->bool
  //
  // console_c_file
  //   // console_c_file is used to wrap stdin, stdout, and stderr
  //   special member functions: destructible and move-constructible
  //
  //   base()->FILE *
  //   error()->bool
  //
  //   getc()->int
  //   putc(int)->const this_t &
  //   putc_iter()->fputc_iterator
  //   flush()->const this_t &
  //
  //   get<STR = string>()->STR
  //   put(const char *)->const this_t &
  //   put(char_range)->const this_t &
  //
  // stdin_f()->console_c_file
  //   // straight call stdin_f() when needed,
  //   //   because it's meaningless to store the wrapper
  // stdout_f()->console_c_file // straight call stdin_f() when needed
  // stderr_f()->console_c_file // straight call stdin_f() when needed
}
void doc_print_tag() {
  // print_tag_left left
  // print_tag_right right
  //
  // print_tag_min_width
  //   size_t value
  // setw(size_t w)->print_tag_min_width
  //
  // print_tag_fill
  //   char32_t value
  // setfill(char c)->print_tag_fill
  // setfill(char32_t c)->print_tag_fill
  //
  // print_tag_showpos showpos
  // print_tag_noshowpos noshowpos
  //
  // print_tag_dec dec
  // print_tag_hex hex
  // print_tag_oct oct
  // print_tag_bin bin
  //
  // print_tag_separator
  //   size_t value
  // setseparator(size_t n)->print_tag_separator
  //
  // print_tag_uppercase uppercase
  // print_tag_nouppercase nouppercase
  // print_tag_fixed fixed
  // print_tag_scientific scientific
  //
  // print_tag_precision
  //   size_t value
  // print_tag_precision_fold0
  //   size_t value
  // setprecision(size_t n)->print_tag_precision
  //   // if n == numeric_limits<size_t>::max()
  //   //   show all valid numbers
  //   //   omit any zero tail after the dot
  //   //   if no non-zero tail after the dot, omit the dot
  //   // otherwise
  //   //   show exactly n numbers after the dot,
  //   //   the dot is omitted if n == 0
  // setprecision_fold0(n)->print_tag_precision_fold0
  //   // if n == numeric_limits<size_t>::max()
  //   //   same as setprecision(n)
  //   // otherwise
  //   //   show at most n numbers after the dot,
  //   //   omit any zero tail after dot
  //   //   omit the dot if no non-zero tail after the dot
}
void doc_sscan() {
  // sscan(sv_ref, sv)->bool // if string is string, sv can be string literal
  // sscan(sv_ref, bool &)->bool // non-zero integer means true
  // sscan(sv_ref, int_ref)->bool
  // sscan(sv_ref, int_ref, dec)->bool
  // sscan(sv_ref, int_ref, bin)->bool
  // sscan(sv_ref, int_ref, oct)->bool
  // sscan(sv_ref, int_ref, hex)->bool
  // sscan(sv_ref, float_ref)->bool
  //
  // sscan_single(sv_ref, sv_char_t &)->bool
  // sscan_single(sv_ref)->optional<sv_char_t>
  //
  // sscan_while(sv_ref, f)->sview
  //
  // sscan_ln(sv_ref)->bool
  //
  // sscan_line(sv_ref)->optional<sv>
  //   // if sv_ref is empty, return nullopt
  //   // otherwise, return reference of one line except the new-line character,
  //   //   and sv_ref exclude the line and the tailed new-line character
  //
  // sscan_spaces(sview &) // skip spaces defined by isspace
  // sscan_spaces(u32sview &)
  //   // skip spaces defined by isspace and other unicode spaces
  //
  // sscan_common_spaces(sv_ref)
  //   // skip while unicode_is_common_space()
}
void doc_sprint() {
  // print_args
  //   size_t min_width
  //   bool left_padding
  //   char padding_char
  //
  //   left()->this_t &
  //   right()->this_t &
  //   setw(n)->this_t &
  //   setfill(c)->this_t &
  //
  // int_print_args
  //   size_t radix
  //   const char *numbers
  //   bool upper_case
  //   bool show_positive_symbol
  //   size_t separator_n
  //
  //   left()->this_t &
  //   right()->this_t &
  //   setw(n)->this_t &
  //   setfill(c)->this_t &
  //
  //   dec()->this_t &
  //   bin()->this_t &
  //   oct()->this_t &
  //   hex()->this_t &
  //   uppercase()->this_t &
  //   nouppercase()->this_t &
  //   showpos()->this_t &
  //   noshowpos()->this_t &
  //   setseparator(n)->this_t &
  //
  // float_print_args
  //   const char *numbers
  //   bool upper_case
  //   bool show_positive_symbol
  //   size_t separator_n
  //   bool scientific_notation
  //   size_t precision
  //   bool fold_zero
  //
  //   left()->this_t &
  //   right()->this_t &
  //   setw(n)->this_t &
  //   setfill(c)->this_t &
  //
  //   uppercase()->this_t &
  //   nouppercase()->this_t &
  //   showpos()->this_t &
  //   noshowpos()->this_t &
  //   setseparator(n)->this_t &
  //   scientific()->this_t &
  //   fixed()->this_t &
  //   setprecision(n)->this_t &
  //
  // sprint(s, sv, print_args)->s_t &
  // sprint(s, sv, print_tags...)->s_t &
  // sprint(s, integral, int_print_args)->s_t &
  // sprint(s, integral, print_tags...)->s_t &
  // sprint(s, floating_point, float_print_args)->s_t &
  // sprint(s, floating_point, print_tags...)->s_t &
  //
  // sprint(sv, print_args)->s_t
  // sprint(sv, print_tags...)->s_t
  // sprint(integral, int_print_args)->s_t
  // sprint(integral, print_tags...)->s_t
  // sprint(floating_point, float_print_args)->s_t
  // sprint(floating_point, print_tags...)->s_t
  //
  // sprint_u32(...)->...
  //   // same as sprint but the target string's value_type is char32_t
  //
  // put(s...) // call std::fputc() to print char string to stdout
  // putln(s...) // same as put(...), but plus '\n'
  // putws(s...) // call std::putws() to print wide-char string to stdout
  // putwsln(s...) // same as putws(...), but plus L'\n'

  assert(sprint("abc"_sv, left, setw(4), setfill('_')) == "_abc");
  assert(sprint(12345, right, setw(6)) == "12345 ");
  assert(sprint(12345, setseparator(2)) == "1\'23\'45");
  assert(sprint(12345, showpos) == "+12345");
  assert(sprint(12345, noshowpos) == "12345");
  assert(sprint(2 * 8 * 8 + 3 * 8 + 4, oct) == "234");
  assert(sprint(2 * 16 * 16 + 3 * 16 + 4, hex) == "234");
  assert(sprint(0b1011, bin) == "1011");
  assert(sprint(123, dec) == "123");
  assert(sprint(10 * 16 * 16 + 11 * 16 + 12, hex) == "abc");
  assert(sprint(10 * 16 * 16 + 11 * 16 + 12, hex, uppercase) == "ABC");
  assert(sprint(10 * 16 * 16 + 11 * 16 + 12, hex, nouppercase) == "abc");

  assert(sprint(1.5) == "1.5");
  assert(sprint(1.5, fixed) == "1.5");
  assert(sprint(1.5, scientific) == "1.5e+0");
  assert(sprint(1.5, scientific, noshowpos) == "1.5e+0");
  assert(sprint(1.5, scientific, showpos) == "+1.5e+0");
  assert(sprint(1.5, scientific, nouppercase) == "1.5e+0");
  assert(sprint(1.5, scientific, uppercase) == "1.5E+0");
  assert(sprint(1.5, setprecision(6)) == "1.500000");
  assert(sprint(1000.5, setprecision(7), setseparator(3))
         == "1\'000.500\'000\'0");
}
void doc_big_int() {
  // big_int
  //   special member functions: full
  //   ==
  //   <=>
  //
  //   explicit big_int(int64_t)
  //   =(int64_t)->this_t &
  //   to_int64()->optional<int64_t>
  //
  //   explicit big_int(int32_t)
  //   =(int32_t)->this_t &
  //   to_int32()->optional<int32_t>
  //
  //   is_non_negative()->bool
  //   neg()
  //
  //   is_zero()->bool
  //
  //   container_type
  //   const container_type &data() const &
  //   data() &&->container_type
  //   data(container_type &&)
  //
  //   mul_pow_of_2(int32_t)->this_t &
  //   mul_pow_of_10(int32_t)->this_t &
  //   mul(uint32_t)->this_t &
  //   div(uint32_t)->uint32_t // return remainder
  //
  //   +=(const this_t &)
  //   -=(const this_t &)
  //   +(const this_t &)->this_t
  //   -(const this_t &)->this_t
  //
  //   sscan(sv_ref)->bool
  //   sprint(separator_n = 0)->string
  //
  //   sscan_hex(sv_ref)->bool
  //   sprint_hex(separator_n = 0)->string
  //
  //   sscan_scientific(sv_ref)->bool
  //   sprint_scientific(precision = 6u,
  //                     fold_zero = (precision == (max of size_t)),
  //                     separator_n = 0, upper_e = false,
  //                     show_pos = false)->string
  //     // fold_zero: remove all tailed zero,
  //     //   and remove dot if there is not number after dot

  sview sv = "123467890111222333444555666777888999000";
  big_int i;
  i.sscan(sv);
  assert(sv.empty());
  assert(i.sprint() == "123467890111222333444555666777888999000");
}
void doc_ssplitter() {
  // ssplitter<STR = sview>
  //   // in-object data: small_vector<STR, 10>
  //
  //   special member functions: full
  //   ssplitter(string_view<rng_vt<STR>, string_view<rng_vt<STR>,
  //             search_f = decay<decltype(search)>{})  
  //   ssplitter(string_view<rng_vt<STR>, rng_vt<STR>)
  //
  //   begin() // the value_type is STR
  //   end()
  //   empty()->bool
  //   size()->some_uint
  //   clear()
  //   ()(sv, oi)->oi_itr
  //   ()(iter, oi)->pair<iter, oi_itr> // need enough elements from iter
  //   ()(r, oi)->pair<iter, oi_itr>
  //     // if empty(r), do nothing,
  //     // otherwise
  //     //   if r has no enough elements, repeatedly use the last one,
  //     //   otherwise
  //     //     same as operator ()(begin(r), oi)
  //
  // ssplit(s_or_sv, sv_delimiter, oi)->oi_itr
  //   // *oi++ = sv

  // constructors
  {
    ssplitter s("ab_cd_efg_h", "_");
    assert(equal(s, seq("ab"_sv, "cd", "efg", "h")));
    ssplitter s2("ab_cd_efg_h", '_');
    assert(equal(s2, s));

    ssplitter s3("_cd_efg_h", "_");
    assert(equal(s3, seq(""_sv, "cd", "efg", "h")));

    ssplitter s4("_", "_");
    assert(equal(s4, seq(""_sv, "")));

    ssplitter s5("", "_");
    assert(s5.empty());
    ssplitter s6("", "");
    assert(s6.empty());

    ssplitter s7("abc", "");
    assert(equal(s7, seq("abc"_sv)));
    ssplitter s8("", "abc");
    assert(s8.empty());
  }

  assert(ssplitter({"ab_cd_efg_h", '_'})("||", to_back(string{})).base()
         == "ab||cd||efg||h");

  const vector<string> v = {"$", "%%", "^^^", "&&&&"};
  assert(ssplitter({"ab_cd_efg_h", '_'})(v.begin(), to_back(string{}))
         .second.base()
         == "ab$cd%%efg^^^h");

  assert(equal(ssplit("ab__cd__efg"_sv, "__", to_back(vector<sview>{})).base(),
               seq("ab"_sv, "cd", "efg")));
}
void doc_arithmetic_parser() {
  // default_arithmetic_grammar
  //   enum class opr
  //     paren, pos, neg, pow, mul, div, add, sub
  //   operator_type = opr
  //   operand_type = double
  //   stack_type = vector<operand_type>
  //   function_type = function<operand_type (const stack_type &)>
  //   char_type = char
  //   string_type = string
  //   view_type = sview
  //   const auto &operator_list() const {
  //     static const auto g
  //       = vec(hvec(true) // left to right
  //             (hvec(opr::paren, 1,
  //                   fn_t([](const stack_type &s) {return back(s);}))
  //              ("(", "", ")")
  //              ), ...);
  //     return g;
  //   }
  //   const flat_map<string, flat_map<int, function_type>> &
  //   function_list() const
  //   special member functions: only movable
  //
  // default_arithmetic_grammar_u32
  //   ...
  //
  // arithmetic_parser<G>
  //   special member functions: dependent
  //   explicit this_t(const G &) // depend on G
  //   explicit this_t(G &&) // depend on G
  //   ()(G::view_type &)->optional<G::operand_type>
  //   ()(G::view_type &,
  //      G::view_type::iterator &failed_pos)->optional<G::operand_type>
  //   clear_buffer()
  //
  // default_arithmetic_parser = arithmetic_parser<default_arithmetic_grammar>
  // default_arithmetic_parser_u32
  //   = arithmetic_parser<default_arithmetic_grammar_u32>
  //
  // sscan_arithmetic(sview &)->optional<double>
  // sscan_arithmetic(u32sview &)->optional<double>

  sview v = "1+2*3^4+1";
  auto x = sscan_arithmetic(v);
  assert(x.has_value());
  assert(*x == 164.0);
  assert(v.empty());
}
void doc_file() {
  // read_file_t
  // read_file
  // open_file_t
  // open_file
  // create_file_t
  // create_file
  // file
  //   special member functions: only movable
  //   explicit file(sv) // same as file(sv, open_file)
  //   explicit file(sv, open_file)
  //     // open an existed file or create a new one if no such file
  //   explicit file(sv, read_file)
  //     // open an existed file, failed if no such file
  //     // can read but can not write
  //   explicit file(sv, create_file)
  //     // create a new file, failed if there is a file with the same name
  //   empty()->bool
  //   open(sv)
  //   close()
  //   read<S = string>()->S
  //   write(contiguous_r)
  //
  // to_full_path(sv)->s
  // simplify_path(sv)->s
  //   // remove all redundant '/' or '\', and use '\' to replace '/'
  // remove_path_last_name(sv)->s // .../###/ // unchange if no last name
  // replace_path_last_name(sv, sv_new_last_name)->s // unchange if no last name
  //
  // is_file(sv)->bool // is file or directory, check existence
  // is_directory(sv)->bool
  // try_create_directory(sv)->bool
  // create_directory(sv) // call throw_or_terminate() if failed
  // file_size(sv)->ull
  // file_time(sv)->system_clock::time_point
  // try_remove_file(sv)->bool
  // remove_file(sv) // call throw_or_terminate() if failed
  // try_rename_file(sv, sv2)->bool // sv is full path, sv2 is just name
  // rename_file(sv, sv2) // call throw_or_terminate() if failed
  //
  // file_info<S = string>
  //   // here directory is also file
  //   string_type = S
  //   S path
  //   S name
  //   unsigned long long size
  //   time_point<system_clock> time
  //   bool is_dir
  // view_file(sv)->file_info<...> // non-recursive
  // view_directory(sv)->tree<file_info<...>> // recursive to all children
}

void doc_atomic() {
  // atomic<int>
  // atomic<long>
  // atomic<long long>
  //   special member functions:
  //     this_t() initialize to zero
  //     non-movable
  //   value_type
  //   atomic(vt)
  //   =(vt)->vt
  //   operator vt()
  //   load()->vt
  //   store(vt)
  //   exchange(vt)->vt
  //   compare_exchange(vt &expected, vt desired)->bool
  //     // if load() == expected, do store(desired), and return true
  //     // otherwise, do expected = load(), and return false
  //   fetch_add(vt)->vt
  //   fetch_sub(vt)->vt
  //   ++ --()->vt
  //   ++ --(int)->vt
  //   += -= &= |= ^=(vt)->vt
  //
  // atomic<T *>
  //   special member functions:
  //     this_t() initialize to zero
  //     non-movable
  //   value_type
  //   difference_type
  //   store(vt)
  //   load()->vt
  //   operator vt()
  //   =(vt)->vt
  //   exchange(vt)->vt
  //   compare_exchange(vt &expected, vt desired)->bool
  //     // if load() == expected, do store(desired), and return true
  //     // otherwise, do expected = load(), and return false
  //
  // atomic_flag
  //   special member functions:
  //     default-constructor: after the initialization, test() return false
  //     can not copy and move
  //   test()->bool
  //   test_and_set()->bool // make test() return true
  //   clear() // make test() return false
}
void doc_weak_ptr() {
  // bad_weak_ptr : public exception
  //
  // weak_ptr<T>
  //   // inner data: a T * and a pointer of reference-count block
  //   // weak means 0 cost of reference-count
  //   element_type = remove_extent<T>
  //   special member functions: full
  //   weak_ptr(const weak_ptr<Y> &)
  //     // Y * is compatible pointer with T *,
  //     //   and the most commonly used case is that T is base of Y
  //   =(const weak_ptr<Y> &) // Y * is compatible pointer with T *
  //   weak_ptr(weak_ptr<Y> &&) // Y * is compatible pointer with T *
  //   =(weak_ptr<Y> &&) // Y * is compatible pointer with T *
  //   weak_ptr(const shared_ptr<Y> &) // Y * is compatible pointer with T *
  //   =(const shared_ptr<Y> &) // Y * is compatible pointer with T *
  //   reset()
  //   use_count()->long // 0 if pointer of reference-count block is nullptr
  //   expired()->bool // use_count() == 0
  //   lock()->shared_ptr<T>
  //     // return nullptr if expired,
  //     //   otherwise return a shared_ptr which hold one more reference count
  //   owner_before(const shared_ptr<U> &)->bool // compare, like less
  //   owner_before(const weak_ptr<U> &)->bool // compare, like less
  // weak_ptr(shared_ptr<T>)->weak_ptr<T>
  //
  // owner_less<T = void>
  // owner_less<shared_ptr<T>>
  //   operator ()(const shared_ptr<T> &, const shared_ptr<T> &)->bool
  //   operator ()(const shared_ptr<T> &, const weak_ptr<T> &)->bool
  //   operator ()(const weak_ptr<T> &, const shared_ptr<T> &)->bool
  // owner_less<weak_ptr<T>>
  //   operator ()(const shared_ptr<T> &, const shared_ptr<T> &)->bool
  //   operator ()(const shared_ptr<T> &, const weak_ptr<T> &)->bool
  //   operator ()(const weak_ptr<T> &, const shared_ptr<T> &)->bool
  //   operator ()(const weak_ptr<T> &, const weak_ptr<T> &)->bool
  // owner_less<void>
  //   is_transparent
  //   operator ()(const shared_ptr<T> &, const shared_ptr<U> &)->bool
  //   operator ()(const shared_ptr<T> &, const weak_ptr<U> &)->bool
  //   operator ()(const weak_ptr<T> &, const shared_ptr<U> &)->bool
  //   operator ()(const weak_ptr<T> &, const weak_ptr<U> &)->bool
}
void doc_shared_ptr() {
  // enable_shared_from_this<T>
  //   // as public base class to enable shared from this
  //   protected special member functions: full
  //   shared_from_this()->shared_ptr<(const) T>
  //   weak_from_this()->weak_ptr<(const) T>
  //
  // shared_ptr<T>
  //   element_type = remove_extent<T>
  //   weak_type = weak_ptr<T>
  //   special member functions: full
  //   shared_ptr(nullptr) // make no counter
  //   reset()
  //   shared_ptr(Y *)
  //     // Y * is compatible with T *,
  //     //   and the most commonly used case is that T is base of Y
  //   reset(Y *)
  //   shared_ptr(Y *, deleter) // Y * is compatible with T *
  //   shared_ptr(nullptr, deleter) // make counter
  //   reset(Y *, deleter)
  //   shared_ptr(Y *, deleter, alloc) // Y * is compatible with T *
  //   shared_ptr(nullptr, deleter, alloc) // make counter
  //   reset(Y *, deleter, alloc)
  //   shared_ptr(const shared_ptr<Y> &) // Y * is compatible with T *
  //   =(const shared_ptr<Y> &)->this_t & // Y * is compatible with T *
  //   shared_ptr(shared_ptr<Y> &&) // Y * is compatible with T *
  //   =(shared_ptr<Y> &&)->this_t & // Y * is compatible with T *
  //   shared_ptr(const shared_ptr<Y> &, element_type *)
  //     // Y * is compatible with T *
  //     // normally construct but store a different pointer
  //   shared_ptr(shared_ptr<Y> &&, element_type *)
  //     // Y * is compatible with T *
  //     // normally assign but store a different pointer
  //   shared_ptr(unique_ptr<Y, D> &&)
  //     // T is not array, Y * is implicitly convertible to T *
  //   shared_ptr &operator =(unique_ptr<Y, D> &&)
  //     // T is not array, Y * is implicitly convertible to T *
  //   explicit shared_ptr(const weak_ptr<Y> &)
  //     // Y * is compatible with T *
  //   get()->element_type *
  //   *()->element_type & // T is array
  //   ->()->element_type * // T is array
  //   [](ptrdiff_t)->element & // T is not array
  //   use_count()->long
  //   explicit operator bool() // return get() != nullptr
  //   owner_before(const shared_ptr<U> &)->bool
  //   owner_before(const weak_ptr<U> &)->bool
  // template <class T>
  // shared_ptr(weak_ptr<T>)->shared_ptr<T>
  // template <class T, class D>
  // shared_ptr(unique_ptr<T, D>)->shared_ptr<T>
  // ==(const shared_ptr<T> &, const shared_ptr<U> &)->bool
  // ==(const shared_ptr<T> &, nullptr_t)->bool
  // <=>(const shared_ptr<T> &, const shared_ptr<U> &)->strong_ordering
  // <=>(const shared_ptr<T> &, nullptr_t)->strong_ordering
  // get_deleter<D>(const shared_ptr<T> &)->D *
  //   // D is cv-unqualified
  //   // return nullptr if the stored D is not cv-unqualified
  //   //   or has a different typeid
  // hash<shared_ptr<T>>
  //   argument_type = shared_ptr<T>
  //   result_type = size_t
  //   ()(const shared_ptr<T> &)->size_t
  //
  // make_shared<T>(s...)->shared_ptr<T>
  // make_shared<T []>(n)->shared_ptr<T []>
  // make_shared<T []>(n, const T &)->shared_ptr<T []>
  // make_shared<T [N]>()->shared_ptr<T [N]>
  // make_shared<T [N]>(const T &)->shared_ptr<T [N]>
  //
  // make_shared_for_overwrite<T>()->shared_ptr<T>
  // make_shared_for_overwrite<T []>(n)->shared_ptr<T []>
  //
  // allocate_shared<T>(alloc, s...)->shared_ptr<T>
  // allocate_shared<T [N]>(alloc)->shared_ptr<T [N]>
  // allocate_shared<T [N]>(alloc, const T &)->shared_ptr<T [N]>
  // allocate_shared<T []>(alloc, n)->shared_ptr<T []>
  // allocate_shared<T []>(alloc, n, const T &)->shared_ptr<T []>
  //
  // allocate_shared_for_overwrite<T>(alloc)->shared_ptr<T>
  // allocate_shared_for_overwrite<T []>(alloc, n)->shared_ptr<T []>
  //
  // static_pointer_cast<T>(const shared_ptr<U> &)->shared_ptr<T>
  // static_pointer_cast<T>(shared_ptr<U> &&)->shared_ptr<T>
  // reinterpret_pointer_cast<T>(const shared_ptr<U> &)->shared_ptr<T>
  // reinterpret_pointer_cast<T>(shared_ptr<U> &&)->shared_ptr<T>
  // const_pointer_cast<T>(const shared_ptr<U> &)->shared_ptr<T>
  // const_pointer_cast<T>(shared_ptr<U> &&)->shared_ptr<T>
  // dynamic_pointer_cast<T>(dynamic shared_ptr<U> &)->shared_ptr<T>
  // dynamic_pointer_cast<T>(shared_ptr<U> &&)->shared_ptr<T>
}
void doc_thread() {
  // thread
  //   special member functions: default-constructible, movable
  //   explicit thread(f, s...)
  //
  //   joinable()->bool
  //   join() // joinable()
  //   detach() // joinable()
  //
  //   id
  //     special member functions: full
  //     == <=>
  //   get_id()->id
  //   native_handle_type
  //   native_handle()->native_handle_type
  //   static hardware_concurrency()->unsigned
  //
  // hash<thread::id>
  //   ()(thread::id)->size_t
  //
  // this_thread::get_id()->thread::id
  // this_thread::sleep_for(duration)
  // this_thread::sleep_until(time_point)
  //
  // sleep_for(duration)
  // sleep_until(time_point)
  // get_thread_id()->thread::id
}
void doc_mutex() {
  // mutex
  // timed_mutex
  //   special member functions: default-constructible
  //   lock()
  //   try_lock()->bool
  //   unlock()
  //   native_handle_type
  //   native_handle()->native_handle_type
  //   try_lock_for(t)->bool // only for timed_mutex
  //   try_lock_until(t)->bool // only for timed_mutex
  //
  // defer_lock_t
  // defer_lock
  // try_to_lock_t
  // try_to_lock
  // adopt_lock_t
  // adopt_lock
  //
  // lock_guard<M>
  //   mutex_type = M
  //   special member functions: only destructible
  //   this_t(m, adopt_lock_t) // do not call m.lock because the lock is adopted
  //   explicit this_t(m)
  //
  // unique_lock<M>
  //   mutex_type = M
  //   special member functions: default-constructible, movable
  //   explicit this_t(m)
  //   this_t(m, defer_lock) // do not owns, and do not lock
  //   this_t(m, try_to_lock) // owns if m.try_lock()
  //   this_t(m, adopt_lock) // owns, do not lock
  //   this_t(m, time_point) // busy-wait until the time_point is reached
  //   this_t(m, duration) // busy-wait some duration
  //
  //   lock()
  //   try_lock()->bool
  //   try_lock_for(duration)->bool
  //   try_lock_until(time_point)->bool
  //   unlock()
  //
  //   release()->mutex_type *
  //   owns_lock()->bool
  //   explicit operator bool() // same as owns_lock()
  //   mutex()->mutex_type *
}
void doc_condition_variable() {
  // enum cv_status
  //   no_timeout
  //   timeout
  // cv_no_timeout = cv_status::no_timeout
  // cv_timeout = cv_status::timeout
  // condition_variable
  //   special member functions: default-constructible
  //   notify_one()
  //   notify_all()
  //   wait(unique_lock<mutex> &)
  //   wait(unique_lock<mutex> &, eq)
  //   wait_until(unique_lock<mutex> &, time_point)->cv_status
  //   wait_until(unique_lock<mutex> &, time_point, eq)->bool
  //   wait_for(unique_lock<mutex> &, duration)->cv_status
  //   wait_for(unique_lock<mutex> &, duration, eq)->bool
  //   native_handle_type
  //   native_handle()->native_handle_type
}
void doc_mutex_area() {
  // mutex_area<T>
  //   // every function argument takes no argument
  //   special member functions: default-constructible
  //   explicit this_t(in_place, s...)
  //   value_type = T
  //   ->()->value_type *
  //   *()->value_type &
  //   data()->value_type &
  //   enter(f) // lock the mutex and call f()
  //   try_enter(f)->bool
  //   wait_enter(f, eq)
  //     // repeatedly lock and check until eq(), then call f() and return
  //   enter_notify_one(f, condvar_ref)
  //     // enter(f) and condvar.notify_one()
  //   enter_notify_all(f, condvar_ref)
  //     // enter(f) and condvar.notify_all()
  //   leave_until(eq, condvar_ref) // used in f of this->enter(f)
  //   leave_notify_one(condvar_ref)
  //     // leave_until(eq, condvar_ref) and condvar.notify_one()
  //   leave_notify_all(condvar_ref)
  //     // leave_until(eq, condvar_ref) and condvar.notify_all()
  //   busy_wait(eq)
}
void doc_shared_mutex() {
  // shared_mutex
  // shared_timed_mutex
  //   special member functions: default-constructible
  //
  //   lock()
  //   try_lock()->bool
  //   unlock()
  //
  //   lock_shared()
  //     // "shared" means two or more readers
  //     //   can simultaneously access the data protected by the same mutex
  //   try_lock_shared()->bool
  //   unlock_shared()
  //
  //   native_handle_type
  //   native_handle()->native_handle_type
  //
  //   try_lock_for(duration)->bool // only for shared_timed_mutex
  //   try_lock_until(time_point)->bool // only for shared_timed_mutex
  //   try_lock_shared_for(duration)->bool // only for shared_timed_mutex
  //   try_lock_shared_until(time_point)->bool // only for shared_timed_mutex
  //
  // shared_lock<M>
  //   // M is "shared_", because lock() call m.lock_shared()
  //   // "shared" means two or more readers
  //   //   can simultaneously access the data protected by the same mutex,
  //   // shared_lock is used by the readers to access the data
  //   // for writers, just use lock_guard,
  //   //   writer need all readers are out of the data
  //
  //   mutex_type = M
  //   special member functions: default-constructible, movable
  //   explicit this_t(m)
  //   this_t(m, defer_lock)
  //   this_t(m, try_to_lock)
  //   this_t(m, adopt_lock)
  //   this_t(m, time_point)
  //   this_t(m, duration)
  //
  //   lock()
  //   try_lock()->bool
  //   try_lock_for(t)->bool
  //   try_lock_until(t)->bool
  //   unlock()
  //
  //   release()->mutex_type *
  //   owns_lock()->bool
  //   explicit operator bool()
  //   mutex()->mutex_type *
}
void doc_thread_pool() {
  // thread_pool
  //   special member functions: only default constructible
  //     // when destructor is called, there is no running thread
  //   explicit thread_pool(n)
  //   fetch(f)->pool_thread // if pool is full, just call f in this thread
  //   fetch_real(f)->pool_thread
  //     // if pool is full, append 1 real thread to the pool,
  //     //   and try one more time
  //     //   until successly get a real thread who is free to call f()
  //   size_type
  //   count()->size_type
  //   count_running()->size_type
  //   count_sleeping()->size_type
  //   shrink_to_fit()
  //   append(size_type)
  //
  // default_thread_pool()->thread_pool &
  //
  // pool_thread
  //   special member functions: default-constructible, movable
  //     // move-assignment need empty target
  //   explicit pool_thread(f) // if pool is full, just call f
  //   joinable()->bool
  //   join()
  //   try_join()->bool
  //     // always call try_join because join() can not work if no real thread
  //     // is gotten
  //   pool()->thread_pool *
  //   id = thread::id
  //   get_id()->id
  //     // return id{} if empty of f is called in this thread
  //   native_handle_type
  //     // return invalid value if empty of f is called in this thread
  //   native_handle()->native_handle_type
}
void doc_exclusive_thread_pool() {
  // exclusive_thread_pool
  //   special member functions: only default constructible
  //   exclusive_thread_pool(size_t)
  //   fetch(f) // if the pool is full, just call f() in this thread
  //   join()
  //     // join is not called by thread object, but called by pool,
  //     //   and this is what "exclusive" means
  //   size_type = size_t
  //   count()->size_type
  //   count_running()->size_type
  //   count_sleeping()->size_type
  //   shrink_to_fit()
  //   append(size_type)
}

void test() {
  doc_base();
  doc_std_dependence();
  doc_inner_back_door_functions();
  doc_simple_put_functions();
  doc_simple_mutex();
  doc_print_error();
  doc_basic_template_metaprogramming_tools();
  doc_swap();
  doc_type_traits();
  doc_basic_utility_functions();
  doc_pointer_traits();
  doc_reference_wrapper_and_invoke();
  doc_language_related_concepts();
  doc_concept_constrained_comparisons();
  doc_integral_traits();
  doc_numeric_limits();
  doc_three_way_comparison();
  doc_type_index();
  doc_basic_function_objects();
  doc_tuple();
  doc_bind();
  doc_ratio();
  doc_optional();
  doc_semiregular_function();
  doc_hash();
  doc_miscl();
  doc_floating_point_traits();
  doc_std_dependence_fpfns();
  doc_common_math();
  doc_basic_functions_for_char32_t();

  doc_owner_ptr();
  doc_test_throwing();
  doc_test_ownership();
  doc_test_rational_operator();
  doc_instance_counter();
  doc_ez_dynamic();
  doc_ez_function();
  doc_ez_vector();
  doc_ez_forward_list();
  doc_ez_slist();
  doc_ez_bidirectional_list();
  doc_ez_list();
  doc_ez_map();
  doc_test_allocator();
  doc_test_range();

  doc_iterator_requirements();
  doc_iterator_main_components();
  doc_range_main_components();
  doc_degraded_iterator();
  doc_reverse_iterator();
  doc_insert_iterator();
  doc_move_iterator();
  doc_counted_iterator();
  doc_iterator_wrapper();
  doc_array();
  doc_iter_pair();
  doc_composite_range();
  doc_range_wrapper();
  doc_base_range();
  doc_empty_range();
  doc_single_range();
  doc_counted_range();
  doc_degraded_range();
  doc_move_range();
  doc_reverse_range();
  doc_rng_for_iterator_n();
  doc_rng_for_n_value();
  doc_iterator_range();
  doc_bind_range();
  doc_cache_latest_range();
  doc_iters();
  doc_range_miscl();

  doc_allocator_traits();
  doc_uninitialized_argo();
  doc_default_allocator();
  doc_allocator_wrapper();
  doc_unique_ptr();
  doc_unique_array();
  doc_copyable_ptr();
  doc_copyable_array();
  doc_maybe_owner_ptr();
  doc_buffer();
  doc_scoped_allocator_adaptor();
  doc_allocator_aware_container_ownership();
  doc_object_pool();
  doc_raw_object_pool();
  doc_memory_pool();

  doc_dynamic_base();
  doc_dynamic();
  doc_dynamic_void();
  doc_function();
  doc_optional();
  doc_move_only_function();
  doc_unique_function();
  doc_type_erased_invocation();
  doc_variant();

  doc_random();

  doc_equal();
  doc_allanynone_of();
  doc_for_each();
  doc_find();
  doc_find_last();
  doc_find_first_last_of();
  doc_adjacent_find();
  doc_count();
  doc_mismatch();
  doc_is_permutation();
  doc_search();
  doc_find_end();
  doc_find_subrange();
  doc_contains();
  doc_starts_ends_with();
  doc_fold_left_right();
  doc_copy_move_if_backward_from();
  doc_swap_ranges();
  doc_transform();
  doc_replace_if_copy_if();
  doc_fill();
  doc_generate();
  doc_remove_if_copy_if();
  doc_unique_copy();
  doc_reverse_copy();
  doc_rotate_copy();
  doc_shift();
  doc_sample();
  doc_shuffle();
  doc_binary_search_series();
  doc_partition_series();
  doc_merge_series();
  doc_set_series();
  doc_heap_series();
  doc_min_max();
  doc_clamp();
  doc_lexicographical_compare();
  doc_permutation();
  doc_sort_series();
  doc_accumulate_reduce();
  doc_inner_product();
  doc_partial_sum_series();
  doc_adjacent_difference();
  doc_iota();
  doc_gcd_and_lcm();
  doc_midpoint();
  doc_for_each_node();
  doc_list_fns();
  doc_list_unique();
  doc_two_way_search();
  doc_combination_range();
  doc_filter_range();
  doc_take_range();
  doc_drop_range();
  doc_take_while_range();
  doc_drop_while_range();
  doc_join_range();
  doc_join_with_range();
  doc_adjacent_range();
  doc_slide_range();
  doc_aligned_stride_range();
  doc_stride_range();
  doc_aligned_chunk_range();
  doc_chunk_range();
  doc_chunk_by_range();
  doc_inner_cartesian_product_range();
  doc_cartesian_product_range();
  doc_split_range();
  doc_zip_range();
  doc_aligned_zip_range();
  doc_enumerate_range();
  doc_exclusive_rotate_range();
  doc_rotate_range();
  doc_loop_range();
  doc_concat_range();

  doc_string();
  doc_string_reference();
  doc_bitset();
  doc_vector();
  doc_bool_vector();
  doc_local_vector();
  doc_small_vector();
  doc_pointer_vector();
  doc_headed_vector();
  doc_circular_vector();
  doc_deque();
  doc_forward_list();
  doc_list();
  doc_queue();
  doc_stack();
  doc_priority_queue();
  doc_flat_map();
  doc_binary_search_tree_adaptor();
  doc_map_adaptor();
  doc_map_typedef();
  doc_hashtable_adaptor();
  doc_unordered_map_adaptor();
  doc_unordered_map_typedef();
  doc_mixed_map_adaptor();
  doc_mixed_map_typedef();
  doc_stable_vector_adaptor();
  doc_stable_vector_typedef();
  doc_tree_adaptor();
  doc_tree_vector_adaptor();
  doc_tree_typedef();
  doc_matrix();
  doc_dup_compressed_array();

  doc_duration();
  doc_time_point();
  doc_clock();

  doc_win32_enable_utf8_console();
  doc_c_file();
  doc_print_tag();
  doc_sscan();
  doc_sprint();
  doc_big_int();
  doc_ssplitter();
  doc_arithmetic_parser();
  doc_file();

  doc_atomic();
  doc_weak_ptr();
  doc_shared_ptr();
  doc_thread();
  doc_mutex();
  doc_condition_variable();
  doc_mutex_area();
  doc_shared_mutex();
  doc_thread_pool();
  doc_exclusive_thread_pool();

  printf("ok\n");
}

}

int main() {
  re::inner::test::test();
}
