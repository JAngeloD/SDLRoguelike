#ifndef ITEM_H
#define ITEM_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class tile;

class item
{
    public:
        item(std::string itemName, int texture);
       ~item();

        //Loads item stats;
        void loadStats(int health, int mana, int attack, int defense) { this->health = health;
                                                                        this->mana = mana;
                                                                        this->attack = attack;
                                                                        this->defense = defense;}
        //The tile the item is on
        tile* owner;

        void setXPos(int x) {this->xPos = x;}
        void setYPos(int y) {this->yPos = y;}

        int* getXPos() {return &xPos;}
        int* getYPos() {return &yPos;}

        std::string getItemName() {return itemname;}
        int getTexture() {return texture;}

        int getHealth() {return health;}
        int getMana() {return mana;}
        int getAttack() {return attack;}
        int getDefense() {return defense;}

    private:

        std::string itemname;
        int texture;
        int xPos;
        int yPos;

        int health;
        int mana;
        int attack;
        int defense;
};

#endif // ITEM_H
