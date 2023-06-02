// main.cpp being used in place of ZombieArena.cpp

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;


int main()
{

	// Here is the instance of TextureHolder
	TextureHolder holder;

	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	// Create an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is the clock for timing everything
	Clock clock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;

	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background VertexArray
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	// Prepare a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	// 100 bullets to start
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with a crosshair
	window.setMouseCursorVisible(true); // This should be false to hide the mouse when playing the game.
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// About the game
	int score = 0;
	int hiScore = 0;



	// The main game loop
	while (window.isOpen())
	{
		/*
		 ******************
		 Handle Input
		 ******************
		*/

		// Handle events by polling
		Event event;
		while (window.pollEvent(event))
		{
			if(event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if(event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if(event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if(event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if(state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if(bulletsSpare >= clipSize)
						{
							// Plenty of bullets. Go ahead and Reload. 
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if(bulletsSpare > 0)
						{
							// Only a few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else // don't actually need to do anything since they have no bullets left
						{
							// More here soon?
						}
					} // end reloading event.key.code = R;



				}
			}
		} // end event polling loop

		// Handle the player quitting
		if(Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle WASD while playing
		if(state == State::PLAYING)
		{
			// Handle the pressing and release of the WASD keys
			if(Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if(Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if(Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if(Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// Fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					// Pass the center of the player and the center of the crosshair
					// to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					
					bulletsInClip--;
				}
			} // End Fire a bullet

		} // end handle WASD while playing

		// Handle the LEVELING_UP state
		if (state == State::LEVELING_UP)
		{
			// Handle the player LEVELING up
			if(event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if(event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if(event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if(event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if(event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if(event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if(state == State::PLAYING)
			{
				// Prepare the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				// Pass the vertex array by reference to the createBackground function
				int tileSize = createBackground(background, arena); // remember background's reference is passed

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create a horde of zombies
				numZombies = 10;

				// Delete the previously allocated memory (if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// Reset the clock so there isn't a frame jump
				clock.restart();
			} // end level prep when starting PLAYING after LEVELING_UP state.
		} // end handling LEVELING_UP State

		/*
		******************
		UPDATE THE FRAME
		******************
		*/
		if(state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();

			// Update the total game time
			gameTimeTotal += dt;

			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert the mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the player's new position
			Vector2f playerPosition(player.getCenter());

			// Make the view center around the player
			mainView.setCenter(player.getCenter());

			// Loop through each zombie and update them. 
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			} // end update zombies

			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			} // end update bullets

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);


			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							// Stop the bullet
							bullets[i].stop();

							// Register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
									hiScore = score;

								numZombiesAlive--;

								// When all of the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			} // end zombie being shot

		// Have any zombies touched the player?
		for (int i = 0; i < numZombies; i++)
		{
			if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
			{
				if (player.hit(gameTimeTotal))
				{
					// More here later
				}

				if (player.getHealth() <= 0)
				{
					state = State::GAME_OVER;
				}
			}
		} // end player touched 

		// Has the player touched health pickup
		if (player.getPosition().intersects(healthPickup.getPosition()) &&
				healthPickup.isSpawned())
		{
			player.increaseHealthLevel(healthPickup.gotIt());
		}

		// Has the player touched ammo pickup
		if (player.getPosition().intersects(ammoPickup.getPosition()) &&
				ammoPickup.isSpawned())
		{
			bulletsSpare += ammoPickup.gotIt();
		}

		} // end updating the scene

		/* 
		****************
		Draw the scene
		****************
		*/

		if(state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window and draw everything related to it
			window.setView(mainView);

			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			// draw the bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			// Draw the player
			window.draw(player.getSprite());

			// Draw the pickups, if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			// Draw the crosshair
			window.draw(spriteCrosshair);
		}

		if(state == State::LEVELING_UP)
		{
		}

		if(state == State::PAUSED)
		{
		}

		if(state == State::GAME_OVER)
		{
		}

		window.display();

	} // end of main game loop
	
	// Delete the previously allocated memory (if it exists)
	delete[] zombies;

	return 0;
}
