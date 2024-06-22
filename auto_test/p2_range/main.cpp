#include <re/range.h>
#include <re/test.h>

#include <cassert>

namespace re::inner::fns::help_iterator_requirements {

template <class T>
concept has_difference_type = requires {typename T::difference_type;};

template <class T>
concept has_value_type = requires {typename T::value_type;};

struct t_with_specialized_iter_traits {};

struct itr_t {
  itr_t() = delete;
  ~itr_t() = default;
  itr_t(const itr_t &) = default;
  itr_t &operator =(const itr_t &) = default;
  itr_t(itr_t &&) = default;
  itr_t &operator =(itr_t &&) = default;

  int &operator *() const;
  itr_t &operator ++();
  itr_t operator ++(int);
};
struct iitr_t {
  using value_type = int;
  using difference_type = ptrdiff_t;

  iitr_t() = delete;
  ~iitr_t() = default;
  iitr_t(const iitr_t &) = default;
  iitr_t &operator =(const iitr_t &) = default;
  iitr_t(iitr_t &&) = default;
  iitr_t &operator =(iitr_t &&) = default;

  int &operator *() const;
  iitr_t &operator ++();
  iitr_t operator ++(int);
};
bool operator ==(const iitr_t &, const iitr_t &);

struct fitr_t {
  using value_type = int;
  using difference_type = ptrdiff_t;

  fitr_t() = default;
  ~fitr_t() = default;
  fitr_t(const fitr_t &) = default;
  fitr_t &operator =(const fitr_t &) = default;
  fitr_t(fitr_t &&) = default;
  fitr_t &operator =(fitr_t &&) = default;

  int &operator *() const;
  fitr_t &operator ++();
  fitr_t operator ++(int);
};
bool operator ==(const fitr_t &, const fitr_t &);

struct bitr_t {
  using value_type = int;
  using difference_type = ptrdiff_t;

  bitr_t() = default;
  ~bitr_t() = default;
  bitr_t(const bitr_t &) = default;
  bitr_t &operator =(const bitr_t &) = default;
  bitr_t(bitr_t &&) = default;
  bitr_t &operator =(bitr_t &&) = default;

  int &operator *() const;
  bitr_t &operator ++();
  bitr_t operator ++(int);

  bitr_t &operator --();
  bitr_t operator --(int);
};
bool operator ==(const bitr_t &, const bitr_t &);

struct ritr_t {
  using value_type = int;
  using difference_type = ptrdiff_t;

  ritr_t() = default;
  ~ritr_t() = default;
  ritr_t(const ritr_t &) = default;
  ritr_t &operator =(const ritr_t &) = default;
  ritr_t(ritr_t &&) = default;
  ritr_t &operator =(ritr_t &&) = default;

  int &operator *() const;
  ritr_t &operator ++();
  ritr_t operator ++(int);

  ritr_t &operator --();
  ritr_t operator --(int);

  ritr_t &operator +=(difference_type);
  ritr_t &operator -=(difference_type);
  ritr_t operator +(difference_type);
  ritr_t operator -(difference_type);
  value_type &operator [](difference_type);
};
bool operator ==(const ritr_t &, const ritr_t &);
strong_ordering operator <=>(const ritr_t &, const ritr_t &);
ritr_t operator +(ptrdiff_t, const ritr_t &);
ptrdiff_t operator -(const ritr_t &, const ritr_t &);

template <class T>
constexpr bool is_empty_iterator_traits() {return false;}
template <class T>
constexpr bool is_empty_iterator_traits()
  requires ((!requires {typename T::iterator_category;})
            && (!requires {typename T::value_type;})
            && (!requires {typename T::difference_type;})
            && (!requires {typename T::pointer;})
            && (!requires {typename T::reference;})) {return true;}

struct imov_t {};
constexpr int iter_move(imov_t) {return 1;}
struct imov_t2 {
  constexpr int operator *() {return 1;}
};
struct imov_t3 {
  int value = 2;
  constexpr int &operator *() {return value;}
};
struct imov_t4 {
  int value = 3;
  constexpr int &&operator *() {return move(value);}
};

struct iswap_t {
  using value_type = int;
  using difference_type = ptrdiff_t;
  int value;
};
constexpr void iter_swap(iswap_t &x, iswap_t &y) {
  adl_swap(x.value, y.value);
}

struct iswap_t2 {
  using value_type = int;
  using difference_type = ptrdiff_t;

  int *p;
  constexpr int &operator *() const {return *p;}

  operator int() const {return *p;}
};
void iter_swap(iswap_t2, iswap_t2) = delete;
int &&iter_move(iswap_t2 x) {return move(*x);}

}
namespace re {

template <>
struct iterator_traits<inner::fns::help_iterator_requirements
                       ::t_with_specialized_iter_traits> {
  using value_type = int;
  using reference = int &;
  using pointer = int *;
  using difference_type = ptrdiff_t;
  using iterator_category = input_iterator_tag;
};

}

namespace re {

template <>
struct iterator_is_counted
<degraded_iterator<void ****, bidirectional_iterator_tag>> : false_type {};

}

namespace re::inner::fns {

void test_iterator_requirements() {
  // inner::with_primary_iter_traits
  {
    static_assert(!inner::with_primary_iter_traits<int *>);
    static_assert(inner::with_primary_iter_traits<void>);
  }

  // inner::can_reference
  // inner::dereferenceable
  {
    static_assert(inner::can_reference<int>);
    static_assert(inner::can_reference<void ()>);
    static_assert(!inner::can_reference<void () const>);

    static_assert(inner::dereferenceable<int *>);
    static_assert(!inner::dereferenceable<void *>);
  }

  // incrementable_traits
  // iter_difference_t
  {
    using namespace help_iterator_requirements;
    static_assert(!has_difference_type<incrementable_traits<void (*)()>>);
    static_assert(!has_difference_type<incrementable_traits<void>>);
    static_assert(!has_difference_type<incrementable_traits<void *>>);
    static_assert(has_difference_type<incrementable_traits<int>>);
    static_assert(same_as
                  <incrementable_traits<int *>::difference_type,
                   ptrdiff_t>);
    static_assert(same_as
                  <incrementable_traits<int *const>::difference_type,
                   ptrdiff_t>);
    struct t {using difference_type = void;};
    static_assert(same_as
                  <incrementable_traits<t>::difference_type, void>);
    static_assert(same_as
                  <incrementable_traits<int>::difference_type, int>);
    static_assert(same_as
                  <incrementable_traits<const int>::difference_type, int>);

    using s = help_iterator_requirements
      ::t_with_specialized_iter_traits;
    static_assert(same_as<iter_difference_t<s>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const s>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<volatile s>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const volatile s>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<s &>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const s &>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<volatile s &>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const volatile s &>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<s &&>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const s &&>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<volatile s &&>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<const volatile s &&>, ptrdiff_t>);
    static_assert(same_as<iter_difference_t<int *>, ptrdiff_t>);
  }

  // indirectly_readable_traits
  // iter_value_t
  {
    using namespace help_iterator_requirements;
    static_assert(same_as
                  <indirectly_readable_traits<int *>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int *>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<int *const>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int *const>::value_type,
                   int>);
    static_assert(!has_value_type
                  <indirectly_readable_traits<void (*)()>>);
    static_assert(same_as
                  <indirectly_readable_traits<int []>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int []>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<int [3]>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<const int [3]>::value_type, int>);
    struct t {using value_type = int;};
    struct tt {using element_type = int;};
    struct ttt {
      using element_type = int;
      using value_type = int;
    };
    struct tttt {
      using element_type = int;
      using value_type = short;
    };
    struct ttttt {
      using element_type = const int;
      using value_type = volatile int;
    };
    static_assert(same_as
                  <indirectly_readable_traits<t>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<tt>::value_type, int>);
    static_assert(same_as
                  <indirectly_readable_traits<ttt>::value_type, int>);
    static_assert(!has_value_type
                  <indirectly_readable_traits<tttt>>);
    static_assert(same_as
                  <indirectly_readable_traits<ttttt>::value_type, int>);

    static_assert(same_as<iter_value_t<int *>, int>);
    static_assert(same_as<iter_value_t<const int *>, int>);
    static_assert(same_as<iter_value_t<int *&>, int>);
    static_assert(same_as<iter_value_t<int *&&>, int>);
    static_assert(same_as<iter_value_t<int *const &>, int>);
    static_assert(same_as<iter_value_t<int *const &&>, int>);
    static_assert(same_as<iter_value_t<const int *&>, int>);
    static_assert(same_as<iter_value_t<const int *&&>, int>);
    static_assert(same_as<iter_value_t<const int *const &>, int>);
    static_assert(same_as<iter_value_t<const int *const &&>, int>);
    using s = help_iterator_requirements
      ::t_with_specialized_iter_traits;
    static_assert(same_as<iter_value_t<s>, int>);
    static_assert(same_as<iter_value_t<const s>, int>);
    static_assert(same_as<iter_value_t<volatile s>, int>);
    static_assert(same_as<iter_value_t<const volatile s>, int>);
    static_assert(same_as<iter_value_t<s &>, int>);
    static_assert(same_as<iter_value_t<const s &>, int>);
    static_assert(same_as<iter_value_t<volatile s &>, int>);
    static_assert(same_as<iter_value_t<const volatile s &>, int>);
    static_assert(same_as<iter_value_t<s &&>, int>);
    static_assert(same_as<iter_value_t<const s &&>, int>);
    static_assert(same_as<iter_value_t<volatile s &&>, int>);
    static_assert(same_as<iter_value_t<const volatile s &&>, int>);
  }

  // iter_reference_t
  {
    struct t {
      int operator *() const {return 0;}
    };
    static_assert(same_as<iter_reference_t<int *>, int &>);
    static_assert(same_as<iter_reference_t<t>, int>);
  }

  // iterator_traits
  {
    // specialization for pointer
    {
      static_assert(same_as<iterator_traits<int *>::value_type, int>);
      static_assert(same_as<iterator_traits<const int *>::value_type, int>);
      static_assert(same_as<iterator_traits<const volatile int *>::value_type,
                            int>);
      static_assert(same_as<iterator_traits<int *>::reference, int &>);
      static_assert(same_as<iterator_traits<const int *>::reference,
                            const int &>);
      static_assert(same_as<iterator_traits<const volatile int *>::reference,
                            const volatile int &>);
      static_assert(same_as<iterator_traits<int *>::pointer, int *>);
      static_assert(same_as<iterator_traits<int *>::difference_type,
                            ptrdiff_t>);
      static_assert(same_as<iterator_traits<int *>::iterator_category,
                            contiguous_iterator_tag>);
    }
    // cpp17_XXX_iterator
    {
      using namespace help_iterator_requirements;

      static_assert(cpp17_iterator<int *>);
      static_assert(cpp17_input_iterator<int *>);
      static_assert(cpp17_forward_iterator<int *>);
      static_assert(cpp17_bidirectional_iterator<int *>);
      static_assert(cpp17_random_access_iterator<int *>);

      static_assert(cpp17_iterator<itr_t>);
      static_assert(!cpp17_input_iterator<itr_t>);
      static_assert(cpp17_input_iterator<iitr_t>);
      static_assert(!cpp17_forward_iterator<iitr_t>);
      static_assert(cpp17_forward_iterator<fitr_t>);
      static_assert(!cpp17_bidirectional_iterator<fitr_t>);
      static_assert(cpp17_bidirectional_iterator<bitr_t>);
      static_assert(!cpp17_random_access_iterator<bitr_t>);
      static_assert(cpp17_random_access_iterator<ritr_t>);
    }
    // inner::iter_traits_branch_1
    {
      struct t {
        using iterator_category = void;
        using value_type = void;
        using difference_type = void;
        using reference = void;
      };
      struct t2 {
        using value_type = void;
        using difference_type = void;
        using reference = void;
      };
      struct t3 {
        using iterator_category = void;
        using difference_type = void;
        using reference = void;
      };
      struct t4 {
        using iterator_category = void;
        using value_type = void;
        using reference = void;
      };
      struct t5 {
        using iterator_category = void;
        using value_type = void;
        using difference_type = void;
      };
      static_assert(inner::iter_traits_branch_1<t>);
      static_assert(!inner::not_iter_traits_branch_1<t>);
      static_assert(!inner::iter_traits_branch_1<t2>);
      static_assert(inner::not_iter_traits_branch_1<t2>);
      static_assert(!inner::iter_traits_branch_1<t3>);
      static_assert(inner::not_iter_traits_branch_1<t3>);
      static_assert(!inner::iter_traits_branch_1<t4>);
      static_assert(inner::not_iter_traits_branch_1<t4>);
      static_assert(!inner::iter_traits_branch_1<t5>);
      static_assert(inner::not_iter_traits_branch_1<t5>);
      static_assert(!inner::iter_traits_branch_1<int>);
      static_assert(inner::not_iter_traits_branch_1<int>);
    }
    // inner::has_mpointer
    // inner::get_iter_ptr
    // inner::get_iter_ptr_2
    {
      struct t {
        using pointer = int *;
      };
      static_assert(!inner::has_mpointer<int>);
      static_assert(inner::has_mpointer<t>);
      static_assert(same_as<inner::get_iter_ptr<int>::type, void>);
      static_assert(same_as<inner::get_iter_ptr<t>::type, int *>);

      struct tt {
        int *operator ->() const;
      };
      static_assert(same_as<inner::get_iter_ptr_2<int>::type, void>);
      static_assert(same_as<inner::get_iter_ptr_2<t>::type, int *>);
      static_assert(same_as<inner::get_iter_ptr_2<tt>::type, int *>);
    }
    // inner::has_mictg
    // inner::get_iter_ctg
    {
      struct t {
        using iterator_category = void;
      };
      static_assert(inner::has_mictg<t>);
      static_assert(!inner::has_mictg<int>);

      using namespace help_iterator_requirements;
      static_assert(same_as<inner::get_iter_ctg<iitr_t>::type,
                            input_iterator_tag>);
      static_assert(same_as<inner::get_iter_ctg<fitr_t>::type,
                            forward_iterator_tag>);
      static_assert(same_as<inner::get_iter_ctg<bitr_t>::type,
                            bidirectional_iterator_tag>);
      static_assert(same_as<inner::get_iter_ctg<ritr_t>::type,
                            random_access_iterator_tag>);
    }
    // inner::has_miref
    // inner::get_iter_ref
    {
      struct t {
        using reference = void;
      };
      static_assert(inner::has_miref<t>);
      static_assert(!inner::has_miref<int>);
      static_assert(same_as<inner::get_iter_ref<t>::type, void>);
      static_assert(same_as<inner::get_iter_ref<int *>::type, int &>);
    }
    // inner::has_mdft
    // inner::get_iter_dft
    {
      struct t {
        using difference_type = int;
      };
      static_assert(inner::has_mdft<t>);
      static_assert(!inner::has_mdft<int>);
      static_assert(same_as<inner::get_iter_dft<t>::type, int>);
      static_assert(same_as<inner::get_iter_dft<int>::type, void>);
      static_assert(same_as<inner::get_iter_dft<int *>::type, void>);
    }
    // inner::primary_iter_traits
    {
      using namespace help_iterator_requirements;
      static_assert(is_empty_iterator_traits<iterator_traits<void>>());

      struct t {
        using iterator_category = int;
        using value_type = int;
        using difference_type = int;
        using reference = int;
      };
      static_assert(inner::with_primary_iter_traits<t>);
      static_assert(same_as<iterator_traits<t>::iterator_category, int>);
      static_assert(same_as<iterator_traits<t>::value_type, int>);
      static_assert(same_as<iterator_traits<t>::difference_type, int>);
      static_assert(same_as<iterator_traits<t>::reference, int>);
      static_assert(same_as<iterator_traits<t>::pointer, void>);

      static_assert(inner::with_primary_iter_traits<iitr_t>);
      static_assert(same_as<iterator_traits<iitr_t>::iterator_category,
                            input_iterator_tag>);
      static_assert(same_as<iterator_traits<iitr_t>::value_type, int>);
      static_assert(same_as<iterator_traits<iitr_t>::difference_type,
                            ptrdiff_t>);
      static_assert(same_as<iterator_traits<iitr_t>::reference, int &>);
      static_assert(same_as<iterator_traits<iitr_t>::pointer, void>);

      static_assert(inner::with_primary_iter_traits<itr_t>);
      static_assert(same_as<iterator_traits<itr_t>::iterator_category,
                            output_iterator_tag>);
      static_assert(same_as<iterator_traits<itr_t>::value_type, void>);
      static_assert(same_as<iterator_traits<itr_t>::difference_type, void>);
      static_assert(same_as<iterator_traits<itr_t>::reference, void>);
      static_assert(same_as<iterator_traits<itr_t>::pointer, void>);
    }
  }

  // ranges::iter_move
  {
    using namespace help_iterator_requirements;
    static_assert(!re_adl::can_iter_move<int>);
    static_assert(re_adl::can_iter_move<imov_t>);
    static_assert(same_as<decltype(ranges::iter_move(imov_t{})), int>);
    static_assert(ranges::iter_move(imov_t{}) == 1);
    static_assert(same_as<decltype(ranges::iter_move(imov_t2{})), int>);
    static_assert(ranges::iter_move(imov_t2{}) == 1);
    static_assert(same_as<decltype(ranges::iter_move(imov_t3{})), int &&>);
    constexpr auto f = []() constexpr {
      imov_t3 x;
      decltype(auto) y = ranges::iter_move(x);
      y = 4;
      return x.value;
    };
    static_assert(f() == 4);
    static_assert(same_as<decltype(ranges::iter_move(imov_t4{})), int &&>);
    constexpr auto ff = []() constexpr {
      imov_t4 x;
      decltype(auto) y = ranges::iter_move(x);
      y = 5;
      return x.value;
    };
    static_assert(ff() == 5);

    static_assert(same_as<iter_rvalue_reference_t<int *>, int &&>);
    static_assert(same_as<iter_rvalue_reference_t<imov_t>, int>);
    static_assert(same_as<iter_rvalue_reference_t<imov_t2>, int>);
    static_assert(same_as<iter_rvalue_reference_t<imov_t3>, int &&>);
    static_assert(same_as<iter_rvalue_reference_t<imov_t4>, int &&>);

    static_assert(noexcept(ranges::iter_move(declval<int *>())));
    static_assert(noexcept(ranges::iter_move(declval<imov_t>())));
    static_assert(noexcept(ranges::iter_move(declval<imov_t2>())));
    static_assert(noexcept(ranges::iter_move(declval<imov_t3>())));
    static_assert(noexcept(ranges::iter_move(declval<imov_t4>())));
  }

  // ranges::iter_swap
  {
    using namespace help_iterator_requirements;
    static_assert(noexcept(ranges::iter_swap
                           (declval<int *>(), declval<int *>())));
    constexpr auto f = []() constexpr {
      iswap_t x(1), y(2);
      ranges::iter_swap(x, y);
      return x.value;
    };
    static_assert(f() == 2);
    constexpr auto ff = []() constexpr {
      int a(1), b(2);
      iswap_t2 x(&a), y(&b);
      static_assert(indirectly_readable<iswap_t2>);
      ranges::iter_swap(x, y);
      return a;
    };
    static_assert(ff() == 2);
  }

  // indirectly_readable
  {
    static_assert(indirectly_readable<int *>);
  }
  // iter_common_reference_t
  {
    static_assert(same_as<iter_common_reference_t<int *>, int &>);
  }
  // indirectly_writable
  {
    static_assert(indirectly_writable<int *, int>);
    static_assert(indirectly_writable<int *, int &>);
    static_assert(indirectly_writable<int *, const int &>);
    static_assert(indirectly_writable<int *, int &&>);
  }
  // indirectly_movable
  // indirectly_movable_storable
  // indirectly_copyable
  // indirectly_copyable_storable
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
  // indirectly_swappable
  // indirectly_comparable
  {
    static_assert(indirectly_swappable<int *, int *>);
    static_assert(indirectly_comparable<int *, int *, equal_to<>>);
  }

  // weakly_incrementable
  // incrementable
  {
    static_assert(weakly_incrementable<int *>);
    static_assert(incrementable<int *>);
  }
  // input_or_output_iterator
  // sentinel_for
  // disable_sized_sentinel_for
  // sized_sentinel_for
  {
    static_assert(input_or_output_iterator<int *>);
    static_assert(sentinel_for<const int *, int *>);
    static_assert(sentinel_for<int *, const int *>);
    static_assert(!disable_sized_sentinel_for<int *, int *>);
    static_assert(sized_sentinel_for<int *, int *>);
  }
  // input_iterator
  // output_iterator
  // forward_iterator
  // bidirectional_iterator
  // random_access_iterator
  // contiguous_iterator
  {
    static_assert(!input_iterator<void>);
    static_assert(!forward_iterator<void>);
    static_assert(!bidirectional_iterator<void>);
    static_assert(!random_access_iterator<void>);
    static_assert(!contiguous_iterator<void>);
    static_assert(input_iterator<int *>);
    static_assert(forward_iterator<int *>);
    static_assert(bidirectional_iterator<int *>);
    static_assert(random_access_iterator<int *>);
    static_assert(contiguous_iterator<int *>);
  }

  // indirectly_unary_invocable
  // indirectly_regular_unary_invocable
  {
    const auto f = [](const int &) {};
    static_assert(indirectly_unary_invocable<decltype(f), int *>);
    static_assert(indirectly_regular_unary_invocable<decltype(f), int *>);
  }
  // indirect_unary_predicate
  // indirect_binary_predicate
  {
    const auto f = [](const int &) {return true;};
    static_assert(indirect_unary_predicate<decltype(f), int *>);
    const auto ff = [](const int &, void *&) {return true;};
    static_assert(indirect_binary_predicate<decltype(ff), int *, void **>);
  }
  // indirect_equivalence_relation
  // indirect_strict_weak_order
  {
    static_assert(indirect_equivalence_relation<equal_to<int>, int *, int *>);
    static_assert(indirect_strict_weak_order<less<int>, int *, int *>);
  }
  // indirect_result_t
  {
    const auto f = []() {return true;};
    const auto ff = [](int, int, int, int) {return true;};
    static_assert(same_as<indirect_result_t<decltype(f)>, bool>);
    static_assert(same_as<indirect_result_t
                          <decltype(ff), int *, int *, int *, int *>, bool>);
  }
  // projected
  {
    const auto f = [](const int &)->void * {return nullptr;};
    using pseudo_iter = projected<int *, decltype(f)>;
    static_assert(same_as<iter_value_t<pseudo_iter>, void *>);
    static_assert(same_as<iter_difference_t<pseudo_iter>, ptrdiff_t>);
    static_assert(same_as<iter_reference_t<pseudo_iter>, void *>);
  }
  // permutable
  // mergeable
  // sortable
  {
    static_assert(permutable<int *>);
    static_assert(mergeable<int **, int **, void **>);
    static_assert(mergeable<int **, int **, void **,
                            ranges::less, identity, identity>);
    static_assert(sortable<int *>);
    static_assert(sortable<int *, less<int>, identity>);
  }
}
namespace help_iterator {

template <class T>
class test_oi {
  using this_t = test_oi;

