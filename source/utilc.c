#include "main.h"


int utilLogging(const char* file_name, const char* func_name, const int line, int level, const char* format, ...)
{
    va_list arg_ptr;

    int  length        =  0;
    char buffer[65536] = {0,};
    char date  [16   ] = {0,};
    char time  [32   ] = {0,};

	utilGetDate(date);
	utilGetTime(time);

    length = sprintf(buffer, "%s|%.2s:%.2s:%.2s.%.6s|%-8d|%-16s|%-20s|%-4d|"
        , date     , &time[0] , &time[2], &time[4], &time[6]
		, getpid(), file_name, func_name, line
	);

    switch(level) {
        case LOGLV_NOR:
            length += sprintf(&buffer[length], "NOR");
        break;
        case LOGLV_DBG:
            length += sprintf(&buffer[length], "DBG");
        break;
        case LOGLV_ERR:
            length += sprintf(&buffer[length], "ERR");
        break;
    }
    length += sprintf(&buffer[length], "|");

    va_start(arg_ptr, format);
    length += vsprintf(&buffer[length], format, arg_ptr);
    va_end(arg_ptr);

	char file_name_cp[1024] = {0,};
	strncpy(file_name_cp, file_name, sizeof(file_name) - 1);
	file_name_cp[sizeof(file_name) - 1] = 0x00;

	char* token = strtok(file_name_cp, ".");

	char file_nm     [256 ] = {0,};
	int  flen = sprintf(file_nm, "%s", token);

	while(TRUE) {
		token = strtok(NULL, ".");
		if(token == NULL) {
			break;
		}

		if(strcmp(token, "c") == 0 || strcmp(token, "cpp") == 0) {
			break;
		}

		flen += sprintf(&file_nm[flen], ".%s", token);
	}

    char file[1024] = {0,};
    sprintf(file, "%s/%s.%s.log", DIR_PATH_LOG, file_nm, date);

	FILE* file_ptr = fopen(file, "a");
    if(file_ptr == NULL) {
        return 0;
    }

	fprintf(file_ptr, "%s\n", buffer);

	fclose(file_ptr);

    if(level == LOGLV_ERR) {
		char efile[1024] = {0,};
		sprintf(efile, "%s/error.%s.log", DIR_PATH_LOG, date);

		FILE* efile_ptr = fopen(efile, "a");
		if(efile_ptr == NULL) {
			return 0;
		}

		fprintf(efile_ptr, "%s\n", buffer);

		fclose(efile_ptr);
	}


    return length;
}

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

int utilGetDate(char* output)
{
	time_t current_time = time(NULL);
	if(current_time == ((time_t) - 1)) {
		return 1;
	}

	struct tm* local_time = localtime(&current_time);

	char date[16] = {0,};
	int  len      =  0;
	len  = sprintf( date     , "%04d", local_time->tm_year + 1900);
	len += sprintf(&date[len], "%02d", local_time->tm_mon  + 1   );
	len += sprintf(&date[len], "%02d", local_time->tm_mday       );

	if(output != NULL) {
		strcpy(output, date);
	}


	return atoi(date);
}

long utilGetTime(char* output)
{
	struct timespec ts;
	struct tm* tm_info;

	char time[32] = {0,};

	if(clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		return 1;
	}

	tm_info = localtime(&ts.tv_sec);
	strftime(time, sizeof(time), "%H%M%S", tm_info);

	int len = strlen(time);
	len = sprintf(&time[len], "%06ld", ts.tv_nsec);

	if(output != NULL) {
		strcpy(output, time);
	}


	return atol(time);
}

