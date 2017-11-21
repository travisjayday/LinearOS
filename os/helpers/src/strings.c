uint32_t strlen(uint8_t* str) 
{
	uint32_t i = 0; 
	for (; *(str+i); i++); 
	return i; 
}

uint8_t* int2str(uint32_t num)
{
	    uint8_t* str = malloc(10);
		int i;
	   	for (i = 0; num != 0; i++)
	   	{
			str[10-i] = (num % 10) + '0';
			num /= 10;
		}
		str[10-i] = '\0';
	    return str;
}

uint32_t str2int(uint8_t* str) 
{
	uint8_t len = strlen(str); 
	uint32_t result = 0;
	for (uint8_t i = 0; i < len; i++)
		result += power(10, i) * (*(str+len-i-1) - '0');
	return result;
}

uint8_t* int2hex(uint32_t num)
{
	uint8_t* chars = malloc(9); 
	uint8_t i = 0;
	for (char s = 28; s >= 0; s -= 4)
	{
		chars[i] = num >> s & 0xF; 
		if (chars[i] >= 10) 
			chars[i] += 'A' - 10; 
		else
			chars[i] += '0';
		i++; 
	}
	chars[i] = '\0'; 
	return chars; 
}
