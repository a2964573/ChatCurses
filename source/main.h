#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "payload.h"

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#define MARK_BUTTON '!'
#define MARK_TITLE  '@'
#define MARK_LABEL  '#'
#define MARK_INPUT  '$'
#define CURSOR      "_"

#define KEY_ESC 27

#define POSX_FIRST 0
#define POSX_MARK  2
#define POSX_TEXT  3

#define POSY_LAST  LINES - 1

#define STATUSLVL_NOR   1
#define STATUSLVL_ERR   2
#define STATUSLVL_ALERT 3

#define SZ_SEND_BUFFER_MAX 4096
#define SZ_RECV_BUFFER_MAX 4096

typedef struct {
	char name[64 ];
	char id  [64 ];
	char pw  [512];
	int  id_len;
	int  pw_len;
	int  login_state;
} LOGIN;

typedef struct {
	char server_ip  [32 ];
	int  server_port;
	int  sockfd;
	struct sockaddr_in address;

	int  flags;
	int  isNonBlock;

	char send_buffer[SZ_SEND_BUFFER_MAX];
	int  send_len;

	char recv_buffer[SZ_RECV_BUFFER_MAX];
	int  recv_len;
} CLIENT;

typedef struct {
	int attributes;
	MEVENT* event;
	LOGIN*  login;
	CLIENT* client;
} GLOBAL;

extern GLOBAL _global;

int init();
int process();

// login ///////////////////////////////////////////////////////////////////////////////
#define SZ_COMMAND_MAX          2048
#define SZ_BUFFER_MAX           1024
#define SZ_INPUT_MAX            1024

#define POSY_LOGIN_TITLE_LOGIN   1
#define POSY_LOGIN_LABEL_ID      2
#define POSY_LOGIN_INPUT_ID      3
#define POSY_LOGIN_LABEL_PW      5
#define POSY_LOGIN_INPUT_PW      6
#define POSY_LOGIN_BUTTON_SUBMIT 8

int login_process();
int login_showInterface();
int login_try();
////////////////////////////////////////////////////////////////////////////////////////

// chat ////////////////////////////////////////////////////////////////////////////////
#define POSY_CHAT_TITLE_ROOMNAME 1
#define POSY_CHAT_INPUT_MESSAGE  LINES - 3

int chat_process();
int chat_showInterface();
int chat_send();
int chat_recv();
int chat_showMessage(CLIENT* client, char* name, char* message, int msg_len);
////////////////////////////////////////////////////////////////////////////////////////

// client //////////////////////////////////////////////////////////////////////////////
int clientConnect(CLIENT* client, int isNonBlock);
int clientSocketSend(CLIENT* client);
int clientSocketReceive(CLIENT* client);
int clientClose(CLIENT* client);
int clientSocketSendClear(CLIENT* client);
int clientSocketRecvClear(CLIENT* client);
int clientMakeHeader(CLIENT* client, char* svc_cd);
int clientMakePacket(CLIENT* client, char* inbound, int size);
////////////////////////////////////////////////////////////////////////////////////////

// util ncurses ////////////////////////////////////////////////////////////////////////
int utilNcursesActiveAttr(int attr);
int utilNcursesDeActiveAttr(int attr);
int utilNcursesClicked();
int utilNcursesInputString(char* buffer, int size, int y, int ms);
int utilNcursesCommandShow(int level, char* descript);
////////////////////////////////////////////////////////////////////////////////////////

// util ////////////////////////////////////////////////////////////////////////////////
#define LOGLV_NOR 1
#define LOGLV_DBG 2
#define LOGLV_ERR 3

#define DIR_PATH_LOG "/home/tester/Projects/C/ChatCurses/log"
#define UTILLOG(level, func, format, ...) utilLogging(__FILE__, func, __LINE__, level, format, ##__VA_ARGS__)

int utilLogging(const char* fileName, const char* funcName, const int line, int level, const char* format, ...);
int utilSpaceTrim(char* buffer, int size);
int utilStringBackSpace(char* buffer, int len, int pos);
int utilStringDelete(char* buffer, int len, int pos);
int utilGetDate(char* output);
long utilGetTime(char* output);
////////////////////////////////////////////////////////////////////////////////////////

#endif // _MAIN_H_
