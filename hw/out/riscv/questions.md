# What I know
seems to be based on riscv32 (32 bit registers)

# Question:
For your RV32, what issues have you had?

Do you have a single cycle data path diagram?

What is the word size and addressability of the processor you build?
  - word size is 32 bites
  - addressability is 1 byte

Do you follow any specific calling conventions?

Any OS implementation stuff?

# Things to trim for risc-v for cis 2400
- loads: only keep LW
- stores: only keep sw

- mul: only keep MUL
  - maybe keep MULH?

- cut the fences

- risscv instructions and registers are word size
  - memory is byte addressible 


Cross compoilation!!!!
- clang --target=riscv32 -S cstdlib.c
  works!!!

