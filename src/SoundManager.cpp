#include <SFML/Audio.hpp>
#include "SoundManager.h"
#include <stdexcept>


SoundManager::SoundManager() {
	// Load sound buffers from sound files (files received by Pixabay)
	if (!correctMoveBuffer.loadFromFile("assets/sounds/correct_move.mp3")) {
		throw std::runtime_error("Failed to load correct move sound.");
	}
	if (!winBuffer.loadFromFile("assets/sounds/game_won.mp3")) {
		throw std::runtime_error("Failed to load win sound.");
	}
	if (!loseBuffer.loadFromFile("assets/sounds/game_lost.mp3")) {
		throw std::runtime_error("Failed to load lose sound.");
	}
}

void SoundManager::playCorrectMoveSound() {
	correctMoveSound.play();
}

void SoundManager::playGameWonSound() {
	winSound.play();
}

void SoundManager::playGameLostSound() {
	loseSound.play();
}
