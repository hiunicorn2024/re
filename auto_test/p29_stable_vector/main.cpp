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

namespace re::inner::fns {

void test_stable_vector_briefly() {
  using vec_t = stable_vector<int, stateful_test_allocator<int>>;
  {
    vec_t v;
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 0);
    assert(v.full());
    assert(inner::good(v));
    test_equality(v.begin(), v.end());
    test_equality(v.cbegin(), v.end());
    test_equality(v.begin(), v.cend());
    test_equality(v.cbegin(), v.cend());
    assert(v.begin() + 0 == v.end());
    assert(v.begin() - 0 == v.end());
    assert(0 + v.cbegin() == v.end());
    assert(v.begin() - v.end() == 0);
    assert(v.begin() - v.cend() == 0);
    assert(v.cbegin() - v.end() == 0);
    assert(v.cbegin() - v.cend() == 0);
    assert(v.end() - v.begin() == 0);
    assert(v.end() - v.cbegin() == 0);
    assert(v.cend() - v.begin() == 0);
    assert(v.cend() - v.cbegin() == 0);

    v.reallocate(0);
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 0);
    assert(v.full());
    assert(inner::good(v));

    v.reallocate(1);
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.capacity() == 1);
    assert(!v.full());
    assert(inner::good(v));
    test_equality(v.begin(), v.end());
    test_equality(v.cbegin(), v.end());
    test_equality(v.begin(), v.cend());
    test_equality(v.cbegin(), v.cend());
    assert(v.begin() + 0 == v.end());
    assert(v.begin() - 0 == v.end());
    assert(0 + v.cbegin() == v.end());
    assert(v.begin() - v.end() == 0);
    assert(v.begin() - v.cend() == 0);
    assert(v.cbegin() - v.end() == 0);
    assert(v.cbegin() - v.cend() == 0);
    assert(v.end() - v.begin() == 0);
    assert(v.end() - v.cbegin() == 0);
    assert(v.cend() - v.begin() == 0);
    assert(v.cend() - v.cbegin() == 0);
    for (int i : irng(1, 10)) {
      v.reallocate(i);
      assert(v.empty());
      assert(v.size() == 0);
      assert(v.capacity() == to_unsigned(i));
      assert(!v.full());
      assert(inner::good(v));
      test_equality(v.begin(), v.end());
      test_equality(v.cbegin(), v.end());
      test_equality(v.begin(), v.cend());
      test_equality(v.cbegin(), v.cend());
      assert(v.begin() + 0 == v.end());
      assert(v.begin() - 0 == v.end());
      assert(0 + v.cbegin() == v.end());
      assert(v.begin() - v.end() == 0);
      assert(v.begin() - v.cend() == 0);
      assert(v.cbegin() - v.end() == 0);
      assert(v.cbegin() - v.cend() == 0);
      assert(v.end() - v.begin() == 0);
      assert(v.end() - v.cbegin() == 0);
      assert(v.cend() - v.begin() == 0);
      assert(v.cend() - v.cbegin() == 0);
    }
    v.shrink_to_fit();
    assert(v.full());
    assert(v.empty());
    assert(inner::good(v));

    v.push_back(1);
    assert(inner::good(v));
    auto it = v.end();
    assert(equal(v, seq(1)));
    assert(v.end() == it);
    v.clear();
    assert(inner::good(v));
    assert(v.end() + 0 == v.begin());
    assert(!v.full());
    assert(v.capacity() == 1);
    v.shrink_to_fit();
    assert(inner::good(v));
    assert(v.full());

    it = v.end();
    for (int i : irng(0, 100)) {
      v.push_back(i);
      assert(inner::good(v));
      assert(equal(v, irng(0, i + 1)));
    }
    assert(it == v.end());
    assert(v.capacity() >= v.size() && v.capacity() <= v.size() * 2);
    assert(inner::good(v));
    auto n = v.capacity();
    for (int i : irng(0, 100)) {
      v.pop_back();
      assert(inner::good(v));
      assert(equal(v, irng(0, 99 - i)));
    }
    assert(v.empty());
    assert(v.capacity() == n);
    v.shrink_to_fit();
    assert(inner::good(v));
    assert(v.size() == 0);
    assert(v.begin() + 0 == v.end());
    assert(v.capacity() == 0);
  }
  {
    {
      {
        vec_t v(irng(0, 0));
        assert(v.empty());
        assert(v.full());
        assert(inner::good(v));
      }
      {
        typename vec_t::allocator_type al;
        vec_t v(irng(0, 3), al);
        assert(equal(v, irng(0, 3)));
        assert(v.get_allocator() == al);
        assert(inner::good(v));
      }
    }
    {
      {
        vec_t v;
        vec_t vv(v);
        assert(vv.get_allocator() == v.get_allocator());
        assert(v == vv && v.empty());
        vec_t vvv(move(v));
        assert(vvv.get_allocator() == v.get_allocator());
        assert(v == vv && v.empty());
        assert(inner::good(v));
        assert(inner::good(v));
      }
      {
        vec_t v = {1, 2, 3};
        vec_t vv = move(v);
        assert(v.empty());
        assert(equal(vv, seq(1, 2, 3)));
        assert(inner::good(vv));
      }
      {
        {
          vec_t::allocator_type a;
          vec_t v;
          assert(inner::good(v));
          assert(v.get_allocator() != a);
        }
        {
          vec_t::allocator_type a;
          vec_t v = {{1, 2, 3}, a};
          assert(inner::good(v));
          assert(v.get_allocator() == a);
          assert(equal(v, seq(1, 2, 3)));
        }
      }
    }
    {
      vec_t::allocator_type a, a2;
      vec_t v(a);
      assert(inner::good(v));
      assert(v.get_allocator() == a);
      vec_t vv(v, a2);
      assert(inner::good(v));
      assert(vv.get_allocator() == a2);
      vec_t vvv(move(v), a2);
      assert(inner::good(v));
      assert(vvv.get_allocator() == a2);
      assert(v == vv && vv == vvv && v.empty());
    }
    {
      {
        vec_t v(0);
        assert(inner::good(v));
        assert(v.empty());
        assert(v.capacity() == 0);
      }
      {
        vec_t v(1);
        assert(inner::good(v));
        assert(v.size() == 1);
        assert(v.capacity() == 1);
        assert(v.full());
        assert(v.end() - v.begin() == 1);
        assert(v.begin() + 1 == v.end());
      }
      {
        vec_t v(3);
        assert(inner::good(v));
        assert(v.size() == 3);
        assert(v.capacity() == 3);
        assert(v.full());
        assert(v.cend() - v.begin() == 3);
        assert(v.begin() - -3 == v.cend());
      }
      {
        typename vec_t::allocator_type a;
        vec_t v(3, a);
        assert(inner::good(v));
        assert(v.get_allocator() == a);
        assert(v.size() == 3);
      }
    }
    {
      typename vec_t::allocator_type al;
      {
        vec_t v(3, 3);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        assert(v.get_allocator() != al);
      }
      {
        vec_t v(3, 3, al);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        assert(v.get_allocator() == al);
      }
    }
    {
      const int a[] = {1, 2, 3};
      vec_t::allocator_type al;
      vec_t v(begin(a), end(a));
      assert(inner::good(v));
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() != al);
      v.~vec_t();
      ::new(addressof(v)) vec_t(begin(a), end(a), al);
      assert(inner::good(v));
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }
    {
      vec_t::allocator_type al;
      vec_t v = {1, 2, 3};
      assert(inner::good(v));
      assert(equal(v, irng(1, 4)));
      assert(equal(rrng(v), rrng(irng(1, 4))));
      assert(v.capacity() == 3);
      assert(v.full());
      {
        vec_t z = {{}, al};
        assert(empty(z));
        assert(z.get_allocator() == al);
        assert(inner::good(z));
      }
    }
  }
  {
    {
      {
        vec_t v;
        vec_t vv = {1, 2};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = vv;
        assert(inner::good(v));
        assert(inner::good(vv));
        assert(v == vv);
        assert(equal(v, seq(1, 2)));
      }
      {
        vec_t v = {1, 2, 3, 4};
        vec_t vv = {5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = vv;
        assert(v == vv);
        assert(equal(v, seq(5, 6)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v = {1, 2, 3, 4};
        vec_t vv = {5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        vv = v;
        assert(v == vv);
        assert(equal(v, irng(1, 5)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
    }
    {
      {
        vec_t v;
        vec_t vv;
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(v == vv);
        assert(v.empty());
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v = {1, 2, 3};
        vec_t vv = {4, 5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(v);
        assert(equal(v, seq(1, 2, 3)));
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(inner::good(v));
        assert(inner::good(vv));
        assert(vv.empty());
        assert(equal(v, irng(4, 7)));
      }
      {
        vec_t v = {1, 2};
        vec_t vv = {{3, 4, 5}, v.get_allocator()};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(vv.empty());
        assert(equal(v, seq(3, 4, 5)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v;
        v = seq(1, 2);
        assert(inner::good(v));
        assert(equal(v, seq(1, 2)));
        v = seq(3);
        assert(inner::good(v));
        assert(equal(v, seq(3)));
        v.assign_range(degraded_irng(irng(0, 5)));
        assert(inner::good(v));
        assert(equal(v, irng(0, 5)));
        v.assign_range(degraded_irng(irng(0, 3)));
        assert(inner::good(v));
        assert(equal(v, irng(0, 3)));

        v.assign(3, 3);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        const int a[] = {1, 2};
        v.assign(begin(a), end(a));
        assert(inner::good(v));
        assert(equal(v, seq(1, 2)));

        v = {};
        assert(inner::good(v));
        assert(v.empty());
        v = {5, 6, 7};
        assert(inner::good(v));
        assert(equal(v, seq(5, 6, 7)));
        v = {8, 9};
        assert(inner::good(v));
        assert(equal(v, seq(8, 9)));
        v.assign({1, 2, 3, 4, 5, 6});
        assert(inner::good(v));
        assert(equal(v, irng(1, 7)));

        v.assign(rng(3, 3));
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        v.assign(rng(0, 0));
        assert(inner::good(v));
        assert(v.empty());
        v.assign_range(seq(4, 5));
        assert(inner::good(v));
        assert(equal(v, seq(4, 5)));
      }
    }
  }
  {
    vec_t v = {1, 3, 5};
    vec_t vv = {1, 4, 5};
    assert(inner::good(v));
    assert(inner::good(vv));
    test_equality(v, v);
    test_lessness(v, vv);
  }
  {
    vec_t v;
#ifndef RE_NOEXCEPT
    bool y = false;
    try {
      v.at(0);
    }
    catch (...) {
      y = true;
    }
    assert(y);
    y = false;
    try {
      as_const(v).at(0);
    }
    catch (...) {
      y = true;
    }
    assert(y);
#endif
    v = {1};
    assert(v.front() == 1);
    assert(as_const(v).front() == 1);
    assert(v.back() == 1);
    assert(as_const(v).back() == 1);
    assert(v.emplace_back(2) == 2);
    assert(equal(v, seq(1, 2)));
    v.push_back((const int &)3);
    v.push_back(4);
    assert(equal(v, seq(1, 2, 3, 4)));
    v.pop_back();
    assert(equal(v, seq(1, 2, 3)));
    v.pop_back(3);
    assert(v.empty());
  }
  {
    vec_t v;
    auto it = v.end();
    assert(*v.emplace(v.end(), 1) == 1);
    assert(equal(v, seq(1)));
    assert(inner::good(v));
    assert(it == v.end());
    assert(*v.emplace(v.begin(), 2) == 2);
    assert(equal(v, seq(2, 1)));
    assert(inner::good(v));
    assert(it == v.end());
    it = v.begin();
    v.reallocate(10);
    assert(inner::good(v));
    assert(v.capacity() == 10);
    assert(v.begin() == it);
    assert(*it == 2);
    assert(equal(v, seq(2, 1)));
    v.shrink_to_fit();
    assert(inner::good(v));
    assert(*v.insert(nth(v, 1), 3) == 3);
    assert(inner::good(v));
    assert(equal(v, seq(2, 3, 1)));
    assert(*v.insert(nth(v, 1), (const int &)4) == 4);
    assert(inner::good(v));
    assert(equal(v, seq(2, 4, 3, 1)));
    it = v.erase(v.begin());
    assert(inner::good(v));
    assert(it == v.begin());
    assert(equal(v, seq(4, 3, 1)));
    it = v.erase(v.end(), v.end());
    assert(inner::good(v));
    assert(it == v.end());
    assert(equal(v, seq(4, 3, 1)));
    it = v.erase(nth(v, 1), nth(v, 2));
    assert(inner::good(v));
    assert(*it == 1 && it == prev(v.end()));
    assert(equal(v, seq(4, 1)));
    it = v.insert(nth(v, 1), seq(3, 2));
    assert(inner::good(v));
    assert(it == v.begin() + 1 && equal(v, seq(4, 3, 2, 1)));
    it = v.insert(v.begin(), seq(5));
    assert(inner::good(v));
    assert(it == v.begin() && equal(v, seq(5, 4, 3, 2, 1)));
    it = v.insert_range(v.end(), seq(0));
    assert(inner::good(v));
    assert(it == v.end() - 1 && equal(v, rrng(irng(0, 6))));
  }
  {
    vec_t v = {1};
    v.delete_node(v.exchange_node(v.begin(), v.new_node(2)));
    assert(inner::good(v));
    assert(equal(v, seq(2)));
    auto p = v.begin();
    auto it = v.link(v.end(), v.exchange_node(v.begin(), v.new_node(1)));
    assert(inner::good(v));
    assert(it == p && *it == 2);
    assert(equal(v, seq(1, 2)));
    assert(*v.link_back(v.new_node(3)) == 3);
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    p = v.begin();
    it = v.unlink(p);
    assert(inner::good(v));
    v.delete_node(p);
    assert(it == v.begin());
    assert(equal(v, seq(2, 3)));
    auto a = seq(v.begin(), v.begin() + 1);
    v.unlink();
    assert(inner::good(v));
    assert(v.empty() && v.capacity() != 0);
    for (auto &i : iters(a))
      v.delete_node(*i);
    assert(inner::good(v));
    v = {1};
    auto nd = v.extract(v.begin());
    assert(v.empty());
    assert(inner::good(v));
    assert(*nd == 1);
    assert(*v.insert(v.end(), move(nd)) == 1);
    assert(inner::good(v));
    assert(nd.empty());
    assert(equal(v, seq(1)));
  }
  {
    vec_t v, vv;
    v = {1, 2};
    vv = {{3, 4}, v.get_allocator()};
    v.splice(v.end() - 1, vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty());
    assert(equal(v, seq(1, 3, 4, 2)));
    vv.splice(vv.end(), move(v));
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(equal(vv, seq(1, 3, 4, 2)));
  }
  {
    vec_t v = {1, 2, 3}, vv{{4, 5, 6}, v.get_allocator()};
    v.splice(v.begin(), v, v.begin());
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    v.splice(v.begin(), v, v.end() - 1);
    assert(inner::good(v));
    assert(equal(v, seq(3, 1, 2)));
    v.splice(v.begin(), v, v.begin() + 1);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 2)));
    v.splice(v.end(), vv, vv.begin());
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 2, 4)));
    assert(equal(vv, seq(5, 6)));
    v.clear();
    assert(inner::good(v));
    v.splice(v.end(), vv, vv.end() - 1);
    assert(inner::good(v));
    assert(equal(v, seq(6)));
    assert(equal(vv, seq(5)));
  }
  {
    vec_t v = {}, vv({1, 2, 3, 4}, v.get_allocator());
    v.splice(v.end(), v, v.begin(), v.end());
    assert(inner::good(v));
    assert(v.empty());
    assert(equal(vv, irng(1, 5)));
    v.splice(v.end(), vv, vv.begin() + 1, vv.begin() + 3);
    assert(inner::good(v));
    assert(equal(v, seq(2, 3)));
    assert(equal(vv, seq(1, 4)));
    v.splice(v.begin() + 1, vv, vv.begin(), vv.end());
    assert(inner::good(v));
    assert(vv.empty());
    assert(equal(v, seq(2, 1, 4, 3)));
    v.splice(v.begin(), v, v.begin() + 1, v.end());
    assert(inner::good(v));
    assert(equal(v, seq(1, 4, 3, 2)));
    v.splice(v.end(), v, v.begin(), v.end());
    assert(inner::good(v));
    assert(equal(v, seq(1, 4, 3, 2)));
    v.splice(v.end() - 1, v, v.begin(), v.begin() + 2);
    assert(inner::good(v));
    assert(equal(v, seq(3, 1, 4, 2)));
  }
  {
    vec_t v = {1, 2, 3};
    assert(v.remove(4) == 0);
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    v.remove(2);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3)));
    v = {1, 2, 3, 2, 4, 2, 5, 2};
    assert(v.remove(2) == 4);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 4, 5)));
    v.remove_if([](auto x) {return x == 3 || x == 5;});
    assert(inner::good(v));
    assert(equal(v, seq(1, 4)));
  }
  {
    vec_t v = {1, 2, 2, 3, 3, 4, 5, 5, 5, 5, 6};
    v.unique();
    assert(inner::good(v));
    assert(equal(v, irng(1, 7)));
    v = {1, 1, 1, 1, 1};
    v.unique();
    assert(inner::good(v));
    assert(equal(v, seq(1)));
  }
  {
    vec_t v = {1, 3, 5}, vv({1, 2, 4, 5}, v.get_allocator());
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty());
    assert(equal(v, seq(1, 1, 2, 3, 4, 5, 5)));
    v = {};
    vv = {};
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(vv.empty());
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(vv.empty());
    v = {1, 2};
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(equal(v, seq(1, 2)));
    assert(vv.empty());
    vv.merge(v);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty() && equal(vv, seq(1, 2)));
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty() && equal(v, seq(1, 2)));
  }
  {
    vec_t v = {};
    v.reverse();
    assert(inner::good(v));
    assert(v.empty());
    v = {1, 2, 3};
    v.reverse();
    assert(inner::good(v));
    assert(equal(v, seq(3, 2, 1)));
    v = {1, 2, 3, 4};
    v.reverse();
    assert(inner::good(v));
    assert(equal(v, rrng(irng(1, 5))));
  }
  {
    vec_t v = {};
    v.sort();
    assert(inner::good(v));
    assert(v.empty());
    v = {3, 2, 4, 1};
    assert(equal(v, seq(3, 2, 4, 1)));
    v.sort();
    assert(inner::good(v));
    assert(equal(v, irng(1, 5)));
    v.sort(greater<>());
    assert(inner::good(v));
    assert(equal(v, rrng(irng(1, 5))));
  }
  {
    vec_t v;
    v.append(1, (const int &)2, seq(3, 4, 5)).append(6);
    assert(equal(v, irng(1, 7)));
    assert(inner::good(v));
    v.append_range(irng(7, 100));
    assert(inner::good(v));
    assert(equal(v, irng(1, 100)));
    v.append_range(degraded_irng(irng(100, 1000)));
    assert(inner::good(v));
    assert(equal(v, irng(1, 1000)));
  }
  {
    vec_t v;
    using iter_t = typename vec_t::iterator;
    auto al = v.get_allocator();
    auto p = vec_t::traits::new_node(al, 1);
    v.link_back(iter_t(p));
    assert(equal(v, seq(1)));
    v.unlink_back();
    vec_t::traits::delete_node(al, p);
  }
  {
    {
      vec_t v(from_range, seq(1, 2));
      assert(equal(v, seq(1, 2)));
    }
    {
      vec_t v(from_range, seq(1, 2), vec_t::allocator_type{});
      assert(equal(v, seq(1, 2)));
    }
  }
  {
    using S = vec_t;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}
