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

int Computer_GetRandomPlayerFighter(Player* player)
{
    // Choose random fighter
    int i = rand() % player->team->fighters_count;
    // Rechoose if figher has already been defeated
    while(player->team->fighters[i] == NULL || player->team->fighters[i]->is_locked)
        i = rand() % player->team->fighters_count;

    return i;
}

int Computer_GetRandomSkill(Fighter* computer_fighter)
{
    // Choose random skill
    int i = rand() % computer_fighter->skill_count;
    // Rechoose if skills has already been used
    while(computer_fighter->skills[i] == NULL || computer_fighter->skills[i]->isLocked)
        i = rand() % computer_fighter->skill_count;

    return i;
}

void Computer_UseRandomSkill(Player* computer, int fighter_index, Player* player)
{   
    int selected_skill_index, selected_fighter_index;
    Fighter* selected_fighter = NULL;

    Fighter* computer_fighter = computer->team->fighters[fighter_index];

    // Get index of a random skill
    selected_skill_index = Computer_GetRandomSkill(computer_fighter);

    Skill* selected_skill = computer_fighter->skills[selected_skill_index];

    switch (selected_skill->modifier)
    {
        // Computer choose it's own fighters
        case SKILL_MODIFIER_INCREASE:
            selected_fighter_index = Computer_GetWeakestFighter(computer);
            selected_fighter = computer->team->fighters[selected_fighter_index];

            Fighter_UseSkill(selected_skill, selected_fighter);
            break;

        // Computer use skill on a player fighter //

        // Computer attack multiple times
        case SKILL_MODIFIER_LOOP:
            for (int i = 0; i < selected_skill->loop; i++)
            {
                selected_fighter_index = Computer_GetWeakestFighter(player);
                selected_fighter = player->team->fighters[selected_fighter_index];

                Fighter_Attack(computer_fighter, selected_fighter);
            }
            break;
        
        // Computer use skill on the fighter with least health;
        default:
            selected_fighter_index = Computer_GetWeakestFighter(player);
            selected_fighter = player->team->fighters[selected_fighter_index];

            Fighter_UseSkill(selected_skill, selected_fighter);
            break;
    }

    
}

void Computer_Attack(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty)
{   
    //
    int player_fighter_index;
    Fighter* computer_fighter = computer->team->fighters[fighter_index];
    Fighter* player_fighter = NULL;

    if (difficulty == BATTLE_DIFFICULTY_NOOB)
    {
        // Get random player fighter
        player_fighter_index = Computer_GetRandomPlayerFighter(player);
        player_fighter = player->team->fighters[player_fighter_index];
        
        // Attack player
        Fighter_Attack(computer_fighter, player_fighter);
    }
    else if (difficulty == BATTLE_DIFFICULTY_EASY)
    {
        // Get fighter with least health
        player_fighter_index = Computer_GetWeakestFighter(player);
        player_fighter = player->team->fighters[player_fighter_index];
        
        // Attack player
        Fighter_Attack(computer_fighter, player_fighter);
    }
    else if (difficulty == BATTLE_DIFFICULTY_NORMAL)
    {
        // Choice = 0 : Use skill
        // Choice = 1 : Normal attack
        int choice = rand() % 2;

        if (!Fighter_CheckAllSkillsAreLocked(computer_fighter) && choice == 0)
            Computer_UseRandomSkill(computer, fighter_index, player);
        else
        {
            int weakest_fighter_index = Computer_GetWeakestFighter(player);
            player_fighter = player->team->fighters[weakest_fighter_index];

            Fighter_Attack(computer_fighter, player_fighter);
        }       
    }

    
}

void Computer_Update(Player* computer, int fighter_index, Player* player, enum BATTLE_DIFFICULTY difficulty)
{
    Computer_Attack(computer, fighter_index, player, difficulty);

    // Check if player team is defeated
    Player_IsDefeated(player);
}