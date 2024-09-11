#include "gamedata.h"

Map::Map(int x, int y) {
		width = x;
		height = y;
		if ((width < 5) || (height < 5)) {
			throw "Map too small";
		} else if ((width > 100) || (height > 25)) {
			throw "Map too big";
		}
	
		ycurs = -1;
		xcurs = -1; 
		
		MapGen(); // Generates map
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
    }

    delete [] arrMap;
}

//=======================
// ARRAY INITIALIZATION
//=======================
bool Map::CoinGen() {
	
	srand(time(0));

	for (int y = 1; y < height+1; y++) {
		for (int x=1; x < width+1; x++) {
			if ((x-1 == 0) && (y-1 == 0)) continue; // Skip player position
			
			int placeCoin = rand() % 2;	
			// Randomly places coins		
			if ((placeCoin == 1) && (coinsLeft < coins)) {
				coinsLeft++;
				arrMap[y][x] = COIN;
			}

			if (coinsLeft == coins) return true; // If all coins generated we exit
		}
	}

	return false; // Not all coins where generated
}

void Map::MapGen(){
	
	arrMap = new int*[height + 2]; 

    	// Sets amount of coloumns for array
    	for (int i = 0; i < height+2;i++) {
        	arrMap[i] = new int[width+2];
    	}

    	// Puts walls around Array
    	// Empty space aswell
	for (int y = 0; y < height+2; y++) {
		arrMap[y][0] = WALL;
			
		for (int x = 1; x < width+1; x++){
			// Handles last row of walls
			if ((y == height+1)||(y==0)) { // TOP ROW
				arrMap[y][x] = WALL;
			} else { 
             		   arrMap[y][x] = SPACE;
           		}
		}

	    arrMap[y][width+1] = WALL;
	}
	
	// Generates coins
	coins = (int)((height * width) * 0.6) / 1;
	coinsLeft = 0;

	while(!CoinGen()); // Ensures all coins generated

	// Place player
	arrMap[Player.y + 1][Player.x + 1]=PLAYER;
	ycurs = 0;
	xcurs = 0;

}

//=======================
// CURSER CONTROLS
//=======================

void Map::CursReturn() {
		// Returns curser to beginning
		std::cout << '\r' << ESC << height+2 << UP;
		ycurs = -1;
		xcurs = -1;	
}

void Map::CursPlayer() {
		// Returns curser to player
	
		std::cout << '\r' << ESC << ycurs+1 << UP;
		std::cout << ESC << Player.y << DOWN;

		std::cout << ESC << Player.x << RIGHT;

		xcurs = Player.x;
		ycurs = Player.y;
}

void Map::hideCurs() { 
		CursorProp.bVisible = false;
		
		if (!SetConsoleCursorInfo(hConsoleWindow, &CursorProp)) {
				throw "Cursor failure";
		}
}

void Map::showCurs() {
		CursorProp.bVisible = true;

		if (!SetConsoleCursorInfo(hConsoleWindow, &CursorProp)) {
				throw "Cursor failure";
		}

}

void Map::cursBottom() {
	std::cout << ESC << height + 3 << DOWN;
		ycurs = height + 2;
}

void Map::cursTop() {
		std::cout << ESC << ycurs + 1 << UP;
		ycurs = -1;
}

void Map::MovCurs(int x, int y) {
		// Y Movement
		if (y != 0) {
			if (y > 0) {
				std::cout << ESC << y << DOWN;
			} else {
				std::cout << ESC << -y << UP;
			}
		}

		// X Movement
		if (x != 0) {
			if (x > 0) {
				std::cout << ESC << x << RIGHT;
			} else {
				std::cout << ESC << -x << LEFT;
			}
		}

}

//=======================	
// UPDATING SCREEN
//=======================

void Map::DrawNew() {
		CursReturn();
		
		for (ycurs = -1; ycurs <= height; ycurs++) { 
			for (xcurs = -1; xcurs <= width; xcurs++) {
				std::wcout << getChar(ycurs + 1, xcurs + 1); 
			}
			std::wcout<<'\n';
		}	

		CursPlayer();
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

		case WALL:

			if ((x==0) || (x==width+1)) { return vwall; }
			else { return hwall; }	

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
		switch (arrMap[newy + 1][newx + 1]) {
			case SPACE:
				arrMap[newy + 1][newx + 1] = PLAYER;
				moved = true;
				break;
			case COIN:
				arrMap[newy + 1][newx + 1] = PLAYER;
				//pickCoin();
				moved = true;
				break;
			case WALL:
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
			std::wcout << cplayer << '\b';
			// Update curser
			ycurs = Player.y;
			xcurs = Player.x;

		}
}
