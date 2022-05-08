#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "game.h"

class message
{
    public:
        message();
        virtual ~message();

        SDL_Texture* messageTexture;
        std::string text;
        int messageLength;

        void loadmessage(std::string text, SDL_Color textColor, TTF_Font* messageFont);
        void renderMessage(int x, int y, int w, int h);

    private:
};

#endif // MESSAGE_H
