#include "FirstPersonCamera.h"
#include "MathsHelper.h"
#include "AudioClip.h"
#include "AudioSystem.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>

FirstPersonCamera::FirstPersonCamera()
{
	m_catchupMode = false;
	m_catchupSpeed = 1.0f;
	localForward = Vector3::Zero;
	m_offset = Vector3(0.0f, 10.0f, -10.0f);
	GetTickCount();
	SetPosition(localForward + TransformOffset());
}

FirstPersonCamera::FirstPersonCamera(InputController* input, Vector3 startPos, AudioSystem* audio)
{
	//m_catchupMode = catchup;
	//m_catchupSpeed = catchupSpeed;
	//m_player = player;
	m_input = input;
	m_turnSpeed = 2.0f;
	m_moveSpeed = 0.5f;
	m_audio = audio;
	//heading = Matrix::CreateRotationY(m_player->GetYRotation());
	worldForward = Vector3(0, 0, 1);
	localForward = Vector3::TransformNormal(worldForward, heading);
	// Set starting position so catchup mode doesn't show camera animating from the origin
	SetPosition(startPos);
	m_position = startPos;
	// 3D sounds should be paused while we configure them (second paramater is startPaused)


}

void FirstPersonCamera::Update(float timestep)
{

	//m_offset = Vector3(m_player->GetPosition().x, 3, m_player->GetPosition().z);
	//SetPosition(m_player->GetPosition());
	if (m_player->getLevel() == -1)
	{
		m_moveSpeed = 0.2f;
		m_turnSpeed = 0.2f;
	}

	if (m_player->getLevel() == 0)
	{
		m_moveSpeed = 0.3f;
		m_turnSpeed = 0.3f;
	}

	if (m_player->getLevel() == 1)
	{
		m_moveSpeed = 0.4f;
		m_turnSpeed = 0.4f;
	}
	m_heading += m_input->GetMouseDeltaX() * m_turnSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_turnSpeed * timestep;
	heading = Matrix::CreateRotationY(m_heading);
	localForward = Vector3::TransformNormal(worldForward, heading);

	// Limit how far the player can look down and up
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

	// Wrap heading and pitch up in a matrix so we can transform our look at vector
	// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
	// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
	Matrix heading_1 = Matrix::CreateRotationY(m_heading);
	Matrix pitch = Matrix::CreateRotationX(m_pitch);

	// Transform a world right vector from world space into local space
	Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), heading_1);

	// Essentially our local forward vector but always parallel with the ground
	// Remember a cross product gives us a vector perpendicular to the two input vectors
	Vector3 localForwardXZ = localRight.Cross(Vector3(0, 1, 0));

	// We're going to need this a lot. Store it locally here to save on our function calls 
	Vector3 currentPos = GetPosition();
	if (m_input->GetKeyHold('W'))
	{
		currentPos += localForward * m_moveSpeed * 0.5;
	}
	if (m_input->GetKeyHold('S'))
	{
		currentPos -= localForward * m_moveSpeed * 0.5;
	}
	if (m_input->GetKeyHold('A'))
	{
		currentPos -= localRight * m_moveSpeed * 0.5;
		//currentPos -= localRight. * m_moveSpeed;
	}
	if (m_input->GetKeyHold('D'))
	{
		currentPos += localRight * m_moveSpeed * 0.5;
		//currentPos.z += localRight.z * m_moveSpeed;
	}

	if (currentPos.x >= 48)
		currentPos.x = 48;
	if (currentPos.x <= -48)
		currentPos.x = -48;
	if (currentPos.z >= 48)
		currentPos.z = 48;
	if (currentPos.z <= -48)
		currentPos.z = -48;

	Matrix lookAtRotation = pitch * heading_1;

	// Transform a world forward vector into local space (take pitch and heading into account)
	Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

	// At this point, our look-at vector is still relative to the origin
	// Add our position to it so it originates from the camera and points slightly in front of it
	// Remember the look-at vector needs to describe a point in the world relative to the origin
	lookAt += currentPos;

	// Use parent's mutators so isDirty flags get flipped
	//SetLookAt(lookAt);
	char msgbuf[100];

	sprintf_s(msgbuf, "Rubies: %d\n", m_player->RubiesCollected());
	//OutputDebugString(msgbuf);

	SetPosition(currentPos);
	SetLookAt(lookAt);

	Camera::Update(timestep);
	m_player->SetPosition(m_position);
	m_player->Update(timestep, m_position); // *######## Really important line, to update player's bounding box

	float dist = sqrt((m_player->getCurrPos() - m_player->getPrevPos()).x *
		(m_player->getCurrPos() - m_player->getPrevPos()).x + (m_player->getCurrPos() - m_player->getPrevPos()).z
		* (m_player->getCurrPos() - m_player->getPrevPos()).z);

	float speed = dist / (timestep * 100);

	bullets[0]->setMovespeed(speed);
	grenades[0]->setMovespeed(speed * 2);
	if (bullets[0]->getShot() == 0) // bullet not shot
	{
		char msgbuf[100];
		sprintf_s(msgbuf, "Not Shot\n");
		//OutputDebugString(msgbuf);
		bullets[0]->SetPosition(GetPosition() + Vector3(0, -4, 0));
		bullets[0]->SetDirection(GetLocalForward());
	}

	if (grenades[0]->getThrown() == 0) // Grenade not thrown
	{
		char msgbuf[100];
		sprintf_s(msgbuf, "Not Shot\n");
		//OutputDebugString(msgbuf);
		grenades[0]->SetPosition(GetPosition() + Vector3(0, -4, 0));
		grenades[0]->SetDirection(localForward);
	}

	if (bullets[0]->getShot() == 1) // bullet shot
	{
		char msgbuf[100];
		sprintf_s(msgbuf, "Shot\n");
		//OutputDebugString(msgbuf);
	}
	if (m_input->GetMouseDown(LEFT_MOUSE) && bullets[0]->getShot() != 1 && m_player->AmmoLeft() > 0) // Shoot bullet
	{
		char msgbuf[100];
		sprintf_s(msgbuf, "PaarthBHasin %f %f %f\n", GetLocalForward().x, GetLocalForward().y, GetLocalForward().z);
		//OutputDebugString(msgbuf);
		bullets[0]->SetDirection(GetLocalForward());
		bullets[0]->setFirePosition(GetPosition());
		bullets[0]->setShot(1); //bullet has been shot
		bullets[0]->setOwner(0); // Player owns the bullet;
		bullets[0]->SetStatus(true);
		m_player->SetAmmo(m_player->AmmoLeft() - 1);

		m_engineSound = m_audio->Play("Assets/Sounds/handgun.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
		}
	}

	if (m_input->GetKeyDown('J') && grenades[0]->getThrown() != 1) // Throw grenade
	{
		char msgbuf[100];
		sprintf_s(msgbuf, "PaarthBHasin B %f %f %f\n", GetLocalForward().x, GetLocalForward().y, GetLocalForward().z);
		//OutputDebugString(msgbuf);
		grenades[0]->SetDirection(localForward);
		grenades[0]->setFirePosition(GetPosition());
		grenades[0]->setThrown(1); //bullet has been shot
		grenades[0]->SetStatus(true);
		grenades[0]->setPlayerPos(m_player->GetPosition());
		//m_player->SetAmmo(m_player->AmmoLeft() - 1);

		m_engineSound = m_audio->Play("Assets/Sounds/throwing fire grenade.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
		}

		m_engineSound = m_audio->Play("Assets/Sounds/release.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
		}
	}
}

