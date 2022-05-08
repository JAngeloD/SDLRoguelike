#include "map.h"
std::vector<std::vector<tile>> map::world;

//Monster node methods
monsterListNode::monsterListNode(std::string monsterName, int maxHP, int maxMP, int texture, int baseAttack) {
    this->monsterData = new monster(monsterName, maxHP, maxMP, texture, baseAttack);
    this->next = nullptr;
}

void monsterListNode::deleteMonsterData() {
    delete this->monsterData;
}

//Monster list methods
monsterList::monsterList() {
    size = 0;
    this->monsterHead = nullptr;
    this->monsterTail = nullptr;
}

void monsterList::addMonster(monsterListNode* newMonster) {

    //Empty List
    if(monsterHead == nullptr && monsterTail == nullptr) {
        monsterHead = newMonster;
        monsterTail = newMonster;
    }
    //End
    else {
        monsterTail->next = newMonster;
        monsterTail = newMonster;
    }

    size++;
}

void monsterList::deleteMonster(monsterListNode* target) {

    monsterListNode* currentNode = this->monsterHead;
    monsterListNode* previousNode = this->monsterHead;

    int index = 0;
    bool found = false;
    while(currentNode != nullptr && index <= this->size) {
        if(target == currentNode) {
            found = true;
            break;
        }
        index++;
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if(found) {
        //Beginning
        if(index == 0) {
            monsterListNode* tempNode = currentNode;
            monsterHead = nullptr;

            if(size > 1) {
                monsterHead = tempNode->next;
            }
            else {
                monsterTail = nullptr;
            }
        }
        //End
        else if(index == this->size - 1) {
            monsterListNode* tempNode = previousNode;
            monsterTail == nullptr;
            monsterTail = previousNode;
        }
        //Middle
        else {
            previousNode = currentNode->next;
            currentNode = nullptr;
        }
    }
}

//Map Methods
map::map(std::string mapPath, texture& sprite, player& mainPlayer)
{
    this->mapPath = mapPath.c_str();
    this->sprite = &sprite;

    tileTotalLength = 100;
    tileTotalHeight = 100;
    tileArea = texture::tileArea;

    monsters = new monsterList();
    this->mainPlayer = &mainPlayer; //Loads in main player

    loadMap();
}

map::~map() {destroyMap();}

void map::deleteMonsterListNode(monster& targetMonster) {
    monsterListNode* currentNode = monsters->monsterHead;
    monsterListNode* previousNode = monsters->monsterHead;

    bool targetFound = false;

    do {
        if(currentNode->monsterData == &targetMonster) {
            targetFound = true;
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    } while (!targetFound && currentNode != nullptr);

    if(targetFound) {
        previousNode->next = currentNode->next;
        currentNode->next = nullptr;
        currentNode->deleteMonsterData();
        monsters->deleteMonster(currentNode);
    }

}

void map::loadMap() {
    std::ifstream data(mapPath);
    std::string line;
    world.resize(tileTotalLength * tileTotalHeight); //TODO: Change to be dynamic
    int row = 0;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<int> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(std::stoi(cell));
        }

        for (int i = 0; i < (int)parsedRow.size(); i++) {
            int texture = parsedRow[i];

            //Checks if the tile is flagged for placing an entity
            bool isMonster = false;
            bool isPlayer = false;
            if (parsedRow[i] == 11) {
                isMonster = true;
                texture = 15;
            }
            else if (parsedRow[i] == 5) {
                isPlayer = true;
                texture = 15;
            }

            //Checks if the tile should be passable
            bool isPassable = true;
            if (!isMonster && !isPlayer) {
                if(parsedRow[i] != 15) {
                    isPassable = false;
                }
            }

            tile* tileRef = new tile(i, row, texture, isPassable, *this);
            world[row].push_back(*tileRef);

            if(isMonster) {
                monsterListNode* newMonster = new monsterListNode("Goblin", 12, 12, 11, 3);
                monsters->addMonster(newMonster);
                tileRef->assignEntity(newMonster->monsterData);
            }
            else if(isPlayer) {
                tileRef->assignEntity(mainPlayer);
            }
        }
        row++;
    }
    data.close();

    //Load main dijkstra system
    loadDijkstraMap(4);
    loadAttractor(*mainPlayer);
}

void map::draw(SDL_Rect windowArea){
    screenSizeW = windowArea.w;
    screenSizeH = windowArea.h;

    tileWidthNumRadius = ceil((double)screenSizeW/(tileArea * 2));
    tileHeightNumRadius = ceil((double)screenSizeH/(tileArea * 2));

    mapEdgeX = tileTotalLength - tileWidthNumRadius;
    mapEdgeY = tileTotalHeight - tileHeightNumRadius;

    cornerMapOffsetX = tileWidthNumRadius * 2;
    cornerMapOffsetY = tileHeightNumRadius * 2;

    //Order matters
    drawMap();
    drawPlayer();
}

