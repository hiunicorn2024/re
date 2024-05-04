#ifndef RE_DEFINED_RANGE_H
#define RE_DEFINED_RANGE_H

#include "base.h"

// iterator requirements
namespace re {

template <class>
struct iterator_traits;
namespace inner {

template <class>
struct primary_iter_traits;

template <class I>
concept with_primary_iter_traits
  = is_base_of_v<primary_iter_traits<I>, iterator_traits<I>>;

template <class T>
concept can_reference = is_referenceable_v<T>;
template <class T>
concept dereferenceable = requires(T &t) {
  {*t}->can_reference;
};

}

template <class>
struct incrementable_traits {};
template <class T>
requires is_object_v<T>
struct incrementable_traits<T *> {
  using difference_type = ptrdiff_t;
};
template <class T>
struct incrementable_traits<const T> : incrementable_traits<T> {};
template <class T>
requires requires {typename T::difference_type;}
struct incrementable_traits<T> {
  using difference_type = typename T::difference_type;
};
template <class T>
requires ((!requires {typename T::difference_type;})
          && requires(const T &a, const T &b) {{a - b}->integral;})
struct incrementable_traits<T> {
  using difference_type = make_signed_t<decltype(declval<T>() - declval<T>())>;
};
namespace inner {

template <class I>
struct iter_dif_t_impl {
  using type = typename iterator_traits<I>::difference_type;
};
template <class I>
requires inner::with_primary_iter_traits<I>
struct iter_dif_t_impl<I> {
  using type = typename incrementable_traits<I>::difference_type;
};

}
template <class I>
using iter_difference_t
  = typename inner::iter_dif_t_impl<remove_cvref_t<I>>::type;

namespace inner {

template <class>
struct cond_value_type {};
template <class T>
requires is_object_v<T>
struct cond_value_type<T> {
  using value_type = remove_cv_t<T>;
};

template <class T>
concept has_member_value_type = requires {typename T::value_type;};
template <class T>
concept has_member_element_type = requires {typename T::element_type;};  

}
template <class>
struct indirectly_readable_traits {};
template <class T>
struct indirectly_readable_traits<T *> : inner::cond_value_type<T> {};
template <class T>
requires is_array_v<T>
struct indirectly_readable_traits<T> {
  using value_type = remove_cv_t<remove_extent_t<T>>;
};
template <class T>
struct indirectly_readable_traits<const T>: indirectly_readable_traits<T> {};
template <inner::has_member_value_type T>
struct indirectly_readable_traits<T>
  : inner::cond_value_type<typename T::value_type> {};
template <inner::has_member_element_type T>
struct indirectly_readable_traits<T>
  : inner::cond_value_type<typename T::element_type> {};
template <inner::has_member_value_type T>
requires inner::has_member_element_type<T>
struct indirectly_readable_traits<T> {};
template <inner::has_member_value_type T>
requires inner::has_member_element_type<T>
  && same_as<remove_cv_t<typename T::element_type>,
             remove_cv_t<typename T::value_type>>
struct indirectly_readable_traits<T>
  : inner::cond_value_type<typename T::value_type> {};
namespace inner {

template <class I>
struct iter_value_t_impl {
  using type = typename iterator_traits<I>::value_type;
};
template <class I>
requires inner::with_primary_iter_traits<I>
&& requires {typename indirectly_readable_traits<I>::value_type;}
struct iter_value_t_impl<I> {
  using type = typename indirectly_readable_traits<I>::value_type;
};
template <class I>
requires inner::with_primary_iter_traits<I>
&& (!requires {typename indirectly_readable_traits<I>::value_type;})
struct iter_value_t_impl<I> {};

}
template <class I>
using iter_value_t = typename inner::iter_value_t_impl<remove_cvref_t<I>>::type;

template <inner::dereferenceable T>
using iter_reference_t = decltype(*declval<T &>());

template <class I>
concept cpp17_iterator = requires(I i) {
    {*i}->inner::can_reference;
    {++i}->same_as<I &>;
    {*i++}->inner::can_reference;
  }
  && copyable<I>;
template <class I>
concept cpp17_input_iterator
  = cpp17_iterator<I>
  && equality_comparable<I>
  && requires(I i) {
    typename incrementable_traits<I>::difference_type;
    typename indirectly_readable_traits<I>::value_type;
    typename common_reference_t
    <decltype(*declval<I &>()) &&,
     typename indirectly_readable_traits<I>::value_type &>;
    typename common_reference_t
    <decltype(*i++) &&, typename indirectly_readable_traits<I>::value_type &>;
    requires signed_integral<typename incrementable_traits<I>::difference_type>;
  };
template <class I>
concept cpp17_forward_iterator
  = cpp17_input_iterator<I>
  && constructible_from<I>
  && is_lvalue_reference_v<decltype(*declval<I &>())>
  && same_as<remove_cvref_t<decltype(*declval<I &>())>,
             typename indirectly_readable_traits<I>::value_type>
  && requires(I i) {
    {i++}->convertible_to<const I &>;
    {*i++}->same_as<iter_reference_t<I>>;
  };
template <class I>
concept cpp17_bidirectional_iterator
  = cpp17_forward_iterator<I>
  && requires(I i) {
    {--i}->same_as<I &>;
    {i--}->convertible_to<const I &>;
    {*i--}->same_as<iter_reference_t<I>>;
  };
template<class I>
concept cpp17_random_access_iterator
  = cpp17_bidirectional_iterator<I>
  && totally_ordered<I>
  && requires(I i, typename incrementable_traits<I>::difference_type n) {
    {i += n}->same_as<I &>;
    {i -= n}->same_as<I &>;
    {i + n}->same_as<I>;
    {n + i}->same_as<I>;
    {i - n}->same_as<I>;
    {i - i}->same_as<decltype(n)>;
    {i[n]}->convertible_to<decltype(*declval<I &>())>;
  };

namespace inner {

template <class I>
concept iter_traits_branch_1 = requires {
  typename I::iterator_category;
  typename I::value_type;
  typename I::difference_type;
  typename I::reference;
};
template <class I>
concept not_iter_traits_branch_1 = !iter_traits_branch_1<I>;

template <class I>
concept has_mpointer = requires {typename I::pointer;};
template <class I>
struct get_iter_ptr {
  template <class II>
  static void f();
  template <class II>
  static typename II::pointer f() requires has_mpointer<II>;

  using type = decltype(f<I>());
};
template <class I>
struct get_iter_ptr_2 {
  template <class II>
  static void f();
  template <class II>
  static typename II::pointer f() requires has_mpointer<II>;
  template <class II>
  static decltype(declval<II &>().operator ->()) f()
    requires (!has_mpointer<II>)
    && requires {declval<II &>().operator ->();};

  using type = decltype(f<I>());
};

template <class I>
concept has_mictg = requires {typename I::iterator_category;};
template <class I, bool HAS_MICTG = inner::has_mictg<I>>
struct get_iter_ctg {
  template <class II>
  static input_iterator_tag f();
  template <class II>
  static forward_iterator_tag f()
    requires cpp17_forward_iterator<II>;
  template <class II>
  static bidirectional_iterator_tag f()
    requires cpp17_bidirectional_iterator<II>;
  template <class II>
  static random_access_iterator_tag f()
    requires cpp17_random_access_iterator<II>;

  using type = decltype(f<I>());
};
template <class I>
struct get_iter_ctg<I, true> {
  using type = typename I::iterator_category;
};

template <class I>
concept has_miref = requires {typename I::reference;};
template <class I>
struct get_iter_ref {
  template <class II>
  static iter_reference_t<II> f();
  template <class II>
  static typename II::reference f() requires has_miref<II>;

  using type = decltype(f<I>());
};

template <class I>
concept has_mdft = requires {typename I::difference_type;};
template <class I>
struct get_iter_dft {
  template <class II>
  static void f();
  template <class II>
  static typename II::difference_type f() requires has_mdft<II>;

  using type = decltype(f<I>());
};

template <class I>
struct primary_iter_traits {};
template <class I>
requires iter_traits_branch_1<I>
struct primary_iter_traits<I> {
  using iterator_category = typename I::iterator_category;
  using value_type = typename I::value_type;
  using difference_type = typename I::difference_type;
  using pointer = typename inner::get_iter_ptr<I>::type;
  using reference = typename I::reference;
};
template <class I>
requires not_iter_traits_branch_1<I> && cpp17_input_iterator<I>
struct primary_iter_traits<I> {
  using iterator_category = typename inner::get_iter_ctg<I>::type;
  using value_type = typename indirectly_readable_traits<I>::value_type;
  using difference_type = typename incrementable_traits<I>::difference_type;
  using pointer = typename inner::get_iter_ptr_2<I>::type;
  using reference = typename inner::get_iter_ref<I>::type;
};
template <class I>
requires not_iter_traits_branch_1<I> && cpp17_iterator<I>
struct primary_iter_traits<I> {
  using iterator_category = output_iterator_tag;
  using value_type = void;
  using difference_type = typename inner::get_iter_dft<I>::type;
  using pointer = void;
  using reference = void;
};

}
template <class I>
struct iterator_traits : inner::primary_iter_traits<I> {};
template <class T>
requires is_object_v<T>
struct iterator_traits<T *> {
  using value_type = remove_cv_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = contiguous_iterator_tag;
};

}
namespace re_adl {

void iter_move();
template <class T>
concept can_iter_move = requires(T &&x) {
  iter_move(re::forward<T>(x));
};
template <class T>
constexpr decltype(auto) do_iter_move(T &&x) {
  return iter_move(re::forward<T>(x));
}

}
namespace re {

namespace ranges {

struct fo_iter_move {
  template <class T>
  constexpr decltype(auto) operator ()(T &&x) const noexcept
    requires re_adl::can_iter_move<T> {
    return re_adl::do_iter_move(forward<T>(x));
  }
  template <class T>
  constexpr decltype(auto) operator ()(T &&x) const noexcept {
    if constexpr (is_lvalue_reference_v<decltype(*forward<T>(x))>) {
      return move(*forward<T>(x));
    }
    else {
      return *forward<T>(x);
    }
  }
};
inline constexpr fo_iter_move iter_move{};

}

template <class T>
requires requires(T &x) {{ranges::iter_move(x)}->inner::can_reference;}
using iter_rvalue_reference_t = decltype(ranges::iter_move(declval<T &>()));

}
namespace re_adl {

template <class I1, class I2>
void iter_swap(I1, I2) = delete;
template <class X, class Y>
concept iter_swap_adl_capable
  = ((re::is_class_v<re::remove_reference_t<X>>
     || re::is_enum_v<re::remove_reference_t<X>>)
    || (re::is_class_v<re::remove_reference_t<Y>>
        || re::is_enum_v<re::remove_reference_t<Y>>))
  && requires(X &&x, Y &&y) {iter_swap(static_cast<X &&>(x),
                                       static_cast<Y &&>(y));};

template <class X, class Y>
constexpr void do_iter_swap(X &&x, Y &&y)
  noexcept(noexcept((void)iter_swap(re::declval<X>(), re::declval<Y>()))) {
  (void)iter_swap(static_cast<X &&>(x), static_cast<Y &&>(y));
}

}
namespace re {

namespace inner {

template <class IN>
concept indirectly_readable_impl
  = requires(const IN in) {
    typename iter_value_t<IN>;
    typename iter_reference_t<IN>;
    typename iter_rvalue_reference_t<IN>;
    {*in}->same_as<iter_reference_t<IN>>;
    {ranges::iter_move(in)}->same_as<iter_rvalue_reference_t<IN>>;
  }
  && common_reference_with<iter_reference_t<IN> &&, iter_value_t<IN> &>
  && common_reference_with<iter_reference_t<IN> &&,
                           iter_rvalue_reference_t<IN> &&>
  && common_reference_with<iter_rvalue_reference_t<IN> &&,
                           const iter_value_t<IN> &>;
}
template <class IN>
concept indirectly_readable
  = inner::indirectly_readable_impl<remove_cvref_t<IN>>;

template <indirectly_readable T>
using iter_common_reference_t
  = common_reference_t<iter_reference_t<T>, iter_value_t<T> &>;

template <class OUT, class T>
concept indirectly_writable = requires(OUT &&o, T &&t) {
  *o = forward<T>(t);
  *forward<OUT>(o) = forward<T>(t);
  const_cast<const iter_reference_t<OUT> &&>(*o) = forward<T>(t);
  const_cast<const iter_reference_t<OUT> &&>(*forward<OUT>(o)) = forward<T>(t);
};

template <class IN, class OUT>
concept indirectly_movable = indirectly_readable<IN>
  && indirectly_writable<OUT, iter_rvalue_reference_t<IN>>;

template <class IN, class OUT>
concept indirectly_movable_storable = indirectly_movable<IN, OUT>
  && indirectly_writable<OUT, iter_value_t<IN>>
  && movable<iter_value_t<IN>>
  && constructible_from<iter_value_t<IN>, iter_rvalue_reference_t<IN>>
  && assignable_from<iter_value_t<IN> &, iter_rvalue_reference_t<IN>>;

namespace inner::fns {

template<class X, class Y>
constexpr iter_value_t<X> iter_exchange_move(X &&x, Y &&y)
  noexcept(noexcept(iter_value_t<X>(ranges::iter_move(x))) &&
           noexcept(*x = ranges::iter_move(y))) {
  iter_value_t<X> old_value(ranges::iter_move(x));
  *x = ranges::iter_move(y);
  return old_value;
}

}
namespace ranges {

struct fo_iter_swap {
  template <class X, class Y>
  constexpr void operator ()(X &&x, Y &&y) const
    noexcept(noexcept(re_adl::do_iter_swap(declval<X>(), declval<Y>())))
    requires re_adl::iter_swap_adl_capable<X, Y> {
    re_adl::do_iter_swap(forward<X>(x), forward<Y>(y));
  }
  template <class X, class Y>
  constexpr auto operator ()(X &&x, Y &&y) const
    noexcept(noexcept(ranges::swap(*declval<X>(), *declval<Y>())))
    ->decltype(ranges::swap(*declval<X>(), *declval<Y>()))
    requires (!re_adl::iter_swap_adl_capable<X, Y>)
    && indirectly_readable<remove_reference_t<X>>
    && indirectly_readable<remove_reference_t<Y>>
    && swappable_with<iter_reference_t<remove_reference_t<X>>,
                      iter_reference_t<remove_reference_t<Y>>> {
    return ranges::swap(*forward<X>(x), *forward<Y>(y));
  }
  template <class X, class Y>
  constexpr enable_if_t<indirectly_movable_storable
                        <remove_reference_t<X>, remove_reference_t<Y>>
                        && indirectly_movable_storable
                        <remove_reference_t<Y>, remove_reference_t<X>>>
  operator ()(X &&x, Y &&y) const
    noexcept(noexcept((void)(*declval<X>()
                             = inner::fns::iter_exchange_move
                             (declval<Y>(), declval<X>())))) {
    return (void)(*forward<X>(x)
                  = inner::fns::iter_exchange_move
                  (forward<Y>(y), forward<X>(x)));
  }
};
inline constexpr fo_iter_swap iter_swap{};

}

template <class I>
concept weakly_incrementable = movable<I> && requires(I i) {
    typename iter_difference_t<I>;
    requires signed_integral<iter_difference_t<I>>;
    {++i}->same_as<I &>;
    i++;
  };

template <class I>
concept incrementable = regular<I> && weakly_incrementable<I>
  && requires(I i) {{i++}->same_as<I>;};

template <class I>
concept input_or_output_iterator = requires(I i) {{*i}->inner::can_reference;}
  && weakly_incrementable<I>;

template <class S, class I>
concept sentinel_for = semiregular<S> && input_or_output_iterator<I>
  && inner::weakly_equality_comparable_with<S, I>;

template <class S, class I>
inline constexpr bool disable_sized_sentinel_for = false;
template <class S, class I>
concept sized_sentinel_for = sentinel_for<S, I>
  && !disable_sized_sentinel_for<remove_cv_t<S>, remove_cv_t<I>>
  && requires(const I &i, const S &s) {
    {s - i}->same_as<iter_difference_t<I>>;
    {i - s}->same_as<iter_difference_t<I>>;
  };

// inner::iter_concept<I>
namespace inner {

template <class I>
struct iter_concept_impl {
  static consteval bool is_primary() {
    return is_base_of_v<inner::primary_iter_traits<I>, iterator_traits<I>>;
  }

  template <class T>
  static typename T::iterator_concept f()
    requires requires {typename T::iterator_concept;};
  template <class T>
  static typename T::iterator_category f()
    requires (!(requires {typename T::iterator_concept;}))
    && (requires {typename T::iterator_category;});
  template <class T>
  static random_access_iterator_tag f()
    requires (!(requires {typename T::iterator_concept;}))
    && (!(requires {typename T::iterator_category;}))
    && (is_primary());
  template <class T>
  static void f();

