#include "main.h"


int main(int argc, char** argv)
{
    // ncurses 초기화
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // 키패드 활성화

    // 마우스 활성화
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    // 메뉴 항목 정의
    char *choices[] = { "Option 1", "Option 2", "Option 3", "Exit" };
    int num_choices = sizeof(choices) / sizeof(choices[0]);
    int highlight = 0; // 현재 선택된 항목

    int choice;
    MEVENT event; // 마우스 이벤트 구조체

    while (1) {
        // 화면 갱신
        clear();
        for (int i = 0; i < num_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // 강조
            }
            mvprintw(i + 1, 2, choices[i]);
            attroff(A_REVERSE);
        }
        mvprintw(num_choices + 2, 2, "Use Arrow keys, Enter, Mouse, or 'q' to quit.");
        refresh();

        // 입력 처리
        choice = getch();
        switch (choice) {
            case KEY_UP: // 위쪽 화살표
                highlight = (highlight == 0) ? num_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN: // 아래쪽 화살표
                highlight = (highlight == num_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter 키
                if (highlight == num_choices - 1) { // Exit 선택 시
                    endwin();
                    return 0;
                } else {
                    mvprintw(num_choices + 3, 2, "You selected: %s", choices[highlight]);
                    refresh();
                    getch(); // 메시지 대기
                }
                break;
            case KEY_MOUSE: // 마우스 입력
                if (getmouse(&event) == OK) { // 마우스 이벤트 가져오기
                    if (event.bstate & BUTTON1_CLICKED) { // 왼쪽 버튼 클릭
                        for (int i = 0; i < num_choices; i++) {
                            if (event.y == i + 1 && event.x >= 2 && event.x < 20) { // 메뉴 범위 확인
                                highlight = i;
                                if (highlight == num_choices - 1) { // Exit 선택 시
                                    endwin();
                                    return 0;
                                } else {
                                    mvprintw(num_choices + 3, 2, "You clicked: %s", choices[highlight]);
                                    refresh();
                                    getch(); // 메시지 대기
                                }
                            }
                        }
                    }
                }
                break;
            case 'q': // 종료
                endwin();
                return 0;
            default:
                break;
        }
    }

    endwin(); // ncurses 종료
    return 0;
}

