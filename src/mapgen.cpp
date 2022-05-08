#include "mapgen.h"

BSPDungeon::BSPDungeon(int width, int height, int iterations) {
    dungeonHeight = height;
    dungeonWidth = width;
    dungeonIterations = iterations;

    int val[3]{0, dungeonWidth, dungeonHeight};
    for (int i = 0; i < 4; i++) {
        root.vertex[i] = std::make_pair(val[(i % 2) ? 1 : 0], val[(i > 1) ? 2 : 0]);
    }
    leaves.push_back(root);

    for (int i = 0; i < dungeonIterations; i++) {
        split();
    }

    buildRooms();
    buildCorridors();
    mergeRoomsAndCorridors();
}

//NOTE "path" parameter not in use. Will change later
//path hardcoded to "saves/mapgen.txt"
void BSPDungeon::writeDungeonToFile(std::string path) {

    //Default ground texture
    int defaultTexture = 15;

    //Tile texture vector
    //Not not a 2D vector
    std::vector<int> dungeon((dungeonHeight * dungeonWidth), defaultTexture); //Used as a final vector to be written in the path parameter

    bool playerSpawned = false;
    int playerRoom = rand() % ((int)roomList.size() - 26)  + 26;
    for(int i = 0; i < (int)roomList.size(); i++) {
        std::vector<std::pair<int, int>> corners;

        ///IMPORTANT ORDER IS : A - B - D - C in corners vector
        //Reference to the other 4
        std::pair<int, int> A = roomList[i].origin; //Hash Coordinates
        dungeon[(A.second * dungeonWidth) + A.first] = 0; //Assigns tile texture.
        corners.push_back(A); //Pushes back into corners vector

        std::pair<int, int> B = std::make_pair(roomList[i].origin.first + roomList[i].width, roomList[i].origin.second);
        dungeon[(B.second * dungeonWidth) + B.first] = 3;
        corners.push_back(B);

        std::pair<int, int> D = std::make_pair(roomList[i].origin.first + roomList[i].width, roomList[i].origin.second + roomList[i].height);
        dungeon[(D.second * dungeonWidth) + D.first] = 31;
        corners.push_back(D);

        std::pair<int, int> C = std::make_pair(roomList[i].origin.first, roomList[i].origin.second + roomList[i].height);
        dungeon[(C.second * dungeonWidth) + C.first] = 28;
        corners.push_back(C);

        ///Flags spots where monsters will spawn
        ///Will also flag a room where the player will spawn where there will be no monsters
        ///Uses corners vector to decide, thus will not spawn on corridors
        //Determines if the room is a corridor
        //Exits the loop if it is
        int lengthDiff = corners[1].first - corners[0].first;
        int heightDiff = corners[3].second - corners[0].second;
        if(lengthDiff > 5 && heightDiff > 5) {

            //Makes a range for possible locations to spawn monsters (x and y)
            std::pair<int, int> xRange = std::make_pair(corners[0].first + 1, corners[1].first - 1);
            std::pair<int, int> yRange = std::make_pair(corners[1].second + 1, corners[3].second - 1);

            //Amount of monsters to be spawned
            int numOfMonsters = rand() % 3 + 1;

            //Assigns the monsters to a tile
            for (int k = 0; k < numOfMonsters; k++) {
                if(i == playerRoom) {
                    dungeon[((rand() % (yRange.second - yRange.first + 1) + yRange.first) * dungeonWidth) +
                            (rand() % (xRange.second - xRange.first + 1) + xRange.first)] = 5;
                    break;
                }
                dungeon[((rand() % (yRange.second - yRange.first + 1) + yRange.first) * dungeonWidth) +
                         (rand() % (xRange.second - xRange.first + 1) + xRange.first)] = 11;
            }

        }

        //Draws the whole room using the corners
        int textureID = defaultTexture; //Note this is a blank texture
        bool isHorizontal = true;
        for (int k = 0; k < (int)corners.size(); k++) {
            int nextCorner = (k + 1) == (int)corners.size() ? 0 : k + 1;

            //wall textures
            if(corners[nextCorner].first > corners[k].first || corners[nextCorner].first < corners[k].first) {
                textureID = 1;
                isHorizontal = true;
            }
            else if (corners[nextCorner].second > corners[k].second) {
                textureID = 17;
                isHorizontal = false;
            }
            else if (corners[nextCorner].second < corners[k].second) {
                textureID = 14;
                isHorizontal = false;
            }

            int previousIndex = 0;
            bool closeCorrdior = false;
            int targetIndex = 0;
            int sideSize = (isHorizontal) ? corners[nextCorner].first - corners[k].first : corners[nextCorner].second - corners[k].second;
            for (int j = 0, dynamicIterator = 0; j < std::abs(sideSize); j++, (sideSize > 0) ? dynamicIterator++ : dynamicIterator--) {
                if (j % std::abs(sideSize) != 0) {
                    //Horizontal walls
                    if (isHorizontal) {
                        targetIndex = (corners[k].second * dungeonWidth) + (corners[k].first + dynamicIterator);

                        if (dungeon[targetIndex] == defaultTexture) {
                            if (closeCorrdior) {
                                dungeon[previousIndex] = (k == 0) ? 42 : 45;
                                previousIndex = 0;
                                closeCorrdior = false;
                            }
                            dungeon[targetIndex] = textureID;

                        }
                        else {
                            if(previousIndex == 0) {
                                dungeon[targetIndex] = (k == 0) ? 43 : 44;
                            }
                            //Replace conflicts with the default texture
                            else {
                                dungeon[targetIndex] = defaultTexture;
                            }
                            previousIndex = targetIndex;
                            closeCorrdior = true;
                        }
                    }
                    //Vertical walls
                    else {
                        targetIndex = ((corners[k].second + dynamicIterator) * dungeonWidth) + corners[k].first;
                        if (dungeon[targetIndex] == defaultTexture) {
                            if (closeCorrdior) {
                                dungeon[previousIndex] = (k == 3) ? 43 : 44;
                                previousIndex = 0;
                                closeCorrdior = false;
                            }
                            dungeon[targetIndex] = textureID;
                        }
                        //Replace conflicts with the default texture
                        else {
                            if(previousIndex == 0) {
                                dungeon[targetIndex] = (k == 3) ? 45 : 42;
                            }
                            //Replace conflicts with the default texture
                            else {
                                dungeon[targetIndex] = defaultTexture;
                            }
                            previousIndex = targetIndex;
                            closeCorrdior = true;
                        }
                    }
                }

            }
        }
    }

    std::ofstream myfile;
    myfile.open ("saves/mapgen.txt", std::ofstream::out | std::ofstream::trunc);
    for(int i = 0; i < (int)dungeon.size(); i++) {
        std::string delimit = (i + 1) % dungeonWidth == 0 ? "\n" : ",";
        myfile << std::to_string(dungeon[i]) + delimit;
    }
    myfile.close();
}

