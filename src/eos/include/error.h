#ifndef __EOS_INCLUDE_ERROR__
#define __EOS_INCLUDE_ERROR__

#include "include/types.h"

typedef enum : u8 {
	ERR_NONE = 0,
	ERR_PB5_IS_USED_FOR_ERROR_INDICATION,
	ERR_EVENT_QUEUE_FULL,
	ERR_INVALID_ARGUMENT,
	ERR_PIN_IS_BUSY,
} error_t;

void eos_enable_error_indication();
void eos_indicate_error(error_t err);

#endif // !__EOS_INCLUDE_ERROR__
