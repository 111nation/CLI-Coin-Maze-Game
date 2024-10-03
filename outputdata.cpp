#include "gamedata.h"
#include <sstream>

void Map::Status() {
	
	// CURSER PLACEMENT
	int oldy = ycurs;
	int oldx = xcurs;

	CursTop();
	std::wcout << '\r';

	ClearStatus();
	
	std::wstringstream wss;
	
	// ACTUAL MESSAGE
	wss << L"HEALTH:\t";
	for (int i=1; i <= Player.health; i++) {
		wss << heart;
	}
		
	wss << L"\nCOINS:\t";
	wss << coinsLeft;
	std::wstring msg = wss.str();
	std::wcout << msg;
	wss.str(L"");
	
	// COUNTS MESSAGE LINES
	status_lines = 1;
	for (unsigned int i = 0; i < msg.length(); i++) {
		if (msg[i] == '\n') {
			++status_lines;
		}
	}
	++status_lines;
	CursTop();
	
	//==========================
	// CURSER HANDLING
	//==========================
	// UPDATES CURSER POS (Moves back into original positon)
	std::wcout << '\r';
	std::wcout << ESC << status_lines << DOWN; 
	xcurs = -1;
	ycurs = -1;

	MovCurs(oldx-xcurs, oldy-ycurs);
	ycurs = oldy;
	xcurs = oldx;
}

void Colour(); 

void Map::ClearStatus() {
	CursStatus();

	if (status_lines <= 0) return;

	std::cout << '\r';

	for (int line=0; line < status_lines; line++) {
		std::wcout << ESC << CLEAR_LINE;
		std::wcout << ESC << DOWN;
	}
	
	CursStatus();
	status_lines = 0;
}


// ==============================
// BOTTOM MESSAGE BAR
// ==============================
void Map::ClearMessage() {
		CursBottom();

		// CLEARS PREV MESSAGE
		if (msg_lines > 0) {
			std::wcout << '\r';

			MovCurs(0,msg_lines);
			for (int i = msg_lines; i > 0; i--) {
				std::wcout << ESC << "2K";
			}		
			MovCurs(0,-msg_lines); 	
		}

		msg_lines = 0;
}

void Map::Message(std::wstring msg) { 
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
	
		ClearMessage();

		// DISPLAY MESSAGE
		msg_lines = newMsg_lines;
		std::wcout << '\r'<< msg;

		//==========================
		// CURSER HANDLING
		//==========================
		// UPDATES CURSER POS
		std::wcout << '\r';
		xcurs = -1;
		ycurs = ((height + 2) + msg_lines) - 1;
		
		// BACK TO TOP
		CursTop(); 

		// BELOW STATUS BAR
		std::wcout << ESC << status_lines << DOWN;

		MovCurs(oldx+1, oldy+1);
		ycurs = oldy;
		xcurs = oldx;
}

//=======================
// CURSER CONTROLS
//=======================
void Map::CursStatus() {
	CursReturn();
	if (status_lines <= 0) return;
	std::cout << ESC << status_lines << UP;
}

void Map::CursReturn() {
		// Returns curser to beginning
		std::wcout << '\r' << ESC << height+2 << UP;
		ycurs = -1;
		xcurs = -1;	
}

void Map::CursPlayer() {
		// Returns curser to player
	
		CursTop();
		std::wcout << ESC << Player.y+1 << DOWN;

		std::wcout << ESC << Player.x+1 << RIGHT;

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
	std::wcout << ESC << height + 2 + status_lines << DOWN;
	ycurs = height + 1;
}

void Map::CursTop() {
	std::wcout << ESC << ycurs + 1 << UP; 
	ycurs = -1;
}

void Map::MovCurs(int x, int y) {
		// Y Movement
		if (y != 0) {
			if (y > 0) {
				std::wcout << ESC << y << DOWN;
			} else {
				std::wcout << ESC << -y << UP;
			}
		}

		// X Movement
		if (x != 0) {
			if (x > 0) {
				std::wcout << ESC << x << RIGHT;
			} else {
				std::wcout << ESC << -x << LEFT;
			}
		}		
}

//=======================	
// UPDATING SCREEN
//=======================
void Map::DrawNew() {
		CursReturn();
		std::wcout << ESC << status_lines << DOWN;
		
		for (ycurs = -1; ycurs <= height; ycurs++) { 
			for (xcurs = -1; xcurs <= width; xcurs++) {
				std::wcout << getChar(ycurs + 1, xcurs + 1); 
			}
			std::wcout<<'\n';
		}	

		CursPlayer();
}



