#ifndef TEXTUREFONT_H
#define TEXTUREFONT_H

#include <SDL_ttf.h>
#include <string>

class texturefont
{
    public:
        texturefont();
        virtual ~texturefont();

        void renderText(std::string text, SDL_Color textColor);
    private:
        TTF_Font* font;
};

#endif // TEXTUREFONT_H
