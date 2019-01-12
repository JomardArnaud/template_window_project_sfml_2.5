#include "Scene.h"
Scene::~Scene()
{
	//
}

bool Scene::init(sf::RenderWindow &targetWindow)
{
	_idNextScene = _idScene;
	_parserScene.init(_nameScene + ".txt");
	_assetScene.loadFromParser(_parserScene, _nameScene);
	_buttonField.resize(BUTTON::countButton);
	_axeField.resize(AXE::countAxe);
	for (unsigned int i = 0; i < sf::Joystick::Count; i++)
		if (sf::Joystick::isConnected(i))
		{
			_buttonField.push_back(std::map<int, bool>());
			_axeField.push_back(std::map<int, float>());
		}
	return true;
}

void Scene::reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene)
{
	_idNextScene = _idScene;
}

int Scene::update(const float elaspedTime, sf::RenderWindow &targetWindow)
{
	updateMouse(elaspedTime);
	return _idScene;
}

void Scene::draw(sf::RenderWindow &targetWindow)
{
	//
}

Scene::Scene(const int idScene, const std::string &nameScene) : _idScene(idScene), _nameScene(nameScene)
{
	//
}

void Scene::setInfoFromPreviousScene(const std::string &value, int * var)
{
	(*var) = std::atoi(value.c_str());
}

void Scene::setInfoFromPreviousScene(const std::string &value, float * var)
{
	(*var) = std::atof(value.c_str());
}

void Scene::setInfoFromPreviousScene(const std::string &value, std::string * var)
{
	(*var) = value;
}

const std::map<std::string, std::string> & Scene::getInfoToNextScene()
{
	return _infoToNextScene;
}

void Scene::setInfoToNextScene(const std::string &key, const std::string &value)
{
	if (_infoToNextScene.find(key) == _infoToNextScene.end())
		_infoToNextScene.emplace(std::pair<std::string, std::string>(key, value));
	else
		_infoToNextScene[key] = value;
}

void Scene::setKeyboard(const int idKey, const bool value)
{
	_keyField[idKey] = value;
}

void Scene::setController(const int idController, const int idButton, const bool value)
{
	_buttonField[idController][idButton] = value;
}

void Scene::setController(const int idController, const int idAxe)
{
	const float tmp(sf::Joystick::getAxisPosition(idController, sf::Joystick::Axis(idAxe)));
	(abs(tmp) > DEAD_ZONE) ? _axeField[idController][idAxe] = tmp : _axeField[idController][idAxe] = 0.f;
}

void Scene::setMouseField(const sf::Mouse::Button &idButton, const sf::Vector2f &pos)
{
	_mouseField[idButton].push_back(t_infoClick(pos, 0.f));
	std::cout << idButton << "\n";
}

void Scene::setMousePos(const sf::Vector2f &posMouse)
{
	_posMouse = posMouse;
}

void Scene::updateMouse(const float timeElapsed)
{
	int test = 0;
	for (int i = 0; i != sf::Mouse::Button::ButtonCount; i++)
	{
		std::vector<t_infoClick>::iterator it = _mouseField[i].begin();
		while (it != _mouseField[i].end())
		{
			it->timing += timeElapsed;
			if (it->timing >= TIME_BUFFER_CLICK)
			{
				std::cout << "erased\n";
				it = _mouseField[i].erase(it);
			}
			else
				it++;
		}
	}
}

namespace vector
{
	const float magnitude(const sf::Vector2f &v)
	{
		return sqrt((v.x * v.x) + (v.y * v.y));
	}

	bool normalizeVector(sf::Vector2f & vector)
	{
		float m = magnitude(vector);
		if (!m)
			return false;
		vector.x /= m;
		vector.y /= m;
		return true;
	}
}
