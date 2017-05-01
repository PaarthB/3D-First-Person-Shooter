/*
*	ItemBox.cpp
*	Implementation of ItemBox.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
*	
*	DESCRIPTION:
*	A general class which represents different stationary and moving objects and manages their movement and collision with the player
*	or enemy.
*/

#include "ItemBox.h"
#include "MathsHelper.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

ItemBox::ItemBox(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

void ItemBox::Update(float timestep)
{
	switch (type) // Checking the type of the ItemBox (bullet, ammo or ruby)
	{
	case 0: // Ammunition
	{
		if (cooldown <= 0) // If cooldown has finished, spwan it to a random location
		{
			m_position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
			cooldown = rand() % 400 + 1; // Genrate a random cool-down timer
			SetStatus(true); // Will be visible/rendered
		}
		else
			cooldown -= timestep * 10; // Else keep reducing the cooldown timer
		m_boundingBox.SetMin(m_position + m_mesh->GetMin());
		m_boundingBox.SetMax(m_position + m_mesh->GetMax());
		break;
	}
		
	case 1: // Bullet
	{
		Vector3 worldForward = Vector3(0, 0, 1);

		float angle = atan2(direction.x, direction.z) - atan2(worldForward.x, worldForward.z); 
		// Angle to rotate bullet in direction of player
		if (angle < 0)
			angle += 2 * PI;
		SetYRotation(angle);

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
				m_position.y = 1.2f; // Bullet only travels horizontally
				m_position += direction * 0.1 * ( 50 * moveSpeed + 1); // Time warp included
			}
		}

		// Update the bounding box if they bullet moves
		m_boundingBox.SetMin(m_position + m_mesh->GetMin());
		m_boundingBox.SetMax(m_position + m_mesh->GetMax());

		break;
	}
	case 2: // Ruby
		// Do nothing, rubies are stationary
		break;
	default:
		// Safe step , if everything else fails
		break;
	}
}
void ItemBox::OnPlayerCollisionEnter()
{
	OutputDebugString("Player collides with object 4\n");
	switch (type)
	{
	case 0: // Player goes over ammo block
		OutputDebugString("Ammo 1\n");
		visible = true; // Make it visible and transport to a random location
		m_position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
		break;
	case 2: // Player collects ruby
		visible = false;
		m_position.y = -5;
		break;
	case 1: // Player is hit by bullet
		setShot(0);
		visible = false;
		break;
	default:
		break;
	}
}

void ItemBox::OnPlayerCollisionStay()
{
	OutputDebugString("Player collides with object 5\n");
}

void ItemBox::OnPlayerCollisionExit()
{
	OutputDebugString("Player collides with object 6\n");
}

void ItemBox::OnEnemyCollisionEnter()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void ItemBox::OnEnemyCollisionStay()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void ItemBox::OnEnemyCollisionExit()
{
	OutputDebugString("Enemy shot with bullet\n");
}
