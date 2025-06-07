#include <re/core>
#include <cassert>

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
  //   re::this_thread
  //   re_adl
  //   re_adl::inner
  //
  // used names:
  //   fo_? // for defining function object
}
void doc_inner_back_door_functions() {
  // inner::make<T>(s...)
  // inner::to_mutable(x) // cast iterator to const_iterator
  // inner::node(x) // get the stored pointer from iterator
  // inner::base(x)
  // inner::good(x) // for debug
}
void doc_simple_put_functions() {
  // puti(i) // put integer to stdout
  //
  // is_little_endian_v
  // is_big_endian_v
  // putb(x) // put binary representation of integer or floating point to stdout
}
void doc_std_dependence() {
  // std::size_t // <cstddef>
  // std::ptrdiff_t // <cstddef>
  // std::nullptr_t // <cstddef>
  // std::max_align_t // <cstddef>
  // std::byte // <cstddef>

  // // <cstdint>
  //
  // std::int_least8_t       std::int_fast8_t
  // std::uint_least8_t      std::uint_fast8_t
  // std::int_least16_t      std::int_fast16_t
  // std::uint_least16_t     std::uint_fast16_t
  // std::int_least32_t      std::int_fast32_t
  // std::uint_least32_t     std::uint_fast32_t
  // std::int_least64_t      std::int_fast64_t
  // std::uint_least64_t     std::uint_fast64_t
  // std::intmax_t
  // std::uintmax_t
  //
  // std::intptr_t  // optional
  // std::uintptr_t // optional
  //
  // std::int8_t    // optional
  // std::uint8_t   // optional
  // std::int16_t   // optional
  // std::uint16_t  // optional
  // std::int32_t   // optional
  // std::uint32_t  // optional
  // std::int64_t   // optional
  // std::uint64_t  // optional

  // std::initializer_list // <initializer_list>

  // std::type_info // <typeinfo>
  {
    static_assert(is_same_v<decltype(typeid(int)), const std::type_info &>);
  }

  // std::exception // <exception>
  // std::bad_exception // <exception>
  // std::terminate_handler // <exception>
  // std::get_terminate // <exception>
  // std::set_terminate // <exception>
  // std::terminate // <exception>
  // std::uncaught_exceptions // <exception>
  // std::current_exception // <exception>
  // std::rethrow_exception // <exception>
  // std::exception_ptr // <exception>
  // std::make_exception_ptr // <exception> // wrapped to avoid adl

  // std::nested_exception // <exception>
  // std::throw_with_nested // <exception> // wrapped to avoid adl
  // std::rethrow_if_nested // <exception> // wrapped to avoid adl

  // std::bad_alloc // <new>
  // std::bad_array_new_length // <new>
  // std::bad_typeid // <typeinfo>
  // std::bad_cast // <typeinfo

  // std::logic_error // <stdexcept>
  // std::domain_error // <stdexcept>
  // std::invalid_argument // <stdexcept>
  // std::length_error // <stdexcept>
  // std::out_of_range // <stdexcept>

  // std::runtime_error // <stdexcept>
  // std::range_error // <stdexcept>
  // std::overflow_error // <stdexcept>
  // std::underflow_error // <stdexcept>

  // std::malloc // <cstdlib>
  // std::free // <cstdlib>
  // std::system // <cstdlib>
  // std::getenv // <cstdlib>
  // std::abort // <cstdlib>
  // std::exit // <cstdlib>

  // std::nothrow_t // <new>
  // std::nothrow // <new>
  // std::align_val_t // <new>
  // std::destroying_delete_t // <new>

  // new_handler = void (*)() // <new>
  // std::get_new_handler // <new>
  // std::set_new_handler // <new>

  // FILE = std::FILE // <cstdio>
  // fpos_t = std::fpos_t // <cstdio>
  // std::fopen // <cstdio>
  // std::freopen // <cstdio>
  // std::fclose // <cstdio>
  // std::fflush // <cstdio>
  // std::setbuf // <cstdio>
  // std::setvbuf // <cstdio>
  // std::fread // <cstdio>
  // std::fwrite // <cstdio>
  // std::fgetc // <cstdio>
  // std::getc // <cstdio>
  // std::fputc // <cstdio>
  // std::putc // <cstdio>
  // std::fputs // <cstdio>
  // std::getchar // <cstdio>
  // std::putchar // <cstdio>
  // std::puts // <cstdio>
  // std::ungetc // <cstdio>
  // std::scanf // <cstdio>
  // std::fscanf // <cstdio>
  // std::sscanf // <cstdio>
  // std::printf // <cstdio>
  // std::fprintf // <cstdio>
  // std::sprintf // <cstdio>
  // std::snprintf // <cstdio>
  // std::ftell // <cstdio>
  // std::fgetpos // <cstdio>
  // std::fseek // <cstdio>
  // std::fsetpos // <cstdio>
  // std::rewind // <cstdio>
  // std::clear // <cstdio>
  // std::feof // <cstdio>
  // std::ferror // <cstdio>
  // std::perror // <cstdio>
  // std::rename // <cstdio>
  // std::tmpfile // <cstdio>
  // std::tmpnam // <cstdio>

  // std::isalnum // <cctype>
  // std::isalpha // <cctype>
  // std::islower // <cctype>
  // std::isupper // <cctype>
  // std::isdigit // <cctype>
  // std::isxdigit // <cctype>
  // std::iscntrl // <cctype>
  // std::isgraph // <cctype>
  // std::isspace // <cctype>
  // std::isblank // <cctype>
  // std::isprint // <cctype>
  // std::ispunct // <cctype>
  // std::tolower // <cctype>
  // std::toupper // <cctype>

  // std::strcpy // <cstring>
  // std::strncpy // <cstring>
  // std::strcat // <cstring>
  // std::strncat // <cstring>
  // std::strxfrm // <cstring>
  // std::strlen // <cstring>
  // std::strcmp // <cstring>
  // std::strncmp // <cstring>
  // std::strcoll // <cstring>
  // std::strrchr // <cstring>
  // std::strspn // <cstring>
  // std::strcspn // <cstring>
  // std::strpbrk // <cstring>
  // std::strstr // <cstring>
  // std::strtok // <cstring>
  // std::memchr // <cstring>
  // std::memcmp // <cstring>
  // std::memset // <cstring>
  // std::memcpy // <cstring>
  // std::memmove // <cstring>
  // std::strerror // <cstring>

  // std::is_constant_evaluated // <type_traits>

  // std::source_location
  //   current()
  //   line()
  //   column()
  //   file_name()
  //   function_name()
  {
    const source_location l = source_location::current();
    // assert(l.line() == 250);
    // assert(sview(l.file_name()) == "main.cpp");
  }
}
void doc_std_dependence_fpfns() {
  // ceil // from -inf to +inf
  // floor // from -inf to +inf
  // round // from zero to inf
  // trunc // from inf toward zero

  // NAN
  // INFINITY // positive infinity

  // FP_INFINITE
  // FP_NAN
  // FP_NORMAL
  // FP_SUBNORMAL
  // FP_ZERO
  // fpclassify(x)

  // isfinite(x)
  // isinf(x)
  // isnan(x)
  // isnormal(x)
  // signbit(x)
  // isunordered(x)
}
void doc_print_error() {
  // print_then_terminate(const char *)
  // print_then_terminate(const char *, s...)
  //
  // // throw_or_terminate:
  // //   terminate if RE_NOEXCEPT is defined, throw otherwise
  // throw_or_terminate<E>()
  // throw_or_terminate<E>(const char *)
}
void doc_basic_template_metaprogramming_tools() {
  // declval<T>()
  {
    static_assert(is_same_v<decltype(declval<int>()), int &&>);
    static_assert(is_same_v<decltype(declval<int &&>()), int &&>);
    static_assert(is_same_v<decltype(declval<int &>()), int &>);
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
    static_assert(is_same_v<integer_sequence<int, 0, 1, 2>,
                            make_integer_sequence<int, 3>>);
    static_assert(is_same_v<index_sequence<>, make_index_sequence<0>>);

    static_assert(is_same_v
                  <make_index_sequence_by_stride<3, 4>,
                   index_sequence<0, 4, 8>>);

    static_assert(is_same_v
                  <index_sequence_cat
                   <index_sequence<1>, index_sequence<2>, index_sequence<3, 4>>,
                   index_sequence<1, 2, 3, 4>>);

    static_assert(is_same_v
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

  // f_is_well_formed(_v)<F, S...>
  // return_type_of_f_or(_t)<F, T, S...>
  //   // The result is F::f(type_pack<S...>)::type if it is valid, otherwise T.
  {
    struct check_t {
      static type_tag<int> f(type_pack<int>);
      static type_tag<int> f(type_pack<int, float>);
      static inner::disable f(type_pack<float>);
    };
    static_assert(f_is_well_formed_v<check_t, int>);
    static_assert(f_is_well_formed_v<check_t, int, float>);
    static_assert(!f_is_well_formed_v<check_t, double>);
    static_assert(is_same_v<return_type_of_f_or_t<check_t, void, int, float>,
                            int>);
    static_assert(is_same_v<return_type_of_f_or_t<check_t, void, float>, void>);
  }

  // has_member_type_type(_v)<T>
  // has_member_static_value(_v)<T>
  {
    static_assert(!has_member_type_type_v<int>);
    static_assert(has_member_type_type_v<int_constant<3>>);

    static_assert(!has_member_static_value_v<int>);
    static_assert(has_member_static_value_v<int_constant<3>>);
  }

  // nth_type(_t)<N, S...>
  //   requires: N < sizeof...(S), otherwise get void.
  {
    static_assert(is_same_v<nth_type_t<0>, void>);
    static_assert(is_same_v<nth_type_t<100>, void>);
    static_assert(is_same_v<nth_type_t<0, char, int>, char>);
    static_assert(is_same_v<nth_type_t<1, char, int>, int>);
    static_assert(is_same_v<nth_type_t<2, char, int>, void>);
  }

  // is_one_of_types(_v)<T, S...>
  {
    static_assert(is_one_of_types_v<float, float, double>);
    static_assert(is_one_of_types_v<double, float, double>);
    static_assert(!is_one_of_types_v<int, float, double>);
  }

  // find_type(_v)<T, S...>
  {
    static_assert(find_type_v<char> == 0);
    static_assert(find_type_v<char, int> == 1);
    static_assert(find_type_v<char, char, int, float> == 0);
    static_assert(find_type_v<int, char, int, float> == 1);
    static_assert(find_type_v<float, char, int, float> == 2);
    static_assert(find_type_v<double, char, int, float> == 3);
  }

  // occurs_exactly_once(_v)<T, S...>
  {
    static_assert(!occurs_exactly_once_v<int>);
    static_assert(occurs_exactly_once_v<int, int>);
    static_assert(occurs_exactly_once_v<int, float, int, double>);
    static_assert(!occurs_exactly_once_v<int, int, int>);
    static_assert(!occurs_exactly_once_v<int, int, float, int>);
  }

  // // Following templates are designed for type-to-type calculation between
  // // types of integral_constant<...> or similar classes
  // //
  // // Result value beyond minimal or maximum limit cause unknown result.
  // //
  // // For XY version,
  // // the result type is common_type_t<X::value_type, Y::value_type>.
  // //   // Note: no integer promotion is performed if X is same as Y.
  //
  // compile_time_add(_v/t)<X, Y>
  // compile_time_sub(_v/t)<X, Y>
  // compile_time_mul(_v/t)<X, Y>
  // compile_time_div(_v/t)<X, Y> // the result is 0 if Y is 0
  // compile_time_mod(_v/t)<X, Y>
  // compile_time_neg(_v/t)<X>
  // compile_time_abs(_v/t)<X>
  // compile_time_gcd(_v/t)<X, Y> // greatest common divisor
  // compile_time_lcm(_v/t)<X, Y> // least common multiple
  // compile_time_max(_v/t)<X, Y> // pick X if equal
  // compile_time_min(_v/t)<X, Y> // pick X if equal
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

  // compile_time_acc(_v/t)<FN, FIRST, S...>
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
  {
    static_assert(is_same_v
                  <compile_time_acc_t<type_pack, int, float, void *>,
                   type_pack<type_pack<int, float>::type, void *>::type>);
    static_assert(compile_time_acc_v<compile_time_mul,
                                     size_constant<2>,
                                     size_constant<3>,
                                     size_constant<4>> == 24);
  }

  // type_tag<T>
  //   type = T
  {
    static_assert(is_same_v<typename type_tag<int>::type, int>);
  }


  // // classes worked with type_pack_size and type_pack_element
  // //   type_pack<...>
  // //   T = type_pack<T>
  // //   integer_sequence<INT, ...> = type_pack<integral_constant<>...>
  // //   type_pack_add<...>
  // //   type_pack_mul<...>
  // //   type_pack_first_part<...>
  // //   type_pack_second_part<...>
  //
  // type_pack<X...>
  // type_pack_tag
  //   // Being base class of:
  //   //   type_pack
  //   //   type_pack_add
  //   //   type_pack_mul
  //   //   type_pack_first_part
  //   //   type_pack_second_part
  {
    static_assert(is_base_of_v<type_pack_tag, type_pack<>>);
  }
  // type_pack_size(_v)<T>
  {
    static_assert(type_pack_size_v<int> == 1);
    static_assert(type_pack_size_v<type_pack<>> == 0);
    static_assert(type_pack_size_v<type_pack<int, float>> == 2);
    static_assert(type_pack_size_v<make_index_sequence<5>> == 5);
  }
  // type_pack_element(_t)<N, T>
  //   requires: N < type_pack_size_v<T>, otherwise get unknown type
  {
    static_assert(is_same_v<type_pack_element_t<2, index_sequence<1, 3, 5, 7>>,
                            size_constant<5>>);
  }
  // type_pack_eql(_v)<T1, T2>
  {
    static_assert(type_pack_eql_v
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
          assert((is_same_v<type_pack<S...>, type_pack<type_tag<int>>>));
        else if (sizeof...(x) == 2)
          assert((is_same_v<type_pack<S...>,
                            type_pack<type_tag<int>, type_tag<float>>>));
        else if (sizeof...(x) == 3) {
          assert((is_same_v<nth_type_t<0, S...>, type_tag<size_constant<0>>>));
          assert((is_same_v<nth_type_t<1, S...>, type_tag<size_constant<1>>>));
          assert((is_same_v<nth_type_t<2, S...>, type_tag<size_constant<2>>>));
        }
        else
          assert(false);
      });
  }
  // type_pack_for_each_until_false<T>(f) // may throw
  {
    int c = 0;
    auto f = [&](auto x) {
      ++c;
      return !is_pointer_v<typename decltype(x)::type>;
    };
    type_pack_for_each_until_false<type_pack<int, int, int *, int>>(f);
    assert(c == 3);
  }
  // type_pack_apply(_t)<template <class...> APPLY, P>
  //   type
  {
    static_assert(is_same_v
                  <type_pack_apply_t
                  <type_pack, type_pack_add<int, type_pack<float, void *>>>,
                  type_pack<int, float, void *>>);
  }
  // type_pack_add<X, Y>
  //   type
  // type_pack_mul<X, Y>
  //   type
  //   ()(f) // Apply type_pack_for_each for type of *this
  {
    using P0 = type_pack<void *, void **>;
    using P1 = type_pack<int, long>;
    using P01 = type_pack_mul<P0, P1>;
    using PP = type_pack<type_pack_add<void *, int>,
                         type_pack_add<void *, long>,
                         type_pack_add<void **, int>,
                         type_pack_add<void **, long>>;
    static_assert(type_pack_eql_v<P01, PP>);
  }
  // type_pack_decay(_t)<TYPE_PACK>
  {
    static_assert(is_same_v
                  <type_pack_decay_t
                   <type_pack_add<type_pack<int>, type_pack<float>>>,
                   type_pack<int, float>>);
  }
  // type_pack_recursive_decay(_t)<TYPE_PACK>
  {
    using P = type_pack_mul<type_pack<void *, void **>,
                            type_pack<void ***, void ****>>;
    using PP = type_pack_recursive_decay_t<P>;
    using PPP = type_pack<type_pack<void *, void ***>,
                          type_pack<void *, void ****>,
                          type_pack<void **, void ***>,
                          type_pack<void **, void ****>>;
    static_assert(is_same_v<PP, PPP>);
  }
  // type_pack_recursive_eql(_v)<T1, T2>
  {
    static_assert(type_pack_recursive_eql_v
                  <type_pack<type_pack<int>, float>, type_pack<int, float>>);
    static_assert(!type_pack_recursive_eql_v
                  <type_pack<type_pack<int, long>, float>,
                   type_pack<int, long, float>>);
  }
  // type_pack_first_part<N, P>
  //   requires: P is type_pack, and N <= type_pack_size_v<P>
  // type_pack_second_part<N, P>
  //   requires: P is type_pack, and N <= type_pack_size_v<P>
  {
    using P = type_pack<int, float, void *, void **, void ***>;
    using P0 = type_pack_first_part<3, P>;
    using P1 = type_pack_second_part<3, P>;
    static_assert(type_pack_eql_v<P0, type_pack<int, float, void *>>);
    static_assert(type_pack_eql_v<P1, type_pack<void **, void ***>>);
    static_assert(is_same_v
                  <type_pack_decay_t<P1>, type_pack<void **, void ***>>);
  }
  // n_type_pack(_t)<N, T> // = type_pack<T, T, ..., T>
  {
    static_assert(same_as<n_type_pack_t<0, int>, type_pack<>>);
    static_assert(same_as<n_type_pack_t<1, int>, type_pack<int>>);
    static_assert(same_as<n_type_pack_t<2, int>, type_pack<int, int>>);
    static_assert(same_as<n_type_pack_t<3, int>, type_pack<int, int, int>>);
    static_assert(same_as
                  <n_type_pack_t<6, int>,
                   type_pack<int, int, int, int, int, int>>);
  }

  // conjunction(_v)<S...>
  //   // get first type whose value is false or false_type
  // disjunction(_v)<S...>
  //   // get first type whose value is true or true_type
  // negation(_v)<T>
  //   // get bool_constant<!T::value>
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

  // select_type(_v)<X, S...>
  //   // Use declval<X>() to join function overload resolution,
  //   // then get index of the best one
  //   // or sizeof...(S) if no one wins.
  {
    static_assert(select_type_v<int> == 0);
    static_assert(select_type_v<int, const int &, int &&> == 1);
    static_assert(select_type_v<int &, const int &, int &&> == 0);
    static_assert(select_type_v<int &&, const int &, int &&> == 1);
    static_assert(select_type_v<float *, const int &, int &&> == 2);
  }

  // accumulate_args(add_f, ...) // may throw
  {
    static_assert(accumulate_args(plus<>{}, 1) == 1);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3) == 6);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3, 4, 5) == 15);
  }

  // nth_arg<I>(...) // I and args is valid
  {
    static_assert(nth_arg<2>(1, 2, 3) == 3);
    static_assert(same_as<decltype(nth_arg<2>(1, 2, 3)), int &&>);
  }
}
void doc_swap() {
  // is_swappable_with(_v)<REF1, REF2>
  // is_swappable(_v)<T>
  // is_nothrow_swappable_with(_v)<REF1, REF2>
  // is_nothrow_swappable(_v)<T>
  {
    static_assert(is_swappable_with_v<int &, int &>);
    static_assert(is_swappable_v<int>);
    static_assert(is_nothrow_swappable_with_v<int &, int &>);
    static_assert(is_nothrow_swappable_v<int>);
  }

  // default_swap(x, y) // may throw
  //   // only use move constructor and move assignment operator

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
  // primary type categories
  // is_void(_v)<T>
  {
    static_assert(is_void_v<void>);
    static_assert(is_void_v<const volatile void>);
    static_assert(!is_void_v<void *>);
    static_assert(!is_void_v<int>);
  }
  // is_null_pointer(_v)<T>
  {
    static_assert(is_null_pointer_v<nullptr_t>);
    static_assert(is_null_pointer_v<const volatile nullptr_t>);
    static_assert(!is_null_pointer_v<int>);
  }
  // is_integral(_v)<T>
  {
    static_assert(is_integral_v<bool>);
    static_assert(is_integral_v<int>);
    static_assert(is_integral_v<const volatile int>);
    static_assert(!is_integral_v<float>);
  }
  // is_floating_point(_v)<T>
  {
    static_assert(is_floating_point_v<float>);
    static_assert(is_floating_point_v<double>);
    static_assert(is_floating_point_v<const volatile double>);
    static_assert(!is_floating_point_v<int>);
  }
  // is_array(_v)<T>
  {
    static_assert(is_array_v<int []>);
    static_assert(is_array_v<int [3]>);
    static_assert(!is_array_v<int (&)[3]>);
    static_assert(!is_array_v<int>);
  }
  // is_pointer(_v)<T>
  {
    static_assert(is_pointer_v<int *>);
    static_assert(is_pointer_v<int *const volatile>);
    static_assert(!is_pointer_v<int>);
  }
  // is_lvalue_reference(_v)<T>
  {
    static_assert(is_lvalue_reference_v<int &>);
    static_assert(!is_lvalue_reference_v<int &&>);
    static_assert(!is_lvalue_reference_v<int>);    
  }
  // is_rvalue_reference(_v)<T>
  {
    static_assert(is_rvalue_reference_v<int &&>);
    static_assert(!is_rvalue_reference_v<int &>);
    static_assert(!is_rvalue_reference_v<int>);
  }
  // is_member_object_pointer(_v)<T>
  {
    struct t {};
    // auto pp = &t::j; // pointer to reference member is illegal
    static_assert(is_member_object_pointer_v<int t::*>);
    static_assert(!is_member_object_pointer_v<void (t::*)()>);
    static_assert(!is_member_object_pointer_v<int>);
  }
  // is_member_function_pointer(_v)<T>
  {
    struct A {
      int f();
      int ff(int) &&;
    };
    static_assert(is_member_function_pointer_v<decltype(&A::f)>
                  && is_member_function_pointer_v<const decltype(&A::f)>
                  && is_member_function_pointer_v<volatile decltype(&A::f)>
                  && is_member_function_pointer_v
                  <const volatile decltype(&A::f)>
                  && is_member_function_pointer_v<decltype(&A::ff)>
                  && is_member_function_pointer_v<int (A::*)()>
                  && is_member_function_pointer_v<int (A::*)() &&>
                  && !is_member_function_pointer_v<int A::*const>
                  && !is_member_function_pointer_v<int>);
  }
  // is_enum(_v)<T>
  {
    enum t {a, b, c};
    enum struct tt {aa, bb, cc};
    static_assert(is_enum_v<t>);
    static_assert(is_enum_v<tt>);
    static_assert(is_enum_v<decltype(a)>);
    static_assert(is_enum_v<decltype(tt::aa)>);
  }
  // is_union(_v)<T>
  {
    union t {};
    static_assert(is_union_v<t>);
  }
  // is_class(_v)<T>
  {
    struct t {};
    static_assert(is_class_v<t>);
  }
  // is_function(_v)<T>
  {
    static_assert(is_function_v<void ()>);
    static_assert(is_function_v<void () const &>);
  }

  // // composite type categories
  // is_reference(_v)<T>
  {
    static_assert(is_reference_v<int &>);
    static_assert(is_reference_v<int &&>);
  }
  // is_arithmetic(_v)<T>
  {
    static_assert(is_arithmetic_v<bool>);
    static_assert(is_arithmetic_v<char>);
    static_assert(is_arithmetic_v<const volatile int>);
    static_assert(is_arithmetic_v<float>);
  }
  // is_fundamental(_v)<T>
  // is_compound(_v)<T>
  {
    enum enum_t {a, b, c};
    union union_t {};
    class class_t {};
    static_assert(is_fundamental_v<int>);
    static_assert(is_fundamental_v<float>);
    static_assert(is_fundamental_v<void>);
    static_assert(is_fundamental_v<nullptr_t>);
    static_assert(is_compound_v<int[3]>);
    static_assert(is_compound_v<int *>);
    static_assert(is_compound_v<int &>);
    static_assert(is_compound_v<int ()>);
    static_assert(is_compound_v<enum_t>);
    static_assert(is_compound_v<union_t>);
    static_assert(is_compound_v<class_t>);
    static_assert(is_compound_v<int class_t::*>);
    static_assert(is_compound_v<int (class_t::*)()>);
  }
  // is_object(_v)<T>
  {
    static_assert(!is_object_v<void>);
    static_assert(!is_object_v<int &>);
    static_assert(!is_object_v<int ()>);
  }
  // is_scalar(_v)<T>
  {
    enum enum_t {};
    class t {};
    static_assert(is_scalar_v<int>);
    static_assert(is_scalar_v<float>);
    static_assert(is_scalar_v<nullptr_t>);
    static_assert(is_scalar_v<int *>);
    static_assert(is_scalar_v<int t::*>);
    static_assert(is_scalar_v<int (t::*)()>);
  }
  // is_member_pointer(_v)<T>
  {
    class t {};
    static_assert(is_member_pointer_v<int t::*>);
    static_assert(is_member_pointer_v<int (t::*)()>);
  }
  // is_referenceable(_v)<T>  
  {
    static_assert(!is_referenceable_v<void>);
    static_assert(!is_referenceable_v<void () &>);
    static_assert(!is_referenceable_v<void () const>);
  }

  // // type properties
  // is_const(_v)<T>
  {
    static_assert(is_const_v<const int>);
  }
  // is_volatile(_v)<T>
  {
    static_assert(is_volatile_v<volatile int>);
  }
  // is_trivial(_v)<T>
  {
    struct t1 {t1() = default;};
    struct t2 {t2() {}};
    static_assert(is_trivial_v<int>);
    static_assert(is_trivial_v<t1>);
    static_assert(!is_trivial_v<t2>);
  }
  // is_trivially_copyable(_v)<T>
  {
    static_assert(is_trivially_copyable_v<int>);
    struct t {
      t() = delete;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&) = default;
      t &operator =(t &&) = default;
    };
    static_assert(is_trivially_copyable_v<t>);
  }
  // is_standard_layout(_v)<T>
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
    static_assert(is_standard_layout_v<t>);
    static_assert(is_standard_layout_v<tt>);
    static_assert(!is_standard_layout_v<ttt>);
  }
  // is_empty(_v)<T>
  {
    struct t {};
    struct tt {};
    struct ttt : t, tt {};
    static_assert(is_empty_v<t>);
    static_assert(is_empty_v<tt>);
    static_assert(is_empty_v<ttt>);
  }
  // is_polymorphic(_v)<T>
  // is_abstract(_v)<T>
  {
    struct t {virtual void f() = 0;};
    struct tt {virtual void f() {}};
    static_assert(is_polymorphic_v<t>);
    static_assert(is_polymorphic_v<tt>);
    static_assert(is_abstract_v<t>);
    static_assert(!is_abstract_v<tt>);
  }
  // is_final(_v)<T>
  {
    struct t final {};
    static_assert(is_final_v<t>);
  }
  // is_aggregate(_v)<T>
  {
    struct t {
      int x;
      float y;
    };
    static_assert(is_aggregate_v<t>);
  }
  // is_signed(_v)<T>
  // is_unsigned(_v)<T>
  {
    static_assert(is_signed_v<int>);
    static_assert(is_unsigned_v<bool>);
    static_assert(!is_unsigned_v<void> && !is_signed_v<void>);
  }
  // is_bounded_array(_v)<T>
  // is_unbounded_array(_v)<T>
  {
    static_assert(is_bounded_array_v<int [1]>);
    static_assert(is_unbounded_array_v<int []>);
  }
  // is_constructible(_v)<T, S...>
  // is_default_constructible(_v)<T>
  // is_copy_constructible(_v)<T>
  // is_move_constructible(_v)<T>
  // is_assignable(_v)<L_REF, R_REF>
  // is_copy_assignable(_v)<T>
  // is_move_assignable(_v)<T>
  // is_destructible(_v)<T>

  // is_trivially_constructible(_v)<T, S...>
  // is_trivially_default_constructible(_v)<T>
  // is_trivially_copy_constructible(_v)<T>
  // is_trivially_move_constructible(_v)<T>
  // is_trivially_assignable(_v)<L_REF, R_REF>
  // is_trivially_copy_assignable(_v)<T>
  // is_trivially_move_assignable(_v)<T>
  // is_trivially_destructible(_v)<T>

  // is_nothrow_constructible(_v)<T, S...>
  // is_nothrow_default_constructible(_v)<T>
  // is_nothrow_copy_constructible(_v)<T>
  // is_nothrow_move_constructible(_v)<T>
  // is_nothrow_assignable(_v)<L_REF, R_REF>
  // is_nothrow_copy_assignable(_v)<T>
  // is_nothrow_move_assignable(_v)<T>
  // is_nothrow_destructible(_v)<T>

  // has_virtual_destructor(_v)<T>
  {
    struct t {virtual ~t() {}};
    static_assert(has_virtual_destructor_v<t>);
  }

  // has_unique_object_representations(_v)<T>

  // // type property queires
  // alignment_of(_v)<T>
  {
    static_assert(alignment_of_v<char> == alignof(char));
  }

  // rank(_v)<T>
  {
    static_assert(rank_v<int> == 0u);
    static_assert(rank_v<int []> == 1u);
    static_assert(rank_v<int [][2]> == 2u);
    static_assert(rank_v<int [][2][3]> == 3u);
  }
  // extent(_v)<T, I = 0>
  {
    static_assert(extent_v<int> == 0);
    static_assert(extent_v<int []> == 0);
    static_assert(extent_v<int [3]> == 3);
    static_assert(extent_v<int [3][4]> == 3);
    static_assert(extent_v<int [3][4], 0> == 3);
    static_assert(extent_v<int [3][4], 1> == 4);
  }

  // // type relations
  // is_same(_v)<T1, T2>
  {
    static_assert(is_same_v<void, void>);
  }
  // is_base_of(_v)<T1, T2>
  {
    struct t {};
    struct tt : t {};
    struct ttt : private t {};
    static_assert(!is_base_of_v<int, int>);
    static_assert(is_base_of_v<t, t>);
    static_assert(is_base_of_v<t, tt>);
    static_assert(is_base_of_v<t, ttt>);
  }
  // is_convertible(_v)<T1, T2>
  {
    static_assert(is_convertible_v<int &, const int &>);
    static_assert(!is_convertible_v<const int &, int &>);
  }
  // is_nothrow_convertible(_v)<T1, T2>
  {
    struct t {};
    struct tt {tt(t) {}};
    static_assert(is_nothrow_convertible_v<int &, const int &>);
    static_assert(!is_nothrow_convertible_v<t, tt>);
    static_assert(!is_nothrow_convertible_v<const int &, int &>);
  }

  // // const-volatile modifications
  // remove_const(_t)<T>
  // remove_volatile(_t)<T>
  // remove_cv(_t)<T>
  // add_const(_t)<T>
  // add_volatile(_t)<T>
  // add_cv(_t)<T>

  // // reference modifications
  // remove_reference(_t)<T>
  // add_lvalue_reference(_t)<T>
  // add_rvalue_reference(_t)<T>

  // // sign modifications
  // make_signed(_t)<T>
  // make_unsigned(_t)<T>
  {
    static_assert(is_same_v<make_signed_t<unsigned>, int>);
    static_assert(is_same_v<make_unsigned_t<int>, unsigned>);
    static_assert(is_same_v<make_unsigned_t<unsigned>, unsigned>);
  }

  // // array modifications
  // remove_extent(_t)<T>
  // remove_all_extents(_t)<T>
  {
    static_assert(is_same_v<remove_extent_t<int>, int>);
    static_assert(is_same_v<remove_extent_t<int []>, int>);
    static_assert(is_same_v<remove_extent_t<int [][3]>, int[3]>);

    static_assert(is_same_v<remove_all_extents_t<int [2][3]>, int>);
  }

  // // pointer modifications
  // remove_pointer(_t)<T>
  // add_pointer(_t)<T>
  {
    static_assert(is_same_v<remove_pointer_t<int>, int>);
    static_assert(is_same_v<remove_pointer_t<int *>, int>);

    static_assert(is_same_v<add_pointer_t<int>, int *>);
    static_assert(is_same_v<add_pointer_t<int &>, int *>);

    static_assert(is_same_v<add_pointer_t<void>, void *>);
    static_assert(is_same_v<add_pointer_t<void ()>, void (*)()>);
    static_assert(is_same_v<add_pointer_t<void () &&>, void () &&>);
  }

  // // other transformations
  // type_identity(_t)<T>
  {
    static_assert(is_same_v<type_identity_t<int>, int>);
  }
  // remove_cvref(_t)<T>
  {
    static_assert(is_same_v<remove_cvref_t<const volatile int &>, int>);
  }
  // decay(_t)<T>
  {
    static_assert(is_same_v<decay_t<int [2][3]>, int (*)[3]>);
    static_assert(is_same_v<decay_t<int []>, int *>);
    static_assert(is_same_v<decay_t<int ()>, int (*)()>);
    static_assert(is_same_v<decay_t<int (&&)()>, int (*)()>);
    static_assert(is_same_v<decay_t<const int &>, int>);
  }
  // enable_if(_t)<T>
  {
    static_assert(is_same_v<enable_if_t<true, int>, int>);
    static_assert(is_same_v<enable_if_t<true>, void>);
    static_assert(!has_member_type_type_v<enable_if<false>>);
  }
  // conditional(_t)<Y, T1, T2>
  {
    static_assert(is_same_v<conditional_t<true, char, int>, char>);
    static_assert(is_same_v<conditional_t<false, char, int>, int>);
  }
  // common_type(_t)<...>
  {
    static_assert(!has_member_type_type_v<common_type<>>);
    static_assert(is_same_v<common_type_t<char, char>, char>);
    static_assert(is_same_v<common_type_t<short, char>, int>);
    static_assert(is_same_v<common_type_t<char, unsigned>, unsigned>);
    static_assert(is_same_v<common_type_t<float, double>, double>);
    static_assert(is_same_v<common_type_t<int, float>, float>);
    static_assert(is_same_v
                  <common_type_t<volatile int &&, const float &>, float>);
    struct t {};
    struct tt : t {};
    struct ttt : tt {};
    static_assert(is_same_v<common_type_t<t, tt, ttt>, t>);
  }
  // basic_common_reference<T, U, <class> TQUAL, <class> UQUAL>
  // commmon_reference(_t)<S...>
  {
    static_assert(is_same_v
                  <common_reference_t<volatile char &, const char &>,
                   const volatile char &>);
    struct t {};
    struct tt : t {};
    static_assert(is_same_v
                  <common_reference_t<volatile t &, const tt &>,
                   const volatile t &>);

    struct s {};
    struct ss {operator s();};
    struct sss : s {};
    static_assert(is_same_v<common_reference_t<s &, ss &>, s>);
    static_assert(is_same_v<common_reference_t<s &, sss &>, s &>);
  }
  // underlying_type(_t)<X>
  //   // If X is not enum, there is no member type.
  {
    enum t {a, b, c};
    static_assert(is_integral_v<underlying_type_t<t>>);
  }
  // void_t<S...>
  {
    static_assert(is_void_v<void_t<int, float, void *>>);
  }

  // uses_allocator(_v)<T, ALLOC>
  {
    struct X {
      using allocator_type = float;
    };
    static_assert(uses_allocator_v<X, float>);
    static_assert(uses_allocator_v<X, int>);
    static_assert(uses_allocator_v<X, char>);
    static_assert(!uses_allocator_v<X, void *>);
    static_assert(!uses_allocator_v<X, void>);
  }

  // max_align_of_types(_v)<S...>
  // max_size_of_types(_v)<S...>
  {
    static_assert(max_align_of_types_v<> == 1u);
    static_assert(max_align_of_types_v<char, long double, int>
                  == alignof(long double));

    static_assert(max_size_of_types_v<> == 0u);
    static_assert(max_size_of_types_v<int, long double, char>
                  == sizeof(long double));
  }
  // type_t<T, S...>
  {
    static_assert(is_void_v<type_t<void>>);
    static_assert(is_void_v<type_t<void, int, float, void *>>);
    static_assert(is_same_v<type_t<int>, int>);
    static_assert(is_same_v<type_t<int, int, float, void *>, int>);
  }
  // is_implicitly_constructible(_v)<T, S...>
  // is_implicitly_default_constructible(_v)<T>
  {
    struct t {t();};
    struct t2 {explicit t2();};
    static_assert(is_implicitly_constructible_v<t>);
    static_assert(!is_implicitly_constructible_v<t2>);
    static_assert(is_implicitly_default_constructible_v<t>);
    static_assert(!is_implicitly_default_constructible_v<t2>);
  }
  // is_trivial_empty(_v)<T>
  {
    struct t {};
    static_assert(is_trivial_v<is_trivial_empty<t>>);
    static_assert(is_empty_v<is_trivial_empty<t>>);
    static_assert(is_trivial_empty_v<t>);
  }
  // copy_const(_t)<FROM, TO>
  // copy_volatile(_t)<FROM, TO>
  // copy_lvalue_reference(_t)<FROM, TO>
  // copy_rvalue_reference(_t)<FROM, TO>
  // copy_reference(_t)<FROM, TO>
  // copy_cv(_t)<FROM, TO>
  // copy_cvref(_t)<FROM, TO>
  {
    static_assert(is_same_v
                  <copy_cvref_t<const volatile int &, char>,
                   const volatile char &>);
    static_assert(is_same_v
                  <copy_cvref_t<const volatile int &, void>,
                   const volatile void>);
  }

  // is_nothrow_movable(_v)<T>
  // is_nothrow_copyable(_v)<T>
  {
    static_assert(is_nothrow_movable_v<unique_ptr<int>>);
    static_assert(is_nothrow_copyable_v<int>);
  }

  // is_compatible_pointer_with(_v)<Y, T>
  {
    struct t {};
    struct tt : t {};

    static_assert(is_compatible_pointer_with_v<t *, const volatile t *>);

    static_assert(is_compatible_pointer_with_v<tt *, t *>);
    static_assert(is_compatible_pointer_with_v<tt *, const volatile t *>);

    static_assert(!is_compatible_pointer_with_v<tt (*)[], t (*)[]>);
    static_assert(is_compatible_pointer_with_v<t (*)[3], t (*)[]>);
    static_assert(is_compatible_pointer_with_v
                  <t (*)[3], const volatile t (*)[]>);

    static_assert(!is_compatible_pointer_with_v<tt (*)[3], t (*)[3]>);
    static_assert(is_compatible_pointer_with_v<t (*)[3], t (*)[3]>);
  }
}
void doc_basic_utility_functions() {
  // forward<T>(x)
  // move(x)
  // move_if_noexcept(x)

  // exchange(old_val, new_val)->old_val // may throw

  // as_const(x)

  // // cmp_***: only apply for integral argument
  // cmp_equal(x, y)
  // cmp_not_equal(x, y)
  // cmp_less(x, y)
  // cmp_less_equal(x, y)
  // cmp_greater(x, y)
  // cmp_greater_equal(x, y)
  // in_range<INT>(x)
  {
    static_assert(cmp_less(-1, 1ull));
    static_assert(in_range<signed char>(127));
    static_assert(!in_range<signed char>(128));
    static_assert(in_range<signed char>(-128));
    static_assert(!in_range<signed char>(-129));
  }

  // to_underlying(e) // requires: e is enum
  {
    enum class t : char {x, y, z};
    static_assert(is_same_v<decltype(to_underlying(t::x)), char>);
    assert(to_underlying(t::x) == 0);
  }

  // addressof(x) // no constexpr if user-defined operator-> is selected
  {
    int x{};
    const volatile int y{};
    assert(addressof(x) == &x);
    assert(addressof(y) == &y);
  }

  // align(a, sz, p, n) // requires: a is 2 ^ n, a > 0
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
}
void doc_pointer_traits() {
  // pointer_traits<P> // from std <memory>
  //   pointer
  //   element_type
  //   difference_type
  //   rebind<X>
  //   pointer_to(x)
  //   to_address(x) // may be provided by user defined specialization
  // pointer_element_t<P>
  // pointer_difference_t<P>
  // pointer_rebind_t<P, X>
  // pointer_to<PTR>(x) // return PTR rebound to remove_reference_t<decltype(x)>
  {
    using pt = pointer_traits<const int *>;
    static_assert(is_same_v<pt::pointer, const int *>);
    static_assert(is_same_v<pt::element_type, const int>);
    static_assert(is_same_v<pt::difference_type, ptrdiff_t>);
    static_assert(is_same_v<pt::rebind<char>, char *>);
  }

  // to_address(p) // may throw
  // can_apply_to_address<T>
  {
    static_assert(can_apply_to_address<int *>);
    static_assert(!can_apply_to_address<int>);
  }
}
void doc_reference_wrapper_and_invoke() {
  // reference_wrapper<T>
  //   special member functions: no default constructor
  //   reference_wrapper(U &&)
  //   get()
  //   operator ()(...) // may throw
  // reference_wrapper(T &)->reference_wrapper<T>
  // is_reference_wrapper(_v)<T>
  // unwrap_reference(_t)<T>
  // unwrap_ref_decay(_t)<T>
  // ref(x)
  // cref(x)
  {
    static_assert(is_reference_wrapper_v<reference_wrapper<int>>);
    static_assert(same_as
                  <unwrap_reference_t<const reference_wrapper<int>>,
                   const reference_wrapper<int>>);
    static_assert(same_as
                  <unwrap_ref_decay_t<const reference_wrapper<int>>,
                   int &>);
    static_assert(same_as
                  <unwrap_reference_t<reference_wrapper<int>>,
                   int &>);
  }

  // is_invocable(_v)<F, S...>
  // is_nothrow_invocable(_v)<F, S...>
  // invoke_result(_t)<F, S...>
  // invoke(f, s...) // may throw
  //
  // is_invocable_r(_v)<R, F, S...> // from std <type_traits>
  // is_nothrow_invocable_r(_v)<R, F, S...> // from std <type_traits>
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
  // assignable_from<L, R>
  // destructible<T>
  // constructible_from<T, S...>
  // default_initializable<T>
  // move_constructible<T>
  // copy_constructible<T>
  //
  // ranges::swap(x, y)
  // swappable<T>
  // swappable_with<A, B>
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
  // movable<T>
  // copyable<T>
  // semiregular<T> // have all special member functions
  // regular<T> // semiregular + equality_comparable
  {
    static_assert(movable<int>);
    static_assert(copyable<int>);
    static_assert(semiregular<int>);
    static_assert(regular<int>);
  }
  // invocable<F, S...>
  // regular_invocable<F, S...>
  // predicate<F, S...>
  // relation<R, A, B>
  // equivalence_relation<R, A, B>
  // strict_weak_order<R, A, B>
  {
    static_assert(invocable<int (*)()>);
    static_assert(invocable<int (*)(int), int>);
    static_assert(regular_invocable<int (*)()>);
    static_assert(regular_invocable<int (*)(int), int>);
    static_assert(predicate<bool (*)()>);
    static_assert(predicate<bool (*)(int), int>);
    static_assert(relation<bool (*)(int, int), int, int>);
    static_assert(equivalence_relation<bool (*)(int, int), int, int>);
    static_assert(strict_weak_order<bool (*)(int, int), int, int>);
  }
}
void doc_concept_constrained_comparisons() {
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
    using T = integral_traits<int>;
    static_assert(T::is_signed);
    static_assert(T::is_specialized);
    static_assert(T::min() < T::max());

    static_assert(!integral_traits<void>::is_specialized);
  }
}
void doc_numeric_limits() {
  // numeric_limits<INT_OR_FLOAT>
  //   static constexpr bool is_specialized = true
  //   static constexpr bool is_signed
  //   static constexpr lowest() // minimal infinite value
  //   static constexpr denorm_min() // only for floating point
  //   static constexpr min()
  //   static constexpr max()
  //   static constexpr bool is_integer
  //
  //   static infinity() // only for floating point
  //   static nan() // only for floating point
  //   static has_infinity(f) // only for floating point
  //   static has_denorm(f) // only for floating point
  //   static has_nan(f) // only for floating point
}
void doc_three_way_comparison() {
  // std::partial_ordering
  // std::weak_ordering
  // std::strong_ordering

  // is_eq(ordering)
  // is_neq(ordering)
  // is_lt(ordering)
  // is_lteq(ordering)
  // is_gt(ordering)
  // is_gteq(ordering)

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

  // common_comparison_category(_t)<S...>
  {
    static_assert(same_as<common_comparison_category_t<>, strong_ordering>);
    static_assert(same_as
                  <common_comparison_category_t
                   <partial_ordering, strong_ordering, weak_ordering>,
                   partial_ordering>);
    static_assert(is_void_v
                  <common_comparison_category_t
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

  // compare_three_way_result(_t)<A, B>
  // compare_three_way
  //   typename is_transparent
  //   operator ()(a, b) // may throw
  // can_apply_compare_three_way<A, B>
  {
    static_assert(!has_member_type_type_v
                  <compare_three_way_result<void *, int>>);
    static_assert(is_same_v
                  <compare_three_way_result_t<int, int>, strong_ordering>);
    static_assert(is_same_v
                  <compare_three_way_result_t<float, float>, partial_ordering>);

    assert(compare_three_way{}(0, 0.0) == 0);

    static_assert(can_apply_compare_three_way<const int &, const int &>);
  }

  // synth_3way(a, b) // may throw
  // synth_3way_result<T, U = T> // may throw
  {
    int a = 0, b = 1;
    assert(synth_3way(a, b) < 0);
    static_assert(is_same_v<synth_3way_result<int, int>, strong_ordering>);
  }
}
void doc_type_index() {
  // type_index // from std <typeindex>
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
  // is_transparent_function(_v)<T>
  // plus<T = void>
  //   typename is_transparent // enable if T == void
  //   ()(cx, cy) // may throw
  // minus<T = void>
  // multiplies<T = void>
  // divides<T = void>
  // modulus<T = void>
  // negate<T = void>
  // equal_to<T = void>
  // not_equal_to<T = void> // from std <functional>
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
    static_assert(is_transparent_function_v<plus<>>);
    static_assert(!is_transparent_function_v<plus<int>>);
  }

  // can_apply_plus<X, Y>
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

  // mem_fn(fp) // from std <functional>
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

  // tuple_size(_v)<T>
  // tuple_element(_t)<I, T>
  {
    using A = array<int, 2>;
    using T = tuple<char, int>;

    static_assert(tuple_size_v<A> == 2);
    static_assert(tuple_size_v<T> == 2);

    static_assert(is_same_v<tuple_element_t<0, A>, int>);
    static_assert(is_same_v<tuple_element_t<1, A>, int>);
    static_assert(is_same_v<tuple_element_t<0, T>, char>);
    static_assert(is_same_v<tuple_element_t<1, T>, int>);
    static_assert(is_same_v<tuple_element_t<1, const T>, const int>);
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

  // min_pair<A, B>
  //   first
  //   second
  // min_tuple<S...>
  //   value
  //   following // enable if sizeof...(S) > 1
  {
    static_assert(semiregular<min_pair<int, float>>);
    static_assert(!is_copy_assignable_v<min_pair<int, float &>>);
    static_assert(semiregular<min_tuple<int, float, double>>);
    static_assert(!is_copy_assignable_v<min_tuple<int, float &, double>>);

    min_pair<int, float> t0{1, 2.0f};
    assert(pair(t0.first, t0.second) == pair(1, 2.0f));

    min_tuple<int, float, double> t{1, 2.0f, 3.0};
    assert(t.value == 1);
    assert(t.following.value == 2.0f);
    assert(t.following.following.value == 3.0);
  }

  // pair<A, B> = tuple<A, B>
  {
    pair p(1, 2);
    static_assert(same_as<decltype(p), tuple<int, int>>);
  }

  // common_reference<CVREF tuple<...S1>, CVREF tuple<...S2>>
  {
    struct t {};
    struct tt : t {};
    static_assert(is_same_v
                  <typename common_reference
                   <tuple<int &, t &>, tuple<long long, tt &>>::type,
                   tuple<long long, t &>>);
    static_assert(is_same_v
                  <typename common_reference
                   <tuple<int &, t &>, const tuple<long long, tt> &>::type,
                   tuple<long long, const t &>>);
    static_assert(is_same_v<common_reference_t<int &, long long &>, long long>);
    static_assert(is_same_v<common_reference_t<t &, const tt &>, const t &>);
  }

  // common_type<CVREF tuple<...S1>, CVREF tuple<...S2>>
  {
    struct t {};
    struct tt : t {};
    static_assert(is_same_v
                  <common_type_t<tuple<int, t *>, tuple<long long, tt *>>,
                   tuple<long long, t *>>);
  }

  // get<I>(tuple REF) // requires legal I
  // get<T>(tuple REF) // requires T is unique
  // at<I>(tuple REF)  // requires legal I
  // as<T>(tuple REF)  // requires T is unique
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

  // uses_allocator(_v)<tuple<...>, AL> // always true
  // uses_allocator_construction_args(al, s...) // may throw
  {
    static_assert(uses_allocator_v<tuple<>, int>);
    static_assert(uses_allocator_v<tuple<int, int>, int>);
    static_assert(uses_allocator_v<tuple<int, int, int>, char>);

    using t = decltype(uses_allocator_construction_args<tuple<int>>(1, 2));
    static_assert(tuple_size_v<t> == 3);
    static_assert(is_same_v<tuple_element_t<0, t>, allocator_arg_t>);
    static_assert(is_same_v<tuple_element_t<1, t>, const int &>);
    static_assert(is_same_v<tuple_element_t<2, t>, int &&>);
  }

  // make_tuple(s...) // may throw
  // make_pair(x, y) // may throw
  {
    const auto t = make_tuple(1, 2, 3);
    const int i = 0;
    const auto t2 = make_pair(1, ref(i));
    assert(t == make_tuple(1, 2, 3));
    assert(t2 == make_tuple(1, 0));
    static_assert(is_same_v<decltype(t), const tuple<int, int, int>>);
    static_assert(is_same_v<decltype(t2), const tuple<int, const int &>>);
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
      X(int i, int j) : i(i), j(j) {}
    };
    X x = make_from_tuple<X>(make_tuple(1, 2));
    assert(x.i == 1 && x.j == 2);
  }

  // tuple<S...> // from std <tuple>
  //   explicit(...) tuple() // may throw
  //     // value initialize S...
  //   ~tuple()
  //   tuple(const tuple &) // may throw
  //   tuple &operator =(const tuple &) // may throw
  //   tuple(tuple &&) // may throw
  //   tuple &operator =(tuple &&) // may throw
  //   swap(tuple &) // may throw
  //   // note: tuple include reference member is assignable but struct can not
  //
  //   tuple(piecewise_construct, ...) // may throw
  //     // note: not from std (borrowed from std::pair) 
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
  //   at<I>() // requires: legal I
  //   as<T>() // requires: T is unique among S...
  // == <=> // may throw

  // bind_tuple(t, f) // may throw
  {
    assert(bind_tuple(tuple(1, 2, 3), [](int x) {return x * 2;})
           == tuple(2, 4, 6));
  }
}
void doc_bind() {
  // is_bind_expression(_v)<T>
  {
    static_assert(is_bind_expression_v<inner::be<plus<>, tuple<>>>);
    static_assert(!is_bind_expression_v<int>);
  }

  // inline namespace placeholders {
  // _1
  // _2
  // ...
  // }
  // is_placeholder(_v)<T>
  {
    static_assert(is_placeholder_v<decltype(_1)>);
    static_assert(!is_placeholder_v<int>);
  }

  // bind(f, s...) // may throw
  // bind_r<T>(f, s...) // may throw
  //
  // decltype(bind(_r<T>)(...))
  //   special member functions: dependent // may throw
  //   ()(...) // may throw
  {
    assert(bind(minus<>(), _1, _2)(3, 4) == -1);
    assert(bind(minus<>(), _2, _1)(3, 4) == 1);

    int x = 1, y = 2, z = 3;
    const auto assign = [](auto &&x, auto &&y)->auto & {
      x = y;
      return x;
    };
    bind(assign, x, bind(plus<>(), y, z)())();
    assert(x == 1 && y == 2 && z == 3);
    bind(assign, ref(x), bind(plus<>(), y, z)())();
    assert(x == 5 && y == 2 && z == 3);
  }

  // bind_front(f, s...) // may throw
  // bind_back(f, s...) // may throw
  //
  // decltype(bind_front/back(s...)) // storw decay_t for function and args
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
  //   special member functions:
  //     no assignment operator
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
  // is_in_place_type(_v)<T>
  //
  // nullopt_t
  // nullopt
  //
  // class bad_optional_access : public exception
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
  //   constexpr explicit(...) optional(x) // may throw
  //   =(x) // may throw
  //
  //   explicit(...) optional(const optional<U> &) // may throw
  //   =(const optional<U> &) // may throw
  //   explicit(...) optional(optional<U> &&) // may throw
  //   =(optional<U> &&) // may throw
  //
  //   T &emplace(s...) // may throw (weak) // requires: no self-emplace
  //   T &emplace({...}, s...) // may throw (weak) // requires: no self-emplace
  //
  //   -> // has_value()
  //   * // has_value()
  //
  //   empty()
  //   clear()
  //   constexpr explicit operator bool()
  //   has_value()
  //   value() // may throw (throw bad_optional_access if there is no value)
  //   value_or(x) // may throw
  //   reset()
  //
  //   and_then(f) // may throw
  //     // return invoke(f, value()) or {}
  //   transform(f) // may throw
  //     // return optional<U>(invoke(f, value())) or optional<U>{}
  //   or_else(f) // may throw
  //     // return *this or f()
  //
  //   ()(...) // used to make function default-constructible
  //
  // make_optional(in_place_type<T>, s...)->optional<T> // may throw
  // make_optional(in_place_type<T>, {...}, s...)->optional<T> // may throw
  // make_optional(x)->optional<decay_t> // may throw
}
void doc_semiregular_function() {
  // assign_non_assignable<F>(to, from) // may throw
  //   // requires:
  //   //   F is copy constructible
  //   //   F can not throw from copy/move constructor
  // copyable_wrapper<F>
  //   // requires:
  //   //   F is copy constructible
  //   //   F can not throw from copy/move constructor
  //   special member functions: copyable, may be default constructible
  //   explicit copyable_wrapper(f)
  //   ->
  //   *
  //   base()->F
  //
  // call_or_deref_call(f, s...) // may throw
  //
  // semiregular_function<F>
  //   // requires:
  //   //   F is copy constructible
  //   //   F can not throw from default constructible
  //   //   F can not throw from copy/move constructor
  //   special member functions: full
  //   explicit semiregular_function(F)
  //   ()(s...) // may throw
  //   base()->F
  // semiregular_fn(f)
}
void doc_hash() {
  // is_nothrow_hashable(_v)<T>
  //
  // hash<T = void>
  //   is_transparent = ...
  //   result_type = size_t
  // hash<short>
  //   argument_type = short
  //   result_type = size_t
  // hash<unsigned short>
  // hash<int>
  // hash<unsigned int>
  // hash<long>
  // hash<unsigned long>
  // hash<long long>
  // hash<unsigned long long>
  // hash<char>
  // hash<unsigned char>
  // hash<char16_t>
  // hash<char32_t>
  // hash<wchar_t>
  // hash<bool>
  // hash<T *>
  // hash<float>
  // hash<double>
  // hash<nullptr_t>
  // hash<type_index>
  // hash<optional<T>>
  // hash<tuple<T1, T2>>
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

  // // min/max_value:
  // //   call by value, return common_type, return first if equal
  // min_value(x, y) // may throw
  // max_value(x, y) // may throw
  {
    static_assert(min_value(1, 2) == 1);
    static_assert(max_value(1, 2) == 2);
  }

  // conditionally_enable_copy_and_move<Y1, Y2, Y3, Y4>
  {
    constexpr bool y1 = true, y2 = false, y3 = true, y4 = false;
    using t = conditionally_enable_copy_and_move<y1, y2, y3, y4>;
    static_assert(is_copy_constructible_v<t> == y1);
    static_assert(is_copy_assignable_v<t> == y2);
  }

  // empty_function
  //   typename is_transparent
  //   void ()(...)
  {
    empty_function{}(1, 2, 3, 4, 5);
  }

  // inner::exit_fn_t<F>
  //   special member functions: only move constructible
  //   explicit exit_fn_t(f) // may throw
  // inner::switchable_exit_fn_t<F>
  //   special member functions: only move constructible
  //   explicit switchable_exit_fn_t(f) // may throw
  //   explicit operator bool()
  //   set()
  //   set(y)
  //   unset()
  // exit_fn(f) // may throw // requires: f can not throw
  // exit_fn(f, y) // may throw // requires: f can not throw
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

  // copy(x) // may throw
  // copy_if_rvalue(x) // may throw
  {
    assert(copy(1) == 1);
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

    const auto ff = [](int &&) {};
    int i = 0;
    static_assert(is_lvalue(i));
    ff(move(i));

    int *p = &i;
    assert(deref(p) == i);

    int x = 1, y = 2;
    assign_to(x, y);
    assert(x == 1 && y == 1);
  }

  // is_allocator_aware(_v)<T>
  {
    struct X {
      using allocator_type = void;
    };
    static_assert(is_allocator_aware_v<X>);
  }

  // ccast<T>(x) // may throw
  //   // c language style cast
  {
    assert(ccast<float>(4) == 4.0f);
  }

  // bit_cast<T>(x) // from std <bit>
  {
    float x = 0;
    int32_t y = bit_cast<int32_t>(x);
    x = bit_cast<float>(y);
    assert(x == 0);
  }

  // to_unsigned(x) // requires: x is integral
  // to_signed(x) // requires: x is integral

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
  //   requires: x and y are the same integral type
  {
    assert(div_round_up(2, 3) == 1);
    assert(div_round_up(-3, 2) == -2);
  }

  // div_round(x, y) // may throw logic_error if y == 0
  //   requires: x and y are the same integral type
  {
    assert(div_round(5, 4) == 1);
    assert(div_round(6, 4) == 2);
    assert(div_round(-5, 4) == -1);
    assert(div_round(-6, 4) == -2);
  }

  // integral_cast<T>(x) // may throw length_error if unrepresentable
  //   // cast between integral types

  // call_by_n_times(f, x, n) // may throw
  //   // requires: n > 0
  assert(call_by_n_times(bind(plus{}, _1, 1), 1, 1) == 2);
  assert(call_by_n_times(bind(plus{}, _1, 1), 1, 3) == 4);

  // non_propagating_cache<T>
  //   value_type = T
  //   special member functions: full
  //   non_propagating_cache(in_place, s...) // may throw
  //   emplace(s...) // may throw (weak)
  //   emplace_or_assign(x) // may throw
  //   clear()
  //   empty()
  //   ->
  //   *() // requires: !empty()

  // simple_wrapper<T>
  //   special member functions: full
  //   simple_wrapper(in_place, s...) // may throw
  //   ->
  //   * // requires: !empty()

  // copy_and_swap(from, to) // may throw
  //   // do move_and_swap: copy_and_swap(move(from), to)
  // copy_and_move(from, to) // may throw

  // derivable_wrapper<T>
  //   special member functions: dependent
  //   explicit derivable_wrapper(const T &)
  //   explicit derivable_wrapper(T &&)
  //   derivable_wrapper(in_place, s...)
  //   ->
  //   *

  // container_regular_max_size<C>()

  // function_return_false(f, s...)

  // l_shift<T>(x, y)
  // r_shift<T>(x, y)
  // b_and<T>(x, y)
  // b_or<T>(x, y)
  // b_xor<T>(x, y)

  // add_with_check(integrals...)->optional<integral>
  // sub_with_check(integrals...)->optional<integral>
  // sub_to_zero_at_most(INT, INT, ...)->INT // all args greater than zero

  // max_uint_of_max_size<N>
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
  //   static constexpr uint_t f_max
  //
  //   static is_positive(x)->bool
  //   static is_negative(x)->bool
  //   static e(x)->uint_t
  //   static f(x)->uint_t
  //
  //   static is_normalized(x)
  //   static is_denormalized(x)
  //   static is_infinity(x)
  //   static is_nan(x)
  //
  //   static normalized_final_e(x)->int_t
  //   static normalized_final_f(x)->pair<uint_t, int_t>
  //
  //   static denormalized_final_e(x)->int_t
  //   static denormalized_final_f(x)->pair<uint_t, int_t>
  //
  //   static float_t positive_inf()
  //   static float_t negative_inf()
  //   static float_t positive_nan()
  //   static float_t negative_nan()
  //
  //   static constexpr bool is_specialized = true
  //   static constexpr bool is_signed = true
  //   static constexpr bool is_integer = false
  //   static float_t lowest() // min negative value
  //   static float_t denorm_min() // positive denorm_min
  //   static float_t min() // positive norm_min
  //   static float_t max()
  //
  // float_traits = floating_point_traits<float>
  // double_traits = floating_point_traits<double>
}
void doc_common_math() {
  // abs(x)
  // sqrt(x)
  // square(x)
  // sin(x)
  // cos(x)
  // tan(x)
  // approx_equal(x, y, max_dif)
}

// test
void doc_owner_ptr() {
  // owner_ptr<T>
  //   special member functions: full // copy may throw
  //   owner_ptr(nullptr_t)
  //   == with nullptr
  //   explicit owner_ptr(T x) // may throw
  //   * // requires !empty()
  //   ->
  //   empty()
  //   clear()
  {
    owner_ptr<int> p;
    assert(p.empty());
    owner_ptr pp(1), ppp(2);
    assert(pp != nullptr && *pp == 1);
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
  // // if failed, print then terminate
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
  // // if failed, print then terminate
  // test_equality(a, b) // may throw
  // test_inequality(a, b) // may throw
  // test_lessness(a, b) // may throw

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
  //   instance_counter(s...) // may throw
  //   operator ->()
  //   operator *()
  //   operator T_REF()
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
  //   exception_countdown(s...) // may throw
  //   base()
  //   static constructor_countdown(n)
  //   static constructor_countdown()
  //   static default_constructor_countdown(n)
  //   static default_constructor_countdown()
  //   static copy_constructor_countdown(n)
  //   static copy_constructor_countdown()
  //   static move_constructor_countdown(n)
  //   static move_constructor_countdown()
  //   static copy_assignment_countdown(n)
  //   static copy_assignment_countdown()
  //   static move_assignment_countdown(n)
  //   static move_assignment_countdown()
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
  //   special member functions: full // copy may throw
  //   static make(s...) // make T // may throw
  //   static make<U>(s...) // make U which is derived from T // may throw
  //   empty()
  //   clear()
  //   ->
  //   * // requires !empty()
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
  //   () // may throw (throw logic_error if empty())
  //   empty()
  //   clear()
  {
    ez_function<int ()> f = []() {return 1;};
    assert(f() == 1);
  }
}
void doc_ez_vector() {
  // ez_vector<T>
  //   typename value_type
  //   typename reference
  //   typename const_reference
  //   typename iterator
  //   typename const_iterator
  //   typename difference_type
  //   typename size_type
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_vector(initializer_list<T>) // may throw
  //
  //   begin()
  //   end()
  //   empty()
  //   size()
  //   max_size()
  //   capacity()
  //
  //   insert(i, x) // may throw
  //   erase(i1, i2 = i1 + 1)
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
  //   typename value_type
  //   typename reference
  //   typename const_reference
  //   typename iterator
  //   typename const_iterator
  //   typename difference_type
  //   typename size_type
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   before_begin()
  //   begin()
  //   end()
  //   empty()
  //   max_size()
  //
  //   new_node(x) // may throw
  //   delete_node(i)
  //   link_after(i, node)
  //   unlink_after(node)
  //   insert_after(i, x) // may throw
  //   erase_after(i1, i2 = next(i1))
}
void doc_ez_slist() {
  // ez_slist<T>
  //   typename value_type
  //   typename reference
  //   typename const_reference
  //   typename iterator
  //   typename const_iterator
  //   typename difference_type
  //   typename size_type
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   before_begin()
  //   begin()
  //   end()
  //   empty()
  //   size()
  //   max_size()
  //
  //   new_node(x) // may throw
  //   delete_node(i)
  //   link_after(i, node)
  //   unlink_after(node)
  //   insert_after(i, x) // may throw
  //   erase_after(i1, i2 = next(i1))
}
void doc_ez_bidirectional_list() {
  // ez_bidirectional_list<T>
  //   typename value_type
  //   typename reference
  //   typename const_reference
  //   typename iterator
  //   typename const_iterator
  //   typename difference_type
  //   typename typename size_type
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   begin()
  //   end()
  //   empty()
  //   max_size()
  //
  //   new_node(x) // may throw
  //   delete_node(i)
  //   link(i, node)
  //   unlink(node)
  //   insert(i, x) // may throw
  //   erase(i1, i2 = i1 + 1)
}
void doc_ez_list() {
  // ez_list<T>
  //   typename value_type
  //   typename reference
  //   typename const_reference
  //   typename iterator
  //   typename const_iterator
  //   typename difference_type
  //   typename typename size_type
  //
  //   special member functions: full // copy may throw
  //   == <=> // may throw
  //
  //   ez_list(initializer_list<T>) // may throw
  //
  //   begin()
  //   end()
  //   empty()
  //   size()
  //   max_size()
  //
  //   new_node(x) // may throw
  //   delete_node(i)
  //   link(i, node)
  //   unlink(node)
  //   insert(i, x) // may throw
  //   erase(i1, i2 = i1 + 1)
}
void doc_ez_map() {
  // ez_map<K, M>
  //   special member functions: full // copy may throw
  //   [](k) // may throw from <
  //   find(k) // may throw from <
  //   end()
  //   void erase(i)
  //   erase(k)->bool // may throw from <
  //   remove(k)->bool // may throw from <
  //   empty()
  //   size()
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
void doc_ez_mutex() {
  // ez_mutex
  //   special member functions: default-constructible
  //   lock()
  //   unlock()
}
void doc_test_allocator() {
  // test_allocator<T, MAP = ez_map>
  //   // print then terminate if any illegal operation take place
  //   special member functions: full
  //   ==
  //   allocate(n) // may throw
  //   deallocate(p, n)
  //   static empty()
  //   size()
  // stateful_test_allocator<T, MAP = ez_map,
  //                         COPY_PRPG = true, MOVE_PRPG = true,
  //                         SWAP_PRPG = true, ALWAYS_EQUAL = false>
  //   // print then terminate if any illegal operation take place
  //   special member functions: full
  //   ==
  //   allocate(n) // may throw
  //   deallocate(p, n)
  //   empty()
  //   size()
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
  //   // print then terminate if memory leak
  //   special member functions
  //     full // may throw (only swap is noexcept)
  //   explicit(...) test_object(x) // may throw
  //   =(x) // may throw
  //   ->
  //   *
  //   operator T_REF
  {
    test_object<int> a = 1, b = 2;
    assert(a != b);
  }
}
void doc_test_range() {
  // // below all: if any error is traced, print then terminate
  //
  // test_{i,f,b,r}itr(i, j, ritr_cr, eq = ...) // may throw
  // test_{i,f,b,r}itr(r, ritr_cr) // may throw
  // test_rng(r, rr, eq = ...) // may throw
  //
  // test_itr_minus(i, j, n = j - i) // may throw
  //   // requires: n >= 0 && n == j - i
  // test_itr_minus(r, n = size(r)) // may throw
  //   // requires: size(r) == n
}

// range
void doc_iterator_requirements() {
  // iterator exception requirements
  //   1) special member functions and swap can not throw
  //   2) confirm strong exception guarantee for every related function except
  //      for which takes at least one non-const input iterator argument

  // output_iterator_tag // from std <iterator>
  // input_iterator_tag // from std <iterator>
  // forward_iterator_tag // from std <iterator>
  // bidirectional_iterator_tag // from std <iterator>
  // random_access_iterator_tag // from std <iterator>
  // contiguous_iterator_tag // from std <iterator>

  // incrementable_traits<T> // from std <iterator>
  //   difference_type
  // iter_difference_t<T> // from std <iterator>
  {
    static_assert(same_as
                  <incrementable_traits<int *>::difference_type,
                   ptrdiff_t>);
    static_assert(same_as
                  <incrementable_traits<int *const>::difference_type,
                   ptrdiff_t>);
    static_assert(same_as<iter_difference_t<int *>, ptrdiff_t>);
  }

  // indirectly_readable_traits<T> // from std <iterator>
  //   value_type
  // iter_value_t<T> // from std <iterator>
  {
    static_assert(same_as
                  <indirectly_readable_traits<int *>::value_type,
                   int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int *>::value_type,
                   int>);
    static_assert(same_as<iter_value_t<int *>, int>);
  }

  // iter_reference_t<T> // from std <iterator>
  {
    static_assert(same_as<iter_reference_t<int *>, int &>);
    static_assert(same_as<iter_reference_t<const int *>, const int &>);
  }

  // iterator_traits<T> // from std <iterator>
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

  // ranges::iter_move(i) // from std <iterator>
  // ranges::iter_swap(i, ii) // from std <iterator>
  {
    owner_ptr<int> p(1), pp(2);
    auto i = &p, ii = &pp;
    ranges::iter_swap(i, ii);
    assert(p != nullptr && *p == 2
           && pp != nullptr && *pp == 1);
    *i = ranges::iter_move(ii);
    assert(*p == 1 && pp == nullptr);
  }

  // indirectly_readable<I> // from std <iterator>
  {
    static_assert(indirectly_readable<int *>);
  }
  // iter_common_reference_t<I> // from std <iterator>
  {
    static_assert(same_as<iter_common_reference_t<int *>, int &>);
  }
  // indirectly_writable<I, T> // from std <iterator>
  {
    static_assert(indirectly_writable<int *, int>);
    static_assert(indirectly_writable<int *, int &>);
    static_assert(indirectly_writable<int *, const int &>);
    static_assert(indirectly_writable<int *, int &&>);
  }
  // indirectly_movable<IN, OUT> // from std <iterator>
  // indirectly_movable_storable<IN, OUT> // from std <iterator>
  // indirectly_copyable<IN, OUT> // from std <iterator>
  // indirectly_copyable_storable<IN, OUT> // from std <iterator>
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
  // indirectly_swappable<I, II = I> // from std <iterator>
  // indirectly_comparable<I, II, F, P = identity, P2 = identity>
  //   // from std <iterator>
  {
    static_assert(indirectly_swappable<int *, int *>);
    static_assert(indirectly_comparable<int *, int *, equal_to<>>);
  }
  // weakly_incrementable<I> // from std <iterator>
  // incrementable<I> // from std <iterator>
  {
    static_assert(weakly_incrementable<int *>);
    static_assert(incrementable<int *>);
  }
  // input_or_output_iterator<I> // from std <iterator>
  // sentinel_for<S, I> // from std <iterator>
  // disable_sized_sentinel_for<S, I> // from std <iterator>
  // sized_sentinel_for<S, I> // from std <iterator>
  {
    static_assert(input_or_output_iterator<int *>);
    static_assert(sentinel_for<const int *, int *>);
    static_assert(sentinel_for<int *, const int *>);
    static_assert(!disable_sized_sentinel_for<int *, int *>);
    static_assert(sized_sentinel_for<int *, int *>);
  }
  // input_iterator<I> // from std <iterator>
  // output_iterator<I, T> // from std <iterator>
  // forward_iterator<I> // from std <iterator>
  // bidirectional_iterator<I> // from std <iterator>
  // random_access_iterator<I> // from std <iterator>
  // contiguous_iterator<I> // from std <iterator>
  {
    static_assert(input_iterator<int *>);
    static_assert(forward_iterator<int *>);
    static_assert(bidirectional_iterator<int *>);
    static_assert(random_access_iterator<int *>);
    static_assert(contiguous_iterator<int *>);
  }
  // cpp17_iterator<I>
  // cpp17_input_iterator<I>
  // cpp17_forward_iterator<I>
  // cpp17_bidirectional_iterator<I>
  // cpp17_random_access_iterator<I>
  {
    static_assert(cpp17_iterator<int *>);
    static_assert(cpp17_input_iterator<int *>);
    static_assert(cpp17_forward_iterator<int *>);
    static_assert(cpp17_bidirectional_iterator<int *>);
    static_assert(cpp17_random_access_iterator<int *>);
  }
  // indirectly_unary_invocable<F, I> // from std <iterator>
  // indirectly_regular_unary_invocable<F, I> // from std <iterator>
  {
    const auto f = [](const int &) {};
    static_assert(indirectly_unary_invocable<decltype(f), int *>);
    static_assert(indirectly_regular_unary_invocable<decltype(f), int *>);
  }
  // indirect_unary_predicate<F, I> // from std <iterator>
  // indirect_binary_predicate<F, I, II> // from std <iterator>
  {
    const auto f = [](const int &) {return true;};
    static_assert(indirect_unary_predicate<decltype(f), int *>);
    const auto ff = [](const int &, void *&) {return true;};
    static_assert(indirect_binary_predicate<decltype(ff), int *, void **>);
  }
  // indirect_equivalence_relation<F, I, II = I> // from std <iterator>
  // indirect_strict_weak_order<F, I, II = I> // from std <iterator>
  {
    static_assert(indirect_equivalence_relation<equal_to<int>, int *, int *>);
    static_assert(indirect_strict_weak_order<less<int>, int *, int *>);
  }
  // indirect_result_t<F, ITERS...> // from std <iterator>
  {
    const auto f = []() {return true;};
    const auto ff = [](int, int, int, int) {return true;};
    static_assert(same_as<indirect_result_t<decltype(f)>, bool>);
    static_assert(same_as<indirect_result_t
                          <decltype(ff), int *, int *, int *, int *>, bool>);
  }
  // projected<I, F> // from std <iterator>
  {
    const auto f = [](const int &)->void * {return nullptr;};
    using pseudo_iter = projected<int *, decltype(f)>;
    static_assert(same_as<iter_value_t<pseudo_iter>, void *>);
    static_assert(same_as<iter_difference_t<pseudo_iter>, ptrdiff_t>);
    static_assert(same_as<iter_reference_t<pseudo_iter>, void *>);
  }
  // permutable<I> // from std <iterator>
  // mergeable<I, II, OI, F = ranges::less, P = identity, PP = identity>
  //   // from std <iterator>
  // sortable<I, F = ranges::less, P = identity> // from std <iterator>
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
  // itr_ptr<I>
  // itr_ptr_from_ref<I>
  // itr_ref<I>
  // itr_cref<I>
  // itr_rref<I>
  // itr_common_ref<I>
  // itr_dft<I>
  // itr_ctg<I>
  // is_itr<I>
  // is_oitr<I>
  // is_iitr<I>
  // is_fitr<I>
  // is_bitr<I>
  // is_ritr<I>
  // is_citr<I>
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

  // iterator_is_counted<I>
  //   value
  // itr_is_counted<I>

  // advance(i, n) // from std <iterator>
  // advance(i, j)
  // advance(i, n, j) // return n - distance(i, j)
  // distance(i, ii) // from std <iterator>, with difference
  // next(i, n = 1) // from std <iterator>
  // next(i, j)
  // next(i, n, j)
  // prev(i, n = 1) // from std <iterator>
  // prev(i, n, j)

  // common_iter_t<IT1, IT2>

  // iter_post_increment_return_type<vt>
  // iter_post_increment(i)
  // iter_post_decrement(i)
}
void doc_range_main_components() {
  // begin(r) // from std <iterator>
  // end(r) // from std <iterator>
  // size(r) // from std <iterator>, with difference
  // ssize(r) // similar to std::ranges::size
  // empty(r) // from std <iterator>, with difference
  // is_rng<R>

  // n_value_iterator<T>
  //   iterator category: ritr
  //   n_value_iterator(T &, ptrdiff_t)
  //   base()->T *
  // n_value_itr(x, n)
  // n_value<T>
  //   n_value(n)
  //   n_value(n, s...)
  // range_is_n_value<R>
  //   value
  {
    int x = 1;
    assert(n_value_itr(x, 0).base() == addressof(x));
    assert(equal(rng(n_value_itr(x, 0), n_value_itr(x, 3)), seq(1, 1, 1)));
    x = 2;
    assert(equal(rng(n_value_itr(x, 0), n_value_itr(x, 3)), seq(2, 2, 2)));
  }

  // // rng series:
  // //   get void if failed which means well-formed forever
  // //   is itr_xxx<rng_itr<R>> if the name is shared by itr series
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
  // rng_is_sized<R>
  // rng_is_counted<R> // itr_is_counted<rng_itr<R>>
  // rng_is_n_value<R>

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

  // is_orng<R>
  // is_irng<R>
  // is_frng<R>
  // is_brng<R>
  // is_rrng<R>
  // is_crng<R>
  // is_just_irng<R>
  // is_just_frng<R>
  // is_just_brng<R>
  // is_just_rrng<R>

  // is_iter_pair<R> // R can be reference type

  // is_range_reference<R> // R can be reference type
  //   static value
  // is_rng_ref<R> // = is_range_reference<R>::value
  //   // range reference means:
  //   // (1) its iterator does not depend on it
  //   // (2) const R and R share the same iterator type
  {
    static_assert(is_rng_ref<iter_pair<int *>>);
    static_assert(is_rng_ref<ez_vector<int> &>);
    static_assert(!is_rng_ref<ez_vector<int>>);
  }

  // rng_forward_t<R>
  {
    static_assert(same_as<rng_forward_t<iter_pair<int *> &>, iter_pair<int *>>);
    static_assert(same_as<rng_forward_t<const ez_vector<int> &>,
                          const ez_vector<int> &>);
    static_assert(same_as<rng_forward_t<ez_vector<int> &&>,
                          ez_vector<int>>);
  }

  // before_begin(r) // requires: r.before_begin() is well-foremd
  // before_end(r) // requires: !empty(r)
  // cbegin(r) // from std <iterator>
  // cend(r) // from std <iterator>
  // rbegin(r) // from std <iterator>
  // rend(r) // from std <iterator>
  // crbegin(r) // from std <iterator>
  // crend(r) // from std <iterator>
  // data(r) // from std <iterator>, with difference
  // ref(r, n) // requires valid arguments
  // cref(r, n) // requires valid arguments
  // nth(r, n) // requires valid arguments
  // cnth(r, n) // requires valid arguments  
  // at_most_nth(r, n)
  // front(r) // requires: !empty(r) and nth(r, 0) is possible to get
  // back(r) // requires: !empty(r) and nth(r, size(r) - 1) is possible to get
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
  // degraded_iterator<I, CTG> // requires: is_base_of_v<CTG, itr_ctg<I>>
  //   iterator_type = I
  //   iterator category: iitr - citr
  //   degraded_iterator(const degraded_iterator<I2> &)
  //   explicit degraded_iterator(I)
  //   base()->I
  // degraded_input_iterator = degraded_iterator<...>
  // degraded_forward_iterator = degraded_iterator<...>
  // degraded_bidirectional_iterator = degraded_iterator<...>
  // degraded_random_access_iterator = degraded_iterator<...>
  // degraded_itr<TAG>(i) // mandates: tag is no more than tag of i
  // degraded_iitr(i) // mandates: is_iitr<I>
  // degraded_fitr(i) // mandates: is_fitr<I>
  // degraded_bitr(i) // mandates: is_bitr<I>
  // degraded_ritr(i) // mandates: is_ritr<I>
  {
    int *const p{};
    static_assert(is_citr<decltype(p)>);
    auto i = degraded_iitr(p);
    static_assert(is_just_iitr<decltype(i)>);
    assert(i.base() == p);
  }
}
void doc_reverse_iterator() {
  // reverse_iterator<BI> // requires is_bitr<BI>
  //   iterator_type = BI
  //   iterator category: bitr - citr
  //   reverse_iterator(const reverse_iterator<BI2> &)
  //   =(const reverse_iterator<BI2> &)
  //   explicit reverse_iterator(BI)
  //   base()->BI
  // make_reverse_iterator(bi) // requires: is_bitr<BI>
  // ritr(bi) // return make_reverse_iterator(bi) or bi.base()
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
  //   special member functions: no default constructor
  //   back_insert_iterator(C &)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // back_inserter(c)
  //
  // front_insert_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   special member functions: no default constructor
  //   front_insert_iterator(C &)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // front_insert_inserter(c)
  //
  // insert_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   special member functions: no default constructor
  //   insert_iterator(C &, C::iterator)
  //   =(const C::value_type &)
  //   =(C::value_type &&)
  // inserter(c, i)
  //
  // to_back_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   special member functions: no default constructor
  //   to_back_iterator(C &)
  //   to_back_iterator(C &&)
  //   base() &&->C
  // to_back(c)
  //
  // to_front_iterator<C>
  //   container_type = C
  //   iterator category: oitr
  //   special member functions: no default constructor
  //   to_front_iterator(C &)
  //   to_front_iterator(C &&)
  //   base() &&->C
  // to_front(c)
  //
  // any_output_iterator<UF>
  //   iterator category: oitr
  //   special member functions: depend on UF
  //   explicit any_output_iterator(UF)
  //   =(x)
  // output_itr(f)
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
  }
}
void doc_move_iterator() {
  // move_iterator<II>
  //   iterator_type = II
  //   iterator category: iitr - citr
  //   move_iterator(const move_iterator<II2> &)
  //   =(const move_iterator<II2> &)
  //   explicit move_iterator(II)
  //   base()
  // make_move_iterator(i)
  // move_itr(i) // = make_move_iterator(i)
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
  //   iterator_type = I
  //   iterator category: iitr - citr, or oitr
  //   special member functions: depend on I
  //   counted_iterator(const counted_iterator<I2> &)
  //   =(const counted_iterator<I2> &)
  //   explicit counted_iterator(i, n) // requires: n >= 0
  //   base()
  // make_counted_iterator(i, n) // requires: n >= 0
  // counted_itr(i, n) // = make_counted_iterator(i, n)
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
  //   iterator category: iitr - citr, or oitr
  //   special member functions: depend on I
  //   iterator_wrapper(const iterator_wrapper<I> &)
  //   =(const iterator_wrapper<I> &)
  //   explicit iterator_wrapper(I)
  //   base()
  // wrap_itr(i)
  {
    int *const p{};
    assert(wrap_itr(p).base() == p);
  }
}
void doc_array() {
  // array<T, N>
  //   pointer
  //   const_pointer
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
  //   <=>
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //
  //   empty()
  //   size()
  //   max_size()
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   front()
  //   back()
  //   operator [](i)
  //   at(i)
  //
  //   data()
  //   fill(x)
  //
  // to_array(a)
  // seq(...)
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
  //   special member functions: depend on I
  //   == with iter_pair<I2>
  //   explicit(...) iter_pair(const iter_pair<X> &)
  //   operator =(const iter_pair<X> &)
  //   iter_pair(i, ii)
  // iter_pair()->iter_pair<nullptr_t>
  // iter_pair(I, I)->iter_pair<I>
  // rng(i, i2)->iter_pair
  // rng(r)->iter_pair
  {
    const int a[] = {1, 2, 3};
    iter_pair<const int *> v1 = rng(begin(a), end(a));
    assert(equal(v1, seq(1, 2, 3)));
  }
}
void doc_composite_range() {
  // composite_input_iterator<T, DEREF, PP, EQ>
  //   iterator category: iitr
  //   composite_input_iterator(x, deref_f, pp_f, eq_f)
  //   base()
  // composite_forward_iterator<T, DEREF, PP, EQ>
  //   iterator category: fitr
  //   composite_forward_iterator(x, deref_f, pp_f, eq_f)
  //   base()
  // composite_bidirectional_iterator<T, DEREF, PP, EQ, MM>
  //   iterator category: bitr
  //   composite_forward_iterator(x, deref_f, pp_f, eq_f, mm_f)
  //   base()
  //
  // composite_irng(x, y, df, pp, eq)
  // composite_frng(x, y, df, pp, eq)
  // composite_brng(x, y, df, pp, eq, mm)
}
void doc_range_wrapper() {
  // range_wrapper<R>
  //   special member functions: depend on R
  //   explicit range_wrapper(r)
  // wrap_rng(r)
  // wrap_rng(x, y) // = wrap_rng(rng(x, y))
}
void doc_base_range() {
  // base_range<R>
  //   special member functions: depend on R
  //   explicit base_range(r)
  // base_rng(r)
  // base_rng(i, ii)
  // base_rng(i, n)
  //
  // inplace_base_range<R>
  //   // inplace means: distance(begin(r).base(), end(r).base()) == size(r)
  //   special member functions: depend on R
  //   explicit inplace_base_range(r)
  // inplace_base_rng(r)
  // inplace_base_rng(i, ii);
  // inplace_base_rng(i, n);
}
void doc_empty_range() {
  // empty_range<T>
  // empty_rng<T>()
  {
    auto r = empty_rng<int>();
    assert(empty(r));
  }
}
void doc_single_range() {
  // single_range<T>
  //   special member functions: depend on T
  //   explicit single_range(x)
  //   single_range(in_place, ...)
  // single_rng(x) // take value
  // single_rng(ref(x)) // take reference
}
void doc_counted_range() {
  // counted_range<R>
  //   // requires: copy/move constructed new R has unchanged size than source
  //   special member functions: depend on R
  //   explicit counted_range(r)
  //   counted_range(r, n) // requires n == size(r)
  // counted_rng(fr)
  // counted_rng(r, n) // requires n == size(r)
  // sized_rng(fr)
  // sized_rng(r, n) // requires n == size(r)
}
void doc_degraded_range() {
  // degraded_range<R, CTG> // requires is_base_of_v<CTG, rng_ctg<R>>
  //   special member functions: depend on R
  //   explicit degraded_range(r)
  // degraded_input_range = degraded_range<...>
  // degraded_forward_range = degraded_range<...>
  // degraded_bidirectional_range = degraded_range<...>
  // degraded_random_access_range = degraded_range<...>
  // degrade_rng<TAG>(r)
  // degrade_rng<TAG>(i, ii)
  // degrade_rng<TAG>(i, n)
  // degraded_irng(r)
  // degraded_irng(i, ii)
  // degraded_irng(i, n)
  // degraded_frng(r)
  // degraded_frng(i, ii)
  // degraded_frng(i, n)
  // degraded_brng(r)
  // degraded_brng(i, ii)
  // degraded_brng(i, n)
  // degraded_rrng(r)
  // degraded_rrng(i, ii)
  // degraded_rrng(i, n)
}
void doc_move_range() {
  // move_range<R>
  //   special member functions: depend on R
  //   explicit move_range(r)
  // move_rng(r)
  // move_rng(i1, i2)
  // move_rng(i1, n)
}
void doc_reverse_range() {
  // reverse_range<R>
  //   special member functions: depend on R
  //   explicit move_range(r)
  // rrng(br)
  // rrng(i1, i2)
  // rrng(i1, n)
}
void doc_rng_for_iterator_n() {
  // iter_n_iterator<I>
  //   iterator_type = I
  //   iterator category: iitr - fitr
  //   iter_n_iterator(const iter_n_iterator<I2> &)
  //   iter_n_iterator(i, n) // requires: n > 0
  //   base()->I
  //   nth()
  // iter_n_itr(i, n) // requires: n > 0
  // rng(i, n) // requires: n > 0
  {
    const ez_list<int> l = {1, 2, 3};
    const auto r = rng(l.begin(), 3);
    assert(is_just_frng<decltype(r)>);
    const auto r2 = rng(l.begin(), nth(l, 3));
    assert(is_just_brng<decltype(r2)>);
  }
}
void doc_rng_for_n_value() {
  // rng(n, x) // take value // requires n > 0
  // rng(n, ref(x)) // take reference // requires n > 0
}
void doc_iterator_range() {
  // iterator_iterator<I>
  //   iterator category: iitr - ritr, or oitr
  //   explicit iterator_iterator(i)
  //   base()
  // iitr(i)
  //
  // iterator_range<R>
  // irng(r)
  // irng(n1, n2)
  // irng(i1, i2)
  // irng(i1, n)
  {
    assert(equal(irng(0, 3), seq(0, 1, 2)));
  }
}
void doc_bind_iterator() {
  // bind_iterator<II, UF>
  //   // requires: UF has no cvref, f(*i) is well formed
  //   iterator category: iitr - ritr
  //   special member functions: full
  //   bind_iterator(const bind_iterator<II2, UF2> &) // enable if convertible
  //   bind_iterator(i, f)
  //   base()
  //   fn() // requires: the function is stored
  // bind_itr(i, f)
  //
  // bind_range<R, UF>
  //   special member functions: depend on R and UF
  //   bind_range(r, f)
  //   begin()
  //   end()
  //   empty()
  //   size()
  // bind_rng(r, f)
  //
  // deref_rng(r)
  // deref_rng(i1, i2)
  // deref_rng(i1, n)
  // deref_rng(n, x)
  // deref_itr(i)
  //
  // const_rng(r)
  // const_rng(i1, i2)
  // const_rng(i1, n)
  // const_rng(n, x)
  // const_itr(i)
  //   // requires: the corresponding itr_ref<...> is reference
  {
    assert(equal(bind_rng(seq(1, 2, 3), [](int x) {return x + 1;}),
                 seq(2, 3, 4)));
  }
}
void doc_iters() {
  // for_iter_iterator<IT> // should only be used for iters
  // for_iter_range<R> // should only be used for iters
  // iters(i1, i2)
  // iters(r)
  //
  // r_for_iter_iterator<IT> // should only be used for r_iters
  // r_for_iter_range<R> // should only be used for r_iters
  // r_iters(i1, i2)
  // r_iters(r)
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
  //   propagate_on_container_move_assignment
  //   propagate_on_container_swap
  //   is_always_equal
  //   rebind_alloc<T>
  //   rebind_traits<T>
  //   static allocate(al) // may throw
  //   static allocate(al, n) // may throw
  //   static allocate(al, n, const_void_pointer hint) // may throw
  //   static construct(al, p, s...)
  //   static destroy(al, p)
  //   static deallocate(al, p)
  //   static deallocate(al, p, n)
  //   static max_size(al)
  //   static select_on_container_copy_construction(al)
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
  // allocator_with_primary_traits<AL>
  // allocator_provides_construct_function<AL, T, S...>
  // allocator_provides_destroy_function<AL, T>
  // nothrow_constructible_by_allocator<AL, T, S...>
  // nothrow_move_constructible_by_allocator<AL, T>
}
void doc_uninitialized_argo() {
  // default_construct_non_array_at(p) // may throw
  // construct_non_array_at(p, s...) // may throw
  // construct_at(p, s...) // same as construct_non_array_at
  // destroy_non_array_at(p)
  //
  // destroy_at(p)
  // destroy_at(p, d)
  // destroy(r)
  // destroy(r, d)
  //
  // default_construct_array(p) // p is pointer to bounded array
  // default_construct_array(p, f, d)
  // construct_array(p)
  // construct_array(p, f, d)
  // construct_array(p, fll)
  // construct_array(p, fll, f, d)
  // default_construct_maybe_array(p)
  // default_construct_maybe_array(p, f, d)
  // construct_maybe_array(p)
  // construct_maybe_array(p, f, d)
  // construct_maybe_array(p, u)
  // construct_maybe_array(p, u, f, d)
  //
  // initialize(_plus)(fr, fn) // may throw
  // initialize(_plus)(ir, fi, fn) // may throw
  // securely_initialize(_plus)(fr, fn, destroy_fn) // may throw
  //   // requires: iterator-range operations of fr can not throw
  // securely_initialize(_plus)(fr, fi, fn, destroy_fn) // may throw
  //   // requires: iterator-range operations of fi can not throw
}
void doc_default_allocator() {
  // allocator<T>
  //   value_type = T
  //   size_type = size_t
  //   difference_type = ptrdiff_t
  //   propagate_on_container_move_assignment = true_type
  //
  //   special member functions: full
  //   ==
  //   allocator(const allocator<U> &)
  //   allocate(n)
  //   deallocate(p, n)
  //   max_size()
  //   min_alignment() // return __STDCPP_DEFAULT_NEW_ALIGNMENT__
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
  //   get()->al_ref
  //   rebind<T>()->allocator_wrapper<alloc_rebind<AL, T>>
  //   max_size()
  //   min_alignment()
  //
  //   allocate_alignas(algn, n = 1)->pair<offset_ptr, origin_ptr> // may throw
  //     // requires: same_as<alloc_vt<AL>, byte>
  //   deallocate_alignas(algn, pair, n = 1)
  //     // requires: same_as<alloc_vt<AL>, byte>
  //
  //   allocate(n = 1) // may throw
  //   deallocate(p, n = 1)
  //   construct(p, s...) // may throw
  //   destroy(p)
  //   destroy(fr) // range operations can not throw
  //
  //   destroy_function
  //     ()(p)
  //   destroy_fn()->destroy_function // returned value need *this to be alive
  //
  //   default_construct_non_array_fn() // alloc_vt<AL> is byte
  //   construct_non_array_fn() // alloc_vt<AL> is byte
  //   destroy_non_array_fn() // alloc_vt<AL> is byte
  //
  //   // uninitialized_...(): requires: no overlap
  //   uninitialized_fill(_plus)(fr) // may throw
  //   uninitialized_fill(_plus)(fr, x) // may throw
  //   uninitialized_copy(_plus)(ir, fi) // may throw
  //   uninitialized_move(_plus)(ir, fi) // may throw
  //   uninitialized_fully_move(_plus)(fr, fi, destroy_f = destroy_fn())
  //     // may throw
  //     // destroy_f is for fr
  //     // range operations can not throw
  //   fully_move(_plus)(fr, fi, destroy_f = destroy_fn())
  //     // may throw
  //     // destroy_f is for fr
  //     // range operations can not throw
  //
  //   new_1(s...) // may throw
  //   new_1_with_placement_new(s...) // may throw
  //     // note: the result can not pass to delete_1
  //   new_n(n) // may throw
  //   new_n(n, x) // may throw
  //   delete_1(p)
  //   delete_n(p, n)
  //
  //   new_node<NODE_T>(s...) // may throw
  //   delete_node(node_p)
  //
  //   temporary
  //     special member functions: only move constructible
  //       // may throw
  //     explicit(sizeof...(s) == 0) temporary(p, s...)
  //     ->
  //     *
  //   make_temporary(s...) // may throw
  //
  //   uninitialized
  //     special member functions: only movable
  //     begin()
  //     end()
  //     empty()
  //     size()
  //     release()->ptr
  //   make_uninitialized(n) // may throw
  //
  //   headed_buffer_ptr<H, U>
  //     head_type = H
  //     value_type = U
  //     smf: full
  //     headed_buffer_ptr(nullptr)
  //     =(nullptr)
  //     ==(nullptr)
  //     head()->ref
  //     data()->ptr
  //     size()->alloc_szt<AL>
  //     refer_to_only_head(H &)
  //   allocate_headed_buffer<H, U>(n, s...)->headed_buffer_ptr<H, U>
  //     // alloc_vt<AL> is byte
  //     // construct head by s...
  //     // requires: n is valid
  //   deallocate_headed_buffer(headed_buffer_ptr<H, U>)
  //     // alloc_vt<AL> is byte
  //     // auto destroy head type
  //
  //   headed_bytebuf_ptr<H>
  //     head_type = H
  //     value_type = U
  //     smf: full
  //     headed_bytebuf_ptr(nullptr)
  //     =(nullptr)
  //     ==(nullptr)
  //     head()->ref
  //     data()->ptr
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
  //   new_array(n) // may throw
  //   new_array(n, x) // may throw
  //   new_array(fr) // may throw
  //   new_array_move_individually(x, x_al = get()) // may throw
  //   delete_array(x)
  //
  //   unique_ptr = re::unique_ptr<...>
  //   unique_array = re::unique_array<...>
  //   make_unique(s...) // may throw
  //   make_array(n) // may throw
  //   make_array(n, x) // may throw
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
  //   ()(U *) // T[]
  //   ()(T *) // T[N]
  //
  // unique_ptr<T, D = default_delete<T>>
  //   pointer = D::pointer if D::pointer exists, otherwise T *
  //   element_type = T;
  //   deleter_type = D;
  //
  //   special member functions: default constructible, movable
  //   <=>
  //     // between two unique_ptr objects
  //     // between unique_ptr and nullptr
  //
  //   unique_ptr(nullptr)
  //   operator =(nullptr)
  //   unique_ptr(unique_ptr<U, E> &&u)
  //   operator =(unique_ptr<U, E> &&u)
  //   explicit unique_ptr(p)
  //   unique_ptr(p, d)
  //
  //   ->
  //   * // requires !empty()
  //   pointer get()
  //
  //   empty()
  //   clear()
  //   explicit operator bool()
  //
  //   get_deleter()->ref
  //
  //   pointer release()
  //   reset(p = pointer()) // requires: p != get()
  //
  // unique_ptr<T [], D>
  //   pointer = D::pointer if D::pointer exists, otherwise T *
  //   element_type = T;
  //   deleter_type = D;
  //   []
  //   pointer get()
  //   ... // same as unique_ptr<T, D>
  //
  // hash<unique_ptr<T, D>>
  //
  // make_unique<T>(s...) // value initialize // may throw
  // make_unique<T []>(n) // value initialize // may throw
  // make_unique_for_overwrite<T>() // default initialize // may throw
  // make_unique_for_overwrite<T []>(n) // default initialize // may throw
}
void doc_unique_array() {
  // unique_array<T, D = default_delete<T>>
  //   pointer
  //   const_pointer
  //   element_type
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //   deleter_type
  //   pointer_pair
  //
  //   special member functions
  //     default constructible
  //     movable
  //   <=> // may throw
  //     // between unique_array objects with the same pointer element type
  //
  //   unique_array(unique_array<U, E> &&u)
  //   operator =(unique_array<U, E> &&u)
  //   explicit unique_array(pointer_pair)
  //   unique_array(pointer_pair, d)
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   empty()
  //   size()
  //
  //   front()
  //   back()
  //   [](n)
  //
  //   get_deleter()->ref
  //
  //   pointer_pair release()
  //   reset(pointer_pair = {}) // requires: no self-reset
  //   clear()
  //
  // make_array<T>(n) // may throw
  // make_array<T>(n, x) // may throw
  // make_array<T>(r) // may throw
}
void doc_copyable_ptr() {
  // copyable_ptr<T> // requires: T is copy constructible
  //   element_type = T;
  //   value_type
  //
  //   special member functions: full // copy may throw
  //
  //   copyable_ptr(nullptr)
  //   =(nullptr)
  //   ==(nullptr)
  //   explicit operator bool()
  //   empty()
  //
  //   explicit copyable_ptr(T *)
  //   release()->T *
  //   reset(T * = nullptr) // requires: no self-reset
  //   clear()
  //
  //   copyable_ptr(in_place, s...) // may throw
  //   emplace(s...) // may throw
  //
  //   ->
  //   *  // requires: !empty()
  //   get()->T *
}
void doc_copyable_array() {
  // copyable_array<T>
  //   pointer
  //   element_type = T;
  //   value_type
  //   reference
  //   const_reference
  //   difference_type
  //   size_type
  //   pointer_pair
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
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   empty()
  //   size()
  //
  //   front()
  //   back()
  //   [](n)
}
void doc_maybe_owner_ptr() {
  // maybe_owner_ptr<T>
  //   pointer = T *
  //   element_type = T
  //
  //   smf: full
  //
  //   maybe_owner_ptr(nullptr)
  //   =(nullptr)
  //   ==(nullptr)
  //
  //   explicit(sizeof...(s) == 0) maybe_owner_ptr(in_place, s...)
  //
  //   ->
  //   *
  //   get()->T *
  //
  //   empty()->bool
  //   owns()->bool
  //   clear()
  //   reset()
}
void doc_buffer() {
  // buffer<T, AL = default_allocator<T>>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   special member functions: only move constructible
  //
  //   explicit buffer(al)
  //   explicit buffer(n) // may throw
  //
  //   allocator_type = AL
  //   get_allocator()
  //
  //   begin()
  //   end()
  //
  //   max_size()
  //   size()
  //   empty()
  //   capacity()
  //
  //   front()
  //   back()
  //   [](n)
  //   emplace_front(s...) // may throw
  //     // requires: front_raw_space() != 0
  //   push_front(x) // may throw
  //     // requires: front_raw_space() != 0
  //   emplace_back(s...) // may throw
  //     // requires: back_raw_space() != 0
  //   push_back(x) // may throw
  //     // requires: back_raw_space() != 0
  //   pop_front() // requires: !empty()
  //   pop_back() // requires: !empty()
  //   front_raw_space()
  //   back_raw_space()
  //
  //   reallocate(n) // may throw
  //   increase_capacity(n) // may throw
  //
  //   reset(n) // may throw
  //   reserve(n) // may throw
  //   reserve_more(n) // may throw
  //   clear()
  //
  //   release()->void
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
  //   propagate_on_container_move_assignment
  //   propagate_on_container_swap
  //   is_always_equal
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
  // class allocator_aware_container_ownership
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
  //   smf: movable
  //   empty()->bool
  //   value()->T_ref // throw logic_error if empty
  //   operator *()->T_ref
  //   operator ->()->T_ptr
  //
  // object_pool<T, AL = default_allocator<byte>>
  // sync_object_pool<T, AL = default_allocator<byte>> // in concurrency.h
  //   size_type
  //   smf: default-constructible
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
  //   smf: destructible
  //
  //   raw_object_pool(sz, algn, a = AL{})
  //
  //   object_size()->size_type
  //   object_align()->size_type
  //   capacity()->size_type
  //   used_size()->size_type
  //   idle_size()->size_type
  //   reserve_more(n)
  //   allocate()->void *
  //   deallocate(void *p)
}
void doc_memory_pool() {
  // memory_pool<AL = default_allocator<byte>>
  //   size_type
  //   difference_type
  //   smf: default-constructible
  //   explicit memory_pool(algn, a = AL{})
  //   memory_pool(fr, algn, a = AL{})
  //   allocate<T>(size_type)->T *
  //   deallocate(T *, size_type)
  //   min_alignment()->size_type
  //
  // pool_allocator<T, POOL = memory_pool<>>
  //   value_type
  //   size_type
  //   difference_type
  //
  //   smf: full
  //   explicit pool_allocator(POOL &)
  //   operator pool_allocator<U, POOL>()
  //   ==
  //
  //   allocate(n)->T *
  //   deallocate(T *, n)
}

// dynamic
void doc_dynamic_base() {
  // is<T>(x)
  //
  // in_place_index_t<I>
  // in_place_index<I>
  // is_in_place_index(_v)<T>
}
void doc_dynamic() {
  // as_dynamic
  //
  // dynamic_default_buffer_size<BASE_TYPE>
  //   value
  // dynamic_default_buffer_alignment<BASE_TYPE>
  //   value
  //
  // bad_dynamic_access : exception
  //   what()
  //
  // dynamic<T = void,
  //         BUFSZ = dynamic_default_buffer_size<T>::value,
  //         ALIGN = dynamic_default_buffer_alignment<T>::value>
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
  //   dynamic(const dynamic<T, SZ, ALGN> &) // may throw
  //   operator =(const dynamic<T, SZ, ALGN> &) // may throw
  //   dynamic(dynamic<T, SZ, ALGN> &&) // may throw
  //   operator =(dynamic<T, SZ, ALGN> &&) // may throw
  //
  //   *
  //   ->
  //   value() // may throw
  //
  //   type()
  //   is<X>()
  //   as<X>()
  //
  //   empty()
  //   clear()
  //   has_value()
  //   reset()
  //
  //   local()
  //   size()
  //   align()
  //   wrapper_size()
  //   wrapper_align()
  {
    struct A : as_dynamic {virtual int f() {return 0;}};
    struct B : A {virtual int f() override {return 1;}};
    struct C : A {virtual int f() override {return 2;}};
    dynamic<A> a = A(), b = B(), c = C();
    assert(a->f() == 0 && b->f() == 1 && c->f() == 2);
  }
}
void doc_dynamic_void() {
  // dynamic_default_buffer_size<void>
  //   // equal to dynamic_default_buffer_size<void *>
  // dynamic_default_buffer_alignment<void>
  //   // equal to dynamic_default_buffer_alignment<void *>
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
  //   dynamic(const dynamic<void, SZ, ALGN> &) // may throw
  //   operator =(const dynamic<void, SZ, ALGN> &) // may throw
  //   dynamic(dynamic<voidT, SZ, ALGN> &&) // may throw
  //   operator =(dynamic<void, SZ, ALGN> &&) // may throw
  //
  //   type()
  //   is<X>()
  //   as<X>()
  //
  //   empty()
  //   clear()
  //   reset()
  //
  //   local()
  //   size()
  //   align()
  //   wrapper_size()
  //   wrapper_align()
  //
  // any = dynamic<>;
  // make_any<T>(s...) // may throw
  // make_any<T>({...}, s...) // may throw
  //
  // bad_any_cast : bad_cast
  //   what()
  // any_cast<T>(any REF) // may throw bad_any_cast
  // any_cast<T>(any PTR) // may throw bad_any_cast
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
  // bad_function_call : exception
  //   what()
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
  //   function(x) // may throw // decay_t<x> is copy constructible
  //   =(x) // may throw // decay_t<x> is copy constructible
  //   =(reference_wrapper<T>) // may throw
  //
  //   function(const function<R (S...), SZ, ALGN> &) // may throw
  //   =(const function<R (S...), SZ, ALGN> &) // may throw
  //   function(function<R (S...), SZ, ALGN> &&) // may throw
  //   =(function<R (S...), SZ, ALGN> &&) // may throw
  //
  //   ()(s...) // may throw
  //
  //   type()
  //   is<X>()
  //   as<X>()
  //
  //   local()
  //   static constexpr bool local<X>()
  //
  //   size()
  //   align()
  //   wrapper_size()
  //   wrapper_align()
  //
  //   empty()
  //   clear()
  //   reset()
  //   explicit operator bool()
  //
  //   target_type()
  //   target<X>()->pointer
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
  //   move_only_function(x) // may throw
  //   =(x) // may throw
  //
  //   ()(...) // may throw // requires operator bool()
  //   explicit operator bool()
}
void doc_unique_function() {
  // unique_function<F>
  //   return_type
  //
  //   special member functions
  //     move only
  //
  //   unique_function(nullptr_t)
  //   =(nullptr_t)
  //   == with nullptr
  //   reset()
  //   clear()
  //   empty()
  //   explicit operator bool
  //
  //   unique_function(x) // no guard to nullptr
  //   =(x) // may throw
  //
  //   ()(...) // may throw // !empty()
  //
  //   local()
  //   static local<T>()
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

  // is_invocable_for_all_combinations(_v)<F, S...>
  {
    using f = void (*)(int);
    static_assert(is_invocable_for_all_combinations_v
                  <f, type_pack<int>>);
    static_assert(is_invocable_for_all_combinations_v
                  <f, type_pack<short, int, long, long long>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <f, type_pack<short, void *>>);

    using f2 = void (*)(int, int);
    static_assert(is_invocable_v<f2, int, int>);
    static_assert(is_invocable_for_all_combinations_v
                  <f2, type_pack<int, long>, type_pack<short, int>>);
    static_assert(!is_invocable_for_all_combinations_v
                  <f2, type_pack<int, long>, type_pack<short, int, void *>>);
  }

  // invoke_result_for_all_combinations(_t)<F, S...>
  // type_erased_invocation_array<VOID_PTR, RET, FO, TYPE_PACKS...>()
  {
    const auto f = [](auto x, auto y) {return x + y;};
    static_assert(is_invocable_for_all_combinations_v
                  <decltype(f) &,
                   type_pack<bool, short, int>, type_pack<long, long long>>);
    using r = invoke_result_for_all_combinations_t
      <decltype(f) &, type_pack<bool, short, int>, type_pack<long, long long>>;
    static_assert(is_same_v<r, long long>);

    const auto a = type_erased_invocation_array
      <const void *, r, decltype(f) &,
       type_pack<short &, int &>, type_pack<long &, long long &>>();
    short i = 3;
    long long j = 4;
    assert(do_type_erased_invocation<const void *>(a[0 * 2 + 1], f, i, j) == 7);
  }

  // base_of_type_packs(_v)<I, S...>
  {
    static_assert(base_of_type_packs_v
                  <0, type_pack<int, int>, type_pack<int, int, int>,
                   type_pack<int, int, int, int>> == 12);
    static_assert(base_of_type_packs_v
                  <1, type_pack<int, int>, type_pack<int, int, int>,
                  type_pack<int, int, int, int>> == 4);
    static_assert(base_of_type_packs_v
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
  // class bad_variant_access : public exception
  //
  // variant_size(_v)<T>
  // variant_alternative(_t)<I, T>
  // holds_alternative<T>(v)
  //
  // get<I>(v) // may throw bad_variant_access
  // get<T>(v) // may throw bad_variant_access
  // at<I>(v) // may throw bad_variant_access
  // as<T>(v) // may throw bad_variant_access
  //
  // get_if<I>(vp)->pointer
  // get_if<T>(vp)->pointer
  // 
  // monostate
  //   == <=>
  //
  // uses_allocator<variant<S...>, AL> : true_type
  // 
  // visit(f, vs...) // may throw (weak)
  // visit_r<R>(f, vs...) // may throw (weak)
  // 
  // variant<S...>
  //   special member functions
  //     optional by S... // may throw from the corresponding smf of S...
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
  //   variant(x) // may throw
  //   operator =(x) // may throw
  //
  //   emplace<X>(s...) // may throw (weak)
  //   emplace<X>({...}, s...) // may throw (weak)
  //
  //   emplace<I>(s...) // may throw (weak)
  //   emplace<I>({...}, s...) // may throw (weak)
  // 
  //   emplace<void>()
  // 
  //   valueless_by_exception()
  //   index() // index() == npos() if valueless_by_exception()
  //   npos()
  // 
  //   is<T>() // may throw
  //   as<T>() // may throw bad_variant_access
  //   at<I>() // may throw bad_variant_access
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
  // linear_congruential_engine<UINT, ...>
  // minstd_rand0
  // minstd_rand
  //
  // mersenne_twister_engine<UINT, ...>
  // mt19937
  // mt19937_64
  //
  // uniform_int_distribution<INT = int>
  // rational_bernoulli_distribution<INT = int>
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
  //   ()()
  //   discard(z = 1)
  //   ()(i, j) // make a randomized value in [i, j]
  //   gen_bool(num, den)
  //     // den != 0
  //     // if num >= den, num means den
  //     // if num == 0, always return false
  //   ()(prob)
  // make_rander<E = minstd_rand>()
}

// algorithm
void doc_equal() {
  // equal(ir, ir2, eq = ...) // may throw
  // equal(ir, ii, eq = ...) // may throw
}
void doc_allanynone_of() {
  // all/any/none_of(ir, eq) // may throw
  // all/any/none_of_equal(ir, x, eq = ...) // may throw

  assert(all_of_equal(empty_rng<int>(), 0));
  assert(!any_of_equal(empty_rng<int>(), 0));
  assert(none_of_equal(empty_rng<int>(), 0));
}
void doc_for_each() {
  // for_each(ir, f) // may throw (weak)
  //
  // for_each(ir, ir2, f) // may throw (weak)
  // for_each(_plus)(ir, ii, f) // may throw (weak)
  //
  // for_each_excluding_first(ir, f, f_for_first = empty_function{})
  // for_each_excluding_last(fr, f, f_for_last = empty_function{})
}
void doc_find() {
  // find(ir, x, eq = ...) // may throw
  // find_not(ir, x, eq = ...) // may throw
  // find_if(ir, eq) // may throw
  // find_if_not(ir, eq) // may throw
}
void doc_find_last() {
  // find_last(fr, x, eq = ...) // may throw
  // find_last_not(fr, x, eq = ...) // may throw
  // find_last_if(fr, eq) // may throw
  // find_last_if_not(fr, eq) // may throw

  const int a[] = {1, 1, 2, 3};
  assert(find_last(a, 1) == nth(a, 1));
}
void doc_find_first_last_of() {
  // find_first_of(ir, fr, eq = ...) // may throw
  // find_last_of(fr, fr, eq = ...) // may throw

  const int a[] = {1, 2, 3, 4};
  assert(find_first_of(a, seq(0, 2, 3)) == nth(a, 1));
  assert(find_last_of(a, seq(0, 2, 3)) == nth(a, 2));
}
void doc_adjacent_find() {
  // adjacent_find(fr, eq = ...) // may throw

  const int a[] = {1, 2, 2, 3};
  assert(adjacent_find(a) == begin(a) + 1);
}
void doc_count() {
  // count_if(ir, eq) // may throw
  // count(ir, x) // may throw

  const int a[] = {1, 2, 2, 3};
  assert(count_if(a, bind(equal_to<>(), _1, 2)) == 2);
  assert(count(a, 2) == 2);
}
void doc_mismatch() {
  // mismatch(ir, ii, eq = ...) // may throw
  // mismatch(ir, ir_2, eq = ...) // may throw

  const int a[] = {1, 2, 3};
  const int b[] = {1, 3, 3};
  auto [it1, it2] = mismatch(a, b);
  assert(it1 == nth(a, 1) && it2 == nth(b, 1));
}
void doc_is_permutation() {
  // is_permutation(fr, fr_2, eq = ...) // may throw
  // is_permutation(fr, fi, eq = ...) // may throw

  const int a[] = {1, 2, 3};
  const int b[] = {2, 3, 1};
  assert(is_permutation(a, b));
}
void doc_find_subrange() {
  // find_subrange(fr, fr_2, eq = ...)->iter_pair
  // find_last_subrange(fr, fr_2, eq = ...)->iter_pair

  const auto a = seq(1, 2, 3, 4);
  const auto p = find_subrange(a, seq(2, 3));
  assert(p == rng(nth(a, 1), 2));
}
void doc_search() {
  // search(fr, fr_2, eq = ...) // may throw

  const int a[] = {1, 2, 2, 2, 3};
  assert(search(a, rng(3, 2)) == nth(a, 1));
  assert(search(a, seq(2, 2)) == nth(a, 1));
  assert(search(a, empty_rng<int>()) == begin(a));
}
void doc_find_end() {
  // find_end(fr, fr_2, eq = ...) // may throw

  const int a[] = {1, 2, 3, 2, 3, 4};
  assert(find_end(a, seq(2, 3)) == nth(a, 3));
}
void doc_contains() {
  // contains(ir, x, eq = ...) // may throw
  // contains_subrange(fr, fr2, eq = ...) // may throw

  assert(contains(seq(1, 2, 3), 2));
  assert(contains_subrange(seq(1, 2, 3), seq(2, 3)));
}
void doc_starts_ends_with() {
  // starts_with(ir, ir2, eq = ...) // may throw
  // ends_with(fr, fr2, eq = ...) // may throw

  assert(starts_with(seq(1, 2, 3), seq(1, 2)));
  assert(ends_with(seq(1, 2, 3), seq(2, 3)));
}
void doc_fold_left_right() {
  // fold_left(_plus)(ir, init, add) // may throw
  // fold_left_first(_plus)(ir, add) // may throw
  // fold_right(br, init, add) // may throw
  // fold_right_last(br, add) // may throw

  const auto f1 = [](int x, int y) {return x * 10 + y;};
  const auto f2 = [](int x, int y) {return x + y * 10;};
  assert(fold_left(seq(2, 3), 1, f1) == 123);
  assert(fold_left_first(rng(0, 0), f1).empty());
  assert(fold_left_first(seq(2, 3), f1).value() == 23);
  assert(fold_right(seq(3, 2), 1, f2) == 123);
  assert(fold_right_last(seq(3, 2, 1), f2).value() == 123);
}
void doc_copy_move_if_backward_from() {
  // copy(_plus)(ir, oi) // may throw (weak)
  // move(_plus)(ir, oi) // may throw (weak)
  // copy_if(_plus)(ir, oi, eq) // may throw (weak)
  // copy_backward(br, bi) // may throw (weak)
  // move_backward(br, bi) // may throw (weak)
  // copy_from(_plus)(fr, ii) // may throw (weak)
  // move_from(_plus)(fr, ii) // may throw (weak)
  //
  // for overlap case:
  //   to left: copy/move
  //   to right: [copy/move]_backward
}
void doc_swap_ranges() {
  // swap_ranges(_plus)(fr, fi) // may throw (weak)
  // swap_ranges(fr, fr) // may throw (weak)
}
void doc_transform() {
  // transform(_plus)(ir, oi, ufn) // may throw (weak)
  // transform(_plus)(ir, ii, oi, bfn) // may throw (weak)
  // transform(_plus)(ir, ir2, oi, bfn) // may throw (weak)

  const int a[] = {1, 2, 3};
  const int b[] = {2, 4, 6};
  int c[3];
  transform(a, begin(c), negate{});
  assert(equal(c, vector<int>{-1, -2, -3}));
  transform(a, begin(b), begin(c), plus{});
  assert(equal(c, vector<int>{3, 6, 9}));
}
void doc_replace_if_copy_if() {
  // replace(_plus)(fr, x, y) // may throw (weak)
  // replace_if(_plus)(fr, eq, y) // may throw (weak)
  // replace_copy(_plus)(ir, oi, x, y) // may throw (weak)
  // replace_copy_if(_plus)(ir, oi, eq, y) // may throw (weak)

  int a[] = {1, 2, 3};
  replace(a, 2, 3);
  assert(equal(a, seq(1, 3, 3)));
}
void doc_fill_generate() {
  // fill(_plus)(ir, x) // may throw (weak)
  // fill_zero(ir) // may throw (weak)
  // bytewise_fill(cr, x) // may throw (weak)
  //   // fill with static_cast<unsigned char>(x) for all bytes
  // generate(_plus)(ir, ufn) // may throw (weak)

  int a[] = {1, 2, 3};
  fill(a, 0);
  assert(equal(a, seq(0, 0, 0)));
  fill_zero(a);
  assert(equal(a, seq(0, 0, 0)));
  bytewise_fill(a, 'a');
  assert(all_of_equal
         (rng((const char *)(void *)begin(a), sizeof(int) * 3), 'a'));
  generate(a, [i = 0]() mutable {return ++i;});
  assert(equal(a, seq(1, 2, 3)));
}
void doc_remove_if_copy_if() {
  // remove(fname) // std::remove
  //
  // remove_if(fr, eq) // may throw (weak)
  // remove(fr, x) // may throw (weak)
  // remove_copy_if(_plus)(ir, oi, eq) // may throw (weak)
  // remove_copy(_plus)(ir, oi, x) // may throw (weak)

  int aa[] = {1, 0, 0, 2, 0, 3, 0, 0, 0, 4};
  int *const p = re::remove(aa, 0);
  assert(equal(rng(begin(aa), p), seq(1, 2, 3, 4)));
}
void doc_unique_copy() {
  // unique(fr, eq = ...) // may throw (weak)
  // unique_copy(ir, oi, eq = ...) // may throw (weak)

  int a[] = {1, 2, 2, 3, 3, 3};
  const int *p = unique(a);
  assert(equal(rng(cbegin(a), p), irng(1, 4)));
}
void doc_reverse_copy() {
  // reverse(br, swap_fn = adl_swap) // may throw (weak)
  // reverse_copy(br, oi) // may throw (weak)

  int a[] = {1, 2, 3};
  reverse(a);
  assert(equal(a, rrng(irng(1, 4))));
}
void doc_rotate_copy() {
  // rotate(fr, i, do_swap = adl_swap) // may throw (weak)
  // rotate_copy(fr, i, oi) // may throw (weak)

  int a[] = {1, 2, 3, 4, 5};
  int *it = rotate(a, begin(a) + 2);
  assert(equal(a, seq(3, 4, 5, 1, 2)));
  assert(equal(rng(begin(a), it), seq(3, 4, 5)));
  assert(equal(rng(it, end(a)), seq(1, 2)));
}
void doc_shift() {
  // shift_left(fr, mid) // may throw (weak)
  // shift_left(fr, n) // may throw (weak)
  // shift_right(fr, mid) // may throw (weak)
  // shift_right(fr, n) // may throw (weak)

  ez_vector<int> v = {1, 2, 3, 4};
  assert(equal(rng(begin(v), shift_left(v, nth(v, 2))), irng(3, 5)));
  v = {1, 2, 3, 4};
  assert(equal(rng(shift_right(v, 1), end(v)), irng(1, 4)));
}
void doc_sample() {
  // sample(ir, or, rand_engine_ref) // may throw (weak)
  //   // return a iterator to indicate the used space in or
}
void doc_shuffle() {
  // shuffle(rr, rand_engine_ref) // may throw (weak)
}
void doc_binary_search_series() {
  // lower_bound(fr, x, less = ...) // may throw
  // upper_bound(fr, x, less = ...) // may throw
  // equal_range(fr, x, less = ...) // may throw
  //
  // binary_search(fr, x, less = ...)->bool // may throw
  // bs_find_range(fr, min, max, less = ...) // may throw
  // bs_find(fr, x, less = ...) // may throw

  const int a[] = {1, 2, 3, 3, 5};
  assert(partition_point(a, bind(less{}, _1, 3)) == nth(a, 2));
  const auto it = lower_bound(a, 3);
  const auto it2 = upper_bound(a, 3);
  assert(equal(rng(it, it2), seq(3, 3))
         && equal(rng(it, it2), equal_range(a, 3)));
  assert(!binary_search(a, 4));
  assert(equal(bs_find_range(a, 2, 3), seq(2, 3, 3)));
  assert(bs_find(a, 4) == end(a));
}
void doc_partition_series() {
  // is_partitioned(ir, eq) // may throw
  // partition_point(fr, eq)) // may throw
  // partition(fr, eq)) // may throw (weak)
  // partition_copy(ir, oi_true, oi_false, eq)) // may throw (weak)
  // stable_partition_with_buffer(fr, buf, eq)) // may throw (weak)
  // stable_partition_with_no_buffer(br, eq)) // may throw (weak)
  // stable_partition(br, eq)) // may throw (weak)

  int a[] = {0, 0, 0, 1, 1, 1};
  const auto it = partition(a, bind(equal_to{}, _1, 1));
  assert(equal(rng(begin(a), it), rng(3, 1)));
  assert(equal(rng(it, end(a)), rng(3, 0)));
}
void doc_merge_series() {
  // merge(_plus)(ir, ir2, oi, less = ...) // may throw (weak)
  // inplace_merge_with_no_buffer(br, bi, less = ...) // may throw (weak)
  // inplace_merge_with_buffer(br, bi, buf, less = ...) // may throw (weak)
  // inplace_merge(br, bi, less = ...) // may throw (weak)

  int d[] = {1, 3, 5, 2, 4};
  inplace_merge(d, begin(d) + 3);
  assert(equal(d, irng(1, 6)));
}
void doc_set_series() {
  // includes(ir, ir2, less = ...) // may throw
  // set_union(_plus)(ir, ir2, oi, less = ...) // may throw
  // set_intersection(_plus)(ir, ir2, oi, less = ...) // may throw
  // set_difference(_plus)(ir, ir2, oi, less = ...) // may throw
  // set_symmetric_difference(_plus)(ir, ir2, oi, less = ...) // may throw

  int a[] = {1, 3, 5, 7};
  int b[] = {5, 7, 9, 10};
  ez_vector<int> c, d, e, f;
  set_union(a, b, to_back(c));
  set_intersection(a, b, to_back(d));
  set_difference(a, b, to_back(e));
  set_symmetric_difference(a, b, to_back(f));

  assert(includes(c, a) && includes(c, b));
  assert(equal(c, seq(1, 3, 5, 7, 9, 10)));
  assert(equal(d, seq(5, 7)));
  assert(equal(e, seq(1, 3)));
  assert(equal(f, seq(1, 3, 9, 10)));
}
void doc_heap_series() {
  // push_heap(rr, less = ...) // may throw (weak)
  // pop_heap(rr, less = ...) // may throw (weak)
  // make_heap(rr, less = ...) // may throw (weak)
  // sort_heap(rr, less = ...) // may throw (weak)
  // is_heap(rr, less = ...) // may throw
  // is_heap_until(rr, less = ...) // may throw
  // heap_sort(rr, less = ...) // make_heap then sort_heap // may throw (weak)
}
void doc_min_max() {
  // min_element(fr, less = ...) // may throw
  //   // get the first element than which no one is smaller
  // max_element(fr, less = ...) // may throw
  //   // get the first element than which no one is greater
  // minmax_element(fr, less = ...) // may throw
  //   // the min element is min_element(fr, less)
  //   // the max element is the rightmost one among equal values
  //
  // min({...}, less = ...) // may throw
  // min(x, y, less = ...) // may throw
  // max({...}, less = ...) // may throw
  // max(x, y, less = ...) // may throw
  // minmax({...}, less = ...) // may throw
  // minmax(x, y, less = ...) // may throw

  int x = 2;
  int y = 1;
  const auto [a, b] = minmax(x, y);
  assert(&a == &y && &b == &x);
}
void doc_clamp() {
  // clamp(x, lower, upper, less) // may throw
  // clamp(x, lower, upper) // may throw

  assert(clamp(0, 1, 3) == 1);
  assert(clamp(1, 1, 3) == 1);
  assert(clamp(2, 1, 3) == 2);
  assert(clamp(3, 1, 3) == 3);
  assert(clamp(4, 1, 3) == 3);
}
void doc_lexicographical_compare() {
  // lexicographical_compare(ir1, ir2, less = ...) // may throw
  // lexicographical_compare_three_way // may throw
  //   (ir1, ir2, cmp = compare_three_way) // may throw
  // lexicographical_synth_3way(ir1, ir2) // may throw
}
void doc_permutation() {
  // next_permutation(r, less) // may throw (weak)
  // next_permutation(r) // may throw (weak)
  // prev_permutation(r, less) // may throw (weak)
  // prev_permutation(r) // may throw (weak)

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
}
void doc_sort_series() {
  // is_sorted_until(fr, less = ...) // may throw
  // is_sorted(fr, less = ...) // may throw
  // sort(rr, less = ...) // may throw (weak)
  // stable_sort(rr, less = ...) // may throw (weak)
  // stable_sort_with_no_buffer(rr, less = ...) // may throw (weak)
  // stable_sort_with_buffer(rr, buf, less = ...) // may throw (weak)
  // partial_sort(rr, ri, less = ...) // may throw (weak)
  // partial_sort_copy(fr, rr, less = ...) // may throw (weak)
  // nth_element(rr, ri, less = ...) // may throw (weak)
  {
    int a[] = {5, 3, 4, 1, 6, 2, 7};
    int *i = nth(a, 3);
    partial_sort(a, i);
    assert(equal(rng(begin(a), i), seq(1, 2, 3)));
  }
  {
    int a[] = {5, 3, 4, 1, 6, 2, 7};
    int *i = nth(a, 3);
    nth_element(a, i);
    sort(rng(begin(a), i));
    sort(rng(i, end(a)));
    assert(equal(a, irng(1, ssize(a) + 1)));
  }
}
void doc_accumulate_reduce() {
  // accumulate(_plus)(ir, init = ir_vt{}, plus = ...) // may throw
  // reduce(_plus)(ir, init = ir_vt{}, plus = ...) // may throw

  assert(reduce(seq(1, 2, 3)) == 6);
}
void doc_inner_product() {
  // inner_product(ir, ii, init, plus, mul) // may throw
  // inner_product(ir, ii, init) // may throw
  // transform_reduce(ir, ii, init, plus, mul) // may throw
  // transform_reduce(ir, ii, init) // may throw
  // transform_reduce(ir, init, plus, proj) // may throw

  assert(transform_reduce(seq(2, 3), begin(seq(4, 5)), 0) == 23);
}
void doc_partial_sum_series() {
  // partial_sum(ir, oi, plus) // may throw (weak)
  // partial_sum(ir, oi) // may throw (weak)
  //
  //           inclusive_scan(ir, oi, plus = ...) // may throw (weak)
  //           inclusive_scan(ir, oi, plus, init) // may throw (weak)
  // transform_inclusive_scan(ir, oi, plus, proj) // may throw (weak)
  // transform_inclusive_scan(ir, oi, plus, proj, init) // may throw (weak)
  //
  //           exclusive_scan(ir, oi, init, plus = ...) // may throw (weak)
  // transform_exclusive_scan(ir, oi, init, plus, proj) // may throw (weak)

  int a[] = {1, 3, 5};
  int b[3];

  partial_sum(a, begin(b));
  assert(equal(b, seq(1, 4, 9)));

  inclusive_scan(a, begin(b));
  assert(equal(b, seq(1, 4, 9)));
  transform_inclusive_scan(a, begin(b), plus<>(), negate<>(), 100);
  assert(equal(b, seq(99, 96, 91)));

  exclusive_scan(a, begin(b), 100);
  assert(equal(b, seq(100, 101, 104)));
  transform_exclusive_scan(a, begin(b), 100, plus<>(), negate<>());
  assert(equal(b, seq(100, 99, 96)));
}
void doc_adjacent_difference() {
  // adjacent_difference(ir, oi, minus = ...) // may throw (weak)

  int a[] = {1, 2, 4};
  int b[3];
  adjacent_difference(a, begin(b));
  assert(equal(b, seq(1, 1, 2)));
}
void doc_iota() {
  // iota(fr, x) // may throw (weak)

  int a[3];
  iota(a, 1);
  assert(equal(a, irng(1, 4)));
}
void doc_gcd_and_lcm() {
  // gcd(x, y) // may throw
  //   // requires: x >= 0 && y >= 0, the result can not overflow
  // lcm(x, y) // may throw
  //   // requires: x >= 0 && y >= 0, the result can not overflow

  assert(gcd(6, 9) == 3);
  assert(lcm(6, 9) == 18);

  assert(gcd(0, 0) == 0);
  assert(lcm(0, 0) == 0);
  assert(gcd(0, 9) == 9);
  assert(lcm(0, 9) == 0);
}
void doc_midpoint() {
  // midpoint(x, y) // requires: x, y are both integral(no bool), or pointer

  assert(midpoint(1, 3) == 2);
  assert(midpoint(3, 1) == 2);
  assert(midpoint(1, 4) == 2);
  assert(midpoint(4, 1) == 3);
  assert(midpoint(-1, 1) == 0);
  assert(midpoint(1, -1) == 0);
  assert(midpoint(-1, 2) == 0);
  assert(midpoint(2, -1) == 1);
}
void doc_for_each_node() {
  // // use case:
  // //   fn(iterator) may invalidate the iterator but keep the rest valid
  // // destroyf version:
  // //   requires: only fn(it) can throw, and destroyf(it) can not fail after
  // //   fn(it) throwing
  // //   when throwing at nth element, apply destroyf for all iterators
  // //   in [nth, end)
  // for_each_node(fr, get_next, fn, destroyf) // may throw
  //   // requires: iterator operations can not throw
  // for_each_node(fr, get_next, fn) // may throw (weak)

  list<int> l = {1, 2, 3};
  for_each_node(l, next, [&l](auto it) {l.erase(it);});
  assert(l.empty());
}
void doc_list_fns() {
  // list_functions<GET_NEXT, SET_NEXT, GET_PREV>
  //   link(next, node)
  //   link(next, first_it, last_it)
  //   unlink(node)
  //   unlink(from, to)
  //   unlink_all(end_node)
  // list_fns(get_next, set_next, get_prev, set_prev)
  // list_fns()
  //   // get_next(it): it.next()
  //   // set_next(it, x): it.next(x)
  //   // get_prev(it): it.prev()
  //   // set_prev(it, x): it.prev(x)
  //
  // forward_list_functions<GET_NEXT, SET_NEXT>
  //   link_after(prev, node)
  //   link_after(prev, it, last_it)
  //   unlink_after(prev)
  //   unlink_after(prev, next)
  //   unlink_all(end_node)
  // forward_list_fns(get_next, set_next)
  // forward_list_fns()
  //   // get_next(it): it.next()
  //   // set_next(it, x): it.next(x)
}
void doc_list_unique() {
  // list_unique(l, eq, erase_f) // may throw (weak)

  list<int> l = {1, 2, 2, 3, 3, 3};
  list_unique(l, equal_to<>(), [&l](auto i) {return l.erase(i);});
  assert(equal(l, irng(1, 4)));
}
void doc_combination_range() {
  // permutation_count(n, k = n) // may throw
  //   // requires: k <= n, {n, k} >= 0, the result can not overflow
  //   // if k == 0 or k == n, return 1
  // combination_count(n, k) // may throw
  //   // requires: k <= n, {n, k} >= 0, the result can not overflow 
  //   // if k == 0 or k == n, return 1
  {
    assert(permutation_count(9, 0) == 1);
    assert(combination_count(9, 0) == 1);
    assert(combination_count(9, 4)
           == permutation_count(9, 4) / permutation_count(4));
  }

  // combination_iterator<BR_ITER, BUF_T>
  //   // requires:
  //   //   value_type of BUF_T is BR_ITER
  //   //   BUF_T can not throw from iterator-related operations
  //   iterator category: iitr
  //   combination_iterator(buf_p, r_ed)
  // combination_itr(bufp, r_ed) // size of *bufp is among (0, size(r)]
  {
    auto a = seq(1, 2, 3);
    ez_vector<int *> v{addressof(a[0]), addressof(a[1])};
    auto i = combination_itr(addressof(v), end(a));
    auto j = combination_itr(static_cast<ez_vector<int *> *>(nullptr), end(a));
    assert(equal(rng(i, j), seq(seq(1, 2), seq(1, 3), seq(2, 3)), equal));
  }

  // combination_range<BR>
  //   combination_range(r, n) // requires: 0 <= n <= size(br)
  // combination_rng(r, n) // requires: 0 <= n <= size(br)
  {
    assert(equal(combination_rng(seq(1, 2, 3), 1u),
               seq(seq(1), seq(2), seq(3)), equal));

    auto r = combination_rng(seq(1, 2, 3), 0u);
    assert(empty(r));

    assert(equal(combination_rng(seq(1, 2, 3), 0u),
                 empty_rng<empty_range<int>>(), equal));
  }

  // combination(br, n, o) // requires: 0 <= n <= size(br)
  {
    const ez_vector<int> v = {1, 2, 3};
    ez_vector<ez_vector<int>> vv;

    combination(v, 0u, output_itr([&vv](auto &&r) {
      ez_vector<int> tmp;
      for (int x : r)
        tmp.insert(tmp.end(), x);
      vv.insert(vv.end(), move(tmp));
    }));
    assert(vv.empty());

    combination(v, 2u, output_itr([&vv](auto &&r) {
      ez_vector<int> tmp;
      for (int x : r)
        tmp.insert(tmp.end(), x);
      vv.insert(vv.end(), move(tmp));
    }));
    assert(equal(vv, seq(seq(1, 2), seq(1, 3), seq(2, 3)), equal));
  }
}
void doc_filter_range() {
  // filter(_plus)(ir, oi, eq)
  //
  // filter_iterator<II, F>
  //   iterator category: iitr - fitr
  //   filter_iterator(it, ed, f)
  //   filter_iterator(const filter_iterator<II2, F2> &)
  //   base()
  // filter_itr(it, ed, f)
  //
  // filter_range<IR, F>
  //   filter_range(r, f)
  // filter_rng(r, f)

  int a[] = {0, 0, 1, 0, 1, 1, 0};
  int b[3];
  assert(end(b) == filter(a, begin(b), bind(equal_to<>(), _1, 1)));
  assert(equal(b, rng(3, 1)));

  assert(equal(filter_rng(seq(0, 1, 0, 1), bind(equal_to{}, _1, 1)),
               rng(2, 1)));
}
void doc_take_range() {
  // take(_plus)(r, n, o)
  //
  // take_iterator<I>
  //   iterator category: iitr - fitr
  //   take_iterator(i, n)
  //   base()
  //   nth()
  // take_itr(i, n)
  //
  // take_range<R>
  //   take_range(r, n)
  // take_rng(r, n)

  assert(equal(take_rng(seq(1, 2, 3), 0), empty_rng<int>()));
  assert(equal(take_rng(seq(1, 2, 3), 2), seq(1, 2)));
  assert(equal(take_rng(seq(1, 2, 3), 4), irng(1, 4)));
}
void doc_drop_range() {
  // drop(_plus)(r, n, o)
  //
  // drop_range<R>
  //   drop_range(r, n)
  // drop_rng(r, n)

  const auto r = seq(1, 2, 3);
  assert(equal(drop(r, 4, to_back(ez_vector<int>{})).base(), empty_rng<int>()));
  assert(equal(drop(r, 2, to_back(ez_vector<int>{})).base(), seq(3)));
  assert(equal(drop(r, 1, to_back(ez_vector<int>{})).base(), seq(2, 3)));
  assert(equal(drop_rng(r, 0), irng(1, 4)));
  assert(equal(drop_rng(r, 2), seq(3)));
  assert(equal(drop_rng(r, 4), empty_rng<int>()));
}
void doc_take_while_range() {
  // take_while(_plus)(r, f, o)
  //
  // take_while_iterator<I, F>
  //   iterator category: iitr - fitr
  //   take_while_iterator(i, ed, f)
  //   base()
  //   fn() // requires: the stored function is non-empty
  // take_while_itr(i, ed, f)
  //
  // take_while_range<R, F>
  //   take_while_range(r, f)
  // take_while_rng(r, f)
}
void doc_drop_while_range() {
  // drop_while(_plus)(r, f, o)
  //
  // drop_while_range<R, F>
  //   drop_while_range(r, f)
  // drop_while_rng(r, f)
}
void doc_join_range() {
  // join(r, o)
  //
  // join_iterator<II> // requires: is_rng_ref<itr_ref<II>>
  //   iterator category: iitr - bitr
  //   join_iterator(it, inner_it, it2) // it2 is prev(ed) if it != ed
  //   base()->rng_itr<itr_ref<II>>
  //   outer_iter()->II
  // join_itr(it, inner_it, it2) // requires: is_rng_ref<itr_ref<decltype(it)>>
  //
  // join_range<IR>
  // join_rng(r)
  // join_rng(it, it2)
  // join_rng(it, n)

  assert(equal(join(seq("ab"_sv, "cd"), to_back(ez_vector<int>{})).base(),
               "abcd"_sv));

  ez_vector<ez_vector<int>> a = {{1, 2}, {3, 4}, {5}};
  assert(equal(rng(join_itr(begin(a), begin(*begin(a)), end(a)),
                   join_itr(end(a), decltype(begin(*begin(a))){}, end(a))),
               irng(1, 6)));

  assert(equal(join_rng(seq("ab"_sv, "cd")), "abcd"_sv));
}
void doc_join_with_range() {
  // join_with(r, r2, o)
  //
  // join_with_iterator<II, FI>
  //   iterator category: iitr - bitr
  //   join_with_iterator(it, inner_it, ed, op2, it2, ed2)
  // join_with_itr(it, inner_it, ed, op2, it2, ed2)
  //
  // join_with_range<R, R2>
  // join_with_rng(r, r2)

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
                             seq(3)),
               seq(1, 2, 3, 4, 5)));
}
void doc_adjacent_range() {
  // adjacent(_plus)<N>(fr, o) // requires: N > 0
  //
  // adjacent_iterator<FI, N> // requires: N > 0
  //   iterator category: iitr - ritr
  //   explicit adjacent_iterator(tuple)
  //   base()->tuple<ITERS...>
  // adjacent_itr(its...)
  //
  // adjacent_range<FR, N> // requires: N > 0
  // adjacent_rng<N>(fr) // requires: N > 0
  // bind_adjacent_rng<N>(fr, f) // requires: N > 0

  assert(equal(adjacent<3>(seq(1, 2, 3, 4, 5),
                           to_back(ez_vector<tuple<int, int, int>>{})).base(),
               seq(tuple(1, 2, 3), tuple(2, 3, 4), tuple(3, 4, 5))));

  const int a[] = {1, 2, 3, 4};
  assert(equal(rng(adjacent_itr(nth(a, 0), nth(a, 1)),
                   adjacent_itr(nth(a, 3), nth(a, 4))),
               seq(pair(1, 2), pair(2, 3), pair(3, 4))));

  assert(equal(adjacent_rng<2>(seq(1, 2, 3, 4)),
               seq(pair(1, 2), pair(2, 3), pair(3, 4))));
}
void doc_slide_range() {
  // slide(fr, n, o) // requires: n > 0
  //
  // slide_iterator<FI>
  //   iterator category: iitr - ritr
  //   explicit slide_iterator(i1, i2)
  //   base()->iter_pair<FI>
  // slide_itr(i1, i2)
  // slide_itr(i1, n) // requires: n > 0
  //
  // slide_range<FR>
  // slide_rng(fr, n) // requires: n > 0

  ez_vector<ez_vector<int>> v;
  slide(seq(1, 2, 3, 4), 3, output_itr([&v](auto view) {
    v.insert(v.end(), {});
    for (int x : view)
      back(v).insert(back(v).end(), x);
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
               seq(seq(1, 2), seq(2, 3), seq(3, 4)), equal));
}
void doc_aligned_stride_range() {
  // aligned_stride(r, n, o) // requires: n > 0 && size(r) % n == 0
  //
  // aligned_stride_iterator<I>
  //   iterator category: iitr - ritr
  //   aligned_stride_iterator(i, n) // requires: n > 0
  //   base()->I
  //   stride()
  // aligned_stride_itr(i, n) // requires: n > 0
  //
  // aligned_stride_range<R>
  //   aligned_stride_range(r, n) // requires: n > 0 && size(r) % n == 0
  // aligned_stride_rng(r, n) // requires: n > 0 && size(r) % n == 0

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
  // stride(r, s, o) // requires: s > 0
  //
  // stride_iterator<I>
  //   iterator category: iitr - ritr
  //   stride_iterator(i, ed, s, missing = 0) // requires: s > 0
  //   base()->I
  //   stride()
  // stride_itr(i, ed, s, missing = 0)
  //
  // stride_range<R>
  //   stride_range(r, s) // requires: s > 0
  // stride_rng(r, s) // requires: s > 0

  assert(equal(stride(seq(1, 2, 3, 4), 3, to_back(ez_vector<int>{})).base(),
               seq(1, 4)));

  int a[] = {1, 2, 3, 4, 5};
  test_ritr(rng(stride_itr(begin(a), end(a), 2, 0),
                stride_itr(end(a), end(a), 2, 1)),
            seq(1, 3, 5));

  assert(equal(stride_rng(seq(1, 2, 3, 4, 5), 2), seq(1, 3, 5)));
}
void doc_aligned_chunk_range() {
  // aligned_chunk(r, s, o) // requires: s > 0 && size(r) % s == 0
  //
  // aligned_chunk_iterator<FI>
  //   iterator category: fitr - ritr
  //   aligned_chunk_iterator(i, s) // requires: s > 0
  //   base()->FI
  //   stride()
  // aligned_chunk_itr(i, s) // requires: s > 0
  //
  // aligned_chunk_range<FR>
  //   aligned_chunk_range(r, s) // requires: s > 0 && size(r) % s == 0
  // aligned_chunk_rng(r, s) // requires: s > 0 && size(r) % s == 0

  ez_forward_list<int> l = {1, 2, 3, 4};
  assert(equal(rng(aligned_chunk_itr(begin(l), 2),
                   aligned_chunk_itr(end(l), 2)),
               seq(seq(1, 2), seq(3, 4)), equal));

  assert(equal(aligned_chunk_rng(seq(1, 2, 3, 4), 2),
               seq(seq(1, 2), seq(3, 4)), equal));
}
void doc_chunk_range() {
  // chunk(_plus)(r, s, o) // requires: s > 0
  //
  // chunk_iterator<FI>
  //   iterator category: fitr - ritr
  //   chunk_iterator(i1, i2, ed, s, m) // requires: s > 0, m is valid
  //   base()->iter_pair<FI>
  //   stride()
  // chunk_itr(i1, i2, ed, s, m) // requires: s > 0, m is valid
  //
  // chunk_range<FR>
  //   chunk_range(fr, s) // requires: s > 0
  // chunk_rng(fr, s) // requires: s > 0

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
  // chunk_by(_plus)(fr, f, o)
  //
  // chunk_by_iterator
  //   iterator category: fitr - bitr
  //   chunk_by_iterator(i, i2, op, ed, f)
  //   base()->iter_pair
  // chunk_by_itr(i, i2, op, ed, f)
  //
  // chunk_by_range<FR, EQ>
  // chunk_by_rng(fr, f)

  int a[] = {1, 2, 2, 3};
  test_bitr(chunk_by_itr(nth(a, 0), nth(a, 2), begin(a), end(a), less{}),
            chunk_by_itr(end(a), end(a), begin(a), end(a), less{}),
            seq(seq(1, 2), seq(2, 3)), equal);

  assert(equal(chunk_by_rng(seq(1, 2, 2, 3), less{}),
               seq(seq(1, 2), seq(2, 3)), equal));
}
void doc_inner_cartesian_product_range() {
  // inner_cartesian_product(_plus)(fr, o) // *o = tmp_range_of_refs
  //
  // inner_cartesian_product_range<FR>
  // inner_cartesian_product_rng(r)

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
  // cartesian_product(r, s..., o) // *o = tmp_range_of_refs
  //
  // cartesian_product_iterator<IT, S...>
  //   iterator category: fitr - ritr
  //   explicit cartesian_product_iterator(tuple<array<IT, 3>, array<S, 3>...>)
  //   base()->tuple<array<IT, 3>, array<S, 3>...>
  // cartesian_product_itr(tuple<array<IT, 3>, array<S, 3>...>)
  //
  // cartesian_product_range<FR, S...>
  // cartesian_product_rng(fr, s...)

  assert(equal(cartesian_product_rng(seq(1, 2), seq(3, 4), seq(5, 6)),
               seq(tuple(1, 3, 5), tuple(1, 3, 6),
                   tuple(1, 4, 5), tuple(1, 4, 6),
                   tuple(2, 3, 5), tuple(2, 3, 6),
                   tuple(2, 4, 5), tuple(2, 4, 6))));

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
}
void doc_split_range() {
  // split(_plus)(fr, f, o) // *o = iter_pair
  //
  // split_iterator
  //   iterator category: fitr
  //   split_iterator(i, i2, ed, k)
  //   base()->iter_pair
  // split_itr(i, i2, ed, k)
  //
  // split_range<FR, FR2>
  // split_rng(fr, fr2)

  assert(equal(split_rng(seq(1, 2, 0, 3, 4), seq(0)),
               seq(seq(1, 2), seq(3, 4)), equal));

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
}
void doc_zip_range() {
  // zip_top(x, s...)
  // zip(x, s..., o)
  {
    int a[] = {1, 2};
    int b[] = {3, 4, 5};
    int c[] = {6};
    assert(zip_top(a, b, c) == tuple(nth(a, 1), nth(b, 1), nth(c, 1)));
    assert(zip_top(begin(a), begin(b), c)
           == tuple(nth(a, 1), nth(b, 1), nth(c, 1)));
    assert(equal(zip(a, b, c,
                     to_back(ez_vector<tuple<int, int, int>>{})).base(),
                 seq(tuple(1, 3, 6))));
  }

  // zip_iterator<I, S...>
  //   iterator category: iitr - fitr // note: iitr only for single dimension
  //   explicit zip_iterator(tuple<I, S...>)
  //   explicit(...) zip_iterator(I, S...)
  //   base()->tuple<I, S...>
  // zip_itr(i, s...)
  // zip_itr_from_tuple(t)
  //
  // zip_range<X, S...>
  // zip_rng(x, s...)

  int a[] = {1, 2, 3};
  int b[] = {4, 5};
  assert(zip_top(begin(a), b) == tuple(nth(a, 2), end(b)));
  assert(zip_top(a, b) == tuple(nth(a, 2), end(b)));
  assert(equal(zip_rng(a, b), seq(tuple(1, 4), tuple(2, 5))));
  assert(equal(zip_rng(b, begin(a)), seq(tuple(4, 1), tuple(5, 2))));
}
void doc_aligned_zip_range() {
  // aligned_zip_iterator<I, S...>
  //   iterator category: iitr - ritr // note: iitr only for single dimension
  //   explicit aligned_zip_iterator(tuple<I, S...>)
  //   explicit(...) aligned_zip_iterator(I, S...)
  //   base()->tuple<I, S...>
  // aligned_zip_itr(i, s...)
  // aligned_zip_itr_from_tuple(t)
  //
  // aligned_zip_range<X, S...>
  // aligned_zip_rng(x, s...)

  assert(equal(aligned_zip_rng(iitr(1), seq(1, 2)),
               seq(tuple(1, 1), tuple(2, 2))));
  assert(equal(aligned_zip_rng(seq(1, 2), seq(1, 2, 3, 4), seq(1, 2, 3)),
               seq(tuple(1, 1, 1), tuple(2, 2, 2))));

  int a[] = {1, 2};
  int b[] = {1, 2};
  int c[] = {1, 2};
  assert(equal(rng(aligned_zip_itr(begin(a), begin(b), begin(c)),
                   aligned_zip_itr(end(a), end(b), end(c))),
               seq(tuple(1, 1, 1), tuple(2, 2, 2))));
}
void doc_enumerate_range() {
  // enumerate(_plus)(r, o) // *o = pair<int_t, ref>
  //
  // enumerate_iterator<I>
  //   iterator category: iitr - ritr
  //   enumerate_iterator(i, n)
  //   base()
  //   index()
  // enumerate_itr(i, n)
  //
  // enumerate_range<R>
  // enumerate_rng(r)

  assert(equal(enumerate_rng(seq(1, 2, 3)),
               seq(pair(0, 1), pair(1, 2), pair(2, 3))));
}
void doc_exclusive_rotate_range() {
  // exclusive_rotate(r, i, o)
  //   // exclusive the last valid element than rotate_copy
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
                 seq(1, 2, 3)));
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
  //   iterator category: fitr - ritr
  //   exclusive_rotate_iterator(op, wall, ed, it)
  //   base()->FI
  // exclusive_rotate_itr(op, wall, ed, it)
  //
  // exclusive_rotate_range<FR>
  // exclusive_rotate_rng(r, it)
  // exclusive_rotate_rng(r, d)

  assert(equal(exclusive_rotate_rng(empty_rng<int>(), 0),
               empty_rng<int>()));
  assert(equal(exclusive_rotate_rng(seq(1), 0), empty_rng<int>()));
  assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 4), seq(1, 2)));
  assert(equal(exclusive_rotate_rng(seq(1, 2, 3), 1), seq(3, 1)));
}
void doc_rotate_range() {
  // rotate_iterator<FI>
  //   iterator category: fitr - ritr
  //   rotate_iterator(op, wall, ed, it)
  //   base()->FI
  // rotate_itr(op, wall, ed, it)
  //
  // rotate_range<FR>
  // rotate_rng(r, it)
  // rotate_rng(r, d)

  assert(equal(rotate_rng(empty_rng<int>(), 0), empty_rng<int>()));
  assert(equal(rotate_rng(seq(1, 2, 3), 4), seq(1, 2, 3)));
  assert(equal(rotate_rng(seq(1, 2, 3), 1), seq(2, 3, 1)));
}
void doc_loop_range() {
  // loop(fr, it, n, o)
  //
  // loop_iterator<FI>
  //   iterator category: fitr - ritr
  //   loop_iterator(f, l, it, n)
  //   base()
  //   index()
  // loop_itr(f, l, it, n)
  //
  // loop_range<FR>
  // loop_rng(fr, i, n)
  // loop_rng(fr, n0, n)
  // loop_rng(fr, n)

  assert(loop_rng(empty_rng<int>(), 3).empty());
  assert(equal(loop_rng(seq(1, 2), 3), seq(1, 2, 1)));
  assert(equal(loop_rng(seq(1, 2), 1, 3), seq(2, 1, 2)));
}

