#pragma once

#include "Parser.h"
#include "SFML/Audio.hpp"
#include "Utils.h"

class assetLauncher;
typedef int (assetLauncher::*mFunc)(std::map<std::string, InfoParser>);
//faut que ça stock les sprites aussi plus aut niveau il est sensé stoké tout les trucs haut comme les sprites 
class assetLauncher
{
public:
	assetLauncher() {}
	~assetLauncher() {}
	void loadFromParser(ParserSceneInfo parserScene);

	void draw(sf::RenderWindow &targetWindow);
	//soon it will be MyShape
	void pushCircleShape(const InfoDraw &info);
	void popCircleShape(const std::string &idObj);
	//need to thing another system to change the Shape
	void setCircleShapePos(const std::string &idObj, const sf::Vector2f &nPos);
	void setCircleShapeRect(const std::string &idObj, const sf::Rect<int> &info); // only for change the Rect of Sprite/Texture FOR NOW*/
	void setCircleShapeRadius(const std::string &idObj, const float nRadius);
	void setCircleShapeFillColor(const std::string &idObj, const sf::Color &nColor);

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
	std::map<std::string, sf::CircleShape> _allShape;

	int fillTexture(std::map<std::string, InfoParser> ParserScene);
	int fillFont(std::map<std::string, InfoParser> ParserScene);
	int fillSound(std::map<std::string, InfoParser> ParserScene);

};