#ifndef MENU_H
#define MENU_H

enum MENU_OPTIONS { MENU_OPTIONS_QUICK_BATTLE, MENU_OPTIONS_BATTLE, MENU_OPTIONS_SIZE};

typedef struct 
{
    enum MENU_OPTIONS selected;

    char* options[MENU_OPTIONS_SIZE];

} Menu;

Menu* Menu_Init();

void Menu_Update(Menu* menu);

void Menu_Render(Menu* menu);

void Menu_Free(Menu* menu);

#endif