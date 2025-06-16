#include "GameLogic.h"
#include "UserInterface.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


/**
	Implements the game loop that handles events, updates the game state, and renders the UI.
*/
void gameLoop(GameLogic& gameLogic, UserInterface& ui) {
	sf::RenderWindow& window = ui.getRenderWindow();
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


	ui.drawBoard(); // Draw the initial board
	GameState gameState = GameState::Playing;

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (gameState == GameState::Playing) {

				if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (buttonPressed->button == sf::Mouse::Button::Left) {
						// Handle left mouse button pressed events here
						sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
						if (auto* peg = ui.getClickedField(mousePosition)) {
							std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl; // Debugging output

							// Check if there already is a field selected -> if so, check if the move is valid, if not mark current field as selected (if it is occupied)
							if (auto* selectedPeg = ui.getCurrentSelectedField()) {
								if (peg->getState() == FieldState::Empty) {
									if (gameLogic.isValidMove(*selectedPeg, *peg)) {
										gameLogic.makeMove(*selectedPeg, *peg);
										ui.syncBoard(); // Update the board based on the move, i.e. the map of fields to their corresponding circles
										ui.drawBoard();
										if (gameLogic.solutionFound()) {
											gameWon.play(); // Play game won sound
											std::cout << "Solution found! Congratulations!" << std::endl;
											gameState = GameState::GameOver; // TODO actually you won, that needs to be adapted later on
											ui.syncBoard();
											ui.drawBoard();
											window.draw(gameWonText); // Draw the game won text
											ui.createTryAgainButton(); // Draw the try again button
											break;
										}
										if (!gameLogic.movesAvailable()) {
											gameLost.play(); // Play game lost sound
											std::cout << "No moves available! Game over!" << std::endl;
											gameState = GameState::GameOver;
											ui.syncBoard();
											ui.drawBoard();
											window.draw(gameLostText); // Draw the game lost text
											ui.createTryAgainButton(); // Draw the try again button
											break;
										}
										correctMove.play(); // Play correct move sound (if the game was not won or lost)
									}
									else {
										selectedPeg->setState(FieldState::Occupied);
										ui.syncBoard();
										ui.drawBoard();
									}
								}
								else {
									selectedPeg->setState(FieldState::Occupied);
									ui.syncBoard();
									ui.drawBoard();
								}
							}
							else {
								if (peg->getState() == FieldState::Occupied) {
									peg->setState(FieldState::Selected);
									ui.syncBoard();
									ui.drawBoard();
								}
							}

						}
					}
				}
			}
			else { // If tha game is over, handle the try again button click event
				if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
					if (buttonPressed->button == sf::Mouse::Button::Left) {
						sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
						if (mousePosition.x >= 10 && mousePosition.x <= 160 && mousePosition.y >= 10 && mousePosition.y <= 60) {
							gameLogic.resetGame(); // Reset the game logic
							ui.syncBoard(); // Reset the board
							ui.drawBoard();
							gameState = GameState::Playing; // Change game state back to playing
						}
					}
				}
			}
		}
	}
}

int main()
{
	GameLogic gameLogic{}; // Initialize the game logic
	UserInterface ui(gameLogic);

	gameLoop(gameLogic, ui); // Start the game loop
}