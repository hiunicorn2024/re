// 17 Language support library

// 17.2.1 <cstddef>
namespace std {

using ptrdiff_t = see below ; // using
using size_t = see below ; // using
using max_align_t = see below ; // using
using nullptr_t = decltype(nullptr); // using
enum class byte : unsigned char {}; // using

// 17.2.5, byte type operations
template<class IntType>
constexpr byte& operator<<=(byte& b, IntType shift) noexcept; // using
template<class IntType>
constexpr byte operator<<(byte b, IntType shift) noexcept; // using
template<class IntType>
constexpr byte& operator>>=(byte& b, IntType shift) noexcept; // using
template<class IntType>
constexpr byte operator>>(byte b, IntType shift) noexcept; // using
constexpr byte& operator|=(byte& l, byte r) noexcept; // using
constexpr byte operator|(byte l, byte r) noexcept; // using
constexpr byte& operator&=(byte& l, byte r) noexcept; // using
constexpr byte operator&(byte l, byte r) noexcept; // using
constexpr byte& operator^=(byte& l, byte r) noexcept; // using
constexpr byte operator^(byte l, byte r) noexcept; // using
constexpr byte operator~(byte b) noexcept; // using
template<class IntType>
constexpr IntType to_integer(byte b) noexcept; // using

}
{
#define NULL see below
#define offsetof(P, D) see below
}

// 17.2.2 Header <cstdlib> synopsis
namespace std {
using size_t = see below ; // using
using div_t = see below ;
using ldiv_t = see below ;
using lldiv_t = see below ;
}
{
#define NULL see below
#define EXIT_FAILURE see below
#define EXIT_SUCCESS see below
#define RAND_MAX see below
#define MB_CUR_MAX see below
}
namespace std {
// Exposition-only function type aliases
extern "C" using c-atexit-handler = void(); // exposition only
extern "C++" using atexit-handler = void(); // exposition only
extern "C" using c-compare-pred
  = int(const void*, const void*); // exposition only
extern "C++" using compare-pred
  = int(const void*, const void*); // exposition only
// 17.5, start and termination
[[noreturn]] void abort() noexcept; // using
int atexit(c-atexit-handler * func) noexcept;
int atexit(atexit-handler * func) noexcept;
int at_quick_exit(c-atexit-handler * func) noexcept;
int at_quick_exit(atexit-handler * func) noexcept;
[[noreturn]] void exit(int status); // using
[[noreturn]] void _Exit(int status) noexcept;
[[noreturn]] void quick_exit(int status) noexcept;
char* getenv(const char* name); // using
int system(const char* string); // using
// 20.2.11, C library memory allocation
void* aligned_alloc(size_t alignment, size_t size);
void* calloc(size_t nmemb, size_t size);
void free(void* ptr); // using
void* malloc(size_t size); // using
void* realloc(void* ptr, size_t size);
double atof(const char* nptr);
int atoi(const char* nptr);
long int atol(const char* nptr);
long long int atoll(const char* nptr);
double strtod(const char* nptr, char** endptr);
float strtof(const char* nptr, char** endptr);
long double strtold(const char* nptr, char** endptr);
long int strtol(const char* nptr, char** endptr, int base);
long long int strtoll(const char* nptr, char** endptr, int base);
unsigned long int strtoul(const char* nptr, char** endptr, int base);
unsigned long long int strtoull(const char* nptr, char** endptr, int base);
// 23.5.6, multibyte / wide string and character conversion functions
int mblen(const char* s, size_t n);
int mbtowc(wchar_t* pwc, const char* s, size_t n);
int wctomb(char* s, wchar_t wchar);
size_t mbstowcs(wchar_t* pwcs, const char* s, size_t n);
size_t wcstombs(char* s, const wchar_t* pwcs, size_t n);
// 27.12, C standard library algorithms
void* bsearch(const void* key, const void* base, size_t nmemb,
              size_t size, c-compare-pred * compar);
void* bsearch(const void* key, const void* base, size_t nmemb, size_t size,
              compare-pred * compar);
void qsort(void* base, size_t nmemb, size_t size, c-compare-pred * compar);
void qsort(void* base, size_t nmemb, size_t size, compare-pred * compar);
// 28.5.10, low-quality random number generation
int rand();
void srand(unsigned int seed);
// 28.7.2, absolute values
constexpr int abs(int j);
constexpr long int abs(long int j);
constexpr long long int abs(long long int j);
constexpr float abs(float j);
constexpr double abs(double j);
constexpr long double abs(long double j);
constexpr long int labs(long int j);
constexpr long long int llabs(long long int j);
constexpr div_t div(int numer, int denom);
constexpr ldiv_t div(long int numer, long int denom); // see 16.2
constexpr lldiv_t div(long long int numer, long long int denom); // see 16.2
constexpr ldiv_t ldiv(long int numer, long int denom);
constexpr lldiv_t lldiv(long long int numer, long long int denom);
}

