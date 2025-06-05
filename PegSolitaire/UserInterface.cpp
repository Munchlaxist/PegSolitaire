#include <SFML/Graphics.hpp>
#include <iostream>
#include "UserInterface.h";


UserInterface::UserInterface(GameLogic gameLogic) : m_gameLogic{ gameLogic } {
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


void UserInterface::setBackground(std::filesystem::path filename) {
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

void UserInterface::drawBoard() {
	m_window.clear();
	setBackground("white_oak_bg.png");
	for (auto& peg : m_gameLogic.getBoard()) {
		m_window.draw(fieldToShape[&peg]);
	}
	m_window.display();
}

void UserInterface::updateBoard() {
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

Field* UserInterface::getClickedPeg(const sf::Vector2i& mousePosition) {
	for (auto& peg : m_gameLogic.getBoard()) {
		if (fieldToShape[&peg].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) { // TODO check if the circle is occupied
			return &peg; // Return the clicked peg
		}
	}
	return nullptr;
}

Field* UserInterface::getSelectedPeg() {
	for (auto& peg : m_gameLogic.getBoard()) {
		if (peg.getState() == FieldState::Selected) {
			return &peg; // Return the selected peg
		}
	}
	return nullptr;
}


void UserInterface::gameLoop() {
	drawBoard(); // Draw the initial board
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
									}
									else {
										selectedPeg->setState(FieldState::Occupied);
										updateBoard();
									}

								}
								else {
									selectedPeg->setState(FieldState::Occupied);
									updateBoard();
								}
							}
							else {
								if (peg->getState() == FieldState::Occupied) {
									peg->setState(FieldState::Selected);
									updateBoard();
								}
							}

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