  using traits_t = conditional_t<is_primary(), I, iterator_traits<I>>;
  using type = decltype(f<traits_t>());
};
template <class T>
using iter_concept = typename inner::iter_concept_impl<T>::type;

}

template <class I>
concept input_iterator = input_or_output_iterator<I>
  && indirectly_readable<I>
  && (!is_void_v<inner::iter_concept<I>>)
  && derived_from<inner::iter_concept<I>, input_iterator_tag>;

template <class I, class T>
concept output_iterator = input_or_output_iterator<I>
  && indirectly_writable<I, T>
  && requires(I i, T &&t) {*i++ = forward<T>(t);};

template <class I>
concept forward_iterator = input_iterator<I>
  && derived_from<inner::iter_concept<I>, forward_iterator_tag>
  && incrementable<I> && sentinel_for<I, I>;

template <class I>
concept bidirectional_iterator = forward_iterator<I>
  && derived_from<inner::iter_concept<I>, bidirectional_iterator_tag>
  && requires(I i) {{--i}->same_as<I &>; {i--}->same_as<I>;};

template <class I>
concept random_access_iterator = bidirectional_iterator<I>
  && derived_from<inner::iter_concept<I>, random_access_iterator_tag>
  && totally_ordered<I> && sized_sentinel_for<I, I>
  && requires(I i, const I j, const iter_difference_t<I> n) {
    {i += n}->same_as<I &>;
    {j + n}->same_as<I>;
    {n + j}->same_as<I>;
    {i -= n}->same_as<I&>;
    {j - n}->same_as<I>;
    {j[n]}->same_as<iter_reference_t<I>>;
  };

template <class I>
concept contiguous_iterator = random_access_iterator<I>
  && derived_from<inner::iter_concept<I>, contiguous_iterator_tag>
  && is_lvalue_reference_v<iter_reference_t<I>>
  && same_as<iter_value_t<I>, remove_cvref_t<iter_reference_t<I>>>
  && requires(const I &i) {
    {to_address(i)}->same_as<add_pointer_t<iter_reference_t<I>>>;
  };

template <class F, class I>
concept indirectly_unary_invocable = indirectly_readable<I>
  && copy_constructible<F>
  && invocable<F &, iter_value_t<I> &>
  && invocable<F &, iter_reference_t<I>>
  && invocable<F &, iter_common_reference_t<I>>
  && common_reference_with<invoke_result_t<F &, iter_value_t<I> &>,
                           invoke_result_t<F &, iter_reference_t<I>>>;

template <class F, class I>
concept indirectly_regular_unary_invocable = indirectly_readable<I>
  && copy_constructible<F>
  && regular_invocable<F &, iter_value_t<I> &>
  && regular_invocable<F &, iter_reference_t<I>>
  && regular_invocable<F &, iter_common_reference_t<I>>
  && common_reference_with<invoke_result_t<F &, iter_value_t<I> &>,
                           invoke_result_t<F &, iter_reference_t<I>>>;

template <class F, class I>
concept indirect_unary_predicate = indirectly_readable<I>
  && copy_constructible<F>
  && predicate<F &, iter_value_t<I> &>
  && predicate<F &, iter_reference_t<I>>
  && predicate<F &, iter_common_reference_t<I>>;

template <class F, class I1, class I2>
concept indirect_binary_predicate
  = indirectly_readable<I1> && indirectly_readable<I2>
  && copy_constructible<F>
  && predicate<F &, iter_value_t<I1> &, iter_value_t<I2> &>
  && predicate<F &, iter_value_t<I1> &, iter_reference_t<I2>>
  && predicate<F &, iter_reference_t<I1>, iter_value_t<I2> &>
  && predicate<F &, iter_reference_t<I1>, iter_reference_t<I2>>
  && predicate<F &, iter_common_reference_t<I1>, iter_common_reference_t<I2>>;

template <class F, class I1, class I2 = I1>
concept indirect_equivalence_relation
  = indirectly_readable<I1> && indirectly_readable<I2>
  && copy_constructible<F>
  && equivalence_relation<F &, iter_value_t<I1> &, iter_value_t<I2> &>
  && equivalence_relation<F &, iter_value_t<I1> &, iter_reference_t<I2>>
  && equivalence_relation<F &, iter_reference_t<I1>, iter_value_t<I2> &>
  && equivalence_relation<F &, iter_reference_t<I1>, iter_reference_t<I2>>
  && equivalence_relation<F &, iter_common_reference_t<I1>,
                          iter_common_reference_t<I2>>;

template <class F, class I1, class I2 = I1>
concept indirect_strict_weak_order
  = indirectly_readable<I1> && indirectly_readable<I2> && copy_constructible<F>
  && strict_weak_order<F &, iter_value_t<I1> &, iter_value_t<I2> &>
  && strict_weak_order<F &, iter_value_t<I1> &, iter_reference_t<I2>>
  && strict_weak_order<F &, iter_reference_t<I1>, iter_value_t<I2> &>
  && strict_weak_order<F &, iter_reference_t<I1>, iter_reference_t<I2>>
  && strict_weak_order<F &, iter_common_reference_t<I1>,
                       iter_common_reference_t<I2>>;

template <class F, class...IS>
requires (indirectly_readable<IS> && ...)
  && invocable<F, iter_reference_t<IS>...>
using indirect_result_t = invoke_result_t<F, iter_reference_t<IS>...>;

template <indirectly_readable I, indirectly_regular_unary_invocable<I> PROJ>
struct projected {
  using value_type = remove_cvref_t<indirect_result_t<PROJ &, I>>;
  indirect_result_t<PROJ &, I> operator *() const;
};
template <weakly_incrementable I, class PROJ>
struct incrementable_traits<projected<I, PROJ>> {
  using difference_type = iter_difference_t<I>;
};

template <class IN, class OUT>
concept indirectly_copyable = indirectly_readable<IN>
  && indirectly_writable<OUT, iter_reference_t<IN>>;

template <class IN, class OUT>
concept indirectly_copyable_storable
  = indirectly_copyable<IN, OUT>
  && indirectly_writable<OUT, iter_value_t<IN> &>
  && indirectly_writable<OUT, const iter_value_t<IN> &>
  && indirectly_writable<OUT, iter_value_t<IN> &&>
  && indirectly_writable<OUT, const iter_value_t<IN> &&>
  && copyable<iter_value_t<IN>>
  && constructible_from<iter_value_t<IN>, iter_reference_t<IN>>
  && assignable_from<iter_value_t<IN> &, iter_reference_t<IN>>;

template <class I1, class I2 = I1>
concept indirectly_swappable
  = indirectly_readable<I1> && indirectly_readable<I2>
  && requires(const I1 i1, const I2 i2) {
    ranges::iter_swap(i1, i1);
    ranges::iter_swap(i2, i2);
    ranges::iter_swap(i1, i2);
    ranges::iter_swap(i2, i1);
  };

template <class I1, class I2, class R,
          class P1 = identity, class P2 = identity>
concept indirectly_comparable
  = indirect_binary_predicate<R, projected<I1, P1>, projected<I2, P2>>;

template <class I>
concept permutable = forward_iterator<I> && indirectly_movable_storable<I, I>
  && indirectly_swappable<I, I>;

template <class I1, class I2, class OUT, class R = ranges::less,
          class P1 = identity, class P2 = identity>
concept mergeable = input_iterator<I1> && input_iterator<I2>
  && weakly_incrementable<OUT>
  && indirectly_copyable<I1, OUT>
  && indirectly_copyable<I2, OUT>
  && indirect_strict_weak_order<R, projected<I1, P1>, projected<I2, P2>>;

template <class I, class R = ranges::less, class P = identity>
concept sortable = permutable<I>
  && indirect_strict_weak_order<R, projected<I, P>>;

}

// iterator main components
namespace re {

namespace inner {

template <class I>
struct itr_vt_impl {
  using type = void;
};
template <class I>
requires requires {typename I::value_type;}
struct itr_vt_impl<I> {
  using type = typename I::value_type;
};
template <class I>
struct itr_vt_impl<const I> : itr_vt_impl<I> {};
template <class T>
requires is_object_v<T>
struct itr_vt_impl<T *> {
  using type = remove_cv_t<T>;
};

template <class I>
struct itr_ptr_impl {
  using type = void;
};
template <class I>
requires requires {typename I::pointer;}
struct itr_ptr_impl<I> {
  using type = typename I::pointer;
};
template <class I>
struct itr_ptr_impl<const I> : itr_ptr_impl<I> {};
template <class T>
requires is_object_v<T>
struct itr_ptr_impl<T *> {
  using type = T *;
};

template <class I>
struct itr_ref_impl {
  using type = void;
};
template <class I>
requires requires {typename I::reference;}
struct itr_ref_impl<I> {
  using type = typename I::reference;
};
template <class I>
struct itr_ref_impl<const I> : itr_ref_impl<I> {};
template <class T>
requires is_object_v<T>
struct itr_ref_impl<T *> {
  using type = T &;
};

template <class I>
struct itr_cref_impl {
  using r = typename inner::itr_ref_impl<I>::type;
  using type = conditional_t<is_lvalue_reference_v<r>,
                             add_lvalue_reference_t
                             <add_const_t<remove_reference_t<r>>>,
                             r>;
};

template <class I>
struct itr_rref_impl {
  using r = typename inner::itr_ref_impl<I>::type;
  using type = conditional_t<is_lvalue_reference_v<r>,
                             add_rvalue_reference_t<remove_reference_t<r>>,
                             r>;
};

template <class I>
struct itr_common_ref_impl {
  using type = void;
};
template <class I>
requires requires {
  typename common_reference_t<add_lvalue_reference_t
                              <typename inner::itr_vt_impl<I>::type>,
                              typename inner::itr_ref_impl<I>::type>;
}
struct itr_common_ref_impl<I> {
  using type = common_reference_t<add_lvalue_reference_t
                                  <typename inner::itr_vt_impl<I>::type>,
                                  typename inner::itr_ref_impl<I>::type>;
};

template <class I>
struct itr_dft_impl {
  using type = void;
};
template <class I>
requires requires {typename I::difference_type;}
struct itr_dft_impl<I> {
  using type = typename I::difference_type;
};
template <class I>
struct itr_dft_impl<const I> : itr_dft_impl<I> {};
template <class T>
requires is_object_v<T>
struct itr_dft_impl<T *> {
  using type = ptrdiff_t;
};

template <class I>
struct itr_ctg_impl {
  using type = void;
};
template <class I>
requires requires {typename I::iterator_category;}
struct itr_ctg_impl<I> {
  using type = typename I::iterator_category;
};
template <class I>
struct itr_ctg_impl<const I> : itr_ctg_impl<I> {};
template <class T>
requires is_object_v<T>
struct itr_ctg_impl<T *> {
  using type = contiguous_iterator_tag;
};

}
template <class I>
using itr_vt = typename inner::itr_vt_impl<I>::type;
template <class I>
using itr_ptr = typename inner::itr_ptr_impl<I>::type;
template <class I>
using itr_ref = typename inner::itr_ref_impl<I>::type;
template <class I>
using itr_ptr_from_ref
  = conditional_t<is_reference_v<itr_ref<I>>,
                  add_pointer_t<remove_reference_t<itr_ref<I>>>,
                  void>;
template <class I>
using itr_cref = typename inner::itr_cref_impl<I>::type;
template <class I>
using itr_rref = typename inner::itr_rref_impl<I>::type;
template <class I>
using itr_common_ref = typename inner::itr_common_ref_impl<I>::type;
template <class I>
using itr_dft = typename inner::itr_dft_impl<I>::type;
template <class I>
using itr_ctg = typename inner::itr_ctg_impl<I>::type;

template <class I>
inline constexpr bool is_itr = is_base_of_v<input_iterator_tag, itr_ctg<I>>
  || is_base_of_v<output_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_oitr = is_base_of_v<output_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_iitr = is_base_of_v<input_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_fitr = is_base_of_v<forward_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_bitr
  = is_base_of_v<bidirectional_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_ritr
  = is_base_of_v<random_access_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_citr
  = is_base_of_v<contiguous_iterator_tag, itr_ctg<I>>;
template <class I>
inline constexpr bool is_just_iitr = is_iitr<I> && !is_fitr<I>;
template <class I>
inline constexpr bool is_just_fitr = is_fitr<I> && !is_bitr<I>;
template <class I>
inline constexpr bool is_just_bitr = is_bitr<I> && !is_ritr<I>;
template <class I>
inline constexpr bool is_just_ritr = is_ritr<I> && !is_citr<I>;

template <class CTG>
inline constexpr bool is_itr_ctg = is_base_of_v<input_iterator_tag, CTG>
  || is_base_of_v<output_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_oitr_ctg = is_base_of_v<output_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_iitr_ctg = is_base_of_v<input_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_fitr_ctg = is_base_of_v<forward_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_bitr_ctg
  = is_base_of_v<bidirectional_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_ritr_ctg
  = is_base_of_v<random_access_iterator_tag, CTG>;
template <class CTG>
inline constexpr bool is_citr_ctg
  = is_base_of_v<contiguous_iterator_tag, CTG>;

namespace inner {

template <class I>
concept iter_is_counted_impl = requires(const I &x, const I &y) {
  requires is_itr<I>;
  requires signed_integral<itr_dft<I>>;
  {x - y}->same_as<itr_dft<I>>;
  {y - x}->same_as<itr_dft<I>>;
};

}
template <class I>
struct iterator_is_counted : bool_constant<inner::iter_is_counted_impl<I>> {};
template <class I>
struct iterator_is_counted<const I> : iterator_is_counted<I> {};
template <class I>
inline constexpr bool itr_is_counted = iterator_is_counted<I>::value;

struct fo_advance {
  template <class I, class T>
  constexpr void operator ()(I &i, T n) const requires is_integral_v<T> {
    if constexpr (is_ritr<I>) {
      i += n;
    }
    else if constexpr (is_just_bitr<I>) {
      if (n < 0) {
        do {
          --i;
        } while (++n < 0);
      }
      else
        for (; n; --n)
          ++i;
    }
    else {
      for (; n; --n)
        ++i;
    }
  }
  template <class I, class T>
  constexpr void operator ()(I &i, T j) const requires (!is_integral_v<T>) {
    if constexpr (itr_is_counted<I>) {
      operator ()(i, j - i);
    }
    else {
      while (i != j)
        ++i;
    }
  }
  template <class I, class T>
  constexpr itr_dft<I> operator ()(I &i, itr_dft<I> n, T j) const {
    if constexpr (itr_is_counted<I>) {
      auto n2 = j - i;
      auto nn = n >= 0 ? n : -n;
      auto nn2 = n2 >= 0 ? n2 : -n2;
      if (nn >= nn2) {
        operator ()(i, j);
        return n - n2;
      }
      else {
        operator ()(i, n);
        return 0;
      }
    }
    else if constexpr (is_bitr<I>) {
      itr_dft<I> c = 0;
      if (n >= 0) {
        for (; c != n; ++c) {
          if (i != j)
            ++i;
          else
            break;
        }
      }
      else {
        for (; c != n; --c) {
          if (i != j)
            --i;
          else
            break;
        }
      }
      return n - c;
    }
    else {
      itr_dft<I> c = 0;
      for (; c != n; ++c) {
        if (i != j)
          ++i;
        else
          break;
      }
      return n - c;
    }
  }
};
inline constexpr fo_advance advance{};
struct fo_distance {
  template <class I>
  constexpr itr_dft<I> operator ()(I from, I to) const {
    itr_dft<I> n = 0;
    for (; from != to; ++from)
      ++n;
    return n;
  }
  template <class I>
  constexpr auto operator ()(I from, I to) const
    requires itr_is_counted<I> {
    return to - from;
  }
};
inline constexpr fo_distance distance{};
struct fo_next {
  template <class I>
  constexpr I operator ()(I i) const {
    return ++i;
  }
  template <class I>
  constexpr I operator ()(I i, itr_dft<I> n) const {
    advance(i, n);
    return i;
  }
  template <class I, class S>
  constexpr I operator ()(I i, S bound) const requires (!is_integral_v<S>) {
    advance(i, bound);
    return i;
  }
  template <class I, class S>
  constexpr I operator ()(I i, itr_dft<I> n, S bound) const {
    advance(i, n, bound);
    return i;
  }
};
inline constexpr fo_next next{};
struct fo_prev {
  template <class I>
  constexpr I operator ()(I i) const {
    return --i;
  }
  template <class I>
  constexpr I operator ()(I i, itr_dft<I> n) const {
    advance(i, -n);
    return i;
  }
  template <class I, class S>
  constexpr I operator ()(I i, itr_dft<I> n, S bound) const {
    advance(i, -n, bound);
    return i;
  }
};
inline constexpr fo_prev prev{};

namespace inner {

template <class A, class B,
          bool Y1 = is_convertible_v<A, B>,
          bool Y2 = is_convertible_v<B, A>>
struct common_iter : common_type<A, B> {};
template <class A, class B>
struct common_iter<A, B, true, false> {
  using type = B;
};
template <class A, class B>
struct common_iter<A, B, false, true> {
  using type = A;
};

}
template <class A, class B>
using common_iter_t = typename inner::common_iter<A, B>::type;
template <class A, class B>
concept has_common_iter = requires {
  typename common_iter_t<A, B>;
};

template <class VT>
class iter_post_increment_return_type {
  using this_t = iter_post_increment_return_type;

  VT x;

public:
  iter_post_increment_return_type() = delete;
  ~iter_post_increment_return_type() = default;
  iter_post_increment_return_type(const this_t &) = delete;
  this_t &operator =(const this_t &) = delete;
  iter_post_increment_return_type(this_t &&) = default;
  this_t &operator =(this_t &&) = delete;

  template <class T>
  constexpr explicit iter_post_increment_return_type(T &&xx)
    requires is_convertible_v<T &&, VT> : x(forward<T>(xx)) {}

  constexpr VT operator *() && {
    return move(x);
  }
};
struct fo_iter_post_increment {
  template <class I>
  constexpr auto operator ()(I &i) const {
    if constexpr (is_just_iitr<I>) {
      iter_post_increment_return_type<itr_vt<I>> ret(*i);
      ++i;
      return ret;
    }
    else {
      auto ret = i;
      ++i;
      return ret;
    }
  }
};
inline constexpr fo_iter_post_increment iter_post_increment{};
struct fo_iter_post_decrement {
  template <class I>
  constexpr auto operator ()(I &i) const {
    auto ret = i;
    --i;
    return ret;
  }
};
inline constexpr fo_iter_post_decrement iter_post_decrement{};

}

// range main components
// range:
//   (1) re::begin(r), re::end(r), re::empty(r) and re::size(r) are all well
//       foremd
//   (2) re::rbegin(r), re::rend(r) is defined only if the iterator is
//       bidirectional
namespace re {

struct fo_begin {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.begin()) {
    return r.begin();
  }
  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.begin()) {
    return r.begin();
  }
  template <class T, size_t N>
  constexpr T *operator ()(T (&a)[N]) const noexcept {
    return a;
  }
  template <class T>
  constexpr const T *operator ()(initializer_list<T> l) const noexcept {
    return l.begin();
  }
};
struct fo_end {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.end()) {
    return r.end();
  }
  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.end()) {
    return r.end();
  }
  template <class T, size_t N>
  constexpr T *operator ()(T (&a)[N]) const noexcept {
    return a + N;
  }
  template <class T>
  constexpr const T *operator ()(initializer_list<T> l) const noexcept {
    return l.end();
  }
};
inline constexpr fo_begin begin{};
inline constexpr fo_end end{};
namespace inner {

template <class R>
concept is_rng_impl = requires(R &r) {
  requires is_same_v<decltype(begin(r)), decltype(end(r))>;
};

}
template <class R>
inline constexpr bool is_rng = inner::is_rng_impl<R>;