// 17.3.2 Header <version> synopsis
{
#define __cpp_lib_addressof_constexpr 201603L // also in <memory>
#define __cpp_lib_allocate_at_least 202106L // also in <memory>
#define __cpp_lib_allocator_traits_is_always_equal 201411L //
// also in <memory>, <scoped_allocator>, <string>, <deque>, <forward_list>,
// <list>, <vector>, <map>, <set>, <unordered_map>, <unordered_set>
#define __cpp_lib_adaptor_iterator_pair_constructor 202106L //
// also in <stack>, <queue>
#define __cpp_lib_any 201606L // also in <any>
#define __cpp_lib_apply 201603L // also in <tuple>
#define __cpp_lib_array_constexpr 201811L // also in <iterator>, <array>
#define __cpp_lib_as_const 201510L // also in <utility>
#define __cpp_lib_associative_heterogeneous_erasure 202110L
// also in <map>, <set>, <unordered_map>, <unordered_set>
#define __cpp_lib_assume_aligned 201811L // also in <memory>
#define __cpp_lib_atomic_flag_test 201907L // also in <atomic>
#define __cpp_lib_atomic_float 201711L // also in <atomic>
#define __cpp_lib_atomic_is_always_lock_free 201603L // also in <atomic>
#define __cpp_lib_atomic_lock_free_type_aliases 201907L // also in <atomic>
#define __cpp_lib_atomic_ref 201806L // also in <atomic>
#define __cpp_lib_atomic_shared_ptr 201711L // also in <memory>
#define __cpp_lib_atomic_value_initialization 201911L //
// also in <atomic>, <memory>
#define __cpp_lib_atomic_wait 201907L // also in <atomic>
#define __cpp_lib_barrier 201907L // also in <barrier>
#define __cpp_lib_bind_back 202202L // also in <functional>
#define __cpp_lib_bind_front 201907L // also in <functional>
#define __cpp_lib_bit_cast 201806L // also in <bit>
#define __cpp_lib_bitops 201907L // also in <bit>
#define __cpp_lib_bool_constant 201505L // also in <type_traits>
#define __cpp_lib_bounded_array_traits 201902L // also in <type_traits>
#define __cpp_lib_boyer_moore_searcher 201603L // also in <functional>
#define __cpp_lib_byte 201603L // also in <cstddef>
#define __cpp_lib_byteswap 202110L // also in <bit>
#define __cpp_lib_char8_t 201907L //
// also in <atomic>, <filesystem>, <istream>, <limits>, <locale>, <ostream>,
// <string>, <string_view>
#define __cpp_lib_chrono 201907L // also in <chrono>
#define __cpp_lib_chrono_udls 201304L // also in <chrono>
#define __cpp_lib_clamp 201603L // also in <algorithm>
#define __cpp_lib_complex_udls 201309L // also in <complex>
#define __cpp_lib_concepts 202002L // also in <concepts>
#define __cpp_lib_constexpr_algorithms 201806L // also in <algorithm>
#define __cpp_lib_constexpr_cmath 202202L // also in <cmath>, <cstdlib>
#define __cpp_lib_constexpr_complex 201711L // also in <complex>
#define __cpp_lib_constexpr_dynamic_alloc 201907L // also in <memory>
#define __cpp_lib_constexpr_functional 201907L // also in <functional>
#define __cpp_lib_constexpr_iterator 201811L // also in <iterator>
#define __cpp_lib_constexpr_memory 202202L // also in <memory>
#define __cpp_lib_constexpr_numeric 201911L // also in <numeric>
#define __cpp_lib_constexpr_string 201907L // also in <string>
#define __cpp_lib_constexpr_string_view 201811L // also in <string_view>
#define __cpp_lib_constexpr_tuple 201811L // also in <tuple>
#define __cpp_lib_constexpr_typeinfo 202106L // also in <typeinfo>
#define __cpp_lib_constexpr_utility 201811L // also in <utility>
#define __cpp_lib_constexpr_vector 201907L // also in <vector>
#define __cpp_lib_containers_ranges 202202L
// also in <vector>, <list>, <forward_list>, <map>, <set>, <unordered_map>,
// <unordered_set>,
// <deque>, <queue>, <stack>, <string>
#define __cpp_lib_coroutine 201902L // also in <coroutine>
#define __cpp_lib_destroying_delete 201806L // also in <new>
#define __cpp_lib_enable_shared_from_this 201603L // also in <memory>
#define __cpp_lib_endian 201907L // also in <bit>
#define __cpp_lib_erase_if 202002L
// also in <string>, <deque>, <forward_list>, <list>, <vector>, <map>, <set>,
// <unordered_map>,  <unordered_set>
#define __cpp_lib_exchange_function 201304L // also in <utility>
#define __cpp_lib_execution 201902L // also in <execution>
#define __cpp_lib_expected 202202L // also in <expected>
#define __cpp_lib_filesystem 201703L // also in <filesystem>
#define __cpp_lib_format 202110L // also in <format>
#define __cpp_lib_gcd_lcm 201606L // also in <numeric>
#define __cpp_lib_generic_associative_lookup 201304L // also in <map>, <set>
#define __cpp_lib_generic_unordered_lookup 201811L
// also in <unordered_map>, <unordered_set>
#define __cpp_lib_hardware_interference_size 201703L // also in <new>
#define __cpp_lib_has_unique_object_representations 201606L //
// also in <type_traits>
#define __cpp_lib_hypot 201603L // also in <cmath>
#define __cpp_lib_incomplete_container_elements 201505L
// also in <forward_list>, <list>, <vector>
#define __cpp_lib_int_pow2 202002L // also in <bit>
#define __cpp_lib_integer_comparison_functions 202002L // also in <utility>
#define __cpp_lib_integer_sequence 201304L // also in <utility>
#define __cpp_lib_integral_constant_callable 201304L // also in <type_traits>
#define __cpp_lib_interpolate 201902L // also in <cmath>, <numeric>
#define __cpp_lib_invoke 201411L // also in <functional>
#define __cpp_lib_invoke_r 202106L // also in <functional>
#define __cpp_lib_is_aggregate 201703L // also in <type_traits>
#define __cpp_lib_is_constant_evaluated 201811L // also in <type_traits>
#define __cpp_lib_is_final 201402L // also in <type_traits>
#define __cpp_lib_is_invocable 201703L // also in <type_traits>
#define __cpp_lib_is_layout_compatible 201907L // also in <type_traits>
#define __cpp_lib_is_nothrow_convertible 201806L // also in <type_traits>
#define __cpp_lib_is_null_pointer 201309L // also in <type_traits>
#define __cpp_lib_is_pointer_interconvertible 201907L // also in <type_traits>
#define __cpp_lib_is_scoped_enum 202011L // also in <type_traits>
#define __cpp_lib_is_swappable 201603L // also in <type_traits>
#define __cpp_lib_jthread 201911L // also in <stop_token>, <thread>
#define __cpp_lib_latch 201907L // also in <latch>
#define __cpp_lib_launder 201606L // also in <new>
#define __cpp_lib_list_remove_return_type 201806L //
// also in <forward_list>, <list>
#define __cpp_lib_logical_traits 201510L // also in <type_traits>
#define __cpp_lib_make_from_tuple 201606L // also in <tuple>
#define __cpp_lib_make_reverse_iterator 201402L // also in <iterator>
#define __cpp_lib_make_unique 201304L // also in <memory>
#define __cpp_lib_map_try_emplace 201411L // also in <map>
#define __cpp_lib_math_constants 201907L // also in <numbers>
#define __cpp_lib_math_special_functions 201603L // also in <cmath>
#define __cpp_lib_memory_resource 201603L // also in <memory_resource>
#define __cpp_lib_move_only_function 202110L // also in <functional>
#define __cpp_lib_node_extract 201606L
// also in <map>, <set>, <unordered_map>, <unordered_set>
#define __cpp_lib_nonmember_container_access 201411L
// also in <array>, <deque>, <forward_list>, <iterator>, <list>, <map>,
// <regex>, <set>, <string>,
// <unordered_map>, <unordered_set>, <vector>
#define __cpp_lib_not_fn 201603L // also in <functional>
#define __cpp_lib_null_iterators 201304L // also in <iterator>
#define __cpp_lib_optional 202110L // also in <optional>
#define __cpp_lib_out_ptr 202106L // also in <memory>
#define __cpp_lib_parallel_algorithm 201603L // also in <algorithm>, <numeric>
#define __cpp_lib_polymorphic_allocator 201902L // also in <memory_resource>
#define __cpp_lib_quoted_string_io 201304L // also in <iomanip>
#define __cpp_lib_ranges 202202L
// also in <algorithm>, <functional>, <iterator>, <memory>, <ranges>
#define __cpp_lib_ranges_chunk 202202L // also in <ranges>
#define __cpp_lib_ranges_chunk_by 202202L // also in <ranges>
#define __cpp_lib_ranges_iota 202202L // also in <numeric>
#define __cpp_lib_ranges_join_with 202202L // also in <ranges>
#define __cpp_lib_ranges_slide 202202L // also in <ranges>
#define __cpp_lib_ranges_starts_ends_with 202106L // also in <algorithm>
#define __cpp_lib_ranges_to_container 202202L // also in <ranges>
#define __cpp_lib_ranges_zip 202110L // also in <ranges>, <tuple>, <utility>
#define __cpp_lib_raw_memory_algorithms 201606L // also in <memory>
#define __cpp_lib_reference_from_temporary 202202L // also in <type_traits>
#define __cpp_lib_remove_cvref 201711L // also in <type_traits>
#define __cpp_lib_result_of_sfinae 201210L //
// also in <functional>, <type_traits>
#define __cpp_lib_robust_nonmodifying_seq_ops 201304L // also in <algorithm>
#define __cpp_lib_sample 201603L // also in <algorithm>
#define __cpp_lib_scoped_lock 201703L // also in <mutex>
#define __cpp_lib_semaphore 201907L // also in <semaphore>
#define __cpp_lib_shared_mutex 201505L // also in <shared_mutex>
#define __cpp_lib_shared_ptr_arrays 201707L // also in <memory>
#define __cpp_lib_shared_ptr_weak_type 201606L // also in <memory>
#define __cpp_lib_shared_timed_mutex 201402L // also in <shared_mutex>
#define __cpp_lib_shift 202202L // also in <algorithm>
#define __cpp_lib_smart_ptr_for_overwrite 202002L // also in <memory>
#define __cpp_lib_source_location 201907L // also in <source_location>
#define __cpp_lib_span 202002L // also in <span>
#define __cpp_lib_spanstream 202106L // also in <spanstream>
#define __cpp_lib_ssize 201902L // also in <iterator>
#define __cpp_lib_stacktrace 202011L // also in <stacktrace>
#define __cpp_lib_starts_ends_with 201711L // also in <string>, <string_view>
#define __cpp_lib_stdatomic_h 202011L // also in <stdatomic.h>
#define __cpp_lib_string_contains 202011L // also in <string>, <string_view>
#define __cpp_lib_string_resize_and_overwrite 202110L // also in <string>
#define __cpp_lib_string_udls 201304L // also in <string>
#define __cpp_lib_string_view 201803L // also in <string>, <string_view>
#define __cpp_lib_syncbuf 201803L // also in <syncstream>
#define __cpp_lib_three_way_comparison 201907L // also in <compare>
#define __cpp_lib_to_address 201711L // also in <memory>
#define __cpp_lib_to_array 201907L // also in <array>
#define __cpp_lib_to_chars 201611L // also in <charconv>
#define __cpp_lib_to_underlying 202102L // also in <utility>
#define __cpp_lib_transformation_trait_aliases 201304L // also in <type_traits>
#define __cpp_lib_transparent_operators 201510L //
// also in <memory>, <functional>
#define __cpp_lib_tuple_element_t 201402L // also in <tuple>
#define __cpp_lib_tuples_by_type 201304L // also in <utility>, <tuple>
#define __cpp_lib_type_identity 201806L // also in <type_traits>
#define __cpp_lib_type_trait_variable_templates 201510L // also in <type_traits>
#define __cpp_lib_uncaught_exceptions 201411L // also in <exception>
#define __cpp_lib_unordered_map_try_emplace 201411L // also in <unordered_map>
#define __cpp_lib_unreachable 202202L // also in <utility>
#define __cpp_lib_unwrap_ref 201811L // also in <type_traits>
#define __cpp_lib_variant 202106L // also in <variant>
#define __cpp_lib_void_t 201411L // also in <type_traits>
}

