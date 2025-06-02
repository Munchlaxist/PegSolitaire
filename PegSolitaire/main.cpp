#include <iostream>
#include <array>
#include <string_view>

static const std::array<std::array<int, 7>, 7> defaultBoard = { {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
} };


class PegSolitaireSolver {
private:
	std::array<std::array<int, 7>, 7>& m_board_state; // representation of the 7x7 board
	int m_moves{ 0 };
	

public:
	enum class Direction {
		Up,
		Down,
		Left,
		Right
	};

	PegSolitaireSolver(std::array<std::array<int, 7>, 7> board_state = defaultBoard) : m_board_state{ board_state } {};

	const std::string_view getDirectionString(const Direction& direction) const {
		/**
		* get the string representation of the direction.
		*/
		switch (direction) {
		case Direction::Up:
			return "Up";
		case Direction::Down:
			return "Down";
		case Direction::Left:
			return "Left";
		case Direction::Right:
			return "Right";
		default:
			return "Unknown direction";
		}
	}

	bool foundSolution() {
		/**
		* check if the current board state is the solution board state.
		*/
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (m_board_state[row][col] == 1) {
					if (row == 3 && col == 3) {
						continue;
					}
					else {
						return false;
					}
				}
			}
		}
		return true;
		
	}

	bool isValidMove(std::pair<std::size_t, std::size_t>& position, const Direction& direction) {
		/**
		* check if the next move is valid, i.e. the peg can jump over another peg and land in an empty space.
		*/
		switch (direction) {
		case Direction::Up:
			if (position.first < 2 || m_board_state[position.first - 1][position.second] != 1 || m_board_state[position.first - 2][position.second] != 0) {
				return false;
			}
			break;
		case Direction::Down:
			if (position.first > 4 || m_board_state[position.first + 1][position.second] != 1 || m_board_state[position.first + 2][position.second] != 0) {
				return false;
			}
			break;
		case Direction::Left:
			if (position.second < 2 || m_board_state[position.first][position.second - 1] != 1 || m_board_state[position.first][position.second - 2] != 0) {
				return false;
			}
			break;
		case Direction::Right:
			if (position.second > 4 || m_board_state[position.first][position.second + 1] != 1 || m_board_state[position.first][position.second + 2] != 0) {
				return false;
			}
			break;
		default:
			std::cout << "This should not have happened" << std::endl;
			return false;
		}
		return true;
	}

	void makeMove(std::pair<std::size_t, std::size_t>& position, const Direction& direction) {
		/**
		* make the given move by updating the board state.
		*/
		switch (direction) {
		case Direction::Up:
			m_board_state[position.first][position.second] = 0;
			m_board_state[position.first - 1][position.second] = 0;
			m_board_state[position.first - 2][position.second] = 1;
			break;
		case Direction::Down:
			m_board_state[position.first][position.second] = 0;
			m_board_state[position.first + 1][position.second] = 0;
			m_board_state[position.first + 2][position.second] = 1;
			break;
		case Direction::Left:
			m_board_state[position.first][position.second] = 0;
			m_board_state[position.first][position.second - 1] = 0;
			m_board_state[position.first][position.second - 2] = 1;
			break;
		case Direction::Right:
			m_board_state[position.first][position.second] = 0;
			m_board_state[position.first][position.second + 1] = 0;
			m_board_state[position.first][position.second + 2] = 1;
			break;
		default:
			break;
		}
		++m_moves;
	}

	void undoMove(std::pair<std::size_t, std::size_t>& position, const Direction& direction) {
		/**
		* undo the given move by reverting the board state to its previous state.
		*/
		switch (direction) {
		case Direction::Up:
			m_board_state[position.first][position.second] = 1;
			m_board_state[position.first - 1][position.second] = 1;
			m_board_state[position.first - 2][position.second] = 0;
			break;
		case Direction::Down:
			m_board_state[position.first][position.second] = 1;
			m_board_state[position.first + 1][position.second] = 1;
			m_board_state[position.first + 2][position.second] = 0;
			break;
		case Direction::Left:
			m_board_state[position.first][position.second] = 1;
			m_board_state[position.first][position.second - 1] = 1;
			m_board_state[position.first][position.second - 2] = 0;
			break;
		case Direction::Right:
			m_board_state[position.first][position.second] = 1;
			m_board_state[position.first][position.second + 1] = 1;
			m_board_state[position.first][position.second + 2] = 0;
			break;
		default:
			break;
		}
		--m_moves;
	}

	void printMove(std::pair<std::size_t, std::size_t>& position, const Direction& direction) {
		/**
		* print the move made in a human-readable format.
		*/
		switch (direction) {
		case Direction::Up:
			std::cout << "Jump " << getDirectionString(direction) << " from (" << position.first << ", " << position.second << ") to (" << position.first-2 << ", " << position.second << ")." << std::endl;
			break;
		case Direction::Down:
			std::cout << "Jump " << getDirectionString(direction) << " from (" << position.first << ", " << position.second << ") to (" << position.first+2 << ", " << position.second << ")." << std::endl;
			break;
		case Direction::Left:
			std::cout << "Jump " << getDirectionString(direction) << " from (" << position.first << ", " << position.second << ") to (" << position.first << ", " << position.second-2 << ")." << std::endl;
			break;
		case Direction::Right:
			std::cout << "Jump " << getDirectionString(direction) << " from (" << position.first << ", " << position.second << ") to (" << position.first << ", " << position.second+2 << ")." << std::endl;
			break;
		default:
			std::cout << "Invalid move." << std::endl;
			break;
		}
	}

	bool findSolution() {
		/**
		* find a solution to the peg solitaire puzzle using backtracking.
		*/
		if (foundSolution()) {
			return true;
		}
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (m_board_state[row][col] == 1) {
					std::pair<std::size_t, std::size_t> position = {row, col};
					for (const auto& direction : { Direction::Up, Direction::Down, Direction::Left, Direction::Right }) {
						if (isValidMove(position, direction)) {
							makeMove(position, direction);
							if (findSolution()) {
								printMove(position, direction);
								return true;
							}
							undoMove(position, direction);
						}
					}
				}
			}
		}
		return false;
	}

	void solvePegSolitaire() {
		/**
		* solve the peg solitaire puzzle and print the moves being made (if found).
		*/
		std::cout << "Solving Peg Solitaire..." << std::endl;
		if (findSolution()) {
			std::cout << "Solution found!" << std::endl;
		}
		else {
			std::cout << "No solution found." << std::endl;
		}
	}
	

};


/*
int main() {
	// Initialize board state - could be any valid state of the game
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
	PegSolitaireSolver pegSolitaire(board_state);
	pegSolitaire.solvePegSolitaire();
    return 0;
}
*/
