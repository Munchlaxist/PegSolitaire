#include <unordered_set>
#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include "Move.h"


class Solver {
protected:
    uint64_t m_board;
private:
    
    std::unordered_set<uint64_t> m_visitedBoardStates{}; // tracks visited board states
    std::vector<Move2> m_solutionPath{}; // stores the moves of the found solution path

public:
    Solver(uint64_t board) : m_board{ board } {};
    ~Solver() = default;

    bool solve(const std::chrono::time_point<std::chrono::system_clock>& startTime, std::chrono::milliseconds& timeout) {
        m_visitedBoardStates.clear();
        m_solutionPath.clear();
        return backtrack(startTime, timeout);
    }

    std::vector<Move2>& getSolutionPath() {
        return m_solutionPath;
    }

protected:
    bool backtrack(const std::chrono::time_point<std::chrono::system_clock>& startTime, std::chrono::milliseconds& timeout) {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime) > timeout) {
            std::cout << "Timeout reached. No solution found." << std::endl;
            return false;
        }
        
        if (foundSolution()) {
            return true;
        }

        if (m_visitedBoardStates.count(m_board)) {
            return false;
        }
        else {
            m_visitedBoardStates.insert(m_board);
        }

        std::vector<Move2> candidateMoves = getNextPossibleMoves();
		//std::cout << "Possible moves count: " << candidateMoves.size() << std::endl;
        for (const Move2& move : candidateMoves) {
			//std::cout << "Trying move: " << move.from << " -> " << move.to << " over " << move.over << std::endl;
            if (isValidMove(move)) {
                //std::cout << "Hello";
                applyMove(move);
                m_solutionPath.push_back(move);

                if (backtrack(startTime, timeout)) {
                    return true;
                }

                undoMove(move);
                m_solutionPath.pop_back();
            }
        }
        return false;
    }


    void applyMove(const Move2& move) {
        m_board &= ~(1ULL << move.from);
        m_board &= ~(1ULL << move.over);
        m_board |= (1ULL << move.to);
    }

    void undoMove(const Move2& move) {
        m_board |= (1ULL << move.from);
        m_board |= (1ULL << move.over);
        m_board &= ~(1ULL << move.to);
    }

    bool isValidMove(const Move2& move) const {
        //std::cout << board << std::endl;
        return ((m_board & (1ULL << move.from)) && (m_board & (1ULL << move.over)) && !(m_board & (1ULL << move.to)));
    }

    virtual bool foundSolution() = 0;

    virtual std::vector<Move2> getNextPossibleMoves() = 0;

    virtual void canonical() = 0;

};


class EnglishBoardSolver : public Solver {
private:
    const std::array<Move2, 76> m_allMovePatterns = {{
        // Horizontale Reihen
        {0,1,2}, {2,1,0},
        {3,4,5}, {5,4,3},
        {6,7,8}, {7,8,9}, {8,9,10}, {8,7,6}, {9,10,11}, {9,8,7}, {10,11,12}, {10,9,8}, {11,10,9}, {12,11,10},
        {13,14,15}, {14,15,16}, {15,16,17}, {16,17,18}, {17,18,19}, {15,14,13}, {16,15,14}, {17,16,15}, {18,17,16}, {19,18,17},
        {20,21,22}, {21,22,23}, {22,23,24}, {23,24,25}, {24,25,26}, {22,21,20}, {23,22,21}, {24,23,22}, {25,24,23}, {26,25,24},
        {27,28,29}, {29,28,27},
        {30,31,32}, {32,31,30},

        // Vertikale Sprünge
        {0,3,8}, {1,4,9}, {2,5,10},
        {3,8,15}, {4,9,16}, {5,10,17},
        {6,13,20}, {7,14,21}, {8,15,22}, {8,3,0}, {9,16,23}, {9,4,1}, {10,17,24}, {10,5,2}, {11,18,25}, {12,19,26},
        {15,8,3}, {15,22,27}, {16,9,4}, {16,23,28}, {17,10,5}, {17,24,29},
        {20,13,6}, {21,14,7}, {22,15,8}, {22,27,30}, {23,16,9}, {23,28,31}, {24,17,10}, {24,29,32}, {25,18,11}, {26,19,12},
        {27,22,15}, {28,23,16}, {29,24,17},
        {30,27,22}, {31,28,23}, {32,29,24}
     }};
    static const uint64_t m_solutionBoard = 0x10000;

public:
    EnglishBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<Move2> getNextPossibleMoves() override {
        std::vector<Move2> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
				//std::cout << "Possible move found: " << static_cast<int>(move.from) << " -> " << move.to << " over " << move.over << std::endl;
            }
        }
		//std::cout << "Total possible moves: " << possibleMoves.size() << std::endl;
        return possibleMoves;
    }

    bool foundSolution() override {
        return m_board == m_solutionBoard;
	}

    void canonical() override {
        // TODO: Implement symmetry reduction logic here later on
	}

};


