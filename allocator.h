#ifndef RE_DEFINED_ALLOCATOR_H
#define RE_DEFINED_ALLOCATOR_H

#include "base.h"
#include "range.h"

// allocator_traits
namespace re {

template <class>
struct allocator_traits;
namespace inner {

struct alloc_traits_get_pointer {
  template <class AL>
  static type_tag<typename AL::pointer> f(type_pack<AL>);
};

struct alloc_traits_get_const_pointer_0 {
  template <class AL>
  static type_tag<typename AL::const_pointer> f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_const_pointer_0, AL>>
struct alloc_traits_get_const_pointer {
  using type = typename AL::const_pointer;
};
template <class AL>
struct alloc_traits_get_const_pointer<AL, false> {
  using pointer = typename allocator_traits<AL>::pointer;
  using value_type = typename allocator_traits<AL>::value_type;
  using type = typename pointer_traits<pointer>
    ::template rebind<const value_type>;
};

struct alloc_traits_get_void_pointer_0 {
  template <class AL>
  static type_tag<typename AL::void_pointer> f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_void_pointer_0, AL>>
struct alloc_traits_get_void_pointer {
  using type = typename AL::void_pointer;
};
template <class AL>
struct alloc_traits_get_void_pointer<AL, false> {
  using pointer = typename allocator_traits<AL>::pointer;
  using type = typename pointer_traits<pointer>::template rebind<void>;
};

struct alloc_traits_get_const_void_pointer_0 {
  template <class AL>
  static type_tag<typename AL::const_void_pointer> f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v
          <alloc_traits_get_const_void_pointer_0, AL>>
struct alloc_traits_get_const_void_pointer {
  using type = typename AL::const_void_pointer;
};
template <class AL>
struct alloc_traits_get_const_void_pointer<AL, false> {
  using pointer = typename allocator_traits<AL>::pointer;
  using type = typename pointer_traits<pointer>::template rebind<const void>;
};

struct alloc_traits_get_difference_type_0 {
  template <class AL>
  static type_tag<typename AL::difference_type> f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_difference_type_0, AL>>
struct alloc_traits_get_difference_type {
  using type = typename AL::difference_type;
};
template <class AL>
struct alloc_traits_get_difference_type<AL, false> {
  using pointer = typename allocator_traits<AL>::pointer;
  using type = typename pointer_traits<pointer>::difference_type;
};

struct alloc_traits_get_size_type_0 {
  template <class AL>
  static type_tag<typename AL::size_type> f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_size_type_0, AL>>
struct alloc_traits_get_size_type {
  using type = typename AL::size_type;
};
template <class AL>
struct alloc_traits_get_size_type<AL, false> {
  using difference_type = typename allocator_traits<AL>::difference_type;
  using type = make_unsigned_t<difference_type>;
};

struct alloc_traits_get_propg_on_copy_0 {
  template <class AL>
  static type_tag<typename AL::propagate_on_container_copy_assignment>
  f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_propg_on_copy_0, AL>>
struct alloc_traits_get_propg_on_copy {
  using type = typename AL::propagate_on_container_copy_assignment;
};
template <class AL>
struct alloc_traits_get_propg_on_copy<AL, false> {
  using type = false_type;
};

struct alloc_traits_get_propg_on_move_0 {
  template <class AL>
  static type_tag<typename AL::propagate_on_container_move_assignment>
  f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_propg_on_move_0, AL>>
struct alloc_traits_get_propg_on_move {
  using type = typename AL::propagate_on_container_move_assignment;
};
template <class AL>
struct alloc_traits_get_propg_on_move<AL, false> {
  using type = false_type;
};

struct alloc_traits_get_propg_on_swap_0 {
  template <class AL>
  static type_tag<typename AL::propagate_on_container_swap>
  f(type_pack<AL>);
};
template <class AL,
          bool = f_is_well_formed_v<alloc_traits_get_propg_on_swap_0, AL>>
struct alloc_traits_get_propg_on_swap {
  using type = typename AL::propagate_on_container_swap;
};
template <class AL>
struct alloc_traits_get_propg_on_swap<AL, false> {
  using type = false_type;
};

struct alloc_traits_get_is_always_equal {
  template <class AL>
  static type_tag<typename AL::is_always_equal> f(type_pack<AL>);
};

struct alloc_traits_get_rebind_alloc_0 {
  template <class AL, class T>
  static type_tag<typename AL::template rebind<T>::other>
  f(type_pack<AL, T>);
};
template <class AL, class T,
          bool = f_is_well_formed_v<alloc_traits_get_rebind_alloc_0, AL, T>>
struct alloc_traits_get_rebind_alloc;
template <class AL, class T>
struct alloc_traits_get_rebind_alloc<AL, T, true> {
  using type = typename AL::template rebind<T>::other;
};
template <template <class, class...> class T, class U, class...S, class V>
struct alloc_traits_get_rebind_alloc<T<U, S...>, V, false> {
  using type = T<V, S...>;
};

struct check_alloc_is_capable_of_hint_allocation {
  template <class AL>
  static type_tag
  <decltype(declval<AL &>()
            .allocate(declval<typename allocator_traits<AL>::size_type>(),
                      declval<typename allocator_traits<AL>
                              ::const_void_pointer>()))>
  f(type_pack<AL>);
};

struct check_alloc_can_construct_by_member_function {
  template <class AL, class T, class...S>
  static type_tag<decltype(declval<AL &>()
                           .construct(declval<T *>(), declval<S>()...))>
  f(type_pack<AL, T, S...>);
};

struct check_alloc_can_destroy_by_member_function {
  template <class AL, class T>
  static type_tag<decltype(declval<AL &>().destroy(declval<T *>()))>
  f(type_pack<AL, T>);
};

struct check_alloc_has_member_function_max_size {
  template <class AL>
  static type_tag<decltype(declval<const AL &>().max_size())> f(type_pack<AL>);
};

struct check_alloc_can_select_on_container_copy_construction {
  template <class AL>
  static type_tag<decltype(declval<const AL &>()
                           .select_on_container_copy_construction())>
  f(type_pack<AL>);
};

struct allocator_traits_primary_tag {};

template <class A>
concept allocator_can_implicitly_allocate_1 = requires(A &a) {
  a.allocate();
};
template <class A>
concept allocator_can_implicitly_deallocate_1 = requires(A &a) {
  a.deallocate(nullptr);
};

}
template <class AL>
struct allocator_traits : inner::allocator_traits_primary_tag {
  using allocator_type = AL;

  using value_type = typename AL::value_type;
  using pointer = return_type_of_f_or_t
    <inner::alloc_traits_get_pointer, value_type *, AL>;
  using const_pointer
    = typename inner::alloc_traits_get_const_pointer<AL>::type;
  using void_pointer
    = typename inner::alloc_traits_get_void_pointer<AL>::type;
  using const_void_pointer
    = typename inner::alloc_traits_get_const_void_pointer<AL>::type;
  using difference_type
    = typename inner::alloc_traits_get_difference_type<AL>::type;
  using size_type
    = typename inner::alloc_traits_get_size_type<AL>::type;

  using propagate_on_container_copy_assignment
    = typename inner::alloc_traits_get_propg_on_copy<AL>::type;
  using propagate_on_container_move_assignment
    = typename inner::alloc_traits_get_propg_on_move<AL>::type;
  using propagate_on_container_swap
    = typename inner::alloc_traits_get_propg_on_swap<AL>::type;
  using is_always_equal = return_type_of_f_or_t
    <inner::alloc_traits_get_is_always_equal, is_empty<AL>, AL>;

  template <class T>
  using rebind_alloc
    = typename inner::alloc_traits_get_rebind_alloc<AL, T>::type;
  template <class T>
  using rebind_traits = allocator_traits<rebind_alloc<T>>;

  [[nodiscard]] static pointer allocate(AL &a) {
    if constexpr (inner::allocator_can_implicitly_allocate_1<AL>) {
      return a.allocate();
    }
    else {
      return a.allocate(1);
    }
  }
  [[nodiscard]] static pointer allocate(AL &a, size_type n) {
    return a.allocate(n);
  }
  [[nodiscard]] static pointer allocate(AL &a, size_type n,
                                        const_void_pointer hint) {
    if constexpr (f_is_well_formed_v
                  <inner::check_alloc_is_capable_of_hint_allocation,
                   AL>) {
      return a.allocate(n, hint);
    }
    else {
      return a.allocate(n);
    }
  }
  static void deallocate(AL &a, pointer p, size_type n) {
    a.deallocate(p, n);
  }
  static void deallocate(AL &a, pointer p) {
    if constexpr (inner::allocator_can_implicitly_deallocate_1<AL>) {
      a.deallocate(p);
    }
    else {
      a.deallocate(p, 1);
    }
  }

  template <class T, class...S>
  static void construct(AL &a, T *p, S &&...s) {
    if constexpr (f_is_well_formed_v
                  <inner::check_alloc_can_construct_by_member_function,
                   AL, T, S &&...>) {
      a.construct(p, forward<S>(s)...);
    }
    else {
      ::new(p) T(forward<S>(s)...);
    }
  }
  template <class T>
  static void destroy(AL &a, T *p) {
    if constexpr (f_is_well_formed_v
                  <inner::check_alloc_can_destroy_by_member_function,
                   AL, T>) {
      a.destroy(p);
    }
    else {
      p->~T();
    }
  }

  static size_type max_size(const AL &a) noexcept {
    if constexpr (f_is_well_formed_v
                  <inner::check_alloc_has_member_function_max_size,
                   AL>) {
      return a.max_size();
    }
    else {
      return integral_traits<size_type>::max() / sizeof(value_type);
    }
  }

