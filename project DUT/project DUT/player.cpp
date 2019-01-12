#include "player.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::init()
{
	_tmp.setFillColor(sf::Color::Cyan);
	_tmp.setRadius(18.f);
	_velocity = 80.f;
}

void PlayerController::update(const float elapsedTime, std::vector<std::map<int, bool> > &buttonInfo, std::vector<std::map<int, float> > &axeInfo)
{
	//_force += sf::Vector2f(axeInfo[0][AXE::LX], axeInfo[0][AXE::LY]) * _velocity * elapsedTime;
	_tmp.move(_force);
	_force *= 0.8f;
}

void PlayerController::update(const float elapsedTime, bool * keyField, std::map<int, std::vector<t_infoClick>> mouseField)
{
	_force = sf::Vector2f(keyField[sf::Keyboard::Q] * -10.f + keyField[sf::Keyboard::D] * 10.f,
		keyField[sf::Keyboard::Z] * -10.f + keyField[sf::Keyboard::S] * 10.f);
	_force *= _velocity * elapsedTime;
	_tmp.move(_force);
	_force *= 0.8f;

}

void PlayerController::draw(sf::RenderWindow &targetWindow)
{
	targetWindow.draw(_tmp);
}
