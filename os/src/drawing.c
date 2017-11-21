void allocate_vbuffer()
{
	VBUFFER_MEM = malloc(VBUFFER_SIZE); 
}

void fill_buffer(uint8_t color)
{
	uint16_t c16 = color << 8 | color;
	uint32_t c32 = c16 << 16 | c16;
	memset32_t((uint32_t*)VBUFFER_MEM, c32, VBUFFER_SIZE/4); 
}

void flip_buffers()
{
	memcpy32_t((uint32_t*)VGA_MEM, (uint32_t*)VBUFFER_MEM, VBUFFER_SIZE/4); 
}

void draw_char(uint16_t _x, uint16_t _y, uint8_t c, uint8_t color)
{
	if (c >= 'a' && c <= 'z')
		c -= ('a' - 'A');
	else if (c > 'z') 
		c -= ('z' - 'a' +1); 
	else if (c == ' ') 
		return; 

	c -= '!';

	uint32_t loc = 1; 
	uint32_t character = CHAR_MAP[c];
    _y *= VGA_WIDTH; 
	loc = loc << 31; 
	for (uint16_t h = 0; h < 5 * VGA_WIDTH; h += VGA_WIDTH)
	{
		for (uint8_t w = 0; w < 5; w++)
		{
			if (character & loc)
			       *(VBUFFER_MEM + _y + _x + h + w) = color; 
			loc = loc >> 1; 	
		}
	}
}

void draw_string(uint16_t _x, uint16_t _y, uint8_t* str, uint8_t color)
{
	uint8_t c;			// character var
	uint8_t dx = 0;		// delta x position coordinate 
	for (uint8_t i = 0; (c = *(str + i)) != '\0'; i++)
	{
		if (c == '\n')
		{
			_y += 7; 	// newline
			dx = 0;		// delta x = 0 = carriage return 
		}
		else
		{
			draw_char(_x + dx * 7, _y, c, color); 
			dx++; 
		}
	}
}

void draw_rect(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t color)
{
	_y *= VGA_WIDTH; 
	for (int h = 0; h < height * VGA_WIDTH; h += VGA_WIDTH) 
	{
		for (uint16_t w = 0; w < width; w++)
		{
			*(VBUFFER_MEM + _y + h + _x + w) = color; 	
		}
	}
}


void draw_rect_border(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t color, uint8_t thickness, uint8_t bcolor)
{
	draw_rect(
		_x - thickness,
	       	_y - thickness,
	       	width + thickness, 
		height + thickness,
		bcolor
	); 
	draw_rect(_x, _y, width, height, color); 
}
