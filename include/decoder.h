#include<stdint.h>
#include<stdbool.h>

decoded_instr_t decode_r_type(uint32_t instr);
decoded_instr_t decode_i_type(uint32_t instr);
decoded_instr_t decode_s_type(uint32_t instr);
decoded_instr_t decode_b_type(uint32_t instr);
decoded_instr_t decode_u_type(uint32_t instr);
decoded_instr_t decode_j_type(uint32_t instr);


typedef struct {    
    char op[10];
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
    bool valid;
} decoded_instr_t;

typedef enum { 
    OP_R_TYPE  = 0x33,  // Register-register 
    OP_I_TYPE  = 0x13,  // Immediate arithmetic 
    OP_LOAD    = 0x03,  // Load from memory 
    OP_STORE   = 0x23,  // Store to memory 
    OP_BRANCH  = 0x63,  // Conditional branch 
    OP_JAL     = 0x6F,  // Jump and link 
    OP_JALR    = 0x67,  // Jump and link register 
    OP_LUI     = 0x37,  // Load upper immediate 
    OP_AUIPC   = 0x17,  // Add upper immediate to PC 
} opcode_t; 