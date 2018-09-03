
#ifndef AVRIO
	#define AVRIO 1
	#include <avr/io.h>
#endif

class EEPROM
{
	public:
	EEPROM()
	{
		// do nothing
	}	
	
	// eepread()
	// param : addr - address in EEPROM to read
	// returns : content at address addr
	uint8_t eepread(uint16_t addr)
	{
		while((EECR & (1 << EEWE)));	// wait for previous write
		EEAR = addr;					// set address
		EECR |= (1 << EERE);			// set  read enable
		_delay_ms(1);					// wait for 1 ms (no polling)
		return EEDR;					// return data
	}
	
	// eepwrite()
	// param : addr - address in EEPROM to write at
	// param : data - data byte to write
	void eepwrite(uint16_t addr, uint8_t data)
	{
		while((EECR & (1 << EEWE)));	// wait for previous write
		EEAR = addr;					// set address 
		EEDR = data;					// add data
		EECR |= (1 << EEMWE);			// master write enable
		EECR |= (1 << EEWE);			// set write enable bit
	}
	
	// eeprint()
	// param : addr - starting address
	// param : source[] - data bytes to write
	// param : len - length (number of elements) of source
	void eeprint(uint16_t addr, char source[], uint16_t len)
	{
		for(uint16_t i = addr,j = 0; i < (addr + len); i ++ , j ++)	// write from addr to (addr + len - 1)
		{
			eepwrite(i,source[j]);		// write byte
		}
	}
};