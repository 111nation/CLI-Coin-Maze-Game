@echo off

if exist *.o (
	del *.o 
)

if exist "exec\Game.exe" (
	del "exec\Game.exe"
)

g++ -c game.cpp
g++ -c gamedata.cpp
g++ -c roomdata.cpp
g++ -c coindata.cpp
g++ game.o gamedata.o roomdata.o coindata.o -o "Game.exe"

@echo on
echo .
echo Run the game from Game.exe
