#include "computer.h"

#include <stdlib.h>

int Computer_GetWeakestFighter(Player* opponent)
{
    // For Normal and Higher Difficulty

    int fighter_index;
    int check_health;

    // Get a random current health of a fighter who isn't defeated
    for (int i = 0; i < opponent->team->fighters_count; i++)
    {
        if (opponent->team->fighters[i] != NULL && !opponent->team->fighters[i]->is_locked)
        {
            check_health = opponent->team->fighters[i]->current_health;
            fighter_index = i; // In case if the last fighter has the least health
        }
    }

    // Update to get the fighter with the least health
    for (int i = 0; i < opponent->team->fighters_count; i++)
    {
        if (opponent->team->fighters[i] != NULL && !opponent->team->fighters[i]->is_locked)
        {
            // Check if the fighter has less health than the one keeped
            if (opponent->team->fighters[i]->current_health < check_health)
            {
                // Change variable keeping the minimum health
                check_health = opponent->team->fighters[i]->current_health;
                // Change fighter index with the one with less health
                fighter_index = i;
            }
                
        }
    }

    return fighter_index;
}

void Computer_UseRandomSkill(Fighter* computer_fighter, Fighter* player_fighter)
{
    int i = rand() % computer_fighter->skill_count;
    // Rechoose if skills has already been used
    while(computer_fighter->skills[i] == NULL || computer_fighter->skills[i]->isLocked)
        i = rand() % computer_fighter->skill_count;

    Fighter_UseSkill(computer_fighter->skills[i], player_fighter);
}

void Computer_Attack(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty)
{   
    //
    Fighter* computer_fighter = computer->team->fighters[fighter_index];
    Fighter* player_fighter;

    if (difficulty == BATTLE_DIFFICULTY_NOOB)
    {
        // Choose random fighter
        int i = rand() % player->team->fighters_count;
        // Rechoose if figher has already been defeated
        while(player->team->fighters[i] == NULL || player->team->fighters[i]->is_locked)
            i = rand() % player->team->fighters_count;
        player_fighter = player->team->fighters[i];
        
        Fighter_Attack(computer_fighter, player_fighter);
    }
    else if (difficulty == BATTLE_DIFFICULTY_EASY)
    {
        
        int weakest_fighter_index = Computer_GetWeakestFighter(player);
        player_fighter = player->team->fighters[weakest_fighter_index];
        
        Fighter_Attack(computer_fighter, player_fighter);
    }
    else if (difficulty == BATTLE_DIFFICULTY_NORMAL)
    {
        int weakest_fighter_index = Computer_GetWeakestFighter(player);
        player_fighter = player->team->fighters[weakest_fighter_index];

        // TO FIX : ADD COMPABILITY WITH FIGHTER WITH 0 SKILLS
        if (!Fighter_CheckAllSkillsAreLocked(computer_fighter))
            Computer_UseRandomSkill(computer_fighter, player_fighter);
        else
            Fighter_Attack(computer_fighter, player_fighter);
    }

    
}

void Computer_Update(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty)
{
    Computer_Attack(computer, fighter_index, player, difficulty);

    // Check if player team is defeated
    Player_IsDefeated(player);
}