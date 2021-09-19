#include "activityList.h"
#include <malloc.h>

int appendActivity(ActivityList *l, Activity *a) {
	if (l == NULL || a == NULL) return 1;
	if (l->first_act == NULL) l->first_act = a;
	if (l->last_act != NULL) {
		l->last_act->next = a;
		a->prev = l->last_act;
	}
	l->last_act = a;
	return 0;
}

int appendMSS(ActivityList *l, MSS *m) {
	if (!l || !m) return 1;
	if (!l->first_mss) l->first_mss = m;
	if (l->last_mss) {
		l->last_mss->next = m;
		m->prev = l->last_mss;
	}
	l->last_mss = m;
	return 0;
}

ActivityList *new_activityList() {
	ActivityList *r = (ActivityList*)malloc(sizeof(ActivityList));
	r->first_act = NULL;
	r->last_act = NULL;
	r->first_mss = NULL;
	r->last_mss = NULL;
	r->appendActivity = &appendActivity;
	r->appendMSS = &appendMSS;
	return r;
}

void free_activityList(ActivityList *l) {
	if (l == NULL) return;
	if (l->first_act != NULL) {
		free_all_activities_from_first(l->first_act);
		l->first_act = NULL;
		l->last_act = NULL;
	}
	if (l->first_mss) {
		free_all_mss_from_first(l->first_mss);
		l->first_mss = NULL;
		l->last_mss = NULL;
	}
	l->appendActivity = NULL;
	l->appendMSS = NULL;
	free(l);
}
