#pragma once
#include <filesystem>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Wheel.h"
#include "Button.h"
#include "StateMachine.h"


class SlotsMachine {
	const sf::Vector2i WHEELOFFSET{ 50, 70 }, BUTTONSPOINT{ 650, 90 }, BUTTONOFFSET{ 0, 90 };
	std::vector<std::shared_ptr<Wheel>> wheels;
	std::map<std::string, std::shared_ptr<Button>> buttons;
	std::shared_ptr<StateMachine> stateMachine;
	std::vector<std::shared_ptr<Texture>> wheelPics;
	
	RenderWindow* window;
	
	std::filesystem::path picsPath{ "resourses\\WheelPics" };
	std::filesystem::path bStartPath{ "resourses\\Buttons\\Start.png" };
	std::filesystem::path bStopPath{ "resourses\\Buttons\\Stop.png" };

	int wheelsCount = 0;
	float baseWheelAcc = 0.02f;
	int fitPicsCount = 3;

	Clock rotationTimer;
	Time maxRotationTime = seconds(4);
	float minWheelSpeed = 1;
	
public:
	SlotsMachine(RenderWindow *w, int wheelsCnt): wheelsCount(wheelsCnt) {
		
		loadPics();
		Vector2i tPicSize = (Vector2i)wheelPics[0]->getSize();

		for (int i = 0; i < wheelsCount; i++) {
			Rect<int> wheelRect = { WHEELOFFSET.x * (i + 1) + tPicSize.x * i,
						WHEELOFFSET.y,
						tPicSize.x,
						fitPicsCount * tPicSize.y };
			wheels.push_back(std::make_shared<Wheel>(wheelRect, wheelPics));
		}

		buttons["Start"] = (std::make_shared<Button>(BUTTONSPOINT, this, & SlotsMachine::btnStartPressed, bStartPath));
		buttons["Stop"] = (std::make_shared<Button>(BUTTONSPOINT + BUTTONOFFSET, this, &SlotsMachine::btnStopPressed, bStopPath));

		stateMachine = std::make_shared<StateMachine>(this);
		stateMachine->addState(&SlotsMachine::waitUserAction);
		stateMachine->addState(&SlotsMachine::waitBoostRotation);
		stateMachine->addState(&SlotsMachine::stopWheels);
		stateMachine->addState(&SlotsMachine::showResult);

		window = w;
	}

	~SlotsMachine() {
		buttons.clear();
	}

	void makeFrame() {
		stateMachine->makeAction();
		update();
		render();
	}

	void update() {
		for (auto w : wheels) {
			w->update();
		}
	}

	void render() {
		for (auto w : wheels) {
			w->draw(window);
		}

		for (auto& [k, v] : buttons) {
			v->draw(window);
		}
		
	}

	void clickHandle() {
		static int mouseDelay = 0;
		mouseDelay++;
		mouseDelay = mouseDelay % 60;

		if (Mouse::isButtonPressed(Mouse::Left) && mouseDelay % (FPS / 10) == 0){
			//std::cout << "Click " << Mouse::getPosition(*window).x << " " << Mouse::getPosition(*window).y << "\n";
			Vector2i point = Mouse::getPosition(*window);

			for (auto & [key, value] : buttons)	{
				value->checkOnPress(Vector2f(point));
			}
		}
	}

	void waitUserAction() {
		clickHandle();
	}

	void waitBoostRotation() {
		clickHandle();
		if (rotationTimer.getElapsedTime() >= maxRotationTime) {
			rotationTimer.restart();
			stateMachine->nextState();
		}
	}

	void stopWheels() {
		for (auto w : wheels) {
			w->stopWheel();
		}
		stateMachine->nextState();
	}

	void showResult() {

		int res = calcRes();

		if (res < 0) {
			std::cout << "You lost! " << res << "$!" << std::endl;
		}
		else {
			std::cout << "Congratulations! You won " << (res+1) * 100 << "$!" << std::endl;
		}
		
		buttons["Start"]->setActive(true);
		buttons["Stop"]->setActive(false);
		stateMachine->nextState();
	}

	void btnStartPressed() {
		buttons["Start"]->setActive(false);
		buttons["Stop"]->setActive(true);
		rotationTimer.restart();
		setAcceleration(baseWheelAcc);
		stateMachine->nextState();
	}

	void btnStopPressed() {
		buttons["Stop"]->setActive(false);
		stateMachine->nextState();
	}

	void setAcceleration(float acc) {
		int i = 1;
		for (auto w : wheels) {
			w->setAcceleration((rand()%5 + 3)*acc);
			i++;
		}
	}

	void loadPics()
	{
		sf::Image img;
		std::shared_ptr<sf::Texture> tex;
	
		for (const auto& fn : std::filesystem::directory_iterator{ picsPath })
		{
			tex = std::make_shared<Texture>();
			img.loadFromFile(fn.path().string());
			img.createMaskFromColor(img.getPixel(0, 0));
			tex->loadFromImage(img);
			wheelPics.push_back(tex);
		}
	}

	int calcRes() {
		std::vector<std::vector<int>> matrix;
		int i = 0;

		for (auto w : wheels) {
			std::vector<int> t;
			for (int j = 0; j < fitPicsCount; j++) {
				t.push_back(w->getPicInd(w->curPos + j));
			}
			matrix.push_back(t);
			i++;
		}

		/*for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++)
				std::cout << matrix[i][j] << " ";
			std::cout << std::endl;
		}*/

		bool isAccord=false;
		for (int p = 0; p < fitPicsCount && !isAccord; p++) {
			isAccord = true;
			i = p;
			for (int w = 0; w < wheels.size() - 1; w++) {
				if (matrix[w][p] != matrix[w + 1][p]) {
					isAccord = false;
					break;
				}
			}
		}

		if (isAccord) {
				std::cout << wheels.size() <<" in a row!\n";
				return matrix[i][0];
		}
		return -10;
	}
};
