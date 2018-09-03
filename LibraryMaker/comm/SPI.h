# ifndef AVRIO
	# define AVRIO 1 
	# include <avr/io.h>
# endif

# define SPI_MASTER 0x01	
# define SPI_SLAVE 0x00

// pin numbers
# define MOSI 5			
# define MISO 6			
# define SCK 7	

class SPI
{
		public:
		// constructors
		SPI() {
			SPI(SPI_MASTER);		// init with MASTER mode
		}
		
		SPI(uint8_t mode) {
			if(mode) {				// MASTER MODE
				initMaster();
			}
			else {					// SLAVE MODE
				initSlave();
			}
		}

		// initMaster() - initializes as Master
		void initMaster() {
			DDRB |= (1 << MOSI) | (1 << SCK);				// set MOSI and SCK as O/P
			SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);	//SPI Enable, Master Mode, Fosc/16	
		}
		
		// initSlave() - initializes as Slave
		void initSlave() {
			DDRB = (1 << MISO);								// set MISO as O/P
			SPCR = (1 << SPE);								//SPI Enable, Slave Mode, Fosc given by master
		}
		
		// send() - send a byte through SPI
		// param : ch - data byte
		void send(uint8_t ch) {
			SPDR = ch;										// copy byte to SPDR
			while(! (SPSR & (1 << SPIF)));					// wait till transfer completes
		}
		
		// receive() - receive a byte on SPI
		// returns : received byte
		uint8_t receive() {
			SPDR = 0xFF;									// pass any byte to SPDR
			while(! (SPSR & (1 << SPIF)));					// wait till transfer completes
			return SPDR;									// return content (received byte) in SPDR
		}
		
		// transfer() - sends a byte and returns received byte
		// param : data - data to send
		// returns : received byte
		uint8_t transfer(uint8_t data)
		{
			SPDR = data;									// copy byte to SPDR
			while(! (SPSR & (1 << SPIF)));					// wait till transfer completes
			return SPDR;									// return received content after transfer
		}
};