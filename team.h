#ifndef TEAM_H
#define TEAM_H

#define TEAM_MAX_SIZE 3

#include <stdbool.h>

#include "fighter.h"
#include "team_interface.h"

typedef struct 
{
    Team_Interface* interface;

    Fighter* fighters[TEAM_MAX_SIZE];
    int fighters_count;

    // Team Attributes
    char* name;

    bool is_locked;

} Team;

// Constructor //

// Allocate memory
Team* Team_Init(Team_Interface* interface);

// Add fighter to team
void Team_AddFighter(Team* team, Fighter* fighter);

// Gameplay Functions //

// Check if team has been defeated
void Team_isBeaten(Team* team);

// Update state of team
void Team_Update(Team* team);

// Rendering Functions //

// Draw team name
void Team_DrawName(Team* team);

// Render team on screen
void Team_Render(Team* team);

// Destructor //

// Free memory
void Team_Free(Team* team);

#endif