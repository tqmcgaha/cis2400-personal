#include <iomanip>
#include <iostream>
#include "./simulator.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./penn-sim <obj file>" << endl;
    return EXIT_FAILURE;
  }

  auto res = load_riscv_prog(argv[1]);

  if (!res) {
    cerr << res.error() << endl;
    return EXIT_FAILURE;
  }

  prog = res.value();

  while (cin) {
    string cmd;
    cin >> cmd;

    if (cmd == "pc") {
      string label;
      cin >> label;
      auto addr = prog.resolve_label(label);

      if (!addr) {
        cerr << "Error: Unable to resolve label: \"" << label << "\"" << endl;
      } else {
        prog.set_pc(addr.value());
      }
    } else if (cmd == "run") {
      auto res = prog.run();
      if (!res) {
        cerr << res.error() << endl;
      }
    } else if (cmd == "list") {
      uint32_t start_addr = prog.get_pc() - 5;
      if (start_addr < INITIAL_CODE_OFFSET) {
        start_addr = INITIAL_CODE_OFFSET;
      }

      for (size_t i = 0; i < 15; i++) {
        if (prog.has_instr(start_addr + i)) {
          cerr << prog.get_instr(start_addr + i) << endl;
        }
      }
    } else if (cmd == "") {
    }
  }
}
