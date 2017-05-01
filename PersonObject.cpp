/*
*	PersonObject.cpp
*	Implementation of PersonObject.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2A
*	@Date: 29th April 2017
*
*	DESCRIPTION:
*	A general class which represents different kinds of persons/characters that may exist in the game.
*/

#include "PersonObject.h"
#include "MathsHelper.h"



PersonObject::PersonObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) : GameObject(mesh, shader, texture, position)
{
	// Try lowering the friction - it's good fun :)
	m_position = position;
	m_boundingBox = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}

PersonObject::PersonObject(Vector3 position) : GameObject(position)
{
	// Try lowering the friction - it's good fun :)
	m_position = position;
	m_boundingBox = CBoundingBox(m_position + Vector3(-0.725, -2, -0.75), m_position + Vector3(0.725, -0.2, 0.75));
}

void PersonObject::Update(float timestep, Vector3 position)
{
	m_position = position;
	m_boundingBox = CBoundingBox(m_position + Vector3(-0.725, -2, -0.75), m_position + Vector3(0.725, -0.2, .75));
	if (rubies_collected == 10)
		PostQuitMessage(0);
	if (health == 0)
		PostQuitMessage(0);
}

void PersonObject::Update(float timestep)
{
	
	if (type > 0) // Person is enemy, then warp its spped due to time warp
	{
		float dist = sqrt((currPos - prevPos).x * (currPos - prevPos).x + (currPos - prevPos).z * (currPos - prevPos).z);
		speed = dist / (timestep * 100);
		bullet[0]->setMovespeed(speed);
	}
	switch (type)
	{
	case 0: // Player
		break;
	case 1:	// Enemy 1
		Update1(timestep);
		break;
	case 2:	// Enemy 2
		Update2(timestep);
		break;
	case 3:	// Enemy 3
		Update3(timestep);
		break;
	case 4:	// Enemy 4
		Update4(timestep);
		break;
	case 5:	// Enemy 5
		Update5(timestep);
		break;
	default:
		break;
	}
	
	// Update the bounding box as the posiition of the person changes
	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());	
}


void PersonObject::OnBulletCollisionEnter(ItemBox* other)
{
	// Decrease health of the person if it gets hit by a bullet
	health -= 10;
	OutputDebugString("Player collides with bullet\n");
}

void PersonObject::OnBulletCollisionStay(ItemBox* other)
{
	OutputDebugString("Player collides with bullet 2\n");
}

void PersonObject::OnBulletCollisionExit(ItemBox* other)
{
	OutputDebugString("Player collides with bullet 3\n");
}

void PersonObject::OnItemCollisionStay(ItemBox* other)
{
	OutputDebugString("Player collides with object\n");
}
void PersonObject::OnItemCollisionEnter(ItemBox* other)
{
	OutputDebugString("Player collides with object 2\n");
	if (other->GetType() == 0) // Person collects an ammunition block
	{
		ammo_remaining++;
		OutputDebugString("Ammo\n");
	}

	if (other->GetType() == 1) // Person get hits by a bullet
	{
		health -= 10;
		other->setShot(0);
	}
		
	if (other->GetType() == 2) // Person collects a ruby
		rubies_collected++;
}
void PersonObject::OnItemCollisionExit(ItemBox* other)
{
	OutputDebugString("Player collides with object 3\n");
}


