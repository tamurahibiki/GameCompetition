#pragma once
#include "camera/SpringCamera.h"

class Player;

class GameCamera :public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
	Vector3                   m_toCameraPos = Vector3::One;
	SpringCamera              m_springCamera;
	Player*                   m_player;
};