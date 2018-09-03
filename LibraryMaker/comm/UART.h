
#ifndef AVRIO
	#define AVRIO 1
	#include <avr/io.h>
#endif

#ifndef BAUD_RATE
	#define BAUD_RATE 0x07		// for 115200 baud rate @ 14.7456MHz
//	#define BAUD_RATE 0x5F		// for 9600 baud rate @ 14.7456MHz
#endif

#ifndef STRINGLIB
	#define STRINGLIB 1
	#include "../misc/String.h"
#endif

class UART
{
	Str slib;					// just for len (length)
	
	public:
	// default constructor
	UART() {
		// init UART module
		init();
	}
	
	// init() - initialises UART module
	void init() {
		UCSRB = (1 << RXEN) | (1 << TXEN);					// enable transmitter and receiver
		UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);	// URSEL for nextStep, 8 bit data size
		UBRRL = BAUD_RATE;									// #nextStep, set baud rate
	}
	
	// send() - transmits passed byte over UART
	// param : ch - byte to transmit
	void send(uint8_t ch) {
		while(! emptyUDR());				// wait till UDR is not empty
		UDR = ch;							// copy ch to UDR
	}
	
	// receive() - return received byte over UART
	// returns : received byte
	uint8_t receive() {
		while(! receiveComplete());			// wait till reception is not complete
		return UDR;							// return contents of UDR
	}
	
	// receiveComplete() - check if reception is complete
	// returns : has reception completed
	uint8_t receiveComplete() {
		return ((UCSRA) & (1 << RXC));		// 1 if receive complete (RXC) is set, else 0
	}
	
	// emptyUDR() - check if UDR is empty
	// returns : is UDR empty
	uint8_t emptyUDR() {
		return (UCSRA & (1 << UDRE));		// 1 if UDR is empty, else 0
	}
	
	// getUDR() - returns content of UDR
	uint8_t getUDR() {
		return UDR;							// return UDR
	}

	// print() - overloaded method
	void print(const char* str) {
		int length = slib.len(str);			// calculate length
		print(str,length);
	}
	
	// print() - transmit passed sequence of bytes
	// param : str - sequence of bytes to send
	// param : length - length of str
	void print(const char* str, uint8_t length) {
		// for all characters (length of str)
		for(int i = 0;i < length;i ++)
		{
			send(str[i]);					// transmit all bytes
		}
	}
			
};