#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "item.h"
#include "inventory.h"

class tile;

class entity
{
    public:
        entity();
        virtual ~entity();

        void free();

        //The tile the entity is on
        tile* owner;

        void setXPos(int x) {this->xPos = x;}
        void setYPos(int y) {this->yPos = y;}

        int* getXPos() {return &xPos;}
        int* getYPos() {return &yPos;}

        inventory getItems() {return items;}

        void setEntityName(std::string name) {this->entityName = name;}
        std::string getEntityName() {return entityName;}

        void setMaxHP(int health) {this->maxHP = health;}
        void setMaxMP(int mana) {this->maxMP = mana;}

        void setCurrentHP(int health) {this->curHP = health;}
        void setCurrentMP(int mana) {this->curMP = mana;}

        int& getMaxHP() {return maxHP;}
        int& getMaxMP() {return maxMP;}

        int& getCurrentHP() {return curHP;}
        int& getCurrentMP() {return curMP;}

        virtual int& getAttack() {return attack;}
        virtual int& getDefense() {return defense;}

        virtual void setAttack(int attack) {this->attack = attack;}
        virtual void setDefense(int defense) {this->defense = defense;}

        virtual void beDamaged(int damage) {curHP -= damage;}

        void setTexture(int textureID) {this->texture = textureID;}

        int getTexture() {return texture;}

    private:
        std::string entityName;
        inventory items;

        int texture;
        int xPos;
        int yPos;

        int maxHP;
        int maxMP;
        int curHP;
        int curMP;

        int attack;
        int defense;

};

#endif // ENTITY_H