//NOTE: Makes nodes in the dungeon NOT the actual rooms, see buildRooms
void BSPDungeon::split() {
    /** For Reference:
     *  A----------B
     *  |          |
     *  |          |
     *  C----------D
     *  Pair [X, Y]
     */
    //Initializes rand with local time
    time_t t;
    srand(time(&t));

    std::vector<Cell> leafBuffer;
    for (int i = 0; i < (int)leaves.size(); i++) {
        // Leaf1 will be the left most side during a Y-Split
        // Leaf1 will be the top most side during a X-Split
        // Vice versa
        Cell* leaf1 = new Cell();
        Cell* leaf2 = new Cell();

        //Hash Coordinates
        std::pair<int, int> A = leaves[i].vertex[0];
        std::pair<int, int> B = leaves[i].vertex[1];
        std::pair<int, int> C = leaves[i].vertex[2];
        std::pair<int, int> D = leaves[i].vertex[3];

        //Flips the axis of the split if the opposite dimension (length or width) is larger (side * splitMult).
        //Lower number for more square shaped rooms vice versa.
        int splitMult = 1.15;

        //Makes sure that it does not split too near to the borders and making a really small Cell
        //sizeVariance is the minimum of which a Node can be split. Cannot split a Node and have one
        //side less than sizeVariance% of the whole Node size.
        //Lower number for larger range of size variance vice versa. Has to be between 0.16 - 0.5
        float sizeVariance = 0.45;

        //Note: Name of the split is parallel to the axis it's splitting to. Ex: Y-Split is horizontal
        //Split in Y-Axis
        if(rand() % 2 ? ((C.second - A.second) > ((B.first - A.first) * splitMult) ? false : true) :
                        ((B.first - A.first) > ((C.second - A.second) * splitMult) ? true  : false)) {

            int capVal = (B.first - A.first) * sizeVariance;

            //Random Y Split value (X value)
            int randX = rand() % ((A.first + capVal) - (B.first - capVal)) + (A.first + capVal);

            leaf1->vertex[0] = A;
            leaf1->vertex[1] = std::make_pair(randX, A.second);
            leaf1->vertex[2] = C;
            leaf1->vertex[3] = std::make_pair(randX, C.second);

            leaf2->vertex[0] = std::make_pair(randX, A.second);
            leaf2->vertex[1] = B;
            leaf2->vertex[2] = std::make_pair(randX, C.second);
            leaf2->vertex[3] = D;
        }
        //Split in X-Axis
        else {
            int capVal = (C.second - A.second) * sizeVariance;

            //Random X Split value (Y value)
            int randY = rand() % ((A.second + capVal) - (C.second - capVal)) + (A.second + capVal);

            leaf1->vertex[0] = A;
            leaf1->vertex[1] = B;
            leaf1->vertex[2] = std::make_pair(A.first, randY);
            leaf1->vertex[3] = std::make_pair(B.first, randY);

            leaf2->vertex[0] = std::make_pair(A.first, randY);
            leaf2->vertex[1] = std::make_pair(B.first, randY);
            leaf2->vertex[2] = C;
            leaf2->vertex[3] = D;
        }

        leafBuffer.push_back(*leaf1);
        leafBuffer.push_back(*leaf2);
    }
    leaves.clear();
    leaves = leafBuffer;
}

