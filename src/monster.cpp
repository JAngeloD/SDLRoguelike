#include "monster.h"

monster::monster(std::string monsterName, int maxHP, int maxMP, int texture, int baseAttack)
{
    this->setEntityName(monsterName);
    this->owner = nullptr;
    this->setTexture(texture);

    this->setMaxHP(maxHP);
    this->setCurrentHP(maxHP);

    this->setMaxMP(maxMP);
    this->setCurrentMP(maxMP);

    this->baseAttack = baseAttack;
}

monster::~monster() {
    this->owner->tentity = nullptr;
}

void monster::handleEvents() {
    pollAction();

}

void monster::pollAction() {
    int currentIndex = (this->owner->ypos * 100) + (this->owner->xpos);
    int destination = this->owner->gameMap->getLowestNumNeighborDev(currentIndex, this->owner->xpos, this->owner->ypos);

    //X and Y are the coordinates of the destination not the current position of the monster
    int x = destination % 100;
    int y = (int)floor (destination / 100);

    if (this->getCurrentHP() > 0) {
        if (this->owner->gameMap->world[y][x].tentity != nullptr && destination != currentIndex) {
            attack(x, y);
        }
        else {
            moveToPlayer(x, y);
        }
    }
    else {
        item* newItem = generateItem();
        this->owner->assignItem(newItem);
        this->owner->gameMap->deleteMonsterListNode(*this);
    }

}

//NOTE: REFACTOR, LOOKS GARBO
void monster::moveToPlayer(int x, int y) {
    this->owner->gameMap->world[y][x].assignEntity(this);
}

void monster::attack(int x, int y) {
    this->owner->gameMap->world[y][x].tentity->beDamaged(baseAttack);
}

item* monster::generateItem() {

    item* newItem;

    int roll = rand() % 100 + 1;

    if(roll > 25) {
        newItem = new item("Health Potion", 119);
        newItem->loadStats(10, 0, 0, 0);
    }
    else {
        newItem = new item("Sword", 62);
        newItem->loadStats(0, 0, 1, 0);
    }

    return newItem;
}

void monster::wander() {

}