namespace inner {

template <class T>
concept has_mfn_size = requires(T &x) {
  {x.size()}->unsigned_integral;
};

}
struct fo_size {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.size()) {
    return r.size();
  }
  template <class R>
  constexpr make_unsigned_t<itr_dft<decltype(begin(declval<R &>()))>>
  operator ()(R &r) const requires (!inner::has_mfn_size<R> && is_rng<R>) {
    return distance(begin(r), end(r));
  }

  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.size()) {
    return r.size();
  }
  template <class R>
  constexpr make_unsigned_t<itr_dft<decltype(begin(declval<const R &>()))>>
  operator ()(const R &r) const
    requires (!inner::has_mfn_size<const R> && is_rng<const R>) {
    return distance(begin(r), end(r));
  }

  template <class T, size_t N>
  constexpr size_t operator ()(T (&)[N]) const noexcept {
    return N;
  }
  template <class T, size_t N>
  constexpr size_t operator ()(const T (&)[N]) const noexcept {
    return N;
  }
  template <class E>
  constexpr size_t operator ()(initializer_list<E> l) const noexcept {
    return l.size();
  }
};
inline constexpr fo_size size{};

struct fo_ssize {
  template <class R>
  constexpr auto operator ()(R &r) const
    ->common_type_t<ptrdiff_t, make_signed_t<decltype(size(r))>> {
    return static_cast
      <common_type_t<ptrdiff_t, make_signed_t<decltype(size(r))>>>(size(r));
  }
  template <class R>
  constexpr auto operator ()(const R &r) const
    ->common_type_t<ptrdiff_t, make_signed_t<decltype(size(r))>> {
    return static_cast
      <common_type_t<ptrdiff_t, make_signed_t<decltype(size(r))>>>(size(r));
  }
};
inline constexpr fo_ssize ssize{};

namespace inner {

template <class T>
concept has_mfn_empty = requires(T &x) {{x.empty()}->same_as<bool>;};

}
struct fo_empty {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.empty()) {
    return r.empty();
  }
  template <class R>
  constexpr bool operator ()(R &r) const
    requires (!inner::has_mfn_empty<R> && is_rng<R>) {
    return begin(r) == end(r);
  }

  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.empty()) {
    return r.empty();
  }
  template <class R>
  constexpr bool operator ()(const R &r) const
    requires (!inner::has_mfn_empty<const R> && is_rng<const R>) {
    return begin(r) == end(r);
  }

  template <class T, size_t N>
  constexpr bool operator ()(T (&)[N]) const noexcept {
    return false;
  }
  template <class T, size_t N>
  constexpr bool operator ()(const T (&)[N]) const noexcept {
    return false;
  }
  template <class E>
  constexpr bool operator ()(initializer_list<E> l) const noexcept {
    return l.size() == 0;
  }
};
inline constexpr fo_empty empty{};

template <class T>
class n_value_iterator {
  template <class>
  friend class n_value_iterator;

  T *p;
  ptrdiff_t n;

public:
  using value_type = remove_cv_t<T>;
  using difference_type = ptrdiff_t;
  using pointer = T *;
  using reference = T &;
  using iterator_category = random_access_iterator_tag;

  constexpr n_value_iterator() noexcept : p{}, n{} {}
  ~n_value_iterator() = default;
  n_value_iterator(const n_value_iterator &) = default;
  n_value_iterator &operator =(const n_value_iterator &) = default;
  n_value_iterator(n_value_iterator &&) = default;
  n_value_iterator &operator =(n_value_iterator &&) = default;
  friend constexpr void swap(n_value_iterator &x,
                             n_value_iterator &y) noexcept {
    adl_swap(x.p, y.p);
    adl_swap(x.n, y.n);
  }

  constexpr n_value_iterator(T &xx, ptrdiff_t nn) : p(addressof(xx)), n(nn) {}
  constexpr auto base() const {
    return p;
  }

  constexpr reference operator *() const {
    return *p;
  }
  constexpr pointer operator ->() const {
    return p;
  }
  friend bool operator ==(n_value_iterator x, n_value_iterator y) = default;

  constexpr n_value_iterator &operator ++() {
    ++n;
    return *this;
  }
  constexpr n_value_iterator operator ++(int) {
    auto tmp = *this;
    operator ++();
    return tmp;
  }

  constexpr n_value_iterator &operator --() {
    --n;
    return *this;
  }
  constexpr n_value_iterator operator --(int) {
    auto tmp = *this;
    operator --();
    return tmp;
  }

  constexpr reference operator [](difference_type) const {
    return *p;
  }
  constexpr n_value_iterator &operator +=(difference_type i) {
    n += i;
    return *this;
  }
  constexpr n_value_iterator operator +(difference_type i) const {
    return copy(*this) += i;
  }
  constexpr n_value_iterator &operator -=(difference_type i) {
    n -= i;
    return *this;
  }
  constexpr n_value_iterator operator -(difference_type i) const {
    return copy(*this) -= i;
  }
  friend constexpr n_value_iterator operator +(difference_type i,
                                               n_value_iterator x) {
    return copy(x) += i;
  }
  friend constexpr difference_type operator -(n_value_iterator x,
                                              n_value_iterator y) {
    return x.n - y.n;
  }
  friend constexpr strong_ordering operator <=>(n_value_iterator x,
                                                n_value_iterator y) {
    return x.n <=> y.n;
  }
};
struct fo_n_value_itr {
  template <class T>
  constexpr n_value_iterator<T> operator ()(T &x, ptrdiff_t n) const {
    return {x, n};
  }
};
inline constexpr fo_n_value_itr n_value_itr{};
template <class T>
class n_value {
  size_t n = size_t{};
  T value = T{};

public:
  n_value() = delete;
  n_value() requires is_default_constructible_v<T> = default;
  ~n_value() = default;
  n_value(const n_value &) = default;
  n_value &operator =(const n_value &) = default;
  n_value(n_value &&) = default;
  n_value &operator =(n_value &&) = default;
  friend constexpr void swap(n_value &x, n_value &y)
    noexcept(is_nothrow_swappable_v<T>)
    requires is_swappable_v<T> && default_swappable<n_value> {
    adl_swap(x.n, y.n);
    adl_swap(x.value, y.value);
  }

  template <class...S>
  constexpr explicit(sizeof...(S) == 0)
  n_value(size_t nn, S &&...s)
    requires is_constructible_v<T, S &&...> : n(nn), value(forward<S>(s)...) {}

  constexpr n_value_iterator<add_const_t<remove_reference_t<T>>>
  begin() const noexcept {
    return {value, 0};
  };
  constexpr n_value_iterator<add_const_t<remove_reference_t<T>>>
  end() const noexcept {
    return {value, to_signed(n)};
  };
  constexpr size_t size() const noexcept {
    return n;
  }
  constexpr bool empty() const noexcept {
    return n == 0;
  }
};
template <class R>
struct range_is_n_value : false_type {};
template <class R>
struct range_is_n_value<const R> : range_is_n_value<R> {};
template <class R>
struct range_is_n_value<R &> : range_is_n_value<R> {};
template <class R>
struct range_is_n_value<R &&> : range_is_n_value<R> {};
template <class T>
struct range_is_n_value<n_value<T>> : true_type {};

namespace inner {

template <class R>
struct rng_itr_impl {
  using type = void;
};
template <class R>
requires requires (R &r) {begin(r);}
struct rng_itr_impl<R> {
  using type = conditional_t<is_rng<R>, decltype(begin(declval<R &>())), void>;
};

template <class R>
struct rng_szt_impl {
  using type = void;
};
template <class R>
requires requires (R &r) {size(r);}
struct rng_szt_impl<R> {
  using type = conditional_t<is_rng<R>, decltype(size(declval<R &>())), void>;
};

template <class R>
struct rng_is_counted_impl : false_type {};
template <class R>
requires requires (R &r) {begin(r);}
struct rng_is_counted_impl<R>
  : iterator_is_counted<decltype(begin(declval<R &>()))> {};

}

template <class R>
using rng_itr = typename inner::rng_itr_impl<R>::type;
template <class R>
using rng_szt = typename inner::rng_szt_impl<R>::type;
template <class R>
using rng_vt = itr_vt<rng_itr<R>>;
template <class R>
using rng_ptr = itr_ptr<rng_itr<R>>;
template <class R>
using rng_ptr_from_ref = itr_ptr_from_ref<rng_itr<R>>;
template <class R>
using rng_ref = itr_ref<rng_itr<R>>;
template <class R>
using rng_cref = itr_cref<rng_itr<R>>;
template <class R>
using rng_rref = itr_rref<rng_itr<R>>;
template <class R>
using rng_common_ref = itr_common_ref<rng_itr<R>>;
template <class R>
using rng_dft = itr_dft<rng_itr<R>>;
template <class R>
using rng_ctg = itr_ctg<rng_itr<R>>;
template <class R>
inline constexpr bool rng_is_sized = inner::has_mfn_size<R>
  || is_bounded_array_v<remove_reference_t<R>>;
template <class R>
inline constexpr bool rng_is_counted = inner::rng_is_counted_impl<R>::value;
template <class R>
inline constexpr bool rng_is_n_value = is_rng<R> && range_is_n_value<R>::value;

template <class R>
struct range_traits {
  using iterator = rng_itr<R>;
  using value_type = rng_vt<R>;
  using reference = rng_ref<R>;
  using difference_type = rng_dft<R>;
  using pointer = rng_ptr<R>;
  using iterator_category = rng_ctg<R>;
  using const_reference = rng_cref<R>;
  using rvalue_reference = rng_rref<R>;
  using common_reference = rng_common_ref<R>;
  using size_type = rng_szt<R>;
  using is_sized = bool_constant<rng_is_sized<R>>;
  using is_counted = bool_constant<rng_is_counted<R>>;
  using is_n_value = bool_constant<rng_is_n_value<R>>;
};

template <class R>
inline constexpr bool is_orng = is_rng<R> && is_oitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_irng = is_rng<R> && is_iitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_frng = is_rng<R> && is_fitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_brng = is_rng<R> && is_bitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_rrng = is_rng<R> && is_ritr<rng_itr<R>>;
template <class R>
inline constexpr bool is_crng = is_rng<R> && is_citr<rng_itr<R>>;
template <class R>
inline constexpr bool is_just_irng = is_rng<R> && is_just_iitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_just_frng = is_rng<R> && is_just_fitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_just_brng = is_rng<R> && is_just_bitr<rng_itr<R>>;
template <class R>
inline constexpr bool is_just_rrng = is_rng<R> && is_just_ritr<rng_itr<R>>;

template <class>
class iter_pair;
namespace inner {

struct check_can_be_iter_pair {
  template <class I>
  static void help_f(iter_pair<I>);

  template <class R>
  static enable f(type_pack<R>)
    requires (requires(R &&r) {help_f(static_cast<R &&>(r));});
};
template <class R>
struct can_be_iter_pair
  : f_is_well_formed<check_can_be_iter_pair, R> {};

}
template <class R>
inline constexpr bool is_iter_pair
  = is_rng<R> && inner::can_be_iter_pair<R>::value;

template <class R>
struct is_range_reference : bool_constant<is_rng<R> && is_iter_pair<R>> {};
template <class T>
struct is_range_reference<n_value<T>> : is_lvalue_reference<T> {};
template <class R>
struct is_range_reference<const R>
  : bool_constant<is_rng<const R> && is_range_reference<R>::value> {};
template <class R>
struct is_range_reference<R &> : bool_constant<is_rng<R>> {};
template <class R>
struct is_range_reference<R &&> : is_range_reference<R> {};
template <class R>
inline constexpr bool is_rng_ref = is_range_reference<R>::value;

namespace inner::fns {

template <class R>
R get_rng_forward_t(R &&);
template <class R>
decay_t<R> get_rng_forward_t(R &&) requires is_rng_ref<decay_t<R>>;

}
template <class R>
using rng_forward_t = decltype(inner::fns::get_rng_forward_t(declval<R>()));

namespace inner {

template <class R>
concept has_mfn_before_end = requires(R &x) {x.before_end();};

}
struct fo_before_begin {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.before_begin()) {
    return r.before_begin();
  }
  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.before_begin()) {
    return r.before_begin();
  }
};
inline constexpr fo_before_begin before_begin{};
struct fo_before_end {
  template <class FR>
  constexpr auto operator ()(FR &r) const->decltype(r.before_end())
    requires inner::has_mfn_before_end<FR> {
    return r.before_end();
  }
  template <class FR>
  constexpr enable_if_t<is_frng<FR>, rng_itr<FR>> operator ()(FR &r) const {
    if constexpr (is_brng<FR>) {
      return prev(end(r));
    }
    else {
      auto i = begin(r);
      auto it = next(i);
      const auto ed = end(r);
      while (it != ed) {
        i = it;
        ++it;
      }
      return i;
    }
  }

  template <class FR>
  constexpr auto operator ()(const FR &r) const->decltype(r.before_end())
    requires inner::has_mfn_before_end<FR> {
    return r.before_end();
  }
  template <class FR>
  constexpr enable_if_t<is_frng<const FR>, rng_itr<const FR>>
  operator ()(const FR &r) const {
    if constexpr (is_brng<const FR>) {
      return prev(end(r));
    }
    else {
      auto i = begin(r);
      auto it = next(i);
      const auto ed = end(r);
      while (it != ed) {
        i = it;
        ++it;
      }
      return i;
    }
  }
};
inline constexpr fo_before_end before_end{};

struct fo_cbegin {
  template <class R>
  constexpr auto operator ()(const R &r)
    const noexcept(noexcept(begin(r)))->decltype(begin(r)) {
    return begin(r);
  }
};
struct fo_cend {
  template <class R>
  constexpr auto operator ()(const R &r)
    const noexcept(noexcept(end(r)))->decltype(end(r)) {
    return end(r);
  }
};
inline constexpr fo_cbegin cbegin{};
inline constexpr fo_cend cend{};

template <class>
class reverse_iterator;
namespace inner::fns {

template <class I>
constexpr reverse_iterator<I> make_ritr(I i) {
  return reverse_iterator<I>(i);
}
template <class I>
constexpr I make_ritr(reverse_iterator<I> i) {
  return i.base();
}

}
struct fo_rbegin {
  template <class T, size_t N>
  constexpr reverse_iterator<T *> operator ()(T (&a)[N]) const noexcept {
    return reverse_iterator<T *>(a + N);
  }
  template <class T, size_t N>
  constexpr reverse_iterator<const T *>
  operator ()(initializer_list<T> l) const {
    return reverse_iterator<const T *>(l.end());
  }

  template <class R>
  constexpr auto operator ()(R &r) const
    requires (is_brng<R> && !is_array_v<remove_reference_t<R>>) {
    return inner::fns::make_ritr(end(r));
  }
  template <class R>
  constexpr auto operator ()(const R &r) const
    requires (is_brng<const R>
              && !is_array_v<remove_reference_t<const R>>) {
    return inner::fns::make_ritr(end(r));
  }
};
struct fo_rend {
  template <class T, size_t N>
  constexpr reverse_iterator<T *> operator ()(T (&a)[N]) const noexcept {
    return reverse_iterator<T *>(a);
  }
  template <class T, size_t N>
  constexpr reverse_iterator<const T *>
  operator ()(initializer_list<T> l) const noexcept {
    return reverse_iterator<const T *>(l.begin());
  }

  template <class R>
  constexpr auto operator ()(R &r) const
    requires (is_brng<R> && !is_array_v<remove_reference_t<R>>) {
    return inner::fns::make_ritr(begin(r));
  }
  template <class R>
  constexpr auto operator ()(const R &r) const
    requires (is_brng<const R>
              && !is_array_v<remove_reference_t<const R>>) {
    return inner::fns::make_ritr(begin(r));
  }
};
inline constexpr fo_rbegin rbegin{};
inline constexpr fo_rend rend{};

struct fo_crbegin {
  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(rbegin(r)) {
    return rbegin(r);
  }
};
struct fo_crend {
  template <class R>
  constexpr auto operator ()(const R &r) const ->decltype(rend(r)) {
    return rend(r);
  }
};
inline constexpr fo_crbegin crbegin{};
inline constexpr fo_crend crend{};

namespace inner {

template <class R>
concept has_mfn_data = requires(R &x) {x.data();};

}
struct fo_data {
  template <class R>
  constexpr auto operator ()(R &r) const->decltype(r.data()) {
    return r.data();
  }
  template <class R>
  constexpr auto operator ()(const R &r) const->decltype(r.data()) {
    return r.data();
  }
  template <class T, size_t N>
  constexpr T *operator ()(T (&a)[N]) const noexcept {
    return a;
  }
  template <class E>
  constexpr const E *operator ()(initializer_list<E> l) const noexcept {
    return l.begin();
  }

  template <class R>
  constexpr auto operator ()(R &r)->decltype(to_address(begin(r)))
    requires (!inner::has_mfn_data<R>) && is_crng<R> {
    return to_address(begin(r));
  }
  template <class R>
  constexpr auto operator ()(const R &r)->decltype(to_address(begin(r)))
    requires (!inner::has_mfn_data<const R>) && is_crng<const R> {
    return to_address(begin(r));
  }
};
inline constexpr fo_data data{};

namespace inner {

template <class R>
concept has_mfn_nth = requires(R &x) {x.nth(0);};

}
struct fo_nth {
  template <class R>
  constexpr decltype(auto) operator ()(R &r, rng_dft<R> n) const {
    return next(begin(r), n);
  }
  template <class R>
  constexpr decltype(auto) operator ()(R &r, rng_dft<R> n) const
    requires inner::has_mfn_nth<R> {
    return r.nth(n);
  }

  template <class R>
  constexpr decltype(auto) operator ()(const R &r,
                                       rng_dft<const R> n) const {
    return next(begin(r), n);
  }
  template <class R>
  constexpr decltype(auto) operator ()(const R &r, rng_dft<const R> n) const
    requires inner::has_mfn_nth<const R> {
    return r.nth(n);
  }
};
inline constexpr fo_nth nth{};

template <class R>
constexpr decltype(auto)
fo_ref::operator ()(R &r,
                    typename range_traits<R>::difference_type n) const {
  return *nth(r, n);
}
template <class R>
constexpr decltype(auto)
fo_ref::operator ()(const R &r,
                    typename range_traits<R>::difference_type n) const {
  return *nth(r, n);
}