  static AL select_on_container_copy_construction(const AL &al) {
    if constexpr (f_is_well_formed_v
                  <inner::check_alloc_can_select_on_container_copy_construction,
                   AL>) {
      return al.select_on_container_copy_construction();
    }
    else {
      return al;
    }
  }
};

template <class A>
using alloc_vt = typename allocator_traits<A>::value_type;
template <class A>
using alloc_ptr = typename allocator_traits<A>::pointer;
template <class A>
using alloc_cptr = typename allocator_traits<A>::const_pointer;
template <class A>
using alloc_void_ptr = typename allocator_traits<A>::void_pointer;
template <class A>
using alloc_const_void_ptr = typename allocator_traits<A>::const_void_pointer;
template <class A>
using alloc_dft = typename allocator_traits<A>::difference_type;
template <class A>
using alloc_szt = typename allocator_traits<A>::size_type;
template <class A, class T>
using alloc_rebind = typename allocator_traits<A>::template rebind_alloc<T>;
template <class A, class T>
using alloc_rebind_traits = typename allocator_traits<A>
  ::template rebind_traits<T>;
template <class A, class T>
using alloc_rebind_ptr = typename allocator_traits<A>
  ::template rebind_traits<T>::pointer;
template <class A, class T>
using alloc_rebind_cptr = typename allocator_traits<A>
  ::template rebind_traits<T>::const_pointer;
template <class A>
inline constexpr bool alloc_copy_prpg = allocator_traits<A>
  ::propagate_on_container_copy_assignment::value;
template <class A>
inline constexpr bool alloc_move_prpg = allocator_traits<A>
  ::propagate_on_container_move_assignment::value;
template <class A>
inline constexpr bool alloc_swap_prpg = allocator_traits<A>
  ::propagate_on_container_swap::value;
template <class A>
inline constexpr bool alloc_always_equal = allocator_traits<A>
  ::is_always_equal::value;

template <class AL>
concept allocator_with_primary_traits
  = is_base_of_v<inner::allocator_traits_primary_tag, allocator_traits<AL>>;
template <class AL, class T, class...S>
concept allocator_provides_construct_function
  = requires(AL &a, T *p, S &&...s) {a.construct(p, static_cast<S &&>(s)...);};
template <class AL, class T>
concept allocator_provides_destroy_function
  = requires(AL &a, T *p) {a.destroy(p);};
template <class AL, class T, class...S>
concept nothrow_constructible_by_allocator
  = !allocator_provides_construct_function<AL, T, S...>
  && is_nothrow_constructible_v<T, S...>
  && allocator_with_primary_traits<AL>;
template <class AL, class T>
concept nothrow_move_constructible_by_allocator
  = nothrow_constructible_by_allocator<AL, T, T>;

}

// uninitialized algorithms
namespace re {

struct fo_default_construct_at {
  template <class T>
  T *operator ()(T *p) const {
    return ::new(p) T;
  }
};
inline constexpr fo_default_construct_at default_construct_at{};

struct fo_construct_at {
  template <class T, class...S>
  T *operator ()(T *p, S &&...s) const {
    return ::new(p) T(forward<S>(s)...);
  }
};
inline constexpr fo_construct_at construct_at{};

struct fo_destroy_at {
  template <class T>
  constexpr void operator ()(T *p) const {
    p->~T();
  }
};
inline constexpr fo_destroy_at destroy_at{};

struct fo_initialize_plus {
  template <class IR, class F>
  constexpr rng_itr<IR> operator ()(IR &&r, F f) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      f(to_address(it));
    return it;
  }
  template <class IR, class FI, class F>
  constexpr pair<rng_itr<IR>, FI> operator ()(IR &&r, FI o, F f) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it) {
      f(it, to_address(o));
      ++o;
    }
    return {it, o};
  }
};
inline constexpr fo_initialize_plus initialize_plus{};
struct fo_initialize {
  template <class IR, class F>
  constexpr void operator ()(IR &&r, F f) const {
    initialize_plus(r, f);
  }
  template <class IR, class FI, class F>
  constexpr FI operator ()(IR &&r, FI o, F f) const {
    return initialize_plus(r, o, f).second;
  }
};
inline constexpr fo_initialize initialize{};

struct fo_securely_initialize_plus {
  template <class FR, class F, class F2>
  constexpr rng_itr<FR> operator ()(FR &&r, F f, F2 destroyf) const {
    const auto ed = end(r);
    auto it = begin(r);
#ifndef RE_NOEXCEPT
    try {
#endif
      for (; it != ed; ++it)
        f(to_address(it));
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      for (auto &j : iters(begin(r), it))
        destroyf(to_address(j));
      throw;
    }
#endif
    return it;
  }
  template <class FR, class FI, class F, class F2>
  constexpr pair<rng_itr<FR>, FI>
  operator ()(FR &&r, FI o, F f, F2 destroyf) const {
    const auto to_bk = o;
    const auto ed = end(r);
    auto it = begin(r);
#ifndef RE_NOEXCEPT
    try {
#endif
      for (; it != ed; ++it) {
        f(it, to_address(o));
        ++o;
      }
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      for (auto &p : iters(to_bk, o))
        destroyf(to_address(p));
      throw;
    }
#endif
    return {it, o};
  }
};
inline constexpr fo_securely_initialize_plus securely_initialize_plus{};
struct fo_securely_initialize {
  template <class FR, class F, class F2>
  constexpr void operator ()(FR &&r, F f, F2 destroyf) const {
    securely_initialize_plus(r, f, destroyf);
  }
  template <class FR, class FI, class F, class F2>
  constexpr FI operator ()(FR &&r, FI o, F f, F2 destroyf) const {
    return securely_initialize_plus(r, o, f, destroyf).second;
  }
};
inline constexpr fo_securely_initialize securely_initialize{};

}

// default allocator
namespace re {

template <class T>
class allocator {
public:
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using propagate_on_container_move_assignment = true_type;

  allocator() noexcept = default;
  ~allocator() = default;
  allocator(const allocator &) = default;
  allocator &operator =(const allocator &) = default;
  allocator(allocator &&) = default;
  allocator &operator =(allocator &&) = default;

  template <class U>
  allocator(const allocator<U> &) noexcept {}

  T *allocate(size_type n) {
    if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
      if (n > max_size())
        throw_or_abort<bad_alloc>();
      void *const p = operator new
        (n * sizeof(T), align_val_t(alignof(T)), nothrow);
      if (p == nullptr)
        throw_or_abort<bad_alloc>();
      return reinterpret_cast<T *>(p);
    }
    else {
      if (n > max_size())
        throw_or_abort<bad_alloc>();
      void *const p = operator new(n * sizeof(T), nothrow);
      if (p == nullptr)
        throw_or_abort<bad_alloc>();
      return reinterpret_cast<T *>(p);
    }
  }
  void deallocate(T *p, size_type) {
    if constexpr (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
      operator delete(p, align_val_t(alignof(T)), nothrow);
    }
    else {
      operator delete(p, nothrow);
    }
  }

  size_type max_size() const {
    return to_unsigned(integral_traits<ptrdiff_t>::max()) / sizeof(value_type);
  }
  size_t min_alignment() const {
    return max_value(alignof(value_type),
                     static_cast<size_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));
  }
};
template <class T, class T2>
bool operator ==(const allocator<T> &, const allocator<T2> &) noexcept {
  return true;
}

template <class T>
#ifdef RE_DEFAULT_ALLOCATOR
using default_allocator = RE_DEFAULT_ALLOCATOR<T>;
#else
using default_allocator = allocator<T>;
#endif

}

// allocator_wrapper 
//   = allocator + allocator_traits + uninitialized algorithms
namespace re {

template <class>
class alloc_delete;
template <class, class>
class unique_ptr;
template <class, class>
class unique_array;

template <class AL>
class allocator_wrapper : public AL {
  using this_t = allocator_wrapper;

  template <class>
  friend class allocator_aware_container_ownership;

public:
  using allocator_type = AL;
  using traits = allocator_traits<AL>;

  allocator_wrapper() noexcept(is_nothrow_default_constructible_v<AL>) : AL() {}
  ~allocator_wrapper() = default;
  allocator_wrapper(const this_t &) = default;
  allocator_wrapper &operator =(const this_t &) = default;
  allocator_wrapper(this_t &&) = default;
  allocator_wrapper &operator =(this_t &&) = default;
  friend void swap(allocator_wrapper &x, allocator_wrapper &y)
    noexcept(is_nothrow_swappable_v<AL>) {
    adl_swap(x.get(), y.get());
  }

  explicit allocator_wrapper(const allocator_type &a) : AL(a) {}
  template <class...S>
  allocator_wrapper(S &&...s)
    requires (sizeof...(S) != 1 && is_constructible_v<AL, S &&...>)
    : AL(forward<S>(s)...) {}

  AL &get() noexcept {
    return *this;
  }
  const AL &get() const noexcept {
    return *this;
  }

  template <class T>
  auto rebind() const noexcept {
    return allocator_wrapper<alloc_rebind<AL, T>>(get());
  }
  alloc_szt<AL> max_size() const noexcept {
    return traits::max_size(get());
  }
  alloc_szt<AL> min_alignment() const noexcept {
    return alignof(alloc_vt<AL>);
  }
  alloc_szt<AL> min_alignment() const noexcept
    requires (requires(const AL &a) {a.min_alignment();}) {
    return get().min_alignment();
  }

