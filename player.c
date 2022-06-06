#include "player.h"

#include <stdlib.h>
#include <string.h>

#include "ncurses.h"
#include "team.h"
#include "file_manager.h"

// Constructor

Player* Player_Init(bool isComputer, Team_Interface* team_interface)
{
    Player* player = malloc(sizeof(Player));

    player->isComputer = isComputer;

    player->isYourTurn = false;

    player->team_interface = team_interface;

    // Activate arroww keys for team interface window
    keypad(player->team_interface->window, true);

    // TO CHANGE
    Player_CreateTeam(player, isComputer);

    return player;
}

void Player_CreateTeam(Player* player, bool isComputer) // Add bool isComputer with randomization ?
{
    // Put fighter selection screen here

    player->team = Team_Init(player->team_interface);

    if (!isComputer)
    {
        // Show Cursor
        curs_set(1);

        clear();
        refresh();
        
        mvprintw(0, 0, "%s", "Enter your team name : ");
        // Erase line
        clrtoeol();
        getstr(player->team->name);
        
        // Hide Cursor
        curs_set(0);
    }
    else
        player->team->name = "Equipe 2";

}

// Gameplay Functions //

enum PLAYER_CHOICE Player_ChooseChoice(Player* player, int fighter_index)
{    
    // Refresh team window
    Team_Render(player->team);

    int choice;
    int input = -1, position = 0;
    bool skill_all_locked = false;

    // Get player fighter
    Fighter* player_fighter = player->team->fighters[fighter_index];

    // Print stats of player fighter
    mvwprintw(player->team_interface->window, 9, 2, "ATK : %d  DEF : %d  AG : %d", (int)player_fighter->attack, (int)player_fighter->defense, (int)player_fighter->agility);

    char* choices[PLAYER_CHOICE_SIZE];
    choices[0] = "Normal Attack";
    choices[1] = "Choose Skill";

    skill_all_locked = Fighter_CheckAllSkillsAreLocked(player_fighter);

    do {
        
        for (int i = 0; i < PLAYER_CHOICE_SIZE; i++)
        {   
            // Player can't choose the skill option if they are all locked or have none
            if (i == PLAYER_CHOICE_SKILL && skill_all_locked)
            {
                wattron(player->team_interface->window, A_DIM);
                mvwprintw(player->team_interface->window, i * 2 + 11, 5, "%s", choices[i]);
                wattroff(player->team_interface->window, A_DIM);
            }
            else
                mvwprintw(player->team_interface->window, i * 2 + 11, 5, "%s", choices[i]);
        }
            
        // Draw selected option as highlighted
        wattron(player->team_interface->window, A_REVERSE);
        mvwprintw(player->team_interface->window, position * 2 + 11, 5, "%s", choices[position]);
        wattroff(player->team_interface->window, A_REVERSE);

        // User choose with the arrow keys which option to use
        choice = wgetch(player->team_interface->window);
        switch (choice)
        {
            case KEY_UP:
                
                do {
                    position = (position - 1 + PLAYER_CHOICE_SIZE) % PLAYER_CHOICE_SIZE;     
                } while (skill_all_locked && position == PLAYER_CHOICE_SKILL);
                            
                break;
            case KEY_DOWN:
                
                do {
                    position = (position + 1) % PLAYER_CHOICE_SIZE; 
                } while (skill_all_locked && position == PLAYER_CHOICE_SKILL);
                
                break;
            case 10:
                input = position;
                break;
            default:
                break;
        }
    } while (input < 0);

    return input;
}

