#include "AssetLoader.h"

void assetLauncher::loadFromParser(ParserSceneInfo parserScene)
{
	_fillMaps.insert(std::make_pair("Texture", &assetLauncher::fillTexture));
	_fillMaps.insert(std::make_pair("Font", &assetLauncher::fillFont));
	_fillMaps.insert(std::make_pair("Audio", &assetLauncher::fillSound));

	arrayInfoParser allAssetBlock = parserScene.getAllBlockFromType("Asset");
	for (auto it = allAssetBlock.begin(); it != allAssetBlock.end(); it++)
	{
		std::string nameBlock = it->first;
		this->call(it->first, it->second);
	}
}

void assetLauncher::draw(sf::RenderWindow & targetWindow)
{
	for (auto it = _infoShape.begin(); it != _infoShape.end(); it++)
		if (it->second.isDisplayed)
			targetWindow.draw(_allShape[it->second.idObj]);
}

void assetLauncher::pushCircleShape(const InfoDraw & info)
{
	_infoShape.emplace(std::pair<std::string, InfoDraw>(info.idObj, info));
	_allShape.emplace(std::pair<std::string, sf::CircleShape>(info.idObj, sf::CircleShape()));
	if (info.idAsset != "")
		_allShape[info.idObj].setTexture(&_allTextureForScene[info.idAsset]); //maybe it will break need to test
}

void assetLauncher::popCircleShape(const std::string & idObj)
{
	_infoShape.erase(idObj);
	_allShape.erase(idObj);
}

void assetLauncher::setCircleShapePos(const std::string & idObj, const sf::Vector2f & nPos)
{
	_allShape[idObj].setPosition(nPos);
}

void assetLauncher::setCircleShapeRect(const std::string &idObj, const sf::Rect<int> &info)
{
	_allShape[idObj].setTextureRect(info);

}

void assetLauncher::setCircleShapeRadius(const std::string &idObj, const float nRadius)
{
	_allShape[idObj].setRadius(nRadius);
}

void assetLauncher::setCircleShapeFillColor(const std::string &idObj, const sf::Color & nColor)
{
	_allShape[idObj].setFillColor(nColor);
}

int assetLauncher::fillTexture(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allTextureForScene.insert(std::pair<std::string, sf::Texture>(it->first, sf::Texture()));
		if (!_allTextureForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}

int assetLauncher::fillFont(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allFontForScene.insert(std::pair<std::string, sf::Font>(it->first, sf::Font()));
		if (!_allFontForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}

int assetLauncher::fillSound(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allSoundForScene.insert(std::pair<std::string, sf::SoundBuffer>(it->first, sf::SoundBuffer()));
		if (!_allSoundForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}