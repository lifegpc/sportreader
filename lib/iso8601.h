#if HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _LIBSR_ISO8601_H
#define _LIBSR_ISO8601_H

#include "public.h"

#if HAVE_CONFIG_H
#include "time_config.h"
#else
#include <time.h>
#endif

struct s_time {
	/*milliseconds. [0, 999]*/
	int tm_ms;
	/*The seconds of offset of time zone.*/
	int tm_zone_offset;
	struct tm;
};

typedef struct s_time s_time;

LIBSR_PUBLIC void copySTime(s_time *dest, s_time source);
/*Initialize a new s_time struct.*/
LIBSR_PUBLIC void initSTime(s_time *t);
/*parse ISO8601 string.
0 OK 1 Failed*/
LIBSR_PUBLIC int parseISO8601(char* time, s_time* result);
/*The timestamp. (milliseconds)*/
LIBSR_PUBLIC long long gmSTime(s_time t);
/*Standardize the struct.
0 OK 1 Failed*/
LIBSR_PUBLIC int mkSTime(s_time *t);
/*Generate ISO8601 string.
0 OK 1 Failed*/
LIBSR_PUBLIC int STimeToStr(s_time t, char* buff, size_t buff_size);
/*Convert stime in localtime.
0 OK 1 Failed*/
LIBSR_PUBLIC int STimeToLocalTime(s_time *dest, s_time source);
/*Generate ISO8601 string in localtime.
0 OK 1 Failed*/
LIBSR_PUBLIC int STimeToLocalTimeStr(s_time t, char* buff, size_t buff_size);
/*Print ISO8601 string to stdout.
0 OK 1 Failed*/
LIBSR_PUBLIC int printSTime(s_time t, int new_line);
/*Print ISO8601 string to stdout in localtime.
0 OK 1 Failed*/
LIBSR_PUBLIC int printSTimeInLocalTime(s_time t, int new_line);
LIBSR_PUBLIC void copySTimeToTM(struct tm *dest, s_time source);
LIBSR_PUBLIC void copyTMToSTime(s_time *dest, struct tm source);
void freeRegexCache();

#endif