struct fo_at_most_nth {
  template <class R>
  constexpr auto operator ()(R &r, rng_dft<R> n) const {
    const auto ed = end(r);
    for (auto i = begin(r); i != ed; ++i) {
      if (n == 0)
        return i;
      --n;
    }
    return ed;
  }
  template <class R>
  constexpr auto operator ()(R &r, rng_dft<R> n) const
    requires (rng_is_sized<R> && !inner::has_mfn_nth<R>) {
    const auto sz = ssize(r);
    return n <= sz ? next(begin(r), n) : end(r);
  }
  template <class R>
  constexpr auto operator ()(R &r, rng_dft<R> n) const
    requires rng_is_sized<R> && inner::has_mfn_nth<R> {
    const auto sz = ssize(r);
    return r.nth(n <= sz ? n : sz);
  }

  template <class R>
  constexpr auto operator ()(const R &r, rng_dft<const R> n) const {
    const auto ed = end(r);
    for (auto i = begin(r); i != ed; ++i) {
      if (n == 0)
        return i;
      --n;
    }
    return ed;
  }
  template <class R>
  constexpr auto operator ()(const R &r, rng_dft<const R> n) const
    requires (rng_is_sized<const R> && !inner::has_mfn_nth<const R>) {
    const auto sz = ssize(r);
    return n <= sz ? next(begin(r), n) : end(r);
  }
  template <class R>
  constexpr auto operator ()(const R &r, rng_dft<const R> n) const
    requires rng_is_sized<const R> && inner::has_mfn_nth<const R> {
    const auto sz = ssize(r);
    return r.nth(n <= sz ? n : sz);
  }
};
inline constexpr fo_at_most_nth at_most_nth{};

namespace inner {

template <class R>
concept has_mfn_front = requires(R &x) {x.front();};
template <class R>
concept has_mfn_back = requires(R &x) {x.back();};

}
struct fo_front {
  template <class R>
  constexpr auto operator ()(R &x) const->decltype(*begin(x)) {
    return *begin(x);
  }
  template <class R>
  constexpr auto operator ()(R &x) const->decltype(x.front())
    requires inner::has_mfn_front<R> {
    return x.front();
  }

  template <class R>
  constexpr auto operator ()(const R &x) const->decltype(*begin(x)) {
    return *begin(x);
  }
  template <class R>
  constexpr auto operator ()(const R &x) const->decltype(x.front())
    requires inner::has_mfn_front<const R> {
    return x.front();
  }
};
inline constexpr fo_front front{};
struct fo_back {
  template <class R>
  constexpr auto operator ()(R &x) const->decltype(*begin(x)) {
    if constexpr (inner::has_mfn_before_end<R>) {
      return *x.before_end();
    }
    else {
      return *prev(end(x));
    }
  }
  template <class R>
  constexpr auto operator ()(R &x) const->decltype(x.back())
    requires inner::has_mfn_back<R> {
    return x.back();
  }

  template <class R>
  constexpr auto operator ()(const R &x) const->decltype(*begin(x)) {
    if constexpr (inner::has_mfn_before_end<const R>) {
      return *x.before_end();
    }
    else {
      return *prev(end(x));
    }
  }
  template <class R>
  constexpr auto operator ()(const R &x) const->decltype(x.back())
    requires inner::has_mfn_back<const R> {
    return x.back();
  }
};
inline constexpr fo_back back{};

