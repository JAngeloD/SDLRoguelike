#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cmath>

#include "game.h"
#include "mapgen.h"

class map
{
    public:
        map();
        ~map();

        void loadMap();
        void drawMap();

    private:
<<<<<<< Updated upstream
        std::vector<std::vector<int>> terrain;
        PerlinNoise noiseMap;

        //SDL_Rect tileArea;

=======
>>>>>>> Stashed changes

};

#endif // MAP_H
