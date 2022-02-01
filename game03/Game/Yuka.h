#pragma once
class Yuka :public IGameObject
{
public:
	Yuka();
	~Yuka();
	bool Start();
	void Updata();
	void Render(RenderContext& rc);
	//座標
	void SetPosition(const Vector3& position)
	{
		m_position = position;
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

	ModelRender           m_modelRender;             //モデルレンダ―
	Vector3               m_position;                //座標
	Vector3               m_scale;                   //大きさ
	Quaternion            m_rotation;                //回転
	PhysicsStaticObject   m_physicsStaticObject;     //静的物理オブジェクト
};