asm (".code32"); 
#include <stdint.h>
#include "mem_mgmt.h"
#include "drawing.h"
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
unsigned char kblayout[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
   '9', '0', '-', '=', '\b',    /* Backspace */
    '\t',           /* Tab */
     'q', 'w', 'e', 'r',    /* 19 */
     't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,        /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
   '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
   '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};    
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

uint32_t get_time()
{
	return *((uint32_t*)0x046C); 
//	return 5334;
}

uint8_t* int2str(uint32_t num)
{
	uint8_t* str = malloc(10); 
	int i; 
	for (i = 0; num != 0; i++)
	{
		str[10-i] = (num % 10) + '0'; 
		num /= 10; 
	}
	str[10-i] = '\0';
	return str; 
}

static void panic(char* str) 
{
	fill_buffer(VGA_COLOR_BLUE);
	draw_string(10, 10, str, VGA_COLOR_WHITE); 
	flip_buffers(); 
	for (;;); 
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
	play_sound(1000); 
	allocate_vbuffer(); 
	fill_buffer(VGA_COLOR_DARK_GREY); 
	char* name = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
	draw_char(20, 20, 'F', VGA_COLOR_BLUE); 
	draw_string(10, 50, name, VGA_COLOR_BROWN); 
	draw_rect(20, 20, 100, 100, VGA_COLOR_WHITE);
	// creates widget
	Widget console = widget_create1(10, 100, 200, 80, VGA_COLOR_BLUE, 0); 

	// adds widget to global WidgetList
	widget_register(&console); 

	// enables keyboard inut, callback will be called if widget is selected
	widget_init_keyboard(&console, teletype_keyboard_callback);

	widget_set_text(&console, (uint8_t*)"!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFG this is   space HIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\0"); 
	//widget_set_text(&console, int2str(get_time())); 
	widget_update_all();
	//uint8_t pr = 0; 
	
	while (1)
	{
	//	if (pr != c) 
		{
//		uint8_t c = (uint8_t) get_scancode();
//			widget_append_char(&console, kblayout[c]); 
	//		pr = c;  
		}
		widget_update_all(); 
			
		flip_buffers(); 
	}
	asm volatile ("mov $0x888888, %eax"); 
	asm volatile ("hlt"); 

}
