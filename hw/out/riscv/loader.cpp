#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <expected>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "./decoder.hpp"
#include "./loader.hpp"

using namespace std;

// file format:
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format

// elf structs (and a lot of useful information) can be found by typing
// man 5 elf

// the first instruction I get when I compile is
// fd010113
//
// you may find this useful for how to translate things to ASM:
// https://github.com/cis5710/cis5710-homework/blob/main/riscv%20isa%20reference%20sheet.pdf
//
// sample translation:
//
// fd010113
// 111111010000 00010  000    00010 0010011
//   imm12       rs1  funct3   rd    opcode
//
//   imm12 = -48
//   RS1   = 2
//   RD    = 2
//   opcode & funct3 -> addi
//
//   addi x2, x2, -48

///////////////////////////////////////////////////////////////////////////////
// DON'T HESITATE TO ASK IF YOU HAVE ANY QUESTIONS
///////////////////////////////////////////////////////////////////////////////

static expected<Elf32_Ehdr, string> read_header(FILE* elf_file);

static expected<vector<Elf32_Shdr>, string> read_section_headers(FILE* elf_file, const Elf32_Ehdr& header);

static expected<Elf32_strtab, string> read_string_table(FILE* elf_file, const Elf32_Ehdr& header, const vector<Elf32_Shdr>& section_headers);

static expected<vector<Instruction>, string> read_text(FILE* elf_file, const Elf32_Shdr& text_header);

static expected<vector<symbol>, string> read_symtab(FILE* elf_file, const Elf32_strtab& strtab, const Elf32_Shdr& symtab_header);

static expected<vector<relocation>, string> read_rela_text(FILE* elf_file, const Elf32_Shdr& rela_text_header);

struct lazy_fstream_guard {
  FILE* to_close = nullptr;

  ~lazy_fstream_guard() {
    if (this->to_close != nullptr) {
      fclose(this->to_close);
    }
  }
};

expected<riscv_elf, string> load_riscv32i_elf(const string& elf_fname) {
  
  riscv_elf result{};

  // opening the file
  FILE* elf_file = fopen(elf_fname.data(), "rb+");
  if (elf_file == NULL) {
    return unexpected("Failed to open file");
  }
  
  // will fclose(elf_file) the FILE* if
  // exception is thrown or when we return
  lazy_fstream_guard poopoo {elf_file};

  // header
  auto header_res = read_header(elf_file);
  if (!header_res) {
    return unexpected(header_res.error());
  }
  Elf32_Ehdr header = header_res.value();

  // section headers
  auto section_headers_res = read_section_headers(elf_file, header);
  if (!section_headers_res) {
    return unexpected(section_headers_res.error());
  }
  auto section_headers = section_headers_res.value();

  // string table
  auto str_tab_res = read_string_table(elf_file, header, section_headers);
  if (!str_tab_res) {
    return unexpected(str_tab_res.error());
  }
  auto str_tab = str_tab_res.value();

  // iterate through section headers
  for (size_t i = 0; i < section_headers.size(); ++i) {
    Elf32_Shdr s_hdr = section_headers.at(i);

    // get the string equivalent of the section header name field & get the type
    const char* section_name = &str_tab.contents.at(s_hdr.sh_name);
    // cout << "Section index: " << i << endl;

    // cout << "Section name: " << section_name << endl;
    // cout << "Section type: " << hex << s_hdr.sh_type << endl;

    // the RISCV instructions should be in the header that is named ".text"
    //cout << section_name << endl;
    //cout << s_hdr.sh_type << endl;
    if (strcmp(section_name, ".text") == 0) {
      // yay we found it
      // cout << "Got eem at index: " << i << endl;

      auto text_res = read_text(elf_file, s_hdr);

      result.text = text_res.value();

    } else if (strcmp(section_name, ".data") == 0) {
    } else if (strcmp(section_name, ".symtab") == 0) {
      auto symtab_res = read_symtab(elf_file, str_tab, s_hdr);
      result.symtab = symtab_res.value();
    } else if (strcmp(section_name, ".rela.text") == 0) {
      auto rela_text_res = read_rela_text(elf_file, s_hdr);
      result.relocations[".text"] = rela_text_res.value();
    }
  }
  return result;
}

