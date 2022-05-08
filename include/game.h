#ifndef GAME_H
#define GAME_H

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "windowmanager.h"
#include "messagelog.h"
#include "playerlog.h"
#include "texture.h"
#include "map.h"
#include "mapgen.h"

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
        SDL_Window& getWindow() {return *window;}

        static SDL_Renderer* renderer;
        static SDL_Window* window;

    private:
        bool isRunning;


};

#endif // GAME_H
