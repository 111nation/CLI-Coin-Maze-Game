#include "display.h"

Console::Console () {
	// Gets size of console
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Assigns console
	
	const char * console_error = "Erorr fetching console window";
	if (hConsole == nullptr) {
		throw console_error;
	}
	
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		throw console_error;
	}
}

void Console::CursPos() {
	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);	
}	

void Console::Colour(int fgcolour, int bgcolour) {
	//CursPos();
	// Set the text color attributes
	SetConsoleTextAttribute(hConsole, fgcolour | bgcolour);
}

void Console::ResetColour() {
	
	SetConsoleTextAttribute(hConsole, csbi.wAttributes);
}