void test_stable_vector_construct_from_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20)) {
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i));
      assert(inner::good(v1));
      vec_t v2(irng(0, i));
      assert(inner::good(v2));
      assert(v2.size() == to_unsigned(i) && equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(inner::good(v3));
      assert(v3.size() == to_unsigned(i) && equal(v3, irng(0, i)));
    }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_assign_range() {
  for (int i : irng(0, 20)) {
    for (int j : irng(i, 20)) {
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        vec_t v(rrng(irng(0, i)));
        assert(v.capacity() == to_unsigned(i));
        v.reserve(j);
        assert(v.capacity() == to_unsigned(j));
        v.reserve_more(j + j);
        assert(v.capacity() == to_unsigned(v.size() + j + j));
        assert(equal(v, rrng(irng(0, i))));

        for (int k : irng(0, 20))
          for (int l : irng(k, 20)) {
            vec_t v2(irng(0, k));
            assert(v2.capacity() == to_unsigned(k));
            v2.reserve(l);
            assert(v2.capacity() == to_unsigned(l));
            assert(equal(v2, irng(0, k)));
            v = v2;
            assert(inner::good(v));
            assert(inner::good(v2));
            assert(v == v2);

            vec_t z(rrng(irng(0, i)));
            z.reserve(j);
            z = degraded_irng(v2);
            assert(inner::good(z));
            assert(inner::good(v2));
            assert(z == v2);
          }
      };
      testf(type_tag<stable_vector<int>>());
      testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
    }
  }
}
void test_stable_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            vec_t v(irng(0, i));
            assert(v.capacity() == to_unsigned(i) && equal(v, irng(0, i)));
            auto p0 = v.begin();
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(inner::good(v));
            if (j != 0)
              assert(v.begin() == p0);
            assert(equal(rng(v.begin(), p), irng(0, j)));
          }
        for (int j : irng(0, i)) {
          vec_t v(irng(0, i));
          auto p0 = v.begin();
          auto p = v.erase(nth(v, j));
          assert(inner::good(v));
          if (j != 0)
            assert(v.begin() == p0);
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
        }
      }
    }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_insert_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v = irng(0, i);
        const auto p0 = v.begin();
        assert(v.emplace_back(i) == i);
        assert(inner::good(v));
        assert(equal(v, irng(0, i + 1)));
        if (i != 0)
          assert(p0 == v.begin());
      }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_insert_1() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        for (int k : irng(0, i + 1)) {
          vec_t v;
          v.reserve(j);
          v = irng(0, i);
          const bool full = v.size() == v.capacity();
          const auto old_begin = v.begin();
          const auto p = v.insert(nth(v, k), -1);
          assert(inner::good(v));
          assert(*p == -1);
          if (i != 0 && k != 0)
            assert(old_begin == v.begin());
          assert(equal(rng(v.begin(), p), irng(0, k)));
          assert(equal(rng(p + 1, v.end()), irng(k, i)));
        }
      }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_insert_range_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 20)) {
          const auto r = irng(0, k);
          vec_t v;
          v.reserve(j);
          v = irng(0, i);
          const auto old_begin = v.begin();
          v.push_back(r);
          assert(inner::good(v));
          if (i != 0)
            assert(old_begin == v.begin());
          assert(v.size() == size(r) + i);
          assert(equal(rng(v.begin(), nth(v, i)), irng(0, i)));
          assert(equal(rng(nth(v, i), v.end()), irng(0, k)));
        }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int pos : irng(0, i + 1))
          for (int k : irng(0, 20)) {
            auto fn = [=](auto get_range, bool one_by_one) {
              const auto r = irng(0, k);
              vec_t v;
              v.reserve(j);
              v.push_back(irng(0, i));
              const auto old_begin = v.begin();
              const auto p = v.insert(nth(v, pos), get_range(r));
              assert(inner::good(v));
              assert(equal(rng(p, size(r)), r));
              assert(equal(rng(v.begin(), p), irng(0, pos)));
              assert(equal(rng(p + size(r), v.end()), irng(pos, i)));
              if (!one_by_one && i != 0 && pos != 0)
                assert(v.begin() == old_begin);
            };
            fn(identity{}, false);
            fn(degraded_irng, true);
          }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_clear() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(j));
        v.clear();
        assert(inner::good(v));
        assert(v.empty());
        assert(v.capacity() == to_unsigned(j));
      }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 30)) {
          {
            vec_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(j));
            v.resize(k);
            assert(inner::good(v));
            if (k <= i) {
              assert(v.capacity() == to_unsigned(j));
              assert(equal(v, irng(0, k)));
            }
            else
              assert(equal(rng(v.begin(), i), irng(0, i)));
          }
          {
            vec_t v;
            v.reserve(j);
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            assert(v.capacity() == to_unsigned(j));
            v.resize(k, -1);
            assert(inner::good(v));
            if (k <= i) {
              assert(v.capacity() == to_unsigned(j));
              assert(equal(v, irng(0, k)));
            }
            else {
              assert(equal(rng(v.begin(), i), irng(0, i)));
              assert(all_of_equal(rng(nth(v, i), nth(v, k)), -1));
            }
          }
        }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_reallocate() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(i, 30)) {
          vec_t v;
          v.reserve(j);
          v.push_back(irng(0, i));
          assert(equal(v, irng(0, i)));
          assert(v.capacity() == to_unsigned(j));
          v.reallocate(k);
          assert(inner::good(v));
          assert(v.capacity() == to_unsigned(k));
          assert(equal(v, irng(0, i)));
        }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
