#include "commons.h"

#include "ncurses.h"

void printError(char* error)
{
    clear();
    refresh();
    mvprintw(0, 0, "%s", error);
    mvprintw(2, 0, "Press any key to exit\n");
    getch();
}