#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300, 0, 255, true);

	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Background.jpg", "background");

	const SDL_Color blue = { 237, 244, 255, 255 };//light blue

	m_pTitleLabel = new Label("STAR WAR", "Consolas", 125, blue, glm::vec2(500.0f, 100.0f));
	m_pTitleLabel->setParent(this);
	addChild(m_pTitleLabel);

	m_pStartLabel = new Label("Thermal Detonator Throwing Simulator", "Consolas", 40, blue, glm::vec2(500.0f, 200.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);

	m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, blue, glm::vec2(500.0f, 320.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_GroupLable = new Label("Our Team", "Consolas", 30, blue, glm::vec2(500.0f, 410.0f));
	m_GroupLable->setParent(this);
	addChild(m_GroupLable);

	m_pStudent1Lable = new Label("Jing Yuan Cheng, 101257237", "Consolas", 20, blue, glm::vec2(500.0f, 440.0f));
	m_pStudent1Lable->setParent(this);
	addChild(m_pStudent1Lable);

	m_pStudent2Lable = new Label("Feng Xiao, 101100713", "Consolas", 20, blue, glm::vec2(500.0f, 460.0f));
	m_pStudent2Lable->setParent(this);
	addChild(m_pStudent2Lable);

	m_pStudent3Lable = new Label(" Wanhao Sun, 101277502", "Consolas", 20, blue, glm::vec2(500.0f, 480.0f));
	m_pStudent3Lable->setParent(this);
	addChild(m_pStudent3Lable);


	/*m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(400.0f, 300.0f); 
	addChild(m_pShip); */

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->getTransform()->position = glm::vec2(500.0f, 550.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	
}

