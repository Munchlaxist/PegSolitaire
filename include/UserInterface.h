#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Field.h"
#include "GameLogic.h"
#include "Move.h"


/**
	Specifies the user interface for the Peg Solitaire game (Prototype!).
*/
class UserInterface {
private:
	GameLogic& m_gameLogic; // The game logic instance that manages the game state and rules
	sf::RenderWindow m_window{ sf::VideoMode({ 800, 800 }), "Peg Solitaire", sf::Style::Titlebar | sf::Style::Close }; // The main window for the game
	std::map<Field*, sf::CircleShape> fieldToShape{}; // Maps the game fields to their corresponding graphical shapes

public:
	/**
		Constructor for the UserInterface class.
		\param gameLogic The GameLogic instance that manages the game state and logic
	*/
	UserInterface(GameLogic& gameLogic);

	/**
		Gets the render window used for drawing the game UI.
		\return Reference to the RenderWindow used for rendering the game UI
	*/
	sf::RenderWindow& getRenderWindow();

	/**
		Draws the background of the game window.
		\param filename The relative path to the background image file
	*/
	void drawBackground(std::filesystem::path filename);

	/**
		Displays the current state of the game board.
	*/
	void drawBoard();

	/**
		Updates the state of the game board within the UI based on the previous executed move by the game logic.
	*/
	void updateBoard();

	/**
		Get the field that was clicked based on the mouse position.
		\param mousePosition The position of the mouse when the click occurred
		\return Pointer to the Field that was clicked, or nullptr if no field was clicked
	*/
	Field* getClickedField(const sf::Vector2i& mousePosition);

	/**
		Get the currently selected field whose peg should make a move next (if one exists).
		\return Pointer to the currently selected Field, or nullptr if no Field is selected
	*/
	Field* getCurrentSelectedField();

	/**
		Create and draw a try again button on the game window.
	*/
	void drawTryAgainButton();

	/**
		Create and draw a game over text on the game window.
	*/
	void drawGameOverText();

	/**
		Create and draw a game won text on the game window.
	*/
	void drawGameWonText();

	/**
		Highlights the next possible move to find a valid solution.
	*/
	void highlightHint(Move& move);

	/**
		Clears, draws and displays the game window for the next frame.
	*/
	void render();

	/**
		Resets the field shapes to match the current type of the game board.
	*/
	void resetFieldToShape();

};