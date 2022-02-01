#pragma once
class Collision;
class Game;
class nsK2EngineEffectEmitter;

using namespace std;

class Player : public IGameObject
{
public:
	enum EnPlayerState {
		enPlayerState_Idle,             //待機
		enPlayerState_Run,              //走り
		enPlayerState_magicF,           //炎魔法
		enPlayerState_magicW,           //風魔法
		enPlayerState_magicMiss,        //魔法失敗
		enPlayerState_magicBack,        //足止め魔法
		enPlayerState_Damage,			//被ダメージ
		enPlayerState_Down,				//ダウン
		enPlayerState_Kaihi,            //回避
		enPlayerState_Clear,            //ダンス１
		enPlayerState_Clear2,           //ダンス2
	};
public:
	Player();
	~Player();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//移動処理。
	void Move();
	//回転処理。
	void Rotation();
	//ステート管理。
	void ManageState();
	//アニメーションの再生。
	void PlayAnimation();
private:
	//炎魔法の作成
	void MakeFireBall();
	//炎魔法のエフェクト
	void MakeEffect2();

	//風魔法の作成
	void MakeEar();
	//風魔法のエフェクト
	void MakeEffect3();

	//足止め魔法の作成
	void MakeBack();
	//足止め魔法のエフェクト
	void MakeEffect1();

	//魔法失敗の作成
	void MakeMiss();
	
	//共通ステートの遷移処理。
	void ProcessCommonStateTransition();
	
	//待機ステートの遷移処理。
	void ProcessIdleStateTransition();

	//走りステートの遷移処理。
	void ProcessRunStateTransition();
	
	/// 被ダメージステートの遷移処理。
	void ProcessReceiveDamageStateTransition();

	/// ダウンステートの遷移処理。
	void ProcessDownStateTransition();

	//回避ステートの遷移処理。
	void ProcessKaihiStateTransition();
	
	//魔法攻撃ステートの遷移処理。
	void ProcessMagicAttackStateTransition();

	//クリア1ステートの遷移処理。
	void ProcessClearStateTransition();
	//クリア2ステートの遷移処理。
	void ProcessClear2StateTransition();

	/// プレイヤーの攻撃との当たり判定処理。
	void Collision();
	//ダメージ
	void Damage();
	//ダウン
	void Down();
	
public:
	//座標
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//回転
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//大きさ
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//アニメーションイベント用の関数
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	
	//移動できない
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

	enum EnAnimationClip {		//アニメーション。
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
	
	//メンバ変数。
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
	ModelRender            modelRender;	                            //モデルレンダ―
	Game*                  m_game = nullptr;
	EffectEmitter*         m_effectEmitter = nullptr;               //エフェクト
	CharacterController	   m_charaCon;                              //キャラクターコントロラー
	EnPlayerState		   m_playerState = enPlayerState_Idle;	    //プレイヤーステート
	Vector3                m_position;			                    //座標
	Vector3                moveSpeed;		                        //移動速度
	Vector3                m_scale;                                 //大きさ
	Vector3                m_forward = Vector3::AxisZ;
	Quaternion             m_rotation;                              //座標
	bool                   m_isUnderAttack = false;
	bool                   m_isUnderKaihi = false;
	int                    m_hp = 0;                                //体力
	int                    m_mp = 0;                                //魔法
	int                    m_sp = 0;                                //回避
	float				   m_timer = 0.0f;
	float				   m_timer2 = 0.0f;

};

