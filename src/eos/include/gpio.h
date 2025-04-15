#ifndef __EOS_INCLUDE_GPIO__
#define __EOS_INCLUDE_GPIO__

#include "include/types.h"

typedef enum : u8 {
	PN_0 = (1u << 0u),
	PN_1 = (1u << 1u),
	PN_2 = (1u << 2u),
	PN_3 = (1u << 3u),
	PN_4 = (1u << 4u),
	PN_5 = (1u << 5u),
	PN_6 = (1u << 6u),
	PN_7 = (1u << 7u),
} pin_number_t;

typedef enum : u8 {
	PG_B = 0,
	PG_C = 1,
	PG_D = 2,
} pin_group_t;

typedef struct {
	pin_number_t number;
	pin_group_t group;
} pin_t;

typedef enum {
	PE_BOTH_EDGES = 0,
	PE_RISING_EDGE = 1,
	PE_FALLING_EDGE = 2,
} pin_input_edge_t;

void eos_config_pin_output(pin_t pin);
void eos_config_pin_input (pin_t pin, bool pullup, pin_input_edge_t edge);
void eos_config_pin_pwm(pin_t pin);
void eos_config_pin_adc(); //TODO:

void eos_set_pin_state(pin_t pin, bool state);
void eos_pin_toggle(pin_t pin);

#endif // !__EOS_INCLUDE_GPIO__
