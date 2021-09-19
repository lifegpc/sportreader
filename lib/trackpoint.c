#include "trackpoint.h"
#include<malloc.h>
#include<string.h>

int setPosition(Trackpoint *tr, double latitude, double longitude) {
	if (tr == NULL) return 1;
	if (tr->pos == NULL) {
		tr->pos = new_position(latitude, longitude);
		return !tr->pos;
	} else {
		int i = tr->pos->setLatitude(tr->pos, latitude);
		int j = tr->pos->setLongitude(tr->pos, longitude);
		return i || j;
	}
}

int setAltitudeMeters(Trackpoint *tr, double AltitudeMeters) {
	if (tr == NULL) return 1;
	if (tr->AltitudeMeters == NULL) {
		tr->AltitudeMeters = (double*)malloc(sizeof(double));
		if (tr->AltitudeMeters == NULL) return 1;
	}
	*tr->AltitudeMeters = AltitudeMeters;
	return 0;
}

int setDistanceMeters(Trackpoint *tr, double DistanceMeters) {
	if (tr == NULL) return 1;
	if (tr->DistanceMeters == NULL) {
		tr->DistanceMeters = (double*)malloc(sizeof(double));
		if (tr->DistanceMeters == NULL) return 1;
	}
	if (DistanceMeters < 0) return 1;
	*tr->DistanceMeters = DistanceMeters;
	return 0;
}

int setHeartRateBpm(Trackpoint *tr, unsigned char HeartRateBpm) {
	if (tr == NULL) return 1;
	if (tr->HeartRateBpm == NULL) {
		tr->HeartRateBpm = (unsigned char*)malloc(sizeof(unsigned char));
		if (tr->HeartRateBpm == NULL) return 1;
	}
	if (HeartRateBpm <= 0) return 1;
	*tr->HeartRateBpm = HeartRateBpm;
	return 0;
}

int setCadence(Trackpoint *tr, unsigned char Cadence) {
	if (tr == NULL) return 1;
	if (tr->Cadence == NULL) {
		tr->Cadence = (unsigned char*)malloc(sizeof(unsigned char));
		if (tr->Cadence == NULL) return 1;
	}
	if (Cadence < 0) return 1;
	*tr->Cadence = Cadence;
	return 0;
}

int setSensorState(Trackpoint *tr, SensorState_t state) {
	if (tr == NULL) return 1;
	if (tr->SensorState == NULL) {
		tr->SensorState = (SensorState_t*)malloc(sizeof(SensorState_t));
		if (tr->SensorState == NULL) return 1;
	}
	*tr->SensorState = state;
	return 0;
}

int setSensorStateByStr(Trackpoint *tr, char* s) {
	if (tr == NULL) return 1;
	if (s == NULL) return 1;
	SensorState_t st = 0;
	if (getSensorStateFromStr(&st, s)) return 1;
	return setSensorState(tr, st);
}

const char *getSensorStateStr(Trackpoint *tr) {
	if (tr == NULL || tr->SensorState == NULL) return NULL;
	return getStrFromSensorState(*tr->SensorState);
}

Trackpoint *new_trackpoint(s_time t) {
	Trackpoint *tr = (Trackpoint*)malloc(sizeof(Trackpoint));
	copySTime(&tr->Time, t);
	tr->AltitudeMeters = NULL;
	tr->Cadence = NULL;
	tr->DistanceMeters = NULL;
	tr->HeartRateBpm = NULL;
	tr->next = NULL;
	tr->pos = NULL;
	tr->prev = NULL;
	tr->SensorState = NULL;
	tr->setPosition = &setPosition;
	tr->setAltitudeMeters = &setAltitudeMeters;
	tr->setDistanceMeters = &setDistanceMeters;
	tr->setHeartRateBpm = &setHeartRateBpm;
	tr->setCadence = &setCadence;
	tr->setSensorState = &setSensorState;
	tr->setSensorStateByStr = &setSensorStateByStr;
	tr->getSensorStateStr = &getSensorStateStr;
	return tr;
}

void free_trackpoint(Trackpoint *t) {
	if (t == NULL) return;
	if (t->prev != NULL) {
		t->prev->next = NULL;
		t->prev = NULL;
	}
	if (t->next != NULL) {
		t->next->prev = NULL;
		t->next = NULL;
	}
	if (t->pos != NULL) {
		free_position(t->pos);
		t->pos = NULL;
	}
	if (t->AltitudeMeters != NULL) {
		free(t->AltitudeMeters);
		t->AltitudeMeters = NULL;
	}
	if (t->DistanceMeters != NULL) {
		free(t->DistanceMeters);
		t->DistanceMeters = NULL;
	}
	if (t->HeartRateBpm != NULL) {
		free(t->HeartRateBpm);
		t->HeartRateBpm = NULL;
	}
	if (t->Cadence != NULL) {
		free(t->Cadence);
		t->Cadence = NULL;
	}
	if (t->SensorState != NULL) {
		free(t->SensorState);
		t->SensorState = NULL;
	}
	t->setPosition = NULL;
	t->setAltitudeMeters = NULL;
	t->setDistanceMeters = NULL;
	t->setHeartRateBpm = NULL;
	t->setCadence = NULL;
	t->setSensorState = NULL;
	t->setSensorStateByStr = NULL;
	t->getSensorStateStr = NULL;
	free(t);
}

void free_all_trackpoints_from_first(Trackpoint *first) {
	if (first == NULL) return;
	Trackpoint *next = first->next;
	free_trackpoint(first);
	Trackpoint *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_trackpoint(now);
	}
}

int getSensorStateFromStr(SensorState_t *r, char *s) {
	if (s == NULL || r == NULL) return 1;
	if (!strcmp(s, "Present")) *r = SensorState_Present;
	else if (!strcmp(s, "Absent")) *r = SensorState_Absent;
	else return 1;
	return 0;
}

const char *getStrFromSensorState(SensorState_t s) {
	switch (s) {
	case SensorState_Present:
		return "Present";
	case SensorState_Absent:
		return "Absent";
	default:
		return "Present";
	}
}
