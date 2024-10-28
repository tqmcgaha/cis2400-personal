#include "./simulator.hpp"

#include <cstring> // for memcpy

using namespace std;

riscv_prog::riscv_prog(riscv_prog&& other)
    : registers(move(other.registers)),
      program_counter(other.program_counter),
      mem(move(other.mem)),
      breakpoints(move(other.breakpoints)),
      addresses_read(move(other.addresses_read)),
      addresses_written(move(other.addresses_written)),
      instructions_run(other.instructions_run),
      instruction_limit(other.instruction_limit) {}

riscv_prog& riscv_prog::operator=(riscv_prog&& other) {
  if (&other == this) {
    return *this;
  }

  this->registers = move(other.registers);
  this->program_counter = other.program_counter;
  this->mem = move(other.mem);
  this->breakpoints = move(other.breakpoints);
  this->addresses_read = move(other.addresses_read);
  this->addresses_written = move(other.addresses_written);
  this->instructions_run = other.instructions_run;
  this->instruction_limit = other.instruction_limit;

  return *this;
}

expected<riscv_prog, std::string> load_riscv_prog(const std::string& fname) {
  auto res = load_riscv(fname);
  if (!res) {
    return unexpected(res.error());
  }
  riscv_prog prog;
  prog.mem = res.value();
  return prog;
}

std::expected<uint32_t, std::string> riscv_prog::run() {
  // step while havn't hit the instruction limit
  // or encountered some other issue
  // or hit a breakpoint.

  // TODO: not running

  if (this->instructions_run >= this->instruction_limit && this->instruction_limit != 0U) {
    return unexpected("Hit the instruction limit");
  }

  while (!this->breakpoints.contains(this->program_counter) &&
         (this->instructions_run >= this->instruction_limit || this->instruction_limit == 0U)) {
    auto res = this->step();
    if (!res) {
      return unexpected(res.error());
    }
  }

  return this->program_counter;
}

void riscv_prog::set_instr_limit(uint64_t new_limit) {
  this->instruction_limit = new_limit;
}

void riscv_prog::clear_addresses_read() {
  this->addresses_read.clear();
}

void riscv_prog::clear_addresses_written() {
  this->addresses_written.clear();
}

void riscv_prog::clear_instrs_run() {
  this->instructions_run = 0;
}

void riscv_prog::set_register(uint32_t reg_num, int32_t value) {
  if (reg_num != 0) {
    this->registers.at(reg_num) = value;
  }
}

void riscv_prog::set_pc(uint32_t value) {
  this->program_counter = value;
}

std::optional<uint32_t> riscv_prog::resolve_label(
    const std::string& label_name) {
  if (!this->mem.label_to_addr.contains(label_name)) {
    return nullopt;
  }
  return this->mem.label_to_addr[label_name];
}

bool riscv_prog::set_memory(uint32_t address,
                            const std::span<std::byte> bytes) {
  if (address < INITIAL_DATA_OFFSET ||
      address >= INITIAL_DATA_OFFSET + SEGMENT_SIZE ||
      address + bytes.size() >= INITIAL_DATA_OFFSET + SEGMENT_SIZE) {
    return false;
  }
  memcpy(&(this->mem.data.at(address)), bytes.data(), bytes.size());
  return true;
}

void riscv_prog::register_break_point(uint32_t address) {
  this->breakpoints.insert(address);
}

void riscv_prog::unregister_break_point(uint32_t address) {
  this->breakpoints.erase(address);
}

uint32_t riscv_prog::get_pc() {
  return this->program_counter;
}

int32_t riscv_prog::get_register(uint32_t reg_num) {
  return this->registers.at(reg_num);
}

const Instruction& riscv_prog::get_instr(uint32_t addr) {
  return this->mem.text.at(addr);
}

bool riscv_prog::has_instr(uint32_t addr) {
  return this->mem.text.contains(addr);
}

expected<uint32_t, string> riscv_prog::step() {
  // TODO
  if (this->program_counter % 4 != 0) {
    return unexpected("Program counter is not aligned!");
  }

  if (this->program_counter < INITIAL_CODE_OFFSET ||
      this->program_counter >= INITIAL_DATA_OFFSET) {
    return unexpected("Program counter is not pointing to program data!");
  }

  const Instruction& instr = this->mem.text.at(this->program_counter);

  optional<string> res{};

  switch (instr.type) {
    case InstructionType::R_TYPE:
      res = execute_r_type();
      break;
    case InstructionType::I_TYPE:
      res = execute_i_type();
      break;
    case InstructionType::S_TYPE:
      res = execute_s_type();
      break;
    case InstructionType::B_TYPE:
      res = execute_b_type();
      break;
    case InstructionType::U_TYPE:
      res = execute_u_type();
      break;
    case InstructionType::J_TYPE:
      res = execute_j_type();
      break;
  };

  if (res) {
    return unexpected(res.value());
  }

  return this->program_counter;
}

