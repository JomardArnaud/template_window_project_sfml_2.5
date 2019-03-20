#pragma once

#include "player.h"

class Player1ModeScene : public Scene
{
public:
	Player1ModeScene();
	~Player1ModeScene();
	virtual bool init(sf::RenderWindow &targetWindow, const bool loadAsset = true) override;
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual int	update(const float elaspedTime, sf::RenderWindow &targetWindow)		override;
	virtual void draw(sf::RenderWindow &targetWindow) override;

private:
	PlayerController _player;
};