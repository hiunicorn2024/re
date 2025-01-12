#define RE_NOEXCEPT
#include <re/core>

namespace re::vv {

using rander_t = rander<mt19937>;
struct fo_get_rander {
  rander_t &operator ()() const {
    static thread_local rander_t r
      = []() {
        static mutex mtx;
        static rander_t rander0 = make_rander<rander_t::engine_type>();
        const auto guard = lock_guard(mtx);
        return rander_t(rander0());
      }();
    return r;
  }
};
inline constexpr fo_get_rander get_rander{};
struct fo_rnd {
  bool operator ()(double p) const {
    return get_rander()(p);
  }
};
inline constexpr fo_rnd rnd{};

struct fo_put_rate {
  void operator ()(double x) const {
    if (isnan(x))
      put("0.000%");
    else {
      put(x * 100, setprecision(3));
      put("%");
    }
  }
};
inline constexpr fo_put_rate put_rate{};

struct girl;
struct team;
enum a_cond {
  atk_before,
  atk_after,
  dfd_before,
  dfd_after,
  atk_dfd_before,
  atk_dfd_after
};
using a_fn_t = function<void (team &, girl &, team &, girl &, bool)>;
enum class active_color {
  pow, tec, stm, apl
};
struct a_t {
  a_cond first;
  a_fn_t second;
  pair<active_color, double> v1 = pair(active_color::pow, 0.0);
  pair<active_color, double> v2 = pair(active_color::pow, 0.0);

  a_t() = default;
  ~a_t() = default;
  a_t(const a_t &) = default;
  a_t &operator =(const a_t &) = default;
  a_t(a_t &&) = default;
  a_t &operator =(a_t &&) = default;

  template <class F>
  a_t(a_cond c, F f,
      pair<active_color, double> a,
      pair<active_color, double> b) : first(c), second(f), v1(a), v2(b) {}
  template <class F>
  a_t(a_cond c, F f,
      pair<active_color, double> a) : first(c), second(f), v1(a) {}
  template <class F>
  a_t(a_cond c, F f) : first(c), second(f) {}
};

enum star_level {
  no_star_level, ss_p, sss_m, sss, sss_p
};
struct fo_appeal_score_coeff {
  double operator ()(star_level lv) const {
    switch (lv) {
    case ss_p:
      return 1.1;
    case sss_m:
      return 1.12;
    case sss:
      return 1.15;
    case sss_p:
      return 1.2;
    default:
      print_then_terminate("wrong star level\n");
      return 0.0;
    }
  }
};
inline constexpr fo_appeal_score_coeff appeal_score_coeff{};

enum class sub_strategy {
  none,
  ace, // as ace
  aux, // as aux
  dfd_ace, // defend ace (put to aux position)
  dfd_aux // defend ace (put to ace position)
};
enum class boost_strategy {
  none,
  always,
  always_no_overlap,
  only_once_no_full_ace,
  only_once_no_full_aux,
  no_full_ace,
  no_full_ace_at_most_5th,
  no_full_aux,
  no_full_aux_at_most_5th,
  small_skip_no_full_ace,
  small_skip_no_full_aux,
  small_skip_no_full_ace_at_most_5th,
  small_skip_no_full_aux_at_most_5th
};
enum class race_category {
  normal,
  arena,
  arena_no_sub
};

struct result_t {
  vector<pair<uint64_t, bool>> appeal_scores;
  tuple<uint64_t, uint64_t, uint64_t> ace_aux_sub_appeal_scores;
  pair<uint64_t, uint64_t> ace_atk;
  pair<uint64_t, uint64_t> aux_atk;
  pair<uint64_t, uint64_t> sub_atk;
  pair<uint64_t, uint64_t> ace_dfd;
  pair<uint64_t, uint64_t> aux_dfd;
  pair<uint64_t, uint64_t> sub_dfd;
  tuple<double, double, double> ace_aux_sub_final_stm_percentages;
  uint64_t boost_round_amount = 0;

  void merge(const result_t &x) {
    appeal_scores.append_range(x.appeal_scores);

    at<0>(ace_aux_sub_appeal_scores)
      = add_with_check(at<0>(ace_aux_sub_appeal_scores),
                       at<0>(x.ace_aux_sub_appeal_scores)).value();
    at<1>(ace_aux_sub_appeal_scores)
      = add_with_check(at<1>(ace_aux_sub_appeal_scores),
                       at<1>(x.ace_aux_sub_appeal_scores)).value();
    at<2>(ace_aux_sub_appeal_scores)
      = add_with_check(at<2>(ace_aux_sub_appeal_scores),
                       at<2>(x.ace_aux_sub_appeal_scores)).value();

    ace_atk.first = add_with_check(ace_atk.first, x.ace_atk.first).value();
    ace_atk.second = add_with_check(ace_atk.second, x.ace_atk.second).value();
    aux_atk.first = add_with_check(aux_atk.first, x.aux_atk.first).value();
    aux_atk.second = add_with_check(aux_atk.second, x.aux_atk.second).value();
    sub_atk.first = add_with_check(sub_atk.first, x.sub_atk.first).value();
    sub_atk.second = add_with_check(sub_atk.second, x.sub_atk.second).value();

    ace_dfd.first = add_with_check(ace_dfd.first, x.ace_dfd.first).value();
    ace_dfd.second = add_with_check(ace_dfd.second, x.ace_dfd.second).value();
    aux_dfd.first = add_with_check(aux_dfd.first, x.aux_dfd.first).value();
    aux_dfd.second = add_with_check(aux_dfd.second, x.aux_dfd.second).value();
    sub_dfd.first = add_with_check(sub_dfd.first, x.sub_dfd.first).value();
    sub_dfd.second = add_with_check(sub_dfd.second, x.sub_dfd.second).value();

    at<0>(ace_aux_sub_final_stm_percentages)
      += at<0>(x.ace_aux_sub_final_stm_percentages);
    at<1>(ace_aux_sub_final_stm_percentages)
      += at<1>(x.ace_aux_sub_final_stm_percentages);
    at<2>(ace_aux_sub_final_stm_percentages)
      += at<2>(x.ace_aux_sub_final_stm_percentages);

    boost_round_amount += x.boost_round_amount;
  }
  void print_adjusted_avg(double photo_bonus) {
    if (appeal_scores.empty()) {
      putln("empty result\n");
      return;
    }

    const double winning_rate
      = (double)size(filter_rng(appeal_scores,
                                [](auto &x) {return x.second;}))
      / (double)appeal_scores.size();

    uint64_t acc{};
    const double k
      = (1.0 + 1.05 + 0.30 + photo_bonus)
      / (1.0 + 1.05 + photo_bonus);
    for (auto &x : appeal_scores) {
      acc = add_with_check(acc,
                           static_cast<uint64_t>
                           ((double)x.first * (x.second ? k : 1.0)))
        .value();
    }
    put("adjusted avg: ");
    putln(acc / appeal_scores.size(),
          setw(7), left);
  }
  void print() {
    if (appeal_scores.empty()) {
      putln("empty result\n");
      return;
    }

    re::sort(appeal_scores, [](auto &x, auto &y) {return x.first < y.first;});

    // appeal_scores
    {
      put("average:      ");
      const uint64_t acc = [&]() {
        uint64_t ret = 0;
        for (auto x : appeal_scores)
          ret = add_with_check(ret, x.first).value();
        return ret;
      }();
      putln(acc / appeal_scores.size(),
            setw(7), left);

      put("avg of 0.1%:  ");
      putln(reduce(bind_rng(rng(appeal_scores.rbegin(),
                                appeal_scores.size() / 1000u),
                            at<0>))
            / (appeal_scores.size() / 1000u),
            setw(7), left);

      put("avg of 1%:    ");
      putln(reduce(bind_rng(rng(appeal_scores.rbegin(),
                                appeal_scores.size() / 100u),
                            at<0>))
            / (appeal_scores.size() / 100u),
            setw(7), left);

      put("avg of 10%:   ");
      putln(reduce(bind_rng(rng(appeal_scores.rbegin(),
                                appeal_scores.size() / 10u),
                            at<0>))
            / (appeal_scores.size() / 10u),
            setw(7), left);

      put("avg of 40%:   ");
      putln(reduce(bind_rng(rng(appeal_scores.rbegin(),
                                (size_t)(appeal_scores.size() * 0.4)),
                            at<0>))
            / ((size_t)(appeal_scores.size() * 0.4)),
            setw(7), left);

      put("rank of 80%:  ", setw(6), left);
      putln(appeal_scores[appeal_scores.size() / 5u].first,
            setw(7), left);

      put("winning rate: ");
      put_rate((double)size(filter_rng(appeal_scores,
                                       [](auto &x) {return x.second;}))
               / (double)appeal_scores.size());
      putln();
    }
    // ace_aux_sub_appeal_scores
    {
      auto &x = ace_aux_sub_appeal_scores;
      const uint64_t acc = at<0>(x) + at<1>(x) + at<2>(x);
      put("appeal score from mates:  ");
      put_rate((double)at<0>(x) / (double)acc);
      put(" / ");
      put_rate((double)at<1>(x) / (double)acc);
      put(" / ");
      put_rate((double)at<2>(x) / (double)acc);
      putln();
    }
    // ace/aux/sub atk
    {
      const uint64_t acc = add_with_check(ace_atk.second,
                                          aux_atk.second,
                                          sub_atk.second).value();

      put("average attacking amount: ");
      putln((double)acc / (double)appeal_scores.size(), setprecision(3));

      put("attacking amount:         ");
      put_rate((double)ace_atk.second / (double)acc);
      put(" / ");
      put_rate((double)aux_atk.second / (double)acc);
      put(" / ");
      put_rate((double)sub_atk.second / (double)acc);
      putln();

      put("goal rate:                ");
      put_rate((double)ace_atk.first / (double)ace_atk.second);
      put(" / ");
      put_rate((double)aux_atk.first / (double)aux_atk.second);
      put(" / ");
      put_rate((double)sub_atk.first / (double)sub_atk.second);
      putln();
    }
    // ace/aux/sub dfd
    {
      const uint64_t acc = add_with_check(ace_dfd.second,
                                          aux_dfd.second,
                                          sub_dfd.second).value();

      put("average defending amount: ");
      putln((double)acc / (double)appeal_scores.size(), setprecision(3));

      put("defending amount:         ");
      put_rate((double)ace_dfd.second / (double)acc);
      put(" / ");
      put_rate((double)aux_dfd.second / (double)acc);
      put(" / ");
      put_rate((double)sub_dfd.second / (double)acc);
      putln();

      put("defending success rate:   ");
      put_rate((double)ace_dfd.first / (double)ace_dfd.second);
      put(" / ");
      put_rate((double)aux_dfd.first / (double)aux_dfd.second);
      put(" / ");
      put_rate((double)sub_dfd.first / (double)sub_dfd.second);
      putln();
    }
    // final stm
    {
      put("average final stamina:    ");
      put(at<0>(ace_aux_sub_final_stm_percentages) * 100.0
          / (double)appeal_scores.size(), setprecision(3));
      put("% / ");
      put(at<1>(ace_aux_sub_final_stm_percentages) * 100.0
          / (double)appeal_scores.size(), setprecision(3));
      put("% / ");
      put(at<2>(ace_aux_sub_final_stm_percentages) * 100.0
          / (double)appeal_scores.size(), setprecision(3));
      putln("%");
    }
    // boost round amount
    {
      put("average boost rounds:     ");
      putln(boost_round_amount / (double)appeal_scores.size(),
            setprecision(3));
    }
  }
};

struct girl {
  double orgn_pow;
  double pow_bonus;
  double orgn_tec;
  double tec_bonus;
  double tec_dfd_bonus;
  double orgn_stm;
  double stm_bonus;
  int orgn_apl;
  double apl_bonus;
  double apl_coeff;
  double pow_fever_bonus;
  double tec_fever_bonus;
  double partner_fever_bonus;
  double spike_prob;
  double atk_prob_bonus;
  double dfd_prob_bonus;
  double atk_ace_bonus;
  local_vector<a_t, 11> skls;

