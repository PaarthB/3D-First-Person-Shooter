/*
*	Teleporter.cpp
*	Implementation of Teleporter.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	DESCRIPTION:
*	A general class which represents one the 4 teleporters the player will walk into
*/

#include "Teleporter.h"
#include "MathsHelper.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <iostream>

Teleporter::Teleporter(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
	GameObject(mesh, shader, texture, position)
{
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

void Teleporter::Update(float timestep)
{
	m_boundingBox.SetMin(m_position + m_mesh->GetMin() * 0.29);
	m_boundingBox.SetMax(m_position + m_mesh->GetMax() * 0.29);
}
void Teleporter::OnPlayerCollisionEnter()
{
	OutputDebugString("TELEPORT PLAYER");
}

void Teleporter::OnPlayerCollisionStay()
{
	OutputDebugString("Player collides with object 5\n");
}

void Teleporter::OnPlayerCollisionExit()
{
	OutputDebugString("Player collides with object 6\n");
}

void Teleporter::OnEnemyCollisionEnter()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Teleporter::OnEnemyCollisionStay()
{
	OutputDebugString("Enemy shot with bullet\n");
}

void Teleporter::OnEnemyCollisionExit()
{
	OutputDebugString("Enemy shot with bullet\n");
}
