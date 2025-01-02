#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef TRUE
#define FALSE 0
#endif // TRUE

#define MARK_LABEL '#'
#define MARK_INPUT '$'
#define CURSOR     "_"

typedef struct {
	int attributes;
	int login_state;
} GLOBAL;

int init();
int process();

// util //////////////////////////////
int login_process(GLOBAL* _global);
//////////////////////////////////////

// util //////////////////////////////
int utilActiveAttr(GLOBAL* _global, int attr);
int utilDeActiveAttr(GLOBAL* _global, int attr);
int utilClicked(GLOBAL* _global);
int utilSpaceTrim(char* buffer, int size);
//////////////////////////////////////

#endif // _MAIN_H_