  double stm;
  double tmp_pow_bonus;
  double tmp_tec_bonus;
  double tmp_stm_bonus;
  double tmp_apl_bonus;

  void clear_active() {
    tmp_pow_bonus = 0.0;
    tmp_tec_bonus = 0.0;
    tmp_stm_bonus = 0.0;
    tmp_apl_bonus = 0.0;
  }
  void reset() {
    clear_active();
    stm = orgn_stm;
  }

  girl() : girl(0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0,
                0, 0.0, 0.0,
                0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0) {}
  ~girl() = default;
  girl(const girl &) = default;
  girl &operator =(const girl &) = default;
  girl(girl &&) = default;
  girl &operator =(girl &&) = default;

  template <class...S>
  girl(double power, double power_bonus,
       double technique, double technique_bonus, double tec_defending_bonus,
       double stamina, double stamina_saving,
       int apl, double appeal_bonus, double appeal_coeff,
       double pow_f_bonus, double tec_f_bonus, double partner_f_bonus,
       double spike_probability,
       double attacking_probability_bonus,
       double defending_probability_bonus,
       double attaking_ace_bonus,
       S...s)
    : orgn_pow(power)
    , pow_bonus(power_bonus)
    , orgn_tec(technique)
    , tec_bonus(technique_bonus)
    , tec_dfd_bonus(tec_defending_bonus)
    , orgn_stm(stamina)
    , stm_bonus(stamina_saving)
    , orgn_apl(apl)
    , apl_bonus(appeal_bonus)
    , apl_coeff(appeal_coeff)
    , pow_fever_bonus(pow_f_bonus)
    , tec_fever_bonus(tec_f_bonus)
    , partner_fever_bonus(partner_f_bonus)
    , spike_prob(spike_probability)
    , atk_prob_bonus(attacking_probability_bonus)
    , dfd_prob_bonus(defending_probability_bonus)
    , atk_ace_bonus(attaking_ace_bonus)
    , skls{s...}
    , stm(orgn_stm)
    , tmp_pow_bonus(0.0)
    , tmp_tec_bonus(0.0)
    , tmp_stm_bonus(0.0)
    , tmp_apl_bonus(0.0) {}

  girl &pow_season() {
    orgn_pow *= 1.25;
    return *this;
  }
  girl &tec_season() {
    orgn_tec *= 1.25;
    return *this;
  }
  girl &stm_season() {
    orgn_pow *= 1.38;
    orgn_tec *= 1.38;
    return *this;
  }
  girl &apl_season() {
    apl_coeff = 2.0;
    return *this;
  }
};

struct fo_make_simple_girl {
  girl operator ()(double pow = 5000,
                   double tec = 5000,
                   double stm = 5000,
                   int apl = 150,
                   double spike_prob = 0.5) const {
    return girl(pow, 0.0,
                tec, 0.0, 0.0,
                stm, 0.0,
                apl, 0.0, 1.2,
                0.0, 0.0, 0.0,
                0.5, 0.0, 0.0, 0.0);
  }
};
inline constexpr fo_make_simple_girl make_simple_girl{};

struct team {
  girl ace;
  girl aux;
  double prob_ace_atk;
  double prob_atk_ace;

  girl sub;
  girl *sub_pos; // ace, aux, or nullptr
  int sub_round_left;

  double groove;
  double orgn_groove;
  int boost_left;
  star_level lv;
  double groove_bonus;

  int score;

  void clear_active() {
    ace.clear_active();
    aux.clear_active();
    sub.clear_active();
  }
  void reset() {
    ace.reset();
    aux.reset();
    sub.reset();

    sub_pos = nullptr;
    sub_round_left = 2;

    groove = orgn_groove;
    boost_left = 0;
    score = 0;
  }

  team() = delete;
  ~team() = default;
  team(const team &) = default;
  team &operator =(const team &) = default;
  team(team &&) = default;
  team &operator =(team &&) = default;

  team(const girl &x,
       const girl &y,
       const girl &z = girl{},
       double original_groove = 0.0,
       double prob_ace_attack = 0.50,
       double prob_to_attack_ace = 0.50,
       star_level star_lv = ss_p)
    : ace(x)
    , aux(y)
    , prob_ace_atk(prob_ace_attack)
    , prob_atk_ace(prob_to_attack_ace)
    , sub(z)
    , sub_pos{}
    , sub_round_left(2)
    , groove(original_groove)
    , orgn_groove(original_groove)
    , boost_left(0)
    , lv(star_lv)
    , groove_bonus()
    , score(0) {
    switch (lv) {
    case no_star_level:
      groove_bonus = 0.0;
      break;
    case ss_p:
      groove_bonus = 5.0;
      break;
    case sss_m:
      groove_bonus = 6.0;
      break;
    case sss:
      groove_bonus = 7.5;
      break;
    case sss_p:
      groove_bonus = 10.0;
      break;
    default:
      print_then_terminate("wrong star level\n");
      break;
    }
  }

  team &spike() {
    ace.spike_prob = clamp(ace.spike_prob + 0.20, 0.00, 1.00);
    aux.spike_prob = clamp(aux.spike_prob + 0.20, 0.00, 1.00);
    sub.spike_prob = clamp(sub.spike_prob + 0.20, 0.00, 1.00);
    return *this;
  }
  team &feint() {
    ace.spike_prob = clamp(ace.spike_prob - 0.20, 0.00, 1.00);
    aux.spike_prob = clamp(aux.spike_prob - 0.20, 0.00, 1.00);
    sub.spike_prob = clamp(sub.spike_prob - 0.20, 0.00, 1.00);
    return *this;
  }

  team &ace_atk() {
    prob_ace_atk = 0.63;
    return *this;
  }
  team &aux_atk() {
    prob_ace_atk = 0.37;
    return *this;
  }
  team &rnd_atk() {
    prob_ace_atk = 0.50;
    return *this;
  }

  team &atk_ace() {
    prob_atk_ace = 0.65;
    return *this;
  }
  team &atk_aux() {
    prob_atk_ace = 0.35;
    return *this;
  }
  team &atk_rnd() {
    prob_atk_ace = 0.50;
    return *this;
  }

  team &apply_apl_season() {
    const double x = (double)((int)(ace.apl_coeff == 2.0)
                              + (int)(aux.apl_coeff == 2.0)
                              + (int)(sub.apl_coeff == 2.0));
    groove = orgn_groove = clamp(x * 50.0, 0.0, 100.0);
    return *this;
  }
};

enum active_category {
  atk_pow_plus,
  atk_pow_minus,
  atk_tec_plus,
  atk_tec_minus,

  dfd_pow_plus,
  dfd_pow_minus,
  dfd_tec_plus,
  dfd_tec_minus,

  atk_stm_minus,
  atk_stm_plus,

  all_stand_up,

  groove_minus,

