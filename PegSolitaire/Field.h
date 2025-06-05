#pragma once
#include <utility>

enum class FieldState {
	Empty,
	Occupied,
	Selected,
};

class Field {
private:
	FieldState m_state{};
	std::pair<int, int> m_position{};

public:
	Field() = default;
	Field(FieldState state, std::pair<int, int> position);

	const FieldState& getState();
	const std::pair<int, int> getPosition() const;
	void setState(FieldState state);
	void setPosition(std::pair<int, int> position);
};