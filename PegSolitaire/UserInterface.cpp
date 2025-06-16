#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdexcept>
#include "UserInterface.h"


UserInterface::UserInterface(GameLogic& gameLogic) : m_gameLogic{ gameLogic } {
	// Initialize the user interface and the board
	for (Field& peg : m_gameLogic.getBoard()) {
		sf::CircleShape circle(20.f); // Every game field is represented by a circle with radius 20 pixels
		if (peg.getState() == FieldState::Occupied) {
			circle.setFillColor(sf::Color::Blue);
			circle.setOutlineColor(sf::Color::Black);
			circle.setOutlineThickness(1.f);
			circle.setPosition(sf::Vector2f(static_cast<float>(225 + peg.getPosition().second * 50), static_cast<float>(225 + peg.getPosition().first * 50))); // Adjust position based on the specific field
			fieldToShape[&peg] = circle; // Map the field to its circle representation
		}
		else if (peg.getState() == FieldState::Empty) {
			circle.setFillColor(sf::Color::Transparent);
			circle.setOutlineColor(sf::Color::Black);
			circle.setOutlineThickness(1.f);
			circle.setPosition(sf::Vector2f(static_cast<float>(225 + peg.getPosition().second * 50), static_cast<float>(225 + peg.getPosition().first * 50)));
			fieldToShape[&peg] = circle;
		}
	}
}


void UserInterface::setBackground(std::filesystem::path filename) {
	m_window.clear();
	sf::Texture backgroundTexture; // Create texture for the background
	if (!backgroundTexture.loadFromFile(filename)) {
		throw std::runtime_error("Could not load background image: " + filename.string());
	}
	sf::Sprite backgroundSprite(backgroundTexture); // Create sprite for the background
	backgroundSprite.setScale(sf::Vector2f(1, 1));
	m_window.draw(backgroundSprite);
}


void UserInterface::drawBoard() {
	m_window.clear(); // Clear the window before redrawing the new state of the board
	setBackground("white_oak_bg.png");
	for (auto& peg : m_gameLogic.getBoard()) {
		m_window.draw(fieldToShape[&peg]);
	}
	m_window.display();
}


void UserInterface::syncBoard() {
	for (auto& peg : m_gameLogic.getBoard()) {
		if (peg.getState() == FieldState::Occupied) {
			fieldToShape[&peg].setFillColor(sf::Color::Blue); // Change color to indicate occupied field
		}
		else if (peg.getState() == FieldState::Empty) {
			fieldToShape[&peg].setFillColor(sf::Color::Transparent); // Change color to indicate empty field
		}
		else if (peg.getState() == FieldState::Selected) {
			fieldToShape[&peg].setFillColor(sf::Color::Red); // Change color to indicate selected field
		}
	}
	drawBoard(); // Redraw the board with updated peg states
}


Field* UserInterface::getClickedField(const sf::Vector2i& mousePosition) {
	for (auto& peg : m_gameLogic.getBoard()) {
		if (fieldToShape[&peg].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			return &peg; // Return reference to the clicked field
		}
	}
	return nullptr;
}


Field* UserInterface::getCurrentSelectedField() {
	for (auto& peg : m_gameLogic.getBoard()) {
		if (peg.getState() == FieldState::Selected) {
			return &peg; // Return reference to the selected field
		}
	}
	return nullptr;
}


void UserInterface::createTryAgainButton() {
	sf::RectangleShape tryAgainButton(sf::Vector2f(150.f, 50.f)); // Create a button with size 150x50 pixels
	tryAgainButton.setFillColor(sf::Color::Color(47, 126, 244)); // RBG color
	tryAgainButton.setOutlineColor(sf::Color::Black);
	tryAgainButton.setOutlineThickness(2.f);
	tryAgainButton.setPosition(sf::Vector2f(10.f, 10.f));
	sf::Font font{}; // load a font for the button text
	if (font.openFromFile("arial.ttf")) {
		sf::Text buttonText(font, "Try Again!", 30);
		buttonText.setFillColor(sf::Color::White);
		buttonText.setPosition(sf::Vector2f(tryAgainButton.getPosition().x + 10.f, tryAgainButton.getPosition().y + 8.f));
		m_window.draw(tryAgainButton);
		m_window.draw(buttonText);
		m_window.display();
	}
}


void UserInterface::gameLoop() {
	// TODO: Implement music input as seperate function
	sf::Music gameWon("game_won.mp3");
	sf::Music gameLost("game_lost.mp3");
	sf::Music correctMove("correct_move.mp3");

	// TODO: Implement winning/losing message as seperate function
	sf::Font font;
	if (!font.openFromFile("arial.ttf")) {
		throw std::runtime_error("Could not load font: arial.ttf");
	}
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
						// Handle left mouse button pressed events here
						sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
						if (auto* peg = getClickedField(mousePosition)) {
							std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl; // Debugging output

							// Check if there already is a field selected -> if so, check if the move is valid, if not mark current field as selected (if it is occupied)
							if (auto* selectedPeg = getCurrentSelectedField()) {
								if (peg->getState() == FieldState::Empty) {
									if (m_gameLogic.isValidMove(*selectedPeg, *peg)) {
										m_gameLogic.makeMove(*selectedPeg, *peg);
										syncBoard(); // Update the board based on the move, i.e. the map of fields to their corresponding circles
										if (m_gameLogic.solutionFound()) {
											gameWon.play(); // Play game won sound
											std::cout << "Solution found! Congratulations!" << std::endl;
											gameState = GameState::GameOver; // TODO actually you won, that needs to be adapted later on
											syncBoard();
											m_window.draw(gameWonText); // Draw the game won text
											createTryAgainButton(); // Draw the try again button
											break;
										}
										if (!m_gameLogic.movesAvailable()) {
											gameLost.play(); // Play game lost sound
											std::cout << "No moves available! Game over!" << std::endl;
											gameState = GameState::GameOver;
											syncBoard();
											m_window.draw(gameLostText); // Draw the game lost text
											createTryAgainButton(); // Draw the try again button
											break;
										}
										correctMove.play(); // Play correct move sound (if the game was not won or lost)
									}
									else {
										selectedPeg->setState(FieldState::Occupied);
										syncBoard();
									}
								}
								else {
									selectedPeg->setState(FieldState::Occupied);
									syncBoard();
								}
							}
							else {
								if (peg->getState() == FieldState::Occupied) {
									peg->setState(FieldState::Selected);
									syncBoard();
								}
							}

						}
					}
				}
			}
			else { // If tha game is over, handle the try again button click event
				if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (buttonPressed->button == sf::Mouse::Button::Left) {
						sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
						if (mousePosition.x >= 10 && mousePosition.x <= 160 && mousePosition.y >= 10 && mousePosition.y <= 60) {
							m_gameLogic.resetGame(); // Reset the game logic
							syncBoard(); // Reset the board
							gameState = GameState::Playing; // Change game state back to playing
						}
					}
				}
			}
		}
	}
}
