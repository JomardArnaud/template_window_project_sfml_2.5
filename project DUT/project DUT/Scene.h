#pragma once
class Button;

#define DEAD_ZONE 15.f
#define TIME_BUFFER_CLICK 0.50F

//INCLUDE NEEDED BY NUMEROUS SCENE
#include <SFML/Graphics.hpp>
#include "AssetLoader.h"

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
	T, //NB: For SFML the two trigger use the same idAxe ( = 2) if value is neg is RT pos is for LT
	RX = 4,
	RY,
	countAxe
};

enum SCENE : int
{
	//TITLE,
	MENU,
	TEST,
	TEST2,
	//SCORE,
	countScene,
	none = -1
};

typedef struct s_infoClick
{
	s_infoClick() {
		click = sf::Vector2f(0, 0);
		timing = 0.f;
	};
	s_infoClick(const sf::Vector2f &posClick, const float nTimingClick) {
		click = posClick;
		timing = nTimingClick;
	};
	~s_infoClick() {};
	sf::Vector2f click;
	float timing;
} t_infoClick;

class Scene
{
public:
	virtual ~Scene();
	virtual bool init(sf::RenderWindow &targetWindow);

	//non virtual method
	void setKeyboard(const int idKey, const bool value); //faut définir la taille event::key::count xD
	void setMouseField(const sf::Mouse::Button &idButton, const sf::Vector2f &pos);
	void setMousePos(const sf::Vector2f &posMouse);
	void updateMouse(const float timeElapsed);
	void setController(const int idController, const int idButton, const bool idValue);
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
	bool _keyField[sf::Keyboard::KeyCount] = { false };
	std::map<int, std::vector<t_infoClick> > _mouseField;
	sf::Vector2f _posMouse;
	std::vector<std::map<int, bool> > _buttonField;
	std::vector<std::map<int, float> > _axeField;
	ParserSceneInfo _parserScene;
	assetLauncher _assetScene;
	std::map<std::string, std::string> _infoToNextScene;
	void setInfoFromPreviousScene(const std::string &stringValue, int *value);
	void setInfoFromPreviousScene(const std::string &stringValue, float *value);
	void setInfoFromPreviousScene(const std::string &stringValue, std::string *value);
};

namespace vector
{
	const float magnitude(const sf::Vector2f &tmpVector);
	bool normalizeVector(sf::Vector2f &tmpVector);
};