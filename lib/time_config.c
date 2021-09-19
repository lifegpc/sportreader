#include "time_config.h"
#include <malloc.h>

void copy_tm(struct tm* dest, struct tm source) {
	if (dest) {
		dest->tm_hour = source.tm_hour;
		dest->tm_isdst = source.tm_isdst;
		dest->tm_mday = source.tm_mday;
		dest->tm_min = source.tm_min;
		dest->tm_mon = source.tm_mon;
		dest->tm_sec = source.tm_sec;
		dest->tm_wday = source.tm_wday;
		dest->tm_yday = source.tm_yday;
		dest->tm_year = source.tm_year;
	}
}

#if !HAVE_GMTIME_S
int gmtime_s(struct tm* dest, const time_t* sourceTime) {
	struct tm* source = gmtime(sourceTime);
	if (source && dest) {
		copy_tm(dest, *source);
		return 0;
	}
	return 1;
}
#endif

#if !HAVE_LOCALTIME_S
int localtime_s(struct tm* const dest, const time_t* sourceTime) {
	struct tm* source = localtime(sourceTime);
	if (source && dest) {
		copy_tm(dest, *source);
		return 0;
	}
	return 1;
}
#endif

int *tz_offset = NULL;

void setTzOffset() {
	if (tz_offset == NULL) {
		tz_offset = (int*)malloc(sizeof(int));
		*tz_offset = 0;
	}
	time_t zero = 0;
	struct tm lt;
	if (localtime_s(&lt, &zero)) return;
	*tz_offset = lt.tm_sec + (lt.tm_min + (lt.tm_hour * 60)) * 60;
}

int getTzOffset() {
	if (tz_offset == NULL) setTzOffset();
	return *tz_offset;
}

time_t mkgmtime(struct tm *t) {
	if (t == NULL) return 0;
#if HAVE__MKGMTIME
	return _mkgmtime(t);
#elif HAVE_TIMEGM
	return timegm(t);
#else
	if (tz_offset == NULL) setTzOffset();
	return mktime(t) + *tz_offset;
#endif
}
