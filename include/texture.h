#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "game.h"

class texture
{
    public:
        texture();
        ~texture();

        void load_spritesheet(std::string sheetPath, int row, int column, int width, int height, int area, int gap);
        SDL_Texture* getSpriteSheet();
        void render(int xpos, int ypos, int spriteRow, int spriteColumn);
        void free();

    private:

        SDL_Texture* sheet = nullptr;
        int sheetHeight = 0;
        int sheetWidth = 0;

        std::vector<SDL_Rect> clip;
        int spritecnt = 0;

};

#endif // TEXTURE_H
