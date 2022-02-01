#include "stdafx.h"
#include "Boss.h"

#include "Game.h"
#include "Stone.h"
#include "Player.h"
#include"Mura.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include <time.h>
#include <stdlib.h>


#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

bool onlyOnce = true;
bool onlyOnce2 = true;
bool onlyOnce5 = true;
bool onlyOnce6 = true;

Boss::Boss()
{

}

Boss::~Boss()
{

}

bool Boss::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle3_boss.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Idle2].Load("Assets/animData/idle2_boss.tka");
	m_animationClips[enAnimationClip_Idle2].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Idle3].Load("Assets/animData/idle_boss.tka");
	m_animationClips[enAnimationClip_Idle3].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run_boss.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/attack_boss.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Magic].Load("Assets/animData/magic_boss.tka");
	m_animationClips[enAnimationClip_Magic].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/damage_boss.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down_boss.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down2].Load("Assets/animData/down2_boss.tka");
	m_animationClips[enAnimationClip_Down2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Stand].Load("Assets/animData/stand_boss.tka");
	m_animationClips[enAnimationClip_Stand].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Stand2].Load("Assets/animData/stand2_boss.tka");
	m_animationClips[enAnimationClip_Stand2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Fall].Load("Assets/animData/Fall_boss.tka");
	m_animationClips[enAnimationClip_Fall].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/boss.tkm", m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	Vector3 position = m_position;
	position.y += 2.5f;
	m_modelRender.SetPosition(position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		65.0f,			//���a�B
		250.0f,			//�����B
		m_position		//���W�B
	);

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/up_boss.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/down_boss.efk");
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/magic_boss_effect.efk");
	
	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/attack.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/hit2.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/monster.wav");
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_player = FindGO<Player>("player");
	m_mura = FindGO<Mura>("mura");
	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Boss::Update()
{
	Move();
	//�ǐՏ����B
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	//���f���̍X�V�B
	m_modelRender.Update();
}
void Boss:: Move() 
{
	if (m_charaCon.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
    }
	else
	{
		m_moveSpeed.y -= 10.0f;
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		if (onlyOnce5==true) {
			m_bossState = enBossState_Fall;
		}
	}
	Vector3 modelPosition = m_position;
	//modelPosition.y += 2.5;
	m_modelRender.SetPosition(modelPosition);
}

void Boss::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Boss::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_bossState != enBossState_Chase)
	{
		return;
	}
	
	if (m_charaCon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	Vector3 modelPosition = m_position;
	modelPosition.y += 2.5;
	m_modelRender.SetPosition(modelPosition);

}

void Boss::MakeStone()
{
	Stone* stone = NewGO<Stone>(0);
	Vector3 stonePosition = m_position;
	stonePosition.y += 30.0f;
	stone->SetPosition(stonePosition);
	stone->SetRotation(m_rotation);
	stone->SetEnMagician(Stone::enMagician_Boss);
}

void Boss::Collision()
{
	if (m_bossState == enBossState_Damage ||
		m_bossState == enBossState_Down||
		m_bossState == enBossState_Stand||
		m_bossState == enBossState_Idle3||
		m_bossState == enBossState_Down2)
	{
		return;
	}
	//�����@������������
	const auto& collision1 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
	for (auto collision : collision1)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			if (m_hp == 0)
			{
				Down();
			}
			else if(m_hp == 5)
			{
				m_bossState = enBossState_Down2;
				
			}
			else {
				m_bossState = enBossState_Damage;
				
			}
		
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(12);
			se->Play(false);
			se->SetVolume(0.5f);
			return;
		}
	}
	//�����@������������
	const auto& collision2 = g_collisionObjectManager->FindCollisionObjects("player_Ear");
	for (auto collision : collision2)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			if (m_hp == 0)
			{
				Down();
			}
			else if (m_hp == 5)
			{
				m_bossState = enBossState_Down2;
			}
			else {
				m_bossState = enBossState_Damage;
			}
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(12);
			se->Play(false);
			se->SetVolume(0.5f);
			return;
		}
	}
	//���~�ߖ��@������������
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("player_Back");
	for (auto collision : collision3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_timer += g_gameTime->GetFrameDeltaTime();

			//�^�C�}�[��420.0f�ȉ���������B
			if (m_timer <= 420.0f)
			{
				m_moveSpeed.x = 0.0f;
				m_moveSpeed.z = 0.0f;

				m_timer = 0.0f;
			}
			return;
		}
	}
}

