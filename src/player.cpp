#include "player.h"

player::player(messagelog& messageLog, playerlog& playerLog, int texture, int maxHP, int maxMP)
{

    this->owner = nullptr;
    this->messageLog = &messageLog;
    this->playerLog = &playerLog;

    //Sets players texture using texture id
    this->setTexture(texture);

    //Sets players HP
    this->setMaxHP(maxHP);
    this->setCurrentHP(maxHP);

    //Sets players MP
    this->setMaxMP(maxMP);
    this->setCurrentMP(maxMP);

    //Sets players attack
    this->setAttack(3);

    //Sets players defense
    this->setDefense(0);

    //Sets players username shown on the top of the player log
    this->playerLog->loadStaticStat("Player");

    this->playerLog->loadDynamicStat("HP",255, 0, 0, this->getCurrentHP(), this->getMaxHP());
    this->playerLog->loadDynamicStat("MP",0, 0, 255, this->getCurrentMP(), this->getMaxMP());
    this->playerLog->loadDynamicStat("ATK", 255, 255, 255, this->getAttack());
    this->playerLog->loadDynamicStat("DEF", 255, 255, 255, this->getDefense());

    turns = 0;
}

player::~player() {}

bool player::handleEvent(SDL_Event& event) {
    bool useTurn = false;
    if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                pollAction(this->owner->xpos, this->owner->ypos - 1);
                useTurn = true;
                break;
            case SDLK_DOWN:
                pollAction(this->owner->xpos, this->owner->ypos + 1);
                useTurn = true;
                break;
            case SDLK_LEFT:
                pollAction(this->owner->xpos - 1, this->owner->ypos);
                useTurn = true;
                break;
            case SDLK_RIGHT:
                pollAction(this->owner->xpos + 1, this->owner->ypos);
                useTurn = true;
                break;
        }
    }

    return useTurn;
}

void player::interact(int x, int y) {

}

void player::pickUp() {

    this->setCurrentHP(this->getCurrentHP() + this->owner->titem->getHealth());
    playerLog->updateDynamicValues("HP");

//    this->setMana(this->owner->titem->getMana());

    this->setAttack(this->getAttack() + this->owner->titem->getAttack());
    playerLog->updateDynamicValuesNoDeplete("ATK");

    this->setDefense(this->getDefense() + this->owner->titem->getDefense());
    playerLog->updateDynamicValuesNoDeplete("DEF");

    messageLog->addMessage("Picked up " + this->owner->titem->getItemName() + "!", 255,255,255);

    this->owner->removeItem();
}

void player::move(int x, int y) {
    this->owner->gameMap->world[y][x].assignEntity(this);
}

void player::attack(int x, int y) {
    entity* targetEntity = this->owner->gameMap->world[y][x].tentity;
    targetEntity->beDamaged(3);
    messageLog->addMessage("Hit " + targetEntity->getEntityName() + " for " + std::to_string(3) + " damage", 255, 255, 255);
    playerLog->updateDynamicValues("HP");
    playerLog->updateDynamicValues("MP");
}

void player::pollAction(int x, int y) {
    if (this->owner->gameMap->world[y][x].tentity != nullptr) {
        attack(x, y);
    }
    else if (this->owner->gameMap->world[y][x].tprop != nullptr) {
        interact(x, y);
    }
    else if (this->owner->gameMap->world[y][x].titem != nullptr) {
        move(x, y);
        pickUp();
    }
    else {
        move(x, y);
    }
}
