//#define NOMINMAX
//#include <windows.h>

#include <re/base.h>
#include <re/test.h>
#include <re/range.h>
#include <re/allocator.h>
#include <re/dynamic.h>
#include <re/random.h>
#include <re/algorithm.h>
#include <re/container.h>
#include <re/io.h>

#include <cassert>

using namespace re;

using env_var_data_set_t = vector<string>;
using env_var_t = pair<string, env_var_data_set_t>;
using env_var_set_t = map<string, env_var_data_set_t>;

env_var_set_t env_var_set_load(const char *filename) {
  env_var_set_t ret;
  string s_all;
  {
    c_file f(filename);
    s_all = f.get();
  }

  const auto find_equal_sign = [](auto i) {
    while (*i != '=')
      ++i;
    return i;
  };
  const auto skip_semicolon = [](auto i) {
    while (*i == ';')
      ++i;
    return i;
  };
  const auto find_semicolon_or_newline = [](auto i) {
    while (*i != ';' && *i != '\n')
      ++i;
    return i;
  };
  auto it = begin(s_all);
  const auto ed = end(s_all);
  while (it != ed) {
    const auto it2 = find_equal_sign(it);
    string name(it, it2);
    env_var_data_set_t values;

    it = next(it2);
    it = skip_semicolon(it);
    while (*it != '\n') {
      const auto it3 = find_semicolon_or_newline(it);
      values.emplace_back(it, it3);
      it = skip_semicolon(it3);
    }
    ++it;

    ret.emplace(name, move(values));
    assert(ret.find(name) != ret.end());
  }

  return ret;
}

string env_var_set_to_string(const env_var_set_t &s) {
  string os;
  for (auto [x, y] : s) {
    os.append(x).append('=');
    for (auto &z : y)
      os.append(z).append(';');
    os.back() = '\n';
  }
  return os;
}

auto env_var_set_difference(const env_var_set_t &l,
                            const env_var_set_t &r) {
  pair<env_var_set_t, env_var_set_t> ret;
  auto &[extra, inter] = ret;
  set_difference(l, r, to_back(extra),
                 bind(less<>(), bind(at<0>, _1), bind(at<0>, _2)));
  set_intersection(l, r,
                   output_itr([&inter, &r](const auto &x) {
                       const auto &xx = x.second;
                       const auto &yy = r[x.first];
                       if (xx == yy)
                         return;
                       auto &c = inter[x.first];
                       for (auto &z : xx) {
                         if (find(yy, z) != end(yy))
                           c.push_back("%%%%%");
                         else
                           c.push_back(z);
                       }
                       if (empty(c))
                         inter.remove(x.first);
                     }),
                   bind(less<>(), bind(at<0>, _1), bind(at<0>, _2)));
  return ret;
}

int main()
{
  const env_var_set_t s_origin = env_var_set_load("INPUT_set.txt");
  const env_var_set_t s_vc64 = env_var_set_load("INPUT_set_msvc_x64.txt");
  auto [s_vc64_extra, s_vc64_inter] = env_var_set_difference(s_vc64, s_origin);
  const env_var_set_t s_vc86 = env_var_set_load("INPUT_set_msvc_x86.txt");
  auto [s_vc86_extra, s_vc86_inter] = env_var_set_difference(s_vc86, s_origin);

  const auto impl = [&s_origin]
    (const string &name,
     const env_var_set_t &s_extra,
     const env_var_set_t &s_inter) {
    c_file f_set(copy(name).append("_SET_COMMAND.txt").data(), "wb");
    c_file f_unset(copy(name).append("_UNSET_COMMAND.txt").data(), "wb");
    c_file f_manual(copy(name).append("_MANUAL_MODIFICATION.txt").data(), "wb");

    f_manual.put(env_var_set_to_string(s_inter));

    static thread_local const sview sl1 =
R"([Environment]::SetEnvironmentVariable("%%%", "%%%", "User");
)";
    static thread_local const ssplitter<> fmt_set_var(sl1, "%%%");
    static thread_local const sview sl2 =
R"([Environment]::SetEnvironmentVariable("%%%", "", "User");
)";
    static thread_local const ssplitter<> fmt_del_var(sl2, "%%%");

    string s;

    for (auto [x, y] : s_extra) {
      s.clear();
      auto i = begin(fmt_set_var);
      s.append(*i++);
      s.append(x);
      s.append(*i++);
      for (const auto &z : y)
        s.append(z).append(';');
      s.pop_back();
      s.append(*i++);
      f_set.put(s);
    }

    for (auto [x, y] : s_extra) {
      s.clear();
      auto i = begin(fmt_del_var);
      s.append(*i++);
      s.append(x);
      s.append(*i++);
      f_unset.put(s);
    }
  };
  impl("OUTPUT_MSVC64", s_vc64_extra, s_vc64_inter);
  impl("OUTPUT_MSVC32", s_vc86_extra, s_vc86_inter);
}
