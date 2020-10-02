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

  std::vector<char_type> vec{};
  std::array<char_type, Chunksize> buffer{};

  std::size_t chars_read{};

  while (stream) {
    stream.read(buffer.data(), buffer.size());
    chars_read = stream.gcount();
    vec.insert(vec.end(), buffer.begin(),
               std::next(buffer.begin(), chars_read));
  }

  return vec;
}

} // namespace detail

template <typename CharT> class Textbuffer {
  static constexpr std::size_t CHUNK_SIZE = 0x1000 * sizeof(CharT);

public:
  using char_type = CharT;
  using size_type = std::size_t;

  Textbuffer() = delete;

  template <typename Istream>
  Textbuffer(Istream &stream)
      : _internal_buffer{detail::vector_from_stream<CHUNK_SIZE>(stream)} {
    static_assert(sizeof(char_type) == sizeof(typename Istream::char_type));
  }

  template <typename Ostream> const void dump(Ostream &stream) const {
    static_assert(sizeof(char_type) == sizeof(typename Ostream::char_type));

    stream.write(_internal_buffer.data(), _internal_buffer.size());
  }

private:
  std::vector<char_type> _internal_buffer;
};

using Charbuffer = Textbuffer<char>;

} // namespace aavm

#endif
