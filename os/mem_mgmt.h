#ifndef MEM_MGMT_H_
#define MEM_MGMT_H_

uint8_t* free_memory = (uint8_t*)(0x200000); // heap is 10kiB large 

// allocates memory and returns pointer to it
uint8_t* malloc(uint32_t size);

void linear_free(uint32_t size); 

void memcpy8_t(uint8_t* dest, uint8_t* src, uint32_t size);

void memcpy32_t(uint32_t* dest, uint32_t* src, uint32_t size);

void memset32_t(uint32_t* dest, uint32_t val, uint32_t size);

#include "src/mem_mgmt.c"
#endif

