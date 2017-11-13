

Widget widget_create1(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t _bg, uint8_t _f)
{
	Widget w;
	w.x = _x;
	w.y = _y;
	w.width = _w;
	w.height = _h;
	w.bg_color = _bg;
	w.flags = _f;
	w.txt_color = VGA_COLOR_WHITE;
	return w;
}

Widget widget_create2(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h) 
{
	return widget_create1(_x, _y, _w, _h, VGA_COLOR_BLACK, 0); 
}

void widget_init_textmode(Widget* wid, uint8_t _bufsize)
{
	wid->flags = wid->flags | (1 << 7); 
	wid->bufsize = _bufsize; 
	// allocate space for text
	wid->text = malloc(_bufsize); 
}

void widget_register(Widget* wid)
{
	WidgetList[WidgetCount] = wid;
	WidgetCount++; 
}

void widget_init_keyboard(Widget* wid, void (*key_handler)())
{
	widget_init_textmode(wid, 100); 
	wid->flags = wid->flags | (1 << 6); 	// set second bit to make it selecatble and enable keyboard
	wid->keyboard_handler = key_handler; 
}

void widget_render_text(Widget* wid)
{
	uint8_t cperline = (int)((float)wid->width / 7.0 + 0.5);

	// loop through multi lines
	for (uint8_t l = 0; ; l++) 
	{
		// loop through line
		for (uint8_t i = 0; i < cperline; i++)
		{
			// character to write
			uint8_t ch = *(wid->text + l * cperline + i);

			// if eos, ret
			if (ch == '\0')
				return; 
			else if (ch == ' ')
				continue; 

			// write character
			draw_char(
				wid->x + 3 + 7 * i, 
				wid->y + 3 + 7 * l, 
				ch, 
				wid->txt_color
			);  
		}
	}
	

}

void widget_set_text(Widget* wid, uint8_t* text)
{
	for (int i = 0; *(text + i); i++)
	{
		*(wid->text + i) = *(text + i); 
	}
}

void widget_append_char(Widget* wid, uint8_t c) 
{
	wid->textidx++;
	*(wid->text + wid->textidx) = c; 
}

void widget_update_all()
{
	for (int i = 0; i < WidgetCount; i++)
	{
		if (WidgetList[i]->flags & (1 << 6))
		{
			// keyboard mode enabled
			if (i == SelectedWidget) 
			{
				draw_rect(
					WidgetList[i]->x - SELECTED_BORDER_THICK,
					WidgetList[i]->y - SELECTED_BORDER_THICK,
					WidgetList[i]->width + SELECTED_BORDER_THICK *2,
					WidgetList[i]->height + SELECTED_BORDER_THICK *2,
					SELECTED_COLOR
				);
			}	
		}	
		draw_rect(
			WidgetList[i]->x, 
			WidgetList[i]->y,
			WidgetList[i]->width, 
			WidgetList[i]->height,
			WidgetList[i]->bg_color
		); 
		if (WidgetList[i]->flags & (1 << 7))
		{
			// text mode enabled
			widget_render_text(WidgetList[i]);

		}
	}	
}

