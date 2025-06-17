#include "GameLogic.h"
#include "UserInterface.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


/**
	Implements the game loop that handles events, updates the game state, and renders the UI.
*/
void gameLoop(GameLogic& gameLogic, UserInterface& ui, SoundManager& soundManager) {
	sf::RenderWindow& window = ui.getRenderWindow();
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
						if (auto* field = ui.getClickedField(mousePosition)) {
							std::cout << "Mouse button pressed at: " << mousePosition.x << ", " << mousePosition.y << std::endl; // Debugging output

							// Check if there already is a field selected -> if so, check if the move is valid, if not mark current field as selected (if it is occupied)
							if (auto* selectedField = ui.getCurrentSelectedField()) {
								if (field->getState() == FieldState::Empty) {
									if (gameLogic.isValidMove(*selectedField, *field)) {
										gameLogic.makeMove(*selectedField, *field);
										ui.syncBoard(); // Update the board based on the move, i.e. the map of fields to their corresponding circles
										ui.drawBoard();
										if (gameLogic.solutionFound()) {
											soundManager.playGameWonSound(); // Play game won sound
											std::cout << "Solution found! Congratulations!" << std::endl;
											gameState = GameState::GameOver; // TODO actually you won, that needs to be adapted later on
											ui.syncBoard();
											ui.drawBoard();
											ui.displayGameWonText(); // Draw the game won text
											ui.displayTryAgainButton(); // Draw the try again button
											break;
										}
										if (!gameLogic.movesAvailable()) {
											soundManager.playGameLostSound(); // Play game lost sound
											std::cout << "No moves available! Game over!" << std::endl;
											gameState = GameState::GameOver;
											ui.syncBoard();
											ui.drawBoard();
											ui.displayGameOverText(); // Draw the game lost text
											ui.displayTryAgainButton(); // Draw the try again button
											break;
										}
										soundManager.playCorrectMoveSound(); // Play correct move sound (if the game was not won or lost)
									}
									else {
										selectedField->setState(FieldState::Occupied);
										ui.syncBoard();
										ui.drawBoard();
									}
								}
								else {
									selectedField->setState(FieldState::Occupied);
									ui.syncBoard();
									ui.drawBoard();
								}
							}
							else {
								if (field->getState() == FieldState::Occupied) {
									field->setState(FieldState::Selected);
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
	UserInterface ui(gameLogic); // Initialize the user interface with the game logic
	SoundManager soundManager; // Initialize the sound manager

	gameLoop(gameLogic, ui, soundManager); // Start the game loop
}