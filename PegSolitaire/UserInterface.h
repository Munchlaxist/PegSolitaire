#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include "Field.h"
#include "GameLogic.h"

enum class GameState {
	Playing,
	GameOver
};



class UserInterface {
private:
	sf::RenderWindow m_window{ sf::VideoMode({ 800, 800 }), "Peg Solitaire", sf::Style::Titlebar | sf::Style::Close };
	GameLogic m_gameLogic{};
	std::map<Field*, sf::CircleShape> fieldToShape{};

public:
	UserInterface(GameLogic gameLogic);


	void setBackground(std::filesystem::path filename);

	void drawBoard();

	void updateBoard();

	Field* getClickedPeg(const sf::Vector2i& mousePosition);

	Field* getSelectedPeg();

	void createTryAgainButton();


	void gameLoop();
};