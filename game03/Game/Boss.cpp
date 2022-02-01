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
	//アニメーションを読み込む。
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
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/boss.tkm", m_animationClips, enAnimationClip_Num);

	//座標を設定する。
	Vector3 position = m_position;
	position.y += 2.5f;
	m_modelRender.SetPosition(position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		65.0f,			//半径。
		250.0f,			//高さ。
		m_position		//座標。
	);

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/up_boss.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/down_boss.efk");
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/magic_boss_effect.efk");
	
	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/attack.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/hit2.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/monster.wav");
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_player = FindGO<Player>("player");
	m_mura = FindGO<Mura>("mura");
	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Boss::Update()
{
	Move();
	//追跡処理。
	Chase();
	//回転処理。
	Rotation();
	//当たり判定。
	Collision();
	//攻撃処理。
	Attack();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();
	//モデルの更新。
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
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Boss::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
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
	//炎魔法が当たった時
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
	//風魔法が当たった時
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
	//足止め魔法が当たった時
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("player_Back");
	for (auto collision : collision3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_timer += g_gameTime->GetFrameDeltaTime();

			//タイマーが420.0f以下だったら。
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
	//攻撃ステートでないなら処理をしない。
	if (m_bossState != enBossState_Attack)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
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

	//プレイヤーを見つけたとき
	if (SearchPlayer() == true)
	{
		diff.Normalize();
		//攻撃が届かないとき
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
		//怒り状態のとき
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
		//通常状態のとき
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
	//村を見つけたとき
	else if(SearchMura()== true)
	{
		    diff1.Normalize();
	 //攻撃が届かない       
	 if (IsCanAttack2() == false)
	  {
			 m_moveSpeed = diff1 * 250.0f;
			 m_bossState = enBossState_Chase;
			 return;
	   }
	 //怒り状態のとき
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
	 //通常状態のとき
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
	//上記以外のとき
	else {
		m_bossState = enBossState_Idle;
	}

}

void Boss::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}
}


void Boss::ProcessRunStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Boss::ProcessChaseStateTransition()
{
	//攻撃できる距離なら。
	if (IsCanAttack() == true)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessMagicAttackStateTransition()
{
	if (onlyOnce6 == true) {
		MakeEffectMagic();
		onlyOnce6 = false;
	}
	//魔法攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_bossState = enBossState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
	}
}
void Boss::ProcessIdle2StateTransition() 
{
	//攻撃中待機アニメーションの再生が終わったら。
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
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		game->AddDefeatedEnemyNumber();
		//自身を削除する。
		DeleteGO(this);
		if (onlyOnce) {
			onlyOnce = false;
			MakeEffectDown();
		}
	}
}
void Boss::ProcessDown2StateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Stand;
	}
}

void Boss::ProcessStandStateTransition()
{
	//起き上がり1アニメーションの再生が終わったら。
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
	//起き上がり2アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Idle;
	}
}
void Boss::ProcessFallStateTransition()
{
	//落下アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_bossState = enBossState_Stand2;
	}
}

void Boss::ManageState()
{
	switch (m_bossState)
	{
		//待機ステートの時。
	case enBossState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//攻撃中待機ステートの時。
	case enBossState_Idle2:
		ProcessIdleStateTransition();
		break;
		//特殊待機ステートの時。
	case enBossState_Idle3:
		ProcessIdle2StateTransition();
		break;
		//追跡ステートの時。
	case enBossState_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//魔法攻撃ステートの時。
	case enBossState_Magic:
		ProcessMagicAttackStateTransition();
		break;
		//攻撃ステートの時。
	case enBossState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時。
	case enBossState_Damage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時。
	case enBossState_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
		//ダウンステート2の時。
	case enBossState_Down2:
		//ダウンステート2のステート遷移処理。
		ProcessDown2StateTransition();
		break;
		//起き上がり1ステートの時。
	case enBossState_Stand:
		ProcessStandStateTransition();
		break;
		//起き上がり2ステートの時。
	case enBossState_Stand2:
		ProcessStand2StateTransition();
		onlyOnce5 = false;
		break;
		//落下ステートの時。
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
		//待機ステートの時。
	case enBossState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//攻撃待機ステートの時。
	case enBossState_Idle2:
		//攻撃待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle2, 0.5f);
		break;
		//怒り待機ステートの時。
	case enBossState_Idle3:
		//怒り待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle3, 0.5f);
		break;
		//追跡ステートの時。
	case enBossState_Chase:
		m_modelRender.SetAnimationSpeed(1.5f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//攻撃ステートの時。
	case enBossState_Attack:
		m_modelRender.SetAnimationSpeed(1.3f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//魔法攻撃ステートの時。
	case enBossState_Magic:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Magic, 0.1f);
		break;
		//被ダメージステートの時。
	case enBossState_Damage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.05f);
		break;
		//ダウンステートの時。
	case enBossState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.05f);
		break;
		//ダウンステート2の時。
	case enBossState_Down2:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down2, 0.05f);
		break;
		//立ち上がり1ステート1の時。
	case enBossState_Stand:
		//立ち上がり1アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Stand, 0.05f);
		break;
		//立ち上がり2ステートの時。
	case enBossState_Stand2:
		m_modelRender.SetAnimationSpeed(1.3f);
		//立ち上がり2アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Stand2, 0.01f);
		break;
		//落下ステートの時。
	case enBossState_Fall:
		////落下アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Fall, 1.0f);
		break;
	default:
		break;
	}
}
void Boss::MakeEffect()
{
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(2);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 120.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	m_effectEmitter->Play();
}

void Boss::MakeEffectDown()
{
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(3);
	//エフェクトの大きさを設定する。
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
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(11);
	//エフェクトの大きさを設定する。
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
	//モデルを描画する。
	m_modelRender.Draw(rc);
}
