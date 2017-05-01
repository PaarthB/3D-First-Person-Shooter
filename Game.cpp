/*
*	Game.cpp
*	Implementation of Game.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
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
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_diffuseTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
	//m_player = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{	
	m_renderer = renderer;	
	m_input = input;
	m_currentCam = new FirstPersonCamera(m_input, Vector3(0, 2, 0)); // Our first person camera
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
	InitGameWorld();
	RefreshUI();

	// Our collision manager managing collisions of all game objects separately.
	m_collisionManager = new CollisionManager(&m_rubies, &m_ammoBlocks, &m_bullets, &m_enemies, &m_players);

	//m_currentCam = new FlyingCamera(m_input, Vector3(0, 10, -20));

	//m_currentCam = new ThirdPersonCamera(m_player, Vector3(0, 10, -25), true, 2.0f);
	
	
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

	m_diffuseTexturedShader = new TexturedShader();
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader();
	if (!m_diffuseTexturedFogShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedFogPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	// Loading the Meshes
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ammoBlock.obj"))
		return false;
	return true;
}

bool Game::LoadTextures()
{
	// Loading the textures
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

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
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
	ss << "Rubies Collected: " << m_players[0]->RubiesCollected() << "/10";
	m_rubiesCollected = ss.str();

	std::wstringstream s1;
	s1 << "Ammunition: " << m_players[0]->AmmoLeft() << "/15";
	m_ammoLeft = s1.str();

	std::wstringstream s2;
	s2 << "Health: " << m_players[0]->getHealth() << "/100";;
	m_healthRemaining = s2.str();
}

void Game::InitGameWorld()
{	
	// Static scenery objects
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png")));

	// A player (invisible with no texture or mesh) object to attach to the First-person camera
	m_players.push_back(new PersonObject(m_currentCam->GetPosition()));
	m_players[0]->setType(0);
	m_currentCam->setPlayer(m_players[0]);

	// Creating the bullets, rubies, ammunition blocks and enemies.
	InitItems();
	InitEnemies();
}

void Game::InitEnemies()
{
	std::string base = "Assets/Textures/";
	for (int i = 0; i < 5; i++)
	{
		std::string tex = base + str[i];
		const char* charTex = tex.c_str();
		Vector3 position = Vector3(MathsHelper::RandomRange(-150.0f, 150.0f), 0.0f, MathsHelper::RandomRange(-150.0f, 150.0f));
		PersonObject* enemy = new PersonObject(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture(charTex),
			position);
	
		enemy->setType(i + 1); // Enemy types: 1, 2, 3, 4, 5
		if (i == 2)
			enemy->setRubies(&m_rubies);
		ItemBox* bullet = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/bullet.png"),
			enemy->GetPosition() + Vector3(0, -4, 0));

		bullet->SetType(1); // It is bullet object
		bullet->setOwner(1); // Enemy owns it
		bullet->setShot(0); // It has not been shot

		enemy->AddBullet(bullet); // Enemy has been allocated the bullet
		m_bullets.push_back(bullet); // Adding enemy bullet to the bullet pool
		m_enemies.push_back(enemy); // Adding enemy to the enemy pool

		// Initialising the current and previous positions of player in enemies and player to create time warp funtionality.
		m_enemies[i]->setPrevPos(m_players[0]->GetPosition());
		m_enemies[i]->setCurrPos(m_players[0]->GetPosition());
		m_players[0]->setPrevPos(m_players[0]->GetPosition());
		m_players[0]->setCurrPos(m_players[0]->GetPosition());

		m_gameObjects.push_back(enemy); // Adding enemy to gameObjects
		m_gameObjects.push_back(bullet); // Adding bullet to gameObjects
	}

	Vector3 position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
	int v = rand() % 9;
	m_enemies[1]->setRandomPosition(position); // Setting the first random point the 2nd enemy will head to
	m_enemies[2]->setRandomPosition(m_rubies[v]->GetPosition()); // Setting the first random ruby the 3rd enemy will head to.
}

void Game::InitItems()
{
	for (int i = 0; i < 10; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));

		ItemBox* ruby = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png"),
			position);
		ruby->SetType(2); // Type 0: Ammo, Type 1: Bullet, Type 2: Ruby
		
		m_gameObjects.push_back(ruby); // Putting the ruby in  gameObjects
		m_rubies.push_back(ruby); // Putting the ruby to the ruby pool
	}

	for (int i = 0; i < 2; i++)
	{
		// Generating random position for the ammunition block
		Vector3 position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));

		ItemBox* ammoBlock = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/gradient_redDarker.png"),
			position);
		ammoBlock->SetType(0); // Type 0: Ammo, Type 1: Bullet, Type 2: Ruby

		m_gameObjects.push_back(ammoBlock); // Putting the ammo in  gameObjects
		m_ammoBlocks.push_back(ammoBlock); // Putting the ammo to the ammo pool
		//m_items.push_back(itemBox);
	}

	// Bullet for the player
	ItemBox* bullet = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/bullet.png"),
		m_currentCam->GetPosition() + Vector3(0, -4, 0));

	bullet->setShot(0); // It has not been shot
	bullet->setOwner(0); // Player owns this
	bullet->SetType(1); // It is a bullet object
	bullet->SetStatus(false); // It will not be rendered

	m_bullets.push_back(bullet); // Adding the player bullet to the bullet pool
	m_currentCam->AddBullet(bullet); // Adding the bullet to the player
	m_gameObjects.push_back(bullet); // Adding the bullet to the gameObjects
		
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}
	
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep);
	}

	
	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->setPlayerPosition(m_players[0]->GetPosition()); // Position of our player so that the enemy can turn
		
		// To calculate how fast the player is moving
		m_enemies[i]->setPrevPos(m_enemies[i]->getCurrPos()); // The previous position of the player
		m_enemies[i]->setCurrPos(m_players[0]->GetPosition()); // The current position of the player


		if (m_enemies[i]->getBullet()->getShot() == 0) // If the bullet is not shot, bring it back to the enemy
		{
			m_enemies[i]->getBullet()->SetPosition(m_enemies[i]->GetPosition() + Vector3(0, -4, 0));
		}
		if (m_enemies[i]->getHealth() < 0) // If enemy dies, take the enemy to some other random location and replenish health
		{
			m_enemies[i]->SetPosition(Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f)));
			m_enemies[i]->setHealth(100);
		}
	}

	m_currentCam->Update(timestep);

	// Setting current and previous position to calculate time warp.
	m_players[0]->setPrevPos(m_players[0]->getCurrPos());
	m_players[0]->setCurrPos(m_players[0]->GetPosition());

	//m_players[0]->Update(timestep, m_currentCam->GetPosition());

	m_collisionManager->CheckCollisions();

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.8f, 1.0f, 0.9f, 1.0f);

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}

	RefreshUI();
	DrawUI();
	m_renderer->EndScene();		
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Let's draw some text over our game
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(20, 160), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0,0));
	m_arialFont18->DrawString(m_spriteBatch, m_rubiesCollected.c_str(), Vector2(20, 200), Color(0.627451, 0.12549, 0.941176), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, m_ammoLeft.c_str(), Vector2(20, 230), Color(1, 0, 0), 0, Vector2(0, 0));
	m_arialFont18->DrawString(m_spriteBatch, m_healthRemaining.c_str(), Vector2(20, 260), Color(0.0f, 1.0f, 0.0f), 0, Vector2(0, 0));

	// Here's how we draw a sprite over our game (caching it in m_currentItemSprite so we don't have to find it every frame)
	m_spriteBatch->Draw(m_currentItemSprite->GetShaderResourceView(), Vector2(20, 20), Color(1.0f, 1.0f, 1.0f)); // game info
	m_spriteBatch->Draw(m_health->GetShaderResourceView(), Vector2(25, 300), Color(0.0f, 1.0f, 0.0f)); // Health bar 

	if (m_players[0]->getHitStatus()) // If player is hit, flash fullscreen transparent red sprite.
	{
		m_spriteBatch->Draw(m_flashRed->GetShaderResourceView(), Vector2(0, 0), Color(0.0f, 1.0f, 0.0f));
		m_players[0]->setHitStatus(false);
	}
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

	if(m_diffuseTexturedShader)
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
}