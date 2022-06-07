#include <stdlib.h>

#include "ncurses.h"
#include "team_interface.h"

Team_Interface* Team_Interface_Init(int height, int width, int y, int x)
{
    Team_Interface* team_interface = malloc(sizeof(Team_Interface));
    
    team_interface->y = y;
    team_interface->x = x;

    team_interface->height = height + 2;
    team_interface->width = width + 2;

    WINDOW* window = newwin(team_interface->height, team_interface->width, LINES - (team_interface->height) * 3, team_interface->x);

    team_interface->window = window;

    team_interface->maxBar = 10; // TO CHANGE
    team_interface->spacing = 2;


    return team_interface;
}

void Team_Interface_Free(Team_Interface* team_interface)
{
    delwin(team_interface->window);

    free(team_interface);
    team_interface = NULL;
}