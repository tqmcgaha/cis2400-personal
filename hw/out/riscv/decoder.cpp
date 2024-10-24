#include "./decoder.hpp"

void decode_r_type(Instruction& instr); 
void decode_i_type(Instruction& instr); 
void decode_j_type(Instruction& instr); 
void decode_s_type(Instruction& instr); 
void decode_b_type(Instruction& instr); 

Instruction decode_instruction(uint32_t instruction) {
    Instruction instr; 
    // get da opcode
    instr.opcode = instruction & 0x7F;
    
    // get common fields
    instr.rd = (instruction >> 7) & 0x1F;
    instr.rs1 = (instruction >> 15) & 0x1F;
    instr.rs2 = (instruction >> 20) & 0x1F;
    instr.funct3 = (instruction >> 12) & 0x7;
    instr.funct7 = (instruction >> 25) & 0x7F;
    instr.encoding = instruction;

    switch(instr.opcode) {
        case 0x33: // r type
            instr.type = InstructionType::R_TYPE;
            decode_r_type(instr);
            break;
        case 0x13 | 0x03: // i type  
            instr.type = InstructionType::I_TYPE;
            decode_i_type(instr);            
            break;
        case 0x23: // s type
            instr.type = InstructionType::S_TYPE;
            decode_s_type(instr);
            break;
        case 0x63: // b type
            instr.type = InstructionType::B_TYPE;
            decode_b_type(instr);
            break;        
        }
      return instr; 
}

void decode_r_type(Instruction& instr) {
    instr.uimm = 0U;
    instr.imm = 0;

    switch(instr.funct3) {
        case 0x0:
            instr.mnemonic = (instr.funct7 == 0) ? "add" : "sub"; //is this too error prone?? 
            break;
        case 0x1:
            instr.mnemonic = "sll";
            break;
        case 0x2:
            instr.mnemonic = "slt";
            break;
        case 0x3: 
            instr.mnemonic = "sltu"; 
            break; 
        case 0x4: 
            instr.mnemonic = "xor"; 
            break;
        case 0x5: 
            instr.mnemonic = (instr.funct7 == 0) ? "slr" : "sra"; //is this too error prone?? 
            break;
        case 0x6: 
            instr.mnemonic = "or"; 
            break;
        case 0x7: 
            instr.mnemonic = "and"; 
            break;
        default:
            instr.mnemonic = "error_r";
    }
}


void decode_i_type(Instruction& instr) {
    instr.uimm = (instr.encoding >> 20) & 0xFFF;
    instr.imm = static_cast<int32_t>(instr.encoding) >> 20;

    switch(instr.funct3) {
        case 0x0:
            instr.mnemonic = "addi";
            break;
        case 0x1:
            instr.mnemonic = "slli";
            instr.imm &= 0x1F;
            instr.uimm &= 0x1F;
            break;
        case 0x2:
            instr.mnemonic = "slti";
            break;
        case 0x3:
            instr.mnemonic = "sltiu";
            break;
        case 0x4:
            instr.mnemonic = "xori";
            break;
        case 0x5:
            instr.mnemonic = (instr.funct7 == 0) ? "srli" : "srai";
            instr.imm &= 0x1F;
            instr.uimm &= 0x1F;
            break;
        case 0x6:
            instr.mnemonic = "ori";
            break;
        case 0x7:
            instr.mnemonic = "andi";
            break;
        default:
            instr.mnemonic = "unknown_i";
    }
}

void decode_s_type(Instruction& instr) {
    int32_t mask = 0x001F;
    mask = ~mask;

    instr.imm = static_cast<int32_t>(instr.encoding) >> 25;
    instr.imm &= mask;
    instr.imm |= (instr.encoding >> 7) & 0x1F;

    switch(instr.funct3) {
        case 0x0:
            instr.mnemonic = "sb";
            break;
        case 0x1:
            instr.mnemonic = "sh";
            break;
        case 0x2:
            instr.mnemonic = "sw";
            break;
        default:
            instr.mnemonic = "unknown_s";
    }
}

void decode_b_type(Instruction& instr) {
    int32_t bit12 = (instr.encoding >> 31) & 0x01;
    int32_t bit10_to_5 = (instr.encoding >> 25) & 0x3F;
    int32_t bit4_to_1 = (instr.encoding >> 8) & 0x0F;
    int32_t bit11 = (instr.encoding >> 7) & 0x01;
    
    int32_t imm = (bit12 << 12) | (bit11 << 11) | (bit10_to_5 << 5) | (bit4_to_1 << 1);
    imm = ((imm << 19) >> 19);
    instr.imm = imm;

    switch(instr.funct3) {
        case 0x0:
            instr.mnemonic = "beq";
            break;
        case 0x1:
            instr.mnemonic = "bne";
            break;
        case 0x4:
            instr.mnemonic = "blt";
            break;
        case 0x5:
            instr.mnemonic = "bge";
            break;
        case 0x6:
            instr.mnemonic = "bltu";
            break;
        case 0x7:
            instr.mnemonic = "bgeu";
            break;
        default:
            instr.mnemonic = "unknown_b";
    }
}

void decode_u_type(Instruction& instr) {
    instr.imm = instr.encoding & 0xFFFFF000;

    switch(instr.opcode) {
        case 0x37:
            instr.mnemonic = "lui";
            break;
        case 0x17:
            instr.mnemonic = "auipc";
            break;
        default:
            instr.mnemonic = "unknown_u";
    }
}

void decode_j_type(Instruction& instr) {
    int32_t bit20 = (instr.encoding >> 31) & 0x01;
    int32_t bit10_to_1 = (instr.encoding >> 21) & 0x3FF;
    int32_t bit11 = (instr.encoding >> 20) & 0x01;
    int32_t bit19_to_12 = (instr.encoding >> 12) & 0xFF;

    int32_t imm = (bit20 << 20) | (bit19_to_12 << 12) | (bit11 << 11) | (bit10_to_1 << 1);
    imm = (imm << 11) >> 11;

    instr.imm = imm;

    instr.mnemonic = "jal";
}
