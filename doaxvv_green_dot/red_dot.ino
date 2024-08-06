#include <Mouse.h>
#include <Time.h>
#include <math.h>
#include <assert.h>

#include <inttypes.h>
#include <avr/io.h>

struct fo_to_unsigned {
  constexpr uint16_t operator ()(int16_t x) const {
    return static_cast<uint16_t>(x);
  }
  constexpr uint16_t operator ()(uint16_t x) const {
    return x;
  }

  constexpr uint32_t operator ()(int32_t x) const {
    return static_cast<uint32_t>(x);
  }
  constexpr uint32_t operator ()(uint32_t x) const {
    return x;
  }

  constexpr uint64_t operator ()(int64_t x) const {
    return static_cast<uint64_t>(x);
  }
  constexpr uint64_t operator ()(uint64_t x) const {
    return x;
  }
};
constexpr fo_to_unsigned to_unsigned{};

class TrueRandomClass {
  public:
    int rand();
    long random();
    long random(long howBig);
    long random(long howsmall, long how);
    int randomBit(void);
    char randomByte(void);
    void memfill(char* location, int size);
    void mac(uint8_t* macLocation);
    void uuid(uint8_t* uuidLocation);
  private:
    int randomBitRaw(void);
    int randomBitRaw2(void);
};
int TrueRandomClass::randomBitRaw(void) {
  uint8_t copyAdmux, copyAdcsra, copyAdcsrb, copyPortc, copyDdrc;
  uint16_t i;
  uint8_t bit;
  volatile uint8_t dummy;
  
  // Store all the registers we'll be playing with
  copyAdmux = ADMUX;
  copyAdcsra = ADCSRA;
  copyAdcsrb = ADCSRB;
  copyPortc = PORTC;
  copyDdrc = DDRC;
  
  // Perform a conversion on Analog0, using the Vcc reference
  ADMUX = _BV(REFS0);
  
#if F_CPU > 16000000
  // ADC is enabled, divide by 32 prescaler
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
#elif F_CPU > 8000000
  // ADC is enabled, divide by 16 prescaler
  ADCSRA = _BV(ADEN) | _BV(ADPS2);
#else
  // ADC is enabled, divide by 8 prescaler
  ADCSRA = _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0);
