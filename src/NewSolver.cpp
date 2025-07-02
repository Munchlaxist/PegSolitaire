#include <unordered_set>
#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include "Move.h"


class Solver {
private:
    std::unordered_set<uint64_t> m_visitedBoardStates{}; // tracks visited board states
    std::vector<Move2> m_solutionPath{}; // stores the moves of the found solution path

public:
    ~Solver() = default;

    bool solve(uint64_t& board, const std::chrono::time_point<std::chrono::system_clock>& startTime, std::chrono::milliseconds& timeout) {
        m_visitedBoardStates.clear();
        m_solutionPath.clear();
        return backtrack(board, startTime, timeout);
    }

    std::vector<Move2>& getSolutionPath() {
        return m_solutionPath;
    }

protected:
    bool backtrack(uint64_t& board, const std::chrono::time_point<std::chrono::system_clock>& startTime, std::chrono::milliseconds& timeout) {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime) > timeout) {
            std::cout << "Timeout reached. No solution found." << std::endl;
            return false;
        }
        
        if (foundSolution(board)) {
            return true;
        }

        if (m_visitedBoardStates.count(board)) {
            return false;
        }
        else {
            m_visitedBoardStates.insert(board);
        }

        std::vector<Move2> candidateMoves = getNextPossibleMoves(board);
		//std::cout << "Possible moves count: " << candidateMoves.size() << std::endl;
        for (const Move2& move : candidateMoves) {
			//std::cout << "Trying move: " << move.from << " -> " << move.to << " over " << move.over << std::endl;
            if (isValidMove(board, move)) {
                //std::cout << "Hello";
                board = applyMove(board, move);
                m_solutionPath.push_back(move);

                if (backtrack(board, startTime, timeout)) {
                    return true;
                }

                board = undoMove(board, move);
                m_solutionPath.pop_back();
            }
        }
        return false;
    }


    uint64_t& applyMove(uint64_t& board, const Move2& move) {
        board &= ~(1ULL << move.from);
        board &= ~(1ULL << move.over);
        board |= (1ULL << move.to);
        return board;
    }

    uint64_t& undoMove(uint64_t& board, const Move2& move) {
        board |= (1ULL << move.from);
        board |= (1ULL << move.over);
        board &= ~(1ULL << move.to);
        return board;
    }

    bool isValidMove(uint64_t& board, const Move2& move) {
        //std::cout << board << std::endl;
        return ((board & (1ULL << move.from)) && (board & (1ULL << move.over)) && !(board & (1ULL << move.to)));
    }

    virtual bool foundSolution(uint64_t& board) = 0;

    virtual std::vector<Move2> getNextPossibleMoves(uint64_t& board) = 0;

    virtual void canonical(uint64_t& board) = 0;

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
    EnglishBoardSolver() {
        
    };

protected:
    std::vector<Move2> getNextPossibleMoves(uint64_t& board) override {
        std::vector<Move2> possibleMoves;
        for (const auto& move : m_allMovePatterns) {
            if (isValidMove(board, move)) {
                possibleMoves.push_back(move);
				//std::cout << "Possible move found: " << static_cast<int>(move.from) << " -> " << move.to << " over " << move.over << std::endl;
            }
        }
		//std::cout << "Total possible moves: " << possibleMoves.size() << std::endl;
        return possibleMoves;
    }

    bool foundSolution(uint64_t& board) override {
        return board == m_solutionBoard;
	}

    void canonical(uint64_t& board) override {
        // TODO: Implement symmetry reduction logic here later on
	}

};