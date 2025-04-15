#include <avr/io.h>
#include <util/delay.h>
#include "include/blink.h"

void blink() {
	while(1) {
		PORTB |= (1 << PB5);
		_delay_ms(1000);
		PORTB &= ~(1 << PB5);
		_delay_ms(1000);
	}
}
