void putc(char c)
{
	stdout[stdout_i] = c; 
	stdout_i++; 
}

// TODO: check for buffer overflows
void println(char* str)
{
	char c; 
	for (uint8_t x = 0; (c = *(str + x)); x++)
		putc(c); 
}
