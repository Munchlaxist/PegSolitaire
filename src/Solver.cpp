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
    std::vector<MoveByte> m_solutionPath{}; // stores the moves of the found solution path

public:
    Solver(uint64_t board) : m_board{ board } {};
    ~Solver() = default;

    bool solve(const std::chrono::time_point<std::chrono::system_clock>& startTime, std::chrono::milliseconds& timeout) {
        m_visitedBoardStates.clear();
        m_solutionPath.clear();
        return backtrack(startTime, timeout);
    }

    std::vector<MoveByte>& getSolutionPath() {
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
        
        std::vector<MoveByte> candidateMoves = getNextPossibleMoves();
        for (const MoveByte& move : candidateMoves) {
            if (isValidMove(move)) {
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


    void applyMove(const MoveByte& move) {
        m_board &= ~(1ULL << move.from);
        m_board &= ~(1ULL << move.over);
        m_board |= (1ULL << move.to);
    }

    void undoMove(const MoveByte& move) {
        m_board |= (1ULL << move.from);
        m_board |= (1ULL << move.over);
        m_board &= ~(1ULL << move.to);
    }

    bool isValidMove(const MoveByte& move) const {
        return ((m_board & (1ULL << move.from)) && (m_board & (1ULL << move.over)) && !(m_board & (1ULL << move.to)));
    }

    virtual bool foundSolution() = 0;

    virtual std::vector<MoveByte> getNextPossibleMoves() = 0;

    virtual void canonical() = 0;

};


class EnglishBoardSolver : public Solver {
private:
    static const uint64_t m_solutionBoard = 0x10000;
    const std::array<MoveByte, 76> m_allMovePatterns = {{
        // Horizontal moves
        {0,1,2}, {2,1,0},
        {3,4,5}, {5,4,3},
        {6,7,8}, {7,8,9}, {8,9,10}, {8,7,6}, {9,10,11}, {9,8,7}, {10,11,12}, {10,9,8}, {11,10,9}, {12,11,10},
        {13,14,15}, {14,15,16}, {15,16,17}, {16,17,18}, {17,18,19}, {15,14,13}, {16,15,14}, {17,16,15}, {18,17,16}, {19,18,17},
        {20,21,22}, {21,22,23}, {22,23,24}, {23,24,25}, {24,25,26}, {22,21,20}, {23,22,21}, {24,23,22}, {25,24,23}, {26,25,24},
        {27,28,29}, {29,28,27},
        {30,31,32}, {32,31,30},
        // Vertical moves
        {0,3,8}, {1,4,9}, {2,5,10},
        {3,8,15}, {4,9,16}, {5,10,17},
        {6,13,20}, {7,14,21}, {8,15,22}, {8,3,0}, {9,16,23}, {9,4,1}, {10,17,24}, {10,5,2}, {11,18,25}, {12,19,26},
        {15,8,3}, {15,22,27}, {16,9,4}, {16,23,28}, {17,10,5}, {17,24,29},
        {20,13,6}, {21,14,7}, {22,15,8}, {22,27,30}, {23,16,9}, {23,28,31}, {24,17,10}, {24,29,32}, {25,18,11}, {26,19,12},
        {27,22,15}, {28,23,16}, {29,24,17},
        {30,27,22}, {31,28,23}, {32,29,24}
    }};

public:
    EnglishBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<MoveByte> getNextPossibleMoves() override {
        std::vector<MoveByte> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
            }
        }
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
    static const uint64_t m_solutionBoard = 0x1000000000;
    const std::array<MoveByte, 92> m_allMovePatterns = { {
            // Horizontal moves
            {0,1,2}, {2,1,0},
            {3,4,5}, {4,5,6}, {5,6,7}, {5,4,3}, {6,5,4}, {7,6,5},
            {8,9,10}, {9,10,11}, {10,11,12}, {11,12,13}, {12,13,14}, {10,9,8}, {11,10,9}, {12,11,10}, {13,12,11}, {14,13,12},
            {15,16,17}, {16,17,18}, {17,18,19}, {18,19,20}, {19,20,21}, {17,16,15}, {18,17,16}, {19,18,17}, {20,19,18}, {21,20,19},
            {22,23,24}, {23,24,25}, {24,25,26}, {25,26,27}, {26,27,28}, {24,23,22}, {25,24,23}, {26,25,24}, {27,26,25}, {28,27,26},
            {29,30,31}, {30,31,32}, {31,32,33}, {31,30,29}, {32,31,30}, {33,32,31},
            {34,35,36}, {36,35,34},
            // Vertical moves
            {0,4,10}, {1,5,11}, {2,6,12},
            {3,9,16}, {4,10,17}, {5,11,18}, {6,12,19}, {7,13,20},
            {8,15,22}, {9,16,23}, {10,17,24}, {10,4,0}, {11,18,25}, {11,5,1}, {12,19,26}, {12,6,2}, {13,20,27}, {14,21,28},
            {16,9,3}, {16,23,29}, {17,10,4}, {17,24,30}, {18,11,5}, {18,25,31}, {19,12,6}, {19,26,32}, {20,13,7}, {20,27,33},
            {22,15,8}, {23,16,9}, {24,17,10}, {24,30,34}, {25,18,11}, {25,31,35}, {26,19,12}, {26,32,36}, {27,20,13}, {28,21,14},
            {29,23,16}, {30,24,17}, {31,25,18}, {32,26,19}, {33,27,20},
            {34,30,24}, {35,31,25}, {36,32,26}
        } };

public:
    EuropeanBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<MoveByte> getNextPossibleMoves() override {
        std::vector<MoveByte> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
            }
        }
        return possibleMoves;
    }

    bool foundSolution() override {
        return m_board == m_solutionBoard;
    }

    void canonical() override {
        // TODO: Implement symmetry reduction logic here later on
    }
};


