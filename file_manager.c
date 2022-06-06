#include "file_manager.h"

#include <stdlib.h>
#include <string.h>

void File_Manager_GetFighters(Fighters_Database* fighters_database)
{
    FILE* file = fopen("data/fighters.data", "r");
    if (file == NULL)
    {
        clear();
        refresh();
        mvprintw(0, 0, "fighters.data couldn't be opened !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
        return;
    }

    int index = 0;
    char c;

    c = fgetc(file);

    while (c != EOF)
    {
        c = fgetc(file);

        c = File_Manager_GetFighterNames(fighters_database, file, c, index);

        // skip ','
        c = fgetc(file);
        
        index++;
    }

    //fclose(file);

    //mvprintw(1, 0, "%ld", ftell(file));
}

char File_Manager_GetFighterNames(Fighters_Database* fighter_database, FILE* file, char c, int index)
{
    char* attribute = malloc(sizeof(char) * 256);
    float float_value;

    // Skip "{"
    c = fgetc(file);

    c = File_Manager_Skip_Whitespace(file, c);

    while (c != EOF)
    {
        c = File_Manager_Skip_Whitespace(file, c);

        // Reset Attribute Name
        memset(attribute, 0, 256);
        // Read Attribute Name
        c = File_Manager_ReadString(file, c, attribute);

        // Skip ':'
        c = fgetc(file);
        c = File_Manager_Skip_Whitespace(file, c);

        if (strcmp(attribute, "name") == 0)
        {   
            char* fighter_name = malloc(sizeof(char) * 256);

            c = File_Manager_ReadString(file, c, fighter_name);

            fighter_database->fighters_name[index] = fighter_name;
            fighter_database->fighters_count++;
            //mvprintw(index * 2, 0, "%s", fighter_name);
            //getch();
        }
        else if (strcmp(attribute, "health") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &float_value);
        }     
        else if (strcmp(attribute, "attack") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &float_value);
        }    
        else if (strcmp(attribute, "defense") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &float_value);
        }
        else if (strcmp(attribute, "agility") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &float_value);
        }
        else if (strcmp(attribute, "speed") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &float_value);
        }
        else if (strcmp(attribute, "skills") == 0)
        {
            char* skill_name = malloc(sizeof(char) * 256);

            // Skip '['
            c = fgetc(file);    
            c = File_Manager_Skip_Whitespace(file, c);

            // Read skills array
            while (c != ']')
            {
                memset(skill_name, 0, 256);

                // Read Skill Name
                c = File_Manager_ReadString(file, c, skill_name);

                // Skip whitespaces
                c = fgetc(file);
                c = File_Manager_Skip_Whitespace(file, c);
            
                // Skip whitespaces
                c = File_Manager_Skip_Whitespace(file, c);

                if (c == ']')
                    break;
            }

            // Skip ']'
            c = fgetc(file);
        }

        c = File_Manager_Skip_Whitespace(file, c);

        if (c == '}')
            break;
    }
    
    // Skip '}'
    c = fgetc(file);

    return c;    
}

