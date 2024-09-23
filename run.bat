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
g++ game.o gamedata.o roomdata.o coindata.o -o "exec\Game.exe"

if exist "exec\Game.exe" (
	del *.o
	cls 
	"exec\Game.exe"
)
