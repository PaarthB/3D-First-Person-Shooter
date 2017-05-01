/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
*	
*	Describes a general person which could either be a player or an enemy.
*/
#ifndef PERSON_OBJECT_H
#define PERSON_OBJECT_H

#include "ItemBox.h"
#include <vector>

class PersonObject : public GameObject
{
private:
	CBoundingBox m_boundingBox; // Bounding box of the person
	int type; // Person ID (0: Player, 1: Enemy-1, 2: Enemy-2, 3: Enemy-3, 4: Enemy-4, 5: Enemy-5
	float rotation; // Rotation of the person if we want it to face somewhere specific
	Vector3 player_position; // So that the 1st enemy can chase the player
	float health = 100; // Starting health of every person, this can change
	Vector3 prevPos; // To implement time warp
	Vector3 currPos; // To implement time warp
	bool gotHit = false; // Check whether player has been hit or not
	float speed; // Speed of the player
	Vector3 offset = Vector3(-0.133, 1.2, 0.137); // Offset to spawn bullet at end of gun for enemies
	int rubies_collected = 0; // Number of rubies that the player has collected
	int ammo_remaining = 15; // Starting ammunition of all players. This reduces as we use ammo. Enemies have infinite ammo.
	float cooldown = 0; // Cooldown timer to be able to shoot again
	Vector3 random_point; // The random position Enemy 2 will head towards
	std::vector<ItemBox*> bullet; // Bullets of the person
	std::vector<ItemBox*>* rubies; // The list of rubies that Enemy 3 will randomly head towards.

public:
	PersonObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position); // For enemies
	PersonObject(Vector3 position); // For player (invisible)
	Vector3 TransformOffset(); // To transform the bullet spawning offset, if the enemy rotates
	void Update(float timestep, Vector3 position); // For player
	void Update(float timestep); // General
	void Update1(float timestep); // For enemy 1
	void Update2(float timestep); // For enemy 2
	void Update3(float timestep); // Fpr enemy 3
	void Update4(float timestep); // For enemy 4
	void Update5(float timestep); // For enemy 5
	float RotateToFace(Vector3 target, Vector3 origin); // To rotate enemies towards the player

	CBoundingBox GetBounds() { return m_boundingBox; }

	// How the person reacts in different stages of the collision with differet objects
	void OnBulletCollisionEnter(ItemBox* other);
	void OnBulletCollisionStay(ItemBox* other);
	void OnBulletCollisionExit(ItemBox* other);
	void OnItemCollisionStay(ItemBox* other);
	void OnItemCollisionEnter(ItemBox* other);
	void OnItemCollisionExit(ItemBox* other);

	void setType(int t) { type = t; }  // Setting player ID (explained above)
	void setPlayerPosition(Vector3 pos) { player_position = pos; } // Setting player position for enemies
	int getType() { return type; } // Returns person ID (explained above)
	float getHealth() { return health; } // Returns the health remaining
	void setHealth(int h) { health = h; } // Resetting health of enemy if they die
	ItemBox* getBullet() { return bullet[0]; } // Returning the bullet for usage
	int RubiesCollected() { return rubies_collected; } // Returns number of rubies the player has collected
	void setRandomPosition(Vector3 pos) { random_point = pos; } // Setting random position for enemy 2 
	int AmmoLeft() { return ammo_remaining; } // Returning ammo remaining
	void SetAmmo(int ammo) { ammo_remaining = ammo; } // Resetting the ammo
	void AddBullet(ItemBox* b) { bullet.push_back(b); } // Adding bullet to the arsenal
	void setRubies(std::vector<ItemBox*>* r) { rubies = r; } // Setting pointer to ruby pool for enemy 3
	void setPrevPos(Vector3 prev) { prevPos = prev; } // Setting prev position
	void setCurrPos(Vector3 curr) { currPos = curr; } // Setting curr position
	void setHitStatus(bool status) { gotHit = status; } // Setting the hit status
	bool getHitStatus() { return gotHit; } // To check whether the player has been hit
	Vector3 getPrevPos() { return prevPos; } // Returning previous position
	Vector3 getCurrPos() { return currPos; } // Returing current position
};


#endif
