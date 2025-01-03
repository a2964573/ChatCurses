#include "main.h"


int login_process(GLOBAL* _global)
{
	showLoginInterface();

	MEVENT* event = &_global->event;
	LOGIN   login = {0,};

	char command[SZ_COMMAND_MAX] = {0,};
	char buffer [SZ_BUFFER_MAX ] = {0,};
	char input  [SZ_INPUT_MAX  ] = {0,};
	int  size;
	int  len;

	int y;
	int key;
	while(TRUE) {
		memset(buffer, 0x00, sizeof(SZ_BUFFER_MAX));
		memset(input , 0x00, sizeof(SZ_INPUT_MAX ));

		key = getch();

		switch(key) {
			case KEY_MOUSE:
				if(getmouse(event) != OK) {
					return -1;
				}


				if(!(event->bstate & BUTTON1_CLICKED)) {
					continue;
				}

				y = event->y;

				move(y, 0);
				winstr(stdscr, buffer);

				size = utilSpaceTrim(buffer, strlen(buffer));
				if(size <= 0) {
					utilNcursesCommandShow("[ERR] 잘못된 위치 클릭");
					continue;
				}

				if(buffer[0] == MARK_INPUT) {
					len = sprintf(input, "%s", &buffer[1]);
					len = utilNcursesInputString(input, SZ_INPUT_MAX, y);
					if(len < 0) {
						utilNcursesCommandShow("[ERR] utilGetString is Failed");
						return 0;
					}

					if(y == 3) {
						strcpy(login.id, input);
						login.id_len = len;
					}
					else
					if(y == 6) {
						strcpy(login.pw, input);
						login.pw_len = len;
					}

					move(y, 3);
					clrtoeol();
					printw(input);

					sprintf(command, "Input String [%d:%s]", len, input);
					utilNcursesCommandShow(command);
				}
				else
				if(buffer[0] == MARK_BUTON) {
					if(strcmp("Submit", &buffer[1]) != 0) {
						continue;
					}

					if(login.id_len == 0 || login.pw_len == 0) {
						utilNcursesCommandShow("아이디 또는 비밀번호를 입력하세요.");
						continue;
					}

					return 0;
				}
				else {
					continue;
				}

				refresh();
			break;
			default:
				sprintf(command, "Your key input is [%d:0x%x][%c]", key, key, key);
				utilNcursesCommandShow(command);
			break;
		}
	}


	return 0;
}

int showLoginInterface()
{
	mvprintw(1, 2, "%cLOGIN ", MARK_TITLE);
	mvprintw(2, 2, "%cID"    , MARK_LABEL);
	mvprintw(3, 2, "%c"      , MARK_INPUT);

	mvprintw(5, 2, "%cPW    ", MARK_LABEL);
	mvprintw(6, 2, "%c"      , MARK_INPUT);

	mvprintw(8, 2, "%cSubmit", MARK_BUTON);


	return 0;
}

