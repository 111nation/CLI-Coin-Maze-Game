@echo off

if exist "Game.exe" (
	del "Game.exe"
)

g++ -c game.cpp
g++ -c gamedata.cpp
g++ -c roomdata.cpp
g++ -c coindata.cpp
g++ game.o gamedata.o roomdata.o coindata.o -o "Game.exe"

if exist *.o (
	del *.o 
)

@echo on
echo .
echo Run the game from Game.exe
