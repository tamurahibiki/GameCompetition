#include "stdafx.h"
#include "FireBall.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObject���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

FireBall::FireBall()
{

}

FireBall::~FireBall()
{
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//�G�t�F�N�g���폜����B
	DeleteGO(m_effectEmitter);
}

bool FireBall::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/magic_fire.efk");

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(0);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 20.0f);
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
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(
		m_position,
		Quaternion::Identity, 
		45.0f * m_scale.z
	);
	//�p�҂��v���C���[��������B
	if (m_enMagician == enMagician_Player)
	{
		//���O��player_fireball�ɂ���B
		m_collisionObject->SetName("player_fireball");
	}
	

	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(4,"Assets/sound/magic1.wav");
	////���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(4);
	se->Play(false);
	se->SetVolume(0.3f);
	return true;
}

void FireBall::Update()
{
	//���W���ړ�������B
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	
	//�G�t�F�N�g�̍��W��ݒ肷��B
	m_effectEmitter->SetPosition(m_position);
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	m_collisionObject->SetPosition(m_position);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��1.0f�ȏゾ������B
	if (m_timer >= 1.0f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}
