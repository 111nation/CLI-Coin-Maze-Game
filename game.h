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
#define CHAR_WAIT 40
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
		if (lvl <= 0) throw "Level invalid!";

		level = lvl;
		try {
			map = new Map(Width(), Height());	
		} catch (const char error) {
			delete [] map;
			throw "Failed to initialize game: " + error;
		}
	}

	~Game() {
		delete map;
	}

	//==================
	// GAME MECHANICS
	//==================
	int Width() {
		// Determines the width of the room
		int width = rand() % (MAX_WIDTH - MIN_WIDTH + 1) + MAX_WIDTH;
		width = std::round(width * (0.05 * level));
		
		if (width < MIN_WIDTH) {
			width = MIN_WIDTH;
		} else if (width > MAX_WIDTH) {
			width = MAX_WIDTH;
		}

		return width;
	}

	int Height() {
		// Determines the height of the room
		int height = rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MAX_HEIGHT;	
		height = std::round(height * (0.2 * level));

		if (height < MIN_HEIGHT) {
			height = MIN_HEIGHT;
		} else if (height > MAX_HEIGHT) {
			height = MAX_HEIGHT;
		}

		return height;
	}
	
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
			*message = L"I survived only this room...\nIt only gets...\nharder...\nfrom here...";
			++(*level); // Add a level
			break;
		case LOST:
			*message = L"Ouch!\nI should try not, well...\n...\nstep on mines?\nLet's do this again...";
			if (*level-1 > 0) { 
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
			Sleep(CHAR_WAIT*4);
		} else {
			Sleep(CHAR_WAIT);
		}
	}
	Sleep(WAIT);
	
	if (erase == false) return;

	// Erases Message
	for (unsigned int i=msg.length(); i > 0; i--) {
		std::wcout << "\b \b";
		Sleep(ERASE_WAIT);
	}
}
