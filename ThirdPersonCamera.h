/*	FIT2096 - Example Code
*	ThirdPersonCamera.h
*	Created by Mike Yeates - 2016 - Monash University
*	A third person camera follows along behind a gameobject
*/

#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "Camera.h"
#include "GameObject.h"

class ThirdPersonCamera : public Camera
{
private:
	GameObject* m_objectToFollow;
	Vector3 m_offset;

	bool m_catchupMode;
	float m_catchupSpeed;

	Vector3 TransformOffset();

public:
	ThirdPersonCamera();
	ThirdPersonCamera(GameObject* target, Vector3 offset, bool catchup, float catchupSpeed);

	void Update(float timestep);
};

#endif