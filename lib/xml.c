#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "xml.h"
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "stdio_config.h"
#include <string.h>
#include "numparse.h"

xmlChar *getTextInNode(xmlNode *node) {
	if (node == NULL) return NULL;
	xmlNode *child = NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_TEXT_NODE) {
			return child->content;
		}
	}
	return NULL;
}

Position *parsePosition(xmlNode *node) {
	if (node == NULL) return NULL;
	double Latitude = 0, Longitude = 0;
	xmlNode *child = NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "LatitudeDegrees")) {
				xmlChar *s = getTextInNode(child);
				if (s == NULL) return NULL;
				if (parseDFromStr(&Latitude, s)) return NULL;
			} else if (!strcmp(child->name, "LongitudeDegrees")) {
				xmlChar *s = getTextInNode(child);
				if (s == NULL) return NULL;
				if (parseDFromStr(&Longitude, s)) return NULL;
			}
		}
	}
	return new_position(Latitude, Longitude);
}

Trackpoint *parseTrackpoint(xmlNode *node) {
	if (node == NULL) return NULL;
	s_time time;
	xmlNode *child = NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Time")) {
				xmlChar *s = getTextInNode(child);
				if (s == NULL) return NULL;
				if (parseISO8601(s, &time)) return NULL;
				break;
			}
		}
	}
	Trackpoint *tp = new_trackpoint(time);
	if (tp == NULL) return NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Position")) {
				Position *pos = parsePosition(child);
				if (pos) {
					if (tp->pos) free_position(tp->pos);
					tp->pos = pos;
				}
			} else if (!strcmp(child->name, "AltitudeMeters")) {
				xmlChar *s = getTextInNode(child);
				if (s) {
					double n = 0;
					if (!parseDFromStr(&n, s)) tp->setAltitudeMeters(tp, n);
				}
			} else if (!strcmp(child->name, "DistanceMeters")) {
				xmlChar *s = getTextInNode(child);
				if (s) {
					double n = 0;
					if (!parseDFromStr(&n, s)) tp->setDistanceMeters(tp, n);
				}
			} else if (!strcmp(child->name, "HeartRateBpm")) {
				xmlChar *s = getTextInNode(child->children);
				if (s) {
					unsigned char n = 0;
					if (!parseUCFromStr(&n, s)) tp->setHeartRateBpm(tp, n);
				}
			} else if (!strcmp(child->name, "Cadence")) {
				xmlChar *s = getTextInNode(child);
				if (s) {
					unsigned char n = 0;
					if (!parseUCFromStr(&n, s)) tp->setCadence(tp, n);
				}
			} else if (!strcmp(child->name, "SensorState")) {
				xmlChar *s = getTextInNode(child);
				if (s) {
					tp->setSensorStateByStr(tp, s);
				}
			}
		}
	}
	return tp;
}

Track *parseTrack(xmlNode *node) {
	if (node == NULL) return NULL;
	Track *t = new_track();
	xmlNode *child = NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Trackpoint")) {
				Trackpoint *tp = parseTrackpoint(child);
				if (tp) t->append(t, tp);
			}
		}
	}
	if (t->first == NULL) {
		free_track(t);
		return NULL;
	}
	return t;
}

ActivityLap *parseLap(xmlNode *node) {
	if (node == NULL) return NULL;
	xmlNode *child = NULL;
	s_time StartTime;
	xmlChar *st = xmlGetProp(node, "StartTime");
	if (st == NULL) return NULL;
	if (parseISO8601(st, &StartTime)) {
		xmlFree(st);
		return NULL;
	}
	xmlFree(st);
	double TotalTimeSeconds = 0, DistanceMeters = 0;
	unsigned short Calories = 0;
	xmlChar *Intensity = NULL, *TriggerMethod = NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "TotalTimeSeconds")) {
				xmlChar *s = getTextInNode(child);
				if (s != NULL) parseDFromStr(&TotalTimeSeconds, s);
			} else if (!strcmp(child->name, "DistanceMeters")) {
				xmlChar *s = getTextInNode(child);
				if (s != NULL) parseDFromStr(&DistanceMeters, s);
			} else if (!strcmp(child->name, "Calories")) {
				xmlChar *s = getTextInNode(child);
				if (s != NULL) parseUSFromStr(&Calories, s);
			} else if (!strcmp(child->name, "Intensity")) {
				Intensity = getTextInNode(child);
			} else if (!strcmp(child->name, "TriggerMethod")) {
				TriggerMethod = getTextInNode(child);
			}
		}
	}
	ActivityLap *lap = new_activityLap(StartTime, TotalTimeSeconds, DistanceMeters, Calories, Intensity, TriggerMethod);
	if (lap == NULL) return NULL;
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "MaximumSpeed")) {
				xmlChar *s = getTextInNode(child);
				if (s != NULL) {
					double n = 0;
					if (!parseDFromStr(&n, s)) lap->setMaximumSpeed(lap, n);
				}
			} else if (!strcmp(child->name, "AverageHeartRateBpm")) {
				xmlChar *s = getTextInNode(child->children);
				if (s != NULL) {
					unsigned char n = 0;
					if (!parseUCFromStr(&n, s)) lap->setAverageHeartRateBpm(lap, n);
				}
			} else if (!strcmp(child->name, "MaximumHeartRateBpm")) {
				xmlChar *s = getTextInNode(child->children);
				if (s != NULL) {
					unsigned char n = 0;
					if (!parseUCFromStr(&n, s)) lap->setMaximumHeartRateBpm(lap, n);
				}
			} else if (!strcmp(child->name, "Cadence")) {
				xmlChar *s = getTextInNode(child);
				if (s != NULL) {
					unsigned char n = 0;
					if (!parseUCFromStr(&n, s)) lap->setCadence(lap, n);
				}
			} else if (!strcmp(child->name, "Track")) {
				Track *t = parseTrack(child);
				if (t) lap->appendTrack(lap, t);
			} else if (!strcmp(child->name, "Notes")) {
				xmlChar *s = getTextInNode(child);
				if (s) lap->setNotes(lap, s);
			}
		}
	}
	return lap;
}

