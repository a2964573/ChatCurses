#include "main.h"


int login_process(GLOBAL* _global)
{
	mvprintw(1, 2, "%c LOGIN", MARK_LABEL);
	mvprintw(2, 2, "%c ID"   , MARK_LABEL);
	mvprintw(3, 2, "%c"      , MARK_INPUT);

	mvprintw(5, 2, "%c PW   ", MARK_LABEL);
	mvprintw(6, 2, "%c"      , MARK_INPUT);

	int key = getch();
	switch(key) {
		case KEY_MOUSE:
			if(getmouse(&event) != OK) {
				continue;
			}

			x = event.x;
			y = event.y;

			if(event.bstate & BUTTON1_CLICKED) {
				move(y, 0);
				winstr(stdscr, buffer);

				int size = utilSpaceTrim(buffer, strlen(buffer));

				if(buffer[0] == MARK_INPUT) {

				}

				refresh();
			}
		break;
		default:
			mvprintw(1, 2, "You Input Key is [%d:%c]", key, key);
		break;
	}


	return 0;
}

