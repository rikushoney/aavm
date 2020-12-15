#ifndef AAVM_KEYWORD_MAP_H
#define AAVM_KEYWORD_MAP_H

#include "character.h"
#include "token.h"

#include <algorithm>
#include <array>
#include <optional>
#include <string_view>
#include <utility>

namespace aavm::parser::keyword {

namespace detail {

template <typename T, std::size_t... I>
constexpr auto make_array_impl(T (&a)[sizeof...(I)], std::index_sequence<I...>)
    -> std::array<T, sizeof...(I)> {
  return {{std::move(a[I])...}};
}

template <typename T, std::size_t N> constexpr auto make_array(T(&&a)[N]) {
  return make_array_impl(a, std::make_index_sequence<N>{});
}

constexpr auto gen_keyword_map() {
  using namespace std::string_view_literals;

  return make_array<std::pair<std::string_view, token::Kind>>(
      {{"eq"sv, token::COND_eq},      {"ne"sv, token::COND_ne},
       {"cs"sv, token::COND_cs},      {"cc"sv, token::COND_cc},
       {"mi"sv, token::COND_mi},      {"pl"sv, token::COND_pl},
       {"vs"sv, token::COND_vs},      {"hi"sv, token::COND_hi},
       {"ls"sv, token::COND_ls},      {"ge"sv, token::COND_ge},
       {"lt"sv, token::COND_lt},      {"gt"sv, token::COND_gt},
       {"le"sv, token::COND_le},      {"al"sv, token::COND_al},

       {"r0"sv, token::REG_r0},       {"r1"sv, token::REG_r1},
       {"r2"sv, token::REG_r2},       {"r3"sv, token::REG_r3},
       {"r4"sv, token::REG_r4},       {"r5"sv, token::REG_r5},
       {"r6"sv, token::REG_r6},       {"r7"sv, token::REG_r7},
       {"r8"sv, token::REG_r8},       {"r9"sv, token::REG_r9},
       {"r10"sv, token::REG_r10},     {"r11"sv, token::REG_r11},
       {"r12"sv, token::REG_r12},     {"sp"sv, token::REG_sp},
       {"lr"sv, token::REG_lr},       {"pc"sv, token::REG_pc},

       {"add"sv, token::OP_add},      {"adc"sv, token::OP_adc},

       {"sub"sv, token::OP_sub},      {"sbc"sv, token::OP_sbc},
       {"rsb"sv, token::OP_rsb},      {"rsc"sv, token::OP_rsc},

       {"mul"sv, token::OP_mul},      {"mla"sv, token::OP_mla},
       {"mls"sv, token::OP_mls},      {"umull"sv, token::OP_umull},
       {"umlal"sv, token::OP_umlal},  {"smull"sv, token::OP_smull},
       {"smlal"sv, token::OP_smlal},

       {"sdiv"sv, token::OP_sdiv},    {"udiv"sv, token::OP_udiv},
       {"mov"sv, token::OP_mov},      {"movt"sv, token::OP_movt},
       {"movw"sv, token::OP_movw},

       {"asr"sv, token::OP_asr},      {"lsl"sv, token::OP_lsl},
       {"lsr"sv, token::OP_lsr},      {"ror"sv, token::OP_ror},
       {"rrx"sv, token::OP_rrx},

       {"cmp"sv, token::OP_cmp},      {"cmn"sv, token::OP_cmn},

       {"tst"sv, token::OP_tst},      {"teq"sv, token::OP_teq},
       {"and"sv, token::OP_and},      {"eor"sv, token::OP_eor},
       {"orr"sv, token::OP_orr},      {"bic"sv, token::OP_bic},

       {"bfc"sv, token::OP_bfc},      {"bfi"sv, token::OP_bfi},
       {"sbfx"sv, token::OP_sbfx},    {"ubfx"sv, token::OP_ubfx},

       {"rbit"sv, token::OP_rbit},    {"rev"sv, token::OP_rev},
       {"rev16"sv, token::OP_rev16},  {"revsh"sv, token::OP_revsh},

       {"b"sv, token::OP_b},          {"bl"sv, token::OP_bl},
       {"bx"sv, token::OP_bx},        {"cbz"sv, token::OP_cbz},
       {"cbnz"sv, token::OP_cbnz},

       {"adr"sv, token::OP_adr},

       {"ldr"sv, token::OP_ldr},      {"ldrb"sv, token::OP_ldrb},
       {"ldrh"sv, token::OP_ldrh},    {"ldrsb"sv, token::OP_ldrsb},
       {"ldrsh"sv, token::OP_ldrsh},  {"str"sv, token::OP_str},
       {"strb"sv, token::OP_strb},    {"strh"sv, token::OP_strh},
       {"ldm"sv, token::OP_ldm},      {"ldmia"sv, token::OP_ldmia},
       {"ldmib"sv, token::OP_ldmib},  {"ldmda"sv, token::OP_ldmda},
       {"ldmdb"sv, token::OP_ldmdb},  {"stm"sv, token::OP_stm},
       {"stmia"sv, token::OP_stmia},  {"stmib"sv, token::OP_stmib},
       {"stmda"sv, token::OP_stmda},  {"stmdb"sv, token::OP_stmdb},
       {"push"sv, token::OP_push},    {"pop"sv, token::OP_pop},
       {"dcb"sv, token::DIR_dcb},     {"equ"sv, token::DIR_equ},
       {"space"sv, token::DIR_space}, {"fill"sv, token::DIR_fill}});
}

constexpr auto keyword_map{gen_keyword_map()};

} // namespace detail

constexpr auto none{detail::keyword_map.end()};

constexpr auto find(std::string_view what) {
  for (auto it = detail::keyword_map.begin(); it != none; ++it) {
    if (it->first == what) {
      return it;
    }
  }

  return none;
}

template <typename Pred> constexpr auto find(Pred &&pred) {
  for (auto it = detail::keyword_map.begin(); it != none; ++it) {
    if (pred(it->first)) {
      return it;
    }
  }

  return none;
}

template <typename Pred> constexpr auto find_longest(Pred &&pred) {
  auto match = none;
  for (auto it = detail::keyword_map.begin(); it != none; ++it) {
    if (pred(it->first)) {
      if (match == none || match->first.length() < it->first.length()) {
        match = it;
      }
    }
  }

  return match;
}

constexpr auto to_string(token::Kind tok) -> std::optional<std::string_view> {
  for (auto it = detail::keyword_map.begin(); it != none; ++it) {
    if (it->second == tok) {
      return std::make_optional(it->first);
    }
  }

  return std::nullopt;
}

} // namespace aavm::parser::keyword

#endif
