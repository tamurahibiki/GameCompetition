#pragma once
class Collision;
class Game;
class nsK2EngineEffectEmitter;

using namespace std;

class Player : public IGameObject
{
public:
	enum EnPlayerState {
		enPlayerState_Idle,             //�ҋ@
		enPlayerState_Run,              //����
		enPlayerState_magicF,           //�����@
		enPlayerState_magicW,           //�����@
		enPlayerState_magicMiss,        //���@���s
		enPlayerState_magicBack,        //���~�ߖ��@
		enPlayerState_Damage,			//��_���[�W
		enPlayerState_Down,				//�_�E��
		enPlayerState_Kaihi,            //���
		enPlayerState_Clear,            //�_���X�P
		enPlayerState_Clear2,           //�_���X2
	};
public:
	Player();
	~Player();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�ړ������B
	void Move();
	//��]�����B
	void Rotation();
	//�X�e�[�g�Ǘ��B
	void ManageState();
	//�A�j���[�V�����̍Đ��B
	void PlayAnimation();
private:
	//�����@�̍쐬
	void MakeFireBall();
	//�����@�̃G�t�F�N�g
	void MakeEffect2();

	//�����@�̍쐬
	void MakeEar();
	//�����@�̃G�t�F�N�g
	void MakeEffect3();

	//���~�ߖ��@�̍쐬
	void MakeBack();
	//���~�ߖ��@�̃G�t�F�N�g
	void MakeEffect1();

	//���@���s�̍쐬
	void MakeMiss();
	
	//���ʃX�e�[�g�̑J�ڏ����B
	void ProcessCommonStateTransition();
	
	//�ҋ@�X�e�[�g�̑J�ڏ����B
	void ProcessIdleStateTransition();

	//����X�e�[�g�̑J�ڏ����B
	void ProcessRunStateTransition();
	
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	void ProcessReceiveDamageStateTransition();

	/// �_�E���X�e�[�g�̑J�ڏ����B
	void ProcessDownStateTransition();

	//����X�e�[�g�̑J�ڏ����B
	void ProcessKaihiStateTransition();
	
	//���@�U���X�e�[�g�̑J�ڏ����B
	void ProcessMagicAttackStateTransition();

	//�N���A1�X�e�[�g�̑J�ڏ����B
	void ProcessClearStateTransition();
	//�N���A2�X�e�[�g�̑J�ڏ����B
	void ProcessClear2StateTransition();

	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	void Collision();
	//�_���[�W
	void Damage();
	//�_�E��
	void Down();
	
public:
	//���W
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//��]
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�傫��
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//�A�j���[�V�����C�x���g�p�̊֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	
	//�ړ��ł��Ȃ�
	bool IsEnableMove() const
	{
		return
			m_playerState != enPlayerState_magicF &&
			m_playerState != enPlayerState_magicW &&
			m_playerState != enPlayerState_magicMiss&&
			m_playerState != enPlayerState_magicBack&&
			m_playerState != enPlayerState_Damage &&
			m_playerState != enPlayerState_Down&&
		    m_playerState != enPlayerState_Clear&&
		    m_playerState != enPlayerState_Clear2;
	}

	enum EnAnimationClip {		//�A�j���[�V�����B
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_magicF,
		enAnimationClip_magicW,
		enAnimationClip_magicMiss,
		enAnimationClip_magicBack,
		enAnimationClip_Damage,
		enAnimationClip_Down,
		enAnimationClip_Kaihi,
		enAnimationClip_Clear,
		enAnimationClip_Clear2,
		enAnimationClip_Num,
	};
	
	//�����o�ϐ��B
	SpriteRender		   m_hpBar;
	SpriteRender		   m_mpBar;
	SpriteRender		   m_spBar;
	SpriteRender		   m_waku;
	SpriteRender		   m_waku2;
	SpriteRender		   m_player1;
	SpriteRender		   m_book;

	wstring				   m_text;
	wstring				   m_text2;

	FontRender             fontRender1;
	FontRender             fontRender2;
	FontRender             fontRender3;
	FontRender             fontRender4;
	FontRender             fontRender5;
	FontRender             fontRender6;

	AnimationClip		   m_animationClips[enAnimationClip_Num];
	ModelRender            modelRender;	                            //���f�������_�\
	Game*                  m_game = nullptr;
	EffectEmitter*         m_effectEmitter = nullptr;               //�G�t�F�N�g
	CharacterController	   m_charaCon;                              //�L�����N�^�[�R���g�����[
	EnPlayerState		   m_playerState = enPlayerState_Idle;	    //�v���C���[�X�e�[�g
	Vector3                m_position;			                    //���W
	Vector3                moveSpeed;		                        //�ړ����x
	Vector3                m_scale;                                 //�傫��
	Vector3                m_forward = Vector3::AxisZ;
	Quaternion             m_rotation;                              //���W
	bool                   m_isUnderAttack = false;
	bool                   m_isUnderKaihi = false;
	int                    m_hp = 0;                                //�̗�
	int                    m_mp = 0;                                //���@
	int                    m_sp = 0;                                //���
	float				   m_timer = 0.0f;
	float				   m_timer2 = 0.0f;

};