  [[nodiscard]] pair<alloc_ptr<AL>, alloc_ptr<AL>>
  allocate_alignas(alloc_szt<AL> algn, alloc_szt<AL> n = 1)
    requires is_same_v<alloc_vt<AL>, byte> {
    if (algn <= min_alignment()) {
      const alloc_ptr<AL> p = allocate(n);
      return {p, p};
    }
    else {
      using szt = alloc_szt<AL>;
      if (algn > numeric_limits<szt>::max() - n)
        throw_or_abort<length_error>
          ("re::allocator_wrapper::allocate_alignas: too big size\n");
      szt nn = n + algn;
      const alloc_ptr<AL> p = allocate(nn);
      void *bp = reinterpret_cast<void *>(to_address(p));
      align(algn, n, bp, nn);
      return {pointer_to<alloc_ptr<AL>>(*reinterpret_cast<byte *>(bp)), p};
    }
  }
  void deallocate_alignas(alloc_szt<AL> algn,
                          pair<alloc_ptr<AL>, alloc_ptr<AL>> p,
                          alloc_szt<AL> n = 1)
    requires is_same_v<alloc_vt<AL>, byte> {
    deallocate(p.second, (algn <= min_alignment()) ? n : (n + algn));
  }

  [[nodiscard]] alloc_ptr<AL> allocate() {
    return traits::allocate(get());
  }
  [[nodiscard]] alloc_ptr<AL> allocate(alloc_szt<AL> n) {
    return traits::allocate(get(), n);
  }
  void deallocate(alloc_ptr<AL> p) {
    traits::deallocate(get(), p);
  }
  void deallocate(alloc_ptr<AL> p, alloc_szt<AL> n) {
    traits::deallocate(get(), p, n);
  }
  template <class T, class...S>
  void construct(T *p, S &&...s) {
    traits::construct(get(), p, forward<S>(s)...);
  }
  template <class T>
  void destroy(T *p) noexcept {
    traits::destroy(get(), p);
  }
  template <class FR>
  void destroy(FR &&r) requires is_rng<FR> {
    for (auto &i : iters(r))
      destroy(to_address(i));
  }

  class destroy_function {
    friend class allocator_wrapper;

    allocator_wrapper *alw_p;

    explicit destroy_function(allocator_wrapper *p) : alw_p(p) {}

  public:
    destroy_function() = delete;
    ~destroy_function() = default;
    destroy_function(const destroy_function &) = default;
    destroy_function &operator =(const destroy_function &) = default;
    destroy_function(destroy_function &&) = default;
    destroy_function &operator =(destroy_function &&) = default;

    void operator ()(alloc_vt<AL> *p) const noexcept {
      alw_p->destroy(p);
    }
  };
  destroy_function destroy_fn() noexcept {
    return destroy_function(this);
  }

  template <class FR>
  rng_itr<FR> uninitialized_fill_plus(FR &&r) {
    return securely_initialize_plus(r, [=, this](auto p) {this->construct(p);},
                                    destroy_fn());
  }
  template <class FR>
  void uninitialized_fill(FR &&r) {
    uninitialized_fill_plus(r);
  }

  template <class FR, class T>
  rng_itr<FR> uninitialized_fill_plus(FR &&r, const T &x) {
    return securely_initialize_plus
      (r, [&x, this](auto p) {this->construct(p, x);}, destroy_fn());
  }
  template <class FR, class T>
  void uninitialized_fill(FR &&r, const T &x) {
    uninitialized_fill_plus(r, x);
  }

  template <class IR, class FI>
  pair<rng_itr<IR>, FI> uninitialized_copy_plus(IR &&r, FI o) {
    return securely_initialize_plus
      (r, o,
       [this](auto r_p, auto o_p) {this->construct(o_p, *r_p);},
       destroy_fn());
  }
  template <class IR, class FI>
  FI uninitialized_copy(IR &&r, FI o) {
    return uninitialized_copy_plus(r, o).second;
  }

  template <class IR, class FI>
  pair<rng_itr<IR>, FI> uninitialized_move_plus(IR &&r, FI o) {
    return securely_initialize_plus
      (r, o,
       [this](auto r_p, auto o_p) {this->construct(o_p, move(*r_p));},
       destroy_fn());
  }
  template <class IR, class FI>
  FI uninitialized_move(IR &&r, FI o) {
    return uninitialized_move_plus(r, o).second;
  }

  template <class FR, class FI, class UF>
  pair<rng_itr<FR>, FI> uninitialized_fully_move_plus(FR &&r, FI o,
                                                      UF destroyf) {
    if constexpr (nothrow_constructible_by_allocator
                  <allocator_type, itr_vt<FI>, rng_rref<FR>>) {
      return initialize_plus(r, o,
                             [destroyf, this](auto r_p, auto o_p) {
                               this->construct(o_p, move(*r_p));
                               destroyf(to_address(r_p));
                             });
    }
    else {
      const auto oo = securely_initialize_plus
        (r, o,
         [this](auto r_p, auto o_p) {this->construct(o_p, *r_p);},
         destroy_fn());
      initialize(r, destroyf);
      return oo;
    }
  }
  template <class FR, class FI>
  pair<rng_itr<FR>, FI> uninitialized_fully_move_plus(FR &&r, FI o) {
    return uninitialized_fully_move_plus(r, o, destroy_fn());
  }
  template <class FR, class FI, class UF>
  FI uninitialized_fully_move(FR &&r, FI o, UF destroyf) {
    return uninitialized_fully_move_plus(r, o, destroyf).second;
  }
  template <class FR, class FI>
  FI uninitialized_fully_move(FR &&r, FI o) {
    return uninitialized_fully_move(r, o, destroy_fn());
  }

  template <class FR, class FI, class UF>
  pair<rng_itr<FR>, FI> fully_move_plus(FR &&r, FI o, UF destroyf) {
    if constexpr (can_memmove_from_range_to_iterator<rng_itr<FR>, FI>) {
      const auto len = size(r);
      const auto i = begin(r);
      memmove(reinterpret_cast<void *>(to_address(o)),
              reinterpret_cast<const void *>(to_address(i)),
              len * sizeof(itr_vt<FI>));
      initialize(r, destroyf);
      return {i + len, o + len};
    }
    else if constexpr (is_nothrow_assignable_v<itr_ref<FI>, rng_rref<FR>>) {
      return initialize_plus(r, o, [=](auto r_p, auto o_p) {
        *o_p = move(*r_p);
        destroyf(to_address(r_p));
      });
    }
    else {
      auto oo = initialize_plus(r, o, bind(assign_to, bind(deref, _1),
                                           bind(deref, _2)));
      initialize(r, destroyf);
      return oo;
    }
  }
  template <class FR, class FI>
  pair<rng_itr<FR>, FI> fully_move_plus(FR &&r, FI o) {
    return fully_move_plus(r, o, destroy_fn());
  }
  template <class FR, class FI, class UF>
  FI fully_move(FR &&r, FI o, UF destroyf) {
    return fully_move_plus(r, o, destroyf).second;
  }
  template <class FR, class FI>
  FI fully_move(FR &&r, FI o) {
    return fully_move(r, o, destroy_fn());
  }

  template <class...S>
  [[nodiscard]] alloc_ptr<AL> new_1(S &&...s) {
    auto p = allocate();
#ifndef RE_NOEXCEPT
    try {
#endif
      construct(to_address(p), forward<S>(s)...);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p);
      throw;
    }
#endif
    return p;
  }
  [[nodiscard]] alloc_ptr<AL> new_n(alloc_szt<AL> n) {
    auto p = allocate(n);
#ifndef RE_NOEXCEPT
    try {
#endif
      uninitialized_fill(rng(p, n));
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p, n);
      throw;
    }
#endif
    return p; 
  }
  [[nodiscard]] alloc_ptr<AL> new_n(alloc_szt<AL> n,
                                    const alloc_vt<AL> &x) {
    auto p = allocate(n);
#ifndef RE_NOEXCEPT
    try {
#endif
      uninitialized_fill(rng(p, n), x);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p, n);
      throw;
    }
#endif
    return p;
  }
  void delete_1(alloc_ptr<AL> p) noexcept {
    destroy(to_address(p));
    deallocate(p);
  }
  void delete_n(alloc_ptr<AL> p, alloc_szt<AL> n) noexcept {
    destroy(rng(p, n));
    deallocate(p, n);
  }

