#ifndef STRINGS_H_
#define STRINGS_H_

#include "math.h"

uint32_t strlen(uint8_t* str); 
uint8_t* int2str(uint32_t num); 
uint32_t str2int(uint8_t* str); 
uint8_t* int2hex(uint32_t num); 

#include "src/strings.c"
#endif

