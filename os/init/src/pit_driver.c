void init_pit()
{
	// defined in ../asm/init_pit.asm
	// sets the pit controller and the divisor for channel 0
	// to 1 irq/ms
	init_pit_channel0_ms(); 

	// register isr handler for physical interrupt 0
	register_isr_gate(0x20, (void*) isr_pit_channel0); 
}
