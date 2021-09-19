#ifndef _STR_LIST_H
#define _STR_LIST_H

typedef struct StrNode {
	char *cur;
	struct StrNode *prev;
	struct StrNode *next;
	int(*setStr) (struct StrNode *n, char *s);
} StrNode;

typedef struct StrList {
	StrNode *first;
	StrNode *last;
	int(*append) (struct StrList *l, char *s);
} StrList;

StrNode *new_strNode(char *s);
void free_strNode(StrNode *n);
void free_all_strNodes_from_first(StrNode *first);
StrList *new_strList();
void free_strList(StrList *l);

#endif
