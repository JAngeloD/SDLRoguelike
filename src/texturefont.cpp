#include "texturefont.h"

texturefont::texturefont()
{
    //ctor
}

texturefont::~texturefont()
{
    //dtor
}

void texturefont::renderText(std::string text, SDL_Color textcolor) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textcolor);



}