// 17.3.4.1 Type float_round_style
namespace std {
enum float_round_style {
  round_indeterminate = -1,
  round_toward_zero = 0,
  round_to_nearest = 1,
  round_toward_infinity = 2,
  round_toward_neg_infinity = 3
};
}
// 17.3.5 Class template numeric_limits
namespace std {
// numeric_limits:
//   integral:
//     min/max(): O
template<class T> class numeric_limits {
public:
static constexpr bool is_specialized = false;
static constexpr T min() noexcept { return T(); }
static constexpr T max() noexcept { return T(); }
static constexpr T lowest() noexcept { return T(); }
static constexpr int digits = 0;
static constexpr int digits10 = 0;
static constexpr int max_digits10 = 0;
static constexpr bool is_signed = false;
static constexpr bool is_integer = false;
static constexpr bool is_exact = false;
static constexpr int radix = 0;
static constexpr T epsilon() noexcept { return T(); }
static constexpr T round_error() noexcept { return T(); }
static constexpr int min_exponent = 0;
static constexpr int min_exponent10 = 0;
static constexpr int max_exponent = 0;
static constexpr int max_exponent10 = 0;
static constexpr bool has_infinity = false;
static constexpr bool has_quiet_NaN = false;
static constexpr bool has_signaling_NaN = false;
static constexpr float_denorm_style has_denorm = denorm_absent;
static constexpr bool has_denorm_loss = false;
static constexpr T infinity() noexcept { return T(); }
static constexpr T quiet_NaN() noexcept { return T(); }
static constexpr T signaling_NaN() noexcept { return T(); }
static constexpr T denorm_min() noexcept { return T(); }
static constexpr bool is_iec559 = false;
static constexpr bool is_bounded = false;
static constexpr bool is_modulo = false;
static constexpr bool traps = false;
static constexpr bool tinyness_before = false;
static constexpr float_round_style round_style = round_toward_zero;
};
}

// 17.3.6 Header <climits> synopsis
{
#define CHAR_BIT see below
#define SCHAR_MIN see below
#define SCHAR_MAX see below
#define UCHAR_MAX see below
#define CHAR_MIN see below
#define CHAR_MAX see below
#define MB_LEN_MAX see below
#define SHRT_MIN see below
#define SHRT_MAX see below
#define USHRT_MAX see below
#define INT_MIN see below
#define INT_MAX see below
#define UINT_MAX see below
#define LONG_MIN see below
#define LONG_MAX see below
#define ULONG_MAX see below
#define LLONG_MIN see below
#define LLONG_MAX see below
#define ULLONG_MAX see below
}

// 17.3.7 Header <cfloat> synopsis
{
#define FLT_ROUNDS see below
#define FLT_EVAL_METHOD see below
#define FLT_HAS_SUBNORM see below
#define DBL_HAS_SUBNORM see below
#define LDBL_HAS_SUBNORM see below
#define FLT_RADIX see below
#define FLT_MANT_DIG see below
#define DBL_MANT_DIG see below
#define LDBL_MANT_DIG see below
#define FLT_DECIMAL_DIG see below
#define DBL_DECIMAL_DIG see below
#define LDBL_DECIMAL_DIG see below
#define DECIMAL_DIG see below
#define FLT_DIG see below
#define DBL_DIG see below
#define LDBL_DIG see below
#define FLT_MIN_EXP see below
#define DBL_MIN_EXP see below
#define LDBL_MIN_EXP see below
#define FLT_MIN_10_EXP see below
#define DBL_MIN_10_EXP see below
#define LDBL_MIN_10_EXP see below
#define FLT_MAX_EXP see below
#define DBL_MAX_EXP see below
#define LDBL_MAX_EXP see below
#define FLT_MAX_10_EXP see below
#define DBL_MAX_10_EXP see below
#define LDBL_MAX_10_EXP see below
#define FLT_MAX see below
#define DBL_MAX see below
#define LDBL_MAX see below
#define FLT_EPSILON see below
#define DBL_EPSILON see below
#define LDBL_EPSILON see below
#define FLT_MIN see below
#define DBL_MIN see below
#define LDBL_MIN see below
#define FLT_TRUE_MIN see below
#define DBL_TRUE_MIN see below
#define LDBL_TRUE_MIN see below
}

