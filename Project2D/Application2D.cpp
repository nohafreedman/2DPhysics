#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "glm/ext.hpp"
#include "Sphere.h"
#include "Plane.h"
#include <vector>
#include <iostream>
#include "Spring.h"

Application2D::Application2D() {
	
}

Application2D::~Application2D() {

}

bool Application2D::startup() {

	//increase 2d line count
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 16);
	m_timer = 0;
	m_physicsScene = new PhysicsScene();

	m_physicsScene->setTimeStep(0.01f);
	m_physicsScene->setGravity(glm::vec2(0, -10));

	Sphere* ball;
	ball = new Sphere(vec2(-40, 0), vec2(10, 0), 3.0f, 5, vec4(1, 0, 0, 1));
	Sphere* ball2;
	ball2 = new Sphere(vec2(40, 0), vec2(-20, 0), 3.0f, 1, vec4(0, 1, 0, 1));

	Spring* spring1;
	spring1 = new Spring(ball, ball2, 20, 0.1f, 20);

	Plane* plane1;
	plane1 = new Plane(vec2(0, 1), -50);

	//Plane* plane = new Plane(vec2(0.707f, 0.707f), -49);
	//Plane* plane2 = new Plane(vec2(-0.707f, 0.707f), -49);

	//m_physicsScene->addActor(plane);
	//m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(ball);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(spring1);
	m_physicsScene->addActor(plane1);

	firstMousePosition.x = 0;
	firstMousePosition.y = 0;
	currentColour = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//std::thread deez(&Application2D::CUMZONE, this);

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();

	totalMouseScroll = input->getMouseScroll();
	mouseScrollDelta = totalMouseScroll - mouseScrollLastFrame;

	// Update the camera position using the arrow keys
	float camPosX;
	float camPosY;
	m_2dRenderer->getCameraPos(camPosX, camPosY);

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		camPosY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		camPosY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		camPosX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->setCameraPos(camPosX, camPosY);

	vec2 mouseIn(0, 0);
	mouseIn.x = input->getMouseX();
	mouseIn.y = input->getMouseY();

	vec2 output = screenToWorld(mouseIn);

	aie::Gizmos::add2DCircle(output, circleSize, 8, vec4(0, 0, 1, 0));

	// key codes for 1 and 2, found out they've all been defined in aie later

	if (input->wasKeyPressed(49)) {
		currentTool = BALLCREATE;
		tutorial = true;
	}

	if (input->wasKeyPressed(50)) {
		currentTool = BALLSPRING;
		tutorial = true;
	}

	if (input->wasKeyPressed(aie::INPUT_KEY_3)) {
		currentTool = BALLGRID;
		tutorial = true;
	}

	if (input->wasKeyPressed(aie::INPUT_KEY_4)) {
		currentTool = PLANECREATE;
		tutorial = true;
	}

	if (input->wasKeyPressed(aie::INPUT_KEY_F))
		tutorial = false;

	switch (currentTool) {
	default:

		break;
	case BALLCREATE:
		ballCreate();
		break;
	case BALLSPRING:
		ballSpring();
		break;
	case BALLGRID:
		ballGrid();
		break;
	case PLANECREATE:
		planeCreate();
		break;
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	mouseScrollLastFrame = totalMouseScroll;

}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	m_2dRenderer->drawText(m_font, "Tutorial (F to Hide)", 0, 512);

	if (tutorial) {
		printTutorialText();
	}

	switch (currentTool) {
	default:

		break;
	case BALLCREATE:
		m_2dRenderer->drawText(m_font, "current tool: ", 0, 720 - 128);
		m_2dRenderer->drawText(m_font, "ball creation", 250/2, 720 - 128);
		char mass[32];
		sprintf_s(mass, 32, "ball mass: %f", c_mass);
		m_2dRenderer->drawText(m_font, mass, 0, 592 - 16);
		char vMult[32];
		sprintf_s(vMult, 32, "velocity multiplier: %f", c_vMult);
		m_2dRenderer->drawText(m_font, vMult, 0, 592 - (16 * 2));
		char velocityX[32];
		/*sprintf_s(velocityX, 32, "velocity: %f", ballVelocity.x);
		m_2dRenderer->drawText(m_font, velocityX, 0, 592 - (16 * 3));
		char velocityY[32];
		sprintf_s(velocityY, 32, "%f", ballVelocity.y);
		m_2dRenderer->drawText(m_font, velocityY, 250, 592 - (16 * 3));*/
		break;
	case BALLSPRING:
		m_2dRenderer->drawText(m_font, "current tool: ", 0, 720 - 128);
		m_2dRenderer->drawText(m_font, "ball spring", 250 / 2, 720 - 128);
		char kValue[32];
		sprintf_s(kValue, 32, "spring coefficient: %f", k);
		m_2dRenderer->drawText(m_font, kValue, 0, 592 - (16 * 2));
		char cSpringiness[32];
		sprintf_s(cSpringiness, 32, "springiness: %f", c_springiness);
		m_2dRenderer->drawText(m_font, cSpringiness, 0, 592 - (16 * 3));
		char cDampener[32];
		sprintf_s(cDampener, 32, "spring damper: %f", c_dampener);
		m_2dRenderer->drawText(m_font, cDampener, 0, 592 - (16 * 4));
		break;
	case BALLGRID:
		m_2dRenderer->drawText(m_font, "current tool: ", 0, 720 - 128);
		m_2dRenderer->drawText(m_font, "ball grid", 250 / 2, 720 - 128);
		break;
	case PLANECREATE:
		m_2dRenderer->drawText(m_font, "current tool: ", 0, 720 - 128);
		m_2dRenderer->drawText(m_font, "plane creator", 250 / 2, 720 - 128);
		break;
	}

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 Application2D::screenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	worldPos.x *= 2.0f * screenSize / getWindowWidth();
	worldPos.y *= 2.0f * screenSize / (aspectRatio * getWindowHeight());

	return worldPos;
}

