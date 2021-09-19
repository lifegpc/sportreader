#include "util.h"
#include<malloc.h>
#include<string.h>
#include"stdio_config.h"
#include"stdlib_config.h"

void extractInt(char* origin, int *dest, int start, int end, int precise) {
	int llen = end - start;
	if (llen > 0) {
		int need_bytes = precise <= 0 ? llen : precise;
		char *temp = (char*)malloc(need_bytes + 1);
		int copy_num = precise <= 0 ? llen : min(precise, llen);
		memcpy(temp, origin + start, copy_num);
		while (precise > 0 && copy_num < need_bytes) {
			temp[copy_num] = '0';
			copy_num++;
		}
		temp[need_bytes] = 0;
		sscanf_s(temp, "%d", dest);
		free(temp);
		temp = NULL;
	}
}

int copyCharToBuff(void** buff, size_t *buff_size, char* str, size_t str_len) {
	if (buff == NULL || buff_size == NULL) return 1;
	if (*buff == NULL) return 1;
	if (str_len <= 0) str_len = strlen(str);
	size_t copy_nums = min(str_len, *buff_size);
	memcpy(*buff, str, copy_nums);
	*buff = (char*)*buff + copy_nums;
	*buff_size -= copy_nums;
	return copy_nums < str_len ? 1 : 0;
}

int copyIntToBuff(void** buff, size_t *buff_size, int source, int minlen) {
	if (buff == NULL || buff_size == NULL) return 1;
	if (*buff == NULL) return 1;
	if (source < 0) {
		if (*buff_size <= 0) return 1;
		char g = '-';
		memcpy(*buff, &g, 1);
		*buff = ((char *)*buff) + 1;
		*buff_size--;
		source = -source;
	}
	int add_num = 0;
	do {
		char g = source % 10 + '0';
		if (add_num >= *buff_size) {
			swapBuff(*buff, 1, add_num);
			*buff = ((char*)*buff) + add_num;
			*buff_size = 0;
			return 1;
		}
		memcpy((char*)*buff + add_num, &g, 1);
		add_num++;
		source = source / 10;
	} while (source > 0 || (minlen > 0 && add_num < minlen));
	swapBuff(*buff, 1, add_num);
	*buff = ((char*)*buff) + add_num;
	*buff_size -= add_num;
	return 0;
}

void swapBuff(void* buff, size_t unit_len, size_t len) {
	if (unit_len < 1 || len < 2) return;
	size_t mid = len / 2;
	size_t i = 0;
	void* temp = malloc(unit_len);
	for (; i < mid; i++) {
		memcpy(temp, (char*)buff + i * unit_len, unit_len);
		memcpy((char*)buff + i * unit_len, (char*)buff + (len - 1 - i) * unit_len, unit_len);
		memcpy((char*)buff + (len - 1 - i) * unit_len, temp, unit_len);
	}
	free(temp);
	temp = NULL;
}
