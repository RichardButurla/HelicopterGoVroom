/// <summary>
///Richard Buturla
/// 
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouse(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processMouse(sf::Event t_event)
{
	float length = 0.0f;
	sf::Vector2f displacement;

	if (sf::Mouse::Left == t_event.mouseButton.button)
	{
		m_target = sf::Vector2f(t_event.mouseButton.x, t_event.mouseButton.y);
		displacement.x = static_cast<float>(t_event.mouseButton.x) - m_location.x;
		displacement.y = static_cast<float>(t_event.mouseButton.y) - m_location.y;
		length = std::sqrtf((displacement.x * displacement.x) + (displacement.y * displacement.y));
		displacement = displacement / length;
		m_velocity = displacement;
		m_velocity = m_velocity * m_speed;
		m_increment = 0.79;
		if (static_cast<float>(t_event.mouseButton.x) > m_location.x)
		{
			m_facing = Direction::Right;
			m_helicopter.setScale(1.0f, 1.0f);
		}
		else
		{
			m_facing = Direction::Left;
			m_helicopter.setScale(-1.0, 1.0f);
		}
		
	}

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	animateHelo();
	move();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_welcomeMessage);
	m_window.draw(m_helicopter);
	m_window.display();
}

void Game::animateHelo()
{
	int newFrame = 0;
	m_frameCounter += m_increment;
	newFrame = static_cast<int>(m_frameCounter);
	newFrame = newFrame % 4;
	if (newFrame != m_currentFrame)
	{
		m_currentFrame = newFrame;
		m_helicopter.setTextureRect(sf::IntRect(0, 64 * m_currentFrame, 180, 64));
	}

}

void Game::move()
{
	if (m_facing != Direction::None)
	{
		m_location += m_velocity;
		m_helicopter.setPosition(m_location);
		if (m_facing == Direction::Right && m_location.x > m_target.x)
		{
			m_facing = Direction::None;
			m_increment = 0.28;
		}
		if (m_facing == Direction::Left && m_location.x < m_target.x)
		{
			m_facing = Direction::None;
			m_increment = 0.28;
		}
	}
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("Heli Go Vroom");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_heliTexture.loadFromFile("ASSETS\\IMAGES\\helicopter.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading helicopter" << std::endl;
	}
	m_helicopter.setTexture(m_heliTexture);
	m_helicopter.setTextureRect(sf::IntRect(0, 192, 180, 64));
	m_helicopter.setPosition(300.0f, 180.0f);
	m_helicopter.setOrigin(90.0f, 32.0f);

}
