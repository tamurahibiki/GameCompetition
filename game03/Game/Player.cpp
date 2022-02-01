#include "stdafx.h"
#include "Player.h"
#include"Game.h"
#include"FireBall.h"
#include"Ear.h"
#include"Back.h"
#include"Miss.h"
#include"collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"
#include"sound/SoundEngine.h"
#include"sound/SoundSource.h"


namespace
{
	//�ő�HP�B
	const int MAXIMUM_HP = 10;

	const int MAXIMUM_MP = 300;

	const int MAXIMUM_SP = 1;
}

Player::Player()
{
	
}

Player::~Player()
{

}
bool Player::Start()
{
	//HP�̕\�� ��������
	m_hpBar.Init("Assets/sprite/hpbar3.DDS", 600, 100);
	m_hpBar.SetPivot(Vector2(0.0f, 0.5f));
	m_hpBar.SetPosition(Vector3(-860.0f, -485.0f, 0.0f));
	//�X�V�����B
	m_hpBar.Update();
	m_hp = MAXIMUM_HP;

	m_waku.Init("Assets/sprite/waku1.dds", 600, 100);
	m_waku.SetPivot(Vector2(0.0f, 0.5f));
	m_waku.SetPosition(Vector3(-860.0f, -485.0f, 0.0f));
	m_waku.Update();

	fontRender5.SetText(L"HP");
	fontRender5.SetPosition({ -800.0f,-470.0f,0.0f });
	fontRender5.SetColor(g_vec4White);

	fontRender2.SetText(L"/  10");
	fontRender2.SetPosition({ -570.0f,-470.0f,0.0f });
	fontRender2.SetColor(g_vec4White);

	//�v���C���[�A�C�R��
	m_player1.Init("Assets/sprite/player2.dds", 100, 100);
	m_player1.SetPivot(Vector2(0.0f, 0.5f));
	m_player1.SetPosition(Vector3(-960.0f, -485.0f, 0.0f));
	//�X�V�����B
	m_player1.Update();
	//HP�̕\�� �����܂�

	//MP�̕\�� ��������
	m_mpBar.Init("Assets/sprite/mpbar2.dds", 500,60);
	m_mpBar.SetPivot(Vector2(0.0f, 0.5f));
	m_mpBar.SetPosition(Vector3(-860.0f, -405.0f, 0.0f));
	//�X�V�����B
	m_mpBar.Update();
	m_mp = MAXIMUM_MP;

	m_waku2.Init("Assets/sprite/waku1.dds", 500, 60);
	m_waku2.SetPivot(Vector2(0.0f, 0.5f));
	m_waku2.SetPosition(Vector3(-860.0f, -405.0f, 0.0f));
	m_waku2.Update();

	fontRender6.SetText(L"MP");
	fontRender6.SetPosition({ -800.0f,-386.0f,0.0f });
	fontRender6.SetColor(g_vec4White);

	fontRender4.SetText(L" /  300");
	fontRender4.SetPosition({ -630.0f,-386.0f,0.0f });
	fontRender4.SetColor(g_vec4White);
	//���@�A�C�R��
	m_book.Init("Assets/sprite/book.dds", 80, 80);
	m_book.SetPivot(Vector2(0.0f, 0.5f));
	m_book.SetPosition(Vector3(-950.0f, -400.0f, 0.0f));
	//�X�V�����B
	m_book.Update();
	//MP�̕\�� �����܂�
	
	///
	///
	//����񐔂̕\�� ��������
	m_spBar.Init("Assets/sprite/kutu.dds", 130, 130);
	m_spBar.SetPivot(Vector2(0.0f, 0.5f));
	m_spBar.SetPosition(Vector3(-960.0f, -300.0f, 0.0f));

	//�X�V�����B
	m_spBar.Update();
	m_sp = MAXIMUM_SP;
	//����񐔂̕\�� �����܂�

	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle01.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run01.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_magicF].Load("Assets/animData/magic_attackF.tka");
	m_animationClips[enAnimationClip_magicF].SetLoopFlag(false);
	m_animationClips[enAnimationClip_magicW].Load("Assets/animData/magic_attackW.tka");
	m_animationClips[enAnimationClip_magicW].SetLoopFlag(false);
	m_animationClips[enAnimationClip_magicBack].Load("Assets/animData/magic_attackback.tka");
	m_animationClips[enAnimationClip_magicBack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_magicMiss].Load("Assets/animData/magic_miss.tka");
	m_animationClips[enAnimationClip_magicMiss].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Kaihi].Load("Assets/animData/kaihi.tka");
	m_animationClips[enAnimationClip_Kaihi].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Clear].Load("Assets/animData/clear.tka");
	m_animationClips[enAnimationClip_Clear].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Clear2].Load("Assets/animData/clear3.tka");
	m_animationClips[enAnimationClip_Clear2].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	modelRender.Init("Assets/modelData/player.tkm",m_animationClips, enAnimationClip_Num);

	modelRender.SetPosition(m_position);
		
	
	//�L�����R��������������B
	m_charaCon.Init(
		40.0f,
		120.0f, 
		m_position
	);

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/effect/efk/magic_back_effect.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/effect/efk/magic_fire_effect.efk");
	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/efk/magic_ear_effect.efk");

	//���y��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/clear2.wav");
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/kaihi.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/hit3.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");
	g_soundEngine->ResistWaveFileBank(18, "Assets/sound/man.wav");


	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	
		
	m_game = FindGO<Game>("game");
	return true;
}

