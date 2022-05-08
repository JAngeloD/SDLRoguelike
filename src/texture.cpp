#include "texture.h"
int texture::tileArea = 24;

texture::texture() {}

texture::~texture() {}

void texture::load_spritesheet(std::string sheetPath, int row, int column, int width, int height, int area, int gap) {

    free();

    SDL_Surface* loadedSurface = IMG_Load( sheetPath.c_str());

    if (loadedSurface == NULL) {

        printf("Did not load texture of :%s\n %s", sheetPath.c_str(), SDL_GetError());

    }
    else {

        //Color key image
        //SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 34, 35, 35));
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        sheet = SDL_CreateTextureFromSurface(game::renderer, loadedSurface);


        if(sheet != NULL) {

            sheetHeight = loadedSurface->h;
            sheetWidth = loadedSurface->w;

            printf("Sheet Texture: %s loaded\n", sheetPath.c_str());

        }
    }

    SDL_FreeSurface(loadedSurface);

    clip.resize(row * column);
    int xPtr = 0;
    int yPtr = 0;
    for (int y = 0, i = 0; y < row; y++) {
        if(y != 0)yPtr += gap;
        for (int x = 0; x < column; x++, i++) {

            clip[i].x = xPtr;
            clip[i].y = yPtr;
            clip[i].w = area;
            clip[i].h = area;

            xPtr += gap + area;
        }
        xPtr = 0;
        yPtr += area;
    }

    //for(int i=0; i<clip.size(); i++)
    //  printf("Texture %i = Xpos:%i, Ypos:%i \n", i, clip[i].x, clip[i].y);
}
void texture::render(int xpos, int ypos, int sprR, int sprC) {
    SDL_Rect clipRect = {clip[(sprR*14) - (14- sprC)-1].x, clip[(sprR*14) - (14- sprC)-1].y, clip[sprR* sprC].w, clip[sprR* sprC].h};
    SDL_Rect destRect = {xpos, ypos, tileArea, tileArea};
    SDL_RenderCopy(game::renderer, sheet, &clipRect, &destRect);
}

void texture::render(int xpos, int ypos, int id) {
    SDL_Rect clipRect = {clip[id].x, clip[id].y, clip[id].w, clip[id].h}; //Gets single sprite from spritesheet
    SDL_Rect destRect = {xpos, ypos, tileArea, tileArea};
    SDL_RenderCopy(game::renderer, sheet, &clipRect, &destRect);
}

void texture::free() {

	//Free texture if it exists
	if( sheet != NULL )
	{
		SDL_DestroyTexture(sheet);
		sheet = NULL;
		sheetHeight = 0;
		sheetWidth = 0;
		spritecnt = 0;
		clip.clear();
	}

}
