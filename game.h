#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "menu.h"
#include "battle.h"
#include "fighters_database.h"


typedef struct
{
    bool quit;

    Menu* menu;
    Battle* battle;
    Fighters_Database* fighters_database;

} Game;

// Constructor //

// Allocate memory
Game* Game_Init();

// Gameplay Functions //

// Main game loop, where everything happens
void Game_Loop(Game* game);

// Destructor //

// Free memory
void Game_Free(Game* game);

#endif