  template <class...S>
  [[nodiscard]] alloc_ptr<AL> new_1_with_placement_new(S &&...s) {
    alloc_ptr<AL> p = allocate();
#ifndef RE_NOEXCEPT
    try {
#endif
      construct(to_address(p), forward<S>(s)...);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p);
      throw;
    }
#endif
    return p;
  }

  template <class NODE_T, class...S>
  auto new_node(S &&...s) {
    auto node_alw = rebind<NODE_T>();
    auto p = node_alw.new_1();
#ifndef RE_NOEXCEPT
    try {
#endif
      construct(reinterpret_cast<alloc_vt<AL> *>(addressof(p->data)),
                forward<S>(s)...);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      node_alw.delete_1(p);
      throw;
    }
#endif
    return p;
  }
  template <class P>
  void delete_node(P p) noexcept {
    using node_t = pointer_traits<P>::element_type;
    static_assert(is_same_v<alloc_rebind_ptr<AL, node_t>, P>);
    traits::destroy(get(),
                    reinterpret_cast<alloc_vt<AL> *>(addressof(p->data)));
    rebind<node_t>().delete_1(p);
  }

  class temporary {
    friend class allocator_wrapper;

    using value_type = typename traits::value_type;

    alignas(value_type) byte buf[sizeof(value_type)];
    allocator_wrapper a;

    value_type *ptr() noexcept {
      return reinterpret_cast<value_type *>(addressof(buf));
    }
    const value_type *ptr() const noexcept {
      return reinterpret_cast<const value_type *>(addressof(buf));
    }

  public:
    temporary() = delete;
    ~temporary() {
      a.destroy(ptr());
    }
    temporary(const temporary &) = delete;
    temporary &operator =(const temporary &) = delete;
    temporary(temporary &&x) : a(x.a) {
      a.construct(ptr(), move(*x));
    }
    temporary &operator =(temporary &&) = delete;

  private:
    template <class...S>
    explicit(sizeof...(S) == 0)
    temporary(const allocator_wrapper &aa, S &&...s) : a(aa) {
      a.construct(ptr(), forward<S>(s)...);
    }

  public:
    value_type *operator ->() noexcept {
      return ptr();
    }
    const value_type *operator ->() const noexcept {
      return ptr();
    }

    value_type &operator *() noexcept {
      return *ptr();
    }
    const value_type &operator *() const noexcept {
      return *ptr();
    }
  };
  template <class...S>
  temporary make_temporary(S &&...s) {
    return temporary(*this, forward<S>(s)...);
  }

  class uninitialized {
    friend class allocator_wrapper;

    alloc_ptr<AL> p;
    alloc_szt<AL> n;
    allocator_wrapper a;

    void init() {
      p = nullptr;
      n = 0;
    }
    void assign_from(uninitialized &x) {
      p = x.p;
      n = x.n;
      a = x.a;
    }
    void clean() {
      if (p != nullptr)
        a.deallocate(p, n);
    }

    uninitialized(alloc_ptr<AL> pp, alloc_szt<AL> nn,
                  const allocator_wrapper &aa)
      : p(pp), n(nn), a(aa) {}

  public:
    uninitialized() = delete;
    ~uninitialized() {
      clean();
    }
    uninitialized(const uninitialized &) = delete;
    uninitialized &operator =(const uninitialized &) = delete;
    uninitialized(uninitialized &&x) noexcept : p(x.p), n(x.n), a(x.a) {
      x.init();
    }
    uninitialized &operator =(uninitialized &&x) noexcept {
      copy_and_swap(move(x), *this);
      return *this;
    }
    friend void swap(uninitialized &x, uninitialized &y) noexcept {
      adl_swap(x.p, y.p);
      adl_swap(x.n, y.n);
      adl_swap(x.a, y.a);
    }

    alloc_ptr<AL> begin() const noexcept {
      return p;
    }
    alloc_ptr<AL> end() const noexcept {
      return p + n;
    }
    bool empty() const noexcept {
      return begin() == end();
    }
    alloc_szt<AL> size() const noexcept {
      return n;
    }

    alloc_ptr<AL> release() noexcept {
      auto ret = p;
      init();
      return ret;
    }
  };
  uninitialized make_uninitialized(alloc_szt<AL> n) {
    return (n == 0) ? uninitialized(nullptr, 0, *this)
      : uninitialized(allocate(n), n, *this);
  }

  template <class H, class U>
  requires is_same_v<alloc_vt<AL>, byte>
  class headed_buffer_ptr {
    friend class allocator_wrapper;

    pair<alloc_ptr<AL>, alloc_ptr<AL>> p{};
    alloc_szt<AL> n{};
    alloc_ptr<AL> op{};
    alloc_szt<AL> sz{};

    headed_buffer_ptr(pair<alloc_ptr<AL>, alloc_ptr<AL>> p2, alloc_szt<AL> n2,
                      alloc_ptr<AL> op2, alloc_szt<AL> sz2)
      : p(p2), n(n2), op(op2), sz(sz2) {}

  public:
    using head_type = H;
    using value_type = U;

    headed_buffer_ptr() = default;
    ~headed_buffer_ptr() = default;
    headed_buffer_ptr(const headed_buffer_ptr &) = default;
    headed_buffer_ptr &operator =(const headed_buffer_ptr &) = default;
    headed_buffer_ptr(headed_buffer_ptr &&) = default;
    headed_buffer_ptr &operator =(headed_buffer_ptr &&) = default;
    friend void swap(headed_buffer_ptr &x, headed_buffer_ptr &y) noexcept {
      adl_swap(x.p, y.p);
      adl_swap(x.n, y.n);
      adl_swap(x.op, y.op);
      adl_swap(x.sz, y.sz);
    }

    headed_buffer_ptr(nullptr_t) : headed_buffer_ptr() {}
    headed_buffer_ptr &operator =(nullptr_t) {
      return *this = headed_buffer_ptr{};
    }
    bool operator ==(nullptr_t) const {
      return p == pair<alloc_ptr<AL>, alloc_ptr<AL>>{};
    }

    H &head() const {
      return reinterpret_cast<H &>(*p.first);
    }
    alloc_rebind_ptr<AL, U> data() const {
      return pointer_to<alloc_ptr<AL>>(reinterpret_cast<U &>(*op));
    }
    alloc_szt<AL> size() const {
      return sz;
    }

    void refer_to_only_head(H &x) {
      p.first = pointer_to<alloc_ptr<AL>>(reinterpret_cast<byte &>(x));
      p.second = nullptr;
      n = 0;
      op = nullptr;
      sz = 0;
    }
  };
  template <class H, class U, class...S>
  headed_buffer_ptr<H, U> allocate_headed_buffer(alloc_szt<AL> n, S &&...s)
    requires is_same_v<alloc_vt<AL>, byte> {
    const size_t max_align = max_value(alignof(H), alignof(U));
    const size_t b = sizeof(H) + max_align;
    const alloc_szt<AL> k = sizeof(U) * max_value(1u, n);
    const alloc_szt<AL> nn = b + k;
    const auto p = allocate_alignas(alignof(H), nn);

    void *data_p = static_cast<void *>(to_address(p.first));
    if (alignof(H) >= alignof(U))
      data_p = static_cast<void *>(static_cast<byte *>(data_p) + sizeof(H));
    else {
      size_t holder = k + max_align;
      align(alignof(U), n, data_p, holder);
    }
#ifndef RE_NOEXCEPT
    try {
#endif
      new(reinterpret_cast<H *>(to_address(p.first))) H(forward<S>(s)...);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate_alignas(alignof(H), p);
      throw;
    }
#endif
    return {p, nn, pointer_to<alloc_ptr<AL>>(*static_cast<byte *>(data_p)), n};
  }
  template <class H, class U>
  void deallocate_headed_buffer(headed_buffer_ptr<H, U> p)
    requires is_same_v<alloc_vt<AL>, byte> {
    p.head().~H();
    deallocate_alignas(alignof(H), p.p, p.n);
  }

  using pointer_pair = iter_pair<alloc_ptr<AL>>;
  pointer_pair new_array(alloc_szt<AL> n) {
    return n != 0 ? rng(new_n(n), n) : pointer_pair{};
  }
  pointer_pair new_array(alloc_szt<AL> n, const alloc_vt<AL> &x) {
    if (n != 0)
      return rng(new_n(n, x), n);
    return {};
  }
  template <class IR>
  pointer_pair new_array(IR &&r) requires is_rng<IR> {
    if (empty(r))
      return pointer_pair{};
    const auto n = size(r);
    if (n > numeric_limits<size_t>::max())
      throw_or_abort<length_error>
        ("re::allocator_wrapper::new_array: too big size\n");
    const auto p = allocate(n);
#ifndef RE_NOEXCEPT
    try {
#endif
      uninitialized_copy(r, p);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p, n);
      throw;
    }
#endif
    return rng(p, n);
  }
  pointer_pair new_array_move_individually(pointer_pair &x, AL &x_al) {
    if (empty(x))
      return pointer_pair{};
    this_t x_alw(x_al);
    const auto n = size(x);
    const auto p = allocate(n);
#ifndef RE_NOEXCEPT
    try {
#endif
      uninitialized_fully_move(x, p, x_alw.destroy_fn());
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      deallocate(p, n);
      throw;
    }
#endif
    x_alw.deallocate(begin(x), n);
    x = {};
    return rng(p, n);
  }
  pointer_pair new_array_move_individually(pointer_pair &x) {
    return new_array_move_individually(x, get());
  }
  void delete_array(pointer_pair x) noexcept {
    if (!empty(x))
      delete_n(begin(x), size(x));
  }

  using unique_ptr = re::unique_ptr<alloc_vt<AL>, alloc_delete<AL>>;
  using unique_array = re::unique_array<alloc_vt<AL>, alloc_delete<AL>>;
  template <class...S>
  unique_ptr make_unique(S &&...s) {
    return unique_ptr(new_1(forward<S>(s)...),
                      alloc_delete<allocator_type>(get()));
  }
  unique_array make_array(alloc_szt<AL> n) {
    return unique_array(new_array(n), alloc_delete<allocator_type>(get()));
  }
  unique_array make_array(alloc_szt<AL> n, const alloc_vt<AL> &x) {
    return unique_array(new_array(n, x), alloc_delete<allocator_type>(get()));
  }
  template <class IR>
  unique_array make_array(IR &&r) requires is_rng<IR> {
    return unique_array(new_array(r), alloc_delete<allocator_type>(get()));
  }
};

template <class T>
using default_alloc_wrapper = allocator_wrapper<default_allocator<T>>;

}

// unique_ptr
// unique_array
namespace re {

template <class AL>
class alloc_delete : allocator_wrapper<AL> {
  template <class>
  friend class alloc_delete;

  using alw_t = allocator_wrapper<AL>;

  AL &alloc_ref() noexcept {
    return *this;
  }
  const AL &alloc_ref() const noexcept {
    return *this;
  }

public:
  using pointer = alloc_ptr<AL>;
  using pointer_pair = iter_pair<pointer>;
  using size_type = alloc_szt<AL>;

  alloc_delete() = default;
  ~alloc_delete() = default;
  alloc_delete(const alloc_delete &) = default;
  alloc_delete &operator =(const alloc_delete &) = default;
  alloc_delete(alloc_delete &&) = default;
  alloc_delete &operator =(alloc_delete &&) = default;
  friend void swap(alloc_delete &x, alloc_delete &y) noexcept {
    adl_swap(x.alloc_ref(), y.alloc_ref());
  }

  alloc_delete(const AL &a) noexcept : alw_t(a) {}

  template <class AL2>
  alloc_delete(const alloc_delete<AL2> &x) noexcept
    requires is_convertible_v<const AL2 &, AL>
    : alw_t(static_cast<const AL2 &>(x)) {}

