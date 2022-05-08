#include <vector>
#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <utility>

#include <SDL.h>
#include <time.h>

#ifndef MAPGEN_H
#define MAPGEN_H

struct Room {
    std::pair<int, int> origin; //Upper Left Coordinate
    int height;
    int width;
};

//Contains split dimensions / dungeon dimensions
struct Cell {
    std::pair<int, int> vertex[4]; //Node dimensions
    bool split; //X Split = True
                //Y Split = False
    Room room;
};

class BSPDungeon {
    public:
    BSPDungeon(int width, int height, int iterations);

    void writeDungeonToFile(std::string path);

    public:
    std::vector<Cell> getLeaves() {return leaves;}
    std::vector<Room> getRooms() {return roomList;}

    private:
    void buildRooms();
    void split();
    void connectRoom(Room room1ID, Room room2ID, bool splitAxis);
    void buildCorridors();
    void mergeRoomsAndCorridors();
    int findSmallestRoomDimension(); //Finds the smallest width/height of a room to be used to make corridor size scale when increasing iteration #

    private:
    Cell root; //Contains dimensions of the whole dungeon
    int dungeonWidth;
    int dungeonHeight;
    int dungeonIterations;

    //Note: important to seperate rooms and leaves. Don't remove/combine
    std::vector<Cell> leaves;
    std::vector<Room> roomList;
};
#endif // MAPGEN_H
