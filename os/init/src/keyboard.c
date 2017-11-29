// returns status of keyboard form status reg
uint8_t kybrd_ctrl_read_status()
{
	return inb(KYBRD_CTRL_STATS_REG); 
}

uint8_t kybrd_enc_read_buf()
{
	return inb(KYBRD_ENC_INPUT_BUF); 
}

//sends cmd to keyboard through io port
void kybrd_ctrl_send_cmd(uint8_t cmd)
{
	// wait until kybrd ctrler buffer clear

	while (kybrd_ctrl_read_status() & 0b00000010);

	// send cmd
	outb(KYBRD_CTRL_CMD_REG, cmd); 
}

void kybrd_enc_send_cmd(uint8_t cmd)
{

	while (kybrd_ctrl_read_status() & 0b00000010);
	outb(KYBRD_ENC_CMD_REG, cmd); 
}

void init_keyboard()
{
	println("Preparing keyboard...\n"); 
	flip_buffers(); 

	// waiting for empty input buffer 
	while (kybrd_ctrl_read_status() & 0b00000010);

	println("Initializing keyboard...\n"); 

	io_wait();	
	kybrd_ctrl_send_cmd(0xAA); 	// self test

	while (kybrd_ctrl_read_status() & 0b00000010);

	io_wait(); 
	if (kybrd_enc_read_buf() == 0x55)
		println("Keyboard passed test"); 
	else 
		println("Keyboard FAILED test"); 

	io_wait(); 
	// reset and enable keyboard
	kybrd_enc_send_cmd(0xF6); 
	io_wait();
	uint8_t encoder_status = inb(0x64);
	io_wait(); 
	outb(0x64, encoder_status | 0b00000010); // force enable irq0*/

	// register keyboard handling isr
	register_isr_trap(0x21, (uint32_t*) isr_keyboard_handler); 
}