  atk_pow_stm_plus,
  atk_tec_stm_plus
};
struct active {
  active_category ctg;
  double prob;
  double v1;
  double v2 = 0.0;
};
struct fo_make_a {
  a_t operator ()(active_category ctg,
                  double prob,
                  double v1,
                  double v2 = 0.0) const {
    if (!(v1 >= 0.0 && v1 <= 1.0))
      print_then_terminate("make_a(): value 1 is out of [0.0, 1.0]\n");
    if (!(v2 >= 0.0 && v2 <= 1.0))
      print_then_terminate("make_a(): value 2 is out of [0.0, 1.0]\n");
    switch (ctg) {
    case atk_pow_plus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob))
                     x.tmp_pow_bonus += v1;
                 },
                 pair(active_color::pow, prob * v1));
    case atk_pow_minus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &, team &, girl &y, bool) {
                   if (rnd(prob))
                     y.tmp_pow_bonus -= v1;
                 },
                 pair(active_color::pow, prob * v1));
    case atk_tec_plus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob))
                     x.tmp_tec_bonus += v1;
                 },
                 pair(active_color::tec, prob * v1));
    case atk_tec_minus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &, team &, girl &y, bool) {
                   if (rnd(prob))
                     y.tmp_tec_bonus -= v1;
                 },
                 pair(active_color::tec, prob * v1));

    case dfd_pow_plus:
      return a_t(a_cond::dfd_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob))
                     x.tmp_pow_bonus += v1;
                 },
                 pair(active_color::pow, prob * v1));
    case dfd_pow_minus:
      return a_t(a_cond::dfd_before,
                 [=](team &, girl &, team &, girl &y, bool) {
                   if (rnd(prob))
                     y.tmp_pow_bonus -= v1;
                 },
                 pair(active_color::pow, prob * v1));
    case dfd_tec_plus:
      return a_t(a_cond::dfd_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob))
                     x.tmp_tec_bonus += v1;
                 },
                 pair(active_color::tec, prob * v1));
    case dfd_tec_minus:
      return a_t(a_cond::dfd_before,
                 [=](team &, girl &, team &, girl &y, bool) {
                   if (rnd(prob))
                     y.tmp_tec_bonus -= v1;
                 },
                 pair(active_color::tec, prob * v1));

    case atk_stm_minus:
      return a_t(a_cond::atk_after,
                 [=](team &, girl &x, team &, girl &, bool suc) {
                   if (suc && rnd(prob))
                     x.tmp_stm_bonus += v1;
                 },
                 pair(active_color::stm, prob * v1));
    case atk_stm_plus:
      return a_t(a_cond::atk_after,
                 [=](team &, girl &, team &, girl &y, bool suc) {
                   if (suc && rnd(prob))
                     y.tmp_stm_bonus -= v1;
                 },
                 pair(active_color::stm, prob * v1));

    case all_stand_up:
      return a_t(a_cond::atk_after,
                 [=](team &, girl &x, team &, girl &, bool suc) {
                   if (suc && rnd(prob))
                     x.tmp_apl_bonus += v1;
                 },
                 pair(active_color::apl, prob * v1));

    case groove_minus:
      return a_t(a_cond::atk_dfd_after,
                 [=](team &, girl &, team &t2, girl &, bool) {
                   if (rnd(prob))
                     t2.groove -= v1 * 100.0;
                 });

    case atk_pow_stm_plus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob)) {
                     x.tmp_pow_bonus += v1;
                     x.tmp_stm_bonus += v2;
                   }
                 },
                 pair(active_color::pow, prob * v1),
                 pair(active_color::stm, prob * v2));
    case atk_tec_stm_plus:
      return a_t(a_cond::atk_before,
                 [=](team &, girl &x, team &, girl &, bool) {
                   if (rnd(prob)) {
                     x.tmp_tec_bonus += v1;
                     x.tmp_stm_bonus += v2;
                   }
                 },
                 pair(active_color::tec, prob * v1),
                 pair(active_color::stm, prob * v2));
    default:
      print_then_terminate("make_a(): wrong category\n");
      return a_t{};
    }
  }
  a_t operator ()(active x) const {
    return operator ()(x.ctg, x.prob, x.v1, x.v2);
  }
};
inline constexpr fo_make_a make_a{};

extern const active pow_j0(atk_pow_plus, 0.50, 0.30);
extern const active pow_j(atk_pow_plus, 0.70, 0.50);
extern const active pow_i0(atk_pow_plus, 0.40, 0.35);
extern const active pow_i(atk_pow_plus, 0.60, 0.55);
extern const active pow_wg0(atk_pow_stm_plus, 0.24, 0.24, 0.24);
extern const active pow_wg(atk_pow_stm_plus, 0.40, 0.40, 0.40);
extern const active pow_h0(atk_pow_plus, 0.35, 0.40);
extern const active pow_h(atk_pow_plus, 0.55, 0.60);
extern const active pow_g0(atk_pow_plus, 0.35, 0.30);
extern const active pow_g(atk_pow_plus, 0.55, 0.50);
extern const active pow_a0(atk_pow_plus, 0.15, 0.40);
extern const active pow_a(atk_pow_plus, 0.35, 0.60);
extern const active pow_b0(atk_pow_plus, 0.20, 0.35);
extern const active pow_b(atk_pow_plus, 0.40, 0.55);
extern const active pow_c0(atk_pow_plus, 0.25, 0.30);
extern const active pow_c(atk_pow_plus, 0.45, 0.50);
extern const active pow_d0(atk_pow_plus, 0.30, 0.25);
extern const active pow_d(atk_pow_plus, 0.50, 0.45);
extern const active pow_e0(atk_pow_plus, 0.35, 0.20);
extern const active pow_e(atk_pow_plus, 0.55, 0.45);
extern const active pow_f0(atk_pow_plus, 0.40, 0.15);
extern const active pow_f(atk_pow_plus, 0.60, 0.35);

extern const active tec_wg0(atk_tec_stm_plus, 0.24, 0.24, 0.24);
extern const active tec_wg(atk_tec_stm_plus, 0.40, 0.40, 0.40);
extern const active tec_j0(atk_tec_plus, 0.50, 0.30);
extern const active tec_j(atk_tec_plus, 0.70, 0.50);
extern const active tec_i0(atk_tec_plus, 0.40, 0.35);
extern const active tec_i(atk_tec_plus, 0.60, 0.55);
extern const active tec_h0(atk_tec_plus, 0.35, 0.40);
extern const active tec_h(atk_tec_plus, 0.55, 0.60);
extern const active tec_g0(atk_tec_plus, 0.35, 0.30);
extern const active tec_g(atk_tec_plus, 0.55, 0.50);
extern const active tec_a0(atk_tec_plus, 0.15, 0.40);
extern const active tec_a(atk_tec_plus, 0.35, 0.60);
extern const active tec_b0(atk_tec_plus, 0.20, 0.35);
extern const active tec_b(atk_tec_plus, 0.40, 0.55);
extern const active tec_c0(atk_tec_plus, 0.25, 0.30);
extern const active tec_c(atk_tec_plus, 0.45, 0.50);
extern const active tec_d0(atk_tec_plus, 0.30, 0.25);
extern const active tec_d(atk_tec_plus, 0.50, 0.45);
extern const active tec_e0(atk_tec_plus, 0.35, 0.20);
extern const active tec_e(atk_tec_plus, 0.55, 0.45);
extern const active tec_f0(atk_tec_plus, 0.40, 0.15);
extern const active tec_f(atk_tec_plus, 0.60, 0.35);

extern const active stm_a0(atk_stm_minus, 0.15, 0.40);
extern const active stm_a(atk_stm_minus, 0.35, 0.60);
extern const active stm_b0(atk_stm_minus, 0.20, 0.35);
extern const active stm_b(atk_stm_minus, 0.40, 0.55);
extern const active stm_c0(atk_stm_minus, 0.25, 0.30);
extern const active stm_c(atk_stm_minus, 0.45, 0.50);
extern const active stm_d0(atk_stm_minus, 0.30, 0.25);
extern const active stm_d(atk_stm_minus, 0.50, 0.45);
extern const active stm_e0(atk_stm_minus, 0.35, 0.20);
extern const active stm_e(atk_stm_minus, 0.55, 0.40);
extern const active stm_ee0(atk_stm_minus, 0.35, 0.25);
extern const active stm_ee(atk_stm_minus, 0.55, 0.45);
extern const active stm_f0(atk_stm_minus, 0.40, 0.15);
extern const active stm_f(atk_stm_minus, 0.60, 0.35);

extern const active dance_b0(atk_pow_minus, 0.20, 0.35);
extern const active dance_c0(atk_pow_minus, 0.25, 0.30);
extern const active dance_d0(atk_pow_minus, 0.30, 0.25);
extern const active dance_e0(atk_pow_minus, 0.35, 0.20);
extern const active dance_f0(atk_pow_minus, 0.40, 0.15);
extern const active dance_b(atk_pow_minus, 0.40, 0.55);
extern const active dance_c(atk_pow_minus, 0.45, 0.50);
extern const active dance_d(atk_pow_minus, 0.50, 0.45);
extern const active dance_e(atk_pow_minus, 0.55, 0.40);
extern const active dance_f(atk_pow_minus, 0.60, 0.35);

extern const active pressure_b0(dfd_pow_minus, 0.20, 0.35);
extern const active pressure_c0(dfd_pow_minus, 0.25, 0.30);
extern const active pressure_d0(dfd_pow_minus, 0.30, 0.25);
extern const active pressure_e0(dfd_pow_minus, 0.35, 0.20);
extern const active pressure_f0(dfd_pow_minus, 0.40, 0.15);
extern const active pressure_b(dfd_pow_minus, 0.40, 0.55);
extern const active pressure_c(dfd_pow_minus, 0.45, 0.50);
extern const active pressure_d(dfd_pow_minus, 0.50, 0.45);
extern const active pressure_e(dfd_pow_minus, 0.55, 0.40);
extern const active pressure_f(dfd_pow_minus, 0.60, 0.35);

