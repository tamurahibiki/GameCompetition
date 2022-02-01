#include "stdafx.h"
#include "Ear.h"
#include"Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

Ear::Ear()
{

}

Ear::~Ear()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool Ear::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/magic_ear2.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(1);
	m_effectEmitter->Init(1);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 40.0f);
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 5000.0f;
	m_rotation.AddRotationDegY(360.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//ボックス状のコリジョンを作成する。
	m_collisionObject->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(700.0f,100.0f,700.0f)
	);
	//術者がプレイヤーだったら。
	if (m_enMagician == enMagician_Player)
	{
		//名前をplayer_fireballにする。
		m_collisionObject->SetName("player_Ear");
	}
	
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(5,"Assets/sound/magic2.wav");
	////効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.1f);
	return true;
}

void Ear::Update()
{
	
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが0.5f以上だったら。
	if (m_timer >= 0.5f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}