std::optional<std::string> riscv_prog::execute_r_type() {
  if (this->program_counter >= INITIAL_CODE_OFFSET + SEGMENT_SIZE - 4) {
    return "Program counter would be set to be out of bounds";
  }

  const Instruction& instr = this->mem.text.at(this->program_counter);
  if (instr.mnemonic == "add") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) + this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "sub") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) - this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "sll") {
    this->set_register(instr.rd, this->registers.at(instr.rs1)
                                   << (this->registers.at(instr.rs2) & 0x01F));
  } else if (instr.mnemonic == "slt") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) < this->registers.at(instr.rs2) ? 1 : 0);
  } else if (instr.mnemonic == "sltu") {
    this->set_register(instr.rd, 
        static_cast<uint32_t>(this->registers.at(instr.rs1)) <
                static_cast<uint32_t>(this->registers.at(instr.rs2))
            ? 1
            : 0);
  } else if (instr.mnemonic == "xor") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) ^ this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "srl") {
    this->set_register(instr.rd, static_cast<int32_t>(
        static_cast<uint32_t>(this->registers.at(instr.rs1)) >>
        (this->registers.at(instr.rs2) & 0x01F)));
  } else if (instr.mnemonic == "sra") {
    this->set_register(instr.rd, this->registers.at(instr.rs1) >>
                                   (this->registers.at(instr.rs2) & 0x01F));
  } else if (instr.mnemonic == "or") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) | this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "and") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) & this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "mul") {
    int64_t res = this->registers.at(instr.rs1) * this->registers.at(instr.rs2);
    this->set_register(instr.rd, static_cast<int32_t>(res & 0xFFFFFFFF));
  } else if (instr.mnemonic == "mulh") {
    int64_t res = this->registers.at(instr.rs1) * this->registers.at(instr.rs2);
    this->set_register(instr.rd, 
        static_cast<int32_t>((res >> 32) & 0xFFFFFFFF));
  } else if (instr.mnemonic == "mulhsu") {
    int64_t res = this->registers.at(instr.rs1) *
                  static_cast<uint32_t>(this->registers.at(instr.rs2));
    this->set_register(instr.rd, 
        static_cast<int32_t>((res >> 32) & 0xFFFFFFFF));
  } else if (instr.mnemonic == "mulhu") {
    int64_t res = static_cast<uint32_t>(this->registers.at(instr.rs1)) *
                  static_cast<uint32_t>(this->registers.at(instr.rs2));
    this->set_register(instr.rd, 
        static_cast<int32_t>((res >> 32) & 0xFFFFFFFF));
  } else if (instr.mnemonic == "div") {
    if (this->registers.at(instr.rs2) == 0) {
      return "Divide by Zero";
    }
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) / this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "divu") {
    if (this->registers.at(instr.rs2) == 0) {
      return "Divide Unsigned by Zero";
    }
    this->set_register(instr.rd, static_cast<int32_t>(
        static_cast<uint32_t>(this->registers.at(instr.rs1)) /
        static_cast<uint32_t>(this->registers.at(instr.rs2))));
  } else if (instr.mnemonic == "rem") {
    if (this->registers.at(instr.rs2) == 0) {
      return "Modulo by Zero";
    }
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) % this->registers.at(instr.rs2));
  } else if (instr.mnemonic == "remu") {
    if (this->registers.at(instr.rs2) == 0) {
      return "Modulo Unsigned by Zero";
    }
    this->set_register(instr.rd, static_cast<int32_t>(
        static_cast<uint32_t>(this->registers.at(instr.rs1)) %
        static_cast<uint32_t>(this->registers.at(instr.rs2))));
  } else {
    return "ILLEGAL R TYPE INSTRUCTION";
  }

  this->program_counter += 4;

  return nullopt;
}