void Application2D::ballCreate()
{

	aie::Input* input = aie::Input::getInstance();

	int extraMultiplier = 1;

	if (input->isKeyDown(aie::INPUT_KEY_F))
		extraMultiplier = 10;
	else
		extraMultiplier = 1;
	
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
		c_mass += mouseScrollDelta * extraMultiplier;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) {
		c_vMult += mouseScrollDelta * extraMultiplier;
	}
	else {
		circleSize += mouseScrollDelta * extraMultiplier;
	}

	if (input->isKeyDown(aie::INPUT_KEY_R)) {
		c_mass = 1;
		c_vMult = 1;
		circleSize = 1;
	}

	vec2 mouseInput(0, 0);
	mouseInput.x = input->getMouseX();
	mouseInput.y = input->getMouseY();

	vec2 output = screenToWorld(mouseInput);

	if (input->isMouseButtonDown(0) && mouseHeldLastFrame == false) {

		mouseHeldLastFrame = true;
		firstMousePosition = output;
	}

	if (mouseHeldLastFrame) {
		aie::Gizmos::add2DLine(firstMousePosition, output, vec4(0, 0, 1, 1));
		ballVelocity = (output - firstMousePosition) * c_vMult;
	}

	if (input->wasMouseButtonReleased(0)) {
		Sphere* newSphere = new Sphere(firstMousePosition, (output - firstMousePosition) * c_vMult , c_mass, circleSize, vec4(1, 1, 1, 1));
		m_physicsScene->addActor(newSphere);
		if (currentColour >= 2)
			currentColour = -1;
		currentColour += 1;
		tutorial = false;
		mouseHeldLastFrame = false;
	}

	if (input->isMouseButtonDown(1)) {
		Sphere* newSphere = m_physicsScene->mouse2Sphere(output);
		m_physicsScene->removeActor(newSphere);
	}

}

