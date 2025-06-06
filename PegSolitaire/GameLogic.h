#pragma once
#include <array>
#include <utility>
#include "Field.h"



class GameLogic {
private:
	std::array<Field, 33> m_board; // only 33 fields are used in the game, the rest is unused and can be ignored
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard{ {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	} };
public:

	GameLogic();

	std::array<Field, 33>& getBoard();

	Field& getField(std::pair<int, int> position);

	bool isValidMove(Field& selectedPeg, Field& peg); 

	void makeMove(Field& selectedPeg, Field& peg); 


	bool movesAvailable();

	bool solutionFound();

	void resetGame();
};