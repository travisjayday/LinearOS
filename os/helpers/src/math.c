uint32_t power(uint32_t base, uint32_t exp) 
{
	uint32_t result = 1;
	for (; exp > 0; exp--) 
		result *= base; 
	return result;
}


