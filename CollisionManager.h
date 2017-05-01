#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "ItemBox.h"
#include "PersonObject.h"

#define MAX_ALLOWED_COLLISIONS 2048

class CollisionManager
{
private:
	//std::vector<Kart*>* m_karts;
	std::vector<ItemBox*>* m_rubies;
	std::vector<ItemBox*>* m_ammo;
	std::vector<ItemBox*>* m_bullets;
	std::vector<PersonObject*>*  m_players;
	std::vector<PersonObject*>*  m_enemies;

	GameObject* m_currentCollisions[MAX_ALLOWED_COLLISIONS];

	// We need to know what objects were colliding last frame so we can determine if a collision has just begun or ended
	GameObject* m_previousCollisions[MAX_ALLOWED_COLLISIONS];

	int m_nextCurrentCollisionSlot;

	// Check if we already know about two objects colliding
	bool ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second);

	// Register that a collision has occurred
	void AddCollision(GameObject* first, GameObject* second);

	// Collision check helpers
	void PlayerToBlock();
	void PlayerToRuby();
	void PlayerToBullet();
	void EnemyToBullet();

public:
	CollisionManager(std::vector<ItemBox*>* rubies, std::vector<ItemBox*>* ammo, std::vector<ItemBox*>* bullets, 
		std::vector<PersonObject*>* enemies, std::vector<PersonObject*>* players);
	void CheckCollisions();

};

#endif