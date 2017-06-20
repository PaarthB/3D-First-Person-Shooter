/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a Item Object class which represents rubies, ammo blocks and armors.
*/
#ifndef ITEM_OBJECT_H
#define ITEM_OBJECT_H

#include "GameObject.h"
class ItemObject : public GameObject
{
private:
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	int type; // 0: Ammunition Block, 1: Armor, 2: Ruby
	float cooldown = 200; // Used for ammunition blocks to move to random location and random times. This is the initialisation
	int level;

public:
	ItemObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

	void Update(float timestep);

	//void Update(float timestep, Camera* cam, Direct3D* renderer);

	CBoundingBox GetBounds() { return m_boundingBox; }
	void SetBounds(CBoundingBox bounds) { m_boundingBox = bounds; }
	void OnEnemyCollisionEnter();
	void OnEnemyCollisionStay();
	void OnEnemyCollisionExit();

	void OnPlayerCollisionEnter();
	void OnPlayerCollisionStay();
	void OnPlayerCollisionExit();

	void SetType(int t) { type = t; }
	int GetType() { return type; }
	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setLevel(int lev) { level = lev; }
	int getLevel() { return level; }
};

#endif