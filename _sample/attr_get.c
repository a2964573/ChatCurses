#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // 화면에 텍스트 출력
    mvprintw(5, 10, "Hello, world!");

    // 속성 및 문자 읽기
    attr_t attributes;
    short color_pair;
    mvchgat(5, 10, -1, A_BOLD, 1, NULL);  // 굵은 글씨로 출력

    attr_get(&attributes, &color_pair, NULL);

    printw("Attributes at (5, 10): %d, Color pair: %d\n", attributes, color_pair);

    refresh();
    getch();
    endwin();
    return 0;
}

