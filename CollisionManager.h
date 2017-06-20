#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "ItemBox.h"
#include "PersonObject.h"
#include "Grenade.h"
#include "ItemObject.h"
#include "Teleporter.h"
#include "FirstPersonCamera.h"

#define MAX_ALLOWED_COLLISIONS 2048

class CollisionManager
{
private:
	//std::vector<Kart*>* m_karts;
	std::vector<ItemObject*>* m_rubies;
	std::vector<ItemObject*>* m_ammo;
	std::vector<ItemObject*>* m_addons;
	std::vector<ItemBox*>* m_boxes;
	std::vector<Grenade*>* m_grenades;
	std::vector<Bullet*>* m_bullets;
	std::vector<Teleporter*>* teleporters;
	std::vector<PersonObject*>* m_players;
	std::vector<PersonObject*>* m_enemies;
	FirstPersonCamera* m_cam;
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
	void PlayerToAddon();
	void PlayerToTeleporter();
	void EnemyToBullet();
	void GrenadeToBox();
	void GrenadeToEnemy();
	void BulletToBox();

public:
	CollisionManager(std::vector<ItemObject*>* rubies, std::vector<ItemObject*>* ammo, std::vector<Bullet*>* bullets,
		std::vector<PersonObject*>* enemies, std::vector<PersonObject*>* players, std::vector<ItemObject*>* addons,
		std::vector<Teleporter*>* tel, FirstPersonCamera* cam, std::vector<Grenade*>* grenades, std::vector<ItemBox*>* boxes);
	void CheckCollisions();

};

#endif