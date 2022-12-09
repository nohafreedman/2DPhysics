#pragma once
#include "glm/vec2.hpp"
#include <vector>
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void draw();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }

	glm::vec2 getGravity() const { return m_gravity; }
	float getTimeStep() const { return m_timeStep; }

	static bool plane2Plane(PhysicsObject* plane, PhysicsObject* plane2);
	static bool plane2Sphere(PhysicsObject* plane, PhysicsObject* sphere);
	static bool sphere2Plane(PhysicsObject* sphere, PhysicsObject* plane);
	static bool sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);

	Sphere* mouse2Sphere(vec2 mousePos);

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

