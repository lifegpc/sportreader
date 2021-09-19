#include "iso8601.h"
#include <string.h>
#include "pcre.h"
#include <malloc.h>
#include "stdio_config.h"
#include "stdlib_config.h"
#include "util.h"
#include "log.h"

pcre *pa = NULL;
pcre *pa2 = NULL;

#define MAX_VECT_COUNT 42
#define PRINT_BUFF_SIZE 40

void copySTime(s_time *dest, s_time source) {
	if (dest) {
		dest->tm_hour = source.tm_hour;
		dest->tm_isdst = source.tm_isdst;
		dest->tm_mday = source.tm_mday;
		dest->tm_min = source.tm_min;
		dest->tm_mon = source.tm_mon;
		dest->tm_ms = source.tm_ms;
		dest->tm_sec = source.tm_sec;
		dest->tm_wday = source.tm_wday;
		dest->tm_yday = source.tm_yday;
		dest->tm_year = source.tm_year;
		dest->tm_zone_offset = source.tm_zone_offset;
	}
}

void initSTime(s_time *t) {
	if (t == NULL) return;
	t->tm_hour = 0;
	t->tm_isdst = 0;
	t->tm_mday = 1;
	t->tm_min = 0;
	t->tm_mon = 0;
	t->tm_ms = 0;
	t->tm_sec = 0;
	t->tm_wday = 4;
	t->tm_yday = 0;
	t->tm_year = 70;
	t->tm_zone_offset = 0;
}

int parseISO8601(char* time, s_time* result) {
	if (result == NULL) return 1;
	initSTime(result);
	time_t temp2 = 0;
	struct tm temp;
	if (gmtime_s(&temp, &temp2)) return 1;
	size_t timel = strlen(time);

	int vect[MAX_VECT_COUNT];
	
	if (pa == NULL) {
		const char *errmsg = "";
		int erroffset;
		char *pattern = "^(\\+|-)?(\\d{4})-?(\\d{2})?-?(\\d{2})?$";

		pa = pcre_compile(pattern, 0, &errmsg, &erroffset, NULL);
		if (!pa) {
			libsr_log_error("Compile regex pattern failed at offset %d: %s.", erroffset, errmsg);
			exit(-1);
		}
	}

	if (pa2 == NULL) {
		const char *errmsg = "";
		int erroffset;
		char *pattern = "^(\\+|-)?(\\d{4})-?(\\d{2})-?(\\d{2})T(\\d{2}):?(\\d{2}):?(\\d{2})(\\.(\\d+)?)?(Z|[+-](\\d{2})(:(\\d{2})+)?)$";

		pa2 = pcre_compile(pattern, 0, &errmsg, &erroffset, NULL);
		if (!pa2) {
			libsr_log_error("Compile regex pattern failed at offset %d: %s.", erroffset, errmsg);
			exit(-1);
		}
	}

	int r = pcre_exec(pa, NULL, time, timel, 0, 0, vect, MAX_VECT_COUNT);
	char nega = 0;
	if (r > -1) {
		if (vect[2] > -1) {
			if (time[0] == '-') nega = 1;
		}
		if (vect[4] > -1) {
			extractInt(time, &temp.tm_year, vect[4], vect[5], 0);
		}
		if (vect[6] > -1) {
			extractInt(time, &temp.tm_mon, vect[6], vect[7], 0);
			temp.tm_mon--;
		}
		if (vect[8] > -1) {
			extractInt(time, &temp.tm_mday, vect[8], vect[9], 0);
		}
	} else {
		r = pcre_exec(pa2, NULL, time, timel, 0, 0, vect, MAX_VECT_COUNT);
		if (r > -1) {
			char tz_nega = 0;
			if (vect[2] > -1) {
				if (time[0] == '-') nega = 1;
			}
			if (vect[4] > -1) {
				extractInt(time, &temp.tm_year, vect[4], vect[5], 0);
			}
			if (vect[6] > -1) {
				extractInt(time, &temp.tm_mon, vect[6], vect[7], 0);
				temp.tm_mon--;
			}
			if (vect[8] > -1) {
				extractInt(time, &temp.tm_mday, vect[8], vect[9], 0);
			}
			if (vect[10] > -1) {
				extractInt(time, &temp.tm_hour, vect[10], vect[11], 0);
			}
			if (vect[12] > -1) {
				extractInt(time, &temp.tm_min, vect[12], vect[13], 0);
			}
			if (vect[14] > -1) {
				extractInt(time, &temp.tm_sec, vect[14], vect[15], 0);
			}
			if (vect[18] > -1) {
				extractInt(time, &result->tm_ms, vect[18], vect[19], 3);
			}
			if (vect[20] > -1) {
				if (time[vect[20]] == '-') tz_nega = 1;
			}
			if (vect[22] > -1) {
				int tz_hours = 0;
				extractInt(time, &tz_hours, vect[22], vect[23], 0);
				result->tm_zone_offset += tz_hours * 3600;
			}
			if (vect[26] > -1) {
				int tz_mins = 0;
				extractInt(time, &tz_mins, vect[26], vect[27], 0);
				result->tm_zone_offset += tz_mins * 60;
			}
			if (tz_nega) result->tm_zone_offset = -result->tm_zone_offset;
		}
	}
	if (nega) temp.tm_year = -temp.tm_year;
	temp.tm_year -= 1900;
	mktime(&temp);
	copyTMToSTime(result, temp);
	return 0;
}

