#include <stdlib.h>

#include "ncurses.h"
#include "team_interface.h"

Team_Interface* Team_Interface_Init(int height, int width, int y, int x)
{
    Team_Interface* team_interface = malloc(sizeof(Team_Interface));

    WINDOW* window = newwin(height + 2, width + 2, LINES - (height + 2) * 3, x);

    team_interface->window = window;

    team_interface->maxBar = 10; // TO CHANGE
    team_interface->spacing = 2;

    team_interface->height = height;
    team_interface->width = width;

    return team_interface;
}

void Team_Interface_Free(Team_Interface* team_interface)
{
    delwin(team_interface->window);

    free(team_interface);
    team_interface = NULL;
}