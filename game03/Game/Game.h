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
	/// �Q�[���N���A�[��ʒm����B
	/// </summary>
	void NotifyGameClear();
	/// <summary>
	/// �Q�[���I�[�o�[��ʒm����B
	/// </summary>
	void NotifyGameOver();
	/// <summary>
	/// <returns>�G�l�~�[���S�ł��Ă�����true�B</returns>
	const bool IsWannihilationEnemy() const
	{
		return m_numEnemy == m_numDefeatedEnemy;
	}
	/// <summary>
	/// �|���ꂽ�G�̐���+1����B
	/// </summary>
	void AddDefeatedEnemyNumber()
	{
		m_numDefeatedEnemy++;
	}
private:
	ModelRender m_modelRender;                     //���f�������_�[
	LevelRender m_levelRender;                     //���x�������_�[
	PhysicsStaticObject* m_physicsStaticObject;    //�ÓI�����I�u�W�F�N�g
	GameCamera* m_gameCamera;
	SoundSource* m_bgm = nullptr;
	Fade* m_fade = nullptr;				           //�t�F�[�h�B
	int m_numEnemy = 0;
	int m_numDefeatedEnemy = 0;
	bool       m_isWaitFadeout = false;
};

