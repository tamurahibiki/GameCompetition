#include "stdafx.h"
#include "Miss.h"

#include"Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



#include "graphics/effect/EffectEmitter.h"

Miss::Miss()
{

}

Miss::~Miss()
{

	m_effectEmitter->Stop();
	//エフェクトを削除する。
	DeleteGO(m_effectEmitter);
}

bool Miss::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/efk/magic_miss2.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(4);
	m_effectEmitter->Init(4);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 20.0f);
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 10.0f;
	m_rotation.AddRotationDegY(360.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/miss.wav");
	////効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(16);
	se->Play(false);
	se->SetVolume(0.6f);
	
	return true;
}

void Miss::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが0.7f以上だったら。
	if (m_timer >= 0.7f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}