Fighter* File_Manager_GetFighter(char* fighter_name)
{
    FILE* file = fopen("data/fighters.data", "r");
    if (file == NULL)
    {
        clear();
        refresh();
        mvprintw(0, 0, "fighters.data couldn't be opened !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
        return NULL;
    }

    Fighter* fighter = Fighter_Init(fighter_name, NULL);

    char c;

    // Continue searching the file and stop when it's the correct fighter
    while (strcmp(fighter->name, fighter_name) != 0 && c != EOF)
    {
        c = fgetc(file);

        fighter = Fighter_Init(fighter_name, NULL);

        c = File_Manager_ReadFighterAttributes(file, c, fighter);

        // skip ','
        c = fgetc(file);
    }

    //fclose(file);

    // Return Error when the searched fighter isn't found
    if (strcmp(fighter->name, fighter_name) != 0)
    {   
        clear();
        refresh();
        mvprintw(0, 0, "%s has no been found !", fighter_name);
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
        return NULL;
    }

    return fighter;
}

Skill* File_Manager_GetSkill(char* skill_name)
{
    FILE* file = fopen("data/skills.data", "r");
    if (file == NULL)
    {
        clear();
        refresh();
        mvprintw(0, 0, "skills.data couldn't be opened !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
        return NULL;
    }

    Skill* skill = Skill_Init(skill_name, NULL);

    char c;

    while (strcmp(skill->name, skill_name) != 0 && c != EOF)
    {
        c = fgetc(file);

        skill = Skill_Init(skill_name, NULL);

        c = File_Manager_ReadSkillAttributes(file, c, skill);

        // skip ','
        c = fgetc(file);
    }

    //fclose(file);

    if (strcmp(skill->name, skill_name) != 0)
    {
        clear();
        refresh();
        mvprintw(0, 0, "%s has no been found !", skill_name);
        mvprintw(2, 0, "Press any key to exit\n");
        getch();
        return NULL;
    }

    return skill;
}

char File_Manager_ReadFighterAttributes(FILE* file, char c, Fighter* fighter)
{
    char* attribute = malloc(sizeof(char) * 256);

    // Skip "{"
    c = fgetc(file);

    c = File_Manager_Skip_Whitespace(file, c);

    while (c != EOF)
    {
        c = File_Manager_Skip_Whitespace(file, c);

        // Reset Attribute Name
        memset(attribute, 0, 256);
        // Read Attribute Name
        c = File_Manager_ReadString(file, c, attribute);

        // Skip ':'
        c = fgetc(file);
        c = File_Manager_Skip_Whitespace(file, c);

        if (strcmp(attribute, "name") == 0)
        {   
            memset(fighter->name, 0, sizeof(fighter->name));
            c = File_Manager_ReadString(file, c, fighter->name);
        }
        else if (strcmp(attribute, "health") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &fighter->max_health);
            fighter->current_health = fighter->max_health;
        }     
        else if (strcmp(attribute, "attack") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &fighter->attack);
        }    
        else if (strcmp(attribute, "defense") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &fighter->defense);
        }
        else if (strcmp(attribute, "agility") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &fighter->agility);
        }
        else if (strcmp(attribute, "speed") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &fighter->speed);
        }
        else if (strcmp(attribute, "skills") == 0)
        {
            char* skill_name = malloc(sizeof(char) * 256);

            // Skip '['
            c = fgetc(file);    
            c = File_Manager_Skip_Whitespace(file, c);

            // Read skills array
            while (c != ']')
            {
                memset(skill_name, 0, 256);

                // Read Skill Name
                c = File_Manager_ReadString(file, c, skill_name);

                // Skip whitespaces
                c = fgetc(file);
                c = File_Manager_Skip_Whitespace(file, c);

                Fighter_AddSkill(fighter, File_Manager_GetSkill(skill_name));
            
                // Skip whitespaces
                c = File_Manager_Skip_Whitespace(file, c);

                if (c == ']')
                    break;
            }

            // Skip ']'
            c = fgetc(file);
        }

        c = File_Manager_Skip_Whitespace(file, c);

        if (c == '}')
            break;
    }
    
    // Skip '}'
    c = fgetc(file);

    free(attribute);

    return c;
}

