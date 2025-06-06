#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "UserInterface.h"


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

void UserInterface::createTryAgainButton() {
	sf::RectangleShape tryAgainButton(sf::Vector2f(150.f, 50.f)); // Create a button with size 200x50 pixels
	tryAgainButton.setFillColor(sf::Color::Color(47, 126, 244)); // Set button color
	tryAgainButton.setOutlineColor(sf::Color::Black); // Set button outline color
	tryAgainButton.setOutlineThickness(2.f); // Set button outline thickness
	tryAgainButton.setPosition(sf::Vector2f(10.f, 10.f)); // Set button position
	sf::Font font{};
	if (font.openFromFile("arial.ttf")) {
		sf::Text buttonText(font, "Try Again!", 30);
		buttonText.setFillColor(sf::Color::White); // Set text color
		buttonText.setPosition(sf::Vector2f(tryAgainButton.getPosition().x + 10.f, tryAgainButton.getPosition().y + 8.f)); // Set text position
		m_window.draw(tryAgainButton);
		m_window.draw(buttonText);
		m_window.display();
	}
}

void UserInterface::gameLoop() {
	sf::Music gameWon("game_won.mp3");
	sf::Music gameLost("game_lost.mp3");
	sf::Music correctMove("correct_move.mp3");

	sf::Font font;
	font.openFromFile("arial.ttf");
	sf::Text gameWonText(font, "Congratulations!", 50);
	gameWonText.setFillColor(sf::Color::Blue); // Set text color
	gameWonText.setPosition(sf::Vector2f(200.f, 20.f)); // Set text position
	sf::Text gameLostText(font, "You lost, try again!", 50);
	gameLostText.setFillColor(sf::Color::Red); // Set text color
	gameLostText.setPosition(sf::Vector2f(200.f, 20.f)); // Set text position
	

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
											gameWon.play(); // Play game won sound
											std::cout << "Solution found! Congratulations!" << std::endl;
											
											// todo show solution found message on screen
											gameState = GameState::GameOver;
											updateBoard(); // update the board based on the move, i.e. the map of pegs to circles
											m_window.draw(gameWonText); // Draw the game won text
											createTryAgainButton(); // Draw the try again button
											break;
										}
										if (!m_gameLogic.movesAvailable()) {
											gameLost.play(); // Play game lost sound
											std::cout << "No moves available! Game over!" << std::endl;
											
											// todo show no moves available message on screen
											gameState = GameState::GameOver;
											updateBoard(); // update the board based on the move, i.e. the map of pegs to circles
											m_window.draw(gameLostText); // Draw the game lost text
											createTryAgainButton(); // Draw the try again button
											break;
										}
										correctMove.play(); // Play correct move sound
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
			else {
				if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (buttonPressed->button == sf::Mouse::Button::Left) {
						// Handle mouse button pressed events here
						sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
						if (mousePosition.x >= 10 && mousePosition.x <= 160 && mousePosition.y >= 10 && mousePosition.y <= 60) {
							// Try again button clicked
							m_gameLogic.resetGame(); // Reset the game logic
							updateBoard(); // Update the board to reflect the reset state
							gameState = GameState::Playing; // Change game state back to playing
						}
					}
				}

			}


		}
	}
}
