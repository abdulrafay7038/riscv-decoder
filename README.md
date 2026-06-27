# RISC-V RV32I Instruction Decoder

A modular command-line **RISC-V RV32I Instruction Decoder** written in **C**. The decoder reads hexadecimal machine instructions from a file, decodes them according to the **RV32I Base Integer Instruction Set**, and prints the corresponding assembly instructions.

---

## Features

* Supports the complete **RV32I Base Integer ISA**
* Decodes all six RISC-V instruction formats:

  * R-Type
  * I-Type

    * Arithmetic
    * Loads
    * JALR
  * S-Type
  * B-Type
  * U-Type
  * J-Type
* Detects unknown/invalid instructions
* Proper immediate extraction and sign extension
* Byte-addressable memory model for instruction storage
* Modular decoder implementation
* Unit tests for each instruction format
* Makefile support for building, testing, debugging, and memory checking

---

## Supported Instructions

### R-Type

* ADD
* SUB
* AND
* OR
* XOR
* SLL
* SRL
* SRA
* SLT
* SLTU

### I-Type (Arithmetic)

* ADDI
* ANDI
* ORI
* XORI
* SLTI
* SLTIU
* SLLI
* SRLI
* SRAI

### I-Type (Load)

* LB
* LH
* LW
* LBU
* LHU

### S-Type

* SB
* SH
* SW

### B-Type

* BEQ
* BNE
* BLT
* BGE
* BLTU
* BGEU

### U-Type

* LUI
* AUIPC

### J-Type

* JAL

### I-Type (Jump)

* JALR

---

## Project Structure

```text
riscv-decoder/
├── include/
│   ├── common.h
│   ├── decoder.h
│   └── memory.h
│
├── src/
│   ├── main.c
│   ├── decoder.c
│   └── memory.c
│
├── test/
│   ├── test_decoder.c
│   └── programs/
│       ├── r_type.hex
│       ├── i_type.hex
│       ├── branch.hex
│       └── mixed.hex
│
├── Makefile
├── README.md
└── .gitignore
```

---

## Building

Compile the decoder:

```bash
make all
```

This builds the executable (`run`) and automatically runs the decoder on all supplied test programs.

---

## Running

To decode a specific program:

```bash
./run test/programs/mixed.hex
```

Example output:

```text
RISC-V RV32I Instruction Decoder
================================

Loaded 18 instructions from test/programs/mixed.hex

Addr              Hex           Assembly
----------     ---------- -------------------------
0x00000000      123450B7        lui     x1, 305418240
0x00000004      00001117        auipc   x2, 4096
0x00000008      010000EF        jal     x1, 16
0x0000000C      003100B3        add     x1, x2, x3
...
```

---

## Running Unit Tests

Compile and execute the decoder tests:

```bash
make test
```

---

## Memory Leak Detection

Run the decoder under Valgrind:

```bash
make valgrind
```

This checks all provided test programs for:

* Memory leaks
* Invalid reads/writes
* Use of uninitialized memory
* Invalid memory frees

---

## Debugging

Launch the decoder inside GDB:

```bash
make debug
```

The debugger starts with:

```text
./run test/programs/mixed.hex
```

already configured as the program and its command-line argument.

---

## Build Targets

| Command         | Description                                          |
| --------------- | ---------------------------------------------------- |
| `make all`      | Build the decoder and run all provided test programs |
| `make test`     | Compile and execute unit tests                       |
| `make valgrind` | Run Valgrind on all supplied test programs           |
| `make debug`    | Start GDB with `mixed.hex` loaded                    |
| `make clean`    | Remove generated executables                         |

---

## Technologies Used

* GCC
* Makefile
* GDB
* Valgrind

---

## Learning Outcomes

This project demonstrates:

* Bit manipulation and masking
* RISC-V instruction encoding and decoding
* Immediate reconstruction and sign extension
* Modular software design
* File I/O
* Byte-addressable memory simulation
* Unit testing
* Build automation using Make
* Debugging with GDB
* Memory analysis using Valgrind

---

## License

Author: Abdul Rafay
