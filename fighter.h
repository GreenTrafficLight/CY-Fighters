#ifndef FIGHTER_H
#define FIGHTER_H

#include "skill.h"
#include "team_interface.h"

#include <stdbool.h>

#define MAX_FIGHTER_STATUS 16

enum FIGHTER_STATUS 
{ 
	FIGHTER_STATUS_NORMAL = 0,
	FIGHTER_STATUS_FROZEN,
	FIGHTER_STATUS_ON_FIRE,
	FIGHTER_STATUS_SLEEPING,
	FIGHTER_STATUS_STATS_DECREASED,
	FIGHTER_STATUS_STATS_INCREASED,
};

typedef struct 
{
	Skill* affected_skill;

	enum FIGHTER_STATUS current_status;

} Fighter_Status;

typedef struct 
{
    Team_Interface* team_interface;

	// Fighter Description
	char* name;

	// Fighter Attributes
	float max_health;
	float current_health;
	float attack;
	float defense;
	float agility;
	float speed;

	// Fighter skills
    Skill* skills[SKILL_MAX_SIZE];
	int skill_count;

	Fighter_Status* fighter_status[MAX_FIGHTER_STATUS];

	enum FIGHTER_STATUS status;

	bool is_selected;

	bool is_locked;

} Fighter;

//// Constructor ////

// Allocator memory for Fighter struct
Fighter* Fighter_Init(char* name, Team_Interface* team_interface);

// Add skill to fighter
void Fighter_AddSkill(Fighter* fighter, Skill* skill);

//// Gameplay Functions ////

// Attack Functions //

// Decrease health of fighter
void Fighter_TakeDamage(Fighter* fighter, float damage);

// Decrease opponent health if the player has choosen the "Normal Attack" option
void Fighter_Attack(Fighter* fighter, Fighter* opponent);

// Status Functions //

// Set stauts of the choosen fighter if a skill has been used
void Fighter_SetStatus(Fighter* fighter, Skill* skill, enum FIGHTER_STATUS fighter_status);

// Update status of the fighter at the end of player turn
void Fighter_UpdateStatus(Fighter* fighter);

// Add damage of the status to the fighter
void Fighter_TakeStatusEffect(Fighter* fighter, Fighter_Status* fighter_status);

// Reset status and stats of the fighter if the skill duration is finished
void Fighter_ResetStatus(Fighter* fighter, Fighter_Status* fighter_status);

// Skill Functions //

// Check if the skills of the fighter are all locked
bool Fighter_CheckAllSkillsAreLocked(Fighter* fighter);

// Use skill of fighter on a opponent
void Fighter_UseSkill(Skill* skill, Fighter* opponent);

// Update skills of the fighter at the end of player turn
void Fighter_UpdateSkills(Fighter* fighter);

// Update attributes of the fighter at the end of player turn
void Fighter_Update(Fighter* fighter);

//// Rendering Functions ////

// Draw fighter name in the team interface
void Fighter_DrawName(Fighter* fighter, int fighter_index);

// Draw fighter status in the team interface
void Fighter_DrawStatus(Fighter* fighter, int fighter_index);

// Draw fighter health bar in the team interface
void Fighter_DrawHealthBar(Fighter* fighter, int fighter_index);

// Draw fighter turn bar (TO DO)
void Fighter_DrawTurnBar(Fighter* fighter, int fighter_index);

// Draw current fighter skills in the team interface 
void Fighter_DrawSkills(Fighter* fighter);

// Render the fighters and it's attributes
void Fighter_Render(Fighter* fighter, int fighter_index);

//// Destructor ////

// Free memory
void Fighter_Free(Fighter* fighter);

#endif