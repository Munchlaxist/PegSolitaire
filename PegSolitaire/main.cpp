#include <iostream>
#include <array>

std::array<std::array<int, 7>, 7> defaultBoard = { {
		{-1, -1, 1, 1, 1, -1, -1},
		{-1, -1, 1, 1, 1, -1, -1},
		{ 1,  1, 1, 1, 1,  1,  1},
		{ 1,  1, 1, 0, 1,  1,  1},
		{ 1,  1, 1, 1, 1,  1,  1},
		{-1, -1, 1, 1, 1, -1, -1},
		{-1, -1, 1, 1, 1, -1, -1},
} };

enum class Direction {
	Up,
	Down,
	Left,
	Right
};

class SolvePegSolitaire {
	std::array<std::array<int, 7>, 7>& m_board_state;
	

public:

	SolvePegSolitaire(std::array<std::array<int, 7>, 7> board_state = defaultBoard) : m_board_state{ board_state } {};

	bool foundSolution() {
	}

	bool isValidMove(std::pair<int, int>& position, const Direction& direction) {

	}

	void makeMove(std::pair<int, int>& position, const Direction& direction) {
	}

	void undoMove(std::pair<int, int>& position, const Direction& direction) {
	}

	bool findSolution() {
		if (foundSolution()) {
			//TODO: print board state
			return true;
		}
		for (int row = 0; row < 7; ++row) {
			for (int col = 0; col < 7; ++col) {
				if (m_board_state[row][col] == 1) {
					std::pair<int, int> position = {row, col};
					for (const auto& direction : { Direction::Up, Direction::Down, Direction::Left, Direction::Right }) {
						if (isValidMove(position, direction)) {
							makeMove(position, direction);
							if (findSolution()) {
								// TODO: print move we just have done based on position, direction and board state
								return true;
							}
							undoMove(position, direction);
						}
					}
				}
			}
		}
	}
	

};

int main() {
	// Initialize default board state
	std::array<std::array<int, 7>, 7> board_state = { {
		{-1, -1, 1, 1, 1, -1, -1},
		{-1, -1, 1, 1, 1, -1, -1},
		{ 1,  1, 1, 1, 1,  1,  1},
		{ 1,  1, 1, 0, 1,  1,  1},
		{ 1,  1, 1, 1, 1,  1,  1},
		{-1, -1, 1, 1, 1, -1, -1},
		{-1, -1, 1, 1, 1, -1, -1},
	} };

	// Solve the peg solitaire puzzle
	SolvePegSolitaire pegSolitaire(board_state);
	pegSolitaire.findSolution();
    return 0;
}