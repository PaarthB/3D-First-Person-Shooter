/*	FIT2096 - Example Code
*	FlyingCamera.h
*	Created by Mike Yeates - 2016 - Monash University
*	A flying camera can move up and down, look around freely with
*	the mouse, and always moves parallel to the ground
*/

#ifndef FLYING_CAMERA_H
#define FLYING_CAMERA_H

#include "Camera.h"
#include "InputController.h"

class FlyingCamera : public Camera
{
private:
	InputController* m_input;

	float m_moveSpeed;
	float m_rotationSpeed;
	float m_heightChangeSpeed;

	float m_heading;
	float m_pitch;

public:
	FlyingCamera(InputController* input, Vector3 startPos);

	void Update(float timestep);
};

#endif