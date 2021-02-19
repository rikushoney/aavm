#ifndef AAVM_TEXTBUFFER_H_
#define AAVM_TEXTBUFFER_H_

#include <array>
#include <string_view>
#include <vector>

namespace aavm {

namespace detail_ {

template <typename Istream> auto read_stream_into_vector_(Istream &instream) {
  constexpr auto chunk_size = sizeof(unsigned int) * 32u;
  using char_type = typename Istream::char_type;
  auto vec = std::vector<char_type>();
  auto buffer = std::array<char_type, chunk_size>();

  const auto buffer_start = buffer.begin();

  while (instream) {
    instream.read(buffer.data(), chunk_size);
    const auto chars_read = instream.gcount();
    const auto read_end = std::next(buffer_start, chars_read);
    vec.insert(vec.end(), buffer_start, read_end);
  }

  return vec;
}

} // namespace detail_

template <typename CharT> class Textbuffer {
public:
  using value_type = CharT;
  using container_type = std::vector<value_type>;
  using size_type = typename container_type::size_type;
  using iterator = typename container_type::const_iterator;

  Textbuffer() = delete;
  template <typename Istream>
  Textbuffer(Istream &instream)
      : container_{detail_::read_stream_into_vector_(instream)} {
    static_assert(sizeof(value_type) == sizeof(typename Istream::char_type));
  }
  Textbuffer(std::string_view source)
      : container_{source.begin(), source.end()} {}

  Textbuffer &operator=(std::string_view source) {
    container_ = container_type{source.begin(), source.end()};
  }

  auto begin() const { return container_.begin(); }

  auto end() const { return container_.end(); }

  auto view(iterator first, iterator last) const {
    return std::string_view(&(*first), static_cast<size_type>(last - first));
  }

  auto size() const { return container_.size(); }

  template <typename Ostream> auto dump(Ostream &outstream) const {
    static_assert(sizeof(value_type) == sizeof(typename Ostream::char_type));
    outstream.write(container_.data(), container_.size());
  }

private:
  const container_type container_;
};

using Charbuffer = Textbuffer<char>;

namespace textbuffer_literals {

inline Charbuffer operator""_tb(const char *str, std::size_t len) {
  return {std::string_view{str, len}};
}

} // namespace textbuffer_literals

} // namespace aavm

#endif
