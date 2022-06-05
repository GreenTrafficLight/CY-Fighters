#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include "battle.h"

// Computer find which fighter has the least health (For Normal and higher difficulty)
int Computer_GetWeakestFighter(Player* opponent);

// Computer use a random skill on the weakest fighter
void Computer_UseRandomSkill(Fighter* computer_fighter, Fighter* player_fighter);

// Computer attack the player
void Computer_Attack(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty);

// Update state of computer
void Computer_Update(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty);

#endif