extern const active firm_b0(dfd_pow_plus, 0.20, 0.35);
extern const active firm_c0(dfd_pow_plus, 0.25, 0.30);
extern const active firm_d0(dfd_pow_plus, 0.30, 0.25);
extern const active firm_e0(dfd_pow_plus, 0.35, 0.20);
extern const active firm_f0(dfd_pow_plus, 0.40, 0.15);
extern const active firm_b(dfd_pow_plus, 0.40, 0.55);
extern const active firm_c(dfd_pow_plus, 0.45, 0.50);
extern const active firm_d(dfd_pow_plus, 0.50, 0.45);
extern const active firm_e(dfd_pow_plus, 0.55, 0.45);
extern const active firm_f(dfd_pow_plus, 0.60, 0.35);

extern const active genius_b0(dfd_tec_minus, 0.20, 0.35);
extern const active genius_c0(dfd_tec_minus, 0.25, 0.30);
extern const active genius_d0(dfd_tec_minus, 0.30, 0.25);
extern const active genius_e0(dfd_tec_minus, 0.35, 0.20);
extern const active genius_f0(dfd_tec_minus, 0.40, 0.15);
extern const active genius_b(dfd_tec_minus, 0.40, 0.55);
extern const active genius_c(dfd_tec_minus, 0.45, 0.50);
extern const active genius_d(dfd_tec_minus, 0.50, 0.45);
extern const active genius_e(dfd_tec_minus, 0.55, 0.40);
extern const active genius_f(dfd_tec_minus, 0.60, 0.35);

extern const active iron_b0(dfd_tec_plus, 0.20, 0.35);
extern const active iron_c0(dfd_tec_plus, 0.25, 0.30);
extern const active iron_d0(dfd_tec_plus, 0.30, 0.25);
extern const active iron_e0(dfd_tec_plus, 0.35, 0.20);
extern const active iron_f0(dfd_tec_plus, 0.40, 0.15);
extern const active iron_b(dfd_tec_plus, 0.40, 0.55);
extern const active iron_c(dfd_tec_plus, 0.45, 0.50);
extern const active iron_d(dfd_tec_plus, 0.50, 0.45);
extern const active iron_e(dfd_tec_plus, 0.55, 0.45);
extern const active iron_f(dfd_tec_plus, 0.60, 0.35);

struct race {
  double appeal_score;
  team l;
  team r;
  int max_score;
  sub_strategy sub_sttg;
  boost_strategy boost_sttg;
  race_category race_ctg;

  void clear_active() {
    l.clear_active();
    r.clear_active();
  }
  void reset() {
    l.reset();
    r.reset();
    appeal_score = 0.0;
  }

  race() = delete;
  ~race() = default;
  race(const race &) = default;
  race &operator =(const race &) = default;
  race(race &&) = default;
  race &operator =(race &&) = default;

  race(const team &a, const team &b, int score = 8,
       sub_strategy sub_s = sub_strategy::none,
       boost_strategy boost_s = boost_strategy::none,
       race_category race_c = race_category::normal)
    : appeal_score(0.0), l(a), r(b), max_score(score)
    , sub_sttg(sub_s), boost_sttg(boost_s), race_ctg(race_c) {}

