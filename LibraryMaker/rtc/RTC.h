
# include "../comm/I2C.h"

# define RTC_ADDR_READ 0xD1
# define RTC_ADDR_WRITE 0xD0

// class for DS1307 RTC module using I2C
class RTC
{
	I2C twi;
	char s, m, h;
	char dd, mm, yy; 
	char str[9];
	public:
	// default constructor
	RTC() {
		init();							// initialize
	}
	
	// init() - initialize RTC module
	void init() {
		twi.start();					// start transaction
		twi.write(RTC_ADDR_WRITE);		// send RTC address with LSB 1 (for write)
		twi.write(0x07);				// set register pointer to 07 (control register)
		twi.write(0x00);				// set control register = 0
		twi.stop();						// end transaction
	}
	
	// setTime() - sets time in RTC module
	// param : in - time in format hh:mm:ss
	// for e.g. "19:37:15"
	void setTime(const char* in) {
		s = (in[6] << 4) + (in[7] & 0x0F);	// "15" ->> 15 BCD		
		m = (in[3] << 4) + (in[4] & 0x0F);	// "37" ->> 37 BCD
		h = (in[0] << 4) + (in[1] & 0x0F);	// "19" ->> 19 BCD
		twi.start();						// start transaction
		twi.write(RTC_ADDR_WRITE);			// send RTC address to write
		twi.write(0);						// set register pointer to 0
		// send seconds, minutes, hour
		twi.write(s);					
		twi.write(m);
		twi.write(h);
		twi.stop();							// end transaction
	}
	
	// setDate() - sets date in RTC module
	// param : in - date in format dd-mm-yy
	// for e.g. "19-08-18"
	void setDate(const char* in) {
		yy = (in[6] << 4) + (in[7] & 0x0F);	// "18" ->> 18 BCD
		mm = (in[3] << 4) + (in[4] & 0x0F); // "08" ->> 08 BCD
		dd = (in[0] << 4) + (in[1] & 0x0F); // "19" ->> 19 BCD
		twi.start();						// start transaction
		twi.write(RTC_ADDR_WRITE);			// send RTC address to write
		twi.write(4);						// set register pointer to 4
		// send day, month, year
		twi.write(dd);						
		twi.write(mm);
		twi.write(yy);
		twi.stop();							// end transaction
	}
	
	// getTime() - returns time
	// returns : time in format hh:mm:ss
	// for e.g. 15:09:27
	char* getTime() {
		twi.start();						// start transaction
		twi.write(RTC_ADDR_WRITE);			// send RTC address to write
		twi.write(0);						// set register pointer to 0
		twi.stop();							// end transaction
		
		twi.start();						// start another transaction
		twi.write(RTC_ADDR_READ);			// send RTC address to read 
		s = twi.read();						// receive seconds (from location 0)
		m = twi.read();						// receive minute (from location 1)
		h = twi.readLast();					// receive hour (from location 2)
		twi.stop();							// end transaction
		
		str[0] = '0' + (h >> 4);			// '0' + 1 ->> '1'
		str[1] = '0' + (h & 0x0F);			// '0' + 5 ->> '5'
		str[2] = ':';			
		str[3] = '0' + (m >> 4);			// '0' + 0 ->> '0'
		str[4] = '0' + (m & 0x0F);			// '0' + 9 ->> '9'
		str[5] = ':';
		str[6] = '0' + (s >> 4);			// '0' + 2 ->> '2'
		str[7] = '0' + (s & 0x0F);			// '0' + 7 ->> '7'
		str[8] = '\0';
		return str;							// return string
	}
	
	// getDate() - returns date
	// returns : date in format dd-mm-yy
	// for e.g. "19-08-18"
	char* getDate() {
		twi.start();						// start transaction
		twi.write(RTC_ADDR_WRITE);			// send RTC address to write
		twi.write(4);						// set register pointer to 4
		twi.stop();							// end transaction
		
		twi.start();						// start transaction
		twi.write(RTC_ADDR_READ);			// send RTC address to read
		dd = twi.read();					// receive day (from location 4)
		mm = twi.read();					// receive month (from location 5)
		yy = twi.readLast();				// receive year (from location 6)
		twi.stop();							// end transaction
		
		str[0] = '0' + (dd >> 4);			// '0' + 1 ->> '1'
		str[1] = '0' + (dd & 0x0F);			// '0' + 9 ->> '9'
		str[2] = '-';
		str[3] = '0' + (mm >> 4);			// '0' + 0 ->> '0'
		str[4] = '0' + (mm & 0x0F);			// '0' + 8 ->> '8'
		str[5] = '-';
		str[6] = '0' + (yy >> 4);			// '0' + 1 ->> '1'
		str[7] = '0' + (yy & 0x0F);			// '0' + 8 ->> '8'
		str[8] = '\0';
		return str;							// return string
	}
};
