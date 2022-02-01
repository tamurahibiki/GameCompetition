#pragma once

class Player;
class Mura;
class nsK2EngineEffectEmitter;

class Boss :public IGameObject
{
public:
	enum EnBossState {
		enBossState_Idle,				 //待機。
		enBossState_Idle2,				 //攻撃中待機。
		enBossState_Idle3,               //特殊待機
		enBossState_Chase,				 //追跡。
		enBossState_Magic,               //魔法攻撃
		enBossState_Attack,				 //打撃攻撃。
		enBossState_Attack2,             //怒り攻撃。
		enBossState_Damage,			     //被ダメージ。
		enBossState_Down,			     //ダウン。
		enBossState_Down2,			     //ダウン。
		enBossState_Stand,               //起き上がり1
		enBossState_Stand2,              //起き上がり2
		enBossState_Fall,                //落下
		enBossState_end                  //怒り状態
	};
public:
	Boss();
	~Boss();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
private:
	/// <summary>
	/// 追跡処理。
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃処理。
	/// </summary>
	void Attack();
	/// <summary>
	/// 魔法攻撃処理。
	/// </summary>
	void MakeStone();
	/// <summary>
	/// プレイヤーの攻撃との当たり判定処理。
	/// </summary>
	void Collision();
	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// 村を探索する。
	/// </summary>
	/// <returns>村が見つかったらtrue。</returns>
	const bool SearchMura() const;
	///
	/// 攻撃用の当たり判定コリジョンを作成する。
	///
	void MakeAttackCollision();
	/// <summary>
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();
	///
	/// エフェクトの再生。
	/// 
	void MakeEffect();
	///
	/// エフェクトの再生。
	/// 
	void MakeEffectDown();
	///
	/// エフェクトの再生。
	/// 
	void MakeEffectMagic();
	///
	/// ダウンの処理
	/// 
	void Down();
	/// <summary>
	/// 各ステートの遷移処理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移処理。
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理。
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
    /// 待機ステートの遷移処理。
    ///</summary>
	void ProcessIdle2StateTransition();
	/// <summary>
	/// 走りステートの遷移処理。
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// 魔法攻撃ステートの背遷移処理。
	/// 
	void ProcessMagicAttackStateTransition();
	/// <summary>
	/// 追跡ステートの背遷移処理。
	/// 
	void ProcessChaseStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDown2StateTransition();
	/// <summary>
	/// 起き上がり1ステートの遷移処理。
	/// </summary>
	void ProcessStandStateTransition();
	/// <summary>
	/// 起き上がり2ステートの遷移処理。
	/// </summary>
	void ProcessStand2StateTransition();
	/// <summary>
	/// 落下ステートの遷移処理。
	/// </summary>
	void ProcessFallStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack() const;
	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack2() const;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Idle2,					//攻撃中待機アニメーション。
		enAnimationClip_Idle3,                  //怒り待機アニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃1アニメーション。
		enAnimationClip_Attack2,                //攻撃2アニメーション。
		enAnimationClip_Magic,                  //魔法攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウン1アニメーション。
		enAnimationClip_Down2,                  //ダウン2アニメーション。
		enAnimationClip_Stand,                  //起き上がり1アニメーション。
		enAnimationClip_Stand2,                 //起き上がり2アニメーション。
		enAnimationClip_Fall,                   //落下アニメーション。
		enAnimationClip_Num,
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;                                 //キャラクターコントロラー
	EnBossState				    m_bossState = enBossState_Idle;			    //ボスステート。
	Player*                     m_player = nullptr;
	Mura*                       m_mura = nullptr;
	EffectEmitter*              m_effectEmitter = nullptr;
	bool                        m_isUnderAttack = false;
	bool                        m_isUnderAttack2 = false;
	bool                        m_end = false;
	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;	                        //待機タイマー。
	float                       m_timer = 0.0f;                             //タイマー
	int                         m_hp = 10;                                  //HP
};

