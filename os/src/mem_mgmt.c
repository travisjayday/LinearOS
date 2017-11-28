// TODO: this is preliminary for quick and dirty heap allocations
// returns pointer to free memory
void* malloc(uint32_t size)
{
	void* start = free_memory; 
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
	// copy byte for byte
	for (uint32_t i = 0; i < size; i++) 
	{
		*((uint8_t*)dest + i) = *((uint8_t*)src + i); 
	}
}

void memcpy32_t(void* dest, void* src, uint32_t size)
{
	// copy dword for dword
	for (uint32_t i = 0; i < size; i++)
	{
		*((uint32_t*)dest + i) = *((uint32_t*)src + i); 
	}
}

void memset32_t(void* dest, uint32_t val, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		*((uint32_t*)dest + i) = val;
	}
}
