#include "main.h"


int chat_process(GLOBAL* _global)
{
	if(_global == NULL || _global->event == NULL || _global->login == NULL) {
		return -1;
	}

	chat_showInterface();
	LOGIN* login = _global->login;

	if(login->login_state == FALSE) {
		utilNcursesCommandShow(STATUSLVL_ALERT, "로그인을 해주세요.");
		return 0;
	}

	char command[SZ_COMMAND_MAX] = {0,};
	sprintf(command, "%s님, 어서오세요.", login->id);

	utilNcursesCommandShow(STATUSLVL_NOR, command);

	while(TRUE) {
		if(chat_send(_global->client) < 0) {
			return -1;
		}
		clientSocketSendClear(_global->client);

		if(chat_recv(_global->client) < 0) {
			return -1;
		}
		clientSocketRecvClear(_global->client);
	}


	return 0;
}

int chat_showInterface()
{
	clear();
	mvprintw(POSY_CHAT_TITLE_ROOMNAME, POSX_MARK, "%cCHATTING ROOM", MARK_TITLE);
	mvprintw(POSY_CHAT_INPUT_MESSAGE , POSX_MARK, "%c"             , MARK_INPUT);


	return 0;
}

int chat_send(CLIENT* client)
{
	int rtn = utilNcursesInputString(client->send_buffer, SZ_SEND_BUFFER_MAX, POSY_CHAT_INPUT_MESSAGE, -1);
	if(rtn < 0) {
		utilNcursesCommandShow(STATUSLVL_ERR, "Send Input is Failed.");
		return -1;
	}
	client->send_len = rtn;

	char command[SZ_COMMAND_MAX] = {0,};
	sprintf(command, "send: %d:%.*s", client->send_len, client->send_len, client->send_buffer);
	utilNcursesCommandShow(STATUSLVL_ALERT, command);

	if(client->send_len > 0) {
		clientSocketSend(client);
	}


	return 0;
}

int chat_recv(CLIENT* client)
{
	if(clientSocketReceive(client) < 0) {
		utilNcursesCommandShow(STATUSLVL_ERR, "Recv Failed.");
		return -1;
	}
	chat_showMessage(client);


	return 0;
}

int chat_showMessage(CLIENT* client)
{
	chtype ch;

	int start_pos  = POSY_CHAT_TITLE_ROOMNAME + 2;
	int finish_pos = POSY_CHAT_INPUT_MESSAGE  - 2;
	int ypos;
	for(ypos = start_pos; ypos < finish_pos; ypos++) {
		ch = mvinch(ypos, POSX_MARK);

		if(ch == 0x00 || ch == 0x20) {
			break;
		}
	}

	if(ypos >= finish_pos) {
		int pos;
		for(pos = start_pos; pos < finish_pos; pos++) {
			move(pos, POSX_FIRST);
			clrtoeol();
		}

		ypos = start_pos;
	}

	mvprintw(ypos, POSX_MARK, "%c%s: %.*s", MARK_LABEL, "other", client->recv_len, client->recv_buffer);


	return 0;
}