void map::drawMap() {
    //Offset for the world index not window coordinates
    int offsetX = 0;
    int offsetY = 0;
    if (*mainPlayer->getXPos() > tileWidthNumRadius) {
        offsetX = *mainPlayer->getXPos() - tileWidthNumRadius;
        if (*mainPlayer->getXPos() > mapEdgeX) {
            offsetX = mapEdgeX - tileWidthNumRadius;
        }
    }
    if (*mainPlayer->getYPos() > tileHeightNumRadius) {
        offsetY = *mainPlayer->getYPos() - tileHeightNumRadius;
        if (*mainPlayer->getYPos() > mapEdgeY) {
            offsetY = mapEdgeY - tileHeightNumRadius;
        }
    }
    for(int y = 0; y < cornerMapOffsetY; y++)
        for (int x = 0; x < cornerMapOffsetX; x++) {
            world[offsetY + y][offsetX + x].drawTile(x, y, sprite);
    }
}

void map::drawPlayer() {
    int cameraX = tileWidthNumRadius;
    int cameraY = tileHeightNumRadius;
    if (*mainPlayer->getXPos() < cameraX) {
        cameraX = *mainPlayer->getXPos();
    }
    if(*mainPlayer->getXPos() > mapEdgeX) {
        cameraX = *mainPlayer->getXPos() - (mapEdgeX - tileWidthNumRadius);
    }
    if (*mainPlayer->getYPos() < cameraY) {
        cameraY = *mainPlayer->getYPos();
    }
    if(*mainPlayer->getYPos() > mapEdgeY) {
        cameraY = *mainPlayer->getYPos() - (mapEdgeY - tileHeightNumRadius);
    }
    sprite->render(cameraX * tileArea, cameraY * tileArea, 5);
}

void map::loadDijkstraMap(int defaultVal) {
    defaultDijkstraVal = defaultVal;
    defaultAttractorVal = 0;

    for(int i = 0; i < (int)world.size(); i++) {
            dijkstraGrid.push_back(defaultDijkstraVal);
    }
    dijkstraGrid.resize(tileTotalLength * tileTotalHeight);

}

void map::loadAttractor(entity& attractor) {
    attractors.push_back(&attractor);
}

void map::resetDijsktra() {
    for(int i = 0; i < (int)dijkstraGrid.size(); i++) {
        dijkstraGrid[i] = defaultDijkstraVal;
    }
}

void map::updateAttractors() {
    for (int i = 0; i < (int)attractors.size(); i++) {
        int x = *attractors[i]->getXPos();
        int y = *attractors[i]->getYPos();
        dijkstraGrid[(y*tileTotalLength) + x] = defaultAttractorVal;
    }
}

//TODO : Make "i", "k", "l" into proper variable names :(
void map::dijkstraUpdate() {
    resetDijsktra();
    updateAttractors();
    bool noChanges = false;
    while(!noChanges) {
        noChanges = true;
        for(int i = 0; i < (int)dijkstraGrid.size(); i++) {
            //Scans for nearby neighbors in both ordinal and cardinal directions of the "i" index
            //Tries to find the lowest number in said directions and puts the index of the number
            //into the var "lowestNeighborInt"

            int lowestNeighborIndex = getLowestNumNeighbor(i);
            if(!world[floor (i / tileTotalLength)][i % tileTotalHeight].isPassable()) {
                continue;
            }

            int lowestNeighborInt = dijkstraGrid[lowestNeighborIndex];
            if (lowestNeighborInt != dijkstraGrid[i]) {
                dijkstraGrid[i] = lowestNeighborInt + 1;
                noChanges = false;
            }
        }
    }

}

int map::getLowestNumNeighbor(int currentIndex) {
    int lowestPossibleNeighborIndex = currentIndex;
    for(int j = 0, k = 0, l = 0; j < 9; j++) {
        k = (j % 3 == 0) ? 0 : k + 1;
        l = (j % 3 == 0 && j != 0)? l + 1 : l;
        int possibleNeighbor = (currentIndex - (tileTotalLength + 1)) + ((l * tileTotalLength) + k);

        if (possibleNeighbor >= 0
            && possibleNeighbor < (int)dijkstraGrid.size() //Has to be within the bounds of the array
            && possibleNeighbor != currentIndex //Skips current currentIndex (Compass rose middle)
            ) {

            if (!(currentIndex % tileTotalLength == 0 && (possibleNeighbor + 1) % tileTotalLength == 0) && //If it's on the left most side of the currentIndex
                !((currentIndex + 1) % tileTotalLength == 0 && possibleNeighbor % tileTotalLength == 0)) { //If it's on the right most side of the currentIndex
                if (dijkstraGrid[currentIndex] > dijkstraGrid[possibleNeighbor] + 1
                    && dijkstraGrid[possibleNeighbor] < dijkstraGrid[lowestPossibleNeighborIndex]) {
                    lowestPossibleNeighborIndex = possibleNeighbor;
                }
            }
        }
    }

    return lowestPossibleNeighborIndex;
}

