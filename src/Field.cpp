#include "Field.h"
#include <utility>


Field::Field(FieldState state, std::pair<int, int> position) : m_state{ state }, m_position{ position } {}

const FieldState& Field::getState() const {
	return m_state;
}

const std::pair<int, int> Field::getPosition() const {
	return m_position;
}

void Field::setState(const FieldState& state) {
	m_state = state;
}

void Field::setPosition(const std::pair<int, int>& position) {
	m_position = position;
}