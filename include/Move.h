#pragma once
#include <SFML/System/Vector2.hpp>

/**
	Represents a move in the peg solitaire game.
*/
struct Move {
	std::pair<int, int> from;
	std::pair<int, int> over;
	std::pair<int, int> to;

	Move(std::pair<int, int> from, std::pair<int, int> over, std::pair<int, int> to) {
		this->from = from;
		this->over = over;
		this->to = to;
	}
};