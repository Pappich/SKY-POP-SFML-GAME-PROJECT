#pragma once
#include<SFML\Graphics.hpp>
#define MAX_ITEMS 2

class Gamewin
{
public:
	Gamewin(float width, float height);
	~Gamewin();

	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem()
	{
		return selectedItem;
	}

private:
	int selectedItem = 0;
	sf::Font font;
	sf::Text gamewin[MAX_ITEMS]; 
};

