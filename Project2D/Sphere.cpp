#include "Sphere.h"
#include "Gizmos.h"

Sphere::Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 colour) :
	Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{

}

void Sphere::draw()
{

	float xCol = m_position.x;
	float yCol = m_position.y;
	float zCol = glm::length(m_velocity) / 50;

	aie::Gizmos::add2DCircle(m_position, m_radius, 8, vec4(0.1f, .1f, zCol, 1));
}