// 17.4.2 Header <cstdint> synopsis
namespace std {
// using all
using int8_t = signed integer type ; // optional
using int16_t = signed integer type ; // optional
using int32_t = signed integer type ; // optional
using int64_t = signed integer type ; // optional
using intN_t = see below ; // optional
using int_fast8_t = signed integer type ;
using int_fast16_t = signed integer type ;
using int_fast32_t = signed integer type ;
using int_fast64_t = signed integer type ;
using int_fastN_t = see below ; // optional
using int_least8_t = signed integer type ;
using int_least16_t = signed integer type ;
using int_least32_t = signed integer type ;
using int_least64_t = signed integer type ;
using int_leastN_t = see below ; // optional
using intmax_t = signed integer type ;
using intptr_t = signed integer type ; // optional
using uint8_t = unsigned integer type ; // optional
using uint16_t = unsigned integer type ; // optional
using uint32_t = unsigned integer type ; // optional
using uint64_t = unsigned integer type ; // optional
using uintN_t = see below ; // optional
using uint_fast8_t = unsigned integer type ;
using uint_fast16_t = unsigned integer type ;
using uint_fast32_t = unsigned integer type ;
using uint_fast64_t = unsigned integer type ;
using uint_fastN_t = see below ; // optional
using uint_least8_t = unsigned integer type ;
using uint_least16_t = unsigned integer type ;
using uint_least32_t = unsigned integer type ;
using uint_least64_t = unsigned integer type ;
using uint_leastN_t = see below ; // optional
using uintmax_t = unsigned integer type ;
using uintptr_t = unsigned integer type ; // optional
}
{
#define INTN_MIN see below
#define INTN_MAX see below
#define UINTN_MAX see below
#define INT_FASTN_MIN see below
#define INT_FASTN_MAX see below
#define UINT_FASTN_MAX see below
#define INT_LEASTN_MIN see below
#define INT_LEASTN_MAX see below
#define UINT_LEASTN_MAX see below
#define INTMAX_MIN see below
#define INTMAX_MAX see below
#define UINTMAX_MAX see below
#define INTPTR_MIN see below // optional
#define INTPTR_MAX see below // optional
#define UINTPTR_MAX see below // optional
#define PTRDIFF_MIN see below
#define PTRDIFF_MAX see below
#define SIZE_MAX see below
#define SIG_ATOMIC_MIN see below
#define SIG_ATOMIC_MAX see below
#define WCHAR_MIN see below
#define WCHAR_MAX see below
#define WINT_MIN see below
#define WINT_MAX see below
#define INTN_C(value) see below
#define UINTN_C(value) see below
#define INTMAX_C(value) see below
#define UINTMAX_C(value) see below
}

// 17.6.2 Header <new> synopsis
namespace std {
// 17.6.4, storage allocation errors
class bad_alloc; // using
class bad_array_new_length; // using
struct destroying_delete_t {
explicit destroying_delete_t() = default;
}; // using
inline constexpr destroying_delete_t destroying_delete{};
// global operator new control
enum class align_val_t : size_t {}; // using
struct nothrow_t { explicit nothrow_t() = default; }; // using
extern const nothrow_t nothrow; // using
using new_handler = void (*)(); // using
new_handler get_new_handler() noexcept; // using
new_handler set_new_handler(new_handler new_p) noexcept; // using
// 17.6.5, pointer optimization barrier
template<class T> [[nodiscard]] constexpr T* launder(T* p) noexcept;
// 17.6.6, hardware interference size
inline constexpr size_t hardware_destructive_interference_size
  = implementation-defined ;
inline constexpr size_t hardware_constructive_interference_size
  = implementation-defined ;
}

// 17.6.3, storage allocation and deallocation
{
[[nodiscard]] void* operator new(std::size_t size);
[[nodiscard]] void* operator new(std::size_t size, std::align_val_t alignment);
[[nodiscard]] void* operator new(std::size_t size,
                                 const std::nothrow_t&) noexcept;
[[nodiscard]] void* operator new(std::size_t size, std::align_val_t alignment,
const std::nothrow_t&) noexcept;
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, std::size_t size) noexcept;
void operator delete(void* ptr, std::align_val_t alignment) noexcept;
void operator delete(void* ptr, std::size_t size,
                     std::align_val_t alignment) noexcept;
void operator delete(void* ptr, const std::nothrow_t&) noexcept;
void operator delete(void* ptr, std::align_val_t alignment,
                     const std::nothrow_t&) noexcept;
[[nodiscard]] void* operator new[](std::size_t size);
[[nodiscard]] void* operator new[](std::size_t size,
                                   std::align_val_t alignment);
[[nodiscard]] void* operator new[](std::size_t size,
                                   const std::nothrow_t&) noexcept;
[[nodiscard]] void* operator new[](std::size_t size,
                                   std::align_val_t alignment,
                                   const std::nothrow_t&) noexcept;
void operator delete[](void* ptr) noexcept;
void operator delete[](void* ptr, std::size_t size) noexcept;
void operator delete[](void* ptr, std::align_val_t alignment) noexcept;
void operator delete[](void* ptr, std::size_t size,
                       std::align_val_t alignment) noexcept;
void operator delete[](void* ptr, const std::nothrow_t&) noexcept;
void operator delete[](void* ptr, std::align_val_t alignment,
                       const std::nothrow_t&) noexcept;
[[nodiscard]] void* operator new (std::size_t size, void* ptr) noexcept;
[[nodiscard]] void* operator new[](std::size_t size, void* ptr) noexcept;
void operator delete (void* ptr, void*) noexcept;
void operator delete[](void* ptr, void*) noexcept;
}

// 17.7.2 Header <typeinfo> synopsis
namespace std {
class type_info; // using
class bad_cast; // using
class bad_typeid; // using
}

// 17.8.1 Header <source_location> synopsis
namespace std {
struct source_location; // using
}

// 17.9.2 Header <exception> synopsis
namespace std {
class exception; // using
class bad_exception; // using
class nested_exception; // using
using terminate_handler = void (*)(); // using
terminate_handler get_terminate() noexcept; // using
terminate_handler set_terminate(terminate_handler f) noexcept; // using
[[noreturn]] void terminate() noexcept; // using
int uncaught_exceptions() noexcept; // using
using exception_ptr = unspecified ; // using
exception_ptr current_exception() noexcept; // using
[[noreturn]] void rethrow_exception(exception_ptr p); // using
template<class E> exception_ptr make_exception_ptr(E e) noexcept; // using
template<class T> [[noreturn]] void throw_with_nested(T&& t); // using
template<class E> void rethrow_if_nested(const E& e); // using
}

