#pragma once
class Fade;

/// <summary>
/// ゲームオーバー。
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
	SpriteRender			m_spriteRender;					//ゲームオーバーの画像。
	float					m_alpha = 0.0f;					//pressbuttonのα値。
	SpriteRender			m_pressButton;					//pressbuttonの画像。
	SoundSource*            m_bgm = nullptr;				//ゲームオーバーのBGM。
	Fade*                   m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
};

