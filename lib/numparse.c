#include "numparse.h"
#include "pcre.h"
#include "stdio_config.h"
#include "log.h"
#include <string.h>
#include <math.h>

pcre *num_pa = NULL;

#define MAX_VECT_COUNT 21

int parseDFromStrT(double *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	int r = sscanf_s(s, "%lf", num);
	return r <= 0 ? 1 : 0;
}

int parseLLFromStrT(long long *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	int r = sscanf_s(s, "%lld", num);
	return r <= 0 ? 1 : 0;
}

int parseULLFromStrT(unsigned long long *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	int r = sscanf_s(s, "%llu", num);
	return r <= 0 ? 1 : 0;
}

NumberType getNumTypeFromStr(char *s) {
	if (s == NULL) return NUM_UNKNOWN;
	if (num_pa == NULL) {
		const char *errmsg = "";
		int erroffset;
		char* pat = "^[+-]?(\\d+)(\\.(\\d+))?([eE](\\d+))?$";

		num_pa = pcre_compile(pat, 0, &errmsg, &erroffset, NULL);
		if (!num_pa) {
			libsr_log_error("Compile regex pattern failed at offset %d: %s.", erroffset, errmsg);
			exit(-1);
		}
	}

	int vect[MAX_VECT_COUNT];
	int r = pcre_exec(num_pa, NULL, s, strlen(s), 0, 0, vect, MAX_VECT_COUNT);
	if (r > -1) {
		if (vect[8] > -1 || vect[4] > -1) return NUM_DOUBLE;
		return NUM_INT;
	}
	return NUM_UNKNOWN;
}

int parseLLFromStr(long long *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	NumberType type = getNumTypeFromStr(s);
	if (type == NUM_UNKNOWN) return 1;
	if (type == NUM_INT) {
		return parseLLFromStrT(num, s);
	} else {
		double r = 0;
		if (parseDFromStrT(&r, s)) return 1;
		*num = llround(r);
		return 0;
	}
}

int parseDFromStr(double *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	NumberType type = getNumTypeFromStr(s);
	if (type == NUM_UNKNOWN) return 1;
	if (type == NUM_INT) {
		long long i;
		if (parseLLFromStrT(&i, s)) return 1;
		*num = i;
		return 0;
	} else {
		return parseDFromStrT(num, s);
	}
}

int parseULLFromStr(unsigned long long *num, char* s) {
	if (s == NULL || num == NULL) return 1;
	NumberType type = getNumTypeFromStr(s);
	if (type == NUM_UNKNOWN) return 1;
	if (type == NUM_INT) {
		return parseULLFromStrT(num, s);
	} else {
		double r = 0;
		if (parseDFromStrT(&r, s)) return 1;
		*num = llround(r);
		return 0;
	}
}

int parseLFromStr(long *num, char *s) {
	long long n = 0;
	if (parseLLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseFFromStr(float *num, char* s) {
	double n = 0;
	if (parseDFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseULFromStr(unsigned long *num, char *s) {
	unsigned long long n = 0;
	if (parseULLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseIFromStr(int *num, char *s) {
	long long n = 0;
	if (parseLLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseUIFromStr(unsigned int *num, char *s) {
	unsigned long long n = 0;
	if (parseULLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseSFromStr(short *num, char *s) {
	long long n = 0;
	if (parseLLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseUSFromStr(unsigned short *num, char *s) {
	unsigned long long n = 0;
	if (parseULLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseCFromStr(char *num, char *s) {
	long long n = 0;
	if (parseLLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}

int parseUCFromStr(unsigned char *num, char *s) {
	unsigned long long n = 0;
	if (parseULLFromStr(&n, s)) return 1;
	*num = n;
	return 0;
}
