#include "game.h"


map* gameMap;
texture* spriteSheetA;
player* mainPlayer;

game::game() {}
game::~game() {close();}

SDL_Renderer* game::renderer = nullptr;

bool game::init(const char *title, int xpos, int ypos, int width, int height, Uint32 flags) {

    bool initSuccess = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        //SDL has fucked up lmao
        printf("SDL Failed to load");

        initSuccess = false;
    }
    else {
        //SDL has successfully initialized

        //Tries to set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        //Sets application window
        window = SDL_CreateWindow( title , xpos, ypos , width, height, flags );
        if ( window == NULL) {
            printf("SDL_CreateWindow didn't work nigga %s/n", SDL_GetError());

            initSuccess = false;
        }
        else {

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL) {
                printf("SDL_CreateRenderer didn't work nigga %s/n", SDL_GetError());
                initSuccess = false;
            }
            else {

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                printf("SDL fully functional and initialized\n ");

                //Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )  //PS: Binary math
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}

                isRunning = true;
            }
        }

    }

    return initSuccess;
}

void game::handleEvents() {

    SDL_Event event;
    SDL_PollEvent (&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }

    //mainPlayer->handleEvent(event);

}

void game::loadCoreData() {

    gameMap = new map();

    spriteSheetA = new texture();
    spriteSheetA->load_spritesheet("assets/tilemap/colored_tilemap.png", 10, 14, 125, 89, 8, 1);

    //mainPlayer = new player();

}

void game::update() {

    plyrXPos = mainPlayer->getPlayerXPos();
    plyrYPos = mainPlayer->getPlayerYPos();
    //mainPlayer->move();

}

void game::render() {

    SDL_RenderClear(renderer);
    //spriteSheetA->render(*plyrXPos, *plyrYPos, 1, 7);
    gameMap->drawMap();
    SDL_RenderPresent(renderer);
}

void game::close() {

    printf("quitting...");
    //Free loaded images
    // *****************

	//Destroy window
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

}
