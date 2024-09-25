#include "gamedata.h"

// DIRECTION DATA
#define HORIZONTAL true
#define VERTICAL false

void Map::InitRoom() {
	// Initializes room array
	arrRoom = new int*[height + 2]; 

    	// Sets amount of coloumns for array
    	for (int y = 0; y < height+2;y++) {
        	arrRoom[y] = new int[width+2];
		// UNOCCUPIES SPACE
		for (int x = 0; x < width+2; x++) {
			if (x == 0 || x == width+1) {
				arrRoom[y][x] = RWALL; // Indicates map walls
			} else if (y == 0 || y == height+1) {
				arrRoom[y][x] = RWALL; // Indicates map walls
			} else {
				arrRoom[y][x] = UNOCCUPIED;
			}
		}
    	}

}

//=============================================================================
//	 		DOOR GENERATION LOGIC
//=============================================================================
bool Map::isDoorPossible (const bool direction, int starty, int startx, int**PATH) {

	// ENSURES COORDINATES NOT OUT OF BOUNDS (INSIDE MAP NOT BOUNDARIES)
	if (starty <= 0 || starty >= height+2 - 1) return false;
	if (startx <= 0 || startx >= width+2 - 1) return false;
	// ONLY DOES CHECKS ON WALLS
	if (PATH[starty][startx] != ROOM_WALL) return false;


	// CHECKS IF DOOR CAN BE PLACED:
	// WALL MUST BE NEXT TO VISITED AND UNVISITED SPACE
	// [U][W][V]

	int adjacent [2] = {}; // GETS ALL ADJACENT TILES
	if (direction == VERTICAL) {		
		adjacent[0] = PATH[starty-1][startx];
		adjacent[1] = PATH[starty+1][startx];
	} else {
		adjacent[0] = PATH[starty][startx-1];
		adjacent[1] = PATH[starty][startx+1];

	}

	// MAKES SURE THAT DOOR CAN BE PLACED
	// Make sure door can be placed: wall must be next to visited and unvisited space	
	if (adjacent[0] != ROOM_WALL && adjacent[1] != ROOM_WALL) {
		if (adjacent[0] != adjacent[1]) {
			return true;
		}
	}

	return false;
}

void Map::floodFill(int y, int x, int** PATH, int* visitedSpaces) { 
	// MAKE SURE WE DO NOT CROSS BOUNDARIES
	if (x < 0 || y < 0 || x >= width+2 || y >= height+2) return;
	
	// ONLY COUNTS UNVISITED SPACES
	if (PATH[y][x] != ROOM_UNVISITED) return;

	PATH[y][x] = ROOM_VISITED;	
	++(*visitedSpaces);

	// CHECK FOR SURROUNDING SPACES
	floodFill(y, x + 1, PATH, visitedSpaces); // Right	
	floodFill(y, x - 1, PATH, visitedSpaces); // Left
	floodFill(y + 1, x, PATH, visitedSpaces); // Down
	floodFill(y - 1, x, PATH, visitedSpaces); // Up
}	

void Map::resetPATH(int ** PATH, int * visitedSpaces) {
	*visitedSpaces = 0;
	for (int y = 0; y < height+2; y++) {
		for (int x = 0; x < width+2; x++) {
			if (PATH[y][x] == ROOM_VISITED) {
				PATH[y][x] = ROOM_UNVISITED;
			}
		}
	}
}

bool Map::createDoor(int y, int x, int ** PATH) {
	// Ensures not out of bounds
	// AND not allowed to make map boundaries doors
	if (y <= 0 || y >= height+1) return false;
	if (x <= 0 || x >= width+1) return false;

       	// Ignores if door cannot be created
	if (PATH[y][x] != ROOM_WALL) return false;

	PATH[y][x] = ROOM_UNVISITED; // Makes it space in path
	arrRoom[y][x] = RDOOR;
	arrMap[y][x] = DOOR;

	return true;

}

