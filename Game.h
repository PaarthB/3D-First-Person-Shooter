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
#include "AudioSystem.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "ItemBox.h"
#include "Bullet.h"
#include "Grenade.h"
#include "ItemObject.h"
#include "Teleporter.h"
#include "Button.h"
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
	AudioSystem* m_audio;
	AudioClip* m_engineSound;

	enum class GameStates
	{
		MENU_STATE,
		GAMEPLAY_STATE,
		PAUSE_STATE,
		STATE_COUNT,
		HELP_STATE,
		FREE_EXPLORE
	};


	// Our menu screen will have a "start" and "quit" button
	Button* m_startButton;
	Button* m_quitButton;
	Button* m_freeExplore;
	Button* m_resume;
	Button* m_controls;

	StateMachine<GameStates, Game>* m_stateMachine;

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
	std::wstring m_ammoLeft2; // Player health
	std::wstring m_score; // Player Score
	std::wstring m_treasure; // Treasure's left

	Grenade* grenade;

	bool gotHit = false; // To see if the player has been hit or not
	bool b_sound = true; // back ground sound
	bool no_b_sound = false;
	bool sound_effects = true; // sound effects inside game
	bool infinite = false; // true: Free Roam, false: play game

	//Kart* m_player;

	// This contains everything for easy calls to update and render
	std::vector<GameObject*> m_gameObjects;
	std::vector<PersonObject*> m_players;
	// We also need more specific collections for easier collision checks
	//std::vector<Kart*> m_karts;
	std::vector<ItemObject*> m_rubies; // Ruby pool
	std::vector<ItemBox*> m_itemBoxes;
	std::vector<ItemObject*> m_ammoBlocks; // Ruby pool
	std::vector<ItemObject*> m_rubies1; // Ruby pool
	std::vector<ItemObject*> m_ammoBlocks1; // Ammo pool
	std::vector<ItemObject*> m_rubies2; // Ruby pool
	std::vector<ItemObject*> m_ammoBlocks2; // Ammo pool
	std::vector<ItemObject*> m_rubies3; // Ruby pool
	std::vector<ItemObject*> m_ammoBlocks3; // Ammo pool
	std::vector<ItemObject*> m_addons; // Addons Collection
	std::vector<PersonObject*> m_enemies; // Enemy pool
	std::vector<PersonObject*> m_enemies1; // Enemy pool
	std::vector<PersonObject*> m_enemies2; // Enemy pool
	std::vector<PersonObject*> m_enemies3; // Enemy pool
	std::vector<ItemBox*> boxes1; // Enemy pool
	std::vector<ItemBox*> boxes2; // Enemy pool
	std::vector<ItemBox*> boxes3; // Enemy pool

	std::vector<Bullet*> m_bullets; // Bullet pool
	std::vector<Grenade*> m_grenades; // Ruby pool
	std::vector<Teleporter*>  m_teleporters; // Teleporter pool


	Texture* m_currentItemSprite; // game info
	Texture* m_flashRed; // hit flash
	Texture* m_health; // current health
	Texture* m_ammo; // ammo info
	Texture* m_invisibility; // invisibility sprite
	Texture* m_paralysis; // paralysis sprite
	std::string str[5] = { "gradient_red.png", "gradient_redDarker.png", "gradient_redPink.png", "gradient_redOrange.png",
		"gradient_redLighter.png" };
	std::array <int, 5> arr = { 1, 2, 3, 4, 5 };

	// Initialisation Helpers
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void setRoam(bool t) { infinite = t; }
	void LoadFonts();
	void InitGameWorld();
	void InitEnemies();
	void InitItems();
	bool LoadAudio();
	void InitStates();

	void InitUI();
	void DrawUI();
	void RefreshUI();
	void DrawMenuUI();
	void DrawGameUI();
	void DrawPauseUI();
	void DrawHelpUI();
	void BeginUI();
	void EndUI();

	// Every state in our game will have four callbacks
	// We register these with the StateMachine and it calls them for us
	void Menu_OnEnter();
	void Menu_OnUpdate(float timestep);
	void Menu_OnRender();
	void Menu_OnExit();

	void Gameplay_OnEnter();
	void Gameplay_OnUpdate(float timestep);
	void Gameplay_OnRender();
	void Gameplay_OnExit();

	void Pause_OnEnter();
	void Pause_OnUpdate(float timestep);
	void Pause_OnRender();
	void Pause_OnExit();

	void Help_OnEnter();
	void Help_OnUpdate(float timestep);
	void Help_OnRender();
	void Help_OnExit();

	void Free_OnEnter();
	void Free_OnUpdate(float timestep);
	void Free_OnRender();
	void Free_OnExit();


public:
	enum class Levels
	{
		ICE_LAND,
		DEVILS_KINGDOM,
		NORMAL
	};
	Game();
	~Game();

	bool Initialise(Direct3D* renderer, AudioSystem* audio, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn

	void Shutdown(); //Cleanup everything we initialised
};

#endif