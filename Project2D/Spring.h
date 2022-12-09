#pragma once
#include "Rigidbody.h"
class Spring : public PhysicsObject
{

public:

	Spring(Rigidbody* start, Rigidbody* end, float springiness, float dampener, float restLength);

	virtual void fixedUpdate(vec2 gravity, float timeStep);
	virtual void draw();

	Rigidbody* getStart() { return m_start; }
	Rigidbody* getEnd() { return m_end; }

	float getSpringiness() { return m_springiness; }
	float getDampener() { return m_dampener; }

	void setSpringiness(float springiness) { m_springiness = springiness; }
	void setDampener(float dampener) { m_dampener = dampener; }

protected:
	Rigidbody* m_start;
	Rigidbody* m_end;

	float m_springiness;
	float m_dampener;
	float m_restLength;

};

