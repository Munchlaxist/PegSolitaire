#include <utility>
#include <array>
#include "Field.h";
#include "GameLogic.h";


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
				continue; // skip unused fields
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
	/**
	* get the field at the given position, i.e. row and column
	*/
	for (Field& field : m_board) {
		if (field.getPosition() == position) {
			return field;
		}
	}
	//throw an exception if there is no field with the given position
	//throw std::runtime_error("Field not found at the given position");
}

bool GameLogic::isValidMove(Field& selectedPeg, Field& peg) {
	/**
	* check if the next move is valid, i.e. the selected peg can jump over another peg and land in an empty space given by peg.
	*/
	std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
	std::pair<int, int> nextPegPosition = peg.getPosition();

	if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// then we jump up, check if there is an occupied peg in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true; // Valid move

		}
	}
	else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true; // Valid move


		}
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true; // Valid move

		}
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) + 1));
		if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
			return true; // Valid move

		}

	}
	return false;
}

void GameLogic::makeMove(Field& selectedPeg, Field& peg) {
	std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
	std::pair<int, int> nextPegPosition = peg.getPosition();

	if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// then we jump up, check if there is an occupied peg in between. If so the move is valid
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
		Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
		selectedPeg.setState(FieldState::Empty);
		peg.setState(FieldState::Occupied);
		jumpedOverPeg.setState(FieldState::Empty);
	}
	else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
		// Check if there is a peg in between
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
			return false; // If any peg is still occupied, the solution is not found
		}
	}
	return true;
}