#include "message.h"

message::message() {}

message::~message() {}

void message::loadmessage(std::string text, SDL_Color textColor, TTF_Font* messageFont) {
    this->messageLength = text.length();
    this->text = text;
    SDL_Surface* tempSurface = TTF_RenderText_Solid(messageFont, text.c_str(), textColor);
    messageTexture = SDL_CreateTextureFromSurface(game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

void message::renderMessage(int x, int y, int w, int h) {
    SDL_Rect destRect = {x, y, w, h};
    SDL_RenderCopy(game::renderer, messageTexture, NULL, &destRect);
}
