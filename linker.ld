ENTRY(bootstrapper);
SECTIONS
{
	. = 0x7E00; 
	.text : AT(0x7E00)
	{
		*(.text);
	}
	/*.data : ALIGN(0x8) 
	{
		*(.data);
	}
	.bss : ALIGN(0x8) 
	{
		*(.bss); 
	}*/
/*	.sig : AT(0x7E00)
	{
		SHORT(0xAA55);
	}*/
}
