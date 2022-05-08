#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>

#include "tgmath.h"
#include "game.h"
#include "mapgen.h"
#include "texture.h"
#include "tile.h"
#include "prop.h"
#include "player.h"
#include "monster.h"
#include "messagelog.h"
#include "item.h"

class tile;
class monster;
class messagelog;
class player;

struct monsterListNode {
    public:
        monsterListNode(std::string monsterName, int maxHP, int maxMP, int texture, int baseAttack);
        monster* monsterData;
        monsterListNode* next;

        void deleteMonsterData();
        //See deleteMonsterListNode() for deleting a node

};

struct monsterList {
    public:
        monsterList();
        int size;
        monsterListNode* monsterHead;
        monsterListNode* monsterTail;


        void addMonster(monsterListNode* newMonster);
        void deleteMonster(monsterListNode* target);
};

class map
{
    public:
        map(std::string mapPath, texture& sprite, player& mainPlayer);
        ~map();

        void loadMap();
        void destroyMap();
        void handleEvents(SDL_Event& event);

        void deleteMonsterListNode(monster& targetMonster);

        void draw(SDL_Rect windowArea);
        void drawPlayer();
        void drawMap();

        void drawBSPmap();

    public:
        //2D vector for storing all tiles in the map
        static std::vector<std::vector<tile>> world;

        int getLowestNumNeighbor(int currentIndex);
        int getLowestNumNeighborDev(int currentIndex, int xpos, int ypos);
        int getTileTotalHeight() {return tileTotalHeight;}
        int getTileTotalLength() {return tileTotalLength;}
        void printDijkstraGrid();

    private :
        //Dijkstra Map related functions
        void loadDijkstraMap(int defaultVal);
        void resetDijsktra();
        void loadAttractor(entity& attractor);
        void updateAttractors();
        void dijkstraUpdate();

    private:
        //Absolute location of the map file is
        std::string mapPath;

        //Texture object to fetch textures off of
        texture* sprite;

        //Main Actor
        player* mainPlayer;

        //A list of monsters
        monsterList* monsters;

        //Size of Screen
        int screenSizeW;
        int screenSizeH;

        //Total amount of tiles
        int tileTotalLength;
        int tileTotalHeight;

        //Number of tiles from the player to the edge off the screen
        int tileWidthNumRadius;
        int tileHeightNumRadius;

        //Number used to calculate camera position
        int mapEdgeX;
        int mapEdgeY;

        //
        int cornerMapOffsetX;
        int cornerMapOffsetY;

        //Size of tiles
        int tileArea;

        //1D vector for storing all entities in the map
        std::vector<item> items;
        std::vector<prop> props;

        //Dijkstra map related variables
        std::vector<int> dijkstraGrid;
        std::vector<entity*> attractors;
        int defaultDijkstraVal;
        int defaultAttractorVal;

};

#endif // MAP_H