struct range_fns {
  template <class R>
  static constexpr decltype(auto) begin(R &&r) {
    return re::begin(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) end(R &&r) {
    return re::end(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) rbegin(R &&r) {
    return re::rbegin(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) rend(R &&r) {
    return re::rend(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) size(R &&r) {
    return re::size(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) empty(R &&r) {
    return re::empty(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) nth(R &&r, rng_dft<R> n) {
    return re::nth(forward<R>(r), n);
  }
  template <class R>
  static constexpr decltype(auto) ref(R &&r, rng_dft<R> n) {
    return re::ref(forward<R>(r), n);
  }
  template <class R>
  static constexpr decltype(auto) front(R &&r) {
    return re::front(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) back(R &&r) {
    return re::back(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) cbegin(R &&r) {
    return re::cbegin(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) cend(R &&r) {
    return re::cend(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) crbegin(R &&r) {
    return re::crbegin(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) crend(R &&r) {
    return re::crend(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) ssize(R &&r) {
    return re::ssize(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) data(R &&r) {
    return re::data(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) before_begin(R &&r) {
    return re::before_begin(forward<R>(r));
  }
  template <class R>
  static constexpr decltype(auto) before_end(R &&r) {
    return re::before_end(forward<R>(r));
  }
};

}

// degraded_iterator
namespace re {

template <class I, class CTG>
class degraded_iterator {
  template <class II, class CTGG>
  friend class degraded_iterator;
  using this_t = degraded_iterator;

  I it = I{};

public:
  using iterator_type = I;

  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category = CTG;

  degraded_iterator() = default;
  ~degraded_iterator() = default;
  degraded_iterator(const this_t &) = default;
  degraded_iterator &operator =(const this_t &) = default;
  degraded_iterator(this_t &&) = default;
  degraded_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(degraded_iterator &x, degraded_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.it, y.it);
  }

  constexpr explicit degraded_iterator(I i) : it(i) {}
  constexpr I base() const {
    return it;
  }

  template <class II>
  constexpr degraded_iterator(const degraded_iterator<II, CTG> &x)
    requires (!is_same_v<I, II>) && is_convertible_v<const II &, I>
    : it(x.it) {}

  constexpr I operator ->() const requires can_apply_to_address<const I &> {
    return it;
  }
  constexpr decltype(auto) operator *() const {
    return *it;
  }
  constexpr this_t &operator ++() {
    ++it;
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int) requires is_fitr_ctg<CTG> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr_ctg<CTG> {
    --it;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr_ctg<CTG> {
    return iter_post_decrement(*this);
  }

  constexpr decltype(auto) operator [](difference_type n) const
    requires is_ritr_ctg<CTG> {
    return it[n];
  }
  constexpr this_t &operator +=(difference_type n)
    requires is_ritr_ctg<CTG> {
    it += n;
    return *this;
  }
  constexpr this_t &operator -=(difference_type n)
    requires is_ritr_ctg<CTG> {
    it -= n;
    return *this;
  }
  constexpr this_t operator +(difference_type n) const
    requires is_ritr_ctg<CTG> {
    return copy(*this) += n;
  }
  constexpr this_t operator -(difference_type n) const
    requires is_ritr_ctg<CTG> {
    return copy(*this) -= n;
  }
};
template <class I, class II, class CTG>
constexpr auto operator ==(const degraded_iterator<I, CTG> &x,
                           const degraded_iterator<II, CTG> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class I, class CTG>
constexpr degraded_iterator<I, CTG>
operator +(itr_dft<I> n, const degraded_iterator<I, CTG> &i)
  requires is_ritr_ctg<CTG> {
  return copy(i) += n;
}
template <class I, class II, class CTG>
constexpr auto operator -(const degraded_iterator<I, CTG> &x,
                          const degraded_iterator<II, CTG> &y)
  ->decltype(x.base() - y.base())
  requires itr_is_counted<I> && itr_is_counted<II> {
  return x.base() - y.base();
}
template <class I, class II, class CTG>
constexpr synth_3way_result<I, II>
operator <=>(const degraded_iterator<I, CTG> &x,
             const degraded_iterator<II, CTG> &y)
  requires is_ritr_ctg<CTG> {
  return synth_3way(x.base(), y.base());
}

template <class I>
using degraded_input_iterator = degraded_iterator<I, input_iterator_tag>;
template <class I>
using degraded_forward_iterator = degraded_iterator<I, forward_iterator_tag>;
template <class I>
using degraded_bidirectional_iterator
  = degraded_iterator<I, bidirectional_iterator_tag>;
template <class I>
using degraded_random_access_iterator
  = degraded_iterator<I, random_access_iterator_tag>;

template <class TAG>
struct fo_degraded_itr {
  template <class I>
  constexpr degraded_iterator<I, TAG> operator ()(I i) const
    requires is_base_of_v<TAG, itr_ctg<I>> {
    return degraded_iterator<I, TAG>(i);
  }
};
template <class TAG>
inline constexpr fo_degraded_itr<TAG> degraded_itr{};
struct fo_degraded_iitr {
  template <class I>
  constexpr auto operator ()(I i) const requires is_iitr<I> {
    return degraded_itr<input_iterator_tag>(i);
  }
};
struct fo_degraded_fitr {
  template <class I>
  constexpr auto operator ()(I i) const requires is_fitr<I> {
    return degraded_itr<forward_iterator_tag>(i);
  }
};
struct fo_degraded_bitr {
  template <class I>
  constexpr auto operator ()(I i) const requires is_bitr<I> {
    return degraded_itr<bidirectional_iterator_tag>(i);
  }
};
struct fo_degraded_ritr {
  template <class I>
  constexpr auto operator ()(I i) const requires is_ritr<I> {
    return degraded_itr<random_access_iterator_tag>(i);
  }
};
inline constexpr fo_degraded_iitr degraded_iitr{};
inline constexpr fo_degraded_fitr degraded_fitr{};
inline constexpr fo_degraded_bitr degraded_bitr{};
inline constexpr fo_degraded_ritr degraded_ritr{};

}

// reverse_iterator
namespace re {

template <class BI>
class reverse_iterator {
  template <class>
  friend class reverse_iterator;

protected:
  BI iter = BI{};

public:
  using iterator_type = BI;

  using value_type = itr_vt<BI>;
  using reference = itr_ref<BI>;
  using pointer = BI;
  using difference_type = itr_dft<BI>;
  using iterator_category = conditional_t
    <is_ritr<BI>, random_access_iterator_tag, bidirectional_iterator_tag>;

  reverse_iterator() = default;
  ~reverse_iterator() = default;
  reverse_iterator(const reverse_iterator &) = default;
  reverse_iterator &operator =(const reverse_iterator &) = default;
  reverse_iterator(reverse_iterator &&) = default;
  reverse_iterator &operator =(reverse_iterator &&) = default;
  friend constexpr void swap(reverse_iterator &x,
                             reverse_iterator &y)
    noexcept(is_nothrow_swappable_v<BI>) {
    adl_swap(x.iter, y.iter);
  }

  constexpr explicit reverse_iterator(BI x) : iter(x) {}

  template <class BI2>
  constexpr reverse_iterator(const reverse_iterator<BI2> &x)
    requires (!is_same_v<BI, BI2>) && is_convertible_v<const BI2 &, BI>
    : iter(x.iter) {}
  template <class BI2>
  constexpr reverse_iterator &operator =(const reverse_iterator<BI2> &x)
    requires (!is_same_v<BI, BI2> && is_convertible_v<const BI2 &, BI>
              && is_assignable_v<BI &, const BI2 &>) {
    iter = x.iter;
    return *this;
  }

  constexpr BI base() const {
    return iter;
  }

  constexpr reference operator *() const {
    BI i = iter;
    return *--i;
  }
  constexpr BI operator ->() const
    requires can_apply_to_address<const BI &> {
    return prev(iter);
  }

  constexpr reverse_iterator &operator ++() {
    --iter;
    return *this;
  }
  constexpr reverse_iterator operator ++(int) {
    reverse_iterator x = *this;
    --iter;
    return x;
  }

  constexpr reverse_iterator &operator --() {
    ++iter;
    return *this;
  }
  constexpr reverse_iterator operator --(int) {
    reverse_iterator x = *this;
    ++iter;
    return x;
  }

  constexpr reverse_iterator &operator +=(difference_type n)
    requires is_ritr<BI> {
    iter -= n;
    return *this;
  }
  constexpr reverse_iterator &operator -=(difference_type n)
    requires is_ritr<BI> {
    iter += n;
    return *this;
  }
  constexpr reverse_iterator operator +(difference_type n) const
    requires is_ritr<BI> {
    return reverse_iterator(iter - n);
  }
  constexpr reverse_iterator operator -(difference_type n) const
    requires is_ritr<BI> {
    return reverse_iterator(iter + n);
  }
  constexpr decltype(auto) operator [](difference_type n) const
    requires is_ritr<BI> {
    return iter[-n - 1];
  }
};
template <class BI1, class BI2>
constexpr auto operator ==(const reverse_iterator<BI1> &x,
                           const reverse_iterator<BI2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class BI1, class BI2>
constexpr synth_3way_result<BI1, BI2>
operator <=>(const reverse_iterator<BI1> &x,
             const reverse_iterator<BI2> &y) {
  return synth_3way(y.base(), x.base());
}
template <class BI1, class BI2>
constexpr auto operator -(const reverse_iterator<BI1> &x,
                          const reverse_iterator<BI2> &y)
  ->decltype(y.base() - x.base())
  requires itr_is_counted<BI1> && itr_is_counted<BI2> {
  return y.base() - x.base();
}
template <class BI>
constexpr reverse_iterator<BI>
operator +(itr_dft<BI> n, const reverse_iterator<BI> &x)
  requires is_ritr<BI> {
  return x + n;
}

struct fo_make_reverse_iterator {
  template <class BI>
  constexpr reverse_iterator<BI> operator ()(BI i) const
    requires is_bitr<BI> {
    return reverse_iterator<BI>(i);
  }
};
inline constexpr fo_make_reverse_iterator make_reverse_iterator{};
struct fo_ritr {
  template <class BI>
  constexpr auto operator ()(BI i) const requires is_bitr<BI> {
    return inner::fns::make_ritr(i);
  }
};
inline constexpr fo_ritr ritr{};

}

// inserter
namespace re {

template <class C>
class back_insert_iterator {
  using this_t = back_insert_iterator;

protected:
  C *container = nullptr;

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  using container_type = C;

  back_insert_iterator() = default;
  ~back_insert_iterator() = default;
  back_insert_iterator(const back_insert_iterator &) = default;
  back_insert_iterator &operator =(const back_insert_iterator &) = default;
  back_insert_iterator(back_insert_iterator &&) = default;
  back_insert_iterator &operator =(back_insert_iterator &&) = default;
  friend constexpr void swap(back_insert_iterator &x,
                             back_insert_iterator &y) noexcept {
    adl_swap(x.container, y.container);
  }

  constexpr explicit back_insert_iterator(C &x) : container(addressof(x)) {}

  constexpr this_t &operator =(const typename C::value_type &x) {
    container->push_back(x);
    return *this;
  }
  constexpr this_t &operator =(typename C::value_type &&x) {
    container->push_back(move(x));
    return *this;
  }

  constexpr back_insert_iterator &operator *() {
    return *this;
  }
  constexpr back_insert_iterator &operator ++() {
    return *this;
  }
  constexpr back_insert_iterator operator ++(int) {
    return *this;
  }
};
struct fo_back_inserter {
  template <class C>
  constexpr back_insert_iterator<C> operator ()(C &x) const {
    return back_insert_iterator<C>(x);
  }
};
inline constexpr fo_back_inserter back_inserter{};

template <class C>
class front_insert_iterator {
  using this_t = front_insert_iterator;

protected:
  C *container = nullptr;

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  using container_type = C;

  front_insert_iterator() = default;
  ~front_insert_iterator() = default;
  front_insert_iterator(const front_insert_iterator &) = default;
  front_insert_iterator &operator =(const front_insert_iterator &) = default;
  front_insert_iterator(front_insert_iterator &&) = default;
  front_insert_iterator &operator =(front_insert_iterator &&) = default;
  friend constexpr void swap(front_insert_iterator &x,
                             front_insert_iterator &y) noexcept {
    adl_swap(x.container, y.container);
  }

  constexpr explicit front_insert_iterator(C &x) : container(addressof(x)) {}

  constexpr this_t &operator =(const typename C::value_type &x) {
    container->push_front(x);
    return *this;
  }
  constexpr this_t &operator =(typename C::value_type &&x) {
    container->push_front(move(x));
    return *this;
  }

  constexpr front_insert_iterator &operator *() {
    return *this;
  }
  constexpr front_insert_iterator &operator ++() {
    return *this;
  }
  constexpr front_insert_iterator operator ++(int) {
    return *this;
  }
};
struct fo_front_inserter {
  template <class C>
  constexpr front_insert_iterator<C> operator ()(C &x) const {
    return front_insert_iterator<C>(x);
  }
};
inline constexpr fo_front_inserter front_inserter{};

template <class C>
class insert_iterator {
  using this_t = insert_iterator;

  using iter_t = typename C::iterator;

protected:
  C *container = nullptr;
  iter_t iter = iter_t{};

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  using container_type = C;

  insert_iterator() = default;
  ~insert_iterator() = default;
  insert_iterator(const insert_iterator &) = default;
  insert_iterator &operator =(const insert_iterator &) = default;
  insert_iterator(insert_iterator &&) = default;
  insert_iterator &operator =(insert_iterator &&) = default;
  friend constexpr void swap(insert_iterator &x, insert_iterator &y)
    noexcept(is_nothrow_swappable_v<iter_t>) {
    adl_swap(x.container, y.container);
    adl_swap(x.iter, y.iter);
  }

  constexpr insert_iterator(C &x, typename C::iterator i)
    : container(addressof(x)), iter(i) {}

  constexpr this_t &operator =(const typename C::value_type &x) {
    iter = container->insert(iter, x);
    ++iter;
    return *this;
  }
  constexpr this_t &operator =(typename C::value_type &&x) {
    iter = container->insert(iter, move(x));
    ++iter;
    return *this;
  }

  constexpr insert_iterator &operator *() {
    return *this;
  }
  constexpr insert_iterator &operator ++() {
    return *this;
  }
  constexpr insert_iterator &operator ++(int) {
    return *this;
  }
};
struct fo_inserter {
  template <class C>
  constexpr insert_iterator<C> operator ()(C &x,
                                           typename C::iterator i) const {
    return insert_iterator<C>(x, i);
  }
};
inline constexpr fo_inserter inserter{};

namespace inner {

template <class C, class X>
concept can_push_front = requires(C &c, X &&x) {
  c.push_front(forward<X>(x));
};
template <class C, class X>
concept can_push_back = requires(C &c, X &&x) {
  c.push_back(forward<X>(x));
};
template <class C, class X>
concept can_auto_insert = requires(C &c, X &&x) {
  c.insert(forward<X>(x));
};
template <class C, class X>
concept can_insert_at_end = requires(C &c, X &&x) {
  c.insert(c.end(), forward<X>(x));
};

template <class C, class X>
concept can_insert_after_before_begin = requires(C &c, X &&x) {
  c.insert_after(before_begin(c), forward<X>(x));
};
template <class C>
concept forward_list_like = requires {
  typename C::value_type;
  requires can_insert_after_before_begin<C, const typename C::value_type &>;
};

}
template <class C>
class to_back_iterator {
  C *c = nullptr;

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  using container_type = C;

  to_back_iterator() = default;
  ~to_back_iterator() = default;
  to_back_iterator(const to_back_iterator &) = default;
  to_back_iterator &operator =(const to_back_iterator &) = default;
  to_back_iterator(to_back_iterator &&) = default;
  to_back_iterator &operator =(to_back_iterator &&) = default;
  friend constexpr void swap(to_back_iterator &x,
                             to_back_iterator &y) noexcept {
    adl_swap(x.c, y.c);
  }

  constexpr explicit to_back_iterator(C &x) : c(addressof(x)) {}
  constexpr explicit to_back_iterator(C &&x) : c(addressof(x)) {}

  template <class X>
  constexpr to_back_iterator &operator =(X &&x) {
    if constexpr (inner::can_push_back<C, X>) {
      c->push_back(forward<X>(x));
    }
    else if constexpr (inner::can_auto_insert<C, X>) {
      c->insert(forward<X>(x));
    }
    else {
      c->insert(c->end(), forward<X>(x));
    }
    return *this;
  }

  constexpr to_back_iterator &operator *() {
    return *this;
  }
  constexpr to_back_iterator &operator ++() {
    return *this;
  }
  constexpr to_back_iterator operator ++(int) {
    return *this;
  }

  constexpr C base() && {
    return move(*c);
  }
};
struct fo_to_back {
  template <class C>
  constexpr to_back_iterator<remove_reference_t<C>>
  operator ()(C &&c) const {
    return to_back_iterator<remove_reference_t<C>>(forward<C>(c));
  }
};
inline constexpr fo_to_back to_back{};
template <class C>
class to_front_iterator {
  using iter_t = typename C::iterator;

  C *c = nullptr;
  iter_t i = iter_t{};

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  using container_type = C;

  to_front_iterator() = default;
  ~to_front_iterator() = default;
  to_front_iterator(const to_front_iterator &) = default;
  to_front_iterator &operator =(const to_front_iterator &) = default;
  to_front_iterator(to_front_iterator &&) = default;
  to_front_iterator &operator =(to_front_iterator &&) = default;
  friend constexpr void swap(to_front_iterator &x, to_front_iterator &y)
    noexcept(is_nothrow_swappable_v<iter_t>) {
    adl_swap(x.c, y.c);
    adl_swap(x.i, y.i);
  }

  constexpr explicit to_front_iterator(C &x) : c(addressof(x)) {
    if constexpr (inner::forward_list_like<C>) {
      i = x.before_begin();
    }
    else {
      i = x.begin();
    }
  }
  constexpr explicit to_front_iterator(C &&x) : to_front_iterator(x) {}

  template <class X>
  constexpr to_front_iterator &operator =(X &&x) {
    i = c->insert(i, forward<X>(x));
    ++i;
    return *this;
  }
  template <class X>
  constexpr to_front_iterator &operator =(X &&x)
    requires inner::forward_list_like<C> {
    i = c->insert_after(i, forward<X>(x));
    return *this;
  }

  constexpr to_front_iterator &operator *() {
    return *this;
  }
  constexpr to_front_iterator &operator ++() {
    return *this;
  }
  constexpr to_front_iterator operator ++(int) {
    return *this;
  }

  constexpr C base() && {
    return move(*c);
  }
};
struct fo_to_front {
  template <class C>
  constexpr to_front_iterator<remove_reference_t<C>>
  operator ()(C &&c) const {
    return to_front_iterator<remove_reference_t<C>>(forward<C>(c));
  }
};
inline constexpr fo_to_front to_front{};

template <class UF>
class any_output_iterator {
  template <class>
  friend class any_output_iterator;

  using this_t = any_output_iterator;

  semiregular_function<UF> fn = semiregular_function<UF>{};

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  any_output_iterator() = default;
  ~any_output_iterator() = default;
  any_output_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  any_output_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<semiregular_function<UF>>) {
    adl_swap(x.fn, y.fn);
  }

  constexpr explicit any_output_iterator(UF x) : fn(x) {}

  constexpr this_t &operator *() {
    return *this;
  }
  constexpr this_t &operator ++() {
    return *this;
  }
  constexpr this_t &operator ++(int) {
    return *this;
  }
  template <class T>
  constexpr this_t &operator =(T &&x)
    requires (!convertible_to<T &&, const this_t &>
              && !convertible_to<T &&, this_t &&>) {
    fn(forward<T>(x));
    return *this;
  }
};
struct fo_output_itr {
  template <class F>
  constexpr auto operator ()(F f) const {
    return any_output_iterator<F>(f);
  }
};
inline constexpr fo_output_itr output_itr{};

}

// move_iterator
namespace re {

template <class II>
class move_iterator {
  II i = II{};

  template <class>
  friend class move_iterator;

public:
  using iterator_type = II;

  using value_type = itr_vt<II>;
  using reference = itr_rref<II>;
  using pointer = II;
  using difference_type = itr_dft<II>;
  using iterator_category = itr_ctg<II>;

  move_iterator() = default;
  ~move_iterator() = default;
  move_iterator(const move_iterator &) = default;
  move_iterator &operator =(const move_iterator &) = default;
  move_iterator(move_iterator &&) = default;
  move_iterator &operator =(move_iterator &&) = default;
  friend constexpr void swap(move_iterator &x, move_iterator &y)
    noexcept(is_nothrow_swappable_v<II>) {
    adl_swap(x.i, y.i);
  }

  constexpr explicit move_iterator(II ii) : i(ii) {}

  template <class II2>
  constexpr move_iterator(const move_iterator<II2> &x)
    requires (!is_same_v<II, II2> && convertible_to<const II2 &, II>)
    : i(x.i) {}
  template <class II2>
  constexpr move_iterator &operator =(const move_iterator<II2> &x)
    requires (!is_same_v<II, II2> && convertible_to<const II2 &, II>
              && assignable_from<II &, const II2 &>) {
    i = x.i;
    return *this;
  }

  constexpr decltype(auto) operator *() const {
    return *i;
  }
  constexpr reference operator *() const
    requires (!is_same_v<itr_rref<II>, itr_ref<II>>) {
    return static_cast<reference>(*i);
  }
  constexpr pointer operator ->() const
    requires can_apply_to_address<const II &> {
    return i;
  }
  constexpr move_iterator &operator ++() {
    ++i;
    return *this;
  }
  value_type *operator ++(int);
  constexpr auto operator ++(int) requires is_fitr<II> {
    return iter_post_increment(*this);
  }

  constexpr move_iterator &operator --() requires is_bitr<II> {
    --i;
    return *this;
  }
  constexpr move_iterator operator --(int) requires is_bitr<II> {
    return iter_post_decrement(*this);
  }

  constexpr move_iterator &operator +=(difference_type n)
    requires is_ritr<II> {
    i += n;
    return *this;
  }
  constexpr move_iterator &operator -=(difference_type n)
    requires is_ritr<II> {
    i -= n;
    return *this;
  }
  constexpr move_iterator operator +(difference_type n) const
    requires is_ritr<II> {
    return move_iterator(i + n);
  }
  constexpr move_iterator operator -(difference_type n) const
    requires is_ritr<II> {
    return move_iterator(i - n);
  }
  constexpr reference operator [](difference_type n) const
    requires is_ritr<II> {
    return *(*this + n);
  }

  constexpr II base() const {
    return i;
  }
};
template <class II1, class II2>
constexpr auto operator ==(const move_iterator<II1> &x,
                           const move_iterator<II2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class II1, class II2>
constexpr synth_3way_result<II1, II2>
operator <=>(const move_iterator<II1> &x, const move_iterator<II2> &y) {
  return synth_3way(x.base(), y.base());
}
template <class II1, class II2>
constexpr auto operator -(const move_iterator<II1> &x,
                          const move_iterator<II2> &y)
  ->decltype(x.base() - y.base())
  requires itr_is_counted<II1> && itr_is_counted<II2> {
  return x.base() - y.base();
}
template <class II1>
constexpr move_iterator<II1> operator +
(itr_dft<II1> n, const move_iterator<II1> &x) requires is_ritr<II1> {
  return x + n;
}

struct fo_make_move_iterator {
  template <class II>
  constexpr move_iterator<II> operator ()(II i) const {
    return move_iterator<II>(i);
  }
};
inline constexpr fo_make_move_iterator make_move_iterator{};
inline constexpr fo_make_move_iterator move_itr{};

}

// counted_iterator
namespace re {

template <class I>
class counted_iterator {
  I iter = I{};
  itr_dft<I> len = itr_dft<I>{};

  template <class>
  friend class counted_iterator;

public:
  using iterator_type = I;

  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category = itr_ctg<I>;

  counted_iterator() = default;
  ~counted_iterator() = default;
  counted_iterator(const counted_iterator &) = default;
  counted_iterator &operator =(const counted_iterator &) = default;
  counted_iterator(counted_iterator &&) = default;
  counted_iterator &operator =(counted_iterator &&) = default;
  friend constexpr void swap(counted_iterator &x, counted_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.iter, y.iter);
    adl_swap(x.len, y.len);
  }

  constexpr counted_iterator(I i, itr_dft<I> n) : iter(i), len(n) {}

  template <class II>
  constexpr counted_iterator(const counted_iterator<II> &x)
    requires (!same_as<I, II> && convertible_to<const II &, I>)
    : iter(x.iter), len(x.len) {}
  template <class II>
  constexpr counted_iterator &operator =(const counted_iterator<II> &x)
    requires (!same_as<I, II> && convertible_to<const II &, I>
              && assignable_from<I &, const II &>) {
    iter = x.iter;
    len = x.len;
    return *this;
  }

  constexpr I base() const {
    return iter;
  }
  constexpr itr_dft<I> count() const noexcept {
    return len;
  }

  constexpr decltype(auto) operator *() const {
    return *iter;
  }
  constexpr pointer operator ->() const
    requires can_apply_to_address<I> {
    return iter;
  }
  constexpr counted_iterator &operator ++() {
    ++iter;
    --len;
    return *this;
  }
  value_type *operator ++(int);
  constexpr counted_iterator operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }

  constexpr counted_iterator &operator --() requires is_bitr<I> {
    --iter;
    ++len;
    return *this;
  }
  constexpr counted_iterator operator --(int) requires is_bitr<I> {
    return iter_post_decrement(*this);
  }

  constexpr counted_iterator &operator +=(difference_type n)
    requires is_ritr<I> {
    iter += n;
    len -= n;
    return *this;
  }
  constexpr counted_iterator &operator -=(difference_type n)
    requires is_ritr<I> {
    iter -= n;
    len += n;
    return *this;
  }
  constexpr counted_iterator operator +(difference_type n) const
    requires is_ritr<I> {
    return counted_iterator(iter + n, len - n);
  }
  constexpr counted_iterator operator -(difference_type n) const
    requires is_ritr<I> {
    return counted_iterator(iter - n, len + n);
  }
  constexpr decltype(auto) operator [](difference_type n) const
    requires is_ritr<I> {
    return iter[n];
  }
};
template <class I1, class I2>
constexpr auto operator ==(const counted_iterator<I1> &x,
                           const counted_iterator<I2> &y)
  ->decltype(x.count() == y.count())
  requires can_apply_equal_to<const I1 &, const I2 &> {
  return x.count() == y.count();
}
template <class I1, class I2, class = synth_3way_result<I1, I2>>
constexpr strong_ordering
operator <=>(const counted_iterator<I1> &x, const counted_iterator<I2> &y) {
  return y.count() <=> x.count();
}
template <class I1, class I2>
constexpr itr_dft<I1> operator -(const counted_iterator<I1> &x,
                                 const counted_iterator<I2> &y)
  requires can_apply_equal_to<const I1 &, const I2 &> {
  return y.count() - x.count();
}
template <class I1>
constexpr counted_iterator<I1>
operator +(itr_dft<I1> n, const counted_iterator<I1> &i)
  requires is_ritr<I1> {
  return i + n;
}

struct fo_make_counted_iterator {
  template <class I>
  constexpr counted_iterator<I> operator ()(I i, itr_dft<I> n) const
    requires is_itr<I> {
    return counted_iterator<I>(i, n);
  }
};
inline constexpr fo_make_counted_iterator make_counted_iterator{};
inline constexpr fo_make_counted_iterator counted_itr{};

}

// iterator_wrapper
namespace re {

template <class I>
class iterator_wrapper {
  I i = I{};

public:
  using iterator_type = I;

  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category = itr_ctg<I>;

  iterator_wrapper() = default;
  ~iterator_wrapper() = default;
  iterator_wrapper(const iterator_wrapper &) = default;
  iterator_wrapper &operator =(const iterator_wrapper &) = default;
  iterator_wrapper(iterator_wrapper &&) = default;
  iterator_wrapper &operator =(iterator_wrapper &&) = default;
  friend constexpr void swap(iterator_wrapper &x, iterator_wrapper &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
  }

  constexpr explicit iterator_wrapper(I ii) : i(ii) {}

  template <class II>
  constexpr iterator_wrapper(const iterator_wrapper<II> &x)
    requires (!same_as<I, II> && convertible_to<const II &, I>)
    : i(x.base()) {}
  template <class II>
  constexpr iterator_wrapper &operator =(const iterator_wrapper<II> &x)
    requires (!same_as<I, II> && convertible_to<const II &, I>
              && assignable_from<I &, const II &>) {
    i = x.base();
    return *this;
  }

  constexpr I base() const {
    return i;
  }

  constexpr decltype(auto) operator *() const {
    return *i;
  }
  constexpr pointer operator ->() const requires can_apply_to_address<I> {
    return i;
  }
  constexpr iterator_wrapper &operator ++() {
    ++i;
    return *this;
  }
  value_type *operator ++(int);
  constexpr iterator_wrapper operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }

  constexpr iterator_wrapper &operator --() requires is_bitr<I> {
    --i;
    return *this;
  }
  constexpr iterator_wrapper operator --(int) requires is_bitr<I> {
    return iter_post_decrement(*this);
  }

  constexpr iterator_wrapper &operator +=(difference_type n)
    requires is_ritr<I> {
    i += n;
    return *this;
  }
  constexpr iterator_wrapper &operator -=(difference_type n)
    requires is_ritr<I> {
    i -= n;
    return *this;
  }
  constexpr iterator_wrapper operator +(difference_type n) const
    requires is_ritr<I> {
    return iterator_wrapper(i + n);
  }
  constexpr iterator_wrapper operator -(difference_type n) const
    requires is_ritr<I> {
    return iterator_wrapper(i - n);
  }
  constexpr decltype(auto) operator [](difference_type n) const
    requires is_ritr<I> {
    return i[n];
  }
};
template <class I1, class I2>
constexpr auto operator ==(const iterator_wrapper<I1> &x,
                           const iterator_wrapper<I2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class I1, class I2>
constexpr synth_3way_result<I1, I2>
operator <=>(const iterator_wrapper<I1> &x,
             const iterator_wrapper<I2> &y) {
  return synth_3way(x.base(), y.base());
}
template <class I1, class I2>
constexpr auto operator -(const iterator_wrapper<I1> &x,
                          const iterator_wrapper<I2> &y)
  ->decltype(x.base() - y.base())
  requires (itr_is_counted<I1> && itr_is_counted<I2>) {
  return x.base() - y.base();
}
template <class I1>
constexpr iterator_wrapper<I1>
operator +(itr_dft<I1> n, const iterator_wrapper<I1> &i)
  requires is_ritr<I1> {
  return i + n;
}

struct fo_wrap_itr {
  template <class I>
  constexpr iterator_wrapper<I> operator ()(I i) const {
    return iterator_wrapper<I>(i);
  }
};
inline constexpr fo_wrap_itr wrap_itr{};

}

// forward declarations
namespace re {

template <class>
class iter_pair;

template <class>
class range_wrapper;

template <class, class>
class degraded_range;
template <class>
class move_range;
template <class>
class reverse_range;

template <class>
class iter_n_iterator;
template <class>
class iterator_iterator;
template <class>
class iterator_range;
template <class, class>
class bind_range;

struct fo_rng {
  // rng(it1, it2)
  template <class A, class B>
  constexpr iter_pair<common_iter_t<A, B>>
  operator ()(A a, B b) const requires (!is_integral_v<A> && !is_integral_v<B>);

  // rng(r)
  template <class R>
  constexpr iter_pair<rng_itr<R>>
  operator ()(R &&r) const requires is_rng<R> && is_rng_ref<R>;

  // rng(it1, n)
  template <class A, class B>
  constexpr iter_pair<A> operator ()(A, B) const
    requires (!is_integral_v<A> && is_integral_v<B> && is_ritr<A>);
  template <class A, class B>
  constexpr iter_pair<iter_n_iterator<A>> operator ()(A, B) const
    requires (!is_integral_v<A> && is_integral_v<B> && !is_ritr<A>);

  // rng(n, x)
  template <class A, class B>
  constexpr n_value<B> operator ()(A, B) const requires is_integral_v<A>;
  template <class A, class B>
  constexpr n_value<add_const_t<B> &>
  operator ()(A, reference_wrapper<B>) const requires is_integral_v<A>;
};
inline constexpr fo_rng rng{};

struct fo_wrap_rng;

template <class>
struct fo_degraded_rng;
struct fo_degraded_iitr_rng;
struct fo_degraded_fitr_rng;
struct fo_degraded_bitr_rng;
struct fo_degraded_ritr_rng;

struct fo_move_rng {
  // move_rng(r)
  // move_rng(i1, i2)
  // move_rng(i1, n)
  template <class R>
  constexpr auto operator ()(R &&) const requires is_rng<R>;
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A>;
};
inline constexpr fo_move_rng move_rng{};

struct fo_rrng {
  // rrng(r)
  // rrng(i1, i2)
  // rrng(i1, n)
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_brng<R>;
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A>;
};
inline constexpr fo_rrng rrng{};

struct fo_irng {
  // irng(n1, n2)
  template <class A, class B>
  constexpr iter_pair<iterator_iterator<common_type_t<A, B>>>
  operator ()(A, B) const
    requires (is_integral_v<A> && is_integral_v<B>);
  template <class A, class B>
  constexpr iter_pair<iterator_iterator<common_iter_t<A, B>>>
  operator ()(A, B) const
    requires (!is_integral_v<A> && !is_integral_v<B>);

  // irng(r)
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R>;

  // irng(it1, n)
  template <class A, class B>
  constexpr auto operator ()(A, B) const
    requires (!is_integral_v<A> && is_integral_v<B>);
};
inline constexpr fo_irng irng{};
struct fo_iitr {
  template <class I>
  constexpr iterator_iterator<I> operator ()(I) const;
};
inline constexpr fo_iitr iitr{};

}

// array
namespace re {

template <class T, size_t N>
class array;
template <class T, class...S>
array(T, S...)->array<T, 1 + sizeof...(S)>;

template <class T, size_t N>
class array {
public:
  T a[N];

  using pointer = T *;
  using const_pointer = const T *;

  // container

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;

  using iterator = T *;
  using const_iterator = const T *;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  friend constexpr void swap(array &x, array &y)
    noexcept(is_nothrow_swappable_v<value_type>)
    requires is_swappable_v<value_type> {
    auto i = x.begin();
    for (auto ii = y.begin(); ii != y.end(); ++ii) {
      adl_swap(*i, *ii);
      ++i;
    }
  }

  constexpr iterator begin() noexcept {
    return a;
  }
  constexpr iterator end() noexcept {
    return begin() + N;
  }
  constexpr const_iterator begin() const noexcept {
    return a;
  }
  constexpr const_iterator end() const noexcept {
    return begin() + N;
  }
  constexpr const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr const_iterator cend() const noexcept {
    return end();
  }

  constexpr bool empty() const noexcept {
    return false;
  }
  constexpr size_type size() const noexcept {
    return N;
  }
  constexpr size_type max_size() const noexcept {
    return N;
  }

  // optional container operations

  // reversible container

  using reverse_iterator = re::reverse_iterator<iterator>;
  using const_reverse_iterator = re::reverse_iterator<const_iterator>;

  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return rend();
  }

  // sequence container, optional sequence container operations

  constexpr reference front() {
    return a[0];
  }
  constexpr reference back() {
    return a[N - 1];
  }
  constexpr const_reference front() const {
    return a[0];
  }
  constexpr const_reference back() const {
    return a[N - 1];
  }

  constexpr reference operator [](size_type i) {
    return a[i];
  }
  constexpr const_reference operator [](size_type i) const {
    return a[i];
  }
  constexpr reference at(size_type i) {
    return (i < N)
      ? a[i]
      : ([]{throw_or_abort<out_of_range>("array index is out of range\n");}(),
         a[i]);
  }
  constexpr const_reference at(size_type i) const {
    return (i < N)
      ? a[i]
      : ([]{throw_or_abort<out_of_range>("array index is out of range\n");}(),
         a[i]);
  }

  // specialized operations

  constexpr T *data() noexcept {
    return a;
  }
  constexpr const T *data() const noexcept {
    return a;
  }

  constexpr void fill(const T &x) {
    for (auto i = begin(); i != end(); ++i)
      *i = x;
  }
};
template <class T>
class array<T, 0> {
public:
  using pointer = T *;
  using const_pointer = const T *;

  // container

  using value_type = T;
  using reference = T &;
  using const_reference = const T &;

  using iterator = T *;
  using const_iterator = const T *;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  friend constexpr void swap(array &, array &) noexcept {}

  constexpr iterator begin() noexcept {
    return nullptr;
  }
  constexpr iterator end() noexcept {
    return begin();
  }
  constexpr const_iterator begin() const noexcept {
    return nullptr;
  }
  constexpr const_iterator end() const noexcept {
    return begin();
  }
  constexpr const_iterator cbegin() const noexcept {
    return begin();
  }
  constexpr const_iterator cend() const noexcept {
    return end();
  }

  constexpr bool empty() const noexcept {
    return true;
  }
  constexpr size_type size() const noexcept {
    return 0;
  }
  constexpr size_type max_size() const noexcept {
    return 0;
  }

  // optional container operations

  // reversible container

  using reverse_iterator = re::reverse_iterator<iterator>;
  using const_reverse_iterator = re::reverse_iterator<const_iterator>;

  constexpr reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  constexpr reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return rend();
  }

  // sequence container, optional sequence container operations

  constexpr reference front() {
    return begin()[0];
  }
  constexpr reference back() {
    return begin()[0 - 1];
  }
  constexpr const_reference front() const {
    return begin()[0];
  }
  constexpr const_reference back() const {
    return begin()[0 - 1];
  }

  constexpr reference operator [](size_type i) {
    return begin()[i];
  }
  constexpr const_reference operator [](size_type i) const {
    return begin()[i];
  }
  constexpr reference at(size_type i) {
    return (i < 0)
      ? begin()[i]
      : ([]{throw_or_abort<out_of_range>("array index is out of range\n");}(),
         begin()[i]);
  }
  constexpr const_reference at(size_type i) const {
    return (i < 0)
      ? begin()[i]
      : ([]{throw_or_abort<out_of_range>("array index is out of range\n");}(),
         begin()[i]);
  }

  // specialized operations

  constexpr T *data() noexcept {
    return nullptr;
  }
  constexpr const T *data() const noexcept {
    return nullptr;
  }

  constexpr void fill(const T &) noexcept {}
};
template <class T, size_t N>
constexpr auto operator ==(const array<T, N> &x, const array<T, N> &y)
  ->decltype(x.front() == y.front()) {
  auto ii = y.begin();
  for (auto i = x.begin(); i != x.end(); ++i) {
    if (!(*i == *ii))
      return false;
    ++ii;
  }
  return true;
}
template <class T, size_t N>
constexpr synth_3way_result<T>
operator <=>(const array<T, N> &x, const array<T, N> &y) {
  auto ii = y.begin();
  for (auto i = x.begin(); i != x.end(); ++i) {
    if (auto tmp = synth_3way(*i, *ii); tmp != 0)
      return tmp;
    ++ii;
  }
  return strong_eq;
}

template <size_t I, class T, size_t N>
constexpr T &get(array<T, N> &a) noexcept {
  return a[I];
}
template <size_t I, class T, size_t N, class...U>
constexpr const T &get(const array<T, N> &a) noexcept {
  return a[I];
}
template <size_t I, class T, size_t N, class...U>
constexpr T &&get(array<T, N> &&a) noexcept {
  return static_cast<T &&>(a[I]);
}
template <size_t I, class T, size_t N, class...U>
constexpr const T &&get(const array<T, N> &&a) noexcept {
  return static_cast<const T &&>(a[I]);
}

namespace inner::fns {

template <class T, size_t N, size_t...IDS>
constexpr array<remove_cv_t<T>, N>
to_array_impl(T (&a)[N], index_sequence<IDS...>) {
  return array<remove_cv_t<T>, N>{a[IDS]...};
}
template <class T, size_t N, size_t...IDS>
constexpr array<remove_cv_t<T>, N>
to_array_impl2(T (&a)[N], index_sequence<IDS...>) {
  return array<remove_cv_t<T>, N>{move(a[IDS])...};
}

}
struct fo_to_array {
  template <class T, size_t N>
  constexpr array<remove_cv_t<T>, N> operator ()(T (&a)[N]) const {
    return inner::fns::to_array_impl(a, make_index_sequence<N>{});
  }
  template <class T, size_t N>
  constexpr array<remove_cv_t<T>, N> operator ()(T (&&a)[N]) const {
    return inner::fns::to_array_impl2(a, make_index_sequence<N>{});
  }
};
inline constexpr fo_to_array to_array{};

struct fo_seq {
  template <class X, class...S>
  constexpr array<decay_t<X &&>, 1 + sizeof...(S)>
  operator ()(X &&x, S &&...s) const {
    using t = decay_t<X &&>;
    return {t(forward<X>(x)), t(forward<S>(s))...};
  }
};
inline constexpr fo_seq seq{};

}

// iter_pair
namespace re {

template <class IT>
class iter_pair;
template <size_t I, class IT>
struct tuple_element<I, iter_pair<IT>> : tuple_element<I, pair<IT, IT>> {};
template <class IT>
struct tuple_size<iter_pair<IT>> : tuple_size<pair<IT, IT>> {};
template <class IT>
class iter_pair : public pair<IT, IT> {
  using base_t = pair<IT, IT>;

  using pair<IT, IT>::swap;

public:
  iter_pair() = default;
  ~iter_pair() = default;
  iter_pair(const iter_pair &) = default;
  iter_pair &operator =(const iter_pair &) = default;
  iter_pair(iter_pair &&) = default;
  iter_pair &operator =(iter_pair &&) = default;
  friend constexpr void swap(iter_pair &x, iter_pair &y)
    noexcept(is_nothrow_swappable_v<IT>) {
    adl_swap(static_cast<base_t &>(x), static_cast<base_t &>(y));
  }

  template <class X, class Y>
  constexpr iter_pair(X x, Y y)
    requires is_constructible_v<IT, X &> && is_constructible_v<IT, Y &>
    : base_t(x, y)  {}

  template <class T>
  constexpr explicit(!is_convertible_v<const T &, IT>)
  iter_pair(const iter_pair<T> &x)
    requires (!is_same_v<T, IT> && is_constructible_v<IT, const T &>)
    : base_t(static_cast<const pair<T, T> &>(x)) {}
  template <class T>
  constexpr iter_pair &operator =(const iter_pair<T> &x)
    requires (!is_same_v<T, IT> && is_assignable_v<IT &, const T &>) {
    base_t::operator =(static_cast<const pair<T, T> &>(x));
    return *this;
  }

  constexpr IT begin() const {
    return base_t::first;
  }
  constexpr IT end() const {
    return base_t::second;
  }

  constexpr bool empty() const {
    return begin() == end();
  }

  constexpr make_unsigned_t<itr_dft<IT>> size() const
    requires itr_is_counted<IT> {
    return end() - begin();
  }
  constexpr size_t size() const requires is_null_pointer_v<IT> {
    return 0;
  }
};
iter_pair()->iter_pair<nullptr_t>;
template <class X>
iter_pair(X, X)->iter_pair<X>;
template <class X, class Y>
constexpr auto operator ==(const iter_pair<X> &x,
                           const iter_pair<Y> &y)
  ->decltype(static_cast<const pair<X, X> &>(x)
             == static_cast<const pair<Y, Y> &>(y)) {
  return static_cast<const pair<X, X> &>(x)
    == static_cast<const pair<Y, Y> &>(y);
}

// rng(it1, it2)
template <class A, class B>
constexpr iter_pair<common_iter_t<A, B>>
fo_rng::operator ()(A a, B b) const
  requires (!is_integral_v<A> && !is_integral_v<B>) {
  return {a, b};
}

// rng(r)
template <class R>
constexpr iter_pair<rng_itr<R>> fo_rng::operator ()(R &&r) const
  requires is_rng<R> && is_rng_ref<R> {
  return iter_pair<rng_itr<R>>(begin(r), end(r));
}

}

// composite iterator/range
namespace re {

template <class T, class DEREF, class PP, class EQ>
class composite_input_iterator {
  using this_t = composite_input_iterator;

  T x = T{};
  semiregular_function<DEREF> df = semiregular_function<DEREF>{};
  semiregular_function<PP> pp = semiregular_function<PP>{};
  copyable_wrapper<optional<EQ>> eq = copyable_wrapper<optional<EQ>>{};

public:
  using reference = decltype(declval<const DEREF &>()(declval<const T &>()));
  using value_type = remove_cvref_t<reference>;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using iterator_category = input_iterator_tag;

  composite_input_iterator() = default;
  ~composite_input_iterator() = default;
  composite_input_iterator(const this_t &) = default;
  composite_input_iterator &operator =(const this_t &) = default;
  composite_input_iterator(this_t &&) = default;
  composite_input_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &a, this_t &b)
    noexcept(is_nothrow_swappable_v<T>
             && is_nothrow_swappable_v<semiregular_function<DEREF>>
             && is_nothrow_swappable_v<semiregular_function<PP>>
             && is_nothrow_swappable_v<copyable_wrapper<optional<EQ>>>) {
    adl_swap(a.x, b.x);
    adl_swap(a.df, b.df);
    adl_swap(a.pp, b.pp);
    adl_swap(a.eq, b.eq);
  }

  constexpr composite_input_iterator(const T &a, DEREF b, PP c, EQ d)
    : x(a), df(b), pp(c), eq(d) {}

  constexpr void operator ->() const = delete;
  constexpr reference operator *() const {
    return df(x);
  }
  constexpr this_t &operator ++() {
    pp(x);
    return *this;
  }
  value_type *operator ++(int);
  constexpr bool operator ==(const this_t &y) const {
    return (*eq) ? (**eq)(x, y.x) : true;
  }

  constexpr T base() const {
    return x;
  }
};
template <class T, class DEREF, class PP, class EQ>
class composite_forward_iterator {
  using this_t = composite_forward_iterator;

  T x = T{};
  semiregular_function<DEREF> df = semiregular_function<DEREF>{};
  semiregular_function<PP> pp = semiregular_function<PP>{};
  copyable_wrapper<optional<EQ>> eq = copyable_wrapper<optional<EQ>>{};

public:
  using reference = decltype(declval<const DEREF &>()(declval<const T &>()));
  using value_type = remove_cvref_t<reference>;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using iterator_category = forward_iterator_tag;

  composite_forward_iterator() = default;
  ~composite_forward_iterator() = default;
  composite_forward_iterator(const this_t &) = default;
  composite_forward_iterator &operator =(const this_t &) = default;
  composite_forward_iterator(this_t &&) = default;
  composite_forward_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &a, this_t &b)
    noexcept(is_nothrow_swappable_v<T>
             && is_nothrow_swappable_v<semiregular_function<DEREF>>
             && is_nothrow_swappable_v<semiregular_function<PP>>
             && is_nothrow_swappable_v<copyable_wrapper<optional<EQ>>>) {
    adl_swap(a.x, b.x);
    adl_swap(a.df, b.df);
    adl_swap(a.pp, b.pp);
    adl_swap(a.eq, b.eq);
  }

  constexpr composite_forward_iterator(const T &a, DEREF b, PP c, EQ d)
    : x(a), df(b), pp(c), eq(d) {}

  constexpr void operator ->() const = delete;
  constexpr reference operator *() const {
    return df(x);
  }
  constexpr this_t &operator ++() {
    pp(x);
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }
  constexpr bool operator ==(const this_t &y) const {
    return *eq ? (**eq)(x, y.x) : true;
  }

  constexpr T base() const {
    return x;
  }
};
template <class T, class DEREF, class PP, class EQ, class MM>
class composite_bidirectional_iterator {
  using this_t = composite_bidirectional_iterator;

  T x = T{};
  semiregular_function<DEREF> df = semiregular_function<DEREF>{};
  semiregular_function<PP> pp = semiregular_function<PP>{};
  copyable_wrapper<optional<EQ>> eq = copyable_wrapper<optional<EQ>>{};
  semiregular_function<MM> mm = semiregular_function<MM>{};

public:
  using reference = decltype(declval<const DEREF &>()(declval<const T &>()));
  using value_type = remove_cvref_t<reference>;
  using difference_type = ptrdiff_t;
  using pointer = void;
  using iterator_category = bidirectional_iterator_tag;

  composite_bidirectional_iterator() = default;
  ~composite_bidirectional_iterator() = default;
  composite_bidirectional_iterator(const this_t &) = default;
  composite_bidirectional_iterator &operator =(const this_t &) = default;
  composite_bidirectional_iterator(this_t &&) = default;
  composite_bidirectional_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &a, this_t &b)
    noexcept(is_nothrow_swappable_v<T>
             && is_nothrow_swappable_v<semiregular_function<DEREF>>
             && is_nothrow_swappable_v<semiregular_function<PP>>
             && is_nothrow_swappable_v<copyable_wrapper<optional<EQ>>>
             && is_nothrow_swappable_v<semiregular_function<MM>>) {
    adl_swap(a.x, b.x);
    adl_swap(a.df, b.df);
    adl_swap(a.pp, b.pp);
    adl_swap(a.eq, b.eq);
    adl_swap(a.mm, b.mm);
  }

  constexpr composite_bidirectional_iterator(const T &a,
                                             DEREF b, PP c, EQ d, MM e)
    : x(a), df(b), pp(c), eq(d), mm(e) {}

  constexpr void operator ->() const = delete;
  constexpr reference operator *() const {
    return df(x);
  }
  constexpr this_t &operator ++() {
    pp(x);
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }
  constexpr bool operator ==(const this_t &y) const {
    return (*eq).has_value() ? (**eq)(x, y.x) : true;
  }

  constexpr this_t &operator --() {
    mm(x);
    return *this;
  }
  constexpr this_t operator --(int) {
    return iter_post_decrement(*this);
  }

  constexpr T base() const {
    return x;
  }
};

struct fo_composite_irng {
  template <class X, class DEREF, class PP, class EQ>
  constexpr auto operator ()(X x, X y, DEREF df, PP pp, EQ eq) const {
    return rng(composite_input_iterator<X, DEREF, PP, EQ>(x, df, pp, eq),
               composite_input_iterator<X, DEREF, PP, EQ>(y, df, pp, eq));
  }
};
inline fo_composite_irng composite_irng{};
struct fo_composite_frng {
  template <class X, class DEREF, class PP, class EQ>
  constexpr auto operator ()(X x, X y, DEREF df, PP pp, EQ eq) const {
    return rng(composite_forward_iterator<X, DEREF, PP, EQ>(x, df, pp, eq),
               composite_forward_iterator<X, DEREF, PP, EQ>(y, df, pp, eq));
  }
};
inline fo_composite_frng composite_frng{};
struct fo_composite_brng {
  template <class X, class DEREF, class PP, class EQ, class MM>
  constexpr auto operator ()(X x, X y,
                             DEREF df, PP pp, EQ eq, MM mm) const {
    return rng(composite_bidirectional_iterator<X, DEREF, PP, EQ, MM>
               (x, df, pp, eq, mm),
               composite_bidirectional_iterator<X, DEREF, PP, EQ, MM>
               (y, df, pp, eq, mm));
  }
};
inline fo_composite_brng composite_brng{};

}

// range_wrapper
namespace re {

template <class R>
class range_wrapper : range_fns {
  friend struct fo_wrap_rng;

  R r = R{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  range_wrapper() = delete;
  range_wrapper() requires is_default_constructible_v<R> = default;
  ~range_wrapper() = default;
  range_wrapper(const range_wrapper &) = default;
  range_wrapper &operator =(const range_wrapper &) = default;
  range_wrapper(range_wrapper &&) = default;
  range_wrapper &operator =(range_wrapper &&) = default;
  friend constexpr void swap(range_wrapper &x, range_wrapper &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<range_wrapper>) {
    adl_swap(x.r, y.r);
  }

  constexpr explicit range_wrapper(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return begin(r);
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return begin(r);
  }
  constexpr auto end() {
    return end(r);
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return end(r);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<const R>) {
    return size(r);
  }
};
template <class R>
struct is_range_reference<range_wrapper<R>> : is_range_reference<R> {};

struct fo_wrap_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return range_wrapper<rng_forward_t<R>>(forward<R>(r));
  }
  template <class X, class Y>
  constexpr auto operator ()(X x, Y y) const {
    return operator ()(rng(x, y));
  }
};
inline constexpr fo_wrap_rng wrap_rng{};

}

// base_range
// inplace_base_range
namespace re {

template <class R>
class base_range : range_fns {
  friend struct fo_base_rng;

  R r = R{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  base_range() = delete;
  base_range() requires is_default_constructible_v<R> = default;
  ~base_range() = default;
  base_range(const base_range &) = default;
  base_range &operator =(const base_range &) = default;
  base_range(base_range &&) = default;
  base_range &operator =(base_range &&) = default;
  friend constexpr void swap(base_range &x, base_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<base_range>) {
    adl_swap(x.r, y.r);
  }

  constexpr explicit base_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return begin(r).base();
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return begin(r).base();
  }
  constexpr auto end() {
    return end(r).base();
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return end(r).base();
  }

  constexpr bool empty() {
    return begin() == end();
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return begin() == end();
  }

  constexpr auto size()
    requires itr_is_counted<decltype(declval<rng_itr<R>>().base())> {
    return to_unsigned(end() - begin());
  }
  constexpr auto size() const
    requires (is_rng<add_const_t<R>>
              && itr_is_counted<decltype(declval<rng_itr<const R>>()
                                         .base())>) {
    return to_unsigned(end() - begin());
  }
};
template <class R>
struct is_range_reference<base_range<R>> : is_range_reference<R> {};
struct fo_base_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return base_range<rng_forward_t<R>>(forward<R>(r));
  }
  template <class I, class T>
  constexpr auto operator ()(I i, T x) const requires is_itr<I> {
    return operator ()(rng(i, x));
  }
};
inline constexpr fo_base_rng base_rng{};

template <class R>
class inplace_base_range : range_fns {
  friend struct fo_inplace_base_rng;

  R r = R{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  inplace_base_range() = delete;
  inplace_base_range() requires is_default_constructible_v<R> = default;
  ~inplace_base_range() = default;
  inplace_base_range(const inplace_base_range &) = default;
  inplace_base_range &operator =(const inplace_base_range &) = default;
  inplace_base_range(inplace_base_range &&) = default;
  inplace_base_range &operator =(inplace_base_range &&) = default;
  friend constexpr void swap(inplace_base_range &x, inplace_base_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<inplace_base_range>) {
    adl_swap(x.r, y.r);
  }

  constexpr explicit inplace_base_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return begin(r).base();
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return begin(r).base();
  }
  constexpr auto end() {
    return end(r).base();
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return end(r).base();
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return size(r);
  }
};
template <class R>
struct is_range_reference<inplace_base_range<R>> : is_range_reference<R> {};
struct fo_inplace_base_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return inplace_base_range<rng_forward_t<R>>(forward<R>(r));
  }
  template <class I, class T>
  constexpr auto operator ()(I i, T x) const requires is_itr<I> {
    return operator ()(rng(i, x));
  }
};
inline constexpr fo_inplace_base_rng inplace_base_rng{};

}

// empty_range
namespace re {

template <class T>
requires is_object_v<T>
class empty_range {
public:
  static constexpr T *begin() {
    return nullptr;
  }
  static constexpr T *end() {
    return nullptr;
  }
  static constexpr size_t size() {
    return 0;
  }
  static constexpr bool empty() {
    return true;
  }
};
template <class T>
struct is_range_reference<empty_range<T>> : true_type {};

template <class T>
struct fo_empty_rng {
  constexpr auto operator ()() const {
    return empty_range<T>{};
  }
};
template <class T>
inline constexpr fo_empty_rng<T> empty_rng{};

}

// single_range
namespace re {

template <class T>
class single_range {
  T v = T{};

public:
  single_range() = delete;
  single_range() requires is_default_constructible_v<T> = default;
  ~single_range() = default;
  single_range(const single_range &) = default;
  single_range &operator =(const single_range &) = default;
  single_range(single_range &&) = default;
  single_range &operator =(single_range &&) = default;
  friend constexpr void swap(single_range &x, single_range &y)
    noexcept(is_nothrow_swappable_v<T>)
    requires is_swappable_v<T> && default_swappable<single_range> {
    adl_swap(x.v, y.v);
  }

  constexpr explicit single_range(T x) : v(forward<T>(x)) {}
  template <class...S>
  constexpr explicit single_range(in_place_t, S &&...s)
    requires is_constructible_v<T, S &&...> : v(forward<S>(s)...) {}

  constexpr auto begin() noexcept {
    return addressof(v);
  }
  constexpr auto begin() const noexcept {
    return addressof(v);
  }
  constexpr auto end() noexcept {
    return addressof(v) + 1;
  }
  constexpr auto end() const noexcept {
    return addressof(v) + 1;
  }
  static constexpr size_t size() noexcept {
    return 1;
  }
  static constexpr bool empty() noexcept {
    return false;
  }
};
struct fo_single_rng {
  template <class T>
  constexpr single_range<T> operator ()(T x) const {
    return single_range<T>(forward<T>(x));
  }
  template <class T>
  constexpr single_range<T &> operator ()(reference_wrapper<T> x) const {
    return single_range<T &>(x.get());
  }
};
inline constexpr fo_single_rng single_rng{};

}

// counted_range
namespace re {

template <class R>
class counted_range : range_fns {
  // constraint: sz == 0 || sz == size(r)
  R r = R{};
  rng_dft<R> sz = rng_dft<R>{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  void re_calc_size() {
    if constexpr (is_frng<R> || rng_is_sized<R>) {
      sz = 0;
      sz = size(r);
    }
    else {
      sz = 0;
    }
  }

public:
  counted_range() = delete;
  counted_range() requires is_default_constructible_v<R> = default;
  ~counted_range() = default;
  counted_range(const counted_range &) = default;
  counted_range &operator =(const counted_range &) = default;
  constexpr counted_range(counted_range &&) = delete;
  constexpr counted_range(counted_range &&x)
    requires is_move_constructible_v<R>
    : r(forward<R>(x.r)), sz(x.sz) {
    if constexpr (!is_rng_ref<R>) {
      x.re_calc_size();
    }
  }
  constexpr counted_range &operator =(counted_range &&) = delete;
  constexpr counted_range &operator =(counted_range &&x)
    requires (!is_rng_ref<R> && is_move_assignable_v<R>) {
    if (addressof(x) != this) {
      r = forward<R>(x.r);
      sz = x.sz;
      x.re_calc_size();
    }
    return *this;
  }
  friend constexpr void swap(counted_range &x, counted_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<counted_range>) {
    adl_swap(x.r, y.r);
    adl_swap(x.sz, y.sz);
  }

  constexpr explicit counted_range(R rr) : r(forward<R>(rr)), sz(ssize(r)) {}

  // requires n == size(rr)
  constexpr counted_range(R rr, rng_dft<R> n) : r(forward<R>(rr)), sz(n) {}

  constexpr auto begin() {
    return counted_itr((sz == 0 ? end(r) : begin(r)), sz);
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return counted_itr((sz == 0 ? end(r) : begin(r)), sz);
  }
  constexpr auto end() {
    return counted_itr(end(r), 0);
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return counted_itr(end(r), 0);
  }

  constexpr bool empty() const {
    return sz == 0;
  }

  constexpr rng_szt<R> size() const {
    return sz;
  }
};
template <class R>
struct is_range_reference<counted_range<R>> : is_range_reference<R> {};

struct fo_counted_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_frng<R> {
    return counted_range<rng_forward_t<R>>(forward<R>(r));
  }
  template <class R> // requires n == size(r)
  constexpr auto operator ()(R &&r, rng_dft<R> n) const requires is_rng<R> {
    return counted_range<rng_forward_t<R>>(forward<R>(r), n);
  }
};
inline constexpr fo_counted_rng counted_rng{};

struct fo_sized_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_frng<R> {
    return inplace_base_rng(counted_rng(forward<R>(r)));
  }
  template <class R>
  constexpr auto operator ()(R &&r, rng_dft<R> n) const requires is_rng<R> {
    return inplace_base_rng(counted_rng(forward<R>(r), n));
  }
};
inline constexpr fo_sized_rng sized_rng{};

}

// degraded_range
namespace re {

template <class R, class TAG>
class degraded_range : range_fns {
  template <class>
  friend struct fo_degraded_rng;

  R r = R{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  degraded_range() = delete;
  degraded_range() requires is_default_constructible_v<R> = default;
  ~degraded_range() = default;
  degraded_range(const degraded_range &) = default;
  degraded_range &operator =(const degraded_range &) = default;
  degraded_range(degraded_range &&) = default;
  degraded_range &operator =(degraded_range &&) = default;
  friend constexpr void swap(degraded_range &x, degraded_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<degraded_range>) {
    adl_swap(x.r, y.r);
  }

  constexpr explicit degraded_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return degraded_iterator<rng_itr<R>, TAG>(begin(r));
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return degraded_iterator<rng_itr<const R>, TAG>(begin(r));
  }
  constexpr auto end() {
    return degraded_iterator<rng_itr<R>, TAG>(end(r));
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return degraded_iterator<rng_itr<const R>, TAG>(end(r));
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return size(r);
  }
};
template <class R, class TAG>
struct is_range_reference<degraded_range<R, TAG>> : is_range_reference<R> {};
template <class R>
using degraded_input_range
  = degraded_range<rng_forward_t<R>, input_iterator_tag>;
template <class R>
using degraded_forward_range
  = degraded_range<rng_forward_t<R>, forward_iterator_tag>;
template <class R>
using degraded_bidirectional_range
  = degraded_range<rng_forward_t<R>, bidirectional_iterator_tag>;
template <class R>
using degraded_random_access_range
  = degraded_range<rng_forward_t<R>, random_access_iterator_tag>;

template <class TAG>
struct fo_degraded_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return degraded_range<rng_forward_t<R>, TAG>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A> {
    return operator ()(rng(a, b));
  }
};
template <class TAG>
inline constexpr fo_degraded_rng<TAG> degraded_rng{};
struct fo_degraded_irng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return degraded_rng<input_iterator_tag>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A> {
    return operator ()(rng(a, b));
  }
};
struct fo_degraded_frng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_frng<R> {
    return degraded_rng<forward_iterator_tag>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A> {
    return operator ()(rng(a, b));
  }
};
struct fo_degraded_brng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_brng<R> {
    return degraded_rng<bidirectional_iterator_tag>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A> {
    return operator ()(rng(a, b));
  }
};
struct fo_degraded_rrng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rrng<R> {
    return degraded_rng<random_access_iterator_tag>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const requires is_itr<A> {
    return operator ()(rng(a, b));
  }
};
inline constexpr fo_degraded_irng degraded_irng{};
inline constexpr fo_degraded_frng degraded_frng{};
inline constexpr fo_degraded_brng degraded_brng{};
inline constexpr fo_degraded_rrng degraded_rrng{};

}

