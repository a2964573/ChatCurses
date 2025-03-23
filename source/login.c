#include "main.h"


int login_process()
{
	if(_global.event == NULL || _global.login == NULL) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "[ERR] _global is NULL");
		return -1;
	}

	login_showInterface();

	MEVENT* event = _global.event;
	LOGIN*  login = _global.login;

	char command[SZ_COMMAND_MAX] = {0,};
	char buffer [SZ_BUFFER_MAX ] = {0,};
	char input  [SZ_INPUT_MAX  ] = {0,};
	int  size;
	int  len;

	int y;
	int key;
	while(login->login_state == FALSE) {
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

				move(y, POSX_FIRST);
				winstr(stdscr, buffer);

				size = utilSpaceTrim(buffer, strlen(buffer));
				if(size <= 0) {
					utilNcursesCommandShow(STATUSLVL_ERR, "잘못된 위치 클릭");
					continue;
				}

				if(buffer[0] == MARK_INPUT) {
					len = sprintf(input, "%s", &buffer[1]);
					len = utilNcursesInputString(input, SZ_INPUT_MAX, y, -1);
					if(len < 0) {
						UTILLOG(LOGLV_ERR, __FUNCTION__, "utilGetString Failed.");
						return 0;
					}

					if(y == POSY_LOGIN_INPUT_ID) {
						strcpy(login->id, input);
						login->id_len = len;
					}
					else
					if(y == POSY_LOGIN_INPUT_PW) {
						strcpy(login->pw, input);
						login->pw_len = len;
					}

					move(y, POSX_TEXT);
					clrtoeol();
					printw(input);

					sprintf(command, "Input String [%d:%s]", len, input);
					utilNcursesCommandShow(STATUSLVL_NOR, command);
				}
				else
				if(buffer[0] == MARK_BUTTON) {
					if(strcmp("Submit", &buffer[1]) != 0) {
						continue;
					}

					if(login->id_len == 0 || login->pw_len == 0) {
						utilNcursesCommandShow(STATUSLVL_ALERT, "아이디 또는 비밀번호를 입력하세요.");
						continue;
					}

					if(login_try() < 0) {
						utilNcursesCommandShow(STATUSLVL_ALERT, "로그인에 실패하였습니다.");
						login->login_state = FALSE;
						continue;
					}

					login->login_state = TRUE;
				}
				else {
					continue;
				}

				refresh();
			break;
			default:
				sprintf(command, "Your key input is [%d:0x%x][%c]", key, key, key);
				utilNcursesCommandShow(STATUSLVL_NOR, command);
			break;
		}
	}


	return 0;
}

int login_showInterface()
{
	clear();
	mvprintw(POSY_LOGIN_TITLE_LOGIN  , POSX_MARK, "%cLOGIN ", MARK_TITLE );
	mvprintw(POSY_LOGIN_LABEL_ID     , POSX_MARK, "%cID"    , MARK_LABEL );
	mvprintw(POSY_LOGIN_INPUT_ID     , POSX_MARK, "%c"      , MARK_INPUT );

	mvprintw(POSY_LOGIN_LABEL_PW     , POSX_MARK, "%cPW    ", MARK_LABEL );
	mvprintw(POSY_LOGIN_INPUT_PW     , POSX_MARK, "%c"      , MARK_INPUT );

	mvprintw(POSY_LOGIN_BUTTON_SUBMIT, POSX_MARK, "%cSubmit", MARK_BUTTON);


	return 0;
}

int login_try()
{
	LOGIN*  login  = _global.login;
	CLIENT* client = _global.client;

	if(clientMakeHeader(client, SERVICE_CODE_LOGIN0001) < 0) {
		return -1;
	}

	LOGIN0001_IN inbound;
	memset(&inbound, 0x20, sizeof(LOGIN0001_IN));

	memcpy(inbound.id, login->id, login->id_len);
	memcpy(inbound.pw, login->pw, login->pw_len);

	if(clientMakePacket(client, (char*)&inbound, SZ_LOGIN0001_IN) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientMakePacket Failed.");
		return -1;
	}

	if(client->sockfd == 0 && clientConnect(client, FALSE) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientConnect Failed.");
		return -1;
	}

	if(clientSocketSend(client) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientSocketSend Failed.");
		return -1;
	}

	if(clientSocketReceive(client) < 0) {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientSocketReceive Failed.");
		return -1;
	}

	if(clientClose(client) < 0) {
		utilNcursesCommandShow(STATUSLVL_ERR, "clientClose Failed");
		UTILLOG(LOGLV_ERR, __FUNCTION__, "clientClose Failed.");
		return -1;
	}

	LOGIN0001_OUT* outbound = (LOGIN0001_OUT*)client->recv_buffer;
	if(outbound->result[0] == 'N') {
		UTILLOG(LOGLV_ERR, __FUNCTION__, "Login Failed.");
		return -1;
	}

	char id  [128] = {0,};
	char name[128] = {0,};

	memcpy(id  , outbound->id  , sizeof(outbound->id  ));
	memcpy(name, outbound->name, sizeof(outbound->name));

	utilSpaceTrim(id, sizeof(id  ));
	utilSpaceTrim(id, sizeof(name));

	strcpy(login->id  , id);
	strcpy(login->name, name);

	clientSocketSendClear(client);
	clientSocketRecvClear(client);


	return 0;
}