int map::getLowestNumNeighborDev(int currentIndex, int xpos, int ypos) {
    int lowestPossibleNeighborIndex = currentIndex;
    for(int j = 0, k = 0, l = 0; j < 9; j++) {
        k = (j % 3 == 0) ? 0 : k + 1;
        l = (j % 3 == 0 && j != 0)? l + 1 : l;
        int possibleNeighbor = (currentIndex - (tileTotalLength + 1)) + ((l * tileTotalLength) + k);
        if (possibleNeighbor >= 0
            && possibleNeighbor < (int)dijkstraGrid.size() //Has to be within the bounds of the array
            && possibleNeighbor != currentIndex
            ) { //Skips current currentIndex (Compass rose middle)

            if (!(currentIndex % tileTotalLength == 0 && (possibleNeighbor + 1) % tileTotalLength == 0) && //If it's on the left most side of the currentIndex
                !((currentIndex + 1) % tileTotalLength == 0 && possibleNeighbor % tileTotalLength == 0)) { //If it's on the right most side of the currentIndex

                if (dijkstraGrid[currentIndex] > dijkstraGrid[possibleNeighbor] && dijkstraGrid[possibleNeighbor] < dijkstraGrid[lowestPossibleNeighborIndex]) {
                    lowestPossibleNeighborIndex = possibleNeighbor;

                }
            }
        }
    }

    return lowestPossibleNeighborIndex;
}

void map::printDijkstraGrid() {
    for(int i = 0; i < (int)dijkstraGrid.size(); i++) {
        std::cout << dijkstraGrid[i] << ((i + 1) % tileTotalLength == 0? "\n" : "");
    }
}

void map::handleEvents(SDL_Event& event) {
    monsterListNode* n = monsters->monsterHead;

    while(n != nullptr) {
        n->monsterData->handleEvents();
        n = n->next;
    }

    dijkstraUpdate();
//    printDijkstraGrid();
}

void map::destroyMap() {
    world.clear();
    sprite->free();
    sprite = nullptr;
    mainPlayer = nullptr;
}

//void map::drawBSPmap() {
//    BSPDungeon* city = new BSPDungeon(100, 100, 3);
//    std::vector<Cell> cells = city->getLeaves();
//    std::vector<Room> corridors = city->getRooms();
//
//    //Draw Cell containers
//    for (int i = 0 ; i < (int)cells.size(); i++) {
//        printf("Cell %i\n", i+1);
//        printf("A: x= %i, y= %i\n", cells[i].vertex[0].first, cells[i].vertex[0].second);
//        printf("B: x= %i, y= %i\n", cells[i].vertex[1].first, cells[i].vertex[1].second);
//        printf("C: x= %i, y= %i\n", cells[i].vertex[2].first, cells[i].vertex[2].second);
//        printf("D: x= %i, y= %i\n", cells[i].vertex[3].first, cells[i].vertex[3].second);
//        for (int f = 0; f < 4; f++) {
//            //controls index number
//            int offset1 = (f % 2) ? 2 : 1;
//            int offset2 = (f > 1) ? 3 : 0;
//
//            SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
//            SDL_RenderDrawLine(game::renderer, cells[i].vertex[offset1].first + 100, cells[i].vertex[offset1].second + 100
//                                             , cells[i].vertex[offset2].first + 100, cells[i].vertex[offset2].second + 100);
//
//        }
//    }
//
//
//    printf("%i", cells.size());
//    //Draw rooms
//    for (int i = 0; i < (int)cells.size(); i++) {
//        SDL_Rect roomRect;
//        roomRect.x = cells[i].room.origin.first + 100;
//        roomRect.y = cells[i].room.origin.second + 100;
//        roomRect.h = cells[i].room.height;
//        roomRect.w = cells[i].room.width;
//
//        SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
//        if(SDL_RenderDrawRect(game::renderer, &roomRect) < 0) {
//            printf("Rect drawing failed");
//        }
//    }
//
//    //Draw corridors
//    for (int i = 0; i < (int)corridors.size(); i++) {
//        SDL_Rect corrRect;
//        corrRect.x = corridors[i].origin.first + 100;
//        corrRect.y = corridors[i].origin.second + 100;
//        corrRect.h = corridors[i].height;
//        corrRect.w = corridors[i].width;
//
//        SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
//        if (SDL_RenderFillRect(game::renderer, &corrRect) < 0 ) {
//            printf("corrRect drawing failed");
//        }
//    }
//}

