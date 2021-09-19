#include "strlist.h"
#include <string.h>
#include <malloc.h>

int StrNode_setStr(StrNode *n, char *s) {
	if (n == NULL || s == NULL) return 1;
	if (n->cur) free(n->cur);
	size_t le = strlen(s);
	n->cur = (char*)malloc(le + 1);
	if (!n->cur) return 1;
	memcpy(n->cur, s, le);
	n->cur[le] = 0;
	return 0;
}

StrNode *new_strNode(char *s) {
	if (s == NULL) return NULL;
	StrNode *n = (StrNode*)malloc(sizeof(StrNode));
	if (!n) return NULL;
	n->prev = NULL;
	n->next = NULL;
	size_t le = strlen(s);
	n->cur = (char*)malloc(le + 1);
	memcpy(n->cur, s, le);
	n->cur[le] = 0;
	n->setStr = &StrNode_setStr;
	return n;
}

void free_strNode(StrNode *n) {
	if (!n) return;
	if (n->prev) {
		n->prev->next = NULL;
		n->prev = NULL;
	}
	if (n->next) {
		n->next->prev = NULL;
		n->next = NULL;
	}
	if (n->cur) {
		free(n->cur);
		n->cur = NULL;
	}
	n->setStr = NULL;
	free(n);
}

void free_all_strNodes_from_first(StrNode *first) {
	if (first == NULL) return;
	StrNode *next = first->next;
	free_strNode(first);
	StrNode *now = NULL;
	while (next != NULL) {
		now = next;
		next = next->next;
		free_strNode(now);
	}
}

int StrList_append(StrList *l, char *s) {
	if (!l || !s) return 1;
	StrNode *n = new_strNode(s);
	if (!n) return 1;
	if (!l->first) l->first = n;
	if (l->last) {
		l->last->next = n;
		n->prev = l->last;
	}
	l->last = n;
	return 0;
}

StrList *new_strList() {
	StrList *l = (StrList*)malloc(sizeof(StrList));
	if (!l) return NULL;
	l->first = NULL;
	l->last = NULL;
	l->append = &StrList_append;
	return l;
}

void free_strList(StrList *l) {
	if (!l) return;
	if (l->first) {
		free_all_strNodes_from_first(l->first);
		l->first = NULL;
		l->last = NULL;
	}
	l->append = NULL;
	free(l);
}
