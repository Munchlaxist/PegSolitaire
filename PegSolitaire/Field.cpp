#include "Field.h";
#include <utility>


Field::Field(FieldState state = FieldState::Empty, std::pair<int, int> position = { 0,0 }) : m_state{ state }, m_position{ position } {}

const FieldState& Field::getState() {
	return m_state;
}
const std::pair<int, int> Field::getPosition() const {
	return m_position;
}
void Field::setState(FieldState state) {
	m_state = state;
}

void Field::setPosition(std::pair<int, int> position) {
	m_position = position;
}