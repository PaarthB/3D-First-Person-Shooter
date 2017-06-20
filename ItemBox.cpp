/*
*	ItemBox.cpp
*	Implementation of ItemBox.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	DESCRIPTION:
*	A general class which represents the item boxes in the battlefield
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
	//SetStatus(true);
	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());
}

void ItemBox::OnGrenadeCollisionEnter()
{
	visible = false;
}
void ItemBox::OnGrenadeCollisionStay()
{

}
void ItemBox::OnGrenadeCollisionExit()
{

}

void ItemBox::OnBulletCollisionEnter()
{
	visible = false;
}
void ItemBox::OnBulletCollisionStay()
{

}
void ItemBox::OnBulletCollisionExit()
{

}



void ItemBox::OnPlayerCollisionEnter()
{

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
