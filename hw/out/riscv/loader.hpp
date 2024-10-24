#ifndef LOADER_HPP_
#define LOADER_HPP_

#include <elf.h>
#include <expected>
#include <string>
#include <unordered_map>
#include <vector>

class riscv_elf {
 public:
 private:
};

std::expected<riscv_elf, std::string> load_riscv32i_elf(
    const std::string& elf_fname);

#endif  // LOADER_HPP_
