#include "gamedata.h"

Map::Map(int x, int y) {
		game_status = PLAYING;

		width = x;
		height = y;
		if ((width < MIN_WIDTH) || (height < MIN_HEIGHT)) {
			throw "Map too small";
		} else if ((width > MAX_WIDTH) || (height > MAX_HEIGHT)) {
			throw "Map too big";
		}
	
		ycurs = -1;
		xcurs = -1; 
		
		// Calculates PLAYER HEALTH
		Player.health = 5;

		MapGen(); // Generates map
		Status();
		DrawNew();

		// Initiializes curser properties
		if (!GetConsoleCursorInfo(hConsoleWindow, &CursorProp)) {
				throw "Cursor failure";
		}

		hideCurs();	
}

Map::~Map () {
    // Deletes each row from memory
    for (int i = 0; i < height + 2; i++) {
        delete [] arrMap[i];
	delete [] arrRoom[i];
	delete [] PATH[i];
    }

    delete [] PATH;
    delete [] arrMap;
    delete [] arrRoom;

    showCurs();
}

//=======================
// ARRAY INITIALIZATION
//=======================
int Map::getObject(int y, int x) {
	// -1 is out of bounds
	if ( (y < 0) || (y > height+1)) return -1;
	if ( (x < 0) || (x > width+1)) return -1;

	return arrMap[y][x];			

}

void Map::MapGen(){

	//============================
	// INITIALIZE
	//============================
	coinsLeft = 0;
	coins = 0;
	
	arrMap = new int*[height + 2]; 

    	// Sets amount of coloumns for array
    	for (int i = 0; i < height+2;i++) {
        	arrMap[i] = new int[width+2];
    	}

    	// Puts walls around Array
    	// Empty space aswell
	for (int y = 0; y < height+2; y++) {
		arrMap[y][0] = VWALL;
			
		for (int x = 1; x < width+1; x++){
			

			// Handles last row of walls
			if ((y == height+1)||(y==0)) { // TOP ROW
				arrMap[y][x] = HWALL;
			} else {
             		   arrMap[y][x] = SPACE;
           		}
		}

	    arrMap[y][width+1] = VWALL;
	}

	// GENERATES ROOMS
	InitRoom();
	RoomGen();

	// Generates mines
	InitMines();

	int space_for_coins = ObjCount(arrMap, SPACE);
	// Generates coins
	if (space_for_coins == 0) throw "Map has no space for coins";
	coins = (int)(space_for_coins*0.3);

	coinsLeft = 0;

	while(!CoinGen()); // Ensures all coins generated
	
	// Place player
	arrMap[Player.y + 1][Player.x + 1]=PLAYER;
	ycurs = 0;
	xcurs = 0;
}

wchar_t Map::getChar(int y, int x) {
	if ( (y < 0) || (y > height+1)) throw "Out of bounds";
	if ( (x < 0) || (x > width+1)) throw "Out of bounds";

	switch (arrMap[y][x]) {
		case PLAYER: 
			return cplayer;
		case SPACE:
			return space;

		case COIN:
			return coin;

		case HWALL:
			return hwall;
		case VWALL:
			return vwall;
		case MINE: 
			return mine;

		default: 
			return space;
	}
}

//=======================
// PLAYER MOVEMENT
//=======================
void Map::Move(int x, int y) {
		
		// if move out of bounds of array, move not considered
		if ((Player.x + x > width + 1)||(Player.x + x < -1)) return;
		if ((Player.y + y > height + 1) || (Player.y + y < -1)) return;
	
		int newx = Player.x + x;
		int newy = Player.y + y;

		bool moved = false;
		
		// Checks what object player landed on
		int objLanded = arrMap[newy + 1][newx + 1];
		switch (objLanded) {
			
			case MINE:
				arrMap[newy + 1][newx + 1] = PLAYER;
				moved = true;
			case DOOR:
			case SPACE:
				arrMap[newy + 1][newx + 1] = PLAYER;
				moved = true;
				break;

			case COIN:
				arrMap[newy + 1][newx + 1] = PLAYER;
				moved = true;
				break;

			case VWALL:
			case HWALL:
				break;

			default: 
				break;
		}

		// Successful movement
		if (moved == true) {
			arrMap[Player.y + 1][Player.x + 1] = SPACE; // Overwrites in array
			std::wcout << " " << '\b'; // Delete in I/O
				
			Player.x = newx;
			Player.y = newy; 	
			MovCurs(Player.x - xcurs, Player.y - ycurs);

			arrMap[newy + 1][newx + 1] = PLAYER;
			console.Colour(getFgColour(PLAYER), 0);
			std::wcout << cplayer << '\b';
			console.ResetColour();
			// Update curser
			ycurs = Player.y;
			xcurs = Player.x;
			
			// Determines what to do when user lands on certain object
			switch (objLanded) {
				case COIN:
					pickCoin();
					Status();
					break;
				case MINE:
					steppedOnMine();
					Status();
					break;
			}
		}

}

//======================
//   UTILITIES
//======================
//=======================================================
//		BOUNDARY CHECKING
//=======================================================
bool Map::OutOfBounds(int y, int x) {
	if (y < 0 || x < 0) return true;
	if (y >= height+2 || x >= width+2) return true;

	return false;
}

bool Map::OutOfBounds(int y, int x, int pheight, int pwidth) {
	if (pheight < 0 || pwidth < 0) return true;
	if (OutOfBounds(y, x) || OutOfBounds(y + pheight, x + pwidth)) return true;
	
	return false;
}

bool Map::OnBoundary(int y, int x) {
	if (y == 0 || x == 0) return true;
	if (y == height+1 || x == width+1) return true;

	return false;
}

bool Map::OnBoundary(int y, int x, int pheight, int pwidth) {
	if (pheight < 0 || pwidth < 0) return true;
	if (OnBoundary(y, x) || OnBoundary(y + pheight, x + pwidth)) return true;

	return false;
}


// Counts amount of the element in an array
int Map::ObjCount(int ** ARR, const int obj) { 
	int count = 0;
	for (int y = 0; y < height + 2; y++) {
		for (int x = 0; x < width + 2; x++) {
			if (ARR[y][x] == obj) {
				++count;
			}	
		}
	}

	return count;
}


//===========================
// GAME CONTROLS
//===========================
void Map::GameOver() {
	game_status = LOST;	
}

void Map::Win() {
	game_status = WIN;
}
