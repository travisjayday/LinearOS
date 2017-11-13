ENTRY(bootstrapper);
SECTIONS
{
	. = 0x7E00; 
	.text : AT(0x7E00)
	{
		*(.text);
	}
/*	.sig : AT(0x7E00)
	{
		SHORT(0xAA55);
	}*/
}
