#ifndef _LIBSR_TRACKPOINT_H
#define _LIBSR_TRACKPOINT_H

#include "iso8601.h"
#include "position.h"

typedef enum SensorState_t{
	  SensorState_Present
	, SensorState_Absent
} SensorState_t;

typedef struct Trackpoint {
	s_time Time;
	Position *pos;
	double *AltitudeMeters;
	double *DistanceMeters;
	unsigned char *HeartRateBpm;
	unsigned char *Cadence;
	SensorState_t *SensorState;
	struct Trackpoint *prev;
	struct Trackpoint *next;
	int(*setPosition) (struct Trackpoint *tr, double latitude, double longitude);
	int(*setAltitudeMeters) (struct Trackpoint *tr, double AltitudeMeters);
	int(*setDistanceMeters) (struct Trackpoint *tr, double DistanceMeters);
	int(*setHeartRateBpm) (struct Trackpoint *tr, unsigned char HeartRateBpm);
	int(*setCadence) (struct Trackpoint *tr, unsigned char Cadence);
	int(*setSensorState) (struct Trackpoint *tr, SensorState_t state);
	int(*setSensorStateByStr) (struct Trackpoint *tr, char* s);
	const char *(*getSensorStateStr) (struct Trackpoint *tr);
} Trackpoint;

/*Create a new trackpoint. Need free memory by calling free_trackpoint.*/
LIBSR_PUBLIC Trackpoint *new_trackpoint(s_time t);
LIBSR_PUBLIC void free_trackpoint(Trackpoint *t);
LIBSR_PUBLIC void free_all_trackpoints_from_first(Trackpoint *first);
LIBSR_PUBLIC int getSensorStateFromStr(SensorState_t *r, char *s);
LIBSR_PUBLIC const char *getStrFromSensorState(SensorState_t s);

#endif
