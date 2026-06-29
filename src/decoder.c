#include "../include/decoder.h"
#include "../include/common.h"
#include <stdio.h>
#include <string.h>

// Top Function for Instruction Decoding
decoded_instr_t decode_instr(uint32_t instr){
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    decoded_instr_t unknown;
    unknown.valid = false;
    switch (opcode){
        case OP_R_TYPE: return decode_r_type(instr); break;
        case OP_I_TYPE: return decode_i_type(instr); break;
        case OP_LOAD:   return decode_i_type(instr); break;
        case OP_BRANCH: return decode_b_type(instr); break;
        case OP_JAL:    return decode_j_type(instr); break;
        case OP_JALR:   return decode_i_type(instr); break;
        case OP_LUI:    return decode_u_type(instr); break;
        case OP_AUIPC:  return decode_u_type(instr); break;
        case OP_STORE:  return decode_s_type(instr); break;
        default: return unknown; break;
    }
}

// R-Type Decode
decoded_instr_t decode_r_type(uint32_t instr){

    decoded_instr_t r_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    r_type.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr,14,12);
    uint32_t funct7 = EXTRACT_BITS(instr,31,25);
    r_type.rd  = EXTRACT_BITS(instr,11,7);
    r_type.rs1 = EXTRACT_BITS(instr,19,15);
    r_type.rs2 = EXTRACT_BITS(instr,24,20);

// funct3 and funct7 check
    if (funct3 == 0x0){
        if (funct7 == 0x00) {
            strcpy(r_type.op, "add"); 
            r_type.valid = true;
        }
        else if (funct7 == 0x20){
            strcpy(r_type.op, "sub"); 
            r_type.valid = true;
        } 
        else r_type.valid = false;  
    }

    else if(funct3 == 0x1 && funct7 == 0x00){
        strcpy(r_type.op, "sll"); 
        r_type.valid = true;
    }

    else if(funct3 == 0x2 && funct7 == 0x00){
        strcpy(r_type.op, "slt"); 
        r_type.valid = true;
    }

    else if(funct3 == 0x3 && funct7 == 0x00){
        strcpy(r_type.op, "sltu"); 
        r_type.valid = true;
    }

    else if(funct3 == 0x4 && funct7 == 0x00){
        strcpy(r_type.op, "xor"); 
        r_type.valid = true;
    }

    else if(funct3 == 0x5){
        if (funct7 == 0x00) {
            strcpy(r_type.op, "srl"); 
            r_type.valid = true;
        }
        else if (funct7 == 0x20){
            strcpy(r_type.op, "sra"); 
            r_type.valid = true;
        } 
        else r_type.valid = false;   
    }

    else if(funct3 == 0x6 && funct7 == 0x00){
        strcpy(r_type.op, "or"); 
        r_type.valid = true;
    }
    else if(funct3 == 0x7 && funct7 == 0x00){
        strcpy(r_type.op, "and"); 
        r_type.valid = true;
    }
    else r_type.valid = false;
    return r_type;
}
// I-Type Decode
decoded_instr_t decode_i_type(uint32_t instr){
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    switch (opcode){
        case OP_I_TYPE: return decode_arithmetic(instr); 
        case OP_LOAD:   return decode_load(instr); 
        case OP_JALR:   return decode_jump(instr); 
        default:
        {
            decoded_instr_t unknown;
            unknown.valid = false;
            return unknown;
        }
    } 
}
// S-Type Decode
decoded_instr_t decode_s_type(uint32_t instr){

    decoded_instr_t s_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    s_type.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr,14,12);
    s_type.rs1 = EXTRACT_BITS(instr,19,15);
    s_type.rs2 = EXTRACT_BITS(instr,24,20);

    // Extract 12-bit immediate
    uint32_t imm11_5 = EXTRACT_BITS(instr,31,25);
    uint32_t imm4_0  = EXTRACT_BITS(instr,11,7);

    uint32_t imm = (imm11_5 << 5) | imm4_0;

    // Sign extend 12-bit immediate
    s_type.imm = (int32_t)(imm << 20) >> 20;
    switch (funct3){
        case 0x0: strcpy(s_type.op, "sb"); s_type.valid = true; break;
        case 0x1: strcpy(s_type.op, "sh"); s_type.valid = true; break;
        case 0x2: strcpy(s_type.op, "sw"); s_type.valid = true; break;
        default: s_type.valid = false;
    }
    return s_type;
}

