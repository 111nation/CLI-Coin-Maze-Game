@echo off

g++ -c game.cpp
g++ -c gamedata.cpp
g++ -c roomdata.cpp
g++ -c coindata.cpp
g++ -c minedata.cpp
g++ -c outputdata.cpp
g++ -c display.cpp
g++ display.o game.o gamedata.o roomdata.o coindata.o minedata.o outputdata.o -o "Game.exe"

if exist *.o (
	del *.o 
)

if exist "Game.exe" (
	 .\Game.exe
)

@echo on
