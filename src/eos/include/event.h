#ifndef __EOS_INCLUDE_EVENT__
#define __EOS_INCLUDE_EVENT__

typedef enum {
	EVENT_PIN_STATE_CHANGE,
} event_type_t;

typedef void (*event_handler_t)(void);
void eos_set_event_handler(event_type_t event, event_handler_t handler);

#endif // !__EOS_INCLUDE_EVENT__
