// returns pointer to free memory
uint8_t* malloc(uint32_t size)
{
	uint8_t* start = free_memory; 
	free_memory += size; 

	// system out of memory
//	if (free_memory > (uint8_t*) 0x7FFFF)
	{	
//		asm volatile ("mov $0xFFFFFF, %eax");
//		asm volatile ("hlt");
	}
	return start; 	
}

void linear_free(uint32_t size)
{
	free_memory -= size; 
}

void memcpy8_t(void* dest, void* src, uint32_t size)
{
	for (int i = 0; i < size; i++) 
	{
		*((uint8_t*)dest + i) = *((uint8_t*)src + i); 
	}
}

void memcpy32_t(uint32_t* dest, uint32_t* src, uint32_t size)
{
	for (int i = 0; i < size; i++)
	{
		*(dest + i) = *(src + i); 
	}
}

void memset32_t(uint32_t* dest, uint32_t val, uint32_t size)
{
	for (int i = 0; i < size; i++)
	{
		*(dest + i) = val;
	}
}
