#pragma once
#include "Settings.h"

class SlotsMachine;

class Button
{
	typedef void (SlotsMachine::* fPtr)();
	fPtr callback;
	SlotsMachine* machine = nullptr;
	bool isActive;
	Sprite sprite;
	sf::Texture tex;

public:
	Button(sf::Vector2i c, SlotsMachine* m, void(SlotsMachine::* func)(), std::filesystem::path path){		
		machine = m;
		callback = func;

		Image img;
		
		img.loadFromFile(path.string());
		img.createMaskFromColor(Color::White);
		tex.loadFromImage(img);
		sprite.setTexture(tex);
		sprite.setPosition(c.x, c.y);  
	}

	void checkOnPress(sf::Vector2f point){
		if (isActive && sprite.getGlobalBounds().contains(point))	{
			(machine->*callback)();
			std::cout << "BtnClicked\n";
		}
	}

	void setActive(bool v) {
		isActive = v;
	}

	void draw(RenderWindow* window)
	{
		if (isActive)
		{
			window->draw(sprite);
		}
	}
};