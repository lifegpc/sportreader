#ifndef _STDLIB_CONFIG_H
#define _STDLIB_CONFIG_H

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#if !HAVE_MIN
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
