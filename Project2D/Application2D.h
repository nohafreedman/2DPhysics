#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <thread>

enum ToolType {
	BALLCREATE,
	BALLSPRING,
	BALLDELETE,
	BALLGRID,
	SPRINGCREATE,
	PLANECREATE,
};

class Application2D : public aie::Application {
public:

	const float screenSize = 100;
	const float aspectRatio = 16.0f / 9.f;

	void CUMZONE() {
		m_physicsScene->draw();
	}

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	/// <summary>
	/// switches between screen and world pos
	/// </summary>
	/// <param name="screenPos">SCREEN POSITION i.e. somethin inbetween ur window size like 1280x720</param>
	/// <returns>WORLD POSITION i.e. somethin in the ortho like -100 to 100</returns>
	glm::vec2 screenToWorld(glm::vec2 screenPos);

	// j = (j * (MA * MB) / (MA + MB))

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	float circleSize = 1.0f;
	bool mouseHeldLastFrame = false;
	bool rightMouseHeldLastFrame = false;
	vec2 firstMousePosition;
	int currentColour;
	bool cancelAction = false;

	float totalMouseScroll = 0;
	float mouseScrollLastFrame = 0;
	float mouseScrollDelta = 0;

	float k = 20;

	Sphere* clickedSphere;

	// ballcreate stuff

	float c_mass = 1;
	float c_vMult = 1;
	vec2 ballVelocity;
	bool tutorial = true;

	// springcreate stuff

	float c_springiness = 20;
	float c_dampener = 1;

	ToolType currentTool = BALLCREATE;

	float scrollLastFrame = 0;
	int gridSize = 1;
	int gridWidth = 5;
	int gridHeight = 5;

	void ballCreate();
	void ballSpring();
	void ballDelete();
	void ballGrid();
	void springCreate();
	void planeCreate();

	void printTutorialText();

	float m_timer;
	PhysicsScene* m_physicsScene;
};