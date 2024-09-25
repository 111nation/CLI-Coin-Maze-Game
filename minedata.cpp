#include "gamedata.h"

void Map::InitMines() {
	srand(time(0));

	//=================================
	// INITIALIZE FOR RANDOMIZING DOORS
	//=================================
	// TRACKS AND COUNTS SPACES
	int spaces =0;
	for (int y = 0; y < height+2; y++) {
			// CREATES COLUMNS
			PATH[y] = new int [width+2];
		for (int x = 0; x < width+2; x++) {
			int obj = arrMap[y][x];
			if (obj == PLAYER || obj == SPACE || obj == DOOR){
				++spaces;
				PATH[y][x] = ROOM_UNVISITED; // INDICATES UNVISITED SPACE	
			} else {
				PATH[y][x] = ROOM_WALL;
			}
		}
	}

	mines = (int)(spaces * 0.10);		
	

	// Loop until all mines created
	int total_mines=0;
	int visitedSpaces=0;
	while (total_mines < mines) {
		int placed=0;
		placeMines(mines - total_mines, &placed); 
		if (placed > 0) {
			total_mines+=placed;
			spaces-=placed;
			//===== CHECKS IF WHOLE MAP ACCESSABLE OR NOT ======
			resetPATH(PATH, &visitedSpaces);
			floodFill(SPAWNY, SPAWNX, PATH, &visitedSpaces);
			//======IF MINES ARE BLOCKING PATH==================
			if (visitedSpaces < spaces) {
			       	int deleted = 0;	
				deleteBlockingMines(&visitedSpaces, &deleted);
				spaces+=deleted;
				total_mines-=placed;

				resetPATH(PATH, &visitedSpaces);
				floodFill(SPAWNY, SPAWNX, PATH, &visitedSpaces);
			}

		}	
	}
	
	
	Message("\nMINES ON MAP: " + std::to_string(ObjCount(arrMap, MINE)) + 
		"\n\nMINES: " + std::to_string(mines) +
		"\nTOTAL MINES: " + std::to_string(total_mines) +
		"\nVISITED: " + std::to_string(visitedSpaces) +
		"\nSPACES: " + std::to_string(spaces) +
		"\n");

}

void Map::deleteBlockingMines(int * visitedSpaces, int * deleted) {
	*deleted = 0;
	// DELETES ALL MINES BLOCKING PATH
	// LOOPS ALL ADJACENT TILES
	resetPATH(PATH, visitedSpaces);
	floodFill(SPAWNY, SPAWNX, PATH, visitedSpaces);
	for (int y=1; y<height+1; y++) {
		for (int x=1; x<width+1; x++){ 
			if (arrMap[y][x] == MINE) {
				// REMOVES A MINE IF ADJACENT TO UNVISITED SPACE
				if (isBlocking(y, x, PATH)) {
						
					arrMap[y][x] = SPACE;
					PATH[y][x] = ROOM_VISITED;
					
					// Updates reachable areas
					resetPATH(PATH, visitedSpaces);
					floodFill(SPAWNY, SPAWNX, PATH, visitedSpaces);
					++(*deleted);

				}
			}
		}
	}	
}

bool Map::isBlocking(int y, int x, int ** PATH) {
	// An object is blocking an area if its adjacent to an unvisited space
	for (int sy=-1; sy<=1; sy++) {
		for (int sx=-1; sx<=1; sx++){
			if (sx == 0 && sy == 0) continue; // skips area which we asses
			// SKIPS CORNERS
			if (sx == -1 && sy == -1) continue;
			if (sx == 1 && sy == -1) continue;
			if (sx == -1 && sy == 1) continue;
			if (sx == 1 && sy == 1) continue;

			int adjY = sy + y;
			int adjX = sx + x;

			// BOUNDARY CHECKING
			if (adjY < 0 || adjX < 0) continue;
			if (adjY >= height+2 || adjX >= width+2) continue;
			
			// CHECKS IF NEXT TO EMPTY UNVISITED SPACE
			if (PATH[adjY][adjX] == ROOM_UNVISITED) {
				return true;
			}	
		}
	}
	return false;
}

// PLACES AMOUNT OF MINES SPECIFIED
// ATTEMPTS ONCE AND GIVES HOW MANY MINES CREATED
void Map::placeMines(int amount_of_mines, int * placed_mines) { 	
	*placed_mines=0;
	if (amount_of_mines <= 0) return;

	for (int y=1; y<height+1; y++) {
		for (int x=1; x<width+1; x++) {
			if ((x-1 == 0) && (y-1 == 0)) continue; // Skip player position
			if (arrMap[y][x] == MINE) continue; // Skip if mine already placed
			if (arrMap[y][x] != SPACE) continue; // ENSURES ONLY SPACES ARE TAKEN
			
			//================================================
			// CHECKS SURROUNDING CMINES (Reused from coins)
			//================================================
			int surrounding_mines = 0;
			// Counts surrounding coins
			for (int sy=-3; sy<=3; sy++) {
				for (int sx=-3; sx<=3; sx++){
					if (sx == 0 && sy == 0) continue; // skips area which we asses
					
					int object = getObject(y+sy, x+sx);
					if (object == MINE) {
						++surrounding_mines;
					}

				}
			}

			// Calculates probability of mine placed based on surrounding coins
			int probability = surrounding_mines+21;
			probability = rand() % probability;
			// Ensures mine generation not too rare
			if (probability > 200) {
				probability = (rand() % 201);
			}	

			// Randomly places mine		
			if (probability == 1 && (*placed_mines < amount_of_mines)) {
				++(*placed_mines);
				arrMap[y][x] = MINE;
				PATH[y][x] = ROOM_WALL;
			}

			if (*placed_mines >= amount_of_mines) return;
					
		}
	}
}

