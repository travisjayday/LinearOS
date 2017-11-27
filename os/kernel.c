asm (".code32"); 
#include <stdint.h>
#include "mem_mgmt.h"
#include "helpers/strings.h"
#include "graphics/drawing.h"
#include "init/interrupts.h"
#include "init/keyboard.h"
#include "init/pit_driver.h"
#include "widget.h"


static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
} 

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
	asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
	return ret;
}
void teletype_keyboard_callback()
{}
   
char get_scancode()
{
    char c=0;
   // do {
        if(inb(0x60)!=c) {
            c=inb(0x60);
            if(c>0)
                return c;
        }
	return 0;
  //  } while(1);
}

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
	allocate_vbuffer(); 		
//	asm ("movl %0, %%eax;": : "r"((uint32_t)free_memory)); 
//	for (; ; ); 
	
	fill_buffer_pattern(STRIPES, VGA_COLOR_DARK_GREY, VGA_COLOR_LIGHT_GREY); 
	flip_buffers();
	isr_traps_init(); 
	init_keyboard(); 
	init_pit(); 
	enable_hardware_interrupts(); 
	asm ("sti"); 

	char* name = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	
	draw_char(20, 20, 'F', VGA_COLOR_BLUE); 
	draw_string(10, 50, name, VGA_COLOR_BROWN); 
	draw_rect(20, 20, 100, 100, VGA_COLOR_WHITE);	
	
	asm volatile ("push $0xdeadbeef"); 	
	asm volatile ("pop %eax");
	register uint32_t my_eax asm("%eax"); 
	uint32_t j = my_eax;
	draw_string(50, 10, int2hex(j), VGA_COLOR_BLACK); 
	
//	for (;;);
//	asm volatile ("int $0x00"); 
	
	Widget console = widget_create1(10, 100, 300, 80, VGA_COLOR_BLUE, 0); 
	Widget time = widget_create1(200, 5, 110, 30, VGA_COLOR_GREEN, 0); 

	// adds widget to global WidgetList
	widget_register(&console); 
	widget_register(&time); 

	// enables keyboard inut, callback will be called if widget is selected
	widget_init_keyboard(&console, teletype_keyboard_callback);
	widget_init_textmode(&time, 50); 
		
	while (1)
	{
		widget_set_text(&console, stdin); 
		/*if ((uint32_t)*system_time % 100 == 0)
		{
			widget_set_text(&time, int2str(s)); 
			s++; 
		}*/
		widget_update_all(); 
	
		#include "../tools/graphics/examples/smiley.c"
		draw_sprite(0, 0, sprite_player); 			
		flip_buffers(); 
	}
	asm volatile ("mov $0x888888, %eax"); 
	asm volatile ("hlt"); 

}
