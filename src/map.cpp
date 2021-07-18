#include "map.h"

map::map()
{

}

map::~map()
{
    //dtor
}

void map::loadMap() {



}

void map::drawMap() {

    /*
    for ( int x = 0; x < 800; x++)
        for ( int y = 0; y < 600; y++) {

            //Increase number on divisor to increase
            double noise = (noiseMap->noise(10 * (x/(double)600), 10 * (y/(double)600), 0.8)) * 255;

            if (noise >= 200) {
                SDL_SetRenderDrawColor(game::renderer, 255, 255, 255, 255); //white
            }
            else if (noise >= 150 && noise < 210) {
                 SDL_SetRenderDrawColor(game::renderer, 107, 104, 98, 255); //gray
            }
            else if (noise >= 95 && noise < 150) {
                SDL_SetRenderDrawColor(game::renderer, 18, 148, 74, 255); //green
            }
            else if (noise >= 80 && noise < 95 ) {
                SDL_SetRenderDrawColor(game::renderer, 242, 209, 153, 255); //yellow
            }
            else if (noise < 80 ) {
                SDL_SetRenderDrawColor(game::renderer, 0, 213, 255, 255); //blue
            }

            SDL_RenderDrawPoint(game::renderer, x, y);
        };
    */


    BSPDungeon* city = new BSPDungeon(400, 400, 4);
    std::vector<Cell> cells = city->getLeaves();
    std::vector<Room> corridors = city->getRooms();

    //Draw Cell containers
    for (int i = 0 ; i < (int)cells.size(); i++) {
        for (int f = 0; f < 4; f++) {
            //controls index number
            int offset1 = (f % 2) ? 2 : 1;
            int offset2 = (f > 1) ? 3 : 0;


            SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(game::renderer, cells[i].vertex[offset1].first + 100, cells[i].vertex[offset1].second + 100
                                             , cells[i].vertex[offset2].first + 100, cells[i].vertex[offset2].second + 100);

        }
    }

    //Draw rooms
    for (int i = 0; i < (int)cells.size(); i++) {
        SDL_Rect roomRect;
        roomRect.x = cells[i].room.origin.first + 100;
        roomRect.y = cells[i].room.origin.second + 100;
        roomRect.h = cells[i].room.height;
        roomRect.w = cells[i].room.width;

        SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
        if(SDL_RenderDrawRect(game::renderer, &roomRect) < 0) {
            printf("Rect drawing failed");
        }
    }

    //Draw corridors
    for (int i = 0; i < (int)corridors.size(); i++) {
        SDL_Rect corrRect;
        corrRect.x = corridors[i].origin.first + 100;
        corrRect.y = corridors[i].origin.second + 100;
        corrRect.h = corridors[i].height;
        corrRect.w = corridors[i].width;

        SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
        if (SDL_RenderFillRect(game::renderer, &corrRect) < 0 ) {
            printf("corrRect drawing failed");
        } else {

        }
    }



}

/* IDK what this is lmao
 *
    void map::drawMap(float *perlinNoise) {

        float max = perlinNoise[0];
        for ( int i = 0; i < 360000; i++) {
            if (perlinNoise[i] > max) {
                max = perlinNoise[i];
            }
        }

        float scale = 255/max;

        for ( int x = 0; x < 600; x++) {
            for ( int y = 0; y < 600; y++)  {

                int colour;

                colour = (int)(perlinNoise[y * 600 + x] * scale);

                if (colour >= 200) {
                    SDL_SetRenderDrawColor(game::renderer, 255, 255, 255, 255); //white
                }
                else if (colour >= 180 && colour < 210) {
                     SDL_SetRenderDrawColor(game::renderer, 107, 104, 98, 255); //gray
                }
                else if (colour >= 120 && colour < 180) {
                    SDL_SetRenderDrawColor(game::renderer, 18, 148, 74, 255); //green
                }
                else if (colour >= 90 && colour < 120 ) {
                    SDL_SetRenderDrawColor(game::renderer, 242, 209, 153, 255); //yellow
                }
                else if (colour < 90 ) {
                    SDL_SetRenderDrawColor(game::renderer, 0, 213, 255, 255); //blue
                }

                SDL_RenderDrawPoint(game::renderer, x, y);

            }
        }

    }
*/
