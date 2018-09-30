
#ifndef LCDLIB
	#define LCDLIB 1
	#include "../lcd/LCD.h"
#endif

#ifndef UARTLIB
	#define UARTLIB 1
	#include "../comm/UART.h"
#endif

#ifndef STRINGLIB
	#define STRINGLIB 1
	#include "../misc/String.h"
#endif

#ifndef TIMERLIB
	#define TIMERLIB 1
	#include "../misc/Timer.h"
#endif

#include "ESPStructs.h"

#define CR 0x0D
#define NL 0x0A
#define DQUOTES 0x22

Timer timer;

ISR(TIMER0_OVF_vect)
{
	timer.overflow();
}

// this class is for ESP 8266 - 01 wifi module with station mode config.
// it covers most of basic functionality
class ESPWifi
{
	char* AP;
	char* temp;
	UART serial;
	Str slib;
	WifiBuffer wifibuffer;
	public:
	LCD display;
	// default constructor
	ESPWifi() {
		init();
	}
	
	// init() - initialise
	void init() {
		_delay_ms(1000);			
		cmdCall("ATE0",1);									// disable echoing of cmd
		while(!test());										// AT TEST
		setMode(STATION_MODE);								// set to station mode
		display.print("WIFI STARTED");
	}
	
	// cmdCall() - send command and save response in wifi buffer
	// param : cmd - command
	// param : timeout - time to wait collecting response
	void cmdCall(const char* cmd, uint8_t timeout) {
		uint8_t i = 0;
		serial.print(cmd);									// send command			
		serial.send(CR);									// followed by '\r'
		serial.send(NL);									// followed by '\n'
		timer.startTimer(timeout);							// start Timer
		while(! timer.timeout()) {							// for timeout seconds, do
			if(serial.receiveComplete()) {					// if RXC flag is set
				wifibuffer.buffer[i ++] = serial.getUDR();	// get content of UDR in wifibuffer
			}
		}
		wifibuffer.buffer[i] = '\0';						// string ending for further processing
	}	
	
	// test() - run AT TEST
	uint8_t test()
	{
		cmdCall(AT_TEST,1);											// send AT\r\n
		return slib.match(wifibuffer.t_r.result, OK_RESPONSE, CR);	// return if received OK response
	}
	
	// setMode() - set mode for wifi module
	// param : mode - 1 for station mode, 2 for AP mode
	// returns : if setting mode was successful
	uint8_t setMode(uint8_t mode) {
		switch(mode) {
			case STATION_MODE:
				cmdCall(MODE_STATION, 1);							// send AT+CWMODE_DEF=1
				break;
			case AP_MODE:
				cmdCall(MODE_AP, 1);								// send AT+CWMODE_DEF=2
				break;
			default:
				cmdCall(MODE_STATION, 1);							// send AT+CWMODE_DEF=1
				break;
		}
		return slib.match(wifibuffer.cwmode_a.t_r.result, OK_RESPONSE, CR);	 // return if received OK response
	}
	
	// getMode() - get current running mode for wifi module
	// returns : 1 for station, 2 for AP, 3 for station+AP, -1 for not successful response
	uint8_t getMode() {
			cmdCall(MODE_QUERY, 1);												// send AT+CWMODE_DEF?
			if(slib.match(wifibuffer.cwmode_q.t_r.result, OK_RESPONSE, CR)) {	// if received OK response
				return (wifibuffer.cwmode_q.mode - 48);							// return in numeric value					
			}
			else {
				return -1;														// return -1
			}
	}
	
	// APConnected() - check if module connected to AP
	// returns : is module connected to an AP
	uint8_t APConnected() {
		cmdCall(AP_QUERY, 1);													// send AT+CWJAP_DEF?
		return !slib.match(wifibuffer.cwjap_q.noAP, NO_AP, CR);					// return not module not connected 
	}
	