class AsymmetricBoardSolver : public Solver {
private:
    static const uint64_t m_solutionBoard = 0x100000;
    const std::array<MoveByte, 96> m_allMovePatterns = { {
            // Horizontal moves
            {0,1,2}, {2,1,0},
            {3,4,5}, {5,4,3},
            {6,7,8}, {8,7,6},
            {9,10,11}, {10,11,12}, {11,12,13}, {12,13,14}, {13,14,15}, {14,15,16}, {11,10,9}, {12,11,10}, {13,12,11}, {14,13,12}, {15,14,13}, {16,15,14},
            {17,18,19}, {18,19,20}, {19,20,21}, {20,21,22}, {21,22,23}, {22,23,24}, {19,18,17}, {20,19,18}, {21,20,19}, {22,21,20}, {23,22,21}, {24,23,22},
            {25,26,27}, {26,27,28}, {27,28,29}, {28,29,30}, {29,30,31}, {30,31,32}, {27,26,25}, {28,27,26}, {29,28,27}, {30,29,28}, {31,30,29}, {32,31,30},
            {33,34,25}, {35,34,33},
            {36,37,38}, {38,37,36},
            // Vertical moves
            {0,3,6}, {1,4,7}, {2,5,8},
            {3,6,11}, {4,7,12}, {5,8,13},
            {6,3,0}, {6,11,19}, {7,4,1}, {7,12,20}, {8,5,2}, {8,13,21},
            {9,17,25}, {10,18,26}, {11,6,3}, {11,19,27}, {12,7,4}, {12,20,28}, {13,8,5}, {13,21,29}, {14,22,30}, {14,22,30}, {15,23,31}, {16,24,32},
            {19,11,6}, {19,27,33}, {20,12,7}, {20,28,34}, {21,13,8}, {21,29,35}, {30,22,14}, {31,23,15}, {32,24,16},
            {25,17,9}, {26,18,10}, {27,19,11}, {27,33,36}, {28,20,12}, {28,34,37}, {29,21,13}, {29,35,38}, {30,22,14}, {31,23,15}, {32,24,16},
            {33,27,19}, {34,28,20}, {35,29,21},
            {36,33,27}, {37,34,28}, {38,35,29}
        } };

public:
    AsymmetricBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<MoveByte> getNextPossibleMoves() override {
        std::vector<MoveByte> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
            }
        }
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
    static const uint64_t m_solutionBoard = 0x1000;
    const std::array<MoveByte, 72> m_allMovePatterns = { {
        // Horizontal moves
        {1,2,3}, {3,2,1}, 
        {4,5,6}, {5,6,7}, {6,5,4}, {6,7,8}, {7,6,5}, {8,7,6},
        {9,10,11}, {10,11,12}, {11,12,13}, {11,10,9}, {12,13,14}, {12,11,10}, {13,14,15}, {13,12,11}, {14,13,12}, {15,14,13},
		{16,17,18}, {17,18,19}, {18,19,20}, {19,20,21}, {20,21,22}, {18,17,16}, {19,18,17}, {20,19,18}, {21,20,19}, {22,21,20},
        {23,24,25}, {24,25,26}, {25,26,27}, {25,24,23}, {26,25,24}, {27,26,25},
        {28,29,30}, {30,29,28},

        // Vertical moves
        {0,2,6}, 
        {1,5,11}, {2,6,12}, {3,7,13},
        {4,10,17}, {5,11,18}, {6,12,19}, {6,2,0}, {7,13,20}, {8,14,21},
        {10,17,23}, {11,18,24}, {11,5,1}, {12,19,25}, {12,6,2}, {13,20,26}, {13,7,3}, {14,21,27}, 
        {17,10,4}, {18,11,5}, {18,24,28}, {19,12,6}, {19,25,29}, {20,13,7}, {20,26,30}, {21,14,8},
        {23,17,10}, {24,18,11}, {25,19,12}, {25,29,31}, {26,20,13}, {27,21,14},
        {28,24,18}, {29,25,19}, {30,26,20},
        {31,29,25}
    }};

public:
    SmallDiamondBoardSolver(uint64_t board) : Solver(board) {};

protected:
    std::vector<MoveByte> getNextPossibleMoves() override {
        std::vector<MoveByte> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(move)) {
                possibleMoves.push_back(move);
            }
        }
        return possibleMoves;
    }

    bool foundSolution() override {
        return m_board == m_solutionBoard;
    }

    void canonical() override {
        // TODO: Implement symmetry reduction logic here later on
    }
};
