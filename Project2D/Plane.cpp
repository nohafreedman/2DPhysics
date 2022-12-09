#include "Plane.h"
#include <Gizmos.h>

Plane::Plane(vec2 normal, float distance) : PhysicsObject(PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = vec4(1, 0, 0, 1);
}

Plane::~Plane()
{

}

void Plane::fixedUpdate(vec2 gravity, float timeStep)
{

}

void Plane::draw()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distanceToOrigin;
	vec2 parallel(m_normal.y, -m_normal.x);
	vec4 colourFade = m_colour;
	colourFade.a = 0;
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_colour, colourFade, colourFade);
}

void Plane::resetPosition()
{
	m_distanceToOrigin = 0;
}

void Plane::resolveCollision(Rigidbody* actor)
{
	vec2 normal = m_normal;
	vec2 relativeVelocity = actor->getVelocity();

	// j = (j * (MA * MB) / (MA + MB))

	float elasticity = 0.8f;
	float j = (dot(-(1 + elasticity) * (relativeVelocity), normal)) / (1 / actor->getMass());
	vec2 force = normal * j;
	actor->applyForce(force);
}
