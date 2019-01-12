#include "Game.h"

Game::Game()
{
	//
}

Game::~Game()
{
	for (auto it : _allScene)
		delete it;
}

void Game::preloadScene()
{
	std::srand(std::time(0));

	
	Menu *scMenu = new Menu();
	Test *scTest = new Test();
	Test2 *scTest2 = new Test2();

	scMenu->init(_wGame);
	scTest->init(_wGame);
	scTest2->init(_wGame);

	_allScene.resize(SCENE::countScene);

	_allScene[SCENE::MENU] = scMenu;
	_allScene[SCENE::TEST] = scTest;
	_allScene[SCENE::TEST2] = scTest2;
}

bool Game::init()
{
	_game = true;

	ParserSceneInfo tmpSetGame;

	tmpSetGame.init("config_game.txt");

	sf::Vector2i tmp = tmpSetGame.getInfo("setting", "size", sf::Vector2i());
	_wGame.create(sf::VideoMode(tmp.x, tmp.y), tmpSetGame.getInfo("setting", "name_window", std::string()));
	//Maybe do a struct t_IdScene consistituée d'un int et d'un string
	_timer = sf::seconds(1.f / tmpSetGame.getInfo("setting", "fps", int()));
	preloadScene();
	_currentIdScene = tmpSetGame.getInfo("setting", "scene_at_start", int());
	return (_game);
}

bool Game::loop()
{
	sf::Event	e;

	_clock.restart();
	while (_game)
	{
		while (_wGame.pollEvent(e))
			event(e);
		if (_clock.getElapsedTime() >= _timer)
		{
			update(_clock.getElapsedTime().asSeconds());
			draw();
			_clock.restart();
		}
		else
			sf::sleep(_timer - _clock.getElapsedTime());
	}
	return (_game);
}

void Game::event(const sf::Event & event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		_game = false;
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape)
			_game = false;
		else if (event.key.code != sf::Keyboard::Unknown)
			_allScene[_currentIdScene]->setKeyboard(event.key.code, true);
		break;
	case sf::Event::KeyReleased:
		if (event.key.code != sf::Keyboard::Unknown)
			_allScene[_currentIdScene]->setKeyboard(event.key.code, false);
		break;
	case sf::Event::MouseButtonReleased:
		_allScene[_currentIdScene]->setMouseField(sf::Mouse::Button(0), sf::Vector2f(sf::Mouse::getPosition(_wGame)));
		break;
	case sf::Event::JoystickButtonPressed:
		_allScene[_currentIdScene]->setController(event.joystickButton.joystickId, event.joystickButton.button, true);
		break;
	case sf::Event::JoystickButtonReleased:
		_allScene[_currentIdScene]->setController(event.joystickButton.joystickId, event.joystickButton.button, false);
		break;
	case sf::Event::JoystickMoved:
		_allScene[_currentIdScene]->setController(event.joystickMove.joystickId, event.joystickMove.axis);
		break;
	default:
		break;
	}

	_allScene[_currentIdScene]->setMousePos(sf::Vector2f(sf::Mouse::getPosition(_wGame)));
}

void Game::update(const float & elaspedTime)
{
	int idNextScene;

	idNextScene = _allScene[_currentIdScene]->update(elaspedTime, _wGame);
	if (idNextScene != _currentIdScene)
	{
		_allScene[idNextScene]->reset(_wGame, _allScene[_currentIdScene]->getInfoToNextScene());
		_currentIdScene = idNextScene;
	}
}

void Game::draw()
{
	_allScene[_currentIdScene]->draw(_wGame);
}