  void operator ()(pointer p) noexcept {
    alw_t::delete_1(p);
  }
  void operator ()(iter_pair<pointer> x) noexcept {
    alw_t::delete_array(x);
  }

  using allocator_type = AL;
  AL get_allocator() const noexcept {
    return alloc_ref();
  }
};

template <class T>
using default_delete = alloc_delete<default_allocator<T>>;

namespace inner {

struct check_deleter_has_pointer_type_member {
  template <class T>
  static type_tag<typename remove_reference_t<T>::pointer> f(type_pack<T>);
};
template <class T>
struct deleter_has_pointer_type_member
  : f_is_well_formed<inner::check_deleter_has_pointer_type_member, T> {};
template <class T>
inline constexpr bool deleter_has_pointer_type_member_v
  = deleter_has_pointer_type_member<T>::value;

template <class T, class D, bool = deleter_has_pointer_type_member_v<D>>
struct unique_ptr_pointer_type {
  using type = typename remove_reference_t<D>::pointer;
};
template <class T, class D>
struct unique_ptr_pointer_type<T, D, false> {
  using type = T *;
};
template <class T, class D>
using unique_ptr_pointer_type_t = typename unique_ptr_pointer_type<T, D>::type;

}
template <class T, class D = default_delete<T>>
class unique_ptr : derivable_wrapper<D> {
  template <class, class>
  friend class unique_ptr;

  using this_t = unique_ptr;
  using base_t = derivable_wrapper<D>;
  
  base_t &base() noexcept {
    return *this;
  }
  const base_t &base() const noexcept {
    return *this;
  }

  inner::unique_ptr_pointer_type_t<T, D> p;

public:
  using pointer = inner::unique_ptr_pointer_type_t<T, D>;
  using element_type = T;
  using deleter_type = D;

  unique_ptr() noexcept : p(nullptr) {}
  ~unique_ptr() {
    if (p != nullptr)
      (*base())(p);
  }
  unique_ptr(const unique_ptr &) = delete;
  unique_ptr &operator =(const unique_ptr &) = delete;
  unique_ptr(unique_ptr &&x) noexcept
    requires is_move_constructible_v<D>
    : base_t(move(x.base())), p(x.p) {
    x.p = nullptr;
  }
  unique_ptr &operator =(unique_ptr &&x) noexcept
    requires is_move_assignable_v<D> {
    reset(x.release());
    base() = move(x.base());
    return *this;
  }
  friend void swap(unique_ptr &x, unique_ptr &y) noexcept
    requires is_nothrow_swappable_v<D> {
    adl_swap(x.base(), y.base());
    adl_swap(x.p, y.p);
  }

  unique_ptr(nullptr_t) noexcept : p(nullptr) {}
  unique_ptr &operator =(nullptr_t) noexcept {
    reset();
    return *this;
  }

  template <class T2, class D2>
  unique_ptr(unique_ptr<T2, D2> &&u) noexcept
    requires (is_convertible_v<typename unique_ptr<T2, D2>::pointer, pointer>
              && !is_array_v<T2>
              && ((is_reference_v<D> && is_same_v<D2, D>)
                  || (!is_reference_v<D> && is_convertible_v<D2, D>)))
    : base_t(in_place, forward<D2>(*u.base())), p(u.p) {
    u.p = nullptr;
  }
  template <class T2, class D2>
  unique_ptr &operator =(unique_ptr<T2, D2> &&u) noexcept
    requires (is_convertible_v<typename unique_ptr<T2, D2>::pointer, pointer>
              && !is_array_v<T2> && is_assignable_v<D &, D2 &&>) {
    reset(u.release());
    *base() = forward<D2>(*u.base());
    return *this;
  }

  explicit unique_ptr(type_identity_t<pointer> pp) noexcept
    requires (!is_pointer_v<deleter_type>
              && is_default_constructible_v<deleter_type>) : p(pp) {}
  unique_ptr(type_identity_t<pointer> pp, const D &d) noexcept
    : base_t(in_place, d), p(pp) {}
  unique_ptr(type_identity_t<pointer> pp, remove_reference_t<D> &&d) noexcept
    : base_t(in_place, move(d)), p(pp) {}

  add_lvalue_reference_t<T> operator *() const {
    return *p;
  }
  pointer operator ->() const noexcept {
    return p;
  }
  pointer get() const noexcept {
    return p;
  }

  bool empty() const noexcept {
    return p == nullptr;
  }
  void clear() noexcept {
    reset();
  }
  explicit operator bool() const noexcept {
    return !empty();
  }

  deleter_type &get_deleter() noexcept {
    return *base();
  }
  const deleter_type &get_deleter() const noexcept {
    return *base();
  }

  pointer release() noexcept {
    return exchange(p, nullptr);
  }
  void reset(pointer pp = pointer{}) noexcept {
    if (auto old = exchange(p, pp); old != nullptr)
      (*base())(old);
  }
};
template <class T1, class D1, class T2, class D2>
bool operator ==(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y) {
  return x.get() == y.get();
}
template <class T1, class D1, class T2, class D2>
synth_3way_result<common_type_t<typename unique_ptr<T1, D1>::pointer,
                                typename unique_ptr<T2, D2>::pointer>>
operator <=>(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y) {
  using common_t = common_type_t<typename unique_ptr<T1, D1>::pointer,
                                 typename unique_ptr<T2, D2>::pointer>;
  return synth_3way(static_cast<const common_t &>(x.get()),
                    static_cast<const common_t &>(y.get()));
}
template <class T, class D>
bool operator ==(const unique_ptr<T, D> &x, nullptr_t) noexcept {
  return !x;
}
template <class T, class D>
strong_ordering operator <=>(const unique_ptr<T, D> &x,
                             nullptr_t) noexcept {
  return x != nullptr ? strong_gt : strong_eq;
}
template <class T, class D>
class unique_ptr<T [], D>;
template <class T, class D>
struct hash<unique_ptr<T, D>> {
  size_t operator ()(const unique_ptr<T, D> &x) const {
    return x != nullptr ? hash<>{}(*x) : 0u;
  }
};

template <class T>
struct fo_make_unique {
  template <class...S>
  unique_ptr<T> operator ()(S &&...s) const {
    using alw_t = allocator_wrapper<default_allocator<T>>;
    return alw_t{}.make_unique(forward<S>(s)...);
  }
};
template <class T>
inline constexpr fo_make_unique<T> make_unique{};

namespace inner {

template <class T, class D>
struct unique_array_size_type {
  using type = size_t;
};
template <class T, class D>
requires requires {typename D::size_type;}
struct unique_array_size_type<T, D> {
  using type = typename D::size_type;
};
template <class T, class D>
using unique_array_size_type_t = typename unique_array_size_type<T, D>::type;

}
template <class T, class D = default_delete<T>>
class unique_array : derivable_wrapper<D> {
  template <class, class>
  friend class unique_array;

  using this_t = unique_array;
  using base_t = derivable_wrapper<D>;

  base_t &base() noexcept {
    return *this;
  }
  const base_t &base() const noexcept {
    return *this;
  }

  iter_pair<inner::unique_ptr_pointer_type_t<T, D>> p;

public:
  using pointer = inner::unique_ptr_pointer_type_t<T, D>;
  using size_type = inner::unique_array_size_type_t<T, D>;
  using element_type = T;
  using deleter_type = D;
  using pointer_pair = iter_pair<pointer>;

  unique_array() noexcept : p{} {}
  ~unique_array() {
    if (!p.empty())
      (*base())(p);
  }
  unique_array(const unique_array &) = delete;
  unique_array &operator =(const unique_array &) = delete;
  unique_array(unique_array &&x) noexcept
    requires is_move_constructible_v<D>
    : base_t(move(x.base())), p(x.p) {
    x.p = {};
  }
  unique_array &operator =(unique_array &&x) noexcept
    requires is_move_assignable_v<D> {
    reset(x.release());
    base() = move(x.base());
    return *this;
  }
  friend void swap(unique_array &x, unique_array &y) noexcept
    requires is_nothrow_swappable_v<D> {
    adl_swap(x.base(), y.base());
    adl_swap(x.p, y.p);
  }

  template <class T2, class D2>
  unique_array(unique_array<T2, D2> &&u) noexcept
    requires (is_convertible_v<typename unique_array<T2, D2>::pointer, pointer>
              && is_same_v
              <typename pointer_traits
               <typename unique_array<T2, D2>::pointer>::element_type,
               typename pointer_traits<pointer>::element_type>
              && is_convertible_v
              <typename unique_array<T2, D2>::size_type, size_type>
              && is_convertible_v<D2, D>)
  : base_t(in_place, forward<D2>(*u.base())), p(u.p) {
    u.p = {};
  }
  template <class T2, class D2>
  unique_array &operator =(unique_array<T2, D2> &&u) noexcept
    requires (is_convertible_v<typename unique_array<T2, D2>::pointer, pointer>
              && is_same_v
              <typename pointer_traits
               <typename unique_array<T2, D2>::pointer>::element_type,
               typename pointer_traits<pointer>::element_type>
              && is_convertible_v
              <typename unique_array<T2, D2>::size_type, size_type>
              && is_assignable_v<D &, D2 &&>) {
    reset(u.release());
    *base() = forward<D2>(*u.base());
    return *this;
  }

  explicit unique_array(iter_pair<pointer> x) noexcept
    requires (!is_pointer_v<deleter_type>
              && is_default_constructible_v<deleter_type>)
    : p(x) {}
  unique_array(iter_pair<pointer> x, const D &d) noexcept
    : base_t(in_place, d), p(x) {}
  unique_array(iter_pair<pointer> x, remove_reference_t<D> &&d) noexcept
    : base_t(in_place, move(d)), p(x) {}