// 17.11.1 Header <compare> synopsis
namespace std {
// 17.11.2, comparison category types
class partial_ordering; // using
class weak_ordering; // using
class strong_ordering; // using
// named comparison functions
constexpr bool is_eq (partial_ordering cmp) noexcept { return cmp == 0; } // O
constexpr bool is_neq (partial_ordering cmp) noexcept { return cmp != 0; } // O
constexpr bool is_lt (partial_ordering cmp) noexcept { return cmp < 0; } // O
constexpr bool is_lteq(partial_ordering cmp) noexcept { return cmp <= 0; } // O
constexpr bool is_gt (partial_ordering cmp) noexcept { return cmp > 0; } // O
constexpr bool is_gteq(partial_ordering cmp) noexcept { return cmp >= 0; } // O
// 17.11.3, common comparison category type
template<class... Ts>
struct common_comparison_category {
  using type = see below ;
}; // O
template<class... Ts>
using common_comparison_category_t
= typename common_comparison_category<Ts...>::type; // O
// 17.11.4, concept three_way_comparable
template<class T, class Cat = partial_ordering>
concept three_way_comparable = see below ; // O
template<class T, class U, class Cat = partial_ordering>
concept three_way_comparable_with = see below ; // O
// 17.11.5, result of three-way comparison
template<class T, class U = T> struct compare_three_way_result; // O
template<class T, class U = T>
using compare_three_way_result_t
  = typename compare_three_way_result<T, U>::type; // O
// 22.10.8.8, class compare_three_way
struct compare_three_way; // O
// 17.11.6, comparison algorithms
inline namespace unspecified {
inline constexpr unspecified strong_order = unspecified ;
inline constexpr unspecified weak_order = unspecified ;
inline constexpr unspecified partial_order = unspecified ;
inline constexpr unspecified compare_strong_order_fallback = unspecified ;
inline constexpr unspecified compare_weak_order_fallback = unspecified ;
inline constexpr unspecified compare_partial_order_fallback = unspecified ;
}
}

// 17.12.2 Header <coroutine> synopsis
#include <compare> // see 17.11.1
namespace std {
// 17.12.3, coroutine traits
template<class R, class... ArgTypes>
struct coroutine_traits;
// 17.12.4, coroutine handle
template<class Promise = void>
struct coroutine_handle;
// 17.12.4.8, comparison operators
constexpr bool operator==(coroutine_handle<> x,
                          coroutine_handle<> y) noexcept;
constexpr strong_ordering operator<=>(coroutine_handle<> x,
                                      coroutine_handle<> y) noexcept;
// 17.12.4.9, hash support
template<class T> struct hash;
template<class P> struct hash<coroutine_handle<P>>;
// 17.12.5, no-op coroutines
struct noop_coroutine_promise;
template<> struct coroutine_handle<noop_coroutine_promise>;
using noop_coroutine_handle = coroutine_handle<noop_coroutine_promise>;
noop_coroutine_handle noop_coroutine() noexcept;
// 17.12.6, trivial awaitables
struct suspend_never;
struct suspend_always;
}

// 17.13.2 Header <cstdarg> synopsis
namespace std {
using va_list = see below ;
}
{
#define va_arg(V, P) see below
#define va_copy(VDST, VSRC) see below
#define va_end(V) see below
#define va_start(V, P) see below
}

// 17.13.3 Header <csetjmp> synopsis
namespace std {
using jmp_buf = see below ;
[[noreturn]] void longjmp(jmp_buf env, int val);
}
#define setjmp(env) see below

// 17.13.4 Header <csignal> synopsis
namespace std {
using sig_atomic_t = see below ;
// 17.13.5, signal handlers
extern "C" using signal-handler = void(int); // exposition only
signal-handler * signal(int sig, signal-handler * func);
int raise(int sig);
}
{
#define SIG_DFL see below
#define SIG_ERR see below
#define SIG_IGN see below
#define SIGABRT see below
#define SIGFPE see below
#define SIGILL see below
#define SIGINT see below
#define SIGSEGV see below
#define SIGTERM see below
}

// 17.14 C headers
// <complex>
// <iso646.h>
// <stdalign.h>
// <stdbool.h>
// <tgmath.h>


// 18 Concepts library
// 18.3 Header <concepts> synopsis
namespace std {
// 18.4, language-related concepts
// 18.4.2, concept same_as
template<class T, class U>
concept same_as = see below ; // O
// 18.4.3, concept derived_from
template<class Derived, class Base>
concept derived_from = see below ; // O
// 18.4.4, concept convertible_to
template<class From, class To>
concept convertible_to = see below ; // O
// 18.4.5, concept common_reference_with
template<class T, class U>
concept common_reference_with = see below ; // O
// 18.4.6, concept common_with
template<class T, class U>
concept common_with = see below ; // O
// 18.4.7, arithmetic concepts
template<class T>
concept integral = see below ; // O
template<class T>
concept signed_integral = see below ; // O
template<class T>
concept unsigned_integral = see below ; // O
template<class T>
concept floating_point = see below ; // O
// 18.4.8, concept assignable_from
template<class LHS, class RHS>
concept assignable_from = see below ;
// 18.4.9, concept swappable
namespace ranges {
inline namespace unspecified {
inline constexpr unspecified swap = unspecified ;
}
}
template<class T>
concept swappable = see below ; // o
template<class T, class U>
concept swappable_with = see below ; // o
// 18.4.10, concept destructible
template<class T>
concept destructible = see below ; // O
// 18.4.11, concept constructible_from
template<class T, class... Args>
concept constructible_from = see below ; // O
// 18.4.12, concept default_initializable
template<class T>
concept default_initializable = see below ; // O
// 18.4.13, concept move_constructible
template<class T>
concept move_constructible = see below ; // O
// 18.4.14, concept copy_constructible
template<class T>
concept copy_constructible = see below ; // O
// 18.5, comparison concepts
// 18.5.3, concept equality_comparable
template<class T>
concept equality_comparable = see below ; // O
template<class T, class U>
concept equality_comparable_with = see below ; // O
// 18.5.4, concept totally_ordered
template<class T>
concept totally_ordered = see below ; // O
template<class T, class U>
concept totally_ordered_with = see below ; // O
// 18.6, object concepts
template<class T>
concept movable = see below ; // O
template<class T>
concept copyable = see below ; // O
template<class T>
concept semiregular = see below ; // O
template<class T>
concept regular = see below ; // O
// 18.7, callable concepts
// 18.7.2, concept invocable
template<class F, class... Args>
concept invocable = see below ; // O
// 18.7.3, concept regular_invocable
template<class F, class... Args>
concept regular_invocable = see below ; // O
// 18.7.4, concept predicate
template<class F, class... Args>
concept predicate = see below ; // O
// 18.7.5, concept relation
template<class R, class T, class U>
concept relation = see below ; // O
// 18.7.6, concept equivalence_relation
template<class R, class T, class U>
concept equivalence_relation = see below ; // O
// 18.7.7, concept strict_weak_order
template<class R, class T, class U>
concept strict_weak_order = see below ; // O
}


// 19 Diagnostics library
// 19.2.2 Header <stdexcept> synopsis
namespace std {
class logic_error; // using
class domain_error; // using
class invalid_argument; // using
class length_error; // using
class out_of_range; // using
class runtime_error; // using
class range_error; // using
class overflow_error; // using
class underflow_error; // using
}

// 19.3.2 Header <cassert> synopsis
#define assert(E) see below

