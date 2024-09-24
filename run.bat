@echo off

g++ -c game.cpp
g++ -c gamedata.cpp
g++ -c roomdata.cpp
g++ -c coindata.cpp
g++ game.o gamedata.o roomdata.o coindata.o -o "Game.exe"

if exist *.o (
	del *.o 
)

if exist "Game.exe" (
	 .\Game.exe
)

@echo on
