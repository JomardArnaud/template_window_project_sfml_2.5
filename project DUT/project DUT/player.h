#pragma once

#include "Scene.h"
#include "Utils.h"

class PlayerController;

class PlayerController : public Circle2d
{
public:
	PlayerController();
	~PlayerController();
	void init(ParserSceneInfo &infoScene, assetLauncher &assetScene);
	void update(const float elapsedTime, assetLauncher &assetScene, sf::Vector2f *offsetView, std::vector<std::map<int, bool> > &buttonInfo, std::vector<std::map<int, float> > &axeInfo);
	Line getLine() const;
private:
	InfoDraw _infoShapePlayer;
	sf::CircleShape _test;
	Line test;
	float _velocityLAxis;
};

