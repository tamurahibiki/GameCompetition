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
	//�G�t�F�N�g���폜����B
	DeleteGO(m_effectEmitter);
}

bool Miss::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/efk/magic_miss2.efk");

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(4);
	m_effectEmitter->Init(4);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 20.0f);
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 10.0f;
	m_rotation.AddRotationDegY(360.0f);
	//��]��ݒ肷��B
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/miss.wav");
	////���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(16);
	se->Play(false);
	se->SetVolume(0.6f);
	
	return true;
}

void Miss::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//�G�t�F�N�g�̍��W��ݒ肷��B
	m_effectEmitter->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��0.7f�ȏゾ������B
	if (m_timer >= 0.7f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}