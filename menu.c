#include "menu.h"

#include "ncurses.h"

#include "team_interface.h"
#include "player.h"

#include <stdlib.h>

Menu* Menu_Init()
{
    Menu* menu = malloc(sizeof(Menu));

    menu->options[0] = "Quick Battle";
    menu->options[1] = "Battle";

    // Activate arrow keys for menu
    keypad(stdscr, TRUE);

    return menu;
}

int Menu_ChooseOption(Menu* menu)
{
    int choice;
    int input = -1, position = 0;



    do {

        Menu_Render(menu);

        // Highlight the current choosen option
        if (menu->options[position] != NULL)
        {
            attron(A_REVERSE);
            mvprintw(position * 2, 0, "%s", menu->options[position]);
            attroff(A_REVERSE);
        }

        // User choose with the keypad which opponent fighter to use
        choice = getch();
        switch (choice)
        {
            case KEY_UP:
                position = (position - 1 + MENU_OPTIONS_SIZE) % MENU_OPTIONS_SIZE;

                break;
            case KEY_DOWN:
                position = (position + 1) % MENU_OPTIONS_SIZE;   
                
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

void Menu_Update(Menu* menu)
{
    int menu_option;

    // Disable keyboard typing
    noecho();

    // Get selected option
    menu_option = Menu_ChooseOption(menu);
    menu->selected = menu_option;

    // Enable keyboard typing
    echo();
}

void Menu_Render(Menu* menu)
{
    // Render the options
    for (int i = 0; i < MENU_OPTIONS_SIZE; i++)
        mvprintw(i * 2, 0, "%s", menu->options[i]);
}

void Menu_Free(Menu* menu)
{
    free(menu);
    menu = NULL;
}