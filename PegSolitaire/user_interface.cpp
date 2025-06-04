#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <map>


enum class GameState {
	Playing,
	GameOver
};


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
	Field(FieldState state = FieldState::Empty, std::pair<int, int> position = { 0,0 }) : m_state{ state }, m_position{ position } {}

	const FieldState& getState() {
		return m_state;
	}
	const std::pair<int, int> getPosition() const {
		return m_position;
	}
	void setState(FieldState state) {
		m_state = state;
	}

	void setPosition(std::pair<int, int> position) {
		m_position = position;
	}
};

/*
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
*/

class Move {
private:
	Field m_from{};
	Field m_over{};
	Field m_to{};

public:
	Move(Field from, Field over, Field to) : m_from{ from }, m_over{ over }, m_to{ to } {}

	Field& getFrom()  {
		return m_from;
	}
	Field& getOver() {
		return m_over;
	}
	Field& getTo()  {
		return m_to;
	}
};

class GameLogic {
private:
	std::array<Field, 33> m_board{};

public:
	static constexpr std::array<std::array<int, 7>, 7> defaultBoard { {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	} };
	GameLogic() {
		// Initialize the game logic with the default board state
		std::size_t index = 0;
		for (std::size_t row = 0; row < 7; ++row) {
			for (std::size_t col = 0; col < 7; ++col) {
				if (defaultBoard[row][col] == 1) {
					m_board[index].setState(FieldState::Occupied);
				} else if (defaultBoard[row][col] == 0) {
					m_board[index].setState(FieldState::Empty);
				}
				m_board[index].setPosition(std::make_pair(row, col));
				++index;
			}
		}
	}

	std::array<Field, 33>& getBoard() {
		return m_board;
	}

	bool isValidMove(Move& move) {
		/**
		* check if the next move is valid, i.e. the selected peg can jump over another peg and land in an empty space given by peg.
		*/
		std::pair<int, int> selectedPegPosition = move.getFrom().getPosition();
		std::pair<int, int> jumpedOverPegPosition = move.getOver().getPosition();
		std::pair<int, int> nextPegPosition = move.getTo().getPosition();

		if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// then we jump up, check if there is an occupied peg in between. If so the move is valid
			if (jumpedOverPegPosition == std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition))) {
				if (move.getOver().getState() == FieldState::Occupied) {
					return true; // Valid move
				}
			}
		}
		else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			if (jumpedOverPegPosition == std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition))) {
				if (move.getOver().getState() == FieldState::Occupied) {
					return true; // Valid move
				}
			}
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			if (jumpedOverPegPosition == std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1)) {
				if (move.getOver().getState() == FieldState::Occupied) {
					return true; // Valid move
				}
			}
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			if (jumpedOverPegPosition == std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) + 1)) {
				if (move.getOver().getState() == FieldState::Occupied) {
					return true; // Valid move
				}
			}
			
		}
		return false;
	}

	void makeMove(Move& move) {
		/**
		* update and adapt the selected Peg, the peg that is jumped over and the peg that is landed on (given by peg).
		*/
		move.getFrom().setState(FieldState::Empty);
		//selectedPeg.getCircle().setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
		//peg.getCircle().setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
		move.getTo().setState(FieldState::Occupied);
		move.getOver().setState(FieldState::Empty);
		
	}

	bool movesAvailable() {

	}

	bool solutionFound() {

	}
};

class UserInterface {
private:
	sf::RenderWindow& m_window;
	GameLogic m_gameLogic{};
	std::map<Field, sf::CircleShape> fieldToShape{};

public:
	UserInterface(sf::RenderWindow& window) : m_window{ window } {
		// Initialize the user interface
		// Set positions for the circles based on a peg solitaire board layout
		for (auto& peg : m_gameLogic.getBoard()) {
			sf::CircleShape circle(20.f); // Radius of 20 pixels
			if (peg.getState() == FieldState::Occupied) {
				circle.setFillColor(sf::Color::Blue);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(225 + peg.getPosition().second * 50, 225 + peg.getPosition().first * 50)); // Adjust position based on row and column
				fieldToShape[peg] = circle; // Map the peg to its circle shape
			}
			else if (peg.getState() == FieldState::Empty) {
				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(225 + peg.getPosition().second * 50, 225 + peg.getPosition().first * 50)); // Adjust position based on row and column
				fieldToShape[peg] = circle; // Map the peg to its circle shape
			}
		}
	}

	std::map<Field, sf::CircleShape>& getFieldToShape() {
		return fieldToShape;
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

	void drawBoard() {
		m_window.clear();
		setBackground("white_oak_bg.png");
		for (auto& peg : m_gameLogic.getBoard()) {
			m_window.draw(fieldToShape[peg]);
		}
		m_window.display();
	}

	void updateBoard() {

	}

	Field* getClickedPeg(const sf::Vector2i& mousePosition) {
		for (auto& peg : m_gameLogic.getBoard()) {
			if (fieldToShape[peg].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) { // TODO check if the circle is occupied
				return &peg; // Return the clicked peg
			}
		}
		return nullptr;
	}

	Field* getSelectedPeg() {
		for (auto& peg : m_gameLogic.getBoard()) {
			if (peg.getState() == FieldState::Selected) {
				return &peg; // Return the selected peg
			}
		}
		return nullptr;
	}


	void gameLoop() {
		GameState gameState = GameState::Playing;

		while (m_window.isOpen())
		{
			while (const std::optional event = m_window.pollEvent())
			{

				if (event->is<sf::Event::Closed>()) {
					m_window.close();
				}

				if (gameState == GameState::Playing) {

					if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
						if (buttonPressed->button == sf::Mouse::Button::Left) {
							// Handle mouse button pressed events here
							sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
							if (auto* peg = getClickedPeg(mousePosition)) {
								std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl;
								// check if there is a peg already selected -> if so, check if the move is valid, if not mark current peg as selected and change color of circle
								// if the move is valid, do the move and update the board (UPDATE POSITION OF CIRCLE ASWELL!!!), if not, just reset the seleted peg and circle color
								if (auto* selectedPeg = getSelectedPeg()) {
									// TODO check if the move is valid
									if (peg->getState() == FieldState::Empty) {
										// check if the move is valid
										if (m_gameLogic.isValidMove(...)) {
											m_gameLogic.makeMove(...);
											updateBoard(...); // update the board based on the move, i.e. the map of pegs to circles

											// check if there are still valid moves left (if not, end the game)
											//if (noValidMovesLeft()) {
												// todo check if you won or lost
											//	gameState = GameState::GameOver;
											//}
										}
										else {
											selectedPeg->setState(FieldState::Occupied);
											getFieldToShape()[*selectedPeg].setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
										}

									}
									else {
										selectedPeg->setState(FieldState::Occupied);
										getFieldToShape()[*selectedPeg].setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
									}
								}
								else {
									if (peg->getState() == FieldState::Occupied) {
										peg->setState(FieldState::Selected);
										getFieldToShape()[*peg].setFillColor(sf::Color::Red); // Change color to indicate selection
									}
								}
								drawBoard();

							}
						}
					}
					else {
						// todo try again button with mouse click event
					}
				}


			}
		}
	}

};


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Peg Solitaire");
	window.clear();
	UserInterface ui(window);
	//ui.setBackground("white_oak_bg.png");
	ui.drawBoard();
	ui.gameLoop();
	
}