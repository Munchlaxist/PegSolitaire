#include <utility>
#include <array>
#include <cmath>
#include "Field.h"
#include <map>
#include <stdexcept>
#include "GameLogic.h"
#include "Move.h"


GameLogic::GameLogic() : m_board{ std::make_unique<EnglishBoard>()} {}

std::vector<Field>& GameLogic::getBoard() {
	return m_board->getBoardRepresentation();
}

void GameLogic::setBoard(std::unique_ptr<Board> board) {
	m_board = std::move(board);
}

const std::map<std::pair<int, int>, uint8_t>& GameLogic::getGridToIndexMap() {
	return m_board->getGridToIndexMap();
}

std::stack<MovePair>& GameLogic::getMoveHistory() {
	return m_moveHistory;
}

GameState& GameLogic::getGameState() {
	return m_gameState;
}

void GameLogic::setGameState(GameState state) {
	m_gameState = state;
}

BoardType& GameLogic::getBoardType() {
	return m_board->getBoardType();
}

Field& GameLogic::getField(std::pair<int, int> position) {
	for (Field& field : getBoard()) {
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
		m_moveHistory.push(MovePair(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) + 2 == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) == std::get<1>(nextFieldPosition)) {
		// Then we jump down - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition) + 1, std::get<1>(selectedFieldPosition)));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(MovePair(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) - 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the left - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) - 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(MovePair(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
	else if (std::get<0>(selectedFieldPosition) == std::get<0>(nextFieldPosition) && std::get<1>(selectedFieldPosition) + 2 == std::get<1>(nextFieldPosition)) {
		// Then we jump to the right - set the new states of the fields accordingly
		Field& jumpedOverField = getField(std::make_pair(std::get<0>(selectedFieldPosition), std::get<1>(selectedFieldPosition) + 1));
		selectedField.setState(FieldState::Empty);
		field.setState(FieldState::Occupied);
		jumpedOverField.setState(FieldState::Empty);
		m_moveHistory.push(MovePair(selectedFieldPosition, jumpedOverField.getPosition(), nextFieldPosition));
	}
}

void GameLogic::undoMove() {
	if (!m_moveHistory.empty()) {
		MovePair lastMove = m_moveHistory.top();
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
	for (Field& selectedField : getBoard()) {
		if (selectedField.getState() == FieldState::Occupied) {
			for (Field& field : getBoard()) {
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
	return m_board->solutionFound();
}

uint64_t GameLogic::convertBoardFormat() {
	uint64_t solverBoard { 0 };
	int exponent = 0;
	
	for (Field& field : getBoard()) {
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
	m_moveHistory = std::stack<MovePair>(); // Reset the move history for the new game
	//m_board->initializeBoard();
}
