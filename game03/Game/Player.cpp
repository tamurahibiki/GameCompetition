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
	//最大HP。
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
	//HPの表示 ここから
	m_hpBar.Init("Assets/sprite/hpbar3.DDS", 600, 100);
	m_hpBar.SetPivot(Vector2(0.0f, 0.5f));
	m_hpBar.SetPosition(Vector3(-860.0f, -485.0f, 0.0f));
	//更新処理。
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

	//プレイヤーアイコン
	m_player1.Init("Assets/sprite/player2.dds", 100, 100);
	m_player1.SetPivot(Vector2(0.0f, 0.5f));
	m_player1.SetPosition(Vector3(-960.0f, -485.0f, 0.0f));
	//更新処理。
	m_player1.Update();
	//HPの表示 ここまで

	//MPの表示 ここから
	m_mpBar.Init("Assets/sprite/mpbar2.dds", 500,60);
	m_mpBar.SetPivot(Vector2(0.0f, 0.5f));
	m_mpBar.SetPosition(Vector3(-860.0f, -405.0f, 0.0f));
	//更新処理。
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
	//魔法アイコン
	m_book.Init("Assets/sprite/book.dds", 80, 80);
	m_book.SetPivot(Vector2(0.0f, 0.5f));
	m_book.SetPosition(Vector3(-950.0f, -400.0f, 0.0f));
	//更新処理。
	m_book.Update();
	//MPの表示 ここまで
	
	///
	///
	//回避回数の表示 ここから
	m_spBar.Init("Assets/sprite/kutu.dds", 130, 130);
	m_spBar.SetPivot(Vector2(0.0f, 0.5f));
	m_spBar.SetPosition(Vector3(-960.0f, -300.0f, 0.0f));

	//更新処理。
	m_spBar.Update();
	m_sp = MAXIMUM_SP;
	//回避回数の表示 ここまで

	//アニメーションクリップをロードする。
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
	//モデルを読み込む。
	modelRender.Init("Assets/modelData/player.tkm",m_animationClips, enAnimationClip_Num);

	modelRender.SetPosition(m_position);
		
	
	//キャラコンを初期化する。
	m_charaCon.Init(
		40.0f,
		120.0f, 
		m_position
	);

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/effect/efk/magic_back_effect.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/effect/efk/magic_fire_effect.efk");
	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/efk/magic_ear_effect.efk");

	//音楽を読み込む。
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/clear2.wav");
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/kaihi.wav");
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/hit3.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");
	g_soundEngine->ResistWaveFileBank(18, "Assets/sound/man.wav");


	//アニメーションイベント用の関数を設定する。
	modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	
		
	m_game = FindGO<Game>("game");
	return true;
}

//更新処理。
void Player::Update()
{
	//現在のHPの処理　ここから
	m_text = std::to_wstring(m_hp);
	
	fontRender1.SetText(m_text.c_str());
	
	fontRender1.SetPosition({ -670.0f,-470.0f,0.0f });
	//HPが0なら
	if (m_hp <= 0)
	{
		//文字を黒にする。
		fontRender1.SetColor(g_vec4Black);
	}
	//HPが3以下なら
	else if (m_hp <= 3)
	{
		//文字を赤にする。
		fontRender1.SetColor(g_vec4Red);
	}
	//HPが7以下なら
	else if (m_hp <= 7)
	{
		//文字を黄色にする。
		fontRender1.SetColor(g_vec4Yellow);
	}
	//それ10以下なら
	else {
		//文字を白にする。
		fontRender1.SetColor(g_vec4White);
	}

	Vector3 scale = Vector3::One;
	//現HP/最大HPをHPバーのスケールにする。
	//int型同士の計算だと、小数点以下切り捨てになるので。
	//float型に変換して計算を行う。
	scale.x = float(m_hp) / float(MAXIMUM_HP);
	//スケールを設定。
	m_hpBar.SetScale(scale);
	m_hpBar.Update();
	//現在のHPの処理　ここまで
	

	//現在のMPの処理　ここから
	m_text2 = std::to_wstring(m_mp);

	fontRender3.SetText(m_text2.c_str());

	fontRender3.SetPosition({ -700.0f,-386.0f,0.0f });
	//MPが0なら
	if (m_mp <= 0)
	{ 
		//文字を黒にする。
		fontRender3.SetColor(g_vec4Black);
	}
	//MPが300以下なら
	else {
		//文字を白にする。
		fontRender3.SetColor(g_vec4White);
	}
	//現MP/最大MPをMPバーのスケールにする。
	//int型同士の計算だと、小数点以下切り捨てになるので。
	//float型に変換して計算を行う。
	scale.x = float(m_mp) / float(MAXIMUM_MP);
	//スケールを設定。
	m_mpBar.SetScale(scale);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();
	//タイマーが7.0fを超えたら
	if (m_timer >= 7.0f)
	{
		//MPを＋100する。
		m_mp += 100;

		//タイマーを0に戻す。
		m_timer = 0.0f;
	}
	//MPの上限は300
	if (m_mp > MAXIMUM_MP)
	{
		m_mp = MAXIMUM_MP;
	}
	m_mpBar.Update();
	//現在のMPの処理　ここまで


	//現在のSPの処理　ここから
	//現SP/最大SPをSPバーのスケールにする。
	//int型同士の計算だと、小数点以下切り捨てになるので。
	//float型に変換して計算を行う。
	scale.x = float(m_sp) / float(MAXIMUM_SP);

	//スケールを設定。
	m_spBar.SetScale(scale);
	//タイマーを加算する。
	m_timer2 += g_gameTime->GetFrameDeltaTime();
	//タイマーが15.0fを超えたら
	if (m_timer2 >= 15.0f)
	{
		//MPを＋1する。
		m_sp += 1;

		//タイマーを0に戻す。
		m_timer2 = 0.0f;
	}
	//SPの上限は1
	if (m_sp > MAXIMUM_SP)
	{
		m_sp = MAXIMUM_SP;
	}
	m_spBar.Update();
	//現在のSPの処理　ここまで
	
	//移動処理。
	Move();

	//回転処理。
	Rotation();

	Collision();
	//アニメーションの再生。
	PlayAnimation();

	//ステート管理。
	ManageState();

	//絵描きさんの更新処理。
	modelRender.Update();
	
}

