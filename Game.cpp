/*
*	Game.cpp
*	Implementation of Game.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*	NOTE: Player only has one bullet to shoot. It has to wait for it to hit the enemy, or the cooldown timer or before it reaches far
*	enough so that it disappears.
*/

#include "Game.h"
#include "TexturedShader.h"
#include "StaticObject.h"

#include "FlyingCamera.h"
#include "ThirdPersonCamera.h"
#include "FirstPersonCamera.h"

#include "MathsHelper.h"
#include "DirectXTK/CommonStates.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_stateMachine = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_diffuseTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
	m_quitButton = NULL;
	m_startButton = NULL;
	//m_player = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, AudioSystem* audio, InputController* input)
{
	m_renderer = renderer;
	m_input = input;
	m_audio = audio;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();
	//m_player = new Player(m_currentCam->GetPosition());

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	LoadFonts();
	InitUI();
	LoadAudio();
	m_currentCam = new FirstPersonCamera(m_input, Vector3(0, 62, 0), m_audio); // Our first person camera
	InitGameWorld();
	RefreshUI();
	InitStates();

	// Our collision manager managing collisions of all game objects separately.
	m_collisionManager = new CollisionManager(&m_rubies, &m_ammoBlocks, &m_bullets, &m_enemies, &m_players, &m_addons, &m_teleporters, m_currentCam, &m_grenades, &m_itemBoxes);

	//m_currentCam = new FlyingCamera(m_input, Vector3(0, 10, -20));

	//m_currentCam = new ThirdPersonCamera(m_player, Vector3(0, 10, -25), true, 2.0f);

	m_engineSound = m_audio->Play("Assets/Sounds/Sasuke_2.wav", true);

	if (m_engineSound)
	{
		m_engineSound->SetLoopCount(-1);
		m_engineSound->SetIs3D(true);
		m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
		m_engineSound->SetPaused(false);
		//b_sound = false;
	}

	return true;
}