static expected<Elf32_Ehdr, string> read_header(FILE* elf_file) {
  Elf32_Ehdr header{};

  // read ELF headef
  int res = fseek(elf_file, 0, SEEK_SET);
  if (res == -1) {
    return unexpected(string("Error Reading Header: ") + strerror(errno));
  }

  res = fread(&header, sizeof(header), 1, elf_file);
  if (res == 0) {
    return unexpected("Error Reading Header");
  }

  return header;

  // just some prints for debugging
  // cout << "File type is: " << hex << header.e_type << endl;
  // cout << "Architecture is: " << hex << header.e_machine << endl;
  // cout << "\t(should be 0x00F3 for RISC-V)" << endl;
}

static expected<vector<Elf32_Shdr>, string> read_section_headers(FILE* elf_file, const Elf32_Ehdr& header) {
  // seek to the offest of the section headers
  fseek(elf_file, header.e_shoff, SEEK_SET);

  // read the section headers into a vector
  vector<Elf32_Shdr> section_headers{};

  for (size_t i = 0; i < header.e_shnum; ++i) {
    Elf32_Shdr section_header{};
    fread(&section_header, sizeof(Elf32_Shdr), 1, elf_file);
    section_headers.push_back(section_header);
  }

  return section_headers;
}

static expected<Elf32_strtab, string> read_string_table(FILE* elf_file, const Elf32_Ehdr& header, const vector<Elf32_Shdr>& section_headers) {
  // section header string table
  // get the string table
  Elf32_Shdr shstrtab_header = section_headers.at(header.e_shstrndx);

  // allocate a vector of the size of string table
  vector<char> shstrtab(shstrtab_header.sh_size);

  // go to the table in the elf file
  fseek(elf_file, shstrtab_header.sh_offset, SEEK_SET);

  // read it into the vector
  fread(shstrtab.data(), shstrtab_header.sh_size, 1, elf_file);

  Elf32_strtab result {};
  result.header = shstrtab_header;
  result.contents = std::move(shstrtab);
  return result;
}

static expected<vector<Instruction>, string> read_text(FILE* elf_file, const Elf32_Shdr& text_header){
  // read the .text section

  // create a vector of the number of elemenjts
  // (#bytes of file / 4 bytes per inst)
  vector<uint32_t> encodings(text_header.sh_size / 4); 

  vector<Instruction> instructions;

  // cout << "Text size: 0x" << std::hex << text_header.sh_size << endl;

  // move file ptr to start of the .text
  fseek(elf_file, text_header.sh_offset, SEEK_SET);
  fread(encodings.data(), sizeof(uint32_t), encodings.size(),
        elf_file);

  for (uint32_t instr : encodings) {
    instructions.push_back(decode_instruction(instr));
  }

  return instructions;
}

static expected<vector<symbol>, string> read_symtab(FILE* elf_file, const Elf32_strtab& strtab, const Elf32_Shdr& symtab_header) {
  fseek(elf_file, symtab_header.sh_offset, SEEK_SET);
  
  vector<Elf32_Sym> symbols(symtab_header.sh_size / sizeof(Elf32_Sym));
  vector<symbol> res;
  // std::cout << symtab_header.sh_size << std::endl;
  // std::cout << symtab_header.sh_size % sizeof(Elf32_Sym) << std::endl;

  fread(symbols.data(), sizeof(Elf32_Sym), symbols.size(), elf_file);

  for (const auto&  sym : symbols) {
    const char* name = &strtab.contents.at(sym.st_name);
    symbol s{};
    s.name = name;
    s.size = sym.st_size;
    s.index = sym.st_value;
    s.global = sym.st_other == STB_GLOBAL;
    res.push_back(s);
  }

  return res;
}

static expected<vector<relocation>, string> read_rela_text(FILE* elf_file, const Elf32_Shdr& rela_text_header) {
  fseek(elf_file, rela_text_header.sh_offset, SEEK_SET);

  vector<Elf32_Rela> relocations(rela_text_header.sh_size / sizeof(Elf32_Rela));
  vector<relocation> res;
  std::cout << rela_text_header.sh_size << std::endl;
  // std::cout << symtab_header.sh_size % sizeof(Elf32_Sym) << std::endl;

  fread(relocations.data(), sizeof(Elf32_Rela), relocations.size(), elf_file);



  for (const auto& rela : relocations) {
    relocation r{};
    r.addr = rela.r_offset;
    r.symbol_index = ELF32_R_SYM(rela.r_info);
    r.type = ELF32_R_TYPE(rela.r_info);
    r.addend = rela.r_addend;
    res.push_back(r);
  }

  return res;

}