void PersonObject::Update1(float timestep) // Update motion of enemy 1
{
	rotation = RotateToFace(player_position, m_position); // Face the player to shoot accurately
	SetYRotation(rotation);

	if (cooldown <= 0) // Cooldown over
	{
		cooldown = 100;
		bullet[0]->SetYRotation(rotation);
		Vector3 direction = player_position - m_position;

		// Generating a unit vector in the direction in which the enemy has to shoot
		bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + 
			(direction.z * direction.z))));
		
		bullet[0]->setShot(1); // Bullet has been shot
		bullet[0]->setFirePosition(m_position);
		bullet[0]->SetPosition(m_position + TransformOffset()); // Offset of the gun
		bullet[0]->SetStatus(true); // It will be visible
	}
	else
	{
		cooldown -= timestep * 10; // Decreasing the cooldown timer
	}

	// Make the first enemy chase the player and warp its speed
	Vector3 step = player_position + Vector3(0, 0, 1) - m_position;
	step.Normalize();
	step.y = 0;
	m_position += step * 0.1 * (1 + 10 * speed);
}
void PersonObject::Update2(float timestep) // Update motion of enemy 2
{
	if (abs(m_position.z - random_point.z) >= 1) // Has the enemy reached the random point yet?
	{
		Vector3 step = random_point - m_position;
		step.Normalize();
		m_position += step * 0.1 * (1 + 15 * speed);
	}

	else // If yes then generate a new random point for the player to go to.
	{
		random_point = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
	}

	rotation = RotateToFace(player_position, m_position); // Face the player to shoot accurately
	SetYRotation(rotation);

	if (cooldown <= 0) // Cooldown over
	{
		cooldown = 100; // Reset cooldown
		bullet[0]->SetYRotation(rotation);
		Vector3 direction = player_position - m_position; // Direction towards the player

		// Generating a unit vector in the direction in which the enemy has to shoot
		bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + 
			(direction.z * direction.z))));

		bullet[0]->setShot(1); // Bullet has been shot
		bullet[0]->setFirePosition(m_position);
		bullet[0]->SetPosition(m_position + TransformOffset());
		bullet[0]->SetStatus(true); // it will be visible
	}
	else
	{
		cooldown -= timestep * 10;
	}


}
void PersonObject::Update3(float timestep)
{
	if (abs(m_position.z - random_point.z) >= 1) // Has the enemy reached the random ruby yet?
	{
		Vector3 step = random_point + Vector3(0, 0, 1) - m_position;
		step.Normalize();
		m_position += step *  0.1 * (1 + 15 * speed);
	}

	else // If yes then generate a new random ruby for the player to go to.
	{
		int v = rand() % 9;
		random_point = (*rubies)[v]->GetPosition();
	}

	rotation = RotateToFace(player_position, m_position);
	SetYRotation(rotation);

	if (cooldown <= 0)
	{
		cooldown = 100;
		bullet[0]->SetYRotation(rotation);
		Vector3 direction = player_position - m_position;
		bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z))));
		bullet[0]->setShot(1);
		bullet[0]->setFirePosition(m_position);
		bullet[0]->SetPosition(m_position + TransformOffset());
		bullet[0]->SetStatus(true);
	}
	else
	{
		cooldown -= timestep * 10;
	}

}
void PersonObject::Update4(float timestep)
{
	if (sqrt((m_position.x - player_position.x) * (m_position.x - player_position.x) +
		((m_position.z - player_position.z) * (m_position.z - player_position.z))) <= 10) // If the player gets too close, flee to a 
																						  // random point
	{
		m_position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
	}

	rotation = RotateToFace(player_position, m_position);
	SetYRotation(rotation);
	
	if (cooldown <= 0)
	{
		cooldown = 50; // Enemy 4 shoots more frequently than others
		bullet[0]->SetYRotation(rotation);
		Vector3 direction = player_position - m_position;
		bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z))));
		bullet[0]->setShot(1);
		bullet[0]->setFirePosition(m_position);
		bullet[0]->SetPosition(m_position + TransformOffset());
		bullet[0]->SetStatus(true);
	}
	else
	{
		cooldown -= timestep * 10;
	}
}
void PersonObject::Update5(float timestep)
{
	if (sqrt((m_position.x - player_position.x) * (m_position.x - player_position.x) +
		((m_position.z - player_position.z) * (m_position.z - player_position.z))) <= 10) // If the player gets too close, flee to a 
																						  // random point
	{
		m_position = Vector3(MathsHelper::RandomRange(-145.0f, 145.0f), 0.0f, MathsHelper::RandomRange(-145.0f, 145.0f));
	}

	rotation = RotateToFace(player_position, m_position);
	SetYRotation(rotation);
	
	if (cooldown <= 0)
	{
		cooldown = 45; // Enemy 5 shoots more frequently than the others
		bullet[0]->SetYRotation(rotation);
		Vector3 direction = player_position - m_position;
		bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z))));
		bullet[0]->setShot(1);
		bullet[0]->setFirePosition(m_position);
		bullet[0]->SetPosition(m_position + TransformOffset());
		bullet[0]->SetStatus(true);
	}
	else
	{
		cooldown -= timestep * 10;
	}

}

// Returns angle to rotate if we are at origin and want to face target
float PersonObject::RotateToFace(Vector3 target, Vector3 origin)
{
	Vector3 direction = target - origin;

	float angle = atan2(direction.x, direction.z);
	if (angle < 0)
		angle += 2 * PI;
	return angle;

}


// Rotates the offset gun vector for enemies to ensure it is correct when enemy rotates
Vector3 PersonObject::TransformOffset()
{
	// Wrap up the Y rotation of the object we're following in a matrix
	Matrix rotation = Matrix::CreateRotationY(GetYRotation());

	// Transform our offset vector by the rotation so the offset rotates with our follow object
	return Vector3::TransformNormal(offset, rotation);
}