// move_range
namespace re {

template <class R>
class move_range {
  friend struct fo_move_rng;

  R r = R{};

public:
  move_range() = delete;
  move_range() requires is_default_constructible_v<R> = default;
  ~move_range() = default;
  move_range(const move_range &) = default;
  move_range &operator =(const move_range &) = default;
  move_range(move_range &&) = default;
  move_range &operator =(move_range &&) = default;
  friend constexpr void swap(move_range &x, move_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires is_swappable_v<R> && default_swappable<move_range> {
    adl_swap(x.r, y.r);
  }

  constexpr explicit move_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return move_itr(re::begin(r));
  }
  constexpr auto end() {
    return move_itr(re::end(r));
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return move_itr(re::begin(r));
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return move_itr(re::end(r));
  }

  constexpr bool empty() {
    return re::empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return re::empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return re::size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return re::size(r);
  }
};
template <class R>
struct is_range_reference<move_range<R>> : is_range_reference<R> {};

template <class R>
constexpr auto fo_move_rng::operator ()(R &&r) const requires is_rng<R> {
  return move_range<rng_forward_t<R>>(forward<R>(r));
}
template <class A, class B>
constexpr auto fo_move_rng::operator ()(A a, B b) const requires is_itr<A> {
  return move_rng(rng(a, b));
}

}

