#ifndef RE_DEFINED_RANDOM_H
#define RE_DEFINED_RANDOM_H

namespace re {

template <class UINT, UINT A, UINT C, UINT M>
class linear_congruential_engine;
template <class UINT, UINT A, UINT C, UINT M>
requires (is_same_v<UINT, uint32_t>
          || is_same_v<UINT, uint_fast32_t>
          || is_same_v<UINT, uint_least32_t>)
class linear_congruential_engine<UINT, A, C, M> {
  using this_t = linear_congruential_engine;

  UINT x;

public:
  using result_type = UINT;

  static constexpr result_type multiplier = A;
  static constexpr result_type increment = C;
  static constexpr result_type modulus = M;
  static constexpr result_type min() {
    return C == 0u ? 1u: 0u;
  }
  static constexpr result_type max() {
    return M - 1u;
  }
  static constexpr result_type default_seed = 1u;

  linear_congruential_engine() : linear_congruential_engine(default_seed) {}
  ~linear_congruential_engine() = default;
  linear_congruential_engine(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  linear_congruential_engine(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend void swap(this_t &a, this_t &b) {
    adl_swap(a.x, b.x);
  }
  friend bool operator ==(const this_t &a, const this_t &b) {
    return a.x == b.x;
  }

  explicit linear_congruential_engine(result_type s) {
    if ((C % M == 0) && (s % M == 0))
      x = 1;
    else
      x = s % M;
  }
  void seed(result_type s = default_seed) {
    *this = this_t(s);
  }

  result_type operator()() {
    return x = (A * static_cast<uint64_t>(x) + C) % M;
  }
  void discard(unsigned long long z) {
    for (; z != 0; --z)
      operator ()();
  }
};
using minstd_rand0 = linear_congruential_engine
  <uint_fast32_t, 16807, 0, 2147483647>;
using minstd_rand = linear_congruential_engine
  <uint_fast32_t, 48271, 0, 2147483647>;

template <class UINT,
          size_t W, size_t N, size_t M, size_t R,
          UINT A, size_t U, UINT D, size_t S,
          UINT B, size_t T,
          UINT C, size_t L, UINT F>
class mersenne_twister_engine {
  using this_t = mersenne_twister_engine;

  UINT m[N];
  size_t index;
  static constexpr UINT lower_mask = (UINT)((UINT)1u << (UINT)R) - (UINT)1u;
  static constexpr UINT ww = (UINT)(sizeof(UINT) * 8u) - (UINT)W;
  static constexpr UINT upper_mask = (UINT)((UINT)~lower_mask << ww) >> ww;

  static UINT lower_w_bits(UINT x) {
    return (UINT)(x << ww) >> ww;
  }

public:
  using result_type = UINT;

  static constexpr size_t word_size = W;
  static constexpr size_t state_size = N;
  static constexpr size_t shift_size = M;
  static constexpr size_t mask_bits = R;

  static constexpr UINT xor_mask = A;
  static constexpr size_t tempering_u = U;
  static constexpr UINT tempering_d = D;
  static constexpr size_t tempering_s = S;
  static constexpr UINT tempering_b = B;
  static constexpr size_t tempering_t = T;
  static constexpr UINT tempering_c = C;
  static constexpr size_t tempering_l = L;
  static constexpr UINT initialization_multiplier = F;
  static constexpr result_type min() {
    return 0;
  }
  static constexpr result_type max() {
    if constexpr (W == sizeof(result_type) * 8u) {
      return ~(result_type)0;
    }
    else {
      return (result_type)(((result_type)1u) << W) - (result_type)1;
    }
  }
  static constexpr result_type default_seed = 5489u;

  mersenne_twister_engine() : mersenne_twister_engine(default_seed) {}
  ~mersenne_twister_engine() = default;
  mersenne_twister_engine(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  mersenne_twister_engine(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend void swap(this_t &x, this_t &y) noexcept {
    default_swap(x, y);
  }

  explicit mersenne_twister_engine(result_type value) {
    seed(value);
  }

  void seed(result_type value = default_seed) {
    index = N;
    m[0] = value;
    for (auto &i : iters(1u, N))
      m[i] = lower_w_bits(F * (UINT)(m[i - 1]
                                     ^ (UINT)(m[i - 1] >> (UINT)(W - 2u)))
                          + i);
  }

  friend bool operator ==(const this_t &x, const this_t &y) = default;
  result_type operator()() {
    if (index >= N) {
      for (auto &i : iters(0u, N)) {
        UINT x = (UINT)(m[i] & upper_mask)
          | (UINT)(m[(i + 1u) % N] & lower_mask);
        UINT x_a = (UINT)(x >> 1u);
        if (x % 2u != 0)
          x_a = (UINT)(x_a ^ A);
        m[i] = m[(i + M) % N] ^ x_a;
      }
      index = 0;
    }

    UINT y = m[index];
    y = y ^ (UINT)((UINT)(y >> (UINT)U) & (UINT)D);
    y = y ^ (UINT)((UINT)(y << (UINT)S) & (UINT)B);
    y = y ^ (UINT)((UINT)(y << (UINT)T) & (UINT)C);
    y = y ^ (UINT)(y >> (UINT)L);
 
    ++index;
    return lower_w_bits(y);
  }
  void discard(unsigned long long z) {
    for (; z != 0; --z)
      operator ()();
  }
};
using mt19937 = mersenne_twister_engine
  <uint_fast32_t, 32, 624, 397, 31,
   0x9908'b0df, 11, 0xffff'ffff, 7, 0x9d2c'5680, 15, 0xefc6'0000, 18,
   1'812'433'253>;
using mt19937_64 = mersenne_twister_engine
  <uint_fast64_t, 64, 312, 156, 31,
   0xb502'6f5a'a966'19e9, 29, 0x5555'5555'5555'5555, 17,
   0x71d6'7fff'eda6'0000, 37, 0xfff7'eee0'0000'0000, 43,
   6'364'136'223'846'793'005>;

template <class INT = int>
class uniform_int_distribution {
  using this_t = uniform_int_distribution;

  INT x;
  INT y;

public:
  using result_type = INT;
  using param_type = pair<INT, INT>;

  uniform_int_distribution() : uniform_int_distribution(0) {}
  ~uniform_int_distribution() = default;
  uniform_int_distribution(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  uniform_int_distribution(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend void swap(this_t &a, this_t &b) noexcept {
    adl_swap(a.x, b.x);
    adl_swap(a.y, b.y);
  }

  explicit uniform_int_distribution(INT a, INT b = numeric_limits<INT>::max())
    : x(a), y(b) {}
  explicit uniform_int_distribution(const param_type &p)
    : x(p.first), y(p.second) {}
  void reset() {}

  friend bool operator ==(const this_t &, const this_t &) = default;

  template <class G>
  result_type operator ()(G &g) {
    using uint_t = typename G::result_type;
    const auto g_dif = to_unsigned(g.max() - g.min());
    const auto dif = to_unsigned(y - x);
    if (dif > g_dif)
      throw_or_abort<logic_error>
        ("re::uniform_int_distribution::operator (): "
         "distribute small to big\n");

    const uint_t len_minus_1 = static_cast<uint_t>(g_dif);
    if (dif == to_unsigned(numeric_limits<uint_t>::max()))
      return g();
    const uint_t dis_len = static_cast<uint_t>(dif) + (uint_t)1;
    const uint_t r = len_minus_1 % dis_len;
    const uint_t discard_point = ((r == dis_len - (uint_t)1)
                                  ? len_minus_1 : (len_minus_1 - r));
    for (;;) {
      const auto k = g() - g.min();
      if (k > discard_point)
        continue;
      return x + static_cast<result_type>(k % dis_len);
    }
  }
  template <class G>
  result_type operator ()(G &g, const param_type &p) {
    return this_t(p)(g);
  }

  result_type a() const {
    return x;
  }
  result_type b() const {
    return y;
  }
  param_type param() const {
    return {x, y};
  }
  void param(const param_type &p) {
    x = p.first;
    y = p.second;
  }
  result_type min() const {
    return x;
  }
  result_type max() const {
    return y;
  }
};

template <class INT = int>
class rational_bernoulli_distribution {
  using this_t = rational_bernoulli_distribution;

  INT x;
  INT y;

public:
  using result_type = bool;
  using param_type = pair<INT, INT>;

  rational_bernoulli_distribution() : rational_bernoulli_distribution(1, 2) {}
  ~rational_bernoulli_distribution() = default;
  rational_bernoulli_distribution(const this_t &) = default;
  this_t &operator =(const this_t &) = default;
  rational_bernoulli_distribution(this_t &&) = default;
  this_t &operator =(this_t &&) = default;
  friend void swap(this_t &a, this_t &b) noexcept {
    adl_swap(a.x, b.x);
    adl_swap(a.y, b.y);
  }

  explicit rational_bernoulli_distribution(INT a, INT b) : x(a), y(b) {}
  explicit rational_bernoulli_distribution(const param_type &p)
    : x(p.first), y(p.second) {}
  void reset() {}

  friend bool operator ==(const this_t &, const this_t &) = default;

  template <class G>
  result_type operator ()(G &g) {
    const auto a = uniform_int_distribution<INT>(1, y)(g);
    return a <= x ? true : false;
  }
  template <class G>
  result_type operator ()(G &g, const param_type &p) {
    return this_t(p)(g);
  }

  result_type a() const {
    return x;
  }
  result_type b() const {
    return y;
  }
  param_type param() const {
    return {x, y};
  }
  void param(const param_type &p) {
    x = p.first;
    y = p.second;
  }
  result_type min() const {
    return 0;
  }
  result_type max() const {
    return 1;
  }
};

template <class E = minstd_rand>
class rander {
  E e;

public:
  using result_type = typename E::result_type;
  using engine_type = E;

  rander() = default;
  ~rander() = default;
  rander(const rander &) = default;
  rander &operator =(const rander &) = default;
  rander(rander &&) = default;
  rander &operator =(rander &&) = default;
  friend constexpr void swap(rander &x, rander &y) noexcept {
    adl_swap(x.e, y.e);
  }

  explicit rander(result_type i) : e(i) {}

  E engine() const {
    return e;
  }

  static constexpr result_type min() {
    return E::min();
  }
  static constexpr result_type max() {
    return E::max();
  }
  result_type operator ()() {
    return e();
  }
  void discard() {
    e.discard();
  }
  void discard(unsigned long long z) {
    e.discard(z);
  }

  result_type operator ()(result_type i, result_type j)
    requires is_integral_v<result_type> {
    return uniform_int_distribution<result_type>(i, j)(e);
  }

  bool gen_bool(result_type num, result_type den)
    requires is_integral_v<result_type> {
    return rational_bernoulli_distribution<result_type>(num, den)(e);
  }
};

template <class E = minstd_rand>
struct fo_make_rander {
  rander<E> operator ()() const {
    return rander<E>(static_cast<typename E::result_type>(time(nullptr)));
  }
};
template <class E = minstd_rand>
inline constexpr fo_make_rander<E> make_rander{};

}

#endif
