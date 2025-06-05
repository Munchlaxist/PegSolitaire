#include "GameLogic.h"
#include "UserInterface.h"

int main()
{
	GameLogic gameLogic{}; // Initialize the game logic
	UserInterface ui(gameLogic);
	ui.gameLoop();
}