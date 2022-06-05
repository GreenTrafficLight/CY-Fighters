#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "fighters_database.h"
#include "fighter.h"
#include "skill.h"

void File_Manager_GetFighters(Fighters_Database* fighter_database);

char File_Manager_GetFighterNames(Fighters_Database* fighter_database, FILE* file, char c, int index);

// Read Fighter from file
Fighter* File_Manager_GetFighter(char* fighter_name);

// Read Skill from File
Skill* File_Manager_GetSkill(char* skill_name);

// Read fighter attributes from file
char File_Manager_ReadFighterAttributes(FILE* file, char c, Fighter* fighter);

// Read skill attributes from file
char File_Manager_ReadSkillAttributes(FILE* file, char c, Skill* skill);

// Read modifier string of skill attributes
void File_Manager_ReadSkillModifier(char* modifier, Skill* skill);

// Read attribute string of skill attributes
void File_Manager_ReadSkillEffect(char* attribute, Skill* skill);

// Read string in file
char File_Manager_ReadString(FILE* file, char c, char* str);

// Read float in file
char File_Manager_ReadFloat(FILE* file, char c, float* attribute);

// Read integer in file
char File_Manager_ReadInteger(FILE* file, char c, int* attribute);

// Skip whitespaces in file
char File_Manager_Skip_Whitespace(FILE* file, char c);

#endif