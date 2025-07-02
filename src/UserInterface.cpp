#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdexcept>
#include "UserInterface.h"
#include "Move.h"


UserInterface::UserInterface(GameLogic& gameLogic) : m_gameLogic{ gameLogic } {
	// Initialize the user interface and the board
	for (Field& field : m_gameLogic.getBoard()) {
		sf::CircleShape circle(20.f); // Every game field is represented by a circle with radius 20 pixels
		if (field.getState() == FieldState::Occupied) {
			circle.setFillColor(sf::Color::Blue);
			circle.setOutlineColor(sf::Color::Black);
			circle.setOutlineThickness(1.f);
			circle.setPosition(sf::Vector2f(static_cast<float>(225 + field.getPosition().second * 50), static_cast<float>(225 + field.getPosition().first * 50))); // Adjust position based on the specific field
			fieldToShape[&field] = circle; // Map the field to its circle representation
		}
		else if (field.getState() == FieldState::Empty) {
			circle.setFillColor(sf::Color::Transparent);
			circle.setOutlineColor(sf::Color::Black);
			circle.setOutlineThickness(1.f);
			circle.setPosition(sf::Vector2f(static_cast<float>(225 + field.getPosition().second * 50), static_cast<float>(225 + field.getPosition().first * 50)));
			fieldToShape[&field] = circle;
		}
	}
}

sf::RenderWindow& UserInterface::getRenderWindow() {
	return m_window;
}

void UserInterface::drawBackground(std::filesystem::path filename) {
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
	for (auto& field : m_gameLogic.getBoard()) {
		m_window.draw(fieldToShape[&field]);
	}
}

void UserInterface::updateBoard() {
	for (auto& field : m_gameLogic.getBoard()) {
		if (field.getState() == FieldState::Occupied) {
			fieldToShape[&field].setFillColor(sf::Color::Blue); // Change color to indicate occupied field
		}
		else if (field.getState() == FieldState::Empty) {
			fieldToShape[&field].setFillColor(sf::Color::Transparent); // Change color to indicate empty field
		}
		else if (field.getState() == FieldState::Selected) {
			fieldToShape[&field].setFillColor(sf::Color::Red); // Change color to indicate selected field
		}
		fieldToShape[&field].setOutlineColor(sf::Color::Black); // Change color to indicate occupied field
	}
}

Field* UserInterface::getClickedField(const sf::Vector2i& mousePosition) {
	for (auto& field : m_gameLogic.getBoard()) {
		if (fieldToShape[&field].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			return &field; // Return reference to the clicked field
		}
	}
	return nullptr;
}

Field* UserInterface::getCurrentSelectedField() {
	for (auto& field : m_gameLogic.getBoard()) {
		if (field.getState() == FieldState::Selected) {
			return &field; // Return reference to the selected field
		}
	}
	return nullptr;
}

void UserInterface::drawTryAgainButton() {
	sf::RectangleShape tryAgainButton(sf::Vector2f(150.f, 50.f)); // Create a button with size 150x50 pixels
	tryAgainButton.setFillColor(sf::Color::Color(47, 126, 244)); // RBG color
	tryAgainButton.setOutlineColor(sf::Color::Black);
	tryAgainButton.setOutlineThickness(2.f);
	tryAgainButton.setPosition(sf::Vector2f(10.f, 10.f));
	sf::Font font{}; // load a font for the button text
	if (font.openFromFile("assets/fonts/arial.ttf")) {
		sf::Text buttonText(font, "Try Again!", 30);
		buttonText.setFillColor(sf::Color::White);
		buttonText.setPosition(sf::Vector2f(tryAgainButton.getPosition().x + 10.f, tryAgainButton.getPosition().y + 8.f));
		m_window.draw(tryAgainButton);
		m_window.draw(buttonText);
	}
}

void UserInterface::drawGameOverText() {
	sf::Font font;
	if (!font.openFromFile("assets/fonts/arial.ttf")) {
		throw std::runtime_error("Could not load font for game over text message.");
	}
	sf::Text gameLostText(font, "You lost, try again!", 50);
	gameLostText.setFillColor(sf::Color::Red); // Set text color
	gameLostText.setPosition(sf::Vector2f(200.f, 20.f)); // Set text position
	m_window.draw(gameLostText);
}