void Boss::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_bossState != enBossState_Attack)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(11);
		se->Play(false);
		se->SetVolume(0.4f);
	}
}


void Boss::MakeAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition.y += 300.0f;
	collisionPosition += m_forward * 180.0f;
	collisionObject->CreateSphere(collisionPosition,
		Quaternion::Identity,
		190.0f
	);
	collisionObject->SetName("boss_attack");
}


const bool Boss::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	if (diff.LengthSq() <= 5000.0 * 5000.0f)
	{
		diff.Normalize();
		float cos = m_forward.Dot(diff);
		float angle = acosf(cos);
		if (angle <= (Math::PI / 360.0f) * 360.0f)
		{
			return true;
		}
	}
	return false;
}

const bool Boss::SearchMura()const
{
	Vector3 diff1 = m_mura->GetPosition() - m_position;
	if (m_charaCon.IsOnGround())
	{
		if (diff1.LengthSq() <= 10000.0 * 10000.0f)
		{
			diff1.Normalize();
			float cos = m_forward.Dot(diff1);
			float angle = acosf(cos);
			if (angle <= (Math::PI / 360.0f) * 360.0f)
			{
				return true;
			}
		}
	}
	return false;
}
void Boss::ProcessCommonStateTransition()
{
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	Vector3 diff = m_player->GetPosition() - m_position;
	Vector3 diff1 = m_mura->GetPosition() - m_position;

	//�v���C���[���������Ƃ�
	if (SearchPlayer() == true)
	{
		diff.Normalize();
		//�U�����͂��Ȃ��Ƃ�
		if (IsCanAttack() == false)
		{
			int ram = rand() % 100;
		
			if (ram > 40)
			{
				onlyOnce6 = true;
				m_moveSpeed = diff * 580.0f;
				m_bossState = enBossState_Chase;
				return;
			}
			else {
				
				m_bossState = enBossState_Magic;
				return;
			}
		}
		//�{���Ԃ̂Ƃ�
		else if (m_end == true)
		{
			int ram = rand() % 100;
			if (ram > 20)
			{
				m_bossState = enBossState_Attack;
				return;
			}
			else
			{
				m_bossState = enBossState_Idle2;
			}	
		}
		//�ʏ��Ԃ̂Ƃ�
		else
		{
			int ram = rand() % 100;
			if (ram > 30)
			{
				m_bossState = enBossState_Attack;
				return;
			}
			else
			{
				m_bossState = enBossState_Idle2;
			}
		}
	}
	//�����������Ƃ�
	else if(SearchMura()== true)
	{
		    diff1.Normalize();
	 //�U�����͂��Ȃ�       
	 if (IsCanAttack2() == false)
	  {
			 m_moveSpeed = diff1 * 250.0f;
			 m_bossState = enBossState_Chase;
			 return;
	   }
	 //�{���Ԃ̂Ƃ�
	    else if (m_end == true)
	   {
		int ram = rand() % 100;
		if (ram > 25)
		{
			m_bossState = enBossState_Attack;
			return;
		}
		else
		{
			m_bossState = enBossState_Idle2;
		}
	   }
	 //�ʏ��Ԃ̂Ƃ�
	   else
	   {
		int ram = rand() % 100;
		if (ram > 30)
		{
			m_bossState = enBossState_Attack;
			return;
		}
		else
		{
			m_bossState = enBossState_Idle2;
		}
	   }
   }
	//��L�ȊO�̂Ƃ�
	else {
		m_bossState = enBossState_Idle;
	}

}

void Boss::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}


void Boss::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Boss::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessMagicAttackStateTransition()
{
	if (onlyOnce6 == true) {
		MakeEffectMagic();
		onlyOnce6 = false;
	}
	//���@�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_bossState = enBossState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
	}
}
void Boss::ProcessIdle2StateTransition() 
{
	//�U�����ҋ@�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
	    m_end = true;
		m_bossState = enBossState_Chase;
	}
	if (onlyOnce) {
		MakeEffect();
		onlyOnce = false;
	}
}

void Boss::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		game->AddDefeatedEnemyNumber();
		//���g���폜����B
		DeleteGO(this);
		if (onlyOnce) {
			onlyOnce = false;
			MakeEffectDown();
		}
	}
}
void Boss::ProcessDown2StateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Stand;
	}
}

