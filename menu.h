#ifndef MENU_H
#define MENU_H

enum MENU_OPTIONS { MENU_OPTIONS_QUICK_BATTLE, MENU_OPTIONS_BATTLE, MENU_OPTIONS_SIZE};

typedef struct 
{
    enum MENU_OPTIONS selected;

    char* options[MENU_OPTIONS_SIZE];

} Menu;

// Allocate memory
Menu* Menu_Init();

// User choose their option in the menu
void Menu_ChooseOptions(Menu* menu); 

// Update state of menu
void Menu_Update(Menu* menu);

// Render menu on screen
void Menu_Render(Menu* menu);

// Free memory
void Menu_Free(Menu* menu);

#endif