//�X�V�����B
void Player::Update()
{
	//���݂�HP�̏����@��������
	m_text = std::to_wstring(m_hp);
	
	fontRender1.SetText(m_text.c_str());
	
	fontRender1.SetPosition({ -670.0f,-470.0f,0.0f });
	//HP��0�Ȃ�
	if (m_hp <= 0)
	{
		//���������ɂ���B
		fontRender1.SetColor(g_vec4Black);
	}
	//HP��3�ȉ��Ȃ�
	else if (m_hp <= 3)
	{
		//������Ԃɂ���B
		fontRender1.SetColor(g_vec4Red);
	}
	//HP��7�ȉ��Ȃ�
	else if (m_hp <= 7)
	{
		//���������F�ɂ���B
		fontRender1.SetColor(g_vec4Yellow);
	}
	//����10�ȉ��Ȃ�
	else {
		//�����𔒂ɂ���B
		fontRender1.SetColor(g_vec4White);
	}

	Vector3 scale = Vector3::One;
	//��HP/�ő�HP��HP�o�[�̃X�P�[���ɂ���B
	//int�^���m�̌v�Z���ƁA�����_�ȉ��؂�̂ĂɂȂ�̂ŁB
	//float�^�ɕϊ����Čv�Z���s���B
	scale.x = float(m_hp) / float(MAXIMUM_HP);
	//�X�P�[����ݒ�B
	m_hpBar.SetScale(scale);
	m_hpBar.Update();
	//���݂�HP�̏����@�����܂�
	

	//���݂�MP�̏����@��������
	m_text2 = std::to_wstring(m_mp);

	fontRender3.SetText(m_text2.c_str());

	fontRender3.SetPosition({ -700.0f,-386.0f,0.0f });
	//MP��0�Ȃ�
	if (m_mp <= 0)
	{ 
		//���������ɂ���B
		fontRender3.SetColor(g_vec4Black);
	}
	//MP��300�ȉ��Ȃ�
	else {
		//�����𔒂ɂ���B
		fontRender3.SetColor(g_vec4White);
	}
	//��MP/�ő�MP��MP�o�[�̃X�P�[���ɂ���B
	//int�^���m�̌v�Z���ƁA�����_�ȉ��؂�̂ĂɂȂ�̂ŁB
	//float�^�ɕϊ����Čv�Z���s���B
	scale.x = float(m_mp) / float(MAXIMUM_MP);
	//�X�P�[����ݒ�B
	m_mpBar.SetScale(scale);

	//�^�C�}�[�����Z����B
	m_timer += g_gameTime->GetFrameDeltaTime();
	//�^�C�}�[��7.0f�𒴂�����
	if (m_timer >= 7.0f)
	{
		//MP���{100����B
		m_mp += 100;

		//�^�C�}�[��0�ɖ߂��B
		m_timer = 0.0f;
	}
	//MP�̏����300
	if (m_mp > MAXIMUM_MP)
	{
		m_mp = MAXIMUM_MP;
	}
	m_mpBar.Update();
	//���݂�MP�̏����@�����܂�


	//���݂�SP�̏����@��������
	//��SP/�ő�SP��SP�o�[�̃X�P�[���ɂ���B
	//int�^���m�̌v�Z���ƁA�����_�ȉ��؂�̂ĂɂȂ�̂ŁB
	//float�^�ɕϊ����Čv�Z���s���B
	scale.x = float(m_sp) / float(MAXIMUM_SP);

	//�X�P�[����ݒ�B
	m_spBar.SetScale(scale);
	//�^�C�}�[�����Z����B
	m_timer2 += g_gameTime->GetFrameDeltaTime();
	//�^�C�}�[��15.0f�𒴂�����
	if (m_timer2 >= 15.0f)
	{
		//MP���{1����B
		m_sp += 1;

		//�^�C�}�[��0�ɖ߂��B
		m_timer2 = 0.0f;
	}
	//SP�̏����1
	if (m_sp > MAXIMUM_SP)
	{
		m_sp = MAXIMUM_SP;
	}
	m_spBar.Update();
	//���݂�SP�̏����@�����܂�
	
	//�ړ������B
	Move();

	//��]�����B
	Rotation();

	Collision();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();

	//�X�e�[�g�Ǘ��B
	ManageState();

	//�G�`������̍X�V�����B
	modelRender.Update();
	
}

