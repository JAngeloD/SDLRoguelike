#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <utility>

#include <SDL.h>
#include <time.h>

// THIS CLASS IS A TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

struct Room {

    //Upper Left Coordinate
    std::pair<int, int> origin;
    int height;
    int width;

};

//Contains split dimensions / dungeon dimensions
struct Cell {
    std::pair<int, int> vertex[4]; //Node dimensions
    bool split; //X Split = True
                //Y Split = False

    struct Cell* parent = nullptr;
    struct Cell* sister = nullptr;
    struct Cell* children[2] = {};
    Room room;
};

class BSPDungeon {
    public:
    BSPDungeon(int width, int height, int iterations);

    public:
    void draw();
    std::vector<Cell> getLeaves() {return leaves;}
    std::vector<Room> getRooms() {return roomList;}

    private:
    void split();
    void buildRooms();
    void buildCorridors();
    int findSmallestRoomDimension(); //Finds the smallest width/height of a room to be used to make corridor size scale
    void destroyTree();

    private:
    int dungeonWidth;
    int dungeonHeight;
    Cell root;
    std::vector<Room> roomList;
    std::vector<Cell> leaves;


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

#endif
