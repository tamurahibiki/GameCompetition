#pragma once

class Player;
class Mura;
class nsK2EngineEffectEmitter;

class Enemy:public IGameObject
{
private:
	enum EnEnemyState {
		enEnemyState_Idle,					//待機。
		enEnemyState_Chase,					//追跡。
		enEnemyState_Attack,				//攻撃。
		enEnemyState_Damage,			    //被ダメージ。
		enEnemyState_Down,					//ダウン。
	};
 public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
	/// プレイヤーの攻撃との当たり判定処理。
	/// </summary>
	void Collision();

	//ダメージ
	void Damage();

	//ダウン
	void Down();

	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// 村を探索する。
	/// </summary>
	/// <returns>村が見つかったらtrue。</returns>
	const bool SearchMura()const;
	/// <summary>
	/// 攻撃用の当たり判定コリジョンを作成する。
	/// </summary>
	void MakeAttackCollision();
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();

	void MakeEffectDown();
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
	/// 走りステートの遷移処理。
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// 追跡ステートの背遷移処理。
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDownStateTransition();
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack() const;

	const bool IsCanAttack2() const;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Run,					//走りアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
        enAnimationClip_Num,
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	EffectEmitter*              m_effectEmitter = nullptr;
	Vector3						m_position;									//座標。
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//エネミーステート。
	Player*                     m_player = nullptr;
	Mura*                       m_mura = nullptr;
	bool                        m_isUnderAttack = false;
	int                         m_hp = 4;

	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;		                    //待機タイマー。
	float					    m_timer = 0.0f;						        //タイマー。
};
