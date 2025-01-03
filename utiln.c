#include "main.h"


int utilNcursesActiveAttr(GLOBAL* _global, int attr)
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

int utilNcursesDeActiveAttr(GLOBAL* _global, int attr)
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

int utilNcursesClicked(GLOBAL* _global)
{
    utilNcursesDeActiveAttr(_global, A_BOLD | A_UNDERLINE | A_REVERSE | A_BLINK | A_STANDOUT);
    utilNcursesActiveAttr(_global, A_BLINK);
    printw(CURSOR);
    utilNcursesDeActiveAttr(_global, A_BLINK);
    refresh();


    return 0;
}

int _isValidChar(int ch)
{
    return (isdigit(ch) ||
            (ch >= '~' && ch <= '/') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z'));
}

int utilNcursesInputString(char* buffer, int size, int y)
{
    if(buffer == NULL || size <= 0) {
        return -1;
    }

    MEVENT event;

	char command[1024] = {0,};

	int   origin_len = strlen(buffer);
	char* origin = (char*)malloc(size);
	memset(origin, 0x00, size);
	strcpy(origin, buffer);

    int key = 0;
    int len = strlen(buffer);
    while(len < size - 1) {
		sprintf(command, "utilNcursesInputString [%d:%s]", len, buffer);
		utilNcursesCommandShow(command);

        key = getch();

        // 입력을 초기화한다
        if(key == KEY_ESC) {
			strcpy(buffer, origin);
			len = origin_len;
			break;
        }
		// 입력을 종료한다
		else
		if(key == 0x0a) {
			break;
		}
        // 이전 위치의 값을 삭제한다
        else
        if(key == 0x08) {
            if(len > 0) {
                len = utilStringBackSpace(buffer, len, len);
            }
        }
        // 포커스 아웃으로 판단하고 종료한다
        else
        if(key == KEY_MOUSE) {
            if(getmouse(&event) != OK) {
                continue;
            }

            break;
        }

        if(_isValidChar(key)) {
            buffer[len++] = (char)key;
            buffer[len  ] = 0x00;
        }

		move(y, 3);
		clrtoeol();
		printw(buffer);
		refresh();
    }

	free(origin);


    return len;
}

int utilNcursesCommandShow(char* descript)
{
    move(LINES - 1, 0);
    clrtoeol();
    printw(descript);
    refresh();


    return 0;
}
