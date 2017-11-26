#ifndef MEM_MGMT_H_
#define MEM_MGMT_H_

static void* free_memory = (void*) 0xCE00;//(void*)(0x200000); // heap is 10kiB large 

// allocates memory and returns pointer to it
void* malloc(uint32_t size);

void linear_free(uint32_t size); 

void memcpy8_t(void* dest, void* src, uint32_t size);

void memcpy32_t(uint32_t* dest, uint32_t* src, uint32_t size);

void memset32_t(uint32_t* dest, uint32_t val, uint32_t size);

#include "src/mem_mgmt.c"
#endif

