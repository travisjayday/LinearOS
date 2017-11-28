#ifndef MEM_MGMT_H_
#define MEM_MGMT_H_

static void* free_memory = (void*) 0xCE00;

// TODO: fix this implemenatation
// allocates memory and returns pointer to it (preliminary)
void* malloc(uint32_t size);

void linear_free(uint32_t size); 

// copies size bytes of memory from src to dest byte by byte
void memcpy8_t(void* dest, void* src, uint32_t size);

// copies size bytes of memory from src to dest dword by dword
void memcpy32_t(void* dest, void* src, uint32_t size);

// sets memory dword by dword to val
void memset32_t(void* dest, uint32_t val, uint32_t size);

#include "src/mem_mgmt.c"
#endif

