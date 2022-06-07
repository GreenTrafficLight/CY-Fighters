#ifndef SKILL_H
#define SKILL_H

#include <stdbool.h>
#include "team_interface.h"

#define SKILL_MAX_SIZE 3

enum SKILL_MODIFIER 
{ 
    SKILL_MODIFIER_NONE,

    SKILL_MODIFIER_INCREASE, // ( i.e : Increase Health, Attack, Defense, etc. )
    SKILL_MODIFIER_DECREASE, // ( i.e : Decrease Health, Attack, Defense etc. )
    SKILL_MODIFIER_LOOP, // ( i.e : Attack multiple times )
    SKILL_MODIFIER_RANGE, // ( i.e : Attack multiple fighters in one attack )
    SKILL_MODIFIER_SET_EFFECT, // ( i.e : Froze, Fire)
};

enum SKILL_ATTRIBUTE
{
    SKILL_ATTRIBUTE_NONE,

    SKILL_ATTRIBUTE_HEALTH,
    SKILL_ATTRIBUTE_ATTACK,
    SKILL_ATTRIBUTE_DEFENSE,
    SKILL_ATTRIBUTE_AGILITY,
    SKILL_ATTRIBUTE_SPEED,

    SKILL_ATTRIBUTE_FIRE,
    SKILL_ATTRIBUTE_FROZEN,
    SKILL_ATTRIBUTE_SLEEP,
};

typedef struct {

    Team_Interface* team_interface;
    
    char* name;
    char* description;

    enum SKILL_MODIFIER modifier;
    enum SKILL_ATTRIBUTE attribute;
    float value; // (In percentage) For Increase, Decrease Modifier
    int range; // For Range Modifier
    int loop; // For Loop Modifier
    int cooldown;
    int duration;

    int current_cooldown;
    int current_duration;

    bool isLocked;

} Skill;

// Constructor //

// Allocate Memory
Skill* Skill_Init(Team_Interface* team_interface);

// Gameplay Functions //


void Skill_Update(Skill* skill);

// Rendering Functions //

// Draw name of skill
void Skill_DrawName(Skill* skill, int skill_index);

// Render skill on screen
void Skill_Render(Skill* skill, int skill_index);

// Destructor //

// Free Memory
void Skill_Free();

#endif