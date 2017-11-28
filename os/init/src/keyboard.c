void init_keyboard()
{
	register_isr_trap(0x21, (uint32_t*) isr_keyboard_handler); 
}
