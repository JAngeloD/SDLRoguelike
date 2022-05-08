#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "map.h"
#include "game.h"

//Controls interface element's area
class windowmanager
{
    public:
        windowmanager();
        virtual ~windowmanager();

        void load();
        void resizeArea();
        void drawWindow();
        SDL_Rect& getMapScreenSize() {return mapscreen;}
        SDL_Rect& getPlayerDataScreenSize() {return playerdata;}
        SDL_Rect& getMessageLogScreenSize() {return messagelog;}
        SDL_Rect& getConsumableSlots() {return consumableSlots;}

    private:
        int windowSizeW;
        int windowSizeH;

        SDL_Rect mapscreen;
        SDL_Rect playerdata;
        SDL_Rect messagelog;
        SDL_Rect consumableSlots;
};

#endif // WINDOWMANAGER_H
