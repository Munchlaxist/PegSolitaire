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
	Field m_to{};

public:
	Move(Field from, Field to) : m_from{ from }, m_to{ to } {}

	Field& getFrom()  {
		return m_from;
	}

	Field& getTo() {
		return m_to;
	}
};

class GameLogic {
private:
	std::array<Field, 33> m_board{}; // only 33 fields are used in the game, the rest is unused and can be ignored

public:

	enum class Direction {
		Up,
		Down,
		Left,
		Right
	};

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
					m_board[index].setPosition(std::make_pair(row, col));
				} else if (defaultBoard[row][col] == 0) {
					m_board[index].setState(FieldState::Empty);
					m_board[index].setPosition(std::make_pair(row, col));
				}
				else {
					continue; // skip unused fields
				}
				m_board[index].setPosition(std::make_pair(row, col));
				++index;
			}
		}
	}

	std::array<Field, 33>& getBoard() {
		return m_board;
	}

	Field& getField(std::pair<int, int> position) {
		/**
		* get the field at the given position, i.e. row and column
		*/
		for (Field& field : m_board) {
			if (field.getPosition() == position) {
				return field;
			}
		}
		//throw an exception if there is no field with the given position
		//throw std::runtime_error("Field not found at the given position");
	}

	bool isValidMove(Field& selectedPeg, Field& peg) {
		/**
		* check if the next move is valid, i.e. the selected peg can jump over another peg and land in an empty space given by peg.
		*/
		std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
		std::pair<int, int> nextPegPosition = peg.getPosition();

		if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// then we jump up, check if there is an occupied peg in between. If so the move is valid
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
			if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
				return true; // Valid move
				
			}
		}
		else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
				if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
					return true; // Valid move

				
				}
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
			if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
				return true; // Valid move

			}
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) +1));
			if (jumpedOverPeg.getState() == FieldState::Occupied && peg.getState() == FieldState::Empty) {
				return true; // Valid move

			}
			
		}
		return false;
	}

	void makeMove(Field& selectedPeg, Field& peg) {
		std::pair<int, int> selectedPegPosition = selectedPeg.getPosition();
		std::pair<int, int> nextPegPosition = peg.getPosition();

		if (std::get<0>(selectedPegPosition) - 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// then we jump up, check if there is an occupied peg in between. If so the move is valid
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) - 1, std::get<1>(selectedPegPosition)));
			selectedPeg.setState(FieldState::Empty);
			//selectedPeg.getCircle().setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
			//peg.getCircle().setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
			peg.setState(FieldState::Occupied);
			jumpedOverPeg.setState(FieldState::Empty);
		}
		else if (std::get<0>(selectedPegPosition) + 2 == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition) + 1, std::get<1>(selectedPegPosition)));
			selectedPeg.setState(FieldState::Empty);
			//selectedPeg.getCircle().setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
			//peg.getCircle().setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
			peg.setState(FieldState::Occupied);
			jumpedOverPeg.setState(FieldState::Empty);
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) - 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) - 1));
			selectedPeg.setState(FieldState::Empty);
			//selectedPeg.getCircle().setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
			//peg.getCircle().setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
			peg.setState(FieldState::Occupied);
			jumpedOverPeg.setState(FieldState::Empty);
		}
		else if (std::get<0>(selectedPegPosition) == std::get<0>(nextPegPosition) && std::get<1>(selectedPegPosition) + 2 == std::get<1>(nextPegPosition)) {
			// Check if there is a peg in between
			Field& jumpedOverPeg = getField(std::make_pair(std::get<0>(selectedPegPosition), std::get<1>(selectedPegPosition) + 1));
			selectedPeg.setState(FieldState::Empty);
			//selectedPeg.getCircle().setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
			//peg.getCircle().setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
			peg.setState(FieldState::Occupied);
			jumpedOverPeg.setState(FieldState::Empty);

		}
		/**
		* update and adapt the selected Peg, the peg that is jumped over and the peg that is landed on (given by peg).
		*/
		
	}


	bool movesAvailable() {
		// todo check if there are any moves available
		for (Field& selectedPeg : m_board) {
			if (selectedPeg.getState() == FieldState::Occupied) {
				for (Field& peg : m_board) {
					if (peg.getState() == FieldState::Empty) {
						if (isValidMove(selectedPeg, peg)) {
							return true; // If any valid move is found, return true
						}
					}
				}
			}
		}
		return false;
	}

	bool solutionFound() {
		for (Field& peg : m_board) {
			if (peg.getState() == FieldState::Occupied && peg.getPosition() != std::pair<int, int>(3,3)) {
				return false; // If any peg is still occupied, the solution is not found
			}
		}
		return true;
	}
};