// container
void doc_string() {
  // from_unicode_t
  // from_unicode
  //
  // basic_string<T, AL = default_allocator<T>>
  // sso_string<T, size_t N, AL = default_allocator<T>>
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
  //
  //   special member functions: full // may throw
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
  //
  //   this_t(alloc)
  //   this_t(const this_t &, alloc) // may throw
  //   this_t(this_t &&, alloc) // may throw
  //
  //   explicit this_t(n, alloc = {}) // may throw
  //   this_t(n, x, alloc = {}) // may throw
  //   assign(n, x) // may throw
  //
  //   this_t(from, to, alloc = {}) // may throw
  //   assign(from, to) // may throw
  //
  //   this_t(il, alloc = {}) // may throw
  //   operator =(il) // may throw
  //   assign(il) // may throw
  //
  //   insert(cit, x) // may throw
  //   insert(cit, n, x) // may throw
  //   insert(cit, from, to) // may throw
  //   insert(cit, il) // may throw
  //   erase(cit)
  //   erase(from, to)
  //   clear()
  //
  //   front()
  //   back()
  //   push_back(x) // may throw
  //   pop_back()
  //   operator [](n)
  //   at(n) // may throw
  //
  //   local() // only for this_t
  //   data()
  //   capacity()
  //   full()
  //   resize(n, x) // may throw
  //   resize(n) // may throw
  //   reserve(n) // may throw
  //   reserve_more(n) // may throw
  //   reallocate(n = size()) // may throw
  //   shrink_to_fit() // may throw
  //   remove_if(eq) // may throw
  //   remove(x) // may throw
  //   replace(i1, i2, r) // may throw
  //
  //   this_t(from_unicode_t, r) // may throw
  //   try_assign_unicode(s)->bool
  //   assign_unicode(s) // throw or terminate if !try_assign_unicode(s)
  //
  //   this_t(from_range, r, a = {}) // may throw
  //   this_t(array) // may throw
  //   explicit this_t(r) // may throw
  //   this_t(r, alloc) // may throw
  //   operator =(r) // may throw
  //   assign(r) // may throw
  //   assign_range(r) // may throw
  //   insert(cit, r) // may throw
  //   insert_range(cit, r) // may throw
  //   push_back(r) // may throw
  //   append_range(r) // may throw
  //   pop_back(n) // requires: n <= size()
  //   append(s...) // may throw
  //
  // using string = sso_string<char, 15u>;
  // template <class C, class AL = default_allocator<C>>
  // using default_sso_string = sso_string<C, 15u, AL>;
  // using wstring = basic_string<wchar_t>;
  // using u16string = basic_string<char16_t>;
  // using u32string = basic_string<char32_t>;
  // template <class C>
  // using string_t = ...
  //
  // string operator ""_s(const char *s, size_t n);
  // wstring operator ""_s(const wchar_t *s, size_t n);
  // u16string operator ""_s(const char16_t *s, size_t n);
  // u32string operator ""_s(const char32_t *s, size_t n);
  //
  // less<basic_string>
  // less<sso_string>
  // equal_to<basic_string>
  // equal_to<sso_string>
  // hash<basic_string>
  // hash<sso_string>
}
void doc_string_reference() {
  // string_reference<T>
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   string_reference(nullptr_t)
  //   operator =(nullptr_t)
  //   clear()
  //
  //   string_reference(char_ref)
  //   string_reference(r)
  //   string_reference(from, to)
  //   string_reference(r, n, len)
  //   string_reference(const string_reference<const T> &) // ref cast to view
  //
  //   string_reference(ptr) // from c string
  //
  //   special member functions: full
  //   == <=> between {const} T and {const} T
  //   == <=> between built-in array and string_reference
  //   == <=> between basic_string and string_reference
  //   == <=> between sso_string and string_reference
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   size()
  //   empty()
  //   data()
  //   operator []
  //   at(n)
  //
  //   front()
  //   back()
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
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
  // sview operator ""_sv(const char *s, size_t n);
  // wsview operator ""_sv(const wchar_t *s, size_t n);
  // u16sview operator ""_sv(const char16_t *s, size_t n);
  // u32sview operator ""_sv(const char32_t *s, size_t n);
  //
  // less<string_reference<T>>
  // equal_to<string_reference<T>>
  // hash<string_reference<T>>
}
void doc_bitset() {
  // bitset<N>
  //   special member functions: full
  //   == <=>
  //
  //   bitset(unsigned long long) // the part out of N is ignored
  //   operator =(unsigned long long) // the part out of N is ignored
  //
  //   explicit bitset(str, pos = 0, n = -1, zero = '0', one = '1')
  //     // may throw invalid_argument if not zero nor one
  //   explicit bitset(const CHAR *, n = -1, zero = '0', one = '1')
  //     // may throw invalid_argument if not zero nor one
  //
  //   to_ullong() // may throw overflow_error
  //   to_ulong() // may throw overflow_error
  //   to<STRING>(zero = '0', one = '1')
  //   to_string(zero = '0', one = '1')
  //   print(oitr, zero = '0', one = '1')
  //   print() // print to stdout
  //
  //   reference
  //     operator bool()
  //     bool operator ~()
  //     flip()
  //   const_reference = bool
  //   pointer = reference *
  //   const_pointer = const reference *
  //   value_type = bool
  //   iterator
  //   const_iterator
  //   difference_type
  //   size_type
  //
  //   empty()
  //   size()
  //   max_size()
  //
  //   operator [](n)
  //   at(n) // may throw out_of_range
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   set()
  //   set(i, y) // may throw out_of_range
  //   reset()
  //   reset(i) // may throw out_of_range
  //
  //   count()
  //   ~
  //   flip()
  //   flip(i) // may throw out_of_range
  //   test(i) // may throw out_of_range
  //   all()->bool
  //   any()->bool
  //   none()->bool
  //
  //   &=(const this_t &)
  //   |=(const this_t &)
  //   ^=(const this_t &)
  //   <<=(size_type)
  //   >>=(size_type)
  //   <<(size_type)
  //   >>(size_type)
  //
  // hash<bitset<N>>

  bitset<4> s(0b1011u);
  assert(equal(s, seq(true, true, false, true)));
  bitset<4> s2(0b1001011u);
  assert(equal(s2, seq(true, true, false, true)));

  bitset<4> s3("1011"_sv);
  assert(equal(s3, seq(true, true, false, true)));
  bitset<4> s4("10"_sv);
  assert(equal(s4, seq(false, true, false, false)));
  bitset<4> s5("10110001"_sv);
  assert(equal(s5, seq(true, true, false, true)));

  bitset<4> s6("10110001");
  assert(equal(s6, seq(true, true, false, true)));
}
void doc_vector() {
  // vector<T, AL = default_allocator<T>>
  //   pointer = T *
  //   const_pointer = const T *
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
  //   allocator_type
  //   get_allocator()
  //   vector(alloc)
  //   vector(const vector &, alloc)
  //   vector(vector &&, alloc)
  //   explicit vector(n, alloc = {})
  //   vector(n, x, alloc = {})
  //   assign(n, x)
  //   vector(from, to, alloc = {})
  //   assign(from, to)
  //   vector(il, alloc = {})
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
  //   operator [](n)
  //   at(n)
  //
  //   data()
  //   capacity()
  //   full()
  //   resize(n, x)
  //   resize(n)
  //   reserve(size_type n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   vector(from_range, r, a = AL{})
  //   explicit vector(r)
  //   vector(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //   append(s...)
}
void doc_bool_vector() {
  // vector<bool, AL = default_allocator<bool>>
  //   pointer = void
  //   const_pointer = void
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
  //   vector(alloc)
  //   vector(const vector &, alloc)
  //   vector(vector &&, alloc)
  //   explicit vector(n, alloc = {})
  //   vector(n, x, alloc = {})
  //   assign(n, x)
  //   vector(from, to, alloc = {})
  //   assign(from, to)
  //   vector(il, alloc = {})
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
  //   operator [](n)
  //   at(n)
  //
  //   capacity()
  //   full()
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   flip()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   vector(from_range, r, a = AL{})
  //   explicit vector(r)
  //   vector(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //   append(s...)
  //
  // hash<vector<bool, AL>>
}
void doc_local_vector() {
  // local_vector<T, size_t N>
  //   pointer = void
  //   const_pointer = void
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
  //   operator [](n)
  //   at(n)
  //
  //   data()
  //   capacity()
  //   resize(n)
  //   resize(n, x)
  //   full()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   local_vector(from_range, r)
  //   explicit local_vector(r)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //   append(s...)
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
  //   operator [](n)
  //   at(n)
  //
  //   local_buffer_size()
  //   local()
  //   data()
  //   capacity()
  //   full()
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reserve_more(n)
  //   reallocate(n = size())
  //   shrink_to_fit()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   small_vector(from_range, r, a = AL{})
  //   explicit small_vector(r)
  //   small_vector(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //   append(s...)

  small_vector<int, 3, test_allocator<int>> v;
  assert(v.capacity() == 3);
}
void doc_pointer_vector() {
  // pointer_vector<T, V = vector<T *>>
  //   pointer = V::value_type
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
  //
  //   new_node(...)->pointer
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
  //   allocator_type
  //   get_allocator()
  //   explicit pointer_vector(a)
  //   pointer_vector(const pointer_vector &, a)
  //   pointer_vector(pointer_vector &&, a)
  //
  //   explicit pointer_vector(n, a = {})
  //   pointer_vector(n, x, a = {})
  //   assign(n, x)
  //   pointer_vector(from, to, a = {})
  //   assign(from, to)
  //
  //   pointer_vector(il, a = {})
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
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   operator [](n)
  //   at(n)
  //
  //   capacity()
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
  //   remove_if(eq)
  //   remove(x)
  //   unique(eq = ...)
  //   merge(this_ref, less = ...)
  //   reverse()
  //   sort(less = ...)
  //
  //   full()
  //   reserve_more(n)
  //   reallocate(n = size())
  //   replace(i1, i2, r) // may throw
  //
  //   pointer_vector(from_range, r, a = {})
  //   explicit pointer_vector(r)
  //   pointer_vector(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(n)
  //   pop_front(n)
  //   append(s...)
  //   prepend(s...)
  //
  //   node_type = unique_ptr<...>
  //   make_node(s...)->node_type
  //   extract(ci)->node_type
  //   insert(ci, node_type &&)->it
  //   push_front(ci, node_type &&)
  //   push_back(ci, node_type &&)
  //   exchange(ci, node_type &)->node_type
  //   exchange(ci, node_type &&)->node_type
  //   replace(ci, node_type &&)->it
  //
  //   extract(ci, ci2)
  //   insert(ci, this_t &&) // no overlap
  //   push_front(ci, this_t &&) // no overlap
  //   push_back(ci, this_t &&) // no overlap
  //   exchange(ci, ci2, this_t &)->this_t // no overlap
  //   exchange(ci, ci2, this_t &&)->this_t // no overlap
  //   replace(ci, ci2, this_t &&)->it // no overlap
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
  //   explicit this_t(n, alloc = {})
  //   this_t(n, x, alloc = {})
  //   assign(n, x)
  //   this_t(from, to, alloc = {})
  //   assign(from, to)
  //   this_t(il, alloc = {})
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
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   operator [](n)
  //   at(n)
  //
  //   capacity()
  //   full()
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reallocate(n = size())
  //   reserve_more(n)
  //   shrink_to_fit()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   circular_vector(from_range, r, a = {})
  //   explicit circular_vector(r)
  //   circular_vector(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(n)
  //   pop_front(n)
  //   append(s...)
  //   prepend(s...)
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
  //   explicit this_t(n, alloc = {})
  //   this_t(n, x, alloc = {})
  //   assign(n, x)
  //   this_t(from, to, alloc = {})
  //   assign(from, to)
  //   this_t(il, alloc = {})
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
  //   emplace_front(s...)
  //   push_front(x)
  //   pop_front()
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   operator [](n)
  //   at(n)
  //
  //   upper_capacity()
  //   lower_capacity()
  //   full() // if reached lower_capacity
  //   resize(n, x)
  //   resize(n)
  //   reserve(n)
  //   reserve_more(n)
  //   reallocate(n = size())
  //   shrink_to_fit()
  //   remove_if(eq)
  //   remove(x)
  //   replace(i1, i2, r)
  //
  //   deque(from_range, r, alloc = {})
  //   explicit deque(r)
  //   deque(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(n)
  //   pop_front(n)
  //   append(s...)
  //   prepend(s...)
}
void doc_forward_list() {
  // join_forward_list<size_t ID = 0, VOID_PTR = void *>
  // forward_list_node<T, AL = default_allocator<T>>
  //   : join_forward_list<0, typename allocator_traits<AL>::void_pointer>
  //   uninitialized_storage_t<sizeof(T), alignof(T)> data;
  //   T &operator *() const
  //   T *operator ->() const
  //
  // forward_list_traits<NODE_T, ID = 0,
  //                     STORE_SIZE = false, STORE_ALLOCATOR = true,
  //                     AL = default_allocator<NODE_T>>
  //   value_type
  //   allocator_type
  //   node_type
  //   node_pointer
  //   node_base_type
  //   node_base_pointer
  //   difference_type
  //   size_type
  //   id // ::value
  //   store_node_allocator //  ::value
  //   store_allocator //  ::value
  //   store_size // ::value
  //   static node_base_pointer next(node_base_pointer)
  //   static void next(node_base_pointer, node_base_pointer)
  //   static value_type *data(node_base_pointer)
  //   static node_base_pointer new_node(AL &, s...)
  //   static delete_node(AL &, node_base_pointer)
  //   header_type
  //   static node_base_pointer before_begin_node(const header_type &)
  //   static size_type size(const header_type &)
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
  //   const_iterator
  //     explicit this_t(TRAITS::node_base_pointer)
  //     node()->TRAITS::node_base_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
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
  //   new_node(s...)
  //   exchange_node_after(cit, cit)->it
  //   delete_node(cit)
  //   link_after(prev, cit)->it
  //   link_after(prev, cit, cit)->it // requires: [i, i_last] are linked
  //   link_front(cit)->it
  //   link_front(cit, cit) // requires: [i, i_last] are linked
  //   unlink_after(prev)->it
  //   unlink_after(prev, next)->it
  //   unlink()
  //   unlink_front()->it
  //   erase_or_unlink_after(prev)->it
  //
  //   node_type
  //   make_node(s...)->node_type
  //   extract_after(ci)->node_type
  //   insert_after(ci, node_type &&)->it
  //   push_front(node_type &&)
  //   exchange_after(ci, node_type &)->node_type
  //   exchange_after(ci, node_type &&)->node_type
  //   replace_after(ci, node_type &&)->it
  //
  //   extract_after(ci, ci2)
  //   insert_after(ci, this_t &&)
  //   push_front(this_t &&)
  //   exchange_after(ci, ci2, this_t &&)->this_t
  //   replace_after(ci, ci2, this_t &&)->it
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = {})
  //   this_t(n, x, alloc = {})
  //   assign(n, x)
  //   this_t(from, to, alloc)
  //   assign(from, to)
  //   this_t(il, alloc = {})
  //   operator =(il)
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
  //   splice_after(prev, l, prev)
  //   splice_after(prev, l, prev_cit1, cit2) // prev is not in (cit1, cit2)
  //   swap_after(prev, x_prev)
  //   swap_after(prev, l, x_prev)
  //   remove_if(eq)
  //   remove(x)
  //   unique(eq)
  //   unique()
  //   merge(l, less)
  //   merge(l)
  //   reverse()
  //   sort(less)
  //   sort()
  //   replace_after(prev_i1, i2, r)
  //     // return iterator to the last inserted pos or prev_i1
  //
  //   this_t(from_range, r, alloc = {})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert_after(prev, r)
  //   insert_range_after(prev, r)
  //   push_front(r)
  //   prepend_range(r)
  //   pop_front(n)
  //   prepend(s...)
}
void doc_list() {
  // join_list<size_t ID = 0, VOID_PTR = void *>
  // list_node<T, AL = default_allocator<T>>
  //   : join_list<0, typename allocator_traits<AL>::void_pointer>
  //   T &operator *() const
  //   T *operator ->() const
  //
  // list_traits<NODE_T, ID = 0,
  //             STORE_SIZE = true, STORE_ALLOCATOR = true,
  //             AL = default_allocator<NODE_T>>
  //   value_type
  //   allocator_type
  //   node_type
  //   node_pointer
  //   node_base_type
  //   node_base_pointer
  //   difference_type
  //   size_type
  //   id // ::value
  //   store_node_allocator // ::value
  //   store_allocator // ::value
  //   store_size // ::value
  //   static node_base_pointer prev(node_base_pointer)
  //   static void prev(node_base_pointer, node_base_pointer)
  //   static node_base_pointer next(node_base_pointer)
  //   static void next(node_base_pointer, node_base_pointer)
  //   static value_type *data(node_base_pointer)
  //   static node_base_pointer new_node(AL &, s...)
  //   static delete_node(AL &, node_base_pointer)
  //   header_type
  //   static node_base_pointer end_node(const header_type &)
  //   static size_type size(const header_type &)
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
  //   const_iterator
  //     explicit this_t(TRAITS::node_base_pointer)
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
  //   new_node(s...)
  //   delete_node(cit)
  //   exchange_node(cit, j)->iterator
  //   link(next, i)->iterator
  //   link(next, i, i_last)->iterator // requires: [i, i_last] are linked
  //   link_front(i)->iterator
  //   link_front(i, i_last)->iterator // requires: [i, i_last] are linked
  //   link_back(i)->iterator
  //   link_back(i, i_last)->iterator // requires: [i, i_last] are linked
  //   unlink(i)->iterator
  //   unlink(from, to)->iterator
  //   unlink()
  //   unlink_front()->iterator
  //   unlink_back()->iterator
  //   erase_or_unlink(i)->iterator
  //
  //   node_type
  //   make_node(s...)->node_type
  //   extract(ci)->node_type
  //   insert(ci, node_type &&)->it
  //   push_back(node_type &&)
  //   push_front(node_type &&)
  //   exchange(ci, node_type &)->node_type
  //   exchange(ci, node_type &&)->node_type
  //   replace(ci, node_type &&)->it
  //
  //   extract(ci, ci2)
  //   insert(ci, this_t &&)
  //   push_back(this_t &&)
  //   push_front(this_t &&)
  //   exchange(ci, ci2, this_t &)->this_t
  //   exchange(ci, ci2, this_t &&)->this_t
  //   replace(ci, ci2, this_t &&)->it
  //
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //   explicit this_t(n, alloc = {})
  //   this_t(n, x, alloc = {})
  //   assign(n, x)
  //   this_t(from, to, alloc)
  //   assign(from, to)
  //   this_t(il, alloc = {})
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
  //   remove_if(eq)
  //   remove(x)
  //   unique(eq)
  //   unique()
  //   merge(l, less)
  //   merge(l)
  //   reverse()
  //   sort(less)
  //   sort()
  //   replace(i1, i2, r)
  //
  //   this_t(from_range, r, alloc = {})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(next, r)
  //   insert_range(next, r)
  //   push_back(r)
  //   push_front(r)
  //   append_range(r)
  //   prepend_range(r)
  //   pop_back(n)
  //   pop_front(n)
  //   append(s...)
  //   prepend(s...)
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
  //   operator =(il)
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
  //   erase(i)
  //   erase(from, to)
  //   erase(key)
  //   remove(key)
  //   remove_if(eq)
  //   clear()
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)
  //   at(key)
  //   ///
  //
  //   find(key)
  //   contains(key)
  //   count(key)
  //   lower_bound(key)
  //   upper_bound(key)
  //   equal_range(key)
  //   find_range(key_min, key_max)
  //
  //   front()
  //   back()
  //
  //   merge(this_t &&)
  //   merge(r)
  //   unique(eq = ...)
  //
  //   reserve(n)
  //   capacity()
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
  //   operator =(r)
  //   assign_range(r)
  //   assign(r)
  //   void insert(r)
  //   void insert_range(r)
  //   append(s...)
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
  //   value_type
  //   allocator_type
  //   node_type
  //   node_pointer
  //   node_base_type
  //   node_base_pointer
  //   difference_type
  //   size_type
  //   id = size_constant<ID>
  //   store_size = bool_constant<STORE_SIZE>
  //   store_node_allocator = bool_constant<STORE_ALLOCATOR>
  //   store_allocator = bool_constant<STORE_ALLOCATOR>
  //
  //   static base_p left_child(base_p)
  //   static base_p right_child(base_p)
  //   static base_p parent(base_p)
  //   static left_child(base_p, base_p)
  //   static right_child(base_p, base_p)
  //   static parent(base_p, base_p)
  //
  //   static bool red(base_p) // only for rbtree
  //   static red(base_p, bool) // only for rbtree
  //
  //   static int factor(base_p) // only for avltree
  //   static factor(base_p, int) // only for avltree
  //
  //   static ptrdiff_t state(base_p) // only for ranked_rbtree
  //   static state(base_p, ptrdiff_t) // only for ranked_rbtree
  //
  //   static value_type *data(base_p)
  //
  //   // the node type is regular raw space wrapper
  //   // or is same as value_type or 
  //   static base_p new_node(AL &, s...)
  //   static delete_node(AL &, base_p)
  //
  //   header_type
  //   static base_p end_node(const header_type &)
  //   // size(...) is not for ranked_rbtree,
  //   // and is only used if store_size::value
  //   static size_type size(const header_type &)
  //   static size(header_type &, size_type)
  //
  // rbtree_adaptor<TRAITS>
  // avltree_adaptor<TRAITS>
  // ranked_rbtree_adaptor<TRAITS>
  //   traits
  //   node_pointer
  //
  //   value_type
  //   reference
  //   const_reference
  //   iterator
  //   const_iterator
  //     explicit this_t(TRAITS::node_base_pointer)
  //     node()->node_pointer
  //     to_mutable()->iterator
  //     ==(nullptr)
  //     promote()->random_access_iter // only for ranked_rbtree
  //     nth() // return rank if *this
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
  //   delete_node(cit)
  //   delete_tree(cit)
  //   exchange_node(cit, cit)->iterator // ranked_rbtree only
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
  //   unlink(cit)
  //   unlink(cit, cit)
  //   unlink()
  //   unlink(get_key, key_less, k)->size_type
  //   erase(cit)
  //   erase(cit, cit)
  //   erase(get_key, key_less, k)
  //   extract<NODE_T>(cit)->NODE_T
  //   extract<NODE_T>(getkey, keyless, k)->NODE_T
  //   clear()
  //
  //   link(cit, cit)
  //   emplace(cit, s...)
  //   insert(cit, x)
  //   insert(cit, r)
  //   insert_range(cit, r)
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
  //   insert_node_handle<ITER>(get_key, key_less, nh_rref)->...
  //   insert_node_handle(get_key, key_less, hint nh_rref)->iterator
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
  //   link_front(nd) // ranked_rbtree only
  //   emplace_front(s...) // ranked_rbtree only
  //   push_front(x) // ranked_rbtree only
  //
  //   back() // ranked_rbtree only
  //   unlink_back() // ranked_rbtree only
  //   pop_back() // ranked_rbtree only
  //   pop_back(n) // ranked_rbtree only
  //   link_back(nd) // ranked_rbtree only
  //   emplace_back(s...) // ranked_rbtree only
  //   push_back(x) // ranked_rbtree only
  //
  //   [](n) // ranked_rbtree only
  //   at(n) // ranked_rbtree only
  //
  //   explicit this_t(n, alloc = {}) // ranked_rbtree only
  //   this_t(n, x, alloc = {}) // ranked_rbtree only
  //   assign(n, x) // ranked_rbtree only
  //   insert(cit, n, x) // ranked_rbtree only
  //
  //   this_t(from, to, alloc = {}) // ranked_rbtree only
  //   assign(from, to) // ranked_rbtree only
  //   insert(cit, from, to) // ranked_rbtree only
  //
  //   this_t(il, alloc = {}) // ranked_rbtree only
  //   =(il) // ranked_rbtree only
  //   assign(il) // ranked_rbtree only
  //   insert(cit, il) // ranked_rbtree only
  //
  //   this_t(from_range, r, alloc = {}) // ranked_rbtree only
  //   explicit this_t(r) // ranked_rbtree only
  //   this_t(r, alloc) // ranked_rbtree only
  //   =(r) // ranked_rbtree only
  //   insert(cit, r) // ranked_rbtree only
  //   assign(r) // ranked_rbtree only
  //
  //   append_range(r) // ranked_rbtree only
  //   push_back(r) // ranked_rbtree only
  //   append(...) // ranked_rbtree only
  //
  //   prepend_range(r) // ranked_rbtree only
  //   push_front(r) // ranked_rbtree only
  //   prepend(...) // ranked_rbtree only
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
  //   erase_or_unlink(cit) // ranked_rbtree only
  //   remove_if(eq) // ranked_rbtree only
  //   remove(x) // ranked_rbtree only
  //
  //   unique(eq) // ranked_rbtree only
  //   unique() // ranked_rbtree only
  //
  //   merge(this_ref, less = ...) // ranked_rbtree only
  //
  //   reverse() // ranked_rbtree only
  //
  //   sort(less = ...) // ranked_rbtree only
  //
  //   replace(i1, i2, r) // ranked_rbtree only
  //
  //   node_type // ranked_rbtree only
  //   make_node(s...)->node_type // ranked_rbtree only
  //   extract(ci)->node_type // ranked_rbtree only
  //   insert(ci, node_type &&)->it // ranked_rbtree only
  //   exchange(ci, node_type &)->node_type // ranked_rbtree only
  //   exchange(ci, node_type &&)->node_type // ranked_rbtree only
  //   replace(ci, node_type &&)->it // ranked_rbtree only
  //
  //   extract(ci, ci2) // ranked_rbtree only
  //   insert(ci, this_t &&) // ranked_rbtree only
  //   exchange(ci, ci2, this_t &)->this_t // ranked_rbtree only
  //   exchange(ci, ci2, this_t &&)->this_t // ranked_rbtree only
  //   replace(ci, ci2, this_t &&)->it // ranked_rbtree only
}
void doc_map_adaptor() {
  // set_adaptor<TREE, LESS>
  // multiset_adaptor<TREE, LESS>
  // map_adaptor<TREE, LESS>
  // multimap_adaptor<TREE, LESS>
  //   traits
  //   node_pointer
  //
  //   root() // may == nullptr
  //   leftmost() // may == nullptr
  //   rightmost() // may == nullptr
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
  //     get_allocator() // requires not empty
  //     explicit operator bool()
  //     empty()
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
  //     position
  //     inserted
  //     node
  //   key_comp()
  //   value_comp()
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
  //   operator =(il)
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
  //   [](key)
  //   at(key)
  //   ///
  //
  //   erase(i)
  //   erase(from, to)
  //   erase(key)
  //   remove(key)
  //   remove_if(eq)
  //   clear()
  //
  //   merge(multi_or_non_multi_ref)
  //
  //   find(key) // get the first among equal elements
  //   contains(key)
  //   count(key)
  //   lower_bound(key)
  //   upper_bound(key)
  //   equal_range(key)
  //   partition_point(eq)
  //   find_range(min_key, max_key)
  //
  //   new_node(...)->iterator
  //   delete_node(cit)
  //
  //   unlink(cit)->iterator
  //   unlink(cit, cit)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(cit)->pair<iterator, bool> // unique only
  //   link(cit)->iterator // multi only
  //   link(hint, cit)->iterator
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
  //   front()
  //   back()
  //
  //   nth(n) // ranked_rbtree only
  //   nth(cit) // ranked_rbtree only
  //
  //   erase_or_unlink(cit)
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
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   void insert(r)
  //   void insert_range(r)
  //   append(s...)
}
void doc_map_typedef() {
  // (nosz_)rbtt<T, AL>
  // (nosz_)rbsett<T, AL>
  //   key(x)
  // (nosz_)rbmapt<T, AL>
  //   key(x)
  //   mapped(x)
  // (nosz_)rbtree<T, AL = ...>
  // (nosz_)rb_(multi)set<T, LESS = ..., AL = ...>
  // (nosz_)rb_(multi)map<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // (nosz_)avltt<T, AL>
  // (nosz_)avlsett<T, AL>
  // (nosz_)avlmapt<T, AL>
  // (nosz_)avltree<T, AL = ...>
  // (nosz_)avl_(multi)set<T, LESS = ..., AL = ...>
  // (nosz_)avl_(multi)map<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // (nosz_)(multi)set<T, LESS = ..., AL = ...>
  // (nosz_)(multi)map<KEY, MAPPED, LESS = ..., AL = ...>
  //
  // rrbtt<T, AL>
  // rsett<T, AL>
  // rmapt<T, AL>
  // ranked_rbtree<T, AL = ...>
  // ranked_(multi)set)<T, LESS = ..., AL = ...>
  // ranked_(multi)map<KEY, MAPPED, LESS = ..., AL = ...>
}
void doc_hashtable_adaptor() {
  // htb_node_base<ID = 0, VOID_PTR = void *>
  //   next
  // join_hashtable<ID = 0, VOID_PTR = void *> : htb_node_base<ID, VOID_PTR>
  //   prev
  // hashtable_node<T, VOID_PTR = void *> : join_hashtable<T, VOID_PTR>
  //   data
  //   * ->
  //
  // hashtable_traits<NODE_T, ID = 0, STORE_NODE_ALLOCATOR = true,
  //                  AL = default_allocator<NODE_T>>
  //   value_type
  //   allocator_type
  //   node_type
  //   node_pointer
  //   node_base_type
  //   node_base_pointer
  //   difference_type
  //   size_type
  //
  //   id = size_constant<ID>
  //   store_node_allocator = bool_constant<STORE_NODE_ALLOCATOR>
  //   store_allocator = true_type
  //
  //   static base_p next(base_p)
  //   static void next(base_p, base_p)
  //
  //   static base_p prev(base_p)
  //   static void prev(base_p, base_p)
  //
  //   static value_type *data(base_p)
  //
  //   header_type
  //
  //   static base_p end_node(const header_type &)
  //
  //   static base_p placeholder_node(const header_type &)
  //
  //   static size_type size(const header_type &)
  //   static void size(const header_type &, size_type)
  //
  //   static float factor(const header_type &)
  //   static void factor(const header_type &, float)
  //
  //   static base_p new_node(alloc, s...)
  //   static void delete_node(alloc, base_p)
  //
  // hashtable_adaptor<TRAITS>
  //   traits
  //   node_pointer
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
  //   delete_node(cit)
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
  //   begin(n)
  //   end(n)
  //   cbegin(n)
  //   cend(n)
  //
  //   bucket_count()
  //   active_bucket_count()
  //   max_bucket_count()
  //   bucket_size(n)
  //   bucket(hash, key)
  //   bucket_range(n)->iter_pair<(const_)local_iterator>
  //
  //   load_factor()
  //   max_load_factor()
  //   max_load_factor(f)
  //
  //   find(get_key, hash, eq, key)->iterator
  //   equal_range(get_key, hash, eq, key)->iter_pair<iterator>
  //   count(get_key, hash, eq, key)->size_type
  //
  //   unlink(cit)->iterator
  //   unlink(cit, cit)->iterator
  //   unlink()
  //   unlink(get_key, hash, eq, key)
  //   erase(cit)
  //   erase(cit, cit)
  //   erase(get_key, hash, eq, key)
  //   extract<NODE_HANDLE>(cit)
  //   extract<NODE_HANDLE>(get_key, hash, eq, key)
  //   clear()
  //
  //   auto_rehash(get_key, hash, eq, current_size)
  //   auto_rehash(get_key, hash, eq)
  //   rehash(get_key, hash, eq, n)
  //   reserve(get_key, hash, eq, n)
  //
  //   insert(get_key, hash, eq, key, get_new_node, do_when_equal)
  //   insert_equal(get_key, hash, eq, key, get_new_node)
  //
  //   link(get_key, hash, eq, it)
  //   emplace(get_key, hash, eq, s...)
  //   insert(get_key, hash, eq, const value_type &)
  //   insert(get_key, hash, eq, value_type &&)
  //
  //   try_emplace(get_key, hash, eq, key, s...)
  //   insert_or_assign(get_key, get_mapped, hash, eq, key, x)
  //   insert_node_handle<ITER>(get_key, hash, eq, nh)
  //
  //   link_equal(get_key, hash, eq, it)
  //   emplace_equal(get_key, hash, eq, s...)
  //   insert_equal(get_key, hash, eq, x)
  //   insert_node_handle_equal(get_key, hash, eq, nh)
  //
  //   link_equal(get_key, hash, eq, hint, it)
  //   emplace_equal_hint(get_key, hash, eq, hint, s...)
  //   insert_equal(get_key, hash, eq, hint, x)
  //   insert_node_handle_equal(get_key, hash, eq, hint, nh)
  //
  //   merge(get_key, hash, eq, analogous)
  //   merge_equal(get_key, hash, eq, analogous)
  //
  //   insert_range_transparent(get_key, hash, eq, r) // only for unordered_set
  //   insert_range(get_key, hash, eq, r)
  //   insert_range(get_key, hash, eq, r, no_rehash_ins)
  //
  //   assign_range_transparent(get_key, hash, eq, r) // only for unordered_set
  //   assign_range(get_key, hash, eq, r)
  //   assign_range(get_key, hash, eq, r, no_rehash_ins)
  //
  //   insert_range_equal(get_key, hash, eq, r)
  //   assign_range_equal(get_key, hash, eq, r)
  //
  //   equal(get_key, hash, eq, hash2, eq2, x)
  //   equal_equal(get_key, hash, eq, hash2, eq2, x)
  //
  //   static constexpr default_bucket_count = 20;
}
void doc_unordered_map_adaptor() {
  // unordered_set_adaptor<TABLE, HASH, EQ>
  // unordered_multiset_adaptor<TABLE, HASH, EQ>
  // unordered_map_adaptor<TABLE, HASH, EQ>
  // unordered_multimap_adaptor<TABLE, HASH, EQ>
  //   traits
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
  //   key_type
  //   mapped_type // only for map/multimap
  //   hasher
  //   key_equal
  //   node_type
  //     get_allocator()
  //     explicit operator bool()
  //     empty()
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
  //     position
  //     inserted
  //     node
  //   hash_function()
  //   key_eq()
  //
  //   this_t(hf, eq, alloc)
  //   this_t(hf, eq)
  //   explicit this_t(n, hf = {}, eq = {}, alloc = {})
  //   this_t(n, alloc)
  //   this_t(n, hf, alloc)
  //
  //   this_t(from, to, n = ..., hf = {}, eql = {}, alloc = {})
  //   this_t(from, to, alloc)
  //   this_t(from, to, n, alloc)
  //   this_t(from, to, n, hf, alloc)
  //   assign(from, to)
  //
  //   this_t(il, n = ..., hf = {}, equl = {}, alloc = {})
  //   this_t(il, alloc)
  //   this_t(il, n, alloc)
  //   this_t(il, n, hf, alloc)
  //   operator =(il)
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
  //   [](key)
  //   at(key)
  //   ///
  //
  //   erase(i)
  //   erase(from, to)
  //   erase(key)
  //   remove(key)
  //   remove_if(eq)
  //   clear()
  //
  //   merge(multi_or_non_multi_version)
  //
  //   find(key) // for multi-version get the last among equal elements
  //   contains(key)
  //   count(key)
  //   equal_range(key)
  //
  //   bucket_count()
  //   active_bucket_count()
  //   max_bucket_count()
  //   bucket_size(n)
  //   bucket(key)
  //
  //   local_iterator
  //   const_local_iterator
  //   begin(n)
  //   end(n)
  //   cbegin(n)
  //   cend(n)
  //   bucket_range(n)->iter_pair<(const_)local_iterator>
  //
  //   load_factor()
  //   max_load_factor()
  //   max_load_factor(f)
  //   rehash(n)
  //   reserve(n)
  //
  //   new_node(...)->iterator
  //   delete_node(cit)
  //
  //   unlink(cit)->iterator
  //   unlink(cit, cit)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(cit)->pair<iterator, bool> // unique only
  //   link(cit)->iterator // multi only
  //   link(hint, cit)->iterator
  //
  //   try_link(key, get_node, do_when_eq = ...)->pair
  //     // unique only
  //   try_link_hint(hint, key, get_node, do_when_eq = ...)->iterator
  //     // unique only
  //   try_link(key, get_node)->iterator
  //     // multi only
  //   try_link_hint(hint, key, get_node)->iterator
  //     // multi only
  //
  //   erase_or_unlink(cit)
  //
  //   unique(eq = ...)
  //
  //   this_t(from_range, r)
  //   this_t(from_range, r, n, hf = {}, eq = {}, alloc = {})
  //   this_t(from_range, r, alloc)
  //   this_t(from_range, r, n, alloc)
  //   this_t(from_range, r, n, hf, alloc)
  //   explicit this_t(r)
  //   this_t(r, n, hf = {}, eq = {}, alloc = {})
  //   this_t(r, alloc)
  //   this_t(r, n, alloc)
  //   this_t(r, n, hf, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   void insert(r)
  //   void insert_range(r)
  //   append(s...)
}
void doc_unordered_map_typedef() {
  // htbt<T, AL>
  // hsett<T, AL>
  // hmapt<T, AL>
  // hashtable<T, AL>
  // unordered_set<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //               AL = default_allocator<KEY>>
  // unordered_multiset<KEY, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                    AL = default_allocator<KEY>>
  // unordered_map<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //               AL = default_allocator<pair<KEY, MAPPED>>>
  // unordered_multimap<KEY, MAPPED, HASH = hash<KEY>, EQ = equal_to<KEY>,
  //                    AL = default_allocator<pair<KEY, MAPPED>>>
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
  //   traits
  //   node_pointer
  //   tree_type
  //   unordered_type
  //   tree()
  //   unordered()
  //
  //   pointer
  //   const_pointer
  //   value_type
  //   reference
  //   const_reference
  //   iterator = typename tree_type::iterator;
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
  //   allocator_type
  //   get_allocator()
  //   this_t(alloc)
  //   this_t(const this_t &, alloc)
  //   this_t(this_t &&, alloc)
  //
  //   key_type
  //   mapped_type // only for map and multimap
  //   key_compare
  //   value_compare
  //   key_comp
  //   value_comp
  //
  //   this_t(hf, eq, cmp, alloc = {})
  //
  //   this_t(first, last, hf, eq, cmp, alloc = {})
  //   this_t(first, last, alloc = {})
  //   assign(first, last)
  //
  //   this_t(il, hf, eq, cmp, alloc = {})
  //   this_t(il, alloc = {})
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
  //     position
  //     inserted
  //     node
  //   extract(cit)->node_type
  //   extract(key)->node_type
  //   insert(nh)->insert_return_type
  //   insert(hint, nh)
  //
  //   // only for map
  //   try_emplace(key, s...)->pair<iterator, bool>
  //   try_emplace(hint, key, s...)->iterator
  //   insert_or_assign(key, x)->pair<iterator, bool>
  //   insert_or_assign(hint, key, x)->iterator
  //   [](key)
  //   at(key)
  //   ///
  //
  //   erase(cit)
  //   erase(cit, cit)
  //   clear()
  //
  //   erase(key)
  //   remove(key)
  //   remove_if(eql)
  //
  //   merge(multi_or_non_multi_version)
  //
  //   find(key)
  //   contains(key)
  //   count(key)
  //   lower_bound(key)
  //   upper_bound(key)
  //   equal_range(key)
  //   partition_point(eql)
  //   find_range(min, max)
  //
  //   new_node(s...)
  //   delete_node(cit)
  //
  //   unlink(cit)->iterator
  //   unlink(cit, cit)->iterator
  //   unlink()
  //   unlink_key(key)->size_type
  //   link(cit)->pair // unique only
  //   link(cit)->iterator // multi only
  //   link(hint, cit)->iterator
  //
  //   try_link(key, get_node, do_when_eq = ...)->pair<iterator, bool>
  //     // unique only
  //   try_link_hint(hint, key, get_node,
  //                 do_when_eq = ...)->iterator
  //     // unique only
  //   try_link(key, get_node)->iterator
  //     // multi only
  //   try_link_hint(hint, key, get_node)->iterator
  //     // multi only
  //
  //   front()
  //   back()
  //
  //   root()
  //   leftmost()
  //   rightmost()
  //
  //   nth(n) // ranked_rb_tree only
  //   nth(cit) // ranked_rb_tree only
  //
  //   erase_or_unlink(cit)->iterator
  //
  //   unique()
  //   unique(eql)
  //
  //   hasher
  //   key_equal
  //   hash_function()
  //   key_eq()
  //   bucket_count()
  //   max_bucket_count()
  //   active_bucket_count()
  //   bucket_size(n)
  //   bucket_range(n)
  //   bucket(key)
  //   load_factor()
  //   max_load_factor()
  //   max_load_factor(f)
  //   rehash(n)
  //   reserve(n)
  //   local_iterator
  //   const_local_iterator
  //   begin(n)
  //   end(n)
  //   cbegin(n)
  //   cend(n)
  //
  //   this_t(from_range, r, alloc = {})
  //   this_t(from_range, r, hf, eq, cmp, alloc = {})
  //   explicit this_t(r)
  //   this_t(r, hf, eq, cmp, alloc = {})
  //   this_t(r, alloc)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(r)
  //   insert_range(r)
  //   append(s...)
}
void doc_mixed_map_typedef() {
  // default_set_traits<TRAITS>
  // default_map_traits<TRAITS>
}
void doc_stable_vector_adaptor() {
  // join_stable_vector<ID, VOID_PTR = void *>
  // stable_vector_node<T, AL>
  // stable_vector_traits<NODE_T, ID = 0,
  //                      STORE_NODE_ALLOCATOR = false,
  //                      AL = default_allocator<NODE_T>>
  //   value_type
  //   allocator_type
  //   node_type
  //   node_pointer
  //   node_base_type
  //   node_base_pointer
  //   difference_type
  //   size_type
  //
  //   id // ::value
  //   store_node_allocator // :: value
  //   store_allocator // :: value
  //
  //   node_base_pointer_pointer
  //   index_iter(node_base_pointer)->node_base_pointer_pointer
  //   index_iter(node_base_pointer, node_base_pointer_pointer)
  //
  //   data(p)
  //   new_node(alloc, s...)
  //   delete_node(alloc, p)
  //
  // stable_vector_adaptor<TRAITS, LIM = 0>
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
  //   traits
  //   node_pointer
  //   new_node(s...)->iterator
  //   delete_node(cit)
  //   exchange_node(cit, cit)->iterator
  //   link(cit, cit)->iterator
  //   link_back(cit)->iterator
  //   unlink(cit)->iterator
  //   unlink(cit, cit)->iterator
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
  //   explicit this_t(n, alloc = {})
  //   this_t(n, x, alloc = {})
  //   assign(n, x)
  //   this_t(from, to, alloc = {})
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
  //   back()
  //   emplace_back(s...)
  //   push_back(x)
  //   pop_back()
  //   [](n)
  //   at(n)
  //
  //   capacity()
  //   resize(n)
  //   resize(n, x)
  //   reserve(n) // not for limited_stable_vector
  //   reserve_more(n) // not for limited_stable_vector
  //   shrink_to_fit() // not for limited_stable_vector
  //   splice(cit, this_ref)
  //   splice(cit, this_ref, cit)
  //   splice(cit, this_ref, cit, cit)
  //   swap(cit, cit)
  //   swap(cit, this_ref, cit)
  //   erase_or_unlink(cit)
  //
  //   remove_if(eq)
  //   remove(x)
  //   unique(eq)
  //   unique()
  //   merge(analoguous_ref, less = ...)
  //   reverse()
  //   sort(less = ...)
  //
  //   full()
  //   reallocate(n = size()) // not for limited_stable_vector
  //   reserve_more(n) // not for limited_stable_vector
  //   replace(i1, i2, r)->iterator
  //
  //   node_type
  //   make_node(s...)->node_type
  //   extract(ci)->node_type
  //   insert(ci, node_type &&)->it
  //   exchange(ci, node_type &)->node_type
  //   exchange(ci, node_type &&)->node_type
  //   replace(ci, node_type &&)->it
  //
  //   extract(ci, ci2)
  //   insert(ci, this_t &&)
  //   exchange(ci, ci2, this_t &)->this_t
  //   exchange(ci, ci2, this_t &&)->this_t
  //   replace(ci, ci2, this_t &&)->it
  //
  //   this_t(from_range, r, alloc = {})
  //   explicit this_t(r)
  //   this_t(r, alloc)
  //   operator =(r)
  //   assign(r)
  //   assign_range(r)
  //   insert(cit, r)
  //   insert_range(cit, r)
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //   append(s...)
}
void doc_stable_vector_typedef() {
  // stbvt<T, AL>
  // stable_vector<T, AL>
  // limited_stable_vector<T, N, AL>
}
void doc_tree_adaptor() {
  // tree_adaptor<TRAITS>
  //   tree_type
  //   vector_type
  //
  //   key_type
  //   children_type
  //   tree_node_type
  //   value_type
  //     ... // range operations
  //     key_type
  //     nth(n)
  //     key()
  //     parent()
  //     iter()
  //   reference
  //   const_reference
  //
  //   iterator
  //   const_iterator
  //     first_order_iterator
  //     last_order_iterator
  //     nth_order_iterator
  //       // note: reverse sequence of last order is first order of tree with
  //       //   reverse children for all nodes
  //       // bidirectional
  //       // prev(begin) == end
  //       // begin == next(end)
  //       root()
  //       base()
  //       nth() // only for nth_order_iterator
  //       removed_next()
  //         // *this is dereferenceable, only for first_order_iterator
  //       advance()->difference_type // return depth
  //     first_order(_begin/end)()
  //     last_order(_begin/end)()
  //     nth_order(_begin/end)()
  //   difference_type
  //   size_type
  //
  //   root() // get null if empty
  //   croot() // get null if empty
  //
  //   empty()
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
  //   explicit tree_adaptor(const tree_adaptor<T2> &, a = {})
  //   =(const tree_adaptor<T2> &)
  //
  //   explicit tree_adaptor(tree_adaptor<T2> &&, a = {})
  //   =(tree_adaptor<T2> &&)
  //
  //   explicit tree_adaptor(in_place_t, s...)
  //   explicit tree_adaptor(in_place_type_t<U>, s...)
  //   explicit tree_adaptor(const key_type &, this_rrefs...)
  //     // all trees have equal allocators
  //   explicit tree_adaptor(key_type &&, this_rrefs...)
  //     // all trees have equal allocators
  //   explicit tree_adaptor(allocator_arg, a, in_place_t, s...)
  //   explicit tree_adaptor(allocator_arg, a, in_place_type_t<U>, s...)
  //   explicit tree_adaptor(allocator_arg, a, const key_type &, s...)
  //   explicit tree_adaptor(allocator_arg, a, key_type &&, s...)
  //   maker()
  //   maker_with_allocator(a)
  //
  //   copy(it, get_key = deref)->this_t // it belongs to *this
  //   copy(tree/vec_ref, it, get_key)->this_t
  //   emplace(s...)
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
  //   emplace_front(i, s...)
  //   push_front(i, k)
  //   push_front(i, tree_ref)
  //   push_front(i, vector_ref)
  //   push_front(i, kr)
  //   prepend_range(i, r)
  //   prepend(i, s...)
  //   pop_front(i)
  //   pop_front(i, n)
  //
  //   capacity(i)
  //   full(i)
  //   reserve(i, n)
  //   first_order_reserve(i, n);
  //   reserve_more(i, n)
  //   first_order_reserve_more(i, n);
  //
  //   resize(i, n, x = ...)
  //   first_order_resize(i, n, x = ...)
  //
  //   shrink_to_fit(i)
  //   first_order_shrink_to_fit(i)
  //
  //   remove_if(i, eq)
  //   first_order_remove_if(i, eq)
  //
  //   unique(i, eq)
  //   first_order_unique(i, eq)
  //
  //   merge(i, tree/vec_ref, i2, less)
  //   merge(i, vec_ref, less)
  //
  //   sort(i, iter_less)
  //   first_order_sort(i, less)
  //
  // private:
  //   good()->bool
}
void doc_tree_vector_adaptor() {
  // tree_vector_adaptor<TRAITS>
  //   tree_type
  //   vector_type
  //
  //   key_type
  //   children_type
  //   tree_node_type
  //   value_type
  //     ... // range operations
  //     key_type
  //     nth(n)
  //     key()
  //     parent()
  //     iter()
  //   reference
  //   const_reference
  //
  //   iterator
  //   const_iterator
  //     first_order_iterator
  //     last_order_iterator
  //     nth_order_iterator
  //       // bidirectional
  //       // prev(begin) == end
  //       // begin == next(end)
  //       root()
  //       base()
  //       nth() // only for nth_order_iterator
  //       removed_next()
  //         // *this is dereferenceable, only for first_order_iterator
  //     first_order(_begin/end)()
  //     last_order(_begin/end)()
  //     nth_order(_begin/end)()
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
  //   explicit tree_vector_adaptor(const tree_vector_adaptor<T2> &, a = {})
  //   =(const tree_vector_adaptor<T2> &)
  //
  //   explicit tree_vector_adaptor(tree_vector_adaptor<T2> &&, a = {})
  //   =(tree_vector_adaptor<T2> &&)
  //
  //   tree_vector_adaptor(n, a = {})
  //   tree_vector_adaptor(n, k, a = {})
  //   assign(n, k)
  //
  //   tree_vector_adaptor(from, to, a = {})
  //   assign(from, to)
  //
  //   tree_vector_adaptor(il, a = {})
  //   =(il)
  //   assign(il)
  //
  //   tree_vector_adaptor(r, a = {})
  //   assign(r)
  //   tree_vector_adaptor(from_range, r, a = {})
  //   assign_range(r)
  //
  //   explicit tree_vector_adaptor(tree_types...)
  //   explicit tree_vector_adaptor(allocator_arg, a, tree_types...)
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
  //   first_order_reserve(n)
  //   first_order_reserve(i, n)
  //   reserve_more(n)
  //   reserve_more(i, n)
  //   first_order_reserve_more(n);
  //   first_order_reserve_more(i, n);
  //
  //   resize(n, x = ...)
  //   resize(i, n, x = ...)
  //   first_order_resize(n, x = ...)
  //   first_order_resize(i, n, x = ...)
  //
  //   shrink_to_fit()
  //   shrink_to_fit(i)
  //   first_order_shrink_to_fit()
  //   first_order_shrink_to_fit(i)
  //
  //   remove_if(eq)
  //   remove_if(i, eq)
  //   first_order_remove_if(eq)
  //   first_order_remove_if(i, eq)
  //
  //   unique(eq)
  //   unique(i, eq)
  //   first_order_unique(eq)
  //   first_order_unique(i, eq)
  //
  //   merge(tree/vec_ref, i2, less)
  //   merge(i, tree/vec_ref, i2, less)
  //   merge(vec_ref, less)
  //   merge(i, vec_ref, less)
  //
  //   sort(less)
  //   sort(i, less)
  //   first_order_sort(less)
  //   first_order_sort(i, less)
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

  // store_this_iter
  //   static apply(IT)
  //
  // dynamic_tree<T, F = store_this_iter>
  // dynamic_tree_vector<T, F = store_this_iter>
  // linked_dynamic_tree<T, F = store_this_iter>
  // linked_dynamic_tree_vector<T, F = store_this_iter>
  // logn_dynamic_tree<T, F = store_this_iter>
  // logn_dynamic_tree_vector<T, F = store_this_iter>
}
void doc_matrix() {
  // matrix<T, C = ...>
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
  //   empty() // width() == 0 || height() == 0
  //
  //   width()->int
  //   height()->int
  //
  //   smf: full
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
  //   row(n)
  //   rows()
  //   column(n)
  //   columns()
  //   sub_range(x, y, w, h) // overflowed part are removed
  //   fill(x, y, w, h, value)
  //     // rectangle of (x, y)-(w, h) belong to *this or empty rectangle
  //   fill(value)
  //
  //   left_top()
  //   left_bottom()
  //   right_top()
  //   right_bottom()
  //   ()(x, y)
  //   iter(x, y)
  //   includes_point(x, y)
  //
  //   capacity()
  //   reserve(size_t)
  //   reserve_more(size_t)
  //   shrink_to_fit()
  //
  //   swap(it, it2)
  //   swap(it, m, it2)
  //   replace(it, it2, m_rv)
  //   exchange(it, it2, m_rv)->matrix // returned matriex has 1 height
  //
  //   +()
  //   -()
  //   +(m)
  //   +=(m)
  //   -(m)
  //   -=(m)
  //   *(k)
  //   *=(k)
  //   friend *(k, const this_t &)
  //   /(k)
  //   /=(k)
  //
  //   *(m)
}
void doc_dup_compressed_array() {
  // dup_compressed_array<T, AL>
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
  //     smf: full
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
  //   smf: full
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
  //   explicit dup_compressed_array(a)
  //   dup_compressed_array(const this_t &, a)
  //   dup_compressed_array(this_t &&, a)
  //
  //   explicit dup_compressed_array(n, a = {})
  //   dup_compressed_array(n, value_type, a = {})
  //   assign(n, value_type)
  //
  //   dup_compressed_array(from, to, a = {})
  //   assign(from, to)
  //
  //   dup_compressed_array(il, a = {})
  //   =(il)
  //   assign(il)
  //
  //   insert(iterator, value_type)
  //   insert(iterator, n, value_type)
  //   insert(iterator, from, to)
  //   insert(iterator, il)
  //   erase(iterator)
  //   erase(iterator, iterator)
  //   clear()
  //
  //   front()->value_type
  //   back()->value_type
  //   push_back(value_type)
  //   pop_back()
  //
  //   compressed_size() // return inner size of the inner vector
  //   capacity()
  //   full()
  //   reserve(n)
  //   reserve_more(n)
  //   shrink_to_fit()
  //   reallocate(n = 0)
  //   replace(iterator, iterator, r)
  //
  //   dup_compressed_array(from_range, r, a = {})
  //   explicit dup_compressed_array(r)
  //   dup_compressed_array(r, a)
  //   =(r)
  //   assign(r)
  //   assign_range(r)
  //
  //   insert(iterator, r)
  //   insert_range(iterator, r)
  //
  //   push_back(r)
  //   append_range(r)
  //   pop_back(n)
  //
  //   append(s...)
}