	// getAP() - get name of AP to which module is connected
	// returns : name of AP
	char* getAP() {
		if(APConnected()) {							// if module is connected to AP
			//+CWJAP_DEF:"<AP NAME>","<MAC>",...... // is in wifi buffer
			slib.substr(wifibuffer.buffer, 12, slib.indexOf(wifibuffer.buffer, DQUOTES, 12, '\r'), AP); // extract and copy AP name to AP
			return AP;								// return AP
		}
		else {				
			return "Not Connected";					// return Not Connected
		}
	}
	
	// setAP() - connect module to AP
	// param : ssid - name of AP
	// param : psswd - password to AP
	// returns : if connection was successful 
	uint8_t setAP(const char* ssid, const char* psswd) {
		temp = AP_ASSIGN;							// start with AT+CWJAP_DEF=
		temp[13] = DQUOTES;							// AT+CWJAP_DEF="
		uint8_t x = 14;
		for(uint8_t y = 0; ssid[y] != '\0'; y ++) {
			temp[x] = ssid[y];
			x ++;
		}
		temp[x ++] = DQUOTES;						// AT+CWJAP_DEF="<ssid>"
		temp[x ++] = ',';							// AT+CWJAP_DEF="<ssid>",
		temp[x ++] = DQUOTES;						// AT+CWJAP_DEF="<ssid>","
		for(uint8_t y = 0; psswd[y] != '\0'; y++) {
			temp[x] = psswd[y];
			x ++;	
		}
		temp[x++] = DQUOTES;						// AT+CWJAP_DEF="<ssid>","<psswd"
		temp[x] = '\0';								// string ending
		cmdCall(temp, 7);							// send it
		return slib.match(wifibuffer.cwjap_a.t_r.result, OK_RESPONSE, CR);	// return if connecting to ssid was successful
	}
	
	// checkStatus() - check connection status of AP
	// returns : connection status of AP, check "ESPStructs.h"
	uint8_t checkStatus() {
		cmdCall(CHECK_STATUS, 1);					// send AT+CIPSTATUS
		return wifibuffer.cipstatus.status;
	}
	
	// startTCPConnection() - start TCP connection
	// param : host - host to connect
	// param : port - port to which connect
	// returns : if connection was successful
	uint8_t startTCPConnection(const char* host, const char* port) {
		char* str = START_TCP_CONN_CMD;				// start with AT+CIPSTART="TCP",		
		str[18] = DQUOTES;							// AT+CIPSTART="TCP","
		uint8_t x = 19;
		for(uint8_t y = 0; host[y] != '\0'; y ++) {
			str[x] = host[y];
			x ++;
		}
		str[x ++] = DQUOTES;						// AT+CIPSTART="TCP","<host>"
		str[x ++] = ',';							// AT+CIPSTART="TCP","<host>",
		for(uint8_t y = 0; port[y] != '\0'; y ++) {
			str[x] = port[y];
			x ++;
		}
		str[x] = '\0';								// AT+CIPSTART="TCP","<host>",port
		cmdCall(str, 3);							// send it
		return slib.match(wifibuffer.cipstart_s.t_r.result, OK_RESPONSE, CR);	// return if connection is successful
	}
	
	// closeTCPConnection() - close TCP connection
	// returns : if connection was closed
	uint8_t closeTCPConnection() {
		cmdCall(CLOSE_CONN_CMD, 2);					// send AT+CIPCLOSE
		return slib.match(wifibuffer.cipclose.close, CLOSED, CR);	// return result
	}
	
	// ping() - check if connection exists
	// param : host to check connection with
	// returns : if connection exists
	uint8_t ping(const char* host) {
		char str[] = PING_CMD;						// start with AT+PING=
		str[8] = DQUOTES;							// AT+PING="
		uint8_t x = 9;
		for(uint8_t y = 0; host[y] != '\0'; y ++) {
			str[x] = host[y];
			x ++;
		}
		str[x ++] = DQUOTES;						// AT+PING="<host>"
		str[x] = '\0';
		cmdCall(str, 3);							// send it
		return !slib.match(wifibuffer.ping.timeout, TIMEOUT, CR);	// return if timeout didn't occur
	}
	
