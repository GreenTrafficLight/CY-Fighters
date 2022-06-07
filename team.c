#include "team.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ncurses.h"

#include "commons.h"

// Constructor

Team* Team_Init(Team_Interface* interface)
{
    Team* team = malloc(sizeof(Team));
    
    team->interface = interface;

    team->name = malloc(sizeof(char)* 16);

    team->fighters_count = 0;

    for (int i = 0; i < TEAM_MAX_SIZE; i++)
        team->fighters[i] = NULL;

    return team;
}

void Team_AddFighter(Team* team, Fighter* fighter)
{  
    // If team fighters count exceed the maximum fighter count size, do nothing
    if (team->fighters_count > TEAM_MAX_SIZE)
        return;

    // Add fighter to the team
    for (int i = 0; i < TEAM_MAX_SIZE; i++)
    {
        if (team->fighters[i] == NULL)
        {
            team->fighters[i] = fighter;
            
            // Set team interface for the fighter and his skills
            team->fighters[i]->team_interface = team->interface;
            for (int j = 0; j < team->fighters[i]->skill_count; j++)
                team->fighters[i]->skills[j]->team_interface = team->interface;
            
            break;
        }
    }

    team->fighters_count++;
}

// Gameplay Functions //

void Team_isBeaten(Team* team)
{
    for (int i = 0; i < team->fighters_count; i++)
    {
        if (team->fighters[i] != NULL)
        {
            if(!team->fighters[i]->is_locked)
            {
                team->is_locked = false;
                return;
            }          
        }
    }
    team->is_locked = true;
}

// Rendering Functions

void Team_DrawName(Team* team)
{
    char* team_name_drawing = malloc(sizeof(char)* strlen(team->name));

    for(long unsigned int i = 0; i < strlen(team->name); i++)
    {
        team_name_drawing[i] = toupper(team->name[i]);
    }

    team_name_drawing[strlen(team_name_drawing) - 1] = '\0';

    mvwprintw(team->interface->window, 0, 1, "[%s]", team->name);
}

void Team_Render(Team* team)
{   
    if (team != NULL)
    {
        // Clear window
        wclear(team->interface->window);

        // Reset the borders around the box
        box(team->interface->window, 0, 0);

        // Draw Team Name
        Team_DrawName(team);

        // Draw each fighter with their stats
        for(int i = 0; i < team->fighters_count; i++)
        {
            Fighter_Render(team->fighters[i], i);
        }
    
        // Refresh window so the drawings can show up
        wrefresh(team->interface->window);
    }
    else 
    {
        printError("Team couldn't be rendered !");
    }
}

// Destructor //

void Team_Free(Team* team)
{
    for (int i = 0; i < team->fighters_count; i++)
        Fighter_Free(team->fighters[i]);

    free(team);
    team = NULL;
}