  result_t sim(uint64_t n = 1) {
    result_t ret;
    ret.appeal_scores.reserve(n);

    const bool is_arena = (race_ctg != race_category::normal);
    const bool is_no_sub_arena = (race_ctg == race_category::arena_no_sub);

    const double star_coeff = appeal_score_coeff(l.lv);

    for (auto nn : irng(0u, n)) {
      // for every race win
      bool l_start = rnd(0.5); // select start team
      bool arena_first_boost_done = false;
      bool first_boost_done = false; // only for one boost strategy
      int boost_amount = 0;
      for (;;) {
        // for every attacking success
        double rand_bonus = 0.10;
        for (;;) {
          team &t = l_start ? l : r;
          team &t2 = l_start ? r : l;
          girl &t_ace = (t.sub_pos == addressof(t.ace) ? t.sub : t.ace);
          girl &t_aux = (t.sub_pos == addressof(t.aux) ? t.sub : t.aux);
          girl &t2_ace = (t2.sub_pos == addressof(t2.ace) ? t2.sub : t2.ace);
          girl &t2_aux = (t2.sub_pos == addressof(t2.aux) ? t2.sub : t2.aux);

          // boost
          if (l.groove == 100.0) {
            auto [ace_girl, aux_girl]
              = ((addressof(t) == addressof(l))
                 ? tie(t_ace, t_aux) : tie(t2_ace, t2_aux));
            bool boost_y = false;
            switch (boost_sttg) {
            case boost_strategy::always:
              boost_y = true;              
              break;
            case boost_strategy::always_no_overlap:
              if (l.boost_left == 0)
                boost_y = true;
              break;
            case boost_strategy::only_once_no_full_ace:
              if (!first_boost_done && ace_girl.stm < ace_girl.orgn_stm) {
                boost_y = true;
                first_boost_done = true;
              }
              break;
            case boost_strategy::only_once_no_full_aux:
              if (!first_boost_done && aux_girl.stm < aux_girl.orgn_stm) {
                boost_y = true;
                first_boost_done = true;
              }
              break;
            case boost_strategy::no_full_ace:
              if (ace_girl.stm < ace_girl.orgn_stm)
                boost_y = true;
              break;
            case boost_strategy::no_full_ace_at_most_5th:
              if (ace_girl.stm < ace_girl.orgn_stm && l.score <= 4)
                boost_y = true;
              break;
            case boost_strategy::no_full_aux:
              if (aux_girl.stm < aux_girl.orgn_stm)
                boost_y = true;
              break;
            case boost_strategy::no_full_aux_at_most_5th:
              if (aux_girl.stm < aux_girl.orgn_stm && l.score <= 4)
                boost_y = true;
              break;
            case boost_strategy::small_skip_no_full_ace:
              if (rand_bonus < 0.11
                  && ace_girl.stm < ace_girl.orgn_stm)
                boost_y = true;
              break;
            case boost_strategy::small_skip_no_full_aux:
              if (rand_bonus < 0.11
                  && aux_girl.stm < aux_girl.orgn_stm)
                boost_y = true;
              break;
            case boost_strategy::small_skip_no_full_ace_at_most_5th:
              if (rand_bonus < 0.11
                  && ace_girl.stm < ace_girl.orgn_stm
                  && l.score <= 4)
                boost_y = true;
              break;
            case boost_strategy::small_skip_no_full_aux_at_most_5th:
              if (rand_bonus < 0.11
                  && aux_girl.stm < aux_girl.orgn_stm
                  && l.score <= 4)
                boost_y = true;
              break;
            default:
              break;
            }
            if (boost_y) {
              l.groove = 0.0;
              l.boost_left = 2;
              ace_girl.stm += ace_girl.orgn_stm * 0.1;
              ace_girl.stm = min(ace_girl.stm, ace_girl.orgn_stm);
              aux_girl.stm += aux_girl.orgn_stm * 0.1;
              aux_girl.stm = min(aux_girl.stm, aux_girl.orgn_stm);
            }
          }
          if (r.groove == 100.0 && is_arena) {
            auto [ace_girl, aux_girl]
              = ((addressof(t) == addressof(r))
                 ? tie(t_ace, t_aux) : tie(t2_ace, t2_aux));
            r.groove = 0.0;
            r.boost_left = 2;
            ace_girl.stm += ace_girl.orgn_stm * 0.1;
            ace_girl.stm = min(ace_girl.stm, ace_girl.orgn_stm);
            aux_girl.stm += aux_girl.orgn_stm * 0.1;
            aux_girl.stm = min(aux_girl.stm, aux_girl.orgn_stm);
            arena_first_boost_done = true;
          }

          // select attaking girl
          const bool ace_attack = rnd(clamp(t.prob_ace_atk
                                            + (t_ace.atk_prob_bonus
                                               - t_aux.atk_prob_bonus),
                                            0.0, 1.0));
          girl &g = ace_attack ? t_ace : t_aux;

          // select defending girl
          const bool ace_defend = rnd(clamp(t.prob_atk_ace
                                            + g.atk_ace_bonus
                                            + (t2_ace.dfd_prob_bonus
                                               - t2_aux.dfd_prob_bonus),
                                            0.0, 1.0));
          girl &g2 = ace_defend ? t2_ace : t2_aux;

          // apply skills before attack
          for (auto &x : g.skls)
            switch (x.first) {
            case atk_before:
            case atk_dfd_before:
              x.second(t, g, t2, g2, false);
              break;
            default:
              break;
            }
          for (auto &x : g2.skls)
            switch (x.first) {
            case dfd_before:
            case atk_dfd_before:
              x.second(t2, g2, t, g, false);
              break;
            default:
              break;
            }

          // get attack result
          const bool using_spike = rnd(g.spike_prob);
          bool atk_success = false;
          double atk_value{};
          double dfd_value{};
          if (using_spike) {
            atk_value
              = g.orgn_pow
              * (0.4 + 0.6 * pow(g.stm / g.orgn_stm, 0.6))
              * (1.0 + g.pow_bonus + g.tmp_pow_bonus)
              * ((t.boost_left != 0) ? g.apl_coeff : 1.0)
              * (1.0 + rand_bonus * (get_rander()(0, 10000) / 10000.0));
            dfd_value
              = g2.orgn_pow
              * (0.4 + 0.6 * pow(g2.stm / g2.orgn_stm, 0.6))
              * (1.0 + g2.pow_bonus + g2.tmp_pow_bonus)
              * ((t2.boost_left != 0) ? g2.apl_coeff : 1.0);
          }
          else {
            atk_value
              = g.orgn_tec
              * (0.4 + 0.6 * pow(g.stm / g.orgn_stm, 0.6))
              * (1.0 + g.tec_bonus + g.tmp_tec_bonus)
              * ((t.boost_left != 0) ? g.apl_coeff : 1.0)
              * (1.0 + rand_bonus * (get_rander()(0, 10000) / 10000.0));
            dfd_value
              = g2.orgn_tec
              * (0.4 + 0.6 * pow(g2.stm / g2.orgn_stm, 0.6))
              * (1.0 + g2.tec_bonus + g2.tec_dfd_bonus + g2.tmp_tec_bonus)
              * ((t2.boost_left != 0) ? g2.apl_coeff : 1.0);
          }
          atk_success = (max(atk_value, 0.0) > max(dfd_value, 0.0));

          // apply skills after getting attack result
          for (auto &x : g.skls)
            switch (x.first) {
            case atk_after:
              x.second(t, g, t2, g2, atk_success);
              break;
            case atk_dfd_after:
              x.second(t, g, t2, g2, false);
              break;
            default:
              break;
            }
          for (auto &x : g2.skls)
            switch (x.first) {
            case dfd_after:
              x.second(t2, g2, t, g, !atk_success);
              break;
            case atk_dfd_after:
              x.second(t2, g2, t, g, false);
              break;
            default:
              break;
            }

          // record attacking amount for every teammate
          const auto record_atk_amnt = [=](auto &x) {
            if (atk_success)
              ++x.first;
            ++x.second;
          };
          if (addressof(g) == addressof(l.ace))
            record_atk_amnt(ret.ace_atk);
          else if (addressof(g) == addressof(l.aux))
            record_atk_amnt(ret.aux_atk);
          else if (addressof(g) == addressof(l.sub))
            record_atk_amnt(ret.sub_atk);

          // record defending amount for every teammate
          const auto record_dfd_amnt = [=](auto &x) {
            if (!atk_success)
              ++x.first;
            ++x.second;
          };
          if (addressof(g2) == addressof(l.ace))
            record_dfd_amnt(ret.ace_dfd);
          else if (addressof(g2) == addressof(l.aux))
            record_dfd_amnt(ret.aux_dfd);
          else if (addressof(g2) == addressof(l.sub))
            record_dfd_amnt(ret.sub_dfd);

          if (atk_success) {
            // consume stm
            g.stm -= atk_value * 0.15 * (1 - g.stm_bonus - g.tmp_stm_bonus);
            g.stm = max(g.stm, 0.0);
            g2.stm -= dfd_value * 0.084;
            g2.stm = max(g2.stm, 0.0);

            // increase groove for team 1
            double groove_v = 0.0;
            if (addressof(g) == addressof(t_ace))
              groove_v += ((addressof(g2) == addressof(t2_ace)) ? 20.0 : 16.0);
            else
              groove_v += ((addressof(g2) == addressof(t2_ace)) ? 17.0 : 14.0);
            groove_v
              += 0.04 * g.orgn_apl * (1.0 + g.apl_bonus + g.tmp_apl_bonus);
            groove_v += t.groove_bonus;
            if (is_arena && arena_first_boost_done
                && addressof(t) == addressof(r))
              groove_v += groove_v;
            t.groove += ceil(groove_v);
            t.groove = clamp(t.groove, 0.0, 100.0);

            // increase groove for team 2
            groove_v = 20.0 + t2.groove_bonus;
            if (is_arena && arena_first_boost_done
                && addressof(t2) == addressof(r))
              groove_v += groove_v;
            t2.groove += ceil(groove_v);
            t2.groove = clamp(t2.groove, 0.0, 100.0);

            if (l.boost_left != 0)
              ++boost_amount;

            // try to cancel boost effects
            if (t.boost_left != 0)
              --t.boost_left;
            if (t2.boost_left != 0)
              --t2.boost_left;

            // score and appeal score
            ++t.score;
            if (addressof(t) == addressof(l)) {
              const girl &partner_g = (addressof(g) == addressof(t_ace)
                                       ? t_aux : t_ace);
              const double appeal_v
                = atk_value * (1.00 + t.groove / 100.0) * 0.1 * star_coeff
                * (1.00
                   + (using_spike ? g.pow_fever_bonus : g.tec_fever_bonus)
                   + partner_g.partner_fever_bonus)
                * ((l.score + r.score > max_score)
                   ? (1.0 - ((l.score + r.score - max_score)
                             / (double)(max_score + max_score)))
                   : 1.0);
              (addressof(g) == addressof(l.ace)
               ? at<0>(ret.ace_aux_sub_appeal_scores)
               : (addressof(g) == addressof(l.aux)
                  ? at<1>(ret.ace_aux_sub_appeal_scores)
                  : at<2>(ret.ace_aux_sub_appeal_scores)))
                += appeal_v;
              appeal_score += appeal_v;
            }

            // sub affairs for attacking team
            if (t.sub_pos != nullptr) {
              if (--t.sub_round_left == 0) {
                girl &re_entered_g = (t.sub_pos == addressof(t.ace)
                                      ? t.ace : t.aux);
                re_entered_g.stm = min(re_entered_g.stm
                                       + re_entered_g.orgn_stm * 0.05,
                                       re_entered_g.orgn_stm);
                t.sub_pos = nullptr;
              }
            }
            else {
              if (t.sub_round_left != 0) {
                if (addressof(t) == addressof(r)) {
                  if (is_arena && !is_no_sub_arena) {
                    const double t_ace_stm_ratio = t.ace.stm / t.ace.orgn_stm;
                    const double t_aux_stm_ratio = t.aux.stm / t.aux.orgn_stm;
                    if (t_ace_stm_ratio < t_aux_stm_ratio) {
                      if (t_ace_stm_ratio < 0.9)
                        t.sub_pos = addressof(t.ace);
                    }
                    else {
                      if (t_aux_stm_ratio < 0.9)
                        t.sub_pos = addressof(t.aux);
                    }
                  }
                }
                else {
                  switch (sub_sttg) {
                  case sub_strategy::ace:
                    t.sub_pos = addressof(t.ace);
                    break;
                  case sub_strategy::aux:
                    t.sub_pos = addressof(t.aux);
                    break;
                  default:
                    break;
                  }
                }
              }
            }

            // sub affairs for defending team
            if (t2.sub_pos != nullptr) {
              if (--t2.sub_round_left == 0) {
                girl &re_entered_g = (t2.sub_pos == addressof(t2.ace)
                                      ? t2.ace : t2.aux);
                re_entered_g.stm = min(re_entered_g.stm
                                       + re_entered_g.orgn_stm * 0.05,
                                       re_entered_g.orgn_stm);
                t2.sub_pos = nullptr;
              }
            }
            else {
              if (t2.sub_round_left != 0) {
                if (addressof(t2) == addressof(r)) {
                  if (is_arena && !is_no_sub_arena) {
                    const double t2_ace_stm_ratio
                      = t2.ace.stm / t2.ace.orgn_stm;
                    const double t2_aux_stm_ratio
                      = t2.aux.stm / t2.aux.orgn_stm;
                    if (t2_ace_stm_ratio < t2_aux_stm_ratio) {
                      if (t2_ace_stm_ratio < 0.9)
                        t2.sub_pos = addressof(t2.ace);
                    }
                    else {
                      if (t2_aux_stm_ratio < 0.9)
                        t2.sub_pos = addressof(t2.aux);
                    }
                  }
                }
                else {
                  switch (sub_sttg) {
                  case sub_strategy::dfd_ace:
                    t2.sub_pos = addressof(t2.aux);
                    break;
                  case sub_strategy::dfd_aux:
                    t2.sub_pos = addressof(t2.ace);
                    break;
                  default:
                    break;
                  }
                }
              }
            }

            // next start team
            l_start = (addressof(t) == addressof(l));

            // end one game
            if (l.score == max_score || r.score == max_score) {
              ret.appeal_scores.push_back
                (pair(static_cast<uint64_t>(round(appeal_score)),
                      (l.score == max_score)));
              {
                auto &acc = ret.ace_aux_sub_final_stm_percentages;
                at<0>(acc) += l.ace.stm / l.ace.orgn_stm;
                at<1>(acc) += l.aux.stm / l.aux.orgn_stm;
                at<2>(acc) += l.sub.stm / l.sub.orgn_stm;
              }
              ret.boost_round_amount += boost_amount;
              goto quit_label;
            }

            break;
          }
          else {
            // consume stm
            g.stm -= atk_value * 0.05;
            g.stm = max(g.stm, 0.0);
            g2.stm -= dfd_value * 0.10;
            g2.stm = max(g2.stm, 0.0);

            // increase groove for team 1
            double groove_v = 0.01 * g.orgn_apl + t.groove_bonus;
            if (is_arena && arena_first_boost_done
                && addressof(t) == addressof(r))
              groove_v += groove_v;
            t.groove += ceil(groove_v);
            t.groove = clamp(t.groove, 0.0, 100.0);

            // increase groove for team 2
            groove_v = 0.01 * g2.orgn_apl + t2.groove_bonus;
            if (is_arena && arena_first_boost_done
                && addressof(t2) == addressof(r))
              groove_v += groove_v;
            t2.groove += ceil(groove_v);
            t2.groove = clamp(t2.groove, 0.0, 100.0);

            // next start team
            l_start = (addressof(t2) == addressof(l));

            // increase randomized bonus by success defending
            if (rand_bonus <= 10.0)
              rand_bonus += 0.10;
          }

          clear_active();
        }
        clear_active();
      }
    quit_label:
      reset();
    }

    return ret;
  }
};

enum season_category {
  no_season, pow_season, tec_season, stm_season, apl_season
};

