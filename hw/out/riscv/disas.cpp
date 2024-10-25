#include "./decoder.hpp"
#include "./loader.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
  //auto res = load_riscv32i_elf("cstdlib.o");
  auto res = load_riscv32i_elf("handwritten.o");
  //auto res = load_riscv32i_elf("test_riscv_main.o");

  riscv_elf state = std::move(res.value());

  for (const auto& instr : state.text) {
    cout << instr << endl;
    cout << hex << setw(8) << setfill('0') << instr.encoding << endl;
  }

  // cout << std::dec << res.value().text.size() << endl;

  // load_riscv32i_elf("test_riscv_main.o");
  // uint64_t size = 1;
  // size = size << 32;

  for (const auto& sym : state.symtab) {
    cout << sym.name << " index: " << dec << sym.index << " size: " << sym.size << endl;
  }

  for (const auto& p : state.relocations[".text"]) {
    cout << "addr: " << hex << setw(8) << setfill('0') << p.addr << endl;;
    cout << "symbol_index: " << p.symbol_index << endl;
    cout << "type: " << static_cast<unsigned int>(p.type) << endl;
    cout << "addend: " << p.addend << endl;
  }

  // cout << "The address of main is: 0x" << std::setfill('0') << std::setw(16) << std::hex << &main << endl;

/*
  std::cout << std::dec << size << std::endl;

  uint8_t* rv32_mem = (uint8_t *) malloc(sizeof(uint8_t) * size);
  
  std::cout << "PLEASE WORK DEAR GOD" << std::endl;
  std::cout << sizeof(size_t) << std::endl;

  free(rv32_mem);

  std::cout << "please" << std::endl;
*/

}