bool Game::InitShaders()
{
	// Loading the shaders
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitVertexColourPixelShader.ps"))
		return false;

	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader();
	if (!m_diffuseTexturedFogShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedFogPixelShader.ps"))
		return false;

	m_diffuseTexturedShader = new TexturedShader();
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	// Loading the Meshes
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/temp.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/item_box.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ammoBlock.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/sg-armor.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/grenade.obj"))
		return false;
	return true;
}

bool Game::LoadTextures()
{
	// Loading the textures
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ammo.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/button.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/bullet.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_star.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redDarker.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redLighter.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redOrange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redPink.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/purple.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/paralysis.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/invisibility.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/yellow.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/green.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/grenade.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/teleporter.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/item_box.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/teleporter1.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/teleporter_3.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/skating_ice.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/Brick white plastered.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/lava.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ice.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/fire1.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/fire.jpg"))
		return false;

	/*if (!m_textureManager->Load(m_renderer, "Assets/Textures/steel.jpg"))
	return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/Football_1.jpg"))
	return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/Football_2.jpg"))
	return false;*/

	return true;
}

bool Game::LoadAudio()
{
	if (!m_audio->Load("Assets/Sounds/Sasuke_2.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/Sasuke_4.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/drumloop.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/swish.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/teleporter.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/handgun.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/wearing jacket.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/throwing fire grenade.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/explosion.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/Victory_Sound_Effect.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/Game_Over_sound_effect.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/Correct_Answer_Sound_Effect.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/release.wav"))
		return false;

	if (!m_audio->Load("Assets/Sounds/collect_ruby.wav"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
}

void Game::InitUI()
{
	// Creating the sprites which would be displayed on the UI.
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext()); // Sprite manager
	m_currentItemSprite = m_textureManager->GetTexture("Assets/Textures/sprite_star.png"); // Game info sprite
	m_flashRed = m_textureManager->GetTexture("Assets/Textures/sprite_hurtOverlay.png"); // Flash red sprite.
	m_health = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png"); // Health bar sprite.
	m_ammo = m_textureManager->GetTexture("Assets/Textures/ammo.png"); // Ammo bar sprite.
	m_paralysis = m_textureManager->GetTexture("Assets/Textures/paralysis.png"); // Paralysis sprite
	m_invisibility = m_textureManager->GetTexture("Assets/Textures/invisibility.png"); // Paralysis sprite

	Texture* buttonTexture = m_textureManager->GetTexture("Assets/Textures/button.png");

	// Also init any buttons here
	m_startButton = new Button(128, 64, buttonTexture, L"Start Game", Vector2(564, 365), m_spriteBatch, m_arialFont12, m_input, [this]
	{
		// Transition into the Gameplay state (these buttons are only used on the menu screen)
		m_stateMachine->ChangeState(GameStates::GAMEPLAY_STATE);
	});

	m_quitButton = new Button(128, 64, buttonTexture, L"Quit", Vector2(694, 365), m_spriteBatch, m_arialFont12, m_input, [this]
	{
		// Tell windows to send a WM_QUIT message into our message pump
		PostQuitMessage(0);
	});

	m_freeExplore = new Button(128, 64, buttonTexture, L"Free Explore", Vector2(824, 365), m_spriteBatch, m_arialFont12, m_input, [this]
	{
		// Transition into the Gameplay state (these buttons are only used on the menu screen)
		m_stateMachine->ChangeState(GameStates::FREE_EXPLORE);
	});

	m_resume = new Button(128, 64, buttonTexture, L"Resume", Vector2(1150, 50), m_spriteBatch, m_arialFont12, m_input, [this]
	{
		// Transition into the Gameplay state (these buttons are only used on the menu screen)
		m_stateMachine->ChangeState(GameStates::GAMEPLAY_STATE);
	});

	m_controls = new Button(128, 64, buttonTexture, L"Controls", Vector2(838, 385), m_spriteBatch, m_arialFont12, m_input, [this]
	{
		// Transition into the Gameplay state (these buttons are only used on the menu screen)
		m_stateMachine->ChangeState(GameStates::HELP_STATE);
	});

	// Also init any buttons here
}

void Game::RefreshUI()
{
	// Ensure text in UI matches latest scores etc (call this after data changes)
	if (m_players[0]->AmmoLeft() > 15)
		m_players[0]->SetAmmo(15); // Ammo can't go beyond 15.
	if (m_players[0]->AmmoLeft() < 0)
		m_players[0]->SetAmmo(0); // Ammo can't go beneath 0.

								  // The scores and player information that will be displayed and be dynamically changed.
	std::wstringstream ss;
	ss << "Rubies Collected: " << m_players[0]->RubiesCollected() << "/15";
	m_rubiesCollected = ss.str();

	std::wstringstream s1;
	s1 << "Ammunition: " << m_players[0]->AmmoLeft() << "/15";
	m_ammoLeft = s1.str();

	std::wstringstream s2;
	s2 << "Health: " << m_players[0]->getHealth() << "/100";;
	m_healthRemaining = s2.str();

	std::wstringstream s3;
	s3 << " x " << m_players[0]->AmmoLeft();
	m_ammoLeft2 = s3.str();

	std::wstringstream s4;
	s4 << m_players[0]->getScore();
	m_score = s4.str();

	std::wstringstream s5;
	s5 << m_players[0]->getTreasures();
	m_treasure = s5.str();

}

void Game::InitStates()
{
	// Our state machine needs to know its owner (so it only runs the callbacks while its owner exists)
	m_stateMachine = new StateMachine<GameStates, Game>(this, GameStates::MENU_STATE);

	// Let's match some states with with their OnEnter, OnUpdate, OnRender, OnExit callbacks.
	// Have a look in StateMachine.h to see how this works internally.
	m_stateMachine->RegisterState(GameStates::MENU_STATE, &Game::Menu_OnEnter,
		&Game::Menu_OnUpdate, &Game::Menu_OnRender, &Game::Menu_OnExit);

	m_stateMachine->RegisterState(GameStates::GAMEPLAY_STATE, &Game::Gameplay_OnEnter,
		&Game::Gameplay_OnUpdate, &Game::Gameplay_OnRender, &Game::Gameplay_OnExit);

	m_stateMachine->RegisterState(GameStates::PAUSE_STATE, &Game::Pause_OnEnter,
		&Game::Pause_OnUpdate, &Game::Pause_OnRender, &Game::Pause_OnExit);

	m_stateMachine->RegisterState(GameStates::HELP_STATE, &Game::Help_OnEnter,
		&Game::Help_OnUpdate, &Game::Help_OnRender, &Game::Help_OnExit);

	m_stateMachine->RegisterState(GameStates::FREE_EXPLORE, &Game::Free_OnEnter,
		&Game::Free_OnUpdate, &Game::Free_OnRender, &Game::Free_OnExit);
}

void Game::InitGameWorld()
{
	// Static scenery objects
	StaticObject* ground = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png"), Vector3(0, 0, -0.5));

	StaticObject* hell = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/fire1.jpg"), Vector3(0, -60, -0.5));
	//m_gameObjects.push_back();

	StaticObject* ice = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/skating_ice.jpg"), Vector3(0, 60, -0.5));

	ground->SetUniformScale(0.35);
	hell->SetUniformScale(0.35);
	ice->SetUniformScale(0.35);

	StaticObject* obj = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png"), Vector3(0, 60, 0));

	StaticObject* obj_2 = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png"), Vector3(0, 0, 0));

	StaticObject* wall_1 = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/Brick white plastered.jpg"), Vector3(0, 0, -50));

	StaticObject* wall_2 = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/Brick white plastered.jpg"), Vector3(0, 0, 49));

	StaticObject* wall_3 = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/Brick white plastered.jpg"), Vector3(49.5, 0, -0.5));

	StaticObject* wall_4 = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/Brick white plastered.jpg"), Vector3(-49.5, 0, -0.5));

	wall_1->SetXScale(0.35);
	wall_1->SetZScale(0.51);

	wall_2->SetXScale(0.35);
	wall_2->SetZScale(0.51);

	wall_3->SetXScale(0.35);
	wall_3->SetZScale(0.51);

	wall_4->SetXScale(0.35);
	wall_4->SetZScale(0.51);
	//obj2->SetUniformScale(0.7);
	m_gameObjects.push_back(ground);
	m_gameObjects.push_back(hell);
	m_gameObjects.push_back(ice);

	wall_1->SetXRotation(PI / 2);

	wall_2->SetXRotation(PI / 2);
	wall_2->SetYRotation(PI);

	wall_3->SetZRotation(-PI / 2);
	wall_3->SetXRotation(PI / 2);
	wall_3->SetYRotation(PI);

	wall_4->SetZRotation(PI / 2);
	wall_4->SetXRotation(PI / 2);
	wall_4->SetYRotation(PI);

	obj->SetXRotation(PI);
	obj->SetUniformScale(0.35);

	obj_2->SetXRotation(PI);
	obj_2->SetUniformScale(0.35);

	m_gameObjects.push_back(obj);
	m_gameObjects.push_back(obj_2);
	m_gameObjects.push_back(wall_1);
	m_gameObjects.push_back(wall_2);
	m_gameObjects.push_back(wall_3);
	m_gameObjects.push_back(wall_4);
	// A player (invisible with no texture or mesh) object to attach to the First-person camera
	m_players.push_back(new PersonObject(m_currentCam->GetPosition(), m_audio));
	m_players[0]->setType(0);
	m_players[0]->setLevel(1);
	if (infinite)
		m_players[0]->FreeRoam(true);
	m_currentCam->setPlayer(m_players[0]);

	// Creating the bullets, rubies, ammunition blocks and enemies.
	InitItems();
	InitEnemies();
}

void Game::InitEnemies()
{
	std::string base = "Assets/Textures/";
	int count = 0;
	for (signed int j = -60; j < 80; j += 60)
	{
		for (int i = 0; i < 5; i++)
		{
			std::string tex = base + str[i];
			const char* charTex = tex.c_str();
			Vector3 position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), j, MathsHelper::RandomRange(-45.0f, 45.0f));
			PersonObject* enemy = new PersonObject(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture(charTex),
				position);

			enemy->setType(i + 1); // Enemy types: 1, 2, 3, 4, 5

			if (i == 2 && j == -60)
				enemy->setRubies(&m_rubies1);
			if (i == 2 && j == 0)
				enemy->setRubies(&m_rubies2);
			if (i == 2 && j == 60)
				enemy->setRubies(&m_rubies3);

			Bullet* bullet = new Bullet(m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture("Assets/Textures/bullet.png"),
				enemy->GetPosition() + Vector3(0, -4, 0));

			bullet->setOwner(1); // Enemy owns it
			bullet->setShot(0); // It has not been shot
			bullet->SetUniformScale(3.0f);

			enemy->AddBullet(bullet); // Enemy has been allocated the bullet
			m_bullets.push_back(bullet); // Adding enemy bullet to the bullet pool
			m_enemies.push_back(enemy); // Adding enemy to the enemy pool

										// Initialising the current and previous positions of player in enemies and player to create time warp funtionality.
			enemy->setPrevPos(m_players[0]->GetPosition());
			enemy->setCurrPos(m_players[0]->GetPosition());
			m_players[0]->setPrevPos(m_players[0]->GetPosition());
			m_players[0]->setCurrPos(m_players[0]->GetPosition());
			m_gameObjects.push_back(enemy); // Adding enemy to gameObjects
			m_gameObjects.push_back(bullet); // Adding bullet to gameObjects

			Vector3 pos = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), j, MathsHelper::RandomRange(-45.0f, 45.0f));
			int v = rand() % 9;
			if (j == -60)
			{
				bullet->setLevel(-1);
				m_enemies[count]->setLevel(-1);
				m_enemies1.push_back(m_enemies[count]);
			}

			if (j == 0)
			{
				bullet->setLevel(0);
				m_enemies[count]->setLevel(0);
				m_enemies2.push_back(m_enemies[count]);
			}

			if (j == 60)
			{
				bullet->setLevel(1);
				m_enemies[count]->setLevel(1);
				m_enemies3.push_back(m_enemies[count]);
			}

			if (i == 1)
				m_enemies[count]->setRandomPosition(pos); // Setting the first random point the 2nd enemy will head to

			if (i == 2)
			{
				switch (j)
				{
				case -60:
					m_enemies[count]->setRandomPosition(m_rubies1[v]->GetPosition()); // Setting the first random ruby the 3rd enemy will head to.
					break;
				case 0:
					m_enemies[count]->setRandomPosition(m_rubies2[v]->GetPosition()); // Setting the first random ruby the 3rd enemy will head to.
					break;
				case 60:
					m_enemies[count]->setRandomPosition(m_rubies3[v]->GetPosition()); // Setting the first random ruby the 3rd enemy will head to.
					break;
				}
			}
			count++;
			//char msgbuf[100];
			//sprintf_s(msgbuf, "\nCount: %d\n", i+count-1);
			//OutputDebugString(msgbuf);
		}
	}
	// So that our grenade knows where enemies are
	grenade->setEnemy1(m_enemies1);
	grenade->setEnemy2(m_enemies2);
	grenade->setEnemy3(m_enemies3);

	grenade->setBox1(boxes1);
	grenade->setBox2(boxes2);
	grenade->setBox3(boxes3);

	m_currentCam->setGrenade(grenade);
	char msgbuf[100];
	//sprintf_s(msgbuf, )
}

