#pragma once
#include <SFML/System/Vector2.hpp>

/**
	Represents a move in the peg solitaire game.
*/
template <typename CoordType>
struct Move {
	CoordType from;
	CoordType over;
	CoordType to;

	Move(CoordType from, CoordType over, CoordType to) {
		this->from = from;
		this->over = over;
		this->to = to;
	}
};

using MovePair = Move<std::pair<int, int>>; // For 2D coordinates (improved readability)
using MoveByte = Move<uint8_t>; // For 1D coordinates (improved performance within solver)
