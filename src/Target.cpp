#include "Target.h"
#include "TextureManager.h"
#include <iostream>
using namespace std;
const int mpp = 2;

Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png", "circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");


	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 400.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::m_move()
{
	float deltaTime = 1.0f / 60.0f;
	glm::vec2 gravity = glm::vec2(0, 9.8f);

	if (getTransform()->position.y <= 400) {
		/*if (!isGravityEnabled) getRigidBody()->velocity.y = 0;*/
		if (isGravityEnabled)
		{
			getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * (mpp * deltaTime);
		}
		else
		{
			getRigidBody()->velocity += getRigidBody()->acceleration * (deltaTime * mpp);
		}
		getTransform()->position += getRigidBody()->velocity * (deltaTime * mpp);

	}
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

void Target::doThrow()
{
	getTransform()->position = throwposition;
	getRigidBody()->velocity = throwspeed;
}