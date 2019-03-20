#pragma once
class Button;

#define DEAD_ZONE 15.f

//INCLUDE NEEDED BY NUMEROUS SCENE
#include <SFML/Graphics.hpp>
#include "AssetLoader.h"
#include "Button.h"
#include "Utils.h"

enum BUTTON
{
	A,
	B,
	X,
	Y,
	LB,
	RB,
	BACK,
	START,
	L3,
	R3,
	countButton
};


enum AXE
{
	LX,
	LY,
	LT,
	RT,
	RX,
	RY,
	countAxe
};

enum SCENE : int
{
	//TITLE,
	MENU,
	PAYER1MODE,
	//SCORE,
	countScene,
	none = -1
};

class Scene
{
public:
	virtual ~Scene();
	virtual bool init(sf::RenderWindow &targetWindow, const bool loadAsset = true);

	//non virtual method
	void setKeyboard(const int idKey, const bool value); //faut définir la taille event::key::count xD
	void setMouseField(const sf::Mouse::Button &idButton, const sf::Vector2f &pos);
	void setMousePos(const sf::Vector2f &posMouse);
	void updateMouse(const float timeElapsed);
	void setController(const int idController, const int idButton, const bool idValue);
	void setController(const int idController, const int idAxe, const float value);
	void setController(const int idController, const int idAxe);
	const std::map<std::string, std::string> & getInfoToNextScene();
	void setInfoToNextScene(const std::string &key, const std::string &value);

	//method 
	virtual void reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene);
	virtual int update(const float elaspedTime, sf::RenderWindow &targetWindow); //& because i don't sure about change the setting or not of window 
	virtual void draw(sf::RenderWindow &targetWindow);

protected:
	Scene(const int idScene, const std::string &nameScene);

	int _idScene;
	std::string _nameScene;
	int _idNextScene;
	sf::View _viewScene;
	bool _keyField[sf::Keyboard::KeyCount] = { false };
	std::map<int, std::vector<t_infoClick> > _mouseField;
	sf::Vector2f _posMouse;
	std::vector<std::map<int, bool> > _buttonField;
	std::vector<std::map<int, float> > _axeField;
	ParserSceneInfo _parserScene;
	AssetLoader _assetScene;
	std::map<std::string, std::string> _infoToNextScene;
	void setInfoFromPreviousScene(const std::string &stringValue, int *value);
	void setInfoFromPreviousScene(const std::string &stringValue, float *value);
	void setInfoFromPreviousScene(const std::string &stringValue, std::string *value);
};