	// listAP() - check available AP in surrounding
	// refer instructions sheet for details on response for AT+CWLAP
	void listAP() {
		cmdCall(LIST_AP, 5);						// AT+CWLAP
		if(slib.match("+CWLAP:", slib.substr(wifibuffer.buffer, 0, 7))) {							// at least has one AP
			int point = -1;		
			int OKIndex = slib.patternIndex(wifibuffer.buffer, OK_RESPONSE, 0, 256);				// index of OK RESPONSE after list
			int cwlapIndex = 0, d1 = 0, d2 = 0;
			uint8_t i = 0;
			do {																					// to extract all AP
				d1 = slib.indexOf(wifibuffer.buffer, DQUOTES, cwlapIndex, CR);						// index for " 
				d1 ++;
				d2 = slib.indexOf(wifibuffer.buffer, DQUOTES, d1, CR);								// index for next "
				if((d1 > 0) & (d2 > 0))	 {															// if both not -1
					slib.toCharArray(slib.substr(wifibuffer.buffer, d1, d2), wifibuffer.APlist[i]);	// extract and copy to start of APlist
					i ++;		
					cwlapIndex = slib.patternIndex(wifibuffer.buffer, "+CWLAP:", d2, OKIndex);		// next +CWLAP label
					if(cwlapIndex == -1) {															// if = -1, break
						break;
					}
				}
				else {																				// mostly won't break here
					break;
				}
			} while (cwlapIndex < OKIndex);															
			slib.toCharArray("EOL", wifibuffer.APlist[i]);											// add EOL to last APlist to be used by displayAP()
		}
		else {
			slib.toCharArray("No AP Found", wifibuffer.APlist[0]);									// no Access point found
			slib.toCharArray("EOL", wifibuffer.APlist[1]);
		}
	}
	
	// displayAP() - display list of available AP
	void displayAP() {
		listAP();													// find available AP
		uint8_t i = 0;
		do {
			display.print(wifibuffer.APlist[i]);					// display names of found AP
			_delay_ms(2000);										// delay
			i++;
		} while (!slib.match(wifibuffer.APlist[i], "EOL") & (i < 6));// while EOL is not intercepted
		display.print("END OF LIST");								// to flag end of list
	}
	
	// sendData() - overloaded method
	uint8_t sendData(const char* data) {
		uint8_t l = slib.len(data);
		char size[4];
		slib.intToStr(l, size);
		return sendData(data, size);
	}
	
	// sendData() - send data over TCP connection
	// param : data - data to send
	// param : size - length
	// returns : if sending is successful
	uint8_t sendData(const char* data, const char* size) {
		if(checkStatus() == STATUS_CONNECTED) {							// if connected to TCP connection, proceed
			char* str = SEND_DATA_CMD;									// AT+CIPSENDEX=
			uint8_t len = slib.len(size);
			uint8_t y = 13;
			for(uint8_t x = 0; x < len; x ++) {	
				str[y] = size[x];
				y ++;
			}
			str[y] = '\0';												// AT+CIPSENDEX=<size>
			cmdCall(str, 1);
			if(slib.match(wifibuffer.t_r.result, OK_RESPONSE, '\r')) {	// if received OK response
				timer.startTimer(3);									 	
				serial.print(data);										// send data
				serial.send('\0');
				int i = 0;
				while(! timer.timeout()) {								// same as sending cmd in cmd in cmdCall
					if(serial.receiveComplete()) {
						wifibuffer.buffer[i ++] = serial.getUDR();
					}					
				}
				wifibuffer.buffer[i] = '\0';
				if(slib.patternIndex(wifibuffer.sendex.rest, "SEND OK", 0, 23)) {	// if sending OK
					return 1;											// sent successfully
				}
				return 0;
			}
			else {
				return 0;												// sending failed														
			}
		}
	}
		
	//SAKE OF TESTING
	void displayBuffer()
	{
		uint8_t i = 0;
		do
		{
			wifibuffer.dislist[i][15] = '\0';
			display.print(wifibuffer.dislist[i]);
			_delay_ms(2000);
			i++;
		} while ((i < 16));
		display.print("END of LIST");
	}
};