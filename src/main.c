#include <avr/io.h>
#include <util/delay.h>
#include "include/blink.h"

int main() {
	DDRB |= (1 << PB5);
	blink();
}
