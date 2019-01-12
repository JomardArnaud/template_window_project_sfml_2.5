#include "AssetLoader.h"

void assetLauncher::loadFromParser(ParserSceneInfo parserScene, const std::string & nameScene)
{
	std::vector<std::string> tmpIdTexture = parserScene.getAllIdsObjectFromBlock("Texture");
	std::vector<std::string> tmpIdFont = parserScene.getAllIdsObjectFromBlock("Font");
	//std::vector<std::string> tmpIdTexture = parserScene.getAllIdsObjectFromBlock("Sound");

	for (auto it = tmpIdTexture.begin(); it != tmpIdTexture.end(); it++)
	{
		std::string nameTexture = parserScene.getInfo("Texture", (*it), std::string());
		_allTextureForScene.insert(std::pair<std::string, sf::Texture>((*it), sf::Texture()));
		if (!_allTextureForScene[(*it)].loadFromFile(nameTexture))
			std::cout << "failed to load ressource :" + nameTexture << std::endl;
	}
	for (auto it = tmpIdFont.begin(); it != tmpIdFont.end(); it++)
	{
		std::string nameFont = parserScene.getInfo("Font", (*it), std::string());
		_allFontForScene.insert(std::pair<std::string, sf::Font>((*it), sf::Font()));
		if (!_allFontForScene[(*it)].loadFromFile(nameFont))
			std::cout << "failed to load ressource :" + nameFont << std::endl;
	}
	/*for (auto it = tmpIdTexture.begin(); it != tmpIdTexture.end(); it++)
		tmpIdTexture.push_back(parserScene.getInfo("Texture", (*it), std::string()));*/
}
