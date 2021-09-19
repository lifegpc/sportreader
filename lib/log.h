#ifndef _LIBSR_LOG_H
#define _LIBSR_LOG_H

#include "public.h"

typedef enum LIBSR_LOG_LEVEL {
	  LIBSR_QUIET_LEVEL
	, LIBSR_ERROR_LEVEL
	, LIBSR_WARNING_LEVEL
	, LIBSR_VERBOSE_LEVEL
} LIBSR_LOG_LEVEL;

LIBSR_PUBLIC void set_libsr_log_level(LIBSR_LOG_LEVEL level);
LIBSR_PUBLIC int libsr_log_verbose(char *s, ...);
LIBSR_PUBLIC int libsr_log_warning(char *s, ...);
LIBSR_PUBLIC int libsr_log_error(char *s, ...);

#endif
