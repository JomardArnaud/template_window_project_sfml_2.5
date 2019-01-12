#pragma once

#include "Parser.h"

class assetLauncher //currently is only for Texture, Font but it willcertainly for audio too
{
public:
	assetLauncher() {}
	~assetLauncher() {}
	void loadFromParser(ParserSceneInfo parserScene, const std::string & nameScene);
	// 1 file for 1 scene
	std::map<std::string, sf::Texture> _allTextureForScene;
	std::map<std::string, sf::Font> _allFontForScene;
};