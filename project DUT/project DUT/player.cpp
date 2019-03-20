#include "player.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::init(ParserSceneInfo &infoScene, AssetLoader &assetScene)
{
	_circleInfo.radius = infoScene.setInfo<float>("ValueObject", "Player", "size");
	Circle2d::init(IdEntity::PLAYER, "player", { sf::Vector2f(0,0), sf::Vector2f(0,0), 100.f, 0.8f, IdEntity::PLAYER }, { false, false, StatusCollider::OVERLAP });
	Circle2d::setPos(infoScene.setInfo<sf::Vector2f>("ValueObject", "Player", "startingPosition"));
	_velocityLAxis = infoScene.setInfo<float>("ValueObject", "Player", "velocityLAxis");

	_tmp.resize(100);
	_infoShapePlayer.idAsset = "";
	_infoShapePlayer.idObj = "CPlayerShape";
	_infoShapePlayer.isUsed = true;
	assetScene.pushShape(_infoShapePlayer);
	assetScene.setShapeSize(_infoShapePlayer.idObj, _circleInfo.radius);
	assetScene.setShapeFillColor(_infoShapePlayer.idObj, sf::Color::Cyan);
}

void PlayerController::update(const float elapsedTime, AssetLoader &assetScene, sf::Vector2f *offsetView, std::vector<std::map<int, bool> > &buttonInfo, std::vector<std::map<int, float> > &axeInfo)
{
	static sf::Color tmpTestColor = sf::Color::Cyan;
	static int n;

	Circle2d::setDir(sf::Vector2f(axeInfo[0][AXE::LX], axeInfo[0][AXE::LY]) * _velocityLAxis);
	if (buttonInfo[0][A])
	{
		Circle2d::addForce(AddedForce(0.125f, MyMath::normalizeVector(sf::Vector2f(axeInfo[0][AXE::LX], axeInfo[0][AXE::LY])) * 550.f));
		buttonInfo[0][A] = false;
	}
	Circle2d::updateForce(elapsedTime);
	Circle2d::updatePos();
	assetScene.setShapePos(_infoShapePlayer.idObj, Circle2d::getPos());
	if (axeInfo[0][AXE::LT] > 0.f)
	{
		assetScene.setShapeFillColor(_infoShapePlayer.idObj, sf::Color::Magenta);
		tmpTestColor = sf::Color::Magenta;
	}
	else
	{
		assetScene.setShapeFillColor(_infoShapePlayer.idObj, sf::Color::Cyan);
		tmpTestColor = sf::Color::Cyan;
	}
	if (buttonInfo[0][BUTTON::B])
	{
		_tmp[n].idObj = CIRCLE_FLAG + std::to_string(n);
		_tmp[n].isUsed = true;
		assetScene.pushShape(_tmp[n]);
		assetScene.setShape(CIRCLE_FLAG + std::to_string(n++), Circle2d::getPos(), 25.f, tmpTestColor);
		buttonInfo[0][BUTTON::B] = false;
	}
}


Line PlayerController::getLine() const
{
	return test;
}