void Application2D::ballSpring()
{

	aie::Input* input = aie::Input::getInstance();

	circleSize = 1;

	vec2 mouseInput(0, 0);
	mouseInput.x = input->getMouseX();
	mouseInput.y = input->getMouseY();
	vec2 output = screenToWorld(mouseInput);

	Sphere* firstClicked;
	Sphere* secondClicked;

	if (input->isMouseButtonDown(0) && mouseHeldLastFrame == false) {
		clickedSphere = m_physicsScene->mouse2Sphere(output);
		firstClicked = m_physicsScene->mouse2Sphere(output);
		mouseHeldLastFrame = true;
	}

	if (mouseHeldLastFrame) {
		aie::Gizmos::add2DLine(clickedSphere->getPos(), output, vec4(1, 1, 1, 1));
	}

	if (input->wasMouseButtonReleased(0)) {
		firstClicked = clickedSphere;
		secondClicked = m_physicsScene->mouse2Sphere(output);
		// dont need to check if the rigidbodies worked because if they're spheres they HAVE to be rigidbodies
		if (secondClicked != nullptr) {
			float restLength = 5;
			vec2 distance = secondClicked->getPos() - firstClicked->getPos();
			restLength = length(distance);
			std::cout << restLength << std::endl;
			Spring* newSpring = new Spring(firstClicked, secondClicked, c_springiness, c_dampener, restLength);
			m_physicsScene->addActor(newSpring);
			tutorial = false;
		}
		mouseHeldLastFrame = false;
	}

	if (scrollLastFrame != input->getMouseScroll()) {
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
			c_springiness += mouseScrollDelta;
		}
		else if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) {
			c_dampener += mouseScrollDelta * 0.1f;
		}
		else {
			k += mouseScrollDelta;
		}
	}

	if (input->isMouseButtonDown(1) && rightMouseHeldLastFrame == false) {
		clickedSphere = m_physicsScene->mouse2Sphere(output);
		firstMousePosition = output;
		rightMouseHeldLastFrame = true;
	}

	if (rightMouseHeldLastFrame && clickedSphere != nullptr) {
		vec2 forceDir = output - clickedSphere->getPos();
		vec2 force = forceDir * 0.5f;
		clickedSphere->applyForce(force);
		aie::Gizmos::add2DLine(clickedSphere->getPos(), output, vec4(1, 0, 0, 1));
	}

	if (input->wasMouseButtonReleased(1)) {
		clickedSphere = nullptr;
		rightMouseHeldLastFrame = false;
		tutorial = false;
	}

}

void Application2D::ballDelete()
{

}

void Application2D::ballGrid()
{

	aie::Input* input = aie::Input::getInstance();

	vec2 mouseInput(0, 0);
	mouseInput.x = input->getMouseX();
	mouseInput.y = input->getMouseY();

	int yIterator = 0;
	int xIterator = 0;
	float ySpacing = 2;
	float xSpacing = 2;

	vec2 output = screenToWorld(mouseInput);

	if (scrollLastFrame != input->getMouseScroll()) {
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT)) {
			gridWidth += mouseScrollDelta;
		}
		else if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL)) {
			gridHeight += mouseScrollDelta;
		}
		else {
			circleSize += mouseScrollDelta;
		}
	}

	for (int i = 0; i < gridHeight * gridWidth; i++) {
		if (i % gridHeight == 0 && i != 0) {
			yIterator++;
		}
		if (i % gridWidth == 0 && i != 0) {
			xIterator++;
		}

		vec2 position = output;	
		position.x += (i - (gridWidth * xIterator)) * (xSpacing * circleSize);
		position.y -= xIterator * (ySpacing * circleSize);
		aie::Gizmos::add2DCircle(position, circleSize, 8, vec4(0, 0, 1, 0));

	}

	if (input->isMouseButtonDown(0) && mouseHeldLastFrame != true) {
		firstMousePosition = output;
		mouseHeldLastFrame = true;
	}

	if (mouseHeldLastFrame == true) {
		aie::Gizmos::add2DLine(firstMousePosition, output, vec4(0, 0, 1, 1));
	}

	if (input->wasMouseButtonReleased(0)) {
		yIterator = 0;
		xIterator = 0;

		output = screenToWorld(mouseInput);
			
		for (int i = 0; i < gridHeight * gridWidth; i++)
		{
			if (i % gridHeight == 0 && i != 0) {
				yIterator++;
			}
			if (i % gridWidth == 0 && i != 0) {
				xIterator++;
			}

			vec2 position = output;
			position.x += (i - (gridWidth * xIterator)) * (xSpacing * circleSize);
			position.y -= xIterator * (ySpacing * circleSize);

			Sphere* newSphere = new Sphere(position, output - firstMousePosition, circleSize, circleSize, vec4(1, 1, 0, 1));

			m_physicsScene->addActor(newSphere);

			tutorial = false;

		}
		mouseHeldLastFrame = false;
	}

	scrollLastFrame = input->getMouseScroll();

}

void Application2D::springCreate()
{

}