enum tactics_category {
  angel,
  fairy,
  platinum_atk,
  platinum_dfd,
  precise_atk
};
struct tactics {
  tactics_category ctg;
  size_t value;

  tactics() = delete;
  ~tactics();
  tactics(const tactics &);
  tactics &operator =(const tactics &);
  tactics(tactics &&) noexcept;
  tactics &operator =(tactics &&) noexcept;

  tactics(tactics_category);
  tactics(tactics_category, size_t);
};
tactics::~tactics() = default;
tactics::tactics(const tactics &) = default;
tactics &tactics::operator =(const tactics &) = default;
tactics::tactics(tactics &&) noexcept = default;
tactics &tactics::operator =(tactics &&) noexcept = default;
tactics::tactics(tactics_category c) : ctg(c) {
  switch (c) {
  case platinum_atk:
  case platinum_dfd:
    value = 15;
    break;
  default:
    value = 10;
    break;
  }
}
tactics::tactics(tactics_category c, size_t v) : ctg(c), value(v) {}

enum fever_category {
  killer_f,
  sweet_f,
  yell_f
};
struct fever {
  fever_category ctg;
  size_t value;

  fever() = delete;
  ~fever();
  fever(const fever &);
  fever &operator =(const fever &);
  fever(fever &&) noexcept;
  fever &operator =(fever &&) noexcept;

  fever(fever_category);
  fever(fever_category, size_t);
};
fever::~fever() = default;
fever::fever(const fever &) = default;
fever &fever::operator =(const fever &) = default;
fever::fever(fever &&) noexcept = default;
fever &fever::operator =(fever &&) noexcept = default;
fever::fever(fever_category c) : ctg(c), value(30) {}
fever::fever(fever_category c, size_t v) : ctg(c), value(v) {}

void *mk_girl(double pow = 5000,
              double tec = 5000,
              double stm = 5000,
              int apl = 150,
              double spike_prob = 0.5);
void *mk_apl_girl(double pow = 5000,
                  double tec = 5000,
                  double stm = 5000,
                  int apl = 150,
                  double spike_prob = 0.5);
void *mk_girl(double pow, double pow_bonus,
              double tec, double tec_bonus, double tec_dfd_bonus,
              double stm, double stm_bonus,
              int apl, double appeal_bonus,
              initializer_list<fever> = {},
              initializer_list<tactics> = {},
              initializer_list<active> = {},
              season_category = no_season);
void *mk_girl(double pow, double tec, double stm,
              int apl, double spike_prob) {
  return new girl(make_simple_girl(pow, tec, stm, apl, spike_prob));
}
void *mk_apl_girl(double pow, double tec, double stm,
                  int apl, double spike_prob) {
  girl g = make_simple_girl(pow, tec, stm, apl, spike_prob);
  g.apl_season();
  return new girl(g);
}
void *mk_girl(double pow, double pow_bonus,
              double tec, double tec_bonus, double tec_dfd_bonus,
              double stm, double stm_bonus,
              int apl, double appeal_bonus,
              initializer_list<fever> f_v,
              initializer_list<tactics> t_v,
              initializer_list<active> a_v,
              season_category s_ctg) {
  girl *ret{};
  ret = new girl(pow, pow_bonus,
                 tec, tec_bonus, tec_dfd_bonus,
                 stm, stm_bonus,
                 apl, appeal_bonus, 1.2,
                 0.0, 0.0, 0.0,
                 0.5, 0.0, 0.0, 0.0);
  auto &g = *ret;
  for (auto &x : f_v) {
    switch(x.ctg) {
    case killer_f:
      g.pow_fever_bonus = clamp(g.pow_fever_bonus
                                + ((double)x.value / 100.0),
                                0.0, 1.0);
      break;
    case sweet_f:
      g.tec_fever_bonus = clamp(g.tec_fever_bonus
                                + ((double)x.value / 100.0),
                                0.0, 1.0);
      break;
    case yell_f:
      g.partner_fever_bonus = clamp(g.partner_fever_bonus
                                    + ((double)x.value / 100.0),
                                    0.0, 1.0);
      break;
    default:
      break;
    }
  }
  for (auto &x : t_v) {
    switch(x.ctg) {
    case angel:
      g.spike_prob = clamp(g.spike_prob + ((double)x.value / 100.0),
                           0.0, 1.0);
      break;
    case fairy:
      g.spike_prob = clamp(g.spike_prob - ((double)x.value / 100.0),
                           0.0, 1.0);
      break;
    case platinum_atk:
      g.atk_prob_bonus = clamp(g.atk_prob_bonus + ((double)x.value / 100.0),
                               0.0, 1.0);
      break;
    case platinum_dfd:
      g.dfd_prob_bonus = clamp(g.dfd_prob_bonus + ((double)x.value / 100.0),
                               0.0, 1.0);
      break;
    case precise_atk:
      g.atk_ace_bonus = clamp(g.atk_ace_bonus + ((double)x.value / 100.0),
                              0.0, 1.0);
      break;
    default:
      break;
    }
  }
  for (auto &x : a_v) {
    if (g.skls.full())
      print_then_terminate("one girl can only hold 11 skills at most\n");
    g.skls.push_back(make_a(x));
  }
  switch (s_ctg) {
  case pow_season:
    g.pow_season();
    break;
  case tec_season:
    g.tec_season();
    break;
  case stm_season:
    g.stm_season();
    break;
  case apl_season:
    g.apl_season();
    break;
  default:
    break;
  }
  return ret;
}
void *mk_girl(const void *p, season_category s) {
  girl g(*static_cast<const girl *>(p));
  switch (s) {
  case pow_season:
    g.pow_season();
    break;
  case tec_season:
    g.tec_season();
    break;
  case stm_season:
    g.stm_season();
    break;
  case apl_season:
    g.apl_season();
    break;
  default:
    break;
  }
  return new girl(g);
}

enum class attacking_method {
  none, spike, feint
};
enum class select_attacker {
  none, ace, aux
};
enum class select_target {
  none, ace, aux
};
void *mk_team(const void *, const void *, const void *,
              attacking_method = attacking_method::none,
              select_attacker = select_attacker::none,
              select_target = select_target::none,
              star_level = ss_p);
void *mk_team(const void *ace, const void *aux, const void *sub,
              attacking_method a, select_attacker b, select_target c,
              star_level lv) {
  team *ret = new team(*static_cast<const girl *>(ace),
                       *static_cast<const girl *>(aux),
                       *static_cast<const girl *>(sub),
                       0.0, 0.50, 0.50, lv);
  auto &t = *ret;

  if (a == attacking_method::spike)
    t.spike();
  else if (a == attacking_method::feint)
    t.feint();

  if (b == select_attacker::ace)
    t.ace_atk();
  else if (b == select_attacker::aux)
    t.aux_atk();

  if (c == select_target::ace)
    t.atk_ace();
  else if (c == select_target::aux)
    t.atk_aux();

  t.apply_apl_season();

  return ret;
}
void *mk_team(const void *ace, const void *aux, const void *sub,
              star_level lv) {
  const girl &g1 = *static_cast<const girl *>(ace);
  const attacking_method a = (g1.orgn_pow > g1.orgn_tec)
    ? attacking_method::spike : attacking_method::feint;
  return mk_team(ace, aux, sub,
                 a, select_attacker::ace, select_target::ace, lv);
}
void *mk_team(const void *a, const void *b) {
  return mk_team(a, b, mk_girl(), ss_p);
}
void *mk_team(const void *a, const void *b, star_level lv) {
  return mk_team(a, b, mk_girl(), lv);
}
void *mk_apl_team(const void *ace, const void *aux, star_level lv) {
  const girl &g1 = *static_cast<const girl *>(ace);
  const attacking_method a = (g1.orgn_pow > g1.orgn_tec)
    ? attacking_method::spike : attacking_method::feint;
  return mk_team(ace, aux, mk_apl_girl(),
                 a, select_attacker::ace, select_target::ace, lv);
}
void *mk_apl_team(const void *ace, const void *aux) {
  return mk_apl_team(ace, aux, ss_p);
}

void sim(const void *t1, const void *t2,
         int score = 8,
         boost_strategy = boost_strategy::none,
         sub_strategy = sub_strategy::none,
         race_category = race_category::normal);
void sim(const void *team1, const void *team2,
         int score,
         boost_strategy boost_s,
         sub_strategy sub_s,
         race_category race_ctg) {
  const team &t1 = *static_cast<const team *>(team1);
  const team &t2 = *static_cast<const team *>(team2);
  const race r(t1, t2, score, sub_s, boost_s, race_ctg);

  result_t res;
  array<pair<result_t, pool_thread>, 32> a;
  for (auto &x : a)
    x.second = pool_thread([&r, &x]() {x.first = race(r).sim(50000);});
  for (auto &x : a)
    x.second.try_join();
  for (auto &x : a)
    res.merge(x.first);
  res.print();
}

const girl arena_pow_ace(11000, 0.12 + 0.20,
                         6500, 0.00, 0.00,
                         8300, 0.80,
                         150, 0.00, 1.2,
                         0.30, 0.00, 0.00,
                         0.58, 0.00, 0.00, 0.00,
                         make_a(pow_c0),
                         make_a(pressure_c0),
                         make_a(stm_c0),
                         make_a(pow_wg),
                         make_a(pow_wg),
                         make_a(pow_h0),
                         make_a(pow_e));
const girl arena_tec_ace(6500, 0.00,
                         11000, 0.12 + 0.20, 0.00,
                         8300, 0.80,
                         150, 0.00, 1.2,
                         0.30, 0.00, 0.00,
                         0.42, 0.00, 0.00, 0.00,
                         make_a(tec_c0),
                         make_a(stm_c0),
                         make_a(tec_wg),
                         make_a(tec_wg),
                         make_a(tec_h0),
                         make_a(tec_e));
