// number of timer cycle (from 0x00 to 0xFF) for one second @ 14.7456MHz clock / 1024
#define secondcycle 57	
#include <avr/interrupt.h>

// ISR for Timer0 overflow 
// copy it wherever used to count seconds 
// provided Timer 's object is timer
/*ISR(TIMER0_OVF_vect)
{
	timer.overflow();
}
*/

// this class is used for using timer to do some task for n second(s) using interrupt
class Timer
{
	uint8_t overflow_flag;					// used to count number of clock cycles
	uint8_t curr_sec;						// number of seconds completed
	uint8_t target;							// target seconds
	volatile uint8_t timeup;				// if timeout has occurred
	
	public:
	// default constructor
	Timer() {
		// initialise
		overflow_flag = 0;
		curr_sec = 0;
		target = 0;
		timeup = 0;
		init();
	}	
	
	// init () - initialise by clearing Timer0 overflow flag and enabling interrupt for same
	void init() {
		clearFlag();					// clear interrupt flag for precaution sake
		enableInterrupt();				// enable interrupt
	}
	
	// enableInterrupt() - enable Timer0 overflow and global interrupt
	void enableInterrupt() {
		TIMSK0 |= (1 << TOIE0);			// enable Timer0 overflow interrupt
		sei();							// enable global interrupt
	}
	
	// clearFlag() - clear timer overflow flag
	void clearFlag() {
		TIFR0 |= (1 << TOV0);			// clear TOV0 flag
	}
	
	// startTimer() - start timer for t seconds
	// param : t - seconds 
	void startTimer(uint8_t t) {
		// set variables
		overflow_flag = 0;
		curr_sec = 0;
		timeup = 0;
		target = t;								// set target as t seconds
		TCCR0B |= (1 << CS02) | (1 << CS00);	// plug clock as CLK/1024
		TCNT0 = 0;								// set timer/counter0 to 0
	}
	
	// stopTimer() - stop timer
	void stopTimer() {
		TCCR0B &= ~((1 << CS02) | (1 << CS00)); // unplug the clock
		clearFlag();							// clear the flag, in some worst case scenario.. not required
	} 
	
	// overflow() - called when timer overflows
	void overflow() {
		overflow_flag ++;						// increment overflow_flag
		if(overflow_flag >= secondcycle) {		// if overflow_flag has reached to secondcycle
			overflow_flag = 0;					// reset overflow_flag
			curr_sec ++;						// increment seconds count
			if(curr_sec >= target) {				// if current second count has reached target
				timeup = 1;						// set timeup = 1
				stopTimer();					// stop timer
			}
		}
	}
	
	// timeout() - used externally to check if target seconds have passed
	// returns : if target seconds have passed
	uint8_t timeout() {
		return timeup;							// if timeout has occurred 
	}
	
};