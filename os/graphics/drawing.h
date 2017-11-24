#ifndef DRAWING_H_
#define DRAWING_H_

// vga constants for mode 0x13
#define VBUFFER_SIZE 200 * 320
#define VGA_WIDTH 320

// vga memory pointer
static uint8_t* VGA_MEM = (uint8_t*)0xA0000;

// video buffer memory pointer (offscreen buffer for vsync) 
uint8_t* VBUFFER_MEM; 

// nibble vga color constants
enum VGA_COLOR {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

enum PATTERN {
	SOLID = 0,
	STRIPES = 1
};

#include "data/font.dat"

void allocate_vbuffer();

void fill_buffer_pattern(uint8_t pattern, uint8_t color1, uint8_t color2); 
void fill_buffer(uint8_t color); 

void flip_buffers(); 

void draw_char(uint16_t _x, uint16_t _y, uint8_t c, uint8_t color);

void draw_string(uint16_t _x, uint16_t _y, uint8_t* str, uint8_t color);

void draw_rect(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t color);

void draw_rect_border(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t color, uint8_t thickness, uint8_t bcolor);

void draw_sprite(uint16_t _x, uint16_t _y, uint8_t* sprite); 

#include "src/drawing.c"
#endif 
