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

Field& GameLogic::getField(std::pair<int, int> position) {
	for (Field& field : m_board) {
		if (field.getPosition() == position) {
			return field;
		}
	}
	throw std::runtime_error("Field not found at the given position (" + position.first + ',' + position.second + ')');
}

bool GameLogic::isValidMove(Field& selectedPeg, Field& peg) {
	std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
	std::pair<int, int> nextPegPosition = peg.getPosition();

	// Check which direction the move is going and check if the move is valid
	if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Then we jump up; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true;

		}
	}
	else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Then we jump down; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true;


		}
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
		// Then we jump to the left; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true;

		}
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
		// Then we jump to the right; check if there is an occupied field in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) + 1));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true;

		}
	}
	return false;
}

void GameLogic::makeMove(Field& selectedPeg, Field& peg) {
	std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
	std::pair<int, int> nextPegPosition = peg.getPosition();

	if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Then we jump up - set the new states of the fields accordingly
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Then we jump down - set the new states of the fields accordingly
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
		// Then we jump to the left - set the new states of the fields accordingly
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
		// Then we jump to the right - set the new states of the fields accordingly
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) + 1));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
}

bool GameLogic::movesAvailable() {
	for (Field& selectedPeg : m_board) {
		if (selectedPeg.getState() == FieldState::Occupied) {
			for (Field& peg : m_board) {
				if (peg.getState() == FieldState::Empty) {
					if (isValidMove(selectedPeg, peg)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool GameLogic::solutionFound() {
	for (Field& peg : m_board) {
		if (peg.getState() == FieldState::Occupied && peg.getPosition() != std::pair<int, int>(3, 3)) {
			return false; // If any peg is still occupied besides the one in the center, the solution is not found
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