void Game::InitItems()
{
	for (signed int j = -60; j < 80; j += 60)
	{
		for (int i = 0; i < 10; i++)
		{
			Vector3 position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), j, MathsHelper::RandomRange(-45.0f, 45.0f));

			ItemObject* ruby = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png"),
				position);
			ruby->SetType(2); // Type 0: Ammo, Type 1: Armor, Type 2: Ruby
			ruby->SetUniformScale(2);
			m_rubies.push_back(ruby);
			m_gameObjects.push_back(ruby); // Putting the ruby in  gameObjects
			switch (j)
			{
			case -60:
				m_rubies1.push_back(ruby); // Putting the ruby to the ruby pool of dragon lair
				ruby->setLevel(-1);
				break;
			case 0:
				m_rubies2.push_back(ruby); // Putting the ruby to the ruby pool of Egypt
				ruby->setLevel(0);
				break;
			case 60:
				m_rubies3.push_back(ruby); // // Putting the ruby to the ruby pool of Frosty Caves
				ruby->setLevel(1);
				break;
			}
		}
	}

	// Treasure rubies for each world
	ItemObject* ruby1 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/purple.jpg"),
		Vector3(21, 1.5, 45));
	ItemObject* ruby2 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/purple.jpg"),
		Vector3(21, -58.5, 45));
	ItemObject* ruby3 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/purple.jpg"),
		Vector3(21, 61.5, 45));

	// Invisible rubies for each world
	ItemObject* invisible1 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/green.png"),
		Vector3(11, 1.5, -23));
	ItemObject* invisible2 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/green.png"),
		Vector3(11, -58.5, -23));
	ItemObject* invisible3 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/green.png"),
		Vector3(11, 61.5, -23));

	// Paralysis Rubies for each world
	ItemObject* paralysis1 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/yellow.jpg"),
		Vector3(19, 1.5, 23));
	ItemObject* paralysis2 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/yellow.jpg"),
		Vector3(19, -58.5, 23));
	ItemObject* paralysis3 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/yellow.jpg"),
		Vector3(19, 61.5, 23));

	//Customizing the special rubies
	ruby1->SetUniformScale(5);
	ruby1->SetType(12);
	ruby2->SetUniformScale(5);
	ruby2->SetType(12);
	ruby3->SetUniformScale(5);
	ruby3->SetType(12);

	paralysis1->SetUniformScale(5);
	paralysis1->SetType(13);
	paralysis2->SetUniformScale(5);
	paralysis2->SetType(13);
	paralysis3->SetUniformScale(5);
	paralysis3->SetType(13); 
	
	invisible1->SetUniformScale(5);
	invisible1->SetType(14);
	invisible2->SetUniformScale(5);
	invisible2->SetType(14);
	invisible3->SetUniformScale(5);
	invisible3->SetType(14);

	m_gameObjects.push_back(ruby1);
	m_gameObjects.push_back(ruby2);
	m_gameObjects.push_back(ruby3);
	m_rubies.push_back(ruby1);
	m_rubies.push_back(ruby2);
	m_rubies.push_back(ruby3);

	m_gameObjects.push_back(paralysis1);
	m_gameObjects.push_back(paralysis2);
	m_gameObjects.push_back(paralysis3);
	m_rubies.push_back(paralysis1);
	m_rubies.push_back(paralysis2);
	m_rubies.push_back(paralysis3);

	m_gameObjects.push_back(invisible1);
	m_gameObjects.push_back(invisible2);
	m_gameObjects.push_back(invisible3);
	m_rubies.push_back(invisible1);
	m_rubies.push_back(invisible2);
	m_rubies.push_back(invisible3);

	for (signed int j = -60; j < 80; j += 60)
	{
		for (int i = 0; i < 2; i++)
		{
			// Generating random position for the ammunition block
			Vector3 position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), j + 1, MathsHelper::RandomRange(-45.0f, 45.0f));

			ItemObject* ammoBlock = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png"),
				position);
			ammoBlock->SetType(0); // Type 0: Ammo, Type 1: Bullet, Type 2: Ruby
			ammoBlock->SetUniformScale(2);
			m_ammoBlocks.push_back(ammoBlock);
			m_gameObjects.push_back(ammoBlock); // Putting the ammo in  gameObjects
			switch (j) 
			{
			case -60: // Dragon lair
				m_ammoBlocks1.push_back(ammoBlock); // Putting the ammo to the ammo pool
				ammoBlock->setLevel(-1);
				break;
			case 0: // Egypt
				m_ammoBlocks2.push_back(ammoBlock);
				ammoBlock->setLevel(0);
				break;
			case 60: // Frosty Caves
				m_ammoBlocks3.push_back(ammoBlock);
				ammoBlock->setLevel(1);
				break;
			}
		}
	}


	// Bullet for the player
	Bullet* bullet = new Bullet(m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/bullet.png"),
		m_currentCam->GetPosition() + Vector3(0, -4, 0));

	// A grenade to be observed closely at frosty caves. Can be used as an idea to generate grenade pick ups
	Grenade* grenade_collection = new Grenade(m_meshManager->GetMesh("Assets/Meshes/grenade.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/grenade.png"),
		m_currentCam->GetPosition() + Vector3(0, -1, 10));

	// Our actual hand grenade
	Grenade* hand_grenade = new Grenade(m_meshManager->GetMesh("Assets/Meshes/grenade.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/grenade.png"),
		m_currentCam->GetPosition() + Vector3(0, -4, 0), m_audio);

	grenade = hand_grenade;
	grenade->setLevel(1);
	m_grenades.push_back(grenade);
	hand_grenade->SetStatus(true);
	hand_grenade->setThrown(0);// Not thrown
	grenade_collection->SetStatus(true);
	bullet->setShot(0); // It has not been shot
	bullet->setOwner(0); // Player owns this
	bullet->SetStatus(true); // It will not be rendered
	bullet->SetUniformScale(3.0f);

	m_bullets.push_back(bullet); // Adding the player bullet to the bullet pool
	m_currentCam->AddBullet(bullet); // Adding the bullet to the player
	m_gameObjects.push_back(bullet); // Adding the bullet to the gameObjects

	m_gameObjects.push_back(grenade_collection);
	m_gameObjects.push_back(hand_grenade);
	m_currentCam->AddGrenade(hand_grenade);


	// The three keplars/armors in the three worlds
	ItemObject* armor = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/sg-armor.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter.jpg"),
		Vector3(3, 1, 2));

	ItemObject* armor2 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/sg-armor.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter.jpg"),
		Vector3(3, -59, 2));

	ItemObject* armor3 = new ItemObject(m_meshManager->GetMesh("Assets/Meshes/sg-armor.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter.jpg"),
		Vector3(3, 61, 2));

	int count = 0;

	// Item Boxes for each world that restrict teleportation usage
	for (signed i = -60; i < 61; i += 60)
	{
		for (signed int j = -15; j < 30; j += 20)
		{
			ItemBox* itemBox = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/item_box.obj"),
				m_diffuseTexturedFogShader,
				m_textureManager->GetTexture("Assets/Textures/item_box.png"),
				Vector3(j, i, 22 - j));
			m_gameObjects.push_back(itemBox);
			itemBox->SetStatus(true);
			m_itemBoxes.push_back(itemBox);
			count++;
			if (i == -60)
			{
				boxes1.push_back(itemBox);
				itemBox->setTeleporter(1);
			}
				
			if (i == 0)
			{
				boxes2.push_back(itemBox);
				itemBox->setTeleporter(2);
			}
				
			if (i == 60)
			{
				boxes3.push_back(itemBox);
				itemBox->setTeleporter(4);
			}
				
		}

		// Egypt has 6 item boxes
		if (i == 0)
		{
			for (signed int j = -15; j < 30; j += 20)
			{
				ItemBox* itemBox = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/item_box.obj"),
					m_diffuseTexturedFogShader,
					m_textureManager->GetTexture("Assets/Textures/item_box.png"),
					Vector3(j - 10, i, 22 - j));
				m_gameObjects.push_back(itemBox);
				itemBox->SetStatus(true);
				itemBox->setTeleporter(3);
				m_itemBoxes.push_back(itemBox);
				count++;
				boxes2.push_back(itemBox);
			}
		}
	}

	char msgbuf[100];
	sprintf_s(msgbuf, "Item Boxes: %d", count);
	OutputDebugString(msgbuf);

	// Teleporters for each world. Egypt has 2
	Teleporter* teleporter_A = new Teleporter(m_meshManager->GetMesh("Assets/Meshes/temp.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter1.jpg"),
		Vector3(0, 0, 3));

	Teleporter* teleporter_B = new Teleporter(m_meshManager->GetMesh("Assets/Meshes/temp.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter_3.jpg"),
		Vector3(0, 0, -10));

	Teleporter* teleporter_C = new Teleporter(m_meshManager->GetMesh("Assets/Meshes/temp.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter.jpg"),
		Vector3(0, 60, -10));

	Teleporter* teleporter_D = new Teleporter(m_meshManager->GetMesh("Assets/Meshes/temp.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/teleporter.jpg"),
		Vector3(0, -60, -10));

	teleporter_A->setID(2);
	teleporter_B->setID(3);
	teleporter_C->setID(4);
	teleporter_D->setID(1);
	// Where do each of the teleporters teleport to
	teleporter_A->setLevel(1);
	teleporter_B->setLevel(-1);
	teleporter_C->setLevel(0);
	teleporter_D->setLevel(0);


	armor->SetType(1);
	armor->SetUniformScale(0.0049);
	armor->SetStatus(true);
	armor2->SetType(1);
	armor2->SetUniformScale(0.0049);
	armor2->SetStatus(true);
	armor3->SetType(1);
	armor3->SetUniformScale(0.0049);
	armor3->SetStatus(true);
	m_addons.push_back(armor);
	m_gameObjects.push_back(armor);
	/*m_addons.push_back(armor2);
	m_gameObjects.push_back(armor2);
	m_addons.push_back(armor3);
	m_gameObjects.push_back(armor3);
	*/

	teleporter_A->SetUniformScale(0.40);
	teleporter_B->SetUniformScale(0.40);
	teleporter_C->SetUniformScale(0.40);
	teleporter_D->SetUniformScale(0.40);

	m_gameObjects.push_back(teleporter_A);
	m_gameObjects.push_back(teleporter_B);
	m_gameObjects.push_back(teleporter_C);
	m_gameObjects.push_back(teleporter_D);

	m_teleporters.push_back(teleporter_A);
	m_teleporters.push_back(teleporter_B);
	m_teleporters.push_back(teleporter_C);
	m_teleporters.push_back(teleporter_D);


}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	if (infinite)
		m_players[0]->FreeRoam(true);
	if (m_input->GetKeyDown('M') && b_sound)
	{
		OutputDebugString("No Music!\n");
		m_engineSound->SetMute(true);
		b_sound = false;
	}

	else if (m_input->GetKeyDown('M') && !b_sound)
	{
		OutputDebugString("Music Please!\n");
		m_engineSound->SetMute(false);
		b_sound = true;
	}

	if (m_input->GetKeyDown('L') && sound_effects)
		sound_effects = false;

	else if (m_input->GetKeyDown('L') && !sound_effects)
		sound_effects = true;

	m_currentCam->setSoundEffects(sound_effects);
	m_players[0]->setSoundEffects(sound_effects);
	m_grenades[0]->setSoundEffects(sound_effects);


	/*char msgbuf[100];
	sprintf_s(msgbuf, "%d %d\n", m_players[0]->GetPosition().x, m_players[0]->GetPosition().z);
	OutputDebugString(msgbuf);*/

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	// The state machine knows what state we're in, so things are nice and simple out here
	m_stateMachine->Update(timestep);


	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.74902, 0.847059, 0.847059, 1.0f);
	//m_renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//RefreshUI();
	//DrawGameUI();
	m_stateMachine->Render();

	m_renderer->EndScene();

}

