#include "mapgen.h"

mapgen::mapgen(int width, int height)
{
    mapWidth = width;
    mapHeight = height;
}

mapgen::~mapgen()
{
    //dtor
}

void mapgen::mapInit() {
    noiseSeed = new float[mapWidth * mapHeight];
    outputPerlin = new float[mapWidth * mapHeight];

    for (int i = 0; i < mapWidth * mapHeight; i++) noiseSeed[i]  = (float)rand() / (float)RAND_MAX;
}

void mapgen::perlinCreate(int mapWidth, int mapHeight, int octave, float frequency) {


    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++) {

            float noise = 0.0f;
            float scaleAcc = 0.0f;
            float scale = 1.0f;

            for (int i = 0; i < octave; i++) {

                int pitch = mapWidth >> i;
                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;

                int sampleX2 = (sampleX1 + pitch) % mapWidth;
                int sampleY2 = (sampleY1 + pitch) % mapWidth;

                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;


                float sampleT = (1.0f - blendX) * noiseSeed[sampleY1 * mapWidth + sampleX1] + blendX * noiseSeed[sampleY1 * mapWidth + sampleX2];
                float sampleB = (1.0f - blendX) * noiseSeed[sampleY2 * mapWidth + sampleX1] + blendX * noiseSeed[sampleY2 * mapWidth + sampleX2];

                scaleAcc += scale;
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scale = scale / frequency;
            }

            outputPerlin[y * mapWidth + x] = noise / scaleAcc;

        }
}
