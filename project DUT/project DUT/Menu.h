#pragma once

#include "Scene.h"

class Button;

class Menu : public Scene
{
public:
	Menu();
	~Menu();
	virtual bool init(sf::RenderWindow &targetWindow) override;
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual int	update(const float elaspedTime, sf::RenderWindow &targetWindow)		override;
	virtual void draw(sf::RenderWindow &targetWindow) override;

private:
	Button *_help;
};