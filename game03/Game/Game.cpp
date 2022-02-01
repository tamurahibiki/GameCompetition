#include "stdafx.h"
#include "Game.h"
#include"Kabe.h"
#include"Yuka.h"
#include"Mura.h"
#include"Box.h"
#include"Player.h"
#include"Enemy.h"
#include"Boss.h"
#include"GameCamera.h"
#include "SkyCube.h"
#include"Game_Over.h"
#include"Game_Clear.h"
#include "Fade.h"

Game::Game() 
{

}
Game::~Game()
{
	//�J�������폜����B
	DeleteGO(FindGO<GameCamera>("gamecamera"));
	//�w�i���폜����B
	DeleteGO(FindGO<SkyCube>("skyCube"));
	//�ǂ��폜����B
	DeleteGO(FindGO<Kabe>("kabe"));
	//�����폜����B
	const auto& yukas = FindGOs<Yuka>("yuka");
	for (auto yuka : yukas)
	{
		DeleteGO(yuka);
	}
	//BOX���폜����B
	DeleteGO(FindGO<Box>("box"));
	//�����폜����B
	DeleteGO(FindGO<Mura>("mura"));
	//�v���C���[���폜����B
	DeleteGO(FindGO<Player>("player"));
	//�G�l�~�[���폜����B
	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
	//�{�X���폜����B
	DeleteGO(FindGO<Boss>("boss"));
	//���y���폜����B
	DeleteGO(m_bgm);
};

bool Game::Start()
{
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D->SetFar(40000.0f);
	//�J�����̃I�u�W�F�N�g���쐬����B
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//�w�i���쐬����B
	SkyCube* skyCube = NewGO<SkyCube>(0, "skyCube");

	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/stage1.tkl",
		[&](LevelObjectData& objData) {
			//��
			if (objData.EqualObjectName(L"kabe") == true) {
				Kabe* kabe = NewGO<Kabe>(0, "kabe");
				kabe->SetPosition(objData.position);
				kabe->SetScale(objData.scale);
				kabe->SetRotation(objData.rotation);
				return true;
			}
			//��
			else if (objData.EqualObjectName(L"yuka") == true) {
				Yuka* yuka = NewGO<Yuka>(0, "yuka");
				yuka->SetPosition(objData.position);
				yuka->SetScale(objData.scale);
				yuka->SetRotation(objData.rotation);
			    return true;
			}
			//BOX
			else if (objData.EqualObjectName(L"box") == true) {
				Box* box = NewGO<Box>(0, "box");
				box->SetPosition(objData.position);
				box->SetScale(objData.scale);
				box->SetRotation(objData.rotation);
				return true;
			}
			//��
			else if (objData.EqualObjectName(L"mura") == true) {
				Mura* mura = NewGO<Mura>(0, "mura");
				mura->SetPosition(objData.position);
				mura->SetScale(objData.scale);
				mura->SetRotation(objData.rotation);
				return true;
			}
			//�v���C���[
			else if (objData.EqualObjectName(L"player") == true) {
				Player* player = NewGO<Player>(0, "player");
				player->SetPosition(objData.position);
				player->SetScale(objData.scale);
				player->SetRotation(objData.rotation);
				return true;
			}
			//�G�l�~�[
			else if (objData.EqualObjectName(L"enemy") == true) {
				Enemy* enemy = NewGO<Enemy>(0, "enemy");
				enemy->SetPosition(objData.position);
				enemy->SetScale(objData.scale);
				m_numEnemy++;
				return true;
			}
			//�{�X
			else if (objData.EqualObjectName(L"boss") == true) {
					Boss* boss = NewGO<Boss>(0, "boss");
					boss->SetPosition(objData.position);
					boss->SetScale(objData.scale);
					m_numEnemy++;
					return true;
			}
			return true;
		});
	//�����蔻���L��������B
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	//���y���Đ�����
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/batolu2.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(1);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);

    return true;
}

void Game::NotifyGameClear()
{
	NewGO<Game_Clear>(0, "game_clear");
	//���g���폜����B
	DeleteGO(this);
}

void Game::NotifyGameOver()
{	
	NewGO<Game_Over>(0, "game_over");
	//���g���폜����B
	DeleteGO(this);
}

void Game::Update()
{
	
}