// reverse_range
namespace re {

template <class R>
class reverse_range : range_fns {
  friend struct fo_rrng;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::rbegin;
  using range_fns::rend;
  using range_fns::size;
  using range_fns::empty;

  R r = R{};

public:
  reverse_range() = delete;
  reverse_range() requires is_default_constructible_v<R> = default;
  ~reverse_range() = default;
  reverse_range(const reverse_range &) = default;
  reverse_range &operator =(const reverse_range &) = default;
  reverse_range(reverse_range &&) = default;
  reverse_range &operator =(reverse_range &&) = default;
  friend constexpr void swap(reverse_range &x, reverse_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires is_swappable_v<R> && default_swappable<reverse_range> {
    adl_swap(x.r, y.r);
  }

  constexpr explicit reverse_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return rbegin(r);
  }
  constexpr auto end() {
    return rend(r);
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return rbegin(r);
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return rend(r);
  }

  constexpr bool empty() {
    return re::empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return re::empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return re::size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return re::size(r);
  }
};
template <class R>
struct is_range_reference<reverse_range<R>> : is_range_reference<R> {};

// rrng(r)
// rrng(a, b)
template <class R>
constexpr auto fo_rrng::operator ()(R &&r) const requires is_brng<R> {
  return reverse_range<rng_forward_t<R>>(forward<R>(r));
}
template <class A, class B>
constexpr auto fo_rrng::operator ()(A a, B b) const requires is_itr<A> {
  return rrng(rng(a, b));
}

}

// rng(i, n)
namespace re {

template <class I>
class iter_n_iterator {
  I iter = I{};
  itr_dft<I> n = itr_dft<I>{};

  template <class>
  friend class iter_n_iterator;

public:
  using iterator_type = I;

  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category = conditional_t
    <is_fitr<I>, forward_iterator_tag, itr_ctg<I>>;

  iter_n_iterator() = default;
  ~iter_n_iterator() = default;
  iter_n_iterator(const iter_n_iterator &) = default;
  iter_n_iterator &operator =(const iter_n_iterator &) = default;
  iter_n_iterator(iter_n_iterator &&) = default;
  iter_n_iterator &operator =(iter_n_iterator &&) = default;
  friend constexpr void swap(iter_n_iterator &x, iter_n_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.iter, y.iter);
    adl_swap(x.n, y.n);
  }

  constexpr iter_n_iterator(I i, itr_dft<I> n) : iter(i), n(n) {}

  template <class II>
  constexpr iter_n_iterator(const iter_n_iterator<II> &x)
    requires (!same_as<I, II>) && is_convertible_v<const II &, I>
    : iter(x.iter), n(x.n) {}

  constexpr I base() const {
    return iter;
  }
  constexpr auto nth() const {
    return n;
  }

  constexpr decltype(auto) operator *() const {
    return *iter;
  }
  constexpr pointer operator ->() const requires can_apply_to_address<I> {
    return iter;
  }

  constexpr iter_n_iterator &operator ++() {
    ++iter;
    ++n;
    return *this;
  }
  value_type *operator ++(int);
  constexpr iter_n_iterator operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }
};
template <class I1, class I2>
constexpr bool operator ==(const iter_n_iterator<I1> &x,
                           const iter_n_iterator<I2> &y)
  requires is_same_v<itr_dft<I1>, itr_dft<I2>> {
  return x.nth() == y.nth();
}
template <class I1, class I2>
constexpr itr_dft<I1> operator -(const iter_n_iterator<I1> &x,
                                 const iter_n_iterator<I2> &y)
  requires is_same_v<itr_dft<I1>, itr_dft<I2>> {
  return x.nth() - y.nth();
}

struct fo_iter_n_itr {
  template <class I>
  constexpr iter_n_iterator<I> operator ()(I i, itr_dft<I> n) const {
    return iter_n_iterator<I>(i, n);
  }
};
inline constexpr fo_iter_n_itr iter_n_itr{};

template <class A, class B>
constexpr iter_pair<A> fo_rng::operator ()(A a, B b) const
  requires (!is_integral_v<A> && is_integral_v<B> && is_ritr<A>) {
  return iter_pair<A>(a, a + b);
}
template <class A, class B>
constexpr iter_pair<iter_n_iterator<A>>
fo_rng::operator ()(A a, B b) const
  requires (!is_integral_v<A> && is_integral_v<B> && !is_ritr<A>) {
  using it = iter_n_iterator<A>;
  return iter_pair<it>(it(a, 0), it(a, b));
}

}

// rng(n, x)
namespace re {

template <class A, class B>
constexpr n_value<B>
fo_rng::operator ()(A a, B b) const requires is_integral_v<A> {
  return n_value<B>(to_unsigned(a), forward<B>(b));
}
template <class A, class B>
constexpr n_value<add_const_t<B> &>
fo_rng::operator ()(A a, reference_wrapper<B> b) const
  requires is_integral_v<A> {
  return n_value<add_const_t<B> &>(to_unsigned(a), b.get());
}

}

