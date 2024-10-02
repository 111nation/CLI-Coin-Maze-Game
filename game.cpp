#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "gamedata.h"

char getKeyStroke();
bool gameLoop (Map map);
int game();

int main () {
	system("cls");

	std::setlocale(LC_ALL, "");  // Explicitly set to UTF-8 locale

	int level=1;
	int game_state = PLAYING;
	while (1) {
		system("cls");

		if (game_state == WIN) {
			++level;
			std::cout << "\n YOU WON! " << "\n\n" << "LEVEL: " << level; 
		} else if (game_state == LOST) {
			std::cout << "\n YOU LOST! " << "\n\n" << "LEVEL: " << level; 
		} else if (game_state == PLAYING) {
			std::cout << "LEVEL: " << level; 
		}


		Sleep(5000);
		system("cls");

		game_state = game();
		// Playes game until user exits
		if (game_state==EXIT) {
			return 0;
		} else if (game_state == WIN) {
			++level;
		} 

		std::cout << game_state;
	}
}

char getKeyStroke() {
	// Returns key pressed
	if (GetAsyncKeyState('W') & 0x8000) return 'W'; 

	if (GetAsyncKeyState('A') & 0x8000) return 'A';

	if (GetAsyncKeyState('S') & 0x8000) return 'S';

	if (GetAsyncKeyState('D') & 0x8000) return 'D'; 

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 'Q';

	return 'N';
}

bool gameLoop (Map map) {
	while (true) {
		
		char key;

		// Gets regular keys if player still playing game
		if (map.game_status == PLAYING){
			key = getKeyStroke();
		} else {
			key = 'E';	// Signifies end of game
		}

		switch (key) {
			case 'Q': 
				return false;
				break;
			case 'N':
				break;
			case 'W': 
				map.Move(0, -1);
				break;
			case 'S':
				map.Move(0, 1);
				break;
			case 'A':
				map.Move(-1, 0);
				break;
			case 'D':
				map.Move(1, 0);
				break;
			case 'E':
				return true;
				break;
		}

		Sleep(50);
	}
}

int game () {
	// Initializes MAP
	Map *map=nullptr;
	try {
		map = new Map(5, 5);
		
		if (!gameLoop(*map)){
			// User quits game
			map->CursBottom();
			map->showCurs();
			delete[] map;
			return EXIT;
		}

		return map->game_status;

	} catch (const char* err) {
	 	//=======================
		// ERROR HANDLING
		//=======================
		Sleep(500);
		std::wcerr << err << std::endl;
		std::wcerr << "\nAttempting to restart";
		delete[] map;
		return GAME_ERROR;
	}
}
