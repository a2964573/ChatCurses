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
    mvgetnstr(6, 0, line, sizeof(line) - 1);

    // ���� ���� ���
    printw("Read from line 6: %s\n", line);

    refresh();
    getch();
    endwin();
    return 0;
}

