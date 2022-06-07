#include "game.h"

#include <stdlib.h>

#include "ncurses.h"


#include "menu.h"
#include "battle.h"
#include "fighters_database.h"


#include "player.h"
#include "file_manager.h"

Game* Game_Init()
{
    Game* game = malloc(sizeof(Game));

    game->menu = Menu_Init();
    game->battle = Battle_Init();
    game->fighters_database = Fighters_Database_Init();

    game->quit = false;

    return game;
}

void Game_Loop(Game* game)
{
    //while (!game->quit)
    //{
        clear();
        refresh();

        Menu_Render(game->menu);

        Menu_Update(game->menu);

        if (game->menu->selected == MENU_OPTIONS_BATTLE || game->menu->selected == MENU_OPTIONS_QUICK_BATTLE)
        {
            if (game->menu->selected == MENU_OPTIONS_QUICK_BATTLE)
                game->battle->is_quickBattle = true;

            enum BATTLE_AGAINST_OPTION batte_against_option = Battle_ChooseAgainst(game->battle);

            // If the battle is against a computer, set computer difficulty
            if (batte_against_option == BATTLE_AGAINST_OPTION_COMPUTER)
            {
                enum BATTLE_DIFFICULTY batte_difficulty = Battle_ChooseComputerDifficutly(game->battle);

                game->battle->difficulty = batte_difficulty;
            }

            // Draw player 1 team interface at the left bottom screen
            Team_Interface* team_interface1 = Team_Interface_Init(6, 50, 0, 0);

            WINDOW* sprite_window1 = newwin(LINES - team_interface1->height * 3, team_interface1->width, 0, 0);

            // Draw player 2 team interface at the right bottom screen
            Team_Interface* team_interface2 = Team_Interface_Init(6, 50, 0, COLS - (50 + 2));

            WINDOW* sprite_window2 = newwin(LINES - team_interface2->height * 3, team_interface1->width, 0, team_interface2->x);
            
            // Initialize the two players

            // Always a controlled player
            Player* player1 = Player_Init(false, team_interface1, sprite_window1);

            // Choose fighter of player
            if (!game->battle->is_quickBattle)
                Fighters_Database_Update(game->fighters_database, player1->team, false);
            else if (game->battle->is_quickBattle)
                Fighters_Database_Update(game->fighters_database, player1->team, true);

            // A controlled or not player
            Player* player2;

            if (batte_against_option == BATTLE_AGAINST_OPTION_PLAYER)
                player2 = Player_Init(false, team_interface2, sprite_window2);
            else if (batte_against_option == BATTLE_AGAINST_OPTION_COMPUTER)
                player2 = Player_Init(true, team_interface2, sprite_window2);

            // Choose fighter of player
            if (!game->battle->is_quickBattle)
                Fighters_Database_Update(game->fighters_database, player2->team, false);
            else if (game->battle->is_quickBattle)
                Fighters_Database_Update(game->fighters_database, player2->team, true);
            
            // Main battle loop
            Battle_Update(game->battle, player1, player2);

            // Free memory
            Player_Free(player1);
            Player_Free(player2);
        }
    
        if (getch() == 'q')
            game->quit = true;
    //}

    //}



}

void Game_Free(Game* game)
{
    Menu_Free(game->menu);

    Battle_Free(game->battle);

    Fighters_Database_Free(game->fighters_database);

    free(game);
    game = NULL;
}