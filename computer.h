#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include "battle.h"

// Computer find which fighter has the least health (For Normal and higher difficulty)
int Computer_GetWeakestFighter(Player* opponent);

// Computer chooses a random player fighter
int Computer_GetRandomPlayerFighter(Player* player);

// Computer chooses a random skill
int Computer_GetRandomSkill(Fighter* computer_fighter);

// Computer use a random skill on the weakest fighter
void Computer_UseRandomSkill(Player* computer, int fighter_index, Player* player);

// Computer attack the player
void Computer_Attack(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty);

// Update state of computer
void Computer_Update(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty);

#endif