  pointer begin() const noexcept {
    return p.first;
  }
  pointer end() const noexcept {
    return p.second;
  }
  bool empty() const noexcept {
    return p.first == p.second;
  }
  size_type size() const noexcept {
    return to_signed(p.second - p.first);
  }

  deleter_type &get_deleter() noexcept {
    return *base();
  }
  const deleter_type &get_deleter() const noexcept {
    return *base();
  }

  iter_pair<pointer> release() noexcept {
    return exchange(p, iter_pair<pointer>{});
  }
  void reset(iter_pair<pointer> x = iter_pair<pointer>{}) noexcept {
    if (auto old = exchange(p, x); old != iter_pair<pointer>{})
      (*base())(old);
  }
  void clear() noexcept {
    (*base())(p);
    p = iter_pair<pointer>{};
  }
};
template <class T1, class D1, class T2, class D2>
bool operator ==(const unique_array<T1, D1> &x,
                 const unique_array<T2, D2> &y) noexcept
  requires (is_same_v
            <pointer_element_t<typename unique_array<T1, D1>::pointer>,
             pointer_element_t<typename unique_array<T2, D2>::pointer>>
            && equality_comparable
            <pointer_element_t<typename unique_array<T1, D1>::pointer>>) {
  if (x.size() != y.size())
    return false;
  const auto x_ed = end(x);
  auto i = begin(x);
  auto j = begin(y);
  while (i != x_ed) {
    if (*i != *j)
      return false;
    ++i;
    ++j;
  }
  return true;
}
template <class T1, class D1, class T2, class D2>
synth_3way_result<pointer_element_t<typename unique_array<T1, D1>::pointer>>
operator <=>(const unique_array<T1, D1> &x,
             const unique_array<T2, D2> &y)
  requires (is_same_v
            <pointer_element_t<typename unique_array<T1, D1>::pointer>,
             pointer_element_t<typename unique_array<T2, D2>::pointer>>) {
  const auto x_ed = x.end();
  const auto y_ed = y.end();
  auto x_i = x.begin();
  auto y_i = y.begin();
  for (;;) {
    if (x_i == x_ed) {
      if (y_i == y_ed)
        return strong_eq;
      else
        return strong_lt;
    }
    else {
      if (y_i == y_ed)
        return strong_gt;
      else {
        if (auto o = synth_3way(*x_i, *y_i); o != 0)
          return o;
      }
    }
    ++x_i;
    ++y_i;
  }
}

template <class T>
struct fo_make_array {
  unique_array<T> operator ()(size_t n) const {
    return default_alloc_wrapper<T>{}.make_array(n);
  }
  unique_array<T> operator ()(size_t n, const T &x) const {
    return default_alloc_wrapper<T>{}.make_array(n, x);
  }
  template <class IR>
  unique_array<T> operator ()(IR &&r) const requires is_rng<IR> {
    return default_alloc_wrapper<T>{}.make_array(r);
  }
};
template <class T>
inline constexpr fo_make_array<T> make_array{};

}

// copyable_ptr
// copyable_array
namespace re {

template <class T>
class copyable_ptr {
  template <class>
  friend class copyable_ptr;

  unique_ptr<T> v;

public:
  using value_type = T;

  copyable_ptr() = default;
  ~copyable_ptr() = default;
  copyable_ptr(const copyable_ptr &x) {
    if (x.v != nullptr)
      v = make_unique<T>(*x.v);
  }
  copyable_ptr &operator =(const copyable_ptr &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  copyable_ptr(copyable_ptr &&) = default;
  copyable_ptr &operator =(copyable_ptr &&) = default;
  friend void swap(copyable_ptr &x, copyable_ptr &y) noexcept {
    adl_swap(x.v, y.v);
  }

  copyable_ptr(nullptr_t) noexcept {}
  copyable_ptr &operator =(nullptr_t) noexcept {
    v = nullptr;
    return *this;
  }
  bool operator ==(nullptr_t) const noexcept {
    return v == nullptr;
  }
  explicit operator bool() const noexcept {
    return v != nullptr;
  }
  bool empty() const noexcept {
    return v == nullptr;
  }

  explicit copyable_ptr(T *p) noexcept : v(p) {}
  T *release() noexcept {
    return v.release();
  }
  void reset(T *pp = nullptr) noexcept {
    v.reset(pp);
  }
  void clear() noexcept {
    v.clear();
  }

  template <class...S>
  explicit copyable_ptr(in_place_t, S &&...s)
    requires is_constructible_v<T, S &&...>
    : v(make_unique<T>(forward<S>(s)...)) {}
  template <class...S>
  void emplace(S &&...s) requires is_constructible_v<T, S &&...> {
    v = make_unique<T>(forward<S>(s)...);
  }

  T *operator ->() const noexcept {
    return v.operator ->();
  }
  T &operator *() const noexcept {
    return *v;
  }
  T *get() const noexcept {
    return v.get();
  }
};

template <class T>
class copyable_array {
  template <class>
  friend class copyable_array;

  unique_array<T> v;

public:
  using value_type = T;
  using size_type = size_t;

  copyable_array() = default;
  ~copyable_array() = default;
  copyable_array(const copyable_array &x) : v(make_array<T>(x.v)) {}
  copyable_array &operator =(const copyable_array &x) {
    copy_and_swap(x, *this);
    return *this;
  }
  copyable_array(copyable_array &&) = default;
  copyable_array &operator =(copyable_array &&) = default;
  friend void swap(copyable_array &x, copyable_array &y) noexcept {
    adl_swap(x.v, y.v);
  }

  explicit copyable_array(iter_pair<T *> x) noexcept : v(x) {}
  iter_pair<T *> release() noexcept {
    return v.release();
  }
  void reset(iter_pair<T *> x = {}) noexcept {
    v.reset(x);
  }
  void clear() noexcept {
    v.clear();
  }

  explicit copyable_array(size_t n) : v(make_array<T>(n)) {}
  copyable_array(size_t n, const T &x) : v(make_array<T>(n, x)) {}
  template <class IR>
  copyable_array(from_range_t, IR &&r)
    requires (is_rng<IR> && is_constructible_v<T, rng_ref<IR>>)
    : v(make_array<T>(r)) {}

  template <class T1>
  friend bool operator ==(const copyable_array<T1> &,
                          const copyable_array<T1> &)
    requires can_apply_equal_to<const T1 &, const T1 &>;
  template <class T1>
  friend synth_3way_result<T1> operator <=>(const copyable_array<T1> &,
                                            const copyable_array<T1> &);

  T *begin() const noexcept {
    return v.begin();
  }
  T *end() const noexcept {
    return v.end();
  }
  bool empty() const noexcept {
    return v.empty();
  }
  size_t size() const noexcept {
    return v.size();
  }
};
template <class T1>
bool operator ==(const copyable_array<T1> &x,
                 const copyable_array<T1> &y)
  requires can_apply_equal_to<const T1 &, const T1 &> {
  return x.v == y.v;
}
template <class T1>
synth_3way_result<T1> operator <=>(const copyable_array<T1> &x,
                                   const copyable_array<T1> &y) {
  return x.v <=> y.v;
}

}

// buffer - minimized dynamic array with fixed storage
namespace re {

template <class T, class AL = default_allocator<T>>
class buffer : AL {
  static_assert(is_same_v<alloc_vt<AL>, T>);

public:
  using pointer = alloc_ptr<AL>;
  using const_pointer = alloc_cptr<AL>;

private:
  // range [bufp, bufpp) is the allocated buffer
  // every constructed object is in range [p, pp)
  pointer bufp;
  pointer bufpp;
  pointer p;
  pointer pp;

  AL &alloc_ref() noexcept {
    return *this;
  }
  const AL &alloc_ref() const noexcept {
    return *this;
  }

  void new_data() noexcept {
    bufp = bufpp = p = pp = nullptr;
  }
  void delete_data() noexcept {
    if (bufp != bufpp) {
      for (auto &x : iters(p, pp))
        allocator_traits<AL>::destroy(alloc_ref(), x);
      allocator_traits<AL>::deallocate(alloc_ref(), bufp, bufpp - bufp);
    }
  }

public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using difference_type = alloc_dft<AL>;
  using size_type = alloc_szt<AL>;

  buffer() noexcept {
    new_data();
  }
  ~buffer() {
    delete_data();
  }
  buffer(const buffer &) = delete;
  buffer &operator =(const buffer &) = delete;
  buffer(buffer &&x) noexcept : AL(move(x.alloc_ref())) {
    bufp = x.bufp;
    bufpp = x.bufpp;
    p = x.p;
    pp = x.pp;
    x.new_data();
  }
  buffer &operator =(buffer &&) = delete;

  explicit buffer(const AL &al) noexcept : AL(al) {
    new_data();
  }

  template <class L>
  explicit buffer(L l) requires integral<L> : AL() {
    size_type len = integral_cast<size_type>(l);
    if (len != 0) {
      if (len > max_size())
        throw_or_abort<length_error>("re::buffer(n): too big size\n");
      bufp = allocator_traits<AL>::allocate(alloc_ref(), len);
      bufpp = bufp + to_signed(len);
      p = pp = bufp;
    }
    else
      new_data();
  }

  using allocator_type = AL;
  allocator_type get_allocator() const noexcept {
    return *this;
  }

  pointer begin() noexcept {
    return p;
  }
  pointer end() noexcept {
    return pp;
  }
  const_pointer begin() const noexcept {
    return p;
  }
  const_pointer end() const noexcept {
    return pp;
  }

  size_type max_size() const noexcept {
    return min_value
      (allocator_traits<AL>::max_size(alloc_ref()),
       to_unsigned(numeric_limits<ptrdiff_t>::max()) / sizeof(value_type));
  }
  size_type size() const noexcept {
    return pp - p;
  }
  bool empty() const noexcept {
    return p == pp;
  }
  size_type capacity() const noexcept {
    return bufpp - bufp;
  }

