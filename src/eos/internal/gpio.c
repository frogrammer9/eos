#include "internal/gpio.h"
#include "include/error.h"
#include "include/event.h"
#include "internal/event.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static u8 pin_output[3] = {0};
static u8 pin_reserved[3] = {0};
static u8 pin_input_rising[3] = {0};
static u8 pin_input_falling[3] = {0};
static u8 pin_adc = 0;
static u8 pin_pwm = 0; //this will need to be goofy :3 

void handle_PCINT(pin_group_t pg) {
	static u8 state_copy[3] = {0};
	const u8 state = (u8[3]){PINB, PINC, PIND}[pg];
	const u8 state_change = state_copy[pg] ^ state;
	state_copy[pg] = state;
	for(u8 i = 0; i < 8; ++i) {
		const u8 mask = (1u << i);
		const event_t event = (event_t){
			.type = EVENT_PIN_STATE_CHANGE,
			.pin_state_change_data.pin = (pin_t){
				.number = mask,
				.group = pg,
			},
			.pin_state_change_data.state = (state & mask) >> i
		};
		if(state_change & mask) {
			if((state & mask) == 0) { if(pin_input_falling[pg] & mask) _eos_push_event(event); }
			else if((state & mask) == 1) { if(pin_input_rising[pg]  & mask)  _eos_push_event(event); }
		}
	}
}

ISR(PCINT0_vect) { handle_PCINT(PG_B); }
ISR(PCINT1_vect) { handle_PCINT(PG_C); }
ISR(PCINT2_vect) { handle_PCINT(PG_D); }

void _eos_gpio_init() {
	PCICR = 0x07;
}

void free_pin(pin_t pin) {
	if(pin.group == PG_C) pin_adc &= ~pin.number;
	pin_output[pin.group] &= ~pin.number;
	pin_input_rising[pin.group] &= ~pin.number;
	pin_input_falling[pin.group] &= ~pin.number;
	switch(pin.group) {
		case PG_B: PCMSK0 &= ~pin.number; break;
		case PG_C: PCMSK0 &= ~pin.number; break;
		case PG_D: PCMSK0 &= ~pin.number; break;
	}
	//TODO: free pwn
}

void eos_config_pin_output(pin_t pin) {
	free_pin(pin);
	pin_output[pin.group] |= pin.number;
	if(pin_reserved[pin.group] & pin.number) { eos_indicate_error(ERR_PIN_IS_BUSY); return; }
	switch(pin.group) {
		case PG_B: DDRB |= pin.number; break;
		case PG_C: DDRC |= pin.number; break;
		case PG_D: DDRD |= pin.number; break;
	}
}

void eos_config_pin_input (pin_t pin, bool pullup, pin_input_edge_t edge) {
	free_pin(pin);
	if(pin_reserved[pin.group] & pin.number) { eos_indicate_error(ERR_PIN_IS_BUSY); return; }
	if((edge & 1) == 0) pin_input_falling[pin.group] |= pin.number;
	if((edge & 2) == 0) pin_input_rising[pin.group] |= pin.number;
	switch(pin.group) {
		case PG_B: DDRB &= ~pin.number; if(pullup) PORTB |= pin.number; break;
		case PG_C: DDRC &= ~pin.number; if(pullup) PORTC |= pin.number; break;
		case PG_D: DDRD &= ~pin.number; if(pullup) PORTD |= pin.number; break;
	}
}

void eos_set_pin_state(pin_t pin, bool state) {
	if((pin_output[pin.group] & pin.number) == 0) { eos_indicate_error(ERR_INVALID_ARGUMENT); return; }
	switch(pin.group) {
        case PG_B: if(state) PORTB |= pin.number; else PORTB &= ~pin.number; PCMSK0 |= pin.number; break;
        case PG_C: if(state) PORTC |= pin.number; else PORTC &= ~pin.number; PCMSK1 |= pin.number; break;
        case PG_D: if(state) PORTD |= pin.number; else PORTD &= ~pin.number; PCMSK2 |= pin.number; break;
	}
}

void eos_pin_toggle(pin_t pin) {
	if((pin_output[pin.group] & pin.number) == 0) { eos_indicate_error(ERR_INVALID_ARGUMENT); return; }
	switch(pin.group) {
        case PG_B: PORTB ^= pin.number; break;
        case PG_C: PORTC ^= pin.number; break;
        case PG_D: PORTD ^= pin.number; break;
	}
}

