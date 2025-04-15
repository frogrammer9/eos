#include "include/error.h"
#include <avr/io.h>

void eos_enable_error_indication() {

}

void eos_indicate_error(error_t err) {
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	for(;;);
}

