#include "flags.h"

unsigned long long libsr_flags = 0;

void set_libsr_flags(unsigned long long flags) {
	libsr_flags = flags;
}