void doc_duration() {
  // treat_as_floating_point<rep> : false_type
  // treat_as_floating_point<float or double> : true_type
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
  //   duration(const duration<R2, P2> &) requires semi-safe
  //
  //   + // unary or binary, always return duration<...>
  //   - // ...
  //   ++
  //   --
  //   +=
  //   -=
  //
  //   *=(rep)
  //   /=(rep)
  //   %=(rep)->this_t
  //   %=(const this_t &)->this_t
  //
  //   * // with rep
  //   / // with rep (->common_duration) or duration<R2, P2> (->common_rep)
  //   % // with rep or duration<R2, P2> // both return common_duration
  //
  //   static zero()
  //   static min()
  //   static max()
  //
  //   flour<rep2>()
  //   ceil<rep2>()
  //   round<rep2>()
  //   abs()
  // duration_cast<d2_t>(d)
  // duration_floor<d2_t>(d)
  // duration_ceil<d2_t>(d)
  // duration_round<d2_t>(d)
  // abs(d)
  //
  // nanoseconds = ...
  // microseconds
  // milliseconds = duration<long long, milli>;
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
  // ""_[h/min/s/ms/us/ns](f) // for floating-point version
}
void doc_time_point() {
  // time_point<C, D = C::duration>
  //   clock = C
  //   duration = D
  //   rep = duration::rep
  //   period = duration::period
  //
  //   smf: full
  //
  //   explicit time_point(d)
  //   time_point(tp2) requires is_convertible_v<tp2_t, duration>
  //
  //   time_since_epoch()
  //   time()
  //   count()
  //
  //   ++ -- += -=
  //   + with duration<...> return common_time_point
  //   - with duration<...> return common_time_point
  //   - with time_point_2 return common_duration
  //
  //   static min()
  //   static max()
  //
  //   flour<rep2>()
  //   ceil<rep2>()
  //   round<rep2>()
  //
  // time_point_cast<d2_t>(t)
  // time_point_floor<d2_t>(t)
  // time_point_ceil<d2_t>(t)
  // time_point_round<d2_t>(t)
}
void doc_clock() {
  // scoped_timer<CLOCK>
  //   smf: destructible
  //   explicit scoped_timer(double &)
  //
  // system_clock
  // steady_clock
  //   rep = long long
  //   period = ratio(...)
  //   duration = duration<rep, period>
  //   time_point = time_point<this_t>
  //   is_steady->bool
  //
  //   using timer = scoped_timer<this_t>
  //
  //   static now()->time_point
  //
  // wait_for(duration)
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
  //   c_file(name, mode = "a+b")
  //   open(name, mode = "a+b")
  //   empty()
  //   close()
  //   base()->FILE *
  //   release()->FILE *
  //   error()->bool
  //
  //   getc()->int
  //   putc(int) // no flush()
  //   putc_iter() // no flush()
  //   flush()
  //
  //   get<STR = string>()->STR
  //   put(const char *) // auto call flush()
  //   put(char_range) // auto call flush()
  //
  //   putwc(wint_t) // no flush()
  //   putws(const wchar_t *)
  //   putws(wchar_range)
  //
  // file_exists(const char *)
  //
  // console_c_file
  //   special member functions: destructible and move constructible
  //
  //   base()->FILE *
  //   error()->bool
  //
  //   getc()->int
  //   putc(int)
  //   putc_iter()
  //   flush()
  //
  //   get<STR = string>()->STR
  //   put(const char *)
  //   put(char_range)
  //
  // stdin_f()->console_c_file
  // stdout_f()->console_c_file
  // stderr_f()->console_c_file
}
void doc_print_tag() {
  // print_tag_left
  // print_tag_right
  // print_tag_min_width
  //   value
  // print_tag_fill
  //   value
  // print_tag_showpos
  // print_tag_noshowpos
  // print_tag_dec
  // print_tag_hex
  // print_tag_oct
  // print_tag_bin
  // print_tag_separator
  //   value
  // print_tag_uppercase
  // print_tag_nouppercase
  // print_tag_fixed
  // print_tag_scientific
  // print_tag_precision
  //   value
  //
  // left
  // right
  // setw(w)
  // setfill(c)
  // showpos
  // noshowpos
  // dec
  // hex
  // oct
  // bin
  // setseparator(n)
  // uppercase
  // nouppercase
  // fixed
  // scientific
  // setprecision(n) // n == -1 means show all numbers
}
void doc_sscan() {
  // sscan(sv_ref, sv)->bool
  // sscan(sv_ref, bool &)->bool // non-zero integer means true
  // sscan(sv_ref, int_ref)->bool
  // sscan(sv_ref, int_ref, dec)->bool
  // sscan(sv_ref, int_ref, bin)->bool
  // sscan(sv_ref, int_ref, oct)->bool
  // sscan(sv_ref, int_ref, hex)->bool
  // sscan(sv_ref, float_ref)->bool
  //
  // sscan_single(sv_ref, char &)->bool
  //
  // sscan_while(sv_ref, f)->sview
}
void doc_sprint() {
  // print_args
  //   size_t min_width
  //   bool left_padding
  //   char padding_char
  //
  //   left()
  //   right()
  //   setw(n)
  //   setfill(c)
  //
  // int_print_args
  //   size_t radix
  //   const char *numbers
  //   bool upper_case
  //   bool show_positive_symbol
  //   size_t separator_n
  //
  //   left()
  //   right()
  //   setw(n)
  //   setfill(c)
  //
  //   dec()
  //   bin()
  //   oct()
  //   hex()
  //   uppercase()
  //   nouppercase()
  //   showpos()
  //   noshowpos()
  //   setseparator(n)
  //
  // float_print_args
  //   const char *numbers
  //   bool upper_case
  //   bool show_positive_symbol
  //   size_t separator_n
  //   bool scientific_notation
  //   size_t precision
  //
  //   left()
  //   right()
  //   setw(n)
  //   setfill(c)
  //
  //   uppercase()
  //   nouppercase()
  //   showpos()
  //   noshowpos()
  //   setseparator(n)
  //   scientific()
  //   fixed()
  //   setprecision(n)
  //
  // sprint(s, sv, ...)
  // sprint(s, integral, ...)
  // sprint(s, floating_point, ...)
  //
  // to_string(s...)->string // call sprint
  //
  // put(...)
  // putln(...)
  // putws(...)
  // putwsln(...)
}
void doc_big_int() {
  // big_int
  //   special member functions: full
  //   ==
  //   <=> with this_t or int64_t
  //
  //   explicit big_int(int64_t)
  //   =(int64_t)
  //   to_int64()->optional<int64_t>
  //
  //   explicit big_int(int32_t)
  //   =(int32_t)
  //   to_int32()->optional<int32_t>
  //
  //   is_non_negative()
  //   neg()
  //
  //   is_zero()->bool
  //
  //   container_type
  //   const container_type &data() const &
  //   container_type data() &&
  //   data(container_type &&)
  //
  //   mul_pow_of_2(int32_t)->this_ref
  //   mul_pow_of_10(int32_t)->this_ref
  //   mul(uint32_t)->this_ref
  //   div(uint32_t)->uint32_t // return remainder
  //
  //   +=(const this_t &)
  //   -=(const this_t &)
  //   +(const this_t &)->this_t
  //   -(const this_t &)->this_t
  //
  //   sscan(sv_ref)->bool
  //   sprint(sep_n = 0)->string
  //
  //   sscan_hex(sv_ref)->bool
  //   sprint_hex(sep_n = 0)->string
  //
  //   sscan_scientific(sv_ref)->bool
  //   sprint_scientific(precision = 6u, sep_n = 0, upper_e = false,
  //                     show_pos = false)->string
}
void doc_ssplitter() {
  // ssplitter<STR = sview>
  //   special member functions: full
  //   ssplitter(sv, sv_delimiter)
  //   ssplitter(sv, char_delimiter)
  //
  //   begin()
  //   end()
  //   empty()
  //   size()
  //   clear()
  //   ()(sv, oi)->oi
  //   ()(iter, oi)->pair<iter, oi> // need enough elements from iter
  //   ()(r, oi)->pair<iter, oi>
  //     // if empty(r), do nothing,
  //     // otherwise
  //     //   if r has no enough elements, repeatedly use the last one,
  //     //   otherwise
  //     //     same as operator ()(begin(r), oi)
  //
  // split(s, delimiter)

  ssplitter s("ab_cd_efg_h", "_"); // equal(s, seq("ab"_sv, "cd", "efg", "h"))
  ssplitter s2("ab_cd_efg_h", '_'); // same as above
  ssplitter s3("_cd_efg_h", "_"); // {"", "cd", "efg"}
  ssplitter s4("_", "_"); // {"", ""}
  ssplitter s5("", "_"); // {}
  ssplitter s6("", ""); // {}
  ssplitter s7("abc", ""); // {"abc"}
  ssplitter s8("", "abc"); // {}
  assert(s("||", to_back(string{})).base() == "ab||cd||efg||h");
  assert(s(seq("$"_sv, "%%"), to_back(string{})).second.base()
         == "ab$cd%%efg%%h");
  const vector<string> v = {"$", "%%", "^^^", "&&&&"};
  assert(s(v.begin(), to_back(string{})).second.base() == "ab$cd%%efg^^^h");

  vector<string> vv;
  ssplit("ab__cd__efg"_sv, "__",
         output_itr([&](auto sv) {vv.emplace_back(sv);}));
  assert(equal(vv, seq("ab"_sv, "cd", "efg")));
}
void doc_file() {
  // read_file_t read_file
  // open_file_t open_file
  // create_file_t create_file
  // file
  //   special member functions: only movable
  //   explicit file(sv) // sview, wsview, u16sview or u32sview
  //   explicit file(sv, read_file)
  //   explicit file(sv, open_file)
  //   explicit file(sv, create_file)
  //   empty()->bool
  //   open(sv)
  //   close()
  //   read<S = string>()->S
  //   write(contiguous_r)
  //
  // to_full_path(sv)->s
  // simplify_path(sv)->s
  // remove_path_last_name(sv)->s // unchange if no last name // .../###/
  // replace_path_last_name(sv)->s // unchange if no last name
  //
  // is_file(sv)->bool // is file or directory
  // is_directory(sv)->bool
  // try_create_directory(sv)->bool
  // create_directory(sv)
  // file_size(sv)->ull
  // file_time(sv)->system_clock::time_point
  // try_remove_file(sv)->bool
  // remove_file(sv)
  // try_rename_file(sv, sv2)->bool // sv is full path, sv2 is just name
  // rename_file(sv, sv2) // same as try_...
  //
  // file_info<S = string>
  //   string_type
  //   path
  //   name
  //   size
  //   time
  //   is_dir
  // view_file(sv)->file_info<...> // non recursive
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
  //     // if not equal, assign stored value to expected,
  //     // otherwise, assign stored value by expected
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
  //     // if not equal, assign stored value to expected,
  //     // otherwise, assign stored value by expected
  //
  // atomic_flag
  //   special member functions:
  //     this_t() initialize to zero
  //     can not copy and move
  //   test()->bool
  //   test_and_set()->bool
  //   clear()
}
void doc_weak_ptr() {
  // bad_weak_ptr : exception
  //   what()
  //
  // weak_ptr<T>
  //   element_type
  //   smf: full
  //   weak_ptr(const weak_ptr<Y> &)
  //   =(const weak_ptr<Y> &)
  //   weak_ptr(weak_ptr<Y> &&)
  //   =(weak_ptr<Y> &&)
  //   weak_ptr(const shared_ptr<Y> &)
  //   =(const shared_ptr<Y> &)
  //   reset()
  //   use_count()->long
  //   expired()->bool
  //   lock()->shared_ptr<T>
  //   owner_before(const shared_ptr<U> &)->bool
  //   owner_before(const weak_ptr<U> &)->bool
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
  // shared_ptr<T>
  // weak_ptr<T>
  //   element_type = remove_extent_t<T>
  //   smf: full
  //   weak_ptr(const weak_ptr<Y> &) // Y * is compatible with T *
  //   =(const weak_ptr<Y> &) // Y * is compatible with T *
  //   weak_ptr(weak_ptr<Y> &&) // Y * is compatible with T *
  //   =(weak_ptr<Y> &&) // Y * is compatible with T *
  //   weak_ptr(const shared_ptr<Y> &) // Y * is compatible with T *
  //   =(const shared_ptr<Y> &) // Y * is compatible with T *
  //   reset()
  //   use_count()
  //   expired()
  //   lock()->shared_ptr<T>
  //   owner_before(const shared_ptr<U> &)
  //   owner_before(const weak_ptr<U> &)
  // template <class T>
  // weak_ptr(shared_ptr<T>)->weak_ptr<T>;
  //
  // owner_less<T = void>
  // owner_less<shared_ptr<T>>
  //   ()(const shared_ptr<T> &, const shared_ptr<T> &)
  //   ()(const weak_ptr<T> &, const shared_ptr<T> &)
  //   ()(const shared_ptr<T> &, const weak_ptr<T> &)
  // owner_less<weak_ptr<T>>
  //   ()(const weak_ptr<T> &, const weak_ptr<T> &)
  //   ()(const weak_ptr<T> &, const shared_ptr<T> &)
  //   ()(const shared_ptr<T> &, const weak_ptr<T> &)
  // owner_less<void>
  //   is_transparent
  //   ()(const shared_ptr<T> &, const shared_ptr<T> &)
  //   ()(const weak_ptr<T> &, const weak_ptr<T> &)
  //   ()(const weak_ptr<T> &, const shared_ptr<T> &)
  //   ()(const shared_ptr<T> &, const weak_ptr<T> &)
  //
  // enable_shared_from_this<T>
  //   protected smf: full
  //   shared_from_this()->shared_ptr<(const) T>
  //   weak_from_this()->weak_ptr<(const) T>
  //
  // shared_ptr<T>
  //   element_type = remove_extent_t<T>
  //   weak_type = weak_ptr<T>
  //   smf: full
  //   shared_ptr(nullptr) // make no counter
  //   reset()
  //   shared_ptr(Y *) // Y * is compatible with T *
  //   reset(Y *)
  //   shared_ptr(Y *, D) // Y * is compatible with T *
  //   shared_ptr(nullptr, D) // make counter
  //   reset(Y *, D)
  //   shared_ptr(Y *, D, A) // Y * is compatible with T *
  //   shared_ptr(nullptr, D, A) // make counter
  //   reset(Y *, D, A)
  //   shared_ptr(const shared_ptr<Y> &) // Y * is compatible with T *
  //   =(const shared_ptr<Y> &) // Y * is compatible with T *
  //   shared_ptr(shared_ptr<Y> &&) // Y * is compatible with T *
  //   =(shared_ptr<Y> &&) // Y * is compatible with T *
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
  //   element_type *get()
  //   *()->element_type & // T is array
  //   ->()->element_type * // T is array
  //   [](ptrdiff_t)->element & // T is not array
  //   use_count()->long
  //   explicit bool() // return get() != nullptr
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
  // allocate_shared<T>(a, s...)->shared_ptr<T>
  // allocate_shared<T [N]>(a)->shared_ptr<T [N]>
  // allocate_shared<T [N]>(a, const T &)->shared_ptr<T [N]>
  // allocate_shared<T []>(a, n)->shared_ptr<T []>
  // allocate_shared<T []>(a, n, const T &)->shared_ptr<T []>
  //
  // allocate_shared_for_overwrite<T>(a)->shared_ptr<T>
  // allocate_shared_for_overwrite<T []>(a, n)->shared_ptr<T []>
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
  //   joinable()
  //   join()
  //   detach()
  //
  //   id
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
  //   this_t(m, adopt_lock_t)
  //   explicit this_t(m)
  //
  // unique_lock<M>
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
  //   release()->mutex_ptr
  //   owns_lock()->bool
  //   explicit operator bool()
  //   mutex()->mutex_ptr
  //
  // enum cv_status
  //   no_timeout
  //   timeout
  // condition_variable
  //   special member functions: default-constructible
  //   notify_one()
  //   notify_all()
  //   wait(unique_lock<mutex> &lock)
  //   wait(lock, eq)
  //   wait_until(lock, t)
  //   wait_until(lock, t, eq)->bool
  //   wait_for(lock, t)
  //   wait_for(lock, t, eq)->bool
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
  //   try_enter(f)
  //   enter(f)
  //   enter_until(f, eq)
  //   enter_notify_one(f, condvar_ref)
  //   enter_notify_all(f, condvar_ref)
  //   leave_until(eq, condvar_ref) // in this->enter()
  //   leave_notify_one(condvar_ref)
  //   leave_notify_all(condvar_ref)
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
  //   try_lock_shared()
  //   unlock_shared()
  //
  //   native_handle_type
  //   native_handle()->native_handle_type
  //
  //   try_lock_for(t)->bool // only for shared_timed_mutex
  //   try_lock_until(t)->bool // only for shared_timed_mutex
  //   try_lock_shared_for(t)->bool // only for shared_timed_mutex
  //   try_lock_shared_until(t)->bool // only for shared_timed_mutex
  //
  // shared_lock<M>
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
  //   release()->mutex_ptr
  //   owns_lock()->bool
  //   explicit operator bool()
  //   mutex()->mutex_ptr
}
void doc_thread_pool() {
  // thread_pool
  //   smf: only default constructible
  //     // when destructor is called, there is no running thread
  //   explicit thread_pool(n)
  //   fetch(f)->pool_thread // if pool is full, just call f
  //   fetch_real(f)->pool_thread
  //     // if pool is full, append and try again and again
  //   size_type
  //   count()
  //   count_running()
  //   count_sleeping()
  //   shrink_to_fit()
  //   append(n);
  //
  // default_thread_pool()->thread_pool &
  //
  // pool_thread
  //   smf: default-constructible, movable
  //     // move assignment need empty target
  //   explicit pool_thread(f) // if pool is full, just call f
  //   joinable()
  //   join()
  //   try_join()
  //     // always call try_join because join() can not work if no real thread
  //     // is getted
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
  //   smf: only default constructible
  //   exclusive_thread_pool(size_t)
  //   fetch(f)
  //   join()
  //     // join is not called by thread object, but called by pool
  //   size_type = size_t
  //   count()->size_type
  //   count_running()->size_type
  //   count_sleeping()->size_type
  //   shrink_to_fit()
  //   append(size_type)
}

