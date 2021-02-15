#include "textbuffer.h"
#include "gtest/gtest.h"
#include <sstream>
#include <string>
#include <string_view>

using namespace aavm;
using namespace std::string_view_literals;

TEST(TextbufferTest, CanConstructFromStream) {
  const auto text = std::string{"some 😄 text"};
  auto stream = std::stringstream{text};
  const auto buffer = Charbuffer{stream};
  EXPECT_EQ(text.length(), buffer.size());
  auto sink = std::stringstream{};
  buffer.dump(sink);
  EXPECT_EQ(text, sink.str());
}

TEST(TextbufferTest, CanConstructFromString) {
  const auto text = "some 😃 other text"sv;
  const auto buffer = Charbuffer{text};
  EXPECT_EQ(text.length(), buffer.size());
  auto sink = std::stringstream{};
  buffer.dump(sink);
  EXPECT_EQ(text, sink.str());
}
