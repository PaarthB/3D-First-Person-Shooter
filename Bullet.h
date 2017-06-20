/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a Bullet class which represents bullets used by enemies and players
*/
#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
class Bullet : public GameObject
{
private:
	Vector3 m_pos;
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	int shot; // Used for bullets to check if they have been shot or not
	int owner; // Used for bullets to see who shot the bullet
	float moveSpeed = 0; // Used for time warping of bullets
	float cooldown = 200; // This is the initialisation
	Vector3 direction; // Directon the bullet will travel in
	Vector3 firePosition; // The position the bullet was fired from
	int level; // Floor at which it fires
	bool holdFire = false; // For paralysis and invisibility cases

public:
	Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

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

	void OnBoxCollisionEnter();
	void OnBoxCollisionStay();
	void OnBoxCollisionExit();


	void SetDirection(Vector3 dir) { direction = dir; }
	void setShot(int i) { shot = i; }
	int getShot() { return shot; }
	int getOwner() { return owner; }
	void setOwner(int i) { owner = i; }
	void setHold(bool i) { holdFire = i; }
	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setFirePosition(Vector3 pos) { firePosition = pos; }
	void setMovespeed(float speed) { moveSpeed = speed; }
	void setPlayerPos(Vector3 Pos) { m_pos = Pos; }
	void setLevel(int lev) { level = lev; }
	int getLevel() { return level; }
};

#endif