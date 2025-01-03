#include "main.h"


int utilSpaceTrim(char* buffer, int len)
{
	if(buffer == NULL) {
		return -1;
	}

	if(len <= 0) {
		len = strlen(buffer);
	}

	int first_pos = -1;
	int last_pos  =  0;
	int pos;
	for(pos = 0; pos < len; pos++) {
		if(buffer[pos] != 0x20) {
			if(first_pos == -1) {
				first_pos = pos;
			}

			last_pos = pos;
		}
	}

	if(first_pos >= 0) {
		len = last_pos - first_pos + 1;

		memcpy(buffer, &buffer[first_pos], len);
		buffer[len] = 0x00;
	}
	else {
		buffer[0] = 0x00;
	}


	return len;
}

int utilStringBackSpace(char* buffer, int len, int pos)
{
    if(buffer == NULL) {
        return -1;
    }

    if(len <= 0) {
        len = strlen(buffer);
    }

    if(pos <= 0) {
        return 0;
    }
    else
    if(pos > len) {
        pos = len;
    }

    int apos = 0;
    int bpos;
    for(bpos = 0; bpos < len; bpos++) {
        if(bpos == pos - 1) {
            continue;
        }

        buffer[apos++] = buffer[bpos];
    }
    buffer[apos] = 0x00;


    return apos;
}

int utilStringDelete(char* buffer, int len, int pos)
{
    if(buffer == NULL) {
        return -1;
    }

    if(len <= 0) {
        len = strlen(buffer);
    }

    if(pos > len) {
        return 0;
    }

    int apos = 0;
    int bpos;
    for(bpos = 0; bpos < len; bpos++) {
        if(bpos == pos) {
            continue;
        }

        buffer[apos++] = buffer[bpos];
    }
    buffer[apos] = 0x00;


    return apos;
}

