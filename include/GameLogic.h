#pragma once
#include <array>
#include <utility>
#include "Field.h"
#include <stack>
#include "Move.h"

/**
	Describes the possible states of the game.
	- Playing: The game is ongoing and the player can make moves.
	- GameWon: The player has successfully completed the game by leaving only one peg in the center position.
	- GameLost: The player has no valid moves left and is game over.
*/
enum class GameState {
	Playing,
	GameWon,
	GameLost,
};

/**
	Implementation of the game logic for the Peg Solitaire game.
*/
class GameLogic {
private:
	std::array<Field, 33> m_board{}; // Represents the board (33 fields in total) with 7x7-grid positions
	std::stack<Move> m_moveHistory{}; // Stack to keep track of moves made during the game for undo functionality
	GameState m_gameState{ GameState::Playing }; // Current state of the game
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
		Gets the current move history of the game.
		\return A reference to the stack containing the moves made during the game
	*/
	std::stack<Move>& getMoveHistory();

	/**
		Gets the current state of the game.
		\return The current game state (Playing, GameWon, GameLost)
	*/
	GameState& getCurrentGameState();

	/**
		Sets the current state of the game.
		\param state The new game state to set
	*/
	void setGameState(GameState state);

	/**
		Gets the field at a specific position on the board.
		\param position The position of the field on the board as a pair of integers (row, column)
		\return A reference to the field at the specified position
	*/
	Field& getField(std::pair<int, int> position);

	/**
		Checks if a move from selectedField to field is valid.
		\param selectedField The field from which the move is attempted (the selected field)
		\param field The field to which the move is attempted (the target field)
		\return True if the move is valid, false otherwise
	*/
	bool isValidMove(Field& selectedField, Field& field); 

	/**
		Makes a move from selectedField to field and updates the board accordingly.
		\param selectedField The field from which the move is made (the selected field)
		\param field The field to which the move is made (the target field)
	*/
	void makeMove(Field& selectedField, Field& field);

	/**
		Undoes the last move made in the game, restoring the previous state of the board.
	*/
	void undoMove();

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
		Converts the current board state into a format suitable for the PegSolitaireSolver.
		\return A 7x7 array representing the board, where -1 indicates an invalid field, 0 indicates an empty field and 1 indicates a n occued field
	*/
	std::array<std::array<int, 7>, 7>& convertBoardToSolverBoardFormat();

	/**
		Resets the game to its initial state, restoring the default board configuration.
	*/
	void resetGame();
};