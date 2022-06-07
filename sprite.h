#ifndef SPRITE_H
#define SPRITE_H

#include "stdio.h"
#include "ncurses.h"

#define SPRITE_MAX_HEIGHT 128

// Sprite of fighter
typedef struct 
{
    int height;
    int width;

    char* data[SPRITE_MAX_HEIGHT]; 
} Sprite;

// Constructor //

// Allocate memory for sprite
Sprite* Sprite_Init();

// Load a sprite from a file
void Sprite_Load(Sprite* sprite, char* sprite_name);

// Get height and width of sprite
void Sprite_GetSizeFromFile(Sprite* sprite, FILE* file);

// Rendering Functions //

// Draw sprite of fighter on screen
void Sprite_Render(Sprite* sprite, WINDOW* win);

// Destructor //

void Sprite_Free(Sprite* sprite);

#endif