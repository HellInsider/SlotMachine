#pragma once
#include "Settings.h"
#include "SlotsMachine.h"

class Game
{
private:
	RenderWindow window;
	SlotsMachine* machine;


public:
	Game()
	{
		window.create(VideoMode(WINSIZEX, WINSIZEY), "Slots Machine");
		window.setFramerateLimit(60);
		machine = new SlotsMachine(&window, 3);
	}

	~Game() {
		delete machine;
	}

	void run()
	{
		Image img;
		Texture tex;
		img.loadFromFile("resourses\\background.png");
		tex.loadFromImage(img);
		Sprite background(tex);

		window.display();

		while (window.isOpen())
		{
			Event event;
			window.clear();
			window.draw(background);
			machine->makeFrame();
			while (window.pollEvent(event))
			{

				if (event.type == Event::Closed)
				{
					window.close();
				}

			}

			window.display();
		}
	}
};