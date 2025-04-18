#include <avr/io.h>
#include <util/delay.h>
#include "internal/event.h"

#define EVENT_QUEUE_SIZE 16 //NOTE: must be power of 2
#define EVENT_TYPE_AMOUNT 1

static volatile event_t event_queue[EVENT_QUEUE_SIZE] = {0};
static event_handler_t event_handlers[EVENT_TYPE_AMOUNT] = {NULL};
u8 event_queue_start_inx = 0, event_queue_end_inx = 1;

error_t _eos_push_event(event_t event) {
	if(event_queue_start_inx == event_queue_end_inx) return ERR_EVENT_QUEUE_FULL;
	event_queue[event_queue_end_inx++] = event;
	event_queue_end_inx &= EVENT_QUEUE_SIZE - 1;
	return ERR_NONE;
}

void _eos_process_events() {
	while(((event_queue_start_inx + 1) & (EVENT_QUEUE_SIZE - 1)) != event_queue_end_inx) {
		volatile const event_t* cevent = &event_queue[event_queue_start_inx++];
		event_queue_start_inx &= EVENT_QUEUE_SIZE - 1;
		switch(cevent->type) {
			case EVENT_PIN_STATE_CHANGE:
				if(event_handlers[cevent->type]) (*(void (*)(pin_t, bool))(event_handlers[cevent->type]))(
						cevent->pin_state_change_data.pin,
						cevent->pin_state_change_data.state
					);
			break;
		}
	}
}

void eos_set_event_handler(event_type_t event, event_handler_t handler) {
	event_handlers[event] = handler;
}