  T *p;

public:
  using value_type = void;
  using reference = void;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = output_iterator_tag;

  test_oi() = default;
  ~test_oi() = default;
  test_oi(const test_oi &) = default;
  test_oi &operator =(const test_oi &) = default;
  test_oi(test_oi &&) = default;
  test_oi &operator =(test_oi &&) = default;

  constexpr explicit test_oi(T *p) : p(p) {}

  constexpr operator test_oi<add_const_t<T>>() const
    requires (!is_const_v<T>) && (!same_as<add_const_t<T>, T>) {
    return test_oi<add_const_t<T>>(p);
  }

  constexpr this_t &operator =(const T &x) {
    *p = x;
    return *this;
  }

  constexpr this_t &operator *() {return *this;}
  constexpr this_t &operator ++() {
    ++p;
    return *this;
  }
  constexpr this_t operator ++(int) {
    auto ret = *this;
    ++p;
    return ret;
  }
};

template <class T>
class test_ii {
  using this_t = test_ii;

  T *p;

public:
  using value_type = remove_cv_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = input_iterator_tag;

  test_ii() = default;
  ~test_ii() = default;
  test_ii(const test_ii &) = default;
  test_ii &operator =(const test_ii &) = default;
  test_ii(test_ii &&) = default;
  test_ii &operator =(test_ii &&) = default;

  constexpr explicit test_ii(T *p) : p(p) {}

  constexpr operator test_ii<add_const_t<T>>() const
    requires (!is_const_v<T>) && (!same_as<add_const_t<T>, T>) {
    return test_ii<add_const_t<T>>(p);
  }

  constexpr reference operator *() const {return *p;}
  constexpr this_t &operator ++() {
    ++p;
    return *this;
  }
  constexpr this_t operator ++(int) {
    auto ret = *this;
    ++p;
    return ret;
  }

  constexpr T *base() const {return p;}
};
template <class X, class Y>
constexpr bool operator ==(const test_ii<X> &x, const test_ii<Y> &y)
  requires equality_comparable_with<X *, Y *> {
  return x.base() == y.base();
}

template <class T>
class test_fi {
  using this_t = test_fi;

  T *p;

public:
  using value_type = remove_cv_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = forward_iterator_tag;

  test_fi() = default;
  ~test_fi() = default;
  test_fi(const test_fi &) = default;
  test_fi &operator =(const test_fi &) = default;
  test_fi(test_fi &&) = default;
  test_fi &operator =(test_fi &&) = default;

  constexpr explicit test_fi(T *p) : p(p) {}

  constexpr operator test_fi<add_const_t<T>>() const
    requires (!is_const_v<T>) && (!same_as<add_const_t<T>, T>) {
    return test_fi<add_const_t<T>>(p);
  }

  constexpr reference operator *() const {return *p;}
  constexpr this_t &operator ++() {
    ++p;
    return *this;
  }
  constexpr this_t operator ++(int) {
    auto ret = *this;
    ++p;
    return ret;
  }

  constexpr T *base() const {return p;}
};
template <class X, class Y>
constexpr bool operator ==(const test_fi<X> &x, const test_fi<Y> &y)
  requires equality_comparable_with<X *, Y *> {
  return x.base() == y.base();
}

template <class T>
class test_bi {
  using this_t = test_bi;

  T *p;

public:
  using value_type = remove_cv_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = bidirectional_iterator_tag;

  test_bi() = default;
  ~test_bi() = default;
  test_bi(const test_bi &) = default;
  test_bi &operator =(const test_bi &) = default;
  test_bi(test_bi &&) = default;
  test_bi &operator =(test_bi &&) = default;

  constexpr explicit test_bi(T *p) : p(p) {}

  constexpr operator test_bi<add_const_t<T>>() const
    requires (!is_const_v<T>) && (!same_as<add_const_t<T>, T>) {
    return test_bi<add_const_t<T>>(p);
  }

  constexpr reference operator *() const {return *p;}
  constexpr this_t &operator ++() {
    ++p;
    return *this;
  }
  constexpr this_t operator ++(int) {
    auto ret = *this;
    ++p;
    return ret;
  }

  constexpr this_t &operator --() {
    --p;
    return *this;
  }
  constexpr this_t operator --(int) {
    auto ret = *this;
    --p;
    return ret;
  }

  constexpr T *base() const {return p;}
};
template <class X, class Y>
constexpr bool operator ==(const test_bi<X> &x, const test_bi<Y> &y)
  requires equality_comparable_with<X *, Y *> {
  return x.base() == y.base();
}

template <class T>
class test_ri {
  using this_t = test_ri;

  T *p;

public:
  using value_type = remove_cv_t<T>;
  using reference = T &;
  using pointer = T *;
  using difference_type = ptrdiff_t;
  using iterator_category = random_access_iterator_tag;

  test_ri() = default;
  ~test_ri() = default;
  test_ri(const test_ri &) = default;
  test_ri &operator =(const test_ri &) = default;
  test_ri(test_ri &&) = default;
  test_ri &operator =(test_ri &&) = default;

  constexpr explicit test_ri(T *p) : p(p) {}

  constexpr operator test_ri<add_const_t<T>>() const
    requires (!is_const_v<T>) && (!same_as<add_const_t<T>, T>) {
    return test_ri<add_const_t<T>>(p);
  }

  constexpr reference operator *() const {return *p;}
  constexpr this_t &operator ++() {
    ++p;
    return *this;
  }
  constexpr this_t operator ++(int) {
    auto ret = *this;
    ++p;
    return ret;
  }

  constexpr this_t &operator --() {
    --p;
    return *this;
  }
  constexpr this_t operator --(int) {
    auto ret = *this;
    --p;
    return ret;
  }

  constexpr reference operator [](difference_type n) const {return p[n];}
  constexpr this_t &operator +=(difference_type n) {
    p += n;
    return *this;
  }
  constexpr this_t &operator -=(difference_type n) {
    p -= n;
    return *this;
  }
  constexpr this_t operator +(difference_type n) const {
    auto ret = *this;
    return ret += n;
  }
  constexpr this_t operator -(difference_type n) const {
    auto ret = *this;
    return ret -= n;
  }

  constexpr T *base() const {return p;}
};
template <class X, class Y>
constexpr bool operator ==(const test_ri<X> &x, const test_ri<Y> &y)
  requires equality_comparable_with<X *, Y *> {
  return x.base() == y.base();
}
template <class X, class Y>
constexpr synth_3way_result<X *, Y *>
operator <=>(const test_ri<X> &x, const test_ri<Y> &y) {
  return synth_3way(x.base(), y.base());
}
template <class X, class Y>
constexpr auto operator -(const test_ri<X> &x, const test_ri<Y> &y)
  ->decltype(x.base() - y.base()) {
  return x.base() - y.base();
}
template <class X>
constexpr test_ri<X> operator +(itr_dft<X *> n, const test_ri<X> &x) {
  auto ret = x;
  return ret += n;
}

}
void test_iterator_main_components() {
  // itr series for pointer
  {
    {
      static_assert(same_as<itr_vt<void>, void>);
      static_assert(same_as<itr_ptr<void>, void>);
      static_assert(same_as<itr_ptr_from_ref<void>, void>);
      static_assert(same_as<itr_ref<void>, void>);
      static_assert(same_as<itr_cref<void>, void>);
      static_assert(same_as<itr_rref<void>, void>);
      static_assert(same_as<itr_common_ref<void>, void>);
      static_assert(same_as<itr_dft<void>, void>);
      static_assert(same_as<itr_ctg<void>, void>);
      static_assert(!is_itr<void>);
      static_assert(!is_oitr<void>);
      static_assert(!is_iitr<void>);
      static_assert(!is_fitr<void>);
      static_assert(!is_bitr<void>);
      static_assert(!is_ritr<void>);
      static_assert(!is_citr<void>);
      static_assert(!is_just_iitr<void>);
      static_assert(!is_just_fitr<void>);
      static_assert(!is_just_bitr<void>);
      static_assert(!is_just_ritr<void>);
      static_assert(!itr_is_counted<void>);
    }
    {
      static_assert(same_as<itr_vt<int>, void>);
      static_assert(same_as<itr_ptr<int>, void>);
      static_assert(same_as<itr_ptr_from_ref<int>, void>);
      static_assert(same_as<itr_ref<int>, void>);
      static_assert(same_as<itr_cref<int>, void>);
      static_assert(same_as<itr_rref<int>, void>);
      static_assert(same_as<itr_common_ref<int>, void>);
      static_assert(same_as<itr_dft<int>, void>);
      static_assert(same_as<itr_ctg<int>, void>);
      static_assert(!is_itr<int>);
      static_assert(!is_oitr<int>);
      static_assert(!is_iitr<int>);
      static_assert(!is_fitr<int>);
      static_assert(!is_bitr<int>);
      static_assert(!is_ritr<int>);
      static_assert(!is_citr<int>);
      static_assert(!is_just_iitr<int>);
      static_assert(!is_just_fitr<int>);
      static_assert(!is_just_bitr<int>);
      static_assert(!is_just_ritr<int>);
      static_assert(!itr_is_counted<int>);
    }
    {
      static_assert(same_as<itr_vt<int *>, int>);
      static_assert(same_as<itr_ptr<int *>, int *>);
      static_assert(same_as<itr_ptr_from_ref<int *>, int *>);
      static_assert(same_as<itr_ref<int *>, int &>);
      static_assert(same_as<itr_cref<int *>, const int &>);
      static_assert(same_as<itr_rref<int *>, int &&>);
      static_assert(same_as<itr_common_ref<int *>, int &>);
      static_assert(same_as<itr_dft<int *>, ptrdiff_t>);
      static_assert(same_as<itr_ctg<int *>, contiguous_iterator_tag>);
      static_assert(is_itr<int *>);
      static_assert(!is_oitr<int *>);
      static_assert(is_iitr<int *>);
      static_assert(is_fitr<int *>);
      static_assert(is_bitr<int *>);
      static_assert(is_ritr<int *>);
      static_assert(is_citr<int *>);
      static_assert(!is_just_iitr<int *>);
      static_assert(!is_just_fitr<int *>);
      static_assert(!is_just_bitr<int *>);
      static_assert(!is_just_ritr<int *>);
      static_assert(itr_is_counted<int *>);
    }
    {
      static_assert(same_as<itr_vt<int *const>, int>);
      static_assert(same_as<itr_ptr<int *const>, int *>);
      static_assert(same_as<itr_ptr_from_ref<int *const>, int *>);
      static_assert(same_as<itr_ref<int *const>, int &>);
      static_assert(same_as<itr_cref<int *const>, const int &>);
      static_assert(same_as<itr_rref<int *const>, int &&>);
      static_assert(same_as<itr_common_ref<int *const>, int &>);
      static_assert(same_as<itr_dft<int *const>, ptrdiff_t>);
      static_assert(same_as<itr_ctg<int *const>, contiguous_iterator_tag>);
      static_assert(is_itr<int *const>);
      static_assert(!is_oitr<int *const>);
      static_assert(is_iitr<int *const>);
      static_assert(is_fitr<int *const>);
      static_assert(is_bitr<int *const>);
      static_assert(is_ritr<int *const>);
      static_assert(is_citr<int *const>);
      static_assert(!is_just_iitr<int *const>);
      static_assert(!is_just_fitr<int *const>);
      static_assert(!is_just_bitr<int *const>);
      static_assert(!is_just_ritr<int *const>);
      static_assert(itr_is_counted<int *const>);
    }
    {
      static_assert(same_as<itr_vt<const int *>, int>);
      static_assert(same_as<itr_ptr<const int *>, const int *>);
      static_assert(same_as<itr_ptr_from_ref<const int *>, const int *>);
      static_assert(same_as<itr_ref<const int *>, const int &>);
      static_assert(same_as<itr_cref<const int *>, const int &>);
      static_assert(same_as<itr_rref<const int *>, const int &&>);
      static_assert(same_as<itr_common_ref<const int *>, const int &>);
      static_assert(same_as<itr_dft<const int *>, ptrdiff_t>);
      static_assert(same_as<itr_ctg<const int *>, contiguous_iterator_tag>);
      static_assert(is_itr<const int *>);
      static_assert(!is_oitr<const int *>);
      static_assert(is_iitr<const int *>);
      static_assert(is_fitr<const int *>);
      static_assert(is_bitr<const int *>);
      static_assert(is_ritr<const int *>);
      static_assert(is_citr<const int *>);
      static_assert(!is_just_iitr<const int *>);
      static_assert(!is_just_fitr<const int *>);
      static_assert(!is_just_bitr<const int *>);
      static_assert(!is_just_ritr<const int *>);
      static_assert(itr_is_counted<const int *>);
    }
  }
  // itr series for help_iterator::test_oi
  {
    using namespace help_iterator;

    using i = test_oi<int>;

    static_assert(input_or_output_iterator<i>);
    static_assert(!input_iterator<i>);
    static_assert(!forward_iterator<i>);
    static_assert(!bidirectional_iterator<i>);
    static_assert(!random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);

    static_assert(cpp17_iterator<i>);
    static_assert(!cpp17_input_iterator<i>);

    static_assert(same_as<itr_vt<i>, void>);
    static_assert(same_as<itr_ref<i>, void>);
    static_assert(same_as<itr_cref<i>, void>);
    static_assert(same_as<itr_rref<i>, void>);
    static_assert(same_as<itr_common_ref<i>, void>);
    static_assert(same_as<itr_ptr<i>, void>);
    static_assert(same_as<itr_ptr_from_ref<i>, void>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, output_iterator_tag>);
    static_assert(is_itr<i>);
    static_assert(is_oitr<i>);
    static_assert(!is_iitr<i>);
    static_assert(!is_fitr<i>);
    static_assert(!is_bitr<i>);
    static_assert(!is_ritr<i>);
    static_assert(!is_citr<i>);
    static_assert(!itr_is_counted<i>);

    int a[3] = {1, 2, 3};
    i j(begin(a));
    *j++ = 4;
    *j++ = 5;
    *j++ = 6;
    assert(tuple(a[0], a[1], a[2]) == tuple(4, 5, 6));
    using ii = test_oi<const int>;
    static_assert(convertible_to<i, ii>);
    static_assert(common_with<i, ii>);
    static_assert(same_as<common_type_t<i, ii>, ii>);
  }
  // itr series for help_iterator::test_ii
  {
    using namespace help_iterator;

    using i = test_ii<int>;
    using ci = test_ii<const int>;

    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(!forward_iterator<i>);
    static_assert(!bidirectional_iterator<i>);
    static_assert(!random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);

    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(!cpp17_bidirectional_iterator<i>);

    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_cref<i>, const int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, input_iterator_tag>);
    static_assert(is_itr<i>);
    static_assert(!is_oitr<i>);
    static_assert(is_iitr<i>);
    static_assert(!is_fitr<i>);
    static_assert(!is_bitr<i>);
    static_assert(!is_ritr<i>);
    static_assert(!is_citr<i>);
    static_assert(!itr_is_counted<i>);

    int a[3] = {1, 2, 3};
    i x(begin(a));
    ci y(begin(a));
    test_equality(x, y);
    ++y;
    test_inequality(x, y);
    y = x;
    test_equality(x, y);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    x = i(begin(a));
    *x = 4;
    *++x = 5;
    *++x = 6;
    assert(++x == i(end(a)));
    test_rng(a, ez_vector<int>({4, 5, 6}));
  }
  // itr series for help_iterator::test_fi
  {
    using namespace help_iterator;

    using i = test_fi<int>;
    using ci = test_fi<const int>;

    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(!bidirectional_iterator<i>);
    static_assert(!random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);

    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(!cpp17_bidirectional_iterator<i>);

    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_cref<i>, const int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, forward_iterator_tag>);
    static_assert(is_itr<i>);
    static_assert(!is_oitr<i>);
    static_assert(is_iitr<i>);
    static_assert(is_fitr<i>);
    static_assert(!is_bitr<i>);
    static_assert(!is_ritr<i>);
    static_assert(!is_citr<i>);
    static_assert(!itr_is_counted<i>);

