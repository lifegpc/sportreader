#include "log.h"
#include "stdio_config.h"
#include <stdarg.h>

LIBSR_LOG_LEVEL libsr_log_level = LIBSR_WARNING_LEVEL;

void set_libsr_log_level(LIBSR_LOG_LEVEL level) {
	libsr_log_level = level;
}

int libsr_log_verbose(char *s, ...) {
	if (libsr_log_level >= LIBSR_VERBOSE_LEVEL) {
		va_list myargs;
		va_start(myargs, s);
		int ret = vprintf_s(s, myargs);
		va_end(myargs);
		return ret;
	} else return 0;
}

int libsr_log_warning(char *s, ...) {
	if (libsr_log_level >= LIBSR_WARNING_LEVEL) {
		va_list myargs;
		va_start(myargs, s);
		int ret = vprintf_s(s, myargs);
		va_end(myargs);
		return ret;
	} else return 0;
}

int libsr_log_error(char *s, ...) {
	if (libsr_log_level >= LIBSR_ERROR_LEVEL) {
		va_list myargs;
		va_start(myargs, s);
		int ret = vprintf_s(s, myargs);
		va_end(myargs);
		return ret;
	} else return 0;
}
