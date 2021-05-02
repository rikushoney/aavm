#ifndef AAVM_KEYWORD_H_
#define AAVM_KEYWORD_H_

#include "stl_array.h"
#include "token.h"
#include <string_view>

namespace aavm::keyword {

namespace detail_ {

using namespace aavm::token;
using namespace std::string_view_literals;

constexpr auto keyword_lookup_matrix_ =
    stl::make_array<std::pair<std::string_view, Kind>>(
        {{"eq"sv, kw_eq},       {"ne"sv, kw_ne},       {"cs"sv, kw_cs},
         {"hs"sv, kw_hs},       {"cc"sv, kw_cc},       {"lo"sv, kw_lo},
         {"mi"sv, kw_mi},       {"pl"sv, kw_pl},       {"vs"sv, kw_vs},
         {"vc"sv, kw_vc},       {"hi"sv, kw_hi},       {"ls"sv, kw_ls},
         {"ge"sv, kw_ge},       {"lt"sv, kw_lt},       {"gt"sv, kw_gt},
         {"le"sv, kw_le},       {"al"sv, kw_al},       {"r0"sv, kw_r0},
         {"r1"sv, kw_r1},       {"r2"sv, kw_r2},       {"r3"sv, kw_r3},
         {"r4"sv, kw_r4},       {"r5"sv, kw_r5},       {"r6"sv, kw_r6},
         {"r7"sv, kw_r7},       {"r8"sv, kw_r8},       {"r9"sv, kw_r9},
         {"r10"sv, kw_r10},     {"r11"sv, kw_r11},     {"r12"sv, kw_r12},
         {"r13"sv, kw_r13},     {"r14"sv, kw_r14},     {"r15"sv, kw_r15},
         {"sp"sv, kw_sp},       {"lr"sv, kw_lr},       {"pc"sv, kw_pc},
         {"nop"sv, kw_nop},     {"add"sv, kw_add},     {"adc"sv, kw_adc},
         {"sub"sv, kw_sub},     {"sbc"sv, kw_sbc},     {"rsb"sv, kw_rsb},
         {"rsc"sv, kw_rsc},     {"and"sv, kw_and},     {"eor"sv, kw_eor},
         {"orr"sv, kw_orr},     {"bic"sv, kw_bic},     {"adr"sv, kw_adr},
         {"asr"sv, kw_asr},     {"lsl"sv, kw_lsl},     {"lsr"sv, kw_lsr},
         {"ror"sv, kw_ror},     {"rrx"sv, kw_rrx},     {"mul"sv, kw_mul},
         {"mla"sv, kw_mla},     {"mls"sv, kw_mls},     {"umull"sv, kw_umull},
         {"umlal"sv, kw_umlal}, {"smull"sv, kw_smull}, {"smlal"sv, kw_smlal},
         {"sdiv"sv, kw_sdiv},   {"udiv"sv, kw_udiv},   {"mov"sv, kw_mov},
         {"mvn"sv, kw_mvn},     {"movt"sv, kw_movt},   {"movw"sv, kw_movw},
         {"cmp"sv, kw_cmp},     {"cmn"sv, kw_cmn},     {"tst"sv, kw_tst},
         {"teq"sv, kw_teq},     {"bfc"sv, kw_bfc},     {"bfi"sv, kw_bfi},
         {"sbfx"sv, kw_sbfx},   {"ubfx"sv, kw_ubfx},   {"rbit"sv, kw_rbit},
         {"rev"sv, kw_rev},     {"rev16"sv, kw_rev16}, {"revsh"sv, kw_revsh},
         {"b"sv, kw_b},         {"bl"sv, kw_bl},       {"bx"sv, kw_bx},
         {"cbz"sv, kw_cbz},     {"cbnz"sv, kw_cbnz},   {"ldr"sv, kw_ldr},
         {"ldrb"sv, kw_ldrb},   {"ldrsb"sv, kw_ldrsb}, {"ldrh"sv, kw_ldrh},
         {"ldrsh"sv, kw_ldrsh}, {"str"sv, kw_str},     {"strb"sv, kw_strb},
         {"strh"sv, kw_strh},   {"ldm"sv, kw_ldm},     {"ldmia"sv, kw_ldmia},
         {"ldmib"sv, kw_ldmib}, {"ldmda"sv, kw_ldmda}, {"ldmdb"sv, kw_ldmdb},
         {"stm"sv, kw_stm},     {"stmia"sv, kw_stmia}, {"stmib"sv, kw_stmib},
         {"stmda"sv, kw_stmda}, {"stmdb"sv, kw_stmdb}, {"push"sv, kw_push},
         {"pop"sv, kw_pop}});

} // namespace detail_

constexpr auto none = detail_::keyword_lookup_matrix_.end();

template <typename Pred> constexpr auto find(Pred &&predicate) {
  for (auto it = detail_::keyword_lookup_matrix_.begin(); it != none; ++it) {
    if (predicate(it->first)) {
      return it;
    }
  }

  return none;
}

constexpr auto find(std::string_view keyword) {
  return find([keyword](const auto key) { return keyword == key; });
}

} // namespace aavm::keyword

#endif