void Map::DoorGen() {

	//=================================
	// INITIALIZE FOR RANDOMIZING DOORS
	//=================================
	int spaces=0;
	// TRACKS AND COUNTS SPACES
	for (int y = 0; y < height+2; y++) {
			// CREATES COLUMNS
			PATH[y] = new int [width+2];
		for (int x = 0; x < width+2; x++) {
			int obj = arrMap[y][x];
			if (obj == PLAYER || obj == SPACE){
				PATH[y][x] = ROOM_UNVISITED; // INDICATES UNVISITED SPACE	
				++spaces;
			} else {
				PATH[y][x] = ROOM_WALL;
			}
		}
	}
	
	//===========================
	// CREATE DOORS
	//===========================
	int visitedSpaces = 0;
       	
	resetPATH(PATH, &visitedSpaces);	
	floodFill(SPAWNY, SPAWNX, PATH, &visitedSpaces);
	
	std::cerr << arrMap[1][1];
	while (visitedSpaces < spaces) {
		bool doorAdded = false;
		for (int y = 1; y < height+1; y++) {
			for (int x = 1; x < width+1; x++) {
				doorAdded = false;
				int prob = rand() % 10;
				// 1 IN 10 CHANCE OF ATTEMPTING TO CREATE DOOR	
				if (isDoorPossible (VERTICAL, y, x, PATH) && prob == 0) { 
					doorAdded = createDoor(y, x, PATH);
				} else if (isDoorPossible(HORIZONTAL, y, x, PATH) && prob == 1) {
					doorAdded = createDoor(y, x, PATH);
				}

				if (doorAdded == true) {
					++spaces;
					resetPATH(PATH, &visitedSpaces);
					floodFill(SPAWNY, SPAWNX, PATH, &visitedSpaces);
				}
			}
		}
	}

	Message("Visited Spaces: " + std::to_string(visitedSpaces) + 
			"\nTotal Spaces: " + std::to_string(spaces));	
	// Resets PATH for later use
	resetPATH(PATH, &visitedSpaces);

/*	// CLEARS ARRAY FROM MEMORY
	for (int y = 0; y < height+2; y++) {
		delete [] PATH[y];
	}

	delete [] PATH; */
}

int Map::calcRoomProb(int starty, int startx) {

	int surrounding = 0;
	// CHECKS AROUND FOR SURROUNDING ROOMS
	for (int y = starty-1; (y < height+1) && (y <= starty + 1); y++) {
		for (int x = startx-1; (x < width+1) && (x <= startx + 1); x++) {
			// ENSURES THAT WE ARE NOT OUT OF BOUNDS
			if (y < 0) y = 0;
			if (x < 0) x = 0;

			// SKIPS MAP BOUNDARIES
			if (y == 0) continue;
			if (x == 0) continue;

			// COUNTS HOW MANY ROOM SPACES OCCUPIED
			if (arrRoom[y][x] != UNOCCUPIED){
				++surrounding;
			}	
		}

	}

	if (surrounding <= 0) {
        return (rand() % 3);
    } else {
        return (rand() % surrounding+1);
    }
}

bool Map::willRoomFit(int starty, int startx, int rwidth, int rheight) {
	// ===PREVENT PLACING ON PLAYER===========
	if (starty == SPAWNY && startx == SPAWNX) return false;
	
	// ===CHECKS TO SEE WITHIN MAP BOUNDARY===
	// BOTTOM
	if (rheight + starty >= height+2) return false;
	// RIGHT
	if (rwidth + startx >= width+2) return false;
	
	//===CHECKS ROOM NOT TOO SMALL============
	if (rwidth <= 3 || rheight <= 3) return false;

	//===ROOM OVERLAP PREVENTION==============
	const int xend = startx + rwidth;
	const int yend = starty + rheight;
	
	for (int y = starty; (y < height+1) && (y <= yend) ; y++) {
		for (int x = startx; (x < width+1) && (x <= xend) ; x++) {
			
			if (isWall(y,x)) {
				if (!((x == startx || x == xend) || (y == starty || y == yend))) {
					return false; // PREVENTS ROOM FROM OVERLAPPING
				}
			}

			// PREVENTS ROOM FROM SPAWNING
			// INSIDE ANOTHER ROOM BY 
			// CHECKING IF ROOM OVERLAPS 
			// EXISTING OCCUPIED ROOM SPACE
			if (arrRoom[y][x] == OCCUPIED) { 
				return false;
			}
			
			/*
			if (x == startx || x == xend) {
				if (x-1 == 0) return false;
			} 

			if (y== starty || y == yend) {
				if (y-1 == 0) return false;
			}
			*/

		}
	}

	return true;	
}

bool Map::isWall(int y, int x) {
	return (arrMap[y][x] == VWALL) || (arrMap[y][x] == HWALL); 
}

void Map::CreateRoom(int starty, int startx, int rwidth, int rheight) {
	const int xend = startx + rwidth;
	const int yend = starty + rheight;

	for (int y = starty; (y < height+2) && (y <= yend) ; y++) {

		for (int x = startx; (x < width+2) && (x <= xend) ; x++) {
			// ENSURES WE DONT OVERWRITE WALLS
			if (!isWall(y, x)) {
				if (x == startx || x == xend) {
					arrMap[y][x] = VWALL; // LEFT AND RIGHT WALLS
					arrRoom[y][x] = RWALL;
				} else if (y == starty || y == yend) {
					arrMap[y][x] = HWALL; // TOP AND BOTTOM WALLS
					arrRoom[y][x] = RWALL;
				} else {
				
					arrMap[y][x] = SPACE; // REGULAR SPACE
					arrRoom[y][x] = OCCUPIED;
				
				}	
					
			}

		}
	
	}
}

