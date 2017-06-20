/*
*	Grenade.cpp
*	Implementation of Grenade.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	DESCRIPTION:
*	A general class which represents a grenade held by the player
*/

#include "Grenade.h"
#include "MathsHelper.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>


Grenade::Grenade(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

Grenade::Grenade(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, AudioSystem* audio) :
	GameObject(mesh, shader, texture, position, audio)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

void Grenade::Update(float timestep)
{
	Vector3 worldForward = Vector3(0, 0, 1);

	float angle = atan2(direction.x, direction.z) - atan2(worldForward.x, worldForward.z);
	// Angle to rotate grenade in direction of player
	if (angle < 0)
		angle += 2 * PI;
	SetYRotation(angle);
	char msgbuf[100];
	sprintf_s(msgbuf, "Angle: %f %f %f\n", direction.x, direction.y, direction.z);
	//OutputDebugString(msgbuf);

	if (thrown == 1)
	{
		if (sqrt((m_position.x - firePosition.x) * (m_position.x - firePosition.x) +
			((m_position.z - firePosition.z) * (m_position.z - firePosition.z)) >= 300))
			// If it grenade travelled more than 100 m, it vanishes
		{
			thrown = 0;
			visible = true;
			//TimedBlast(m_position);
		}
		else
		{
			m_position.y = m_pos.y - 1.8f; // Bullet only travels horizontally
			m_position += direction * 0.4 * (50 * moveSpeed + 1); // Time warp included
		}
	}

	// Update the bounding box if they bullet moves
	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());

}

void Grenade::TimedBlast(Vector3 pos)
{
	if (level == -1)
	{
		for (unsigned int i = 0; i < boxes_1.size(); i++)
		{
			if (sqrt((pos.x - boxes_1[i]->GetPosition().x) * (pos.x - boxes_1[i]->GetPosition().x) +
				((pos.z - boxes_1[i]->GetPosition().z) * (pos.z - boxes_1[i]->GetPosition().z)) <= 15))
			{
				boxes_1[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}

		for (unsigned int i = 0; i < enemies_1.size(); i++)
		{
			if (sqrt((pos.x - enemies_1[i]->GetPosition().x) * (pos.x - enemies_2[i]->GetPosition().x) +
				((pos.z - enemies_1[i]->GetPosition().z) * (pos.z - enemies_1[i]->GetPosition().z)) <= 30))
			{
				enemies_1[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}
	}

	if (level == 0)
	{
		for (unsigned int i = 0; i < boxes_2.size(); i++)
		{
			if (sqrt((pos.x - boxes_2[i]->GetPosition().x) * (pos.x - boxes_2[i]->GetPosition().x) +
				((pos.z - boxes_2[i]->GetPosition().z) * (pos.z - boxes_2[i]->GetPosition().z)) <= 15))
			{
				boxes_2[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}

		for (unsigned int i = 0; i < enemies_2.size(); i++)
		{
			if (sqrt((pos.x - enemies_2[i]->GetPosition().x) * (pos.x - enemies_2[i]->GetPosition().x) +
				((pos.z - enemies_2[i]->GetPosition().z) * (pos.z - enemies_2[i]->GetPosition().z)) <= 30))
			{
				enemies_2[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}
	}

	if (level == 1)
	{
		for (unsigned int i = 0; i < boxes_3.size(); i++)
		{
			if (sqrt((pos.x - boxes_3[i]->GetPosition().x) * (pos.x - boxes_3[i]->GetPosition().x) +
				((pos.z - boxes_3[i]->GetPosition().z) * (pos.z - boxes_3[i]->GetPosition().z)) <= 15))
			{
				boxes_3[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}

		for (unsigned int i = 0; i < enemies_3.size(); i++)
		{
			if (sqrt((pos.x - enemies_3[i]->GetPosition().x) * (pos.x - enemies_3[i]->GetPosition().x) +
				((pos.z - enemies_3[i]->GetPosition().z) * (pos.z - enemies_3[i]->GetPosition().z)) <= 30))
			{
				enemies_3[i]->SetStatus(false);
				thrown = 0;
				visible = true;
			}
		}

	}
		
}
void Grenade::Update1(Vector3 pos)
{
	for (unsigned int i = 0; i < enemies_1.size(); i++)
	{
		if (sqrt((pos.x - enemies_1[i]->GetPosition().x) * (pos.x - enemies_1[i]->GetPosition().x) +
			((pos.z - enemies_1[i]->GetPosition().z) * (pos.z - enemies_1[i]->GetPosition().z)) <= 30))
		{
			enemies_1[i]->setHealth(0);
			enemies_1[i]->decLives();
		}
	}
}

void Grenade::Update2(Vector3 pos)
{
	for (unsigned int i = 0; i < enemies_2.size(); i++)
	{
		if (sqrt((pos.x - enemies_2[i]->GetPosition().x) * (pos.x - enemies_2[i]->GetPosition().x) +
			((pos.z - enemies_2[i]->GetPosition().z) * (pos.z - enemies_2[i]->GetPosition().z)) <= 30))
		{
			enemies_2[i]->setHealth(0);
			enemies_2[i]->decLives();
		}
	}
}

void Grenade::Update3(Vector3 pos)
{
	for (unsigned int i = 0; i < enemies_3.size(); i++)
	{
		if (sqrt((pos.x - enemies_3[i]->GetPosition().x) * (pos.x - enemies_3[i]->GetPosition().x) +
			((pos.z - enemies_3[i]->GetPosition().z) * (pos.z - enemies_3[i]->GetPosition().z)) <= 30))
		{
			enemies_3[i]->setHealth(0);
			enemies_3[i]->decLives();
		}
	}
}

void Grenade::OnBoxCollisionEnter(ItemBox* other)
{
	other->SetStatus(false);
	thrown = 0;
	visible = true;
}
void Grenade::OnBoxCollisionStay(ItemBox* other)
{

}
void Grenade::OnBoxCollisionExit(ItemBox* other)
{

}
void Grenade::OnPlayerCollisionEnter()
{
	setThrown(0);
	visible = true;
}

void Grenade::OnPlayerCollisionStay()
{
	OutputDebugString("Player collides with object 5\n");
}

void Grenade::OnPlayerCollisionExit()
{
	OutputDebugString("Player collides with object 6\n");
}

void Grenade::OnEnemyCollisionEnter()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Grenade::OnEnemyCollisionStay()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Grenade::OnEnemyCollisionExit()
{
	OutputDebugString("Enemy shot with bullet\n");
}
