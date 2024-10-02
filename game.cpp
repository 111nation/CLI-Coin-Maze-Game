#include "game.h"
#include <sstream>

int main () {
	system("cls");

	std::setlocale(LC_ALL, "");  // Explicitly set to UTF-8 locale
	
	// Initialize game
	// INTRO
	std::wstringstream wss;
	wss << L"You, Player, have heard that in a faraway island lay treasure...\n"
			<< L"but... this treasure has been scattered around a forbidden temple yet to discover..."
			<< L"\n...\n...\n" 
			<< L"\"Are you able to take the challange player\"?"
		        << L"\n\n\n\nPress ENTER to continue...";	
	std::wstring msg = wss.str();
	Dialogue(msg, 0, false); 
	wss.str(L"");	

	bool continue_game=false;
	while(continue_game==false) {
		if (getKeyStroke() == ENTER) {
			continue_game = true;
		}
	}

	system("cls");
	int level = 1;
	wss << L"Good Luck!";
	msg = wss.str();
	wss.str(L"");
	while (true) {
		Sleep (600);
		system("cls");
		wss << "\n\n\nLEVEL: " << level;
		Dialogue(wss.str(), LEVEL_WAIT, false);
		wss.str(L"");
		system("cls");
				
		Game *game=nullptr;
		try {
			game = new Game(level);
			// Game Loop
			if (game->Loop() == false) {
				// Exit Game
				Quit(game);
				return 0;
			}
		
			// Determine how user ended level
			system("cls");

			UpdateGame(game->getStatus(), &msg, &level);
			Dialogue(msg, GAME_WAIT, false);
		} catch (const char * error[]) {
			wss.str(L"");
			wss << L"An error occured:\n\"" << *error << "\"\n\nAttempting to retry...";		
			Dialogue(wss.str(), GAME_WAIT, false);
			wss.str(L"");
		}
		game->~Game();
		delete [] game;
	}

	return 0;	
}
