#include "Gameover.h"
Gameover::Gameover(float width, float height)
{
	if (!font.loadFromFile("puimek.ttf"))
	{
		//haddle error
	}
	gameover[0].setFont(font);
	gameover[0].setFillColor(sf::Color(128, 64, 96));
	gameover[0].setString("SAVE SCORE");
	gameover[0].setCharacterSize(90);
	gameover[0].setOrigin(gameover[0].getLocalBounds().width / 2, gameover[0].getLocalBounds().height / 2);
	gameover[0].setPosition(sf::Vector2f(width / 1.40, height / (MAX_ITEMS + 1) * 1.05));

	gameover[1].setFont(font);
	gameover[1].setFillColor(sf::Color::White);
	gameover[1].setCharacterSize(90);
	gameover[1].setString("QUIT");
	gameover[1].setOrigin(gameover[1].getLocalBounds().width / 2, gameover[1].getLocalBounds().height / 2);
	gameover[1].setPosition(sf::Vector2f(width / 1.40, height / (MAX_ITEMS + 1) * 1.80));
}

Gameover::~Gameover()
{

}

void Gameover::draw(sf::RenderWindow& window)
{
	for (int i = 0;i < MAX_ITEMS;i++)
	{
		window.draw(gameover[i]);
	}
}

void Gameover::MoveUp()
{
	if (selectedItem - 1 >= 0)
	{
		gameover[selectedItem].setFillColor(sf::Color::White);
		selectedItem--;
		gameover[selectedItem].setFillColor(sf::Color(128, 64, 96));
	}
}

void Gameover::MoveDown()
{
	if (selectedItem + 1 < MAX_ITEMS)
	{
		gameover[selectedItem].setFillColor(sf::Color::White);
		selectedItem++;
		gameover[selectedItem].setFillColor(sf::Color(128, 64, 96));
	}
}