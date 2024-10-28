#include "./decoder.hpp"
#include "./elf_loader.hpp"
#include "./riscv_constants.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

static Instruction relocate_instruction(const Instruction& instruction,
                                 int32_t instr_address,
                                 int32_t label_address);

static bool write_data_section(const section& section, FILE* elf_file, uint32_t& data_offset);

struct fcloser {
  void operator()(FILE* f) const { fclose(f); }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./single_linker <risc-v asm file>" << endl;
    return EXIT_FAILURE;
  }

  string fname = argv[1];

  size_t len = fname.length();

  if (len <= 2 || fname[len - 2] != '.' || fname[len - 1] != 's') {
    cerr << "Error: Expected a `.s` file for the asm file and it was not "
            "provided"
         << endl;
    cerr << "Usage: ./single_linker <risc-v asm file>" << endl;
    return EXIT_FAILURE;
  }

  string obj_name = fname;
  obj_name[len - 1] = 'o';

  char cc[] = "clang-15";
  char target[] = "--target=riscv32";
  char march[] = "-march=rv32i";
  char nostdlib[] = "-nostdlib";
  char nowarn[] = "-Wnoincompatible-library-redeclaration";
  char dashc[] = "-c";

  array<char*, 8> args = {cc,     target, march,   nostdlib,
                          nowarn, dashc,  argv[1], nullptr};

  // run clang to get the object file

  pid_t pid = fork();

  if (pid == -1) {
    cerr << "CRITICAL SYSTEM ERROR" << endl;
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    execvp(args[0], args.data());
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

  // file has been compiled to a .o, now time to read the .o and rebuild it into
  // our format
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
    if (elf.section_names.at(symbol.sh_index) == ".text") {
      symbol.value += curr_code_offset;
    }

    if (elf.section_names.at(symbol.sh_index) == ".data") {
      symbol.value += curr_data_offset;
    }

    // in multi-file will need to change non-global symbols to be unique
    // and record all global labels

    // data symbols will need to be relocated as we parse them prolly
  }

  for (auto& p : elf.relocations) {
    const auto& relos = p.second;
    if (p.first != ".text") {
      cerr << "unrecognized relocation for: " << p.first << " section" << endl;
      return EXIT_FAILURE;
    }

    for (auto& relo : relos) {
      symbol associated_symbol = elf.symtab.at(relo.symbol_index);
      Instruction to_relocate = elf.text.at(relo.addr / 4);
      elf.text.at(relo.addr / 4) = relocate_instruction(
          to_relocate, relo.addr,
          static_cast<int32_t>(associated_symbol.value) + relo.addend);
      if (relo.addend != 0) {
        cerr << "Report to travis: addend is not 0" << endl;
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // Magic word, and opening the file
  /////////////////////////////////////////////////////////////////////////////

  // deletes old contents

  unique_ptr<FILE, fcloser> out_obj{fopen(obj_name.data(), "w")};
  uint32_t temp32 = MAGIC_WORD;
  fwrite(&temp32, sizeof(temp32), 1, out_obj.get());

  /////////////////////////////////////////////////////////////////////////////
  // Writing xC0DE
  /////////////////////////////////////////////////////////////////////////////

  uint16_t temp16 = CODE_INDICATOR;

  fwrite(&temp16, sizeof(temp16), 1, out_obj.get());

  temp32 = curr_code_offset;
  fwrite(&temp32, sizeof(temp32), 1, out_obj.get());

  temp32 = elf.text.size();
  fwrite(&temp32, sizeof(temp32), 1, out_obj.get());

  for (const auto& instr : elf.text) {
    fwrite(&instr.encoding, sizeof(instr.encoding), 1, out_obj.get());
  }

  curr_code_offset += (elf.text.size() * 4);

  /////////////////////////////////////////////////////////////////////////////
  // Writing xDADA
  /////////////////////////////////////////////////////////////////////////////

  write_data_section(elf.data, out_obj.get(), curr_data_offset);
  write_data_section(elf.bss, out_obj.get(), curr_data_offset);
  write_data_section(elf.sbss, out_obj.get(), curr_data_offset);
  write_data_section(elf.data1, out_obj.get(), curr_data_offset);
  write_data_section(elf.rodata, out_obj.get(), curr_data_offset);
  write_data_section(elf.rodata1, out_obj.get(), curr_data_offset);
  

  /////////////////////////////////////////////////////////////////////////////
  // Writing Symbols
  /////////////////////////////////////////////////////////////////////////////

  for (const auto& symbol : elf.symtab) {

    // SYMB indicator
    temp16 = SYMB_INDICATOR;
    fwrite(&temp16, sizeof(temp16), 1, out_obj.get());

    // address 
    temp32 = symbol.value;
    fwrite(&temp32, sizeof(temp32), 1, out_obj.get());

    // name length
    temp32 = symbol.name.length();
    fwrite(&temp32, sizeof(temp32), 1, out_obj.get());

    // name contents
    fwrite(symbol.name.data(), 1, symbol.name.length(), out_obj.get());
  }

}

static void r_relocate(Instruction& instr, int32_t label_address);
static void i_relocate(Instruction& instr, int32_t label_address);
static void s_relocate(Instruction& instr, int32_t label_address);
static void b_relocate(Instruction& instr, int32_t label_address);
static void u_relocate(Instruction& instr, int32_t label_address);
static void j_relocate(Instruction& instr, int32_t label_address);

Instruction relocate_instruction(const Instruction& instruction,
                                 int32_t instr_address,
                                 int32_t label_address) {
  Instruction result = instruction;
  switch (instruction.type) {
    case InstructionType::R_TYPE:
      r_relocate(result, label_address);
      break;
    case InstructionType::I_TYPE:
      i_relocate(result, label_address);
      break;
    case InstructionType::S_TYPE:
      s_relocate(result, label_address);
      break;
    case InstructionType::B_TYPE:
      b_relocate(result, label_address - instr_address);
      break;
    case InstructionType::U_TYPE:
      u_relocate(result, label_address);
      break;
    case InstructionType::J_TYPE:
      j_relocate(result, label_address - instr_address);
      break;
    default:
      cerr << "WHAT THE FUCKKKKKKKKKKKK ILLEGAL INSTRUCTION TYPE IN RELOCATION"
           << endl;
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
  address = address >> 1;
  uint32_t bit12 = (address >> 12) & 0x1;
  uint32_t bit11 = (address >> 11) & 0x1;
  uint32_t bit10_to_5 = (address >> 5) & 0x3F;
  uint32_t bit4_to_1 = (address >> 1) & 0x0F;

  uint32_t pattern =
      (bit12 << 31) | (bit10_to_5 << 25) | (bit4_to_1 << 8) | (bit11 << 7);
  instr.encoding |= pattern;
}

static void u_relocate(Instruction& instr, int32_t address) {
  uint32_t pattern = reinterpret_cast<uint32_t>(address & 0xFFFFF000);
  instr.encoding |= pattern;
}

static void j_relocate(Instruction& instr, int32_t address) {
  address = address >> 1;
  uint32_t bit20 = (address >> 20) & 0x1;
  uint32_t bit10_to_1 = (address >> 1) & 0x3FF;
  uint32_t bit11 = (address >> 11) & 0x1;
  uint32_t bit19_to_12 = (address >> 12) & 0xFF;

  uint32_t pattern =
      (bit20 << 31) | (bit10_to_1 << 21) | (bit11 << 20) | (bit19_to_12 << 12);

  instr.encoding |= pattern;
}

static bool write_data_section(const section& section, FILE* out_obj, uint32_t& data_offset) {
  if (section.contents.size() == 0U) {
    // Nothing to write!
    return true;
  }
  uint16_t indicator = DATA_INDICATOR;
  fwrite(&indicator, sizeof(indicator), 1, out_obj);

  fwrite(&data_offset, sizeof(data_offset), 1, out_obj);
  
  uint32_t size = section.contents.size();
  fwrite(&size, sizeof(size), 1, out_obj);

  fwrite(section.contents.data(), sizeof(byte), section.contents.size(), out_obj);

  data_offset += size;

  return true;
}
