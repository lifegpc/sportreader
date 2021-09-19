#ifndef _UTIL_H
#define _UTIL_H

#include <stddef.h>

void extractInt(char* origin, int *dest, int start, int end, int precise);
/*Copy string to buff. If buff is not enough, return 1.
The buff will move in copy progress.*/
int copyCharToBuff(void** buff, size_t *buff_size, char* str, size_t str_len);
/*Copy int to buff. If buff is not enough, return 1.
minlen: the string at least minlen. 0 if not used. if minlen is 3 and source is 3, it will be 003. 
The buff will move in copy progress.*/
int copyIntToBuff(void** buff, size_t *buff_size, int source, int minlen);
/*Swap content in buff. unit_len is a unit's length, len is how many units should be swaped.*/
void swapBuff(void* buff, size_t unit_len, size_t len);

#endif
