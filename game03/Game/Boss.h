#pragma once

class Player;
class Mura;
class nsK2EngineEffectEmitter;

class Boss :public IGameObject
{
public:
	enum EnBossState {
		enBossState_Idle,				 //�ҋ@�B
		enBossState_Idle2,				 //�U�����ҋ@�B
		enBossState_Idle3,               //����ҋ@
		enBossState_Chase,				 //�ǐՁB
		enBossState_Magic,               //���@�U��
		enBossState_Attack,				 //�Ō��U���B
		enBossState_Attack2,             //�{��U���B
		enBossState_Damage,			     //��_���[�W�B
		enBossState_Down,			     //�_�E���B
		enBossState_Down2,			     //�_�E���B
		enBossState_Stand,               //�N���オ��1
		enBossState_Stand2,              //�N���オ��2
		enBossState_Fall,                //����
		enBossState_end                  //�{����
	};
public:
	Boss();
	~Boss();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
private:
	/// <summary>
	/// �ǐՏ����B
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U�������B
	/// </summary>
	void Attack();
	/// <summary>
	/// ���@�U�������B
	/// </summary>
	void MakeStone();
	/// <summary>
	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// ����T������B
	/// </summary>
	/// <returns>��������������true�B</returns>
	const bool SearchMura() const;
	///
	/// �U���p�̓����蔻��R���W�������쐬����B
	///
	void MakeAttackCollision();
	/// <summary>
	/// �A�j���[�V�����̍Đ��B
	/// </summary>
	void PlayAnimation();
	///
	/// �G�t�F�N�g�̍Đ��B
	/// 
	void MakeEffect();
	///
	/// �G�t�F�N�g�̍Đ��B
	/// 
	void MakeEffectDown();
	///
	/// �G�t�F�N�g�̍Đ��B
	/// 
	void MakeEffectMagic();
	///
	/// �_�E���̏���
	/// 
	void Down();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
    /// �ҋ@�X�e�[�g�̑J�ڏ����B
    ///</summary>
	void ProcessIdle2StateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// ���@�U���X�e�[�g�̔w�J�ڏ����B
	/// 
	void ProcessMagicAttackStateTransition();
	/// <summary>
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	/// 
	void ProcessChaseStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDown2StateTransition();
	/// <summary>
	/// �N���オ��1�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessStandStateTransition();
	/// <summary>
	/// �N���オ��2�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessStand2StateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessFallStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// �U���ł��鋗�����ǂ������ׂ�B
	/// </summary>
	/// <returns>�U���ł���Ȃ�true�B</returns>
	const bool IsCanAttack() const;
	/// <summary>
	/// �U���ł��鋗�����ǂ������ׂ�B
	/// </summary>
	/// <returns>�U���ł���Ȃ�true�B</returns>
	const bool IsCanAttack2() const;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Idle2,					//�U�����ҋ@�A�j���[�V�����B
		enAnimationClip_Idle3,                  //�{��ҋ@�A�j���[�V�����B
		enAnimationClip_Run,					//����A�j���[�V�����B
		enAnimationClip_Attack,					//�U��1�A�j���[�V�����B
		enAnimationClip_Attack2,                //�U��2�A�j���[�V�����B
		enAnimationClip_Magic,                  //���@�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,					//�_�E��1�A�j���[�V�����B
		enAnimationClip_Down2,                  //�_�E��2�A�j���[�V�����B
		enAnimationClip_Stand,                  //�N���オ��1�A�j���[�V�����B
		enAnimationClip_Stand2,                 //�N���オ��2�A�j���[�V�����B
		enAnimationClip_Fall,                   //�����A�j���[�V�����B
		enAnimationClip_Num,
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_moveSpeed;								//�ړ����x�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3::One;						//�傫���B
	CharacterController			m_charaCon;                                 //�L�����N�^�[�R���g�����[
	EnBossState				    m_bossState = enBossState_Idle;			    //�{�X�X�e�[�g�B
	Player*                     m_player = nullptr;
	Mura*                       m_mura = nullptr;
	EffectEmitter*              m_effectEmitter = nullptr;
	bool                        m_isUnderAttack = false;
	bool                        m_isUnderAttack2 = false;
	bool                        m_end = false;
	float						m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float						m_idleTimer = 0.0f;	                        //�ҋ@�^�C�}�[�B
	float                       m_timer = 0.0f;                             //�^�C�}�[
	int                         m_hp = 10;                                  //HP
};

