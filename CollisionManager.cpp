#include "CollisionManager.h"

CollisionManager::CollisionManager(std::vector<ItemObject*>* rubies, std::vector<ItemObject*>* ammo, std::vector<Bullet*>* bullets,
	std::vector<PersonObject*>* enemies, std::vector<PersonObject*>* p, std::vector<ItemObject*>* addons, std::vector<Teleporter*>* tel
	, FirstPersonCamera* cam, std::vector<Grenade*>* grenades, std::vector<ItemBox*>* boxes)
{
	//m_karts = karts;
	m_rubies = rubies;
	m_bullets = bullets;
	m_ammo = ammo;
	m_players = p;
	m_enemies = enemies;
	m_addons = addons;
	teleporters = tel;
	m_cam = cam;
	m_grenades = grenades;
	m_boxes = boxes;
	// Clear our arrays to 0 (NULL)
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));
	memset(m_previousCollisions, 0, sizeof(m_previousCollisions));

	m_nextCurrentCollisionSlot = 0;
}

void CollisionManager::CheckCollisions()
{
	PlayerToBlock(); // Checking Player to Ammo collision
	PlayerToRuby(); // Checking Player to Ruby collision
	PlayerToBullet(); // Checking Player to Bullet collision
	EnemyToBullet(); // Checking Enemy to Bullet collision
	PlayerToAddon(); // Checking Player to Armor collisions
	PlayerToTeleporter(); // Checking player to Teleporter collisions
	GrenadeToBox(); // Checking grenade to box collisions
	BulletToBox(); // Checking bullet to box collisions
	// Move all current collisions into previous
	memcpy(m_previousCollisions, m_currentCollisions, sizeof(m_currentCollisions));

	// Clear out current collisions
	memset(m_currentCollisions, 0, sizeof(m_currentCollisions));

	// Now current collisions is empty, we'll start adding from the start again
	m_nextCurrentCollisionSlot = 0;

}

bool CollisionManager::ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second)
{
	// See if these two GameObjects appear one after the other in specified collisions array
	// Stop one before length so we don't overrun as we'll be checking two elements per iteration
	for (int i = 0; i < MAX_ALLOWED_COLLISIONS - 1; i += 2)
	{
		if ((arrayToSearch[i] == first && arrayToSearch[i + 1] == second) ||
			arrayToSearch[i] == second && arrayToSearch[i + 1] == first)
		{
			// Found them!
			return true;
		}
	}

	// These objects were not colliding last frame
	return false;
}

void CollisionManager::AddCollision(GameObject* first, GameObject* second)
{
	// Add the two colliding objects to the current collisions array
	// We keep track of the next free slot so no searching is required
	m_currentCollisions[m_nextCurrentCollisionSlot] = first;
	m_currentCollisions[m_nextCurrentCollisionSlot + 1] = second;

	m_nextCurrentCollisionSlot += 2;
}

void CollisionManager::PlayerToAddon()
{
	// We'll check each player against every armor or addons
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_players->size(); i++)
	{
		for (unsigned int j = 0; j < m_addons->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
			ItemObject* addon = (*m_addons)[j];
			PersonObject* player = (*m_players)[i];
			CBoundingBox PlayerBounds = player->GetBounds();
			CBoundingBox ammoBounds = addon->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(PlayerBounds, ammoBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, addon);

			if (isColliding && addon->GetStatus())
			{
				// Register the collision
				AddCollision(player, addon);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ammunition block a player has collided with it
					addon->OnPlayerCollisionStay();
					player->OnItemCollisionStay(addon);
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					addon->OnPlayerCollisionEnter();
					player->OnItemCollisionEnter(addon);
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					addon->OnPlayerCollisionExit();
					player->OnItemCollisionExit(addon);
				}
			}
		}

	}
}