Vector3 FirstPersonCamera::TransformOffset()
{
	// Wrap up the Y rotation of the object we're following in a matrix
	Matrix rotation = Matrix::CreateRotationY(m_player->GetYRotation());

	// Transform our offset vector by the rotation so the offset rotates with our follow object
	return Vector3::TransformNormal(m_offset, rotation);
}

void FirstPersonCamera::OnTeleportationEnter(Teleporter* other)
{

	m_engineSound = m_audio->Play("Assets/Sounds/teleporter.wav", true);

	if (m_engineSound && sound_effects)
	{
		m_engineSound->SetLoopCount(0);
		m_engineSound->SetIs3D(true);
		m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
		m_engineSound->SetPaused(false);
	}

	int level = other->getLevel();
	int ID = other->getID();
	if (ID == 1 && m_player->getT1() == 3)
	{
		m_position = Vector3(-5.0f, 2.0f, -5.0f);
		m_player->setLevel(0);
		grenade->setLevel(0);
	}


	if (ID == 2 && m_player->getT2() == 3)
	{
		m_position = Vector3(5.0f, 62.0f, -5.0f);
		m_player->setLevel(1);
		grenade->setLevel(1);
	}

	if (ID == 3 && m_player->getT3() == 3)
	{
		m_position = Vector3(5.0f, -58.0f, -5.0f);
		m_player->setLevel(-1);
		grenade->setLevel(-1);
	}

	if (ID == 4 && m_player->getT4() == 3)
	{
		m_position = Vector3(5.0f, 2.0f, -5.0f);
		m_player->setLevel(0);
		grenade->setLevel(0);
	}

	m_player->SetPosition(m_position);
}
void FirstPersonCamera::OnTeleportationStay(Teleporter* other)
{

}
void FirstPersonCamera::OnTeleportationExit(Teleporter* other)
{

}