std::optional<std::string> riscv_prog::execute_i_type() {
  const Instruction& instr = this->mem.text.at(this->program_counter);

  // JALR
  if (instr.mnemonic == "jalr") {
    uint32_t new_pc = (this->registers.at(instr.rs1) + instr.imm) & 0xFFFFFFFE;
    if (new_pc < INITIAL_CODE_OFFSET || new_pc >= INITIAL_DATA_OFFSET) {
      return "Program Counter will be set to not pointing to program data "
             "after JALR!";
    }

    this->set_register(instr.rd, this->program_counter + 4);
    this->program_counter = new_pc;
    return nullopt;
  }

  if (this->program_counter >= INITIAL_CODE_OFFSET + SEGMENT_SIZE - 4) {
    return "Program counter would be set to be out of bounds";
  }

  // All other I type instructions
  if (instr.mnemonic == "addi") {
    this->set_register(instr.rd, this->registers.at(instr.rs1) + instr.imm);
  } else if (instr.mnemonic == "slti") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) < instr.imm ? 1 : 0);
  } else if (instr.mnemonic == "sltiu") {
    this->set_register(instr.rd, 
        static_cast<uint32_t>(this->registers.at(instr.rs1)) < instr.uimm ? 1 : 0);
  } else if (instr.mnemonic == "xori") {
    this->set_register(instr.rd, this->registers.at(instr.rs1) ^ instr.imm);
  } else if (instr.mnemonic == "ori") {
    this->set_register(instr.rd, this->registers.at(instr.rs1) | instr.imm);
  } else if (instr.mnemonic == "andi") {
    this->set_register(instr.rd, this->registers.at(instr.rs1) & instr.imm);
  } else if (instr.mnemonic == "slli") {
    this->set_register(instr.rd, this->registers.at(instr.rs1)
                                   << (instr.imm & 0x1F));
  } else if (instr.mnemonic == "srli") {
    this->set_register(instr.rd, static_cast<int32_t>(
        static_cast<uint32_t>(this->registers.at(instr.rs1) >>
        (instr.imm & 0x1F))));
  } else if (instr.mnemonic == "srai") {
    this->set_register(instr.rd, 
        this->registers.at(instr.rs1) >> (instr.imm & 0x1F));
  } else if (instr.mnemonic == "lb") {
    uint32_t addr =
        static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE) {
      return "Tries to load bytes from non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->set_register(instr.rd, 
        static_cast<int32_t>(static_cast<int8_t>(this->mem.data.at(addr))));
  } else if (instr.mnemonic == "lh") {
    uint32_t addr =
        static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE - 1) {
      return "Tries to load bytes from non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->set_register(instr.rd, static_cast<int32_t>(
        *reinterpret_cast<int16_t*>(&(this->mem.data.at(addr)))));
  } else if (instr.mnemonic == "lw") {
    uint32_t addr =
        static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE - 3) {
      return "Tries to load bytes from non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->set_register(instr.rd, 
        *reinterpret_cast<int32_t*>(&(this->mem.data.at(addr))));
  } else if (instr.mnemonic == "lbu") {
    uint32_t addr =
        static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE) {
      return "Tries to load bytes from non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->set_register(instr.rd, 
        static_cast<int32_t>(static_cast<uint8_t>(this->mem.data.at(addr))));
  } else if (instr.mnemonic == "lhu") {
    uint32_t addr =
        static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE - 1) {
      return "Tries to load bytes from non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->set_register(instr.rd, static_cast<int32_t>(
        *reinterpret_cast<uint16_t*>(&(this->mem.data.at(addr)))));
  } else {
    return "ILLEGAL I TYPE INSTRUCTION";
  }

  this->program_counter += 4;

  return nullopt;
}

std::optional<std::string> riscv_prog::execute_s_type() {
  const Instruction& instr = this->mem.text.at(this->program_counter);
  uint32_t addr =
      static_cast<uint32_t>(this->registers.at(instr.rs1) + instr.imm);

  if (this->program_counter >= INITIAL_CODE_OFFSET + SEGMENT_SIZE - 4) {
    return "Program counter would be set to be out of bounds";
  }

  if (instr.mnemonic == "sb") {
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE) {
      return "Tries to store bytes to non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->mem.data.at(addr) = static_cast<byte>(this->registers.at(instr.rs2) & 0xFF);
  } else if (instr.mnemonic == "sh") {
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE - 1) {
      return "Tries to store bytes to non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->mem.data.at(addr) = static_cast<byte>(this->registers.at(instr.rs2) & 0xFF);
    this->mem.data.at(addr + 1) =
        static_cast<byte>((this->registers.at(instr.rs2) >> 8) & 0xFF);
  } else if (instr.mnemonic == "sw") {
    if (addr < INITIAL_DATA_OFFSET ||
        addr >= INITIAL_DATA_OFFSET + SEGMENT_SIZE - 3) {
      return "Tries to store bytes to non-data memory";
    }
    addr -= INITIAL_DATA_OFFSET;
    this->mem.data.at(addr) = static_cast<byte>(this->registers.at(instr.rs2) & 0xFF);
    this->mem.data.at(addr + 1) =
        static_cast<byte>((this->registers.at(instr.rs2) >> 8) & 0xFF);
    this->mem.data.at(addr + 2) =
        static_cast<byte>((this->registers.at(instr.rs2) >> 16) & 0xFF);
    this->mem.data.at(addr + 3) =
        static_cast<byte>((this->registers.at(instr.rs2) >> 24) & 0xFF);
  } else {
    return "ILLEGAL S TYPE INSTRUCTION";
  }

  this->program_counter += 4;

  return nullopt;
}

