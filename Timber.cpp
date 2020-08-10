#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;


const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPosition[NUM_BRANCHES];

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPosition[j] = branchPosition[j - 1];
	}

	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPosition[0] = side::LEFT;
		break;
	case 1:
		branchPosition[0] = side::RIGHT;
		break;
	default:
		branchPosition[0] = side::NONE;
		break;
	}

}

int main()
{
	//Create a video mode object
	VideoMode vm(1920, 1080);

	//Create and open a window
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	//Create a texture to hold graphic on the GPU
	Texture textureBackground;

	//Load texture into graphic
	textureBackground.loadFromFile("graphics/background.png");

	//Create a sprite
	Sprite spriteBackground;

	//Set sprite background
	spriteBackground.setTexture(textureBackground);

	//Set sprite to over the screen
	spriteBackground.setPosition(0, 0);

	//Make tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//Make bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	//is bee moving and speed of bee
	bool beeActive = false;
	float beeSpeed = 0.0f;
	
	//make cloud
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	//Create 3 instances
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	//cloud Position
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	//cloud moving
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//Cloud speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//time control
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//check if game is paused
	bool paused = true;

	//draw some text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	//Choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Assign font
	messageText.setFont(font);
	scoreText.setFont(font);

	//Assign message
	messageText.setString("Press Enter to Start!!!");
	scoreText.setString("Score = 0");

	//Assign text size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Chose Color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::Red);

	//Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + (textRect.width / 2.0f), textRect.top + (textRect.height / 2.0f));
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Prepare Branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//Add Player Texture
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//Player start position
	side playerSide = side::LEFT;

	//prepare gravestone
	Texture textureGrave;
	textureGrave.loadFromFile("graphics/rip.png");
	Sprite spriteGrave;
	spriteGrave.setTexture(textureGrave);
	spriteGrave.setPosition(700, 860);

	//prepare axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Lineup axe with tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//prepare flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{
		/*
		*****************************************************
		Handle Player inputs
		*****************************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;

				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			score = 0;
			timeRemaining = 5;

			//Make branch dissapear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPosition[i] = side::NONE;
			}

			//Make gravestone hidden
			spriteGrave.setPosition(675, 2000);

			//Move player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}

		if (acceptInput)
		{
			// pressing right key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				chop.play();
			}
		}

		/*
		*****************************************************
		Update the scene
		*****************************************************
		*/
		if (!paused)
		{
			//Measure Time
			Time dt = clock.restart();

			//time remaining
			timeRemaining -= dt.asSeconds();

			//timebar size
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;

				messageText.setString("!!!Out of Time!!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				outOfTime.play();
			}

			//Setup Bee
			if (!beeActive)
			{
				//Bee speed
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				//Bee height
				srand((int)time(0) * 10);
				float height = (rand() % 500 + 500);
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				//Move the bee
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				//Has bee reached edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			//Setup cloud 1
			if (!cloud1Active)
			{
				//Manage speed 
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//Manage height of the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}
			//Setup cloud 2
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}
			//Setup cloud 3
			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}
			//update score
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//update branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPosition[i] == side::LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}

				else if (branchPosition[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

			//Flying logs
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), 
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			//Player Death
			if (branchPosition[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteGrave.setPosition(525, 760);
				spritePlayer.setPosition(2000, 660);
				messageText.setString("SQUISHED!!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}
		}

		/*
		*****************************************************
		Draw the scene
		*****************************************************
		*/
		//Clear everything from the frame
		window.clear();

		//Draw the scene here
		window.draw(spriteBackground);

		//Draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		//Draw Branhes
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		//Draw tree
		window.draw(spriteTree);

		//Draw player
		window.draw(spritePlayer);

		//Draw axe
		window.draw(spriteAxe);

		//Draw flying log
		window.draw(spriteLog);

		//Draw gravestone
		window.draw(spriteGrave);

		//Draw bee
		window.draw(spriteBee);

		//Draw text
		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}

		//Show everything that is drawn
		window.display();
	}
	return 0;
}
