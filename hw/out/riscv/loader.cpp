#include "./loader.hpp"
#include "./riscv_constants.hpp"

#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>

using namespace std;

struct fcloser {
  void operator()(FILE* f) const { fclose(f); }
};

void read_code(riscv_mem& mem, FILE* obj_file);
void read_data(riscv_mem& mem, FILE* obj_file);
void read_symbol(riscv_mem& mem, FILE* obj_file);
void read_file(riscv_mem& mem, FILE* obj_file);
void read_line(riscv_mem& mem, FILE* obj_file);

expected<riscv_mem, string> load_riscv(const string& fname) {
  unique_ptr<FILE, fcloser> obj{fopen(fname.data(), "rb")};

  uint32_t temp32{};
  uint16_t temp16{};

  fread(&temp32, sizeof(temp32), 1, obj.get());

  if (temp32 != MAGIC_WORD) {
    return unexpected("MAGIC WORD IS INCORRECT");
  }

  riscv_mem mem;

  while (!feof(obj.get())) {
    uint16_t section_header{};
    int res = fread(&section_header, sizeof(temp16), 1, obj.get());

    if (res == 0) {
      break;
    }

    switch (section_header) {
      case CODE_INDICATOR:
        read_code(mem, obj.get());
        break;
      case DATA_INDICATOR:
        read_data(mem, obj.get());
        break;
      case SYMB_INDICATOR:
        read_symbol(mem, obj.get());
        break;
      case FILE_INDICATOR:
        read_file(mem, obj.get());
        break;
      case LINE_INDICATOR:
        read_line(mem, obj.get());
        break;
      default:
        cerr << "UNKOWN INDICATOR FUUUUUUCK" << hex << section_header << endl;
        return unexpected("UNKOWN INDICATOR FUCKKKKK");
    }
  }

  return mem;
}

void read_code(riscv_mem& mem, FILE* obj_file) {
  uint32_t address{};
  uint32_t size{};
  fread(&address, sizeof(address), 1, obj_file);
  fread(&size, sizeof(size), 1, obj_file);

  for (size_t i = 0; i < size; i++) {
    uint32_t instr{};
    fread(&instr, sizeof(instr), 1, obj_file);
    mem.text[address + (i * 4)] = decode_instruction(instr);

    // cout << mem.text[address + (i * 4)] << endl;
  }
}

void read_data(riscv_mem& mem, FILE* obj_file) {
  uint32_t address{};
  uint32_t size{};
  fread(&address, sizeof(address), 1, obj_file);
  fread(&size, sizeof(size), 1, obj_file);

  address -= INITIAL_DATA_OFFSET;

  int res = fread(&(mem.data.at(address)), 1, size, obj_file);
  if (static_cast<uint32_t>(res) != size) {
    cerr << "FUCK ME IN THE ASS: ASK TRAVIS TO INVESTIGATE" << endl;
    exit(EXIT_FAILURE);
  }
}

void read_symbol(riscv_mem& mem, FILE* obj_file) {
  uint32_t address{};
  uint32_t size{};
  fread(&address, sizeof(address), 1, obj_file);
  fread(&size, sizeof(size), 1, obj_file);

  string name{};
  name.reserve(size);

  for (size_t i = 0; i < size; i++) {
    char c;
    fread(&c, sizeof(c), 1, obj_file);
    name += c;
  }
  mem.label_to_addr[name] = address;
  mem.addr_to_labels[address].push_back(name);
}

void read_file(riscv_mem& mem, FILE* obj_file) {}

void read_line(riscv_mem& mem, FILE* obj_file) {}

