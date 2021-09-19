#include "activityLap.h"
#include <string.h>
#include <malloc.h>

int setMaximumSpeed(ActivityLap *a, double MaximumSpeed) {
	if (a == NULL || MaximumSpeed <= 0) return 1;
	if (a->MaximumSpeed == NULL) {
		a->MaximumSpeed = (double*)malloc(sizeof(double));
		if (!a->MaximumSpeed) return 1;
	}
	*a->MaximumSpeed = MaximumSpeed;
	return 0;
}

int setAverageHeartRateBpm(ActivityLap *a, unsigned char AverageHeartRateBpm) {
	if (a == NULL || AverageHeartRateBpm <= 0) return 1;
	if (a->AverageHeartRateBpm == NULL) {
		a->AverageHeartRateBpm = (unsigned char*)malloc(sizeof(unsigned char));
		if (!a->AverageHeartRateBpm) return 1;
	}
	*a->AverageHeartRateBpm = AverageHeartRateBpm;
	return 0;
}

int setMaximumHeartRateBpm(ActivityLap *a, unsigned char MaximumHeartRateBpm) {
	if (a == NULL || MaximumHeartRateBpm <= 0) return 1;
	if (a->MaximumHeartRateBpm == NULL) {
		a->MaximumHeartRateBpm = (unsigned char*)malloc(sizeof(unsigned char));
		if (!a->MaximumHeartRateBpm) return 1;
	}
	*a->MaximumHeartRateBpm = MaximumHeartRateBpm;
	return 0;
}

int setCadence2(ActivityLap *a, unsigned char Cadence) {
	if (a == NULL || Cadence <= 0) return 1;
	if (a->Cadence == NULL) {
		a->Cadence = (unsigned char*)malloc(sizeof(unsigned char));
		if (!a->Cadence) return 1;
	}
	*a->Cadence = Cadence;
	return 0;
}

int setNotes(ActivityLap *a, char *s) {
	if (a == NULL || s == NULL) return 1;
	size_t len = strlen(s);
	if (!len) return 1;
	if (a->Notes != NULL) {
		free(a->Notes);
	}
	a->Notes = (char*)malloc(len + 1);
	if (!a->Notes) return 1;
	memcpy(a->Notes, s, len);
	a->Notes[len] = 0;
	return 0;
}

int setIntensityByStr(ActivityLap *a, char *s) {
	if (a == NULL || s == NULL) return 1;
	Intensity_t t;
	if (getIntensityFromStr(&t, s)) return 1;
	a->Intensity = t;
	return 0;
}

int setTriggerMethodByStr(ActivityLap *a, char *s) {
	if (a == NULL || s == NULL) return 1;
	TriggerMethod_t t;
	if (getTriggerMethodFromStr(&t, s)) return 1;
	a->TriggerMethod = t;
	return 0;
}

int appendTrack(ActivityLap *a, Track *t) {
	if (a == NULL || t == NULL) return 1;
	if (a->first_track == NULL) a->first_track = t;
	if (a->last_track != NULL) {
		a->last_track->next = t;
		t->prev = a->last_track;
	}
	a->last_track = t;
	return 0;
}

ActivityLap *new_activityLap(s_time StartTime, double TotalTimeSeconds, double DistanceMeters, unsigned short Calories, char *Intensity, char *TriggerMethod) {
	if (Intensity == NULL || TriggerMethod == NULL) return NULL;
	Intensity_t i;
	if (getIntensityFromStr(&i, Intensity)) return NULL;
	TriggerMethod_t j;
	if (getTriggerMethodFromStr(&j, TriggerMethod)) return NULL;
	if (TotalTimeSeconds < 0) return NULL;
	if (DistanceMeters < 0) return NULL;
	ActivityLap *r = (ActivityLap*)malloc(sizeof(ActivityLap));
	r->MaximumSpeed = NULL;
	r->AverageHeartRateBpm = NULL;
	r->MaximumHeartRateBpm = NULL;
	r->Cadence = NULL;
	r->first_track = NULL;
	r->last_track = NULL;
	r->Notes = NULL;
	r->prev = NULL;
	r->next = NULL;
	copySTime(&r->StartTime, StartTime);
	r->TotalTimeSeconds = TotalTimeSeconds;
	r->DistanceMeters = DistanceMeters;
	r->Calories = Calories;
	r->Intensity = i;
	r->TriggerMethod = j;
	r->setMaximumSpeed = &setMaximumSpeed;
	r->setAverageHeartRateBpm = &setAverageHeartRateBpm;
	r->setMaximumHeartRateBpm = &setMaximumHeartRateBpm;
	r->setCadence = &setCadence2;
	r->setNotes = &setNotes;
	r->setIntensityByStr = &setIntensityByStr;
	r->setTriggerMethodByStr = &setTriggerMethodByStr;
	r->appendTrack = &appendTrack;
	return r;
}

