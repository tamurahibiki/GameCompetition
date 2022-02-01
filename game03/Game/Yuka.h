#pragma once
class Yuka :public IGameObject
{
public:
	Yuka();
	~Yuka();
	bool Start();
	void Updata();
	void Render(RenderContext& rc);
	//���W
	void SetPosition(const Vector3& position)
	{
		m_position = position;
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

	ModelRender           m_modelRender;             //���f�������_�\
	Vector3               m_position;                //���W
	Vector3               m_scale;                   //�傫��
	Quaternion            m_rotation;                //��]
	PhysicsStaticObject   m_physicsStaticObject;     //�ÓI�����I�u�W�F�N�g
};