// B-Type Decode
decoded_instr_t decode_b_type(uint32_t instr){

    decoded_instr_t b_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    b_type.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr,14,12);
    b_type.rs1 = EXTRACT_BITS(instr,19,15);
    b_type.rs2 = EXTRACT_BITS(instr,24,20);
    // Extract immediate
    uint32_t imm12   = EXTRACT_BITS(instr, 31, 31);
    uint32_t imm11   = EXTRACT_BITS(instr, 7, 7);
    uint32_t imm10_5 = EXTRACT_BITS(instr, 30, 25);
    uint32_t imm4_1  = EXTRACT_BITS(instr, 11, 8);

    uint32_t imm = (imm12   << 12) | (imm11   << 11) | (imm10_5 << 5) | (imm4_1  << 1);
    // Sign-Extend immediate
    b_type.imm = (int32_t)(imm << 19) >> 19;
    
    switch (funct3){
        case 0x0: strcpy(b_type.op, "beq");  b_type.valid = true; break;
        case 0x1: strcpy(b_type.op, "bne");  b_type.valid = true; break;
        case 0x4: strcpy(b_type.op, "blt");  b_type.valid = true; break;
        case 0x5: strcpy(b_type.op, "bge");  b_type.valid = true; break;
        case 0x6: strcpy(b_type.op, "bltu"); b_type.valid = true; break;
        case 0x7: strcpy(b_type.op, "bgeu"); b_type.valid = true; break;
        default: b_type.valid = false; break;
    }
    return b_type;
}

// U-Type Decode
decoded_instr_t decode_u_type(uint32_t instr){
    decoded_instr_t u_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    u_type.opcode = opcode;
    u_type.rd  = EXTRACT_BITS(instr,11,7);
    u_type.imm = EXTRACT_BITS(instr,31,12) << 12;

    switch (opcode){
        case OP_AUIPC: strcpy(u_type.op, "auipc"); u_type.valid = true; break;
        case OP_LUI:   strcpy(u_type.op, "lui");   u_type.valid = true; break;
        default:
        {
            decoded_instr_t unknown;
            unknown.valid = false;
            return unknown;
        }
    }
    return u_type;
}

// J-Type Decode
decoded_instr_t decode_j_type(uint32_t instr){
    decoded_instr_t j_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    j_type.opcode = opcode;
    j_type.rd  = EXTRACT_BITS(instr,11,7);
    j_type.valid = true;
    strcpy(j_type.op, "jal");
    
    // Extract immediate
    uint32_t imm20    = EXTRACT_BITS(instr, 31, 31);
    uint32_t imm19_12 = EXTRACT_BITS(instr, 19, 12);
    uint32_t imm11    = EXTRACT_BITS(instr, 20, 20);
    uint32_t imm10_1  = EXTRACT_BITS(instr, 30, 21);
    uint32_t imm = (imm20    << 20) | (imm19_12 << 12) | (imm11    << 11) | (imm10_1  << 1);
    // Sign-Extend immediate
    j_type.imm = (int32_t)(imm << 11) >> 11;

    return j_type;
}

//I-type sub functions

