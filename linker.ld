ENTRY(bootsrapper);
SECTIONS
{
	. = 0x7E00;
	.text : AT(0x7E00)
	{
			*(.text);
	}
	.data : ALIGN(0x8)
	{
			*(.rodata);
			*(.data);
	}
	/* BSS section defnition after the text and data */
	.bss : ALIGN(0x8)
	{
			__bss_start = .;
			*(COMMON);
			*(.bss)
			. = ALIGN(4);
			__bss_end = .;
	}
	__bss_sizeb = SIZEOF(.bss);

	/* Remove sections that won't be relevant to us */
	/DISCARD/ : {
			*(.eh_frame);
			*(.comment);
			*(.note.gnu.build-id);
	}
}

