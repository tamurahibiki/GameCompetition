#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "FireBall.h"
#include "Player.h"
#include"Mura.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include <time.h>
#include <stdlib.h>

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
bool onlyOnce4 = true;
bool onlyOnce7 = false;
Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle_enemy.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run_enemy.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/attack_enemy.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/damage_enemy.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down_enemy.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/enemy.tkm",m_animationClips, enAnimationClip_Num);

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
		40.0f,			//���a�B
		100.0f,			//�����B
		m_position		//���W�B
	);

	//�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/efk/down_enemy.efk");

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/hit2.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/attack_enemy.wav");
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

void Enemy::Update()
{
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


void Enemy::Rotation()
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

void Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
    }
	Vector3 modelPosition = m_position;
	modelPosition.y += 2.5;
	m_modelRender.SetPosition(modelPosition);

}

void Enemy::Collision()
{
	//�ȉ��̃X�e�[�g�̂Ƃ�
	if (m_enemyState == enEnemyState_Damage ||
		m_enemyState == enEnemyState_Down)
	{
		//�����N����Ȃ�
		return;
	}
	//�����@�ɓ���������
	const auto& collision1 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
	for (auto collision : collision1)
	{
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷
			m_hp -= 1;
			//HP��0�Ȃ�
			if (m_hp == 0)
			{
				Down();
			}
			//HP��0�ȊO�Ȃ�
			else 
			{
				Damage();
			}
			
			return;
		}
	}
	//�����@�ɓ���������
	const auto& collision2 = g_collisionObjectManager->FindCollisionObjects("player_Ear");
	for (auto collision : collision2)
	{
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷
			m_hp -= 1;
			//HP��0�Ȃ�
			if (m_hp == 0)
			{
				Down();
			}
			//HP��0�ȊO�Ȃ�
			else 
			{
				Damage();
			}
			
			return;
		}
	}
	//���~�ߖ��@�ɓ���������
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("player_Back");
	for (auto collision : collision3)
	{
		if (collision->IsHit(m_charaCon))
		{
			//�G�l�~�[�̈ړ����x��0�ɂ���B
					m_moveSpeed.x = 0.0f;
					m_moveSpeed.z = 0.0f;
		}
		return;
	}
}

void Enemy::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
		//���y���Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(14);
		se->Play(false);
		se->SetVolume(0.05f);
	}
}
void Enemy::MakeAttackCollision()
{
	//�R���W�������쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;

	collisionPosition += m_forward * 140.0f;

	collisionObject->CreateSphere(collisionPosition,
		Quaternion::Identity,
		100.0f
	);
	collisionObject->SetName("enemy_attack");
}

const bool Enemy::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	if (diff.LengthSq() <= 1000.0 * 1000.0f)
	{
		diff.Normalize();
		float cos = m_forward.Dot(diff);
		float angle = acosf(cos);
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			return true;
		}
	}
	return false;
}

const bool Enemy::SearchMura()const
{
	Vector3 diff1 = m_mura->GetPosition() - m_position;

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
	return false;
}
void Enemy::ProcessCommonStateTransition()
{
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;
	
	Vector3 diff = m_player->GetPosition() - m_position;
	Vector3 diff1 = m_mura->GetPosition() - m_position;
	
	if (SearchPlayer() == true)
	{
		//�U���ł��Ȃ������Ȃ�
		if (IsCanAttack()==false)
		{
			diff.Normalize();
			m_moveSpeed = diff * 250.0f;
			//�X�e�[�g��ǐՂɂ���
			m_enemyState = enEnemyState_Chase;
			return;
		}
		//�U���ł��鋗���Ȃ�
		else
		{
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�X�e�[�g���U���ɂ���
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//�X�e�[�g��ҋ@�ɂ���B
				m_enemyState = enEnemyState_Idle;
			}
		}
	}
	else if (SearchMura() == true)
	{
		diff1.Normalize();
		
		//�U���ł��Ȃ������Ȃ�
		if (IsCanAttack2() == false)
		{
			m_moveSpeed = diff1 * 250.0f;
			//�X�e�[�g��ǐՂɂ���
			m_enemyState = enEnemyState_Chase;
			return;
		}
		//�U���ł��鋗���Ȃ�
		else
		{
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�X�e�[�g���U���ɂ���
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//�X�e�[�g��ҋ@�ɂ���B
				m_enemyState = enEnemyState_Idle;
			}
		}
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//�^�C�}�[�����Z����B
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}
}


void Enemy::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
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
	if (m_chaseTimer >= 1.5f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
	}
}

void Enemy::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�G�t�F�N�g���Ăяo��
		MakeEffectDown();
		Game* game = FindGO<Game>("game");
		game->AddDefeatedEnemyNumber();
		//���g���폜����B
		DeleteGO(this);
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_Damage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_Damage:
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy::MakeEffectDown()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(5);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 80.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	//�G�t�F�N�g���Đ�����
	m_effectEmitter->Play();
}

void Enemy::Damage()
{
	m_enemyState = enEnemyState_Damage;
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(12);
	se->Play(false);
	se->SetVolume(0.5f);
}

void Enemy::Down()
{
	m_enemyState = enEnemyState_Down;
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(10);
	se->Play(false);
	se->SetVolume(0.5f);
}

const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 200.0f * 200.0f)
	{
		return true;
	}
	return false;
}
const bool Enemy::IsCanAttack2() const
{
	Vector3 diff1 = m_mura->GetPosition() - m_position;

	if (diff1.LengthSq() <= 1300.0f * 1300.0f)
	{
		return true;
	}

	return false;
}
void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_start�v�̎�
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U�����ɂ���
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎�
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���I���ɂ���
		m_isUnderAttack = false;
	}
}

void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}