void BSPDungeon::buildRooms(){
    for(int i = 0; i < (int)leaves.size(); i++) {

        //Hash Coordinates
        //Note = corner "C" & "B" not needed
        std::pair<int, int> A = leaves[i].vertex[0];
        std::pair<int, int> D = leaves[i].vertex[3];

        //Rules for how rooms are generated
        //capValX/Y handle the range of where the origin is generated
        //
        int capValX = rand() % ((D.first - A.first) / 3) + (A.first+((D.first - A.first) * 0.1));
        int capValY = rand() % ((D.second - A.second) / 3) + (A.second+((D.second - A.second) * 0.1));

        int width = (D.first - capValX);
        int height = (D.second - capValY);

        int capWidth = rand() % (int)((width * 0.95)-(width * 0.8)) + (int)(width * 0.8);
        int capHeight = rand() % (int)((height * 0.95)-(height * 0.8)) + (int)(height * 0.8);

        //Generate random location, height, and width using cap values
        Room tRoom;
        tRoom.origin = std::make_pair(capValX, capValY);
        tRoom.height = capHeight;
        tRoom.width = capWidth;

        leaves[i].room = tRoom;
    }
}

//See connectRoom() for usage
void BSPDungeon::connectRoom(Room room1ID, Room room2ID, bool splitAxis) {

    Room room1 = room1ID;
    Room room2 = room2ID;

    bool split = splitAxis;

    //Hash points to be used for determining range for rand()
    //Changes value of the vertices used in determining the range based on what kind of split occurred
    int room1P = (split) ? room1.origin.first : room1.origin.second;
    int room2P = (split) ? room2.origin.first : room2.origin.second;
    int room1S = (split) ? room1.width : room1.height;
    int room2S = (split) ? room2.width : room2.height;

    //Determines range for rand()
    int widthCap = findSmallestRoomDimension() / 3;
    int pointA = (room1P > room2P) ? room1P : room2P; //Picks the higher number
    int pointB = ((room1P + room1S) > (room2P + room2S)) ? (room2P + room2S) : (room1P + room1S); //Picks smaller number
    //Ends function if pointA - pointB cant support the widthcap
    if ((pointB - pointA - widthCap) < widthCap/2) {
        return;
    }

    int corridorPoint = rand() % (pointB - pointA - widthCap) + pointA;

    Room corridor;
    int corridorX = (split) ? corridorPoint : (room1.origin.first + room1.width);
    int corridorY = (split) ? (room1.origin.second + room1.height) : corridorPoint;

    corridor.origin = std::make_pair(corridorX, corridorY);
    corridor.height = (split) ? (room2.origin.second - (room1.origin.second + room1.height)) : widthCap;
    corridor.width =  (split) ? widthCap : (room2.origin.first - (room1.origin.first + room1.width));

    roomList.push_back(corridor);
}

