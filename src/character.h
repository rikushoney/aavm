#ifndef AAVM_CHARACTER_H_
#define AAVM_CHARACTER_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace aavm {

namespace detail_ {

constexpr auto lower_mask_ = std::byte{0b0000'0001};
constexpr auto upper_mask_ = std::byte{0b0000'0010};
constexpr auto alpha_mask_ = std::byte{0b0000'0100};
constexpr auto digit_mask_ = std::byte{0b0000'1000};
constexpr auto alnum_mask_ = std::byte{0b0001'0000};
constexpr auto xdigit_mask_ = std::byte{0b0010'0000};
constexpr auto punct_mask_ = std::byte{0b0100'0000};
constexpr auto space_mask_ = std::byte{0b1000'0000};
constexpr auto char_count_ = 0x80;

constexpr auto charclass_table_ = [] {
  auto table = std::array<std::byte, char_count_>();

  for (auto i = 0; i < char_count_; ++i) {
    if (i >= 0x61 && i <= 0x7A) {
      table[i] |= lower_mask_;
      table[i] |= alpha_mask_;
      table[i] |= alnum_mask_;

      if (i <= 0x66) {
        table[i] |= xdigit_mask_;
      }
    }

    if (i >= 0x41 && i <= 0x5A) {
      table[i] |= upper_mask_;
      table[i] |= alpha_mask_;
      table[i] |= alnum_mask_;

      if (i <= 0x46) {
        table[i] |= xdigit_mask_;
      }
    }

    if (i >= 0x30 && i <= 0x39) {
      table[i] |= digit_mask_;
      table[i] |= xdigit_mask_;
      table[i] |= alnum_mask_;
    }

    if ((i >= 0x21 && i <= 0x2F) || (i >= 0x3A && i <= 0x40) ||
        (i >= 0x5B && i <= 0x60) || (i >= 0x7B && i <= 0x7E)) {
      table[i] |= punct_mask_;
    }

    if ((i >= 0x09 && i <= 0x0D) || i == 0x20) {
      table[i] |= space_mask_;
    }
  }

  return table;
}();

constexpr auto check_mask_(std::byte ch, std::byte mask) {
  return (ch & mask) == mask;
}

} // namespace detail_

constexpr auto is_ascii(int ch) { return ch >= 0 && ch <= 0x7F; }

constexpr auto is_lower(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::lower_mask_);
}

constexpr auto is_upper(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::upper_mask_);
}

constexpr auto is_alpha(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::alpha_mask_);
}

constexpr auto is_digit(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::digit_mask_);
}

constexpr auto is_alnum(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::alnum_mask_);
}

constexpr auto is_xdigit(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::xdigit_mask_);
}

constexpr auto is_punct(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::punct_mask_);
}

constexpr auto is_space(int ch) {
  return detail_::check_mask_(detail_::charclass_table_[ch],
                              detail_::space_mask_);
}

namespace detail_ {

constexpr auto lowercaseconv_table_ = [] {
  auto table = std::array<int, char_count_>();

  for (auto i = 0; i < char_count_; ++i) {
    if (is_upper(i)) {
      table[i] = i - 0x41 + 0x61;
    } else {
      table[i] = i;
    }
  }

  return table;
}();

constexpr auto uppercaseconv_table_ = [] {
  auto table = std::array<int, char_count_>();

  for (auto i = 0; i < char_count_; ++i) {
    if (is_lower(i)) {
      table[i] = i - 0x61 + 0x41;
    } else {
      table[i] = i;
    }
  }

  return table;
}();

constexpr auto ctoiconv_table_ = [] {
  auto table = std::array<int, char_count_>();

  for (auto i = 0; i < char_count_; ++i) {
    if (is_xdigit(i)) {
      if (is_lower(i)) {
        table[i] = i - 0x61 + 0xA;
      } else if (is_upper(i)) {
        table[i] = i - 0x41 + 0xA;
      } else {
        table[i] = i - 0x30;
      }
    } else {
      table[i] = -1;
    }
  }

  return table;
}();

} // namespace detail_

constexpr auto to_lower(int ch) { return detail_::lowercaseconv_table_[ch]; }

inline auto to_lower(std::string_view str) {
  auto lowered = std::string(str.length(), '\0');
  std::transform(str.begin(), str.end(), lowered.begin(),
                 [](char ch) { return to_lower(ch); });
  return lowered;
}

constexpr auto to_upper(int ch) { return detail_::uppercaseconv_table_[ch]; }

inline auto to_upper(std::string_view str) {
  auto uppered = std::string(str.length(), '\0');
  std::transform(str.begin(), str.end(), uppered.begin(),
                 [](char ch) { return to_upper(ch); });
  return uppered;
}

constexpr auto ctoi(int ch) { return detail_::ctoiconv_table_[ch]; }

} // namespace aavm

#endif
