#include "mss.h"
#include <malloc.h>
#include <string.h>

int setTransition(NextSport *ns, ActivityLap* tr) {
	if (ns == NULL || tr == NULL) return 1;
	if (ns->Transition) free_activityLap(ns->Transition);
	ns->Transition = tr;
	return 0;
}

NextSport *new_nextSport(Activity *act) {
	if (act == NULL) return NULL;
	NextSport *ns = (NextSport*)malloc(sizeof(NextSport));
	ns->activity = act;
	ns->Transition = NULL;
	ns->prev = NULL;
	ns->next = NULL;
	ns->setTransition = &setTransition;
	return ns;
}

void free_nextSport(NextSport *n) {
	if (n == NULL) return;
	if (n->prev) {
		n->prev->next = NULL;
		n->prev = NULL;
	}
	if (n->next) {
		n->next->prev = NULL;
		n->next = NULL;
	}
	if (n->activity) {
		free_activity(n->activity);
	}
	if (n->Transition) {
		free_activityLap(n->Transition);
	}
	n->setTransition = NULL;
	free(n);
}

void free_all_nextSports_from_first(NextSport *first) {
	if (first == NULL) return;
	NextSport *next = first->next;
	free_nextSport(first);
	NextSport *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_nextSport(now);
	}
}

int setNotes3(MSS *mss, char* s) {
	if (mss == NULL || s == NULL) return 1;
	if (mss->Notes) free(mss->Notes);
	size_t le = strlen(s);
	mss->Notes = (char*)malloc(le + 1);
	memcpy(mss->Notes, s, le);
	mss->Notes[le] = 0;
	return 0;
}

int appendNextSport(MSS *mss, NextSport *ns) {
	if (mss == NULL || ns == NULL) return 1;
	if (!mss->first_NextSport) mss->first_NextSport = ns;
	if (mss->last_NextSport) {
		mss->last_NextSport->next = ns;
		ns->prev = mss->last_NextSport;
	}
	mss->last_NextSport = ns;
	return 0;
}

MSS *new_mss(s_time id, Activity *FirstSport) {
	if (FirstSport == NULL) return NULL;
	MSS *mss = (MSS*)malloc(sizeof(MSS));
	mss->Id = id;
	mss->FirstSport = FirstSport;
	mss->first_NextSport = NULL;
	mss->last_NextSport = NULL;
	mss->Notes = NULL;
	mss->prev = NULL;
	mss->next = NULL;
	mss->setNotes = &setNotes3;
	mss->appendNextSport = &appendNextSport;
	return mss;
}

void free_mss(MSS *n) {
	if (n == NULL) return;
	if (n->prev) {
		n->prev->next = NULL;
		n->prev = NULL;
	}
	if (n->next) {
		n->next->prev = NULL;
		n->next = NULL;
	}
	if (n->FirstSport) {
		free_activity(n->FirstSport);
		n->FirstSport = NULL;
	}
	if (n->first_NextSport) {
		free_all_nextSports_from_first(n->first_NextSport);
		n->first_NextSport = NULL;
		n->last_NextSport = NULL;
	}
	if (n->Notes) {
		free(n->Notes);
		n->Notes = NULL;
	}
	n->setNotes = NULL;
	n->appendNextSport = NULL;
	free(n);
}

void free_all_mss_from_first(MSS *first) {
	if (first == NULL) return;
	MSS *next = first->next;
	free_mss(first);
	MSS *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_mss(now);
	}
}
