#ifndef PLAYERLOG_H
#define PLAYERLOG_H

#include <vector>
#include <string>

#include "player.h"
#include "message.h"

struct dynamicStat {
    message stat;
    std::string statName;
    SDL_Color textColor;
    int* currentVal;
    int* maxVal;
};

class playerlog
{
    public:
        playerlog();
        virtual ~playerlog();

        void loadPlayerLog(std::string fontPath);
        void updateDynamicValues(std::string statName);
        void updateDynamicValuesNoDeplete(std::string statName);

        void loadStaticStat(std::string name);
        void loadDynamicStat(std::string name, Uint8 r, Uint8 g, Uint8 b, int& currentVal, int& maxVal);
        void loadDynamicStat(std::string name, Uint8 r, Uint8 g, Uint8 b, int& currentVal);
        void draw(SDL_Rect interfaceArea);

    private:
        TTF_Font* messageFont;
        int textHeight = 32;
        int textWidth = 16;

        std::vector<dynamicStat> dynamicStatList;
        std::vector<message> fixedStatList;
};

#endif // PLAYERLOG_H
