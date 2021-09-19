#include "tcd.h"
#include <malloc.h>

int appendActivity2(TCD *tcd, Activity *a) {
	if (tcd == NULL || a == NULL) return 1;
	if (tcd->Activities == NULL) {
		tcd->Activities = new_activityList();
		if (!tcd->Activities) return 1;
	}
	return tcd->Activities->appendActivity(tcd->Activities, a);
}

int appendMSS2(TCD* tcd, MSS *mss) {
	if (tcd == NULL || mss == NULL) return 1;
	if (tcd->Activities == NULL) {
		tcd->Activities = new_activityList();
		if (!tcd->Activities) return 1;
	}
	return tcd->Activities->appendMSS(tcd->Activities, mss);
}

TCD *new_tcd() {
	TCD *tcd = (TCD*)malloc(sizeof(TCD));
	tcd->Activities = NULL;
	tcd->appendActivity = &appendActivity2;
	tcd->appendMSS = &appendMSS2;
	return tcd;
}

void free_tcd(TCD *tcd) {
	if (tcd == NULL) return;
	if (tcd->Activities != NULL) {
		free_activityList(tcd->Activities);
		tcd->Activities = NULL;
	}
	tcd->appendActivity = NULL;
	tcd->appendMSS = NULL;
	free(tcd);
}
