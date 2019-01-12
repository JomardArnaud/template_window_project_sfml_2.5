#pragma once

#include "Scene.h"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();
	void init();
	void update(const float elapsedTime, std::vector<std::map<int, bool> > &buttonInfo,std::vector<std::map<int, float> > &axeInfo);
	void update(const float elapsedTime, bool *keyField, std::map<int, std::vector<t_infoClick> > mouseField);
	void draw(sf::RenderWindow &targetWindow);
private:
	sf::CircleShape _tmp;
	float _velocity;
	sf::Vector2f _force;
};

