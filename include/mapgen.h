#include <vector>
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

    public:
    void buildDungeon(int iterations);
    void draw();
    std::vector<Cell> getLeaves() {return leaves;}
    std::vector<Room> getRooms() {return roomList;}

    private:
    void buildRooms();
    void connectRoom(Room room1ID, Room room2ID, bool splitAxis);
    void buildCorridors();
    int findSmallestRoomDimension(); //Finds the smallest width/height of a room to be used to make corridor size scale when increasing iteration #

    private:
    Cell root; //Contains dimensions of the whole dungeon
    int dungeonWidth;
    int dungeonHeight;
    int dungeonIterations;
    std::vector<Cell> leaves; //Note: important to seperate rooms and leaves dont remove
    std::vector<Room> roomList;
};

class PerlinNoise {
	// The permutation vector
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
#endif // MAPGEN_H