void Game::Menu_OnEnter()
{
	OutputDebugString("Menu OnEnter\n");
}

void Game::Menu_OnUpdate(float timestep)
{
	// Button's need to update so they can check if the mouse is over them (they swap to a hover section of the image)
	m_startButton->Update();
	m_quitButton->Update();
	m_freeExplore->Update();
}

void Game::Menu_OnRender()
{
	DrawMenuUI();
}

void Game::Menu_OnExit()
{
	OutputDebugString("Menu OnExit\n");
}

void Game::Free_OnEnter()
{
	char msgbuf[100];
	sprintf_s(msgbuf, "FREE %d\n", infinite);
	OutputDebugString(msgbuf);
	OutputDebugString("GamePlay OnEnter\n");
	infinite = true;
}
void Game::Free_OnUpdate(float timestep)
{
	// Update all our gameobjects. What they really are doesn't matter
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep);
	}

	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->setPlayerPos(m_players[0]->GetPosition());
	}
	// Only interested in collisions during the gameplay state
	m_collisionManager->CheckCollisions();

	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->setPlayerPosition(m_players[0]->GetPosition()); // Position of our player so that the enemy can turn

																		  // To calculate how fast the player is moving
		m_enemies[i]->setPrevPos(m_enemies[i]->getCurrPos()); // The previous position of the player
		m_enemies[i]->setCurrPos(m_players[0]->GetPosition()); // The current position of the player

		char msgbuf[100];
		sprintf_s(msgbuf, "INVISIBILITY: %dPARALYSIS: %d\n", m_players[0]->getStealth(), m_players[0]->getParalyse());
		//OutputDebugString(msgbuf);
		m_enemies[i]->paralyse(m_players[0]->getParalyse());
		m_enemies[i]->setStealth(m_players[0]->getStealth());

		if (m_enemies[i]->getBullet()->getShot() == 0) // If the bullet is not shot, bring it back to the enemy
		{
			m_enemies[i]->getBullet()->SetPosition(m_enemies[i]->GetPosition() + Vector3(0, -4, 0));
		}

		if (m_enemies[i]->getLives() <= 0)
			m_enemies[i]->SetStatus(false);
		if (m_enemies[i]->getHealth() < 0 && m_enemies[i]->getLives() != 0) // If enemy dies, take the enemy to some other random location and replenish health
		{
			switch (m_enemies[i]->getLevel())
			{
			case -1:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -60.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			case 0:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 0.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			case 1:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 60.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			default:
				break;
			}
		}
	}

	m_currentCam->Update(timestep);

	// Setting current and previous position to calculate time warp.
	m_players[0]->setPrevPos(m_players[0]->getCurrPos());
	m_players[0]->setCurrPos(m_players[0]->GetPosition());

	//m_players[0]->Update(timestep, m_currentCam->GetPosition());

	// Should we pause
	if (m_input->GetKeyDown('P'))
	{
		m_stateMachine->ChangeState(GameStates::PAUSE_STATE);
	}

	if (m_input->GetKeyDown('C'))
	{
		m_stateMachine->ChangeState(GameStates::HELP_STATE);
	}
}
void Game::Free_OnRender()
{
	// Draw our gameobjects

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}

	RefreshUI();
	DrawGameUI();
}
void Game::Free_OnExit()
{
	OutputDebugString("GamePlay OnExit\n");

}
void Game::Gameplay_OnEnter()
{
	char msgbuf[100];
	sprintf_s(msgbuf, "FREE %d\n", infinite);
	OutputDebugString(msgbuf);
	OutputDebugString("GamePlay OnEnter\n");
}

