#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <cstdint>  // for sized integers
#include <string>   // for string

enum class InstructionType {
  R_TYPE,
  I_TYPE,
  S_TYPE,
  B_TYPE,
  U_TYPE,
  J_TYPE,
};

struct Instruction {
  InstructionType type;
  uint32_t opcode;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t funct3;
  uint32_t funct7;
  int32_t imm;
  uint32_t uimm;
  uint32_t encoding;
  std::string mnemonic;
};

Instruction decode_instruction(uint32_t instruction);

std::string instr_to_string(const Instruction& instr);

#endif  // DECODER_HPP_
