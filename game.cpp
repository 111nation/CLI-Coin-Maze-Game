#include "game.h"
#include <sstream>

int main () {
	system("cls");

//	SetConsoleOutputCP(CP_UTF8);
	std::setlocale(LC_ALL, "");  // Explicitly set to UTF-8 locale
	
	// Initialize game
	Game *game=nullptr;
	// Initialize game
	// INTRO
	std::wstringstream wss;
	wss << L"Setting: Jungle, Unknown...\n"
			<< L"\n\"Dear Diary - 10/03/1999:\n"
			<< L"I have trekked for nearly a month and finally... I found the lost temple...\n" 
			<< L"I see coins all around here, they look so... shiny...\n"
			<< L"This temple is a long labyrinth of coins and mines, I should watch my step...\n"
			<< L"\nIt is now or never...\""
			<< L"\n(You don't see an exit out...)\n"
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
	wss << L"Let's do this...";
	Dialogue(wss.str(), LEVEL_WAIT, true);
	wss.str(L"");
	while (true) {
		Sleep (600);
		system("cls");
		wss << "\n\n\nROOM: " << level;
		Dialogue(wss.str(), LEVEL_WAIT, false);
		wss.str(L"");
		system("cls");
		
		game = nullptr;		
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
			Sleep(LEVEL_WAIT / 2);
			Dialogue(msg, GAME_WAIT, false);
		} catch (const char * error[]) {
			wss.str(L"");
			wss << L"An error occured:\n\"" << *error << "\"\n\nAttempting to retry...";		
			Dialogue(wss.str(), GAME_WAIT, false);
			wss.str(L"");
		}
		delete game;
	}

	return 0;	
}
