#ifndef MAPGEN_H
#define MAPGEN_H

#include <stdio.h>
#include <fstream>
#include <algorithm>


class mapgen
{
    public:
        mapgen(int width, int height);
        ~mapgen();

        void mapInit();
        void perlinCreate(int mapWidth, int mapHeight, int octave, float frequency);

        float *outputPerlin = nullptr;
        float *noiseSeed = nullptr;

    private:
        int mapWidth;
        int mapHeight;




};

#endif // MAPGEN_H
