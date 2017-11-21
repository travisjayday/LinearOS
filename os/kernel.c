asm (".code32"); 
#include <stdint.h>
#include "mem_mgmt.h"
#include "helpers/strings.h"
#include "drawing.h"
#include "src/panic.c"
#include "init/interrupts.h"
#include "init/keyboard.h"
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
 static void play_sound(uint32_t nFrequence) {
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
 }



extern void kernel_main()
{
	allocate_vbuffer(); 
	fill_buffer(VGA_COLOR_DARK_GREY); 
	isr_traps_init(); 
	init_keyboard(); 
	enable_hardware_interrupts(); 
	asm volatile ("sti"); 
	play_sound(1000); 
	uint8_t* name = (uint8_t*)"A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	
	draw_char(20, 20, 'F', VGA_COLOR_BLUE); 
	draw_string(10, 50, name, VGA_COLOR_BROWN); 
	draw_rect(20, 20, 100, 100, VGA_COLOR_WHITE);
	// creates widget
	Widget console = widget_create1(10, 100, 300, 80, VGA_COLOR_BLUE, 0); 

	// adds widget to global WidgetList
	widget_register(&console); 

	// enables keyboard inut, callback will be called if widget is selected
	widget_init_keyboard(&console, teletype_keyboard_callback);

//	widget_set_text(&console, (uint8_t*)"!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFG this is   space HIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\0"); 
	//widget_set_text(&console, int2str(get_time())); 
	widget_update_all(); //uint8_t pr = 0; 
		
	while (1)
	{
	//	if (pr != c) 
		{
//		uint8_t c = (uint8_t) get_scancode();
//			widget_append_char(&console, kblayout[c]); 
	//		pr = c;  
		}
		widget_set_text(&console, stdin); 
	//	widget_append_char(&console, kblay); 
		widget_update_all(); 
			
		flip_buffers(); 
//		volatile int j = 20 / *((uint8_t*)0x920); 
//		j++;
	}
	asm volatile ("mov $0x888888, %eax"); 
	asm volatile ("hlt"); 

}
