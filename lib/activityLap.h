#ifndef _LIBSR_ACTIVITY_LAP_H
#define _LIBSR_ACTIVITY_LAP_H

#include "track.h"

typedef enum Intensity_t {
	  Intensity_Active
	, Intensity_Resting
} Intensity_t;

typedef enum TriggerMethod_t {
	  Trigger_Manual
	, Trigger_Distance
	, Trigger_Location
	, Trigger_Time
	, Trigger_HeartRate
} TriggerMethod_t;

typedef struct ActivityLap {
	s_time StartTime;
	double TotalTimeSeconds;
	double DistanceMeters;
	double *MaximumSpeed;
	unsigned short Calories;
	unsigned char *AverageHeartRateBpm;
	unsigned char *MaximumHeartRateBpm;
	Intensity_t Intensity;
	unsigned char *Cadence;
	TriggerMethod_t TriggerMethod;
	Track *first_track;
	Track *last_track;
	char *Notes;
	struct ActivityLap *prev;
	struct ActivityLap *next;
	int(*setMaximumSpeed) (struct ActivityLap *a, double MaximumSpeed);
	int(*setAverageHeartRateBpm) (struct ActivityLap *a, unsigned char AverageHeartRateBpm);
	int(*setMaximumHeartRateBpm) (struct ActivityLap *a, unsigned char MaximumHeartRateBpm);
	int(*setCadence) (struct ActivityLap *a, unsigned char Cadence);
	int(*setNotes) (struct ActivityLap *a, char *s);
	int(*setIntensityByStr) (struct ActivityLap *a, char *s);
	int(*setTriggerMethodByStr) (struct ActivityLap *a, char *s);
	int(*appendTrack) (struct ActivityLap *a, Track *t);
} ActivityLap;

LIBSR_PUBLIC ActivityLap *new_activityLap(s_time StartTime, double TotalTimeSeconds, double DistanceMeters, unsigned short Calories, char *Intensity, char *TriggerMethod);
LIBSR_PUBLIC void free_activityLap(ActivityLap* t);
LIBSR_PUBLIC void free_all_activityLaps_from_first(ActivityLap *first);
LIBSR_PUBLIC int getIntensityFromStr(Intensity_t *r, char *s);
LIBSR_PUBLIC const char *getStrFromIntensity(Intensity_t t);
LIBSR_PUBLIC int getTriggerMethodFromStr(TriggerMethod_t *r, char *s);
LIBSR_PUBLIC const char *getStrFromTriggerMethod(TriggerMethod_t t);

#endif
