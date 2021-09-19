#ifndef _TCD_H
#define _TCD_H

#include "activityList.h"

typedef struct TrainingCenterDatabase {
	ActivityList *Activities;
	int(*appendActivity) (struct TrainingCenterDatabase *tcd, Activity *a);
	int(*appendMSS) (struct TrainingCenterDatabase* tcd, MSS *mss);
} TrainingCenterDatabase, TCD;

LIBSR_PUBLIC TCD *new_tcd();
LIBSR_PUBLIC void free_tcd(TCD *tcd);

#endif
