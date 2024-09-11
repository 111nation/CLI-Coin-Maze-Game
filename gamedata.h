#pragma once 

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define ESC "\033["

// DIRECTION
#define UP 'A'
#define DOWN 'B'
#define LEFT 'D'
#define RIGHT 'C'

//=======================
// ARRAY MAP DATA
//=======================
#define WALL 0
#define PLAYER 1
#define COIN 2
#define SPACE 3

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
    int** arrMap = new int*[height+2];


    //=======================
    // MAP DISPLAY
    //=======================
	const wchar_t vwall = L'█';
    const wchar_t hwall = L'-';	
	const wchar_t space = L'0';
	
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
    // ARRAY INITIALIZATION
    //=======================
    void ArrInit();

    //=======================
	// CURSER CONTROLS
    //=======================
    void CursReturn();
    void cursTop();
    void cursBottom();
    void CursPlayer();
    void MovCurs(int x, int y); // Custom moving

    // UTILITY
	void hideCurs();
	void showCurs();

    //=======================
	// UPDATING SCREEN
    //=======================
	void DrawNew();
	wchar_t getChar(int y, int x);

    //=======================
	// PLAYER MOVEMENT
    //=======================
	void Move(int x, int y);

};
