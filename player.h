#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "team_interface.h"
#include "team.h"

enum PLAYER_CHOICE { PLAYER_CHOICE_ATTACK, PLAYER_CHOICE_SKILL,};

typedef struct 
{
    Team* team;

	Team_Interface* team_interface;

	bool isYourTurn;

	bool is_defeated;

	bool isComputer;

} Player;

// Constructor //

Player* Player_Init(bool isComputer, Team_Interface* team_interface);

void Player_CreateTeam(Player* player, bool isComputer);

// Gameplay Functions //
 
// Handle player keyboard inputs
void Player_HandleInputs(Player* player, int fighter_index, Player* opponent);

// Player choose between the "Normal attack" and "Special Skills" option
enum PLAYER_CHOICE Player_ChooseChoice(Player* player, int fighter_index);

// Player choose which skill to use from the fighter (If the "Special Skills" option has been choosen)
int Player_ChooseSkill(Player* player, int fighter_index);

// Player choose which fighter to attack
int Player_ChooseFighter(Player* player, int fighter_index, Player* opponent);

// Check if the player has been defeated
void Player_IsDefeated(Player* player);

// Update state of player
void Player_Update(Player* player, int fighter_index, Player* opponent);

// Rendering Functions //

void Player_Render(Player* player);

// Destructor //

// Free memory
void Player_Free(Player* player);

#endif