// 19.4.2 Header <cerrno> synopsis
{
#define errno see below
#define E2BIG see below
#define EACCES see below
#define EADDRINUSE see below
#define EADDRNOTAVAIL see below
#define EAFNOSUPPORT see below
#define EAGAIN see below
#define EALREADY see below
#define EBADF see below
#define EBADMSG see below
#define EBUSY see below
#define ECANCELED see below
#define ECHILD see below
#define ECONNABORTED see below
#define ECONNREFUSED see below
#define ECONNRESET see below
#define EDEADLK see below
#define EDESTADDRREQ see below
#define EDOM see below
#define EEXIST see below
#define EFAULT see below
#define EFBIG see below
#define EHOSTUNREACH see below
#define EIDRM see below
#define EILSEQ see below
#define EINPROGRESS see below
#define EINTR see below
#define EINVAL see below
#define EIO see below
#define EISCONN see below
#define EISDIR see below
#define ELOOP see below
#define EMFILE see below
#define EMLINK see below
#define EMSGSIZE see below
#define ENAMETOOLONG see below
#define ENETDOWN see below
#define ENETRESET see below
#define ENETUNREACH see below
#define ENFILE see below
#define ENOBUFS see below
#define ENODATA see below
#define ENODEV see below
#define ENOENT see below
#define ENOEXEC see below
#define ENOLCK see below
#define ENOLINK see below
#define ENOMEM see below
#define ENOMSG see below
#define ENOPROTOOPT see below
#define ENOSPC see below
#define ENOSR see below
#define ENOSTR see below
#define ENOSYS see below
#define ENOTCONN see below
#define ENOTDIR see below
#define ENOTEMPTY see below
#define ENOTRECOVERABLE see below
#define ENOTSOCK see below
#define ENOTSUP see below
#define ENOTTY see below
#define ENXIO see below
#define EOPNOTSUPP see below
#define EOVERFLOW see below
#define EOWNERDEAD see below
#define EPERM see below
#define EPIPE see below
#define EPROTO see below
#define EPROTONOSUPPORT see below
#define EPROTOTYPE see below
#define ERANGE see below
#define EROFS see below
#define ESPIPE see below
#define ESRCH see below
#define ETIME see below
#define ETIMEDOUT see below
#define ETXTBSY see below
#define EWOULDBLOCK see below
#define EXDEV see below
}

// 19.5.2 Header <system_error> synopsis
#include <compare>
namespace std {
class error_category;
const error_category& generic_category() noexcept;
const error_category& system_category() noexcept;
class error_code;
class error_condition;
class system_error;
template<class T>
struct is_error_code_enum : public false_type {};
template<class T>
struct is_error_condition_enum : public false_type {};
enum class errc {
  address_family_not_supported, // EAFNOSUPPORT
  address_in_use, // EADDRINUSE
  address_not_available, // EADDRNOTAVAIL
  already_connected, // EISCONN
  argument_list_too_long, // E2BIG
  argument_out_of_domain, // EDOM
  bad_address, // EFAULT
  bad_file_descriptor, // EBADF
  bad_message, // EBADMSG
  broken_pipe, // EPIPE
  connection_aborted, // ECONNABORTED
  connection_already_in_progress, // EALREADY
  connection_refused, // ECONNREFUSED
  connection_reset, // ECONNRESET
  cross_device_link, // EXDEV
  destination_address_required, // EDESTADDRREQ
  device_or_resource_busy, // EBUSY
  directory_not_empty, // ENOTEMPTY
  executable_format_error, // ENOEXEC
  file_exists, // EEXIST
  file_too_large, // EFBIG
  filename_too_long, // ENAMETOOLONG
  function_not_supported, // ENOSYS
  host_unreachable, // EHOSTUNREACH
  identifier_removed, // EIDRM
  illegal_byte_sequence, // EILSEQ
  inappropriate_io_control_operation, // ENOTTY
  interrupted, // EINTR
  invalid_argument, // EINVAL
  invalid_seek, // ESPIPE
  io_error, // EIO
  is_a_directory, // EISDIR
  message_size, // EMSGSIZE
  network_down, // ENETDOWN
  network_reset, // ENETRESET
  network_unreachable, // ENETUNREACH
  no_buffer_space, // ENOBUFS
  no_child_process, // ECHILD
  no_link, // ENOLINK
  no_lock_available, // ENOLCK
  no_message_available, // ENODATA
  no_message, // ENOMSG
  no_protocol_option, // ENOPROTOOPT
  no_space_on_device, // ENOSPC
  no_stream_resources, // ENOSR
  no_such_device_or_address, // ENXIO
  no_such_device, // ENODEV
  no_such_file_or_directory, // ENOENT
  no_such_process, // ESRCH
  not_a_directory, // ENOTDIR
  not_a_socket, // ENOTSOCK
  not_a_stream, // ENOSTR
  not_connected, // ENOTCONN
  not_enough_memory, // ENOMEM
  not_supported, // ENOTSUP
  operation_canceled, // ECANCELED
  operation_in_progress, // EINPROGRESS
  operation_not_permitted, // EPERM
  operation_not_supported, // EOPNOTSUPP
  operation_would_block, // EWOULDBLOCK
  owner_dead, // EOWNERDEAD
  permission_denied, // EACCES
  protocol_error, // EPROTO
  protocol_not_supported, // EPROTONOSUPPORT
  read_only_file_system, // EROFS
  resource_deadlock_would_occur, // EDEADLK
  resource_unavailable_try_again, // EAGAIN
  result_out_of_range, // ERANGE
  state_not_recoverable, // ENOTRECOVERABLE
  stream_timeout, // ETIME
  text_file_busy, // ETXTBSY
  timed_out, // ETIMEDOUT
  too_many_files_open_in_system, // ENFILE
  too_many_files_open, // EMFILE
  too_many_links, // EMLINK
  too_many_symbolic_link_levels, // ELOOP
  value_too_large, // EOVERFLOW
  wrong_protocol_type, // EPROTOTYPE
};
template<> struct is_error_condition_enum<errc> : true_type {};
// 19.5.4.5, non-member functions
error_code make_error_code(errc e) noexcept;
template<class charT, class traits>
basic_ostream<charT, traits>&
operator<<(basic_ostream<charT, traits>& os, const error_code& ec);
// 19.5.5.5, non-member functions
error_condition make_error_condition(errc e) noexcept;
// 19.5.6, comparison operator functions
bool operator==(const error_code& lhs, const error_code& rhs) noexcept;
bool operator==(const error_code& lhs, const error_condition& rhs) noexcept;
bool operator==(const error_condition& lhs,
                const error_condition& rhs) noexcept;
strong_ordering operator<=>(const error_code& lhs,
                            const error_code& rhs) noexcept;
strong_ordering operator<=>(const error_condition& lhs,
                            const error_condition& rhs) noexcept;
// 19.5.7, hash support
template<class T> struct hash;
template<> struct hash<error_code>;
template<> struct hash<error_condition>;
// 19.5, system error support
template<class T>
inline constexpr bool is_error_code_enum_v = is_error_code_enum<T>::value;
template<class T>
inline constexpr bool is_error_condition_enum_v
  = is_error_condition_enum<T>::value;
}

// 19.6.2 Header <stacktrace> synopsis
namespace std {
// 19.6.3, class stacktrace_entry
class stacktrace_entry;
// 19.6.4, class template basic_stacktrace
template<class Allocator>
class basic_stacktrace;
// basic_stacktrace typedef-names
using stacktrace = basic_stacktrace<allocator<stacktrace_entry>>;
// 19.6.4.6, non-member functions
template<class Allocator>
void swap(basic_stacktrace<Allocator>& a, basic_stacktrace<Allocator>& b)
  noexcept(noexcept(a.swap(b)));
string to_string(const stacktrace_entry& f);
template<class Allocator>
string to_string(const basic_stacktrace<Allocator>& st);
template<class charT, class traits>
basic_ostream<charT, traits>&
operator<<(basic_ostream<charT, traits>& os, const stacktrace_entry& f);
template<class charT, class traits, class Allocator>
basic_ostream<charT, traits>&
operator<<(basic_ostream<charT, traits>& os,
           const basic_stacktrace<Allocator>& st);
namespace pmr {
using stacktrace = basic_stacktrace<polymorphic_allocator<stacktrace_entry>>;
}
// 19.6.4.7, hash support
template<class T> struct hash;
template<> struct hash<stacktrace_entry>;
template<class Allocator> struct hash<basic_stacktrace<Allocator>>;
}



