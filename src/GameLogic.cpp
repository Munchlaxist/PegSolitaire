#include <utility>
#include <array>
#include "Field.h"
#include <stdexcept>
#include "GameLogic.h"
#include "Move.h"


GameLogic::GameLogic() {
	// Initialize the game logic with the default board state
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoardEnglish[row][col] == 1) {
				Field field{ FieldState::Occupied, std::make_pair(row, col) };
				m_board.push_back(field);
			}
			else if (defaultBoardEnglish[row][col] == 0) {
				Field field{ FieldState::Empty, std::make_pair(row, col) };
				m_board.push_back(field);
			}
			else {
				continue; // Skip invalid fields
			}
		}
	}
}

std::vector<Field>& GameLogic::getBoard() {
	return m_board;
}

std::stack<Move>& GameLogic::getMoveHistory() {
	return m_moveHistory;
}

GameState& GameLogic::getCurrentGameState() {
	return m_gameState;
}

void GameLogic::setGameState(GameState state) {
	m_gameState = state;
}

BoardType& GameLogic::getBoardType() {
	return m_boardType;
}

void GameLogic::setBoardType(BoardType type) {
	m_boardType = type;
}

void GameLogic::initializeEnglishBoard() {
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoardEnglish[row][col] == 1) {
				Field field{ FieldState::Occupied, std::make_pair(row, col) };
				m_board.push_back(field);
			}
			else if (defaultBoardEnglish[row][col] == 0) {
				Field field{ FieldState::Empty, std::make_pair(row, col) };
				m_board.push_back(field);
			}
			else {
				continue; // Skip invalid fields
			}
		}
	}
}

void GameLogic::initializeEuropeanBoard() {
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoardEuropean[row][col] == 1) {
				m_board.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
			}
			else if (defaultBoardEuropean[row][col] == 0) {
				m_board.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
			}
			else {
				continue; // Skip invalid fields
			}
		}
	}
}

void GameLogic::initializeSmallDiamondBoard() {
	for (std::size_t row = 0; row < 8; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoardSmallDiamond[row][col] == 1) {
				m_board.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
			}
			else if (defaultBoardSmallDiamond[row][col] == 0) {
				m_board.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
			}
			else {
				continue; // Skip invalid fields
			}
		}
	}
}

void GameLogic::initializeAsymmetricBoard() {
	for (std::size_t row = 0; row < 8; ++row) {
		for (std::size_t col = 0; col < 8; ++col) {
			if (defaultBoardAsymmetric[row][col] == 1) {
				m_board.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
			}
			else if (defaultBoardAsymmetric[row][col] == 0) {
				m_board.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
			}
			else {
				continue; // Skip invalid fields
			}
		}
	}
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
		m_moveHistory.push(Move(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) + 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump down - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) + 1, std::get<1>(selectedFieldPosition)));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(Move(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) - 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the left - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) - 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(Move(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) + 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the right - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) + 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(Move(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
}

void GameLogic::undoMove() {
	if (!m_moveHistory.empty()) {
		Move lastMove = m_moveHistory.top();
		m_moveHistory.pop();
		Field& selectedField = getField(lastMove.from);
		Field& jumpedOverField = getField(lastMove.over);
		Field& nextField = getField(lastMove.to);
		selectedField.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Occupied);
		nextField.setState(FieldState::Empty);
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
	switch (m_boardType) {
	case BoardType::English:
	case BoardType::SmallDiamond:
		for (Field& field : m_board) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(3, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		break;
	case BoardType::European:
		for (Field& field : m_board) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(0, 4)) {
				return false; // If any field is still occupied besides the one in the bottom right corner, the solution is not found
			}
		}
		break;
	case BoardType::Asymmetric:
		for (Field& field : m_board) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(4, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		break;
	default:
		break;
	}
	return true;
}

std::array<std::array<int, 7>, 7>& GameLogic::convertBoardToSolverBoardFormat() {
	std::array<std::array<int, 7>, 7> solverBoard = {}; // Initialize a 7x7 board with all -1
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			solverBoard[row][col] = -1; 
		}
	}

	for (Field& field : m_board) {
		std::pair<int, int> position = field.getPosition();
		if (field.getState() == FieldState::Occupied || field.getState() == FieldState::Selected) {
			solverBoard[std::get<0>(position)][std::get<1>(position)] = 1; // Occupied fields are represented by 1, Selected can only be occupied
		}
		else {
			solverBoard[std::get<0>(position)][std::get<1>(position)] = 0; // Empty fields are represented by 0
		}
	}
	return solverBoard;
}

void GameLogic::resetGame() {
	m_gameState = GameState::Playing; // Reset the game state to playing
	m_moveHistory = std::stack<Move>(); // Reset the move history for the new game
	m_board.clear();

	switch (m_boardType) {
	case BoardType::English:
		initializeEnglishBoard();
		break;
	case BoardType::European:
		initializeEuropeanBoard();
		break;
	case BoardType::Asymmetric:
		initializeAsymmetricBoard();
		break;
	case BoardType::SmallDiamond:
		initializeSmallDiamondBoard();
		break;
	default:
		break;
	}
}
