#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

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

typedef struct {
	int attributes;
	int login_state;
	MEVENT event;
} GLOBAL;

int init();
int process();

// login ///////////////////////////////////////////////////////////////////////////////
typedef struct {
	char id[64 ];
	char pw[512];
	int  id_len;
	int  pw_len;
} LOGIN;

#define SZ_COMMAND_MAX          2048
#define SZ_BUFFER_MAX           1024
#define SZ_INPUT_MAX            1024

#define POSY_LOGIN_TITLE_LOGIN   1
#define POSY_LOGIN_LABEL_ID      2
#define POSY_LOGIN_INPUT_ID      3
#define POSY_LOGIN_LABEL_PW      5
#define POSY_LOGIN_INPUT_PW      6
#define POSY_LOGIN_BUTTON_SUBMIT 8

int login_process(GLOBAL* _global);
int showLoginInterface();
////////////////////////////////////////////////////////////////////////////////////////


// util ncurses ////////////////////////////////////////////////////////////////////////
int utilNcursesActiveAttr(GLOBAL* _global, int attr);
int utilNcursesDeActiveAttr(GLOBAL* _global, int attr);
int utilNcursesClicked(GLOBAL* _global);
int utilNcursesInputString(char* buffer, int size, int y);
int utilNcursesCommandShow(char* descript);
////////////////////////////////////////////////////////////////////////////////////////


// util ////////////////////////////////////////////////////////////////////////////////
int utilSpaceTrim(char* buffer, int size);
int utilStringBackSpace(char* buffer, int len, int pos);
int utilStringDelete(char* buffer, int len, int pos);
////////////////////////////////////////////////////////////////////////////////////////

#endif // _MAIN_H_
