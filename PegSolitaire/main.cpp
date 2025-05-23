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


class SolvePegSolitaire {
private:
	std::array<std::array<int, 7>, 7>& m_board_state;
	int m_moves{ 0 };
	

public:
	enum class Direction {
		Up,
		Down,
		Left,
		Right
	};

	SolvePegSolitaire(std::array<std::array<int, 7>, 7> board_state = defaultBoard) : m_board_state{ board_state } {};

	const std::string_view getDirectionString(const Direction& direction) const {
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
		int peg_count = 0;
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (m_board_state[row][col] == 1) {
					++peg_count;
				}
			}
		}
		return (peg_count == 1 && m_board_state[3][3] == 1);
	}

	bool isValidMove(std::pair<std::size_t, std::size_t>& position, const Direction& direction) {
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
		if (foundSolution()) {
			//TODO: print board state
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
		std::cout << "Solving Peg Solitaire..." << std::endl;
		if (findSolution()) {
			std::cout << "Solution found!" << std::endl;
		}
		else {
			std::cout << "No solution found." << std::endl;
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
	pegSolitaire.solvePegSolitaire();
    return 0;
}