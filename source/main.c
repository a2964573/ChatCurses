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


	return 0;
}

int process()
{
	MEVENT  event = {0,};
	_global.event = &event;

	LOGIN   login = {0,};
	_global.login = &login;

	CLIENT  client = {0,};
	_global.client = &client;

	while(TRUE) {
		if(_global.login->login_state == FALSE) {
			if(login_process(&_global) < 0) {
				continue;
			}

			 _global.login->login_state = TRUE;
		}

		if(chat_process(&_global) < 0) {
			clientClose(_global.client);
			break;
		}
	}


	return 0;
}