void Boss::ProcessStandStateTransition()
{
	//�N���オ��1�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Idle3;
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(15);
		se->Play(false);
		se->SetVolume(0.5f);
	}		
}
void Boss::ProcessStand2StateTransition()
{
	//�N���オ��2�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Idle;
	}
}
void Boss::ProcessFallStateTransition()
{
	//�����A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Stand2;
	}
}

void Boss::ManageState()
{
	switch (m_bossState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enBossState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�U�����ҋ@�X�e�[�g�̎��B
	case enBossState_Idle2:
		ProcessIdleStateTransition();
		break;
		//����ҋ@�X�e�[�g�̎��B
	case enBossState_Idle3:
		ProcessIdle2StateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enBossState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//���@�U���X�e�[�g�̎��B
	case enBossState_Magic:
		ProcessMagicAttackStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enBossState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enBossState_Damage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enBossState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
		//�_�E���X�e�[�g2�̎��B
	case enBossState_Down2:
		//�_�E���X�e�[�g2�̃X�e�[�g�J�ڏ����B
		ProcessDown2StateTransition();
		break;
		//�N���オ��1�X�e�[�g�̎��B
	case enBossState_Stand:
		ProcessStandStateTransition();
		break;
		//�N���オ��2�X�e�[�g�̎��B
	case enBossState_Stand2:
		ProcessStand2StateTransition();
		onlyOnce5 = false;
		break;
		//�����X�e�[�g�̎��B
	case enBossState_Fall:
		ProcessFallStateTransition();
		break;
	}
}

void Boss::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_bossState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enBossState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�U���ҋ@�X�e�[�g�̎��B
	case enBossState_Idle2:
		//�U���ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle2, 0.5f);
		break;
		//�{��ҋ@�X�e�[�g�̎��B
	case enBossState_Idle3:
		//�{��ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle3, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enBossState_Chase:
		m_modelRender.SetAnimationSpeed(1.5f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enBossState_Attack:
		m_modelRender.SetAnimationSpeed(1.3f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//���@�U���X�e�[�g�̎��B
	case enBossState_Magic:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enBossState_Damage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.05f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enBossState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.05f);
		break;
		//�_�E���X�e�[�g2�̎��B
	case enBossState_Down2:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down2, 0.05f);
		break;
		//�����オ��1�X�e�[�g1�̎��B
	case enBossState_Stand:
		//�����オ��1�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Stand, 0.05f);
		break;
		//�����オ��2�X�e�[�g�̎��B
	case enBossState_Stand2:
		m_modelRender.SetAnimationSpeed(1.3f);
		//�����オ��2�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Stand2, 0.01f);
		break;
		//�����X�e�[�g�̎��B
	case enBossState_Fall:
		////�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Fall, 1.0f);
		break;
	default:
		break;
	}
}
void Boss::MakeEffect()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(2);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 120.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Boss::MakeEffectDown()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(3);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 50.0f);
	Vector3 effectPosition = m_position;
	effectPosition.x += 0.0f;
	effectPosition.y += 600.0f;
	effectPosition.z += 0.0f;
	m_effectEmitter->SetPosition(effectPosition);
	Quaternion rotation;
	rotation = m_rotation;
	rotation.AddRotationDegX(180.0f);
	rotation.AddRotationDegY(0.0f);
	rotation.AddRotationDegZ(180.0f);
	m_effectEmitter->SetRotation(rotation);
	m_effectEmitter->Play();
}


void Boss::MakeEffectMagic()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(11);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 40.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}


const bool Boss::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	Vector3 diff1 = m_mura->GetPosition() - m_position;
	if (diff.LengthSq() <= 300.0f * 300.0f)
	{
		return true;
	}
	if (diff1.LengthSq() <= 1500.0f * 1500.0f)
	{
		return true;
	}

	return false;
}


const bool Boss::IsCanAttack2() const
{
	Vector3 diff1 = m_mura->GetPosition() - m_position;
	if (diff1.LengthSq() <= 1600.0f * 1600.0f)
	{
		return true;
	}

	return false;
}

void Boss::Down()
{
	m_bossState = enBossState_Down;
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(15);
	se->Play(false);
	se->SetVolume(0.5f);
}


void Boss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	if (wcscmp(eventName, L"attack_start") == 0) {
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0) {
		m_isUnderAttack = false;
		onlyOnce = true;
	}
	else if (wcscmp(eventName, L"magic_stone") == 0) {
		
		MakeStone();
	}
}


void Boss::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}
