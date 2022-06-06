#include "fighter.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ncurses.h"

#include "commons.h"

//// Constructor ////

Fighter* Fighter_Init(char* name, Team_Interface* team_interface)
{
    Fighter* fighter = malloc(sizeof(Fighter));

    fighter->team_interface = team_interface;

    fighter->name = malloc(sizeof(char) * 16);

    fighter->status = FIGHTER_STATUS_NORMAL;

    for (int i = 0; i < SKILL_MAX_SIZE; i++)
        fighter->skills[i] = NULL;

    fighter->skill_count = 0;

    for (int i = 0; i < MAX_FIGHTER_STATUS; i++)
    {
        Fighter_Status* fighter_status = malloc(sizeof(Fighter_Status));
        
        fighter->fighter_status[i] = fighter_status;
        
        fighter->fighter_status[i]->current_status = FIGHTER_STATUS_NORMAL;
        fighter->fighter_status[i]->affected_skill = NULL;
    }

    fighter->is_selected = false;
    fighter->is_locked = false;

    return fighter;
}

void Fighter_AddSkill(Fighter* fighter, Skill* skill)
{
    // If figther exceed the skill count size, do nothing
    if (fighter->skill_count > SKILL_MAX_SIZE)
        return;

    // Add skill to fighter
    for (int i = 0; i < SKILL_MAX_SIZE; i++)
    {
        if (fighter->skills[i] == NULL)
        {
            fighter->skills[i] = skill;
            break;
        }
    }

    fighter->skill_count++;
}

//// Gameplay Functions ////

void Fighter_TakeDamage(Fighter* fighter, float damage)
{
    fighter->current_health -= damage;
    if (fighter->current_health <= 0)
    {
        fighter->current_health = 0;
        fighter->is_locked = true;
    }
}

void Fighter_Attack(Fighter* fighter, Fighter* opponent)
{
    float damage = fighter->attack - opponent-> defense;
    Fighter_TakeDamage(opponent, damage);
}

// Skills Functions //

bool Fighter_CheckAllSkillsAreLocked(Fighter* fighter)
{
    // Return true if the fighter doesn't have any skills
    if (fighter->skill_count == 0)
        return true;

    // Check if all skills are locked
    for (int i = 0; i < fighter->skill_count; i++)
    {
        if (fighter->skills[i] != NULL)
        {
            // If one skill isn't locked, return false
            if (!fighter->skills[i]->isLocked)
            {
                return false;
            }   
        }
    }

    // If all skills are locked return true
    return true;
}

void Fighter_UseSkill(Skill* skill, Fighter* fighter)
{
    // TO CHANGE
    // Change attributes of choosen fighter
    if (skill->modifier == SKILL_MODIFIER_INCREASE || skill->modifier == SKILL_MODIFIER_DECREASE)
    {    
        if (skill->modifier == SKILL_MODIFIER_INCREASE)
        {  
            Fighter_SetStatus(fighter, skill, FIGHTER_STATUS_STATS_INCREASED);
            skill->value = 1 + skill->value / 100;
        }
            
        else if (skill->modifier == SKILL_MODIFIER_DECREASE)
        {
            Fighter_SetStatus(fighter, skill, FIGHTER_STATUS_STATS_DECREASED);
            skill->value = 1 - skill->value / 100;
            if (skill->value >= 0)
                skill->value = 0.1;
        }

        switch (skill->attribute)
        {
            case SKILL_ATTRIBUTE_HEALTH:
                fighter->current_health *= skill->value;
                // if player current health exceed max health, restore to max health
                if (fighter->current_health > fighter->max_health)
                    fighter->current_health = fighter->max_health;
                break;                

            case SKILL_ATTRIBUTE_ATTACK:
                fighter->attack *= skill->value;
                break;

            case SKILL_ATTRIBUTE_DEFENSE:
                fighter->defense *= skill->value;
                break;

            case SKILL_ATTRIBUTE_AGILITY:
                fighter->agility *= skill->value;
                break;

            case SKILL_ATTRIBUTE_SPEED:
                fighter->speed *= skill->value;
                break;
            
            default:
                break;
        }
    }
    // Set Effect of choosen fighter
    else if (skill->modifier == SKILL_MODIFIER_SET_EFFECT)
    {
        switch (skill->attribute)
        {
            case SKILL_ATTRIBUTE_FIRE:
                Fighter_SetStatus(fighter, skill, FIGHTER_STATUS_ON_FIRE);
                break;
            case SKILL_ATTRIBUTE_FROZEN:
                Fighter_SetStatus(fighter, skill, FIGHTER_STATUS_FROZEN);
            default:
                break;
        }
    }

    // Update skill cooldown and duration
    skill->current_duration = skill->duration;
    skill->current_cooldown = skill->cooldown;

    // Lock skill
    skill->isLocked = true;
}

