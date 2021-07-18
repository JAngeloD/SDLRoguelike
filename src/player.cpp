#include "player.h"

int player::playerXPos = 0;
int player::playerYPos = 0;

player::player()
{
    velX = 0;
    velY = 0;
    diag = sqrt(pow(maxVelX, 2) /2);
}

player::~player()
{

}

void player::handleEvent(SDL_Event& event){
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_UP: velY -= maxVelY; break;
            case SDLK_DOWN: velY += maxVelY; break;
            case SDLK_LEFT: velX -= maxVelX; break;
            case SDLK_RIGHT: velX += maxVelX; break;
        }
    }
    else if ( event.type == SDL_KEYUP && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_UP: velY = 0; break;
            case SDLK_DOWN: velY = 0; break;
            case SDLK_LEFT: velX = 0; break;
            case SDLK_RIGHT: velX = 0; break;
        }
    }
}

void player::move() {
    if (velX != 0 && velY != 0) {
        velX = (velX / abs(velX)) * diag;
        velY = (velY / abs(velY)) * diag;
    }

    playerXPos += velX;
    playerYPos += velY;
}