void Application2D::planeCreate()
{

	aie::Input* input = aie::Input::getInstance();
	vec2 mouseIn = vec2(input->getMouseX(), input->getMouseY());
	vec2 output = screenToWorld(mouseIn);

	if (input->isMouseButtonDown(0) && mouseHeldLastFrame == false) {

		mouseHeldLastFrame = true;
		firstMousePosition = output;
	}

	if (mouseHeldLastFrame && !cancelAction) {
		aie::Gizmos::add2DLine(firstMousePosition, output, vec4(0, 0, 1, 1));

		if (input->wasMouseButtonPressed(1))
			cancelAction = true;

		vec2 normal = normalize(output - firstMousePosition);
		float distance = length(vec2(0,0) - firstMousePosition);
		vec4 colour = vec4(0, 0, 1, 1);

		float dotProd = dot(normal, vec2(0, 0) - output);

		if (dotProd > 0)
			distance = distance * -1;

		float lineSegmentLength = 300;
		vec2 centerPoint = normal * distance;
		vec2 parallel(normal.y, -normal.x);
		vec4 colourFade = colour;
		colourFade.a = 0;
		vec2 start = centerPoint + (parallel * lineSegmentLength);
		vec2 end = centerPoint - (parallel * lineSegmentLength);
		aie::Gizmos::add2DTri(start, end, start - normal * 10.0f, colour, colour, colourFade);
		aie::Gizmos::add2DTri(end, end - normal * 10.0f, start - normal * 10.0f, colour, colourFade, colourFade);

		ballVelocity = (output - firstMousePosition) * c_vMult;
	}

	if (input->wasMouseButtonReleased(0)) {
		
		if (cancelAction != true) {
			vec2 normal = normalize(output - firstMousePosition);
			float distance = length(vec2(0, 0) - firstMousePosition);

			float dotProd = dot(normal, vec2(0, 0) - output);

			if (dotProd > 0)
				distance = distance * -1;

			Plane* newPlane = new Plane(normal, distance);
			m_physicsScene->addActor(newPlane);
			cancelAction = false;
			tutorial = false;
		}

		cancelAction = false;
		tutorial = false;
		mouseHeldLastFrame = false;
	}
}

void Application2D::printTutorialText()
{
	switch (currentTool) {
	case BALLCREATE:
		m_2dRenderer->drawText(m_font, "Click and drag to create a sphere", 0, 512 - 16);
		m_2dRenderer->drawText(m_font, "The sphere will be given a velocity = to the distance you drag,", 0, 512 - (16 * 2));
		m_2dRenderer->drawText(m_font, "multiplied by the above velocity multiplier.", 0, 512 - (16 * 3));
		m_2dRenderer->drawText(m_font, "To change the mass and velocity multiplier, use the scroll wheel", 0, 512 - (16 * 5));
		m_2dRenderer->drawText(m_font, "and hold SHIFT or CTRL respectively.", 0, 512 - (16 * 6));
		m_2dRenderer->drawText(m_font, "Hold F while scrolling to multiply by 10 (scrolls faster)", 0, 512 - (16 * 7));
		m_2dRenderer->drawText(m_font, "Hit buttons 1, 2, and 3 to switch tools.", 0, 512 - (16 * 8));
		m_2dRenderer->drawText(m_font, "To make this text go away, create a ball", 0, 512 - (16 * 10));
		break;
	case BALLSPRING:
		m_2dRenderer->drawText(m_font, "Click and drag to another ball to create a spring", 0, 512 - 16);
		m_2dRenderer->drawText(m_font, "The spring will have the values shown", 0, 512 - (16 * 2));
		m_2dRenderer->drawText(m_font, "To change the spring coefficient, use the scroll wheel", 0, 512 - (16 * 5));
		m_2dRenderer->drawText(m_font, "Hold SHIFT or CTRL while scrolling to change the other two.", 0, 512 - (16 * 6));
		m_2dRenderer->drawText(m_font, "To make this text go away, create a spring", 0, 512 - (16 * 10));
		break;
	case BALLGRID:
		m_2dRenderer->drawText(m_font, "Click and drag to create a grid", 0, 512 - 16);
		m_2dRenderer->drawText(m_font, "The grid will be given a velocity = to the distance you drag,", 0, 512 - (16 * 2));
		m_2dRenderer->drawText(m_font, "To change the grid size use the scroll wheel", 0, 512 - (16 * 5));
		m_2dRenderer->drawText(m_font, "and hold SHIFT or CTRL respectively.", 0, 512 - (16 * 6));
		m_2dRenderer->drawText(m_font, "To make this text go away, create a grid", 0, 512 - (16 * 10));
		break;
	case PLANECREATE:
		m_2dRenderer->drawText(m_font, "Click and drag to create a plane", 0, 512 - 16);
		m_2dRenderer->drawText(m_font, "A blue preview of the plane will be shown before you place.", 0, 512 - (16 * 2));
		m_2dRenderer->drawText(m_font, "Right click to cancel placement.", 0, 512 - (16 * 3));
		break;
	}
}
