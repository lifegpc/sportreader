#ifndef _LIBSR_MSS_H
#define _LIBSR_MSS_H

#include "activity.h"

typedef struct NextSport {
	ActivityLap *Transition;
	Activity *activity;
	struct NextSport *prev;
	struct NextSport *next;
	int(*setTransition) (struct NextSport *ns, ActivityLap* tr);
} NextSport;

typedef struct MultiSportSession {
	s_time Id;
	Activity *FirstSport;
	NextSport *first_NextSport;
	NextSport *last_NextSport;
	char *Notes;
	int(*setNotes) (struct MultiSportSession *mss, char* s);
	int(*appendNextSport) (struct MultiSportSession *mss, NextSport *ns);
	struct MultiSportSession *prev;
	struct MultiSportSession *next;
} MultiSportSession, MSS;

LIBSR_PUBLIC NextSport *new_nextSport(Activity *act);
LIBSR_PUBLIC void free_nextSport(NextSport *n);
LIBSR_PUBLIC void free_all_nextSports_from_first(NextSport *first);
LIBSR_PUBLIC MSS *new_mss(s_time id, Activity *FirstSport);
LIBSR_PUBLIC void free_mss(MSS *n);
LIBSR_PUBLIC void free_all_mss_from_first(MSS *first);

#endif
