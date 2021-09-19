#ifndef _XML_H
#define _XML_H

#include "tcd.h"

typedef enum XMLStatus {
	PARSE_OK
	, FAILED_PARSE
	, NO_ROOT_ELEMENT
	, WRONG_ROOT_ELEMENT
	, TOO_MANY_ACTIVITIES_ELEMENT
} XMLStatus;

typedef struct XMLParseResult {
	XMLStatus status;
	TCD *tcd;
} XMLParseResult;

LIBSR_PUBLIC XMLParseResult readXML(char* filename);

#endif
