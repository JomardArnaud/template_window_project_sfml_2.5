#pragma once

#include "SFML/Graphics.hpp"

typedef struct	s_infoWeapon {
	sf::Vector2f pos;

}		t_infoWeapon;

class Weapon
{
public: 
	Weapon();
	~Weapon();
	void init();
	void update(const float elapsedTime, const t_infoWeapon &tmpInfo);
	void draw(sf::RenderWindow &targetWindow);
	
private:
	sf::CircleShape	cursor;
};
