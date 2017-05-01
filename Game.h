/*	Game.h
*	This class is the heart of the game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "GameObject.h"
#include "ItemBox.h"
#include "PersonObject.h"
#include <array>
#include <sstream>
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

#include <vector>

class Game
{
private:
	PersonObject* m_player;
	FirstPersonCamera* m_currentCam;		
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	CollisionManager* m_collisionManager;
	//ItemBox* bullet;

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;

	// Shaders
	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;
	Shader* m_diffuseTexturedShader;
	Shader* m_diffuseTexturedFogShader;
	float x, y, z;
	std::wstring m_rubiesCollected; // rubies collected
	std::wstring m_ammoLeft; // Ammo remaining
	std::wstring m_healthRemaining; // Player health
	bool gotHit = false; // To see if the player has been hit or not


	//Kart* m_player;

	// This contains everything for easy calls to update and render
	std::vector<GameObject*> m_gameObjects;
	std::vector<PersonObject*> m_players;
	// We also need more specific collections for easier collision checks
	//std::vector<Kart*> m_karts;
	std::vector<ItemBox*> m_rubies; // Ruby pool
	std::vector<PersonObject*> m_enemies; // Enemy pool
	std::vector<ItemBox*> m_ammoBlocks; // Ammo pool
	std::vector<ItemBox*> m_bullets; // Bullet pool

	Texture* m_currentItemSprite; // game info
	Texture* m_flashRed; // hit flash
	Texture* m_health; // current health
	std::string str[5] = { "gradient_red.png", "gradient_redDarker.png", "gradient_redPink.png", "gradient_redOrange.png",
		"gradient_redLighter.png"};
	std::array <int, 5> arr = { 1, 2, 3, 4, 5};

	// Initialisation Helpers
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void LoadFonts();
	void InitGameWorld();
	void InitEnemies();
	void InitItems();

	void InitUI();
	void DrawUI();
	void RefreshUI();

public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised
};

#endif