#include "Plane.h"
#include "TextureManager.h"

Plane::Plane()
{
	TextureManager::Instance()->load("../Assets/textures/enemy.png", "enemy1");

	//setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(53);

	// set frame height
	setHeight(58);

	getTransform()->position = glm::vec2(400.0f, 700.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	//m_buildAnimations();
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->draw("enemy1", 400, 700, 0, 255, true);
}

void Plane::update()
{
}

void Plane::clean()
{
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "plane";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane1"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane2"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane3"));

	setAnimation(planeAnimation);
}