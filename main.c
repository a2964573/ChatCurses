#include "main.h"


GLOBAL _global = {0,};

int main(int argc, char** argv)
{
	if(init() < 0) {
		return -1;
	}

	process();
	endwin();


	return 0;
}

int init()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	clear();

	_global.attributes  = A_BOLD | A_UNDERLINE | A_REVERSE | A_BLINK | A_STANDOUT;
	_global.login_state = FALSE;


	return 0;
}

int process()
{
	MEVENT event;
	int key;
//	int x;
	int y;

//	int input;
	while(TRUE) {
		if(_global.login_state == FALSE) {
			if(login_process(&_global) < 0) {
				continue;
			}
		}

		key = getch();

		switch(key) {
			case KEY_MOUSE:
				if(getmouse(&event) != OK) {
					continue;
				}

//				x = event.x;
				y = event.y;

				if(event.bstate & BUTTON1_CLICKED) {
					char buffer[1024] = {0,};
					move(y, 0);
					winstr(stdscr, buffer);

					int size = utilSpaceTrim(buffer, strlen(buffer));

					move(LINES - 1, 0);
					clrtoeol();
					mvprintw(LINES - 1, 0, "Your mouse clicked line %d, string[%d:%s]", y, size, buffer);
					refresh();
				}
			break;
			default:
				mvprintw(1, 2, "You Input Key is [%d:%c]", key, key);
			break;
		}
	}


	return 0;
}

