# Decoder Design

## Overview

The RISC-V RV32I Instruction Decoder is implemented as a modular application that separates memory management, instruction decoding, and output formatting into independent components. This separation of concerns makes the code easier to understand, maintain, test, and extend.

The decoder reads hexadecimal machine instructions from an input file, stores them in a simulated byte-addressable memory, reconstructs 32-bit instructions, decodes them according to the RV32I specification, and prints the corresponding assembly instructions.

---

# Overall Architecture

```text
                 Hex File (.hex)
                        │
                        ▼
               Load Instructions
                        │
                        ▼
       Byte-Addressable Memory (Little Endian)
                        │
                        ▼
            Read 32-bit Instruction Word
                        │
                        ▼
               Extract Opcode [6:0]
                        │
                        ▼
            Top-Level Instruction Decoder
                        │
     ┌──────────┬──────────┬──────────┐
     │          │          │          │
     ▼          ▼          ▼          ▼
   R-Type    I-Type     S-Type    Other Types
     │          │          │
     ▼          ▼          ▼
              Sub Decoder Functions
                        │
                        ▼
             decoded_instr_t Structure
                        │
                        ▼
              Assembly Formatter
                        │
                        ▼
                 Console Output
```

---

# Project Organization

The project is divided into three independent modules.

## 1. Memory Module

Files:

- `memory.c`
- `memory.h`

Responsibilities:

- Read hexadecimal instructions from an input file.
- Store instructions in simulated byte-addressable memory.
- Reconstruct 32-bit instruction words from four bytes.
- Provide memory access functions for instruction fetching.

The decoder never directly accesses the memory array. All memory operations are performed through this module.

---

## 2. Decoder Module

Files:

- `decoder.c`
- `decoder.h`

Responsibilities:

- Decode every supported RV32I instruction.
- Extract register fields.
- Reconstruct immediate values.
- Perform sign extension.
- Determine the instruction mnemonic.
- Return a decoded instruction structure.

Each instruction format has its own dedicated decoding function.

---

## 3. Main Module

File:

- `main.c`

Responsibilities:

- Parse command-line arguments.
- Load the instruction file.
- Iterate through every instruction.
- Call the decoder.
- Print decoded assembly.
- Report decoding statistics.

---

# Memory Organization

## Byte-Addressable Memory

The decoder simulates a byte-addressable memory similar to real RISC-V hardware.

Instead of storing instructions as 32-bit integers, each instruction is divided into four bytes.

For example,

```
0x12345678
```

is stored as

| Address | Value |
|---------|------|
| A | 0x78 |
| A+1 | 0x56 |
| A+2 | 0x34 |
| A+3 | 0x12 |

This follows the **little-endian** convention used by RISC-V.

When the decoder needs an instruction, the memory module reconstructs the original word by reading four consecutive bytes.

Conceptually,

```c
instruction =
      memory[address]
    | memory[address + 1] << 8
    | memory[address + 2] << 16
    | memory[address + 3] << 24;
```

This implementation closely models how instructions are fetched on real processors while keeping the decoding logic independent of memory representation.

---

# Decoder Design

The decoder follows a hierarchical design.

The first step is extracting the **7-bit opcode**.

```
instruction
      │
      ▼
Extract bits [6:0]
      │
      ▼
Switch(opcode)
```

The opcode determines which decoding function should process the instruction.

```
decode_instruction()
        │
        ▼
   Extract Opcode
        │
        ▼
┌──────────────────────┐
│ switch(opcode)       │
└──────────────────────┘
        │
 ┌──────┼──────────────┐
 │      │      │       │
 ▼      ▼      ▼       ▼
R      I      S       B
 │      │      │       │
 ▼      ▼      ▼       ▼
Dedicated decoder functions
```

Each decoder extracts only the fields required for its instruction format.

---

# Instruction-Specific Decoders

Separate decoding functions are implemented for each RV32I instruction format.

- `decode_r_type()`
- `decode_i_type()`
- `decode_s_type()`
- `decode_b_type()`
- `decode_u_type()`
- `decode_j_type()`

The I-Type decoder is further divided because three different instruction classes share the same encoding format.

```
         decode_i_type()
                │
 ┌────────────────────┐───────────┐
 │                    │           │
 ▼                    ▼           ▼
Arithmetic          Load        JALR
 │                    │           │
 ▼                    ▼           ▼
decode_arithmetic() decode_load() decode_jump()
```

