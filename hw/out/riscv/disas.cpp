#include "./decoder.hpp"
#include "./loader.hpp"

int main() {
  load_riscv32i_elf("cstdlib.o");
}
