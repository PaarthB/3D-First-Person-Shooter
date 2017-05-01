/*#ifndef PLAYER_H
#define PLAYER_H

#include "ItemBox.h"

class Player : public GameObject
{
protected:
	CBoundingBox m_boundingBox;

	float m_moveSpeed;
	float m_turnSpeed;
	
public:
	Player(Vector3 position);
	virtual void Update(float timestep, Vector3 position);
	virtual void Update(float timestep);
	void OnItemCollisionEnter(ItemBox* other);
	void OnItemCollisionStay(ItemBox* other);
	void OnItemCollisionExit(ItemBox* other);

	void OnBulletCollisionEnter(ItemBox* other);
	void OnBulletCollisionStay(ItemBox* other);
	void OnBulletCollisionExit(ItemBox* other);

	CBoundingBox GetBounds() { return m_boundingBox; }
};

#endif
*/