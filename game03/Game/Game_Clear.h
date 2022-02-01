#pragma once
class Fade;

/// <summary>
/// �Q�[���N���A�B
/// </summary>
class Game_Clear: public IGameObject
{
public:
	Game_Clear();
	~Game_Clear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender;					//�Q�[���N���A�̉摜�B
	float					m_alpha = 0.0f;					//pressbutton�̃��l�B
	SpriteRender			m_pressButton;					//pressbutton�̉摜�B
	SoundSource*            m_bgm = nullptr;				//�Q�[���N���A��BGM�B
	Fade*                   m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
};


