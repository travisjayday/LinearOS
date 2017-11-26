#ifndef WIDGET_H_
#define WIDGET_H_

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height; 
	uint8_t bg_color; 
	uint8_t flags; 		/*  0           1   0 0 0 0 0 0 */ 
			        // text mode	keyboard enable
	void (*keyboard_handler)();	
	char* text;
	uint8_t textidx; 
	uint8_t txt_color; 
	uint8_t bufsize; 
} Widget; 

#define SELECTED_BORDER_THICK 3
#define SELECTED_COLOR VGA_COLOR_LIGHT_RED
uint8_t WidgetCount = 0; 
Widget* WidgetList[10]; 
uint8_t SelectedWidget = 0; 

Widget widget_create1(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t _bg, uint8_t _f);

Widget widget_create2(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h);

void widget_register(Widget* wid);

void widget_init_textmode(Widget* wid, uint8_t _bufsize);

// inits textmode and keyboard mode
void widget_init_keyboard(Widget* wid, void (*key_handler)());

void widget_set_text(Widget* wid, char* text);

void widget_append_char(Widget* wid, char c);

#include "src/widget.c"
#endif
