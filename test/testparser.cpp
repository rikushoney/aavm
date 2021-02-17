#include "instruction.h"
#include "instructions.h"
#include "operand2.h"
#include "parser.h"
#include "register.h"
#include "textbuffer.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <variant>

using namespace aavm;
using namespace std::string_view_literals;

TEST(ParserTest, CanParseArithmeticInstruction) {
  const auto source = Charbuffer{"add r0, r1, #100"sv};
  auto parser = parser::Parser{source};
  const auto parsed = parser.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Add);
  EXPECT_FALSE(parsed->updatesflags());
  const auto instruction =
      static_cast<ir::ArithmeticInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.rn, ir::Register::R1);
  EXPECT_TRUE(instruction.src2.immediate);
  EXPECT_EQ(instruction.src2.imm12, 100u);
}
