/*
*	@Developed by: Paarth Bhasin
*	@Purpose: FIT2096 Assignment-2B
*	@Date: 27th May 2017
*
*	Describes a Grenade class which represents a grenade thrown by a player.
*/
#ifndef GRENADE_H
#define GRENADE_H

#include "GameObject.h"
#include "AudioClip.h"
#include "AudioSystem.h"
#include "PersonObject.h"
#include "ItemBox.h"
class Grenade : public GameObject
{
private:
	Vector3 m_pos;
	CBoundingBox m_boundingBox; // Bounding box of the item to detect collisions
	float moveSpeed = 0; // Initialisation
	float cooldown = 200; // This is the initialisation
	Vector3 direction; // Directon the bullet will travel in
	Vector3 firePosition; // The position the bullet was fired from
	// Enemies on each level
	std::vector<PersonObject*> enemies_1;
	std::vector<PersonObject*> enemies_2;
	std::vector<PersonObject*> enemies_3;
	// Boxes on each level
	std::vector<ItemBox*> boxes_1;
	std::vector<ItemBox*> boxes_2;
	std::vector<ItemBox*> boxes_3;
	int level; // Current building level of the grenade
	int thrown; // To check whether grenade has been thrown or not
	AudioSystem* m_audio;
	AudioClip* m_engineSound;
	bool sound_effects = true;

public:
	Grenade(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, AudioSystem* audio);
	Grenade(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

	void Update(float timestep);

	//void Update(float timestep, Camera* cam, Direct3D* renderer);

	CBoundingBox GetBounds() { return m_boundingBox; }
	void SetBounds(CBoundingBox bounds) { m_boundingBox = bounds; }

	void TimedBlast(Vector3 pos); // Grenade explodes after some time even though it hasn't collided with anyone

	void OnEnemyCollisionEnter();
	void OnEnemyCollisionStay();
	void OnEnemyCollisionExit();

	void OnPlayerCollisionEnter();
	void OnPlayerCollisionStay();
	void OnPlayerCollisionExit();

	void OnBoxCollisionEnter(ItemBox* other);
	void OnBoxCollisionStay(ItemBox* other);
	void OnBoxCollisionExit(ItemBox* other);

	void Update1(Vector3 pos);
	void Update2(Vector3 pos);
	void Update3(Vector3 pos);

	void SetDirection(Vector3 dir) { direction = dir; }
	void setThrown(int i) { thrown = i; } 
	int getThrown() { return thrown; }
	//void setStatus(bool status) { visible = status; }
	//bool getStatus() { return visible; }
	void setFirePosition(Vector3 pos) { firePosition = pos; }
	void setMovespeed(float speed) { moveSpeed = speed; }
	void setPlayerPos(Vector3 Pos) { m_pos = Pos; }
	void setLevel(int lev) { level = lev; }
	int getLevel() { return level; }
	void setEnemy1(std::vector<PersonObject*> e1) { enemies_1 = e1; }
	void setEnemy2(std::vector<PersonObject*> e2) { enemies_1 = e2; }
	void setEnemy3(std::vector<PersonObject*> e3) { enemies_1 = e3; }
	void setBox1(std::vector<ItemBox*> b1) { boxes_1 = b1; }
	void setBox2(std::vector<ItemBox*> b2) { boxes_2 = b2; }
	void setBox3(std::vector<ItemBox*> b3) { boxes_3 = b3; }
	void setSoundEffects(bool t) { sound_effects = t; }

};

#endif