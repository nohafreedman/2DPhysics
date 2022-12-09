#include "PhysicsScene.h"
#include <iostream>

PhysicsScene::PhysicsScene()
{
	m_gravity = glm::vec2(0, 0);
	m_timeStep = 0.01f;
}

PhysicsScene::~PhysicsScene()
{
	for (auto actor : m_actors) {
		delete actor;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	
	for (int i = 0; i < m_actors.size(); i++) {
		if (m_actors[i] == actor) {
			m_actors.erase(m_actors.begin() + i);
		}
	}
	
}

typedef bool(*function)(PhysicsObject*, PhysicsObject*);

static function collisionFunctions[] = {
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere
};


void PhysicsScene::update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep) {
		for (auto actor : m_actors) {
			actor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
	}

	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* obj1 = m_actors[outer];
			PhysicsObject* obj2 = m_actors[inner];
			int shid1 = obj1->getShapeID();
			int shid2 = obj2->getShapeID();

			if (shid1 < 0 || shid2 < 0)
				continue;

			int functionID = (shid1 * SHAPE_COUNT) + shid2;
			function collisionFunctionsPtr = collisionFunctions[functionID];
			if (collisionFunctionsPtr != nullptr) {
				collisionFunctionsPtr(obj1, obj2);
			}

		}
	}

}

void PhysicsScene::draw()
{
	for (auto actor : m_actors) {
		if(actor != nullptr)
			actor->draw();
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject* plane, PhysicsObject* plane2)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* plane, PhysicsObject* sphere)
{
	return sphere2Plane(sphere, plane);
}

bool PhysicsScene::sphere2Plane(PhysicsObject* sphere, PhysicsObject* plane)
{
	Sphere* realSphere = dynamic_cast<Sphere*>(sphere);
	Plane* realPlane = dynamic_cast<Plane*>(plane);

	if (realSphere != nullptr && realPlane != nullptr) {
		float sphereToPlane = dot(realSphere->getPos(), realPlane->getNormal()) - realPlane->getDistance();
		float intersection = realSphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = dot(realSphere->getVelocity(), realPlane->getNormal());

		if (intersection > 0 && velocityOutOfPlane < 0) {
			realPlane->resolveCollision(realSphere);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	if (sphere1 != nullptr && sphere2 != nullptr) {

		vec2 difference = sphere1->getPos() - sphere2->getPos();

		if (length(difference) < sphere1->getRadius() + sphere2->getRadius()) {
			sphere1->resolveCollision(sphere2);
			return true;
		}
		else {
			return false;
		}

	}
	else {
		return false;
	}

}

Sphere* PhysicsScene::mouse2Sphere(vec2 mousePos)
{
	for (auto actor : m_actors) {
		Sphere* sphereCheck = dynamic_cast<Sphere*>(actor);
		if (sphereCheck != nullptr) {
			// check if collided with area around mouse
			float mouseRadius = 1;
			vec2 difference = sphereCheck->getPos() - mousePos;
			if (length(difference) < sphereCheck->getRadius() + mouseRadius) {
				// we collided!
				return sphereCheck;
			}
		}
	}
	return nullptr;
}