    int a[3] = {1, 2, 3};
    i x(begin(a));
    ci y(begin(a));
    test_equality(x, y);
    ++y;
    test_inequality(x, y);
    y = x;
    test_equality(x, y);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    x = i(begin(a));
    *x = 4;
    *++x = 5;
    *++x = 6;
    assert(++x == i(end(a)));
    test_rng(a, ez_vector<int>({4, 5, 6}));
  }
  // itr series for help_iterator::test_bi
  {
    using namespace help_iterator;

    using i = test_bi<int>;
    using ci = test_bi<const int>;

    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(bidirectional_iterator<i>);
    static_assert(!random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);

    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(cpp17_bidirectional_iterator<i>);
    static_assert(!cpp17_random_access_iterator<i>);

    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_cref<i>, const int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, bidirectional_iterator_tag>);
    static_assert(is_itr<i>);
    static_assert(!is_oitr<i>);
    static_assert(is_iitr<i>);
    static_assert(is_fitr<i>);
    static_assert(is_bitr<i>);
    static_assert(!is_ritr<i>);
    static_assert(!is_citr<i>);
    static_assert(!itr_is_counted<i>);

    int a[3] = {1, 2, 3};
    i x(begin(a));
    ci y(begin(a));
    test_equality(x, y);
    ++y;
    test_inequality(x, y);
    y = x;
    test_equality(x, y);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    x = i(begin(a));
    *x = 4;
    *++x = 5;
    *++x = 6;
    assert(++x == i(end(a)));
    test_rng(a, ez_vector<int>({4, 5, 6}));
    *--x = 9;
    *--x = 8;
    *--x = 7;
    test_rng(a, ez_vector<int>({7, 8, 9}));
  }
  // itr series for help_iterator::test_ri
  {
    using namespace help_iterator;

    using i = test_ri<int>;
    using ci = test_ri<const int>;

    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(bidirectional_iterator<i>);
    static_assert(random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);

    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(cpp17_bidirectional_iterator<i>);
    static_assert(cpp17_random_access_iterator<i>);

    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_cref<i>, const int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, random_access_iterator_tag>);
    static_assert(is_itr<i>);
    static_assert(!is_oitr<i>);
    static_assert(is_iitr<i>);
    static_assert(is_fitr<i>);
    static_assert(is_bitr<i>);
    static_assert(is_ritr<i>);
    static_assert(!is_citr<i>);
    static_assert(itr_is_counted<i>);

    int a[3] = {1, 2, 3};
    i x(begin(a));
    ci y(begin(a));
    test_equality(x, y);
    ++y;
    test_inequality(x, y);
    y = x;
    test_equality(x, y);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    x = i(begin(a));
    *x = 4;
    *++x = 5;
    *++x = 6;
    assert(++x == i(end(a)));
    test_rng(a, ez_vector<int>({4, 5, 6}));
    *--x = 9;
    *--x = 8;
    *--x = 7;
    test_rng(a, ez_vector<int>({7, 8, 9}));
    x = i(begin(a));
    y = i(end(a));
    assert(x[0] == 7 && x[1] == 8 && x[2] == 9);
    test_lessness(x, y);
    (x += 1) += 2;
    test_equality(x, y);
    (x -= 1) -= 2;
    test_rng(rng(x, y), ez_vector<int>({7, 8, 9}));
    (x -= -1) -= -2;
    test_equality(x, y);
    (x += -1) += -2;
    test_rng(rng(x, y), ez_vector<int>({7, 8, 9}));
    assert(y - x == 3);
    assert(x - y == -3);
    test_equality(x + 1, y - 2);
    test_equality(x + 2, y - 1);
    test_equality(x - (-1), y + (-2));
    test_equality(x - (-2), y + (-1));
  }
  // itr_cref
  // itr_rref
  // itr_common_ref
  {
    struct t {operator bool() const {return true;}};
    struct tt {
      using reference = t;
      using value_type = bool;
    };
    static_assert(same_as<itr_common_ref<bool *>, bool &>);
    static_assert(same_as<itr_common_ref<tt>, bool>);
    struct ttt {
      using reference = int;
    };
    static_assert(same_as<itr_cref<int *>, const int &>);
    static_assert(same_as<itr_cref<ttt>, int>);
    static_assert(same_as<itr_rref<int *>, int &&>);
    static_assert(same_as<itr_rref<ttt>, int>);
  }
  // itr_is_counted
  {
    static_assert(itr_is_counted
                  <degraded_iterator
                   <const void ****, bidirectional_iterator_tag>>);
    static_assert(itr_is_counted
                  <degraded_iterator
                   <degraded_iterator
                    <const void ****, bidirectional_iterator_tag>,
                    forward_iterator_tag>>);
    static_assert(!itr_is_counted
                  <degraded_iterator<void ****, bidirectional_iterator_tag>>);
    static_assert(!itr_is_counted
                  <degraded_iterator
                   <degraded_iterator<void ****, bidirectional_iterator_tag>,
                    forward_iterator_tag>>);
  }

  // distance
  {
    using namespace help_iterator;

    static_assert(is_trivial_empty_v<decltype(distance)>);

    {
      int a[] = {1, 2, 3};
      int *p = begin(a);
      int *cp = end(a);
      assert(distance(p, cp) == 3);

      test_fi<int> i(begin(a)), j(end(a));
      assert(distance(i, j) == 3);

      static_assert(!noexcept(distance(p, cp)));
      static_assert(!noexcept(distance(i, j)));
    }

    constexpr auto f0 = []() constexpr {
      int a[] = {1, 2, 3};
      int *p = begin(a);
      int *pp = end(a);
      return distance(p, pp);
    };
    static_assert(f0() == 3);
    constexpr auto f = []() constexpr {
      int a[] = {1, 2, 3};
      test_fi<int> i(begin(a)), j(end(a));
      return distance(i, j);
    };
    static_assert(f() == 3);
  }
  // advance
  {
    using namespace help_iterator;

    static_assert(is_trivial_empty_v<decltype(advance)>);

    {
      int a[] = {1, 2, 3};
      test_ii<int> ii(begin(a));
      advance(ii, 0);
      assert(ii.base() == begin(a));
      advance(ii, 3);
      assert(ii.base() == end(a));

      test_bi<int> bi(begin(a));
      advance(bi, 3);
      assert(bi.base() == end(a));
      advance(bi, -3);
      assert(bi.base() == begin(a));
      advance(bi, 0);
      assert(bi.base() == begin(a));

      const int *p = begin(a);
      advance(p, 2);
      assert(p == begin(a) + 2);
      advance(p, -1);
      assert(p == begin(a) + 1);
      advance(p, -1);
      advance(p, 3);
      assert(p == end(a));
      advance(p, -3);
      assert(p == begin(a));
    }

    {
      constexpr auto f = []() constexpr {
        int a[] = {1, 2, 3};
        int *p = begin(a);
        advance(p, 3);
        return p == end(a);
      };
      static_assert(f());
      constexpr auto ff = []() constexpr {
        int a[] = {1, 2, 3};
        test_fi<int> i(begin(a));
        advance(i, 3);
        return i.base() == end(a);
      };
      static_assert(ff());
      constexpr auto fff = []() constexpr {
        int a[] = {1, 2, 3};
        test_bi<int> i(end(a));
        advance(i, -3);
        return i.base() == begin(a);
      };
      static_assert(fff());
    }

    {
      const auto f = []<class V>(V v) {
        v = {1, 2, 3, 4, 5, 6, 7, 8};
        const auto sz = ssize(v);
        for (int i : irng(0, sz)) {
          for (int j : irng(0, sz)) {
            auto r = irng(i, sz);
            if (j < i) {
              if constexpr (is_just_frng<V>)
                             continue;
              r = irng(0, i + 1);
            }
            for (int n : r) {
              n = n - i;
              auto it = nth(v, i);
              auto bound = nth(v, j);

              assert((i <= j && n >= 0) || (j < i && n <= 0));
              auto rest = advance(it, n, bound);
              if (i <= j)
                assert(n == distance(nth(v, i), it) + rest);
              else
                assert(n == -distance(it, nth(v, i)) + rest);
            }
          }
        }
      };
      f(ez_vector<int>{});
      f(ez_list<int>{});
      f(ez_forward_list<int>{});
    }
  }
  // next
  // prev
  {
    static_assert(is_trivial_empty_v<decltype(next)>);
    static_assert(is_trivial_empty_v<decltype(prev)>);

    constexpr int a[3] = {1, 2, 3};
    static_assert(next(begin(a)) == begin(a) + 1);
    static_assert(prev(next(begin(a))) == begin(a));
    static_assert(next(begin(a), 3) == end(a));
    static_assert(prev(begin(a), -3) == end(a));
    static_assert(prev(end(a), 3) == begin(a));
    static_assert(next(end(a), -3) == begin(a));
  }

  // extra advance/distance/next/prev
  {
    // advance(i, i2)
    {
      const ez_forward_list<int> l = {1, 2, 3};
      const auto r = rng(begin(l), 3);
      test_rng(r, irng(1, 4));
      auto it = begin(r);
      advance(it, end(r));
      assert(it.base() == end(l));
      advance(it, it);
      assert(it.base() == end(l));

      const ez_vector v = {1, 2, 3};
      auto it2 = begin(v);
      advance(it2, v.end());
      assert(it2 == v.end());
      advance(it2, it2);
      assert(it2 == v.end());
    }
    // advance(i, n, i2)
    {
      const ez_vector<int> v = {1, 2, 3, 4};
      auto it = begin(v);
      advance(it, 4, end(v));
      assert(it == end(v));
      it = begin(v);
      advance(it, 4, prev(end(v)));
      assert(it == prev(end(v)));
      it = begin(v);
      advance(it, 4, it);
      assert(it == begin(v));
      it = begin(v);
      advance(it, 2, end(v));
      assert(it == nth(v, 2));
      it = begin(v);
      advance(it, 0, end(v));
      assert(it == begin(v));
      it = begin(v);
      advance(it, 0, begin(v));
      assert(it == begin(v));

      it = end(v);
      advance(it, 0, end(v));
      assert(it == end(v));
      it = end(v);
      advance(it, 0, begin(v));
      assert(it == end(v));
      it = end(v);
      advance(it, -2, end(v));
      assert(it == end(v));
      it = end(v);
      advance(it, -4, begin(v));
      assert(it == begin(v));
      it = end(v);
      advance(it, -5, begin(v));
      assert(it == begin(v));
      it = end(v);
      advance(it, -3, begin(v));
      assert(it == next(begin(v)));

      using l_t = ez_forward_list<int>;
      l_t l = {1, 2, 3, 4};
      auto it2 = begin(l);
      advance(it2, 4, end(l));
      assert(it2 == end(l));
      it2 = begin(l);
      advance(it2, 0, begin(l));
      assert(it2 == begin(l));
      it2 = begin(l);
      advance(it2, 0, end(l));
      assert(it2 == begin(l));
      it2 = begin(l);
      advance(it2, 2, end(l));
      assert(it2 == nth(l, 2));
      it2 = begin(l);
      advance(it2, 2, nth(l, 1));
      assert(it2 == nth(l, 1));
    }
    // next(i, j)
    // next(i, n, j)
    // prev(i, n, j)
    {
      ez_list l = {1, 2, 3, 4};
      assert(next(begin(l), end(l)) == end(l));
      assert(next(begin(l), 3, end(l)) == nth(l, 3));
      assert(next(begin(l), 3, nth(l, 2)) == nth(l, 2));
      assert(next(end(l), -4, begin(l)) == nth(l, 0));
      assert(next(end(l), -4, nth(l, 1)) == nth(l, 1));
      assert(prev(end(l), 4, begin(l)) == nth(l, 0));
      assert(prev(end(l), 4, nth(l, 1)) == nth(l, 1));
    }
  }

  // common_iter_t
  // has_common_iter
  {
    static_assert(same_as<common_iter_t<int *, const int *>, const int *>);
    struct t {};
    struct tt {operator t();};
    static_assert(same_as<common_iter_t<t, t>, t>);
    static_assert(same_as<common_iter_t<t, tt>, t>);
    static_assert(same_as<common_iter_t<tt, t>, t>);
    static_assert(same_as<common_iter_t<tt, tt>, tt>);

    static_assert(has_common_iter<int *, const int *>);
    static_assert(has_common_iter<int *, int *>);
    static_assert(!has_common_iter<int *, int **>);
  }

  // iter_post_increment_return_type
  // iter_post_increment
  // iter_post_decrement
  {
    using t = iter_post_increment_return_type<int>;
    static_assert(!is_default_constructible_v<t>);
    static_assert(!is_copy_constructible_v<t>);
    static_assert(!is_copy_assignable_v<t>);
    static_assert(is_move_constructible_v<t>);
    static_assert(!is_move_assignable_v<t>);
    static_assert(!is_swappable_v<t>);

    const int a[] = {1, 2, 3};
    {
      ez_vector<int> v;
      const int *i = begin(a);
      v.insert(end(v), *iter_post_increment(i));
      v.insert(end(v), *iter_post_increment(i));
      v.insert(end(v), *iter_post_increment(i));
      assert(i == end(a));
      assert(v == ez_vector({1, 2, 3}));
    }
    {
      ez_vector<int> v;
      auto i = degraded_iitr(begin(a));
      v.insert(end(v), *iter_post_increment(i));
      v.insert(end(v), *iter_post_increment(i));
      v.insert(end(v), *iter_post_increment(i));
      assert(i.base() == end(a));
      assert(v == ez_vector({1, 2, 3}));
    }
    {
      auto i = end(a);
      static_assert(same_as<decltype(iter_post_decrement(i)), const int *>);
      assert(iter_post_decrement(i) == end(a));
      assert(i == end(a) - 1);
    }
  }
}
void test_range_main_components() {
  // begin
  // end
  {
    static_assert(is_trivial_empty_v<decltype(begin)>);
    static_assert(is_trivial_empty_v<decltype(end)>);
    struct t {
      constexpr int begin() const {return 1;}
      constexpr int begin() {return 2;}
      constexpr int end() const {return 1;}
      constexpr int end() {return 2;}
    };
    static_assert([]() constexpr {t x; return begin(x);}() == 2);
    static_assert([]() constexpr {t x; return begin(move(x));}() == 1);
    static_assert([]() constexpr {t x; return begin(as_const(x));}() == 1);
    static_assert([]() constexpr {t x; return begin(move(as_const(x)));}()
                  == 1);
    static_assert([]() constexpr {t x; return end(x);}() == 2);
    static_assert([]() constexpr {t x; return end(move(x));}() == 1);
    static_assert([]() constexpr {t x; return end(as_const(x));}() == 1);
    static_assert([]() constexpr {t x; return end(move(as_const(x)));}()
                  == 1);

    static_assert(*begin({1, 2, 3}) == 1);
    static_assert(*(end({1, 2, 3}) - 1) == 3);

    constexpr int ca[] = {1, 2, 3};
    static_assert(*begin(ca) == 1);
    static_assert(*(end(ca) - 1) == 3);
    int a[] = {1, 2, 3};
    assert(*begin(a) == 1);
    assert(*(end(a) - 1) == 3);
    static_assert(!invocable<decltype(begin), decltype(move(a))>);
    static_assert(!invocable<decltype(end), decltype(move(a))>);
    assert(*begin(as_const(a)) == 1);
    assert(*(end(as_const(a)) - 1) == 3);
    assert(*begin(move(as_const(a))) == 1);
    assert(*(end(move(as_const(a))) - 1) == 3);

    static_assert(!invocable<decltype(begin), int>);
    static_assert(!invocable<decltype(end), int>);
  }
  // size
  {
    static_assert(is_trivial_empty_v<decltype(size)>);
    static_assert(!invocable<decltype(size), int>);

    static_assert(size({1}) == 1);
    static_assert(size({1, 2, 3}) == 3);
    static_assert(size(initializer_list<int>{}) == 0);
    static_assert(noexcept(size({1, 2, 3})));

    int a[] = {1, 2, 3};
    assert(size(a) == 3);
    assert(size(as_const(a)) == 3);
    assert(size(move(a)) == 3);
    assert(size(move(as_const(a))) == 3);
    static_assert(noexcept(size(a)));
    static_assert([]() constexpr {
      int aa[] = {1, 2};
      return size(aa) == 2;
    }());

    struct t {
      constexpr size_t size() const {return 1;}
    };
    static_assert([]() constexpr {
      t x;
      return size(x) == 1;
    }());

    struct tt {
      const int a[3] = {1, 2, 3};
      constexpr const int *begin() const {return a;}
      constexpr const int *end() const {return re::end(a);}
    };
    static_assert([]() constexpr {
      tt x;
      return size(x) == 3;
    }());

    struct ttt {
      size_t size() {return 1;}
      size_t size() const {return 2;}
    };
    ttt x{};
    assert(size(x) == 1);
    assert(size(move(x)) == 2);
    assert(size(as_const(x)) == 2);
    assert(size(move(as_const(x))) == 2);
  }
  // ssize
  {
    constexpr int a[] = {1, 2, 3};
    static_assert(same_as<decltype(ssize(a)), ptrdiff_t>);
    static_assert(ssize(a) == 3);
    static_assert(!invocable<decltype(ssize), int>);

    struct ttt {
      size_t size() {return 1;}
      size_t size() const {return 2;}
    };
    ttt x{};
    assert(ssize(x) == 1);
    assert(ssize(move(x)) == 2);
    assert(ssize(as_const(x)) == 2);
    assert(ssize(move(as_const(x))) == 2);
  }
  // empty
  {
    static_assert(is_trivial_empty_v<decltype(empty)>);
    static_assert(!invocable<decltype(empty), int>);

    static_assert(empty({1}) == false);
    static_assert(empty({1, 2, 3}) == false);
    static_assert(empty(initializer_list<int>{}) == true);
    static_assert(noexcept(empty({1, 2, 3})));

    int a[] = {1, 2, 3};
    assert(!empty(a));
    assert(!empty(as_const(a)));
    assert(!empty(move(a)));
    assert(!empty(move(as_const(a))));
    static_assert(noexcept(empty(a)));
    static_assert([]() constexpr {
      int aa[] = {1, 2};
      return !empty(aa);
    }());

    struct t {
      constexpr bool empty() const {return false;}
    };
    static_assert([]() constexpr {
      t x;
      return !empty(x);
    }());

    struct tt {
      const int a[3] = {1, 2, 3};
      constexpr const int *begin() const {return a;}
      constexpr const int *end() const {return re::end(a);}
    };
    static_assert([]() constexpr {
      tt x;
      return !empty(x);
    }());

    struct ttt {
      bool empty() {return true;}
      bool empty() const {return false;}
    };
    ttt x{};
    assert(empty(x));
    assert(!empty(move(x)));
    assert(!empty(as_const(x)));
    assert(!empty(move(as_const(x))));
  }
  // is_rng
  {
    static_assert(!is_rng<int []>);
    static_assert(!is_rng<int (&)[]>);
    static_assert(!is_rng<int (&&)[]>);
    static_assert(!is_rng<const int []>);
    static_assert(!is_rng<const int (&)[]>);
    static_assert(!is_rng<const int (&&)[]>);

    static_assert(is_rng<int [1]>);
    static_assert(is_rng<int (&)[1]>);
    static_assert(is_rng<int (&&)[1]>);
    static_assert(is_rng<const int [1]>);
    static_assert(is_rng<const int (&)[1]>);
    static_assert(is_rng<const int (&&)[1]>);
    static_assert(is_rng<int [3]>);
    static_assert(is_rng<int (&)[3]>);
    static_assert(is_rng<int (&&)[3]>);
    static_assert(is_rng<const int [3]>);
    static_assert(is_rng<const int (&)[3]>);
    static_assert(is_rng<const int (&&)[3]>);

    struct t {
      int begin() const {return 0;}
      int end() const {return 0;}
    };
    static_assert(is_rng<t>);
    static_assert(is_rng<const t>);
    static_assert(is_rng<t &>);
    static_assert(is_rng<const t &>);
    static_assert(is_rng<t &&>);
    static_assert(is_rng<const t &&>);

    struct tt {
      int *begin() const {return 0;}
      const int *end() const {return 0;}
    };
    static_assert(!is_rng<tt>);
    static_assert(!is_rng<const tt>);
    static_assert(!is_rng<tt &>);
    static_assert(!is_rng<const tt &>);
    static_assert(!is_rng<tt &&>);
    static_assert(!is_rng<const tt &&>);

    struct ttt {
      const int *begin() const {return 0;}
      const int *end() const {return 0;}
    };
    static_assert(is_rng<ttt>);
    static_assert(is_rng<const ttt>);
    static_assert(is_rng<ttt &>);
    static_assert(is_rng<const ttt &>);
    static_assert(is_rng<ttt &&>);
    static_assert(is_rng<const ttt &&>);
  }

  // n_value_iterator
  // n_value
  {
    static_assert(random_access_iterator<n_value_iterator<int>>);
    static_assert(!contiguous_iterator<n_value_iterator<int>>);
    static_assert(random_access_iterator<n_value_iterator<const int>>);
    static_assert(!contiguous_iterator<n_value_iterator<const int>>);
    static_assert(cpp17_random_access_iterator<n_value_iterator<int>>);
    static_assert(cpp17_random_access_iterator
                  <n_value_iterator<const int>>);

    using t = n_value_iterator<const int>;
    static_assert(regular<t>);
    static_assert(is_nothrow_swappable_v<t>);
    static_assert(same_as<itr_vt<t>, int>);
    static_assert(same_as<itr_dft<t>, ptrdiff_t>);
    static_assert(same_as<itr_ptr<t>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<t>, const int *>);
    static_assert(same_as<itr_ref<t>, const int &>);
    static_assert(same_as<itr_cref<t>, const int &>);
    static_assert(same_as<itr_rref<t>, const int &&>);
    static_assert(same_as<itr_common_ref<t>, const int &>);
    static_assert(same_as<itr_ctg<t>, random_access_iterator_tag>);
    static_assert(itr_is_counted<t>);

    {
      int x = 1;
      t i(x, 0), j(x, 3);
      int a[] = {1, 1, 1};
      test_rng(rng(i, j), a);
      test_ritr(i, j, a);
      test_ritr(n_value_iterator(x, 0), n_value_iterator(x, 3), a);
      test_ritr(n_value_itr(x, 0), n_value_itr(x, 3), a);
      assert(to_address(i) == addressof(x));
      assert(to_address(i + 1) == addressof(x));
      assert(to_address(i + 2) == addressof(x));
      assert(to_address(i + 3) == addressof(x));

      int y = 2;
      i = {x, 1};
      j = {y, 2};
      adl_swap(i, i);
      adl_swap(j, j);
      adl_swap(i, j);
      assert(to_address(i) == addressof(y));
      assert(to_address(j) == addressof(x));
      assert(j - i == -1);
    }

    using nref = n_value<int &>;
    static_assert(!is_swappable_v<nref>);
    static_assert(!semiregular<nref>);
    static_assert(is_copy_constructible_v<nref>);
    static_assert(!is_copy_assignable_v<nref>);
    static_assert(is_move_constructible_v<nref>);
    static_assert(!is_move_assignable_v<nref>);
    static_assert(noexcept(declval<nref &>().begin()));
    static_assert(noexcept(declval<nref &>().end()));
    static_assert(noexcept(declval<nref &>().size()));
    static_assert(noexcept(declval<nref &>().empty()));
    static_assert(is_rng<nref>);
    static_assert(same_as<rng_szt<nref>, size_t>);
    static_assert(rng_is_sized<nref>);
    static_assert(rng_is_n_value<nref>);

    using nv = n_value<int>;
    static_assert(is_swappable_v<nv>);
    static_assert(semiregular<nv>);
    static_assert(noexcept(declval<nv &>().begin()));
    static_assert(noexcept(declval<nv &>().end()));
    static_assert(noexcept(declval<nv &>().size()));
    static_assert(noexcept(declval<nv &>().empty()));
    static_assert(is_rng<nv>);
    static_assert(same_as<rng_szt<nv>, size_t>);
    static_assert(rng_is_sized<nv>);
    static_assert(rng_is_n_value<nv>);

    {
      nv x(2, 4);
      long long a0[] = {4, 4};
      test_ritr(begin(x), end(x), a0);
      int a[] = {4, 4};
      test_ritr(begin(x), end(x), a);
      static_assert([]() {
        nv xx(3, 3);
        int aa[] = {3, 3, 3};
        test_ritr(begin(xx), end(xx), aa);
        return true;
      }());
      constexpr nv y(0);
      static_assert(empty(y));
      static_assert(size(y) == 0);
    }

    static_assert([]() constexpr {
      n_value<int> nv(1, 0);
      n_value<int> nv2(2, 3);
      adl_swap(nv, nv2);
      assert(size(nv) == 2);
      assert(ref(nv, 0) == 3);
      assert(ref(nv, 1) == 3);
      assert(size(nv2) == 1);
      assert(ref(nv2, 0) == 0);
      return true;
    }());
  }

  // range_traits
  // rng_itr
  // rng_szt
  // rng_vt
  // rng_ptr
  // rng_ptr_from_ref
  // rng_ref
  // rng_rref
  // rng_common_ref
  // rng_dft
  // rng_ctg
  // rng_is_sized
  // rng_is_counted
  // rng_is_n_value
  // is_orng
  // is_irng
  // is_frng
  // is_brng
  // is_rrng
  // is_crng
  // is_just_irng
  // is_just_frng
  // is_just_brng
  // is_just_rrng
  {
    static_assert(same_as<rng_itr<const void>, void>);
    static_assert(same_as<rng_itr<void>, void>);
    static_assert(same_as<rng_itr<int>, void>);
    static_assert(same_as
                  <rng_itr<n_value<int>>, n_value_iterator<const int>>);
    static_assert(same_as
                  <rng_itr<n_value<int &>>, n_value_iterator<const int>>);
    static_assert(same_as
                  <rng_itr<n_value<const int &>>, n_value_iterator<const int>>);
    static_assert(same_as
                  <rng_itr<const n_value<int>>, n_value_iterator<const int>>);
    static_assert(same_as
                  <rng_itr<n_value<int &> &>, n_value_iterator<const int>>);
    static_assert(same_as
                  <rng_itr<n_value<const int &> &&>,
                   n_value_iterator<const int>>);
    static_assert(same_as<rng_itr<void>, range_traits<void>::iterator>);
    static_assert(same_as<rng_itr<int [3]>, range_traits<int [3]>::iterator>);

    static_assert(same_as<rng_szt<const void>, void>);
    static_assert(same_as<rng_szt<void>, void>);
    static_assert(same_as<rng_szt<int>, void>);
    static_assert(same_as<rng_szt<n_value<int>>, size_t>);
    static_assert(same_as<rng_szt<const n_value<int>>, size_t>);
    static_assert(same_as<rng_szt<n_value<int> &>, size_t>);
    static_assert(same_as<rng_szt<const n_value<int> &>, size_t>);
    static_assert(same_as<rng_szt<n_value<int> &&>, size_t>);
    static_assert(same_as<rng_szt<const n_value<int> &&>, size_t>);
    static_assert(same_as<rng_szt<void>, range_traits<void>::size_type>);
    static_assert(same_as<rng_szt<int [3]>, range_traits<int [3]>::size_type>);

    static_assert(same_as<rng_vt<const void>, void>);
    static_assert(same_as<rng_vt<void>, void>);
    static_assert(same_as<rng_vt<int>, void>);
    static_assert(same_as<rng_vt<n_value<int>>, int>);
    static_assert(same_as<rng_vt<n_value<const int>>, int>);
    static_assert(same_as<rng_vt<n_value<const int> &>, int>);
    static_assert(same_as<rng_vt<void>, range_traits<void>::value_type>);
    static_assert(same_as<rng_vt<int [3]>, range_traits<int [3]>::value_type>);

    static_assert(same_as<rng_ptr<const void>, void>);
    static_assert(same_as<rng_ptr<void>, void>);
    static_assert(same_as<rng_ptr<int>, void>);
    static_assert(same_as<rng_ptr<n_value<int> &&>, const int *>);
    static_assert(same_as<rng_ptr<n_value<const int> &>, const int *>);
    static_assert(same_as<rng_ptr<void>, range_traits<void>::pointer>);
    static_assert(same_as<rng_ptr<int [3]>, range_traits<int [3]>::pointer>);

    static_assert(same_as<rng_ref<const void>, void>);
    static_assert(same_as<rng_ref<void>, void>);
    static_assert(same_as<rng_ref<int>, void>);
    static_assert(same_as<rng_ref<n_value<int> &&>, const int &>);
    static_assert(same_as<rng_ref<n_value<const int> &>, const int &>);
    static_assert(same_as<rng_ref<void>, range_traits<void>::reference>);
    static_assert(same_as<rng_ref<int [3]>, range_traits<int [3]>::reference>);

    static_assert(same_as<rng_common_ref<const void>, void>);
    static_assert(same_as<rng_common_ref<void>, void>);
    static_assert(same_as<rng_common_ref<int>, void>);
    static_assert(same_as<rng_common_ref<n_value<int> &&>, const int &>);
    static_assert(same_as<rng_common_ref<n_value<const int> &>, const int &>);
    static_assert(same_as
                  <rng_common_ref<void>, range_traits<void>::common_reference>);
    static_assert(same_as
                  <rng_common_ref<int [3]>,
                   range_traits<int [3]>::common_reference>);

    static_assert(same_as<rng_rref<const void>, void>);
    static_assert(same_as<rng_rref<void>, void>);
    static_assert(same_as<rng_rref<int>, void>);
    static_assert(same_as<rng_rref<n_value<int> &&>, const int &&>);
    static_assert(same_as<rng_rref<n_value<const int> &>, const int &&>);
    static_assert(same_as
                  <rng_rref<void>, range_traits<void>::rvalue_reference>);
    static_assert(same_as
                  <rng_rref<int [3]>, range_traits<int [3]>::rvalue_reference>);

    static_assert(same_as<rng_dft<const void>, void>);
    static_assert(same_as<rng_dft<void>, void>);
    static_assert(same_as<rng_dft<int>, void>);
    static_assert(same_as<rng_dft<n_value<int> &&>, ptrdiff_t>);
    static_assert(same_as<rng_dft<n_value<const int> &>, ptrdiff_t>);
    static_assert(same_as
                  <rng_dft<void>, range_traits<void>::difference_type>);
    static_assert(same_as
                  <rng_dft<int [3]>, range_traits<int [3]>::difference_type>);

    static_assert(same_as<rng_ctg<const void>, void>);
    static_assert(same_as<rng_ctg<void>, void>);
    static_assert(same_as<rng_ctg<int>, void>);
    static_assert(same_as
                  <rng_ctg<n_value<int> &&>, random_access_iterator_tag>);
    static_assert(same_as
                  <rng_ctg<n_value<const int> &>, random_access_iterator_tag>);
    static_assert(same_as
                  <rng_ctg<void>, range_traits<void>::iterator_category>);
    static_assert(same_as
                  <rng_ctg<int [3]>, range_traits<int [3]>::iterator_category>);

    static_assert(!rng_is_sized<void>);
    static_assert(rng_is_sized<int [3]>);
    static_assert(rng_is_sized<n_value<int>>);
    static_assert(rng_is_sized<void>
                  == range_traits<void>::is_sized::value);
    static_assert(rng_is_sized<int [3]>
                  == range_traits<int [3]>::is_sized::value);

    static_assert(!rng_is_counted<void>);
    static_assert(!rng_is_counted<int *>);
    static_assert(rng_is_counted<int (&)[3]>);
    static_assert(rng_is_counted<n_value<int>>);
    static_assert(rng_is_counted<void>
                  == range_traits<void>::is_counted::value);
    static_assert(rng_is_counted<int [3]>
                  == range_traits<int [3]>::is_counted::value);

    static_assert(!rng_is_n_value<void>);
    static_assert(rng_is_n_value<n_value<int>>);
    static_assert(rng_is_n_value<const n_value<int>>);
    static_assert(rng_is_n_value<n_value<int> &>);
    static_assert(rng_is_n_value<const n_value<int> &>);
    static_assert(rng_is_n_value<n_value<int> &&>);
    static_assert(rng_is_n_value<const n_value<int> &&>);
    static_assert(rng_is_n_value<void>
                  == range_traits<void>::is_n_value::value);
    static_assert(rng_is_n_value<int [3]>
                  == range_traits<int [3]>::is_n_value::value);

    static_assert(!is_orng<void>);
    static_assert(!is_irng<void>);
    static_assert(!is_frng<void>);
    static_assert(!is_brng<void>);
    static_assert(!is_rrng<void>);
    static_assert(!is_crng<void>);
    static_assert(!is_just_irng<void>);
    static_assert(!is_just_frng<void>);
    static_assert(!is_just_brng<void>);
    static_assert(!is_just_rrng<void>);
    static_assert(!is_orng<int>);
    static_assert(!is_irng<int>);
    static_assert(!is_frng<int>);
    static_assert(!is_brng<int>);
    static_assert(!is_rrng<int>);
    static_assert(!is_crng<int>);
    static_assert(!is_just_irng<int>);
    static_assert(!is_just_frng<int>);
    static_assert(!is_just_brng<int>);
    static_assert(!is_just_rrng<int>);

    struct t0 {
      to_back_iterator<ez_vector<int>> begin() const;
      to_back_iterator<ez_vector<int>> end() const;
    };
    static_assert(is_orng<t0>);

    struct t1 {
      degraded_input_iterator<int *> begin() const;
      degraded_input_iterator<int *> end() const;
    };
    static_assert(is_irng<t1>);
    static_assert(!is_frng<t1>);
    static_assert(is_just_irng<t1>);

    struct t2 {
      degraded_forward_iterator<int *> begin() const;
      degraded_forward_iterator<int *> end() const;
    };
    static_assert(is_frng<t2>);
    static_assert(!is_brng<t2>);
    static_assert(is_just_frng<t2>);

    struct t3 {
      degraded_bidirectional_iterator<int *> begin() const;
      degraded_bidirectional_iterator<int *> end() const;
    };
    static_assert(is_brng<t3>);
    static_assert(!is_rrng<t3>);
    static_assert(is_just_brng<t3>);

    struct t4 {
      degraded_random_access_iterator<int *> begin() const;
      degraded_random_access_iterator<int *> end() const;
    };
    static_assert(is_rrng<t4>);
    static_assert(!is_crng<t4>);
    static_assert(is_just_rrng<t4>);
  }

  // is_iter_pair
  // is_rng_ref
  {
    struct t : private iter_pair<int *> {};
    struct tt : iter_pair<int *> {};
    static_assert(!is_iter_pair<void>);
    static_assert(!is_iter_pair<int>);
    static_assert(!is_iter_pair<int &>);
    static_assert(!is_iter_pair<const int &>);
    static_assert(!is_iter_pair<int &&>);
    static_assert(!is_iter_pair<t>);
    static_assert(is_iter_pair<tt>);
    static_assert(is_iter_pair<tt &>);
    static_assert(is_iter_pair<const tt &>);
    static_assert(is_iter_pair<tt &&>);
    static_assert(is_iter_pair<const tt &&>);

    static_assert(!is_rng_ref<t>);
    static_assert(!is_rng_ref<t &>);
    static_assert(!is_rng_ref<const t &>);
    static_assert(!is_rng_ref<t &&>);
    static_assert(!is_rng_ref<const t &&>);
    static_assert(is_rng_ref<tt>);
    static_assert(is_rng_ref<tt &>);
    static_assert(is_rng_ref<const tt &>);
    static_assert(is_rng_ref<tt &&>);
    static_assert(is_rng_ref<const tt &&>);
    static_assert(is_rng_ref<iter_pair<int *>>);
    static_assert(is_rng_ref<iter_pair<int *> &>);
    static_assert(is_rng_ref<const iter_pair<int *> &>);
    static_assert(is_rng_ref<iter_pair<int *> &&>);
    static_assert(is_rng_ref<const iter_pair<int *> &&>);
    static_assert(!is_rng_ref<ez_vector<int>>);
    static_assert(is_rng_ref<ez_vector<int> &>);
    static_assert(is_rng_ref<const ez_vector<int> &>);
    static_assert(!is_rng_ref<ez_vector<int> &&>);
    static_assert(!is_rng_ref<const ez_vector<int> &&>);
  }

  // rng_forward_t
  {
    {
      using r_t = iter_pair<int *>;
      static_assert(is_rng_ref<r_t>);
      static_assert(same_as<rng_forward_t<r_t>, r_t>);
      static_assert(same_as<rng_forward_t<r_t &>, r_t>);
      static_assert(same_as<rng_forward_t<const r_t &>, r_t>);
      static_assert(same_as<rng_forward_t<r_t &&>, r_t>);
      static_assert(same_as<rng_forward_t<const r_t &&>, r_t>);
    }
    {
      using r_t = ez_vector<int>;
      static_assert(!is_rng_ref<r_t>);
      static_assert(same_as<rng_forward_t<r_t>, r_t>);
      static_assert(same_as<rng_forward_t<r_t &>, r_t &>);
      static_assert(same_as<rng_forward_t<const r_t &>, const r_t &>);
      static_assert(same_as<rng_forward_t<r_t &&>, r_t>);
      static_assert(same_as<rng_forward_t<const r_t &&>, const r_t>);
    }
    {
      using r_t = const int (&)[2];
      static_assert(same_as<rng_forward_t<r_t>, r_t>);
      static_assert(same_as<rng_forward_t<r_t &>, r_t>);
      static_assert(same_as<rng_forward_t<const r_t &>, r_t>);
      static_assert(same_as<rng_forward_t<r_t &&>, r_t>);
      static_assert(same_as<rng_forward_t<const r_t &&>, r_t>);
    }
  }

  // before_begin
  // before_end
  {
    struct t {
      constexpr int before_begin() {return 1;}
      constexpr int before_begin() const {return 2;}
    };
    t x;
    assert(before_begin(x) == 1);
    assert(before_begin(as_const(x)) == 2);
    assert(before_begin(move(x)) == 2);
    static_assert([]() {
      t z;
      return before_begin(z) == 1 && before_begin(as_const(z)) == 2;
    }());

    struct tt {
      constexpr int before_end() {return 1;}
      constexpr int before_end() const {return 2;}
    };
    tt y;
    assert(before_end(y) == 1);
    assert(before_end(as_const(y)) == 2);
    assert(before_end(move(y)) == 2);
    int a[] = {1, 2, 3};
    assert(*before_end(a) == 3);
    assert(*before_end(as_const(a)) == 3);
    assert(*before_end(move(a)) == 3);
    iter_pair<degraded_forward_iterator<int *>> p(begin(a), end(a));
    assert(*before_end(p) == 3);
    assert(*before_end(as_const(p)) == 3);
    assert(*before_end(move(p)) == 3);
    iter_pair<degraded_bidirectional_iterator<int *>> pp(begin(a), end(a));
    assert(*before_end(pp) == 3);
    assert(*before_end(as_const(pp)) == 3);
    assert(*before_end(move(pp)) == 3);
    static_assert([]() {
      tt z;
      int a[] = {1, 2};
      return before_end(z) == 1 && before_end(as_const(z)) == 2
        && *before_end(a) == 2 && *before_end(as_const(a)) == 2;
    }());
  }

  // cbegin
  // cend
  // rbegin
  // rend
  // crbegin
  // crend
  {
    static_assert([]() {
      int a[] = {1, 2};
      return *cbegin(a) == 1 && *prev(cend(a)) == 2;
    }());

    static_assert([]() {
      int a[] = {1, 2};
      return *rbegin(a) == 2 && *prev(rend(a)) == 1
        && *rbegin(as_const(a)) == 2 && *prev(rend(as_const(a))) == 1;
    }());
    static_assert([]() {
      int a[] = {1, 2};
      struct t {
        degraded_bidirectional_iterator<int *> x;
        degraded_bidirectional_iterator<int *> y;

        constexpr auto begin() const {return x;}
        constexpr auto end() const {return y;}
      };
      t z(degraded_bidirectional_iterator<int *>(begin(a)),
          degraded_bidirectional_iterator<int *>(end(a)));
      return *rbegin(z) == 2 && *prev(rend(z)) == 1
        && *rbegin(as_const(z)) == 2 && *prev(rend(as_const(z))) == 1;
    }());

    static_assert([]() {
      int a[] = {1, 2};
      return *crbegin(a) == 2 && *prev(crend(a)) == 1
        && *crbegin(as_const(a)) == 2 && *prev(crend(as_const(a))) == 1;
    }());
    static_assert([]() {
      int a[] = {1, 2};
      struct t {
        degraded_bidirectional_iterator<int *> x;
        degraded_bidirectional_iterator<int *> y;

        constexpr auto begin() const {return x;}
        constexpr auto end() const {return y;}
      };
      t z(degraded_bidirectional_iterator<int *>(begin(a)),
          degraded_bidirectional_iterator<int *>(end(a)));
      return *crbegin(z) == 2 && *prev(crend(z)) == 1
        && *crbegin(as_const(z)) == 2 && *prev(crend(as_const(z))) == 1;
    }());
  }

  // data
  {
    struct t {constexpr int data() const {return 1;}};
    static_assert(data(t{}) == 1);
    static_assert([]() {t x; return data(x) == 1;}());
    static_assert([]() {
      int a[] = {1, 2};
      return *data(a) == 1 && *data(as_const(a)) == 1;
    }());
    static_assert(*data({1, 2, 3}) == 1);
    static_assert([]() {
      int a[] = {1, 2};
      struct t {
        int *i;
        int *j;
        constexpr int *data() {return i;}
        constexpr const int *data() const {return j;}
      };
      t x(begin(a), end(a) - 1);
      return *data(x) == 1 && *data(as_const(x)) == 2
        && *data(move(x)) == 2 && *data(move(as_const(x))) == 2;
    }());
  }

  // ref
  {
    static_assert([]() {
      int a[] = {1, 2, 3};
      return ref(a, 2) == 3 && ref(as_const(a), 2) == 3
        && ref(move(a), 2) == 3 && ref(move(as_const(a)), 2) == 3;
    }());
  }

  // nth
  {
    static_assert([]() {
      int a[] = {1, 2, 3};
      return *nth(a, 2) == 3 && *nth(as_const(a), 2) == 3
        && *nth(move(a), 2) == 3 && *nth(move(as_const(a)), 2) == 3;
    }());
  }
  // at_most_nth
  {
    {
      int a[] = {1, 2, 3};
      for (int i : irng(0, 4)) {
        assert(at_most_nth(a, i) == nth(a, i));
        assert(at_most_nth(as_const(a), i) == nth(a, i));
      }
      assert(at_most_nth(a, 4) == end(a));
      assert(at_most_nth(a, 100) == end(a));
      assert(at_most_nth(as_const(a), 4) == cend(a));
      assert(at_most_nth(as_const(a), 100) == cend(a));
      ez_vector<int> v{};
      assert(at_most_nth(v, 0) == end(v));
      assert(at_most_nth(v, 1) == end(v));
      assert(at_most_nth(v, 10) == end(v));
      assert(at_most_nth(as_const(v), 0) == cend(v));
      assert(at_most_nth(as_const(v), 1) == cend(v));
      assert(at_most_nth(as_const(v), 10) == cend(v));
    }
    {
      struct t {
        ez_vector<int> v;
        auto nth(ptrdiff_t n) {return v.begin() + n;}
        auto nth(ptrdiff_t n) const {return v.begin() + n;}
        auto size() const {return v.size();}
        auto begin() {return v.begin();}
        auto end() {return v.end();}
        auto begin() const {return v.begin();}
        auto end() const {return v.end();}
      };
      t x{};
      x.v = {1, 2, 3};
      static_assert(rng_is_sized<t>);
      for (int &i : iters(0, 4)) {
        assert(at_most_nth(x, i) == nth(x.v, i));
        assert(at_most_nth(as_const(x), i) == nth(as_const(x).v, i));
      }
      assert(at_most_nth(x, 4) == x.v.end());
      assert(at_most_nth(x, 5) == x.v.end());
      assert(at_most_nth(x, 50) == x.v.end());
      assert(at_most_nth(as_const(x), 4) == x.v.end());
      assert(at_most_nth(as_const(x), 5) == x.v.end());
      assert(at_most_nth(as_const(x), 50) == x.v.end());
    }
  }

  // front
  // back
  {
    static_assert([]() {
      int a[] = {1, 2, 3};
      return front(a) == 1 && front(as_const(a)) == 1
        && front(move(a)) == 1 && front(move(as_const(a))) == 1
        && back(a) == 3 && back(as_const(a)) == 3
        && back(move(a)) == 3 && back(move(as_const(a))) == 3;
    }());
    struct t {
      constexpr int front() {return 1;}
      constexpr int back() {return 2;}
    };
    struct tt {
      int x = 2;
      int *begin() {return nullptr;}
      constexpr int *before_end() {return addressof(x);}
    };
    static_assert([]() {
      t x;
      return front(x) == 1 && back(x) == 2;
    }());
    static_assert([]() {
      tt x;
      return back(x) == 2;
    }());
  }

  // range_fns
  {
    constexpr int a[] = {1, 2, 3};
    static_assert(range_fns::begin(a) == begin(a));
    static_assert(range_fns::end(a) == end(a));
    static_assert(range_fns::rbegin(a) == rbegin(a));
    static_assert(range_fns::rend(a) == rend(a));
    static_assert(range_fns::size(a) == size(a));
    static_assert(range_fns::empty(a) == empty(a));
    static_assert(range_fns::nth(a, 1) == nth(a, 1));
    static_assert(range_fns::ref(a, 1) == ref(a, 1));
    static_assert(range_fns::front(a) == front(a));
    static_assert(range_fns::back(a) == back(a));
    static_assert(range_fns::cbegin(a) == cbegin(a));
    static_assert(range_fns::cend(a) == cend(a));
    static_assert(range_fns::crbegin(a) == crbegin(a));
    static_assert(range_fns::crend(a) == crend(a));
    static_assert(range_fns::ssize(a) == ssize(a));
    static_assert(range_fns::data(a) == data(a));
    static_assert(range_fns::before_end(a) == before_end(a));
    struct t {
      constexpr int before_begin() const {return 1;}
    };
    static_assert(range_fns::before_begin(t{}) == before_begin(t{}));
  }
}

