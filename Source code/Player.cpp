#include "Player.h"
#include <SFML\Graphics.hpp>
#include <stdio.h>

void Player::initVariables()
{
	movementSpeed = 6.f;
	preWidth = 115.f;
	preHeight = 102.f;
	width = 86.f;
	height = 155.f;
	moving = false;
	standNum = 1;
	moveNum = 1;
	turnLeft = false;
}

void Player::updateAnimation()
{
	if (standNum >= 5)
	{
		standNum = 1;
	}
	if (moveNum >= 7)
	{
		moveNum = 1;
	}
	if (aniTime.getElapsedTime().asSeconds() >= 0.1f)
	{
		if (moving == false)
		{
			moveNum = 1;
			preHeight = 102.f;
			width = 86.f;
			switch (standNum)
			{
			case 1: preWidth = 271.f;
				break;
			case 2: preWidth = 421.f;
				break;
			case 3: preWidth = 573.f;
				break;
			case 4: preWidth = 115.f;
				break;
			default:
				break;
			}
			standNum++;
		}
		else if (moving == true)
		{
			standNum = 1;
			preHeight = 332.f;
			width = 91.f;
			switch (moveNum)
			{
			case 1: preWidth = 113.f;
				break;
			case 2: preWidth = 272.f;
				break;
			case 3: preWidth = 424.f;
				break;
			case 4: preWidth = 576.f;
				break;
			case 5: preWidth = 735.f;
				break;
			case 6: preWidth = 891.f;
				break;
			default:
				break;
			}
			moveNum++;
		}
		if (turnLeft == false)
		{
			currentFrame = IntRect(preWidth, preHeight, width, height);
		}
		else if (turnLeft == true)
		{
			currentFrame = IntRect(preWidth + width, preHeight, -width, height);
		}
		sprite.setTextureRect(currentFrame);
		aniTime.restart();
	}
}
