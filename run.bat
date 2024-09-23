@echo off

if exist *.o (
	del *.o 
)

g++ -c game.cpp
g++ -c gamedata.cpp
g++ -c roomdata.cpp
g++ -c coindata.cpp
g++ game.o gamedata.o roomdata.o coindata.o -o "Game.exe"

if exist "Game.exe" (
	del "Game.exe"
)

@echo on
echo .
echo Run the game from Game.exe
