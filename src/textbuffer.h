#ifndef AAVM_TEXTBUFFER_H
#define AAVM_TEXTBUFFER_H

#include <array>
#include <cstddef>
#include <istream>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aavm {

namespace detail {

template <std::size_t Chunksize, typename Istream>
auto vector_from_stream(Istream &stream) {
  using char_type = typename Istream::char_type;

  auto vec = std::vector<char_type>{};
  auto buffer = std::array<char_type, Chunksize>{};
  const auto chars_start = buffer.begin();

  while (stream) {
    stream.read(buffer.data(), buffer.size());
    const auto chars_end = std::next(chars_start, stream.gcount());
    vec.insert(vec.end(), chars_start, chars_end);
  }

  return vec;
}

} // namespace detail

template <typename CharT> class Textbuffer {
private:
  // 4K buffer size
  static constexpr auto CHUNK_SIZE{0x1000};

public:
  using char_type = CharT;
  using size_type = std::size_t;
  using buffer_type = std::vector<char_type>;
  using iterator = typename buffer_type::const_iterator;

  Textbuffer() = delete;

  Textbuffer(Textbuffer &&buffer)
      : internal_buffer_{std::move(buffer.internal_buffer_)} {}

  auto operator=(Textbuffer &&buffer) -> Textbuffer & {
    internal_buffer_ = std::move(buffer.internal_buffer_);
    return *this;
  }

  template <typename Istream>
  Textbuffer(Istream &stream)
      : internal_buffer_{detail::vector_from_stream<CHUNK_SIZE>(stream)} {
    static_assert(sizeof(char_type) == sizeof(typename Istream::char_type));
  }

  auto begin() const { return internal_buffer_.begin(); }

  auto end() const { return internal_buffer_.end(); }

  template <typename Ostream> auto dump(Ostream &stream) const {
    static_assert(sizeof(char_type) == sizeof(typename Ostream::char_type));
    stream.write(internal_buffer_.data(), internal_buffer_.size());
  }

  auto view(iterator first, iterator last) const
      -> std::basic_string_view<char_type> {
    return {&(*first), static_cast<size_type>(last - first)};
  }

private:
  buffer_type internal_buffer_;
};

using Charbuffer = Textbuffer<char>;

} // namespace aavm

#endif
