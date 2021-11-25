#pragma once
#include<SFML\Graphics.hpp>
#define MAX_ITEMS 2

class Gameover
{
public:
	Gameover(float width, float height);
	~Gameover();

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
	sf::Text gameover[MAX_ITEMS]; //start top exit
};

