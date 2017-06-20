/*
*	ItemBox.cpp
*	Implementation of ItemBox.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	DESCRIPTION:
*	A general class which represents rubies, ammo blocks and armors
*/

#include "ItemObject.h"
#include "MathsHelper.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

ItemObject::ItemObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

void ItemObject::Update(float timestep)
{
	switch (type) // Checking the type of the ItemBox (bullet, ammo or ruby)
	{
	case 0: // Ammunition
	{
		if (cooldown <= 0) // If cooldown has finished, spwan it to a random location
		{
			if (level == -1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -59.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 0)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 1.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 61.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			cooldown = rand() % 400 + 1; // Genrate a random cool-down timer
			SetStatus(true); // Will be visible/rendered
		}
		else
			cooldown -= timestep * 10; // Else keep reducing the cooldown timer

		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 2);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 2);

		m_rotY += 0.01f;
		break;
	}


	case 2: // Ruby
			// Do nothing, rubies are stationary
		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 2);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 2);
		m_rotY += 0.01f;
		break;
	case 1:
		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 0.0049);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 0.0049);
		m_rotY += 0.01f;
		break;
	case 12:
		m_rotY += 0.01f;
		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 5);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 5);

	case 13: // Ruby
			// Do nothing, rubies are stationary
		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 2);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 2);
		m_rotY += 0.01f;
	case 14: // Ruby
			// Do nothing, rubies are stationary
		m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 2);
		m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 2);
		m_rotY += 0.01f;
	default:
		// Safe step , if everything else fails
		break;

	}
}
void ItemObject::OnPlayerCollisionEnter()
{
	OutputDebugString("Player collides with object 4\n");
	switch (type)
	{
	case 0: // Player goes over ammo block
		OutputDebugString("Ammo 1\n");
		visible = true; // Make it visible and transport to a random location
		if (level == -1)
			m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -59.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		if (level == 0)
			m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 1.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		if (level == 1)
			m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 61.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		break;
	case 2: // Player collects ruby
		visible = false;
		m_position.y = -5;
		break;
	case 1: // Player wears an armor
		visible = false;
		OutputDebugString("TELEPORT PLAYER");
		break;
	case 12: // Player collects a treasure
		visible = false;
		m_position.y = -5;
		OutputDebugString("TELEPORT PLAYER");
		break;
	case 13: // Player collects a invisibility ruby
		visible = false;
		m_position.y = -5;
		break;
	case 14: // Player collects a paralysis ruby
		visible = false;
		m_position.y = -5;
		break;
	default:
		break;
	}
}

void ItemObject::OnPlayerCollisionStay()
{
	OutputDebugString("Player collides with object 5\n");
}

void ItemObject::OnPlayerCollisionExit()
{
	OutputDebugString("Player collides with object 6\n");
}

void ItemObject::OnEnemyCollisionEnter()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void ItemObject::OnEnemyCollisionStay()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void ItemObject::OnEnemyCollisionExit()
{
	OutputDebugString("Enemy shot with bullet\n");
}