char File_Manager_ReadSkillAttributes(FILE* file, char c, Skill* skill)
{
    char* attribute = malloc(sizeof(char) * 256);

    // Skip "{"
    c = fgetc(file);

    c = File_Manager_Skip_Whitespace(file, c);

    while (c != EOF)
    {
        c = File_Manager_Skip_Whitespace(file, c);

        // Reset Attribute Name
        memset(attribute, 0, 256);
        // Read Attribute Name
        c = File_Manager_ReadString(file, c, attribute);

        // Skip ':'
        c = fgetc(file);
        c = File_Manager_Skip_Whitespace(file, c);

        if (strcmp(attribute, "name") == 0)
        {   
            memset(skill->name, 0, sizeof(skill->name));
            c = File_Manager_ReadString(file, c, skill->name);
        }
        else if (strcmp(attribute, "description") == 0)
        {
            memset(skill->description, 0, sizeof(skill->description));
            c = File_Manager_ReadString(file, c, skill->description);
        }
        else if (strcmp(attribute, "modifier") == 0)
        {
            char* modifier_name = malloc(sizeof(char) * 256);

            c = File_Manager_ReadString(file, c, modifier_name);

            File_Manager_ReadSkillModifier(modifier_name, skill);

            free(modifier_name);            
        }
        else if (strcmp(attribute, "attribute") == 0)
        {
            char* attribute_name = malloc(sizeof(char) * 256);

            c = File_Manager_ReadString(file, c, attribute_name);

            File_Manager_ReadSkillEffect(attribute_name, skill);

            free(attribute_name);
        }
        else if (strcmp(attribute, "value") == 0)
        {
            c = File_Manager_ReadFloat(file, c, &skill->value);
        }           
        else if (strcmp(attribute, "loop") == 0)
        {
            c = File_Manager_ReadInteger(file, c, &skill->loop);
        }
        else if (strcmp(attribute, "range") == 0)
        {
            c = File_Manager_ReadInteger(file, c, &skill->range);
        }           
        else if (strcmp(attribute, "cooldown") == 0)
        {
            c = File_Manager_ReadInteger(file, c, &skill->cooldown);
        }          
        else if (strcmp(attribute, "duration") == 0)
        {
            c = File_Manager_ReadInteger(file, c, &skill->duration);
        }

        c = File_Manager_Skip_Whitespace(file, c);

        if (c == '}')
            break;
    }

    // Skip '}'
    c = fgetc(file);

    free(attribute);

    return c;
}

void File_Manager_ReadSkillModifier(char* modifier, Skill* skill)
{
    if (strcmp(modifier, "increase") == 0)
        skill->modifier = SKILL_MODIFIER_INCREASE;
    else if (strcmp(modifier, "decrease") == 0)
        skill->modifier = SKILL_MODIFIER_DECREASE;
    else if (strcmp(modifier, "loop") == 0)
        skill->modifier = SKILL_MODIFIER_LOOP;
    else if (strcmp(modifier, "range") == 0)
        skill->modifier = SKILL_MODIFIER_RANGE;
    else if (strcmp(modifier, "set_effect") == 0)
        skill->modifier = SKILL_MODIFIER_SET_EFFECT;
    else
        skill->modifier = SKILL_MODIFIER_NONE;
}

void File_Manager_ReadSkillEffect(char* attribute, Skill* skill)
{
    if (strcmp(attribute, "health") == 0)
        skill->attribute = SKILL_ATTRIBUTE_HEALTH;
    else if (strcmp(attribute, "attack") == 0)
        skill->attribute = SKILL_ATTRIBUTE_ATTACK;
    else if (strcmp(attribute, "defense") == 0)
        skill->attribute = SKILL_ATTRIBUTE_DEFENSE;
    else if (strcmp(attribute, "agility") == 0)
        skill->attribute = SKILL_ATTRIBUTE_AGILITY;
    else if (strcmp(attribute, "speed") == 0)
        skill->attribute = SKILL_ATTRIBUTE_SPEED;
    else if (strcmp(attribute, "fire") == 0) 
        skill->attribute = SKILL_ATTRIBUTE_FIRE;        
    else if (strcmp(attribute, "frozen") == 0)
        skill->attribute = SKILL_ATTRIBUTE_FROZEN;
    else if (strcmp(attribute, "sleep") == 0)
        skill->attribute = SKILL_ATTRIBUTE_SLEEP;
    else
        skill->attribute = SKILL_ATTRIBUTE_NONE;
}

char File_Manager_ReadString(FILE* file, char c, char* str)
{
    // Skip '"'
    int i = 0;

    c = fgetc(file);
    // Read until '""
    while (c != '\"')
    {  
        str[i] = c;
        i++;
        c = fgetc(file);
    }

    str[i] = '\0';
    
    c = fgetc(file);

    return c;
}

char File_Manager_ReadFloat(FILE* file, char c, float* attribute)
{
    fseek(file, -1, SEEK_CUR);

    fscanf(file, "%f", attribute);

    c = fgetc(file);

    return c;
}

char File_Manager_ReadInteger(FILE* file, char c, int* attribute)
{
    fseek(file, -1, SEEK_CUR);

    fscanf(file, "%d", attribute);

    c = fgetc(file);

    return c;
}

char File_Manager_Skip_Whitespace(FILE* file, char c)
{
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t')
        c = fgetc(file);
    return c;
}