void free_activityLap(ActivityLap* t) {
	if (t == NULL) return;
	if (t->prev != NULL) {
		t->prev->next = NULL;
		t->prev = NULL;
	}
	if (t->next != NULL) {
		t->next->prev = NULL;
		t->next = NULL;
	}
	if (t->MaximumSpeed != NULL) {
		free(t->MaximumSpeed);
		t->MaximumSpeed = NULL;
	}
	if (t->AverageHeartRateBpm != NULL) {
		free(t->AverageHeartRateBpm);
		t->AverageHeartRateBpm = NULL;
	}
	if (t->MaximumHeartRateBpm != NULL) {
		free(t->MaximumHeartRateBpm);
		t->MaximumHeartRateBpm = NULL;
	}
	if (t->Cadence != NULL) {
		free(t->Cadence);
		t->Cadence = NULL;
	}
	if (t->first_track != NULL) {
		free_all_tracks_from_first(t->first_track);
		t->first_track = NULL;
		t->last_track = NULL;
	}
	if (t->Notes != NULL) {
		free(t->Notes);
		t->Notes = NULL;
	}
	t->setMaximumSpeed = NULL;
	t->setAverageHeartRateBpm = NULL;
	t->setMaximumHeartRateBpm = NULL;
	t->setCadence = NULL;
	t->setNotes = NULL;
	t->setIntensityByStr = NULL;
	t->setTriggerMethodByStr = NULL;
	t->appendTrack = NULL;
	free(t);
}

void free_all_activityLaps_from_first(ActivityLap *first) {
	if (first == NULL) return;
	ActivityLap *next = first->next;
	free_activityLap(first);
	ActivityLap *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_activityLap(now);
	}
}

int getIntensityFromStr(Intensity_t *r, char *s) {
	if (s == NULL || r == NULL) return 1;
	if (!strcmp(s, "Active")) *r = Intensity_Active;
	else if (!strcmp(s, "Resting")) *r = Intensity_Resting;
	else return 1;
	return 0;
}

const char *getStrFromIntensity(Intensity_t t) {
	switch (t) {
	case Intensity_Active:
		return "Active";
	case Intensity_Resting:
		return "Resting";
	default:
		return "Active";
	}
}

int getTriggerMethodFromStr(TriggerMethod_t *r, char *s) {
	if (s == NULL || r == NULL) return 1;
	if (!strcmp(s, "Manual")) *r = Trigger_Manual;
	else if (!strcmp(s, "Distance")) *r = Trigger_Distance;
	else if (!strcmp(s, "Location")) *r = Trigger_Location;
	else if (!strcmp(s, "Time")) *r = Trigger_Time;
	else if (!strcmp(s, "HeartRate")) *r = Trigger_HeartRate;
	else return 1;
	return 0;
}

const char *getStrFromTriggerMethod(TriggerMethod_t t) {
	switch (t) {
	case Trigger_Manual:
		return "Manual";
	case Trigger_Distance:
		return "Distance";
	case Trigger_Location:
		return "Location";
	case Trigger_Time:
		return "Time";
	case Trigger_HeartRate:
		return "HeartRate";
	default:
		return "Manual";
	}
}
