#include "main.h"


int main(int argc, char** argv)
{
    // ncurses �ʱ�ȭ
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Ű�е� Ȱ��ȭ

    // ���콺 Ȱ��ȭ
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    // �޴� �׸� ����
    char *choices[] = { "Option 1", "Option 2", "Option 3", "Exit" };
    int num_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 0; // ���� ���õ� �׸�

    int choice;
    MEVENT event; // ���콺 �̺�Ʈ ����ü

    while (1) {
        // ȭ�� ����
        clear();
        for (int i = 0; i < num_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // ����
            }
            mvprintw(i + 1, 2, choices[i]);
            attroff(A_REVERSE);
        }
        mvprintw(num_choices + 2, 2, "Use Arrow keys, Enter, Mouse, or 'q' to quit.");
        refresh();

        // �Է� ó��
        choice = getch();
        switch (choice) {
            case KEY_UP: // ���� ȭ��ǥ
                highlight = (highlight == 0) ? num_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN: // �Ʒ��� ȭ��ǥ
                highlight = (highlight == num_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter Ű
                if (highlight == num_choices - 1) { // Exit ���� ��
                    endwin();
                    return 0;
                } else {
                    mvprintw(num_choices + 3, 2, "You selected: %s", choices[highlight]);
                    refresh();
                    getch(); // �޽��� ���
                }
                break;
            case KEY_MOUSE: // ���콺 �Է�
                if (getmouse(&event) == OK) { // ���콺 �̺�Ʈ ��������
                    if (event.bstate & BUTTON1_CLICKED) { // ���� ��ư Ŭ��
                        for (int i = 0; i < num_choices; i++) {
                            if (event.y == i + 1 && event.x >= 2 && event.x < 20) { // �޴� ���� Ȯ��
                                highlight = i;
                                if (highlight == num_choices - 1) { // Exit ���� ��
                                    endwin();
                                    return 0;
                                } else {
                                    mvprintw(num_choices + 3, 2, "You clicked: %s", choices[highlight]);
                                    refresh();
                                    getch(); // �޽��� ���
                                }
                            }
                        }
                    }
                }
                break;
            case 'q': // ����
                endwin();
                return 0;
            default:
                break;
        }
    }

    endwin(); // ncurses ����
    return 0;
}

