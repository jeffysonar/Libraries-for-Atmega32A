// this library can be used for atmega328 
#ifndef AVRIO
	#define AVRIO 1
	#include <avr/io.h>
#endif

#ifndef BAUD_RATE
	#define BAUD_RATE 0x07		// for 115200 baud rate @ 14.7456 MHz
//	#define BAUD_RATE 0x5F		// for 9600 baud rate @ 14.7456 MHz
#endif

#ifndef STRINGLIB
	#define STRINGLIB 1
	#include "../misc/String.h"
#endif

class UART
{
	Str slib;		// just for len() in print
	
	public:
	// default constructor
	UART() {
		init();									// init UART module
	}
	
	// init() - initialises UART module
	void init() {
		UCSR0B = (1 << RXEN0) | (1 << TXEN0);	// enable transmitter and receiver
		UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	// 8 bit data size
		UBRR0L = BAUD_RATE;						// #nextStep, set baud rate
	}
	
	// send() - transmits passed byte over UART
	// param : ch - byte to transmit
	void send(uint8_t ch) {
		while(! emptyUDR());					// wait till UDR0 is not empty
		UDR0 = ch;								// copy ch to UDR0
	}
	
	// receive() - return received byte over UART
	// returns : received byte
	uint8_t receive() {
		while(! receiveComplete());				// wait till reception is not complete
		return getUDR();						// return content of UDR0
	}
	
	// receiveComplete() - check if reception is complete
	// returns : has reception completed
	uint8_t receiveComplete() {
		return ((UCSR0A) & (1 << RXC0));		// 1 if receive complete (RXC0) is set, else 0	
	}
	
	// emptyUDR() - check if UDR is empty
	// returns : is UDR empty
	uint8_t emptyUDR() {
		return (UCSR0A & (1 << UDRE0));			// 1 if UDR is empty, else 0
	}
	
	// getUDR() - returns content of UDR0
	uint8_t getUDR() {
		return UDR0;							// return UDR0
	}
	
	// print() - overloaded method
	void print(const char* str) {
		int length = slib.len(str);
		print(str,length);
	}
	
	// print() - transmit passed sequence of bytes
	// param : str - sequence of bytes to send
	// param : length - length of str
	void print(const char* str, uint8_t length) {
		// for all characters (length of str)
		for(int i = 0;i < length;i ++)
		{
			send(str[i]);							// transmit all bytes
		}
	}	
};