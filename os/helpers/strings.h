#ifndef STRINGS_H_
#define STRINGS_H_

#include "math.h"

uint32_t strlen(char* str); 
char* int2str(uint32_t num); 
uint32_t str2int(char* str); 
char* int2hex(uint32_t num); 
void strcpy(char* dest, char* str);
char* strcat(char* dest, char* src); 
char* format_segmented_addr(uint32_t selector, uint32_t addr);

#include "src/strings.c"
#endif

