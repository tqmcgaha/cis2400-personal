#ifndef LOADER_HPP_
#define LOADER_HPP_

#include <elf.h>
#include <expected>
#include <string>
#include <unordered_map>
#include <vector>
#include "./decoder.hpp"

// elf32 string table
struct Elf32_strtab {
  Elf32_Shdr header;
  std::vector<char> contents;
};

struct symbol {
  std::string name;
  uint32_t index;
  uint32_t size;
  bool global;
};


struct section {
  Elf32_Shdr header;
  std::vector<std::byte> contents;
};

struct relocation {
  uint32_t addr;
  uint32_t symbol_index;
  uint8_t type;
  // type seems to indicate what type of instruction
  // is needed to be relocated. B type had this be 10, J type had 11.
  int32_t addend;
};

struct riscv_elf {
  // header
  Elf32_Ehdr header;
  
  // section_headers
  std::vector<Elf32_Shdr> section_headers;

  // string table
  Elf32_strtab string_table;

  // .text
  std::vector<Instruction> text;

  // .bss

  // .sbss

  
  // .data

  // .data1

  // .rodata

  // .rodata1

  // .symtab
  std::vector<symbol> symtab;

  // .rela
  std::unordered_map<std::string, std::vector<relocation>> relocations;

};

std::expected<riscv_elf, std::string> load_riscv32i_elf(
    const std::string& elf_fname);

#endif  // LOADER_HPP_