decoded_instr_t decode_arithmetic(uint32_t instr){
    decoded_instr_t i_type;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    i_type.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr, 14, 12);
    uint32_t funct7 = EXTRACT_BITS(instr, 31, 25);

    i_type.rd  = EXTRACT_BITS(instr, 11, 7);
    i_type.rs1 = EXTRACT_BITS(instr, 19, 15);
    // Extract and Sign-Extend immediate
    i_type.imm = (int32_t)(EXTRACT_BITS(instr, 31, 20) << 20) >> 20;

    switch (funct3)
    {
        case 0x0:
            strcpy(i_type.op, "addi");
            i_type.valid = true;
            break;

        case 0x2:
            strcpy(i_type.op, "slti");
            i_type.valid = true;
            break;

        case 0x3:
            strcpy(i_type.op, "sltiu");
            i_type.valid = true;
            break;

        case 0x4:
            strcpy(i_type.op, "xori");
            i_type.valid = true;
            break;

        case 0x6:
            strcpy(i_type.op, "ori");
            i_type.valid = true;
            break;

        case 0x7:
            strcpy(i_type.op, "andi");
            i_type.valid = true;
            break;

        case 0x1:
            strcpy(i_type.op, "slli");
            i_type.imm = EXTRACT_BITS(instr, 24, 20);   // shamt
            i_type.valid = true;
            break;

        case 0x5:
            if (funct7 == 0x00)
            {
                strcpy(i_type.op, "srli");
                i_type.imm = EXTRACT_BITS(instr, 24, 20);
                i_type.valid = true;
            }
            else if (funct7 == 0x20)
            {
                strcpy(i_type.op, "srai");
                i_type.imm = EXTRACT_BITS(instr, 24, 20);
                i_type.valid = true;
            }
            break;

        default:
            i_type.valid = false;
    }

    return i_type;
}

decoded_instr_t decode_load(uint32_t instr){
    decoded_instr_t load;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    load.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr, 14, 12);

    load.rd  = EXTRACT_BITS(instr, 11, 7);
    load.rs1 = EXTRACT_BITS(instr, 19, 15);
    // Extract and Sign-Extend immediate
    load.imm = (int32_t)(EXTRACT_BITS(instr, 31, 20) << 20) >> 20;
    
    switch (funct3)
    {
        case 0x0: strcpy(load.op, "lb");  load.valid = true; break;
        case 0x1: strcpy(load.op, "lh");  load.valid = true; break;
        case 0x2: strcpy(load.op, "lw");  load.valid = true; break;
        case 0x4: strcpy(load.op, "lbu"); load.valid = true; break;
        case 0x5: strcpy(load.op, "lhu"); load.valid = true; break;
        default:  load.valid = false;
    }
    
    return load;
}

decoded_instr_t decode_jump(uint32_t instr){
    decoded_instr_t jump;
    opcode_t opcode = EXTRACT_BITS(instr,6,0);
    jump.opcode = opcode;
    uint32_t funct3 = EXTRACT_BITS(instr, 14, 12);

    jump.rd  = EXTRACT_BITS(instr, 11, 7);
    jump.rs1 = EXTRACT_BITS(instr, 19, 15);
    // Extract and Sign-Extend immediate
    jump.imm = (int32_t)(EXTRACT_BITS(instr, 31, 20) << 20) >> 20;

    if (funct3 == 0x0)
    {
        strcpy(jump.op, "jalr");
        jump.valid = true;
    }
    else
    {
        jump.valid = false;
    }

    return jump;
}

//Instruction to Assembly converter
void instr_to_string(decoded_instr_t instr){
    switch (instr.opcode){
        case OP_R_TYPE: printf("%-7s x%d, x%d, x%d\n", instr.op, instr.rd, instr.rs1, instr.rs2); break;
        case OP_I_TYPE: printf("%-7s x%d, x%d, 0x%X\n", instr.op, instr.rd, instr.rs1, instr.imm); break;
        case OP_LOAD:   printf("%-7s x%d, %d(x%d)\n", instr.op, instr.rd, instr.imm, instr.rs1); break;
        case OP_JALR:   printf("%-7s x%d, %d(x%d)\n", instr.op, instr.rd, instr.imm, instr.rs1); break;
        case OP_STORE:  printf("%-7s x%d, %d(x%d)\n", instr.op, instr.rs2, instr.imm, instr.rs1); break;
        case OP_BRANCH: printf("%-7s x%d, x%d, 0x%X\n", instr.op, instr.rs1, instr.rs2, instr.imm); break;
        case OP_LUI:    printf("%-7s x%d, 0x%X\n", instr.op, instr.rd, instr.imm); break;
        case OP_AUIPC:  printf("%-7s x%d, 0x%X\n", instr.op, instr.rd, instr.imm); break;
        case OP_JAL:    printf("%-7s x%d, 0x%X\n", instr.op, instr.rd, instr.imm); break;
    }
}