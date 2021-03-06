# 3D-FirstPerson-Shooter-SuperHot-Reinterpretation
- A reinterpretation of the famous Indie game SuperHot, where time only moves when you do
- Developed in C++ using Microsoft DirectX v11.0 <br />
- New Features :
  - Stealth/Invisibility
  - Keplar / Bullet Proof Armor
  - Paralyzing Enemy
  - Free Explore, Control Menu
  - Teleportation
  - Level Generation/different worlds (Frosty Caves, Dragon Lair and Egypt) with their unique shaders
  - Breaking boxes to unlock gates
  - High Scores
  - Sound Effects and Background Music
  - Hand Grenade
- Future Additions:
  - Night Vision
  - Aim and shoot
  - Direction Compass (would require animated sprite, rotating by angle between player and ruby) 
  
![Image](/game.png?raw=true "3D First Person Shooter Super Hot v2.0")

# Usage:
Just run the application in MS Visual Studio 2015 or later. It runs full screen.
Needs FMOD Sound engine for sound.

# Important things:
This app contain some really important things implemented within it. 
- It has a simple mechanism to make the AI (Enemies and their bullets) face the player. It is contained in the method ```RotateToFace``` within PersonObject.cpp. 
- Vertex Shaders and Pixel Shaders which are modifiable. 
- It has important implementation of time dilation, object teleportation and player teleportation.
- It has within it implemented a Finite State Machine (FSM) for handling game all 5 game states.
- Yaw Pitch Roll transformation in GameObject.cpp is to evaluate each objects rotation matrix or rotation amount in the view matrix.
