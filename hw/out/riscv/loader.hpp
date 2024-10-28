#ifndef LOADER_HPP_
#define LOADER_HPP_

#include "./decoder.hpp"
#include "./riscv_constants.hpp"

#include <expected>
#include <string>
#include <unordered_map>
#include <vector>

struct riscv_mem {
  std::unordered_map<uint32_t, Instruction>
      text;                     // will be SEGMENT_SIZE / 4 in size
  std::vector<std::byte> data;  // will be SEGMENT_SIZE in size
  std::unordered_map<std::string, uint32_t> label_to_addr;
  std::unordered_map<uint32_t, std::vector<std::string>> addr_to_labels;
};

std::expected<riscv_mem, std::string> load_riscv(const std::string& fname);

#endif  // LOADER_HPP_
