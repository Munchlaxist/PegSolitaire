#include <utility>
#include <array>
#include <cmath>
#include "Field.h"
#include <map>
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

const std::map<std::pair<int, int>, uint8_t> GameLogic::englishGridIdxMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,2), 3}, {std::make_pair(1,3), 4}, {std::make_pair(1,4), 5},
	{std::make_pair(2,0), 6}, {std::make_pair(2,1), 7}, {std::make_pair(2,2), 8}, {std::make_pair(2,3), 9}, {std::make_pair(2,4), 10}, {std::make_pair(2,5), 11}, {std::make_pair(2,6), 12},
	{std::make_pair(3,0), 13}, {std::make_pair(3,1), 14}, {std::make_pair(3,2), 15}, {std::make_pair(3,3), 16}, {std::make_pair(3,4), 17}, {std::make_pair(3,5), 18}, {std::make_pair(3,6), 19},
	{std::make_pair(4,0), 20}, {std::make_pair(4,1), 21}, {std::make_pair(4,2), 22}, {std::make_pair(4,3), 23}, {std::make_pair(4,4), 24}, {std::make_pair(4,5), 25}, {std::make_pair(4,6), 26},
	{std::make_pair(5,2), 27}, {std::make_pair(5,3), 28}, {std::make_pair(5,4), 29},
	{std::make_pair(6,2), 30}, {std::make_pair(6,3), 31}, {std::make_pair(6,4), 32},
};

const std::map<std::pair<int, int>, uint8_t> GameLogic::europeanGridIdxMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,1), 3}, {std::make_pair(1,2), 4}, {std::make_pair(1,3), 5}, {std::make_pair(1,4), 6}, {std::make_pair(1,5), 7},
	{std::make_pair(2,0), 8}, {std::make_pair(2,1), 9}, {std::make_pair(2,2), 10}, {std::make_pair(2,3), 11}, {std::make_pair(2,4), 12}, {std::make_pair(2,5), 13}, {std::make_pair(2,6), 14},
	{std::make_pair(3,0), 15}, {std::make_pair(3,1), 16}, {std::make_pair(3,2), 17}, {std::make_pair(3,3), 18}, {std::make_pair(3,4), 19}, {std::make_pair(3,5), 20}, {std::make_pair(3,6), 21},
	{std::make_pair(4,0), 22}, {std::make_pair(4,1), 23}, {std::make_pair(4,2), 24}, {std::make_pair(4,3), 25}, {std::make_pair(4,4), 26}, {std::make_pair(4,5), 27}, {std::make_pair(4,6), 28},
	{std::make_pair(5,1), 29}, {std::make_pair(5,2), 30}, {std::make_pair(5,3), 31}, {std::make_pair(5,4), 32}, {std::make_pair(5,5), 33},
	{std::make_pair(6,2), 34}, {std::make_pair(6,3), 35}, {std::make_pair(6,4), 36},
};

const std::map<std::pair<int, int>, uint8_t> GameLogic::asymmetricGridIdxMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,2), 3}, {std::make_pair(1,3), 4}, {std::make_pair(1,4), 5},
	{std::make_pair(2,2), 6}, {std::make_pair(2,3), 7}, {std::make_pair(2,4), 8},
	{std::make_pair(3,0), 9}, {std::make_pair(3,1), 10}, {std::make_pair(3,2), 11}, {std::make_pair(3,3), 12}, {std::make_pair(3,4), 13}, {std::make_pair(3,5), 14}, {std::make_pair(3,6), 15}, {std::make_pair(3,7), 16},
	{std::make_pair(4,0), 17}, {std::make_pair(4,1), 18}, {std::make_pair(4,2), 19}, {std::make_pair(4,3), 20}, {std::make_pair(4,4), 21}, {std::make_pair(4,5), 22}, {std::make_pair(4,6), 23}, {std::make_pair(4,7), 24},
	{std::make_pair(5,0), 25}, {std::make_pair(5,1), 26}, {std::make_pair(5,2), 27}, {std::make_pair(5,3), 28}, {std::make_pair(5,4), 29}, {std::make_pair(5,5), 30}, {std::make_pair(5,6), 31}, {std::make_pair(5,7), 32},
	{std::make_pair(6,2), 33}, {std::make_pair(6,3), 34}, {std::make_pair(6,4), 35},
	{std::make_pair(7,2), 36}, {std::make_pair(7,3), 37}, {std::make_pair(7,4), 38},
};

const std::map<std::pair<int, int>, uint8_t> GameLogic::smallDiamondGridIdxMap = {
	{std::make_pair(0,3), 0},
	{std::make_pair(1,2), 1}, {std::make_pair(1,3), 2}, {std::make_pair(1,4), 3},
	{std::make_pair(2,1), 4}, {std::make_pair(2,2), 5}, {std::make_pair(2,3), 6}, {std::make_pair(2,4), 7}, {std::make_pair(2,5), 8},
	{std::make_pair(3,0), 9}, {std::make_pair(3,1), 10}, {std::make_pair(3,2), 11}, {std::make_pair(3,3), 12}, {std::make_pair(3,4), 13}, {std::make_pair(3,5), 14}, {std::make_pair(3,6), 15},
	{std::make_pair(4,0), 16}, {std::make_pair(4,1), 17}, {std::make_pair(4,2), 18}, {std::make_pair(4,3), 19}, {std::make_pair(4,4), 20}, {std::make_pair(4,5), 21}, {std::make_pair(4,6), 22},
	{std::make_pair(5,1), 23}, {std::make_pair(5,2), 24}, {std::make_pair(5,3), 25}, {std::make_pair(5,4), 26}, {std::make_pair(5,5), 27},
	{std::make_pair(6,2), 28}, {std::make_pair(6,3), 29}, {std::make_pair(6,4), 30},
	{std::make_pair(7,3), 31},
};

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
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(6, 4)) {
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

uint64_t GameLogic::convertBoardToSolverBoardFormat() {
	uint64_t solverBoard { 0 };
	int exponent = 0;
	
	for (Field& field : m_board) {
		std::pair<int, int> position = field.getPosition();
		if (field.getState() == FieldState::Occupied || field.getState() == FieldState::Selected) {
			solverBoard += static_cast<uint64_t>(std::pow(2, exponent));
		}
		++exponent;
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
