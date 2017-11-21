static void panic(uint8_t* str) 
{
	fill_buffer(VGA_COLOR_BLUE);
	draw_string(10, 10, (uint8_t*)"Kernel Panic!\n\n----------------\n\n", VGA_COLOR_WHITE);
	draw_string(10, 38, str, VGA_COLOR_WHITE); 
	flip_buffers(); 
	for (;;); 
}


