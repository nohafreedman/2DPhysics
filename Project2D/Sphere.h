#pragma once
#include "Rigidbody.h"

using namespace glm;

class Sphere : public Rigidbody
{
public:
	Sphere(vec2 position, vec2 velocity, float mass, float radius, vec4 colour);
	~Sphere();

	virtual void draw();

	float getRadius() { return m_radius; }
	vec4 getColour() { return m_colour; }
	void setColour(vec4 colour) { m_colour = colour; }

protected:
	float m_radius;
	vec4 m_colour;
};

