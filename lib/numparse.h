#ifndef _NUM_PARSE_H
#define _NUM_PARSE_H

typedef enum NumberType {
	  NUM_UNKNOWN
	, NUM_INT
	, NUM_DOUBLE
} NumberType;

NumberType getNumTypeFromStr(char *s);
int parseLLFromStr(long long *num, char* s);
int parseDFromStr(double *num, char* s);
int parseULLFromStr(unsigned long long *num, char* s);
int parseLFromStr(long *num, char *s);
int parseFFromStr(float *num, char* s);
int parseULFromStr(unsigned long *num, char *s);
int parseIFromStr(int *num, char *s);
int parseUIFromStr(unsigned int *num, char *s);
int parseSFromStr(short *num, char *s);
int parseUSFromStr(unsigned short *num, char *s);
int parseCFromStr(char *num, char *s);
int parseUCFromStr(unsigned char *num, char *s);

#endif
