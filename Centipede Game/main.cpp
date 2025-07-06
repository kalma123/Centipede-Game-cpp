#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;	  // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid
int centipede[12][3];
int length = 12;
bool inPlayerArea = false;
float player[2] = {};
// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};
bool movingleft = true;
bool movingdown = false;
bool canMove = true;
// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;
bool canShoot = false; //bullet status
int playerScore = 0;
bool alive = true;
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void shoot(float bullet[], sf::Clock& bulletClock, bool& canShoot, float player[]);
void initializeMushrooms();
void drawMushrooms(sf::RenderWindow& window);
void drawCentipede(sf::RenderWindow& window, sf::Texture& centipedeTexture, float centipede[][3], int centipedeLength);
void drawCentipede(sf::RenderWindow& window, int row, int col);
void moveCentipede(bool& movingleft, bool& canMove);
void moveCentipedeInPlayerArea(bool& movingleft, bool& canMove, bool& movingdown);

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures\Background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	player[x] = (gameRows / 2) * boxPixelsX;
	player[y] = (gameColumns - 3) * boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	initializeMushrooms();
	for (int i = 0; i < length; ++i)
	{
		centipede[i][x] = (30 - length + i) * boxPixelsX; // Initial x position
		centipede[i][y] = 0;							  // Initial y position
		centipede[i][exists] = true;					  // The segment exists
	}
	while (window.isOpen())
	{

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		window.draw(backgroundSprite);
		drawMushrooms(window);
		if (alive && length)
		{
			drawPlayer(window, player, playerSprite);
		}
		else
		{
			window.clear();
			sf::Font font;
			font.loadFromFile("font.otf");

			sf::Text text;
			text.setFont(font);
			text.setString("Score: " + std::to_string(playerScore));
			text.setCharacterSize(50);
			text.setFillColor(sf::Color::Red);
			text.setStyle(sf::Text::Bold);
			text.setPosition(sf::Vector2f(250, 480));
			window.draw(text);
		}

		if (length && alive)
		{
			if (!inPlayerArea)
			{
				moveCentipede(movingleft, canMove);
			}
			else
			{
				moveCentipedeInPlayerArea(movingleft, canMove, movingdown);
			}
			sf::Sprite centipedeSprit;
			sf::Texture centipedeTexture;
			centipedeTexture.loadFromFile("Textures/c_head_left_walk.png");
			centipedeSprit.setTexture(centipedeTexture);
			centipedeSprit.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
			centipedeSprit.setPosition(centipede[0][x], centipede[0][y]);
			window.draw(centipedeSprit);

			sf::Sprite centipedeSprit1;
			sf::Texture centipedeTexture1;
			for (int i = 1; i < length - 1; i++)
			{
				centipedeTexture1.loadFromFile("Textures/c_body_left_walk.png");
				centipedeSprit1.setTexture(centipedeTexture1);
				centipedeSprit1.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
				centipedeSprit1.setPosition(centipede[i][x], centipede[i][y]);
				centipedeSprit1.setColor(sf::Color::Blue);
				window.draw(centipedeSprit1);
			}
		}
		else
		{
			window.clear();
			sf::Font font;
			font.loadFromFile("font.otf");

			sf::Text text;
			text.setFont(font);
			text.setString("Score: " + std::to_string(playerScore));
			text.setCharacterSize(50);
			text.setFillColor(sf::Color::Red);
			text.setStyle(sf::Text::Bold);
			text.setPosition(sf::Vector2f(250, 480));
			window.draw(text);
		}

		canMove = true;


		bool leftKeyPressed = false;
		bool rightKeyPressed = false;
		bool upKeyPressed = false;
		bool downKeyPressed = false;
		bool spaceKeyPressed = false;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			// Check for key pressed events
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					leftKeyPressed = true;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					rightKeyPressed = true;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					upKeyPressed = true;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					downKeyPressed = true;
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					spaceKeyPressed = true;
				}
			}

			// Check for key released events
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					leftKeyPressed = false;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					rightKeyPressed = false;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					upKeyPressed = false;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					downKeyPressed = false;
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					spaceKeyPressed = false;
				}
			}

			// Update player position based on key presses
			if (leftKeyPressed && player[x] - 32 >= 0)
			{
				player[x] -= 32;
			}
			if (rightKeyPressed && player[x] + 32 <= 960 - 32)
			{
				player[x] += 32;
			}
			if (upKeyPressed && player[y] - 32 > boxPixelsY * 24)
			{
				player[y] -= 32;
			}
			if (downKeyPressed && player[y] + 32 <= 960 - 32)
			{
				player[y] += 32;
			}
		}
		shoot(bullet, bulletClock, canShoot, player);
		if (bullet[exists])
		{
			drawBullet(window, bullet, bulletSprite);
			moveBullet(bullet, bulletClock);
		}
		window.display();
		window.clear();
	}
}
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void moveCentipedeInPlayerArea(bool& movingleft, bool& canMove, bool& movingdown)
{

	if (movingleft && canMove)
	{
		canMove = false;
		int temp = centipede[0][x];
		if (centipede[0][x] > 0)
		{
			for (int i = 0; i < 12; i++)
			{
				centipede[i][x] -= 1;
			}
		}
		else
		{
			if (movingdown) {
				if (centipede[0][y] + 32 < 960)
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] += 32;
					}
					movingleft = false;
				}
				else
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] -= 32;
					}
					movingdown = false;
					movingleft = false;
				}
			}
			if (!movingdown) {
				if (centipede[0][y] - 32 > 768)
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] -= 32;
					}
					movingleft = false;
				}
				else
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] += 32;
					}
					movingdown = true;
					movingleft = false;
				}
			}
		}
	}
	else if (!movingleft && canMove)
	{
		canMove = false;
		if (centipede[0][x] <= 960 - ((length - 1) * 32))
		{
			for (int i = 0; i < 12; i++)
			{
				centipede[i][x] += 1;
			}
		}
		else
		{
			if (movingdown) {
				if (centipede[0][y] + 32 < 960)
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] += 32;
					}
					movingleft = true;
				}
				else
				{
					for (int i = 0; i < 12; i++)
					{
						centipede[i][y] -= 32;
					}
					movingleft = true;
					movingdown = false;
				}
			}
		}
	}
	int row = centipede[0][x] / 32;
	int col = centipede[0][y] / 32;
	if (gameGrid[row][col] > 0)
	{
		if (movingdown) {
			if (centipede[0][y] + 32 < 960)
			{
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] += 32;
				}
				movingleft = !movingleft;
			}
			else {
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] -= 32;
				}
				movingleft = !movingleft;
				movingdown = false;
			}
		}
		else {
			if (centipede[0][y] - 32 > 768)
			{
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] -= 32;
				}
				movingleft = !movingleft;
			}
			else {
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] += 32;
				}
				movingleft = !movingleft;
				movingdown = true;
			}
		}

	}
	if (centipede[0][x] == player[x] && centipede[0][y] == player[y])
	{
		alive = false;
	}
}
// Modify the moveCentipede function to move towards the left
void moveCentipede(bool& movingleft, bool& canMove)
{

	if (movingleft && canMove)
	{
		canMove = false;
		int temp = centipede[0][x];
		if (centipede[0][x] > 0)
		{
			for (int i = 0; i < 12; i++)
			{
				centipede[i][x] -= 1;
			}
		}
		else
		{
			if (centipede[0][y] + 32 < 960)
			{
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] += 32;
				}
				movingleft = false;
			}
			else
			{
				inPlayerArea = true;
			}
		}
	}
	else if (!movingleft && canMove)
	{
		canMove = false;
		if (centipede[0][x] <= 960 - ((length - 1) * 32))
		{
			for (int i = 0; i < 12; i++)
			{
				centipede[i][x] += 1;
			}
		}
		else
		{
			if (centipede[0][y] + 32 < 960)
			{
				for (int i = 0; i < 12; i++)
				{
					centipede[i][y] += 32;
				}
				movingleft = true;
			}
			else
			{
				inPlayerArea = true;
				movingleft = true;
			}
		}
	}
	int row = centipede[0][x] / 32;
	int col = centipede[0][y] / 32;
	if (gameGrid[row][col] > 0)
	{
		if (centipede[0][y] + 32 < 960)
		{
			for (int i = 0; i < 12; i++)
			{
				centipede[i][y] += 32;
			}
			movingleft = !movingleft;
		}
	}
	if (centipede[0][x] == player[x] && centipede[0][y] == player[y])
	{
		alive = false;
	}
}

