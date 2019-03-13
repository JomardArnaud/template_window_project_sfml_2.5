#include "Button.h"

DisplayElement::DisplayElement()
{
	//
}

DisplayElement::~DisplayElement()
{
	//
}

void DisplayElement::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	//
}

void DisplayElement::update(const float elapsedTime)
{
	//
}

void DisplayElement::init(const sf::Rect<float>& nRect)
{
	_pos = sf::Vector2f(nRect.left, nRect.top);
	_size = sf::Vector2f(nRect.width, nRect.height);
	_display.setPosition(_pos);
	_display.setSize(_size);
	_display.setFillColor(sf::Color(0, 26, 225, 255));
}

void DisplayElement::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.draw(_display);
}


void Txt::init(const InfoButton &info, const sf::Font &fontText)
{
	std::string tmpText;
	float currentLine = 0;

	DisplayElement::init(info.rectText);
	_tx.setFont(fontText);
	_tx.setCharacterSize(info.sizeLetter);
	_tx.setLetterSpacing(1); // must equal 1
	for (size_t i = 0; i < info.message.length(); i++)
	{
		currentLine += fontText.getGlyph(info.message[i], info.sizeLetter, true).advance;
		if (currentLine >= _size.x)
		{
			currentLine = fontText.getGlyph(info.message[i], info.sizeLetter, true).advance;
			tmpText.push_back('\n');
		}
		if (tmpText[i] == '\n')
			currentLine = 0;
		tmpText.push_back(info.message[i]);
	}
	_tx.setString(tmpText);
	_tx.setStyle(info.style);
	_tx.setFillColor(info.color);
	_tx.setPosition(_pos.x, _pos.y);
}

void Txt::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{

}

void Txt::draw(sf::RenderWindow & targetWindow)
{
	DisplayElement::draw(targetWindow);
	targetWindow.draw(_tx);
}

void Txt::setMessage(const std::string &nMessage)
{
	_tx.setString(nMessage);
}

void Button::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{
	_isUsed = false;
}

bool Button::getTriggered(const std::map<int, std::vector<t_infoClick>>& mouseField)
{
	for (auto it : mouseField.at(sf::Mouse::Button::Left))
		if (sf::Rect<float>(_pos, _size).contains(it.click))
			return true;
	return false;
}

void Button::update(const float elapsedTime, const std::map<int, std::vector<t_infoClick>>& mouseField)
{
	if (getTriggered(mouseField) && !_isUsed)
	{
		_changeData();
		_isUsed = TIME_BUFFER_CLICK;
	}
	_isUsed -= elapsedTime;
	if (_isUsed < 0.f)
		_isUsed = 0.F;
}

void Button::draw(sf::RenderWindow &targetWindow)
{
	Txt::draw(targetWindow);
}
