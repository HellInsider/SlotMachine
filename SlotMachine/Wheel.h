#pragma once

#include "Pic.h"
#include <stdlib.h>
#include <memory>

class Wheel{

private:
	int totalPics = 6;
	Rect<int> rect;
	Vector2f velocity{ 0.0, 0.0 };
	float acceleration = 0.f;

	std::vector<std::shared_ptr<Pic>> pics;
	Vector2i picSize;

public:
	int curPos;				//top pic in rect bounds
	bool isStopped = true;

	Wheel( Rect<int> rectangle, std::vector<std::shared_ptr<Texture>> tex): rect(rectangle) {
		curPos = 1;
		for (int i = 0; i < totalPics; i++)	{
			Sprite s;
			s.setTexture(*tex[i % tex.size()]);		
			
			Vector2f pos;
			pos.x = rect.left;
			pos.y = rect.top + i * s.getTexture()->getSize().y;
			std::shared_ptr<Pic> p = std::make_shared<Pic>(pos, &s, i%tex.size());
			
			pics.push_back(p);
			checkOnActive(p);
		}

		picSize = Vector2i(pics[0]->getSize());
	}

	void update() {
		sf::Vector2f step = velocity;
		velocity.y += acceleration;

		for (auto p : pics) {
			p->sprite.move(step);
			checkOnActive(p);
			if (p->sprite.getPosition().y - rect.top > (totalPics - 1) * picSize.y) {
				sf::Vector2f newPos{float(rect.left), float(rect.top - picSize.y)};
				p->sprite.setPosition(newPos);
			}
		}
	}

	void setAcceleration(float acc) {
		acceleration = acc;
	}

	float getAcceleration() {
		return acceleration;
	}

	Vector2f getVelocity() {
		return velocity;
	}

	void setVelocity(Vector2f v) {
		velocity = v;
	}

	int getPicInd(int ind) {
		return pics[ind%totalPics]->index;
	}

	void  stopWheel() {
		float diff = 99999999.f;
		int ind = 0;
		acceleration = 0.f;
		for (auto p : pics) {
			if (diff > abs(p->getCoords().y - rect.top)) {
				diff = abs(p->getCoords().y - rect.top);
				curPos = ind;
			}

			velocity = { 0,0 };
			ind++;
		}
		fixWheel();
	}

	void draw(RenderWindow *window) {
		for (auto p : pics) {
			p->draw(window);
		}
	}

	void checkOnActive(std::shared_ptr<Pic> p)
	{
		Vector2f curPicCoords = p->getCoords();
		if (curPicCoords.y >= rect.top + rect.height || curPicCoords.y + picSize.y < rect.top)
		{
			p->setActive(false);
		}
		else
		{
			p->setActive(true);
		}
	}
	
	void fixWheel() {
		int i = 0;
		for (auto p : pics)
		{
			Vector2f pos;
			pos.x = rect.left;
			pos.y = (rect.top + (i + curPos)%totalPics * p->getSize().y);
			p->sprite.setPosition(pos);
			i++;			
		}
		
	}

};
