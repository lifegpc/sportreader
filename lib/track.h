#ifndef _LIBSR_TRACK_H
#define _LIBSR_TRACK_H

#include "trackpoint.h"

typedef struct Track {
	Trackpoint *first;
	Trackpoint *last;
	struct Track *prev;
	struct Track *next;
	int(*append) (struct Track *t, Trackpoint *tp);
} Track;

/*Create a new track. Need free memory by calling free_track.*/
LIBSR_PUBLIC Track *new_track();
LIBSR_PUBLIC void free_track(Track *t);
LIBSR_PUBLIC void free_all_tracks_from_first(Track *first);

#endif
