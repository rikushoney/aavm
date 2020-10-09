#ifndef AAVM_CHARACTER_H
#define AAVM_CHARACTER_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

namespace aavm {

namespace detail {

constexpr std::uint8_t lowercase_mask = 1 << 0;
constexpr std::uint8_t uppercase_mask = 1 << 1;
constexpr std::uint8_t alpha_mask = 1 << 2;
constexpr std::uint8_t digit_mask = 1 << 3;
constexpr std::uint8_t alnum_mask = 1 << 4;
constexpr std::uint8_t xdigit_mask = 1 << 5;
constexpr std::uint8_t punct_mask = 1 << 6;
constexpr std::uint8_t space_mask = 1 << 7;
constexpr auto ASCII_MAX{std::numeric_limits<char>::max() + 1};

constexpr auto gen_charcat_table() {
  auto table = std::array<std::uint8_t, ASCII_MAX>{};

  // byte layout:
  /*
   *  >space
   *  | >punct
   *  | | >xdigit
   *  | | | >alnum
   *  | | | |
   *  7 6 5 4 3 2 1 0
   *          | | | |
   *          | | | >lowercase
   *          | | >uppercase
   *          | >alpha
   *          >digit
   */

  for (auto c = 0; c < ASCII_MAX; ++c) {
    if ((c >= 0x09 && c <= 0x0D) || c == 0x20) {
      table[c] |= space_mask;
    }

    if (c >= 0x61 && c <= 0x7A) {
      table[c] |= lowercase_mask;
      table[c] |= alpha_mask;
      table[c] |= alnum_mask;

      if (c <= 0x66) {
        table[c] |= xdigit_mask;
      }
    }

    if (c >= 0x41 && c <= 0x5A) {
      table[c] |= uppercase_mask;
      table[c] |= alpha_mask;
      table[c] |= alnum_mask;

      if (c <= 0x46) {
        table[c] |= xdigit_mask;
      }
    }

    if (c >= 0x30 && c <= 0x39) {
      table[c] |= digit_mask;
      table[c] |= alnum_mask;
      table[c] |= xdigit_mask;
    }

    if ((c >= 0x21 && c <= 0x2F) || (c >= 0x3A && c <= 0x40) ||
        (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7E)) {
      table[c] |= punct_mask;
    }
  }

  return table;
}

constexpr auto charcat{gen_charcat_table()};

constexpr auto belongs_to(char c, std::uint8_t mask) {
  return (charcat[static_cast<std::size_t>(c)] & mask) == mask;
}

} // namespace detail

constexpr auto is_ascii(char c) { return c >= 0; }

constexpr auto is_lowercase(char c) {
  return detail::belongs_to(c, detail::lowercase_mask);
}

constexpr auto is_uppercase(char c) {
  return detail::belongs_to(c, detail::uppercase_mask);
}

constexpr auto is_alpha(char c) {
  return detail::belongs_to(c, detail::alpha_mask);
}

constexpr auto is_digit(char c) {
  return detail::belongs_to(c, detail::digit_mask);
}

constexpr auto is_alnum(char c) {
  return detail::belongs_to(c, detail::alnum_mask);
}

constexpr auto is_xdigit(char c) {
  return detail::belongs_to(c, detail::xdigit_mask);
}

constexpr auto is_punct(char c) {
  return detail::belongs_to(c, detail::punct_mask);
}

constexpr auto is_space(char c) {
  return detail::belongs_to(c, detail::space_mask);
}

namespace detail {

constexpr auto gen_lowerconv_table() {
  auto table = std::array<char, ASCII_MAX>{};

  for (auto c = 0; c < ASCII_MAX; ++c) {
    if (is_uppercase(c)) {
      table[c] = c - 0x41 + 0x61;
    } else {
      table[c] = c;
    }
  }

  return table;
}

constexpr auto gen_upperconv_table() {
  auto table = std::array<char, ASCII_MAX>{};

  for (auto c = 0; c < ASCII_MAX; ++c) {
    if (is_lowercase(c)) {
      table[c] = c - 0x61 + 0x41;
    } else {
      table[c] = c;
    }
  }

  return table;
}

constexpr auto gen_ctoiconv_table() {
  auto table = std::array<int, ASCII_MAX>{};

  for (auto c = 0; c < ASCII_MAX; ++c) {
    if (is_xdigit(c)) {
      if (is_lowercase(c)) {
        table[c] = static_cast<int>(c - 0x61 + 0xA);
      } else if (is_uppercase(c)) {
        table[c] = static_cast<int>(c - 0x41 + 0xA);
      } else {
        table[c] = static_cast<int>(c - 0x30 + 0x0);
      }
    } else {
      table[c] = -1;
    }
  }

  return table;
}

constexpr auto lowercaseconv{gen_lowerconv_table()};
constexpr auto uppercaseconv{gen_upperconv_table()};
constexpr auto ctoiconv{gen_ctoiconv_table()};

} // namespace detail

constexpr auto to_lower(char c) { return detail::lowercaseconv[c]; }

constexpr auto to_upper(char c) { return detail::uppercaseconv[c]; }

constexpr auto ctoi(char c) { return detail::ctoiconv[c]; }

inline auto to_lower(std::string_view str) -> std::string {
  auto result = std::string{str};
  std::transform(result.begin(), result.end(), result.begin(),
                 [](char c) { return to_lower(c); });
  return result;
}

inline auto to_upper(std::string_view str) -> std::string {
  auto result = std::string{str};
  std::transform(result.begin(), result.end(), result.begin(),
                 [](char c) { return to_upper(c); });
  return result;
}

constexpr auto starts_with(std::string_view str, std::string_view prefix) {
  return str.substr(0, prefix.length()) == prefix;
}

constexpr auto ends_with(std::string_view str, std::string_view suffix) {
  return str.length() >= suffix.length() &&
         str.compare(str.length() - suffix.length(), std::string_view::npos,
                     suffix) == 0;
}

} // namespace aavm

#endif
