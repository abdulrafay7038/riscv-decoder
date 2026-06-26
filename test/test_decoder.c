#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/decoder.h"

int tests_passed = 0;
int tests_failed = 0;

void report(const char *name, int pass)
{
    if (pass)
    {
        printf("[PASS] %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s\n", name);
        tests_failed++;
    }
}

void test_r_type()
{
    decoded_instr_t instr = decode_r_type(0x00C58533);   // add x10,x11,x12

    report("R-Type ADD",
        strcmp(instr.op, "add") == 0 &&
        instr.opcode == OP_R_TYPE &&
        instr.rd == 10 &&
        instr.rs1 == 11 &&
        instr.rs2 == 12 &&
        instr.valid);
}

void test_i_type()
{
    decoded_instr_t instr = decode_i_type(0x00500093);   // addi x1,x0,5

    report("I-Type ADDI",
        strcmp(instr.op, "addi") == 0 &&
        instr.opcode == OP_I_TYPE &&
        instr.rd == 1 &&
        instr.rs1 == 0 &&
        instr.imm == 5 &&
        instr.valid);
}

void test_load()
{
    decoded_instr_t instr = decode_i_type(0x0040A603);   // lw x12,4(x1)

    report("Load LW",
        strcmp(instr.op, "lw") == 0 &&
        instr.opcode == OP_LOAD &&
        instr.rd == 12 &&
        instr.rs1 == 1 &&
        instr.imm == 4 &&
        instr.valid);
}

void test_store()
{
    decoded_instr_t instr = decode_s_type(0x00C02023);   // sw x12,0(x0)

    report("Store SW",
        strcmp(instr.op, "sw") == 0 &&
        instr.opcode == OP_STORE &&
        instr.rs1 == 0 &&
        instr.rs2 == 12 &&
        instr.imm == 0 &&
        instr.valid);
}

void test_branch()
{
    decoded_instr_t instr = decode_b_type(0x00208463);   // beq x1,x2,8

    report("Branch BEQ",
        strcmp(instr.op, "beq") == 0 &&
        instr.opcode == OP_BRANCH &&
        instr.rs1 == 1 &&
        instr.rs2 == 2 &&
        instr.imm == 8 &&
        instr.valid);
}

void test_u_type()
{
    decoded_instr_t instr = decode_u_type(0x123450B7);   // lui x1,0x12345000

    report("U-Type LUI",
        strcmp(instr.op, "lui") == 0 &&
        instr.opcode == OP_LUI &&
        instr.rd == 1 &&
        instr.imm == 0x12345000 &&
        instr.valid);
}

void test_j_type()
{
    decoded_instr_t instr = decode_j_type(0x010000EF);   // jal x1,16

    report("J-Type JAL",
        strcmp(instr.op, "jal") == 0 &&
        instr.opcode == OP_JAL &&
        instr.rd == 1 &&
        instr.imm == 16 &&
        instr.valid);
}

void test_jalr()
{
    decoded_instr_t instr = decode_i_type(0x000080E7);   // jalr x1,0(x1)

    report("I-Type JALR",
        strcmp(instr.op, "jalr") == 0 &&
        instr.opcode == OP_JALR &&
        instr.rd == 1 &&
        instr.rs1 == 1 &&
        instr.imm == 0 &&
        instr.valid);
}

int main(void)
{
    test_r_type();
    test_i_type();
    test_load();
    test_store();
    test_branch();
    test_u_type();
    test_j_type();
    test_jalr();

    printf("\n=========================\n");
    printf("Passed : %d\n", tests_passed);
    printf("Failed : %d\n", tests_failed);
    printf("=========================\n");

    return 0;
}