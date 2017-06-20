/*
*	Bullet.cpp
*	Implementation of Bullet.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	DESCRIPTION:
*	A general class which represents a bullet owned by a PersonObject
*/

#include "Bullet.h"
#include "MathsHelper.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

Bullet::Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

void Bullet::Update(float timestep)
{

	Vector3 worldForward = Vector3(0, 0, 1);

	float angle = atan2(direction.x, direction.z) - atan2(worldForward.x, worldForward.z);
	// Angle to rotate bullet in direction of player
	if (angle < 0)
		angle += 2 * PI;
	SetYRotation(angle);

	char msgbuf[100];
	sprintf_s(msgbuf, "Angle A: %f %f %f\n", direction.x, direction.y, direction.z);
	//OutputDebugString(msgbuf);

	if (shot == 1) // If bullet has been shot
	{
		if (sqrt((m_position.x - firePosition.x) * (m_position.x - firePosition.x) +
			((m_position.z - firePosition.z) * (m_position.z - firePosition.z))) >= 75)
			// If it has travelled more than 75 m, it vanishes
		{
			shot = 0;
			visible = false;
		}
		else
		{
			if (owner == 0)
				m_position.y = m_pos.y - 0.8f; // Bullet only travels horizontally
			else
			{
				if (level == -1)
					m_position.y = -58.8f;
				if (level == 0)
					m_position.y = 1.2f;
				if (level == 1)
					m_position.y = 61.2f;
			}
			if (level == -1)
				m_position += direction * 0.1 * 0.6 * (200 * moveSpeed + 1); // Time warp included
			if (level == 0)
				m_position += direction * 0.1 * 0.8 * (50 * moveSpeed + 1); // Time warp included
			if (level == 1)
				m_position += direction * 0.1 * 1 * (50 * moveSpeed + 1); // Time warp included
		}
	}

	// Update the bounding box if they bullet moves
	m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 3.0f);
	m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 3.0f);


}

void Bullet::OnBoxCollisionEnter()
{

}
void Bullet::OnBoxCollisionStay()
{

}
void Bullet::OnBoxCollisionExit()
{

}
void Bullet::OnPlayerCollisionEnter()
{
	// Player is hit by bullet
	setShot(0);
	visible = false;

}

void Bullet::OnPlayerCollisionStay()
{
	OutputDebugString("Player collides with object 5\n");
}

void Bullet::OnPlayerCollisionExit()
{
	OutputDebugString("Player collides with object 6\n");
}

void Bullet::OnEnemyCollisionEnter()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Bullet::OnEnemyCollisionStay()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Bullet::OnEnemyCollisionExit()
{
	OutputDebugString("Enemy shot with bullet\n");
}
