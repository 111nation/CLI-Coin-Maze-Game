#include "gamedata.h"

void Map::InitRoom() {
	// Initializes room array
	arrRoom = new int*[height + 2]; 

    	// Sets amount of coloumns for array
    	for (int i = 0; i < height+2;i++) {
        	arrRoom[i] = new int[width+2];
		// UNOCCUPIES SPACE
		for (int j = 0; j < width+2; j++) {
			if (j == 0 || j == width+1) {
				arrRoom[i][j] = RWALL; // Indicates map walls
			} else if (i == 0 || i == width+1) {
				arrRoom[i][j] = RWALL; // Indicates map walls
			} else {
				arrRoom[i][j] = UNOCCUPIED;
			}
		}
    	}

}

void Map::DoorGen() {
	// CREATES ARRAY FOR PATHFINDING
	int** PATH = new int * [height+2];
	// TRACKS AND COUNTS SPACES
	const int visited = 0, unvisited = 1, wall = 2;
	int spaces = 0;
	for (int y = 0; y < height+2; y++) {
			// CREATES COLUMNS
			PATH[y] = new int [width+2];
		for (int x = 0; x < width+2; x++) {
			int obj = arrMap[y][x];
			if (obj == PLAYER || obj == SPACE){
				++spaces;
				PATH[y][x] = unvisited; // INDICATES UNVISITED SPACE	
			} else {
				PATH[y][x] = wall;
			}
		}
	}

	// FLOOD-FILL TO CHECK IF WHOLE MAP ACCESSABLE
	for (int y = 1; y < height+1; y++) {
		for (int x = 1; x < width+1; x++) {
			// COUNTS ACCESSABLE SPACE
			if (arrMap[y][x] == SPACE) { 
				--spaces;
				PATH[y][x] = visited;
			}

			// DETERMINES WHERE TO GO NEXT
			// ATTEMPS GOING RIGHT
			if (PATH[y][x+1] != wall && !(x+1 >= width+1)) {
				continue;
			}	
			// ATTEMPS GOING LEFT
			if (PATH[y][x-1] != wall && !(x-1 < 0)) {
				x-=2;
				continue;
			}	
			// ATTEMPS GOING DOWN
			if (PATH[y+1][x] != wall && !(y+1 >= height+1)) {
				break;
			}
			// ATTEMPS GOING UP
			if (PATH[y-1][x] != wall && !(y-1 < 0)) {
				y-=2;
				break;
			}

		}
	}

	// CLEARS ARRAY FROM MEMORY
	for (int y = 0; y < height+2; y++) {
		delete [] PATH[y];
	}

	delete [] PATH;
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
	// ===CHECKS TO SEE WITHIN MAP BOUNDARY===
	// BOTTOM
	if (rheight + starty >= height+1) return false;
	// RIGHT
	if (rwidth + startx >= width+1) return false;
	
	//===CHECKS ROOM NOT TOO SMALL============
	if (rwidth <= 1 || rheight <= 1) return false;

	//===CHECKS ROOM NOT ON PLAYER SPAWN======
	/*if (starty-1==0 && startx-1==0) return false;
	if (startx-1==0 && starty-1<=0) return false;
	if (starty-1==0 && startx-1<=0) return false;*/
	
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
			
			if (x == startx || x == xend) {
				if (x-1 == 0) return false;
			} 

			if (y== starty || y == yend) {
				if (y-1 == 0) return false;
			}

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

	for (int y = starty; (y < height+1) && (y <= yend) ; y++) {

		for (int x = startx; (x < width+1) && (x <= xend) ; x++) {
			// ENSURES WE DONT OVERWRITE WALLS
			if (!isWall(y, x)) {
				if (x == startx || x == startx + rwidth) {
					
					arrMap[y][x] = VWALL; // LEFT AND RIGHT WALLS
					arrRoom[y][x] = RWALL;
					--spaceLeft; // Space decreases
				} else if (y == starty || y == starty + rheight) {
					arrMap[y][x] = HWALL; // TOP AND BOTTOM WALLS
					arrRoom[y][x] = RWALL;
					--spaceLeft;
				} else {
				
					arrMap[y][x] = SPACE; // REGULAR SPACE
					arrRoom[y][x] = OCCUPIED;
				
				}	
					
			}

		}
	
	}
}

bool Map::RoomGen() {

	struct struct_room { 
		int width;
		int height;
		int area;
	};

    srand(time(0));

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
			Room.width = (rand() % 10 ) + 1;
			Room.height = (rand() % 10 ) + 1;
			Room.area = Room.width * Room.height;

			//	CHECK IF ROOM WILL FIT
			if (!willRoomFit(y, x, Room.width, Room.height)) continue;
		
			CreateRoom(y, x, Room.width, Room.height);

            Room = {};
		}

	}

	return false; // Rooms didnt finish generating	
}
