#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "gamedata.h"

char getKeyStroke();
bool gameLoop (Map map);

int main () {
	system("cls");

	std::setlocale(LC_ALL, "");  // Explicitly set to UTF-8 locale

	// Initializes MAP
	Map *map=nullptr;
	try {
		map = new Map(25, 10);
		
		if (!gameLoop(*map)){
			// User quits game
			map->CursBottom();
			map->showCurs();
			delete[] map;
			return 0;
		}

	} catch (const char* err) {
	 	//=======================
		// ERROR HANDLING
		//=======================
		std::cerr << err << std::endl;
		delete[] map;
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
		char key = getKeyStroke();
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
		}

		Sleep(50);
	}
}
