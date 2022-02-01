#pragma once
class nsK2EngineEffectEmitter;
class Player;

/// <summary>
/// 失敗魔法。
/// </summary>
class Miss : public IGameObject
{

public:
	Miss();
	~Miss();
	bool Start();
	void Update();
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
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
	/// <summary>
	/// 術者を設定する。
	/// </summary>
	/// <param name="enMagician">術者。</param>
	
private:
	Vector3					m_position;							//座標。
	Quaternion				m_rotation;							//回転。
	Vector3					m_scale = Vector3::One;				//大きさ。
	Vector3					m_moveSpeed;						//移動速度。
	float					m_timer = 0.0f;						//タイマー。
	EffectEmitter*          m_effectEmitter = nullptr;
};
