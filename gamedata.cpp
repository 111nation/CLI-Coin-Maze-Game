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
	delete [] arrRoom[i];
	delete [] PATH[i];
    }

    delete [] PATH;
    delete [] arrMap;
    delete [] arrRoom;
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
	spaceLeft = 0;
	
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
			   ++spaceLeft;
           		}
		}

	    arrMap[y][width+1] = VWALL;
	}

	// PLACING PLAYER DECREASES SPACE
	--spaceLeft;
	
	// GENERATES ROOMS
	InitRoom();
	RoomGen();

	// Generates mines
	//InitMines();

	// Generates coins
	if (spaceLeft == 0) throw "Map has no space for coins";
	coins = (int)(spaceLeft*0.3);

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
		std::cout << ESC << Player.y+1 << DOWN;

		std::cout << ESC << Player.x+1 << RIGHT;

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

void Map::CursBottom() {
	CursTop();
	std::cout << ESC << height + 2 << DOWN;
	ycurs = height + 1;
}

void Map::CursTop() {
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

void Map::Message(std::string msg) { 
		// CURSER PLACEMENT
		int oldy = ycurs;
		int oldx = xcurs;
		

		//==========================
		// MESSAGE HANDLING
		//==========================
		// COUNTS MESSAGE LINES
		int newMsg_lines = 0;
		for (unsigned int i = 0; i < msg.length(); i++) {
			if (msg[i] == '\n') {
				++newMsg_lines;
			}
		}
		
		CursBottom();

		// CLEARS PREV MESSAGE
		if (msg_lines > 0) {
			std::cout << '\r';

			MovCurs(0,msg_lines);
			for (int i = msg_lines; i > 0; i--) {
				std::cout << ESC << "2K";
			}		
			MovCurs(0,-msg_lines); 	
		}
		
		// DISPLAY MESSAGE
		msg_lines = newMsg_lines;
		std::cout << '\r'<< msg;

		//==========================
		// CURSER HANDLING
		//==========================
		// UPDATES CURSER POS
		std::cout << '\r';
		xcurs = -1;
		ycurs = ((height + 2) + msg_lines) - 1;
		
		// BACK TO TOP
		CursTop(); 
		
		MovCurs(oldx+1, oldy+1);
		ycurs = oldy;
		xcurs = oldx;
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

			case DOOR:
			case MINE:
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
			std::wcout << cplayer << '\b';
			// Update curser
			ycurs = Player.y;
			xcurs = Player.x;
			
			// Determins what to do when user lands on certain object
			switch (objLanded) {
				case COIN:
					pickCoin();
					break;
			}
		}

}

//======================
//   UTILITIES
//======================
// CHECKS IF COORDINATES OUT OF BOUNDS
bool Map::OutOfBounds(int y, int x) {
	if (y < 0 || x < 0) return true;
	if (y >= height+2 || x >= width+2) return true;

	return false;
}

bool Map::OutOfBounds(int y, int x, int height, int width) {
	if (height < 0 || width < 0) return true;
	if (OutOfBounds(y, x) || OutOfBounds(y + height, x + width)) return true;
	
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