  reference front() noexcept {
    return *begin();
  }
  const_reference front() const noexcept {
    return *begin();
  }
  reference back() noexcept {
    return *prev(end());
  }
  const_reference back() const noexcept {
    return *prev(end());
  }
  void push_front(const value_type &x) {
    allocator_traits<AL>::construct(alloc_ref(), p - 1, x);
    --p;
  }
  void push_front(value_type &&x) {
    allocator_traits<AL>::construct(alloc_ref(), p - 1, move(x));
    --p;
  }
  void push_back(const value_type &x) {
    allocator_traits<AL>::construct(alloc_ref(), pp, x);
    ++pp;
  }
  void push_back(value_type &&x) {
    allocator_traits<AL>::construct(alloc_ref(), pp, move(x));
    ++pp;
  }
  void pop_front() noexcept {
    allocator_traits<AL>::destroy(alloc_ref(), p);
    ++p;
  }
  void pop_back() noexcept {
    allocator_traits<AL>::destroy(alloc_ref(), --pp);
  }
  size_type front_raw_space() const noexcept {
    return p - bufp;
  }
  size_type back_raw_space() const noexcept {
    return bufpp - pp;
  }

  void reallocate(size_type n) {
    if (n == 0) {
      reset();
      return;
    }
    if (n < size())
      n = size();

    allocator_wrapper<AL> alw(alloc_ref());
    const pointer new_bufp = alw.allocate(n);
    const pointer new_bufpp = new_bufp + n;

#ifndef RE_NOEXCEPT
    try {
#endif
      pp = alw.uninitialized_fully_move(*this, new_bufp);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      alw.deallocate(new_bufp, n);
      throw;
    }
#endif

    if (p != nullptr)
      alw.deallocate(bufp, bufpp - bufp);
    bufp = new_bufp;
    p = bufp;
    bufpp = new_bufpp;
  }
  template <class INT>
  void increase_capacity(INT nn) requires integral<INT> {
    if (const size_type n = integral_cast<size_type>(nn);
        n != 0) {
      if (n > max_size() - to_unsigned(bufpp - bufp))
        throw_or_abort<length_error>
          ("re::buffer::increase_capacity(n): too big size\n");
      const size_type new_n = to_unsigned(to_signed(n) + (bufpp - bufp));
      reallocate(new_n);
    }
  }

  template <class L = size_type>
  void reset(L l = {}) requires integral<L> {
    const size_type len = integral_cast<size_type>(l);
    delete_data();
    new_data();
    if (len != 0) {
      if (len > max_size())
        throw_or_abort<length_error>("re::buffer::reset(n): too big size\n");
      bufp = allocator_traits<AL>::allocate(alloc_ref(), len);
      bufpp = bufp + to_signed(len);
      p = pp = bufp;
    }
  }
  template <class L>
  void reserve(L l) requires integral<L> {
    const size_type len = integral_cast<size_type>(l);
    if (len > capacity())
      reallocate(len);
  }
  void reserve_more(size_type n) {
    const auto z = size();
    const auto rest = capacity() - z;
    if (n > rest)
      increase_capacity(n - rest);
  }
  void clear() noexcept {
    for (auto &x : iters(p, pp))
      allocator_traits<AL>::destroy(alloc_ref(), to_address(x));
    p = pp = bufp;
  }

  void release() noexcept {
    new_data();
  }
};

}

// scoped_allocator_adaptor
namespace re {

template <class, class...>
class scoped_allocator_adaptor;
namespace inner {

template <class A>
concept scoped_alloc_with_mem_fn_outer_allocator
  = requires(A &a) {a.outer_allocator();};

namespace fns {

template <class A>
auto &scoped_alloc_outermost(A &a) noexcept {
  if constexpr (scoped_alloc_with_mem_fn_outer_allocator<A>) {
    return inner::fns::scoped_alloc_outermost(a.outer_allocator());
  }
  else {
    return a;
  }
}

}

template <class A>
using scoped_alloc_outermost_traits
  = allocator_traits<remove_reference_t
                     <decltype(inner::fns::scoped_alloc_outermost
                               (declval<A &>()))>>;

template <class...INNER_ALLOCS>
struct scoped_alloc_inner {
  using type = scoped_allocator_adaptor<INNER_ALLOCS...>;
};
template <>
struct scoped_alloc_inner<> {
  using type = empty_type;
};
template <class...INNER_ALLOCS>
using scoped_alloc_inner_t = typename scoped_alloc_inner<INNER_ALLOCS...>::type;

}

template <class OUTER_ALLOC, class...INNER_ALLOCS>
class scoped_allocator_adaptor : public OUTER_ALLOC {
  inner::scoped_alloc_inner_t<INNER_ALLOCS...> inner;

  template <class, class...>
  friend class scoped_allocator_adaptor;

  using this_t = scoped_allocator_adaptor;
  using outer_traits = allocator_traits<OUTER_ALLOC>;

public:
  using outer_allocator_type = OUTER_ALLOC;
  using inner_allocator_type = conditional_t
    <sizeof...(INNER_ALLOCS) == 0,
     this_t, inner::scoped_alloc_inner_t<INNER_ALLOCS...>>;
  using value_type = typename outer_traits::value_type;
  using size_type = typename outer_traits::size_type;
  using difference_type = typename outer_traits::difference_type;
  using pointer = typename outer_traits::pointer;
  using const_pointer = typename outer_traits::const_pointer;
  using void_pointer = typename outer_traits::void_pointer;
  using const_void_pointer = typename outer_traits::const_void_pointer;
  using propagate_on_container_copy_assignment
    = bool_constant<alloc_copy_prpg<OUTER_ALLOC>
                    && (alloc_copy_prpg<INNER_ALLOCS> && ...)>;
  using propagate_on_container_move_assignment
    = bool_constant<alloc_move_prpg<OUTER_ALLOC>
                    && (alloc_move_prpg<INNER_ALLOCS> && ...)>;
  using propagate_on_container_swap
    = bool_constant<alloc_swap_prpg<OUTER_ALLOC>
                    && (alloc_swap_prpg<INNER_ALLOCS> && ...)>;
  using is_always_equal
    = bool_constant<alloc_always_equal<OUTER_ALLOC>
                    && (alloc_always_equal<INNER_ALLOCS> && ...)>;
  template <class T>
  struct rebind {
    using other = scoped_allocator_adaptor
      <typename outer_traits::template rebind_alloc<T>, INNER_ALLOCS...>;
  };

  scoped_allocator_adaptor() = default;
  ~scoped_allocator_adaptor() = default;
  scoped_allocator_adaptor(const scoped_allocator_adaptor &other) = default;
  scoped_allocator_adaptor &operator =(const scoped_allocator_adaptor &)
    = default;
  scoped_allocator_adaptor(scoped_allocator_adaptor &&other) = default;
  scoped_allocator_adaptor &operator =(scoped_allocator_adaptor &&) = default;
  friend void swap(scoped_allocator_adaptor &x,
                   scoped_allocator_adaptor &y) noexcept {
    adl_swap(static_cast<OUTER_ALLOC &>(x), static_cast<OUTER_ALLOC &>(y));
    adl_swap(x.inner, y.inner);
  }

  template <class OUTER_A2>
  scoped_allocator_adaptor(OUTER_A2 &&a, const INNER_ALLOCS &...s) noexcept
    requires is_constructible_v<OUTER_ALLOC, OUTER_A2 &&>
    : OUTER_ALLOC(forward<OUTER_A2>(a)), inner(s...) {}

  template <class OUTER_A2>
  scoped_allocator_adaptor(const scoped_allocator_adaptor
                           <OUTER_A2, INNER_ALLOCS...> &other) noexcept
    requires is_constructible_v<OUTER_ALLOC, const OUTER_A2 &>
    : OUTER_ALLOC(static_cast<const OUTER_A2 &>(other))
    , inner(other.inner) {}
  template <class OUTER_A2>
  scoped_allocator_adaptor(scoped_allocator_adaptor
                           <OUTER_A2, INNER_ALLOCS...> &&other) noexcept
    requires is_constructible_v<OUTER_ALLOC, OUTER_A2>
    : OUTER_ALLOC(static_cast<OUTER_A2 &&>(other))
    , inner(move(other.inner)) {}

  outer_allocator_type &outer_allocator() noexcept {
    return *this;
  }
  const outer_allocator_type &outer_allocator() const noexcept {
    return *this;
  }

  inner_allocator_type &inner_allocator() noexcept {
    if constexpr (sizeof...(INNER_ALLOCS) != 0) {
      return inner;
    }
    else {
      return *this;
    }
  }
  const inner_allocator_type &inner_allocator() const noexcept {
    if constexpr (sizeof...(INNER_ALLOCS) != 0) {
      return inner;
    }
    else {
      return *this;
    }
  }

  pointer allocate(size_type n) {
    return outer_traits::allocate(outer_allocator(), n);
  }
  pointer allocate(size_type n, const_void_pointer hint) {
    return outer_traits::allocate(outer_allocator(), n, hint);
  }
  void deallocate(pointer p, size_type n) {
    outer_traits::deallocate(outer_allocator(), p, n);
  }

  size_type max_size() const {
    return outer_traits::max_size(outer_allocator());
  }

