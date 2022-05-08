#include "playerlog.h"

playerlog::playerlog() {}

playerlog::~playerlog() {}

void playerlog::loadPlayerLog(std::string fontPath) {
    messageFont = TTF_OpenFont( fontPath.c_str(), 32);
}

void playerlog::updateDynamicValues(std::string statName) {
    for (int i = 0 ; i < (int)dynamicStatList.size(); i++) {

        if (dynamicStatList[i].statName == statName) {
            struct message newMessage;

            statName = statName + ": " + std::to_string(*dynamicStatList[i].currentVal) + "/" + std::to_string(*dynamicStatList[i].maxVal);
            newMessage.loadmessage(statName, dynamicStatList[i].textColor, messageFont);

            dynamicStatList[i].stat = newMessage;
        }
    }
}

void playerlog::updateDynamicValuesNoDeplete(std::string statName) {
    for (int i = 0 ; i < (int)dynamicStatList.size(); i++) {

        if (dynamicStatList[i].statName == statName) {
            struct message newMessage;

            statName = statName + ": " + std::to_string(*dynamicStatList[i].currentVal);
            newMessage.loadmessage(statName, dynamicStatList[i].textColor, messageFont);

            dynamicStatList[i].stat = newMessage;
        }
    }
}

void playerlog::loadDynamicStat(std::string name, Uint8 r, Uint8 g, Uint8 b, int& currentVal, int& maxVal) {
    struct message newMessage;
    struct dynamicStat newDynamicStat;
    newDynamicStat.statName = name;


    SDL_Color newTextColor = {r, g, b, 255};

    name = name + ": " + std::to_string(currentVal) + "/" + std::to_string(maxVal);
    newMessage.loadmessage(name, newTextColor, messageFont);

    newDynamicStat.stat = newMessage;
    newDynamicStat.currentVal = &currentVal;
    newDynamicStat.maxVal = &maxVal;
    newDynamicStat.textColor = newTextColor;

    dynamicStatList.push_back(newDynamicStat);
}

void playerlog::loadDynamicStat(std::string name, Uint8 r, Uint8 g, Uint8 b, int& currentVal) {
    struct message newMessage;
    struct dynamicStat newDynamicStat;
    newDynamicStat.statName = name;


    SDL_Color newTextColor = {r, g, b, 255};

    name = name + ": " + std::to_string(currentVal);

    newMessage.loadmessage(name, newTextColor, messageFont);

    newDynamicStat.stat = newMessage;
    newDynamicStat.currentVal = &currentVal;
    newDynamicStat.textColor = newTextColor;

    dynamicStatList.push_back(newDynamicStat);
}

void playerlog::loadStaticStat(std::string name) {
    struct message newMessage;
    SDL_Color textColor = {255, 255, 255, 255}; //DEFAULT: WHITE CHANGE IF NEEDED
    newMessage.loadmessage(name, textColor, messageFont);
    fixedStatList.push_back(newMessage);
}

void playerlog::draw(SDL_Rect interfaceArea) {

    int indentation = interfaceArea.w / 16;
    this->textWidth = interfaceArea.w / 16;
    this->textHeight = interfaceArea.h / 8;

    int startingPoint = interfaceArea.y + (interfaceArea.h / 12);
    for (int i = 0; i < (int)fixedStatList.size(); i++) {
        fixedStatList[i].renderMessage(interfaceArea.x + indentation,
                                       startingPoint,
                                       fixedStatList[i].messageLength * textWidth,
                                       textHeight);
        startingPoint += textHeight + i;
    }

    for (int i = 0; i < (int)dynamicStatList.size(); i++) {
        dynamicStatList[i].stat.renderMessage(interfaceArea.x + indentation,
                                              startingPoint,
                                              dynamicStatList[i].stat.messageLength * textWidth,
                                              textHeight);
        startingPoint += textHeight + i;
    }
}
