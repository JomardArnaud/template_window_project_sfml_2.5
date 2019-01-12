#pragma once

#include "player.h"

class Test : public Scene
{
public:
	Test();
	~Test();
	virtual bool init(sf::RenderWindow &targetWindow) override;
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual int	update(const float elaspedTime, sf::RenderWindow &targetWindow)		override;
	virtual void draw(sf::RenderWindow &targetWindow) override;

private:
	int x;
	PlayerController _player;
	int t;
	//std::vector<int> listControll later  
};

class Test2 : public Scene
{
public:
	Test2();
	~Test2();	
	virtual bool init(sf::RenderWindow &targetWindow) override;
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual int	update(const float elaspedTime, sf::RenderWindow &targetWindow) override;
	virtual void draw(sf::RenderWindow &targetWindow) override;

private:
	int x;
	PlayerController _player;
	int t;
	//std::vector<int> listControll later  
};