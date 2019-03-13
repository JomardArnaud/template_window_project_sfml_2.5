#include "Game.h"

Game::Game() : _inputMap(_inputManager)
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
	Player1ModeScene *scPlayer1Mode = new Player1ModeScene();

	scMenu->init(_wGame);
	scPlayer1Mode->init(_wGame);

	_allScene.resize(SCENE::countScene);

	_allScene[SCENE::MENU] = scMenu;
	_allScene[SCENE::PAYER1MODE] = scPlayer1Mode;
}

bool Game::init()
{
	_game = true;

	ParserSceneInfo tmpSetGame;
	tmpSetGame.init("config_game.txt");

	sf::Vector2i tmp;
	tmp = tmpSetGame.setInfo<sf::Vector2i>("windowInfo", "setting", "size");
	_wGame.create(sf::VideoMode(tmp.x, tmp.y), tmpSetGame.setInfo<std::string>("windowInfo", "setting", "name_window"));
	_timer = sf::seconds(1.f / tmpSetGame.setInfo<int>("gameInfo", "infoScene", "fps"));
	preloadScene();
	_currentIdScene = tmpSetGame.setInfo<int>("gameInfo", "infoScene", "scene_at_start");
	_padId = _inputManager.CreateDevice<gainput::InputDevicePad>();
	_inputMap.MapFloat(0, _padId, gainput::PadButtonAxis4);
	_inputMap.MapFloat(1, _padId, gainput::PadButtonAxis5);
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
	_inputManager.Update();
	_inputMap.GetFloatDelta(0);

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