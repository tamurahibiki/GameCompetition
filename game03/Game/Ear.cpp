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
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//�G�t�F�N�g���폜����B
	DeleteGO(m_effectEmitter);
}

bool Ear::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/magic_ear2.efk");

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(1);
	m_effectEmitter->Init(1);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 40.0f);
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 5000.0f;
	m_rotation.AddRotationDegY(360.0f);
	//��]��ݒ肷��B
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	//�{�b�N�X��̃R���W�������쐬����B
	m_collisionObject->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(700.0f,100.0f,700.0f)
	);
	//�p�҂��v���C���[��������B
	if (m_enMagician == enMagician_Player)
	{
		//���O��player_fireball�ɂ���B
		m_collisionObject->SetName("player_Ear");
	}
	
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(5,"Assets/sound/magic2.wav");
	////���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.1f);
	return true;
}

void Ear::Update()
{
	
	//�G�t�F�N�g�̍��W��ݒ肷��B
	m_effectEmitter->SetPosition(m_position);
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	m_collisionObject->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��0.5f�ȏゾ������B
	if (m_timer >= 0.5f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}
