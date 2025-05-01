//#define RE_NOEXCEPT
#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>

#include <cassert>

namespace re::inner::test {

void test_matrix0() {
  // basic sequence container operations
  // width
  // height
  {
    matrix<int> m0;
    assert(m0.range().begin() == m0.range().end());
    assert(as_const(m0).range().begin() == as_const(m0).range().end());
    assert(m0.range().size() == 0u);
    assert(m0.range().empty());
    assert(m0.empty());
    assert(m0.width() == 0 && m0.height() == 0);

    matrix<int> m(3, 2, seq(1, 2, 3, 4, 5, 6));
    assert(equal(m.range(), irng(1, 7)));
    assert(m.range().begin() != m.range().end());
    assert(as_const(m).range().begin() != as_const(m).range().end());
    assert(m.range().size() == 6);
    assert(!m.range().empty());
    assert(!m.empty());
    assert(m.width() == 3 && m.height() == 2);
  }

  // smf
  {
    matrix<int> m1, m2(m1), m3(move(m1));
    (m1 = m2) = move(m3);
    adl_swap(m2, m3);
    assert(m1.empty() && m1.width() == 0 && m1.height() == 0);
    assert(m2.empty() && m2.width() == 0 && m2.height() == 0);
    assert(m3.empty() && m3.width() == 0 && m3.height() == 0);
  }

  // ==
  {
    matrix<int> m0;
    matrix<int> m(1, 3);
    matrix<int> m2(3, 1);
    assert(m0 == m0 && m == m && m2 == m2);
    assert(m0 != m);
    assert(m != m2);
  }

  // allocator_type
  // get_allocator
  {
    static_assert(!has_allocator_type<matrix<int, local_vector<int, 10>>>);
    matrix<int> m(3, 3, 1);
    assert(equal(m.range(), rng(9, 1)));
    assert(m.get_allocator() == default_allocator<int>{});
  }

  // matrix(a)
  // matrix(m, a)
  // matrix(m_rv, a)
  {
    using mat_t = matrix<int, vector<int, stateful_test_allocator<int>>>;
    stateful_test_allocator<int> a;
    mat_t m(a);
    assert(m.get_allocator() == a);
    m.resize(3, 3, 1);
    mat_t m2(m, a);
    assert(m2.get_allocator() == a);
    assert(equal(m2.range(), rng(9, 1)));
    mat_t m3(move(m), a);
    assert(m3.get_allocator() == a);
    assert(equal(m3.range(), rng(9, 1)));
    assert(m.empty() && m.width() == 0 && m.height() == 0);
  }

  // matrix(w, h)
  {
    matrix<int> m(3, 2);
    assert(equal(m.range(), rng(6, 0)));
    assert(m.width() == 3 && m.height() == 2);
  }
  // matrix(w, h, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, a);
    assert(m.get_allocator() == a);
    assert(equal(m.range(), rng(6, 0)));
    assert(m.width() == 3 && m.height() == 2);
  }

