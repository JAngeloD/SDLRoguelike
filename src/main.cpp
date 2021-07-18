/*************************************************************
 *  main.cpp
 *  GameEngine
 *
 *
 *  Created by Jeric Angelo De Vera 15/06/2021
 *  Copyright @2021 Jeric Angelo De Vera. All rights reserved
 */

#include "game.h"

game *Game = nullptr;

const int FPS = 60;
const int FRAMEDELAY = 1000/FPS;

Uint32 frameStart;
int frameTime;

int main( int argc, char* argv[]){

    Game = new game();
    Game->init("SDLRoguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    Game->loadCoreData();
    Game->render();

    while ( Game->running()) {

        frameStart = SDL_GetTicks();

        Game->handleEvents();
        Game->update();


        frameTime = SDL_GetTicks() - frameStart;

        if (FRAMEDELAY > frameTime) {

            SDL_Delay(FRAMEDELAY - frameTime);

        }
    }

    Game->close();

	return 0;
}
