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

expected<riscv_elf, string> load_riscv32i_elf(const string& elf_fname) {
  Elf32_Ehdr header{};

  FILE* elf_file = fopen(elf_fname.data(), "rb+");
  if (elf_file == NULL) {
    return unexpected("Failed to open file");
  }
  // read ELF headef
  fread(&header, sizeof(header), 1, elf_file);

  // just some prints for debugging
  cout << "File type is: " << hex << header.e_type << endl;
  cout << "Architecture is: " << hex << header.e_machine << endl;
  cout << "\t(should be 0x00F3 for RISC-V)" << endl;

  // seek to the offest of the section headers
  fseek(elf_file, header.e_shoff, SEEK_SET);

  // read the section headers into a vector
  vector<Elf32_Shdr> section_headers{};

  for (size_t i = 0; i < header.e_shnum; ++i) {
    Elf32_Shdr section_header{};
    fread(&section_header, sizeof(Elf32_Shdr), 1, elf_file);
    section_headers.push_back(section_header);
  }

  // section header string table
  Elf32_Shdr shstrtab_header =
      section_headers[header.e_shstrndx];  // get the string table
  vector<char> shstrtab(
      shstrtab_header
          .sh_size);  // allocate a vector of the size of string table
  fseek(elf_file, shstrtab_header.sh_offset,
        SEEK_SET);  // go to the table in the elf file
  fread(shstrtab.data(), shstrtab_header.sh_size, 1,
        elf_file);  // read it into the vector
  //   cout << "Section header string table size: " << shstrtab.size() << endl;

  // iterate through section headers
  for (size_t i = 0; i < section_headers.size(); ++i) {
    Elf32_Shdr s_hdr = section_headers[i];

    const char* section_name = &shstrtab[s_hdr.sh_name];
    // cout << "Section index: " << i << endl;
    // cout << "Section name: " << section_name << endl;
    // cout << "Section type: 0x" << hex << s_hdr.sh_type << endl;
    // cout << "Section offset: 0x" << hex << s_hdr.sh_offset << endl;
    // cout << "Section size: 0x" << hex << s_hdr.sh_size << endl;
    // cout << endl;
  }

  // iterate through section headers
  for (size_t i = 0; i < section_headers.size(); ++i) {
    Elf32_Shdr s_hdr = section_headers.at(i);
    // Elf32_Shdr s_hdr = section_headers[i];

    // get the string equivalent of the section header name field & get the type
    const char* section_name = &shstrtab[s_hdr.sh_name];
    // cout << "Section index: " << i << endl;

    // cout << "Section name: " << section_name << endl;
    // cout << "Section type: " << hex << s_hdr.sh_type << endl;

    // the RISCV instructions should be in the header that is named ".text"
    cout << section_name << endl;
    if (strcmp(section_name, ".text") == 0) {
      // yay we found it
      cout << "Got eem at index: " << i << endl;

      // read the .text section
      vector<uint32_t> instructions(
          s_hdr.sh_size / 4);  // create a vector of the number of elemenjts (#
                               // bytes of file / 4 bytes per inst)
      fseek(elf_file, s_hdr.sh_offset,
            SEEK_SET);  // move file ptr to start of the .text
      fread(instructions.data(), sizeof(uint32_t), instructions.size(),
            elf_file);
      for (uint32_t instr : instructions) {
        Instruction instruction = decode_instruction(instr);

        // cout << instruction.mnemonic << " x" << instruction.rd << ", x" <<
        // instruction.rs1 << ", x" << instruction.rs2 << ", " <<
        // instruction.imm << endl;
        cout << instr_to_string(instruction) << endl;
      }
      fclose(elf_file);
    }
  }
}
