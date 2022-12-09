#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

using namespace glm;

class Plane : public PhysicsObject
{
public:
	Plane(vec2 normal, float distance);
	~Plane();

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void draw();
	virtual void resetPosition();

	vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }
	vec4 getColour() { return m_colour; }
	vec4 setColour(vec4 colour) { m_colour = colour; }

	void resolveCollision(Rigidbody* actor);

protected:
	vec2 m_normal;
	float m_distanceToOrigin;
	vec4 m_colour;

};

