#include "stdafx.h"
#include "Mura.h"
#include"Game.h"
#include"Player.h"

#include "collision/CollisionObject.h"



namespace
{
	//�ő�HP�B
	const int MAXIMUM_HP = 100;
}

Mura::Mura()
{

}

Mura::~Mura()
{
	DeleteGO(m_collisionObject);
}

bool Mura::Start()
{
	//�X�V����
	m_hp = MAXIMUM_HP;
	
	m_waku.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku.SetPivot(Vector2(0.0f, 0.5f));
	m_waku.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku.Update();

	m_waku2.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku2.SetPivot(Vector2(0.0f, 0.5f));
	m_waku2.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku2.Update();

	m_waku3.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku3.SetPivot(Vector2(0.0f, 0.5f));
	m_waku3.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku3.Update();

	m_waku4.Init("Assets/sprite/waku4.dds", 700, 100);
	m_waku4.SetPivot(Vector2(0.0f, 0.5f));
	m_waku4.SetPosition(Vector3(0.0f, 1000.0f, 0.0f));
	m_waku4.Update();

	m_modelRender.Init("Assets/modelData/ie.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();

	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//�R���W�����I�u�W�F�N�g���쐬����B

	//�{�b�N�X��̃R���W�������쐬����B
	Vector3 collisonPosition = m_position;

	collisonPosition.z -= 1100.0f;
	collisonPosition.y += 300.0f;

	m_collisionObject = NewGO<CollisionObject>(0);

	m_collisionObject->CreateBox(
		collisonPosition,
		Quaternion::Identity,
		Vector3 (2000.0f, 700.0f, 500.0f)
	);
	m_collisionObject->SetName("mura");
	//�R���W�����I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���B
	m_collisionObject->SetIsEnableAutoDelete(false);
	//���y��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/hit3.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");

	m_game = FindGO<Game>("game");
	return true;
}

void Mura::Update()
{
	//HP��10�ȉ��Ȃ�
	if (m_hp <= 10)
	{
		//�^�C�}�[�����Z����B
		m_timer5 += g_gameTime->GetFrameDeltaTime();
		//�e�L�X�g�̐ݒ�
		fontRender4.SetText(L"���̎c��ϋv�l 10��");
		fontRender4.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender4.SetScale(1.5f);
		fontRender4.SetColor(g_vec4Red);
		//�^�C�}�[�����Z����B
		m_timer9 += g_gameTime->GetFrameDeltaTime();
		m_waku4.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku4.Update();
		
	}
	//HP��30�ȉ��Ȃ�
	else if (m_hp <= 30)
	{
		//�^�C�}�[�����Z����B
		m_timer4 += g_gameTime->GetFrameDeltaTime();
		//�e�L�X�g�̐ݒ�
		fontRender3.SetText(L"���̎c��ϋv�l 30��");
		fontRender3.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender3.SetScale(1.5f);
		fontRender3.SetColor(g_vec4Oreng);
		//�^�C�}�[�����Z����B
		m_timer8 += g_gameTime->GetFrameDeltaTime();
		m_waku3.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku3.Update();
	}
	//HP��50�ȉ��Ȃ�
	else if (m_hp <= 50)
	{
		//�^�C�}�[�����Z����B
		m_timer3 += g_gameTime->GetFrameDeltaTime();
		//�e�L�X�g�̐ݒ�
		fontRender2.SetText(L"���̎c��ϋv�l 50��");
		fontRender2.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender2.SetScale(1.5f);
		fontRender2.SetColor(g_vec4Yellow);
		//�^�C�}�[�����Z����B
		m_timer7 += g_gameTime->GetFrameDeltaTime();
		m_waku2.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku2.Update();
	}
	//HP��70�ȉ��Ȃ�
	else if (m_hp <= 70)
	{
		//�^�C�}�[�����Z����B
		m_timer2 += g_gameTime->GetFrameDeltaTime();
		//�e�L�X�g�̐ݒ�
		fontRender1.SetText(L"���̎c��ϋv�l 70��");
		fontRender1.SetPosition({ -300.0f,300.0f,0.0f });
		fontRender1.SetScale(1.5f);
		fontRender1.SetColor(g_vec4White);
		//�^�C�}�[�����Z����B
		m_timer6 += g_gameTime->GetFrameDeltaTime();
		m_waku.SetPosition(Vector3(-325.0f, 280.0f, 0.0f));
		m_waku.Update();
	}
	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();
	//�^�C�}�[��2.0f�𒴂�����
	if (m_timer >= 2.0f)
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_MuraState = enMuraState_Idle;
		//�^�C�}�[��0�ɖ߂��B
		m_timer = 0.0f;
	}
	//�����蔻��
	Collision();
	//�G�`������̍X�V�����B
	m_modelRender.Update();
}



void Mura::Collision()
{
	//�_���[�W�X�e�[�g�̎�
	if (m_MuraState==enMuraState_Damage)
	{
		//�����N����Ȃ�
		return;
	}
	//�G�l�~�[�̍U��������������
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		if (collision->IsHit(m_collisionObject)==true)
		{
			//HP��10���炷
			m_hp -= 10;
			
			//HP��0�Ȃ�
			if (m_hp == 0)
			{
				//���g���폜����B
				m_game->NotifyGameOver();
			}
			//HP��0�ȊO�Ȃ�
			else 
			{
				//�X�e�[�g���_���[�W�ɂ���B
				m_MuraState = enMuraState_Damage;
				//���y���Đ�����
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(9);
				se->Play(false);
				se->SetVolume(0.4f);
			}
			return;
		}
	}
}

void Mura::ManageState()
{
	switch (m_MuraState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enMuraState_Idle:
		break;

		//��_���[�W�X�e�[�g�̎��B
	case enMuraState_Damage:
		break;
	}
}


void Mura::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	
	if (m_timer6 <= 2.0f)
	{
		m_waku.Draw(rc);
	}
	
	if (m_timer7 <= 2.0f)
	{
		m_waku2.Draw(rc);
	}
	if (m_timer8 <= 2.0f)
	{
		m_waku3.Draw(rc);
	}
	
	if (m_timer9 <= 2.0f)
	{
		m_waku4.Draw(rc);
	}

	if (m_timer2 <= 2.0f)
	{
		fontRender1.Draw(rc);
	}

	if (m_timer3 <= 2.0f)
	{
		fontRender2.Draw(rc);

	}
	if (m_timer4 <= 2.0f)
	{
		fontRender3.Draw(rc);

	}
	if (m_timer5 <= 2.0f)
	{
		fontRender4.Draw(rc);

	}
}