Activity *parseActivity(xmlNode *node) {
	if (node == NULL) return NULL;
	xmlNode *child = NULL;
	s_time t;
	int find_id = 0;
	for (child = node->children; child; child = node->children) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Id")) {
				xmlChar *id = getTextInNode(child);
				if (id != NULL) {
					find_id = 1;
					if (parseISO8601(id, &t)) return NULL;
					break;
				}
				break;
			}
		}
	}
	if (!find_id) return NULL;
	char *type = xmlGetProp(node, "Sport");
	if (type == NULL) return NULL;
	Activity *act = new_activity(type, t);
	xmlFree(type);
	if (act == NULL) {
		return NULL;
	}
	for (child = node->children; child; child = child->next) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Lap")) {
				ActivityLap* lap = parseLap(child);
				if (lap) act->appendActivityLap(act, lap);
			} else if (!strcmp(child->name, "Notes")) {
				xmlChar *s = getTextInNode(child);
				if (s) act->setNotes(act, s);
			}
		}
	}
	return act;
}

NextSport *parseNextSport(xmlNode *node) {
	if (node == NULL) return NULL;
	xmlNode *child = NULL;
	Activity *act = NULL;
	for (child = node->children; child; child = node->children) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Activity")) {
				act = parseActivity(child);
				break;
			}
		}
	}
	if (!act) return NULL;
	NextSport *ns = new_nextSport(act);
	if (ns == NULL) {
		free_activity(act);
		return NULL;
	}
	for (child = node->children; child; child = node->children) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Transition")) {
				ActivityLap* lap = parseLap(child);
				if (lap) ns->setTransition(ns, lap);
				break;
			}
		}
	}
	return ns;
}

MSS *parseMSS(xmlNode *node) {
	if (node == NULL) return NULL;
	xmlNode *child = NULL;
	s_time Id;
	Activity *firstSport = NULL;
	int find_id = 0;
	for (child = node->children; child; child = node->children) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "Id")) {
				xmlChar *id = getTextInNode(child);
				if (id != NULL) {
					find_id = 1;
					if (parseISO8601(id, &Id)) {
						if (firstSport) free_activity(firstSport);
						return NULL;
					}
				}
			} else if (!strcmp(child->name, "FirstSport")) {
				firstSport = parseActivity(child->children);
			}
		}
	}
	if (!find_id || !firstSport) {
		if (firstSport) free_activity(firstSport);
		return NULL;
	}
	MSS *mss = new_mss(Id, firstSport);
	if (!mss) {
		free_activity(firstSport);
		return NULL;
	}
	for (child = node->children; child; child = node->children) {
		if (child->type == XML_ELEMENT_NODE) {
			if (!strcmp(child->name, "NextSport")) {
				NextSport *ns = parseNextSport(child);
				if (ns) mss->appendNextSport(mss, ns);
			} else if (!strcmp(child->name, "Notes")) {
				xmlChar *s = getTextInNode(child);
				if (s) mss->setNotes(mss, s);
			}
		}
	}
	return mss;
}

XMLParseResult readXML(char* filename) {
	xmlDocPtr doc;
	XMLParseResult r;
	int opt = XML_PARSE_NOBLANKS;
	
	r.status = PARSE_OK;
	r.tcd = NULL;
	short activities_count = 0;
	doc = xmlReadFile(filename, NULL, opt);
	if (doc == NULL) return r.status = FAILED_PARSE, r;
	xmlNode *root = doc->children;
	if (root == NULL) {
		xmlFreeDoc(doc);
		return r.status = NO_ROOT_ELEMENT, r;
	}
	if (root->name == NULL || strcmp(root->name, "TrainingCenterDatabase")) {
		xmlFreeDoc(doc);
		return r.status = WRONG_ROOT_ELEMENT, r;
	}
	r.tcd = new_tcd();
	xmlNode *cur_child = NULL;
	for (cur_child = root->children; cur_child; cur_child = cur_child->next) {
		if (!strcmp(cur_child->name, "Activities")) {
			activities_count++;
			if (activities_count > 1) return r.status = TOO_MANY_ACTIVITIES_ELEMENT, r;
			xmlNode *activity_node = NULL;
			for (activity_node = cur_child->children; activity_node; activity_node = activity_node->next) {
				if (!strcmp(activity_node->name, "Activity")) {
					Activity *act = parseActivity(activity_node);
					if (act) r.tcd->appendActivity(r.tcd, act);
				} else if (!strcmp(activity_node->name, "MultiSportSession")) {
					MSS *mss = parseMSS(activity_node);
					if (mss) r.tcd->appendMSS(r.tcd, mss);
				}
			}
		}
	}
	xmlFreeDoc(doc);
	return r;
}
