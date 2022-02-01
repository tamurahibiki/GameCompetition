#include "stdafx.h"
#include "Back.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObject���g�p�������̂ŁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

#include "graphics/effect/EffectEmitter.h"

Back::Back()
{

}

Back::~Back()
{
	//�R���W�����I�u�W�F�N�g���폜����B
	DeleteGO(m_collisionObject);

	m_effectEmitter->Stop();
	//�G�t�F�N�g���폜����B
	DeleteGO(m_effectEmitter);
}

bool Back::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(6, u"Assets/effect/efk/magic_Back.efk");

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(6);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale *80.0f);
	//�ړ����x���v�Z�B
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(180.0f);
	//��]��ݒ肷��B
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);

	m_position2= m_position;
	m_position2.z -= 40.0f;
	//����̃R���W�������쐬����B
	m_collisionObject->CreateBox(
		m_position2,
		Quaternion::Identity,
		Vector3(2000.0f, 50.0f, 2000.0f)
	);
	//�p�҂��v���C���[��������B
	if (m_enMagician == enMagician_Player)
	{
		//���O��player_fireball�ɂ���B
		m_collisionObject->SetName("player_Back");
	}
	
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(6,"Assets/sound/magic3.wav");
	////���ʉ����Đ�����B
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(6);
	se->Play(false);
	se->SetVolume(0.4f);
	return true;
}

void Back::Update()
{

	//�G�t�F�N�g�̍��W��ݒ肷��B
	m_effectEmitter->SetPosition(m_position);

	m_position2 += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��B
	m_collisionObject->SetPosition(m_position2);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��1.5f�ȏゾ������B
	if (m_timer >= 1.5f)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}