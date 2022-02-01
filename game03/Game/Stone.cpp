#include "stdafx.h"
#include "Stone.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいので、ファイルをインクルードする。
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

Stone::Stone()
{

}

Stone::~Stone()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool Stone::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(7, u"Assets/effect/efk/magic_boss.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(7);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 50.0f);
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
	collisionPosition = m_position;
	//collisionPosition.y += 1000.0f;
	//球状のコリジョンを作成する。
	m_collisionObject->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(600.0f, 50.0f, 600.0f)
	);
	
	//術者がエネミーだったら。
	 if (m_enMagician == enMagician_Boss)
	{
		//名前をenemy_fireballにする。
		m_collisionObject->SetName("boss_stone");
	}

	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/magic4.wav");
	////効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(13);
	se->Play(false);
	se->SetVolume(0.4f);
	return true;
}

void Stone::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが1.5f以上だったら。
	if (m_timer >= 1.5f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}