void Fighter_UpdateSkills(Fighter* fighter)
{
    // Update every skill of fighter
    for (int i = 0; i < fighter->skill_count; i++)
    {
        if (fighter->skills[i] != NULL)
            Skill_Update(fighter->skills[i]);
    }
}

// Status Functions //

void Fighter_SetStatus(Fighter* fighter, Skill* skill, enum FIGHTER_STATUS choosen_status)
{
    // Check every status of the fighter
    for (int i = 0; i < MAX_FIGHTER_STATUS; i++)
    {
        if (fighter->fighter_status[i] != NULL)
        {
            // Change normal status to affected status
            if (fighter->fighter_status[i]->current_status == FIGHTER_STATUS_NORMAL)
            {
                fighter->fighter_status[i]->current_status = choosen_status; // Change fighter status
                fighter->fighter_status[i]->affected_skill = skill; // Set skill affected to status
                return;
            }
        }
    }  
}

void Fighter_UpdateStatus(Fighter* fighter)
{
    // Check every status of the fighter
    for (int i = 0; i < MAX_FIGHTER_STATUS; i++)
    {
        if (fighter->fighter_status[i] != NULL)
        {
            // If the status isn't normal
            if (fighter->fighter_status[i]->current_status != FIGHTER_STATUS_NORMAL)
            {
                // TO DO
                Fighter_TakeStatusEffect(fighter, fighter->fighter_status[i]);

                // Decrease duration of status
                fighter->fighter_status[i]->affected_skill->current_duration--;

                // Reset status to normal if duration has finished
                if (fighter->fighter_status[i]->affected_skill->current_duration <= 0)
                {
                    // TO DO
                    Fighter_ResetStatus(fighter, fighter->fighter_status[i]);
                }   
            }
        }
    } 
}

void Fighter_TakeStatusEffect(Fighter* fighter, Fighter_Status* fighter_status)
{    
    // Take effect of current status of choosen fighter
    if (fighter_status->affected_skill->modifier == SKILL_MODIFIER_SET_EFFECT)
    {
        switch (fighter_status->affected_skill->attribute)
        {
            case SKILL_ATTRIBUTE_FIRE:
                Fighter_TakeDamage(fighter, fighter_status->affected_skill->value);
                break;
            default:
                break;
        }
    }
}

void Fighter_ResetStatus(Fighter* fighter, Fighter_Status* fighter_status)
{
    fighter_status->current_status = FIGHTER_STATUS_NORMAL;

    // TO DO : Add with <100% decrease
    //if (fighter_status->affected_skill->value == 0)

    switch (fighter_status->affected_skill->attribute)
    {
        case SKILL_ATTRIBUTE_ATTACK:
            fighter->attack /= fighter_status->affected_skill->value;
            break;

        case SKILL_ATTRIBUTE_DEFENSE:
            fighter->defense /= fighter_status->affected_skill->value;
            break;

        case SKILL_ATTRIBUTE_AGILITY:
            fighter->agility /= fighter_status->affected_skill->value;
            break;

        case SKILL_ATTRIBUTE_SPEED:
            fighter->speed /= fighter_status->affected_skill->value;
            break;

        default:
            break;
    }
}

//

void Fighter_Update(Fighter* fighter)
{
    if (fighter != NULL)
    {
        // Update skill (cooldown)
        Fighter_UpdateSkills(fighter);

        // Status Update
        Fighter_UpdateStatus(fighter);
    }
    else
        printError("Fighter couldn't be updated !");

}

//// Rendering Functions ////

