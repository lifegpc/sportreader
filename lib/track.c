#include "track.h"
#include <malloc.h>

int append(Track *t, Trackpoint *tp) {
	if (t == NULL || tp == NULL) return 1;
	if (t->first == NULL) t->first = tp;
	if (t->last != NULL) {
		t->last->next = tp;
		tp->prev = t->last;
	}
	t->last = tp;
	return 0;
}

Track *new_track() {
	Track *tr = (Track*)malloc(sizeof(Track));
	tr->first = NULL;
	tr->last = NULL;
	tr->prev = NULL;
	tr->next = NULL;
	tr->append = &append;
	return tr;
}

void free_track(Track *t) {
	if (t == NULL) return;
	if (t->first != NULL) {
		free_all_trackpoints_from_first(t->first);
		t->first = NULL;
		t->last = NULL;
	}
	if (t->prev != NULL) {
		t->prev->next = NULL;
		t->prev = NULL;
	}
	if (t->next != NULL) {
		t->next->prev = NULL;
		t->next = NULL;
	}
	t->append = NULL;
	free(t);
}

void free_all_tracks_from_first(Track *first) {
	if (first == NULL) return;
	Track *next = first->next;
	free_track(first);
	Track *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_track(now);
	}
}
