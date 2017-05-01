/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
*
*	First-person camera class for the player. Forms the essence of a first person shooter.
*/
#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "ItemBox.h"
#include "InputController.h"
#include "MathsHelper.h"
#include "PersonObject.h"
#include <vector>

class FirstPersonCamera : public Camera
{
private:
	//Vector3 localForward;
	Vector3 m_offset;
	PersonObject* m_player;
	std::vector<ItemBox*> bullets;
	InputController* m_input;
	float m_heading;
	float m_pitch;
	bool m_catchupMode;
	float m_catchupSpeed;
	float m_turnSpeed;
	float m_moveSpeed;
	//CBoundingBox m_boundingBox;
	Vector3 max;
	Vector3 min;
	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading;
	Vector3 TransformOffset();

public:
	FirstPersonCamera();
	FirstPersonCamera(InputController* input, Vector3 startPos);
	Vector3 GetMax() { return max; };
	Vector3 GetMin() { return min; };
	Vector3 GetLocalForward() { return localForward; }
	void SetMax(Vector3 new_max) { max = new_max; }
	void SetMin(Vector3 new_min) { max = new_min; }
	void Update(float timestep);
	CBoundingBox GetBounds() { return m_boundingBox; }
	void AddBullet(ItemBox* bullet) { bullets.push_back(bullet); }
	std::vector<ItemBox*> GetBullets() { return bullets; }
	void setPlayer(PersonObject* player) { m_player = player; }
};
#endif