void Game::Gameplay_OnUpdate(float timestep)
{
	// Update all our gameobjects. What they really are doesn't matter
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep);
	}

	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->setPlayerPos(m_players[0]->GetPosition());
	}
	// Only interested in collisions during the gameplay state
	m_collisionManager->CheckCollisions();

	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->setPlayerPosition(m_players[0]->GetPosition()); // Position of our player so that the enemy can turn

																	  // To calculate how fast the player is moving
		m_enemies[i]->setPrevPos(m_enemies[i]->getCurrPos()); // The previous position of the player
		m_enemies[i]->setCurrPos(m_players[0]->GetPosition()); // The current position of the player

		m_enemies[i]->paralyse(m_players[0]->getParalyse());
		m_enemies[i]->setStealth(m_players[0]->getStealth());

		if (m_enemies[i]->getLives() <= 0)
			m_enemies[i]->SetStatus(false);

		if (m_enemies[i]->getBullet()->getShot() == 0) // If the bullet is not shot, bring it back to the enemy
		{
			m_enemies[i]->getBullet()->SetPosition(m_enemies[i]->GetPosition() + Vector3(0, -4, 0));
		}
		if (m_enemies[i]->getHealth() < 0 && m_enemies[i]->getLives() != 0) // If enemy dies, take the enemy to some other random location and replenish health
		{
			switch (m_enemies[i]->getLevel())
			{
			case -1:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -60.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			case 0:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 0.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			case 1:
				m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 60.0f, MathsHelper::RandomRange(-45.0f, 45.0f)));
				m_enemies[i]->setHealth(100);
				break;
			default:
				break;
			}

		}
	}

	m_currentCam->Update(timestep);

	// Setting current and previous position to calculate time warp.
	m_players[0]->setPrevPos(m_players[0]->getCurrPos());
	m_players[0]->setCurrPos(m_players[0]->GetPosition());

	//m_players[0]->Update(timestep, m_currentCam->GetPosition());

	// Should we pause
	if (m_input->GetKeyDown('P'))
	{
		m_stateMachine->ChangeState(GameStates::PAUSE_STATE);
	}

	if (m_input->GetKeyDown('C'))
	{
		m_stateMachine->ChangeState(GameStates::HELP_STATE);
	}
}

