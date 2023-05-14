#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
	private:
		// How fast is each Zombie type?
		const float BLOATER_SPEED = 40;
		const float CHASER_SPEED = 80;
		const float CRAWLER_SPEED = 20;

		// How tough is each zombie type?
		const float BLOATER_HEALTH = 5;
		const float CHASER_HEALTH = 1;
		const float CRAWLER_HEALTH = 3;

		// Make each zombie vary its speed slightly
		const int MAX_VARIANCE = 30;
		const int OFFSET = 101 - MAX_VARIANCE;

		// Where is this zombie?
		Vector2f m_Position;

		// a Sprite for the zombie
		Sprite m_Sprite;

		// How fast can this one run/crawl?
		float m_Speed;

		// How much health has it got?
		float m_Health;

		// Is it still alive?
		bool m_Alive;

		// public prototypes
	public:
		bool hit();// Handle when a bullet hits a zombie 
		bool isAlive(); // Find out if zombie is alive
		void spawn(float startX, float startY, int type, int seed); // spawn a new zombie
		FloatRect getPosition(); // return a rectangle that is the position in the world
		Sprite getSprite(); // get a copy of the sprite to draw
		void update(float elapsedTime, Vector2f playerLocation); // update the zombie each frame

};
