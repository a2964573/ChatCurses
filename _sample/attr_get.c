#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // ȭ�鿡 �ؽ�Ʈ ���
    mvprintw(5, 10, "Hello, world!");

    // �Ӽ� �� ���� �б�
    attr_t attributes;
    short color_pair;
    mvchgat(5, 10, -1, A_BOLD, 1, NULL);  // ���� �۾��� ���

    attr_get(&attributes, &color_pair, NULL);

    printw("Attributes at (5, 10): %d, Color pair: %d\n", attributes, color_pair);

    refresh();
    getch();
    endwin();
    return 0;
}

