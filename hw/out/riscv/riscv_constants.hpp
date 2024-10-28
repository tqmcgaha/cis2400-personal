#ifndef RISCV_CONSTANTS_HPP_
#define RISCV_CONSTANTS_HPP_

#include <cstdint>

// 32 bits is 4gb
// can break this into 16 segments that are 256 MiB each (2^28)
// 

// 0x 00 00 00 00
// first page of memory purposefully left blank

// 0x 10 00 00 00  
// .text xC0DE

// 0x 20 00 00 00
// .data xDADA

// 0x 30 00 00 00
// heap segment (4 segments)

// 0x 70 00 00 00
// stack segment

// 0x 80 00 00 00
// OS has the last 8 segments

static constexpr uint32_t MAGIC_WORD = 0xCAFEF00D;
static constexpr uint32_t INITIAL_CODE_OFFSET = 0x10000000;
static constexpr uint32_t INITIAL_DATA_OFFSET = 0x20000000;

static constexpr uint32_t SEGMENT_SIZE = 1 << 28;

static constexpr uint16_t CODE_INDICATOR = 0xC0DE;
static constexpr uint16_t DATA_INDICATOR = 0xDADA;
static constexpr uint16_t SYMB_INDICATOR = 0xC3B7;
static constexpr uint16_t FILE_INDICATOR = 0xF17E;
static constexpr uint16_t LINE_INDICATOR = 0x715E;

#endif   // RISCV_CONSTANTS.HPP
