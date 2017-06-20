/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a Item Box class which represents different types of items/objects.
*/
#ifndef TELEPORTER_H
#define TELEPORTER_H

#include "GameObject.h"
class Teleporter : public GameObject
{
private:
	Vector3 m_pos; // Player pos
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	int level; // Which floor level does it transport to
	int ID; //4: Dragon lair, 2-3: Egypt, 1: Frosty Caves


public:
	Teleporter(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

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

	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setLevel(int lev) { level = lev; }
	int getLevel() { return level; }
	void setID(int i) { ID = i; }
	int getID() { return ID; }
};

#endif