//�ړ�
void Player::Move()
{
	//false�̏ꍇ�ړ��ł��Ȃ�
	if (IsEnableMove() == false)
	{
		return;
	}
	//xz�̈ړ����x��0.0f�ɂ���B
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 120.0f;
	forward *= stickL.y * 120.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	moveSpeed += right + forward;


	////�n�ʂɕt���Ă�����B
	if (m_charaCon.IsOnGround())
	{
		//�d�͂𖳂����B
		moveSpeed.y = 0.0f;
	}
	////�n�ʂɕt���Ă��Ȃ�������B
	else
	{
	//	//�d�͂𔭐�������B
			moveSpeed.y -= 2.5f;
	}

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charaCon.Execute(moveSpeed, 5.0f / 60.0f);

	//�G�`������ɍ��W��������B
	modelRender.SetPosition(m_position);
}

//��]
void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(moveSpeed);
		
	}
	//�G�`������ɉ�]��������B
	modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}


//���@//
//�����@
void Player::MakeFireBall()
{
	FireBall* fireBall = NewGO<FireBall>(0);
	Vector3 fireBallPosition = m_position;
	fireBallPosition.y += 70.0f;
	fireBall->SetPosition(fireBallPosition);
	fireBall->SetRotation(m_rotation);
	fireBall->SetEnMagician(FireBall::enMagician_Player);
}
//�����@
void Player::MakeEar()
{
	Ear* ear = NewGO<Ear>(0);
	Vector3 earPosition = m_position;
    earPosition.y += 70.0f;
	ear->SetPosition(earPosition);
	ear->SetRotation(m_rotation);
	ear->SetEnMagician(Ear::enMagician_Player);
}
//���~�ߖ��@
void Player::MakeBack()
{
	Back* back = NewGO<Back>(0);
	Vector3 backPosition = m_position;
	backPosition.y += 70.0f;
	back->SetPosition(backPosition);
	back->SetRotation(m_rotation);
	back->SetEnMagician(Back::enMagician_Player);
}
//���@���s
void Player::MakeMiss()
{
	Miss* miss = NewGO<Miss>(0);
	Vector3 missPosition = m_position;
	missPosition.y += 50.0f;
	miss->SetPosition(missPosition);
	miss->SetRotation(m_rotation);
	
}

//�_���[�W����
void Player::Collision() 
{
	//�ȉ��̃X�e�[�g���̓_���[�W���󂯂Ȃ�
	if (m_playerState == enPlayerState_Damage ||
		m_playerState == enPlayerState_Down||
		m_playerState==enPlayerState_Kaihi)
	{
		return;
	}

	//�G�l�~�[�̍U��������������
	const auto& collision1 = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
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
			else {
				Damage();
			}

			return;
		}
	}
	//�{�X�̋ߋ����U��������������
	const auto& collision2 = g_collisionObjectManager->FindCollisionObjects("boss_attack");
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
			else {
				Damage();
			}
			
			return;
		}
	}
	//�{�X�̉������U��������������
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("boss_stone");
	for (auto collision : collision3)
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
			else {
				Damage();
			}
			
			return;
		}
	}
}


