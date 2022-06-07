#ifndef TEAM_INTERFACE_H
#define TEAM_INTERFACE_H

#include "ncurses.h"

// Used to draw the team interface (aka window)
typedef struct 
{
    WINDOW* window;

    int maxBar;

    int spacing;

    int x;
    int y;

    int height;
    int width;

} Team_Interface;

// Allocate memory
Team_Interface* Team_Interface_Init(int height, int width, int y, int x);

void Team_Interface_Free(Team_Interface* team_interface);

#endif 