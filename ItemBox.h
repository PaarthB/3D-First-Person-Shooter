/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a Item Box class which represents different types of items/objects.
*/
#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "GameObject.h"
class ItemBox : public GameObject
{
private:
	Vector3 m_pos;
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	int level; // Floor at which it is situated
	int teleporter; // The ID of the teleporter it is guarding

public:
	ItemBox(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

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

	void OnGrenadeCollisionEnter();
	void OnGrenadeCollisionStay();
	void OnGrenadeCollisionExit();

	void OnBulletCollisionEnter();
	void OnBulletCollisionStay();
	void OnBulletCollisionExit();

	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setLevel(int lev) { level = lev; }
	int getLevel() { return level; }
	void setTeleporter(int i) { teleporter = i; }
	int getTeleporter() { return teleporter; }
};

#endif