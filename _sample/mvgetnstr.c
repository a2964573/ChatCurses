#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // 화면에 텍스트 출력
    mvprintw(5, 0, "Hello, world!");
    mvprintw(6, 0, "This is a second line.");
    
    // 두 번째 줄을 읽기
    char line[100];
    mvgetnstr(6, 0, line, sizeof(line) - 1);

    // 읽은 내용 출력
    printw("Read from line 6: %s\n", line);

    refresh();
    getch();
    endwin();
    return 0;
}

