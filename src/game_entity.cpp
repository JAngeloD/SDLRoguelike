#include "game_entity.h"

game_entity::game_entity()
{
    entityTexture = NULL;
    textureHeight = 0;
    textureWidth = 0;
}

game_entity::~game_entity()
{
    free();
}

void game_entity::load_texture(std::string texturePath, Uint8 r, Uint8 g, Uint8 b) {

    free();

    SDL_Surface* loadedSurface = IMG_Load( texturePath.c_str());

    if (loadedSurface == NULL) {

        printf("Did not load texture of :%s\n %s", texturePath.c_str(), SDL_GetError());

    }
    else {

        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, r, g, b));

        entityTexture = SDL_CreateTextureFromSurface(game::renderer, loadedSurface);

        if(entityTexture != NULL) {

            textureHeight = loadedSurface->h;
            textureWidth = loadedSurface->w;
                printf("H:%i\n W:%i",textureHeight, textureWidth);

            printf("Texture loaded");

        }
    }

    SDL_FreeSurface(loadedSurface);

}

void game_entity::render(int xpos, int ypos) {

    SDL_Rect renderpos = { xpos, ypos, textureWidth/5, textureHeight/5};
    SDL_RenderCopy(game::renderer, entityTexture, NULL, &renderpos);

}

void game_entity::free() {

	//Free texture if it exists
	if( entityTexture != NULL )
	{
		SDL_DestroyTexture( entityTexture );
		entityTexture = NULL;
		textureHeight = 0;
		textureWidth = 0;
	}

}
