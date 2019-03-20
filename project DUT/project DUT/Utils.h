#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <iterator>
#include <SFML/Graphics.hpp>

class Circle2d;

struct Segment
{
	Segment() {
		A = sf::Vector2f(0, 0);
		B = sf::Vector2f(0, 0);
	}
	Segment(const sf::Vector2f &p1, const sf::Vector2f &p2)
	{
		A = p1;
		B = p2;
	}
	~Segment() {}
	sf::Vector2f A;
	sf::Vector2f B;
};

struct InfoCircle
{
	sf::Vector2f center;
	float radius;
};

struct InfoDraw
{
	InfoDraw() {
		idObj = "";
		idAsset = "";
		isUsed = false;
	}
	InfoDraw(const std::string &nIdObj, const std::string &nIdAsset, const bool nIsUsed) {
		idObj = nIdObj;
		idAsset = nIdAsset;
		isUsed = nIsUsed;
	}
	std::string idObj; //first letter is a flag For exemple R/C RPlayer for a rectangleShape and CPlayer for a circ... same for sound/music
	// Rect/Cirlce | Sound/Music
	std::string idAsset;
	bool isUsed;
};

namespace MyMath
{
	const float norm(const float value, const float min, const float max);
	const float lerp(const float norm, const float min, const float max);
	const float clamp(const float value, const float min, const float max);
	const float magnitude(const sf::Vector2f &v);
	const float dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);
	const sf::Vector2f normalizeVector(const sf::Vector2f & vector);
	const sf::Vector2f middleRectangle(const sf::FloatRect &c0);
	const float distance(const sf::Vector2f &p0, const sf::Vector2f &p1);
	const sf::Vector2f centerCircle(const sf::CircleShape &c);

	//COLLIDER PART
	const bool pointToCircle(const sf::Vector2f &p, const sf::Vector2f &center, const float radius);
	bool circleToCircle(const sf::Vector2f &centerC1, const float radiusC1, const sf::Vector2f &centerC2, const float radiusC2);
	bool segmentToCircle(const sf::Vector2f & centerC, const float raidusC, const Segment &ab);
}

class Line
{
public:
	Line() { _points[1].color = sf::Color::Cyan; };
	Line(const sf::Vector2f &coorLeftPoint, const sf::Vector2f &coorRightPoint) {
		_points[0].position = coorLeftPoint;
		_points[1].position = coorRightPoint;
	};
	~Line() {};
	void setPos(const sf::Vector2f &coorLeftPoint, const sf::Vector2f &coorRightPoint);
	void setDisplay(const bool isDisplay);
	void setColor(const sf::Color &nColor);
	//getter collider func for later
	Segment getSegment() const;
	bool getIsDisplay() const;
	void draw(sf::RenderWindow &targetWindow);

private:
	sf::Vertex _points[2];
	bool _isDisplay;
};

enum IdEntity
{
	PLAYER,
	ENNEMIES,
	BULLETS,
	COLLECTABLE,
	NONE
};

enum StatusCollider
{
	COLLIDE,
	OVERLAP,
	NO_CHECK
};

struct InfoPhysics
{
	InfoPhysics() {}
	~InfoPhysics() {}
	InfoPhysics(const sf::Vector2f &nForce,	const sf::Vector2f &nDir, const float nVelocity, const float nIinertia, const IdEntity nOverlap) {
		force = nForce;
		dir = nDir;
		velocity = nVelocity;
		inertia = nIinertia;
		overlap = nOverlap;
	}
	sf::Vector2f force;
	sf::Vector2f dir;
	float velocity;
	float inertia;
	IdEntity overlap;
};

struct StatusPhysics
{
	StatusPhysics() {}
	~StatusPhysics() {}
	StatusPhysics(const bool nIsPosLocked, const bool nIsVelocityLocked, const StatusCollider nCheckCollision) {
		isPosLocked = nIsPosLocked;
		isVelocityLocked = nIsVelocityLocked;
		checkCollision = nCheckCollision;
	}
	bool isPosLocked;
	bool isVelocityLocked;
	StatusCollider checkCollision;
};

struct AddedForce
{
	AddedForce() {

	}
	AddedForce(const float duration, const sf::Vector2f &force, const float slowDown = 0.f) {
		_duration = duration;
		_force = force;
		_slowDown = slowDown;
	}
	~AddedForce() {}
	float _duration;
	sf::Vector2f _force;
	float _slowDown;
};

class Circle2d
{
public:
	Circle2d() {};
	~Circle2d() {};
	void init(const IdEntity nId, const std::string nStringId, const InfoPhysics & infoClass, const StatusPhysics & status);
	void setVeloctiy(const float nVelocity);
	void setDir(const sf::Vector2f &nDir);
	void setCheckCollsion(const StatusCollider nCheckCollision);
	void addForce(const AddedForce &newForce);
	void updateForce(const float elapsedTime);
	void updatePos();
	void setPos(const sf::Vector2f &nPos);
	void collide(const Circle2d &otherObject);
	void collide(const Segment &otherObject, IdEntity otherObjectId);
	void collide(const std::vector<Circle2d> &circles);
	//getters
	float getVelocity() const;
	sf::Vector2f getDir() const;
	sf::Vector2f getForce() const;
	InfoPhysics getValueVector() const;
	StatusPhysics getStatus() const;
	sf::Vector2f getPos() const;
	std::vector<AddedForce> getAddedForce() const;
	const InfoCircle getShapeInfo() const;
	//collider
	template <typename T>
	void collideT(const std::vector<T> &circles)
	{
		for (auto it = circles.cbegin(); it != circles.cend(); it++)
		{
			Circle2d::collide((*it));
		}
	}
	const bool pointToCircle(const sf::Vector2f &p);
	const bool circleToCircle(const Circle2d & otherCircle);

protected:
	IdEntity _id;
	std::string _idString;
	InfoPhysics _valueVector;
	StatusPhysics _statusChange;
	std::vector<AddedForce> _addedForce;
	InfoCircle _circleInfo;
};

#endif