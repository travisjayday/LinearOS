void init_keyboard()
{
	register_isr_trap(0x21, (void*) isr_keyboard_handler); 
}
