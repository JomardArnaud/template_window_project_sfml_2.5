#include "AssetLoader.h"

void AssetLoader::loadFromParser(ParserSceneInfo parserScene)
{
	_fillMaps.insert(std::make_pair("Texture", &AssetLoader::fillTexture));
	_fillMaps.insert(std::make_pair("Font", &AssetLoader::fillFont));
	_fillMaps.insert(std::make_pair("Audio", &AssetLoader::fillSound));

	arrayInfoParser allAssetBlock = parserScene.getAllBlockFromType("Asset");
	for (auto it = allAssetBlock.begin(); it != allAssetBlock.end(); it++)
	{
		std::string nameBlock = it->first;
		this->call(it->first, it->second);
	}
}

void AssetLoader::draw(sf::RenderWindow & targetWindow)
{
	for (auto it = _infoShape.begin(); it != _infoShape.end(); it++)
		if (it->second.isUsed)
			if (it->second.idObj[0] == CIRCLE_FLAG)
				targetWindow.draw(_allCircleShape[it->second.idObj]);
			else if (it->second.idObj[0] == RECT_FLAG)
				targetWindow.draw(_allRectangleShape[it->second.idObj]);
}

void AssetLoader::pushShape(const InfoDraw &info)
{
	_infoShape.emplace(info.idObj, info);
	if (info.idObj[0] == CIRCLE_FLAG)
	{
		_allCircleShape.emplace(info.idObj, sf::CircleShape());
		if (info.idAsset != "")
			_allCircleShape[info.idObj].setTexture(&_allTextureForScene[info.idAsset]); //maybe it will break need to test
	}
	else if (info.idObj[0] == RECT_FLAG)
	{
		_allRectangleShape.emplace(info.idObj, sf::RectangleShape());
		if (info.idAsset != "")
			_allRectangleShape[info.idObj].setTexture(&_allTextureForScene[info.idAsset]); //maybe it will break need to test

	}
}

void AssetLoader::popShape(const std::string & idObj)
{
	_infoShape.erase(idObj);
	if (idObj[0] == CIRCLE_FLAG)
		_allCircleShape.erase(idObj);
	if (idObj[0] == RECT_FLAG)
		_allRectangleShape.erase(idObj);
}

void AssetLoader::setShape(const std::string & idObj, const sf::Vector2f & nPos, const float nRadius, const sf::Color & nColor)
{
	_allCircleShape[idObj].setPosition(nPos);
	_allCircleShape[idObj].setRadius(nRadius);
	_allCircleShape[idObj].setFillColor(nColor);
}

void AssetLoader::setShape(const std::string & idObj, const sf::Vector2f & nPos, const sf::Vector2f nSize, const sf::Color & nColor)
{
	_allRectangleShape[idObj].setPosition(nPos);
	_allRectangleShape[idObj].setSize(nSize);
	_allRectangleShape[idObj].setFillColor(nColor);
}

void AssetLoader::setShapePos(const std::string & idObj, const sf::Vector2f & nPos)
{
	if (idObj[0] == CIRCLE_FLAG)
		_allCircleShape[idObj].setPosition(nPos);
	if (idObj[0] == RECT_FLAG)
		_allRectangleShape[idObj].setPosition(nPos);
}

void AssetLoader::setShapeSize(const std::string & idObj, const float nRadius)
{
	_allCircleShape[idObj].setRadius(nRadius);
}

void AssetLoader::setShapeSize(const std::string & idObj, const sf::Vector2f nSize)
{
	_allRectangleShape[idObj].setSize(nSize);
}

void AssetLoader::setShapeFillColor(const std::string & idObj, const sf::Color & nColor)
{
	if (idObj[0] == CIRCLE_FLAG)
		_allCircleShape[idObj].setFillColor(nColor);
	if (idObj[0] == RECT_FLAG)
		_allRectangleShape[idObj].setFillColor(nColor);
}

void AssetLoader::setTextureRect(const std::string & idObj, const sf::Rect<int>& info)
{
	if (idObj[0] == CIRCLE_FLAG)
		_allCircleShape[idObj].setTextureRect(info);
	if (idObj[0] == RECT_FLAG)
		_allRectangleShape[idObj].setTextureRect(info);
}

int AssetLoader::fillTexture(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allTextureForScene.insert(std::pair<std::string, sf::Texture>(it->first, sf::Texture()));
		if (!_allTextureForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}

int AssetLoader::fillFont(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allFontForScene.insert(std::pair<std::string, sf::Font>(it->first, sf::Font()));
		if (!_allFontForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}

int AssetLoader::fillSound(std::map<std::string, InfoParser> tmpblock)
{
	for (auto it = tmpblock.begin(); it != tmpblock.end(); it++)
	{
		_allSoundForScene.insert(std::pair<std::string, sf::SoundBuffer>(it->first, sf::SoundBuffer()));
		if (!_allSoundForScene[it->first].loadFromFile(it->second.value))
			std::cout << "failed to load ressource :" + it->second.value << std::endl;
	}
	return 1;
}