long long gmSTime(s_time t) {
	struct tm temp;
	copySTimeToTM(&temp, t);
	time_t s = mkgmtime(&temp);
	s -= t.tm_zone_offset;
	return s * 1000 + t.tm_ms;
}

int mkSTime(s_time *t) {
	if (t == NULL) return 1;
	if (t->tm_ms >= 1000) {
		t->tm_sec += t->tm_ms / 1000;
		t->tm_ms = t->tm_ms % 1000;
	}
	struct tm temp;
	copySTimeToTM(&temp, *t);
	mktime(&temp);
	copyTMToSTime(t, temp);
	return 0;
}

int STimeToStr(s_time t, char* buff, size_t buff_size) {
	if (buff == NULL) return 1;
	char *buf = buff;
	if (mkSTime(&t)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_year + 1900, 4)) return 1;
	if (copyCharToBuff(&buf, &buff_size, "-", 1)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_mon + 1, 2)) return 1;
	if (copyCharToBuff(&buf, &buff_size, "-", 1)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_mday, 2)) return 1;
	if (copyCharToBuff(&buf, &buff_size, "T", 1)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_hour, 2)) return 1;
	if (copyCharToBuff(&buf, &buff_size, ":", 1)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_min, 2)) return 1;
	if (copyCharToBuff(&buf, &buff_size, ":", 1)) return 1;
	if (copyIntToBuff(&buf, &buff_size, t.tm_sec, 2)) return 1;
	if (t.tm_ms > 0) {
		if (copyCharToBuff(&buf, &buff_size, ".", 1)) return 1;
		if (copyIntToBuff(&buf, &buff_size, t.tm_ms, 3)) return 1;
	}
	if (t.tm_zone_offset == 0) {
		if (copyCharToBuff(&buf, &buff_size, "Z", 1)) return 1;
	} else {
		int offset = t.tm_zone_offset;
		if (offset < 0) {
			if (copyCharToBuff(&buf, &buff_size, "-", 1)) return 1;
			offset = -offset;
		} else {
			if (copyCharToBuff(&buf, &buff_size, "+", 1)) return 1;
		}
		int hours = offset / 3600, mins = (offset % 3600) / 60;
		if (copyIntToBuff(&buf, &buff_size, hours, 2)) return 1;
		if (copyCharToBuff(&buf, &buff_size, ":", 1)) return 1;
		if (copyIntToBuff(&buf, &buff_size, mins, 2)) return 1;
	}
	if (buff_size == 0) return 1;
	*buf = 0;
	return 0;
}

int STimeToLocalTime(s_time *dest, s_time source) {
	if (dest == NULL) return 1;
	int tz_offset = getTzOffset();
	copySTime(dest, source);
	dest->tm_sec += tz_offset - source.tm_zone_offset;
	dest->tm_zone_offset = tz_offset;
	return 0;
}

int STimeToLocalTimeStr(s_time t, char* buff, size_t buff_size) {
	if (buff == NULL) return 1;
	int tz_offset = getTzOffset();
	s_time temp;
	if (STimeToLocalTime(&temp, t)) return 1;
	return STimeToStr(temp, buff, buff_size);
}

int printSTime(s_time t, int new_line) {
	char buff[PRINT_BUFF_SIZE];
	if (STimeToStr(t, buff, PRINT_BUFF_SIZE)) return 1;
	char *f = new_line ? "%s\n" : "%s";
	printf_s(f, buff);
	return 0;
}

int printSTimeInLocalTime(s_time t, int new_line) {
	char buff[PRINT_BUFF_SIZE];
	if (STimeToLocalTimeStr(t, buff, PRINT_BUFF_SIZE)) return 1;
	char *f = new_line ? "%s\n" : "%s";
	printf_s(f, buff);
	return 0;
}

void copySTimeToTM(struct tm *dest, s_time source) {
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

void copyTMToSTime(s_time *dest, struct tm source) {
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

void freeRegexCache() {
	if (pa != NULL) {
		free(pa);
		pa = NULL;
	}
	if (pa2 != NULL) {
		free(pa2);
		pa2 = NULL;
	}
}
