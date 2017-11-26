void allocate_vbuffer()
{
	VBUFFER_MEM = (uint8_t*) malloc(VBUFFER_SIZE); 
}

void fill_buffer_pattern(uint8_t pattern, uint8_t color1, uint8_t color2)
{
	switch (pattern)
	{
		case SOLID: 
			color2 = color1; 

		case STRIPES: 
	
		default: 
		{
			uint16_t c16 = color1 << 8 | color2;
			uint32_t c32 = c16 << 16 | c16;
			memset32_t((uint32_t*)VBUFFER_MEM, c32, VBUFFER_SIZE/4); 
			break;
		}
	}
}

void fill_buffer(uint8_t color)
{
	fill_buffer_pattern(SOLID, color, 0); 
}	

void flip_buffers()
{
	memcpy32_t((uint32_t*)VGA_MEM, (uint32_t*)VBUFFER_MEM, VBUFFER_SIZE/4); 
}

void draw_char(uint16_t _x, uint16_t _y, char c, uint8_t color)
{
	if (c >= 'a' && c <= 'z')
		c -= ('a' - 'A');
	else if (c > 'z') 
		c -= ('z' - 'a' +1); 
	else if (c == ' ') 
		return; 

	c -= '!';

	uint32_t loc = 1; 
	uint32_t character = CHAR_MAP[(uint8_t)c];
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

void draw_string(uint16_t _x, uint16_t _y, char* str, uint8_t color)
{
	char c;			// character var
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
	for (uint16_t h = 0; h < height * VGA_WIDTH; h += VGA_WIDTH) 
	{
		for (uint16_t w = 0; w < width; w++)
		{
			*(VBUFFER_MEM + _y + h + _x + w) = color; 	
		}
	}
}


void draw_rect_border(uint16_t _x, uint16_t _y, uint16_t width, uint16_t height, uint8_t color, uint8_t thickness, uint8_t bcolor)
{
	// TODO: Make this more efficient
	draw_rect(
		_x - thickness,
	       	_y - thickness,
	       	width + thickness, 
		height + thickness,
		bcolor
	); 
	draw_rect(_x, _y, width, height, color); 
}


void draw_sprite(uint16_t _x, uint16_t _y, uint8_t* sprite)
{
	uint8_t height = sprite[0]; 
	uint16_t width = sprite[1] | ((sprite[2] & (~(1 << 7))) << 8);
	uint8_t rle = sprite[2] >> 7; 	// on if == 1, off if == 0
	//uint16_t width = sprite[1] | sprite[2] << 8; 

	uint8_t* buffer = VBUFFER_MEM + _y * VGA_WIDTH + _x;  
	sprite += 5; 

	uint16_t i = 0; 
	uint16_t w = 0; 
	uint16_t h = 0; 
	//uint16_t t = 0;
	uint8_t cmd_b = 0; 

	// if encoded, decode
	if (rle) 
	{
		while (1) 
		{
	next_command: 
			cmd_b = *(sprite + i); 					// transparent or color or end?
			uint8_t repeat_b = *(sprite + i + 1); 	// number of times to repeat action

			// transparent byte
			if (cmd_b == 0xfa) 
			{
				h += (w + repeat_b) / (width); 		// absolute height in px	
				w = (w + repeat_b) % (width);		// relative width from left border to currently drawn widtd pixel 
				i += 2; 
			}
			// color byte
			else if (cmd_b == 0xff) 
			{
				uint8_t data_b = *(sprite + i + 2); 		// pixel color

				// loop until repeat_b = zero
				while (1)
				{	
					// loop until width is filled and put pixels
					while (w < width)
					{
						*(buffer + (h * (VGA_WIDTH)) + w) = data_b; 	// set pixel 

						// move 1px left and decrement repeat_b
						w++;
						repeat_b--;

						// if finished, jump to next command byte (outermost while loop)
						if (repeat_b == 0)
						{
							i += 3; 	
							goto next_command; 
						}
					}
					// once width is filled, go to next row and set relative widht back to 0
					w = 0; 
					h++; 
				}
			}
			else
			{
				break; 
			}
		}
	}
	// no encoding
	else
	{
		for (uint16_t h = 0; h < height; h ++) 
		{
			for (uint16_t w = 0; w < width; w++)
			{
				uint8_t data_b = *(sprite + i); 
				if (data_b != 0xfa)
					*(buffer + (h  * VGA_WIDTH) + w) = data_b; 	
				i++; 
			}
		}
	}
}
