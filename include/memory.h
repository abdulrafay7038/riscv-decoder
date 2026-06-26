#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h> 
#include <stddef.h>

uint32_t read_mem (uint8_t *memory, int addr);

int load_hex_file(const char *filename, uint8_t *memory, size_t mem_size);

#endif