#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "../include/memory.h"
#include "../include/decoder.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <hex_file>\n", argv[0]);
        return 1;
    }
    uint8_t *mem = malloc(1024*sizeof(uint8_t)); // 1KB memory
    if (!mem) {
        perror("Cannot allocate memory");
        return 1;
    }
    int n = load_hex_file(argv[1], mem, 1024);
    if (n < 0) {
        printf("Failed to load hex file\n");
        free(mem);
        return 1;
    }
    
    printf("RISC-V RV32I Instruction Decoder\n");
    printf("================================\n");
    printf("Loaded %d instructions from %s\n", n, argv[1]);
    printf("\nAddr\t\t  Hex\t\tAssembly\n");
    printf("----------     ---------- -------------------------\n");

    uint32_t i = 0;
    int valid = 0;
    int unknown = 0;

    while (i<n*4){
        uint32_t packed_instr = (mem[i] | mem[i+1] << 8 | mem[i+2] << 16 | mem[i+3] << 24);
        decoded_instr_t decoded_instr = decode_instr(packed_instr);

        char *instr_string;
        if (decoded_instr.valid == true){
            valid +=1;
            instr_string = instr_to_string(decoded_instr);
        }
        else if(decoded_instr.valid == false){
            unknown +=1;
            instr_string = "UNKNOWN";
        }

        printf("0x%08X\t%08X\n", i, packed_instr);
        i +=4;
    }
    printf("\nDecoded %d instructions (%d valid, %d unknown)\n", n, valid, unknown);
}    