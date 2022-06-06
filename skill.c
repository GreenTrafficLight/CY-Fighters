#include "skill.h"

#include <stdlib.h>

//  Constructor

Skill* Skill_Init(char* name, Team_Interface* team_interface)
{
    Skill* skill = malloc(sizeof(Skill));

    skill->team_interface = team_interface;

    skill->name = malloc(sizeof(char) * 16);
    skill->description = malloc(sizeof(char) * 256);

    skill->modifier = SKILL_MODIFIER_NONE;
    skill->attribute = SKILL_ATTRIBUTE_NONE;

    skill->isLocked = false;

    skill->value = 0;
    skill->loop = 0;
    skill->range = 0;

    skill->cooldown = 0;
    skill->duration = 0;

    skill->current_cooldown = 0;
    skill->current_duration = 0;

    return skill;
}

// Gameplay Functions //

void Skill_Update(Skill* skill)
{   
    // Decrease cooldown of skill if it's locked
    if (skill->current_cooldown != 0 && skill->isLocked)
        skill->current_cooldown--;

    // Unlock skill if cooldown is finished
    if (skill->current_cooldown == 0)
        skill->isLocked = false;
}

// Rendering Functions //

void Skill_DrawName(Skill* skill, int skill_index)
{
    if (!skill->isLocked)
        mvwprintw(skill->team_interface->window, 10 + skill_index * 2, 6, "|%d| %s (%d) (%d)", skill_index + 1, skill->name, skill->duration, skill->cooldown);
    // Grey out skill name if the skill is locked
    else
    {
        wattron(skill->team_interface->window, A_DIM);
        mvwprintw(skill->team_interface->window, 10 + skill_index * 2, 6, "|-| %s (%d) (%d)", skill->name, skill->duration, skill->current_cooldown);
        wattroff(skill->team_interface->window, A_DIM);
    }
        
}

void Skill_DrawDescription(Skill* skill, int skill_index)
{
    if (!skill->isLocked)
        mvwprintw(skill->team_interface->window, 11 + skill_index * 2, 11, "%s", skill->description);
    // Grey out skill description if the skill is locked
    else
    {
        wattron(skill->team_interface->window, A_DIM);
        mvwprintw(skill->team_interface->window, 11 + skill_index * 2, 11, "%s", skill->description);
        wattroff(skill->team_interface->window, A_DIM);
    }
}

void Skill_Render(Skill* skill, int skill_index)
{
    if (skill != NULL)
    {
        Skill_DrawName(skill, skill_index);
        Skill_DrawDescription(skill, skill_index);
    }
    else 
    {
        clear();
        refresh();
        mvprintw(0, 0, "Skill couldn't be rendered !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
    }
}

// Destructor

void Skill_Free(Skill* skill)
{
    free(skill);
    skill = NULL;
}