void UserInterface::drawGameWonText() {
	sf::Font font;
	if (!font.openFromFile("assets/fonts/arial.ttf")) {
		throw std::runtime_error("Could not load font for game won text message.");
	}
	sf::Text gameWonText(font, "Congratulations!", 50);
	gameWonText.setFillColor(sf::Color::Red); // Set text color
	gameWonText.setPosition(sf::Vector2f(200.f, 20.f)); // Set text position
	m_window.draw(gameWonText);
}

void UserInterface::highlightHint(Move2& move) {
	switch (m_gameLogic.getBoardType()) {
	case BoardType::English:
		for (Field& field : m_gameLogic.getBoard()) {
			if (m_gameLogic.englishGridIdxMap.at(field.getPosition()) == move.from) {
				fieldToShape[&field].setFillColor(sf::Color::Yellow);
			}
			if (m_gameLogic.englishGridIdxMap.at(field.getPosition()) == move.to) {
				fieldToShape[&field].setOutlineColor(sf::Color::Yellow);
			}
		}
		break;

	case BoardType::European:
		for (Field& field : m_gameLogic.getBoard()) {
			if (m_gameLogic.europeanGridIdxMap.at(field.getPosition()) == move.from) {
				fieldToShape[&field].setFillColor(sf::Color::Yellow);
			}
			if (m_gameLogic.europeanGridIdxMap.at(field.getPosition()) == move.to) {
				fieldToShape[&field].setOutlineColor(sf::Color::Yellow);
			}
		}
		break;
	case BoardType::Asymmetric:
		for (Field& field : m_gameLogic.getBoard()) {
			if (m_gameLogic.asymmetricGridIdxMap.at(field.getPosition()) == move.from) {
				fieldToShape[&field].setFillColor(sf::Color::Yellow);
			}
			if (m_gameLogic.asymmetricGridIdxMap.at(field.getPosition()) == move.to) {
				fieldToShape[&field].setOutlineColor(sf::Color::Yellow);
			}
		}
		break;
	case BoardType::SmallDiamond:
		for (Field& field : m_gameLogic.getBoard()) {
			if (m_gameLogic.smallDiamondGridIdxMap.at(field.getPosition()) == move.from) {
				fieldToShape[&field].setFillColor(sf::Color::Yellow);
			}
			if (m_gameLogic.smallDiamondGridIdxMap.at(field.getPosition()) == move.to) {
				fieldToShape[&field].setOutlineColor(sf::Color::Yellow);
			}
		}
		break;
	}
}

void UserInterface::render() {
	m_window.clear();
	drawBackground("assets/images/white_oak_bg.png");
	drawBoard();
	if (m_gameLogic.getCurrentGameState() == GameState::GameLost) {
		drawGameOverText();
		drawTryAgainButton();
	} else if (m_gameLogic.getCurrentGameState() == GameState::GameWon) {
		drawGameWonText();
		drawTryAgainButton();
	}
	m_window.display();
}

void UserInterface::resetFieldToShape() {
	fieldToShape.clear();
	for (Field& field : m_gameLogic.getBoard()) {
		sf::CircleShape circle(20.f); // Every game field is represented by a circle with radius 20 pixels
		switch (m_gameLogic.getBoardType()) {
		case BoardType::English:
		case BoardType::European:
		case BoardType::SmallDiamond:
		case BoardType::Asymmetric:
			if (field.getState() == FieldState::Occupied) {
				circle.setFillColor(sf::Color::Blue);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(static_cast<float>(225 + field.getPosition().second * 50), static_cast<float>(225 + field.getPosition().first * 50))); // Adjust position based on the specific field
				fieldToShape[&field] = circle; // Map the field to its circle representation
			}
			else if (field.getState() == FieldState::Empty) {
				circle.setFillColor(sf::Color::Transparent);
				circle.setOutlineColor(sf::Color::Black);
				circle.setOutlineThickness(1.f);
				circle.setPosition(sf::Vector2f(static_cast<float>(225 + field.getPosition().second * 50), static_cast<float>(225 + field.getPosition().first * 50)));
				fieldToShape[&field] = circle;
			}
			break;
		default:
			throw std::runtime_error("Unsupported board type for peg solitaire.");
			break;
		}
	}
}
