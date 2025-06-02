#include <SFML/Graphics.hpp>
#include <iostream>

static const std::array<std::array<int, 7>, 7> defaultBoard = { {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
} };

void setBackground(sf::RenderWindow& window, std::filesystem::path filename) {
	window.clear();
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("white_oak_bg.png")) {
		// TODO exception that the background could not be loaded
		;
	}
	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(sf::Vector2f(1, 1));
	window.draw(backgroundSprite);
}

void drawInitialBoard(sf::RenderWindow& window) {
	std::array<sf::CircleShape, 33> circles;

	// Set positions for the circles based on a peg solitaire board layout
	std::size_t index = 0;
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			
			if (defaultBoard[row][col] == 1) { // Only place circles where there is a peg
				circles[index].setRadius(20.f);
				circles[index].setFillColor(sf::Color::Blue);
				circles[index].setOutlineColor(sf::Color::Black);
				circles[index].setOutlineThickness(1.f);
				circles[index].setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
				++index;
			}
			if (defaultBoard[row][col] == 0) {
				circles[index].setRadius(20.f);
				circles[index].setFillColor(sf::Color::Transparent);
				circles[index].setOutlineColor(sf::Color::Black);
				circles[index].setOutlineThickness(1.f);
				circles[index].setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
				++index;
			}
			
		}
	}
	
	for (const auto& circle : circles) {
		window.draw(circle);
	}
	window.display();
}



void startGame() {
}


int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Peg Solitaire");

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		window.clear();
		setBackground(window, "white_oak_bg.png");
		drawInitialBoard(window);
	
	}
}