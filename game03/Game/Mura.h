#pragma once
class Player;

class Game;
using namespace std;

class Mura :public IGameObject
{
public:
	enum EnMuraState {
		enMuraState_Idle,      //待機。
		enMuraState_Damage     //ダメージ。        
	};
public:
	Mura();
	~Mura();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Collision();
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
	//ステート
	void ManageState();

	
	SpriteRender		   m_waku;
	SpriteRender		   m_waku2;
	SpriteRender		   m_waku3;
	SpriteRender		   m_waku4;
	
	wstring				   m_text;

	FontRender             fontRender1;
	FontRender             fontRender2;
	FontRender             fontRender3;
	FontRender             fontRender4;
	FontRender             fontRender5;

	int                    m_hp = 0;
	ModelRender            m_modelRender;                       //モデルレンダー
	Vector3                m_position;                          //座標
	Vector3                m_scale;                             //大きさ
	Quaternion             m_rotation;                          //回転
	Game*                  m_game = nullptr;
	PhysicsStaticObject    m_physicsStaticObject;
	CollisionObject*       m_collisionObject = nullptr;         //コリジョンオブジェクト
	EnMuraState		       m_MuraState = enMuraState_Idle;	    //村ステート

	float                   m_timer  = 0.0f;
	float                   m_timer2 = 0.0f;
	float                   m_timer3 = 0.0f;
	float                   m_timer4 = 0.0f;
	float                   m_timer5 = 0.0f;
	float                   m_timer6 = 0.0f;
	float                   m_timer7 = 0.0f;
	float                   m_timer8 = 0.0f;
	float                   m_timer9 = 0.0f;
};


