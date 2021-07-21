#include "mapgen.h"

// THIS IS A DIRECT TRANSLATION TO C++11 FROM THE REFERENCE
// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// I ADDED AN EXTRA METHOD THAT GENERATES A NEW PERMUTATION VECTOR (THIS IS NOT PRESENT IN THE ORIGINAL IMPLEMENTATION)

// Initialize with the reference values for the permutation vector
PerlinNoise::PerlinNoise() {

	// Initialize the permutation vector with the reference values
	p = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
		43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
		107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) {
	p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
	// Find the unit cube that contains the point
	int X = (int) floor(x) & 255;
	int Y = (int) floor(y) & 255;
	int Z = (int) floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
	return (res + 1.0)/2.0;
}

double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

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
}

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
        float sizeVariance = 0.35;

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
            }

        }
    }
}

int BSPDungeon::findSmallestRoomDimension() {
    //Default size
    //Note: Setting making this empty/NULL or even 0 will make only return 0
    //Check printf() bug
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

