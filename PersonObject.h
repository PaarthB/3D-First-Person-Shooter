/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a general person which could either be a player or an enemy.
*/
#ifndef PERSON_OBJECT_H
#define PERSON_OBJECT_H

#include "ItemObject.h"
#include "Bullet.h"
#include "AudioSystem.h"
#include "AudioClip.h"
#include <vector>

class PersonObject : public GameObject
{
private:
	CBoundingBox m_boundingBox; // Bounding box of the person
	int type; // Person ID (0: Player, 1: Enemy-1, 2: Enemy-2, 3: Enemy-3, 4: Enemy-4, 5: Enemy-5
	float rotation; // Rotation of the person if we want it to face somewhere specific
	Vector3 player_position; // So that the 1st enemy can chase the player
	float health = 100; // Starting health of every person, this can change
	float armor = 0;
	Vector3 prevPos; // To implement time warp
	Vector3 currPos; // To implement time warp
	bool gotHit = false; // Check whether player has been hit or not
	bool protect = false;
	int treasure = 3;
	int score = 0;
	bool infinite = false;
	float speed; // Speed of the player
	float invisibility_cooldown = 0;
	float paralysis_cooldown = 0;
	Vector3 offset = Vector3(-0.133, 1.2, 0.137); // Offset to spawn bullet at end of gun for enemies
	int rubies_collected = 0; // Number of rubies that the player has collected
	int ammo_remaining = 15; // Starting ammunition of all players. This reduces as we use ammo. Enemies have infinite ammo.
	float cooldown = 0; // Cooldown timer to be able to shoot again
	Vector3 random_point; // The random position Enemy 2 will head towards
	std::vector<Bullet*> bullet; // Bullets of the person
	std::vector<ItemObject*>* rubies; // The list of rubies that Enemy 3 will randomly head towards.
	AudioSystem* m_audio;
	AudioClip* m_engineSound;
	int t1 = 0;
	int t2 = 0;
	int t3 = 0;
	int t4 = 0;
	bool stealth = false; // Invisibility
	bool sound_effects = true;
	int paralysis = 1; // 1: Don't paralyse, 0: paralyse
	int level; // Floor at which it is situated
	int lives = 2; // Each enemy has two lives

public:
	PersonObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position); // For enemies
	PersonObject(Vector3 position, AudioSystem* audio); // For player (invisible)
	Vector3 TransformOffset(); // To transform the bullet spawning offset, if the enemy rotates
	void Update(float timestep, Vector3 position); // For player
	void Update(float timestep); // General
	void Update1(float timestep); // For enemy 1
	void Update2(float timestep); // For enemy 2
	void Update3(float timestep); // Fpr enemy 3
	void Update4(float timestep); // For enemy 4
	void Update5(float timestep); // For enemy 5
	float RotateToFace(Vector3 target, Vector3 origin); // To rotate enemies towards the player
	void incT1() { t1++; }
	void incT2() { t2++; }
	void incT3() { t3++; }
	void incT4() { t4++; }
	int getT1() { return t1; }
	int getT2() { return t2; }
	int getT3() { return t3; }
	int getT4() { return t4; }
	CBoundingBox GetBounds() { return m_boundingBox; }

	// How the person reacts in different stages of the collision with differet objects
	void OnBulletCollisionEnter();
	void OnBulletCollisionStay();
	void OnBulletCollisionExit();

	void OnGrenadeCollisionEnter();
	void OnGrenadeCollisionStay();
	void OnGrenadeCollisionExit();

	void OnItemCollisionStay(ItemObject* other);
	void OnItemCollisionEnter(ItemObject* other);
	void OnItemCollisionExit(ItemObject* other);
	void Explosion();
	void setType(int t) { type = t; }  // Setting player ID (explained above)
	void setPlayerPosition(Vector3 pos) { player_position = pos; } // Setting player position for enemies
	int getType() { return type; } // Returns person ID (explained above)
	float getHealth() { return health; } // Returns the health remaining
	float getProtection() { return armor; } // Amount of armor left
	void setHealth(int h) { health = h; } // Resetting health of enemy if they die
	Bullet* getBullet() { return bullet[0]; } // Returning the bullet for usage
	int RubiesCollected() { return rubies_collected; } // Returns number of rubies the player has collected
	void setRandomPosition(Vector3 pos) { random_point = pos; } // Setting random position for enemy 2 
	int AmmoLeft() { return ammo_remaining; } // Returning ammo remaining
	void SetAmmo(int ammo) { ammo_remaining = ammo; } // Resetting the ammo
	void AddBullet(Bullet* b) { bullet.push_back(b); } // Adding bullet to the arsenal
	void setRubies(std::vector<ItemObject*>* r) { rubies = r; } // Setting pointer to ruby pool for enemy 3
	void setPrevPos(Vector3 prev) { prevPos = prev; } // Setting prev position
	void setCurrPos(Vector3 curr) { currPos = curr; } // Setting curr position
	void setHitStatus(bool status) { gotHit = status; } // Setting the hit status
	bool getHitStatus() { return gotHit; } // To check whether the player has been hit
	bool getProtected() { return protect; } // To check whether the player is wearing an armor or not
	Vector3 getPrevPos() { return prevPos; } // Returning previous position
	Vector3 getCurrPos() { return currPos; } // Returing current position
	void setSoundEffects(bool s) { sound_effects = s; }
	int getTreasures() { return treasure; } // treasures left
	void setStealth(bool s) { stealth = s; }
	int getParalyse() { return paralysis; }
	bool getStealth() { return stealth; }
	void paralyse(int t) { paralysis = t; }
	int getScore() { return score; }
	void decLives() { lives--; }
	int getLives() { return lives; }
	void setLevel(int i) { level = i; }
	int getLevel() { return level; }
	void FreeRoam(bool t) { infinite = t; }
};


#endif
