#ifndef TILE_H
#define TILE_H

#include <stdio.h>
#include <iostream>

#include "map.h"
#include "texture.h"
#include "prop.h"
#include "item.h"
#include "entity.h"

class texture;
class map;

class tile
{
    public:
        tile();
        tile(int xPos, int yPos, int textureID, bool passable, map& gameMap);
        ~tile();

        int xpos;
        int ypos;
        int terrain;

        void sync();
        void free();
        void drawTile(int x, int y, texture* t);
        bool isPassable() {return passable;}

    public:
        void assignItem(item* target);
        void assignProp();
        void assignEntity(entity* target);

        void removeItem() {this->titem = nullptr;}
        void removeProp() {this->tprop = nullptr;}
        void removeEntity() {this->tentity = nullptr;}

        void freeEntity();

        map* gameMap = nullptr;
        item* titem = nullptr;
        prop* tprop = nullptr;
        entity* tentity = nullptr;

    private:
        //Note: xpos and ypos refers to the grid coordinates
        int tileArea;
        bool passable;
};

#endif // TILE_H
