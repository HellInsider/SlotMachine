#pragma once
#include "Settings.h"

using namespace sf;

class Pic
{
public:
	Sprite sprite;
	Vector2u size;
	int index;

	bool isActive = false;

	Pic(Vector2f coords, Sprite *s, int ind) { 
		index = ind;
		sprite = *s;
		size = sprite.getTexture()->getSize();
		sprite.setPosition(coords); 
	}

	void setSprite(Sprite s)
	{
		sprite = s;
		size = sprite.getTexture()->getSize();
	}

	Sprite* getSprite()
	{
		return &sprite;
	}

	Vector2f getCoords()
	{
		return sprite.getPosition();
	}

	Vector2u getSize()
	{
		return size;
	}

	void setActive(bool b)
	{
		isActive = b;
	}

	void draw(RenderWindow* window)
	{
		if (isActive)
		{
			window->draw(sprite);
		}
	}

};