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
	_display.setFillColor(sf::Color::Cyan);
}

void DisplayElement::draw(sf::RenderWindow & targetWindow)
{
	targetWindow.draw(_display);
}


void Txt::init(const sf::Font &fontText, int letterSize, const std::string & message, const sf::Text::Style & styleText, const sf::Color & colerText, const sf::Rect<float> &rectText)
{
	std::string tmpText;
	float currentLine = 0;

	DisplayElement::init(rectText);
	_tx.setFont(fontText);
	_tx.setCharacterSize(letterSize);
	_tx.setLetterSpacing(1); // must equal 1
	for (size_t i = 0; i < message.length(); i++)
	{
		currentLine += fontText.getGlyph(message[i], letterSize, true).advance;
		if (currentLine >= _size.x)
		{
			currentLine = fontText.getGlyph(message[i], letterSize, true).advance;
			tmpText.push_back('\n');
		}
		if (tmpText[i] == '\n')
			currentLine = 0;
		tmpText.push_back(message[i]);
	}
	_tx.setString(tmpText);
	_tx.setStyle(styleText);
	_tx.setFillColor(colerText);
	_tx.setPosition(_pos.x, _pos.y);
}

void Txt::reset(sf::RenderWindow & targetWindow, const std::map<std::string, std::string>& infoFromPreviousScene)
{

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
		std::cout << "i'm triggered\n";
		_changeData();
		_isUsed = TIME_BUFFER_CLICK;
	}
	_isUsed -= elapsedTime;
	if (_isUsed < 0.f)
		_isUsed = 0.F;
}

void Button::draw(sf::RenderWindow &targetWindow)
{
	DisplayElement::draw(targetWindow);
	targetWindow.draw(_tx);
}