void doc_key_code() {
  // key_code_t = unsigned
  //
  // inline constexpr unsigned
  // key_lbutton = ...
  // key_rbutton
  // key_mbutton
  // key_xbutton1
  // key_xbutton2
  // key_volume_mute key_volume_down key_volume_up
  // key_print key_pause key_insert key_delete key_scroll
  // key_home key_end key_pageup key_pagedown
  // key_esc                     key_backspace
  // key_tab
  // key_capslock                    key_enter
  // key_shift
  // key_ctrl key_alt   key_space    key_up
  //                        key_left key_down key_right
  // key_app
  // key_lwin key_rwin
  // key_lshift key_rshift
  // key_lctrl key_rctrl
  // key_lalt key_ralt
  //
  // key_0 key_1 key_2 key_3 key_4 key_5 key_6 key_7 key_8 key_9
  //
  // key_a key_b key_c key_d key_e key_f key_g key_h key_i key_j key_k
  // key_l key_m key_n key_o key_p key_q key_r key_s key_t key_u
  // key_v key_w key_x key_y key_z
  //
  // key_f1 key_f2 key_f3 key_f4 key_f5 key_f6 key_f7 key_f8 key_f9 key_f10
  // key_f11 key_f12 key_f13 key_f14 key_f15 key_f16 key_f17 key_f18 key_f19
  // key_f20 key_f21 key_f22 key_f23 key_f24
  //
  // key_us_semicolon = VK_OEM_1;
  //   // Used for miscellaneous characters;
  //   // it can vary by keyboard. For the US standard keyboard, the ;: key
  // key_us_plus = VK_OEM_PLUS;
  //   // For any country/region, the + key
  // key_us_comma = VK_OEM_COMMA;
  //   // For any country/region, the , key
  // key_us_minus = VK_OEM_MINUS;
  //   // For any country/region, the - key
  // key_us_period = VK_OEM_PERIOD;
  //   // For any country/region, the . key
  // key_us_slash = VK_OEM_2;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the /? key
  // key_us_tilde = VK_OEM_3;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the `~ key
  // key_us_lbrace = VK_OEM_4;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the [{ key
  // key_us_backslash = VK_OEM_5;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the \\| key
  // key_us_rbrace = VK_OEM_6;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the ]} key
  // key_us_quote = VK_OEM_7;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  //   // For the US standard keyboard, the '" key
  // key_oem_8 = VK_OEM_8;
  //   // Used for miscellaneous characters; it can vary by keyboard.
  // key_oem_102 = VK_OEM_102;
  //   // The <> keys on the US standard keyboard,
  //   // or the \\| key on the non-US 102-key keyboard
  // key_ime_process = VK_PROCESSKEY; // IME PROCESS key
  // key_packet = VK_PACKET;
  //   // Used to pass Unicode characters as if they were keystrokes.
  //   // The VK_PACKET key is the low word of a 32-bit Virtual Key value
  //   // used for non-keyboard input methods. For more information,
  //   // see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
  // key_oem_clear = VK_OEM_CLEAR; // Clear key
  //
  // key_media_next_track   key_media_previous_track
  // key_media_stop   key_media_play_pause
  //
  // key_numlock
  // key_numpad0 key_numpad1 key_numpad2
  // key_numpad3 key_numpad4 key_numpad5
  // key_numpad6 key_numpad7 key_numpad8 key_numpad9
  // key_add key_subtract key_multiply key_divide
  // key_decimal key_separator
  //
  // key_clear
  // key_sleep
}
void doc_key_comb() {
  // key_name(unsigned)->sview
  // key_code(sview)->optional<unsigned>
  // is_modifier_key(unsigned)->bool // (l/r)(ctrl/alt/shift)
  // is_upper_modifier_key(unsigned)->bool // shift
  // is_lower_modifier_key(unsigned)->bool // lshift/rshift
  // to_upper_modifier_key(unsigned)->unsigned // lshift/rshift -> shift
  //
  // key_comb
  //   // if valid, back(*this) is the non-modifier key
  //   smf: full
  //   ==
  //   <=> // compare size for different sizes, otherwise compare sequence
  //   explicit key_comb(r)
  //     // at most 8 keys, invalid input get nil key_comb
  //     //
  //     // if size(r) == 1u
  //     //   all key is acceptable  
  //     // otherwise
  //     //   invalid if only contains modifier keys
  //     //   invalid if number of non-modifier key is not 1
  //     //
  //     // for modifier key:
  //     //   no modifier key, all upper modifier key, or all lower modifier key
  //     //   no repeated modifier key
  //   explicit key_comb(unsigned) // invalid input get nil key_comb
  //   key_comb(k1, k2) // invalid input get nil key_comb
  //   key_comb(k1, k2, k3) // invalid input get nil key_comb
  //   key_comb(k1, k2, k3, k4) // invalid input get nil key_comb
  //   valid()->bool // not nil
  //   explicit operator bool() // valid()
  //   begin()->const unsigned *
  //   end()->const unsigned *
  //   size()
  //   empty()
  //   conflicts_with(const key_comb &)
  //     // e.g. a conflicts with a
  //     // e.g. lctrl conflicts with ctrl
  //     // e.g. lctrl-lalt-a conflicts with ctrl-alt-a
  //     // e.g. lctrl-lalt-a does not conflicts with rctrl-ralt-a
  //   includes(const key_comb &) // false if equal
  //     // e.g. a does not includes a
  //     // e.g. lctrl-a includes a
  //     // e.g. lctrl-lalt-a includes lctrl-a
  //   triggered_by(const local_vector<unsigned, 8> &)
  //     // upper modifier can be triggered by lower modifier
  //     // lower modifier can not be triggered by upper modifier
  //
  // enum key_action
  //   key_press
  //   key_release
  //   key_double_press
  //   key_action_lim
}
void doc_key_recorder() {
  // key_recorder
  //   smf: only default constructible
  //
  //   press(key_code_t)
  //   release(key_code_t)
  //   unset_focus() // clear states
  //   set_focus() // read states from system
  //
  //   pressed(key_code_t)
  //   locked(key_code_t)
  //     // return false if not one of {key_capslock, key_numlock, key_scroll}
  //   copy_pressed(O)->O
}
void doc_key_mapper() {
  // key_mapper
  //   smf: only destructible
  //   explicit key_mapper(const key_recorder &)
  //
  //   set_focus() // do nothing
  //   unset_focus() // clear time caches for double-press
  //   press(key_code_t) // no upper modifier key is accepted
  //     // key_recorder::press
  //   release(key_code_t) // no upper modifier key is accepted
  //
  //   rotate_wheel(int)
  //   bound_wheel()->bool
  //   bind_wheel(f)
  //   unbind_wheel()
  //
  //   clear()
  //   empty()
  //
  //   contains(pair<key_comb, key_action>)->bool // exactly contain
  //
  //   insert(pair<key_comb, key_action>,
  //          function<void ()>)->bool
  //   insert_or_assign(pair<key_comb, key_action>,
  //                    function<void ()>)->bool
  //   remove(pair<key_comb, key_action>)->bool
}
void doc_vector_math() {
  // pi_f
  // pi_d
  //
  // abs_square(v1)
  // dot(v1, v2)
  // cross(v1, v2)
  // normalize(v)
  // traverse(m)
  //
  // mat<T, W, H>
  // ivec2 = mat<int, 1, 2>
  // ivec3 = mat<int, 1, 3>
  // ivec4 = mat<int, 1, 4>
  // lvec2 = mat<long, 1, 2>
  // lvec3 = mat<long, 1, 3>
  // lvec4 = mat<long, 1, 4>
  // llvec2 = mat<long long, 1, 2>
  // llvec3 = mat<long long, 1, 3>
  // llvec4 = mat<long long, 1, 4>
  // zvec2 = mat<size_t, 1, 2>
  // zvec3 = mat<size_t, 1, 3>
  // zvec4 = mat<size_t, 1, 4>
  // fvec2 = mat<float, 1, 2>
  // fvec3 = mat<float, 1, 3>
  // fvec4 = mat<float, 1, 4>
  // fmat2 = mat<float, 2, 2>
  // fmat3 = mat<float, 3, 3>
  // fmat4 = mat<float, 4, 4>
  // dvec2 = mat<double, 1, 2>
  // dvec3 = mat<double, 1, 3>
  // dvec4 = mat<double, 1, 4>
  // dmat2 = mat<double, 2, 2>
  // dmat3 = mat<double, 3, 3>
  // dmat4 = mat<double, 4, 4>
  //
  // mat<T, W, H>
  //   smf: dependent
  //   ==(const mat &) = default;
  //   <=>(const mat &)
  //
  //   explicit(sizeof...(s) == 1u) mat(s...)
  //   explicit mat(m2_lv_or_rv)
  //   mat(m2_lv_or_rv, fll)
  //   =(m2_lv_or_rv)
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
  //
  //   matrix_width()->size_type
  //   matrix_height()->size_type
  //
  //   begin()
  //   end()
  //   cbegin()
  //   cend()
  //   size()
  //   empty()
  //
  //   front()
  //   back()
  //   [](i)
  //
  //   left_top()
  //   left_bottom()
  //   right_top()
  //   right_bottom()
  //
  //   ()(x, y)
  //   iter(x, y)
  //   row(n)
  //   rows()
  //   column(n)
  //   columns()
  //
  //   -()
  //   +()
  //   +=(m)
  //   -=(m)
  //   +(m)
  //   -(m)
  //   add_with_check(const mat &)->mat // value type is integral
  //   sub_with_check(const mat &)->mat // value type is integral
  //   sub_to_zero_at_most(const mat &)->mat
  //     // value type is integral, all arg element greater than zero
  //
  //   *=(k)
  //   *(k)
  //   friend k * m
  //   /=(k)
  //   /(k)
  //
  //   *=(m)
  //   *(m)
  //
  //   is_zero()
  //   fill(x)
  //
  //   x() r() s()
  //   y() g() t()
  //   z() b() p()
  //   w() a() q()
  //
  //   apply_w() // vec3 to vec2, vec4 to vec3
  //   reset_w() // unchanged vec values but set w to 1
  //
  //   abs()->T // only for vec
  //   abs_square()->T // only for vec
  //   dot(v)->T // only for vec
  //   cross(v)->mat // only for vec
  //   normalize()->mat // only for vec, unchange if abs() is zero
  //
  //   on_left(const vec2 &p, const vec2 &v)->bool // only for vec2
  //   on_right(const vec2 &p, const vec2 &v)->bool // only for vec2
  //
  //   in_triangle(const vec2 &p1, const vec2 &p2, const vec2 &p3)->bool
  //     // only for vec2
  //   in_triangle(const vec2 &p1, const vec2 &p2_m_p1,
  //               const vec2 &p2, const vec2 &p3_m_p2,
  //               const vec2 &p3, const vec2 &p1_m_p3)->bool
  //     // only for vec2
  //
  //   static identity()->mat
  //
  //   traverse()->mat
  //
  //   static x_translation(s)->mat // for mat3
  //   static y_translation(s)->mat // for mat3
  //   static translation(vec2)->mat // for mat3
  //
  //   static x_translation(s)->mat // for mat4
  //   static y_translation(s)->mat // for mat4
  //   static z_translation(s)->mat // for mat4
  //   static translation(vec3)->mat // for mat4
  //
  //   static scaling(s)->mat // for mat3 or mat4
  //   static x_scaling(s)->mat // for mat3 or mat4
  //   static y_scaling(s)->mat // for mat3 or mat4
  //   static z_scaling(s)->mat // for mat3 or mat4
  //
  //   static z_rotation(r)->mat // for mat2 or mat3 or mat4
  //   static x_rotation(r)->mat // for mat3 or mat4
  //   static y_rotation(r)->mat // for mat3 or mat4
  //
  //   static rotation(vec3 pos, vec3 normalized_arrow, r)->mat // for mat4
  //   static rotation(vec3 normalized_arrow, r)->mat // for mat4
  //
  //   z_rotate(r)->vec2 // for vec2
  //   z_rotate(r)->vec3 // for vec3
  //   y_rotate(r)->vec3 // for vec3
  //   x_rotate(r)->vec3 // for vec3
  //   rotate(vec3 normalized_arrow, r)->vec3 // for vec3
  //   rotate(vec3 pos, vec3 normalized_arrow, r)->vec3 // for vec3
  //
  //   static persp_projection(w, h, near, far) // for mat4 // perspective
  //   static ortho_projection(w, h, near, far) // for mat4 // orthographic
  //
  //   static look_at(vec3 center, vec3 eye, vec3 up)->mat // for mat4
  //   static look_at(angle_hrot, angle_vrot, dst)->mat // for mat4
  //   static look_at(angle_hrot, angle_vrot, angle_uprot, dst)->mat // for mat4
  //     // angle_uprot: do counterclockwise rotation for camera image
  //   static look_at(vec3 center,
  //                  angle_hrot, angle_vrot, dst)->mat // for mat4
  //   static look_at(vec3 center,
  //                  angle_hrot, angle_vrot, angle_uprot, dst)->mat // for mat4
  //
  //   static reflective_ray(vec incident_ray,
  //                         vec normalized_normal)->vec // for vec
  //   static refractive_ray(vec normalized_incident_ray,
  //                         vec normalized_normal,
  //                         relative_refractive_index)->vec // for vec2 or vec3
  //
  //   project_onto_line(vec pos, vec normalized_arrow)->vec // for vec2 or vec3
  //   project_onto_plane(vec pos, vec normalized_normal)->vec // for vec3
  //
  //   sprint()->string
  //     // for mat<integral, 1, 2>
  //     // for mat<integral, 1, 3>
  //   sprint(size_t precision = 6u)->string
  //     // for mat<floating_point, 1, 2>
  //     // for mat<floating_point, 1, 3>
  //
  //   size_includes(const mat &, less_f = less<>{})->bool
  //     // return true if:
  //     //   for every element of arg, named by x
  //     //     !less_f(the_corresponding_element_in_this, x)
  //   size_no_eq_includes(const mat &, less_f = less<>{})->bool
  //     // *this != arg && this->size_includes(arg, less_f)
  //
  //   // to_abs_point: cast ivec2 of pixel point to absolute point
  //   to_abs_point<FP>()->mat<FP, 1, 2>
  //   to_abs_fpoint()->fvec2
  //   to_abs_dpoint()->dvec2
  //   to_pixel_point(ivec2 pixmap_sz)->ivec2
  //     // cast ivec2 of absolute point to pixel coordinates
  //     // sz != ivec2(0, 0)
  //
  //   dif_from_left_top(ivec2 p)->ivec2
  //     // p is at left of *this
  //     // p is at higher of *this
  //     // return two postive values
  //
  // tuple_element<I, mat<T, 1, N>>
  //   type
  // tuple_size<mat<T, 1, N>>
  //   value
  // get<I>(mat<T, 1, N> &)
  // get<I>(const mat<T, 1, N> &)
  // get<I>(mat<T, 1, N> &&)
  // get<I>(const mat<T, 1, N> &&)
  //
  //
  // hash<mat<T, 1, 2>>
}
void doc_rgba() {
  // rgba_r(uint32_t)->uint32_t
  // rgba_r(uint32_t &, uint32_t)
  // rgba_g
  // rgba_b
  // rgba_a
  //
  // rgba_rgba(uint32_t)->tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  // rgba_rgba(uint32_t &, tuple<uint32_t, uint32_t, uint32_t, uint32_t>)
  // rgba_rgba(uint32_t &, uint32_t, uint32_t, uint32_t, uint32_t)
  //
  // rgba_rgb(uint32_t)->tuple<uint32_t, uint32_t, uint32_t>
  // rgba_rgb(uint32_t &, tuple<uint32_t, uint32_t, uint32_t>)
  // rgba_rgb(uint32_t &, uint32_t, uint32_t, uint32_t)
  // rgba_rga
  // rgba_rba
  // rgba_gba
  //
  // rgba_rg(uint32_t)->tuple<uint32_t, uint32_t>
  // rgba_rg(uint32_t &, tuple<uint32_t, uint32_t>)
  // rgba_rg(uint32_t &, uint32_t, uint32_t)
  // rgba_rb
  // rgba_ra
  // rgba_gb
  // rgba_ga
  // rgba_ba
  //
  // rgba_from_rgba(tuple<uint32_t, uint32_t, uint32_t, uint32_t>)
  // rgba_from_rgba(uint32_t, uint32_t, uint32_t, uint32_t)
  // rgba_from_rgb(...)
  // rgba_from_rga(...)
  // rgba_from_rba(...)
  // rgba_from_gba(...)
  // rgba_from_rg(...)
  // rgba_from_rb(...)
  // rgba_from_ra(...)
  // rgba_from_gb(...)
  // rgba_from_ga(...)
  // rgba_from_ba(...)
  // rgba_from_r(...)
  // rgba_from_g(...)
  // rgba_from_b(...)
  // rgba_from_a(...)
  //
  // rgba_f = fvec4
  // rgba_d = dvec4
  // rgba_to_rgba_f(uint32_t)->rgba_f
  // rgba_to_rgba_d(uint32_t)->rgba_d
  // rgba_from_rgba_f(fvec4)->uint32_t
  // rgba_from_rgba_d(dvec4)->uint32_t
  //
  // rgba_sprint(uint32_t)->string
  //
  // alpha_blend(uint32_t lower, uint32_t upper)->uint32_t
  // alpha_blend(rgba_f lower, rgba_f upper)->rgba_f
  // alpha_blend(rgba_d lower, rgba_d upper)->rgba_d
  //
  // premultiplied_alpha_blend(uint32_t lower, uint32_t upper)->uint32_t
  // premultiplied_alpha_blend(rgba_f lower, rgba_f upper)->rgba_f
  // premultiplied_alpha_blend(rgba_d lower, rgba_d upper)->rgba_d
  //
  // rgba_rgb_mul_div(uint32_t c, uint32_t num, uint32_t den)->uint32_t
  //   // for per element of c, multiply num then divide by den
  //
  // rgba_black
  // rgba_white
  // rgba_red
  // rgba_green
  // rgba_blue
  // rgba_yellow
}
void doc_irect2() {
  // basic_align
  //   left
  //   middle
  //   right
  // basic_v_align
  //   top
  //   middle
  //   bottom
  //
  // irect2
  //   smf: full
  //   ==
  //   irect2(ivec2, ivec2) // {x, y}, {w, h}
  //     // no check for negative size
  //     // terminate if size is too big or w * h overflowed
  //
  //   origin()->ivec2 // return left_bottom()
  //   left_bottom()->ivec2
  //   left_top()->ivec2
  //   right_bottom()->ivec2
  //   right_top()->ivec2
  //   size()->ivec2
  //   width()->int
  //   height()->int
  //   empty()->bool // width() == 0 || height() == 0
  //
  //   includes_point(ivec2)->bool
  //
  //   intersection_with(const irect2 &)->irect2
  //     // if the result is empty, return irect2(this->origin(), ivec2(0, 0))
  //   has_intersection_with(const irect2 &)->bool
  //   exclude_from_non_overlapping(circular_vector<ivec2> &v,
  //                                size_t n_at_most = v.size())
  //   exclude_from(irect2)->local_vector<irect2, 4>
  //
  //   left_of(irect2)->irect2
  //   right_of(irect2)->irect2
  //   top_of(irect2)->irect2
  //   bottom_of(irect2)->irect2
  //
  //   move_to(irect2 r2,
  //           basic_align = basic_align::middle,
  //           basic_v_align = basic_v_align::middle)->irect2
  //     // move *this to r2 by align
  //     // the returned irect2 has the same size() than *this
  //     // if r2 do not includes *this, return irect2(r2.origin(), size())
  //   
  //   to_matrix_point(ivec2 point)->ivec2
  //     // point is in *this (hint: *this is not empty)
  //     // return relative window point: (0, 0) is left-top
  //   to_point(ivec2 relative_win_point)->ivec2
  //     // relative_win_point is in *this (hint: *this is not empty)
  //     // return absolute point: (0, 0) is left-bottom
  //
  //   sprint()->string
}
void doc_pixmap() {
  // pixmap
  //   smf: full
  //   ==
  //
  //   explicit pixmap(ivec2 non_neg_sz)
  //   pixmap(ivec2 non_neg_sz, fll)
  //
  //   pixmap(ivec2 non_neg_sz, r)
  //
  //   width()->int
  //   height()->int
  //   size()->ivec2
  //   empty()->bool
  //   clear()
  //
  //   row(int)->range // valid row number
  //   rows()->range
  //   column(int)->range // valid column number
  //   columns()->range
  //   sub_range(irect2 r)
  //     // return stored sequence (from top line to bottom line)
  //
  //   fill(irect2 r, uint32_t)
  //   fill(uint32_t)
  //
  //   iter(ivec2) // valid coordinates (from left-bottom to right-top)
  //   operator ()(ivec2)->value_type_ref // valid coordinates
  //   iter(int, int)
  //     // valid matrix coordinates (from left-top to right-bottom)
  //   includes_point(ivec2)
  //
  //   value_type = uint32_t
  //
  //   range()
  //
  //   rectangle()->irect2
  //
  //   resize(ivec2, uint32_t = 0u)
  //   reset(ivec2, uint32_t = 0u)
  //
  //   capacity()->size_t
  //   reserve(size_t)
  //   reserve_more(size_t)
  //   shrink_to_fit()
  //
  //   reverse_left_right()
  //   reverse_up_down()
  //
  //   cover(ivec2, const pixmap &, mix_f = ...)->ivec2
  //     // return replaced size
  //     // mix_f eats two value of const uint32_t &
  //     // mix_f(old, new)->uint32_t
  //   cover(ivec2, const pixmap &b, irect2 r2, mix_f = ...)->ivec2
  //     // return replaced size
  //     // r2 is in b.rectangle()
  //
  //   cover(irect2 r, const pixmap &b, {irect2 r2, }, mix_f = ...,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->ivec2
  //     // r2 is in b.rectangle()
  //     // return replaced size
  //     // if r can not include r2, do nothing and return {}
  //
  //   cover(irect2 r, irect2 r2, const pixmap &b, {irect2 b_r, }, mix_f = ...,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->ivec2
  //     // same as no-r2-version except for keeping only part of r2
  //     // b_r is in b.rectangle(), b_r is b.rectangle() if no list
  //     // return replaced size
  //     // if r can not include b_r, do nothing and return {}
  //
  //   default_multi_sample_coefficient()->int // return 6
  //   default_multi_sample_coefficient(int) // not thread-safe
  //
  //   // draw_xxx():
  //   //   size() is not too big, msaa_coeff is not too big
  //
  //   draw_point(fvec2, uint32_t clr, int msaa_coeff = ...)
  //   draw_point(dvec2, uint32_t clr, int msaa_coeff = ...)
  //
  //   // draw_line(): if two point have no difference, draw one point
  //   draw_line(fvec2, fvec2, uint32_t clr, int msaa_coeff = ...)
  //   draw_line(dvec2, dvec2, uint32_t clr, int msaa_coeff = ...)
  //   draw_triangle(fvec2, fvec2, fvec2, uint32_t clr, int msaa_coeff = ...)
  //   draw_triangle(dvec2, dvec2, dvec2, uint32_t clr, int msaa_coeff = ...)
  //
  //   draw_triangle_outline(fvec2, fvec2, fvec2,
  //                         uint32_t clr, int msaa_coeff = ...)
  //   draw_triangle_outline(dvec2, dvec2, dvec2,
  //                         uint32_t clr, int msaa_coeff = ...)
  //
  //   draw_triangle(fvec2, fvec2, fvec2, uint32_t clr, int msaa_coeff = ...)
  //   draw_triangle(dvec2, dvec2, dvec2, uint32_t clr, int msaa_coeff = ...)
  //
  //   draw_triangle_with_outline(fvec2, fvec2, fvec2,
  //                              uint32_t clr, int msaa_coeff = ...)
  //   draw_triangle_with_outline(dvec2, dvec2, dvec2,
  //                              uint32_t clr, int msaa_coeff = ...)
}
void doc_font_glyph() {
  // glyph
  //   // store reference or dyanmic array
  //   smf: full
  //   value_type = unsigned char
  //   range()->iter_pair<dup_compressed_array<unsigned char>::const_iterator>
  //   compressed_length()->size_t
  //   empty()->bool
  //   owns()->bool
  //   rectangle()->irect2
  //   size()->ivec2
  //   width()->int
  //   height()->int
  //   left_offset()->int // leftmost_point_x - baseline_point_x (may be minus)
  //   top_offset()->int // topmost_point_y - baseline_point_y (may be minus)
  //   left_width()->int // baseline left
  //   right_width()->int
  //   upper_height()->int // baseline left
  //   lower_height()->int
  //   cover(bitmap &b, ivec2 pos, uint32_t color)->ivec2
  //   cover(bitmap &b, ivec2 pos, irect2 rect, uint32_t color)->ivec2
  //     // return painted size
  //     // paint to intersection area of b.rectangle(), rect, and
  //     // irect2(pos, size)
  //   code()->uint32_t
  //   utf8_code()->string
  //
  // font_file_data
  //   smf: destructible
  //   explicit font_file_data(sview)
  // font
  //   smf: only destructible
  //   font(font_file_data, size_t sz, cached = true) // throw if n == 0
  //   font(sview, size_t sz, cached = true) // throw if n == 0
  //   cached()->bool
  //   size()->int
  //   has_character(unsigned long)->bool
  //   [](unsigned long)->glyph
  //   [](sview)->glyph
  //   a_to_z_upper_height() const noexcept->int;
  //   a_to_z_lower_height() const noexcept->int;
  //
  //   default_separator_width() const noexcept->int
  //   default_separator_width(int) noexcept
  //
  //   cover(sview, pixmap &, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //   cover(u32sview, pixmap &, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //
  //   cover(sview, pixmap &, irect2, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //   cover(u32sview, pixmap &, irect2, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //     // cover the first irect2, but keep only part of irect2
  //
  //   cover(sview, int separator_width,
  //         pixmap &, irect2, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //   cover(u32sview, int separator_width,
  //         pixmap &, irect2, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //
  //   cover(sview, int separator_width,
  //         pixmap &, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //   cover(u32sview, int separator_width,
  //         pixmap &, irect2, uint32_t,
  //         basic_align = basic_align::middle,
  //         basic_v_align = basic_v_align::middle)->size_t
  //     // return number of the painted characters
  //
  //   min_size_for(sview, int space_width = ...)->ivec2
  //   min_size_for(u32sview, int space_width = ...)->ivec2
  //
  // font_ptr
  //   smf: non-default-constructible
  //   font_ptr(shared_ptr<font>) // throw for nullptr
  //   font_ptr(font &)
  //   font_ptr(font *) // throw for nullptr
  //   get()->font *
  //   operator ->()->font *
  //   operator *()->font &
}
void doc_basic_window_program() {
  // basic_system_window
  //   smf: none
  //
  //   static has_console()->bool
  //   static start_time()->steady_clock::time_point
  //   static monitor_position()->ivec2
  //     // left-top corrdinates from system origin point
  //     // (left-top to right-bottom)
  //   static monitor_size()->ivec2
  //   static frame_position()->ivec2
  //     // left-top corrdinates from system origin point
  //     // (left-top to right-bottom)
  //   static frame_size()->ivec2
  //   static size()->ivec2 // size of client area
  //
  //   static resize(ivec2) // resize client area
  //   static move_to(ivec2) // move frame
  //   static paint(const pixmap &)
  //
  // as_basic_window_program<DERIVED>
  //   // DERIVED optional functions:
  //   //   create()
  //   //   destroy()
  //   //   paint()
  //   //   set_focus()
  //   //   unset_focus()
  //   //   press(unsigned)
  //   //   release(unsigned)
  //   //   rotate_wheel(int)
  //   //   mouse_leave()
  //   //   mouse_move(ivec2) // from left-bottom to right-top
  //   //   resize(ivec2)
  //
  // run_basic_window_program<DERIVED>(console_on = false)->int
}
void doc_window_paint_traits_default_affairs() {
  // window_paint_traits
  //   uint32_t background_color
  //   uint32_t forground_color
  //   uint32_t light_forground_color
  //   uint32_t font_color
  //   uint32_t frame_color
  //
  //   uint32_t groove_color
  //   uint32_t check_box_color
  //   uint32_t progress_bar_color
  // default_window_paint_traits()->const window_paint_traits &
  //
  // default_locked_color(uint32_t)->uint32_t
  //   // map normal rgb to locked rgb, do not touch alpha
  // default_pressed_color(uint32_t)->uint32_t // do not touch alpha
  // default_hovered_color(uint32_t)->uint32_t // do not touch alpha
  // default_window_color(uint32_t,
  //                      bool hovered,
  //                      bool pressed,
  //                      bool locked)->uint32_t // do not touch alpha
  // default_window_color(uint32_t,
  //                      bool hovered,
  //                      bool locked)->uint32_t // do not touch alpha
  // default_window_color(uint32_t,
  //                      bool locked)->uint32_t // do not touch alpha
  //
  // default_font()->font * // same as system_window::get()->vf_default_font()
  //
  // default_slider_line_width()->int
  //   // same as system_window::get()->vf_default_bar_line_width()
  // default_slider_length()->int
  //   // same as system_window::get()->vf_default_slider_length()
}
void doc_window() {
  // window
  // window_controller
  // window_tree_vector
  // window_tree
  // window_iterator = window_tree_vector::iterator;
  // window_const_iterator = window_tree_vector::const_iterator;
  //
  // window // abstract class
  //   smf: only default constructible
  //
  //   iterator
  //   const_iterator
  //   iter() noexcept->window_iterator
  //   iter() const noexcept->window_const_iterator
  //   protected iter(window_iterator) noexcept
  //
  //   has_focus() const noexcept->bool
  //   protected focus(bool) noexcept
  //   protected virtual set_focus()
  //   protected virtual unset_focus()
  //
  //   hovered() const noexcept->bool
  //   protected hovered(bool) noexcept
  //   protected virtual set_hovered()
  //   protected virtual unset_hovered()
  //
  //   virtual mouse_move(ivec2);
  //     // eat absolute position
  //     // only be called when focused
  //
  //   virtual bool aware_keys() const
  //     // if true, key-focused-window stoped at *this to avoid upward search
  //   has_keys() const noexcept->bool
  //   set_keys()
  //   unset_keys() noexcept
  //   key_mapper &keys()
  //   protected virtual press(key_code_t)
  //   protected virtual release(key_code_t)
  //   protected virtual rotate_wheel(int)->bool
  //     // return whether stop calling iter()->parent()->rotate_wheel()
  //
  //   position() const noexcept->ivec2 // return left-bottom point
  //   protected assign_position(ivec2) noexcept // no adjustment
  //   virtual void move_to(ivec2)
  //   size() const noexcept->ivec2
  //   protected assign_size(ivec2) noexcept // no adjustment
  //   virtual resize(ivec2)
  //   virtual rectangle(irect2)
  //   min_size() const noexcept->ivec2
  //   max_size() const noexcept->ivec2
  //   min_width() const noexcept->int
  //   min_height() const noexcept->int
  //   max_width() const noexcept->int
  //   max_height() const noexcept->int
  //   width() const noexcept->int
  //   height() const noexcept->int
  //   width(int);
  //   height(int);
  //
  //   protected assign_min_size(ivec2) noexcept // no adjustment
  //   protected assign_max_size(ivec2) noexcept // no adjustment
  //   protected assign_locked_minmax_size(ivec2 v = {}) noexcept
  //     // no adjustment
  //     // set min_size = max_size = v
  //   protected lock_size() noexcept
  //     // assign_locked_minmax_size() with adjustment
  //   protected assign_unlocked_minmax_size() noexcept // no adjustment
  //     // set min_size = {} and max_size = {max, max}
  //   protected unlock_size() noexcept
  //     // assign_unlocked_minmax_size() with adjustment
  //   protected lock_width() noexcept
  //   protected lock_height() noexcept
  //   protected unlock_width() noexcept
  //   protected unlock_height() noexcept
  //   protected virtual controls_children() const->bool
  //     // if true, adjust_to_fit_parent() should do nothing to let the parent
  //     // decide rectangle of *this
  //   protected virtual adjust_to_fit_children() // do not touch abs_rectangle
  //   protected virtual adjust_to_fit_parent() // do not touch abs_rectangle
  //   protected virtual adjust_children_to_fit_this()
  //     // do not touch abs_rectangle
  //   protected virtual adjust_for_rectangle()
  //     // set data for pre-provided rectangle info by sz() and position()
  //   adjust()
  //     // call "adjust_" functions and adjust abs_rectangle
  //     // when all rectangle-changed windows are hooked in sub-tree of *this
  //
  //   hidden() const noexcept->bool
  //   protected assign_hidden(bool) noexcept
  //   virtual hide()
  //   virtual show()
  //
  //   locked() const noexcept->bool
  //   protected assign_locked(bool) noexcept
  //   virtual lock()
  //   virtual unlock()
  //
  //   can_cross() const noexcept->bool
  //   protected assign_cross(bool) noexcept
  //   virtual lock_cross()
  //   virtual unlock_cross()
  //
  //   virtual init_after_first_insertion()
  //   virtual init_new_child(window_iterator)
  //   protected stored_data() noexcept->dynamic_ptr<void> &
  //   protected stored_data() const noexcept->const dynamic_ptr<void> &
  //   virtual cat(window_tree &, window_tree &&);
  //   virtual cat(window_tree &, window_tree_vector &&);
  //   cat(window_tree &, s...)
  //
  //   protected assign_rectangle(irect2) noexcept
  //   rectangle() const noexcept->irect2
  //   virtual rectangle(irect2)
  //   rectangle_info_t
  //     rectangle
  //     min_size
  //     max_size
  //     ==
  //   rectangle_info()const noexcept->rectangle_info_t
  //   abs_rectangle() const noexcept->irect2
  //   protected abs_rectangle(irect2) noexcept
  //   protected abs_position(ivec2)
  //   abs_position() const noexcept->ivec2
  //
  //   set_tooltip<T = tooltip>(s...)
  //   unset_tooltip()
  //   has_tooltip() const noexcept->bool
  //   tooltip() noexcept->window *
  //
  //   companion_tooltip_offset()->optional<ivec2>
  //   companion_tooltip_offset(optional<ivec2>)
  //   set_companion_tooltip<T>(s...)
  //   unset_companion_tooltip()
  //   has_companion_tooltip() const noexcept->bool
  //   companion_tooltip() noexcept->window *
  //
  //   virtual refresh_when_focused()
  //
  //   protected fill_color_excluding_children(uint32_t,
  //                                           pixmap &,
  //                                           exclusive_thread_pool &,
  //                                           bool)
  //   protected fill_color_excluding_one_child(uint32_t,
  //                                            pixmap &,
  //                                            exclusive_thread_pool &,
  //                                            bool)
  //
  //   protected rect_buf_for_painting() noexcept->circular_vector<irect2> &
  //   protected list_hook() const noexcept->window *
  //   protected list_hook(window *) noexcept
  //   protected virtual no_overlap() const->bool // children have no overlap
  //   protected virtual paint(pixmap &, exclusive_thread_pool &,
  //                           bool no_overlap) = 0
  //     // no overlap with left and right windows
}
void doc_window_tree() {
  // window_tree_arg_tuple<S...>
  //   smf: default-contructible
  //   explicit window_tree_arg_tuple(s...)
  //   operator *()->tuple &
  //   operator ->()->tuple *
  // make_window_tree_arg_tuple(s...)->window_tree_arg_tuple<decay_t<S>...>
  //
  // inner::fns::paint_window_tree(window_tree &,
  //                               pixmap &, exclusive_thread_pool &)
  //
  // window_tree
  //   tree_type = this_t
  //   vector_type = window_tree_vector
  //   iterator = window_iterator
  //   const_iterator = window_const_iterator
  //   key_type = dynamic<window, 0>
  //   children_type
  //   tree_node_type
  //   value_type = inner::tree_node_impl<...>
  //   reference = window &
  //   const_reference = const window &
  //   difference_type
  //   size_type
  //
  //   root()->(const_)iterator
  //   croot()->const_iterator
  //
  //   empty()->bool
  //
  //   smf:
  //     default constructible
  //     movable
  //
  //   allocator_type
  //   get_allocator()->allocator_type
  //
  //   explicit window_tree(const allocator_type &)
  //   explicit window_tree(allocator_arg_t, const allocator_type &)
  //
  //   explicit(...) window_tree(in_place_type_t<T>, s...)
  //   append_children(s...) &->this &
  //   append_children(s...) &&->this &&
  //   cat(window_tree or window_tree_vector ...) &->this &
  //   cat(window_tree or window_tree_vector ...) &&->this &&
  //   prepend_children(s...) &->this &
  //   prepend_children(s...) &&->this &&
  //
  //   copy(tree_t, tree_iter_t, get_key)->this_t
  //     // get_key return dynamic_window
  //
  //   emplace<T>(s...)->T *
  //   clear()
  //
  //   swap(const_iterator, const_iterator)
  //   swap(const_iterator, tree_type ref, const_iterator)
  //   swap(const_iterator, vector_type ref, const_iterator)
  //
  //   extract(const_iterator)->tree_type
  //   insert(const_iterator, tree_type &&)->iterator
  //   exchange(const_iterator, tree_type &&)->tree_type
  //   replace(const_iterator, tree_type &&)->iterator
  //
  //   extract(const_iterator, const_iterator)->vector_type
  //   insert(const_iterator, vector_type &&)->iterator
  //   exchange(const_iterator, const_iterator, vector_type &&)->vector_type
  //   replace(const_iterator, const_iterator, vector_type &&)->iterator
  //
  //   extract_children(const_iterator)->vector_type
  //   exchange_children(const_iterator, vector_type &&)->vector_type
  //   replace_children(const_iterator, vector_type &&)->iterator
  //
  //   splice(const_iterator, tree_type ref, const_iterator)
  //   splice(const_iterator, tree_type ref, const_iterator, const_iterator)
  //
  //   splice(const_iterator, vector_type ref, const_iterator)
  //   splice(const_iterator, vector_type ref, const_iterator, const_iterator)
  //
  //   emplace<T>(const_iterator, s...)->T *
  //
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   clear(const_iterator)
  //
  //   emplace_back<T>(const_iterator, s...)->T *
  //   push_back(const_iterator, tree_type &&)
  //   push_back(const_iterator, vector_type &&)
  //   append(const_iterator, s...)
  //   pop_back(const_iterator, size_type = 1u)
  //
  //   emplace_front<T>(const_iterator, s...)->T *
  //   push_front(const_iterator, tree_type &&)
  //   push_front(const_iterator, vector_type &&)
  //   prepend(const_iterator, s...)
  //   pop_front(const_iterator, size_type = 1u)
  //
  //   capacity(const_iterator)->size_type
  //   full(const_iterator)->bool
  //   reserve(const_iterator, size_type)
  //   first_order_reserve(const_iterator, size_type)
  //   reserve_more(const_iterator, size_type)
  //   first_order_reserve_more(const_iterator, size_type)
  //   shrink_to_fit(const_iterator)
  //   first_order_shrink_to_fit(const_iterator)
  //
  //   rect(irect2)->this_t ref
  //   sz(ivec2)->this_t ref
  //   wid(int)->this_t ref
  //   hei(int)->this_t ref
  //   pos(ivec2)->this_t ref
  //   args(s...)->this_t ref // store window_tree_arg_tuple in stored_data()
  //
  // make_win<T>(s...)->window_tree
  // make_win<T>(T *&, s...)->window_tree
  // make_window(T *&, s...)->window_tree // = make_win<T>(...)
}
void doc_window_tree_vector() {
  // window_tree_vector
  //   tree_type = window_tree
  //   vector_type = this_t
  //   iterator = window_iterator
  //   const_iterator = window_const_iterator
  //   key_type = dynamic<window, 0>
  //   children_type
  //   tree_node_type
  //   value_type = inner::tree_node_impl<...>
  //   reference = window &
  //   const_reference = const window &
  //   difference_type
  //   size_type
  //
  //   begin()->(const_)iterator
  //   end()->(const_)iterator
  //   cbegin()->const_iterator
  //   cend()->const_iterator
  //
  //   max_size()->size_type
  //   size()->size_type
  //   empty()->bool
  //
  //   [](size_type)->(const_)reference
  //   nth(difference_type)->(const_)iterator
  //
  //   reverse_iterator
  //   const_reverse_iterator
  //   rbegin()
  //   rend()
  //   crbegin()
  //   crend()
  //
  //   smf:
  //     default constructible
  //     movable
  //
  //   allocator_type
  //   get_allocator()->allocator_type
  //
  //   explicit window_tree_vector(const allocator_type &)
  //   explicit window_tree_vector(allocator_arg_t, const allocator_type &)
  //
  //   clear()
  //
  //   swap(const_iterator, const_iterator)
  //   swap(const_iterator, tree_type ref, const_iterator)
  //   swap(const_iterator, vector_type ref, const_iterator)
  //
  //   extract(const_iterator)->tree_type
  //   insert(const_iterator, tree_type &&)->iterator
  //   exchange(const_iterator, tree_type &&)->tree_type
  //   replace(const_iterator, tree_type &&)->iterator
  //
  //   extract(const_iterator, const_iterator)->vector_type
  //   insert(const_iterator, tree_type &&)->iterator
  //   exchange(const_iterator, const_iterator, vector_type &&)->vector_type
  //   replace(const_iterator, const_iterator, vector_type &&)->iterator
  //
  //   extract_children(const_iterator)->vector_type
  //   exchange_children(const_iterator, vector_type &&)->vector_type
  //   replace_children(const_iterator, vector_type &&)->iterator
  //
  //   splice(const_iterator, tree_type ref, const_iterator)
  //   splice(const_iterator, tree_type ref, const_iterator, const_iterator)
  //   splice(const_iterator, vector_type ref, const_iterator)
  //   splice(const_iterator, vector_type ref, const_iterator, const_iterator)
  //
  //   splice(const_iterator, vector_type ref)
  //
  //   emplace<T>(const_iterator, s...)->T *
  //
  //   erase(const_iterator)->iterator
  //   erase(const_iterator, const_iterator)->iterator
  //   clear(const_iterator)
  //
  //   emplace_back<T>(s...)->T *
  //   push_back(tree_type &&)
  //   push_back(vector_type &&)
  //   append(s...) &->this_t &
  //   append(s...) &&->this_t &&
  //   pop_back(size_type = 1u)
  //   emplace_back<T>(const_iterator, s...)->T *
  //   push_back(const_iterator, tree_type &&)
  //   push_back(const_iterator, vector_type &&)
  //   append(const_iterator, s...)
  //   pop_back(const_iterator, size_type = 1u)
  //
  //   emplace_front<T>(s...)->T *
  //   push_front(tree_type &&)
  //   push_front(vector_type &&)
  //   prepend(s...) &->this_t &
  //   prepend(s...) &&->this_t &&
  //   pop_front(size_type = 1u)
  //   emplace_front<T>(const_iterator, s...)->T *
  //   push_front(const_iterator, tree_type &&)
  //   push_front(const_iterator, vector_type &&)
  //   prepend(const_iterator, s...)
  //   pop_front(const_iterator, size_type = 1u)
  //
  //   capacity()->size_type
  //   capacity(const_iterator)->size_type
  //   full()->bool
  //   full(const_iterator)->bool
  //   reserve(size_type)
  //   reserve(const_iterator, size_type)
  //   first_order_reserve(size_type)
  //   first_order_reserve(const_iterator, size_type)
  //   reserve_more(size_type)
  //   reserve_more(const_iterator, size_type)
  //   first_order_reserve_more(size_type)
  //   first_order_reserve_more(const_iterator, size_type)
  //   shrink_to_fit()
  //   shrink_to_fit(const_iterator)
  //   first_order_shrink_to_fit()
  //   first_order_shrink_to_fit(const_iterator)
}
void doc_window_controller() {
  // window_controller
  //   smf: only destructible
  //
  //   explicit window_controller(window_tree &)
  //
  //   wins()->window_tree &
  //
  //   keys()->const key_recorder &
  //   cursor_position()->ivec2
  //
  //   mouse_leave()
  //     // make mouse_in() return false
  //     // auto called by window_program when some system message is received
  //   mouse_enter()
  //     // make mouse_in() return true
  //     // auto called by window_program when some system message is received
  //   mouse_entered()->bool
  //     // return true after mouse_enter()
  //     // return false after mouse_leave() is called
  //
  //   focused_window()->window *
  //   key_focused_window()->window *
  //
  //   has_focus()->bool
  //   set_focus()
  //   unset_focus()
  //   set_focus(window *)
  //     // has_focus()
  //     // call after set_focus()
  //     // nullptr means no abstract window is focused
  //   clear_focus() // if has_focus(), call set_focus(nullptr)
  //
  //   set_hovered(window *)
  //     // work if has_hovered_tracing()
  //     // nullptr means unset_hovered()
  //   unset_hovered()
  //     // work if has_hovered_tracing()
  //
  //   hovering_locked()->bool
  //   hovering_lock()
  //   hovering_unlock()
  //
  //   press(key_code_t) // !hidden()
  //   release(key_code_t)
  //   rotate_wheel(int)
  //     // if the hovered window is not null
  //     //   if (!(w.rotate_wheel(n) && w.keys().rotate_wheel(n)))
  //     //     go upward
  //     //
  //     // note: if !w.has_keys(), stop if w.rotate_wheel(n)
  //   mouse_move(ivec2)
  //     // clear tooltip
  //     // update window over which the mouse arrow hovered
  //     // cancel mouse-leave situation
  //
  //   hovered_window()->window *
  //     // if tracing is not enabled, traverse to find it
  //   clear_hovered_window()
  //     // clear the cached hovered window
  //     // which is only used if has_hovered_tracing()
  //
  //   tooltip_countdown_finished()->bool
  //   update_tooltip_countdown()
  //   restart_tooltip_countdown()
  //
  //   refresh()
  //     // auto call for every paint time if has_auto_refreshing()
  //     // process:
  //     //   call update_tooltip_countdown()
  //     //   call refresh_when_focused() for focused_window() if not nullptr
  //     //   more things to be added
  //   has_auto_refreshing()->bool
  //   set_auto_refreshing()
  //   unset_auto_refreshing()
}
void doc_window_program() {
  // system_window
  //   smf: none
  //
  //   static has_console()->bool
  //   static start_time()->steady_clock::time_point
  //   static monitor_position()->ivec2
  //     // left-top corrdinates from system origin point
  //     // (left-top to right-bottom)
  //   static monitor_size()->ivec2
  //   static frame_position()->ivec2
  //     // left-top corrdinates from system origin point
  //     // (left-top to right-bottom)
  //   static frame_size()->ivec2
  //   static size()->ivec2
  //
  //   static resize(ivec2)
  //   static move_to(ivec2)
  //   static paint(const pixmap &)
  //
  //   static get()->system_window *
  //   static controller()->window_controller &
  //   static keys()->const key_recorder &
  //   static cursor_position()->ivec2
  //   static wins()->window_tree &
  //   static paint_wins(pixmap &)
  //
  // as_window_program<DERIVED>
  //   // DERIVED optional functions:
  //   //   create()
  //   //   paint()
  //   //   destroy()
  //   //   set_focus()
  //   //   unset_focus()
  //   //   press(unsigned)
  //   //   release(unsigned)
  //   //   rotate_wheel(int)
  //   //   mouse_leave()
  //   //   mouse_move(ivec2)
  //   //   resize(ivec2)
  //   //   button_font()->font * // may be nullptr
  //   //   mouse_arrow_size()->ivec2 // {25, 25} if undefined, can not be empty
  //
  // run_window_program<DERIVED>(console_on = false)->int
}
void doc_root_window() {
  // root_window : public window
  //   // set_keys() auto-called by constructor
  //   smf: only default constructible
  //   explicit root_window(uint32_t)
  //
  //   has_color() const noexcept->bool
  //   color() const noexcept->uint32_t // has_color()
  //   set_color(uint32_t) noexcept
  //   unset_color() noexcept
  //
  //   virtual move_to(ivec2) override
  //   virtual resize(ivec2) override
  //   virtual rectangle(irect2) override
  //
  //   protected virtual adjust_to_fit_parent() override
  //
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_colored_window() {
  // colored_window : public window
  //   smf: only destructible
  //
  //   explicit colored_window(uint32_t)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   reset(uint32_t)
  //
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_image_window() {
  // image_window : public window
  //   // size is dependent on the stored pixmap
  //
  //   smf : destructible
  //
  //   explicit image_window(pixmap)
  //   reset(pixmap)
  //   image(pixmap)
  //   image() const noexcept->const pixmap &
  //
  //   resize(ivec2) override
  //   rectangle(irect2) override
  //
  //   protected virtual void paint(pixmap &, exclusive_thread_pool &, bool)
}
void doc_frame_hook_box() {
  // frame_hook : public window
  //   // size keeps being the same as result of
  //   //   the size of the child adding a fixed-width frame
  //   // position has no dependence
  //   //
  //   // size change:
  //   //   1) change size of the child to fit the new size,
  //   //   2) change size to maintain frame line width
  //   //   3) set position of the child to the center
  //   // position change: free
  //
  //   smf: default-constructible
  //   explicit frame_hook(int wid)
  //   frame_hook(int wid, uint32_t clr)
  //   reset(int wid, uint32_t clr)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //
  //   protected virtual controls_children() const->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual no_overlap() const->bool
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
  //
  // frame_box : public frame_hook
  //   // position-size affairs are same as frame_hook
  //   //   but make full rectangle for parent
  //   //   if iter().parent()->controls_children()
  //   smf: default-constructible
  //   explicit frame_box(int wid)
  //   frame_box(int wid, uint32_t clr)
  //   reset(int wid, uint32_t clr)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
}
void doc_edge_hook() {
  // enum class basic_direction
  //   up
  //   down
  //   left
  //   right
  //
  // edge_hook : public window
  //   // size keeps being the same as the child
  //   // position keeps being what to put the child at a specified position
  //   //   on edge of the parent
  //   //
  //   // size change: do nothing
  //   // position change: do nothing
  //
  //   smf: destructible
  //
  //   edge_hook(basic_direction)
  //   edge_hook(basic_direction, double k_parent, double k_child)
  //     // k in [0.0, 1.0] denotes position
  //     // (from left to right or from bottom to top)
  //   void reset(basic_direction, double k_parent, double k_child)
  //
  //   virtual move_to(ivec2) override
  //   virtual resize(ivec2) override
  //   virtual rectangle(irect2) override
  //
  //   protected virtual controls_children() const override->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual no_overlap() const->bool override
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_center_hook() {
  // center_hook : public window
  //   // size keeps being the same as the child
  //   // position keeps being what to put the child to center of the parent
  //   //
  //   // size change: do nothing
  //   // position change: do nothing
  //
  //   smf: default-constructible
  //   virtual move_to(ivec2)
  //   virtual resize(ivec2) // do nothing
  //   virtual rectangle(irect2) // only change position
  //   protected virtual controls_children() const override->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual no_overlap() const->bool override
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_size_locked_hook() {
  // size_locked_hook : public window
  //   // size keeps being the same as the child but minmax-locked
  //   // position has no dependence
  //   //
  //   // size change: do nothing
  //   // position change: free
  //
  //   smf: default-constructible
  //   virtual resize(ivec2) // do nothing
  //   virtual rectangle(irect2) // only change position
  //   protected virtual controls_children() const override->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual no_overlap() const->bool override
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_label_tooltip() {
  // label_traits
  //   smf: full
  //
  //   align(basic_align)->this_t &
  //   frame_line_width(int)->this_t &
  //   color(uint32_t)->this_t &
  //   frame_color(uint32_t)->this_t &
  //   font_color(uint32_t)->this_t &
  //
  //   align()->basic_laign
  //   frame_line_width()->int
  //   color()->uint32_t
  //   frame_color()->uint32_t
  //   font_color()->uint32_t
  //
  // label
  //   default_size() const->ivec2
  //   set_default_size() // min-size is assigned by default_size()
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   smf: destructible
  //   label(font_ptr, sview, label_traits = {})
  //   explicit label(sview, label_traits = {})
  //   reset(font_ptr, sview, label_traits = {})
  //
  //   font() const noexcept->font &
  //   font(font_ptr)
  //   text() const->string
  //   text(sview)
  //
  //   align() const noexcept->basic_align
  //   align(basic_align)
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_color() const noexcept->uint32_t
  //   frame_color(uint32_t)
  //   font_color() const noexcept->uint32_t
  //   font_color(uint32_t)
  //
  //   protected paint(pixmap &, exclusive_thread_pool &, bool)
  //
  //
  // tooltip_traits
  //   smf: full
  //
  //   frame_line_width(int)
  //   color(uint32_t)
  //   frame_color(uint32_t)
  //   font_color(uint32_t)
  //
  //   frame_line_width()->int
  //   color()->uint32_t
  //   frame_color()->uint32_t
  //   font_color()->uint32_t
  //
  // tooltip
  //   smf: destructible
  //   tooltip(font_ptr, sview, label_traits = {})
  //   explicit tooltip(sview, label_traits = {})
  //   reset(font_ptr, sview, label_traits = {})
  //
  //   font() const noexcept->font &
  //   font(font_ptr)
  //   text() const->string
  //   text(sview)
  //
  //   align() const noexcept->basic_align
  //   align(basic_align)
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_color() const noexcept->uint32_t
  //   frame_color(uint32_t)
  //   font_color() const noexcept->uint32_t
  //   font_color(uint32_t)
  //
  //   clone() const->window_tree
  //
  //   protected paint(pixmap &, exclusive_thread_pool &, bool)
}
void doc_button() {
  // button_traits
  //   smf: full
  //
  //   align(basic_align)->this_t &
  //   frame_line_width(int)->this_t &
  //   color(int)->this_t &
  //   frame_color(int)->this_t &
  //   font_color(int)->this_t &
  //
  //   align()->basic_align
  //   frame_line_width()->int
  //   color()->uint32_t
  //   frame_color()->uint32_t
  //   font_color()->uint32_t
  //
  // button : public window
  //   default_size()
  //   set_default_size()
  //     // post condition:
  //     //   min_size is locked to the default size
  //     //   max_size is open
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   smf: destructible
  //
  //   button(font_ptr, pixmap, sview, button_traits = {})
  //   button(font_ptr, sview, button_traits = {})
  //   button(pixmap, sview, button_traits = {})
  //   explicit button(sview, button_traits = {})
  //
  //   // after any update may change rectangle info
  //   //   size is set to be the_default_size
  //   //   min_size is set to be the_default_size
  //   //   max_size is set to be max
  //
  //   reset(font_ptr, pixmap, sview, button_traits = {})
  //     // do not change function bindings
  //
  //   // every function to change geometric data ends with set_default_size()
  //
  //   image() const noexcept->const pixmap &
  //   image(pixmap)
  //   font() const noexcept->font &
  //   font(font_ptr)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //   text() const->string
  //   text(sview)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //
  //   align()->basic_align
  //   align(basic_align)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_color() const noexcept->uint32_t
  //   frame_color(uint32_t)
  //   font_color() const noexcept->uint32_t
  //   font_color(uint32_t)
  //
  //   pressed() const noexcept->bool
  //
  //   bind_press(f)
  //   bound_press() const noexcept->bool
  //   unbind_press()
  //   bind_double_press(f)
  //   bound_double_press() const noexcept->bool
  //   unbind_double_press()
  //   bind_release_pressed(f)
  //   bound_release_pressed() const noexcept->bool
  //   unbind_release_pressed()
  //   clear_callback_functions()
  //
  //   protected virtual set_focus() override
  //   protected virtual unset_focus() override
  //
  //   protected virtual unset_hovered() override
  //
  //   virtual lock() override
  //
  //   protected virtual press(key_code_t) override
  //   protected virtual release(key_code_t) override
  //
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_check_button() {
  // check_button_traits
  //   smf: full
  //
  //   align(basic_align)->this_t &
  //   frame_line_width(int)->this_t &
  //   color(uint32_t)->this_t &
  //   frame_color(uint32_t)->this_t &
  //   font_color(uint32_t)->this_t &
  //   box_color(uint32_t)->this_t &
  //
  //   align()->basic_align
  //   frame_line_width()->int
  //   color()->uint32_t
  //   frame_color()->uint32_t
  //   font_color()->uint32_t
  //   box_color()->uint32_t
  //
  // check_button : public button
  //   default_size()
  //   set_default_size()
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   operator bool() const noexcept
  //   on() const noexcept->bool
  //   set(bool = true) noexcept // omit exclusive set limit
  //   unset() noexcept // omit exclusive set limit
  //
  //   smf: destructible
  //
  //   check_button(font_ptr, sview, check_button_traits = {})
  //   explicit check_button(sview, check_button_traits = {})
  //
  //   // after any update may change rectangle info
  //   //   size is set to be the_default_size
  //   //   min_size is set to be the_default_size
  //   //   max_size is set to be max
  //
  //   reset(font_ptr, sview, check_button_traits = {})
  //
  //   font() const noexcept->font &
  //   font(font_ptr)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //   text() const->string
  //   text(sview)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //
  //   align()->basic_align
  //   align(basic_align)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //     // call set_default_size()
  //     // press-realted info is reset
  //     // call no callback function
  //
  //   box_color() const noexcept->uint32_t
  //   box_color(uint32_t)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_color() const noexcept->uint32_t
  //   frame_color(uint32_t)
  //   font_color() const noexcept->uint32_t
  //   font_color(uint32_t)
  //
  //   bind_press(f)
  //   bound_press() const noexcept->bool
  //   unbind_press()
  //   bind_double_press(f)
  //   bound_double_press() const noexcept->bool
  //   unbind_double_press()
  //   bind_release(f)
  //   bound_release() const noexcept->bool
  //   unbind_release()
  //
  //   make_set(n_at_most, window *, ...)
  //     // call clear_set() then make a new set, n > 0
  //     // window * is cast to check_button *
  //     // one check_button can only join one set
  //   make_set_from_range(n, rng_of_window_ptr)
  //     // range value is window * to be cast to check_button *
  //   has_set()->bool
  //   count_of_set()->int
  //   limit_of_set()->int
  //   clear_set() // post-condition: !has_set() for all buttons in the set
  //
  //   protected virtual set_focus() override
  //   protected virtual unset_focus() override
  //
  //   protected virtual unset_hovered() override
  //
  //   virtual lock() override
  //
  //   protected virtual press(key_code_t) override
  //   protected virtual release(key_code_t) override
  //
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_hbox_vbox() {
  // h_space : public window // with fixed width
  // v_space : public window // with fixed height
  //   smf: default-constructible
  //
  //   constructor(int len)
  //   constructor(int len, uint32_t clr)
  //   reset(int len, uint32_t clr)
  //
  //   int length() const noexcept
  //   void length(int)
  //   uint32_t color() const noexcept
  //   void color(uint32_t)
  //
  //   protected paint(pixmap &, exclusive_thread_pool &, bool) override
  //
  //
  // hbox : public window
  //   // position-size is dependent on children and the parent
  //   //   1) fit children
  //   //   2) if the parent returns false on controls_children(),
  //   //      fill the full rectangle of the parent
  //   //
  //   // position change:
  //   //   free if the parent returns false on controls_children()
  //   // size change:
  //   //   max-size is not limited but adjust() always reset size
  //   //     when fit the children

  //   smf: default-constructible
  //   hbox(basic_align)
  //   hbox(basic_align, bool v_full)
  //   hbox(basic_align, bool v_full, uint32_t clr)
  //   void reset(basic_align, bool v_full, uint32_t clr)
  //
  //   align() const noexcept->basic_align
  //   align(basic_align)
  //   v_full() const noexcept->bool
  //   v_full(bool)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   stretch(window &, int)
  //   stretch(window &)->int
  //
  //   protected virtual controls_children() const->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual init_new_child(window_iterator) override
  //   protected virtual no_overlap() const override->bool
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
  //
  // vbox : public window
  //   // position-size is dependent on children and the parent
  //   //   1) fit children
  //   //   2) if the parent returns false on controls_children(),
  //   //      fill the full rectangle of the parent
  //   //
  //   // position change:
  //   //   free if the parent returns false on controls_children()
  //   // size change:
  //   //   max-size is not limited but adjust() always reset size
  //   //     when fit the children
  //
  //   smf: default-constructible
  //   vbox(basic_v_align)
  //   vbox(basic_v_align, bool h_full)
  //   vbox(basic_v_align, bool h_full, uint32_t clr)
  //   void reset(basic_v_align, bool h_full, uint32_t clr)
  //
  //   align() const noexcept->basic_v_align
  //   align(basic_v_align)
  //   h_full() const noexcept->bool
  //   h_full(bool)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   stretch(window &, int)
  //   stretch(window &)->int
  //
  //   protected virtual controls_children() const->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual init_new_child(window_iterator) override
  //   protected virtual no_overlap() const override->bool
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_grid_box() {
  // grid_box_traits
  //   smf: full
  //
  //   color(uint32_t)->this_t &
  //   frame_line_width(int)->this_t &
  //   line_width(int)->this_t &
  //   set_h_full(bool = true)->this_t &
  //   unset_h_full()->this_t &
  //   set_v_full(bool = true)->this_t &
  //   unset_v_full()->this_t &
  //   h_align(basic_align)->this_t &
  //   v_align(basic_v_align)->this_t &
  //
  //   color()->uint32_t
  //   frame_line_width()->int
  //   line_width()->int
  //   h_full()->bool
  //   v_full()->bool
  //   h_align()->basic_align
  //   v_align()->basic_v_align
  //
  // grid_box
  //   // position-size is dependent on children and the parent
  //   //   1) fit children
  //   //   2) if the parent returns false on controls_children(),
  //   //      fill the full rectangle of the parent
  //   //
  //   // position change:
  //   //   free if the parent returns false on controls_children()
  //   // size change:
  //   //   max-size is not limited but adjust() always reset size
  //   //     when fit the children
  //
  //   smf: destructible
  //
  //   grid_box(int w, int h, grid_box_traits = {})
  //   grid_box(int w, int h,
  //            h_stretch_range, v_stretch_range,
  //            grid_box_traits = {})
  //   reset(int w, int h, grid_box_traits{})
  //   reset(int w, int h, h_stretch_range, v_stretch_range,
  //         grid_box_traits = {})
  //
  //   h_length(int) const->int
  //   v_length(int) const->int
  //
  //   h_stretch(int) const->int
  //   v_stretch(int) const->int
  //   h_stretch(int, int)
  //   v_stretch(int, int)
  //
  //   grid_width() const noexcept->int
  //   grid_width(int)
  //   grid_height() const noexcept->int
  //   grid_height(int)
  //   color() const noexcept->uint32_t
  //   color(uint32_t)
  //   frame_line_width() const noexcept->int
  //   frame_line_width(int)
  //   line_width() const noexcept->int // space between two adjacent cells
  //   line_width(int)
  //   h_full() const noexcept->bool
  //   set_h_full(bool = true)
  //   unset_h_full()
  //   v_full() const noexcept->bool
  //   set_v_full(bool = true)
  //   unset_v_full()
  //   h_align() const noexcept->basic_align
  //   h_align(basic_align)
  //   v_align() const noexcept->basic_v_align
  //   v_align(basic_v_align)
  //
  //   protected virtual controls_children() const->bool
  //   protected virtual adjust_to_fit_children() override
  //   protected virtual adjust_to_fit_parent() override
  //   protected virtual adjust_children_to_fit_this() override
  //   protected virtual no_overlap() const override->bool
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_v_progress_bar() {
  // progress_bar_traits
  // v_progress_bar_traits
  //   smf: full
  //
  //   frame_line_width()->int
  //   groove_line_width()->int
  //   frame_color()->uint32_t
  //   groove_color()->uint32_t
  //   color()->uint32_t
  //
  //   frame_line_width(int)->this_t &
  //   groove_line_width(int)->this_t &
  //   frame_color(uint32_t)->this_t &
  //   groove_color(uint32_t)->this_t &
  //   color(uint32_t)->this_t &
  //
  // progress_bar
  // v_progress_bar
  //   set_default_size()
  //   default_size() const->ivec2
  //
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   smf: default-constructible
  //
  //   explicit progress_bar(double, {v_}progress_bar_traits = {})
  //   explicit progress_bar({v_}progress_bar_traits)
  //   reset(double, {v_}progress_bar_traits = {})
  //
  //   frame_line_width() const noexcept->int
  //   groove_line_width() const noexcept->int
  //   frame_color() const noexcept->uint32_t
  //   groove_color() const noexcept->uint32_t
  //   color() const noexcept->uint32_t
  //
  //   frame_line_width(int)
  //     // call set_default_size()
  //     // value() is not changed
  //   groove_line_width(int)
  //     // call set_default_size()
  //     // value() is not changed
  //   frame_color(uint32_t)
  //   groove_color(uint32_t)
  //   color(uint32_t)
  //
  //   value() const noexcept->double
  //   value(double);
  //
  //   max_pixel_value() const->int
  //   max_pixel_value(int) // only call resize
  //   groove_length() const->int // same as max_pixel_value()
  //   groove_length(int) // same as max_pixel_value(i)
  //
  //   protected virtual paint(pixmap &,
  //                           exclusive_thread_pool &, bool) override;
}
void doc_v_slider_bar() {
  // slide_bar_traits
  // v_slide_bar_traits
  //   smf: full
  //
  //   frame_line_width()->int
  //   groove_line_width()->int
  //   slider_length()->int
  //   relaxed_hovering()->bool
  //   frame_color()->uint32_t
  //   groove_color()->uint32_t
  //   color()->uint32_t
  //
  //   frame_line_width(int)->this_t &
  //   groove_line_width(int)->this_t &
  //   slider_length(int)->this_t &
  //   set_relaxed_hovering(bool = true)->this_t &
  //   unset_relaxed_hovering()->this_t &
  //   frame_color(uint32_t)->this_t &
  //   groove_color(uint32_t)->this_t &
  //   color(uint32_t)->this_t &
  //
  // slide_bar
  // v_slide_bar
  //   set_default_size()
  //   default_size() const->ivec2
  //
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   smf: default-constructible
  //
  //   explicit {v_}slide_bar(double, {v_}slide_bar_traits = {})
  //   explicit {v_}slide_bar({v_}slide_bar_traits)
  //   reset(double, {v_}slide_bar_traits = {})
  //
  //   frame_line_width() const noexcept->int
  //   groove_line_width() const noexcept->int
  //   slider_length() const noexcept->int
  //   relaxed_hovering() const noexcept->bool
  //   frame_color() const noexcept->uint32_t
  //   groove_color() const noexcept->uint32_t
  //   color() const noexcept->uint32_t
  //
  //   frame_line_width(int)
  //     // call set_default_size()
  //     // press-related info is reset and call no callback function
  //     // value() is unchanged
  //   groove_line_width(int)
  //     // call set_default_size()
  //     // press-related info is reset and call no callback function
  //     // value() is unchanged
  //   slider_length(int)
  //     // call set_default_size()
  //     // press-related info is reset and call no callback function
  //     // value() is unchanged
  //   set_relaxed_hovering(bool = true)
  //   unset_relaxed_hovering()
  //   frame_color(uint32_t)
  //   groove_color(uint32_t)
  //   color(uint32_t)
  //
  //   groove_length() const noexcept->int
  //   groove_length(int) // only call resize
  //
  //   value() const noexcept->double
  //   value(double)
  //   assign_value(double) // same as value(double) but no callback
  //
  //   max_pixel_value() const->int
  //   max_pixel_value(int) // only call resize
  //   pixel_value() const noexcept->int
  //   pixel_value(int)
  //   assign_pixel_value(int) // same as pixel_value(int) but no callback
  //
  //   bind_value_change(f)
  //   bound_value_change() const noexcept->bool
  //   bind_value_change_after_release(f)
  //   bound_value_change_after_release() const noexcept->bool
  //
  //   protected virtual set_focus() override
  //   protected virtual unset_focus() override
  //
  //   protected virtual unset_hovered() override
  //
  //   protected virtual mouse_move(ivec2)
  //
  //   virtual lock() override
  //
  //   protected press(key_code_t) override
  //   protected virtual release(key_code_t) override
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}
void doc_h_scroll_bar_v_scroll_bar() {
  // inner::fns::get_arrow_button_grey_map(basic_direction, int, int)
  //   ->const dup_compressed_array<uint8_t> &
  //
  // h_scroll_bar_traits
  // v_scroll_bar_traits
  //   frame_line_width()->int
  //   groove_line_width()->int
  //   slider_length()->int
  //   arrow_length()->int
  //   step()->double
  //   max_step()->double
  //   frame_color()->uint32_t
  //   groove_color()->uint32_t
  //   color()->uint32_t
  //   arrow_color()->uint32_t
  //   arrow_button_color()->uint32_t
  //
  //   frame_line_width(int)->this_t &
  //   groove_line_width(int)->this_t &
  //   slider_length(int)->this_t &
  //   arrow_length(int)->this_t &
  //   step(double)->this_t &
  //   max_step(double)->this_t &
  //   frame_color(uint32_t)->this_t &
  //   groove_color(uint32_t)->this_t &
  //   color(uint32_t)->this_t &
  //   arrow_color(uint32_t)->this_t &
  //   arrow_button_color(uint32_t)->this_t &
  //
  //
  // h_scroll_bar
  // v_scroll_bar
  //   set_default_size()
  //   default_size() const->ivec2
  //
  //   lock_width()
  //   lock_height()
  //   lock_size()
  //   unlock_width()
  //   unlock_height()
  //   unlock_size()
  //
  //   smf: default-constructible
  //
  //   explicit {v_}scroll_bar(double, {v_}scroll_bar_traits = {})
  //   explicit {v_}scroll_bar({v_}scroll_bar_traits)
  //   reset(double, {v_}scroll_bar_traits = {})
  //
  //   frame_line_width() const noexcept->int
  //   groove_line_width() const noexcept->int
  //   slider_length() const noexcept->int
  //   arrow_length() const noexcept->int
  //   step() const noexcept->double // move distance of every pressed interval
  //   max_step() const noexcept->double
  //   frame_color() const noexcept->uint32_t
  //   groove_color() const noexcept->uint32_t
  //   color() const noexcept->uint32_t
  //   arrow_color() const noexcept->uint32_t
  //   arrow_button_color() const noexcept->uint32_t
  //
  //   frame_line_width(int);
  //   groove_line_width(int);
  //   slider_length(int);
  //   arrow_length(int);
  //   step(double);
  //   max_step(double);
  //   frame_color(uint32_t);
  //   groove_color(uint32_t);
  //   color(uint32_t);
  //   arrow_color(uint32_t);
  //   arrow_button_color(uint32_t);
  //
  //   groove_length() const noexcept->int
  //   groove_length(int)
  //
  //   value() const noexcept->double
  //   value(double)
  //   assign_value(double)
  //
  //   max_pixel_value() const->int
  //   max_pixel_value(int) // only call resize
  //   pixel_value() const noexcept->int
  //   pixel_value(int)
  //   assign_pixel_value(int) // same as pixel_value(int) but no callback
  //
  //   bind_value_change(f)
  //   bound_value_change() const noexcept->bool
  //   bind_value_change_after_release(f)
  //   bound_value_change_after_release() const noexcept->bool
  //
  //   protected virtual set_focus() override
  //   protected virtual unset_focus() override
  //
  //   protected virtual unset_hovered() override
  //
  //   protected virtual mouse_move(ivec2)
  //
  //   virtual lock() override
  //
  //   protected virtual refresh_when_focused() override
  //   protected press(key_code_t) override
  //   protected virtual release(key_code_t) override
  //   protected virtual paint(pixmap &, exclusive_thread_pool &, bool) override
}

}

int main() {
  re::inner::test::doc_base();
  re::inner::test::doc_simple_put_functions();
  re::inner::test::doc_std_dependence();
  re::inner::test::doc_std_dependence_fpfns();
  re::inner::test::doc_print_error();
  re::inner::test::doc_basic_template_metaprogramming_tools();
  re::inner::test::doc_swap();
  re::inner::test::doc_type_traits();
  re::inner::test::doc_basic_utility_functions();
  re::inner::test::doc_pointer_traits();
  re::inner::test::doc_reference_wrapper_and_invoke();
  re::inner::test::doc_language_related_concepts();
  re::inner::test::doc_concept_constrained_comparisons();
  re::inner::test::doc_integral_traits();
  re::inner::test::doc_numeric_limits();
  re::inner::test::doc_three_way_comparison();
  re::inner::test::doc_type_index();
  re::inner::test::doc_basic_function_objects();
  re::inner::test::doc_tuple();
  re::inner::test::doc_bind();
  re::inner::test::doc_ratio();
  re::inner::test::doc_optional();
  re::inner::test::doc_semiregular_function();
  re::inner::test::doc_hash();
  re::inner::test::doc_miscl();
  re::inner::test::doc_floating_point_traits();
  re::inner::test::doc_common_math();

  re::inner::test::doc_owner_ptr();
  re::inner::test::doc_test_throwing();
  re::inner::test::doc_test_ownership();
  re::inner::test::doc_test_rational_operator();
  re::inner::test::doc_instance_counter();
  re::inner::test::doc_ez_dynamic();
  re::inner::test::doc_ez_function();
  re::inner::test::doc_ez_vector();
  re::inner::test::doc_ez_forward_list();
  re::inner::test::doc_ez_slist();
  re::inner::test::doc_ez_bidirectional_list();
  re::inner::test::doc_ez_list();
  re::inner::test::doc_ez_map();
  re::inner::test::doc_ez_mutex();
  re::inner::test::doc_test_allocator();
  re::inner::test::doc_test_range();

  re::inner::test::doc_iterator_requirements();
  re::inner::test::doc_iterator_main_components();
  re::inner::test::doc_range_main_components();
  re::inner::test::doc_degraded_iterator();
  re::inner::test::doc_reverse_iterator();
  re::inner::test::doc_insert_iterator();
  re::inner::test::doc_move_iterator();
  re::inner::test::doc_counted_iterator();
  re::inner::test::doc_iterator_wrapper();
  re::inner::test::doc_array();
  re::inner::test::doc_iter_pair();
  re::inner::test::doc_composite_range();
  re::inner::test::doc_range_wrapper();
  re::inner::test::doc_base_range();
  re::inner::test::doc_empty_range();
  re::inner::test::doc_single_range();
  re::inner::test::doc_counted_range();
  re::inner::test::doc_degraded_range();
  re::inner::test::doc_move_range();
  re::inner::test::doc_reverse_range();
  re::inner::test::doc_rng_for_iterator_n();
  re::inner::test::doc_rng_for_n_value();
  re::inner::test::doc_iterator_range();
  re::inner::test::doc_bind_iterator();
  re::inner::test::doc_iters();
  re::inner::test::doc_range_miscl();

  re::inner::test::doc_allocator_traits();
  re::inner::test::doc_uninitialized_argo();
  re::inner::test::doc_default_allocator();
  re::inner::test::doc_allocator_wrapper();
  re::inner::test::doc_unique_ptr();
  re::inner::test::doc_unique_array();
  re::inner::test::doc_copyable_ptr();
  re::inner::test::doc_copyable_array();
  re::inner::test::doc_maybe_owner_ptr();
  re::inner::test::doc_buffer();
  re::inner::test::doc_scoped_allocator_adaptor();
  re::inner::test::doc_allocator_aware_container_ownership();
  re::inner::test::doc_object_pool();
  re::inner::test::doc_raw_object_pool();
  re::inner::test::doc_memory_pool();

  re::inner::test::doc_dynamic_base();
  re::inner::test::doc_dynamic();
  re::inner::test::doc_dynamic_void();
  re::inner::test::doc_function();
  re::inner::test::doc_optional();
  re::inner::test::doc_move_only_function();
  re::inner::test::doc_unique_function();
  re::inner::test::doc_type_erased_invocation();
  re::inner::test::doc_variant();

  re::inner::test::doc_random();

  re::inner::test::doc_equal();
  re::inner::test::doc_allanynone_of();
  re::inner::test::doc_for_each();
  re::inner::test::doc_find();
  re::inner::test::doc_find_last();
  re::inner::test::doc_find_first_last_of();
  re::inner::test::doc_adjacent_find();
  re::inner::test::doc_count();
  re::inner::test::doc_mismatch();
  re::inner::test::doc_is_permutation();
  re::inner::test::doc_find_subrange();
  re::inner::test::doc_search();
  re::inner::test::doc_find_end();
  re::inner::test::doc_contains();
  re::inner::test::doc_starts_ends_with();
  re::inner::test::doc_fold_left_right();
  re::inner::test::doc_copy_move_if_backward_from();
  re::inner::test::doc_swap_ranges();
  re::inner::test::doc_transform();
  re::inner::test::doc_replace_if_copy_if();
  re::inner::test::doc_fill_generate();
  re::inner::test::doc_remove_if_copy_if();
  re::inner::test::doc_unique_copy();
  re::inner::test::doc_reverse_copy();
  re::inner::test::doc_rotate_copy();
  re::inner::test::doc_shift();
  re::inner::test::doc_sample();
  re::inner::test::doc_shuffle();
  re::inner::test::doc_binary_search_series();
  re::inner::test::doc_partition_series();
  re::inner::test::doc_merge_series();
  re::inner::test::doc_set_series();
  re::inner::test::doc_heap_series();
  re::inner::test::doc_min_max();
  re::inner::test::doc_clamp();
  re::inner::test::doc_lexicographical_compare();
  re::inner::test::doc_permutation();
  re::inner::test::doc_sort_series();
  re::inner::test::doc_accumulate_reduce();
  re::inner::test::doc_inner_product();
  re::inner::test::doc_partial_sum_series();
  re::inner::test::doc_adjacent_difference();
  re::inner::test::doc_iota();
  re::inner::test::doc_gcd_and_lcm();
  re::inner::test::doc_midpoint();
  re::inner::test::doc_for_each_node();
  re::inner::test::doc_list_fns();
  re::inner::test::doc_list_unique();
  re::inner::test::doc_combination_range();
  re::inner::test::doc_filter_range();
  re::inner::test::doc_take_range();
  re::inner::test::doc_drop_range();
  re::inner::test::doc_take_while_range();
  re::inner::test::doc_drop_while_range();
  re::inner::test::doc_join_range();
  re::inner::test::doc_join_with_range();
  re::inner::test::doc_adjacent_range();
  re::inner::test::doc_slide_range();
  re::inner::test::doc_aligned_stride_range();
  re::inner::test::doc_stride_range();
  re::inner::test::doc_aligned_chunk_range();
  re::inner::test::doc_chunk_range();
  re::inner::test::doc_chunk_by_range();
  re::inner::test::doc_inner_cartesian_product_range();
  re::inner::test::doc_cartesian_product_range();
  re::inner::test::doc_split_range();
  re::inner::test::doc_zip_range();
  re::inner::test::doc_aligned_zip_range();
  re::inner::test::doc_enumerate_range();
  re::inner::test::doc_exclusive_rotate_range();
  re::inner::test::doc_rotate_range();
  re::inner::test::doc_loop_range();

  re::inner::test::doc_string();
  re::inner::test::doc_string_reference();
  re::inner::test::doc_bitset();
  re::inner::test::doc_vector();
  re::inner::test::doc_bool_vector();
  re::inner::test::doc_local_vector();
  re::inner::test::doc_small_vector();
  re::inner::test::doc_pointer_vector();
  re::inner::test::doc_circular_vector();
  re::inner::test::doc_deque();
  re::inner::test::doc_forward_list();
  re::inner::test::doc_list();
  re::inner::test::doc_queue();
  re::inner::test::doc_stack();
  re::inner::test::doc_priority_queue();
  re::inner::test::doc_flat_map();
  re::inner::test::doc_binary_search_tree_adaptor();
  re::inner::test::doc_map_adaptor();
  re::inner::test::doc_map_typedef();
  re::inner::test::doc_hashtable_adaptor();
  re::inner::test::doc_unordered_map_adaptor();
  re::inner::test::doc_unordered_map_typedef();
  re::inner::test::doc_mixed_map_adaptor();
  re::inner::test::doc_mixed_map_typedef();
  re::inner::test::doc_stable_vector_adaptor();
  re::inner::test::doc_stable_vector_typedef();
  re::inner::test::doc_tree_adaptor();
  re::inner::test::doc_tree_vector_adaptor();
  re::inner::test::doc_tree_typedef();
  re::inner::test::doc_matrix();
  re::inner::test::doc_dup_compressed_array();

  re::inner::test::doc_duration();
  re::inner::test::doc_time_point();
  re::inner::test::doc_clock();

  re::inner::test::doc_c_file();
  re::inner::test::doc_print_tag();
  re::inner::test::doc_sscan();
  re::inner::test::doc_sprint();
  re::inner::test::doc_big_int();
  re::inner::test::doc_ssplitter();
  re::inner::test::doc_file();

  re::inner::test::doc_atomic();
  re::inner::test::doc_weak_ptr();
  re::inner::test::doc_shared_ptr();
  re::inner::test::doc_thread();
  re::inner::test::doc_mutex();
  re::inner::test::doc_mutex_area();
  re::inner::test::doc_shared_mutex();
  re::inner::test::doc_thread_pool();
  re::inner::test::doc_exclusive_thread_pool();

  re::inner::test::doc_key_code();
  re::inner::test::doc_key_comb();
  re::inner::test::doc_key_recorder();
  re::inner::test::doc_key_mapper();
  re::inner::test::doc_vector_math();
  re::inner::test::doc_rgba();
  re::inner::test::doc_irect2();
  re::inner::test::doc_pixmap();
  re::inner::test::doc_font_glyph();
  re::inner::test::doc_basic_window_program();
  re::inner::test::doc_window_paint_traits_default_affairs();
  re::inner::test::doc_window();
  re::inner::test::doc_window_tree();
  re::inner::test::doc_window_tree_vector();
  re::inner::test::doc_window_controller();
  re::inner::test::doc_window_program();
  re::inner::test::doc_root_window();
  re::inner::test::doc_colored_window();
  re::inner::test::doc_image_window();
  re::inner::test::doc_frame_hook_box();
  re::inner::test::doc_edge_hook();
  re::inner::test::doc_center_hook();
  re::inner::test::doc_size_locked_hook();
  re::inner::test::doc_label_tooltip();
  re::inner::test::doc_button();
  re::inner::test::doc_check_button();
  re::inner::test::doc_hbox_vbox();
  re::inner::test::doc_grid_box();
  re::inner::test::doc_v_progress_bar();
  re::inner::test::doc_v_slider_bar();
  re::inner::test::doc_h_scroll_bar_v_scroll_bar();

  printf("ok\n");
}
