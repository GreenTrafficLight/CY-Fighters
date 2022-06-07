#include "sprite.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ncurses.h"

#include "commons.h"

Sprite* Sprite_Init()
{
    Sprite* sprite = malloc(sizeof(Sprite));

    sprite->height = 0;
    sprite->width = 0;

    return sprite;
}

void Sprite_Load(Sprite* sprite, char* sprite_name)
{
    // Get path of sprite
    int path_size = strlen("data/sprites/") + strlen(sprite_name) + strlen(".sprite") + 1;

    char* path = malloc(sizeof(char) * path_size);
    if (path == NULL)
    {
        printError("Memory for path couldn't be allocated !");
        return;
    }
    
    strcat(path, "data/sprites/");
    strcat(path, sprite_name);
    strcat(path, ".sprite");

    path[strlen(path)] = '\0';

    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printError("Sprite couldn't be opened !");
        return;
    }
    
    // Get height and width of sprite in text
    Sprite_GetSizeFromFile(sprite, file);

    // Get back of the beginning of the file
    rewind(file);

    // Get sprite
    for (int height = 0; height < sprite->height; height++)
    {
                
        //char width[sprite->width + 1], drawing_width[sprite->width + 1];
        //fgets(width, sizeof(width + 1), file);
        //sscanf(width, "%[^\n]", drawing_width);
        //drawing_width[strlen(drawing_width)] = '\0';
        //sprite->data[height] = width;
        
        // Read whole line of sprite
        char* width = malloc(sizeof(char) * sprite->width + 1);
        char c;
        int j = 0;
        while ((c = fgetc(file)) != '\n')
        {
           width[j] = c;
           j++;
        }
        width[j] = '\0';
        
        sprite->data[height] = width;
    }

    // Cause errors for some reason
    //fclose(file);
}

void Sprite_GetSizeFromFile(Sprite* sprite, FILE* file)
{
    int current_line_size = 0;
    char c;

    while((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            sprite->height++;
            if (current_line_size > sprite->width)
                sprite->width = current_line_size;
            current_line_size = 0;
        }
        else
        {
            current_line_size++;
        }
        if (current_line_size > sprite->width)
            sprite->width = current_line_size;
    }
}

void Sprite_Render(Sprite* sprite, WINDOW* win)
{
    if (sprite != NULL)
    {
        wclear(win);
        // Each line of the sprite
        for (int height = 0; height < sprite->height; height++)
        {
            // Center the sprite in the window
            mvwprintw(win, (win->_maxy / 2) - (sprite->height / 2) + height, (win->_maxx / 2) - (sprite->width / 2), "%s", sprite->data[height]);
        }
        wrefresh(win);
    }
}

void Sprite_Free(Sprite* sprite)
{
    free(sprite);
    sprite = NULL;
}