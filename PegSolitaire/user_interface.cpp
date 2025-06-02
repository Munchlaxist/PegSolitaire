#include <SFML/Graphics.hpp>

static const std::array<std::array<int, 7>, 7> defaultBoard = { {
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{ 1,  1, 1, 0, 1,  1,  1},
	{ 1,  1, 1, 1, 1,  1,  1},
	{-1, -1, 1, 1, 1, -1, -1},
	{-1, -1, 1, 1, 1, -1, -1},
} };

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Peg Solitaire");
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("white_oak_bg.png")) {
		return -1;
	}
	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(sf::Vector2f(1, 1));

	std::array<sf::CircleShape, 32> circles;
	for (std::size_t i = 0; i < circles.size(); ++i) {
		circles[i].setRadius(20.f);
		circles[i].setFillColor(sf::Color::Blue);
		circles[i].setOutlineColor(sf::Color::Black);
		circles[i].setOutlineThickness(1.f);
	}
	// Set positions for the circles based on a peg solitaire board layout
	std::size_t index = 0;
	for (std::size_t row = 0; row < 7; ++row) {
		for (std::size_t col = 0; col < 7; ++col) {
			if (defaultBoard[row][col] == 1) { // Only place circles where there is a peg
				circles[index].setPosition(sf::Vector2f(225 + col * 50, 225 + row * 50)); // Adjust position based on row and column
				++index;
			}
		}
	}

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		window.draw(backgroundSprite);
		for (const auto& circle : circles) {
			window.draw(circle);
		}
		window.display();
	}
}