void test_degraded_iterator() {
  {
    static_assert([]() {
      int x = 1, y = 2;
      degraded_input_iterator i(addressof(x)), j(addressof(y));
      adl_swap(i, j);
      return *i == 2 && *j == 1;
    }());
    static_assert([]() {
      int x = 1, y = 2;
      degraded_forward_iterator i(addressof(x));
      degraded_forward_iterator j(addressof(y));
      adl_swap(i, j);
      return *i == 2 && *j == 1;
      return true;
    }());
    static_assert([]() {
      int x = 1, y = 2;
      degraded_bidirectional_iterator i(addressof(x)), j(addressof(y));
      adl_swap(i, j);
      return *i == 2 && *j == 1;
    }());
    static_assert([]() {
      int x = 1, y = 2;
      degraded_random_access_iterator i(addressof(x)), j(addressof(y));
      adl_swap(i, j);
      return *i == 2 && *j == 1;
    }());
  }
  {
    using i = degraded_iterator<int *, input_iterator_tag>;
    static_assert(regular<i>);
    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(!forward_iterator<i>);
    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(!cpp17_forward_iterator<i>);
    static_assert(!cpp17_bidirectional_iterator<i>);
    static_assert(can_apply_to_address<const i &>);
    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, input_iterator_tag>);

    using ci = degraded_iterator<const int *, input_iterator_tag>;
    static_assert(same_as<itr_vt<ci>, int>);
    static_assert(same_as<itr_ref<ci>, const int &>);
    static_assert(same_as<itr_common_ref<ci>, const int &>);
    static_assert(same_as<itr_rref<ci>, const int &&>);
    static_assert(same_as<itr_ptr<ci>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<ci>, const int *>);
    static_assert(same_as<itr_dft<ci>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<ci>, input_iterator_tag>);
    static_assert(common_with<i, ci>);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    static_assert(equality_comparable_with<i, ci>);
    static_assert(!three_way_comparable_with<i, ci>);
    static_assert(can_apply_minus<i, i>);
    static_assert(can_apply_minus<ci, ci>);
    static_assert(can_apply_minus<i, ci>);
    static_assert(can_apply_minus<ci, i>);
    static_assert(itr_is_counted<i> && itr_is_counted<ci>);

    int a[] = {1, 2, 3, 4, 5};
    test_rng(rng(i(begin(a)), i(end(a))), a);
    test_rng(rng(ci(begin(a)), ci(end(a))), a);
    test_rng(rng(i(begin(a)), ci(end(a))), a);
    test_rng(rng(ci(begin(a)), i(end(a))), a);
  }
  {
    using i = degraded_iterator<int *, forward_iterator_tag>;
    static_assert(regular<i>);
    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(!bidirectional_iterator<i>);
    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(!cpp17_bidirectional_iterator<i>);
    static_assert(can_apply_to_address<const i &>);
    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, forward_iterator_tag>);

    using ci = degraded_iterator<const int *, forward_iterator_tag>;
    static_assert(same_as<itr_vt<ci>, int>);
    static_assert(same_as<itr_ref<ci>, const int &>);
    static_assert(same_as<itr_common_ref<ci>, const int &>);
    static_assert(same_as<itr_rref<ci>, const int &&>);
    static_assert(same_as<itr_ptr<ci>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<ci>, const int *>);
    static_assert(same_as<itr_dft<ci>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<ci>, forward_iterator_tag>);
    static_assert(common_with<i, ci>);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    static_assert(equality_comparable_with<i, ci>);
    static_assert(!three_way_comparable_with<i, ci>);
    static_assert(can_apply_minus<i, i>);
    static_assert(can_apply_minus<ci, ci>);
    static_assert(can_apply_minus<i, ci>);
    static_assert(can_apply_minus<ci, i>);
    static_assert(itr_is_counted<i> && itr_is_counted<ci>);

    int a[] = {1, 2, 3, 4, 5};
    test_fitr(i(begin(a)), i(end(a)), a);
    test_fitr(ci(begin(a)), ci(end(a)), a);
    test_fitr(i(begin(a)), ci(end(a)), a);
    test_fitr(ci(begin(a)), i(end(a)), a);
  }
  {
    using i = degraded_iterator<int *, bidirectional_iterator_tag>;
    static_assert(regular<i>);
    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(bidirectional_iterator<i>);
    static_assert(!random_access_iterator<i>);
    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(cpp17_bidirectional_iterator<i>);
    static_assert(!cpp17_random_access_iterator<i>);
    static_assert(can_apply_to_address<const i &>);
    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, bidirectional_iterator_tag>);

    using ci = degraded_iterator<const int *, bidirectional_iterator_tag>;
    static_assert(same_as<itr_vt<ci>, int>);
    static_assert(same_as<itr_ref<ci>, const int &>);
    static_assert(same_as<itr_common_ref<ci>, const int &>);
    static_assert(same_as<itr_rref<ci>, const int &&>);
    static_assert(same_as<itr_ptr<ci>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<ci>, const int *>);
    static_assert(same_as<itr_dft<ci>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<ci>, bidirectional_iterator_tag>);
    static_assert(common_with<i, ci>);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    static_assert(equality_comparable_with<i, ci>);
    static_assert(!three_way_comparable_with<i, ci>);
    static_assert(can_apply_minus<i, i>);
    static_assert(can_apply_minus<ci, ci>);
    static_assert(can_apply_minus<i, ci>);
    static_assert(can_apply_minus<ci, i>);
    static_assert(itr_is_counted<i> && itr_is_counted<ci>);

    int a[] = {1, 2, 3, 4, 5};
    test_bitr(i(begin(a)), i(end(a)), a);
    test_bitr(ci(begin(a)), ci(end(a)), a);
    test_bitr(i(begin(a)), ci(end(a)), a);
    test_bitr(ci(begin(a)), i(end(a)), a);
  }
  {
    using i = degraded_iterator<int *, random_access_iterator_tag>;
    static_assert(regular<i>);
    static_assert(input_or_output_iterator<i>);
    static_assert(input_iterator<i>);
    static_assert(forward_iterator<i>);
    static_assert(bidirectional_iterator<i>);
    static_assert(random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);
    static_assert(cpp17_iterator<i>);
    static_assert(cpp17_input_iterator<i>);
    static_assert(cpp17_forward_iterator<i>);
    static_assert(cpp17_bidirectional_iterator<i>);
    static_assert(cpp17_random_access_iterator<i>);
    static_assert(can_apply_to_address<const i &>);
    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, random_access_iterator_tag>);

    using ci = degraded_iterator<const int *, random_access_iterator_tag>;
    static_assert(same_as<itr_vt<ci>, int>);
    static_assert(same_as<itr_ref<ci>, const int &>);
    static_assert(same_as<itr_common_ref<ci>, const int &>);
    static_assert(same_as<itr_rref<ci>, const int &&>);
    static_assert(same_as<itr_ptr<ci>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<ci>, const int *>);
    static_assert(same_as<itr_dft<ci>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<ci>, random_access_iterator_tag>);
    static_assert(common_with<i, ci>);
    static_assert(convertible_to<i, ci>);
    static_assert(!convertible_to<ci, i>);
    static_assert(equality_comparable_with<i, ci>);
    static_assert(three_way_comparable_with<i, ci>);
    static_assert(can_apply_minus<i, i>);
    static_assert(can_apply_minus<ci, ci>);
    static_assert(can_apply_minus<i, ci>);
    static_assert(can_apply_minus<ci, i>);
    static_assert(itr_is_counted<i> && itr_is_counted<ci>);

    int a[] = {1, 2, 3, 4, 5};
    test_ritr(i(begin(a)), i(end(a)), a);
    test_ritr(ci(begin(a)), ci(end(a)), a);
    test_ritr(i(begin(a)), ci(end(a)), a);
    test_ritr(ci(begin(a)), i(end(a)), a);

    static_assert([]() {
      int aa[] = {1, 2, 3};
      test_ritr(i(begin(aa)), i(end(aa)), aa);
      return true;
    }());
  }
  {
    int a[] = {1, 2, 3};
    static_assert(same_as
                  <decltype(degraded_iitr(begin(a))),
                   degraded_input_iterator<int *>>);
    static_assert(same_as
                  <decltype(degraded_iitr(degraded_ritr(begin(a)))),
                   degraded_input_iterator
                   <degraded_random_access_iterator<int *>>>);
    int aa[] = {1, 2, 3};
    test_fitr(degraded_fitr(degraded_bitr(begin(a))),
              degraded_fitr(degraded_bitr(end(a))),
              aa);
    test_fitr(degraded_fitr(begin(a)), degraded_fitr(end(a)), aa);
    test_itr_minus(degraded_bitr(begin(a)),
                   degraded_bitr(end(a)),
                   size(aa));
    test_itr_minus(degraded_fitr(degraded_bitr(begin(a))),
                   degraded_fitr(degraded_bitr(end(a))),
                   size(aa));
    test_itr_minus(degraded_fitr(begin(a)), degraded_fitr(end(a)), size(aa));
  }
}
void test_reverse_iterator() {
  {
    using i = reverse_iterator<int *>;
    using ci = reverse_iterator<const int *>;
    static_assert(regular<i>);
    static_assert(regular<ci>);
    static_assert(swappable<i>);
    static_assert(swappable<ci>);
    static_assert(random_access_iterator<i>);
    static_assert(!contiguous_iterator<i>);
    static_assert(cpp17_random_access_iterator<i>);
    static_assert(convertible_to<i, ci> && !convertible_to<ci, i>);
    static_assert(can_apply_minus<const i &, const ci &>);
    static_assert(can_apply_minus<const ci &, const i &>);
    static_assert(same_as<decltype(declval<i>() - declval<ci>()), ptrdiff_t>);
    static_assert(equality_comparable<i>);
    static_assert(equality_comparable<ci>);
    static_assert(equality_comparable_with<i, ci>);
    static_assert(three_way_comparable<i>);
    static_assert(three_way_comparable<ci>);
    static_assert(three_way_comparable_with<i, ci>);
    static_assert(is_just_ritr<i>);
    static_assert(same_as<itr_vt<i>, int>);
    static_assert(same_as<itr_vt<ci>, int>);
    static_assert(same_as<itr_ref<i>, int &>);
    static_assert(same_as<itr_ref<ci>, const int &>);
    static_assert(same_as<itr_common_ref<i>, int &>);
    static_assert(same_as<itr_common_ref<ci>, const int &>);
    static_assert(same_as<itr_rref<i>, int &&>);
    static_assert(same_as<itr_rref<ci>, const int &&>);
    static_assert(same_as<itr_ptr<i>, int *>);
    static_assert(same_as<itr_ptr<ci>, const int *>);
    static_assert(same_as<itr_ptr_from_ref<i>, int *>);
    static_assert(same_as<itr_ptr_from_ref<ci>, const int *>);
    static_assert(same_as<itr_dft<i>, ptrdiff_t>);
    static_assert(same_as<itr_dft<ci>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<i>, random_access_iterator_tag>);
    static_assert(same_as<itr_ctg<ci>, random_access_iterator_tag>);
    static_assert(itr_is_counted<i>);
    static_assert(itr_is_counted<ci>);

    int a0[] = {5, 4, 3, 2, 1};
    int a[] = {1, 2, 3, 4, 5};
    reverse_iterator x(end(a)), y(begin(a));
    reverse_iterator cx(cend(a)), cy(cbegin(a));
    test_ritr(x, y, a0);
    test_ritr(cx, cy, a0);
    test_ritr(x, cy, a0);
    test_ritr(cx, y, a0);
    static_assert([]() {
      int aa[] = {1, 2, 3};
      int aaa[] = {3, 2, 1};
      test_ritr(rbegin(aa), rend(aa), aaa);
      return true;
    }());
  }
  {
    int ra[] = {5, 4, 3, 2, 1};
    int a[] = {1, 2, 3, 4, 5};
    degraded_bidirectional_iterator<int *> i(begin(a)), j(end(a));
    degraded_bidirectional_iterator<const int *> ci(begin(a)), cj(end(a));
    reverse_iterator ii(j), jj(i);
    reverse_iterator cii(cj), cjj(ci);
    test_bitr(ii, jj, ra);
    test_bitr(cii, cjj, ra);
    test_bitr(ii, cjj, ra);
    test_bitr(cii, jj, ra);
    using rit = decltype(ii);
    using crit = decltype(cii);
    static_assert(regular<rit>);
    static_assert(regular<crit>);
    static_assert(swappable<rit>);
    static_assert(swappable<crit>);
    static_assert(bidirectional_iterator<rit>);
    static_assert(!random_access_iterator<rit>);
    static_assert(bidirectional_iterator<crit>);
    static_assert(!random_access_iterator<crit>);
    static_assert(convertible_to<rit, crit> && !convertible_to<crit, rit>);
    static_assert(can_apply_minus<const rit &, const crit &>);
    static_assert(can_apply_minus<const crit &, const rit &>);
    static_assert(same_as
                  <decltype(declval<rit>() - declval<crit>()), ptrdiff_t>);
    static_assert(equality_comparable<rit>);
    static_assert(equality_comparable<crit>);
    static_assert(equality_comparable_with<rit, crit>);
    static_assert(cpp17_bidirectional_iterator<rit>);
    static_assert(cpp17_bidirectional_iterator<crit>);
    static_assert(is_just_bitr<rit>);
    static_assert(is_just_bitr<crit>);
    static_assert(same_as<itr_vt<rit>, int>);
    static_assert(same_as<itr_vt<crit>, int>);
    static_assert(same_as<itr_ref<rit>, int &>);
    static_assert(same_as<itr_ref<crit>, const int &>);
    static_assert(same_as<itr_common_ref<rit>, int &>);
    static_assert(same_as<itr_common_ref<crit>, const int &>);
    static_assert(same_as<itr_rref<rit>, int &&>);
    static_assert(same_as<itr_rref<crit>, const int &&>);
    static_assert(same_as<itr_ptr<rit>, decltype(i)>);
    static_assert(same_as<itr_ptr<crit>, decltype(ci)>);
    static_assert(same_as<itr_ptr_from_ref<rit>, int *>);
    static_assert(same_as<itr_ptr_from_ref<crit>, const int *>);
    static_assert(same_as<itr_dft<rit>, ptrdiff_t>);
    static_assert(same_as<itr_dft<crit>, ptrdiff_t>);
    static_assert(same_as<itr_ctg<rit>, bidirectional_iterator_tag>);
    static_assert(same_as<itr_ctg<crit>, bidirectional_iterator_tag>);
    static_assert(itr_is_counted<rit>);
    static_assert(itr_is_counted<crit>);
  }
  {
    static_assert(same_as
                  <decltype(ritr(declval<reverse_iterator<int *>>())),
                   int *>);
   static_assert(same_as
                  <decltype(ritr(declval<const reverse_iterator<int *> &>())),
                   int *>);
  }
}
void test_insert_iterator() {
  // insert_iterator
  {
    using oiter_t = insert_iterator<ez_vector<int>>;
    static_assert(output_iterator<oiter_t, int>);
    ez_vector<int> v;
    oiter_t o(v, v.end());
    *o++ = 1;
    {
      int tmp = 2;
      *o++ = tmp;
    }
    {
      short tmp = 3;
      *o++ = tmp;
    }
    assert(v.begin()[0] == 1 && v.begin()[1] == 2 && v.begin()[2] == 3);
    assert(v.size() == 3);
  }

  // back_insert_iterator
  // front_insert_iterator
  struct vt {
    ez_vector<int> v;
    using value_type = int;
    void push_front(const int &x) {v.insert(v.begin(), x);}
    void push_front(int &&x) {v.insert(v.begin(), move(x));}
    void push_back(const int &x) {v.insert(v.end(), x);}
    void push_back(int &&x) {v.insert(v.end(), move(x));}
  };
  {
    using oiter_t = back_insert_iterator<vt>;
    static_assert(output_iterator<oiter_t, int>);
    vt v;
    oiter_t o(v);
    *o++ = 1;
    {
      int tmp = 2;
      *o++ = tmp;
    }
    {
      short tmp = 3;
      *o++ = tmp;
    }
    assert(v.v.begin()[0] == 1 && v.v.begin()[1] == 2 && v.v.begin()[2] == 3);
    assert(v.v.size() == 3);
  }
  {
    using oiter_t = front_insert_iterator<vt>;
    static_assert(output_iterator<oiter_t, int>);
    vt v;
    oiter_t o(v);
    *o++ = 1;
    {
      int tmp = 2;
      *o++ = tmp;
    }
    {
      short tmp = 3;
      *o++ = tmp;
    }
    assert(v.v.begin()[0] == 3 && v.v.begin()[1] == 2 && v.v.begin()[2] == 1);
    assert(v.v.size() == 3);
  }

  // output_itr
  {
    ez_vector<int> v;
    auto o = output_itr([&](int x) {v.insert(v.end(), x);});
    *o++ = 1;
    *o++ = 2;
    *o++ = 3;
    *o++ = 4;
    assert(v == ez_vector<int>({1, 2, 3, 4}));
  }

  const auto copy = [](auto &&r, auto o) {
    const auto ed = end(r);
    for (auto it = begin(r); it != ed; ++it) {
      *o = *it;
      ++o;
    }
    return o;
  };
  // to_front(v)
  {
    ez_vector<int> v;
    auto it = to_front(v);
    it = copy(irng(0, 4), it);
    test_rng(v, irng(0, 4));
    copy(rng(3, 0), it);
    test_rng(v, ez_vector{0, 1, 2, 3, 0, 0, 0});
  }
  {
    ez_vector<int> v = copy(irng(1, 4), to_front(ez_vector{4})).base();
    test_rng(v, irng(1, 5));
    ez_forward_list<int> l
      = copy(irng(1, 4), to_front(ez_forward_list{4})).base();
    test_rng(l, irng(1, 5));
  }
  // to_back(v)
  {
    {
      ez_vector<int> v;
      auto it = to_back(v);
      it = copy(irng(1, 4), it);
      copy(irng(4, 6), it);
      test_rng(v, irng(1, 6));
    }
    {
      ez_vector<int> v = copy(irng(1, 4), to_back(ez_vector{0})).base();
      test_rng(v, irng(0, 4));
    }
    {
      struct t {
        ez_vector<int> v;
        void push_back(int x) {v.insert(v.end(), x);}
      };
      t x = copy(irng(1, 4), to_back(t{})).base();
      test_rng(x.v, irng(1, 4));
    }
    {
      struct t {
        ez_vector<int> v;
        void insert(int x) {v.insert(v.end(), x);}
      };
      t x = copy(irng(1, 4), to_back(t{})).base();
      test_rng(x.v, irng(1, 4));
    }
  }
}
void test_move_iterator() {
  {
    using it = move_iterator<int *>;
    static_assert(same_as<it::iterator_type, int *>);
    static_assert(same_as<it::value_type, int>);
    static_assert(same_as<it::reference, int &&>);
    static_assert(same_as<it::pointer, int *>);
    static_assert(same_as<it::difference_type, ptrdiff_t>);
    static_assert(is_citr<it>);

    int x = 1;
    it i(addressof(x));
    assert(*i == 1);
    assert(i.operator ->() == addressof(x));
    static_assert(same_as<decltype(i.operator ->()), int *>);

    int a[] = {1, 2, 3};
    int b[] = {1, 2, 3};
    test_ritr(it(begin(a)), it(end(a)), b);
  }
  {
    using vt = ez_vector<int>;
    vt a[] = {vt{1}, vt{2}, vt{3}};
    vt b[3];
    assert(a[0] == vt{1});
    assert(a[1] == vt{2});
    assert(a[2] == vt{3});
    for (auto &v : b)
      assert(empty(v));

    vt *o = begin(b);
    for (auto &i : iters(make_move_iterator(begin(a)), move_itr(end(a))))
      *o++ = *i;
    for (auto &v : a)
      assert(empty(v));
    assert(b[0] == vt{1});
    assert(b[1] == vt{2});
    assert(b[2] == vt{3});
  }
}
void test_counted_iterator() {
  using it = counted_iterator<int *>;
  static_assert(regular<it>);
  static_assert(three_way_comparable<it>);
  static_assert(is_ritr<it>);
  static_assert(random_access_iterator<it>);
  static_assert(cpp17_random_access_iterator<it>);
  static_assert(same_as<it::iterator_type, int *>);
  static_assert(same_as<it::value_type, int>);
  static_assert(same_as<it::reference, int &>);
  static_assert(same_as<it::pointer, int *>);
  static_assert(same_as<it::difference_type, ptrdiff_t>);
  static_assert(same_as<it::iterator_category, contiguous_iterator_tag>);
  static_assert(itr_is_counted<it>);

  int a[] = {1, 2, 3};
  it i(begin(a), 1);
  it j(begin(a) + 1, 0);
  test_equality(i.base(), begin(a));
  assert(i.count() == 1);
  assert(*i.operator ->() == 1);
  test_equality(i, i);
  {
    int aa[] = {1};
    test_ritr(i, j, aa);
  }

  i = {begin(a), 3};
  j = {begin(a) + 2, 1};
  {
    int aa[] = {1, 2};
    test_ritr(i, j, aa);
  }

  {
    int aa[] = {1, 2, 3};
    test_ritr(counted_iterator(begin(a), 3),
              counted_iterator(end(a), 0), aa);
    test_ritr(counted_iterator(begin(a), 3),
              counted_iterator(cend(a), 0), aa);
    test_ritr(counted_iterator(cbegin(a), 3),
              counted_iterator(end(a), 0), aa);
    test_ritr(counted_iterator(cbegin(a), 3),
              counted_iterator(cend(a), 0), aa);
    test_ritr(counted_itr(begin(a), 3),
              counted_itr(end(a), 0), aa);
    test_ritr(make_counted_iterator(begin(a), 3),
              make_counted_iterator(end(a), 0), aa);
  }
  {
    ez_vector<int> v;
    auto o = inserter(v, v.end());
    *o++ = 1;
    *o++ = 2;
    assert(v == ez_vector({1, 2}));
  }
}

