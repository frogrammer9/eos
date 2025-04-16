#include "internal/event.h"
#include "internal/gpio.h"
#include <avr/interrupt.h>

void init();

void kinit() {
	_eos_gpio_init();
	init();
	sei();
}

int main() {
	kinit();
	while(1) {
		_eos_process_events();
		//TODO: Sleep
	}
}
