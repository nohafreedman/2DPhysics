#pragma once
#include "glm/glm.hpp"

#define SHAPE_COUNT 2

enum ShapeType {
	SPRING = -1,
	PLANE = 0,
	SPHERE,
	BOX,
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}
	ShapeType m_shapeID;
public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};

	int getShapeID() { return m_shapeID; }

};