void BSPDungeon::buildCorridors() {

    for (int i = 1; i <= dungeonIterations; i++) {
        for (int j = 0; j < (int)leaves.size(); j += (int)std::pow(2,i)) {

            int indexRef = ((int)std::pow(2,i) / 2 ) - 1;

            bool splitAxis;
            int vertexRef;
            if(leaves[indexRef + j].vertex[1].first == leaves[indexRef + j + 1].vertex[0].first) {
                splitAxis = false;
                vertexRef = leaves[indexRef + j].vertex[1].first;
            }
            else if (leaves[indexRef + j].vertex[2].second == leaves[indexRef + j + 1].vertex[0].second) {
                splitAxis = true;
                vertexRef = leaves[indexRef + j].vertex[2].second;
            }

            std::vector<Cell> potentialRoom1;
            std::vector<Cell> potentialRoom2;
            for (int k = 0; k < (indexRef + 1)*2; k++) {
                //X-Split
                if(splitAxis) {
                    if (leaves[j+k].vertex[2].second == vertexRef) {
                        potentialRoom1.push_back(leaves[j+k]);
                    }
                    if (leaves[j+k].vertex[0].second == vertexRef) {
                        potentialRoom2.push_back(leaves[j+k]);
                    }
                }
                //Y-Split
                else if (!splitAxis) {
                    if(leaves[j+k].vertex[1].first == vertexRef) {
                        potentialRoom1.push_back(leaves[j+k]);
                    }
                    if(leaves[j+k].vertex[0].first == vertexRef) {
                        potentialRoom2.push_back(leaves[j+k]);
                    }
                }
            }

            for (int m = 0; m < (int)potentialRoom1.size(); m++)
                for (int n = 0; n < (int)potentialRoom2.size(); n++) {
                    connectRoom(potentialRoom1[m].room, potentialRoom2[n].room, splitAxis);
                    //Add a break here if you only one corridor attached to each room
                    break;
                }

        }
    }
}

//Merges corridors from roomList and rooms from leaves into roomList
void BSPDungeon::mergeRoomsAndCorridors() {
    for(int i = 0; i < (int)leaves.size(); i++) {
        roomList.push_back(leaves[i].room);
    }
}

int BSPDungeon::findSmallestRoomDimension() {
    //Default size
    //FIXME: Setting making this empty/NULL or even 0 will make only return 0
    int side = 25;

    for(int i = 0; i < (int)leaves.size(); i++) {
        if (leaves[i].room.height > leaves[i].room.width) {
            side = (leaves[i].room.width < side) ? leaves[i].room.width : side;
        }
        else {
            side = (leaves[i].room.height < side) ? leaves[i].room.height : side;
        }
    }
    return side;
}

