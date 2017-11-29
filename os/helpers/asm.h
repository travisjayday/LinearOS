#ifndef ASM_H_ 
#define ASM_H_ 

#define interrupt(arg) __asm__("int %0\n" : : "N"((arg)) : "cc", "memory")

static inline void outb(uint16_t port, uint8_t val);

static inline uint8_t inb(uint16_t port); 

static inline void io_wait(void);

#include "src/asm.c"

#endif
