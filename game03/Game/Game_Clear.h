#pragma once
class Fade;

/// <summary>
/// ゲームクリア。
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
	SpriteRender			m_spriteRender;					//ゲームクリアの画像。
	float					m_alpha = 0.0f;					//pressbuttonのα値。
	SpriteRender			m_pressButton;					//pressbuttonの画像。
	SoundSource*            m_bgm = nullptr;				//ゲームクリアのBGM。
	Fade*                   m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
};


