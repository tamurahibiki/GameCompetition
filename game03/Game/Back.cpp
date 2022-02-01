#include "stdafx.h"
#include "Back.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいので、ファイルをインクルードする。
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

Back::Back()
{

}

Back::~Back()
{
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool Back::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(6, u"Assets/effect/efk/magic_Back.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(6);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale *80.0f);
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(180.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	m_position2= m_position;
	m_position2.z -= 40.0f;
	//球状のコリジョンを作成する。
	m_collisionObject->CreateBox(
		m_position2,
		Quaternion::Identity,
		Vector3(2000.0f, 50.0f, 2000.0f)
	);
	//術者がプレイヤーだったら。
	if (m_enMagician == enMagician_Player)
	{
		//名前をplayer_fireballにする。
		m_collisionObject->SetName("player_Back");
	}
	
	//コリジョンオブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(6,"Assets/sound/magic3.wav");
	////効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(6);
	se->Play(false);
	se->SetVolume(0.4f);
	return true;
}

void Back::Update()
{

	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);

	m_position2 += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position2);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが1.5f以上だったら。
	if (m_timer >= 1.5f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}