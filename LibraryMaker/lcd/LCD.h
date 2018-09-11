
// change below DDRx and PORTx as per your connections
# ifndef LCD_PRT
	# define LCD_DRT DDRA
	# define LCD_PRT PORTA
# endif
# ifndef F_CPU							// F_CPU required to pass for delay.h
	# define F_CPU 14745600UL			// i am using 14.7456MHz
# endif
# include <util/delay.h>

# define LCD_RS 0						// port pin to RS 
# define LCD_RW 1						// port pin to R/W
# define LCD_EN 2						// port pin to ENABLE
// attach port pins 4, 5, 6, 7 to D4, D5, D6, D7

class LCD
{
	public:
	// default constructor
	LCD() {
		init();
	}
	
	// init() - initialize LCD
	void init() {
		LCD_DRT = 0xFF;					// set port as output
		LCD_PRT &= ~(1 << LCD_EN);		// make EN = 0
		_delay_ms(20);					// startup delay
		cmdSend(0x33);					// 33 for 4 bit mode
		_delay_ms(2);
		cmdSend(0x32);					// 32 for 4 bit mode
		_delay_ms(2);
		cmdSend(0x28);					// 28 for 4 bit mode
		_delay_ms(2);
		cmdSend(0x0C);					// display on, cursor off
		_delay_ms(2);
		clearDisplay();					// clear display
		_delay_ms(2);
		print("Initialized");
	}
	
	// clearDisplay() - sends cmd to clear display
	void clearDisplay() {
		cmdSend(0x01);					// send clear display
		_delay_ms(2);
	}
	
	// dataSend() - sends data to display on LCD
	// param : data - data to send
	void dataSend(uint8_t data) {
		// 4 msb bits, LCD enable, select data register, write
		LCD_PRT = (((data & 0xF0) | (1 << LCD_EN) | (1 << LCD_RS)) & ~(1 << LCD_RW));	
		_delay_us(100);
		LCD_PRT &= ~(1 << LCD_EN);					// EN = 0
		LCD_PRT = (LCD_PRT & 0x0F) | (data << 4);	// send lsb data
		LCD_PRT |= (1 << LCD_EN);					// make EN = 1
		_delay_us(100);
		LCD_PRT &= ~(1 << LCD_EN);					// EN = 0 
	}
	
	// cmdSend() - sends command to LCD
	// param : cmd - predefined command 
	void cmdSend(uint8_t cmd) {
		// 4 msb bits, LCD enable, select cmd register, write
		LCD_PRT = ((cmd & 0xF0) | (1 << LCD_EN)) & ~(1 << LCD_RW) & ~(1 << LCD_RS);
		_delay_us(100);
		LCD_PRT &= ~(1 << LCD_EN);				// EN = 0
		_delay_us(100);
		LCD_PRT = (LCD_PRT & 0x0F) | (cmd << 4);// send lsb cmd
		LCD_PRT |= (1 << LCD_EN);				// make EN = 1
		_delay_us(100);
		LCD_PRT &= ~(1 << LCD_EN);				// EN = 0
	}
	
	// print() - sends string to display 
	// param : str - sequence of characters
	void print(const char* str) {
		clearDisplay();							// clear display
		_delay_ms(1);
		for(int i = 0; str[i] != '\0'; i ++) {	// till end of string
			dataSend(str[i]);					// send each character
		}
	}
};