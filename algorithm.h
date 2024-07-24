#ifndef RE_DEFINED_ALGORITHM_H
#define RE_DEFINED_ALGORITHM_H

#include "base.h"
#include "range.h"
#include "allocator.h"
#include "dynamic.h"
#include "random.h"

// todo:
//   boyer_moore_searcher
//   two_way_searcher

// equal
namespace re {

namespace inner::fns {

template <class IR1, class IR2>
constexpr bool equal_impl_r_and_r2_sz_neq(IR1 &&r, IR2 &&r2) {
  return false;
}
template <class IR1, class IR2>
constexpr bool equal_impl_r_and_r2_sz_neq(IR1 &&r, IR2 &&r2)
  requires rng_is_sized<IR1> && rng_is_sized<IR2> {
  return size(r) != size(r2);
}

}
struct fo_equal {
  template <class IR1, class IR2, class BF>
  constexpr bool operator ()(IR1 &&r, IR2 &&r2, BF eq) const
    requires (is_rng<IR1> && is_rng<IR2>) {
    if (inner::fns::equal_impl_r_and_r2_sz_neq(r, r2))
      return false;

    auto it = begin(r);
    const auto ed = end(r);
    auto it2 = begin(r2);
    const auto ed2 = end(r2);
    for (;;) {
      if (it == ed)
        return it2 == ed2;
      else {
        if (it2 == ed2)
          return false;
        else {
          if (!eq(*it, *it2))
            return false;
          ++it;
          ++it2;
        }
      }
    }
  }
  template <class IR, class IR2>
  constexpr bool operator ()(IR &&r, IR2 &&r2) const
    requires (is_rng<IR> && is_rng<IR2>) {
    if constexpr (is_scalar_v<rng_vt<IR>>
                  && is_same_v<rng_vt<IR>, rng_vt<IR2>>
                  && is_citr<rng_itr<IR>>
                  && is_citr<rng_itr<IR2>>) {
      if (!is_constant_evaluated()) {
        const auto sz = size(r);
        if (sz != size(r2))
          return false;
        if (to_address(begin(r2)) != nullptr)
          return memcmp(static_cast<const void *>(to_address(begin(r))),
                        static_cast<const void *>(to_address(begin(r2))),
                        sizeof(rng_vt<IR>) * size(r)) == 0;
        else
          return true;
      }
      else {
        return operator ()(r, r2, equal_to{});
      }
    }
    else {
      return operator ()(r, r2, equal_to{});
    }
  }

  template <class IR, class II, class BF>
  constexpr bool operator ()(IR &&r, const II &it, BF eq) const
    requires (is_rng<IR> && !is_rng<II>) {
    II i = it;
    for (auto &p : iters(r)) {
      if (!eq(*p, *i))
        return false;
      ++i;
    }
    return true;
  }
  template <class IR, class II>
  constexpr bool operator ()(IR &&r, const II &it) const
    requires (is_rng<IR> && !is_rng<II>) {
    if constexpr (is_scalar_v<rng_vt<IR>>
                  && is_same_v<rng_vt<IR>, itr_vt<II>>
                  && is_citr<rng_itr<IR>> && is_citr<II>) {
      if (!is_constant_evaluated()) {
        if (to_address(it) != nullptr)
          return memcmp(static_cast<const void *>(to_address(begin(r))),
                        static_cast<const void *>(to_address(it)),
                        sizeof(rng_vt<IR>) * size(r)) == 0;
        else
          return true;
      }
      else {
        return operator ()(r, it, equal_to{});
      }
    }
    else {
      return operator ()(r, it, equal_to{});
    }
  }
};
inline constexpr fo_equal equal{};

}

// all_of
// any_of
// none_of
// all_of_equal
// any_of_equal
// none_of_equal
namespace re {

struct fo_all_of {
  template <class IR, class UF>
  constexpr bool operator ()(IR &&r, UF eq) const requires is_rng<IR> {
    for (auto &p : iters(r))
      if (!eq(*p))
        return false;
    return true;
  }
};
inline constexpr fo_all_of all_of{};
struct fo_any_of {
  template <class IR, class UF>
  constexpr bool operator ()(IR &&r, UF eq) const requires is_rng<IR> {
    return !all_of(r, not_fn(eq));
  }
};
inline constexpr fo_any_of any_of{};
struct fo_none_of {
  template <class IR, class UF>
  constexpr bool operator ()(IR &&r, UF eq) const requires is_rng<IR> {
    return !any_of(r, eq);
  }
};
inline constexpr fo_none_of none_of{};
struct fo_all_of_equal {
  template <class IR, class T, class BF = equal_to<>>
  constexpr bool operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return all_of(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_all_of_equal all_of_equal{};
struct fo_any_of_equal {
  template <class IR, class T, class BF = equal_to<>>
  constexpr bool operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return any_of(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_any_of_equal any_of_equal{};
struct fo_none_of_equal {
  template <class IR, class T, class BF = equal_to<>>
  constexpr bool operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return none_of(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_none_of_equal none_of_equal{};

}

// for_each(_plus)
// for_each_excluding_first
// for_each_excluding_last
namespace re {

struct fo_for_each_plus {
  template <class IR, class II, class UFN>
  constexpr pair<rng_itr<IR>, II> operator ()(IR &&r, const II &i, UFN f) const
    requires (is_rng<IR> && !is_rng<II>) {
    const auto ed = end(r);
    auto it = begin(r);
    auto it2 = i;
    while (it != ed) {
      f(*it, *it2);
      ++it;
      ++it2;
    }
    return pair(it, it2);
  }
};
inline constexpr fo_for_each_plus for_each_plus{};

struct fo_for_each {
  template <class IR, class UFN>
  constexpr rng_itr<IR> operator ()(IR &&r, UFN f) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      f(*it);
    return it;
  }

  template <class IR, class IR2, class UFN>
  constexpr pair<rng_itr<IR>, rng_itr<IR2>>
  operator ()(IR &&r, IR2 &&rr, UFN f) const
    requires (is_rng<IR> && is_rng<IR2>) {
    const auto ed = end(r);
    const auto ed2 = end(rr);
    auto it = begin(r);
    auto it2 = begin(rr);
    while (it != ed && it2 != ed2) {
      f(*it, *it2);
      ++it;
      ++it2;
    }
    return pair(it, it2);
  }
  template <class IR, class II, class UFN>
  constexpr II operator ()(IR &&r, const II &i, UFN f) const
    requires (is_rng<IR> && !is_rng<II>) {
    return for_each_plus(r, i, f).second;
  }
};
inline constexpr fo_for_each for_each{};

struct fo_for_each_excluding_first {
  template <class R, class F, class FF = empty_function>
  constexpr void operator ()(R &&r, F f, FF ff = {}) const
    requires is_rng<R> {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed) {
      ff(*it);
      ++it;
      for (; it != ed; ++it)
        f(*it);
    }
  }
};
inline constexpr fo_for_each_excluding_first for_each_excluding_first{};
struct fo_for_each_excluding_last {
  template <class FR, class F, class FF = empty_function>
  constexpr void operator ()(FR &&r, F f, FF ff = {}) const
    requires is_frng<FR> {
    if constexpr (is_brng<FR>) {
      const auto ed = end(r);
      auto it = begin(r);
      if (it != ed) {
        const auto prev_ed = prev(ed);
        for (; it != prev_ed; ++it)
          f(*it);
        ff(*it);
      }
    }
    else {
      const auto ed = end(r);
      auto it = begin(r);
      auto it2 = it;
      if (it != ed) {
        ++it2;
        for (;;) {
          if (it2 != ed)
            f(*it);
          else {
            ff(*it);
            break;
          }
          it = it2;
          ++it2;
        }
      }
    }
  }
};
inline constexpr fo_for_each_excluding_last for_each_excluding_last{};

}

// find_if
// find_if_not
// find
// find_not
namespace re {

struct fo_find_if {
  template <class IR, class UF>
  constexpr rng_itr<IR> operator ()(IR &&r, UF eq) const
    requires is_rng<IR> {
    auto it = begin(r);
    const auto ed = end(r);
    for (; it != ed; ++it)
      if (eq(*it))
        return it;
    return it;
  }
};
inline constexpr fo_find_if find_if{};
struct fo_find_if_not {
  template <class IR, class UF>
  constexpr rng_itr<IR> operator ()(IR &&r, UF eq) const
    requires is_rng<IR> {
    return find_if(r, not_fn(eq));
  }
};
inline constexpr fo_find_if_not find_if_not{};
struct fo_find {
  template <class IR, class T, class BF = equal_to<>>
  constexpr rng_itr<IR> operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return find_if(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_find find{};
struct fo_find_not {
  template <class IR, class T, class BF = equal_to<>>
  constexpr rng_itr<IR> operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return find_if(r, bind(not_fn(eq), _1, ref(x)));
  }
};
inline constexpr fo_find_not find_not{};

}

// find_last_if
// find_last_if_not
// find_last
// find_last_not
namespace re {

struct fo_find_last_if {
  template <class FR, class UF>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq) const
    requires is_frng<FR> {
    if constexpr (is_brng<FR>) {
      const auto op = begin(r);
      const auto ed = end(r);
      for (auto it = ed; it != op;) {
        --it;
        if (eq(*it))
          return it;
      }
      return ed;
    }
    else {
      auto it = begin(r);
      const auto ed = end(r);
      auto ret = ed;
      for (; it != ed; ++it)
        if (eq(*it))
          ret = it;
      return ret;
    }
  }
};
inline constexpr fo_find_last_if find_last_if{};
struct fo_find_last_if_not {
  template <class FR, class UF>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq) const
    requires is_frng<FR> {
    return find_last_if(r, not_fn(eq));
  }
};
inline constexpr fo_find_last_if_not find_last_if_not{};
struct fo_find_last {
  template <class FR, class T, class BF = equal_to<>>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, BF eq = {}) const
    requires is_frng<FR> {
    return find_last_if(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_find_last find_last{};
struct fo_find_last_not {
  template <class FR, class T, class BF = equal_to<>>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, BF eq = {}) const
    requires is_rng<FR> {
    return find_last_if(r, bind(not_fn(eq), _1, ref(x)));
  }
};
inline constexpr fo_find_last_not find_last_not{};

}

// find_first_of
// find_last_of
namespace re {

struct fo_find_first_of {
  template <class IR, class FR, class BF = equal_to<>>
  constexpr rng_itr<IR> operator ()(IR &&r, FR &&matched_elements,
                                    BF eq = {}) const
    requires is_rng<IR> {
    for (auto &it : iters(r))
      if (any_of_equal(matched_elements, *it, eq))
        return it;
    return end(r);
  }
};
inline constexpr fo_find_first_of find_first_of{};

struct fo_find_last_of {
  template <class FR, class FR2, class BF = equal_to<>>
  constexpr rng_itr<FR> operator ()(FR &&r, FR2 &&matched_elements,
                                    BF eq = {}) const
    requires (is_frng<FR> && is_frng<FR2>) {
    if constexpr (is_brng<FR>) {
      const auto ed = end(r);
      for (auto it = ed; it != begin(r);)
        if (any_of_equal(matched_elements, *--it, eq))
          return it;
      return ed;
    }
    else {
      auto ret = end(r);
      for (auto &it : iters(r))
        if (any_of_equal(matched_elements, *it, eq))
          ret = it;
      return ret;
    }
  }
};
inline constexpr fo_find_last_of find_last_of{};

}

// adjacent_find
namespace re {

struct fo_adjacent_find {
  template <class FR, class BF = equal_to<>>
  constexpr rng_itr<FR> operator ()(FR &&r, BF eq = {}) const
    requires is_frng<FR> {
    auto ret = end(r);
    if (!empty(r)) {
      auto lastp = begin(r);
      for (auto &p : iters(next(begin(r)), end(r))) {
        if (eq(*lastp, *p)) {
          ret = lastp;
          break;
        }
        lastp = p;
      }
    }
    return ret;
  }
};
inline constexpr fo_adjacent_find adjacent_find{};

}

// count
// count_if
namespace re {

struct fo_count_if {
  template <class IR, class UF>
  constexpr rng_dft<IR> operator ()(IR &&r, UF eq) const
    requires is_rng<IR> {
    rng_dft<IR> n = 0;
    for (auto &p : iters(r))
      if (eq(*p))
        ++n;
    return n;
  }
};
inline constexpr fo_count_if count_if{};
struct fo_count {
  template <class IR, class T, class BF = equal_to<>>
  constexpr rng_dft<IR> operator ()(IR &&r, const T &x, BF eq = {}) const
    requires is_rng<IR> {
    return count_if(r, bind(eq, _1, ref(x)));
  }
};
inline constexpr fo_count count{};

}

// mismatch
namespace re {

struct fo_mismatch {
  template <class IR, class II, class BF = equal_to<>>
  constexpr pair<rng_itr<IR>, II> operator ()(IR &&r, const II &it,
                                              BF eq = {}) const
    requires (is_rng<IR> && !is_rng<II>) {
    II ii = it;
    auto i = begin(r);
    const auto ed = end(r);
    while (i != ed) {
      if (!eq(*i, *ii))
        break;
      ++i;
      ++ii;
    }
    return {i, ii};
  }

  template <class IR1, class IR2, class BF = equal_to<>>
  constexpr pair<rng_itr<IR1>, rng_itr<IR2>>
  operator ()(IR1 &&r, IR2 &&r2, BF eq = {}) const
    requires (is_rng<IR1> && is_rng<IR2>) {
    auto it = begin(r);
    const auto ed = end(r);
    auto it2 = begin(r2);
    const auto ed2 = end(r2);
    while (it != ed && it2 != ed2) {
      if (!eq(*it, *it2))
        break;
      ++it;
      ++it2;
    }
    return {it, it2};
  }
};
inline constexpr fo_mismatch mismatch{};

}

// is_permutation
namespace re {

struct fo_is_permutation {
  template <class FR, class FR2, class BF = equal_to<>>
  constexpr bool operator ()(FR &&r, FR2 &&r2, BF eq = {}) const
    requires (is_frng<FR> && is_frng<FR2>) {
    if (size(r) != size(r2))
      return false;
    if (equal(r, r2, eq))
      return true;
    for (auto &it : iters(r)) {
      const auto eq2 = bind(eq, _1, bind(deref, it));
      if (none_of(rng(begin(r), it), eq2)
          && count_if(rng(next(it), end(r)), eq2) + 1 != count_if(r2, eq2))
        return false;
    }
    return true;
  }

  template <class FR, class FI, class BF = equal_to<>>
  constexpr bool operator ()(FR &&r, const FI &i, BF eq = {}) const
    requires (is_frng<FR> && !is_rng<FI>) {
    if (equal(r, i, eq))
      return true;
    auto r2 = rng(i, next(i, size(r)));
    for (auto &it : iters(r)) {
      const auto eq2 = bind(eq, _1, bind(deref, it));
      if (none_of(rng(begin(r), it), eq2)
          && count_if(rng(next(it), end(r)), eq2) + 1 != count_if(r2, eq2))
        return false;
    }
    return true;
  }
};
inline constexpr fo_is_permutation is_permutation{};

}

// find_subrange
// find_last_subrange
namespace re {

struct fo_find_subrange {
  template <class FR1, class FR2, class BF = equal_to<>>
  constexpr iter_pair<rng_itr<FR1>> operator ()(FR1 &&r, FR2 &&pattern,
                                                BF eq = {}) const
    requires (is_frng<FR1> && is_frng<FR2>) {
    if constexpr (!rng_is_n_value<FR2>
                  && is_brng<FR1>
                  && (rng_is_sized<FR1> && rng_is_sized<FR2>)) {
      const auto n = size(pattern);
      const auto op = begin(r);
      const auto ed = end(r);
      if (n == 0)
        return rng(op, op);
      const auto eq2 = bind(eq, _2, _1);
      if (size(r) >= n)
        for (auto &p : iters(op, prev(ed, n - 1)))
          if (auto [tmp, pp] = mismatch(pattern, p, eq2);
              tmp == end(pattern))
            return rng(p, pp);
      return rng(ed, ed);
    }
    else if constexpr (!rng_is_n_value<FR2>
                       && is_brng<FR1>
                       && !(rng_is_sized<FR1> && rng_is_sized<FR2>)) {
      const auto op = begin(r);
      const auto ed = end(r);
      if (empty(pattern))
        return rng(op, op);

      auto it = ed;
      for (auto &it2 : iters(next(begin(pattern)), end(pattern))) {
        if (it == op)
          return rng(ed, ed);
        --it;
      }

      const auto eq2 = bind(eq, _2, _1);
      for (auto &p : iters(op, it))
        if (auto [tmp, pp] = mismatch(pattern, p, eq2); tmp == end(pattern))
          return rng(p, pp);
      return rng(ed, ed);
    }
    else if constexpr (!rng_is_n_value<FR2>) {
      const auto op = begin(r);
      const auto ed = end(r);
      const auto sz1 = size(r);
      const auto sz2 = size(pattern);
      const auto eq2 = bind(eq, _2, _1);
      if (sz2 == 0)
        return rng(op, op);
      if (sz1 >= sz2) {
        auto p = op;
        for (auto c = sz1 - (sz2 - 1); c; --c) {
          if (auto [tmp, pp] = mismatch(pattern, p, eq2); tmp == end(pattern))
            return rng(p, pp);
          ++p;
        }
      }
      return rng(ed, ed);
    }
    else {
      const auto op = begin(r);
      const auto ed = end(r);
      const auto n = size(pattern);
      if (n == 0)
        return rng(op, op);
      const auto x = begin(pattern);
      for (auto p = op; p != ed; ++p)
        if (eq(*p, *x)) {
          const auto pp = p;
          auto c = n;
          for (;;) {
            if (++p == ed || --c == 0)
              return rng(pp, p);
            if (!eq(*p, *x))
              break;
          }
        }
      return rng(ed, ed);
    }
  }
};
inline constexpr fo_find_subrange find_subrange{};
struct fo_find_last_subrange {
  template <class FR1, class FR2, class BF = equal_to<>>
  constexpr iter_pair<rng_itr<FR1>>
  operator ()(FR1 &&r, FR2 &&pattern, BF eq = {}) const {
    if constexpr (is_brng<FR1> && rng_is_sized<FR1> && rng_is_sized<FR2>) {
      const auto n = size(pattern);
      const auto op = begin(r);
      const auto ed = end(r);
      const auto eq2 = bind(eq, _2, _1);
      if (n != 0 && size(r) >= n)
        for (auto p = prev(ed, n - 1); p != op;) {
          --p;
          if (auto [tmp, pp] = mismatch(pattern, p, eq2); tmp == end(pattern))
            return rng(p, pp);
        }
      return rng(ed, ed);
    }
    else if constexpr (is_brng<FR1>
                       && !(rng_is_sized<FR1> && rng_is_sized<FR2>)) {
      const auto ed = end(r);
      const auto op = begin(r);
      const auto ed2 = end(pattern);

      auto p = ed;
      auto it2 = begin(pattern);
      if (it2 == ed2)
        return rng(ed, ed);
      else
        while (++it2 != ed2) {
          if (p == op)
            return rng(ed, ed);
          --p;
        }

      const auto eq2 = bind(eq, _2, _1);
      while (p != op) {
        --p;
        if (auto [tmp, pp] = mismatch(pattern, p, eq2); tmp == end(pattern))
          return rng(p, pp);
      }
      return rng(ed, ed);
    }
    else {
      const auto sz1 = size(r);
      const auto sz2 = size(pattern);
      auto ret = rng(end(r), end(r));
      if (sz1 < sz2 || sz2 == 0)
        return ret;
      const auto eq2 = bind(eq, _2, _1);
      auto it = begin(r);
      for (auto c = sz1 - sz2 + 1u; c != 0; --c) {
        if (auto [tmp, it2] = mismatch(pattern, it, eq2); tmp == end(pattern))
          ret = rng(it, it2);
        ++it;
      }
      return ret;
    }
  }
};
inline constexpr fo_find_last_subrange find_last_subrange{};

}

// search
namespace re {

struct fo_search {
  template <class FR1, class FR2, class BF = equal_to<>>
  constexpr rng_itr<FR1> operator ()(FR1 &&r, FR2 &&pattern,
                                     BF eq = {}) const
    requires (is_frng<FR1> && is_frng<FR2>) {
    return find_subrange(r, pattern, eq).first;
  }
};
inline constexpr fo_search search{};

}

// find_end
namespace re {

struct fo_find_end {
  template <class FR1, class FR2, class BF = equal_to<>>
  constexpr rng_itr<FR1> operator ()(FR1 &&r, FR2 &&pattern, BF eq = {}) const
    requires (is_frng<FR1> && is_frng<FR2>) {
    return find_last_subrange(r, pattern, eq).first;
  }
};
inline constexpr fo_find_end find_end{};

}

// contains
// contains_subrange
namespace re {

struct fo_contains {
  template <class IR, class T, class EQ = equal_to<>>
  constexpr bool operator ()(IR &&r, const T &x, EQ eq = {}) const
    requires is_rng<IR> {
    const auto ed = end(r);
    return find(rng(begin(r), ed), x, eq) != ed;
  }
};
inline constexpr fo_contains contains{};
struct fo_contains_subrange {
  template <class FR, class FR2, class EQ = equal_to<>>
  constexpr bool operator ()(FR &&r, FR2 &&r2, EQ eq = {}) const
    requires (is_frng<FR> && is_frng<FR2>) {
    if (!empty(r2)) {
      const auto ed = end(r);
      return !find_subrange(rng(begin(r), ed), r2, eq).empty();
    }
    return true;
  }
};
inline constexpr fo_contains_subrange contains_subrange{};

}

// starts_with
// ends_with
namespace re {

struct fo_starts_with {
  template <class IR, class IR2, class BF = equal_to<>>
  constexpr bool operator ()(IR &&r, IR2 &&r2, BF eq = {}) const
    requires (is_rng<IR> && is_rng<IR2>) {
    const auto ed = end(r2);
    return mismatch(r, rng(begin(r2), ed), eq).second == ed;
  }
};
inline constexpr fo_starts_with starts_with{};

struct fo_ends_with {
  template <class IR, class IR2, class BF = equal_to<>>
  constexpr bool operator ()(IR &&r, IR2 &&r2, BF eq = {}) const
    requires ((is_frng<IR> || rng_is_sized<IR>)
              && (is_frng<IR2> || rng_is_sized<IR2>)) {
    if constexpr (is_brng<IR> && is_brng<IR2>) {
      const auto op = begin(r);
      const auto op2 = begin(r2);
      auto it = end(r);
      auto it2 = end(r2);
      for (; op != it && op2 != it2;)
        if (!eq(*--it, *--it2))
          return false;
      return op2 == it2;
    }
    else {
      const auto n1 = size(r);
      const auto n2 = size(r2);
      if (n1 < n2)
        return false;
      return equal(rng(nth(r, to_signed(n1 - n2)), end(r)), r2, eq);
    }
  }
};
inline constexpr fo_ends_with ends_with{};

}

// fold_left(_plus)
// fold_left_first(_plus)
// fold_right
namespace re {

struct fo_fold_left_plus {
  template <class IR, class T, class F>
  constexpr pair<rng_itr<IR>, decay_t<invoke_result_t<F &, T, rng_ref<IR>>>>
  operator ()(IR &&r, T init, F f) const requires is_rng<IR> {
    using t = decay_t<invoke_result_t<F &, T, rng_ref<IR>>>;
    const auto ed = end(r);
    auto it = begin(r);
    if (it == ed)
      return {it, t(move(init))};
    t acc = invoke(f, move(init), *it);
    for (++it; it != ed; ++it)
      acc = invoke(f, move(acc), *it);
    return {it, move(acc)};
  }
};
inline constexpr fo_fold_left_plus fold_left_plus{};
struct fo_fold_left {
  template <class IR, class T, class F>
  constexpr auto operator ()(IR &&r, T init, F f) const
    requires is_rng<IR> {
    return fold_left_plus(r, move(init), f).second;
  }
};
inline constexpr fo_fold_left fold_left{};
struct fo_fold_left_first_plus {
  template <class IR, class F>
  constexpr auto operator ()(IR &&r, F f) const requires is_rng<IR> {
    using t = decltype(fold_left(r, rng_vt<IR>(front(r)), f));
    using ret_t = pair<rng_itr<IR>, optional<t>>;
    const auto ed = end(r);
    auto it = begin(r);
    if (it == ed)
      return ret_t(it, optional<t>{});
    optional<t> init(in_place, *it);
    for (++it; it != ed; ++it)
      *init = invoke(f, move(*init), *it);
    return ret_t(it, move(init));
  }
};
inline constexpr fo_fold_left_first_plus fold_left_first_plus{};
struct fo_fold_left_first {
  template <class IR, class F>
  constexpr auto operator ()(IR &&r, F f) const requires is_rng<IR> {
    return fold_left_first_plus(r, f).second;
  }
};
inline constexpr fo_fold_left_first fold_left_first{};
struct fo_fold_right {
  template <class BR, class T, class F>
  constexpr decay_t<invoke_result_t<F &, rng_ref<BR>, T>>
  operator ()(BR &&r, T init, F f) const requires is_brng<BR> {
    using t = decay_t<invoke_result_t<F &, rng_ref<BR>, T>>;
    const auto op = begin(r);
    auto it = end(r);
    if (op == it)
      return t(move(init));
    t acc = invoke(f, *--it, move(init));
    while (op != it)
      acc = invoke(f, *--it, move(acc));
    return acc;
  }
};
inline constexpr fo_fold_right fold_right{};
struct fo_fold_right_last {
  template <class BR, class F>
  constexpr auto operator ()(BR &&r, F f) const requires is_brng<BR> {
    using t = decltype(fold_right(r, rng_vt<BR>(back(r)), f));
    const auto ed = end(r);
    const auto op = begin(r);
    if (op == ed)
      return optional<t>();
    const auto tail = prev(ed);
    return optional<t>(in_place,
                       fold_right(rng(op, tail), rng_vt<BR>(*tail), f));
  }
};
inline constexpr fo_fold_right_last fold_right_last{};

}

// copy(_plus)
// copy_if(_plus)
// copy_backward
// move(_plus)
// move_backward
// copy_from(_plus)
// move_from(_plus)
namespace re {

namespace inner::fns {

template <class IR, class OI>
constexpr pair<rng_itr<IR>, OI> copy_plus_impl(IR &&r, OI o) {
  if constexpr (is_citr<rng_itr<IR>> && is_citr<OI>
                && is_trivially_copyable_v<itr_vt<OI>>
                && is_same_v<rng_vt<IR>, itr_vt<OI>>) {
    const auto n = size(r);
    const auto i = begin(r);
    memmove(static_cast<void *>(to_address(o)),
            static_cast<const void *>(to_address(i)),
            n * sizeof(rng_vt<IR>));
    return {end(r), o + n};
  }
  else {
    const auto ed = end(r);
    auto p = begin(r);
    for (; p != ed; ++p) {
      *o = *p;
      ++o;
    }
    return {p, o};
  }
}

}
struct fo_copy_plus {
  template <class IR, class OI>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o) const
    requires is_rng<IR> {
    return inner::fns::copy_plus_impl(r, o);
  }
};
inline constexpr fo_copy_plus copy_plus{};
struct fo_move_plus {
  template <class IR, class OI>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o) const
    requires is_rng<IR> {
    const auto tmp = inner::fns::copy_plus_impl(move_rng(r), o);
    return {tmp.first.base(), tmp.second};
  }
};
inline constexpr fo_move_plus move_plus{};
template <class A, class B>
auto fo_copy::operator ()(A &&a, B b) const {
  return inner::fns::copy_plus_impl(a, b).second;
}
template <class A, class B>
auto fo_move::operator ()(A &&a, B b) const {
  return inner::fns::copy_plus_impl(move_rng(a), b).second;
}

struct fo_copy_if_plus {
  template <class IR, class OI, class BF>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, BF eq) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      if (eq(*it)) {
        *o = *it;
        ++o;
      }
    return {it, o};
  }
};
inline constexpr fo_copy_if_plus copy_if_plus{};
struct fo_copy_if {
  template <class IR, class OI, class BF>
  constexpr OI operator ()(IR &&r, OI o, BF eq) const requires is_rng<IR> {
    return copy_if_plus(r, o, eq).second;
  }
};
inline constexpr fo_copy_if copy_if{};

struct fo_copy_backward {
  template <class BR, class BI>
  constexpr BI operator ()(BR &&r, BI o) const
    requires (is_brng<BR> && is_bitr<BI>) {
    if constexpr (is_citr<rng_itr<BR>> && is_trivially_copyable_v<rng_vt<BR>>
                  && is_citr<BI> && is_trivially_copyable_v<itr_vt<BI>>
                  && is_same_v<itr_vt<BI>, rng_vt<BR>>) {
      const auto n = size(r);
      memmove(static_cast<void *>
              (reinterpret_cast<char *>(to_address(o))
               - n * sizeof(itr_vt<BI>)),
              static_cast<const void *>(to_address(begin(r))),
              n * sizeof(itr_vt<BI>));
      return o - n;
    }
    else {
      const auto ed = end(r);
      for (auto p = ed, op = begin(r); p != op;)
        *--o = *--p;
      return o;
    }
  }
};
inline constexpr fo_copy_backward copy_backward{};
struct fo_move_backward {
  template <class BR, class BI>
  constexpr BI operator ()(BR &&r, BI o) const
    requires (is_brng<BR> && is_bitr<BI>) {
    return copy_backward(move_rng(r), o);
  }
};
inline constexpr fo_move_backward move_backward{};

struct fo_copy_from_plus {
  template <class FR, class II>
  constexpr pair<rng_itr<FR>, II> operator ()(FR &&r, II i) const
    requires (is_frng<FR> && is_itr<II>) {
    if constexpr (is_citr<rng_itr<FR>> && is_trivially_copyable_v<rng_vt<FR>>
                  && is_citr<II> && is_trivially_copyable_v<itr_vt<II>>
                  && is_same_v<rng_vt<FR>, itr_vt<II>>) {
      auto n = size(r);
      memmove(static_cast<void *>(to_address(begin(r))),
              static_cast<const void *>(to_address(i)),
              n * sizeof(rng_vt<FR>));
      return {end(r), i + n};
    }
    else {
      const auto ed = end(r);
      auto p = begin(r);
      for (; p != ed; ++p) {
        *p = *i;
        ++i;
      }
      return {p, i};
    }
  }
};
inline constexpr fo_copy_from_plus copy_from_plus{};
struct fo_copy_from {
  template <class FR, class II>
  constexpr II operator ()(FR &&r, II i) const
    requires (is_frng<FR> && is_itr<II>) {
    return copy_from_plus(r, i).second;
  }
};
inline constexpr fo_copy_from copy_from{};
struct fo_move_from_plus {
  template <class FR, class II>
  constexpr pair<rng_itr<FR>, II> operator ()(FR &&r, II i) const
    requires (is_frng<FR> && is_itr<II>) {
    const auto tmp = copy_from_plus(r, move_itr(i));
    return {tmp.first, tmp.second.base()};
  }
};
inline constexpr fo_move_from_plus move_from_plus{};
struct fo_move_from {
  template <class FR, class II>
  constexpr II operator ()(FR &&r, II i) const
    requires (is_frng<FR> && is_itr<II>) {
    return copy_from_plus(r, move_itr(i)).second.base();
  }
};
inline constexpr fo_move_from move_from{};

}

// swap_ranges(_plus)
namespace re {

struct fo_swap_ranges {
  template <class FR, class FI>
  constexpr FI operator ()(FR &&r, const FI &it) const
    requires (is_frng<FR> && !is_rng<FI> && is_fitr<FI>) {
    return for_each(r, it, adl_swap);
  }
  template <class FR, class FR2>
  constexpr pair<rng_itr<FR>, rng_itr<FR2>> operator ()(FR &&r, FR2 &&r2) const
    requires (is_frng<FR> && is_frng<FR2>) {
    return for_each(r, r2, adl_swap);
  }
};
inline constexpr fo_swap_ranges swap_ranges{};
struct fo_swap_ranges_plus {
  template <class FR, class FI>
  constexpr pair<rng_itr<FR>, FI> operator ()(FR &&r, FI it) const
    requires (is_frng<FR> && is_fitr<FI>) {
    return for_each_plus(r, it, adl_swap);
  }
};
inline constexpr fo_swap_ranges_plus swap_ranges_plus{};

}

// transform(_plus)
namespace re {

struct fo_transform {
  template <class IR, class OI, class UF>
  constexpr OI operator ()(IR &&r, OI o, UF f) const
    requires (is_rng<IR> && is_itr<OI>) {
    for (auto &it : iters(r)) {
      *o = f(*it);
      ++o;
    }
    return o;
  }
  template <class IR, class IR_OR_II, class OI, class BF>
  constexpr OI operator ()(IR &&r, IR_OR_II &&rr, OI o, BF f) const
    requires (is_rng<IR> && (is_rng<IR_OR_II> || is_itr<IR_OR_II>)) {
    for_each(r, rr, [&o, f]<class T, class U>(T &&x, U &&y) {
        *o = f(forward<T>(x), forward<U>(y));
        ++o;
      });
    return o;
  }
};
inline constexpr fo_transform transform{};
struct fo_transform_plus {
  template <class IR, class OI, class UF>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, UF f) const
    requires (is_rng<IR> && is_itr<OI>) {
    return for_each_plus(r, o, [f]<class T, class U>(T &&x, U &&y) {
        forward<U>(y) = f(forward<T>(x));
      });
  }
  template <class IR, class II, class OI, class BF>
  constexpr auto operator ()(IR &&r, const II &&i, OI o, BF f)
    const requires (is_rng<IR> && !is_rng<II> && is_itr<II> && is_itr<OI>) {
    const auto tmp = for_each_plus(r, i,
                                   [f, &o]<class T, class U>(T &&x, U &&y) {
                                     *o = f(forward<T>(x), forward<U>(y));
                                     ++o;
                                   });
    return tuple(tmp.first, tmp.second, o);
  }
  template <class IR, class IR2, class OI, class BF>
  constexpr auto operator ()(IR &&r, IR2 &&r2, OI o, BF f)
    const requires (is_rng<IR> && is_rng<IR2> && is_itr<OI>) {
    const auto tmp = for_each(r, r2,
                              [f, &o]<class T, class U>(T &&x, U &&y) {
                                *o = f(forward<T>(x), forward<U>(y));
                                ++o;
                              });
    return tuple(tmp.first, tmp.second, o);
  }
};
inline constexpr fo_transform_plus transform_plus{};

}

// replace(_plus)
// replace_if(_plus)
// replace_copy(_plus)
// replace_copy_if(_plus)
namespace re {

struct fo_replace_if_plus {
  template <class FR, class UF, class S>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq, const S &x) const
    requires is_frng<FR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      if (eq(*it))
        *it = x;
    return it;
  }
};
inline constexpr fo_replace_if_plus replace_if_plus{};
struct fo_replace_if {
  template <class FR, class UF, class S>
  constexpr void operator ()(FR &&r, UF eq, const S &x) const
    requires is_frng<FR> {
    replace_if_plus(r, eq, x);
  }
};
inline constexpr fo_replace_if replace_if{};

struct fo_replace_plus {
  template <class FR, class T, class S>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, const S &y) const
    requires is_frng<FR> {
    return replace_if_plus(r, bind(equal_to{}, _1, ref(x)), y);
  }
};
inline constexpr fo_replace_plus replace_plus{};
struct fo_replace {
  template <class FR, class T, class S>
  constexpr void operator ()(FR &&r, const T &x, const S &y) const
    requires is_frng<FR> {
    replace_if_plus(r, bind(equal_to{}, _1, ref(x)), y);
  }
};
inline constexpr fo_replace replace{};

struct fo_replace_copy_if_plus {
  template <class IR, class OI, class UF, class S>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, UF eq, const S &y) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it) {
      *o = eq(*it) ? y : *it;
      ++o;
    }
    return {it, o};
  }
};
inline constexpr fo_replace_copy_if_plus replace_copy_if_plus{};
struct fo_replace_copy_if {
  template <class IR, class OI, class UF, class S>
  constexpr OI operator ()(IR &&r, OI o, UF eq, const S &y) const
    requires (is_rng<IR> && is_itr<OI>) {
    return replace_copy_if_plus(r, o, eq, y).second;
  }
};
inline constexpr fo_replace_copy_if replace_copy_if{};

struct fo_replace_copy_plus {
  template <class IR, class OI, class T, class S>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, const T &x, const S &y) const
    requires (is_rng<IR> && is_itr<OI>) {
    return replace_copy_if_plus(r, o, bind(equal_to{}, _1, ref(x)), y);
  }
};
inline constexpr fo_replace_copy_plus replace_copy_plus{};
struct fo_replace_copy {
  template <class IR, class OI, class T, class S>
  constexpr OI operator ()(IR &&r, OI o, const T &x, const S &y) const
    requires (is_rng<IR> && is_itr<OI>) {
    return replace_copy_if_plus(r, o, bind(equal_to{}, _1, ref(x)), y).second;
  }
};
inline constexpr fo_replace_copy replace_copy{};

}

// fill(_plus)
// fill_zero(_plus)
// bytewise_fill
namespace re {

struct fo_fill_plus {
  template <class IR, class T>
  constexpr rng_itr<IR> operator ()(IR &&r, const T &x) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      *it = x;
    return it;
  }
};
inline constexpr fo_fill_plus fill_plus{};
struct fo_fill {
  template <class IR, class T>
  constexpr void operator ()(IR &&r, const T &x) const requires is_rng<IR> {
    for (auto &p : iters(r))
      *p = x;
  }
};
inline constexpr fo_fill fill{};

namespace inner::fns {

template <class IR>
constexpr rng_itr<IR> fill_zero_plus_impl(IR &&r) {
  return fill_plus(r, 0);
}
template <class IR>
constexpr rng_itr<IR> fill_zero_plus_impl(IR &&r)
  requires is_crng<IR> && is_integral_v<rng_vt<IR>> {
  memset(static_cast<void *>(to_address(begin(r))), 0,
         sizeof(rng_vt<IR>) * size(r));
  return end(r);
}

}
struct fo_fill_zero_plus {
  template <class IR>
  constexpr rng_itr<IR> operator ()(IR &&r) const requires is_rng<IR> {
    if (is_constant_evaluated()) {
      return fill_plus(r, 0);
    }
    else {
      return inner::fns::fill_zero_plus_impl(r);
    }
  }
};
inline constexpr fo_fill_zero_plus fill_zero_plus{};
struct fo_fill_zero {
  template <class IR>
  constexpr void operator ()(IR &&r) const requires is_rng<IR> {
    fill_zero_plus(r);
  }
};
inline constexpr fo_fill_zero fill_zero{};

struct fo_bytewise_fill {
  template <class CR, class T>
  constexpr void operator ()(CR &&r, const T &x) const
    requires is_crng<CR> {
    memset(static_cast<void *>(to_address(begin(r))),
           static_cast<unsigned char>(x),
           sizeof(rng_vt<CR>) * size(r));
  }
};
inline constexpr fo_bytewise_fill bytewise_fill{};

}

// generate(_plus)
namespace re {

struct fo_generate {
  template <class IR, class UFN>
  constexpr void operator ()(IR &&r, UFN f) const requires is_rng<IR> {
    for (auto &p : iters(r))
      *p = f();
  }
};
inline constexpr fo_generate generate{};
struct fo_generate_plus {
  template <class IR, class UFN>
  constexpr rng_itr<IR> operator ()(IR &&r, UFN f) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      *it = f();
    return it;
  }
};
inline constexpr fo_generate_plus generate_plus{};

}

// remove
// remove_if
// remove_copy(_plus)
// remove_copy_if(_plus)
namespace re {

struct fo_remove_if {
  template <class FR, class UF>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq) const
    requires is_frng<FR> {
    auto it = find_if(r, eq);
    if (it != end(r))
      for (auto &it2 : iters(next(it), end(r)))
        if (!eq(*it2)) {
          *it = move(*it2);
          ++it;
        }
    return it;
  }
};
inline constexpr fo_remove_if remove_if{};
struct fo_remove {
  int operator ()(const char *fname) const {
    return std::remove(fname);
  }

  template <class FR, class T>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x) const
    requires is_frng<FR> {
    return remove_if(r, bind(equal_to{}, _1, ref(x)));
  }
};
inline constexpr fo_remove remove{};

struct fo_remove_copy_if_plus {
  template <class IR, class OI, class UF>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, UF eq) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      if (!eq(*it)) {
        *o = *it;
        ++o;
      }
    return {it, o};
  }
};
inline constexpr fo_remove_copy_if_plus remove_copy_if_plus{};
struct fo_remove_copy_plus {
  template <class IR, class OI, class T>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, const T &x) const
    requires (is_rng<IR> && is_itr<OI>) {
    return remove_copy_if_plus(r, o, bind(equal_to{}, _1, ref(x)));
  }
};
inline constexpr fo_remove_copy_plus remove_copy_plus{};
struct fo_remove_copy_if {
  template <class IR, class OI, class UF>
  constexpr OI operator ()(IR &&r, OI o, UF eq) const
    requires (is_rng<IR> && is_itr<OI>) {
    return remove_copy_if_plus(r, o, eq).second;
  }
};
inline constexpr fo_remove_copy_if remove_copy_if{};
struct fo_remove_copy {
  template <class IR, class OI, class T>
  constexpr OI operator ()(IR &&r, OI o, const T &x) const
    requires (is_rng<IR> && is_itr<OI>) {
    return remove_copy_if_plus(r, o, bind(equal_to{}, _1, ref(x))).second;
  }
};
inline constexpr fo_remove_copy remove_copy{};

}

// unique
// unique_copy
namespace re {

struct fo_unique {
  template <class FR, class BF = equal_to<>>
  constexpr rng_itr<FR> operator ()(FR &&r, BF eq = {}) const
    requires is_frng<FR> {
    auto it = adjacent_find(r, eq);
    if (it == end(r))
      return it;
    for (auto &p : iters(next(it, 2), end(r)))
      if (!eq(*it, *p))
        *++it = move(*p);
    return ++it;
  }
};
inline constexpr fo_unique unique{};
struct fo_unique_copy_plus {
  template <class IR, class OI, class BF = equal_to<>>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, BF eq = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    if (empty(r))
      return {ed, o};
    auto it = begin(r);
    *o = *it;
    auto p = next(it);
    for (; p != ed; ++p)
      if (!eq(*p, *it)) {
        *++o = *p;
        it = p;
      }
    return {p, ++o};
  }
};
inline constexpr fo_unique_copy_plus unique_copy_plus{};
struct fo_unique_copy {
  template <class IR, class OI, class BF = equal_to<>>
  constexpr OI operator ()(IR &&r, OI o, BF eq = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return unique_copy_plus(r, o, eq).second;
  }
};
inline constexpr fo_unique_copy unique_copy{};

}

// reverse, and reverse_copy
namespace re {

struct fo_reverse {
  template <class BR, class DO_SWAP = decay_t<decltype(adl_swap)>>
  constexpr void operator ()(BR &&r, DO_SWAP do_swap = adl_swap) const
    requires is_brng<BR> {
    if constexpr (is_rrng<BR>) {
      for (auto it = begin(r), it2 = end(r); it < it2;) {
        do_swap(*it, *--it2);
        ++it;
      }
    }
    else {
      if (!empty(r)) {
        auto it = begin(r), it2 = end(r);
        for (;;) {
          --it2;
          if (it == it2)
            return;
          do_swap(*it, *it2);
          ++it;
          if (it == it2)
            return;
        }
      }
    }
  }
};
inline constexpr fo_reverse reverse{};
struct fo_reverse_copy {
  template <class BR, class OI>
  constexpr OI operator ()(BR &&r, OI o) const
    requires (is_brng<BR> && is_itr<OI>) {
    for (auto it = end(r), ed = begin(r); it != ed;) {
      *o = *--it;
      ++o;
    }
    return o;
  }
};
inline constexpr fo_reverse_copy reverse_copy{};

}

// rotate
// rotate_copy
namespace re {

struct fo_rotate {
  template <class FR, class SWAP_FN = decay_t<decltype(adl_swap)>>
  constexpr rng_itr<FR> operator ()(FR &&r, rng_itr<FR> mid,
                                    SWAP_FN do_swap = adl_swap) const
    requires is_frng<FR> {
    const auto ed = end(r);
    rng_itr<FR> it = begin(r), it2 = mid;
    if (it == mid)
      return ed;
    if (it2 == ed)
      return it;
    for (;;) {
      do_swap(*it, *it2);
      ++it;
      ++it2;
      if (it == mid) {
        if (it2 == ed)
          return mid;
        else
          mid = it2;
      }
      else {
        if (it2 == ed) {
          it2 = mid;
          {
            const auto ret = it;
            for (;;) {
              do_swap(*it, *it2);
              ++it;
              ++it2;
              if (it == mid) {
                if (it2 == ed)
                  break;
                else
                  mid = it2;
              }
              else {
                if (it2 == ed)
                  it2 = mid;
              }
            }
            return ret;
          }
        }
      }
    }
  }
};
inline constexpr fo_rotate rotate{};
struct fo_rotate_copy {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_itr<FR> mid, OI o) const
    requires (is_frng<FR> && is_itr<OI>) {
    return copy(rng(begin(r), mid), copy(rng(mid, end(r)), o));
  }
};
inline constexpr fo_rotate_copy rotate_copy{};

}

// shift_left
// shift_right
namespace re {

struct fo_shift_left {
  template <class FR>
  constexpr rng_itr<FR> operator ()(FR &&r, rng_itr<FR> i) const
    requires is_frng<FR> {
    return move(rng(i, end(r)), begin(r));
  }
  template <class FR>
  constexpr rng_itr<FR> operator ()(FR &&r, rng_dft<FR> n) const
    requires is_frng<FR> {
    return operator ()(r, next(begin(r), n, end(r)));
  }
};
inline constexpr fo_shift_left shift_left{};
struct fo_shift_right {
  template <class FR>
  constexpr rng_itr<FR> operator ()(FR &&r, rng_itr<FR> mid) const
    requires is_frng<FR> {
    if constexpr (is_brng<FR>) {
      return move_backward(rng(begin(r), mid), end(r));
    }
    else {
      const auto ret = nth(r, distance(mid, end(r)));
      const auto op = begin(r);
      if (ret == op)
        return ret;
      const auto ed = end(r);
      auto it = op;
      auto it2 = ret;
      for (;;) {
        if (it2 == ed)
          break;
        else {
          if (it == ret)
            it = op;
        }
        adl_swap(*it, *it2);
        ++it;
        ++it2;
      }
      return ret;
    }
  }
  template <class FR>
  constexpr rng_itr<FR> operator ()(FR &&r, rng_dft<FR> n) const
    requires is_frng<FR> {
    if constexpr (is_brng<FR>) {
      const auto op = begin(r);
      const auto ed = end(r);
      return move_backward(rng(op, prev(ed, n, begin(r))), end(r));
    }
    else {
      const auto sz = ssize(r);
      const auto ed = end(r);
      if (n >= sz)
        return ed;
      return operator ()(r, nth(r, sz - n));
    }
  }
};
inline constexpr fo_shift_right shift_right{};

}

// sample
namespace re {

struct fo_sample {
  template <class IR, class OR, class URBG>
  constexpr rng_itr<OR> operator ()(IR &&r, OR &&r2, URBG &&rand) const
    requires (is_frng<IR> && is_rng<OR>) {
    const rng_szt<IR> n = size(r);
    rng_szt<IR> n_used = 0;
    const rng_szt<OR> k = size(r2);
    rng_szt<OR> k_used = 0;
    auto it = begin(r2);
    const auto ed = end(r2);
    auto dis = rational_bernoulli_distribution();
    using dis_param_t = decltype(dis)::param_type;
    for (auto &p : iters(r)) {
      if (it == ed)
        break;
      if (dis(rand, dis_param_t(k - k_used, n - n_used))) {
        *it = *p;
        ++it;
        ++k_used;
      }
      ++n_used;
    }
    return it;
  }
  template <class IR, class OR, class URBG>
  constexpr rng_itr<OR> operator ()(IR &&r, OR &&r2, URBG &&rand) const
    requires (!is_frng<IR> && is_rrng<OR>) {
    const auto n = size(r2);
    auto it = begin(r);
    const auto ed = end(r);
    auto it2 = begin(r2);
    const auto ed2 = end(r2);
    for (; it != ed && it2 != ed2; ++it, (void)++it2)
      *it2 = *it;
    if (it != ed) {
      auto i = n;
      uniform_int_distribution<rng_szt<OR>> dis;
      using dis_param_t = typename decltype(dis)::param_type;
      const auto op = begin(r2);
      for (auto &p : iters(it, ed)) {
        const auto j = dis(rand, dis_param_t(0, i));
        ++i;
        if (j < n)
          op[j] = *p;
      }
    }
    return it2;
  }
};
inline constexpr fo_sample sample{};

}

// shuffle
namespace re {

struct fo_shuffle {
  template <class RR, class URBG>
  constexpr void operator ()(RR &&r, URBG &&rand) const
    requires is_rrng<RR> {
    using szt = rng_szt<RR>;
    const szt n = size(r);
    if (n == 0)
      return;
    uniform_int_distribution<szt> dis;
    using dis_param_t = typename decltype(dis)::param_type;
    const auto op = begin(r);
    for (auto &i : iters(0u, n - 1u)) {
      const auto j = dis(rand, dis_param_t(i, n - 1u));
      if (j != i)
        adl_swap(op[i], op[j]);
    }
  }
};
inline constexpr fo_shuffle shuffle{};

}

// partition_point
// lower_bound
// upper_bound
// equal_range
// binary_search
// bs_find_range
// bs_find
namespace re {

struct fo_partition_point {
  template <class FR, class UF>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq) const
    requires is_frng<FR> {
    auto p = begin(r);
    auto sz = size(r);
    decltype(sz) i;
    while (sz != 0) {
      auto it = next(p, (i = sz / 2));
      if (eq(*it)) {
        p = ++it;
        sz -= i + 1;
      }
      else
        sz = i;
    }
    return p;
  }
};
inline constexpr fo_partition_point partition_point{};
struct fo_lower_bound {
  template <class FR, class T, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, BF less = {}) const
    requires is_frng<FR> {
    return partition_point(r, bind(less, _1, ref(x)));
  }
};
inline constexpr fo_lower_bound lower_bound{};
struct fo_upper_bound {
  template <class FR, class T, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, BF less = {}) const
    requires is_frng<FR> {
    return partition_point(r, not_fn(bind(less, ref(x), _1)));
  }
};
inline constexpr fo_upper_bound upper_bound{};
struct fo_equal_range {
  template <class FR, class T, class BF = less<>>
  constexpr iter_pair<rng_itr<FR>>
  operator ()(FR &&r, const T &x, BF less = {}) const requires is_frng<FR> {
    const auto ed = end(r);
    const auto it = lower_bound(r, x, less);
    return {it, upper_bound(rng(it, ed), x, less)};
  }
};
inline constexpr fo_equal_range equal_range{};

struct fo_binary_search {
  template <class FR, class T, class BF = less<>>
  constexpr bool operator ()(FR &&r, const T &x, BF less = {}) const
    requires is_frng<FR> {
    const auto p = lower_bound(r, x, less);
    return p != end(r) && !less(x, *p);
  }
};
inline constexpr fo_binary_search binary_search{};
struct fo_bs_find {
  template <class FR, class T, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, const T &x, BF less = {}) const
    requires is_frng<FR> {
    auto p = lower_bound(r, x, less);
    if (const auto ed = end(r); p != ed && less(x, *p))
      p = ed;
    return p;
  }
};
inline constexpr fo_bs_find bs_find{};
struct fo_bs_find_range {
  template <class FR, class T, class BF = less<>>
  constexpr iter_pair<rng_itr<FR>>
  operator ()(FR &&r, const T &lower, const T &upper, BF less = {}) const
    requires is_frng<FR> {
    const auto it1 = lower_bound(r, lower, less);
    const auto it2 = upper_bound(rng(it1, end(r)), upper, less);
    return {it1, it2};
  }
};
inline constexpr fo_bs_find_range bs_find_range{};

}

// is_partitioned
// partition
// partition_copy
// stable_partition_with_no_buffer
// stable_partition_with_buffer
// stable_partition
namespace re {

struct fo_is_partitioned {
  template <class IR, class UF>
  constexpr bool operator ()(IR &&r, UF eq) const requires is_rng<IR> {
    auto it = find_if_not(r, eq);
    if (it == end(r))
      return true;
    return none_of(rng(++it, end(r)), eq);
  }
};
inline constexpr fo_is_partitioned is_partitioned{};

struct fo_partition {
  template <class FR, class UF>
  constexpr rng_itr<FR> operator ()(FR &&r, UF eq) const
    requires is_frng<FR> {
    if constexpr (is_brng<FR>) {
      const auto ed = end(r);
      auto p = begin(r);
      if (p == ed)
        return ed;
      auto pp = ed;
      for (;;) {
        for (;; ++p) {
          if (p == pp)
            return p;
          if (!eq(*p))
            break;
        }
        for (--pp;; --pp) {
          if (p == pp)
            return p;
          if (eq(*pp))
            break;
        }
        adl_swap(*p, *pp);
        ++p;
      }
    }
    else {
      const auto ed = end(r);
      auto it = find_if_not(r, eq);
      if (it != ed)
        for (auto &p : iters(next(it), ed))
          if (eq(*p)) {
            adl_swap(*it, *p);
            ++it;
          }
      return it;
    }
  }
};
inline constexpr fo_partition partition{};

struct fo_partition_copy {
  template <class IR, class OI, class OI2, class UF>
  constexpr pair<OI, OI2>
  operator ()(IR &&r, OI o_true, OI2 o_false, UF eq) const
    requires (is_rng<IR> && is_itr<OI> && is_itr<OI2>) {
    for (auto &p : iters(r)) {
      if (eq(*p)) {
        *o_true = *p;
        ++o_true;
      }
      else {
        *o_false = *p;
        ++o_false;
      }
    }
    return {o_true, o_false};
  }
};
inline constexpr fo_partition_copy partition_copy{};

namespace inner::fns {

template <class BR, class UF>
constexpr rng_itr<BR> stable_partition_nobuf_ritr_impl(BR &&r, UF eq) {
  const auto sz = size(r);
  if (sz <= 1) {
    auto ret = begin(r);
    if (sz == 0)
      return ret;
    else {
      if (eq(*ret))
        ++ret;
    }
    return ret;
  }
  const auto it = begin(r) + sz / 2;
  const auto p1 = stable_partition_nobuf_ritr_impl(rng(begin(r), it), eq);
  const auto p2 = stable_partition_nobuf_ritr_impl(rng(it, end(r)), eq);
  return rotate(rng(p1, p2), it);
}
template <class BR, class UF>
constexpr rng_itr<BR> stable_partition_nobuf_bitr_impl(BR &&r, rng_szt<BR> sz,
                                                       UF eq) {
  if (sz <= 1) {
    auto ret = begin(r);
    if (sz == 0)
      return ret;
    else if (eq(*ret))
      ++ret;
    return ret;
  }
  const auto n = sz / 2;
  const auto it = next(begin(r), n);
  const auto p1 = stable_partition_nobuf_bitr_impl(rng(begin(r), it), n, eq);
  const auto p2 = stable_partition_nobuf_bitr_impl(rng(it, end(r)), sz - n, eq);
  return rotate(rng(p1, p2), it);
}
template <class BR, class UF>
constexpr rng_itr<BR> stable_partition_nobuf_impl(BR &&r, UF eq) {
  if constexpr (is_rrng<BR>) {
    return inner::fns::stable_partition_nobuf_ritr_impl(r, eq);
  }
  else {
    return inner::fns::stable_partition_nobuf_bitr_impl(r, size(r), eq);
  }
}

}
struct fo_stable_partition_with_buffer {
  template <class FR, class BUF, class UF>
  constexpr rng_itr<FR>
  operator ()(FR &&r, BUF &buf, UF eq) const requires is_frng<FR> {
    const auto guard = exit_fn([&buf]() {buf.clear();});
    auto it = find_if_not(r, eq);
    const auto ed = end(r);
    if (it == ed)
      return it;
    const auto sz = size(r);
    buf.reserve(sz);
    buf.clear();
    buf.push_back(move(*it));
    for (auto &p : iters(next(it), ed)) {
      if (eq(*p)) {
        *it = move(*p);
        ++it;
      }
      else
        buf.push_back(move(*p));
    }
    for (auto &p : iters(it, end(r))) {
      *p = move(buf.front());
      buf.pop_front();
    }
    return it;
  }
};
inline constexpr fo_stable_partition_with_buffer stable_partition_with_buffer{};
struct fo_stable_partition_with_no_buffer {
  template <class BR, class UF>
  constexpr rng_itr<BR> operator ()(BR &&r, UF eq) const
    requires is_brng<BR> {
    return inner::fns::stable_partition_nobuf_impl(r, eq);
  }
};
inline constexpr
fo_stable_partition_with_no_buffer stable_partition_with_no_buffer{};
struct fo_stable_partition {
  template <class BR, class UF>
  constexpr rng_itr<BR> operator ()(BR &&r, UF eq) const
    requires is_brng<BR> {
    using vt = rng_vt<BR>;
    const auto len = size(r);
    buffer<vt> buf;
    if (len > buf.max_size())
      return inner::fns::stable_partition_nobuf_impl(r, eq);
#ifndef RE_NOEXCEPT
    try {
#endif
      buf.reset(len);
#ifndef RE_NOEXCEPT
    }
    catch (const bad_alloc &) {
      return inner::fns::stable_partition_nobuf_impl(r, eq);
    }
#endif
    return stable_partition_with_buffer(r, buf, eq);
  }
};
inline constexpr fo_stable_partition stable_partition{};

}

// merge
// inplace_merge_with_no_buffer
// inplace_merge_with_buffer
// inplace_merge
namespace re {

struct fo_merge_plus {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
  operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires (is_rng<IR> && is_rng<IR2> && is_itr<OI>) {
    const auto ed = end(r);
    const auto ed2 = end(r2);
    auto p = begin(r);
    auto p2 = begin(r2);
    if (p == ed) {
      const auto tmp = copy_plus(rng(p2, ed2), o);
      return {p, tmp.first, tmp.second};
    }
    if (p2 == ed2) {
      const auto tmp = copy_plus(rng(p, ed), o);
      return {tmp.first, p2, tmp.second};
    }
    for (;;) {
      if (less(*p2, *p)) {
        *o = *p2;
        ++o;
        ++p2;
        if (p2 == ed2) {
          const auto tmp = copy_plus(rng(p, ed), o);
          return {tmp.first, p2, tmp.second};
        }
      }
      else {
        *o = *p;
        ++o;
        ++p;
        if (p == ed) {
          const auto tmp = copy_plus(rng(p2, ed2), o);
          return {p, tmp.first, tmp.second};
        }
      }
    }
  }
};
inline constexpr fo_merge_plus merge_plus{};
struct fo_merge {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr OI operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires (is_rng<IR> && is_rng<IR2> && is_itr<OI>) {
    return at<2>(merge_plus(r, r2, o, less));
  }
};
inline constexpr fo_merge merge{};

namespace inner::fns {

template <class BI, class BF>
constexpr bool inplace_merge_nobuf_clear_right(BI &op, BI &mid, BI &ed,
                                               BF less) {
  const auto it = upper_bound(rng(op, mid), *prev(ed), less);
  ed = rotate(rng(it, ed), mid);
  mid = it;
  return op == mid;
}
template <class BI, class BF>
constexpr bool inplace_merge_nobuf_clear_left(BI &op, BI &mid, BI &ed,
                                              BF less) {
  const auto it = lower_bound(rng(mid, ed), *op, less);
  op = rotate(rng(op, it), mid);
  mid = it;
  return mid == ed;
}
template <class BI, class BF>
constexpr void inplace_merge_nobuf_recurse(BI op, BI mid, BI ed, BF less) {
  // requires: !less(*mid, *op) && !less(*(ed - 1), *(mid - 1))
  //           && op != mid && mid != ed
  const auto n1 = distance(op, mid);
  const auto n2 = distance(mid, ed);
  using szt = make_unsigned_t<itr_dft<BI>>;
  if ((szt)n1 < (szt)n2) {
    auto it = next(mid, n2 / 2);
    if (it == mid)
      rotate(rng(upper_bound(rng(op, mid), *it, less), ed), it);
    else {
      auto it2 = upper_bound(rng(op, mid), *prev(it), less);
      mid = rotate(rng(it2, it), mid);
      auto mid2 = mid;
      if (inner::fns::inplace_merge_nobuf_clear_right(op, it2, mid, less))
        return;
      if (inner::fns::inplace_merge_nobuf_clear_left(mid2, it, ed, less))
        return;
      inplace_merge_nobuf_recurse(op, it2, mid, less);
      inplace_merge_nobuf_recurse(mid2, it, ed, less);
    }
    //  [op, mid)  [mid, ed)
    //  [op, mid)  [mid, it) [it, ed)
    //  [op, it2) [it2, mid)  [mid, it) [it, ed)
    // *it2 > *(it - 1)
    // *(it2 - 1) <= *(it - 1)
    // rotate middle 2 seqs
    // now everyone of left two sequences is less than any of the rest
  }
  else {
    auto it = next(op, n1 / 2);
    if (it == op)
      rotate(rng(it, lower_bound(rng(mid, ed), *it, less)), mid);
    else {
      auto it2 = lower_bound(rng(mid, ed), *it, less);
      mid = rotate(rng(it, it2), mid);
      auto mid2 = mid;
      if (inner::fns::inplace_merge_nobuf_clear_right(op, it, mid, less))
        return;
      if (inner::fns::inplace_merge_nobuf_clear_left(mid2, it2, ed, less))
        return;
      inplace_merge_nobuf_recurse(op, it, mid, less);
      inplace_merge_nobuf_recurse(mid2, it2, ed, less);
    }
    // [op, mid)  [mid, ed)
    // [op, it) [it, mid)  [mid, ed)
    // [op, it) [it, mid)  [mid, it2) [it2, ed)
    // *(it2 - 1) < *it
    // *it2 >= *it
    // rotate middle 2 seqs
    // now everyone of the left two sequences is less than any of the rest
  }
}
template <class BI, class BF>
void inplace_merge_nobuf_to_recurse(BI op, BI mid, BI ed, BF less) {
  if constexpr (is_ritr<BI>) {
    inner::fns::inplace_merge_nobuf_recurse(op, mid, ed, less);
  }
  else {
    const auto n1 = distance(op, mid);
    const auto n2 = distance(mid, ed);
    inner::fns::inplace_merge_nobuf_recurse(counted_iterator<BI>(op, n1 + n2),
                                            counted_iterator<BI>(mid, n2),
                                            counted_iterator<BI>(ed, 0),
                                            less);
  }
}

template <class BI, class BF>
void inplace_merge_nobuf_impl(BI op, BI mid, BI ed, BF less) {
  if (op == mid || mid == ed)
    return;
  if (inner::fns::inplace_merge_nobuf_clear_right(op, mid, ed, less))
    return;
  if (inner::fns::inplace_merge_nobuf_clear_left(op, mid, ed, less))
    return;
  inner::fns::inplace_merge_nobuf_to_recurse(op, mid, ed, less);
}

}
struct fo_inplace_merge_with_no_buffer {
  template <class BR, class BF = less<>>
  constexpr void operator ()(BR &&r, rng_itr<BR> mid, BF less = {}) const
    requires is_brng<BR> {
    inner::fns::inplace_merge_nobuf_impl(begin(r), mid, end(r), less);
  }
};
inline constexpr fo_inplace_merge_with_no_buffer inplace_merge_with_no_buffer{};
struct fo_inplace_merge_with_buffer {
  template <class BR, class BUF, class BF = less<>>
  constexpr void operator ()(BR &&r, rng_itr<BR> mid,
                             BUF &buf, BF less = {}) const
    requires is_brng<BR> {
    const auto op = begin(r);
    buf.reserve(size(r));
    buf.clear();
    copy(rng(op, mid), to_back(buf));
    merge(move_rng(buf), move_rng(mid, end(r)), op, less);
    buf.clear();
  }
};
inline constexpr fo_inplace_merge_with_buffer inplace_merge_with_buffer{};
struct fo_inplace_merge {
  template <class BR, class BF = less<>>
  constexpr void operator ()(BR &&r, rng_itr<BR> mid, BF less = {}) const
    requires is_brng<BR> {
    buffer<rng_vt<BR>> buf;
#ifndef RE_NOEXCEPT
    try {
#endif
      buf.reset(size(r));
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      inplace_merge_with_no_buffer(r, mid, less);
    }
#endif
    inplace_merge_with_buffer(r, mid, buf, less);
  }
};
inline constexpr fo_inplace_merge inplace_merge{};

}

// includes
// set_union
// set_intersection
// set_difference
// set_symmetric_difference
namespace re {

struct fo_includes {
  template <class IR, class IR2, class BF = less<>>
  constexpr bool operator ()(IR &&r, IR2 &&r2, BF less = {}) const
    requires is_rng<IR> {
    auto it = begin(r);
    const auto ed = end(r);
    for (auto &p : iters(r2)) {
      for (;;) {
        if (it == ed)
          return false;
        if (less(*it, *p))
          ++it;
        else
          break;
      }
      if (less(*p, *it))
        return false;
      ++it;
    }
    return true;
  }
};
inline constexpr fo_includes includes{};

namespace inner::fns {

template <bool ENABLE, class R, class OI>
constexpr pair<rng_itr<R>, OI> copy_all_plus(R r, OI o) {
  if constexpr (ENABLE) {
    return copy_plus(r, o);
  }
  else if constexpr (is_brng<R>) {
    return {end(r), o};
  }
  else {
    return {next(begin(r), end(r)), o};
  }
}
template <bool ENABLE, class R, class OI>
constexpr OI copy_all(R r, OI o) {
  if constexpr (ENABLE) {
    return copy(r, o);
  }
  else {
    return o;
  }
}
template <bool ENABLE, class II, class OI>
constexpr void copy_one(II &i, OI &o) {
  if constexpr (ENABLE) {
    *o = *i;
    ++o;
    ++i;
  }
  else {
    ++i;
  }
}
template <bool ENABLE, class II, class II2, class OI>
constexpr void copy_one(II &a, II2 &b, OI &o) {
  if constexpr (ENABLE) {
    *o = *a;
    ++o;
    ++a;
    ++b;
  }
  else {
    ++a;
    ++b;
  }
}
// A, B and C respectively denote
// set1 - set2,
// (set1 + set2) - (set1 - set2) - (set2 - set1)
// and set2 - set1
template <bool ENABLE_A, bool ENABLE_B, bool ENABLE_C,
          class IR, class IR2, class OI, class BF>
constexpr OI set_operation(IR &&r, IR2 &&r2, OI o, BF less) {
  auto it = begin(r);
  const auto ed = end(r);
  auto it2 = begin(r2);
  const auto ed2 = end(r2);
  for (;;) {
    if (it == ed)
      return inner::fns::copy_all<ENABLE_C>(rng(it2, ed2), o);
    if (it2 == ed2)
      return inner::fns::copy_all<ENABLE_A>(rng(it, ed), o);
    if (less(*it, *it2))
      inner::fns::copy_one<ENABLE_A>(it, o);
    else if (less(*it2, *it))
      inner::fns::copy_one<ENABLE_C>(it2, o);
    else
      inner::fns::copy_one<ENABLE_B>(it, it2, o);
  }
}
template <bool ENABLE_A, bool ENABLE_B, bool ENABLE_C,
          class IR, class IR2, class OI, class BF>
constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
set_operation_plus(IR &&r, IR2 &&r2, OI o, BF less) {
  auto it = begin(r);
  const auto ed = end(r);
  auto it2 = begin(r2);
  const auto ed2 = end(r2);
  for (;;) {
    if (it == ed) {
      auto tmp = inner::fns::copy_all_plus<ENABLE_C>(rng(it2, ed2), o);
      return {it, tmp.first, tmp.second};
    }
    if (it2 == ed2) {
      auto tmp = inner::fns::copy_all_plus<ENABLE_A>(rng(it, ed), o);
      return {tmp.first, it2, tmp.second};
    }
    if (less(*it, *it2))
      inner::fns::copy_one<ENABLE_A>(it, o);
    else if (less(*it2, *it))
      inner::fns::copy_one<ENABLE_C>(it2, o);
    else
      inner::fns::copy_one<ENABLE_B>(it, it2, o);
  }
}

}
struct fo_set_union {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr OI operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation<true, true, true>(r, r2, o, less);
  }
};
inline constexpr fo_set_union set_union{};
struct fo_set_intersection {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr OI operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation<false, true, false>(r, r2, o, less);
  }
};
inline constexpr fo_set_intersection set_intersection{};
struct fo_set_difference {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr OI operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation<true, false, false>(r, r2, o, less);
  }
};
inline constexpr fo_set_difference set_difference{};
struct fo_set_symmetric_difference {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr OI operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation<true, false, true>(r, r2, o, less);
  }
};
inline constexpr fo_set_symmetric_difference set_symmetric_difference{};

struct fo_set_union_plus {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
  operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation_plus<true, true, true>(r, r2, o, less);
  }
};
inline constexpr fo_set_union_plus set_union_plus{};
struct fo_set_intersection_plus {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
  operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation_plus<false, true, false>(r, r2, o, less);
  }
};
inline constexpr fo_set_intersection_plus set_intersection_plus{};
struct fo_set_difference_plus {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
  operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation_plus<true, false, false>(r, r2, o, less);
  }
};
inline constexpr fo_set_difference_plus set_difference_plus{};
struct fo_set_symmetric_difference_plus {
  template <class IR, class IR2, class OI, class BF = less<>>
  constexpr tuple<rng_itr<IR>, rng_itr<IR2>, OI>
  operator ()(IR &&r, IR2 &&r2, OI o, BF less = {}) const
    requires is_rng<IR> {
    return inner::fns::set_operation_plus<true, false, true>(r, r2, o, less);
  }
};
inline constexpr fo_set_symmetric_difference_plus
set_symmetric_difference_plus{};

}

// push_heap
// pop_heap
// make_heap
// sort_heap
// is_heap
// is_heap_until
// heap_sort
// heap_push
// heap_pop
namespace re {

namespace inner::fns {

template <class SZ>
constexpr bool heap_is_lchild(SZ offset) {
  return offset % 2;
}
template <class SZ>
constexpr bool heap_is_rchild(SZ offset) {
  return !(offset % 2);
}
template <class SZ>
constexpr auto heap_parent(SZ offset) {
  return (offset - 1) / 2;
}
template <class SZ>
constexpr auto heap_lchild(SZ offset) {
  return offset * 2 + 1;
}
template <class SZ>
constexpr auto heap_rchild(SZ offset) {
  return offset * 2 + 2;
}

template <class RR, class BF>
constexpr auto is_heap_until_impl(RR &&r, BF less) {
  using itr_t = rng_itr<RR>;
  using dif_t = rng_dft<RR>;

  const itr_t p = begin(r);
  const dif_t n = ssize(r);
  if (n <= 1)
    return make_pair(true, p + n);
  const bool y = inner::fns::heap_is_rchild(n);
  const dif_t nn = y ? n - 1 : n;

  dif_t i = 1;
  while (i < nn) {
    if (less(p[inner::fns::heap_parent(i)], p[i]))
      return make_pair(false, p + i);
    ++i;
    if (less(p[inner::fns::heap_parent(i)], p[i]))
      return make_pair(false, p + i);
    ++i;
  }
  if (y) {
    if (less(p[inner::fns::heap_parent(i)], p[i]))
      return make_pair(false, p + i);
    ++i;
  }
  return make_pair(true, p + i);
}

}

struct fo_push_heap {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    using itr_t = rng_itr<RR>;
    using dif_t = itr_dft<itr_t>;

    const dif_t n = ssize(r);
    if (n <= 1)
      return;

    const itr_t p = begin(r);
    auto i = n - 1;
    rng_vt<RR> v = move(p[i]);
    for (;;) {
      const auto next_i = inner::fns::heap_parent(i);
      decltype(auto) x = p[next_i];
      if (less(x, v)) {
        p[i] = move(x);
        i = next_i;
      }
      else {
        p[i] = move(v);
        return;
      }
      if (i == 0) {
        *p = move(v);
        return;
      }
    }
  }
};
inline constexpr fo_push_heap push_heap{};

struct fo_pop_heap {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    using itr_t = rng_itr<RR>;
    using dif_t = rng_dft<RR>;

    const dif_t nn = ssize(r);
    if (nn <= 1)
      return;
    const itr_t p = begin(r);
    const dif_t n = nn - 1;
    const dif_t k = inner::fns::heap_parent(n);

    rng_vt<RR> v = move(p[n]);
    p[n] = move(p[0]);
    dif_t i = 0, next_i;
    while (i < k) {
      next_i = inner::fns::heap_lchild(i);
      if (less(p[next_i], p[next_i + 1]))
        ++next_i;
      p[i] = move(p[next_i]);
      i = next_i;
    }
    if (i == k && inner::fns::heap_is_rchild(n)) {
      next_i = n - 1;
      p[i] = move(p[next_i]);
      i = next_i;
    }
    for (; i != 0; i = next_i) {
      next_i = inner::fns::heap_parent(i);
      if (less(p[next_i], v))
        p[i] = move(p[next_i]);
      else
        break;
    }
    p[i] = move(v);
  }
};
inline constexpr fo_pop_heap pop_heap{};

struct fo_make_heap {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    //    prove the number of comparison times is less than 2 * n
    //
    //    define n to represent the elements number of some heap
    //    define h to represent the height of some heap
    //    define N(h) = n to represent the elements number of some heap
    //    define H(n) = h to reprensent the elements number of some heap
    //
    //    N(h) = 2 ^ h - 1
    //    H(n) = log2(n + 1)
    //
    //    define F(h), P(n) to represent the comparision times to make specified
    //    heap in worest-case
    //
    //    for heap which the elements count equal to 2 ^ h - 1
    //
    //      P(n) = F(H(n))
    //
    //      because percolate down one element per step need 2 times comparison,
    //
    //      P(h + 1) = 2 * F(h) + 2 * h
    //      <==>
    //      P(h + 1) + 2 * (h + 1) + 2 = 2 * (F(h) + 2 * h + 2)
    //
    //      because P(1) = 0,
    //      F(h) + 2 * h + 2 = (P(1) + 2 * 1 + 2) * 2 ^ (h - 1)
    //                       = 2 ^ (h + 1) - 2 * (h + 1)
    //
    //      P(n) = F(H(n)) = 2 * n - 2 * log2(n + 1) < 2 * n
    //
    //
    //    for heap which the elements count do not equal to 2 ^ h - 1
    //
    //      define n' to represent the greatest n make the heap size sufficing
    //      "n = 2 ^ h - 1" and belong to [1, n)
    //
    //      if (n - n') % 2 = 0
    //        P(n) < P(n') + 2 * (n - n')
    //             = 2 * n - 2 * log2(n' + 1)
    //             < 2 * n
    //      if (n - n') % 2 = 1
    //        P(n) < P(n') + 2 * (n - n' + 1)
    //             = 2 * n + 2 * (1 - log2(n' + 1))
    //             <= 2 * n

    using itr_t = rng_itr<RR>;
    using dif_t = rng_dft<RR>;

    const dif_t n = ssize(r);
    if (n <= 1)
      return;
    const itr_t p = begin(r);
    const dif_t k = inner::fns::heap_parent(n);

    for (auto j = k + 1; j != 0;) {
      --j;
      dif_t i = j;
      rng_vt<RR> v = move(p[i]);

      for (;;) {
        if (i < k) {
          dif_t next_i = inner::fns::heap_lchild(i);
          if (less(p[next_i], p[next_i + 1]))
            ++next_i;
          if (less(v, p[next_i])) {
            p[i] = move(p[next_i]);
            i = next_i;
          }
          else {
            p[i] = move(v);
            break;
          }
        }
        else {
          if (i == k && inner::fns::heap_is_rchild(n)) {
            const itr_t pp = p + n - 1;
            if (less(v, *pp)) {
              p[i] = move(*pp);
              *pp = move(v);
              break;
            }
          }
          p[i] = move(v);
          break;
        }
      }
    }
  }
};
inline constexpr fo_make_heap make_heap{};

struct fo_sort_heap {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    for (auto rr = rng(r); ssize(rr) >= 2; --rr.second)
      pop_heap(rr, less);
  }
};
inline constexpr fo_sort_heap sort_heap{};

struct fo_is_heap {
  template <class RR, class BF = less<>>
  constexpr bool operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    return inner::fns::is_heap_until_impl(r, less).first;
  }
};
inline constexpr fo_is_heap is_heap{};

struct fo_is_heap_until {
  template <class RR, class BF = less<>>
  constexpr rng_itr<RR> operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    return inner::fns::is_heap_until_impl(r, less).second;
  }
};
inline constexpr fo_is_heap_until is_heap_until{};

struct fo_heap_sort {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    make_heap(r, less);
    sort_heap(r, less);
  }
};
inline constexpr fo_heap_sort heap_sort{};

}

// min
// max
// minmax
// min_element
// max_element
// minmax_element
namespace re {

struct fo_min_element {
  template <class FR, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, BF less = {}) const
    requires is_frng<FR> {
    const auto ed = end(r);
    auto it = begin(r);
    auto ret = it;
    if (it != ed)
      for (++it; it != ed; ++it)
        if (less(*it, *ret))
          ret = it;
    return ret;
  }
};
inline constexpr fo_min_element min_element{};
struct fo_max_element {
  template <class FR, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, BF less = {}) const
    requires is_frng<FR> {
    const auto ed = end(r);
    auto it = begin(r);
    auto ret = it;
    if (it != ed)
      for (++it; it != ed; ++it)
        if (less(*ret, *it))
          ret = it;
    return ret;
  }
};
inline constexpr fo_max_element max_element{};
struct fo_minmax_element {
  template <class FR, class BF = less<>>
  constexpr pair<rng_itr<FR>, rng_itr<FR>>
  operator ()(FR &&r, BF less = {}) const
    requires is_frng<FR> {
    const auto ed = end(r);
    auto next = begin(r);
    auto ret = make_pair(next, next);
    auto &min_it = ret.first;
    auto &max_it = ret.second;

    if (next == ed)
      return ret;
    if (++next == ed)
      return ret;
    if (less(*next, *ret.first))
      ret.first = next;
    else
      ret.second = next;
    for (;;) {
      if (++next == ed)
        break;
      const auto prev = next;
      if (++next == ed) {
        if (less(*prev, *min_it))
          min_it = prev;
        else if (!less(*prev, *max_it))
          max_it = prev;
        break;
      }
      if (!less(*next, *prev)) {
        if (less(*prev, *min_it))
          min_it = prev;
        if (!less(*next, *max_it))
          max_it = next;
      }
      else {
        if (less(*next, *min_it))
          min_it = next;
        if (!less(*prev, *max_it))
          max_it = prev;
      }
    }
    return ret;
  }
};
inline constexpr fo_minmax_element minmax_element{};

struct fo_min {
  template <class T, class BF = less<>>
  constexpr const T &operator ()(const T &a, const T &b,
                                 BF less = {}) const {
    return less(b, a) ? b : a;
  }
  template <class T, class BF = less<>>
  constexpr T operator ()(initializer_list<T> l, BF less = {}) const {
    return *min_element(l, less);
  }
};
inline constexpr fo_min min{};
struct fo_max {
  template <class T, class BF = less<>>
  constexpr const T &operator ()(const T &a, const T &b,
                                 BF less = {}) const {
    return less(a, b) ? b : a;
  }
  template <class T, class BF = less<>>
  constexpr T operator ()(initializer_list<T> l, BF less = {}) const {
    return *max_element(l, less);
  }
};
inline constexpr fo_max max{};
struct fo_minmax {
  template <class T, class BF = less<>>
  constexpr pair<const T &, const T &>
  operator ()(const T &a, const T &b, BF less = {}) const {
    return less(b, a) ? tie(b, a) : tie(a, b);
  }
  template <class T, class BF = less<>>
  constexpr pair<T, T>
  operator ()(initializer_list<T> l, BF less = {}) const {
    auto p = minmax_element(l, less);
    return {*p.first, *p.second};
  }
};
inline constexpr fo_minmax minmax{};

}

// clamp
namespace re {

struct fo_clamp {
  template <class T, class BF = less<>>
  constexpr const T &operator ()(const T &v, const T &lo, const T &hi,
                                 BF less = {}) const {
    if (less(v, lo))
      return lo;
    else if (less(hi, v))
      return hi;
    else
      return v;
  }
};
inline constexpr fo_clamp clamp{};

}

// lexicographical_compare
// lexicographical_compare_three_way
// lexicographical_synth_3way
namespace re {

struct fo_lexicographical_compare {
  template <class IR1, class IR2, class BF = less<>>
  constexpr bool operator ()(IR1 &&r, IR2 &&r2, BF less = {}) const
    requires (is_rng<IR1> && is_rng<IR2>) {
    auto it = begin(r);
    const auto ed = end(r);
    auto it2 = begin(r2);
    const auto ed2 = end(r2);
    for (; it != ed && it2 != ed2; ++it, (void)++it2) {
      if (less(*it, *it2))
        return true;
      if (less(*it2, *it))
        return false;
    }
    return it == ed && it2 != ed2;
  }
};
inline constexpr fo_lexicographical_compare lexicographical_compare{};

struct fo_lexicographical_compare_three_way {
  template <class IR1, class IR2, class BF = compare_three_way>
  constexpr auto operator ()(IR1 &&r, IR2 &&r2, BF cmp = {}) const
    ->decltype(*begin(r) <=> *begin(r2))
    requires (is_rng<IR1> && is_rng<IR2>) {
    auto it = begin(r);
    const auto ed = end(r);
    auto it2 = begin(r2);
    const auto ed2 = end(r2);
    for (; it != ed && it2 != ed2; ++it, (void)++it2)
      if (auto c = cmp(*it, *it2); c != 0)
        return c;
    return cmp((it != ed), (it2 != ed2));
  }
};
inline constexpr fo_lexicographical_compare_three_way
lexicographical_compare_three_way{};

struct fo_lexicographical_synth_3way {
  template <class IR1, class IR2>
  constexpr synth_3way_result<rng_ref<IR1>, rng_ref<IR2>>
  operator ()(IR1 &&r, IR2 &&r2) const
    requires (is_rng<IR1> && is_rng<IR2>) {
    return lexicographical_compare_three_way(r, r2, synth_3way);
  }
};
inline constexpr fo_lexicographical_synth_3way lexicographical_synth_3way{};

}

// next_permutation
// prev_permutation
namespace re {

struct fo_next_permutation {
  template <class BR, class BF = less<>>
  constexpr bool operator ()(BR &&r, BF less = {}) const
    requires is_brng<BR> {
    if (empty(r))
      return false;
    const auto op = begin(r);
    const auto ed = end(r);
    auto it = op;
    ++it;
    if (it == ed)
      return false;
    const auto rr = rrng(op, ed);
    auto it2 = adjacent_find(rr, bind(less, _2, _1));
    if (it2 == end(rr)) {
      reverse(r);
      return false;
    }
    ++it2;
    const auto it3 = find_if(rr, bind(less, bind(deref, it2), _1));
    adl_swap(*it2, *it3);
    reverse(rng(ritr(it2), ritr(begin(rr))));
    return true;
  }
};
inline constexpr fo_next_permutation next_permutation{};
struct fo_prev_permutation {
  template <class BR, class BF = less<>>
  constexpr bool operator ()(BR &&r, BF less = {}) const
    requires is_brng<BR> {
    return next_permutation(r, bind(less, _2, _1));
  }
};
inline constexpr fo_prev_permutation prev_permutation{};

}

// is_sorted_until
// is_sorted
// sort
// stable_sort_with_no_buffer
// stable_sort_with_buffer
// stable_sort,
// partial_sort
// partial_sort_copy
// nth_element
namespace re {

struct fo_is_sorted_until {
  template <class FR, class BF = less<>>
  constexpr rng_itr<FR> operator ()(FR &&r, BF less = {}) const
    requires is_frng<FR> {
    const auto op = begin(r);
    const auto ed = end(r);
    if (op != ed) {
      auto lastp = op;
      for (auto &p : iters(next(op), ed)) {
        if (less(*p, *lastp))
          return p;
        lastp = p;
      }
    }
    return ed;
  }
};
inline constexpr fo_is_sorted_until is_sorted_until{};
struct fo_is_sorted {
  template <class FR, class BF = less<>>
  constexpr bool operator ()(FR &&r, BF less = {}) const
    requires is_frng<FR> {
    return is_sorted_until(r, less) == end(r);
  }
};
inline constexpr fo_is_sorted is_sorted{};

namespace inner::fns {

template <class RI, class BF>
constexpr void insertion_sort_impl(RI op, RI ed, BF less) {
  if (op != ed) {
    for (auto &it : iters(op + 1, ed)) {
      itr_vt<RI> tmp = move(*it);
      if (less(tmp, *op)) {
        move_backward(rng(op, it), it + 1);
        *op = move(tmp);
      }
      else {
        auto it2 = it, it3 = it;
        for (;;) {
          if (less(tmp, *--it2))
            *it3-- = move(*it2);
          else {
            *it3 = move(tmp);
            break;
          }
        }
      }
    }
  }
}

template <class UINT>
constexpr unsigned intro_sort_max_depth(UINT len) {
  unsigned ret = 0;
  while ((len /= 2) != 0)
    ++ret;
  return ret * 2;
}
// qsort_move_pivot_to_begin(it1, it2, it3): requires: it1 < it2 && it2 < it3
template <class RI, class BF>
constexpr void qsort_move_pivot_to_begin(RI it1, RI it2, RI it3, BF less) {
  if (!less(*it2, *it1)) {
    if (!less(*it3, *it2))
      adl_swap(*it1, *it2);
    else if (!less(*it3, *it1))
      adl_swap(*it1, *it3);
  }
  else {
    if (!less(*it2, *it3))
      adl_swap(*it1, *it2);
    else if (!less(*it1, *it3))
      adl_swap(*it1, *it3);
  }
}
template <class RI, class BF>
constexpr void intro_sort_impl(RI op, RI ed, unsigned depth_lim, BF less) {
  const auto len = ed - op;
  if (len <= 12)
    return inner::fns::insertion_sort_impl(op, ed, less);
  if (depth_lim == 0)
    return heap_sort(rng(op, ed), less);
  qsort_move_pivot_to_begin(op, op + len / 2, ed - 1, less);
  auto it = op, it2 = ed;
  for (;;) {
    while (less(*++it, *op));
    while (less(*op, *--it2));
    if (it >= it2)
      break;
    adl_swap(*it, *it2);
  }
  --depth_lim;
  inner::fns::intro_sort_impl(op, it, depth_lim, less);
  inner::fns::intro_sort_impl(it, ed, depth_lim, less);
}

}
struct fo_sort {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    inner::fns::intro_sort_impl(begin(r), end(r),
                                inner::fns::intro_sort_max_depth(size(r)),
                                less);
  }
};
inline constexpr fo_sort sort{};

struct fo_stable_sort_with_no_buffer {
  template <class RR, class UF = less<>>
  constexpr void operator ()(RR &&r, UF less = {}) const
    requires is_rrng<RR> {
    const auto len = ssize(r);
    const auto ed = end(r);
    for (make_signed_t<rng_szt<RR>> stride = 1; stride < len; stride *= 2) {
      auto p = begin(r);
      for (;;) {
        if (ed - p <= stride)
          break;
        const auto mid = p + stride;
        if (ed - mid < stride) {
          inplace_merge_with_no_buffer(rng(p, ed), mid, less);
          break;
        }
        else {
          const auto right = mid + stride;
          inplace_merge_with_no_buffer(rng(p, right), mid, less);
          p = right;
        }
      }
    }
  }
};
inline constexpr fo_stable_sort_with_no_buffer stable_sort_with_no_buffer{};
struct fo_stable_sort_with_buffer {
  template <class RR, class BUF, class BF = less<>>
  constexpr void operator ()(RR &&r, BUF &buf, BF less = {}) const
    requires is_rrng<RR> {
    const auto len = ssize(r);
    const auto ed = end(r);
    if (len == 0)
      return;
    buf.reserve(to_unsigned(len));
    buf.clear();
    for (auto p = begin(r);;) {
      const auto mid = p + 1;
      if (mid == ed) {
        buf.push_back(move(*p));
        break;
      }
      if (less(*mid, *p)) {
        buf.push_back(move(*mid));
        buf.push_back(move(*p));
      }
      else {
        buf.push_back(move(*p));
        buf.push_back(move(*mid));
      }
      p = mid + 1;
      if (p == ed)
        break;
    }
    const auto merge_through_consecutive_gaps
      = [](auto &&r, auto o, auto gaplen, auto less) {
      const auto ed = end(r);
      auto p = begin(r);
      for (;;) {
        if (ed - p <= gaplen) {
          move(rng(p, ed), o);
          return;
        }
        const auto mid = p + gaplen;
        if (ed - mid < gaplen) {
          o = merge(move_rng(p, mid), move_rng(mid, ed), o, less);
          return;
        }
        else {
          const auto right = mid + gaplen;
          o = merge(move_rng(p, mid), move_rng(mid, right), o, less);
          p = right;
        }
      }
    };
    for (rng_szt<RR> stride = 2;;) {
      if (stride >= to_unsigned(len)) {
        move(buf, begin(r));
        break;
      }
      merge_through_consecutive_gaps(buf, begin(r), to_signed(stride), less);
      stride += stride;
      if (stride >= to_unsigned(len))
        break;
      merge_through_consecutive_gaps(r, begin(buf), to_signed(stride), less);
      stride += stride;
    }
  }
};
inline constexpr fo_stable_sort_with_buffer stable_sort_with_buffer{};
struct fo_stable_sort {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, BF less = {}) const
    requires is_rrng<RR> {
    const auto len = size(r);
    if (len < 70)
      return inner::fns::insertion_sort_impl(begin(r), end(r), less);

    buffer<rng_vt<RR>> buf;
#ifndef RE_NOEXCEPT
    try {
#endif
      buf.reset(len);
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      return stable_sort_with_no_buffer(r, less);
    }
#endif
    stable_sort_with_buffer(r, buf, less);
  }
};
inline constexpr fo_stable_sort stable_sort{};

struct fo_partial_sort {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, rng_itr<RR> mid, BF less = {}) const
    requires is_rrng<RR> {
    const auto op = begin(r);
    const auto front_part = rng(op, mid);
    if (op != mid) {
      make_heap(front_part, less);
      for (auto &p : iters(mid, end(r)))
        if (less(*p, *op)) {
          pop_heap(front_part, less);
          adl_swap(*p, *prev(mid));
          push_heap(front_part, less);
        }
      sort_heap(front_part, less);
    }
  }
};
inline constexpr fo_partial_sort partial_sort{};
struct fo_partial_sort_copy {
  template <class IR, class RR, class BF = less<>>
  constexpr rng_itr<RR> operator ()(IR &&r, RR &&r2, BF less = {}) const
    requires is_rng<IR> && is_rrng<RR> {
    const auto ed = end(r);
    const auto ed2 = end(r2);
    const auto op2 = begin(r2);
    auto it = begin(r);
    auto o = begin(r2);
    for (; it != ed && o != ed2; ++o, (void)++it)
      *o = *it;
    if (op2 != o) {
      const auto heap_seq = rng(op2, o);
      make_heap(heap_seq, less);
      for (; it != ed; ++it)
        if (less(*it, *begin(heap_seq))) {
          pop_heap(heap_seq, less);
          *prev(end(heap_seq)) = *it;
          push_heap(heap_seq, less);
        }
      sort_heap(heap_seq, less);
      return end(heap_seq);
    }
    else
      return op2;
  }
};
inline constexpr fo_partial_sort_copy partial_sort_copy{};

struct fo_nth_element {
  template <class RR, class BF = less<>>
  constexpr void operator ()(RR &&r, rng_itr<RR> mid, BF less = {}) const
    requires is_rrng<RR> {
    auto rr = rng(r);
    if (mid == end(rr))
      return;
    for (;;) {
      const auto len = size(rr);
      if (len <= 2) {
        if (len == 2 && less(*prev(end(rr)), *begin(rr)))
          adl_swap(*begin(rr), *prev(end(rr)));
        return;
      }
      inner::fns::qsort_move_pivot_to_begin(begin(rr), begin(rr) + len / 2,
                                            end(rr) - 1, less);
      auto it = begin(rr);
      auto it2 = end(rr);
      for (;;) {
        while (less(*++it, *begin(rr)));
        while (less(*begin(rr), *--it2));
        if (it >= it2)
          break;
        adl_swap(*it, *it2);
      }
      if (mid < it)
        rr.second = it;
      else
        rr.first = it;
    }
  }
};
inline constexpr fo_nth_element nth_element{};

}

// accumulate(_plus)
// reduce(_plus)
namespace re {

struct fo_accumulate_plus {
  template <class IR, class T = rng_vt<IR>, class BF = plus<>>
  constexpr pair<rng_itr<IR>, T>
  operator ()(IR &&r, T init = T{}, BF plus = {}) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      init = plus(move(init), *it);
    return {it, move(init)};
  }
};
inline constexpr fo_accumulate_plus accumulate_plus{};
struct fo_accumulate {
  template <class IR, class T = rng_vt<IR>, class BF = plus<>>
  constexpr T operator ()(IR &&r, T init = T{}, BF plus = {}) const
    requires is_rng<IR> {
    for (auto &p : iters(r))
      init = plus(move(init), *p);
    return init;
  }
};
inline constexpr fo_accumulate accumulate{};
struct fo_reduce_plus {
  template <class IR, class T = rng_vt<IR>, class BF = plus<>>
  constexpr pair<rng_itr<IR>, T>
  operator ()(IR &&r, T init = T{}, BF plus = {}) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      init = plus(init, *it);
    return {it, move(init)};
  }
};
inline constexpr fo_reduce_plus reduce_plus{};
struct fo_reduce {
  template <class IR, class T = rng_vt<IR>, class BF = plus<>>
  constexpr T operator ()(IR &&r, T init = T{}, BF plus = {}) const
    requires is_rng<IR> {
    for (auto &p : iters(r))
      init = plus(init, *p);
    return init;
  }
};
inline constexpr fo_reduce reduce{};

}

// inner_product(_plus)
// transform_reduce(_plus)
namespace re {

struct fo_inner_product_plus {
  template <class IR, class II, class T, class BF, class BF2>
  constexpr tuple<rng_itr<IR>, II, T>
  operator ()(IR &&r, II it2, T init, BF plus, BF2 mul) const
    requires (is_rng<IR> && is_itr<II>) {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it) {
      init = plus(move(init), mul(*it, *it2));
      ++it2;
    }
    return {it, it2, move(init)};
  }
  template <class IR, class II, class T>
  constexpr tuple<rng_itr<IR>, II, T>
  operator ()(IR &&r, II it, T init) const
    requires (is_rng<IR> && is_itr<II>) {
    return operator ()(r, it, move(init), plus{}, multiplies{});
  }
};
inline constexpr fo_inner_product_plus inner_product_plus{};
struct fo_inner_product {
  template <class IR, class II, class T, class BF, class BF2>
  constexpr T operator ()(IR &&r, II it, T init, BF plus, BF2 mul) const
    requires (is_rng<IR> && is_itr<II>) {
    for (auto &p : iters(r)) {
      init = plus(move(init), mul(*p, *it));
      ++it;
    }
    return init;
  }
  template <class IR, class II, class T>
  constexpr T operator ()(IR &&r, II it, T init) const
    requires (is_rng<IR> && is_itr<II>) {
    return operator ()(r, it, move(init), plus{}, multiplies{});
  }
};
inline constexpr fo_inner_product inner_product{};

struct fo_transform_reduce_plus {
  template <class IR, class II, class T, class BF, class BF2>
  constexpr tuple<rng_itr<IR>, II, T>
  operator ()(IR &&r, II it2, T init, BF plus, BF2 mul) const
    requires (is_rng<IR> && is_itr<II>) {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it) {
      init = plus(init, mul(*it, *it2));
      ++it2;
    }
    return {it, it2, init};
  }
  template <class IR, class II, class T>
  constexpr tuple<rng_itr<IR>, II, T> operator ()(IR &&r, II it, T init) const
    requires (is_rng<IR> && is_itr<II>) {
    return operator ()(r, it, move(init), plus{}, multiplies{});
  }

  template <class IR, class T, class BF, class UF>
  constexpr pair<rng_itr<IR>, T>
  operator ()(IR &&r, T init, BF plus, UF proj) const
    requires is_rng<IR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      init = plus(init, proj(*it));
    return {it, init};
  }
};
inline constexpr fo_transform_reduce_plus transform_reduce_plus{};
struct fo_transform_reduce {
  template <class IR, class II, class T, class BF, class BF2>
  constexpr T operator ()(IR &&r, II it, T init, BF plus, BF2 mul) const
    requires (is_rng<IR> && is_itr<II>) {
    for (auto &p : iters(r)) {
      init = plus(init, mul(*p, *it));
      ++it;
    }
    return init;
  }
  template <class IR, class II, class T>
  constexpr T operator ()(IR &&r, II it, T init) const
    requires (is_rng<IR> && is_itr<II>) {
    return operator ()(r, it, forward<T>(init), plus{}, multiplies{});
  }

  template <class IR, class T, class BF, class UF>
  constexpr T operator ()(IR &&r, T init, BF plus, UF proj) const
    requires is_rng<IR> {
    for (auto &p : iters(r))
      init = plus(init, proj(*p));
    return init;
  }
};
inline constexpr fo_transform_reduce transform_reduce{};

}

// partial_sum(_plus)
// transform_exclusive_scan(_plus)
// exclusive_scan(_plus)
// transform_inclusive_scan(_plus)
// inclusive_scan(_plus)
namespace re {

struct fo_partial_sum_plus {
  template <class IR, class OI, class BF = plus<>>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed) {
      auto acc = *it;
      *o = acc;
      ++o;
      for (++it; it != ed; ++it) {
        acc = plus(move(acc), *it);
        *o = acc;
        ++o;
      }
    }
    return {it, o};
  }
};
inline constexpr fo_partial_sum_plus partial_sum_plus{};
struct fo_partial_sum {
  template <class IR, class OI, class BF = plus<>>
  constexpr OI operator ()(IR &&r, OI o, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return partial_sum_plus(r, o, plus).second;
  }
};
inline constexpr fo_partial_sum partial_sum{};

struct fo_transform_exclusive_scan_plus {
  template <class IR, class OI, class T, class BF, class UF>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, T init, BF plus, UF uop) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed) {
      T acc = move(init);
      *o = acc;
      ++o;
      for (;;) {
        T ex = uop(*it);
        ++it;
        if (it == ed)
          break;
        acc = plus(acc, ex);
        *o = acc;
        ++o;
      }
    }
    return {it, o};
  }
};
inline constexpr fo_transform_exclusive_scan_plus
transform_exclusive_scan_plus{};
struct fo_transform_exclusive_scan {
  template <class IR, class OI, class T, class BF, class UF>
  constexpr OI operator ()(IR &&r, OI o, T init, BF plus, UF uop) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_exclusive_scan_plus(r, o, move(init), plus, uop).second;
  }
};
inline constexpr fo_transform_exclusive_scan transform_exclusive_scan{};

struct fo_transform_inclusive_scan_plus {
  template <class IR, class OI, class BF, class UF, class T>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, BF plus, UF uop, T init) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    T acc = move(init);
    for (; it != ed; ++it) {
      acc = plus(acc, uop(*it));
      *o = acc;
      ++o;
    }
    return {it, o};
  }
  template <class IR, class OI, class BF, class UF>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, BF plus, UF uop) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed) {
      rng_vt<IR> acc = uop(*it);
      *o = acc;
      ++o;
      for (++it; it != ed; ++it) {
        acc = plus(acc, uop(*it));
        *o = acc;
        ++o;
      }
    }
    return {it, o};
  }
};
inline constexpr fo_transform_inclusive_scan_plus
transform_inclusive_scan_plus{};
struct fo_transform_inclusive_scan {
  template <class IR, class OI, class BF, class UF, class T>
  constexpr OI operator ()(IR &&r, OI o, BF plus, UF uop, T init) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, uop, move(init)).second;
  }
  template <class IR, class OI, class BF, class UF>
  constexpr OI operator ()(IR &&r, OI o, BF plus, UF uop) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, uop).second;
  }
};
inline constexpr fo_transform_inclusive_scan transform_inclusive_scan{};

struct fo_exclusive_scan_plus {
  template <class IR, class OI, class T, class BF = plus<>>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, T init, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_exclusive_scan_plus(r, o, move(init), plus, identity{});
  }
};
inline constexpr fo_exclusive_scan_plus exclusive_scan_plus{};
struct fo_exclusive_scan {
  template <class IR, class OI, class T, class BF = plus<>>
  constexpr OI operator ()(IR &&r, OI o, T init, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_exclusive_scan_plus(r, o, move(init), plus, identity{})
      .second;
  }
};
inline constexpr fo_exclusive_scan exclusive_scan{};

struct fo_inclusive_scan_plus {
  template <class IR, class OI, class BF, class T>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, BF plus, T init) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, identity{}, move(init));
  }
  template <class IR, class OI, class BF = plus<>>
  constexpr pair<rng_itr<IR>, OI>
  operator ()(IR &&r, OI o, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, identity{});
  }
};
inline constexpr fo_inclusive_scan_plus inclusive_scan_plus{};
struct fo_inclusive_scan {
  template <class IR, class OI, class BF, class T>
  constexpr OI operator ()(IR &&r, OI o, BF plus, T init) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, identity{}, move(init))
      .second;
  }
  template <class IR, class OI, class BF = plus<>>
  constexpr OI operator ()(IR &&r, OI o, BF plus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return transform_inclusive_scan_plus(r, o, plus, identity{}).second;
  }
};
inline constexpr fo_inclusive_scan inclusive_scan{};

}

// adjacent_difference(_plus)
namespace re {

struct fo_adjacent_difference_plus {
  template <class IR, class OI, class BF = minus<>>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, BF minus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed) {
      rng_vt<IR> tmp = *it;
      *o = tmp;
      ++o;
      for (++it; it != ed; ++it) {
        *o = minus(*it, move(tmp));
        ++o;
        tmp = *it;
      }
    }
    return {it, o};
  }
};
inline constexpr fo_adjacent_difference_plus adjacent_difference_plus{};
struct fo_adjacent_difference {
  template <class IR, class OI, class BF = minus<>>
  constexpr OI operator ()(IR &&r, OI o, BF minus = {}) const
    requires (is_rng<IR> && is_itr<OI>) {
    return adjacent_difference_plus(r, o, minus).second;
  }
};
inline constexpr fo_adjacent_difference adjacent_difference{};

}

// iota
namespace re {

struct fo_iota {
  template <class FR, class T>
  constexpr void operator ()(FR &&r, T x) const requires is_frng<FR> {
    for (auto &p : iters(r)) {
      *p = as_const(x);
      ++x;
    }
  }
};
inline constexpr fo_iota iota{};

}

// gcd and lcm
namespace re {

namespace inner::fns {

template <class X>
constexpr void gcd_lcm_impl_set_as_abs(X &x) {
  if constexpr (is_signed_v<X>) {
    if (x < 0)
      x = -x;
  }
}

}
struct fo_gcd {
  template <class T, class S>
  constexpr common_type_t<T, S> operator ()(T t, S s) const
    requires (integral<T> && integral<S>
              && !is_same_v<remove_cvref_t<T>, bool>
              && !is_same_v<remove_cvref_t<S>, bool>) {
    inner::fns::gcd_lcm_impl_set_as_abs(t);
    inner::fns::gcd_lcm_impl_set_as_abs(s);
    if (t == 0)
      return s;
    if (s == 0)
      return t;
    common_type_t<T, S> a = 0, b = 0, c = 0;
    if (to_unsigned(s) < to_unsigned(t)) {
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
};
inline constexpr fo_gcd gcd{};

struct fo_lcm {
  template <class T, class S>
  constexpr common_type_t<T, S> operator ()(T t, S s) const
    requires (integral<T> && integral<S>
              && !is_same_v<remove_cvref_t<T>, bool>
              && !is_same_v<remove_cvref_t<S>, bool>) {
    inner::fns::gcd_lcm_impl_set_as_abs(t);
    inner::fns::gcd_lcm_impl_set_as_abs(s);
    return (t == 0 || s == 0)
      ? static_cast<common_type_t<T, S>>(0)
      : static_cast<common_type_t<T, S>>(t / gcd(t, s) * s);
  }
};
inline constexpr fo_lcm lcm{};

}

// midpoint
namespace re {

// todo: float point version
struct fo_midpoint {
  template <class T>
  constexpr T operator ()(T x, T y) const
    requires (unsigned_integral<T> && !is_same_v<remove_cvref_t<T>, bool>) {
    using uint_t = T;
    if (x > y) {
      uint_t dif = x - y;
      return y + divides<uint_t>{}(dif, 2) + bit_and<uint_t>{}(dif, 1);
    }
    else {
      uint_t dif = y - x;
      return x + divides<uint_t>{}(dif, 2);
    }
  }
  template <class T>
  constexpr T operator ()(T x, T y) const
    requires (signed_integral<T> && !is_same_v<remove_cvref_t<T>, bool>) {
    using int_t = T;
    using uint_t = make_unsigned_t<T>;
    if (x > y) {
      uint_t dif = x - y;
      return y
        + static_cast<int_t>(divides<uint_t>{}(dif, 2))
        + static_cast<int_t>(bit_and<uint_t>{}(dif, 1));
    }
    else {
      uint_t dif = y - x;
      return x + static_cast<int_t>(divides<uint_t>{}(dif, 2));
    }
  }

  template <class T>
  constexpr T *operator ()(T *a, T *b) const requires is_object_v<T> {
    return a + (b - a) / 2;
  }
};
inline constexpr fo_midpoint midpoint{};

}

// for_each_node
namespace re {

struct fo_for_each_node {
  template <class R, class GET_NEXT, class FN>
  void operator ()(R &&r, GET_NEXT get_next, FN fn) const {
    const auto ed = end(r);
    auto it = begin(r);
    while (it != ed) {
      const auto tmp = get_next(it);
      fn(it);
      it = tmp;
    }
  }
  template <class R, class GET_NEXT, class FN, class DF>
  void operator ()(R &&r, GET_NEXT get_next, FN fn, DF destroy) const {
    const auto ed = end(r);
    auto it = begin(r);
#ifndef RE_NOEXCEPT
    try {
#endif
      while (it != ed) {
        const auto tmp = get_next(it);
        fn(it);
        it = tmp;
      }
#ifndef RE_NOEXCEPT
    }
    catch (...) {
      while (it != ed) {
        const auto tmp = get_next(it);
        destroy(it);
        it = tmp;
      }
      throw;
    }
#endif
  }
};
inline constexpr fo_for_each_node for_each_node{};

}

// list_fns
// forward_list_fns
namespace re {

template <class GET_NEXT, class SET_NEXT, class GET_PREV, class SET_PREV>
class list_functions {
  GET_NEXT get_next;
  SET_NEXT set_next;
  GET_PREV get_prev;
  SET_PREV set_prev;

public:
  list_functions() = default;
  ~list_functions() = default;
  list_functions(const list_functions &) = delete;
  list_functions &operator =(const list_functions &) = delete;
  list_functions(list_functions &&) = default;
  list_functions &operator =(list_functions &&) = delete;

  list_functions(GET_NEXT x, SET_NEXT y, GET_PREV z, SET_PREV w)
    : get_next(x), set_next(y), get_prev(z), set_prev(w) {}

  template <class IT>
  IT link(IT next, IT node) const {
    const IT prev = get_prev(next);
    set_next(node, next);
    set_prev(node, prev);
    set_next(prev, node);
    set_prev(next, node);
    return node;
  }
  template <class IT>
  IT link(IT next, IT first_it, IT last_it) const {
    const auto prev = get_prev(next);

    set_next(prev, first_it);
    set_prev(first_it, prev);

    set_next(last_it, next);
    set_prev(next, last_it);

    return first_it;
  }
  template <class IT>
  IT unlink(IT node) const {
    const auto prev = get_prev(node);
    const auto next = get_next(node);

    set_next(prev, next);
    set_prev(next, prev);

    return next;
  }
  template <class IT>
  IT unlink(IT from, IT to) const {
    const auto prev = get_prev(from);
    set_next(prev, to);
    set_prev(to, prev);
    return to;
  }
  template <class IT>
  void unlink_all(IT end_node) const {
    set_next(end_node, end_node);
    set_prev(end_node, end_node);
  }
};
struct fo_list_fns {
  template <class GET_NEXT, class SET_NEXT,
            class GET_PREV, class SET_PREV>
  list_functions<decay_t<GET_NEXT>, decay_t<SET_NEXT>,
                 decay_t<GET_PREV>, decay_t<SET_PREV>>
  operator ()(GET_NEXT get_next, SET_NEXT set_next,
              GET_PREV get_prev, SET_PREV set_prev) const {
    return {get_next, set_next, get_prev, set_prev};
  }

  auto operator ()() const {
    auto get_next = [](auto it) {return it.next();};
    auto set_next = [](auto it, auto x) {it.next(x);};
    auto get_prev = [](auto it) {return it.prev();};
    auto set_prev = [](auto it, auto x) {it.prev(x);};
    return operator ()(get_next, set_next, get_prev, set_prev);
  }
};
inline constexpr fo_list_fns list_fns{};

template <class GET_NEXT, class SET_NEXT>
class forward_list_functions {
  GET_NEXT get_next;
  SET_NEXT set_next;

public:
  forward_list_functions() = default;
  ~forward_list_functions() = default;
  forward_list_functions(const forward_list_functions &) = delete;
  forward_list_functions &operator =(const forward_list_functions &) = delete;
  forward_list_functions(forward_list_functions &&) = default;
  forward_list_functions &operator =(forward_list_functions &&) = delete;

  forward_list_functions(GET_NEXT x, SET_NEXT y) : get_next(x), set_next(y) {}

  template <class IT>
  IT link_after(IT prev, IT node) const {
    const auto next = get_next(prev);
    set_next(prev, node);
    set_next(node, next);
    return node;
  }
  template <class IT>
  IT link_after(IT prev, IT it, IT last_it) const {
    const auto next = get_next(prev);
    set_next(prev, it);
    set_next(last_it, next);
    return it;
  }
  template <class IT>
  IT unlink_after(IT prev) const {
    const auto next = get_next(get_next(prev));
    set_next(prev, next);
    return next;
  }
  template <class IT>
  IT unlink_after(IT prev, IT next) const {
    set_next(prev, next);
    return next;
  }
  template <class IT>
  void unlink_all(IT end_node) const {
    set_next(end_node, end_node);
  }
};
struct fo_forward_list_fns {
  template <class GET_NEXT, class SET_NEXT>
  forward_list_functions<decay_t<GET_NEXT>, decay_t<SET_NEXT>>
  operator ()(GET_NEXT get_next, SET_NEXT set_next) const {
    return {get_next, set_next};
  }

  auto operator ()() const {
    auto get_next = [](auto it) {return it.next();};
    auto set_next = [](auto it, auto x) {it.next(x);};
    return operator ()(get_next, set_next);
  }
};
inline constexpr fo_forward_list_fns forward_list_fns{};

}

// list_unique
namespace re {

struct fo_list_unique {
  template <class LIST_LIKE_CONTAINER, class EQ, class ERASE>
  void operator ()(LIST_LIKE_CONTAINER &&l, EQ eq, ERASE erase) const {
    const auto ed = l.end();
    auto it = l.begin();
    while (it != ed) {
      auto next = re::next(it);
      for (;;) {
        if (next == ed)
          return;
        if (eq(*it, *next))
          next = erase(next);
        else {
          it = next;
          break;
        }
      }
    }
  }
  template <class LIST_LIKE_CONTAINER>
  void operator ()(LIST_LIKE_CONTAINER &&l) const {
    operator ()(l, equal_to{}, [&](auto it) {return l.erase(it);});
  }
  template <class LIST_LIKE_CONTAINER, class EQ>
  void operator ()(LIST_LIKE_CONTAINER &&l, EQ eq) const {
    operator ()(l, eq, [&](auto it) {return l.erase(it);});
  }
};
inline fo_list_unique list_unique{};

}

// permutation_count
// combination_count
// combination_range
namespace re {

struct fo_permutation_count {
  template <class T>
  constexpr T operator ()(const T &x) const {
    const T z{};
    T ret{};
    ++ret;
    for (T y = x; y != z; --y)
      ret *= y;
    return ret;
  }
  template <class T>
  constexpr T operator ()(const T &x, const T &k) const {
    const T z{};
    T ret{};
    ++ret;
    for (T y = x, yy = k; yy != z; --yy) {
      ret *= y;
      --y;
    }
    return ret;
  }
};
inline constexpr fo_permutation_count permutation_count{};

struct fo_combination_count {
  template <class T>
  constexpr T operator ()(const T &x, const T &k) const {
    const T z{};    
    T n = x - k;
    if (n > k)
      n = k;

    T ret{};
    ++ret;
    for (T y = x, c = n; c != z; --c) {
      ret *= y;
      --y;
    }
    return ret / permutation_count(n);
  }
};
inline constexpr fo_combination_count combination_count{};

template <class R_ITR, class BUF_T>
class combination_iterator {
  using buf_t = BUF_T;

  buf_t *p = {};
  R_ITR r_ed = R_ITR{};

public:
  using value_type = bind_range<buf_t &, decay_t<decltype(deref)>>;
  using reference = value_type;
  using pointer = void;
  using difference_type = rng_dft<buf_t>;
  using iterator_category = input_iterator_tag;

  combination_iterator() = default;
  ~combination_iterator() = default;
  combination_iterator(const combination_iterator &) = default;
  combination_iterator &operator =(const combination_iterator &) = default;
  combination_iterator(combination_iterator &&) = default;
  combination_iterator &operator =(combination_iterator &&) = default;
  friend constexpr void swap(combination_iterator &x,
                             combination_iterator &y)
    noexcept(is_nothrow_swappable_v<R_ITR>) {
    adl_swap(x.p, y.p);
    adl_swap(x.r_ed, y.r_ed);
  }

  constexpr combination_iterator(buf_t *pp, R_ITR ed) : p(pp), r_ed(ed) {}

  pointer operator ->() const = delete;
  constexpr reference operator *() const {
    return deref_rng(*p);
  }
  constexpr bool operator ==(const combination_iterator &x) const {
    return p == x.p;
  }

  constexpr combination_iterator &operator ++() {
    auto &buf = *p;
    const auto buf_ed = end(buf);
    const auto buf_op = begin(buf);
    auto i = before_end(buf);
    for (;;) {
      ++*i;
      if (const auto d = distance(*i, r_ed);
          d >= buf_ed - i) {
        auto tmp = *i;
        for (++i; i != buf_ed; ++i)
          *i = ++tmp;
        break;
      }
      else {
        if (i == buf_op) {
          p = nullptr;
          break;
        }
        --i;
      }
    }
    return *this;
  }
  value_type *operator ++(int);
};
struct fo_combination_itr {
  template <class BUF, class I>
  combination_iterator<I, BUF> operator ()(BUF *p, I ed) const {
    return combination_iterator<I, BUF>(p, ed);
  }
};
inline constexpr fo_combination_itr combination_itr{};

template <class R>
class combination_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using buf_t = copyable_array<rng_itr<R>>;
  using buf_szt = rng_szt<buf_t>;
  using iter_t = combination_iterator<rng_itr<R>, buf_t>;

  using cache_t = non_propagating_cache<buf_t>;

  R r = R{};
  rng_szt<R> n = rng_szt<R>{};
  cache_t c = cache_t{};

  void delayed_init() {
    if (c.empty()) {
      buf_t tmp_buf(n);
      const auto tmp = for_each(irng(r), tmp_buf, assign_to);
      if (empty(tmp_buf) || tmp.second != end(tmp_buf))
        tmp_buf.clear();
      c.emplace(move(tmp_buf));
    }
  }
  bool finished() {
    const auto op = begin(*c);
    auto it = end(*c);
    auto r_it = end(r);
    while (op != it) {
      --it;
      if (*it != r_it)
        return false;
      --r_it;
    }
    return true;
  }

public:
  combination_range() = delete;
  combination_range() requires is_default_constructible_v<R> = default;
  ~combination_range() = default;
  combination_range(const combination_range &) = default;
  combination_range &operator =(const combination_range &) = default;
  combination_range(combination_range &&) = default;
  combination_range &operator =(combination_range &&) = default;
  template <class RR = R>
  friend void swap(combination_range &x, combination_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR>
              && default_swappable<combination_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.n, y.n);
    adl_swap(x.c, y.c);
  }

  combination_range(R rr, rng_szt<R> nn) : r(forward<R>(rr)), n(nn), c() {
    if (n > numeric_limits<buf_szt>::max())
      throw_or_terminate<length_error>
        ("re::combination_range::combination_range(r, n): too big size\n");
  }

  iter_t begin() {
    delayed_init();
    return iter_t(finished() ? nullptr : addressof(*c), end(r));
  }
  iter_t end() {
    return {nullptr, end(r)};
  }
  bool empty() {
    delayed_init();
    return finished();
  }
};
struct fo_combination_rng {
  template <class R>
  constexpr auto operator ()(R &&r, rng_szt<R> n) const
    requires is_brng<R> {
    return combination_range<rng_forward_t<R>>(forward<R>(r), n);
  }
};
inline constexpr fo_combination_rng combination_rng{};

struct fo_combination {
  template <class BR, class OI>
  OI operator ()(BR &&r, rng_szt<BR> n, OI o) const requires is_brng<BR> {
    // requires: 0 <= n <= size(r)
    using buf_t = copyable_array<rng_itr<BR>>;
    using buf_szt = rng_szt<buf_t>;
    buf_t buf(n);
    if (empty(buf))
      return o;
    copy_from(buf, iitr(begin(r)));
    const auto buf_ed = end(buf);
    const auto buf_op = begin(buf);
    auto r_ed = end(r);

    for (;;) {
      auto tmp_r = deref_rng(buf);
      *o = tmp_r;
      ++o;

      auto i = before_end(buf);
      for (;;) {
        ++*i;
        if (const auto d = distance(*i, r_ed);
            d >= buf_ed - i) {
          auto tmp = *i;
          for (++i; i != buf_ed; ++i)
            *i = ++tmp;
          break;
        }
        else {
          if (i == buf_op)
            return o;
          --i;
        }
      }
    }

    return o;
  }
};
inline constexpr fo_combination combination{};

}

// filter(_plus)
// filter_range
namespace re {

struct fo_filter_plus {
  template <class IR, class OI, class UF>
  constexpr pair<rng_itr<IR>, OI> operator ()(IR &&r, OI o, UF eq) const
    requires is_rng<IR> && is_itr<OI> {
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it)
      if (eq(*it)) {
        *o = *it;
        ++o;
      }
    return {it, o};
  }
};
inline constexpr fo_filter_plus filter_plus{};
struct fo_filter {
  template <class IR, class OI, class UF>
  constexpr OI operator ()(IR &&r, OI o, UF eq) const
    requires is_rng<IR> && is_itr<OI> {
    return filter_plus(r, o, eq).second;
  }
};
inline constexpr fo_filter filter{};

template <class II, class F>
class filter_iterator {
  template <class, class>
  friend class filter_iterator;

  using this_t = filter_iterator;

  II it = II{};
  II ed = II{};
  semiregular_function<F> f = semiregular_function<F>{};

public:
  using value_type = itr_vt<II>;
  using difference_type = itr_dft<II>;
  using reference = itr_ref<II>;
  using pointer = II;
  using iterator_category
    = conditional_t<is_fitr<II>, forward_iterator_tag, itr_ctg<II>>;

  filter_iterator() = default;
  ~filter_iterator() = default;
  filter_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  filter_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<II>
             && is_nothrow_swappable_v<semiregular_function<F>>) {
    adl_swap(x.it, y.it);
    adl_swap(x.ed, y.ed);
    adl_swap(x.f, y.f);
  }

  constexpr filter_iterator(II a, II b, F ff) : it(a), ed(b), f(ff) {}
  constexpr II base() const {
    return it;
  }
  constexpr F fn() const {
    return f.base();
  }

  template <class II2, class F2>
  constexpr filter_iterator(const filter_iterator<II2, F2> &x)
    requires (!(is_same_v<II, II2> && is_same_v<F2, F>)
              && is_convertible_v<const II2 &, II>
              && is_convertible_v<const F2 &, F>)
    : it(x.it), ed(x.ed), f(x.f) {}

  constexpr reference operator *() const {
    return *it;
  }
  constexpr pointer operator ->() const
    requires can_apply_to_address<const II &> {
    return it;
  }
  constexpr this_t &operator ++() {
    for (;;) {
      ++it;
      if (it == ed)
        break;
      if (f(*it))
        break;
    }
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int) requires is_fitr<II> {
    return iter_post_increment(*this);
  }
};
template <class I1, class I2, class F>
constexpr auto operator ==(const filter_iterator<I1, F> &x,
                           const filter_iterator<I2, F> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
struct fo_filter_itr {
  template <class I, class F>
  constexpr auto operator ()(I i, I ed, F f) const requires is_itr<I> {
    return filter_iterator<I, F>(i, ed, f);
  }
};
inline constexpr fo_filter_itr filter_itr{};

template <class R, class F>
class filter_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using cache_t = conditional_t<is_rng_ref<R>, simple_wrapper<rng_itr<R>>,
                                non_propagating_cache<rng_itr<R>>>;

  R r = R{};
  copyable_wrapper<F> f = copyable_wrapper<F>{};
  cache_t c = cache_t{};

  constexpr void init() {
    if constexpr (is_rng_ref<R>) {
      *c = find_if(r, *f);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<R>) {
      if (c.empty())
        c.emplace(find_if(r, *f));
    }
  }

public:
  filter_range() = delete;
  constexpr filter_range() requires (is_default_constructible_v<R>
                                     && is_default_constructible_v<F>) {
    init();
  }
  ~filter_range() = default;
  filter_range(const filter_range &) = default;
  filter_range &operator =(const filter_range &) = default;
  filter_range(filter_range &&) = default;
  filter_range &operator =(filter_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(filter_range &x, filter_range &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<rng_itr<RR>>)
    requires (is_swappable_v<RR>
              && default_swappable<filter_range<RR, F>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.f, y.f);
    adl_swap(x.c, y.c);
  }

  constexpr filter_range(R rr, F ff) : r(forward<R>(rr)), f(ff), c() {
    init();
  }

  constexpr auto begin() {
    if constexpr (is_default_constructible_v<F>) {
      delayed_init();
      return filter_iterator<rng_itr<R>, F>(*c, end(r), *f);
    }
    else {
      delayed_init();
      return filter_iterator<rng_itr<R>, const F *>(*c, end(r), addressof(*f));
    }
  }
  constexpr auto end() {
    if constexpr (is_default_constructible_v<F>) {
      const auto ed = end(r);
      return filter_iterator<rng_itr<R>, F>(ed, ed, *f);
    }
    else {
      const auto ed = end(r);
      return filter_iterator<rng_itr<R>, const F *>(ed, ed, addressof(*f));
    }
  }
  constexpr auto begin() const requires is_rng_ref<add_const_t<R>> {
    if constexpr (is_default_constructible_v<F>) {
      return filter_iterator<rng_itr<const R>, F>(*c, end(r), *f);
    }
    else {
      return filter_iterator<rng_itr<const R>, const F *>
        (*c, end(r), addressof(*f));
    }
  }
  constexpr auto end() const requires is_rng_ref<add_const_t<R>> {
    if constexpr (is_default_constructible_v<F>) {
      const auto ed = end(r);
      return filter_iterator<rng_itr<const R>, F>(ed, ed, *f);
    }
    else {
      const auto ed = end(r);
      return filter_iterator<rng_itr<const R>, const F *>
        (ed, ed, addressof(*f));
    }
  }

  constexpr bool empty() {
    delayed_init();
    return *c == end(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<R>> {
    return *c == end(r);
  }
};
template <class R, class F>
struct is_range_reference<filter_range<R, F>>
  : bool_constant<is_range_reference<R>::value
                  && is_default_constructible_v<F>> {};
struct fo_filter_rng {
  template <class R, class F>
  constexpr auto operator ()(R &&r, F f) const requires is_rng<R> {
    return filter_range<rng_forward_t<R>, F>(forward<R>(r), f);
  }
};
inline constexpr fo_filter_rng filter_rng{};

}

// take(_plus)
// take_range
namespace re {

struct fo_take_plus {
  template <class R, class OI>
  constexpr pair<rng_itr<R>, OI>
  operator ()(R &&r, rng_dft<R> d, OI o) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (; d != 0; --d) {
      if (it != ed) {
        *o = *it;
        ++o;
      }
      else
        break;
      ++it;
    }
    return {it, o};
  }
};
inline constexpr fo_take_plus take_plus{};
struct fo_take {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, rng_dft<R> d, OI o) const {
    return take_plus(r, d, o).second;
  }
};
inline constexpr fo_take take{};

template <class I>
class take_iterator {
  template <class>
  friend class take_iterator;

  I i = I{};
  itr_dft<I> d = itr_dft<I>{};

public:
  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using difference_type = itr_dft<I>;
  using pointer = I;
  using iterator_category = conditional_t
    <is_fitr<I>, forward_iterator_tag, itr_ctg<I>>;

  take_iterator() = default;
  ~take_iterator() = default;
  take_iterator(const take_iterator &) = default;
  take_iterator &operator =(const take_iterator &) = default;
  take_iterator(take_iterator &&) = default;
  take_iterator &operator =(take_iterator &&) = default;
  friend constexpr void swap(take_iterator &x, take_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
    adl_swap(x.d, y.d);
  }

  constexpr take_iterator(I ii, itr_dft<I> dd) : i(ii), d(dd) {}
  constexpr I base() const {
    return i;
  }
  constexpr auto nth() const {
    return d;
  }

  template <class I2>
  constexpr take_iterator(const take_iterator<I2> &x)
    requires (!is_same_v<I, I2> && is_convertible_v<const I2 &, I>)
    : i(x.i), d(x.d) {}

  template <class I1, class I2>
  friend constexpr bool operator ==(const take_iterator<I1> &x,
                                    const take_iterator<I2> &y)
    requires (is_same_v<itr_dft<I1>, itr_dft<I2>>
              && can_apply_equal_to<const I1 &, const I2 &>);

  constexpr reference operator *() const {
    return *i;
  }
  constexpr I operator ->() const
    requires can_apply_to_address<const I &> {
    return i;
  }
  constexpr take_iterator &operator ++() {
    ++i;
    ++d;
    return *this;
  }
  value_type *operator ++(int);
  constexpr take_iterator operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }
};
template <class I1, class I2>
constexpr bool operator ==(const take_iterator<I1> &x,
                           const take_iterator<I2> &y)
  requires (is_same_v<itr_dft<I1>, itr_dft<I2>>
            && can_apply_equal_to<const I1 &, const I2 &>) {
  return x.i == y.i || x.d == y.d;
}
struct fo_take_itr {
  template <class I>
  constexpr take_iterator<I> operator ()(I i, itr_dft<I> n) const {
    return {i, n};
  }
};
inline constexpr fo_take_itr take_itr{};

template <class R>
class take_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  R r = R{};
  rng_dft<R> d = rng_dft<R>{};

public:
  take_range() = delete;
  take_range() requires is_default_constructible_v<R> = default;
  ~take_range() = default;
  take_range(const take_range &) = default;
  take_range &operator =(const take_range &) = default;
  take_range(take_range &&) = default;
  take_range &operator =(take_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(take_range &x, take_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR> && default_swappable<take_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.d, y.d);
  }

  constexpr take_range(R rr, rng_dft<R> dd)
    : r(forward<R>(rr)), d(dd) {}

  constexpr auto begin() {
    if constexpr (is_rrng<R>) {
      return begin(r);
    }
    else {
      return take_iterator<rng_itr<R>>(begin(r), 0);
    }
  }
  constexpr auto end() {
    if constexpr (is_rrng<R>) {
      return next(begin(r), d, end(r));
    }
    else {
      return take_iterator<rng_itr<R>>(end(r), d);
    }
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    if constexpr (is_rrng<const R>) {
      return begin(r);
    }
    else {
      return take_iterator<rng_itr<const R>>(begin(r), 0);
    }
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    if constexpr (is_rrng<const R>) {
      return next(begin(r), d, end(r));
    }
    else {
      return take_iterator<rng_itr<const R>>(end(r), d);
    }
  }

  constexpr bool empty() {
    return empty(r) || d == 0;
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r) || d == 0;
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return min_value(d, ssize(r));
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    return min_value(d, ssize(r));
  }  
};
template <class R>
struct is_range_reference<take_range<R>>
  : bool_constant<is_range_reference<R>::value> {};
struct fo_take_rng {
  template <class R>
  constexpr auto operator ()(R &&r, rng_dft<R> d) const requires is_rng<R> {
    return take_range<rng_forward_t<R>>(forward<R>(r), d);
  }
};
inline constexpr fo_take_rng take_rng{};

}

// drop(_plus)
// drop_range
namespace re {

struct fo_drop_plus {
  template <class R, class OI>
  constexpr pair<rng_itr<R>, OI>
  operator ()(R &&r, rng_dft<R> d, OI o) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (; d != 0; --d) {
      if (it != ed)
        ++it;
      else
        return {it, o};
    }
    for (; it != ed; ++it) {
      *o = *it;
      ++o;
    }
    return {it, o};
  }
};
inline constexpr fo_drop_plus drop_plus{};
struct fo_drop {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, rng_dft<R> d, OI o) const {
    return drop_plus(r, d, o).second;
  }
};
inline constexpr fo_drop drop{};

template <class R>
class drop_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using cache_t = conditional_t<is_rng_ref<R>,
                                simple_wrapper<rng_itr<R>>,
                                non_propagating_cache<rng_itr<R>>>;

  R r = R{};
  rng_dft<R> d = rng_dft<R>{};
  cache_t c = cache_t{};

  constexpr void init() {
    if constexpr (is_rng_ref<R>) {
      *c = next(begin(r), d, end(r));
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<R>) {
      if (c.empty())
        c.emplace(next(begin(r), d, end(r)));
    }
  }

public:
  drop_range() = delete;
  constexpr drop_range() requires is_default_constructible_v<R> {
    init();
  }
  ~drop_range() = default;
  drop_range(const drop_range &) = default;
  drop_range &operator =(const drop_range &) = default;
  drop_range(drop_range &&) = default;
  drop_range &operator =(drop_range &&) = default;
  template <class RR>
  friend constexpr void swap(drop_range &x, drop_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR> && default_swappable<drop_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.d, y.d);
    adl_swap(x.c, y.c);
  }

  constexpr explicit drop_range(R rr, rng_dft<R> dd)
    : r(forward<R>(rr)), d(dd), c() {
    init();
  }

  constexpr rng_itr<R> begin() {
    delayed_init();
    return *c;
  }
  constexpr rng_itr<R> end() {
    return end(r);
  }
  constexpr rng_itr<add_const_t<R>> begin() const
    requires is_rng_ref<add_const_t<R>> {
    return *c;
  }
  constexpr rng_itr<add_const_t<R>> end() const
    requires is_rng_ref<add_const_t<R>> {
    return end(r);
  }

  constexpr bool empty() {
    delayed_init();
    return *c == end(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<R>> {
    return *c == end(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    const auto sz = ssize(r);
    return sz - min_value(d, sz);
  }
  constexpr rng_szt<R> size() const
    requires (is_rng_ref<add_const_t<R>> && rng_is_sized<add_const_t<R>>) {
    const auto sz = ssize(r);
    return sz - min_value(d, sz);
  }
};
template <class R>
struct is_range_reference<drop_range<R>>
  : bool_constant<is_range_reference<R>::value> {};
struct fo_drop_rng {
  template <class R>
  constexpr auto operator ()(R &&r, rng_dft<R> d) const requires is_rng<R> {
    return drop_range<rng_forward_t<R>>(forward<R>(r), d);
  }
};
inline constexpr fo_drop_rng drop_rng{};

}

// take_while(_plus)
// take_while_range
namespace re {

struct fo_take_while_plus {
  template <class R, class F, class OI>
  constexpr pair<rng_itr<R>, OI> operator ()(R &&r, F eq, OI o) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (;;) {
      if (it != ed && eq(*it)) {
        *o = *it;
        ++o;
      }
      else
        break;
      ++it;
    }
    return {it, o};
  }
};
inline constexpr fo_take_while_plus take_while_plus{};
struct fo_take_while {
  template <class R, class F, class OI>
  constexpr OI operator ()(R &&r, F eq, OI o) const {
    return take_while_plus(r, eq, o).second;
  }
};
inline constexpr fo_take_while take_while{};

template <class I, class F>
class take_while_iterator {
  template <class, class>
  friend class take_while_iterator;

  I i = I{};
  I ed = I{};
  semiregular_function<F> f = semiregular_function<F>{};

public:
  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using difference_type = itr_dft<I>;
  using pointer = I;
  using iterator_category = conditional_t
    <is_fitr<I>, forward_iterator_tag, itr_ctg<I>>;

  take_while_iterator() = default;
  ~take_while_iterator() = default;
  take_while_iterator(const take_while_iterator &) = default;
  take_while_iterator &operator =(const take_while_iterator &) = default;
  take_while_iterator(take_while_iterator &&) = default;
  take_while_iterator &operator =(take_while_iterator &&) = default;
  friend constexpr void swap(take_while_iterator &x,
                             take_while_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
    adl_swap(x.ed, y.ed);
    adl_swap(x.f, y.f);
  }

  constexpr take_while_iterator(I ii, I ed2, F ff) : i(ii), ed(ed2), f(ff) {}
  constexpr I base() const {
    return i;
  }
  constexpr F fn() const {
    return f.base();
  }

  template <class I2, class F2>
  constexpr take_while_iterator(const take_while_iterator<I2, F2> &x)
    requires (!is_same_v<I, I2> && is_convertible_v<const I2 &, I>
              && is_convertible_v<const F2 &, F>)
    : i(x.i), ed(x.ed), f(x.f) {}

  template <class I1, class I2, class FF>
  friend constexpr bool operator ==(const take_while_iterator<I1, FF> &x,
                                    const take_while_iterator<I2, FF> &y)
    requires (is_same_v<itr_dft<I1>, itr_dft<I2>>
              && can_apply_equal_to<const I1 &, const I2 &>);

  constexpr reference operator *() const {
    return *i;
  }
  constexpr I operator ->() const
    requires can_apply_to_address<const I &> {
    return i;
  }
  constexpr take_while_iterator &operator ++() {
    ++i;
    return *this;
  }
  value_type *operator ++(int);
  constexpr take_while_iterator operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }
};
template <class I1, class I2, class FF>
constexpr bool operator ==(const take_while_iterator<I1, FF> &x,
                           const take_while_iterator<I2, FF> &y)
  requires (is_same_v<itr_dft<I1>, itr_dft<I2>>
            && can_apply_equal_to<const I1 &, const I2 &>) {
  if (x.i == y.i)
    return true;
  else {
    if (y.i == x.ed)
      return !x.f(*x.i);
    else if (x.i == x.ed)
      return !x.f(*y.i);
    else
      return false;
  }
}
struct fo_take_while_itr {
  template <class I, class F>
  constexpr take_while_iterator<I, F> operator ()(I i, I ed, F f) const {
    return {i, ed, f};
  }
};
inline constexpr fo_take_while_itr take_while_itr{};

template <class R, class F>
class take_while_range : range_fns {
  friend struct fo_take_while_rng;

  R r = R{};
  copyable_wrapper<F> f = copyable_wrapper<F>{};

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  take_while_range() = delete;
  take_while_range()
    requires (is_default_constructible_v<R> && is_default_constructible_v<F>)
    = default;
  ~take_while_range() = default;
  take_while_range(const take_while_range &) = default;
  take_while_range &operator =(const take_while_range &) = default;
  take_while_range(take_while_range &&) = default;
  take_while_range &operator =(take_while_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(take_while_range &x, take_while_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR>
              && default_swappable<take_while_range<RR, F>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.f, y.f);
  }

  constexpr explicit take_while_range(R rr, F ff) : r(forward<R>(rr)), f(ff) {}

  constexpr auto begin() {
    if constexpr (is_default_constructible_v<F>) {
      return take_while_iterator<rng_itr<R>, F>(begin(r), end(r), *f);
    }
    else {
      return take_while_iterator<rng_itr<R>, const F *>(begin(r), end(r),
                                                        addressof(*f));
    }
  }
  constexpr auto end() {
    if constexpr (is_default_constructible_v<F>) {
      return take_while_iterator<rng_itr<R>, F>(end(r), end(r), *f);
    }
    else {
      return take_while_iterator<rng_itr<R>, const F *>(end(r), end(r),
                                                        addressof(*f));
    }
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    if constexpr (is_default_constructible_v<F>) {
      return take_while_iterator<rng_itr<const R>, F>(begin(r), end(r), *f);
    }
    else {
      return take_while_iterator<rng_itr<const R>, const F *>(begin(r), end(r),
                                                              addressof(*f));
    }
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    if constexpr (is_default_constructible_v<F>) {
      return take_while_iterator<rng_itr<const R>, F>(end(r), end(r), *f);
    }
    else {
      return take_while_iterator<rng_itr<const R>, const F *>(end(r), end(r),
                                                              addressof(*f));
    }
  }

  constexpr bool empty() {
    return empty(r) || !(*f)(*begin(r));
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r) || !(*f)(*begin(r));
  }
};
template <class R, class F>
struct is_range_reference<take_while_range<R, F>>
  : bool_constant<is_range_reference<R>::value
                  && is_default_constructible_v<F>> {};
struct fo_take_while_rng {
  template <class R, class F>
  constexpr auto operator ()(R &&r, F f) const {
    return take_while_range<rng_forward_t<R>, F>(forward<R>(r), f);
  }
};
inline constexpr fo_take_while_rng take_while_rng{};

}

// drop_while(_plus)
// drop_while_range
namespace re {

struct fo_drop_while_plus {
  template <class R, class F, class OI>
  constexpr pair<rng_itr<R>, OI> operator ()(R &&r, F eq, OI o) const {
    const auto ed = end(r);
    auto it = begin(r);
    while (it != ed && eq(*it))
      ++it;
    for (; it != ed; ++it) {
      *o = *it;
      ++o;
    }
    return {it, o};
  }
};
inline constexpr fo_drop_while_plus drop_while_plus{};
struct fo_drop_while {
  template <class R, class F, class OI>
  constexpr OI operator ()(R &&r, F eq, OI o) const {
    return drop_while_plus(r, eq, o).second;
  }
};
inline constexpr fo_drop_while drop_while{};

template <class R, class F>
class drop_while_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using cache_t = conditional_t<is_rng_ref<R>,
                                simple_wrapper<rng_itr<R>>,
                                non_propagating_cache<rng_itr<R>>>;

  R r = R{};
  copyable_wrapper<F> f = copyable_wrapper<F>{};
  cache_t c = cache_t{};

  constexpr void init() {
    if constexpr (is_rng_ref<R>) {
      *c = find_if_not(r, *f);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<R>) {
      if (c.empty())
        c.emplace(find_if_not(r, *f));
    }
  }

public:
  drop_while_range() = delete;
  constexpr drop_while_range()
    requires (is_default_constructible_v<R>
              && is_default_constructible_v<F>) {
    init();
  }
  ~drop_while_range() = default;
  drop_while_range(const drop_while_range &) = default;
  drop_while_range &operator =(const drop_while_range &) = default;
  drop_while_range(drop_while_range &&) = default;
  drop_while_range &operator =(drop_while_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(drop_while_range &x, drop_while_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR>
              && default_swappable<drop_while_range<RR, F>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.f, y.f);
    adl_swap(x.c, y.c);
  }

  constexpr explicit drop_while_range(R rr, F ff)
    : r(forward<R>(rr)), f(ff), c() {
    init();
  }

  constexpr rng_itr<R> begin() {
    delayed_init();
    return *c;
  }
  constexpr rng_itr<R> end() {
    return end(r);
  }
  constexpr rng_itr<add_const_t<R>> begin() const
    requires is_rng_ref<add_const_t<R>> {
    return *c;
  }
  constexpr rng_itr<add_const_t<R>> end() const
    requires is_rng_ref<add_const_t<R>> {
    return end(r);
  }

  constexpr bool empty() {
    delayed_init();
    return *c == end(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<R>> {
    return *c == end(r);
  }

  constexpr rng_szt<R> size() requires (is_rng<R> && rng_is_counted<R>) {
    delayed_init();
    return end(r) - *c;
  }
  constexpr rng_szt<R> size() const
    requires (is_rng_ref<add_const_t<R>>
              && rng_is_counted<add_const_t<R>>) {
    return end(r) - *c;
  }
};
template <class R, class F>
struct is_range_reference<drop_while_range<R, F>>
  : bool_constant<is_range_reference<R>::value> {};
struct fo_drop_while_rng {
  template <class R, class F>
  constexpr auto operator ()(R &&r, F f) const {
    return drop_while_range<rng_forward_t<R>, F>(forward<R>(r), f);
  }
};
inline constexpr fo_drop_while_rng drop_while_rng{};

}

// join
// join_range
namespace re {

struct fo_join {
  template <class R, class O>
  constexpr O operator ()(R &&r, O o) const requires is_rng<R> {
    for (auto &i : iters(r))
      for (auto &j : iters(*i)) {
        *o = *j;
        ++o;
      }
    return o;
  }
};
inline constexpr fo_join join{};

template <class II>
class join_iterator {
  template <class>
  friend class join_iterator;

  using this_t = join_iterator;

  using inner_iter_t = rng_itr<itr_ref<II>>;

  II it = II{};
  inner_iter_t i = inner_iter_t{};
  II ed = II{};

public:
  using value_type = itr_vt<inner_iter_t>;
  using difference_type = common_type_t<itr_dft<II>, itr_dft<inner_iter_t>>;
  using pointer = inner_iter_t;
  using reference = itr_ref<inner_iter_t>;
  using iterator_category
    = conditional_t<is_bitr<II> && is_bitr<inner_iter_t>,
                    bidirectional_iterator_tag,
                    conditional_t<is_fitr<II> && is_fitr<inner_iter_t>,
                                  forward_iterator_tag,
                                  input_iterator_tag>>;

  join_iterator() = default;
  ~join_iterator() = default;
  join_iterator(const join_iterator &) = default;
  join_iterator &operator =(const join_iterator &) = default;
  join_iterator(join_iterator &&) = default;
  join_iterator &operator =(join_iterator &&) = default;
  friend constexpr void swap(join_iterator &x, join_iterator &y)
    noexcept(is_nothrow_swappable_v<II>
             && is_nothrow_swappable_v<inner_iter_t>) {
    adl_swap(x.it, y.it);
    adl_swap(x.i, y.i);
    adl_swap(x.ed, y.ed);
  }

  constexpr join_iterator(II x, inner_iter_t y, II z) : it(x), i(y), ed(z) {}
  constexpr inner_iter_t base() const {
    return i;
  }
  constexpr II outer_iter() const {
    return it;
  }

  template <class X>
  constexpr join_iterator(const join_iterator<X> &x)
    requires (!is_same_v<X, II> && is_convertible_v<const X &, II>
              && is_convertible_v
              <const typename join_iterator<X>::inner_iter_t &, inner_iter_t>)
    : it(x.it), i(x.i), ed(x.ed) {}

  constexpr reference operator *() const {
    return *i;
  }
  constexpr pointer operator ->() const
    requires can_apply_to_address<inner_iter_t> {
    return i;
  }
  constexpr this_t &operator ++() {
    ++i;
    while (i == end(*it)) {
      ++it;
      if (it == ed) {
        i = inner_iter_t{};
        break;
      }
      i = begin(*it);
    }
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int)
    requires is_fitr_ctg<iterator_category> {
    return iter_post_increment(*this);
  }
  template <class A, class B>
  friend constexpr auto operator ==(const join_iterator<A> &x,
                                    const join_iterator<B> &y)
    ->decltype(x.base() == y.base())
    requires can_apply_equal_to<const A &, const B &>;

  constexpr this_t &operator --()
    requires is_bitr_ctg<iterator_category> {
    if (it == ed) {
      --it;
      i = end(*it);
    }
    while (i == begin(*it)) {
      --it;
      i = end(*it);
    }
    --i;
    return *this;
  }
  constexpr this_t operator --(int)
    requires is_bitr_ctg<iterator_category> {
    return iter_post_decrement(*this);
  }
};
template <class A, class B>
constexpr auto operator ==(const join_iterator<A> &x,
                           const join_iterator<B> &y)
  ->decltype(x.base() == y.base())
  requires can_apply_equal_to<const A &, const B &> {
  return x.it == y.it && x.i == y.i;
}
struct fo_join_itr {
  template <class II>
  constexpr auto operator ()(II it, rng_itr<itr_ref<II>> i, II ed) const
    requires is_rng_ref<itr_ref<II>> {
    return join_iterator<II>(it, i, ed);
  }
};
inline constexpr fo_join_itr join_itr{};

template <class R>
class join_range;
template <class R>
requires is_rng_ref<rng_ref<R>>
class join_range<R> : range_fns {
  using this_t = join_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;

  using iter_t = rng_itr<R>;
  using inner_iter_t = rng_itr<itr_ref<iter_t>>;

  using const_iter_t = rng_itr<const R>;
  using const_inner_iter_t = rng_itr<itr_ref<const_iter_t>>;

  using cache_data_t = pair<iter_t, inner_iter_t>;
  using cache_t = conditional_t<is_rng_ref<R>,
                                simple_wrapper<cache_data_t>,
                                non_propagating_cache<cache_data_t>>;

  R r = R{};
  cache_t c = cache_t{};

  constexpr void init(iter_t &it, inner_iter_t &inner_it) {
    const auto ed = end(r);
    for (it = begin(r); it != ed; ++it)
      if (!empty(*it)) {
        inner_it = begin(*it);
        return;
      }
    inner_it = inner_iter_t{};
  }
  constexpr void init() {
    if constexpr (is_rng_ref<R>) {
      init(c->first, c->second);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<R>) {
      if (c.empty()) {
        iter_t it{};
        inner_iter_t inner_it{};
        init(it, inner_it);
        c.emplace(it, inner_it);
      }
    }
  }

public:
  join_range() = delete;
  constexpr join_range() requires is_default_constructible_v<R> {
    init();
  }
  ~join_range() = default;
  join_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  join_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  template <class RR = R>
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<typename join_range<RR>::cache_t>)
    requires is_swappable_v<RR> && default_swappable<join_range<RR>> {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr explicit join_range(R rr) : r(forward<R>(rr)) {
    init();
  }
    
  constexpr auto begin() {
    delayed_init();
    return join_iterator<iter_t>(c->first, c->second, end(r));
  }
  constexpr auto end() {
    const auto ed = end(r);
    return join_iterator<iter_t>(ed, inner_iter_t{}, ed);
  }
  constexpr auto begin() const requires is_rng_ref<add_const_t<R>> {
    return join_iterator<const_iter_t>(c->first, c->second, end(r));
  }
  constexpr auto end() const requires is_rng_ref<add_const_t<R>> {
    const auto ed = end(r);
    return join_iterator<const_iter_t>(ed, const_inner_iter_t{}, ed);
  }

  constexpr bool empty() {
    delayed_init();
    return c->first == end(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<R>> {
    return c->first == end(r);
  }
};
template <class R>
struct is_range_reference<join_range<R>>
  : bool_constant<is_range_reference<R>::value
                  && is_range_reference<rng_ref<R>>::value> {};
struct fo_join_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires is_rng<R> {
    return join_range<rng_forward_t<R>>(forward<R>(r));
  }
  template <class A, class B>
  constexpr auto operator ()(A x, B y) const requires is_itr<A> {
    return operator ()(rng(x, y));
  }
};
inline constexpr fo_join_rng join_rng{};

namespace inner {

template <class C>
class join_input_iterator {
  using this_t = join_input_iterator;

  using iter_t = typename C::iterator_type;
  using inner_iter_t = rng_itr<itr_ref<iter_t>>;

  C *p = nullptr;

public:
  using value_type = itr_vt<inner_iter_t>;
  using reference = itr_ref<inner_iter_t>;
  using difference_type = common_type_t<itr_dft<iter_t>, itr_dft<inner_iter_t>>;
  using pointer = inner_iter_t;
  using iterator_category = input_iterator_tag;

  join_input_iterator() = default;
  ~join_input_iterator() = default;
  join_input_iterator(const join_input_iterator &) = default;
  join_input_iterator &operator =(const join_input_iterator &) = default;
  join_input_iterator(join_input_iterator &&) = default;
  join_input_iterator &operator =(join_input_iterator &&) = default;
  friend constexpr void swap(join_input_iterator &x,
                             join_input_iterator &y) noexcept {
    adl_swap(x.p, y.p);
  }

  constexpr explicit join_input_iterator(C *pp) : p(pp) {}
  constexpr auto base() const {
    return p->i;
  }
  constexpr auto outer_iter() const {
    return p->it;
  }

  constexpr reference operator *() const {
    return *p->i;
  }
  constexpr pointer operator ->() const
    requires can_apply_to_address<inner_iter_t> {
    return p->i;
  }
  constexpr this_t &operator ++() {
    auto &it = p->it;
    auto &i = p->i;
    auto &it_ref = p->it_ref;
    auto &ed = p->ed;
    ++i;
    while (i == end(*it_ref)) {
      ++it;
      if (it == ed) {
        i = inner_iter_t{};
        p = nullptr;
        break;
      }
      it_ref = *it;
      i = begin(*it_ref);
    }
    return *this;
  }
  this_t operator ++(int);
  friend constexpr bool operator ==(const join_input_iterator &x,
                                    const join_input_iterator &y) {
    return x.p == y.p;
  }
};

}
template <class R>
requires (!is_rng_ref<rng_ref<R>>)
class join_range<R> : range_fns {
  using this_t = join_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;

  using iter_t = rng_itr<R>;
  using inner_iter_t = rng_itr<itr_ref<iter_t>>;

  using const_iter_t = rng_itr<const R>;
  using const_inner_iter_t = rng_itr<itr_ref<const_iter_t>>;

  struct cache_data_t {
    using iterator_type = iter_t;
    iter_t it;
    inner_iter_t i;
    optional<rng_ref<R>> it_ref;
    iter_t ed;

    cache_data_t() = delete;
    ~cache_data_t() = default;
    cache_data_t(const cache_data_t &) = delete;
    cache_data_t &operator =(const cache_data_t &) = delete;
    cache_data_t(cache_data_t &&) = delete;
    cache_data_t &operator =(cache_data_t &&) = delete;

    cache_data_t(iter_t it2, iter_t ed2) : it(it2), i{}, ed(ed2) {
      for (; it != ed; ++it)
        if (decltype(auto) tmp = *it; !empty(tmp)) {
          it_ref = forward<decltype(tmp)>(tmp);
          i = begin(*it_ref);
          break;
        }
    }
  };
  using cache_t = non_propagating_cache<cache_data_t>;

  R r = R{};
  cache_t c = cache_t{};

  constexpr void delayed_init() {
    if (c.empty())
      c.emplace(begin(r), end(r));
  }

public:
  join_range() = default;
  ~join_range() = default;
  join_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  join_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  template <class RR = R>
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<typename join_range<RR>::cache_t>)
    requires (is_swappable_v<RR> && default_swappable<join_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr explicit join_range(R rr) : r(forward<R>(rr)) {}

  constexpr auto begin() {
    delayed_init();
    return inner::join_input_iterator
      <typename cache_t::value_type>(c->it == c->ed ? nullptr : addressof(*c));
  }
  constexpr auto end() {
    delayed_init();
    return inner::join_input_iterator<typename cache_t::value_type>(nullptr);
  }
  constexpr bool empty() {
    delayed_init();
    return c->it == c->ed;
  }
};

}

// join_with
// join_with_range
namespace re {

struct fo_join_with {
  template <class R, class R2, class O>
  constexpr O operator ()(R &&r, R2 &&r2, O o) const
    requires is_rng<R> && is_rng<R2> {
    const auto ed = end(r);
    auto it = begin(r);
    if (it != ed)
      for (;;) {
        copy(*it, o);
        ++it;
        if (it == ed)
          break;
        copy(r2, o);
      }
    return o;
  }
};
inline constexpr fo_join_with join_with{};

template <class II, class FI>
class join_with_iterator {
  template <class, class>
  friend class join_with_iterator;

  using this_t = join_with_iterator;

  using inner_iter_t = rng_itr<itr_ref<II>>;

  II it;
  inner_iter_t i;
  II ed;
  FI op2;
  FI it2;
  FI ed2;

public:
  using value_type = common_type_t<itr_vt<inner_iter_t>, itr_vt<FI>>;
  using difference_type = common_type_t<itr_dft<II>,
                                        itr_dft<inner_iter_t>,
                                        itr_dft<FI>>;
  using pointer = void;
  using reference = common_reference_t<itr_ref<inner_iter_t>, itr_ref<FI>>;
  using iterator_category
    = conditional_t<is_bitr<II> && is_bitr<inner_iter_t> && is_bitr<FI>,
                    bidirectional_iterator_tag,
                    conditional_t<is_fitr<II>
                                  && is_fitr<inner_iter_t>
                                  && is_fitr<FI>,
                                  forward_iterator_tag,
                                  input_iterator_tag>>;

  join_with_iterator() = default;
  ~join_with_iterator() = default;
  join_with_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  join_with_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<II>
             && is_nothrow_swappable_v<inner_iter_t>
             && is_nothrow_swappable_v<FI>) {
    adl_swap(x.it, y.it);
    adl_swap(x.i, y.i);
    adl_swap(x.ed, y.ed);
    adl_swap(x.op2, y.op2);
    adl_swap(x.it2, y.it2);
    adl_swap(x.ed2, y.ed2);
  }

  constexpr join_with_iterator(II a, inner_iter_t b, II c, FI d, FI e, FI f)
    : it(a), i(b), ed(c), op2(d), it2(e), ed2(f) {}

  template <class II2, class FI2>
  constexpr join_with_iterator(const join_with_iterator<II2, FI2> &x)
    requires (!(is_same_v<II, II2> && is_same_v<FI, FI2>)
              && is_convertible_v<const II2 &, II>
              && is_convertible_v<const rng_itr<itr_ref<II2>> &, inner_iter_t>
              && is_convertible_v<const FI2 &, FI>)
    : it(x.it), i(x.i), ed(x.ed)
    , op2(x.op2), it2(x.it2), ed2(x.ed2) {}

  constexpr reference operator *() const {
    if (it2 == ed2)
      return *i;
    else
      return *it2;
  }
  void operator ->() const = delete;
  constexpr this_t &operator ++() {
    if (it2 == ed2) {
      ++i;
      if (i == end(*it)) {
        if (op2 != ed2) {
          ++it;
          if (it == ed)
            i = inner_iter_t{};
          else {
            i = begin(*it);
            it2 = op2;
          }
        }
        else {
          for (;;) {
            ++it;
            if (it == ed) {
              i = inner_iter_t{};
              break;
            }
            i = begin(*it);
            if (i != end(*it))
              break;
          }
        }
      }
    }
    else {
      ++it2;
      if (it2 == ed2) {
        if (i == end(*it)) {
          ++it;
          if (it == ed)
            i = inner_iter_t{};
          else {
            i = begin(*it);
            it2 = op2;
          }
        }
      }
    }
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int)
    requires is_fitr_ctg<iterator_category> {
    return iter_post_increment(*this);
  }
  template <class I1, class I2, class I3, class I4>
  friend constexpr bool operator ==(const join_with_iterator<I1, I2> &x,
                                    const join_with_iterator<I3, I4> &y)
    requires (can_apply_equal_to<const I1 &, const I3 &>
              && can_apply_equal_to
              <const rng_itr<itr_ref<I1>> &, const rng_itr<itr_ref<I3>> &>
              && can_apply_equal_to<const I2 &, const I4 &>);

  constexpr this_t &operator --()
    requires is_bitr_ctg<iterator_category> {
    if (it2 == ed2) {
      if (it == ed) {
        --it;
        if (!empty(*it))
          i = before_end(*it);
        else {
          i = begin(*it);
          if (op2 != ed2)
            it2 = prev(ed2);
          else
            for (;;) {
              --it;
              if (!empty(*it)) {
                i = before_end(*it);
                break;
              }
            }
        }
      }
      else {
        if (i != begin(*it))
          --i;
        else {
          if (op2 != ed2)
            it2 = prev(ed2);
          else {
            for (;;) {
              --it;
              if (!empty(*it)) {
                i = before_end(*it);
                break;
              }
            }
          }
        }
      }
    }
    else {
      if (op2 != it2)
        --it2;
      else {
        --it;
        i = end(*it);
        if (i != begin(*it)) {
          --i;
          it2 = ed2;
        }
        else
          it2 = prev(ed2);
      }
    }
    return *this;
  }
  constexpr this_t operator --(int)
    requires is_bitr_ctg<iterator_category> {
    return iter_post_decrement(*this);
  }
};
template <class I1, class I2, class I3, class I4>
constexpr bool operator ==(const join_with_iterator<I1, I2> &x,
                           const join_with_iterator<I3, I4> &y)
  requires (can_apply_equal_to<const I1 &, const I3 &>
            && can_apply_equal_to
            <const rng_itr<itr_ref<I1>> &, const rng_itr<itr_ref<I3>> &>
            && can_apply_equal_to<const I2 &, const I4 &>) {
  return x.it == y.it && x.i == y.i && x.it2 == y.it2;
}
struct fo_join_with_itr {
  template <class II, class FI>
  constexpr auto operator ()(II a, rng_itr<itr_ref<II>> b, II c,
                             FI d, FI e, FI f) const {
    return join_with_iterator<II, FI>(a, b, c, d, e, f);
  }
};
inline constexpr fo_join_with_itr join_with_itr{};

template <class R, class R2>
class join_with_range;
template <class R, class R2>
requires is_rng_ref<R> && is_rng_ref<rng_ref<R>> && is_rng_ref<R2>
class join_with_range<R, R2> : range_fns {
  using iter_t = rng_itr<R>;
  using inner_iter_t = rng_itr<itr_ref<iter_t>>;
  using iter2_t = rng_itr<R2>;

  using const_iter_t = rng_itr<const R>;
  using const_inner_iter_t = rng_itr<itr_ref<const_iter_t>>;
  using const_iter2_t = rng_itr<const R2>;

  R r = R{};
  R2 r2 = R2{};
  iter_t it = iter_t{};
  inner_iter_t inner_it = inner_iter_t{};
  iter2_t it2 = iter2_t{};

  constexpr void init() {
    const auto ed = end(r);
    if (empty(r2)) {
      for (it = begin(r); it != ed; ++it) {
        if (!empty(*it)) {
          inner_it = begin(*it);
          it2 = end(r2);
          return;
        }
      }
      inner_it = inner_iter_t{};
      it2 = end(r2);
    }
    else {
      if (it = begin(r); it != ed) {
        if (!empty(*it)) {
          inner_it = begin(*it);
          it2 = end(r2);
        }
        else {
          ++it;
          if (it != ed) {
            inner_it = begin(*it);
            it2 = begin(r2);
          }
          else {
            inner_it = inner_iter_t{};
            it2 = end(r2);
          }
        }
      }
      else {
        inner_it = inner_iter_t{};
        it2 = end(r2);
      }
    }
  }

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

public:
  join_with_range() = delete;
  join_with_range()
    requires (is_default_constructible_v<R>
              && is_default_constructible_v<R2>) {
    init();
  }
  join_with_range(const join_with_range &) = default;
  join_with_range &operator =(const join_with_range &) = default;
  join_with_range(join_with_range &&) = default;
  join_with_range &operator =(join_with_range &&) = default;
  constexpr join_with_range &operator =(const join_with_range &x)
    requires (is_copy_assignable_v<R> && is_copy_assignable_v<R2>
              && !is_reference_v<R> && !is_reference_v<R2>
              && (!is_nothrow_copy_assignable_v<R>
                  || !is_nothrow_copy_assignable_v<R2>)) {
    copy_and_swap(x, *this);
    return *this;
  }
  template <class RR = R, class RR2 = R2>
  friend constexpr void swap(join_with_range &x, join_with_range &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<RR2>
             && is_nothrow_swappable_v
             <typename join_with_range<RR, RR2>::iter_t>
             && is_nothrow_swappable_v
             <typename join_with_range<RR, RR2>::inner_iter_t>
             && is_nothrow_swappable_v
             <typename join_with_range<RR, RR2>::iter2_t>)
    requires (is_swappable_v<RR> && is_swappable_v<RR2>
              && default_swappable<join_with_range<RR, RR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.r2, y.r2);
    adl_swap(x.it, y.it);
    adl_swap(x.inner_it, y.inner_it);
    adl_swap(x.it2, y.it2);
  }

  constexpr join_with_range(R a, R2 b)
    : r(forward<R>(a)), r2(forward<R2>(b)) {
    init();
  }

  constexpr join_with_iterator<iter_t, iter2_t> begin() {
    return join_with_iterator<iter_t, iter2_t>(it, inner_it, end(r),
                                               begin(r2), it2, end(r2));
  }
  constexpr join_with_iterator<iter_t, iter2_t> end() {
    return join_with_iterator<iter_t, iter2_t>(end(r), inner_iter_t{}, end(r),
                                               begin(r2), end(r2), end(r2));
  }

  constexpr join_with_iterator<const_iter_t, const_iter2_t> begin() const
    requires (is_rng<add_const_t<R>> && is_rng<add_const_t<R2>>) {
    return join_with_iterator<const_iter_t, const_iter2_t>
      (it, inner_it, end(r), begin(r2), it2, end(r2));
  }
  constexpr join_with_iterator<const_iter_t, const_iter2_t> end() const
    requires (is_rng<add_const_t<R>> && is_rng<add_const_t<R2>>) {
    return join_with_iterator<const_iter_t, const_iter2_t>
      (end(r), inner_iter_t{}, end(r), begin(r2), end(r2), end(r2));
  }

  constexpr bool empty() {
    return it == end(r);
  }
  constexpr bool empty() const
    requires (is_rng<add_const_t<R>> && is_rng<add_const_t<R2>>) {
    return it == end(r);
  }
};
template <class R, class R2>
struct is_range_reference<join_with_range<R, R2>>
  : bool_constant<is_rng_ref<R> && is_rng_ref<rng_ref<R>> && is_rng_ref<R2>> {};

namespace inner {

template <class R>
class join_with_input_iterator {
  using this_t = join_with_input_iterator;

  R *p = nullptr;

  using r_t = typename R::r_t;
  using r2_t = typename R::r2_t;
  using inner_iter_t = rng_itr<rng_ref<r_t>>;

public:
  using value_type = common_type_t<rng_vt<rng_ref<r_t>>, rng_vt<r2_t>>;
  using difference_type = common_type_t<rng_dft<r_t>, rng_dft<rng_ref<r_t>>,
                                        rng_dft<r2_t>>;
  using pointer = void;
  using reference = common_reference_t<rng_ref<rng_ref<r_t>>, rng_ref<r2_t>>;
  using iterator_category = input_iterator_tag;

  join_with_input_iterator() = default;
  ~join_with_input_iterator() = default;
  join_with_input_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  join_with_input_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y) noexcept {
    adl_swap(x.p, y.p);
  }

  constexpr explicit join_with_input_iterator(R *pp) : p(pp) {}

  constexpr reference operator *() const {
    if (p->c->it2 == end(p->r2))
      return *(p->c->inner_it);
    else
      return *(p->c->it2);
  }
  void operator ->() const = delete;
  constexpr this_t &operator ++() {
    auto &it = p->c->it;
    auto &i = p->c->inner_it;
    const auto ed = end(p->r);
    const auto op2 = begin(p->r2);
    auto &it2 = p->c->it2;
    const auto ed2 = end(p->r2);

    if (it2 == ed2) {
      ++i;
      if (i == end(*it)) {
        if (op2 != ed2) {
          ++it;
          if (it == ed) {
            i = inner_iter_t{};
            it2 = ed2;
            p = nullptr;
          }
          else {
            i = begin(*it);
            it2 = op2;
          }
        }
        else {
          for (;;) {
            ++it;
            if (it == ed) {
              i = inner_iter_t{};
              p = nullptr;
              break;
            }
            i = begin(*it);
            if (i != end(*it))
              break;
          }
        }
      }
    }
    else {
      ++it2;
      if (it2 == ed2) {
        if (i == end(*it)) {
          ++it;
          if (it == ed) {
            i = inner_iter_t{};
            it2 = ed2;
            p = nullptr;
          }
          else {
            i = begin(*it);
            it2 = op2;
          }
        }
      }
    }
    return *this;
  }
  value_type *operator ++(int);
  friend constexpr bool operator ==(const this_t &x, const this_t &y) {
    return x.p == y.p;
  }
};

}
template <class R, class R2>
requires (!(is_rng_ref<R> && is_rng_ref<rng_ref<R>> && is_rng_ref<R2>))
class join_with_range<R, R2> : range_fns {
  using this_t = join_with_range;

  using r_t = R;
  using r2_t = R2;

  template <class>
  friend class inner::join_with_input_iterator;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using iter_t = rng_itr<R>;
  using inner_iter_t = rng_itr<itr_ref<iter_t>>;
  using iter2_t = rng_itr<R2>;

  using const_iter_t = rng_itr<const R>;
  using const_inner_iter_t = rng_itr<itr_ref<const_iter_t>>;
  using const_iter2_t = rng_itr<const R2>;

  R r = R{};
  R2 r2 = R2{};
  struct cache_data_t {
    iter_t it;
    inner_iter_t inner_it;
    iter2_t it2;
  };
  using cache_t = non_propagating_cache<cache_data_t>;
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &tmp) {
    auto &it = tmp.it;
    auto &inner_it = tmp.inner_it;
    auto &it2 = tmp.it2;

    const auto ed = end(r);
    if (empty(r2)) {
      for (it = begin(r); it != ed; ++it) {
        if (!empty(*it)) {
          inner_it = begin(*it);
          it2 = end(r2);
          return;
        }
      }
      inner_it = inner_iter_t{};
      it2 = end(r2);
    }
    else {
      if (it = begin(r); it != ed) {
        if (!empty(*it)) {
          inner_it = begin(*it);
          it2 = end(r2);
        }
        else {
          ++it;
          if (it != ed) {
            inner_it = begin(*it);
            it2 = begin(r2);
          }
          else {
            inner_it = inner_iter_t{};
            it2 = end(r2);
          }
        }
      }
      else {
        inner_it = inner_iter_t{};
        it2 = end(r2);
      }
    }
  }
  constexpr void delayed_init() {
    if (c.empty()) {
      cache_data_t tmp{};
      init_impl(tmp);
      c.emplace(tmp);
    }
  }

public:
  join_with_range() = delete;
  join_with_range()
    requires (is_default_constructible_v<R>
              && is_default_constructible_v<R2>) = default;
  join_with_range(const join_with_range &) = default;
  join_with_range &operator =(const join_with_range &) = default;
  join_with_range(join_with_range &&) = default;
  join_with_range &operator =(join_with_range &&) = default;
  constexpr join_with_range &operator =(const join_with_range &x)
    requires (is_copy_assignable_v<R> && is_copy_assignable_v<R2>
              && !is_reference_v<R> && !is_reference_v<R2>
              && (!is_nothrow_copy_assignable_v<R>
                  || !is_nothrow_copy_assignable_v<R2>)) {
    copy_and_swap(x, *this);
    return *this;
  }
  template <class RR = R, class RR2 = R2>
  friend constexpr void swap(join_with_range &x, join_with_range &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<RR2>)
    requires (is_swappable_v<RR> && is_swappable_v<RR2>
              && default_swappable<join_with_range<RR, RR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.r2, y.r2);
    adl_swap(x.c, y.c);
  }

  constexpr join_with_range(R a, R2 b) : r(forward<R>(a)), r2(forward<R2>(b)) {}

  constexpr auto begin() {
    delayed_init();
    return inner::join_with_input_iterator<this_t>(c->it == end(r)
                                                   ? nullptr : this);
  }
  constexpr auto end() {
    delayed_init();
    return inner::join_with_input_iterator<this_t>(nullptr);
  }

  constexpr bool empty() {
    delayed_init();
    return c->it == end(r);
  }
};

struct fo_join_with_rng {
  template <class R, class R2>
  constexpr auto operator ()(R &&r, R2 &&r2) const
    requires is_rng<R> && is_rng<R2> {
    return join_with_range<rng_forward_t<R>, rng_forward_t<R2>>
      (forward<R>(r), forward<R2>(r2));
  }
};
inline constexpr fo_join_with_rng join_with_rng{};

}

// adjacent
// adjacent_range
namespace re {

template <size_t N>
struct fo_adjacent_plus {
  template <class R, class OI>
  constexpr pair<rng_itr<R>, OI> operator ()(R &&r, OI o) const
    requires (is_frng<R> && N > 0) {
    using t = type_pack_apply_t<tuple, n_type_pack_t<N, rng_itr<R>>>;
    using ref_t = type_pack_apply_t<tuple, n_type_pack_t<N, rng_ref<R>>>;

    const auto ed = end(r);
    t x{};
    {
      size_t c = 0;
      auto it = begin(r);
      type_pack_for_each_until_false
        <make_index_sequence<N>>([&](auto tag) {
          using type = typename decltype(tag)::type;
          if (it == ed)
            return false;
          at<type::value>(x) = it;
          ++c;
          ++it;
          return true;
        });
      if (c != N)
        return {it, o};
    }

    for (;;) {
      const auto mk_f = []<size_t...IDS>(t xx, index_sequence<IDS...>) {
        return ref_t(*at<IDS>(xx)...);
      };
      *o = mk_f(x, make_index_sequence<N>{});
      ++o;

      type_pack_for_each<make_index_sequence<N - 1>>([&](auto tag) {
        at<decltype(tag)::type::value>(x)
          = at<decltype(tag)::type::value + 1>(x);
      });
      if (++at<N - 1>(x) == ed)
        return {at<N - 1>(x), o};
    }
  }
};
template <size_t N>
inline constexpr fo_adjacent_plus<N> adjacent_plus{};
template <size_t N>
struct fo_adjacent {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, OI o) const
    requires (is_frng<R> && N > 0) {
    return adjacent_plus<N>(r, o).second;
  }
};
template <size_t N>
inline constexpr fo_adjacent<N> adjacent{};

template <class FI, size_t N>
requires (N > 0)
class adjacent_iterator {
  template <class, size_t NN>
  requires (NN > 0)
  friend class adjacent_iterator;

  using t = type_pack_apply_t<tuple, n_type_pack_t<N, FI>>;

  t x = t{};

public:
  using value_type = type_pack_apply_t<tuple, n_type_pack_t<N, itr_vt<FI>>>;
  using reference = type_pack_apply_t<tuple, n_type_pack_t<N, itr_ref<FI>>>;
  using difference_type = itr_dft<FI>;
  using pointer = void;
  using iterator_category = conditional_t<is_citr<FI>,
                                          random_access_iterator_tag,
                                          itr_ctg<FI>>;

  adjacent_iterator() = default;
  ~adjacent_iterator() = default;
  adjacent_iterator(const adjacent_iterator &) = default;
  adjacent_iterator &operator =(const adjacent_iterator &) = default;
  adjacent_iterator(adjacent_iterator &&) = default;
  adjacent_iterator &operator =(adjacent_iterator &&) = default;
  friend constexpr void swap(adjacent_iterator &a, adjacent_iterator &b)
    noexcept(is_nothrow_swappable_v<t>) {
    adl_swap(a.x, b.x);
  }

  constexpr explicit adjacent_iterator(t xx) : x(xx) {}
  constexpr t base() const {
    return x;
  }

  template <class FI2>
  constexpr adjacent_iterator(const adjacent_iterator<FI2, N> &u)
    requires (!is_same_v<FI, FI2>
              && is_convertible_v<const FI2 &, FI>) : x(u.x) {}

  pointer operator ->() const = delete;
  constexpr reference operator *() const {
    constexpr auto f = []<size_t...IDS>(t xx, index_sequence<IDS...> ids) {
      return reference(*at<IDS>(xx)...);
    };
    return f(x, make_index_sequence<N>{});
  }
  constexpr adjacent_iterator &operator ++() {
    auto tmp = at<N - 1>(x);
    ++tmp;
    type_pack_for_each<make_index_sequence<N - 1>>([&](auto tag) {
      constexpr size_t n = decltype(tag)::type::value;
      at<n>(x) = at<n + 1>(x);
    });
    at<N - 1>(x) = tmp;
    return *this;
  }
  constexpr adjacent_iterator operator ++(int) {
    return iter_post_increment(*this);
  }
  template <class FI1, class FI2, size_t NN>
  friend constexpr bool operator ==(const adjacent_iterator<FI1, NN> &,
                                    const adjacent_iterator<FI2, NN> &)
    requires can_apply_equal_to<const FI1 &, const FI2 &>;

  constexpr adjacent_iterator &operator --() requires is_bitr<FI> {
    auto tmp = at<0>(x);
    --tmp;
    type_pack_for_each<make_index_sequence<N - 1>>([&](auto tag) {
      constexpr size_t n = N - 1 - decltype(tag)::type::value;
      at<n>(x) = at<n - 1>(x);
    });
    at<0>(x) = tmp;
    return *this;
  }
  constexpr adjacent_iterator operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr adjacent_iterator operator +(difference_type i) const
    requires is_ritr<FI> {
    auto tmp = x;
    type_pack_for_each<make_index_sequence<N>>([&tmp, i](auto tag) {
      at<decltype(tag)::type::value>(tmp) += i;
    });
    return adjacent_iterator(tmp);
  }
  constexpr adjacent_iterator operator -(difference_type i) const
    requires is_ritr<FI> {
    return *this + (-i);
  }
  constexpr adjacent_iterator &operator +=(difference_type i)
    requires is_ritr<FI> {
    return *this = *this + i;
  }
  constexpr adjacent_iterator &operator -=(difference_type i)
    requires is_ritr<FI> {
    return *this = *this + (-i);
  }
  constexpr reference operator [](difference_type i) const
    requires is_ritr<FI> {
    return *(*this + i);
  }

  template <class FI1, class FI2, size_t NN>
  friend constexpr auto operator -(const adjacent_iterator<FI1, NN> &,
                                   const adjacent_iterator<FI2, NN> &)
    ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
    requires itr_is_counted<FI1> && itr_is_counted<FI2>;
  template <class FI1, class FI2, size_t NN>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const adjacent_iterator<FI1, NN> &,
               const adjacent_iterator<FI2, NN> &);
};
template <class FI1, class FI2, size_t NN>
constexpr bool operator ==(const adjacent_iterator<FI1, NN> &a,
                           const adjacent_iterator<FI2, NN> &b)
  requires can_apply_equal_to<const FI1 &, const FI2 &> {
  return at<NN - 1>(a.x) == at<NN - 1>(b.x);
}
template <class FI1, size_t NN>
constexpr adjacent_iterator<FI1, NN>
operator +(itr_dft<FI1> i, const adjacent_iterator<FI1, NN> &a) {
  return a + i;
}
template <class FI1, class FI2, size_t NN>
constexpr auto operator -(const adjacent_iterator<FI1, NN> &a,
                          const adjacent_iterator<FI2, NN> &b)
  ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
  requires itr_is_counted<FI1> && itr_is_counted<FI2> {
  return at<NN - 1>(a.x) - at<NN - 1>(b.x);
}
template <class FI1, class FI2, size_t NN>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const adjacent_iterator<FI1, NN> &a,
             const adjacent_iterator<FI2, NN> &b) {
  return at<NN - 1>(a.x) <=> at<NN - 1>(b.x);
}
struct fo_adjacent_itr {
  template <class X, class...S>
  constexpr auto operator ()(X x, S...s) const
    requires (is_same_v<X, S> && ...) {
    return adjacent_iterator<X, sizeof...(s) + 1>(tuple(x, s...));
  }
};
inline constexpr fo_adjacent_itr adjacent_itr{};

template <class FR, size_t N>
requires (N > 0)
class adjacent_range : range_fns {
  using this_t = adjacent_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  FR r = FR{};

public:
  adjacent_range() = default;
  ~adjacent_range() = default;
  adjacent_range(const adjacent_range &) = default;
  adjacent_range &operator =(const adjacent_range &) = default;
  adjacent_range(adjacent_range &&) = default;
  adjacent_range &operator =(adjacent_range &&) = default;
  template <class FR2 = FR>
  friend constexpr void swap(adjacent_range &x, adjacent_range &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires (is_swappable_v<FR2>
              && default_swappable<adjacent_range<FR2, N>>) {
    adl_swap(x.r, y.r);
  }

  constexpr explicit adjacent_range(FR rr) : r(forward<FR>(rr)) {}

private:
  template <class R>
  static constexpr auto get_begin(R &rr) {
    using t = type_pack_apply_t<tuple, n_type_pack_t<N, rng_itr<R>>>;
    t x;

    const auto ed = end(rr);
    auto it = begin(rr);
    size_t c = 0;
    type_pack_for_each_until_false
      <make_index_sequence<N>>([&](auto tag) {
        using type = typename decltype(tag)::type;
        if (it == ed)
          return false;
        at<type::value>(x) = it;
        ++c;
        ++it;
        return true;
      });

    if (c != N) {
      it = begin(rr);
      type_pack_for_each<make_index_sequence<N>>([&](auto tag) {
        at<decltype(tag)::type::value>(x) = it;
      });
    }
    return x;
  }
  template <class R>
  static constexpr auto get_end(R &rr) {
    if constexpr (is_just_frng<R>) {
      using t = type_pack_apply_t<tuple, n_type_pack_t<N, rng_itr<R>>>;
      t x;
      auto it = end(rr);
      type_pack_for_each<make_index_sequence<N>>([&](auto tag) {
        at<decltype(tag)::type::value>(x) = it;
      });
      return x;
    }
    else {
      using t = type_pack_apply_t<tuple, n_type_pack_t<N, rng_itr<R>>>;
      t x;

      const auto op = begin(rr);
      auto it = end(rr);
      size_t c = 0;
      type_pack_for_each_until_false
        <make_index_sequence<N>>([&](auto tag) {
          using type = typename decltype(tag)::type;
          if (op == it)
            return false;
          at<N - 1 - type::value>(x) = it;
          ++c;
          --it;
          return true;
        });

      if (c != N) {
        it = op;
        type_pack_for_each<make_index_sequence<N>>([&](auto tag) {
          at<decltype(tag)::type::value>(x) = it;
        });
      }
      return x;
    }
  }
public:
  constexpr auto begin() {
    return adjacent_iterator<rng_itr<FR>, N>(get_begin(r));
  }
  constexpr auto end() {
    return adjacent_iterator<rng_itr<FR>, N>(get_end(r));
  }
  constexpr auto begin() const requires is_rng<add_const_t<FR>> {
    return adjacent_iterator<rng_itr<add_const_t<FR>>, N>(get_begin(r));
  }
  constexpr auto end() const requires is_rng<add_const_t<FR>> {
    return adjacent_iterator<rng_itr<add_const_t<FR>>, N>(get_end(r));
  }

private:
  template <class R>
  static constexpr bool empty_impl(R &rr) {
    if constexpr (rng_is_sized<R>) {
      return size(rr) < N;
    }
    else {
      rng_szt<R> c = 0;
      for (auto &it : iters(rr)) {
        ++c;
        if (c >= N)
          return false;
      }
      return true;
    }
  }
public:
  constexpr bool empty() {
    return empty_impl(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<FR>> {
    return empty_impl(r);
  }

  constexpr rng_szt<FR> size() requires rng_is_sized<FR> {
    return empty() ? 0u : (size(r) - N + 1u);
  }
  constexpr rng_szt<FR> size() const
    requires (is_rng<add_const_t<FR>> && rng_is_sized<add_const_t<FR>>) {
    return empty() ? 0u : (size(r) - N + 1u);
  }
};
template <class FR, size_t N>
struct is_range_reference<adjacent_range<FR, N>>
  : bool_constant<is_rng_ref<FR>> {};
template <size_t N>
struct fo_adjacent_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const requires (is_frng<R> && N > 0) {
    return adjacent_range<rng_forward_t<R>, N>(forward<R>(r));
  }
};
template <size_t N>
inline constexpr fo_adjacent_rng<N> adjacent_rng{};

template <size_t N>
struct fo_bind_adjacent_rng {
  template <class R, class F>
  constexpr auto operator ()(R &&r, F f) const
    requires (is_frng<R> && N > 0) {
    return bind_rng(adjacent_rng<N>(forward<R>(r)), bind(apply, f, _1));
  }
};
template <size_t N>
inline constexpr fo_bind_adjacent_rng<N> bind_adjacent_rng{};

}

// slide
// slide_range
namespace re {

struct fo_slide {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_dft<FR> d, OI o) const {
    const auto ed = end(r);
    auto it = begin(r);
    for (;;) {
      auto it2 = it;
      if (advance(it2, d, ed) == 0) {
        *o = rng(it, it2);
        ++o;
        ++it;
      }
      else
        return o;
    }
  }
};
inline constexpr fo_slide slide{};

template <class FI>
class slide_iterator {
  template <class>
  friend class slide_iterator;

  FI i = FI{};
  FI ii = FI{};

public:
  using value_type = iter_pair<FI>;
  using reference = iter_pair<FI>;
  using difference_type = itr_dft<FI>;
  using pointer = void;
  using iterator_category = conditional_t<is_citr<FI>,
                                          random_access_iterator_tag,
                                          itr_ctg<FI>>;

  slide_iterator() = default;
  ~slide_iterator() = default;
  slide_iterator(const slide_iterator &) = default;
  slide_iterator &operator =(const slide_iterator &) = default;
  slide_iterator(slide_iterator &&) = default;
  slide_iterator &operator =(slide_iterator &&) = default;
  friend constexpr void swap(slide_iterator &x, slide_iterator &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.i, y.i);
    adl_swap(x.ii, y.ii);
  }

  constexpr explicit slide_iterator(FI x, FI y) : i(x), ii(y) {}
  constexpr explicit slide_iterator(FI x, itr_dft<FI> y)
    : i(x), ii(next(x, y - 1)) {}
  constexpr auto base() const {
    return pair(i, ii);
  }

  template <class FI2>
  constexpr slide_iterator(const slide_iterator<FI2> &u)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>)
    : i(u.i), ii(u.ii) {}

  pointer operator ->() const = delete;
  constexpr reference operator *() const {
    return rng(i, next(ii));
  }
  constexpr slide_iterator &operator ++() {
    return *this = slide_iterator(next(i), next(ii));
  }
  constexpr slide_iterator operator ++(int) {
    return iter_post_increment(*this);
  }
  template <class FI1, class FI2>
  friend constexpr bool operator ==(const slide_iterator<FI1> &,
                                    const slide_iterator<FI2> &)
    requires can_apply_equal_to<const FI1 &, const FI2 &>;

  constexpr slide_iterator &operator --() requires is_bitr<FI> {
    return *this = slide_iterator(prev(i), prev(ii));
    return *this;
  }
  constexpr slide_iterator operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr slide_iterator operator +(difference_type n) const
    requires is_ritr<FI> {
    auto tmp = *this;
    tmp.i += n;
    tmp.ii += n;
    return tmp;
  }
  constexpr slide_iterator operator -(difference_type n) const
    requires is_ritr<FI> {
    return *this + (-n);
  }
  constexpr slide_iterator &operator +=(difference_type n)
    requires is_ritr<FI> {
    return *this = *this + n;
  }
  constexpr slide_iterator &operator -=(difference_type n)
    requires is_ritr<FI> {
    return *this = *this + (-n);
  }
  constexpr reference operator [](difference_type n) const
    requires is_ritr<FI> {
    return *(*this + n);
  }
  template <class FI1, class FI2>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const slide_iterator<FI1> &, const slide_iterator<FI2> &);
  template <class FI1, class FI2>
  friend constexpr auto operator -(const slide_iterator<FI1> &x,
                                   const slide_iterator<FI2> &y)
    ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
    requires itr_is_counted<FI1> && itr_is_counted<FI2>;
};
template <class FI1, class FI2>
constexpr bool operator ==(const slide_iterator<FI1> &x,
                           const slide_iterator<FI2> &y)
  requires can_apply_equal_to<const FI1 &, const FI2 &> {
  return x.ii == y.ii;
}
template <class FI1, class FI2>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const slide_iterator<FI1> &x, const slide_iterator<FI2> &y) {
  return synth_3way(x.ii, y.ii);
}
template <class FI1, class FI2>
constexpr auto operator -(const slide_iterator<FI1> &x,
                          const slide_iterator<FI2> &y)
  ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
  requires itr_is_counted<FI1> && itr_is_counted<FI2> {
  return x.ii - y.ii;
}
template <class FI>
constexpr slide_iterator<FI> operator +
(itr_dft<FI> n, const slide_iterator<FI> &x) requires is_ritr<FI> {
  return x + n;
}
struct fo_slide_itr {
  template <class FI>
  constexpr auto operator ()(FI i, FI ii) const requires is_fitr<FI> {
    return slide_iterator<FI>(i, ii);
  }
  template <class FI>
  constexpr auto operator ()(FI i, itr_dft<FI> d) const
    requires is_fitr<FI> {
    // requires: d > 0
    return slide_iterator<FI>(i, d);
  }
};
inline constexpr fo_slide_itr slide_itr{};

template <class FR>
class slide_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  using cache_data_t = conditional_t<is_brng<FR>,
                                     tuple<rng_itr<FR>, rng_itr<FR>>,
                                     tuple<rng_itr<FR>>>;
  using cache_t = conditional_t<is_rng_ref<FR>,
                                simple_wrapper<cache_data_t>,
                                non_propagating_cache<cache_data_t>>;

  FR r = FR{};
  rng_dft<FR> n = 1;
  cache_t c = cache_t{};

  constexpr void init(cache_data_t &x) {
    if constexpr (is_brng<FR>) {
      const auto op = begin(r);
      const auto ed = end(r);
      at<0>(x) = op;
      if (advance(at<0>(x), n - 1, ed) == 0)
        at<1>(x) = prev(ed, n - 1);
      else
        at<0>(x) = at<1>(x) = ed;
    }
    else {
      const auto op = begin(r);
      const auto ed = end(r);
      at<0>(x) = op;
      if (advance(at<0>(x), n - 1, ed) != 0)
        at<0>(x) = ed;
    }
  }
  constexpr void init() {
    if constexpr (is_rng_ref<FR>) {
      init(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<FR>) {
      if (c.empty()) {
        cache_data_t tmp{};
        init(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  slide_range() = delete;
  constexpr slide_range() requires is_default_constructible_v<FR> {
    init();
  }
  ~slide_range() = default;
  slide_range(const slide_range &) = default;
  slide_range &operator =(const slide_range &) = default;
  slide_range(slide_range &&) = default;
  slide_range &operator =(slide_range &&) = default;
  template <class FR2 = FR>
  friend constexpr void swap(slide_range &x, slide_range &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires is_swappable_v<FR2> && default_swappable<slide_range<FR2>> {
    adl_swap(x.r, y.r);
    adl_swap(x.n, y.n);
    adl_swap(x.c, y.c);
  }

  constexpr slide_range(FR rr, rng_dft<FR> nn) : r(rr), n(nn), c() {
    init();
  }

  constexpr auto begin() {
    delayed_init();
    return slide_iterator<rng_itr<FR>>(begin(r), at<0>(*c));
  }
  constexpr auto end() {
    if constexpr (is_just_frng<FR>) {
      delayed_init();
      return slide_iterator<rng_itr<FR>>(end(r), end(r));
    }
    else {
      delayed_init();
      return slide_iterator<rng_itr<FR>>(at<1>(*c), end(r));
    }
  }
  constexpr auto begin() const requires is_rng_ref<add_const_t<FR>> {
    return slide_iterator<rng_itr<add_const_t<FR>>>(begin(r), at<0>(*c));
  }
  constexpr auto end() const requires is_rng_ref<add_const_t<FR>> {
    if constexpr (is_just_frng<FR>) {
      return slide_iterator<rng_itr<add_const_t<FR>>>(end(r), end(r));
    }
    else {
      return slide_iterator<rng_itr<add_const_t<FR>>>(at<1>(*c), end(r));
    }
  }

  constexpr bool empty() {
    delayed_init();
    return at<0>(*c) == end(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<FR>> {
    return at<0>(*c) == end(r);
  }

  constexpr rng_szt<FR> size() requires itr_is_counted<rng_itr<FR>> {
    delayed_init();
    return end(r) - at<0>(*c);
  }
  constexpr rng_szt<FR> size() const
    requires (is_rng_ref<add_const_t<FR>>
              && itr_is_counted<rng_itr<add_const_t<FR>>>) {
    return end(r) - at<0>(*c);
  }
};
template <class FR>
struct is_range_reference<slide_range<FR>>
  : bool_constant<is_range_reference<FR>::value> {};
struct fo_slide_rng {
  template <class FR>
  constexpr auto operator ()(FR &&r, rng_dft<FR> n) const
    requires is_frng<FR> {
    // requires: n > 0
    return slide_range<rng_forward_t<FR>>(forward<FR>(r), n);
  }
};
inline constexpr fo_slide_rng slide_rng{};

}

// aligned_stride
// aligned_stride_range
namespace re {

struct fo_aligned_stride {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, rng_dft<R> d, OI o) const
    requires is_rng<R> {
    // requires: d > 0
    auto it = begin(r);
    const auto ed = end(r);
    for (; it != ed; advance(it, d)) {
      *o = *it;
      ++o;
    }
    return o;
  }
};
inline constexpr fo_aligned_stride aligned_stride{};

template <class I>
class aligned_stride_iterator {
  using this_t = aligned_stride_iterator;

  template <class>
  friend class aligned_stride_iterator;

  I i = I{};
  itr_dft<I> d = 1;

public:
  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category
    = conditional_t<is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>;

  aligned_stride_iterator() = default;
  ~aligned_stride_iterator() = default;
  aligned_stride_iterator(const aligned_stride_iterator &) = default;
  this_t &operator =(const aligned_stride_iterator &) = default;
  aligned_stride_iterator(aligned_stride_iterator &&) = default;
  this_t &operator =(aligned_stride_iterator &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
    adl_swap(x.d, y.d);
  }

  constexpr explicit aligned_stride_iterator(I ii, itr_dft<I> dd)
    : i(ii), d(dd) {}

  template <class I2>
  constexpr aligned_stride_iterator(const aligned_stride_iterator<I2> &x)
    requires (!is_same_v<I, I2> && is_convertible_v<const I2 &, I>)
    : i(x.i), d(x.d) {}

  constexpr I base() const {
    return i;
  }
  constexpr difference_type stride() const {
    return d;
  }

  constexpr pointer operator ->() const requires can_apply_to_address<I> {
    return i;
  }
  constexpr reference operator *() const {
    return *i;
  }
  constexpr this_t &operator ++() {
    i = next(i, d);
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<I> {
    i = next(i, -d);
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<I> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type n) const
    requires is_ritr<I> {
    return *copy(*this).operator +=(n);
  }
  constexpr this_t &operator +=(difference_type n)
    requires is_ritr<I> {
    i += n * d;
    return *this;
  }
  constexpr this_t &operator -=(difference_type n)
    requires is_ritr<I> {
    return operator +=(-n);
  }
  constexpr this_t operator +(difference_type n) const
    requires is_ritr<I> {
    return copy(*this).operator +=(n);
  }
  constexpr this_t operator -(difference_type n) const
    requires is_ritr<I> {
    return copy(*this).operator +=(-n);
  }
};
template <class I1, class I2>
constexpr auto operator ==(const aligned_stride_iterator<I1> &x,
                           const aligned_stride_iterator<I2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class I1>
constexpr aligned_stride_iterator<I1>
operator +(itr_dft<I1> n, const aligned_stride_iterator<I1> &x)
  requires is_ritr<I1> {
  return copy(x).operator +=(n);
}
template <class I1, class I2>
constexpr auto operator -(const aligned_stride_iterator<I1> &x,
                          const aligned_stride_iterator<I2> &y)
  ->decltype(x.base() - y.base())
  requires itr_is_counted<I1> && itr_is_counted<I2> {
  return (x.base() - y.base()) / x.stride();
}
template <class I1, class I2>
constexpr synth_3way_result<I1, I2>
operator <=>(const aligned_stride_iterator<I1> &x,
             const aligned_stride_iterator<I2> &y) {
  return synth_3way(x.base(), y.base());
}
struct fo_aligned_stride_itr {
  template <class I>
  constexpr auto operator ()(I i, itr_dft<I> n) const requires is_itr<I> {
    // requires: n > 0
    return aligned_stride_iterator<I>(i, n);
  }
};
inline constexpr fo_aligned_stride_itr aligned_stride_itr{};

template <class R>
class aligned_stride_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  R r = R{};
  rng_dft<R> d = 1;

public:
  aligned_stride_range() = delete;
  aligned_stride_range() requires is_default_constructible_v<R> = default;
  ~aligned_stride_range() = default;
  aligned_stride_range(const aligned_stride_range &) = default;
  aligned_stride_range &operator =(const aligned_stride_range &) = default;
  aligned_stride_range(aligned_stride_range &&) = default;
  aligned_stride_range &operator =(aligned_stride_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(aligned_stride_range &x, aligned_stride_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR>
              && default_swappable<aligned_stride_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.d, y.d);
  }

  constexpr explicit aligned_stride_range(R rr, rng_dft<R> dd)
    : r(forward<R>(rr)) , d(dd) {}

  constexpr auto begin() {
    return aligned_stride_itr(begin(r), d);
  }
  constexpr auto end() {
    return aligned_stride_itr(end(r), d);
  }
  constexpr auto begin() const requires is_rng<add_const_t<R>> {
    return aligned_stride_itr(begin(r), d);
  }
  constexpr auto end() const requires is_rng<add_const_t<R>> {
    return aligned_stride_itr(end(r), d);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<R>> {
    return empty(r);
  }
  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    return size(r) / to_unsigned(d);
  }
  constexpr rng_szt<R> size() const
    requires is_rng<add_const_t<R>> && rng_is_sized<add_const_t<R>> {
    return size(r) / to_unsigned(d);
  }
};
template <class R>
struct is_range_reference<aligned_stride_range<R>> : is_range_reference<R> {};
struct fo_aligned_stride_rng {
  template <class R>
  constexpr auto operator ()(R &&r, rng_dft<R> d) const requires is_rng<R> {
    return aligned_stride_range<rng_forward_t<R>>(forward<R>(r), d);
  }
};
inline constexpr fo_aligned_stride_rng aligned_stride_rng{};

}

// stride
// stride_range
namespace re {

struct fo_stride {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, rng_dft<R> d, OI o) const
    requires is_rng<R> {
    auto it = begin(r);
    const auto ed = end(r);
    for (; it != ed; advance(it, d, ed)) {
      *o = *it;
      ++o;
    }
    return o;
  }
};
inline constexpr fo_stride stride{};

template <class I>
class stride_iterator {
  using this_t = stride_iterator;

  template <class>
  friend class stride_iterator;

  I i = I{};
  I ed = I{};
  itr_dft<I> s = 1;
  itr_dft<I> missing = itr_dft<I>{};

public:
  using value_type = itr_vt<I>;
  using reference = itr_ref<I>;
  using pointer = I;
  using difference_type = itr_dft<I>;
  using iterator_category
    = conditional_t<is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>;

  stride_iterator() = default;
  ~stride_iterator() = default;
  stride_iterator(const stride_iterator &) = default;
  stride_iterator &operator =(const stride_iterator &) = default;
  stride_iterator(stride_iterator &&) = default;
  stride_iterator &operator =(stride_iterator &&) = default;
  friend constexpr void swap(stride_iterator &x, stride_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
    adl_swap(x.ed, y.ed);
    adl_swap(x.s, y.s);
    adl_swap(x.missing, y.missing);
  }

  constexpr explicit stride_iterator(I ii, I ed2, itr_dft<I> ss,
                                     itr_dft<I> m = 0)
    : i(ii), ed(ed2), s(ss), missing(m) {}

  template <class I2>
  constexpr stride_iterator(const stride_iterator<I2> &x)
    requires (!is_same_v<I, I2> && is_convertible_v<const I2 &, I>)
    : i(x.i), ed(x.ed), s(x.s), missing(x.missing) {}

  constexpr I base() const {
    return i;
  }
  constexpr itr_dft<I> stride() const {
    return s;
  }

  constexpr pointer operator ->() const requires can_apply_to_address<I> {
    return i;
  }
  constexpr reference operator *() const {
    return *i;
  }
  constexpr this_t &operator ++() {
    auto it = i;
    missing = advance(it, s, ed);
    i = it;
    return *this;
  }
  value_type *operator ++(int);
  constexpr this_t operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<I> {
    auto it = i;
    advance(it, missing - s);
    i = it;
    missing = 0;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<I> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type n) const
    requires is_ritr<I> {
    return *(copy(*this).operator +=(n));
  }
  constexpr this_t &operator +=(difference_type n)
    requires is_ritr<I> {
    auto it = i;
    if (n > 0)
      missing = advance(it, s * n, ed);
    else if (n < 0) {
      advance(it, s * n + missing);
      missing = 0;
    }
    i = it;
    return *this;
  }
  constexpr this_t &operator -=(difference_type n)
    requires is_ritr<I> {
    return operator +=(-n);
  }
  constexpr this_t operator +(difference_type n) const
    requires is_ritr<I> {
    return copy(*this).operator +=(n);
  }
  constexpr this_t operator -(difference_type n) const
    requires is_ritr<I> {
    return copy(*this).operator +=(-n);
  }

  template <class I1, class I2>
  friend constexpr auto operator -(const stride_iterator<I1> &x,
                                   const stride_iterator<I2> &y)
    ->decltype(x.base() - y.base())
    requires itr_is_counted<I1> && itr_is_counted<I2>;
};
template <class I1, class I2>
constexpr auto operator ==(const stride_iterator<I1> &x,
                           const stride_iterator<I2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class I1>
constexpr stride_iterator<I1>
operator +(itr_dft<I1> n, const stride_iterator<I1> &x)
  requires is_ritr<I1> {
  return copy(x).operator +=(n);
}
template <class I1, class I2>
constexpr auto operator -(const stride_iterator<I1> &x,
                          const stride_iterator<I2> &y)
  ->decltype(x.base() - y.base())
  requires itr_is_counted<I1> && itr_is_counted<I2> {
  if constexpr (is_bitr<I1> && is_bitr<I2>) {
    return ((x.base() - y.base()) + (x.missing - y.missing)) / x.stride();
  }
  else {
    return div_round_up(x.base() - y.base(), x.stride());
  }
}
template <class I1, class I2>
constexpr synth_3way_result<I1, I2>
operator <=>(const stride_iterator<I1> &x, const stride_iterator<I2> &y) {
  return synth_3way(x.base(), y.base());
}
struct fo_stride_itr {
  template <class I>
  constexpr auto operator ()(I i, I ed, itr_dft<I> n,
                             itr_dft<I> missing = 0) const
    requires is_itr<I> {
    return stride_iterator<I>(i, ed, n, missing);
  }
};
inline constexpr fo_stride_itr stride_itr{};

template <class R>
class stride_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  static constexpr bool store_missing = is_brng<R>;
  static constexpr bool delay_init = store_missing && !is_rng_ref<R>;
  using cache_t = conditional_t<delay_init,
                                non_propagating_cache<rng_dft<R>>,
                                simple_wrapper<rng_dft<R>>>;

  R r = R{};
  rng_dft<R> s = 1;
  cache_t c = cache_t{};
  
  constexpr void init() {
    if constexpr (!delay_init && store_missing) {
      *c = (s - ssize(r) % s) % s;
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init && store_missing) {
      if (c.empty())
        c.emplace((s - ssize(r) % s) % s);
    }
  }

public:
  stride_range() = delete;
  constexpr stride_range() requires is_default_constructible_v<R> {
    init();
  }
  ~stride_range() = default;
  stride_range(const stride_range &) = default;
  stride_range &operator =(const stride_range &) = default;
  stride_range(stride_range &&) = default;
  stride_range &operator =(stride_range &&) = default;
  template <class RR = R>
  friend constexpr void swap(stride_range &x, stride_range &y)
    noexcept(is_nothrow_swappable_v<RR>)
    requires (is_swappable_v<RR> && default_swappable<stride_range<RR>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.s, y.s);
    adl_swap(x.c, y.c);
  }

  constexpr stride_range(R rr, rng_dft<R> n) : r(forward<R>(rr)), s(n) {
    init();
  }

  constexpr auto begin() {
    return stride_itr(begin(r), end(r), s);
  }
  constexpr auto end() {
    delayed_init();
    return stride_itr(end(r), end(r), s, *c);
  }
  constexpr auto begin() const
    requires (is_rng<add_const_t<R>> && !delay_init) {
    return stride_itr(begin(r), end(r), s);
  }
  constexpr auto end() const
    requires (is_rng<add_const_t<R>> && !delay_init) {
    return stride_itr(end(r), end(r), s, *c);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const
    requires (is_rng<add_const_t<R>> && !delay_init) {
    return empty(r);
  }

  constexpr rng_szt<R> size() requires rng_is_sized<R> {
    if constexpr (itr_is_counted<rng_itr<R>> && store_missing) {
      return to_unsigned(end() - begin());
    }
    else {
      return to_unsigned(div_round_up(ssize(r), s));
    }
  }
  constexpr rng_szt<R> size() const
    requires (is_rng<add_const_t<R>> && !delay_init
              && rng_is_sized<add_const_t<R>>) {
    if constexpr (itr_is_counted<rng_itr<R>> && store_missing) {
      return to_unsigned(end() - begin());
    }
    else {
      return to_unsigned(div_round_up(ssize(r), s));
    }
  }
};
template <class R>
struct is_range_reference<stride_range<R>> : is_range_reference<R> {};
struct fo_stride_rng {
  template <class R>
  constexpr auto operator ()(R &&r, rng_dft<R> s) const requires is_rng<R> {
    return stride_range<rng_forward_t<R>>(forward<R>(r), s);
  }
};
inline constexpr fo_stride_rng stride_rng{};

}

// aligned_chunk
// aligned_chunk_range
namespace re {

struct fo_aligned_chunk {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_dft<FR> d, OI o) const
    requires is_frng<FR> {
    // requires: d > 0, size(r) % d == 0
    const auto ed = end(r);
    auto it = begin(r);
    while (it != ed) {
      auto it2 = it;
      advance(it2, d);
      *o = iter_pair(it, it2);
      ++o;
      it = it2;
    }
    return o;
  }
};
inline constexpr fo_aligned_chunk aligned_chunk{};

template <class FI>
class aligned_chunk_iterator {
  using this_t = aligned_chunk_iterator;

  template <class>
  friend class aligned_chunk_iterator;

  FI i = FI{};
  itr_dft<FI> s = 1;

public:
  using value_type = decltype(rng(declval<FI>(), declval<itr_dft<FI>>()));
  using reference = value_type;
  using pointer = void;
  using difference_type = itr_dft<FI>;
  using iterator_category
    = conditional_t<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>;

  aligned_chunk_iterator() = default;
  ~aligned_chunk_iterator() = default;
  aligned_chunk_iterator(const aligned_chunk_iterator &) = default;
  this_t &operator =(const aligned_chunk_iterator &) = default;
  aligned_chunk_iterator(aligned_chunk_iterator &&) = default;
  this_t &operator =(aligned_chunk_iterator &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.i, y.i);
    adl_swap(x.s, y.s);
  }

  constexpr explicit aligned_chunk_iterator(FI a, itr_dft<FI> b) : i(a), s(b) {}

  template <class FI2>
  constexpr aligned_chunk_iterator(const aligned_chunk_iterator<FI2> &x)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>)
    : i(x.i), s(x.s) {}

  constexpr FI base() const {
    return i;
  }
  constexpr itr_dft<FI> stride() const {
    return s;
  }

  void operator ->() const = delete;
  constexpr reference operator *() const {
    return rng(i, s);
  }
  constexpr this_t &operator ++() {
    i = next(i, s);
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }
  template <class FI1, class FI2>
  friend constexpr auto operator ==(const aligned_chunk_iterator<FI1> &x,
                                    const aligned_chunk_iterator<FI2> &y)
    ->decltype(x.i == y.i);

  constexpr this_t &operator --() requires is_bitr<FI> {
    i = next(i, -s);
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type n) const
    requires is_ritr<FI> {
    return *copy(*this).operator +=(n);
  }
  constexpr this_t &operator +=(difference_type n)
    requires is_ritr<FI> {
    i = next(i, n * s);
    return *this;
  }
  constexpr this_t &operator -=(difference_type n)
    requires is_ritr<FI> {
    return operator +=(-n);
  }
  constexpr this_t operator +(difference_type n) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(n);
  }
  constexpr this_t operator -(difference_type n) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(-n);
  }

  template <class FI1, class FI2>
  friend constexpr auto operator -(const aligned_chunk_iterator<FI1> &x,
                                   const aligned_chunk_iterator<FI2> &y)
    ->decltype(x.i - y.i)
    requires itr_is_counted<FI1> && itr_is_counted<FI2>;
  template <class FI1, class FI2>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const aligned_chunk_iterator<FI1> &,
               const aligned_chunk_iterator<FI2> &);
};
template <class FI1, class FI2>
constexpr auto operator ==(const aligned_chunk_iterator<FI1> &x,
                           const aligned_chunk_iterator<FI2> &y)
  ->decltype(x.i == y.i) {
  return x.i == y.i;
}
template <class FI1>
constexpr aligned_chunk_iterator<FI1>
operator +(itr_dft<FI1> n, const aligned_chunk_iterator<FI1> &x)
  requires is_ritr<FI1> {
  return copy(x).operator +=(n);
}
template <class FI1, class FI2>
constexpr auto operator -(const aligned_chunk_iterator<FI1> &x,
                          const aligned_chunk_iterator<FI2> &y)
  ->decltype(x.i - y.i)
  requires itr_is_counted<FI1> && itr_is_counted<FI2> {
  return (x.i - y.i) / x.s;
}
template <class FI1, class FI2>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const aligned_chunk_iterator<FI1> &x,
             const aligned_chunk_iterator<FI2> &y) {
  return synth_3way(x.i, y.i);
}
struct fo_aligned_chunk_itr {
  template <class FI>
  constexpr auto operator ()(FI i, itr_dft<FI> n) const
    requires is_fitr<FI> {
    // requires: n > 0
    return aligned_chunk_iterator<FI>(i, n);
  }
};
inline constexpr fo_aligned_chunk_itr aligned_chunk_itr{};

template <class FR>
class aligned_chunk_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  FR r = FR{};
  rng_dft<FR> d = 1;

public:
  aligned_chunk_range() = delete;
  aligned_chunk_range() requires is_default_constructible_v<FR> = default;
  ~aligned_chunk_range() = default;
  aligned_chunk_range(const aligned_chunk_range &) = default;
  aligned_chunk_range &operator =(const aligned_chunk_range &) = default;
  aligned_chunk_range(aligned_chunk_range &&) = default;
  aligned_chunk_range &operator =(aligned_chunk_range &&) = default;
  template <class FR2 = FR>
  friend constexpr void swap(aligned_chunk_range &x, aligned_chunk_range &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires (is_swappable_v<FR2>
              && default_swappable<aligned_chunk_range<FR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.d, y.d);
  }

  constexpr explicit aligned_chunk_range(FR rr, rng_dft<FR> dd)
    : r(forward<FR>(rr)) , d(dd) {}

  constexpr auto begin() {
    return aligned_chunk_itr(begin(r), d);
  }
  constexpr auto end() {
    return aligned_chunk_itr(end(r), d);
  }
  constexpr auto begin() const requires is_rng<add_const_t<FR>> {
    return aligned_chunk_itr(begin(r), d);
  }
  constexpr auto end() const requires is_rng<add_const_t<FR>> {
    return aligned_chunk_itr(end(r), d);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng<add_const_t<FR>> {
    return empty(r);
  }
  constexpr rng_szt<FR> size() requires rng_is_sized<FR> {
    return size(r) / to_unsigned(d);
  }
  constexpr rng_szt<FR> size() const
    requires is_rng<add_const_t<FR>> && rng_is_sized<add_const_t<FR>> {
    return size(r) / to_unsigned(d);
  }
};
template <class FR>
struct is_range_reference<aligned_chunk_range<FR>> : is_range_reference<FR> {};
struct fo_aligned_chunk_rng {
  template <class FR>
  constexpr auto operator ()(FR &&r, rng_dft<FR> d) const requires is_rng<FR> {
    return aligned_chunk_range<rng_forward_t<FR>>(forward<FR>(r), d);
  }
};
inline constexpr fo_aligned_chunk_rng aligned_chunk_rng{};

}

// chunk
// chunk_range
namespace re {

struct fo_chunk_plus {
  template <class FR, class OI>
  constexpr pair<rng_itr<FR>, OI>
  operator ()(FR &&r, rng_dft<FR> d, OI o) const {
    // requires: d > 0
    const auto ed = end(r);
    auto it = begin(r);
    for (;;) {
      auto it2 = it;
      if (advance(it2, d, ed) == 0) {
        *o = rng(it, it2);
        ++o;
        it = it2;
      }
      else {
        if (it != it2) {
          *o = rng(it, it2);
          ++o;
        }
        return {it2, o};
      }
    }
  }
};
inline constexpr fo_chunk_plus chunk_plus{};
struct fo_chunk {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_dft<FR> d, OI o) const {
    return chunk_plus(r, d, o).second;
  }
};
inline constexpr fo_chunk chunk{};

template <class FI>
class chunk_iterator {
  using this_t = chunk_iterator;

  template <class>
  friend class chunk_iterator;

  FI i = FI{};
  FI i2 = FI{};
  FI ed = FI{};
  itr_dft<FI> s = 1;
  itr_dft<FI> missing = itr_dft<FI>{};

public:
  using value_type = iter_pair<FI>;
  using reference = iter_pair<FI>;
  using difference_type = itr_dft<FI>;
  using pointer = void;
  using iterator_category = conditional_t<is_ritr<FI>,
                                          random_access_iterator_tag,
                                          itr_ctg<FI>>;

  chunk_iterator() = default;
  ~chunk_iterator() = default;
  chunk_iterator(const chunk_iterator &) = default;
  chunk_iterator &operator =(const chunk_iterator &) = default;
  chunk_iterator(chunk_iterator &&) = default;
  chunk_iterator &operator =(chunk_iterator &&) = default;
  friend constexpr void swap(chunk_iterator &x, chunk_iterator &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.i, y.i);
    adl_swap(x.i2, y.i2);
    adl_swap(x.ed, y.ed);
    adl_swap(x.s, y.s);
    adl_swap(x.missing, y.missing);
  }

  constexpr explicit chunk_iterator(FI x, FI y, FI z,
                                    itr_dft<FI> a, itr_dft<FI> b)
    : i(x), i2(y), ed(z), s(a), missing(b) {}
  constexpr auto base() const {
    return rng(i, i2);
  }
  constexpr auto stride() const {
    return s;
  }

  template <class FI2>
  constexpr chunk_iterator(const chunk_iterator<FI2> &u)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>)
    : i(u.i), i2(u.i2), ed(u.ed), s(u.s), missing(u.missing) {}

  pointer operator ->() const = delete;
  constexpr reference operator *() const {
    return rng(i, i2);
  }
  constexpr chunk_iterator &operator ++() {
    auto it = i2;
    auto it2 = it;
    auto tmp = advance(it2, s, ed);
    if (it != ed)
      missing = tmp;
    i = it;
    i2 = it2;
    return *this;
  }
  constexpr chunk_iterator operator ++(int) {
    return iter_post_increment(*this);
  }
  template <class FI1, class FI2>
  friend constexpr bool operator ==(const chunk_iterator<FI1> &,
                                    const chunk_iterator<FI2> &)
    requires can_apply_equal_to<const FI1 &, const FI2 &>;

  constexpr chunk_iterator &operator --() requires is_bitr<FI> {
    auto it = i;
    itr_dft<FI> m = (it != ed) ? 0 : missing;
    advance(it, m - s);
    i2 = i;
    i = it;
    missing = m;
    return *this;
  }
  constexpr chunk_iterator operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type n) const
    requires is_ritr<FI> {
    return *(copy(*this).operator +=(n));
  }
  constexpr this_t &operator +=(difference_type n) requires is_ritr<FI> {
    FI it = i;
    FI it2 = i2;
    itr_dft<FI> m = missing;
    if (n > 0) {
      const itr_dft<FI> mm = advance(it2, s * n, ed) % s;
      advance(it, s * n, ed);
      if (i2 != ed)
        m = mm;
    }
    else if (n < 0) {
      m = ((i == ed) ? m : 0);
      advance(it, m + s * n);
      it2 = next(it, s, ed);
      if (it2 != ed)
        m = 0;
    }
    i = it;
    i2 = it2;
    missing = m;
    return *this;
  }
  constexpr this_t &operator -=(difference_type n) requires is_ritr<FI> {
    return operator +=(-n);
  }
  constexpr this_t operator +(difference_type n) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(n);
  }
  constexpr this_t operator -(difference_type n) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(-n);
  }

  template <class FI1, class FI2>
  friend constexpr auto operator -(const chunk_iterator<FI1> &x,
                                   const chunk_iterator<FI2> &y)
    ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
    requires itr_is_counted<FI1> && itr_is_counted<FI2>;
  template <class FI1, class FI2>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const chunk_iterator<FI1> &, const chunk_iterator<FI2> &);
};
template <class FI1, class FI2>
constexpr bool operator ==(const chunk_iterator<FI1> &x,
                           const chunk_iterator<FI2> &y)
  requires can_apply_equal_to<const FI1 &, const FI2 &> {
  return x.i == y.i;
}
template <class FI>
constexpr chunk_iterator<FI>
operator +(itr_dft<FI> n, const chunk_iterator<FI> &x)
  requires is_ritr<FI> {
  return copy(x).operator +=(n);
}
template <class FI1, class FI2>
constexpr auto operator -(const chunk_iterator<FI1> &x,
                          const chunk_iterator<FI2> &y)
  ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
  requires itr_is_counted<FI1> && itr_is_counted<FI2> {
  return div_round_up(x.i - y.i, x.s);
}
template <class FI1, class FI2>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const chunk_iterator<FI1> &x, const chunk_iterator<FI2> &y) {
  return synth_3way(x.i, y.i);
}
struct fo_chunk_itr {
  template <class FI>
  constexpr auto operator ()(FI i, FI i2, FI ed,
                             itr_dft<FI> s, itr_dft<FI> missing) const
    requires is_fitr<FI> {
    return chunk_iterator<FI>(i, i2, ed, s, missing);
  }
};
inline constexpr fo_chunk_itr chunk_itr{};

template <class FR>
class chunk_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  struct cache_data_t {
    rng_itr<FR> i;
    rng_dft<FR> m1;
    rng_dft<FR> m2;
  };
  using cache_t = conditional_t<is_rng_ref<FR>,
                                simple_wrapper<cache_data_t>,
                                non_propagating_cache<cache_data_t>>;

  FR r = FR{};
  rng_dft<FR> s = 1;
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x) {
    x.i = begin(r);
    x.m1 = advance(x.i, s, end(r)) % s;
    x.m2 = (s - ssize(r) % s) % s;
  }
  constexpr void init() {
    if constexpr (is_rng_ref<FR>) {
      init_impl(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<FR>) {
      if (c.empty()) {
        cache_data_t tmp{};
        init_impl(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  chunk_range() = delete;
  constexpr chunk_range() requires is_default_constructible_v<FR> {
    init();
  }
  ~chunk_range() = default;
  chunk_range(const chunk_range &) = default;
  chunk_range &operator =(const chunk_range &) = default;
  chunk_range(chunk_range &&) = default;
  chunk_range &operator =(chunk_range &&) = default;
  template <class FR2 = FR>
  friend constexpr void swap(chunk_range &x, chunk_range &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires (is_swappable_v<FR2> && default_swappable<chunk_range<FR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.s, y.s);
    adl_swap(x.c, y.c);
  }

  constexpr chunk_range(FR rr, rng_dft<FR> n) : r(forward<FR>(rr)), s(n) {
    init();
  }

  constexpr auto begin() {
    delayed_init();
    return chunk_itr(begin(r), c->i, end(r), s, c->m1);
  }
  constexpr auto end() {
    delayed_init();
    const auto ed = end(r);
    return chunk_itr(ed, ed, ed, s, c->m2);
  }
  constexpr auto begin() const requires is_rng_ref<add_const_t<FR>> {
    return chunk_iterator<rng_itr<add_const_t<FR>>>
      (begin(r), c->i, end(r), s, c->m1);
  }
  constexpr auto end() const requires is_rng_ref<add_const_t<FR>> {
    const auto ed = end(r);
    return chunk_itr(ed, ed, ed, s, c->m2);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<FR>> {
    return empty(r);
  }

  constexpr rng_szt<FR> size() requires rng_is_sized<FR> {
    return to_unsigned(div_round_up(ssize(r), s));
  }
  constexpr rng_szt<FR> size() const
    requires is_rng_ref<add_const_t<FR>> && rng_is_sized<add_const_t<FR>> {
    return to_unsigned(div_round_up(ssize(r), s));
  }
};
template <class FR>
struct is_range_reference<chunk_range<FR>> : is_range_reference<FR> {};
struct fo_chunk_rng {
  template <class FR>
  constexpr auto operator ()(FR &&r, rng_dft<FR> n) const
    requires is_frng<FR> {
    return chunk_range<rng_forward_t<FR>>(forward<FR>(r), n);
  }
};
inline constexpr fo_chunk_rng chunk_rng{};

}

// chunk_by
// chunk_by_range
namespace re {

struct fo_chunk_by_plus {
  template <class FR, class F, class OI>
  constexpr pair<rng_itr<FR>, OI> operator ()(FR &&r, F f, OI o) const
    requires is_frng<FR> {
    const auto ed = end(r);
    auto it = begin(r);
    for (;;) {
      if (const auto tmp = adjacent_find(rng(it, ed), not_fn(f));
          tmp != ed) {
        const auto tmp2 = next(tmp);        
        *o = rng(it, tmp2);
        ++o;
        it = tmp2;
      }
      else {
        if (it != ed) {
          const auto tmp2 = next(it, ed);
          *o = rng(it, tmp2);
          ++o;
          return {tmp2, o};
        }
        else
          return {it, o};
      }
    }
  }
};
inline constexpr fo_chunk_by_plus chunk_by_plus{};
struct fo_chunk_by {
  template <class FR, class F, class OI>
  constexpr OI operator ()(FR &&r, F f, OI o) const requires is_frng<FR> {
    return chunk_by_plus(r, f, o).second;
  }
};
inline constexpr fo_chunk_by chunk_by{};

template <class FI, class F>
class chunk_by_iterator {
  FI i = FI{};
  FI i2 = FI{};
  FI op = FI{};
  FI ed = FI{};
  semiregular_function<F> f = semiregular_function<F>{};

public:
  using value_type = iter_pair<FI>;
  using reference = value_type;
  using difference_type = itr_dft<FI>;
  using pointer = void;
  using iterator_category = conditional_t<is_bitr<FI>,
                                          bidirectional_iterator_tag,
                                          forward_iterator_tag>;

  chunk_by_iterator() = default;
  ~chunk_by_iterator() = default;
  chunk_by_iterator(const chunk_by_iterator &) = default;
  chunk_by_iterator &operator =(const chunk_by_iterator &) = default;
  chunk_by_iterator(chunk_by_iterator &&) = default;
  chunk_by_iterator &operator =(chunk_by_iterator &&) = default;
  friend constexpr void swap(chunk_by_iterator &x, chunk_by_iterator &y)
    noexcept(is_nothrow_swappable_v<FI>
             && is_nothrow_swappable_v<semiregular_function<F>>) {
    adl_swap(x.i, y.i);
    adl_swap(x.i2, y.i2);
    adl_swap(x.op, y.op);
    adl_swap(x.ed, y.ed);
    adl_swap(x.f, y.f);
  }

  constexpr chunk_by_iterator(FI a, FI b, FI c, FI d, F e)
    : i(a), i2(b), op(c), ed(d), f(e) {}

  template <class FI2, class F2>
  constexpr chunk_by_iterator(const chunk_by_iterator<FI2, F2> &x)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>
              && is_convertible_v<const F2 &, F>)
    : i(x.i), i2(x.i2), op(x.op), ed(x.ed), f(x.f) {}

  constexpr value_type base() const {
    return rng(i, i2);
  }

  void operator ->() const = delete;
  constexpr reference operator *() const {
    return rng(i, i2);
  }
  constexpr chunk_by_iterator &operator ++() {
    auto it2 = adjacent_find(rng(i2, ed), not_fn(f));
    if (it2 != ed)
      ++it2;
    i = i2;
    i2 = it2;
    return *this;
  }
  constexpr chunk_by_iterator operator ++(int) {
    return iter_post_increment(*this);
  }

  constexpr chunk_by_iterator &operator --() requires is_bitr<FI> {
    auto r = rrng(irng(op, i));
    auto x = adjacent_find(r, [eq = f](auto b, auto a) {return !eq(*a, *b);});
    i2 = i;
    i = ((x != end(r)) ? *x : op);
    return *this;
  }
  constexpr chunk_by_iterator operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }
};
template <class FI1, class FI2, class F>
constexpr auto operator ==(const chunk_by_iterator<FI1, F> &x,
                           const chunk_by_iterator<FI2, F> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
struct fo_chunk_by_itr {
  template <class FI, class F>
  constexpr auto operator ()(FI i, FI i2, FI op, FI ed, F f) const {
    return chunk_by_iterator<FI, F>(i, i2, op, ed, f);
  }
};
inline constexpr fo_chunk_by_itr chunk_by_itr{};

template <class FR, class EQ>
class chunk_by_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  using cache_data_t = rng_itr<FR>;
  using cache_t = conditional_t<is_rng_ref<FR>,
                                simple_wrapper<cache_data_t>,
                                non_propagating_cache<cache_data_t>>;

  FR r = FR{};
  copyable_wrapper<EQ> eq = copyable_wrapper<EQ>{};
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x) {
    rng_itr<FR> it = adjacent_find(r, not_fn(*eq));
    if (it != end(r))
      ++it;
    x = it;
  }
  constexpr void init() {
    if constexpr (is_rng_ref<FR>) {
      init_impl(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<FR>) {
      if (c.empty()) {
        cache_data_t tmp{};
        init_impl(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  chunk_by_range() = delete;
  constexpr chunk_by_range() requires (is_default_constructible_v<FR>
                                       && is_default_constructible_v<EQ>) {
    init();
  }
  ~chunk_by_range() = default;
  chunk_by_range(const chunk_by_range &) = default;
  chunk_by_range &operator =(const chunk_by_range &) = default;
  chunk_by_range(chunk_by_range &&) = default;
  chunk_by_range &operator =(chunk_by_range &&) = default;
  template <class FR2 = FR>
  friend constexpr void swap(chunk_by_range &x, chunk_by_range &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires (is_swappable_v<FR2>
              && default_swappable<chunk_by_range<FR2, EQ>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.eq, y.eq);
    adl_swap(x.c, y.c);
  }

  constexpr chunk_by_range(FR rr, EQ f) : r(forward<FR>(rr)), eq(f) {
    init();
  }

  constexpr auto begin() {
    if constexpr (is_default_constructible_v<EQ>) {
      delayed_init();
      const auto op = begin(r);
      return chunk_by_itr(op, *c, op, end(r), *eq);
    }
    else {
      delayed_init();
      const auto op = begin(r);
      return chunk_by_itr(op, *c, op, end(r), addressof(as_const(*eq)));
    }
  }
  constexpr auto end() {
    if constexpr (is_default_constructible_v<EQ>) {
      delayed_init();
      const auto ed = end(r);
      return chunk_by_itr(ed, ed, begin(r), ed, *eq);
    }
    else {
      delayed_init();
      const auto ed = end(r);
      return chunk_by_itr(ed, ed, begin(r), ed, addressof(as_const(*eq)));
    }
  }
  constexpr auto begin() const requires is_rng_ref<add_const_t<FR>> {
    if constexpr (is_default_constructible_v<EQ>) {
      const auto op = begin(r);
      return chunk_by_itr(op, *c, op, end(r), *eq);
    }
    else {
      const auto op = begin(r);
      return chunk_by_itr(op, *c, op, end(r), addressof(as_const(*eq)));
    }
  }
  constexpr auto end() const requires is_rng_ref<add_const_t<FR>> {
    if constexpr (is_default_constructible_v<EQ>) {
      const auto ed = end(r);
      return chunk_by_itr(ed, ed, begin(r), ed, *eq);
    }
    else {
      const auto ed = end(r);
      return chunk_by_itr(ed, ed, begin(r), ed, addressof(as_const(*eq)));
    }
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires is_rng_ref<add_const_t<FR>> {
    return empty(r);
  }
};
template <class FR, class EQ>
struct is_range_reference<chunk_by_range<FR, EQ>>
  : bool_constant<is_range_reference<FR>::value
                  && is_default_constructible_v<EQ>> {};
struct fo_chunk_by_rng {
  template <class FR, class F>
  constexpr auto operator ()(FR &&r, F f) const requires is_frng<FR> {
    return chunk_by_range<rng_forward_t<FR>, F>(forward<FR>(r), f);
  }
};
inline constexpr fo_chunk_by_rng chunk_by_rng{};

}

// inner_cartesian_product
// inner_cartesian_product_range
namespace re {

struct fo_inner_cartesian_product_plus {
  template <class FR, class OI>
  constexpr pair<rng_itr<FR>, OI> operator ()(FR &&fr, OI o) const
    requires is_frng<FR> && is_frng<rng_ref<FR>> {
    if (empty(fr) || any_of(fr, empty))
      return {next(begin(fr), end(fr)), o};

    if (auto tmp = size(fr);
        tmp > to_unsigned(numeric_limits<make_signed_t<size_t>>::max()))
      throw length_error
        ("re::inner_cartesian_product(r, o): too big range size\n");

    unique_array<rng_itr<rng_ref<FR>>> v
      = make_array<rng_itr<rng_ref<FR>>>(static_cast<size_t>(size(fr)));
    copy(bind_rng(fr, begin), begin(v));
    *o = deref_rng(v);
    ++o;
    for (;;) {
      const auto ed = end(v);
      const auto ed2 = end(fr);
      auto it = begin(v);
      auto it2 = begin(fr);
      for (;;) {
        if (it == ed)
          return {it2, o};
        if (++*it == end(*it2))
          *it = begin(*it2);
        else {
          *o = deref_rng(v);
          ++o;
          break;
        }
        ++it;
        ++it2;
      }
    }
  }
};
inline constexpr fo_inner_cartesian_product_plus inner_cartesian_product_plus{};
struct fo_inner_cartesian_product {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&fr, OI o) const
    requires is_frng<FR> && is_frng<rng_ref<FR>> {
    return inner_cartesian_product_plus(fr, o).second;
  }
};
inline constexpr fo_inner_cartesian_product inner_cartesian_product{};

template <class>
class inner_cartesian_product_range;
namespace inner {

template <class R>
class inner_cartesian_product_iterator;
template <class FR>
class inner_cartesian_product_iterator<inner_cartesian_product_range<FR>> {
  using this_t = inner_cartesian_product_iterator;

  inner_cartesian_product_range<FR> *p = nullptr;

public:
  using value_type = decltype(deref_rng
                              (declval
                               <unique_array<rng_itr<rng_ref<FR>>> &>()));
  using reference = value_type;
  using pointer = void;
  using difference_type = ptrdiff_t;
  using iterator_category = input_iterator_tag;

  inner_cartesian_product_iterator() = default;
  ~inner_cartesian_product_iterator() = default;
  inner_cartesian_product_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  inner_cartesian_product_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend void swap(this_t &x, this_t &y) noexcept {
    adl_swap(x.p, y.p);
  }

  friend bool operator ==(const this_t &x, const this_t &y) noexcept {
    return x.p == y.p;
  }

  explicit inner_cartesian_product_iterator
  (inner_cartesian_product_range<FR> *pp) : p(pp) {}

  void operator ->() const = delete;
  reference operator *() const {
    return deref_rng(p->c->a);
  }
  this_t &operator ++() {
    const auto ed = end(p->c->a);
    const auto ed2 = end(p->r);
    auto it = begin(p->c->a);
    auto it2 = begin(p->r);
    for (;;) {
      if (it == ed) {
        p->c->y = false;
        p = nullptr;
        break;
      }
      if (++*it == end(*it2))
        *it = begin(*it2);
      else
        break;
      ++it;
      ++it2;
    }
    return *this;
  }
  this_t operator ++(int) {
    return iter_post_increment(*this);
  }
};

}
template <class FR>
class inner_cartesian_product_range : range_fns {
  using this_t = inner_cartesian_product_range;

  template <class>
  friend class inner::inner_cartesian_product_iterator;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  struct cache_data_t {
    bool y;
    unique_array<rng_itr<rng_ref<FR>>> a;
  };

  FR r = FR{};
  non_propagating_cache<cache_data_t> c = non_propagating_cache<cache_data_t>{};

  void delayed_init() {
    if (c.empty()) {
      cache_data_t tmp;
      tmp.y = !(empty(r) || any_of(r, re::empty));
      if (tmp.y) {
        if (auto tmp2 = size(r);
            tmp2 > to_unsigned(numeric_limits<make_signed_t<size_t>>::max()))
          throw length_error
            ("re::inner_cartesian_product_range::delayed_init(): "
             "too big range size\n");
        tmp.a = make_array<rng_itr<rng_ref<FR>>>(static_cast<size_t>(size(r)));
        copy(bind_rng(r, re::begin), begin(tmp.a));
      }
      c.emplace(move(tmp));
    }
  }

public:
  inner_cartesian_product_range() = delete;
  inner_cartesian_product_range() requires is_default_constructible_v<FR>
    = default;
  ~inner_cartesian_product_range() = default;
  inner_cartesian_product_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  inner_cartesian_product_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  template <class FR2 = FR>  
  friend void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FR2>)
    requires (is_swappable_v<FR2>
              && default_swappable<inner_cartesian_product_range<FR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  explicit inner_cartesian_product_range(FR rr) : r(forward<FR>(rr)) {}

  auto begin() {
    delayed_init();
    return inner::inner_cartesian_product_iterator<this_t>
      (c->y ? this : nullptr);
  }
  auto end() {
    return inner::inner_cartesian_product_iterator<this_t>(nullptr);
  }
  bool empty() {
    delayed_init();
    return !c->y;
  }
};
struct fo_inner_cartesian_product_rng {
  template <class FR>
  auto operator ()(FR &&r) const requires is_frng<FR> {
    return inner_cartesian_product_range<rng_forward_t<FR>>(forward<FR>(r));
  }
};
inline constexpr fo_inner_cartesian_product_rng inner_cartesian_product_rng{};

}

// cartesian_product
// cartesian_product_range
namespace re {

namespace inner::fns {

template <size_t N, class T>
constexpr bool cartesian_product_impl_any_empty(T &x) {
  if constexpr (N == 1) {
    return at<0>(x)[1] == at<0>(x)[2];
  }
  else {
    return at<N - 1>(x)[1] == at<N - 1>(x)[2]
      || inner::fns::cartesian_product_impl_any_empty<N - 1>(x);
  }
}

template <size_t N, class T>
constexpr bool cartesian_product_impl_plus_plus(T &x) {
  if constexpr (N == 1) {
    auto &z = at<0>(x);
    ++z[0];
    return z[0] != z[2];
  }
  else {
    auto &z = at<N - 1>(x);
    ++z[0];
    if (z[0] == z[2]) {
      z[0] = z[1];
      return inner::fns::cartesian_product_impl_plus_plus<N - 1>(x);
    }
    else
      return true;
  }
}

template <class T, class OI, size_t...IDS>
constexpr void cartesian_product_impl_assign0(T &x, OI &o,
                                              index_sequence<IDS...>) {
  *o = tuple<itr_ref<tuple_element_t<0, tuple_element_t<IDS, T>>>...>
    (*at<IDS>(x)[0]...);
  ++o;
}
template <size_t N, class T, class OI>
constexpr void cartesian_product_impl_assign(T &x, OI &o) {
  inner::fns::cartesian_product_impl_assign0(x, o, make_index_sequence<N>{});
}

template <class OI, class IR, class FR, class...S>
constexpr OI cartesian_product_impl0(OI o, IR &&r1, FR &&r2, S &&...s) {
  constexpr size_t sz = sizeof...(s) + 2u;
  tuple<array<rng_itr<IR>, 3>, array<rng_itr<FR>, 3>,
        array<rng_itr<S>, 3>...>
    x(array<rng_itr<IR>, 3>{begin(r1), begin(r1), end(r1)},
      array<rng_itr<FR>, 3>{begin(r2), begin(r2), end(r2)},
      array<rng_itr<S>, 3>{begin(s), begin(s), end(s)}...);
  if (cartesian_product_impl_any_empty<sz>(x))
    return o;
  for (;;) {
    cartesian_product_impl_assign<sz>(x, o);
    if (!cartesian_product_impl_plus_plus<sz>(x))
      break;
  }
  return o;
}
template <class OI, class T, size_t...IDS>
constexpr auto cartesian_product_impl1(OI o, T &&x,
                                       index_sequence<IDS...>) {
  return inner::fns::cartesian_product_impl0(o, at<IDS>(forward<T>(x))...);
}
template <size_t N, class T, size_t...IDS>
constexpr auto cartesian_product_impl2(T &&x) {
  return inner::fns::cartesian_product_impl1
    (at<N - 1>(forward<T>(x)), forward<T>(x), make_index_sequence<N - 1>{});
}
template <size_t N, class T>
constexpr auto cartesian_product_impl(T &&x) {
  return cartesian_product_impl2<tuple_size_v<remove_reference_t<T>>>
    (forward<T>(x));
}

}
struct fo_cartesian_product {
  template <class IR, class FR, class...S>
  constexpr auto operator ()(IR &&r1, FR &&r2, S &&...s) const {
    return inner::fns::cartesian_product_impl<sizeof...(S) + 2u>
      (tuple<IR &&, FR &&, S &&...>
       (forward<IR>(r1), forward<FR>(r2), forward<S>(s)...));
  }
  template <class IR, class OI>
  constexpr OI operator ()(IR &&r1, OI o) const {
    return copy(bind_rng(irng(r1),
                         bind(bind_tuple, bind(make_tuple, _1), deref)),
                o);
  }
};
inline constexpr fo_cartesian_product cartesian_product{};

namespace inner::fns {

template <size_t N, class T>
constexpr void cartesian_product_iterator_impl_pp_fitr(T &x) {
  if constexpr (N == 1) {
    auto &z = at<0>(x);
    ++z[0];
    if (z[0] == z[2]) {
      type_pack_for_each<make_index_sequence<tuple_size_v<T>>>
        ([&](auto tt) {
          constexpr size_t i = decltype(tt)::type::value;
          auto &zz = at<i>(x);
          zz[0] = zz[2];
        });
    }
  }
  else {
    auto &z = at<N - 1>(x);
    ++z[0];
    if (z[0] == z[2]) {
      z[0] = z[1];
      return inner::fns::cartesian_product_iterator_impl_pp_fitr<N - 1>(x);
    }
  }
}

template <size_t N, class T>
constexpr void cartesian_product_iterator_impl_pp(T &x) {
  if constexpr (N == 1) {
    auto &z = at<0>(x);
    ++z[0];
    if (z[0] == z[2]) {
      type_pack_for_each<make_index_sequence<tuple_size_v<T>>>
        ([&](auto tt) {
          constexpr size_t i = decltype(tt)::type::value;
          auto &zz = at<i>(x);
          zz[0] = prev(zz[2], 1, zz[1]);
        });
      auto &&zz = at<tuple_size_v<T> - 1u>(x);
      zz[0] = zz[2];
    }
  }
  else {
    auto &z = at<N - 1>(x);
    ++z[0];
    if (z[0] == z[2]) {
      z[0] = z[1];
      return inner::fns::cartesian_product_iterator_impl_pp<N - 1>(x);
    }
  }
}
template <size_t N, class T>
constexpr void cartesian_product_iterator_impl_mm(T &x) {
  if constexpr (N == 1) {
    auto &z = at<0>(x);
    --z[0];
  }
  else {
    auto &z = at<N - 1>(x);
    if (z[0] != z[1])
      --z[0];
    else {
      z[0] = prev(z[2]);
      return inner::fns::cartesian_product_iterator_impl_mm<N - 1>(x);
    }
  }
}

}
template <class IT, class...S>
class cartesian_product_iterator {
  using this_t = cartesian_product_iterator;

  tuple<array<IT, 3>, array<S, 3>...> a;

public:
  using value_type = tuple<itr_vt<IT>, itr_vt<S>...>;
  using reference = tuple<itr_ref<IT>, itr_ref<S>...>;
  using pointer = void;
  using difference_type = common_type_t<itr_dft<IT>, itr_dft<S>...>;
  using iterator_category = conditional_t
    <is_ritr<IT> && (is_ritr<S> && ...), random_access_iterator_tag,
     conditional_t
     <is_bitr<IT> && (is_bitr<S> && ...), bidirectional_iterator_tag,
      conditional_t
      <is_fitr<IT> && (is_fitr<S> && ...), forward_iterator_tag,
       input_iterator_tag>>>;

  cartesian_product_iterator() = default;
  ~cartesian_product_iterator() = default;
  cartesian_product_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  cartesian_product_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<tuple<array<IT, 3>, array<S, 3>...>>)
    requires is_swappable_v<tuple<array<IT, 3>, array<S, 3>...>> {
    adl_swap(x.a, y.a);
  }

  template <class IT1, class IT2, class...S1, class...S2>
  friend constexpr bool operator ==(const cartesian_product_iterator
                                    <IT1, S1...> &,
                                    const cartesian_product_iterator
                                    <IT2, S2...> &)
    requires (can_apply_equal_to<const IT1 &, const IT2 &>
              && (can_apply_equal_to<const S1 &, const S2 &> && ...));

  constexpr explicit cartesian_product_iterator
  (const tuple<array<IT, 3>, array<S, 3>...> &x) : a(x) {}

  constexpr auto base() const {
    return a;
  }

  template <class IT2, class...SS>
  constexpr cartesian_product_iterator
  (const cartesian_product_iterator<IT2, SS...> &x)
    requires (is_convertible_v<const IT2 &, IT2>
              && (is_convertible_v<const SS &, S> && ...)) : a(x.a) {}

  void operator ->() const = delete;
  constexpr reference operator *() const {
    return bind_tuple(a, [](auto &x)->decltype(auto) {return *x[0];});
  }
  constexpr this_t &operator ++() {
    if constexpr (is_same_v<iterator_category, forward_iterator_tag>
                  || is_same_v<iterator_category, input_iterator_tag>) {
      auto tmp = *this;
      inner::fns::cartesian_product_iterator_impl_pp_fitr
        <sizeof...(S) + 1u>(tmp.a);
      a = tmp.a;
      return *this;
    }
    else {
      auto tmp = *this;
      inner::fns::cartesian_product_iterator_impl_pp<sizeof...(S) + 1u>(tmp.a);
      a = tmp.a;
      return *this;
    }
  }
  constexpr this_t operator ++(int);
  constexpr this_t operator ++(int)
    requires is_fitr_ctg<iterator_category> {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --()
    requires is_bitr_ctg<iterator_category> {
    auto tmp = *this;
    inner::fns::cartesian_product_iterator_impl_mm<sizeof...(S) + 1u>(tmp.a);
    a = tmp.a;
    return *this;
  }
  constexpr this_t operator --(int)
    requires is_bitr_ctg<iterator_category> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type d) const
    requires is_ritr_ctg<iterator_category> {
    return *(operator +(d));
  }
  constexpr this_t &operator +=(difference_type d)
    requires is_ritr_ctg<iterator_category> {
    return operator =(operator +(d));
  }
  constexpr this_t &operator -=(difference_type d)
    requires is_ritr_ctg<iterator_category> {
    return operator =(operator +(-d));
  }
  constexpr this_t operator +(difference_type d) const
    requires is_ritr_ctg<iterator_category> {
    auto ret = *this;
    if (d == 0)
      return ret;
    difference_type dd = id() + d;
    assign_id(ret, dd);
    return ret;
  }
  constexpr this_t operator -(difference_type d) const
    requires is_ritr_ctg<iterator_category> {
    return operator +(-d);
  }
private:
  static constexpr bool try_end_id(this_t &x, difference_type k) {
    bool y = true;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        constexpr size_t i = decltype(tt)::type::value;
        if constexpr (i == 0 && tuple_size_v<decltype(x.a)> != 1) {
          auto &z = at<sizeof...(S)>(x.a);
          const auto tmp = (z[2] - z[1]);
          if (const auto d = tmp; k >= d) {
            z[0] = z[2];
            k -= d;
            k /= tmp;
          }
          else {
            z[0] = z[1] + (k % tmp);
            k /= tmp;
          }
          if (z[0] != z[2])
            y = false;
        }
        else if constexpr (i == 0 && tuple_size_v<decltype(x.a)> == 1) {
          auto &z = at<sizeof...(S)>(x.a);
          const auto tmp = (z[2] - z[1]);
          z[0] = z[1] + k;
          if (z[0] != z[2])
            y = false;
        }
        else {
          auto &z = at<sizeof...(S) - i>(x.a);
          const auto tmp = (z[2] - z[1]);
          z[0] = z[1] + (k % tmp);
          k /= tmp;
          if (z[0] != prev(z[2], 1, z[1]))
            y = false;
        }
      });
    return y;
  }
  static constexpr void assign_id(this_t &x, difference_type k) {
    if (!try_end_id(x, k))
      type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
        ([&](auto tt) {
          constexpr size_t i = decltype(tt)::type::value;
          auto &z = at<sizeof...(S) - i>(x.a);
          const auto tmp = (z[2] - z[1]);
          z[0] = z[1] + (k % tmp);
          k /= tmp;
        });
  }
  constexpr difference_type id() const {
    difference_type ret{};
    difference_type b = 1;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        constexpr size_t i = decltype(tt)::type::value;
        auto &z = at<sizeof...(S) - i>(a);
        ret += ((z[0] - z[1]) * b);
        b *= (z[2] - z[1]);
      });
    return ret;
  }
public:
  template <class IT1, class IT2, class...S1, class...S2>
  friend
  constexpr auto operator -(const cartesian_product_iterator<IT1, S1...> &,
                            const cartesian_product_iterator<IT2, S2...> &)
    requires (is_bitr<cartesian_product_iterator<IT1, S1...>>
              && is_bitr<cartesian_product_iterator<IT2, S2...>>
              && itr_is_counted<IT1> && itr_is_counted<IT2>
              && (itr_is_counted<S1> && ...)
              && (itr_is_counted<S2> && ...)
              && can_apply_minus<const IT1 &, const IT2 &>
              && (can_apply_minus<const S1 &, const S2 &> && ...));
  template <class IT1, class IT2, class...S1, class...S2>
  friend
  constexpr common_comparison_category_t<synth_3way_result<IT1, IT2>,
                                         synth_3way_result<S1, S2>...>
  operator <=>(const cartesian_product_iterator<IT1, S1...> &,
               const cartesian_product_iterator<IT2, S2...> &);
};
template <class IT1, class IT2, class...S1, class...S2>
constexpr bool operator ==(const cartesian_product_iterator<IT1, S1...> &x,
                           const cartesian_product_iterator<IT2, S2...> &y)
  requires (can_apply_equal_to<const IT1 &, const IT2 &>
            && (can_apply_equal_to<const S1 &, const S2 &> && ...)) {
  return bind_tuple(x.a, bind(ref, _1, 0)) == bind_tuple(y.a, bind(ref, _1, 0));
}
template <class IT1, class IT2, class...S1, class...S2>
constexpr auto operator -(const cartesian_product_iterator<IT1, S1...> &x,
                          const cartesian_product_iterator<IT2, S2...> &y)
  requires (is_bitr<cartesian_product_iterator<IT1, S1...>>
            && is_bitr<cartesian_product_iterator<IT2, S2...>>
            && itr_is_counted<IT1> && itr_is_counted<IT2>
            && (itr_is_counted<S1> && ...)
            && (itr_is_counted<S2> && ...)
            && can_apply_minus<const IT1 &, const IT2 &>
            && (can_apply_minus<const S1 &, const S2 &> && ...)) {
  return x.id() - y.id();
}
template <class IT1, class...S1>
constexpr auto operator +(typename cartesian_product_iterator<IT1, S1...>
                          ::difference_type d,
                          const cartesian_product_iterator<IT1, S1...> &it)
  requires is_ritr<cartesian_product_iterator<IT1, S1...>> {
  return it.operator +(d);
}
template <class IT1, class IT2, class...S1, class...S2>
constexpr common_comparison_category_t<synth_3way_result<IT1, IT2>,
                                       synth_3way_result<S1, S2>...>
operator <=>(const cartesian_product_iterator<IT1, S1...> &x,
             const cartesian_product_iterator<IT2, S2...> &y) {
  return bind_tuple(x.a, bind(ref, _1, 0))
    <=> bind_tuple(y.a, bind(ref, _1, 0));
}
struct fo_cartesian_product_itr {
  template <class IT, class...S>
  constexpr cartesian_product_iterator<IT, S...>
  operator ()(tuple<array<IT, 3>, array<S, 3>...> a) const {
    return cartesian_product_iterator<IT, S...>(a);
  }
};
inline constexpr fo_cartesian_product_itr cartesian_product_itr{};

template <class FR, class...S>
class cartesian_product_range : range_fns {
  using this_t = cartesian_product_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  tuple<FR, S...> t;

public:
  cartesian_product_range() = default;
  ~cartesian_product_range() = default;
  cartesian_product_range(const this_t &) = default;
  this_t &operator =(const this_t &) = delete;
  this_t &operator =(const this_t &)
    requires (is_copy_assignable_v<FR> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<FR> && ((!is_reference_v<S>) && ...)
              && (is_nothrow_copy_assignable_v<FR>
                  && (is_nothrow_copy_assignable_v<S> && ...)))
    = default;
  cartesian_product_range(this_t &&) = default;
  this_t &operator =(this_t &&) = delete;
  this_t &operator =(this_t &&)
    requires (is_move_assignable_v<FR> && (is_move_assignable_v<S> && ...)
              && !is_reference_v<FR> && ((!is_reference_v<S>) && ...))
    = default;
  this_t &operator =(const this_t &x)
    requires (is_copy_assignable_v<FR> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<FR> && (!is_reference_v<S> && ...)
              && !(is_nothrow_copy_assignable_v<FR>
                   && (is_nothrow_copy_assignable_v<S> && ...))) {
    copy_and_swap(x.t, t);
    return *this;
  }
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<tuple<FR, S...>>)
    requires (is_swappable_v<tuple<FR, S...>>
              && (!is_reference_v<FR> && ((!is_reference_v<S>) && ...))) {
    adl_swap(x.t, y.t);
  }

  constexpr explicit(sizeof...(S) == 0) cartesian_product_range(FR r, S...s)
    : t(forward<FR>(r), forward<S>(s)...) {}

private:
  constexpr bool any_of_mpt() {
    bool ret = false;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        if (empty(at<decltype(tt)::type::value>(t)))
          ret = true;
      });
    return ret;
  }
  constexpr bool any_of_mpt() const {
    bool ret = false;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        if (empty(at<decltype(tt)::type::value>(t)))
          ret = true;
      });
    return ret;
  }
public:
  constexpr auto begin() {
    if (any_of_mpt())
      return end();
    else
      return cartesian_product_iterator<rng_itr<FR>, rng_itr<S>...>
        (bind_tuple(t, [](auto &&r) {
          return seq(begin(r), begin(r), end(r));
        }));
  }
  constexpr auto end() {
    if constexpr (!is_bitr
                  <cartesian_product_iterator
                   <rng_itr<FR>, rng_itr<S>...>>) {
      return cartesian_product_iterator<rng_itr<FR>, rng_itr<S>...>
        (bind_tuple(t, [&](auto &&r) {
          return seq(end(r), begin(r), end(r));
        }));
    }
    else {
      return cartesian_product_iterator<rng_itr<FR>, rng_itr<S>...>
        (bind_tuple(t, [&](auto &&r) {
          return seq(addressof(r) == addressof(at<sizeof...(S)>(t))
                     ? end(r) : prev(end(r), 1, begin(r)),
                     begin(r), end(r));
        }));
    }
  }
  constexpr auto begin() const
    requires (is_rng<add_const_t<FR>>
              && (is_rng<add_const_t<S>> && ...)) {
    if (any_of_mpt())
      return end();
    else
      return cartesian_product_iterator<rng_itr<add_const_t<FR>>,
                                        rng_itr<add_const_t<S>>...>
        (bind_tuple(t, [](auto &&r) {
          return seq(begin(r), begin(r), end(r));
        }));
  }
  constexpr auto end() const
    requires (is_rng<add_const_t<FR>>
              && (is_rng<add_const_t<S>> && ...)) {
    if constexpr (!is_bitr
                  <cartesian_product_iterator
                   <rng_itr<add_const_t<FR>>,
                    rng_itr<add_const_t<S>>...>>) {
      return cartesian_product_iterator
        <rng_itr<add_const_t<FR>>, rng_itr<add_const_t<S>>...>
        (bind_tuple(t, [&](auto &&r) {
          return seq(end(r), begin(r), end(r));
        }));
    }
    else {
      return cartesian_product_iterator<rng_itr<add_const_t<FR>>,
                                        rng_itr<add_const_t<S>>...>
        (bind_tuple(t, [&](auto &&r) {
          return seq(addressof(r) == addressof(at<sizeof...(S)>(t))
                     ? end(r)
                     : prev(end(r), 1, begin(r)),
                     begin(r), end(r));
        }));
    }
  }

  constexpr bool empty() {
    return begin() == end();
  }
  constexpr bool empty() const
    requires (is_rng<add_const_t<FR>>
              && (is_rng<add_const_t<S>> && ...)) {
    return begin() == end();
  }

  constexpr common_type_t<rng_szt<FR>, rng_szt<S>...> size()
    requires (rng_is_sized<FR> && (rng_is_sized<S> && ...)) {
    common_type_t<rng_szt<FR>, rng_szt<S>...> ret = 1;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        constexpr auto i = decltype(tt)::type::value;
        ret *= at<i>(t).size();
      });
    return ret;
  }
  constexpr common_type_t<rng_szt<FR>, rng_szt<S>...> size() const
    requires (is_rng<add_const_t<FR>> && (is_rng<add_const_t<S>> && ...)
              && (rng_is_sized<add_const_t<FR>>
                  && (rng_is_sized<add_const_t<S>> && ...))) {
    common_type_t<rng_szt<add_const_t<FR>>, rng_szt<add_const_t<S>>...> ret = 1;
    type_pack_for_each<make_index_sequence<sizeof...(S) + 1u>>
      ([&](auto tt) {
        constexpr auto i = decltype(tt)::type::value;
        ret *= at<i>(t).size();
      });
    return ret;
  }
};
template <class FR, class...S>
struct is_range_reference<cartesian_product_range<FR, S...>>
  : bool_constant<is_range_reference<FR>::value
                  && (is_range_reference<S>::value && ...)> {};
struct fo_cartesian_product_rng {
  template <class FR, class...S>
  constexpr auto operator ()(FR &&fr, S &&...s) const
    requires (is_frng<FR> && (is_frng<S> && ...)) {
    return cartesian_product_range<rng_forward_t<FR>, rng_forward_t<S>...>
      (forward<FR>(fr), forward<S>(s)...);
  }
};
inline constexpr fo_cartesian_product_rng cartesian_product_rng{};

}

// split
// split_range
namespace re {

struct fo_split {
  template <class FR, class FR2, class OI>
  constexpr OI operator ()(FR &&r, FR2 &&r2, OI o) const
    requires is_frng<FR> && is_frng<FR2> {
    if (!empty(r)) {
      if (!empty(r2)) {
        auto p = begin(r);
        const auto ed = end(r);
        for (;;) {
          const auto [it, it2] = find_subrange(rng(p, ed), r2);
          *o = rng(p, it);
          ++o;
          if (it2 == ed) {
            if (it != ed) {
              *o = rng(ed, ed);
              ++o;
            }
            break;
          }
          p = it2;
        }
      }
      else {
        *o = rng(r);
        ++o;
      }
    }
    return o;
  }
};
inline constexpr fo_split split{};

template <class FI>
class split_iterator {
  FI i = FI{};
  FI i2 = FI{};
  FI ed = FI{};
  itr_dft<FI> k = itr_dft<FI>{};

public:
  using value_type = iter_pair<FI>;
  using reference = value_type;
  using difference_type = itr_dft<FI>;
  using pointer = void;
  using iterator_category = forward_iterator_tag;

  split_iterator() = default;
  ~split_iterator() = default;
  split_iterator(const split_iterator &) = default;
  split_iterator &operator =(const split_iterator &) = default;
  split_iterator(split_iterator &&) = default;
  split_iterator &operator =(split_iterator &&) = default;
  friend constexpr void swap(split_iterator &x, split_iterator &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.i, y.i);
    adl_swap(x.i2, y.i2);
    adl_swap(x.ed, y.ed);
    adl_swap(x.k, y.k);
  }

  constexpr split_iterator(FI a, FI b, FI c, itr_dft<FI> d)
    : i(a), i2(b), ed(c), k(d) {}

  template <class FI2>
  constexpr split_iterator(const split_iterator<FI2> &x)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>)
    : i(x.i), i2(x.i2), ed(x.ed), k(x.k) {}

  constexpr value_type base() const {
    return rng(i, i2);
  }

  void operator ->() const = delete;
  constexpr reference operator *() const {
    return rng(i, i2);
  }
  constexpr split_iterator &operator ++() {
    auto it = i2;
    if (advance(it, k, ed) != 0)
      k = 0;
    const auto it2 = search(rng(it, ed), rng(i2, it));
    i = it;
    i2 = it2;
    return *this;
  }
  constexpr split_iterator operator ++(int) {
    return iter_post_increment(*this);
  }

  template <class T, class U>
  friend constexpr auto operator ==(const split_iterator<T> &x,
                                    const split_iterator<U> &y)
    ->decltype(x.base() == y.base());
};
template <class T, class U>
constexpr auto operator ==(const split_iterator<T> &x,
                           const split_iterator<U> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base() && x.k == y.k;
}
struct fo_split_itr {
  template <class FI>
  constexpr auto operator ()(FI i, FI i2, FI ed, itr_dft<FI> k) const
    requires is_fitr<FI> {
    return split_iterator<FI>(i, i2, ed, k);
  }
};
inline constexpr fo_split_itr split_itr{};

template <class FR, class FR2>
class split_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::size;
  using range_fns::empty;

  struct cache_data_t {
    rng_itr<FR> it;
    rng_dft<FR2> k;
  };
  using cache_t = conditional_t<is_rng_ref<FR> && is_rng_ref<FR2>,
                                simple_wrapper<cache_data_t>,
                                non_propagating_cache<cache_data_t>>;

  FR r = FR{};
  FR2 r2 = FR2{};
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x) {
    const auto k = ssize(r2);
    const auto ed = end(r);
    const auto it = ((k != 0) ? search(r, r2) : ed);
    x.k = (it == ed ? 0 : k);
    x.it = it;
  }
  constexpr void init() {
    if constexpr (is_rng_ref<FR>) {
      init_impl(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (!is_rng_ref<FR>) {
      if (c.empty()) {
        cache_data_t tmp{};
        init_impl(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  split_range() = delete;
  constexpr split_range() requires (is_default_constructible_v<FR>
                                    && is_default_constructible_v<FR2>) {
    init();
  }
  ~split_range() = default;
  split_range(const split_range &) = default;
  split_range &operator =(const split_range &) = default;
  split_range(split_range &&) = default;
  split_range &operator =(split_range &&) = default;
  constexpr split_range &operator =(const split_range &x)
    requires (is_copy_assignable_v<FR> && is_copy_assignable_v<FR2>
              && !is_reference_v<FR> && !is_reference_v<FR2>
              && (!is_nothrow_copy_assignable_v<FR>
                  || !is_nothrow_copy_assignable_v<FR2>)) {
    copy_and_swap(x, *this);
    return *this;
  }
  template <class RR = FR, class RR2 = FR2>
  friend constexpr void swap(split_range &x, split_range &y)
    noexcept(is_nothrow_swappable_v<RR>
             && is_nothrow_swappable_v<RR2>)
    requires (is_swappable_v<RR> && is_swappable_v<RR2>
              && default_swappable<split_range<RR, RR2>>) {
    adl_swap(x.r, y.r);
    adl_swap(x.r2, y.r2);
    adl_swap(x.c, y.c);
  }

  constexpr split_range(FR a, FR2 b)
    : r(forward<FR>(a)), r2(forward<FR2>(b)) {
    init();
  }

  constexpr auto begin() {
    delayed_init();
    return split_itr(begin(r), c->it, end(r), c->k);
  }
  constexpr auto end() {
    const auto ed = end(r);
    return split_itr(ed, ed, ed, 0);
  }
  constexpr auto begin() const
    requires is_rng_ref<add_const_t<FR>> && is_rng_ref<add_const_t<FR2>> {
    return split_itr(begin(r), c->it, end(r), c->k);
  }
  constexpr auto end() const
    requires is_rng_ref<add_const_t<FR>> && is_rng_ref<add_const_t<FR2>> {
    const auto ed = end(r);
    return split_itr(ed, ed, ed, 0);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const
    requires is_rng_ref<add_const_t<FR>> && is_rng_ref<add_const_t<FR2>> {
    return empty(r);
  }
};
template <class FR, class FR2>
struct is_range_reference<split_range<FR, FR2>>
  : bool_constant<is_range_reference<FR>::value
                  && is_range_reference<FR2>::value> {};
struct fo_split_rng {
  template <class FR, class FR2>
  constexpr auto operator ()(FR &&r, FR2 &&r2) const
    requires is_frng<FR> && is_frng<FR2> {
    return split_range<rng_forward_t<FR>, rng_forward_t<FR2>>
      (forward<FR>(r), forward<FR2>(r2));
  }
};
inline constexpr fo_split_rng split_rng{};

}

// zip_top
// zip
// zip_range
namespace re {

namespace inner::fns {

template <class R>
constexpr auto zip_help_make(R &r) {
  if constexpr (is_rng<R>) {
    return rng(r);
  }
  else {
    return r;
  }
}
template <class R>
constexpr void zip_help_advance(R &r) {
  if constexpr (is_rng<R>) {
    ++r.first;
  }
  else {
    ++r;
  }
}
template <class R>
constexpr bool zip_help_not_over(R &r) {
  if constexpr (is_rng<R>) {
    return !empty(r);
  }
  else {
    return true;
  }
}

template <class T>
constexpr decltype(auto) zip_help_deref(T &x) {
  if constexpr (is_rng<T>) {
    return *x.first;
  }
  else {
    return *x;
  }
}
template <class T, size_t...IDS>
constexpr void zip_help_assign(T &x, index_sequence<IDS...>) {
  auto &o = at<tuple_size_v<T> - 1>(x);
  *o = forward_as_tuple(zip_help_deref(at<IDS>(x))...);
  ++o;
}
template <class T>
constexpr void zip_help_assign(T &x) {
  zip_help_assign(x, make_index_sequence<tuple_size_v<T> - 1>{});
}

template <class T>
constexpr auto zip_help_extract_one(T &x) {
  if constexpr (is_rng<T>) {
    return x.first;
  }
  else {
    return x;
  }
}
template <class T, size_t...IDS>
constexpr auto zip_help_extract(T &x, index_sequence<IDS...>) {
  return tuple(zip_help_extract_one(at<IDS>(x))...);
}
template <class T>
constexpr auto zip_help_extract(T &x) {
  return zip_help_extract(x, make_index_sequence<tuple_size_v<T>>{});
}

template <class T>
constexpr void zip_top_help_rrng(T &d) {}
template <class T, class R, class...S>
constexpr void zip_top_help_rrng(T &d, R &r, S &...s) {
  if constexpr (is_rng<R>) {
    const auto n = ssize(r);
    if (d.has_value()) {
      if (n < *d)
        *d = n;
    }
    else
      d.emplace(n);
    inner::fns::zip_top_help_rrng(d, s...);
  }
  else {
    inner::fns::zip_top_help_rrng(d, s...);
  }
}
template <class T>
constexpr auto zip_top_help_rrng_get(T &x) {
  if constexpr (is_rng<T>) {
    return begin(x);
  }
  else {
    return x;
  }
}

}
struct fo_zip_top {
  template <class R, class...S>
  constexpr auto operator ()(R &&r, S &&...s) const
    requires ((is_rng<R> || is_itr<decay_t<R>>)
              && ((is_rng<S> || is_itr<decay_t<S>>) && ...)) {
    if constexpr (!((is_rrng<R> || is_itr<decay_t<R>>)
                    && ((is_rrng<S> || is_itr<decay_t<S>>) && ...))) {
      auto t = tuple(inner::fns::zip_help_make(r),
                     inner::fns::zip_help_make(s)...);
      for (;;) {
        bool y = true;
        type_pack_for_each<make_index_sequence<sizeof...(S) + 1>>
          ([&](auto tag) {
            constexpr size_t i = decltype(tag)::type::value;
            if (!inner::fns::zip_help_not_over(at<i>(t)))
              y = false;
          });
        if (!y)
          break;

        type_pack_for_each<make_index_sequence<sizeof...(S) + 1>>
          ([&](auto tag) {
            constexpr size_t i = decltype(tag)::type::value;
            inner::fns::zip_help_advance(at<i>(t));
          });
      }
      return inner::fns::zip_help_extract(t);
    }
    else {
      using dft = common_type_t
        <conditional_t<is_rng<R>, rng_dft<R>, itr_dft<decay_t<R>>>,
         conditional_t<is_rng<S>, rng_dft<S>, itr_dft<decay_t<S>>>...>;
      optional<dft> d;
      inner::fns::zip_top_help_rrng(d, r, s...);
      return tuple(next(inner::fns::zip_top_help_rrng_get(r), *d),
                   next(inner::fns::zip_top_help_rrng_get(s), *d)...);
    }
  }
};
inline constexpr fo_zip_top zip_top{};
struct fo_zip {
  template <class R, class...S>
  constexpr auto operator ()(R &&r, S &&...s) const
    requires (is_rng<R> || is_itr<R>) {
    auto t = tuple(inner::fns::zip_help_make(r),
                   inner::fns::zip_help_make(s)...);
    for (;;) {
      bool y = true;
      type_pack_for_each<make_index_sequence<sizeof...(S)>>([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        if (!inner::fns::zip_help_not_over(at<i>(t)))
          y = false;
      });
      if (!y)
        break;

      inner::fns::zip_help_assign(t);

      type_pack_for_each<make_index_sequence<sizeof...(S)>>([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        inner::fns::zip_help_advance(at<i>(t));
      });
    }
    return at<sizeof...(S)>(t);
  }
};
inline constexpr fo_zip zip{};

namespace inner {

template <class I>
struct zip_node_iter {
  using type = I;
};
template <class I>
requires (!is_itr<I>)
struct zip_node_iter<I> : tuple_element<0, I> {};
template <class I>
using zip_node_iter_t = typename zip_node_iter<I>::type;

}
namespace inner::fns {

template <class T>
constexpr auto &zip_node_ref(T &x) {
  if constexpr (is_itr<decay_t<T>>) {
    return x;
  }
  else {
    return at<0>(x);
  }
}

template <size_t I, size_t J, class T, class U>
constexpr void zip_node_cmp(T &x, U &y, auto &ret) requires (I == J) {
  if constexpr (!is_itr<T>) {
    if (const auto tmp = synth_3way(zip_node_ref(at<I>(x)),
                                    zip_node_ref(at<I>(y)));
        tmp != 0)
      ret = tmp;
    else
      ret = strong_eq;
  }
  else {
    ret = strong_eq;
  }
}
template <size_t I, size_t J, class T, class U>
constexpr void zip_node_cmp(T &x, U &y, auto &ret) requires (I != J) {
  if constexpr (!is_itr<decltype(at<I>(x))>) {
    if (const auto tmp = synth_3way(zip_node_ref(at<I>(x)),
                                    zip_node_ref(at<I>(y)));
        tmp != 0)
      ret = tmp;
    else
      inner::fns::zip_node_cmp<I + 1, J>(x, y, ret);
  }
  else {
    inner::fns::zip_node_cmp<I + 1, J>(x, y, ret);
  }
}

}
template <class I, class...S>
class zip_iterator {
  using this_t = zip_iterator;

  template <class, class...>
  friend class zip_iterator;

  using data_t = tuple<I, S...>;
  tuple<I, S...> t;

public:
  using value_type = tuple
    <itr_vt<inner::zip_node_iter_t<I>>, itr_vt<inner::zip_node_iter_t<S>>...>;
  using reference = tuple
    <itr_ref<inner::zip_node_iter_t<I>>, itr_ref<inner::zip_node_iter_t<S>>...>;
  using difference_type = common_type_t
    <itr_dft<inner::zip_node_iter_t<I>>,
     itr_dft<inner::zip_node_iter_t<S>>...>;
  using pointer = void;
  using iterator_category
    = conditional_t
    <is_fitr<inner::zip_node_iter_t<I>>
     && (is_fitr<inner::zip_node_iter_t<S>> && ...),
     forward_iterator_tag,
     input_iterator_tag>;

  zip_iterator() = default;
  ~zip_iterator() = default;
  zip_iterator(const zip_iterator &) = default;
  zip_iterator &operator =(const zip_iterator &) = default;
  zip_iterator(zip_iterator &&) = default;
  zip_iterator &operator =(zip_iterator &&) = default;
  friend constexpr void swap(zip_iterator &x, zip_iterator &y)
    noexcept(is_nothrow_swappable_v<tuple<I, S...>>) {
    adl_swap(x.t, y.t);
  }

  constexpr explicit zip_iterator(const data_t &x) : t(x) {}
  constexpr explicit(sizeof...(S) == 1) zip_iterator(I i, S...s) : t(i, s...) {}
  constexpr data_t base() const {
    return t;
  }

  template <class I2, class...S2>
  constexpr zip_iterator(const zip_iterator<I2, S2...> &x)
    requires (!is_same_v<I2, I> && is_convertible_v<const I2 &, I>
              && ((!is_same_v<S2, S> && is_convertible_v<const S2 &, S>)
                  && ...))
    : t(x.t) {}

  constexpr auto operator *() const {
    return bind_tuple(t, [](auto &x)->decltype(auto) {
      return *inner::fns::zip_node_ref(x);
    });
  }
  constexpr void operator ->() const = delete;
  constexpr zip_iterator &operator ++() {
    auto ret = *this;
    type_pack_for_each<make_index_sequence<tuple_size_v<data_t>>>
      ([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        ++inner::fns::zip_node_ref(at<i>(ret.t));
      });
    *this = ret;
    return *this;
  }
  value_type *operator ++(int);
  constexpr auto operator ++(int) requires is_fitr_ctg<iterator_category> {
    return iter_post_increment(*this);
  }

  constexpr zip_iterator &operator --()
    requires is_bitr_ctg<iterator_category> {
    auto ret = *this;
    type_pack_for_each<make_index_sequence<tuple_size_v<data_t>>>
      ([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        --inner::fns::zip_node_ref(at<i>(ret.t));
      });
    *this = ret;
    return *this;
  }
  constexpr zip_iterator operator --(int)
    requires is_bitr_ctg<iterator_category> {
    return iter_post_decrement(*this);
  }

  template <class I1, class I2, class...S1, class...S2>
  friend constexpr bool operator ==(const zip_iterator<I1, S1...> &,
                                    const zip_iterator<I2, S2...> &)
    requires (can_apply_equal_to<const I1 &, const I2 &>
              && (can_apply_equal_to<const S1 &, const S2 &> && ...));
};
template <class I1, class I2, class...S1, class...S2>
constexpr bool operator ==(const zip_iterator<I1, S1...> &x,
                           const zip_iterator<I2, S2...> &y)
  requires (can_apply_equal_to<const I1 &, const I2 &>
            && (can_apply_equal_to<const S1 &, const S2 &> && ...)) {
  bool ret = false;
  type_pack_for_each_until_false
    <make_index_sequence
     <tuple_size_v<typename zip_iterator<I1, S1...>::data_t>>>
    ([&](auto tag) {
      constexpr size_t i = decltype(tag)::type::value;
      if constexpr (!is_itr<decay_t<decltype(at<i>(x.t))>>
                    && !is_itr<decay_t<decltype(at<i>(y.t))>>) {
        if (at<0>(at<i>(x.t)) == at<0>(at<i>(y.t))) {
          ret = true;
          return false;
        }
        else
          return true;
      }
      else {
        return true;
      }
    });
  return ret;
}
struct fo_zip_itr {
  template <class I, class...S>
  constexpr auto operator ()(I i, S...s) const {
    return zip_iterator<I, S...>(i, s...);
  }
};
inline constexpr fo_zip_itr zip_itr{};
struct fo_zip_itr_from_tuple {
  template <class I, class...S>
  constexpr auto operator ()(tuple<I, S...> x) const {
    return zip_iterator<I, S...>(x);
  }
};
inline constexpr fo_zip_itr_from_tuple zip_itr_from_tuple{};

namespace inner {

template <class T>
struct zip_rng_forward {
  using type = decay_t<T>;
};
template <class T>
requires is_rng<T>
struct zip_rng_forward<T> {
  using type = rng_forward_t<T>;
};
template <class T>
using zip_rng_forward_t = typename zip_rng_forward<T>::type;

template <class X, class...S>
inline constexpr bool zip_rng_args_are_valid
  = ((is_rng<X> || (is_itr<decay_t<X>>))
     && ((is_rng<S> || (is_itr<decay_t<S>>)) && ...));

}
namespace inner::fns {

template <class T>
constexpr decay_t<T> zip_rng_begin(T &&x) {
  return x;
}
template <class T>
constexpr auto zip_rng_begin(T &&x) requires is_rng<T> {
  return tuple(begin(x));
}

template <class T>
constexpr decay_t<T> zip_rng_end(T &&x) {
  return x;
}
template <class T>
constexpr auto zip_rng_end(T &&x) requires is_rng<T> {
  return tuple(end(x));
}

}
template <class X, class...S>
class zip_range : range_fns {
  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool enable_const = inner::zip_rng_args_are_valid
    <add_const_t<X>, add_const_t<S>...>;

  tuple<X, S...> t;

public:
  zip_range() = default;
  ~zip_range() = default;
  zip_range(const zip_range &) = default;
  zip_range &operator =(const zip_range &) = delete;
  zip_range &operator =(const zip_range &)
    requires (is_copy_assignable_v<X> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)
              && (is_nothrow_copy_assignable_v<X>
                  && (is_nothrow_copy_assignable_v<S> && ...)))
    = default;
  zip_range(zip_range &&) = default;
  zip_range &operator =(zip_range &&) = delete;
  zip_range &operator =(zip_range &&)
    requires (is_move_assignable_v<X> && (is_move_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...))
    = default;
  constexpr zip_range &operator =(const zip_range &x)
    requires (is_copy_assignable_v<X> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)
              && (!is_nothrow_copy_assignable_v<X>
                  || ((!is_nothrow_copy_assignable_v<S>) || ...))) {
    copy_and_swap(x, *this);
    return *this;
  }
  friend constexpr void swap(zip_range &x, zip_range &y)
    noexcept(is_nothrow_swappable_v<tuple<X, S...>>)
    requires (is_swappable_v<tuple<X, S...>>
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)) {
    adl_swap(x.t, y.t);
  }

  constexpr zip_range(X x, S...s) : t(forward<X>(x), forward<S>(s)...) {}

  constexpr auto begin() {
    return zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::zip_rng_begin(x);
    }));
  }
  constexpr auto begin() const requires enable_const {
    return zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::zip_rng_begin(x);
    }));
  }
  constexpr auto end() {
    return zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::zip_rng_end(x);
    }));
  }
  constexpr auto end() const requires enable_const {
    return zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::zip_rng_end(x);
    }));
  }

  constexpr bool empty() {
    return begin() == end();
  }
  constexpr bool empty() const requires enable_const {
    return begin() == end();
  }
};
template <class X, class...S>
struct is_range_reference<zip_range<X, S...>>
  : bool_constant<(is_rng_ref<X> || !is_rng<X>)
                  && ((is_rng_ref<S> || !is_rng<S>) && ...)> {};
struct fo_zip_rng {
  template <class X, class...S>
  constexpr auto operator ()(X &&x, S &&...s) const {
    return zip_range<inner::zip_rng_forward_t<X>,
                     inner::zip_rng_forward_t<S>...>
      (forward<X>(x), forward<S>(s)...);
  }
};
inline constexpr fo_zip_rng zip_rng{};

}

// aligned_zip_range
namespace re {

template <class FI, class...S>
class aligned_zip_iterator {
  using this_t = aligned_zip_iterator;

  template <class, class...>
  friend class aligned_zip_iterator;

  using data_t = tuple<FI, S...>;
  tuple<FI, S...> t;

public:
  using value_type = tuple<itr_vt<FI>, itr_vt<S>...>;
  using reference = tuple<itr_ref<FI>, itr_ref<S>...>;
  using difference_type = common_type_t<itr_dft<FI>, itr_dft<S>...>;
  using pointer = void;
  using iterator_category
    = conditional_t<is_ritr<FI> && (is_ritr<S> && ...),
                    random_access_iterator_tag,
                    conditional_t<is_bitr<FI> && (is_bitr<S> && ...),
                                  bidirectional_iterator_tag,
                                  forward_iterator_tag>>;

  aligned_zip_iterator() = default;
  ~aligned_zip_iterator() = default;
  aligned_zip_iterator(const aligned_zip_iterator &) = default;
  aligned_zip_iterator &operator =(const aligned_zip_iterator &) = default;
  aligned_zip_iterator(aligned_zip_iterator &&) = default;
  aligned_zip_iterator &operator =(aligned_zip_iterator &&) = default;
  friend constexpr void swap(aligned_zip_iterator &x, aligned_zip_iterator &y)
    noexcept(is_nothrow_swappable_v<tuple<FI, S...>>) {
    adl_swap(x.t, y.t);
  }

  constexpr explicit aligned_zip_iterator(const data_t &x) : t(x) {}
  constexpr explicit(sizeof...(S) == 1) aligned_zip_iterator(FI i, S...s)
    : t(i, s...) {}
  constexpr data_t base() const {
    return t;
  }

  template <class FI2, class...S2>
  constexpr aligned_zip_iterator(const aligned_zip_iterator<FI2, S2...> &x)
    requires (!is_same_v<FI2, FI> && is_convertible_v<const FI2 &, FI>
              && ((!is_same_v<S2, S> && is_convertible_v<const S2 &, S>)
                  && ...))
    : t(x.t) {}

  constexpr reference operator *() const {
    return bind_tuple(t, [](auto &x)->decltype(auto) {return *x;});
  }
  constexpr void operator ->() const = delete;
  constexpr aligned_zip_iterator &operator ++() {
    auto ret = *this;
    type_pack_for_each<make_index_sequence<tuple_size_v<data_t>>>
      ([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        ++at<i>(ret.t);
      });
    *this = ret;
    return *this;
  }
  value_type *operator ++(int);
  constexpr auto operator ++(int) requires is_fitr_ctg<iterator_category> {
    return iter_post_increment(*this);
  }

  constexpr aligned_zip_iterator &operator --()
    requires is_bitr_ctg<iterator_category> {
    auto ret = *this;
    type_pack_for_each<make_index_sequence<tuple_size_v<data_t>>>
      ([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        --at<i>(ret.t);
      });
    *this = ret;
    return *this;
  }
  constexpr aligned_zip_iterator operator --(int)
    requires is_bitr_ctg<iterator_category> {
    return iter_post_decrement(*this);
  }

  constexpr aligned_zip_iterator &operator +=(difference_type n)
    requires is_ritr_ctg<iterator_category> {
    return operator =(operator +(n));
  }
  constexpr aligned_zip_iterator &operator -=(difference_type n)
    requires is_ritr_ctg<iterator_category> {
    return operator =(operator +(-n));
  }
  constexpr aligned_zip_iterator operator +(difference_type n) const
    requires is_ritr_ctg<iterator_category> {
    auto ret = *this;
    type_pack_for_each<make_index_sequence<tuple_size_v<data_t>>>
      ([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        at<i>(ret.t) += n;
      });
    return ret;
  }
  constexpr aligned_zip_iterator operator -(difference_type n) const
    requires is_ritr_ctg<iterator_category> {
    return operator +(-n);
  }
  constexpr reference operator [](difference_type n) const
    requires is_ritr_ctg<iterator_category> {
    return *(operator +(n));
  }

  template <class FI1, class FI2, class...S1, class...S2>
  friend constexpr bool operator ==(const aligned_zip_iterator<FI1, S1...> &,
                                    const aligned_zip_iterator<FI2, S2...> &)
    requires (can_apply_equal_to<const FI1 &, const FI2 &>
              && (can_apply_equal_to<const S1 &, const S2 &> && ...));
  template <class FI1, class FI2, class...S1, class...S2>
  friend constexpr common_comparison_category_t
  <synth_3way_result<FI1, FI2>, synth_3way_result<S1, S2>...>
  operator <=>(const aligned_zip_iterator<FI1, S1...> &,
               const aligned_zip_iterator<FI2, S2...> &);
  template <class FI1, class FI2, class...S1, class...S2>
  friend constexpr itr_dft<aligned_zip_iterator<FI1, S1...>>
  operator -(const aligned_zip_iterator<FI1, S1...> &,
             const aligned_zip_iterator<FI2, S2...> &)
    requires (can_apply_minus<const FI1 &, const FI2 &>
              && (can_apply_minus<const S1 &, const S2 &> && ...)
              && itr_is_counted<FI1> && itr_is_counted<FI2>
              && ((itr_is_counted<S1> && itr_is_counted<S2>) && ...));
};
template <class FI1, class FI2, class...S1, class...S2>
constexpr bool operator ==(const aligned_zip_iterator<FI1, S1...> &x,
                           const aligned_zip_iterator<FI2, S2...> &y)
  requires (can_apply_equal_to<const FI1 &, const FI2 &>
            && (can_apply_equal_to<const S1 &, const S2 &> && ...)) {
  return at<0>(x.t) == at<0>(y.t);
}
template <class FI1, class FI2, class...S1, class...S2>
constexpr common_comparison_category_t
<synth_3way_result<FI1, FI2>, synth_3way_result<S1, S2>...>
operator <=>(const aligned_zip_iterator<FI1, S1...> &x,
             const aligned_zip_iterator<FI2, S2...> &y) {
  return synth_3way(at<0>(x.t), at<0>(y.t));
}
template <class FI1, class FI2, class...S1, class...S2>
constexpr itr_dft<aligned_zip_iterator<FI1, S1...>>
operator -(const aligned_zip_iterator<FI1, S1...> &x,
           const aligned_zip_iterator<FI2, S2...> &y)
  requires (can_apply_minus<const FI1 &, const FI2 &>
            && (can_apply_minus<const S1 &, const S2 &> && ...)
            && itr_is_counted<FI1> && itr_is_counted<FI2>
            && ((itr_is_counted<S1> && itr_is_counted<S2>) && ...)) {
  return at<0>(x.t) - at<0>(y.t);
}
template <class FI1, class...S1>
constexpr aligned_zip_iterator<FI1, S1...>
operator +(itr_dft<aligned_zip_iterator<FI1, S1...>> n,
           const aligned_zip_iterator<FI1, S1...> &x)
  requires is_ritr<aligned_zip_iterator<FI1, S1...>> {
  return x.operator +(n);
}
struct fo_aligned_zip_itr {
  template <class FI, class...S>
  constexpr auto operator ()(FI i, S...s) const {
    return aligned_zip_iterator<FI, S...>(i, s...);
  }
};
inline constexpr fo_aligned_zip_itr aligned_zip_itr{};
struct fo_aligned_zip_itr_from_tuple {
  template <class FI, class...S>
  constexpr auto operator ()(tuple<FI, S...> x) const {
    return aligned_zip_iterator<FI, S...>(x);
  }
};
inline constexpr fo_aligned_zip_itr_from_tuple aligned_zip_itr_from_tuple{};

namespace inner {

template <class X>
struct aligned_zip_iter {
  using type = decay_t<X>;
};
template <class X>
requires is_rng<X>
struct aligned_zip_iter<X> {
  using type = rng_itr<X>;
};
template <class X>
using aligned_zip_iter_t = typename aligned_zip_iter<X>::type;

}
namespace inner::fns {

template <class T>
constexpr decay_t<T> aligned_zip_rng_begin(T &&x) {
  return x;
}
template <class T>
constexpr auto aligned_zip_rng_begin(T &&x) requires is_rng<T> {
  return begin(x);
}

}
template <class X, class...S>
class aligned_zip_range : range_fns {
  using this_t = aligned_zip_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool enable_const
    = inner::zip_rng_args_are_valid<add_const_t<X>, add_const_t<S>...>;
  static constexpr bool delay_init
    = !((is_rng_ref<X> || !is_rng<X>)
        && ((is_rng_ref<S> || !is_rng<S>) && ...));
  using cache_data_t
    = tuple<inner::aligned_zip_iter_t<X>, inner::aligned_zip_iter_t<S>...>;
  using cache_t = conditional_t
    <!delay_init,
     simple_wrapper<cache_data_t>, non_propagating_cache<cache_data_t>>;

  tuple<X, S...> t;
  cache_t c = cache_t{};

  template <size_t...IDS>
  constexpr void init_impl(cache_data_t &x, index_sequence<IDS...>) {
    x = zip_top(at<IDS>(t)...);
  }
  constexpr void init_impl(cache_data_t &x) {
    init_impl(x, make_index_sequence<sizeof...(S) + 1>{});
  }
  constexpr void init() {
    if constexpr (!delay_init) {
      init_impl(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init) {
      if (c.empty()) {
        cache_data_t tmp;
        init_impl(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  aligned_zip_range() = delete;
  constexpr aligned_zip_range()
    requires (is_default_constructible_v<tuple<X, S...>>
              && is_default_constructible_v<cache_t>) {
    init();
  }
  ~aligned_zip_range() = default;
  aligned_zip_range(const aligned_zip_range &) = default;
  aligned_zip_range &operator =(const aligned_zip_range &) = delete;
  constexpr aligned_zip_range &operator =(const aligned_zip_range &)
    requires (is_copy_assignable_v<X> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)
              && (is_nothrow_copy_assignable_v<X>
                  && (is_nothrow_copy_assignable_v<S> && ...)))
    = default;
  aligned_zip_range(aligned_zip_range &&) = default;
  aligned_zip_range &operator =(aligned_zip_range &&) = delete;
  aligned_zip_range &operator =(aligned_zip_range &&)
    requires (is_move_assignable_v<X> && (is_move_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...))
    = default;
  constexpr aligned_zip_range &operator =(const aligned_zip_range &x)
    requires (is_copy_assignable_v<X> && (is_copy_assignable_v<S> && ...)
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)
              && (!is_nothrow_copy_assignable_v<X>
                  || ((!is_nothrow_copy_assignable_v<S>) || ...))) {
    copy_and_swap(x, *this);
    return *this;
  }
  friend constexpr void swap(aligned_zip_range &x, aligned_zip_range &y)
    noexcept(is_nothrow_swappable_v<tuple<X, S...>>)
    requires (is_swappable_v<tuple<X, S...>>
              && !is_reference_v<X> && ((!is_reference_v<S>) && ...)) {
    adl_swap(x.t, y.t);
    adl_swap(x.c, y.c);
  }

  constexpr aligned_zip_range(X x, S...s)
    : t(forward<X>(x), forward<S>(s)...) {
    init();
  }

  constexpr auto begin() {
    return aligned_zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::aligned_zip_rng_begin(x);
    }));
  }
  constexpr auto begin() const requires (enable_const && !delay_init) {
    return aligned_zip_itr_from_tuple(bind_tuple(t, [](auto &&x) {
      return inner::fns::aligned_zip_rng_begin(x);
    }));
  }
  constexpr auto end() {
    delayed_init();
    return aligned_zip_itr_from_tuple(*c);
  }
  constexpr auto end() const requires (enable_const && !delay_init) {
    return aligned_zip_iterator
      <inner::aligned_zip_iter_t<add_const_t<X>>,
       inner::aligned_zip_iter_t<add_const_t<S>>...>(*c);
  }

  constexpr bool empty() {
    return begin() == end();
  }
  constexpr bool empty() const requires (enable_const && !delay_init) {
    return begin() == end();
  }

  constexpr auto size()
    requires ((!is_rng<X> || rng_is_sized<X>)
              && ((!is_rng<S> || rng_is_sized<S>) && ...)) {
    optional<common_type_t
             <conditional_t<is_rng<X>, rng_szt<X>, unsigned char>,
              conditional_t<is_rng<S>, rng_szt<S>, unsigned char>...>> ret;
    type_pack_for_each
      <make_index_sequence<sizeof...(S) + 1>>([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        if constexpr (is_rng<tuple_element_t<i, tuple<X, S...>>>) {
          auto &x = at<i>(t);
          const auto n = size(x);
          if (ret.has_value()) {
            if (n < *ret)
              *ret = n;
          }
          else
            ret.emplace(n);
        }
      });
    return *ret;
  }
  constexpr auto size() const
    requires ((enable_const && !delay_init)
              && (!is_rng<X> || rng_is_sized<X>)
              && ((!is_rng<S> || rng_is_sized<S>) && ...)) {
    optional<common_type_t
             <conditional_t<is_rng<X>, rng_szt<X>, unsigned char>,
              conditional_t<is_rng<S>, rng_szt<S>, unsigned char>...>> ret;
    type_pack_for_each
      <make_index_sequence<sizeof...(S) + 1>>([&](auto tag) {
        constexpr size_t i = decltype(tag)::type::value;
        if constexpr (is_rng<tuple_element_t<i, tuple<X, S...>>>) {
          auto &x = at<i>(t);
          const auto n = size(x);
          if (ret.has_value()) {
            if (n < *ret)
              *ret = n;
          }
          else
            ret.emplace(n);
        }
      });
    return *ret;
  }
};
template <class X, class...S>
struct is_range_reference<aligned_zip_range<X, S...>>
  : bool_constant<(is_rng_ref<X> || !is_rng<X>)
                  && ((is_rng_ref<S> || !is_rng<S>) && ...)> {};
struct fo_aligned_zip_rng {
  template <class X, class...S>
  constexpr auto operator ()(X &&x, S &&...s) const {
    return aligned_zip_range<inner::zip_rng_forward_t<X>,
                             inner::zip_rng_forward_t<S>...>
      (forward<X>(x), forward<S>(s)...);
  }
};
inline constexpr fo_aligned_zip_rng aligned_zip_rng{};

}

// enumerate
// enumerate_range
namespace re {

struct fo_enumerate_plus {
  template <class R, class OI>
  constexpr pair<rng_itr<R>, OI>
  operator ()(R &&r, OI o) const requires is_rng<R> {
    rng_dft<R> i = 0;
    const auto ed = end(r);
    auto it = begin(r);
    for (; it != ed; ++it) {
      *o = tuple<rng_dft<R>, rng_ref<R>>(i, *it);
      ++o;
      ++i;
    }
    return pair(it, o);
  }
};
inline constexpr fo_enumerate_plus enumerate_plus{};
struct fo_enumerate {
  template <class R, class OI>
  constexpr OI operator ()(R &&r, OI o) const requires is_rng<R> {
    return enumerate_plus(r, o).second;
  }
};
inline constexpr fo_enumerate enumerate{};

template <class I>
class enumerate_iterator {
  template <class>
  friend class enumerate_iterator;

  itr_dft<I> i = 0;
  I it = I{};

public:
  using value_type = pair<itr_dft<I>, itr_vt<I>>;
  using reference = pair<itr_dft<I>, itr_ref<I>>;
  using pointer = void;
  using difference_type = itr_dft<I>;
  using iterator_category = conditional_t
    <is_ritr<I>, random_access_iterator_tag, itr_ctg<I>>;

  enumerate_iterator() = default;
  ~enumerate_iterator() = default;
  enumerate_iterator(const enumerate_iterator &) = default;
  enumerate_iterator &operator =(const enumerate_iterator &) = default;
  enumerate_iterator(enumerate_iterator &&) = default;
  enumerate_iterator &operator =(enumerate_iterator &&) = default;
  friend constexpr void swap(enumerate_iterator &x, enumerate_iterator &y)
    noexcept(is_nothrow_swappable_v<I>) {
    adl_swap(x.i, y.i);
    adl_swap(x.it, y.it);
  }

  constexpr enumerate_iterator(I b, itr_dft<I> a) : i(a), it(b) {}

  template <class II>
  constexpr enumerate_iterator(const enumerate_iterator<II> &x)
    requires (!same_as<I, II> && is_convertible_v<const II &, I>)
    : i(x.i), it(x.it) {}

  constexpr I base() const {
    return it;
  }
  constexpr itr_dft<I> index() const noexcept {
    return i;
  }

  constexpr reference operator *() const {
    return {i, *it};
  }
  constexpr void operator ->() const = delete;
  constexpr enumerate_iterator &operator ++() {
    ++i;
    ++it;
    return *this;
  }
  value_type *operator ++(int);
  constexpr enumerate_iterator operator ++(int) requires is_fitr<I> {
    return iter_post_increment(*this);
  }

  constexpr enumerate_iterator &operator --() requires is_bitr<I> {
    --i;
    --it;
    return *this;
  }
  constexpr enumerate_iterator operator --(int) requires is_bitr<I> {
    return iter_post_decrement(*this);
  }

  constexpr enumerate_iterator &operator +=(difference_type n)
    requires is_ritr<I> {
    i += n;
    it += n;
    return *this;
  }
  constexpr enumerate_iterator &operator -=(difference_type n)
    requires is_ritr<I> {
    i -= n;
    it -= n;
    return *this;
  }
  constexpr enumerate_iterator operator +(difference_type n) const
    requires is_ritr<I> {
    return enumerate_iterator(it + n, i + n);
  }
  constexpr enumerate_iterator operator -(difference_type n) const
    requires is_ritr<I> {
    return enumerate_iterator(it - n, i - n);
  }
  constexpr reference operator [](difference_type n) const
    requires is_ritr<I> {
    return {i + n, it[n]};
  }
};
template <class I1, class I2>
constexpr auto operator ==(const enumerate_iterator<I1> &x,
                           const enumerate_iterator<I2> &y)
  ->decltype(x.index() == y.index())
  requires can_apply_equal_to<const I1 &, const I2 &> {
  return x.index() == y.index();
}
template <class I1, class I2, class = synth_3way_result<I1, I2>>
constexpr strong_ordering
operator <=>(const enumerate_iterator<I1> &x,
             const enumerate_iterator<I2> &y) {
  return x.index() <=> y.index();
}
template <class I1, class I2>
constexpr itr_dft<I1> operator -(const enumerate_iterator<I1> &x,
                                 const enumerate_iterator<I2> &y)
  requires can_apply_equal_to<const I1 &, const I2 &> {
  return x.index() - y.index();
}
template <class I1>
constexpr enumerate_iterator<I1>
operator +(itr_dft<I1> n, const enumerate_iterator<I1> &i)
  requires is_ritr<I1> {
  return i + n;
}
struct fo_enumerate_itr {
  template <class I>
  constexpr enumerate_iterator<I> operator ()(I i, itr_dft<I> n) const
    requires is_itr<I> {
    return enumerate_iterator<I>(i, n);
  }
};
inline constexpr fo_enumerate_itr enumerate_itr{};

template <class R>
class enumerate_range : range_fns {
  using this_t = enumerate_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool delay_init = !is_rng_ref<R>;
  static constexpr bool enable_const = is_rng<add_const_t<R>>;

  using cache_data_t = rng_dft<R>;
  using cache_t = conditional_t<delay_init,
                                non_propagating_cache<cache_data_t>,
                                simple_wrapper<cache_data_t>>;

  R r = R{};
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x) {
    x = ssize(r);
  }
  constexpr void init() {
    if constexpr (!delay_init) {
      init_impl(*c);
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init) {
      if (c.empty()) {
        cache_data_t tmp;
        init_impl(tmp);
        c.emplace(tmp);
      }
    }
  }

public:
  enumerate_range() = delete;
  constexpr enumerate_range() requires is_default_constructible_v<R> {
    init();
  }
  ~enumerate_range() = default;
  enumerate_range(const enumerate_range &) = default;
  enumerate_range &operator =(const enumerate_range &) = default;
  enumerate_range(enumerate_range &&) = default;
  enumerate_range &operator =(enumerate_range &&) = default;
  friend constexpr void swap(enumerate_range &x, enumerate_range &y)
    noexcept(is_nothrow_swappable_v<R>)
    requires (is_swappable_v<R> && !is_reference_v<R>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr enumerate_range(R rr) : r(forward<R>(rr)) {
    init();
  }

  constexpr enumerate_iterator<rng_itr<R>> begin() {
    return enumerate_iterator<rng_itr<R>>(begin(r), 0);
  }
  constexpr enumerate_iterator<rng_itr<R>> end() {
    delayed_init();
    return enumerate_iterator<rng_itr<R>>(end(r), *c);
  }
  constexpr enumerate_iterator<rng_itr<add_const_t<R>>> begin() const
    requires (enable_const && !delay_init) {
    return enumerate_iterator<rng_itr<add_const_t<R>>>(begin(r), 0);
  }
  constexpr enumerate_iterator<rng_itr<add_const_t<R>>> end() const
    requires (enable_const && !delay_init) {
    return enumerate_iterator<rng_itr<add_const_t<R>>>(end(r), *c);
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires (enable_const && !delay_init) {
    return empty(r);
  }

  constexpr auto size() {
    delayed_init();
    return to_unsigned(*c);
  }
  constexpr auto size() const requires (enable_const && !delay_init) {
    return to_unsigned(*c);
  }
};
template <class R>
struct is_range_reference<enumerate_range<R>> : is_range_reference<R> {};
struct fo_enumerate_rng {
  template <class R>
  constexpr auto operator ()(R &&r) const {
    return enumerate_range<rng_forward_t<R>>(forward<R>(r));
  }
};
inline constexpr fo_enumerate_rng enumerate_rng{};

}

// exclusive_rotate
// exclusive_rotate_range
namespace re {

struct fo_exclusive_rotate {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_itr<FR> i, OI o) const
    requires is_frng<FR> {
    if (!empty(r)) {
      const auto op = begin(r);
      const auto ed = end(r);
      if (i == ed)
        i = before_end(rng(op, ed));
      auto i2 = i;
      for (;;) {
        ++i2;
        if (i2 == ed)
          i2 = op;
        if (i2 == i)
          break;
        *o = *i2;
        ++o;
      }
    }
    return o;
  }
};
inline constexpr fo_exclusive_rotate exclusive_rotate{};

template <class FI>
class exclusive_rotate_iterator {
  template <class>
  friend class exclusive_rotate_iterator;
  using this_t = exclusive_rotate_iterator;

  FI op = FI{};
  FI wall = FI{}; // wall != ed || (wall == ed && op == ed)
  FI ed = FI{};
  FI it = FI{}; // it != ed || (it == ed && op == ed)

public:
  using value_type = itr_vt<FI>;
  using reference = itr_ref<FI>;
  using pointer = FI;
  using difference_type = itr_dft<FI>;
  using iterator_category
    = conditional_t<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>;

  constexpr exclusive_rotate_iterator() = default;
  ~exclusive_rotate_iterator() = default;
  exclusive_rotate_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  exclusive_rotate_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(exclusive_rotate_iterator &x,
                             exclusive_rotate_iterator &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.op, y.op);
    adl_swap(x.wall, y.wall);
    adl_swap(x.ed, y.ed);
    adl_swap(x.it, y.it);
  }

  constexpr exclusive_rotate_iterator(FI a, FI b, FI c, FI d)
    : op(a), wall(b), ed(c), it(d) {}
  constexpr FI base() const {
    return it;
  }

  template <class FI2>
  constexpr exclusive_rotate_iterator
  (const exclusive_rotate_iterator<FI2> &x)
    requires (!is_same_v<FI2, FI> && is_convertible_v<const FI2 &, FI>)
    : op(x.op), wall(x.wall), ed(x.ed), it(x.it) {}

  constexpr FI operator ->() const
    requires can_apply_to_address<const FI &> {
    return it;
  }
  constexpr reference operator *() const {
    return *it;
  }
  constexpr this_t &operator ++() {
    auto tmp = it;
    ++tmp;
    if (tmp == ed)
      tmp = op;
    it = tmp;
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<FI> {
    auto tmp = it;
    if (tmp == op)
      tmp = ed;
    --tmp;
    it = tmp;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr this_t &operator +=(difference_type i) requires is_ritr<FI> {
    if (i != 0) {
      i %= ed - op;
      if (i > 0) {
        const auto rest = ed - it;
        it = (i < rest) ? (it + i) : (op + (i - rest));
      }
      else {
        i = -i;
        const auto rest = it - op;
        it = (i <= rest) ? (it - i) : (ed - (i - rest));
      }
    }
    return *this;
  }
  constexpr this_t &operator -=(difference_type i) requires is_ritr<FI> {
    return operator +=(-i);
  }
  constexpr this_t operator +(difference_type i) const
    requires is_ritr<FI> {
    return copy(*this) += i;
  }
  constexpr this_t operator -(difference_type i) const
    requires is_ritr<FI> {
    return copy(*this) += -i;
  }
  constexpr reference operator [](difference_type i) const
    requires is_ritr<FI> {
    return *(*this + i);
  }
  template <class FI1, class FI2>
  friend constexpr auto operator -(const exclusive_rotate_iterator<FI1> &,
                                   const exclusive_rotate_iterator<FI2> &)
    ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
    requires is_ritr<FI1> && is_ritr<FI2>;
  template <class FI1, class FI2>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const exclusive_rotate_iterator<FI1> &,
               const exclusive_rotate_iterator<FI2> &);
};
template <class FI1, class FI2>
constexpr auto operator ==(const exclusive_rotate_iterator<FI1> &x,
                           const exclusive_rotate_iterator<FI2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class FI1>
constexpr exclusive_rotate_iterator<FI1>
operator +(itr_dft<FI1> y, const exclusive_rotate_iterator<FI1> &x)
  requires is_ritr<FI1> {
  return x + y;
}
template <class FI1, class FI2>
constexpr auto operator -(const exclusive_rotate_iterator<FI1> &x,
                          const exclusive_rotate_iterator<FI2> &y)
  ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
  requires is_ritr<FI1> && is_ritr<FI2> {
  auto ret = x.it - y.it;
  if (x.it >= x.wall) {
    if (y.it < x.wall)
      ret += (x.op - x.ed);
  }
  else {
    if (y.it >= x.wall)
      ret -= (x.op - x.ed);
  }
  return ret;
}
template <class FI1, class FI2>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const exclusive_rotate_iterator<FI1> &x,
             const exclusive_rotate_iterator<FI2> &y) {
  using o = synth_3way_result<FI1, FI2>;
  if (x.it != y.it) {
    if (x.wall <= x.it)
      return x.wall <= y.it ? synth_3way(x.it, y.it) : o::less;
    else
      return y.it < x.wall ? synth_3way(x.it, y.it) : o::greater;
  }
  return o::equivalent;
}
struct fo_exclusive_rotate_itr {
  template <class FI>
  constexpr auto operator ()(FI op, FI wall, FI ed, FI i) const {
    return exclusive_rotate_iterator<FI>(op, wall, ed, i);
  }
};
inline constexpr fo_exclusive_rotate_itr exclusive_rotate_itr{};

template <class FR>
class exclusive_rotate_range : range_fns {
  using this_t = exclusive_rotate_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool enable_const = is_rng<add_const_t<FR>>;
  static constexpr bool delay_init = !is_rng_ref<FR>;

  struct cache_data_t {
    rng_itr<FR> wall;
    rng_itr<FR> prev_wall;
  };
  struct cache_t1 {
    simple_wrapper<cache_data_t> c;
  };
  struct cache_t2 {
    rng_dft<FR> n = 0;
    non_propagating_cache<cache_data_t> c;
  };
  using cache_t = conditional_t<delay_init, cache_t2, cache_t1>;

  FR r = FR{};
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x, rng_itr<FR> i) {
    const auto f = begin(r);
    const auto l = end(r);
    if (f != l) {
      rng_itr<FR> y = i;
      if (y == l)
        y = before_end(rng(f, l));
      rng_itr<FR> wall = y;
      ++wall;
      if (wall == l)
        wall = f;
      x.wall = wall;
      x.prev_wall = y;
    }
    else {
      x.wall = f;
      x.prev_wall = f;
    }
  }
  constexpr void init(rng_itr<FR> i) {
    if constexpr (!delay_init) {
      init_impl(*c.c, i);
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init) {
      if (c.c.empty()) {
        cache_data_t tmp;
        init_impl(tmp, next(begin(r), c.n, end(r)));
        c.c.emplace(tmp);
      }
    }
  }

public:
  exclusive_rotate_range() = delete;
  constexpr exclusive_rotate_range()
    requires is_default_constructible_v<FR> {
    init(end(r));
  }
  ~exclusive_rotate_range() = default;
  exclusive_rotate_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  exclusive_rotate_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FR>)
    requires (is_swappable_v<FR> && !is_reference_v<FR>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr exclusive_rotate_range(FR rr, rng_itr<FR> i) requires (!delay_init)
    : r(forward<FR>(rr)) {
    init(i);
  }
  constexpr exclusive_rotate_range(FR rr, rng_dft<FR> d)
    : r(forward<FR>(rr)) {
    if constexpr (delay_init) {
      c.n = d;
    }
    else {
      init(nth(r, d));
    }
  }

  constexpr auto begin() {
    delayed_init();
    return exclusive_rotate_itr(begin(r), c.c->wall, end(r), c.c->wall);
  }
  constexpr auto end() {
    delayed_init();
    return exclusive_rotate_itr(begin(r), c.c->wall, end(r), c.c->prev_wall);
  }
  constexpr auto begin() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return exclusive_rotate_itr(begin(r), citer_t(c.c->wall), end(r),
                                citer_t(c.c->wall));
  }
  constexpr auto end() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return exclusive_rotate_itr(begin(r), citer_t(c.c->wall), end(r),
                                citer_t(c.c->prev_wall));
  }

  constexpr bool empty() {
    return empty(r) || (begin(r) == end(r) || next(begin(r)) == end(r));
  }
  constexpr bool empty() const requires (enable_const && !delay_init) {
    return empty(r) || (begin(r) == end(r) || next(begin(r)) == end(r));
  }

  constexpr rng_szt<FR> size() requires rng_is_sized<FR> {
    const auto n = size(r);
    return (n == 0) ? n : (n - 1u);
  }
  constexpr rng_szt<FR> size() const
    requires (enable_const && !delay_init
              && rng_is_sized<add_const_t<FR>>) {
    const auto n = size(r);
    return (n == 0) ? n : (n - 1u);
  }
};
template <class FR>
struct is_range_reference<exclusive_rotate_range<FR>>
  : is_range_reference<FR> {};
struct fo_exclusive_rotate_rng {
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T i) const
    requires is_frng<FR> && is_integral_v<T> {
    return exclusive_rotate_range<rng_forward_t<FR>>
      (forward<FR>(r), static_cast<rng_dft<FR>>(i));
  }
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T i) const
    requires (is_frng<FR> && is_rng_ref<FR> && !is_integral_v<T>) {
    return exclusive_rotate_range<rng_forward_t<FR>>
      (forward<FR>(r), static_cast<rng_itr<FR>>(i));
  }
};
inline constexpr fo_exclusive_rotate_rng exclusive_rotate_rng{};

}

// rotate_range
namespace re {

template <class FI>
class rotate_iterator {
  using this_t = rotate_iterator;

  FI f = FI{};
  FI wall = FI{}; // wall != l || (wall == l && f == l)
  FI l = FI{};
  FI it = FI{};

public:
  using value_type = itr_vt<FI>;
  using reference = itr_ref<FI>;
  using pointer = FI;
  using difference_type = itr_dft<FI>;
  using iterator_category
    = conditional_t<is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>;

  rotate_iterator() = default;
  ~rotate_iterator() = default;
  rotate_iterator(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  rotate_iterator(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.f, y.f);
    adl_swap(x.l, y.l);
    adl_swap(x.wall, y.wall);
    adl_swap(x.it, y.it);
  }

  template <class FI2>
  constexpr rotate_iterator(const rotate_iterator<FI2> &x)
    requires (!is_same_v<FI2, FI> && is_convertible_v<const FI2 &, FI>)
    : f(x.f), l(x.l), wall(x.wall), it(x.it) {}

  constexpr rotate_iterator(FI a, FI b, FI c, FI d)
    : f(a), wall(b), l(c), it(d) {}
  constexpr FI base() const {
    return it;
  }

  constexpr reference operator *() const {
    return *it;
  }
  constexpr FI operator ->() const requires can_apply_to_address<FI> {
    return it;
  }
  constexpr this_t &operator ++() {
    auto tmp = it;
    ++tmp;
    if (tmp == l)
      tmp = f;
    if (tmp == wall)
      tmp = l;
    it = tmp;
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<FI> {
    auto tmp = it;
    if (tmp == l)
      tmp = wall;
    if (tmp == f)
      tmp = l;
    --tmp;
    it = tmp;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr reference operator [](difference_type i) const
    requires is_ritr<FI> {
    return *(*this + i);
  }
  constexpr this_t &operator +=(difference_type i) requires is_ritr<FI> {
    if (i != 0) {
      if (i > 0) {
        const auto rest = l - it;
        auto tmp = (i < rest) ? (it + i) : (f + (i - rest));
        if (tmp == wall)
          tmp = l;
        it = tmp;
      }
      else {
        auto tmp = it;
        if (tmp == l)
          tmp = wall;
        i = -i;
        const auto rest = tmp - f;
        it = (i <= rest) ? (tmp - i) : (l - (i - rest));
      }
    }
    return *this;
  }
  constexpr this_t &operator -=(difference_type i) requires is_ritr<FI> {
    return operator +=(-i);
  }
  constexpr this_t operator +(difference_type i) const
    requires is_ritr<FI> {
    return copy(*this) += i;
  }
  constexpr this_t operator -(difference_type i) const
    requires is_ritr<FI> {
    return copy(*this) -= i;
  }
  template <class FI1, class FI2>
  friend constexpr auto operator -(const rotate_iterator<FI1> &,
                                   const rotate_iterator<FI2> &)
    ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
    requires is_ritr<FI1> && is_ritr<FI2>;
  template <class FI1, class FI2>
  friend constexpr synth_3way_result<FI1, FI2>
  operator <=>(const rotate_iterator<FI1> &,
               const rotate_iterator<FI2> &);
};
template <class FI1, class FI2>
constexpr auto operator ==(const rotate_iterator<FI1> &x,
                           const rotate_iterator<FI2> &y)
  ->decltype(x.base() == y.base()) {
  return x.base() == y.base();
}
template <class FI1>
constexpr rotate_iterator<FI1>
operator +(itr_dft<FI1> y, const rotate_iterator<FI1> &x)
  requires is_ritr<FI1> {
  return x + y;
}
template <class FI1, class FI2>
constexpr auto operator -(const rotate_iterator<FI1> &x,
                          const rotate_iterator<FI2> &y)
  ->decltype(declval<const FI1 &>() - declval<const FI2 &>())
  requires is_ritr<FI1> && is_ritr<FI2> {
  auto ret = x.it - y.it;

  if (x.it >= x.wall) {
    if (x.it != x.l)
      ret -= (x.wall - x.f);
  }
  else
    ret += (x.l - x.wall);

  if (y.it >= x.wall) {
    if (y.it != x.l)
      ret += (x.wall - x.f);
  }
  else
    ret -= (x.l - x.wall);
  
  return ret;
}
template <class FI1, class FI2>
constexpr synth_3way_result<FI1, FI2>
operator <=>(const rotate_iterator<FI1> &x,
             const rotate_iterator<FI2> &y) {
  using o = synth_3way_result<FI1, FI2>;
  if (x.it != y.it) {
    if (x.wall <= x.it) {
      if (x.it == x.l)
        return o::greater;
      else
        return (x.wall <= y.it && y.it != x.l)
          ? synth_3way(x.it, y.it) : o::less;
    }
    else {
      if (y.it < x.wall)
        return synth_3way(x.it, y.it);
      else
        return y.it != x.l ? o::greater : o::less;
    }
  }
  return o::equivalent;
}
struct fo_rotate_itr {
  template <class FI>
  constexpr rotate_iterator<FI>
  operator ()(FI op, FI wall, FI ed, FI i) const {
    return rotate_iterator<FI>(op, wall, ed, i);
  }
};
inline constexpr fo_rotate_itr rotate_itr{};

template <class FR>
class rotate_range : range_fns {
  using this_t = rotate_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool enable_const = is_rng<add_const_t<FR>>;
  static constexpr bool delay_init = !is_rng_ref<FR>;

  using cache_data_t = rng_itr<FR>;
  struct cache_t1 {
    simple_wrapper<cache_data_t> c;
  };
  struct cache_t2 {
    rng_dft<FR> n = 0;
    non_propagating_cache<cache_data_t> c;
  };
  using cache_t = conditional_t<delay_init, cache_t2, cache_t1>;

  FR r = FR{};
  cache_t c = cache_t{};

  constexpr void init(rng_itr<FR> i) {
    if constexpr (!delay_init) {
      if (i == end(r))
        i = begin(r);
      *c.c = i;
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init) {
      if (c.c.empty()) {
        auto it = begin(r);
        if (!empty(r)) {
          advance(it, c.n, end(r));
          if (it == end(r))
            it = begin(r);
        }
        c.c.emplace(it);
      }
    }
  }

public:
  rotate_range() = delete;
  constexpr rotate_range()
    requires is_default_constructible_v<FR> {
    init(begin(r));
  }
  ~rotate_range() = default;
  rotate_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  rotate_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FR>)
    requires (is_swappable_v<FR> && !is_reference_v<FR>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr rotate_range(FR rr, rng_itr<FR> i) requires (!delay_init)
    : r(forward<FR>(rr)) {
    init(i);
  }
  constexpr rotate_range(FR rr, rng_dft<FR> d) : r(forward<FR>(rr)) {
    if constexpr (delay_init) {
      c.n = d;
    }
    else {
      init(nth(r, d));
    }
  }

  constexpr auto begin() {
    delayed_init();
    return rotate_itr(begin(r), *c.c, end(r), *c.c);
  }
  constexpr auto end() {
    delayed_init();
    return rotate_itr(begin(r), *c.c, end(r), end(r));
  }
  constexpr auto begin() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return rotate_itr(begin(r), citer_t(*c.c), end(r), citer_t(*c.c));
  }
  constexpr auto end() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return rotate_itr(begin(r), citer_t(*c.c), end(r), end(r));
  }

  constexpr bool empty() {
    return empty(r);
  }
  constexpr bool empty() const requires (enable_const && !delay_init) {
    return empty(r);
  }

  constexpr rng_szt<FR> size() requires rng_is_sized<FR> {
    return size(r);
  }
  constexpr rng_szt<FR> size() const
    requires (enable_const && !delay_init
              && rng_is_sized<add_const_t<FR>>) {
    return size(r);
  }
};
template <class FR>
struct is_range_reference<rotate_range<FR>> : is_range_reference<FR> {};
struct fo_rotate_rng {
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T i) const
    requires is_frng<FR> && is_integral_v<T> {
    return rotate_range<rng_forward_t<FR>>
      (forward<FR>(r), static_cast<rng_dft<FR>>(i));
  }
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T i) const
    requires (is_frng<FR> && is_rng_ref<FR> && !is_integral_v<T>) {
    return rotate_range<rng_forward_t<FR>>
      (forward<FR>(r), static_cast<rng_itr<FR>>(i));
  }
};
inline constexpr fo_rotate_rng rotate_rng{};

}

// loop
// loop_range
namespace re {

struct fo_loop {
  template <class FR, class OI>
  constexpr OI operator ()(FR &&r, rng_itr<FR> it, rng_dft<FR> n, OI o) const
    requires is_frng<FR> {
    const auto op = begin(r);
    const auto ed = end(r);
    if (op != ed) {
      if (it == ed)
        it = op;
      for (; n != 0; --n) {
        *o = *it;
        ++o;
        ++it;
        if (it == ed)
          it = op;
      }
    }
    return o;
  }
};
inline constexpr fo_loop loop{};

template <class FI>
class loop_iterator {
  template <class>
  friend class loop_iterator;

  using this_t = loop_iterator;

  FI f;
  FI l;
  FI it;
  itr_dft<FI> n;

public:
  using value_type = itr_vt<FI>;
  using reference = itr_ref<FI>;
  using pointer = FI;
  using difference_type = itr_dft<FI>;
  using iterator_category = conditional_t
    <is_ritr<FI>, random_access_iterator_tag, itr_ctg<FI>>;

  loop_iterator() = default;
  ~loop_iterator() = default;
  loop_iterator(const this_t &) = default;
  loop_iterator &operator =(const this_t &) = default;
  loop_iterator(this_t &&) = default;
  loop_iterator &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FI>) {
    adl_swap(x.f, y.f);
    adl_swap(x.l, y.l);
    adl_swap(x.it, y.it);
    adl_swap(x.n, y.n);
  }

  template <class FI2>
  constexpr loop_iterator(const loop_iterator<FI2> &x)
    requires (!is_same_v<FI, FI2> && is_convertible_v<const FI2 &, FI>)
    : f(x.f), l(x.l), it(x.it), n(x.n) {}

  constexpr loop_iterator(FI f, FI l, FI it, itr_dft<FI> n)
    : f(f), l(l), it(it), n(n) {}
  constexpr FI base() const {
    return it;
  }
  constexpr auto index() const {
    return n;
  }

  constexpr FI operator ->() const requires can_apply_to_address<FI> {
    return it;
  }
  constexpr reference operator *() const {
    return *it;
  }
  constexpr this_t &operator ++() {
    auto i = it;
    ++i;
    if (i == l)
      i = f;
    it = i;
    ++n;
    return *this;
  }
  constexpr this_t operator ++(int) {
    return iter_post_increment(*this);
  }

  constexpr this_t &operator --() requires is_bitr<FI> {
    auto i = it;
    if (i == f)
      i = l;
    --i;
    it = i;
    --n;
    return *this;
  }
  constexpr this_t operator --(int) requires is_bitr<FI> {
    return iter_post_decrement(*this);
  }

  constexpr loop_iterator &operator +=(difference_type k)
    requires is_ritr<FI> {
    if (k != 0) {
      if (k >= (l - it))
        it = f + (k + (it - f)) % (l - f);
      else if (k < (f - it)) {
        const auto tmp = prev(l);
        it = tmp - (-k + (tmp - it)) % (l - f);
      }
      else
        it += k;
      n += k;
    }
    return *this;
  }
  constexpr loop_iterator &operator -=(difference_type k)
    requires is_ritr<FI> {
    return operator +=(-k);
  }
  constexpr loop_iterator operator +(difference_type k) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(k);
  }
  constexpr loop_iterator operator -(difference_type k) const
    requires is_ritr<FI> {
    return copy(*this).operator +=(-k);
  }
  constexpr reference operator [](difference_type k) const
    requires is_ritr<FI> {
    return *(*this + k);
  }
};
template <class FI1, class FI2>
constexpr auto operator ==(const loop_iterator<FI1> &x,
                           const loop_iterator<FI2> &y)
  ->decltype(x.index() == y.index()) {
  return x.index() == y.index();
}
template <class FI1, class FI2>
constexpr auto operator -(const loop_iterator<FI1> &x,
                          const loop_iterator<FI2> &y)
  requires can_apply_equal_to<const FI1 &, const FI2 &> {
  return x.index() - y.index();
}
template <class FI>
constexpr loop_iterator<FI>
operator +(itr_dft<FI> k, const loop_iterator<FI> &x) requires is_ritr<FI> {
  return x.operator +(k);
}
template <class FI1, class FI2>
constexpr strong_ordering operator <=>(const loop_iterator<FI1> &x,
                                       const loop_iterator<FI2> &y)
  requires can_apply_equal_to<const FI1 &, const FI2 &> {
  return x.index() <=> y.index();
}
struct fo_loop_itr {
  template <class FI>
  constexpr auto operator ()(FI f, FI l, FI it, itr_dft<FI> n) const {
    return loop_iterator<FI>(f, l, it, n);
  }
};
inline constexpr fo_loop_itr loop_itr{};

template <class FR>
class loop_range : range_fns {
  using this_t = loop_range;

  using range_fns::begin;
  using range_fns::end;
  using range_fns::empty;
  using range_fns::size;

  static constexpr bool enable_const = is_rng<add_const_t<FR>>;
  static constexpr bool delay_init = !is_rng_ref<FR>;

  using cache_data_t = pair<rng_itr<FR>, rng_itr<FR>>;
  struct cache_t1 {
    simple_wrapper<cache_data_t> c;
    rng_dft<FR> n = 0;
  };
  struct cache_t2 {
    rng_dft<FR> it_n = 0;
    non_propagating_cache<cache_data_t> c;
    rng_dft<FR> n = 0;
  };
  using cache_t = conditional_t<delay_init, cache_t2, cache_t1>;

  FR r = FR{};
  cache_t c = cache_t{};

  constexpr void init_impl(cache_data_t &x, rng_itr<FR> i, rng_dft<FR> n) {
    if constexpr (is_brng<FR>) {
      if (i == end(r))
        i = begin(r);
      const auto d = distance(begin(r), i);
      const auto dd = empty(r) ? d : ((d + n) % ssize(r));
      const auto i2 = nth(r, dd);
      x.first = i;
      x.second = i2;
    }
    else {
      if (i == end(r))
        i = begin(r);
      x.first = i;
      x.second = i;
    }
  }
  constexpr void init(rng_itr<FR> i, rng_dft<FR> nn) {
    if constexpr (!delay_init) {
      c.n = empty(r) ? 0 : nn;
      init_impl(*c.c, i, c.n);
    }
  }
  constexpr void delayed_init() {
    if constexpr (delay_init) {
      if (c.c.empty()) {
        auto it = begin(r);
        if (empty(r))
          c.n = 0;
        else {
          advance(it, c.it_n, end(r));
          if (it == end(r))
            it = begin(r);
        }
        cache_data_t tmp;
        init_impl(tmp, it, c.n);
        c.c.emplace(tmp);
      }
    }
  }

public:
  loop_range() = delete;
  constexpr loop_range() requires is_default_constructible_v<FR> {
    init(begin(r), 0);
  }
  ~loop_range() = default;
  loop_range(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  loop_range(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend constexpr void swap(this_t &x, this_t &y)
    noexcept(is_nothrow_swappable_v<FR>)
    requires (is_swappable_v<FR> && !is_reference_v<FR>) {
    adl_swap(x.r, y.r);
    adl_swap(x.c, y.c);
  }

  constexpr loop_range(FR rr, rng_itr<FR> i, rng_dft<FR> d)
    requires (!delay_init)
    : r(forward<FR>(rr)) {
    init(i, d);
  }
  constexpr loop_range(FR rr, rng_dft<FR> it_d, rng_dft<FR> d)
    : r(forward<FR>(rr)) {
    if constexpr (delay_init) {
      c.it_n = it_d;
      c.n = d;
    }
    else {
      init(nth(r, it_d), d);
    }
  }

  constexpr auto begin() {
    delayed_init();
    return loop_itr(begin(r), end(r), c.c->first, 0);
  }
  constexpr auto end() {
    delayed_init();
    return loop_itr(begin(r), end(r), c.c->second, c.n);
  }
  constexpr auto begin() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return loop_itr(begin(r), end(r), citer_t(c.c->first), 0);
  }
  constexpr auto end() const requires (enable_const && !delay_init) {
    using citer_t = decltype(begin(r));
    return loop_itr(begin(r), end(r), citer_t(c.c->second), c.n);
  }

  constexpr bool empty() {
    delayed_init();
    return c.n == 0;
  }
  constexpr bool empty() const requires (enable_const && !delay_init) {
    return c.n == 0;
  }

  constexpr rng_szt<FR> size() {
    delayed_init();
    return c.n;
  }
  constexpr rng_szt<FR> size() const
    requires (enable_const && !delay_init) {
    return c.n;
  }
};
template <class FR>
struct is_range_reference<loop_range<FR>> : is_range_reference<FR> {};
struct fo_loop_rng {
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T i, rng_dft<FR> n) const
    requires (is_frng<FR> && is_rng_ref<FR> && !is_integral_v<T>) {
    return loop_range<rng_forward_t<FR>>(forward<FR>(r),
                                         static_cast<rng_itr<FR>>(i),
                                         n);
  }
  template <class FR, class T>
  constexpr auto operator ()(FR &&r, T d, rng_dft<FR> n) const
    requires is_frng<FR> && is_integral_v<T> {
    return loop_range<rng_forward_t<FR>>(forward<FR>(r),
                                         static_cast<rng_dft<FR>>(d),
                                         n);
  }
  template <class FR>
  constexpr auto operator ()(FR &&r, rng_dft<FR> n) const
    requires is_frng<FR> {
    return loop_range<rng_forward_t<FR>>(forward<FR>(r), rng_dft<FR>(0), n);
  }
};
inline constexpr fo_loop_rng loop_rng{};

}

#endif
