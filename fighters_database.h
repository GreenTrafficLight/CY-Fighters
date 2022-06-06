#ifndef FIGHTER_DATABASE_H
#define FIGHTER_DATABASE_H

#include "team.h"

// Total fighters in game
#define MAX_FIGHTER 256

typedef struct {

    char* fighters_name[MAX_FIGHTER];

    // Get fighter total count from the file 
    int fighters_count;

} Fighters_Database;

// Constructor //

// Allocate memory for the Fighters_Database struct
Fighters_Database* Fighters_Database_Init();

// Gameplay Functions //

// User choose their fighter
char* Fighters_Database_ChooseFighter(Fighters_Database* fighters_database);

// Create team from fighters database
void Fighters_Database_CreateTeam(Fighters_Database* fighters_database, Team* team, bool randomize);

// Update fighter database state
void Fighters_Database_Update(Fighters_Database* fighters_database, Team* team, bool randomize);

// Rendering Functions //

// Render all the fighters available on screen
void Fighters_Database_Render(Fighters_Database* fighters_database);

// Destructor //

// Free memory
void Fighters_Database_Free(Fighters_Database* fighters_database);

#endif