  template <class T, class...S>
  void construct(T *p, S &&...s) {
    apply([=, this]<class...SS>(SS &&...ss) {
        inner::scoped_alloc_outermost_traits<this_t>
          ::construct(inner::fns::scoped_alloc_outermost(*this),
                      p, forward<SS>(ss)...);
      },
      uses_allocator_construction_args<T>(inner_allocator(), forward<S>(s)...));
  }
  template <class T>
  void destroy(T *p) {
    inner::scoped_alloc_outermost_traits<this_t>
      ::destroy(inner::fns::scoped_alloc_outermost(*this), p);
  }

private:
  struct select_on_copy_tag {};
  template <class X>
  scoped_allocator_adaptor(select_on_copy_tag, const X &x,
                           inner_allocator_type &&y)
    requires (sizeof...(INNER_ALLOCS) != 0)
    : OUTER_ALLOC(x), inner(move(y)) {}
  template <class X>
  scoped_allocator_adaptor(select_on_copy_tag, const X &x)
    requires (sizeof...(INNER_ALLOCS) == 0)
    : OUTER_ALLOC(x) {}
public:
  this_t select_on_container_copy_construction() const {
    if constexpr (sizeof...(INNER_ALLOCS) != 0) {
      return this_t
        (select_on_copy_tag{},
         outer_traits::select_on_container_copy_construction(outer_allocator()),
         inner.select_on_container_copy_construction());
    }
    else {
      return this_t
        (select_on_copy_tag{},
         outer_traits::select_on_container_copy_construction
         (outer_allocator()));
    }
  }

private:
  template <class AA, class BB, class...SS>
  friend bool operator ==(const scoped_allocator_adaptor<AA, SS...> &,
                          const scoped_allocator_adaptor<BB, SS...> &) noexcept;
  template <class...S>
  bool eq(const scoped_allocator_adaptor<S...> &x) const {
    if constexpr (sizeof...(INNER_ALLOCS) != 0) {
      return outer_allocator() == x.outer_allocator()
        && inner_allocator().eq(x.inner_allocator());
    }
    else {
      return outer_allocator() == x.outer_allocator();
    }
  }
};
template <class AA, class BB, class...SS>
bool operator ==(const scoped_allocator_adaptor<AA, SS...> &x,
                 const scoped_allocator_adaptor<BB, SS...> &y) noexcept {
  return x.eq(y);
}

}

// allocator_aware_container_ownership
namespace re {

template <class T>
class allocator_aware_container_ownership {
  //  requires:
  //    1) this type is a friend of T
  //    2) the following member functions of T are defined:
  //      typename T::allocator_type &alloc_ref() noexcept
  //      const typename T::allocator_type &alloc_ref() const noexcept
  //      void new_data()
  //      void delete_data() noexcept
  //      void new_data(const T &)
  //      void assign_data(const T &)         (optional)
  //      void new_data(T &&) noexcept
  //      void new_data_individually(T &&)
  //      void assign_data_individually(T &&) (optional)
  //      void swap_data(T &)
  //      void swap_data_individually(T &)    (optional)
  //      T(const T::allocator_type &)
  //      [Note1]: self-assignment guard is not needed (because it is done here)
  //               but swap_data(T &) must be defended against self-swap
  //      [Note2]: *this can be invalid but has no leak after new_xxx() throwing

  using this_t = allocator_aware_container_ownership;
  using alloc_t = typename T::allocator_type;

  template <class TT>
  struct has_mem_fn_assign_data {
    static inner::disable f(...);
    template <class TTT>
    static decltype(declval<TTT &>().assign_data(declval<const TTT &>()))
    f(type_tag<TTT> x);
    static constexpr bool value
      = !is_same_v<decltype(f(type_tag<TT>{})), inner::disable>;
  };
  template <class TT>
  struct has_mem_fn_assign_data_individually {
    static inner::disable f(...);
    template <class TTT>
    static decltype(declval<TTT &>().assign_data_individually(declval<TTT>()))
    f(type_tag<TTT> x);
    static constexpr bool value
      = !is_same_v<decltype(f(type_tag<TT>{})), inner::disable>;
  };
  template <class TT>
  struct has_mem_fn_swap_data_individually {
    static inner::disable f(...);
    template <class TTT>
    static decltype(declval<TTT &>().swap_data_individually(declval<TTT &>()))
    f(type_tag<TTT> x);
    static constexpr bool value
      = !is_same_v<decltype(f(type_tag<TT>{})), inner::disable>;
  };

private:
  template <class TT>
  static auto &&move(TT &&x) noexcept {
    return re::move(x);
  }

public:
  static void move_construct(T &target, T &from) {
    if constexpr (alloc_always_equal<alloc_t>) {
      target.new_data(move(from));
    }
    else {
      target.alloc_ref() == from.alloc_ref()
        ? target.new_data(move(from))
        : target.new_data_individually(move(from));
    }
  }

private:
  static void new_data(const T &src, T &o) {
#ifndef RE_NOEXCEPT
    try {
#endif
      o.new_data(src);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      o.new_data();
      throw;
    }
#endif
  }
  static void new_data_individually(T &&src, T &o) {
#ifndef RE_NOEXCEPT
    try {
#endif
      o.new_data_individually(move(src));
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      o.new_data();
      throw;
    }
#endif
  }

  static void propagate_then_assign(const T &src, T &o) {
    o.alloc_ref() = src.alloc_ref();
    o.assign_data(src);
  }
  static void propagate_then_clone(const T &src, T &o) {
    o.delete_data();
    o.alloc_ref() = src.alloc_ref();
    new_data(src, o);
  }
  static void clean_then_clone(const T &src, T &o) {
    o.delete_data();
    new_data(src, o);
  }

  static void copy_impl(const T &src, T &o)
    requires (alloc_copy_prpg<alloc_t>
              && has_mem_fn_assign_data<T>::value
              && alloc_always_equal<alloc_t>) {
    propagate_then_assign(src, o);
  }
  static void copy_impl(const T &src, T &o)
    requires (alloc_copy_prpg<alloc_t>
              && has_mem_fn_assign_data<T>::value
              && !alloc_always_equal<alloc_t>) {
    o.alloc_ref() == src.alloc_ref()
      ? propagate_then_assign(src, o)
      : propagate_then_clone(src, o);
  }
  static void copy_impl(const T &src, T &o)
    requires (alloc_copy_prpg<alloc_t>
              && !has_mem_fn_assign_data<T>::value) {
    propagate_then_clone(src, o);
  }
  static void copy_impl(const T &src, T &o)
    requires (!alloc_copy_prpg<alloc_t>
              && has_mem_fn_assign_data<T>::value) {
    o.assign_data(src);
  }
  static void copy_impl(const T &src, T &o)
    requires (!alloc_copy_prpg<alloc_t>
              && !has_mem_fn_assign_data<T>::value) {
    clean_then_clone(src, o);
  }
public:
  static void copy_assign(T &target, const T &from) {
    if (addressof(target) != addressof(from))
      copy_impl(from, target);
  }

private:
  static void move_impl(T &src, T &o) requires alloc_move_prpg<alloc_t> {
    o.delete_data();
    o.alloc_ref() = move(src.alloc_ref());
    o.new_data(move(src));
  }
  static void move_impl(T &src, T &o)
    requires (!alloc_move_prpg<alloc_t> && alloc_always_equal<alloc_t>) {
    o.delete_data();
    o.new_data(move(src));
  }
  static void move_impl(T &src, T &o)
    requires (!alloc_move_prpg<alloc_t> && !alloc_always_equal<alloc_t>
              && has_mem_fn_assign_data_individually<T>::value) {
    if (src.alloc_ref() == o.alloc_ref()) {
      o.delete_data();
      o.new_data(move(src));
    }
    else
      o.assign_data_individually(move(src));
  }
  static void move_impl(T &src, T &o)
    requires (!alloc_move_prpg<alloc_t> && !alloc_always_equal<alloc_t>
              && !has_mem_fn_assign_data_individually<T>::value) {
    o.delete_data();
    src.alloc_ref() == o.alloc_ref()
      ? o.new_data(move(src))
      : new_data_individually(move(src), o);
  }
public:
  static void move_assign(T &target, T &from) {
    if (addressof(target) != addressof(from))
      move_impl(from, target);
  }

private:
  static void swap_impl(T &x, T &y) requires (alloc_swap_prpg<alloc_t>) {
    x.swap_data(y);
    adl_swap(x.alloc_ref(), y.alloc_ref());
  }
  static void swap_impl(T &x, T &y)
    requires (!alloc_swap_prpg<alloc_t> && alloc_always_equal<alloc_t>) {
    x.swap_data(y);
  }
  static void swap_impl(T &x, T &y)
    requires (!alloc_swap_prpg<alloc_t> && !alloc_always_equal<alloc_t>
              && has_mem_fn_swap_data_individually<T>::value) {
    x.alloc_ref() == y.alloc_ref()
      ? x.swap_data(y)
      : x.swap_data_individually(y);
  }
  static void swap_impl(T &x, T &y)
    requires (!alloc_swap_prpg<alloc_t> && !alloc_always_equal<alloc_t>
              && !has_mem_fn_swap_data_individually<T>::value
              && has_mem_fn_assign_data_individually<T>::value) {
    if (x.alloc_ref() == y.alloc_ref())
      x.swap_data(y);
    else {
      T tmp(x.alloc_ref());
      tmp.assign_data_individually(move(x));
      x.assign_data_individually(move(y));
      y.assign_data_individually(move(tmp));
    }
  }
  static void swap_impl(T &x, T &y)
    requires (!alloc_swap_prpg<alloc_t> && !alloc_always_equal<alloc_t>
              && !has_mem_fn_swap_data_individually<T>::value
              && !has_mem_fn_assign_data_individually<T>::value) {
    if (x.alloc_ref() == y.alloc_ref())
      x.swap_data(y);
    else {
      T tmp(x.alloc_ref());
      tmp.delete_data();
      new_data_individually(move(y), tmp);
      y.delete_data();
      new_data_individually(move(x), y);
      x.delete_data();
      new_data_individually(move(tmp), x);
    }
  }
public:
  static void swap(T &x, T &y) {
    swap_impl(x, y);
  }
};

}

#endif