This organization minimizes duplicated code and keeps each function focused on a single instruction class.

---

# Instruction Representation

Every decoded instruction is represented using a common structure.

```c
typedef struct {
    char op[6];
    opcode_t opcode;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
    bool valid;
} decoded_instr_t;
```

This structure stores all information required to represent any RV32I instruction.

| Field | Description |
|--------|-------------|
| `op` | Assembly mnemonic |
| `opcode` | Instruction opcode |
| `rd` | Destination register |
| `rs1` | Source register 1 |
| `rs2` | Source register 2 |
| `imm` | Sign-extended immediate |
| `valid` | Indicates whether decoding succeeded |

Returning a common structure simplifies testing and separates decoding from output formatting.

---

# Bit Extraction

To simplify field extraction, the decoder uses a reusable macro.

```c
EXTRACT_BITS(value, high, low)
```

Instead of repeatedly writing masks and shifts, every field is extracted through this macro.

Example:

```c
rd     = EXTRACT_BITS(instr, 11, 7);
rs1    = EXTRACT_BITS(instr, 19, 15);
funct3 = EXTRACT_BITS(instr, 14, 12);
```

Using a common macro improves readability and reduces implementation errors.

---

# Immediate Reconstruction

Each RISC-V instruction format stores immediate values differently.

The decoder reconstructs immediates according to the RV32I specification.

| Format | Immediate Layout |
|---------|------------------|
| I-Type | bits `[31:20]` |
| S-Type | `{31:25,11:7}` |
| B-Type | `{31,7,30:25,11:8,0}` |
| U-Type | `{31:12}` shifted left by 12 |
| J-Type | `{31,19:12,20,30:21,0}` |

Signed immediates are sign-extended before being stored in the decoded instruction structure.

---

# Assembly Formatting

The decoder separates instruction decoding from instruction presentation.

Each decoder function returns a populated `decoded_instr_t` structure without printing anything.

Once decoding is complete, `instr_to_string()` converts the structure into human-readable RISC-V assembly.

Different instruction formats require different output syntax.

| Type | Example Output |
|------|----------------|
| R-Type | `add x1, x2, x3` |
| I-Type | `addi x1, x2, 5` |
| Load | `lw x1, 4(x2)` |
| Store | `sw x3, 8(x1)` |
| Branch | `beq x1, x2, 16` |
| U-Type | `lui x1, 0x12345000` |
| J-Type | `jal x1, 32` |

Keeping formatting separate from decoding provides several advantages:

- Decoder functions remain focused only on instruction interpretation.
- Output formatting is centralized in a single function.
- Additional output formats can be added without modifying the decoder.

---

# Error Handling

The decoder is designed to continue execution even when invalid instructions are encountered.

If an opcode or function field does not correspond to a valid RV32I instruction,

- `valid` is set to `false`.
- The instruction is reported as `UNKNOWN`.
- Decoding continues with the next instruction.

This makes the decoder robust against malformed input files.

---

# Testing Strategy

The project includes several levels of testing.

## Unit Tests

Individual decoder functions are tested independently using known instruction encodings.

## Instruction Files

Four hexadecimal programs verify decoder correctness.

- `r_type.hex`
- `i_type.hex`
- `branch.hex`
- `mixed.hex`

The mixed test file contains both valid and intentionally invalid instructions to verify unknown instruction detection.

## Memory Testing

Valgrind is used to detect:

- Memory leaks
- Invalid reads
- Invalid writes
- Improper memory usage

## Debugging

GDB support is provided through the Makefile to allow step-by-step execution of the decoder.

---

# Key Design Decisions

## Modular Architecture

Each instruction format has its own decoding function.

Benefits:

- Easier maintenance
- Better readability
- Simpler debugging
- Easier unit testing

---

## Common Return Structure

Every decoder returns the same `decoded_instr_t` structure.

Benefits:

- Consistent interface
- Easy formatting
- Easier testing
- Future extensibility

---

## Opcode-Based Dispatch

Instructions are classified using the opcode before decoding.

Benefits:

- Fast instruction selection
- Mirrors the RISC-V ISA organization
- Avoids unnecessary comparisons

---

## License

Author: Abdul Rafay

