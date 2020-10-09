#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <cmath>
#include <math.h>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"


const int mpp = 2;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 500, 300, 0, 255, true);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	//Loads the background textrue from the disk into RAM
	TextureManager::Instance()->load("../Assets/textures/Background.jpg", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	//Ball
	m_pBall = new Target();
	addChild(m_pBall);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = false;


	m_pEnemy = new Player();
	addChild(m_pEnemy);
	m_playerFacingRight = true;

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(100.0f, 550.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(900.0f, 550.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);


	const SDL_Color blue = { 237, 244, 255, 255 };//light blue

	/* Instructions Label */
	m_pInstructionsLabel_1 = new Label("Press the backtick (`) to access the physics control.", "Consolas", 20, blue);
	m_pInstructionsLabel_1->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 470.0f);

	addChild(m_pInstructionsLabel_1);

	m_pInstructionsLabel_2 = new Label("Set ALL(IMPORTANT!) the values before click on 'throw' button.", "Consolas", 20, blue);
	m_pInstructionsLabel_2->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel_2);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Physics Controll", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	ImGui::Value("The scale is 1", mpp);

	if (ImGui::Button("Throw"))
	{
		m_pBall->doThrow();
	}
	/*if (buttonPressed)
	{
		std::cout << "Throw button pressed" << std:: endl;
	}*/
	ImGui::Separator();

	static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity", &isGravityEnabled))
	{
		m_pBall->isGravityEnabled = isGravityEnabled;
	}
	/*if (isGravityEnabled)
	{
		std::cout << " Gravity On" << std::endl;
	}
	else
	{
		std::cout << " Gravity Off" << std::endl;
	}*/
	static int xPlayerPos = 100;
	if (ImGui::SliderInt("Player Position X", &xPlayerPos, 0, 500))
	{
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		//m_pBall->isGravityEnabled = isGravityEnabled;
		m_pBall->throwposition = glm::vec2(xPlayerPos, 400);

	}
	static int xEnemyPos = 700;
	if (ImGui::SliderInt("Enemy Position X", &xEnemyPos, 500, 1000))
	{
		m_pEnemy->getTransform()->position.x = xEnemyPos;


	}

	float distance;
	if (xEnemyPos - xPlayerPos >= 0)
	{
		distance = xEnemyPos - xPlayerPos;
	}
	else
	{
		distance = xPlayerPos - xEnemyPos;
	}
	ImGui::Value("The distance between is: ", distance);

	glm::vec2 speed = m_pBall->getRigidBody()->velocity;
	ImGui::Value("ball speed X-axis: ", speed.x);
	ImGui::Value("ball speed Y-axis: ", -speed.y);

	static float mass = 2.2;
	ImGui::Value("The mass of the ball is: ", mass);


	/*glm::vec2 acc = m_pBall->getRigidBody()->acceleration;
	ImGui::Value("The acceleration of the ball is: ", acc.y);*/


	float accc;
	if (isGravityEnabled)
	{
		accc = -9.8;
	}
	else
	{
		accc = 0;
	}
	ImGui::Value("The acceleration of the ball is : ", accc);


	//static float velocity[2] = { 0,0 };
	//if (ImGui::SliderFloat2("Throw Speed X, Y", velocity, 0, 200))
	//{
	//	m_pBall->throwspeed = glm::vec2(velocity[0], -velocity[1]);
	//}
	static float velocity[2] = { 0,0 };

	static float ballSpeed = 95;
	static float ballAngle;
	float angleRad;
	
	if ((ImGui::SliderFloat("Thrown angle", &ballAngle, 0, 90)) /*&& (ImGui::SliderFloat("Ball speed is: ", &ballSpeed, 0, 200))*/ ) {
		angleRad = (ballAngle / 180) * M_PI;
		velocity[0] = ballSpeed * cos(angleRad);
		velocity[1] = ballSpeed * sin(angleRad);
		m_pBall->throwspeed = glm::vec2(velocity[0], -velocity[1]);
		
	}

	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}


