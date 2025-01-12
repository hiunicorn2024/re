#include <initializer_list>
#include <cstdio>

namespace re::vv {

using size_t = std::size_t;

using std::initializer_list;

enum star_level {
  no_star_level, ss_p, sss_m, sss, sss_p
};
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

enum tactics_category {
  angel,
  fairy,
  platinum_atk,
  platinum_dfd,
  precise_atk // positive for ace lock, negative for aux lock
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

enum season_category {
  no_season, pow_season, tec_season, stm_season, apl_season
};

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
void *mk_girl(const void *, season_category);

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
void *mk_team(const void *, const void *, const void *, star_level);
void *mk_team(const void *, const void *);
void *mk_team(const void *, const void *, star_level);
void *mk_apl_team(const void *, const void *, star_level);
void *mk_apl_team(const void *, const void *);

void sim(const void *t1, const void *t2,
         int score = 8,
         boost_strategy = boost_strategy::none,
         sub_strategy = sub_strategy::none,
         race_category = race_category::normal);

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
  pick_dfd_teams
};
void sim_arena(const void *t,
               double photo_bonus,
               target_team_color,
               boost_strategy = boost_strategy::none,
               sub_strategy = sub_strategy::none);
void sim_arena(const void *t1,
               const void *t2,
               const void *t3,
               double photo_bonus,
               unsigned repeating_times,
               arena_picking_strategy ps,
               boost_strategy = boost_strategy::none,
               boost_strategy = boost_strategy::none,
               boost_strategy = boost_strategy::none,
               sub_strategy = sub_strategy::none,
               sub_strategy = sub_strategy::none,
               sub_strategy = sub_strategy::none);

extern const active pow_j0;
extern const active pow_j;
extern const active pow_i0;
extern const active pow_i;
extern const active pow_wg0;
extern const active pow_wg;
extern const active pow_h0;
extern const active pow_h;
extern const active pow_g0;
extern const active pow_g;
extern const active pow_a0;
extern const active pow_a;
extern const active pow_b0;
extern const active pow_b;
extern const active pow_c0;
extern const active pow_c;
extern const active pow_d0;
extern const active pow_d;
extern const active pow_e0;
extern const active pow_e;
extern const active pow_f0;
extern const active pow_f;
extern const active tec_wg0;
extern const active tec_wg;
extern const active tec_j0;
extern const active tec_j;
extern const active tec_i0;
extern const active tec_i;
extern const active tec_h0;
extern const active tec_h;
extern const active tec_g0;
extern const active tec_g;
extern const active tec_a0;
extern const active tec_a;
extern const active tec_b0;
extern const active tec_b;
extern const active tec_c0;
extern const active tec_c;
extern const active tec_d0;
extern const active tec_d;
extern const active tec_e0;
extern const active tec_e;
extern const active tec_f0;
extern const active tec_f;
extern const active stm_a0;
extern const active stm_a;
extern const active stm_b0;
extern const active stm_b;
extern const active stm_c0;
extern const active stm_c;
extern const active stm_d0;
extern const active stm_d;
extern const active stm_e0;
extern const active stm_e;
extern const active stm_ee0;
extern const active stm_ee;
extern const active stm_f0;
extern const active stm_f;
extern const active dance_b0;
extern const active dance_c0;
extern const active dance_d0;
extern const active dance_e0;
extern const active dance_f0;
extern const active dance_b;
extern const active dance_c;
extern const active dance_d;
extern const active dance_e;
extern const active dance_f;
extern const active pressure_b0;
extern const active pressure_c0;
extern const active pressure_d0;
extern const active pressure_e0;
extern const active pressure_f0;
extern const active pressure_b;
extern const active pressure_c;
extern const active pressure_d;
extern const active pressure_e;
extern const active pressure_f;
extern const active firm_b0;
extern const active firm_c0;
extern const active firm_d0;
extern const active firm_e0;
extern const active firm_f0;
extern const active firm_b;
extern const active firm_c;
extern const active firm_d;
extern const active firm_e;
extern const active firm_f;
extern const active genius_b0;
extern const active genius_c0;
extern const active genius_d0;
extern const active genius_e0;
extern const active genius_f0;
extern const active genius_b;
extern const active genius_c;
extern const active genius_d;
extern const active genius_e;
extern const active genius_f;
extern const active iron_b0;
extern const active iron_c0;
extern const active iron_d0;
extern const active iron_e0;
extern const active iron_f0;
extern const active iron_b;
extern const active iron_c;
extern const active iron_d;
extern const active iron_e;
extern const active iron_f;

double get_strength(const void *);
void put_strength(const void *);

void test() {
  const void *sssp_pow_g1 = mk_girl(13511, 0.0,
                                    10011, 0.0, 0.0,
                                    10811, 0.0,
                                    210, 0.0,
                                    {}, {},
                                    {
                                      active(groove_minus, 0.50, 0.10),
                                      active(groove_minus, 0.50, 0.10),
                                      active(atk_tec_minus, 0.30, 0.10),
                                      active(atk_pow_plus, 0.20, 0.10)
                                    });
  const void *sssp_pow_g2 = mk_girl(13686, 0.0,
                                    11077, 0.0, 0.0,
                                    10346, 0.0,
                                    210, 0.0,
                                    {}, {},
                                    {
                                      active(groove_minus, 0.50, 0.10),
                                      active(groove_minus, 0.50, 0.10),
                                      active(atk_pow_plus, 0.20, 0.10),
                                      active(atk_tec_minus, 0.16, 0.20)
                                    });
  const void *sssp_tec_g1 = mk_girl(10011, 0.0,
                                    13511, 0.0, 0.0,
                                    10811, 0.0,
                                    210, 0.0,
                                    {}, {},
                                    {
                                      active(groove_minus, 0.50, 0.10),
                                      active(groove_minus, 0.50, 0.10),
                                      active(atk_tec_minus, 0.30, 0.10),
                                      active(atk_pow_plus, 0.20, 0.10)
                                    });
  const void *sssp_tec_g2 = mk_girl(11077, 0.0,
                                    13686, 0.0, 0.0,
                                    10346, 0.0,
                                    210, 0.0,
                                    {}, {},
                                    {
                                      active(groove_minus, 0.50, 0.10),
                                      active(groove_minus, 0.50, 0.10),
                                      active(atk_pow_plus, 0.20, 0.10),
                                      active(atk_tec_minus, 0.16, 0.20)
                                    });
  const void *sssp_pow_team = mk_team(sssp_pow_g1, sssp_pow_g2, mk_girl());
  const void *sssp_tec_team = mk_team(sssp_tec_g1, sssp_tec_g2, mk_girl());

  const void *sss_pow_g1 = mk_girl(10923, 0.0,
                                   9549, 0.0, 0.0,
                                   11145, 0.0,
                                   200, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *sss_pow_g2 = mk_girl(12168, 0.0,
                                   9340, 0.0, 0.0,
                                   9952, 0.0,
                                   200, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.30, 0.10),
                                     active(atk_pow_plus, 0.20, 0.10)
                                   });
  const void *sss_tec_g1 = mk_girl(9549, 0.0,
                                   10923, 0.0, 0.0,
                                   11145, 0.0,
                                   200, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *sss_tec_g2 = mk_girl(9340, 0.0,
                                   12168, 0.0, 0.0,
                                   9952, 0.0,
                                   200, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.30, 0.10),
                                     active(atk_pow_plus, 0.20, 0.10)
                                   });
  const void *sss_pow_team = mk_team(sss_pow_g1, sss_pow_g2, mk_girl());
  const void *sss_tec_team = mk_team(sss_tec_g1, sss_tec_g2, mk_girl());

  const void *ssp_pow_g1 = mk_girl(10924, 0.0,
                                   9030, 0.0, 0.0,
                                   9580, 0.0,
                                   190, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *ssp_pow_g2 = mk_girl(11046, 0.0,
                                   9274, 0.0, 0.0,
                                   9344, 0.0,
                                   190, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *ssp_tec_g1 = mk_girl(9030, 0.0,
                                   10924, 0.0, 0.0,
                                   9580, 0.0,
                                   190, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *ssp_tec_g2 = mk_girl(9274, 0.0,
                                   11046, 0.0, 0.0,
                                   9344, 0.0,
                                   190, 0.0,
                                   {}, {},
                                   {
                                     active(groove_minus, 0.50, 0.10),
                                     active(groove_minus, 0.50, 0.10),
                                     active(atk_tec_minus, 0.16, 0.20),
                                     active(atk_pow_plus, 0.20, 0.15)
                                   });
  const void *ssp_pow_team = mk_team(ssp_pow_g1, ssp_pow_g2, mk_girl());
  const void *ssp_tec_team = mk_team(ssp_tec_g1, ssp_tec_g2, mk_girl());

  const void *g = mk_girl(12000, 0.50,
                          8000, 0.0, 0.0,
                          8000, 1.20 - 0.50,
                          280, 0.0,
                          {
                            fever(killer_f),
                            fever(sweet_f),
                            fever(yell_f)
                          },
                          {
                            tactics(angel),
                            tactics(fairy),
                            tactics(platinum_atk),
                            tactics(platinum_dfd),
                            tactics(precise_atk)
                          },
                          {
                            pressure_b0,
                            pow_b0,
                            stm_b0,
                            pow_h,
                            pow_h, pow_h, pow_h
                          },
                          no_season);

  const bool single_team = true;
  if (single_team) {
    /*
    sim(mk_team(g, g, mk_girl(),
                attacking_method::feint,
                select_attacker::none,
                select_target::aux,
                ss_p),
        ssp_pow_team, 8, boost_strategy::always);
    */
    sim_arena(mk_team(g, g, mk_girl(), ss_p),
              0.50,
              target_team_color::rnd,
              boost_strategy::none,
              sub_strategy::none);
  }
  else {
    const star_level lv = ss_p;
    const auto make_team
      = [=](const void *g1, const void *g2) {
        return mk_team(g1, g2, mk_girl(), lv);
      };
    sim_arena(make_team(g, g),
              make_team(g, g),
              make_team(g, g),
              0.50,
              35,
              arena_picking_strategy::pick_w_atk_teams,
              boost_strategy::none,
              boost_strategy::none,
              boost_strategy::none);
  }
}

}

int main() {
  re::vv::test();
}
