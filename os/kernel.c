asm (".code32"); 
#include <stdint.h>
#include "helpers/asm.h"
#include "mem_mgmt.h"
#include "helpers/strings.h"
#include "graphics/drawing.h"
#include "init/interrupts.h"
#include "init/keyboard.h"
#include "init/pit_driver.h"
#include "widget.h"

// does nothing for now...
void teletype_keyboard_callback()
{}

//Play sound using built in speaker
/*static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }*/

extern void kernel_main()
{
	// initialize important OS stuff
	
	allocate_vbuffer(); 	// allocates video buffer
	fill_buffer_pattern(STRIPES, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY); // fills video buffer with stripes
	flip_buffers();			// moves video buffer into physical vram
	isr_traps_init();		// installs cpu exception traps 
	init_keyboard();		// install keyboard isr
	//init_pit(); 			// installs pit isr
	enable_hardware_interrupts(); // unmasks keyboard isr 
	asm ("sti"); 			// enables interrputs

	// test drawing functions
	char* name = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	draw_char(20, 20, 'F', VGA_COLOR_BLUE); 
	draw_string(10, 50, name, VGA_COLOR_BROWN); 
	draw_rect(20, 20, 100, 100, VGA_COLOR_WHITE);	
	
	// creates two rectagle widgets
	Widget console = widget_create1(10, 100, 300, 80, VGA_COLOR_BLUE, 0); 
	Widget time = widget_create1(200, 5, 110, 30, VGA_COLOR_GREEN, 0); 

	// adds widget to global WidgetList
	widget_register(&console); 
	widget_register(&time); 

	// enables keyboard input for widget, callback will be called if widget is selected
	widget_init_keyboard(&console, teletype_keyboard_callback);

	// enables text output (allocates 50 bytes text buffer)
	widget_init_textmode(&time, 50); 
		
	while (1)
	{
		// set text of console to the keyboard input buffer
		widget_set_text(&console, stdin); 
		/*if ((uint32_t)*system_time % 100 == 0)
		{
			widget_set_text(&time, int2str(s)); 
			s++; 
		}*/

		// update all widget (graphically redraw everything) 
		widget_update_all(); 
	
		// draws sprite to screen 
		#include "../tools/graphics/examples/smiley.c"
		draw_sprite(0, 0, sprite_player); 			

		// moves video buffer into physical vram 
		flip_buffers(); 
	}
}
