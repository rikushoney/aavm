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
  const auto &instruction = *ir::as_arithmetic(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Add);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.rn(), ir::Register::R1);
  EXPECT_TRUE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseShiftInstruction) {
  const auto parsed = Parser{"asr r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_move(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Mov);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_FALSE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().rm().rm(), ir::Register::R1);
  EXPECT_EQ(instruction.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_TRUE(instruction.src2().rm().immediate());
  EXPECT_EQ(instruction.src2().rm().shamt5(), 1u);
}

TEST(ParserTest, CanParseMultiplyInstruction) {
  const auto parsed = Parser{"mul r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_multiply(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Mul);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.rm(), ir::Register::R1);
  EXPECT_EQ(instruction.rs(), ir::Register::R2);
}

TEST(ParserTest, CanParseDivideInstruction) {
  const auto parsed = Parser{"sdiv r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_divide(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Sdiv);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.rn(), ir::Register::R1);
  EXPECT_EQ(instruction.rm(), ir::Register::R2);
}

TEST(ParserTest, CanParseMoveInstruction) {
  const auto parsed = Parser{"mov r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_move(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Mov);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_TRUE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseComparisonInstruction) {
  const auto parsed = Parser{"cmp r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_comparison(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Cmp);
  EXPECT_TRUE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rn(), ir::Register::R0);
  EXPECT_TRUE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseBitfieldInstruction) {
  const auto parsed = Parser{"bfc r0, #1, #2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_bitfield(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Bfc);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.lsb(), 1u);
  EXPECT_EQ(instruction.width(), 2u);
}

TEST(ParserTest, CanParseReverseInstruction) {
  const auto parsed = Parser{"rbit r0, r1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_reverse(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Rbit);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.rm(), ir::Register::R1);
}

TEST(ParserTest, CanParseBranchInstruction) {
  const auto parsed = Parser{"b loop"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_branch(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::B);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
}

TEST(ParserTest, CanParseSingleMemoryInstruction) {
  const auto parsed = Parser{"ldr r0, [r1, #1]"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_single_memory(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Ldr);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rd(), ir::Register::R0);
  EXPECT_EQ(instruction.rn(), ir::Register::R1);
  EXPECT_TRUE(std::holds_alternative<ir::Operand2>(instruction.source()));
  EXPECT_TRUE(std::get<ir::Operand2>(instruction.source()).immediate());
  EXPECT_EQ(std::get<ir::Operand2>(instruction.source()).imm12(), 1u);
  EXPECT_EQ(instruction.indexmode(),
            ir::SingleMemoryInstruction::IndexMode::Offset);
  EXPECT_FALSE(instruction.subtract());
}

TEST(ParserTest, CanParseBlockMemoryInstruction) {
  const auto parsed = Parser{"ldm r0, {r1, r2}"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_block_memory(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Ldm);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
  EXPECT_EQ(instruction.rn(), ir::Register::R0);
  EXPECT_FALSE(instruction.writeback());
  EXPECT_EQ(instruction.register_list().size(), 2);
  EXPECT_EQ(instruction.register_list()[0], ir::Register::R1);
  EXPECT_EQ(instruction.register_list()[1], ir::Register::R2);
}

TEST(ParserTest, CanParseConditionSuffix) {
  const auto parsed = Parser{"addeq r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instruction = *as_arithmetic(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Add);
  EXPECT_FALSE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::EQ);
}

TEST(ParserTest, CanParseUpdateFlag) {
  const auto parsed = Parser{"adds r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instruction = *as_arithmetic(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Add);
  EXPECT_TRUE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::AL);
}

TEST(ParserTest, CanParseUpdateFlagAndConditionSuffix) {
  const auto parsed = Parser{"addseq r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instruction = *as_arithmetic(*parsed.get());
  EXPECT_EQ(instruction.operation(), ir::Instruction::Add);
  EXPECT_TRUE(instruction.updatesflags());
  EXPECT_EQ(instruction.condition(), ir::Condition::EQ);
}

TEST(ParserTest, CanParseOperand2Immediate) {
  const auto parsed = Parser{"add r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_arithmetic(*parsed.get());
  EXPECT_TRUE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseOperand2Register) {
  const auto parsed = Parser{"add r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_arithmetic(*parsed.get());
  EXPECT_FALSE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instruction.src2().rm().sh(), ir::Instruction::Lsl);
  EXPECT_TRUE(instruction.src2().rm().immediate());
  EXPECT_EQ(instruction.src2().rm().shamt5(), 0u);
}

TEST(ParserTest, CanParseOperand2RegisterImmediateShift) {
  const auto parsed = Parser{"add r0, r1, r2, ASR #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_arithmetic(*parsed.get());
  EXPECT_FALSE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instruction.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_TRUE(instruction.src2().rm().immediate());
  EXPECT_EQ(instruction.src2().rm().shamt5(), 1u);
}

TEST(ParserTest, CanParseOperand2RegisterRegisterShift) {
  const auto parsed = Parser{"add r0, r1, r2, ASR r3"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instruction = *ir::as_arithmetic(*parsed.get());
  EXPECT_FALSE(instruction.src2().immediate());
  EXPECT_EQ(instruction.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instruction.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_FALSE(instruction.src2().rm().immediate());
  EXPECT_EQ(instruction.src2().rm().rs(), ir::Register::R3);
}