#endif

  // Autotriggering disabled
  ADCSRB = 0;

  // Pull Analog0 to ground
  PORTC &=~_BV(0);
  DDRC |= _BV(0);
  // Release Analog0, apply internal pullup
  DDRC &= ~_BV(0);
  PORTC |= _BV(1);
  // Immediately start a sample conversion on Analog0
  ADCSRA |= _BV(ADSC);
  // Wait for conversion to complete
  while (ADCSRA & _BV(ADSC)) PORTC ^= _BV(0);
  // Xor least significant bits together
  bit = ADCL;
  // We're ignoring the high bits, but we have to read them before the next conversion
  dummy = ADCH;

  // Restore register states
  ADMUX = copyAdmux;
  ADCSRA = copyAdcsra;
  ADCSRB = copyAdcsrb;
  PORTC = copyPortc;
  DDRC = copyDdrc;

  return bit & 1;
}
int TrueRandomClass::randomBitRaw2(void) {
  // Software whiten bits using Von Neumann algorithm
  //
  // von Neumann, John (1951). "Various techniques used in connection
  // with random digits". National Bureau of Standards Applied Math Series
  // 12:36.
  //
  for(;;) {
    int a = randomBitRaw() | (randomBitRaw()<<1);
    if (a==1) return 0; // 1 to 0 transition: log a zero bit
    if (a==2) return 1; // 0 to 1 transition: log a one bit
    // For other cases, try again.
  }
}
int TrueRandomClass::randomBit(void) {
  // Software whiten bits using Von Neumann algorithm
  //
  // von Neumann, John (1951). "Various techniques used in connection
  // with random digits". National Bureau of Standards Applied Math Series
  // 12:36.
  //
  for(;;) {
    int a = randomBitRaw2() | (randomBitRaw2()<<1);
    if (a==1) return 0; // 1 to 0 transition: log a zero bit
    if (a==2) return 1; // 0 to 1 transition: log a one bit
    // For other cases, try again.
  }
}
char TrueRandomClass::randomByte(void) {
  char result;
  uint8_t i;
  result = 0;
  for (i=8; i--;) result += result + randomBit();
  return result;
}
int TrueRandomClass::rand() {
  int result;
  uint8_t i;
  result = 0;
  for (i=15; i--;) result += result + randomBit();
  return result;
}
long TrueRandomClass::random() {
  long result;
  uint8_t i;
  result = 0;
  for (i=31; i--;) result += result + randomBit();
  return result;
}
long TrueRandomClass::random(long howBig) {
  long randomValue;
  long maxRandomValue;
  long topBit;
  long bitPosition;
  
  if (!howBig) return 0;
  randomValue = 0;
  if (howBig & (howBig-1)) {
    // Range is not a power of 2 - use slow method
    topBit = howBig-1;
    topBit |= topBit>>1;
    topBit |= topBit>>2;
    topBit |= topBit>>4;
    topBit |= topBit>>8;
    topBit |= topBit>>16;
    topBit = (topBit+1) >> 1;

    bitPosition = topBit;
    do {
      // Generate the next bit of the result
      if (randomBit()) randomValue |= bitPosition;

      // Check if bit 
      if (randomValue >= howBig) {
        // Number is over the top limit - start again.
        randomValue = 0;
        bitPosition = topBit;
      } else {
        // Repeat for next bit
        bitPosition >>= 1;
      }
    } while (bitPosition);
  } else {
    // Special case, howBig is a power of 2
    bitPosition = howBig >> 1;
    while (bitPosition) {
      if (randomBit()) randomValue |= bitPosition;
      bitPosition >>= 1;
    }
  }
  return randomValue;
}
long TrueRandomClass::random(long howSmall, long howBig) {
  if (howSmall >= howBig) return howSmall;
  long diff = howBig - howSmall;
  return TrueRandomClass::random(diff) + howSmall;
}
void TrueRandomClass::memfill(char* location, int size) {
  for (;size--;) *location++ = randomByte();
}
void TrueRandomClass::mac(uint8_t* macLocation) {
  memfill((char*)macLocation,6);
}
void TrueRandomClass::uuid(uint8_t* uuidLocation) {
  // Generate a Version 4 UUID according to RFC4122
  memfill((char*)uuidLocation,16);
  // Although the UUID contains 128 bits, only 122 of those are random.
  // The other 6 bits are fixed, to indicate a version number.
  uuidLocation[6] = 0x40 | (0x0F & uuidLocation[6]); 
  uuidLocation[8] = 0x80 | (0x3F & uuidLocation[8]);
}
TrueRandomClass TrueRandom;


template <class T, class T2>
struct pair_t {
  T first;
  T2 second;
};
template <class UINT, UINT A, UINT C, UINT M>
class linear_congruential_engine {
  using this_t = linear_congruential_engine;

  UINT x;

public:
  using result_type = UINT;

