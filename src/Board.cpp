#include <vector>
#include <array>
#include <map>
#include <utility>
#include "Field.h"


enum class BoardType {
	English,		// Standard English Peg Solitaire board
	European,		// European variant of the Peg Solitaire board
	Asymmetric,		// Diamond-shaped Peg Solitaire board
	SmallDiamond,	// Small Diamond-shaped Peg Solitaire board
	ArrowUp,		// Arrow Up-shaped Peg Solitaire board
};


class Board {
protected:
	std::vector<Field> m_boardRepresentation{}; // 64-bit integer representing the board state
	BoardType m_boardType{}; // Type of the board
	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap;

public:
	Board() = default;

	std::vector<Field>& getBoardRepresentation() {
		return m_boardRepresentation;
	}
	BoardType& getBoardType() {
		return m_boardType;
	}
	virtual void initializeBoard() = 0; // Pure virtual function to initialize the board
	virtual bool solutionFound() = 0;
	const std::map<std::pair<int, int>, uint8_t>& getGridToIndexMap() {
		return gridToIndexMap;
	}
};


class EnglishBoard : public Board {
private:
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard{ {
	{-1, -1,  1,  1,  1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{ 1,  1,  1,  0,  1,  1,  1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{-1, -1,  1,  1,  1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	} }; // Describes the initial state of the board when represented as a 7x7 grid

	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap{ {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,2), 3}, {std::make_pair(1,3), 4}, {std::make_pair(1,4), 5},
	{std::make_pair(2,0), 6}, {std::make_pair(2,1), 7}, {std::make_pair(2,2), 8}, {std::make_pair(2,3), 9}, {std::make_pair(2,4), 10}, {std::make_pair(2,5), 11}, {std::make_pair(2,6), 12},
	{std::make_pair(3,0), 13}, {std::make_pair(3,1), 14}, {std::make_pair(3,2), 15}, {std::make_pair(3,3), 16}, {std::make_pair(3,4), 17}, {std::make_pair(3,5), 18}, {std::make_pair(3,6), 19},
	{std::make_pair(4,0), 20}, {std::make_pair(4,1), 21}, {std::make_pair(4,2), 22}, {std::make_pair(4,3), 23}, {std::make_pair(4,4), 24}, {std::make_pair(4,5), 25}, {std::make_pair(4,6), 26},
	{std::make_pair(5,2), 27}, {std::make_pair(5,3), 28}, {std::make_pair(5,4), 29},
	{std::make_pair(6,2), 30}, {std::make_pair(6,3), 31}, {std::make_pair(6,4), 32},
	} };

public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (defaultBoard[row][col] == 1) {
					Field field{ FieldState::Occupied, std::make_pair(row, col) };
					m_boardRepresentation.push_back(field);
				}
				else if (defaultBoard[row][col] == 0) {
					Field field{ FieldState::Empty, std::make_pair(row, col) };
					m_boardRepresentation.push_back(field);
				}
				else {
					continue; // Skip invalid fields
				}
			}
		}
	}

	EnglishBoard() : Board() {
		// Initialize the English board representation
		m_boardType = BoardType::English;
		m_boardRepresentation.resize(33);
		initializeBoard();
	}

	bool solutionFound() override {
		for (Field& field : m_boardRepresentation) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(3, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		return true;
	}

};

class EuropeanBoard : public Board {
private:
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard{ {
	{-1, -1,  0,  1,  1, -1, -1},
	{-1,  1,  1,  1,  1,  1, -1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{-1,  1,  1,  1,  1,  1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	} };

	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,1), 3}, {std::make_pair(1,2), 4}, {std::make_pair(1,3), 5}, {std::make_pair(1,4), 6}, {std::make_pair(1,5), 7},
	{std::make_pair(2,0), 8}, {std::make_pair(2,1), 9}, {std::make_pair(2,2), 10}, {std::make_pair(2,3), 11}, {std::make_pair(2,4), 12}, {std::make_pair(2,5), 13}, {std::make_pair(2,6), 14},
	{std::make_pair(3,0), 15}, {std::make_pair(3,1), 16}, {std::make_pair(3,2), 17}, {std::make_pair(3,3), 18}, {std::make_pair(3,4), 19}, {std::make_pair(3,5), 20}, {std::make_pair(3,6), 21},
	{std::make_pair(4,0), 22}, {std::make_pair(4,1), 23}, {std::make_pair(4,2), 24}, {std::make_pair(4,3), 25}, {std::make_pair(4,4), 26}, {std::make_pair(4,5), 27}, {std::make_pair(4,6), 28},
	{std::make_pair(5,1), 29}, {std::make_pair(5,2), 30}, {std::make_pair(5,3), 31}, {std::make_pair(5,4), 32}, {std::make_pair(5,5), 33},
	{std::make_pair(6,2), 34}, {std::make_pair(6,3), 35}, {std::make_pair(6,4), 36},
	};

