#pragma once
#include "PhysicsObject.h"

using namespace glm;

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	void applyForce(vec2 force);
	void applyForceToActor(Rigidbody* actor, vec2 force);

	vec2 getPos() { return m_position; }
	vec2 getVelocity() { return m_velocity; }
	float getOrientation() { return m_orientation; }
	float getMass() { return m_mass; }

	void resolveCollision(Rigidbody* actor);

protected:
	vec2 m_position;
	vec2 m_velocity;
	float m_mass;
	float m_orientation;
};