void Game::Gameplay_OnRender()
{
	// Draw our gameobjects

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}

	RefreshUI();
	DrawGameUI();
}

void Game::Gameplay_OnExit()
{
	OutputDebugString("GamePlay OnExit\n");
}

void Game::Pause_OnEnter()
{
	OutputDebugString("Pause OnEnter\n");
	m_engineSound->SetPaused(true);
}

void Game::Pause_OnUpdate(float timestep)
{
	// Check if we should exit pause
	if (m_input->GetKeyDown('P'))
	{
		m_stateMachine->ChangeState(GameStates::GAMEPLAY_STATE);
	}
}

void Game::Pause_OnRender()
{
	// Keep drawing the game when paused (it's not being updated though which is what freezes it)
	Gameplay_OnRender();

	// In addition to the game, draw some UI to say we're paused
	DrawPauseUI();
}

void Game::Pause_OnExit()
{
	OutputDebugString("Pause OnExit\n");
	m_engineSound->SetPaused(false);
}

void Game::Help_OnEnter()
{
	OutputDebugString("Help On Enter\n");
}

void Game::Help_OnUpdate(float timestep)
{
	m_resume->Update();
}

void Game::Help_OnRender()
{
	DrawHelpUI();
}

void Game::Help_OnExit()
{

}