public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (defaultBoard[row][col] == 1) {
					m_boardRepresentation.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
				}
				else if (defaultBoard[row][col] == 0) {
					m_boardRepresentation.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
				}
				else {
					continue; // Skip invalid fields
				}
			}
		}
	}

	EuropeanBoard() {
		// Initialize the English board representation
		m_boardType = BoardType::European;
		m_boardRepresentation.resize(37);
		initializeBoard();
	}

	bool solutionFound() override {
		for (Field& field : m_boardRepresentation) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(6, 4)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		return true;
	}
};

class AsymmetricBoard : public Board {
private:
	static constexpr std::array<std::array<int, 8>, 8> defaultBoard{ {
	{-1, -1,  1,  1,  1, -1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1, -1},
	{ 1,  1,  1,  1,  1,  1,  1,  1},
	{ 1,  1,  1,  0,  1,  1,  1,  1},
	{ 1,  1,  1,  1,  1,  1,  1,  1},
	{-1, -1,  1,  1,  1, -1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1, -1},
	} };

	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,2), 3}, {std::make_pair(1,3), 4}, {std::make_pair(1,4), 5},
	{std::make_pair(2,2), 6}, {std::make_pair(2,3), 7}, {std::make_pair(2,4), 8},
	{std::make_pair(3,0), 9}, {std::make_pair(3,1), 10}, {std::make_pair(3,2), 11}, {std::make_pair(3,3), 12}, {std::make_pair(3,4), 13}, {std::make_pair(3,5), 14}, {std::make_pair(3,6), 15}, {std::make_pair(3,7), 16},
	{std::make_pair(4,0), 17}, {std::make_pair(4,1), 18}, {std::make_pair(4,2), 19}, {std::make_pair(4,3), 20}, {std::make_pair(4,4), 21}, {std::make_pair(4,5), 22}, {std::make_pair(4,6), 23}, {std::make_pair(4,7), 24},
	{std::make_pair(5,0), 25}, {std::make_pair(5,1), 26}, {std::make_pair(5,2), 27}, {std::make_pair(5,3), 28}, {std::make_pair(5,4), 29}, {std::make_pair(5,5), 30}, {std::make_pair(5,6), 31}, {std::make_pair(5,7), 32},
	{std::make_pair(6,2), 33}, {std::make_pair(6,3), 34}, {std::make_pair(6,4), 35},
	{std::make_pair(7,2), 36}, {std::make_pair(7,3), 37}, {std::make_pair(7,4), 38},
	};

public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		for (std::size_t row = 0; row < 8; ++row) {
			for (std::size_t col = 0; col < 8; ++col) {
				if (defaultBoard[row][col] == 1) {
					m_boardRepresentation.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
				}
				else if (defaultBoard[row][col] == 0) {
					m_boardRepresentation.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
				}
				else {
					continue; // Skip invalid fields
				}
			}
		}
	}

	AsymmetricBoard() {
		// Initialize the English board representation
		m_boardType = BoardType::Asymmetric;
		m_boardRepresentation.resize(39);
		initializeBoard();
	}

	bool solutionFound() override {
		for (Field& field : m_boardRepresentation) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(4, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		return true;
	}
};

