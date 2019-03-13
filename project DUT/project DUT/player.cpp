#include "player.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::init(ParserSceneInfo &infoScene, assetLauncher &assetScene)
{
	_circleInfo.radius = infoScene.setInfo<float>("ValueObject", "Player", "size");
	Circle2d::init(IdEntity::PLAYER, "player", { sf::Vector2f(0,0), sf::Vector2f(0,0), 100.f, 0.8f, IdEntity::PLAYER }, { false, false, StatusCollider::OVERLAP });
	Circle2d::setPos(infoScene.setInfo<sf::Vector2f>("ValueObject", "Player", "startingPosition"));
	_velocityLAxis = infoScene.setInfo<float>("ValueObject", "Player", "velocityLAxis");

	_infoShapePlayer.idAsset = "";
	_infoShapePlayer.idObj = "PlayerShape";
	_infoShapePlayer.isDisplayed = true;
	assetScene.pushCircleShape(_infoShapePlayer);
	assetScene.setCircleShapeRadius(_infoShapePlayer.idObj, _circleInfo.radius);
	assetScene.setCircleShapeFillColor(_infoShapePlayer.idObj, sf::Color::Cyan);
}

void PlayerController::update(const float elapsedTime, assetLauncher &assetScene, sf::Vector2f *offsetView, std::vector<std::map<int, bool> > &buttonInfo, std::vector<std::map<int, float> > &axeInfo)
{
	Circle2d::setDir(sf::Vector2f(axeInfo[0][AXE::LX], axeInfo[0][AXE::LY]) * _velocityLAxis);
	if (buttonInfo[0][A])
	{
		Circle2d::addForce(AddedForce(0.125f, MyMath::normalizeVector(sf::Vector2f(axeInfo[0][AXE::LX], axeInfo[0][AXE::LY])) * 550.f));
		buttonInfo[0][A] = false;
	}
	Circle2d::updateForce(elapsedTime);
	Circle2d::updatePos();
	assetScene.setCircleShapePos(_infoShapePlayer.idObj, Circle2d::getPos());
	if (axeInfo[0][AXE::LT] < 0.f)
	{
	}
	if (buttonInfo[0][BUTTON::RB])
	{
		buttonInfo[0][BUTTON::RB] = false;
	}
	if (buttonInfo[0][BUTTON::LB])
	{
		buttonInfo[0][BUTTON::LB] = false;
	}
	if (buttonInfo[0][BUTTON::B])
	{
		buttonInfo[0][BUTTON::B] = false;
	}

	_test.setPosition(_circleInfo.center.x - _circleInfo.radius / 2.f, _circleInfo.center.y - _circleInfo.radius / 2.f);
}


Line PlayerController::getLine() const
{
	return test;
}