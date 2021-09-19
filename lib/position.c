#include "position.h"
#include <malloc.h>

int setLatitude(Position *pos, double latitude) {
	if (pos == NULL || latitude < -90 || latitude > 90) return 1;
	pos->LatitudeDegrees = latitude;
	return 0;
}

int setLongitude(Position *pos, double longitude) {
	if (pos == NULL || longitude < -180 || longitude > 180) return 1;
	pos->LongitudeDegrees = longitude;
	return 0;
}

Position *new_position(double latitude, double longitude) {
	if (latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) return NULL;
	Position *pos = (Position*)malloc(sizeof(Position));
	pos->LatitudeDegrees = latitude;
	pos->LongitudeDegrees = longitude;
	pos->setLatitude = &setLatitude;
	pos->setLongitude = &setLongitude;
	return pos;
}

void free_position(Position *pos) {
	if (pos == NULL) return;
	pos->setLatitude = NULL;
	pos->setLongitude = NULL;
	free(pos);
}
