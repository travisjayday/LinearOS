#ifndef TTY_H_
#define TTY_H_

#define STDOUT_SIZE 256

uint8_t stdout_i = 0; 
// stdout buffer
char stdout[STDOUT_SIZE]; 

void putc(char c); 
void println(char* str); 

#include "src/tty.c"

#endif
