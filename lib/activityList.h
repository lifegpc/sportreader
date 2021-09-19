#ifndef _LIBSR_ACTIVITY_LIST_H
#define _LIBSR_ACTIVITY_LIST_H

#include "mss.h"

typedef struct ActivityList {
	Activity *first_act;
	Activity *last_act;
	MSS *first_mss;
	MSS *last_mss;
	int(*appendActivity) (struct ActivityList *l, Activity *a);
	int(*appendMSS) (struct ActivityList *l, MSS *m);
} ActivityList;

LIBSR_PUBLIC ActivityList *new_activityList();
LIBSR_PUBLIC void free_activityList(ActivityList *l);

#endif
