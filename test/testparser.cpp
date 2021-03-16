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
  const auto &instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Add);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.rn(), ir::Register::R1);
  EXPECT_TRUE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseShiftInstruction) {
  const auto parsed = Parser{"asr r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::MoveInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Mov);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_FALSE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().rm().rm(), ir::Register::R1);
  EXPECT_EQ(instr.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_TRUE(instr.src2().rm().immediate());
  EXPECT_EQ(instr.src2().rm().shamt5(), 1u);
}

TEST(ParserTest, CanParseMultiplyInstruction) {
  const auto parsed = Parser{"mul r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::MultiplyInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Mul);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.rm(), ir::Register::R1);
  EXPECT_EQ(instr.rs(), ir::Register::R2);
}

TEST(ParserTest, CanParseDivideInstruction) {
  const auto parsed = Parser{"sdiv r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::DivideInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Sdiv);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.rn(), ir::Register::R1);
  EXPECT_EQ(instr.rm(), ir::Register::R2);
}

TEST(ParserTest, CanParseMoveInstruction) {
  const auto parsed = Parser{"mov r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::MoveInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Mov);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_TRUE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseComparisonInstruction) {
  const auto parsed = Parser{"cmp r0, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ComparisonInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Cmp);
  EXPECT_TRUE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rn(), ir::Register::R0);
  EXPECT_TRUE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseBitfieldInstruction) {
  const auto parsed = Parser{"bfc r0, #1, #2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::BitfieldInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Bfc);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.lsb(), 1u);
  EXPECT_EQ(instr.width(), 2u);
}

TEST(ParserTest, CanParseReverseInstruction) {
  const auto parsed = Parser{"rbit r0, r1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ReverseInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Rbit);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.rm(), ir::Register::R1);
}

TEST(ParserTest, CanParseBranchInstruction) {
  const auto parsed = Parser{"b loop"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::BranchInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::B);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
}

TEST(ParserTest, CanParseSingleMemoryInstruction) {
  const auto parsed = Parser{"ldr r0, [r1, #1]"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::SingleMemoryInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Ldr);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rd(), ir::Register::R0);
  EXPECT_EQ(instr.rn(), ir::Register::R1);
  EXPECT_TRUE(std::holds_alternative<ir::Operand2>(instr.source()));
  EXPECT_TRUE(std::get<ir::Operand2>(instr.source()).immediate());
  EXPECT_EQ(std::get<ir::Operand2>(instr.source()).imm12(), 1u);
  EXPECT_EQ(instr.indexmode(), ir::SingleMemoryInstruction::IndexMode::Offset);
  EXPECT_FALSE(instr.subtract());
}

TEST(ParserTest, CanParseBlockMemoryInstruction) {
  const auto parsed = Parser{"ldm r0, {r1, r2}"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::BlockMemoryInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Ldm);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
  EXPECT_EQ(instr.rn(), ir::Register::R0);
  EXPECT_FALSE(instr.writeback());
  EXPECT_EQ(instr.register_list().size(), 2);
  EXPECT_EQ(instr.register_list()[0], ir::Register::R1);
  EXPECT_EQ(instr.register_list()[1], ir::Register::R2);
}

TEST(ParserTest, CanParseConditionSuffix) {
  const auto parsed = Parser{"addeq r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Add);
  EXPECT_FALSE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::EQ);
}

TEST(ParserTest, CanParseUpdateFlag) {
  const auto parsed = Parser{"adds r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Add);
  EXPECT_TRUE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::AL);
}

TEST(ParserTest, CanParseUpdateFlagAndConditionSuffix) {
  const auto parsed = Parser{"addseq r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_EQ(instr.operation(), ir::Instruction::Add);
  EXPECT_TRUE(instr.updatesflags());
  EXPECT_EQ(instr.condition(), ir::Condition::EQ);
}

TEST(ParserTest, CanParseOperand2Immediate) {
  const auto parsed = Parser{"add r0, r1, #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_TRUE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().imm12(), 1u);
}

TEST(ParserTest, CanParseOperand2Register) {
  const auto parsed = Parser{"add r0, r1, r2"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_FALSE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instr.src2().rm().sh(), ir::Instruction::Lsl);
  EXPECT_TRUE(instr.src2().rm().immediate());
  EXPECT_EQ(instr.src2().rm().shamt5(), 0u);
}

TEST(ParserTest, CanParseOperand2RegisterImmediateShift) {
  const auto parsed = Parser{"add r0, r1, r2, ASR #1"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_FALSE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instr.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_TRUE(instr.src2().rm().immediate());
  EXPECT_EQ(instr.src2().rm().shamt5(), 1u);
}

TEST(ParserTest, CanParseOperand2RegisterRegisterShift) {
  const auto parsed = Parser{"add r0, r1, r2, ASR r3"_tb}.parse_instruction();
  ASSERT_NE(parsed.get(), nullptr);
  const auto &instr = *ir::cast<ir::ArithmeticInstruction>(parsed.get());
  EXPECT_FALSE(instr.src2().immediate());
  EXPECT_EQ(instr.src2().rm().rm(), ir::Register::R2);
  EXPECT_EQ(instr.src2().rm().sh(), ir::Instruction::Asr);
  EXPECT_FALSE(instr.src2().rm().immediate());
  EXPECT_EQ(instr.src2().rm().rs(), ir::Register::R3);
}
