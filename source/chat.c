#include "main.h"


int chat_process()
{
	if(_global.event == NULL || _global.login == NULL) {
		return -1;
	}

	chat_showInterface();
	LOGIN*  login  = _global.login;
	CLIENT* client = _global.client;

	if(login->login_state == FALSE) {
		utilNcursesCommandShow(STATUSLVL_ALERT, "로그인을 해주세요.");
		return 0;
	}

	char command[SZ_COMMAND_MAX] = {0,};
	sprintf(command, "%s님, 어서오세요.", login->id);
	utilNcursesCommandShow(STATUSLVL_NOR, command);

    if(client->sockfd <= 0 && clientConnect(client, TRUE) < 0) {
        UTILLOG(LOGLV_ERR, __FUNCTION__, "clientConnect Failed.");
        return -1;
    }
	UTILLOG(LOGLV_NOR, __FUNCTION__, "clientConnect Success.");

	fd_set readfds;

	int activity;
	int rtn = 0;
	while(TRUE) {
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(client->sockfd, &readfds);

		activity = select(client->sockfd + 1, &readfds, NULL, NULL, NULL);
		if(activity < 0) {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "select failed.");
			break;
		}

		if(FD_ISSET(STDIN_FILENO, &readfds)) {
			UTILLOG(LOGLV_DBG, __FUNCTION__, "select file discriptor.[STDIN_FILENO][%d:%d]"
				, STDIN_FILENO, client->sockfd);
			rtn = chat_send();
			if(rtn < 0) {
				UTILLOG(LOGLV_ERR, __FUNCTION__, "chat_send failed.[%d]", rtn);
				break;
			}
			clientSocketSendClear(client);
		}
		else
		if(FD_ISSET(client->sockfd, &readfds)) {
			UTILLOG(LOGLV_DBG, __FUNCTION__, "select file discriptor.[client->sockfd][%d:%d]"
				, STDIN_FILENO, client->sockfd);
			rtn = chat_recv();
			if(rtn < 0) {
				UTILLOG(LOGLV_ERR, __FUNCTION__, "chat_recv failed.[%d]", rtn);
				break;
			}
			clientSocketRecvClear(client);
		}
	}

    if(clientClose(client) < 0) {
        utilNcursesCommandShow(STATUSLVL_ERR, "clientClose failed");
        UTILLOG(LOGLV_ERR, __FUNCTION__, "clientClose failed.");
        return -1;
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

int chat_send()
{
	int  msg_len = 0;
	char message[SZ_MCHAT0001_MESSAGE_MAX] = {0,};

//	UTILLOG(LOGLV_DBG, __FUNCTION__, "message init[%d:%.*s].", SZ_MCHAT0001_MESSAGE_MAX, SZ_MCHAT0001_MESSAGE_MAX, message);

	int rtn = utilNcursesInputString(message, SZ_MCHAT0001_MESSAGE_MAX, POSY_CHAT_INPUT_MESSAGE, -1);
	if(rtn < 0) {
		utilNcursesCommandShow(STATUSLVL_ERR, "Send Input is Failed.");
		return -1;
	}
	else
	if(rtn == 0) {
		return 0;
	}
	msg_len = rtn;

	UTILLOG(LOGLV_DBG, __FUNCTION__, "message input[%d:%.*s].", msg_len, msg_len, message);

	move(POSY_CHAT_INPUT_MESSAGE, POSX_TEXT);
	clrtoeol();
	refresh();

    if(clientMakeHeader(_global.client, SERVICE_CODE_MCHAT0001) < 0) {
        UTILLOG(LOGLV_ERR, __FUNCTION__, "clientMakeHeader Failed.");
        return -1;
    }

	MCHAT0001_IN inbound;
	memset(&inbound, 0x20, SZ_MCHAT0001_IN);

	memcpy(inbound.id     , _global.login->id  , strlen(_global.login->id  ));
	memcpy(inbound.name   , _global.login->name, strlen(_global.login->name));
	memcpy(inbound.message, message            , msg_len                    );

    if(clientMakePacket(_global.client, (char*)&inbound, SZ_MCHAT0001_IN) < 0) {
        UTILLOG(LOGLV_ERR, __FUNCTION__, "clientMakePacket Failed.");
        return -1;
    }

	char command[SZ_COMMAND_MAX] = {0,};
	sprintf(command, "send[%d:%.*s]", msg_len, msg_len, message);
	utilNcursesCommandShow(STATUSLVL_ALERT, command);

	rtn = clientSocketSend(_global.client);
	if(rtn <= 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientSocketSend failed.[%d]", rtn * (-1));

		memset(command, 0x00, SZ_COMMAND_MAX);
		sprintf(command, "send error [%d]", rtn * (-1));
		utilNcursesCommandShow(STATUSLVL_ERR, command);
		return rtn;
	}
	_global.client->send_len = rtn;

	return _global.client->send_len;
}

int chat_recv()
{
	int rtn = clientSocketReceive(_global.client);
	if(rtn < 0) {
		utilNcursesCommandShow(STATUSLVL_ERR, "Recv Failed.");
		return -1;
	}
	if(rtn == 0) {
		return 0;
	}

	UTILLOG(LOGLV_NOR, __FUNCTION__, "Recv Success[%d:%.*s]."
		, _global.client->recv_len, _global.client->recv_len, _global.client->recv_buffer);

	char name[128] = {0,};
	char message[SZ_MCHAT0002_MESSAGE_MAX + 8] = {0,};

	char command[SZ_COMMAND_MAX] = {0,};
	char* svc_cd = &_global.client->recv_buffer[HEADER_SVCCD_POS];
	if(memcmp(svc_cd, SERVICE_CODE_MCHAT0001, HEADER_SVCCD_LEN) == 0) {
		sprintf(command, "recv success.");
		utilNcursesCommandShow(STATUSLVL_ALERT, command);

		MCHAT0001_OUT* outbound = (MCHAT0001_OUT*)&_global.client->recv_buffer[SZ_HEADER];
		if(outbound->result[0] == 'Y') {
			strcpy(name   , _global.login->name);
			memcpy(message, outbound->message, SZ_MCHAT0001_MESSAGE_MAX);

			utilSpaceTrim(name   , strlen(name   ));
			utilSpaceTrim(message, strlen(message));

			chat_showMessage(_global.client, name, message, strlen(message));
		}
	}
	else
	if(memcmp(svc_cd, SERVICE_CODE_MCHAT0002, HEADER_SVCCD_LEN) == 0) {
		MCHAT0002_IN* inbound = (MCHAT0002_IN*)&_global.client->recv_buffer[SZ_HEADER];

		memcpy(name   , inbound->name   , 64                      );
		memcpy(message, inbound->message, SZ_MCHAT0002_MESSAGE_MAX);

		utilSpaceTrim(name   , 0);
		utilSpaceTrim(message, 0);

		chat_showMessage(_global.client, name, message, strlen(message));

		if(clientMakeHeader(_global.client, SERVICE_CODE_MCHAT0002) < 0) {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "clientMakeHeader Failed.");
			return -1;
		}

		MCHAT0002_OUT outbound;
		memset(&outbound, 0x20, SZ_MCHAT0002_OUT);

		outbound.result[0] = 'Y';
		memcpy(outbound.message, message, strlen(message));

		if(clientMakePacket(_global.client, (char*)&outbound, SZ_MCHAT0002_OUT) < 0) {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "clientMakePacket Failed.");
			return -1;
		}

		UTILLOG(LOGLV_DBG, __FUNCTION__, "send[%d:%.*s]"
			, _global.client->send_len, _global.client->send_len, _global.client->send_buffer);
		rtn = clientSocketSend(_global.client);
		if(rtn <= 0) {
			UTILLOG(LOGLV_ERR, __FUNCTION__, "clientSocketSend failed.[%d]", rtn * (-1));

			memset(command, 0x00, SZ_COMMAND_MAX);
			sprintf(command, "send error [%d]", rtn * (-1));
			utilNcursesCommandShow(STATUSLVL_ERR, command);

			return rtn;
		}
		_global.client->send_len = rtn;
	}
	else {
		return -1;
	}


	return rtn;
}

int chat_showMessage(CLIENT* client, char* name, char* message, int msg_len)
{
	if(client == NULL || name == NULL || message == NULL) {
		return -1;
	}

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

	mvprintw(ypos, POSX_MARK, "%c%s: %.*s", MARK_LABEL, name, msg_len, message);
	refresh();

	return 0;
}

