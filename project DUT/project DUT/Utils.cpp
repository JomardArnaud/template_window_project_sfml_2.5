#include "Utils.h"

namespace MyMath
{
	const float norm(const float value, const float min, const float max)
	{
		return (value - min) / (max - min); //l'inverse de lerp
	}

	const float lerp(const float norm, const float min, const float max)
	{
		return (norm * max) + ((1 - norm) * min);
	}

	const float clamp(const float value, const float min, const float max)
	{
		return std::min(std::max(value, min), max);
	}

	const float magnitude(const sf::Vector2f &v)
	{
		return sqrt((v.x * v.x) + (v.y * v.y));
	}

	const float dotProduct(const sf::Vector2f & v1, const sf::Vector2f & v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	const sf::Vector2f normalizeVector(const sf::Vector2f & vector)
	{
		float m = magnitude(vector);
		if (!m)
			return (sf::Vector2f(0, 0));
		return sf::Vector2f(vector.x / m, vector.y / m);
	}

	const sf::Vector2f middleRectangle(const sf::FloatRect &c0)
	{
		return sf::Vector2f(c0.left + (c0.width / 2), c0.top + (c0.height / 2));
	}

	const float distance(const sf::Vector2f &p0, const sf::Vector2f &p1)
	{
		const float dx(p1.x - p0.x);
		const float dy(p1.y - p0.y);

		return sqrt(dx * dx + dy * dy);
	}
	
	const sf::Vector2f centerCircle(const sf::CircleShape &c)
	{
		const sf::Vector2f tmpPos(c.getPosition());
		const float tmpRadius(c.getRadius());
		return sf::Vector2f(tmpPos.x + tmpRadius / 2, tmpPos.y + tmpRadius / 2);
	}

	//COLLIDER PART

	const bool pointToCircle(const sf::Vector2f &p, const sf::Vector2f &center, const float radius) {
		return	distance(p, center) < radius;
	}

	bool circleToCircle(const sf::Vector2f & centerC1, const float radiusC1, const sf::Vector2f & centerC2, const float radiusC2)
	{
		return distance(centerC1, centerC2) <= (radiusC1 + radiusC2);
	}

	bool segmentToCircle(const sf::Vector2f & c, const float raiduC, const Segment &ab)
	{
		sf::Vector2f AB(ab.B - ab.A);
		sf::Vector2f AC(c - ab.A);
		
		float mAB = magnitude(AB);
		float mAD = dotProduct(AB, AC) / mAB;
		if (mAD > mAB && !pointToCircle(ab.B, c, raiduC))
			return false;
		sf::Vector2f AD = normalizeVector(AB) * mAD;
		sf::Vector2f D = ab.A + AD;
		float mCD = magnitude(D - c);
		return (mCD < raiduC);
	}
}

void Line::setPos(const sf::Vector2f & coorLeftPoint, const sf::Vector2f & coorRightPoint)
{
	_points[0].position = coorLeftPoint;
	_points[1].position = coorRightPoint;
}

void Line::setDisplay(const bool isDisplay)
{
	_isDisplay = isDisplay;
}

void Line::setColor(const sf::Color & nColor)
{
	_points[0].color = nColor;
	_points[1].color = nColor;
}

Segment Line::getSegment() const
{
	return Segment(sf::Vector2f(_points[0].position.x, _points[0].position.y), sf::Vector2f(_points[1].position.x, _points[1].position.y));
}

bool Line::getIsDisplay() const
{
	return _isDisplay;
}

void Line::draw(sf::RenderWindow & targetWindow)
{
	if (_isDisplay)
	{
		targetWindow.draw(_points, 2, sf::Lines);
	}
}

void Circle2d::init(const IdEntity nId, const std::string nStringId, const InfoPhysics & infoClass, const StatusPhysics & status)
{
	_id = nId;
	_idString = nStringId;
	_valueVector = infoClass;
	_statusChange = status;
}

void Circle2d::setVeloctiy(const float nVelocity)
{
	_valueVector.velocity = nVelocity;
}

void Circle2d::setDir(const sf::Vector2f & nDir)
{
	_valueVector.dir = nDir;
}

void Circle2d::setCheckCollsion(const StatusCollider nCheckCollision)
{
	_statusChange.checkCollision = nCheckCollision;
}

void Circle2d::addForce(const AddedForce & newForce)
{
	_addedForce.push_back(newForce);
}

void Circle2d::updateForce(const float elapsedTime)
{
	sf::Vector2f tmpForce;

	auto it = _addedForce.begin();
	while (it != _addedForce.end())
	{
		it->_force = sf::Vector2f(it->_force.x - it->_slowDown, it->_force.y - it->_slowDown);
		tmpForce += it->_force * elapsedTime;
		it->_duration -= elapsedTime;
		if (it->_duration <= 0.f)
			it = _addedForce.erase(it);
		else
			it++;
	}
	_valueVector.force +=  _valueVector.velocity * _valueVector.dir * elapsedTime;
	_valueVector.force += tmpForce;
}

void Circle2d::updatePos()
{
	_circleInfo.center += _valueVector.force;
	_valueVector.force *= _valueVector.inertia;
}

void Circle2d::setPos(const sf::Vector2f & nPos)
{
	_circleInfo.center = sf::Vector2f(nPos.x - _circleInfo.radius, nPos.y - _circleInfo.radius);
}

void Circle2d::collide(const Circle2d & otherObject)
{
	sf::Vector2f tmpPos = _circleInfo.center + _valueVector.force;

	if (_statusChange.checkCollision == StatusCollider::COLLIDE)
	{
		//
	}
	else if (_statusChange.checkCollision == StatusCollider::OVERLAP)
	{
		if (MyMath::circleToCircle(otherObject._circleInfo.center, otherObject._circleInfo.radius, tmpPos, _circleInfo.radius))
			_valueVector.overlap = otherObject._id;
	}
}

void Circle2d::collide(const Segment &ab, IdEntity otherObjectId)
{
	sf::Vector2f tmpPos = _circleInfo.center + _valueVector.force;

	if (_statusChange.checkCollision == StatusCollider::OVERLAP)
	{
		if (MyMath::segmentToCircle(tmpPos, _circleInfo.radius, ab))
			_valueVector.overlap = otherObjectId;
	}
}

void Circle2d::collide(const std::vector<Circle2d> &circles)
{
	for (auto it = circles.cbegin(); it != circles.cend(); it++)
	{
		Circle2d::collide((*it));
	}
}

float Circle2d::getVelocity() const
{
	return _valueVector.velocity;
}

sf::Vector2f Circle2d::getDir() const
{
	return _valueVector.dir;
}

sf::Vector2f Circle2d::getForce() const
{
	return _valueVector.force;
}

InfoPhysics Circle2d::getValueVector() const
{
	return _valueVector;
}

StatusPhysics Circle2d::getStatus() const
{
	return _statusChange;
}

sf::Vector2f Circle2d::getPos() const
{
	return _circleInfo.center;
}

std::vector<AddedForce> Circle2d::getAddedForce() const
{
	return _addedForce;
}

const InfoCircle Circle2d::getShapeInfo() const
{
	return _circleInfo;
}

const bool Circle2d::pointToCircle(const sf::Vector2f & p)
{
	return MyMath::pointToCircle(p, _circleInfo.center, _circleInfo.radius);
}

const bool Circle2d::circleToCircle(const Circle2d & otherCircle)
{
	return MyMath::circleToCircle(_circleInfo.center, _circleInfo.radius, otherCircle._circleInfo.center, otherCircle._circleInfo.radius);
}