//移動
void Player::Move()
{
	//falseの場合移動できない
	if (IsEnableMove() == false)
	{
		return;
	}
	//xzの移動速度を0.0fにする。
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 120.0f;
	forward *= stickL.y * 120.0f;

	//移動速度にスティックの入力量を加算する。
	moveSpeed += right + forward;


	////地面に付いていたら。
	if (m_charaCon.IsOnGround())
	{
		//重力を無くす。
		moveSpeed.y = 0.0f;
	}
	////地面に付いていなかったら。
	else
	{
	//	//重力を発生させる。
			moveSpeed.y -= 2.5f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(moveSpeed, 5.0f / 60.0f);

	//絵描きさんに座標を教える。
	modelRender.SetPosition(m_position);
}

//回転
void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(moveSpeed);
		
	}
	//絵描きさんに回転を教える。
	modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}


//魔法//
//炎魔法
void Player::MakeFireBall()
{
	FireBall* fireBall = NewGO<FireBall>(0);
	Vector3 fireBallPosition = m_position;
	fireBallPosition.y += 70.0f;
	fireBall->SetPosition(fireBallPosition);
	fireBall->SetRotation(m_rotation);
	fireBall->SetEnMagician(FireBall::enMagician_Player);
}
//風魔法
void Player::MakeEar()
{
	Ear* ear = NewGO<Ear>(0);
	Vector3 earPosition = m_position;
    earPosition.y += 70.0f;
	ear->SetPosition(earPosition);
	ear->SetRotation(m_rotation);
	ear->SetEnMagician(Ear::enMagician_Player);
}
//足止め魔法
void Player::MakeBack()
{
	Back* back = NewGO<Back>(0);
	Vector3 backPosition = m_position;
	backPosition.y += 70.0f;
	back->SetPosition(backPosition);
	back->SetRotation(m_rotation);
	back->SetEnMagician(Back::enMagician_Player);
}
//魔法失敗
void Player::MakeMiss()
{
	Miss* miss = NewGO<Miss>(0);
	Vector3 missPosition = m_position;
	missPosition.y += 50.0f;
	miss->SetPosition(missPosition);
	miss->SetRotation(m_rotation);
	
}

//ダメージ判定
void Player::Collision() 
{
	//以下のステート中はダメージを受けない
	if (m_playerState == enPlayerState_Damage ||
		m_playerState == enPlayerState_Down||
		m_playerState==enPlayerState_Kaihi)
	{
		return;
	}

	//エネミーの攻撃が当たった時
	const auto& collision1 = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collision1)
	{
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす
			m_hp -= 1;
			//HPが0なら
			if (m_hp == 0)
			{
				Down();
			}
			//HPが0以外なら
			else {
				Damage();
			}

			return;
		}
	}
	//ボスの近距離攻撃が当たった時
	const auto& collision2 = g_collisionObjectManager->FindCollisionObjects("boss_attack");
	for (auto collision : collision2)
	{
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす
				m_hp -= 1;
				//HPが0なら
			if (m_hp == 0)
			{
				Down();
			}
			//HPが0以外なら
			else {
				Damage();
			}
			
			return;
		}
	}
	//ボスの遠距離攻撃が当たった時
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("boss_stone");
	for (auto collision : collision3)
	{
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす
				m_hp -= 1;
				//HPが0なら
			if (m_hp == 0)
			{
				Down();
			}
			//HPが0以外なら
			else {
				Damage();
			}
			
			return;
		}
	}
}


