#include "battle.h"

#include <stdlib.h>

#include "computer.h"

// Constructor //

Battle* Battle_Init()
{
    Battle* battle = malloc(sizeof(Battle));

    battle->is_over = false;

    battle->is_quickBattle = false;

    return battle;
}

enum BATTLE_AGAINST_OPTION Battle_ChooseAgainst(Battle* battle)
{
    clear();
    refresh();

    char* choices[BATTLE_AGAINST_OPTION_SIZE];
    choices[0] = "Against Player\0";
    choices[1] = "Against Computer\0";

    int choice;
    int input = -1, position = 0;

    do {

        // Render the options
        for (int i = 0; i < BATTLE_AGAINST_OPTION_SIZE; i++)
            mvprintw(i * 2, 0, "%s", choices[i]);

        if (choices[position] != NULL)
        {
            attron(A_REVERSE);
            mvprintw(position * 2, 0, "%s", choices[position]);
            attroff(A_REVERSE);
        }

        // User choose with the keypad
        choice = getch();
        switch (choice)
        {
            case KEY_UP:
                position = (position - 1 + BATTLE_AGAINST_OPTION_SIZE) % BATTLE_AGAINST_OPTION_SIZE;

                break;
            case KEY_DOWN:
                position = (position + 1) % BATTLE_AGAINST_OPTION_SIZE;   
                
                break;
            case 10:
                // Confirm choice by pressing Enter
                input = position;
                break;
            default:
                break;
        }
    } while (input < 0);

    return input;
}

enum BATTLE_DIFFICULTY Battle_ChooseComputerDifficutly(Battle* battle)
{
    clear();
    refresh();

    char* choices[BATTLE_DIFFICULTY_SIZE];
    choices[0] = "NOOB";
    choices[1] = "EASY";
    choices[2] = "NORMAL";

    int choice;
    int input = -1, position = 0;

    do {

        // Render the options
        for (int i = 0; i < BATTLE_DIFFICULTY_SIZE; i++)
            mvprintw(i * 2, 0, "%s", choices[i]);

        // Highlight the current choosen option
        if (choices[position] != NULL)
        {
            attron(A_REVERSE);
            mvprintw(position * 2, 0, "%s", choices[position]);
            attroff(A_REVERSE);
        }

        // User choose with the keypad
        choice = getch();
        switch (choice)
        {
            case KEY_UP:
                position = (position - 1 + BATTLE_DIFFICULTY_SIZE) % BATTLE_DIFFICULTY_SIZE;

                break;
            case KEY_DOWN:
                position = (position + 1) % BATTLE_DIFFICULTY_SIZE;   
                
                break;
            case 10:
                // Confirm choice by pressing Enter
                input = position;
                break;
            default:
                break;
        }
    } while (input < 0);

    return input;
}

// Gameplay Functions //

void Battle_TakeTurn(Battle* battle, Player* player1, Player* player2)
{
    // Render both Teams
    Player_Render(player1);
    Player_Render(player2);

    if (!player1->isComputer)
    {
        // Player turn
        player1->isYourTurn = true;

        // Render player window after being resized
        Player_Render(player1);

        if (player1->isYourTurn)
        {
            // Each fighter gets their turn
            for (int i = 0; i < player1->team->fighters_count; i++)
            {
                if (!player1->team->fighters[i]->is_locked)
                {
                    // Break the loop if the opponent team has been defeated
                    if (player2->is_defeated)
                        break;

                    // Player control what to do
                    Player_Update(player1, i, player2);
                    
                    // Render both team
                    Player_Render(player1);
                    Player_Render(player2);
                }
            }
        }

        // End of player turn
        player1->isYourTurn = false;

        // Render player window after being resized
        Player_Render(player1);
    }
    
    else if (player1->isComputer)
    {
        // Each fighter gets their turn
        for (int i = 0; i < player1->team->fighters_count; i++)
        {
            if (!player1->team->fighters[i]->is_locked)
            {
                // Break the loop if the opponent team has been defeated
                if (player2->is_defeated)
                    break;

                // Computer control what to do
                Computer_Update(player1, i, player2, battle->difficulty);
                
                // Render both team
                Player_Render(player1);
                Player_Render(player2);
            }
        }
    }


    // TO DO : Add for loop to add skill damage
    /*for (int i = 0; i < 3; i++)
        Player_Update(player2, i, player1);

    Player_Render(player2);*/

    for (int i = 0; i < player1->team->fighters_count; i++)
        Player_Update(player1, i, player2);

    // Check if one of the player team is beaten
    if (player1->is_defeated || player2->is_defeated)
        battle->is_over = true;
}

void Battle_Update(Battle* battle, Player* player1, Player* player2)
{
    // Disable keyboard typing
    noecho();

    // Refresh Screen
    clear();
    refresh();

    // 
    while (!battle->is_over)
    {
        // Each player has it's turn
        Battle_TakeTurn(battle, player1, player2);
        Battle_TakeTurn(battle, player2, player1);
    }

    // Announce which player won
    Battle_AnnounceVictory(battle, player1, player2);

    // Enable keyboard typing
    echo();
}

void Battle_AnnounceVictory(Battle* battle, Player* player1, Player* player2)
{
    // Refresh Screen
    clear();
    refresh();

    if (player1->is_defeated)
        mvprintw(0,0, "%s", "Player 1 has been defeated, Player 2 won !");
    else if (player2->is_defeated)
        mvprintw(0,0, "%s", "Player 2 has been defeated, Player 1 won !");
}

// Destructor //

// Free memory
void Battle_Free(Battle* battle)
{
    free(battle);
    battle = NULL;
}