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
using namespace std::string_view_literals;

TEST(ParserTest, CanParseArithmeticInstruction) {
  const auto parsed = Parser{"add r0, r1, #1"_tb}.parse_instruction();
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
  const auto parsed = Parser{"asr r0, r1, #1"_tb}.parse_instruction();
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

TEST(ParserTest, CanParseMultiplyInstruction) {
  const auto parsed = Parser{"mul r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Mul);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction =
      static_cast<ir::MultiplyInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.rm, ir::Register::R1);
  EXPECT_EQ(instruction.rs, ir::Register::R2);
}

TEST(ParserTest, CanParseDivideInstruction) {
  const auto parsed = Parser{"sdiv r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Sdiv);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction = static_cast<ir::DivideInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.rn, ir::Register::R1);
  EXPECT_EQ(instruction.rm, ir::Register::R2);
}

TEST(ParserTest, CanParseMoveInstruction) {
  const auto parsed = Parser{"mov r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Mov);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction = static_cast<ir::MoveInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_TRUE(instruction.src2.immediate);
  EXPECT_EQ(instruction.src2.imm12, 1u);
}

TEST(ParserTest, CanParseComparisonInstruction) {
  const auto parsed = Parser{"cmp r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Cmp);
  EXPECT_TRUE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction =
      static_cast<ir::ComparisonInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rn, ir::Register::R0);
  EXPECT_TRUE(instruction.src2.immediate);
  EXPECT_EQ(instruction.src2.imm12, 1u);
}

TEST(ParserTest, CanParseBitfieldInstruction) {
  const auto parsed = Parser{"bfc r0, #1, #2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Bfc);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction =
      static_cast<ir::BitfieldInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.lsb, 1u);
  EXPECT_EQ(instruction.width, 2u);
}

TEST(ParserTest, CanParseReverseInstruction) {
  const auto parsed = Parser{"rbit r0, r1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::Rbit);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
  const auto instruction = static_cast<ir::ReverseInstruction &>(*parsed.get());
  EXPECT_EQ(instruction.rd, ir::Register::R0);
  EXPECT_EQ(instruction.rm, ir::Register::R1);
}

TEST(ParserTest, CanParseBranchInstruction) {
  const auto parsed = Parser{"b loop"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  EXPECT_EQ(parsed->operation(), ir::Instruction::B);
  EXPECT_FALSE(parsed->updatesflags());
  EXPECT_EQ(parsed->condition(), ir::condition::AL);
}
