#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

/*
TO DO :
- FIX SKILLS
- PUT DURATION OF SKILL IN OPPONENT
- ADD TEAM UPDATE (?)
- FIX PLAYER 2 STARTING AT THE BEGINNING AND ENDING DIRECTLY
*/

int main()
{
    setlocale(LC_ALL, "");

    srand(time(NULL));

    initscr();
    cbreak();

    // Start the game
    Game* game = Game_Init();

    Game_Loop(game);

    // When the game is ended, free memory
    Game_Free(game);

    endwin();

    return 0;
}