//�X�e�[�g�Ǘ��B
void Player::ProcessCommonStateTransition()
{
	
	//MP��0�̎�
	if (m_mp == 0)
	{
		//B�{�^������������
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//�X�e�[�g�𖂖@���s�ɂ���
			m_playerState = enPlayerState_magicMiss;
			return;
		}
		//A�{�^������������
		if (g_pad[0]->IsTrigger(enButtonA))
		{ 
			//�X�e�[�g�𖂖@���s�ɂ���
			m_playerState = enPlayerState_magicMiss;
			return;
		}
		//Y�{�^������������
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//�X�e�[�g�𖂖@���s�ɂ���
			m_playerState = enPlayerState_magicMiss;
			return;
		}
	}
	
	else {
		//B�{�^���������ꂽ�牊���@�U���B
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//MP��100���炷
			m_mp -= 100;
			
			//�X�e�[�g�������@�ɂ���
				m_playerState = enPlayerState_magicF;

				m_isUnderAttack = false;

				//���y���Đ�����
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(18);
				se->Play(false);
				se->SetVolume(0.5f);
				
				//�G�t�F�N�g���Ăяo��
				MakeEffect2();
				
			return;
		}
		//A�{�^���������ꂽ�畗���@�U���B
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//MP��100���炷
			m_mp -= 100;
			//�X�e�[�g�𕗖��@�ɂ���
			m_playerState = enPlayerState_magicW;

			m_isUnderAttack = false;

			//���y���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(18);
			se->Play(false);
			se->SetVolume(0.5f);

			//�G�t�F�N�g���Ăяo��
			MakeEffect3();

			return;
		}
	}
	//MP��200���鎞
	if (m_mp >= 200) {
		//Y�{�^���������ꂽ�瑫�~�ߖ��@�U���B
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//MP��200���炷
			m_mp -= 200;
			//�X�e�[�g�𑫎~�ߖ��@�ɂ���
			m_playerState = enPlayerState_magicBack;

			m_isUnderAttack = false;

			//���y���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(18);
			se->Play(false);
			se->SetVolume(0.5f);

			//�G�t�F�N�g���Ăяo��
			MakeEffect1();

			return;
		}
	}
	//MP��100�̎�
	if (m_mp == 100)
	{
		//Y�{�^������������
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//�X�e�[�g�𖂖@���s�ɂ���
			m_playerState = enPlayerState_magicMiss;
			return;
		}
	}

	//SP��0�̎�
	if (m_sp == 0) {
		//L�{�^������������
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			//�����N����Ȃ�
			return;
		}
	}
	//SP��1�̎�
	else {
		//L�{�^������������
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			//SP��1���炷
			m_sp -= 1;

			////�X�e�[�g������ɂ���
			m_playerState = enPlayerState_Kaihi;

			//���y���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(8);
			se->Play(false);
			se->SetVolume(0.2f);

			return;
		}
	}
	
	//�n�ʂɕt���Ă�����B
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//�X�e�[�g�𑖂�ɂ���B
		m_playerState = enPlayerState_Run;
		
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_playerState = enPlayerState_Idle;
	}

	//�G�l�~�[�����ׂē|������
	if (m_game->IsWannihilationEnemy())
	{
		//�v���C���[�X�e�[�g���N���A�X�e�[�g�ɂ���
		m_playerState = enPlayerState_Clear;

		//���y���Đ�����B
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->Play(false);
		se->SetVolume(0.2f);
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void  Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void  Player::ProcessKaihiStateTransition()
{
	if (modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicAttackStateTransition()
{
	//���@�U���A�j���[�V�����̍Đ����I�������B
	if (modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (modelRender.IsPlayingAnimation() == false)
	{
		m_game->NotifyGameOver();
	}
}
void Player::ProcessClearStateTransition()
{
	//�N���A1�A�j���[�V�����̍Đ����I�������B
	if (modelRender.IsPlayingAnimation() == false)
	{
		//�v���C���[�X�e�[�g���N���A2�X�e�[�g�ɂ���
		m_playerState = enPlayerState_Clear2;
	}
}

void Player::ProcessClear2StateTransition()
{
	//�N���A2�A�j���[�V�����̍Đ����I�������B
	if (modelRender.IsPlayingAnimation() == false)
	{
		m_game->NotifyGameClear();
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;

	case enPlayerState_Run:
		ProcessRunStateTransition();
		break;

	case enPlayerState_Kaihi:
		ProcessKaihiStateTransition();
		break;

	case enPlayerState_magicF:
		ProcessMagicAttackStateTransition();
		break;

	case enPlayerState_magicW:
		ProcessMagicAttackStateTransition();
		break;

	case enPlayerState_magicBack:
		ProcessMagicAttackStateTransition();
		break;

	case enPlayerState_magicMiss:
		ProcessMagicAttackStateTransition();
		break;

	case enPlayerState_Damage:
		ProcessReceiveDamageStateTransition();
		break;

	case enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	
	case enPlayerState_Clear:
		ProcessClearStateTransition();
		break;
	case enPlayerState_Clear2:
		ProcessClear2StateTransition();
		break;
	}
}

//�A�j���[�V�����̍Đ��B
void Player::PlayAnimation()
{
	modelRender.SetAnimationSpeed(1.0f);
	////switch���B
	switch (m_playerState) {

	case enPlayerState_Idle:
		modelRender.PlayAnimation(enAnimationClip_Idle, 0.8f);
		break;

	case enPlayerState_Run:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_Run, 0.8f);
		break;

	case enPlayerState_Kaihi:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_Kaihi, 0.3f);
		break;

	case enPlayerState_magicF:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_magicF, 0.1f);
		break;

	case enPlayerState_magicW:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_magicW, 0.1f);
		break;

	case enPlayerState_magicBack:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_magicBack, 0.1f);
		break;

	case enPlayerState_magicMiss:
		modelRender.SetAnimationSpeed(1.2f);
		modelRender.PlayAnimation(enAnimationClip_magicMiss, 0.1f);
		break;

	case enPlayerState_Damage:
		modelRender.SetAnimationSpeed(1.0f);
		modelRender.PlayAnimation(enAnimationClip_Damage, 0.05f);
		break;

	case enPlayerState_Down:		
		modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	
	case enPlayerState_Clear:
		modelRender.PlayAnimation(enAnimationClip_Clear, 0.1f);
		break;

	case enPlayerState_Clear2:
		modelRender.PlayAnimation(enAnimationClip_Clear2, 0.1f);
		break;
	default:
		break;
	}
}

