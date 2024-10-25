#include "./loader.hpp"
#include "./decoder.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;


// 0x 00 00 00 00
// first page of memory purposefully left blank

// 0x 00 00 10 00  
// .text xC0DE

// 0x 00 08 00 00
// .data xDADA

static constexpr uint32_t MAGIC_WORD = 0xCAFEF00D;
static constexpr uint32_t INITIAL_CODE_OFFSET = 0x00001000;
static constexpr uint32_t INITIAL_DATA_OFFSET = 0x00080000;

static constexpr uint16_t CODE_INDICATOR = 0xC0DE;
static constexpr uint16_t DATA_INDICATOR = 0xDADA;
static constexpr uint16_t SYMB_INDICATOR = 0xC3B7;
static constexpr uint16_t FILE_INDICATOR = 0xF17E;
static constexpr uint16_t LINE_INDICATOR = 0x715E;

Instruction relocate_instruction(const Instruction& instruction, int32_t address);


int main(int argc, char* argv[]) {

  if (argc != 2) {
    cerr << "Usage: ./single_linker <risc-v asm file>" << endl;
  }


  string fname = argv[1];

  size_t len = fname.length();

  if (len <= 2 || fname[len - 2] != '.' || fname[len-1] != 's') {
    cerr << "Error: Expected a `.s` file for the asm file and it was not provided" << endl; 
    cerr << "Usage: ./single_linker <risc-v asm file>" << endl;
    return EXIT_FAILURE;
  }

  string obj_name = fname;
  obj_name[len-1] = 'o';

  array<const char*, 8> args = {"clang-15", "--target=riscv32", "-march=rv32i", "-nostdlib", "-Wnoincompatible-library-redeclaration", "-c", argv[1], nullptr};

  // run clang to get the object file

  pid_t pid = fork();

  if (pid == -1) {
    cerr << "CRITICAL SYSTEM ERROR" << endl;
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    execvp(args[0], args);
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(-1, &status, 0);
    if (!WIFEXITED(status)) {
      cerr << "CRITICAL SYSTEM ERROR" << endl;
      return EXIT_FAILURE;
    }

    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
      cerr << "Error: invalid risc-v assembly file ipnut" << endl;
      return EXIT_FAILURE;
    }
  }

  // file has been compiled to a .o, now time to read the .o and rebuild it into our format
  auto elf_parse = load_riscv32i_elf(obj_name);
  if (!elf_parse) {
    cerr << elf_parse.error() << endl;
    return EXIT_FAILURE;
  }
  riscv_elf elf = elf_parse.value();



  // handle relocations;
  uint32_t curr_code_offset = INITIAL_CODE_OFFSET;
  uint32_t curr_data_offset = INITIAL_DATA_OFFSET;
    
  // process symbols
  for (auto& symbol : elf.symtab) {
    symbol.index += curr_code_offset;

    // in multi-file will need to change non-global symbols to be unique
    // and record all global labels
  }

  for (auto& p : elf.relocations) {
    const auto& relos = p.second; 
    if (p.first != "text") {
      cerr << "unrecognized relocation for: " << p.first << " section" << endl;
      return EXIT_FAILURE;
    }

    for (auto& relo : relos) {
      symbol associated_symbol = elf.symtab.at(relo.symbol_index);
      Instruction to_relocate = elf.text.at(relo.addr);
      elf.text.at(relo.addr) = relocate_instruction(to_relocate, reinterpret_cast<int32_t>(associated_symbol.index) + relo.addend);
      if (relo.addend != 0) {
        cerr << "Report to travis: addend is not 0" << endl;
      }
    }
    
  }




  /////////////////////////////////////////////////////////////////////////////
  // Magic word, and opening the file
  /////////////////////////////////////////////////////////////////////////////

  // deletes old contents

  unique_ptr<FILE, deleter=fclose> out_obj {fopen(obj_name.data(), "w")};
  uint32_t temp32 = MAGIC_WORD;
  fwrite(&temp32, sizeof(temp32), 1, out_obj.get());
 

  /////////////////////////////////////////////////////////////////////////////
  // Writing xCADE
  /////////////////////////////////////////////////////////////////////////////

  uint16_t temp16 = CODE_INDICATOR;
  
  fwrite(&temp16, sizeof(temp16), 1, out_obj.get());

  for (const auto& instr : elf.text) {
    fwrite(&instr.encoding, sizeof(instr.encoding), 1, out_obj.get());
  }
}

static void r_relocate(Instruction& instr, int32_t address);
static void i_relocate(Instruction& instr, int32_t address);
static void s_relocate(Instruction& instr, int32_t address);
static void b_relocate(Instruction& instr, int32_t address);
static void u_relocate(Instruction& instr, int32_t address);
static void j_relocate(Instruction& instr, int32_t address);

Instruction relocate_instruction(const Instruction& instruction, int32_t address) {
  Instruction result = instruction;
  switch (instruction.type) {
    case InstructionType::R_TYPE:
      r_relocate(result, address);
      break;
    case InstructionType::I_TYPE:
      i_relocate(result, address);
      break;
    case InstructionType::S_TYPE:
      s_relocate(result, address);
      break;
    case InstructionType::B_TYPE:
      b_relocate(result, address);
      break;
    case InstructionType::U_TYPE:
      u_relocate(result, address);
      break;
    case InstructionType::J_TYPE:
      j_relocate(result, address);
      break;
    default:
      cerr << "WHAT THE FUCKKKKKKKKKKKK ILLEGAL INSTRUCTION TYPE IN RELOCATION" << endl;
      exit(EXIT_FAILURE);
  }
  return result;
}

static void r_relocate(Instruction& instr, int32_t address) {
  cerr << "POKE TRAVIS: WHY ARE WE RELOCATING AN R TYPE?????????" << endl;
  exit(EXIT_FAILURE);
}

static void i_relocate(Instruction& instr, int32_t address) {
  uint32_t pattern = address << 20;
  instr.encoding |= pattern;
}

static void s_relocate(Instruction& instr, int32_t address) {
  address = address & 0xFFF;
  uint32_t bottom_five = address & 0x1F;
  uint32_t upper_seven = (address >> 5) & 0x7f;
  uint32_t pattern = (upper_seven << 25) | (bottom_five << 7);
  instr.encoding |= pattern;
}

static void b_relocate(Instruction& instr, int32_t address) {
  uint32_t bit12 = (address >> 12) & 0x1;
  uint32_t bit11 = (address >>  11) & 0x1;
  uint32_t bit10_to_5 = (address >> 5) & 0x3F;
  uint32_t bit4_to_1 = (address >> 1) & 0x0F;

  uint32_t pattern = (bit12 << 31) | (bit10_to_5 << 25) | (bit4_to_1 << 8) | (bit11 << 7);
  instr.encoding != pattern;
}

static void u_relocate(Instruction& instr, int32_t address) {
  uint32_t pattern = reinterpret_cast<uint32_t>(address & 0xFFFFF000);
  instr.encoding |= pattern;
}

static void j_relocate(Instruction& instr, int32_t address) {
  uint32_t bit20 = (address >> 20) & 0x1;
  uint32_t bit10_to_1 = (address >> 1) & 0x3FF;
  uint32_t bit11 = (address >> 11) & 0x1;
  uint32_t bit19_to_12 = (address >> 12) & 0xFF;
  
  uint32_t pattern = (bit20 << 31) | (bit10_to_1 << 21) | (bit11 << 20) | (bit19_to_12 << 12);

  instr.encoding |= pattern;
}
