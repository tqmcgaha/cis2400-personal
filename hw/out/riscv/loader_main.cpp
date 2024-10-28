#include "./loader.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./loader <riscv_obj>" << endl;
    return EXIT_FAILURE;
  }

  string fname{argv[1]};

  auto res = load_riscv(fname);

  if (!res) {
    cerr << res.error() << endl;
  } else {
    riscv_mem mem = res.value();
  }
}
