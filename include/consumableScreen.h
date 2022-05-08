#ifndef CONSUMABLESCREEN_H
#define CONSUMABLESCREEN_H

#include "consumables.h"
#include "player.h"

class consumableScreen
{
    public:
        consumableScreen();
        ~consumableScreen();

        void loadConsumableScreen();

        void updateConsumableScreen();

        void draw(SDL_Rect interfaceArea);


    protected:

    private:
};

#endif // CONSUMABLESCREEN_H