void CollisionManager::BulletToBox()
{
	// We'll check each bullet against every item box
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_bullets->size(); i++)
	{
		for (unsigned int j = 0; j < m_boxes->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
			ItemBox* box = (*m_boxes)[j];
			Bullet* bullet = (*m_bullets)[i];
			CBoundingBox BoxBounds = box->GetBounds();
			CBoundingBox BulletBounds = bullet->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(BoxBounds, BulletBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, bullet, box);

			if (isColliding && box->GetStatus() && bullet->getOwner() != 1)
			{
				// Register the collision
				AddCollision(bullet, box);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ammunition block a player has collided with it
					box->OnBulletCollisionStay();
					bullet->OnBoxCollisionStay();
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					box->OnBulletCollisionEnter();
					bullet->OnBoxCollisionEnter();
					int level = box->getLevel();
					int t = box->getTeleporter();
					if (t == 1)
						(*m_players)[0]->incT1();
					if (t == 2)
						(*m_players)[0]->incT2();
					if (t == 3)
						(*m_players)[0]->incT3();
					if (t == 4)
						(*m_players)[0]->incT4();
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					box->OnBulletCollisionExit();
					bullet->OnBoxCollisionExit();
				}
			}
		}

	}
}
void CollisionManager::GrenadeToBox()
{
	// We'll check each grenade against every item box block
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_grenades->size(); i++)
	{
		for (unsigned int j = 0; j < m_boxes->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
			ItemBox* box = (*m_boxes)[j];
			Grenade* grenade = (*m_grenades)[i];
			CBoundingBox BoxBounds = box->GetBounds();
			CBoundingBox GrenadeBounds = grenade->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(BoxBounds, GrenadeBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, grenade, box);

			if (isColliding && box->GetStatus())
			{
				// Register the collision
				AddCollision(grenade, box);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ammunition block a player has collided with it
					box->OnGrenadeCollisionStay();
					grenade->OnBoxCollisionStay(box);
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					box->OnGrenadeCollisionEnter();
					grenade->OnBoxCollisionEnter(box);
					(*m_players)[0]->Explosion(); // Grenade blows up on impact
					int level = grenade->getLevel();
					int t = box->getTeleporter();
					if (level == -1)
					{
						(*m_players)[0]->incT1();
						grenade->Update1(box->GetPosition());
					}
						
					else if (level == 0)
					{
						if (t == 2)
							(*m_players)[0]->incT2();
						else if (t == 3)
							(*m_players)[0]->incT3();

						grenade->Update2(box->GetPosition());
					}
						
					else if (level == 1)
					{
						(*m_players)[0]->incT4();
						grenade->Update3(box->GetPosition());
					}
						
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					box->OnGrenadeCollisionExit();
					grenade->OnBoxCollisionExit(box);
				}
			}
		}

	}
}
void CollisionManager::GrenadeToEnemy()
{
	// We'll check each grenade against every other enemy
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_enemies->size(); i++)
	{
		for (unsigned int j = 0; j < m_grenades->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			Grenade* grenade = (*m_grenades)[j];
			PersonObject* enemy = (*m_enemies)[i];
			// Don't compare a kart to itself
			// Comparing memory addresses - beauty of pointers
			CBoundingBox EnemyBounds = enemy->GetBounds();
			CBoundingBox grenadeBounds = grenade->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(EnemyBounds, grenadeBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, enemy, grenade);

			if (isColliding && enemy->GetStatus())
				// Bullet should not be shot be from the enemy and it should have been shot
			{
				OutputDebugString("Collision");
				grenade->SetStatus(false);
				// Register the collision
				AddCollision(enemy, grenade);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the bullet an enemy has collided with it
					grenade->OnEnemyCollisionStay();
					enemy->OnGrenadeCollisionStay();
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					grenade->OnEnemyCollisionEnter();
					enemy->OnGrenadeCollisionEnter();
					(*m_players)[0]->Explosion(); // Grenade blows up on impact
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					grenade->OnEnemyCollisionExit();
					enemy->OnGrenadeCollisionExit();
				}
			}
		}
	}
}
void CollisionManager::PlayerToTeleporter()
{
	// We'll check each player against every teleporter
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_players->size(); i++)
	{
		for (unsigned int j = 0; j < teleporters->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
			Teleporter* teleporter = (*teleporters)[j];
			PersonObject* player = (*m_players)[i];
			CBoundingBox PlayerBounds = player->GetBounds();
			CBoundingBox tel_bounds = teleporter->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(PlayerBounds, tel_bounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, teleporter);

			if (isColliding && teleporter->GetStatus())
			{
				// Register the collision
				AddCollision(player, teleporter);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ammunition block a player has collided with it
					teleporter->OnPlayerCollisionStay();
					m_cam->OnTeleportationStay(teleporter);
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					teleporter->OnPlayerCollisionEnter();
					m_cam->OnTeleportationEnter(teleporter);
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					teleporter->OnPlayerCollisionExit();
					m_cam->OnTeleportationExit(teleporter);
				}
			}
		}

	}

}
void CollisionManager::PlayerToBlock()
{
	// We'll check each player against every ammunition block
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_players->size(); i++)
	{
		for (unsigned int j = 0; j < m_ammo->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			// Can't index into these directly as they're a pointer to a vector. We need to dereference them first
			ItemObject* block = (*m_ammo)[j];
			PersonObject* player = (*m_players)[i];
			CBoundingBox PlayerBounds = player->GetBounds();
			CBoundingBox ammoBounds = block->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(PlayerBounds, ammoBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, block);

			if (isColliding && block->GetStatus())
			{
				// Register the collision
				AddCollision(player, block);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ammunition block a player has collided with it
					block->OnPlayerCollisionStay();
					player->OnItemCollisionStay(block);
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					block->OnPlayerCollisionEnter();
					player->OnItemCollisionEnter(block);
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					block->OnPlayerCollisionExit();
					player->OnItemCollisionExit(block);
				}
			}
		}

	}

}