std::optional<std::string> riscv_prog::execute_b_type() {
  const Instruction& instr = this->mem.text.at(this->program_counter);

  int32_t a = this->registers.at(instr.rs1);
  int32_t b = this->registers.at(instr.rs2);

  bool branch = false;

  if (instr.mnemonic == "beq") {
    branch = a == b;
  } else if (instr.mnemonic == "bne") {
    branch = a != b;
  } else if (instr.mnemonic == "blt") {
    branch = a < b;
  } else if (instr.mnemonic == "bge") {
    branch = a >= b;
  } else if (instr.mnemonic == "bltu") {
    branch = static_cast<uint32_t>(a) < static_cast<uint32_t>(b);
  } else if (instr.mnemonic == "bgeu") {
    branch = static_cast<uint32_t>(a) >= static_cast<uint32_t>(b);
  } else {
    return "ILLEGAL B TYPE INSTRUCTION";
  }

  uint32_t offset = branch ? (instr.imm << 1) : 4;

  uint32_t new_pc = offset + this->program_counter;

  if (new_pc < INITIAL_CODE_OFFSET ||
      new_pc >= INITIAL_CODE_OFFSET + SEGMENT_SIZE - 4) {
    return "Would Branch to a non code segment";
  }

  this->program_counter = new_pc;
  return nullopt;
}

std::optional<std::string> riscv_prog::execute_u_type() {
  if (this->program_counter >= INITIAL_CODE_OFFSET + SEGMENT_SIZE - 4) {
    return "Program counter would be set to be out of bounds";
  }

  const Instruction& instr = this->mem.text.at(this->program_counter);

  if (instr.mnemonic == "lui") {
    this->set_register(instr.rd, (instr.imm << 20));
  } else if (instr.mnemonic == "auipc") {
    this->set_register(instr.rd, this->program_counter + (instr.imm << 20));
  } else {
    return "ILLEGAL U TYPE INSTRUCTION";
  }

  this->program_counter += 4;

  return nullopt;
}

std::optional<std::string> riscv_prog::execute_j_type() {
  const Instruction& instr = this->mem.text.at(this->program_counter);

  if (instr.mnemonic != "jal") {
    return "ILLEGAL J TYPE INSTRUCTION";
  }

  uint32_t new_pc = (this->program_counter + (instr.imm << 1));
  if (new_pc < INITIAL_CODE_OFFSET || new_pc >= INITIAL_DATA_OFFSET) {
    return "Program Counter will be set to not pointing to program data after "
           "JAL!";
  }

  this->set_register(instr.rd, this->program_counter + 4);
  this->program_counter = new_pc;

  return nullopt;
}

/*
constexpr uint64_t ONE_MILLION = 1000000;
constexpr size_t NUM_REGISTERS = 32U;



class riscv_prog {

  riscv_prog(const riscv_prog& other) = delete;
  riscv_prog& operator=(const riscv_prog& other) = delete;

  riscv_prog(riscv_prog&& other);
  riscv_prog& operator=(riscv_prog&& other);

  std::expected<riscv_prog, std::string> load_riscv_prog(const std::string&
fname);

  std::expected<uint32_t, std::string> run();

  std::expected<uint32_t, std::string> step();

  void set_instr_limit(uint64_t);

  void clear_addresses_read();

  void clear_addresses_written();

  void clear_instrs_run();

  void set_register(uint32_t reg_num, int32_t value);

  std::optional<uint32_t> resolve_label(const std::string& label_name);

  void set_memory(uint32_t address, const std::span<std::byte> bytes);

  void register_break_point(uint32_t address);

  void unregister_break_point(uint32_t address);

 private:
  riscv_prog();

  // data members
  std::array<int32_t, NUM_REGISTERS> registers;
  int32_t program_counter;
  riscv_mem mem;
  std::unordered_set<uint32_t> breakpoints;
  std::unordered_set<uint32_t> addresses_read;
  std::unordered_set<uint32_t> addresses_written;
  uint64_t instructions_run;
  uint64_t instruction_limit;
*/
