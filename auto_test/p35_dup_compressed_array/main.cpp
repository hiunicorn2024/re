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

void test_dup_compressed_array() {
  printf("dup_compressed_array: ");

  using a_t = stateful_test_allocator<uint8_t>;
  using v_t = dup_compressed_array<uint8_t, a_t>;

  // range operations
  {
    v_t v;
    assert(v.empty());
    assert(v.max_size() > 0u);
    assert(v.begin() == v.end());
    assert(v.cbegin() == v.end());
    assert(v.begin() == v.cend());
    assert(v.cbegin() == v.cend());
  }

  // smf
  // == <=>
  {
    static_assert(regular<v_t>);
    v_t v = {1u, 1u};
    v_t v2 = {1u, 2u, 2u, 1u};
    assert((v <=> v2) == strong_lt);
    assert(v != v2);
    assert(v == v && v2 == v2);
    assert(equal(v, rng(2, 1u)));
    assert(equal(v2, seq(1u, 2, 2, 1)));
    adl_swap(v, v2);
    assert(equal(v2, rng(2, 1u)));
    assert(equal(v, seq(1u, 2, 2, 1)));
  }

  // this_t(a)
  // this_t(const this_t &, a)
  // this_t(this_t &&, a)
  {
    a_t a1, a2;
    v_t v(a1);
    v_t v2(v, a2);
    v_t v3(move(v), a2);
    assert(v.get_allocator() == a1);
    assert(v2.get_allocator() == a2);
    assert(v3.get_allocator() == a2);
  }

  // explicit this_t(n, a = {})
  // this_t(n, x, a = {})
  // assign(n, x)
  {
    a_t a1;
    v_t v(3);
    assert(equal(v, rng(3, 0u)));
    assert(v.get_allocator() != a1);
    v_t v2(0, 1u, a1);
    assert(v2.empty());
    assert(v2.get_allocator() == a1);
    v2.assign(10, 6u);
    assert(equal(v2, rng(10, 6u)));
    assert(v2.compressed_size() == 1u);
    v_t v3(10, 6u);
    assert(equal(v3, rng(10, 6u)));
    assert(v3.compressed_size() == 1u);
  }

  // this_t(from, to, a = {})
  // assign(from, to)
  {
    const auto r = rng(600, 1u);
    v_t v(r.begin(), r.end());
    assert(equal(v, r));
    assert(v.compressed_size() == 3u);
    a_t a;
    v_t v2(r.begin(), r.end(), a);
    assert(equal(v, v2) && (v <=> v2) == 0);
    assert(v2.get_allocator() == a);
    const auto r2 = rng(255, 2u);
    v2.assign(r2.begin(), r2.end());
    assert(equal(v2, r2) && v2.compressed_size() == 1u);
  }

  // this_t(il, a = {})
  // =(il)
  // assign(il)
  {
    a_t a;
    v_t v({1u, 2u, 2u, 1u}, a);
    v_t v2 = {1u, 2u, 2u, 1u};
    assert(v == v2);
    assert(equal(v, seq(1u, 2, 2, 1)));
    v = {3u, 1u};
    v2.assign({1u, 3u});
    assert(equal(rrng(v2), v) && equal(v, seq(3u, 1u)));
  }

  // insert(pos, x)
  {
    vector<uint8_t> v0;
    v_t v;
    for (int i : irng(0u, 600u))
      for (int j : irng(0, i + 1)) {
        v0.clear();
        v.clear();
        assert(v0.empty() && v.empty());
        v0.append_range(rng(i, 1u));
        v.append_range(rng(i, 1u));
        assert(equal(v0, v));

        v0.insert(nth(v0, j), 2u);
        v.insert(nth(v, j), 2u);
        assert(equal(v0, v));

        v0.insert(nth(v0, j), 1u);
        v.insert(nth(v, j), 1u);
        assert(equal(v0, v));

        v0.insert(nth(v0, j + 2), 3u);
        v.insert(nth(v, j + 2), 3u);
        assert(equal(v0, v));
      }
    for (unsigned i : irng(0u, 200u))
      for (int j : irng(0, i + 1)) {
        v0.clear();
        v.clear();
        assert(v0.empty() && v.empty());
        v0.append_range(irng(0u, i));
        v.append_range(irng(0u, i));
        assert(equal(v0, v));

        v0.insert(nth(v0, j), 255u);
        v.insert(nth(v, j), 255u);
        assert(equal(v0, v));
      }
  }

  // insert(pos, from, to)
  // insert(pos, il)
  // insert(pos, r)
  // insert_range(pos, r)
  {
    v_t v;
    auto it = v.insert(v.end(), iitr(0u), iitr(5u));
    assert(it == v.begin());
    assert(equal(v, irng(0u, 5u)));

    it = v.insert(nth(v, 3), {0u, 1u, 0u});
    assert(equal(v, seq(0u, 1, 2, 0, 1, 0, 3, 4)));
    assert(it == nth(v, 3));

    it = v.insert(nth(v, 4), rng(3, 1u));
    assert(equal(v, seq(0u, 1, 2, 0, 1, 1, 1, 1, 0, 3, 4)));
    assert(it == nth(v, 4));
  }

  // erase(it)
  {
    const auto r = seq(0u, 0u, 0u, 1u, 2u, 2u, 3u, 3u, 3u, 4u);
    vector<uint8_t> v0;
    v_t v;
    for (int i : irng(0, ssize(r))) {
      v0 = r;
      v = r;
      assert(equal(v0, r));
      assert(equal(v, r));
      const auto it0 = v0.erase(nth(v0, i));
      const auto it = v.erase(nth(v, i));
      assert(distance(v0.begin(), it0) == i);
      assert(distance(v.begin(), it) == i);
      assert(equal(v0, v));
    }
  }
  // erase(it1, it2)
  {
    {
      v_t v = {1u};
      const auto it = v.erase(v.begin(), v.end());
      assert(it == v.end());
      assert(v.empty());
    }
    {
      v_t v;
      vector<uint8_t> v0;
      const auto r = seq(0u, 0u, 0u, 1u, 2u, 2u, 3u, 3u, 3u, 4u);
      for (auto i : irng(0, ssize(r)))
        for (auto j : irng(i, ssize(r))) {
          v0 = r;
          v = r;
          assert(equal(v0, r));
          assert(equal(v, r));
          assert(v.compressed_size() == 5u);
          const auto it0 = v0.erase(nth(v0, i), nth(v0, j));
          assert(it0 == nth(v0, i));
          const auto it = v.erase(nth(v, i), nth(v, j));
          assert(it == nth(v, i));
          assert(equal(v0, v));
        }
    }
  }

  // clear()
  {
    v_t v;
    v.clear();
    assert(v.empty());
    v = {1u, 2u, 3u};
    v.clear();
    assert(v.empty());
  }

  // front()
  // back()
  {
    v_t v = {1u};
    assert(v.front() == 1u);
    assert(v.back() == 1u);
    v = {1u, 2u, 1u, 1u, 1u};
    assert(v.front() == 1u);
    assert(v.back() == 1u);
  }
  // push_back(x)
  // pop_back()
  // pop_back(n)
  {
    v_t v;
    v.push_back(1u);
    v.pop_back();
    assert(v.empty());
    v.push_back(1u);
    v.push_back(1u);
    v.push_back(2u);
    v.pop_back();
    assert(equal(v, seq(1u, 1u)));
    v.pop_back();
    assert(equal(v, seq(1u)));
    v.pop_back();
    assert(v.empty());

    v.append(rng(1100, 1u));
    assert(equal(v, rng(1100, 1u)));
    assert(v.compressed_size() == 5u);
    v.pop_back(1100);
    assert(v.empty());

    v.append(rng(1100, 1u));
    assert(equal(v, rng(1100, 1u)));
    v.pop_back(500);
    assert(equal(v, rng(600, 1u)));
    assert(v.compressed_size() == 3u);
    v.pop_back(600);
    assert(v.empty());
  }

  // compressed_size()
  // capacity()
  // full()
  // reserve(n)
  // reserve_more(n)
  // shrink_to_fit()
  // reallocate(n = 0)
  // replace(i1, i2, r)
  {
    v_t v = {1u, 2u, 2u, 3u};
    assert(v.compressed_size() == 3u);
    v = {1u, 2u};
    assert(v.compressed_size() == 2u);

    v.clear();
    assert(!v.full());
    v.reserve(100u);
    assert(v.capacity() >= 100u);
    v.reserve_more(150u);
    assert(v.capacity() >= 150u);
    v.shrink_to_fit();
    assert(v.full());
    assert(v.capacity() == 0u);
    v.reserve(100u);
    assert(v.capacity() >= 100u);
    v.reallocate();
    assert(v.capacity() == 0u);
    assert(v.empty());

    v = {1u, 2u, 3u, 4u};
    const auto it = v.replace(nth(v, 1), nth(v, 3), seq(0u, 1u, 0u));
    assert(it == nth(v, 1));
    assert(equal(v, seq(1u, 0u, 1u, 0u, 4u)));
    const auto it2 = v.replace(v.begin(), v.end(), empty_rng<uint8_t>());
    assert(it2 == v.end());
    assert(v.empty());
  }

  // this_t(from_range, r, a = {})
  // explicit this_t(r)
  // this_t(r, a = {})
  // =(r)
  // assign(r)
  // assign_range(r)
  {
    v_t v = {from_range, seq(1u, 2u)};
    a_t a;
    v_t v2 = {from_range, seq(1u, 2u), a};
    assert(v2.get_allocator() == a);
    assert(v == v2);
    assert(equal(v, seq(1u, 2u)));

    v_t v3(seq(1u));
    assert(equal(v3, single_rng(1u)));
    (v3 = empty_rng<uint8_t>()) = irng(0u, 4u);
    assert(equal(v3, irng(0u, 4u)));
    v3.assign(rng(3, 1u));
    assert(equal(v3, rng(3, 1u)));
    v3.assign_range(empty_rng<uint8_t>());
    assert(v3.empty());
  }

  // append(s...)
  {
    v_t v;
    v.append(1u, 2u, seq(3u, 4u));
    assert(equal(v, irng(1u, 5u)));
  }

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::test::test_dup_compressed_array();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
