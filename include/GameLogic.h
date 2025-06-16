#pragma once
#include <array>
#include <utility>
#include "Field.h"


/**
	Implementation of the game logic for the Peg Solitaire game.
*/
class GameLogic {
private:
	std::array<Field, 33> m_board{}; // Represents the board (33 fields in total) with 7x7-grid positions
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard{ {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	} }; // Describes the initial state of the board when represented as a 7x7 grid
public:

	/**
		Constructor for the GameLogic class, where the board is initialized with the default board state.
	*/
	GameLogic();

	/**
		Gets the current state of the board.
		\return A reference to the array representing the board as fields
	*/
	std::array<Field, 33>& getBoard();

	/**
		Gets the field at a specific position on the board.
		\param position The position of the field on the board as a pair of integers (row, column)
		\return A reference to the field at the specified position
	*/
	Field& getField(std::pair<int, int> position);

	/**
		Checks if a move from selectedPeg to peg is valid.
		\param selectedPeg The field from which the move is attempted (the selected field)
		\param peg The field to which the move is attempted (the target field)
		\return True if the move is valid, false otherwise
	*/
	bool isValidMove(Field& selectedPeg, Field& peg); 

	/**
		Makes a move from selectedPeg to peg and updates the board accordingly.
		\param selectedPeg The field from which the move is made (the selected field)
		\param peg The field to which the move is made (the target field)
	*/
	void makeMove(Field& selectedPeg, Field& peg); 


	/**
		Checks if there are any available moves left on the board.
		\return True if there are moves available, false otherwise
	*/
	bool movesAvailable();

	/**
		Checks if the game is solved, meaning the only peg left on the board is the one in the center position.
		\return True if the game is solved, false otherwise
	*/
	bool solutionFound();

	/**
		Resets the game to its initial state, restoring the default board configuration.
	*/
	void resetGame();
};