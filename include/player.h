#ifndef PLAYER_H
#define PLAYER_H

#include "monster.h"
#include "map.h"
#include "entity.h"
#include "messagelog.h"
#include "playerlog.h"


class messagelog;
class playerlog;

class player : public entity
{
    public:
        player(messagelog& messageLog, playerlog& playerLog, int texture, int maxHP, int maxMP);
        ~player();

        bool handleEvent(SDL_Event& event);

    private:
        void pollAction(int x, int y);

        void interact(int x, int y);
        void attack(int x, int y);
        void pickUp();
        void move(int x, int y);

    private:
        messagelog* messageLog;
        playerlog* playerLog;
        playerlog* statLog;

        Uint8 turns;
};

#endif // PLAYER_H
