#include "activity.h"
#include <malloc.h>
#include <string.h>

int setNotes2(Activity *a, char *s) {
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

int appendActivityLap(Activity *a, ActivityLap* l) {
	if (a == NULL || l == NULL) return 1;
	if (a->first_lap == NULL) a->first_lap = l;
	if (a->last_lap != NULL) {
		a->last_lap->next = l;
		l->prev = a->last_lap;
	}
	a->last_lap = l;
	return 0;
}

Activity *new_activity(char *type, s_time Id) {
	if (type == NULL) return NULL;
	Activity *act = (Activity*)malloc(sizeof(Activity));
	act->prev = NULL;
	act->next = NULL;
	act->first_lap = NULL;
	act->last_lap = NULL;
	act->Notes = NULL;
	copySTime(&act->Id, Id);
	size_t le = strlen(type);
	char *s = (char*)malloc(le + 1);
	memcpy(s, type, le);
	s[le] = 0;
	act->type = s;
	act->setNotes = &setNotes2;
	act->appendActivityLap = &appendActivityLap;
	return act;
}

void free_activity(Activity *act) {
	if (act == NULL) return;
	if (act->prev != NULL) {
		act->prev->next = NULL;
		act->prev = NULL;
	}
	if (act->next != NULL) {
		act->next->prev = NULL;
		act->next = NULL;
	}
	if (act->first_lap != NULL) {
		free_all_activityLaps_from_first(act->first_lap);
		act->first_lap = NULL;
		act->last_lap = NULL;
	}
	if (act->type != NULL) {
		free(act->type);
		act->type = NULL;
	}
	if (act->Notes != NULL) {
		free(act->Notes);
		act->Notes = NULL;
	}
	act->setNotes = NULL;
	act->appendActivityLap = NULL;
	free(act);
}

void free_all_activities_from_first(Activity *first) {
	if (first == NULL) return;
	Activity *next = first->next;
	free_activity(first);
	Activity *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_activity(now);
	}
}
