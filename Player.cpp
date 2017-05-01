/*#include "Player.h"

Player::Player(Vector3 position) :
	GameObject(position)
{

	m_moveSpeed = 0.1f;
	m_turnSpeed = 2.0f;
	m_position = position;
	m_boundingBox = CBoundingBox(m_position + Vector3(-0.5, 0, -0.125), m_position + Vector3(0.5, 1.8, 0.125));
}

void Player::Update(float timestep)
{
	/*m_boundingBox.SetMin(m_position + Vector3(-0.5, 0, -0.125));
	m_boundingBox.SetMax(m_position + Vector3(0.5, 1.8, 0.125));
}
void Player::Update(float timestep, Vector3 position)
{
	m_position = position;
	m_boundingBox.SetMin(m_position + Vector3(-0.5, 0, -0.125));
	m_boundingBox.SetMax(m_position + Vector3(0.5, 1.8, 0.125));
}

void Player::OnItemCollisionEnter(ItemBox* other)
{
	OutputDebugString("Player-Item Collision Enter\n");
}

void Player::OnItemCollisionStay(ItemBox* other)
{
	OutputDebugString("Player-ItemBox Collision Stay\n");
}

void Player::OnItemCollisionExit(ItemBox* other)
{
	OutputDebugString("Player-ItemBox Collision Exit\n");
}

void Player::OnBulletCollisionEnter(ItemBox* other)
{

}
void Player::OnBulletCollisionStay(ItemBox* other)
{
	
}
void Player::OnBulletCollisionExit(ItemBox* other)
{

}
*/