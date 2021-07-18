#ifndef PLAYER_H
#define PLAYER_H

#include "tgmath.h"
#include "texture.h"
#include "game.h"

class player
{
    public:
        player();
        ~player();

        void handleEvent(SDL_Event& event);
        void move();

        int* getPlayerXPos() {return &playerXPos;}
        int* getPlayerYPos() {return &playerYPos;}

    private:
        const int maxVelX = 5;
        const int maxVelY = 5;

        int velX;
        int velY;
        int diag;

        static int playerXPos;
        static int playerYPos;





};

#endif // PLAYER_H
