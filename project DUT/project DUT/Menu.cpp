#include "Menu.h"
#include "Button.h"

Menu::Menu() : Scene(SCENE::MENU, "Menu")
{
}

Menu::~Menu()
{
	delete _help;
}

int changeScene(int * idScene, int newIdscene)
{
	(*idScene) = newIdscene;
	return 0;
}

bool Menu::init(sf::RenderWindow & targetWindow, const bool loadAsset)
{
	Scene::init(targetWindow);
	_help = new Button();
	//_help->init(_assetScene, &changeScene, &_idScene, (int)SCENE::TEST);
	return false;
}

void Menu::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	Scene::reset(targetWindow, infoFromPreviousScene);
}

int Menu::update(const float elaspedTime, sf::RenderWindow & targetWindow)
{
	Scene::update(elaspedTime, targetWindow);
	_help->update(elaspedTime, _mouseField);
	return _idScene;
}

void Menu::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.clear();
	_help->draw(targetWindow);
	targetWindow.display();
}
