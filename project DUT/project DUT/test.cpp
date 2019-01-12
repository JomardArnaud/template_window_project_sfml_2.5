#include "test.h"

Test::Test() : Scene(SCENE::TEST, "Test")
{
}

Test::~Test()
{
}

bool Test::init(sf::RenderWindow & targetWindow)
{
	x = 0;
	t = 0;
	Scene::init(targetWindow);
	_player.init();
	return true;
}

void Test::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	Scene::reset(targetWindow, infoFromPreviousScene);
}

int Test::update(const float elaspedTime, sf::RenderWindow & targetWindow)
{
	_player.update(elaspedTime, _keyField, _mouseField);
	if (_keyField[A])
	{
		t++;
		setInfoToNextScene("t", std::to_string(t));
		_keyField[A] = false;
		_idNextScene = 1;

	}
	return _idNextScene;
}

void Test::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.clear();
	_player.draw(targetWindow);
	targetWindow.display();
}

Test2::Test2() : Scene(1, "Test2")
{
}

Test2::~Test2()
{
}

bool Test2::init(sf::RenderWindow & targetWindow)
{
	Scene::init(targetWindow);
	x = 0;
	t = 0;
	_player.init();
	return true;
}

void Test2::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	Scene::reset(targetWindow, infoFromPreviousScene);
	setInfoFromPreviousScene(infoFromPreviousScene.at("t"), &t);
}

int Test2::update(const float elaspedTime, sf::RenderWindow & targetWindow)
{
	_player.update(elaspedTime, _buttonField, _axeField);
	if (_keyField[A])
	{
		t++;
		setInfoToNextScene("t", std::to_string(t));
		_keyField[A] = false;
		_idNextScene = 0;

	}
	return _idNextScene;
}

void Test2::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.clear();
	_player.draw(targetWindow);
	targetWindow.display();
}