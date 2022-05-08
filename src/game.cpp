#include "game.h"
game::game() {}
game::~game() {}

SDL_Renderer* game::renderer = nullptr;
SDL_Window* game::window = nullptr;

texture* sprite = nullptr;
map* gameMap = nullptr;
BSPDungeon* mapGenerator = nullptr;
windowmanager* windowManager = nullptr;
messagelog* messageLog = nullptr;
playerlog* playerLog = nullptr;
player* mainPlayer = nullptr;

bool game::init(const char *title, int xpos, int ypos, int width, int height, Uint32 flags) {

    bool initSuccess = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
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

            //Sets renderer
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL) {
                printf("SDL_CreateRenderer didn't work nigga %s/n", SDL_GetError());
                initSuccess = false;
            }
            else {

                SDL_SetRenderDrawColor(renderer,34,35,35,255);
                printf("SDL fully functional and initialized\n");

                //Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )  //PS: Binary math
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				}

				if((TTF_Init() == -1)) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
				}
                SDL_SetWindowResizable(window, SDL_TRUE);
                isRunning = true;
            }
        }
    }

    return initSuccess;
}

void game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
    if (event.type == SDL_WINDOWEVENT) {
        switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                windowManager->resizeArea();
                break;
            default:
                break;
        }
    }

    if(mainPlayer->handleEvent(event)) {
        gameMap->handleEvents(event);
    }

}

void game::loadCoreData() {
    windowManager = new windowmanager();
    windowManager->load();

    sprite = new texture();
    sprite->load_spritesheet("assets/tilemap/colored_tilemap.png", 10, 14, 125, 89, 8, 1);

    messageLog = new messagelog();
    messageLog->loadInterface("assets/Fonts/manaspc.ttf", windowManager->getMessageLogScreenSize());

    playerLog = new playerlog();
    playerLog->loadPlayerLog("assets/Fonts/manaspc.ttf");

    mainPlayer = new player(*messageLog, *playerLog, 5, 32, 32);

    mapGenerator = new BSPDungeon(100, 100, 4);
    mapGenerator->writeDungeonToFile("TEST");

    gameMap = new map("saves/mapgen.txt", *sprite, *mainPlayer);

}

void game::update() {

}

void game::render() {
    SDL_RenderClear(renderer);
    gameMap->draw(windowManager->getMapScreenSize());
    windowManager->drawWindow();
    messageLog->draw(windowManager->getMessageLogScreenSize());
    playerLog->draw(windowManager->getPlayerDataScreenSize());
    SDL_SetRenderDrawColor(renderer,34,35,35,255); //Sets background color
    SDL_RenderPresent(renderer);
}

void game::close() {
    std::cout << "Quitting program..." << std::endl;

    //Destroy Interfaces
    messageLog->destroyInterface();

	//Destroy window and renderer
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}