void test_stable_vector_shrink_to_fit() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.reserve(j);
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(j));
        v.shrink_to_fit();
        assert(inner::good(v));
        assert(equal(v, irng(0, i)));
        assert(v.capacity() == to_unsigned(i));
      }
  };
  testf(type_tag<stable_vector<int>>());
  testf(type_tag<stable_vector<int, stateful_test_allocator<int>>>());
}
namespace stable_vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r, size_t cap) const {
    ::new(p) V();
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
    assert(inner::good(*p));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->reserve(cap);
    p->append(r);
    assert(p->capacity() == cap);
    assert(equal(*p, r));
    assert(inner::good(*p));
  }
};
constexpr fo_init init{};
struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r, size_t cap) const {
    return equal(*p, r) && inner::good(*p);
  }
  template <class V, class R, class AL>
  bool operator ()(V *p, R &&r, size_t cap, const AL &al) const {
    return equal(*p, r) && p->get_allocator() == al && inner::good(*p);
  }
};
constexpr fo_good good{};
struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty() && p->capacity() == 0 && inner::good(*p);
  }
};
constexpr fo_is_empty is_empty{};

}
void test_stable_vector_ownership() {
  auto r = irng(0, 10);
  for (int i : r)
    for (int j : r)
      for (int ii : r)
        for (int jj : r) {
          using namespace stable_vector_test0;
          using stable_vector_test0::is_empty;
          const int n1 = i;
          const int cap1 = i + j;
          const int n2 = ii;
          const int cap2 = ii + jj;
          auto testf = [=](auto tag) {
            using vec_t = typename decltype(tag)::type;
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), cap1),
               bind(good, _1, irng(0, n1), cap1),
               bind(init, _1, irng(0, n2), cap2),
               bind(good, _1, irng(0, n2), cap2),
               is_empty);
          };
          testf(type_tag<stable_vector<int>>());
          testf(type_tag
                <stable_vector<int, stateful_test_allocator<int>>>());
          using P00 = type_pack<true_type, false_type>;
          using P0 = type_pack_mul<P00, P00>;
          using P = type_pack_mul<P0, P0>;
          type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
              constexpr bool y1 = decltype(a)::type::value;
              constexpr bool y2 = decltype(b)::type::value;
              constexpr bool y3 = decltype(c)::type::value;
              constexpr bool y4 = decltype(d)::type::value;
              {
                using vec_t = stable_vector
                  <int, stateful_test_allocator
                   <int, ez_map, y1, y2, y3, y4>>;
                const stateful_test_allocator
                  <int, ez_map, y1, y2, y3, y4> a1, a2;
                test_allocator_aware_container_ownership<vec_t>
                  (bind(init, _1, irng(0, n1), cap1, a1),
                   bind(good, _1, irng(0, n1), cap1),
                   bind(init, _1, irng(0, n2), cap2, a1),
                   bind(good, _1, irng(0, n2), cap2),
                   is_empty);
                test_allocator_aware_container_ownership<vec_t>
                  (bind(init, _1, irng(0, n1), cap1, a1),
                   bind(good, _1, irng(0, n1), cap1),
                   bind(init, _1, irng(0, n2), cap2, a2),
                   bind(good, _1, irng(0, n2), cap2),
                   is_empty);
              }
            });
        }
}
void test_stable_vector_node_operations() {
  using vec_t = stable_vector<int, stateful_test_allocator<int>>;
  {
    vec_t v;
    vec_t::iterator i = v.new_node(1);
    assert(*i == 1);
    v.delete_node(i);

    vec_t::iterator a[30];
    transform(irng(0, 30), begin(a), [&](int i) {return v.new_node(i);});
    assert(equal(bind_rng(a, deref), irng(0, 30)));
    shuffle(a, minstd_rand0{});
    for (auto &it : a)
      v.delete_node(it);
  }
  {
    vec_t v(irng(0, 30));
    for_each_node(v, next, [&](vec_t::iterator i) {
        const auto x = *i;
        const vec_t::iterator it = v.exchange_node(i, v.new_node(99));
        assert(*it == x);
        v.delete_node(it);
      });
    assert(all_of_equal(v, 99));
  }
  {
    {
      vec_t v;
      v.link_back(v.new_node(1));
      assert(v.back() == 1);
      v.link(v.end(), v.new_node(2));
      assert(equal(v, irng(1, 3)));
      for (int i : irng(3, 30))
        v.link_back(v.new_node(i));
      assert(equal(v, irng(1, 30)));
    }
    {
      for (auto &i : iters(0, 30)) {
        vec_t v(irng(0, i));
        assert(equal(v, irng(0, i)));
        const vec_t::iterator it = v.link_back(v.new_node(i));
        assert(*it == i);
        assert(equal(v, irng(0, i + 1)));
        for (auto &j : iters(0, i + 1)) {
          vec_t z(irng(0, i));
          const vec_t::iterator iter = z.link(nth(z, j), z.new_node(99));
          assert(equal(rng(z.begin(), iter), irng(0, j)));
          assert(*iter == 99);
          assert(equal(rng(next(iter), z.end()), irng(j, i)));
        }
      }
    }
  }
  {
    for (int &i : iters(0, 30)) {
      {
        vec_t v(irng(0, i));
        vector<vec_t::iterator> a;
        a.push_back(irng(v));
        assert(equal(bind_rng(a, deref), irng(0, i)));
        v.unlink();
        assert(v.empty());
        assert(equal(bind_rng(a, deref), irng(0, i)));
        for (auto ii : a)
          v.delete_node(ii);

        if (i != 0) {
          vec_t z(irng(0, i));
          const auto it = prev(z.end());
          assert(*it == i - 1);
          const auto it2 = z.unlink_back();
          assert(it2 == z.end());
          assert(equal(z, irng(0, i - 1)));
          assert(*it == i - 1);
          z.delete_node(it);
        }
      }
      for (int &j : iters(0, i)) {
        vec_t v(irng(0, i));
        const vec_t::iterator it0 = nth(v, j);
        vec_t::iterator it = v.unlink(it0);
        v.delete_node(it0);
        assert(equal(rng(v.begin(), it), irng(0, j)));
        assert(equal(rng(it, v.end()), irng(j + 1, i)));
      }
      for (int &j : iters(0, i + 1))
        for (int &k : iters(j, i + 1)) {
          vec_t v(irng(0, i));
          vector<vec_t::iterator> a;
          const vec_t::iterator it = nth(v, j);
          const vec_t::iterator it2 = nth(v, k);
          a.push_back(irng(it, it2));
          assert(equal(bind_rng(a, deref), irng(j, k)));

          const vec_t::iterator it3 = v.unlink(it, it2);
          assert(it3 == it2);
          assert(equal(bind_rng(a, deref), irng(j, k)));
          assert(equal(rng(it3, v.end()), irng(k, i)));
          for (auto ii : a)
            v.delete_node(ii);
        }
    }
  }
  {
    for (int &i : iters(0, 30))
      for (int &j : iters(0, i)) {
        vec_t v(irng(0, i));
        auto n = v.extract(nth(v, j));
        assert(*n == j);
        assert(!n.empty());

        auto nn = move(n);
        assert(n.empty());
        assert(!nn.empty());
        assert(*nn == j);

        nn = move(nn);
        assert(!nn.empty());
        assert(*nn == j);

        n = move(nn);
        assert(nn.empty());
        assert(!n.empty());
        assert(*n == j);
      }

    for (int &i : iters(0, 30))
      for (int &j : iters(0, i + 1)) {
        vec_t v(irng(0, i));
        {
          vec_t::node_type n;
          assert(n.empty());
          const vec_t::iterator it = v.insert(nth(v, j), move(n));
          assert(it == nth(v, j));
          assert(n.empty());
        }
        {
          vec_t::node_type n(v.new_node(999).node(), v.get_allocator());
          assert(!n.empty());
          const vec_t::iterator it = v.insert(nth(v, j), move(n));
          assert(*it == 999);
          assert(n.empty());
          assert(equal(rng(v.begin(), it), irng(0, j)));
          assert(equal(rng(next(it), v.end()), irng(j, i)));
        }
      }
  }
  {
    using L = vec_t;
    {
      L l(irng(0, 10));
      vector<int> v(irng(0, 10));
      for (int i : irng(0, 10))
        for (int j : irng(0, 10)) {
          const auto x = nth(l, i);
          const auto y = nth(l, j);
          l.swap(x, y);
          adl_swap(v[i], v[j]);
          assert(equal(l, v));
        }
    }
    {
      L l = {1, 2, 3}, ll({4, 5, 6}, l.get_allocator());
      l.swap(nth(l, 1), ll, nth(ll, 1));
      assert(equal(l, seq(1, 5, 3)));
      assert(equal(ll, seq(4, 2, 6)));
    }
  }
}
void test_stable_vector_node_and_slice() {
  using L = stable_vector<int, stateful_test_allocator<int>>;
  {
    L::node_type n0;
    assert(n0.empty());
    L l;
    L::node_type n = l.make_node(1);
    assert(!n.empty() && *n == 1);
  }
  {
    L l = {1, 2, 3};
    L::node_type n = l.extract(nth(l, 1));
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    L::iterator it = l.insert(l.begin(), move(n));
    assert(n.empty());
    assert(*it == 2 && equal(l, seq(2, 1, 3)));

    n = l.exchange(l.begin(), L::node_type{});
    assert(!n.empty() && *n == 2);
    assert(equal(l, seq(1, 3)));
    n = l.exchange(l.begin(), n);
    assert(!n.empty() && *n == 1);
    assert(equal(l, seq(2, 3)));

    assert(*l.replace(l.begin(), move(n)) == 1);
    assert(equal(l, seq(1, 3)));
    assert(l.replace(l.begin(), L::node_type{}) == next(l.begin()));
    assert(equal(l, seq(3)));
  }
  {
    L l = {1, 2, 3, 4};
    L m(l.extract(l.begin(), l.end()), l.get_allocator());
    assert(l.empty() && equal(m, irng(1, 5)));
    assert(l.insert(l.end(), L{}) == l.end());
    assert(l.empty());
    const auto tmp = l.insert(l.end(), L({1}, l.get_allocator()));
    assert(tmp == l.begin() && equal(l, seq(1)));

    l = {1, 0, 0, 4};
    m = l.exchange(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator()));
    assert(equal(m, seq(0, 0)) && equal(l, irng(1, 5)));
    m = l.exchange(l.begin(), l.end(), m);
    assert(equal(l, seq(0, 0)) && equal(m, irng(1, 5)));

    l = {1, 0, 0, 4};
    assert(*l.replace(nth(l, 1), nth(l, 3), L({2, 3}, l.get_allocator()))
           == 2);
    assert(equal(l, irng(1, 5)));
  }
  {
    L l = {1, 2, 3};
    L::node_type n;
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(1, 2, 3)));
    n = l.make_node(4);
    l.push_back(move(n));
    assert(n.empty() && equal(l, seq(1, 2, 3, 4)));
  }
  {
    L l = {1, 2, 3};
    L m({0, 0}, l.get_allocator());
    const auto p1 = addressof(ref(m, 0));
    const auto p2 = addressof(ref(m, 1));
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
    assert(addressof(ref(l, 3)) == p1);
    assert(addressof(ref(l, 4)) == p2);
    l.push_back(move(m));
    assert(m.empty() && equal(l, seq(1, 2, 3, 0, 0)));
  }
}

