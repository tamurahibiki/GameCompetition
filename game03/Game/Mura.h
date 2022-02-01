#pragma once
class Player;

class Game;
using namespace std;

class Mura :public IGameObject
{
public:
	enum EnMuraState {
		enMuraState_Idle,      //�ҋ@�B
		enMuraState_Damage     //�_���[�W�B        
	};
public:
	Mura();
	~Mura();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Collision();
	//���W
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
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
	//�X�e�[�g
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
	ModelRender            m_modelRender;                       //���f�������_�[
	Vector3                m_position;                          //���W
	Vector3                m_scale;                             //�傫��
	Quaternion             m_rotation;                          //��]
	Game*                  m_game = nullptr;
	PhysicsStaticObject    m_physicsStaticObject;
	CollisionObject*       m_collisionObject = nullptr;         //�R���W�����I�u�W�F�N�g
	EnMuraState		       m_MuraState = enMuraState_Idle;	    //���X�e�[�g

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


