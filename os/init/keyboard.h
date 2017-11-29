#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "interrupts.h"
#include "../tty.h"

enum KYBRD_ENC_IO {
	KYBRD_ENC_INPUT_BUF = 0x60,
	KYBRD_ENC_CMD_REG = 0x60
};

enum KYBRD_CTRL_IO {
	KYBRD_CTRL_STATS_REG = 0x64,
	KYBRD_CTRL_CMD_REG = 0x64
};

extern char stdin[256]; 
char kblayout[128] =
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
extern void isr_keyboard_handler(); 
void init_keyboard(); 

// returns the byte status of keyboard controller
uint8_t kybrd_ctrl_read_status();

// reads the current item in keyboard enocder buffer
uint8_t kybrd_enc_read_buf();

// send command to keyboard controller
void kybrd_ctrl_send_cmd(uint8_t cmd); 

// send command to keyboard encoder
void kybrd_enc_send_cmd(uint8_t cmd); 
#include "src/keyboard.c"

#endif
