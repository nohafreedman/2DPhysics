#include "Spring.h"
#include <Gizmos.h>

Spring::Spring(Rigidbody* start, Rigidbody* end, float springiness, float dampener, float restLength) : PhysicsObject(SPRING)
{
	m_start = start;
	m_end = end;
	m_springiness = springiness;
	m_dampener = dampener;
	m_restLength = restLength;
}

void Spring::fixedUpdate(vec2 gravity, float timeStep)
{

	vec2 startPos = m_start->getPos();
	vec2 endPos = m_end->getPos();
	vec2 relativeVelocity = m_end->getVelocity() - m_start->getVelocity();

	vec2 direction = normalize(endPos - startPos);
	vec2 force = -direction * m_springiness * (m_restLength - length(endPos - startPos)) - (m_dampener * relativeVelocity);

	m_start->applyForce(force * timeStep);
	m_end->applyForce(-force * timeStep);

}

void Spring::draw()
{
	aie::Gizmos::add2DLine(m_start->getPos(), m_end->getPos(), vec4(1, 1, 1, 1));
}
