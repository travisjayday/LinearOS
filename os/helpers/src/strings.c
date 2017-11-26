// returns lengt of string + \0
uint32_t strlen(char* str) 
{
	const char* s; 
	for (s = str; *s; ++s); 
	return (s - str); 
}

char* int2str(uint32_t num)
{
	// 2^32 = 10 digits + 1 null terminator
	char* str = malloc(11);
	int i;
	for (i = 9; num != 0; i--)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
	}
	str[10] = '\0';
	str += i + 1; 
	return str;
}

uint32_t str2int(char* str) 
{
	uint8_t len = strlen(str); 
	uint32_t result = 0;
	for (uint8_t i = 0; i < len; i++)
		result += power(10, i) * (*(str+len-i-1) - '0');
	return result;
}

char* int2hex(uint32_t num)
{
	char* chars = malloc(9); 
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

void strcpy(char* dest, char* src)
{
	memcpy8_t((uint8_t*) dest, (uint8_t*) src, strlen(src) + 1); 	
}

char* strcat(char* dest, char* src)
{
	char* buf = malloc(strlen(dest) + strlen(src) + 1); 
	strcpy(buf, dest); 
	strcpy(buf + strlen(dest), src); 
	return buf; 
}

char* format_segmented_addr(uint32_t selector, uint32_t addr)
{
	char* formatted = malloc(18);
	strcpy(formatted, int2hex(selector)); 
	*(formatted + 8) = ':';
	strcpy(formatted + 9, int2hex(addr));  	
	return formatted; 
}
