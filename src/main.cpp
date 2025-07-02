#include "GameLogic.h"
#include "UserInterface.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
//#include "Solver.cpp"
#include <iostream>
#include <chrono>
#include "NewSolver.cpp"


/**
	handles events such as mouse clicks and window closing.
*/
static void handleEvents(sf::RenderWindow& window, GameLogic& gameLogic, UserInterface& ui, SoundManager& soundManager) {
	while (const std::optional event = window.pollEvent()) {
		// event for closing the window
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}

		if (gameLogic.getCurrentGameState() == GameState::Playing) {
			// Handle event to undo the previous move when Z is pressed
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::Z and !gameLogic.getMoveHistory().empty()) {
					gameLogic.undoMove();
					ui.updateBoard(); // Update the board after undoing the move
					soundManager.playUndoMoveSound();
				}
			}

			// Change board representation
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::A) {
					gameLogic.setBoardType(BoardType::English);
					gameLogic.resetGame();
					ui.resetFieldToShape();
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::S) {
					gameLogic.setBoardType(BoardType::European);
					gameLogic.resetGame();
					ui.resetFieldToShape();
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::D) {
					gameLogic.setBoardType(BoardType::SmallDiamond);
					gameLogic.resetGame();
					ui.resetFieldToShape();
				}
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::F) {
					gameLogic.setBoardType(BoardType::Asymmetric);
					gameLogic.resetGame();
					ui.resetFieldToShape();
				}
			}

			// Handle event to give a hint for the next move when H is pressed
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->code == sf::Keyboard::Key::H) {
					switch(gameLogic.getBoardType()) {
					case BoardType::English:
						{
							uint64_t newBoard = gameLogic.convertBoardToSolverBoardFormat();
							EnglishBoardSolver solver(newBoard);
							std::chrono::milliseconds timeout(25000);
							const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
							if (solver.solve(startTime, timeout)) {
								std::cout << "Done computing" << std::endl;
								std::vector<Move2>& moves = solver.getSolutionPath();
								ui.highlightHint(moves[0]); // moves[0] contains the next move of the solution found
							}
							break;
						}
					case BoardType::European:
						{
							uint64_t newBoard = gameLogic.convertBoardToSolverBoardFormat();
							EuropeanBoardSolver solver(newBoard);
							std::chrono::milliseconds timeout(50000);
							const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
							if (solver.solve(startTime, timeout)) {
								std::cout << "Done computing" << std::endl;
								std::vector<Move2>& moves = solver.getSolutionPath();
								ui.highlightHint(moves[0]); // moves[0] contains the next move of the solution found
							}
							break;
						}
					case BoardType::SmallDiamond:
						{
							uint64_t newBoard = gameLogic.convertBoardToSolverBoardFormat();
							SmallDiamondBoardSolver solver(newBoard);
							std::chrono::milliseconds timeout(25000);
							const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
							if (solver.solve(startTime, timeout)) {
								std::cout << "Done computing" << std::endl;
								std::vector<Move2>& moves = solver.getSolutionPath();
								ui.highlightHint(moves[0]); // moves[0] contains the next move of the solution found
							}
							break;
						}
					case BoardType::Asymmetric:
						{
							uint64_t newBoard = gameLogic.convertBoardToSolverBoardFormat();
							AsymmetricBoardSolver solver(newBoard);
							std::chrono::milliseconds timeout(25000);
							const std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
							if (solver.solve(startTime, timeout)) {
								std::cout << "Done computing" << std::endl;
								std::vector<Move2>& moves = solver.getSolutionPath();
								ui.highlightHint(moves[0]); // moves[0] contains the next move of the solution found
							}
							break;
						}
					default:
						break;
					}
				}
			}
			// Handle mouse button pressed events to do moves
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
									ui.updateBoard(); // Update the board based on the move, i.e. the map of fields to their corresponding circles
									if (gameLogic.solutionFound()) {
										soundManager.playGameWonSound(); // Play game won sound
										std::cout << "Solution found! Congratulations!" << std::endl;
										gameLogic.setGameState(GameState::GameWon);
										break;
									}
									if (!gameLogic.movesAvailable()) {
										soundManager.playGameLostSound(); // Play game lost sound
										std::cout << "No moves available! Game over!" << std::endl;
										gameLogic.setGameState(GameState::GameLost);
										break;
									}
									soundManager.playCorrectMoveSound(); // Play correct move sound (if the game was not won or lost)
								} else {
									selectedField->setState(FieldState::Occupied);
									ui.updateBoard();
								}
							} else {
								selectedField->setState(FieldState::Occupied);
								ui.updateBoard();
							}
						} else {
							if (field->getState() == FieldState::Occupied) {
								field->setState(FieldState::Selected);
								ui.updateBoard();
							}
						}
					}
				}
			}
		}
		// Handle button pressed events to try again when the game is over
		else {
			if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (buttonPressed->button == sf::Mouse::Button::Left) {
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (mousePosition.x >= 10 && mousePosition.x <= 160 && mousePosition.y >= 10 && mousePosition.y <= 60) {
						gameLogic.resetGame(); // Reset the game logic to default state
						ui.updateBoard(); // Reset the board
					}
				}
			}
		}
	}
}


/**
	Implements the game loop that handles events, updates the game state, and renders the UI.
*/
static void gameLoop(GameLogic& gameLogic, UserInterface& ui, SoundManager& soundManager) {
	sf::RenderWindow& window = ui.getRenderWindow();

	while (window.isOpen()) {
		ui.render();
		handleEvents(window, gameLogic, ui, soundManager);
	}
}

int main() {
	GameLogic gameLogic{}; // Initialize the game logic
	UserInterface ui(gameLogic); // Initialize the user interface with the game logic
	SoundManager soundManager; // Initialize the sound manager

	gameLoop(gameLogic, ui, soundManager); // Start the game loop
}