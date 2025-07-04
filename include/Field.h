#pragma once
#include <utility>

/**
	Describes the state of a field in the Peg Solitaire game.
	- Empty: There is no peg in the field
	- Occupied: There is a peg in the field
	- Selected: The peg in the field is selected for a move
*/
enum class FieldState {
	Empty,
	Occupied,
	Selected,
};

/**
	Represents a field in the Peg Solitaire game.
*/
class Field {
private:
	FieldState m_state{}; // describes the state of the field
	std::pair<int, int> m_position{}; // position of the field within the game board when represented as a 7x7 grid

public:
	Field() = default;
	Field(FieldState state, std::pair<int, int> position);

	/**
		Gets the state of the field, i.e. whether it is empty, occupied, or selected.
		\return The current state of the field
	*/
	const FieldState& getState() const;

	/**
		Gets the position of the field within the game board when represented as a 7x7 grid.
		\return The position of the field as a pair of integers (row, column)
	*/
	std::pair<int, int> getPosition() const;

	/**
		Sets the state of the field.
		\param state The new state to set for the field
	*/
	void setState(const FieldState& state);

	/**
		Sets the position of the field wihin the game board when represented as a 7x7 grid.
		\param position The new position to set for the field as a pair of integers (row, column)
	*/
	void setPosition(const std::pair<int, int>& position);
};