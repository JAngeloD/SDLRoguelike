#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "game.h"

/**
 *
 */

class game_entity
{
    public:
        game_entity();
        ~game_entity();

        /**
         * @params
         */
        void load_texture(std::string texturePath,Uint8 r, Uint8 g, Uint8 b);
        void render(int xpos, int ypos);
        void free(); //not exit


        /****************************************
         * TBD functions
         ****************************************
         */
        void movement();


    protected:

    private:
        SDL_Texture* entityTexture;

        //Actual dimensions of the texture in the window;
        int textureHeight;
        int textureWidth;


    public:
        SDL_Texture* getTexture() {return entityTexture;}
};

#endif // GAME_ENTITY_H
