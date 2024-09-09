#include <re/base.h>

#include <cassert>

// help test_type_traits
namespace re::inner::fns {

namespace test_common_type {

struct t {};
struct tt {};

}

}
namespace re {

template <>
struct common_type<inner::fns::test_common_type::t,
                   inner::fns::test_common_type::t> {
  using type = void;
};
template <>
struct common_type<inner::fns::test_common_type::t,
                   inner::fns::test_common_type::tt> {
  using type = void;
};
template <>
struct common_type<inner::fns::test_common_type::tt,
                   inner::fns::test_common_type::t> {
  using type = void;
};

}
namespace re::inner::fns::test_common_reference {

struct t {};
struct tt : t {};
struct s {};
struct ss {
  operator s() const;
};
struct u {};
struct uu {
  operator const u &() const;
};

struct v {};
struct vv {};
struct w {};
struct ww {
  operator w() const;
};

struct z {};
struct zz {
  operator z() const;
};

}
namespace re {

template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::t,
                              inner::fns::test_common_reference::tt,
                              F, FF> {
  using type = inner::copycvref_t<FF<void *>, F<void *>>;
};
template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::tt,
                              inner::fns::test_common_reference::t,
                              F, FF> {
  using type = inner::copycvref_t<FF<void *>, F<void *>>;
};

template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::s,
                              inner::fns::test_common_reference::ss,
                              F, FF> {
  using type = inner::copycvref_t<FF<void *>, F<void *>>;
};
template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::ss,
                              inner::fns::test_common_reference::s,
                              F, FF> {
  using type = inner::copycvref_t<FF<void *>, F<void *>>;
};

template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::u,
                              inner::fns::test_common_reference::uu,
                              F, FF> {
  using type = void;
};
template <template <class> class F, template <class> class FF>
struct basic_common_reference<inner::fns::test_common_reference::uu,
                              inner::fns::test_common_reference::u,
                              F, FF> {
  using type = void;
};

template <>
struct common_type<inner::fns::test_common_reference::v,
                   inner::fns::test_common_reference::vv> {
  using type = nullptr_t;
};
template <>
struct common_type<inner::fns::test_common_reference::vv,
                   inner::fns::test_common_reference::v> {
  using type = nullptr_t;
};

template <>
struct common_type<inner::fns::test_common_reference::w,
                   inner::fns::test_common_reference::ww> {
  using type = nullptr_t;
};
template <>
struct common_type<inner::fns::test_common_reference::ww,
                   inner::fns::test_common_reference::w> {
  using type = nullptr_t;
};

}
namespace re::inner::fns::test_common_reference {

void case12_common_ref_or_user_defined_common_reference() {
  // mark fall-through result
  {
    static_assert(is_same_v<basic_common_reference
                            <t, tt, type_identity_t, type_identity_t>::type,
                            void *>);
    static_assert(is_same_v<basic_common_reference
                            <tt, t, type_identity_t, type_identity_t>::type,
                            void *>);
    static_assert(is_same_v<basic_common_reference
                            <s, ss, type_identity_t, type_identity_t>::type,
                            void *>);
    static_assert(is_same_v<basic_common_reference
                            <ss, s, type_identity_t, type_identity_t>::type,
                            void *>);
    static_assert(is_void_v<basic_common_reference
                            <u, uu, type_identity_t, type_identity_t>::type>);
    static_assert(is_void_v<basic_common_reference
                            <uu, u, type_identity_t, type_identity_t>::type>);
  }
  // same non-reference type
  {
    static_assert(is_same_v<inner::common_ref_t<t, t>, inner::disable>);
    static_assert(is_same_v<common_reference_t<t, t>, t>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t, const t>, inner::disable>);
    static_assert(is_same_v<decltype(false ? declval<const t (&)()>()()
                                     : declval<const t (&)()>()()), const t>);
    static_assert(is_same_v<common_reference_t<const t, const t>, const t>);
    static_assert(is_same_v
                  <inner::common_ref_t<const int, const int>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const int, const int>, int>);
  }
  // same type, lvalue reference and lvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<t &, t &>, t &>);
    static_assert(is_same_v<common_reference_t<t &, t &>, t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &, t &>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &, t &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &, const t &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, const t &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, const t &>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, const t &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, volatile t &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, volatile t &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<t &, volatile t &>, volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<t &, volatile t &>, volatile t &>);
  }
  // same type, rvalue reference and rvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<t &&, t &&>, t &&>);
    static_assert(is_same_v<common_reference_t<t &&, t &&>, t &&>);
    static_assert(is_same_v<inner::common_ref_t<const t &&, t &&>, const t &&>);
    static_assert(is_same_v<common_reference_t<const t &&, t &&>, const t &&>);
    static_assert(is_same_v<inner::common_ref_t<t &&, const t &&>, const t &&>);
    static_assert(is_same_v<common_reference_t<t &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, volatile t &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, volatile t &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<t &&, volatile t &&>, volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<t &&, volatile t &&>, volatile t &&>);
  }
  // same type, differenct references
  {
    static_assert(is_same_v<inner::common_ref_t<t &, t &&>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, t &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &, t &&>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &, t &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &, const t &&>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, const t &&>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, const t &&>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, const t &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &&, t &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &&, t &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &&, t &>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &&, t &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &&, const t &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &&, const t &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, const t &>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, const t &>, const t &>);
  }
  // t and tt, non-reference
  {
    static_assert(is_same_v<inner::common_ref_t<t, tt>, inner::disable>);
    static_assert(is_same_v<common_reference_t<t, tt>, void *>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t, const tt>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const t, const tt>,
                            void *const>);
    static_assert(is_same_v<inner::common_ref_t<tt, t>, inner::disable>);
    static_assert(is_same_v<common_reference_t<tt, t>, void *>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt, const t>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const tt, const t>,
                            void *const>);
  }
  // t and tt, lvalue reference and lvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<t &, tt &>, t &>);
    static_assert(is_same_v<common_reference_t<t &, tt &>, t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &, tt &>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &, tt &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &, const tt &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, const tt &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, const tt &>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, const tt &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, volatile tt &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, volatile tt &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<t &, volatile tt &>, volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<t &, volatile tt &>, volatile t &>);

    static_assert(is_same_v<inner::common_ref_t<tt &, t &>, t &>);
    static_assert(is_same_v<common_reference_t<tt &, t &>, t &>);
    static_assert(is_same_v<inner::common_ref_t<const tt &, t &>, const t &>);
    static_assert(is_same_v<common_reference_t<const tt &, t &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<tt &, const t &>, const t &>);
    static_assert(is_same_v<common_reference_t<tt &, const t &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt &, const t &>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const tt &, const t &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt &, volatile t &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<const tt &, volatile t &>,
                   const volatile t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<tt &, volatile t &>, volatile t &>);
    static_assert(is_same_v
                  <common_reference_t<tt &, volatile t &>, volatile t &>);
  }
  // t and tt, rvalue reference and rvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<t &&, tt &&>, t &&>);
    static_assert(is_same_v<common_reference_t<t &&, tt &&>, t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, tt &&>, const t &&>);
    static_assert(is_same_v<common_reference_t<const t &&, tt &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<t &&, const tt &&>, const t &&>);
    static_assert(is_same_v
                  <common_reference_t<t &&, const tt &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, const tt &&>, const t &&>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, const tt &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, volatile tt &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, volatile tt &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<t &&, volatile tt &&>, volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<t &&, volatile tt &&>, volatile t &&>);

    static_assert(is_same_v<inner::common_ref_t<tt &&, t &&>, t &&>);
    static_assert(is_same_v<common_reference_t<tt &&, t &&>, t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt &&, t &&>, const t &&>);
    static_assert(is_same_v<common_reference_t<const tt &&, t &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<tt &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <common_reference_t<tt &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <common_reference_t<const tt &&, const t &&>, const t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const tt &&, volatile t &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<const tt &&, volatile t &&>,
                   const volatile t &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<tt &&, volatile t &&>, volatile t &&>);
    static_assert(is_same_v
                  <common_reference_t<tt &&, volatile t &&>, volatile t &&>);
  }
  // t and tt, differenct references
  {
    static_assert(is_same_v<inner::common_ref_t<t &, tt &&>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, tt &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &, tt &&>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &, tt &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &, const tt &&>, const t &>);
    static_assert(is_same_v<common_reference_t<t &, const tt &&>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &, const tt &&>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &, const tt &&>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &&, tt &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &&, tt &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<const t &&, tt &>, const t &>);
    static_assert(is_same_v<common_reference_t<const t &&, tt &>, const t &>);
    static_assert(is_same_v<inner::common_ref_t<t &&, const tt &>, const t &>);
    static_assert(is_same_v<common_reference_t<t &&, const tt &>, const t &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const t &&, const tt &>, const t &>);
    static_assert(is_same_v
                  <common_reference_t<const t &&, const tt &>, const t &>);
  }
  // s and ss, non_reference
  {
    static_assert(is_same_v<inner::common_ref_t<s, ss>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s, ss>, void *>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s, const ss>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const s, const ss>,
                            void *const>);
  }
  // s and ss, lvalue reference and lvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<s &, ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &, ss &>, void *&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s &, ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &, ss &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<s &, const ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &, const ss &>,
                            void *&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s &, const ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &, const ss &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s &, volatile ss &>,
                   inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &, volatile ss &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<s &, volatile ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &, volatile ss &>,
                            void *&>);

    static_assert(is_same_v<inner::common_ref_t<ss &, s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &, s &>, void *&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const ss &, s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &, s &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<ss &, const s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &, const s &>, void *&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const ss &, const s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &, const s &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const ss &, volatile s &>,
                   inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &, volatile s &>,
                            void *const &>);
    static_assert(is_same_v
                  <inner::common_ref_t<ss &, volatile s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &, volatile s &>, void *&>);
  }
  // s and ss, rvalue reference and rvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<s &&, ss &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &&, ss &&>, void *&&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s &&, ss &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &&, ss &&>,
                            void *const &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<s &&, const ss &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &&, const ss &&>, void *&&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const s &&, const ss &&>,
                   inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &&, const ss &&>,
                            void *const &&>);
    static_assert(is_same_v<inner::common_ref_t<const s &&, volatile ss &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &&, volatile ss &&>,
                            void *const &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<s &&, volatile ss &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &&, volatile ss &&>,
                            void *&&>);

    static_assert(is_same_v<inner::common_ref_t<ss &&, s &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &&, s &&>, void *&&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const ss &&, s &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &&, s &&>,
                            void *const &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<ss &&, const s &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &&, const s &&>, void *&&>);
    static_assert(is_same_v
                  <inner::common_ref_t<const ss &&, const s &&>,
                   inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &&, const s &&>,
                            void *const &&>);
    static_assert(is_same_v<inner::common_ref_t<const ss &&, volatile s &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &&, volatile s &&>,
                            void *const &&>);
    static_assert(is_same_v
                  <inner::common_ref_t<ss &&, volatile s &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &&, volatile s &&>,
                            void *&&>);
  }
  // s and ss, differenct references
  {
    static_assert(is_same_v<inner::common_ref_t<s &, ss &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &, ss &&>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const s &, ss &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &, ss &&>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<s &, const ss &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<s &, const ss &&>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const s &, const ss &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &, const ss &&>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<s &&, ss &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<s &&, ss &>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const s &&, ss &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &&, ss &>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<s &&, const ss &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<s &&, const ss &>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const s &&, const ss &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const s &&, const ss &>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<ss &, s &&>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &, s &&>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const ss &, s &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &, s &&>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<ss &, const s &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &, const s &&>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const ss &, const s &&>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &, const s &&>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<ss &&, s &>, inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &&, s &>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const ss &&, s &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &&, s &>,
                            void *const &>);
    static_assert(is_same_v<inner::common_ref_t<ss &&, const s &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<ss &&, const s &>, void *&>);
    static_assert(is_same_v<inner::common_ref_t<const ss &&, const s &>,
                            inner::disable>);
    static_assert(is_same_v<common_reference_t<const ss &&, const s &>,
                            void *const &>);
  }
  // u and uu, non_reference
  {
    static_assert(is_same_v<inner::common_ref_t<u, uu>, inner::disable>);
    static_assert(is_void_v<common_reference_t<u, uu>>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u, const uu>, inner::disable>);
    static_assert(is_void_v<common_reference_t<const u, const uu>>);

    static_assert(is_same_v<inner::common_ref_t<uu, u>, inner::disable>);
    static_assert(is_void_v<common_reference_t<uu, u>>);
    static_assert(is_same_v
                  <inner::common_ref_t<const uu, const u>, inner::disable>);
    static_assert(is_void_v<common_reference_t<const uu, const u>>);
  }
  // u and uu, lvalue reference and lvalue reference
  {
    // static_assert(is_same_v<inner::common_ref_t<u &, uu &>, inner::disable>);
    // static_assert(is_void_v<common_reference_t<u &, uu &>>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u &, uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<const u &, uu &>, const u &>);
    static_assert(is_same_v
                  <inner::common_ref_t<u &, const uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<u &, const uu &>, const u &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u &, const uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<const u &, const uu &>,
                            const u &>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u &, volatile uu &>,
                   inner::disable>);
    static_assert(is_void_v<common_reference_t<const u &, volatile uu &>>);
    static_assert(is_same_v
                  <inner::common_ref_t<u &, volatile uu &>, inner::disable>);
    static_assert(is_void_v<common_reference_t<u &, volatile uu &>>);
  }
  // u and uu, rvalue reference and rvalue reference
  {
    static_assert(is_same_v<inner::common_ref_t<u &&, uu &&>, inner::disable>);
    static_assert(is_void_v<common_reference_t<u &&, uu &&>>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u &&, uu &&>, inner::disable>);
    static_assert(is_void_v<common_reference_t<const u &&, uu &&>>);
    static_assert(is_same_v
                  <inner::common_ref_t<u &&, const uu &&>, inner::disable>);
    static_assert(is_void_v<common_reference_t<u &&, const uu &&>>);
    static_assert(is_same_v
                  <inner::common_ref_t<const u &&, const uu &&>,
                   inner::disable>);
    static_assert(is_void_v<common_reference_t<const u &&, const uu &&>>);
    static_assert(is_same_v<inner::common_ref_t<const u &&, volatile uu &&>,
                            inner::disable>);
    static_assert(is_void_v<common_reference_t<const u &&, volatile uu &&>>);
    static_assert(is_same_v
                  <inner::common_ref_t<u &&, volatile uu &&>, inner::disable>);
    static_assert(is_void_v<common_reference_t<u &&, volatile uu &&>>);
  }
  // u and uu, differenct references
  {
    static_assert(is_same_v<inner::common_ref_t<u &, uu &&>, const u &>);
    static_assert(is_same_v<common_reference_t<u &, uu &&>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<const u &, uu &&>, const u &>);
    static_assert(is_same_v<common_reference_t<const u &, uu &&>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<u &, const uu &&>, const u &>);
    static_assert(is_same_v<common_reference_t<u &, const uu &&>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<const u &, const uu &&>,
                            const u &>);
    static_assert(is_same_v<common_reference_t<const u &, const uu &&>,
                            const u &>);
    static_assert(is_same_v<inner::common_ref_t<u &&, uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<u &&, uu &>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<const u &&, uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<const u &&, uu &>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<u &&, const uu &>, const u &>);
    static_assert(is_same_v<common_reference_t<u &&, const uu &>, const u &>);
    static_assert(is_same_v<inner::common_ref_t<const u &&, const uu &>,
                            const u &>);
    static_assert(is_same_v<common_reference_t<const u &&, const uu &>,
                            const u &>);
  }
}
void case34_conditional_result_or_common_type() {
  static_assert(is_same_v<common_type_t<t, tt>, t>);
  static_assert(is_same_v<common_type_t<v, vv>, nullptr_t>);
  static_assert(is_same_v<common_type_t<vv, v>, nullptr_t>);
  static_assert(is_null_pointer_v
                <common_reference_t<const vv &, volatile v &>>);
  static_assert(is_null_pointer_v
                <common_type_t<const w &, volatile ww &>>);
  static_assert(is_null_pointer_v
                <common_type_t<const ww &, volatile w &>>);
  static_assert(is_same_v<common_reference_t<w, ww &>, w>);
  static_assert(is_same_v<common_reference_t<ww &, w>, w>);
}
void other_than_two_arguments() {
  static_assert(!has_member_type_type_v<common_reference<>>);
  static_assert(is_same_v<common_reference<int>::type, int>);
  static_assert(is_same_v<common_reference_t<int>, int>);
  static_assert(is_same_v<common_reference<void>::type, void>);
  static_assert(is_same_v<common_reference_t<void>, void>);
  static_assert(is_same_v
                <common_reference_t
                 <w &, const ww &>, w>);
  static_assert(!has_member_type_type_v
                <common_reference<w &, const ww &, v, vv>>);
  static_assert(is_same_v
                <common_reference_t<z &, const zz &>, z>);
  static_assert(is_same_v
                <common_reference_t<z &, const zz &, z &&, const zz &&>, z>);

  struct type {};
  struct type2 : type {};
  struct type3 : type2 {};
  static_assert(is_same_v<common_reference_t
                          <type &, const type2 &, const volatile type3 &>,
                          const volatile type &>);
  static_assert(is_same_v<common_reference_t
                          <type3 &, const type2 &, const volatile type &>,
                          const volatile type &>);
}

}

// help test_pointer_traits
namespace re::inner::fns {

template <class T>
struct test_pointer_1 {
  using element_type = void;
};
template <class T>
struct test_pointer_2 {};
template <class T>
struct test_pointer_3 {};
template <class T>
struct test_pointer_4 {
  using difference_type = void;
};
template <class T>
struct test_pointer_5 {
  template <class T_>
  using rebind = T_;
};
template <class T>
struct test_pointer_6 {
  static test_pointer_6 pointer_to(int &i) {
    i = 1;
    return {};
  }
};
template <class T>
struct test_pointer_7 {
  static test_pointer_7 pointer_to(T &i) {
    i = 1;
    return {};
  }
};
template <class T>
struct test_pointer_8 {
  int x;
};

}
namespace re {

template <class T>
struct pointer_traits<inner::fns::test_pointer_8<T>> {
  static int to_address(inner::fns::test_pointer_8<T> p) noexcept {
    return p.x * 2;
  }
};

}
namespace re::inner::fns {

template <class T>
struct test_pointer_9 {
  int *x;

  int *operator ->() const noexcept {
    return x + 4;
  }
};

}

// help test_tuple
namespace re::inner::fns::help_tuple {

struct t {
  int value;

  t();
  t(allocator_arg_t, int y, int x);
  t(int x, int y);
};
struct tt {
  int value;

  tt();
  tt(int x, int y);
};

}
namespace re {

template <class X>
struct uses_allocator<inner::fns::help_tuple::t, X> : true_type {};
template <class X>
struct uses_allocator<inner::fns::help_tuple::tt, X> : true_type {};

}
namespace re::inner::fns::help_tuple {

void test_uses_allocator_construction_args() {
  static_assert(is_trivial_empty_v
                <decltype(uses_allocator_construction_args<int>)>);

  static_assert(uses_allocator_v<tuple<>, int>);
  static_assert(uses_allocator_v<tuple<int, int>, int>);
  static_assert(uses_allocator_v<tuple<int, int, int>, char>);

  static_assert(!uses_allocator_v<int, int>);
  using t0 = decltype(uses_allocator_construction_args<int>(1, 2));
  static_assert(is_same_v<t0, tuple<int &&>>);
  assert(uses_allocator_construction_args<int>(1, 2).at<0>() == 2);
  static_assert(uses_allocator_construction_args<int>(1, 2).at<0>() == 2);

  static_assert(uses_allocator_v<t, const int &>);
  using t1 = decltype(uses_allocator_construction_args<t>(1, 2));
  static_assert(is_same_v<t1, tuple<allocator_arg_t, const int &, int &&>>);
  assert(uses_allocator_construction_args<t>(1, 2).at<1>() == 1);
  assert(uses_allocator_construction_args<t>(1, 2).at<2>() == 2);
  static_assert(uses_allocator_construction_args<t>(1, 2).at<1>() == 1);
  static_assert(uses_allocator_construction_args<t>(1, 2).at<2>() == 2);

  static_assert(uses_allocator_v<tt, const int &>);
  using t2 = decltype(uses_allocator_construction_args<t>(1, 2, 3));
  static_assert(is_same_v<t2, tuple<int &&, int &&, const int &>>);
  assert(uses_allocator_construction_args<t>(1, 2, 3).at<0>() == 2);
  assert(uses_allocator_construction_args<t>(1, 2, 3).at<1>() == 3);
  assert(uses_allocator_construction_args<t>(1, 2, 3).at<2>() == 1);
  static_assert(uses_allocator_construction_args<t>(1, 2, 3).at<0>() == 2);
  static_assert(uses_allocator_construction_args<t>(1, 2, 3).at<1>() == 3);
  static_assert(uses_allocator_construction_args<t>(1, 2, 3).at<2>() == 1);

  using ttt = tuple<tuple<int, int>, int>;
  using t3 = decltype(uses_allocator_construction_args<ttt>
                      (1, make_tuple(1, 2), 3));
  static_assert(tuple_size_v<t3> == 4);
  static_assert(same_as
                <t3,
                 tuple<allocator_arg_t, const int &,
                       tuple<int, int> &&, int &&>>);
  assert(uses_allocator_construction_args<ttt>
         (1, make_tuple(1, 2), 3).at<1>() == 1);
  assert(uses_allocator_construction_args<ttt>
         (1, make_tuple(1, 2), 3).at<2>() == tuple(1, 2));
  assert(uses_allocator_construction_args<ttt>
         (1, make_tuple(1, 2), 3).at<3>() == 3);
  static_assert(uses_allocator_construction_args<ttt>
                (1, make_tuple(1, 2), 3).at<1>() == 1);
  static_assert(uses_allocator_construction_args<ttt>
                (1, make_tuple(1, 2), 3).at<2>() == tuple(1, 2));
  static_assert(uses_allocator_construction_args<ttt>
                (1, make_tuple(1, 2), 3).at<3>() == 3);
}

struct unmoveable_t {
  unmoveable_t(unmoveable_t &&) = delete;
  unmoveable_t &operator =(unmoveable_t &&) = delete;
  unmoveable_t(const unmoveable_t &) = delete;
  unmoveable_t &operator =(const unmoveable_t &) = delete;
};
struct only_moveable_t {
  only_moveable_t(only_moveable_t &&);
  only_moveable_t &operator =(only_moveable_t &&);
  only_moveable_t(const only_moveable_t &) = delete;
  only_moveable_t &operator =(const only_moveable_t &) = delete;
};

struct const_swappable_t {
  int value;
};
void swap(const const_swappable_t &, const const_swappable_t &) {}

template <template <class T> class enabled, class T, bool y, bool yy = true>
constexpr bool test_conditional_smf() {
  if (!(enabled<tuple<int>>::value == yy
        && enabled<tuple<T>>::value == y))
    return false;

  if (!(enabled<tuple<int, int>>::value == yy
        && enabled<tuple<int, T>>::value == y
        && enabled<tuple<T, int>>::value == y
        && enabled<tuple<T, T>>::value == y))
    return false;

  if (!(enabled<tuple<int, int, int>>::value == yy
        && enabled<tuple<T, int, int>>::value == y
        && enabled<tuple<int, T, int>>::value == y
        && enabled<tuple<int, int, T>>::value == y
        && enabled<tuple<T, T, int>>::value == y
        && enabled<tuple<T, int, T>>::value == y
        && enabled<tuple<int, T, T>>::value == y
        && enabled<tuple<T, T, T>>::value == y))
    return false;

  if (!(enabled<tuple<int, int, int, int, int, int>>::value == yy
        && enabled<tuple<int, int, int, T, int, int>>::value == y))
    return false;

  return true;
}

}


namespace re::inner::fns {

namespace check_integral_constant {

template <class X, class Y, Y y>
void f() {
  static_assert(X::value == y);
  static_assert(is_same_v<typename X::value_type, Y>);
  static_assert(is_same_v<typename X::type, integral_constant<Y, y>>);
  static_assert(X() == y);
  static_assert(X()() == y);
  constexpr X x;
  static_assert(x() == y);
}

}
namespace test_compile_time_acc {

template <class X, class Y>
struct f {};
template <class X, class Y>
requires integral<X> && integral<Y>
struct f<X, Y> {using type = int;};

}
void test_basic_tmp_tools() {
  // declval
  {
    static_assert(is_same_v<decltype(declval<int>()), int &&>);
    static_assert(is_same_v<decltype(declval<int &&>()), int &&>);
    static_assert(is_same_v<decltype(declval<int &>()), int &>);

    static_assert(is_same_v<decltype(declval<const int>()), const int &&>);
    static_assert(is_same_v<decltype(declval<const int &>()), const int &>);
    static_assert(is_same_v<decltype(declval<const int &&>()), const int &&>);

    static_assert(is_trivial_empty_v<decltype(declval<int>)>);
    static_assert(noexcept(declval<int>()));
  }

  // integer_sequence
  {
    static_assert(is_same_v
                  <make_integer_sequence<char, 0>, integer_sequence<char>>);
    static_assert(is_same_v
                  <make_integer_sequence<char, 3>,
                   integer_sequence<char, 0, 1, 2>>);

    static_assert(is_same_v
                  <index_sequence<1, 3, 2>,
                   integer_sequence<size_t, 1, 3, 2>>);
    static_assert(is_same_v
                  <make_index_sequence<5>,
                   integer_sequence<size_t, 0, 1, 2, 3, 4>>);
    static_assert(is_same_v<make_index_sequence<0>, index_sequence<>>);
    static_assert(is_same_v<make_index_sequence<1>, index_sequence<0>>);
    static_assert(is_same_v<make_index_sequence<2>, index_sequence<0, 1>>);

    static_assert(is_same_v
                  <make_index_sequence_by_stride<0, 4>,
                  index_sequence<>>);
    static_assert(is_same_v
                  <make_index_sequence_by_stride<1, 4>,
                  index_sequence<0>>);
    static_assert(is_same_v
                  <make_index_sequence_by_stride<3, 4>,
                  index_sequence<0, 4, 8>>);

    static_assert(is_same_v<integer_sequence_cat<int>, integer_sequence<int>>);
    static_assert(is_same_v
                  <integer_sequence_cat<int, integer_sequence<int, 1, 2, 3>>,
                   integer_sequence<int, 1, 2, 3>>);
    static_assert(is_same_v
                  <integer_sequence_cat
                   <int,
                    integer_sequence<int, 1, 2, 3>,
                    integer_sequence<int, 4, 5>>,
                   integer_sequence<int, 1, 2, 3, 4, 5>>);
    static_assert(is_same_v
                  <integer_sequence_cat
                   <int,
                    integer_sequence<int, 1, 2, 3>,
                    integer_sequence<int, 4, 5>,
                    integer_sequence<int>,
                    integer_sequence<int, 6>,
                    integer_sequence<int, 7, 8, 9>>,
                   integer_sequence<int, 1, 2, 3, 4, 5, 6, 7, 8, 9>>);
    static_assert(is_same_v
                  <index_sequence_cat
                   <index_sequence<1>, index_sequence<2>,
                    index_sequence<3, 4>>,
                   index_sequence<1, 2, 3, 4>>);

    static_assert(is_same_v
                  <integer_sequence_offset
                   <int, integer_sequence<int, 0, 1, 2>, 1>,
                   integer_sequence<int, 1, 2, 3>>);
    static_assert(is_same_v
                  <index_sequence_offset<index_sequence<>, 1>,
                   index_sequence<>>);
    static_assert(is_same_v
                  <index_sequence_offset<index_sequence<0, 1, 2>, 1>,
                   index_sequence<1, 2, 3>>);
  }

  // integral_constant
  {
    using b = integral_constant<bool, true>;
    static_assert(b::value == true);
    static_assert(is_same_v<b::value_type, bool>);
    static_assert(is_same_v<b::type, bool_constant<true>>);
    static_assert(b() == true);
    static_assert(b()() == true);
    assert(noexcept(b()()));

    using c = integral_constant<size_t, 3>;
    assert(c::value == 3);
    static_assert(is_same_v<c::value_type, size_t>);
    static_assert(is_same_v<c::type, integral_constant<size_t, 3>>);
    auto i = integral_constant<size_t, 3>{};
    size_t j = i;
    assert(j == 3);
    static_assert(i() == 3);
    static_assert(noexcept(i()));

    static_assert(is_same_v<false_type, integral_constant<bool, false>>);
    static_assert(is_same_v<true_type, integral_constant<bool, true>>);
    static_assert(is_same_v<size_constant<2>, integral_constant<size_t, 2>>);
    static_assert(is_same_v<int_constant<2>, integral_constant<int, 2>>);

    static_assert(is_trivial_empty_v<integral_constant<int, 0>>);
  }

  // f_is_well_formed
  // return_type_of_f_or
  {
    struct check_t {
      static type_tag<int> f(type_pack<int>);
      static type_tag<int> f(type_pack<int, float>);

      static inner::disable f(type_pack<int, int, int, int>);
      static void f(type_pack<int, int, int, int *>);

      static double f(type_pack<double>);
      static int f(void *);
    };

    static_assert(f_is_well_formed<check_t, int>::value);
    static_assert(f_is_well_formed_v<check_t, int>);
    static_assert(!f_is_well_formed_v<check_t &, int>);

    static_assert(f_is_well_formed_v<check_t, int, float>);
    static_assert(!f_is_well_formed_v<const check_t &, int, float>);

    static_assert(!f_is_well_formed_v<check_t>);
    static_assert(!f_is_well_formed_v<void>);

    static_assert(!f_is_well_formed_v<check_t, void>);
    static_assert(!f_is_well_formed_v<check_t, int &>);
    static_assert(!f_is_well_formed_v<check_t, float, int>);

    static_assert(!f_is_well_formed_v<check_t, int, int, int, int>);
    static_assert(f_is_well_formed_v<check_t, int, int, int, int*>);

    static_assert(is_same_v
                  <return_type_of_f_or<check_t, void, int>::type, int>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<check_t, void, int>, int>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<check_t &, void, int>, void>);

    static_assert(is_same_v
                  <return_type_of_f_or_t<check_t, void, int &, float>, void>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<check_t, void, double>, void>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<check_t, void, void *>, void>);

    static_assert(is_same_v
                  <return_type_of_f_or_t<float, int>, int>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<void, int>, int>);
    static_assert(is_same_v
                  <return_type_of_f_or_t<bool, int>, int>);

    static_assert(is_trivial_empty_v<f_is_well_formed<check_t, int>>);
    static_assert(is_trivial_empty_v<return_type_of_f_or<void, int>>);
  }

  // has_member_type_type
  {
    struct t0 {
      void type() {}
    };
    struct t {
      using type = void;
    };
    struct tt {};

    static_assert(is_trivial_empty_v<has_member_type_type<void>>);
    static_assert(!has_member_type_type<volatile void>::value);
    static_assert(!has_member_type_type_v<volatile void>);
    static_assert(!has_member_type_type_v<int>);
    static_assert(!has_member_type_type_v<t0>);
    static_assert(has_member_type_type_v<t>);
    static_assert(has_member_type_type_v<const t>);
    static_assert(has_member_type_type_v<volatile t>);
    static_assert(has_member_type_type_v<const volatile t>);
    static_assert(!has_member_type_type_v<t &>);
    static_assert(!has_member_type_type_v<tt>);

    static_assert(is_trivial_empty_v<has_member_static_value<void>>);
    static_assert(!has_member_static_value<void>::value);
    static_assert(!has_member_static_value_v<void>);
    static_assert(!has_member_static_value_v<const void>);
    static_assert(!has_member_static_value_v<const volatile void>);
    static_assert(!has_member_static_value_v<int>);
    static_assert(has_member_static_value_v<int_constant<3>>);
    static_assert(has_member_static_value_v<const int_constant<3>>);
    static_assert(has_member_static_value_v<volatile int_constant<3>>);
    static_assert(has_member_static_value_v<const volatile int_constant<3>>);
    static_assert(has_member_static_value_v<bool_constant<true>>);
    static_assert(!has_member_static_value_v<bool_constant<true> &>);
    static_assert(!has_member_static_value_v<const bool_constant<true> &>);
    static_assert(!has_member_static_value_v<volatile bool_constant<true> &>);
    static_assert(!has_member_static_value_v
                  <const volatile bool_constant<true> &&>);
    static_assert(!has_member_static_value_v<bool_constant<true> &&>);
  }

  // nth_type
  {
    static_assert(is_trivial_empty_v<nth_type<0, int>>);
    static_assert(is_same_v<nth_type_t<0>, void>);
    static_assert(is_same_v<nth_type_t<1001>, void>);
    static_assert(is_same_v<nth_type_t<0, int, float>, int>);
    static_assert(is_same_v<nth_type_t<1, int, float>, float>);
    static_assert(is_same_v<nth_type_t<2, int, float>, void>);
    static_assert(is_same_v<nth_type_t<222, int, float>, void>);
    static_assert(is_same_v<nth_type<1, int, float, double>::type, float>);
    static_assert(is_same_v<nth_type_t<1, int, float, double>, float>);
    static_assert(is_same_v<nth_type_t<2, int, float, double>, double>);
    static_assert(is_same_v<nth_type_t<3, int, float, double>, void>);
    static_assert(is_same_v<nth_type_t<33, int, float, double>, void>);
    static_assert(is_same_v<nth_type_t<999, int, float, double>, void>);
  }

  // is_one_of_types
  {
    static_assert(is_trivial_empty_v<is_one_of_types<int, int, float>>);
    static_assert(is_base_of_v<true_type, is_one_of_types<int, int, float>>);
    static_assert(is_base_of_v
                  <false_type, is_one_of_types<double, int, float>>);
    static_assert(is_one_of_types<int, int, float>::value == true);
    static_assert(is_one_of_types_v<int, int>);
    static_assert(!is_one_of_types_v<int>);
    static_assert(is_one_of_types_v<float, float, int, int>);
    static_assert(is_one_of_types_v<int, float, int, int>);
    static_assert(is_one_of_types_v<int, float, int, int, long>);
    static_assert(!is_one_of_types_v<int, float, long, double, double>);
    static_assert(is_one_of_types_v<double, float, long, double, double>);
    static_assert(!is_one_of_types_v<double, float, long, double &>);
    static_assert(!is_one_of_types_v<double, float, long, const double>);
  }

  // find_type
  {
    static_assert(is_trivial_empty_v<find_type<char, char, float>>);
    static_assert(find_type<char, char, float>::value == 0);
    static_assert(find_type<float, char, float>::value == 1);
    static_assert(find_type<void, char, float>::value == 2);
    static_assert(is_base_of_v<size_constant<0>, find_type<int, int>>);
    static_assert(is_base_of_v<size_constant<0>, find_type<int, int, int>>);
    static_assert(is_base_of_v<size_constant<1>, find_type<int, float, int>>);
    static_assert(is_base_of_v
                  <size_constant<2>, find_type<double, float, int>>);
    static_assert(find_type_v<int> == 0);
    static_assert(find_type_v<int, int> == 0);
    static_assert(find_type_v<double, int> == 1);
    static_assert(find_type_v<int, int, int, int, int, int> == 0);
    static_assert(find_type_v<double, int, int, int, int, double> == 4);
    static_assert(find_type_v<double *, int, int, int, int, double> == 5);
    static_assert(find_type_v<double ****, int, int, int, int, double> == 5);
  }

  // occurs_exactly_once
  {
    static_assert(is_trivial_empty_v<occurs_exactly_once<int>>);
    static_assert(occurs_exactly_once<int>::value == false);
    static_assert(occurs_exactly_once<int, int>::value == true);
    static_assert(occurs_exactly_once<int, int, int>::value == false);
    static_assert(occurs_exactly_once<int, int, char, int>::value == false);
    static_assert(is_base_of_v<false_type, occurs_exactly_once<int>>);
    static_assert(is_base_of_v<true_type, occurs_exactly_once<float, float>>);
    static_assert(!occurs_exactly_once_v<int>);
    static_assert(occurs_exactly_once_v<int, int>);
    static_assert(!occurs_exactly_once_v<int, int, int>);
    static_assert(occurs_exactly_once_v<int, int, int &>);
    static_assert(occurs_exactly_once_v<int, int, const int>);
    static_assert(occurs_exactly_once_v<int, int, const volatile int>);
    static_assert(occurs_exactly_once_v<int, int, const volatile int &>);
    static_assert(!occurs_exactly_once_v<int, int, int, int>);
    static_assert(occurs_exactly_once_v<int, char, int>);
    static_assert(occurs_exactly_once_v<int, int, char>);
    static_assert(occurs_exactly_once_v<int, char, int, long>);
    static_assert(occurs_exactly_once_v<int, char, long, int>);
    static_assert(occurs_exactly_once_v<int, int, char, long>);
    static_assert(occurs_exactly_once_v<int, long, char, int>);
    static_assert(occurs_exactly_once_v<int, int, long, char>);
    static_assert(occurs_exactly_once_v<int, long, int, char>);
    static_assert(!occurs_exactly_once_v<int, long, int, char, int>);
    static_assert(!occurs_exactly_once_v
                  <int, long, int, char, int, int, float>);
    static_assert(occurs_exactly_once_v
                  <char, long, int, char, int, int, float>);
    static_assert(occurs_exactly_once_v
                  <long, long, int, char, int, int, float>);
    static_assert(occurs_exactly_once_v
                  <float, long, int, char, int, int, float>);
  }

  // compile_time_add
  {
    static_assert(is_trivial_empty_v
                  <compile_time_add<integral_constant<char, 1>,
                                    integral_constant<char, 2>>>);
    check_integral_constant::f<compile_time_add<integral_constant<char, 1>,
                                                integral_constant<char, 2>>,
                               char, 3>();
    check_integral_constant::f<compile_time_add
                               <integral_constant<char, 1>,
                                integral_constant<short, 2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_add<integral_constant<char, 1>,
                                                integral_constant<int, 2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_add<integral_constant<int, 1>,
                                                integral_constant<int, 2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_add<integral_constant<unsigned, 1>,
                                                integral_constant<int, 2>>,
                               unsigned, 3>();
    check_integral_constant::f<compile_time_add_t
                               <integral_constant<int, 1>,
                                integral_constant<long long, 2>>,
                               long long, 3>();
    check_integral_constant::f<compile_time_add_t
                               <integral_constant<int, 1>,
                                integral_constant<long long, -2>>,
                               long long, -1>();
    static_assert(compile_time_add_v
                  <size_constant<1>, size_constant<2>> == 3);
    static_assert(compile_time_add_v
                  <bool_constant<1>, size_constant<2>> == 3);
    static_assert(compile_time_add_v<int_constant<1>, int_constant<1>> == 2);
    static_assert(compile_time_add_v<int_constant<-1>, int_constant<1>> == 0);
    static_assert(compile_time_add_v<int_constant<-1>, int_constant<2>> == 1);
    {
      constexpr auto v = compile_time_add_v
        <integral_constant<char, 1>, integral_constant<char, 2>>;
      constexpr auto v2 = compile_time_add_v
        <integral_constant<int, 1>, integral_constant<char, 2>>;
      static_assert(is_same_v<decltype(v), const char>);
      static_assert(v == 3);
      static_assert(is_same_v<decltype(v2), const int>);
      static_assert(v2 == 3);
    }
  }
  // compile_time_sub
  {
    static_assert(is_trivial_empty_v
                  <compile_time_sub<integral_constant<char, 1>,
                                    integral_constant<char, 2>>>);
    check_integral_constant::f<compile_time_sub<integral_constant<char, 1>,
                                                integral_constant<char, 2>>,
                               char, -1>();
    check_integral_constant::f<compile_time_sub
                               <integral_constant<char, 1>,
                                integral_constant<short, 2>>,
                               int, -1>();
    check_integral_constant::f<compile_time_sub<integral_constant<char, 1>,
                                                integral_constant<int, 2>>,
                               int, -1>();
    check_integral_constant::f<compile_time_sub<integral_constant<int, 1>,
                                                integral_constant<int, 2>>,
                               int, -1>();
    check_integral_constant::f<compile_time_sub<integral_constant<unsigned, 2>,
                                                integral_constant<int, 1>>,
                               unsigned, 1>();
    check_integral_constant::f<compile_time_sub_t
                               <integral_constant<int, 1>,
                                integral_constant<long long, 2>>,
                               long long, -1>();
    check_integral_constant::f<compile_time_sub_t
                               <integral_constant<int, 1>,
                                integral_constant<long long, -2>>,
                               long long, 3>();
    static_assert(compile_time_sub_v
                  <size_constant<2>, size_constant<1>> == 1);
    static_assert(compile_time_sub_v
                  <bool_constant<1>, size_constant<1>> == 0);
    static_assert(compile_time_sub_v<int_constant<1>, int_constant<1>> == 0);
    static_assert(compile_time_sub_v<int_constant<-1>, int_constant<1>> == -2);
    static_assert(compile_time_sub_v<int_constant<-1>, int_constant<2>> == -3);
    {
      constexpr auto v = compile_time_sub_v
        <integral_constant<char, 1>, integral_constant<char, 2>>;
      constexpr auto v2 = compile_time_sub_v
        <integral_constant<int, 1>, integral_constant<char, 2>>;
      static_assert(is_same_v<decltype(v), const char>);
      static_assert(v == -1);
      static_assert(is_same_v<decltype(v2), const int>);
      static_assert(v2 == -1);
    }
  }
  // compile_time_mul
  {
    static_assert(is_trivial_empty_v
                  <compile_time_mul<integral_constant<char, 2>,
                                    integral_constant<char, 3>>>);
    check_integral_constant::f<compile_time_mul<integral_constant<char, 2>,
                                                integral_constant<char, 3>>,
                               char, 6>();
    check_integral_constant::f<compile_time_mul
                               <integral_constant<char, 2>,
                                integral_constant<short, 3>>,
                               int, 6>();
    check_integral_constant::f<compile_time_mul<integral_constant<char, 2>,
                                                integral_constant<int, -3>>,
                               int, -6>();
    check_integral_constant::f<compile_time_mul<integral_constant<int, 2>,
                                                integral_constant<int, -3>>,
                               int, -6>();
    check_integral_constant::f<compile_time_mul<integral_constant<unsigned, 2>,
                                                integral_constant<int, 3>>,
                               unsigned, 6>();
    check_integral_constant::f<compile_time_mul_t
                               <integral_constant<int, 2>,
                                integral_constant<long long, 3>>,
                               long long, 6>();
    check_integral_constant::f<compile_time_mul_t
                               <integral_constant<int, 2>,
                                integral_constant<long long, -3>>,
                               long long, -6>();
    static_assert(compile_time_mul_v
                  <size_constant<3>, size_constant<4>> == 12);
    static_assert(compile_time_mul_v
                  <bool_constant<0>, size_constant<3>> == 0);
    static_assert(compile_time_mul_v<int_constant<1>, int_constant<1>> == 1);
    static_assert(compile_time_mul_v<int_constant<-1>, int_constant<1>> == -1);
    static_assert(compile_time_mul_v<int_constant<-2>, int_constant<3>> == -6);
    {
      constexpr auto v = compile_time_mul_v
        <integral_constant<char, 3>, integral_constant<char, 2>>;
      constexpr auto v2 = compile_time_mul_v
        <integral_constant<int, 3>, integral_constant<char, 2>>;
      static_assert(is_same_v<decltype(v), const char>);
      static_assert(v == 6);
      static_assert(is_same_v<decltype(v2), const int>);
      static_assert(v2 == 6);
    }
  }
  // compile_time_div
  {
    static_assert(is_trivial_empty_v
                  <compile_time_div<integral_constant<char, 6>,
                                    integral_constant<char, 2>>>);
    check_integral_constant::f<compile_time_div<integral_constant<char, 6>,
                                                integral_constant<char, 2>>,
                               char, 3>();
    check_integral_constant::f<compile_time_div
                               <integral_constant<char, 6>,
                                integral_constant<short, 2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_div<integral_constant<char, -6>,
                                                integral_constant<int, -2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_div<integral_constant<int, -6>,
                                                integral_constant<int, -2>>,
                               int, 3>();
    check_integral_constant::f<compile_time_div<integral_constant<unsigned, 6>,
                                                integral_constant<int, 3>>,
                               unsigned, 2>();
    check_integral_constant::f<compile_time_div_t
                               <integral_constant<int, 6>,
                                integral_constant<long long, -3>>,
                               long long, -2>();
    check_integral_constant::f<compile_time_div_t
                               <integral_constant<int, 6>,
                                integral_constant<long long, -3>>,
                               long long, -2>();
    static_assert(compile_time_div_v
                  <size_constant<10>, size_constant<2>> == 5);
    static_assert(compile_time_div_v
                  <int_constant<1>, size_constant<3>> == 0);
    static_assert(compile_time_div_v<int_constant<1>, int_constant<1>> == 1);
    static_assert(compile_time_div_v<int_constant<-1>, int_constant<1>> == -1);
    static_assert(compile_time_div_v<int_constant<-2>, int_constant<3>> == 0);
    {
      constexpr auto v = compile_time_div_v
        <integral_constant<char, 6>, integral_constant<char, -2>>;
      constexpr auto v2 = compile_time_div_v
        <integral_constant<int, 6>, integral_constant<char, -2>>;
      static_assert(is_same_v<decltype(v), const char>);
      static_assert(v == -3);
      static_assert(is_same_v<decltype(v2), const int>);
      static_assert(v2 == -3);
    }
  }
  // compile_time_mod
  {
    static_assert(is_trivial_empty_v
                  <compile_time_mod<integral_constant<char, 6>,
                                    integral_constant<char, 2>>>);
    check_integral_constant::f<compile_time_mod<integral_constant<char, 6>,
                                                integral_constant<char, 2>>,
                               char, 0>();
    check_integral_constant::f<compile_time_mod
                               <integral_constant<char, 6>,
                                integral_constant<short, 2>>,
                               int, 0>();
    check_integral_constant::f<compile_time_mod<integral_constant<char, -6>,
                                                integral_constant<int, -2>>,
                               int, 0>();
    check_integral_constant::f<compile_time_mod<integral_constant<int, -6>,
                                                integral_constant<int, -2>>,
                               int, 0>();
    check_integral_constant::f<compile_time_mod<integral_constant<unsigned, 6>,
                                                integral_constant<int, 3>>,
                               unsigned, 0>();
    check_integral_constant::f<compile_time_mod_t
                               <integral_constant<int, 6>,
                                integral_constant<long long, -4>>,
                               long long, 2>();
    check_integral_constant::f<compile_time_mod_t
                               <integral_constant<int, 6>,
                                integral_constant<long long, 1>>,
                               long long, 0>();
    static_assert(compile_time_mod_v
                  <size_constant<10>, size_constant<2>> == 0);
    static_assert(compile_time_mod_v
                  <int_constant<1>, size_constant<3>> == 1);
    static_assert(compile_time_mod_v<int_constant<1>, int_constant<1>> == 0);
    static_assert(compile_time_mod_v
                  <int_constant<-15>, int_constant<-4>> == -3);
    static_assert(compile_time_mod_v<int_constant<15>, int_constant<4>> == 3);
    {
      constexpr auto v = compile_time_mod_v
        <integral_constant<char, 7>, integral_constant<char, -2>>;
      constexpr auto v2 = compile_time_mod_v
        <integral_constant<int, 7>, integral_constant<char, -2>>;
      static_assert(is_same_v<decltype(v), const char>);
      static_assert(v == 1);
      static_assert(is_same_v<decltype(v2), const int>);
      static_assert(v2 == 1);
    }
    static_assert(is_same_v<compile_time_mod
                            <integral_constant<int, 17>,
                             integral_constant<long long, 4>>::type,
                            integral_constant<long long, 1>>);
    static_assert(is_same_v<compile_time_mod
                            <integral_constant<int, -17>,
                             integral_constant<long long, 4>>::type,
                            integral_constant<long long, -1>>);
    static_assert(is_same_v<compile_time_mod_t
                            <integral_constant<int, -17>,
                             integral_constant<long long, 4>>,
                            integral_constant<long long, -1>>);
    static_assert(compile_time_mod_t
                  <int_constant<7>, int_constant<4>>::value == 3);
    static_assert(compile_time_mod
                  <int_constant<7>, int_constant<4>>::type::value == 3);
    static_assert(compile_time_mod
                  <int_constant<7>, int_constant<4>>::value == 3);
    static_assert(compile_time_mod_v<int_constant<7>, int_constant<4>> == 3);
    static_assert(compile_time_mod_v<int_constant<7>, int_constant<-4>> == 3);
    static_assert(compile_time_mod_v<int_constant<-7>, int_constant<-4>> == -3);
    static_assert(compile_time_mod_v<int_constant<-7>, int_constant<4>> == -3);
    static_assert(compile_time_mod_v
                  <size_constant<6>, size_constant<2>> == 0);
    static_assert(compile_time_mod_v
                  <integral_constant<int, 15>, integral_constant<int, 4>> == 3);
    static_assert(compile_time_mod_v
                  <integral_constant<int, -15>, integral_constant<int, 4>>
                  == -3);
    static_assert(compile_time_mod_v
                  <integral_constant<short, 0>, size_constant<2>> == 0);
  }
  // compile_time_neg
  {
    static_assert(is_trivial_empty_v
                  <compile_time_neg<integral_constant<char, -3>>>);
    check_integral_constant::f<compile_time_neg<integral_constant<char, -3>>,
                               char, 3>();
    check_integral_constant::f<compile_time_neg_t<integral_constant<char, 3>>,
                               char, -3>();
    static_assert(is_base_of_v<integral_constant<char, -3>,
                               compile_time_neg<integral_constant<char, 3>>>);
    static_assert(is_same_v<compile_time_neg_t
                            <integral_constant<char, 3>>,
                            integral_constant<char, -3>>);
    static_assert(is_same_v<compile_time_neg
                            <integral_constant<char, 3>>::type,
                            integral_constant<char, -3>>);
    static_assert(compile_time_neg<integral_constant<char, 3>>::value == -3);
    static_assert(compile_time_neg<integral_constant<char, 0>>::value == 0);
  }
  // compile_time_abs
  {
    static_assert(is_trivial_empty_v
                  <compile_time_abs<integral_constant<char, -3>>>);
    check_integral_constant::f<compile_time_abs<integral_constant<char, -3>>,
                               char, 3>();
    check_integral_constant::f<compile_time_abs<integral_constant<char, 3>>,
                               char, 3>();
    check_integral_constant::f<compile_time_abs<int_constant<-666>>,
                               int, 666>();
    static_assert(is_same_v<compile_time_abs
                            <integral_constant<char, 0>>::type,
                            integral_constant<char, 0>>);
    static_assert(is_same_v<compile_time_abs
                            <integral_constant<char, 1>>::type,
                            integral_constant<char, 1>>);
    static_assert(is_same_v<compile_time_abs_t
                            <integral_constant<char, -3>>,
                             integral_constant<char, 3>>);
    static_assert(compile_time_abs<integral_constant<char, 3>>::value == 3);
    static_assert(compile_time_abs_v<integral_constant<char, -3>> == 3);
  }
  // compile_time_gcd
  {
    static_assert(is_trivial_empty_v
                  <compile_time_gcd<int_constant<569>, int_constant<137>>>);
    check_integral_constant::f<compile_time_gcd
                               <int_constant<569>, int_constant<137>>,
                               int, 1>();
    static_assert(is_same_v<compile_time_gcd
                            <integral_constant<int, 8>,
                             integral_constant<int, 12>>::type,
                            int_constant<4>>);
    static_assert(is_same_v<compile_time_gcd_t
                            <integral_constant<int, 8>,
                             integral_constant<int, 12>>,
                            int_constant<4>>);
    static_assert(is_base_of_v<int_constant<4>,
                               compile_time_gcd
                               <integral_constant<int, 8>,
                                integral_constant<int, 12>>>);
    static_assert(compile_time_gcd
                  <integral_constant<char, 8>,
                   integral_constant<short, 12>>::value == 4);
    static_assert(is_same_v<compile_time_gcd_t
                            <integral_constant<short, 8>,
                             integral_constant<short, 12>>,
                            integral_constant<short, 4>>);
    static_assert(compile_time_gcd
                  <integral_constant<int, 8>,
                   integral_constant<int, 12>>::value == 4);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, -3>,
                   integral_constant<int, 4>> == 1);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 3>,
                   integral_constant<int, -4>> == 1);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 3>,
                   integral_constant<int, 4>> == 1);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, -3>,
                   integral_constant<int, -4>> == 1);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 9>,
                   integral_constant<int, 18>> == 9);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 9 * 17>,
                   integral_constant<int, 9 * 19>> == 9);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 9>,
                   integral_constant<int, 15>> == 3);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 0>,
                   integral_constant<int, 3>> == 3);
    static_assert(compile_time_gcd_v
                  <integral_constant<int, 0>,
                   integral_constant<int, 0>> == 0);
    static_assert(compile_time_gcd_v<int_constant<6>, int_constant<-9>> == 3);
    static_assert(compile_time_gcd_v<int_constant<-6>, int_constant<9>> == 3);
    static_assert(compile_time_gcd_v<int_constant<-6>, int_constant<-9>> == 3);
    static_assert(compile_time_gcd_v<size_constant<6>, size_constant<9>> == 3);
    static_assert(compile_time_gcd_v<size_constant<3>, size_constant<0>> == 3);
    static_assert(compile_time_gcd_v<size_constant<0>, size_constant<0>> == 0);
    static_assert(is_same_v<compile_time_gcd_t
                            <integral_constant<unsigned char, 250>,
                             integral_constant<unsigned char, 200>>,
                            integral_constant<unsigned char, 50>>);
    static_assert(is_same_v<compile_time_gcd_t
                            <integral_constant<unsigned char, 250>,
                             integral_constant<short, 200>>,
                            integral_constant<int, 50>>);
  }
  // compile_time_lcm
  {
    static_assert(is_trivial_empty_v
                  <compile_time_lcm<int_constant<569>, int_constant<137>>>);
        
    check_integral_constant::f<compile_time_lcm
                               <int_constant<569>, int_constant<137>>,
                               int, 569 * 137>();
    static_assert(is_same_v<compile_time_lcm
                            <integral_constant<int, 8>,
                             integral_constant<int, 12>>::type,
                            int_constant<24>>);
    static_assert(is_same_v<compile_time_lcm_t
                            <integral_constant<int, 8>,
                             integral_constant<int, 12>>,
                            int_constant<24>>);
    static_assert(is_base_of_v<int_constant<24>,
                               compile_time_lcm
                               <integral_constant<int, 8>,
                                integral_constant<int, 12>>>);
    static_assert(compile_time_lcm_v
                  <integral_constant<int, 9>,
                   integral_constant<int, 15>> == 45);
    static_assert(compile_time_lcm_v
                  <integral_constant<int, 0>,
                   integral_constant<int, 15>> == 0);
    static_assert(compile_time_lcm_v
                  <integral_constant<int, 0>,
                   integral_constant<int, 0>> == 0);
    static_assert(is_same_v<compile_time_lcm_t
                            <integral_constant<unsigned char, 40>,
                             integral_constant<unsigned char, 50>>,
                            integral_constant<unsigned char, 200>>);
    static_assert(is_same_v<compile_time_lcm_t
                            <integral_constant<unsigned char, 40>,
                             integral_constant<short, 50>>,
                            integral_constant<int, 200>>);
    static_assert(compile_time_lcm_v
                  <int_constant<6>, int_constant<-9>> == 18);
    static_assert(compile_time_lcm_v
                  <int_constant<-6>, int_constant<9>> == 18);
    static_assert(compile_time_lcm_v
                  <int_constant<-6>, int_constant<-9>> == 18);
    static_assert(compile_time_lcm_v
                  <int_constant<6>, int_constant<9>> == 18);
    static_assert(compile_time_lcm_v<size_constant<6>, size_constant<0>> == 0);
    static_assert(compile_time_lcm_v<size_constant<0>, size_constant<0>> == 0);
  }
  // compile_time_max
  // compile_time_min
  {
    static_assert(is_trivial_empty_v
                  <compile_time_max<integral_constant<char, 55>,
                                    integral_constant<char, 44>>>);
    static_assert(is_trivial_empty_v<compile_time_min
                                     <integral_constant<char, 55>,
                                      integral_constant<char, 44>>>);
    check_integral_constant::f<compile_time_max
                               <integral_constant<char, 55>,
                                integral_constant<char, 44>>,
                               char, 55>();
    check_integral_constant::f<compile_time_max
                               <integral_constant<unsigned, 257>,
                                integral_constant<int, -3>>,
                               unsigned, (unsigned)-3>();
    check_integral_constant::f<compile_time_max
                               <integral_constant<int, 257>,
                                integral_constant<int, -3>>,
                               int, 257>();
    check_integral_constant::f<compile_time_max_t
                               <integral_constant<int, 257>,
                                integral_constant<int, -3>>,
                               int, 257>();
    check_integral_constant::f<compile_time_min
                               <integral_constant<char, 55>,
                                integral_constant<char, 44>>,
                               char, 44>();
    check_integral_constant::f<compile_time_min
                               <integral_constant<unsigned, 257>,
                                integral_constant<int, -3>>,
                               unsigned, 257>();
    check_integral_constant::f<compile_time_min
                               <integral_constant<int, 257>,
                                integral_constant<int, -3>>,
                               int, -3>();
    check_integral_constant::f<compile_time_min_t
                               <integral_constant<int, 257>,
                                integral_constant<int, -3>>,
                               int, -3>();
    static_assert(compile_time_max_v<size_constant<2>, size_constant<1>> == 2);
    static_assert(compile_time_min_v<size_constant<2>, size_constant<1>> == 1);
  }
  // compile_time_acc
  {
    static_assert(is_trivial_empty_v<compile_time_acc
                                     <type_pack, int, float, void *>>);
    static_assert(is_same_v
                  <compile_time_acc_t<type_pack, int, float, void *>,
                   type_pack<type_pack<int, float>::type, void *>::type>);
    static_assert(compile_time_acc_v<compile_time_mul,
                                     size_constant<2>,
                                     size_constant<3>,
                                     size_constant<4>> == 24);

    check_integral_constant::f
      <compile_time_acc_t<compile_time_mul, int_constant<2>>::type,
       int, 2>();
    check_integral_constant::f
      <compile_time_acc_t<compile_time_mul,
                          int_constant<2>, int_constant<3>>::type,
       int, 6>();
    check_integral_constant::f
      <compile_time_acc_t<compile_time_mul, int_constant<2>,
                          int_constant<3>, integral_constant<long, 4>>::type,
       long, 24>();

    {
      using test_compile_time_acc::f;
      static_assert(has_member_type_type_v<f<int, int>>);
      static_assert(!has_member_type_type_v<f<int, float>>);
      static_assert(is_same_v<compile_time_acc<f, int>::type, int>);
      static_assert(is_integral_v<compile_time_acc_t<f, int, int>>);
      static_assert(is_integral_v
                    <compile_time_acc_t<f, int, int, long, long, short>>);
      static_assert(!has_member_type_type_v
                    <compile_time_acc<f, int, nullptr_t, long, long, short>>);
    }
  }

  // type_tag
  {
    using t = type_tag<int>;
    static_assert(is_same_v<t::type, int>);
    static_assert(is_trivial_empty_v<t>);
  }

  // type_pack
  // type_pack_size
  // type_pack_element
  {
    {
      using P = void;
      static_assert(is_trivial_empty_v<type_pack_size<P>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 1);
      static_assert(is_same_v<type_pack_element_t<0, P>, void>);
      static_assert(is_same_v<type_pack_element_t<1, P>, void>);
      static_assert(is_same_v<type_pack_element_t<100, P>, void>);
    }
    {
      using P = int;
      static_assert(is_trivial_empty_v<type_pack_size<P>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 1);
      static_assert(is_same_v<type_pack_element_t<0, P>, int>);
      static_assert(is_same_v<type_pack_element_t<1, P>, void>);
      static_assert(is_same_v<type_pack_element_t<100, P>, void>);
    }
    {
      using P = type_pack<>;
      static_assert(is_trivial_empty_v<type_pack_size<P>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 0);
      static_assert(is_same_v<type_pack_element_t<0, P>, void>);
      static_assert(is_same_v<type_pack_element_t<2222, P>, void>);
    }
    {
      using P = type_pack<int, float>;
      static_assert(is_trivial_empty_v<type_pack_size<P>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 2);
      static_assert(is_same_v<type_pack_element_t<0, P>, int>);
      static_assert(is_same_v<type_pack_element_t<1, P>, float>);
      static_assert(is_same_v<type_pack_element_t<2, P>, void>);
      static_assert(is_same_v<type_pack_element_t<2222, P>, void>);
    }
    {
      using P0 = index_sequence<>;
      static_assert(is_trivial_empty_v<type_pack_size<P0>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P0>>);
      static_assert(type_pack_size_v<P0> == 0);
      static_assert(is_same_v<type_pack_element_t<0, P0>, void>);
      static_assert(is_same_v<type_pack_element_t<1, P0>, void>);
      static_assert(is_same_v<type_pack_element_t<111, P0>, void>);

      using P = index_sequence<1, 3, 5>;
      static_assert(is_trivial_empty_v<type_pack_size<P>>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 3);
      static_assert(is_same_v<type_pack_element_t<0, P>, size_constant<1>>);
      static_assert(is_same_v<type_pack_element_t<1, P>, size_constant<3>>);
      static_assert(is_same_v<type_pack_element_t<2, P>, size_constant<5>>);
      static_assert(is_same_v<type_pack_element_t<3, P>, void>);
      static_assert(is_same_v<type_pack_element_t<2222, P>, void>);
    }
  }
  // type_pack_for_each
  {
    {
      using P = type_pack<type_pack<int>>;
      type_pack_for_each<P>([](auto x) {
        static_assert(is_same_v<typename decltype(x)::type, int>);
      });
      static_assert(!noexcept(type_pack_for_each<P>([](auto) {})));
      static_assert(!noexcept(type_pack_for_each<P>([](auto) noexcept {})));
      static_assert(is_trivial_empty_v<decltype(type_pack_for_each<P>)>);
    }
    {
      using PP = type_pack<int, float, double>;
      int i = 0;
      type_pack_for_each<PP>([&](auto x) {
        ++i;
        if (i == 1)
          assert(typeid(typename decltype(x)::type) == typeid(int));
        else if (i == 2)
          assert(typeid(typename decltype(x)::type) == typeid(float));
        else if (i == 3)
          assert(typeid(typename decltype(x)::type) == typeid(double));
        else
          assert(false);
      });
      static_assert(!noexcept(type_pack_for_each<PP>([](auto, auto) {})));
      static_assert(!noexcept
                    (type_pack_for_each<PP>([](auto, auto) noexcept {})));
    }
    {
      size_t i = 2;
      using P = type_pack<type_pack<int, float>,
                          type_pack<int, float, void *>,
                          type_pack<int, float, void *, void **>>;
      type_pack_for_each<P>([&]<typename...S>(S...s) {
        assert(sizeof...(s) == i);
        if constexpr (sizeof...(s) == 2) {
          static_assert(is_same_v<type_pack<S...>,
                                  type_pack<type_tag<int>,
                                            type_tag<float>>>);
          goto quit_label;
        }
        if constexpr (sizeof...(s) == 3) {
          static_assert(is_same_v<type_pack<S...>,
                                  type_pack<type_tag<int>,
                                            type_tag<float>,
                                            type_tag<void *>>>);
          goto quit_label;
        }
        if constexpr (sizeof...(s) == 4) {
          static_assert(is_same_v<type_pack<S...>,
                                  type_pack<type_tag<int>,
                                            type_tag<float>,
                                            type_tag<void *>,
                                            type_tag<void **>>>);
          goto quit_label;
        }
        assert(false);
      quit_label:
        ++i;
      });
    }
  }
  // type_pack_for_each_until_false
  {
    int c = 0;
    auto f = [&](auto x) {
      ++c;
      return !is_pointer_v<typename decltype(x)::type>;
    };
    c = 0;
    type_pack_for_each_until_false<type_pack<>>(f);
    assert(c == 0);
    c = 0;
    type_pack_for_each_until_false<type_pack<int>>(f);
    assert(c == 1);
    c = 0;
    type_pack_for_each_until_false<type_pack<int, int *>>(f);
    assert(c == 2);
    c = 0;
    type_pack_for_each_until_false<type_pack<int, int, int *, int>>(f);
    assert(c == 3);
    c = 0;
    type_pack_for_each_until_false<type_pack<int, int, int, int>>(f);
    assert(c == 4);
  }
  // type_pack_apply
  {
    using T = type_pack_apply<is_same, type_pack_add<int, float>>::type;
    using T2 = type_pack_apply_t<is_same, type_pack<int, float>>;
    using T3 = type_pack_apply_t<type_pack, type_pack<>>;
    static_assert(is_trivial_empty_v<T>);
    static_assert(is_same_v<T, is_same<int, float>>);
    static_assert(is_same_v<T, T2>);
    static_assert(is_same_v<T3, type_pack<>>);
  }
  // type_pack_add
  {
    {
      using P = compile_time_acc_t<type_pack_add, void *, void **, void ***>;
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<P> == 3);
      static_assert(is_same_v<type_pack_element_t<0, P>, void *>);
      static_assert(is_same_v<type_pack_element_t<1, P>, void **>);
      static_assert(is_same_v<type_pack_element_t<2, P>, void ***>);
      static_assert(is_same_v<type_pack_element_t<3, P>, void>);
    }
    {
      using P = type_pack_add<type_pack<int, int *>, int **>;
      using PP = type_pack<P>;
      static_assert(is_trivial_empty_v<P>);
      static_assert(is_trivial_empty_v<type_pack_element<0, P>>);
      static_assert(type_pack_size_v<PP> == 1);
      using T = type_pack_element_t<0, PP>;
      static_assert(type_pack_size_v<T> == 3);
      static_assert(is_same_v<type_pack_element_t<0, T>, int>);
      static_assert(is_same_v<type_pack_element_t<1, T>, int *>);
      static_assert(is_same_v<type_pack_element_t<2, T>, int **>);
      static_assert(is_same_v<type_pack_element_t<3, T>, void>);
      type_pack_for_each<PP>([](auto x, auto y, auto z) {
        static_assert(is_same_v<typename decltype(x)::type, int>);
        static_assert(is_same_v<typename decltype(y)::type, int *>);
        static_assert(is_same_v<typename decltype(z)::type, int **>);
      });
    }
    {
      using P1 = type_pack<int, float, void *>;
      using P2 = type_pack<void *, int **>;
      using P3 = type_pack<int (*)(), void>;

      using PP = type_pack_add<P1, P2>;
      static_assert(is_trivial_empty_v<PP>);
      static_assert(is_trivial_empty_v<type_pack_element<0, PP>>);
      static_assert(is_trivial_empty_v<type_pack_element<3, PP>>);
      static_assert(type_pack_eql
                    <PP, type_pack<int, float, void *, void *, int **>>::value);
      using PP2 = type_pack_add<P3, PP>;
      static_assert(type_pack_eql_v
                    <PP2, type_pack
                     <int (*)(), void, int, float, void *, void *, int **>>);
    }
  }
  // type_pack_mul
  {
    using P1 = type_pack<char [1], char [2], char [3]>;
    using P2 = type_pack<char *[1], char *[2], char *[3], char *[4]>;
    using P3 = type_pack<char **[1], char **[2], char **[3],
                         char **[4], char **[5]>;
    auto f = [](auto tag) {
      using P = typename decltype(tag)::type;
      static_assert(type_pack_size_v<P> == 3 * 4 * 5);

      using E1 = type_pack_decay_t
        <type_pack_element_t<2 * 4 * 5 + 3 * 5 + 4, P>>;
      static_assert(is_same_v
                    <E1, type_pack<char [3], char *[4], char **[5]>>);

      using E2 = type_pack_decay_t
        <type_pack_element_t<1 * 4 * 5 + 2 * 5 + 3, P>>;
      static_assert(is_same_v
                    <E2, type_pack<char [2], char *[3], char **[4]>>);
    };
    f(type_tag<compile_time_acc_t<type_pack_mul, P1, P2, P3>>{});
    f(type_tag<type_pack_mul<P1, type_pack_mul<P2, P3>>>{});
    static_assert(is_trivial_empty_v<type_pack_mul<P1, type_pack_mul<P2, P3>>>);
  }
  // type_pack_decay
  // type_pack_eql
  {
    {
      static_assert(type_pack_eql_v<int, int>);
      static_assert(!type_pack_eql_v<int, int &>);
      static_assert(is_trivial_empty_v<type_pack_eql<int, int>>);

      static_assert(type_pack_eql_v<int, type_pack<int>>);
      static_assert(type_pack_eql_v<type_pack<int>, type_pack<int>>);

      using P = compile_time_acc_t<type_pack_add, int, float, void *, void **>;
      using PP = type_pack_decay_t<P>;
      static_assert(is_trivial_empty_v<PP>);
      static_assert(!is_same_v<PP, P>);
      static_assert(is_same_v<PP, type_pack<int, float, void *, void **>>);
      static_assert(type_pack_eql_v<P, PP>);
    }
    {
      using P1 = type_pack<int *, int **, int ***>;
      using P2 = type_pack<void *>;
      using P12 = type_pack<type_pack_add<int *, void *>,
                            type_pack_add<int **, void *>,
                            type_pack_add<int ***, void *>>;
      using P21 = type_pack<type_pack_add<void *, int *>,
                            type_pack_add<void *, int **>,
                            type_pack_add<void *, int ***>>;
      using PP = type_pack_mul<P1, P2>;
      static_assert(!is_same_v<PP, P12>);
      static_assert(is_same_v<type_pack_decay_t<PP>, P12>);
      static_assert(type_pack_eql_v<PP, type_pack_decay_t<PP>>);
    }
  }
  // type_pack_recursive_decay
  {
    {
      static_assert(is_same_v<type_pack_recursive_decay_t<int>, int>);
      static_assert(is_trivial_empty_v<type_pack_recursive_decay<int>>);
      static_assert(is_same_v
                    <type_pack_recursive_decay_t<type_pack<>>, type_pack<>>);

      using P = type_pack_mul<type_pack<void *, void **>,
                              type_pack<void ***, void ****>>;
      using PP = type_pack_recursive_decay_t<P>;
      using PPP = type_pack<type_pack<void *, void ***>,
                            type_pack<void *, void ****>,
                            type_pack<void **, void ***>,
                            type_pack<void **, void ****>>;
      static_assert(is_same_v<PP, PPP>);
    }
    {
      using P = type_pack_mul<type_pack_add<void *, void **>,
                              type_pack_add<void ***,
                                            type_pack_add<void ****,
                                                          void *****>>>;
      using PP = type_pack_recursive_decay_t<P>;
      using PPP = type_pack<type_pack<void *, void ***>,
                            type_pack<void *, void ****>,
                            type_pack<void *, void *****>,
                            type_pack<void **, void ***>,
                            type_pack<void **, void ****>,
                            type_pack<void **, void *****>>;
      static_assert(is_same_v<PP, PPP>);
    }
  }
  // type_pack_recursive_eql
  {
    {
      static_assert(is_trivial_empty_v<type_pack_recursive_eql<int, int>>);
      static_assert(is_trivial_empty_v
                    <type_pack_recursive_eql<type_pack<int>, int>>);
      static_assert(is_trivial_empty_v
                    <type_pack_recursive_eql<int, type_pack<int>>>);
      static_assert(is_trivial_empty_v
                    <type_pack_recursive_eql<type_pack<int>, type_pack<int>>>);
    }
    {
      static_assert(type_pack_recursive_eql_v<type_pack<>, index_sequence<>>);

      static_assert(type_pack_recursive_eql_v<int, type_pack<int>>);
      static_assert(type_pack_recursive_eql_v<type_pack<int>, int>);
      static_assert(type_pack_recursive_eql_v<type_pack<type_pack<int>>, int>);
      static_assert(type_pack_recursive_eql_v<int, type_pack<type_pack<int>>>);

      static_assert(type_pack_recursive_eql_v
                    <type_pack<int, int>, type_pack_add<int, int>>);

      static_assert(type_pack_recursive_eql_v
                    <type_pack<int, float>, type_pack<int, float>>);
      static_assert(type_pack_recursive_eql_v
                    <type_pack<type_pack<int>, float>, type_pack<int, float>>);
      static_assert(type_pack_recursive_eql_v
                    <type_pack<int, type_pack<type_pack<float>>>,
                     type_pack<int, float>>);
    }
    {
      using P1 = type_pack<int *, int **, int ***>;
      using P2 = type_pack<void *>;
      using P12 = type_pack<type_pack<int *, void *>,
                            type_pack<int **, void *>,
                            type_pack<int ***, void *>>;
      using PP = type_pack_mul<P1, P2>;
      static_assert(type_pack_recursive_eql_v<PP, P12>);
      static_assert(type_pack_recursive_eql_v<P12, PP>);
    }
  }
  // type_pack_first_part
  // type_pack_second_part
  {
    {
      using P = int;
      static_assert(is_trivial_empty_v<type_pack_first_part<0, P>>);
      static_assert(is_trivial_empty_v<type_pack_second_part<0, P>>);
      static_assert(type_pack_size_v<type_pack_first_part<0, P>> == 0);
      static_assert(type_pack_size_v<type_pack_second_part<0, P>> == 1);
      static_assert(type_pack_size_v<type_pack_first_part<1, P>> == 1);
      static_assert(type_pack_size_v<type_pack_second_part<1, P>> == 0);
      static_assert(is_same_v
                    <type_pack_apply_t<type_pack, type_pack_first_part<1, P>>,
                     type_pack<int>>);
      static_assert(is_same_v
                    <type_pack_apply_t<type_pack, type_pack_second_part<1, P>>,
                     type_pack<>>);
    }
    {
      using P = type_pack<int, float, void *, void **, void ***>;
      using P0 = type_pack_first_part<3, P>;
      using P1 = type_pack_second_part<3, P>;
      static_assert(is_trivial_empty_v<P0>);
      static_assert(is_trivial_empty_v<P1>);
      static_assert(type_pack_eql_v<P0, type_pack<int, float, void *>>);
      static_assert(type_pack_eql_v<P1, type_pack<void **, void ***>>);
    }
  }
  // n_type_pack
  {
    static_assert(same_as<n_type_pack_t<0, int>, type_pack<>>);
    static_assert(same_as<n_type_pack_t<1, int>, type_pack<int>>);
    static_assert(same_as<n_type_pack_t<2, int>, type_pack<int, int>>);
    static_assert(same_as<n_type_pack_t<3, int>, type_pack<int, int, int>>);
    static_assert(same_as
                  <n_type_pack_t<6, int>,
                   type_pack<int, int, int, int, int, int>>);
  }

  // conjunction
  // disjunction
  // negation
  {
    using _1 = true_type;
    using _0 = false_type;

    static_assert(is_trivial_empty_v<conjunction<>>);
    static_assert(is_trivial_empty_v<conjunction<_1>>);
    static_assert(is_trivial_empty_v<conjunction<_1, _0, _1>>);
    static_assert(is_trivial_empty_v<disjunction<>>);
    static_assert(is_trivial_empty_v<disjunction<_1>>);
    static_assert(is_trivial_empty_v<disjunction<_1, _0, _1>>);
    static_assert(is_trivial_empty_v<negation<_1>>);

    static_assert(conjunction<>::value);
    static_assert(conjunction_v<>);
    static_assert(conjunction_v<_1>);
    static_assert(!conjunction_v<_0>);
    static_assert(!conjunction_v<_0, _1, _1>);
    static_assert(!conjunction_v<_0, _1, _0, _1>);
    static_assert(conjunction_v<_1, _1>);
    static_assert(!conjunction_v<_0, _1>);
    static_assert(!conjunction_v<_1, _0>);
    static_assert(!conjunction_v<_0, _0>);
    static_assert(conjunction_v<_1, _1, _1, _1, _1>);
    static_assert(!conjunction_v<_1, _1, _0, _1, _1>);
    static_assert(!conjunction_v<_0, _0, _0>);

    static_assert(!disjunction<>::value);
    static_assert(!disjunction_v<>);
    static_assert(disjunction_v<_1>);
    static_assert(!disjunction_v<_0>);
    static_assert(disjunction_v<_1, _1>);
    static_assert(disjunction_v<_1, _0>);
    static_assert(disjunction_v<_0, _1>);
    static_assert(!disjunction_v<_0, _0>);
    static_assert(!disjunction_v<_0, _0, _0, _0, _0, _0>);
    static_assert(disjunction_v<_0, _1, _0, _0, _0, _0>);
    static_assert(disjunction_v<_0, _0, _0, _0, _1, _0>);
    static_assert(disjunction_v<_1, _0, _0, _0, _0, _1>);

    static_assert(negation<_0>::value);
    static_assert(negation_v<_0>);
    static_assert(!negation_v<_1>);
  }

  // select_type
  {
    static_assert(is_trivial_empty_v<select_type<int>>);
    static_assert(is_trivial_empty_v<select_type<int, const int &, int &&>>);
    static_assert(select_type<int>::value == 0);
    static_assert(select_type_v<int> == 0);
    static_assert(select_type_v<int, const int &, int &&> == 1);
    static_assert(select_type_v<int &, const int &, int &&> == 0);
    static_assert(select_type_v<int &&, const int &, int &&> == 1);
    static_assert(select_type_v<int, int *> == 1);
    static_assert(select_type_v<int, int *, int **> == 2);
    static_assert(select_type_v<int, const int &, int *, int **, int ***> == 0);
    static_assert(select_type_v<nullptr_t, float, int *, int> == 1);
  }

  // accumulate_args
  {
    static_assert(is_trivial_empty_v<decltype(accumulate_args)>);
    static_assert(accumulate_args(plus<>{}, 1) == 1);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3) == 6);
    static_assert(accumulate_args(plus<>{}, 1, 2, 3, 4, 5) == 15);
    static_assert(!noexcept(accumulate_args(plus<>{}, 1)));
    static_assert(!noexcept(accumulate_args(plus<>{}, 1, 2, 3)));
    static_assert(!noexcept(accumulate_args
                            ([](int, int) noexcept {return 0;}, 2, 3)));
  }

  // nth_arg
  {
    static_assert(is_trivial_empty_v<decltype(nth_arg<0>)>);

    static_assert(nth_arg<0>((short)1, 2.0, 3) == 1);
    static_assert(nth_arg<1>((short)1, 2.0, 3) == 2);
    static_assert(nth_arg<2>((short)1, 2.0, 3) == 3);
    static_assert(same_as<decltype(nth_arg<0>((short)1, 2.0, 3)), short &&>);
    static_assert(same_as<decltype(nth_arg<1>((short)1, 2.0, 3)), double &&>);
    static_assert(same_as<decltype(nth_arg<2>((short)1, 2.0, 3)), int &&>);

    const volatile int x = 1, y = 2, z = 3;
    assert(nth_arg<0>(x) == 1);
    assert(nth_arg<0>(x, 2, move(z)) == 1);
    assert(nth_arg<1>(x, 2, move(z)) == 2);
    assert(nth_arg<2>(x, 2, move(z)) == 3);
    static_assert(same_as<decltype(nth_arg<0>(x)), const volatile int &>);
    static_assert(same_as<decltype(nth_arg<0>(x, 2, move(z))),
                          const volatile int &>);
    static_assert(same_as<decltype(nth_arg<1>(x, 2, move(z))),
                          int &&>);
    static_assert(same_as<decltype(nth_arg<2>(x, 2, move(z))),
                          const volatile int &&>);
  }
}

namespace help_test_swap {

struct A {};
void swap(A &, A &);
struct B {
  B(const B &);
};
void swap(B &, B &) noexcept;
struct C {
  C(C &&) noexcept;
  C &operator =(C &&) noexcept;
};
struct D {
  D(const D &);
  D &operator =(const D &);
};
struct E {};
void swap(E &, E &) = delete;

struct swap_nothing0 {
  int value;
};
inline void swap(swap_nothing0 &x, swap_nothing0 &y) {}

struct swap_nothing {
  int value;
};
inline void swap(const swap_nothing &x, const swap_nothing &y) {}

struct swap_nothing2 {
  int value;
};
inline void swap(swap_nothing2 x, swap_nothing2 y) noexcept {}

struct swap_nothing0_constexpr {
  int value;
};
inline constexpr void swap(swap_nothing0_constexpr &x,
                           swap_nothing0_constexpr &y) {}

struct swap_nothing_constexpr {
  int value;
};
inline constexpr void swap(const swap_nothing_constexpr &x,
                           const swap_nothing_constexpr &y) {}

struct swap_nothing2_constexpr {
  int value;
};
inline constexpr void swap(swap_nothing2_constexpr x,
                           swap_nothing2_constexpr y) noexcept {}

struct t {
  int value;

  t() = default;
  ~t() = default;
  t(const t &) = delete;
  t &operator =(const t &) = delete;
  t(t &&) = delete;
  t &operator =(t &&) = delete;

  constexpr t(int x) : value(x) {}
};
constexpr void swap(t &x, t &y) {
  int tmp = x.value;
  x.value = y.value;
  y.value = tmp;
}

}
void test_swap() {
  using namespace help_test_swap;

  // swap-related traits
  {
    static_assert(is_trivial_empty_v<is_swappable<A>>);
    static_assert(is_swappable<A>::value);
    static_assert(is_swappable_v<A>);
    static_assert(!is_nothrow_swappable_v<A>);
    static_assert(is_trivial_empty_v<is_nothrow_swappable<A>>);
    static_assert(is_swappable<B>::value);
    static_assert(is_nothrow_swappable<B>::value);
    static_assert(is_swappable_v<C>);
    static_assert(is_nothrow_swappable_v<C>);
    static_assert(is_swappable_v<D>);
    static_assert(!is_nothrow_swappable_v<D>);
    static_assert(is_swappable_v<E>); // differ from std
    static_assert(is_nothrow_swappable_v<E>);

    static_assert(is_trivial_empty_v<is_swappable_with<int &, int &>>);
    static_assert(!is_swappable_with_v<int, int>);
    static_assert(!is_swappable_with_v<int, int &>);
    static_assert(is_swappable_with_v<int &, int &>);

    static_assert(is_trivial_empty_v<is_nothrow_swappable_with<int &, int &>>);
    static_assert(!is_nothrow_swappable_with_v<int, int>);
    static_assert(!is_nothrow_swappable_with_v<int, int &>);
    static_assert(is_nothrow_swappable_with_v<int &, int &>);

    static_assert(!is_swappable_with_v<int (&)[2], int (&)[3]>);
    static_assert(!is_nothrow_swappable_with_v<int (&)[2], int (&)[3]>);

    static_assert(!is_swappable_with_v<int (&)[], int (&)[]>);
    static_assert(!is_nothrow_swappable_with_v<int (&)[], int (&)[]>);

    static_assert(is_swappable_with_v<int (&)[2], int (&)[2]>);
    static_assert(is_nothrow_swappable_with_v<int (&)[2], int (&)[2]>);

    static_assert(is_swappable_with_v
                  <int (&)[2][2][2][2], int (&)[2][2][2][2]>);
    static_assert(is_nothrow_swappable_with_v
                  <int (&)[2][2][2][2], int (&)[2][2][2][2]>);

    {
      struct t00 {
        t00(t00 &&) {}
        t00 &operator =(t00 &&) {return *this;}
      };
      struct t10 {
        t10(t10 &&) noexcept {}
        t10 &operator =(t10 &&) {return *this;}
      };
      struct t01 {
        t01(t01 &&) {}
        t01 &operator =(t01 &&) noexcept {return *this;}
      };
      struct t11 {
        t11(t11 &&) noexcept {}
        t11 &operator =(t11 &&) noexcept {return *this;}
      };
      static_assert(!noexcept
                    (default_swap(declval<t00 &>(), declval<t00 &>())));
      static_assert(!noexcept
                    (default_swap(declval<t01 &>(), declval<t01 &>())));
      static_assert(!noexcept
                    (default_swap(declval<t10 &>(), declval<t10 &>())));
      static_assert(noexcept(default_swap(declval<t11 &>(), declval<t11 &>())));
      static_assert(!noexcept
                    (adl_swap(declval<t00 &>(), declval<t00 &>())));
      static_assert(!noexcept
                    (adl_swap(declval<t01 &>(), declval<t01 &>())));
      static_assert(!noexcept
                    (adl_swap(declval<t10 &>(), declval<t10 &>())));
      static_assert(noexcept(adl_swap(declval<t11 &>(), declval<t11 &>())));
      static_assert(!is_nothrow_swappable_v<t00>);
      static_assert(!is_nothrow_swappable_v<t01>);
      static_assert(!is_nothrow_swappable_v<t10>);
      static_assert(is_nothrow_swappable_v<t11>);
      static_assert(!is_nothrow_swappable_with_v<t00 &, t00 &>);
      static_assert(!is_nothrow_swappable_with_v<t01 &, t01 &>);
      static_assert(!is_nothrow_swappable_with_v<t10 &, t10 &>);
      static_assert(is_nothrow_swappable_with_v<t11 &, t11 &>);
    }
  }
  // default_swap
  // default_swappable
  {
    struct t {
      int x;
    };
    static_assert(is_swappable_v<t>);
    static_assert(is_nothrow_swappable_v<t>);
    t a = {1}, b = {2};
    adl_swap(a, a);
    adl_swap(b, b);
    default_swap(a, a);
    default_swap(b, b);
    default_swap(a, b);
    adl_swap(a, b);
    default_swap(a, b);
    assert(a.x == 2 && b.x == 1);
    static_assert(noexcept(adl_swap(a, b)));
    static_assert(noexcept(default_swap(a, b)));
    static_assert(noexcept(default_swap(declval<int &>(), declval<int &>())));

    t aa[2] = {{1}, {2}};
    t bb[2] = {{3}, {4}};
    adl_swap(aa, bb);
    assert(aa[0].x == 3 && aa[1].x == 4 && bb[0].x == 1 && bb[1].x == 2);
    static_assert(noexcept(adl_swap(aa, bb)));

    static_assert(default_swappable<int>);
    static_assert(default_swappable<int &>);
    static_assert(!default_swappable<int [2]>);
    static_assert(!default_swappable<int (&)[2]>);
    static_assert(!default_swappable<const int (&)[2]>);
    static_assert(!default_swappable<int (&&)[2]>);
    static_assert(!default_swappable<const int (&&)[2]>);
  }
  // swap by user defined swap
  {
    // swap_nothing0
    {
      swap_nothing0 x{1}, y{2};
      default_swap(x, y);
      assert(x.value == 2 && y.value == 1);
      adl_swap(x, y);
      assert(x.value == 2 && y.value == 1);

      swap_nothing0 a[] = {{1}, {2}, {3}};
      swap_nothing0 b[] = {{4}, {5}, {6}};
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);
      adl_swap(a, b);
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);

      static_assert(is_swappable_v<swap_nothing0>);
      static_assert(!is_nothrow_swappable_v<swap_nothing0>);
      static_assert(!is_nothrow_swappable_v<swap_nothing0 []>);
      static_assert(!is_nothrow_swappable_v<swap_nothing0 [3]>);
      static_assert(!is_nothrow_swappable_v<swap_nothing0 [3][3][3]>);
    }
    // swap_nothing
    {
      swap_nothing x{1}, y{2};
      default_swap(x, y);
      assert(x.value == 2 && y.value == 1);
      adl_swap(x, y);
      assert(x.value == 2 && y.value == 1);

      swap_nothing a[] = {{1}, {2}, {3}};
      swap_nothing b[] = {{4}, {5}, {6}};
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);
      adl_swap(a, b);
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);

      static_assert(is_swappable_v<swap_nothing>);
      static_assert(!noexcept(adl_swap(declval<swap_nothing &>(),
                                       declval<swap_nothing &>())));
      static_assert(!is_nothrow_swappable_v<swap_nothing>);
      static_assert(!is_nothrow_swappable_v<swap_nothing []>);
      static_assert(!is_nothrow_swappable_v<swap_nothing [3]>);
      static_assert(!is_nothrow_swappable_v<swap_nothing [3][3][3]>);
    }
    // swap_nothing2
    {
      swap_nothing2 x{1}, y{2};
      default_swap(x, y);
      assert(x.value == 2 && y.value == 1);
      adl_swap(x, y);
      assert(x.value == 2 && y.value == 1);

      swap_nothing2 a[] = {{1}, {2}, {3}};
      swap_nothing2 b[] = {{4}, {5}, {6}};
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);
      adl_swap(a, b);
      assert(a[0].value == 1 && a[1].value == 2 && a[2].value == 3);
      assert(b[0].value == 4 && b[1].value == 5 && b[2].value == 6);

      static_assert(is_swappable_v<swap_nothing2>);
      static_assert(is_nothrow_swappable_v<swap_nothing2>);
      static_assert(!is_nothrow_swappable_v<swap_nothing2 []>);
      static_assert(is_nothrow_swappable_v<swap_nothing2 [3]>);
      static_assert(is_nothrow_swappable_v<swap_nothing2 [3][3][3]>);
    }
  }
  // constexpr
  {
    constexpr auto f = []() constexpr->int {
      int x = 1, y = 2;
      adl_swap(x, y);
      return x;
    };
    static_assert(f() == 2);

    constexpr auto ff = []() constexpr->int {
      int x = 3, y = 4;
      default_swap(x, y);
      return x;
    };
    static_assert(ff() == 4);

    constexpr auto fff0 = []() constexpr
      ->help_test_swap::swap_nothing0_constexpr {
      help_test_swap::swap_nothing0_constexpr x(1);
      help_test_swap::swap_nothing0_constexpr y(2);
      adl_swap(x, y);
      return x;
    };
    static_assert(fff0().value == 1);

    constexpr auto fff = []() constexpr
      ->help_test_swap::swap_nothing_constexpr {
      help_test_swap::swap_nothing_constexpr x(1);
      help_test_swap::swap_nothing_constexpr y(2);
      adl_swap(x, y);
      return x;
    };
    static_assert(fff().value == 1);

    constexpr auto fff2 = []() constexpr
      ->help_test_swap::swap_nothing2_constexpr {
      help_test_swap::swap_nothing2_constexpr x(1);
      help_test_swap::swap_nothing2_constexpr y(2);
      adl_swap(x, y);
      return x;
    };
    static_assert(fff2().value == 1);

    using namespace help_test_swap;
    static_assert(!is_move_constructible_v<t>);
    static_assert(!is_move_assignable_v<t>);
    static_assert(is_swappable_v<t>);
    static_assert(!is_nothrow_swappable_v<t>);
    constexpr auto ffff = []() constexpr->int {
      t a(1), b(2);
      adl_swap(a, b);
      return a.value;
    };
    static_assert(ffff() == 2);
  }
}

void test_type_traits() {
  // is_void
  {
    static_assert(is_trivial_empty_v<is_void<void>>);
    static_assert(is_trivial_empty_v<is_void<const void>>);
    static_assert(is_trivial_empty_v<is_void<volatile void>>);
    static_assert(is_trivial_empty_v<is_void<const volatile void>>);
    static_assert(is_trivial_empty_v<is_void<int>>);
    static_assert(is_void<void>::value);
    static_assert(is_void_v<void>);
    static_assert(is_void_v<const void>);
    static_assert(is_void_v<volatile void>);
    static_assert(is_void_v<const volatile void>);
    static_assert(!is_void_v<void *>);
    static_assert(!is_void_v<void *const>);
    static_assert(!is_void_v<void *volatile>);
    static_assert(!is_void_v<void *const volatile>);
    static_assert(!is_void_v<void *&>);
    static_assert(!is_void_v<void *const &>);
    static_assert(!is_void_v<void *volatile &>);
    static_assert(!is_void_v<void *const volatile &>);
    static_assert(!is_void_v<void *&&>);
    static_assert(!is_void_v<void *const &&>);
    static_assert(!is_void_v<void *volatile &&>);
    static_assert(!is_void_v<void *const volatile &&>);
    static_assert(!is_void_v<int>);
  }
  // is_null_pointer
  {
    static_assert(is_trivial_empty_v<is_null_pointer<nullptr_t>>);
    static_assert(is_trivial_empty_v<is_null_pointer<const nullptr_t>>);
    static_assert(is_trivial_empty_v<is_null_pointer<volatile nullptr_t>>);
    static_assert(is_trivial_empty_v
                  <is_null_pointer<const volatile nullptr_t>>);
    static_assert(is_trivial_empty_v<is_null_pointer<int>>);
    static_assert(is_null_pointer<nullptr_t>::value);
    static_assert(is_null_pointer_v<nullptr_t>);
    static_assert(is_null_pointer_v<const nullptr_t>);
    static_assert(is_null_pointer_v<volatile nullptr_t>);
    static_assert(is_null_pointer_v<const volatile nullptr_t>);
    static_assert(!is_null_pointer_v<nullptr_t &>);
    static_assert(!is_null_pointer_v<const nullptr_t &>);
    static_assert(!is_null_pointer_v<volatile nullptr_t &>);
    static_assert(!is_null_pointer_v<const volatile nullptr_t &>);
    static_assert(!is_null_pointer_v<nullptr_t &&>);
    static_assert(!is_null_pointer_v<const nullptr_t &&>);
    static_assert(!is_null_pointer_v<volatile nullptr_t &&>);
    static_assert(!is_null_pointer_v<const volatile nullptr_t &&>);
    static_assert(!is_null_pointer_v<int>);
  }
  // is_integral
  {
    static_assert(is_trivial_empty_v<is_integral<bool>>);
    static_assert(is_trivial_empty_v<is_integral<const bool>>);
    static_assert(is_trivial_empty_v<is_integral<volatile bool>>);
    static_assert(is_trivial_empty_v<is_integral<const volatile bool>>);
    static_assert(is_trivial_empty_v<is_integral<int>>);
    static_assert(is_integral<bool>::value);
    static_assert(is_integral_v<bool>);
    static_assert(is_integral_v<const bool>);
    static_assert(is_integral_v<volatile bool>);
    static_assert(is_integral_v<const volatile bool>);
    static_assert(!is_integral_v<bool &>);
    static_assert(!is_integral_v<const bool &>);
    static_assert(!is_integral_v<volatile bool &>);
    static_assert(!is_integral_v<const volatile bool &>);
    static_assert(!is_integral_v<bool &&>);
    static_assert(!is_integral_v<const bool &&>);
    static_assert(!is_integral_v<volatile bool &&>);
    static_assert(!is_integral_v<const volatile bool &&>);
    static_assert(!is_integral_v<bool *>);

    static_assert(is_integral_v<char>);
    static_assert(is_integral_v<unsigned char>);
    static_assert(is_integral_v<signed char>);
    static_assert(is_integral_v<char8_t>);
    static_assert(is_integral_v<char16_t>);
    static_assert(is_integral_v<char32_t>);
    static_assert(is_integral_v<wchar_t>);
    static_assert(is_integral_v<short int>);
    static_assert(is_integral_v<unsigned short int>);
    static_assert(is_integral_v<int>);
    static_assert(is_integral_v<unsigned int>);
    static_assert(is_integral_v<long int>);
    static_assert(is_integral_v<unsigned long int>);
    static_assert(is_integral_v<long long int>);
    static_assert(is_integral_v<unsigned long long int>);
    static_assert(is_integral_v<size_t>);
    static_assert(is_integral_v<ptrdiff_t>);
    static_assert(is_integral_v<int_least8_t>);
    static_assert(is_integral_v<uint_least8_t>);
    static_assert(is_integral_v<int_least16_t>);
    static_assert(is_integral_v<uint_least16_t>);
    static_assert(is_integral_v<int_least32_t>);
    static_assert(is_integral_v<uint_least32_t>);
    static_assert(is_integral_v<int_least64_t>);
    static_assert(is_integral_v<uint_least64_t>);
    static_assert(is_integral_v<int_fast8_t>);
    static_assert(is_integral_v<uint_fast8_t>);
    static_assert(is_integral_v<int_fast16_t>);
    static_assert(is_integral_v<uint_fast16_t>);
    static_assert(is_integral_v<int_fast32_t>);
    static_assert(is_integral_v<uint_fast32_t>);
    static_assert(is_integral_v<int_fast64_t>);
    static_assert(is_integral_v<uint_fast64_t>);
    static_assert(is_integral_v<intmax_t>);
    static_assert(is_integral_v<uintmax_t>);

    static_assert(!is_integral_v<nullptr_t>);
    static_assert(!is_integral_v<void>);
    static_assert(!is_integral_v<int &>);
    static_assert(!is_integral_v<int &&>);
    static_assert(!is_integral_v<int *>);
    static_assert(!is_integral_v<int *&>);
    static_assert(!is_integral_v<int *&&>);
    static_assert(!is_integral_v<float>);
    static_assert(!is_integral_v<double>);
    static_assert(!is_integral_v<void *>);
    static_assert(!is_integral_v<void ()>);
    static_assert(!is_integral_v<void (&)()>);
    static_assert(!is_integral_v<void (*)()>);
  }
  // is_floating_point
  {
    static_assert(is_trivial_empty_v<is_floating_point<float>>);
    static_assert(is_trivial_empty_v<is_floating_point<const float>>);
    static_assert(is_trivial_empty_v<is_floating_point<volatile float>>);
    static_assert(is_trivial_empty_v<is_floating_point<const volatile float>>);
    static_assert(is_trivial_empty_v<is_floating_point<double>>);
    static_assert(is_trivial_empty_v<is_floating_point<long double>>);
    static_assert(is_trivial_empty_v<is_floating_point<int>>);
    static_assert(is_floating_point<float>::value);
    static_assert(is_floating_point_v<float>);
    static_assert(is_floating_point_v<double>);
    static_assert(is_floating_point_v<long double>);
    static_assert(is_floating_point_v<const long double>);
    static_assert(is_floating_point_v<volatile long double>);
    static_assert(is_floating_point_v<const volatile long double>);
    static_assert(!is_floating_point_v<float &>);
    static_assert(!is_floating_point_v<const float &>);
    static_assert(!is_floating_point_v<volatile float &>);
    static_assert(!is_floating_point_v<const volatile float &>);
    static_assert(!is_floating_point_v<float &&>);
    static_assert(!is_floating_point_v<const float &&>);
    static_assert(!is_floating_point_v<volatile float &&>);
    static_assert(!is_floating_point_v<const volatile float &&>);
    static_assert(!is_floating_point_v<int>);
  }
  // is_array
  {
    static_assert(is_trivial_empty_v<is_array<int []>>);
    static_assert(is_trivial_empty_v<is_array<int [1]>>);
    static_assert(is_trivial_empty_v<is_array<const int [1]>>);
    static_assert(is_trivial_empty_v<is_array<volatile int [1]>>);
    static_assert(is_trivial_empty_v<is_array<const volatile int [1]>>);
    static_assert(is_trivial_empty_v<is_array<int>>);
    static_assert(is_array<int []>::value);
    static_assert(is_array_v<int []>);
    static_assert(is_array_v<int [3]>);
    static_assert(is_array_v<const int *[100]>);
    static_assert(is_array_v<volatile int *[100]>);
    static_assert(is_array_v<const volatile int *[100]>);
    static_assert(!is_array_v<const volatile int *(&&)[100]>);
    static_assert(!is_array_v<int>);
  }
  // is_pointer
  {
    static_assert(is_trivial_empty_v<is_pointer<int *>>);
    static_assert(is_trivial_empty_v<is_pointer<int *const>>);
    static_assert(is_trivial_empty_v<is_pointer<int *volatile>>);
    static_assert(is_trivial_empty_v<is_pointer<int *const volatile>>);
    static_assert(is_trivial_empty_v<is_pointer<int>>);
    static_assert(is_pointer<int *>::value);
    static_assert(is_pointer_v<int *>);
    static_assert(is_pointer_v<volatile int *>);
    static_assert(is_pointer_v<int *const *>);
    static_assert(is_pointer_v<int ****const>);
    static_assert(is_pointer_v<int ****volatile>);
    static_assert(is_pointer_v<int (****volatile const)[100]>);
    static_assert(is_pointer_v<int (****volatile const)()>);
    static_assert(!is_pointer_v<int>);
    static_assert(!is_pointer_v<int *&>);
  }
  // is_lvalue_reference
  // is_rvalue_reference
  {
    static_assert(is_trivial_empty_v<is_lvalue_reference<int &>>);
    static_assert(is_trivial_empty_v<is_lvalue_reference<int>>);
    static_assert(is_lvalue_reference<int &>::value);
    static_assert(is_lvalue_reference_v<int &>);
    static_assert(is_lvalue_reference_v<const int &>);
    static_assert(is_lvalue_reference_v<const volatile int &>);
    static_assert(is_lvalue_reference_v<void (&)()>);
    static_assert(is_lvalue_reference_v<int (&)[]>);
    static_assert(is_lvalue_reference_v<const int (&)[3]>);
    static_assert(!is_lvalue_reference_v<int &&>);
    static_assert(!is_lvalue_reference_v<void (&&)()>);

    static_assert(is_trivial_empty_v<is_rvalue_reference<int &&>>);
    static_assert(is_trivial_empty_v<is_rvalue_reference<int>>);
    static_assert(is_rvalue_reference<int &&>::value);
    static_assert(is_rvalue_reference_v<int &&>);
    static_assert(is_rvalue_reference_v<const int &&>);
    static_assert(is_rvalue_reference_v<const volatile int &&>);
    static_assert(is_rvalue_reference_v<void (&&)()>);
    static_assert(!is_rvalue_reference_v<void (&)()>);
    static_assert(!is_rvalue_reference_v<int &>);
    static_assert(!is_rvalue_reference_v<const int>);
  }
  // is_member_object_pointer
  // is_member_function_pointer
  {
    struct t {
      int i;
    };
    int t::*const p = &t::i;
    static_assert(is_trivial_empty_v<is_member_object_pointer<decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_object_pointer<const decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_object_pointer<volatile decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_object_pointer<const volatile decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_object_pointer<int>>);
    static_assert(is_member_object_pointer<decltype(p)>::value);
    static_assert(is_member_object_pointer_v<decltype(p)>);
    static_assert(is_member_object_pointer<int t::*>::value);
    static_assert(is_member_object_pointer_v<int t::*>);
    static_assert(is_member_object_pointer_v<int t::*const>);
    static_assert(is_member_object_pointer_v<int t::*volatile>);
    static_assert(is_member_object_pointer_v<int t::*const volatile>);
    static_assert(!is_member_object_pointer_v<int (t::*)()>);
    static_assert(!is_member_object_pointer_v<int (t::*)() const>);
    static_assert(!is_member_object_pointer_v<int (t::*)() volatile>);
    static_assert(!is_member_object_pointer_v<int (t::*)() const volatile>);
    static_assert(!is_member_object_pointer_v<int (t::*)(int, float)>);
    static_assert(!is_member_object_pointer_v<int (t::*)(int, float) const>);
    static_assert(!is_member_object_pointer_v<int (t::*)(int, float) volatile>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) const volatile>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) &>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) const &>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) volatile &>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) const volatile &>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) &&>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) const &&>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) volatile &&>);
    static_assert(!is_member_object_pointer_v
                  <int (t::*)(int, float) const volatile &&>);
    static_assert(!is_member_object_pointer_v<int>);
    static_assert(!is_member_object_pointer_v<int ()>);
    static_assert(!is_member_object_pointer_v<int (int, int *)>);
    static_assert(!is_member_object_pointer_v<int (int, int *, ...)>);

    static_assert(is_trivial_empty_v<is_member_function_pointer<decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_function_pointer<const decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_function_pointer<volatile decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_function_pointer<const volatile decltype(p)>>);
    static_assert(is_trivial_empty_v
                  <is_member_function_pointer<int>>);
    static_assert(!is_member_function_pointer<decltype(p)>::value);
    static_assert(!is_member_function_pointer_v<decltype(p)>);
    static_assert(!is_member_function_pointer<int t::*>::value);
    static_assert(!is_member_function_pointer_v<int t::*>);
    static_assert(!is_member_function_pointer_v<int t::*const>);
    static_assert(!is_member_function_pointer_v<int t::*volatile>);
    static_assert(!is_member_function_pointer_v<int t::*const volatile>);
    static_assert(is_member_function_pointer_v<int (t::*)()>);
    static_assert(is_member_function_pointer_v<int (t::*)() const>);
    static_assert(is_member_function_pointer_v<int (t::*)() volatile>);
    static_assert(is_member_function_pointer_v<int (t::*)() const volatile>);
    static_assert(is_member_function_pointer_v<int (t::*)(int, float)>);
    static_assert(is_member_function_pointer_v<int (t::*)(int, float) const>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) volatile>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) const volatile>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) &>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) const &>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) volatile &>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) const volatile &>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) &&>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) const &&>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) volatile &&>);
    static_assert(is_member_function_pointer_v
                  <int (t::*)(int, float) const volatile &&>);
    static_assert(!is_member_function_pointer_v<int>);
    static_assert(!is_member_function_pointer_v<int ()>);
    static_assert(!is_member_function_pointer_v<int (int, int *)>);
    static_assert(!is_member_function_pointer_v<int (int, int *, ...)>);
  }
  // is_enum
  {
    enum t {a, b, c};
    enum struct tt {aa, bb, cc};
    static_assert(is_trivial_empty_v<is_enum<t>>);
    static_assert(is_trivial_empty_v<is_enum<const t>>);
    static_assert(is_trivial_empty_v<is_enum<volatile t>>);
    static_assert(is_trivial_empty_v<is_enum<const volatile t>>);
    static_assert(is_trivial_empty_v<is_enum<tt>>);
    static_assert(is_trivial_empty_v<is_enum<int>>);
    static_assert(is_enum<t>::value);
    static_assert(is_enum_v<t>);
    static_assert(is_enum_v<const t>);
    static_assert(is_enum_v<volatile t>);
    static_assert(is_enum_v<const volatile t>);
    static_assert(is_enum_v<decltype(a)>);
    static_assert(is_enum_v<tt>);
    static_assert(is_enum_v<const tt>);
    static_assert(is_enum_v<volatile tt>);
    static_assert(is_enum_v<const volatile tt>);
    static_assert(is_enum_v<decltype(tt::aa)>);
    static_assert(!is_enum_v<int>);
  }
  // is_union
  {
    union t {};
    static_assert(is_trivial_empty_v<is_union<t>>);
    static_assert(is_trivial_empty_v<is_union<const t>>);
    static_assert(is_trivial_empty_v<is_union<volatile t>>);
    static_assert(is_trivial_empty_v<is_union<const volatile t>>);
    static_assert(is_trivial_empty_v<is_union<int>>);
    static_assert(is_union<t>::value);
    static_assert(is_union_v<t>);
    static_assert(is_union_v<const t>);
    static_assert(is_union_v<volatile t>);
    static_assert(is_union_v<const volatile t>);
    static_assert(!is_union_v<t &>);
    static_assert(!is_union_v<int>);
  }
  // is_class
  {
    struct t {};
    static_assert(is_trivial_empty_v<is_class<t>>);
    static_assert(is_trivial_empty_v<is_class<const t>>);
    static_assert(is_trivial_empty_v<is_class<volatile t>>);
    static_assert(is_trivial_empty_v<is_class<const volatile t>>);
    static_assert(is_trivial_empty_v<is_class<int>>);
    static_assert(is_class<t>::value);
    static_assert(is_class_v<t>);
    static_assert(is_class_v<const t>);
    static_assert(is_class_v<volatile t>);
    static_assert(is_class_v<const volatile t>);
    static_assert(!is_class_v<int>);
  }
  // is_function
  {
    static_assert(is_trivial_empty_v<is_function<void ()>>);
    static_assert(is_trivial_empty_v<is_function<void () const>>);
    static_assert(is_trivial_empty_v<is_function<void () volatile>>);
    static_assert(is_trivial_empty_v<is_function<void () const volatile>>);
    static_assert(is_trivial_empty_v<is_function<void () &>>);
    static_assert(is_trivial_empty_v<is_function<void () const &>>);
    static_assert(is_trivial_empty_v<is_function<void () volatile &>>);
    static_assert(is_trivial_empty_v<is_function<void () const volatile &>>);
    static_assert(is_trivial_empty_v<is_function<void () &&>>);
    static_assert(is_trivial_empty_v<is_function<void () const &&>>);
    static_assert(is_trivial_empty_v<is_function<void () volatile &&>>);
    static_assert(is_trivial_empty_v<is_function<void () const volatile &&>>);
    static_assert(is_trivial_empty_v<is_function<void () noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () const noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () volatile noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void () const volatile noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () const & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () volatile & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void () const volatile & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () && noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void () const && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void () volatile && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void () const volatile && noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (...)>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const>>);
    static_assert(is_trivial_empty_v<is_function<void (...) volatile>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const volatile>>);
    static_assert(is_trivial_empty_v<is_function<void (...) &>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const &>>);
    static_assert(is_trivial_empty_v<is_function<void (...) volatile &>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const volatile &>>);
    static_assert(is_trivial_empty_v<is_function<void (...) &&>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const &&>>);
    static_assert(is_trivial_empty_v<is_function<void (...) volatile &&>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) const volatile &&>>);
    static_assert(is_trivial_empty_v<is_function<void (...) noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) volatile noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) const volatile noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (...) & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (...) const & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) volatile & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) const volatile & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (...) && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) const && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) volatile && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (...) const volatile && noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...)>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) const>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) volatile>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) &>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) const &>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) volatile &>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile &>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) &&>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) const &&>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) volatile &&>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile &&>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) volatile noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) volatile & noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile & noexcept>>);
    static_assert(is_trivial_empty_v<is_function<void (int, ...) && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) volatile && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile && noexcept>>);
    static_assert(is_trivial_empty_v
                  <is_function<void (int, ...) const volatile && noexcept>>);

    static_assert(is_function<void ()>::value);
    static_assert(is_function_v<void ()>);
    static_assert(is_function_v<void () const>);
    static_assert(is_function_v<void () volatile>);
    static_assert(is_function_v<void () const volatile>);
    static_assert(is_function_v<void () &>);
    static_assert(is_function_v<void () const &>);
    static_assert(is_function_v<void () volatile &>);
    static_assert(is_function_v<void () const volatile &>);
    static_assert(is_function_v<void () &&>);
    static_assert(is_function_v<void () const &&>);
    static_assert(is_function_v<void () volatile &&>);
    static_assert(is_function_v<void () const volatile &&>);
    static_assert(is_function_v<void () noexcept>);
    static_assert(is_function_v<void () const noexcept>);
    static_assert(is_function_v<void () volatile noexcept>);
    static_assert(is_function_v<void () const volatile noexcept>);
    static_assert(is_function_v<void () & noexcept>);
    static_assert(is_function_v<void () const & noexcept>);
    static_assert(is_function_v<void () volatile & noexcept>);
    static_assert(is_function_v<void () const volatile & noexcept>);
    static_assert(is_function_v<void () && noexcept>);
    static_assert(is_function_v<void () const && noexcept>);
    static_assert(is_function_v<void () volatile && noexcept>);
    static_assert(is_function_v<void () const volatile && noexcept>);
    static_assert(is_function_v<void (...)>);
    static_assert(is_function_v<void (...) const>);
    static_assert(is_function_v<void (...) volatile>);
    static_assert(is_function_v<void (...) const volatile>);
    static_assert(is_function_v<void (...) &>);
    static_assert(is_function_v<void (...) const &>);
    static_assert(is_function_v<void (...) volatile &>);
    static_assert(is_function_v<void (...) const volatile &>);
    static_assert(is_function_v<void (...) &&>);
    static_assert(is_function_v<void (...) const &&>);
    static_assert(is_function_v<void (...) volatile &&>);
    static_assert(is_function_v<void (...) const volatile &&>);
    static_assert(is_function_v<void (...) noexcept>);
    static_assert(is_function_v<void (...) const noexcept>);
    static_assert(is_function_v<void (...) volatile noexcept>);
    static_assert(is_function_v<void (...) const volatile noexcept>);
    static_assert(is_function_v<void (...) & noexcept>);
    static_assert(is_function_v<void (...) const & noexcept>);
    static_assert(is_function_v<void (...) volatile & noexcept>);
    static_assert(is_function_v<void (...) const volatile & noexcept>);
    static_assert(is_function_v<void (...) && noexcept>);
    static_assert(is_function_v<void (...) const && noexcept>);
    static_assert(is_function_v<void (...) volatile && noexcept>);
    static_assert(is_function_v<void (...) const volatile && noexcept>);
    static_assert(is_function_v<void (int, ...)>);
    static_assert(is_function_v<void (int, ...) const>);
    static_assert(is_function_v<void (int, ...) volatile>);
    static_assert(is_function_v<void (int, ...) const volatile>);
    static_assert(is_function_v<void (int, ...) &>);
    static_assert(is_function_v<void (int, ...) const &>);
    static_assert(is_function_v<void (int, ...) volatile &>);
    static_assert(is_function_v<void (int, ...) const volatile &>);
    static_assert(is_function_v<void (int, ...) &&>);
    static_assert(is_function_v<void (int, ...) const &&>);
    static_assert(is_function_v<void (int, ...) volatile &&>);
    static_assert(is_function_v<void (int, ...) const volatile &&>);
    static_assert(is_function_v<void (int, ...) noexcept>);
    static_assert(is_function_v<void (int, ...) const noexcept>);
    static_assert(is_function_v<void (int, ...) volatile noexcept>);
    static_assert(is_function_v<void (int, ...) const volatile noexcept>);
    static_assert(is_function_v<void (int, ...) & noexcept>);
    static_assert(is_function_v<void (int, ...) const & noexcept>);
    static_assert(is_function_v<void (int, ...) volatile & noexcept>);
    static_assert(is_function_v<void (int, ...) const volatile & noexcept>);
    static_assert(is_function_v<void (int, ...) && noexcept>);
    static_assert(is_function_v<void (int, ...) const && noexcept>);
    static_assert(is_function_v<void (int, ...) volatile && noexcept>);
    static_assert(is_function_v<void (int, ...) const volatile && noexcept>);
  }

  // is_reference
  {
    static_assert(is_trivial_empty_v<is_reference<int &>>);
    static_assert(is_trivial_empty_v<is_reference<int &&>>);
    static_assert(is_trivial_empty_v<is_reference<int>>);
    static_assert(is_reference<int &>::value);
    static_assert(is_reference_v<int &>);
    static_assert(is_reference_v<const int &>);
    static_assert(is_reference_v<volatile int &>);
    static_assert(is_reference_v<const volatile int &>);
    static_assert(is_reference_v<int &&>);
    static_assert(is_reference_v<const int &&>);
    static_assert(is_reference_v<volatile int &&>);
    static_assert(is_reference_v<const volatile int &&>);
    static_assert(!is_reference_v<int>);
  }
  // is_arithmetic
  {
    static_assert(is_trivial_empty_v<is_arithmetic<char>>);
    static_assert(is_trivial_empty_v<is_arithmetic<const char>>);
    static_assert(is_trivial_empty_v<is_arithmetic<volatile char>>);
    static_assert(is_trivial_empty_v<is_arithmetic<const volatile char>>);
    static_assert(is_trivial_empty_v<is_arithmetic<unsigned char>>);
    static_assert(is_trivial_empty_v<is_arithmetic<short>>);
    static_assert(is_trivial_empty_v<is_arithmetic<unsigned short>>);
    static_assert(is_trivial_empty_v<is_arithmetic<int>>);
    static_assert(is_trivial_empty_v<is_arithmetic<unsigned int>>);
    static_assert(is_trivial_empty_v<is_arithmetic<long>>);
    static_assert(is_trivial_empty_v<is_arithmetic<unsigned long>>);
    static_assert(is_trivial_empty_v<is_arithmetic<long long>>);
    static_assert(is_trivial_empty_v<is_arithmetic<unsigned long long>>);
    static_assert(is_trivial_empty_v<is_arithmetic<float>>);
    static_assert(is_trivial_empty_v<is_arithmetic<double>>);
    static_assert(is_trivial_empty_v<is_arithmetic<long double>>);
    static_assert(is_trivial_empty_v<is_arithmetic<void *>>);
    static_assert(is_arithmetic<char>::value);
    static_assert(is_arithmetic_v<char>);
    static_assert(is_arithmetic_v<int>);
    static_assert(is_arithmetic_v<const int>);
    static_assert(is_arithmetic_v<volatile int>);
    static_assert(is_arithmetic_v<const volatile int>);
    static_assert(is_arithmetic_v<float>);
    static_assert(!is_arithmetic_v<void>);
    static_assert(!is_arithmetic_v<void *>);
    static_assert(!is_arithmetic_v<void ()>);
    static_assert(!is_arithmetic_v<void (*)()>);
  }
  // is_fundamental
  // is_object
  // is_scalar
  // is_compound
  // is_referenceable
  {
    using integral_t = int;
    using float_t = float;
    using void_t = void;
    using nullptr_t = nullptr_t;

    using array_t = int [3];
    using pointer_t = int *;
    using reference_t = int &;
    using function_t = void ();

    enum enum_t {A, B, C};
    union union_t {};
    struct class_t {};
    using member_object_pointer_t = int class_t::*;
    using member_function_pointer_t = void (class_t::*)();

    static_assert(is_trivial_empty_v<is_fundamental<integral_t>>);
    static_assert(is_trivial_empty_v<is_fundamental<int []>>);
    static_assert(is_fundamental<integral_t>::value);
    static_assert(is_fundamental_v<integral_t>);
    static_assert(is_fundamental_v<const integral_t>);
    static_assert(is_fundamental_v<volatile integral_t>);
    static_assert(is_fundamental_v<const volatile integral_t>);
    static_assert(is_fundamental_v<float_t>);
    static_assert(is_fundamental_v<void_t>);
    static_assert(is_fundamental_v<nullptr_t>);
    static_assert(!is_fundamental_v<array_t>);
    static_assert(!is_fundamental_v<pointer_t>);
    static_assert(!is_fundamental_v<reference_t>);
    static_assert(!is_fundamental_v<function_t>);
    static_assert(!is_fundamental_v<enum_t>);
    static_assert(!is_fundamental_v<union_t>);
    static_assert(!is_fundamental_v<class_t>);
    static_assert(!is_fundamental_v<member_object_pointer_t>);
    static_assert(!is_fundamental_v<member_function_pointer_t>);

    static_assert(is_trivial_empty_v<is_compound<integral_t>>);
    static_assert(is_trivial_empty_v<is_compound<void>>);
    static_assert(!is_compound<integral_t>::value);
    static_assert(!is_compound_v<integral_t>);
    static_assert(!is_compound_v<const integral_t>);
    static_assert(!is_compound_v<volatile integral_t>);
    static_assert(!is_compound_v<const volatile integral_t>);
    static_assert(!is_compound_v<float_t>);
    static_assert(!is_compound_v<void_t>);
    static_assert(!is_compound_v<nullptr_t>);
    static_assert(is_compound_v<array_t>);
    static_assert(is_compound_v<pointer_t>);
    static_assert(is_compound_v<reference_t>);
    static_assert(is_compound_v<function_t>);
    static_assert(is_compound_v<enum_t>);
    static_assert(is_compound_v<union_t>);
    static_assert(is_compound_v<class_t>);
    static_assert(is_compound_v<member_object_pointer_t>);
    static_assert(is_compound_v<member_function_pointer_t>);

    static_assert(is_trivial_empty_v<is_object<integral_t>>);
    static_assert(is_trivial_empty_v<is_object<void>>);
    static_assert(is_object<integral_t>::value);
    static_assert(is_object_v<integral_t>);
    static_assert(is_object_v<const integral_t>);
    static_assert(is_object_v<volatile integral_t>);
    static_assert(is_object_v<const volatile integral_t>);
    static_assert(is_object_v<float_t>);
    static_assert(!is_object_v<void_t>);
    static_assert(is_object_v<nullptr_t>);
    static_assert(is_object_v<array_t>);
    static_assert(is_object_v<pointer_t>);
    static_assert(!is_object_v<reference_t>);
    static_assert(!is_object_v<function_t>);
    static_assert(is_object_v<enum_t>);
    static_assert(is_object_v<union_t>);
    static_assert(is_object_v<class_t>);
    static_assert(is_object_v<member_object_pointer_t>);
    static_assert(is_object_v<member_function_pointer_t>);

    static_assert(is_trivial_empty_v<is_scalar<integral_t>>);
    static_assert(is_trivial_empty_v<is_scalar<void>>);
    static_assert(is_scalar<integral_t>::value);
    static_assert(is_scalar_v<integral_t>);
    static_assert(is_scalar_v<const integral_t>);
    static_assert(is_scalar_v<volatile integral_t>);
    static_assert(is_scalar_v<const volatile integral_t>);
    static_assert(is_scalar_v<float_t>);
    static_assert(!is_scalar_v<void_t>);
    static_assert(is_scalar_v<nullptr_t>);
    static_assert(!is_scalar_v<array_t>);
    static_assert(is_scalar_v<pointer_t>);
    static_assert(!is_scalar_v<reference_t>);
    static_assert(!is_scalar_v<function_t>);
    static_assert(is_scalar_v<enum_t>);
    static_assert(!is_scalar_v<union_t>);
    static_assert(!is_scalar_v<class_t>);
    static_assert(is_scalar_v<member_object_pointer_t>);
    static_assert(is_scalar_v<member_function_pointer_t>);

    static_assert(is_trivial_empty_v<is_referenceable<integral_t>>);
    static_assert(is_trivial_empty_v<is_referenceable<void>>);
    static_assert(is_referenceable<integral_t>::value);
    static_assert(is_referenceable_v<integral_t>);
    static_assert(is_referenceable_v<const integral_t>);
    static_assert(is_referenceable_v<volatile integral_t>);
    static_assert(is_referenceable_v<const volatile integral_t>);
    static_assert(is_referenceable_v<float_t>);
    static_assert(!is_referenceable_v<void_t>);
    static_assert(is_referenceable_v<nullptr_t>);
    static_assert(is_referenceable_v<array_t>);
    static_assert(is_referenceable_v<pointer_t>);
    static_assert(is_referenceable_v<reference_t>);
    static_assert(is_referenceable_v<function_t>);
    static_assert(is_referenceable_v<enum_t>);
    static_assert(is_referenceable_v<union_t>);
    static_assert(is_referenceable_v<class_t>);
    static_assert(is_referenceable_v<member_object_pointer_t>);
    static_assert(is_referenceable_v<member_function_pointer_t>);
    static_assert(is_referenceable_v<void ()>);
    static_assert(!is_referenceable_v<void () &>);
    static_assert(!is_referenceable_v<void () &&>);
    static_assert(!is_referenceable_v<void () const>);
    static_assert(!is_referenceable_v<void () const &>);
    static_assert(!is_referenceable_v<void () const &&>);
  }
  // is_member_pointer
  {
    struct t {};
    static_assert(is_trivial_empty_v<is_member_pointer<int t::*>>);
    static_assert(is_trivial_empty_v<is_member_pointer<int t::*const>>);
    static_assert(is_trivial_empty_v<is_member_pointer<int t::*volatile>>);
    static_assert(is_trivial_empty_v
                  <is_member_pointer<int t::*const volatile>>);
    static_assert(is_trivial_empty_v<is_member_pointer<int>>);
    static_assert(is_member_pointer<int t::*>::value);
    static_assert(is_member_pointer_v<int t::*>);
    static_assert(is_member_pointer_v<int t::* const>);
    static_assert(is_member_pointer_v<int t::* volatile>);
    static_assert(is_member_pointer_v<int t::* const volatile>);
    static_assert(is_member_pointer_v<int (t::* const volatile)() &>);
    static_assert(!is_member_pointer_v<int>);
  }

  // is_const
  {
    static_assert(is_trivial_empty_v<is_const<int>>);
    static_assert(is_trivial_empty_v<is_const<const int>>);
    static_assert(is_trivial_empty_v<is_const<volatile int>>);
    static_assert(is_trivial_empty_v<is_const<const volatile int>>);
    static_assert(is_const<const int>::value);
    static_assert(is_const_v<const int>);
    static_assert(is_const_v<const volatile int>);
    static_assert(!is_const_v<int>);
    static_assert(!is_const_v<const int &>);
    static_assert(!is_const_v<const int *>);
    static_assert(is_const_v<int *const>);
  }
  // is_volatile
  {
    static_assert(is_trivial_empty_v<is_volatile<volatile int>>);
    static_assert(is_trivial_empty_v<is_volatile<int>>);
    static_assert(is_volatile<volatile int>::value);
    static_assert(is_volatile_v<volatile int>);
    static_assert(is_volatile_v<const volatile int>);
    static_assert(!is_volatile_v<int>);
    static_assert(!is_volatile_v<volatile int &>);
    static_assert(!is_volatile_v<volatile int *>);
    static_assert(is_volatile_v<int *volatile>);
  }
  // is_trivial
  // is_trivially_copyable
  {
    struct t1 {
      int x;
      t1() {}
      t1(const t1 &) {}
      t1 &operator =(const t1 &) {return *this;}
    };
    struct t2 {
      int x;
      t2() = default;
      t2(const t2 &) = default;
      t2 &operator =(const t2 &) = default;
    };
    static_assert(is_trivial_empty_v<is_trivial<t1>>);
    static_assert(is_trivial_empty_v<is_trivial<t2>>);
    static_assert(!is_trivial<t1>::value);
    static_assert(!is_trivial_v<t1>);
    static_assert(is_trivial_v<t2>);
    static_assert(is_trivial_v<const t2>);
    static_assert(is_trivial_v<volatile t2>);
    static_assert(is_trivial_v<const volatile t2>);
    static_assert(is_trivial_v<int>);
    static_assert(is_trivial_v<const float>);
    static_assert(is_trivial_v<int [2]>);
    static_assert(!is_trivial_v<int &>);
    static_assert(!is_trivial_v<int ()>);
    static_assert(is_trivial_v<int (*)()>);

    static_assert(is_trivial_empty_v<is_trivially_copyable<t1>>);
    static_assert(is_trivial_empty_v<is_trivially_copyable<t2>>);
    static_assert(!is_trivially_copyable<t1>::value);
    static_assert(is_trivially_copyable_v<t2>);
    static_assert(!is_trivially_copyable_v<int [2]>);
    static_assert(!is_trivially_copyable_v<int &>);
    static_assert(!is_trivially_copyable_v<int ()>);
  }
  // is_standard_layout
  {
    struct t0 {
      int x;
    };
    struct t {
      virtual void f() {}
    };
    struct tt {
      tt() {}
    };
    struct ttt {
      ttt(const tt &) {}
    };
    struct tttt {
      tttt &operator =(const tttt &) {return *this;}
    };
    struct ttttt {
    private:
      int x;
    public:
      int y;
    };
    static_assert(is_trivial_empty_v<is_standard_layout<t0>>);
    static_assert(is_trivial_empty_v<is_standard_layout<int &>>);
    static_assert(is_standard_layout<t0>::value);
    static_assert(is_standard_layout_v<t0>);
    static_assert(is_standard_layout_v<const t0>);
    static_assert(is_standard_layout_v<volatile t0>);
    static_assert(is_standard_layout_v<const volatile t0>);
    static_assert(is_standard_layout_v<tt>);
    static_assert(!is_trivial_v<tt>);
    static_assert(is_standard_layout_v<ttt>);
    static_assert(!is_trivial_v<ttt>);
    static_assert(is_standard_layout_v<tttt>);
    static_assert(!is_trivial_v<tttt>);
    static_assert(!is_standard_layout_v<ttttt>);
    static_assert(is_trivial_v<ttttt>);
  }
  // is_empty
  {
    struct t {};
    struct tt {};
    struct ttt : t, tt {};

    static_assert(is_trivial_empty_v<is_empty<t>>);
    static_assert(is_trivial_empty_v<is_empty<int>>);
    static_assert(is_empty<t>::value);
    static_assert(is_empty_v<t>);
    static_assert(is_empty_v<tt>);
    static_assert(is_empty_v<const tt>);
    static_assert(is_empty_v<volatile tt>);
    static_assert(is_empty_v<const volatile tt>);
    static_assert(is_empty_v<ttt>);
    static_assert(is_empty_v<integral_constant<int, 0>>);
    static_assert(!is_empty_v<int>);
  }
  // is_polymorphic
  // is_abstract
  {
    struct t {
      virtual void f() = 0;
    };
    struct tt {
      virtual void f() {}
    };
    struct ttt {};

    static_assert(is_trivial_empty_v<is_polymorphic<t>>);
    static_assert(is_trivial_empty_v<is_polymorphic<int>>);
    static_assert(is_polymorphic<t>::value);
    static_assert(is_polymorphic_v<t>);
    static_assert(is_polymorphic_v<const t>);
    static_assert(is_polymorphic_v<volatile t>);
    static_assert(is_polymorphic_v<const volatile t>);
    static_assert(is_polymorphic_v<tt>);
    static_assert(!is_polymorphic_v<ttt>);
    static_assert(!is_polymorphic_v<int>);

    static_assert(is_trivial_empty_v<is_abstract<t>>);
    static_assert(is_trivial_empty_v<is_abstract<int>>);
    static_assert(is_abstract<t>::value);
    static_assert(is_abstract_v<t>);
    static_assert(is_abstract_v<const t>);
    static_assert(is_abstract_v<volatile t>);
    static_assert(is_abstract_v<const volatile t>);
    static_assert(!is_abstract_v<tt>);
    static_assert(!is_abstract_v<ttt>);
    static_assert(!is_abstract_v<int>);
  }
  // is_final
  {
    struct t final {};

    static_assert(is_trivial_empty_v<is_final<t>>);
    static_assert(is_trivial_empty_v<is_final<int>>);
    static_assert(is_final<t>::value);
    static_assert(is_final_v<t>);
    static_assert(is_final_v<const t>);
    static_assert(is_final_v<volatile t>);
    static_assert(is_final_v<const volatile t>);
    static_assert(!is_final_v<int>);
  }
  // is_aggregate
  {
    struct t {
      int x;
      float y;
    };
    struct tt {
      int x;
      float y;

      tt() {}
    };
    struct ttt : t {
      double z;
    };
    static_assert(is_aggregate<t>::value);
    static_assert(is_aggregate_v<t>);
    static_assert(!is_aggregate<tt>::value);
    static_assert(!is_aggregate_v<tt>);
    static_assert(is_aggregate_v<ttt>);
  }
  // is_signed
  // is_unsigned
  {
    static_assert(is_trivial_empty_v<is_signed<bool>>);
    static_assert(is_trivial_empty_v<is_signed<const bool>>);
    static_assert(is_trivial_empty_v<is_signed<volatile bool>>);
    static_assert(is_trivial_empty_v<is_signed<const volatile bool>>);
    static_assert(is_trivial_empty_v<is_signed<void>>);
    static_assert(is_trivial_empty_v<is_unsigned<bool>>);
    static_assert(is_trivial_empty_v<is_unsigned<const bool>>);
    static_assert(is_trivial_empty_v<is_unsigned<volatile bool>>);
    static_assert(is_trivial_empty_v<is_unsigned<const volatile bool>>);
    static_assert(is_trivial_empty_v<is_unsigned<void>>);

    static_assert(!is_signed<bool>::value);
    static_assert(is_unsigned<bool>::value);
    static_assert(!is_signed_v<bool>);
    static_assert(!is_signed_v<const bool>);
    static_assert(!is_signed_v<volatile bool>);
    static_assert(!is_signed_v<const volatile bool>);
    static_assert(is_unsigned_v<bool>);
    static_assert(is_unsigned_v<const bool>);
    static_assert(is_unsigned_v<volatile bool>);
    static_assert(is_unsigned_v<const volatile bool>);

    static_assert((is_signed_v<char> && !is_unsigned_v<char>)
                  || (!is_signed_v<char> && is_unsigned_v<char>));

    static_assert(is_signed_v<signed char>);
    static_assert(!is_unsigned_v<signed char>);

    static_assert(!is_signed_v<unsigned char>);
    static_assert(is_unsigned_v<unsigned char>);

    static_assert(is_unsigned_v<char8_t> && !is_signed_v<char8_t>);
    static_assert(is_unsigned_v<char16_t> && !is_signed_v<char16_t>);
    static_assert(is_unsigned_v<char32_t> && !is_signed_v<char32_t>);
    static_assert((is_signed_v<wchar_t> && !is_unsigned_v<wchar_t>)
                  || (!is_signed_v<wchar_t> && is_unsigned_v<wchar_t>));

    static_assert(is_signed_v<short int>);
    static_assert(!is_unsigned_v<short int>);

    static_assert(!is_signed_v<unsigned short int>);
    static_assert(is_unsigned_v<unsigned short int>);

    static_assert(is_signed_v<int>);
    static_assert(!is_unsigned_v<int>);

    static_assert(!is_signed_v<unsigned int>);
    static_assert(is_unsigned_v<unsigned int>);

    static_assert(is_signed_v<long int>);
    static_assert(!is_unsigned_v<long int>);

    static_assert(!is_signed_v<unsigned long int>);
    static_assert(is_unsigned_v<unsigned long int>);

    static_assert(is_signed_v<long long int>);
    static_assert(!is_unsigned_v<long long int>);

    static_assert(!is_signed_v<unsigned long long int>);
    static_assert(is_unsigned_v<unsigned long long int>);

    static_assert(!is_signed_v<size_t>);
    static_assert(is_unsigned_v<size_t>);

    static_assert(is_signed_v<ptrdiff_t>);
    static_assert(!is_unsigned_v<ptrdiff_t>);

    static_assert(is_signed_v<int_least8_t>);
    static_assert(!is_unsigned_v<int_least8_t>);

    static_assert(!is_signed_v<uint_least8_t>);
    static_assert(is_unsigned_v<uint_least8_t>);

    static_assert(is_signed_v<int_least16_t>);
    static_assert(!is_unsigned_v<int_least16_t>);

    static_assert(!is_signed_v<uint_least16_t>);
    static_assert(is_unsigned_v<uint_least16_t>);

    static_assert(is_signed_v<int_least32_t>);
    static_assert(!is_unsigned_v<int_least32_t>);

    static_assert(!is_signed_v<uint_least32_t>);
    static_assert(is_unsigned_v<uint_least32_t>);

    static_assert(is_signed_v<int_least64_t>);
    static_assert(!is_unsigned_v<int_least64_t>);

    static_assert(!is_signed_v<uint_least64_t>);
    static_assert(is_unsigned_v<uint_least64_t>);

    static_assert(is_signed_v<intmax_t>);
    static_assert(!is_unsigned_v<intmax_t>);

    static_assert(!is_signed_v<uintmax_t>);
    static_assert(is_unsigned_v<uintmax_t>);

    static_assert(!is_signed_v<nullptr_t>);
    static_assert(!is_unsigned_v<nullptr_t>);

    static_assert(!is_signed_v<void>);
    static_assert(!is_unsigned_v<void>);

    static_assert(!is_signed_v<int &>);
    static_assert(!is_unsigned_v<int &>);

    static_assert(is_signed_v<float>);
    static_assert(!is_unsigned_v<float>);

    static_assert(is_signed_v<double>);
    static_assert(!is_unsigned_v<double>);

    static_assert(!is_signed_v<void *>);
    static_assert(!is_unsigned_v<void *>);

    static_assert(!is_signed_v<void ()>);
    static_assert(!is_unsigned_v<void ()>);

    static_assert(!is_signed_v<void (&)()>);
    static_assert(!is_unsigned_v<void (&)()>);

    static_assert(!is_signed_v<void (*)()>);
    static_assert(!is_unsigned_v<void (*)()>);
  }
  // is_bounded_array(_v)<T>
  // is_unbounded_array(_v)<T>
  {
    static_assert(is_trivial_empty_v<is_bounded_array<int [1]>>);
    static_assert(is_trivial_empty_v<is_bounded_array<int []>>);
    static_assert(is_trivial_empty_v<is_bounded_array<int>>);
    static_assert(is_trivial_empty_v<is_unbounded_array<int [1]>>);
    static_assert(is_trivial_empty_v<is_unbounded_array<int []>>);
    static_assert(is_trivial_empty_v<is_unbounded_array<int>>);

    static_assert(is_bounded_array<const volatile int [1]>::value);
    static_assert(is_bounded_array_v<const volatile int [1]>);
    static_assert(!is_unbounded_array_v<const volatile int [1]>);
    static_assert(is_unbounded_array<const volatile int []>::value);
    static_assert(is_unbounded_array_v<const volatile int []>);
    static_assert(!is_bounded_array_v<const volatile int []>);
    static_assert(!is_unbounded_array<int [0]>::value);
    static_assert(!is_unbounded_array_v<int [0]>);
    static_assert(!is_bounded_array<int [0]>::value);
    static_assert(!is_bounded_array_v<int [0]>);
    static_assert(!is_bounded_array_v<int>);
    static_assert(!is_unbounded_array_v<int>);
  }
  // is_constructible
  // is_default_constructible
  {
    static_assert(is_trivial_empty_v<is_constructible<int>>);
    static_assert(is_trivial_empty_v<is_constructible<int &>>);
    static_assert(is_trivial_empty_v<is_default_constructible<int>>);
    static_assert(is_trivial_empty_v<is_default_constructible<int &>>);

    static_assert(is_constructible<int>::value);
    static_assert(is_constructible_v<int>);
    static_assert(is_constructible_v<const int>);
    static_assert(is_constructible_v<volatile int>);
    static_assert(is_constructible_v<const volatile int>);

    static_assert(is_default_constructible<int>::value);
    static_assert(is_default_constructible_v<int>);

    static_assert(!is_constructible_v<int ()>);
    static_assert(!is_default_constructible_v<int ()>);

    static_assert(is_constructible_v<int (*)(), int (*)() noexcept>);
    static_assert(is_default_constructible_v<int (*)()>);

    static_assert(!is_constructible_v<int &, int>);
    static_assert(is_constructible_v<const int &, int &>);
    static_assert(!is_default_constructible_v<int &>);
  }
  // is_copy_constructible
  {
    static_assert(is_trivial_empty_v<is_copy_constructible<int>>);
    static_assert(is_trivial_empty_v<is_copy_constructible<void>>);

    struct t {
      t(const t &) = delete;
    };
    struct tt {
      tt(const tt &) {}
    };
    static_assert(is_copy_constructible<int>::value);
    static_assert(is_copy_constructible_v<int>);
    static_assert(is_copy_constructible_v<const int>);
    static_assert(is_copy_constructible_v<volatile int>);
    static_assert(is_copy_constructible_v<const volatile int>);
    static_assert(!is_copy_constructible_v<t>);
    static_assert(!is_copy_constructible_v<void>);
    static_assert(is_copy_constructible_v<tt>);
  }
  // is_move_constructible
  {
    static_assert(is_trivial_empty_v<is_move_constructible<int>>);
    static_assert(is_trivial_empty_v<is_move_constructible<void>>);

    struct t {
      t(t &&) = delete;
    };
    struct tt {
      tt(tt &&) {}
    };
    static_assert(is_move_constructible<int>::value);
    static_assert(is_move_constructible_v<int>);
    static_assert(is_move_constructible_v<const int>);
    static_assert(is_move_constructible_v<volatile int>);
    static_assert(is_move_constructible_v<const volatile int>);
    static_assert(!is_move_constructible_v<t>);
    static_assert(is_move_constructible_v<tt>);
  }
  // is_assignable
  {
    static_assert(is_trivial_empty_v<is_assignable<int, int>>);
    static_assert(is_trivial_empty_v<is_assignable<void, void>>);

    struct t {};
    struct tt {
      void operator =(t &) {}
    };
    static_assert(is_assignable<tt &, t &>::value);
    static_assert(is_assignable_v<tt &, t &>);
    static_assert(!is_assignable_v<tt &, const t &>);
    static_assert(!is_assignable_v<int &, void *>);
  }
  // is_copy_assignable
  {
    static_assert(is_trivial_empty_v<is_copy_assignable<int>>);
    static_assert(is_trivial_empty_v<is_copy_assignable<void>>);

    struct t {
      void operator =(const t &) {}
    };
    struct tt {
      void operator =(const tt &) = delete;
    };
    static_assert(is_copy_assignable<t>::value);
    static_assert(is_copy_assignable_v<t>);
    static_assert(is_copy_assignable_v<t &>);
    static_assert(!is_copy_assignable_v<tt>);
    static_assert(!is_copy_assignable_v<tt &>);
    static_assert(is_copy_assignable_v<int>);
    static_assert(is_copy_assignable_v<int &>);
    static_assert(!is_copy_assignable_v<void>);
    static_assert(!is_copy_assignable_v<int []>);
  }
  // is_move_assignable
  {
    static_assert(is_trivial_empty_v<is_move_assignable<int>>);
    static_assert(is_trivial_empty_v<is_move_assignable<void>>);

    struct t {
      void operator =(const t &) {}
    };
    struct tt {
      void operator =(tt &&) = delete;
    };
    static_assert(is_move_assignable<t>::value);
    static_assert(is_move_assignable_v<t>);
    static_assert(is_move_assignable_v<t &>);
    static_assert(!is_move_assignable_v<tt>);
    static_assert(!is_move_assignable_v<tt &>);
    static_assert(is_move_assignable_v<int>);
    static_assert(is_move_assignable_v<int &>);
    static_assert(!is_move_assignable_v<void>);
    static_assert(!is_move_assignable_v<int []>);
  }
  // is_destructible
  {
    struct t {
      ~t() = delete;
    };
    struct tt {};
    static_assert(is_trivial_empty_v<is_destructible<int>>);
    static_assert(is_trivial_empty_v<is_destructible<t>>);
    static_assert(!is_destructible<void>::value);
    static_assert(!is_destructible_v<void>);
    static_assert(!is_destructible_v<int []>);
    static_assert(is_destructible_v<int [3]>);
    static_assert(is_destructible_v<int [3][3][3][3][3]>);
    static_assert(!is_destructible_v<int [][3][3][3][3]>);
    static_assert(is_destructible_v<int &>);
    static_assert(is_destructible_v<const int *********&>);
    static_assert(!is_destructible_v<t>);
    static_assert(is_destructible_v<t &>);
    static_assert(is_destructible_v<t &&>);
    static_assert(is_destructible_v<tt>);
    static_assert(is_destructible_v<const tt>);
    static_assert(is_destructible_v<volatile tt>);
    static_assert(is_destructible_v<const volatile tt>);
  }
  // is_trivially_constructible
  // is_trivially_default_constructible
  // is_trivially_copy_constructible
  // is_trivially_move_constructible
  // is_trivially_assignable
  // is_trivially_copy_assignable
  // is_trivially_move_assignable
  // is_trivially_destructible
  {
    static_assert(is_trivial_empty_v
                  <is_trivially_constructible<int, int &>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_constructible<int, void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_default_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_default_constructible<reference_wrapper<int>>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_copy_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_copy_constructible<void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_move_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_move_constructible<void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_assignable<int &, const int &>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_assignable<int &, void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_copy_assignable<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_copy_assignable<void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_move_assignable<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_move_assignable<void>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_destructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_trivially_destructible<void>>);

    struct t {
      int &x;

      t(int &i) : x(i) {}
    };
    static_assert(!is_trivially_constructible<t, int &>::value);
    static_assert(!is_trivially_default_constructible<t>::value);
    static_assert(is_trivially_copy_constructible<t>::value);
    static_assert(is_trivially_move_constructible<t>::value);
    static_assert(!is_trivially_assignable<t &, t &>::value);
    static_assert(!is_trivially_copy_assignable<t>::value);
    static_assert(!is_trivially_move_assignable<t>::value);
    static_assert(is_trivially_destructible<t>::value);

    static_assert(!is_trivially_constructible_v<t, int &>);
    static_assert(!is_trivially_default_constructible_v<t>);
    static_assert(is_trivially_copy_constructible_v<t>);
    static_assert(is_trivially_move_constructible_v<t>);
    static_assert(!is_trivially_assignable_v<t &, t &>);
    static_assert(!is_trivially_copy_assignable_v<t>);
    static_assert(!is_trivially_move_assignable_v<t>);
    static_assert(is_trivially_destructible_v<t>);

    static_assert(!is_trivial_v<t>);
    static_assert(!is_trivial_v<int &>);
    static_assert(!is_trivially_default_constructible_v<int &>);
    static_assert(is_trivially_copy_constructible_v<int &>);
    static_assert(is_trivially_move_constructible_v<int &>);
    static_assert(is_trivially_assignable_v<int &, int &>);
    static_assert(is_trivially_copy_assignable_v<int &>);
    static_assert(is_trivially_move_assignable_v<int &>);

    static_assert(is_trivially_constructible_v<int, int>);
    static_assert(is_trivially_default_constructible_v<int>);
    static_assert(is_trivially_copy_constructible_v<int>);
    static_assert(is_trivially_move_constructible_v<int>);
    static_assert(is_trivially_assignable_v<int &, int &>);
    static_assert(is_trivially_copy_assignable_v<int>);
    static_assert(is_trivially_move_assignable_v<int>);

    struct tt {
      int x;
    };
    static_assert(is_trivially_constructible_v<tt>);
    static_assert(is_trivially_default_constructible_v<tt>);
    static_assert(is_trivially_copy_constructible_v<tt>);
    static_assert(is_trivially_move_constructible_v<tt>);
    static_assert(is_trivially_copy_assignable_v<tt>);
    static_assert(is_trivially_move_assignable_v<tt>);
    static_assert(is_trivially_destructible_v<tt>);

    static_assert(is_trivially_constructible_v<int, int>);
    static_assert(is_trivially_assignable_v<int &, int>);
    static_assert(is_trivially_constructible_v<int>);
    static_assert(is_trivially_default_constructible_v<int>);
    static_assert(is_trivially_copy_constructible_v<int>);
    static_assert(is_trivially_move_constructible_v<int>);
    static_assert(is_trivially_copy_assignable_v<int>);
    static_assert(is_trivially_move_assignable_v<int>);
    static_assert(is_trivially_destructible_v<int>);
  }
  // is_nothrow_constructible
  // is_nothrow_default_constructible
  // is_nothrow_copy_constructible
  // is_nothrow_move_constructible
  {
    static_assert(is_trivial_empty_v
                  <is_nothrow_constructible<int, int &>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_constructible<int, void>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_default_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_default_constructible<reference_wrapper<int>>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_copy_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_copy_constructible<void>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_move_constructible<int>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_move_constructible<void>>);

    static_assert(is_nothrow_constructible<int>::value);
    static_assert(is_nothrow_default_constructible<int>::value);
    static_assert(is_nothrow_copy_constructible<int>::value);
    static_assert(is_nothrow_move_constructible<int>::value);

    static_assert(is_nothrow_constructible_v<int>);
    static_assert(is_nothrow_default_constructible_v<int>);
    static_assert(is_nothrow_copy_constructible_v<int>);
    static_assert(is_nothrow_move_constructible_v<int>);

    static_assert(!is_nothrow_constructible_v<int []>);
    static_assert(!is_nothrow_default_constructible_v<int []>);
    static_assert(is_nothrow_constructible_v<int [1]>);
    static_assert(is_nothrow_default_constructible_v<int [1]>);
    static_assert(!is_nothrow_copy_constructible_v<int [1]>);
    static_assert(!is_nothrow_move_constructible_v<int [1]>);

    static_assert(!is_nothrow_constructible_v<void>);
    static_assert(!is_nothrow_copy_constructible_v<void>);
    static_assert(!is_nothrow_move_constructible_v<void>);

    struct t {
      int x;
    };
    static_assert(is_nothrow_constructible_v<t>);
    static_assert(is_nothrow_default_constructible_v<t>);
    static_assert(is_nothrow_copy_constructible_v<t>);
    static_assert(is_nothrow_move_constructible_v<t>);

    struct tt {
      tt(const tt &) {}
    };
    static_assert(!is_nothrow_copy_constructible_v<tt>);
    static_assert(!is_nothrow_move_constructible_v<tt>);

    struct ttt {
      ttt(const ttt &) = default;
    };
    static_assert(is_nothrow_copy_constructible_v<ttt>);
    static_assert(is_nothrow_move_constructible_v<ttt>);

    struct t2 {
      t2(const t2 &) noexcept;
    };
    static_assert(is_nothrow_copy_constructible_v<t2>);
    static_assert(is_nothrow_move_constructible_v<t2>);
  }
  // is_nothrow_assignable
  // is_nothrow_copy_assignable
  // is_nothrow_move_assignable
  {
    static_assert(is_trivial_empty_v
                  <is_nothrow_assignable<int &, const int &>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_assignable<int &, void>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_copy_assignable<int>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_copy_assignable<void>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_move_assignable<int>>);
    static_assert(is_trivial_empty_v
                  <is_nothrow_move_assignable<void>>);

    static_assert(is_nothrow_assignable<int &, int &>::value);
    static_assert(is_nothrow_copy_assignable<int>::value);
    static_assert(is_nothrow_move_assignable<int>::value);

    static_assert(is_nothrow_assignable_v<int &, int &>);
    static_assert(is_nothrow_copy_assignable_v<int>);
    static_assert(is_nothrow_move_assignable_v<int>);

    static_assert(!is_nothrow_assignable_v<int (&)[1], int (&)[1]>);
    static_assert(!is_nothrow_copy_assignable_v<int [1]>);
    static_assert(!is_nothrow_move_assignable_v<int [1]>);

    static_assert(!is_nothrow_assignable_v<void, void>);
    static_assert(!is_nothrow_copy_assignable_v<void>);
    static_assert(!is_nothrow_move_assignable_v<void>);

    struct t {
      int x;
    };
    static_assert(is_nothrow_assignable_v<t &, t &>);
    static_assert(is_nothrow_copy_assignable_v<t>);
    static_assert(is_nothrow_move_assignable_v<t>);

    struct tt {
      tt &operator =(const tt &);
    };
    static_assert(!is_nothrow_copy_assignable_v<tt>);
    static_assert(!is_nothrow_move_assignable_v<tt>);

    struct ttt {
      ttt &operator =(const ttt &) = default;
    };
    static_assert(is_nothrow_copy_assignable_v<ttt>);
    static_assert(is_nothrow_move_assignable_v<ttt>);

    struct t2 {
      t2 &operator =(const t2 &) noexcept;
    };
    static_assert(is_nothrow_copy_assignable_v<t2>);
    static_assert(is_nothrow_move_assignable_v<t2>);
  }
  // is_nothrow_destructible
  {
    static_assert(is_trivial_empty_v<is_nothrow_destructible<int>>);
    static_assert(is_trivial_empty_v<is_nothrow_destructible<void>>);

    struct t {
      ~t();
    };
    struct tt {
      ~tt() = delete;
    };
    static_assert(is_nothrow_destructible<int>::value);
    static_assert(is_nothrow_destructible_v<int>);
    static_assert(is_nothrow_destructible_v<t>);
    static_assert(!is_nothrow_destructible_v<tt>);
  }
  // has_virtual_destructor
  {
    struct t {
      virtual ~t() {}
    };
    static_assert(is_trivial_empty_v<has_virtual_destructor<t>>);
    static_assert(is_trivial_empty_v<has_virtual_destructor<int>>);
    static_assert(has_virtual_destructor<t>::value);
    static_assert(has_virtual_destructor_v<t>);
    static_assert(!has_virtual_destructor_v<int>);
  }
  // has_unique_object_representations(_v)<T>
  {
    static_assert(is_trivial_empty_v
                  <has_unique_object_representations<char>>);
    static_assert(is_trivial_empty_v
                  <has_unique_object_representations<void>>);
    static_assert(has_unique_object_representations_v<char>);
    static_assert(!has_unique_object_representations_v<void>);
  }

  // alignment_of
  {
    static_assert(is_trivial_empty_v<alignment_of<char>>);
    static_assert(alignment_of<char>::value == alignof(char));
    static_assert(alignment_of_v<char> == alignof(char));
    static_assert(alignment_of_v<const char> == alignof(char));
    static_assert(alignment_of_v<volatile char> == alignof(char));
    static_assert(alignment_of_v<const volatile char> == alignof(char));
    static_assert(alignment_of_v<const volatile char &> == alignof(char));
  }
  // rank
  {
    static_assert(is_trivial_empty_v<rank<int>>);
    static_assert(is_trivial_empty_v<rank<int []>>);
    static_assert(is_trivial_empty_v<rank<int [1]>>);
    static_assert(is_trivial_empty_v<rank<int [2][3]>>);
    static_assert(is_trivial_empty_v<rank<int [2][3][4]>>);

    static_assert(rank<void>::value == 0);
    static_assert(rank_v<void> == 0);
    static_assert(rank_v<int> == 0);
    static_assert(rank_v<int &> == 0);
    static_assert(rank_v<int []> == 1);
    static_assert(rank_v<int [][3]> == 2);
    static_assert(rank_v<int [][3][4]> == 3);
    static_assert(rank_v<int [][3][4][5]> == 4);
    static_assert(rank_v<int [1][2][3][4][5]> == 5);
    static_assert(rank_v<const int [1][2][3][4][5]> == 5);
    static_assert(rank_v<volatile int [1][2][3][4][5]> == 5);
    static_assert(rank_v<const volatile int [1][2][3][4][5]> == 5);
    static_assert(rank_v<const volatile int (&)[1][2][3][4][5]> == 0);
  }
  // extent
  {
    static_assert(is_trivial_empty_v<extent<int, 0>>);
    static_assert(is_trivial_empty_v<extent<int [1], 0>>);
    static_assert(is_trivial_empty_v<extent<int [1][2][3], 0>>);
    static_assert(is_trivial_empty_v<extent<int [1][2][3], 1>>);
    static_assert(is_trivial_empty_v<extent<int [1][2][3][4], 3>>);

    static_assert(extent<int>::value == 0);
    static_assert(extent_v<int> == 0);
    static_assert(extent_v<int []> == 0);
    static_assert(extent_v<int [3]> == 3);
    static_assert(extent_v<int [3][4]> == 3);
    static_assert(extent_v<int [3][4][5], 2> == 5);
    static_assert(extent_v<int [3][4][5], 1> == 4);
    static_assert(extent_v<int [3][4][5], 0> == 3);
    static_assert(extent_v<int [][4][5], 0> == 0);
    static_assert(extent_v<int (&)[][4][5], 1> == 0);
    static_assert(extent_v<int, 100> == 0);
  }

  // is_same
  {
    static_assert(is_trivial_empty_v<is_same<void, void>>);
    static_assert(is_trivial_empty_v<is_same<void, void *>>);
    static_assert(is_same<void, void>::value);
    static_assert(is_same_v<void, void>);
    static_assert(is_same_v<int, int>);
    static_assert(!is_same_v<int, int &>);
    static_assert(!is_same_v<int &, int>);
    static_assert(!is_same_v<int, const int>);
    static_assert(!is_same_v<int, int[]>);
  }
  // is_base_of
  {
    struct A {};
    struct B : A {};
    struct C {};
    struct D : private C {};

    static_assert(is_trivial_empty_v<is_base_of<A, B>>);
    static_assert(is_trivial_empty_v<is_base_of<B, A>>);

    static_assert(is_base_of<A, B>::value);
    static_assert(is_base_of_v<A, B>);
    static_assert(is_base_of_v<C, D>);
    static_assert(!is_base_of_v<C, B>);
    static_assert(is_base_of_v<C, D>);

    static_assert(is_base_of_v<C, const volatile D>);
    static_assert(is_base_of_v<const C, const volatile D>);
    static_assert(is_base_of_v<volatile C, const volatile D>);
    static_assert(is_base_of_v<const volatile C, const volatile D>);
    static_assert(is_base_of_v<C, volatile D>);
    static_assert(is_base_of_v<const C, volatile D>);
    static_assert(is_base_of_v<volatile C, volatile D>);
    static_assert(is_base_of_v<const volatile C, volatile D>);
    static_assert(is_base_of_v<C, const D>);
    static_assert(is_base_of_v<const C, const D>);
    static_assert(is_base_of_v<volatile C, const D>);
    static_assert(is_base_of_v<const volatile C, const D>);
    static_assert(is_base_of_v<C, D>);
    static_assert(is_base_of_v<const C, D>);
    static_assert(is_base_of_v<volatile C, D>);
    static_assert(is_base_of_v<const volatile C, D>);

    static_assert(!is_base_of_v<int, int>);
    static_assert(is_base_of_v<A, A>);
    static_assert(is_base_of_v<const A, A>);
    static_assert(is_base_of_v<volatile A, A>);
    static_assert(is_base_of_v<const volatile A, A>);
    static_assert(is_base_of_v<A, const A>);
    static_assert(is_base_of_v<A, volatile A>);
    static_assert(is_base_of_v<A, const volatile A>);
  }
  // is_convertible
  {
    static_assert(is_trivial_empty_v<is_convertible<int, void>>);
    static_assert(is_trivial_empty_v<is_convertible<int, int>>);

    static_assert(!is_convertible_v<int, void>);
    static_assert(!is_convertible_v<const int &, int *>);
    static_assert(is_convertible_v<double, int>);

    struct t {};
    struct tt {
      explicit tt(const t &) {}
    };
    struct ttt {
      ttt(const t &) {}
    };
    struct tttt {
      operator t() {return {};}
    };
    static_assert(!is_convertible_v<t, tt>);
    static_assert(is_convertible_v<t, ttt>);
    static_assert(is_convertible_v<tttt, t>);
    static_assert(!is_convertible_v<const tttt, t>);

    // convert between int lvalue references
    // convert between int rvalue references
    {
      static_assert(is_convertible_v<int &, int &>);
      static_assert(is_convertible_v<int &, const int &>);
      static_assert(is_convertible_v<int &, volatile int &>);
      static_assert(is_convertible_v<int &, const volatile int &>);
      static_assert(!is_convertible_v<const int &, int &>);
      static_assert(is_convertible_v<const int &, const int &>);
      static_assert(!is_convertible_v<const int &, volatile int &>);
      static_assert(is_convertible_v<const int &, const volatile int &>);
      static_assert(!is_convertible_v<volatile int &, int &>);
      static_assert(!is_convertible_v<volatile int &, const int &>);
      static_assert(is_convertible_v<volatile int &, volatile int &>);
      static_assert(is_convertible_v<volatile int &, const volatile int &>);
      static_assert(!is_convertible_v<const volatile int &, int &>);
      static_assert(!is_convertible_v<const volatile int &, const int &>);
      static_assert(!is_convertible_v<const volatile int &, volatile int &>);
      static_assert(is_convertible_v
                    <const volatile int &, const volatile int &>);

      static_assert(is_convertible_v<int &&, int &&>);
      static_assert(is_convertible_v<int &&, const int &&>);
      static_assert(is_convertible_v<int &&, volatile int &&>);
      static_assert(is_convertible_v<int &&, const volatile int &&>);
      static_assert(!is_convertible_v<const int &&, int &&>);
      static_assert(is_convertible_v<const int &&, const int &&>);
      static_assert(!is_convertible_v<const int &&, volatile int &&>);
      static_assert(is_convertible_v<const int &&, const volatile int &&>);
      static_assert(!is_convertible_v<volatile int &&, int &&>);
      static_assert(!is_convertible_v<volatile int &&, const int &&>);
      static_assert(is_convertible_v<volatile int &&, volatile int &&>);
      static_assert(is_convertible_v<volatile int &&, const volatile int &&>);
      static_assert(!is_convertible_v<const volatile int &&, int &&>);
      static_assert(!is_convertible_v<const volatile int &&, const int &&>);
      static_assert(!is_convertible_v<const volatile int &&, volatile int &&>);
      static_assert(is_convertible_v
                    <const volatile int &&, const volatile int &&>);
    }
    // convert between int lvalue reference and rvalue reference
    {
      static_assert(!is_convertible_v<int &, int &&>);
      static_assert(!is_convertible_v<int &, const int &&>);
      static_assert(!is_convertible_v<int &, volatile int &&>);
      static_assert(!is_convertible_v<int &, const volatile int &&>);
      static_assert(!is_convertible_v<const int &, int &&>);
      static_assert(!is_convertible_v<const int &, const int &&>);
      static_assert(!is_convertible_v<const int &, volatile int &&>);
      static_assert(!is_convertible_v<const int &, const volatile int &&>);
      static_assert(!is_convertible_v<volatile int &, int &&>);
      static_assert(!is_convertible_v<volatile int &, const int &&>);
      static_assert(!is_convertible_v<volatile int &, volatile int &&>);
      static_assert(!is_convertible_v<volatile int &, const volatile int &&>);
      static_assert(!is_convertible_v<const volatile int &, int &&>);
      static_assert(!is_convertible_v<const volatile int &, const int &&>);
      static_assert(!is_convertible_v<const volatile int &, volatile int &&>);
      static_assert(!is_convertible_v
                    <const volatile int &, const volatile int &&>);

      static_assert(!is_convertible_v<int &&, int &>);
      static_assert(is_convertible_v<int &&, const int &>);
      static_assert(!is_convertible_v<int &&, volatile int &>);
      static_assert(!is_convertible_v<int &&, const volatile int &>);
      static_assert(!is_convertible_v<const int &&, int &>);
      static_assert(is_convertible_v<const int &&, const int &>);
      static_assert(!is_convertible_v<const int &&, volatile int &>);
      static_assert(!is_convertible_v<const int &&, const volatile int &>);
      static_assert(!is_convertible_v<volatile int &&, int &>);
      static_assert(!is_convertible_v<volatile int &&, const int &>);
      static_assert(!is_convertible_v<volatile int &&, volatile int &>);
      static_assert(!is_convertible_v<volatile int &&, const volatile int &>);
      static_assert(!is_convertible_v<const volatile int &&, int &>);
      static_assert(!is_convertible_v<const volatile int &&, const int &>);
      static_assert(!is_convertible_v<const volatile int &&, volatile int &>);
      static_assert(!is_convertible_v
                    <const volatile int &&, const volatile int &>);
    }
  }
  // is_nothrow_convertible
  {
    static_assert(is_trivial_empty_v<is_nothrow_convertible<int, void>>);
    static_assert(is_trivial_empty_v<is_nothrow_convertible<int, int>>);

    struct t {};
    struct tt {explicit tt(const t &) {}};
    struct ttt {ttt(const t &) {}};
    struct tttt {tttt(const t &) noexcept {}};
    static_assert(is_nothrow_convertible_v<int *, void *>);
    static_assert(is_nothrow_convertible_v<t, tttt>);
    static_assert(!is_nothrow_convertible_v<t, ttt>);
    static_assert(!is_nothrow_convertible_v<t, tt>);
  }

  // remove_const
  // remove_volatile
  // remove_cv
  // add_const
  // add_volatile
  // add_cv
  {
    static_assert(is_trivial_empty_v<remove_const<int>>);
    static_assert(is_trivial_empty_v<remove_const<const int>>);
    static_assert(is_trivial_empty_v<remove_const<const volatile int>>);
    static_assert(is_trivial_empty_v<remove_volatile<int>>);
    static_assert(is_trivial_empty_v<remove_volatile<const int>>);
    static_assert(is_trivial_empty_v<remove_volatile<const volatile int>>);
    static_assert(is_trivial_empty_v<remove_cv<int>>);
    static_assert(is_trivial_empty_v<remove_cv<const int>>);
    static_assert(is_trivial_empty_v<remove_cv<const volatile int>>);
    static_assert(is_trivial_empty_v<add_const<int>>);
    static_assert(is_trivial_empty_v<add_const<const int>>);
    static_assert(is_trivial_empty_v<add_const<const volatile int>>);
    static_assert(is_trivial_empty_v<add_volatile<int>>);
    static_assert(is_trivial_empty_v<add_volatile<const int>>);
    static_assert(is_trivial_empty_v<add_volatile<const volatile int>>);
    static_assert(is_trivial_empty_v<add_cv<int>>);
    static_assert(is_trivial_empty_v<add_cv<const int>>);
    static_assert(is_trivial_empty_v<add_cv<const volatile int>>);

    using integral_t = int;
    using float_t = float;
    using void_t = void;
    using nullptr_t = nullptr_t;

    using array_t = int [3];
    using pointer_t = int *;
    using reference_t = int &;
    using function_t = void ();

    enum enum_t {A, B, C};
    union union_t {};
    struct class_t {};
    using member_object_pointer_t = int class_t::*;
    using member_function_pointer_t = void (class_t::*)();

    static_assert(is_same_v<add_const<integral_t>::type, const integral_t>);
    static_assert(is_same_v<add_const_t<integral_t>, const integral_t>);
    static_assert(is_same_v<add_const_t<float_t>, const float_t>);
    static_assert(is_same_v<add_const_t<void_t>, const void_t>);
    static_assert(is_same_v<add_const_t<nullptr_t>, const nullptr_t>);
    static_assert(is_same_v<add_const_t<array_t>, const array_t>);
    static_assert(is_same_v<add_const_t<pointer_t>, const pointer_t>);
    static_assert(is_same_v<add_const_t<reference_t>, reference_t>);
    static_assert(is_same_v<add_const_t<function_t>, function_t>);
    static_assert(is_same_v<add_const_t<enum_t>, const enum_t>);
    static_assert(is_same_v<add_const_t<union_t>, const union_t>);
    static_assert(is_same_v<add_const_t<class_t>, const class_t>);
    static_assert(is_same_v
                  <add_const_t<member_object_pointer_t>,
                   const member_object_pointer_t>);
    static_assert(is_same_v
                  <add_const_t<member_function_pointer_t>,
                   const member_function_pointer_t>);

    static_assert(is_same_v<remove_const<int>::type, int>);
    static_assert(is_same_v<remove_const_t<int>, int>);
    static_assert(is_same_v<remove_const_t<const int>, int>);
    static_assert(is_same_v
                  <remove_const_t<const volatile int>, volatile int>);

    static_assert(is_same_v
                  <add_volatile<integral_t>::type, volatile integral_t>);
    static_assert(is_same_v<add_volatile_t<integral_t>, volatile integral_t>);
    static_assert(is_same_v<add_volatile_t<float_t>, volatile float_t>);
    static_assert(is_same_v<add_volatile_t<void_t>, volatile void_t>);
    static_assert(is_same_v<add_volatile_t<nullptr_t>, volatile nullptr_t>);
    static_assert(is_same_v<add_volatile_t<array_t>, volatile array_t>);
    static_assert(is_same_v<add_volatile_t<pointer_t>, volatile pointer_t>);
    static_assert(is_same_v<add_volatile_t<reference_t>, reference_t>);
    static_assert(is_same_v<add_volatile_t<function_t>, function_t>);
    static_assert(is_same_v<add_volatile_t<enum_t>, volatile enum_t>);
    static_assert(is_same_v<add_volatile_t<union_t>, volatile union_t>);
    static_assert(is_same_v<add_volatile_t<class_t>, volatile class_t>);
    static_assert(is_same_v
                  <add_volatile_t<member_object_pointer_t>,
                   volatile member_object_pointer_t>);
    static_assert(is_same_v
                  <add_volatile_t<member_function_pointer_t>,
                   volatile member_function_pointer_t>);

    static_assert(is_same_v<remove_volatile<int>::type, int>);
    static_assert(is_same_v<remove_volatile_t<int>, int>);
    static_assert(is_same_v<remove_volatile_t<volatile int>, int>);
    static_assert(is_same_v
                  <remove_volatile_t<const volatile int>, const int>);

    static_assert(is_same_v
                  <add_cv<integral_t>::type, const volatile integral_t>);
    static_assert(is_same_v<add_cv_t<integral_t>, const volatile integral_t>);
    static_assert(is_same_v<add_cv_t<float_t>, const volatile float_t>);
    static_assert(is_same_v<add_cv_t<void_t>, const volatile void_t>);
    static_assert(is_same_v<add_cv_t<nullptr_t>, const volatile nullptr_t>);
    static_assert(is_same_v<add_cv_t<array_t>, const volatile array_t>);
    static_assert(is_same_v<add_cv_t<pointer_t>, const volatile pointer_t>);
    static_assert(is_same_v<add_cv_t<reference_t>, reference_t>);
    static_assert(is_same_v<add_cv_t<function_t>, function_t>);
    static_assert(is_same_v<add_cv_t<enum_t>, const volatile enum_t>);
    static_assert(is_same_v<add_cv_t<union_t>, const volatile union_t>);
    static_assert(is_same_v<add_cv_t<class_t>, const volatile class_t>);
    static_assert(is_same_v
                  <add_cv_t<member_object_pointer_t>,
                   const volatile member_object_pointer_t>);
    static_assert(is_same_v
                  <add_cv_t<member_function_pointer_t>,
                   const volatile member_function_pointer_t>);

    static_assert(is_same_v<remove_cv<int>::type, int>);
    static_assert(is_same_v<remove_cv_t<int>, int>);
    static_assert(is_same_v<remove_cv_t<const int>, int>);
    static_assert(is_same_v<remove_cv_t<volatile int>, int>);
    static_assert(is_same_v<remove_cv_t<const volatile int>, int>);
  }

  // remove_reference
  // add_lvalue_reference
  // add_rvalue_reference
  {
    static_assert(is_trivial_empty_v<remove_reference<int &>>);
    static_assert(is_trivial_empty_v<remove_reference<int &&>>);
    static_assert(is_trivial_empty_v<remove_reference<const int &>>);
    static_assert(is_trivial_empty_v<remove_reference<const int &&>>);
    static_assert(is_trivial_empty_v<add_lvalue_reference<int>>);
    static_assert(is_trivial_empty_v<add_lvalue_reference<int &>>);
    static_assert(is_trivial_empty_v<add_lvalue_reference<int &&>>);
    static_assert(is_trivial_empty_v<add_lvalue_reference<const int &>>);
    static_assert(is_trivial_empty_v<add_lvalue_reference<const int &&>>);
    static_assert(is_trivial_empty_v<add_rvalue_reference<int>>);
    static_assert(is_trivial_empty_v<add_rvalue_reference<int &>>);
    static_assert(is_trivial_empty_v<add_rvalue_reference<int &&>>);
    static_assert(is_trivial_empty_v<add_rvalue_reference<const int &>>);
    static_assert(is_trivial_empty_v<add_rvalue_reference<const int &&>>);

    static_assert(is_same_v<remove_reference<int>::type, int>);
    static_assert(is_same_v<remove_reference_t<int>, int>);
    static_assert(is_same_v<remove_reference_t<const int &>, const int>);
    static_assert(is_same_v<remove_reference_t<int &>, int>);
    static_assert(is_same_v<remove_reference_t<int &&>, int>);

    static_assert(is_same_v<add_lvalue_reference<int>::type, int &>);
    static_assert(is_same_v<add_lvalue_reference_t<int>, int &>);
    static_assert(is_same_v<add_lvalue_reference_t<int &>, int &>);
    static_assert(is_same_v<add_lvalue_reference_t<int &&>, int &>);

    static_assert(is_same_v<add_rvalue_reference<int>::type, int &&>);
    static_assert(is_same_v<add_rvalue_reference_t<int>, int &&>);
    static_assert(is_same_v<add_rvalue_reference_t<int &>, int &>);
    static_assert(is_same_v<add_rvalue_reference_t<int &&>, int &&>);
  }

  // make_signed
  {
    static_assert(is_trivial_empty_v<make_signed<int>>);
    static_assert(is_trivial_empty_v<make_signed<unsigned>>);
    static_assert(is_trivial_empty_v<make_signed<const unsigned>>);
    static_assert(is_trivial_empty_v<make_signed<volatile unsigned>>);
    static_assert(is_trivial_empty_v<make_signed<const volatile unsigned>>);

    static_assert(is_same_v<make_signed<unsigned>::type, int>);
    static_assert(is_same_v<make_signed_t<const unsigned>, const int>);
    static_assert(is_same_v<make_signed_t<volatile unsigned>, volatile int>);
    static_assert(is_same_v
                  <make_signed_t<const volatile unsigned>, const volatile int>);

    static_assert(is_same_v<make_signed_t<bool>, bool>);
    static_assert(is_same_v<make_signed_t<char>, signed char>);
    static_assert(is_same_v<make_signed_t<unsigned char>, signed char>);
    static_assert(is_same_v<make_signed_t<signed char>, signed char>);
    static_assert(is_same_v<make_signed_t<char8_t>, signed char>);
    static_assert(is_same_v<make_signed_t<char16_t>, int_least16_t>);
    static_assert(is_same_v<make_signed_t<char32_t>, int_least32_t>);
    static_assert(is_same_v<make_signed_t<short>, short>);
    static_assert(is_same_v<make_signed_t<unsigned short>, short>);
    static_assert(is_same_v<make_signed_t<int>, int>);
    static_assert(is_same_v<make_signed_t<unsigned int>, int>);
    static_assert(is_same_v<make_signed_t<long>, long>);
    static_assert(is_same_v<make_signed_t<unsigned long>, long>);
    static_assert(is_same_v<make_signed_t<long long>, long long>);
    static_assert(is_same_v<make_signed_t<unsigned long long>, long long>);
    static_assert(is_same_v<make_signed_t<int_least8_t>, int_least8_t>);
    static_assert(is_same_v<make_signed_t<uint_least8_t>, int_least8_t>);
    static_assert(is_same_v<make_signed_t<int_least16_t>, int_least16_t>);
    static_assert(is_same_v<make_signed_t<uint_least16_t>, int_least16_t>);
    static_assert(is_same_v<make_signed_t<int_least32_t>, int_least32_t>);
    static_assert(is_same_v<make_signed_t<uint_least32_t>, int_least32_t>);
    static_assert(is_same_v<make_signed_t<int_least64_t>, int_least64_t>);
    static_assert(is_same_v<make_signed_t<uint_least64_t>, int_least64_t>);
    static_assert(is_same_v<make_signed_t<int_fast8_t>, int_fast8_t>);
    static_assert(is_same_v<make_signed_t<uint_fast8_t>, int_fast8_t>);
    static_assert(is_same_v<make_signed_t<int_fast16_t>, int_fast16_t>);
    static_assert(is_same_v<make_signed_t<uint_fast16_t>, int_fast16_t>);
    static_assert(is_same_v<make_signed_t<int_fast32_t>, int_fast32_t>);
    static_assert(is_same_v<make_signed_t<uint_fast32_t>, int_fast32_t>);
    static_assert(is_same_v<make_signed_t<int_fast64_t>, int_fast64_t>);
    static_assert(is_same_v<make_signed_t<uint_fast64_t>, int_fast64_t>);
    static_assert(is_same_v<make_signed_t<uintmax_t>, intmax_t>);
    static_assert(is_same_v<make_signed_t<intmax_t>, intmax_t>);
    static_assert(is_same_v<make_signed_t<int8_t>, int8_t>);
    static_assert(is_same_v<make_signed_t<uint8_t>, int8_t>);
    static_assert(is_same_v<make_signed_t<int16_t>, int16_t>);
    static_assert(is_same_v<make_signed_t<uint16_t>, int16_t>);
    static_assert(is_same_v<make_signed_t<int32_t>, int32_t>);
    static_assert(is_same_v<make_signed_t<uint32_t>, int32_t>);
    static_assert(is_same_v<make_signed_t<int64_t>, int64_t>);
    static_assert(is_same_v<make_signed_t<uint64_t>, int64_t>);

    static_assert(is_signed_v<make_signed_t<wchar_t>>);
    static_assert(sizeof(make_signed_t<wchar_t>) == sizeof(wchar_t));
    static_assert(is_integral_v<make_signed_t<wchar_t>>);

    static_assert(is_signed_v<make_signed_t<ptrdiff_t>>);
    static_assert(is_integral_v<make_signed_t<ptrdiff_t>>);
    static_assert(sizeof(make_signed_t<ptrdiff_t>) == sizeof(ptrdiff_t));

    static_assert(is_signed_v<make_signed_t<size_t>>);
    static_assert(is_integral_v<make_signed_t<size_t>>);
    static_assert(sizeof(make_signed_t<size_t>) == sizeof(size_t));

    static_assert(is_same_v<make_signed_t<float>, float>);
    static_assert(is_same_v<make_signed_t<double>, double>);
    static_assert(is_same_v<make_signed_t<long double>, long double>);
  }
  // make_unsigned
  {
    static_assert(is_trivial_empty_v<make_unsigned<unsigned>>);
    static_assert(is_trivial_empty_v<make_unsigned<int>>);
    static_assert(is_trivial_empty_v<make_unsigned<const int>>);
    static_assert(is_trivial_empty_v<make_unsigned<volatile int>>);
    static_assert(is_trivial_empty_v<make_unsigned<const volatile int>>);

    static_assert(is_same_v<make_unsigned<int>::type, unsigned>);
    static_assert(is_same_v<make_unsigned_t<const int>, const unsigned>);
    static_assert(is_same_v<make_unsigned_t<volatile int>, volatile unsigned>);
    static_assert(is_same_v
                  <make_unsigned_t<const volatile int>,
                   const volatile unsigned>);

    static_assert(is_same_v<make_unsigned_t<bool>, bool>);
    static_assert(is_same_v<make_unsigned_t<char>, unsigned char>);
    static_assert(is_same_v<make_unsigned_t<unsigned char>, unsigned char>);
    static_assert(is_same_v<make_unsigned_t<signed char>, unsigned char>);
    static_assert(is_same_v<make_unsigned_t<char8_t>, unsigned char>);
    static_assert(is_same_v<make_unsigned_t<char16_t>, uint_least16_t>);
    static_assert(is_same_v<make_unsigned_t<char32_t>, uint_least32_t>);
    static_assert(is_same_v<make_unsigned_t<short>, unsigned short>);
    static_assert(is_same_v<make_unsigned_t<unsigned short>, unsigned short>);
    static_assert(is_same_v<make_unsigned_t<int>, unsigned>);
    static_assert(is_same_v<make_unsigned_t<unsigned int>, unsigned>);
    static_assert(is_same_v<make_unsigned_t<long>, unsigned long>);
    static_assert(is_same_v<make_unsigned_t<unsigned long>, unsigned long>);
    static_assert(is_same_v<make_unsigned_t<long long>, unsigned long long>);
    static_assert(is_same_v
                  <make_unsigned_t<unsigned long long>, unsigned long long>);
    static_assert(is_same_v<make_unsigned_t<int_least8_t>, uint_least8_t>);
    static_assert(is_same_v<make_unsigned_t<uint_least8_t>, uint_least8_t>);
    static_assert(is_same_v<make_unsigned_t<int_least16_t>, uint_least16_t>);
    static_assert(is_same_v<make_unsigned_t<uint_least16_t>, uint_least16_t>);
    static_assert(is_same_v<make_unsigned_t<int_least32_t>, uint_least32_t>);
    static_assert(is_same_v<make_unsigned_t<uint_least32_t>, uint_least32_t>);
    static_assert(is_same_v<make_unsigned_t<int_least64_t>, uint_least64_t>);
    static_assert(is_same_v<make_unsigned_t<uint_least64_t>, uint_least64_t>);
    static_assert(is_same_v<make_unsigned_t<int_fast8_t>, uint_fast8_t>);
    static_assert(is_same_v<make_unsigned_t<uint_fast8_t>, uint_fast8_t>);
    static_assert(is_same_v<make_unsigned_t<int_fast16_t>, uint_fast16_t>);
    static_assert(is_same_v<make_unsigned_t<uint_fast16_t>, uint_fast16_t>);
    static_assert(is_same_v<make_unsigned_t<int_fast32_t>, uint_fast32_t>);
    static_assert(is_same_v<make_unsigned_t<uint_fast32_t>, uint_fast32_t>);
    static_assert(is_same_v<make_unsigned_t<int_fast64_t>, uint_fast64_t>);
    static_assert(is_same_v<make_unsigned_t<uint_fast64_t>, uint_fast64_t>);
    static_assert(is_same_v<make_unsigned_t<uintmax_t>, uintmax_t>);
    static_assert(is_same_v<make_unsigned_t<intmax_t>, uintmax_t>);

    static_assert(is_same_v<make_unsigned_t<int8_t>, uint8_t>);
    static_assert(is_same_v<make_unsigned_t<uint8_t>, uint8_t>);
    static_assert(is_same_v<make_unsigned_t<int16_t>, uint16_t>);
    static_assert(is_same_v<make_unsigned_t<uint16_t>, uint16_t>);
    static_assert(is_same_v<make_unsigned_t<int32_t>, uint32_t>);
    static_assert(is_same_v<make_unsigned_t<uint32_t>, uint32_t>);
    static_assert(is_same_v<make_unsigned_t<int64_t>, uint64_t>);
    static_assert(is_same_v<make_unsigned_t<uint64_t>, uint64_t>);

    static_assert(is_unsigned_v<make_unsigned_t<wchar_t>>);
    static_assert(sizeof(make_unsigned_t<wchar_t>) == sizeof(wchar_t));
    static_assert(is_integral_v<make_unsigned_t<wchar_t>>);

    static_assert(is_unsigned_v<make_unsigned_t<ptrdiff_t>>);
    static_assert(is_integral_v<make_unsigned_t<ptrdiff_t>>);
    static_assert(sizeof(make_unsigned_t<ptrdiff_t>) == sizeof(ptrdiff_t));

    static_assert(is_unsigned_v<make_unsigned_t<size_t>>);
    static_assert(is_integral_v<make_unsigned_t<size_t>>);
    static_assert(sizeof(make_unsigned_t<size_t>) == sizeof(size_t));

    static_assert(is_same_v<make_unsigned_t<float>, float>);
    static_assert(is_same_v<make_unsigned_t<double>, double>);
    static_assert(is_same_v<make_unsigned_t<long double>, long double>);
  }

  // remove_extent
  // remove_all_extents
  {
    static_assert(is_trivial_empty_v<remove_extent<int>>);
    static_assert(is_trivial_empty_v<remove_extent<int []>>);
    static_assert(is_trivial_empty_v<remove_extent<int [3]>>);
    static_assert(is_trivial_empty_v<remove_all_extents<int>>);
    static_assert(is_trivial_empty_v<remove_all_extents<int []>>);
    static_assert(is_trivial_empty_v<remove_all_extents<int [3]>>);
    static_assert(is_trivial_empty_v<remove_all_extents<int [3][4]>>);
    static_assert(is_trivial_empty_v<remove_all_extents<int [3][4][5][6]>>);

    static_assert(is_same_v<remove_extent<int>::type, int>);
    static_assert(is_same_v<remove_extent_t<int>, int>);
    static_assert(is_same_v<remove_extent_t<const int>, const int>);
    static_assert(is_same_v<remove_extent_t<volatile int>, volatile int>);
    static_assert(is_same_v
                  <remove_extent_t<const volatile int>, const volatile int>);
    static_assert(is_same_v<remove_extent_t<int []>, int>);
    static_assert(is_same_v<remove_extent_t<const int []>, const int>);
    static_assert(is_same_v<remove_extent_t<int [][3]>, int [3]>);
    static_assert(is_same_v<remove_extent_t<int [2][3]>, int [3]>);
    static_assert(is_same_v
                  <remove_extent_t<volatile int [2][3]>, volatile int [3]>);
    static_assert(is_same_v<remove_extent_t<int [2][3][4]>, int [3][4]>);

    static_assert(is_same_v<remove_all_extents_t<int>, int>);
    static_assert(is_same_v<remove_all_extents_t<const int>, const int>);
    static_assert(is_same_v<remove_all_extents<int [][3]>::type, int>);
    static_assert(is_same_v<remove_all_extents_t<int [][3]>, int>);
    static_assert(is_same_v
                  <remove_all_extents_t<int [2][3][4]>, int>);
    static_assert(is_same_v
                  <remove_all_extents_t<const int [2][3][4]>, const int>);
    static_assert(is_same_v
                  <remove_all_extents_t<volatile int [2][3][4]>, volatile int>);
    static_assert(is_same_v
                  <remove_all_extents_t<const volatile int [2][3][4]>,
                   const volatile int>);
    static_assert(is_same_v
                  <remove_all_extents_t<const volatile int [2][3][4]>,
                   const volatile int>);
  }

  // remove_pointer
  // add_pointer
  {
    static_assert(is_trivial_empty_v<remove_pointer<int>>);
    static_assert(is_trivial_empty_v<remove_pointer<int *>>);
    static_assert(is_trivial_empty_v<remove_pointer<int &>>);
    static_assert(is_trivial_empty_v<add_pointer<int>>);
    static_assert(is_trivial_empty_v<add_pointer<int *>>);
    static_assert(is_trivial_empty_v<add_pointer<int &>>);

    static_assert(is_same_v<remove_pointer<int>::type, int>);
    static_assert(is_same_v<remove_pointer_t<int>, int>);
    static_assert(is_same_v<remove_pointer_t<const int *>, const int>);
    static_assert(is_same_v<remove_pointer_t<const int *const>, const int>);
    static_assert(is_same_v<remove_pointer_t<const int *volatile>, const int>);
    static_assert(is_same_v
                  <remove_pointer_t<const int *const volatile>, const int>);
    static_assert(is_same_v<remove_pointer_t<int ***>, int **>);

    static_assert(is_same_v<add_pointer_t<void>, void *>);
    static_assert(is_same_v<add_pointer_t<const void>, const void *>);
    static_assert(is_same_v<add_pointer_t<int>, int *>);
    static_assert(is_same_v<add_pointer_t<void () &&>, void () &&>);
    static_assert(is_same_v<add_pointer_t<void (...)>, void (*)(...)>);
    static_assert(is_same_v
                  <add_pointer_t<void (int, ...)>, void (*)(int, ...)>);
    static_assert(is_same_v<add_pointer_t<void ()>, void (*)()>);
    static_assert(is_same_v<add_pointer_t<int &>, int *>);
    static_assert(is_same_v<add_pointer_t<int ***>, int ****>);
  }

  // type_identity
  {
    static_assert(is_trivial_empty_v<type_identity<int>>); 

    static_assert(is_same_v<type_identity<int>::type, int>);
    static_assert(is_same_v<type_identity_t<int>, int>);
  }
  // remove_cvref
  {
    static_assert(is_trivial_empty_v<remove_cvref<const int>>);
    static_assert(is_trivial_empty_v<remove_cvref<const volatile int>>);
    static_assert(is_trivial_empty_v<remove_cvref<const volatile int &>>);

    static_assert(is_same_v<remove_cvref_t<const volatile int &&>, int>);
    static_assert(is_same_v<remove_cvref_t<volatile int &&>, int>);
    static_assert(is_same_v<remove_cvref_t<const int &&>, int>);
    static_assert(is_same_v<remove_cvref_t<int &&>, int>);
    static_assert(is_same_v<remove_cvref_t<const volatile int>, int>);
    static_assert(is_same_v<remove_cvref_t<volatile int>, int>);
    static_assert(is_same_v<remove_cvref_t<const int>, int>);
    static_assert(is_same_v<remove_cvref_t<int>, int>);
    static_assert(is_same_v<remove_cvref_t<void>, void>);
    static_assert(is_same_v<remove_cvref_t<int (&)()>, int ()>);
    static_assert(is_same_v<remove_cvref_t<int (&&)()>, int ()>);
  }
  // decay
  {
    static_assert(is_trivial_empty_v<decay<const int &>>);
    static_assert(is_trivial_empty_v<decay<int [3]>>);
    static_assert(is_trivial_empty_v<decay<int (&)()>>);

    static_assert(is_same_v<decay_t<int>, int>);
    static_assert(is_same_v<decay_t<const int>, int>);
    static_assert(is_same_v<decay_t<volatile int>, int>);
    static_assert(is_same_v<decay_t<const volatile int>, int>);
    static_assert(is_same_v<decay_t<const int &>, int>);
    static_assert(is_same_v<decay_t<void (&)()>, void (*)()>);
    static_assert(is_same_v<decay_t<void () &>, void () &>);
    static_assert(is_same_v<decay_t<int []>, int *>);
    static_assert(is_same_v<decay_t<int [2][3][4]>, int (*)[3][4]>);
    static_assert(is_same_v<decay_t<int (&)[2][3][4]>, int (*)[3][4]>);
    static_assert(is_same_v
                  <decay_t<const int (&)[2][3][4]>, const int (*)[3][4]>);
  }
  // enable_if
  {
    static_assert(is_trivial_empty_v<enable_if<true>>);
    static_assert(is_trivial_empty_v<enable_if<true, int>>);
    static_assert(is_trivial_empty_v<enable_if<false, int>>);

    static_assert(has_member_type_type_v<enable_if<true>>);
    static_assert(is_same_v<enable_if<true, int>::type, int>);
    static_assert(is_same_v<enable_if_t<true, int>, int>);
    static_assert(is_same_v<enable_if_t<true>, void>);
    static_assert(!has_member_type_type_v<enable_if<false>>);
  }
  // conditional
  {
    static_assert(is_trivial_empty_v<conditional<true, int, float>>);
    static_assert(is_trivial_empty_v<conditional<false, int, float>>);

    static_assert(is_same_v<conditional<true, int, float>::type, int>);
    static_assert(is_same_v<conditional_t<true, int, float>, int>);
    static_assert(is_same_v<conditional_t<false, int, float>, float>);
  }
  // common_type
  {
    static_assert(is_trivial_empty_v<common_type<>>);
    static_assert(is_trivial_empty_v<common_type<int, int>>);
    static_assert(is_trivial_empty_v<common_type<int &, int &>>);
    static_assert(is_trivial_empty_v<common_type<int, int, int>>);
    static_assert(is_trivial_empty_v<common_type<int, int &, int>>);
    static_assert(is_trivial_empty_v<common_type<int &, int &, int &>>);

    static_assert(!has_member_type_type_v<common_type<>>);

    static_assert(is_same_v<common_type<char, char>::type, char>);
    static_assert(is_same_v<common_type_t<char, char>, char>);
    static_assert(is_same_v<common_type_t<short, char>, int>);
    static_assert(is_same_v<common_type_t<char, unsigned>, unsigned>);
    static_assert(is_same_v<common_type_t<float, double>, double>);
    static_assert(is_same_v<common_type_t<float, int>, float>);
    static_assert(is_same_v<common_type_t<int, float>, float>);
    static_assert(is_same_v<common_type_t<int &, float>, float>);
    static_assert(is_same_v<common_type_t<int &, const float &>, float>);
    static_assert(is_same_v<common_type_t<int &, int &>, int>);
    static_assert(is_same_v<common_type_t<void *, int *>, void *>);

    static_assert(!has_member_type_type_v<common_type<void *, int>>);
    static_assert(!has_member_type_type_v<common_type<void *, char>>);
    static_assert(is_same_v<common_type_t<void *, void **>, void *>);

    static_assert(is_same_v<common_type_t<void, void>, void>);
    static_assert(is_same_v<common_type_t<const void, volatile void>, void>);

    static_assert(is_same_v<common_type_t<char, short, long>, long>);
    static_assert(is_same_v
                  <common_type_t<const char, short &, volatile long &&>, long>);
    static_assert(is_same_v<common_type_t<char, short, unsigned short>, int>);
    static_assert(is_same_v<common_type_t<float, char, short>, float>);
    static_assert(is_same_v<common_type_t<float, char, double, short>, double>);
    static_assert(is_same_v<common_type_t<bool, bool, bool>, bool>);
    static_assert(is_same_v<common_type_t<bool, bool, bool, bool>, bool>);
    static_assert(is_same_v<common_type_t<bool, bool, bool, bool, bool>, bool>);
    static_assert(!has_member_type_type_v
                  <common_type_t<bool, bool, bool, bool, bool>>);

    {
      static_assert(is_void_v
                    <common_type_t<test_common_type::t, test_common_type::t>>);
      static_assert(is_void_v
                    <common_type_t<test_common_type::t, test_common_type::tt>>);
      static_assert(is_void_v
                    <common_type_t<const volatile test_common_type::t,
                     test_common_type::tt>>);
      static_assert(is_void_v
                    <common_type_t<test_common_type::t,
                     const volatile test_common_type::tt>>);
      static_assert(is_void_v
                    <common_type_t<test_common_type::tt, test_common_type::t>>);
    }

    {
      struct t {};
      struct tt : t {};
      struct ttt : tt {};
      static_assert(is_same_v<common_type_t<t, tt>, t>);
      static_assert(is_same_v<common_type_t<t, tt, ttt>, t>);
      static_assert(is_same_v<common_type_t<t, ttt>, t>);
      static_assert(is_same_v<common_type_t<t *, const tt *>, const t *>);
      static_assert(is_same_v<common_type_t<volatile t *, const tt *>,
                              const volatile t *>);
      static_assert(is_same_v<common_type_t<volatile t, const tt>, t>);
    }
  }
  // basic_common_reference
  // commmon_reference
  {
    // inner::cref_t<X>
    {
      static_assert(is_same_v<inner::cref_t<int>, const int &>);
      static_assert(is_same_v
                    <inner::cref_t<volatile int>, const volatile int &>);
      static_assert(is_same_v
                    <inner::cref_t<const volatile int>, const volatile int &>);
      static_assert(is_same_v<inner::cref_t<const volatile int &&>,
                              const volatile int &>);
    }

    // inner::xref<X>
    {
      static_assert(is_same_v<inner::xref<const volatile int &&>::tmpl<float>,
                              const volatile float &&>);
      static_assert(is_same_v<inner::xref<const volatile int &>::tmpl<float>,
                              const volatile float &>);
      static_assert(is_same_v<inner::xref<const volatile int>::tmpl<float>,
                              const volatile float>);
      static_assert(is_same_v<inner::xref<volatile int>::tmpl<float>,
                              volatile float>);
      static_assert(is_same_v<inner::xref<const int>::tmpl<float>,
                              const float>);
      static_assert(is_same_v<inner::xref<int>::tmpl<float>, float>);
    }

    // inner::copycv
    {
      static_assert(is_same_v<inner::copycv_t<const volatile int &&, float>,
                              float>);
      static_assert(is_same_v<inner::copycv_t<const volatile int &, float>,
                              float>);

      static_assert(is_same_v<inner::copycv_t<const volatile int, float>,
                              const volatile float>);
      static_assert(is_same_v<inner::copycv_t<volatile int, float>,
                              volatile float>);
      static_assert(is_same_v<inner::copycv_t<const int, float>,
                              const float>);

      static_assert(is_same_v<inner::copycv_t<const volatile int, const float>,
                              const volatile float>);
      static_assert(is_same_v<inner::copycv_t<volatile int, const float>,
                              const volatile float>);
      static_assert(is_same_v<inner::copycv_t<const int, const float>,
                              const float>);

      static_assert(is_same_v
                    <inner::copycv_t<const volatile int, volatile float>,
                     const volatile float>);
      static_assert(is_same_v<inner::copycv_t<volatile int, volatile float>,
                              volatile float>);
      static_assert(is_same_v<inner::copycv_t<const int, volatile float>,
                              const volatile float>);

      static_assert(is_same_v<inner::copycv_t<const int, volatile float &>,
                              volatile float &>);
    }

    // inner::copycvref
    {
      static_assert(is_same_v
                    <inner::copycvref_t<const volatile int &&, float &>,
                     float &>);
      static_assert(is_same_v
                    <inner::copycvref_t<const volatile int &&, float &&>,
                     float &&>);
      static_assert(is_same_v<inner::copycvref_t<const int &&, volatile float>,
                              const volatile float &&>);

      static_assert(is_same_v<inner::copycvref_t<const volatile int &&, float>,
                              const volatile float &&>);
      static_assert(is_same_v<inner::copycvref_t<volatile int &&, float>,
                              volatile float &&>);
      static_assert(is_same_v<inner::copycvref_t<const int &&, float>,
                              const float &&>);
      static_assert(is_same_v<inner::copycvref_t<int &&, float>,
                              float &&>);

      static_assert(is_same_v<inner::copycvref_t<const volatile int &, float>,
                              const volatile float &>);
      static_assert(is_same_v<inner::copycvref_t<volatile int &, float>,
                              volatile float &>);
      static_assert(is_same_v<inner::copycvref_t<const int &, float>,
                              const float &>);
      static_assert(is_same_v<inner::copycvref_t<int &, float>,
                              float &>);

      static_assert(is_same_v<inner::copycvref_t<const volatile int, float>,
                              const volatile float>);
      static_assert(is_same_v<inner::copycvref_t<volatile int, float>,
                              volatile float>);
      static_assert(is_same_v<inner::copycvref_t<const int, float>,
                              const float>);
      static_assert(is_same_v<inner::copycvref_t<int, float>, float>);
    }

    // inner::common_ref
    // common_reference
    {
      static_assert(is_trivial_empty_v
                    <basic_common_reference
                     <int, int, type_identity_t, type_identity_t>>);
      static_assert(is_trivial_empty_v<common_reference<>>);
      static_assert(is_trivial_empty_v<common_reference<int>>);
      static_assert(is_trivial_empty_v<common_reference<int, int>>);
      static_assert(is_trivial_empty_v<common_reference<int, int>>);
      static_assert(is_trivial_empty_v<common_reference<int, int &>>);
      static_assert(is_trivial_empty_v<common_reference<int &, int>>);
      static_assert(is_trivial_empty_v<common_reference<int &, int &&>>);
      static_assert(is_trivial_empty_v<common_reference<int &&, int &>>);
      static_assert(is_trivial_empty_v<common_reference<int &, int &>>);
      static_assert(is_trivial_empty_v<common_reference<int &&, int &&>>);
      static_assert(is_trivial_empty_v
                    <common_reference<tuple<int, int>, tuple<int &, int &>>>);
      static_assert(is_trivial_empty_v
                    <common_reference
                     <tuple<int, int> &, tuple<int &, int &> &,
                      tuple<int &, int &> &>>);

      test_common_reference
        ::case12_common_ref_or_user_defined_common_reference();
      test_common_reference::case34_conditional_result_or_common_type();
      test_common_reference::other_than_two_arguments();
    }
  }
  // underlying_type
  {
    enum t {a, b, c};
    enum class tt {a, b, c};

    static_assert(is_trivial_empty_v<underlying_type<void>>);
    static_assert(is_trivial_empty_v<underlying_type<t>>);
    static_assert(is_trivial_empty_v<underlying_type<const t>>);
    static_assert(is_trivial_empty_v<underlying_type<volatile t>>);
    static_assert(is_trivial_empty_v<underlying_type<const volatile t>>);
    static_assert(is_trivial_empty_v<underlying_type<tt>>);

    static_assert(is_integral_v<underlying_type<t>::type>);
    static_assert(is_integral_v<underlying_type_t<t>>);
    static_assert(is_integral_v<underlying_type_t<const t>>);
    static_assert(is_integral_v<underlying_type_t<volatile t>>);
    static_assert(is_integral_v<underlying_type_t<const volatile t>>);
    static_assert(is_integral_v<underlying_type_t<tt>>);
    static_assert(!has_member_type_type_v<underlying_type<void>>);
    static_assert(!has_member_type_type_v<underlying_type<int>>);
  }
  // void_t
  {
    static_assert(is_void_v<void_t<>>);
    static_assert(is_void_v<void_t<int, float, void *>>);
  }

  // uses_allocator
  {
    struct A {
      using allocator_type = int;
    };
    struct B {};

    static_assert(is_trivial_empty_v<uses_allocator<A, int>>);
    static_assert(is_trivial_empty_v<uses_allocator<int, int>>);

    static_assert(uses_allocator<A, int>::value);
    static_assert(uses_allocator_v<A, int>);
    static_assert(uses_allocator_v<A, float>);
    static_assert(!uses_allocator_v<A, int *>);
    static_assert(!uses_allocator_v<B, int>);
    static_assert(!uses_allocator_v<void, void>);
  }

  // max_align_of_types
  // max_size_of_types
  {
    {
      static_assert(is_trivial_empty_v<max_align_of_types<>>);
      static_assert(is_trivial_empty_v<max_align_of_types<int, float, double>>);

      static_assert(max_align_of_types<>::value == 1);
      static_assert(max_align_of_types_v<> == 1);

      static_assert(max_align_of_types_v<char> == 1);

      static_assert(max_align_of_types_v<char, int, long double>
                    == alignof(long double));
      static_assert(max_align_of_types_v<char, long double, int>
                    == alignof(long double));
      static_assert(max_align_of_types_v<int, long double, char>
                    == alignof(long double));
      static_assert(max_align_of_types_v<int, char, long double>
                    == alignof(long double));
      static_assert(max_align_of_types_v<long double, char, int>
                    == alignof(long double));
      static_assert(max_align_of_types_v<long double, int, char>
                    == alignof(long double));
    }
    {
      static_assert(is_trivial_empty_v<max_size_of_types<>>);
      static_assert(is_trivial_empty_v<max_size_of_types<int, float, double>>);

      static_assert(max_size_of_types<>::value == 0);
      static_assert(max_size_of_types_v<> == 0);

      static_assert(max_size_of_types_v<char> == 1);

      static_assert(max_size_of_types_v<char, int, long double>
                    == sizeof(long double));
      static_assert(max_size_of_types_v<char, long double, int>
                    == sizeof(long double));
      static_assert(max_size_of_types_v<int, long double, char>
                    == sizeof(long double));
      static_assert(max_size_of_types_v<int, char, long double>
                    == sizeof(long double));
      static_assert(max_size_of_types_v<long double, char, int>
                    == sizeof(long double));
      static_assert(max_size_of_types_v<long double, int, char>
                    == sizeof(long double));
    }
  }
  // type_t
  {
    static_assert(is_void_v<type_t<void>>);
    static_assert(is_void_v<type_t<void, void *>>);
    static_assert(is_void_v<type_t<void, int, float, void *>>);
    static_assert(is_same_v<type_t<int>, int>);
    static_assert(is_same_v<type_t<int, float>, int>);
    static_assert(is_same_v<type_t<int, int, float, void *>, int>);
  }
  // is_implicitly_constructible
  // is_implicitly_default_constructible
  {
    struct A {
      explicit A() {}
    };
    struct B {
      B() {}
    };
    static_assert(is_trivial_empty_v<is_implicitly_constructible<A>>);
    static_assert(is_trivial_empty_v<is_implicitly_constructible<B>>);
    static_assert(!is_implicitly_constructible<A>::value);
    static_assert(!is_implicitly_constructible_v<A>);
    static_assert(is_implicitly_constructible<B>::value);
    static_assert(is_implicitly_constructible_v<B>);
    static_assert(!is_implicitly_constructible_v<tuple<A>>);
    static_assert(!is_implicitly_constructible_v<tuple<A, A>>);
    static_assert(!is_implicitly_constructible_v<tuple<B, A>>);
    static_assert(!is_implicitly_constructible_v<tuple<A, B>>);
    static_assert(is_implicitly_constructible_v<tuple<B, B>>);
    static_assert(!is_implicitly_constructible_v<tuple<A, A, A>>);
    static_assert(!is_implicitly_constructible_v<tuple<A, B, A>>);
    static_assert(!is_implicitly_constructible_v<tuple<B, B, A>>);
    static_assert(is_implicitly_constructible_v<tuple<B, B, B>>);

    struct t {
      t(int, int) {}
    };
    struct tt {
      explicit tt(int, int) {}
    };
    static_assert(is_implicitly_constructible_v<t, int, int>);
    static_assert(!is_implicitly_constructible_v<tt, int, int>);
  }
  // is_trivial_empty
  {
    struct t {};
    static_assert(is_trivial_v<is_trivial_empty<t>>);
    static_assert(is_empty_v<is_trivial_empty<t>>);
    static_assert(is_trivial_empty_v<t>);

    struct tt {
      int x;
    };
    static_assert(is_trivial_v<tt>);
    static_assert(!is_empty_v<tt>);
    static_assert(!is_trivial_empty_v<tt>);

    struct ttt {
      ttt() {}
    };
    static_assert(!is_trivial_v<ttt>);
    static_assert(is_empty_v<ttt>);
    static_assert(!is_trivial_empty_v<ttt>);

    struct tttt {
      int x;
      tttt() {}
    };
    static_assert(!is_trivial_v<tttt>);
    static_assert(!is_empty_v<tttt>);
    static_assert(!is_trivial_empty_v<tttt>);
  }

  // copy_const
  // copy_volatile
  // copy_lvalue_reference
  // copy_rvalue_reference
  // copy_reference
  // copy_cv
  // copy_cvref
  {
    static_assert(same_as<copy_const_t<const char, int>, const int>);
    static_assert(same_as<copy_const_t<const char, int *>, int *const>);
    static_assert(same_as<copy_const_t<const char, int &>, int &>);

    static_assert(same_as<copy_volatile_t<volatile char, int>, volatile int>);
    static_assert(same_as<copy_volatile_t<volatile char, int *>,
                          int *volatile>);
    static_assert(same_as<copy_volatile_t<volatile char, int &>, int &>);

    static_assert(same_as<copy_cv_t<char, int>, int>);
    static_assert(same_as<copy_cv_t<const char, int>, const int>);
    static_assert(same_as<copy_cv_t<volatile char, int>, volatile int>);
    static_assert(same_as<copy_cv_t<const volatile char, int>,
                          const volatile int>);
    static_assert(same_as<copy_cv_t<char *, int>, int>);
    static_assert(same_as<copy_cv_t<const char *, int>, int>);
    static_assert(same_as<copy_cv_t<volatile char *, int>, int>);
    static_assert(same_as<copy_cv_t<const volatile char *, int>, int>);
    static_assert(same_as<copy_cv_t<char &, int>, int>);
    static_assert(same_as<copy_cv_t<const char &, int>, int>);
    static_assert(same_as<copy_cv_t<volatile char &, int>, int>);
    static_assert(same_as<copy_cv_t<const volatile char &, int>, int>);

    static_assert(same_as<copy_lvalue_reference_t<char, int>, int>);
    static_assert(same_as<copy_lvalue_reference_t<char, int *>, int *>);
    static_assert(same_as<copy_lvalue_reference_t<char, int &>, int &>);
    static_assert(same_as<copy_lvalue_reference_t<char, int &&>, int &&>);
    static_assert(same_as<copy_lvalue_reference_t<char &, int>, int &>);
    static_assert(same_as<copy_lvalue_reference_t<char &, int *>, int *&>);
    static_assert(same_as<copy_lvalue_reference_t<char &, int &>, int &>);
    static_assert(same_as<copy_lvalue_reference_t<char &, int &&>, int &>);
    static_assert(same_as<copy_lvalue_reference_t<char &&, int>, int>);
    static_assert(same_as<copy_lvalue_reference_t<char &&, int *>, int *>);
    static_assert(same_as<copy_lvalue_reference_t<char &&, int &>, int &>);
    static_assert(same_as<copy_lvalue_reference_t<char &&, int &&>, int &&>);

    static_assert(same_as<copy_rvalue_reference_t<char, int>, int>);
    static_assert(same_as<copy_rvalue_reference_t<char, int *>, int *>);
    static_assert(same_as<copy_rvalue_reference_t<char, int &>, int &>);
    static_assert(same_as<copy_rvalue_reference_t<char, int &&>, int &&>);
    static_assert(same_as<copy_rvalue_reference_t<char &, int>, int>);
    static_assert(same_as<copy_rvalue_reference_t<char &, int *>, int *>);
    static_assert(same_as<copy_rvalue_reference_t<char &, int &>, int &>);
    static_assert(same_as<copy_rvalue_reference_t<char &, int &&>, int &&>);
    static_assert(same_as<copy_rvalue_reference_t<char &&, int>, int &&>);
    static_assert(same_as<copy_rvalue_reference_t<char &&, int *>, int *&&>);
    static_assert(same_as<copy_rvalue_reference_t<char &&, int &>, int &>);
    static_assert(same_as<copy_rvalue_reference_t<char &&, int &&>, int &&>);

    static_assert(same_as<copy_reference_t<char, int>, int>);
    static_assert(same_as<copy_reference_t<char, int *>, int *>);
    static_assert(same_as<copy_reference_t<char, int &>, int &>);
    static_assert(same_as<copy_reference_t<char, int &&>, int &&>);
    static_assert(same_as<copy_reference_t<char &, int>, int &>);
    static_assert(same_as<copy_reference_t<char &, int *>, int *&>);
    static_assert(same_as<copy_reference_t<char &, int &>, int &>);
    static_assert(same_as<copy_reference_t<char &, int &&>, int &>);
    static_assert(same_as<copy_reference_t<char &&, int>, int &&>);
    static_assert(same_as<copy_reference_t<char &&, int *>, int *&&>);
    static_assert(same_as<copy_reference_t<char &&, int &>, int &>);
    static_assert(same_as<copy_reference_t<char &&, int &&>, int &&>);

    static_assert(same_as<copy_cvref_t<char, int>, int>);
    static_assert(same_as<copy_cvref_t<const char, int>, const int>);
    static_assert(same_as<copy_cvref_t<volatile char, int>, volatile int>);
    static_assert(same_as<copy_cvref_t<const volatile char, int>,
                          const volatile int>);
    static_assert(same_as<copy_cvref_t<char &, int>, int &>);
    static_assert(same_as<copy_cvref_t<const char &, int>, const int &>);
    static_assert(same_as<copy_cvref_t<volatile char &, int>, volatile int &>);
    static_assert(same_as<copy_cvref_t<const volatile char &, int>,
                          const volatile int &>);
    static_assert(same_as<copy_cvref_t<char &&, int>, int &&>);
    static_assert(same_as<copy_cvref_t<const char &&, int>, const int &&>);
    static_assert(same_as<copy_cvref_t<volatile char &&, int>,
                          volatile int &&>);
    static_assert(same_as<copy_cvref_t<const volatile char &&, int>,
                          const volatile int &&>);
  }

  // is_nothrow_movable
  // is_nothrow_copyable
  {
    static_assert(is_nothrow_movable_v<int>);
    static_assert(is_nothrow_copyable_v<int>);
    {
      struct t {
        t() = default;
        ~t() = default;
        t(t &&) = delete;
        t &operator =(t &&) = delete;
        t(const t &) = delete;
        t &operator =(const t &) = delete;
      };
      static_assert(!is_nothrow_movable_v<t>);
      static_assert(!is_nothrow_copyable_v<t>);
    }
    {
      struct t {
        t() = default;
        ~t() = default;
        t(t &&) {}
        t &operator =(t &&) {return *this;}
        t(const t &) = delete;
        t &operator =(const t &) = delete;
      };
      static_assert(!is_nothrow_movable_v<t>);
      static_assert(!is_nothrow_copyable_v<t>);
    }
    {
      struct t {};
      static_assert(is_nothrow_movable_v<t>);
      static_assert(is_nothrow_copyable_v<t>);
    }
  }
}

namespace help_addressof {

struct t {
  int value;
};
void operator &(t &) {}

struct tt {
  int value;
  void operator &() {}
};

struct ttt {
  int value;
private:
  void operator &() & {}
};

struct tttt {
  int value;
};
tttt *operator &(tttt &x) {
  return nullptr;
}

void f() {}

}
void test_utility_functions() {
  // forward
  // move
  // move_if_noexcept
  {
    static_assert(is_trivial_empty_v<decltype(forward<int>)>);
    static_assert(is_trivial_empty_v<decltype(move)>);
    static_assert(is_trivial_empty_v<decltype(move_if_noexcept)>);

    struct fn_t {
      int operator ()(const int &) const {return 1;}
      int operator ()(int &&) const {return 2;}
    } f;
    int x;
    int &&y = (int &&)x;

    assert(f(forward<int>(0)) == 2);
    assert(f(forward<int &>(x)) == 1);
    assert(f(forward<int &&>(y)) == 2);

    assert(f(move(x)) == 2);
    assert(f(move((int &)x)) == 2);
    assert(f(move((const int &)x)) == 1);

    assert(f(move_if_noexcept(x)) == 2);
    assert(f(move_if_noexcept((int &)x)) == 2);
    assert(f(move_if_noexcept((const int &)x)) == 1);

    struct t2 {
      int x = 0;
      t2() {}
      t2(const t2 &) : x(1) {}
    } x2{};
    struct t3 {
      int x = 0;
      t3() {}
      t3(const t3 &) : x(1) {}
    } x3{};
    struct t4 {
      int x = 0;
      t4() {}
      t4(const t4 &) noexcept : x(1) {}
    } x4{};
    struct t5 {
      int x = 0;
      t5() {}
      t5(const t5 &) : x(1) {}
      t5(t5 &&) : x(2) {}
    } x5{};
    struct t6 {
      int x = 0;
      t6() {}
      t6(const t6 &) : x(1) {}
      t6(t6 &&) noexcept : x(2) {}
    } x6{};
    struct t7 {
      int x = 0;
      t7() {}
      t7(const t7 &) noexcept : x(1) {}
      t7(t7 &&) noexcept : x(2) {}
    } x7{};
    struct t8 {
      int x = 0;
      t8() {}
      t8(const t8 &) noexcept : x(1) {}
      t8(t8 &&) : x(2) {}
    } x8{};
    struct t9 {
      int x = 0;
      t9() {}
      t9(const t9 &) = delete;
      t9(t9 &&) : x(2) {}
    } x9{};
    struct ta {
      int x = 0;
      ta() {}
      ta(const ta &) = delete;
      ta(ta &&) noexcept : x(2) {}
    } xa{};

    assert(t2{move_if_noexcept(x2)}.x == 1);
    assert(t3{move_if_noexcept(x3)}.x == 1);
    assert(t4{move_if_noexcept(x4)}.x == 1);
    assert(t5{move_if_noexcept(x5)}.x == 1);
    assert(t6{move_if_noexcept(x6)}.x == 2);
    assert(t7{move_if_noexcept(x7)}.x == 2);
    assert(t8{move_if_noexcept(x8)}.x == 1);
    assert(t9{move_if_noexcept(x9)}.x == 2);
    assert(ta{move_if_noexcept(xa)}.x == 2);

    // noexcept
    {
      int i{};
      struct t {
        t() {}
        t(const t &) {}
      };
      t xx{};
      static_assert(noexcept(forward<int>(i)));
      static_assert(noexcept(forward<int &>(i)));
      static_assert(noexcept(move(i)));
      static_assert(noexcept(move_if_noexcept(i)));
      static_assert(same_as<decltype(move_if_noexcept(xx)), const t &>);
      static_assert(noexcept(move_if_noexcept(xx)));
    }

    // constexpr
    {
      constexpr auto ff = []() constexpr->int {
        return forward<int>(1);
      };
      static_assert(ff() == 1);
    }
    {
      constexpr auto ff = []() constexpr->int {
        int x = 1;
        return move(x);
      };
      static_assert(ff() == 1);
    }
    {
      constexpr auto ff = []() constexpr->int {
        int x = 1;
        return move_if_noexcept(x);
      };
      static_assert(ff() == 1);
    }
    {
      struct t {
        int value = 1;

        t() = default;
        ~t() = default;
        t &operator =(const t &) = delete;
        t &operator =(t &&) = delete;
        t(const t &) = delete;
        constexpr t(t &&) : t() {}
      };
      constexpr auto ff = []() constexpr->t {
        t x{};
        return move_if_noexcept(x);
      };
      static_assert(ff().value == 1);
    }
  }
  // exchange
  {
    static_assert(is_trivial_empty_v<decltype(exchange)>);

    int a = 1, b = 2, c = 3;
    a = exchange(b, c);
    assert(a == 2 && b == 3 && c == 3);

    // noexcept
    {
      struct t {};
      struct tt {
        tt() {}
        tt(tt &&) {}
      };
      t x{};
      tt xx{};
      static_assert(noexcept(exchange(x, x)));
      static_assert(!noexcept(exchange(xx, xx)));
    }

    // constexpr
    {
      constexpr auto f = []() constexpr->int {
        int x = 1, y = 2;
        return exchange(x, y);
      };
      static_assert(f() == 1);
    }
  }
  // as_const
  {
    static_assert(is_trivial_empty_v<decltype(as_const)>);

    struct t {
      void f() const {}
      void f() = delete;
    };
    t x;
    as_const(x).f();

    // noexcept
    {
      int xx{};
      static_assert(noexcept(as_const(xx)));
    }

    // constexpr
    {
      constexpr auto f = []() constexpr->int {
        int x = 1;
        return as_const(x) + as_const(x);
      };
      static_assert(f() == 2);
    }
  }
  // cmp_(not_)equal
  // cmp_less(_equal)
  // cmp_greater(_equal)
  // in_range
  {
    static_assert(is_trivial_empty_v<decltype(cmp_equal)>);
    static_assert(is_trivial_empty_v<decltype(cmp_not_equal)>);
    static_assert(is_trivial_empty_v<decltype(cmp_less)>);
    static_assert(is_trivial_empty_v<decltype(cmp_less_equal)>);
    static_assert(is_trivial_empty_v<decltype(cmp_greater)>);
    static_assert(is_trivial_empty_v<decltype(cmp_greater_equal)>);
    static_assert(is_trivial_empty_v<decltype(in_range<int>)>);

    static_assert(noexcept(cmp_equal(1, 1)));
    static_assert(noexcept(cmp_not_equal(1, 1)));
    static_assert(noexcept(cmp_less(1, 1)));
    static_assert(noexcept(cmp_greater(1, 1)));
    static_assert(noexcept(cmp_less_equal(1, 1)));
    static_assert(noexcept(cmp_greater_equal(1, 1)));

    static_assert(cmp_equal(1, 1));
    static_assert(!cmp_equal(1, 2));
    static_assert(cmp_equal(1, 1UL));
    static_assert(cmp_equal(1UL, 1));
    // static_assert(-1 == numeric_limits<unsigned long long>::max());
    // static_assert(numeric_limits<unsigned long long>::max() == -1);
    static_assert(!cmp_equal(-1, numeric_limits<unsigned long long>::max()));
    static_assert(!cmp_equal(numeric_limits<unsigned long long>::max(), -1));

    static_assert(cmp_not_equal(1, 2));
    static_assert(!cmp_not_equal(1, 1));

    static_assert(cmp_less(1, 2));
    static_assert(!cmp_less(1, 1));
    static_assert(!cmp_less(1, 0));
    static_assert(cmp_less(1, 2ul));
    static_assert(!cmp_less(1, 1ul));
    static_assert(cmp_less(-1, 1ul));
    static_assert(cmp_less(-1, 0ul));
    static_assert(cmp_less(-1, static_cast<unsigned char>(0)));
    static_assert(cmp_less(1ul, 2));
    static_assert(!cmp_less(1ul, 1));
    static_assert(!cmp_less(1ul, -1));
    static_assert(!cmp_less(0ul, -1));

    static_assert(cmp_greater(2, 1));
    static_assert(!cmp_greater(1, 1));
    static_assert(!cmp_greater(0, 1));

    static_assert(cmp_greater_equal(2, 1));
    static_assert(cmp_greater_equal(1, 1));
    static_assert(!cmp_greater_equal(0, 1));

    static_assert(cmp_less_equal(1, 2));
    static_assert(cmp_less_equal(1, 1));
    static_assert(!cmp_less_equal(1, 0));

    static_assert(!in_range<unsigned char>(-1));
    static_assert(in_range<unsigned char>(0));
    static_assert(in_range<unsigned char>(128));
    static_assert(in_range<unsigned char>(255));
    static_assert(!in_range<unsigned char>(256));

    static_assert(!in_range<signed char>(128));
    static_assert(in_range<signed char>(127));
    static_assert(in_range<signed char>(10));
    static_assert(in_range<signed char>(-128));
    static_assert(!in_range<signed char>(-129));
  }
  // to_underlying(enum)
  {
    static_assert(is_trivial_empty_v<decltype(to_underlying)>);

    enum class t : char {x, y, z};
    static_assert(is_same_v<decltype(to_underlying(t::x)), char>);
    assert(to_underlying(t::x) == 0);
    static_assert(noexcept(to_underlying(t::x)));
  }
  // addressof
  {
    static_assert(is_trivial_empty_v<decltype(addressof)>);

    int a{};
    const volatile int b{};
    assert(addressof(a) == &a);
    assert(addressof(b) == &b);
    static_assert(noexcept(addressof(a)));
    static_assert(noexcept(addressof(b)));

    static_assert(inner::addressof_invocable<int>);
    static_assert(!inner::addressof_invocable<void>);
    static_assert(inner::addressof_invocable<help_addressof::t>);
    static_assert(inner::addressof_invocable<help_addressof::tt>);
    static_assert(!inner::addressof_invocable<help_addressof::ttt>);

    static_assert(!inner::addressof_overloaded<int>);
    static_assert(!inner::addressof_overloaded<void>);
    static_assert(inner::addressof_overloaded<help_addressof::t>);
    static_assert(inner::addressof_overloaded<help_addressof::tt>);
    static_assert(!inner::addressof_overloaded<help_addressof::ttt>);

    help_addressof::t x{1};
    help_addressof::tt xx{2};
    help_addressof::ttt xxx{3};
    help_addressof::tttt xxxx{4};
    assert(addressof(x)->value == 1);
    assert(addressof(xx)->value == 2);
    assert(addressof(xxx)->value == 3);
    assert(addressof(xxxx)->value == 4);
    static_assert(noexcept(addressof(x)));
    static_assert(noexcept(addressof(xx)));
    static_assert(noexcept(addressof(xxx)));
    static_assert(noexcept(addressof(xxxx)));

    assert(addressof(help_addressof::f) == &help_addressof::f);

    // constexpr
    constexpr auto f = []() constexpr->bool {
      int x{}, y{};
      return addressof(x) != addressof(y);
    };
    static_assert(f());
  }

  // align
  {
    unsigned char *const p0{};
    struct input_t {
      void *p;
      size_t n;

      bool operator ==(const input_t &x) const {return p == x.p && n == x.n;}
    };
    static_assert(regular<input_t>);
    {
      input_t i{};
      static_assert(noexcept(align(0, 0, i.p, i.n)));
    }
    {
      {
        input_t i{};
        assert(align(1, 1, i.p, i.n) == nullptr);
        assert(i == input_t{});
      }
      {
        input_t i0{p0 + 3, 1}, i = i0;
        assert(align(1, 1, i.p, i.n) == i.p);
        assert(i0 == i);
      }
      {
        input_t i0{p0 + 3, 1}, i = i0;
        assert(align(2, 1, i.p, i.n) == nullptr);
        assert(i0 == i);
      }
      {
        // 0 1 2 3 4 5
        input_t i0{p0 + 3, 2}, i = i0;
        assert(align(2, 1, i.p, i.n) == p0 + 4);
        assert(i.p == p0 + 4);
        assert(i.n == 1);
      }
    }
    {
      {
        // 0 1 2 3 4 {5 6 7 8 9}
        input_t i(p0 + 5, 5);
        assert(align(4, 2, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 {5 6 7 8 9}
        input_t i(p0 + 5, 5);
        assert(align(4, 3, i.p, i.n) == nullptr);
        assert(i == input_t(p0 + 5, 5));
      }
      {
        // 0 1 2 3 4 {5 6 7 [8 9]}
        input_t i(p0 + 5, 5);
        assert(align(4, 1, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 {5 6 7 [8 9]}
        input_t i(p0 + 5, 5);
        assert(align(4, 0, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }

      {
        // 0 1 2 3 4 5 6 7 {8 9}
        input_t i(p0 + 8, 2);
        assert(align(2, 1, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 5 6 7 {8 9}
        input_t i(p0 + 8, 2);
        assert(align(2, 0, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 5 6 7 {8 9}
        input_t i(p0 + 8, 2);
        assert(align(2, 2, i.p, i.n) == p0 + 8);
        assert(i == input_t(p0 + 8, 2));
      }
      {
        // 0 1 2 3 4 5 6 7 {8 9}
        input_t i(p0 + 8, 2);
        assert(align(2, 3, i.p, i.n) == nullptr);
        assert(i == input_t(p0 + 8, 2));
      }
    }
  }
}

void test_pointer_traits() {
  static_assert(is_same_v
                <pointer_traits<test_pointer_1<int>>::element_type, void>);
  static_assert(is_same_v
                <pointer_traits<test_pointer_2<int>>::element_type, int>);
  static_assert(is_same_v
                <pointer_traits<test_pointer_3<int>>::difference_type,
                 ptrdiff_t>);
  static_assert(is_same_v
                <pointer_traits<test_pointer_4<int>>::difference_type,
                 void>);
  static_assert(is_same_v
                <pointer_traits<test_pointer_2<int>>::rebind<void>,
                 test_pointer_2<void>>);
  static_assert(is_same_v
                <pointer_traits<test_pointer_5<int>>::rebind<int>, int>);

  int i = 0;
  pointer_traits<test_pointer_6<void>>::pointer_to(i);
  assert(i == 1);
  i = 0;
  pointer_traits<test_pointer_6<int>>::pointer_to(i);
  assert(i == 1);

  assert(pointer_traits<int *>::pointer_to(i) == &i);
  static_assert(is_same_v<pointer_traits<int *>::element_type, int>);
  static_assert(is_same_v<pointer_traits<const int *>::element_type,
                          const int>);
  static_assert(is_same_v<pointer_traits<int *>::difference_type,
                          ptrdiff_t>);
  static_assert(is_same_v<pointer_traits<int *>::pointer, int *>);
  static_assert(is_same_v<pointer_traits<int *>::rebind<char>, char *>);

  // pointer_element_t
  // pointer_difference_t
  // pointer_rebind_t
  {
    static_assert(same_as<pointer_element_t<int *>, int>);
    static_assert(same_as<pointer_element_t<const int *>, const int>);
    static_assert(same_as<pointer_difference_t<int *>, ptrdiff_t>);
    static_assert(same_as<pointer_rebind_t<int *, const long>, const long *>);
  }

  // pointer_to
  {
    struct t {};
    struct tt : t {};
    tt x;
    auto p = pointer_to<t *>(x);
    static_assert(same_as<decltype(p), tt *>);
    assert(p == addressof(x));
  }

  // to_address
  {
    static_assert(is_trivial_empty_v<decltype(to_address)>);

    struct x {
      int *operator ->() const {return nullptr;}
    };
    struct y {
      x operator ->() const {return x{};}
    };
    struct z {
      y operator ->() const {return y{};}
    };

    int *p = nullptr;
    p = to_address(p);
    assert(p == nullptr);
    p = to_address(x{});
    assert(p == nullptr);
    p = to_address(y{});
    assert(p == nullptr);
    p = to_address(z{});
    assert(p == nullptr);

    test_pointer_8<int> p8(3);
    assert(to_address(p8) == 6);
    static_assert(noexcept(to_address(p8)));
    int xxx{};
    test_pointer_9<int> p9(addressof(xxx));
    assert(to_address(p9) - addressof(xxx) == 4);
    static_assert(noexcept(to_address(p9)));
    static_assert(can_apply_to_address<int *>);
    static_assert(can_apply_to_address<const int *>);
    static_assert(can_apply_to_address<volatile int *>);
    static_assert(can_apply_to_address<const volatile int *>);
    static_assert(can_apply_to_address<test_pointer_8<int>>);
    static_assert(can_apply_to_address<test_pointer_9<int>>);
    static_assert(!can_apply_to_address<test_pointer_7<int>>);
    static_assert(!can_apply_to_address<int>);
    static_assert(!can_apply_to_address<void>);
    static_assert(can_apply_to_address<int *&>);
    static_assert(can_apply_to_address<int *const &>);
    static_assert(can_apply_to_address<int *&&>);
    static_assert(can_apply_to_address<int *const &&>);

    // noexcept
    {
      int x = 1;
      static_assert(noexcept(to_address(addressof(x))));
    }

    // constexpr
    constexpr auto f = []() constexpr {
      int x = 1;
      return *to_address(addressof(x));
    };
    static_assert(f() == 1);
  }
}

void test_reference_wrapper_and_invoke() {
  // reference_wrapper
  {
    // is_reference_wrapper
    {
      using ref_t = reference_wrapper<int>;
      static_assert(is_reference_wrapper<ref_t>::value);
      static_assert(is_reference_wrapper_v<reference_wrapper<int>>);
      static_assert(is_reference_wrapper_v<const reference_wrapper<int>>);
      static_assert(is_reference_wrapper_v<volatile reference_wrapper<int>>);
      static_assert(is_reference_wrapper_v
                    <const volatile reference_wrapper<int>>);
    }

    // unwrap_reference
    {
      using t = unwrap_reference_t<reference_wrapper<int>>;
      using tt = unwrap_reference_t<const reference_wrapper<int>>;
      using ttt = unwrap_reference_t<reference_wrapper<int> &>;
      using tttt = unwrap_reference_t<const volatile int>;
      static_assert(is_same_v<t, int &>);
      static_assert(is_same_v<tt, const reference_wrapper<int>>);
      static_assert(is_same_v<ttt, reference_wrapper<int> &>);
      static_assert(is_same_v<tttt, const volatile int>);
    }
    // unwrap_ref_decay
    {
      using t = unwrap_ref_decay_t<const volatile reference_wrapper<int [3]> &>;
      using t2 = unwrap_ref_decay_t<const reference_wrapper<int [3]> &>;
      using t3 = unwrap_ref_decay_t<volatile reference_wrapper<int [3]> &>;
      using t4 = unwrap_ref_decay_t<const volatile reference_wrapper<int [3]>>;
      using t5 = unwrap_ref_decay_t<reference_wrapper<int [3]>>;
      static_assert(is_same_v<t, int (&)[3]>);
      static_assert(is_same_v<t2, int (&)[3]>);
      static_assert(is_same_v<t3, int (&)[3]>);
      static_assert(is_same_v<t4, int (&)[3]>);
      static_assert(is_same_v<t5, int (&)[3]>);
    }

    // special member functions
    {
      using ref_t = reference_wrapper<int>;
      static_assert(!is_default_constructible_v<ref_t>);
      static_assert(is_nothrow_destructible_v<ref_t>);
      static_assert(is_nothrow_copy_constructible_v<ref_t>);
      static_assert(is_nothrow_copy_assignable_v<ref_t>);
      static_assert(is_nothrow_move_constructible_v<ref_t>);
      static_assert(is_nothrow_move_assignable_v<ref_t>);
      static_assert(is_nothrow_swappable_v<ref_t>);
    }
    // construct from marched reference
    {
      static_assert(f_is_well_formed_v<inner::refw_check_u<int>, int &>);
      static_assert(!f_is_well_formed_v<inner::refw_check_u<int>, const int &>);
      static_assert(!f_is_well_formed_v<inner::refw_check_u<int>, int &&>);
      static_assert(!f_is_well_formed_v
                    <inner::refw_check_u<int>, const int &&>);
      static_assert(f_is_well_formed_v<inner::refw_check_u<const int>, int &>);
      static_assert(f_is_well_formed_v
                    <inner::refw_check_u<const int>, const int &>);
      static_assert(!f_is_well_formed_v
                    <inner::refw_check_u<const int>, int &&>);
      static_assert(!f_is_well_formed_v
                    <inner::refw_check_u<const int>, const int &&>);
      static_assert(!is_convertible_v<int &&, reference_wrapper<int>>);
      static_assert(!is_convertible_v<const int &&, reference_wrapper<int>>);
      static_assert(!is_convertible_v<const int &, reference_wrapper<int>>);
      static_assert(is_convertible_v<int &, reference_wrapper<int>>);
      static_assert(is_nothrow_convertible_v<int &, reference_wrapper<int>>);
      static_assert(!is_convertible_v<int &&, reference_wrapper<const int>>);
      static_assert(!is_convertible_v
                    <const int &&, reference_wrapper<const int>>);
      static_assert(is_convertible_v
                    <const int &, reference_wrapper<const int>>);
      static_assert(is_nothrow_convertible_v
                    <const int &, reference_wrapper<const int>>);
      static_assert(is_convertible_v<int &, reference_wrapper<const int>>);
    }
    // operator T &
    // get
    // operator ()
    {
      constexpr auto f = []() constexpr->int {
        int i = 3;
        reference_wrapper<int> r = i;
        return r.get();
      };
      static_assert(f() == 3);

      constexpr auto ff = []() constexpr->int {
        int i = 2;
        reference_wrapper<int> r = i;
        int &x = r;
        return x;
      };
      static_assert(ff() == 2);

      constexpr auto fff = []() constexpr->bool {
        less<> l;
        reference_wrapper<less<>> r = l;
        return r(1, 2);
      };
      static_assert(fff() == true);

      using ref_t = reference_wrapper<int>;
      int i = 5;
      ref_t r(i);
      assert(addressof(r.get()) == addressof(i));

      static_assert(noexcept(static_cast<int &>
                             (declval<reference_wrapper<int>>())));
      static_assert(is_nothrow_convertible_v<reference_wrapper<int>, int &>);
      static_assert(same_as
                    <decltype(declval<reference_wrapper<int>>().get()), int &>);
      static_assert(noexcept(declval<reference_wrapper<int>>().get()));
    }
    // deduction guide
    {
      constexpr auto f = []() constexpr->int {
        const int x = 1;
        reference_wrapper r(x);
        static_assert(same_as<decltype(r), reference_wrapper<const int>>);
        return r.get();
      };
      static_assert(f() == 1);
    }

    // ref
    // cref
    {
      constexpr auto f = []() constexpr->int {
        int x = 1;
        auto r = ref(x);
        auto rr = ref(r);
        static_assert(is_same_v<decltype(r), reference_wrapper<int>>);
        static_assert(is_same_v<decltype(rr), decltype(r)>);
        return rr.get();
      };
      static_assert(f() == 1);

      constexpr auto ff = []() constexpr->int {
        int x = 1;
        const int y = 2;
        auto r = cref(x);
        auto rr = cref(y);
        static_assert(is_same_v<decltype(r), reference_wrapper<const int>>);
        static_assert(is_same_v<decltype(rr), reference_wrapper<const int>>);
        return r.get() * 10 + rr.get();
      };
      static_assert(ff() == 12);
    }

    {
      using ref_t = reference_wrapper<int>;
      int i = 5;
      ref_t r(i);
      assert(addressof(r.get()) == addressof(i));
    }

    // noexcept
    {
      int x{};
      reference_wrapper r(x);
      static_assert(noexcept(reference_wrapper<int>(x)));
      static_assert(noexcept(static_cast<int &>(r)));
      static_assert(noexcept(r.get()));

      const auto f0 = []() noexcept {return 0;};
      const auto f = []() {return 0;};
      static_assert(!noexcept(reference_wrapper(f0)()));
      static_assert(!noexcept(reference_wrapper(f)()));
    }
  }

  // is_invocable
  // is_nothrow_invocable
  // invoke_result
  // invoke
  //
  // is_invocable_r
  // is_nothrow_invocable_r
  // invoke_r
  {
    {
      const auto f = []() {};
      static_assert(is_trivial_empty_v<is_invocable<decltype(f)>>);
      static_assert(is_trivial_empty_v<is_nothrow_invocable<decltype(f)>>);
      static_assert(is_trivial_empty_v<invoke_result<decltype(f)>>);
      static_assert(is_trivial_empty_v<decltype(invoke)>);
      static_assert(is_trivial_empty_v<is_invocable_r<void, decltype(f)>>);
      static_assert(is_trivial_empty_v
                    <is_nothrow_invocable_r<void, decltype(f)>>);
      static_assert(is_trivial_empty_v<decltype(invoke_r<int>)>);
    }

    // invoke(f, x, s...)
    // case 1: (x.*f)(s...)
    // case 2: (x.get().*f)(s...)
    // case 3: ((*x).*f)(s...)
    {
      struct t {
        constexpr int f() {return 1;}
        int ff() const noexcept {return 2;}
      };
      struct tt : t {};
      struct ttt : private t {};

      struct fp {
        using type = int (t::*)();
        operator type() const {return &t::f;}
      };
      static_assert(!is_invocable_v<fp, t &>);

      struct xt {
        int x;

        xt() = delete;
        ~xt() = default;
        xt(const xt &) = default;
        xt &operator =(const xt &) = default;
        xt(xt &&) = default;
        xt &operator =(xt &&) = default;

        xt(const int x) : x(x * 2) {}

        operator int() const {return x;}
      };

      // is_invocable
      {
        static_assert(is_invocable_v<decltype(&t::f), t &>);
        static_assert(is_invocable_v<decltype(&t::f), t &&>);
        static_assert(!is_invocable_v<decltype(&t::f), const t &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<t> &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<t> &&>);
        static_assert(is_invocable_v
                      <decltype(&t::f), const reference_wrapper<t> &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_invocable_v<decltype(&t::f), t *>);
        static_assert(!is_invocable_v<decltype(&t::f), const t *>);

        static_assert(is_invocable_v<decltype(&t::f), tt &>);
        static_assert(is_invocable_v<decltype(&t::f), tt &&>);
        static_assert(!is_invocable_v<decltype(&t::f), const tt &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<tt> &>);
        static_assert(is_invocable_v
                      <decltype(&t::f), reference_wrapper<tt> &&>);
        static_assert(is_invocable_v
                      <decltype(&t::f), const reference_wrapper<tt> &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<const tt>>);
        static_assert(is_invocable_v<decltype(&t::f), tt *>);
        static_assert(!is_invocable_v<decltype(&t::f), const tt *>);

        static_assert(!is_invocable_v<decltype(&t::f), ttt &>);
        static_assert(!is_invocable_v<decltype(&t::f), ttt &&>);
        static_assert(!is_invocable_v<decltype(&t::f), const ttt &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<ttt> &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<ttt> &&>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<const ttt>>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), const reference_wrapper<ttt> &>);
        static_assert(!is_invocable_v<decltype(&t::f), ttt *>);
        static_assert(!is_invocable_v<decltype(&t::f), const ttt *>);
      }
      // is_invocable_r
      {
        static_assert(is_invocable_r_v<void, decltype(&t::f), t &>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), t &>);
        static_assert(is_invocable_r_v<xt, decltype(&t::f), t &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), t &>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), tt &>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), tt &>);
        static_assert(is_invocable_r_v<xt, decltype(&t::f), tt &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), tt &>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), ttt &>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), ttt &>);
        static_assert(!is_invocable_r_v<xt, decltype(&t::f), ttt &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), ttt &>);

        static_assert(is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<t>>);
        static_assert(is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<t>>);
        static_assert(is_invocable_r_v
                      <xt, decltype(&t::f), reference_wrapper<t>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<t>>);
        static_assert(!is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(!is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(!is_invocable_r_v
                      <xt, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(is_invocable_r_v
                      <xt, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(!is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<ttt>>);
        static_assert(!is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<ttt>>);
        static_assert(!is_invocable_r_v
                      <xt, decltype(&t::f), reference_wrapper<ttt>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<ttt>>);

        static_assert(is_invocable_r_v<void, decltype(&t::f), t *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), const t *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), t *>);
        static_assert(is_invocable_r_v<xt, decltype(&t::f), t *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), const t *>);
        static_assert(!is_invocable_r_v<xt, decltype(&t::f), const t *>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), tt *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), const tt *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), tt *>);
        static_assert(is_invocable_r_v<xt, decltype(&t::f), tt *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), const tt *>);
        static_assert(!is_invocable_r_v<xt, decltype(&t::f), const tt *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), ttt *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), const ttt *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), ttt *>);
        static_assert(!is_invocable_r_v<xt, decltype(&t::f), ttt *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), const ttt *>);
        static_assert(!is_invocable_r_v<xt, decltype(&t::f), const ttt *>);
      }
      // is_nothrow_invocable
      {
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), t &>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), const t &>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), t &&>);
        static_assert(!is_nothrow_invocable_v
                      <decltype(&t::f), reference_wrapper<t>>);
        static_assert(!is_nothrow_invocable_v
                      <decltype(&t::f), reference_wrapper<const t>>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), t *>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), tt *>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), ttt *>);

        static_assert(is_nothrow_invocable_v<decltype(&t::ff), t &>);
        static_assert(is_nothrow_invocable_v<decltype(&t::ff), tt &>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::ff), ttt &>);
        static_assert(is_nothrow_invocable_v
                      <decltype(&t::ff), reference_wrapper<t>>);
        static_assert(is_nothrow_invocable_v
                      <decltype(&t::ff), reference_wrapper<tt>>);
        static_assert(!is_nothrow_invocable_v
                      <decltype(&t::ff), reference_wrapper<ttt>>);
        static_assert(is_nothrow_invocable_v<decltype(&t::ff), t *>);
        static_assert(is_nothrow_invocable_v<decltype(&t::ff), tt *>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::ff), ttt *>);
      }
      // is_nothrow_invocable_r
      {
        static_assert(is_nothrow_invocable_r_v
                      <void, decltype(&t::ff), const t &>);
        static_assert(is_nothrow_invocable_r_v<void, decltype(&t::ff), t &>);
        static_assert(is_nothrow_invocable_r_v<float, decltype(&t::ff), t &>);
        static_assert(!is_nothrow_invocable_r_v<xt, decltype(&t::ff), t &>);
        static_assert(!is_nothrow_invocable_r_v<void, decltype(&t::f), t &>);
        static_assert(!is_nothrow_invocable_r_v<float, decltype(&t::f), t &>);
        static_assert(!is_nothrow_invocable_r_v<xt, decltype(&t::f), t &>);

        static_assert(is_nothrow_invocable_r_v
                      <void, decltype(&t::ff), reference_wrapper<tt>>);
        static_assert(is_nothrow_invocable_r_v
                      <float, decltype(&t::ff), reference_wrapper<tt>>);
        static_assert(!is_nothrow_invocable_r_v
                      <xt, decltype(&t::ff), reference_wrapper<tt>>);
        static_assert(!is_nothrow_invocable_r_v
                      <void, decltype(&t::ff), reference_wrapper<ttt>>);

        static_assert(is_nothrow_invocable_r_v<void, decltype(&t::ff), t *>);
        static_assert(is_nothrow_invocable_r_v<float, decltype(&t::ff), t *>);
        static_assert(!is_nothrow_invocable_r_v<void, decltype(&t::ff), ttt *>);
      }

      t xx{};
      assert(invoke(&t::f, xx) == 1);
      assert(invoke(&t::f, move(xx)) == 1);
      assert(invoke(&t::f, ref(xx)) == 1);
      assert(invoke(&t::f, addressof(xx)) == 1);
      assert(invoke_r<float>(&t::f, xx) == 1);
      assert(invoke_r<float>(&t::f, move(xx)) == 1);
      assert(invoke_r<float>(&t::f, ref(xx)) == 1);
      assert(invoke_r<float>(&t::f, addressof(xx)) == 1);
      static_assert(is_same_v<decltype(invoke_r<float>(&t::f, xx)), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, move(xx))), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, ref(xx))), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, addressof(xx))), float>);
      assert(invoke_r<xt>(&t::f, xx) == 2);
      assert(invoke_r<xt>(&t::f, move(xx)) == 2);
      assert(invoke_r<xt>(&t::f, ref(xx)) == 2);
      assert(invoke_r<xt>(&t::f, addressof(xx)) == 2);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, xx)), xt>);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, move(xx))), xt>);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, ref(xx))), xt>);
      static_assert(is_same_v
                    <decltype(invoke_r<xt>(&t::f, addressof(xx))), xt>);
      invoke_r<void>(&t::f, xx);
      invoke_r<void>(&t::f, move(xx));
      invoke_r<void>(&t::f, ref(xx));
      invoke_r<void>(&t::f, addressof(xx));
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, xx))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, move(xx)))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, ref(xx)))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, addressof(xx)))>);
      static_assert(invoke(&t::f, t{}) == 1);
      constexpr auto cf = []() constexpr->int {
        t x{};
        return invoke(&t::f, ref(x));
      };    
      static_assert(cf() == 1);
      constexpr auto cf2 = []() constexpr->int {
        t x{};
        return invoke(&t::f, addressof(x));
      };
      static_assert(cf2() == 1);
      static_assert(invoke_r<float>(&t::f, t{}) == 1.0f);
      constexpr auto cff = []() constexpr->int {
        t x{};
        return invoke_r<float>(&t::f, ref(x));
      };    
      static_assert(cff() == 1.0f);
      constexpr auto cff2 = []() constexpr->int {
        t x{};
        return invoke_r<float>(&t::f, addressof(x));
      };
      static_assert(cff2() == 1.0f);
      constexpr auto cff3 = []() constexpr->int {
        t x{};
        invoke_r<void>(&t::f, x);
        return 0;
      };
      static_assert(cff3() == 0);
    }

    // invoke(f, x)
    // case 4: x.*f
    // case 5: x.get().*f
    // case 6: (*x).*f
    //   // code is copied from case 123, with minor modification
    {
      struct t {
        int f = 1;
      };
      struct tt : t {};
      struct ttt : private t {};

      struct fp {
        operator int t::*() const {return &t::f;}
      };
      static_assert(!is_invocable_v<fp, t &>);

      struct xt {
        int x;

        xt() = delete;
        ~xt() = default;
        xt(const xt &) = default;
        xt &operator =(const xt &) = default;
        xt(xt &&) = default;
        xt &operator =(xt &&) = default;

        xt(const int x) : x(x * 2) {}

        operator int() const {return x;}
      };

      // is_invocable
      {
        static_assert(is_invocable_v<decltype(&t::f), t &>);
        static_assert(is_invocable_v<decltype(&t::f), t &&>);
        static_assert(is_invocable_v<decltype(&t::f), const t &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<t> &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<t> &&>);
        static_assert(is_invocable_v
                      <decltype(&t::f), const reference_wrapper<t> &>);
        static_assert(is_invocable_v
                      <decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_invocable_v<decltype(&t::f), t *>);
        static_assert(is_invocable_v<decltype(&t::f), const t *>);

        static_assert(is_invocable_v<decltype(&t::f), tt &>);
        static_assert(is_invocable_v<decltype(&t::f), tt &&>);
        static_assert(is_invocable_v<decltype(&t::f), const tt &>);
        static_assert(is_invocable_v<decltype(&t::f), reference_wrapper<tt> &>);
        static_assert(is_invocable_v
                      <decltype(&t::f), reference_wrapper<tt> &&>);
        static_assert(is_invocable_v
                      <decltype(&t::f), const reference_wrapper<tt> &>);
        static_assert(is_invocable_v
                      <decltype(&t::f), reference_wrapper<const tt>>);
        static_assert(is_invocable_v<decltype(&t::f), tt *>);
        static_assert(is_invocable_v<decltype(&t::f), const tt *>);

        static_assert(!is_invocable_v<decltype(&t::f), ttt &>);
        static_assert(!is_invocable_v<decltype(&t::f), ttt &&>);
        static_assert(!is_invocable_v<decltype(&t::f), const ttt &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<ttt> &>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<ttt> &&>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), reference_wrapper<const ttt>>);
        static_assert(!is_invocable_v
                      <decltype(&t::f), const reference_wrapper<ttt> &>);
        static_assert(!is_invocable_v<decltype(&t::f), ttt *>);
        static_assert(!is_invocable_v<decltype(&t::f), const ttt *>);
      }
      // is_invocable_r
      {
        static_assert(is_invocable_r_v<void, decltype(&t::f), t &>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), t &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), t &>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), tt &>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), tt &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), tt &>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), ttt &>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), ttt &>);
        static_assert(!is_invocable_r_v<void *, decltype(&t::f), ttt &>);

        static_assert(is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<t>>);
        static_assert(is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<t>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<t>>);
        static_assert(is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(!is_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<ttt>>);
        static_assert(!is_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<ttt>>);
        static_assert(!is_invocable_r_v
                      <void *, decltype(&t::f), reference_wrapper<ttt>>);

        static_assert(is_invocable_r_v<void, decltype(&t::f), t *>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), const t *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), t *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), const t *>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), tt *>);
        static_assert(is_invocable_r_v<void, decltype(&t::f), const tt *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), tt *>);
        static_assert(is_invocable_r_v<float, decltype(&t::f), const tt *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), ttt *>);
        static_assert(!is_invocable_r_v<void, decltype(&t::f), const ttt *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), ttt *>);
        static_assert(!is_invocable_r_v<float, decltype(&t::f), const ttt *>);
      }
      // is_nothrow_invocable
      {
        static_assert(is_nothrow_invocable_v<decltype(&t::f), t &>);
        static_assert(is_nothrow_invocable_v<decltype(&t::f), const t &>);
        static_assert(is_nothrow_invocable_v<decltype(&t::f), t &&>);
        static_assert(is_nothrow_invocable_v
                      <decltype(&t::f), reference_wrapper<t>>);
        static_assert(is_nothrow_invocable_v
                      <decltype(&t::f), reference_wrapper<const t>>);
        static_assert(is_nothrow_invocable_v<decltype(&t::f), t *>);
        static_assert(is_nothrow_invocable_v<decltype(&t::f), tt *>);
        static_assert(!is_nothrow_invocable_v<decltype(&t::f), ttt *>);
      }
      // is_nothrow_invocable_r
      {
        static_assert(is_nothrow_invocable_r_v
                      <void, decltype(&t::f), const t &>);
        static_assert(is_nothrow_invocable_r_v<void, decltype(&t::f), t &>);
        static_assert(is_nothrow_invocable_r_v<float, decltype(&t::f), t &>);
        static_assert(!is_nothrow_invocable_r_v<void *, decltype(&t::f), t &>);

        static_assert(is_nothrow_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(is_nothrow_invocable_r_v
                      <float, decltype(&t::f), reference_wrapper<tt>>);
        static_assert(!is_nothrow_invocable_r_v
                      <void, decltype(&t::f), reference_wrapper<ttt>>);

        static_assert(is_nothrow_invocable_r_v<void, decltype(&t::f), t *>);
        static_assert(is_nothrow_invocable_r_v<float, decltype(&t::f), t *>);
        static_assert(!is_nothrow_invocable_r_v<void, decltype(&t::f), ttt *>);
      }

      t xx{};
      assert(invoke(&t::f, xx) == 1);
      assert(invoke(&t::f, move(xx)) == 1);
      assert(invoke(&t::f, ref(xx)) == 1);
      assert(invoke(&t::f, addressof(xx)) == 1);
      assert(invoke_r<float>(&t::f, xx) == 1);
      assert(invoke_r<float>(&t::f, move(xx)) == 1);
      assert(invoke_r<float>(&t::f, ref(xx)) == 1);
      assert(invoke_r<float>(&t::f, addressof(xx)) == 1);
      static_assert(is_same_v<decltype(invoke_r<float>(&t::f, xx)), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, move(xx))), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, ref(xx))), float>);
      static_assert(is_same_v
                    <decltype(invoke_r<float>(&t::f, addressof(xx))), float>);
      assert(invoke_r<xt>(&t::f, xx) == 2);
      assert(invoke_r<xt>(&t::f, move(xx)) == 2);
      assert(invoke_r<xt>(&t::f, ref(xx)) == 2);
      assert(invoke_r<xt>(&t::f, addressof(xx)) == 2);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, xx)), xt>);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, move(xx))), xt>);
      static_assert(is_same_v<decltype(invoke_r<xt>(&t::f, ref(xx))), xt>);
      static_assert(is_same_v
                    <decltype(invoke_r<xt>(&t::f, addressof(xx))), xt>);
      invoke_r<void>(&t::f, xx);
      invoke_r<void>(&t::f, move(xx));
      invoke_r<void>(&t::f, ref(xx));
      invoke_r<void>(&t::f, addressof(xx));
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, xx))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, move(xx)))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, ref(xx)))>);
      static_assert(is_void_v<decltype(invoke_r<void>(&t::f, addressof(xx)))>);
      static_assert(invoke(&t::f, t{}) == 1);
      constexpr auto cf = []() constexpr->int {
        t x{};
        return invoke(&t::f, ref(x));
      };    
      static_assert(cf() == 1);
      constexpr auto cf2 = []() constexpr->int {
        t x{};
        return invoke(&t::f, addressof(x));
      };
      static_assert(cf2() == 1);
      static_assert(invoke_r<float>(&t::f, t{}) == 1.0f);
      constexpr auto cff = []() constexpr->int {
        t x{};
        return invoke_r<float>(&t::f, ref(x));
      };    
      static_assert(cff() == 1.0f);
      constexpr auto cff2 = []() constexpr->int {
        t x{};
        return invoke_r<float>(&t::f, addressof(x));
      };
      static_assert(cff2() == 1.0f);
      constexpr auto cff3 = []() constexpr->int {
        t x{};
        invoke_r<void>(&t::f, x);
        return 0;
      };
      static_assert(cff3() == 0);
    }

    // invoke(f, s...)
    // case 0: f(s...)
    {
      struct xt {
        int x;

        xt() = delete;
        ~xt() = default;
        xt(const xt &) = default;
        xt &operator =(const xt &) = default;
        xt(xt &&) = default;
        xt &operator =(xt &&) = default;

        xt(const int x) : x(x * 2) {}

        operator int() const {return x;}
      };

      const auto f = []() {return 1;};
      assert(f() == 1);
      assert(invoke(f) == 1);
      static_assert(is_invocable_v<decltype(f)>);
      static_assert(!is_nothrow_invocable_v<decltype(f)>);
      static_assert(same_as<invoke_result_t<decltype(f)>, int>);
      assert(invoke_r<float>(f) == 1);
      assert(invoke_r<xt>(f) == 2);
      static_assert(is_invocable_r_v<float, decltype(f)>);
      static_assert(is_invocable_r_v<xt, decltype(f)>);
      static_assert(!is_nothrow_invocable_r_v<float, decltype(f)>);
      static_assert(!is_nothrow_invocable_r_v<xt, decltype(f)>);

      const auto ff = []() noexcept {return 1;};
      assert(ff() == 1);
      assert(invoke(ff) == 1);
      static_assert(is_invocable_v<decltype(ff)>);
      static_assert(is_nothrow_invocable_v<decltype(ff)>);
      static_assert(same_as<invoke_result_t<decltype(ff)>, int>);
      assert(invoke_r<float>(ff) == 1);
      assert(invoke_r<xt>(ff) == 2);
      static_assert(is_invocable_r_v<float, decltype(ff)>);
      static_assert(is_invocable_r_v<xt, decltype(ff)>);
      static_assert(is_nothrow_invocable_r_v<float, decltype(ff)>);
      static_assert(!is_nothrow_invocable_r_v<xt, decltype(ff)>);

      const auto fff = []() constexpr {return 1;};
      static_assert(invoke(fff) == 1);

      const auto ffff = []<class...S>(S &&...) {
        return is_same_v
          <type_pack<S &&...>, type_pack<int &, int &&, const int &>>;
      };
      int x{};
      assert(invoke(ffff, x, move(x), as_const(x)));

      const auto acc_f = [](int &&x, int &&y, int &&z)->int {
        return x * 100 + y * 10 + z;
      };
      assert(invoke(acc_f, 1, 2, 3) == 123);

      struct t {
        int value;

        int f(int &&x, int &&y, int &&z) {
          return value * 1000 + x * 100 + y * 10 + z;
        }
        int ff(int &&x, int &&y, int &&z) noexcept {
          return value * 1000 + x * 100 + y * 10 + z;
        }
      };
      t y{4};
      assert(invoke(&t::f, y, 3, 2, 1) == 4321);
      assert(invoke(&t::f, ref(y), 3, 2, 1) == 4321);
      assert(invoke(&t::f, addressof(y), 3, 2, 1) == 4321);
      static_assert(is_same_v<invoke_result_t
                              <decltype(&t::f), t &, int &&, int &&, int &&>,
                              int>);
      static_assert(is_same_v<invoke_result_t
                              <decltype(&t::f), reference_wrapper<t>,
                               int &&, int &&, int &&>,
                              int>);
      static_assert(is_same_v<invoke_result_t
                              <decltype(&t::f), t *, int &&, int &&, int &&>,
                              int>);
      static_assert(!is_invocable_v<decltype(&t::f), t &, int &, int &, int &>);
      static_assert(!is_invocable_v<decltype(&t::f), t &,
                                    const int &, const int &, const int &>);
      static_assert(is_invocable_v<decltype(&t::f), t &,
                                   int &&, int &&, int &&>);
      static_assert(!is_invocable_v<decltype(&t::f), reference_wrapper<t>,
                                    int &, int &, int &>);
      static_assert(!is_invocable_v<decltype(&t::f), t *, int &, int &, int &>);
      static_assert(!is_nothrow_invocable_v<decltype(&t::f),
                                            t &, int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_v
                    <decltype(&t::f), reference_wrapper<t>,
                     int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_v<decltype(&t::f), t *,
                                            int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_v<decltype(&t::f), t *,
                                            int &, int &, int &>);
      static_assert(is_nothrow_invocable_v<decltype(&t::ff),
                                           t &, int &&, int &&, int &&>);
      static_assert(is_nothrow_invocable_v
                    <decltype(&t::ff), reference_wrapper<t>,
                     int &&, int &&, int &&>);
      static_assert(is_nothrow_invocable_v<decltype(&t::ff), t *,
                                           int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_v<decltype(&t::ff), t *,
                                            int &, int &, int &>);

      static_assert(!is_invocable_r_v<void, decltype(&t::f), t &,
                                      const int &, const int &, const int &>);
      static_assert(!is_invocable_r_v<xt, decltype(&t::f), t &,
                                      const int &, const int &, const int &>);
      static_assert(!is_invocable_r_v<void *, decltype(&t::f), t &,
                                      const int &, const int &, const int &>);
      static_assert(is_invocable_r_v<void, decltype(&t::f), t &,
                                     int &&, int &&, int &&>);
      static_assert(is_invocable_r_v<xt, decltype(&t::f), t &,
                                     int &&, int &&, int &&>);
      static_assert(!is_invocable_r_v<void *, decltype(&t::f), t &,
                                      int &&, int &&, int &&>);

      static_assert(!is_nothrow_invocable_r_v<void, decltype(&t::f), t &,
                                              int &&, int &&, int &&>);
      static_assert(is_nothrow_invocable_r_v<void, decltype(&t::ff), t &,
                                             int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_r_v<void, decltype(&t::ff), t &,
                                              int &, int &, int &>);
      static_assert(!is_nothrow_invocable_r_v<void *, decltype(&t::f), t &,
                                              int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_r_v<void *, decltype(&t::ff), t &,
                                              int &&, int &&, int &&>);
      static_assert(!is_nothrow_invocable_r_v<void *, decltype(&t::ff), t &,
                                              int &, int &, int &>);
    }
  }
}

namespace help_ranges_swap {

struct t {};
void swap(const t &, const t &) {}

struct tt {};
void swap(tt &, tt &) = delete;

struct ttt {
  ttt(ttt &&) {}
  ttt &operator =(ttt &&) {return *this;}
};
void swap(ttt &, ttt &) = delete;

struct tttt {
  tttt(tttt &&) noexcept {}
  tttt &operator =(tttt &&) noexcept {return *this;}
};
void swap(tttt &, tttt &) = delete;

struct t5 {
  int value;
};
void swap(t5 &, t5 &) noexcept {}

struct t6 {
  int value;
};
template <size_t N>
void swap(t6 (&x)[N], t6 (&y)[N]) {}

}
void test_language_related_concepts() {
  // same_as
  {
    static_assert(!same_as<void, const void>);
    static_assert(same_as<void, void>);
    static_assert(same_as<int [], int []>);
    static_assert(same_as<int, int>);
    static_assert(!same_as<int, int &>);
  }

  // derived_from
  {
    struct t {};
    struct tt : t {};
    struct ttt : private t {};

    static_assert(derived_from<tt, t>);
    static_assert(!derived_from<ttt, t>);

    static_assert(!derived_from<int, int>);

    static_assert(derived_from<t, const t>);
    static_assert(derived_from<t, volatile t>);
    static_assert(derived_from<t, const volatile t>);
    static_assert(derived_from<const t, t>);
    static_assert(derived_from<volatile t, t>);
    static_assert(derived_from<const t, t>);

    static_assert(derived_from<tt, const t>);
    static_assert(derived_from<tt, volatile t>);
    static_assert(derived_from<tt, const volatile t>);
    static_assert(derived_from<const tt, t>);
    static_assert(derived_from<volatile tt, t>);
    static_assert(derived_from<const tt, t>);

    static_assert(!derived_from<ttt, const t>);
    static_assert(!derived_from<ttt, volatile t>);
    static_assert(!derived_from<ttt, const volatile t>);
    static_assert(!derived_from<const ttt, t>);
    static_assert(!derived_from<volatile ttt, t>);
    static_assert(!derived_from<const ttt, t>);
  }

  // convertible_to
  {
    static_assert(convertible_to<int, double>);
    static_assert(convertible_to<int *, void *>);
    static_assert(!convertible_to<void *, int *>);
  }

  // common_reference_with
  {
    static_assert(!has_member_type_type_v<common_reference<int, char *>>);
    static_assert(!common_reference_with<int, char *>);
    struct t {};
    struct tt : t {};
    struct ttt : private t {};
    static_assert(common_reference_with<t, t>);
    static_assert(common_reference_with<t &, t &>);
    static_assert(common_reference_with<t, tt>);
    static_assert(common_reference_with<t &, tt &>);
    static_assert(!common_reference_with<t, ttt>);
    static_assert(!common_reference_with<t &, ttt &>);
  }

  // common_with
  {
    static_assert(common_with<int, int>);
  }

  // integral
  // signed_integral
  // unsigned_integral
  // floating_point
  {
    static_assert(integral<int>);
    static_assert(signed_integral<int>);
    static_assert(unsigned_integral<unsigned>);
    static_assert(floating_point<float>);
  }

  // assignable_from
  {
    static_assert(assignable_from<int &, bool>);
  }

  // swappable
  // swappable_with
  {
    {
      static_assert(swappable<int>);
      static_assert(swappable<int &>);
      static_assert(swappable<int &&>);
      static_assert(!swappable<const int>);
      static_assert(!swappable<const int &>);
      static_assert(!swappable<const int &&>);
      static_assert(swappable_with<int &, int &>);
      static_assert(!swappable_with<int, int>);
      static_assert(!swappable_with<int &, int>);
      static_assert(!swappable_with<int, int &>);
      static_assert(!swappable_with<int, int &&>);
      static_assert(!swappable_with<int &&, int>);
      static_assert(!swappable_with<int &&, int &&>);
      static_assert(!swappable_with<int &, int &&>);
      static_assert(!swappable_with<int &&, int &>);
      static_assert(!swappable_with<int &, const int &>);
      static_assert(!swappable_with<const int &, int &>);
      static_assert(!swappable_with<const int, const int>);
      static_assert(!swappable_with<const int, const int>);
      static_assert(!swappable_with<const int &, const int>);
      static_assert(!swappable_with<const int, const int &>);
      static_assert(!swappable_with<const int, const int &&>);
      static_assert(!swappable_with<const int &&, const int>);
      static_assert(!swappable_with<const int &&, const int &&>);
      static_assert(!swappable_with<const int &, const int &&>);
      static_assert(!swappable_with<const int &&, const int &>);
      int x = 1, y = 2;
      ranges::swap(x, y);
      assert(x == 2 && y == 1);
      static_assert(noexcept(ranges::swap(x, y)));
      constexpr auto f = []() constexpr {
        int x = 1, y = 2;
        ranges::swap(x, y);
        return x;
      };
      static_assert(f() == 2);
      int a[1] = {1}, aa[1] = {2};
      ranges::swap(a, aa);
      static_assert(noexcept(ranges::swap(a, aa)));
      assert(a[0] == 2 && aa[0] == 1);
      constexpr auto ff = []() constexpr {
        int a[3] = {1, 2, 3}, b[3] = {4, 5, 6};
        ranges::swap(a, b);
        return a[0] * 100 + a[1] * 10 + a[2];
      };
      static_assert(ff() == 456);

      int c[3][3][3] = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
                        {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}},
                        {{19, 20, 21}, {22, 23, 24}, {25, 26, 27}}};
      int d[3][3][3] = {};
      static_assert(swappable<int [3][3][3]>);
      static_assert(noexcept(ranges::swap(c, d)));
      ranges::swap(c, d);
      for (int i = 0; i != 3; ++i)
        for (int j = 0; j != 3; ++j)
          for (int k = 0; k != 3; ++k)
            assert(c[i][j][k] == 0);
      int kk = 1;
      for (int i = 0; i != 3; ++i)
        for (int j = 0; j != 3; ++j)
          for (int k = 0; k != 3; ++k) {
            assert(d[i][j][k] == kk);
            ++kk;
          }
    }
    {
      using namespace help_ranges_swap;

      static_assert(swappable<t>);
      static_assert(swappable_with<t &, t &>);
      static_assert(swappable_with<t &&, t &&>);
      static_assert(swappable_with<t &&, t &>);
      static_assert(swappable_with<t &, t &&>);
      static_assert(swappable_with<t &&, const t &>);
      static_assert(!noexcept(ranges::swap(t{}, t{})));

      static_assert(swappable<tt>);
      static_assert(noexcept(ranges::swap(declval<tt &>(), declval<tt &>())));
      static_assert(swappable<ttt>);
      static_assert(!noexcept
                    (ranges::swap(declval<ttt &>(), declval<ttt &>())));
      static_assert(swappable<tttt>);
      static_assert(noexcept
                    (ranges::swap(declval<tttt &>(), declval<tttt &>())));

      static_assert(swappable<t5>);
      static_assert(noexcept(ranges::swap(declval<t5 &>(), declval<t5 &>())));
      t5 x{1}, y{2};
      ranges::swap(x, y);
      assert(x.value == 1 && y.value == 2);

      static_assert(swappable<t5 [1][2][3][4]>);
      static_assert(noexcept(ranges::swap(declval<t5 (&)[1][2][3][4]>(),
                                          declval<t5 (&)[1][2][3][4]>())));

      t6 xx[2] = {1, 2};
      t6 yy[2] = {3, 4};
      static_assert(swappable_with<t6 (&)[2], t6 (&)[2]>);
      static_assert(!re_adl::inner::ranges_swap_adl_version_capable
                    <decltype(xx) &, decltype(yy) &>);
      ranges::swap(xx, yy);
      assert(xx[0].value == 3 && xx[1].value == 4);
      assert(yy[0].value == 1 && yy[1].value == 2);
      adl_swap(xx, yy);
      assert(xx[0].value == 3 && xx[1].value == 4);
      assert(yy[0].value == 1 && yy[1].value == 2);
      ranges::swap(xx[0], xx[1]);
      assert(xx[0].value == 4 && xx[1].value == 3);
      adl_swap(xx[0], xx[1]);
      assert(xx[0].value == 3 && xx[1].value == 4);
    }
    static_assert(swappable<int>);
    static_assert(swappable<int &>);
    static_assert(swappable<reference_wrapper<int>>);
    static_assert(swappable<reference_wrapper<int> &>);
    static_assert(swappable_with<int &, int &>);
  }

  // destructible
  {
    static_assert(destructible<int>);
  }

  // constructible_from
  {
    static_assert(constructible_from<int>);
    static_assert(constructible_from<int, int>);
    static_assert(constructible_from<int, float>);
  }

  // default_initializable
  {
    static_assert(default_initializable<int>);
    static_assert(!default_initializable<int &>);
    static_assert(!default_initializable<void>);
  }

  // move_constructible
  // copy_constructible
  {
    static_assert(move_constructible<int>);
    static_assert(copy_constructible<int>);
    static_assert(!move_constructible<int ()>);
    static_assert(!copy_constructible<int ()>);
  }

  // equality_comparable
  // equality_comparable_with
  {
    static_assert(equality_comparable<int>);
    static_assert(equality_comparable<int ()>);
    static_assert(equality_comparable_with<int, float>);
  }

  // totally_ordered
  // totally_ordered_with
  {
    static_assert(inner::partially_ordered_with<char *, const char *>);
    static_assert(!inner::partially_ordered_with<int *, const char *>);
    static_assert(inner::partially_ordered<int>);
    static_assert(inner::partially_ordered<int ()>);
    static_assert(inner::partially_ordered<reference_wrapper<int>>);
    static_assert(totally_ordered<int>);
    static_assert(totally_ordered<int ()>);
    static_assert(totally_ordered<void *>);
    static_assert(totally_ordered<int [2]>);
    static_assert(totally_ordered_with<int &, const int &>);
    static_assert(totally_ordered_with<const int &, int &>);
    struct t {};
    struct tt {};
    static_assert(!totally_ordered<t>);
    static_assert(!totally_ordered_with<t &, tt &>);
    static_assert(!totally_ordered_with<t &, tt &>);
  }

  // movable
  // copyable
  // semiregular
  // regular
  {
    static_assert(movable<int>);
    static_assert(copyable<int>);
    static_assert(semiregular<int>);
    static_assert(regular<int>);
  }

  // invocable
  // regular_invocable
  // predicate
  // relation
  // equivalence_relation
  // strict_weak_order
  {
    static_assert(invocable<int (*)()>);
    static_assert(invocable<int (*)(int), int>);
    static_assert(!invocable<int (*)(int &), const int &>);
    static_assert(!invocable<int (*)(int &), int &&>);
    static_assert(regular_invocable<int (*)()>);
    static_assert(regular_invocable<int (*)(int), int>);
    static_assert(predicate<bool (*)()>);
    static_assert(predicate<bool (*)(int), int>);
    static_assert(relation<bool (*)(int, int), int, int>);
    static_assert(equivalence_relation<bool (*)(int, int), int, int>);
    static_assert(strict_weak_order<bool (*)(int, int), int, int>);
  }
}

void test_concept_constrained_comparisons() {
  static_assert(is_trivial_empty_v<ranges::equal_to>);
  static_assert(is_trivial_empty_v<ranges::not_equal_to>);
  static_assert(is_trivial_empty_v<ranges::less>);
  static_assert(is_trivial_empty_v<ranges::greater>);
  static_assert(is_trivial_empty_v<ranges::less_equal>);
  static_assert(is_trivial_empty_v<ranges::greater_equal>);

  static_assert(is_transparent_function_v<ranges::equal_to>);
  static_assert(is_transparent_function_v<ranges::not_equal_to>);
  static_assert(is_transparent_function_v<ranges::less>);
  static_assert(is_transparent_function_v<ranges::greater>);
  static_assert(is_transparent_function_v<ranges::less_equal>);
  static_assert(is_transparent_function_v<ranges::greater_equal>);

  assert(ranges::equal_to{}(1, 1));
  assert(ranges::not_equal_to{}(1, 2));
  assert(ranges::less{}(1, 2));
  assert(ranges::less_equal{}(1, 2));
  assert(ranges::less_equal{}(2, 2));
  assert(ranges::greater{}(2, 1));
  assert(ranges::greater_equal{}(2, 1));
  assert(ranges::greater_equal{}(2, 2));
}

void test_integral_traits() {
  static_assert(is_trivial_empty_v<integral_traits<bool>>);
  static_assert(is_trivial_empty_v<integral_traits<char>>);
  static_assert(is_trivial_empty_v<integral_traits<unsigned char>>);
  static_assert(is_trivial_empty_v<integral_traits<signed char>>);
  static_assert(is_trivial_empty_v<integral_traits<char8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<char16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<char32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<wchar_t>>);
  static_assert(is_trivial_empty_v<integral_traits<short>>);
  static_assert(is_trivial_empty_v<integral_traits<unsigned short>>);
  static_assert(is_trivial_empty_v<integral_traits<int>>);
  static_assert(is_trivial_empty_v<integral_traits<unsigned>>);
  static_assert(is_trivial_empty_v<integral_traits<long>>);
  static_assert(is_trivial_empty_v<integral_traits<unsigned long>>);
  static_assert(is_trivial_empty_v<integral_traits<long long>>);
  static_assert(is_trivial_empty_v<integral_traits<unsigned long long>>);
  static_assert(is_trivial_empty_v<integral_traits<size_t>>);
  static_assert(is_trivial_empty_v<integral_traits<ptrdiff_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_least8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_least16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_least32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_least64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_least8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_least16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_least32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_least64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_fast8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_fast16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_fast32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int_fast64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_fast8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_fast16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_fast32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint_fast64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<int64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint8_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint16_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint32_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uint64_t>>);
  static_assert(is_trivial_empty_v<integral_traits<intmax_t>>);
  static_assert(is_trivial_empty_v<integral_traits<uintmax_t>>);
  static_assert(is_trivial_empty_v<integral_traits<void>>);

  static_assert(integral_traits<bool>::is_specialized);
  static_assert(integral_traits<char>::is_specialized);
  static_assert(integral_traits<unsigned char>::is_specialized);
  static_assert(integral_traits<signed char>::is_specialized);
  static_assert(integral_traits<char8_t>::is_specialized);
  static_assert(integral_traits<char16_t>::is_specialized);
  static_assert(integral_traits<char32_t>::is_specialized);
  static_assert(integral_traits<wchar_t>::is_specialized);
  static_assert(integral_traits<short int>::is_specialized);
  static_assert(integral_traits<unsigned short int>::is_specialized);
  static_assert(integral_traits<int>::is_specialized);
  static_assert(integral_traits<unsigned int>::is_specialized);
  static_assert(integral_traits<long int>::is_specialized);
  static_assert(integral_traits<unsigned long int>::is_specialized);
  static_assert(integral_traits<long long int>::is_specialized);
  static_assert(integral_traits<unsigned long long int>::is_specialized);
  static_assert(integral_traits<size_t>::is_specialized);
  static_assert(integral_traits<ptrdiff_t>::is_specialized);
  static_assert(integral_traits<int_least8_t>::is_specialized);
  static_assert(integral_traits<uint_least8_t>::is_specialized);
  static_assert(integral_traits<int_least16_t>::is_specialized);
  static_assert(integral_traits<uint_least16_t>::is_specialized);
  static_assert(integral_traits<int_least32_t>::is_specialized);
  static_assert(integral_traits<uint_least32_t>::is_specialized);
  static_assert(integral_traits<int_least64_t>::is_specialized);
  static_assert(integral_traits<uint_least64_t>::is_specialized);
  static_assert(integral_traits<int_fast8_t>::is_specialized);
  static_assert(integral_traits<uint_fast8_t>::is_specialized);
  static_assert(integral_traits<int_fast16_t>::is_specialized);
  static_assert(integral_traits<uint_fast16_t>::is_specialized);
  static_assert(integral_traits<int_fast32_t>::is_specialized);
  static_assert(integral_traits<uint_fast32_t>::is_specialized);
  static_assert(integral_traits<int_fast64_t>::is_specialized);
  static_assert(integral_traits<uint_fast64_t>::is_specialized);
  static_assert(integral_traits<intmax_t>::is_specialized);
  static_assert(integral_traits<uintmax_t>::is_specialized);
  static_assert(integral_traits<int8_t>::is_specialized);
  static_assert(integral_traits<uint8_t>::is_specialized);
  static_assert(integral_traits<int16_t>::is_specialized);
  static_assert(integral_traits<uint16_t>::is_specialized);
  static_assert(integral_traits<int32_t>::is_specialized);
  static_assert(integral_traits<uint32_t>::is_specialized);
  static_assert(integral_traits<int64_t>::is_specialized);
  static_assert(integral_traits<uint64_t>::is_specialized);

  static_assert(!integral_traits<nullptr_t>::is_specialized);
  static_assert(!integral_traits<void>::is_specialized);
  static_assert(!integral_traits<int &>::is_specialized);
  static_assert(!integral_traits<float>::is_specialized);
  static_assert(!integral_traits<double>::is_specialized);
  static_assert(!integral_traits<void *>::is_specialized);
  static_assert(!integral_traits<void ()>::is_specialized);
  static_assert(!integral_traits<void (&)()>::is_specialized);
  static_assert(!integral_traits<void (*)()>::is_specialized);

  static_assert(!integral_traits<bool>::is_signed);
  static_assert(integral_traits<char>::is_signed
                || !integral_traits<char>::is_signed);
  static_assert(!integral_traits<unsigned char>::is_signed);
  static_assert(integral_traits<signed char>::is_signed);
  static_assert(!integral_traits<char8_t>::is_signed);
  static_assert(!integral_traits<char16_t>::is_signed);
  static_assert(!integral_traits<char32_t>::is_signed);
  static_assert(integral_traits<wchar_t>::is_signed
                || !integral_traits<wchar_t>::is_signed);
  static_assert(integral_traits<short int>::is_signed);
  static_assert(!integral_traits<unsigned short int>::is_signed);
  static_assert(integral_traits<int>::is_signed);
  static_assert(!integral_traits<unsigned int>::is_signed);
  static_assert(integral_traits<long int>::is_signed);
  static_assert(!integral_traits<unsigned long int>::is_signed);
  static_assert(integral_traits<long long int>::is_signed);
  static_assert(!integral_traits<unsigned long long int>::is_signed);
  static_assert(!integral_traits<size_t>::is_signed);
  static_assert(integral_traits<ptrdiff_t>::is_signed);
  static_assert(integral_traits<int_least8_t>::is_signed);
  static_assert(!integral_traits<uint_least8_t>::is_signed);
  static_assert(integral_traits<int_least16_t>::is_signed);
  static_assert(!integral_traits<uint_least16_t>::is_signed);
  static_assert(integral_traits<int_least32_t>::is_signed);
  static_assert(!integral_traits<uint_least32_t>::is_signed);
  static_assert(integral_traits<int_least64_t>::is_signed);
  static_assert(!integral_traits<uint_least64_t>::is_signed);
  static_assert(integral_traits<int_fast8_t>::is_signed);
  static_assert(!integral_traits<uint_fast8_t>::is_signed);
  static_assert(integral_traits<int_fast16_t>::is_signed);
  static_assert(!integral_traits<uint_fast16_t>::is_signed);
  static_assert(integral_traits<int_fast32_t>::is_signed);
  static_assert(!integral_traits<uint_fast32_t>::is_signed);
  static_assert(integral_traits<int_fast64_t>::is_signed);
  static_assert(!integral_traits<uint_fast64_t>::is_signed);
  static_assert(integral_traits<intmax_t>::is_signed);
  static_assert(!integral_traits<uintmax_t>::is_signed);
  static_assert(integral_traits<int8_t>::is_signed);
  static_assert(!integral_traits<uint8_t>::is_signed);
  static_assert(integral_traits<int16_t>::is_signed);
  static_assert(!integral_traits<uint16_t>::is_signed);
  static_assert(integral_traits<int32_t>::is_signed);
  static_assert(!integral_traits<uint32_t>::is_signed);
  static_assert(integral_traits<int64_t>::is_signed);
  static_assert(!integral_traits<uint64_t>::is_signed);

  static_assert(integral_traits<bool>::max() == 1);
  static_assert(integral_traits<bool>::min() == 0);
  static_assert(integral_traits<char>::max() == CHAR_MAX);
  static_assert(integral_traits<char>::min() == CHAR_MIN);
  static_assert(integral_traits<unsigned char>::max() == UCHAR_MAX);
  static_assert(integral_traits<unsigned char>::min() == 0);
  static_assert(integral_traits<signed char>::max() == SCHAR_MAX);
  static_assert(integral_traits<signed char>::min() == SCHAR_MIN);
  static_assert(integral_traits<char8_t>::max() == UCHAR_MAX);
  static_assert(integral_traits<char8_t>::min() == 0);
  static_assert(integral_traits<char16_t>::max() == UINT_LEAST16_MAX);
  static_assert(integral_traits<char16_t>::min() == 0);
  static_assert(integral_traits<char32_t>::max() == UINT_LEAST32_MAX);
  static_assert(integral_traits<char32_t>::min() == 0);
  static_assert(integral_traits<wchar_t>::max() == WCHAR_MAX);
  static_assert(integral_traits<wchar_t>::min() == WCHAR_MIN);
  static_assert(integral_traits<short int>::max() == SHRT_MAX);
  static_assert(integral_traits<short int>::min() == SHRT_MIN);
  static_assert(integral_traits<unsigned short int>::max() == USHRT_MAX);
  static_assert(integral_traits<unsigned short int>::min() == 0);
  static_assert(integral_traits<int>::max() == INT_MAX);
  static_assert(integral_traits<int>::min() == INT_MIN);
  static_assert(integral_traits<unsigned int>::max() == UINT_MAX);
  static_assert(integral_traits<unsigned int>::min() == 0);
  static_assert(integral_traits<long int>::max() == LONG_MAX);
  static_assert(integral_traits<long int>::min() == LONG_MIN);
  static_assert(integral_traits<unsigned long int>::max() == ULONG_MAX);
  static_assert(integral_traits<unsigned long int>::min() == 0);
  static_assert(integral_traits<long long int>::max() == LLONG_MAX);
  static_assert(integral_traits<long long int>::min() == LLONG_MIN);
  static_assert(integral_traits<unsigned long long int>::max()
                == ULLONG_MAX);
  static_assert(integral_traits<unsigned long long int>::min() == 0);
  static_assert(integral_traits<size_t>::max() == SIZE_MAX);
  static_assert(integral_traits<size_t>::min() == 0);
  static_assert(integral_traits<ptrdiff_t>::max() == PTRDIFF_MAX);
  static_assert(integral_traits<ptrdiff_t>::min() == PTRDIFF_MIN);
  static_assert(integral_traits<int_least8_t>::max() == INT_LEAST8_MAX);
  static_assert(integral_traits<int_least8_t>::min() == INT_LEAST8_MIN);
  static_assert(integral_traits<uint_least8_t>::max() == UINT_LEAST8_MAX);
  static_assert(integral_traits<uint_least8_t>::min() == 0);
  static_assert(integral_traits<int_least16_t>::max() == INT_LEAST16_MAX);
  static_assert(integral_traits<int_least16_t>::min() == INT_LEAST16_MIN);
  static_assert(integral_traits<uint_least16_t>::max() == UINT_LEAST16_MAX);
  static_assert(integral_traits<uint_least16_t>::min() == 0);
  static_assert(integral_traits<int_least32_t>::max() == INT_LEAST32_MAX);
  static_assert(integral_traits<int_least32_t>::min() == INT_LEAST32_MIN);
  static_assert(integral_traits<uint_least32_t>::max() == UINT_LEAST32_MAX);
  static_assert(integral_traits<uint_least32_t>::min() == 0);
  static_assert(integral_traits<int_least64_t>::max() == INT_LEAST64_MAX);
  static_assert(integral_traits<int_least64_t>::min() == INT_LEAST64_MIN);
  static_assert(integral_traits<uint_least64_t>::max() == UINT_LEAST64_MAX);
  static_assert(integral_traits<uint_least64_t>::min() == 0);
  static_assert(integral_traits<int_fast8_t>::max() == INT_FAST8_MAX);
  static_assert(integral_traits<int_fast8_t>::min() == INT_FAST8_MIN);
  static_assert(integral_traits<uint_fast8_t>::max() == UINT_FAST8_MAX);
  static_assert(integral_traits<uint_fast8_t>::min() == 0);
  static_assert(integral_traits<int_fast16_t>::max() == INT_FAST16_MAX);
  static_assert(integral_traits<int_fast16_t>::min() == INT_FAST16_MIN);
  static_assert(integral_traits<uint_fast16_t>::max() == UINT_FAST16_MAX);
  static_assert(integral_traits<uint_fast16_t>::min() == 0);
  static_assert(integral_traits<int_fast32_t>::max() == INT_FAST32_MAX);
  static_assert(integral_traits<int_fast32_t>::min() == INT_FAST32_MIN);
  static_assert(integral_traits<uint_fast32_t>::max() == UINT_FAST32_MAX);
  static_assert(integral_traits<uint_fast32_t>::min() == 0);
  static_assert(integral_traits<int_fast64_t>::max() == INT_FAST64_MAX);
  static_assert(integral_traits<int_fast64_t>::min() == INT_FAST64_MIN);
  static_assert(integral_traits<uint_fast64_t>::max() == UINT_FAST64_MAX);
  static_assert(integral_traits<uint_fast64_t>::min() == 0);
  static_assert(integral_traits<intmax_t>::max() == INTMAX_MAX);
  static_assert(integral_traits<intmax_t>::min() == INTMAX_MIN);
  static_assert(integral_traits<uintmax_t>::max() == UINTMAX_MAX);
  static_assert(integral_traits<uintmax_t>::min() == 0);
  static_assert(integral_traits<int8_t>::max() == INT8_MAX);
  static_assert(integral_traits<int8_t>::min() == INT8_MIN);
  static_assert(integral_traits<int16_t>::max() == INT16_MAX);
  static_assert(integral_traits<int16_t>::min() == INT16_MIN);
  static_assert(integral_traits<int32_t>::max() == INT32_MAX);
  static_assert(integral_traits<int32_t>::min() == INT32_MIN);
  static_assert(integral_traits<int64_t>::max() == INT64_MAX);
  static_assert(integral_traits<int64_t>::min() == INT64_MIN);
  static_assert(integral_traits<uint8_t>::max() == UINT8_MAX);
  static_assert(integral_traits<uint8_t>::min() == 0);
  static_assert(integral_traits<uint16_t>::max() == UINT16_MAX);
  static_assert(integral_traits<uint16_t>::min() == 0);
  static_assert(integral_traits<uint32_t>::max() == UINT32_MAX);
  static_assert(integral_traits<uint32_t>::min() == 0);
  static_assert(integral_traits<uint64_t>::max() == UINT64_MAX);
  static_assert(integral_traits<uint64_t>::min() == 0);

  static_assert(is_same_v<integral_traits<bool>::change_signedness, bool>);
  static_assert(is_same_v
                <integral_traits<char>::change_signedness, signed char>
                || is_same_v
                <integral_traits<char>::change_signedness, unsigned char>);
  static_assert(is_same_v
                <integral_traits<unsigned char>::change_signedness,
                 signed char>);
  static_assert(is_same_v
                <integral_traits<signed char>::change_signedness,
                 unsigned char>);
  static_assert(is_same_v
                <integral_traits<char8_t>::change_signedness,
                 signed char>);
  static_assert(is_same_v
                <integral_traits<char16_t>::change_signedness,
                 int_least16_t>);
  static_assert(is_same_v
                <integral_traits<char32_t>::change_signedness,
                 int_least32_t>);
  static_assert(is_same_v
                <integral_traits<short>::change_signedness, unsigned short>);
  static_assert(is_same_v
                <integral_traits<unsigned short>::change_signedness, short>);
  static_assert(is_same_v<integral_traits<int>::change_signedness, unsigned>);
  static_assert(is_same_v<integral_traits<unsigned>::change_signedness, int>);
  static_assert(is_same_v
                <integral_traits<long>::change_signedness, unsigned long>);
  static_assert(is_same_v
                <integral_traits<unsigned long>::change_signedness, long>);
  static_assert(is_same_v
                <integral_traits<long long>::change_signedness,
                 unsigned long long>);
  static_assert(is_same_v
                <integral_traits<unsigned long long>::change_signedness,
                 long long>);
  static_assert(is_same_v
                <integral_traits<int_least8_t>::change_signedness,
                 uint_least8_t>);
  static_assert(is_same_v
                <integral_traits<uint_least8_t>::change_signedness,
                 int_least8_t>);
  static_assert(is_same_v
                <integral_traits<int_least16_t>::change_signedness,
                 uint_least16_t>);
  static_assert(is_same_v
                <integral_traits<uint_least16_t>::change_signedness,
                 int_least16_t>);
  static_assert(is_same_v
                <integral_traits<int_least32_t>::change_signedness,
                 uint_least32_t>);
  static_assert(is_same_v
                <integral_traits<uint_least32_t>::change_signedness,
                 int_least32_t>);
  static_assert(is_same_v
                <integral_traits<int_least64_t>::change_signedness,
                 uint_least64_t>);
  static_assert(is_same_v
                <integral_traits<uint_least64_t>::change_signedness,
                 int_least64_t>);
  static_assert(is_same_v
                <integral_traits<int_fast8_t>::change_signedness,
                 uint_fast8_t>);
  static_assert(is_same_v
                <integral_traits<uint_fast8_t>::change_signedness,
                 int_fast8_t>);
  static_assert(is_same_v
                <integral_traits<int_fast16_t>::change_signedness,
                 uint_fast16_t>);
  static_assert(is_same_v
                <integral_traits<uint_fast16_t>::change_signedness,
                 int_fast16_t>);
  static_assert(is_same_v
                <integral_traits<int_fast32_t>::change_signedness,
                 uint_fast32_t>);
  static_assert(is_same_v
                <integral_traits<uint_fast32_t>::change_signedness,
                 int_fast32_t>);
  static_assert(is_same_v
                <integral_traits<int_fast64_t>::change_signedness,
                 uint_fast64_t>);
  static_assert(is_same_v
                <integral_traits<uint_fast64_t>::change_signedness,
                 int_fast64_t>);
  static_assert(is_same_v
                <integral_traits<uintmax_t>::change_signedness, intmax_t>);
  static_assert(is_same_v
                <integral_traits<intmax_t>::change_signedness, uintmax_t>);
  static_assert(is_same_v
                <integral_traits<int8_t>::change_signedness, uint8_t>);
  static_assert(is_same_v
                <integral_traits<uint8_t>::change_signedness, int8_t>);
  static_assert(is_same_v
                <integral_traits<int16_t>::change_signedness, uint16_t>);
  static_assert(is_same_v
                <integral_traits<uint16_t>::change_signedness, int16_t>);
  static_assert(is_same_v
                <integral_traits<int32_t>::change_signedness, uint32_t>);
  static_assert(is_same_v
                <integral_traits<uint32_t>::change_signedness, int32_t>);
  static_assert(is_same_v
                <integral_traits<int64_t>::change_signedness, uint64_t>);
  static_assert(is_same_v
                <integral_traits<uint64_t>::change_signedness, int64_t>);

  static_assert(integral_traits
                <integral_traits<size_t>::change_signedness>::is_signed);
  static_assert(integral_traits
                <integral_traits<size_t>::change_signedness>::is_specialized);
  static_assert(sizeof(integral_traits<size_t>::change_signedness)
                == sizeof(size_t));

  static_assert(!integral_traits
                <integral_traits<ptrdiff_t>::change_signedness>::is_signed);
  static_assert(integral_traits
                <integral_traits<ptrdiff_t>::change_signedness>
                ::is_specialized);
  static_assert(sizeof(integral_traits<ptrdiff_t>::change_signedness)
                == sizeof(ptrdiff_t));

  static_assert((integral_traits
                 <integral_traits<wchar_t>::change_signedness>::is_signed
                 && !integral_traits<wchar_t>::is_signed)
                || (!integral_traits
                    <integral_traits<wchar_t>::change_signedness>::is_signed
                    && integral_traits<wchar_t>::is_signed));
  static_assert(integral_traits
                <integral_traits<wchar_t>::change_signedness>::is_specialized);
  static_assert(sizeof(integral_traits<wchar_t>::change_signedness)
                == sizeof(wchar_t));

  static_assert(integral_traits<const char>::is_specialized);
  static_assert(integral_traits<const char>::max() == CHAR_MAX);
  static_assert(integral_traits<const char>::min() == CHAR_MIN);
  static_assert(integral_traits<volatile char>::is_specialized);
  static_assert(integral_traits<volatile char>::max() == CHAR_MAX);
  static_assert(integral_traits<volatile char>::min() == CHAR_MIN);
  static_assert(integral_traits<const volatile char>::is_specialized);
  static_assert(integral_traits<const volatile char>::max() == CHAR_MAX);
  static_assert(integral_traits<const volatile char>::min() == CHAR_MIN);

  static_assert(noexcept(integral_traits<bool>::max()));
  static_assert(noexcept(integral_traits<bool>::min()));
  static_assert(noexcept(integral_traits<char>::max()));
  static_assert(noexcept(integral_traits<char>::min()));
  static_assert(noexcept(integral_traits<unsigned char>::max()));
  static_assert(noexcept(integral_traits<unsigned char>::min()));
  static_assert(noexcept(integral_traits<signed char>::max()));
  static_assert(noexcept(integral_traits<signed char>::min()));
  static_assert(noexcept(integral_traits<char8_t>::max()));
  static_assert(noexcept(integral_traits<char8_t>::min()));
  static_assert(noexcept(integral_traits<char16_t>::max()));
  static_assert(noexcept(integral_traits<char16_t>::min()));
  static_assert(noexcept(integral_traits<char32_t>::max()));
  static_assert(noexcept(integral_traits<char32_t>::min()));
  static_assert(noexcept(integral_traits<wchar_t>::max()));
  static_assert(noexcept(integral_traits<wchar_t>::min()));
  static_assert(noexcept(integral_traits<short int>::max()));
  static_assert(noexcept(integral_traits<short int>::min()));
  static_assert(noexcept(integral_traits<unsigned short int>::max()));
  static_assert(noexcept(integral_traits<unsigned short int>::min()));
  static_assert(noexcept(integral_traits<int>::max()));
  static_assert(noexcept(integral_traits<int>::min()));
  static_assert(noexcept(integral_traits<unsigned int>::max()));
  static_assert(noexcept(integral_traits<unsigned int>::min()));
  static_assert(noexcept(integral_traits<long int>::max()));
  static_assert(noexcept(integral_traits<long int>::min()));
  static_assert(noexcept(integral_traits<unsigned long int>::max()));
  static_assert(noexcept(integral_traits<unsigned long int>::min()));
  static_assert(noexcept(integral_traits<long long int>::max()));
  static_assert(noexcept(integral_traits<long long int>::min()));
  static_assert(noexcept(integral_traits<unsigned long long int>::max()));
  static_assert(noexcept(integral_traits<unsigned long long int>::min()));
  static_assert(noexcept(integral_traits<size_t>::max()));
  static_assert(noexcept(integral_traits<size_t>::min()));
  static_assert(noexcept(integral_traits<ptrdiff_t>::max()));
  static_assert(noexcept(integral_traits<ptrdiff_t>::min()));
  static_assert(noexcept(integral_traits<int_least8_t>::max()));
  static_assert(noexcept(integral_traits<int_least8_t>::min()));
  static_assert(noexcept(integral_traits<uint_least8_t>::max()));
  static_assert(noexcept(integral_traits<uint_least8_t>::min()));
  static_assert(noexcept(integral_traits<int_least16_t>::max()));
  static_assert(noexcept(integral_traits<int_least16_t>::min()));
  static_assert(noexcept(integral_traits<uint_least16_t>::max()));
  static_assert(noexcept(integral_traits<uint_least16_t>::min()));
  static_assert(noexcept(integral_traits<int_least32_t>::max()));
  static_assert(noexcept(integral_traits<int_least32_t>::min()));
  static_assert(noexcept(integral_traits<uint_least32_t>::max()));
  static_assert(noexcept(integral_traits<uint_least32_t>::min()));
  static_assert(noexcept(integral_traits<int_least64_t>::max()));
  static_assert(noexcept(integral_traits<int_least64_t>::min()));
  static_assert(noexcept(integral_traits<uint_least64_t>::max()));
  static_assert(noexcept(integral_traits<uint_least64_t>::min()));
  static_assert(noexcept(integral_traits<int_fast8_t>::max()));
  static_assert(noexcept(integral_traits<int_fast8_t>::min()));
  static_assert(noexcept(integral_traits<uint_fast8_t>::max()));
  static_assert(noexcept(integral_traits<uint_fast8_t>::min()));
  static_assert(noexcept(integral_traits<int_fast16_t>::max()));
  static_assert(noexcept(integral_traits<int_fast16_t>::min()));
  static_assert(noexcept(integral_traits<uint_fast16_t>::max()));
  static_assert(noexcept(integral_traits<uint_fast16_t>::min()));
  static_assert(noexcept(integral_traits<int_fast32_t>::max()));
  static_assert(noexcept(integral_traits<int_fast32_t>::min()));
  static_assert(noexcept(integral_traits<uint_fast32_t>::max()));
  static_assert(noexcept(integral_traits<uint_fast32_t>::min()));
  static_assert(noexcept(integral_traits<int_fast64_t>::max()));
  static_assert(noexcept(integral_traits<int_fast64_t>::min()));
  static_assert(noexcept(integral_traits<uint_fast64_t>::max()));
  static_assert(noexcept(integral_traits<uint_fast64_t>::min()));
  static_assert(noexcept(integral_traits<intmax_t>::max()));
  static_assert(noexcept(integral_traits<intmax_t>::min()));
  static_assert(noexcept(integral_traits<uintmax_t>::max()));
  static_assert(noexcept(integral_traits<uintmax_t>::min()));
  static_assert(noexcept(integral_traits<int8_t>::max()));
  static_assert(noexcept(integral_traits<int8_t>::min()));
  static_assert(noexcept(integral_traits<int16_t>::max()));
  static_assert(noexcept(integral_traits<int16_t>::min()));
  static_assert(noexcept(integral_traits<int32_t>::max()));
  static_assert(noexcept(integral_traits<int32_t>::min()));
  static_assert(noexcept(integral_traits<int64_t>::max()));
  static_assert(noexcept(integral_traits<int64_t>::min()));
  static_assert(noexcept(integral_traits<uint8_t>::max()));
  static_assert(noexcept(integral_traits<uint8_t>::min()));
  static_assert(noexcept(integral_traits<uint16_t>::max()));
  static_assert(noexcept(integral_traits<uint16_t>::min()));
  static_assert(noexcept(integral_traits<uint32_t>::max()));
  static_assert(noexcept(integral_traits<uint32_t>::min()));
  static_assert(noexcept(integral_traits<uint64_t>::max()));
  static_assert(noexcept(integral_traits<uint64_t>::min()));
}
void test_numeric_limits() {
  static_assert(numeric_limits<uint32_t>::max() == 0xffffffffu);
  static_assert(numeric_limits<uint32_t>::min() == 0u);
  static_assert(numeric_limits<uint32_t>::lowest() == 0u);
  static_assert(numeric_limits<uint32_t>::is_signed == false);
  static_assert(numeric_limits<uint32_t>::is_specialized == true);

  assert(numeric_limits<float>::max()
         == bit_cast<float>
         ((uint32_t)0b0'11111110'111'11111'11111'11111'11111u));
  assert(numeric_limits<float>::min()
         == bit_cast<float>
         ((uint32_t)0b0'00000001'000'00000'00000'00000'00000u));
  assert(numeric_limits<float>::denorm_min()
         == bit_cast<float>
         ((uint32_t)0b0'00000000'000'00000'00000'00000'00001u));
  assert(numeric_limits<float>::lowest()
         == bit_cast<float>
         ((uint32_t)0b1'11111110'111'11111'11111'11111'11111u));
  static_assert(numeric_limits<float>::is_signed == true);
  static_assert(numeric_limits<float>::is_specialized == true);

  static_assert(numeric_limits<double>::is_signed == true);
  static_assert(numeric_limits<double>::is_specialized == true);
}

namespace help_three_way {

struct t {
  int value;
};
strong_ordering operator <=>(const t &, const t &) = delete;
bool operator <(t a, t b) {
  return a.value < b.value;
}

struct tt {
  int value;

  friend auto operator <=>(const tt &, const tt &) = default;
};

void test_synth_three_way() {
  {
    t a{1}, b{2};
    assert(a < b);
    assert(synth_3way(a, b) < 0);
  }
  {
    tt a{1}, b{2};
    assert(a <=> b < 0);
    assert(synth_3way(a, b) < 0);
    static_assert(same_as<synth_3way_result<tt>, strong_ordering>);
  }
}

}
void test_three_way_comparison() {
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
    static_assert(same_as<decltype(1 <=> 2), strong_ordering>);
    static_assert(same_as<decltype(1.0 <=> 2.0), partial_ordering>);
    static_assert((1 <=> 2) == strong_lt);
    static_assert((2 <=> 1) == strong_gt);
    static_assert((1 <=> 1) == strong_eq);
    static_assert((1 <=> 2) == weak_lt);
    static_assert((2 <=> 1) == weak_gt);
    static_assert((1 <=> 1) == weak_eq);
    static_assert((1 <=> 2) == partial_lt);
    static_assert((2 <=> 1) == partial_gt);
    static_assert((1 <=> 1) == partial_eq);
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

  // is_eq/neq/lt/gt/lteq/gteq
  {
    static_assert(is_trivial_empty_v<decltype(is_eq)>);
    static_assert(is_trivial_empty_v<decltype(is_neq)>);
    static_assert(is_trivial_empty_v<decltype(is_lt)>);
    static_assert(is_trivial_empty_v<decltype(is_gt)>);
    static_assert(is_trivial_empty_v<decltype(is_lteq)>);
    static_assert(is_trivial_empty_v<decltype(is_gteq)>);

    static_assert(is_eq(1 <=> 1));
    static_assert(is_neq(0 <=> 1));
    static_assert(is_lt(0 <=> 1));
    static_assert(is_gt(1 <=> 0));
    static_assert(is_lteq(1 <=> 1));
    static_assert(is_lteq(0 <=> 1));
    static_assert(is_gteq(1 <=> 1));
    static_assert(is_gteq(1 <=> 0));
  }

  // common_comparison_category
  {
    using t0 = void;
    using t1 = partial_ordering;
    using t2 = weak_ordering;
    using t3 = strong_ordering;
    static_assert(is_same_v<common_comparison_category_t<>, t3>);
    static_assert(is_same_v<common_comparison_category_t<t3>, t3>);
    static_assert(is_same_v<common_comparison_category_t<t2>, t2>);
    static_assert(is_same_v<common_comparison_category_t<t1>, t1>);
    static_assert(is_same_v<common_comparison_category_t<t0>, t0>);
    static_assert(is_same_v<common_comparison_category_t<int>, void>);
    static_assert(is_same_v<common_comparison_category_t<t0, t1, t2, t3>, t0>);
    static_assert(is_same_v<common_comparison_category_t<t3, t2, t1, t0>, t0>);
    static_assert(is_same_v
                  <common_comparison_category_t<t3, volatile t2, t1, t0>, t0>);
    static_assert(is_same_v<common_comparison_category_t<t1, t3, t2>, t1>);
    static_assert(is_same_v<common_comparison_category_t<t3, t2, t3>, t2>);
    static_assert(is_same_v<common_comparison_category_t<t3, t3, t3>, t3>);
    static_assert(is_void_v<common_comparison_category_t<t3, t3, t3, int>>);
  }

  // inner::compares_as
  {
    static_assert(inner::compares_as<strong_ordering, weak_ordering>);
    static_assert(!inner::compares_as<weak_ordering, strong_ordering>);
    static_assert(inner::compares_as<weak_ordering, partial_ordering>);
  }

  // three_way_comparable
  // three_way_comparable_with
  {
    static_assert(three_way_comparable<int>);
    static_assert(!three_way_comparable<nullptr_t>);
    static_assert(three_way_comparable_with<int, int>);
    static_assert(!three_way_comparable_with<int, void *>);
  }

  // compare_three_way_result
  // compare_three_way
  // can_apply_compare_three_way
  {
    static_assert(is_trivial_empty_v<compare_three_way_result<int, int>>);
    static_assert(is_trivial_empty_v<compare_three_way>);

    static_assert(!has_member_type_type_v
                  <compare_three_way_result<void *, int>>);
    static_assert(is_same_v
                  <compare_three_way_result_t<int, int>, strong_ordering>);
    static_assert(is_same_v
                  <compare_three_way_result_t<float, float>, partial_ordering>);
    static_assert(is_same_v
                  <compare_three_way_result_t<int, float>, partial_ordering>);

    static_assert(is_transparent_function_v<compare_three_way>);
    assert(compare_three_way{}(0, 0.0) == 0);

    struct t {
      strong_ordering operator <=>(const t &) const = default;
    };
    struct tt {};
    static_assert(can_apply_compare_three_way<const int &, const int &>);
    static_assert(can_apply_compare_three_way<const t &, const t &>);
    static_assert(!can_apply_compare_three_way<tt, tt>);
  }

  // synth_3way
  // synth_3way_result
  help_three_way::test_synth_three_way();
}

void test_type_index() {
  static_assert(!default_initializable<type_index>);
  static_assert(destructible<type_index>);
  static_assert(copyable<type_index>);
  static_assert(swappable<type_index>);
  static_assert(equality_comparable<type_index>);
  static_assert(three_way_comparable<type_index>);
  static_assert(constructible_from<type_index, const type_info &>);

  type_index i = typeid(int);
  assert(i.hash_code() == typeid(int).hash_code());
  assert(i.name() == typeid(int).name());
  assert(i == i);
  assert(!(i != i));
  assert(!(i < i));
  assert(!(i > i));
  assert(i <= i);
  assert(i >= i);
  assert(i <=> i == 0);

  type_index ii = typeid(int *);
  if (typeid(int).before(typeid(int *))) {
    assert(i < ii);
    assert(!(i > ii));
    assert(i <= ii);
    assert(!(i >= ii));
    assert(i <=> ii < 0);
  }
  else {
    assert(ii < i);
    assert(!(ii > i));
    assert(ii <= i);
    assert(!(ii >= i));
    assert(ii <=> i < 0);
  }
}

void test_basic_function_objects() {
  // is_transparent_function
  // plus
  // minus
  // multiplies
  // divides
  // modulus
  // negate
  // equal_to
  // not_equal_to
  // greater
  // less
  // greater_equal
  // less_equal
  // logical_and
  // logical_or
  // logical_not
  // bit_and
  // bit_or
  // bit_xor
  // bit_not
  {
    static_assert(is_trivial_empty_v<plus<>>);
    static_assert(is_trivial_empty_v<plus<int>>);
    static_assert(is_trivial_empty_v<minus<>>);
    static_assert(is_trivial_empty_v<minus<int>>);
    static_assert(is_trivial_empty_v<multiplies<>>);
    static_assert(is_trivial_empty_v<multiplies<int>>);
    static_assert(is_trivial_empty_v<divides<>>);
    static_assert(is_trivial_empty_v<divides<int>>);
    static_assert(is_trivial_empty_v<modulus<>>);
    static_assert(is_trivial_empty_v<modulus<int>>);
    static_assert(is_trivial_empty_v<negate<>>);
    static_assert(is_trivial_empty_v<negate<int>>);
    static_assert(is_trivial_empty_v<equal_to<>>);
    static_assert(is_trivial_empty_v<equal_to<int>>);
    static_assert(is_trivial_empty_v<not_equal_to<>>);
    static_assert(is_trivial_empty_v<not_equal_to<int>>);
    static_assert(is_trivial_empty_v<greater<>>);
    static_assert(is_trivial_empty_v<greater<int>>);
    static_assert(is_trivial_empty_v<less<>>);
    static_assert(is_trivial_empty_v<less<int>>);
    static_assert(is_trivial_empty_v<greater_equal<>>);
    static_assert(is_trivial_empty_v<greater_equal<int>>);
    static_assert(is_trivial_empty_v<less_equal<>>);
    static_assert(is_trivial_empty_v<less_equal<int>>);
    static_assert(is_trivial_empty_v<logical_and<>>);
    static_assert(is_trivial_empty_v<logical_and<int>>);
    static_assert(is_trivial_empty_v<logical_or<>>);
    static_assert(is_trivial_empty_v<logical_or<int>>);
    static_assert(is_trivial_empty_v<logical_not<>>);
    static_assert(is_trivial_empty_v<logical_not<int>>);
    static_assert(is_trivial_empty_v<bit_and<>>);
    static_assert(is_trivial_empty_v<bit_and<int>>);
    static_assert(is_trivial_empty_v<bit_or<>>);
    static_assert(is_trivial_empty_v<bit_or<int>>);
    static_assert(is_trivial_empty_v<bit_xor<>>);
    static_assert(is_trivial_empty_v<bit_xor<int>>);
    static_assert(is_trivial_empty_v<bit_not<>>);
    static_assert(is_trivial_empty_v<bit_not<int>>);

    static_assert(is_transparent_function_v<plus<>>);
    static_assert(!is_transparent_function_v<plus<int>>);
    static_assert(is_transparent_function_v<minus<>>);
    static_assert(!is_transparent_function_v<minus<int>>);
    static_assert(is_transparent_function_v<multiplies<>>);
    static_assert(!is_transparent_function_v<multiplies<int>>);
    static_assert(is_transparent_function_v<divides<>>);
    static_assert(!is_transparent_function_v<divides<int>>);
    static_assert(is_transparent_function_v<modulus<>>);
    static_assert(!is_transparent_function_v<modulus<int>>);
    static_assert(is_transparent_function_v<negate<>>);
    static_assert(!is_transparent_function_v<negate<int>>);
    static_assert(is_transparent_function_v<equal_to<>>);
    static_assert(!is_transparent_function_v<equal_to<int>>);
    static_assert(is_transparent_function_v<not_equal_to<>>);
    static_assert(!is_transparent_function_v<not_equal_to<int>>);
    static_assert(is_transparent_function_v<greater<>>);
    static_assert(!is_transparent_function_v<greater<int>>);
    static_assert(is_transparent_function_v<less<>>);
    static_assert(!is_transparent_function_v<less<int>>);
    static_assert(is_transparent_function_v<greater_equal<>>);
    static_assert(!is_transparent_function_v<greater_equal<int>>);
    static_assert(is_transparent_function_v<less_equal<>>);
    static_assert(!is_transparent_function_v<less_equal<int>>);
    static_assert(is_transparent_function_v<logical_and<>>);
    static_assert(!is_transparent_function_v<logical_and<int>>);
    static_assert(is_transparent_function_v<logical_or<>>);
    static_assert(!is_transparent_function_v<logical_or<int>>);
    static_assert(is_transparent_function_v<logical_not<>>);
    static_assert(!is_transparent_function_v<logical_not<int>>);
    static_assert(is_transparent_function_v<bit_and<>>);
    static_assert(!is_transparent_function_v<bit_and<int>>);
    static_assert(is_transparent_function_v<bit_or<>>);
    static_assert(!is_transparent_function_v<bit_or<int>>);
    static_assert(is_transparent_function_v<bit_xor<>>);
    static_assert(!is_transparent_function_v<bit_xor<int>>);
    static_assert(is_transparent_function_v<bit_not<>>);
    static_assert(!is_transparent_function_v<bit_not<int>>);

    static_assert(!noexcept(plus<>()(1, 2.0)));
    static_assert(!noexcept(plus<int>()(1, 2)));
    static_assert(!noexcept(minus<>()(1, 2.0)));
    static_assert(!noexcept(minus<int>()(1, 2)));
    static_assert(!noexcept(multiplies<>()(3.0, 2)));
    static_assert(!noexcept(multiplies<int>()(3, 2)));
    static_assert(!noexcept(divides<>()(7.0, 2)));
    static_assert(!noexcept(divides<int>()(7, 2)));
    static_assert(!noexcept(modulus<>()(7ul, 2)));
    static_assert(!noexcept(modulus<int>()(7, 2)));
    static_assert(!noexcept(negate<>()(2.0)));
    static_assert(!noexcept(negate<int>()(1)));

    static_assert(!noexcept(equal_to<>()(1, 1.0)));
    static_assert(!noexcept(equal_to<int>()(1, 1)));
    static_assert(!noexcept(not_equal_to<>()(1, 2.0)));
    static_assert(!noexcept(not_equal_to<int>()(1, 2)));
    static_assert(!noexcept(greater<>()(2.0, 1)));
    static_assert(!noexcept(greater<int>()(2, 1)));
    static_assert(!noexcept(less<>()(1, 2.0)));
    static_assert(!noexcept(less<int>()(1, 2)));
    static_assert(!noexcept(greater_equal<>()(2, 2.0)));
    static_assert(!noexcept(greater_equal<int>()(2, 2)));
    static_assert(!noexcept(greater_equal<>()(3, 2.0)));
    static_assert(!noexcept(greater_equal<int>()(3, 2)));
    static_assert(!noexcept(less_equal<>()(2, 2.0)));
    static_assert(!noexcept(less_equal<int>()(2, 2)));
    static_assert(!noexcept(less_equal<>()(2.0, 3)));
    static_assert(!noexcept(less_equal<int>()(2, 3)));

    static_assert(!noexcept(logical_and<>()(1, false)));
    static_assert(!noexcept(logical_and<>()(0, false)));
    static_assert(!noexcept(logical_and<>()(1, true)));
    static_assert(!noexcept(logical_and<>()(0, true)));
    static_assert(!noexcept(logical_and<bool>()(false, false)));
    static_assert(!noexcept(logical_and<bool>()(true, false)));
    static_assert(!noexcept(logical_and<bool>()(false, true)));
    static_assert(!noexcept(logical_and<bool>()(true, true)));
    static_assert(!noexcept(logical_or<>()(1, false)));
    static_assert(!noexcept(logical_or<>()(0, false)));
    static_assert(!noexcept(logical_or<>()(1, true)));
    static_assert(!noexcept(logical_or<>()(0, true)));
    static_assert(!noexcept(logical_or<bool>()(false, false)));
    static_assert(!noexcept(logical_or<bool>()(true, false)));
    static_assert(!noexcept(logical_or<bool>()(false, true)));
    static_assert(!noexcept(logical_or<bool>()(true, true)));
    static_assert(!noexcept(logical_not<>()(1)));
    static_assert(!noexcept(logical_not<>()(0)));
    static_assert(!noexcept(logical_not<bool>()(false)));
    static_assert(!noexcept(logical_not<bool>()(true)));

    static_assert(!noexcept(bit_and<>()(0b10110u, 0b10101u)));
    static_assert(!noexcept(bit_and<>()(0b0u, 0b1u)));
    static_assert(!noexcept(bit_and<>()(0b10u, 0b1u)));
    static_assert(!noexcept(bit_and<uint8_t>()(0b100u, 0b110u)));
    static_assert(!noexcept(bit_and<uint8_t>()(0b10u, 0b11u)));
    static_assert(!noexcept(bit_or<>()(0b10110u, 0b10101u)));
    static_assert(!noexcept(bit_or<>()(0b0u, 0b1u)));
    static_assert(!noexcept(bit_or<>()(0b10u, 0b1u)));
    static_assert(!noexcept(bit_or<uint8_t>()(0b100u, 0b110u)));
    static_assert(!noexcept(bit_or<uint8_t>()(0b10u, 0b11u)));
    static_assert(!noexcept(bit_xor<>()(0b10110u, 0b10101u)));
    static_assert(!noexcept(bit_xor<>()(0b0u, 0b1u) == 0b1u));
    static_assert(!noexcept(bit_xor<>()(0b10u, 0b1u) == 0b11u));
    static_assert(!noexcept(bit_xor<uint8_t>()(0b100u, 0b110u)));
    static_assert(!noexcept(bit_xor<uint8_t>()(0b10u, 0b11u)));
    static_assert(!noexcept(bit_not<>()(1234)));
    static_assert(!noexcept(bit_not<>()(0b0u)));
    static_assert(!noexcept(bit_not<uint8_t>()(0b10101010u)));
    static_assert(!noexcept(bit_not<uint8_t>()(0b0u)));

    static_assert(plus<>()(1, 2.0) == 3.0);
    static_assert(plus<int>()(1, 2) == 3);
    static_assert(minus<>()(1, 2.0) == -1.0);
    static_assert(minus<int>()(1, 2) == -1);
    static_assert(multiplies<>()(3.0, 2) == 6.0);
    static_assert(multiplies<int>()(3, 2) == 6);
    static_assert(divides<>()(7.0, 2) == 3.5);
    static_assert(divides<int>()(7, 2) == 3);
    static_assert(modulus<>()(7ul, 2) == 1);
    static_assert(modulus<int>()(7, 2) == 1);
    static_assert(negate<>()(2.0) == -2.0);
    static_assert(negate<int>()(1) == -1);

    static_assert(equal_to<>()(1, 1.0));
    static_assert(equal_to<int>()(1, 1));
    static_assert(not_equal_to<>()(1, 2.0));
    static_assert(not_equal_to<int>()(1, 2));
    static_assert(greater<>()(2.0, 1));
    static_assert(greater<int>()(2, 1));
    static_assert(less<>()(1, 2.0));
    static_assert(less<int>()(1, 2));
    static_assert(greater_equal<>()(2, 2.0));
    static_assert(greater_equal<int>()(2, 2));
    static_assert(greater_equal<>()(3, 2.0));
    static_assert(greater_equal<int>()(3, 2));
    static_assert(less_equal<>()(2, 2.0));
    static_assert(less_equal<int>()(2, 2));
    static_assert(less_equal<>()(2.0, 3));
    static_assert(less_equal<int>()(2, 3));

    static_assert(logical_and<>()(1, false) == false);
    static_assert(logical_and<>()(0, false) == false);
    static_assert(logical_and<>()(1, true) == true);
    static_assert(logical_and<>()(0, true) == false);
    static_assert(logical_and<bool>()(false, false) == false);
    static_assert(logical_and<bool>()(true, false) == false);
    static_assert(logical_and<bool>()(false, true) == false);
    static_assert(logical_and<bool>()(true, true) == true);
    static_assert(logical_or<>()(1, false) == true);
    static_assert(logical_or<>()(0, false) == false);
    static_assert(logical_or<>()(1, true) == true);
    static_assert(logical_or<>()(0, true) == true);
    static_assert(logical_or<bool>()(false, false) == false);
    static_assert(logical_or<bool>()(true, false) == true);
    static_assert(logical_or<bool>()(false, true) == true);
    static_assert(logical_or<bool>()(true, true) == true);
    static_assert(logical_not<>()(1) == false);
    static_assert(logical_not<>()(0) == true);
    static_assert(logical_not<bool>()(false) == true);
    static_assert(logical_not<bool>()(true) == false);

    static_assert(bit_and<>()(0b10110u, 0b10101u) == 0b10100u);
    static_assert(bit_and<>()(0b0u, 0b1u) == 0);
    static_assert(bit_and<>()(0b10u, 0b1u) == 0);
    static_assert(bit_and<uint8_t>()(0b100u, 0b110u) == 0b100u);
    static_assert(bit_and<uint8_t>()(0b10u, 0b11u) == 0b10u);
    static_assert(bit_or<>()(0b10110u, 0b10101u) == 0b10111u);
    static_assert(bit_or<>()(0b0u, 0b1u) == 0b1u);
    static_assert(bit_or<>()(0b10u, 0b1u) == 0b11u);
    static_assert(bit_or<uint8_t>()(0b100u, 0b110u) == 0b110u);
    static_assert(bit_or<uint8_t>()(0b10u, 0b11u) == 0b11u);
    static_assert(bit_xor<>()(0b10110u, 0b10101u) == 0b00011u);
    static_assert(bit_xor<>()(0b0u, 0b1u) == 0b1u);
    static_assert(bit_xor<>()(0b10u, 0b1u) == 0b11u);
    static_assert(bit_xor<uint8_t>()(0b100u, 0b110u) == 0b010u);
    static_assert(bit_xor<uint8_t>()(0b10u, 0b11u) == 0b01u);
    static_assert(bit_not<>()(1234) == ~1234);
    static_assert(bit_not<>()(0b0u) == (unsigned)-1);
    static_assert(bit_not<uint8_t>()(0b10101010u) == 0b01010101u);
    static_assert(bit_not<uint8_t>()(0b0u) == 0b11111111u);
  }

  // can_apply_plus
  {
    struct t {};
    struct s {
      int operator +(const t &) const {return 0;}
    };
    static_assert(!can_apply_plus<const t &, const t &>);
    static_assert(!can_apply_plus<const s &, const s &>);
    static_assert(!can_apply_plus<const t &, const s &>);
    static_assert(can_apply_plus<const s &, const t &>);
  }
  // can_apply_minus
  {
    struct t {};
    struct s {
      int operator -(const t &) const {return 0;}
    };
    static_assert(!can_apply_minus<const t &, const t &>);
    static_assert(!can_apply_minus<const s &, const s &>);
    static_assert(!can_apply_minus<const t &, const s &>);
    static_assert(can_apply_minus<const s &, const t &>);
  }
  // can_apply_multiplies
  {
    struct t {};
    struct s {
      int operator *(const t &) const {return 0;}
    };
    static_assert(!can_apply_multiplies<const t &, const t &>);
    static_assert(!can_apply_multiplies<const s &, const s &>);
    static_assert(!can_apply_multiplies<const t &, const s &>);
    static_assert(can_apply_multiplies<const s &, const t &>);
  }
  // can_apply_divides
  {
    struct t {};
    struct s {
      int operator /(const t &) const {return 0;}
    };
    static_assert(!can_apply_divides<const t &, const t &>);
    static_assert(!can_apply_divides<const s &, const s &>);
    static_assert(!can_apply_divides<const t &, const s &>);
    static_assert(can_apply_divides<const s &, const t &>);
  }
  // can_apply_modulus
  {
    struct t {};
    struct s {
      int operator %(const t &) const {return 0;}
    };
    static_assert(!can_apply_modulus<const t &, const t &>);
    static_assert(!can_apply_modulus<const s &, const s &>);
    static_assert(!can_apply_modulus<const t &, const s &>);
    static_assert(can_apply_modulus<const s &, const t &>);
  }
  // can_apply_negate
  {
    struct t {};
    struct tt {
      int operator -() {return 0;}
    };
    static_assert(!can_apply_negate<t>);
    static_assert(!can_apply_negate<const t &>);
    static_assert(!can_apply_negate<const tt &>);
    static_assert(can_apply_negate<tt &>);
  }
  // can_apply_equal_to
  {
    struct t {};
    struct s {
      int operator ==(const t &) const {return 0;}
    };
    static_assert(!can_apply_equal_to<const t &, const t &>);
    static_assert(!can_apply_equal_to<const s &, const s &>);
    static_assert(!can_apply_equal_to<const t &, const s &>);
    static_assert(can_apply_equal_to<const s &, const t &>);
  }
  // can_apply_not_equal_to
  {
    struct t {};
    struct s {
      int operator !=(const t &) const {return 0;}
    };
    static_assert(!can_apply_not_equal_to<const t &, const t &>);
    static_assert(!can_apply_not_equal_to<const s &, const s &>);
    static_assert(!can_apply_not_equal_to<const t &, const s &>);
    static_assert(can_apply_not_equal_to<const s &, const t &>);
  }
  // can_apply_greater
  {
    struct t {};
    struct s {
      int operator >(const t &) const {return 0;}
    };
    static_assert(!can_apply_greater<const t &, const t &>);
    static_assert(!can_apply_greater<const s &, const s &>);
    static_assert(!can_apply_greater<const t &, const s &>);
    static_assert(can_apply_greater<const s &, const t &>);
  }
  // can_apply_less
  {
    struct t {};
    struct s {
      int operator <(const t &) const {return 0;}
    };
    static_assert(!can_apply_less<const t &, const t &>);
    static_assert(!can_apply_less<const s &, const s &>);
    static_assert(!can_apply_less<const t &, const s &>);
    static_assert(can_apply_less<const s &, const t &>);
  }
  // can_apply_greater_equal
  {
    struct t {};
    struct s {
      int operator >=(const t &) const {return 0;}
    };
    static_assert(!can_apply_greater_equal<const t &, const t &>);
    static_assert(!can_apply_greater_equal<const s &, const s &>);
    static_assert(!can_apply_greater_equal<const t &, const s &>);
    static_assert(can_apply_greater_equal<const s &, const t &>);
  }
  // can_apply_less_equal
  {
    struct t {};
    struct s {
      int operator <=(const t &) const {return 0;}
    };
    static_assert(!can_apply_less_equal<const t &, const t &>);
    static_assert(!can_apply_less_equal<const s &, const s &>);
    static_assert(!can_apply_less_equal<const t &, const s &>);
    static_assert(can_apply_less_equal<const s &, const t &>);
  }
  // can_apply_logical_and
  {
    struct t {};
    struct s {
      int operator &&(const t &) const {return 0;}
    };
    static_assert(!can_apply_logical_and<const t &, const t &>);
    static_assert(!can_apply_logical_and<const s &, const s &>);
    static_assert(!can_apply_logical_and<const t &, const s &>);
    static_assert(can_apply_logical_and<const s &, const t &>);
  }
  // can_apply_logical_or
  {
    struct t {};
    struct s {
      int operator ||(const t &) const {return 0;}
    };
    static_assert(!can_apply_logical_or<const t &, const t &>);
    static_assert(!can_apply_logical_or<const s &, const s &>);
    static_assert(!can_apply_logical_or<const t &, const s &>);
    static_assert(can_apply_logical_or<const s &, const t &>);
  }
  // can_apply_logical_not
  {
    struct t {};
    struct tt {
      int operator !() {return 0;}
    };
    static_assert(!can_apply_logical_not<t>);
    static_assert(!can_apply_logical_not<const t &>);
    static_assert(!can_apply_logical_not<const tt &>);
    static_assert(can_apply_logical_not<tt &>);
  }
  // can_apply_bit_and
  {
    struct t {};
    struct s {
      int operator &(const t &) const {return 0;}
    };
    static_assert(!can_apply_bit_and<const t &, const t &>);
    static_assert(!can_apply_bit_and<const s &, const s &>);
    static_assert(!can_apply_bit_and<const t &, const s &>);
    static_assert(can_apply_bit_and<const s &, const t &>);
  }
  // can_apply_bit_or
  {
    struct t {};
    struct s {
      int operator |(const t &) const {return 0;}
    };
    static_assert(!can_apply_bit_or<const t &, const t &>);
    static_assert(!can_apply_bit_or<const s &, const s &>);
    static_assert(!can_apply_bit_or<const t &, const s &>);
    static_assert(can_apply_bit_or<const s &, const t &>);
  }
  // can_apply_bit_xor
  {
    struct t {};
    struct s {
      int operator ^(const t &) const {return 0;}
    };
    static_assert(!can_apply_bit_xor<const t &, const t &>);
    static_assert(!can_apply_bit_xor<const s &, const s &>);
    static_assert(!can_apply_bit_xor<const t &, const s &>);
    static_assert(can_apply_bit_xor<const s &, const t &>);
  }
  // can_apply_bit_not
  {
    struct t {};
    struct tt {
      int operator ~() {return 0;}
    };
    static_assert(!can_apply_bit_not<t>);
    static_assert(!can_apply_bit_not<const t &>);
    static_assert(!can_apply_bit_not<const tt &>);
    static_assert(can_apply_bit_not<tt &>);
  }

  // identity
  {
    static_assert(is_trivial_empty_v<identity>);
    static_assert(is_transparent_function_v<identity>);
    static_assert(noexcept(identity()(2)));
    static_assert(identity()(2) == 2);
    static_assert(identity()(identity()(3.5)) == 3.5);
  }

  // not_fn
  {
    int i{};
    const auto f = not_fn([i]() constexpr {return true;});
    static_assert(!f());
    const auto f2 = not_fn([](int, int) constexpr {return true;});
    static_assert(!f2(1, 1));
    static_assert(!default_initializable<decltype(f)>);
    static_assert(is_move_constructible_v<decltype(f)>);
    static_assert(is_copy_constructible_v<decltype(f)>);
    static_assert(!is_move_assignable_v<decltype(f)>);
    static_assert(!is_copy_assignable_v<decltype(f)>);
    static_assert(!is_swappable_v<decltype(f)>);
    static_assert(default_initializable<decltype(f2)>);
    static_assert(is_move_constructible_v<decltype(f2)>);
    static_assert(is_copy_constructible_v<decltype(f2)>);
    static_assert(!is_move_assignable_v<decltype(f2)>);
    static_assert(!is_copy_assignable_v<decltype(f2)>);
    static_assert(!is_swappable_v<decltype(f2)>);

    struct t {
      int x;

      int operator ()(int) & {return x;}
      int operator ()(int) const & {return !x;}
    };
    static_assert(is_nothrow_constructible_v<t>);
    static_assert(is_nothrow_destructible_v<t>);
    static_assert(is_nothrow_copy_constructible_v<t>);
    static_assert(is_nothrow_copy_assignable_v<t>);
    static_assert(is_nothrow_move_constructible_v<t>);
    static_assert(is_nothrow_move_assignable_v<t>);
    static_assert(is_nothrow_swappable_v<t>);
    t t1{1}, t2{0};
    auto nf1 = not_fn(t1);
    auto nf2 = not_fn(t2);
    static_assert(!noexcept(nf1(0)));
    assert(!nf1(0));
    assert(as_const(nf1)(0));
    assert(nf2(0));
    assert(!as_const(nf2)(0));
    adl_swap(nf1, nf2);
    assert(!nf2(0));
    assert(as_const(nf2)(0));
    assert(nf1(0));
    assert(!as_const(nf1)(0));

    struct tt {
      int x;

      int operator ()(int) && noexcept {return x;}
      int operator ()(int) const && noexcept {return !x;}
    };
    static_assert(is_nothrow_constructible_v<tt>);
    static_assert(is_nothrow_destructible_v<tt>);
    static_assert(is_nothrow_copy_constructible_v<tt>);
    static_assert(is_nothrow_copy_assignable_v<tt>);
    static_assert(is_nothrow_move_constructible_v<tt>);
    static_assert(is_nothrow_move_assignable_v<tt>);
    static_assert(is_nothrow_swappable_v<tt>);
    tt t3{1}, t4{0};
    auto nf3 = not_fn(t3);
    auto nf4 = not_fn(t4);
    static_assert(!noexcept(move(nf3)(0)));
    assert(!move(nf3)(0));
    assert(move(as_const(nf3))(0));
    assert(move(nf4)(0));
    assert(!move(as_const(nf4))(0));
    adl_swap(nf3, nf4);
    assert(!move(nf4)(0));
    assert(move(as_const(nf4))(0));
    assert(move(nf3)(0));
    assert(!move(as_const(nf3))(0));
  }

  // mem_fn
  {
    struct A {
      int value = 0;
      int f() {
        return 0;
      }
      int f(int x, int y) {
        return x + y;
      }
    };
    A a;

    const auto f1 = mem_fn(&A::value);
    static_assert(semiregular<remove_const_t<decltype(f1)>>);
    static_assert(is_trivially_copyable_v<remove_const_t<decltype(f1)>>);
    assert(f1(a) == 0);

    int (A::*const fp)() = &A::f;
    const auto f2 = mem_fn(fp);
    static_assert(!noexcept(f2(a)));
    assert(f2(a) == 0);

    int (A::*const fp2)(int, int) = &A::f;
    const auto f3 = mem_fn(fp2);
    assert(f3(&a, 1, 2) == 3);
  }
}

template <int I, bool EXPLICIT = false, int USES_ALLOC_CTG = 0>
struct test_t {
  int value;

  test_t() : value{} {}
  ~test_t() {}
  test_t(const test_t &) = default;
  test_t &operator =(const test_t &) = default;
  test_t(test_t &&) = default;
  test_t &operator =(test_t &&) = default;

  template <integral U>
  explicit(EXPLICIT) test_t(U i) requires same_as<U, int> : value(i) {}

  template <integral U>
  explicit(EXPLICIT) test_t(allocator_arg_t, U x, U y)
    requires same_as<U, int> && (USES_ALLOC_CTG == 1)
    : value(x * 10 + y) {}
  template <integral U>
  explicit(EXPLICIT) test_t(U y, U x)
    requires same_as<U, int> && (USES_ALLOC_CTG == 2)
    : value(x * 10 + y) {}

  friend bool operator ==(const test_t &, const test_t &) = default;
  friend strong_ordering operator <=>(const test_t &, const test_t &) = default;
};
void test_test_t() {
  {
    using t = test_t<0, true, 0>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(!is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(!is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(!is_constructible_v<t, int, int>);
    t x;
    assert(x.value == 0);
    t y{3};
    assert(y.value == 3);
  }
  {
    using t = test_t<0, true, 1>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(!is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(!is_constructible_v<t, int, int>);
    t x(allocator_arg, 4, 3);
    assert(x.value == 43);
  }
  {
    using t = test_t<0, true, 2>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(!is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(!is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(is_constructible_v<t, int, int>);
    t x(3, 4);
    assert(x.value == 43);
  }
  {
    using t = test_t<0, false, 0>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(!is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(!is_constructible_v<t, int, int>);
  }
  {
    using t = test_t<0, false, 1>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(!is_constructible_v<t, int, int>);
  }
  {
    using t = test_t<0, false, 2>;
    static_assert(regular<t>);
    static_assert(is_constructible_v<t, int>);
    static_assert(is_convertible_v<int, t>);
    static_assert(!is_constructible_v<t, long>);
    static_assert(!is_constructible_v<t, allocator_arg_t, int, int>);
    static_assert(is_constructible_v<t, int, int>);
    t x(3, 4);
    assert(x.value == 43);
  }
}
template <class T>
class owner_pointer {
  T *p;

public:
  owner_pointer() : p{} {}
  ~owner_pointer() {
    if (p != nullptr)
      delete p;
  }
  owner_pointer(const owner_pointer &x) {
    if (x.p != nullptr)
      p = new T(*x.p);
    else
      p = nullptr;
  }
  owner_pointer &operator =(const owner_pointer &x) {
    if (addressof(x) == this)
      return *this;
    if (p != nullptr) {
      if (x.p != nullptr)
        *p = *x.p;
      else {
        delete p;
        p = nullptr;
      }
    }
    else {
      if (x.p != nullptr)
        p = new T(*x.p);
    }
    return *this;
  }
  owner_pointer(owner_pointer &&x) {
    p = x.p;
    x.p = nullptr;
  }
  owner_pointer &operator =(owner_pointer &&x) {
    if (addressof(x) == this)
      return *this;
    if (p != nullptr)
      delete p;
    p = x.p;
    x.p = nullptr;
    return *this;
  }
  void swap(owner_pointer &x) noexcept {
    default_swap(p, x.p);
  }

  owner_pointer(nullptr_t) : p{} {}
  explicit owner_pointer(T x) : p(new T(move(x))) {}

  T &operator *() const {
    return *p;
  }
  T *operator ->() const {
    return p;
  }

  bool operator ==(nullptr_t) const {
    return p == nullptr;
  }
  bool operator !=(nullptr_t) const {
    return p != nullptr;
  }
};
template <class T>
void swap(owner_pointer<T> &x, owner_pointer<T> &y) noexcept {
  x.swap(y);
}

void test_tuple() {
  // test_t
  {
    test_test_t();
  }

  // owner_pointer
  {
    {
      owner_pointer<int> a{}, b{2}, c{3};
      assert(a == nullptr);
      assert(b != nullptr && *b == 2);
      assert(c != nullptr && *c == 3);

      owner_pointer<int> d{move(a)}, e{move(b)}, f{a}, g{c};
      assert(d == nullptr && a == nullptr);
      assert(e != nullptr && *e == 2 && b == nullptr);
      assert(f == nullptr && a == nullptr);
      assert(g != nullptr && *g == 3 && c != nullptr && *c == 3);

      owner_pointer<int> h = nullptr;
      assert(h == nullptr);
    }
    {
      owner_pointer<int> a{}, b{}, c{3}, d{4};
      assert(a == nullptr && b == nullptr);
      assert(c != nullptr && *c == 3);
      assert(d != nullptr && *d == 4);
      a = a;
      assert(a == nullptr);
      c = c;
      assert(c != nullptr && *c == 3);
      a = b;
      assert(a == nullptr && b == nullptr);
      a = c;
      assert(a != nullptr && *a == 3);
      assert(c != nullptr && *c == 3);
      a = d;
      assert(a != nullptr && *a == 4);
      assert(d != nullptr && *d == 4);
      a = b;
      assert(a == nullptr && b == nullptr);
    }
    {
      owner_pointer<int> a{1}, b{2};
      adl_swap(a, b);
      assert(a != nullptr && *a == 2);
      assert(b != nullptr && *b == 1);
    }
  }

  // deduction guide
  {
    tuple t0(0);
    tuple t(1, 2, 3);
    pair p(1, 2);
    assert(t0 == make_tuple(0));
    assert(t == make_tuple(1, 2, 3));
    assert(p == make_pair(1, 2));

    static_assert(is_same_v<decltype(t0), tuple<int>>);
    static_assert(is_same_v<decltype(t), tuple<int, int, int>>);
    static_assert(is_same_v<decltype(p), tuple<int, int>>);
    static_assert(is_same_v<decltype(p), pair<int, int>>);

    tuple t00(allocator_arg, 0, 0);
    tuple tt(allocator_arg, 0, 1, 2, 3);
    pair pp(allocator_arg, 0, 1, 2);
    assert(t0 == t00);
    assert(t == tt);
    assert(p == pp);

    tuple x{};
    tuple y(allocator_arg, 0);
    static_assert(is_same_v<decltype(x), tuple<>>);
    static_assert(is_same_v<decltype(y), tuple<>>);
  }

  // piecwise_construct
  // ignore
  // allocator_arg
  {
    static_assert(is_trivial_empty_v<piecewise_construct_t>);
    static_assert(is_same_v
                  <decltype(piecewise_construct), const piecewise_construct_t>);
    static_assert(is_trivial_empty_v<allocator_arg_t>);
    static_assert(is_same_v<decltype(allocator_arg), const allocator_arg_t>);
    static_assert(is_trivial_empty_v<inner::ignore_t>);
    static_assert(is_same_v<decltype(ignore), const inner::ignore_t>);
  }

  // tuple_size
  // tuple_element
  {
    using t0 = tuple<>;
    using t1 = tuple<int>;
    using t2 = tuple<char, int>;
    using t3 = tuple<char, int, float>;
    using t4 = tuple<char, int, float, double>;

    static_assert(!has_member_static_value_v<tuple_size<void>>);
    static_assert(!has_member_type_type_v<tuple_element<0, void>>);

    static_assert(tuple_size<t0>::value == 0);
    static_assert(tuple_size<t1>::value == 1);
    static_assert(tuple_size<t2>::value == 2);
    static_assert(tuple_size<t3>::value == 3);
    static_assert(tuple_size<t4>::value == 4);

    static_assert(is_same_v<tuple_element_t<0, t0>, void>);

    static_assert(is_same_v<tuple_element_t<0, t1>, int>);
    static_assert(is_same_v<const tuple_element_t<0, t1>, const int>);
    static_assert(is_same_v<volatile tuple_element_t<0, t1>, volatile int>);
    static_assert(is_same_v
                  <const volatile tuple_element_t<0, t1>, const volatile int>);
    static_assert(is_same_v<tuple_element_t<1, t1>, void>);
    static_assert(is_same_v<tuple_element_t<100, t1>, void>);

    static_assert(is_same_v<tuple_element_t<0, t2>, char>);
    static_assert(is_same_v<tuple_element_t<1, t2>, int>);

    static_assert(is_same_v<tuple_element_t<0, t3>, char>);
    static_assert(is_same_v<tuple_element_t<1, t3>, int>);
    static_assert(is_same_v<tuple_element_t<2, t3>, float>);

    static_assert(is_same_v<tuple_element_t<0, t4>, char>);
    static_assert(is_same_v<tuple_element_t<1, t4>, int>);
    static_assert(is_same_v<tuple_element_t<2, t4>, float>);
    static_assert(is_same_v<tuple_element_t<3, t4>, double>);

    static_assert(is_same_v
                  <tuple_element_t<3, const volatile t4>,
                  const volatile double>);
  }

  // common_reference<CVREF tuple, CVREF tuple>
  {
    struct t {};
    struct tt : t {};

    static_assert(same_as
                  <common_reference_t<tuple<int &>, tuple<long>>, tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<int>, tuple<long>>, tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<int>, tuple<const long &>>,
                  tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<t>, tuple<tt>>, tuple<t>>);
    static_assert(same_as
                  <common_reference_t<const tuple<t> &, tuple<tt> &&>,
                  tuple<const t &>>);
    static_assert(same_as
                  <common_reference_t<tuple<const t &>, tuple<tt &&>>,
                  tuple<const t &>>);

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

    static_assert(is_same_v
                  <common_reference_t<tuple<>, tuple<> &>, tuple<>>);
    static_assert(is_same_v
                  <common_reference_t<tuple<> &, tuple<> &>, tuple<> &>);

    static_assert(same_as
                  <common_reference_t<tuple<int &>, tuple<long>>, tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<int>, tuple<long>>, tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<int>, tuple<const long &>>,
                  tuple<long>>);
    static_assert(same_as
                  <common_reference_t<tuple<t>, tuple<tt>>, tuple<t>>);
    static_assert(same_as
                  <common_reference_t<const tuple<t> &, tuple<tt> &&>,
                  tuple<const t &>>);
    static_assert(same_as
                  <common_reference_t
                  <tuple<const t &, int, int>,
                  tuple<tt &&, long, long long &>>,
                  tuple<const t &, long, long long>>);
    static_assert(same_as
                  <common_reference_t
                  <tuple<int, int, const t &>, tuple<long, long long, tt &&>>,
                  tuple<long, long long, const t &>>);
  }

  // common_type<CVREF tuple, CVREF tuple>
  {
    struct t {};
    struct tt : t {};
    static_assert(is_same_v
                  <common_type_t<tuple<int, t *>, tuple<long long, tt *>>,
                  tuple<long long, t *>>);

    static_assert(is_same_v
                  <common_type_t<tuple<>, tuple<> &>, tuple<>>);
    static_assert(is_same_v
                  <common_type_t<tuple<> &, tuple<> &>, tuple<>>);

    static_assert(same_as
                  <common_type_t<tuple<tt, t, tt>, tuple<t, tt, t>>,
                  tuple<t, t, t>>); 
  }

  // get
  // at
  // as
  {
    static_assert(is_trivial_empty_v<decltype(as<int>)>);
    static_assert(is_trivial_empty_v<decltype(at<0>)>);

    const auto f1 = []<class T, class F, class I>(T &t, F f, I i) {
      assert(f(t) == i);
      assert(f((const T &)t) == i);
      assert(f((T &&)t) == i);
      assert(f((const T &&)t) == i);
      static_assert(is_same_v<decltype(f(t)), I &>);
      static_assert(is_same_v<decltype(f((const T &)t)), const I &>);
      static_assert(is_same_v<decltype(f((T &&)t)), I &&>);
      static_assert(is_same_v<decltype(f((const T &&)t)), const I &&>);

      auto &x = f(t);
      x = 0;
      assert(f(t) == 0);
      f(t) = i;
      assert(f(t) == i);
    };

    using t1 = tuple<int>;
    t1 t{1};
    f1(t, at<0>, 1);
    f1(t, as<int>, 1);
    f1(t, []<class T>(T &&x)->decltype(auto) {return re::get<0>((T &&)x);}, 1);
    f1(t, []<class T>(T &&x)->decltype(auto) {return re::get<int>((T &&)x);},
       1);
    f1(t,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<0>();
       },
       1);
    f1(t,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<int>();
       },
       1);
    static_assert(is_invocable_v<decltype(at<0>), t1 &>);
    static_assert(!is_invocable_v<decltype(at<1>), t1 &>);
    static_assert(!is_invocable_v<decltype(at<100>), t1 &>);
    static_assert(is_invocable_v<decltype(as<int>), t1 &>);
    static_assert(!is_invocable_v<decltype(as<int &>), t1 &>);
    static_assert(!is_invocable_v<decltype(as<float>), t1 &>);
    {
      constexpr t1 x(1);
      static_assert(x.as<int>() == 1);
      static_assert(x.at<0>() == 1);
      static_assert(as<int>(x) == 1);
      static_assert(at<0>(x) == 1);
      static_assert(get<int>(x) == 1);
      static_assert(get<0>(x) == 1);
      static_assert(noexcept(x.as<int>()));
      static_assert(noexcept(x.at<0>()));
      static_assert(noexcept(as<int>(x)));
      static_assert(noexcept(at<0>(x)));
      static_assert(noexcept(get<int>(x)));
      static_assert(noexcept(get<0>(x)));
    }

    using t2 = tuple<int, short>;
    t2 tt{1, 2};
    f1(tt, at<0>, 1);
    f1(tt, at<1>, (short)2);
    f1(tt, as<int>, 1);
    f1(tt, as<short>, (short)2);
    f1(tt, []<class T>(T &&x)->decltype(auto) {return re::get<0>((T &&)x);}, 1);
    f1(tt, []<class T>(T &&x)->decltype(auto) {return re::get<int>((T &&)x);},
       1);
    f1(tt, []<class T>(T &&x)->decltype(auto) {return re::get<1>((T &&)x);},
       (short)2);
    f1(tt, []<class T>(T &&x)->decltype(auto) {return re::get<short>((T &&)x);},
       (short)2);
    f1(tt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<0>();
       },
       1);
    f1(tt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<1>();
       },
       (short)2);
    f1(tt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<int>();
       },
       1);
    f1(tt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<short>();
       },
       (short)2);
    static_assert(is_invocable_v<decltype(at<0>), t2 &>);
    static_assert(is_invocable_v<decltype(at<1>), t2 &>);
    static_assert(!is_invocable_v<decltype(at<2>), t2 &>);
    static_assert(is_invocable_v<decltype(as<int>), t2 &>);
    static_assert(is_invocable_v<decltype(as<short>), t2 &>);
    static_assert(!is_invocable_v<decltype(as<float>), t2 &>);
    {
      constexpr t2 x(1, 2);
      static_assert(x.as<int>() == 1);
      static_assert(x.at<0>() == 1);
      static_assert(as<int>(x) == 1);
      static_assert(at<0>(x) == 1);
      static_assert(get<int>(x) == 1);
      static_assert(get<0>(x) == 1);
      static_assert(noexcept(x.as<int>()));
      static_assert(noexcept(x.at<0>()));
      static_assert(noexcept(as<int>(x)));
      static_assert(noexcept(at<0>(x)));
      static_assert(noexcept(get<int>(x)));
      static_assert(noexcept(get<0>(x)));
      static_assert(x.as<short>() == 2);
      static_assert(x.at<1>() == 2);
      static_assert(as<short>(x) == 2);
      static_assert(at<1>(x) == 2);
      static_assert(get<short>(x) == 2);
      static_assert(get<1>(x) == 2);
      static_assert(noexcept(x.as<short>()));
      static_assert(noexcept(x.at<1>()));
      static_assert(noexcept(as<short>(x)));
      static_assert(noexcept(at<1>(x)));
      static_assert(noexcept(get<short>(x)));
      static_assert(noexcept(get<1>(x)));
    }

    using t3 = tuple<int, short, long>;
    t3 ttt{1, 2, 3};
    f1(ttt, at<0>, 1);
    f1(ttt, at<1>, (short)2);
    f1(ttt, at<2>, (long)3);
    f1(ttt, as<int>, 1);
    f1(ttt, as<short>, (short)2);
    f1(ttt, as<long>, (long)3);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<0>((T &&)x);},
       1);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<1>((T &&)x);},
       (short)2);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<2>((T &&)x);},
       (long)3);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<int>((T &&)x);},
       1);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<short>((T &&)x);},
       (short)2);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {return re::get<long>((T &&)x);},
       (long)3);

    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<0>();
       },
       1);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<1>();
       },
       (short)2);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template at<2>();
       },
       (long)3);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<int>();
       },
       1);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<short>();
       },
       (short)2);
    f1(ttt,
       []<class T>(T &&x)->decltype(auto) {
         return forward<T>(x).template as<long>();
       },
       (long)3);
    {
      constexpr t3 x(1, 2, 3);
      static_assert(x.as<int>() == 1);
      static_assert(x.at<0>() == 1);
      static_assert(as<int>(x) == 1);
      static_assert(at<0>(x) == 1);
      static_assert(get<int>(x) == 1);
      static_assert(get<0>(x) == 1);
      static_assert(noexcept(x.as<int>()));
      static_assert(noexcept(x.at<0>()));
      static_assert(noexcept(as<int>(x)));
      static_assert(noexcept(at<0>(x)));
      static_assert(noexcept(get<int>(x)));
      static_assert(noexcept(get<0>(x)));
      static_assert(x.as<short>() == 2);
      static_assert(x.at<1>() == 2);
      static_assert(as<short>(x) == 2);
      static_assert(at<1>(x) == 2);
      static_assert(get<short>(x) == 2);
      static_assert(get<1>(x) == 2);
      static_assert(noexcept(x.as<short>()));
      static_assert(noexcept(x.at<1>()));
      static_assert(noexcept(as<short>(x)));
      static_assert(noexcept(at<1>(x)));
      static_assert(noexcept(get<short>(x)));
      static_assert(noexcept(get<1>(x)));
      static_assert(x.as<long>() == 3);
      static_assert(x.at<2>() == 3);
      static_assert(as<long>(x) == 3);
      static_assert(at<2>(x) == 3);
      static_assert(get<long>(x) == 3);
      static_assert(get<2>(x) == 3);
      static_assert(noexcept(x.as<long>()));
      static_assert(noexcept(x.at<2>()));
      static_assert(noexcept(as<long>(x)));
      static_assert(noexcept(at<2>(x)));
      static_assert(noexcept(get<long>(x)));
      static_assert(noexcept(get<2>(x)));
    }

    using t5 = tuple<int, short, long, long long, unsigned long long>;
    t5 tfive{1, 2, 3, 4, 5};
    assert(at<0>(tfive) == 1);
    assert(at<1>(tfive) == 2);
    assert(at<2>(tfive) == 3);
    assert(at<3>(tfive) == 4);
    assert(at<4>(tfive) == 5);
    assert(as<int>(tfive) == 1);
    assert(as<short>(tfive) == 2);
    assert(as<long>(tfive) == 3);
    assert(as<long long>(tfive) == 4);
    assert(as<unsigned long long>(tfive) == 5);

    {
      int a = 1;
      tuple<const int &&> tx(move(as_const(a)));
      static_assert(same_as<decltype(at<0>(tx)), const int &>);
      static_assert(same_as<decltype(at<0>(move(tx))), const int &&>);
      static_assert(same_as<decltype(at<0>(as_const(tx))), const int &>);
      static_assert(same_as<decltype(at<0>(move(as_const(tx)))), const int &&>);
      assert(at<0>(tx) == 1);
      assert(at<0>(move(tx)) == 1);
      assert(at<0>(as_const(tx)) == 1);
      assert(at<0>(move(as_const(tx))) == 1);
      assert(addressof(at<0>(tx)) == &a);
      assert(addressof(at<0>(move(tx))) == &a);
      assert(addressof(at<0>(as_const(tx))) == &a);
      assert(addressof(at<0>(move(as_const(tx)))) == &a);
    }
    {
      int a = 1;
      short b = 2;
      long c = 3;
      tuple<int &, short &&, const long &&> tx(a, move(b), move(as_const(c)));
      static_assert(same_as<decltype(at<0>(tx)), int &>);
      static_assert(same_as<decltype(at<0>(move(tx))), int &>);
      static_assert(same_as<decltype(at<0>(as_const(tx))), int &>);
      static_assert(same_as<decltype(at<0>(move(as_const(tx)))), int &>);
      static_assert(same_as<decltype(get<0>(tx)), int &>);
      static_assert(same_as<decltype(get<0>(move(tx))), int &>);
      static_assert(same_as<decltype(get<0>(as_const(tx))), int &>);
      static_assert(same_as<decltype(get<0>(move(as_const(tx)))), int &>);
      assert(at<0>(tx) == 1);
      assert(at<0>(move(tx)) == 1);
      assert(at<0>(as_const(tx)) == 1);
      assert(at<0>(move(as_const(tx))) == 1);
      assert(addressof(at<0>(tx)) == &a);
      assert(addressof(at<0>(move(tx))) == &a);
      assert(addressof(at<0>(as_const(tx))) == &a);
      assert(addressof(at<0>(move(as_const(tx)))) == &a);
      static_assert(same_as<decltype(at<1>(tx)), short &>);
      static_assert(same_as<decltype(at<1>(move(tx))), short &&>);
      static_assert(same_as<decltype(at<1>(as_const(tx))), short &>);
      static_assert(same_as<decltype(at<1>(move(as_const(tx)))), short &&>);
      static_assert(same_as<decltype(get<1>(tx)), short &>);
      static_assert(same_as<decltype(get<1>(move(tx))), short &&>);
      static_assert(same_as<decltype(get<1>(as_const(tx))), short &>);
      static_assert(same_as<decltype(get<1>(move(as_const(tx)))), short &&>);
      assert(at<1>(tx) == 2);
      assert(at<1>(move(tx)) == 2);
      assert(at<1>(as_const(tx)) == 2);
      assert(at<1>(move(as_const(tx))) == 2);
      assert(addressof(at<1>(tx)) == &b);
      assert(addressof(at<1>(as_lvalue(move(tx)))) == &b);
      assert(addressof(at<1>(as_const(tx))) == &b);
      assert(addressof(at<1>(as_lvalue(move(as_const(tx))))) == &b);
      static_assert(same_as<decltype(at<2>(tx)), const long &>);
      static_assert(same_as<decltype(at<2>(move(tx))), const long &&>);
      static_assert(same_as<decltype(at<2>(as_const(tx))), const long &>);
      static_assert(same_as
                    <decltype(at<2>(move(as_const(tx)))), const long &&>);
      static_assert(same_as<decltype(get<2>(tx)), const long &>);
      static_assert(same_as<decltype(get<2>(move(tx))), const long &&>);
      static_assert(same_as<decltype(get<2>(as_const(tx))), const long &>);
      static_assert(same_as
                    <decltype(get<2>(move(as_const(tx)))), const long &&>);
      assert(at<2>(tx) == 3);
      assert(at<2>(move(tx)) == 3);
      assert(at<2>(as_const(tx)) == 3);
      assert(at<2>(move(as_const(tx))) == 3);
      assert(addressof(at<2>(tx)) == &c);
      assert(addressof(at<2>(as_lvalue(move(tx)))) == &c);
      assert(addressof(at<2>(as_const(tx))) == &c);
      assert(addressof(at<2>(as_lvalue(move(as_const(tx))))) == &c);
    }
    {
      int a = 1;
      short b = 2;
      tuple<int &, short &&> tx(a, move(b));
      static_assert(same_as<decltype(at<0>(tx)), int &>);
      static_assert(same_as<decltype(at<0>(move(tx))), int &>);
      static_assert(same_as<decltype(at<0>(as_const(tx))), int &>);
      static_assert(same_as<decltype(at<0>(move(as_const(tx)))), int &>);
      static_assert(same_as<decltype(get<0>(tx)), int &>);
      static_assert(same_as<decltype(get<0>(move(tx))), int &>);
      static_assert(same_as<decltype(get<0>(as_const(tx))), int &>);
      static_assert(same_as<decltype(get<0>(move(as_const(tx)))), int &>);
      assert(at<0>(tx) == 1);
      assert(at<0>(move(tx)) == 1);
      assert(at<0>(as_const(tx)) == 1);
      assert(at<0>(move(as_const(tx))) == 1);
      assert(addressof(at<0>(tx)) == &a);
      assert(addressof(at<0>(move(tx))) == &a);
      assert(addressof(at<0>(as_const(tx))) == &a);
      assert(addressof(at<0>(move(as_const(tx)))) == &a);
      static_assert(same_as<decltype(at<1>(tx)), short &>);
      static_assert(same_as<decltype(at<1>(move(tx))), short &&>);
      static_assert(same_as<decltype(at<1>(as_const(tx))), short &>);
      static_assert(same_as<decltype(at<1>(move(as_const(tx)))), short &&>);
      static_assert(same_as<decltype(get<1>(tx)), short &>);
      static_assert(same_as<decltype(get<1>(move(tx))), short &&>);
      static_assert(same_as<decltype(get<1>(as_const(tx))), short &>);
      static_assert(same_as<decltype(get<1>(move(as_const(tx)))), short &&>);
      assert(at<1>(tx) == 2);
      assert(at<1>(move(tx)) == 2);
      assert(at<1>(as_const(tx)) == 2);
      assert(at<1>(move(as_const(tx))) == 2);
      assert(at<1>(tx) == 2);
      assert(at<1>(move(tx)) == 2);
      assert(at<1>(as_const(tx)) == 2);
      assert(at<1>(move(as_const(tx))) == 2);
    }

    {
      long i{};
      tuple<int, int, int, int, const long &&, int> x(0, 0, 0, 0, move(i), 0);
      assert(at<4>(x) == 0);
      static_assert(same_as<decltype(at<4>(x)), const long &>);
      static_assert(same_as<decltype(at<4>(move(x))), const long &&>);
      static_assert(same_as<decltype(as<const long &&>(x)), const long &>);
      static_assert(same_as
                    <decltype(as<const long &&>(move(x))), const long &&>);
    }
  }

  // forward_as_tuple
  {
    static_assert(is_trivial_empty_v<decltype(forward_as_tuple)>);

    {
      static_assert(noexcept(forward_as_tuple()));
      static_assert(noexcept(forward_as_tuple(1, 2, 3)));
    }
    {
      const auto f = []() {return 0;};
      const auto static_f = []() constexpr {return 0;};
      static_assert(same_as<decltype(forward_as_tuple()), tuple<>>);
      assert(apply(f, forward_as_tuple()) == 0);
      static_assert(apply(static_f, forward_as_tuple()) == 0);
    }
    {
      static_assert(same_as
                    <decltype(forward_as_tuple(1)),
                    tuple<int &&>>);
      const auto is_1 = [](auto x) {return x == 1;};
      const auto static_is_1 = [](auto x) constexpr {return x == 1;};
      assert(apply(static_is_1, forward_as_tuple(1)));
      static_assert(apply(static_is_1, forward_as_tuple(1)));

      int x = 1;
      static_assert(same_as
                    <decltype(forward_as_tuple(x)), tuple<int &>>);
      assert(apply(is_1, forward_as_tuple(x)));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(x))), tuple<int &&>>);
      assert(apply(is_1, forward_as_tuple(move(x))));
      static_assert(same_as
                    <decltype(forward_as_tuple(as_const(x))),
                    tuple<const int &>>);
      assert(apply(is_1, forward_as_tuple(as_const(x))));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(as_const(x)))),
                    tuple<const int &&>>);
      assert(apply(is_1, forward_as_tuple(move(as_const(x)))));
    }
    {
      static_assert(same_as
                    <decltype(forward_as_tuple(1, 2)),
                    tuple<int &&, int &&>>);
      const auto is_12 = [](auto x, auto y) {return x == 1 && y == 2;};
      const auto static_is_12 = [](auto x, auto y) constexpr {
        return x == 1 && y == 2;
      };
      assert(apply(static_is_12, forward_as_tuple(1, 2)));
      static_assert(apply(static_is_12, forward_as_tuple(1, 2)));

      int x = 1;
      short y = 2;
      static_assert(same_as
                    <decltype(forward_as_tuple(x, y)), tuple<int &, short &>>);
      assert(apply(is_12, forward_as_tuple(x, y)));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(x), move(y))),
                    tuple<int &&, short &&>>);
      assert(apply(is_12,
                   forward_as_tuple(move(x), move(y))));
      static_assert(same_as
                    <decltype(forward_as_tuple(as_const(x), as_const(y))),
                    tuple<const int &, const short &>>);
      assert(apply(is_12,
                   forward_as_tuple(as_const(x), as_const(y))));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(as_const(x)),
                                               move(as_const(y)))),
                    tuple<const int &&, const short &&>>);
      assert(apply(is_12, forward_as_tuple(move(as_const(x)),
                                           move(as_const(y)))));
    }
    {
      static_assert(same_as
                    <decltype(forward_as_tuple(1, 2, 3)),
                    tuple<int &&, int &&, int &&>>);
      const auto is_123 = [](auto x, auto y, auto z) {
        return x == 1 && y == 2 && z == 3;
      };
      const auto static_is_123 = [](auto x, auto y, auto z) constexpr {
        return x == 1 && y == 2 && z == 3;
      };
      assert(apply(is_123, forward_as_tuple(1, 2, 3)));
      static_assert(apply(static_is_123, forward_as_tuple(1, 2, 3)));

      int x = 1;
      short y = 2;
      long z = 3;
      static_assert(same_as
                    <decltype(forward_as_tuple(x, y, z)),
                    tuple<int &, short &, long &>>);
      assert(apply(is_123, forward_as_tuple(x, y, z)));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(x), move(y), move(z))),
                    tuple<int &&, short &&, long &&>>);
      assert(apply(is_123,
                   forward_as_tuple(move(x), move(y), move(z))));
      static_assert(same_as
                    <decltype(forward_as_tuple(as_const(x),
                                               as_const(y),
                                               as_const(z))),
                    tuple<const int &, const short &, const long &>>);
      assert(apply(is_123,
                   forward_as_tuple(as_const(x), as_const(y), as_const(z))));
      static_assert(same_as
                    <decltype(forward_as_tuple(move(as_const(x)),
                                               move(as_const(y)),
                                               move(as_const(z)))),
                    tuple<const int &&, const short &&, const long &&>>);
      assert(apply(is_123,
                   forward_as_tuple(move(as_const(x)),
                                    move(as_const(y)),
                                    move(as_const(z)))));
    }

    {
      owner_pointer<int> c(0);
      assert(c != nullptr && *c == 0);
      auto p = forward_as_tuple(move(c));
      assert(c != nullptr && *c == 0);
      tuple<owner_pointer<int>> pp = move(p);
      assert(c == nullptr);
      assert(at<0>(pp) != nullptr && *at<0>(pp) == 0);
    }
    {
      owner_pointer<void *> c1((void *)nullptr);
      owner_pointer<int> c2(0);
      assert(c1 != nullptr && *c1 == nullptr);
      assert(c2 != nullptr && *c2 == 0);
      auto p = forward_as_tuple(move(c1), move(c2));
      assert(c1 != nullptr && *c1 == nullptr);
      assert(c2 != nullptr && *c2 == 0);
      tuple<decltype(c1), decltype(c2)> pp = move(p);
      assert(c1 == nullptr && c2 == nullptr);
      assert(at<0>(pp) != nullptr && *at<0>(pp) == nullptr);
      assert(at<1>(pp) != nullptr && *at<1>(pp) == 0);
    }
    {
      owner_pointer<void *> c1((void *)nullptr);
      owner_pointer<int> c2(0);
      owner_pointer<double> c3(1.5);
      assert(c1 != nullptr && *c1 == nullptr);
      assert(c2 != nullptr && *c2 == 0);
      assert(c3 != nullptr && *c3 == 1.5);
      auto p = forward_as_tuple(move(c1), move(c2), move(c3));
      assert(c1 != nullptr && *c1 == nullptr);
      assert(c2 != nullptr && *c2 == 0);
      assert(c3 != nullptr && *c3 == 1.5);
      tuple<decltype(c1), decltype(c2), decltype(c3)> pp = move(p);
      assert(c1 == nullptr && c2 == nullptr && c3 == nullptr);
      assert(at<0>(pp) != nullptr && *at<0>(pp) == nullptr);
      assert(at<1>(pp) != nullptr && *at<1>(pp) == 0);
      assert(at<2>(pp) != nullptr && *at<2>(pp) == 1.5);
    }
  }

  // uses_allocator<tuple<...>, AL>
  // uses_allocator_construction_args
  help_tuple::test_uses_allocator_construction_args();

  // make_tuple
  // make_pair
  {
    static_assert(is_trivial_empty_v<decltype(make_tuple)>);
    static_assert(is_trivial_empty_v<decltype(make_pair)>);

    static_assert(!noexcept(make_tuple()));
    static_assert(!noexcept(make_tuple(1, 2, 3, 4, 5)));
    static_assert(!noexcept(make_pair(1, 2)));

    const auto t = make_tuple(1, 2, 3);
    const int i = 0;
    const auto t2 = make_pair(1, ref(i));
    assert(t == make_tuple(1, 2, 3));
    assert(t2 == make_tuple(1, 0));
    static_assert(is_same_v<decltype(t), const tuple<int, int, int>>);
    static_assert(is_same_v<decltype(t2), const tuple<int, const int &>>);

    const auto t3 = make_tuple(allocator_arg, 1);
    assert(tuple_size_v<decltype(t3)> == 2);
    assert(t3.at<1>() == 1);
    static_assert(is_same_v<decltype(t3), const tuple<allocator_arg_t, int>>);

    const auto t4 = make_tuple();
    assert(tuple_size_v<decltype(t4)> == 0);
    static_assert(is_same_v<decltype(t4), const tuple<>>);

    int x = 1;
    const int y = 2;
    int z = 3;
    const auto t5 = make_tuple(ref(x), ref(y), cref(z));
    assert(t5 == make_tuple(1, 2, 3));
    static_assert(is_same_v
                  <decltype(t5), const tuple<int &, const int &, const int &>>);

    {
      static_assert(make_pair(1, 2) == pair(1, 2));
      constexpr int a = 1, b = 2;
      static_assert(make_pair(ref(a), ref(b)) == pair(1, 2));
    }
    {
      static_assert(make_tuple(1, 2, 3) == tuple(1, 2, 3));
      constexpr int a = 1, b = 2, c = 3;
      static_assert(make_tuple(ref(a), ref(b), ref(c)) == tuple(1, 2, 3));
    }
  }

  // tie
  {
    {
      int i, j;
      static_assert(is_trivial_empty_v<decltype(tie)>);
      static_assert(noexcept(tie(i, j)));
      tie(i, j) = make_tuple(1, 2);
      assert(i == 1 && j == 2);
      tie(ignore, ignore) = make_tuple(1, 2);
    }
    {
      tuple<int> x{1};
      int i = 0;

      tie(ignore) = x;
      assert(i == 0 && at<0>(x) == 1);

      tie(i) = x;
      assert(i == 1 && at<0>(x) == 1);
      assert(addressof(i) != addressof(at<0>(x)));
      i = 2;
      assert(i == 2 && at<0>(x) == 1);
    }
    {
      tuple<int, unsigned> x{1, 2};
      int i = 0;
      unsigned j = 0;
      unsigned long k = 0;
      tie(ignore, ignore) = x;
      assert(to_unsigned(i) == j && i == 0 && at<0>(x) == 1 && at<1>(x) == 2);
      tie(ignore, j) = x;
      assert(j == 2);
      tie(j, ignore) = x;
      assert(j == 1);
      tie(ignore, k) = x;
      assert(k == 2);
      tie(i, j) = x;
      assert(i == 1 && j == 2);
    }
    {
      tuple<short, unsigned short, int, unsigned, long, unsigned long> x{};
      unsigned long i = 1;
      tie(ignore, ignore, i, ignore, ignore, ignore) = x;
      assert(i == 0);
    }

    {
      constexpr auto f = []() constexpr->pair<int, double> {
        int x;
        double y;
        tie(x, y) = pair(1, 2);
        return pair(x, y);
      };
      constexpr auto ff = []() constexpr->tuple<int, double, long> {
        int x;
        double y;
        long z;
        tie(x, y, z) = tuple(1, 2, 3);
        return tuple(x, y, z);
      };
      static_assert(f() == pair(1, 2));
      static_assert(ff() == tuple(1, 2, 3));
    }
  }

  // tuple_cat
  {
    static_assert(is_trivial_empty_v<decltype(tuple_cat)>);
    static_assert(!noexcept(tuple_cat(tuple(1, 2), tuple(3, 4))));

    static_assert(same_as
                  <inner::tuple_cat_result_t
                  <const tuple<int, int *>,
                  tuple<int, int *> &&,
                  const tuple<int, int *> &&,
                  tuple<int, int *> &>,
                  tuple<int, int *, int, int *, int, int *, int, int *>>);

    static_assert(tuple_cat(pair(1, 2), tuple(3), pair(4, 5), tuple(6, 7, 8))
                  == tuple(1, 2, 3, 4, 5, 6, 7, 8));

    assert(make_tuple() == tuple_cat());
    assert(make_tuple() == tuple_cat(make_tuple(), make_tuple(),
                                     make_tuple(), make_tuple(),
                                     make_tuple(), make_tuple()));

    assert(make_tuple(1) == tuple_cat(make_tuple(1)));
    assert(make_tuple(1)
           == tuple_cat(make_tuple(), make_tuple(1), make_tuple()));

    assert(make_tuple(1, 2) == tuple_cat(make_tuple(1), make_tuple(2)));

    assert(make_tuple(1, 2, 3, 4, 5)
           == tuple_cat(make_tuple(1, 2, 3), make_tuple(4, 5)));
    assert(make_tuple(1, 2, 3, 4, 5, 6, 7, 8)
           == tuple_cat(make_tuple(1, 2), make_tuple(),
                        make_tuple(3, 4, 5, 6), make_tuple(7, 8)));

    {
      const tuple<int, int> x{1, 2};
      const tuple<int, int, int> y{3, 4, 5};
      auto z = tuple_cat(x, y);
      static_assert(is_same_v<decltype(z), tuple<int, int, int, int, int>>);
      assert(z == make_tuple(1, 2, 3, 4, 5));
    }

    {
      tuple<owner_pointer<int>> x{1};
      assert(*at<0>(x) == 1);
      auto y = tuple_cat(x);
      assert(*at<0>(y) == 1 && *at<0>(x) == 1);
      auto z = tuple_cat(move(x));
      assert(*at<0>(z) == 1 && at<0>(x) == nullptr);
    }
    {
      tuple<owner_pointer<int>, owner_pointer<long>> x{1, 2};
      tuple<owner_pointer<int>, owner_pointer<long>> y{3, 4};
      tuple<owner_pointer<int>, owner_pointer<long>> z{5, 6};
      auto zz = tuple_cat(move(x), move(y), move(z));
      assert(at<0>(x) == nullptr && at<1>(x) == nullptr);
      assert(at<0>(y) == nullptr && at<1>(y) == nullptr);
      assert(at<0>(z) == nullptr && at<1>(z) == nullptr);
      assert(*at<0>(zz) == 1);
      assert(*at<1>(zz) == 2);
      assert(*at<2>(zz) == 3);
      assert(*at<3>(zz) == 4);
      assert(*at<4>(zz) == 5);
      assert(*at<5>(zz) == 6);
    }
    {
      using t = tuple<owner_pointer<int>, owner_pointer<long>,
                       owner_pointer<long long>>;
      t x{1, 2, 3};
      t y{4, 5, 6};
      t z{7, 8, 9};
      auto zz = tuple_cat(x, move(y), z);
      assert(*at<0>(x) == 1 && *at<1>(x) == 2 && *at<2>(x) == 3);
      assert(at<0>(y) == nullptr && at<1>(y) == nullptr && at<2>(y) == nullptr);
      assert(*at<0>(z) == 7 && *at<1>(z) == 8 && *at<2>(z) == 9);
      assert(*at<0>(zz) == 1);
      assert(*at<1>(zz) == 2);
      assert(*at<2>(zz) == 3);
      assert(*at<3>(zz) == 4);
      assert(*at<4>(zz) == 5);
      assert(*at<5>(zz) == 6);
      assert(*at<6>(zz) == 7);
      assert(*at<7>(zz) == 8);
      assert(*at<8>(zz) == 9);
    }
  }

  // apply
  {
    static_assert(is_trivial_empty_v<decltype(apply)>);
    static_assert(!noexcept(apply([]() noexcept {}, tuple{})));

    assert(apply([]() {return 1;}, tuple<>{}) == 1);
    assert(apply([](auto x, auto y, auto z) {return x + y + z;},
                 make_tuple(1, 2, 3)) == 6);
    {
      owner_pointer<int> a(1);
      owner_pointer<int> aa;
      apply([&]<class X>(X &&x){aa = forward<X>(x);},
            forward_as_tuple(a));
      assert(a != nullptr && *a == 1);
      assert(aa != nullptr && *aa == 1);
      apply([&]<class X>(X &&x){aa = forward<X>(x);},
            forward_as_tuple(move(a)));
      assert(a == nullptr);
      assert(aa != nullptr && *aa == 1);
    }
    {
      owner_pointer<int> a(1), b(2);
      owner_pointer<int> aa, bb;
      apply([&]<class X, class Y>(X &&x, Y &&y)
            {aa = forward<X>(x); bb = forward<Y>(y);},
            forward_as_tuple(a, b));
      assert(a != nullptr && b != nullptr && *a == 1 && *b == 2);
      assert(aa != nullptr && bb != nullptr && *aa == 1 && *bb == 2);
      apply([&]<class X, class Y>(X &&x, Y &&y)
            {aa = forward<X>(x); bb = forward<Y>(y);},
            forward_as_tuple(move(a), move(b)));
      assert(a == nullptr && b == nullptr);
      assert(aa != nullptr && bb != nullptr && *aa == 1 && *bb == 2);
    }
    {
      owner_pointer<int> a(1), b(2), c(3);
      owner_pointer<int> aa, bb, cc;
      apply([&]<class X, class Y, class Z>(X &&x, Y &&y, Z &&z)
            {aa = forward<X>(x); bb = forward<Y>(y); cc = forward<Z>(z);},
            forward_as_tuple(a, b, c));
      assert(a != nullptr && b != nullptr && c != nullptr);
      assert(*a == 1 && *b == 2 && *c == 3);
      assert(aa != nullptr && bb != nullptr && cc != nullptr);
      assert(*aa == 1 && *bb == 2 && *cc == 3);
      apply([&]<class X, class Y, class Z>(X &&x, Y &&y, Z &&z)
            {aa = forward<X>(x); bb = forward<Y>(y); cc = forward<Z>(z);},
            forward_as_tuple(move(a), move(b), move(c)));
      assert(a == nullptr && b == nullptr && c == nullptr);
      assert(aa != nullptr && bb != nullptr && cc != nullptr);
      assert(*aa == 1 && *bb == 2 && *cc == 3);
    }
    {
      struct t {
        int operator ()(int &&x) && {return x * 2;}
      };
      assert(apply(t{}, forward_as_tuple(1)) == 2);
    }
    {
      assert(apply([](int x, int *y, int **z, int **zz, int ***zzz) {return 1;},
                   tuple_cat(tuple(1), tuple<int *, int **>({}, {}),
                             tuple<int **, int ***>(nullptr, nullptr)))
             == 1);
    }

    static_assert(apply([](auto x, auto y) {return x + y;}, tuple(1, 2)) == 3);
    static_assert(apply([](auto x, auto y, auto z) {return x + y + z;},
                        tuple(1, 2, 3)) == 6);
  }

  // make_from_tuple
  {
    static_assert(is_trivial_empty_v<decltype(make_from_tuple<int>)>);
    static_assert(!noexcept(make_from_tuple<int>(tuple{})));

    struct t0 {
      t0() {}
    };
    t0 x{make_from_tuple<t0>(tuple<>())};

    struct t {
      int i;
      int j;

      t(int &&i, int &&j) : i(i + 1), j(j + 1) {}
      t(const int &i, const int &j) : i(i), j(j) {}
    };
    t y = make_from_tuple<t>(make_tuple(1, 2));
    assert(y.i == 2 && y.j == 3);
    t yy = make_from_tuple<t>(as_lvalue(make_tuple(1, 2)));
    assert(yy.i == 1 && yy.j == 2);

    static_assert(make_from_tuple<tuple<int>>(tuple(1)) == tuple(1));
    static_assert(make_from_tuple<tuple<int, float>>(tuple(1, 2))
                  == tuple(1, 2));
    static_assert(make_from_tuple<tuple<int, float, long>>(tuple(1, 2, 3))
                  == tuple(1, 2, 3));

    {
      owner_pointer<int> a(1);
      tuple<owner_pointer<int>> p
        = make_from_tuple<tuple<owner_pointer<int>>>
        (forward_as_tuple(move(a)));
      assert(a == nullptr);
      assert(at<0>(p) != nullptr && *at<0>(p) == 1);
    }
    {
      owner_pointer<int> a(1), b(2);
      tuple<owner_pointer<int>, owner_pointer<int>> p
        = make_from_tuple<tuple<owner_pointer<int>, owner_pointer<int>>>
        (forward_as_tuple(move(a), move(b)));
      assert(a == nullptr && b == nullptr);
      assert(at<0>(p) != nullptr && *at<0>(p) == 1);
      assert(at<1>(p) != nullptr && *at<1>(p) == 2);
    }
    {
      owner_pointer<int> a(1), b(2), c(3);
      tuple<owner_pointer<int>,
            owner_pointer<int>,
            owner_pointer<int>> p
        = make_from_tuple<tuple<owner_pointer<int>,
                                owner_pointer<int>,
                                owner_pointer<int>>>
        (forward_as_tuple(move(a), move(b), move(c)));
      assert(a == nullptr && b == nullptr && c == nullptr);
      assert(at<0>(p) != nullptr && *at<0>(p) == 1);
      assert(at<1>(p) != nullptr && *at<1>(p) == 2);
      assert(at<2>(p) != nullptr && *at<2>(p) == 3);
    }
  }

  // tuple:: special member functions / swap
  {
    {
      static_assert(regular<tuple<>>);
      static_assert(is_nothrow_constructible_v<tuple<>>);
      static_assert(is_nothrow_destructible_v<tuple<>>);
      static_assert(is_nothrow_copy_constructible_v<tuple<>>);
      static_assert(is_nothrow_copy_assignable_v<tuple<>>);
      static_assert(is_nothrow_move_constructible_v<tuple<>>);
      static_assert(is_nothrow_move_assignable_v<tuple<>>);
      static_assert(is_nothrow_swappable_v<tuple<>>);
      static_assert(is_nothrow_swappable_v<const tuple<>>);
      static_assert(is_trivially_constructible_v<tuple<>>);
      static_assert(is_trivially_destructible_v<tuple<>>);
      static_assert(is_trivially_copy_constructible_v<tuple<>>);
      static_assert(is_trivially_copy_assignable_v<tuple<>>);
      static_assert(is_trivially_move_constructible_v<tuple<>>);
      static_assert(is_trivially_move_assignable_v<tuple<>>);

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_default_constructible, const int, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_default_constructible, const int &, false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_default_constructible, int &&, false>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_destructible, int, true>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_default_constructible, char,
                    false, false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_default_constructible, tuple<int>,
                    false, false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_implicitly_default_constructible, char,
                    true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_implicitly_default_constructible, long,
                    true>());
      struct t {explicit t() {}};
      inner::fns::help_tuple::test_conditional_smf
          <is_implicitly_default_constructible, t, false>();
      inner::fns::help_tuple::test_conditional_smf
          <is_nothrow_default_constructible, char, true>();
      inner::fns::help_tuple::test_conditional_smf
          <is_nothrow_default_constructible, t, false>();
      inner::fns::help_tuple::test_conditional_smf
          <is_default_constructible, reference_wrapper<int>, false>();

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_copy_constructible, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_copy_constructible,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_copy_constructible, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_copy_constructible,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_copy_constructible,
                    inner::fns::help_tuple::only_moveable_t,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_copy_constructible,
                    inner::fns::help_tuple::unmoveable_t,
                    false>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_copy_assignable, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_copy_assignable,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_copy_assignable, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_copy_assignable,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_copy_assignable,
                    inner::fns::help_tuple::only_moveable_t,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_copy_assignable,
                    inner::fns::help_tuple::unmoveable_t,
                    false>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_move_constructible, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_move_constructible,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_move_constructible, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_move_constructible,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_move_constructible,
                    inner::fns::help_tuple::only_moveable_t,
                    true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_move_constructible,
                    inner::fns::help_tuple::unmoveable_t,
                    false>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_move_assignable, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_trivially_move_assignable,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_move_assignable, char, true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_move_assignable,
                    owner_pointer<int>,
                    false>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_move_assignable,
                    inner::fns::help_tuple::only_moveable_t,
                    true>());
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_move_assignable,
                    inner::fns::help_tuple::unmoveable_t,
                    false>());

      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_swappable, char, true>());
      struct tt {
        tt(tt &&) {}
        tt &operator =(tt &&) {return *this;}
      };
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_swappable, tt, false>());
      struct ttt {
        ttt(ttt &&) = delete;
        ttt &operator =(ttt &&) = delete;
      };
      static_assert(inner::fns::help_tuple::test_conditional_smf
                    <is_nothrow_swappable, ttt, false>());

      {
        constexpr auto f = []() {tuple<> t{}; return true;};
        static_assert(f());
      }
      {
        constexpr auto f = []() {tuple<int> t{}; return true;};
        static_assert(f());
      }
      {
        constexpr auto f = []() {tuple<int, int> t{}; return true;};
        static_assert(f());
      }
      {
        constexpr auto f = []() {tuple<int, int, int> t{}; return true;};
        static_assert(f());
      }
    }

    {
      using t = tuple<int, owner_pointer<int>, int>;
      static_assert(!is_nothrow_default_constructible_v<t>);
      t a[500]{};
      for (size_t i = 0; i != 500; ++i) {
        assert(at<0>(a[i]) == 0);
        assert(at<1>(a[i]) == nullptr);
        assert(at<2>(a[i]) == 0);
      }
    }
    {
      using t = tuple<int, owner_pointer<int>>;
      t a[500]{};
      for (size_t i = 0; i != 500; ++i) {
        assert(at<0>(a[i]) == 0);
        assert(at<1>(a[i]) == nullptr);
      }
    }
    {
      using t = tuple<int>;
      t a[500]{};
      for (size_t i = 0; i != 500; ++i)
        assert(at<0>(a[i]) == 0);
    }

    {
      tuple<int, int> t{};
      tuple<int, int> tt(t);
      assert(at<0>(tt) == 0);
      assert(at<1>(tt) == 0);
    }
    {
      tuple<int, int, int> t{};
      tuple<int, int, int> tt(t);
      assert(at<0>(tt) == 0);
      assert(at<1>(tt) == 0);
      assert(at<2>(tt) == 0);
    }
    {
      int i, j;
      tuple<int &, int &> t{i, j};
      at<0>(t) = 1;
      at<1>(t) = 2;
      assert(i == 1 && j == 2);

      int k, s;
      tuple<int &, int &> tt{k, s};
      tt = t;
      assert(i == 1 && j == 2);
      assert(k == 1 && s == 2);
    }
    {
      int i, j, k;
      tuple<int &, int &, int &> t{i, j, k};
      at<0>(t) = 1;
      at<1>(t) = 2;
      at<2>(t) = 3;
      assert(i == 1 && j == 2 && k == 3);
      int a, b, c;
      tuple<int &, int &, int &> tt{a, b, c};
      tt = t;
      assert(i == 1 && j == 2 && k == 3);
      assert(a == 1 && b == 2 && c == 3);
    }
    {
      using cpt = owner_pointer<int>;
      tuple<cpt, cpt, cpt> t{1, 2, 3};
      tuple<cpt, cpt, cpt> tt{};
      tt = move(t);
      assert(at<0>(t) == nullptr);
      assert(at<1>(t) == nullptr);
      assert(at<2>(t) == nullptr);
      assert(*at<0>(tt) == 1);
      assert(*at<1>(tt) == 2);
      assert(*at<2>(tt) == 3);
    }
    {
      using cpt = owner_pointer<int>;
      tuple<cpt, cpt> t{1, 2};
      tuple<cpt, cpt> tt{};
      tt = move(t);
      assert(at<0>(t) == nullptr);
      assert(at<1>(t) == nullptr);
      assert(*at<0>(tt) == 1);
      assert(*at<1>(tt) == 2);
    }
    {
      using cpt = owner_pointer<int>;
      tuple<cpt, cpt, cpt> t{1, 2, 3};
      tuple<cpt, cpt, cpt> tt{4, 5, 6};
      adl_swap(t, tt);
      assert(*at<0>(t) == 4);
      assert(*at<1>(t) == 5);
      assert(*at<2>(t) == 6);
      assert(*at<0>(tt) == 1);
      assert(*at<1>(tt) == 2);
      assert(*at<2>(tt) == 3);
    }
    {
      using cpt = owner_pointer<int>;
      tuple<cpt, cpt> t{1, 2};
      tuple<cpt, cpt> tt{4, 5};
      adl_swap(t, tt);
      assert(*at<0>(t) == 4);
      assert(*at<1>(t) == 5);
      assert(*at<0>(tt) == 1);
      assert(*at<1>(tt) == 2);
    }

    {
      constexpr tuple<> x0{}, y0{};
      adl_swap(x0, y0);
      static_assert(x0 == tuple{} && y0 == tuple{});

      constexpr tuple<help_tuple::const_swappable_t> x(1), y(2);
      adl_swap(x, y);
      static_assert(at<0>(x).value == 1);
      static_assert(at<0>(y).value == 2);

      constexpr tuple<help_tuple::const_swappable_t,
                      help_tuple::const_swappable_t> xx(1, 2), yy(3, 4);
      adl_swap(xx, yy);
      static_assert(at<0>(xx).value == 1 && at<1>(xx).value == 2);
      static_assert(at<0>(yy).value == 3 && at<1>(yy).value == 4);

      constexpr tuple<help_tuple::const_swappable_t,
                      help_tuple::const_swappable_t,
                      help_tuple::const_swappable_t> xxx(1, 2, 3), yyy(3, 4, 5);
      adl_swap(xxx, yyy);
      static_assert(at<0>(xxx).value == 1 && at<1>(xxx).value == 2
                    && at<2>(xxx).value == 3);
      static_assert(at<0>(yyy).value == 3 && at<1>(yyy).value == 4
                    && at<2>(yyy).value == 5);
    }

    {
      {
        constexpr tuple x{};
        constexpr tuple y(x);
        constexpr tuple z(move(x));
        static_assert(x == tuple{} && x == y && y == z);
        constexpr tuple<> zz = []() constexpr->auto {
          tuple<> x{}, y{};
          return x = y;
        }();
        static_assert(zz == tuple{});
        constexpr tuple<> zzz = []() constexpr->auto {
          tuple<> x{}, y{};
          return x = move(y);
        }();
        static_assert(zzz == tuple{});
        constexpr tuple<> zzzz = []() constexpr->auto {
          tuple<> x{}, y{};
          adl_swap(x, y);
          return x;
        }();
        static_assert(zzzz == tuple{});
      }
      {
        constexpr tuple<int> x(1);
        constexpr tuple<int> y(x);
        constexpr tuple<int> z(move(x));
        static_assert(x == tuple(1) && x == y && y == z);
        constexpr tuple<int> zz = []() constexpr->auto {
          tuple<int> x{2}, y{1};
          return x = y;
        }();
        static_assert(zz == tuple(1));
        constexpr tuple<int> zzz = []() constexpr->auto {
          tuple<int> x{2}, y{1};
          return x = move(y);
        }();
        static_assert(zzz == tuple(1));
        constexpr tuple<int> zzzz = []() constexpr->auto {
          tuple<int> x{2}, y{1};
          adl_swap(x, y);
          return x;
        }();
        static_assert(zzzz == tuple(1));
      }
      {
        constexpr tuple<int, double> x(1, 2);
        constexpr tuple<int, double> y(x);
        constexpr tuple<int, double> z(move(x));
        static_assert(x == tuple(1, 2) && x == y && y == z);
        constexpr tuple<int, double> zz = []() constexpr->auto {
          tuple<int, double> x{2, 3}, y{1, 2};
          return x = y;
        }();
        static_assert(zz == tuple(1, 2));
        constexpr tuple<int, double> zzz = []() constexpr->auto {
          tuple<int, double> x{2, 3}, y{1, 2};
          return x = move(y);
        }();
        static_assert(zzz == tuple(1, 2));
        constexpr tuple<int, double> zzzz = []() constexpr->auto {
          tuple<int, double> x{2, 3}, y{1, 2};
          adl_swap(x, y);
          return x;
        }();
        static_assert(zzzz == tuple(1, 2));
      }
      {
        constexpr tuple<int, double, long> x(1, 2, 3);
        constexpr tuple<int, double, long> y(x);
        constexpr tuple<int, double, long> z(move(x));
        static_assert(x == tuple(1, 2, 3) && x == y && y == z);
        constexpr tuple<int, double, long> zz = []() constexpr->auto {
          tuple<int, double, long> x{2, 3, 4}, y{1, 2, 3};
          return x = y;
        }();
        static_assert(zz == tuple(1, 2, 3));
        constexpr tuple<int, double, long> zzz = []() constexpr->auto {
          tuple<int, double, long> x{2, 3, 4}, y{1, 2, 3};
          return x = move(y);
        }();
        static_assert(zzz == tuple(1, 2, 3));
        constexpr tuple<int, double, long> zzzz = []() constexpr->auto {
          tuple<int, double, long> x{2, 3, 4}, y{1, 2, 3};
          adl_swap(x, y);
          return x;
        }();
        static_assert(zzzz == tuple(1, 2, 3));
      }
    }
  }

  // tuple::tuple(piecewise_construct, ...)
  {
    struct t {
      t(int &, int &&) {}
    };
    struct tt {
      tt() = default;
    };
    struct ttt {
      ttt(int &&, int &&, int &&) {}
    };

    int i{};
    tuple<t> x = {piecewise_construct, forward_as_tuple(i, 1)};
    tuple<tt> xx = {piecewise_construct, forward_as_tuple()};
    tuple<ttt> xxx = {piecewise_construct, forward_as_tuple(1, 2, 3)};

    tuple<ttt, ttt> yy(piecewise_construct,
                       forward_as_tuple(1, 2, 3), forward_as_tuple(1, 2, 3));
    tuple<tt, tt> yyy(piecewise_construct,
                      forward_as_tuple(), forward_as_tuple());

    static_assert(tuple<int, double>(piecewise_construct,
                                     forward_as_tuple(1), forward_as_tuple(2))
                  == tuple(1, 2));
    static_assert(tuple<int, double, long>
                  (piecewise_construct,
                   forward_as_tuple(1), forward_as_tuple(2),
                   forward_as_tuple(3))
                  == tuple(1, 2, 3));
    static_assert(tuple<>(piecewise_construct) == tuple{});

    {
      using t = owner_pointer<int>;
      t a(1), b(2), c(3), d(4), e(5);
      tuple<tuple<t, t>, tuple<t, t, t>> z(piecewise_construct,
                                           forward_as_tuple(a, b),
                                           forward_as_tuple(c, d, move(e)));
      static_assert(!noexcept(tuple<tuple<t, t>, tuple<t, t, t>>
                              (piecewise_construct,
                               forward_as_tuple(a, b),
                               forward_as_tuple(c, d, move(e)))));
      assert(a != nullptr);
      assert(b != nullptr);
      assert(c != nullptr);
      assert(d != nullptr);
      assert(e == nullptr);
      assert(at<0>(z).first != nullptr && *at<0>(z).first == 1);
      assert(at<0>(z).second != nullptr && *at<0>(z).second == 2);
      assert(at<0>(at<1>(z)) != nullptr && *at<0>(at<1>(z)) == 3);
      assert(at<1>(at<1>(z)) != nullptr && *at<1>(at<1>(z)) == 4);
      assert(at<2>(at<1>(z)) != nullptr && *at<2>(at<1>(z)) == 5);
    }
    {
      using t = owner_pointer<int>;
      t a(1), b(2), c(3), d(4), e(5);
      tuple<tuple<t, t>, t, tuple<t, t>>
        z(piecewise_construct,
          forward_as_tuple(move(a), move(b)),
          forward_as_tuple(move(c)),
          forward_as_tuple(move(d), move(e)));
      static_assert(!noexcept(tuple<tuple<t, t>, t, tuple<t, t>>
                              (piecewise_construct,
                               forward_as_tuple(move(a), move(b)),
                               forward_as_tuple(move(c)),
                               forward_as_tuple(move(d), move(e)))));
      assert(a == nullptr);
      assert(b == nullptr);
      assert(c == nullptr);
      assert(d == nullptr);
      assert(e == nullptr);
      assert(at<0>(z).first != nullptr && *at<0>(z).first == 1);
      assert(at<0>(z).second != nullptr && *at<0>(z).second == 2);
      assert(at<1>(z) != nullptr && *at<1>(z) == 3);
      assert(at<2>(z).first != nullptr && *at<2>(z).first == 4);
      assert(at<2>(z).second != nullptr && *at<2>(z).second == 5);
    }
  }

  // tuple::tuple(const S &...)
  // tuple::tuple(SS &&...)
  {
    static_assert(!noexcept(tuple<int, long>(1, 2)));
    static_assert(!noexcept(tuple<int, long>({1}, {2})));
    static_assert(!noexcept(tuple<int, long, long long>(1, 2, 3)));
    static_assert(!noexcept(tuple<int, long, long long>({1}, {2}, {3})));

    const int x = 1;
    const long y = 2;

    tuple<int, long, int> t0 = {{}, {}, {}};
    assert(at<0>(t0) == 0 && at<1>(t0) == 0 && at<2>(t0) == 0);

    tuple<int, long, int> t = {x, y, x};
    assert(at<0>(t) == 1);
    assert(at<1>(t) == 2);
    assert(at<2>(t) == 1);
    tuple<int, long, int> tt = {1, 2, 3};
    assert(at<0>(tt) == 1);
    assert(at<1>(tt) == 2);
    assert(at<2>(tt) == 3);

    tuple<int, long> b = {x, y};
    assert(at<0>(b) == 1 && at<1>(b) == 2);
    tuple<int, long> bb = {1, 2};
    assert(at<0>(bb) == 1 && at<1>(bb) == 2);

    tuple<int> u = {x};
    assert(at<0>(u) == 1);
    tuple<int> uu = {1};
    assert(at<0>(uu) == 1);

    struct type0 {
      type0(int) {}
    };
    struct type {
      explicit type(int) {}
    };
    static_assert(is_convertible_v<int, tuple<type0>>);
    static_assert(!is_convertible_v<int, tuple<type>>);
    static_assert(is_convertible_v<const int &, tuple<type0>>);
    static_assert(!is_convertible_v<const int &, tuple<type>>);
    static_assert(is_implicitly_constructible_v<tuple<type0, type0>, int, int>);
    static_assert(!is_implicitly_constructible_v<tuple<int, type>, int, int>);
    static_assert(!is_implicitly_constructible_v<tuple<type, type>, int, int>);

    static_assert(is_implicitly_constructible_v
                  <tuple<type0, type0, type0>, int, int, int>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<int, type, int>, int, int, int>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<type, type, type>, int, int, int>);

    {
      tuple<tuple<double>, tuple<double, double>> z = {{1}, {2, 3}};
      assert(at<0>(z.first) == 1);
      assert(at<0>(z.second) == 2);
      assert(at<1>(z.second) == 3);
      tuple<tuple<double>, tuple<double, double>> zz
        = {tuple(1), tuple(2, 3)};
      assert(z == zz);
    }
    {
      tuple<tuple<double>, tuple<double, double>, int> z = {{1}, {2, 3}, {4}};
      assert(at<0>(at<0>(z)) == 1);
      assert(at<0>(at<1>(z)) == 2);
      assert(at<1>(at<1>(z)) == 3);
      assert(at<2>(z) == 4);
      tuple<tuple<double>, tuple<double, double>, int> zz
        = {tuple(1), tuple(2, 3), 4};
      assert(z == zz);
    }

    static_assert(tuple<int, long, int>(1, 2, 3)
                  == tuple<int, int, int>({1}, {2}, {3}));
    static_assert(tuple<int, long>(1, 2)
                  == tuple<int, int>({1}, {2}));
  }

  // tuple::tuple(const tuple<...> &)
  // tuple::operator =(const tuple<...> &)
  // tuple::tuple(tuple<...> &&)
  // tuple::operator =(tuple<...> &&)
  {
    static_assert(noexcept(tuple<>(tuple<>{})));
    static_assert(noexcept(declval<tuple<> &>() = tuple<>{}));
    static_assert(noexcept(declval<tuple<> &>() = declval<tuple<> &>()));
    static_assert(!noexcept(tuple<int>(tuple<long>{})));
    static_assert(!noexcept(declval<tuple<int> &>()
                            = declval<tuple<long> &>()));
    static_assert(!noexcept(tuple<int>(tuple<long>{})));
    static_assert(!noexcept(declval<tuple<int> &>() = tuple<long>{}));
    static_assert(!noexcept(tuple<int, int>(tuple<int, long>{})));
    static_assert(!noexcept(declval<tuple<int, int> &>()
                            = declval<tuple<int, long> &>()));
    static_assert(!noexcept(tuple<int, int>(tuple<int, long>{})));
    static_assert(!noexcept(declval<tuple<int, int> &>() = tuple<int, long>{}));

    struct t {
      explicit t(int) {}
    };
    static_assert(is_implicitly_constructible_v
                  <tuple<int>, const tuple<short> &>);
    static_assert(is_implicitly_constructible_v
                  <tuple<int>, tuple<short> &&>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t>, const tuple<int> &>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t>, tuple<int> &&>);
    static_assert(is_implicitly_constructible_v
                  <tuple<int, int>, const tuple<short, short> &>);
    static_assert(is_implicitly_constructible_v
                  <tuple<int, int>, tuple<short, short> &&>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t, t>, const tuple<int, int> &>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t, t>, tuple<int, int> &&>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<long, t>, const tuple<int, int> &>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<long, t>, tuple<int, int> &&>);
    static_assert(is_implicitly_constructible_v
                  <tuple<int, int, int>, const tuple<short, short, short> &>);
    static_assert(is_implicitly_constructible_v
                  <tuple<int, int, int>, tuple<short, short, short> &&>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t, t, t>, const tuple<int, int, int> &>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<t, t, t>, tuple<int, int, int> &&>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<long, t, long>, const tuple<int, int, int> &>);
    static_assert(!is_implicitly_constructible_v
                  <tuple<long, t, long>, tuple<int, int, int> &&>);

    {
      tuple<int> x = tuple<short>(1);
      assert(at<0>(x) == 1);
      assert(addressof(x = tuple<short>(3)) == addressof(x));
      assert(at<0>(x) == 3);
    }
    {
      tuple<int> x = as_const(as_lvalue(tuple<short>(1)));
      assert(at<0>(x) == 1);
      assert(addressof(x = as_const(as_lvalue(tuple<short>(3))))
             == addressof(x));
      assert(at<0>(x) == 3);
    }

    {
      tuple<int, int> x = tuple<short, short>(1, 2);
      assert(at<0>(x) == 1);
      assert(at<1>(x) == 2);
      assert(addressof(x = tuple<short, short>(3, 4)) == addressof(x));
      assert(at<0>(x) == 3);
      assert(at<1>(x) == 4);
    }
    {
      tuple<int, int> x = as_const(as_lvalue(tuple<short, short>(1, 2)));
      assert(at<0>(x) == 1);
      assert(at<1>(x) == 2);
      assert(addressof(x = as_const(as_lvalue(tuple<short, short>(3, 4))))
             == addressof(x));
      assert(at<0>(x) == 3);
      assert(at<1>(x) == 4);
    }

    {
      tuple<int, int, int> x = tuple<short, short, short>(1, 2, 3);
      assert(at<0>(x) == 1);
      assert(at<1>(x) == 2);
      assert(at<2>(x) == 3);
      assert(addressof(x = tuple<short, short, short>(4, 5, 6))
             == addressof(x));
      assert(at<0>(x) == 4);
      assert(at<1>(x) == 5);
      assert(at<2>(x) == 6);
    }
    {
      tuple<int, int, int> x
        = as_const(as_lvalue(tuple<short, short, short>(1, 2, 3)));
      assert(at<0>(x) == 1);
      assert(at<1>(x) == 2);
      assert(at<2>(x) == 3);
      assert(addressof(x = as_const(as_lvalue
                                    (tuple<short, short, short>(4, 5, 6))))
             == addressof(x));
      assert(at<0>(x) == 4);
      assert(at<1>(x) == 5);
      assert(at<2>(x) == 6);
    }

    {
      {
        constexpr auto f = []() constexpr {
          tuple<long> y(1);
          tuple<int> x(y);
          return x;
        };
        static_assert(f() == tuple(1));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int> x(tuple<long>(1));
          return x;
        };
        static_assert(f() == tuple(1));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int> x;
          tuple<long> y(1);
          x = y;
          return x;
        };
        static_assert(f() == tuple(1));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int> x;
          x = tuple<long>(1);
          return x;
        };
        static_assert(f() == tuple(1));
      }

      {
        constexpr auto f = []() constexpr {
          tuple<int, long> y(1, 2);
          tuple<int, int> x(y);
          return x;
        };
        static_assert(f() == tuple(1, 2));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int, int> x(tuple<int, long>(1, 2));
          return x;
        };
        static_assert(f() == tuple(1, 2));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int, int> x;
          tuple<int, long> y(1, 2);
          x = y;
          return x;
        };
        static_assert(f() == tuple(1, 2));
      }
      {
        constexpr auto f = []() constexpr {
          tuple<int, int> x;
          x = tuple<int, long>(1, 2);
          return x;
        };
        static_assert(f() == tuple(1, 2));
      }
    }
  }

  // tuple::tuple(allocator_arg_t, alloc, const tuple &)
  // tuple::tuple(allocator_arg_t, alloc, tuple &&)
  // tuple::tuple(allocator_arg_t, alloc, const S &...)
  // tuple::tuple(allocator_arg_t, alloc, SS &&...)
  // tuple::tuple(allocator_arg_t, alloc, const tuple<...> &)
  // tuple::tuple(allocator_arg_t, alloc, tuple<...> &&)
  {
    {
      struct t {
        explicit t(int) {}
      };
      static_assert(is_implicitly_constructible_v
                    <tuple<int>, allocator_arg_t, int, const tuple<short> &>);
      static_assert(is_implicitly_constructible_v
                    <tuple<int>, allocator_arg_t, int, tuple<short> &&>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t>, allocator_arg_t, int, const tuple<int> &>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t>, allocator_arg_t, int, tuple<int> &&>);
      static_assert(is_implicitly_constructible_v
                    <tuple<int, int>,
                     allocator_arg_t, int, const tuple<short, short> &>);
      static_assert(is_implicitly_constructible_v
                    <tuple<int, int>,
                     allocator_arg_t, int, tuple<short, short> &&>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t, int>,
                     allocator_arg_t, int, const tuple<int, int> &>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t, int>,
                     allocator_arg_t, int, tuple<int, int> &&>);

      static_assert(is_implicitly_constructible_v
                    <tuple<int>, allocator_arg_t, int, const int &>);
      static_assert(is_implicitly_constructible_v
                    <tuple<int>, allocator_arg_t, int, short>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t>, allocator_arg_t, int, const int &>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t>, allocator_arg_t, int, short>);

      static_assert(is_implicitly_constructible_v
                    <tuple<int, int>,
                     allocator_arg_t, int, const int &, const int &>);
      static_assert(is_implicitly_constructible_v
                    <tuple<int, int>, allocator_arg_t, int, short, short>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t, int>,
                     allocator_arg_t, int, const int &, const int &>);
      static_assert(!is_implicitly_constructible_v
                    <tuple<t, int>,
                     allocator_arg_t, int, short, short>);
     }

    static_assert(!noexcept(tuple<>(allocator_arg, 0, declval<tuple<> &>())));
    static_assert(!noexcept(tuple<>(allocator_arg, 0, declval<tuple<>>())));
    static_assert(!noexcept(tuple<int>
                            (allocator_arg, 0, declval<tuple<int> &>())));
    static_assert(!noexcept(tuple<int>
                            (allocator_arg, 0, declval<tuple<int>>())));
    static_assert(!noexcept(tuple<int, int>
                            (allocator_arg, 0, declval<tuple<int, int> &>())));
    static_assert(!noexcept(tuple<int, int>
                            (allocator_arg, 0, declval<tuple<int, int>>())));
    static_assert(!noexcept(tuple<>(allocator_arg, 0)));
    static_assert(!noexcept(tuple<int>(allocator_arg, 0, 1)));
    static_assert(!noexcept(tuple<int>(allocator_arg, 0, 1l)));
    static_assert(!noexcept(tuple<int, int>(allocator_arg, 0, 1, 2)));
    static_assert(!noexcept(tuple<int, int>(allocator_arg, 0, 1l, 2l)));
    static_assert(!noexcept(tuple<int>(allocator_arg, 0, tuple<short>{})));
    static_assert(!noexcept(tuple<int>
                            (allocator_arg, 0, declval<tuple<short> &>())));
    static_assert(!noexcept(tuple<int, int>
                            (allocator_arg, 0, tuple<short, int>{})));
    static_assert(!noexcept(tuple<int, int>
                            (allocator_arg, 0,
                             declval<tuple<int, short> &>())));

    struct t {
      int x;

      explicit t(int i) : x(i) {}

      using allocator_type = int;
      t(int &&i, const allocator_type &) : x(i + 1) {}
      t(const t &x, const allocator_type &) : x(x.x + 2) {}
      t(t &&x, const allocator_type &) : x(x.x + 3) {}

      t(const int &i, const allocator_type &) : x(i * 10) {}
    };
    static_assert(uses_allocator_v<t, const int &>);
    static_assert(uses_allocator_v<t, int &&>);
    static_assert(uses_allocator_v<t, int>);
    static_assert(!uses_allocator_v<t, const nullptr_t &>);
    static_assert(!uses_allocator_v<t, nullptr_t &&>);
    static_assert(!uses_allocator_v<t, nullptr_t>);

    struct tt {
      int x;

      explicit tt(int i) : x(i) {}

      using allocator_type = int;
      tt(int &&i, const allocator_type &) : x(i + 1) {}
      tt(const t &x, const allocator_type &) : x(x.x + 2) {}
      tt(t &&x, const allocator_type &) : x(x.x + 3) {}

      tt(allocator_arg_t, const allocator_type &, int &&i) : x(i + 4) {}
      tt(allocator_arg_t, const allocator_type &, const tt &x) : x(x.x + 5) {}
      tt(allocator_arg_t, const allocator_type &, tt &&x) : x(x.x + 6) {}

      tt(const int &i, const allocator_type &) : x(i * 10) {}
      tt(allocator_arg_t, const allocator_type &, const int &i) : x(i * 100) {}
    };
    static_assert(uses_allocator_v<tt, const int &>);
    static_assert(uses_allocator_v<tt, int &&>);
    static_assert(uses_allocator_v<tt, int>);
    static_assert(!uses_allocator_v<tt, const nullptr_t &>);
    static_assert(!uses_allocator_v<tt, nullptr_t &&>);
    static_assert(!uses_allocator_v<tt, nullptr_t>);

    // tuple::tuple(allocator_arg_t, alloc, const tuple &)
    // tuple::tuple(allocator_arg_t, alloc, tuple &&)
    {
      // t
      {
        tuple<t, t, t> x(1, 2, 3);
        assert(at<0>(x).x == 1);
        assert(at<1>(x).x == 2);
        assert(at<2>(x).x == 3);
        {
          tuple<t, t, t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 3);
          assert(at<1>(y).x == 4);
          assert(at<2>(y).x == 5);
        }
        {
          tuple<t, t, t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 4);
          assert(at<1>(y).x == 5);
          assert(at<2>(y).x == 6);
        }
        {
          tuple<t, t, t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
        {
          tuple<t, t, t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
      }
      {
        tuple<t, t> x(1, 2);
        assert(at<0>(x).x == 1);
        assert(at<1>(x).x == 2);
        {
          tuple<t, t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 3);
          assert(at<1>(y).x == 4);
        }
        {
          tuple<t, t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 4);
          assert(at<1>(y).x == 5);
        }
        {
          tuple<t, t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
        {
          tuple<t, t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
      }
      {
        tuple<t> x(1);
        assert(at<0>(x).x == 1);
        {
          tuple<t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 3);
        }
        {
          tuple<t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 4);
        }
        {
          tuple<t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
        }
        {
          tuple<t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
        }
      }

      // tt
      {
        tuple<tt, tt, tt> x(1, 2, 3);
        assert(at<0>(x).x == 1);
        assert(at<1>(x).x == 2);
        assert(at<2>(x).x == 3);
        {
          tuple<tt, tt, tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 6);
          assert(at<1>(y).x == 7);
          assert(at<2>(y).x == 8);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 7);
          assert(at<1>(y).x == 8);
          assert(at<2>(y).x == 9);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
      }
      {
        tuple<tt, tt> x(1, 2);
        assert(at<0>(x).x == 1);
        assert(at<1>(x).x == 2);
        {
          tuple<tt, tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 6);
          assert(at<1>(y).x == 7);
        }
        {
          tuple<tt, tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 7);
          assert(at<1>(y).x == 8);
        }
        {
          tuple<tt, tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
        {
          tuple<tt, tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
      }
      {
        tuple<tt> x(1);
        assert(at<0>(x).x == 1);
        {
          tuple<tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 6);
        }
        {
          tuple<tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 7);
        }
        {
          tuple<tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
        }
        {
          tuple<tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
        }
      }
    }

    // tuple::tuple(allocator_arg_t, alloc, const S &...)
    // tuple::tuple(allocator_arg_t, alloc, SS &&...)
    {
      // t
      {
        {
          tuple<t, t, t> x(allocator_arg, 0,
                           1, 2, 3);
          assert(at<0>(x).x == 2);
          assert(at<1>(x).x == 3);
          assert(at<2>(x).x == 4);
        }
        {
          tuple<t, t, t> x(allocator_arg, 0,
                           as_const(as_lvalue(t(1))), as_const(as_lvalue(t(2))),
                           as_const(as_lvalue(t(3))));
          assert(at<0>(x).x == 3);
          assert(at<1>(x).x == 4);
          assert(at<2>(x).x == 5);
        }
        {
          tuple<t, t, t> x(allocator_arg, 0, t(1), t(2), t(3));
          assert(at<0>(x).x == 4);
          assert(at<1>(x).x == 5);
          assert(at<2>(x).x == 6);
        }

        {
          tuple<t, t, t> x(allocator_arg, nullptr,
                           1, 2, 3);
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
        {
          tuple<t, t, t> x(allocator_arg, nullptr,
                           as_const(as_lvalue(t(1))), as_const(as_lvalue(t(2))),
                           as_const(as_lvalue(t(3))));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
        {
          tuple<t, t, t> x(allocator_arg, nullptr, t(1), t(2), t(3));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
      }
      {
        {
          tuple<t, t> x(allocator_arg, 0,
                        1, 2);
          assert(at<0>(x).x == 2);
          assert(at<1>(x).x == 3);
        }
        {
          tuple<t, t> x(allocator_arg, 0,
                        as_const(as_lvalue(t(1))), as_const(as_lvalue(t(2))));
          assert(at<0>(x).x == 3);
          assert(at<1>(x).x == 4);
        }
        {
          tuple<t, t> x(allocator_arg, 0, t(1), t(2));
          assert(at<0>(x).x == 4);
          assert(at<1>(x).x == 5);
        }

        {
          tuple<t, t> x(allocator_arg, nullptr,
                        1, 2);
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
        {
          tuple<t, t> x(allocator_arg, nullptr,
                        as_const(as_lvalue(t(1))), as_const(as_lvalue(t(2))));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
        {
          tuple<t, t> x(allocator_arg, nullptr, t(1), t(2));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
      }
      {
        {
          tuple<t> x(allocator_arg, 0,
                     1);
          assert(at<0>(x).x == 2);
        }
        {
          tuple<t> x(allocator_arg, 0,
                     as_const(as_lvalue(t(1))));
          assert(at<0>(x).x == 3);
        }
        {
          tuple<t> x(allocator_arg, 0, t(1));
          assert(at<0>(x).x == 4);
        }

        {
          tuple<t> x(allocator_arg, nullptr,
                     1);
          assert(at<0>(x).x == 1);
        }
        {
          tuple<t> x(allocator_arg, nullptr,
                     as_const(as_lvalue(t(1))));
          assert(at<0>(x).x == 1);
        }
        {
          tuple<t> x(allocator_arg, nullptr, t(1));
          assert(at<0>(x).x == 1);
        }
      }

      // tt
      {
        {
          tuple<tt, tt, tt> x(allocator_arg, 0,
                              1, 2, 3);
          assert(at<0>(x).x == 5);
          assert(at<1>(x).x == 6);
          assert(at<2>(x).x == 7);
        }
        {
          tuple<tt, tt, tt> x(allocator_arg, 0,
                              as_const(as_lvalue(tt(1))),
                              as_const(as_lvalue(tt(2))),
                              as_const(as_lvalue(tt(3))));
          assert(at<0>(x).x == 6);
          assert(at<1>(x).x == 7);
          assert(at<2>(x).x == 8);
        }
        {
          tuple<tt, tt, tt> x(allocator_arg, 0, tt(1), tt(2), tt(3));
          assert(at<0>(x).x == 7);
          assert(at<1>(x).x == 8);
          assert(at<2>(x).x == 9);
        }

        {
          tuple<tt, tt, tt> x(allocator_arg, nullptr,
                              1, 2, 3);
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
        {
          tuple<tt, tt, tt> x(allocator_arg, nullptr,
                              as_const(as_lvalue(tt(1))),
                              as_const(as_lvalue(tt(2))),
                              as_const(as_lvalue(tt(3))));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
        {
          tuple<tt, tt, tt> x(allocator_arg, nullptr, tt(1), tt(2), tt(3));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
          assert(at<2>(x).x == 3);
        }
      }
      {
        {
          tuple<tt, tt> x(allocator_arg, 0,
                          1, 2);
          assert(at<0>(x).x == 5);
          assert(at<1>(x).x == 6);
        }
        {
          tuple<tt, tt> x(allocator_arg, 0,
                          as_const(as_lvalue(tt(1))),
                          as_const(as_lvalue(tt(2))));
          assert(at<0>(x).x == 6);
          assert(at<1>(x).x == 7);
        }
        {
          tuple<tt, tt> x(allocator_arg, 0, tt(1), tt(2));
          assert(at<0>(x).x == 7);
          assert(at<1>(x).x == 8);
        }

        {
          tuple<tt, tt> x(allocator_arg, nullptr,
                          1, 2);
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
        {
          tuple<tt, tt> x(allocator_arg, nullptr,
                          as_const(as_lvalue(tt(1))),
                          as_const(as_lvalue(tt(2))));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
        {
          tuple<tt, tt> x(allocator_arg, nullptr, tt(1), tt(2));
          assert(at<0>(x).x == 1);
          assert(at<1>(x).x == 2);
        }
      }
      {
        {
          tuple<tt> x(allocator_arg, 0,
                      1);
          assert(at<0>(x).x == 5);
        }
        {
          tuple<tt> x(allocator_arg, 0,
                      as_const(as_lvalue(tt(1))));
          assert(at<0>(x).x == 6);
        }
        {
          tuple<tt> x(allocator_arg, 0, tt(1));
          assert(at<0>(x).x == 7);
        }

        {
          tuple<tt> x(allocator_arg, nullptr,
                      1);
          assert(at<0>(x).x == 1);
        }
        {
          tuple<tt> x(allocator_arg, nullptr,
                      as_const(as_lvalue(tt(1))));
          assert(at<0>(x).x == 1);
        }
        {
          tuple<tt> x(allocator_arg, nullptr, tt(1));
          assert(at<0>(x).x == 1);
        }
      }
    }

    // tuple::tuple(allocator_arg_t, alloc, const tuple<...> &)
    // tuple::tuple(allocator_arg_t, alloc, tuple<...> &&)
    {
      // t
      {
        tuple<int, int, int> x(1, 2, 3);
        assert(at<0>(x) == 1);
        assert(at<1>(x) == 2);
        assert(at<2>(x) == 3);
        {
          tuple<t, t, t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 10);
          assert(at<1>(y).x == 20);
          assert(at<2>(y).x == 30);
        }
        {
          tuple<t, t, t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 2);
          assert(at<1>(y).x == 3);
          assert(at<2>(y).x == 4);
        }
        {
          tuple<t, t, t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
        {
          tuple<t, t, t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
      }
      {
        tuple<int, int> x(1, 2);
        assert(at<0>(x) == 1);
        assert(at<1>(x) == 2);
        {
          tuple<t, t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 10);
          assert(at<1>(y).x == 20);
        }
        {
          tuple<t, t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 2);
          assert(at<1>(y).x == 3);
        }
        {
          tuple<t, t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
        {
          tuple<t, t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
      }
      {
        tuple<int> x(1);
        assert(at<0>(x) == 1);
        {
          tuple<t> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 10);
        }
        {
          tuple<t> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 2);
        }
        {
          tuple<t> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
        }
        {
          tuple<t> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
        }
      }

      // tt
      {
        tuple<int, int, int> x(1, 2, 3);
        assert(at<0>(x) == 1);
        assert(at<1>(x) == 2);
        assert(at<2>(x) == 3);
        {
          tuple<tt, tt, tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 100);
          assert(at<1>(y).x == 200);
          assert(at<2>(y).x == 300);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 5);
          assert(at<1>(y).x == 6);
          assert(at<2>(y).x == 7);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
        {
          tuple<tt, tt, tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
          assert(at<2>(y).x == 3);
        }
      }
      {
        tuple<int, int> x(1, 2);
        assert(at<0>(x) == 1);
        assert(at<1>(x) == 2);
        {
          tuple<tt, tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 100);
          assert(at<1>(y).x == 200);
        }
        {
          tuple<tt, tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 5);
          assert(at<1>(y).x == 6);
        }
        {
          tuple<tt, tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
        {
          tuple<tt, tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
          assert(at<1>(y).x == 2);
        }
      }
      {
        tuple<int> x(1);
        assert(at<0>(x) == 1);
        {
          tuple<tt> y(allocator_arg, 0, x);
          assert(at<0>(y).x == 100);
        }
        {
          tuple<tt> y(allocator_arg, 0, move(x));
          assert(at<0>(y).x == 5);
        }
        {
          tuple<tt> y(allocator_arg, nullptr, x);
          assert(at<0>(y).x == 1);
        }
        {
          tuple<tt> y(allocator_arg, nullptr, move(x));
          assert(at<0>(y).x == 1);
        }
      }
    }

    {
      constexpr tuple<> t(allocator_arg, 0, tuple{});
      constexpr tuple<> tt(allocator_arg, 0, t);
      constexpr tuple<> ttt(allocator_arg, 0);
      static_assert(t == tuple{} && t == tt && t == ttt);
    }
    {
      constexpr tuple<int> t(allocator_arg, 0, tuple(1));
      constexpr tuple<int> tt(allocator_arg, 0, t);
      constexpr tuple<int> ttt(allocator_arg, 0, 1);
      constexpr tuple<int> tttt(allocator_arg, 0, 1.0);
      constexpr tuple<int> ttttt(allocator_arg, 0, tuple(1.0));
      constexpr tuple tmp(1.0);
      constexpr tuple<int> tttttt(allocator_arg, 0, tmp);
      static_assert(t == tuple(1));
      static_assert(t == tt);
      static_assert(t == ttt);
      static_assert(t == tttt);
      static_assert(t == ttttt);
      static_assert(t == tttttt);
    }
    {
      constexpr tuple<int, long> t(allocator_arg, 0, tuple(1, 2l));
      constexpr tuple<int, long> tt(allocator_arg, 0, t);
      constexpr tuple<int, long> ttt(allocator_arg, 0, 1, 2l);
      constexpr tuple<int, long> tttt(allocator_arg, 0, 1.0, 2.0);
      constexpr tuple<int, long> ttttt(allocator_arg, 0, tuple(1.0, 2.0));
      constexpr tuple tmp(1.0, 2.0);
      constexpr tuple<int, long> tttttt(allocator_arg, 0, tmp);
      static_assert(t == tuple(1, 2));
      static_assert(t == tt);
      static_assert(t == ttt);
      static_assert(t == tttt);
      static_assert(t == ttttt);
      static_assert(t == tttttt);
    }
    {
      constexpr tuple<int, long, int> t(allocator_arg, 0, tuple(1, 2l, 3));
      constexpr tuple<int, long, int> tt(allocator_arg, 0, t);
      constexpr tuple<int, long, int> ttt(allocator_arg, 0, 1, 2l, 3);
      constexpr tuple<int, long, int> tttt(allocator_arg, 0, 1.0, 2.0, 3.0);
      constexpr tuple<int, long, int> ttttt
        (allocator_arg, 0, tuple(1.0, 2.0, 3.0));
      constexpr tuple tmp(1.0, 2.0, 3.0);
      constexpr tuple<int, long, int> tttttt(allocator_arg, 0, tmp);
      static_assert(t == tuple(1, 2, 3));
      static_assert(t == tt);
      static_assert(t == ttt);
      static_assert(t == tttt);
      static_assert(t == ttttt);
      static_assert(t == tttttt);
    }
  }

  // tuple::tuple(tuple<...> &)
  // tuple::tuple(allocator_arg_t, alloc, tuple<...> &)
  // tuple::operator =(tuple<...> &)
  // tuple::tuple(const tuple<...> &&)
  // tuple::tuple(allocator_arg_t, alloc, const tuple<...> &&)
  // tuple::operator =(const tuple<...> &&)
  {}

  // == <=>
  {
    {
      struct t {};

      static_assert(equality_comparable<tuple<>>);

      static_assert(equality_comparable<tuple<int>>);
      static_assert(!equality_comparable<tuple<t>>);
      static_assert(equality_comparable_with<tuple<int>, tuple<long>>);
      static_assert(!equality_comparable_with<tuple<t>, tuple<t, t>>);
      static_assert(!equality_comparable_with<tuple<t, t>, tuple<int>>);
      static_assert(!equality_comparable_with
                    <tuple<int, int, int>, tuple<int>>);

      static_assert(equality_comparable<tuple<int, long>>);
      static_assert(!equality_comparable<tuple<int, t>>);
      static_assert(equality_comparable_with
                    <tuple<int, long>, tuple<short, int>>);
      static_assert(!equality_comparable_with
                    <tuple<int, long>, tuple<short, int, long>>);
      static_assert(!equality_comparable_with
                    <tuple<int, t>, tuple<t, int>>);

      static_assert(equality_comparable<tuple<int, int, int>>);
      static_assert(!equality_comparable<tuple<t, int, int>>);
      static_assert(equality_comparable_with
                    <tuple<int, int, int>, tuple<long, long, short>>);
      static_assert(!equality_comparable_with
                    <tuple<int, int, int, int>, tuple<long, long, short>>);
      static_assert(!equality_comparable_with
                    <tuple<t, t, int>, tuple<int, int, int>>);

      static_assert(three_way_comparable<tuple<>, strong_ordering>);

      static_assert(three_way_comparable<tuple<int>, strong_ordering>);
      static_assert(!three_way_comparable<tuple<t>>);

      static_assert(three_way_comparable<tuple<int, int>, strong_ordering>);
      static_assert(!three_way_comparable<tuple<int, t>>);
      static_assert(!three_way_comparable<tuple<t, int>>);
      static_assert(!three_way_comparable<tuple<t, t>>);
      static_assert(!three_way_comparable_with<tuple<t, t>, tuple<int, int>>);
      static_assert(three_way_comparable_with
                    <tuple<int, long>, tuple<long, int>, strong_ordering>);
      static_assert(!three_way_comparable_with
                    <tuple<int, long>, tuple<t, int>>);

      static_assert(three_way_comparable
                    <tuple<float, int, int>, partial_ordering>);
      static_assert(three_way_comparable
                    <tuple<float, float, float>, partial_ordering>);
      static_assert(!three_way_comparable<tuple<int, int, t>>);
      static_assert(!three_way_comparable<tuple<t, int, int>>);
      static_assert(!three_way_comparable<tuple<int, t, int>>);
      static_assert(!three_way_comparable<tuple<int, t, t>>);
      static_assert(!three_way_comparable<tuple<t, t, int>>);
      static_assert(!three_way_comparable<tuple<t, int, t>>);
      static_assert(!three_way_comparable<tuple<t, t, t>>);
      static_assert(!three_way_comparable_with
                    <tuple<int, long, short>, tuple<long, int>>);
      static_assert(three_way_comparable_with
                    <tuple<int, long, short>, tuple<short, long, int>,
                     strong_ordering>);
      static_assert(!three_way_comparable_with
                    <tuple<int, long, float>, tuple<short, long, int>,
                     strong_ordering>);
      static_assert(three_way_comparable_with
                    <tuple<int, long, float>, tuple<short, long, int>,
                     partial_ordering>);
      static_assert(!three_way_comparable_with
                    <tuple<int, long>, tuple<t, int>>);
    }

    {
      static_assert(!noexcept(declval<tuple<> &>() == declval<tuple<> &>()));
      static_assert(!noexcept(declval<tuple<> &>() <=> declval<tuple<> &>()));
      static_assert(!noexcept(declval<tuple<int> &>()
                              == declval<tuple<long> &>()));
      static_assert(!noexcept(declval<tuple<int> &>()
                              <=> declval<tuple<long> &>()));
      static_assert(!noexcept(declval<tuple<int, int> &>()
                              == declval<tuple<long, long> &>()));
      static_assert(!noexcept(declval<tuple<int, int> &>()
                              <=> declval<tuple<long, long> &>()));
    }
    {
      static_assert(tuple<>{} == tuple<>{});
      static_assert(tuple<>{} <=> tuple<>{} == 0);
      static_assert(tuple<int>{} == tuple<long>{});
      static_assert(tuple<int>{} <=> tuple<long>{} == 0);
      static_assert(tuple<int, long>{} == tuple<long, int>{});
      static_assert(tuple<int, long>{} <=> tuple<long, int>{} == 0);
    }

    {
      tuple<> a, b;
      assert(a == b);
      assert(b == a);
      assert(!(a != b));
      assert(!(b != a));
      assert(!(a < b));
      assert(!(b < a));
      assert(!(a > b));
      assert(!(b > a));
      assert(a <= b);
      assert(b <= a);
      assert(a >= b);
      assert(b >= a);
    }
    {
      tuple<nullptr_t> a, b;
      assert(a == b);
      assert(b == a);
      assert(!(a != b));
      assert(!(b != a));

      struct t {};
      static_assert(!three_way_comparable<tuple<t>>);
      static_assert(!three_way_comparable<tuple<int, int, t>>);
      static_assert(!three_way_comparable<tuple<int, t, int, int>>);
      static_assert(!equality_comparable<tuple<t>>);
      static_assert(!equality_comparable<tuple<int, int, t>>);
      static_assert(!equality_comparable<tuple<int, t, int, int>>);
    }
    {
      tuple<int> a(1), b(1), c(2);
      assert(a == b);
      strong_ordering o = b <=> c;
      assert(o < 0);
      o = a <=> b;
      assert(o == 0);
    }
    {
      assert(make_tuple(1, 2) == make_tuple(1, 2));
      assert(make_pair(1, 2) == make_tuple(1, 2));

      assert(make_tuple(1, 2.0) <=> make_tuple(1, 2.0) == 0);
      assert(make_tuple(1, 2.0) <=> make_tuple(2, 2.0) < 0);
      assert(make_tuple(2, 2.0) <=> make_tuple(1, 2.0) > 0);
      assert(make_tuple(2, 2.0) <=> make_tuple(2, 1.0) > 0);
      assert(make_tuple(2, 1.0) <=> make_tuple(2, 2.0) < 0);
      static_assert(same_as
                    <decltype(make_tuple(2, 1.0) <=> make_tuple(2, 2.0)),
                     partial_ordering>);
    }
    {
      assert(make_tuple(1, 2, 3, 4) <=> make_tuple(1, 2, 3, 4) == 0);
      assert(make_tuple(1, 2, 3, 4) <=> make_tuple(1, 2, 3, 5) < 0);
      assert(make_tuple(1, 2, 3, 4) <=> make_tuple(1, 2, 3, 3) > 0);
      assert(make_tuple(0, 2, 3, 4) <=> make_tuple(1, 2, 3, 4) < 0);
      assert(make_tuple(1, 2, 3, 4) <=> make_tuple(0, 2, 3, 4) > 0);
      static_assert(same_as
                    <decltype(make_tuple(1, 2, 3, 4)
                              <=> make_tuple(0, 2, 3, 4)),
                     strong_ordering>);
      static_assert(same_as
                    <decltype(make_tuple(1, 2, 3, 4)
                              <=> make_tuple(0.0, 2, 3, 4)),
                     partial_ordering>);
    }

    {
      static_assert(tuple{} == tuple{});
      static_assert(tuple(1) == tuple(1));
      static_assert(tuple(1) != tuple(2));
      static_assert(tuple(1, 2) == tuple(1, 2));
      static_assert(tuple(1, 2) != tuple(3, 4));
      static_assert(tuple(1, 2, 3) == tuple(1, 2, 3));
      static_assert(tuple(1, 2, 3) != tuple(4, 5, 6));

      static_assert(tuple{} <=> tuple{} == strong_eq);

      static_assert(tuple(1) <=> tuple(2) == strong_lt);
      static_assert(tuple(1, 2) <=> tuple(1, 3) == strong_lt);
      static_assert(tuple(1, 2, 3) <=> tuple(1, 2, 4) == strong_lt);

      static_assert(tuple(2) <=> tuple(1) == strong_gt);
      static_assert(tuple(1, 3) <=> tuple(1, 2) == strong_gt);
      static_assert(tuple(1, 2, 4) <=> tuple(1, 2, 3) == strong_gt);

      static_assert(tuple{} <=> tuple{} == strong_eq);
      static_assert(tuple(1) <=> tuple(1) == strong_eq);
      static_assert(tuple(1, 2) <=> tuple(1, 2) == strong_eq);
      static_assert(tuple(1, 2, 3) <=> tuple(1, 2, 3) == strong_eq);
    }
  }

  // min_pair
  // min_tuple
  {
    static_assert(is_trivial_v<min_pair<int, float>>);
    static_assert(semiregular<min_pair<int, float>>);
    static_assert(!is_default_constructible_v<min_pair<int, float &>>);
    static_assert(is_move_constructible_v<min_pair<int, float &>>);
    static_assert(is_copy_constructible_v<min_pair<int, float &>>);
    static_assert(!is_move_assignable_v<min_pair<int, float &>>);
    static_assert(!is_copy_assignable_v<min_pair<int, float &>>);

    static_assert(semiregular<min_tuple<int, float, double>>);
    static_assert(is_trivial_v<min_tuple<int, float, double>>);
    static_assert(!is_default_constructible_v<min_tuple<int, float &, double>>);
    static_assert(is_move_constructible_v<min_tuple<int, float &, double>>);
    static_assert(is_copy_constructible_v<min_tuple<int, float &, double>>);
    static_assert(!is_move_assignable_v<min_tuple<int, float &, double>>);
    static_assert(!is_copy_assignable_v<min_tuple<int, float &, double>>);
  }

  // pair
  {
    static_assert(same_as<pair<int, float>, tuple<int, float>>);
    pair<int, float> x;
    x.first = 1;
    x.second = 2.0f;
    assert(at<0>(x) == 1);
    assert(at<1>(x) == 2.0f);
  }

  // structured binding
  {
    {
      auto [x] = tuple(1);
      auto &&[a] = tuple(1);
      assert(x == 1);
      assert(a == 1);
    }
    {
      auto [x, y] = tuple(1, 2);
      auto &&[a, b] = tuple(1, 2);
      assert(x == 1 && y == 2);
      assert(a == 1 && b == 2);
    }
    {
      auto [x, y, z] = tuple(1, 2, 3);
      auto &&[a, b, c] = tuple(1, 2, 3);
      assert(x == 1 && y == 2 && z == 3);
      assert(a == 1 && b == 2 && c == 3);
    }
  }

  // bind_tuple
  {
    {
      auto t = bind_tuple(tuple(1), [](int x) {return x * 2;});
      assert(t == tuple(2));
    }
    {
      auto t = bind_tuple(tuple(1, 2, 3), [](int x) {return x * 2;});
      assert(t == tuple(2, 4, 6));
    }
  }
}

namespace help_bind {

struct ft {
  int operator ()(int a) {return a;}
  int operator ()(int a) const {return a + 1;}
  int operator ()(int a, int b) {return a + b * 10;}
  int operator ()(int a, int b) const {return (a + 1) + b * 10;}
  int operator ()(int a, int b, int c) {return a + b * 10 + c * 100;}
  int operator ()(int a, int b, int c) const {
    return (a + 1) + b * 10 + c * 100;
  }
  int operator ()(int a, int b, int c, int d) {
    return a + b * 10 + c * 100 + d * 1000;
  }
  int operator ()(int a, int b, int c, int d) const {
    return (a + 1) + b * 10 + c * 100 + d * 1000;
  }
};
void test() {
  {
    {
      auto f = bind(ft(), 1);
      assert(f() == 1);
      assert(as_const(f)() == 2);
      assert(bind_r<int>(ft(), 1)() == 1);
    }
    {
      auto f = bind(ft(), 1, 2);
      assert(f() == 21);
      assert(as_const(f)() == 22);
      assert(bind_r<int>(ft(), 1, 2)() == 21);
    }
    {
      auto f = bind(ft(), 1, 2, 3);
      assert(f() == 321);
      assert(as_const(f)() == 322);
      assert(bind_r<int>(ft(), 1, 2, 3)() == 321);
    }
    {
      auto f = bind(ft(), 1, 2, 3, 4);
      assert(f() == 4321);
      assert(as_const(f)() == 4322);
      assert(bind_r<int>(ft(), 1, 2, 3, 4)() == 4321);
    }
  }
  {
    {
      {
        auto f = bind(ft(), _1, _2, _3, _4);
        auto fr = bind_r<long>(ft(), _1, _2, _3, _4);

        assert(f(1, 2, 3, 4) == 4321);
        assert(fr(1, 2, 3, 4) == 4321);
        static_assert(same_as<decltype(fr(1, 2, 3, 4)), long>);

        assert(as_const(f)(1, 2, 3, 4) == 4322);
        assert(as_const(fr)(1, 2, 3, 4) == 4322);
        static_assert(same_as<decltype(as_const(fr)(1, 2, 3, 4)), long>);
      }
      {
        auto f = bind(ft(), _4, _3, _2, _1);
        auto fr = bind_r<long>(ft(), _4, _3, _2, _1);

        assert(f(1, 2, 3, 4) == 1234);
        assert(fr(1, 2, 3, 4) == 1234);
        static_assert(same_as<decltype(fr(1, 2, 3, 4)), long>);

        assert(as_const(f)(1, 2, 3, 4) == 1235);
        assert(as_const(fr)(1, 2, 3, 4) == 1235);
        static_assert(same_as<decltype(as_const(fr)(1, 2, 3, 4)), long>);
      }
    }
    {
      {
        auto f = bind(ft(), _1, _2);
        auto fr = bind_r<long>(ft(), _1, _2);

        assert(f(1, 2, 3, 4) == 21);
        assert(fr(1, 2, 3, 4) == 21);
        static_assert(same_as<decltype(fr(1, 2, 3, 4)), long>);

        assert(as_const(f)(1, 2, 3, 4) == 22);
        assert(as_const(fr)(1, 2, 3, 4) == 22);
        static_assert(same_as<decltype(as_const(fr)(1, 2, 3, 4)), long>);
      }
      {
        auto f = bind(ft(), _2, _1);
        auto fr = bind_r<long>(ft(), _2, _1);

        assert(f(1, 2, 3, 4) == 12);
        assert(fr(1, 2, 3, 4) == 12);
        static_assert(same_as<decltype(fr(1, 2, 3, 4)), long>);

        assert(as_const(f)(1, 2, 3, 4) == 13);
        assert(as_const(fr)(1, 2, 3, 4) == 13);
        static_assert(same_as<decltype(as_const(fr)(1, 2, 3, 4)), long>);
      }
    }
    {
      {
        auto f = bind(ft(), _1);
        auto fr = bind_r<long>(ft(), _1);

        assert(f(1) == 1);
        assert(fr(1) == 1);
        static_assert(same_as<decltype(fr(1)), long>);

        assert(as_const(f)(1) == 2);
        assert(as_const(fr)(1) == 2);
        static_assert(same_as<decltype(as_const(fr)(1)), long>);
      }
      {
        auto f = bind(ft(), _1);
        auto fr = bind_r<long>(ft(), _1);

        assert(f(1, 2, 3) == 1);
        assert(fr(1, 2, 3) == 1);
        static_assert(same_as<decltype(fr(1, 2, 3)), long>);

        assert(as_const(f)(1, 2, 3) == 2);
        assert(as_const(fr)(1, 2, 3) == 2);
        static_assert(same_as<decltype(as_const(fr)(1, 2, 3)), long>);
      }
    }
  }
  {
    {
      auto swap_f = [](int &x, int &y) {
        adl_swap(x, y);
      };

      int x = 1, y = 2;
      auto f = bind(swap_f, ref(x), ref(y));
      f();
      assert(x == 2 && y == 1);
    }
    {
      auto add1_f = [](int &x) {
        ++x;
      };

      int x = 1;
      auto f = bind(add1_f, ref(x));
      f();
      assert(x == 2);
      f();
      assert(x == 3);
    }
    {
      auto add_f = [](int &x, int &y, int &z) {
        z = x + y;
        x = 0;
        y = 0;
      };

      int x = 1, y = 2, z = 0;
      auto f = bind(add_f, ref(x), ref(y), ref(z));
      f();
      assert(z == 3 && x == 0 && y == 0);
    }
  }
  {
    assert(bind(plus<>(), _1, _2)(3, 4) == 7);
    assert(bind(plus<>(), _1, 10)(3, 4) == 13);
    assert(bind_r<float>(plus<>(), _1, 10)(3, 4) / 2 == (float)13 / 2);

    assert(bind(negate<>(), 1)() == -1);
    assert(bind(negate<>(),
                bind(negate<>(), 1))() == 1);
    assert(bind(negate<>(),
                bind(negate<>(),
                     bind(negate<>(),
                          bind(negate<>(),
                               bind(negate<>(), 3)))))()
           == -3);
    assert(bind(negate<>(),
                bind(negate<>(),
                     bind(negate<>(),
                          bind(negate<>(), _1))))(4) == 4);

    assert(bind(plus<>(),
                bind(multiplies<>(), _3, _4),
                bind(divides<>(), _1, _2))(3, 2, 5, 6) == 31);

    assert(bind(plus<>(),
                bind(plus<>(),
                     bind(plus<>(), _1, _2),
                     bind(plus<>(), _3, _4)),
                bind(plus<>(), _5, _6))(1, 10, 100, 1000, 10000, 100000)
           == 111111);

    {
      owner_pointer<int> p(1), pp(2);
      auto f = [](owner_pointer<int> &&x, owner_pointer<int> &&y) {
        int r = *x + *y * 10;
        x = nullptr;
        y = nullptr;
        return r;
      };
      auto ff = bind(f, _1, _2);
      assert(ff(move(p), move(pp)) == 21);
      assert(p == nullptr && pp == nullptr);
    }
    {
      owner_pointer<int> p(1), pp(2);
      const auto f = bind([](auto x, auto y) {return *x * 10 + *y;},
                          move(p), move(pp));
      assert(p == nullptr && pp == nullptr);
      assert(f() == 12);
    }
    {
      owner_pointer<int> p(1), pp(2);
      const auto f = [](auto x, auto y) {return x * 10 + y;};
      const auto ff = bind([](const auto &x) {return *x;},
                           bind(make_from_tuple<owner_pointer<int>>,
                                bind(forward_as_tuple, _1)));
      assert(ff(3) == 3);
      const auto fff = bind(make_from_tuple<int>,
                            bind(forward_as_tuple, _1));
      assert(fff(1) == 1);
      const auto ffff = bind(f,
                             bind([](const auto &x) {return *x;},
                                  bind(make_from_tuple<owner_pointer<int>>,
                                       bind(forward_as_tuple, _3))),
                             bind(make_from_tuple<int>,
                                  bind(forward_as_tuple, _1)));
      assert(ffff(1, 2, 3) == 31);
    }
    {
      constexpr auto f = bind(minus<>{},
                              bind(multiplies<>{}, _1, _2),
                              bind(multiplies<>{}, _3, _4));
      static_assert(f(2, 3, 4, 5) == -14);
      static_assert(!noexcept(f(2, 3, 4, 5)));

      constexpr auto ff = bind_r<long>(minus<>{},
                                       bind(multiplies<>{}, _1, _2),
                                       bind(multiplies<>{}, _3, _4));
      static_assert(semiregular<decay_t<decltype(ff)>>);
      static_assert(!is_copy_assignable_v<decltype(ff)>);
      static_assert(is_trivially_copyable_v<decay_t<decltype(ff)>>);
      static_assert(is_nothrow_copy_constructible_v<decay_t<decltype(ff)>>);
      static_assert(is_nothrow_move_constructible_v<decay_t<decltype(ff)>>);
      static_assert(is_nothrow_copy_assignable_v<decay_t<decltype(ff)>>);
      static_assert(is_nothrow_move_assignable_v<decay_t<decltype(ff)>>);
      static_assert(is_nothrow_swappable_v<decay_t<decltype(ff)>>);
      static_assert(ff(2, 3, 4, 5) == -14);
      static_assert(!noexcept(ff(2, 3, 4, 5)));
      static_assert(same_as<decltype(ff(2, 3, 4, 5)), long>);
    }
    {
      owner_pointer a(1), b(2), c(3), d(4), e(5), f(6), g(7), h(8), i(9);
    }
  }
}

}
void test_bind() {
  // is_bind_expression
  // is_placeholder
  {
    static_assert(is_bind_expression<inner::be<plus<>, tuple<>>>::value);
    static_assert(is_bind_expression_v<inner::be<plus<>, tuple<>>>);
    static_assert(is_bind_expression_v<const inner::be<plus<>, tuple<>>>);
    static_assert(is_bind_expression_v<volatile inner::be<plus<>, tuple<>>>);
    static_assert(is_bind_expression_v
                  <const volatile inner::be<plus<>, tuple<>>>);
    static_assert(!is_bind_expression_v<int>);

    static_assert(is_placeholder<decltype(_1)>::value);
    static_assert(is_placeholder<inner::bpos<0>>::value);
    static_assert(is_placeholder_v<inner::bpos<0>>);
    static_assert(is_placeholder_v<inner::bpos<0>>);
    static_assert(is_placeholder_v<const inner::bpos<0>>);
    static_assert(is_placeholder_v<volatile inner::bpos<0>>);
    static_assert(is_placeholder_v<const volatile inner::bpos<0>>);
    static_assert(!is_placeholder_v<int>);
  }

  // bind
  help_bind::test();

  // bind_front
  {
    constexpr auto fzero = [](int a, int b) constexpr {return a * 10 + b;};
    static_assert(fzero(1, 2) == 12);
    constexpr auto ffzero = bind_front(fzero, 1);
    static_assert(ffzero(2) == 12);

    const auto f = [](int a, int b, int c, int d) {
      return a * 1000 + b * 100 + c * 10 + d;
    };
    auto ff = bind_front(f, 1, 2, 3, 4);
    adl_swap(ff, ff);
    assert(ff() == 1234);
    assert(bind_front(f, 1, 2, 3, 4)() == 1234);
    assert(bind_front(f, 1, 2, 3)(4) == 1234);
    assert(bind_front(f, 1, 2)(3, 4) == 1234);
    assert(bind_front(f, 1)(2, 3, 4) == 1234);
    assert(bind_front(f)(1, 2, 3, 4) == 1234);
    static_assert(!noexcept(bind_front(f)(1, 2, 3, 4)));

    owner_pointer<int> a(1), b(2), c(3);
    const auto ffff = [](owner_pointer<int> &x,
                         owner_pointer<int> &y,
                         owner_pointer<int> &z)->int {
      if (x == nullptr && y == nullptr && z == nullptr)
        return 1;
      else {
        int ret = *x * 100 + *y * 10 + *z;
        x = nullptr;
        y = nullptr;
        z = nullptr;
        return ret;
      }
    };
    auto fffff = bind_front(ffff, move(a), b);
    static_assert(semiregular<decay_t<decltype(fffff)>>);
    assert(a == nullptr && b != nullptr && *b == 2);
    assert(fffff(c) == 123);
    assert(a == nullptr && b != nullptr && *b == 2);
    assert(c == nullptr);
    assert(fffff(c) == 1);
    assert(a == nullptr && b != nullptr && *b == 2);
    assert(c == nullptr);

    int val{};
    const auto f0 = [&val]() {return 1;};
    const auto f00 = bind_front(f0);
    static_assert(is_copy_constructible_v<decay_t<decltype(f00)>>);
    static_assert(is_move_constructible_v<decay_t<decltype(f00)>>);
    static_assert(!is_copy_assignable_v<decay_t<decltype(f00)>>);
    static_assert(!is_move_assignable_v<decay_t<decltype(f00)>>);
    static_assert(!copyable<decay_t<decltype(f00)>>);
    static_assert(!movable<decay_t<decltype(f00)>>);
    static_assert(!swappable<decay_t<decltype(f00)>>);
    assert(f00() == 1);

    struct t {
      int operator ()(int x, int y) & {return x * 10 + y;}
      int operator ()(int x, int y) const & {return x * 100 + y * 10;}
      int operator ()(int x, int y) && {return x * 1000 + y * 100;}
      int operator ()(int x, int y) const && {return x * 10000 + y * 1000;}
    };
    auto f2 = bind_front(t{}, 1);
    assert(f2(2) == 12);
    assert(as_const(f2)(2) == 120);
    assert(move(f2)(2) == 1200);
    assert(move(as_const(f2))(2) == 12000);
  }

  // bind_back
  {
    constexpr auto fzero = [](int a, int b) constexpr {return a * 10 + b;};
    static_assert(fzero(1, 2) == 12);
    constexpr auto ffzero = bind_back(fzero, 1);
    static_assert(ffzero(2) == 21);

    const auto f = [](int a, int b, int c, int d) {
      return a * 1000 + b * 100 + c * 10 + d;
    };
    auto ff = bind_back(f, 1, 2, 3, 4);
    adl_swap(ff, ff);
    assert(ff() == 1234);
    assert(bind_back(f, 1, 2, 3, 4)() == 1234);
    assert(bind_back(f, 2, 3, 4)(1) == 1234);
    assert(bind_back(f, 3, 4)(1, 2) == 1234);
    assert(bind_back(f, 4)(1, 2, 3) == 1234);
    assert(bind_back(f)(1, 2, 3, 4) == 1234);
    static_assert(!noexcept(bind_back(f)(1, 2, 3, 4)));

    owner_pointer<int> a(1), b(2), c(3);
    const auto ffff = [](owner_pointer<int> &x,
                         owner_pointer<int> &y,
                         owner_pointer<int> &z)->int {
      if (x == nullptr && y == nullptr && z == nullptr)
        return 1;
      else {
        int ret = *x * 100 + *y * 10 + *z;
        x = nullptr;
        y = nullptr;
        z = nullptr;
        return ret;
      }
    };
    auto fffff = bind_back(ffff, move(b), c);
    static_assert(semiregular<decay_t<decltype(fffff)>>);
    assert(b == nullptr && c != nullptr && *c == 3);
    assert(a != nullptr && *a == 1);
    assert(fffff(a) == 123);
    assert(b == nullptr && c != nullptr && *c == 3);
    assert(a == nullptr);
    assert(fffff(a) == 1);
    assert(b == nullptr && c != nullptr && *c == 3);
    assert(a == nullptr);

    int val{};
    const auto f0 = [&val]() {return 1;};
    const auto f00 = bind_back(f0);
    static_assert(is_copy_constructible_v<decay_t<decltype(f00)>>);
    static_assert(is_move_constructible_v<decay_t<decltype(f00)>>);
    static_assert(!is_copy_assignable_v<decay_t<decltype(f00)>>);
    static_assert(!is_move_assignable_v<decay_t<decltype(f00)>>);
    static_assert(!copyable<decay_t<decltype(f00)>>);
    static_assert(!movable<decay_t<decltype(f00)>>);
    static_assert(!swappable<decay_t<decltype(f00)>>);
    assert(f00() == 1);

    struct t {
      int operator ()(int x, int y) & {return x * 10 + y;}
      int operator ()(int x, int y) const & {return x * 100 + y * 10;}
      int operator ()(int x, int y) && {return x * 1000 + y * 100;}
      int operator ()(int x, int y) const && {return x * 10000 + y * 1000;}
    };
    auto f2 = bind_back(t{}, 2);
    assert(f2(1) == 12);
    assert(as_const(f2)(1) == 120);
    assert(move(f2)(1) == 1200);
    assert(move(as_const(f2))(1) == 12000);
  }
}

void test_ratio() {
  // constructors
  {
    {
      constexpr ratio r;
      constexpr ratio r2(0, 99999999);
      constexpr ratio r3(0, -99999999);
      static_assert(r.num == 0 && r.den == 1);
      static_assert(r2.num == 0 && r2.den == 1);
      static_assert(r3.num == 0 && r3.den == 1);
      static_assert(r == r2);
      static_assert(r2 == r3);
    }
    {
      constexpr ratio r(3 * 9999, 5 * 9999);
      constexpr ratio r2(3 * 999999, 5 * 999999);
      constexpr ratio r3(-3, -5);
      constexpr ratio r4(-3 * 33, -5 * 33);
      constexpr ratio r5(-3 * 999, -5 * 999);
      assert(r.num == 3 && r.den == 5);
      static_assert(r == r2);
      static_assert(r2 == r3);
      static_assert(r3 == r4);
      static_assert(r4 == r5);

      constexpr ratio r6(3, -5);
      constexpr ratio r7(3 * 999, -5 * 999);
      constexpr ratio r8(-3 * 999, 5 * 999);
      static_assert(r6.num == -3 && r6.den == 5);
      static_assert(r6 == r7);
      static_assert(r7 == r8);
    }
  }

  // unary +-
  {
    static_assert(-+-+-+-+-+-ratio(1, 2) == ratio(1, 2));
    static_assert(-(-ratio(1, 2)) == ratio(1, 2));
    static_assert(-ratio(1, 2) == ratio(-1, 2));
    static_assert(-+-+-ratio(1, 2) == ratio(-1, 2));
  }

  // binary +-
  {
    static_assert(ratio(1, 2) + ratio(1, 2) == ratio(1, 1));
    static_assert(ratio(1, 2) + ratio(-1, 2) == ratio(0, 1));
    static_assert(ratio(1, 2) - ratio(1, 2) == ratio(0, 1));
    static_assert(ratio(1, 2) - ratio(-1, 2) == ratio(1, 1));

    static_assert(ratio(15, 17) + ratio(2, 17) == ratio(1, 1));
    static_assert(ratio(15, 17) - ratio(-2, 17) == ratio(1, 1));

    static_assert(ratio(159, 161) + ratio(666, 999)
                  == ratio(159 * 999 + 161 * 666, 161 * 999));
    static_assert(ratio(159, 161) - ratio(-666, 999)
                  == ratio(159 * 999 + 161 * 666, 161 * 999));
    static_assert(ratio(159, 161) + ratio(-666, 999)
                  == ratio(159 * 999 - 161 * 666, 161 * 999));
    static_assert(ratio(159, 161) - ratio(666, 999)
                  == ratio(159 * 999 - 161 * 666, 161 * 999));
  }

  //
  {
    static_assert(ratio(5, 6) * ratio(6, 5) == ratio(1, 1));
    static_assert(ratio(5, 6) * ratio(-6, 5) == ratio(-1, 1));

    static_assert(ratio(7 * 20, 9 * 9999) * ratio(11 * 9999, 13 * 20)
                  == ratio(7 * 11, 9 * 13));
    static_assert(ratio(-7 * 20, 9 * 9999) * ratio(11 * 9999, 13 * 20)
                  == ratio(-7 * 11, 9 * 13));
    static_assert(ratio(7 * 20, 9 * 9999) * ratio(-11 * 9999, 13 * 20)
                  == ratio(-7 * 11, 9 * 13));
    static_assert(ratio(-7 * 20, 9 * 9999) * ratio(-11 * 9999, 13 * 20)
                  == ratio(7 * 11, 9 * 13));

    static_assert(ratio(7 * 20, 9 * 9999)
                  / ratio(13 * 20, 11 * 9999)
                  == ratio(7 * 11, 9 * 13));

    static_assert(ratio(0, 1) / ratio(123, 456) == ratio(0, 1));
    static_assert(ratio(0, 1) / ratio(-123, 456) == ratio(0, 1));
  }

  // <=>
  {
    static_assert(ratio(16, 19) <=> ratio(16, 20) > 0);
    static_assert(ratio(16, 19) <=> ratio(16, 19) == 0);
    static_assert(ratio(-16, 19) <=> ratio(-16, 20) < 0);
    static_assert(ratio(-16, 19) <=> ratio(-16, 19) == 0);

    static_assert(ratio(-16, 19) != ratio(-16, 20));
  }

  // constexpr ratio objects
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

void test_iterator_tags() {
  // output_iterator_tag
  // input_iterator_tag
  // forward_iterator_tag
  // bidirectional_iterator_tag
  // random_access_iterator_tag
  // contiguous_iterator_tag
  {
    static_assert(is_trivial_empty_v<output_iterator_tag>);
    static_assert(is_trivial_empty_v<input_iterator_tag>);
    static_assert(is_trivial_empty_v<forward_iterator_tag>);
    static_assert(is_trivial_empty_v<bidirectional_iterator_tag>);
    static_assert(is_trivial_empty_v<random_access_iterator_tag>);
    static_assert(is_trivial_empty_v<contiguous_iterator_tag>);
  }
}

namespace help_optional {

static int test_t_count = 0;
class test_t {
  int data;

  void plus() {++test_t_count;}
  void minus() {--test_t_count;}

public:
  test_t() noexcept : data{} {plus();}
  ~test_t() {minus();}
  test_t(const test_t &x) : data(x.data) {plus();}
  test_t &operator =(const test_t &x) {data = x.data; return *this;}
  test_t(test_t &&x) noexcept : test_t(as_const(x)) {x.data = 0;}
  test_t &operator =(test_t &&x) noexcept {
    if (addressof(x) != this) {
      operator =(as_const(x));
      x.data = 0;
    }
    return *this;
  }
  friend void swap(test_t &x, test_t &y) noexcept {adl_swap(x.data, y.data);}

  bool operator ==(const test_t &x) const {
    return data == x.data;
  }
  strong_ordering operator <=>(const test_t &x) const {
    return data <=> x.data;
  }

  explicit test_t(int x) : data(x) {plus();}

  int operator *() const {
    return data;
  }
};

}
void test_optional() {
  using help_optional::test_t;
  using help_optional::test_t_count;

  // in_place(_t)
  // in_place_type(_t)
  // is_in_place_type(_v)
  // nullopt
  {
    static_assert(is_empty_v<decltype(in_place)>);
    static_assert(is_empty_v<decltype(in_place_type<int>)>);
    static_assert(is_empty_v<decltype(nullopt)>);

    static_assert(is_in_place_type_v<in_place_type_t<int>>);
    static_assert(is_in_place_type_v<const in_place_type_t<int>>);
    static_assert(is_in_place_type_v<volatile in_place_type_t<int>>);
    static_assert(is_in_place_type_v<const volatile in_place_type_t<int>>);
    static_assert(!is_in_place_type_v<in_place_type_t<int> &>);
    static_assert(!is_in_place_type_v<int>);
  }
  // smf - triviality
  {
    static_assert(!is_trivially_constructible_v<optional<int>>);
    static_assert(is_trivially_destructible_v<optional<int>>);
    static_assert(is_trivially_copy_constructible_v<optional<int>>);
    static_assert(is_trivially_copy_assignable_v<optional<int>>);
    static_assert(is_trivially_move_constructible_v<optional<int>>);
    static_assert(is_trivially_move_assignable_v<optional<int>>);
    {
      struct t {
        t() {}
        ~t() {}
        t(const t &) {}
        t &operator =(const t &) {return *this;}
        t(t &&) {}
        t &operator =(t &&) {return *this;}
      };
      static_assert(!is_trivially_destructible_v<optional<t>>);
      static_assert(!is_trivially_copy_constructible_v<optional<t>>);
      static_assert(!is_trivially_copy_assignable_v<optional<t>>);
      static_assert(!is_trivially_move_constructible_v<optional<t>>);
      static_assert(!is_trivially_move_assignable_v<optional<t>>);
    }
  }
  // smf - optional noexcept
  {
    struct t {
      t(const t &) {}
      t &operator =(const t &) {return *this;}
    };
    static_assert(!is_nothrow_copy_constructible_v<optional<t>>);
    static_assert(!is_nothrow_copy_assignable_v<optional<t>>);

    static_assert(is_nothrow_constructible_v<optional<int>>);
    static_assert(is_nothrow_copy_constructible_v<optional<int>>);
    static_assert(is_nothrow_copy_assignable_v<optional<int>>);
    static_assert(is_nothrow_move_constructible_v<optional<int>>);
    static_assert(is_nothrow_move_assignable_v<optional<int>>);
    static_assert(is_nothrow_swappable_v<optional<int>>);
  }
  // smf - optional delete
  {
    {
      struct t {
        t(const t &) = delete;
      };
      static_assert(!is_copy_constructible_v<optional<t>>);
    }
    {
      struct t {
        t &operator =(const t &) = delete;
      };
      static_assert(!is_copy_assignable_v<optional<t>>);
    }
    {
      struct t {
        t(t &&) = delete;
      };
      static_assert(!is_move_constructible_v<optional<t>>);
      static_assert(!is_swappable_v<optional<t>>);
    }
    {
      struct t {
        t &operator =(t &&) = delete;
      };
      static_assert(!is_move_assignable_v<optional<t>>);
      static_assert(!is_swappable_v<optional<t>>);
    }
  }
  // smf - default constructor and destructor
  {
    {
      optional<int> x;
      assert(!x.has_value() && x == nullopt);
      optional<int> y(4);
      assert(y.has_value() && y != nullopt && *y == 4);
    }
    {
      optional<test_t> x;
      assert(!x.has_value() && test_t_count == 0);
      optional<test_t> y(test_t(1));
      assert(y.has_value() && *y.value() == 1 && test_t_count == 1);
    }
    assert(test_t_count == 0);
  }
  // smf - copy
  {
    // optional<int>
    {
      optional<int> x;
      assert(!x.has_value());
      optional<int> y(1);
      assert(y.has_value() && y.value() == 1);
      {
        optional<int> z(x);
        assert(!z.has_value());
        optional<int> zz(y);
        assert(zz.has_value() && zz.value() == 1);
      }
      optional<int> z(2);
      assert(z.has_value() && z.value() == 2);

      assert(addressof(x = x) == addressof(x));
      assert(!x.has_value());
      assert(addressof(x = y) == addressof(x));
      assert(x.has_value() && x.value() == 1);
      assert(y.has_value() && y.value() == 1);
      assert(addressof(y = z) == addressof(y));
      assert(y.has_value() && y.value() == 2);
      assert(z.has_value() && z.value() == 2);
      z.reset();
      assert(!z.has_value());
      assert(addressof(y = z) == addressof(y));
      assert(!y.has_value() && !z.has_value());
    }
    // optional<test_t>
    {
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y(x);
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1));
        optional<test_t> y(x);
        assert(x.has_value() && *x.value() == 1);
        assert(y.has_value() && *y.value() == 1);
        assert(test_t_count == 2);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y(test_t(1));
        assert(addressof(x = x) == addressof(x));
        assert(!x.has_value() && y.has_value() && *y.value() == 1);
        assert(addressof(y = y) == addressof(y));
        assert(!x.has_value() && y.has_value() && *y.value() == 1);
        assert(test_t_count == 1);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y;
        assert(!x.has_value() && !y.has_value());
        assert(addressof(x = y) == addressof(x));
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1));
        optional<test_t> y;
        assert(x.has_value() && *x.value() == 1 && !y.has_value());
        assert(addressof(x = y) == addressof(x));
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y(test_t(1));
        assert(y.has_value() && *y.value() == 1 && !x.has_value());
        assert(addressof(x = y) == addressof(x));
        assert(x.has_value() && *x.value() == 1);
        assert(y.has_value() && *y.value() == 1);
        assert(test_t_count == 2);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1));
        optional<test_t> y(test_t(2));
        assert(x.has_value() && *x.value() == 1);
        assert(y.has_value() && *y.value() == 2);
        assert(addressof(x = y) == addressof(x));
        assert(x.has_value() && *x.value() == 2);
        assert(y.has_value() && *y.value() == 2);
        assert(test_t_count == 2);
      }
    }
  }
  // smf - move
  {
    // optional<int>
    {
      {
        optional<int> x;
        optional<int> y(move(x));
        assert(!x.has_value() && !y.has_value());
      }
      {
        optional<int> x(1);
        optional<int> y(move(x));
        assert(x.has_value() && x.value() == 1);
        assert(y.has_value() && y.value() == 1);
      }
      {
        optional<int> x;
        optional<int> y(1);
        assert(addressof(x = x) == addressof(x));
        assert(!x.has_value() && y.has_value() && y.value() == 1);
        assert(addressof(y = y) == addressof(y));
        assert(!x.has_value() && y.has_value() && y.value() == 1);
      }
      {
        optional<int> x;
        optional<int> y;
        assert(addressof(x = move(y)) == addressof(x));
        assert(!x.has_value() && !y.has_value());
      }
      {
        optional<int> x;
        optional<int> y(1);
        assert(addressof(x = move(y)) == addressof(x));
        assert(x.has_value() && x.value() == 1);
        assert(y.has_value() && y.value() == 1);
      }
      {
        optional<int> x(1);
        optional<int> y;
        assert(addressof(x = move(y)) == addressof(x));
        assert(!x.has_value() && !y.has_value());
      }
      {
        optional<int> x(1);
        optional<int> y(2);
        assert(addressof(x = move(y)) == addressof(x));
        assert(x.has_value() && x.value() == 2);
        assert(y.has_value() && y.value() == 2);
      }
    }
    // optional<test_t>
    {
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y(move(x));
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1));
        optional<test_t> y(move(x));
        assert(x.has_value() && *x.value() == 0);
        assert(y.has_value() && *y.value() == 1);
        assert(test_t_count == 2);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x;
        optional<test_t> y(test_t(1));
        assert(addressof(x = move(x)) == addressof(x));
        assert(!x.has_value() && y.has_value() && *y.value() == 1);
        assert(addressof(y = move(y)) == addressof(y));
        assert(!x.has_value() && y.has_value() && *y.value() == 1);
        assert(test_t_count == 1);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x, y;
        assert(addressof(x = move(y)) == addressof(x));
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1)), y;
        assert(addressof(x = move(y)) == addressof(x));
        assert(!x.has_value() && !y.has_value());
        assert(test_t_count == 0);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x, y(test_t(1));
        assert(addressof(x = move(y)) == addressof(x));
        assert(x.has_value() && *x.value() == 1);
        assert(y.has_value() && *y.value() == 0);
        assert(test_t_count == 2);
      }
      {
        assert(test_t_count == 0);
        optional<test_t> x(test_t(1)), y(test_t(2));
        assert(addressof(x = move(y)) == addressof(x));
        assert(x.has_value() && *x.value() == 2);
        assert(y.has_value() && *y.value() == 0);
        assert(test_t_count == 2);
      }
      assert(test_t_count == 0);
    }
  }
  // smf - swap
  {
    // optional<int>
    {
      {
        optional<int> x;
        optional<int> y(1);
        assert(!x.has_value() && y.value() == 1);
        adl_swap(x, x);
        assert(!x.has_value() && y.value() == 1);
        adl_swap(y, y);
        assert(!x.has_value() && y.value() == 1);
      }
      {
        optional<int> x, y;
        assert(!x.has_value() && !y.has_value());
        adl_swap(x, y);
        assert(!x.has_value() && !y.has_value());
        adl_swap(x, y);
        assert(!x.has_value() && !y.has_value());
        adl_swap(x, y);
        assert(!x.has_value() && !y.has_value());
      }
      {
        optional<int> x, y(1);
        assert(!x.has_value() && y.value() == 1);
        adl_swap(x, y);
        assert(!y.has_value() && x.value() == 1);
        adl_swap(x, y);
        assert(!x.has_value() && y.value() == 1);
        adl_swap(x, y);
        assert(!y.has_value() && x.value() == 1);
      }
      {
        optional<int> x(1), y(2);
        assert(x.value() == 1 && y.value() == 2);
        adl_swap(x, y);
        assert(x.value() == 2 && y.value() == 1);
        adl_swap(x, y);
        assert(x.value() == 1 && y.value() == 2);
        adl_swap(x, y);
        assert(x.value() == 2 && y.value() == 1);
      }
    }
    // optional<test_t>
    {
      {
        optional<test_t> x;
        optional<test_t> y(test_t(1));
        assert(!x.has_value() && *y.value() == 1);
        adl_swap(x, x);
        assert(!x.has_value() && *y.value() == 1);
        adl_swap(y, y);
        assert(!x.has_value() && *y.value() == 1);
      }
      {
        optional<test_t> x;
        optional<test_t> y;
        assert(!x.has_value() && !y.has_value());
        adl_swap(x, y);
        assert(!x.has_value() && !y.has_value());
      }
      {
        optional<test_t> x(test_t(1));
        optional<test_t> y;
        assert(*x.value() == 1 && !y.has_value());
        adl_swap(x, y);
        assert(*y.value() == 1 && !x.has_value());
        adl_swap(x, y);
        assert(*x.value() == 1 && !y.has_value());
        adl_swap(x, y);
        assert(*y.value() == 1 && !x.has_value());
      }
      {
        optional<test_t> x(test_t(1));
        optional<test_t> y(test_t(2));
        assert(*x.value() == 1 && *y.value() == 2);
        adl_swap(x, y);
        assert(*y.value() == 1 && *x.value() == 2);
      }
    }
  }
  // construct/assign with nullopt
  {
    // optional<int>
    {
      optional<int> x = nullopt;
      assert(!x.has_value());
      x = 1;
      assert(x.value() == 1);
      assert(addressof(x = nullopt) == addressof(x));
      assert(!x.has_value());
    }
    // optional<test_t>
    {
      optional<test_t> x = nullopt;
      assert(!x.has_value());
      x = test_t(1);
      assert(*x.value() == 1 && test_t_count == 1);
      assert(addressof(x = nullopt) == addressof(x));
      assert(!x.has_value() && test_t_count == 0);
    }
  }
  // in-place construct
  {
    {
      optional<int> x(in_place, 1);
      assert(x.value() == 1);
    }
    {
      optional<int> x(in_place);
      assert(x.value() == 0);
    }
    {
      struct t {
        int data;
        t(initializer_list<int> l, int x) : data(x) {}
      };
      optional<t> x(in_place, {1, 2, 3}, 1);
      assert(x.value().data == 1);
    }
  }
  // construct/assign with value_type
  {
    {
      optional<int> x = 1;
      assert(x.value() == 1);
      x = 2;
      assert(x.value() == 2);
    }
    {
      optional<test_t> x = test_t(1);
      assert(*x.value() == 1);
      x = test_t(2);
      assert(*x.value() == 2);
      test_t z(3);
      x = move(z);
      assert(*z == 0 && *x.value() == 3);
      assert(test_t_count == 2);
    }
  }
  // construct/assign with optional<other>
  {
    {
      optional<int> x;
      optional<long> y(x);
      optional<long> z(move(x));
      assert(!x.has_value() && !y.has_value() && !z.has_value());
    }
    {
      optional<int> x(1);
      optional<long> y(x);
      optional<long> z(move(x));
      assert(x.value() == 1);
      assert(y.value() == 1);
      assert(z.value() == 1);
    }
    {
      optional<int> x(1);
      optional<long> y;
      assert(addressof(y = x) == addressof(y));
      assert(y.value() == 1);
      assert(x.value() == 1);
    }
    {
      optional<int> x(1);
      optional<long> y;
      assert(addressof(y = move(x)) == addressof(y));
      assert(y.value() == 1);
      assert(x.value() == 1);
    }
    {
      optional<int> x;
      optional<long> y(1);
      assert(addressof(y = x) == addressof(y));
      assert(!y.has_value());
      assert(!x.has_value());
    }
    {
      optional<int> x;
      optional<long> y(1);
      assert(addressof(y = move(x)) == addressof(y));
      assert(!y.has_value());
      assert(!x.has_value());
    }
    {
      optional<int> x(1);
      optional<long> y(2);
      assert(addressof(y = x) == addressof(y));
      assert(x.value() == 1 && y.value() == 1);
    }
    {
      optional<int> x(1);
      optional<long> y(2);
      assert(addressof(y = move(x)) == addressof(y));
      assert(x.value() == 1 && y.value() == 1);
    }
  }
  // emplace
  {
    {
      optional<test_t> x;
      test_t &xx = x.emplace(1);
      assert(*x.value() == 1 && addressof(x.value()) == addressof(xx));
      optional<test_t> y(test_t(2));
      test_t &yy = y.emplace(3);
      assert(*y.value() == 3 && addressof(y.value()) == addressof(yy));
    }
    {
      struct t {
        int data;
        t(initializer_list<int> l, int x) : data(x) {}
      };
      optional<t> x;
      t &xx = x.emplace({1}, 2);
      assert(xx.data == 2 && addressof(x.value()) == addressof(xx));
      t &xxx = x.emplace({2, 3}, 4);
      assert(xxx.data == 4 && addressof(x.value()) == addressof(xxx));
    }
  }
  // ->
  {
    optional<int> x(1);
    static_assert(same_as<decltype(x.operator ->()), int *>);
    static_assert(same_as<decltype(as_const(x).operator ->()), const int *>);
    static_assert(same_as
                  <decltype(move(x).operator ->()), int *>);
    static_assert(same_as
                  <decltype(move(as_const(x)).operator ->()), const int *>);
    assert(*x.operator ->() == 1);
    assert(*as_const(x).operator ->() == 1);
    assert(*move(x).operator ->() == 1);
    assert(*move(as_const(x)).operator ->() == 1);
  }
  // *
  {
    optional<int> x(1);
    static_assert(same_as<decltype(*x), int &>);
    static_assert(same_as<decltype(*as_const(x)), const int &>);
    static_assert(same_as<decltype(*move(x)), int &&>);
    static_assert(same_as<decltype(*move(as_const(x))), const int &&>);
    assert(*x == 1);
    assert(*as_const(x) == 1);
    assert(*move(x) == 1);
    assert(*move(as_const(x)) == 1);
  }
  // explicit operator bool
  // has_value
  // empty
  // value
  {
    {
      optional<int> x;
      assert(x.empty() && !x && !x.has_value());
      x = 1;
      assert(x && x.has_value() && x.value() == 1);
      assert(x);
    }
#ifndef RE_NOEXCEPT
    optional<int> y;
    bool thrown = false;
    try {
      const int x = y.value();
    }
    catch (const bad_optional_access &) {
      thrown = true;
    }
    assert(thrown);
    thrown = false;
    y = 1;
    try {
      const int x = y.value();
      assert(x == 1);
    }
    catch (const bad_optional_access &) {
      thrown = true;
    }
    assert(!thrown);
#endif
  }
  // value_or
  {
    optional<test_t> x;
    static_assert(same_as<decltype(x.value_or(test_t(1))), test_t>);
    static_assert(same_as<decltype(as_const(x).value_or(test_t(1))), test_t>);
    static_assert(same_as<decltype(move(x).value_or(test_t(1))), test_t>);
    static_assert(same_as
                  <decltype(move(as_const(x)).value_or(test_t(1))), test_t>);
    assert(*x.value_or(test_t(1)) == 1);
    assert(*as_const(x).value_or(test_t(1)) == 1);
    assert(*move(x).value_or(test_t(1)) == 1);
    assert(*move(as_const(x)).value_or(test_t(1)) == 1);
  }
  // clear
  // reset
  // =(nullopt)
  {
    assert(test_t_count == 0);
    {
      optional<test_t> x;
      x.clear();
      assert(!x.has_value());
      x = test_t(1);
      assert(*x.value() == 1);
      x.clear();
      assert(!x.has_value());
    }
    {
      optional<test_t> x;
      x.reset();
      assert(!x.has_value());
      x = test_t(1);
      assert(*x.value() == 1);
      x.reset();
      assert(!x.has_value());
    }
    {
      optional<test_t> x;
      x = nullopt;
      assert(!x.has_value());
      x = test_t(1);
      assert(*x.value() == 1);
      x = nullopt;
      assert(!x.has_value());
    }
    assert(test_t_count == 0);
  }
  // and_then
  {
    const auto f = [](int x)->int {return x * 2;};
    const auto ff = []() {return optional<int>(0);};
    {
      optional<int> x;
      static_assert(same_as<int, decltype(x.and_then(f))>);
      static_assert(same_as<int, decltype(as_const(x).and_then(f))>);
      static_assert(same_as<int, decltype(move(x).and_then(f))>);
      static_assert(same_as<int, decltype(move(as_const(x)).and_then(f))>);
      assert(x.and_then(f) == 0);
      assert(as_const(x).and_then(f) == 0);
      assert(move(x).and_then(f) == 0);
      assert(move(as_const(x)).and_then(f) == 0);
      x = 1;
      assert(x.and_then(f) == 2);
      assert(as_const(x).and_then(f) == 2);
      assert(move(x).and_then(f) == 2);
      assert(move(as_const(x)).and_then(f) == 2);
    }
    {
      optional<int> x;
      static_assert(same_as<int, decltype(x.and_then(f))>);
      static_assert(same_as<int, decltype(as_const(x).and_then(f))>);
      static_assert(same_as<int, decltype(move(x).and_then(f))>);
      static_assert(same_as<int, decltype(move(as_const(x)).and_then(f))>);
      assert(x.and_then(f) == 0);
      assert(as_const(x).and_then(f) == 0);
      assert(move(x).and_then(f) == 0);
      assert(move(as_const(x)).and_then(f) == 0);
      x = 1;
      assert(x.and_then(f) == 2);
      assert(as_const(x).and_then(f) == 2);
      assert(move(x).and_then(f) == 2);
      assert(move(as_const(x)).and_then(f) == 2);
    }
    {
      optional<int> x;
      static_assert(same_as<optional<int>, decltype(x.transform(f))>);
      static_assert(same_as<optional<int>, decltype(as_const(x).transform(f))>);
      static_assert(same_as<optional<int>, decltype(move(x).transform(f))>);
      static_assert(same_as
                    <optional<int>, decltype(move(as_const(x)).transform(f))>);
      assert(!x.transform(f).has_value());
      assert(!as_const(x).transform(f).has_value());
      assert(!move(x).transform(f).has_value());
      assert(!move(as_const(x)).transform(f).has_value());
      x = 1;
      assert(x.transform(f).value() == 2);
      assert(as_const(x).transform(f).value() == 2);
      assert(move(x).transform(f).value() == 2);
      assert(move(as_const(x)).transform(f).value() == 2);
    }
    {
      optional<int> x;
      static_assert(same_as<optional<int>, decltype(x.or_else(ff))>);
      static_assert(same_as<optional<int>, decltype(as_const(x).or_else(ff))>);
      static_assert(same_as<optional<int>, decltype(move(x).or_else(ff))>);
      static_assert(same_as
                    <optional<int>, decltype(move(as_const(x)).or_else(ff))>);
      assert(x.or_else(ff).value() == 0);
      assert(as_const(x).or_else(ff).value() == 0);
      assert(move(x).or_else(ff).value() == 0);
      assert(move(as_const(x)).or_else(ff).value() == 0);
      x = 1;
      assert(x.or_else(ff).value() == 1);
      assert(as_const(x).or_else(ff).value() == 1);
      assert(move(x).or_else(ff).value() == 1);
      assert(move(as_const(x)).or_else(ff).value() == 1);
    }
  }
  // invoke
  {
    auto f = [](int x, int y) {return x + y;};
    optional<decltype(f)> x(f);
    assert(x(1, 2) == 3);
  }
  // compare with nullopt
  {
    optional<int> x;
    assert(x == nullopt && nullopt == x);
    assert(!(x != nullopt) && !(nullopt != x));
    assert((x <=> nullopt) == strong_eq);
    assert((nullopt <=> x) == strong_eq);
    x = 1;
    assert(!(x == nullopt) && !(nullopt == x));
    assert((x != nullopt) && (nullopt != x));
    assert((x <=> nullopt) == strong_gt);
    assert((nullopt <=> x) == strong_lt);
    assert(x > nullopt);
    assert(nullopt < x);
    assert(x >= nullopt);
    assert(nullopt <= x);
  }
  // compare with T
  {
    optional<int> x;
    assert(x != 0 && 0 != x && !(x == 0) && !(0 == x));
    assert(!(x > 0) && 0 > x
           && x < 0 && !(0 < x)
           && !(x >= 0) && 0 >= x
           && x <= 0 && !(0 <= x));
    assert((x <=> 0) == strong_lt);
    assert((0 <=> x) == strong_gt);
    x = 1;
    assert(x == 1 && 1 == x && !(x != 1) && !(1 != x));
    assert((x <=> 1) == strong_eq);
    assert(!(x == 0) && !(0 == x) && (x != 0) && (0 != x));
    assert((x <=> 0) == strong_gt);
    assert(!(x == 2) && !(2 == x) && (x != 2) && (2 != x));
    assert((x <=> 2) == strong_lt);
  }
  // compare with other optional
  {
    optional<int> x;
    optional<long> y;
    assert(x == y && y == x);
    assert((x <=> y) == strong_eq && (y <=> x) == strong_eq);
    x = 1;
    assert(!(x == y) && !(y == x));
    assert((x <=> y) == strong_gt && (y <=> x) == strong_lt);
    y = 0;
    assert(!(x == y) && !(y == x));
    assert((x <=> y) == strong_gt && (y <=> x) == strong_lt);
    y = 2;
    assert(!(x == y) && !(y == x));
    assert((x <=> y) == strong_lt && (y <=> x) == strong_gt);
    y = 1;
    assert(x == y && y == x);
    assert((x <=> y) == strong_eq && (y <=> x) == strong_eq);
  }
  // compare with optional
  {
    optional<int> x, y;
    assert(x == y && !(x != y));
    assert(!(x > y) && !(x < y));
    assert(x >= y && x <= y);
    assert(x <=> y == strong_eq);
    x = 1;
    assert(!(x == y) && !(y == x));
    assert(x != y && y != x);
    assert(x > y && !(y > x));
    assert(!(x < y) && y < x);
    assert(x >= y && !(y >= x));
    assert(!(x <= y) && y <= x);
    assert(x <=> y == strong_gt);
    assert(y <=> x == strong_lt);
    y = 1;
    assert(x == y && y == x);
    assert((x <=> y) == strong_eq && (y <=> x) == strong_eq);
    y = 2;
    assert(!(x == y) && !(y == x));
    assert((x <=> y) == strong_lt && (y <=> x) == strong_gt);
  }
}

void test_semiregular_function() {
  // assign_non_assignable
  {
    {
      auto f = [i = 0]() mutable {++i; return i;};
      auto ff = f;
      ff();
      assert(f() == 1 && ff() == 2);
      static_assert(noexcept(noexcept(assign_non_assignable
                                      <decltype(f)>(f, ff))));
      static_assert(noexcept(noexcept(assign_non_assignable
                                      <decltype(f)>(f, move(ff)))));
      assign_non_assignable<decltype(f)>(f, ff);
      assign_non_assignable<decltype(f)>(f, move(ff));
      assert(ff() == 3 && f() == 3);
    }
    {
      int (*f)() = []() {return 1;};
      int (*ff)() = []() {return 2;};
      static_assert(noexcept(noexcept(assign_non_assignable
                                      <decltype(f)>(f, ff))));
      static_assert(noexcept(noexcept(assign_non_assignable
                                      <decltype(f)>(f, move(ff)))));
      assign_non_assignable<decltype(f)>(f, ff);
      assign_non_assignable<decltype(f)>(f, move(ff));
      assert(f() == 2);
    }
  }
  // copyable_wrapper
  {
    {
      int x{};
      auto f = [x]() {
        return x;
      };
      using fw_t = copyable_wrapper<decltype(f)>;
      static_assert(!default_initializable<fw_t>);
      static_assert(copyable<fw_t>);
      static_assert(is_swappable_v<fw_t>);
      fw_t fw(f);
      assert((*fw)() == 0);
      assert((*to_address(fw))() == 0);
    }
    {
      struct ft {
        int x = 0;
        int operator ()() const {return x;}
      };
      static_assert(semiregular<ft>);
      static_assert(is_nothrow_default_constructible_v<ft>);
      static_assert(is_nothrow_copy_constructible_v<ft>);
      static_assert(is_nothrow_move_constructible_v<ft>);
      static_assert(is_nothrow_copy_assignable_v<ft>);
      static_assert(is_nothrow_move_assignable_v<ft>);
      static_assert(is_nothrow_swappable_v<ft>);
      copyable_wrapper<ft> f1, f2;
      (*f1).x = 1;
      (*f2).x = 2;
      assert((*f1)() == 1 && (*f2)() == 2);
      adl_swap(f1, f1);
      adl_swap(f1, f2);
      adl_swap(f2, f2);
      assert((*f1)() == 2 && (*f2)() == 1);
      f1 = f1;
      f1 = f2;
      f2 = f2;
      assert((*f1)() == 1 && (*f2)() == 1);
      (*f2).x = 2;
      f1 = move(f1);
      f1 = move(f2);
      f2 = move(f2);
      assert((*f1)() == 2 && (*f2)() == 2);
      copyable_wrapper<ft> f3(f1);
      assert((*f3)() == 2);
      assert((*f1)() == 2);
      copyable_wrapper<ft> f4(move(f1));
      assert((*f4)() == 2);
      assert((*f1)() == 2);
    }
    {
      struct ft {
        int x = 0;

        ft() = default;
        ~ft() = default;
        ft(const ft &) = default;
        ft &operator =(const ft &) = delete;
        ft(ft &&) = default;
        ft &operator =(ft &&) = delete;
      };
      using fw = copyable_wrapper<ft>;
      static_assert(is_nothrow_default_constructible_v<fw>);
      static_assert(is_nothrow_copy_constructible_v<fw>);
      static_assert(is_nothrow_move_constructible_v<fw>);
      static_assert(is_nothrow_copy_assignable_v<fw>);
      static_assert(is_nothrow_move_assignable_v<fw>);
      static_assert(is_nothrow_swappable_v<fw>);
      fw x, y;
      x->x = 1;
      y->x = 2;
      adl_swap(x, y);
      assert(x->x == 2 || y->x == 1);
      x = y;
      assert(x->x == 1 || y->x == 1);
      y->x = 2;
      x = move(y);
      assert(x->x == 2 || y->x == 2);
      auto z(x);
      auto zz(move(x));
      assert(x->x == 2 || y->x == 2);
      assert(z->x == 2 || zz->x == 2);
    }
    {
      struct ft {
        int x = 0;

        ft() = default;
        ~ft() = default;
        ft(const ft &y) noexcept : x(y.x) {}
        ft &operator =(const ft &) = delete;
        ft(ft &&y) : x(y.x) {}
        ft &operator =(ft &&) = delete;
      };
      using fw = copyable_wrapper<ft>;
      static_assert(is_swappable_v<fw>);
      static_assert(is_nothrow_default_constructible_v<fw>);
      static_assert(is_nothrow_copy_constructible_v<fw>);
      static_assert(!is_nothrow_move_constructible_v<fw>);
      static_assert(is_nothrow_copy_assignable_v<fw>);
      static_assert(!is_nothrow_move_assignable_v<fw>);
      static_assert(!is_nothrow_swappable_v<fw>);
      fw x, y;
      x->x = 1;
      y->x = 2;
      adl_swap(x, y);
      assert(x->x == 2 || y->x == 1);
      x = y;
      assert(x->x == 1 || y->x == 1);
      y->x = 2;
      x = move(y);
      assert(x->x == 2 || y->x == 2);
      auto z(x);
      auto zz(move(x));
      assert(x->x == 2 || y->x == 2);
      assert(z->x == 2 || zz->x == 2);
    }
  }

  // call_or_deref_call
  {
    struct t {int operator ()() {return 1;}};
    t x{};
    assert(call_or_deref_call(x) == 1);
    assert(call_or_deref_call(addressof(x)) == 1);
  }

  // semiregular_function
  {
    {
      int tmp = 1;
      auto f = []() {return 0;};
      auto f2 = [tmp]() {return tmp;};
      auto f3 = [&tmp]() {return tmp;};
      static_assert(semiregular<semiregular_function<decltype(f)>>);
      static_assert(semiregular<semiregular_function<decltype(f2)>>);
      static_assert(semiregular<semiregular_function<decltype(f3)>>);
      semiregular_function<decltype(f)> ff(f);
      assert(ff() == 0);
      semiregular_function<decltype(f2)> ff2(f2);
      assert(ff2() == 1);
      tmp = 2;
      assert(ff2() == 1);
      semiregular_function<decltype(f3)> ff3(f3);
      assert(ff3() == 2);
      tmp = 3;
      assert(ff3() == 3);
    }
    {
      auto f4 = [tmp = 1]() mutable {return tmp++;};
      auto f44 = f4;
      static_assert(semiregular<semiregular_function<decltype(f4) *>>);
      semiregular_function<decltype(f4) *> g(addressof(f4));
      assert(g() == 1);
      assert(g() == 2);
      assert(g() == 3);
      assert(f4() == 4);
      assert(g() == 5);
      semiregular_function<decltype(f4) *> gg{addressof(f44)};
      assert(addressof(g = gg) == addressof(g));
      assert(g() == 1);
      assert(g() == 2);
    }
    {
      const int tmp2 = 0;
      auto f5 = [tmp2](int &x, const int &y, int &&z) {
        return tmp2 * 1000 + (x++) * 100 + y * 10 + z;
      };
      static_assert(semiregular<semiregular_function<decltype(f5)>>);
      semiregular_function<decltype(f5)> ff5(f5);
      static_assert(is_invocable_v<decltype(ff5) &, int &, const int &, int &&>);
      static_assert(!is_invocable_v
                    <decltype(ff5) &, int &&, const int &, int &&>);
      static_assert(!is_invocable_v<decltype(ff5) &, int &, const int &, int &>);
    }
    {
      auto f1 = []() {return 1;};
      int tmp = 2;
      auto f2 = [&]() {return tmp;};
      semiregular_function<decltype(f1)> ff1(f1);
      semiregular_function<decltype(f2)> ff2(f2);
      assert(ff1.base()() == 1);
      assert(ff2.base()() == 2);
    }
  }
  // semiregular_fn
  {
    int tmp{};
    auto f = [&tmp]() {return tmp;};
    auto ff = semiregular_fn(f);
    static_assert(!semiregular<decltype(f)>);
    static_assert(semiregular<decltype(ff)>);
    tmp = 2;
    assert(ff() == 2);
    auto fff = ff;
    assert(fff() == 2);
    assert((ff = fff)() == 2);
  }
}

namespace help_is_template {

template <class X>
struct t {};
template <class X, class Y>
struct tt {};
template <class X, class Y, class Z>
struct ttt {};

}
void test_miscl() {
  // min_value
  // max_value
  {
    static_assert(is_trivial_empty_v<decltype(min_value)>);
    static_assert(is_trivial_empty_v<decltype(max_value)>);

    assert(min_value(1, 1) == 1);
    assert(max_value(1, 1) == 1);
    assert(min_value(1, 2) == 1);
    assert(max_value(1, 2) == 2);
    static_assert(min_value(1, 1) == 1);
    static_assert(max_value(1, 1) == 1);
    static_assert(min_value(1, 2) == 1);
    static_assert(max_value(1, 2) == 2);
    static_assert(!noexcept(min_value(1, 1)));
    static_assert(!noexcept(max_value(1, 1)));

    struct t {
      int x;
      int y;
      bool operator <(const t &z) const {return x < z.x;}
    };
    t x(1, 2), y(1, 3), z(2, 4);
    assert(min_value(x, y).y == 2);
    assert(max_value(x, y).y == 2);
    assert(min_value(y, x).y == 3);
    assert(max_value(y, x).y == 3);
    assert(min_value(x, z).x == 1);
    assert(min_value(z, x).x == 1);
    assert(max_value(x, z).x == 2);
    assert(max_value(z, x).x == 2);
  }

  // conditionally_enable_copy_and_move
  {
    using P = type_pack<true_type, false_type>;
    using PP = type_pack_mul<type_pack_mul<P, P>, type_pack_mul<P, P>>;
    type_pack_for_each<PP>([](auto y1_, auto y2_, auto y3_, auto y4_) {
        constexpr bool y1 = decltype(y1_)::type::value;
        constexpr bool y2 = decltype(y2_)::type::value;
        constexpr bool y3 = decltype(y3_)::type::value;
        constexpr bool y4 = decltype(y4_)::type::value;
        using X = conditionally_enable_copy_and_move<y1, y2, y3, y4>;
        static_assert(is_copy_constructible_v<X> == y1);
        static_assert(is_copy_assignable_v<X> == y2);
        static_assert(is_move_constructible_v<X> == y3);
        static_assert(is_move_assignable_v<X> == y4);
      });
  }

  // empty_function
  {
    static_assert(is_trivially_copyable_v<empty_function>);
    static_assert(is_trivially_constructible_v<empty_function>);
    constexpr empty_function f{};
    f(1, 2, 3, 4, 5);
  }

  // exit_fn
  {
    int i = 0;
    {
      const auto f = [&i]() {i = 1;};
      auto guard = exit_fn(f);
      using guard_t = decltype(guard);
      static_assert(!is_swappable_v<guard_t>);
      static_assert(!is_copy_constructible_v<guard_t>);
      static_assert(!is_copy_assignable_v<guard_t>);
      static_assert(is_move_constructible_v<guard_t>);
      static_assert(!is_move_assignable_v<guard_t>);
    }
    assert(i == 1);

    i = 0;
    {
      const auto f = [&i]() {i = 1;};
      auto guard = exit_fn(f, true);
      guard.unset();
      guard.set(true);
      guard.set();
    }
    assert(i == 1);

    i = 0;
    copy(exit_fn([&i]() {i = 1;}, true));
    assert(i == 1);

    {
      const auto f = []() constexpr {
        bool r = false;
        {
          const auto g = exit_fn([&]() {r = true;});
        }
        return r;
      };
      static_assert(f());
      static_assert(!noexcept(f()));

      const auto ff = []() constexpr {
        bool r = false;
        {
          auto g = exit_fn([&]() {r = true;}, false);
          g.set();
        }
        return r;
      };
      static_assert(ff());
    }
  }

  // copy
  {
    int x = copy(1);
    owner_pointer<int> y = copy(owner_pointer<int>{2});
    assert(x == 1);
    assert(*y == 2);
    static_assert(copy(copy(1)) == 1);
    static_assert(noexcept(copy(copy(1))));
  }
  // copy_if_rvalue
  {
    static_assert(noexcept(copy_if_rvalue(declval<int &>())));
    static_assert(noexcept(copy_if_rvalue(declval<const int &>())));
    static_assert(noexcept(copy_if_rvalue(declval<int &&>())));
    struct t {
      t(t &&) {}
    };
    static_assert(!noexcept(copy_if_rvalue(declval<t>())));

    static_assert(same_as<decltype(copy_if_rvalue(declval<int &>())), int &>);
    static_assert(same_as
                  <decltype(copy_if_rvalue(declval<const int &>())),
                   const int &>);
    static_assert(same_as<decltype(copy_if_rvalue(declval<int>())), int>);
    int x = 1;
    copy_if_rvalue(x) = 2;
    assert(x == 2);
    assert(addressof(copy_if_rvalue(as_const(x))) == addressof(x));
    assert(x == 2);
    decltype(auto) y = copy_if_rvalue(move(x));
    static_assert(same_as<decltype(y), int>);
    assert(x == 2 && y == 2 && addressof(x) != addressof(y));
  }

  // as_lvalue
  // is_lvalue
  // deref
  // assign_to
  {
    const auto f = [](int &) {};
    f(as_lvalue(1));
    static_assert(is_lvalue(as_lvalue(1)));
    static_assert(noexcept(as_lvalue(1)));
    constexpr auto f0 = []() {
      int x{};
      as_lvalue(x);
    };
    f0();

    const auto ff = [](int &&) {};
    int i = 0;
    ff(move(i));
    static_assert(!is_lvalue(move(i)));
    static_assert(noexcept(move(i)));
    constexpr auto f00 = []() {
      int x{};
      move(x);
    };
    f00();

    int *p = &i;
    assert(deref(p) == i);
    static_assert(!noexcept(deref(p)));
    constexpr auto fff = []() constexpr {
      int x{};
      return deref(addressof(x));
    };
    static_assert(fff() == 0);

    int x = 1, y = 2;
    assign_to(x, y);
    static_assert(!noexcept(assign_to(x, y)));
    assert(x == 1 && y == 1);
    constexpr auto ffff = []() constexpr {
      int a = 1, b = 2;
      assign_to(b, a);
      return a * 10 + b;
    };
    static_assert(ffff() == 22);
  }

  // is_allocator_aware
  {
    struct t {using allocator_type = void;};
    struct tt {};
    static_assert(is_allocator_aware_v<t>);
    static_assert(!is_allocator_aware_v<tt>);
  }

  // ccast
  {
    static_assert(ccast<float>(4) == 4.0f);
    static_assert(!noexcept(ccast<float>(4)));
  }

  // bit_cast
  {
    float x = 123.45f;
    const uint32_t y = bit_cast<uint32_t>(x);
    const float z = x;
    x = bit_cast<float>(y);
    static_assert(noexcept(bit_cast<float>(y)));
    assert(x == z);
  }

  // to_signed
  // to_unsigned
  {
    constexpr int x = -1;
    static_assert(noexcept(to_unsigned(x)));
    static_assert(same_as<decltype(to_unsigned(x)), unsigned>);
    constexpr unsigned y = to_unsigned(x);
    assert(y == numeric_limits<unsigned>::max());

    static_assert(noexcept(to_signed(y)));
    static_assert(same_as<decltype(to_signed(y)), int>);
    constexpr int z = to_signed(y);
    assert(y == to_unsigned(static_cast<int>(-1)));

    static_assert(same_as
                  <decltype(to_unsigned(declval<short>())),
                   unsigned short>);
    static_assert(same_as
                  <decltype(to_unsigned(declval<unsigned short>())),
                   unsigned short>);
    static_assert(same_as
                  <decltype(to_signed(declval<unsigned short>())), short>);
    static_assert(same_as
                  <decltype(to_signed(declval<short>())), short>);

    static_assert(to_unsigned(1) == 1);
    static_assert(to_unsigned(2ul) == 2);
    static_assert(to_signed(1) == 1);
    static_assert(to_signed(2ul) == 2);
  }

  // is_template
  {
    using help_is_template::t;
    using help_is_template::tt;
    using help_is_template::ttt;

    static_assert(!is_template<void, t>);
    static_assert(!is_template<int, t>);
    static_assert(!is_template<const int &, t>);
    static_assert(!is_template<const int &, t>);
    static_assert(is_template<t<int>, t>);
    static_assert(is_template<t<int> &, t>);
    static_assert(is_template<const t<int> &, t>);
    static_assert(is_template<t<int> &&, t>);
    static_assert(is_template<const t<int> &&, t>);
    static_assert(is_template<ttt<int, int, int>, ttt>);
    static_assert(is_template<ttt<int, int, int> &, ttt>);
    static_assert(is_template<const ttt<int, int, int> &, ttt>);
    static_assert(is_template<ttt<int, int, int> &&, ttt>);
    static_assert(is_template<const ttt<int, int, int> &&, ttt>);
  }

  // is_aligned
  {
    struct t {
      alignas(512) int value;
    };
    t x;
    assert(is_aligned(addressof(x), 512));
  }

  // div_round_up
  {
    assert(div_round_up(0, 5) == 0);
    assert(div_round_up(0, -5) == 0);
    assert(div_round_up(1, 5) == 1);
    assert(div_round_up(3, 5) == 1);
    assert(div_round_up(-3, 5) == -1);
    assert(div_round_up(33, 5) == 7);
    assert(div_round_up(-33, 5) == -7);
    assert(div_round_up(-33, -5) == 7);
    assert(div_round_up(33, -5) == -7);
    assert(div_round_up(2, 3) == 1);
    assert(div_round_up(-2, -3) == 1);
    assert(div_round_up(-3, 2) == -2);
    assert(div_round_up(3, -2) == -2);
  }

  // div_round
  {
    assert(div_round(255u + 128u, 255u) == 2u);
    assert(div_round(255u + 127u, 255u) == 1u);

    assert(div_round(6, 4) == 2);
    assert(div_round(5, 4) == 1);
    assert(div_round(-6, -4) == 2);
    assert(div_round(-5, -4) == 1);
    assert(div_round(-6, 4) == -2);
    assert(div_round(-5, 4) == -1);
    assert(div_round(6, -4) == -2);
    assert(div_round(5, -4) == -1);
  }

  // integral_cast
  {
#ifndef RE_NOEXCEPT
    const auto throwing = [](auto f) {
      bool y = false;
      try {
        f();
      }
      catch (const length_error &) {
        y = true;
      }
      catch (...) {}
      return y;
    };
    assert(throwing([]() {integral_cast<uint8_t>(999u);}));
    assert(throwing([]() {integral_cast<uint8_t>(999);}));
    assert(throwing([]() {integral_cast<uint8_t>(-1);}));
    assert(!throwing([]() {integral_cast<int8_t>(-1);}));
    assert(!throwing([]() {integral_cast<int8_t>(-128);}));
    assert(throwing([]() {integral_cast<int8_t>(-129);}));
#endif
  }

  // call_by_n_times
  {
    assert(call_by_n_times(bind(plus{}, _1, 1), 1, -1) == 2);
    assert(call_by_n_times(bind(plus{}, _1, 1), 1, 0) == 2);
    assert(call_by_n_times(bind(plus{}, _1, 1), 1, 1) == 2);
    assert(call_by_n_times(bind(plus{}, _1, 1), 1, 3) == 4);
  }

  // non_propagating_cache
  {
    // special member functions
    {
      static_assert(semiregular<non_propagating_cache<int>>);

      non_propagating_cache<int> x;
      assert(x.empty());
      non_propagating_cache<int> y(in_place, int{});
      assert(!y.empty() && *y == 0);

      non_propagating_cache<int> z(x);
      assert(z.empty());
      assert(x.empty());

      non_propagating_cache<int> zz(y);
      assert(zz.empty());
      assert(!y.empty() && *y == 0);

      assert(addressof(x = x) == addressof(x));
      assert(x.empty());
      assert(addressof(y = y) == addressof(y));
      assert(y.empty());

      assert(addressof(x = move(x)) == addressof(x));
      assert(x.empty());
      y.emplace(1);
      assert(!y.empty() && *y == 1);
      assert(addressof(y = move(y)) == addressof(y));
      assert(y.empty());
      y.emplace(1);
      assert(!y.empty() && *y == 1);
      assert(addressof(x = move(y)) == addressof(x));
      assert(y.empty());

      y.emplace(1);
      assert(x.empty());
      assert(!y.empty() && *y == 1);
      adl_swap(x, x);
      assert(x.empty());
      adl_swap(y, y);
      assert(y.empty());
      y.emplace(1);
      assert(!y.empty() && *y == 1);
      x.emplace(0);
      assert(!x.empty() && *x == 0);
      adl_swap(x, y);
      assert(x.empty() && y.empty());
    }
    // non_propagating_cache(in_place, s...)
    {
      non_propagating_cache<int> x(in_place, 1);
      assert(!x.empty() && *x == 1);
      non_propagating_cache<pair<int, int>> y(in_place, 1, 2);
      assert(!y.empty() && y->first == 1 && y->second == 2);
    }
    // emplace
    // emplace_or_assign
    // clear
    // empty
    // ->
    // *
    {
      non_propagating_cache<int> x;
      assert(x.empty());
      x.emplace(1);
      assert(!x.empty());
      assert(*x == 1 && *as_const(x) == 1);
      assert(*x.operator ->() == 1 && *as_const(x).operator ->() == 1);
      static_assert(same_as<decltype(*x), int &>);
      static_assert(same_as<decltype(*as_const(x)), const int &>);
      x.clear();
      assert(x.empty());
      x.clear();
      assert(x.empty());
      x.emplace_or_assign(1);
      assert(!x.empty() && *x == 1);
      x.emplace_or_assign(2);
      assert(!x.empty() && *x == 2);
    }
  }

  // simple_wrapper
  {
    static_assert(is_default_constructible_v<simple_wrapper<int>>);
    static_assert(is_trivially_copyable_v<simple_wrapper<int>>);
    simple_wrapper<int> x;
    assert(*x == 0);
    assert(*as_const(x) == 0);
    simple_wrapper<int> y(in_place, 1);
    assert(*y == 1);
    assert(*as_const(y) == 1);
    assert(*y.operator ->() == 1);
    assert(*as_const(y).operator ->() == 1);
  }

  // copy_and_swap
  {
    int x = 1, y = 2;
    copy_and_swap(y, x);
    assert(x == 2 && y == 2);
  }
  {
    struct t {
      int value;

      t() = default;
      ~t() = default;
      t(const t &) = default;
      t &operator =(const t &) = default;
      t(t &&x) : value(x.value) {x.value = 0;}
      t &operator =(t &&) = default;

      explicit t(int x) : value(x) {}
    };
    t x(1), y(2);
    copy_and_swap(move(y), x);
    assert(y.value == 0 && x.value == 2);
  }
  // copy_and_move
  {
    int x = 1, y = 2;
    copy_and_move(y, x);
    assert(x == 2 && y == 2);
  }

  // derivable_wrapper
  {
    static_assert(semiregular<derivable_wrapper<int>>);
    struct t {};
    static_assert(semiregular<derivable_wrapper<t>>);
    {
      derivable_wrapper<int> x(in_place, 1);
      assert(*x.operator ->() == 1 && *x == 1);
      assert(*as_const(x).operator ->() == 1 && *as_const(x) == 1);
      *x = 2;
      assert(*x == 2);
      derivable_wrapper<int> y(in_place, 3);
      adl_swap(x, y);
      assert(*x == 3 && *y == 2);
    }
    {
      derivable_wrapper<simple_wrapper<int>> x
        (in_place, simple_wrapper<int>(in_place, 1));
      assert(**x.operator ->() == 1 && **x == 1);
      assert(**as_const(x).operator ->() == 1 && **as_const(x) == 1);
      **x = 2;
      assert(**x == 2);
      derivable_wrapper<simple_wrapper<int>> y
        (in_place, simple_wrapper<int>(in_place, 3));
      adl_swap(x, y);
      assert(**x == 3 && **y == 2);
    }
  }

  // function_return_false
  {
    auto f1 = [](int) {};
    auto f2 = [](int) {return false;};
    auto f3 = [](int) {return true;};

    assert(!function_return_false(f1, 1));
    assert(function_return_false(f2, 1));
    assert(!function_return_false(f3, 1));
  }

  // l_shift
  // r_shift
  // b_or
  // b_and
  // b_xor
  {
    uint16_t x = 0b10111;
    assert(l_shift<uint16_t>(x, 5) == 0b1011100000u);
    assert(r_shift<uint16_t>(x, 2) == 0b101u);
    assert(b_or<uint16_t>(x, 0b1000) == 0b11111u);
    assert(b_and<uint16_t>(x, 0b0110) == 0b110);
    assert(b_xor<uint16_t>(x, 0b0110) == 0b10001);
  }

  // no_overflow_adding
  {
    using int_t = signed char;
    using uint_t = unsigned char;

    assert(no_overflow_adding((int_t)1, (int_t)126));
    assert(no_overflow_adding((int_t)126, (int_t)1));
    assert(!no_overflow_adding((int_t)2, (int_t)126));
    assert(!no_overflow_adding((int_t)126, (int_t)2));
    assert(no_overflow_adding((int_t)-1, (int_t)-127));
    assert(no_overflow_adding((int_t)-127, (int_t)-1));
    assert(!no_overflow_adding((int_t)-2, (int_t)-127));
    assert(!no_overflow_adding((int_t)-127, (int_t)-2));
    assert(no_overflow_adding((int_t)10, (int_t)-20));
    assert(no_overflow_adding((int_t)-20, (int_t)10));
    assert(no_overflow_adding((int_t)-10, (int_t)20));
    assert(no_overflow_adding((int_t)20, (int_t)-10));

    assert(no_overflow_adding((uint_t)254, (uint_t)1));
    assert(no_overflow_adding((uint_t)1, (uint_t)254));
    assert(!no_overflow_adding((uint_t)254, (uint_t)2));
    assert(!no_overflow_adding((uint_t)2, (uint_t)254));
  }

  // add_with_overflow_check
  {
    const unsigned char a = 100;
    const unsigned char b = 200;
    const unsigned char c = 55;
    const unsigned char d = 56;
    assert(add_with_overflow_check(a, b).empty());
    assert(add_with_overflow_check(b, d).empty());
    assert(add_with_overflow_check(b, c).value() == 255u);
    assert(add_with_overflow_check(a, a, d).empty());
    assert(add_with_overflow_check(a, d, a).empty());
    assert(add_with_overflow_check(d, a, a).empty());
    assert(add_with_overflow_check(a, a, c).value() == 255u);
    assert(add_with_overflow_check(a, c, a).value() == 255u);
    assert(add_with_overflow_check(c, a, a).value() == 255u);
  }

  // max_uint_of_max_size
  {
    static_assert(same_as<max_uint_of_max_size<16>, uint64_t>);
    static_assert(same_as<max_uint_of_max_size<9>, uint64_t>);
    static_assert(same_as<max_uint_of_max_size<8>, uint64_t>);
    static_assert(same_as<max_uint_of_max_size<7>, uint32_t>);
    static_assert(same_as<max_uint_of_max_size<6>, uint32_t>);
    static_assert(same_as<max_uint_of_max_size<5>, uint32_t>);
    static_assert(same_as<max_uint_of_max_size<4>, uint32_t>);
    static_assert(same_as<max_uint_of_max_size<3>, uint16_t>);
    static_assert(same_as<max_uint_of_max_size<2>, uint16_t>);
    static_assert(same_as<max_uint_of_max_size<1>, uint8_t>);
    static_assert(same_as<max_uint_of_max_size<0>, void>);
  }
}

void test_floating_point_traits() {
  // float_traits
  {
    using t = float_traits;
    static_assert(is_same_v<t::float_t, float>);
    static_assert(is_same_v<t::uint_t, uint32_t>);
    static_assert(is_same_v<t::int_t, int32_t>);
    static_assert(t::n == 8u);
    static_assert(t::k == 23u);
    static_assert(t::bias == 127u);

    assert(!t::is_positive(-1.0f));
    assert(t::is_positive(+1.0f));
    assert(t::is_negative(-1.0f));
    assert(!t::is_negative(+1.0f));

    static_assert(t::e_max == 255u);
    static_assert(1 - to_signed(t::bias) == -126);
    static_assert(to_signed(t::e_max) - 1 - to_signed(t::bias) == 127);
    assert(t::f_max == t::f(t::max()));
    assert(t::e_max - 1u == t::e(t::max()));

    assert(t::e(-1.0f) - to_signed(t::bias) == 0u);

    assert(t::f(-1.0f) == 0u);

    assert(t::is_normalized(-1.0f));
    assert(!t::is_denormalized(-1.0f));
    assert(!t::is_infinity(-1.0f));

    static_assert(t::is_specialized);
    static_assert(t::is_signed);
    static_assert(!t::is_integer);
    assert(t::lowest() < 0.0f);
    assert(t::denorm_min() > 0.0f);
    assert(t::min() > 0.0f);
    assert(t::max() > 0.0f);

    assert(t::is_infinity(t::positive_inf()));
    assert(t::is_positive(t::positive_inf()));
    assert(t::f(t::positive_inf()) == 0);
    assert(t::e(t::positive_inf()) == t::e_max);
    assert(t::is_infinity(numeric_limits<float>::infinity()));
    assert(t::is_positive(numeric_limits<float>::infinity()));
    assert(t::f(numeric_limits<float>::infinity()) == 0);
    assert(t::e(numeric_limits<float>::infinity()) == t::e_max);

    assert(t::is_infinity(t::negative_inf()));
    assert(t::is_negative(t::negative_inf()));
    assert(t::f(t::negative_inf()) == 0);
    assert(t::e(t::negative_inf()) == t::e_max);

    assert(t::is_nan(t::positive_nan()));
    assert(t::e(t::positive_nan()) == t::e_max);
    assert(t::is_positive(t::positive_nan()));
    assert(t::is_nan(numeric_limits<float>::nan()));
    assert(t::e(numeric_limits<float>::nan()) == t::e_max);
    assert(t::is_positive(numeric_limits<float>::nan()));
  }
  // double_traits
  {
    using t = double_traits;
    static_assert(is_same_v<t::float_t, double>);
    static_assert(is_same_v<t::uint_t, uint64_t>);
    static_assert(is_same_v<t::int_t, int64_t>);
    static_assert(t::n == 11u);
    static_assert(t::k == 52u);
    static_assert(t::bias == 1023u);

    assert(!t::is_positive(-1.0));
    assert(t::is_positive(+1.0));
    assert(t::is_negative(-1.0));
    assert(!t::is_negative(+1.0));

    static_assert(t::e_max == 2047u);
    assert(1 - to_signed(t::bias) == -1022);
    assert(to_signed(t::e_max) - 1 - to_signed(t::bias) == 1023);
    assert(t::f_max == t::f(t::max()));
    assert(t::e_max - 1u == t::e(t::max()));

    assert(t::e(-1.0) - to_signed(t::bias) == 0u);

    assert(t::f(-1.0) == 0u);

    assert(t::is_normalized(-1.0));
    assert(!t::is_denormalized(-1.0));
    assert(!t::is_infinity(-1.0));

    static_assert(t::is_specialized);
    static_assert(t::is_signed);
    static_assert(!t::is_integer);
    assert(t::lowest() < 0.0);
    assert(t::denorm_min() > 0.0);
    assert(t::min() > 0.0);
    assert(t::max() > 0.0);

    assert(t::is_infinity(t::positive_inf()));
    assert(t::is_positive(t::positive_inf()));
    assert(t::f(t::positive_inf()) == 0);
    assert(t::e(t::positive_inf()) == t::e_max);
    assert(t::is_infinity(numeric_limits<double>::infinity()));
    assert(t::is_positive(numeric_limits<double>::infinity()));
    assert(t::f(numeric_limits<double>::infinity()) == 0);
    assert(t::e(numeric_limits<double>::infinity()) == t::e_max);

    assert(t::is_infinity(t::negative_inf()));
    assert(t::is_negative(t::negative_inf()));
    assert(t::f(t::negative_inf()) == 0);
    assert(t::e(t::negative_inf()) == t::e_max);

    assert(t::is_nan(t::positive_nan()));
    assert(t::e(t::positive_nan()) == t::e_max);
    assert(t::is_positive(t::positive_nan()));
    assert(t::is_nan(numeric_limits<double>::nan()));
    assert(t::e(numeric_limits<double>::nan()) == t::e_max);
    assert(t::is_positive(numeric_limits<double>::nan()));
  }

  // hash
  {
    assert(hash{}(+0.0) == hash{}(-0.0));
    assert(hash{}(+0.0f) == hash{}(-0.0f));
    assert(bit_cast<uint32_t>(+0.0f) != bit_cast<uint32_t>(-0.0f));
    assert(bit_cast<uint64_t>(+0.0) != bit_cast<uint64_t>(-0.0));
  }
}

void test_std_fpfns() {
  // ceil
  // floor
  // round
  // trunc
  {
    assert(bind(ceil, _1)(0.5f) == 1.0f);
    assert(bind(ceil, _1)(0.5) == 1.0);
    assert(bind(ceil, _1)(-0.5f) == 0.0f);
    assert(bind(ceil, _1)(-0.5) == 0.0);

    assert(bind(floor, _1)(0.5f) == 0.0f);
    assert(bind(floor, _1)(0.5) == 0.0);
    assert(bind(floor, _1)(-0.5f) == -1.0f);
    assert(bind(floor, _1)(-0.5) == -1.0);

    assert(bind(round, _1)(0.5f) == 1.0f);
    assert(bind(round, _1)(0.5) == 1.0);
    assert(bind(round, _1)(-0.5f) == -1.0f);
    assert(bind(round, _1)(-0.5) == -1.0);

    assert(bind(trunc, _1)(0.6f) == 0.0f);
    assert(bind(trunc, _1)(0.6) == 0.0);
    assert(bind(trunc, _1)(-0.6f) == 0.0f);
    assert(bind(trunc, _1)(-0.6) == 0.0);
  }

  // fpclassify
  // isfinite
  // isinf
  // isnan
  // isnormal
  // signbit
  // isunordered
  {
    assert(bind(fpclassify, _1)(0.0f) == FP_ZERO);
    assert(bind(fpclassify, _1)(0.0) == FP_ZERO);

    assert(bind(isfinite, _1)(0.0f) == true);
    assert(bind(isfinite, _1)(0.0) == true);
    assert(bind(isfinite, _1)(INFINITY) == false);
    assert(bind(isfinite, _1)((double)INFINITY) == false);

    assert(bind(isinf, _1)(0.0f) == false);
    assert(bind(isinf, _1)(0.0) == false);
    assert(bind(isinf, _1)(INFINITY) == true);
    assert(bind(isinf, _1)((double)INFINITY) == true);

    assert(bind(isnan, _1)(2.0f) == false);
    assert(bind(isnan, _1)(2.0) == false);
    assert(bind(isnan, _1)(NAN) == true);
    assert(bind(isnan, _1)((double)NAN) == true);

    assert(bind(isnormal, _1)(2.0f) == true);
    assert(bind(isnormal, _1)(2.0) == true);

    assert(bind(signbit, _1)(0.0f) == false);
    assert(bind(signbit, _1)(0.0) == false);
    assert(bind(signbit, _1)(-0.0f) == true);
    assert(bind(signbit, _1)(-0.0) == true);

    assert(bind(isunordered, _1, _2)(NAN, 1.0f) == true);
    assert(bind(isunordered, _1, _2)(NAN, NAN) == true);
    assert(bind(isunordered, _1, _2)(NAN, INFINITY) == true);
    assert(bind(isunordered, _1, _2)(INFINITY, INFINITY) == false);
    assert(bind(isunordered, _1, _2)((double)INFINITY, (double)INFINITY)
           == false);
    assert(bind(isunordered, _1, _2)(1.0f, INFINITY) == false);
    assert(1.0f < INFINITY);
  }
}

void test_base() {
  printf("base: ");

  inner::fns::test_std_fpfns();
  inner::fns::test_basic_tmp_tools();
  inner::fns::test_swap();
  inner::fns::test_type_traits();
  inner::fns::test_utility_functions();
  inner::fns::test_reference_wrapper_and_invoke();
  inner::fns::test_language_related_concepts();
  inner::fns::test_concept_constrained_comparisons();
  inner::fns::test_integral_traits();
  inner::fns::test_numeric_limits();
  inner::fns::test_three_way_comparison();
  inner::fns::test_type_index();
  inner::fns::test_basic_function_objects();
  inner::fns::test_tuple();
  inner::fns::test_bind();
  inner::fns::test_ratio();
  inner::fns::test_iterator_tags();
  inner::fns::test_optional();
  inner::fns::test_semiregular_function();
  inner::fns::test_miscl();
  inner::fns::test_floating_point_traits();
  inner::fns::test_std_fpfns();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_base();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