void Fighter_DrawName(Fighter* fighter, int fighter_index)
{
    if (fighter->is_selected)
    {
        wattron(fighter->team_interface->window, A_REVERSE);
        mvwprintw(fighter->team_interface->window, 2, 3 + fighter_index * 16, "%s  |%d|", fighter->name, fighter_index + 1);
        wattroff(fighter->team_interface->window, A_REVERSE);
    }
    else if (fighter->is_locked)
    {
        wattron(fighter->team_interface->window, A_DIM);
        mvwprintw(fighter->team_interface->window, 2, 3 + fighter_index * 16, "%s  |\u2620|", fighter->name);
        wattroff(fighter->team_interface->window, A_DIM);  
    }
    else
        mvwprintw(fighter->team_interface->window, 2, 3 + fighter_index * 16, "%s  |%d|", fighter->name, fighter_index + 1);
}

void Fighter_DrawStatus(Fighter* fighter, int fighter_index)
{
    //char* status_drawing = malloc(sizeof(char)*256);
    if (!fighter->is_locked)
    {
        for (int i = 0; i < MAX_FIGHTER_STATUS; i++)
        {
            if (fighter->fighter_status[i] != NULL)
            {
                if (fighter->fighter_status[i]->current_status != FIGHTER_STATUS_NORMAL)
                {
                    char* status_drawing = malloc(sizeof(char)*16);

                    switch (fighter->fighter_status[i]->current_status)
                    {
                        case FIGHTER_STATUS_STATS_INCREASED:
                            status_drawing = "Up"; //"\u2744";
                            break;
                        case FIGHTER_STATUS_STATS_DECREASED:
                            status_drawing = "Down"; //"\u2744";
                            break;
                        case FIGHTER_STATUS_FROZEN:
                            status_drawing = "Frozen"; //"\u2744";
                            break;
                        case FIGHTER_STATUS_ON_FIRE:
                            status_drawing = "Fire"; //"\u1F525";
                        default:
                            break;
                    }

                    mvwprintw(fighter->team_interface->window, 3, 3 + fighter_index * 16, "(%s) (%d)", status_drawing, fighter->fighter_status[i]->affected_skill->current_duration);
                }
            }
        }
    }
    
}

void Fighter_DrawHealthBar(Fighter* fighter, int fighter_index)
{
    int barConvert = fighter->max_health / fighter->team_interface->maxBar;
	int barCount = fighter->current_health / barConvert;
	int emptyCount = fighter->team_interface->maxBar - barCount;

    char*  bar_drawing = malloc(sizeof(char) * barCount);
    memset(bar_drawing, '#', barCount);
    bar_drawing[barCount] = '\0';

    char* empty_drawing = malloc(sizeof(char) * emptyCount);
    memset(empty_drawing, ' ', emptyCount);
    empty_drawing[emptyCount] = '\0';

    mvwprintw(fighter->team_interface->window, 4, 2 + fighter_index * 16, "[%s%s]", bar_drawing, empty_drawing);
}

void Fighter_DrawTurnBar(Fighter* fighter, int fighter_index)
{
    // TO DO :

    mvwprintw(fighter->team_interface->window, 5, 2 + fighter_index * 16, "[]");
}

void Fighter_DrawSkills(Fighter* fighter)
{
    mvwprintw(fighter->team_interface->window, 7, 3, "%s", fighter->name);
    mvwprintw(fighter->team_interface->window, 8, 3, "TECHNIQUES SPECIALES");

    for (int i = 0; i < fighter->skill_count; i++)
        Skill_Render(fighter->skills[i], i);
}

void Fighter_Render(Fighter* fighter, int fighter_index)
{
    if (fighter != NULL)
    {
        // TO DO : Render turn bar
        Fighter_DrawName(fighter, fighter_index);
        Fighter_DrawStatus(fighter, fighter_index);
        Fighter_DrawHealthBar(fighter, fighter_index);
        //Fighter_DrawTurnBar(fighter, fighter_index);
    }
    else
        printError("Fighter couldn't be rendered !");
}

// Destructor //

void Fighter_Free(Fighter* fighter)
{
    for (int i = 0; i < SKILL_MAX_SIZE; i++)
    {
        if (fighter->skills[i] != NULL)
            Skill_Free(fighter->skills[i]);
    }

    free(fighter);
    fighter = NULL;
}