#ifndef __EOS_INTERNAL_EVENT__
#define __EOS_INTERNAL_EVENT__

#include "include/event.h"
#include "include/error.h"

#include "internal/gpio.h"

typedef struct {
	event_type_t type;
	union {
		struct { pin_t pin; bool state; } pin_state_change_data;
	};
} event_t;

error_t _eos_push_event(event_t event);
void _eos_process_events();

#endif // !__EOS_INTERNAL_EVENT__
