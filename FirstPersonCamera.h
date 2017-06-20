/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	First-person camera class for the player. Forms the essence of a first person shooter.
*/
#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Camera.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Teleporter.h"
#include "InputController.h"
#include "MathsHelper.h"
#include "AudioClip.h"
#include "AudioSystem.h"
#include "PersonObject.h"
#include <vector>

class FirstPersonCamera : public Camera
{
private:
	//Vector3 localForward;
	Vector3 m_offset;
	PersonObject* m_player;
	std::vector<Bullet*> bullets; // All player bullets
	std::vector<Grenade*> grenades; // All player grenades
	InputController* m_input;
	float m_heading;
	float m_pitch;
	bool m_catchupMode;
	bool sound_effects = true; // Flag for game sound effects
	float m_catchupSpeed;
	float m_turnSpeed;
	float m_moveSpeed;
	//CBoundingBox m_boundingBox;
	Grenade* grenade; // Grenade handled by the player
	Vector3 max;
	Vector3 min;
	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading;
	Vector3 TransformOffset();
	AudioSystem* m_audio;
	AudioClip* m_engineSound;

public:
	FirstPersonCamera();
	FirstPersonCamera(InputController* input, Vector3 startPos, AudioSystem* audio);
	Vector3 GetMax() { return max; };
	Vector3 GetMin() { return min; };
	Vector3 GetLocalForward() { return localForward; }
	void SetMax(Vector3 new_max) { max = new_max; }
	void SetMin(Vector3 new_min) { max = new_min; }
	void Update(float timestep);
	CBoundingBox GetBounds() { return m_boundingBox; }
	void AddBullet(Bullet* bullet) { bullets.push_back(bullet); }
	void AddGrenade(Grenade* grenade) { grenades.push_back(grenade); }
	std::vector<Bullet*> GetBullets() { return bullets; }
	void setPlayer(PersonObject* player) { m_player = player; }
	void OnTeleportationEnter(Teleporter* other);
	void OnTeleportationStay(Teleporter* other);
	void OnTeleportationExit(Teleporter* other);
	void setSoundEffects(bool s) { sound_effects = s; }
	void setGrenade(Grenade* g) { grenade = g; }
};
#endif