void test_stable_vector_class() {
  test_stable_vector_briefly();
  test_stable_vector_construct_from_range();
  test_stable_vector_assign_range();
  test_stable_vector_erase();
  test_stable_vector_insert_1_back();
  test_stable_vector_insert_1();
  test_stable_vector_insert_range_back();
  test_stable_vector_insert_range();
  test_stable_vector_clear();
  test_stable_vector_resize();
  test_stable_vector_reallocate();
  test_stable_vector_shrink_to_fit();
  test_stable_vector_ownership();
  test_stable_vector_node_operations();
  test_stable_vector_node_and_slice();
}


void test_limited_stable_vector_briefly() {
  using vec_t = limited_stable_vector<int, 10, stateful_test_allocator<int>>;
  {
    vec_t v;
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.max_size() == 10);
    assert(v.capacity() == 10);
    assert(!v.full());
    assert(inner::good(v));
    test_equality(v.begin(), v.end());
    test_equality(v.cbegin(), v.end());
    test_equality(v.begin(), v.cend());
    test_equality(v.cbegin(), v.cend());
    assert(v.begin() + 0 == v.end());
    assert(v.begin() - 0 == v.end());
    assert(0 + v.cbegin() == v.end());
    assert(v.begin() - v.end() == 0);
    assert(v.begin() - v.cend() == 0);
    assert(v.cbegin() - v.end() == 0);
    assert(v.cbegin() - v.cend() == 0);
    assert(v.end() - v.begin() == 0);
    assert(v.end() - v.cbegin() == 0);
    assert(v.cend() - v.begin() == 0);
    assert(v.cend() - v.cbegin() == 0);

    v.push_back(1);
    assert(inner::good(v));
    auto it = v.end();
    assert(equal(v, seq(1)));
    assert(v.end() == it);
    v.clear();
    assert(inner::good(v));
    assert(v.end() + 0 == v.begin());
    assert(!v.full());

    it = v.end();
    for (int i : irng(0, 10)) {
      v.push_back(i);
      assert(inner::good(v));
      assert(equal(v, irng(0, i + 1)));
    }
    assert(v.full());
    assert(it == v.end());
    for (int i : irng(0, 10)) {
      v.pop_back();
      assert(inner::good(v));
      assert(equal(v, irng(0, 9 - i)));
    }
    assert(v.empty());
    assert(v.size() == 0);
    assert(v.begin() + 0 == v.end());
  }
  {
    {
      {
        vec_t v(irng(0, 0));
        assert(v.empty());
        assert(!v.full());
        assert(inner::good(v));
      }
      {
        typename vec_t::allocator_type al;
        vec_t v(irng(0, 3), al);
        assert(equal(v, irng(0, 3)));
        assert(v.get_allocator() == al);
        assert(inner::good(v));
      }
    }
    {
      {
        vec_t v;
        vec_t vv(v);
        assert(vv.get_allocator() == v.get_allocator());
        assert(v == vv && v.empty());
        vec_t vvv(move(v));
        assert(vvv.get_allocator() == v.get_allocator());
        assert(v == vv && v.empty());
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v = {1, 2, 3};
        vec_t vv = move(v);
        assert(v.empty());
        assert(equal(vv, seq(1, 2, 3)));
        assert(inner::good(vv));
      }
      {
        {
          vec_t::allocator_type a;
          vec_t v;
          assert(inner::good(v));
          assert(v.get_allocator() != a);
        }
        {
          vec_t::allocator_type a;
          vec_t v = {{1, 2, 3}, a};
          assert(inner::good(v));
          assert(v.get_allocator() == a);
          assert(equal(v, seq(1, 2, 3)));
        }
      }
    }
    {
      vec_t::allocator_type a, a2;
      vec_t v(a);
      assert(inner::good(v));
      assert(v.get_allocator() == a);
      vec_t vv(v, a2);
      assert(inner::good(v));
      assert(vv.get_allocator() == a2);
      vec_t vvv(move(v), a2);
      assert(inner::good(v));
      assert(vvv.get_allocator() == a2);
      assert(v == vv && vv == vvv && v.empty());
    }
    {
      {
        vec_t v(0);
        assert(inner::good(v));
        assert(v.empty());
      }
      {
        vec_t v(1);
        assert(inner::good(v));
        assert(v.size() == 1);
        assert(!v.full());
        assert(v.end() - v.begin() == 1);
        assert(v.begin() + 1 == v.end());
      }
      {
        vec_t v(3);
        assert(inner::good(v));
        assert(v.size() == 3);
        assert(!v.full());
        assert(v.cend() - v.begin() == 3);
        assert(v.begin() - -3 == v.cend());
      }
      {
        typename vec_t::allocator_type a;
        vec_t v(3, a);
        assert(inner::good(v));
        assert(v.get_allocator() == a);
        assert(v.size() == 3);
      }
    }
    {
      typename vec_t::allocator_type al;
      {
        vec_t v(3, 3);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        assert(v.get_allocator() != al);
      }
      {
        vec_t v(3, 3, al);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        assert(v.get_allocator() == al);
      }
    }
    {
      const int a[] = {1, 2, 3};
      vec_t::allocator_type al;
      vec_t v(begin(a), end(a));
      assert(inner::good(v));
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() != al);
      v.~vec_t();
      ::new(addressof(v)) vec_t(begin(a), end(a), al);
      assert(inner::good(v));
      assert(equal(v, seq(1, 2, 3)));
      assert(v.get_allocator() == al);
    }
    {
      vec_t::allocator_type al;
      vec_t v = {1, 2, 3};
      assert(inner::good(v));
      assert(equal(v, irng(1, 4)));
      assert(equal(rrng(v), rrng(irng(1, 4))));
      {
        vec_t z = {{}, al};
        assert(empty(z));
        assert(z.get_allocator() == al);
        assert(inner::good(z));
      }
    }
  }
  {
    {
      {
        vec_t v;
        vec_t vv = {1, 2};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = vv;
        assert(inner::good(v));
        assert(inner::good(vv));
        assert(v == vv);
        assert(equal(v, seq(1, 2)));
      }
      {
        vec_t v = {1, 2, 3, 4};
        vec_t vv = {5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = vv;
        assert(v == vv);
        assert(equal(v, seq(5, 6)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v = {1, 2, 3, 4};
        vec_t vv = {5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        vv = v;
        assert(v == vv);
        assert(equal(v, irng(1, 5)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
    }
    {
      {
        vec_t v;
        vec_t vv;
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(v == vv);
        assert(v.empty());
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v = {1, 2, 3};
        vec_t vv = {4, 5, 6};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(v);
        assert(equal(v, seq(1, 2, 3)));
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(inner::good(v));
        assert(inner::good(vv));
        assert(vv.empty());
        assert(equal(v, irng(4, 7)));
      }
      {
        vec_t v = {1, 2};
        vec_t vv = {{3, 4, 5}, v.get_allocator()};
        assert(inner::good(v));
        assert(inner::good(vv));
        v = move(vv);
        assert(vv.empty());
        assert(equal(v, seq(3, 4, 5)));
        assert(inner::good(v));
        assert(inner::good(vv));
      }
      {
        vec_t v;
        v = seq(1, 2);
        assert(inner::good(v));
        assert(equal(v, seq(1, 2)));
        v = seq(3);
        assert(inner::good(v));
        assert(equal(v, seq(3)));
        v.assign_range(degraded_irng(irng(0, 5)));
        assert(inner::good(v));
        assert(equal(v, irng(0, 5)));
        v.assign_range(degraded_irng(irng(0, 3)));
        assert(inner::good(v));
        assert(equal(v, irng(0, 3)));

        v.assign(3, 3);
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        const int a[] = {1, 2};
        v.assign(begin(a), end(a));
        assert(inner::good(v));
        assert(equal(v, seq(1, 2)));

        v = {};
        assert(inner::good(v));
        assert(v.empty());
        v = {5, 6, 7};
        assert(inner::good(v));
        assert(equal(v, seq(5, 6, 7)));
        v = {8, 9};
        assert(inner::good(v));
        assert(equal(v, seq(8, 9)));
        v.assign({1, 2, 3, 4, 5, 6});
        assert(inner::good(v));
        assert(equal(v, irng(1, 7)));

        v.assign(rng(3, 3));
        assert(inner::good(v));
        assert(equal(v, rng(3, 3)));
        v.assign(rng(0, 0));
        assert(inner::good(v));
        assert(v.empty());
        v.assign_range(seq(4, 5));
        assert(inner::good(v));
        assert(equal(v, seq(4, 5)));
      }
    }
  }
  {
    vec_t v = {1, 3, 5};
    vec_t vv = {1, 4, 5};
    assert(inner::good(v));
    assert(inner::good(vv));
    test_equality(v, v);
    test_lessness(v, vv);
  }
  {
    vec_t v;
#ifndef RE_NOEXCEPT
    bool y = false;
    try {
      v.at(0);
    }
    catch (const out_of_range &) {
      y = true;
    }
    assert(y);
    y = false;
    try {
      as_const(v).at(0);
    }
    catch (const out_of_range &) {
      y = true;
    }
    assert(y);
#endif
    v = {1};
    assert(v.front() == 1);
    assert(as_const(v).front() == 1);
    assert(v.back() == 1);
    assert(as_const(v).back() == 1);
    assert(v.emplace_back(2) == 2);
    assert(equal(v, seq(1, 2)));
    v.push_back((const int &)3);
    v.push_back(4);
    assert(equal(v, seq(1, 2, 3, 4)));
    assert(inner::good(v));
    v.pop_back();
    assert(equal(v, seq(1, 2, 3)));
    v.pop_back(3);
    assert(v.empty());
  }
  {
    vec_t v;
    auto it = v.end();
    assert(*v.emplace(v.end(), 1) == 1);
    assert(equal(v, seq(1)));
    assert(inner::good(v));
    assert(it == v.end());
    assert(*v.emplace(v.begin(), 2) == 2);
    assert(equal(v, seq(2, 1)));
    assert(inner::good(v));
    assert(it == v.end());
    it = v.begin();
    v.clear();
    assert(v.empty());
    assert(inner::good(v));
    v.insert(v.end(), 2);
    v.insert(v.end(), 1);
    assert(equal(v, seq(2, 1)));
    assert(inner::good(v));
    assert(*v.insert(nth(v, 1), 3) == 3);
    assert(inner::good(v));
    assert(equal(v, seq(2, 3, 1)));
    assert(*v.insert(nth(v, 1), (const int &)4) == 4);
    assert(inner::good(v));
    assert(equal(v, seq(2, 4, 3, 1)));
    it = v.erase(v.begin());
    assert(inner::good(v));
    assert(it == v.begin());
    assert(equal(v, seq(4, 3, 1)));
    it = v.erase(v.end(), v.end());
    assert(inner::good(v));
    assert(it == v.end());
    assert(equal(v, seq(4, 3, 1)));
    it = v.erase(nth(v, 1), nth(v, 2));
    assert(inner::good(v));
    assert(*it == 1 && it == prev(v.end()));
    assert(equal(v, seq(4, 1)));
    it = v.insert(nth(v, 1), seq(3, 2));
    assert(inner::good(v));
    assert(it == v.begin() + 1 && equal(v, seq(4, 3, 2, 1)));
    it = v.insert(v.begin(), seq(5));
    assert(inner::good(v));
    assert(it == v.begin() && equal(v, seq(5, 4, 3, 2, 1)));
    it = v.insert_range(v.end(), seq(0));
    assert(inner::good(v));
    assert(it == v.end() - 1 && equal(v, rrng(irng(0, 6))));
  }
  {
    vec_t v = {1};
    v.delete_node(v.exchange_node(v.begin(), v.new_node(2)));
    assert(inner::good(v));
    assert(equal(v, seq(2)));
    auto p = v.begin();
    auto it = v.link(v.end(), v.exchange_node(v.begin(), v.new_node(1)));
    assert(inner::good(v));
    assert(it == p && *it == 2);
    assert(equal(v, seq(1, 2)));
    assert(*v.link_back(v.new_node(3)) == 3);
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    p = v.begin();
    it = v.unlink(p);
    assert(inner::good(v));
    v.delete_node(p);
    assert(it == v.begin());
    assert(equal(v, seq(2, 3)));
    auto a = seq(v.begin(), v.begin() + 1);
    v.unlink();
    assert(inner::good(v));
    assert(v.empty() && v.capacity() != 0);
    for (auto &i : iters(a))
      v.delete_node(*i);
    assert(inner::good(v));
    v = {1};
    auto nd = v.extract(v.begin());
    assert(v.empty());
    assert(inner::good(v));
    assert(*nd == 1);
    assert(*v.insert(v.end(), move(nd)) == 1);
    assert(inner::good(v));
    assert(nd.empty());
    assert(equal(v, seq(1)));
  }
  {
    vec_t v, vv;
    v = {1, 2};
    vv = {{3, 4}, v.get_allocator()};
    v.splice(v.end() - 1, vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty());
    assert(equal(v, seq(1, 3, 4, 2)));
    vv.splice(vv.end(), move(v));
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(equal(vv, seq(1, 3, 4, 2)));
  }
  {
    vec_t v = {1, 2, 3}, vv{{4, 5, 6}, v.get_allocator()};
    v.splice(v.begin(), v, v.begin());
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    v.splice(v.begin(), v, v.end() - 1);
    assert(inner::good(v));
    assert(equal(v, seq(3, 1, 2)));
    v.splice(v.begin(), v, v.begin() + 1);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 2)));
    v.splice(v.end(), vv, vv.begin());
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 2, 4)));
    assert(equal(vv, seq(5, 6)));
    v.clear();
    assert(inner::good(v));
    v.splice(v.end(), vv, vv.end() - 1);
    assert(inner::good(v));
    assert(equal(v, seq(6)));
    assert(equal(vv, seq(5)));
  }
  {
    vec_t v = {}, vv({1, 2, 3, 4}, v.get_allocator());
    v.splice(v.end(), v, v.begin(), v.end());
    assert(inner::good(v));
    assert(v.empty());
    assert(equal(vv, irng(1, 5)));
    v.splice(v.end(), vv, vv.begin() + 1, vv.begin() + 3);
    assert(inner::good(v));
    assert(equal(v, seq(2, 3)));
    assert(equal(vv, seq(1, 4)));
    v.splice(v.begin() + 1, vv, vv.begin(), vv.end());
    assert(inner::good(v));
    assert(vv.empty());
    assert(equal(v, seq(2, 1, 4, 3)));
    v.splice(v.begin(), v, v.begin() + 1, v.end());
    assert(inner::good(v));
    assert(equal(v, seq(1, 4, 3, 2)));
    v.splice(v.end(), v, v.begin(), v.end());
    assert(inner::good(v));
    assert(equal(v, seq(1, 4, 3, 2)));
    v.splice(v.end() - 1, v, v.begin(), v.begin() + 2);
    assert(inner::good(v));
    assert(equal(v, seq(3, 1, 4, 2)));
  }
  {
    vec_t v = {1, 2, 3};
    v.remove(4);
    assert(inner::good(v));
    assert(equal(v, seq(1, 2, 3)));
    v.remove(2);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3)));
    v = {1, 2, 3, 2, 4, 2, 5, 2};
    v.remove(2);
    assert(inner::good(v));
    assert(equal(v, seq(1, 3, 4, 5)));
    v.remove_if([](auto x) {return x == 3 || x == 5;});
    assert(inner::good(v));
    assert(equal(v, seq(1, 4)));
  }
  {
    vec_t v = {1, 2, 2, 3, 3, 4, 5, 5, 5, 6};
    assert(v.full());
    v.unique();
    assert(inner::good(v));
    assert(equal(v, irng(1, 7)));
    v = {1, 1, 1, 1, 1};
    v.unique();
    assert(inner::good(v));
    assert(equal(v, seq(1)));
  }
  {
    vec_t v = {1, 3, 5}, vv({1, 2, 4, 5}, v.get_allocator());
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty());
    assert(equal(v, seq(1, 1, 2, 3, 4, 5, 5)));
    assert(inner::good(v));
    v = {};
    vv = {};
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(vv.empty());
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty());
    assert(vv.empty());
    v = {1, 2};
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(equal(v, seq(1, 2)));
    assert(vv.empty());
    vv.merge(v);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(v.empty() && equal(vv, seq(1, 2)));
    v.merge(vv);
    assert(inner::good(v));
    assert(inner::good(vv));
    assert(vv.empty() && equal(v, seq(1, 2)));
  }
  {
    vec_t v = {};
    v.reverse();
    assert(inner::good(v));
    assert(v.empty());
    v = {1, 2, 3};
    v.reverse();
    assert(inner::good(v));
    assert(equal(v, seq(3, 2, 1)));
    v = {1, 2, 3, 4};
    v.reverse();
    assert(inner::good(v));
    assert(equal(v, rrng(irng(1, 5))));
  }
  {
    vec_t v = {};
    v.sort();
    assert(inner::good(v));
    assert(v.empty());
    v = {3, 2, 4, 1};
    assert(equal(v, seq(3, 2, 4, 1)));
    v.sort();
    assert(inner::good(v));
    assert(equal(v, irng(1, 5)));
    v.sort(greater<>());
    assert(inner::good(v));
    assert(equal(v, rrng(irng(1, 5))));
  }
  {
    using vt = limited_stable_vector<int, 20, stateful_test_allocator<int>>;
    vt v;
    v.append(1, (const int &)2, seq(3, 4, 5)).append(6);
    assert(equal(v, irng(1, 7)));
    assert(inner::good(v));
    assert(!v.full());
    v.append_range(irng(7, 10));
    assert(inner::good(v));
    assert(equal(v, irng(1, 10)));
    assert(!v.full());
    v.append_range(degraded_irng(irng(10, 21)));
    assert(inner::good(v));
    assert(equal(v, irng(1, 21)));
    assert(v.full());
  }
  {
    vec_t v;
    using iter_t = typename vec_t::iterator;
    auto al = v.get_allocator();
    auto p = vec_t::traits::new_node(al, 1);
    v.link_back(iter_t(p));
    assert(equal(v, seq(1)));
    v.unlink_back();
    vec_t::traits::delete_node(al, p);
  }
  {
    using S = vec_t;
    S s = {1, 2, 3, 4};
    auto z = s.replace(nth(s, 1), nth(s, 3), seq(0, 0, 0));
    assert(equal(s, seq(1, 0, 0, 0, 4)));
  }
}
void test_limited_stable_vector_construct_from_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20)) {
      vec_t v1(i);
      assert(v1.size() == to_unsigned(i));
      assert(inner::good(v1));
      vec_t v2(irng(0, i));
      assert(inner::good(v2));
      assert(v2.size() == to_unsigned(i) && equal(v2, irng(0, i)));
      vec_t v3(degraded_irng(irng(0, i)));
      assert(inner::good(v3));
      assert(v3.size() == to_unsigned(i) && equal(v3, irng(0, i)));
    }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag
        <limited_stable_vector<int, 30, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_assign_range() {
  for (int i : irng(0, 20)) {
    for (int j : irng(i, 20)) {
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        vec_t v(rrng(irng(0, i)));
        assert(equal(v, rrng(irng(0, i))));

        for (int k : irng(0, 20))
          for (int l : irng(k, 20)) {
            vec_t v2(irng(0, k));
            assert(equal(v2, irng(0, k)));
            v = v2;
            assert(inner::good(v));
            assert(inner::good(v2));
            assert(v == v2);

            vec_t z(rrng(irng(0, i)));
            z = degraded_irng(v2);
            assert(inner::good(z));
            assert(inner::good(v2));
            assert(z == v2);
          }
      };
      testf(type_tag<limited_stable_vector<int, 30>>());
      testf(type_tag<limited_stable_vector
                     <int, 30, stateful_test_allocator<int>>>());
    }
  }
}
void test_limited_stable_vector_erase() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 30)) {
      {
        for (int j : irng(0, i))
          for (int k : irng(j, i)) {
            vec_t v(irng(0, i));
            assert(equal(v, irng(0, i)));
            auto p0 = v.begin();
            auto p = v.erase(nth(v, j), nth(v, k));
            assert(inner::good(v));
            if (j != 0)
              assert(v.begin() == p0);
            assert(equal(rng(v.begin(), p), irng(0, j)));
          }
        for (int j : irng(0, i)) {
          vec_t v(irng(0, i));
          auto p0 = v.begin();
          auto p = v.erase(nth(v, j));
          assert(inner::good(v));
          if (j != 0)
            assert(v.begin() == p0);
          assert(equal(rng(v.begin(), p), irng(0, j)));
          assert(equal(rng(p, v.end()), irng(j + 1, i)));
        }
      }
    }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag<limited_stable_vector
                 <int, 30, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_insert_1_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v = irng(0, i);
        const auto p0 = v.begin();
        assert(v.emplace_back(i) == i);
        assert(inner::good(v));
        assert(equal(v, irng(0, i + 1)));
        if (i != 0)
          assert(p0 == v.begin());
      }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag<limited_stable_vector
                 <int, 30, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_insert_1() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        for (int k : irng(0, i + 1)) {
          vec_t v;
          v = irng(0, i);
          const bool full = v.size() == v.capacity();
          const auto old_begin = v.begin();
          const auto p = v.insert(nth(v, k), -1);
          assert(inner::good(v));
          assert(*p == -1);
          if (i != 0 && k != 0)
            assert(old_begin == v.begin());
          assert(equal(rng(v.begin(), p), irng(0, k)));
          assert(equal(rng(p + 1, v.end()), irng(k, i)));
        }
      }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag<limited_stable_vector
                 <int, 30, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_insert_range_back() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 20)) {
          const auto r = irng(0, k);
          vec_t v;
          v = irng(0, i);
          const auto old_begin = v.begin();
          v.push_back(r);
          assert(inner::good(v));
          if (i != 0)
            assert(old_begin == v.begin());
          assert(v.size() == size(r) + i);
          assert(equal(rng(v.begin(), nth(v, i)), irng(0, i)));
          assert(equal(rng(nth(v, i), v.end()), irng(0, k)));
        }
  };
  testf(type_tag<limited_stable_vector<int, 40>>());
  testf(type_tag<limited_stable_vector
                 <int, 40, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_insert_range() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int pos : irng(0, i + 1))
          for (int k : irng(0, 20)) {
            auto fn = [=](auto get_range, bool one_by_one) {
              const auto r = irng(0, k);
              vec_t v;
              v.push_back(irng(0, i));
              const auto old_begin = v.begin();
              const auto p = v.insert(nth(v, pos), get_range(r));
              assert(inner::good(v));
              assert(equal(rng(p, size(r)), r));
              assert(equal(rng(v.begin(), p), irng(0, pos)));
              assert(equal(rng(p + size(r), v.end()), irng(pos, i)));
              if (!one_by_one && i != 0 && pos != 0)
                assert(v.begin() == old_begin);
            };
            fn(identity{}, false);
            fn(degraded_irng, true);
          }
  };
  testf(type_tag<limited_stable_vector<int, 40>>());
  testf(type_tag<limited_stable_vector
                 <int, 40, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_clear() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20)) {
        vec_t v;
        v.push_back(irng(0, i));
        assert(equal(v, irng(0, i)));
        v.clear();
        assert(inner::good(v));
        assert(v.empty());
      }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag<limited_stable_vector
                 <int, 30, stateful_test_allocator<int>>>());
}
void test_limited_stable_vector_resize() {
  auto testf = [](auto tag) {
    using vec_t = typename decltype(tag)::type;
    for (int i : irng(0, 20))
      for (int j : irng(i, 20))
        for (int k : irng(0, 30)) {
          {
            vec_t v;
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.resize(k);
            assert(inner::good(v));
            if (k <= i)
              assert(equal(v, irng(0, k)));
            else
              assert(equal(rng(v.begin(), i), irng(0, i)));
          }
          {
            vec_t v;
            v.push_back(irng(0, i));
            assert(equal(v, irng(0, i)));
            v.resize(k, -1);
            assert(inner::good(v));
            if (k <= i)
              assert(equal(v, irng(0, k)));
            else {
              assert(equal(rng(v.begin(), i), irng(0, i)));
              assert(all_of_equal(rng(nth(v, i), nth(v, k)), -1));
            }
          }
        }
  };
  testf(type_tag<limited_stable_vector<int, 30>>());
  testf(type_tag<limited_stable_vector
                 <int, 30, stateful_test_allocator<int>>>());
}
namespace limited_stable_vector_test0 {

struct fo_init {
  template <class V, class R>
  void operator ()(V *p, R &&r) const {
    ::new(p) V();
    p->append(r);
    assert(equal(*p, r));
    assert(inner::good(*p));
  }
  template <class V, class R, class AL>
  void operator ()(V *p, R &&r, const AL &al) const {
    ::new(p) V(al);
    assert(p->get_allocator() == al);
    p->append(r);
    assert(equal(*p, r));
    assert(inner::good(*p));
  }
};
constexpr fo_init init{};
struct fo_good {
  template <class V, class R>
  bool operator ()(V *p, R &&r) const {
    return equal(*p, r) && inner::good(*p);
  }
  template <class V, class R, class AL>
  bool operator ()(V *p, R &&r, const AL &al) const {
    return equal(*p, r) && p->get_allocator() == al && inner::good(*p);
  }
};
constexpr fo_good good{};
struct fo_is_empty {
  template <class V>
  bool operator ()(V *p) const {
    return p->empty() && inner::good(*p);
  }
};
constexpr fo_is_empty is_empty{};

}
void test_limited_stable_vector_ownership() {
  auto r = irng(0, 10);
  for (int i : r)
    for (int j : r) {
      using namespace limited_stable_vector_test0;
      using limited_stable_vector_test0::is_empty;
      const int n1 = i;
      const int n2 = j;
      auto testf = [=](auto tag) {
        using vec_t = typename decltype(tag)::type;
        test_allocator_aware_container_ownership<vec_t>
        (bind(init, _1, irng(0, n1)),
         bind(good, _1, irng(0, n1)),
         bind(init, _1, irng(0, n2)),
         bind(good, _1, irng(0, n2)),
         is_empty);
      };
      testf(type_tag<limited_stable_vector<int, 30>>());
      testf(type_tag<limited_stable_vector
                     <int, 30, stateful_test_allocator<int>>>());

      using P00 = type_pack<true_type, false_type>;
      using P0 = type_pack_mul<P00, P00>;
      using P = type_pack_mul<P0, P0>;
      type_pack_for_each<P>([=](auto a, auto b, auto c, auto d) {
          constexpr bool y1 = decltype(a)::type::value;
          constexpr bool y2 = decltype(b)::type::value;
          constexpr bool y3 = decltype(c)::type::value;
          constexpr bool y4 = decltype(d)::type::value;
          {
            using vec_t = stable_vector
              <int, stateful_test_allocator<int, ez_map, y1, y2, y3, y4>>;
            const stateful_test_allocator
              <int, ez_map, y1, y2, y3, y4> a1, a2;
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), a1),
               bind(good, _1, irng(0, n1)),
               bind(init, _1, irng(0, n2), a1),
               bind(good, _1, irng(0, n2)),
               is_empty);
            test_allocator_aware_container_ownership<vec_t>
              (bind(init, _1, irng(0, n1), a1),
               bind(good, _1, irng(0, n1)),
               bind(init, _1, irng(0, n2), a2),
               bind(good, _1, irng(0, n2)),
               is_empty);
          }
        });
    }
}
void test_limited_stable_vector_node_operations() {
  using vec_t = limited_stable_vector
    <int, 40, stateful_test_allocator<int>>;
  {
    vec_t v;
    vec_t::iterator i = v.new_node(1);
    assert(*i == 1);
    v.delete_node(i);

    vec_t::iterator a[30];
    transform(irng(0, 30), begin(a), [&](int i) {return v.new_node(i);});
    assert(equal(bind_rng(a, deref), irng(0, 30)));
    shuffle(a, minstd_rand0{});
    for (auto &it : a)
      v.delete_node(it);
  }
  {
    vec_t v(irng(0, 30));
    for_each_node(v, next, [&](vec_t::iterator i) {
        const auto x = *i;
        const vec_t::iterator it = v.exchange_node(i, v.new_node(99));
        assert(*it == x);
        v.delete_node(it);
      });
    assert(all_of_equal(v, 99));
  }
  {
    {
      vec_t v;
      v.link_back(v.new_node(1));
      assert(v.back() == 1);
      v.link(v.end(), v.new_node(2));
      assert(equal(v, irng(1, 3)));
      assert(inner::good(v));
      for (int i : irng(3, 30))
        v.link_back(v.new_node(i));
      assert(equal(v, irng(1, 30)));
      assert(inner::good(v));
    }
    {
      for (auto &i : iters(0, 30)) {
        vec_t v(irng(0, i));
        assert(equal(v, irng(0, i)));
        const vec_t::iterator it = v.link_back(v.new_node(i));
        assert(*it == i);
        assert(equal(v, irng(0, i + 1)));
        for (auto &j : iters(0, i + 1)) {
          vec_t z(irng(0, i));
          const vec_t::iterator iter = z.link(nth(z, j), z.new_node(99));
          assert(inner::good(z));
          assert(equal(rng(z.begin(), iter), irng(0, j)));
          assert(*iter == 99);
          assert(equal(rng(next(iter), z.end()), irng(j, i)));
        }
      }
    }
  }
  {
    for (int &i : iters(0, 30)) {
      {
        vec_t v(irng(0, i));
        vector<vec_t::iterator> a;
        a.push_back(irng(v));
        assert(inner::good(v));
        assert(equal(bind_rng(a, deref), irng(0, i)));
        v.unlink();
        assert(inner::good(v));
        assert(v.empty());
        assert(equal(bind_rng(a, deref), irng(0, i)));
        for (auto ii : a)
          v.delete_node(ii);

        if (i != 0) {
          vec_t z(irng(0, i));
          const auto it = prev(z.end());
          assert(*it == i - 1);
          const auto it2 = z.unlink_back();
          assert(it2 == z.end());
          assert(inner::good(z));
          assert(equal(z, irng(0, i - 1)));
          assert(*it == i - 1);
          z.delete_node(it);
        }
      }
      for (int &j : iters(0, i)) {
        vec_t v(irng(0, i));
        const vec_t::iterator it0 = nth(v, j);
        vec_t::iterator it = v.unlink(it0);
        v.delete_node(it0);
        assert(inner::good(v));
        assert(equal(rng(v.begin(), it), irng(0, j)));
        assert(equal(rng(it, v.end()), irng(j + 1, i)));
      }
      for (int &j : iters(0, i + 1))
        for (int &k : iters(j, i + 1)) {
          vec_t v(irng(0, i));
          vector<vec_t::iterator> a;
          const vec_t::iterator it = nth(v, j);
          const vec_t::iterator it2 = nth(v, k);
          a.push_back(irng(it, it2));
          assert(equal(bind_rng(a, deref), irng(j, k)));
          assert(inner::good(v));

          const vec_t::iterator it3 = v.unlink(it, it2);
          assert(inner::good(v));
          assert(it3 == it2);
          assert(equal(bind_rng(a, deref), irng(j, k)));
          assert(equal(rng(it3, v.end()), irng(k, i)));
          for (auto ii : a)
            v.delete_node(ii);
        }
    }
  }
  {
    for (int &i : iters(0, 30))
      for (int &j : iters(0, i)) {
        vec_t v(irng(0, i));
        auto n = v.extract(nth(v, j));
        assert(*n == j);
        assert(!n.empty());

        auto nn = move(n);
        assert(n.empty());
        assert(!nn.empty());
        assert(*nn == j);

        nn = move(nn);
        assert(!nn.empty());
        assert(*nn == j);

        n = move(nn);
        assert(nn.empty());
        assert(!n.empty());
        assert(*n == j);
      }

    for (int &i : iters(0, 30))
      for (int &j : iters(0, i + 1)) {
        vec_t v(irng(0, i));
        {
          vec_t::node_type n;
          assert(n.empty());
          const vec_t::iterator it = v.insert(nth(v, j), move(n));
          assert(inner::good(v));
          assert(it == nth(v, j));
          assert(n.empty());
        }
        {
          vec_t::node_type n(v.new_node(999).node(), v.get_allocator());
          assert(!n.empty());
          const vec_t::iterator it = v.insert(nth(v, j), move(n));
          assert(inner::good(v));
          assert(*it == 999);
          assert(n.empty());
          assert(equal(rng(v.begin(), it), irng(0, j)));
          assert(equal(rng(next(it), v.end()), irng(j, i)));
        }
      }
  }
  {
    using L = vec_t;
    {
      L l(irng(0, 10));
      vector<int> v(irng(0, 10));
      for (int i : irng(0, 10))
        for (int j : irng(0, 10)) {
          const auto x = nth(l, i);
          const auto y = nth(l, j);
          l.swap(x, y);
          adl_swap(v[i], v[j]);
          assert(equal(l, v));
        }
    }
    {
      L l = {1, 2, 3}, ll({4, 5, 6}, l.get_allocator());
      l.swap(nth(l, 1), ll, nth(ll, 1));
      assert(equal(l, seq(1, 5, 3)));
      assert(equal(ll, seq(4, 2, 6)));
    }
  }
}