class SmallDiamondBoardSolver : public Solver {
private:
    const std::array<Move2, 72> m_allMovePatterns = { {
        // Horizontale Reihen
        {1,2,3}, {3,2,1}, 
        {4,5,6}, {5,6,7}, {6,5,4}, {6,7,8}, {7,6,5}, {8,7,6},
        {9,10,11}, {10,11,12}, {11,12,13}, {11,10,9}, {12,13,14}, {12,11,10}, {13,14,15}, {13,12,11}, {14,13,12}, {15,14,13},
		{16,17,18}, {17,18,19}, {18,19,20}, {19,20,21}, {20,21,22}, {18,17,16}, {19,18,17}, {20,19,18}, {21,20,19}, {22,21,20},
        {23,24,25}, {24,25,26}, {25,26,27}, {25,24,23}, {26,25,24}, {27,26,25},
        {28,29,30}, {30,29,28},

        // Vertikale Sprünge
        {0,2,6}, 
        {1,5,11}, {2,6,12}, {3,7,13},
        {4,10,17}, {5,11,18}, {6,12,19}, {6,2,0}, {7,13,20}, {8,14,21},
        {10,17,23}, {11,18,24}, {11,5,1}, {12,19,25}, {12,6,2}, {13,20,26}, {13,7,3}, {14,21,27}, 
        {17,10,4}, {18,11,5}, {18,24,28}, {19,12,6}, {19,25,29}, {20,13,7}, {20,26,30}, {21,14,8},
        {23,17,10}, {24,18,11}, {25,19,12}, {25,29,31}, {26,20,13}, {27,21,14},
        {28,24,18}, {29,25,19}, {30,26,20},
        {31,29,25}
    } };
    static const uint64_t m_solutionBoard = 0x1000;

public:
    SmallDiamondBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<Move2> getNextPossibleMoves() override {
        std::vector<Move2> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
                //std::cout << "Possible move found: " << static_cast<int>(move.from) << " -> " << move.to << " over " << move.over << std::endl;
            }
        }
        //std::cout << "Total possible moves: " << possibleMoves.size() << std::endl;
        return possibleMoves;
    }

    bool foundSolution() override {
        return m_board == m_solutionBoard;
    }

    void canonical() override {
        // TODO: Implement symmetry reduction logic here later on
    }

};


class EuropeanBoardSolver : public Solver {
private:
    const std::array<Move2, 92> m_allMovePatterns = { {
        // Horizontale Reihen
        {0,1,2}, {2,1,0},
        {3,4,5}, {4,5,6}, {5,6,7}, {5,4,3}, {6,5,4}, {7,6,5},
        {8,9,10}, {9,10,11}, {10,11,12}, {11,12,13}, {12,13,14}, {10,9,8}, {11,10,9}, {12,11,10}, {13,12,11}, {14,13,12},
        {15,16,17}, {16,17,18}, {17,18,19}, {18,19,20}, {19,20,21}, {17,16,15}, {18,17,16}, {19,18,17}, {20,19,18}, {21,20,19},
		{22,23,24}, {23,24,25}, {24,25,26}, {25,26,27}, {26,27,28}, {24,23,22}, {25,24,23}, {26,25,24}, {27,26,25}, {28,27,26},
        {29,30,31}, {30,31,32}, {31,32,33}, {31,30,29}, {32,31,30}, {33,32,31},
        {34,35,36}, {36,35,34},
        // Vertikale Sprünge
        {0,4,10}, {1,5,11}, {2,6,12},
        {3,9,16}, {4,10,17}, {5,11,18}, {6,12,19}, {7,13,20},
        {8,15,22}, {9,16,23}, {10,17,24}, {10,4,0}, {11,18,25}, {11,5,1}, {12,19,26}, {12,6,2}, {13,20,27}, {14,21,28},
        {16,9,3}, {16,23,29}, {17,10,4}, {17,24,30}, {18,11,5}, {18,25,31}, {19,12,6}, {19,26,32}, {20,13,7}, {20,27,33},
        {22,15,8}, {23,16,9}, {24,17,10}, {24,30,34}, {25,18,11}, {25,31,35}, {26,19,12}, {26,32,36}, {27,20,13}, {28,21,14},
        {29,23,16}, {30,24,17}, {31,25,18}, {32,26,19}, {33,27,20},
        {34,30,24}, {35,31,25}, {36,32,26}
    } };
    static const uint64_t m_solutionBoard = 0x1000000000;

public:
    EuropeanBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<Move2> getNextPossibleMoves() override {
        std::vector<Move2> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
                //std::cout << "Possible move found: " << static_cast<int>(move.from) << " -> " << move.to << " over " << move.over << std::endl;
            }
        }
        //std::cout << "Total possible moves: " << possibleMoves.size() << std::endl;
        return possibleMoves;
    }

    bool foundSolution() override {
        return m_board == m_solutionBoard;
    }

    void canonical() override {
        // TODO: Implement symmetry reduction logic here later on
    }

};