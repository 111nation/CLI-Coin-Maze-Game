#pragma once 

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//=======================
// CURSER DATA
//=======================
#define ESC "\033["

// DIRECTION
#define UP 'A'
#define DOWN 'B'
#define LEFT 'D'
#define RIGHT 'C'

//=======================
// ARRAY MAP DATA
//=======================
#define VWALL 0
#define HWALL 1
#define PLAYER 2
#define COIN 3
#define SPACE 4
#define DOOR 5
#define MINE 6

//=======================
// ARRAY ROOM DATA
//=======================
#define OCCUPIED 7
#define UNOCCUPIED 8
#define RWALL 9
#define RDOOR 10

//=======================
// ARRAY PATH DATA
//=======================
#define ROOM_VISITED 0
#define ROOM_UNVISITED 1
#define ROOM_WALL 2

// PLAYER SPAWN
#define SPAWNX 1
#define SPAWNY 1


class Player {
	public: 
	int x, y; 
	Player() { 
		x = 0;
		y = 0;
	}
};

class Map { 
	private:
    	//=======================
	// MAP DETAILS
    	//=======================
    	// DIMENSIONS
	int width, height; 
    	int** arrMap;
	// COINS
	int coins = 0; // number of coins to collect 
	int coinsLeft = 0; // number of coins left
	// MINES
	int mines=0;
	// ROOMS
	int** arrRoom;
	// PATHFINDING
	int** PATH = new int * [height+2];

    	//=======================
    	// MAP DISPLAY
    	//=======================
	const wchar_t vwall = L'█';
    	const wchar_t hwall = L'-';	
	const wchar_t space = L' ';
	const wchar_t coin = L'C';
	const wchar_t mine = L'*';
	
    	//=======================
	//PLAYER
    	//=======================
	Player Player;
	const wchar_t cplayer = L'V';

    	//=======================
    	// CURSER DETAILS
    	//=======================
	// CURSER POSITION
	int ycurs, xcurs;

	//CURSER DETAILS
	HANDLE hConsoleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorProp = {}; // Stores properties of cursor (Visibility and Size)

	public:
    
	Map(int x, int y);
    	~Map();

	//=======================
	// MINES
	//=======================
	void InitMines();
	void placeMines(int amount_of_mines, int * placed_mines);
	void deleteBlockingMines(int * visitedSpaces, int * total_mines);

    	//=======================
    	// ROOMS
    	//=======================
	void InitRoom();
	bool RoomGen();
	
	// WALL SHARING
	void adjustWalls(int * y, int * x, int * room_width, int * room_height);
	bool WallFoundx(const int starty, const int startx, const int wall_width); 
	bool WallFoundy(const int starty, const int startx, const int wall_height);

	// ROOM CREATION
	bool willRoomFit(int starty, int startx, int rwidth, int rheight);
	bool isWall(int y, int x); 
	void CreateRoom(int starty, int startx, int rwidth, int rheight);
	int calcRoomProb(int y, int x);
	
	// DOORS
	bool createDoor(int y, int x, int ** PATH);
	void resetPATH(int ** PATH, int * visitedSpaces);
	bool isDoorPossible(const bool direction, int starty, int startx, int**PATH);
	void floodFill(int y, int x, int**PATH, int*visitedSpaces);
	void DoorGen();
	
    	//=======================
    	// ARRAY INITIALIZATION
    	//=======================
    	bool CoinGen(); // Return true if all coins generated
	void MapGen();

    	//=======================
	// CURSER CONTROLS
    	//=======================
    	void CursReturn();
    	void CursTop();
    	void CursBottom();
    	void CursPlayer();
	void MovCurs(int x, int y); // Custom moving

    	// UTILITY
	void hideCurs();
	void showCurs();

    	//=======================
	// UPDATING SCREEN
    	//=======================
	void DrawNew();
	
	// MESSAGE (BOTTOM)
	int msg_lines = 0;
	void Message(std::string msg);
	void ClearMessage();
    	
	//=======================
	// PLAYER MOVEMENT
    	//=======================
	void Move(int x, int y);
	void pickCoin();

	//=======================
	// UTILITIES
	//=======================
	wchar_t getChar(int y, int x);
	int getObject(int y, int x);

	bool OnBoundary(int y, int x);
	bool OnBoundary(int y, int x, int pheight, int pwidth);
	bool OutOfBounds(int y, int x);
	bool OutOfBounds(int y, int x, int pheight, int pwidth);

	int ObjCount(int ** ARR, const int obj);
	bool isBlocking(int y, int x, int ** PATH);
};
