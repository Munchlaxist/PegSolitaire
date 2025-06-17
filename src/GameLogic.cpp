#include <utility>
#include <array>
#include "Field.h"
#include <stdexcept>
#include "GameLogic.h"


GameLogic::GameLogic() {
	// Initialize the game logic with the default board state
	std::size_t index = 0;
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoard[row][col] == 1) {
				m_board[index].setState(FieldState::Occupied);
				m_board[index].setPosition(std::make_pair(row, col));
			}
			else if (defaultBoard[row][col] == 0) {
				m_board[index].setState(FieldState::Empty);
				m_board[index].setPosition(std::make_pair(row, col));
			}
			else {
				continue; // Skip invalid fields
			}
			m_board[index].setPosition(std::make_pair(row, col));
			++index;
		}
	}
}

std::array<Field, 33>& GameLogic::getBoard() {
	return m_board;
}

GameState& GameLogic::getCurrentGameState() {
	return m_gameState;
}

void GameLogic::setGameState(GameState state) {
	m_gameState = state;
}

Field& GameLogic::getField(std::pair<int, int> position) {
	for (Field& field : m_board) {
		if (field.getPosition() == position) {
			return field;
		}
	}
	throw std::runtime_error("Field not found at the given position (" + position.first + ',' + position.second + ')');
}

bool GameLogic::isValidMove(Field& selectedField, Field& field) {
	std::pair<int, int> selectedFieldPosition = selectedField.getPosition();
	std::pair<int, int> nextFieldPosition = field.getPosition();

	// Check which direction the move is going and check if the move is valid
	if (std::get<0>(selectedFieldPosition) - 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump up; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) - 1, std::get<1>(selectedFieldPosition)));
		if (jumpedOverField.getState() == FieldState::Occupied && field.getState() == FieldState::Empty) {
			return true;

		}
	}
	else if (std::get<0>(selectedFieldPosition) + 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump down; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) + 1, std::get<1>(selectedFieldPosition)));
		if (jumpedOverField.getState() == FieldState::Occupied && field.getState() == FieldState::Empty) {
			return true;


		}
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) - 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the left; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) - 1));
		if (jumpedOverField.getState() == FieldState::Occupied && field.getState() == FieldState::Empty) {
			return true;

		}
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) + 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the right; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) + 1));
		if (jumpedOverField.getState() == FieldState::Occupied && field.getState() == FieldState::Empty) {
			return true;

		}
	}
	return false;
}

void GameLogic::makeMove(Field& selectedField, Field& field) {
	std::pair<int, int> selectedFieldPosition = selectedField.getPosition();
	std::pair<int, int> nextFieldPosition = field.getPosition();

	if (std::get<0>(selectedFieldPosition) - 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump up - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) - 1, std::get<1>(selectedFieldPosition)));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedFieldPosition) + 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump down - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) + 1, std::get<1>(selectedFieldPosition)));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) - 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the left - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) - 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) + 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the right - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) + 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
	}
}

bool GameLogic::movesAvailable() {
	for (Field& selectedField : m_board) {
		if (selectedField.getState() == FieldState::Occupied) {
			for (Field& field : m_board) {
				if (field.getState() == FieldState::Empty) {
					if (isValidMove(selectedField, field)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool GameLogic::solutionFound() {
	for (Field& field : m_board) {
		if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(3, 3)) {
			return false; // If any field is still occupied besides the one in the center, the solution is not found
		}
	}
	return true;
}

void GameLogic::resetGame() {
	std::size_t index = 0;
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoard[row][col] == 1) {
				m_board[index].setState(FieldState::Occupied);
			}
			else if (defaultBoard[row][col] == 0) {
				m_board[index].setState(FieldState::Empty);
			}
			else {
				continue; // Skip invalid fields
			}
			m_board[index].setPosition(std::make_pair(row, col));
			++index;
		}
	}
}
