#ifndef ASM_H_ 
#define ASM_H_ 


static inline void outb(uint16_t port, uint8_t val);

static inline uint8_t inb(uint16_t port); 

#include "src/asm.c"

#endif
