#ifndef GAME_H
#define GAME_H

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "game_entity.h"
#include "texture.h"
#include "map.h"
#include "player.h"

class game
{
    public:
        game(); //cstor
        ~game(); //dstor

        bool init( const char *title, int xpos, int ypos, int width, int height, Uint32 flags ); //Sets up loading functions

        /****************************************
         * Functions that must be made first
         ****************************************
         */
        void handleEvents();
        void loadCoreData();
        void update();
        void render(); //Displays
        void close(); //Closes textures, images, subsystems and windows

        /****************************************
         * Functions that have low priority
         ****************************************
         */
        void save();
        void load();
        void onOptionsChange();

        bool running() {return isRunning;}

        static SDL_Renderer* renderer;

    private:
        //const Uint8* keyState = SDL_GetKeyboardState(NULL);
        SDL_Event* event;

        bool isRunning;
        SDL_Window* window;
        int c = 0;

        int* plyrXPos = nullptr;
        int* plyrYPos = nullptr;
};

#endif // GAME_H
