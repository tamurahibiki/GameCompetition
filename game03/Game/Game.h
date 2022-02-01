#pragma once
#include "Level3DRender/LevelRender.h"

class Player;
class Enemy;
class Boss;
class Kabe;
class Yuka;
class Mura;
class GameCamera;
class Fade;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	/// <summary>
	/// ゲームクリアーを通知する。
	/// </summary>
	void NotifyGameClear();
	/// <summary>
	/// ゲームオーバーを通知する。
	/// </summary>
	void NotifyGameOver();
	/// <summary>
	/// <returns>エネミーが全滅していたらtrue。</returns>
	const bool IsWannihilationEnemy() const
	{
		return m_numEnemy == m_numDefeatedEnemy;
	}
	/// <summary>
	/// 倒された敵の数を+1する。
	/// </summary>
	void AddDefeatedEnemyNumber()
	{
		m_numDefeatedEnemy++;
	}
private:
	ModelRender m_modelRender;                     //モデルレンダー
	LevelRender m_levelRender;                     //レベルレンダー
	PhysicsStaticObject* m_physicsStaticObject;    //静的物理オブジェクト
	GameCamera* m_gameCamera;
	SoundSource* m_bgm = nullptr;
	Fade* m_fade = nullptr;				           //フェード。
	int m_numEnemy = 0;
	int m_numDefeatedEnemy = 0;
	bool       m_isWaitFadeout = false;
};

