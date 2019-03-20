#include "Player1Mode.h"

Player1ModeScene::Player1ModeScene() : Scene(SCENE::PAYER1MODE, "Player1Mode")
{
}

Player1ModeScene::~Player1ModeScene()
{
}

bool Player1ModeScene::init(sf::RenderWindow & targetWindow, const bool loadAsset)
{
	Scene::init(targetWindow);
	_player.init(_parserScene, _assetScene);
	return true;
}

void Player1ModeScene::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	Scene::reset(targetWindow, infoFromPreviousScene);
}

int Player1ModeScene::update(const float elaspedTime, sf::RenderWindow & targetWindow)
{
	sf::Vector2f offsetView;

	Scene::update(elaspedTime, targetWindow);
	offsetView = _viewScene.getCenter();
	_player.update(elaspedTime, _assetScene, &offsetView, _buttonField, _axeField);

	_viewScene.setCenter(_player.getPos());
	//targetWindow.setView(_viewScene);
	return _idNextScene;
}

void Player1ModeScene::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.clear();
	_assetScene.draw(targetWindow);
	targetWindow.display();
}