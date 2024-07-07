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

void test_priority_queue() {
  struct comp_t {
    bool y = true;

    bool operator ()(int a, int b) const {
      return y ? a < b : b < a;
    }
  };
  comp_t greater_f{false};
  comp_t less_f{true};

  // main operations
  {
    {
      priority_queue<int> q;
      q.push_range(seq(1, 3, 2));
      assert(q.top() == 3);
    }
    {
      priority_queue<int> q;
      vector<int> v(irng(0, 1000));
      shuffle(v, minstd_rand{});
      for (auto &x : v)
        q.push(x);
      assert(q.top() == 999);
      q.pop();
      assert(q.top() == 998);
    }
    {
      vector<int> v = {9, 5, 7};
      int a[] = {1, 3, 2, 0};
      priority_queue<int, vector<int>, comp_t> q(begin(a), end(a), less_f, v);
      assert(q.top() == 9);
      assert(is_permutation(inner::base(q), seq(0, 1, 2, 3, 5, 7, 9)));
      assert(is_heap(inner::base(q), less_f));
      priority_queue<int, vector<int>, comp_t>
        q2(begin(a), end(a), greater_f, move(v));
      assert(q2.top() == 0);
      assert(is_permutation(inner::base(q2), seq(0, 1, 2, 3, 5, 7, 9)));
      assert(is_heap(inner::base(q2), greater_f));
    }
    {
      stateful_test_allocator<int> a1, a2;
      priority_queue<int, vector<int, stateful_test_allocator<int>>> q(a1);
      assert(inner::base(q).get_allocator() == a1);
      q.push(1);
      q.push(0);
      priority_queue<int, vector<int, stateful_test_allocator<int>>> q2(q, a2);
      assert(inner::base(q2).get_allocator() == a2);
      assert(inner::base(q) == inner::base(q2));
      priority_queue
        <int, vector<int, stateful_test_allocator<int>>> q3(move(q), a2);
      assert(q.empty());
      assert(q.size() == 0);
      assert(inner::base(q3) == inner::base(q2));
      priority_queue<int, vector<int, stateful_test_allocator<int>>, comp_t>
        q4(greater_f, a2);
      assert(inner::base(q4).get_allocator() == a2);
      vector<int, stateful_test_allocator<int>> v = {1, 3, 2};
      priority_queue<int, vector<int, stateful_test_allocator<int>>, comp_t>
        q5(greater_f, v, a1);
      assert(q5.top() == 1);
      assert(inner::base(q5).get_allocator() == a1);
      priority_queue<int, vector<int, stateful_test_allocator<int>>, comp_t>
        q6(less_f, move(v), a1);
      assert(q6.top() == 3);
      assert(v.empty());
      q6.pop();
      assert(q6.top() == 2);
      q6.pop();
      assert(q6.top() == 1);
      q6.pop();
      assert(q6.empty());
    }
  }

  // priority_queue(a)
  // priority_queue(this_ref, a)
  {
    vector<int> v = {1, 3, 2};
    {
      priority_queue<int> q(v.get_allocator());
      assert(inner::base(q).get_allocator() == v.get_allocator());
    }
    {
      priority_queue<int> q0(less<int>{}, v, v.get_allocator());

      priority_queue<int> q(q0, v.get_allocator());
      assert(q.top() == 3);
      assert(inner::base(q).get_allocator() == v.get_allocator());

      priority_queue<int> q2(move(q), v.get_allocator());
      assert(q.empty());
      assert(q2.top() == 3);
      assert(inner::base(q).get_allocator() == v.get_allocator());
    }
  }

  // priority_queue(cmp)
  // priority_queue(cmp, c)
  // priority_queue(cmp, alloc)
  // priority_queue(cmp, c, alloc)
  {
    {
      vector<int> v = {1, 3, 2};
      priority_queue<int, vector<int>, comp_t> q0(greater_f);
      assert(q0.empty());
      priority_queue<int, vector<int>, comp_t> q(greater_f, v);
      assert(q.top() == 1);
      priority_queue<int, vector<int>, comp_t> q2(less_f, move(v));
      assert(q2.top() == 3);
      assert(v.empty());
    }
    {
      vector<int> v = {1, 3, 2};
      priority_queue<int, vector<int>, comp_t> q0(greater_f, v.get_allocator());
      assert(q0.empty());
      priority_queue<int, vector<int>, comp_t> q(greater_f, v,
                                                 v.get_allocator());
      assert(q.top() == 1);
      priority_queue<int, vector<int>, comp_t> q2(less_f, move(v),
                                                  v.get_allocator());
      assert(q2.top() == 3);
      assert(v.empty());
    }
  }

  // priority_queue(cit, cit)
  // priority_queue(cit, cit, cmp)
  // priority_queue(cit, cit, cmp, c)
  // priority_queue(cit, cit, alloc)
  // priority_queue(cit, cit, cmp, alloc)
  // priority_queue(cit, cit, cmp, c, alloc)
  {
    const vector<int> v = {2, 3, 1};
    {
      priority_queue<int> q(v.begin(), v.end());
      assert(q.top() == 3);
    }
    {
      priority_queue<int> q(v.begin(), v.end(), less<int>{});
      assert(q.top() == 3);
    }
    {
      const vector<int> v2 = {0, 4};
      priority_queue<int> q(v.begin(), v.end(), less<int>{}, v2);
      assert(q.top() == 4);
    }
    {
      vector<int> v2 = {0, 4};
      priority_queue<int> q(v.begin(), v.end(), less<int>{}, move(v2));
      assert(q.top() == 4);
      assert(v2.empty());
    }

    {
      priority_queue<int> q(v.begin(), v.end(), v.get_allocator());
      assert(q.top() == 3);
    }
    {
      priority_queue<int> q(v.begin(), v.end(), less<int>{}, v.get_allocator());
      assert(q.top() == 3);
    }
    {
      const vector<int> v2 = {0, 4};
      priority_queue<int> q(v.begin(), v.end(), less<int>{}, v2,
                            v.get_allocator());
      assert(q.top() == 4);
    }
    {
      vector<int> v2 = {0, 4};
      priority_queue<int> q(v.begin(), v.end(), less<int>{}, move(v2),
                            v.get_allocator());
      assert(q.top() == 4);
      assert(v2.empty());
    }
  }

  // priority_queue(from_range, c, cmp, alloc)
  // priority_queue(from_range, c, alloc)
  // priority_queue(from_range, c, cmp)
  // priority_queue(from_range, c)
  {
    vector<int> v(irng(0, 100));
    shuffle(v, minstd_rand{});
    {
      priority_queue<int> q(from_range, v, less<int>{}, v.get_allocator());
      assert(q.top() == 99);
    }
    {
      priority_queue<int> q(from_range, v, v.get_allocator());
      assert(q.top() == 99);
    }
    {
      priority_queue<int, vector<int>, ez_function<bool (int, int)>>
        q(from_range, v, greater<int>{});
      assert(q.top() == 0);
    }
    {
      priority_queue<int, vector<int>>
        q(from_range, v);
      assert(q.top() == 99);
    }
  }
}
void test_stack() {
  // main operations
  {
    {
      stack<int> q;
      q.push_range(seq(1, 3, 2));
      stack<int> q2(deque<int>{1, 3, 2});
      assert(q == q2);
    }
    {
      using V = deque<int, stateful_test_allocator<int>>;
      stack<int, V> v;
      v.push(1);
      assert(v.top() == 1);
      v.push(2);
      assert(v.top() == 2);
      assert(v.emplace(3) == 3);
      assert(v.top() == 3);
      assert(equal(inner::base(v), irng(1, 4)));
      assert(v.size() == 3);
      assert(!v.empty());
      v.pop();
      v.pop();
      assert(v.size() == 1);
      assert(v.top() == 1);
      v.pop();
      assert(v.size() == 0);
      assert(v.empty());
    }
    {
      using V = deque<int, stateful_test_allocator<int>>;
      V v = {3, 2, 1, 0};
      assert(equal(v, rrng(irng(0, 4))));
      stack<int, V> s(move(v));
      assert(v.empty());
      assert(equal(inner::base(s), rrng(irng(0, 4))));
      stack<int, V> s2(V{5, 6, 7});
      assert(s2.size() == 3);
      assert(s2.top() == 7);
      s2.pop();
      assert(s2.top() == 6);
      s2.pop();
      s2.pop();
      assert(s2.empty());
    }
    {
      stack<string> s;
      s.push("abc");
      assert(s.top() == "abc");
      string str("def");
      s.push(str);
      assert(s.top() == "def");
      s.push(move(str));
      assert(s.top() == "def");
      assert(str.empty());
    }
  }
  // stack(a)
  // stack(this_ref, a)
  {
    using v_t = stack<int, vector<int, stateful_test_allocator<int>>>;
    stateful_test_allocator<int> a1, a2;
    v_t v(a1);
    v.push(1);
    v.push(2);
    v.push(3);
    assert(inner::base(v).get_allocator() == a1);
    assert(v == v_t(from_range, irng(1, 4)));

    v_t v2(v, a2);
    assert(inner::base(v).get_allocator() == a1);
    assert(v == v_t(from_range, irng(1, 4)));
    assert(v == v2);
    assert(inner::base(v2).get_allocator() == a2);

    v_t v3(move(v2), a1);
    assert(v2.empty());
    assert(equal(inner::base(v3), irng(1, 4)));
    assert(inner::base(v3).get_allocator() == a1);
  }
  // stack(c)
  // stack(c, a)
  {
    {
      vector<int> v = {1, 2, 3};
      stack<int, vector<int>> s(v);
      assert(equal(v, irng(1, 4)));
      assert(equal(inner::base(s), irng(1, 4)));
    }
    {
      vector<int> v = {1, 2, 3};
      stack<int, vector<int>> s(move(v));
      assert(v.empty());
      assert(equal(inner::base(s), irng(1, 4)));
    }
    {
      using alloc_t = stateful_test_allocator<int>;
      using v_t = vector<int, alloc_t>;
      using s_t = stack<int, v_t>;
      alloc_t a1, a2;
      v_t v({1, 2, 3}, a1);
      s_t s(v, a2);
      assert(equal(v, irng(1, 4)));
      assert(v.get_allocator() == a1);
      assert(equal(inner::base(s), irng(1, 4)));
      assert(inner::base(s).get_allocator() == a2);
    }
    {
      using alloc_t = stateful_test_allocator<int>;
      using v_t = vector<int, alloc_t>;
      using s_t = stack<int, v_t>;
      alloc_t a1, a2;
      v_t v({1, 2, 3}, a1);
      s_t s(move(v), a2);
      assert(v.empty() && v.get_allocator() == a1);
      assert(equal(inner::base(s), irng(1, 4)));
      assert(inner::base(s).get_allocator() == a2);
    }
  }
  // stack(from, to)
  // stack(from, to, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    using v_t = vector<int, alloc_t>;
    using s_t = stack<int, v_t>;

    const auto r = irng(1, 4);

    const s_t s(begin(r), end(r));
    assert(equal(inner::base(s), irng(1, 4)));

    const alloc_t a;
    s_t s2(begin(r), end(r), a);
    assert(equal(inner::base(s2), irng(1, 4)));
    assert(inner::base(s2).get_allocator() == a);
  }
  // stack(from_range, r)
  // stack(from_range, r, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    using v_t = vector<int, alloc_t>;
    using s_t = stack<int, v_t>;

    const auto r = irng(1, 4);

    const s_t s(from_range, r);
    assert(equal(inner::base(s), irng(1, 4)));

    const alloc_t a;
    s_t s2(from_range, r, a);
    assert(equal(inner::base(s2), irng(1, 4)));
    assert(inner::base(s2).get_allocator() == a);
  }
}
void test_queue() {
  // main operations
  {
    {
      stack<int> q;
      q.push_range(seq(1, 3, 2));
      stack<int> q2(deque<int>{1, 3, 2});
      assert(q == q2);
    }

    queue<int> q;
    q.push(3);
    q.push(4);
    const auto &x = inner::base(q);
    assert(equal(x, seq(3, 4)));
    {
      auto y = inner::base(move(q));
      assert(equal(y, seq(3, 4)));
    }
    assert(q.empty());
    q.push(3);
    q.emplace(4);
    q.pop();
    assert(q.front() == 4);
    assert(q.back() == 4);
    assert(q.size() == 1);
    assert(!q.empty());

    queue<int> q2(from_range, seq(1, 2, 3));
    queue<int> q3(from_range, seq(1, 3, 2));
    assert((q2 <=> q3) < 0);
    assert(q2 != q3);
    q3 = q2;
    assert((q2 <=> q3) == 0);
    assert(q2 == q3);
  }
  // queue(a)
  // queue(this_ref, a)
  {
    using v_t = queue<int, vector<int, stateful_test_allocator<int>>>;
    stateful_test_allocator<int> a1, a2;
    v_t v(a1);
    v.push(1);
    v.push(2);
    v.push(3);
    assert(inner::base(v).get_allocator() == a1);
    assert(v == v_t(from_range, irng(1, 4)));

    v_t v2(v, a2);
    assert(inner::base(v).get_allocator() == a1);
    assert(v == v_t(from_range, irng(1, 4)));
    assert(v == v2);
    assert(inner::base(v2).get_allocator() == a2);

    v_t v3(move(v2), a1);
    assert(v2.empty());
    assert(equal(inner::base(v3), irng(1, 4)));
    assert(inner::base(v3).get_allocator() == a1);
  }
  // queue(c)
  // queue(c, a)
  {
    {
      vector<int> v = {1, 2, 3};
      queue<int, vector<int>> s(v);
      assert(equal(v, irng(1, 4)));
      assert(equal(inner::base(s), irng(1, 4)));
    }
    {
      vector<int> v = {1, 2, 3};
      queue<int, vector<int>> s(move(v));
      assert(v.empty());
      assert(equal(inner::base(s), irng(1, 4)));
    }
    {
      using alloc_t = stateful_test_allocator<int>;
      using v_t = vector<int, alloc_t>;
      using s_t = queue<int, v_t>;
      alloc_t a1, a2;
      v_t v({1, 2, 3}, a1);
      s_t s(v, a2);
      assert(equal(v, irng(1, 4)));
      assert(v.get_allocator() == a1);
      assert(equal(inner::base(s), irng(1, 4)));
      assert(inner::base(s).get_allocator() == a2);
    }
    {
      using alloc_t = stateful_test_allocator<int>;
      using v_t = vector<int, alloc_t>;
      using s_t = queue<int, v_t>;
      alloc_t a1, a2;
      v_t v({1, 2, 3}, a1);
      s_t s(move(v), a2);
      assert(v.empty() && v.get_allocator() == a1);
      assert(equal(inner::base(s), irng(1, 4)));
      assert(inner::base(s).get_allocator() == a2);
    }
  }
  // queue(from, to)
  // queue(from, to, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    using v_t = vector<int, alloc_t>;
    using s_t = queue<int, v_t>;

    const auto r = irng(1, 4);

    const s_t s(begin(r), end(r));
    assert(equal(inner::base(s), irng(1, 4)));

    const alloc_t a;
    s_t s2(begin(r), end(r), a);
    assert(equal(inner::base(s2), irng(1, 4)));
    assert(inner::base(s2).get_allocator() == a);
  }
  // queue(from_range, r)
  // queue(from_range, r, a)
  {
    using alloc_t = stateful_test_allocator<int>;
    using v_t = vector<int, alloc_t>;
    using s_t = queue<int, v_t>;

    const auto r = irng(1, 4);

    const s_t s(from_range, r);
    assert(equal(inner::base(s), irng(1, 4)));

    const alloc_t a;
    s_t s2(from_range, r, a);
    assert(equal(inner::base(s2), irng(1, 4)));
    assert(inner::base(s2).get_allocator() == a);
  }
}

void test_stack_and_queue() {
  printf("container adaptors - stack and queue: ");

  inner::fns::test_priority_queue();
  inner::fns::test_stack();
  inner::fns::test_queue();

  printf("ok\n");
}

}

int main() {
  using namespace re;
#ifndef RE_NOEXCEPT
  try {
#endif
    inner::fns::test_stack_and_queue();
#ifndef RE_NOEXCEPT
  }
  catch (const exception &e) {
    print_then_abort(e.what());
  }
#endif
}
