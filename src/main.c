#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "../include/memory.h"
#include "../include/decoder.h"

int main(int argc, char *argv[]){
    // Check Argument Count
    if (argc != 2) {
        printf("Usage: %s <hex_file>\n", argv[0]);
        return 1;
    }
    // Allocate Memory
    uint8_t *mem = malloc(1024*sizeof(uint8_t)); // 1KB memory
    if (!mem) {
        perror("Cannot allocate memory");
        return 1;
    }
    // Load data from Hex file
    int n = load_hex_file(argv[1], mem, 1024);
    if (n < 0) {
        printf("Failed to load hex file\n");
        free(mem);
        return 1;
    }
    
    printf("Loaded %d instructions from %s\n", n, argv[1]);
    printf("\nAddr\t\t  Hex\t\t       Assembly\n");
    printf("----------     ----------      -------------------------\n");

    int i = 0;
    int valid = 0;
    int unknown = 0;
    
    // Read memory and print assembly
    while (i<n*4){
        uint32_t instr = read_mem(mem, i);
        decoded_instr_t decoded_instr = decode_instr(instr);

        if (decoded_instr.valid == true){
            valid +=1;
            printf("0x%08X\t%08X\t", i, instr);
            instr_to_string(decoded_instr);
        }
        else if(decoded_instr.valid == false){
            unknown +=1;
            printf("0x%08X\t%08X\tUNKNOWN\n", i, instr);
        }
        i +=4;
    }
    printf("\nDecoded %d instructions (%d valid, %d unknown)\n\n", n, valid, unknown);
    free(mem);
    return 0;
}    