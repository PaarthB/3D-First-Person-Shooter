/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
*
*	Describes a Item Box class which represents different types of items/objects.
*/
#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "GameObject.h"

class ItemBox : public GameObject
{
private:
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	int type; // 0: Ammunition Block, 1: Bullet, 2: Ruby
	int shot; // Used for bullets to check if they have been shot or not
	int owner; // Used for bullets to see who shot the bullet
	float moveSpeed = 0; // Used for time warping of bullets
	float cooldown = 200; // Used for ammunition blocks to move to random location and random times. This is the initialisation
	Vector3 direction; // Directon the bullet will travel in
	Vector3 firePosition; // The position the bullet was fired from

public:
	ItemBox(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

	void Update(float timestep);

	//void Update(float timestep, Camera* cam, Direct3D* renderer);

	CBoundingBox GetBounds() { return m_boundingBox; }
	void OnEnemyCollisionEnter();
	void OnEnemyCollisionStay();
	void OnEnemyCollisionExit();

	void OnPlayerCollisionEnter();
	void OnPlayerCollisionStay();
	void OnPlayerCollisionExit();

	void SetType(int t) { type = t; }
	void SetDirection(Vector3 dir) { direction = dir; }
	int GetType() { return type; }
	void setShot(int i) { shot = i; }
	int getShot() { return shot; }
	int getOwner() { return owner; }
	void setOwner(int i) { owner = i; }
	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setFirePosition(Vector3 pos) { firePosition = pos; }
	void setMovespeed(float speed) { moveSpeed = speed; }

};

#endif