// 25.2 Header <iterator> synopsis
#include <compare> // see 17.11.1
#include <concepts> // see 18.3
namespace std {
template<class T> using with-reference = T&; // exposition only
template<class T> concept can-reference // exposition only
= requires { typename with-reference<T>; };
template<class T> concept dereferenceable // exposition only
= requires(T& t) {
{ *t } -> can-reference ; // not required to be equality-preserving
};
// 25.3.2, associated types
// 25.3.2.1, incrementable traits
template<class> struct incrementable_traits;
template<class T>
using iter_difference_t = see below ;
// 25.3.2.2, indirectly readable traits
template<class> struct indirectly_readable_traits;
template<class T>
using iter_value_t = see below ;
// 25.3.2.3, iterator traits
template<class I> struct iterator_traits;
template<class T> requires is_object_v<T> struct iterator_traits<T*>;
template<dereferenceable T>
using iter_reference_t = decltype(*declval<T&>());
namespace ranges {
// 25.3.3, customization point objects
inline namespace unspecified {
// 25.3.3.1, ranges::iter_move
inline constexpr unspecified iter_move = unspecified ;
// 25.3.3.2, ranges::iter_swap
inline constexpr unspecified iter_swap = unspecified ;
}
}
template<dereferenceable T>
requires requires(T& t) {
{ ranges::iter_move(t) } -> can-reference ;
}
using iter_rvalue_reference_t
= decltype(ranges::iter_move(declval<T&>()));
// 25.3.4, iterator concepts
// 25.3.4.2, concept indirectly_readable
template<class In>
concept indirectly_readable = see below ;
template<indirectly_readable T>
using iter_common_reference_t =
common_reference_t<iter_reference_t<T>, iter_value_t<T>&>;
// 25.3.4.3, concept indirectly_writable
template<class Out, class T>
concept indirectly_writable = see below ;
// 25.3.4.4, concept weakly_incrementable
template<class I>
concept weakly_incrementable = see below ;
// 25.3.4.5, concept incrementable
template<class I>
concept incrementable = see below ;
// 25.3.4.6, concept input_or_output_iterator
template<class I>
concept input_or_output_iterator = see below ;
// 25.3.4.7, concept sentinel_for
template<class S, class I>
concept sentinel_for = see below ;
// 25.3.4.8, concept sized_sentinel_for
template<class S, class I>
inline constexpr bool disable_sized_sentinel_for = false;
template<class S, class I>
concept sized_sentinel_for = see below ;
// 25.3.4.9, concept input_iterator
template<class I>
concept input_iterator = see below ;
// 25.3.4.10, concept output_iterator
template<class I, class T>
concept output_iterator = see below ;
// 25.3.4.11, concept forward_iterator
template<class I>
concept forward_iterator = see below ;
// 25.3.4.12, concept bidirectional_iterator
template<class I>
concept bidirectional_iterator = see below ;
// 25.3.4.13, concept random_access_iterator
template<class I>
concept random_access_iterator = see below ;
// 25.3.4.14, concept contiguous_iterator
template<class I>
concept contiguous_iterator = see below ;
// 25.3.6, indirect callable requirements
// 25.3.6.2, indirect callables
template<class F, class I>
concept indirectly_unary_invocable = see below ;
template<class F, class I>
concept indirectly_regular_unary_invocable = see below ;
template<class F, class I>
concept indirect_unary_predicate = see below ;
template<class F, class I1, class I2>
concept indirect_binary_predicate = see below ;
template<class F, class I1, class I2 = I1>
concept indirect_equivalence_relation = see below ;
template<class F, class I1, class I2 = I1>
concept indirect_strict_weak_order = see below ;
template<class F, class... Is>
requires (indirectly_readable<Is> && ...)
&& invocable<F, iter_reference_t<Is>...>
using indirect_result_t = invoke_result_t<F, iter_reference_t<Is>...>;
// 25.3.6.3, projected
template<indirectly_readable I, indirectly_regular_unary_invocable<I> Proj>
struct projected;
template<weakly_incrementable I, class Proj>
struct incrementable_traits<projected<I, Proj>>;
// 25.3.7, common algorithm requirements
// 25.3.7.2, concept indirectly_movable
template<class In, class Out>
concept indirectly_movable = see below ;
template<class In, class Out>
concept indirectly_movable_storable = see below ;
// 25.3.7.3, concept indirectly_copyable
template<class In, class Out>
concept indirectly_copyable = see below ;
template<class In, class Out>
concept indirectly_copyable_storable = see below ;
// 25.3.7.4, concept indirectly_swappable
template<class I1, class I2 = I1>
concept indirectly_swappable = see below ;
// 25.3.7.5, concept indirectly_comparable
template<class I1, class I2, class R, class P1 = identity, class P2 = identity>
concept indirectly_comparable = see below ;
// 25.3.7.6, concept permutable
template<class I>
concept permutable = see below ;
// 25.3.7.7, concept mergeable
template<class I1, class I2, class Out,
class R = ranges::less, class P1 = identity, class P2 = identity>
concept mergeable = see below ;
// 25.3.7.8, concept sortable
template<class I, class R = ranges::less, class P = identity>
concept sortable = see below ;
// 25.4, primitives
// 25.4.2, iterator tags
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag: public input_iterator_tag { };
struct bidirectional_iterator_tag: public forward_iterator_tag { };
struct random_access_iterator_tag: public bidirectional_iterator_tag { };
struct contiguous_iterator_tag: public random_access_iterator_tag { };
// 25.4.3, iterator operations
template<class InputIterator, class Distance>
constexpr void
advance(InputIterator& i, Distance n);
template<class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last);
template<class InputIterator>
constexpr InputIterator
next(InputIterator x,
typename iterator_traits<InputIterator>::difference_type n = 1);
template<class BidirectionalIterator>
constexpr BidirectionalIterator
prev(BidirectionalIterator x,
typename iterator_traits<BidirectionalIterator>::difference_type n = 1);
// 25.4.4, range iterator operations
namespace ranges {
// 25.4.4.2, ranges::advance
template<input_or_output_iterator I>
constexpr void advance(I& i, iter_difference_t<I> n);
template<input_or_output_iterator I, sentinel_for<I> S>
constexpr void advance(I& i, S bound);
template<input_or_output_iterator I, sentinel_for<I> S>
constexpr iter_difference_t<I> advance(I& i, iter_difference_t<I> n, S bound);
// 25.4.4.3, ranges::distance
template<input_or_output_iterator I, sentinel_for<I> S>
requires (!sized_sentinel_for<S, I>)
constexpr iter_difference_t<I> distance(I first, S last);
template<input_or_output_iterator I, sized_sentinel_for<I> S>
constexpr iter_difference_t<I> distance(const I& first, const S& last);
template<range R>
constexpr range_difference_t<R> distance(R&& r);
// 25.4.4.4, ranges::next
template<input_or_output_iterator I>
constexpr I next(I x);
template<input_or_output_iterator I>
constexpr I next(I x, iter_difference_t<I> n);
template<input_or_output_iterator I, sentinel_for<I> S>
constexpr I next(I x, S bound);
template<input_or_output_iterator I, sentinel_for<I> S>
constexpr I next(I x, iter_difference_t<I> n, S bound);
// 25.4.4.5, ranges::prev
template<bidirectional_iterator I>
constexpr I prev(I x);
template<bidirectional_iterator I>
constexpr I prev(I x, iter_difference_t<I> n);
template<bidirectional_iterator I>
constexpr I prev(I x, iter_difference_t<I> n, I bound);
}
// 25.5, predefined iterators and sentinels
// 25.5.1, reverse iterators
template<class Iterator> class reverse_iterator;
template<class Iterator1, class Iterator2>
constexpr bool operator==(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator!=(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator<(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator>(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator<=(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator>=(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, three_way_comparable_with<Iterator1> Iterator2>
constexpr compare_three_way_result_t<Iterator1, Iterator2>
operator<=>(const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr auto operator-(
const reverse_iterator<Iterator1>& x,
const reverse_iterator<Iterator2>& y) -> decltype(y.base() - x.base());
template<class Iterator>
constexpr reverse_iterator<Iterator> operator+(
iter_difference_t<Iterator> n,
const reverse_iterator<Iterator>& x);
template<class Iterator>
constexpr reverse_iterator<Iterator> make_reverse_iterator(Iterator i);
template<class Iterator1, class Iterator2>
requires (!sized_sentinel_for<Iterator1, Iterator2>)
inline constexpr bool disable_sized_sentinel_for<reverse_iterator<Iterator1>,
reverse_iterator<Iterator2>> = true;
// 25.5.2, insert iterators
template<class Container> class back_insert_iterator;
template<class Container>
constexpr back_insert_iterator<Container> back_inserter(Container& x);
template<class Container> class front_insert_iterator;
template<class Container>
constexpr front_insert_iterator<Container> front_inserter(Container& x);
template<class Container> class insert_iterator;
template<class Container>
constexpr insert_iterator<Container>
inserter(Container& x, ranges::iterator_t<Container> i);
// 25.5.3, move iterators and sentinels
template<class Iterator> class move_iterator;
template<class Iterator1, class Iterator2>
constexpr bool operator==(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator<(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator>(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator<=(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr bool operator>=(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y);
template<class Iterator1, three_way_comparable_with<Iterator1> Iterator2>
constexpr compare_three_way_result_t<Iterator1, Iterator2>
operator<=>(const move_iterator<Iterator1>& x,
const move_iterator<Iterator2>& y);
template<class Iterator1, class Iterator2>
constexpr auto operator-(
const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y)
-> decltype(x.base() - y.base());
template<class Iterator>
constexpr move_iterator<Iterator>
operator+(iter_difference_t<Iterator> n, const move_iterator<Iterator>& x);
template<class Iterator>
constexpr move_iterator<Iterator> make_move_iterator(Iterator i);
template<semiregular S> class move_sentinel;
// 25.5.4, common iterators
template<input_or_output_iterator I, sentinel_for<I> S>
requires (!same_as<I, S> && copyable<I>)
class common_iterator;
template<class I, class S>
struct incrementable_traits<common_iterator<I, S>>;
template<input_iterator I, class S>
struct iterator_traits<common_iterator<I, S>>;
// 25.5.5, default sentinel
struct default_sentinel_t;
inline constexpr default_sentinel_t default_sentinel{};
// 25.5.6, counted iterators
template<input_or_output_iterator I> class counted_iterator;
template<input_iterator I>
requires see below
struct iterator_traits<counted_iterator<I>>;
// 25.5.7, unreachable sentinel
struct unreachable_sentinel_t;
inline constexpr unreachable_sentinel_t unreachable_sentinel{};
// 25.6, stream iterators
template<class T, class charT = char, class traits = char_traits<charT>,
class Distance = ptrdiff_t>
class istream_iterator;
template<class T, class charT, class traits, class Distance>
bool operator==(const istream_iterator<T,charT,traits,Distance>& x,
const istream_iterator<T,charT,traits,Distance>& y);
template<class T, class charT = char, class traits = char_traits<charT>>
class ostream_iterator;
template<class charT, class traits = char_traits<charT>>
class istreambuf_iterator;
template<class charT, class traits>
bool operator==(const istreambuf_iterator<charT,traits>& a,
const istreambuf_iterator<charT,traits>& b);
template<class charT, class traits = char_traits<charT>>
class ostreambuf_iterator;
// 25.7, range access
template<class C> constexpr auto begin(C& c) -> decltype(c.begin());
template<class C> constexpr auto begin(const C& c) -> decltype(c.begin());
template<class C> constexpr auto end(C& c) -> decltype(c.end());
template<class C> constexpr auto end(const C& c) -> decltype(c.end());
template<class T, size_t N> constexpr T* begin(T (&array)[N]) noexcept;
template<class T, size_t N> constexpr T* end(T (&array)[N]) noexcept;
template<class C> constexpr auto cbegin(const C& c)
  noexcept(noexcept(std::begin(c)))
-> decltype(std::begin(c));
template<class C> constexpr auto cend(const C& c)
  noexcept(noexcept(std::end(c)))
-> decltype(std::end(c));
template<class C> constexpr auto rbegin(C& c) -> decltype(c.rbegin());
template<class C> constexpr auto rbegin(const C& c) -> decltype(c.rbegin());
template<class C> constexpr auto rend(C& c) -> decltype(c.rend());
template<class C> constexpr auto rend(const C& c) -> decltype(c.rend());
template<class T, size_t N>
constexpr reverse_iterator<T*> rbegin(T (&array)[N]);
template<class T, size_t N> constexpr reverse_iterator<T*> rend(T (&array)[N]);
template<class E>
constexpr reverse_iterator<const E*> rbegin(initializer_list<E> il);
template<class E>
constexpr reverse_iterator<const E*> rend(initializer_list<E> il);
template<class C>
constexpr auto crbegin(const C& c) -> decltype(std::rbegin(c));
template<class C> constexpr auto crend(const C& c) -> decltype(std::rend(c));
template<class C> constexpr auto size(const C& c) -> decltype(c.size());
template<class T, size_t N> constexpr size_t size(const T (&array)[N]) noexcept;
template<class C> constexpr auto ssize(const C& c)
-> common_type_t<ptrdiff_t, make_signed_t<decltype(c.size())>>;
template<class T, ptrdiff_t N>
constexpr ptrdiff_t ssize(const T (&array)[N]) noexcept;
template<class C>
[[nodiscard]] constexpr auto empty(const C& c) -> decltype(c.empty());
template<class T, size_t N>
[[nodiscard]] constexpr bool empty(const T (&array)[N]) noexcept;
template<class E>
[[nodiscard]] constexpr bool empty(initializer_list<E> il) noexcept;
template<class C> constexpr auto data(C& c) -> decltype(c.data());
template<class C> constexpr auto data(const C& c) -> decltype(c.data());
template<class T, size_t N> constexpr T* data(T (&array)[N]) noexcept;
template<class E> constexpr const E* data(initializer_list<E> il) noexcept;
}
