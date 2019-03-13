#pragma once

#include "gainput/gainput.h"

//ALL SCENE INCLUDE
#include "Player1Mode.h"
#include "Menu.h"

#define FPS 60 // i will do a file who contains these info later 

class Game
{
public:
  Game();
  ~Game();
  
  bool	init();
  void	preloadScene();
  bool	loop();
  void	event(const sf::Event &event);
  void	update(const float &elaspedTime);
  void	draw();
private:
  bool _game;
  gainput::InputManager _inputManager;
  gainput::InputMap _inputMap;
  gainput::DeviceId _padId;
  sf::RenderWindow _wGame;
  sf::Clock _clock;
  sf::Time _timer;
  int _currentIdScene;
  std::vector<Scene *>	_allScene;
};