#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // ȭ�鿡 �ؽ�Ʈ ���
    mvprintw(5, 0, "Hello, world!");
    mvprintw(6, 0, "This is a second line.");
    
    // �� ��° ���� �б�
    char line[100];
    move(6, 0);  // Ŀ���� (6, 0)���� �̵�
    winstr(stdscr, line);

    // ���� ���� ���
    printw("Read from line 6: %s\n", line);

    refresh();
    getch();
    endwin();
    return 0;
}

