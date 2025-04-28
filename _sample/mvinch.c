#include <ncurses.h>

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // ȭ�鿡 �ؽ�Ʈ ���
    mvprintw(5, 10, "Hello, world!");

    // Ư�� ��ǥ (5, 10)�� �ִ� ���� Ȯ��
    chtype ch = mvinch(5, 10);  // (5, 10) ��ǥ���� ���� �б�

    // ��µ� ���� Ȯ��
    if (ch != ERR) {
        printw("Character at (5, 10): %c\n", ch & A_CHARTEXT);  // ���ڸ� ���
    } else {
        printw("Error reading character at (5, 10)\n");
    }

    refresh();
    getch();
    endwin();
    return 0;
}

int isEmptyAtPosition(int y, int x) {
    // y, x ��ǥ���� ���ڸ� �����ɴϴ�.
    chtype ch = mvinch(y, x);

    // ���� ���� ���ڶ�� (���⼭�� ���� ���ڸ� " "���� ����)
    if (ch == ' ' || ch == '\0') {
        return 1;  // �ش� ��ġ�� ����ֽ��ϴ�.
    }
    return 0;  // �ش� ��ġ�� �����Ͱ� �ֽ��ϴ�.
}

int main() {
    initscr();
    cbreak();
    noecho();

    // ���÷� �� ���� ���ڿ��� ����մϴ�.
    mvprintw(1, 5, "Hello");
    mvprintw(2, 5, "World");

    // y��ǥ�� ���� x��ǥ���� Ȯ���غ��ϴ�.
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 50; x++) {
            if (isEmptyAtPosition(y, x)) {
                printw("Empty at (%d, %d)\n", y, x);
            }
        }
    }

    refresh();
    getch();  // ����� �Է� ���

    endwin();
    return 0;
}


