#pragma once

#include "Parser.h"
#include "SFML/Audio.hpp"
#include "Utils.h"

class AssetLoader;
typedef int (AssetLoader::*mFunc)(std::map<std::string, InfoParser>);
//faut que ça stock les sprites aussi plus aut niveau il est sensé stoké tout les trucs haut comme les sprites 

#define CIRCLE_FLAG 'C'
#define RECT_FLAG 'R'
#define SOUND_FLAG 'S'
#define MUSIC_FLAG 'M'

class AssetLoader
{
public:
	AssetLoader() {}
	~AssetLoader() {}
	void loadFromParser(ParserSceneInfo parserScene);

	void draw(sf::RenderWindow &targetWindow);
	//soon it will be MyShape
	void pushShape(const InfoDraw &info);
	void popShape(const std::string &idObj);
	//need to thing another system to change the Shape
	void setShape(const std::string &idObj, const sf::Vector2f &nPos, const float nRadius, const sf::Color & nColor);
	void setShape(const std::string &idObj, const sf::Vector2f &nPos, const sf::Vector2f nSize, const sf::Color & nColor);
	void setShapePos(const std::string &idObj, const sf::Vector2f &nPos);
	void setShapeSize(const std::string &idObj, const float nRadius);
	void setShapeSize(const std::string &idObj, const sf::Vector2f nSize);
	void setShapeFillColor(const std::string &idObj, const sf::Color &nColor);
	void setTextureRect(const std::string &idObj, const sf::Rect<int> &info); // only for change the Rect of Sprite/Texture FOR NOW*/

	//std::map<std::string, sf::
	int call(const std::string & id, std::map<std::string, InfoParser> infos) {
		mFunc tmp = _fillMaps[id];
		return (this->*tmp)(infos);
	}
private:
	std::map<std::string, mFunc> _fillMaps;
	std::map<std::string, sf::Texture> _allTextureForScene;
	std::map<std::string, sf::Font> _allFontForScene;
	std::map<std::string, sf::SoundBuffer> _allSoundForScene;
	
	std::map<std::string, InfoDraw> _infoShape;
	std::map<std::string, sf::CircleShape> _allCircleShape;
	std::map<std::string, sf::RectangleShape> _allRectangleShape;

	int fillTexture(std::map<std::string, InfoParser> ParserScene);
	int fillFont(std::map<std::string, InfoParser> ParserScene);
	int fillSound(std::map<std::string, InfoParser> ParserScene);

};