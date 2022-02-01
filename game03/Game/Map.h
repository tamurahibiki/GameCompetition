#pragma once
class Player;
class Enemy;

class Map : public IGameObject
{
public:
	Map();
	~Map();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ��B
	/// </summary>
	/// <param name="worldCenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W�B</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W�B</param>
	/// <param name="mapPosition">�ϊ�������̃}�b�v���W�B</param>
	/// <returns>�}�b�v�ɕ\��������W�Ȃ�true�B</returns>
	const bool WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition);
	SpriteRender				m_spriteRender;
	SpriteRender				m_playerSprite;
	SpriteRender				m_enemySprite;

	
	Player*                     m_player = nullptr;
	Enemy*                      m_enemy = nullptr;
	bool						m_isImage = false;
};