void test_limited_stable_vector() {
  test_limited_stable_vector_briefly();
  test_limited_stable_vector_construct_from_range();
  test_limited_stable_vector_assign_range();
  test_limited_stable_vector_erase();
  test_limited_stable_vector_insert_1_back();
  test_limited_stable_vector_insert_1();
  test_limited_stable_vector_insert_range_back();
  test_limited_stable_vector_insert_range();
  test_limited_stable_vector_clear();
  test_limited_stable_vector_resize();
  test_limited_stable_vector_ownership();
  test_limited_stable_vector_node_operations();
}


struct nd_t : join_stable_vector<> {
  int value;

  nd_t() = default;
  ~nd_t() = default;
  nd_t(const nd_t &) = default;
  nd_t &operator =(const nd_t &) = default;
  nd_t(nd_t &&) = default;
  nd_t &operator =(nd_t &&) = default;

  nd_t(int x) : value(x) {}

  int operator *() const {
    return value;
  }
};
template <class V>
void test_stbvec_intrusive_mode() {
  using alloc_t = typename V::allocator_type;
  using alw_t = allocator_wrapper<alloc_t>;
  using value_t = typename V::value_type;
  using iter_t = typename V::iterator;

  {
    assert((is_same_v<value_t, nd_t>));
    assert((is_same_v
            <typename allocator_traits<alloc_t>::value_type, value_t>));

    alw_t alw{};
    V v(alw.get());
    assert(inner::good(v));
    assert(v.empty());
    assert(v.size() == 0);

    nd_t *p = alw.new_1(10);
    assert(p->value == 10);
    alw.delete_1(p);

    p = alw.new_1(4);
    auto it = v.link_back(iter_t(p));
    assert(it == v.begin());
    assert(inner::good(v));
    assert(equal(deref_rng(v), seq(4)));
    assert(v.end() == v.unlink(v.begin()));
    assert(v.empty());
    assert(inner::good(v));
    it = v.link(v.end(), iter_t(p));
    assert(it == v.begin());
    assert(inner::good(v));
    assert(equal(deref_rng(v), seq(4)));
    assert(v.end() == v.erase_or_unlink(v.begin()));
    assert(v.empty());
    assert(inner::good(v));
    it = v.link(v.begin(), iter_t(p));
    assert(it == v.begin());
    assert(inner::good(v));
    assert(equal(deref_rng(v), seq(4)));
    it = v.unlink_back();
    assert(it == v.end());
    assert(v.empty());
    assert(inner::good(v));
    alw.delete_1(p);

    v.link(v.end(), iter_t(alw.new_1(1)));
    v.link(v.end(), iter_t(alw.new_1(2)));
    v.link(v.end(), iter_t(alw.new_1(3)));
    assert(equal(deref_rng(v), irng(1, 4)));
    auto it2 = v.begin();
    it = v.unlink(v.begin());
    alw.delete_1((nd_t *)it2.node());
    assert(it == v.begin() && **it == 2);
    assert(equal(deref_rng(v), seq(2, 3)));
    it2 = v.begin() + 1;
    it = v.unlink(v.begin() + 1, v.end());
    alw.delete_1((nd_t *)it2.node());
    assert(it == v.end());
    assert(equal(deref_rng(v), seq(2)));
    it2 = v.begin();
    v.unlink();
    assert(v.empty());
    alw.delete_1((nd_t *)it2.node());

    v.link_back(iter_t(alw.new_1(1)));
    v.link_back(iter_t(alw.new_1(2)));
    assert(equal(deref_rng(v), seq(1, 2)));
    it2 = v.exchange_node(v.begin(), iter_t(alw.new_1(3)));
    assert(**it2 == 1);
    alw.delete_1((nd_t *)it2.node());
    it2 = v.exchange_node(v.begin() + 1, iter_t(alw.new_1(4)));
    assert(**it2 == 2);
    alw.delete_1((nd_t *)it2.node());
    assert(equal(deref_rng(v), seq(3, 4)));
    it2 = prev(v.end());
    v.unlink_back();
    alw.delete_1((nd_t *)it2.node());
    it2 = prev(v.end());
    v.unlink_back();
    alw.delete_1((nd_t *)it2.node());
    assert(v.empty());

    const auto clear = [&](V &v) {
      while (!v.empty()) {
        auto it = prev(v.end());
        v.unlink_back();
        alw.delete_1((nd_t *)it.node());
      }
    };
    {
      v.link_back(iter_t(alw.new_1(1)));
      v.link_back(iter_t(alw.new_1(2)));
      assert(equal(deref_rng(v), seq(1, 2)));
      V vv(move(v), v.get_allocator());
      assert(v.get_allocator() == vv.get_allocator());
      assert(v.empty());
      assert(equal(deref_rng(vv), seq(1, 2)));
      clear(vv);
    }
    {
      v.link_back(iter_t(alw.new_1(1)));
      v.link_back(iter_t(alw.new_1(2)));
      assert(equal(deref_rng(v), seq(1, 2)));
      V vv(move(v));
      assert(v.get_allocator() == vv.get_allocator());
      assert(v.empty());
      assert(equal(deref_rng(vv), seq(1, 2)));
      clear(vv);
    }
    {
      v.link_back(iter_t(alw.new_1(1)));
      v.link_back(iter_t(alw.new_1(2)));
      assert(equal(deref_rng(v), seq(1, 2)));
      V vv(v.get_allocator());
      assert(v.get_allocator() == vv.get_allocator());
      vv.link_back(iter_t(alw.new_1(3)));
      vv.link_back(iter_t(alw.new_1(4)));
      assert(equal(deref_rng(vv), seq(3, 4)));
      const auto i1 = v.begin();
      const auto i2 = v.begin() + 1;
      v = move(vv);
      assert(equal(deref_rng(v), seq(3, 4)));
      assert(vv.empty());
      alw.delete_1((nd_t *)i1.node());
      alw.delete_1((nd_t *)i2.node());
      clear(v);
    }
    {
      v.link_back(iter_t(alw.new_1(1)));
      v.link_back(iter_t(alw.new_1(2)));
      assert(equal(deref_rng(v), seq(1, 2)));
      V vv;
      assert(v.get_allocator() != vv.get_allocator());
      vv.link_back(iter_t(alw.new_1(3)));
      vv.link_back(iter_t(alw.new_1(4)));
      assert(equal(deref_rng(vv), seq(3, 4)));
      const auto i1 = v.begin();
      const auto i2 = v.begin() + 1;
      v = move(vv);
      assert(equal(deref_rng(v), seq(3, 4)));
      assert(vv.empty());
      alw.delete_1((nd_t *)i1.node());
      alw.delete_1((nd_t *)i2.node());
      clear(v);
    }
  }
}
void test_stable_vector_intrusive_mode() {
  test_stbvec_intrusive_mode<stable_vector_adaptor
                             <stable_vector_traits<nd_t, 0, 0,
                                                   stateful_test_allocator
                                                   <nd_t>>>>();
  test_stbvec_intrusive_mode<stable_vector_adaptor
                             <stable_vector_traits<nd_t, 0, 0,
                                                   stateful_test_allocator
                                                   <nd_t>>,
                              20>>();
}


void test_stable_vector() {
  printf("stable_vector: ");

  inner::fns::test_stable_vector_class();
  inner::fns::test_limited_stable_vector();
  inner::fns::test_stable_vector_intrusive_mode();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_stable_vector();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_terminate(e.what());
  }
#endif
}
