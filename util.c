#include "main.h"


int utilActiveAttr(GLOBAL* _global, int attr)
{
	if(_global == NULL) {
		return -1;
	}

	if(_global->attributes & attr) {
		return 0;
	}

	attron(attr);
	_global->attributes = _global->attributes | attr;


	return 0;
}

int utilDeActiveAttr(GLOBAL* _global, int attr)
{
	if(_global == NULL) {
		return -1;
	}

	if(!(_global->attributes & attr)) {
		return 0;
	}

	attroff(attr);
	_global->attributes &= ~attr;


	return 0;
}

int utilClicked(GLOBAL* _global)
{
	utilDeActiveAttr(_global, A_BOLD | A_UNDERLINE | A_REVERSE | A_BLINK | A_STANDOUT);
	utilActiveAttr(_global, A_BLINK);
	printw(CURSOR);
	utilDeActiveAttr(_global, A_BLINK);
	refresh();


	return 0;
}

int utilSpaceTrim(char* buffer, int size)
{
	if(buffer == NULL) {
		return -1;
	}

	if(size == 0) {
		size = strlen(buffer);
	}

	int first_pos = -1;
	int last_pos  =  0;
	int pos;
	for(pos = 0; pos < size; pos++) {
		if(buffer[pos] != 0x20) {
			if(first_pos == -1) {
				first_pos = pos;
			}

			last_pos = pos;
		}
	}

	int len = 0;
	if(first_pos >= 0) {
		len = last_pos - first_pos + 1;

		char temp[1024] = {0,};
		memcpy(temp, &buffer[first_pos], len);

		memset(buffer, 0x00, size);
		strcpy(buffer, temp);
	}
	else {
		memset(buffer, 0x00, size);
	}

	return len;
}