//=============================================================================
//	 		WALL SHARING LOGIC
//=============================================================================
// CHECKS IF SHARING WALLS NECISSARY
bool Map::WallFoundx(const int starty, const int startx, const int wall_width) {
	if (OutOfBounds(starty, startx, 0, wall_width)) return false;	
	if (wall_width <= 0) return false;
	// If on boundary wall found
	if (OnBoundary(starty, startx) || OnBoundary(starty, startx, 0, wall_width)) return true;

	for (int x = startx; x < width+1 && x <= (startx+wall_width); x++) {
		if (arrRoom[starty][x] == RWALL) return true;
	}
	
	return false;
}

bool Map::WallFoundy(const int starty, const int startx, const int wall_height) {
	if (OutOfBounds(starty, startx, wall_height, 0)) return false;	
	if (wall_height <= 0) return false;	
	// If on boundary wall found
	if (OnBoundary(starty, startx) || OnBoundary(starty, startx, wall_height, 0)) return true;
	
	for (int y = starty; y < height+1 && y <= (starty+wall_height); y++) {
		if (arrRoom[y][startx] == RWALL) return true;
	}	
	
	return false;
}



void Map::adjustWalls(int * y, int * x, int * room_width, int * room_height) {
	// PREVENTS ADJACENT WALLS BY SHIFTING THEM
	// TO SHARE WALLS INSTEAD
	
	// PREVENTS OUT OF BOUNDS
	if (*y < 0 || *x < 0) return;
	if (*y + *room_height >= height+2 || *x + *room_width >= width+2) return;	
		
	//=========================================
	// 	     VERTICAL WALLS
	//=========================================	
	// LEFT
 	   // ENSURES WALL NOT CONNECTED
	if ((!WallFoundy(*y, *x, *room_height) && WallFoundy(*y, *x-1, *room_height)) || (*x == 1)) { 
		--(*x);
	} 
	// RIGHT
 	   // ENSURES WALL NOT CONNECTED
	if ((!WallFoundy(*y, (*x + *room_width), *room_height)&&
	    WallFoundy(*y, (*x + *room_width+1), *room_height)) || (*x+*room_width==width)) { 
		++(*room_width);
	}

	//=========================================
	// 	     HORIZONTAL WALLS
	//=========================================	
	// TOP
 	   // ENSURES WALL NOT CONNECTED
	if ((!WallFoundx(*y, *x, *room_width) && WallFoundx(*y-1, *x, *room_width)) || (*y==1)) { 
		--(*y);
	} 
	// BOTTOM
 	   // ENSURES WALL NOT CONNECTED
	if ((!WallFoundx((*y + *room_height), *x, *room_width)&&
	    WallFoundx((*y + *room_height)+1, *x, *room_width)) || (*y+*room_height == height)) { 
		++(*room_height);
	}

	
}

//=============================================================================
//	 		MAIN ROOM GENERATION
//=============================================================================

bool Map::RoomGen() {

	struct struct_room { 
		int width;
		int height;
		int area;
	};

	srand(time(0));
	//========================================
	int multiplier_width = (int)(width / 20);
	int multiplier_height = (int)(height / 20);
	//========================================

	for (int y = 0; y < height+1; y++) {
		for (int x=0; x < width+1; x++) {
			if ((x-1 == 0) && (y-1 == 0)) continue; // Skip player position

			//===PROBABILITY============
			// GETS SURROUNDING ROOMS
			//==========================   

			int prob = calcRoomProb(y, x);
				
            		if (prob != 0) continue;	

			// DATA OF HYPOTHETICAL ROOM
			struct_room Room = {};
			Room.width = (rand() % 5-3) + multiplier_width+3;
			Room.height = (rand() % 5-3) + multiplier_height+3;
			Room.area = Room.width * Room.height;

						
			adjustWalls(&y, &x, &(Room.width), &(Room.height));

			//	CHECK IF ROOM WILL FIT
			if (!willRoomFit(y, x, Room.width, Room.height)) continue;
			
			CreateRoom(y, x, Room.width, Room.height);

            		Room = {};
		}

	}

	DoorGen();
	std::cout << "DOOR GEN DIDNT CAUSE CRASH";	
	return false; // Rooms didnt finish generating	
}
