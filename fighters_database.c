#include "fighters_database.h"

#include "ncurses.h"
#include <stdlib.h>

#include "file_manager.h"

Fighters_Database* Fighters_Database_Init()
{
    Fighters_Database* fighters_database = malloc(sizeof(Fighters_Database));

    File_Manager_GetFighters(fighters_database);

    return fighters_database;
}

char* Fighters_Database_ChooseFighter(Fighters_Database* fighters_database)
{
    int choice;
    int input = -1, position = 0;

    do {

        // Render the options
        Fighters_Database_Render(fighters_database);

        // Highlight the current choosen option
        if (fighters_database->fighters_name[position] != NULL)
        {
            attron(A_REVERSE);
            mvprintw(position * 2, 0, "%s", fighters_database->fighters_name[position]);
            attroff(A_REVERSE);
        }

        // User choose with the keypad which opponent fighter to use
        choice = getch();
        switch (choice)
        {
            case KEY_UP:
                position = (position - 1 + fighters_database->fighters_count) % fighters_database->fighters_count;

                break;
            case KEY_DOWN:
                position = (position + 1) % fighters_database->fighters_count;   
                
                break;
            case 10:
                // Confirm choice by pressing Enter
                input = position;
                break;
            default:
                break;
        }
    } while (input < 0);

    return fighters_database->fighters_name[position];
}

void Fighters_Database_CreateTeam(Fighters_Database* fighters_database, Team* team, bool randomize)
{
    char* fighter_name;

    // User choose their fighter
    if (!randomize)
    {
        for (int i = 0; i < TEAM_MAX_SIZE; i++)
        {
            fighter_name = Fighters_Database_ChooseFighter(fighters_database);

            Team_AddFighter(team, File_Manager_GetFighter(fighter_name));
        }
    }

    // User cannot choose their fighter, it's randomized (for Quick Battle)
    else if (randomize)
    {
        for (int i = 0; i < TEAM_MAX_SIZE; i++)
        {
            int random_index = rand() % fighters_database->fighters_count;

            fighter_name = fighters_database->fighters_name[random_index];

            Team_AddFighter(team, File_Manager_GetFighter(fighter_name));
        }
    }
}

void Fighters_Database_Update(Fighters_Database* fighters_database, Team* team, bool randomize)
{
    // Disable keyboard typing
    noecho();

    // Create team from fighters database
    Fighters_Database_CreateTeam(fighters_database, team, randomize);

    // Disable enable
    echo();

    clear();
    refresh();
}

void Fighters_Database_Render(Fighters_Database* fighters_database)
{
    clear();
    refresh();

    for (int i = 0; i < fighters_database->fighters_count; i++)
    {
        if (fighters_database->fighters_name[i] != NULL)
        {
            mvprintw(i * 2, 0, "%s", fighters_database->fighters_name[i]);
        }
    }
}