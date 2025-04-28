#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // 화면에 텍스트 출력
    mvprintw(5, 10, "Hello, world!");

    // 특정 좌표 (5, 10)에 있는 문자 확인
    chtype ch = mvinch(5, 10);  // (5, 10) 좌표에서 문자 읽기

    // 출력된 문자 확인
    if (ch != ERR) {
        printw("Character at (5, 10): %c\n", ch & A_CHARTEXT);  // 문자를 출력
    } else {
        printw("Error reading character at (5, 10)\n");
    }

    refresh();
    getch();
    endwin();
    return 0;
}

int isEmptyAtPosition(int y, int x) {
    // y, x 좌표에서 문자를 가져옵니다.
    chtype ch = mvinch(y, x);

    // 만약 공백 문자라면 (여기서는 공백 문자를 " "으로 가정)
    if (ch == ' ' || ch == '\0') {
        return 1;  // 해당 위치는 비어있습니다.
    }
    return 0;  // 해당 위치는 데이터가 있습니다.
}

int main() {
    initscr();
    cbreak();
    noecho();

    // 예시로 몇 개의 문자열을 출력합니다.
    mvprintw(1, 5, "Hello");
    mvprintw(2, 5, "World");

    // y좌표에 대해 x좌표들을 확인해봅니다.
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 50; x++) {
            if (isEmptyAtPosition(y, x)) {
                printw("Empty at (%d, %d)\n", y, x);
            }
        }
    }

    refresh();
    getch();  // 사용자 입력 대기

    endwin();
    return 0;
}


