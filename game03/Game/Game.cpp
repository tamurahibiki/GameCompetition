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
	//カメラを削除する。
	DeleteGO(FindGO<GameCamera>("gamecamera"));
	//背景を削除する。
	DeleteGO(FindGO<SkyCube>("skyCube"));
	//壁を削除する。
	DeleteGO(FindGO<Kabe>("kabe"));
	//床を削除する。
	const auto& yukas = FindGOs<Yuka>("yuka");
	for (auto yuka : yukas)
	{
		DeleteGO(yuka);
	}
	//BOXを削除する。
	DeleteGO(FindGO<Box>("box"));
	//村を削除する。
	DeleteGO(FindGO<Mura>("mura"));
	//プレイヤーを削除する。
	DeleteGO(FindGO<Player>("player"));
	//エネミーを削除する。
	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
	//ボスを削除する。
	DeleteGO(FindGO<Boss>("boss"));
	//音楽を削除する。
	DeleteGO(m_bgm);
};

bool Game::Start()
{
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D->SetFar(40000.0f);
	//カメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//背景を作成する。
	SkyCube* skyCube = NewGO<SkyCube>(0, "skyCube");

	//レベルを構築する。
	m_levelRender.Init("Assets/level3D/stage1.tkl",
		[&](LevelObjectData& objData) {
			//壁
			if (objData.EqualObjectName(L"kabe") == true) {
				Kabe* kabe = NewGO<Kabe>(0, "kabe");
				kabe->SetPosition(objData.position);
				kabe->SetScale(objData.scale);
				kabe->SetRotation(objData.rotation);
				return true;
			}
			//床
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
			//村
			else if (objData.EqualObjectName(L"mura") == true) {
				Mura* mura = NewGO<Mura>(0, "mura");
				mura->SetPosition(objData.position);
				mura->SetScale(objData.scale);
				mura->SetRotation(objData.rotation);
				return true;
			}
			//プレイヤー
			else if (objData.EqualObjectName(L"player") == true) {
				Player* player = NewGO<Player>(0, "player");
				player->SetPosition(objData.position);
				player->SetScale(objData.scale);
				player->SetRotation(objData.rotation);
				return true;
			}
			//エネミー
			else if (objData.EqualObjectName(L"enemy") == true) {
				Enemy* enemy = NewGO<Enemy>(0, "enemy");
				enemy->SetPosition(objData.position);
				enemy->SetScale(objData.scale);
				m_numEnemy++;
				return true;
			}
			//ボス
			else if (objData.EqualObjectName(L"boss") == true) {
					Boss* boss = NewGO<Boss>(0, "boss");
					boss->SetPosition(objData.position);
					boss->SetScale(objData.scale);
					m_numEnemy++;
					return true;
			}
			return true;
		});
	//当たり判定を有効化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	//音楽を再生する
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
	//自身を削除する。
	DeleteGO(this);
}

void Game::NotifyGameOver()
{	
	NewGO<Game_Over>(0, "game_over");
	//自身を削除する。
	DeleteGO(this);
}

void Game::Update()
{
	
}
