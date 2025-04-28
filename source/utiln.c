#include "main.h"


int utilNcursesActiveAttr(int attr)
{
    if(_global.attributes & attr) {
        return 0;
    }

    attron(attr);
    _global.attributes = _global.attributes | attr;


    return 0;
}

int utilNcursesDeActiveAttr(int attr)
{
    if(!(_global.attributes & attr)) {
        return 0;
    }

    attroff(attr);
    _global.attributes &= ~attr;


    return 0;
}

int utilNcursesClicked()
{
    utilNcursesDeActiveAttr(A_BOLD | A_UNDERLINE | A_REVERSE | A_BLINK | A_STANDOUT);
    utilNcursesActiveAttr(A_BLINK);
    printw(CURSOR);
    utilNcursesDeActiveAttr(A_BLINK);
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

int utilNcursesInputString(char* buffer, int size, int y, int ms)
{
    if(buffer == NULL || size <= 0) {
        return -1;
    }

	timeout(ms);

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
		utilNcursesCommandShow(STATUSLVL_NOR, command);

        key = getch();

		if(key < 0) {
			break;
		}
        else
        if(key == KEY_ESC) {
			// 입력을 초기화한다
			strcpy(buffer, origin);
			len = origin_len;
			break;
        }
		else
		if(key == 0x0a) {
			// 입력을 종료한다
			break;
		}
        else
        if(key == 0x08) {
			// 이전 위치의 값을 삭제한다
            if(len > 0) {
                len = utilStringBackSpace(buffer, len, len);
            }
        }
        else
        if(key == KEY_MOUSE) {
			// 포커스 아웃으로 판단하고 종료한다
            if(getmouse(&event) != OK) {
                continue;
            }

			// 동일한 y라면 포커스 아웃이 아니다
			if(event.y == y) {
				continue;
			}

            break;
        }

        if(_isValidChar(key)) {
            buffer[len++] = (char)key;
            buffer[len  ] = 0x00;
        }

		move(y, POSX_TEXT);
		clrtoeol();
		printw(buffer);
		refresh();
    }

	free(origin);
	timeout(-1);


    return len;
}

int utilNcursesCommandShow(int level, char* descript)
{
	char lv[16] = {0,};
	switch(level) {
		case STATUSLVL_NOR:
			strcpy(lv, "[NOR]");
		break;
		case STATUSLVL_ERR:
			strcpy(lv, "[ERR]");
		break;
		case STATUSLVL_ALERT:
			strcpy(lv, "[!!!]");
		break;
	}

    move(POSY_LAST, POSX_MARK);
    clrtoeol();
    printw("%s %s", lv, descript);
    refresh();


    return 0;
}
