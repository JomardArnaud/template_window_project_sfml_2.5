#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "Game.h"


class DisplayElement
{
public:
	DisplayElement();
	~DisplayElement();
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &targetWindow);
	void init(const sf::Rect<float> &nRect);

protected:
	sf::Vector2f _pos;
	sf::Vector2f _size;
	sf::RectangleShape _display;
};

class Txt : public DisplayElement
{
public:
	Txt() {}
	~Txt() {}
	void init(const sf::Font &fontText, const int letterSize,const std::string &message, const sf::Text::Style &styleText, const sf::Color &colerText, const sf::Rect<float> &pos);
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
protected:
	sf::Text _tx;
};

class Button : public Txt
{
public:
	Button() {}
	~Button() {}
	//T*... et refaire un T... ne peut pâs marcher comment le compilateur fait pour svoir quand s'arrête la première list BOLOSS
	template <typename T> void init(const assetLauncher & assetScene, int(*func)(T*, T), T*  params, T nValueParams)
	{
		Txt::init(assetScene._allFontForScene.at("buttonHello"), 32, "hello World!", sf::Text::Style::Regular, sf::Color::Black, sf::Rect<float>(50, 500, 250, 50));
		_isUsed = 0.f;
		_changeData = std::bind(func, params, nValueParams);
	}
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual void update(const float elapsedTime, const std::map<int, std::vector<t_infoClick>>& mouseField);
	void draw(sf::RenderWindow &targetWindow) override;
	bool getTriggered(const std::map<int, std::vector<t_infoClick> > &mouseField);
	/*template <typename... T> void setOnActivate(int (*func)(T &..., T ...), T &... params, T ... nValueParams)
	{
		_changeData = std::bind(func, params..., nValueParams);
	}*/
private:
	std::function<int()> _changeData;
	float _isUsed;

};
