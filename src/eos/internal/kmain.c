#include "internal/event.h"

void init();

void kinit() {

	init();
}

int main() {
	kinit();
	while(1) {
		_eos_process_events();
		//TODO: Sleep
	}
}