void test_array() {
  // compile-time info
  {
    // member types
    {
      {
        using t = array<int, 3>;
        static_assert(same_as<t::pointer, int *>);
        static_assert(same_as<t::const_pointer, const int *>);
        static_assert(same_as<t::value_type, int>);
        static_assert(same_as<t::reference, int &>);
        static_assert(same_as<t::const_reference, const int &>);
        static_assert(same_as<t::iterator, int *>);
        static_assert(same_as<t::const_iterator, const int *>);
        static_assert(same_as<t::difference_type, ptrdiff_t>);
        static_assert(same_as<t::size_type, size_t>);
        static_assert(same_as<t::reverse_iterator, reverse_iterator<int *>>);
        static_assert(same_as
                      <t::const_reverse_iterator,
                       reverse_iterator<const int *>>);
      }
      {
        using t = array<int, 0>;
        static_assert(same_as<t::pointer, int *>);
        static_assert(same_as<t::const_pointer, const int *>);
        static_assert(same_as<t::value_type, int>);
        static_assert(same_as<t::reference, int &>);
        static_assert(same_as<t::const_reference, const int &>);
        static_assert(same_as<t::iterator, int *>);
        static_assert(same_as<t::const_iterator, const int *>);
        static_assert(same_as<t::difference_type, ptrdiff_t>);
        static_assert(same_as<t::size_type, size_t>);
        static_assert(same_as<t::reverse_iterator, reverse_iterator<int *>>);
        static_assert(same_as
                      <t::const_reverse_iterator,
                       reverse_iterator<const int *>>);
      }
    }
    // special member functions
    {
      using t1 = array<int, 3>;
      using t2 = array<int, 0>;
      static_assert(regular<t1>);
      static_assert(regular<t2>);
      static_assert(is_trivially_default_constructible_v<t1>);
      static_assert(is_trivially_default_constructible_v<t2>);
      static_assert(is_trivially_copyable_v<t1>);
      static_assert(is_trivially_copyable_v<t2>);
      static_assert(is_default_constructible_v<t1>);
      static_assert(is_default_constructible_v<t2>);
      static_assert(is_nothrow_copyable_v<t1>);
      static_assert(is_nothrow_copyable_v<t2>);
      static_assert(is_nothrow_swappable_v<t1>);
      static_assert(is_nothrow_swappable_v<t2>);
    }
  }
  // special member functions
  {
    {
      array<int, 0> x, y;
      adl_swap(x, y);
      assert(x.empty() && y.empty());
    }
    {
      array<int, 3> x = {1, 2, 3};
      array<int, 3> y = {4, 5, 6};
      test_rng(x, irng(1, 4));
      test_rng(y, irng(4, 7));
      adl_swap(x, x);
      test_rng(x, irng(1, 4));
      adl_swap(x, y);
      test_rng(x, irng(4, 7));
      test_rng(y, irng(1, 4));
    }
  }
  // compare
  {
    {
      array<int, 0> x, y;
      test_equality(x, y);
    }
    {
      array<int, 3> x = {1, 2, 3}, y = {4, 5, 6};
      test_lessness(x, y);
      x = {1, 1, 2};
      y = {1, 1, 3};
      test_lessness(x, y);
      x = {1, 2, 5};
      y = {1, 5, 2};
      test_lessness(x, y);
      x = {1, 2, 3};
      y = {1, 2, 3};
      test_equality(x, y);
    }
  }
  // range operations
  {
    array<int, 0> z;
    array<int, 3> x = {1, 2, 3};

    // begin
    // end
    {
      test_rng(z, empty_rng<int>());
      test_rng(as_const(z), empty_rng<int>());
      test_rng(x, irng(1, 4));
      test_rng(as_const(x), irng(1, 4));
    }

    // empty
    {
      assert(z.empty());
      assert(!x.empty());
    }

    // size
    // max_size
    {
      assert(z.size() == 0);
      assert(z.max_size() == 0);
      assert(x.size() == 3);
      assert(x.max_size() == 3);
    }

    // rbegin
    // rend
    {
      assert(empty(rng(z.rbegin(), z.rend())));
      test_rng(rng(x.rbegin(), x.rend()), rrng(irng(1, 4)));
    }

    // front
    // back
    {
      static_assert(same_as<decltype(z.front()), int &>);
      static_assert(same_as<decltype(as_const(z).front()), const int &>);
      static_assert(same_as<decltype(z.back()), int &>);
      static_assert(same_as<decltype(as_const(z).back()), const int &>);
      assert(x.front() == 1 && x.back() == 3);
      assert(as_const(x).front() == 1 && as_const(x).back() == 3);
    }

    // []
    // at
    {
      static_assert(same_as<decltype(z[0]), int &>);
      static_assert(same_as<decltype(as_const(z)[0]), const int &>);
      assert(x.at(0) == 1 && x.at(2) == 3);
      assert(x[0] == 1 && x[2] == 3);
#ifndef RE_NOEXCEPT
      assert(test_throwing<out_of_range>([&]() {z.at(0);}));
      assert(test_throwing<out_of_range>([&]() {x.at(3);}));
#endif
    }
  }
  // data
  {
    array<int, 0> z;
    array<int, 3> x = {1, 2, 3};
    assert(z.data() == z.begin());
    assert(as_const(z).data() == z.begin());
    assert(x.data() == x.begin());
    assert(as_const(x).data() == x.begin());
  }
  // fill
  {
    array<int, 0> z;
    array<int, 3> x = {1, 2, 3};
    z.fill(1);
    x.fill(1);
    assert(z.empty());
    test_rng(rng(3, 1), x);
  }
  // get
  {
    array<int, 3> x = {1, 2, 3};
    assert(get<0>(x) == 1);
    assert(get<1>(x) == 2);
    assert(get<2>(x) == 3);
    assert(get<0>(as_const(x)) == 1);
    assert(get<1>(as_const(x)) == 2);
    assert(get<2>(as_const(x)) == 3);
    assert(get<0>(move(x)) == 1);
    assert(get<0>(move(as_const(x))) == 1);
  }
  // to_array
  {
    ez_vector<int> a[2] = {{1, 2}, {3, 4}};
    auto b = to_array(a);
    assert(a[0] == ez_vector<int>({1, 2}));
    assert(a[1] == ez_vector<int>({3, 4}));
    assert(b[0] == a[0]);
    assert(b[1] == a[1]);
    auto c = to_array(move(a));
    assert(a[0].empty());
    assert(a[1].empty());
    assert(c[0] == ez_vector<int>({1, 2}));
    assert(c[1] == ez_vector<int>({3, 4}));
  }
  // deduction guide
  {
    auto a = array{1, 2, 3};
    static_assert(same_as<decltype(a), array<int, 3>>);
  }
  // seq
  {
    test_rng(seq(1, 2, 3), array{1, 2, 3});
    test_rng(seq(1), array{1});
  }
}
void test_iter_pair() {
  // tuple_element<iter_pair<I>>
  // tuple_size<iter_pair<I>>
  // is_rng_ref<iter_pair>
  {
    using it = iter_pair<int *>;
    static_assert(is_convertible_v<it, pair<int *, int *>>);
    static_assert(same_as<tuple_element_t<0, it>, int *>);
    static_assert(same_as<tuple_element_t<1, it>, int *>);
    static_assert(tuple_size_v<it> == 2);
    static_assert(same_as<tuple_element_t<0, const volatile it>,
                  int *const volatile>);
    static_assert(tuple_size_v<const volatile it> == 2);

    static_assert(!is_rng_ref<int>);
    static_assert(is_rng_ref<iter_pair<nullptr_t>>);
    static_assert(is_rng_ref<iter_pair<int *>>);
    static_assert(is_rng_ref<iter_pair<int *> &>);
    static_assert(is_rng_ref<const iter_pair<int *> &>);
    static_assert(is_rng_ref<iter_pair<int *> &&>);
    static_assert(is_rng_ref<const iter_pair<int *> &&>);
    struct t : iter_pair<int *> {};
    static_assert(is_rng_ref<t>);
    static_assert(is_rng_ref<t &>);
    static_assert(is_rng_ref<const t &>);
    static_assert(is_rng_ref<t &>);
    static_assert(is_rng_ref<const t &>);

    static_assert(is_convertible_v<iter_pair<int *>, pair<int *, int *>>);
    static_assert(is_base_of_v<pair<int *, int *>, iter_pair<int *>>);
  }
  // smf
  // iter_pair(i, ii)
  // ==
  {
    int x = 1;
    int y = 2;
    iter_pair<int *> xx(addressof(x), addressof(x) + 1);
    assert(xx.first == addressof(x));
    assert(xx.begin() == addressof(x));
    assert(as_const(xx).begin() == addressof(x));
    assert(x == 1 && y == 2);

    iter_pair<int *> yy(xx);
    assert(xx.first == yy.first && xx.second == yy.second);
    assert(*xx.begin() == 1 && *yy.begin() == 1);
    assert(x == 1 && y == 2);
    test_equality(xx, yy);

    iter_pair<const int *> zz(addressof(y), addressof(y) + 1);
    test_inequality(xx, zz);
    assert(addressof(yy = iter_pair<int *>(addressof(y), addressof(y) + 1))
           == addressof(yy));
    test_equality(yy, zz);
    assert(*yy.begin() == 2 && *zz.begin() == 2);
    assert(x == 1 && y == 2);

    assert(*xx.begin() == 1 && *yy.begin() == 2);
    adl_swap(xx, yy);
    assert(*xx.begin() == 2 && *yy.begin() == 1);
    adl_swap(xx, yy);
    assert(*xx.begin() == 1 && *yy.begin() == 2);

    assert(addressof(xx = yy) == addressof(xx));
    assert(*xx.begin() == 2 && *yy.begin() == 2);
  }
  // begin
  // end
  // empty
  // size
  {
    int a[] = {1, 2, 3};
    iter_pair<int *> i(begin(a), end(a));
    iter_pair<const int *> j = i;
    test_equality(i, j);
    assert(i.size() == 3);
    assert(!i.empty());
    assert(ref(i, 0) == 1);
    assert(ref(i, 1) == 2);
    assert(ref(i, 2) == 3);
    static_assert(same_as<rng_szt<decltype(i)>, size_t>);

    iter_pair<move_iterator<int *>> k(move_itr(begin(a)), move_itr(end(a)));
    assert(i.begin() == k.begin().base());
    assert(i.end() == k.end().base());
    static_assert(rng_is_sized<decltype(k)>);
    assert(i.size() == k.size());
    assert(i.empty() == k.empty());

    ez_forward_list l = {1, 2};
    static_assert(!rng_is_sized<decltype(l)>);
    iter_pair<decltype(l)::iterator> lp(l.begin(), l.end());
    static_assert(!rng_is_sized<decltype(lp)>);
    test_rng(lp, ez_vector{1, 2});
    test_rng(lp, l);
  }
  // iter_pair()->iter_pair
  // iter_pair(i1, i2)->iter_pair
  {
    iter_pair p{};
    static_assert(same_as<decltype(p), iter_pair<nullptr_t>>);
    assert(p.begin() == p.end() && p.empty() && p.size() == 0);

    const int x = 1;
    iter_pair xx(addressof(x), addressof(x) + 1);
    static_assert(same_as<decltype(xx), iter_pair<const int *>>);
    assert(xx.size() == 1);
    assert(ref(xx, 0) == 1);
  }
  // rng(i1, i2)
  // rng(r)
  {
    int a[] = {1, 2};
    const auto r1 = rng(begin(a), end(a));
    const auto r2 = rng(cbegin(a), end(a));
    const auto r3 = rng(begin(a), cend(a));
    const auto r4 = rng(cbegin(a), cend(a));
    static_assert(same_as<decltype(r1), const iter_pair<int *>>);
    static_assert(same_as<decltype(r2), const iter_pair<const int *>>);
    static_assert(same_as<decltype(r3), const iter_pair<const int *>>);
    static_assert(same_as<decltype(r4), const iter_pair<const int *>>);
    test_rng(r1, a);
    test_rng(r2, a);
    test_rng(r3, a);
    test_rng(r4, a);
    test_rng(a, ez_vector{1, 2});

    const auto rr = rng(r1);
    static_assert(same_as<decltype(rr), const iter_pair<int *>>);
    test_rng(r1, a);
    test_rng(rr, a);
    test_rng(a, ez_vector{1, 2});
  }
}
void test_composite_range() {
  auto make_irng = [](int a, int b) {
    return composite_irng(a, b, copy, [](auto &x) {++x;}, equal_to{});
  };
  auto make_frng = [](int a, int b) {
    return composite_frng(a, b, copy, [](auto &x) {++x;}, equal_to{});
  };
  auto make_brng = [](int a, int b) {
    return composite_brng(a, b, copy, [](auto &x) {++x;}, equal_to{},
                          [](auto &x) {--x;});
  };

  assert(empty(make_irng(0, 0)));
  assert(empty(make_irng(1, 1)));
  test_rng(make_irng(0, 3), irng(0, 3));
  test_rng(make_irng(3, 11), irng(3, 11));
  assert(empty(make_frng(0, 0)));
  assert(empty(make_frng(1, 1)));
  test_fitr(make_frng(0, 3), irng(0, 3));
  test_fitr(make_frng(3, 11), irng(3, 11));
  assert(empty(make_brng(0, 0)));
  assert(empty(make_brng(1, 1)));
  test_bitr(make_brng(0, 3), irng(0, 3));
  test_bitr(make_brng(3, 11), irng(3, 11));

  int z{};
  auto make_irng2 = [&](int x, int y) {
    return composite_irng(x, y,
                          [&z](int u) {return u;},
                          [&z](auto &u) {++u;},
                          [&z](auto a, auto b) {return a == b;});
  };
  auto make_frng2 = [&](int x, int y) {
    return composite_frng(x, y,
                          [&z](int u) {return u;},
                          [&z](auto &u) {++u;},
                          [&z](auto a, auto b) {return a == b;});
  };
  auto make_brng2 = [&](int x, int y) {
    return composite_brng(x, y,
                          [&z](int u) {return u;},
                          [&z](auto &u) {++u;},
                          [&z](auto a, auto b) {return a == b;},
                          [&z](auto &u) {--u;});
  };
  assert(empty(make_irng2(0, 0)));
  assert(empty(make_irng2(1, 1)));
  test_rng(make_irng2(0, 3), irng(0, 3));
  test_rng(make_irng2(3, 11), irng(3, 11));
  assert(empty(make_frng2(0, 0)));
  assert(empty(make_frng2(1, 1)));
  test_fitr(make_frng2(0, 3), irng(0, 3));
  test_fitr(make_frng2(3, 11), irng(3, 11));
  assert(empty(make_brng2(0, 0)));
  assert(empty(make_brng2(1, 1)));
  test_bitr(make_brng2(0, 3), irng(0, 3));
  test_bitr(make_brng2(3, 11), irng(3, 11));
}
void test_iterator_wrapper() {
  {
    const ez_vector v = {1, 2, 3, 4, 5};
    const ez_vector<int> z = {};
    test_ritr(wrap_itr(begin(v)), wrap_itr(end(v)), v);
    test_ritr(wrap_itr(begin(z)), wrap_itr(end(z)), empty_rng<int>());
  }
  {
    const ez_vector v = {1, 2, 3, 4, 5};
    const ez_list l = {1, 2, 3, 4, 5};
    const ez_list<int> z = {};
    test_bitr(wrap_itr(begin(l)), wrap_itr(end(l)), v);
    test_bitr(wrap_itr(begin(z)), wrap_itr(end(z)), empty_rng<int>());
  }
  {
    const ez_vector v = {1, 2, 3, 4, 5};
    const ez_forward_list l = {1, 2, 3, 4, 5};
    const ez_forward_list<int> z = {};
    test_fitr(wrap_itr(begin(l)), wrap_itr(end(l)), v);
    test_fitr(wrap_itr(begin(z)), wrap_itr(end(z)), empty_rng<int>());
  }
}
void test_range_wrapper() {
  static_assert(!is_default_constructible_v<range_wrapper<ez_vector<int> &>>);
  static_assert(is_default_constructible_v<range_wrapper<ez_vector<int>>>);

  static_assert(same_as
                <decltype(wrap_rng(declval<int (&)[2]>())),
                 range_wrapper<int (&)[2]>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<const int (&)[2]>())),
                 range_wrapper<const int (&)[2]>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<ez_vector<int> &>())),
                 range_wrapper<ez_vector<int> &>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<const ez_vector<int> &>())),
                 range_wrapper<const ez_vector<int> &>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<ez_vector<int> &&>())),
                 range_wrapper<ez_vector<int>>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<iter_pair<int *> &>())),
                 range_wrapper<iter_pair<int *>>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<const iter_pair<int *> &>())),
                 range_wrapper<iter_pair<int *>>>);
  static_assert(same_as
                <decltype(wrap_rng(declval<iter_pair<int *> &&>())),
                 range_wrapper<iter_pair<int *>>>);

  static_assert(rng_is_sized<range_wrapper<iter_pair<int *>>>);
  static_assert(!rng_is_sized<range_wrapper<ez_forward_list<int>>>);
  static_assert(!rng_is_sized<range_wrapper<ez_forward_list<int> &>>);
  static_assert(rng_is_sized<range_wrapper<ez_list<int>>>);
  static_assert(rng_is_sized<range_wrapper<ez_list<int>> &>);

  static_assert(is_rng_ref<range_wrapper<iter_pair<int *>>>);
  static_assert(!is_rng_ref<range_wrapper<ez_vector<int>>>);
  static_assert(is_rng_ref<range_wrapper<ez_vector<int> &>>);

  static_assert(same_as
                <decltype(wrap_rng
                          (declval<range_wrapper
                                   <range_wrapper
                                     <ez_vector<int> &>>>())),
                 range_wrapper
                 <range_wrapper<range_wrapper<ez_vector<int> &>>>>);
  
  int a[] = {1, 2};
  ez_vector v = {3, 4};
  test_rng(wrap_rng(a), ez_vector{1, 2});
  test_rng(wrap_rng(as_const(a)), ez_vector{1, 2});
  test_rng(wrap_rng(v), ez_vector{3, 4});
  test_rng(wrap_rng(as_const(v)), ez_vector{3, 4});
  test_rng(v, ez_vector{3, 4});
  test_rng(wrap_rng(move(v)), ez_vector{3, 4});
  assert(empty(v));
  v = {3, 4};
  auto r = rng(v);
  test_rng(v, ez_vector{3, 4});
  test_rng(r, ez_vector{3, 4});
  test_rng(wrap_rng(move(r)), ez_vector{3, 4});
  test_rng(wrap_rng(move(as_const(r))), ez_vector{3, 4});
  test_rng(wrap_rng(r), ez_vector{3, 4});
  test_rng(wrap_rng(as_const(r)), ez_vector{3, 4});
  test_rng(v, ez_vector{3, 4});
  test_rng(r, ez_vector{3, 4});
  test_rng(wrap_rng(3, 4), rng(3, 4));
}
void test_base_range() {
  // default constructible
  {
    static_assert(!is_default_constructible_v
                  <base_range<ez_vector<move_iterator<int *>> &>>);
    static_assert(is_default_constructible_v
                  <base_range<ez_vector<move_iterator<int *>>>>);

    static_assert(!is_default_constructible_v
                  <inplace_base_range<ez_vector<move_iterator<int *>> &>>);
    static_assert(is_default_constructible_v
                  <inplace_base_range<ez_vector<move_iterator<int *>>>>);
  }
  // sized or not
  {
    {
      const ez_list<int> l = {1, 2, 3};
      const auto r = degraded_irng(l);
      const auto rr = base_rng(r);
      const auto rrr = inplace_base_rng(r);
      static_assert(!rng_is_sized<decltype(rr)>);
      static_assert(rng_is_sized<decltype(rrr)>);
      test_rng(rr, irng(1, 4));
      test_rng(rrr, irng(1, 4));
    }
    {
      const ez_vector<int> v = {1, 2, 3};
      const auto r = degraded_frng(v);
      const auto rr = base_rng(r);
      const auto rrr = inplace_base_rng(r);
      static_assert(rng_is_sized<decltype(rr)>);
      static_assert(rng_is_sized<decltype(rrr)>);
      test_rng(rr, irng(1, 4));
      test_rng(rrr, irng(1, 4));
    }
  }
  // case for iter_pair
  {
    const ez_vector<int> v = {1, 2, 3};
    const iter_pair<const int *> x = rng(v);
    using t = counted_iterator<const int *>;
    const iter_pair<t> xx(t(x.begin(), 3), t(x.end(), 0));
    const auto r = base_rng(xx);
    static_assert(same_as
                  <remove_const_t<decltype(r)>,
                   base_range<iter_pair<counted_iterator<const int *>>>>);
    test_rng(r, irng(1, 4));
  }
  // is_rng_ref
  {
    {
      const ez_forward_list l = {1, 2, 3};
      const auto r = base_rng(rng(move_itr(l.begin()), move_itr(l.end())));
      const auto r2 = base_rng(move_itr(l.begin()), move_itr(l.end()));
      const auto r3 = base_rng(move_rng(copy(l)));
      static_assert(is_rng_ref<decltype(r)>);
      static_assert(is_rng_ref<decltype(r2)>);
      static_assert(!is_rng_ref<decltype(r3)>);
    }
    {
      const ez_forward_list l = {1, 2, 3};
      const auto r = inplace_base_rng
        (rng(move_itr(l.begin()), move_itr(l.end())));
      const auto r2 = inplace_base_rng(move_itr(l.begin()), move_itr(l.end()));
      const auto r3 = inplace_base_rng(move_rng(copy(l)));
      static_assert(is_rng_ref<decltype(r)>);
      static_assert(is_rng_ref<decltype(r2)>);
      static_assert(!is_rng_ref<decltype(r3)>);
    }
  }
}
void test_empty_range() {
  empty_range<int> r;
  assert(r.begin() == nullptr);
  assert(r.end() == nullptr);
  assert(r.size() == 0);
  assert(r.empty());
  assert(as_const(r).begin() == nullptr);
  assert(as_const(r).end() == nullptr);
  assert(as_const(r).size() == 0);
  assert(as_const(r).empty());

  empty_range<int> rr = empty_rng<int>();
  test_rng(rr, r);

  static_assert(is_rng_ref<empty_range<int>>);
  static_assert(is_rng_ref<empty_range<int> &>);
  static_assert(is_rng_ref<const empty_range<int> &>);
  static_assert(is_rng_ref<empty_range<int> &&>);
  static_assert(is_rng_ref<const empty_range<int> &&>);
}
void test_single_range() {
  static_assert(is_default_constructible_v<single_range<int>>);
  static_assert(!is_default_constructible_v<single_range<int &>>);

  int x = 1;
  auto r = single_rng(ref(x));
  x = 2;
  test_rng(r, single_rng(2));

  auto rr = single_rng(x);
  auto rrr = single_rng(move(x));
  x = 3;
  test_rng(rr, single_rng(2));
  test_rng(rrr, single_rng(2));

  auto r4 = single_range<int>(in_place, 3);
  test_rng(r4, rng(1, 3));
  test_rng(as_const(r4), rng(1, 3));
  assert(!r4.empty() && r4.size() == 1);
}
void test_counted_range() {
  static_assert(is_default_constructible_v<counted_range<ez_vector<int>>>);
  static_assert(!is_default_constructible_v<counted_range<ez_vector<int> &>>);
  {
    counted_range<ez_vector<int>> r;
    assert(r.empty() && r.size() == 0 && r.begin() == r.end()
           && as_const(r).begin() == as_const(r).end());

    counted_range<ez_vector<int>> r1({1, 2, 3}, 3), r2({4, 5}, 2);
    r1 = move(r2);
    assert(r2.empty() && r2.size() == 0 && r2.begin() == r2.end()
           && as_const(r2).begin() == as_const(r2).end());
    test_rng(r1, irng(4, 6));
  }

  static_assert(same_as<decltype(counted_rng(ez_vector<int>{})),
                        counted_range<ez_vector<int>>>);
  static_assert(same_as<decltype(counted_rng(as_lvalue(ez_vector<int>{}))),
                        counted_range<ez_vector<int> &>>);
  static_assert(same_as<decltype(counted_rng(as_lvalue(ez_list<int>{}))),
                        counted_range<ez_list<int> &>>);
  static_assert(same_as
                <decltype(counted_rng
                          (iter_pair(declval<const int *>(),
                                     declval<const int *>()))),
                 counted_range<iter_pair<const int *>>>);
  ez_forward_list l = {1, 2, 3, 4};
  auto r = counted_rng(l);
  assert(r.size() == 4 && !r.empty());
  assert(as_const(r).size() == 4 && !as_const(r).empty());
  test_rng(l, irng(1, 5));
  test_rng(as_const(l), irng(1, 5));

  auto rr = counted_rng(l, 4);
  assert(rr.size() == 4 && !rr.empty());
  assert(as_const(rr).size() == 4 && !as_const(rr).empty());
  test_rng(rr, irng(1, 5));
  test_rng(as_const(rr), irng(1, 5));

  auto rr3 = sized_rng(l);
  assert(rr3.size() == 4 && !rr3.empty());
  assert(as_const(rr3).size() == 4 && !as_const(rr3).empty());
  test_rng(rr3, irng(1, 5));
  test_rng(as_const(rr3), irng(1, 5));

  auto rr4 = sized_rng(l, 4);
  assert(as_const(rr4).size() == 4 && !as_const(rr4).empty());
  test_rng(rr4, irng(1, 5));
  test_rng(as_const(rr4), irng(1, 5));

  {
    auto x = counted_rng(ez_vector<int>{1, 2, 3}, 3);
    auto y = counted_rng(ez_vector<int>{4, 5}, 2);
    adl_swap(x, y);
    test_rng(x, irng(4, 6));
    test_rng(y, irng(1, 4));

    auto z = move(x);
    test_rng(x, empty_rng<int>());
    test_rng(z, irng(4, 6));
    assert(addressof(y = move(z)) == addressof(y));
    test_rng(z, empty_rng<int>());
    test_rng(y, irng(4, 6));
  }
}
void test_degraded_range() {
  {
    static_assert(is_default_constructible_v
                  <degraded_range<ez_vector<int>, input_iterator_tag>>);
    static_assert(!is_default_constructible_v
                  <degraded_range<ez_vector<int> &, input_iterator_tag>>);

    int a[] = {1, 2, 3};
    using rt = degraded_range<int (&)[3], input_iterator_tag>;
    static_assert(same_as<rng_itr<rt>, degraded_input_iterator<int *>>);
    rt r(a);
    assert(ref(r, 0) == 1);
    assert(ref(r, 1) == 2);
    assert(ref(r, 2) == 3);
    assert(r.size() == 3);
    assert(!r.empty());

    ez_vector<int> v = {1, 2};
    ez_vector<int> vv = {3, 4, 5};
    degraded_range<ez_vector<int>, forward_iterator_tag> r1(move(v));
    degraded_range<ez_vector<int>, forward_iterator_tag> r2(move(vv));
    adl_swap(r1, r2);
    test_rng(r1, irng(3, 6));
    test_rng(r2, irng(1, 3));
  }
  {
    ez_vector<int> v = {1, 2, 3};
    auto r1 = degraded_rng<input_iterator_tag>(move(v));
    auto r2 = degraded_irng(move(v));
    auto r3 = degraded_frng(move(v));
    auto r4 = degraded_brng(move(v));
    auto r5 = degraded_rrng(move(v));
    assert(ref(r1, 0) == 1 && ref(r1, 1) == 2 && ref(r1, 2) == 3);
    assert(ref(as_const(r1), 0) == 1 && ref(as_const(r1), 1) == 2
           && ref(as_const(r1), 2) == 3);
    assert(r1.size() == 3);
    assert(!r1.empty());
    assert(r2.empty());
    assert(r3.empty());
    assert(r4.empty());
    assert(r5.empty());
    v = {1, 2};
    auto rr = degraded_rng<input_iterator_tag>(v);
    static_assert(same_as
                  <decltype(rr),
                   degraded_range<ez_vector<int> &, input_iterator_tag>>);
    assert(ref(rr, 0) == 1);
    assert(ref(rr, 1) == 2);
    assert(rr.size() == 2);
    assert(!rr.empty());
  }
  {
    ez_vector<int> a[] = {{1, 2, 3}};
    ez_vector<int> b[] = {{}};
    auto r1 = degraded_irng(a);
    auto r2 = degraded_irng(b);
    assert(size(ref(r1, 0)) == 3);
    assert(size(ref(r2, 0)) == 0);
    assert(a[0].size() == 3);
    assert(b[0].size() == 0);
    assert(ref(a[0], 0) == 1);
    assert(ref(a[0], 1) == 2);
    assert(ref(a[0], 2) == 3);
    assert(ref(ref(r1, 0), 0) == 1);
    assert(ref(ref(r1, 0), 1) == 2);
    assert(ref(ref(r1, 0), 2) == 3);

    auto r3 = degraded_irng(ez_vector{1, 2});
    auto r4 = degraded_irng(ez_vector<int>{});
    adl_swap(r3, r4);
    test_rng(r3, ez_vector<int>{});
    test_rng(r4, ez_vector{1, 2});
    adl_swap(r3, r3);
    test_rng(r3, ez_vector<int>{});
    test_rng(r4, ez_vector{1, 2});
    adl_swap(r4, r4);
    test_rng(r3, ez_vector<int>{});
    test_rng(r4, ez_vector{1, 2});
    adl_swap(r3, r4);
    test_rng(r3, ez_vector{1, 2});
    test_rng(r4, ez_vector<int>{});
  }
  {
    struct t : private iter_pair<int *> {
      int *begin() const {return nullptr;}
      int *end() const {return nullptr;}
    };
    struct tt : iter_pair<int *> {};
    static_assert(same_as<decltype(degraded_rng<input_iterator_tag>(t{})),
                          degraded_range<t, input_iterator_tag>>);
    static_assert(same_as<decltype(degraded_rng<input_iterator_tag>(tt{})),
                          degraded_range<tt, input_iterator_tag>>);
    static_assert(same_as
                  <decltype(degraded_rng<input_iterator_tag>(as_lvalue(tt{}))),
                   degraded_range<tt, input_iterator_tag>>);
  }
  {
    {
      int a[] = {1, 2};
      auto r = degraded_frng(a);
      auto r2 = degraded_irng(r);
      auto r3 = degraded_irng(as_const(r));
      auto r4 = degraded_irng(move(r));
      auto r5 = degraded_irng(move(as_const(r)));
      static_assert(same_as<decltype(r), degraded_forward_range<int (&)[2]>>);
      static_assert(same_as<decltype(r2), degraded_input_range<decltype(r) &>>);
      const int aa[] = {1, 2};
      test_rng(r2, aa);
      test_rng(r3, aa);
      test_rng(r4, aa);
      test_rng(r5, aa);
    }

    ez_vector a = {1, 2};
    auto r = degraded_frng(move(a));
    assert(a.empty());
    auto r2 = degraded_irng(r);
    auto r3 = degraded_irng(as_const(r));
    static_assert(same_as<decltype(r), degraded_forward_range<ez_vector<int>>>);
    static_assert(same_as
                  <decltype(r2), degraded_input_range<decltype(r) &>>);
    static_assert(same_as
                  <decltype(r3), degraded_input_range<const decltype(r) &>>);
    const int aa[] = {1, 2};
    test_rng(r2, aa);
    test_rng(r3, aa);
    test_rng(r, aa);
    {
      auto rr = degraded_frng(ez_vector{1, 2});
      auto r4 = degraded_irng(move(rr));
      static_assert(is_just_iitr<rng_itr<decltype(r4)>>);
      static_assert(same_as
                    <decltype(r4), degraded_input_range<decltype(rr)>>);
      test_rng(r4, aa);
      assert(empty(rr));
    }
    {
      auto rr = degraded_frng(ez_vector{1, 2});
      auto r5 = degraded_irng(move(as_const(rr)));
      static_assert(is_just_iitr<rng_itr<decltype(r5)>>);
      static_assert(same_as
                    <decltype(r5), degraded_input_range<const decltype(rr)>>);
      test_rng(r5, aa);
      test_rng(rr, aa);
    }
  }

  // is_rng_ref
  {
    const ez_vector v = {1, 2, 3};
    static_assert(is_rng_ref<decltype(degraded_irng(v))>);
    static_assert(!is_rng_ref<decltype(degraded_irng(copy(v)))>);
  }
}
void test_move_range() {
  {
    static_assert(is_default_constructible_v<move_range<ez_vector<int>>>);
    static_assert(!is_default_constructible_v<move_range<ez_vector<int> &>>);

    using rt = move_range<int (&)[2]>;
    static_assert(same_as<rng_itr<rt>, move_iterator<int *>>);
    static_assert(same_as<rng_itr<const rt>, move_iterator<int *>>);
    static_assert(rng_is_sized<rt>);

    using rt2 = move_range<ez_vector<int>>;
    rt2 x({1, 2, 3});
    rt2 y({4, 5});
    adl_swap(x, y);
    test_rng(x, irng(4, 6));
    test_rng(y, irng(1, 4));
  }
  {
    using rt = move_range<ez_vector<int>>;
    static_assert(same_as<rng_itr<rt>, move_iterator<int *>>);
    static_assert(same_as<rng_itr<const rt>, move_iterator<const int *>>);
    static_assert(rng_is_sized<rt>);

    rt r(ez_vector{1});
    test_rng(r, ez_vector{1});
    rt r2;
    r2 = r;
    test_rng(r, ez_vector{1});
    test_rng(r2, ez_vector{1});
  }
  {
    struct t : private iter_pair<int *> {
      int *begin() const {return nullptr;}
      int *end() const {return nullptr;}
    };
    struct tt : iter_pair<int *> {};
    static_assert(same_as<decltype(move_rng(t{})), move_range<t>>);
    static_assert(same_as<decltype(move_rng(tt{})), move_range<tt>>);
    static_assert(same_as
                  <decltype(move_rng(as_lvalue(tt{}))), move_range<tt>>);
    int a[] = {1, 2, 3};
    test_rng(move_rng(rng(a)), ez_vector{1, 2, 3});
    test_rng(move_rng(a), ez_vector{1, 2, 3});
    test_rng(move_rng(rng(a)), ez_vector{1, 2, 3});
    test_rng(move_rng(move_rng(rng(a))), ez_vector{1, 2, 3});
    test_rng(move_rng(move_rng(rng(a))), ez_vector{1, 2, 3});
  }
  {
    int a[] = {1, 2};
    const int aa[] = {1, 2};
    auto r = move_rng(a);
    static_assert(same_as<decltype(r), move_range<int (&)[2]>>);
    test_rng(r, aa);
    decltype(auto) r2 = move_rng(r);
    static_assert(same_as<decltype(r2), move_range<decltype(r)>>);
    test_rng(r, aa);
    test_rng(r2, aa);
    decltype(auto) r3 = move_rng(as_const(r));
    static_assert(same_as<decltype(r3), move_range<decltype(r)>>);
    test_rng(r, aa);
    test_rng(r3, aa);
    decltype(auto) r4 = move_rng(move(r));
    static_assert(same_as<decltype(r4), move_range<decltype(r)>>);
    test_rng(r, aa);
    test_rng(r4, aa);
    decltype(auto) r5 = move_rng(move(as_const(r)));
    static_assert(same_as<decltype(r5), move_range<decltype(r)>>);
    test_rng(r, aa);
    test_rng(r5, aa);
  }
  {
    const int aa[] = {1, 2};
    static_assert(same_as
                  <decltype(move_rng(move_rng(move_rng(aa)))),
                   move_range<move_range<move_range<const int (&)[2]>>>>);
    {
      auto r = move_rng(ez_vector{1, 2});
      test_rng(r, aa);
      decltype(auto) r2 = move_rng(r);
      static_assert(same_as<decltype(r2), move_range<decltype(r) &>>);
      test_rng(r, aa);
      test_rng(r2, aa);
    }
    {
      auto r = move_rng(ez_vector{1, 2});
      test_rng(r, aa);
      decltype(auto) r2 = move_rng(as_const(r));
      static_assert(same_as<decltype(r2), move_range<const decltype(r) &>>);
      test_rng(r, aa);
      test_rng(r2, aa);
    }
    {
      auto r = move_rng(ez_vector{1, 2});
      test_rng(r, aa);
      decltype(auto) r2 = move_rng(move(r));
      static_assert(same_as<decltype(r2), move_range<decltype(r)>>);
      assert(empty(r));
      test_rng(r2, aa);
    }
    {
      auto r = move_rng(ez_vector{1, 2});
      test_rng(r, aa);
      decltype(auto) r2 = move_rng(move(as_const(r)));
      static_assert(same_as<decltype(r2), move_range<const decltype(r)>>);
      test_rng(r, aa);
      test_rng(r2, aa);
    }
  }

  // is_rng_ref
  {
    const ez_vector v = {1, 2, 3};
    static_assert(is_rng_ref<decltype(move_rng(v))>);
    static_assert(!is_rng_ref<decltype(move_rng(copy(v)))>);
  }
}
void test_reverse_range() {
  {
    using rt2 = reverse_range<ez_vector<int>>;
    rt2 x({3, 2, 1});
    rt2 y({5, 4});
    adl_swap(x, y);
    test_rng(x, irng(4, 6));
    test_rng(y, irng(1, 4));
  }    
  {
    static_assert(is_default_constructible_v<reverse_range<ez_vector<int>>>);
    static_assert(!is_default_constructible_v<reverse_range<ez_vector<int> &>>);

    auto is_12 = [](auto &&r) {
      return *begin(r) == 1 && *next(begin(r)) == 2;
    };
    auto is_21 = [](auto &&r) {
      return *begin(r) == 2 && *next(begin(r)) == 1;
    };

    int a[] = {1, 2};
    assert(is_21(rrng(a)));
    assert(is_12(rrng(rrng(a))));
    assert(is_21(rrng(rrng(rrng(a)))));

    ez_vector<int> v = {1, 2};
    auto r = rrng(v);
    assert(is_21(r));
    ref(v, 0) = 2;
    ref(v, 1) = 1;
    assert(is_12(r));

    auto rr = rrng(move(v));
    assert(is_12(rr));
    v = {9, 9, 9};
    assert(is_12(rr));
  }
  {
    int a[] = {1, 2, 3};
    static_assert(same_as<decltype(rrng(a)), reverse_range<int (&)[3]>>);
  }
  {
    struct t : iter_pair<int *> {
      using base_t = iter_pair<int *>;
    };
    struct tt : iter_pair<reverse_iterator<int *>> {
      using base_t = iter_pair<reverse_iterator<int *>>;
    };
    static_assert(same_as
                  <decltype(rrng(declval<t>())), reverse_range<t>>);
    static_assert(same_as
                  <decltype(rrng(declval<const t &>())), reverse_range<t>>);
    static_assert(same_as
                  <decltype(rrng(declval<tt>())), reverse_range<tt>>);

    test_rng(rrng(ez_vector{1, 2, 3}), ez_vector{3, 2, 1});
    test_rng(rrng(as_lvalue(ez_vector{1, 2, 3})), ez_vector{3, 2, 1});
    ez_vector a = {1, 2, 3};
    test_rng(rrng(a), ez_vector{3, 2, 1});
    test_rng(rrng(rrng(a)), ez_vector{1, 2, 3});
    test_rng(rrng(rrng(rrng(a))), ez_vector{3, 2, 1});
    test_rng(rrng(rng(a)), ez_vector{3, 2, 1});
    test_rng(rrng(as_lvalue(rng(a))), ez_vector{3, 2, 1});
    test_rng(rrng(rrng(rng(a))), ez_vector{1, 2, 3});

    test_rng(rrng(ez_vector{1, 2, 3}), ez_vector{3, 2, 1});
    test_rng(rrng(rrng(ez_vector{1, 2, 3})), ez_vector{1, 2, 3});
    test_rng(rrng(rrng(rrng(ez_vector{3, 2, 1}))), ez_vector{1, 2, 3});
    static_assert(same_as
                  <decltype(rrng(ez_vector<int>{})),
                   reverse_range<ez_vector<int>>>);
    static_assert(same_as
                  <decltype(rrng(rrng(rrng(ez_vector<int>{})))),
                   reverse_range
                   <reverse_range<reverse_range<ez_vector<int>>>>>);

    static_assert(same_as
                  <decltype(rrng(declval<ez_vector<int> &>())),
                   reverse_range<ez_vector<int> &>>);
    static_assert(same_as
                  <decltype(rrng(declval<const ez_vector<int> &>())),
                   reverse_range<const ez_vector<int> &>>);
    static_assert(same_as
                  <decltype(rrng(declval<ez_vector<int> &&>())),
                   reverse_range<ez_vector<int>>>);
    static_assert(same_as
                  <decltype(rrng(declval<const ez_vector<int> &&>())),
                   reverse_range<const ez_vector<int>>>);
 
    static_assert(same_as
                  <decltype(rrng(rrng(declval<ez_vector<int> &>()))),
                   reverse_range<reverse_range<ez_vector<int> &>>>);
    static_assert(same_as
                  <decltype(rrng(rrng(declval<const ez_vector<int> &>()))),
                   reverse_range<reverse_range<const ez_vector<int> &>>>);
    static_assert(same_as
                  <decltype(rrng(rrng(declval<ez_vector<int> &&>()))),
                   reverse_range<reverse_range<ez_vector<int>>>>);
    static_assert(same_as
                  <decltype(rrng(rrng(declval<const ez_vector<int> &&>()))),
                   reverse_range<reverse_range<const ez_vector<int>>>>);
  }

  // is_rng_ref
  {
    const ez_vector v = {1, 2, 3};
    static_assert(is_rng_ref<decltype(rrng(v))>);
    static_assert(!is_rng_ref<decltype(rrng(copy(v)))>);
  }
}
void test_rng_for_iterator_n() {
  static_assert(rng_is_sized
                <iter_pair
                 <counted_iterator
                  <degraded_iterator<int *, input_iterator_tag>>>>);
  int a[] = {1, 2, 3, 4, 5};
  static_assert(is_rng_ref<decltype(rng(begin(a), 5))>);
  static_assert(is_rng_ref<decltype(rng(degraded_iitr(begin(a)), 5))>);
  static_assert(is_just_iitr<iter_n_iterator<degraded_input_iterator<int *>>>);
  static_assert(is_just_fitr
                <iter_n_iterator<degraded_forward_iterator<int *>>>);
  static_assert(is_just_fitr
                <iter_n_iterator<degraded_bidirectional_iterator<int *>>>);
  {
    auto tmp = rng(nth(a, 1), 3);
    test_fitr(tmp.begin(), tmp.end(), ez_vector{2, 3, 4});
  }
  {
    auto tmp = rng(nth(a, 1), 0);
    test_fitr(tmp.begin(), tmp.end(), ez_vector<int>{});
  }
  test_rng(rng(nth(degraded_irng(a), 1), 3), ez_vector{2, 3, 4});
  test_rng(rng(nth(degraded_irng(a), 1), 0), ez_vector<int>{});

  static_assert(same_as<decltype(rng(declval<int *>(), 3)),
                        iter_pair<int *>>);
  using it = degraded_iterator<int *, bidirectional_iterator_tag>;
  static_assert(same_as<decltype(rng(declval<it>(), 3)),
                        iter_pair<iter_n_iterator<it>>>);
  using it2 = degraded_iterator<int *, input_iterator_tag>;
  static_assert(same_as<decltype(rng(declval<it2>(), 3)),
                        iter_pair<iter_n_iterator<it2>>>);
}
void test_rng_for_n_value() {
  {
    int x = 2;
    auto r = rng(3, x);
    auto r2 = rng(3, as_const(x));
    static_assert(same_as<decltype(r), n_value<int>>);
    static_assert(same_as<decltype(r2), n_value<int>>);
    x = 3;
    test_rng(r, rng(3, 2));
    test_rng(r2, rng(3, 2));
  }
  {
    int x = 2;
    auto r = rng(3, ref(x));
    auto r2 = rng(3, ref(as_const(x)));
    static_assert(same_as<decltype(r), n_value<const int &>>);
    static_assert(same_as<decltype(r2), n_value<const int &>>);
    x = 3;
    test_rng(r, rng(3, 3));
    test_rng(r2, rng(3, 3));
  }
}
void test_iterator_range() {
  {
    static_assert(same_as<ptrdiff_t, typename inner::get_iitr_dft<void>::type>);
    static_assert(same_as<int, typename inner::get_iitr_dft<int>::type>);
    static_assert(same_as<int, typename inner::get_iitr_dft<short>::type>);
    static_assert(same_as
                  <ptrdiff_t, typename inner::get_iitr_dft<int *>::type>);

    static_assert(same_as
                  <random_access_iterator_tag,
                   typename inner::get_iitr_ctg<int *>::type>);
    static_assert(same_as
                  <random_access_iterator_tag,
                   typename inner::get_iitr_ctg<int>::type>);
    static_assert(same_as
                  <input_iterator_tag,
                   typename inner::get_iitr_ctg
                   <degraded_input_iterator<int *>>::type>);
    static_assert(same_as
                  <forward_iterator_tag,
                   typename inner::get_iitr_ctg
                   <degraded_forward_iterator<int *>>::type>);
    static_assert(same_as
                  <bidirectional_iterator_tag,
                   typename inner::get_iitr_ctg
                   <degraded_bidirectional_iterator<int *>>::type>);
    static_assert(same_as
                  <random_access_iterator_tag,
                   typename inner::get_iitr_ctg
                   <degraded_random_access_iterator<int *>>::type>);
  }
  {
    test_ritr(iterator_iterator<int>(1),
              iterator_iterator<int>(6),
              ez_vector<int>{1, 2, 3, 4, 5});
    test_ritr(iterator_iterator<int>(1),
              iterator_iterator<int>(1),
              ez_vector<int>{});

    ez_vector<int> v{};
    test_ritr(iterator_iterator<int *>(v.begin()),
              iterator_iterator<int *>(v.end()),
              ez_vector<int *>{});
    v = {1, 2, 3};
    ez_vector<int *> vv = {nth(v, 0), nth(v, 1), nth(v, 2)};
    test_ritr(iterator_iterator<int *>(v.begin()),
              iterator_iterator<int *>(v.end()),
              vv);
  }
  {
    {
      using it = iterator_iterator<short>;
      static_assert(same_as<it::value_type, short>);
      static_assert(same_as<it::difference_type, int>);
      static_assert(same_as<it::pointer, void>);
      static_assert(same_as<it::reference, short>);
      static_assert(same_as<it::iterator_category, random_access_iterator_tag>);
      static_assert(regular<it>);
      static_assert(swappable<it>);
      static_assert(three_way_comparable<it>);
      static_assert(random_access_iterator<it>);
      short i = 3;
      it ii(i);
      static_assert(same_as<decltype(ii.base()), short>);
      assert(ii.base() == i);
    }
    {
      using it = iterator_iterator<int *>;
      static_assert(same_as<it::value_type, int *>);
      static_assert(same_as<it::difference_type, ptrdiff_t>);
      static_assert(same_as<it::pointer, void>);
      static_assert(same_as<it::reference, int *>);
      static_assert(same_as<it::iterator_category, random_access_iterator_tag>);
      static_assert(regular<it>);
      static_assert(swappable<it>);
      static_assert(three_way_comparable<it>);
      static_assert(random_access_iterator<it>);
    }
  }
  {
    int a[] = {1, 2};
    auto p = rng(begin(a), end(a));
    static_assert(same_as
                  <decltype(irng(p)), iterator_range<iter_pair<int *>>>);
    test_rng(p, ez_vector{1, 2});
    static_assert(same_as
                  <decltype(irng(move(p))), iterator_range<iter_pair<int *>>>);
    static_assert(same_as
                  <decltype(irng(ez_vector<int>{})),
                   iterator_range<ez_vector<int>>>);
    ez_vector<int> v{1, 2, 3};
    test_rng(irng(v), ez_vector{v.begin(), v.begin() + 1, v.begin() + 2});
    test_rng(irng(as_const(v)),
              ez_vector{v.begin(), v.begin() + 1, v.begin() + 2});
    auto r = irng(move(v));
    assert(empty(v));
    assert(size(r) == 3);
    assert(*ref(r, 0) == 1 && *ref(r, 1) == 2 && *ref(r, 2) == 3);
  }
  {
    ez_vector<int> v;
    auto oo = inserter(v, v.end());
    auto o = iitr(oo);
    static_assert(is_oitr<decltype(o)>);
    *o = 1;
    assert(v == ez_vector({1}));
  }
  {
    using rt2 = iterator_range<ez_vector<int>>;
    rt2 x({1, 2, 3});
    rt2 y({4, 5});
    adl_swap(x, y);
    test_rng(deref_rng(x), irng(4, 6));
    test_rng(deref_rng(y), irng(1, 4));
  }

  // is_rng_ref
  {
    ez_vector<int> v;
    static_assert(is_rng_ref<decltype(irng(v))>);
    static_assert(!is_rng_ref<decltype(irng(move(v)))>);
  }
}
void test_bind_range() {
  // bind_iterator compile time info
  {
    auto f = [](int &x) {return 0;};
    int i = 0;
    auto ff = [i](int &) {return 0;};
    using it = bind_iterator<int *, decltype(f)>;
    using it2 = bind_iterator<int *, decltype(ff)>;

    static_assert(semiregular<it>);
    static_assert(semiregular<it2>);
    static_assert(copyable<it2> && equality_comparable<it2>
                  && three_way_comparable<it2>);
    static_assert(default_initializable<it2>);

    static_assert(same_as<it::value_type, int>);
    static_assert(same_as<it::difference_type, ptrdiff_t>);
    static_assert(same_as<it::pointer, void>);
    static_assert(same_as<it::reference, int>);
    static_assert(same_as<it::iterator_category, random_access_iterator_tag>);

    static_assert(same_as<it2::value_type, int>);
    static_assert(same_as<it2::difference_type, ptrdiff_t>);
    static_assert(same_as<it2::pointer, void>);
    static_assert(same_as<it2::reference, int>);
    static_assert(same_as<it2::iterator_category, random_access_iterator_tag>);

    {
      int x = 0;
      int y = 3;
      auto fx = [&y](int &s) {return s + y;};
      auto ii = bind_iterator(addressof(x), fx);
      static_assert(is_copy_constructible_v<decltype(fx)>);
      static_assert(is_move_constructible_v<decltype(fx)>);
      static_assert(is_nothrow_copy_assignable_v<decltype(ii)>);
      static_assert(is_nothrow_move_assignable_v<decltype(ii)>);
      static_assert(is_nothrow_swappable_v<decltype(ii)>);
    }
    {
      int x{};
      auto fx = [x = ez_vector<int>{}](int &s) {return 0;};
      auto ii = bind_iterator(addressof(x), fx);
      static_assert(is_copy_constructible_v<decltype(fx)>);
      static_assert(is_move_constructible_v<decltype(fx)>);
      static_assert(!is_nothrow_copy_assignable_v<decltype(ii)>);
      static_assert(is_nothrow_move_assignable_v<decltype(ii)>);
      static_assert(is_nothrow_swappable_v<decltype(ii)>);
    }
    {
      int x{};
      struct t {
        int value;
        t() {}
        t(const t &) {}
        t(t &&) {}
      };
      auto fx = [x = t{}](int &s) {return 0;};
      auto ii = bind_iterator(addressof(x), fx);
      static_assert(is_copy_constructible_v<decltype(fx)>);
      static_assert(is_move_constructible_v<decltype(fx)>);
      static_assert(!is_nothrow_copy_assignable_v<decltype(ii)>);
      static_assert(!is_nothrow_move_assignable_v<decltype(ii)>);
      static_assert(!is_nothrow_swappable_v<decltype(ii)>);
    }
  }
  // bind_iterator
  {
    auto f = [](const int &x) {return x * 10;};
    const int placeholder = 0;
    auto ff = [placeholder](const int &x) {return x * 100;};
    static_assert(!default_initializable<decltype(ff)>);

    int a[] = {1, 2, 3, 4};
    bind_iterator i(begin(a), f);
    assert(i.fn()(2) == 20);
    assert(*i == 10);
    bind_iterator ii(begin(a), addressof(f));
    assert(*ii == 10);
    bind_iterator j(begin(a), ff);
    assert(*j == 100);
    bind_iterator jj(begin(a), addressof(ff));
    assert(*jj == 100);

    const ez_vector v = {10, 20, 30, 40};
    const ez_vector vv = {100, 200, 300, 400};
    test_ritr(bind_iterator(begin(a), f),
              bind_iterator(end(a), f),
              v);
    test_ritr(bind_iterator(begin(a), f),
              bind_iterator(cend(a), f),
              v);
    test_ritr(bind_iterator(cbegin(a), f),
              bind_iterator(end(a), f),
              v);
    test_ritr(bind_iterator(cbegin(a), f),
              bind_iterator(cend(a), f),
              v);
    test_ritr(bind_iterator(begin(a), addressof(f)),
              bind_iterator(end(a), addressof(f)),
              v);
    test_ritr(bind_iterator(begin(a), ff),
              bind_iterator(end(a), ff),
              vv);
    test_ritr(bind_iterator(begin(a), addressof(ff)),
              bind_iterator(end(a), addressof(ff)),
              vv);

    static_assert(same_as
                  <decltype(bind_rng(rng(a), f)),
                   bind_range<iter_pair<int *>, decltype(f)>>);
    static_assert(is_rng_ref<decltype(bind_rng(rng(a), f))>);
    static_assert(same_as
                  <decltype(bind_rng(rng(a), ff)),
                   bind_range<iter_pair<int *>, decltype(ff)>>);
    static_assert(!is_rng_ref<bind_range<iter_pair<int *>, decltype(ff)>>);
    static_assert(is_rng_ref<bind_range<iter_pair<int *>, decltype(f)>>);
    static_assert(same_as
                  <decltype(bind_rng(a, f)),
                   bind_range<int (&)[4], decltype(f)>>);
    static_assert(same_as
                  <decltype(bind_rng(a, ff)),
                   bind_range<int (&)[4], decltype(ff)>>);
    test_rng(bind_rng(rng(a), f), v);
    test_rng(bind_rng(rng(a), ff), vv);
    test_rng(bind_rng(a, f), v);
    test_rng(bind_rng(a, ff), vv);
      
    test_rng(rng(bind_itr(iitr(1), copy), bind_itr(iitr(4), copy)),
             ez_vector{1, 2, 3});
  }
  // bind_range
  {
    static_assert(is_default_constructible_v
                  <bind_range<ez_vector<int>, int (*)(int)>>);
    static_assert(!is_default_constructible_v
                  <bind_range<ez_vector<int> &, int (*)(int)>>);
    {
      bind_range<ez_vector<int>, int (*)(int)> r;
      assert(empty(r) && size(r) == 0
             && r.begin() == r.end()
             && as_const(r).begin() == as_const(r).end());
    }

    int v[] = {1, 2, 3};
    const int cv[] = {1, 2, 3};
    static_assert(same_as<decltype(const_rng(v)),
                          bind_range<int (&)[3], inner::as_cref>>);
    static_assert(same_as<decltype(const_rng(const_rng(const_rng(v)))),
                          bind_range<decltype(const_rng(const_rng(v))),
                                        inner::as_cref>>);
    test_rng(const_rng(v), ez_vector{1, 2, 3});
    test_rng(const_rng(const_rng(v)), ez_vector{1, 2, 3});
    test_rng(const_rng(const_rng(const_rng(v))), ez_vector{1, 2, 3});
    test_rng(const_rng(cv), ez_vector{1, 2, 3});
    test_rng(const_rng(const_rng(cv)), ez_vector{1, 2, 3});
    test_rng(const_rng(const_rng(const_rng(cv))), ez_vector{1, 2, 3});

    test_rng(deref_rng(irng(v)), cv);

    {
      {
        int *p{};
        auto i = const_itr(p);
        auto r = const_rng(rng(p, p));
        static_assert(same_as<rng_itr<decltype(r)>, decltype(i)>);
      }
      {
        int *p{};
        auto i = const_itr(p);
        auto r = const_rng(rng(p, p));
        static_assert(same_as<rng_itr<decltype(r)>, decltype(i)>);
      }
    }
    {
      {
        int z = 1;
        int *x = addressof(z);
        int **p = addressof(x);
        auto i = deref_itr(p);
        assert(*i == 1);
        auto r = deref_rng(rng(p, p));
        static_assert(same_as<rng_itr<decltype(r)>, decltype(i)>);
      }
    }

    {
      auto f1 = bind(plus{}, _1, 100);
      decltype(auto) f2 = bind(plus{}, _1, 10);
      using rt2 = bind_range<ez_vector<int>, decltype(f1)>;
      rt2 x({1, 2, 3}, f1);
      rt2 y({4, 5}, f2);
      adl_swap(x, y);
      test_rng(x, ez_vector{14, 15});
      test_rng(y, ez_vector{101, 102, 103});
    }
  }
}
void test_iters() {
  int a[3] = {};

  for (auto x : iters(0, 0))
    throw_or_abort<logic_error>("test_iters 0\n");
  for (auto x : r_iters(0, 0))
    throw_or_abort<logic_error>("test_iters 0\n");
  {
    ez_vector<int> v;
    for (auto x : iters(1, 2))
      v.insert(end(v), x);
    assert(v == ez_vector<int>({1}));

    v = {};
    for (auto x : r_iters(1, 2))
      v.insert(end(v), x);
    assert(v == ez_vector<int>({1}));

    v = {};
    for (auto x : iters(1, 6))
      v.insert(end(v), x);
    assert(v == ez_vector<int>({1, 2, 3, 4, 5}));

    v = {};
    for (auto x : r_iters(1, 6))
      v.insert(end(v), x);
    assert(v == ez_vector<int>({5, 4, 3, 2, 1}));
  }

  int i = 0;
  for (auto &p : iters(a))
    *p = ++i;
  test_rng(a, ez_vector{1, 2, 3});

  i = 4;
  for (auto &p : iters(begin(a), end(a)))
    *p = --i;
  test_rng(a, ez_vector{3, 2, 1});

  i = 0;
  for (auto &p : r_iters(a))
    *p = ++i;
  test_rng(a, ez_vector{3, 2, 1});

  i = 4;
  for (auto &p : r_iters(begin(a), end(a)))
    *p = --i;
  test_rng(a, ez_vector{1, 2, 3});
}
void test_range_miscl() {
  // can_memmove_from_range_to_iterator
  {
    static_assert(can_memmove_from_range_to_iterator
                  <iter_pair<int *>, int *>);

    static_assert(!can_memmove_from_range_to_iterator
                  <iter_pair<int *>, unsigned *>);

    static_assert(!can_memmove_from_range_to_iterator
                  <iter_pair<ez_vector<int> *>, ez_vector<int> *>);

    static_assert(!can_memmove_from_range_to_iterator
                  <iter_pair<int *>, const int *>);
    static_assert(!can_memmove_from_range_to_iterator
                  <iter_pair<int *>, volatile int *>);
    static_assert(!can_memmove_from_range_to_iterator
                  <iter_pair<int *>, move_iterator<const int *>>);

    static_assert(can_memmove_from_range_to_iterator
                  <move_range<iter_pair<int *>>, int *>);
    static_assert(can_memmove_from_range_to_iterator
                  <move_range<iter_pair<int *>>,
                   move_iterator<int *>>);
    static_assert(can_memmove_from_range_to_iterator
                  <move_range<iter_pair<const int *>>,
                   move_iterator<int *>>);
    static_assert(!can_memmove_from_range_to_iterator
                  <move_range<iter_pair<volatile int *>>,
                   move_iterator<int *>>);
  }
}

void test_range() {
  printf("range: ");

  inner::fns::test_iterator_requirements();
  inner::fns::test_iterator_main_components();
  inner::fns::test_range_main_components();

  inner::fns::test_degraded_iterator();
  inner::fns::test_reverse_iterator();
  inner::fns::test_insert_iterator();
  inner::fns::test_move_iterator();
  inner::fns::test_counted_iterator();

  inner::fns::test_array();
  inner::fns::test_iter_pair();
  inner::fns::test_composite_range();
  inner::fns::test_iterator_wrapper();
  inner::fns::test_range_wrapper();
  inner::fns::test_base_range();
  inner::fns::test_empty_range();
  inner::fns::test_single_range();
  inner::fns::test_counted_range();
  inner::fns::test_degraded_range();
  inner::fns::test_move_range();
  inner::fns::test_reverse_range();
  inner::fns::test_rng_for_iterator_n();
  inner::fns::test_rng_for_n_value();
  inner::fns::test_iterator_range();
  inner::fns::test_bind_range();
  inner::fns::test_iters();
  inner::fns::test_range_miscl();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_range();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
