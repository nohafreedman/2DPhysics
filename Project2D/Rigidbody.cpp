#include "Rigidbody.h"

Rigidbody::Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, float orientation, float mass) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::fixedUpdate(vec2 gravity, float timeStep)
{

	if (length(m_velocity) < 0.01f) {
		m_velocity = vec2(0, 0);
	}

	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep);
}

void Rigidbody::applyForce(vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody* actor, vec2 force)
{
	this->m_velocity += -force / this->m_mass;
	actor->m_velocity += force / actor->m_mass;
}

void Rigidbody::resolveCollision(Rigidbody* actor)
{
	vec2 normal = normalize(actor->getPos() - m_position);
	vec2 relativeVelocity = actor->getVelocity() - m_velocity;

	if (dot(relativeVelocity, normal) < 0) {
		float elasticity = 0.8f;
		float j = (dot(-(1 + elasticity) * (relativeVelocity), normal)) / ((1 / m_mass) + (1 / actor->getMass()));
		vec2 force = normal * j;
		this->applyForce(-force);
		actor->applyForce(force);
	}
	else {
		
	}
}
