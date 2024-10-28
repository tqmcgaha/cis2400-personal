#ifndef SIMULATOR_HPP_
#define SIMULATOR_HPP_

#include "./decoder.hpp"
#include "./loader.hpp"

#include <expected>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>

constexpr uint64_t ONE_MILLION = 1000000;
constexpr size_t NUM_REGISTERS = 32U;

#include <concepts>

class riscv_prog {
 public:
  riscv_prog(const riscv_prog& other) = delete;
  riscv_prog& operator=(const riscv_prog& other) = delete;

  riscv_prog(riscv_prog&& other);
  riscv_prog& operator=(riscv_prog&& other);

  std::expected<uint32_t, std::string> run();

  std::expected<uint32_t, std::string> step();

  void set_instr_limit(uint64_t);

  void clear_addresses_read();

  void clear_addresses_written();

  void clear_instrs_run();

  void set_register(uint32_t reg_num, int32_t value);

  void set_pc(uint32_t value);

  std::optional<uint32_t> resolve_label(const std::string& label_name);

  bool set_memory(uint32_t address, const std::span<std::byte> bytes);

  void register_break_point(uint32_t address);

  void unregister_break_point(uint32_t address);

  riscv_prog() = default;

  uint32_t get_pc();

  int32_t get_register(uint32_t reg_num);

  const Instruction& get_instr(uint32_t addr);

  bool has_instr(uint32_t addr);

  friend std::expected<riscv_prog, std::string> load_riscv_prog(
    const std::string& fname);
 private:
  // data members
  std::array<int32_t, NUM_REGISTERS> registers;
  uint32_t program_counter;
  riscv_mem mem;
  std::unordered_set<uint32_t> breakpoints;
  std::unordered_set<uint32_t> addresses_read;
  std::unordered_set<uint32_t> addresses_written;
  uint64_t instructions_run;
  uint64_t instruction_limit;

  std::optional<std::string> execute_r_type();
  std::optional<std::string> execute_i_type();
  std::optional<std::string> execute_s_type();
  std::optional<std::string> execute_b_type();
  std::optional<std::string> execute_u_type();
  std::optional<std::string> execute_j_type();
};


template <typename T>
concept byte_copyable = std::is_trivially_copyable_v<T>;

template <typename T>
requires byte_copyable<T>
[[nodiscard]] const std::span<std::byte> as_bytes(const T& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(&thing),
                              reinterpret_cast<std::byte*>((&thing) + 1));
}

template <typename T, size_t N>
requires byte_copyable<T>
[[nodiscard]] const std::span<std::byte> as_writable_bytes(
    const std::array<T, N>& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(thing.data()),
                              reinterpret_cast<std::byte*>(thing.end()));
}

template <typename T>
requires byte_copyable<T>
[[nodiscard]] const std::span<std::byte> as_writable_bytes(
    const std::vector<T>& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(thing.data()),
                              reinterpret_cast<std::byte*>(thing.end()));
}

template <typename T>
requires byte_copyable<T>
[[nodiscard]] std::span<std::byte> as_writable_bytes(T& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(&thing),
                              reinterpret_cast<std::byte*>((&thing) + 1));
}

template <typename T, size_t N>
requires byte_copyable<T>
[[nodiscard]] std::span<std::byte> as_writable_bytes(std::array<T, N>& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(thing.data()),
                              reinterpret_cast<std::byte*>(thing.end()));
}

template <typename T>
requires byte_copyable<T>
[[nodiscard]] std::span<std::byte> as_writable_bytes(std::vector<T>& thing) {
  return std::span<std::byte>(reinterpret_cast<std::byte*>(thing.data()),
                              reinterpret_cast<std::byte*>(thing.end()));
}

#endif  // SIMULATOR_HPP_
