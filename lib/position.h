#ifndef _LIBSR_POSITION_H
#define _LIBSR_POSITION_H

#include "public.h"

typedef struct Position {
	double LatitudeDegrees;
	double LongitudeDegrees;
	int(*setLatitude) (struct Position *pos, double latitude);
	int(*setLongitude) (struct Position *pos, double longitude);
} Position;

/*Create a position. Need free memory by using free_position.*/
LIBSR_PUBLIC Position *new_position(double latitude, double longitude);
LIBSR_PUBLIC void free_position(Position *pos);

#endif