int Player_ChooseSkill(Player* player, int fighter_index)
{
    // Refresh team window
    Team_Render(player->team);

    int choice;
    int input = -1, position = 0;

    // Skip the first skills if they're locked
    while (player->team->fighters[fighter_index]->skills[position]->isLocked)
        position = (position + 1) % player->team->fighters[fighter_index]->skill_count;

    do {
        // Render current fighter skills
        Fighter_DrawSkills(player->team->fighters[fighter_index]);

        // Draw selected skill as highlighted
        if (player->team->fighters[fighter_index]->skills[position] != NULL)
        {
            wattron(player->team_interface->window, A_REVERSE);
            Skill_DrawName(player->team->fighters[fighter_index]->skills[position], position);
            wattroff(player->team_interface->window, A_REVERSE);
        }

        // User choose with the keypad which skill to use
        choice = wgetch(player->team_interface->window);
        switch (choice)
        {
            case KEY_UP:
                
                do {
                    position = (position - 1 + player->team->fighters[fighter_index]->skill_count) % player->team->fighters[fighter_index]->skill_count;                 
                } while (player->team->fighters[fighter_index]->skills[position]->isLocked); // Skip skills that are locked

                break;
            case KEY_DOWN:
                
                do {
                    position = (position + 1) % player->team->fighters[fighter_index]->skill_count; 
                } while (player->team->fighters[fighter_index]->skills[position]->isLocked); // Skip skills that are locked
                
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

int Player_ChooseFighter(Player* player, int fighter_index, Player* opponent)
{
    // Refresh team window
    Team_Render(player->team);

    int choice;
    int input = -1, position = 0;

    // Skip the first fighters if they're locked
    while (opponent->team->fighters[position]->is_locked)
        position = (position + 1) % opponent->team->fighters_count;

    do {

        // Render each opponent fighter name
        for (int i = 0; i < opponent->team->fighters_count; i++)
        {
            if (opponent->team->fighters[i] != NULL)
            {
                if (!opponent->team->fighters[i]->is_locked)
                    mvwprintw(player->team_interface->window, i * 2 + 11, 5, "%s, H : %d / %d", opponent->team->fighters[i]->name, (int)opponent->team->fighters[i]->current_health, (int)opponent->team->fighters[i]->max_health);
                else
                {
                    // Grey out if fighter is locked
                    wattron(player->team_interface->window, A_DIM);
                    mvwprintw(player->team_interface->window, i * 2 + 11, 5, "%s, H : %d / %d", opponent->team->fighters[i]->name, (int)opponent->team->fighters[i]->current_health, (int)opponent->team->fighters[i]->max_health);
                    wattroff(player->team_interface->window, A_DIM);
                }
            }
                
        }

        // Draw selected opponent fighter as highlighted
        if (opponent->team->fighters[position] != NULL)
        {
            wattron(player->team_interface->window, A_REVERSE);
            mvwprintw(player->team_interface->window, position * 2 + 11, 5, "%s", opponent->team->fighters[position]->name);
            wattroff(player->team_interface->window, A_REVERSE);
        }

        // User choose with the keypad which opponent fighter to use
        choice = wgetch(player->team_interface->window);
        switch (choice)
        {
            case KEY_UP:

                do {
                    position = (position - 1 + opponent->team->fighters_count) % opponent->team->fighters_count;                 
                } while (opponent->team->fighters[position]->is_locked); // Skip Fighters that are lockeds

                break;
            case KEY_DOWN:
                
                do {
                    position = (position + 1) % opponent->team->fighters_count;                 
                } while (opponent->team->fighters[position]->is_locked); // Skip Fighters that are locked

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
 
void Player_HandleInputs(Player* player, int fighter_index, Player* opponent)
{
    enum PLAYER_CHOICE choice;
    int selected_skill, selected_fighter;
    Fighter* choosen_fighter = NULL;

    Fighter* player_fighter = player->team->fighters[fighter_index];
    
    // Player choose bewteen "Normal Attack" and "Skills" (maybe more ?)
    choice = Player_ChooseChoice(player, fighter_index);
    
    // If the player has choosen to use the "Skill" option
    if (choice == PLAYER_CHOICE_SKILL && player_fighter->skill_count != 0)
    {
        selected_skill = Player_ChooseSkill(player, fighter_index);

        switch (player_fighter->skills[selected_skill]->modifier)
        {
            // Choose your own fighters
            case SKILL_MODIFIER_INCREASE:
                selected_fighter = Player_ChooseFighter(player, fighter_index, player);
                choosen_fighter = player->team->fighters[selected_fighter];
                break;
            
            // Choose your opponents fighters
            default:
                selected_fighter = Player_ChooseFighter(player, fighter_index, opponent);
                choosen_fighter = opponent->team->fighters[selected_fighter];
                break;
        }
    }
    // If the player has choosen to use the "Normal Attack" option
    else
    {
        selected_fighter = Player_ChooseFighter(player, fighter_index, opponent);
        choosen_fighter = opponent->team->fighters[selected_fighter];
    }


    if (choosen_fighter != NULL)
    {
        switch (choice)
        {
            case PLAYER_CHOICE_ATTACK:
                Fighter_Attack(player_fighter, choosen_fighter);
                break;
            case PLAYER_CHOICE_SKILL:
                Fighter_UseSkill(player_fighter->skills[selected_skill], choosen_fighter);
            
            default:
                break;
        }
    }
}

void Player_IsDefeated(Player* player)
{
    Team_isBeaten(player->team);
    if (player->team->is_locked)
        player->is_defeated = true;
}

void Player_Update(Player* player, int fighter_index, Player* opponent)
{
    if (player != NULL)
    {
        if (player->isYourTurn)
        {
            // Select current fighter
            player->team->fighters[fighter_index]->is_selected = true;

            // Choose what to do with this fighter
            Player_HandleInputs(player, fighter_index, opponent);
            
            // De-select current fighter
            player->team->fighters[fighter_index]->is_selected = false;

            // Check if opponent team is defeated
            Player_IsDefeated(opponent);
        }
        else
        {
            // Update each fighter at the end of player turn
            Fighter_Update(player->team->fighters[fighter_index]);
        }
    }
    else
    {
        clear();
        refresh();
        mvprintw(0, 0, "Player couldn't be updated !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch(); 
    }
}

// Rendering Functions

void Player_Render(Player* player)
{
    if (player != NULL)
    {
        // Scale up window if it's the player turn
        if (player->isYourTurn)
        {
            wclear(player->team_interface->window);
            wrefresh(player->team_interface->window);
            wresize(player->team_interface->window, 6 * 3 + 2, 50 + 2);
        }
        // Scale down window if it's not the player turn
        else if (!player->isYourTurn)
        {
            wclear(player->team_interface->window);
            wrefresh(player->team_interface->window);
            wresize(player->team_interface->window, 6 + 2, 50 + 2);
        }

        // Render team of player
        Team_Render(player->team);
    }
    else
    {
        clear();
        refresh();
        mvprintw(0, 0, "Player couldn't be rendered !");
        mvprintw(2, 0, "Press any key to exit\n");
        getch(); 
    }
}

// Destructor

// Free memory
void Player_Free(Player* player)
{
    Team_Interface_Free(player->team_interface);

    Team_Free(player->team);

    free(player);
    player = NULL;
}