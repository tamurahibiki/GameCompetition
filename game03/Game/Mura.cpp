#include "stdafx.h"
#include "Mura.h"
#include"Game.h"
#include"Player.h"

#include "collision/CollisionObject.h"



namespace
{
	//最大HP。
	const int MAXIMUM_HP = 100;
}

Mura::Mura()
{

}

Mura::~Mura()
{
	DeleteGO(m_collisionObject);
}

bool Mura::Start()
{
	//更新処理
	m_hp = MAXIMUM_HP;
	
	m_waku.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku.SetPivot(Vector2(0.0f, 0.5f));
	m_waku.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku.Update();

	m_waku2.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku2.SetPivot(Vector2(0.0f, 0.5f));
	m_waku2.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku2.Update();

	m_waku3.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku3.SetPivot(Vector2(0.0f, 0.5f));
	m_waku3.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku3.Update();

	m_waku4.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku4.SetPivot(Vector2(0.0f, 0.5f));
	m_waku4.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku4.Update();

	m_modelRender.Init("Assets/modelData/ie.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//コリジョンオブジェクトを作成する。

	//ボックス状のコリジョンを作成する。
	Vector3 collisonPosition = m_position;

	collisonPosition.z -= 1100.0f;
	collisonPosition.y += 300.0f;

	m_collisionObject = NewGO<CollisionObject>(0);

	m_collisionObject->CreateBox(
		collisonPosition,
		Quaternion::Identity,
		Vector3 (2000.0f, 700.0f, 500.0f)
	);
	m_collisionObject->SetName("mura");
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	//音楽を読み込む。
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/hit3.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");

	m_game = FindGO<Game>("game");
	return true;
}

void Mura::Update()
{
	//HPが10以下なら
	if (m_hp <= 10)
	{
		//タイマーを加算する。
		m_timer5 += g_gameTime->GetFrameDeltaTime();
		//テキストの設定
		fontRender4.SetText(L"村の残り耐久値 10％");
		fontRender4.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender4.SetScale(1.5f);
		fontRender4.SetColor(g_vec4Red);
		//タイマーを加算する。
		m_timer9 += g_gameTime->GetFrameDeltaTime();
		m_waku4.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku4.Update();
		
	}
	//HPが30以下なら
	else if (m_hp <= 30)
	{
		//タイマーを加算する。
		m_timer4 += g_gameTime->GetFrameDeltaTime();
		//テキストの設定
		fontRender3.SetText(L"村の残り耐久値 30％");
		fontRender3.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender3.SetScale(1.5f);
		fontRender3.SetColor(g_vec4Oreng);
		//タイマーを加算する。
		m_timer8 += g_gameTime->GetFrameDeltaTime();
		m_waku3.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku3.Update();
	}
	//HPが50以下なら
	else if (m_hp <= 50)
	{
		//タイマーを加算する。
		m_timer3 += g_gameTime->GetFrameDeltaTime();
		//テキストの設定
		fontRender2.SetText(L"村の残り耐久値 50％");
		fontRender2.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender2.SetScale(1.5f);
		fontRender2.SetColor(g_vec4Yellow);
		//タイマーを加算する。
		m_timer7 += g_gameTime->GetFrameDeltaTime();
		m_waku2.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku2.Update();
	}
	//HPが70以下なら
	else if (m_hp <= 70)
	{
		//タイマーを加算する。
		m_timer2 += g_gameTime->GetFrameDeltaTime();
		//テキストの設定
		fontRender1.SetText(L"村の残り耐久値 70％");
		fontRender1.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender1.SetScale(1.5f);
		fontRender1.SetColor(g_vec4White);
		//タイマーを加算する。
		m_timer6 += g_gameTime->GetFrameDeltaTime();
		m_waku.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku.Update();
	}
	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();
	//タイマーが2.0fを超えたら
	if (m_timer >= 2.0f)
	{
		//ステートを待機にする。
		m_MuraState = enMuraState_Idle;
		//タイマーを0に戻す。
		m_timer = 0.0f;
	}
	//当たり判定
	Collision();
	//絵描きさんの更新処理。
	m_modelRender.Update();
}



void Mura::Collision()
{
	//ダメージステートの時
	if (m_MuraState==enMuraState_Damage)
	{
		//何も起こらない
		return;
	}
	//エネミーの攻撃が当たった時
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		if (collision->IsHit(m_collisionObject)==true)
		{
			//HPを10減らす
			m_hp -= 10;
			
			//HPが0なら
			if (m_hp == 0)
			{
				//自身を削除する。
				m_game->NotifyGameOver();
			}
			//HPが0以外なら
			else 
			{
				//ステートをダメージにする。
				m_MuraState = enMuraState_Damage;
				//音楽を再生する
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(9);
				se->Play(false);
				se->SetVolume(0.4f);
			}
			return;
		}
	}
}

void Mura::ManageState()
{
	switch (m_MuraState)
	{
		//待機ステートの時。
	case enMuraState_Idle:
		break;

		//被ダメージステートの時。
	case enMuraState_Damage:
		break;
	}
}


void Mura::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	
	if (m_timer6 <= 2.0f)
	{
		m_waku.Draw(rc);
	}
	
	if (m_timer7 <= 2.0f)
	{
		m_waku2.Draw(rc);
	}
	if (m_timer8 <= 2.0f)
	{
		m_waku3.Draw(rc);
	}
	
	if (m_timer9 <= 2.0f)
	{
		m_waku4.Draw(rc);
	}

	if (m_timer2 <= 2.0f)
	{
		fontRender1.Draw(rc);
	}

	if (m_timer3 <= 2.0f)
	{
		fontRender2.Draw(rc);

	}
	if (m_timer4 <= 2.0f)
	{
		fontRender3.Draw(rc);

	}
	if (m_timer5 <= 2.0f)
	{
		fontRender4.Draw(rc);

	}
}