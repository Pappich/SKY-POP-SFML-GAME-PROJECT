#include "Gamewin.h"
Gamewin::Gamewin(float width, float height)
{
	if (!font.loadFromFile("puimek.ttf"))
	{
		//haddle error
	}

	gamewin[0].setFont(font);
	gamewin[0].setFillColor(sf::Color(128, 64, 96));
	gamewin[0].setString("SAVE SCORE");
	gamewin[0].setCharacterSize(75);
	gamewin[0].setOrigin(gamewin[0].getLocalBounds().width / 2, gamewin[0].getLocalBounds().height / 2);
	gamewin[0].setPosition(sf::Vector2f(width / 1.40, height / (MAX_ITEMS + 1) * 1.05));
	
	gamewin[1].setFont(font);
	gamewin[1].setFillColor(sf::Color::White);
	gamewin[1].setCharacterSize(90);
	gamewin[1].setString("QUIT");
	gamewin[1].setOrigin(gamewin[1].getLocalBounds().width / 2, gamewin[1].getLocalBounds().height / 2);
	gamewin[1].setPosition(sf::Vector2f(width / 1.40, height / (MAX_ITEMS + 1) * 1.80));
}

Gamewin::~Gamewin()
{

}

void Gamewin::draw(sf::RenderWindow& window)
{
	for (int i = 0;i < MAX_ITEMS;i++)
	{
		window.draw(gamewin[i]);
	}
}

void Gamewin::MoveUp()
{
	if (selectedItem - 1 >= 0)
	{
		gamewin[selectedItem].setFillColor(sf::Color::White);
		selectedItem--;
		gamewin[selectedItem].setFillColor(sf::Color(128, 64, 96));
	}
}

void Gamewin::MoveDown()
{
	if (selectedItem + 1 < MAX_ITEMS)
	{
		gamewin[selectedItem].setFillColor(sf::Color::White);
		selectedItem++;
		gamewin[selectedItem].setFillColor(sf::Color(128, 64, 96));
	}
}