  static constexpr result_type multiplier = A;
  static constexpr result_type increment = C;
  static constexpr result_type modulus = M;
  static constexpr result_type min_v() {
    return C == 0u ? 1u: 0u;
  }
  static constexpr result_type max_v() {
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
  <uint32_t, 16807, 0, 2147483647>;
using minstd_rand = linear_congruential_engine
  <uint32_t, 48271, 0, 2147483647>;

template <class INT = int>
class uniform_int_distribution {
  using this_t = uniform_int_distribution;

  INT x;
  INT y;

public:
  using result_type = INT;
  using param_type = pair_t<INT, INT>;

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

  explicit uniform_int_distribution(INT a, INT b) : x(a), y(b) {}
  explicit uniform_int_distribution(const param_type &p)
    : x(p.first), y(p.second) {}
  void reset() {}

  friend bool operator ==(const this_t &a, const this_t &b) {
    return a.x == b.x && a.y == b.y;
  }
  friend bool operator !=(const this_t &a, const this_t &b) {
    return !(a == b);
  }

  template <class G>
  result_type operator ()(G &g) {
    using uint_t = typename G::result_type;
    const auto g_dif = to_unsigned(g.max_v() - g.min_v());
    const auto dif = to_unsigned(y - x);

    const uint_t len_minus_1 = static_cast<uint_t>(g_dif);
    if (dif == uint_t(-1))
      return g();
    const uint_t dis_len = static_cast<uint_t>(dif) + (uint_t)1;
    const uint_t r = len_minus_1 % dis_len;
    const uint_t discard_point = ((r == dis_len - (uint_t)1)
                                  ? len_minus_1 : (len_minus_1 - r));
    for (;;) {
      const auto k = g() - g.min_v();
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
  result_type min_v() const {
    return x;
  }
  result_type max_v() const {
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
  using param_type = pair_t<INT, INT>;

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

  friend bool operator ==(const this_t &a, const this_t &b) {
    return a.x == b.x && a.y == b.y;
  }
  friend bool operator !=(const this_t &a, const this_t &b) {
    return !(a == b);
  }

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
  result_type min_v() const {
    return 0;
  }
  result_type max_v() const {
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

  static constexpr result_type min_v() {
    return E::min_v();
  }
  static constexpr result_type max_v() {
    return E::max_v();
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

  result_type operator ()(result_type i, result_type j) {
    return uniform_int_distribution<result_type>(i, j)(e);
  }

  bool gen_bool(result_type num, result_type den) {
    return rational_bernoulli_distribution<result_type>(num, den)(e);
  }
};
struct fo_make_rander {
  using E = minstd_rand;
  rander<E> operator ()() const {
    return rander<E>(static_cast<typename E::result_type>
                     (TrueRandom.random()));
  }
};
constexpr fo_make_rander make_rander{};


struct coord_t {
  double x;
  double y;
  bool operator ==(coord_t z) const {
    return x == z.x && y == z.y;
  }
  bool operator !=(coord_t z) const {
    return !(x == z.x && y == z.y);
  }
};
struct actual_coord_t {
  int32_t x;
  int32_t y;
};

rander<minstd_rand> rnd = make_rander();
coord_t pos{};
actual_coord_t actual_pos{};

void sync_two_pos_for_one_step() {
  const int32_t dest_x = static_cast<int32_t>(round(pos.x));
  const int32_t dest_y = static_cast<int32_t>(round(pos.y));
  Mouse.move(static_cast<signed char>(dest_x - actual_pos.x),
             static_cast<signed char>(dest_y - actual_pos.y), 0);
  actual_pos.x = dest_x;
  actual_pos.y = dest_y;
}
void move_to(coord_t dest_pos) {
  static const double a[] = {
    0.02, 0.03, 0.05, 0.07, 0.075,
    0.1, 0.2, 0.3, 0.4, 0.5,
    0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85,
    0.86, 0.87, 0.88, 0.89, 0.9,
    0.91, 0.92, 0.93, 0.94, 0.95,
    0.96, 0.97, 0.98, 0.99, 0.995
  };
  const size_t a_sz = sizeof(a) / sizeof(double);
  uint32_t n = 0;
  while (pos != dest_pos) {
    ++n;
    const double k = (n < a_sz) ? a[n] : 1.0;
    const double x_dif = dest_pos.x - pos.x;
    const double y_dif = dest_pos.y - pos.y;
    const double dif_square = x_dif * x_dif + y_dif * y_dif;
    const double dif = sqrt(dif_square);
    const double move_count_for_1s = 60;
    const double base_speed
      = dif / move_count_for_1s * (static_cast<double>(rnd(7000, 14000))
                                   / 10000.0);
    double speed = k * base_speed * (static_cast<double>(rnd(7000, 14000))
                                     / 10000.0);
    if (dif_square <= 10.0) {
      pos.x = dest_pos.x;
      pos.y = dest_pos.y;
      break;
    }
    if (speed < 8.0)
      speed = 8.0;

    double x_move = speed * (x_dif / dif)
      * (static_cast<double>(rnd(8000, 12000))
         / 10000.0);
    double y_move = speed * (y_dif / dif)
      * (static_cast<double>(rnd(8000, 12000))
         / 10000.0);
    if (x_move < -125.0)
      x_move = -125.0;
    else if (x_move > 125.0)
      x_move = 125.0;
    if (y_move < -125.0)
      y_move = -125.0;
    else if (y_move > 125.0)
      y_move = 125.0;
    pos.x += x_move;
    pos.y += y_move;

    sync_two_pos_for_one_step();
    delay(17);
  }
}
void move_to(int32_t x, int32_t y) {
  move_to(coord_t{static_cast<double>(x), static_cast<double>(y)});
}
void move_to(actual_coord_t l_top, actual_coord_t r_bottom) {
  const int32_t w = r_bottom.x - l_top.x;
  const int32_t h = r_bottom.y - l_top.y;
  move_to(static_cast<int32_t>(rnd(l_top.x, l_top.x + w)),
          static_cast<int32_t>(rnd(l_top.y, l_top.y + h)));
}
void click() {
  Mouse.click();
}
void delay_random(int32_t min_v, int32_t max_v) {
  delay(static_cast<unsigned long>(rnd(min_v, max_v)));
}

// start_botton
// 1280, 761
// 1664, 865
//
// confir_botton
// 1033, 834
// 1112, 917
//
// 0.751

enum class task_ctg {
  clear_green_dot,
  delete_nr,
  clear_mailbox
};
task_ctg task_select = task_ctg::clear_green_dot;

void setup() {
  Mouse.begin();
  Serial.begin(9600);

  delay(10000);

  const auto i = Serial.parseInt();
  if (i == 1)
    task_select = task_ctg::clear_green_dot;
  else if (i == 2)
    task_select = task_ctg::delete_nr;
  else if (i == 3)
    task_select = task_ctg::clear_mailbox;
  else {
    Serial.println("wrong input");
    for (;;)
      ;
  }

  move_to(-6666, -6666);
  assert(pos == coord_t({-6666.0, -6666.0}));
  assert(actual_pos.x = -6666);
  assert(actual_pos.y = -6666);
  pos = coord_t{};
  actual_pos = actual_coord_t{};
}
void loop() {
  static bool y = false;
  if (!y) {
    y = true;
    if (task_select == task_ctg::clear_green_dot) {
      actual_coord_t start_button_left_top{
        static_cast<int32_t>(round(1280 / 0.751)),
        static_cast<int32_t>(round(761 / 0.751)),
      };
      actual_coord_t start_button_right_bottom{
        static_cast<int32_t>(round(1664 / 0.751)),
        static_cast<int32_t>(round(865 / 0.751)),
      };
      actual_coord_t start_button2_left_top{
        static_cast<int32_t>(round(1437 / 0.751)),
        static_cast<int32_t>(round(734 / 0.751)),
      };
      actual_coord_t start_button2_right_bottom{
        static_cast<int32_t>(round(1803 / 0.751)),
        static_cast<int32_t>(round(823 / 0.751)),
      };
      actual_coord_t confirm_button_left_top{
        static_cast<int32_t>(round(1033 / 0.751)),
        static_cast<int32_t>(round(834 / 0.751)),
      };
      actual_coord_t confirm_button_right_bottom{
        static_cast<int32_t>(round(1112 / 0.751)),
        static_cast<int32_t>(round(917 / 0.751)),
      };

      move_to(start_button_left_top.x, start_button_left_top.y);
      delay_random(1000, 1500);
      move_to(start_button_right_bottom.x, start_button_right_bottom.y);
      delay_random(1000, 1500);
      move_to(confirm_button_left_top.x, confirm_button_left_top.y);
      delay_random(1000, 1500);
      move_to(confirm_button_right_bottom.x, confirm_button_right_bottom.y);
      delay_random(1000, 1500);

      move_to(start_button_left_top, start_button_right_bottom);
      delay_random(50, 100);
      click();

      delay_random(1000, 1300);
      move_to(confirm_button_left_top, confirm_button_right_bottom);
      delay_random(50, 100);
      click();
      delay_random(1000, 1300);
      click();

      delay_random(5500, 6500);
      click();
      delay_random(1500, 2000);

      for (;;) {
        move_to(start_button2_left_top, start_button2_right_bottom);
        delay_random(50, 100);
        click();

        delay_random(1000, 1300);
        move_to(confirm_button_left_top, confirm_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(1000, 1300);
        click();

        delay_random(5500, 6500);
        click();
        delay_random(1500, 2000);
      }
    }
    else if (task_select == task_ctg::delete_nr) {
      actual_coord_t select_button_left_top{
        static_cast<int32_t>(round(619 / 0.751)),
        static_cast<int32_t>(round(880 / 0.751)),
      };
      actual_coord_t select_button_right_bottom{
        static_cast<int32_t>(round(807 / 0.751)),
        static_cast<int32_t>(round(921 / 0.751)),
      };
      actual_coord_t start_button_left_top{
        static_cast<int32_t>(round(1385 / 0.751)),
        static_cast<int32_t>(round(898 / 0.751)),
      };
      actual_coord_t start_button_right_bottom{
        static_cast<int32_t>(round(1628 / 0.751)),
        static_cast<int32_t>(round(961 / 0.751)),
      };
      actual_coord_t confirm_button_left_top{
        static_cast<int32_t>(round(1020 / 0.751)),
        static_cast<int32_t>(round(872 / 0.751)),
      };
      actual_coord_t confirm_button_right_bottom{
        static_cast<int32_t>(round(1096 / 0.751)),
        static_cast<int32_t>(round(936 / 0.751)),
      };
      actual_coord_t end_button_left_top{
        static_cast<int32_t>(round(915 / 0.751)),
        static_cast<int32_t>(round(838 / 0.751)),
      };
      actual_coord_t end_button_right_bottom{
        static_cast<int32_t>(round(997 / 0.751)),
        static_cast<int32_t>(round(915 / 0.751)),
      };
      for (;;) {
        move_to(select_button_left_top, select_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(700, 1000);

        move_to(start_button_left_top, start_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(300, 600);

        move_to(confirm_button_left_top, confirm_button_right_bottom);
        delay_random(500, 700);
        click();
        delay_random(1000, 1300);
        click();
        delay_random(3500, 4200);

        move_to(end_button_left_top, end_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(500, 1000);
      }
    }
    else if (task_select == task_ctg::clear_mailbox) {
      actual_coord_t select_button_left_top{
        static_cast<int32_t>(round(1128 / 0.751)),
        static_cast<int32_t>(round(926 / 0.751)),
      };
      actual_coord_t select_button_right_bottom{
        static_cast<int32_t>(round(1339 / 0.751)),
        static_cast<int32_t>(round(978 / 0.751)),
      };
      actual_coord_t confirm_button_left_top{
        static_cast<int32_t>(round(1029 / 0.751)),
        static_cast<int32_t>(round(840 / 0.751)),
      };
      actual_coord_t confirm_button_right_bottom{
        static_cast<int32_t>(round(1110 / 0.751)),
        static_cast<int32_t>(round(913 / 0.751)),
      };
      actual_coord_t confirm2_button_left_top{
        static_cast<int32_t>(round(918 / 0.751)),
        static_cast<int32_t>(round(871 / 0.751)),
      };
      actual_coord_t confirm2_button_right_bottom{
        static_cast<int32_t>(round(1001 / 0.751)),
        static_cast<int32_t>(round(936 / 0.751)),
      };

      for (;;) {
        move_to(select_button_left_top, select_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(600, 800);

        move_to(confirm_button_left_top, confirm_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(5000, 5700);

        move_to(confirm2_button_left_top, confirm2_button_right_bottom);
        delay_random(50, 100);
        click();
        delay_random(600, 800);
      }
    }
    else
      ;
  }
}