// iterator_range
namespace re {

namespace inner {

struct iitr_minus_f {
  template <class T>
  static type_tag<decltype(declval<const T &>() - declval<const T &>())>
  f(type_pack<T>);
};
template <class T>
using get_iitr_dft = return_type_of_f_or<iitr_minus_f, ptrdiff_t, T>;

template <class I, bool IS_ITER = is_itr<I>>
struct get_iitr_ctg
  : conditional<is_citr<I>, random_access_iterator_tag, itr_ctg<I>> {};
template <class I>
struct get_iitr_ctg<I, false> : type_tag<random_access_iterator_tag> {};

}
template <class I>
class iterator_iterator {
  template <class>
  friend class iterator_iterator;

  using this_t = iterator_iterator;

  I n = I{};

public:
  using value_type = I;
  using difference_type = make_signed_t<typename inner::get_iitr_dft<I>::type>;
  using pointer = void;
  using reference = I;
  using iterator_category = typename inner::get_iitr_ctg<I>::type;

  iterator_iterator() = default;
  ~iterator_iterator() = default;
  iterator_iterator(const this_t &) = default;
  iterator_iterator &operator =(const this_t &) = default;
  iterator_iterator(this_t &&) = default;
  iterator_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.n, y.n);
  }

  constexpr explicit iterator_iterator(I x) : n(x) {}
  constexpr I base() const {
    return n;
  }

  template <class II>
  constexpr iterator_iterator(const iterator_iterator<II> &x)
    requires (!is_same_v<I, II> && is_convertible_v<const II &, I>): n(x.n) {}

  constexpr reference operator *() const {
    return n;
  }
  pointer operator ->() const = delete;
  constexpr iterator_iterator &operator ++() {
    ++n;
    return *this;
  }
  value_type *operator ++(int);
  constexpr iterator_iterator operator ++(int)
    requires is_fitr_ctg<iterator_category> {
    auto tmp = *this;
    ++n;
    return tmp;
  }

  constexpr iterator_iterator &operator --() requires is_bitr<this_t> {
    --n;
    return *this;
  }
  constexpr iterator_iterator operator --(int) requires is_bitr<this_t> {
    auto tmp = *this;
    --n;
    return tmp;
  }

private:
  static constexpr auto get_real_dft(difference_type i) {
    if constexpr (unsigned_integral<I>) {
      return static_cast<I>(i);
    }
    else {
      return i;
    }
  }
public:
  constexpr reference operator [](difference_type x) const
    requires is_ritr<this_t> {
    return n + get_real_dft(x);
  }
  constexpr iterator_iterator &operator +=(difference_type x)
    requires is_ritr<this_t> {
    n += get_real_dft(x);
    return *this;
  }
  constexpr iterator_iterator &operator -=(difference_type x)
    requires is_ritr<this_t> {
    n -= get_real_dft(x);
    return *this;
  }
  constexpr iterator_iterator operator +(difference_type x) const
    requires is_ritr<this_t> {
    return copy(*this) += x;
  }
  constexpr iterator_iterator operator -(difference_type x) const
    requires is_ritr<this_t> {
    return copy(*this) -= x;
  }
};
template <class T>
constexpr auto operator ==(const iterator_iterator<T> &a,
                           const iterator_iterator<T> &b)
  ->decltype(a.base() == b.base()) {
  return a.base() == b.base();
}
template <class T>
constexpr iterator_iterator<T>
operator +(typename iterator_iterator<T>::difference_type n,
           const iterator_iterator<T> &i)
  requires is_ritr<iterator_iterator<T>> {
  return i + n;
}
template <class A, class B>
constexpr auto operator -(const iterator_iterator<A> &it,
                          const iterator_iterator<B> &it2)
  ->decltype(to_signed(it.base() - it2.base()))
  requires (!is_itr<A> || (itr_is_counted<A> && itr_is_counted<B>)) {
  return it.base() - it2.base();
}
template <class A, class B>
constexpr synth_3way_result<A, B>
operator <=>(const iterator_iterator<A> &a,
             const iterator_iterator<B> &b) {
  return synth_3way(a.base(), b.base());
}
template <class I>
constexpr iterator_iterator<I> fo_iitr::operator ()(I i) const {
  return iterator_iterator<I>(i);
}

template <class R>
class iterator_range : range_fns {
  R r = R{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  iterator_range() = default;
  ~iterator_range() = default;
  iterator_range(const iterator_range &) = default;
  iterator_range &operator =(const iterator_range &) = default;
  iterator_range(iterator_range &&) = default;
  iterator_range &operator =(iterator_range &&) = default;
  friend constexpr void swap(iterator_range &x, iterator_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires is_swappable_v<R> && default_swappable<iterator_range> {
    adl_swap(x.r, y.r);
  }

  constexpr explicit iterator_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    return iterator_iterator<decltype(begin(r))>(begin(r));
  }
  constexpr auto end() {
    return iterator_iterator<decltype(end(r))>(end(r));
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return iterator_iterator<decltype(begin(r))>(begin(r));
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return iterator_iterator<decltype(end(r))>(end(r));
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return size(r);
  }
};
template <class R>
struct is_range_reference<iterator_range<R>> : is_range_reference<R> {};

template <class A, class B>
constexpr iter_pair<iterator_iterator<common_type_t<A, B>>>
fo_irng::operator ()(A a, B b) const
  requires (is_integral_v<A> && is_integral_v<B>) {
  using it = iterator_iterator<common_type_t<A, B>>;
  return iter_pair<it>(it(a), it(b));
}
template <class A, class B>
constexpr iter_pair<iterator_iterator<common_iter_t<A, B>>>
fo_irng::operator ()(A a, B b) const
  requires (!is_integral_v<A> && !is_integral_v<B>) {
  using it = iterator_iterator<common_iter_t<A, B>>;
  return iter_pair<it>(a, b);
}

template <class R>
constexpr auto fo_irng::operator ()(R &&r) const requires is_rng<R> {
  return iterator_range<rng_forward_t<R>>(forward<R>(r));
}
template <class A, class B>
constexpr auto fo_irng::operator ()(A a, B b) const
  requires (!is_integral_v<A> && is_integral_v<B>) {
  return irng(rng(a, b));
}

}

// bind_range
namespace re {

template <class II, class F>
class bind_iterator {
  using this_t = bind_iterator;

  template <class, class>
  friend class bind_iterator;

  II i = II{};
  semiregular_function<F> f = semiregular_function<F>{};

public:
  using difference_type = itr_dft<II>;
  using reference = decltype(call_or_deref_call
                             (declval<const F &>(), declval<itr_ref<II>>()));
  using pointer = itr_ptr_from_ref<this_t>;
  using value_type = remove_cvref_t<reference>;
  using iterator_category
    = conditional_t<is_ritr<II>, random_access_iterator_tag, itr_ctg<II>>;

  bind_iterator() = default;
  ~bind_iterator() = default;
  bind_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  bind_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<II>
             && is_nothrow_swappable_v<semiregular_function<F>>) {
    adl_swap(x.i, y.i);
    adl_swap(x.f, y.f);
  }

  constexpr bind_iterator(II ii, F ff) : i(ii), f(ff) {}

  template <class II2, class FF>
  constexpr bind_iterator(const bind_iterator<II2, FF> &x)
    requires (!(is_same_v<II, II2> && is_same_v<F, FF>)
              && is_convertible_v<const II2 &, II>
              && is_convertible_v<const FF &, F>)
    : i(x.i), f(x.f) {}

  constexpr reference operator *() const {
    return f(*i);
  }
  constexpr pointer operator ->() const
    requires (!is_same_v<pointer, void>) {
    return addressof(operator *());
  }
  constexpr this_t &operator ++() {
    ++i;
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int) requires is_fitr<II> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<II> {
    --i;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<II> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type j) const
    requires is_ritr<II> {
    return *(*this + j);
  }
  constexpr this_t &operator +=(difference_type j) requires is_ritr<II> {
    i += j;
    return *this;
  }
  constexpr this_t &operator -=(difference_type j) requires is_ritr<II> {
    i -= j;
    return *this;
  }
  constexpr this_t operator +(difference_type j) const
    requires is_ritr<II> {
    return copy(*this) += j;
  }
  constexpr this_t operator -(difference_type j) const
    requires is_ritr<II> {
    return copy(*this) -= j;
  }

  constexpr II base() const {
    return i;
  }
  constexpr auto fn() const {
    return f.base();
  }
};
template <class II1, class II2, class FF>
constexpr auto operator ==(const bind_iterator<II1, FF> &x,
                           const bind_iterator<II2, FF> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class II1, class FF>
constexpr bind_iterator<II1, FF>
operator +(typename bind_iterator<II1, FF>::difference_type n,
           const bind_iterator<II1, FF> &x)
  requires is_ritr<II1> {
  return x + n;
}
template <class II1, class II2, class FF>
constexpr auto operator -(const bind_iterator<II1, FF> &x,
                          const bind_iterator<II2, FF> &y)
  ->decltype(x.base() - y.base())
  requires itr_is_counted<II1> && itr_is_counted<II2> {
  return x.base() - y.base();
}
template <class II1, class II2, class FF>
constexpr synth_3way_result<II1, II2>
operator <=>(const bind_iterator<II1, FF> &x,
             const bind_iterator<II2, FF> &y) {
  return synth_3way(x.base(), y.base());
}
struct fo_bind_itr {
  template <class II, class F>
  constexpr auto operator ()(II i, F f) const requires is_iitr<II> {
    return bind_iterator<II, F>(i, f);
  }
};
inline constexpr fo_bind_itr bind_itr{};

template <class R, class F>
class bind_range : range_fns {
  R r = R{};
  copyable_wrapper<F> fn = copyable_wrapper<F>{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  bind_range() = delete;
  bind_range() requires (is_default_constructible_v<R>
                         && is_default_constructible_v<F>) = default;
  bind_range(const bind_range &) = default;
  bind_range &operator =(const bind_range &) = default;
  bind_range(bind_range &&) = default;
  bind_range &operator =(bind_range &&) = default;
  friend constexpr void swap(bind_range &x, bind_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && default_swappable<bind_range>) {
    adl_swap(x.r, y.r);
    adl_swap(x.fn, y.fn);
  }

  constexpr bind_range(R rr, F ff) : r(forward<R>(rr)), fn(ff) {}

  constexpr auto begin() {
    if constexpr (is_default_constructible_v<F>) {
      return bind_iterator<rng_itr<R>, F>(begin(r), *fn);
    }
    else {
      return bind_iterator<rng_itr<R>, const F *>(begin(r), addressof(*fn));
    }
  }
  constexpr auto end() {
    if constexpr (is_default_constructible_v<F>) {
      return bind_iterator<rng_itr<R>, F>(end(r), *fn);
    }
    else {
      return bind_iterator<rng_itr<R>, const F *>(end(r), addressof(*fn));
    }
  }
  constexpr auto begin() const
    requires (is_rng<add_const_t<R>>
              && is_invocable_v<const F &, rng_cref<add_const_t<R>>>) {
    if constexpr (is_default_constructible_v<F>) {
      return bind_iterator<rng_itr<const R>, F>(begin(r), *fn);
    }
    else {
      return bind_iterator<rng_itr<const R>, const F *>
        (begin(r), addressof(*fn));
    }
  }
  constexpr auto end() const
    requires (is_rng<add_const_t<R>>
              && is_invocable_v<const F &, rng_cref<add_const_t<R>>>) {
    if constexpr (is_default_constructible_v<F>) {
      return bind_iterator<rng_itr<const R>, F>(end(r), *fn);
    }
    else {
      return bind_iterator<rng_itr<const R>, const F *>
        (end(r), addressof(*fn));
    }
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const
    requires (is_rng<add_const_t<R>>
              && is_invocable_v<const F &, rng_cref<add_const_t<R>>>) {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>
              && is_invocable_v<const F &, rng_cref<add_const_t<R>>>) {
    return size(r);
  }
};
template <class R, class F>
struct is_range_reference<bind_range<R, F>>
  : bool_constant<is_range_reference<R>::value
                  && is_default_constructible_v<F>> {};
struct fo_bind_rng {
  template <class R, class F>
  constexpr auto operator ()(R &&r, F f) const requires is_rng<R> {
    return bind_range<rng_forward_t<R>, F>(forward<R>(r), f);
  }
};
inline constexpr fo_bind_rng bind_rng{};

struct fo_deref_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return bind_rng(forward<R>(r), deref);
  }
  template <class X, class Y>
  constexpr auto operator ()(X x, Y y) const {
    return operator ()(rng(x, y));
  }
};
inline constexpr fo_deref_rng deref_rng{};
struct fo_deref_itr {
  template <class I>
  constexpr auto operator ()(I i) const {
    using f_t = decay_t<decltype(deref)>;
    return bind_iterator<I, f_t>(i, f_t{});
  }
};
inline constexpr fo_deref_itr deref_itr{};

namespace inner {

struct as_cref {
  template <class T>
  constexpr add_const_t<remove_reference_t<T>> &
  operator ()(T &&x) const noexcept requires is_lvalue_reference_v<T &&> {
    return x;
  }
  template <class T>
  constexpr T operator ()(T &&x) const noexcept
    requires is_rvalue_reference_v<T &&> {
    return T(forward<T>(x));
  }
};

}
struct fo_const_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return bind_rng(forward<R>(r), inner::as_cref{});
  }
  template <class X, class Y>
  constexpr auto operator ()(X x, Y y) const {
    return operator ()(rng(x, y));
  }
};
inline constexpr fo_const_rng const_rng{};
struct fo_const_itr {
  template <class I>
  constexpr auto operator ()(I i) const
    requires (is_itr<I> && is_reference_v<itr_ref<I>>) {
    return bind_itr(i, inner::as_cref{});
  }
};
inline constexpr fo_const_itr const_itr{};

}

// for_iter_range
namespace re {

template <class I>
class for_iter_iterator {
  template <class>
  friend class for_iter_iterator;

  I i;

public:
  for_iter_iterator() = delete;
  ~for_iter_iterator() = default;
  for_iter_iterator(const for_iter_iterator &) = delete;
  for_iter_iterator &operator =(const for_iter_iterator &) = delete;
  for_iter_iterator(for_iter_iterator &&) = default;
  for_iter_iterator &operator =(for_iter_iterator &&) = delete;

  constexpr explicit for_iter_iterator(I ii) : i(ii) {}

  constexpr I &operator *() {
    return i;
  }
  constexpr void operator ++() {
    ++i;
  }
  template <class X>
  constexpr bool operator !=(for_iter_iterator<X> &x) {
    return i != x.i;
  }
};
template <class R>
class for_iter_range {
  R r;

public:
  for_iter_range() = delete;
  ~for_iter_range() = default;
  for_iter_range(const for_iter_range &) = delete;
  for_iter_range &operator =(const for_iter_range &) = delete;
  for_iter_range(for_iter_range &&) = default;
  for_iter_range &operator =(for_iter_range &&) = delete;

  template <class X>
  constexpr explicit for_iter_range(X &&x) : r(forward<X>(x)) {}

  constexpr auto begin() {
    return for_iter_iterator<decltype(re::begin(r))>(re::begin(r));
  }
  constexpr auto end() {
    return for_iter_iterator<decltype(re::end(r))>(re::end(r));
  }
};
struct fo_iters {
  template <class T>
  constexpr auto operator ()(T &&v) const {
    return for_iter_range<T>(forward<T>(v));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const {
    return operator ()(iter_pair<common_iter_t<A, B>>(a, b));
  }
};
inline constexpr fo_iters iters{};

template <class I>
class r_for_iter_iterator {
  template <class>
  friend class r_for_iter_iterator;

  I i;

public:
  r_for_iter_iterator() = delete;
  ~r_for_iter_iterator() = default;
  r_for_iter_iterator(const r_for_iter_iterator &) = delete;
  r_for_iter_iterator &operator =(const r_for_iter_iterator &) = delete;
  r_for_iter_iterator(r_for_iter_iterator &&) = default;
  r_for_iter_iterator &operator =(r_for_iter_iterator &&) = delete;

  constexpr explicit r_for_iter_iterator(I ii) : i(ii) {}

  constexpr I &operator *() {
    return i;
  }
  constexpr void operator ++() {}
  template <class X>
  constexpr bool operator !=(r_for_iter_iterator<X> &x) {
    const bool y = (i != x.i);
    --i;
    return y;
  }
};
template <class R>
class r_for_iter_range {
  R r;

public:
  r_for_iter_range() = delete;
  ~r_for_iter_range() = default;
  r_for_iter_range(const r_for_iter_range &) = delete;
  r_for_iter_range &operator =(const r_for_iter_range &) = delete;
  r_for_iter_range(r_for_iter_range &&) = default;
  r_for_iter_range &operator =(r_for_iter_range &&) = delete;

  template <class X>
  constexpr explicit r_for_iter_range(X &&x) : r(forward<X>(x)) {}

  constexpr auto begin() {
    return r_for_iter_iterator<decltype(re::begin(r))>(re::end(r));
  }
  constexpr auto end() {
    return r_for_iter_iterator<decltype(re::end(r))>(re::begin(r));
  }
};
struct fo_r_iters {
  template <class T>
  constexpr auto operator ()(T &&v) const {
    return r_for_iter_range<T>(forward<T>(v));
  }
  template <class A, class B>
  constexpr auto operator ()(A a, B b) const {
    return operator ()(iter_pair<common_iter_t<A, B>>(a, b));
  }
};
inline constexpr fo_r_iters r_iters{};

}

// range miscl
namespace re {

template <class FR, class FI>
concept can_memmove_from_range_to_iterator
  = is_crng<FR> && is_citr<FI>
  && is_same_v<rng_vt<FR>, itr_vt<FI>>
  && is_trivially_copyable_v<itr_vt<FI>>
  && (is_same_v<itr_ref<FI>, itr_vt<FI> &>
      || is_same_v<itr_ref<FI>, itr_vt<FI> &&>)
  && (is_same_v<rng_ref<FR>, itr_vt<FI> &>
      || is_same_v<rng_ref<FR>, const itr_vt<FI> &>
      || is_same_v<rng_ref<FR>, itr_vt<FI> &&>
      || is_same_v<rng_ref<FR>, const itr_vt<FI> &&>);

}

#endif