void CollisionManager::PlayerToRuby()
{
	// We'll check each player against every other ruby
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_players->size(); i++)
	{
		for (unsigned int j = 0; j < m_rubies->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			ItemObject* ruby = (*m_rubies)[j];
			PersonObject* player = (*m_players)[i];
			// Don't compare a player to itself
			// Comparing memory addresses - beauty of pointers
			CBoundingBox PlayerBounds = player->GetBounds();
			CBoundingBox rubyBounds = ruby->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(PlayerBounds, rubyBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, ruby);

			if (isColliding && ruby->GetStatus())
			{
				OutputDebugString("Collision");
				ruby->SetStatus(false);
				// Register the collision
				AddCollision(player, ruby);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the ruby a player has collided with it
					ruby->OnPlayerCollisionStay();
					player->OnItemCollisionStay(ruby);
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					ruby->OnPlayerCollisionEnter();
					player->OnItemCollisionEnter(ruby);
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					ruby->OnPlayerCollisionExit();
					player->OnItemCollisionExit(ruby);
				}
			}
		}
	}

}

void CollisionManager::PlayerToBullet()
{
	// We'll check each player against every other bullet
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_players->size(); i++)
	{
		for (unsigned int j = 0; j < m_bullets->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			Bullet* bullet = (*m_bullets)[j];
			PersonObject* player = (*m_players)[i];
			// Don't compare a player to itself
			// Comparing memory addresses - beauty of pointers
			CBoundingBox PlayerBounds = player->GetBounds();
			CBoundingBox bulletBounds = bullet->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(PlayerBounds, bulletBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, player, bullet);

			if (isColliding && bullet->GetStatus() && bullet->getOwner() != 0)
				// Bullet should not be shot by player, and it should have been shot
			{
				player->setHitStatus(true);
				OutputDebugString("Collision");
				bullet->SetStatus(false);
				// Register the collision
				AddCollision(player, bullet);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the bullet a player has collided with it
					bullet->OnPlayerCollisionStay();
					player->OnBulletCollisionStay();
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					bullet->OnPlayerCollisionEnter();
					player->OnBulletCollisionEnter();
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					bullet->OnPlayerCollisionExit();
					player->OnBulletCollisionExit();
				}
			}
		}
	}
}

void CollisionManager::EnemyToBullet()
{
	// We'll check each enemy against every other bullet
	// Note this is not overly efficient, both in readability and runtime performance
	for (unsigned int i = 0; i < m_enemies->size(); i++)
	{
		for (unsigned int j = 0; j < m_bullets->size(); j++)
		{
			// Don't need to store pointer to these objects again but favouring clarity
			Bullet* bullet = (*m_bullets)[j];
			PersonObject* enemy = (*m_enemies)[i];
			// Don't compare a kart to itself
			// Comparing memory addresses - beauty of pointers
			CBoundingBox EnemyBounds = enemy->GetBounds();
			CBoundingBox bulletBounds = bullet->GetBounds();

			// Are they colliding this frame?
			bool isColliding = CheckCollision(EnemyBounds, bulletBounds);

			// Were they colliding last frame?
			bool wasColliding = ArrayContainsCollision(m_previousCollisions, enemy, bullet);

			if (isColliding && bullet->GetStatus() && bullet->getOwner() != 1)
				// Bullet should not be shot be from the enemy and it should have been shot
			{
				OutputDebugString("Collision");
				bullet->SetStatus(false);
				// Register the collision
				AddCollision(enemy, bullet);

				if (wasColliding)
				{
					// We are colliding this frame and we were also colliding last frame - that's a collision stay
					// Tell the bullet an enemy has collided with it
					bullet->OnEnemyCollisionStay();
					enemy->OnBulletCollisionStay();
				}
				else
				{
					// We are colliding this frame and we weren't last frame - that's a collision enter
					bullet->OnEnemyCollisionEnter();
					enemy->OnBulletCollisionEnter();
				}
			}
			else
			{
				if (wasColliding)
				{
					// We aren't colliding this frame but we were last frame - that's a collision exit
					bullet->OnEnemyCollisionExit();
					enemy->OnBulletCollisionExit();
				}
			}
		}
	}
}