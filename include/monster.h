#ifndef MONSTER_H
#define MONSTER_H

#include <math.h>

#include "map.h"
#include "entity.h"
#include "item.h"

class monster : public entity
{
    public:
        monster(std::string monsterName, int maxHP, int maxMP, int texture, int baseAttack);
        ~monster();

        void handleEvents();

    private:
        std::string monsterName;

        int baseAttack;

        void pollAction();

        void moveToPlayer(int x, int y);
        void attack(int x, int y);
        void wander(); //Optional implement
        item* generateItem();

};

#endif // MONSTER_H
