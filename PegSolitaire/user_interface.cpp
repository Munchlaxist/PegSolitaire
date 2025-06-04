#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

static const std::array<std::array<int, 7>, 7> defaultBoard = { {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
} };

enum class PegState {
	Empty,
	Occupied,
	Selected,
};

class Peg {
private:
	PegState m_state{};
	std::pair<int, int> m_position;
	sf::CircleShape m_circle;

public:
	Peg() {}

	void setState(PegState state) {
		m_state = state;
		if (state == PegState::Empty) {
			//todo
		} else if (state == PegState::Occupied) {
			//todo
		}
	}

	void setPosition(std::pair<int, int> position) {
		m_position = position;
	}

	PegState getState() const { return m_state; }
	sf::CircleShape& getCircle() { return m_circle; }
	std::pair<int, int> getPosition() const { return m_position; }

};


class UserInterface {
private:
	sf::RenderWindow& m_window;
	std::array<Peg, 33> m_board{};

public:
	UserInterface(sf::RenderWindow& window) : m_window{ window } {
		// Initialize the user interface
		// Set positions for the circles based on a peg solitaire board layout
		std::size_t index = 0;
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {

				if (defaultBoard[row][col] == 1) { // Only place circles where there is a peg
					m_board[index].setPosition(std::pair<int, int>(row, col));
					m_board[index].setState(PegState::Occupied);
					m_board[index].getCircle().setRadius(20.f);
					m_board[index].getCircle().setFillColor(sf::Color::Blue);
					m_board[index].getCircle().setOutlineColor(sf::Color::Black);
					m_board[index].getCircle().setOutlineThickness(1.f);
					m_board[index].getCircle().setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
					++index;
				}
				if (defaultBoard[row][col] == 0) {
					m_board[index].setPosition(std::pair<int, int>(row, col));
					m_board[index].setState(PegState::Empty);
					m_board[index].getCircle().setRadius(20.f);
					m_board[index].getCircle().setFillColor(sf::Color::Transparent);
					m_board[index].getCircle().setOutlineColor(sf::Color::Black);
					m_board[index].getCircle().setOutlineThickness(1.f);
					m_board[index].getCircle().setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
					++index;
				}

			}
		}
	}

	void setBackground(std::filesystem::path filename) {
		m_window.clear();
		sf::Texture backgroundTexture;
		if (!backgroundTexture.loadFromFile(filename)) {
			// TODO exception that the background could not be loaded
			;
		}
		sf::Sprite backgroundSprite(backgroundTexture);
		backgroundSprite.setScale(sf::Vector2f(1, 1));
		m_window.draw(backgroundSprite);
	}

	void drawInitialBoard() {

		for (auto& peg : m_board) {
			m_window.draw(peg.getCircle());
		}
		m_window.display();
	}

	Peg* getClickedPeg(const sf::Vector2i& mousePosition) {
		for (auto& peg : m_board) {
			if (peg.getCircle().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) { // TODO check if the circle is occupied
				return &peg; // Return the clicked peg
			}
		}
		return nullptr;
	}

	void startGame() {
		while (true) {

		}
	}

};

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Peg Solitaire");
	window.clear();
	UserInterface ui(window);
	ui.setBackground("white_oak_bg.png");
	ui.drawInitialBoard();

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (buttonPressed->button == sf::Mouse::Button::Left) {
					// Handle mouse button pressed events here
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (auto* peg = ui.getClickedPeg(mousePosition)) {
						std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl;
						// check if there is a peg already selected -> if so, check if the move is valid, if not mark current peg as selected and change color of circle
						// if the move is valid, do the move and update the board (UPDATE POSITION OF CIRCLE ASWELL!!!), if not, just reset the seleted peg and circle color
						
					}
					
				}
			}
		}
	
	}
}