class SmallDiamondBoard : public Board {
private:
	static constexpr std::array<std::array<int, 7>, 8> defaultBoard{ {
	{-1, -1, -1,  1, -1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	{-1,  1,  1,  1,  1,  1, -1},
	{ 1,  1,  1,  0,  1,  1,  1},
	{ 1,  1,  1,  1,  1,  1,  1},
	{-1,  1,  1,  1,  1,  1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	{-1, -1, -1,  1, -1, -1, -1},
	} };

	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap = {
	{std::make_pair(0,3), 0},
	{std::make_pair(1,2), 1}, {std::make_pair(1,3), 2}, {std::make_pair(1,4), 3},
	{std::make_pair(2,1), 4}, {std::make_pair(2,2), 5}, {std::make_pair(2,3), 6}, {std::make_pair(2,4), 7}, {std::make_pair(2,5), 8},
	{std::make_pair(3,0), 9}, {std::make_pair(3,1), 10}, {std::make_pair(3,2), 11}, {std::make_pair(3,3), 12}, {std::make_pair(3,4), 13}, {std::make_pair(3,5), 14}, {std::make_pair(3,6), 15},
	{std::make_pair(4,0), 16}, {std::make_pair(4,1), 17}, {std::make_pair(4,2), 18}, {std::make_pair(4,3), 19}, {std::make_pair(4,4), 20}, {std::make_pair(4,5), 21}, {std::make_pair(4,6), 22},
	{std::make_pair(5,1), 23}, {std::make_pair(5,2), 24}, {std::make_pair(5,3), 25}, {std::make_pair(5,4), 26}, {std::make_pair(5,5), 27},
	{std::make_pair(6,2), 28}, {std::make_pair(6,3), 29}, {std::make_pair(6,4), 30},
	{std::make_pair(7,3), 31},
	};

public:
	void initializeBoard() {
		m_boardRepresentation.clear();
		for (std::size_t row = 0; row < 8; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (defaultBoard[row][col] == 1) {
					m_boardRepresentation.push_back(Field{ FieldState::Occupied, std::make_pair(row, col) });
				}
				else if (defaultBoard[row][col] == 0) {
					m_boardRepresentation.push_back(Field{ FieldState::Empty, std::make_pair(row, col) });
				}
				else {
					continue; // Skip invalid fields
				}
			}
		}
	}

	SmallDiamondBoard() {
		// Initialize the English board representation
		m_boardType = BoardType::SmallDiamond;
		m_boardRepresentation.resize(32);
		initializeBoard();
	}

	bool solutionFound() override {
		for (Field& field : m_boardRepresentation) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(3, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		return true;
	}
};

class ArrowUpBoard : public Board {
private:
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard{ {
	{-1, -1,  0,  1,  0, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	{ 0,  1,  1,  1,  1,  1,  0},
	{ 0,  0,  0,  1,  0,  0,  0},
	{ 0,  0,  0,  1,  0,  0,  0},
	{-1, -1,  1,  1,  1, -1, -1},
	{-1, -1,  1,  1,  1, -1, -1},
	} }; // Describes the initial state of the board when represented as a 7x7 grid

	const std::map<std::pair<int, int>, uint8_t> gridToIndexMap = {
	{std::make_pair(0,2), 0}, {std::make_pair(0,3), 1}, {std::make_pair(0,4), 2},
	{std::make_pair(1,2), 3}, {std::make_pair(1,3), 4}, {std::make_pair(1,4), 5},
	{std::make_pair(2,0), 6}, {std::make_pair(2,1), 7}, {std::make_pair(2,2), 8}, {std::make_pair(2,3), 9}, {std::make_pair(2,4), 10}, {std::make_pair(2,5), 11}, {std::make_pair(2,6), 12},
	{std::make_pair(3,0), 13}, {std::make_pair(3,1), 14}, {std::make_pair(3,2), 15}, {std::make_pair(3,3), 16}, {std::make_pair(3,4), 17}, {std::make_pair(3,5), 18}, {std::make_pair(3,6), 19},
	{std::make_pair(4,0), 20}, {std::make_pair(4,1), 21}, {std::make_pair(4,2), 22}, {std::make_pair(4,3), 23}, {std::make_pair(4,4), 24}, {std::make_pair(4,5), 25}, {std::make_pair(4,6), 26},
	{std::make_pair(5,2), 27}, {std::make_pair(5,3), 28}, {std::make_pair(5,4), 29},
	{std::make_pair(6,2), 30}, {std::make_pair(6,3), 31}, {std::make_pair(6,4), 32},
	};

public:
	void initializeBoard() {
		m_boardRepresentation.clear();
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (defaultBoard[row][col] == 1) {
					Field field{ FieldState::Occupied, std::make_pair(row, col) };
					m_boardRepresentation.push_back(field);
				}
				else if (defaultBoard[row][col] == 0) {
					Field field{ FieldState::Empty, std::make_pair(row, col) };
					m_boardRepresentation.push_back(field);
				}
				else {
					continue; // Skip invalid fields
				}
			}
		}
	}

	ArrowUpBoard() {
		// Initialize the English board representation
		m_boardType = BoardType::ArrowUp;
		m_boardRepresentation.resize(17);
		initializeBoard();
	}

	bool solutionFound() override {
		for (Field& field : m_boardRepresentation) {
			if (field.getState() == FieldState::Occupied && field.getPosition() != std::pair<int, int>(3, 3)) {
				return false; // If any field is still occupied besides the one in the center, the solution is not found
			}
		}
		return true;
	}
};