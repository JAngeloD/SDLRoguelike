#include "windowmanager.h"


windowmanager::windowmanager() {}
windowmanager::~windowmanager() {}

void windowmanager::load() {
    resizeArea();
}

void windowmanager::resizeArea() {
    SDL_GetWindowSize(game::window, &windowSizeW, &windowSizeH);

    //Resize areas
    mapscreen = {0,0, windowSizeW, (int)(windowSizeH * 0.7)};
    playerdata = {0,mapscreen.h,(int)(windowSizeW * 0.3), windowSizeH - mapscreen.h};
    messagelog = {playerdata.w, playerdata.y, (int) windowSizeW - playerdata.w, playerdata.h};
}

void windowmanager::drawWindow() {
    //Sets background color for windows
    SDL_SetRenderDrawColor(game::renderer,34,35,35,255);
    SDL_RenderFillRect(game::renderer, &playerdata);
    SDL_RenderFillRect(game::renderer, &messagelog);

    //Sets outline color for windows
    SDL_SetRenderDrawColor(game::renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(game::renderer, &playerdata);
    SDL_RenderDrawRect(game::renderer, &messagelog);

}
