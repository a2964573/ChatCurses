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
	while(TRUE) {
		if(_global.login_state == FALSE) {
			if(login_process(&_global) < 0) {
				continue;
			}
		}

		move(POSY_LAST, POSX_FIRST);
		clrtoeol();
		printw("Login Success!!");
	}


	return 0;
}

