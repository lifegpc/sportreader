#ifndef _LIBSR_ACTIVITY_H
#define _LIBSR_ACTIVITY_H

#include "activityLap.h"

typedef struct Activity {
	char *type;
	s_time Id;
	ActivityLap *first_lap;
	ActivityLap *last_lap;
	char *Notes;
	struct Activity *prev;
	struct Activity *next;
	int(*setNotes) (struct Activity *a, char *s);
	int(*appendActivityLap) (struct Activity *a, ActivityLap* l);
} Activity;

/*Create a new activity. Need free memory by calling free_activity.*/
LIBSR_PUBLIC Activity *new_activity(char *type, s_time Id);
LIBSR_PUBLIC void free_activity(Activity *act);
LIBSR_PUBLIC void free_all_activities_from_first(Activity *first);

#endif
