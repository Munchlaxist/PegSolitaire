#pragma once
#include <SFML/Audio.hpp>


/**
	Manages sound effects to efficiently handle audio playback in the game.
*/
class SoundManager {
private:
	// Initialize sound buffers and sounds here such that they can be efficiently reused
	sf::SoundBuffer correctMoveBuffer;
	sf::SoundBuffer undoMoveBuffer;
	sf::SoundBuffer winBuffer;
	sf::SoundBuffer loseBuffer;

	sf::Sound correctMoveSound{ correctMoveBuffer };
	sf::Sound winSound{ winBuffer };
	sf::Sound loseSound{ loseBuffer };
	sf::Sound undoMoveSound{ undoMoveBuffer };

public:
	SoundManager();

	/**
		Play a sound when the player makes a correct move.
	*/
	void playCorrectMoveSound();

	/**
		Play a sound when the player wins the game.
	*/
	void playGameWonSound();

	/**
		Play a sound when the player loses the game.
	*/
	void playGameLostSound();

	/**
		Play a sound when the player undoes a move.
	*/
	void playUndoMoveSound();

};