void Game::DrawHelpUI()
{
	BeginUI();

	m_resume->Render();
	m_arialFont12->DrawString(m_spriteBatch, L"ESC - Quit game", Vector2(128, 100), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"J - Throw hand grenade", Vector2(128, 130), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"P - Toggle Pause", Vector2(128, 160), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"M - Toggle Background Music", Vector2(128, 190), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"L - Toggle Sound Effects", Vector2(128, 220), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Left click - Shoot bullet", Vector2(128, 250), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Level Dragon's lair - Slow speed and cooldown, high damage and most points", Vector2(128, 280), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Level Frosty Caves - Fast speed and cooldown, low damage, medium points", Vector2(128, 310), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Level 21st Century - Normal speed, cooldown and damage, least points", Vector2(128, 340), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"AIM - Collect purple rubies from each level along with atleast 15 rubies", Vector2(250, 40), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"Teleporters need to be unlocked by destroying boxes by bullets or hand grenades", Vector2(250, 680), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"N - Toggle Night Vision (Not supported)", Vector2(128, 370), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);

	m_arialFont12->DrawString(m_spriteBatch, L"Green Rubies provide invisibility against all enemies except The Chaser for some time", Vector2(128, 430), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Yellow Rubies paralyse all enemies for some time", Vector2(128, 460), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont12->DrawString(m_spriteBatch, L"Red and purple rubies give points", Vector2(128, 490), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);

	EndUI();
}
void Game::DrawMenuUI()
{
	BeginUI();

	m_startButton->Render();
	m_quitButton->Render();
	m_freeExplore->Render();
	m_arialFont18->DrawString(m_spriteBatch, L"Super Hot", Vector2(630, 285), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);

	EndUI();
}

void Game::DrawGameUI()
{
	BeginUI();

	// We don't want to see instructional text like this in your assignments (that's what a readme is for)
	if (!sound_effects)
		m_arialFont18->DrawString(m_spriteBatch, L"Sound Effects turned off", Vector2(880, 25), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	// Let's draw some text over our game
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(1225, 145), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"Score: ", Vector2(25, 210), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_score.c_str(), Vector2(95, 210), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"Treasure's Remaining: ", Vector2(25, 250), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_treasure.c_str(), Vector2(275, 250), Color(1.0f, 1.0f, 1.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"C for controls and instructions", Vector2(520, 720), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, L"Level:", Vector2(1130, 200), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_ammoLeft2.c_str(), Vector2(45, 85), Color(0.0f, 1.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_rubiesCollected.c_str(), Vector2(20, 20), Color(0.627451, 0.12549, 0.941176), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_ammoLeft.c_str(), Vector2(20, 50), Color(1, 0, 0), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	//m_arialFont18->DrawString(m_spriteBatch, m_healthRemaining.c_str(), Vector2(20, 80), Color(0.0f, 1.0f, 0.0f), 0, Vector2(0, 0));
	if (m_players[0]->getLevel() == 0)
		m_arialFont18->DrawString(m_spriteBatch, L"Egypt", Vector2(1200, 200), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	if (m_players[0]->getLevel() == -1)
		m_arialFont18->DrawString(m_spriteBatch, L"Dragon Lair", Vector2(1200, 200), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	if (m_players[0]->getLevel() == 1)
		m_arialFont18->DrawString(m_spriteBatch, L"Frosty Caves", Vector2(1200, 200), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);

	if (m_players[0]->getParalyse() == 0)
		m_spriteBatch->Draw(m_paralysis->GetShaderResourceView(), Vector2(25, 280), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // game info
	if (m_players[0]->getStealth())
		m_spriteBatch->Draw(m_invisibility->GetShaderResourceView(), Vector2(25, 330), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // game info

	// Here's how we draw a sprite over our game (caching it in m_currentItemSprite so we don't have to find it every frame)
	m_spriteBatch->Draw(m_currentItemSprite->GetShaderResourceView(), Vector2(1230, 10), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // game info
																																					//m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(25, 300), Color(0.0f, 1.0f, 0.0f)); // Health bar 

	if (m_players[0]->getHitStatus()) // If player is hit, flash fullscreen transparent red sprite.
	{
		m_spriteBatch->Draw(m_flashRed->GetShaderResourceView(), Vector2(0, 0), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1);
		m_players[0]->setHitStatus(false);
	}
	if (m_players[0]->getHealth() <= 0)
	{
		m_spriteBatch->Draw(m_flashRed->GetShaderResourceView(), Vector2(0, 0), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1);
		m_arialFont18->DrawString(m_spriteBatch, L"GAME OVER!", Vector2(640, 340), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
		
		m_engineSound = m_audio->Play("Assets/Sounds/Game_Over_sound_effect.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
		m_stateMachine->ChangeState(GameStates::PAUSE_STATE);

		
		//m_engineSound->Stop();
	}
	for (int i = 25; i < m_players[0]->getHealth() * 4; i += 40)
	{
		m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(i, 130), NULL, Color(0.5f, 1.0, 0.0f), 0.0f, Vector3(0, 0, 0), 1); // Health bar
	}

	if (m_players[0]->getProtection())
	{
		for (int i = 25; i < m_players[0]->getProtection() * 4; i += 40)
		{
			m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(i, 170), NULL, Color(0.854902, 0.647059, 0.12549), 0.0f, Vector3(0, 0, 0), 1); // Health bar
		}
	}
	//std::strcat("x", m_players[0]->AmmoLeft());
	char msgbuf[100];
	sprintf_s(msgbuf, "x %d", m_players[0]->AmmoLeft());

	m_spriteBatch->Draw(m_ammo->GetShaderResourceView(), Vector2(25, 75), NULL, Color(1.0f, 1.0, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // Ammo Left

	EndUI();
}

void Game::DrawPauseUI()
{
	BeginUI();
	if (m_players[0]->getHealth() > 0)
	{
		m_arialFont18->DrawString(m_spriteBatch, L"Paused", Vector2(605, 25), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	}
	EndUI();
}

void Game::BeginUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
}

void Game::EndUI()
{
	m_spriteBatch->End();
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Let's draw some text over our game
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(1145, 145), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_ammoLeft2.c_str(), Vector2(45, 115), Color(0.0f, 1.0f, 0.0f), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_rubiesCollected.c_str(), Vector2(20, 20), Color(0.627451, 0.12549, 0.941176), 0, Vector2(0, 0), m_currentCam->GetAspectRatio(), DirectX::SpriteEffects_None, 0.0f);
	m_arialFont18->DrawString(m_spriteBatch, m_ammoLeft.c_str(), Vector2(20, 53), Color(1, 0, 0), 0, Vector2(0, 0), 1, DirectX::SpriteEffects_None, 0.0f);
	//m_arialFont18->DrawString(m_spriteBatch, m_healthRemaining.c_str(), Vector2(20, 80), Color(0.0f, 1.0f, 0.0f), 0, Vector2(0, 0));
	// Here's how we draw a sprite over our game (caching it in m_currentItemSprite so we don't have to find it every frame)
	m_spriteBatch->Draw(m_currentItemSprite->GetShaderResourceView(), Vector2(1150, 10), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // game info
																																					//m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(25, 300), Color(0.0f, 1.0f, 0.0f)); // Health bar 

	if (m_players[0]->getHitStatus()) // If player is hit, flash fullscreen transparent red sprite.
	{
		m_spriteBatch->Draw(m_flashRed->GetShaderResourceView(), Vector2(0, 0), NULL, Color(1.0f, 1.0f, 1.0f), 0.0f, Vector3(0, 0, 0), 1);
		m_players[0]->setHitStatus(false);
	}

	for (int i = 25; i < m_players[0]->getHealth() * 4; i += 40)
	{
		m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(i, 130), NULL, Color(0.5f, 1.0, 0.0f), 0.0f, Vector3(0, 0, 0), m_currentCam->GetAspectRatio()); // Health bar
	}

	//std::strcat("x", m_players[0]->AmmoLeft());
	char msgbuf[100];
	sprintf_s(msgbuf, "x %d", m_players[0]->AmmoLeft());

	m_spriteBatch->Draw(m_ammo->GetShaderResourceView(), Vector2(25, 70), NULL, Color(1.0f, 1.0, 1.0f), 0.0f, Vector3(0, 0, 0), 1); // Ammo Left
	m_spriteBatch->End();


}

void Game::Shutdown()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	for (unsigned int i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
	}

	m_gameObjects.clear();
	m_rubies.clear();
	m_ammoBlocks.clear();
	m_bullets.clear();
	m_players.clear();
	m_enemies.clear();
	m_grenades.clear();
	m_addons.clear();
	m_ammoBlocks1.clear();
	m_ammoBlocks1.clear();
	m_ammoBlocks3.clear();
	m_enemies1.clear();
	m_enemies2.clear();
	m_enemies3.clear();
	m_itemBoxes.clear();
	boxes1.clear();
	boxes2.clear();
	boxes3.clear();
	
	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_collisionManager)
	{
		delete m_collisionManager;
		m_collisionManager = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_unlitTexturedShader)
	{
		m_unlitTexturedShader->Release();
		delete m_unlitTexturedShader;
		m_unlitTexturedShader = NULL;
	}

	if (m_diffuseTexturedShader)
	{
		m_diffuseTexturedShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_diffuseTexturedFogShader)
	{
		m_diffuseTexturedFogShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	if (m_stateMachine)
	{
		delete m_stateMachine;
		m_stateMachine = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_arialFont12)
	{
		delete m_arialFont12;
		m_arialFont12 = NULL;
	}

	if (m_arialFont18)
	{
		delete m_arialFont18;
		m_arialFont18 = NULL;
	}

	if (m_audio)
	{
		delete m_audio;
		m_audio = NULL;
	}

	if (m_engineSound)
	{
		delete m_engineSound;
		m_engineSound = NULL;
	}
}