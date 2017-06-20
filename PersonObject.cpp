/*
*	PersonObject.cpp
*	Implementation of PersonObject.h
*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
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

PersonObject::PersonObject(Vector3 position, AudioSystem* audio) : GameObject(position, audio)
{
	// Try lowering the friction - it's good fun :)
	m_position = position;
	m_audio = audio;
	m_boundingBox = CBoundingBox(m_position + Vector3(-0.725, -2, -0.75), m_position + Vector3(0.725, -0.2, 0.75));
}

void PersonObject::Update(float timestep, Vector3 position)
{
	m_position = position;
	if (invisibility_cooldown <= 0)
	{
		invisibility_cooldown = 0;
		stealth = false;
	}

	else
		invisibility_cooldown -= timestep * 10;

	if (paralysis_cooldown <= 0)
	{
		paralysis_cooldown = 0;
		paralysis = 1;
	}

	else
		paralysis_cooldown -= timestep * 10;

	m_boundingBox = CBoundingBox(m_position + Vector3(-0.725, -2, -0.75), m_position + Vector3(0.725, -0.2, .75));
	if (rubies_collected >= 15 && treasure == 0 && !infinite)
	{
		m_engineSound = m_audio->Play("Assets/Sounds/Victory_Sound_Effect.wav", true);
		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
		PostQuitMessage(0);
	}
		
}

void PersonObject::Update(float timestep)
{


	if (type > 0) // Person is enemy, then warp its spped due to time warp
	{
		float dist = sqrt((currPos - prevPos).x * (currPos - prevPos).x + (currPos - prevPos).z * (currPos - prevPos).z);
		speed = dist / (timestep * 100);
		bullet[0]->setMovespeed(speed);

		char msgbuf[100];
		sprintf_s(msgbuf, "ENEMY INVISIBILITY: %dPARALYSIS: %d\n", getStealth(), getParalyse());
		OutputDebugString(msgbuf);
	
	}
	if (type == 0) // Person is player
	{
		// Player
		char msgbuf[100];
		sprintf_s(msgbuf, "CURRENT LEVEL %f\n \n \n \n", m_position.y);
		OutputDebugString(msgbuf);
	}
	if (type == 1) // Enemy 1
		Update1(timestep);

	if (type == 2) // Enemy 2
		Update2(timestep);

	if (type == 3) // Enemy 3
		Update3(timestep);

	if (type == 4) // Enemy 4
		Update4(timestep);

	if (type == 5) // Enemy 5
		Update5(timestep);

	// Update the bounding box as the posiition of the person changes
	m_boundingBox.SetMin(m_position + m_mesh->GetMin());
	m_boundingBox.SetMax(m_position + m_mesh->GetMax());
}

void PersonObject::OnGrenadeCollisionEnter()
{
	setHealth(0); // Person immediately dies with a grenade explosion
}
void PersonObject::OnGrenadeCollisionStay()
{

}
void PersonObject::OnGrenadeCollisionExit()
{

}

void PersonObject::OnBulletCollisionEnter()
{
	// Decrease health of the person if it gets hit by a bullet
	if (!infinite)
	{
		if (!protect)
		{
			if (level == -1)
				health -= 20;
			else if (level == 0)
				health -= 15;
			else if (level == 1)
				health -= 10;
		}

		else if (protect && armor > 0)
			armor -= 5;
	}
		
	if (armor <= 0)
		protect = false;

	OutputDebugString("Player collides with bullet\n");
}

void PersonObject::OnBulletCollisionStay()
{
	OutputDebugString("Player collides with bullet 2\n");
}

void PersonObject::OnBulletCollisionExit()
{
	OutputDebugString("Player collides with bullet 3\n");
}

void PersonObject::OnItemCollisionStay(ItemObject* other)
{
	OutputDebugString("Player collides with object\n");
}
void PersonObject::OnItemCollisionEnter(ItemObject* other)
{
	OutputDebugString("Player collides with object 2\n");
	if (other->GetType() == 0) // Person collects an ammunition block
	{
		ammo_remaining++;
		OutputDebugString("Ammo\n");
	}

	if (other->GetType() == 13) // Person collects paralysis ruby
	{
		paralysis = 0;
		other->SetStatus(false);
		OutputDebugString("Ammo\n");
		paralysis_cooldown = 250;
		m_engineSound = m_audio->Play("Assets/Sounds/Correct_Answer_Sound_Effect.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
	}

	if (other->GetType() == 14) // Person collects invisibility ruby
	{
		stealth = true;
		other->SetStatus(false);
		OutputDebugString("Ammo\n");
		invisibility_cooldown = 250;
		m_engineSound = m_audio->Play("Assets/Sounds/Correct_Answer_Sound_Effect.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
	}


	if (other->GetType() == 2) // Person collects a ruby
	{
		m_engineSound = m_audio->Play("Assets/Sounds/collect_ruby.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
		rubies_collected++;
		if (level == -1)
			score += 500;
		if (level == 0)
			score += 100;
		if (level == 1)
			score += 300;
	}

	if (other->GetType() == 12) // Person picks up treasure
	{
		m_engineSound = m_audio->Play("Assets/Sounds/Correct_Answer_Sound_Effect.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
			//m_engineSound->SetVolume(10.0f);
		}
		treasure--;
		other->SetStatus(false);
	}


	if (other->GetType() == 1) // Person wears an armor
	{
		m_engineSound = m_audio->Play("Assets/Sounds/wearing jacket.wav", true);

		if (m_engineSound && sound_effects)
		{
			m_engineSound->SetLoopCount(0);
			m_engineSound->SetIs3D(true);
			m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
			m_engineSound->SetPaused(false);
		}

		OutputDebugString("ARMOR");
		protect = true;
		armor = 50;
	}

}

void PersonObject::Explosion() // Grenade explodes on collision
{
	m_engineSound = m_audio->Play("Assets/Sounds/explosion.wav", true);

	if (m_engineSound && sound_effects)
	{
		m_engineSound->SetLoopCount(0);
		m_engineSound->SetIs3D(true);
		m_engineSound->SetMinMaxDistance(30.0f, 200.0f);
		m_engineSound->SetPaused(false);
	}
}
void PersonObject::OnItemCollisionExit(ItemObject* other)
{
	OutputDebugString("Player collides with object 3\n");
}


void PersonObject::Update1(float timestep) // Update motion of enemy 1
{
	if (lives > 0)
	{
		rotation = RotateToFace(player_position, m_position); // Face the player to shoot accurately
		SetYRotation(rotation);

		if (cooldown <= 0 && paralysis == 1) // Cooldown over
		{
			if (level == -1)
				cooldown = 300;
			if (level == 0)
				cooldown = 150;
			if (level == 1)
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
		if (level == -1)
			m_position += step * 0.1 * 0.5 * (1 + 10 * speed) * paralysis;
		if (level == 0)
			m_position += step * 0.1 * 0.6 * (1 + 10 * speed) * paralysis;
		if (level == 1)
			m_position += step * 0.1 * 0.7 * (1 + 10 * speed) * paralysis;
	}
	
}
void PersonObject::Update2(float timestep) // Update motion of enemy 2
{
	if (lives > 0)
	{
		if (abs(m_position.z - random_point.z) >= 1) // Has the enemy reached the random point yet?
		{
			Vector3 step = random_point - m_position;
			step.Normalize();
			if (level == -1)
				m_position += step * 0.1 * 0.5 * (1 + 15 * speed) * paralysis;
			if (level == 0)
				m_position += step * 0.1 * 0.6 * (1 + 15 * speed) * paralysis;
			if (level == 1)
				m_position += step * 0.1 * 0.7 * (1 + 15 * speed) * paralysis;
		}

		else // If yes then generate a new random point for the player to go to
		{
			if (level == -1)
				random_point = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 0)
				random_point = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 0.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 1)
				random_point = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		}

		if (paralysis == 1 && stealth == false)
		{
			rotation = RotateToFace(player_position, m_position);
			SetYRotation(rotation);
		}

		if (cooldown <= 0 && paralysis == 1 && stealth == false) // Cooldown over
		{
			if (level == -1)
				cooldown = 300;
			if (level == 0)
				cooldown = 150;
			if (level == 1)
				cooldown = 100;
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
}
void PersonObject::Update3(float timestep)
{
	if (lives > 0)
	{
		if (abs(m_position.z - random_point.z) >= 1) // Has the enemy reached the random ruby yet?
		{
			Vector3 step = random_point + Vector3(0, 0, 1) - m_position;
			step.Normalize();
			if (level == -1)
				m_position += step * 0.1 * 0.5 * (1 + 15 * speed) * paralysis;
			if (level == 0)
				m_position += step * 0.1 * 0.6 * (1 + 15 * speed) * paralysis;
			if (level == 1)
				m_position += step * 0.1 * 0.7 * (1 + 15 * speed) * paralysis;
		}

		else // If yes then generate a new random ruby for the player to go to.
		{
			int v = rand() % 9;
			random_point = (*rubies)[v]->GetPosition();
		}

		if (paralysis == 1 && stealth == false)
		{
			rotation = RotateToFace(player_position, m_position);
			SetYRotation(rotation);
		}
		

		if (cooldown <= 0 && paralysis == 1 && stealth == false)
		{
			if (level == -1)
				cooldown = 300;
			if (level == 0)
				cooldown = 150;
			if (level == 1)
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
	

}
void PersonObject::Update4(float timestep)
{	
	if (lives > 0)
	{
		if (sqrt((m_position.x - player_position.x) * (m_position.x - player_position.x) +
			((m_position.z - player_position.z) * (m_position.z - player_position.z))) <= 10 && paralysis != 0 && stealth == false) // If the player gets too close, flee to a 
																							  // random point
		{
			if (level == -1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 0)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 0.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		}

		if (paralysis == 1 && stealth == false)
		{
			rotation = RotateToFace(player_position, m_position);
			SetYRotation(rotation);
		}

		if (cooldown <= 0 && paralysis == 1 && stealth == false)
		{
			if (level == -1)
				cooldown = 200;
			if (level == 0)
				cooldown = 100;
			if (level == 1)
				cooldown = 50;
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
	
}
void PersonObject::Update5(float timestep)
{
	if (lives > 0)
	{
		if (sqrt((m_position.x - player_position.x) * (m_position.x - player_position.x) +
			((m_position.z - player_position.z) * (m_position.z - player_position.z))) <= 10 && paralysis != 0 && stealth == false) // If the player gets too close, flee to a 
																							  // random point
		{
			if (level == -1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), -60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 0)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 0.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
			if (level == 1)
				m_position = Vector3(MathsHelper::RandomRange(-45.0f, 45.0f), 60.0f, MathsHelper::RandomRange(-45.0f, 45.0f));
		}

		if (paralysis == 1 && stealth == false)
		{
			rotation = RotateToFace(player_position, m_position);
			SetYRotation(rotation);
		}

		if (cooldown <= 0 && paralysis == 1 && stealth == false)
		{
			if (level == -1)
				cooldown = 170;
			if (level == 0)
				cooldown = 70;
			if (level == 1)
				cooldown = 40;
			bullet[0]->SetYRotation(rotation);
			Vector3 direction = player_position - m_position;
			bullet[0]->SetDirection(direction / (sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z))));
			bullet[0]->setShot(1);
			bullet[0]->setFirePosition(m_position);
			bullet[0]->SetPosition(m_position + TransformOffset());
			bullet[0]->SetStatus(true);
			Vector3 pos = bullet[0]->GetPosition();

		}
		else
		{
			cooldown -= timestep * 10;
		}
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