const girl arena_pow_aux(5500, 0.00,
                         7000, 0.00, 0.50,
                         8000, 0.00,
                         150, 0.00, 1.2,
                         0.00, 0.00, 0.30,
                         0.50, 0.00, 0.15, 0.00,
                         make_a(pressure_c0),
                         make_a(stm_c0),
                         make_a(genius_c),
                         make_a(pressure_f),
                         make_a(pressure_f0),
                         make_a(pressure_c0));
const girl arena_tec_aux(7000, 0.00,
                         5500, 0.00, 0.50,
                         8000, 0.00,
                         150, 0.00, 1.2,
                         0.00, 0.00, 0.30,
                         0.50, 0.00, 0.15, 0.00,
                         make_a(stm_c0),
                         make_a(pressure_f),
                         make_a(pressure_f0),
                         make_a(pressure_c0));
const girl arena_gns_aux(7000, 0.00,
                         5500, 0.00, 0.50,
                         8000, 0.00,
                         150, 0.00, 1.2,
                         0.00, 0.00, 0.30,
                         0.50, 0.00, 0.15, 0.00,
                         make_a(stm_c0),
                         make_a(pressure_f),
                         make_a(genius_c),
                         make_a(genius_d0),
                         make_a(genius_d0));
const girl arena_pow_atk_aux(9500, 0.11 + 0.20,
                             5800, 0.00, 0.00,
                             6800, 0.60,
                             150, 0.00, 1.2,
                             0.00, 0.00, 0.30,
                             0.55, 0.00, 0.00, 0.00,
                             make_a(pow_c0),
                             make_a(pressure_c0),
                             make_a(stm_c0),
                             make_a(pow_e), make_a(pow_c0), make_a(pow_c0));
const girl arena_tec_atk_aux(5800, 0.00,
                             9500, 0.11 + 0.20, 0.00,
                             6800, 0.60,
                             150, 0.00, 1.2,
                             0.00, 0.00, 0.30,
                             0.45, 0.00, 0.00, 0.00,
                             make_a(tec_c0),
                             make_a(stm_c0),
                             make_a(tec_e), make_a(tec_c0), make_a(tec_c0));
enum class target_team_color {
  rnd, pow, tec,
  aux,
  atk_aux,
  pow_aux,
  pow_atk_aux,
  tec_aux,
  tec_atk_aux,
  tec_gns_aux
};
enum class arena_picking_strategy {
  none,
  pick_w_atk_teams,
  pick_optimal_w_atk_teams,
  pick_dfd_teams,
};
void sim_arena(const void *t,
               double photo_bonus,
               target_team_color,
               boost_strategy = boost_strategy::none,
               sub_strategy = sub_strategy::none);
void sim_arena(const void *p,
               double photo_bonus,
               target_team_color c,
               boost_strategy boost_s,
               sub_strategy sub_s) {
  array<pair<result_t, pool_thread>, 32> a;
  for (auto &x : a)
    x.second = pool_thread([&x, p, c, photo_bonus, boost_s, sub_s]() {
      const auto sim_f = [=](const void *team1, const void *team2,
                             uint64_t n) {
        const team &t1 = *static_cast<const team *>(team1);
        const team &t2 = *static_cast<const team *>(team2);
        return race(t1, t2,
                    8, sub_s, boost_s,
                    race_category::arena_no_sub)
          .sim(n);
      };
      result_t ret;
      switch (c) {
      case target_team_color::rnd:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_aux),
                                  mk_girl()),
                          11250));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_atk_aux),
                                  mk_girl()),
                          13750));

          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_aux),
                                  mk_girl()),
                          7875));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_gns_aux),
                                  mk_girl()),
                          3375));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_atk_aux),
                                  mk_girl()),
                          13750));
        }
        break;
      case target_team_color::pow:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_aux),
                                  mk_girl()),
                          13750 * 2));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_atk_aux),
                                  mk_girl()),
                          11250 * 2));
        }
        break;
      case target_team_color::tec:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_aux),
                                  mk_girl()),
                          9625 * 2));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_gns_aux),
                                  mk_girl()),
                          4125 * 2));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_atk_aux),
                                  mk_girl()),
                          11250 * 2));
        }
        break;
      case target_team_color::atk_aux:
        {
           ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_atk_aux),
                                  mk_girl()),
                          25000));
           ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_atk_aux),
                                  mk_girl()),
                          25000));
        }
        break;
      case target_team_color::aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_aux),
                                  mk_girl()),
                          25000));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_aux),
                                  mk_girl()),
                          17500));
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_gns_aux),
                                  mk_girl()),
                          7500));
        }
        break;
      case target_team_color::pow_aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_aux),
                                  mk_girl()),
                          50000));
        }
        break;
      case target_team_color::pow_atk_aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_pow_ace),
                                  addressof(arena_pow_atk_aux),
                                  mk_girl()),
                          50000));
        }
        break;
      case target_team_color::tec_aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_aux),
                                  mk_girl()),
                          50000));
        }
        break;
      case target_team_color::tec_atk_aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_tec_atk_aux),
                                  mk_girl()),
                          50000));
        }
        break;
      case target_team_color::tec_gns_aux:
        {
          ret.merge(sim_f(p,
                          mk_team(addressof(arena_tec_ace),
                                  addressof(arena_gns_aux),
                                  mk_girl()),
                          50000));
        }
        break;
      default:
        print_then_terminate("wrong team color\n");
        break;
      }
      x.first = ret;
    });
  for (auto &x : a)
    x.second.try_join();
  result_t res;
  for (auto &x : a)
    res.merge(x.first);
  res.print_adjusted_avg(photo_bonus);
  res.print();
}
void sim_arena(const void *t1,
               const void *t2,
               const void *t3,
               double photo_bonus,
               unsigned repeating_times,
               arena_picking_strategy ps,
               boost_strategy boost_s1 = boost_strategy::none,
               boost_strategy boost_s2 = boost_strategy::none,
               boost_strategy boost_s3 = boost_strategy::none,
               sub_strategy sub_s1 = sub_strategy::none,
               sub_strategy sub_s2 = sub_strategy::none,
               sub_strategy sub_s3 = sub_strategy::none);