//ステート管理。
void Player::ProcessCommonStateTransition()
{
	
	//MPが0の時
	if (m_mp == 0)
	{
		//Bボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ステートを魔法失敗にする
			m_playerState = enPlayerState_magicMiss;
			return;
		}
		//Aボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonA))
		{ 
			//ステートを魔法失敗にする
			m_playerState = enPlayerState_magicMiss;
			return;
		}
		//Yボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//ステートを魔法失敗にする
			m_playerState = enPlayerState_magicMiss;
			return;
		}
	}
	
	else {
		//Bボタンが押されたら炎魔法攻撃。
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//MPを100減らす
			m_mp -= 100;
			
			//ステートを炎魔法にする
				m_playerState = enPlayerState_magicF;

				m_isUnderAttack = false;

				//音楽を再生する
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(18);
				se->Play(false);
				se->SetVolume(0.5f);
				
				//エフェクトを呼び出す
				MakeEffect2();
				
			return;
		}
		//Aボタンが押されたら風魔法攻撃。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//MPを100減らす
			m_mp -= 100;
			//ステートを風魔法にする
			m_playerState = enPlayerState_magicW;

			m_isUnderAttack = false;

			//音楽を再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(18);
			se->Play(false);
			se->SetVolume(0.5f);

			//エフェクトを呼び出す
			MakeEffect3();

			return;
		}
	}
	//MPが200ある時
	if (m_mp >= 200) {
		//Yボタンが押されたら足止め魔法攻撃。
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//MPを200減らす
			m_mp -= 200;
			//ステートを足止め魔法にする
			m_playerState = enPlayerState_magicBack;

			m_isUnderAttack = false;

			//音楽を再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(18);
			se->Play(false);
			se->SetVolume(0.5f);

			//エフェクトを呼び出す
			MakeEffect1();

			return;
		}
	}
	//MPが100の時
	if (m_mp == 100)
	{
		//Yボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			//ステートを魔法失敗にする
			m_playerState = enPlayerState_magicMiss;
			return;
		}
	}

	//SPが0の時
	if (m_sp == 0) {
		//Lボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			//何も起こらない
			return;
		}
	}
	//SPが1の時
	else {
		//Lボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonLB1))
		{
			//SPを1減らす
			m_sp -= 1;

			////ステートを回避にする
			m_playerState = enPlayerState_Kaihi;

			//音楽を再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(8);
			se->Play(false);
			se->SetVolume(0.2f);

			return;
		}
	}
	
	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//ステートを走りにする。
		m_playerState = enPlayerState_Run;
		
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを待機にする。
		m_playerState = enPlayerState_Idle;
	}

	//エネミーをすべて倒したら
	if (m_game->IsWannihilationEnemy())
	{
		//プレイヤーステートをクリアステートにする
		m_playerState = enPlayerState_Clear;

		//音楽を再生する。
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->Play(false);
		se->SetVolume(0.2f);
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void  Player::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void  Player::ProcessKaihiStateTransition()
{
	if (modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Player::ProcessMagicAttackStateTransition()
{
	//魔法攻撃アニメーションの再生が終わったら。
	if (modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (modelRender.IsPlayingAnimation() == false)
	{
		m_game->NotifyGameOver();
	}
}
void Player::ProcessClearStateTransition()
{
	//クリア1アニメーションの再生が終わったら。
	if (modelRender.IsPlayingAnimation() == false)
	{
		//プレイヤーステートをクリア2ステートにする
		m_playerState = enPlayerState_Clear2;
	}
}

void Player::ProcessClear2StateTransition()
{
	//クリア2アニメーションの再生が終わったら。
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

//アニメーションの再生。
void Player::PlayAnimation()
{
	modelRender.SetAnimationSpeed(1.0f);
	////switch文。
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
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(8);
	m_effectEmitter->Init(8);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 80.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Player::MakeEffect2()
{
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(9);
	m_effectEmitter->Init(9);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 50.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Player::MakeEffect3()
{
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(10);
	m_effectEmitter->Init(10);
	//エフェクトの大きさを設定する。
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
	//プレイヤーステートをダウンステートにする
	m_playerState = enPlayerState_Down;
	//音楽を再生する
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(10);
	se->Play(false);
	se->SetVolume(0.4f);

}


void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「magic_attackF」の時
	if (wcscmp(eventName, L"mgic_attackF") == 0) {
		//炎魔法を呼び出す
		MakeFireBall();
	}
	//キーの名前が「magic_attackW」の時
    else if (wcscmp(eventName, L"mgic_attackW") == 0) {
		//風魔法を呼び出す
		MakeEar();
	}
	//キーの名前が「magic_miss」の時
	else if (wcscmp(eventName, L"magic_miss") == 0) {
		//失敗魔法を呼び出す
		MakeMiss();
	}
	//キーの名前が「magic_attackBack」の時
	else if (wcscmp(eventName, L"magic_attackBack") == 0) {
		//足止め魔法を呼び出す
		MakeBack();
	}
}

//描画処理。
void Player::Render(RenderContext& rc)
{
	//モデルを描画する。
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