void drawCentipede(sf::RenderWindow& window, int row, int col)
{
	sf::Sprite centipedeSprit;
	sf::Texture centipedeTexture;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	centipedeSprit.setTexture(centipedeTexture);
	centipedeSprit.setTextureRect(sf::IntRect(0, 0, boxPixelsX * 7, boxPixelsY));
	centipedeSprit.setPosition(row * boxPixelsX, col * boxPixelsY);
	centipedeSprit.setColor(sf::Color::Blue);
	window.draw(centipedeSprit);
}
void destroyMushroom(float bullet[], int row, int col)
{
	if (gameGrid[row][col] == 1)
	{
		gameGrid[row][col] = 0;
		playerScore += 1; // Increment the score when a mushroom is destroyed
	}
	if (gameGrid[row][col] == 2)
	{
		gameGrid[row][col] = 1;
	}
	bullet[2] = false;
}
void drawMushrooms(sf::RenderWindow& window)
{
	for (int i = 0; i < gameRows; ++i)
	{
		for (int j = 0; j < gameColumns; ++j)
		{
			if (gameGrid[i][j] == 2)
			{

				sf::Texture mushroom;
				sf::Sprite mushroomSprite;
				mushroom.loadFromFile("Textures/mushroom.png");
				mushroomSprite.setTexture(mushroom);
				mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
				mushroomSprite.setPosition(i * boxPixelsX, j * boxPixelsY);
				mushroomSprite.setColor(sf::Color::Yellow);
				window.draw(mushroomSprite);
			}
			else if (gameGrid[i][j] == 1)
			{

				sf::Texture mushroom;
				sf::Sprite mushroomSprite;
				mushroom.loadFromFile("Textures/mushroom.png");
				mushroomSprite.setTexture(mushroom);
				mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
				mushroomSprite.setPosition(i * boxPixelsX, j * boxPixelsY);
				mushroomSprite.setColor(sf::Color::Red);
				window.draw(mushroomSprite);
			}
		}
	}
}
void initializeMushrooms()
{
	int numMushrooms = rand() % 20 + 30;
	for (int i = 0; i < numMushrooms; ++i)
	{
		int randX = rand() % gameRows;
		int randY = rand() % gameColumns;
		gameGrid[randX][randY] = 2;
	}
}

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite)
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[], sf::Clock& bulletClock)
{
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet[y] -= 16;
	int row = bullet[x] / 32;
	int col = bullet[y] / 32;
	if (gameGrid[row][col])
	{
		destroyMushroom(bullet, row, col);
	}
	if ((bullet[x] > centipede[0][x] - 16) && (bullet[x] < centipede[0][x] + 16) && (bullet[y] > centipede[0][y] - 16) && (bullet[y] < centipede[0][y] + 16))
	{
		bullet[exists] = false;

		length -= 1;
		playerScore += 20;
		cout << "hit head:" << playerScore << endl;
		alive = false;

	}
	if ((bullet[x] > centipede[1][x] - 16) && (bullet[x] < centipede[length - 1][x] + 16) && (bullet[y] > centipede[0][y] - 16) && (bullet[y] < centipede[0][y] + 16))
	{
		bullet[exists] = false;
		length -= 1;
		playerScore += 10;
		cout << "hit body:" << playerScore << endl;
	}
	if (bullet[y] < -8)
		bullet[exists] = false;
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite)
{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void shoot(float bullet[], sf::Clock& bulletClock, bool& canShoot, float player[])
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot)
	{
		// Shoot if the spacebar is pressed and the player can shoot
		bullet[x] = player[x];
		bullet[y] = player[y];
		bullet[exists] = true;
		canShoot = false; // Disable shooting temporarily
		int row = bullet[x] / 32;
		int col = bullet[y] / 32;
		if (gameGrid[row][col])
		{
			destroyMushroom(bullet, row, col);
		}
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		canShoot = true; // Enable shooting when space bar is released
	}
}
