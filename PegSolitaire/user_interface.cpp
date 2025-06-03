#include <SFML/Graphics.hpp>
#include <iostream>

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
	Invalid,
	Empty,
	Occupied,
};

class Peg {
private:
	PegState m_state;
	std::pair<int, int> m_position;
	sf::CircleShape m_circle;

public:
	Peg() {}
	Peg(int row, int col) : m_position{ row, col } {
		if (row < 2 && col < 2 || row > 4 && col < 2 || row < 2 && col > 4 || row > 4 && col > 4) {
			m_state = PegState::Invalid; // Invalid position for a peg
		}
	}

	void setState(PegState state) {
		m_state = state;
		if (state == PegState::Empty) {
			//todo
		} else if (state == PegState::Occupied) {
			//todo
		}
	}

	PegState getState() const { return m_state; }
	const sf::CircleShape& getCircle() const { return m_circle; }
	std::pair<int, int> getPosition() const { return m_position; }

};

class PegBoard {
private:
	std::array<std::array<Peg, 7>, 7> m_board;

public:
	PegBoard() {
		// todo: needs rework
		for (int row = 0; row < 7; ++row) {
			for (int col = 0; col < 7; ++col) {
				m_board[row][col] = Peg(row, col);
				if (defaultBoard[row][col] == 1) {
					m_board[row][col].setState(PegState::Occupied);
				} else if (defaultBoard[row][col] == 0) {
					m_board[row][col].setState(PegState::Empty);
				}
			}
		}
	}
};

class UserInterface {
private:
	sf::RenderWindow& m_window;
	std::array<sf::CircleShape, 33> m_board;

public:
	UserInterface(sf::RenderWindow& window) : m_window{ window } {
		// Initialize the user interface
		// Set positions for the circles based on a peg solitaire board layout
		std::size_t index = 0;
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {

				if (defaultBoard[row][col] == 1) { // Only place circles where there is a peg
					m_board[index].setRadius(20.f);
					m_board[index].setFillColor(sf::Color::Blue);
					m_board[index].setOutlineColor(sf::Color::Black);
					m_board[index].setOutlineThickness(1.f);
					m_board[index].setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
					++index;
				}
				if (defaultBoard[row][col] == 0) {
					m_board[index].setRadius(20.f);
					m_board[index].setFillColor(sf::Color::Transparent);
					m_board[index].setOutlineColor(sf::Color::Black);
					m_board[index].setOutlineThickness(1.f);
					m_board[index].setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
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

		for (const auto& circle : m_board) {
			m_window.draw(circle);
		}
		m_window.display();
	}

	sf::CircleShape* getClickedCircle(const sf::Vector2i& mousePosition) {
		for (auto& circle : m_board) {
			if (circle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) { // TODO check if the circle is occupied
				return &circle; // Return the clicked circle
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
					if (auto* circle = ui.getClickedCircle(mousePosition)) {
						std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl;
						// change color of clicked circle
						while (true) {
							// expect another mouse button pressed event and then check if the move is valid or not
						}
					}
					
				}
			}
		}
	
	}
}