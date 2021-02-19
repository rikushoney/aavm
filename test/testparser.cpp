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
using namespace aavm::textbuffer_literals;

TEST(ParserTest, CanParseArithmeticInstruction) {
  const auto source = "add r0, r1, #1"_tb;
  auto parser = Parser{source};
  const auto parsed = parser.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Add);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction =
      static_cast<ir::ArithmeticInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.rn, ir::Register::R1);
  EXPECT_TRUE(instruction.src2.immediate);
  EXPECT_EQ(instruction.src2.imm12, 1u);
}

TEST(ParserTest, CanParseShiftInstruction) {
  const auto source = "asr r0, r1, #1"_tb;
  auto parser = Parser{source};
  const auto parsed = parser.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Mov);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction = static_cast<ir::MoveInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_FALSE(instruction.src2.immediate);
  EXPECT_EQ(instruction.src2.rm.rm, ir::Register::R1);
  EXPECT_EQ(instruction.src2.rm.sh, ir::Instruction::Asr);
  EXPECT_TRUE(instruction.src2.rm.immediate);
  EXPECT_EQ(instruction.src2.rm.shamt5, 1u);
}