class UserInterface {
private:
	sf::RenderWindow& m_window;
	GameLogic m_gameLogic{};
	std::map<Field*, sf::CircleShape> fieldToShape{};

public:
	UserInterface(sf::RenderWindow& window) : m_window{ window } {
		// Initialize the user interface
		// Set positions for the circles based on a peg solitaire board layout
		for (Field& peg : m_gameLogic.getBoard()) {
			sf::CircleShape circle(20.f); // Radius of 20 pixels
			if (peg.getState() == FieldState::Occupied) {
				circle.setFillColor(sf::Color::Blue);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(225 + peg.getPosition().second * 50, 225 + peg.getPosition().first * 50)); // Adjust position based on row and column
				fieldToShape[&peg] = circle; // Map the peg to its circle shape
			}
			else if (peg.getState() == FieldState::Empty) {
				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(225 + peg.getPosition().second * 50, 225 + peg.getPosition().first * 50)); // Adjust position based on row and column
				fieldToShape[&peg] = circle; // Map the peg to its circle shape
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

	void drawBoard() {
		m_window.clear();
		setBackground("white_oak_bg.png");
		for (auto& peg : m_gameLogic.getBoard()) {
			m_window.draw(fieldToShape[&peg]);
		}
		m_window.display();
	}

	void updateBoard() {
		for (auto& peg : m_gameLogic.getBoard()) {
			if (peg.getState() == FieldState::Occupied) {
				fieldToShape[&peg].setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
			}
			else if (peg.getState() == FieldState::Empty) {
				fieldToShape[&peg].setFillColor(sf::Color::Transparent); // Change color to indicate empty peg
			}
			// selected should not be the case here
			else if (peg.getState() == FieldState::Selected) {
				fieldToShape[&peg].setFillColor(sf::Color::Red); // Change color to indicate selection
			}
		}
		drawBoard(); // Redraw the board with updated peg states
	}

	Field* getClickedPeg(const sf::Vector2i& mousePosition) {
		for (auto& peg : m_gameLogic.getBoard()) {
			if (fieldToShape[&peg].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) { // TODO check if the circle is occupied
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
										if (m_gameLogic.isValidMove(*selectedPeg, *peg)) {
											m_gameLogic.makeMove(*selectedPeg, *peg);
											updateBoard(); // update the board based on the move, i.e. the map of pegs to circles
											if (m_gameLogic.solutionFound()) {
												std::cout << "Solution found! Congratulations!" << std::endl;
												// todo show solution found message on screen
												gameState = GameState::GameOver;
												break;
											}
											if (!m_gameLogic.movesAvailable()) {
												std::cout << "No moves available! Game over!" << std::endl;
												// todo show no moves available message on screen
												gameState = GameState::GameOver;
												break;
											}

											// check if there are still valid moves left (if not, end the game)
											//if (noValidMovesLeft()) {
												// todo check if you won or lost
											//	gameState = GameState::GameOver;
											//}
										}
										else {
											selectedPeg->setState(FieldState::Occupied);
											updateBoard();
											//fieldToShape[selectedPeg].setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
										}

									}
									else {
										selectedPeg->setState(FieldState::Occupied);
										updateBoard();
										//fieldToShape[selectedPeg].setFillColor(sf::Color::Blue); // Change color to indicate occupied peg
									}
								}
								else {
									if (peg->getState() == FieldState::Occupied) {
										peg->setState(FieldState::Selected);
										updateBoard();
										//fieldToShape[peg].setFillColor(sf::Color::Red); // Change color to indicate selection
										//drawBoard();
									}
								}
								//drawBoard();

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