void sim_arena(const void *team1,
               const void *team2,
               const void *team3,
               double photo_bonus,
               unsigned repeating_times,
               arena_picking_strategy ps,
               boost_strategy boost_s1,
               boost_strategy boost_s2,
               boost_strategy boost_s3,
               sub_strategy sub_s1,
               sub_strategy sub_s2,
               sub_strategy sub_s3) {
  const team &t1 = *static_cast<const team *>(team1);
  const team &t2 = *static_cast<const team *>(team2);
  const team &t3 = *static_cast<const team *>(team3);

  result_t res1, res2, res3;
  const uint64_t n1 = 30;
  const uint64_t n2 = 500;
  vector<pair<tuple<result_t, result_t, result_t>, pool_thread>> a(32);
  for (auto &x : a) {
    const auto f = [=, &x, &t1, &t2, &t3]() {
      const uint64_t lim = 150000;
      uint64_t lim_count = 0;
      result_t r1, r2, r3;
      const auto make_race1 = [=](const team &t_a, const team &t_b) {
        return race(t_a, t_b,
                    8, sub_s1, boost_s1, race_category::arena_no_sub);
      };
      const auto make_race2 = [=](const team &t_a, const team &t_b) {
        return race(t_a, t_b,
                    8, sub_s2, boost_s2, race_category::arena_no_sub);
      };
      const auto make_race3 = [=](const team &t_a, const team &t_b) {
        return race(t_a, t_b,
                    8, sub_s3, boost_s3, race_category::arena_no_sub);
      };
      const auto is_pow_team = [](const team &u) {
        return u.ace.orgn_pow > u.ace.orgn_tec;
      };
      if (ps == arena_picking_strategy::pick_w_atk_teams) {
        const auto get_rnd_team = []() {
          return rnd(0.5)
            ? team(arena_tec_ace, arena_tec_atk_aux)
            : team(arena_pow_ace, arena_pow_atk_aux);
        };
        const auto get_3rd_team
          = [is_pow_team](const team &tm1, const team &tm2) {
          if (is_pow_team(tm1) && is_pow_team(tm2))
            return team(arena_tec_ace, arena_tec_atk_aux);
          else if (!is_pow_team(tm1) && !is_pow_team(tm2))
            return team(arena_pow_ace, arena_pow_atk_aux);
          else
            return rnd(0.5)
              ? team(arena_tec_ace, arena_tec_atk_aux)
              : team(arena_pow_ace, arena_pow_atk_aux);
        };
        for (;;) {
          const team ai_t1 = get_rnd_team();
          const team ai_t2 = get_rnd_team();
          const team ai_t3 = get_3rd_team(ai_t1, ai_t2);
          r1.merge(make_race1(t1, ai_t1).sim(5));
          r2.merge(make_race2(t2, ai_t2).sim(5));
          r3.merge(make_race3(t3, ai_t3).sim(5));
          lim_count += 15;
          if (lim_count > lim)
            break;
        }
      }
      else if (ps == arena_picking_strategy::pick_optimal_w_atk_teams) {
        const auto get_right_w_atk_team
          = [is_pow_team](const team &t) {
            return is_pow_team(t)
              ? team(arena_tec_ace, arena_tec_atk_aux)
              : team(arena_pow_ace, arena_pow_atk_aux);
          };
        for (;;) {
          const team ai_t1 = get_right_w_atk_team(t1);
          const team ai_t2 = get_right_w_atk_team(t2);
          const team ai_t3 = get_right_w_atk_team(t3);
          r1.merge(make_race1(t1, ai_t1).sim(5));
          r2.merge(make_race2(t2, ai_t2).sim(5));
          r3.merge(make_race3(t3, ai_t3).sim(5));
          lim_count += 15;
          if (lim_count > lim)
            break;
        }
      }
      else if (ps == arena_picking_strategy::pick_dfd_teams) {
        const auto get_rnd_team = []() {
          return rnd(0.5)
            ? team(arena_tec_ace, (rnd(0.3) ? arena_gns_aux : arena_tec_aux))
            : team(arena_pow_ace, arena_pow_aux);
        };
        const auto get_3rd_team
          = [is_pow_team](const team &tm1, const team &tm2) {
          if (is_pow_team(tm1) && is_pow_team(tm2))
            return team(arena_tec_ace,
                        (rnd(0.3) ? arena_gns_aux : arena_tec_aux));
          else if (!is_pow_team(tm1) && !is_pow_team(tm2))
            return team(arena_pow_ace, arena_pow_aux);
          else
            return rnd(0.5)
              ? team(arena_tec_ace, (rnd(0.3) ? arena_gns_aux : arena_tec_aux))
              : team(arena_pow_ace, arena_pow_aux);
        };
        for (;;) {
          const team ai_t1 = get_rnd_team();
          const team ai_t2 = get_rnd_team();
          const team ai_t3 = get_3rd_team(ai_t1, ai_t2);
          r1.merge(make_race1(t1, ai_t1).sim(5));
          r2.merge(make_race2(t2, ai_t2).sim(5));
          r3.merge(make_race3(t3, ai_t3).sim(5));
          lim_count += 15;
          if (lim_count > lim)
            break;
        }
      }
      else {
        function<team ()> get_rnd_team;
        function<team (const team &, const team &)> get_3rd_team;
        if (rnd(0.60)) {
          get_rnd_team = []() {
            return rnd(0.5)
              ? team(arena_tec_ace, (rnd(0.3) ? arena_gns_aux : arena_tec_aux))
              : team(arena_pow_ace, arena_pow_aux);
          };
          get_3rd_team = [is_pow_team](const team &tm1, const team &tm2) {
            if (is_pow_team(tm1) && is_pow_team(tm2))
              return team(arena_tec_ace,
                          (rnd(0.3) ? arena_gns_aux : arena_tec_aux));
            else if (!is_pow_team(tm1) && !is_pow_team(tm2))
              return team(arena_pow_ace, arena_pow_aux);
            else
              return rnd(0.5)
                ? team(arena_tec_ace,
                       (rnd(0.3) ? arena_gns_aux : arena_tec_aux))
                : team(arena_pow_ace, arena_pow_aux);
          };
        }
        else {
          get_rnd_team = []() {
            return rnd(0.5)
              ? team(arena_tec_ace, arena_tec_atk_aux)
              : team(arena_pow_ace, arena_pow_atk_aux);
          };
          get_3rd_team = [is_pow_team](const team &tm1, const team &tm2) {
            if (is_pow_team(tm1) && is_pow_team(tm2))
              return team(arena_tec_ace, arena_tec_atk_aux);
            else if (!is_pow_team(tm1) && !is_pow_team(tm2))
              return team(arena_pow_ace, arena_pow_atk_aux);
            else
              return rnd(0.5)
                ? team(arena_tec_ace, arena_tec_atk_aux)
                : team(arena_pow_ace, arena_pow_atk_aux);
          };
        }
        for (;;) {
          const team ai_t1 = get_rnd_team();
          const team ai_t2 = get_rnd_team();
          const team ai_t3 = get_3rd_team(ai_t1, ai_t2);
          r1.merge(make_race1(t1, ai_t1).sim(5));
          r2.merge(make_race2(t2, ai_t2).sim(5));
          r3.merge(make_race3(t3, ai_t3).sim(5));
          lim_count += 15;
          if (lim_count > lim)
            break;
        }
      }
      x.first = tie(r1, r2, r3);
    };
    x.second = pool_thread(f);
  }
  for (auto &x : a)
    x.second.try_join();
  for (auto &x : a) {
    res1.merge(at<0>(x.first));
    res2.merge(at<1>(x.first));
    res3.merge(at<2>(x.first));
  }
  const size_t v0_n = res1.appeal_scores.size();
  if (v0_n == 0u || repeating_times == 0) {
    putln("no result");
    return;
  }
  vector<uint64_t> v0(v0_n, 0);
  for (size_t i: irng(0u, v0_n)) {
    const double win_bonus = ((res1.appeal_scores[i].second ? 0.1 : 0.0)
                              + (res2.appeal_scores[i].second ? 0.1 : 0.0)
                              + (res3.appeal_scores[i].second ? 0.1 : 0.0));
    v0[i]
      = res1.appeal_scores[i].first
      + res2.appeal_scores[i].first
      + res3.appeal_scores[i].first;
    v0[i] *= (2.0 + win_bonus + photo_bonus);
  }

  vector<uint64_t> v;
  v.reserve(v0_n / repeating_times + 1u);
  auto p2 = v0.end();
  for (;;) {
    if (to_unsigned(p2 - v0.begin()) < repeating_times)
      break;
    uint64_t tmp = 0;
    for (auto c : irng(0u, repeating_times)) {
      --p2;
      if (*p2 > tmp)
        tmp = *p2;
    }
    v.push_back(tmp);
  }
  if (v.size() < 100u) {
    putln("no result");
    return;
  }
  sort(v);
  const auto v_n = v.size();
  const uint64_t rank_of_1pct = *(v.rbegin() + (size_t)(v_n * 0.01));
  const uint64_t rank_of_10pct = *(v.rbegin() + (size_t)(v_n * 0.10));
  const uint64_t rank_of_33pct = *(v.rbegin() + (size_t)(v_n * 0.33));
  const uint64_t rank_of_50pct = *(v.rbegin() + (size_t)(v_n * 0.50));
  const uint64_t rank_of_66pct = *(v.rbegin() + (size_t)(v_n * 0.66));
  const uint64_t rank_of_90pct = *(v.rbegin() + (size_t)(v_n * 0.90));
  const uint64_t rank_of_95pct = *(v.rbegin() + (size_t)(v_n * 0.95));
  const uint64_t rank_of_99pct = *(v.rbegin() + (size_t)(v_n * 0.99));

  res1.print_adjusted_avg(photo_bonus);
  res1.print();
  putln("----------------");
  res2.print_adjusted_avg(photo_bonus);
  res2.print();
  putln("----------------");
  res3.print_adjusted_avg(photo_bonus);
  res3.print();
  putln("----------------");

  put("average:     ");
  putln(accumulate(v, (uint64_t)0,
                   [](uint64_t x, uint64_t y) {
                     return add_with_check(x, y).value();
                   })
        / v.size());
  put("rank of 1%:  ");
  putln(rank_of_1pct);
  put("rank of 10%: ");
  putln(rank_of_10pct);
  put("rank of 33%: ");
  putln(rank_of_33pct);
  put("rank of 50%: ");
  putln(rank_of_50pct);
  put("rank of 66%: ");
  putln(rank_of_66pct);
  put("rank of 90%: ");
  putln(rank_of_90pct);
  put("rank of 95%: ");
  putln(rank_of_95pct);
  put("rank of 99%: ");
  putln(rank_of_99pct);
}

double get_strength(const void *p) {
  const girl &g = *static_cast<const girl *>(p);
  const double pow_p = 1.0 + g.pow_bonus;
  const double tec_p = 1.0 + g.tec_bonus + g.tec_dfd_bonus;
  const double stm_p = 1.0 + g.stm_bonus;
  const double apl_p = 1.0 + g.apl_bonus;
  double pow_a = 1.0;
  double tec_a = 1.0;
  double stm_a = 1.0;
  double apl_a = 1.0;
  for (const auto &x : g.skls)
    for (const auto &z : seq(x.v1, x.v2)) {
      switch (z.first) {
      case active_color::pow:
        pow_a += z.second;
        break;
      case active_color::tec:
        tec_a += z.second;
        break;
      case active_color::stm:
        stm_a += z.second;
        break;
      case active_color::apl:
        apl_a += z.second;
        break;
      default:
        print_then_terminate("get_strength(): wrong active color\n");
        break;
      }
    }

  const double base_strength
    = ((g.orgn_pow * pow_a * pow_p
        + g.orgn_tec * tec_a * tec_p) * 0.1995
       * (1.0 + g.orgn_apl * apl_a * apl_p * 0.001211))
    + (g.orgn_stm * stm_a * stm_p * 0.50);
  const double angel_strength
    = ((g.orgn_pow * pow_a * pow_p - g.orgn_tec * tec_a * tec_p) * 0.3817
       + (g.orgn_pow * pow_p - g.orgn_tec * tec_p) * 0.3817)
    * (g.spike_prob - 0.50);
  const double fever_strength
    = (g.pow_fever_bonus + g.tec_fever_bonus + g.partner_fever_bonus)
    * (g.orgn_pow * (pow_p * (pow_a + pow_p - 1.0))
       + g.orgn_tec * (tec_p * (tec_a + tec_p - 1.0)))
    * 0.18; // todo
  const double white_dfd_strength
    = (g.orgn_pow * pow_p + g.orgn_tec * tec_p) * 0.5 * g.dfd_prob_bonus;
  const double white_atk_coeff
    = 1.0 + g.atk_prob_bonus * 2;
  return (base_strength
          + angel_strength
          + fever_strength
          + white_dfd_strength)
    * white_atk_coeff;
}
void put_strength(const void *p) {
  putln(get_strength(p));
}

}
