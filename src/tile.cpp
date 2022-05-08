#include "tile.h"

tile::tile() {}

tile::tile(int xPos, int yPos, int textureID, bool passable, map& gameMap)
{
    tileArea = texture::tileArea;
    this->xpos = xPos;
    this->ypos = yPos;
    this->passable = passable;
    this->terrain = textureID;
    this->gameMap = &gameMap;
}

tile::~tile() {}

void tile::drawTile(int x, int y, texture* t) {
    //Draws terrain
    t->render(x * tileArea, y * tileArea, terrain);

    //Draws item
    if(titem != nullptr) {
        t->render(x * tileArea, y * tileArea, titem->getTexture());
    }

    //Draws entity
    if(tentity != nullptr) {
        t->render(x * tileArea, y * tileArea, tentity->getTexture());
    }
}

void tile::assignItem(item* target) {

    this->titem = target;

    //Syncs the item on this tile
    this->titem->owner = this;
    this->titem->setXPos(this->xpos);
    this->titem->setYPos(this->ypos);
}

void tile::assignEntity(entity* target) {

    //If the tile cannot be passed will not execute
    if(!this->isPassable()) {
        return;
    }

    //When moving the target entity the entity is removed from the current tile to avoid duplicate
    //rendering of one entity
    if(target->owner != nullptr) {
        target->owner->removeEntity();
    }

    this->tentity = target;
    sync();
}

void tile::sync() {
    this->tentity->owner = this;
    this->tentity->setXPos(this->xpos);
    this->tentity->setYPos(this->ypos);

}

void tile::freeEntity() {
    this->tentity = nullptr;
}

void tile::free() {
    this->tentity = nullptr;
    this->titem = nullptr;
    this->tprop = nullptr;
}
