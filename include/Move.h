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

struct Move2 {
	uint8_t from;
	uint8_t over;
	uint8_t to;

	Move2(uint8_t from, uint8_t over, uint8_t to) {
		this->from = from;
		this->over = over;
		this->to = to;
	}
};