  // matrix(w, h, v)
  {
    matrix<int> m(3, 2, 1);
    assert(equal(m.range(), rng(6, 1)));
    assert(m.width() == 3 && m.height() == 2);
  }
  // matrix(w, h, v, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, 1, a);
    assert(m.get_allocator() == a);
    assert(equal(m.range(), rng(6, 1)));
    assert(m.width() == 3 && m.height() == 2);
  }

  // matrix(w, h, r)
  {
    matrix<int> m(3, 2, seq(1, 2, 3));
    matrix<int> m2(3, 2, seq(1, 2, 3, 4, 5, 6));
    matrix<int> m3(3, 2, seq(1, 2, 3, 4, 5, 6, 7, 8));
    assert(m.width() == 3 && m.height() == 2);
    assert(equal(m.range(), seq(1, 2, 3, 0, 0, 0)));
    assert(m2.width() == 3 && m2.height() == 2);
    assert(equal(m2.range(), irng(1, 7)));
    assert(m3 == m2);
  }
  // matrix(w, h, r, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, seq(1, 2, 3), a);
    matrix<int, vector<int, alloc_t>> m2(3, 2, seq(1, 2, 3, 4, 5, 6), a);
    matrix<int, vector<int, alloc_t>> m3(3, 2, seq(1, 2, 3, 4, 5, 6, 7, 8), a);
    assert(m.width() == 3 && m.height() == 2);
    assert(equal(m.range(), seq(1, 2, 3, 0, 0, 0)));
    assert(m2.width() == 3 && m2.height() == 2);
    assert(equal(m2.range(), irng(1, 7)));
    assert(m3 == m2);
    assert(m.get_allocator() == a);
    assert(m2.get_allocator() == a);
    assert(m3.get_allocator() == a);
  }

  // explicit matrix(const matrix<T2, C2> &)
  // operator =(const matrix<T2, C2> &)
  {
    matrix<int> m(3, 2, 1);
    matrix<long> m2(m);
    assert(m2.width() == 3 && m2.height() == 2);
    assert(equal(m2.range(), rng(6, 1)));
    matrix<long> m3;
    m3 = m;
    assert(m3.width() == 3 && m3.height() == 2);
    assert(equal(m3.range(), rng(6, 1)));
  }
  // matrix(const matrix<T2, C2> &, const alloc_t &)
  {
    using alloc_t = stateful_test_allocator<int>;
    using alloc_tt = stateful_test_allocator<long>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, 1);
    matrix<long, vector<long, alloc_tt>> m2(m, a);
    assert(m2.get_allocator() == a);
    assert(m2.width() == 3u && m2.height() == 2u);
    assert(equal(m2.range(), rng(6, 1)));
  }

  // explicit matrix(matrix<T2, C2> &&)
  // operator =(matrix<T2, C2> &&)
  {
    matrix<int> m(3, 2, 1);
    matrix<long> m2(move(m));
    assert(m2.width() == 3u && m2.height() == 2u);
    assert(equal(m2.range(), rng(6, 1)));
    assert(m.empty() && m.width() == 0u && m.height() == 0u);
    matrix<long long> m3;
    m3 = move(m2);
    assert(m3.width() == 3u && m3.height() == 2u);
    assert(equal(m3.range(), rng(6, 1)));
    assert(m2.empty() && m2.width() == 0u && m2.height() == 0u);
  }
  // matrix(matrix<T2, C2> &&, const alloc_t &)
  {
    using alloc_t = stateful_test_allocator<int>;
    using alloc_tt = stateful_test_allocator<long>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, 1);
    matrix<long, vector<long, alloc_tt>> m2(move(m), a);
    assert(m2.get_allocator() == a);
    assert(m2.width() == 3u && m2.height() == 2u);
    assert(equal(m2.range(), rng(6, 1)));
    assert(m.empty() && m.width() == 0u && m.height() == 0u);
  }

  // matrix(w, h, m)
  // matrix(w, h, m, fll)
  // assign(w, h, m, fll)
  {
    matrix<int> m(3, 3, seq(1, 2, 3,
                            4, 5, 6,
                            7, 8, 9));
    matrix<int> m2(2, 2, m);
    assert(m2.width() == 2 && m2.height() == 2);
    assert(equal(m2.range(), seq(1, 2, 4, 5)));
    matrix<int> m3(3, 3, m);
    assert(m3 == m);
    matrix<int> m4(4, 4, m);
    assert(m4.width() == 4 && m4.height() == 4);
    assert(equal(m4.range(), seq(1, 2, 3, 0,
                                 4, 5, 6, 0,
                                 7, 8, 9, 0,
                                 0, 0, 0, 0)));
    matrix<int> m44(4, 4, m, -1);
    assert(m44.width() == 4 && m44.height() == 4);
    assert(equal(m44.range(), seq(1, 2, 3, -1,
                                  4, 5, 6, -1,
                                  7, 8, 9, -1,
                                  -1, -1, -1, -1)));
    m44.assign(2, 2, m44);
    assert(m44.width() == 2 && m44.height() == 2);
    assert(equal(m44.range(), seq(1, 2,
                                  4, 5)));
    m44.assign(3, 3, m44, -1);
    assert(m44.width() == 3 && m44.height() == 3);
    assert(equal(m44.range(), seq(1, 2, -1,
                                  4, 5, -1,
                                  -1, -1, -1)));
  }
  // matrix(w, h, m, a)
  // matrix(w, h, m, fll, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    alloc_t a;
    matrix<int, vector<int, alloc_t>> m(3, 2, 1);
    matrix<int, vector<int, alloc_t>> m2(1, 1, m, a);
    assert(m2.width() == 1 && m2.height() == 1);
    assert(equal(m2.range(), single_rng(1)));
    assert(m2.get_allocator() == a);

    matrix<int, vector<int, alloc_t>> m3(3, 3, m, -1, a);
    assert(m3.width() == 3 && m3.height() == 3);
    assert(equal(m3.range(), seq(1, 1, 1,
                                 1, 1, 1,
                                 -1, -1, -1)));
    assert(m3.get_allocator() == a);
  }

  // matrix(w, h, m_rv)
  // matrix(w, h, m_rv, fll)
  // assign(w, h, m_rv, fll)
  {
    {
      matrix<int> m(3, 3, seq(1, 2, 3,
                              4, 5, 6,
                              7, 8, 9));
      matrix<int> m2(2, 2, move(m));
      assert(m2.width() == 2 && m2.height() == 2);
      assert(equal(m2.range(), seq(1, 2, 4, 5)));
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
    {
      matrix<int> m(3, 3, seq(1, 2, 3,
                              4, 5, 6,
                              7, 8, 9));
      matrix<int> m3(3, 3, move(m));
      assert(m3 == matrix<int>(3, 3, seq(1, 2, 3,
                                         4, 5, 6,
                                         7, 8, 9)));
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
    {
      matrix<int> m(3, 3, seq(1, 2, 3,
                              4, 5, 6,
                              7, 8, 9));
      matrix<int> m4(4, 4, move(m));
      assert(m4.width() == 4 && m4.height() == 4);
      assert(equal(m4.range(), seq(1, 2, 3, 0,
                                   4, 5, 6, 0,
                                   7, 8, 9, 0,
                                   0, 0, 0, 0)));
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
    {
      matrix<int> m(3, 3, seq(1, 2, 3,
                              4, 5, 6,
                              7, 8, 9));
      matrix<int> m44(4, 4, move(m), -1);
      assert(m44.width() == 4 && m44.height() == 4);
      assert(equal(m44.range(), seq(1, 2, 3, -1,
                                    4, 5, 6, -1,
                                    7, 8, 9, -1,
                                    -1, -1, -1, -1)));
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
    {
      matrix<int> m(2, 2, seq(1, 2,
                              3, 4));
      m.assign(3, 3, move(m), -1);
      assert(m.width() == 3 && m.height() == 3);
      assert(equal(m.range(), seq(1, 2, -1,
                                  3, 4, -1,
                                  -1, -1, -1)));
      matrix<int> m2(3, 3, seq(1, 2, 3,
                               4, 5, 6,
                               7, 8, 9));
      m.assign(3, 3, move(m2), -1);
      assert(m.width() == 3 && m.height() == 3);
      assert(equal(m.range(), seq(1, 2, 3,
                                  4, 5, 6,
                                  7, 8, 9)));
      assert(m2.empty() && m2.width() == 0 && m2.height() == 0);
    }
  }
  // matrix(w, h, m_rv, a)
  // matrix(w, h, m_rv, fll, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    alloc_t a;
    {
      matrix<int, vector<int, alloc_t>> m(3, 2, 1);
      matrix<int, vector<int, alloc_t>> m2(1, 1, move(m), a);
      assert(m2.width() == 1 && m2.height() == 1);
      assert(equal(m2.range(), single_rng(1)));
      assert(m2.get_allocator() == a);
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
    {
      matrix<int, vector<int, alloc_t>> m(3, 2, 1);
      matrix<int, vector<int, alloc_t>> m3(3, 3, move(m), -1, a);
      assert(m3.width() == 3 && m3.height() == 3);
      assert(equal(m3.range(), seq(1, 1, 1,
                                   1, 1, 1,
                                   -1, -1, -1)));
      assert(m3.get_allocator() == a);
      assert(m.empty() && m.width() == 0 && m.height() == 0);
    }
  }

  // clear()
  // resize(w, h, fll = {})
  {
    matrix<int> m(3, 2, 1);
    m.clear();
    assert(m.empty() && m.width() == 0 && m.height() == 0);
    m.resize(2, 2, 1);
    assert(m.width() == 2 && m.height() == 2);
    assert(equal(m.range(), rng(4, 1)));
    m.resize(1, 1);
    assert(m.width() == 1 && m.height() == 1);
    assert(equal(m.range(), single_rng(1)));
  }

  // row(n)
  // rows()
  {
    matrix<int> m;
    assert(m.rows().empty());

    matrix<int> m2(2, 2, seq(1, 2,
                             3, 4));
    assert(equal(m2.rows(), seq(seq(1, 2), seq(3, 4)), equal));
    assert(equal(as_const(m2).rows(), seq(seq(1, 2), seq(3, 4)), equal));
  }

  // column(n)
  // columns()
  {
    matrix<int> m;
    assert(m.columns().empty());

    matrix<int> m2(2, 2, seq(1, 2,
                             3, 4));
    assert(equal(m2.columns(), seq(seq(1, 3), seq(2, 4)), equal));
    assert(equal(as_const(m2).columns(), seq(seq(1, 3), seq(2, 4)), equal));
  }

  // sub_range(x, y, w, h)
  {
    matrix<int> m(3, 3, seq(1, 2, 3,
                            4, 5, 6,
                            7, 8, 9));
    assert(equal(m.sub_range(1, 1, 2, 2), seq(5, 6, 8, 9)));
    assert(equal(as_const(m).sub_range(1, 1, 2, 2), seq(5, 6, 8, 9)));
    assert(equal(m.sub_range(1, 1, 0, 0), empty_rng<int>()));
    assert(equal(as_const(m).sub_range(1, 1, 0, 0), empty_rng<int>()));

    assert(equal(m.sub_range(1, 1, 2, 2), seq(5, 6, 8, 9)));
    assert(equal(m.sub_range(3, 3, 0, 3), empty_rng<int>()));
    assert(equal(m.sub_range(3, 3, 3, 0), empty_rng<int>()));
    assert(equal(m.sub_range(3, 3, 0, 0), empty_rng<int>()));

    assert(equal(m.sub_range(0, 1, 3, 2), irng(4, 10)));
  }

  // fill(x, y, w, h, z)
  {
    matrix<int> m(3, 3, seq(1, 2, 3,
                            4, 5, 6,
                            7, 8, 9));
    const auto m_bk = m;
    m.fill(3, 3, 0, 4, 1);
    m.fill(3, 3, 4, 0, 1);
    m.fill(3, 3, 0, 0, 1);
    assert(m == m_bk);
    m.fill(1, 1, 2, 2, 0);
    assert(m == matrix<int>(3, 3, seq(1, 2, 3,
                                      4, 0, 0,
                                      7, 0, 0)));
  }

  // front()
  // back()
  // []
  // left_top()
  // left_bottom()
  // right_top()
  // right_bottom()
  // ()(i, j)
  // iter(i, j)
  // includes_point(i, j)
  {
    matrix<int> m(2, 2, seq(1, 2,
                            3, 4));
    assert(m.left_top() == 1 && as_const(m).left_top() == 1);
    assert(m.left_bottom() == 3 && as_const(m).left_bottom() == 3);
    assert(m.right_top() == 2 && as_const(m).right_top() == 2);
    assert(m.right_bottom() == 4 && as_const(m).right_bottom() == 4);
    assert(m(0, 0) == 1 && as_const(m)(0, 0) == 1);
    assert(m(1, 0) == 2 && as_const(m)(1, 0) == 2);
    assert(m(0, 1) == 3 && as_const(m)(0, 1) == 3);
    assert(m(1, 1) == 4 && as_const(m)(1, 1) == 4);
    assert(*m.iter(0, 0) == 1 && *as_const(m).iter(0, 0) == 1);
    assert(*m.iter(1, 0) == 2 && *as_const(m).iter(1, 0) == 2);
    assert(*m.iter(0, 1) == 3 && *as_const(m).iter(0, 1) == 3);
    assert(*m.iter(1, 1) == 4 && *as_const(m).iter(1, 1) == 4);
    assert(matrix<int>(2, 2).includes_point(0, 0));
    assert(matrix<int>(2, 2).includes_point(0, 1));
    assert(matrix<int>(2, 2).includes_point(1, 0));
    assert(matrix<int>(2, 2).includes_point(1, 1));
    assert(!matrix<int>(2, 0).includes_point(0, 0));
  }

  // capacity()
  // reserve(n)
  // reserve_more(n)
  // shrink_to_fit()
  {
    matrix<int> m;
    assert(m.capacity() == 0u);
    m.reserve(2u);
    assert(m.capacity() >= 2u);
    m.reserve_more(3u);
    assert(m.capacity() >= 3u);
    m.shrink_to_fit();
    assert(m.capacity() == 0u);
  }

  // swap(it, it2)
  {
    matrix<int, list<int>> m(2, 2, seq(1, 2,
                                       3, 4));
    assert(m.width() == 2u && m.height() == 2u);
    assert(equal(m.range(), seq(1, 2,
                                3, 4)));
    m.swap(nth(m.range(), 0), nth(m.range(), 1));
    assert(equal(m.range(), seq(2, 1,
                                3, 4)));
  }
  // swap(it, m2, it2)
  {
    matrix<int, list<int>> m(2, 2, seq(1, 2,
                                       3, 4));
    matrix<int, list<int>> m2(2, 2, seq(9, 10,
                                        11, 12));
    m.swap(m.range().begin(), m2, m2.range().begin());
    assert(m.width() == 2u && m.height() == 2u);
    assert(equal(m.range(), seq(9, 2, 3, 4)));
    assert(m2.width() == 2u && m2.height() == 2u);
    assert(equal(m2.range(), seq(1, 10, 11, 12)));
  }
  // replace(it, it2, m_rv)
  {
    matrix<int, list<int>> m(2, 2, seq(1, 2,
                                       3, 4));
    matrix<int, list<int>> m2(2, 1, seq(-1, -2));
    m.replace(nth(m.range(), 1), nth(m.range(), 3), move(m2));
    assert(m.width() == 2u && m.height() == 2u);
    assert(equal(m.range(), seq(1, -1,
                                -2, 4)));
    assert(m2.empty() && m2.width() == 0u && m2.height() == 0u);
  }
  // exchange(it, it2, m_rv)
  {
    matrix<int, list<int>> m(2, 2, seq(1, 2,
                                       3, 4));
    matrix<int, list<int>> m2(2, 1, seq(-1, -2));
    matrix<int, list<int>> m3
      = m.exchange(nth(m.range(), 1), nth(m.range(), 3), move(m2));
    assert(m.width() == 2u && m.height() == 2u);
    assert(equal(m.range(), seq(1, -1,
                                -2, 4)));
    assert(m2.empty() && m2.width() == 0u && m2.height() == 0u);
    assert(m3.width() == 2u && m3.height() == 1u);
    assert(equal(m3.range(), seq(2, 3)));
  }

  // +=(m)
  // -=(m)
  {
    matrix<int> m(2, 2, seq(1, 2,
                            3, 4));
    assert((m += m) == matrix<int>(2, 2, seq(2, 4,
                                             6, 8)));
    assert(all_of_equal((m -= m).range(), 0));
  }
  // operator +()
  // operator -()
  {
    matrix<int> m(2, 2, seq(1, 2,
                            3, 4));
    assert(+m == m);
    assert(-m != m);
    assert(-(-m) == m);
  }

  // operator +(m)
  // operator -(m)
  {
    matrix<int> m(2, 2, seq(1, 2,
                            3, 4));
    m = m + matrix<int>(2, 2, seq(6, 7,
                                  8, 9));
    assert(m == matrix<int>(2, 2, seq(7, 9,
                                      11, 13)));
    m = m - m;
    assert(m == matrix<int>(2, 2, rng(4, 0)));
  }
  // operator *(k)
  // operator *=(k)
  // operator /(k)
  // operator /=(k)
  // k * m
  {
    matrix<int> m(2, 2, seq(1, 2,
                            3, 4));
    (m *= 2) /= 2;
    assert(m == matrix<int>(2, 2, seq(1, 2,
                                      3, 4)));
    m = (m * 2) / 2;
    assert(m == matrix<int>(2, 2, seq(1, 2,
                                      3, 4)));
    m = (2 * m) / 2;
    assert(m == matrix<int>(2, 2, seq(1, 2,
                                      3, 4)));
  }

  // operator *(m)
  {
    matrix<int> m(3, 2, seq(1, 2, 3,
                            4, 5, 6));
    matrix<int> m2(2, 3, seq(9, 10,
                             11, 12,
                             13, 14));
    matrix<int> m3 = m * m2;
    assert(m3.width() == 2u && m3.height() == 2u);
    assert(equal
           (m3.range(),
            seq(inner_product(seq(1, 2, 3), seq(9, 11, 13).begin(), 0),
                inner_product(seq(1, 2, 3), seq(10, 12, 14).begin(), 0),
                inner_product(seq(4, 5, 6), seq(9, 11, 13).begin(), 0),
                inner_product(seq(4, 5, 6), seq(10, 12, 14).begin(), 0))));
  }
}

void test_matrix() {
  printf("matrix: ");

  test_matrix0();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_matrix();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
