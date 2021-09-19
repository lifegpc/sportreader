#ifndef _STDIO_CONFIG_H
#define _STDIO_CONFIG_H

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#if !HAVE_PRINTF_S
#define printf_s printf
#endif

#if !HAVE_VPRINTF_S
#define vprintf_s vprintf
#endif

#if !HAVE_SSCANF_S
#define sscanf_s sscanf
#endif
#endif
