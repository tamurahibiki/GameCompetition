#include "stdafx.h"
#include "FireBall.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいので、ファイルをインクルードする。
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

FireBall::FireBall()
{

}

FireBall::~FireBall()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool FireBall::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/magic_fire.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(0);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 20.0f);
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(
		m_position,
		Quaternion::Identity, 
		45.0f * m_scale.z
	);
	//術者がプレイヤーだったら。
	if (m_enMagician == enMagician_Player)
	{
		//名前をplayer_fireballにする。
		m_collisionObject->SetName("player_fireball");
	}
	

	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(4,"Assets/sound/magic1.wav");
	////効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(4);
	se->Play(false);
	se->SetVolume(0.3f);
	return true;
}

void FireBall::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが1.0f以上だったら。
	if (m_timer >= 1.0f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}
