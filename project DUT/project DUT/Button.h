#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "AssetLoader.h"

#define TIME_BUFFER_CLICK 0.50F

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

struct InfoButton
{
	InfoButton() {
		id = "";
		sizeLetter = 32;
		message = "";
		style = sf::Text::Style::Regular;
		color = sf::Color::Black;
		rectText = sf::Rect<float>();
	};
	InfoButton(const std::string &nId, const int nSizeLetter, const std::string &nMessage, const sf::Text::Style nStyle, const sf::Color &nColor, const sf::Rect<float> nRectText) {
		id = nId;
		sizeLetter = nSizeLetter;
		message = nMessage;
		style = nStyle;
		color = nColor;
		rectText = nRectText;
	};
	~InfoButton() {};
	std::string id;
	int sizeLetter;
	std::string message;
	sf::Text::Style style;
	sf::Color color;
	sf::Rect<float> rectText;
};

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
	void init(const InfoButton &info, const sf::Font &fontText);
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	void draw(sf::RenderWindow &targetWindow);
	void setMessage(const std::string &nMessage);
protected:
	sf::Text _tx;
};

class Button : public Txt
{
public:
	Button() {}
	~Button() {}
	//min width 1 for sizeLetter 1
	template <typename T> void init(const AssetLoader & assetScene, const InfoButton &info, int(*func)(T*, T), T*  params, T nValueParams)
	{
//		Txt::init(info, assetScene._allFontForScene.at(info.id));
		_isUsed = 0.f;
		_changeData = std::bind(func, params, nValueParams);
	}
	virtual void reset(sf::RenderWindow &targetWindow, const std::map<std::string, std::string> &infoFromPreviousScene) override;
	virtual void update(const float elapsedTime, const std::map<int, std::vector<t_infoClick>>& mouseField);
	void draw(sf::RenderWindow &targetWindow);
	bool getTriggered(const std::map<int, std::vector<t_infoClick> > &mouseField);
	/*template <typename... T> void setOnActivate(int (*func)(T &..., T ...), T &... params, T ... nValueParams)
	{
		_changeData = std::bind(func, params..., nValueParams);
	}*/
private:
	std::function<int()> _changeData;
	float _isUsed;
};