#include <vector>
#include <array>
#include <map>
#include <chrono>
#include <utility>
#include "Field.h"
#include "Move.h"
#include "Solver.cpp"


class Board {
protected:
	std::vector<Field> m_boardRepresentation;
	std::map<uint8_t, std::pair<int, int>> m_idxToGridMap;
	uint64_t m_defaultBoard{};

public:
	virtual ~Board() = default; // Virtual destructor for proper cleanup of derived classes

	std::vector<Field>& getBoardRepresentation() {
		return m_boardRepresentation;
	}

	std::map<uint8_t, std::pair<int, int>>& getGridToIndexMap() {
		return m_idxToGridMap;
	}

	virtual void initializeBoard() = 0;
	virtual bool solutionFound() = 0;
	virtual std::pair<int, int> getGoalPosition() = 0;
	virtual std::vector<MoveByte> getNextHint(uint64_t boardState) = 0;
};


class EnglishBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 33; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	EnglishBoard() {
		// Initialize the English board representation
		m_defaultBoard = 0x1FFFEFFFF;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,0)}, {7, std::make_pair(2,1)}, {8, std::make_pair(2,2)}, {9, std::make_pair(2,3)}, {10, std::make_pair(2,4)}, {11, std::make_pair(2,5)}, {12, std::make_pair(2,6)},
		{13, std::make_pair(3,0)}, {14, std::make_pair(3,1)}, {15, std::make_pair(3,2)}, {16, std::make_pair(3,3)}, {17, std::make_pair(3,4)}, {18, std::make_pair(3,5)}, {19, std::make_pair(3,6)},
		{20, std::make_pair(4,0)}, {21, std::make_pair(4,1)}, {22, std::make_pair(4,2)}, {23, std::make_pair(4,3)}, {24, std::make_pair(4,4)}, {25, std::make_pair(4,5)}, {26, std::make_pair(4,6)},
		{27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)},
		{30, std::make_pair(6,2)}, {31, std::make_pair(6,3)}, {32, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		EnglishBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class EuropeanBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 37; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	EuropeanBoard() {
		// Initialize the European board representation
		m_defaultBoard = 0x1FFFFFFFFE;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,1)}, {4, std::make_pair(1,2)}, {5, std::make_pair(1,3)}, {6, std::make_pair(1,4)}, {7, std::make_pair(1,5)},
		{8, std::make_pair(2,0)}, {9, std::make_pair(2,1)}, {10, std::make_pair(2,2)}, {11, std::make_pair(2,3)}, {12, std::make_pair(2,4)}, {13, std::make_pair(2,5)}, {14, std::make_pair(2,6)},
		{15, std::make_pair(3,0)}, {16, std::make_pair(3,1)}, {17, std::make_pair(3,2)}, {18, std::make_pair(3,3)}, {19, std::make_pair(3,4)}, {20, std::make_pair(3,5)}, {21, std::make_pair(3,6)},
		{22, std::make_pair(4,0)}, {23, std::make_pair(4,1)}, {24, std::make_pair(4,2)}, {25, std::make_pair(4,3)}, {26, std::make_pair(4,4)}, {27, std::make_pair(4,5)}, {28, std::make_pair(4,6)},
		{29, std::make_pair(5,1)}, {30, std::make_pair(5,2)}, {31, std::make_pair(5,3)}, {32, std::make_pair(5,4)}, {33, std::make_pair(5,5)},
		{34, std::make_pair(6,2)}, {35, std::make_pair(6,3)}, {36, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(6, 4);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		EuropeanBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(50000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class AsymmetricBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 39; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	AsymmetricBoard() {
		// Initialize the Asymmetric board representation
		m_defaultBoard = 0x7FFFEFFFFF;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,2)}, {7, std::make_pair(2,3)}, {8, std::make_pair(2,4)},
		{9, std::make_pair(3,0)}, {10, std::make_pair(3,1)}, {11, std::make_pair(3,2)}, {12, std::make_pair(3,3)}, {13, std::make_pair(3,4)}, {14, std::make_pair(3,5)}, {15, std::make_pair(3,6)}, {16, std::make_pair(3,7)},
		{17, std::make_pair(4,0)}, {18, std::make_pair(4,1)}, {19, std::make_pair(4,2)}, {20, std::make_pair(4,3)}, {21, std::make_pair(4,4)}, {22, std::make_pair(4,5)}, {23, std::make_pair(4,6)}, {24, std::make_pair(4,7)},
		{25, std::make_pair(5,0)}, {26, std::make_pair(5,1)}, {27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)}, {30, std::make_pair(5,5)}, {31, std::make_pair(5,6)}, {32, std::make_pair(5,7)},
		{33, std::make_pair(6,2)}, {34, std::make_pair(6,3)}, {35, std::make_pair(6,4)},
		{36, std::make_pair(7,2)}, {37, std::make_pair(7,3)}, {38, std::make_pair(7,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(4, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		AsymmetricBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class SmallDiamondBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 32; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	SmallDiamondBoard() {
		// Initialize the SmallDiamond board representation
		m_defaultBoard = 0xFFFFEFFF;
		m_idxToGridMap = {
		{0, std::make_pair(0,3)},
		{1, std::make_pair(1,2)}, {2, std::make_pair(1,3)}, {3, std::make_pair(1,4)},
		{4, std::make_pair(2,1)}, {5, std::make_pair(2,2)}, {6, std::make_pair(2,3)}, {7, std::make_pair(2,4)}, {8, std::make_pair(2,5)},
		{9, std::make_pair(3,0)}, {10, std::make_pair(3,1)}, {11, std::make_pair(3,2)}, {12, std::make_pair(3,3)}, {13, std::make_pair(3,4)}, {14, std::make_pair(3,5)}, {15, std::make_pair(3,6)},
		{16, std::make_pair(4,0)}, {17, std::make_pair(4,1)}, {18, std::make_pair(4,2)}, {19, std::make_pair(4,3)}, {20, std::make_pair(4,4)}, {21, std::make_pair(4,5)}, {22, std::make_pair(4,6)},
		{23, std::make_pair(5,1)}, {24, std::make_pair(5,2)}, {25, std::make_pair(5,3)}, {26, std::make_pair(5,4)}, {27, std::make_pair(5,5)},
		{28, std::make_pair(6,2)}, {29, std::make_pair(6,3)}, {30, std::make_pair(6,4)},
		{31, std::make_pair(7,3)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		SmallDiamondBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class ArrowUpBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 33; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	ArrowUpBoard() {
		// Initialize the ArrowUp board representation
		m_defaultBoard = 0x1F8810FBA;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,0)}, {7, std::make_pair(2,1)}, {8, std::make_pair(2,2)}, {9, std::make_pair(2,3)}, {10, std::make_pair(2,4)}, {11, std::make_pair(2,5)}, {12, std::make_pair(2,6)},
		{13, std::make_pair(3,0)}, {14, std::make_pair(3,1)}, {15, std::make_pair(3,2)}, {16, std::make_pair(3,3)}, {17, std::make_pair(3,4)}, {18, std::make_pair(3,5)}, {19, std::make_pair(3,6)},
		{20, std::make_pair(4,0)}, {21, std::make_pair(4,1)}, {22, std::make_pair(4,2)}, {23, std::make_pair(4,3)}, {24, std::make_pair(4,4)}, {25, std::make_pair(4,5)}, {26, std::make_pair(4,6)},
		{27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)},
		{30, std::make_pair(6,2)}, {31, std::make_pair(6,3)}, {32, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		ArrowUpBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class PyramidBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 33; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	PyramidBoard() {
		// Initialize the SmallDiamond board representation
		m_defaultBoard = 0x7F7C710;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,0)}, {7, std::make_pair(2,1)}, {8, std::make_pair(2,2)}, {9, std::make_pair(2,3)}, {10, std::make_pair(2,4)}, {11, std::make_pair(2,5)}, {12, std::make_pair(2,6)},
		{13, std::make_pair(3,0)}, {14, std::make_pair(3,1)}, {15, std::make_pair(3,2)}, {16, std::make_pair(3,3)}, {17, std::make_pair(3,4)}, {18, std::make_pair(3,5)}, {19, std::make_pair(3,6)},
		{20, std::make_pair(4,0)}, {21, std::make_pair(4,1)}, {22, std::make_pair(4,2)}, {23, std::make_pair(4,3)}, {24, std::make_pair(4,4)}, {25, std::make_pair(4,5)}, {26, std::make_pair(4,6)},
		{27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)},
		{30, std::make_pair(6,2)}, {31, std::make_pair(6,3)}, {32, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		EnglishBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class ShurikenBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 33; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	ShurikenBoard() {
		// Initialize the SmallDiamond board representation
		m_defaultBoard = 0x19BDFF7B3;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,0)}, {7, std::make_pair(2,1)}, {8, std::make_pair(2,2)}, {9, std::make_pair(2,3)}, {10, std::make_pair(2,4)}, {11, std::make_pair(2,5)}, {12, std::make_pair(2,6)},
		{13, std::make_pair(3,0)}, {14, std::make_pair(3,1)}, {15, std::make_pair(3,2)}, {16, std::make_pair(3,3)}, {17, std::make_pair(3,4)}, {18, std::make_pair(3,5)}, {19, std::make_pair(3,6)},
		{20, std::make_pair(4,0)}, {21, std::make_pair(4,1)}, {22, std::make_pair(4,2)}, {23, std::make_pair(4,3)}, {24, std::make_pair(4,4)}, {25, std::make_pair(4,5)}, {26, std::make_pair(4,6)},
		{27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)},
		{30, std::make_pair(6,2)}, {31, std::make_pair(6,3)}, {32, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		EnglishBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};

class PlusBoard : public Board {
public:
	void initializeBoard() override {
		m_boardRepresentation.clear();
		bool set;
		for (unsigned char idx{ 0 }; idx < 33; ++idx) {
			set = (m_defaultBoard >> idx) & 1;
			if (set) {
				// If the bit is set, the field is occupied
				m_boardRepresentation.push_back(Field{ FieldState::Occupied, m_idxToGridMap.at(idx) });
			}
			else {
				// If the bit is not set, the field is empty
				m_boardRepresentation.push_back(Field{ FieldState::Empty, m_idxToGridMap.at(idx) });
			}
		}
	}

	PlusBoard() {
		// Initialize the SmallDiamond board representation
		m_defaultBoard = 0x1087C210;
		m_idxToGridMap = {
		{0, std::make_pair(0,2)}, {1, std::make_pair(0,3)}, {2, std::make_pair(0,4)},
		{3, std::make_pair(1,2)}, {4, std::make_pair(1,3)}, {5, std::make_pair(1,4)},
		{6, std::make_pair(2,0)}, {7, std::make_pair(2,1)}, {8, std::make_pair(2,2)}, {9, std::make_pair(2,3)}, {10, std::make_pair(2,4)}, {11, std::make_pair(2,5)}, {12, std::make_pair(2,6)},
		{13, std::make_pair(3,0)}, {14, std::make_pair(3,1)}, {15, std::make_pair(3,2)}, {16, std::make_pair(3,3)}, {17, std::make_pair(3,4)}, {18, std::make_pair(3,5)}, {19, std::make_pair(3,6)},
		{20, std::make_pair(4,0)}, {21, std::make_pair(4,1)}, {22, std::make_pair(4,2)}, {23, std::make_pair(4,3)}, {24, std::make_pair(4,4)}, {25, std::make_pair(4,5)}, {26, std::make_pair(4,6)},
		{27, std::make_pair(5,2)}, {28, std::make_pair(5,3)}, {29, std::make_pair(5,4)},
		{30, std::make_pair(6,2)}, {31, std::make_pair(6,3)}, {32, std::make_pair(6,4)},
		};
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

	std::pair<int, int> getGoalPosition() {
		return std::make_pair(3, 3);
	}

	std::vector<MoveByte> getNextHint(uint64_t boardState) override {
		EnglishBoardSolver solver(boardState);
		std::chrono::milliseconds timeout(25000);
		const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
		solver.solve(startTime, timeout, true);
		return solver.getSolutionPath();
	}
};