void Player::MakeEffect1()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(8);
	m_effectEmitter->Init(8);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 80.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Player::MakeEffect2()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(9);
	m_effectEmitter->Init(9);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 50.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Player::MakeEffect3()
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(10);
	m_effectEmitter->Init(10);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(m_scale * 50.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void  Player::Damage()
{
	m_playerState = enPlayerState_Damage;
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(9);
	se->Play(false);
	se->SetVolume(0.4f);
}
void Player::Down()
{
	//�v���C���[�X�e�[�g���_�E���X�e�[�g�ɂ���
	m_playerState = enPlayerState_Down;
	//���y���Đ�����
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(10);
	se->Play(false);
	se->SetVolume(0.4f);

}


void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���umagic_attackF�v�̎�
	if (wcscmp(eventName, L"mgic_attackF") == 0) {
		//�����@���Ăяo��
		MakeFireBall();
	}
	//�L�[�̖��O���umagic_attackW�v�̎�
    else if (wcscmp(eventName, L"mgic_attackW") == 0) {
		//�����@���Ăяo��
		MakeEar();
	}
	//�L�[�̖��O���umagic_miss�v�̎�
	else if (wcscmp(eventName, L"magic_miss") == 0) {
		//���s���@���Ăяo��
		MakeMiss();
	}
	//�L�[�̖��O���umagic_attackBack�v�̎�
	else if (wcscmp(eventName, L"magic_attackBack") == 0) {
		//���~�ߖ��@���Ăяo��
		MakeBack();
	}
}

//�`�揈���B
void Player::Render(RenderContext& rc)
{
	//���f����`�悷��B
	modelRender.Draw(rc);
	m_hpBar.Draw(rc);
	m_mpBar.Draw(rc);
	m_spBar.Draw(rc);
	m_waku.Draw(rc);
	m_waku2.Draw(rc);
	m_player1.Draw(rc);
	m_book.Draw(rc);
	fontRender1.Draw(rc);
	fontRender2.Draw(rc);
	fontRender3.Draw(rc);
	fontRender4.Draw(rc);
	fontRender5.Draw(rc);
	fontRender6.Draw(rc);
}