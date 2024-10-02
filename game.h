#pragma once

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "gamedata.h"

#define ESCAPE 'Q'
#define ENTER 'E'
#define EOF_GAME 'Z'

#define KEY_WAIT 50
#define ERASE_WAIT 5
#define CHAR_WAIT 50
#define GAME_WAIT 2000
#define LEVEL_WAIT 1500

class Game;
void Quit(Game *game);
char getKeyStroke();

class Game { 
	public:
	Map * map;
	
	int level=0;
	Game(int lvl) {
		level = lvl;
		try {
			map = new Map(25, 10);	
		} catch (const char error) {
			delete [] map;
			throw "Failed to initialize game: " + error;
		}
	}

	~Game() {
		delete [] map;
	}

	//==================
	// GAME MECHANICS
	//==================
	
	void Message(std::wstring in) {
		map->Message(in);
	}

	int getStatus() {
		return map->game_status;
	}

	bool Loop() {
		while (true) {
		
			char key;

			// Gets regular keys if player still playing game
			if (map->game_status == PLAYING){
				key = ::getKeyStroke();
			} else {
				key = EOF_GAME;	// Signifies end of game
			}
	
			switch (key) {
				case ESCAPE: 
					return false;
					break;
				case 'N':
					break;
				case 'W': 
					map->Move(0, -1);
					break;
				case 'S':
					map->Move(0, 1);
					break;
				case 'A':
					map->Move(-1, 0);
					break;
				case 'D':
					map->Move(1, 0);
					break;
				case EOF_GAME:
					return true;
					break;
		}
		Sleep(KEY_WAIT);
	}
}
	
};

char getKeyStroke() {
	// Returns key pressed
	if (GetAsyncKeyState('W') & 0x8000) return 'W'; 

	if (GetAsyncKeyState('A') & 0x8000) return 'A';

	if (GetAsyncKeyState('S') & 0x8000) return 'S';

	if (GetAsyncKeyState('D') & 0x8000) return 'D'; 

	if (GetAsyncKeyState(VK_RETURN) & 0x8000) return ENTER; 

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return ESCAPE;

	return 'N';
}

void Quit(Game *game) {
	// User quits game
	system("cls");	
	game->map->CursTop();
	game->map->showCurs();
}

void UpdateGame(const int status, std::wstring * message, int *level) {
	switch (status) {
		case WIN:
			*message = L"Good job player!\nIt only get's...\nharder... from here!";
			++(*level); // Add a level
			break;
		case LOST:
			*message = L"Better luck next time!\nTry not, well...\n...die?";
			if (level-1 > 0) { 
				--(*level);
			}
			break;
	}
}

void Dialogue(std::wstring msg, const int WAIT, const bool erase) {
	// Display message
	for (unsigned int i=0; i < msg.length(); i++) {
		std::wcout << msg[i];
		if (msg[i] == '.' || msg[i] == '\n'){
			Sleep(CHAR_WAIT*2);
		} else {
			Sleep(CHAR_WAIT);
		}
	}
	Sleep(WAIT);
	
	if (erase == false) return;

	// Erases Message
	for (unsigned int i=msg.length()-1; i >= 0; i--) {
		if (msg[i] == '\n') {
			std::wcout << ESC << UP;
		}
		std::wcout << "\b \b";
		Sleep(ERASE_WAIT);
	}
}
