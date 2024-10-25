#include "./loader.hpp"
#include "./decoder.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;


// 0x 00 00 00 00
// first page of memory purposefully left blank

// 0x 00 00 10 00  
// .text xC0DE

// 0x 00 08 00 00
// .data xDADA


int main(int argc, char* argv[]) {
  vector<string> fnames;
  for (int i = 1; i < argc; i++) {
    fnames.push_back(argv[i]);
  }

  vector<uint32_t> cade_offsets;

  unordered_man<string, riscv_elf> elfs;

  for (const auto& fname : fnames) {
    auto res_state = load_riscv32i_elf(fname);
    if (!res_state) {
      cerr << res_state.error() << endl;
      return EXIT_FAILURE;
    }
    elfs[fname] = res_state.value();

    
  }

  // fuck my life into pieces

}
