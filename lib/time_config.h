#ifndef _TIME_CONFIG_H
#define _TIME_CONFIG_H

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <time.h>

void copy_tm(struct tm* dest, struct tm source);

#if !HAVE_GMTIME_S
int gmtime_s(struct tm* dest, const time_t* sourceTime);
#endif

#if !HAVE_LOCALTIME_S
int localtime_s(struct tm* const dest, const time_t* sourceTime);
#endif

void setTzOffset();
int getTzOffset();
time_t mkgmtime(struct tm *t);

#endif
