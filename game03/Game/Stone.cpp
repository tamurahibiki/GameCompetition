#include "stdafx.h"
#include "Stone.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObject���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

Stone::Stone()
{

}

Stone::~Stone()
{
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//�G�t�F�N�g���폜����B
	DeleteGO(m_effectEmitter);
}

bool Stone::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(7, u"Assets/effect/efk/magic_boss.efk");

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(7);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 50.0f);
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);
	//��]��ݒ肷��B
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	collisionPosition = m_position;
	//collisionPosition.y += 1000.0f;
	//����̃R���W�������쐬����B
	m_collisionObject->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(600.0f, 50.0f, 600.0f)
	);
	
	//�p�҂��G�l�~�[��������B
	 if (m_enMagician == enMagician_Boss)
	{
		//���O��enemy_fireball�ɂ���B
		m_collisionObject->SetName("boss_stone");
	}

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/magic4.wav");
	////���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(13);
	se->Play(false);
	se->SetVolume(0.4f);
	return true;
}

void Stone::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	//�G�t�F�N�g�̍��W��ݒ肷��B
	m_effectEmitter->SetPosition(m_position);
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	m_collisionObject->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��1.5f�ȏゾ������B
	if (m_timer >= 1.5f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}