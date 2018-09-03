
# ifndef AVRIO
	# define AVRIO 1
	# include <avr/io.h>
# endif

# define I2C_MASTER 0x01
# define I2C_SLAVE 0x00 

class I2C
{
	// for this class, it is assumed that system isn't critical and 
	// there are not more than one master on I2C bus
	// thus, we won't check status flag while proceeding
	public:

	I2C() {
		I2C(I2C_MASTER);
	}
	
	I2C(uint8_t mode) {
		if(mode) {						// MASTER MODE
			initMaster();
		}
		else {
			I2C(I2C_SLAVE, 0x10);		// slave address - 0x10
		}
	} 
	
	I2C(uint8_t mode, uint8_t slaveAddress) {
		if(mode) {
			initMaster();				// most likely not called here
		}
		else {
			initSlave(slaveAddress);	// init slave with slave addr = slaveAddress
		}
	}
	
	// initMaster() - initialise module as master
	void initMaster() {
		TWSR = 0x00;					// set prescalar bits to zero
		TWBR = 0x8C;					// SCL freq. = 50 K for XTAL = 14.7456MHz
		TWCR = 0x04;					// enable TWI module
	}
	
	// initSlave() - initialise module as slave
	// param : slaveAddr - slave address
	void initSlave(uint8_t slaveAddr) {
		TWCR = 0x04;					//enable TWI module
		TWAR = slaveAddr;				// set slave address
		listen();						// wait to complete
	}
	
	// start() - start transfer
	void start() {
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);	// start transaction
		listen();											// wait to complete
	}
	
	// opComplete() - check if operation is complete
	// returns : has operation complete
	uint8_t opComplete() {
		return (TWCR & (1 << TWINT));						// return if current operation if complete
	}
	
	// stop() - stop/end current operation
	void stop() {
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	// stop by setting TWSTO bit
	}
	
	// write() - send data byte
	// param : data - data byte to send
	void write(uint8_t data) {
		TWDR = data;										// copy data to TWDR
		TWCR = (1 << TWINT) | (1 << TWEN);					// start writing
		listen();
	}
	
	// read() - returns received byte
	// param : last_byte - 1 if receiving byte is last
	// returns : depends on last_byte
	uint8_t read(uint8_t last_byte) {
		if(last_byte) {										// if last byte to read
			return readLast();
		}
		else {												// else, need to send ACK
			return read();
		}
	}
	
	// readLast() - returns received byte and doesn't transmit ACK
	// returns : received byte
	uint8_t readLast() {
		TWCR = (1 << TWINT) | (1 << TWEN);					// start reading
		listen();
		return TWDR;										// return content of TWDR
	}
	
	// read() - returns received byte and replies with ACK
	// returns : received byte
	uint8_t read() {
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);	// start reading, enable ACK
		listen();
		return TWDR;										// return content of TWDR
	}
	
	// listen() - wait till operation completes on channel
	void listen() {
		while(! opComplete());								// wait till op is not complete
	}

};