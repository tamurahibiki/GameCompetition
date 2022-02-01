#pragma once
class nsK2EngineCollisionObject;
class nsK2EngineEffectEmitter;

/// <summary>
/// ���~�ߖ��@�B
/// </summary>
class Back : public IGameObject
{
public:
	//���~�ߖ��@���������p�ҁB
	enum EnMagician {
		enMagician_None,
		enMagician_Player,			//�v���C���[�B
		
	};
public:
	Back();
	~Back();
	bool Start();
	void Update();
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	void SetPosition2(const Vector3& position)
	{
		m_position2 = position;
	}

	/// <summary>
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
	/// <summary>
	/// �p�҂�ݒ肷��B
	/// </summary>
	/// <param name="enMagician">�p�ҁB</param>
	void SetEnMagician(const EnMagician enMagician)
	{
		m_enMagician = enMagician;
	}
private:
	Vector3					m_position;							//���W�B
	Vector3					m_position2;						//���W�B
	Quaternion				m_rotation;							//��]�B
	Vector3					m_scale = Vector3::One;				//�傫���B
	Vector3					m_moveSpeed;						//�ړ����x�B
	EnMagician				m_enMagician = enMagician_None;		//�p�ҁB
	float					m_timer = 0.0f;						//�^�C�}�[�B
	CollisionObject* m_collisionObject = nullptr;   //CollisionObject�B
	EffectEmitter* m_effectEmitter = nullptr;
};


