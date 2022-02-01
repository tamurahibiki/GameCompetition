#pragma once
class Fade;

/// <summary>
/// �Q�[���I�[�o�[�B
/// </summary>
class Game_Over : public IGameObject
{
public:
	Game_Over();
	~Game_Over();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender;					//�Q�[���I�[�o�[�̉摜�B
	float					m_alpha = 0.0f;					//pressbutton�̃��l�B
	SpriteRender			m_pressButton;					//pressbutton�̉摜�B
	SoundSource*            m_bgm = nullptr;				//�Q�[���I�[�o�[��BGM�B
	Fade*                   m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
};

