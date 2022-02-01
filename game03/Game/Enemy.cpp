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
	//アニメーションを読み込む。
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
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/enemy.tkm",m_animationClips, enAnimationClip_Num);

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
		40.0f,			//半径。
		100.0f,			//高さ。
		m_position		//座標。
	);

	//エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/efk/down_enemy.efk");

	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/hitD.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/hit2.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/attack_enemy.wav");
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

void Enemy::Update()
{
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


void Enemy::Rotation()
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

void Enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
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
	//以下のステートのとき
	if (m_enemyState == enEnemyState_Damage ||
		m_enemyState == enEnemyState_Down)
	{
		//何も起こらない
		return;
	}
	//炎魔法に当たった時
	const auto& collision1 = g_collisionObjectManager->FindCollisionObjects("player_fireball");
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
			else 
			{
				Damage();
			}
			
			return;
		}
	}
	//風魔法に当たった時
	const auto& collision2 = g_collisionObjectManager->FindCollisionObjects("player_Ear");
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
			else 
			{
				Damage();
			}
			
			return;
		}
	}
	//足止め魔法に当たった時
	const auto& collision3 = g_collisionObjectManager->FindCollisionObjects("player_Back");
	for (auto collision : collision3)
	{
		if (collision->IsHit(m_charaCon))
		{
			//エネミーの移動速度を0にする。
					m_moveSpeed.x = 0.0f;
					m_moveSpeed.z = 0.0f;
		}
		return;
	}
}

void Enemy::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
		//音楽を再生する
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(14);
		se->Play(false);
		se->SetVolume(0.05f);
	}
}
void Enemy::MakeAttackCollision()
{
	//コリジョンを作成する。
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
		//攻撃できない距離なら
		if (IsCanAttack()==false)
		{
			diff.Normalize();
			m_moveSpeed = diff * 250.0f;
			//ステートを追跡にする
			m_enemyState = enEnemyState_Chase;
			return;
		}
		//攻撃できる距離なら
		else
		{
			int ram = rand() % 100;
			if (ram > 30)
			{
				//ステートを攻撃にする
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//ステートを待機にする。
				m_enemyState = enEnemyState_Idle;
			}
		}
	}
	else if (SearchMura() == true)
	{
		diff1.Normalize();
		
		//攻撃できない距離なら
		if (IsCanAttack2() == false)
		{
			m_moveSpeed = diff1 * 250.0f;
			//ステートを追跡にする
			m_enemyState = enEnemyState_Chase;
			return;
		}
		//攻撃できる距離なら
		else
		{
			int ram = rand() % 100;
			if (ram > 30)
			{
				//ステートを攻撃にする
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//ステートを待機にする。
				m_enemyState = enEnemyState_Idle;
			}
		}
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//タイマーを加算する。
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}
}


void Enemy::ProcessRunStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
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
	if (m_chaseTimer >= 1.5f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
	}
}

void Enemy::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//エフェクトを呼び出す
		MakeEffectDown();
		Game* game = FindGO<Game>("game");
		game->AddDefeatedEnemyNumber();
		//自身を削除する。
		DeleteGO(this);
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時。
	case enEnemyState_Damage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_Damage:
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy::MakeEffectDown()
{
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(5);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale(m_scale * 80.0f);
	Vector3 effectPosition = m_position;
	m_effectEmitter->SetPosition(effectPosition);
	//エフェクトを再生する
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
	//キーの名前が「attack_start」の時
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中にする
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃終了にする
		m_isUnderAttack = false;
	}
}

void Enemy::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
}