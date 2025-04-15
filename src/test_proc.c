#include "include/event.h"
#include "include/gpio.h"
#include <util/delay.h>

const pin_t pin_out = (pin_t){
	.group = PG_C,
	.number = PN_3,
};
const pin_t pin_in = (pin_t){
	.group = PG_D,
	.number = PN_4,
};

void handle_pin_state_change(pin_t pin, bool state) {
	eos_pin_toggle(pin_out);
}

void init() {
	eos_config_pin_output(pin_out);
	eos_config_pin_input(pin_in, true, PE_BOTH_EDGES);
	eos_set_pin_state(pin_out, true);

	eos_set_event_handler(EVENT_PIN_STATE_CHANGE, (void(*)(void))(&handle_pin_state_change));
}

